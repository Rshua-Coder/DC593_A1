#include "Type.h"
#include "GxSystem.h"
#include "GxDisplay.h"
#include "SysCfg.h"
#include "UIAppPlay.h"

////////////////////////////////////////////////////////////////////////////////////////////
//#NT#2010/03/05#Jeah Yen -begin
#include "GxGfx.h"
#include "GxImageJPG.h"
//#NT#2010/03/05#Jeah Yen -end
#include "GxImage.h"
#include "UIPlaySlide.h"
#if _DEMO_TODO
#include "SlideEffect.h"
#include "SliderPreview.h"
#endif


///////////////////////////////////////////////////////////////////////


//#NT#2010/01/14#Steven feng -begin
#define UIBASE_W 640
//#NT#2010/01/14#Steven feng -end

//#NT#2010/01/28#JeahYen -begin
UINT32 gPlaySSP_full_w = 0;
UINT32 gPlaySSP_full_h = 0;

UINT32 gPlaySSP_buf_w = 0;
UINT32 gPlaySSP_buf_h = 0;
UINT32 gPlaySSP_buf_a1 = 0;
UINT32 gPlaySSP_buf_a2 = 0;
UINT32 gPlaySSP_win_x = 0;
UINT32 gPlaySSP_win_y = 0;
UINT32 gPlaySSP_win_w = 0;
UINT32 gPlaySSP_win_h = 0;

IMAGE* gPlaySSP_Img1 = 0;
IMAGE* gPlaySSP_Img2 = 0;
//#NT#2011/11/01#Lincy Lin -begin
//#NT#650GxImage
IMG_BUF gPlaySSImgBuf1 = {0};
IMG_BUF gPlaySSImgBuf2 = {0};
//#NT#2011/11/01#Lincy Lin -end

UINT32 gPlaySSP_iEffect = 0;

void UISlide_OpenView(UIPlay_SlideView* pViewCfg, BOOL bOpenFromPB)
{
    gPlaySSP_full_w = pViewCfg->scr_w;
    gPlaySSP_full_h = pViewCfg->scr_h;
    gPlaySSP_win_x = pViewCfg->x;
    gPlaySSP_win_y = pViewCfg->y;
    gPlaySSP_win_w = pViewCfg->w;
    gPlaySSP_win_h = pViewCfg->h;
    gPlaySSP_Img1 = pViewCfg->pMyImg1;
    gPlaySSP_Img2 = pViewCfg->pMyImg2;
    if(bOpenFromPB)
    {
        //#NT#2010/02/10#Ben Wang -begin
        //#NT#fix the bug(F0407) that press "start" to slideshow will display the previous frame buffer
        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYSINGLE, 1, PB_SINGLE_CURR);
        //#NT#2010/02/10#Ben Wang -end
    }
}

void UISlide_CloseView(BOOL bReturnToPB)
{
    if(bReturnToPB)
    {
        //#NT#2010/03/03#Scottie -begin
        //#NT#Need to re-play current image to avoid using wrong IDE setting
        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYSINGLE, 2, PB_SINGLE_CURR, 1);
        //#NT#2010/03/03#Scottie -end
    }
#if _DEMO_TODO
    PB_PlaySliderPreviewUnInit();
#endif
}

void UISlide_OnChangeSize(ISIZE *pSize, ISIZE *pOldSize)
{
#if _DEMO_TODO

    UINT32 win_x, win_y, win_w, win_h;

    win_x = (gPlaySSP_win_x*pSize->w/gPlaySSP_full_w+3)&0xFFFFFFFC;
    win_y = (gPlaySSP_win_y*pSize->h/gPlaySSP_full_h);
    win_w = (gPlaySSP_win_w*pSize->w/gPlaySSP_full_w+15)&0xFFFFFFF0;
    win_h = (gPlaySSP_win_h*pSize->h/gPlaySSP_full_h);
    //debug_msg("OSD x,y,w,h=(%ld,%ld,%ld,%ld)\r\n", gPlaySSP_win_x, gPlaySSP_win_y, gPlaySSP_win_w, gPlaySSP_win_h);
    //debug_msg("VDO x,y,w,h=(%ld,%ld,%ld,%ld)\r\n", win_x, win_y, win_w, win_h);

    //SlideShow Preview Start
    //#NT#2010/01/27#Jeah Yen -begin
    PB_PlaySliderPreviewInit(gPlaySSP_buf_w,gPlaySSP_buf_h,win_x,win_y,win_w,win_h);
    //#NT#2010/01/27#Jeah Yen -end

    if(gPlaySSP_iEffect == PBX_SLIDE_EFFECT_NONE)
        //PB_PlaySliderPreview(gPlaySSP_buf_a1, gPlaySSP_buf_a2, gPlaySSP_iEffect);
        PB_PlaySliderPreview(&gPlaySSImgBuf1, &gPlaySSImgBuf2, gPlaySSP_iEffect);
    else
        //PB_PlaySliderPreview(gPlaySSP_buf_a2, gPlaySSP_buf_a1, gPlaySSP_iEffect);
        PB_PlaySliderPreview(&gPlaySSImgBuf2, &gPlaySSImgBuf1, gPlaySSP_iEffect);
#endif
}

void UISlide_DrawPreview(INT32 iEffect)
{
#if _DEMO_TODO

    UINT32 win_x, win_y, win_w, win_h;

    //#NT#2010/01/27#Jeah Yen -begin
    //#NT#2010/01/20#Jeah Yen -begin
    //while under display change device or change mode flow,
    //the function GxVideo_GetDevice() maybe return 0.
    //#NT#2010/01/20#Jeah Yen -end
    if(GxVideo_GetDevice(DOUT1) == gDevLCDObj)
    {
        ISIZE             BufferSize;
        BufferSize = GxVideo_GetDeviceSize(DOUT1);
        //#NT#2010/01/14#Steven feng -begin
        //It seems we do not do alignment and adjust ratio for LCD buffer width is changed
        // from 640 to 960
        //win_x = (gPlaySSP_win_x+3)&0xFFFFFFFC;
        win_x = (gPlaySSP_win_x*BufferSize.w)/UIBASE_W;
        //#NT#2010/01/14#Steven feng -end

        win_y = (gPlaySSP_win_y);

        //#NT#2010/01/14#Steven feng -begin
        //It seems we do not do alignment and adjust ratio for LCD buffer width is changed
        // from 640 to 960
        //win_w = (gPlaySSP_win_w+15)&0xFFFFFFF0;
        win_w = (gPlaySSP_win_w*BufferSize.w)/UIBASE_W;
        //#NT#2010/01/14#Steven feng -end

        win_h = (gPlaySSP_win_h);
    }
    else //gDevTVObj
    {
        //#NT#2010/09/28#JeahYen -begin
        ISIZE BufferSize;
        //#NT#2010/09/28#JeahYen -end
        //#NT#2010/09/28#JeahYen -begin
        BufferSize = Play_GetBufferSize();
        //#NT#2010/09/28#JeahYen -end
        //scaling display VDO window range by ratio of TV_size/LCD_size
        win_x = (gPlaySSP_win_x*BufferSize.w/gPlaySSP_full_w+3)&0xFFFFFFFC;
        win_y = (gPlaySSP_win_y*BufferSize.h/gPlaySSP_full_h);
        win_w = (gPlaySSP_win_w*BufferSize.w/gPlaySSP_full_w+15)&0xFFFFFFF0;
        win_h = (gPlaySSP_win_h*BufferSize.h/gPlaySSP_full_h);
    }
    //debug_msg("OSD x,y,w,h=(%ld,%ld,%ld,%ld)\r\n", gPlaySSP_win_x, gPlaySSP_win_y, gPlaySSP_win_w, gPlaySSP_win_h);
    //debug_msg("VDO x,y,w,h=(%ld,%ld,%ld,%ld)\r\n", win_x, win_y, win_w, win_h);
    //#NT#2010/01/27#Jeah Yen -end

    //debug_msg("BUF w,h=(%ld,%ld)\r\n", gPlaySSP_buf_w,gPlaySSP_buf_h);
    //SlideShow Preview Start
    //#NT#2010/01/27#Jeah Yen -begin
    PB_PlaySliderPreviewInit(gPlaySSP_buf_w,gPlaySSP_buf_h,win_x,win_y,win_w,win_h);
    //#NT#2010/01/27#Jeah Yen -end

    if(iEffect == PBX_SLIDE_EFFECT_NONE)
        //PB_PlaySliderPreview(gPlaySSP_buf_a1, gPlaySSP_buf_a2, iEffect);
        PB_PlaySliderPreview(&gPlaySSImgBuf1, &gPlaySSImgBuf2, iEffect);
    else
        //PB_PlaySliderPreview(gPlaySSP_buf_a2, gPlaySSP_buf_a1, iEffect);
        PB_PlaySliderPreview(&gPlaySSImgBuf2, &gPlaySSImgBuf1, iEffect);

    gPlaySSP_iEffect = iEffect;
#endif
}
//#NT#2010/01/28#JeahYen -end


//#NT#2010/02/01#Jeah Yen -begin
typedef struct _RAW_IMAGE
{
    UINT16 w,h,pitch;
    UINT16 fmt;
    UINT32 address;
    UINT32 size;
}
RAW_IMAGE;

#define MAKE_ALIGN(s, bytes)    ((((UINT32)(s))+((bytes)-1)) & ~((UINT32)((bytes)-1)))

void _UISlide_DecodeJPG(IMAGE* pImage, RAW_IMAGE* pRaw, PIMG_BUF pOutImgBuf)
{
#if _DEMO_TODO

    //#NT#2011/11/01#Lincy Lin -begin
    //#NT#
    JPG_DEC_INFO     DecJPG={0};
    JPGHEAD_DEC_CFG JDCParams={0};
    //#NT#2011/11/01#Lincy Lin -end

    //UINT32 uiWidth,uiHeight,uiPitch,uiOffset;
    UINT32 uiPitch,uiOffset;
    UINT32 buf_w=0,buf_h=0;

    //UINT32 old_y=0,old_cb=0,old_cr=0;
    //UINT32 new_y=0,new_cb=0,new_cr=0;
    //INT32 diff=0;
    //#NT#2011/02/15#Ben Wang -begin
    //#NT#Remove warning of PCLint
    //uiWidth   = pImage->dc.uiReserved[4*3+0];  //w
    //uiHeight  = pImage->dc.uiReserved[4*3+1];  //h
    //#NT#2011/02/15#Ben Wang -end
    uiPitch   = pImage->dc.uiReserved[4*3+2];  //Bitstream Size
    uiOffset  = pImage->dc.uiReserved[4*3+3];  //bit stream address

    // Get JPG Info.
    Jpeg_DecodeInfo((const UINT8*)uiOffset, uiPitch, &pRaw->w,  &pRaw->h, &pRaw->fmt);
    debug_ind(("uiOffset = %d   uiPitch =%d   w =%d  h = %d fmt = %d\r\n",
        uiOffset,uiPitch,pRaw->w,pRaw->h,pRaw->fmt));

    //For 16 Pixel alignment
    buf_w = MAKE_ALIGN(pRaw->w,16); //for JPG Decode
    buf_h = MAKE_ALIGN(pRaw->h,16); //for JPG Decode
    pRaw->pitch = buf_w;
    pRaw->size = buf_w*buf_h*2 + 64;
    pRaw->w = buf_w;
    pRaw->h = pRaw->h;
    //Jpeg_DecodeImage((UINT8*)uiOffset, uiPitch, (UINT8*)pRaw->address ,pRaw->size);
    debug_ind(("buf_w,buf_h=(%ld,%ld)\r\n",buf_w, buf_h));
    //#NT#2011/11/01#Lincy Lin -begin
    //#NT#
    {
        DecJPG.jdcfg_p = &JDCParams;
        DecJPG.pSrcAddr = (UINT8*)uiOffset;
        DecJPG.JPGFileSize = uiPitch;
        DecJPG.pDstAddr = (UINT8*)pRaw->address;
        GxImg_Decode(pOutImgBuf, &DecJPG);
    }

    //#NT#2011/11/01#Lincy Lin -end


    // Get JPG Decoding Buffer Address
    /*
    old_y = pRaw->address;
    old_y = MAKE_ALIGN(old_y, 32);
    old_cb = old_y + buf_w*buf_h;
    old_cb = MAKE_ALIGN(old_cb, 32);
    old_cr = old_cb + buf_w*buf_h/2;
    old_cr = MAKE_ALIGN(old_cr, 32);
    debug_ind(("Y1=%08x\r\n",old_y));
    debug_ind(("Cb1=%08x\r\n",old_cb));
    debug_ind(("Cr1=%08x\r\n",old_cr));

    // Get Playback Buffer Address
    new_y = pRaw->address;
    new_cb = new_y + buf_w*pRaw->h;
    new_cr = new_cb + buf_w*pRaw->h/2;
    debug_ind(("Y2=%08x\r\n",new_y));
    debug_ind(("Cb2=%08x\r\n",new_cb));
    debug_ind(("Cr2=%08x\r\n",new_cr));

    //compact cb,cr data to y data (JPG Decode Formula -> Playback Formula)
    diff = old_cb - new_cb;
    debug_ind(("Cb diff=%08x\r\n",diff));
    if(diff)
    {
    memcpy((char*)new_cb, (char*)old_cb, diff);
    memcpy((char*)new_cb+diff, (char*)old_cb+diff, (buf_w*pRaw->h/2)-diff);
    }

    diff = old_cr - new_cr;
    debug_ind(("Cr diff=%08x\r\n",diff));
    if(diff)
    {
    memcpy((char*)new_cr, (char*)old_cr, diff);
    memcpy((char*)new_cr+diff, (char*)old_cr+diff, (buf_w*pRaw->h/2)-diff);
    }
    */
#endif
}

void UISlide_LoadData(void)
{
#if _DEMO_TODO

    RAW_IMAGE MyRaw1,MyRaw2;
    UINT32 uiBuf1,uiBuf2,uiTempBuf1;

    // Get Buffer for SlideShow Preview Feature
    uiTempBuf1 = OS_GetMempoolAddr(POOL_ID_SIDC);

    // Prepare 1st image raw data
    uiBuf1 = MyRaw1.address = uiTempBuf1; //assign start address
    _UISlide_DecodeJPG(gPlaySSP_Img1, &MyRaw1, &gPlaySSImgBuf1); //decode JPG to raw data

    // Get Buffer (next)
    uiTempBuf1 += MyRaw1.size;

    // Prepare 2nd image raw data
    uiBuf2 = MyRaw2.address = uiTempBuf1; //assign start address
    _UISlide_DecodeJPG(gPlaySSP_Img2, &MyRaw2, &gPlaySSImgBuf2); //decode JPG to raw data

    // Get Buffer (next)
    //uiTempBuf1 += MyRaw2.size;

    debug_ind(("uiBuf1 = 0x%x , uiBuf2 = 0x%x \r\n",uiBuf1,uiBuf2));

    gPlaySSP_buf_w = MyRaw1.w;
    gPlaySSP_buf_h = MyRaw1.h;
    gPlaySSP_buf_a1 = uiBuf1;
    gPlaySSP_buf_a2 = uiBuf2;
#endif
}
//#NT#2010/02/01#Jeah Yen -end

