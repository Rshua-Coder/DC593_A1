#include "debug.h"
#include "PrjCfg.h"
#include "SysCfg.h"
#include "GxRect.h"
#include "GxDisplay.h"
#include "UIDisplay.h"
//#include "UIView.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIDisp
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#if (OSD_USE_ROTATE_BUFFER == ENABLE)
static UINT32 gbOsdRotate = 0;
static DC LocalDC;
static DC* pPaintDC = &LocalDC;
static DC* pShowDC = 0;
#endif
extern BOOL g_bDualUI;

BOOL UIDisplay_GetCleanBuf(UINT32* pAddr, UINT32* pSize)
{
    //if all OSD buffers are continuous memory block, fill current addr & size
    //UINT32* pAddr = (UINT32*)param1;
    //UINT32* pSize = (UINT32*)param2;
    UINT32 uiBufAddr;
    uiBufAddr = OS_GetMempoolAddr(POOL_ID_DISP_OSD1);
    *pAddr = uiBufAddr;
    *pSize = OS_GetMempoolSize(POOL_ID_DISP_OSD1)*4;
    return TRUE;
}

void UIDisplay_Init(UINT8 iDD, BOOL bClear, ISIZE* pDeviceSize)
{
    RESULT r;
    LAYER_INIT LayerInit;

    UINT32 uiBufAddr;
    //fixed buffer size
    UINT32 osd_w = OSD_W;
    UINT32 osd_h = OSD_H;
    //fixed buffer size
    UINT32 vdo_w = OSD_W;
    UINT32 vdo_h = OSD_H;
    //ISIZE DeviceSize;
    LAYER_INIT* pLayerInit = (LAYER_INIT*)&LayerInit; //layer init parameter
    //DeviceSize = GxVideo_GetDeviceSize(DOUT1);
    ISIZE DeviceSize = pDeviceSize[0];

    DBG_MSG("DOUT=%d\n\r",iDD>>4);
    switch(iDD & 0x0f) //check layer
    {
    case LAYER_OSD1:

        uiBufAddr = OS_GetMempoolAddr(POOL_ID_DISP_OSD1);

        pLayerInit->uiType = TYPE_FB;
        pLayerInit->uiPxlfmt = DISPLAY_OSD_FMT;
        pLayerInit->uiWidth = osd_w;
        pLayerInit->uiHeight = osd_h;
        #if (OSD_USE_DOUBLE_BUFFER == ENABLE)
        pLayerInit->uiBufCount = 1;
        pLayerInit->uiSwapEffect = SWAPEFFECT_COPY;
        #else
        pLayerInit->uiBufCount = 0;
        pLayerInit->uiSwapEffect = SWAPEFFECT_DISCARD;
        #endif
        switch(pLayerInit->uiPxlfmt)
        {
        case PXLFMT_INDEX1: pLayerInit->uiBufSize = (osd_w*osd_h)>>3;break;
        case PXLFMT_INDEX2: pLayerInit->uiBufSize = (osd_w*osd_h)>>2;break;
        case PXLFMT_INDEX4: pLayerInit->uiBufSize = (osd_w*osd_h)>>1;break;
        case PXLFMT_INDEX8: pLayerInit->uiBufSize = (osd_w*osd_h)>>0;break;
        }
        pLayerInit->pBufAddr[0] = uiBufAddr;
        #if (OSD_USE_DOUBLE_BUFFER == ENABLE)
        pLayerInit->pBufAddr[1] = uiBufAddr + pLayerInit->uiBufSize;
        #else
        pLayerInit->pBufAddr[1] = 0;
        #endif
        pLayerInit->pBufAddr[2] = 0;
        //dynamic window size
        pLayerInit->win.x= 0;
        pLayerInit->win.y = 0;
        pLayerInit->win.w = DeviceSize.w;
        pLayerInit->win.h = DeviceSize.h;
        pLayerInit->uiWinAttr = 0;

        #if (OSD_USE_ROTATE_BUFFER == ENABLE)
        if(gbOsdRotate == 1)
        {
        DBG_MSG("^YEnable Rotate\r\n");
        //prepare show DC
        UINT32 buf_w = pLayerInit->uiWidth;
        UINT32 buf_h = pLayerInit->uiHeight;
        UINT32 win_w = pLayerInit->win.w;
        UINT32 win_h = pLayerInit->win.h;

				
        //pLayerInit->uiWidth = buf_h;//drh 
        //pLayerInit->uiHeight = buf_w;
        
        pLayerInit->uiWidth = buf_w; //drh for test
        pLayerInit->uiHeight = buf_h;

		pLayerInit->uiBufCount = 0;
        pLayerInit->uiSwapEffect = SWAPEFFECT_DISCARD;
        pLayerInit->pBufAddr[0] = uiBufAddr;
        pLayerInit->pBufAddr[1] = 0;
        pLayerInit->pBufAddr[2] = 0;
				
        //pLayerInit->win.w = win_h;//drh 
        //pLayerInit->win.h = win_w;

		pLayerInit->win.w = win_w;//drh for test
        pLayerInit->win.h = win_h;		

        //prepare paint DC
        memset(pPaintDC, 0, sizeof(DC));
        GxGfx_AttachDC(pPaintDC, TYPE_FB,
            pLayerInit->uiPxlfmt, buf_w, buf_h, buf_w,
            (UINT8*)(uiBufAddr + pLayerInit->uiBufSize), 0, 0);
        }
        else
        {
        DBG_MSG("^YDisable Rotate\r\n");
        }
        #endif

        r = GxDisplay_InitLayer(iDD, &LayerInit, bClear);
        UI_DirtyDisplaySource(iDD); //must force flush to update new swap-buffer and new swap-effect
        DBG_MSG("OSD1 buf=%08x, size=%08x\n\r",LayerInit.pBufAddr[0], LayerInit.uiBufSize);
        DBG_MSG("OSD1 buf.w=%d, buf.h=%d;\n\r",LayerInit.uiWidth, LayerInit.uiHeight);
        DBG_MSG("OSD1 win.w=%d, win.h=%d;\n\r",LayerInit.win.w, LayerInit.win.h);


        break;

    case LAYER_OSD2:

        uiBufAddr = OS_GetMempoolAddr(POOL_ID_DISP_OSD2);

        pLayerInit->uiType = TYPE_FB;
        pLayerInit->uiPxlfmt = DISPLAY_OSD_FMT;
        pLayerInit->uiWidth = osd_w;
        pLayerInit->uiHeight = osd_h;
        #if (OSD2_USE_DOUBLE_BUFFER == ENABLE)
        pLayerInit->uiBufCount = 1;
        pLayerInit->uiSwapEffect = SWAPEFFECT_COPY;
        #else
        pLayerInit->uiBufCount = 0;
        pLayerInit->uiSwapEffect = SWAPEFFECT_DISCARD;
        #endif
        switch(pLayerInit->uiPxlfmt)
        {
        case PXLFMT_INDEX1: pLayerInit->uiBufSize = (osd_w*osd_h)>>3;break;
        case PXLFMT_INDEX2: pLayerInit->uiBufSize = (osd_w*osd_h)>>2;break;
        case PXLFMT_INDEX4: pLayerInit->uiBufSize = (osd_w*osd_h)>>1;break;
        case PXLFMT_INDEX8: pLayerInit->uiBufSize = (osd_w*osd_h)>>0;break;
        }
        pLayerInit->pBufAddr[0] = uiBufAddr;
        #if (OSD2_USE_DOUBLE_BUFFER == ENABLE)
        pLayerInit->pBufAddr[1] = uiBufAddr + pLayerInit->uiBufSize;
        #else
        pLayerInit->pBufAddr[1] = 0;
        #endif
        pLayerInit->pBufAddr[2] = 0;
        //dynamic window size
        pLayerInit->win.x= 0;
        pLayerInit->win.y = 0;
        pLayerInit->win.w = DeviceSize.w;
        pLayerInit->win.h = DeviceSize.h;
        pLayerInit->uiWinAttr = 0;

        r = GxDisplay_InitLayer(iDD, &LayerInit, bClear);
        DBG_MSG("OSD2 buf=%08x, size=%08x\n\r",LayerInit.pBufAddr[0], LayerInit.uiBufSize);
        DBG_MSG("OSD2 buf.w=%d, buf.h=%d;\n\r",LayerInit.uiWidth, LayerInit.uiHeight);
        DBG_MSG("OSD2 win.w=%d, win.h=%d;\n\r",LayerInit.win.w, LayerInit.win.h);

        break;

    case LAYER_VDO1:

        uiBufAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO1);

        pLayerInit->uiType = TYPE_FB;
        pLayerInit->uiPxlfmt = DISPLAY_VDO_FMT;
        pLayerInit->uiWidth = vdo_w;
        pLayerInit->uiHeight = vdo_h;
        #if (VDO_USE_DOUBLE_BUFFER == ENABLE)
        pLayerInit->uiBufCount = 1;
        pLayerInit->uiSwapEffect = SWAPEFFECT_COPY;
        #else
        pLayerInit->uiBufCount = 0;
        pLayerInit->uiSwapEffect = SWAPEFFECT_DISCARD;
        #endif
        switch(pLayerInit->uiPxlfmt)
        {
            case PXLFMT_YUV422:
            case PXLFMT_YUV422_PK:
                pLayerInit->uiBufSize = (vdo_w*vdo_h)*2 + 64;break;
                break;
            default: DBG_ERR("Pxlfmt %d",pLayerInit->uiPxlfmt);

        }
        pLayerInit->pBufAddr[0] = uiBufAddr;
        #if (VDO_USE_DOUBLE_BUFFER == ENABLE)
        pLayerInit->pBufAddr[1] = uiBufAddr + pLayerInit->uiBufSize;
        #else
        pLayerInit->pBufAddr[1] = 0;
        #endif
        pLayerInit->pBufAddr[2] = 0;
        //dynamic window size
        pLayerInit->win.x= 0;
        pLayerInit->win.y = 0;
        pLayerInit->win.w = DeviceSize.w;
        pLayerInit->win.h = DeviceSize.h;
        pLayerInit->uiWinAttr = 0;

        r = GxDisplay_InitLayer(iDD, &LayerInit, bClear);
        DBG_MSG("VDO1 buf=%08x, size=%08x\n\r",LayerInit.pBufAddr[0], LayerInit.uiBufSize);
        DBG_MSG("VDO1 buf.w=%d, buf.h=%d;\n\r",LayerInit.uiWidth, LayerInit.uiHeight);
        DBG_MSG("VDO1 win.w=%d, win.h=%d;\n\r",LayerInit.win.w, LayerInit.win.h);

        break;

    case LAYER_VDO2:

        uiBufAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO2);

        pLayerInit->uiType = TYPE_FB;
        pLayerInit->uiPxlfmt = DISPLAY_VDO_FMT;
        pLayerInit->uiWidth = vdo_w;
        pLayerInit->uiHeight = vdo_h;
        #if (VDO2_USE_DOUBLE_BUFFER == ENABLE)
        pLayerInit->uiBufCount = 1;
        pLayerInit->uiSwapEffect = SWAPEFFECT_COPY;
        #else
        pLayerInit->uiBufCount = 0;
        pLayerInit->uiSwapEffect = SWAPEFFECT_DISCARD;
        #endif
        switch(pLayerInit->uiPxlfmt)
        {
            case PXLFMT_YUV422:
            case PXLFMT_YUV422_PK: pLayerInit->uiBufSize = (vdo_w*vdo_h)*2 + 64;
                break;
            default: DBG_ERR("Pxlfmt %d",pLayerInit->uiPxlfmt);
        }
        pLayerInit->pBufAddr[0] = uiBufAddr;
        #if (VDO2_USE_DOUBLE_BUFFER == ENABLE)
        pLayerInit->pBufAddr[1] = uiBufAddr + pLayerInit->uiBufSize;
        #else
        pLayerInit->pBufAddr[1] = 0;
        #endif
        pLayerInit->pBufAddr[2] = 0;
        pLayerInit->win.x = 0;
        pLayerInit->win.y = 0;
        pLayerInit->win.w = DeviceSize.w;
        pLayerInit->win.h = DeviceSize.h;
        pLayerInit->uiWinAttr = 0;

        r = GxDisplay_InitLayer(iDD, &LayerInit, bClear);
        DBG_MSG("VDO2 buf1=%08x, size=%08x\n\r",LayerInit.pBufAddr[0], LayerInit.uiBufSize);
        DBG_MSG("VDO2 buf2=%08x, size=%08x\n\r",LayerInit.pBufAddr[1], LayerInit.uiBufSize);
        DBG_MSG("VDO2 buf.w=%d, buf.h=%d;\n\r",LayerInit.uiWidth, LayerInit.uiHeight);
        DBG_MSG("VDO2 win.w=%d, win.h=%d;\n\r",LayerInit.win.w, LayerInit.win.h);

        break;
    }
}

#if (OSD_USE_ROTATE_BUFFER == ENABLE)
void View_Window_ConfigAttr(UINT32 attr, UINT32 value)
{
    if(attr == 0)
        gbOsdRotate = value;
}

DC* View_Window_BeginRotate(DC* pDC)
{
    if(gbOsdRotate == 1)
    {
        GxDisplay_WaitVD();
        pShowDC = pDC;
        //GxGfx_CopyEx(pPaintDC, pShowDC, ROP_ROTATE|SRC_RCCW_90, 0); //copy show to paint with rotate CCW 90
        GxGfx_CopyEx(pPaintDC, pShowDC, ROP_ROTATE|SRC_RCCW_180, 0);//drh 20170927
        DBG_MSG("^YBegin Rotate\r\n");
        return pPaintDC;
    }
	else if(gbOsdRotate == 2){
		GxDisplay_WaitVD();
        pShowDC = pDC;
        GxGfx_CopyEx(pShowDC, pPaintDC, ROP_ROTATE|SRC_RCW_180, 0);//drh 20170927
        DBG_MSG("^YBegin Rotate\r\n");
        return pPaintDC;
	}
	
    return pDC;
}
void View_Window_EndRotate(void)
{
    if(gbOsdRotate == 1)
    {
        GxDisplay_WaitVD();
        //GxGfx_CopyEx(pShowDC, pPaintDC, ROP_ROTATE|SRC_RCW_90, 0); //copy paint to show with rotate CW 90
        GxGfx_CopyEx(pShowDC, pPaintDC, ROP_ROTATE|SRC_RCW_180, 0);//drh 20170927
        pShowDC = 0;
        DBG_MSG("^YEnd Rotate\r\n");
    }
	else if(gbOsdRotate == 2)	{
		GxDisplay_WaitVD();
        GxGfx_CopyEx(pPaintDC, pShowDC, ROP_ROTATE|SRC_RCCW_180, 0);//drh 20170927
        pShowDC = 0;
        DBG_MSG("^YEnd Rotate\r\n");
	}
}
#endif


BOOL bInitOSD1 = FALSE;
BOOL bInitOSD2 = FALSE;
BOOL bInitVDO1 = FALSE;
BOOL bInitVDO2 = FALSE;

//OSD1
BOOL View_Window(UINT32 cmd, UINT32 param1, UINT32 param2)
{
    if(cmd == SHOWCMD_BEGINDRAW)
    {
        BOOL bEn;
        UINT32 uiShowID;
        ISIZE DeviceSize;
        DeviceSize = GxVideo_GetDeviceSize(DOUT1);
        //restore to default (UI backgound)
        DBG_IND("^Gbegin OSD1 src=WINDOW\r\n");

        bEn = GxDisplay_Get(LAYER_OSD1, LAYER_STATE_ENABLE);
        uiShowID = (GxDisplay_Get(LAYER_OSD1, LAYER_STATE_INFO) & 0x000000FF) >> 0;
        if(bInitOSD1 && (uiShowID != 0))
        {
            //dummy flip for avoid flicker effect (double buffer fail when re-init layer)
            DC* pDC = GxDisplay_BeginDraw(LAYER_OSD1); //dummy draw for swap buffer
            GxDisplay_EndDraw(LAYER_OSD1, pDC);
            GxDisplay_Flip(FALSE);
        }
        UIDisplay_Init(LAYER_OSD1, FALSE, &DeviceSize); //attacth to UI source
        //if single buffert cannot SWAPEFFECT_COPY
        //GxDisplay_SetSwapEffect(iDD, SWAPEFFECT_COPY); //enable double buffer
        GxDisplay_Set(LAYER_OSD1, LAYER_STATE_ENABLE, bEn); //keep enable status before init layer

        if(g_bDualUI)
        {
        DeviceSize = GxVideo_GetDeviceSize(DOUT2);
        bEn = GxDisplay_Get(DOUT2|LAYER_OSD1, LAYER_STATE_ENABLE);
        uiShowID = (GxDisplay_Get(DOUT2|LAYER_OSD1, LAYER_STATE_INFO) & 0x000000FF) >> 0;
        if(bInitOSD1 && (uiShowID != 0))
        {
            //dummy flip for avoid flicker effect (double buffer fail when re-init layer)
            DC* pDC = GxDisplay_BeginDraw(DOUT2|LAYER_OSD1); //dummy draw for swap buffer
            GxDisplay_EndDraw(DOUT2|LAYER_OSD1, pDC);
            GxDisplay_FlipEx(DOUT2|LAYER_ALL, FALSE);
        }
        UIDisplay_Init(DOUT2|LAYER_OSD1, FALSE, &DeviceSize); //attacth to UI source
        GxDisplay_Set(DOUT2|LAYER_OSD1, LAYER_STATE_ENABLE, bEn); //keep enable status before init layer
        }

        bInitOSD1= TRUE;

        return TRUE;
    }
    if(cmd == SHOWCMD_ENDDRAW)
    {
        DBG_IND("^Gend OSD1 src=WINDOW\r\n");
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD1COMPARE, COMPARE_KEY_EQUAL); //enable OSD1 color key
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD1COLORKEY, 0); //set color key index
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD1ALPHA, OSD_OPAQUE_100); //set blending OSD1=100%
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD1BLEND, BLEND_PIXELALPHA); //OSD1 blending src
        //if(g_bDualUI)
        {
        GxDisplay_Set(DOUT2|LAYER_OUTPUT, CTRL_STATE_OSD1COMPARE, COMPARE_KEY_EQUAL); //enable OSD1 color key
        GxDisplay_Set(DOUT2|LAYER_OUTPUT, CTRL_STATE_OSD1COLORKEY, 0); //set color key index
        GxDisplay_Set(DOUT2|LAYER_OUTPUT, CTRL_STATE_OSD1ALPHA, OSD_OPAQUE_100); //set blending OSD1=100%
        GxDisplay_Set(DOUT2|LAYER_OUTPUT, CTRL_STATE_OSD1BLEND, BLEND_PIXELALPHA); //OSD1 blending src
        }
        return TRUE;
    }
    if(cmd == SHOWCMD_SETSIZE)
    {
        UINT32 DevID = param1;
        ISIZE *pSize = (ISIZE*)param2;
        DBG_IND("^Gresize OSD1 src=WINDOW\r\n");

        if(DevID == 0)
        {
        UIDisplay_Init(DOUT1|LAYER_OSD1, FALSE, pSize); //attacth to UI source
        GxDisplay_FlipEx(DOUT1|LAYER_OSD1, TRUE);
        }
        else if(DevID == 1)
        {
        UINT32 uiShowID;
        UINT32 uiShowID_2;
        GxDisplay_FlipEx(DOUT2|LAYER_OSD1, TRUE); //load palette?
        UIDisplay_Init(DOUT2|LAYER_OSD1, FALSE, pSize); //attacth to UI source
        GxDisplay_FlipEx(DOUT2|LAYER_OSD1, TRUE);

        //check Dev-1 OSD1 sync to Dev-0 OSD1?
        uiShowID = (GxDisplay_Get(LAYER_OSD1, LAYER_STATE_INFO) & 0x000000FF) >> 0;
        uiShowID_2 = (GxDisplay_Get(DOUT2|LAYER_OSD1, LAYER_STATE_INFO) & 0x000000FF) >> 0;
        if(uiShowID_2 != uiShowID)
        {
            //dummy flip for avoid Dev-1 OSD1 is not sync to Dev-0 OSD1
            DC* pDC = GxDisplay_BeginDraw(DOUT2|LAYER_OSD1); //dummy draw for swap buffer
            GxDisplay_EndDraw(DOUT2|LAYER_OSD1, pDC);
            debug_msg("^GDUMMY FLIP!!!\r\n");
            GxDisplay_FlipEx(DOUT2|LAYER_OSD1, TRUE);
        }
        }

        //Display_SetWindow(LAYER_OSD1, 0, 0, pSize->w, pSize->h); //change window size
        //DBG_MSG("OSD1 win.w=%d, win.h=%d;\n\r",pSize->w, pSize->h);
        return TRUE;
    }
    return FALSE;
}

//OSD2
BOOL View_Info(UINT32 cmd, UINT32 param1, UINT32 param2)
{
    if(cmd == SHOWCMD_BEGINDRAW)
    {
        BOOL bEn;
        UINT32 uiShowID;
        ISIZE DeviceSize;
        DeviceSize = GxVideo_GetDeviceSize(DOUT1);
        //restore to default (UI backgound)
        DBG_IND("^Gbegin OSD2 src=INFO\r\n");

        bEn = GxDisplay_Get(LAYER_OSD2, LAYER_STATE_ENABLE);
        uiShowID = (GxDisplay_Get(LAYER_OSD2, LAYER_STATE_INFO) & 0x000000FF) >> 0;
        if(bInitOSD2 && (uiShowID != 0))
        {
            //dummy flip for avoid flicker effect (double buffer fail when re-init layer)
            DC* pDC = GxDisplay_BeginDraw(LAYER_OSD2); //dummy draw for swap buffer
            GxDisplay_EndDraw(LAYER_OSD2, pDC);
            GxDisplay_Flip(FALSE);
        }
        UIDisplay_Init(LAYER_OSD2, FALSE, &DeviceSize); //attacth to UI source
        //if single buffert cannot SWAPEFFECT_COPY
        //GxDisplay_SetSwapEffect(iDD, SWAPEFFECT_COPY); //enable double buffer
        GxDisplay_Set(LAYER_OSD2, LAYER_STATE_ENABLE, bEn); //keep enable status before init layer
        bInitOSD2= TRUE;

        return TRUE;
    }
    if(cmd == SHOWCMD_ENDDRAW)
    {
        DBG_IND("^Gend OSD2 src=INFO\r\n");
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD2COMPARE, COMPARE_KEY_EQUAL); //enable OSD2 color key
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD2COLORKEY, 0); //set color key index
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD2ALPHA, OSD_OPAQUE_100); //set blending OSD2=100%
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD2BLEND, BLEND_PIXELALPHA); //OSD2 blending src
        return TRUE;
    }
    if(cmd == SHOWCMD_SETSIZE)
    {
        ISIZE *pSize = (ISIZE*)param2;
        DBG_IND("^Gresize OSD2 src=INFO\r\n");
        UIDisplay_Init(LAYER_OSD2, FALSE, pSize); //attacth to UI source
        GxDisplay_FlipEx(LAYER_OSD2, TRUE);
        //Display_SetWindow(LAYER_OSD2, 0, 0, pSize->w, pSize->h); //change window size
        //DBG_MSG("OSD2 win.w=%d, win.h=%d;\n\r",pSize->w, pSize->h);
        return TRUE;
    }
    return FALSE;
}

//VDO1


//VDO2
BOOL View_Background(UINT32 cmd, UINT32 param1, UINT32 param2)
{
    if(cmd == SHOWCMD_BEGINDRAW)
    {
        BOOL bEn;
        UINT32 uiShowID;
        ISIZE DeviceSize;
        DeviceSize = GxVideo_GetDeviceSize(DOUT1);
        //restore to default (UI backgound)
        DBG_IND("^Gbegin VDO2 src=BACKGROUND\r\n");

        bEn = GxDisplay_Get(LAYER_VDO2, LAYER_STATE_ENABLE);
        uiShowID = (GxDisplay_Get(LAYER_VDO2, LAYER_STATE_INFO) & 0x000000FF) >> 0;
        if(bInitVDO2 && (uiShowID != 0))
        {
            //dummy flip for avoid flicker effect (double buffer fail when re-init layer)
            DC* pDC = GxDisplay_BeginDraw(LAYER_VDO2);
            GxDisplay_EndDraw(LAYER_VDO2, pDC);
            GxDisplay_WaitVD();
            GxDisplay_Flip(FALSE);
            GxDisplay_WaitVD();
        }
        UIDisplay_Init(LAYER_VDO2, FALSE, &DeviceSize); //attacth to UI source
        GxDisplay_SetSwapEffect(LAYER_VDO2, SWAPEFFECT_COPY); //enable double buffer
        GxDisplay_Set(LAYER_VDO2, LAYER_STATE_ENABLE, bEn); //keep enable status before init layer
        bInitVDO2 = TRUE;

        return TRUE;
    }
    if(cmd == SHOWCMD_ENDDRAW)
    {
        DBG_IND("^Gend VDO2 src=BACKGROUND\r\n");
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COMPARE, COMPARE_KEY_NEVER);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COLORKEY, 0);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2ALPHA, OSD_OPAQUE_100);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2BLEND, BLEND_DISABLE);
        return TRUE;
    }
    if(cmd == SHOWCMD_SETSIZE)
    {
        ISIZE *pSize = (ISIZE*)param2;
        DBG_IND("^Gresize VDO2 src=BACKGROUND\r\n");
        UIDisplay_Init(LAYER_VDO2, FALSE, pSize); //attacth to UI source
        GxDisplay_FlipEx(LAYER_VDO2, TRUE);
        //Display_SetWindow(LAYER_VDO2, 0, 0, pSize->w, pSize->h); //change window size
        //DBG_MSG("VDO2 win.w=%d, win.h=%d;\n\r",pSize->w, pSize->h);
        return TRUE;
    }
    return FALSE;
}

BOOL View_Background_CK(UINT32 cmd, UINT32 param1, UINT32 param2)
{
    if(cmd == SHOWCMD_BEGINDRAW)
    {
        BOOL bEn;
        UINT32 uiShowID;
        ISIZE DeviceSize;
        DeviceSize = GxVideo_GetDeviceSize(DOUT1);
        //restore to default (UI backgound)
        DBG_IND("^Gbegin VDO2 src=BACKGROUND_CK\r\n");

        bEn = GxDisplay_Get(LAYER_VDO2, LAYER_STATE_ENABLE);
        uiShowID = (GxDisplay_Get(LAYER_VDO2, LAYER_STATE_INFO) & 0x000000FF) >> 0;
        if(bInitVDO2 && (uiShowID != 0))
        {
            //dummy flip for avoid flicker effect (double buffer fail when re-init layer)
            DC* pDC = GxDisplay_BeginDraw(LAYER_VDO2);
            GxDisplay_EndDraw(LAYER_VDO2, pDC);
            GxDisplay_WaitVD();
            GxDisplay_Flip(FALSE);
            GxDisplay_WaitVD();
        }
        UIDisplay_Init(LAYER_VDO2, FALSE, &DeviceSize); //attacth to UI source
        GxDisplay_SetSwapEffect(LAYER_VDO2, SWAPEFFECT_COPY); //enable double buffer
        GxDisplay_Set(LAYER_VDO2, LAYER_STATE_ENABLE, bEn); //keep enable status before init layer
        bInitVDO2 = TRUE;

        return TRUE;
    }
    if(cmd == SHOWCMD_ENDDRAW)
    {
        DBG_IND("^Gend VDO2 src=BACKGROUND_CK\r\n");
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COMPARE, COMPARE_KEY_EQUAL); //enable color key
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COLORKEY, VDO_TRANSPARENT_COLOR); //color key = VDO_TRANSPARENT_COLOR
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2ALPHA, OSD_OPAQUE_100);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2BLEND, BLEND_DISABLE);
        return TRUE;
    }
    if(cmd == SHOWCMD_SETSIZE)
    {
        ISIZE *pSize = (ISIZE*)param2;
        DBG_IND("^Gresize VDO2 src=BACKGROUND_CK\r\n");
        UIDisplay_Init(LAYER_VDO2, FALSE, pSize); //attacth to UI source
        GxDisplay_FlipEx(LAYER_VDO2, TRUE);
        //Display_SetWindow(LAYER_VDO2, 0, 0, pSize->w, pSize->h); //change window size
        //DBG_MSG("VDO2 win.w=%d, win.h=%d;\n\r",pSize->w, pSize->h);
        return TRUE;
    }
    return FALSE;
}






