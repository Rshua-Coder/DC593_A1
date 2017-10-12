#include "UIAppMovie.h"
#include "NVTUserCommand.h"
#include "uimode.h"
#include "ae_task.h"
#include "awb_task.h"
#include "SysCfg.h"
#include "MovieInterface.h"
#include "HWClock.h"
// Audio NR
#include "AudFilterAPI.h"
#include "UIAppMovie_AudNR.h"
#include "Lens.h"
#if MOVIE_FD_FUNC_
#include "fd_lib.h"
#endif

#if _LDWS_FUNC_
#include "ldws_alg.h"
#endif


#define __MODULE__          UIMovieCB
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

#define AUDNR_DELAYFRAME  2

// Lens State
static LENS_CB_MESSAGE gUIMovieLensState = 0;

UINT32 gAudNrIrisDelayFrame = AUDNR_DELAYFRAME;
UINT32 gAudNrAFDelayFrame = AUDNR_DELAYFRAME;
BOOL   gbAudNrZoomOperating = FALSE;


//-------------------------------------------------------------------------------------------------
//
// Callback function for audio filtering & Noise Reduction
//
//-------------------------------------------------------------------------------------------------
void UIMovie_AudFilterProc(void)
{
    // Audio NR
    UINT32              uiNrStopFreq, uiNrReplaceFreq, uiNrDecreaseLvl, uiNrMixBackgrd;
    //INT32               iNrBandNum;
    AUDNR_NOISE_SPEC    *pNoiseSpec = NULL;

    //
    //  Default Config
    //
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_SAMPLE_RATE,            32000);
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_CHANNEL_NO,             2);

    //
    // AudNR Background Suppress Enabled
    //
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_BACKGROUND_ENABLE,       FALSE);
    //AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_BACKGROUND_DECREASE_LVL, AUDNR_COEF(4.0));
    //AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_BACKGROUND_LOWER_BOUND,  AUDNR_COEF(0.1));

    //
    // Aud NR Enabled
    //
    switch (gUIMovieLensState)
    {
        case LENS_CB_ZOOM_START: // Zoom
        {
            //DBG_IND("Lens state is Zoom Moving!\r\n");
            uiNrStopFreq    = gAudNrZoomStopFreq;
            uiNrReplaceFreq = gAudNrZoomReplaceFreq;
            uiNrDecreaseLvl = gAudNrZoomDecreaseLvl;
            uiNrMixBackgrd  = gAudNrZoomMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Zoom;
            //iNrBandNum      = gAudNrZoomBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrZoomBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrZoomNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrZoomNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrZoomNoiseSpec;

            gAudNrAFDelayFrame = gAudNrIrisDelayFrame = AUDNR_DELAYFRAME;
            gbAudNrZoomOperating = TRUE;
            AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                         TRUE);
        }
        break;

        case LENS_CB_FOCUS_START: // Focus
        {
            //DBG_IND("Lens state is Focus Moving!\r\n");
            if (gbAudNrZoomOperating) return; // Zoom operation is enabling, skip AF event
            uiNrStopFreq    = gAudNrFocusStopFreq;
            uiNrReplaceFreq = gAudNrFocusReplaceFreq;
            uiNrDecreaseLvl = gAudNrFocusDecreaseLvl;
            uiNrMixBackgrd  = gAudNrFocusMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Focus;
            //iNrBandNum      = gAudNrFocusBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrFocusBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrFocusNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrFocusNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrFocusNoiseSpec;

            gAudNrAFDelayFrame = 0; // Reset delay frame count
            AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                         TRUE);
        }
        break;

        case LENS_CB_APERTURE_START: // Iris
        {
            // Support CAF case
            if((gAudNrAFDelayFrame < AUDNR_DELAYFRAME) ||(gbAudNrZoomOperating)) // Zoom operation and Auto Focus is enabling, skip AE event
                return;

            //DBG_IND("Lens state is Iris Moving!\r\n");
            uiNrStopFreq    = gAudNrIrisStopFreq;
            uiNrReplaceFreq = gAudNrIrisReplaceFreq;
            uiNrDecreaseLvl = gAudNrIrisDecreaseLvl;
            uiNrMixBackgrd  = gAudNrIrisMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Iris;
            //iNrBandNum      = gAudNrIrisBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrIrisBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrIrisNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrIrisNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrIrisNoiseSpec;

            gAudNrIrisDelayFrame = 0; // Reset delay frame count
            AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                         TRUE);
        }
        break;


        default:
        case LENS_CB_ZOOM_END:
        {
            //DBG_IND("Lens state is ZOOM end!\r\n");
            uiNrStopFreq    = gAudNrZoomStopFreq;
            uiNrReplaceFreq = gAudNrZoomReplaceFreq;
            uiNrDecreaseLvl = gAudNrZoomDecreaseLvl;
            uiNrMixBackgrd  = gAudNrZoomMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Zoom;
            //iNrBandNum      = gAudNrZoomBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrZoomBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrZoomNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrZoomNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrZoomNoiseSpec;

            gbAudNrZoomOperating = FALSE;
            AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                         FALSE);
        }
        break;

        case LENS_CB_FOCUS_END:
        {
            //DBG_IND("Lens state is Idle!\r\n");
            if (gbAudNrZoomOperating) return; // Zoom operation is enabling, skip AF event

            uiNrStopFreq    = gAudNrFocusStopFreq;
            uiNrReplaceFreq = gAudNrFocusReplaceFreq;
            uiNrDecreaseLvl = gAudNrFocusDecreaseLvl;
            uiNrMixBackgrd  = gAudNrFocusMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Focus;
            //iNrBandNum      = gAudNrFocusBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrFocusBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrFocusNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrFocusNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrFocusNoiseSpec;

            if(gAudNrAFDelayFrame >= AUDNR_DELAYFRAME)
            {
                //DBG_IND("IRIS Lens state is Idle!\r\n");
                AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                     FALSE);
            }
            else
            {
                //DBG_IND("IRIS Lens state is not Idle!\r\n");
                gAudNrAFDelayFrame++;
                AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                     TRUE);
            }
        }
        break;

        case LENS_CB_APERTURE_END:
        {
            // Support CAF case
            if((gAudNrAFDelayFrame < AUDNR_DELAYFRAME) ||(gbAudNrZoomOperating)) // Zoom operation and Auto Focus is enabling, skip AE event
                return;

            uiNrStopFreq    = gAudNrIrisStopFreq;
            uiNrReplaceFreq = gAudNrIrisReplaceFreq;
            uiNrDecreaseLvl = gAudNrIrisDecreaseLvl;
            uiNrMixBackgrd  = gAudNrIrisMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Iris;
            //iNrBandNum      = gAudNrIrisBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrIrisBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrIrisNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrIrisNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrIrisNoiseSpec;

            if(gAudNrIrisDelayFrame >= AUDNR_DELAYFRAME)
            {
                //DBG_IND("IRIS Lens state is Idle!\r\n");
                AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                     FALSE);
            }
            else
            {
                //DBG_IND("IRIS Lens state is not Idle!\r\n");
                gAudNrIrisDelayFrame++;
                AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                     TRUE);
            }
        }
        break;
    }

    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_STOP_FREQUENCY,         uiNrStopFreq);
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_REPLACE_FREQUENCY,   uiNrReplaceFreq);
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_MIX_BACKGROUND,       uiNrMixBackgrd);
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_DECREASE_LVL,        uiNrDecreaseLvl);

    // Noise Pattern Spec.
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_SPEC,             (UINT32)pNoiseSpec);

}

//-------------------------------------------------------------------------------------------------
//
// API function for Lens state updating
//
//-------------------------------------------------------------------------------------------------
void UIMovie_UpdateLensState(UINT32 LensState)
{
    gUIMovieLensState = LensState;
}

//-------------------------------------------------------------------------------------------------
//
// Callback function for the events which should be processed immediately (cannot use post event)
//
//-------------------------------------------------------------------------------------------------
void UIMovie_3DNRSetting(void)
{
    UINT32  ui3DNRLevel;

    if (UI_GetData(FL_MovieMCTFIndex) == MOVIE_MCTF_OFF)
    {
        ui3DNRLevel = H264_3DNR_DISABLE;
    }
    else
    {
        ui3DNRLevel = UIMovie_3DNRAdjust();
      }

    MovRec_ChangeParameter(MOVREC_RECPARAM_H2643DNRLEVEL, ui3DNRLevel, 0, 0);
}

void UIMovie_ImmProc(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3)
{
    switch (type)
    {
    case MOVREC_IMMPROC_CUTFILE_END:
        #if (UI_STYLE==UI_STYLE_DRIVER)
        // update DCF Folder/File name
        FlowDCF_UpdateName();
        #endif

        // [TO DO]: log GPS data
        DBG_IND("EVENT %d: Cut file end!\r\n", type);
        break;

    case MOVREC_IMMPROC_DELETE_FILE:
        // [TO DO]: delete related files, such as GPS data file
        DBG_IND("EVENT %d: Delete file!\r\n", type);
        break;

    // special dynamic encoding parameters setting, such as H.264 3DNR
    case MOVREC_IMMPROC_ENC_SETTING:
        UIMovie_3DNRSetting();
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------------------------
//
// Callback function for custom file naming rule
//
//-------------------------------------------------------------------------------------------------
#if (YOUKU_SDK == ENABLE)
#include "NameRule_Custom.h"
static void UIMovie_SetCustomFileName(UINT32 uiPathId, char *pFileName)
{
    static struct tm   CurDateTime = {0};

    if (uiPathId == 0)
    {
        CurDateTime = HwClock_GetCurrentTime();
    }
    snprintf(pFileName,NMC_TOTALFILEPATH_MAX_LEN, "%04d%02d%02d_%02d%02d%02d", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
}
#else
static void UIMovie_SetCustomFileName(UINT32 uiPathId, char *pFileName)
{
//#NT#2016/08/12#Hideo Lin -begin
//#NT#For small size clone movie
#if (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
    static struct tm   CurDateTime = {0};
    static UINT32      uSetPathCnt = 0;
    static UINT32      uiPrevPathId = 0;

    if (((uSetPathCnt % 2) == 0) || (uiPrevPathId == uiPathId))
    {
        CurDateTime = HwClock_GetCurrentTime();

        if (uiPrevPathId == uiPathId)
        {
            uSetPathCnt = 0;
        }
    }

    uSetPathCnt++;
    uiPrevPathId = uiPathId;

    if (uiPathId == 0) // main movie
    {
        sprintf(pFileName, "%04d%02d%02d%02d%02d%02d",
            CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday,
            CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
    }
    else // clone movie
    {
        sprintf(pFileName, "%04d%02d%02d%02d%02d%02d_s",
            CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday,
            CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
    }
#else
    struct tm   CurDateTime;

    CurDateTime = HwClock_GetCurrentTime();

    switch(SysGetFlag(FL_DATE_FORMAT))
    {
    case DATE_FORMAT_DMY:
        if (uiPathId == 0)
            sprintf(pFileName, "FRONT_%02d%02d%04d_%02d%02d%02d", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
        else
            sprintf(pFileName, "REAR_%02d%02d%04d_%02d%02d%02d",  CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
        break;

    case DATE_FORMAT_MDY:
        if (uiPathId == 0)
            sprintf(pFileName, "FRONT_%02d%02d%04d_%02d%02d%02d", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
        else
            sprintf(pFileName, "REAR_%02d%02d%04d_%02d%02d%02d",  CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
        break;

    case DATE_FORMAT_YMD:
    default:
        if (uiPathId == 0)
            sprintf(pFileName, "FRONT_%04d%02d%02d_%02d%02d%02d", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
        else
            sprintf(pFileName, "REAR_%04d%02d%02d_%02d%02d%02d",  CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
        break;
    }
#endif
//#NT#2016/08/12#Hideo Lin -end
}
#endif
void UIMovie_MovieFileNamingCB(UINT32 uiPathId, char *pFileName)
{
    UIMovie_SetCustomFileName(uiPathId, pFileName);
}
void UIMovie_EMRMovFileNamingCB(UINT32 uiPathId, char *pFileName)
{
    UIMovie_SetCustomFileName(uiPathId, pFileName);
}
void UIMovie_PhotoFileNamingCB(UINT32 uiPathId, char *pFileName)
{
    UIMovie_SetCustomFileName(uiPathId, pFileName);
}

// special photo file naming for engineer mode
extern char* EngineerMode_GetWiFiMAC(void);
void UIMovie_PhotoFileNamingCB_EngMode(UINT32 uiPathId, char *pFileName)
{
    static UINT32 uiPhotoNum = 0;

    uiPhotoNum++;
    sprintf(pFileName, "%s_E%d", EngineerMode_GetWiFiMAC(), uiPhotoNum);
}

//-------------------------------------------------------------------------------------------------
//
// Callback function for AF events
//
//-------------------------------------------------------------------------------------------------
void UIMovie_AFCB(AF_CBMSG uiMsgID, void *Data)
{
    UINT32 uiData;

    switch (uiMsgID)
    {
        case AF_CBMSG_AF_START:
            uiData = *(UINT32 *)Data;
            DBG_IND("===AF Start! - %d===\r\n", uiData);
            break;
        case AF_CBMSG_AF_DONE:
            uiData = *(UINT32 *)Data;
            DBG_IND("===AF Done! - %d===\r\n", uiData);
            AE_Lock(AE_ID_1,FALSE,FALSE);
            AWB_Lock(AWB_ID_1,FALSE,FALSE);
            Ux_PostEvent(NVTEVT_ALGMSG_FOCUSEND,1, NVTEVT_ALGMSG_FOCUSEND);
            break;
        default:
            break;
    }
}

#if MOVIE_FD_FUNC_
void Movie_FdCB(FD_CBMSG uiMsgID, void *Data)
{
    switch (uiMsgID)
    {
        case FD_CBMSG_DETEND:
           DBG_IND("===FDEND===\r\n");
           Ux_PostEvent(NVTEVT_EXE_MOVIE_FDEND,1, NVTEVT_EXE_MOVIE_FDEND);
           break;


        default:
           break;
    }
}
#endif
void Movie_RegCB(void)
{
    #if MOVIE_FD_FUNC_
    FD_RegisterCB(Movie_FdCB);
    #endif

}
//-------------------------------------------------------------------------------------------------
//
// Callback function for LDWS events
//
//-------------------------------------------------------------------------------------------------
#if _LDWS_FUNC_

#include "GxSystem.h"
#include "GxFlash.h"
#include "GxDC.h"
#include "Uiresource.h"
#include "uiDisplay.h"
#include "UIFlow.h"

#define LDWS_ROI_WIDTH 720
#define LDWS_ROI_HEIGHT 480
#if 0
#if ((OSD_RATIO == OSD_RATIO_4_3) && (DISPLAY_169HD == DISABLE))
#define LDWS_OSD_ROIX(ROIX, ROI_SX, InWidth, InHeight) ((ROIX+ROI_SX)/(InWidth/OSD_W))
#define LDWS_OSD_ROIY(ROIY, ROI_SY, InWidth, InHeight) ((ROIY+ROI_SY)/(InWidth/OSD_W) + 30)

#define LDWS_OSD_VPX(VPx,ROI_SX, InWidth, InHeight) (VPx/(InWidth/OSD_W))
#define LDWS_OSD_VPY(VPy,ROI_SY, InWidth, InHeight) (VPy/(InWidth/OSD_W) + 30)

#define LDWS_OSD_LANEX(VideoX, ROI_SX, InWidth, InHeight) ((VideoX+ROI_SX)/(InWidth/OSD_W))
#define LDWS_OSD_LANEY(VideoY, ROI_SY, InWidth, InHeight) ((VideoY+ROI_SY)/(InWidth/OSD_W) + 30)
#elif ((OSD_RATIO == OSD_RATIO_4_3) && (DISPLAY_169HD == ENABLE))
#define LDWS_OSD_ROIX(ROIX, ROI_SX, InWidth, InHeight) ((ROIX+ROI_SX)*OSD_W/InWidth)
#define LDWS_OSD_ROIY(ROIY, ROI_SY, InWidth, InHeight) ((ROIY+ROI_SY)*OSD_H/InHeight)

#define LDWS_OSD_VPX(VPx,ROI_SX, InWidth, InHeight) (VPx*OSD_W/InWidth)
#define LDWS_OSD_VPY(VPy,ROI_SY, InWidth, InHeight) (VPy*OSD_H/InHeight)

#define LDWS_OSD_LANEX(VideoX, ROI_SX, InWidth, InHeight) ((VideoX+ROI_SX)*OSD_W/InWidth)
#define LDWS_OSD_LANEY(VideoY, ROI_SY, InWidth, InHeight) ((VideoY+ROI_SY)*OSD_H/InHeight)
#elif ((OSD_RATIO == OSD_RATIO_16_9) && (DISPLAY_169HD == ENABLE))
#define LDWS_OSD_ROIX(ROIX, ROI_SX, InWidth, InHeight) ((ROIX+ROI_SX)/(InWidth/OSD_W))
#define LDWS_OSD_ROIY(ROIY, ROI_SY, InWidth, InHeight) ((ROIY+ROI_SY)/(InHeight/OSD_H))

#define LDWS_OSD_VPX(VPx,ROI_SX, InWidth, InHeight) (VPx/(InWidth/OSD_W))
#define LDWS_OSD_VPY(VPy,ROI_SY, InWidth, InHeight) (VPy/(InHeight/OSD_H))

#define LDWS_OSD_LANEX(VideoX, ROI_SX, InWidth, InHeight) ((VideoX+ROI_SX)/(InWidth/OSD_W))
#define LDWS_OSD_LANEY(VideoY, ROI_SY, InWidth, InHeight) ((VideoY+ROI_SY)/(InHeight/OSD_H))
#else
#error (No define OSD ratio)
#endif
#endif

UINT32 MovieLine[6][20][5];
//UINT32 MovieLineIdx = 0;
LDWS_RESULT_INFO g_LdwsInfo = {0};

void UIMovie_LDWSCB(LDWS_CBMSG uiMsgID, void *Data)
{
    //DC**     pDCList;
    UINT32 Video_VPx1=0, Video_VPy1=0, Video_VPx2=0, Video_VPy2=0;
    UINT32 Video_VPx1P=0, Video_VPy1P=0, Video_VPx2P=0, Video_VPy2P=0;
    UINT32 Video_VPx1D=0, Video_VPy1D=0, Video_VPx2D=0, Video_VPy2D=0;
    UINT32 Video_VPx1Left=0, Video_VPy1Left=0, Video_VPx2Left=0, Video_VPy2Left=0;
    UINT32 Video_VPx1Right=0, Video_VPy1Right=0, Video_VPx2Right=0, Video_VPy2Right=0;
    UINT32 Video_RoiXYArr[4*2]={0}; // [X0, X1, X2, X3, Y0, Y1, Y2, Y3]
    UINT32 iML;
    INT32  MovieLineCnt = 0;
    INT32  iMovieLineIdx = 0;

//    LDWS_OSD_COORDINATE LdwsOsdCoord = {0};
    UINT32 VPx, VPy;

    /*  Video ROI_XYArr[4*2]
     -------------------------------
     -                             -
     -  (x0,y0)           (x1,y1)  -
     -    -------------------      -
     -    -                 -      -
     -    -       ROI       -      -
     -    -                 -      -
     -    -------------------      -
     -  (x3,y3))          (x2,y2)  -
     -------------------------------
    */

    /*  OSD ROI_XYArr[4*2]
     -------------------------------
     -                             -
     -  (x0,y0)           (x3,y3)  -
     -    -------------------      -
     -    -                 -      -
     -    -       ROI       -      -
     -    -                 -      -
     -    -------------------      -
     -  (x1,y1))          (x4,y4)  -
     -------------------------------
    */

    switch (uiMsgID)
    {
        case LDWS_CBMSG_WARNNING:
            if(System_GetState(SYS_STATE_CURRSUBMODE)!=SYS_SUBMODE_WIFI)
            {
                if (FlowMovie_GetMovDataState() !=  MOV_ST_REC)
                {
                    break;
                }
            }

            g_LdwsInfo = *(LDWS_RESULT_INFO*)Data;
            /*--- draw on OSD ---*/
#if 1
            //draw ROI
            Video_RoiXYArr[0] = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx; // X1
            Video_RoiXYArr[4] = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy; // Y1
            Video_RoiXYArr[1] = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx + g_LdwsInfo.LdSrcImgInfo.uiLdRoiWidth; // X2
            Video_RoiXYArr[5] = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy; // Y2
            Video_RoiXYArr[2] = Video_RoiXYArr[1]; // X3
            Video_RoiXYArr[6] = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy + g_LdwsInfo.LdSrcImgInfo.uiLdRoiHeight; // Y3
            Video_RoiXYArr[3] = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx; //X4
            Video_RoiXYArr[7] = Video_RoiXYArr[6]; // Y4
#if 0
            LdwsOsdCoord.ROIx1 = LDWS_OSD_ROIX(Video_RoiXYArr[0]-g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.ROIy1 = LDWS_OSD_ROIY(Video_RoiXYArr[4]-g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.ROIx2 = LDWS_OSD_ROIX(Video_RoiXYArr[3]-g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.ROIy2 = LDWS_OSD_ROIY(Video_RoiXYArr[7]-g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.ROIx3 = LDWS_OSD_ROIX(Video_RoiXYArr[1]-g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.ROIy3 = LDWS_OSD_ROIY(Video_RoiXYArr[5]-g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.ROIx4 = LDWS_OSD_ROIX(Video_RoiXYArr[2]-g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.ROIy4 = LDWS_OSD_ROIY(Video_RoiXYArr[6]-g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
#endif

#if 0
            //vanishing point
            #if 0
            Video_VPx1 = (g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth - 48)/2;
            Video_VPy1 = g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight/2;
            Video_VPx2 = (g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth + 48)/2;
            Video_VPy2 = g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight/2;
            #else
            VPx = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx + LDWS_ROI_WIDTH/2;
            VPy = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy + LDWS_ROI_HEIGHT/2;
            //Video_VPx1 = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx + LDWS_ROI_WIDTH/2 - 48;
            //Video_VPy1 = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy + LDWS_ROI_HEIGHT/2;
            //Video_VPx2 = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx + LDWS_ROI_WIDTH/2 + 48;
            //Video_VPy2 = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy + LDWS_ROI_HEIGHT/2;
            Video_VPx1 = 0;
            Video_VPy1 = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy + LDWS_ROI_HEIGHT/2;
            Video_VPx2 = g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth;
            Video_VPy2 = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy + LDWS_ROI_HEIGHT/2;
            #endif
            LdwsOsdCoord.VPx1 = LDWS_OSD_VPX(Video_VPx1, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPy1 = LDWS_OSD_VPY(Video_VPy1, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPx2 = LDWS_OSD_VPX(Video_VPx2, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPy2 = LDWS_OSD_VPY(Video_VPy2, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
#else
            VPx = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx + LDWS_ROI_WIDTH/2;
            VPy = g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy + LDWS_ROI_HEIGHT/2;
            // VP'
            Video_VPx1P = g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth/2 - 50;
            Video_VPy1P = g_LdwsInfo.uiVPYNew+ g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy;
            Video_VPx2P = g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth/2 + 50;
            Video_VPy2P = g_LdwsInfo.uiVPYNew + g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy;


            #if 0
            LdwsOsdCoord.VPNewx1 = LDWS_OSD_VPX(Video_VPx1P, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPNewy1 = LDWS_OSD_VPY(Video_VPy1P, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPNewx2 = LDWS_OSD_VPX(Video_VPx2P, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPNewy2 = LDWS_OSD_VPY(Video_VPy2P, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            #endif

            Video_VPx1D = 0;
            Video_VPy1D = g_LdwsInfo.uiVPYDef+ g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy;
            Video_VPx2D = g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth;
            Video_VPy2D = g_LdwsInfo.uiVPYDef + g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy;

            #if 0
            LdwsOsdCoord.VPDefx1 = LDWS_OSD_VPX(Video_VPx1D, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPDefy1 = LDWS_OSD_VPY(Video_VPy1D, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPDefx2 = LDWS_OSD_VPX(Video_VPx2D, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPDefy2 = LDWS_OSD_VPY(Video_VPy2D, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            #endif
#endif
            // VP left
            Video_VPx1Left = VPx;
            Video_VPy1Left = VPy;
            Video_VPx2Left = VPx - 200;
            Video_VPy2Left = 1080;
            #if 0
            LdwsOsdCoord.VPx1Left = LDWS_OSD_VPX(Video_VPx1Left, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPy1Left = LDWS_OSD_VPY(Video_VPy1Left, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPx2Left = LDWS_OSD_VPX(Video_VPx2Left, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPy2Left = LDWS_OSD_VPY(Video_VPy2Left, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            #endif

            Video_VPx1Right = VPx;
            Video_VPy1Right = VPy;
            Video_VPx2Right = VPx + 200;
            Video_VPy2Right = 1080;
            #if 0
            LdwsOsdCoord.VPx1Right = LDWS_OSD_VPX(Video_VPx1Right, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPy1Right = LDWS_OSD_VPY(Video_VPy1Right, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPx2Right = LDWS_OSD_VPX(Video_VPx2Right, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.VPy2Right = LDWS_OSD_VPY(Video_VPy2Right, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            #endif

            // VP right

            // lane
            #if 0
            LdwsOsdCoord.LaneLx1 = LDWS_OSD_LANEX(g_LdwsInfo.iLx1, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.LaneLy1 = LDWS_OSD_LANEY(g_LdwsInfo.iLy1, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.LaneLx2 = LDWS_OSD_LANEX(g_LdwsInfo.iLx2, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.LaneLy2 = LDWS_OSD_LANEY(g_LdwsInfo.iLy2, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.LaneRx1 = LDWS_OSD_LANEX(g_LdwsInfo.iRx1, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.LaneRy1 = LDWS_OSD_LANEY(g_LdwsInfo.iRy1, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.LaneRx2 = LDWS_OSD_LANEX(g_LdwsInfo.iRx2, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            LdwsOsdCoord.LaneRy2 = LDWS_OSD_LANEY(g_LdwsInfo.iRy2, g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy, g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth, g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight);
            FlowMovie_LwdsProcess(LdwsOsdCoord, &g_LdwsInfo);
            #endif

#endif

#if 1
            // draw on video
            //=================================================================
            iML = 0;//MovieLineIdx % 6;
            MovieLineCnt = 0;
            memset(MovieLine[iML], 0, sizeof(UINT32)*10*5);

            //draw ROI
            MovieLine[iML][MovieLineCnt][0] = Video_RoiXYArr[0];
            MovieLine[iML][MovieLineCnt][1] = Video_RoiXYArr[4];
            MovieLine[iML][MovieLineCnt][2] = Video_RoiXYArr[1];
            MovieLine[iML][MovieLineCnt][3] = Video_RoiXYArr[5];
            if (g_LdwsInfo.Failure == LDWS_FAILURE_TRUE)
            {
                MovieLine[iML][MovieLineCnt][4] = 1; // red
            }else{
                MovieLine[iML][MovieLineCnt][4] = 3; // green
            }
            MovieLineCnt++;

            MovieLine[iML][MovieLineCnt][0] = Video_RoiXYArr[1];
            MovieLine[iML][MovieLineCnt][1] = Video_RoiXYArr[5];
            MovieLine[iML][MovieLineCnt][2] = Video_RoiXYArr[2];
            MovieLine[iML][MovieLineCnt][3] = Video_RoiXYArr[6];
            if (g_LdwsInfo.Failure == LDWS_FAILURE_TRUE)
            {
                MovieLine[iML][MovieLineCnt][4] = 1;
            }else{
                MovieLine[iML][MovieLineCnt][4] = 3;
            }
            MovieLineCnt++;

            MovieLine[iML][MovieLineCnt][0] = Video_RoiXYArr[3];
            MovieLine[iML][MovieLineCnt][1] = Video_RoiXYArr[7];
            MovieLine[iML][MovieLineCnt][2] = Video_RoiXYArr[2];
            MovieLine[iML][MovieLineCnt][3] = Video_RoiXYArr[6];
            if (g_LdwsInfo.Failure == LDWS_FAILURE_TRUE)
            {
                MovieLine[iML][MovieLineCnt][4] = 1;
            }else{
                MovieLine[iML][MovieLineCnt][4] = 3;
            }
            MovieLineCnt++;

            MovieLine[iML][MovieLineCnt][0] = Video_RoiXYArr[0];
            MovieLine[iML][MovieLineCnt][1] = Video_RoiXYArr[4];
            MovieLine[iML][MovieLineCnt][2] = Video_RoiXYArr[3];
            MovieLine[iML][MovieLineCnt][3] = Video_RoiXYArr[7];
            if (g_LdwsInfo.Failure == LDWS_FAILURE_TRUE)
            {
                MovieLine[iML][MovieLineCnt][4] = 1;
            }else{
                MovieLine[iML][MovieLineCnt][4] = 3;
            }
            MovieLineCnt++;
#if 0

            // draw default vanishing point
            MovieLine[iML][MovieLineCnt][0] = Video_VPx1D;
            MovieLine[iML][MovieLineCnt][1] = Video_VPy1D;
            MovieLine[iML][MovieLineCnt][2] = Video_VPx2D;
            MovieLine[iML][MovieLineCnt][3] = Video_VPy2D;
            MovieLine[iML][MovieLineCnt][4] = 3;
            MovieLineCnt++;
#endif

            //DBG_DUMP("%d %d %d %d\r\n",Video_VPx1D, Video_VPy1D, Video_VPx2D, Video_VPy2D);
            // draw tolerance
            MovieLine[iML][MovieLineCnt][0] = Video_VPx1D;
            MovieLine[iML][MovieLineCnt][1] = Video_VPy1D - 50;
            MovieLine[iML][MovieLineCnt][2] = Video_VPx2D;
            MovieLine[iML][MovieLineCnt][3] = Video_VPy2D - 50;
            MovieLine[iML][MovieLineCnt][4] = 3;
            MovieLineCnt++;

            MovieLine[iML][MovieLineCnt][0] = Video_VPx1D;
            MovieLine[iML][MovieLineCnt][1] = Video_VPy1D + 50;
            MovieLine[iML][MovieLineCnt][2] = Video_VPx2D;
            MovieLine[iML][MovieLineCnt][3] = Video_VPy2D + 50;
            MovieLine[iML][MovieLineCnt][4] = 3;
            MovieLineCnt++;


            // draw new vanishing point
            MovieLine[iML][MovieLineCnt][0] = Video_VPx1P;
            MovieLine[iML][MovieLineCnt][1] = Video_VPy1P;
            MovieLine[iML][MovieLineCnt][2] = Video_VPx2P;
            MovieLine[iML][MovieLineCnt][3] = Video_VPy2P;
            if (g_LdwsInfo.uiEnerUpdateMode)
                MovieLine[iML][MovieLineCnt][4] = 1;
            else
                MovieLine[iML][MovieLineCnt][4] = 3;
            MovieLineCnt++;


            // draw vanishing point
            MovieLine[iML][MovieLineCnt][0] = Video_VPx1;
            MovieLine[iML][MovieLineCnt][1] = Video_VPy1;
            MovieLine[iML][MovieLineCnt][2] = Video_VPx2;
            MovieLine[iML][MovieLineCnt][3] = Video_VPy2;
            MovieLine[iML][MovieLineCnt][4] = 3;
            MovieLineCnt++;
#if 0
            // draw vanishing left & right line
            MovieLine[iML][MovieLineCnt][0] = Video_VPx1Left;
            MovieLine[iML][MovieLineCnt][1] = Video_VPy1Left;
            MovieLine[iML][MovieLineCnt][2] = Video_VPx2Left;
            MovieLine[iML][MovieLineCnt][3] = Video_VPy2Left;
            MovieLine[iML][MovieLineCnt][4] = 3;
            MovieLineCnt++;

            MovieLine[iML][MovieLineCnt][0] = Video_VPx1Right;
            MovieLine[iML][MovieLineCnt][1] = Video_VPy1Right;
            MovieLine[iML][MovieLineCnt][2] = Video_VPx2Right;
            MovieLine[iML][MovieLineCnt][3] = Video_VPy2Right;
            MovieLine[iML][MovieLineCnt][4] = 3;
            MovieLineCnt++;
#endif
            // draw lane
            MovieLine[iML][MovieLineCnt][0] = (UINT32)(g_LdwsInfo.iLx1 + g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx);
            MovieLine[iML][MovieLineCnt][1] = (UINT32)(g_LdwsInfo.iLy1 + g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy);
            MovieLine[iML][MovieLineCnt][2] = (UINT32)(g_LdwsInfo.iLx2 + g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx);
            MovieLine[iML][MovieLineCnt][3] = (UINT32)(g_LdwsInfo.iLy2 + g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy);
            if (g_LdwsInfo.DepartureDir == LDWS_DEPARTURE_LEFT)
            {
                MovieLine[iML][MovieLineCnt][4] = 1; // red
            }else{
                MovieLine[iML][MovieLineCnt][4] = 2; // green
            }

            //DBG_DUMP("Left lane info %d %d %d %d\r\n",MovieLine[iML][MovieLineCnt][0],MovieLine[iML][MovieLineCnt][1],MovieLine[iML][MovieLineCnt][2],MovieLine[iML][MovieLineCnt][3]);
            MovieLineCnt++;

            MovieLine[iML][MovieLineCnt][0] = (UINT32)(g_LdwsInfo.iRx1 + g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx);
            MovieLine[iML][MovieLineCnt][1] = (UINT32)(g_LdwsInfo.iRy1 + g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy);
            MovieLine[iML][MovieLineCnt][2] = (UINT32)(g_LdwsInfo.iRx2 + g_LdwsInfo.LdSrcImgInfo.uiLdRoiSx);
            MovieLine[iML][MovieLineCnt][3] = (UINT32)(g_LdwsInfo.iRy2 + g_LdwsInfo.LdSrcImgInfo.uiLdRoiSy);
            if (g_LdwsInfo.DepartureDir == LDWS_DEPARTURE_RIGHT)
            {
                MovieLine[iML][MovieLineCnt][4] = 1; // red
            }else{
                MovieLine[iML][MovieLineCnt][4] = 2; // green
            }
            //DBG_DUMP("right lane info %d %d %d %d\r\n",MovieLine[iML][MovieLineCnt][0],MovieLine[iML][MovieLineCnt][1],MovieLine[iML][MovieLineCnt][2],MovieLine[iML][MovieLineCnt][3]);

            MovieLineCnt++;

            //=================================================================
#endif

            if(System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI && UIFlowWndWiFiMovie_GetStatus() == WIFI_MOV_ST_RECORD)
            {
                for(iMovieLineIdx = 0; iMovieLineIdx < MovieLineCnt; iMovieLineIdx++)
                {
                    // for path 2 - Live View stream.
                    // draw all lines. Coordinates must be 4-alignment.
                    MovieLine[1][iMovieLineIdx][0] = (MovieLine[0][iMovieLineIdx][0] * GetMovieSizeWidth_2p(1, 0)/g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth) & ~0x03;
                    MovieLine[1][iMovieLineIdx][1] = (MovieLine[0][iMovieLineIdx][1] * GetMovieSizeHeight_2p(1, 0)/g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight) & ~0x03;
                    MovieLine[1][iMovieLineIdx][2] = (MovieLine[0][iMovieLineIdx][2] * GetMovieSizeWidth_2p(1, 0)/g_LdwsInfo.LdSrcImgInfo.uiLdSrcWidth) & ~0x03;
                    MovieLine[1][iMovieLineIdx][3] = (MovieLine[0][iMovieLineIdx][3] * GetMovieSizeHeight_2p(1, 0)/g_LdwsInfo.LdSrcImgInfo.uiLdSrcHeight) & ~0x03;
                    MovieLine[1][iMovieLineIdx][4] = MovieLine[0][iMovieLineIdx][4];
                }
            }

            break;
        default:
            break;
    }



}
#endif
