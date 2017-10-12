
#include "PrjCfg.h"
#include "UIDisplay.h" //for DD_VDO
#include "GxImage.h"

#if _DEMO_TODO
#include "ImageAlgInfor.h" //for AlgMsgInfo
#include "UIPhotoObj.h" //for GetAlgMsgInfo
#include "ImgBorder.h" //for Photo frame
#include "PhotoDisplayTsk.h"
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
#include "PhotoTsk.h"
#include "Prv_PostProcessing.h"
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
#endif
#include "ImgCapCBMsg.h"
#include "GxDisplay.h"
#include "SizeConvert.h"
#include "DispSrvApi.h"
#include "UIInfo.h"
#include "UIPhotoInfo.h"
#include "SysCfg.h"

#define SWAP(t,x,y) do {t z;z=x;x=y;y=z;} while (0)

#define UIDebug_Show   debug_ind


typedef enum
{
    DISPLAY2_MODE_OFF = 0,
    DISPLAY2_MODE_QUICKVIEW = 1,
    DISPLAY2_MODE_PHOTOFRAME = 2
}   DISPLAY2_MODE;

//-----------------------------------------------------------------------
// function prototype description
//-----------------------------------------------------------------------
#if 0
extern ER   PhotoDisplay2_Open(PPHOTODISPLAY_APPOBJ pPhotoDisplayObj);
extern ER   PhotoDisplay2_Close(void);
extern BOOL PhotoDisplay2_WaitIdle(BOOL bWaitInfinite);
extern void PhotoDisplay2_SetMode(DISPLAY2_MODE Mode, BOOL bWaitInfinite);
//extern void PhotoDisplay2_ChgSize(DisplaySizeInfor* SizeInfor, BOOL bWaitInfinite);
#endif

extern void Photo2_OnChangeSize(ISIZE *pSize, ISIZE *pOldSize);


typedef struct _PhotoBuffer
{
    UINT32 buf_Type;
    UINT32 buf_Format;
    UINT32 buf_w;
    UINT32 buf_h;
    UINT32 buf_Y_Addr;
    UINT32 buf_Cb_Addr;
    UINT32 buf_Cr_Addr;
    UINT32 buf_LineOffset;
}
PhotoBuffer;

IRECT gDispBufRect[2] = {0,0,1000,1000}; //virtual coordinate of buffer range
IRECT gDispWinRect[2] = {0,0,1000,1000}; //virtual coordinate of window range

DISPLAY2_MODE ModeDisp2 = DISPLAY2_MODE_OFF;

void p2_GetImageBuffer(PhotoBuffer* pBuf)
{
    switch(ModeDisp2)
    {
    case DISPLAY2_MODE_QUICKVIEW:
    {
#if 1//_DEMO_TODO
        extern IMG_CAP_QV_DATA gImgQvData;
        IMG_CAP_QV_DATA * pImgQvData = &gImgQvData;
        //AlgMsgInfo *pAlgMsgInfo = GetAlgMsgInfo();
        UIDebug_Show(("image buf=[QV]\r\n"));
        pBuf->buf_Type = 0;
        //#NT#2010/10/20#Niven Cho -begin
        //#NT#IDE Format incorrect
        switch(pImgQvData->ImgInfo.type)
        {
            case IPL_IMG_Y_PACK_UV422:
                pBuf->buf_Format = PXLFMT_YUV422_PK;
                break;
            case IPL_IMG_Y_PACK_UV420:
                pBuf->buf_Format = PXLFMT_YUV420_PK;
                break;
            default:
                UIDebug_Show(("unknown Qview Format:%d\r\n",pImgQvData->ImgInfo.type));
        }
        //#NT#2010/10/20#Niven Cho -end
        pBuf->buf_w = pImgQvData->ImgInfo.Ch[0].Width;
        pBuf->buf_h = pImgQvData->ImgInfo.Ch[0].Height;
        pBuf->buf_Y_Addr = pImgQvData->ImgInfo.PixelAddr[0];
        pBuf->buf_Cb_Addr = pImgQvData->ImgInfo.PixelAddr[1];
        pBuf->buf_Cr_Addr = pImgQvData->ImgInfo.PixelAddr[2];
        pBuf->buf_LineOffset = pImgQvData->ImgInfo.Ch[0].LineOfs;

        // debug dump
        #if 0
        {
            IMG_BUF ImgBuf;
            UINT32  LineOff[3];
            UINT32  PxlAddr[3];
            PxlAddr[0] = pBuf->buf_Y_Addr;
            PxlAddr[1] = pBuf->buf_Cb_Addr;
            PxlAddr[2] = pBuf->buf_Cr_Addr;
            LineOff[0] = pBuf->buf_LineOffset;
            LineOff[1] = pBuf->buf_LineOffset;
            LineOff[2] = pBuf->buf_LineOffset;
            GxImg_InitBufEx(&ImgBuf, pBuf->buf_w, pBuf->buf_h, GX_IMAGE_PIXEL_FMT_YUV422, LineOff, PxlAddr);
            GxImg_DumpBuf(&ImgBuf,TRUE);
        }
        #endif
#endif
        UIDebug_Show(("image w,h= %d,%d\r\n", pBuf->buf_w, pBuf->buf_h));
        UIDebug_Show(("image buf= %08x,%08x,%08x\r\n", pBuf->buf_Y_Addr, pBuf->buf_Cb_Addr, pBuf->buf_Cr_Addr));
        UIDebug_Show(("image loff= %d\r\n", pBuf->buf_LineOffset));
    }
    break;
    case DISPLAY2_MODE_PHOTOFRAME:
    {
#if _DEMO_TODO
        AlgMsgInfo *pAlgMsgInfo = GetAlgMsgInfo();
        UIDebug_Show(("image buf=[PF]\r\n"));
        pBuf->buf_Type = 0;
        pBuf->buf_Format = COLOR_YCBCR422;
        pBuf->buf_w = gImageBorder.PrvBorderHsize;
        pBuf->buf_h = gImageBorder.PrvBorderVsize;
        //gPhotoFrameBuf = pAlgMsgInfo->IB_SRCBuffer;
        //gPhotoFrameBufSize = pAlgMsgInfo->IB_SRCBufferSize;
        pBuf->buf_Y_Addr = pAlgMsgInfo->IB_SRCBuffer;
        pBuf->buf_Cb_Addr = pBuf->buf_Y_Addr+gImageBorder.PrvBorderAddrCb-gImageBorder.PrvBorderAddrY;
        pBuf->buf_Cr_Addr = pBuf->buf_Cb_Addr+gImageBorder.PrvBorderAddrCr-gImageBorder.PrvBorderAddrCb;
        pBuf->buf_LineOffset = gImageBorder.PrvBorderHsize;
#endif
        UIDebug_Show(("image w,h= %d,%d\r\n", pBuf->buf_w, pBuf->buf_h));
        UIDebug_Show(("image buf= %08x,%08x,%08x\r\n", pBuf->buf_Y_Addr, pBuf->buf_Cb_Addr, pBuf->buf_Cr_Addr));
        UIDebug_Show(("image loff= %d\r\n", pBuf->buf_LineOffset));
    }
    break;
    default:
        break;
    }
}

//rect : buf range (unit: percentage of current buffer)
void p2_CalcImageBufferByRect(PhotoBuffer* pImg, IRECT rect)
{
    UINT32 buf_w = (pImg->buf_w*rect.w)/1000;
    UINT32 buf_h = (pImg->buf_h*rect.h)/1000;
    UINT32 buf_LineOffset = pImg->buf_LineOffset;
    UINT32 buf_Y_Addr = pImg->buf_Y_Addr;
    UINT32 buf_Cb_Addr = pImg->buf_Cb_Addr;
    UINT32 buf_Cr_Addr = pImg->buf_Cr_Addr;
    UINT32 buf_AddrOffset = 0;
    UINT32 buf_XShift = 0, buf_YShift = 0;
    UIDebug_Show(("Buffer old Y,Cb,Cr = %08x,%08x,%08x\r\n", pImg->buf_Y_Addr, pImg->buf_Cb_Addr, pImg->buf_Cr_Addr));
    buf_XShift = (buf_LineOffset*rect.x)/1000;
    buf_YShift = buf_LineOffset*((pImg->buf_h*rect.y)/1000);
    UIDebug_Show(("Buffer shift x,y = %08x,%08x\r\n", buf_XShift, buf_YShift));
    buf_AddrOffset += buf_XShift + buf_YShift;
    switch(pImg->buf_Format)
    {
        case PXLFMT_YUV422_PK:
        case PXLFMT_YUV420_PK:
        default:
            buf_Y_Addr  += buf_AddrOffset;
            buf_Cb_Addr += buf_AddrOffset/2;
            buf_Cr_Addr += buf_AddrOffset/2;
            break;
    }
    pImg->buf_w = buf_w;
    pImg->buf_h = buf_h;
    pImg->buf_Y_Addr = buf_Y_Addr;
    pImg->buf_Cb_Addr = buf_Cb_Addr;
    pImg->buf_Cr_Addr = buf_Cr_Addr;
    UIDebug_Show(("Buffer new Y,Cb,Cr = %08x,%08x,%08x\r\n", pImg->buf_Y_Addr, pImg->buf_Cb_Addr, pImg->buf_Cr_Addr));
}

//rect : win range (unit: percentage of current window)
void p2_CalcWinByRect(IRECT* pWin, IRECT rect)
{
    //#NT#2012/12/05#Lincy Lin -begin
    //#NT#Fix quick view display bug
    #if 0
    INT32 iWinX = (pWin->w*rect.x)/1000;
    INT32 iWinW = (pWin->w*rect.w)/1000;
    INT32 iWinY = (pWin->h*rect.y)/1000;
    INT32 iWinH = (pWin->h*rect.h)/1000;
    #else
    INT32 iWinX = (pWin->x*rect.w)/1000;
    INT32 iWinW = (pWin->w*rect.w)/1000;
    INT32 iWinY = (pWin->y*rect.h)/1000;
    INT32 iWinH = (pWin->h*rect.h)/1000;
    #endif
    //#NT#2012/12/05#Lincy Lin -end
    pWin->x = iWinX;
    pWin->w = iWinW;
    pWin->y = iWinY;
    pWin->h = iWinH;
}

void p2_Show(UINT8 iDD, PhotoBuffer* pImg, IRECT* pWin)
{
    UINT32 LayerID = LAYER_VDO1;

    if(iDD < LAYER_VDO1)
        return;
    if(iDD == LAYER_VDO1) //VDO1
        LayerID = LAYER_VDO1;
    if(iDD == LAYER_VDO2) //VDO2
        LayerID = LAYER_VDO2;

    Display_SetBuffer(LayerID,
        pImg->buf_w,
        pImg->buf_h,
        pImg->buf_LineOffset,
        pImg->buf_Format,
        pImg->buf_Y_Addr,
        pImg->buf_Cb_Addr,
        pImg->buf_Cr_Addr);
    Display_SetWindow(LayerID,
        pWin->x,
        pWin->y,
        pWin->w,
        pWin->h);
}

void PhotoDisplay2_Update(UINT8 iDD, ISIZE *pSize)
{
    PhotoBuffer Img;
    IRECT win;
    #if(VDO_USE_ROTATE_BUFFER == ENABLE)
    IMG_BUF   DispImg = {0};
    IMG_BUF   DispOutImg = {0};
    UINT32    uiPoolAddr,uiPoolSize;
    UINT32    temp;

    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO2TEMP);
    uiPoolSize = OS_GetMempoolSize(POOL_ID_DISP_VDO2TEMP);
    #endif


    //buffer
    p2_GetImageBuffer(&Img);
    if((Img.buf_w == 0) || (Img.buf_h == 0) || (Img.buf_LineOffset == 0))
    {
        debug_err(("^RPhoto buffer error! Skip display update.\r\n"));
        return;
    }
    //window

    //#NT#2012/12/05#Lincy Lin -begin
    //#NT#Fix quick view display bug
    #if 0
    RECT_Set(&win, 0, 0, pSize->w, pSize->h);
    #else
    {

        SIZECONVERT_INFO     CovtInfo = {0};
        DISPSRV_CMD          DispCmd={0};
        DISPSRV_DISP_CFG     CfgDisp={0};
        USIZE                dev1Ratio={0};

        memset(&DispCmd,0,sizeof(DISPSRV_CMD));
        DispCmd.Idx = DISPSRV_CMD_IDX_GET_DISP;
        DispCmd.Prop.bExitCmdFinish = TRUE;
        DispCmd.Out.uiNumByte = sizeof(CfgDisp);
        DispCmd.Out.pData= &CfgDisp;
        DispSrv_Cmd(&DispCmd);
        dev1Ratio = CfgDisp.Desc[DISPSRV_DISP_IDX_PRIMARY].AspectRatio;

        CovtInfo.uiSrcWidth  = Img.buf_w;
        CovtInfo.uiSrcHeight = Img.buf_h;
        CovtInfo.uiDstWidth  = pSize->w;
        CovtInfo.uiDstHeight = pSize->h;
        CovtInfo.uiDstWRatio = dev1Ratio.w;
        CovtInfo.uiDstHRatio = dev1Ratio.h;
        CovtInfo.alignType = SIZECONVERT_ALIGN_FLOOR_2;
        DisplaySizeConvert(&CovtInfo);
        RECT_Set(&win, CovtInfo.uiOutX, CovtInfo.uiOutY, CovtInfo.uiOutWidth, CovtInfo.uiOutHeight);
    }
    #endif
    //#NT#2012/12/05#Lincy Lin -end

    //calculate buffer
    p2_CalcImageBufferByRect(&Img, gDispBufRect[iDD-LAYER_VDO1]);
    //calculate window
    p2_CalcWinByRect(&win, gDispWinRect[iDD-LAYER_VDO1]);

    UIDebug_Show(("Img w=%d,h=%d,format:%x\r\n",Img.buf_w,Img.buf_h,Img.buf_Format));
    UIDebug_Show(("win x=%d,y=%d,w=%d,h=%d\r\n",win.x,win.y,win.w,win.h));
    UIDebug_Show(("Y:0x%08x,Cb:0x%08x,Cr:0x%08x\r\n",Img.buf_Y_Addr,Img.buf_Cb_Addr,Img.buf_Cr_Addr));

    #if(VDO_USE_ROTATE_BUFFER == ENABLE)
    GxImg_InitBuf(&DispImg,Img.buf_w,Img.buf_h,GX_IMAGE_PIXEL_FMT_YUV420, GXIMAGE_LINEOFFSET_ALIGN(16),Img.buf_Y_Addr,Img.buf_w*Img.buf_h*2);
    GxImg_RotateData(&DispImg,uiPoolAddr,uiPoolSize,GX_IMAGE_ROTATE_90,&DispOutImg);
    // swap w/h
    SWAP(UINT32,win.w,win.h);
    // swap buffer w/h
    SWAP(UINT32,Img.buf_w,Img.buf_h);
    Img.buf_Y_Addr = DispOutImg.PxlAddr[0];
    Img.buf_Cb_Addr = DispOutImg.PxlAddr[1];
    Img.buf_LineOffset = Img.buf_w;
    // force format to 420_PK
    Img.buf_Format = PXLFMT_YUV420_PK;
    #endif

    //show buffer to display
    p2_Show(iDD, &Img, &win);
}

void Photo2_OnChangeSize(ISIZE *pSize, ISIZE *pOldSize)
{
    UIDebug_Show(("old size w,h=%d,%d\r\n", pOldSize->w, pOldSize->h));
    UIDebug_Show(("new size w,h=%d,%d\r\n", pSize->w, pSize->h));

    if(ModeDisp2 != DISPLAY2_MODE_OFF)
    {
        PhotoDisplay2_Update(LAYER_VDO2, pSize);
    }
}


//////////////////////////////////////////////////////////////////////////////

BOOL gPreviewWithDIS = FALSE;

void View_Preview_SetDIS(BOOL bEn)
{
    gPreviewWithDIS = bEn;
}
    /*
    NOTE:

    GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2BLEND, BLEND_PIXELALPHA);
    GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2ALPHA, OSD_OPAQUE_100);
    ==> 不透明度 = 255/256

    GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2BLEND, BLEND_DISABLE);
    GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2ALPHA, OSD_OPAQUE_100);
    ==> 不透明度 = 256/256
    */


//VDO1
BOOL View_Preview(UINT32 cmd, UINT32 param1, UINT32 param2)
{
    if(cmd == SHOWCMD_BEGINDRAW)
    {
        UIDebug_Show(("^Gbegin VDO1 src=PREVIEW\r\n"));
        GxDisplay_SetSwapEffect(LAYER_VDO1, SWAPEFFECT_DISCARD); //disable double buffer
        return TRUE;
    }
    if(cmd == SHOWCMD_SYNCDRAW)
    {
#if _MIPS_TODO
        //#NT#2010/05/24#Jeah Yen -FASTBOOT- -begin
        #if (WAITPHOTO_FUNCTION == ENABLE)
        if(bInitUI && !bFirstStable && g_bPreviewFirstTime)
        {
            //debug_err(("Wait IPL %d\r\n", Perf_GetCurrent()));
            InitIPL_WaitFinish();
            //debug_err(("Wait Lens %d\r\n", Perf_GetCurrent()));
            InitLens_WaitFinish();
            //debug_err(("Wait AE %d\r\n", Perf_GetCurrent()));
            Photo_WaitAEDisplayStable(TRUE); //InitAE_WaitFinish();
            DBG_IND("Show Preview %d\r\n", Perf_GetCurrent());
            idec_setV1En(IDE_ID_1, TRUE);
            //idec_setV2En(IDE_ID_1, FALSE);
            //UI_Show(UI_SHOW_BACKGND, FALSE); //VDO2
            bFirstStable = TRUE;
        }
        #endif
        //#NT#2010/05/24#Jeah Yen -FASTBOOT- -end
#endif
        return TRUE;
    }
    if(cmd == SHOWCMD_ENDDRAW)
    {
        UIDebug_Show(("^Gend VDO1 src=PREVIEW\r\n"));
#if _DEMO_TODO
        if((Prv_getPostProcessingStatus() != Prv_PostProcessing_ON))
        {
            //#NT#2010/12/16#Photon Lin -begin
            //#Enhance DIS flow
            if (gPreviewWithDIS)
            {
                PhotoDisplay_SetMode(DISPLAY_MODE_DISVIEW, FALSE);
            }
            else
            {
                PhotoDisplay_SetMode(DISPLAY_MODE_PREVIEW, FALSE); //attach to IPP source
            }
            //#NT#2010/12/16#Photon Lin -end
        }
#endif
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1COMPARE, COMPARE_KEY_NEVER);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1COLORKEY, 0);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1ALPHA, OSD_OPAQUE_100);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1BLEND, BLEND_DISABLE);
        return TRUE;
    }
    if(cmd == SHOWCMD_SETSIZE)
    {
        //ISIZE *pSize = (ISIZE*)param2;
        //Display_SetWindow(LAYER_VDO1, 0, 0, pSize->w, pSize->h); //change window size
        return TRUE;
    }
    return FALSE;
}

//VDO2
BOOL View_Quickview(UINT32 cmd, UINT32 param1, UINT32 param2)
{
    if(cmd == SHOWCMD_BEGINDRAW)
    {
        UIDebug_Show(("^Gbegin VDO2 src=QUICKVIEW\r\n"));
        GxDisplay_SetSwapEffect(LAYER_VDO2, SWAPEFFECT_DISCARD); //disable double buffer
        return TRUE;
    }
    if(cmd == SHOWCMD_ENDDRAW)
    {
        ISIZE             DeviceSize;
        UIDebug_Show(("^Gend VDO2 src=QUICKVIEW\r\n"));
        DeviceSize = GxVideo_GetDeviceSize(DOUT1);
        //set buffer & window
        RECT_Set(&(gDispBufRect[LAYER_VDO2-LAYER_VDO1]), 0,0,1000,1000); //buffer range
        RECT_Set(&(gDispWinRect[LAYER_VDO2-LAYER_VDO1]), 0,0,1000,1000); //window range
        ModeDisp2 = DISPLAY2_MODE_QUICKVIEW;
        PhotoDisplay2_Update(LAYER_VDO2, &DeviceSize);
        //set blending (alpha = 100, ck = 0)
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COMPARE, COMPARE_KEY_NEVER);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COLORKEY, 0);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2ALPHA, OSD_OPAQUE_100); //const alpha = 100%
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2BLEND, BLEND_DISABLE);
        return TRUE;
    }
    if(cmd == SHOWCMD_SETSIZE)
    {
        ISIZE *pSize = (ISIZE*)param2;
        //Display_SetWindow(LAYER_VDO1, 0, 0, pSize->w, pSize->h); //change window size
        Photo2_OnChangeSize(pSize, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL View_CS_Quickview(UINT32 cmd, UINT32 param1, UINT32 param2)
{
    if(cmd == SHOWCMD_BEGINDRAW)
    {
        UIDebug_Show(("^Gbegin VDO2 src=CS_QUICKVIEW\r\n"));
        GxDisplay_SetSwapEffect(LAYER_VDO2, SWAPEFFECT_DISCARD); //disable double buffer
        return TRUE;
    }
    if(cmd == SHOWCMD_ENDDRAW)
    {
        ISIZE             DeviceSize;
        UIDebug_Show(("^Gend VDO2 src=CS_QUICKVIEW\r\n"));
        DeviceSize = GxVideo_GetDeviceSize(DOUT1);
        //set buffer & window
        RECT_Set(&(gDispBufRect[LAYER_VDO2-2]), 0,0,500,1000); //buffer range
        RECT_Set(&(gDispWinRect[LAYER_VDO2-2]), 0,0,500,1000); //window range
        ModeDisp2 = DISPLAY2_MODE_QUICKVIEW;
        PhotoDisplay2_Update(LAYER_VDO2, &DeviceSize);
        //set blending (alpha = 50, ck = 0)
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COMPARE, COMPARE_KEY_NEVER);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COLORKEY, 0);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2ALPHA, OSD_OPAQUE_050); //const alpha = 50%
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2BLEND, BLEND_CONSTALPHA); //enable constant alpha
        return TRUE;
    }
    if(cmd == SHOWCMD_SETSIZE)
    {
        ISIZE *pSize = (ISIZE*)param2;
        //Display_SetWindow(LAYER_VDO1, 0, 0, pSize->w, pSize->h); //change window size
        Photo2_OnChangeSize(pSize, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL View_Guideview_L(UINT32 cmd, UINT32 param1, UINT32 param2)
{
    if(cmd == SHOWCMD_BEGINDRAW)
    {
        UIDebug_Show(("^Gbegin VDO2 src=GUIDEVIEW_L\r\n"));
        GxDisplay_SetSwapEffect(LAYER_VDO2, SWAPEFFECT_DISCARD); //disable double buffer
        return TRUE;
    }
    if(cmd == SHOWCMD_ENDDRAW)
    {
        ISIZE             DeviceSize;
        UIDebug_Show(("^Gend VDO2 src=GUIDEVIEW_L\r\n"));
        DeviceSize = GxVideo_GetDeviceSize(DOUT1);
        //set buffer & window
        RECT_Set(&(gDispBufRect[LAYER_VDO2-LAYER_VDO1]), 750,0,250,1000); //buffer range
        RECT_Set(&(gDispWinRect[LAYER_VDO2-LAYER_VDO1]), 0,0,250,1000); //window range
        ModeDisp2 = DISPLAY2_MODE_QUICKVIEW;
        PhotoDisplay2_Update(LAYER_VDO2, &DeviceSize);
        //set blending (alpha = 50, ck = 0)
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COMPARE, COMPARE_KEY_NEVER);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COLORKEY, 0);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2ALPHA, OSD_OPAQUE_050); //const alpha = 50%
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2BLEND, BLEND_CONSTALPHA); //enable constant alpha
        return TRUE;
    }
    if(cmd == SHOWCMD_SETSIZE)
    {
        ISIZE *pSize = (ISIZE*)param2;
        //Display_SetWindow(LAYER_VDO1, 0, 0, pSize->w, pSize->h); //change window size
        Photo2_OnChangeSize(pSize, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL View_Guideview_R(UINT32 cmd, UINT32 param1, UINT32 param2)
{
    if(cmd == SHOWCMD_BEGINDRAW)
    {
        UIDebug_Show(("^Gbegin VDO2 src=GUIDEVIEW_R\r\n"));
        GxDisplay_SetSwapEffect(LAYER_VDO2, SWAPEFFECT_DISCARD); //disable double buffer
        return TRUE;
    }
    if(cmd == SHOWCMD_ENDDRAW)
    {
        ISIZE             DeviceSize;
        UIDebug_Show(("^Gend VDO2 src=GUIDEVIEW_R\r\n"));
        DeviceSize = GxVideo_GetDeviceSize(DOUT1);
        //set buffer & window
        RECT_Set(&(gDispBufRect[LAYER_VDO2-LAYER_VDO1]), 0,0,250,1000); //buffer range
        RECT_Set(&(gDispWinRect[LAYER_VDO2-LAYER_VDO1]), 750,0,250,1000); //window range
        ModeDisp2 = DISPLAY2_MODE_QUICKVIEW;
        PhotoDisplay2_Update(LAYER_VDO2, &DeviceSize);
        //set blending (alpha = 50, ck = 0)
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COMPARE, COMPARE_KEY_NEVER);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COLORKEY, 0);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2ALPHA, OSD_OPAQUE_050); //const alpha = 50%
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2BLEND, BLEND_CONSTALPHA); //enable constant alpha
        return TRUE;
    }
    if(cmd == SHOWCMD_SETSIZE)
    {
        ISIZE *pSize = (ISIZE*)param2;
        //Display_SetWindow(LAYER_VDO1, 0, 0, pSize->w, pSize->h); //change window size
        Photo2_OnChangeSize(pSize, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL View_Photoframe(UINT32 cmd, UINT32 param1, UINT32 param2)
{
    if(cmd == SHOWCMD_BEGINDRAW)
    {
        UIDebug_Show(("^Gbegin VDO2 src=PHOTOFRAME\r\n"));
        GxDisplay_SetSwapEffect(LAYER_VDO2, SWAPEFFECT_DISCARD); //disable double buffer
        return TRUE;
    }
    if(cmd == SHOWCMD_ENDDRAW)
    {
        ISIZE             DeviceSize;
        UIDebug_Show(("^Gend VDO2 src=PHOTOFRAME\r\n"));
        DeviceSize = GxVideo_GetDeviceSize(DOUT1);
        //set buffer & window
        RECT_Set(&(gDispBufRect[LAYER_VDO2-LAYER_VDO1]), 0,0,1000,1000); //buffer range
        RECT_Set(&(gDispWinRect[LAYER_VDO2-LAYER_VDO1]), 0,0,1000,1000); //window range
        ModeDisp2 = DISPLAY2_MODE_PHOTOFRAME;
        PhotoDisplay2_Update(LAYER_VDO2, &DeviceSize);
        //set blending (alpha = 100, ck = color)
#if _DEMO_TODO
        #if (IB_COLOR_KEY == IB_CKEY_WHITE)
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COMPARE, COMPARE_KEY_GREATER); //enable color key
        #else
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COMPARE, COMPARE_KEY_LESS); //enable color key
        #endif
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COLORKEY, COLOR_MAKE_YUVD(gImageBorder.BorderColorKeyY,gImageBorder.BorderColorKeyCb,gImageBorder.BorderColorKeyCr)); //color key = border config color
#endif
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2ALPHA, OSD_OPAQUE_100);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2BLEND, BLEND_DISABLE);
        return TRUE;
    }
    if(cmd == SHOWCMD_SETSIZE)
    {
        ISIZE *pSize = (ISIZE*)param2;
        //Display_SetWindow(LAYER_VDO1, 0, 0, pSize->w, pSize->h); //change window size
        Photo2_OnChangeSize(pSize, 0);
        return TRUE;
    }
    return FALSE;
}


