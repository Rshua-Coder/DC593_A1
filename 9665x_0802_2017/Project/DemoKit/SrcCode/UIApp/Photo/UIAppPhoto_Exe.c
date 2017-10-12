////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFramework.h"
#include "GxImage.h"
#include "PhotoTask.h"
#include "fd_lib.h"
#include "UIAppPhoto.h"
#include "NVTUserCommand.h"
#include "UIPhotoMapping.h"
#include "UIPhotoInfo.h"
#include "UIDateImprint.h"
#include "UIAppPhoto.h"
#include "UIAppMovie.h" //for NVTEVT_EXE_MOVIEDIS, NVTEVT_EXE_MOVIEGDC, VTEVT_EXE_MOVIESMEAR
#include "UIInfo.h"
#include "UIMode.h"

#include "UIStorageCheck.h"
//nclude "Demo_Sound.h"
#include "FlowPanInit.h"
#include "MenuDatetime.h"
#include "DateTimeInfo.h"
//#NT#2010/09/02#Niven Cho -begin
//#NT#NEW_DATE_IMPRINT
#include "UiDateImprint.h"
//#NT#2010/09/02#Niven Cho -end

#include "Pstore.h"
#include "ImgCaptureAPI.h"
#include "Sensor.h"
#include "DxSensor.h"
#include "UIControlWnd.h"
#include "UIBackGroundObj.h"


#include "DxOutput.h"
#include "FileSysTsk.h"
#include "DataStamp.h"
#include "IPL_AlgInfor.h"
#include "ImgCapInfor.h"
#include "IDE.h"
#include "GxFlash.h"
#include "DeviceCtrl.h"
#include "GxFlash.h"
#include "Ae_Task.h"
#include "Awb_Task.h"
#include "sd_lib.h"
#include "PrjCfg.h"
#include "SysMain.h"
#include "ObjTracking_alg.h"
#include "UICommon.h"
#include "EngineerMode.h"

//for IPL DZoom - begin
#include "IPL_Cmd.h"
#include "IPL_Utility.h"

//for IPL DZoom - end
#if _FD_FUNC_
#include "fd_lib.h"
#endif
#if _SD_FUNC_
#include "sd_lib.h"
#endif
#if _LDWS_FUNC_
#include "ldws_alg.h"
extern UINT32 gLDWScount;
#endif
#if LENS_FUNCTION
#include "lens.h"
#include "Af_api.h"
#include "AF_Lib.h"
#include "Af_task.h"
#endif
#if PHOTO_CONAF_FUNC
#include "Caf_Sample.h"
#endif
#include "Dis_alg.h"
#if (UI_STYLE==UI_STYLE_DEMO)
#include "Demo_Sound.h"
#endif
#if (UI_STYLE==UI_STYLE_DRIVER)
#include "SoundData.h"
#endif
#include "SizeConvert.h"

#if PIP_VIEW_FUNC
#include "PipView.h"
#endif
#if(LOW_POWER_TIMELAPSE_REC == ENABLE)
#include "HwClock.h"
#include "rtc.h"
#include "dcf.h"
#endif
#if(WIFI_AP_FUNC==ENABLE)
#include "WifiAppCmd.h"
#endif
//#NT#2016/08/08#Lincy Lin -begin
//#NT#Support HTTP liveview datastamp function
#include "MovieStamp.h"
//#NT#2016/08/08#Lincy Lin -end

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppPhoto
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
//#define __DBGFLT__          "[cb]"
//#define __DBGFLT__          "[set]"
//#define __DBGFLT__          "[zoom]"
//#define __DBGFLT__          "[af]"
#include "DebugModule.h"

//---------------------UIAPP_PHOTO Global Variables----------


#ifndef PHOTODISTSK_ENABLE
#define PHOTODISTSK_ENABLE   0
#endif

//#NT#2010/10/13#Niven Cho -begin
//#NT#BRC Ctrl
#define CFG_JPG_HEADER_SIZE (64*1024)       //16K
#define CFG_JPG_SCREENNAIL_SIZE (128*1024)  //128K
//#NT#2010/10/13#Niven Cho -end

#if (_SCREENNAIL_SIZE_ == _SCREENNAIL_SIZE_VGA_)
#define    CFG_SCREENNAIL_SIZE  SIZE_VGA
#define    CFG_SCREENNAIL_W     640
#define    CFG_SCREENNAIL_H     480
#else
#define    CFG_SCREENNAIL_SIZE  SIZE_3M
#define    CFG_SCREENNAIL_W     2048
#define    CFG_SCREENNAIL_H     1536
#endif

#define MWB_R_MASK          0x0000FFFF
//#define MWB_G_MASK          0x0000FF00
#define MWB_B_MASK          0xFFFF0000

UINT32 gPhotoFrameBuf = 0;
UINT32 gPhotoFrameBufSize = 0;
UINT32 gPhotoDzoomStop = 0;


UIAppPhotoExeInfoType UIAppPhotoExeInfoInitVal =
{
    0,                                //FreePicNum;
    FALSE,                            //isFolderFull;
    FALSE,                            //isCardFull;
    FALSE,                           //isFocusEnd
    FALSE,                           //isStartCapture
    FALSE,                           //isContShotStateSetOn;
    FALSE,                           //isStopingContShot
    FALSE,                           //isDoingContShot;
    FALSE,                           //isStartSmileDetect;
    0,                               //uiTakePicNum;
    FALSE,                           // isAscnOn;
    FALSE,                           // isCafOn;
    FALSE,                           // isSdOn;
    FALSE,                           // isFdOn;
    FALSE,                           // IsJPGok
    FALSE,                           // IsAFProcess;              //
    FALSE,                           //BDstatus
    FALSE,                           //IsCallBack
    UIAPPPHOTO_CB_MAX,               //CallBackEvent
    FALSE,                           //isPhotoFrameOn
    FALSE,                           //isFlashTrigger
    FALSE,                           //isSingleViewOpen
    FALSE,                           //isPrvPostProcess
    FALSE,                           //isAFBeam
    0,                               //uiMaxImageSize
    FALSE,                           //isDzoomStart
    UI_ZOOM_CTRL_STOP,               //ZoomCtrl
    FALSE,                           //isStartHighSpeedTone
    {0,0,640,480},                   //FdDispCoord
};

static UIAppPhotoExeInfoType UIAppPhotoExeInfo;
static UIAppPhotoExeInfoType *localInfo = &UIAppPhotoExeInfo;
UIAppPhotoExeInfoType *pPhotoExeInfo = &UIAppPhotoExeInfo;

PhotoFrameInfoType      gPFInfo = {0};

URECT photoIDrect[8]={ {136,108,608,816},
                      {136,108+816+34,608,816},
                      {136+612+68,108,608,816},
                      {136+612+68,108+816+34,608,816},
                      {136+612+68+612+136,108+272,408,544},
                      {136+612+68+612+136+408+68,108+272,408,544},
                      {136+612+68+612+136,108+816+34,408,544},
                      {136+612+68+612+136+408+68,108+816+34,408,544}
};





USIZE IMAGERATIO_SIZE[IMAGERATIO_MAX_CNT]=
{
    {9,16},//IMAGERATIO_9_16
    {2,3}, //IMAGERATIO_2_3
    {3,4}, //IMAGERATIO_3_4
    {1,1}, //IMAGERATIO_1_1
    {4,3}, //IMAGERATIO_4_3
    {3,2}, //IMAGERATIO_3_2
    {16,9},//IMAGERATIO_16_9
};


static IPL_IME_INFOR         gImeInfo={0};
static IPL_SET_IMGRATIO_DATA gIPLRatioInfo = {0};

//---------------------UIAPP_PHOTO Local Functions-------------------------
#if (BRC460_FUNCTION==ENABLE)
//return Primary Size or Header+Primary+Screennail Size
static UINT32 PhotoExe_GetExpectSize_RhoBRCrtl(UINT32 ImgIdx,BOOL bPrimaryOnly);
#else
static UINT32 PhotoExe_GetExpectSize_NoBRCCrtl(UINT32 ImgIdx);
#endif
//---------------------UIAPP_PHOTO MISC-------------------------
extern BOOL GPIOMap_DetS2(void);
extern ISIZE _QueryDeviceModeAspect(UINT32 NewDevObj, UINT32 mode);
extern BOOL Sensor_CheckExtSensor(void);

static ER PhotoExe_LensZoomIn(void)
{
    DBG_IND("[zoom]\r\n");
    #if LENS_FUNCTION
    return Lens_Zoom_In();
    #else
    return E_OK;
    #endif

}

static ER PhotoExe_LensZoomOut(void)
{
    DBG_IND("[zoom]\r\n");
    #if LENS_FUNCTION
    return Lens_Zoom_Out();
    #else
    return E_OK;
    #endif
}

static ER PhotoExe_LensZoomStop(void)
{
    DBG_IND("[zoom]\r\n");
    #if LENS_FUNCTION
    return Lens_Zoom_Stop();
    #else
    return E_OK;
    #endif
}

static ER PhotoExe_LensReset(void)
{
    DBG_IND("[zoom]\r\n");
    #if LENS_FUNCTION
    return Lens_Reset();
    #else
    return E_OK;
    #endif
}

UINT32 PhotoExe_GetMaxOzoomIdx(void)
{
    #if LENS_FUNCTION
    return Lens_Zoom_GetSection(ZOOM_MAX_SECTION);
    #else
    return 1;
    #endif
}

UINT32 PhotoExe_GetMinOzoomIdx(void)
{
    #if LENS_FUNCTION
    return Lens_Zoom_GetSection(ZOOM_MIN_SECTION);
    #else
    return 1;
    #endif
}


UINT32 PhotoExe_GetCurOzoomIdx(void)
{
    #if LENS_FUNCTION
    return Lens_Zoom_GetSection(ZOOM_CUR_SECTION);
    #else
    return 1;
    #endif
}


UINT32 PhotoExe_GetMaxDzoomIdx(void)
{
    IPL_DZOOM_INFOR CurInfo;
    CurInfo.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_CUR_DZ_INFOR, (void *)&CurInfo);
    return CurInfo.MaxIdx;
}

UINT32 PhotoExe_GetDZoomIdx(void)
{
    IPL_DZOOM_INFOR CurInfo;
    CurInfo.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_CUR_DZ_INFOR, (void *)&CurInfo);
    return CurInfo.CurIdx;
}

UINT32 PhotoExe_GetDZoomRatio(void)
{
    IPL_DZOOM_INFOR CurInfo;
    CurInfo.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_CUR_DZ_INFOR, (void *)&CurInfo);
    return CurInfo.Ratio;
}


static void PhotoExe_DZoomIn(void)
{
    #if 0
    IPL_DZOOM Info;
    UINT32    index;

    DBG_IND("[zoom]DzoomIdx=%d, Max=%d\r\n",DZOOM_IDX_GET(),DZOOM_IDX_MAX());
    index = DZOOM_IDX_GET();
    if (index < DZOOM_IDX_MAX())
    {
        Info.Id = IPL_ID_1;
        Info.Idx = index+1;
        IPL_SetCmd(IPL_SET_DZOOM, (void *)&Info);
        localInfo->ZoomCtrl = UI_ZOOM_CTRL_IN;
        localInfo->isDzoomStart = TRUE;
        DBG_IND("Zoom start\r\n");
    }
    #else
    gPhotoDzoomStop = FALSE;
    AppBKW_SetData(BKW_DZOOM_STOP,(UINT32)&gPhotoDzoomStop);
    BKG_PostEvent(NVTEVT_BKW_DZOOM_IN);
    #endif
}

static void PhotoExe_DZoomOut(void)
{
    #if 0
    IPL_DZOOM Info;
    UINT32    index;

    DBG_IND("[zoom]DzoomIdx=%d\r\n",DZOOM_IDX_GET());
    index = DZOOM_IDX_GET();
    if (index > DZOOM_IDX_MIN())
    {
        Info.Id = IPL_ID_1;
        Info.Idx = index-1;
        IPL_SetCmd(IPL_SET_DZOOM, (void *)&Info);
        localInfo->ZoomCtrl = UI_ZOOM_CTRL_OUT;
        localInfo->isDzoomStart = TRUE;
        DBG_IND("Zoom start\r\n");
    }
    #else
    gPhotoDzoomStop = FALSE;
    AppBKW_SetData(BKW_DZOOM_STOP,(UINT32)&gPhotoDzoomStop);
    BKG_PostEvent(NVTEVT_BKW_DZOOM_OUT);
    #endif
}

static void PhotoExe_DZoomStop(void)
{
    #if 0
    DBG_IND("[zoom]\r\n");
    localInfo->isDzoomStart = FALSE;
    localInfo->ZoomCtrl = UI_ZOOM_CTRL_STOP;
    #else
    gPhotoDzoomStop = TRUE;
    #endif
}

static void PhotoExe_DZoomContinue(void)
{
    #if 0
    IPL_DZOOM Info;
    UINT32    index;

    DBG_IND("[zoom]DzoomStart = %d, ZoomCtrl=%d, DzoomIdx=%d\r\n", localInfo->isDzoomStart,localInfo->ZoomCtrl,DZOOM_IDX_GET());
    if (localInfo->isDzoomStart)
    {
        index = DZOOM_IDX_GET();
        if (localInfo->ZoomCtrl == UI_ZOOM_CTRL_IN && index < DZOOM_IDX_MAX())
        {
            Info.Id = IPL_ID_1;
            Info.Idx = index+1;
            IPL_SetCmd(IPL_SET_DZOOM, (void *)&Info);
        }
        else if (localInfo->ZoomCtrl == UI_ZOOM_CTRL_OUT && index > DZOOM_IDX_MIN())
        {
            Info.Id = IPL_ID_1;
            Info.Idx = index-1;
            IPL_SetCmd(IPL_SET_DZOOM, (void *)&Info);
        }
        else
        {
            PhotoExe_DZoomStop();
        }
    }
    #endif
}


static void PhotoExe_DZoomReset(void)
{
    IPL_DZOOM Info;

    DBG_IND("[zoom]\r\n");
    Info.Id = IPL_ID_1;
    Info.Idx = DZOOM_IDX_MIN();
    IPL_SetCmd(IPL_SET_DZOOM, (void *)&Info);
    IPL_WaitCmdFinish();
}

UINT32 PhotoExe_GetFdFaceNum(void)
{
    #if _FD_FUNC_
    return FD_GetRsltFaceNum();
    #else
    return 0;
    #endif
}


static USIZE PhotoExe_RatioSizeConvert(USIZE* devSize, USIZE* devRatio, USIZE* Imgratio)
{
    USIZE  resultSize=*devSize;

    if ((!devRatio->w) || (!devRatio->h))
    {
        DBG_ERR("devRatio w=%d, h=%d\r\n",devRatio->w,devRatio->h);
    }
    else if((!Imgratio->w) || (!Imgratio->h))
    {
        DBG_ERR("Imgratio w=%d, h=%d\r\n",Imgratio->w,Imgratio->h);
    }
    else
    {
        if (((float)Imgratio->w/Imgratio->h) >= ((float)devRatio->w/devRatio->h))
        {
            resultSize.w = devSize->w;
            resultSize.h = ALIGN_ROUND_4(devSize->h * Imgratio->h/Imgratio->w* devRatio->w/devRatio->h);
        }
        else
        {
            resultSize.h = devSize->h;
            resultSize.w = ALIGN_ROUND_16(devSize->w * Imgratio->w/Imgratio->h * devRatio->h/devRatio->w);
        }
    }
    return resultSize;
}



void   PhotoExe_SetData(UINT32 attribute,UINT32 value)
{

    /*
    UIAppPhotoTraceMsg("attribute  %d  value   %d\r\n",attribute,value);
    if  (attribute == UIAPPPHOTO_Attr_FD)
        localInfo->isFdOn = value;
    else if(attribute == UIAPPPHOTO_Attr_SD)
        localInfo->isSdOn = value;
    else if(attribute == UIAPPPHOTO_Attr_CAF)
        localInfo->isCafOn = value;
    else if(attribute == UIAPPPHOTO_Attr_ASCN)
        localInfo->isAscnOn = value;
    else
        UIAppPhotoErrMsg("attribute  %d  value   %d\r\n",attribute,value);
    */
}

UINT32 PhotoExe_GetRatiobySize(void)
{
    UINT32 uiImageSize,ImageRatioIdx;

    uiImageSize = UI_GetData(FL_PHOTO_SIZE);
    if (uiImageSize == SIZE_12M32)
    {
        ImageRatioIdx = IMAGERATIO_3_2;
    }
    else if (uiImageSize == SIZE_10M169)
    {
        ImageRatioIdx = IMAGERATIO_16_9;
    }
    else
    {
        ImageRatioIdx = IMAGERATIO_4_3;
    }
    return ImageRatioIdx;
}
//#NT#2010/10/13#Niven Cho -begin
//#NT#BRC Ctrl
/*
    SIZE_14M = 0,
    SIZE_12M32,
    SIZE_10M169,
    SIZE_8M,
    SIZE_5M,
    SIZE_3M,
    SIZE_VGA,
    SIZE_SETTING_MAX
*/
UINT32 PhotoExe_GetScreenNailSize(void)
{
    UINT32           uiImageSize, ScreenNailSize;
    UINT32           BitStreamSize;


    ScreenNailSize = CFG_SCREENNAIL_SIZE;
    uiImageSize = UI_GetData(FL_PHOTO_SIZE);
    if(uiImageSize < ScreenNailSize)
    {
        #if 0
        BitStreamSize = GetPhotoSizeWidth(ScreenNailSize) * GetPhotoSizeHeight(ScreenNailSize)/4;
        #else
        // fix bug: bs size will be wrong if no VGA/3M resolution!
        BitStreamSize = CFG_SCREENNAIL_W * CFG_SCREENNAIL_H / 4;
        #endif

        if (uiImageSize == SIZE_12M32)
        {
            BitStreamSize = BitStreamSize*2/3*4/3;

        }
        else if (uiImageSize == SIZE_10M169)
        {
            BitStreamSize = BitStreamSize*9/16*4/3;
        }
    }
    else
    {
        BitStreamSize = 0;
    }
    DBG_IND("[cap]ScreenNail BitStreamSize =%d K\r\n",BitStreamSize/1024);
    return BitStreamSize;
}


void PhotoExe_SetScreenNailSize(void)
{
    UINT32           uiImageSize, ScreenNailSize;
    USIZE            devRatio={4,3};
    USIZE            ImageRatioSize={0},BufferSize={0};
    USIZE            ImgSize={0};
    UINT32           ImageRatioIdx,BitStreamSize;

    ScreenNailSize = CFG_SCREENNAIL_SIZE;
    uiImageSize = UI_GetData(FL_PHOTO_SIZE);
    if(uiImageSize < ScreenNailSize)
    {
        //BufferSize.w = GetPhotoSizeWidth(ScreenNailSize);
        //BufferSize.h = GetPhotoSizeHeight(ScreenNailSize);
        BufferSize.w = CFG_SCREENNAIL_W;
        BufferSize.h = BufferSize.w*3/4;

        ImageRatioIdx = PhotoExe_GetRatiobySize();
        ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];
        ImgSize = PhotoExe_RatioSizeConvert(&BufferSize,&devRatio,&ImageRatioSize);
        BitStreamSize = PhotoExe_GetScreenNailSize();

        ImgCap_SetUIInfo(CAP_SEL_SCREEN_IMG,SEL_SCREEN_IMG_ON);
        ImgCap_SetUIInfo(CAP_SEL_SCREEN_IMG_H_SIZE,ImgSize.w);
        ImgCap_SetUIInfo(CAP_SEL_SCREEN_IMG_V_SIZE,ImgSize.h);
        ImgCap_SetUIInfo(CAP_SEL_SCREEN_FMT, IPL_IMG_Y_PACK_UV422);
        ImgCap_SetUIInfo(CAP_SEL_SCREENBUFSIZE, BitStreamSize);
        DBG_IND("[cap]ScreenNail w=%d,h=%d, buffSize=%d k\r\n",ImgSize.w,ImgSize.h,BitStreamSize/1024);

    }
    else
    {
        ImgCap_SetUIInfo(CAP_SEL_SCREEN_IMG,SEL_SCREEN_IMG_OFF);
        ImgCap_SetUIInfo(CAP_SEL_SCREENBUFSIZE, 0);
        DBG_IND("[cap]ScreenNail None\r\n");
    }
}

void PhotoExe_SetQuickViewSize(void)
{
    USIZE            devRatio={4,3};
    USIZE            ImageRatioSize={0},BufferSize={0};
    USIZE            ImgSize={0};
    UINT32           ImageRatioIdx;
    ISIZE            DevSize={0};

    DevSize = GxVideo_GetDeviceSize(DOUT1);
    #if (_QUICKVIEW_SIZE_ == _QUICKVIEW_SIZE_VGA_)
    {
        #if 0
        UINT32 QuickViewSize = SIZE_VGA;

        BufferSize.w = GetPhotoSizeWidth(QuickViewSize);
        BufferSize.h = GetPhotoSizeHeight(QuickViewSize);
        #else
        // fix bug: w/h size will be wrong if no VGA/3M resolution!
        BufferSize.w = CFG_SCREENNAIL_W;
        BufferSize.h = CFG_SCREENNAIL_H;
        #endif

        if (BufferSize.w > (UINT32)DevSize.w)
        {
            BufferSize.w = DevSize.w;
            BufferSize.h = BufferSize.w * devRatio.h/devRatio.w;
        }
    }
    #else
    {
        BufferSize.w = DevSize.w;
        BufferSize.h = BufferSize.w * devRatio.h/devRatio.w;
    }
    #endif

    ImageRatioIdx = PhotoExe_GetRatiobySize();
    ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];
    ImgSize = PhotoExe_RatioSizeConvert(&BufferSize,&devRatio,&ImageRatioSize);


    #if (_QUICKVIEW_SIZE_ != _QUICKVIEW_SIZE_VGA_)
    //#NT#2013/07/19#Lincy Lin -begin
    //#NT#Fine tune TV NTSC/PAL quick view display quality
    {

        SIZECONVERT_INFO     CovtInfo = {0};
        DISPSRV_CMD          DispCmd={0};
        DISPSRV_DISP_CFG     CfgDisp={0};
        USIZE                dev1Ratio={0};
        USIZE                tmpImgSize={0};

        memset(&DispCmd,0,sizeof(DISPSRV_CMD));
        DispCmd.Idx = DISPSRV_CMD_IDX_GET_DISP;
        DispCmd.Prop.bExitCmdFinish = TRUE;
        DispCmd.Out.uiNumByte = sizeof(CfgDisp);
        DispCmd.Out.pData= &CfgDisp;
        DispSrv_Cmd(&DispCmd);
        dev1Ratio = CfgDisp.Desc[DISPSRV_DISP_IDX_PRIMARY].AspectRatio;

        CovtInfo.uiSrcWidth  = ImgSize.w;
        CovtInfo.uiSrcHeight = ImgSize.h;
        CovtInfo.uiDstWidth  = DevSize.w;
        CovtInfo.uiDstHeight = DevSize.h;
        CovtInfo.uiDstWRatio = dev1Ratio.w;
        CovtInfo.uiDstHRatio = dev1Ratio.h;
        CovtInfo.alignType = SIZECONVERT_ALIGN_FLOOR_2;
        DisplaySizeConvert(&CovtInfo);

        DBG_IND("[cap]CovtInfo Srcw=%d,h=%d, Dstw=%d ,h=%d, Ratiow=%d ,h=%d, OutW=%d,h=%d\r\n",CovtInfo.uiSrcWidth,CovtInfo.uiSrcHeight,CovtInfo.uiDstWidth,CovtInfo.uiDstHeight,CovtInfo.uiDstWRatio,CovtInfo.uiDstHRatio,CovtInfo.uiOutWidth,CovtInfo.uiOutHeight);
        DBG_IND("[cap]QuickView w=%d,h=%d, DevSize w=%d ,h=%d\r\n",ImgSize.w,ImgSize.h,DevSize.w,DevSize.h);
        if (ImgSize.h != CovtInfo.uiOutHeight)
        {
            tmpImgSize.h = CovtInfo.uiOutHeight;
            tmpImgSize.w = tmpImgSize.h * ImgSize.w/ImgSize.h;
            ImgSize = tmpImgSize;
        }
    }
    #endif
    //#NT#2013/07/19#Lincy Lin -end

    ImgCap_SetUIInfo(CAP_SEL_QV_IMG_H_SIZE,ImgSize.w);
    ImgCap_SetUIInfo(CAP_SEL_QV_IMG_V_SIZE,ImgSize.h);
    ImgCap_SetUIInfo(CAP_SEL_QV_FMT, IPL_IMG_Y_PACK_UV422);
    DBG_IND("[cap]QuickView w=%d,h=%d, DevSize w=%d ,h=%d\r\n",ImgSize.w,ImgSize.h,DevSize.w,DevSize.h);
}

#if (BRC460_FUNCTION==ENABLE)
//return Primary Size or Header+Primary+Screennail Size
static UINT32 PhotoExe_GetExpectSize_RhoBRCrtl(UINT32 ImgIdx,BOOL bPrimaryOnly)
{
    UINT32 BitstreamSize = GetPhotoSizeWidth(ImgIdx) * GetPhotoSizeHeight(ImgIdx);

    switch(UI_GetData(FL_QUALITY))
    {
        case QUALITY_SUPER:
            BitstreamSize = (UINT32)(BitstreamSize*30/100);
            break;

        case QUALITY_FINE:
            BitstreamSize = (UINT32)(BitstreamSize*25/100);
            break;

        case QUALITY_NORMAL:
        default:
            BitstreamSize = (UINT32)(BitstreamSize*20/100);
    }

    if(!bPrimaryOnly)
        BitstreamSize += CFG_JPG_HEADER_SIZE+PhotoExe_GetScreenNailSize();

    return BitstreamSize;

}
#else
static UINT32 PhotoExe_GetExpectSize_NoBRCCrtl(UINT32 ImgIdx)
{
    UINT8               CompressRateTable[QUALITY_ID_MAX]
                        = {
                           23, // Best
                           19, // Fine
                           16  // Normal
                          };

    UINT32 BitstreamSize;
    UINT64 uiCompressRate = CompressRateTable[UI_GetData(FL_QUALITY)];


    if (ImgIdx == SIZE_VGA)
    {
        BitstreamSize = (GetPhotoSizeWidth(ImgIdx) * GetPhotoSizeHeight(ImgIdx) * 22/5) * uiCompressRate / 100;
    }
    else
    {
        BitstreamSize = (GetPhotoSizeWidth(ImgIdx) * GetPhotoSizeHeight(ImgIdx) * 2) * uiCompressRate / 100;
    }

    return BitstreamSize;
}
#endif
//
UINT32 PhotoExe_GetFreePicNum(void)
{
    #if 1
    UINT64              uiFreeSpace;
    UINT32              uiMaxImageSize;
    UINT32              freeImgNum;
    UINT32              reserveSize = 0x80000; //  500KB
    UINT32              CaptureSize, showFreeSize;
    UINT32              size;

    uiFreeSpace = FileSys_GetDiskInfo(FST_INFO_FREE_SPACE);
    showFreeSize = uiFreeSpace/1024;

    DBG_IND("[cap]Free Space = %d KB\r\n", showFreeSize);

    size = UI_GetData(FL_PHOTO_SIZE);
    #if 0
    if(UI_GetData(FL_ModeIndex)== DSC_MODE_PHOTO_PANORAMA)
    {
        size = SIZE_3M;
    }
    #endif

    #if (BRC460_FUNCTION==ENABLE)
    CaptureSize = PhotoExe_GetExpectSize_RhoBRCrtl(size,FALSE);
    #else
    CaptureSize = PhotoExe_GetExpectSize_NoBRCCrtl(size);
    #endif
    DBG_IND("[cap]CaptureSize = %d K \r\n", CaptureSize/1024);

    if(uiFreeSpace < reserveSize)
        freeImgNum = 0;
    else
        freeImgNum = (uiFreeSpace - reserveSize)/CaptureSize;

    DBG_IND("[cap]Free ImgNum = %d\r\n", freeImgNum);

    uiMaxImageSize = CaptureSize + reserveSize;
    //uiMaxImageSize = CaptureSize+(CaptureSize*15/100); //+15%

    localInfo->uiMaxImageSize = uiMaxImageSize;

    DBG_IND("[cap]uiMaxImageSize = %ld k\r\n", uiMaxImageSize/1024);

    //#NT#store in info
    localInfo->FreePicNum = freeImgNum;
    if (localInfo->FreePicNum > 0)
    {
        localInfo->isCardFull = FALSE;
    }
    return freeImgNum;
    #else
    return 100;
    #endif
}



//#NT#2010/09/03#Niven Cho -begin
//#NT#NEW_DATE_IMPRINT
//void PhotoExe_DateImprint(void)
//#NT#2010/09/03#Niven Cho -end


static BOOL PhotoExe_CheckBD(void)
{
#if _DEMO_TODO
    /* Check Blink status */
    AlgMsgInfo *pAlgMsgInfo = GetAlgMsgInfo();
    //FlowPhotoTraceMsg("pAlgMsgInfo->BlinkDetectStatus = %d",pAlgMsgInfo->BlinkDetectStatus);
    if(UI_GetData(FL_BlinkDetIndex) == BLINKDET_ON && pAlgMsgInfo->BlinkDetectStatus)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else
    return FALSE;
#endif
}


static void PhotoExe_CallBackUpdateInfo(UINT32 callBackEvent)
{
    VControl *pCurrnetWnd;

    Ux_GetFocusedWindow(&pCurrnetWnd);
    localInfo->IsCallBack = TRUE;
    localInfo->CallBackEvent = callBackEvent;
    //NVTEVT_UPDATE_INFO
    Ux_SendEvent(pCurrnetWnd,NVTEVT_UPDATE_INFO, 0);
    localInfo->IsCallBack = FALSE;
}


static void PhotoExe_StopContShot(void)
{
    if (localInfo->isStopingContShot == FALSE)
    {
        localInfo->isStopingContShot = TRUE;
        ImgCap_StopCapture();
        PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_STOP_CONTSHOT);
    }
}


ISIZE Photo_GetBufferSize(void) //return USER CUSTOM buffer size
{
    return GxVideo_GetDeviceSize(DOUT1);
}


PHOTO_FUNC_INFO PhotoFuncInfo_dummy =
{
    {
    PHOTO_ID_1,             ///< function hook process Id.
    PHOTO_ISR_SIE_VD,       ///< isr tigger event
    NULL,                   ///< init fp, only execute at first process
    NULL,                   ///< process fp
    NULL,                   ///< process end fp
    NULL,                   ///< get working buffer fp
    NULL,                   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};

#if _FD_FUNC_
void Photo_FDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    //FD_UnInit();
    // Init FD buffer, set max face number to 10, and process rate as 3
    FD_Init(buf, cachebuf, 10, 15, 3);
}

void Photo_FDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    IMG_BUF          InputImg;
    IPL_IME_BUF_ADDR CurInfo;
    UINT32           srcW,srcH,PxlFmt;
    UINT32           PxlAddr[3];
    UINT32           LineOff[3];
    UINT32           ImageRatioIdx;

    CurInfo.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&CurInfo);

    if (CurInfo.ImeP2.type == IPL_IMG_Y_PACK_UV422)
    {
        PxlFmt = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
    }
    else
    {
        PxlFmt = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
    }
    srcW = CurInfo.ImeP2.Ch[0].Width;
    srcH = CurInfo.ImeP2.Ch[0].Height;
    LineOff[0] = CurInfo.ImeP2.Ch[0].LineOfs;
    LineOff[1] = CurInfo.ImeP2.Ch[1].LineOfs;
    LineOff[2] = CurInfo.ImeP2.Ch[2].LineOfs;
    PxlAddr[0] = CurInfo.ImeP2.PixelAddr[0];
    PxlAddr[1] = CurInfo.ImeP2.PixelAddr[1];
    PxlAddr[2] = CurInfo.ImeP2.PixelAddr[2];
    GxImg_InitBufEx(&InputImg, srcW, srcH, PxlFmt, LineOff, PxlAddr);
    ImageRatioIdx = PhotoExe_GetRatiobySize();
    FD_Process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
}
PHOTO_FUNC_INFO PhotoFuncInfo_fd =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Photo_FDInit,           ///< init fp, only execute at first process
    Photo_FDProcess,        ///< process fp
    NULL,                   ///< process end fp
    FD_CalcBuffSize,        ///< get working buffer fp
    FD_CalcCacheBuffSize,   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif


#if _SD_FUNC_
void Photo_SDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    //SD_UnInit();
    // Init SD buffer , and set process rate as 3
    SD_Init(buf,3);
}



void Photo_SDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    SD_Process();
}

PHOTO_FUNC_INFO PhotoFuncInfo_sd =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Photo_SDInit,           ///< init fp, only execute at first process
    Photo_SDProcess,        ///< process fp
    NULL,                   ///< process end fp
    SD_CalcBuffSize,        ///< get working buffer fp
    NULL,                   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif

void Photo_OTProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    Subject_Tracking();
    //DBG_ERR("\r\n LDWS:: Photo_LDWSProcess \r\n" );
}

void Photo_OTProcess_END(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    UINT32 Obj_CentX, Obj_CentY;
    BOOL Obj_Vaild;

    Obj_Vaild = GetTrackingWindow_Center(&Obj_CentX, &Obj_CentY);
    //DBG_ERR("OT:: [%d] Center(%d %d) \r\n", Obj_Vaild, Obj_CentX, Obj_CentY);
}

PHOTO_FUNC_INFO PhotoFuncInfo_OT =
{
    {
    PHOTO_ID_1,             ///< function hook process Id.
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    NULL,           ///< init fp, only execute at first process
    Photo_OTProcess,        ///< process fp
    Photo_OTProcess_END,    ///< process end fp
    NULL,        ///< get working buffer fp
    NULL,   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};

#if _LDWS_FUNC_
void Photo_LDWSInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    //LDWS_UnInit();
    // Init LDWS buffer , and set process rate as 5
    #if 0
    LdwsParms_Init(buf,1);
    #else
    LdwsParms_Init(buf, 3);
    #endif
    //DBG_ERR("\r\n LDWS:: Photo_LDWSInit \r\n" );
}

void Photo_LDWSProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    LdwsParms_Process();
    //DBG_ERR("\r\n LDWS:: Photo_LDWSProcess \r\n" );
}

PHOTO_FUNC_INFO PhotoFuncInfo_ldws =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
//    PHOTO_ISR_SIE_VD,
    PHOTO_ISR_IPE_FRM_END,  ///< isr tigger event
    Photo_LDWSInit,           ///< init fp, only execute at first process
    Photo_LDWSProcess,        ///< process fp
    NULL,                   ///< process end fp
    LdwsParms_CalcBuffSize,        ///< get working buffer fp
    NULL,                   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif

PHOTO_FUNC_INFO PhotoFuncInfo_dis =
{
    {
    PHOTO_ID_1,             ///< function hook process Id.
    PHOTO_ISR_IPE_THOUTEND,       ///< isr tigger event
    dis_initialize,                   ///< init fp, only execute at first process
    dis_process,           ///< process fp
    NULL,                   ///< process end fp
    dis_GetPrvMaxBuffer,        ///< get working buffer fp
    NULL,                   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};



extern void Photo_FdCB(FD_CBMSG uiMsgID, void *Data);
extern void Photo_SdCB(SD_CBMSG uiMsgID, void *Data);
extern void Photo_AFCB(AF_CBMSG uiMsgID, void *Data);


#if(RTSP_LIVEVIEW_FUNC==ENABLE)
#include "DhcpNvt.h"
#include "RtspNvtApi.h"

static UINT32 PhotoExe_RtspGetVideoBufCB(UINT32 *yAddr, UINT32 *cbAddr, UINT32 *crAddr, UINT32 *lineOfsY)
{
    IPL_IME_BUF_ADDR CurInfo;

    CurInfo.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&CurInfo);

    if (CurInfo.P3Enable == ENABLE)
    {
        *yAddr = CurInfo.ImeP3.PixelAddr[0];
        *cbAddr = CurInfo.ImeP3.PixelAddr[1];
        *crAddr = CurInfo.ImeP3.PixelAddr[2];
        *lineOfsY = CurInfo.ImeP3.Ch[0].LineOfs;
    }
    else
    {
        *yAddr = 0;
        *cbAddr = 0;
        *crAddr = 0;
        *lineOfsY = 0;
    }
    return E_OK;
}

static void PhotoExe_OpenRtspLiveView(void)
{
    RTSPNVT_OPEN Open={0};

#if(_NETWORK_ == _NETWORK_SPI_EVB_ETHERNET_)
    //Dhcp_Client_Start("NVT",NULL,TRUE);
#endif

    Open.uiApiVer = RTSPNVT_API_VERSION;
    Open.Type = RTSPNVT_TYPE_LIVE_VIEW;
    Open.uiPortNum = 554;
    Open.LiveInfo.uiBitRate = 1400*1024;
    Open.LiveInfo.uiFrameRate = 30;
    Open.LiveInfo.uiImgWidth = HTTP_MJPG_W_4;
    Open.LiveInfo.uiImgHeight = HTTP_MJPG_H_3;
    Open.LiveInfo.fpGetVideo = PhotoExe_RtspGetVideoBufCB;
    Open.LiveInfo.bEnableAudio = FALSE;
    Open.LiveInfo.AudioSampleRate = RTSPNVT_AUDIO_SR_32000;
    Open.LiveInfo.AudioChannel = RTSPNVT_AUDIO_CH_LEFT;
    Open.uiWorkAddr = OS_GetMempoolAddr(POOL_ID_HTTPLVIEW);
    Open.uiWorkSize = OS_GetMempoolSize(POOL_ID_HTTPLVIEW);
    RtspNvt_Open(&Open);
}

static void PhotoExe_CloseRtspLiveView(void)
{
    RtspNvt_Close();
}
void PhotoExe_OpenHttpLiveView(void)
{
}
void PhotoExe_CloseHttpLiveView(void)
{
}
#elif HTTP_LIVEVIEW_FUNC
#define TEST_H264_EN          0
#define SUPPORT_LVIEW_AUDIO   0
#include "LviewNvtAPI.h"
#include "UIAppHttpLview.h"

static UINT32 PhotoExe_HttpGetVideoBufCB(UINT32 *yAddr, UINT32 *cbAddr, UINT32 *crAddr, UINT32 *lineOfsY)
{
    IPL_IME_BUF_ADDR CurInfo;
#if PIP_VIEW_FUNC
    IMG_BUF *pDstPIP;
#endif

    CurInfo.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&CurInfo);

    if (CurInfo.P3Enable == ENABLE)
    {
        *yAddr = CurInfo.ImeP3.PixelAddr[0];
        *cbAddr = CurInfo.ImeP3.PixelAddr[1];
        *crAddr = CurInfo.ImeP3.PixelAddr[2];
        *lineOfsY = CurInfo.ImeP3.Ch[0].LineOfs;
    }
    else
    {
        #if PIP_VIEW_FUNC
        PipView_Get_OnDrawBuf(&pDstPIP);
        if(pDstPIP)
        {
            *yAddr  = pDstPIP->PxlAddr[0];
            *cbAddr = pDstPIP->PxlAddr[1];
            *crAddr = pDstPIP->PxlAddr[2];
            *lineOfsY = pDstPIP->LineOffset[0];
            DBG_IND("^Yw:%d, h:%d, off:%d\r\n", pDstPIP->Width, pDstPIP->Height, pDstPIP->LineOffset[0]);
        }
        else
        {
            *yAddr = 0;
            *cbAddr = 0;
            *crAddr = 0;
            *lineOfsY = 0;
        }
        #else
        *yAddr = 0;
        *cbAddr = 0;
        *crAddr = 0;
        *lineOfsY = 0;
        #endif
    }
    return E_OK;
}

#if TEST_H264_EN
static UINT32 PhotoExe_HttpFrameCB(UINT32 frameIdx, MEM_RANGE* frameBuf)
{
    #if 1
    FST_FILE  filehdl;
    UINT32    pos;
    INT32     result;
    UINT32    Wsize = frameBuf->Size;
    char      path[40];
    static    UINT32 id = 0;



    if (frameIdx > 60)
        return 0;
    if (!frameIdx)
    {
        id++;
        sprintf(path, "A:\\H264_%02d.RAW", id);
        filehdl = FileSys_OpenFile(path,FST_CREATE_ALWAYS|FST_OPEN_WRITE);

    }
    else
    {
        sprintf(path, "A:\\H264_%02d.RAW", id);
        filehdl = FileSys_OpenFile(path,FST_OPEN_ALWAYS|FST_OPEN_WRITE);
    }
    if (filehdl)
    {
        // 1. append file
        FileSys_SeekFile(filehdl, 0 , FST_SEEK_END);
        pos = FileSys_TellFile(filehdl);
        result = FileSys_WriteFile(filehdl, (UINT8*)frameBuf->Addr, &Wsize, 0, 0);
        FileSys_CloseFile(filehdl);
    }
    #endif
    DBG_DUMP("frameIdx = %d, frameBuf->Addr=0x%x, size=0x%x\r\n",frameIdx,frameBuf->Addr,frameBuf->Size);
    return 0;
}
#endif

#if SUPPORT_LVIEW_AUDIO

static UINT32 PhotoExe_HttpAudioCB(UINT32 audioIdx, MEM_RANGE* audioBuf)
{
    FST_FILE  filehdl;
    UINT32    pos;
    INT32     result;
    UINT32    Wsize;
    static UINT32    totalAudioSize;
    UINT32    uiTempBufAddr;


    DBG_DUMP("audioIdx = %d, audioBuf->Addr=0x%x, size=0x%x\r\n",audioIdx,audioBuf->Addr,audioBuf->Size);
    if (audioIdx > 60)
        return 0;
    if (!audioIdx)
    {
        totalAudioSize = 0;
    }
    uiTempBufAddr = SxCmd_GetTempMem(0x1000000);
    memcpy((void*)(uiTempBufAddr+totalAudioSize),(void*)audioBuf->Addr,audioBuf->Size);
    totalAudioSize+= audioBuf->Size;

    if (audioIdx == 60)
    {
        filehdl = FileSys_OpenFile("A:\\MUSIC.WAV",FST_CREATE_ALWAYS|FST_OPEN_WRITE);
        Wsize = totalAudioSize;
        if (filehdl)
        {
            result = FileSys_WriteFile(filehdl, (UINT8*)uiTempBufAddr, &Wsize, 0, 0);
            FileSys_CloseFile(filehdl);
            DBG_DUMP("Wsize 0x%x\r\n",Wsize);
        }
    }
    return 0;
}

#endif

//#NT#2016/08/08#Lincy Lin -begin
//#NT#Support HTTP liveview datastamp function
#if (HTTP_LIVEVIEW_DATASTAMP_FUNC == ENABLE)
#define HTTP_PATH_ID  2

static UINT32 NetHTTP_DataStampCB(UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY)
{
    MovieStamp_CopyData(HTTP_PATH_ID, yAddr, cbAddr, crAddr, lineY, lineY);
    return 0;
}
#endif
//#NT#2016/08/08#Lincy Lin -end


void PhotoExe_OpenHttpLiveView(void)
{
    extern UINT32 g_lviewframeRate,g_lviewTargetRate,g_lviewQueueFrame;
    LVIEWNVT_INFO      LViewInfo = {0};
    UINT32             uiPoolAddr;

    DBG_IND("\r\n");
    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_HTTPLVIEW);
    LViewInfo.workMemAdr      = uiPoolAddr;
    LViewInfo.workMemSize     = POOL_SIZE_HTTPLVIEW;
    LViewInfo.targetSize      = g_lviewTargetRate;
    LViewInfo.frameRate       = g_lviewframeRate;

#if(MOVIE_LIVEVIEW==DUAL_REC_HTTP_LIVEVIEW)
    {// Liveview MJPG image resolution is equal to LCD panel resolution.
        ISIZE             BufferSize;
        BufferSize = GxVideo_GetDeviceSize(DOUT1);
        DBG_IND("W:%d, H:%d\r\n", BufferSize.w, BufferSize.h);
        LViewInfo.imgWidth        = BufferSize.w;
        LViewInfo.imgHeight       = BufferSize.h;
    }
#else

    #if(MOVIE_LIVEVIEW==RTSP_LIVEVIEW)
    if(UI_GetData(FL_PHOTO_SIZE)==SIZE_10M169)
    #elif(MOVIE_LIVEVIEW==HTTP_LIVEVIEW) //TODO:photo mode should refer to photo size
    if(DscMovie_GetConfig(MOVIE_CFG_RATIO)==IPL_SENCROPRATIO_16_9)
    #endif
    {
        LViewInfo.imgWidth        = HTTP_MJPG_W_16;
        LViewInfo.imgHeight       = HTTP_MJPG_H_9;
    }
    else
    {
        LViewInfo.imgWidth        = HTTP_MJPG_W_4;
        LViewInfo.imgHeight       = HTTP_MJPG_H_3;
    }
#endif
    LViewInfo.MJPGQueueFrameNum = g_lviewQueueFrame;
    LViewInfo.fpStartVideoCB  = NULL;
    LViewInfo.fpGetVideoBufCB = PhotoExe_HttpGetVideoBufCB;
    LViewInfo.fpStopVideoCB  = NULL;
    #if TEST_H264_EN
    LViewInfo.videoCodec   = LVIEWNVT_VIDEO_CODEC_H264;
    LViewInfo.H2643DNRlevel = 0;
    LViewInfo.fpFrameCB  = PhotoExe_HttpFrameCB;
    #endif
    #if SUPPORT_LVIEW_AUDIO
    LViewInfo.isSupportAudio = TRUE;
    LViewInfo.audioSampleRate = LVIEWNVT_AUD_SR_32K;
    LViewInfo.audioChannel = LVIEWNVT_AUD_CH_STEREO;
    LViewInfo.audioBitPerSample = LVIEWNVT_AUD_16BIT_PER_SAMPLE;
    LViewInfo.audioSliceCount = 100;
    LViewInfo.fpAudioCB = PhotoExe_HttpAudioCB;
    #endif
    //#NT#2016/08/08#Lincy Lin -begin
    //#NT#Support HTTP liveview datastamp function
    #if (HTTP_LIVEVIEW_DATASTAMP_FUNC == ENABLE)
    LViewInfo.fpDrawCB        = NetHTTP_DataStampCB;
    #endif
    //#NT#2016/08/08#Lincy Lin -end

    if (LviewNvt_Open(&LViewInfo) != E_OK)
    {
        DBG_ERR("Error open LView video task\r\n");
    }
    //#NT#2016/08/08#Lincy Lin -begin
    //#NT#Support HTTP liveview datastamp function
    #if (HTTP_LIVEVIEW_DATASTAMP_FUNC == ENABLE)
    //---------------------------------------------------------------------------------------------
    // setup movie date stamp if necessary
    //---------------------------------------------------------------------------------------------
    //if (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON)
    {
        UINT32      uiStampAddr;
        UINT32      uiWidth  = LViewInfo.imgWidth;
        UINT32      uiHeight = LViewInfo.imgHeight;
        STAMP_COLOR StampColorBg = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp background color
        STAMP_COLOR StampColorFr = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp frame color
        STAMP_COLOR StampColorFg = {RGB_GET_Y(224, 224, 192), RGB_GET_U(224, 224, 192), RGB_GET_V(224, 224, 192)}; // date stamp foreground color

        // use POOL_ID_DATEIMPRINT as movie data stamp buffer. Do not conflict with Movie Recording.
        uiStampAddr = OS_GetMempoolAddr(POOL_ID_DATEIMPRINT) + (POOL_SIZE_DATEIMPRINT >> 1);
        MovieStamp_SetDataAddr(HTTP_PATH_ID, uiStampAddr);

        MovieStamp_SetColor(HTTP_PATH_ID, &StampColorBg, &StampColorFr, &StampColorFg);

        MovieStamp_Setup(
            HTTP_PATH_ID,
            STAMP_ON |
            STAMP_AUTO |
            STAMP_DATE_TIME |
            STAMP_BOTTOM_RIGHT |
            STAMP_POS_NORMAL |
            STAMP_BG_TRANSPARENT |
            STAMP_YY_MM_DD,
            uiWidth,
            uiHeight);
    }
    #endif
    //#NT#2016/08/08#Lincy Lin -end
    LviewNvt_Start();
    UIAppHttp_Start();
}

void PhotoExe_CloseHttpLiveView(void)
{
    UIAppHttp_Stop();
    LviewNvt_Close();
}

#endif

INT32 PhotoExe_OnDraw(DISPSRV_DRAW* pDraw)
{
    ER er;
    UINT32 uiLockIdx;
    IMG_BUF* pSrc=NULL;
    IMG_BUF* pDst=pDraw->pImg[0];

    IRECT rcSrc={0},rcDst={0};

    if((er=pDraw->fpLock[DISPSRV_SRC_IDX_PRIMARY](&uiLockIdx,DISPSRV_LOCK_NEWEST))!=E_OK)
    {//locked fail indicate skip this draw
        return er;
    }

    pSrc = &pDraw->pDS[DISPSRV_SRC_IDX_PRIMARY]->pSrc[uiLockIdx];

    //--------------------Customer Draw Begin-----------------------------------
    //start to user draw
    rcSrc.x = 0;
    rcSrc.y = 0;
    rcSrc.w = pSrc->Width;
    rcSrc.h = pSrc->Height;
    rcDst.x = 0;
    rcDst.w = pDst->Width;
    rcDst.y = (pDst->Height - pSrc->Height) >> 1;
    rcDst.h = pSrc->Height;

    //scale src to dst
    GxImg_FillData(pDst,NULL,COLOR_YUV_BLACK);
    GxImg_ScaleData(pSrc,&rcSrc,pDst,&rcDst);

    pDraw->fpUnlock[DISPSRV_SRC_IDX_PRIMARY](uiLockIdx);

    if(pDraw->bDualHandle)
    {
        if((er=pDraw->fpLock[DISPSRV_SRC_IDX_SECONDARY](&uiLockIdx,DISPSRV_LOCK_NEWEST))!=E_OK)
        {//locked fail indicate skip this draw
            return er;
        }

        pDst=pDraw->pImg[1];
        pSrc = &pDraw->pDS[DISPSRV_SRC_IDX_SECONDARY]->pSrc[uiLockIdx];

        rcSrc.x = 0;
        rcSrc.y = 0;
        rcSrc.w = pSrc->Width;
        rcSrc.h = pSrc->Height;
        rcDst.x = 0;
        rcDst.y = 0;
        rcDst.w = pDst->Width;
        rcDst.h = pDst->Height;
        //scale src to dst
        GxImg_FillData(pDst,NULL,COLOR_YUV_BLACK);
        GxImg_ScaleData(pSrc,&rcSrc,pDst,&rcDst);
        pDraw->fpUnlock[DISPSRV_SRC_IDX_SECONDARY](uiLockIdx);
    }
    //--------------------Customer Draw End-------------------------------------

    pDraw->fpFlip(TRUE);

    return E_OK;
}


/**
  Initialize application for Photo mode

  Initialize application for Photo mode.

  @param void
  @return void
*/
INT32 PhotoExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if 1
    MEM_RANGE Pool;
    USIZE Buffer1Size,Buffer2Size;
    ISIZE Device1Size,Device2Size;
    ISIZE Aspect1Size,Aspect2Size;
    UINT32 CurrDevObj,CurrDev2Obj;
    UINT32 useFileDB=0;
    PHOTO_FUNC_INFO* phead = NULL;
    BOOL IsRotatePanel = (VDO_USE_ROTATE_BUFFER==TRUE||OSD_USE_ROTATE_BUFFER==TRUE);

    DBG_IND("\r\n");

    //call default
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_OPEN,paramNum,paramArray);

    Pool.Addr = OS_GetMempoolAddr(POOL_ID_APP);
    Pool.Size = POOL_SIZE_APP;
    DscPhoto_Config(PHOTO_CFG_POOL, (UINT32)&Pool);

    //1.get current device size (current mode)
    Device1Size = GxVideo_GetDeviceSize(DOUT1);
    Buffer1Size.w = Device1Size.w;
    Buffer1Size.h = Device1Size.h;

    #if (_FPGA_EMULATION_ == ENABLE)
    //overwrite buffer size to reduce BW
    Buffer1Size.w = 320;
    Buffer1Size.h = 240;
    #endif
    #if(RTSP_LIVEVIEW_FUNC==ENABLE)
    Buffer1Size.w = 80;
    Buffer1Size.h = 60;
    #endif
    //3.get its Aspect Ratio
    CurrDevObj = GxVideo_GetDevice(DOUT1);
    Aspect1Size = _QueryDeviceModeAspect(CurrDevObj, GxVideo_QueryDeviceLastMode(CurrDevObj));

    phead = &PhotoFuncInfo_dummy;
    phead->pNext = NULL; //mark end
    if(1)
    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_dis;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }

    if(1)
    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_OT; //Object tracking
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }
    #if _FD_FUNC_
    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_fd;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }
    // Register FD callback func
    {
        URECT fdDispCoord={0};
        FD_RegisterCB(Photo_FdCB);
        // Set FD display coordinate
        fdDispCoord.w = Device1Size.w;
        fdDispCoord.h = Device1Size.h;
        //FD_SetDispCoord(&fdDispCoord);
        localInfo->FdDispCoord = fdDispCoord;
    }
    #endif
    #if _SD_FUNC_
    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_sd;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }
    SD_RegisterCB(Photo_SdCB);
    #endif

    #if _LDWS_FUNC_
    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_ldws;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }

    LdwsParms_RegisterCB(UIMovie_LDWSCB);

    DBG_IND("\r\n LDWS:: Photo LDWS RegisterCB \r\n" );
    //gLDWScount=1;
    #endif

    DscPhoto_Config(PHOTO_CFG_FUNCINFO, (UINT32)phead);
    #if _FD_FUNC_
    DscPhoto_Config(PHOTO_CFG_CACHESIZE, 2000*1024);
    #else
    DscPhoto_Config(PHOTO_CFG_CACHESIZE, 0);
    #endif

    #if USE_FILEDB
    UI_SetData(FL_IsUseFileDB, 1);
    #else
    UI_SetData(FL_IsUseFileDB, 0);
    #endif

    useFileDB = UI_GetData(FL_IsUseFileDB);
    if (useFileDB)
    {

        DscPhoto_Config(PHOTO_CFG_USE_FILEDB, 1);
        Pool.Addr = OS_GetMempoolAddr(POOL_ID_FILEDB);
        Pool.Size = POOL_SIZE_FILEDB;
        DscPhoto_Config(PHOTO_CFG_FILEDB_MEM, (UINT32)&Pool);

        if (IsEngineerMode() == FALSE)
        {
#if 1 // sample code for FileDB Naming Rule
            // config FDB root
            DscPhoto_SetFDBFolder(FILEDB_CARDV_ROOT);
            DscPhoto_SetFDBPhotoFolder("PHOTO");
#else // sample code for Custom Naming Rule
            //////////////////////////////////////////////////////////////////////////////////////
            // NOTE! IMPORTANT! Custom Naming Rule only support 1 sensor for Photo Capture!     //
            // Since Photo Capture cannot distinguish path 1 and path 2 now, so the file name   //
            // of path 1 and path 2 may be the same, that's why it cannot suport 2 sensors now! //
            //////////////////////////////////////////////////////////////////////////////////////

            // enable custom naming
            DscPhoto_Config(PHOTO_CFG_CUSTOM_NAMING, ENABLE);

            // set root folder
            DscPhoto_SetRootFolder("A:\\CarDV\\");

            // set folder name of 1st photo path
            DscPhoto_SetPhotoFolder(0, "Capture_Front");

            // set folder name of 2nd photo path
            DscPhoto_SetPhotoFolder(1, "Capture_Rear");

            // set custom file name
            #if 0 // manual naming method
            DscPhoto_SetFileNamingMethod(FILE_NAMING_MANUAL);
            DscPhoto_RegPhotoFileNamingCB(UIMovie_PhotoFileNamingCB);
            #elif 1 // semi-auto naming method
            switch(SysGetFlag(FL_DATE_FORMAT))
            {
            case DATE_FORMAT_DMY:
                DscPhoto_SetFileNamingYMD(FILE_NAMING_DATEFMT_DMY);
                break;
            case DATE_FORMAT_MDY:
                DscPhoto_SetFileNamingYMD(FILE_NAMING_DATEFMT_MDY);
                break;
            case DATE_FORMAT_YMD:
            default:
                DscPhoto_SetFileNamingYMD(FILE_NAMING_DATEFMT_YMD);
                break;
            }
            DscPhoto_SetFileNamingMethod(FILE_NAMING_SEMIAUTO);
            DscPhoto_SetFileNamingType(FILE_NAMING_SEMIAUTO_TYPE2);
            DscPhoto_SetFileNameEndChar(0, ENABLE, "_FR");
            DscPhoto_SetFileNameEndChar(1, ENABLE, "_RE");
            DscPhoto_SetFileNameSerialNum(0, DISABLE);
            DscPhoto_SetFileNameSerialNum(1, ENABLE);
            #else // auto naming method => need to do nothing
            #endif
#endif
            #if(LOW_POWER_TIMELAPSE_REC == ENABLE)
            //if(UI_GetData(FL_LOW_POWER_TIMELAPSE_REC)&& rtc_isPWRAlarm())
            if(rtc_isPWRAlarm())
            {
                DBG_DUMP("RTC Timelapes: set up File DB\r\n");
                // config FDB root
                DscPhoto_SetFDBFolder("TMLPS");
            }
            #endif
        }
        else // Engineer mode, using manual custom naming rule
        {
            char cFDBRoot[30];

            EngineerMode_ReadWiFiMAC();

            // enable custom naming
            DscPhoto_Config(PHOTO_CFG_CUSTOM_NAMING, ENABLE);

            // set root folder
            sprintf(cFDBRoot, "A:\\%s\\", FILEDB_CARDV_ROOT);
            DscPhoto_SetRootFolder(cFDBRoot);

            // set folder name of 1st photo path
            DscPhoto_SetPhotoFolder(0, "Photo");

            // set folder name of 2nd photo path
            DscPhoto_SetPhotoFolder(1, "Photo");

            // set manual naming method
            DscPhoto_SetFileNamingMethod(FILE_NAMING_MANUAL);

            // register callback function for manual file naming method
            DscPhoto_RegPhotoFileNamingCB(UIMovie_PhotoFileNamingCB_EngMode);

#if (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
			// Always add file to last
			NH_FileDB_Set_AlwaysAdd2Last(1);
#endif
        }
    }
    else
    {
        DscPhoto_Config(PHOTO_CFG_USE_FILEDB, 0);
    }

    // Register AF callback func
    AF_RegisterCB(Photo_AFCB);
    // Retiser CAF
    #if PHOTO_CONAF_FUNC
    AFTsk_RegisterCAF((AF_PROC_CAF)caf_AutoFocusProcess);
    #endif

    // register Callback function
    Photo_RegCB();
    {
        SENSOR_INIT_OBJ SenObj;
        SENSOR_DRVTAB *SenTab;
        //open sensor
        SenObj = DrvSensor_GetObj1st();
        SenTab = DrvSensor_GetTab1st();
        Sensor_Open(SENSOR_ID_1, &SenObj, SenTab);
        DscPhoto_Config(PHOTO_CFG_IMGPATH, PHOTO_NORMAL);
    }
    #if  (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
    if (Sensor_CheckExtSensor())
    {
        SENSOR_INIT_OBJ SenObj;
        SENSOR_DRVTAB *SenTab;
        //open sensor
        SenObj = DrvSensor_GetObj2nd();
        SenTab = DrvSensor_GetTab2nd();
        Sensor_Open(SENSOR_ID_2, &SenObj, SenTab);

        DscPhoto_Config(PHOTO_CFG_IMGPATH, PHOTO_DUAL_SENSOR);
    }
    #endif
    {
        USIZE            devRatio={4,3};
        USIZE            ImageRatioSize={0};
        USIZE            ImgSize={0};
        UINT32           ImageRatioIdx;


        devRatio.w = Aspect1Size.w;
        devRatio.h = Aspect1Size.h;
        //
        DBG_IND("dev1Ratio w=%d,h=%d\r\n",devRatio.w,devRatio.h);

        ImageRatioIdx = PhotoExe_GetRatiobySize();
        ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];
        ImgSize = PhotoExe_RatioSizeConvert(&Buffer1Size,&devRatio,&ImageRatioSize);
        DscPhoto_Config(PHOTO_CFG_RATIO,Get_ImageRatioValue(ImageRatioIdx));
        DBG_IND("Device1 w=%d,h=%d\r\n",Device1Size.w,Device1Size.h);
        DBG_IND("Buffer1 w=%d,h=%d\r\n",Buffer1Size.w,Buffer1Size.h);
        DBG_IND("ImgSize1 w=%d,h=%d\r\n",ImgSize.w,ImgSize.h);
        //config disp1 view
        AppView_ConfigEnable(0, TRUE);
        AppView_ConfigDevice(0, Device1Size.w, Device1Size.h, Aspect1Size.w, Aspect1Size.h);
        AppView_ConfigBuffer(0, Buffer1Size.w, Buffer1Size.h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED);
        AppView_ConfigImgSize(0,ImgSize.w, ImgSize.h, ImageRatioSize.w, ImageRatioSize.h);
        #if PIP_VIEW_FUNC
        PipView_SetPrimaryImgRatio(&ImageRatioSize);
        #endif
        AppView_ConfigWindow(0, 0, 0, Device1Size.w, Device1Size.h);


        //config disp2 view
        CurrDev2Obj = GxVideo_GetDevice(DOUT2);
        if (CurrDev2Obj)
        {
            Device2Size = GxVideo_GetDeviceSize(DOUT2);
            Buffer2Size.w = Device2Size.w;
            Buffer2Size.h = Device2Size.h;
            Aspect2Size = _QueryDeviceModeAspect(CurrDev2Obj, GxVideo_QueryDeviceLastMode(CurrDev2Obj));

            devRatio.w = Aspect2Size.w;
            devRatio.h = Aspect2Size.h;
            DBG_IND("dev2Ratio w=%d,h=%d\r\n",devRatio.w,devRatio.h);
            ImgSize = PhotoExe_RatioSizeConvert(&Buffer2Size,&devRatio,&ImageRatioSize);
            DBG_IND("Device2 w=%d,h=%d\r\n",Device2Size.w,Device2Size.h);
            DBG_IND("Buffer2 w=%d,h=%d\r\n",Buffer2Size.w,Buffer2Size.h);
            DBG_IND("ImgSize2 w=%d,h=%d\r\n",ImgSize.w,ImgSize.h);
            AppView_ConfigEnable(1, TRUE);
            AppView_ConfigDevice(1, Device2Size.w, Device2Size.h, Aspect2Size.w, Aspect2Size.h);
            AppView_ConfigBuffer(1, Buffer2Size.w, Buffer2Size.h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED);
            AppView_ConfigImgSize(1,ImgSize.w, ImgSize.h, ImageRatioSize.w, ImageRatioSize.h);
            AppView_ConfigWindow(1, 0, 0, Device2Size.w, Device2Size.h);
        }
        else
        {
            AppView_ConfigEnable(1, FALSE);
        }
        // Isiah, Wi-Fi Dual recording test.
        #if(HTTP_LIVEVIEW_FUNC && MOVIE_LIVEVIEW!=DUAL_REC_HTTP_LIVEVIEW)
        {
            if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
            {
                AppView_ConfigEnable(1, TRUE);
                AppView_ConfigBuffer(1, HTTP_MJPG_W_4,HTTP_MJPG_H_3, GX_IMAGE_PIXEL_FMT_YUV420_PACKED);//HTTP use YUV420
                if(Get_ImageRatioValue(ImageRatioIdx) == IPL_SENCROPRATIO_16_9)
                {
                    AppView_ConfigImgSize(1, HTTP_MJPG_W_16, HTTP_MJPG_H_9, HTTP_MJPG_W_16, HTTP_MJPG_H_9);
                }
                else
                {
                    AppView_ConfigImgSize(1, HTTP_MJPG_W_4, HTTP_MJPG_H_3, HTTP_MJPG_W_4, HTTP_MJPG_H_3);
                }
            }
        }
        #endif
    }
    //config src
    #if PIP_VIEW_FUNC
    gPhoto_src.bDirectMode = FALSE;
    DscPhoto_RegChgDispSizeInforCB(PipView_ActivePrimaryImgRatio);
    #endif

    if (IsRotatePanel == TRUE)
    {
        if (System_IsPlugTVHDMI() == TRUE)
        {
            IsRotatePanel = FALSE; // plug in TV or HDMI and current display object is not a rotate panel.
            #if !PIP_VIEW_FUNC
            gPhoto_src.bDirectMode = TRUE;
            #endif
        }
        else
        {
            gPhoto_src.bDirectMode = FALSE;
        }
    }

    AppView_ConfigSource(&gPhoto_src);
    //open

    // force to disable DIS
    IPL_AlgSetUIInfo(IPL_SEL_DISCOMP, SEL_DISCOMP_OFF);

    AppView_Open();
    #if PIP_VIEW_FUNC
    {
        DISPSRV_CMD Cmd={0};
        DISPSRV_FP_DRAW_CB fpOnDraw = PipView_OnDraw;
        Cmd.Idx = DISPSRV_CMD_IDX_SET_DRAW_CB;
        Cmd.In.pData = &fpOnDraw;
        Cmd.In.uiNumByte = sizeof(fpOnDraw);
        Cmd.Prop.bExitCmdFinish = TRUE;
        DispSrv_Cmd(&Cmd);
    }
    #else
    if (IsRotatePanel==TRUE)
    {
        DISPSRV_CMD Cmd={0};
        DISPSRV_FP_DRAW_CB fpOnDraw = PhotoExe_OnDraw;
        Cmd.Idx = DISPSRV_CMD_IDX_SET_DRAW_CB;
        Cmd.In.pData = &fpOnDraw;
        Cmd.In.uiNumByte = sizeof(fpOnDraw);
        Cmd.Prop.bExitCmdFinish = TRUE;
        DispSrv_Cmd(&Cmd);
    }
    #endif

    DscPhoto_Open();

    // force to disable RSC
    #if(_GYRO_EXT_!=_GYRO_EXT_NONE_)
    if (SysGetFlag(FL_MovieRSCIndex)==MOVIE_RSC_ON)
        IPL_AlgSetUIInfo(IPL_SEL_RSC, SEL_RSC_OFF);
    #endif
    // Enable WDR for still capture function.
    IPL_AlgSetUIInfo(IPL_SEL_WDR, SEL_WDR_LV6);

    #if(RTSP_LIVEVIEW_FUNC)
    PhotoExe_OpenRtspLiveView();
    #elif HTTP_LIVEVIEW_FUNC
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        PhotoExe_OpenHttpLiveView();
        UIAppHttp_Open();
        WifiCmd_Done(WIFIFLAG_MODE_DONE,E_OK);
    }
    #endif
    DBG_FUNC_END("");

#else
    UINT32              uiPoolAddr;
    PHOTO_APPOBJ        PhotoObj ={0};
    CAPTURE_OBJ         CaptureObj = {0};
    PHOTODISPLAY_APPOBJ PhotoDisplayObj = {0};
    //#NT#2010/09/28#JeahYen -begin
    ISIZE BufferSize;
    ISIZE DeviceSize;
    //#NT#2010/09/28#JeahYen -end
    //#NT#2010/09/28#JeahYen -begin
    BufferSize = Photo_GetBufferSize();
    DeviceSize = GxVideo_GetDeviceSize(DOUT1);
    //#NT#2010/09/28#JeahYen -end
    //#NT#2010/08/25#Lincy Lin -begin
    //#NT#Remove wave function for not use
    //WAVSTUDIO_FMT       WavFormat;
    //WAVSTUDIO_APPOBJ    WavObj = {0} ;
    //#NT#2010/08/25#Lincy Lin -end
    //#NT#2010/07/29#Connie Yu -begin
    //#NT#modify IDE input/output size settings
#if (PHOTODISTSK_ENABLE == 1)
    PHOTODIS_APPOBJ PhotoDisObj = {0};
#endif
    //#NT#2010/07/29#Connie Yu -end

    //#NT#2010/09/14#Lincy Lin -begin
    //#NT#Should reset uiappphoto variable before init photo task
    UIAppPhotoExeInfo = UIAppPhotoExeInfoInitVal;
    //#NT#2010/09/14#Lincy Lin -end

    // Get memory for Capture task
    //#NT#2010/08/25#Lincy Lin -begin
    //#NT#Move SIDC for Capture using
    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_SIDC);
    CaptureObj.uiMemoryAddr         = uiPoolAddr;
    CaptureObj.uiMemorySize         = POOL_SIZE_APP + POOL_SIZE_SIDC/*- POOL_SIZE_WAV*/;
    CaptureObj.CallBackFunc         = (FPALGCALLBACK)Photo_CaptureCB;
    //#NT#2010/08/25#Lincy Lin -end
    #if (_MSDCVENDOR_== _MSDCVENDOR_NVT_)
    CaptureObj.CallBackFunc         = MsdcNvt_HookImageCb(CaptureObj.CallBackFunc);
    #endif
    CaptureObj.Fp_S2PressDet = (IMGCAP_FPS2PRESSDET)GPIOMap_DetS2;
    // Open Capture task
    if (ImgCapture_Open(&CaptureObj) == E_NOMEM)
    {
        UIAppPhotoErrMsg("Not enough memory for capture task\r\n");
    }

//#NT#2010/08/25#Lincy Lin -begin
//#NT#Remove wave function for not use
/*
#if (_WAVSTUDIO_MODE_ == ENABLE)
    //#NT#2008/04/28#Chris Hsu -begin
    //#NT#Add sample code for bits per sample and user data
    WavFormat.AudioChannel      = AUDIO_CH_RIGHT;
    WavFormat.AudioSamplingRate = AUDIO_SR_11025;
#if (_VM_FORMAT_ == _VM_FORMAT_PCM8_)
    // PCM 8 bits
    WavFormat.AudioCodec        = AUDIOCODEC_PCM;
    WavFormat.uiBitsPerSample   = WAV_BITS_PER_SAM_PCM_8;
#elif (_VM_FORMAT_ == _VM_FORMAT_PCM16_)
    // PCM 16 bits
    WavFormat.AudioCodec        = AUDIOCODEC_PCM;
    WavFormat.uiBitsPerSample   = WAV_BITS_PER_SAM_PCM_16;
#elif (_VM_FORMAT_ == _VM_FORMAT_ADPCM_)
    // ADPCM
    WavFormat.AudioCodec        = AUDIOCODEC_IMA_ADPCM;
    WavFormat.uiBitsPerSample   = WAV_BITS_PER_SAM_ADPCM;
#elif (_VM_FORMAT_ == _VM_FORMAT_ASFPCM_ &&  WAVFILE_ASF_ENABLE == ENABLE)
    WavFormat.AudioCodec        = AUDIOCODEC_PCM;
    WavFormat.FileFormat        = WAVFILE_ASF;
#endif

    WavStudio_SetFormat(&WavFormat);
    // Set maximum recording time
    WavStudio_SetMaxRecTime(10);

    // Set user data
#if (_WAV_SINGLE_USERDATA_ == ENABLE)
    WavStudio_SetUserData((UINT32)&g_uiWAVHeaderUserData, (UINT32)sizeof(g_uiWAVHeaderUserData));
#else
    WavStudio_SetUserData(0, 0);
#endif

    // Set WAV application object
    WavObj.uiMemoryAddr         = uiPoolAddr + POOL_SIZE_APP - POOL_SIZE_WAV;
    WavObj.uiMemorySize         = POOL_SIZE_WAV;
    WavObj.WavStudioCB          = NULL;
    //#NT#2008/04/28#Chris Hsu -end

    // Open Wav studio task
    if (WavStudio_Open(&WavObj) == E_NOMEM)
    {
        UIAppPhotoErrMsg("Not enough memory for wav studio task\r\n");
    }
#endif
*/
    // Get memory for Photo task
    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO1);

    PhotoObj.uiMemoryAddr           = uiPoolAddr;
    // Photo(Preview) memory allocation note:
    // Dealing with memory-shortage, memory pool might not be in plain order
    // More than one pool could be assigned for Photo with following rules
    //     The first one must be POOL_ID_DISP_VDO1
    //     All pools must be successive
    PhotoObj.uiMemorySize           = POOL_SIZE_DISP_VDO1 + POOL_SIZE_DISP_VDO1TEMP;
    PhotoObj.CallBackFunc           = (FPALGCALLBACK)Photo_CaptureCB;
    #if (_MSDCVENDOR_== _MSDCVENDOR_NVT_)
    PhotoObj.CallBackFunc         = MsdcNvt_HookImageCb(PhotoObj.CallBackFunc);
    #endif

    //#NT#2010/09/28#JeahYen -begin
    PhotoObj.uiDisplayFBWidth   = BufferSize.w;
    PhotoObj.uiDisplayFBHeight  = BufferSize.h;
    //#NT#2010/09/28#JeahYen -end
    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_CACHE);
    if (UI_GetData(FL_NextMode) == DSC_MODE_PHOTO_PANORAMA)
    {
        PhotoObj.uiCacheMemoryAddr = uiPoolAddr+POOL_CACHE_OFFSET_PANO;
        PhotoObj.uiCacheMemorySize = POOL_SIZE_CACHE_PANO;
    }
    else
    {
        PhotoObj.uiCacheMemoryAddr = uiPoolAddr+POOL_CACHE_OFFSET_PHOTO;
        PhotoObj.uiCacheMemorySize = POOL_SIZE_CACHE_PHOTO;
    }
    // Open Photo task
    if (Photo_Open(&PhotoObj) == E_NOMEM)
    {
        UIAppPhotoErrMsg("Not enough memory for photo task\r\n");
    }
//#NT#2010/07/16#Jeffery Chuang -begin
    // Open AF task
    if (AFTsk_Open() == E_NOMEM)
    {
        debug_err(("Not enough memory for AF task\r\n"));
    }
    //#NT#2011/03/24#Lincy Lin -begin
    //#NT#Set cont af rate
    AFTsk_SetCafRate(1);
    //#NT#2011/03/24#Lincy Lin -end

    // Register AFTsk function pointer
    AFTsk_RegisterFP_Param((FPAF_PARAM)AF_ParamInit);
    AFTsk_RegisterFP_Prepare((FPAF_PREPARE)AF_PrepareEN);
    AFTsk_RegisterFP_IrisSwtich((FPAF_IRISSWITCH)AF_AEIrisSwitch);
    AFTsk_RegisterFP_AFTiming((FPAF_TIMGING)AF_TimingPrepare);
//#NT#2010/07/16#Jeffery Chuang -end
//#NT#2010/12/09#Jeffery Chuang -begin
//CAF Move to AFTsk
    AFTsk_RegisterCAF((FPAFTSKCAF)caf_AutoFocusProcess);
//#NT#2010/12/09#Jeffery Chuang -end
    Photo_RegisterAE((FPPHOTOAE)aaa_AEprocess);
    Photo_RegisterAWB((FPPHOTOAWB)aaa_AWBprocess);
    Photo_RegisterFD((FPPHOTOFD)fd_FDprocess);
    Photo_RegisterSD((FPPHOTOSD)sd_SmileDetectionProcess);
    Photo_RegisterASC((FPPHOTOASC)asc_AutoSceneProcess);
    //Photo_RegisterCAF((FPPHOTOCAF)caf_AutoFocusProcess);
    Photo_RegisterPANCAP((FPPHOTOPANCAP)pancap_detectionprocess);
    // Get memory for PhotoDisplay task
    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_APP);
    PhotoDisplayObj.uiMemoryAddr         = uiPoolAddr;
    //#NT#2010/08/25#Lincy Lin -begin
    //#NT#Remove wave function for not use
    //PhotoDisplayObj.uiMemorySize         = POOL_SIZE_APP- POOL_SIZE_WAV;
    PhotoDisplayObj.uiMemorySize         = POOL_SIZE_APP;
    //#NT#2010/08/25#Lincy Lin -end
    PhotoDisplayObj.CallBackFunc         = (FPALGCALLBACK)Photo_CaptureCB;
    #if (_MSDCVENDOR_== _MSDCVENDOR_NVT_)
    PhotoDisplayObj.CallBackFunc         = MsdcNvt_HookImageCb(PhotoDisplayObj.CallBackFunc);
    #endif
    //#NT#2010/08/23#Lincy Lin -end

    //#NT#2010/09/28#JeahYen -begin
    PhotoDisplayObj.uiDisplayFBWidth   = BufferSize.w;
    PhotoDisplayObj.uiDisplayFBHeight  = BufferSize.h;
    PhotoDisplayObj.uiDisplayOutWidth  = DeviceSize.w;
    PhotoDisplayObj.uiDisplayOutHeight = DeviceSize.h;
    //#NT#2010/09/28#JeahYen -end

    // Open PhotoDisplay task
    if (PhotoDisplay_Open(&PhotoDisplayObj) == E_NOMEM)
    {
        UIAppPhotoErrMsg("Not enough memory for photo display task\r\n");
    }
#if (PHOTODISTSK_ENABLE == 1)
    // Open Photo DIS task
    //#NT#2010/07/29#Connie Yu -begin
    //#NT#modify IDE input/output size settings
    //#NT#2010/09/28#JeahYen -begin
    PhotoDisObj.uiDisplayFBWidth   = BufferSize.w;
    PhotoDisObj.uiDisplayFBHeight  = BufferSize.h;
    PhotoDisObj.uiDisplayOutWidth  = DeviceSize.w;
    PhotoDisObj.uiDisplayOutHeight = DeviceSize.h;
    //#NT#2010/09/28#JeahYen -end
    if (PhotoDis_Open(&PhotoDisObj) == E_NOMEM)
    {
        UIAppPhotoErrMsg("Not enough memory for photo DIS task\r\n");
    }
    //#NT#2010/07/29#Connie Yu -end
    PhotoDis_registerDIS((FPPHOTODIS)dis_accumUpdateProcess);
#endif
    //#NT#2010/12/29#Spark Chou -begin
#if (PRV_PREOUT_HIGH_QTY == ENABLE)
    UI_SetParameter(_PrePrvBitDepth, _PreBitDepth_12);
#else
    UI_SetParameter(_PrePrvBitDepth, _PreBitDepth_8);
#endif
    //#NT#2010/12/29#Spark Chou -end
#endif
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if 1
    DscPhoto_Close();
    AppView_Close();
    {
        DISPSRV_CMD      DispCmd={0};
        memset(&DispCmd,0,sizeof(DISPSRV_CMD));
        // show last frame of liveview
        DispCmd.Idx = DISPSRV_CMD_IDX_DIRECT_FRM_TO_TEMP;
        DispCmd.Prop.bExitCmdFinish=TRUE;
        DispSrv_Cmd(&DispCmd);

    }
    Sensor_Close(SENSOR_ID_1);
    #if  (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
    if (Sensor_CheckExtSensor())
        Sensor_Close(SENSOR_ID_2);
    #endif

    #if _SD_FUNC_
    SD_UnInit();
    #endif
    #if _FD_FUNC_
    FD_UnInit();
    #endif

    #if RTSP_LIVEVIEW_FUNC
    PhotoExe_CloseRtspLiveView();
    #elif HTTP_LIVEVIEW_FUNC
    UIAppHttp_Close();
    PhotoExe_CloseHttpLiveView();
    #endif


#else
    ImageAlgInfo *AlgInfo;

    AlgInfo = UI_GetParameter();

    if (localInfo->isSingleViewOpen)
    {
        localInfo->isSingleViewOpen = FALSE;
        //#NT#2010/10/20#Lincy Lin -begin
        //#NT#Avoid seeing garbage screen
 //#NT#2010/11/01#Steven feng -begin
 #if FISHEYETOPLAYBACK_BLACKBG
        idec_setV1En(IDE_ID_1, FALSE);
 #endif
  //#NT#2010/11/01#Steven feng -end
        //#NT#2010/10/20#Lincy Lin -end
        Photo_SingleView_Close(PHOTO_VIEWTYPE_NORMAL);
    }
    if(localInfo->isPrvPostProcess)
    {
        localInfo->isPrvPostProcess = FALSE;
        if(Prv_PostProcessing_Close() != E_OK)
        {
            UIAppPhotoErrMsg("Prv_PostProcessing_Close ...error\r\n");
        }
    }
    //#NT#2010/12/02#Lincy Lin -begin
    if (localInfo->isAFBeam)
    {
        LED_TurnOffLED(GPIOMAP_LED_FCS);   //If AF beam is on ,then turn off the focus LED after AF_Process().
        LED_SetFcsLevel(LCSBRT_LVL_03);
        localInfo->isAFBeam = FALSE;
    }
    //#NT#2010/12/02#Lincy Lin -end


    //Fix Cont Shot hang-up, Due to SIECAPTSK_ID will be terminate when doing Set2SieCap
    //Root Cause: ImgCapture_Close wait idel, but UI use it(will busy), ImgCapture_Close due to get previous idle then terminate it!
    ImgCapture_StopCapture();
    ImgCapture_Close();
    //UI Need to wait capture write all files complete than can change mode
    //Last 3 shot don't need to wait

    if (System_GetState(SYS_STATE_NEXTMODE)!= PRIMARY_MODE_PHOTO && AlgInfo->DriveMode != _IMGDriveMode_Last)
    {
        ImgCapture_WaitFinish();

    }
    Photo_Close();
//#NT#2010/07/16#Jeffery Chunag -begin
    AFTsk_Close();
//#NT#2010/07/16#Jeffery Chunag -end
    PhotoDisplay_Close();
#if (PHOTODISTSK_ENABLE == 1)
    PhotoDis_Close();
#endif
#endif
    //call default
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_CLOSE,paramNum,paramArray);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnMacro(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_MACRO,uhSelect);
    IPL_AlgSetUIInfo(IPL_SEL_AFMODE,Get_MacroValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnSelftimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_SELFTIMER,uhSelect);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnFlash(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;

    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_FLASH_MODE,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_FLASH,uhSelect);
    if (UI_GetData(FL_IsInitSettingFlag) == FALSE)
    {
        if(uhSelect != FLASH_OFF)
        {
            //#NT#Hera14 HW bug , no battery insert can't charge for flash
            if (!UI_GetData(FL_IsStopCharge))
            {
                GxFlash_StartCharge();
            }
        }
        else
        {
            GxFlash_StopCharge();
        }
    }
    IPL_AlgSetUIInfo(IPL_SEL_FLASHMODE,Get_FlashValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnEV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_EV,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_EV,uhSelect);
    IPL_AlgSetUIInfo(IPL_SEL_AEEV,Get_EVValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);

    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_PHOTO_SIZE,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_SIZE,uhSelect);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnQuality(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];
    UI_SetData(FL_QUALITY,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_QUALITY,uhSelect);
    ImgCap_SetUIInfo(CAP_SEL_QUALITY,Get_QualityValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnWB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);

    uhSelect= paramArray[0];
    UI_SetData(FL_WB,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_WB,uhSelect);
    IPL_AlgSetUIInfo(IPL_SEL_WBMODE,Get_WBValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnMwbAdjust(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if (UI_STYLE==UI_STYLE_DEMO)
    UINT32 Rg,Gg,Bg,uhValue;

    UISound_Play(PLAYSOUND_SOUND_SHUTTER_TONE);
    MWB_CalcCustomerColorGain(SEL_WBMODE_CUSTOMER1, &Rg, &Gg, &Bg);
    MWB_WriteColorGain(SEL_WBMODE_CUSTOMER1, Rg, Gg, Bg);
    DBG_IND("Rg=%d, Gg=%d, Bg=%d \r\n",Rg, Gg, Bg);

    uhValue =  ((Bg <<16) & MWB_B_MASK) + (Rg & MWB_R_MASK);
    UI_SetData(FL_MwbRGB,uhValue);
    DBG_IND("uhValue = %d \r\n",uhValue);
    //Need to wait sound play complete
    GxSound_WaitStop();
    #endif
    return NVTEVT_CONSUME;
}
INT32 PhotoExe_OnMwbRGB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhValue = 0;
    UINT32 Rg,Gg = 0,Bg;

    if(paramNum>0)
        uhValue= paramArray[0];
    UI_SetData(FL_MwbRGB,uhValue);
    DBG_IND("uhValue = %d \r\n",uhValue);

    Rg = (uhValue & MWB_R_MASK);
    Bg = ((uhValue & MWB_B_MASK) >> 16);
    MWB_WriteColorGain(SEL_WBMODE_CUSTOMER1, Rg, Gg, Bg);
    DBG_IND("Rg=%d, Gg=%d, Bg=%d \r\n",Rg, Gg, Bg);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnColor(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);

    if(paramNum>0)
        uhSelect= paramArray[0];

    if (UI_GetData(FL_ModeIndex) != DSC_MODE_PHOTO_SCENE)
    {
        UI_SetData(FL_COLOR_EFFECT,uhSelect);
        Photo_SetUserIndex(PHOTO_USR_COLOR,uhSelect);
        IPL_AlgSetUIInfo(IPL_SEL_IMAGEEFFECT,Get_ColorValue(uhSelect));
    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnISO(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);

    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_ISO,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_ISO,uhSelect);
    IPL_AlgSetUIInfo(IPL_SEL_ISOINDEX,Get_ISOValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnAFWindow(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);

    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_AFWindowIndex,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_AFWINDOW,uhSelect);
    IPL_AlgSetUIInfo(IPL_SEL_AFWINDOW,Get_AFWindowValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnAFBeam(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_AFBeamIndex,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_AFBEAM,uhSelect);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnContAF(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_ContAFIndex,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_CONTAF,uhSelect);
    if (uhSelect == CONT_AF_ON)
    {
        localInfo->isCafOn = TRUE;
    }
    else
        localInfo->isCafOn = FALSE;
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnMetering(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);

    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_METERING,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_METERING,uhSelect);
    IPL_AlgSetUIInfo(IPL_SEL_AEMODE,Get_MeteringValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnCaptureMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_CapModeIndex,uhSelect);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnDatePrint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);

    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_DATE_STAMP,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_DATEIMPRINT,uhSelect);
    ImgCap_SetUIInfo(CAL_SEL_DATASTAMP, Get_DatePrintValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnPreview(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_QUICK_REVIEW,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_QREVIEW,uhSelect);
    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnDigitalZoom(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_Dzoom,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_DZOOMSTATUS,uhSelect);
    if (uhSelect == DZOOM_OFF)
    {
        UI_SetData(FL_DzoomIndex,DZOOM_10X);
    }

    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnBShoot(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_BShootIndex,uhSelect);
#if _DEMO_TODO
    Photo_SetUserIndex(PHOTO_USR_SLOWSHUTTER,uhSelect);
    UI_SetParameter(_ManualExpos, Get_BShutterValue(uhSelect));
#endif
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnBlinkDet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_BlinkDetIndex,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_BLINK,uhSelect);
    return NVTEVT_CONSUME;
}
INT32 PhotoExe_OnSmileDet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);

    if(uhSelect == 0)
        UI_SetData(FL_SMILE,uhSelect);
    else
        UI_SetData(FL_SMILE,uhSelect);

    if (uhSelect == 0)
    {
        UI_SetData(FL_FD,FD_ON);
    }
    else
    {
        UI_SetData(FL_FD,FD_SMILE);
    }
    if (UI_GetData(FL_ModeIndex) == DSC_MODE_PHOTO_MANUAL)
    {
        Photo_SetUserIndex(PHOTO_USR_FD,UI_GetData(FL_FD));
    }
    return NVTEVT_CONSUME;
}

//#NT#2010/06/17#Lincy Lin -begin
//#NT#
INT32 PhotoExe_OnFD(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_FD,uhSelect);
    if (UI_GetData(FL_ModeIndex) == DSC_MODE_PHOTO_MANUAL)
    {
        Photo_SetUserIndex(PHOTO_USR_FD,uhSelect);
    }
    if (uhSelect == FD_SMILE)
    {
        localInfo->isSdOn = TRUE;
        localInfo->isFdOn = TRUE;
    #if _FD_FUNC_
        FD_Lock(FALSE);
    #endif
    }
    else if(uhSelect == FD_ON)
    {
        localInfo->isSdOn = FALSE;
        localInfo->isFdOn = TRUE;
    #if _FD_FUNC_
        FD_Lock(FALSE);
    #endif
    }
    else
    {
        localInfo->isSdOn = FALSE;
        localInfo->isFdOn = FALSE;
        if (!localInfo->isAscnOn)
        {
        #if _FD_FUNC_
        FD_Lock(TRUE);
        #endif
        }
    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnASCN(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;

    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
    {
        uhSelect= paramArray[0];
#if _DEMO_TODO
        localInfo->isAscnOn = uhSelect;
        UI_SetParameter(_SceneMode, _SceneMode_Auto);
#endif
    }
    //UI_SetData(FL_BlinkDetIndex,uhSelect);
    return NVTEVT_CONSUME;
}
//#NT#2010/06/17#Lincy Lin -end


INT32 PhotoExe_OnContShot(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_ContShotIndex,uhSelect);
    if (UI_GetData(FL_ModeIndex) == DSC_MODE_PHOTO_MANUAL)
    {
        Photo_SetUserIndex(PHOTO_USR_CONTSHOT,uhSelect);
    }
    return NVTEVT_CONSUME;
}
INT32 PhotoExe_OnTimeLapse(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_TimelapseIndex,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_TIMELAPSE,uhSelect);
    UI_SetData(FL_ContShotIndex,CONT_SHOT_TIMELAPSE);
    Photo_SetUserIndex(PHOTO_USR_CONTSHOT,CONT_SHOT_TIMELAPSE);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnPan(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_PanModeIndex,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_PANDET,uhSelect);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnSceneMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;

    DBG_IND("%d \r\n",paramArray[0]);

    if(paramNum)
    {
        uhSelect= paramArray[0];

        // scene select by menu
        if (paramNum == 1)
        {
            UI_SetData(FL_SCENE,uhSelect);
            IPL_AlgSetUIInfo(IPL_SEL_SCENEMODE, Get_SceneModeValue(uhSelect));
        }
        else
        // scene set by photo init
        {
            IPL_AlgSetUIInfo(IPL_SEL_SCENEMODE, Get_SceneModeValue(uhSelect));
        }

    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnCaptureStart(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 CaptureParm = ON_CAPTURE_SINGLE;
    //ImageAlgInfo *AlgInfo;
    //#NT#2010/09/02#JeahYen -begin
    BOOL bAllowImagePostProcess = TRUE;
    //#NT#2010/09/02#JeahYen -end

    DBG_IND("[cap]\r\n");
    //AlgInfo = UI_GetParameter();

    if (localInfo->isStartCapture)
    {
        DBG_ERR("Capture start in Capturing\r\n");
    }
    if (localInfo->IsAFProcess)
    {
        DBG_ERR("Capture start in AF\r\n");
    }
    if (paramNum == 1)
    {
        CaptureParm = paramArray[0];
    }

    if( UI_GetData(FL_FLASH_MODE) != FLASH_OFF)
    {
        SxTimer_SetFuncActive(SX_TIMER_DET_RECHARGE_ID,FALSE);
        GxFlash_StopCharge();
    }
    //Disable USB detection
    SxTimer_SetFuncActive(SX_TIMER_DET_USB_ID,FALSE);
    //Disable USB detection
    SxTimer_SetFuncActive(SX_TIMER_DET_TV_ID,FALSE);
    //Disable Mode detection
    SxTimer_SetFuncActive(SX_TIMER_DET_MODE_ID,FALSE);


    AE_Lock(AE_ID_1,TRUE,FALSE);
    AWB_Lock(AWB_ID_1,TRUE,FALSE);
    AFTsk_LockCAF(TRUE);
    #if _FD_FUNC_
    FD_Lock(TRUE);
    #endif
    #if _SD_FUNC_
    SD_Lock(TRUE);
    #endif
    AF_WaitIdle(AF_ID_1);
    Photo_WaitIdle(PHOTO_ID_1);

    Ux_FlushEventByRange(NVTEVT_ALGMSG_FDEND,NVTEVT_ALGMSG_FDEND);
    Ux_FlushEventByRange(NVTEVT_ALGMSG_ASCEND,NVTEVT_ALGMSG_ASCEND);

    idec_setV1En(IDE_ID_1, FALSE);
    if (localInfo->isSingleViewOpen)
    {
        //idec_setV1En(IDE_ID_1, FALSE);
        //Photo_SingleView_Close(PHOTO_VIEWTYPE_NORMAL);
        localInfo->isSingleViewOpen = FALSE;
    }
    if(UI_GetData(FL_DATE_STAMP)==DATEIMPRINT_OFF)
    {
        UiDateImprint_SetEnable(FALSE);
    }
    else
    {
        //Avoid seeing garbage screen
        idec_setV1En(IDE_ID_1, FALSE);
        // 1. should enable first then can init buff
        UiDateImprint_SetEnable(TRUE);
        // 2. Init Date Imprint buff
        Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_INIT_DATE_BUF, 0);
    }

    IPL_AlgSetUIInfo(IPL_SEL_FLASHMODE,Get_FlashValue(UI_GetData(FL_FLASH_MODE)));
    ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_H_SIZE, GetPhotoSizeWidth(UI_GetData(FL_PHOTO_SIZE)));
    ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_V_SIZE, GetPhotoSizeHeight(UI_GetData(FL_PHOTO_SIZE)));
    #if  (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
    if (Sensor_CheckExtSensor())
    {   // TBD. User can change desired capture size of sensor 2 here.
        ImgCap_SetUIInfo(CAP_SEL_JPG2_IMG_H_SIZE, 1280);
        ImgCap_SetUIInfo(CAP_SEL_JPG2_IMG_V_SIZE, 720);
    }
    #endif

    if (CaptureParm == ON_CAPTURE_SD_CAPTURE)
    {
        localInfo->uiTakePicNum++;
        //AppPhoto_SetData(&UIPhotoObjCtrl,_BlinkDetectionMode, Get_BlinkValue(BLINKDET_OFF));
    }
    else
    {
        /*
        UIAppPhotoTraceCapMsg("fde_GetFaceNum=%d\r\n",fde_GetFaceNum());
        if ((UI_GetData(FL_FD)== FD_ON) && fde_GetFaceNum())
        {
            AppPhoto_SetData(&UIPhotoObjCtrl,_BlinkDetectionMode, Get_BlinkValue(UI_GetData(FL_BlinkDetIndex)));
        }
        else if ((UI_GetData(FL_ModeIndex)== DSC_MODE_PHOTO_SCENE)&&(UI_GetData(FL_SCENE)== SCENE_ID))
        {
            AppPhoto_SetData(&UIPhotoObjCtrl,_BlinkDetectionMode, Get_BlinkValue(UI_GetData(FL_BlinkDetIndex)));
        }
        else
        {
            AppPhoto_SetData(&UIPhotoObjCtrl,_BlinkDetectionMode, Get_BlinkValue(BLINKDET_OFF));
        }
        */
    }
    bAllowImagePostProcess = TRUE;

    if(CaptureParm == ON_CAPTURE_CONT)
    {
        switch(UI_GetData(FL_ContShotIndex))
        {
            case CONT_SHOT_INFINITE:
                //In cont shot (except for timelapse cont shot) case ,must disable blink
                ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_INFBURST);
                IPL_AlgSetUIInfo(IPL_SEL_FLASHMODE,SEL_FLASHMODE_FORCEOFF);
                ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 2);
                ImgCap_SetUIInfo(CAP_SEL_JPG_BUF_NUM, 3);
                ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
                ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
                UI_SetData(FL_IsSingleCapture,FALSE);
                localInfo->isDoingContShot = TRUE;
                localInfo->isStopingContShot = FALSE;
                localInfo->uiTakePicNum = 0;
                bAllowImagePostProcess = FALSE;//not support image post-process
                break;

            case CONT_SHOT_10:
                //In cont shot (except for timelapse cont shot) case ,must disable blink
                #if 0
                ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_BURST);
                IPL_AlgSetUIInfo(IPL_SEL_FLASHMODE,SEL_FLASHMODE_FORCEOFF);
                ImgCap_SetUIInfo(CAP_SEL_PICNUM, 10);
                ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 1);
                ImgCap_SetUIInfo(CAP_SEL_JPG_BUF_NUM, 2);
                ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
                ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
                #else
                ImgCap_SetUIInfo(CAP_SEL_ENCRATIO, 50);
                ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_EBURST);
                IPL_AlgSetUIInfo(IPL_SEL_FLASHMODE,SEL_FLASHMODE_FORCEOFF);
                #if (UI_STYLE==UI_STYLE_DEMO)
                ImgCap_SetUIInfo(CAP_SEL_PICNUM, 10);
                ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 9);
                #endif
                #if (UI_STYLE==UI_STYLE_DRIVER)
                ImgCap_SetUIInfo(CAP_SEL_PICNUM, 3);
                ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 2);
                ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_BURST);
                #endif
                ImgCap_SetUIInfo(CAP_SEL_JPG_BUF_NUM, 2);
                ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
                ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
                #endif
                UI_SetData(FL_IsSingleCapture,FALSE);
                localInfo->isDoingContShot = TRUE;
                localInfo->isStopingContShot = FALSE;
                localInfo->uiTakePicNum = 0;
                localInfo->isStartHighSpeedTone = FALSE;
                bAllowImagePostProcess = FALSE;//not support image post-process
                break;

            case CONT_SHOT_3LAST:
                #if 0
                AppPhoto_SetData(&UIPhotoObjCtrl,_DriveMode, _IMGDriveMode_Last);
                AppPhoto_SetData(&UIPhotoObjCtrl,_FlashMode,_IMGFlashMode_ForceOFF);
                AppPhoto_SetData(&UIPhotoObjCtrl,_BlinkDetectionMode, _BlinkDetectionMode_Off);
                // need add 3last in sensor lib
                AppPhoto_SetData(&UIPhotoObjCtrl,_BurstNum, _BurstNum_3);
                AppPhoto_SetData(&UIPhotoObjCtrl,_PreCapBitDepth, _PreBitDepth_8);
                AppPhoto_SetData(&UIPhotoObjCtrl,_SiePath,_PostGamma);
                UI_SetParameter(_CapAWBMode,PREVIEW_AWB);
                UI_SetParameter(_JpgBufMode,_JpgBufMode_Auto);
                #endif
                ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_LAST);
                IPL_AlgSetUIInfo(IPL_SEL_FLASHMODE,SEL_FLASHMODE_FORCEOFF);
                ImgCap_SetUIInfo(CAP_SEL_JPG_BUF_NUM, 3);
                ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 3);
                ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
                ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
                UI_SetData(FL_IsSingleCapture,FALSE);
                localInfo->isDoingContShot = TRUE;
                localInfo->isStopingContShot = FALSE;
                localInfo->uiTakePicNum = 0;
                bAllowImagePostProcess = FALSE;//not support image post-process
                break;

            //case CONT_SHOT_TIMELAPSE:
            default:
                ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_SINGLE);
                ImgCap_SetUIInfo(CAP_SEL_PICNUM, 1);
                ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 1);
                ImgCap_SetUIInfo(CAP_SEL_JPG_BUF_NUM, 3);
                ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
                ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
                UI_SetData(FL_IsSingleCapture,TRUE);
                localInfo->isDoingContShot = FALSE;
                bAllowImagePostProcess = TRUE;//support image post-process
                break;
        }
    }
    else
    {
        if (UI_GetData(FL_ModeIndex) == DSC_MODE_PHOTO_SCENE &&
           ((UI_GetData(FL_SCENE) == IMAGEEFFECT_ANTISHAKE)|| (UI_GetData(FL_SCENE) == IMAGEEFFECT_HDR)))
        {
            // Anti-Shake mode
            if (UI_GetData(FL_SCENE) == IMAGEEFFECT_ANTISHAKE)
            {
                ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_MULFMANTISHAKE);
                ImgCap_SetUIInfo(CAP_SEL_PICNUM, 4);
                ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 2);
                ImgCap_SetUIInfo(CAP_SEL_JPG_BUF_NUM, 1);
                ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
                ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);

            }
            // HDR mode
            else if (UI_GetData(FL_SCENE) == IMAGEEFFECT_HDR)
            {
                ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_MULFMHDR);
                ImgCap_SetUIInfo(CAP_SEL_PICNUM, 3);
                ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 2);
                ImgCap_SetUIInfo(CAP_SEL_JPG_BUF_NUM, 1);
                ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
                ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
                AEB_SetRatio2Cap(AEB_GetRatioFromEV(SEL_AEEV_00));
            }
        }
        // single capture
        else
        {
            #if  (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
            if (Sensor_CheckExtSensor())
            {
                #if 1
                ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_DUALSENSOR);
                #else
                if(g_cap_sensor_idx == 1)
                {
                    debug_msg("cap sensor 1\r\n");
                    ImgCap_SetUIInfo(CAP_SEL_IPL_ID,  IPL_ID_1);
                    ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_SINGLE);
                }
                else if(g_cap_sensor_idx == 2)
                {
                    debug_msg("cap sensor 2\r\n");
                    ImgCap_SetUIInfo(CAP_SEL_IPL_ID,  IPL_ID_2);
                    ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_SINGLE);
                }
                else
                {
                    debug_msg("cap both sensor\r\n");
                    ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_DUALSENSOR);
                }
                #endif
            }
            else
            #endif
            {
                ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_SINGLE);
            }

            ImgCap_SetUIInfo(CAP_SEL_PICNUM, 1);
            ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 1);
            ImgCap_SetUIInfo(CAP_SEL_JPG_BUF_NUM, 3);
            ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
            ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
        }
        UI_SetData(FL_IsSingleCapture,TRUE);
        localInfo->isDoingContShot = FALSE;
        bAllowImagePostProcess = TRUE;//support image post-process
    }


    //#NT#2010/09/02#JeahYen -begin
    //config image post-process
#if(MOVIE_GDC_FUNC==ENABLE)
    if((UI_GetData(FL_MovieGdcIndex) == MOVIE_GDC_ON) && bAllowImagePostProcess)
        AppPhoto_SetData(&UIPhotoObjCtrl, _Cap2DGDC, _Cap_2D_GDC_On);
    else
        AppPhoto_SetData(&UIPhotoObjCtrl, _Cap2DGDC, _Cap_2D_GDC_Off);
#endif


#if (BRC460_FUNCTION == ENABLE)
    {
        UINT32 TargetBytes = PhotoExe_GetExpectSize_RhoBRCrtl(UI_GetData(FL_PHOTO_SIZE),TRUE);
        UINT32 UBoundBytes = TargetBytes+(TargetBytes*15/100); //+15%
        UINT32 LBoundBytes = TargetBytes-(TargetBytes*15/100); //-15%
        ImgCap_SetUIInfo(CAP_SEL_REENCTYPE, SEL_REENCTYPE_RHO);
        ImgCap_SetUIInfo(CAP_SEL_RHO_INITQF,128);
        ImgCap_SetUIInfo(CAP_SEL_RHO_TARGETSIZE,TargetBytes);
        ImgCap_SetUIInfo(CAP_SEL_RHO_LBOUNDSIZE,LBoundBytes);
        ImgCap_SetUIInfo(CAP_SEL_RHO_HBOUNDSIZE,UBoundBytes);
        ImgCap_SetUIInfo(CAP_SEL_RHO_RETRYCNT,4);
        DBG_IND("[cap]TargetBytes=%d k, H=%d k, L=%d K\r\n",TargetBytes/1024,UBoundBytes/1024,LBoundBytes/1024);
    }
#else
    ImgCap_SetUIInfo(CAP_SEL_REENCTYPE, SEL_REENCTYPE_FIXQ);
#endif

    if (localInfo->uiMaxImageSize)
    {
        ImgCap_SetUIInfo(CAP_SEL_FILEBUFMODE,SEL_FILEBUFMODE_MANUAL);
        ImgCap_SetUIInfo(CAP_SEL_FILEBUFSIZE,localInfo->uiMaxImageSize);
        DBG_IND("[cap]FileBufSize=%d k\r\n",localInfo->uiMaxImageSize/1024);
    }
    else
    {
        ImgCap_SetUIInfo(CAP_SEL_FILEBUFMODE,SEL_FILEBUFMODE_AUTO);
    }
    PhotoExe_SetScreenNailSize();
    PhotoExe_SetQuickViewSize();

    //Need to wait sound play complete
    GxSound_WaitStop();

    localInfo->isStartCapture = TRUE;
#if(LOW_POWER_TIMELAPSE_REC == ENABLE)
    //if(UI_GetData(FL_LOW_POWER_TIMELAPSE_REC)&& rtc_isPWRAlarm())
    if(rtc_isPWRAlarm())
    {
        DBG_DUMP("RTC Timelape: Setup the Photo Setting\r\n");
        ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_H_SIZE, GetPhotoSizeWidth(PHOTO_SIZE_2MHD));
        ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_V_SIZE, GetPhotoSizeHeight(PHOTO_SIZE_2MHD));
        //ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_420);
        ImgCap_SetUIInfo(CAP_SEL_PRI_FMT, IPL_IMG_Y_PACK_UV420);
    }
#endif

    /* Start to capture */
    localInfo->IsJPGok = FALSE;
    PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_CAPSTART);
    if (ImgCap_StartCapture()!=E_OK)
    {
        ImgCap_WaitFinish();
        ImgCap_StartCapture();
    }
    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnCaptureStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DBG_IND("[cap]\r\n");
    if(localInfo->isDoingContShot)
    {
        ImgCap_StopCapture();
    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnCaptureSmile(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 sdState;

    DBG_IND("[cap]\r\n");
    if (paramNum == 1)
    {
        sdState = paramArray[0];
    }
    else
    {
       DBG_ERR("Wrong param Num %d\r\n",paramNum);
       return NVTEVT_CONSUME;
    }

    switch(sdState)
    {
        case SD_START:
        {
            /* Set smile detection start flag */
            if (!localInfo->isStartSmileDetect)
            {
                localInfo->isStartSmileDetect = TRUE;
                localInfo->uiTakePicNum = 0;

                #if _DEMO_TODO
                Photo_LockASC(LOCK);
                #endif
                /* Smile detecting should unlock FD and SD */
                #if _FD_FUNC_
                FD_Lock(FALSE);
                #endif
                #if _SD_FUNC_
                SD_Lock(FALSE);
                #endif

                /* Smile detecting should unlock AE/AWB */
                AE_Lock(AE_ID_1,FALSE,FALSE);
                AWB_Lock(AWB_ID_1,FALSE,FALSE);
            }
            break;
        }

        case SD_CONTINUE:
        {
            /* Continue next smile detection need to only unlock SD again (FD/AE/AWB is already unlocked after capture completed) */
            if (localInfo->isStartSmileDetect && (localInfo->isStartCapture != TRUE))
            {
                #if _FD_FUNC_
                FD_Lock(FALSE);
                #endif
                #if _SD_FUNC_
                SD_Lock(FALSE);
                #endif
            }

            break;
        }

        case SD_STOP:
        {
            // Lock SD
            #if _SD_FUNC_
            SD_Lock(TRUE);
            #endif
            Photo_WaitIdle(PHOTO_ID_1);

            Ux_FlushEventByRange(NVTEVT_ALGMSG_SDEND,NVTEVT_ALGMSG_SDEND);

            if (localInfo->isStartCapture != TRUE)
            {
                // Unlock AE/AWB if locked
                AE_Lock(AE_ID_1,FALSE,FALSE);
                AWB_Lock(AWB_ID_1,FALSE,FALSE);
                //unlock FD when
                /* Resume FD action if early in FD process */
                if(UI_GetData(FL_FD) != FD_OFF)
                {
                    #if _FD_FUNC_
                    FD_Lock(FALSE);
                    #endif
                }
            }
            /* Reset smile detection start flag */
            localInfo->isStartSmileDetect = FALSE;

            break;
        }

        default:
            break;
    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnCaptureEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //localInfo->isStartSmileDetect = FALSE;
    DBG_IND("[cap]\r\n");
    localInfo->isDoingContShot = FALSE;
    return NVTEVT_CONSUME;
}
//#NT#2010/10/11#Photon Lin -begin
//#Add AS/PS/CS
INT32 PhotoExe_OnASPSCS(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 mode;
    mode = paramArray[0];

#if _DEMO_TODO
    AppPhoto_SetData(&UIPhotoObjCtrl, _FlashMode,      Get_FlashValue(UI_GetData(FL_FLASH_MODE)));
    AppPhoto_SetData(&UIPhotoObjCtrl, _FileFmt,        _FileFmt_Jpg);
    AppPhoto_SetData(&UIPhotoObjCtrl, _PreCapBitDepth, _SieBitDepth_8);
    AppPhoto_SetData(&UIPhotoObjCtrl, _BurstNum,       _BurstNum_1);

    switch(mode)
    {
        case 0:   //MODE_ALBUM_SHOT
            AppPhoto_SetData(&UIPhotoObjCtrl,_DriveMode,       _IMGDriveMode_AlbumShot);
        break;
        case 1:   //MODE_PRESHOT
            AppPhoto_SetData(&UIPhotoObjCtrl,_DriveMode,       _IMGDriveMode_PreShot);
        break;
        case 2:   //MODE_COUPL_SHOT
            AppPhoto_SetData(&UIPhotoObjCtrl,_DriveMode,       _IMGDriveMode_CoupleShot);
        break;
            UIAppPhotoErrMsg("%s, parameter error (%)\r\n",__func__,mode);
            AppPhoto_SetData(&UIPhotoObjCtrl,_DriveMode,       _IMGDriveMode_Single);
        break;
    }

    AppPhoto_SetData(&UIPhotoObjCtrl, _ImgSize,        Get_SizeValue(UI_GetData(FL_PHOTO_SIZE)));

    //Need to wait sound play complete
    GxSound_WaitStop();
    ImgCapture_StartCapture();
#endif

    return NVTEVT_CONSUME;
}
//#NT#2010/10/11#Photon Lin -end

INT32 PhotoExe_OnPanCapture(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _DEMO_TODO
    UINT32  uiPanDir, uiPicCnt;
    ImageAlgInfo *AlgInfo;

    AlgInfo = UI_GetParameter();

    uiPanDir = paramArray[0];
    uiPicCnt = paramArray[1];

    //#NT#2010/10/12#Lincy Lin -begin
    UiDateImprint_SetEnable(FALSE);
    //#NT#2010/10/12#Lincy Lin -end

    if (UI_GetData(FL_PanModeIndex) == PAN_AUTO)
    {
        DBG_IND("PANO:AUTO - START\r\n");

        AppPhoto_SetData(&UIPhotoObjCtrl, _FlashMode,      _IMGFlashMode_ForceOFF);
        AppPhoto_SetData(&UIPhotoObjCtrl, _FileFmt,        _FileFmt_Jpg);
        AppPhoto_SetData(&UIPhotoObjCtrl, _PreCapBitDepth, _SieBitDepth_8);
        AppPhoto_SetData(&UIPhotoObjCtrl, _BurstNum,       _BurstNum_1);
        AppPhoto_SetData(&UIPhotoObjCtrl, _DriveMode,      _IMGDriveMode_PanCapture);
        #if (PANCAPTURE_SIZE == PANCAPTURE_SIZE_2M)
        AppPhoto_SetData(&UIPhotoObjCtrl, _ImgSize,        _IMGSize_2M);
        #else
        AppPhoto_SetData(&UIPhotoObjCtrl, _ImgSize,        _IMGSize_3M);
        #endif
        //#NT#2011/03/28#Lincy Lin -begin
        AppPhoto_SetData(&UIPhotoObjCtrl,_CapAWBMode,CALCALATE_AWB);
        //#NT#2011/03/28#Lincy Lin -end

        pancap_Cmd(PANCAP_INITIAL, 0, 0);
        //#NT#2010/06/04#Photon Lin -begin
        //#Add mode selection
        pancap_Cmd(PANCAP_SET_MODE, PANCAP_MODE_AUTO, 0);
        //#NT#2010/06/04#Photon Lin -end

        pancap_Cmd(PANCAP_SET_DIR, (uiPanDir == PanCapDir_L2R)? PANCAP_DIR_L2R : PANCAP_DIR_R2L, 0);

        Photo_LockPANCAP(UNLOCK);
        pancap_Cmd(PANCAP_SET_START,ENABLE, 0);
    }
    else
    {
        DBG_IND("PANO:MANUAL - CAPTURE, uiPicCnt = %d\r\n",uiPicCnt);

        if(uiPicCnt == 0)
        {
            AppPhoto_SetData(&UIPhotoObjCtrl, _FlashMode,      _IMGFlashMode_ForceOFF);
            AppPhoto_SetData(&UIPhotoObjCtrl, _FileFmt,        _FileFmt_Jpg);
            AppPhoto_SetData(&UIPhotoObjCtrl, _PreCapBitDepth, _SieBitDepth_8);
            AppPhoto_SetData(&UIPhotoObjCtrl, _BurstNum,       _BurstNum_1);
            AppPhoto_SetData(&UIPhotoObjCtrl, _DriveMode,      _IMGDriveMode_PanCapture);
            #if (PANCAPTURE_SIZE == PANCAPTURE_SIZE_2M)
            AppPhoto_SetData(&UIPhotoObjCtrl, _ImgSize,        _IMGSize_2M);
            #else
            AppPhoto_SetData(&UIPhotoObjCtrl, _ImgSize,        _IMGSize_3M);
            #endif
            //#NT#2011/03/28#Lincy Lin -begin
            AppPhoto_SetData(&UIPhotoObjCtrl,_CapAWBMode,CALCALATE_AWB);
            //#NT#2011/03/28#Lincy Lin -end
            pancap_Cmd(PANCAP_INITIAL, 0, 0);
            //#NT#2010/06/04#Photon Lin -begin
            //#Add mode selection
            pancap_Cmd(PANCAP_SET_MODE, PANCAP_MODE_MANUAL, 0);
            //#NT#2010/06/04#Photon Lin -end

            pancap_Cmd(PANCAP_SET_DIR, (uiPanDir == PanCapDir_L2R)? PANCAP_DIR_L2R : PANCAP_DIR_R2L, 0);
            Photo_LockPANCAP(UNLOCK);
            pancap_Cmd(PANCAP_SET_START,ENABLE, 0);
            //#NT#2010/06/04#Photon Lin -begin
            //#Add mode selection
            pancap_Cap();
            //#NT#2010/06/04#Photon Lin -end

        }
        else
        {
            //#NT#2011/03/29#Photon Lin -begin
            AppPhoto_SetData(&UIPhotoObjCtrl,_CapAWBMode,KEEPLATEST_AWB);
            //#NT#2011/03/29#Photon Lin -end
            //turn off capture guide
            idec_setV2En(IDE_ID_1, FALSE);
            pancap_lock(TRUE);
            Photo_LockAE(LOCK);
            Photo_LockAWB(LOCK);
            clr_flg(FLG_ID_PHOTO, FLGPHOTO_PANCAP);

            pancap_Cap();
        }
    }
#endif

    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnZoom(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    UINT32 uiZoomCtrl=UI_ZOOM_CTRL_STOP;
    BOOL   bDZoomEnable = FALSE;
    static BOOL bIsCAFLocked = FALSE;
    #if _DEMO_TODO
    static BOOL bIsASCLocked = FALSE;
    #endif
    static UINT32 uiZoomCnt = 0;
    static UINT32 uiLastSection = 1;
    UINT32 TimeOutCnt = 0;
    UINT32 TimeOutMaxCnt = 20;

    DBG_IND("paramArray[0]=%d, paramArray[1]=%d\r\n",paramArray[0],paramArray[1]);
    if (localInfo->IsAFProcess)
    {
        DBG_ERR("Wait AF Idle\r\n");
        AF_WaitIdle(AF_ID_1);
    }
    if (paramNum == 2)
    {
        uiZoomCtrl = paramArray[0];
        bDZoomEnable = paramArray[1];
    }
    else
    {
        DBG_ERR("ParamNum %d\r\n",paramNum);
    }
    UI_SetData(FL_ZoomOzmDzmChange,FALSE);
    switch(uiZoomCtrl)
    {
        case UI_ZOOM_CTRL_STOP:

            if(UI_GetData(FL_ZoomIsDigital) == FALSE)
            {
                DBG_IND("Photo_OpticalZoom_Stop\r\n");
                if (OZOOM_IDX_GET() == OZOOM_IDX_MIN() &&
                   (OZOOM_IDX_GET() != uiLastSection))
                {
                    uiZoomCnt++;
                }

                uiLastSection = OZOOM_IDX_GET();
                Ux_FlushEventByRange(NVTEVT_CB_ZOOM, NVTEVT_CB_ZOOM);
                UI_SetData(FL_IslensStopping, TRUE);
                if(PhotoExe_LensZoomStop() != E_OK)
                {
                    if(PhotoExe_LensReset()!=E_OK)
                    {
                        DBG_ERR("lens stop and reset error!!!\r\n");
                        #if (LENSERROFF_FUNCTION == ENABLE)
                        System_PowerOff(SYS_POWEROFF_LENSERROR);
                        #endif
                    }
                    else
                    {
                        UI_SetData(FL_ZoomOzmDzmChange,TRUE);
                        Ux_PostEvent(NVTEVT_CB_ZOOM, 1, ZOOM_IF_OPTICAL);
                    }
                }
                else
                {
                    #if 0
                    if (uiZoomCnt == 10)
                    {
                        uiZoomCnt = 0;

                        if(PhotoExe_LensReset()!=E_OK)
                        {
                            DBG_ERR("lens reset error!!!\r\n");
                            #if (LENSERROFF_FUNCTION == ENABLE)
                            System_PowerOff(SYS_POWEROFF_LENSERROR);
                            #endif
                        }

                    }
                    #endif
                }
                UI_SetData(FL_IslensStopping, FALSE);

            }
            else
            {
                DBG_IND("Photo_DZoom_Stop\r\n");
                PhotoExe_DZoomStop();
            }
            if (!bIsCAFLocked)
                AFTsk_LockCAF(FALSE);
            #if _DEMO_TODO
            if (!bIsASCLocked)
                Photo_LockASC(UNLOCK);
            #endif
            break;

        case UI_ZOOM_CTRL_IN:
            bIsCAFLocked = AFTsk_IsCAFLock();
            #if _DEMO_TODO
            bIsASCLocked = Photo_IsASCLock();
            #endif
            if (!bIsCAFLocked)
                AFTsk_LockCAF(TRUE);
            #if _DEMO_TODO
            if (!bIsASCLocked)
                Photo_LockASC(LOCK);
            #endif
            //only CAF or ASC need to wait idle
            if ((!bIsCAFLocked)
            #if _DEMO_TODO
            || !bIsASCLocked
            #endif
            )
            {
                AF_WaitIdle(AF_ID_1);
            }
            // wait image effect
            #if _DEMO_TODO
            Photo_WaitPhotoIdle(TRUE, (PHOTO_1ST|PHOTO_2ND));
            #endif
            SwTimer_DelayMs(60);
            Ux_FlushEventByRange(NVTEVT_ALGMSG_ASCEND,NVTEVT_ALGMSG_ASCEND);

            if (UI_GetData(FL_ZoomIFIndex) == ZOOM_IF_DIGITAL)
            {
                PhotoExe_DZoomIn();
                UI_SetData(FL_ZoomIsDigital,TRUE);
                if (DZOOM_IDX_GET() == DZOOM_IDX_MIN())
                {
                    UI_SetData(FL_ZoomOzmDzmChange,TRUE);
                }

            }
            else if(UI_GetData(FL_ZoomIFIndex) == ZOOM_IF_OPTICAL)
            {
                TimeOutCnt = 0;
                while (TimeOutCnt != TimeOutMaxCnt)
                {
                    if (PhotoExe_LensZoomIn() == E_OK)
                    {
                        break;
                    }
                    else
                    {
                        TimeOutCnt++;
                        SwTimer_DelayMs(20);
                        DBG_IND("retry\r\n");
                    }
                }
                if(TimeOutCnt == TimeOutMaxCnt)
                {
                    DBG_ERR("time out\r\n");
                }
                UI_SetData(FL_ZoomIsDigital,FALSE);
            }
            else
            {
                if(OZOOM_IDX_GET() >= OZOOM_IDX_MAX())
                {
                    if(bDZoomEnable)
                    {
                        DBG_IND("\r\n");
                        PhotoExe_DZoomIn();
                        UI_SetData(FL_ZoomIsDigital,TRUE);
                        UI_SetData(FL_ZoomOzmDzmChange,TRUE);
                    }
                }
                else
                {
                    TimeOutCnt = 0;
                    while (TimeOutCnt != TimeOutMaxCnt)
                    {
                        if (PhotoExe_LensZoomIn() == E_OK)
                        {
                            break;
                        }
                        else
                        {
                            TimeOutCnt++;
                            SwTimer_DelayMs(20);
                            DBG_IND("retry \r\n");
                        }
                    }

                    if(TimeOutCnt == TimeOutMaxCnt)
                    {
                        DBG_ERR("time out\r\n");
                    }
                    UI_SetData(FL_ZoomIsDigital,FALSE);
                }
            }
            break;

        case UI_ZOOM_CTRL_OUT:

            bIsCAFLocked = AFTsk_IsCAFLock();
            #if _DEMO_TODO
            bIsASCLocked = Photo_IsASCLock();
            #endif
            if (!bIsCAFLocked)
                AFTsk_LockCAF(TRUE);
            #if _DEMO_TODO
            if (!bIsASCLocked)
                Photo_LockASC(LOCK);
            #endif

            if ((!bIsCAFLocked)
            #if _DEMO_TODO
            || !bIsASCLocked
            #endif
            )
            {
                AF_WaitIdle(AF_ID_1);//wait CAF idle
            }
            #if _DEMO_TODO
            // wait image effect
            Photo_WaitPhotoIdle(TRUE, (PHOTO_1ST|PHOTO_2ND));
            #endif
            SwTimer_DelayMs(60);
            Ux_FlushEventByRange(NVTEVT_ALGMSG_ASCEND,NVTEVT_ALGMSG_ASCEND);
            if (UI_GetData(FL_ZoomIFIndex) == ZOOM_IF_DIGITAL)
            {
                PhotoExe_DZoomOut();
                UI_SetData(FL_ZoomIsDigital,TRUE);
                if(DZOOM_IDX_GET() == DZOOM_IDX_MIN())
                {
                    UI_SetData(FL_ZoomOzmDzmChange,TRUE);
                    Ux_PostEvent(NVTEVT_CB_ZOOM, 1, ZOOM_IF_DIGITAL);
                }
            }
            else if(UI_GetData(FL_ZoomIFIndex) == ZOOM_IF_OPTICAL)
            {
                TimeOutCnt = 0;
                while (TimeOutCnt != TimeOutMaxCnt)
                {
                    if (PhotoExe_LensZoomOut() == E_OK)
                    {
                        break;
                    }
                    else
                    {
                        TimeOutCnt++;
                        SwTimer_DelayMs(20);
                        DBG_IND("retry \r\n");
                    }
                }

                if(TimeOutCnt == TimeOutMaxCnt)
                {
                    DBG_ERR("time out\r\n");
                }
                //#NT#2010/11/19#Photon Lin -end
                UI_SetData(FL_ZoomIsDigital,FALSE);
            }
            else
            {
                if(DZOOM_IDX_GET() <= DZOOM_IDX_MIN())
                {
                    DBG_IND("Lens_Zoom_Out\r\n");
                    TimeOutCnt = 0;
                    while (TimeOutCnt != TimeOutMaxCnt)
                    {
                        if (PhotoExe_LensZoomOut() == E_OK)
                        {
                            break;
                        }
                        else
                        {
                            TimeOutCnt++;
                            SwTimer_DelayMs(20);
                            DBG_MSG("Lens_Zoom_Out() retry \r\n");
                        }
                    }

                    if(TimeOutCnt == TimeOutMaxCnt)
                    {
                        DBG_MSG("time out Lens_Zoom_Out()\r\n");
                    }
                    UI_SetData(FL_ZoomIsDigital,FALSE);
                    UI_SetData(FL_ZoomOzmDzmChange,TRUE);
                }
                else
                {
                    DBG_IND("Photo_DZoom_Out\r\n");
                    PhotoExe_DZoomOut();
                    UI_SetData(FL_ZoomIsDigital,TRUE);
                }
            }
            break;

        case UI_ZOOM_CTRL_RESET_DZOOM:
            if((UI_GetData(FL_ZoomIsDigital) == TRUE) && (DZOOM_IDX_GET() > DZOOM_IDX_MIN()) /*&& (UI_GetData(UI_MovieHotKeyRec) != TRUE)*/)
            {
                DBG_IND("DZoom Reset\r\n");
                UI_SetData(FL_DzoomReset,TRUE);
                PhotoExe_DZoomReset();
                UI_SetData(FL_ZoomIsDigital,FALSE);
            }
            break;
        case UI_ZOOM_CTRL_RESET_OZOOM:
            uiZoomCnt = 0;
            break;
        default:
            DBG_ERR("Unknown zoom control 0x%x\r\n", uiZoomCtrl);
            break;
    }
    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnStartFunc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    UINT32   func= 0;
    UINT32   waitIdle = FALSE;

    DBG_IND("func=0x%x\r\n",paramArray[0]);
    if (localInfo->isStartCapture)
    {
        DBG_ERR("in capturing\r\n");
    }
    else if (paramNum == 2)
    {
        func = paramArray[0];
        waitIdle = paramArray[1];


        if(func & UIAPP_PHOTO_AE)
            AE_Lock(AE_ID_1,FALSE,FALSE);
        if(func & UIAPP_PHOTO_AWB)
            AWB_Lock(AWB_ID_1,FALSE,FALSE);
        #if _FD_FUNC_
        if(func & UIAPP_PHOTO_FD)
            FD_Lock(FALSE);
        #endif
        if(func & UIAPP_PHOTO_CAF)
            AFTsk_LockCAF(FALSE);
        #if _DEMO_TODO
        if(func & UIAPP_PHOTO_ASC)
            Photo_LockASC(UNLOCK);
        #endif
        if (waitIdle == TRUE)
        {
            Photo_WaitIdle(PHOTO_ID_1);
        }

    }
    else
    {
        DBG_ERR("wrong param 0x%x\r\n",paramNum);
    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnStopFunc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    //UINT32   lock= FALSE;
    UINT32   func= 0;
    UINT32   waitIdle = FALSE;

    DBG_IND("func=0x%x\r\n",paramArray[0]);
    if (paramNum == 2)
    {
        func = paramArray[0];
        waitIdle = paramArray[1];

        if(func & UIAPP_PHOTO_AE)
            AE_Lock(AE_ID_1,TRUE,FALSE);
        if(func & UIAPP_PHOTO_AWB)
            AWB_Lock(AWB_ID_1,TRUE,FALSE);
        #if _FD_FUNC_
        if(func & UIAPP_PHOTO_FD)
            FD_Lock(TRUE);
        #endif
        #if _DEMO_TODO
        if(func & UIAPP_PHOTO_ASC)
            Photo_LockASC(LOCK);
        #endif
        if(func & UIAPP_PHOTO_CAF)
            AFTsk_LockCAF(TRUE);
        if (waitIdle == TRUE)
        {
            AF_WaitIdle(AF_ID_1);
            Photo_WaitIdle(PHOTO_ID_1);
        }

    }
    else
    {
        DBG_ERR("wrong param 0x%x\r\n",paramNum);
    }
    return NVTEVT_CONSUME;
}

////////////////////////////////////////////////////////////
INT32 PhotoExe_OnAFProcess(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if LENS_FUNCTION
    UINT32 isAFBeam;
    //UINT32 isSupportPunchThrough;
    UINT32 currEV, EV_Threshold , LV;

    isAFBeam = paramArray[0];
    //#NT#2010/11/17#Lincy Lin -begin
    #if (ASSIST_BEAM_FUNC == DISABLE)
    isAFBeam = FALSE;
    #endif
    //#NT#2010/11/17#Lincy Lin -end
    //isSupportPunchThrough = paramArray[1];
    DBG_FUNC_BEGIN("[af]\r\n");
    #if _DEMO_TODO
    //#NT#2010/10/12#Lincy Lin -begin
    //#NT#
    if (localInfo->isSingleViewOpen || gImageAlgInfo.Hdr_Preview_En == _HDR_Preview_On)
    {
        SetFBLockFunc(TRUE);
    }
    else
    {
        SetFBLockFunc(FALSE);
    }
    //#NT#2010/10/12#Lincy Lin -end

    // Lock AE/AWB, and do AF process
    Photo_WaitAEStable();
    #endif
    AE_Lock(AE_ID_1,TRUE,FALSE);
    AWB_Lock(AWB_ID_1,TRUE,FALSE);
    AFTsk_LockCAF(TRUE);
    #if _DEMO_TODO
    Photo_LockASC(LOCK);
    #endif
    #if _FD_FUNC_
    //FD_Lock(TRUE);
    #endif
    #if _SD_FUNC_
    SD_Lock(TRUE);
    #endif
    //AF_WaitIdle(AF_ID_1);//wait CAF idle
    #if _DEMO_TODO
    Photo_WaitIdle(PHOTO_ID_1);
    #endif

    Ux_FlushEventByRange(NVTEVT_ALGMSG_FOCUSEND, NVTEVT_ALGMSG_FOCUSEND);
    Ux_FlushEventByRange(NVTEVT_ALGMSG_FDEND,NVTEVT_ALGMSG_FDEND);
    Ux_FlushEventByRange(NVTEVT_ALGMSG_ASCEND,NVTEVT_ALGMSG_ASCEND);

    currEV = CURR_EV();
    LV     =  currEV/10;
    EV_Threshold = AF_BEAM_EV_THRESHOLD;
    DBG_IND("[af]EV = %d, LV=%d\r\n",currEV,LV);
    if(isAFBeam && (currEV < EV_Threshold))//LV 5.6~6.1
    {
        //If AF beam is on ,then turn on the focus LED before AF_Process().
        if (!localInfo->isAFBeam)
        {
            if (LV >6)
            {
                DBG_ERR("[af]AF LV %d\r\n",LV);
                LED_SetFcsLevel(0);
            }
            else
            {
                LED_SetFcsLevel(LCSBRT_LVL_06 - LV);
            }
            LED_TurnOnLED(GPIOMAP_LED_FCS);
            localInfo->isAFBeam = TRUE;
        }
        /* AF_Run no wait. */
        AF_Run(AF_ID_1, FALSE);
        localInfo->IsAFProcess = TRUE;
    }
    else
    {
        /* AF_Run no wait. */
        AF_Run(AF_ID_1, FALSE);
        localInfo->IsAFProcess = TRUE;
    }
    DBG_FUNC_END("[af]\r\n");
#endif
    return NVTEVT_CONSUME;
}
INT32 PhotoExe_OnAFRelease(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if LENS_FUNCTION
    DBG_IND("[af]\r\n");
    AF_Release(AF_ID_1,TRUE);
    if (localInfo->isAFBeam)
    {
        LED_TurnOffLED(GPIOMAP_LED_FCS);   //If AF beam is on ,then turn off the focus LED after AF_Process().
        LED_SetFcsLevel(LCSBRT_LVL_03);
        localInfo->isAFBeam = FALSE;
    }
    DBG_FUNC_END("[af]\r\n");
#endif
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnAFWaitEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if LENS_FUNCTION
    DBG_IND("[af]\r\n");
    if (localInfo->IsAFProcess)
    {
        AF_WaitIdle(AF_ID_1);
        localInfo->IsAFProcess = FALSE;
        if (localInfo->isAFBeam)
        {
            LED_TurnOffLED(GPIOMAP_LED_FCS);   //If AF beam is on ,then turn off the focus LED after AF_Process().
            LED_SetFcsLevel(LCSBRT_LVL_03);
            localInfo->isAFBeam = FALSE;
        }
    }
    DBG_FUNC_END("[af]\r\n");
#endif
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnImageRatio(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32           ImageRatioIdx;
#if _DEMO_TODO
    USIZE            ImageRatioSize={0};
    USIZE            finalSize={0};
    URECT            finalWindow;
#else
    DISPSRV_CMD      DispCmd={0};
    DISPSRV_DISP_CFG CfgDisp={0};
    USIZE            dev1size={0},dev2size={0};
    USIZE            dev1Ratio={0},dev2Ratio={0};
    USIZE            ImageRatioSize={0};
    USIZE            finalSize={0};
    USIZE            buffer1Size,buffer2Size;
    IPL_SET_IMGRATIO_DATA   *pIPLRatioInfo =&gIPLRatioInfo;
    UINT32           CurrDevObj,CurrDev2Obj;
#endif
    BOOL   isCAFLocked;
    //BOOL   isASCLocked;
    BOOL   isWaitIdle = FALSE;

    DBG_IND("PhotoExe_OnImageRatio %d\r\n",paramArray[0]);
    ImageRatioIdx = paramArray[0];
    if (ImageRatioIdx >=IMAGERATIO_MAX_CNT)
    {
        DBG_ERR("ImageRatioIdx =%d\r\n");
        return NVTEVT_CONSUME;
    }

    isCAFLocked = AFTsk_IsCAFLock();
    #if _DEMO_TODO
    isASCLocked = Photo_IsASCLock();
    #endif
    if(!isCAFLocked)
    {
        AFTsk_LockCAF(TRUE);
        isWaitIdle = TRUE;
    }
    #if _DEMO_TODO
    if(!isASCLocked)
    {
        Photo_LockASC(LOCK);
        isWaitIdle = TRUE;
    }
    #endif
    if (isWaitIdle == TRUE)
    {
        AF_WaitIdle(AF_ID_1);;//wait CAF idle
        //Photo_WaitPhotoIdle(TRUE, (PHOTO_1ST|PHOTO_2ND|PHOTO_3RD));
    }

#if _DEMO_TODO
    ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];
    AppView_CalcBestFitInsideDisp(0, &ImageRatioSize, &finalSize, &finalWindow);
    AppView_ConfigWindow(0, finalWindow.x, finalWindow.y, finalWindow.w, finalWindow.h);
    IPLCtrl_UpdateBufferSize(0, &finalSize, Get_ImageRatioValue(ImageRatioIdx));
#else
    CurrDevObj = GxVideo_GetDevice(DOUT1);

    //1. Get Original setting
    memset(&DispCmd,0,sizeof(DISPSRV_CMD));
    DispCmd.Idx = DISPSRV_CMD_IDX_GET_DISP;
    DispCmd.Prop.bExitCmdFinish = TRUE;
    DispCmd.Out.uiNumByte = sizeof(CfgDisp);
    DispCmd.Out.pData= &CfgDisp;
    DispSrv_Cmd(&DispCmd);
    dev1size = CfgDisp.Desc[DISPSRV_DISP_IDX_PRIMARY].DeviceSize;
    dev1Ratio = CfgDisp.Desc[DISPSRV_DISP_IDX_PRIMARY].AspectRatio;
    buffer1Size = AppView_GetBufferSize(0);
    ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];

    //2. Set IPL setting
    memset(&gImeInfo,0x00,sizeof(gImeInfo));
    gImeInfo.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_IME_INFOR, (void *)&gImeInfo);

    finalSize = PhotoExe_RatioSizeConvert(&buffer1Size,&dev1Ratio,&ImageRatioSize);
    AppView_ConfigImgSize(0,finalSize.w, finalSize.h, ImageRatioSize.w, ImageRatioSize.h);
    #if PIP_VIEW_FUNC
    PipView_SetPrimaryImgRatio(&ImageRatioSize);
    #endif
    memset(pIPLRatioInfo,0x00,sizeof(IPL_SET_IMGRATIO_DATA));
    pIPLRatioInfo->Id = IPL_ID_1;
    pIPLRatioInfo->PathId = IPL_IME_PATH_NONE;
    pIPLRatioInfo->PathId |= IPL_IME_PATH2;
    pIPLRatioInfo->CropRatio = Get_ImageRatioValue(ImageRatioIdx);
    pIPLRatioInfo->ImeP2.OutputEn = TRUE;
    pIPLRatioInfo->ImeP2.ImgSizeH = finalSize.w;
    pIPLRatioInfo->ImeP2.ImgSizeV = finalSize.h;
    pIPLRatioInfo->ImeP2.ImgSizeLOfs = finalSize.w;
    pIPLRatioInfo->ImeP2.ImgFmt = gImeInfo.ImeP2[0].type;

    //config disp2 view
    CurrDev2Obj = GxVideo_GetDevice(DOUT2);
    if (CurrDev2Obj)
    {
        dev2size = CfgDisp.Desc[DISPSRV_DISP_IDX_SECONDARY].DeviceSize;
        dev2Ratio = CfgDisp.Desc[DISPSRV_DISP_IDX_SECONDARY].AspectRatio;
        buffer2Size = AppView_GetBufferSize(1);
        finalSize = PhotoExe_RatioSizeConvert(&buffer2Size,&dev2Ratio,&ImageRatioSize);
        AppView_ConfigImgSize(1,finalSize.w, finalSize.h, ImageRatioSize.w, ImageRatioSize.h);
        pIPLRatioInfo->PathId |= IPL_IME_PATH3;
        pIPLRatioInfo->ImeP3.OutputEn = TRUE;
        pIPLRatioInfo->ImeP3.ImgSizeH = finalSize.w;
        pIPLRatioInfo->ImeP3.ImgSizeV = finalSize.h;
        pIPLRatioInfo->ImeP3.ImgSizeLOfs = finalSize.w;
        pIPLRatioInfo->ImeP3.ImgFmt = gImeInfo.ImeP3[0].type;
    }
    // Isiah, Wi-Fi Dual recording test.
    #if(HTTP_LIVEVIEW_FUNC && MOVIE_LIVEVIEW!=DUAL_REC_HTTP_LIVEVIEW)
    {
        if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
        {
            pIPLRatioInfo->PathId |= IPL_IME_PATH3;
            pIPLRatioInfo->ImeP3.OutputEn = TRUE;
            pIPLRatioInfo->ImeP3.ImgFmt = IPL_IMG_Y_PACK_UV420;

            if(Get_ImageRatioValue(ImageRatioIdx)==IPL_SENCROPRATIO_16_9)
            {
                pIPLRatioInfo->ImeP3.ImgSizeH = HTTP_MJPG_W_16;
                pIPLRatioInfo->ImeP3.ImgSizeV = HTTP_MJPG_H_9;
                pIPLRatioInfo->ImeP3.ImgSizeLOfs = HTTP_MJPG_W_16;
            }
            else
            {
                pIPLRatioInfo->ImeP3.ImgSizeH = HTTP_MJPG_W_4;
                pIPLRatioInfo->ImeP3.ImgSizeV = HTTP_MJPG_H_3;
                pIPLRatioInfo->ImeP3.ImgSizeLOfs = HTTP_MJPG_W_4;
            }
        }
    }
    #endif

    IPL_SetCmd(IPL_SET_IMGRATIO, (void *)pIPLRatioInfo);
    IPL_WaitCmdFinish();
#endif

    //3. Set Capture setting
    ImgCap_SetUIInfo(CAP_SEL_IMG_RATIO, Get_ImageRatioValue(ImageRatioIdx));

    //4. Set Fd image ratio
    #if _FD_FUNC_
    {
        URECT fdDispCoord;

        finalSize = PhotoExe_RatioSizeConvert(&dev1size,&dev1Ratio,&ImageRatioSize);
        fdDispCoord.w = finalSize.w;
        fdDispCoord.h = finalSize.h;
        if (finalSize.w == dev1size.w)
        {
            fdDispCoord.x = 0;
            fdDispCoord.y = (dev1size.h - finalSize.h)>>1;
        }
        else
        {
            fdDispCoord.y = 0;
            fdDispCoord.x = (dev1size.w - finalSize.w)>>1;

        }
        //FD_SetDispCoord(&fdDispCoord);
        localInfo->FdDispCoord = fdDispCoord;
    }
    #endif

    if(!isCAFLocked)
    {
        AFTsk_LockCAF(FALSE);

    }
    #if _DEMO_TODO
    if(!isASCLocked)
    {
        Photo_LockASC(UNLOCK);
    }
    #endif

    return NVTEVT_CONSUME;
}



INT32 PhotoExe_OnCallback(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    NVTEVT event;

    event = paramArray[0];

    DBG_IND("[cb]%d\r\n",event);
    switch(event)
    {
        case NVTEVT_ALGMSG_FLASH:
            DBG_IND("[cb]NVTEVT_ALGMSG_FLASH\r\n");
            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FLASH);
            break;

        case NVTEVT_ALGMSG_QVSTART:

            DBG_IND("[cb]NVTEVT_ALGMSG_QVSTART\r\n");
            //Charge flash
            if( UI_GetData(FL_FLASH_MODE) != FLASH_OFF)
            {
                 //#NT#2011/04/15#Lincy Lin -begin
                 //#NT#Hera14 HW bug , no battery insert can't charge for flash
                 if (!UI_GetData(FL_IsStopCharge))
                 //#NT#2011/04/15#Lincy Lin -end
                 {
                     SxTimer_SetFuncActive(SX_TIMER_DET_RECHARGE_ID,TRUE);
                     GxFlash_StartCharge();
                 }
            }
            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_QVSTART);
            break;

        case NVTEVT_ALGMSG_JPGOK:

            DBG_IND("[cb]NVTEVT_ALGMSG_JPGOK\r\n");

            // add picture count 1
            UI_SetData(FL_TakePictCnt,UI_GetData(FL_TakePictCnt)+1);
            localInfo->IsJPGok = TRUE;

            localInfo->isFolderFull = UIStorageCheck(STORAGE_CHECK_FOLDER_FULL, NULL);
            localInfo->isCardFull = UIStorageCheck(STORAGE_CHECK_FULL, &localInfo->FreePicNum);
            if(localInfo->isDoingContShot)
            {
                pPhotoExeInfo->uiTakePicNum++;
                /*
                if((GxKey_GetData(GXKEY_NORMAL_KEY) & FLGKEY_SHUTTER2) == 0 )
                {
                    if (UI_GetData(FL_ContShotIndex) != CONT_SHOT_10)
                    {
                        PhotoExe_StopContShot();
                    }
                }
                else */if(localInfo->isFolderFull || localInfo->isCardFull)
                {
                    PhotoExe_StopContShot();
                }
                else if (UI_GetData(FL_ContShotIndex) == CONT_SHOT_10 && pPhotoExeInfo->uiTakePicNum >=10)
                {
                    PhotoExe_StopContShot();
                }
            }
            else
            {
                localInfo->BDstatus = PhotoExe_CheckBD();
            }
            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_JPGOK);
            break;

        case NVTEVT_ALGMSG_CAPFSTOK:
        {
            INT32  FSsts = paramArray[1];
            DBG_IND("[cb]NVTEVT_ALGMSG_CAPFSTOK\r\n");
            localInfo->isFolderFull = UIStorageCheck(STORAGE_CHECK_FOLDER_FULL, NULL);
            localInfo->isCardFull = UIStorageCheck(STORAGE_CHECK_FULL, &localInfo->FreePicNum);

            if (FSsts != FST_STA_OK)
            {
               UI_SetData(FL_FSStatus, FS_DISK_ERROR);
            }
            if (localInfo->isDoingContShot == TRUE)
            {
                if(localInfo->isFolderFull || localInfo->isCardFull)
                {
                    PhotoExe_StopContShot();
                }
            }
            #if (WIFI_AP_FUNC==ENABLE)
            if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
            {
                WifiCmd_Done(WIFIFLAG_CAPTURE_DONE,E_OK);
            }
            #endif

            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FSTOK);
        }
            break;


        case NVTEVT_ALGMSG_CAPTUREEND:
            DBG_IND("[cb]NVTEVT_ALGMSG_CAPTUREEND \r\n");
            localInfo->isStartCapture = FALSE;
            localInfo->isStopingContShot = FALSE;
            //Disable USB detection
            SxTimer_SetFuncActive(SX_TIMER_DET_USB_ID,TRUE);
            //Disable USB detection
            SxTimer_SetFuncActive(SX_TIMER_DET_TV_ID,TRUE);
            //Disable Mode detection
            SxTimer_SetFuncActive(SX_TIMER_DET_MODE_ID,TRUE);
            //clear fd number
            #if _FD_FUNC_
            FD_ClrRsltFaceNum();
            #endif

            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_CAPTUREEND);
            localInfo->isDoingContShot = FALSE;
            break;
        case NVTEVT_ALGMSG_SLOWSHUTTER:
            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_SLOWSHUTTER);
            break;

        case NVTEVT_ALGMSG_DZOOMSTEP:
            PhotoExe_DZoomContinue();
            break;

        case NVTEVT_ALGMSG_PREVIEW_STABLE:
            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_PREVIEW_STABLE);
            break;

        default:

            break;
    }
    return NVTEVT_PASS;
}


INT32 PhotoExe_OnFDEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DBG_IND("[cb]\r\n");

    if((!localInfo->isStartCapture) && (UI_GetData(FL_FD) != FD_OFF))
    {
        //Flush FD event before draw
        Ux_FlushEventByRange(NVTEVT_ALGMSG_FDEND,NVTEVT_ALGMSG_FDEND);
        PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FDEND);
    }
    return NVTEVT_CONSUME;

}

INT32 PhotoExe_OnSDEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DBG_IND("[cb]\r\n");
    /* Set start capture flag */
    if (localInfo->isStartSmileDetect)
    {
        if ((!localInfo->isFolderFull) && (!localInfo->isCardFull))
        {
            //fix SD cap flash sometimes not trigger
            if( (UI_GetData(FL_FLASH_MODE)!=FLASH_OFF) &&  (GxFlash_IsCharging() || GxFlash_IsChargeSuspended()))
            {
                DBG_ERR("Flash is charging!  %d,%d\r\n",GxFlash_IsCharging(),GxFlash_IsChargeSuspended());
            }
            else
            {
                PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_SDEND);
                Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_START, 1, ON_CAPTURE_SD_CAPTURE);
            }
        }
    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnFocusEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if LENS_FUNCTION
//#NT#2010/09/03#Jeffery Chuang -begin
    #if _DEMO_TODO
    AF_SaveAFInfo();
    #endif
//#NT#2010/09/03#Jeffery Chuang -end
    DBG_IND("[cb]\r\n");
    localInfo->IsAFProcess = FALSE;
    if (localInfo->isAFBeam)
    {
        LED_TurnOffLED(GPIOMAP_LED_FCS);   //If AF beam is on ,then turn off the focus LED after AF_Process().
        LED_SetFcsLevel(LCSBRT_LVL_03);
        localInfo->isAFBeam = FALSE;
    }
    PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FOCUSEND);
#endif
    return NVTEVT_PASS;
}

INT32 PhotoExe_OnASCEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DBG_IND("[cb]\r\n");
    PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_ASCEND);
    Ux_FlushEventByRange(NVTEVT_ALGMSG_ASCEND,NVTEVT_ALGMSG_ASCEND);
    return NVTEVT_CONSUME;
}



INT32 PhotoExe_OnNotifyPrepareCap(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _DEMO_TODO
    DBG_IND("[cb]\r\n");
    ImgCapture_NotifyPrepareCapture();
#endif

    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnInitDateBuf(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if (UI_STYLE==UI_STYLE_DRIVER)
    if (DATEIMPRINT_OFF != UI_GetData(FL_DATE_STAMP))
    #else
    if (DATEIMPRINT_OFF != UI_GetData(FL_DATE_STAMP) && (UI_GetData(FL_ContShotIndex) != CONT_SHOT_10))
    #endif
    {
        UiDateImprint_InitBuff();
    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnGenDateStr(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if (UI_STYLE==UI_STYLE_DRIVER)
    if (DATEIMPRINT_OFF != UI_GetData(FL_DATE_STAMP))
    #else
    if (DATEIMPRINT_OFF != UI_GetData(FL_DATE_STAMP) && (UI_GetData(FL_ContShotIndex) != CONT_SHOT_10))
    #endif
    {
        UiDateImprint_UpdateDate();
    }
    return NVTEVT_CONSUME;
}



INT32 PhotoExe_OnGenDatePic(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(paramNum!=1)
    {
        //error parameter
        return NVTEVT_CONSUME;
    }
    #if (UI_STYLE==UI_STYLE_DRIVER)
    if (DATEIMPRINT_OFF != UI_GetData(FL_DATE_STAMP))
    #else
    if (DATEIMPRINT_OFF != UI_GetData(FL_DATE_STAMP) && (UI_GetData(FL_ContShotIndex) != CONT_SHOT_10))
    #endif
    {
        UiDateImprint_GenData((IMG_CAP_DATASTAMP_INFO*)paramArray[0]);
    }
    return NVTEVT_CONSUME;
}



//#NT#2010/10/20#JeahYen -begin
#define PHOTOFRAME_IMAGE_W 640//src:Photoframe image width
#define PHOTOFRAME_IMAGE_H 480//src:Photoframe image height
//#NT#2010/10/20#JeahYen -end

INT32 PhotoExe_OnPhotoFrame(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _DEMO_TODO
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32                  BufSize/*,fileSize*/;
    UINT32                  uiBuffer;
    UINT16                  uiWidth;
    UINT16                  uiHeight;
    UINT32                  command;
    UINT32                  PhotoFrameIndex;
    IMAGE_BORDER_INFO       BorderInfo;
    //PhotoFrameInfoType      PFInfo = {0};
    AlgMsgInfo              *pAlgMsgInfo = GetAlgMsgInfo();
    //#NT#2010/09/28#JeahYen -begin
    ISIZE BufferSize;
    //#NT#2010/09/28#JeahYen -end
    //#NT#2010/10/20#JeahYen -begin
    BufferSize.w = PHOTOFRAME_IMAGE_W;
    BufferSize.h = PHOTOFRAME_IMAGE_H;
    //#NT#2010/10/20#JeahYen -end

    DBG_IND("%d ,Buf = 0x%x, size=0x%x \r\n",paramArray[0],pAlgMsgInfo->IB_SRCBuffer,pAlgMsgInfo->IB_SRCBufferSize);

    if(paramNum!=1)
    {
        //error parameter
        return NVTEVT_CONSUME;
    }
    command = paramArray[0];

    if (command == UIAPPPHOTO_FRAME_CMD_LOADPRV_ADDR)
    {
        gPhotoFrameBuf = pAlgMsgInfo->IB_SRCBuffer;
        gPhotoFrameBufSize = pAlgMsgInfo->IB_SRCBufferSize;
        DBG_IND("Buf = 0x%x, size=0x%x \r\n",pAlgMsgInfo->IB_SRCBuffer,pAlgMsgInfo->IB_SRCBufferSize);
    }
    else if (command == UIAPPPHOTO_FRAME_CMD_LOADPRV || command == UIAPPPHOTO_FRAME_CMD_LOADCAP)
    {
        if (command == UIAPPPHOTO_FRAME_CMD_LOADPRV)
        {
            PhotoFrameIndex = UI_GetData(FL_PhotoFrameIndex);
            if (gPhotoFrameBuf)
            {
                uiBuffer = gPhotoFrameBuf+BufferSize.w*BufferSize.h*2; // 640x240x2;
                BufSize =  gPhotoFrameBufSize;
            }
            else
            {
                uiBuffer = OS_GetMempoolAddr(POOL_ID_SIDC);
                uiBuffer +=BufferSize.w*BufferSize.h*2; // 640x240x2;
                BufSize = POOL_SIZE_SIDC;
            }
        }
        else
        {
            PhotoFrameIndex = UI_GetData(FL_PhotoFrameIndex)+PHOTO_FRAME_COUNT;
            //PhotoFrameIndex = UI_GetData(FL_PhotoFrameIndex);
            if (!localInfo->isPhotoFrameOn)
            {
                DBG_IND("PF is not on\r\n");
                return NVTEVT_CONSUME;
            }
            else
            {
                uiBuffer = pAlgMsgInfo->IB_SRCBuffer;
                BufSize = pAlgMsgInfo->IB_SRCBufferSize;
                DBG_IND("Buf = 0x%x, size=0x%x \r\n",pAlgMsgInfo->IB_SRCBuffer,pAlgMsgInfo->IB_SRCBufferSize);
            }
        }

        // Read system info from PStore
        if ( (UI_GetData(FL_PhotoFrameWriteSts) == TRUE) && (pSection = PStore_OpenSection("PHFE", PS_RDONLY)) != E_PS_SECHDLER)
        {
            //PStore_ReadSection((UINT8 *)&PFInfo, 0, sizeof(PFInfo), pSection);
            PStore_ReadSection((UINT8 *)&uiWidth, gPFInfo.Data[PhotoFrameIndex].offset+8, sizeof(uiWidth), pSection);
            PStore_ReadSection((UINT8 *)&uiHeight, gPFInfo.Data[PhotoFrameIndex].offset+10, sizeof(uiHeight), pSection);
            PStore_ReadSection((UINT8 *)uiBuffer, gPFInfo.Data[PhotoFrameIndex].offset+12, gPFInfo.Data[PhotoFrameIndex].size, pSection);
            DBG_IND("PF offset = 0x%x, PF size=0x%x \r\n",gPFInfo.Data[PhotoFrameIndex].offset,gPFInfo.Data[PhotoFrameIndex].size);
            PStore_CloseSection(pSection);

        }
        else
        {
            DBG_ERR("Read PHFE fail\r\n");
            return NVTEVT_CONSUME;
        }
        // Set photo frame selection JPEG to image pipeline
        BorderInfo.uiBorderMemSize          = BufSize;
        BorderInfo.uiBorderFileAddr         = (UINT32)uiBuffer;
        BorderInfo.uiBorderFileSize         = gPFInfo.Data[PhotoFrameIndex].size;//fileSize;
        BorderInfo.uiBorderFileImageW       = uiWidth;
        BorderInfo.uiBorderFileImageH       = uiHeight;
        BorderInfo.uiBorderColorKeyY        = IB_CKEY_Y;
        BorderInfo.uiBorderColorKeyCb       = IB_CKEY_CB;
        BorderInfo.uiBorderColorKeyCr       = IB_CKEY_CR;
        BorderInfo.SetBorderMode            = FALSE;
        BorderInfo.bScaleWhileCap           = TRUE;
        BorderInfo.bYOperationIsLess        = TRUE;
        //#NT#2010/09/28#JeahYen -begin
        BorderInfo.uiPrvBorderHsize         = BufferSize.w;
        BorderInfo.uiPrvBorderVsize         = BufferSize.h;
        //#NT#2010/09/28#JeahYen -end
        BorderInfo.uiPrvBorderColorKeyEn    = TRUE;
        BorderInfo.uiPrvBorderOffset        = 0;
        PFLib_ConfigePhotoFrame(&BorderInfo);
        if (command == UIAPPPHOTO_FRAME_CMD_LOADPRV)
        {
            memcpy((UINT8*)gPhotoFrameBuf,(UINT8*)gImageBorder.PrvBorderAddrY, BufferSize.w*BufferSize.h*2);
        }
    }
    else if(command == UIAPPPHOTO_FRAME_CMD_EN)
    {
        PFLib_EnablePhotoFrame(ENABLE);
        localInfo->isPhotoFrameOn = TRUE;
    }
    else if(command == UIAPPPHOTO_FRAME_CMD_DIS)
    {
        PFLib_EnablePhotoFrame(DISABLE);
        localInfo->isPhotoFrameOn = FALSE;
    }
#endif
    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnIDPostProcess(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _DEMO_TODO
    AlgMsgInfo              *pAlgMsgInfo = GetAlgMsgInfo();
    tUT_SCALE_SET            Scale = {0};
    UINT32                   i;

    DBG_IND("PhotoExe_OnIDPostProcess\r\n");

    //Input Settings
    Scale.InAddrY  = pAlgMsgInfo->postproc_info.Info1.Addr[0]+688+pAlgMsgInfo->postproc_info.Info1.LineOffset[0]*162;
    Scale.InAddrCb = pAlgMsgInfo->postproc_info.Info1.Addr[1]+344+pAlgMsgInfo->postproc_info.Info1.LineOffset[1]*81;
    Scale.InAddrCr = pAlgMsgInfo->postproc_info.Info1.Addr[2]+344+pAlgMsgInfo->postproc_info.Info1.LineOffset[2]*81;
    Scale.InWidth  = 1216;
    Scale.InHeight = 1620;
    Scale.InLineOffsetY = pAlgMsgInfo->postproc_info.Info1.LineOffset[0];
    Scale.InLineOffsetUV = pAlgMsgInfo->postproc_info.Info1.LineOffset[1];
    Scale.InFormat = pAlgMsgInfo->postproc_info.Info1.Format;

    //OutPut Settings
    /*
    Scale.OutWidth  = 608;
    Scale.OutHeight = 816;
    Scale.OutLineOffsetY = 608;
    Scale.OutLineOffsetUV = 304;
    */
    Scale.OutWidth  = photoIDrect[0].w;
    Scale.OutHeight = photoIDrect[0].h;
    Scale.OutLineOffsetY = photoIDrect[0].w;
    Scale.OutLineOffsetUV = Scale.OutLineOffsetY >> 1;

    Scale.OutAddrY  = pAlgMsgInfo->postproc_info.Info2.Addr[0];
    Scale.OutAddrCb = Scale.OutAddrY+Scale.OutLineOffsetY*Scale.OutHeight;
    Scale.OutAddrCr = Scale.OutAddrCb+Scale.OutLineOffsetUV*Scale.OutHeight;
    Scale.OutFormat = pAlgMsgInfo->postproc_info.Info1.Format;
    UtImageScaleByIme(&Scale);


    grph_open();

    // clear background to white
    grph_setImg1(pAlgMsgInfo->postproc_info.Info1.Addr[0], pAlgMsgInfo->postproc_info.Info1.LineOffset[0], pAlgMsgInfo->postproc_info.Info1.Height, pAlgMsgInfo->postproc_info.Info1.Width);
    grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0xFFFFFFFF);
    grph_setImg1(pAlgMsgInfo->postproc_info.Info1.Addr[1], pAlgMsgInfo->postproc_info.Info1.LineOffset[1], pAlgMsgInfo->postproc_info.Info1.Height, pAlgMsgInfo->postproc_info.Info1.Width>>1);
    grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0x80808080);
    grph_setImg1(pAlgMsgInfo->postproc_info.Info1.Addr[2], pAlgMsgInfo->postproc_info.Info1.LineOffset[2], pAlgMsgInfo->postproc_info.Info1.Height, pAlgMsgInfo->postproc_info.Info1.Width>>1);
    grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0x80808080);

    for (i=0;i<4;i++)
    {
        grph_setImg1(Scale.OutAddrY,Scale.OutLineOffsetY,Scale.OutHeight, Scale.OutWidth);
        grph_setImg2(pAlgMsgInfo->postproc_info.Info1.Addr[0]+pAlgMsgInfo->postproc_info.Info1.LineOffset[0]*photoIDrect[i].y+photoIDrect[i].x,pAlgMsgInfo->postproc_info.Info1.LineOffset[0]);
        grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

        grph_setImg1(Scale.OutAddrCb,Scale.OutLineOffsetUV,Scale.OutHeight>>1,Scale.OutWidth>>1);
        grph_setImg2(pAlgMsgInfo->postproc_info.Info1.Addr[1]+((pAlgMsgInfo->postproc_info.Info1.LineOffset[1]*photoIDrect[i].y+photoIDrect[i].x)>>1),pAlgMsgInfo->postproc_info.Info1.LineOffset[1]);
        grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

        grph_setImg1(Scale.OutAddrCr,Scale.OutLineOffsetUV,Scale.OutHeight>>1,Scale.OutWidth>>1);
        grph_setImg2(pAlgMsgInfo->postproc_info.Info1.Addr[2]+((pAlgMsgInfo->postproc_info.Info1.LineOffset[2]*photoIDrect[i].y+photoIDrect[i].x)>>1),pAlgMsgInfo->postproc_info.Info1.LineOffset[2]);
        grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
    }
    grph_close();


    Scale.InAddrY  = Scale.OutAddrY;
    Scale.InAddrCb = Scale.OutAddrCb;
    Scale.InAddrCr = Scale.OutAddrCr;
    Scale.InWidth  = Scale.OutWidth;
    Scale.InHeight = Scale.OutHeight;
    Scale.InLineOffsetY = Scale.OutLineOffsetY;
    Scale.InLineOffsetUV = Scale.OutLineOffsetUV;
    Scale.InFormat = Scale.OutFormat;

    //OutPut Settings
    Scale.OutWidth  = photoIDrect[4].w;
    Scale.OutHeight = photoIDrect[4].h;
    Scale.OutLineOffsetY = photoIDrect[4].w;
    Scale.OutLineOffsetUV = Scale.OutLineOffsetY >> 1;
    Scale.OutAddrY  = pAlgMsgInfo->postproc_info.Info2.Addr[2];
    Scale.OutAddrCb = Scale.OutAddrY+Scale.OutLineOffsetY*Scale.OutHeight;
    Scale.OutAddrCr = Scale.OutAddrCb+Scale.OutLineOffsetUV*Scale.OutHeight;

    Scale.OutFormat = pAlgMsgInfo->postproc_info.Info1.Format;
    UtImageScaleByIme(&Scale);

    grph_open();
    for (i=4;i<8;i++)
    {
        grph_setImg1(Scale.OutAddrY,Scale.OutLineOffsetY,Scale.OutHeight, Scale.OutWidth);
        grph_setImg2(pAlgMsgInfo->postproc_info.Info1.Addr[0]+pAlgMsgInfo->postproc_info.Info1.LineOffset[0]*photoIDrect[i].y+photoIDrect[i].x,pAlgMsgInfo->postproc_info.Info1.LineOffset[0]);
        grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

        grph_setImg1(Scale.OutAddrCb,Scale.OutLineOffsetUV,Scale.OutHeight>>1,Scale.OutWidth>>1);
        grph_setImg2(pAlgMsgInfo->postproc_info.Info1.Addr[1]+((pAlgMsgInfo->postproc_info.Info1.LineOffset[1]*photoIDrect[i].y+photoIDrect[i].x)>>1),pAlgMsgInfo->postproc_info.Info1.LineOffset[1]);
        grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

        grph_setImg1(Scale.OutAddrCr,Scale.OutLineOffsetUV,Scale.OutHeight>>1,Scale.OutWidth>>1);
        grph_setImg2(pAlgMsgInfo->postproc_info.Info1.Addr[2]+((pAlgMsgInfo->postproc_info.Info1.LineOffset[2]*photoIDrect[i].y+photoIDrect[i].x)>>1),pAlgMsgInfo->postproc_info.Info1.LineOffset[2]);
        grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
    }
    grph_close();


#endif
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnSingleViewOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _DEMO_TODO
    if((Prv_getPostProcessingStatus() == Prv_PostProcessing_ON))
    {
        localInfo->isPrvPostProcess = FALSE;
        if(Prv_PostProcessing_Close() != E_OK)
        {
            DBG_ERR("Prv_PostProcessing_Close ...error\r\n");
        }
    }
    if (!localInfo->isSingleViewOpen)
    {
        tPHOTO_SINGLE_VIEW_INIT InitSIV;

        if (DevCtrl_GetIPLMode() != DEVCTRL_IPLMODE_OFF)
        {
            DBG_IND("\r\n");
            InitSIV.Mode = IE_MODE_ARF;
            InitSIV.Index = IE_INDEX_ARF_FishEye;
            Photo_SingleView_Open(PHOTO_VIEWTYPE_NORMAL, &InitSIV);
            //#NT#2010/11/10#Lincy Lin -begin
            //avoid seeing garbage screen
            sie_waitVD(5);
            //#NT#2010/11/10#Lincy Lin -end
            localInfo->isSingleViewOpen = TRUE;
        }

    }
#endif

    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnSingleViewClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _DEMO_TODO
    if (localInfo->isSingleViewOpen)
    {
        DBG_IND("\r\n");
        Photo_SingleView_Close(PHOTO_VIEWTYPE_NORMAL);
        localInfo->isSingleViewOpen = FALSE;
    }
#endif

    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnPrvPostProcessOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _DEMO_TODO
    if (localInfo->isSingleViewOpen)
    {
        DBG_IND("\r\n");
        Photo_SingleView_Close(PHOTO_VIEWTYPE_NORMAL);
        localInfo->isSingleViewOpen = FALSE;
    }
    DBG_IND("%d,%d\r\n",Prv_getPostProcessingFunEn(_PrvPost_HDR),Prv_getPostProcessingStatus());
    if (!localInfo->isPrvPostProcess)
    {
        localInfo->isPrvPostProcess = TRUE;
        if (DevCtrl_GetIPLMode() != DEVCTRL_IPLMODE_OFF)
        {
            if(Prv_PostProcessing_Open() != E_OK)
            {
                DBG_ERR("\r\n");
            }
        }
    }
    /*
    if((Prv_getPostProcessingFunEn(_PrvPost_HDR) == _HDR_Preview_On) && (Prv_getPostProcessingStatus() == Prv_PostProcessing_OFF))
    {
        if(Prv_PostProcessing_Open() != E_OK)
        {
            UIAppPhotoErrMsg("Prv_PostProcessing_Open ...error\r\n");
        }
    }
    */
#endif

    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnPrvPostProcessClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _DEMO_TODO
    DBG_IND("\r\n");
    //if((Prv_getPostProcessingStatus() == Prv_PostProcessing_ON))
    if (localInfo->isPrvPostProcess)
    {
        localInfo->isPrvPostProcess = FALSE;
        if(Prv_PostProcessing_Close() != E_OK)
        {
            DBG_ERR("\r\n");
        }
    }
#endif

    return NVTEVT_CONSUME;
}

//#NT#2010/10/15#Jeah Yen -begin
INT32 PhotoExe_OnDis(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_MovieDisIndex,uhSelect);
    if(uhSelect == MOVIE_DIS_ON )
    {
        UI_SetData(FL_MovieRSCIndex,MOVIE_RSC_OFF);
    }
    return NVTEVT_CONSUME;
}
INT32 PhotoExe_OnMCTF(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MovieMCTFIndex, uiSelect);

    return NVTEVT_CONSUME;
}
INT32 PhotoExe_OnRSC(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MovieRSCIndex, uiSelect);
    if(uiSelect == MOVIE_RSC_ON )
    {
        UI_SetData(FL_MovieDisIndex,MOVIE_DIS_OFF);
    }
    return NVTEVT_CONSUME;
}

//#NT#2010/12/17#Lincy Lin -begin
//#NT#
INT32 PhotoExe_OnMovieGdc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
//#NT#2010/12/17#Lincy Lin -end
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_MovieGdcIndex,uhSelect);
#if(MOVIE_GDC_FUNC ==ENABLE)
    if (uhSelect == MOVIE_GDC_ON)
        IPL_AlgSetUIInfo(IPL_SEL_GDCCOMP,SEL_GDCCOMP_ON);
    else
        IPL_AlgSetUIInfo(IPL_SEL_GDCCOMP,SEL_GDCCOMP_OFF);
#endif
    return NVTEVT_CONSUME;
}

//#NT#2010/12/17#Lincy Lin -begin
INT32 PhotoExe_OnGdc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);
    if (uhSelect == TRUE)
        IPL_AlgSetUIInfo(IPL_SEL_GDCCOMP,SEL_GDCCOMP_ON);
    else
        IPL_AlgSetUIInfo(IPL_SEL_GDCCOMP,SEL_GDCCOMP_OFF);
    return NVTEVT_CONSUME;
}
//#NT#2010/12/17#Lincy Lin -end

INT32 PhotoExe_OnSmear(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_MovieSmearIndex,uhSelect);
#if(MOVIE_SMEAR_R_FUNC ==ENABLE)
    AppPhoto_SetData(&UIPhotoObjCtrl, _SmearModeEn,
        (uhSelect==MOVIE_SMEAR_ON)?_SmearMode_on:_SmearMode_off);
#endif
    return NVTEVT_CONSUME;
}
//#NT#2010/10/15#Jeah Yen -end

INT32 PhotoExe_OnObjTrack(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if OBJECT_TRACKING_FUNC
    UINT32 uhValue = 0;
    UIAppPhotoTraceMsg("PhotoExe_OnObjTrack %d \r\n",paramArray[0]);
    if(paramNum>0)
        uhValue= paramArray[0];
    if (uhValue == OBJTRACK_START)
    {
        STStart();
    }
    else
    {
        STStop();
    }
    #endif
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnDscMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);
    IPL_AlgSetUIInfo(IPL_SEL_DSCMODE,Get_DscModeValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnSharpness(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);
    IPL_AlgSetUIInfo(IPL_SEL_SHARPNESS,Get_SharpnessValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnSaturation(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);
    IPL_AlgSetUIInfo(IPL_SEL_SATURATION,Get_SaturationValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnImageEffect(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);
    if (UI_GetData(FL_ModeIndex) == DSC_MODE_PHOTO_SCENE)
    {
        // Anti-Shake or HDR can't support Cont Shot
        if (uhSelect == IMAGEEFFECT_ANTISHAKE || uhSelect == IMAGEEFFECT_HDR)
        {
            UI_SetData(FL_ContShotIndex,CONT_SHOT_OFF);
            Photo_SetUserIndex(PHOTO_USR_CONTSHOT,CONT_SHOT_OFF);
        }
    }
    IPL_AlgSetUIInfo(IPL_SEL_IMAGEEFFECT,Get_ImageEffectValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_On3DNR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if (THREEDNR_FUNC== ENABLE)
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);

    UI_SetData(FL_THREEDNRIndex,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_3DNR,uhSelect);
    //#if _DEMO_TODO
    IPL_AlgSetUIInfo(IPL_SEL_3DNR,Get_3DNRValue(uhSelect));
    //#endif
    #endif
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnSuperResolution(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if (SUPER_RESOLUTION_FUNC== ENABLE)
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);

    UI_SetData(FL_SuperResolutionIndex,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_SUPER_RESOLUTION,uhSelect);
    IPL_AlgSetUIInfo(IPL_SEL_SUPERRESOL,Get_SuperResolutionValue(uhSelect));
    #endif
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnPlayShutterSound(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if (UI_STYLE==UI_STYLE_DEMO)
    if (UI_GetData(FL_ContShotIndex) == CONT_SHOT_10)
    {
        if (!localInfo->isStartHighSpeedTone)
        {
            UISound_Play(PLAYSOUND_SOUND_HIGHSPEED_SHUTTER_TONE);
            localInfo->isStartHighSpeedTone = TRUE;
        }
    }
    else
    {
        // Anti-Shake or HDR play high speed tone
        if ((UI_GetData(FL_ModeIndex) == DSC_MODE_PHOTO_SCENE) && (UI_GetData(FL_SCENE) == IMAGEEFFECT_ANTISHAKE || UI_GetData(FL_SCENE) == IMAGEEFFECT_HDR))
        {
            //GxSound_Stop();
            UISound_Play(PLAYSOUND_SOUND_HIGHSPEED2_SHUTTER_TONE);
        }
        else
        {
            //GxSound_Stop();
            UISound_Play(PLAYSOUND_SOUND_SHUTTER_TONE);
        }
    }
    #endif
    #if (UI_STYLE==UI_STYLE_DRIVER)
        GxSound_Stop();
        UISound_Play(DEMOSOUND_SOUND_SHUTTER_TONE);
    #endif
    return NVTEVT_CONSUME;
}
INT32 PhotoExe_OnFOV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiSelect = 0;
    UINT32  index;
    ER      result;
    IPL_MODE_INFOR  IplModeInfo;
    IPL_DZOOM       IplDzoomInfo;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_FOV, uiSelect);

    switch (uiSelect)
    {
    case FOV_MEDIUM:
        index = 1;
        break;

    case FOV_NARROW:
        index = 2;
        break;

    case FOV_CROP:
        index = 3;
        break;

    case FOV_ULTRA_WIDE:
    default:
        index = 0;
        break;
    }

    // get IPL mode
    IplModeInfo.Id = IPL_ID_1;
    result = IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&IplModeInfo);

    if ((result == E_OK) && (IplModeInfo.Mode != IPL_MODE_OFF))
    {
        IplDzoomInfo.Id = IPL_ID_1;
        IplDzoomInfo.Idx = index;
        IPL_SetCmd(IPL_SET_DZOOM, (void *)&IplDzoomInfo);
        IPL_WaitCmdFinish();
    }
    return NVTEVT_CONSUME;
}

EVENT_ENTRY CustomPhotoObjCmdMap[] =
{
    {NVTEVT_EXE_OPEN,               PhotoExe_OnOpen                 },
    {NVTEVT_EXE_CLOSE,              PhotoExe_OnClose                },
    {NVTEVT_EXE_MACRO,              PhotoExe_OnMacro                },
    {NVTEVT_EXE_SELFTIMER,          PhotoExe_OnSelftimer            },
    {NVTEVT_EXE_FLASH,              PhotoExe_OnFlash                },
    {NVTEVT_EXE_EV,                 PhotoExe_OnEV                   },
    {NVTEVT_EXE_SIZE,               PhotoExe_OnSize                 },
    {NVTEVT_EXE_QUALITY,            PhotoExe_OnQuality              },
    {NVTEVT_EXE_WB,                 PhotoExe_OnWB                   },
    {NVTEVT_EXE_MWB_ADJUST,         PhotoExe_OnMwbAdjust            },
    {NVTEVT_EXE_MWB_RGB,            PhotoExe_OnMwbRGB               },
    {NVTEVT_EXE_COLOR,              PhotoExe_OnColor                },
    {NVTEVT_EXE_ISO,                PhotoExe_OnISO                  },
    {NVTEVT_EXE_AFWINDOW,           PhotoExe_OnAFWindow             },
    {NVTEVT_EXE_AFBEAM,             PhotoExe_OnAFBeam               },
    {NVTEVT_EXE_CONTAF,             PhotoExe_OnContAF               },
    {NVTEVT_EXE_METERING,           PhotoExe_OnMetering             },
    {NVTEVT_EXE_CAPTURE_MODE,       PhotoExe_OnCaptureMode          },
    {NVTEVT_EXE_DATE_PRINT,         PhotoExe_OnDatePrint            },
    {NVTEVT_EXE_PREVIEW,            PhotoExe_OnPreview              },
    {NVTEVT_EXE_DIGITAL_ZOOM,       PhotoExe_OnDigitalZoom          },
    {NVTEVT_EXE_BSHOOT,             PhotoExe_OnBShoot               },
    {NVTEVT_EXE_BLINKDET,           PhotoExe_OnBlinkDet             },
    {NVTEVT_EXE_SMILEDET,           PhotoExe_OnSmileDet             },
    {NVTEVT_EXE_FD,                 PhotoExe_OnFD                   },
    {NVTEVT_EXE_ASCN,               PhotoExe_OnASCN                 },
    {NVTEVT_EXE_CONTSHOT,           PhotoExe_OnContShot             },
    {NVTEVT_EXE_TIMELAPSE,          PhotoExe_OnTimeLapse            },
    {NVTEVT_EXE_PAN,                PhotoExe_OnPan                  },
    {NVTEVT_EXE_SCENEMODE,          PhotoExe_OnSceneMode            },
    {NVTEVT_EXE_CAPTURE_START,      PhotoExe_OnCaptureStart         },
    {NVTEVT_EXE_CAPTURE_STOP,       PhotoExe_OnCaptureStop          },
    {NVTEVT_EXE_CAPTURE_END,        PhotoExe_OnCaptureEnd           },
    {NVTEVT_EXE_CAPTURE_SMILE,      PhotoExe_OnCaptureSmile         },
    {NVTEVT_EXE_ASPSCS,             PhotoExe_OnASPSCS               },
    {NVTEVT_EXE_PANCAPTURE,         PhotoExe_OnPanCapture           },
    {NVTEVT_EXE_ZOOM,               PhotoExe_OnZoom                 },
    {NVTEVT_EXE_MOVIEDIS,           PhotoExe_OnDis                  },
    {NVTEVT_EXE_MOVIE_MCTF,         PhotoExe_OnMCTF                 },
    {NVTEVT_EXE_MOVIE_RSC,          PhotoExe_OnRSC                  },
    {NVTEVT_EXE_MOVIEGDC,           PhotoExe_OnMovieGdc             },
    {NVTEVT_EXE_GDC,                PhotoExe_OnGdc                  },
    {NVTEVT_EXE_MOVIESMEAR,         PhotoExe_OnSmear                },
    {NVTEVT_EXE_START_FUNC,         PhotoExe_OnStartFunc            },
    {NVTEVT_EXE_STOP_FUNC,          PhotoExe_OnStopFunc             },
    {NVTEVT_EXE_AF_PROCESS,         PhotoExe_OnAFProcess            },
    {NVTEVT_EXE_AF_RELEASE,         PhotoExe_OnAFRelease            },
    {NVTEVT_EXE_AF_WAITEND,         PhotoExe_OnAFWaitEnd            },
    {NVTEVT_EXE_IMAGE_RATIO,        PhotoExe_OnImageRatio           },
    {NVTEVT_EXE_PHOTO_FRAME,        PhotoExe_OnPhotoFrame           },
    {NVTEVT_EXE_ID_POSTPRS,         PhotoExe_OnIDPostProcess        },
    {NVTEVT_EXE_SINGLE_VIEW_OPEN,   PhotoExe_OnSingleViewOpen       },
    {NVTEVT_EXE_SINGLE_VIEW_CLOSE,  PhotoExe_OnSingleViewClose      },
    {NVTEVT_EXE_PRV_POSTPRS_OPEN,   PhotoExe_OnPrvPostProcessOpen   },
    {NVTEVT_EXE_PRV_POSTPRS_CLOSE,  PhotoExe_OnPrvPostProcessClose  },
    {NVTEVT_CALLBACK,               PhotoExe_OnCallback             },
    {NVTEVT_ALGMSG_FDEND,           PhotoExe_OnFDEnd                },
    {NVTEVT_ALGMSG_SDEND,           PhotoExe_OnSDEnd                },
    {NVTEVT_ALGMSG_FOCUSEND,        PhotoExe_OnFocusEnd             },
    {NVTEVT_ALGMSG_ASCEND,          PhotoExe_OnASCEnd               },
    {NVTEVT_NOTIFY_PREPARE_CAP,     PhotoExe_OnNotifyPrepareCap     },
    {NVTEVT_EXE_INIT_DATE_BUF,      PhotoExe_OnInitDateBuf          },
    {NVTEVT_EXE_GEN_DATE_STR,       PhotoExe_OnGenDateStr           },
    {NVTEVT_EXE_GEN_DATE_PIC,       PhotoExe_OnGenDatePic           },
    {NVTEVT_EXE_OBJTRACK,           PhotoExe_OnObjTrack             },
    {NVTEVT_EXE_DSCMODE,            PhotoExe_OnDscMode              },
    {NVTEVT_EXE_SHARPNESS,          PhotoExe_OnSharpness            },
    {NVTEVT_EXE_SATURATION,         PhotoExe_OnSaturation           },
    {NVTEVT_EXE_IMAGE_EFFECT,       PhotoExe_OnImageEffect          },
    {NVTEVT_EXE_3DNR,               PhotoExe_On3DNR                 },
    {NVTEVT_EXE_SUPER_RESOLUTION,   PhotoExe_OnSuperResolution      },
    {NVTEVT_EXE_PLAY_SHUTTER_SOUND, PhotoExe_OnPlayShutterSound     },
    {NVTEVT_EXE_FOV,                PhotoExe_OnFOV                  },
    {NVTEVT_NULL,                   0},
};

CREATE_APP(CustomPhotoObj,APP_PHOTO)

