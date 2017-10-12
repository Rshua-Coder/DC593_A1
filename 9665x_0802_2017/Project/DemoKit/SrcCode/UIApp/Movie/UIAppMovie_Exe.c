
#include "UIAppMovie.h"
#include "UIAppPhoto.h"
#include "NVTUserCommand.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "SysCfg.h" //for memory POOL_ID
#include "UIAppPlay.h"
#include "GxDisplay.h"
#include "IPL_AlgInfor.h"
#include "IPL_Display.h"
#include "IPL_Display2.h"
#include "IPL_Cmd.h"
#include "ImgCaptureAPI.h"
#include "ImgCapInfor.h"
#include "PhotoTask.h"
#include "Sensor.h"
#include "DxSensor.h"
#include "NvtSystem.h"
#include "MovieInterface.h"
#include "MovieStamp.h"
#include "UIMode.h"
#include "caf_sample.h"
#include "RawEncApi.h"
#include "PrjCfg.h"
#include "IQS_Utility.h"
#include "SysMain.h"
#include "GSensor.h"
#include "UIAppHttpLview.h"
//#NT#2013/05/15#Calvin Chang#Add customer's user data to MOV/MP4 file format -begin
#include "MovieUdtaVendor.h"
#include "ObjTracking_alg.h"
#include "UIControlWnd.h"
#include "UIFlowWndWiFiMovie.h"
//#NT#2013/05/15#Calvin Chang -end
#include "PipView.h"
#if(WIFI_AP_FUNC==ENABLE)
#include "WifiAppCmd.h"
#endif
#include "IPL_Display.h"
#include "UICommon.h"
#include "EngineerMode.h"
#if _LDWS_FUNC_
#include "ldws_alg.h"

extern UINT32      gLDWScount ;//for test
extern PHOTO_FUNC_INFO PhotoFuncInfo_dummy;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ldws;
#endif

#if (GSENSOR_FUNCTION == ENABLE)
extern int SX_TIMER_DET_GSENSOR_ID;
#endif

//#NT#2012/07/31#Hideo Lin -begin
//#NT#Test codes
#define MOVIE_TEST_ENABLE       DISABLE
#define MOVIE_YUV_SIZE_MAX      (1920*1080*2)   // maximum YUV raw data size
#define MOVIE_YUV_NUM           8   // 8 source images for fixed YUV mode

//#NT#2012/10/30#Calvin Chang#Generate Audio NR pattern by noises of zoom, focus and iris -begin
#define MOVIE_AUDIO_NR_PATTERN_ENABLE    DISABLE
//#NT#2012/10/30#Calvin Chang -end

#if (MOVIE_TEST_ENABLE == ENABLE)
static MEDIAREC_TESTMODE g_MediaRecTestMode = {0};
extern void MediaRec_SetTestMode(MEDIAREC_TESTMODE *pTestMode);
#endif
//#NT#2012/07/31#Hideo Lin -end


#define __MODULE__          UIMovieExe
//#define __DBGLVL__ 0        //OFF mode, show nothing
//#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

#define MEDIAREC_VER	MEDIAREC_VER_2_0
//#define MEDIAREC_VER	MEDIAREC_VER_3_0

//#NT#2010/05/21#Photon Lin -begin
#define THUMB_FRAME_SIZE 0x32000
//#NT#2010/05/21#Photon Lin -end
#define MINUTE_240 14400  // 4 hours
#define MINUTE_60  3600   //1 // 1 hour
#define MINUTE_29  1740   //29 minutes

#if MOVIE_FD_FUNC_
static USIZE IMAGERATIO_SIZE[IMAGERATIO_MAX_CNT]=
{
    {9,16},//IMAGERATIO_9_16
    {2,3}, //IMAGERATIO_2_3
    {3,4}, //IMAGERATIO_3_4
    {1,1}, //IMAGERATIO_1_1
    {4,3}, //IMAGERATIO_4_3
    {3,2}, //IMAGERATIO_3_2
    {16,9},//IMAGERATIO_16_9
};


URECT   gMovieFdDispCord;
static UINT32 GetMovieSizeRatio(void);
static void MovieExe_GetFDDispCord(URECT *dispCord);
#endif


static IPL_IME_INFOR            g_ImeInfo = {0};
static IPL_SET_IMGRATIO_DATA    g_ImgRatio = {0};

#if (UI_STYLE==UI_STYLE_DRIVER)
static BOOL    bSDIsSlow    = FALSE;
#endif
static volatile BOOL   g_bMovieRecYUVMergeEnable = 0;  // Isiah, implement YUV merge mode of recording func.
static volatile UINT32 g_uiMovieRecYUVMergeInterval = 1, g_uiMovieRecYUVMergeCounter = 0;
#if (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
static BOOL g_bSensor2En = FALSE;
#endif

//callback
extern void UIMovie_RecCB(UINT32 uiEventID, UINT32 uiSeconds);

extern void aaa_AEprocess(void);
extern void aaa_AWBprocess(void);
extern BOOL Sensor_CheckExtSensor(void);
ER MovieExe_SetGetReadyCB2_Sample(void);//2013/09/23 Meg


//#NT#2010/09/28#Jeah Yen -begin
ISIZE Movie_GetBufferSize(void) //return USER CUSTOM buffer size
{
    return GxVideo_GetDeviceSize(DOUT1);
}
//#NT#2010/09/28#Jeah Yen -end

//#NT#2012/07/31#Hideo Lin -begin
//-------------------------------------------------------------------------------------------------
// Set (load) YUV test pattern from JPEG files
#if (MOVIE_TEST_ENABLE == ENABLE)
#define YUV_RAW_PATH_FHD    "A:\\YUV_RAW\\FHD\\CLOCK\\"
#define YUV_RAW_PATH_HD     "A:\\YUV_RAW\\HD\\CLOCK\\"
#define YUV_RAW_PATH_VGA    "A:\\YUV_RAW\\VGA\\CLOCK\\"
#define YUV_RAW_PATH_QVGA   "A:\\YUV_RAW\\QVGA\\CLOCK\\"

static char sDirPath[40];
static char sFileName[40];

static void Movie_SetYUV(UINT32 uiMovieSize)
{
    UINT32      i;
    UINT32      uiPoolAddr, uiRawAddr, uiFileSize, uiHeight;
    UINT32      uiYLineOffset, uiUVLineOffset;
    char        *pDirPath;
    ER          err = E_OK;
    FST_FILE    pFile;

    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_APP);
    uiRawAddr = uiPoolAddr;

    // ----------------------------------- <--- APP buffer starting address
    // | YUV buffer 1 for media recorder |
    // -----------------------------------
    // | YUV buffer 2 for media recorder |
    // -----------------------------------
    // | YUV buffer 3 for media recorder |
    // -----------------------------------
    // | ...                             |
    // ----------------------------------- <--- media recorder starting address
    //
    // Note: Be sure the buffer is enough for storing the necessary YUV patterns!

    //pDirPath = UserMedia_GetRawDirPath();

    switch (uiMovieSize)
    {
    case MOVIE_SIZE_1080P_FULLRES: // 1080p
    case MOVIE_SIZE_1080P: // 1080p
        uiYLineOffset  = 1920;
        uiUVLineOffset = 1920; // UV pack
        uiHeight = 1088;
        //if (*pDirPath == 0) // never set directory path, use default
        {
            sprintf(sDirPath, YUV_RAW_PATH_FHD, sizeof(YUV_RAW_PATH_FHD));
            pDirPath = sDirPath;
        }
        break;

    case MOVIE_SIZE_VGA:
    //case MOVIE_SIZE_WVGA: // no WVGA pattern, use VGA instead
        uiYLineOffset  = 640;
        uiUVLineOffset = 640; // UV pack
        uiHeight = 480;
        //if (*pDirPath == 0) // never set directory path, use default
        {
            sprintf(sDirPath, YUV_RAW_PATH_VGA, sizeof(YUV_RAW_PATH_VGA));
            pDirPath = sDirPath;
        }
        break;

    case MOVIE_SIZE_QVGA:
        uiYLineOffset  = 320;
        uiUVLineOffset = 320; // UV pack
        uiHeight = 240;
        //if (*pDirPath == 0) // never set directory path, use default
        {
            sprintf(sDirPath, YUV_RAW_PATH_QVGA, sizeof(YUV_RAW_PATH_QVGA));
            pDirPath = sDirPath;
        }
        break;

    case MOVIE_SIZE_720P:
    default:
        uiYLineOffset  = 1280;
        uiUVLineOffset = 1280; // UV pack
        uiHeight = 720;
        //if (*pDirPath == 0) // never set directory path, use default
        {
            sprintf(sDirPath, YUV_RAW_PATH_HD, sizeof(YUV_RAW_PATH_HD));
            pDirPath = sDirPath;
        }
        break;
    }

    g_MediaRecTestMode.YUVInfo.uiTotalNum = MOVIE_YUV_NUM;

    for (i = 0; i < g_MediaRecTestMode.YUVInfo.uiTotalNum; i++)
    {
        // 1. Read Y data from file
        sprintf(sFileName, "%sY%02d.RAW", pDirPath, i+1);
        uiFileSize = MOVIE_YUV_SIZE_MAX;
        pFile = FileSys_OpenFile(sFileName, FST_OPEN_READ);
        if (pFile)
        {
            FileSys_ReadFile(pFile, (UINT8 *)uiRawAddr, &uiFileSize, 0, NULL);
        }
        FileSys_CloseFile(pFile);
        DBG_IND("File %s, size %d\r\n", sFileName, uiFileSize);

        if (err != E_OK)
        {
            DBG_ERR("Open %s for reading failed!\r\n", sFileName);
            return;
        }
        else if (uiFileSize == MOVIE_YUV_SIZE_MAX)
        {
            DBG_ERR("Read buffer is not enough!\r\n");
            return;
        }

        // Set Y source address to media recorder
        g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiYAddr = uiRawAddr;
        //uiRawAddr += uiFileSize;
        uiRawAddr += (uiYLineOffset * uiHeight);

        // 2. Read UV data from file
        sprintf(sFileName, "%sUV%02d.RAW", pDirPath, i+1);
        uiFileSize = MOVIE_YUV_SIZE_MAX;
        pFile = FileSys_OpenFile(sFileName, FST_OPEN_READ);
        if (pFile)
        {
            FileSys_ReadFile(pFile, (UINT8 *)uiRawAddr, &uiFileSize, 0, NULL);
        }
        FileSys_CloseFile(pFile);
        DBG_IND("File %s, size %d\r\n", sFileName, uiFileSize);

        if (err != E_OK)
        {
            DBG_ERR("Open %s for reading failed!\r\n", sFileName);
            return;
        }
        else if (uiFileSize == MOVIE_YUV_SIZE_MAX)
        {
            DBG_ERR("Read buffer is not enough!\r\n");
            return;
        }

        // Set UV source address to media recorder
        g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiCbAddr = uiRawAddr;
        g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiCrAddr = uiRawAddr;
        //uiRawAddr += uiFileSize;
        uiRawAddr += (uiUVLineOffset * uiHeight) / 2;

        // Set Y line offset
        g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiYLineOffset  = uiYLineOffset;
        // Set UV line offset
        g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiUVLineOffset = uiUVLineOffset;

        DBG_MSG("g_UserMediaYUVInfo.YUVSrc[%d].uiYAddr        0x%x\r\n", i, g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiYAddr);
        DBG_MSG("g_UserMediaYUVInfo.YUVSrc[%d].uiCbAddr       0x%x\r\n", i, g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiCbAddr);
        DBG_MSG("g_UserMediaYUVInfo.YUVSrc[%d].uiCrAddr       0x%x\r\n", i, g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiCrAddr);
        DBG_MSG("g_UserMediaYUVInfo.YUVSrc[%d].uiYLineOffset  %d\r\n",   i, g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiYLineOffset);
        DBG_MSG("g_UserMediaYUVInfo.YUVSrc[%d].uiUVLineOffset %d\r\n",   i, g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiUVLineOffset);
    }
}
#endif

#if (UI_STYLE==UI_STYLE_DRIVER)
void Movie_SetSDSlow(BOOL IsSlow)
{
    bSDIsSlow = IsSlow;
}
#endif

#if (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
void Movie_SetDualRec(BOOL bOn)
{
    if (bOn)
    {
        UI_SetData(FL_MOVIE_DUAL_REC, TRUE);
    }
    else
    {
        UI_SetData(FL_MOVIE_DUAL_REC, FALSE);
        DscMovie_Config(MOVIE_CFG_IMGPATH, MOVIE_DUAL_SENSOR_2NDOFF);
    }
}
#endif

void Movie_SetRecParam(void)
{
    UINT32  uiRecFormat, uiFileType;
    UINT32  uiWidth, uiHeight, uiMovieSize, uiQuality;
    UINT32  uiVidCodec, uiVidFrameRate, uiTargetBitrate, uiH264GopType, ui3DNRLevel, uiVidRotate, uiDispAspectRatio;
    UINT32  uiAudCodec, uiAudSampleRate, uiAudChannelNum, uiAudChannelType;
    BOOL    bAudFilterEn;
    UINT32  uiWidth2, uiHeight2, uiTargetBitrate2, uiDispAspectRatio2, uiVidFrameRate2;

#if (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
    uiMovieSize         = UI_GetData(FL_MOVIE_SIZE);            // movie size
#else
    uiMovieSize         = UI_GetData(FL_MOVIE_DUAL_REC) ?
                          UI_GetData(FL_MOVIE_SIZE_DUAL) :
                          UI_GetData(FL_MOVIE_SIZE);            // movie size
#endif

    uiQuality           = UI_GetData(FL_MOVIE_QUALITY);         // movie quality
    // Isiah, implement YUV merge mode of recording func.
    if (!FlowMovie_RecGetYUVMergeMode())
    {
        if (SysGetFlag(FL_MOVIE_TIMELAPSE_REC)==MOVIE_TIMELAPSEREC_OFF)
            uiRecFormat     = MEDIAREC_AUD_VID_BOTH;            // recording type
        else
            uiRecFormat     = MEDIAREC_TIMELAPSE;               // recording type
    }
    else // Change to YUV Merge mode.
    {
        uiRecFormat         = MEDIAREC_MERGEYUV;
    }

    #if (MEDIAREC_VER == MEDIAREC_VER_2_0)
    uiFileType          = MEDIA_FILEFORMAT_MOV;                 // file type
    #else // MEDIAREC_VER_3_0, MP4 + AAC.
    uiFileType          = MEDIA_FILEFORMAT_MP4;                 // file type
    #endif

    if (UI_GetData(FL_MOVIE_DUAL_REC))
    {
        uiWidth             = GetMovieSizeWidth_2p(0, uiMovieSize);         // image width
        uiHeight            = GetMovieSizeHeight_2p(0, uiMovieSize);        // image height
        uiVidFrameRate      = GetMovieFrameRate_2p(0, uiMovieSize);         // video frame rate
        uiTargetBitrate     = GetMovieTargetBitrate_2p(0, uiMovieSize);     // enable rate control and set target rate
        uiDispAspectRatio   = GetMovieDispAspectRatio_2p(0, uiMovieSize);   // display aspect ratio
    }
    else
    {
        uiWidth             = GetMovieSizeWidth(uiMovieSize);               // image width
        uiHeight            = GetMovieSizeHeight(uiMovieSize);              // image height
        uiVidFrameRate      = GetMovieFrameRate(uiMovieSize);               // video frame rate
        uiTargetBitrate     = GetMovieTargetBitrate(uiMovieSize, uiQuality);// enable rate control and set target rate
        uiDispAspectRatio   = GetMovieDispAspectRatio(uiMovieSize);         // display aspect ratio
    }
    uiH264GopType       = MEDIAREC_H264GOP_IPONLY;              // H.264 GOP type (IP only)
    //uiH264GopType       = MEDIAREC_H264GOP_IPB;                 // H.264 GOP type (IPB)

    if (UI_GetData(FL_MovieMCTFIndex) == MOVIE_MCTF_OFF)
    {
        ui3DNRLevel     = H264_3DNR_DISABLE;                    // H.264 3DNR disable
    }
    else
    {
        //ui3DNRLevel     = H264_3DNR_NORMAL;                     // H.264 3DNR enable (normal)
        //ui3DNRLevel     = H264_3DNR_WEAK;                       // H.264 3DNR enable (weak)
        ui3DNRLevel     = H264_3DNR_STRONGEST;                  // H.264 3DNR enable (strongest)
    }

    uiAudSampleRate     = 32000;                                // audio sampling rate
    uiAudChannelType    = MOVREC_AUDTYPE_RIGHT;                 // audio channel type
    uiAudChannelNum     = 1;                                    // audio channel number
    bAudFilterEn        = FALSE;                                // enable/disable audio filter

    switch (uiFileType)
    {
    case MEDIA_FILEFORMAT_AVI: // AVI format, audio should be PCM, video can be H.264 or MJPEG
        uiAudCodec  = MOVAUDENC_PCM;
        uiVidCodec  = MEDIAVIDENC_H264;
        //uiVidCodec  = MEDIAREC_ENC_JPG;
        break;

    case MEDIA_FILEFORMAT_MP4: // MP4 format, audio should be AAC, video should be H.264
        uiAudCodec  = MOVAUDENC_AAC;
        uiVidCodec  = MEDIAVIDENC_H264;
        break;

    case MEDIA_FILEFORMAT_MOV: // MOV format, audio can be PCM or AAC, video can be H.264 or MJPEG
    default:
        //uiAudCodec  = MEDIAREC_ENC_AAC;
        uiAudCodec  = MOVAUDENC_PCM;
        uiVidCodec  = MEDIAVIDENC_H264;
        //uiVidCodec  = MEDIAREC_ENC_JPG;
        break;
    }

    //#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
    uiVidRotate = MOVREC_MOV_ROTATE_0;
    //#NT#2013/04/17#Calvin Chang -end

    // sample code for EMR
    #if 0
	if (!(UI_GetData(FL_MOVIE_DUAL_REC) && g_bSensor2En))
	{
        DBG_DUMP("EMR Enable..\r\n");
        MovRec_SetMaxStopFlashSec(5); // flash back 5 seconds
        MovRec_SetMaxKeepSec(55);     // keep recording 30 sec		// EMR total time 60s @360
        MovRec_SetEmergencyRecording(0, ENABLE); // enable path 0 EMR
        MovRec_SetEmergencyRecording(1, DISABLE); // disable path 1 EMR
	}
    #endif

    if(MEM_DRAM_SIZE < 0x10000000) // Reduce IPL display buffer size if 1Gb DRAM is used.
    {
        IPL_Display_SetBufferNum(6);
    }
#if  (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
    #if (_SENSORLIB_ == _SENSORLIB_VIRTUAL_ && _SENSORLIB2_ == _SENSORLIB2_CMOS_TVP5150_)
    {
        DscMovie_Config(MOVIE_CFG_IMGPATH, MOVIE_WIFI_DUAL_SENSOR_1STINACT);
        MovRec_SetWriteCard(0, 0);  // No Saving the 1st sensor image sream to SD card.
        MovRec_SetWriteCard(1, 0);  // No Saving the 2nd sensor image sream to SD card.
    }
    #else
    {
        uiH264GopType = MEDIAREC_H264GOP_IPONLY; // forced set to IP-only for dual recording due to buffer issue

        if(MEM_DRAM_SIZE < 0x10000000) // Reduce IPL display buffer size if 1Gb DRAM is used.
        {
            IPL_Display_SetBufferNum(6);
            IPL_Display_SetBufferNum2(5);
        }
        else
        {
            //IPL_Display_SetBufferNum(8);
            //IPL_Display_SetBufferNum2(8);
            IPL_Display_SetBufferNum(10);
            IPL_Display_SetBufferNum2(10);
        }

        //IPL_Display_SetEncodeLag(1);
        IPL_Display_SetEncodeLag(2);
        if (Sensor_CheckExtSensor())
        {
            if (UI_GetData(FL_MOVIE_DUAL_REC) && g_bSensor2En)
            {
                DscMovie_Config(MOVIE_CFG_IMGPATH, MOVIE_DUAL_SENSOR);
                MovRec_SetWriteCard(0, 1);  // Save the 1st sensor image sream to SD card.
                #if (SENSOR2_DISP_ONLY)
                MovRec_SetWriteCard(1, 0);  // No saving the 2nd sensor image sream to SD card.
                #else
                MovRec_SetWriteCard(1, 1);  // Save the 2nd sensor image sream to SD card.
				MovRec_SetEmergencyRecording(0, DISABLE); // disable path 0 EMR for writing multi files
                #endif
            }
            else
            {
                DscMovie_Config(MOVIE_CFG_IMGPATH, MOVIE_DUAL_SENSOR_2NDINACT);
                MovRec_SetWriteCard(0, 1);  // Save the 1st sensor image sream to SD card.
                MovRec_SetWriteCard(1, 0);  // No saving the 2nd sensor image sream to SD card.
            }
        }
        else
        {
            DscMovie_Config(MOVIE_CFG_IMGPATH, MOVIE_DUAL_SENSOR_2NDOFF);
        }

        #if (!SENSOR2_DISP_ONLY)
        MovRec_EnableEndChar4SingleRec(TRUE);
        MovRec_EnableEndChar4MultiRec(TRUE);
        #endif
    }
    #endif
#else

    if (uiH264GopType == MEDIAREC_H264GOP_IPB) // IPB needs 9 buffers at least, set 10 for insurance
    {
        IPL_Display_SetBufferNum(10);
    }
    else
    {
        if(MEM_DRAM_SIZE < 0x10000000) // Reduce IPL display buffer size if 1Gb DRAM is used.
        {
            IPL_Display_SetBufferNum(6); // IPP needs 6 buffers at least
        }
        else
        {
            IPL_Display_SetBufferNum(8); // IPP needs 6 buffers at least, set 8 for insurance
        }
    }

    MovRec_EnableEndChar4SingleRec(FALSE);
    MovRec_EnableEndChar4MultiRec(FALSE);

#endif

    //---------------------------------------------------------------------------------------------
    // set media recording control parameters
    //---------------------------------------------------------------------------------------------
    // disable golf shot recording
    MovRec_ChangeParameter(MOVREC_RECPARAM_GOLFSHOT_ON, FALSE, 0, 0);
    // disable flash recording
    MovRec_ChangeParameter(MOVREC_RECPARAM_FLASH_ON, FALSE, 0, 0);
    // enable/disable power off protection
    MovRec_ChangeParameter(MOVREC_RECPARAM_PWROFFPT, TRUE, 0, 0);
    #if (YOUKU_SDK == ENABLE)
    MovRec_SetEmergencyRecording(1,1);
    #endif
    // disable time lapse recording => not necessary now, since it can be set by using MEDIAREC_RECPARAM_RECFORMAT
    //MediaRec_ChangeParameter(MEDIAREC_RECPARAM_TIMELAPSE, FALSE, 0, 0);
    // set recording format (MEDIAREC_VID_ONLY, MEDIAREC_AUD_VID_BOTH, MEDIAREC_TIMELAPSE, ...)
    MovRec_ChangeParameter(MOVREC_RECPARAM_RECFORMAT, uiRecFormat, 0, 0);
    // set customized user data on/off
    //MediaRec_ChangeParameter(MEDIAREC_RECPARAM_EN_CUSTOMUDATA, FALSE, 0, 0);
    // set file type (MEDIAREC_AVI, MEDIAREC_MOV, MEDIAREC_MP4)
    MovRec_ChangeParameter(MOVREC_RECPARAM_FILETYPE, uiFileType, 0, 0);

    // set recording end type (MEDIAREC_ENDTYPE_NORMAL, MEDIAREC_ENDTYPE_CUTOVERLAP, MEDIAREC_ENDTYPE_CUT_TILLCARDFULL)
    // MP4 cannot support seamless recording
    if ((UI_GetData(FL_MOVIE_CYCLIC_REC) != MOVIE_CYCLICREC_OFF) /*&& (uiFileType != MEDIAREC_MP4)*/)
    {
        MovRec_ChangeParameter(MOVREC_RECPARAM_ENDTYPE, MOVREC_ENDTYPE_CUTOVERLAP, 0, 0);
    }
    else
    {
        #if 1
        MovRec_ChangeParameter(MOVREC_RECPARAM_ENDTYPE, MOVREC_ENDTYPE_NORMAL, 0, 0);
        #else
        MediaRec_ChangeParameter(MEDIAREC_RECPARAM_ENDTYPE, MEDIAREC_ENDTYPE_CUT_TILLCARDFULL, 0, 0);
        MediaRec_ChangeParameter(MEDIAREC_RECPARAM_CUTSEC, 1500, 0, 0); // 25 min
        #endif
    }

    // enable/disable time lapse recording
    if (uiRecFormat == MEDIAREC_TIMELAPSE)
    {
		MovRec_ChangeParameter(MOVREC_RECPARAM_TIMELAPSE, TRUE, 0, 0);
		switch(SysGetFlag(FL_MOVIE_TIMELAPSE_REC))
		{
		case MOVIE_TIMELAPSEREC_100MS:
		default:
			MovRec_ChangeParameter(MOVREC_RECPARAM_TIMELAPSE_TIME, 100, 0, 0);
			break;
		case MOVIE_TIMELAPSEREC_200MS:
			MovRec_ChangeParameter(MOVREC_RECPARAM_TIMELAPSE_TIME, 200, 0, 0);
			break;
		case MOVIE_TIMELAPSEREC_500MS:
			MovRec_ChangeParameter(MOVREC_RECPARAM_TIMELAPSE_TIME, 500, 0, 0);
			break;
		}
		MovRec_ChangeParameter(MOVREC_RECPARAM_ENDTYPE, MOVREC_ENDTYPE_NORMAL, 0, 0); // should be normal end for time lapse recording
    }
    else
    {
        MovRec_ChangeParameter(MOVREC_RECPARAM_TIMELAPSE, FALSE, 0, 0);
    }

    //---------------------------------------------------------------------------------------------
    // set video parameters
    //---------------------------------------------------------------------------------------------
    // set image width
    MovRec_ChangeParameter(MOVREC_RECPARAM_WIDTH, uiWidth, 0, 0);
    // set image height
    MovRec_ChangeParameter(MOVREC_RECPARAM_HEIGHT, uiHeight, 0, 0);
    // set video frame rate
    MovRec_ChangeParameter(MOVREC_RECPARAM_FRAMERATE, uiVidFrameRate, 0, 0);
    // set target data rate (bytes per second)
    MovRec_ChangeParameter(MOVREC_RECPARAM_TARGETRATE, uiTargetBitrate, 0, 0);
    // set video codec (MEDIAREC_ENC_H264, MEDIAREC_ENC_JPG)
    MovRec_ChangeParameter(MOVREC_RECPARAM_VIDEOCODEC, uiVidCodec, 0, 0);
    // set H.264 GOP type (MEDIAREC_H264GOP_IPONLY, MEDIAREC_H264GOP_IPB)
    MovRec_ChangeParameter(MOVREC_RECPARAM_H264GOPTYPE, uiH264GopType, 0, 0);
    // set H.264 3DNR level (H264_3DNR_DISABLE, H264_3DNR_WEAKEST, H264_3DNR_WEAK, ...)
    MovRec_ChangeParameter(MOVREC_RECPARAM_H2643DNRLEVEL, ui3DNRLevel, 0, 0);
    //#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
    // set rotation information
    MovRec_ChangeParameter(MOVREC_RECPARAM_MOV_ROTATE, uiVidRotate, 0, 0);
    //#NT#2013/04/17#Calvin Chang -end
    //#NT#2013/09/17#Calvin Chang#add for YUV fromat setting -begin
    if (uiVidCodec == MEDIAVIDENC_MJPG)
        MovRec_ChangeParameter(MOVREC_RECPARAM_JPG_YUVFORMAT, MOVREC_JPEG_FORMAT_420, 0, 0);
    //#NT#2010/09/17#Calvin Chang -end
    MovRec_ChangeParameter(MOVREC_RECPARAM_DAR, uiDispAspectRatio, 0, 0);

	#if (SENSOR2_DISP_ONLY)
    if (UI_GetData(FL_MOVIE_DUAL_REC) && (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI))
	#else
    if (UI_GetData(FL_MOVIE_DUAL_REC))
	#endif
    {
        uiWidth2            = GetMovieSizeWidth_2p(1, uiMovieSize);         // image width
        uiHeight2           = GetMovieSizeHeight_2p(1, uiMovieSize);        // image height
        uiVidFrameRate2     = GetMovieFrameRate_2p(1, uiMovieSize);         // video frame rate of path 2.
        uiTargetBitrate2    = GetMovieTargetBitrate_2p(1, uiMovieSize);     // enable rate control and set target rate
        uiDispAspectRatio2  = GetMovieDispAspectRatio_2p(1, uiMovieSize);   // display aspect ratio

        DscMovie_Config(MOVIE_CFG_MULTIREC, 1);
        MovRec_ChangeParameter(MOVREC_RECPARAM_MULTIREC_ON, TRUE, 0, 0);

        //MovieExe_SetGetReadyCB2_Sample();
        MovRec_ChangeParameter(MOVREC_RECPARAM_FILETYPE_2, uiFileType, 0, 0);
        MovRec_ChangeParameter(MOVREC_RECPARAM_VIDEOCODEC, MEDIAVIDENC_H264, 1, 0);
        //MediaRec_ChangeParameter(MEDIAREC_RECPARAM_VIDEOCODEC, MEDIAREC_ENC_JPG, 1, 0);
        MovRec_ChangeParameter(MOVREC_RECPARAM_JPG_YUVFORMAT, MOVREC_JPEG_FORMAT_420, 0, 0);
        MovRec_ChangeParameter(MOVREC_RECPARAM_WIDTH, uiWidth2, 1, 0);
        MovRec_ChangeParameter(MOVREC_RECPARAM_HEIGHT, uiHeight2, 1, 0);
        // set video frame rate
        MovRec_ChangeParameter(MOVREC_RECPARAM_FRAMERATE, uiVidFrameRate2, 1, 0);
        MovRec_ChangeParameter(MOVREC_RECPARAM_TARGETRATE, uiTargetBitrate2, 1, 0);
        MovRec_ChangeParameter(MOVREC_RECPARAM_DAR, uiDispAspectRatio2, 1, 0);

        #if  (_SENSORLIB2_ == _SENSORLIB2_DUMMY_)
        DscMovie_Config(MOVIE_CFG_IMGPATH, MOVIE_DUAL_REC_TEST); // just for dual recording test!!!
        MovRec_EnableEndChar4SingleRec(FALSE);
        MovRec_EnableEndChar4MultiRec(TRUE);
        #endif
    }
    else
    {
        #if (_MULTI_RECORD_FUNC_ && (SENSOR2_DISP_ONLY == DISABLE))
        DscMovie_Config(MOVIE_CFG_MULTIREC, 1); // always enable ready buffer CB2
        #else
        DscMovie_Config(MOVIE_CFG_MULTIREC, 0);
        #endif
        MovRec_ChangeParameter(MOVREC_RECPARAM_MULTIREC_ON, FALSE, 0, 0);
    }

#if (1)//(WIFI_AP_FUNC)

    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {

#if(MOVIE_LIVEVIEW==RTSP_LIVEVIEW)
        #if (_SENSORLIB_ == _SENSORLIB_VIRTUAL_ && _SENSORLIB2_ == _SENSORLIB2_CMOS_TVP5150_)
        {
            DscMovie_Config(MOVIE_CFG_IMGPATH, MOVIE_WIFI_DUAL_SENSOR_1STINACT);
            MovRec_SetWriteCard(0, 0);  // No Saving the 1st sensor image sream to SD card.
            MovRec_SetWriteCard(1, 0);  // No Saving the 2nd sensor image sream to SD card.
        }
        #else
        if(UI_GetData(FL_WIFI_MOVIE_FMT) ==WIFI_RTSP_LIVEVIEW)
        {
            #if (SENSOR2_DISP_ONLY)
            DscMovie_Config(MOVIE_CFG_IMGPATH, MOVIE_WIFI_PREVIEW_SEN2_DISPONLY);
            #else
            DscMovie_Config(MOVIE_CFG_IMGPATH, MOVIE_WIFI_PREVIEW);
            #endif

            MovRec_SetWriteCard(0, 0);  // No Saving the 1st sensor image sream to SD card. Because It's live view mode.
            MovRec_SetWriteCard(1, 0);  // No Saving the 2nd sensor image sream to SD card.
			MovRec_SetEmergencyRecording(0, DISABLE); // disable path 0 EMR for WiFi live view
        }
        else
        {
            // If time lapse recording fun. was set, enable normal recording mode instead.
            if (UI_GetData(FL_MOVIE_TIMELAPSE_REC) != MOVIE_TIMELAPSEREC_OFF)
            {
                DscMovie_Config(MOVIE_CFG_IMGPATH,MOVIE_NORMAL);
                DscMovie_Config(MOVIE_CFG_MULTIREC, 0); //wifi off,movie should be single record
                MovRec_ChangeParameter(MOVREC_RECPARAM_MULTIREC_ON, FALSE, 0, 0);
                MovRec_SetWriteCard(0, 1);  // Save the 1st sensor image sream to SD card.
                MovRec_SetWriteCard(1, 0);  // No Saving the 2nd sensor image sream to SD card.
				MovRec_SetEmergencyRecording(0, DISABLE); // disable path 0 EMR for Timelapse
            }
            else
            {
                #if (SENSOR2_DISP_ONLY)
                	DscMovie_Config(MOVIE_CFG_IMGPATH,MOVIE_WIFI_RECORD_SEN2_DISPONLY);
                #else
                	DscMovie_Config(MOVIE_CFG_IMGPATH,MOVIE_WIFI_RECORD);
                #endif
                MovRec_SetWriteCard(0, 1);  // Save the 1st sensor image sream to SD card.
                #if (YOUKU_SDK == ENABLE)
                	MovRec_SetWriteCard(1, 1);  // Saving the WiFi sream to SD card.
					MovRec_SetEmergencyRecording(0, DISABLE); // disable path 0 EMR for writing multi files
                #elif (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
                	MovRec_SetWriteCard(1, 1);  // Save the 2nd path image sream to SD card.
					MovRec_SetEmergencyRecording(0, DISABLE); // disable path 0 EMR for writing multi files
                #else
                	MovRec_SetWriteCard(1, 0);  // No Saving the 2nd sensor image sream to SD card.
                #endif
            }
        }
        #endif
#elif(MOVIE_LIVEVIEW==HTTP_LIVEVIEW)

        //as normal record,but config path 3 for HTTP MJPG
        DscMovie_Config(MOVIE_CFG_IMGPATH,MOVIE_DUAL_DISPLAY);
        DscMovie_Config(MOVIE_CFG_MULTIREC, 0); //wifi off,movie should be single record
        MovRec_SetWriteCard(0, 1);  // Save the 1st sensor image sream to SD card.
        MovRec_SetWriteCard(1, 0);  // No Saving the 2nd sensor image sream to SD card.
#elif(MOVIE_LIVEVIEW==DUAL_REC_HTTP_LIVEVIEW)
        //nothing ,defulat is multi record,not dual display
#endif

    }
    else
    {
        #if (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
            DscMovie_Config(MOVIE_CFG_IMGPATH, MOVIE_DUAL_REC_TEST);
            DscMovie_Config(MOVIE_CFG_MULTIREC, 1);
            MovRec_ChangeParameter(MOVREC_RECPARAM_MULTIREC_ON, TRUE, 0, 0);
            MovRec_SetWriteCard(0, 1);  // Save the 1st path image sream to SD card.
            MovRec_SetWriteCard(1, 1);  // Save the 2nd path image sream to SD card.
			MovRec_SetEmergencyRecording(0, DISABLE); // disable path 0 EMR for writing multi files
        #else
            if(!DrvSensor_Det2ndSensor())
            {
                #if(DUALDISP_FUNC==DUALDISP_ONBOTH) // Cannot do recording while DUAL DISPLAY is enabled.
                if(DscMovie_GetConfig(MOVIE_CFG_IMGPATH) != MOVIE_DUAL_DISPLAY)
                #endif
                {
                    #if  (_SENSORLIB2_ == _SENSORLIB2_DUMMY_)
                    DscMovie_Config(MOVIE_CFG_IMGPATH,MOVIE_NORMAL);
                    #endif
                }
                DscMovie_Config(MOVIE_CFG_MULTIREC, 0);
                MovRec_ChangeParameter(MOVREC_RECPARAM_MULTIREC_ON, FALSE, 0, 0);
                MovRec_SetWriteCard(0, 1);  // Save the 1st sensor image sream to SD card.
                MovRec_SetWriteCard(1, 0);  // No Saving the 2nd sensor image sream to SD card.
            }
        #endif
    }

#endif

    //---------------------------------------------------------------------------------------------
    // set audio parameters
    //---------------------------------------------------------------------------------------------
    // set audio sampling rate (AUDIO_SR_8000, AUDIO_SR_11025, AUDIO_SR_12000, ...)
    MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_SAMPLERATE, uiAudSampleRate, 0, 0);
    // set audio source (AUDIO_CH_RIGHT, AUDIO_CH_LEFT, AUDIO_CH_STEREO)
    MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_SRC, uiAudChannelType, 0, 0);
    // set audio channel number (1 or 2; if audio channel is mono but channel number is 2, the audio data will be duplicated)
    MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_CHS, uiAudChannelNum, 0, 0);
    // set audio codec (MEDIAREC_ENC_PCM, MEDIAREC_ENC_AAC)
    MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_CODEC, uiAudCodec, 0, 0);
    // set audio filter on/off
    MovRec_ChangeParameter(MOVREC_RECPARAM_EN_AUDFILTER, bAudFilterEn, 0, 0);
    // Enable Tiny mode test
    if(MEM_DRAM_SIZE < 0x10000000) // Reduce IPL display buffer size if 1Gb DRAM is used.
    {
        //MovRec_ChangeParameter(MOVREC_RECPARAM_TINYMODE_ON, TRUE, 0, 0);
    }
}

void MovieExe_CalcDispSize(ISIZE *pDispSize, ISIZE *pDevSize, ISIZE *pDevRatio, ISIZE *pImgRatio)
{
    if (((pDevRatio->w * 100) / pDevRatio->h) > ((pImgRatio->w * 100) / pImgRatio->h)) // device aspect ratio > image ratio
    {
        pDispSize->h = pDevSize->h;
        pDispSize->w = ALIGN_ROUND_16((pDevSize->w * pImgRatio->w * pDevRatio->h) / (pImgRatio->h * pDevRatio->w));
    }
    else
    {
        pDispSize->w = pDevSize->w;
        pDispSize->h = ALIGN_ROUND_4((pDevSize->h * pImgRatio->h * pDevRatio->w) / (pImgRatio->w * pDevRatio->h));
    }
}

#if MOVIE_FD_FUNC_
void Movie_FDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    // Init FD buffer, set max face number to 10, and process rate as 3
    FD_Init(buf, cachebuf, 10, 15, 3);
}

void Movie_FDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
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
    ImageRatioIdx = GetMovieSizeRatio();
    FD_Process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
}
static PHOTO_FUNC_INFO PhotoFuncInfo_fd =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Movie_FDInit,           ///< init fp, only execute at first process
    Movie_FDProcess,        ///< process fp
    NULL,                   ///< process end fp
    FD_CalcBuffSize,        ///< get working buffer fp
    FD_CalcCacheBuffSize,   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
INT32 MovieExe_OnDraw(DISPSRV_DRAW* pDraw)
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

extern PHOTO_FUNC_INFO PhotoFuncInfo_dummy;
extern PHOTO_FUNC_INFO PhotoFuncInfo_dis;
extern PHOTO_FUNC_INFO PhotoFuncInfo_OT;

ISIZE _QueryDeviceModeAspect(UINT32 NewDevObj, UINT32 mode);

#if _QR_FUNC_
#include <cyg\zbar\zbar.h>
typedef void (*QRDATA_CB)(UINT32 p1, UINT32 p2,UINT32 p3);

static UINT32 gQRLock=TRUE;
static UINT32 gQRProcessRate = 5;
static QRDATA_CB gQRDataCB=NULL;

void QR_Lock(BOOL bLock)
{
    gQRLock = bLock;
    DBG_IND("gQRLock %d\r\n",gQRLock);
}
UINT32 QR_Processs(UINT32 uiOutAddrY,UINT32 w,UINT32 h)
{
    zbar_image_scanner_t *scanner = NULL;
    zbar_image_t *img;
    zbar_image_t *test;
    UINT32 bFind = 0;

    scanner = zbar_image_scanner_create();

    /* configure the reader */
    zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);

    img = zbar_image_create();
    zbar_image_set_size(img, w, h);
    zbar_image_set_format(img, MAKEFOURCC('Y','8','0','0'));
    zbar_image_set_data(img, (const void *)uiOutAddrY, w*h, NULL);

    test = zbar_image_convert(img, MAKEFOURCC('Y','8','0','0'));

    if(!test)
        return(2);
    #if 0
    DBG_IND("converted: %d x %d (%lx) %08lx 0x%x\r\n",
           zbar_image_get_width(test),
           zbar_image_get_height(test),
           zbar_image_get_data_length(test),
           zbar_image_get_format(test),
           zbar_image_get_data(test));
    #endif
    /* scan the image for barcodes */
    zbar_scan_image(scanner, test);

    /* extract results */
    const zbar_symbol_t *symbol = zbar_image_first_symbol(test);
    for(; symbol; symbol = zbar_symbol_next(symbol)) {

        /* do something useful with results */
        zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
        const char *data = zbar_symbol_get_data(symbol);
        /*
        DBG_IND("decoded %s symbol \"%s\"\n",
               zbar_get_symbol_name(typ), data);
        */
        if(gQRDataCB)
        {
            bFind = 1;
            gQRDataCB((UINT32)zbar_get_symbol_name(typ),(UINT32)data,0);
        }
    }
    /* clean up */
    zbar_image_destroy(test);
    zbar_image_destroy(img);
    zbar_image_scanner_destroy(scanner);

    if(bFind)
        return TRUE;
    else
        return FALSE;
}
void QR_DataCb(UINT32 parm1,UINT32 parm2,UINT32 parm3)
{
    if(parm1){
        char *QRData = (char *)parm2;
        char *pSeparator=0;
        char pSSID[WSC_MAX_SSID_LEN];
        char pPwd[MAX_WEP_KEY_LEN];

        DBG_IND("decoded %s symbol \"%s\"\r\n",parm1,QRData);

        //SSID & Password sample for WiFi
        #if 0
        pSeparator=strchr(QRData,'%');
        if(pSeparator)
        {
            memset(pSSID,0,WSC_MAX_SSID_LEN);
            memset(pPwd,0,MAX_WEP_KEY_LEN);

            snprintf(pPwd,MAX_WEP_KEY_LEN-1,"%s",pSeparator+1);
            QRData[pSeparator-QRData] =0;
            snprintf(pSSID,WSC_MAX_SSID_LEN-1,"%s",QRData);
        }
        else
        {
            DBG_ERR("data no pSeparator ex: SSID\%PWD\r\n");
        }
        #endif
   }
}

void Photo_QRInit(QRDATA_CB CB, UINT32 ProcessRate)
{
    gQRProcessRate = ProcessRate;
    gQRDataCB = CB;
    gQRLock = FALSE;
}

void Photo_QRProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    if(!gQRLock)
    {
        IPL_IME_BUF_ADDR CurInfo;
        UINT32           srcW,srcH;
        UINT32           PxlAddr[3];
        UINT32           LineOff[3];
        static UINT32    gQRcount = 0;

        CurInfo.Id = IPL_ID_1;

        IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&CurInfo);

        srcW = CurInfo.ImeP2.Ch[0].Width;
        srcH = CurInfo.ImeP2.Ch[0].Height;
        LineOff[0] = CurInfo.ImeP2.Ch[0].LineOfs;
        PxlAddr[0] = CurInfo.ImeP2.PixelAddr[0];

        //DBG_IND("w:%d,h:%d %x\r\n",srcW, srcH, PxlAddr[0]);

        gQRcount++;
        if(gQRcount >= gQRProcessRate)
        {
            gQRcount = 0;
            if(QR_Processs(PxlAddr[0],srcW, srcH)==TRUE)
            {
                gQRLock = TRUE;
            }
        }


    }
}
PHOTO_FUNC_INFO PhotoFuncInfo_qr =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    NULL,                   ///< init fp, only execute at first process
    Photo_QRProcess,        ///< process fp
    NULL,                   ///< process end fp
    NULL,                   ///< get working buffer fp
    NULL,                   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif


/**
  Initialize application for AVI mode

  Initialize application for AVI mode.

  @param void
  @return void
*/
INT32 MovieExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    MEM_RANGE Pool;
    ISIZE BufferSize;
    ISIZE DeviceSize;
    ISIZE AspectSize;
    ISIZE DispSize;
    ISIZE ImgRatio;
    UINT32 CurrDevObj;
    IPL_SENCROPRATIO CropRatio;
    UINT32 useFileDB = 0;
#if  (_SENSORLIB2_ == _SENSORLIB2_DUMMY_)
    UINT32 uiMovieSize = 0;
#endif
    BOOL IsRotatePanel = (VDO_USE_ROTATE_BUFFER==TRUE||OSD_USE_ROTATE_BUFFER==TRUE);

    PHOTO_FUNC_INFO* phead = NULL;

    //call default
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_OPEN,paramNum,paramArray);

    Pool.Addr = OS_GetMempoolAddr(POOL_ID_APP);
    Pool.Size = POOL_SIZE_APP;
    DscMovie_Config(MOVIE_CFG_POOL, (UINT32)&Pool);

    #if (MOVIE_PIM == ENABLE)
    {
        DscMovie_Config(MOVIE_CFG_PIMSIZE, 88*1024*1024); // 88MB for picture in movie function (can capture 2 14M images)
    }
    #endif

    //1.get current device size (current mode)
    DeviceSize = GxVideo_GetDeviceSize(DOUT1);
    BufferSize = DeviceSize;
    #if (_FPGA_EMULATION_ == ENABLE)
    //overwrite buffer size to reduce BW
    BufferSize.w = 320;
    BufferSize.h = 240;
    #endif
    #if(RTSP_LIVEVIEW_FUNC==ENABLE)
    BufferSize.w = 80;
    BufferSize.h = 60;
    #endif
    //3.get its Aspect Ratio
    CurrDevObj = GxVideo_GetDevice(DOUT1);
    AspectSize = _QueryDeviceModeAspect(CurrDevObj, GxVideo_QueryDeviceLastMode(CurrDevObj));

    phead = &PhotoFuncInfo_dummy;
    phead->pNext = NULL; //mark end
    #if (MOVIE_TEST_ENABLE != ENABLE)
    {// DIS function
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_dis;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }
    #endif

    #if _QR_FUNC_
    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_qr;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
        BufferSize.w = DISPLAY_VDO_W;
        BufferSize.h = DISPLAY_VDO_H;
        DeviceSize.w = DISPLAY_VDO_W;
        DeviceSize.h = DISPLAY_VDO_H;

        Photo_QRInit(QR_DataCb,10);
        CHKPNT;

    }
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

    DBG_IND("\r\n LDWS::Movie LDWS  RegisterCB \r\n" );
    #endif


    if(1)
    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_OT; //Object tracking
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }

    #if MOVIE_FD_FUNC_
    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_fd;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }
    MovieExe_GetFDDispCord(&gMovieFdDispCord);
    // unlock FD
    FD_Lock(FALSE);
    #endif

    DscMovie_Config(MOVIE_CFG_FUNCINFO, (UINT32)phead);
    #if MOVIE_FD_FUNC_
    DscMovie_Config(MOVIE_CFG_CACHESIZE, 1250 * 1024);
    #else
    DscMovie_Config(MOVIE_CFG_CACHESIZE, 0);
    #endif
    DscMovie_Config(MOVIE_CFG_IMGPATH,MOVIE_NORMAL);
    Movie_RegCB();

    #if (MOVIE_TEST_ENABLE == ENABLE) // test mode: using fixed YUV patterns
    {
        g_MediaRecTestMode.bEnable = TRUE;
        g_MediaRecTestMode.uiVidSrcType = MEDIAREC_SRC_FIXED_YUV;
        g_MediaRecTestMode.uiAudSrcType = MEDIAREC_SRC_NORMAL;

        // Set YUV information if using fixed YUV as video source (MEDIAREC_SRC_FIXED_YUV)
        if (g_MediaRecTestMode.uiVidSrcType == MEDIAREC_SRC_FIXED_YUV)
        {
            //Movie_SetYUV(UI_GetData(FL_MOVIE_SIZE));
        }

        MovRec_SetTestMode(&g_MediaRecTestMode);
    }
    #else // real sensor
    {
        SENSOR_INIT_OBJ SenObj;
        SENSOR_DRVTAB *SenTab;
        //open sensor
        SenObj = DrvSensor_GetObj1st();
        SenTab = DrvSensor_GetTab1st();
        Sensor_Open(SENSOR_ID_1, &SenObj, SenTab);
#if  (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
        if (Sensor_CheckExtSensor())
        {
            SENSOR_INIT_OBJ SenObj;
            SENSOR_DRVTAB *SenTab;

            Movie_SetDualRec(ON);
            SenObj = DrvSensor_GetObj2nd();
            SenTab = DrvSensor_GetTab2nd();
            Sensor_Open(SENSOR_ID_2, &SenObj, SenTab);
            g_bSensor2En = TRUE;
        }
        else
        {
            // forced disable dual recording if no 2nd sensor
            Movie_SetDualRec(OFF);
            g_bSensor2En = FALSE;
        }
#endif
    }

    #if  (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
    {
        switch (UI_GetData(FL_MOVIE_SIZE_DUAL))
        {
            case MOVIE_SIZE_1080P_720P:
            case MOVIE_SIZE_720P_720P:
            default:
                CropRatio = IPL_SENCROPRATIO_16_9;
                ImgRatio.w = 16;
                ImgRatio.h = 9;
                break;
        }
    }
    #else
    {
        if (UI_GetData(FL_MOVIE_DUAL_REC))
        {
            uiMovieSize = UI_GetData(FL_MOVIE_SIZE_DUAL);
        }
        else
        {
            uiMovieSize = UI_GetData(FL_MOVIE_SIZE);
        }

        switch (uiMovieSize)
        {
            case MOVIE_SIZE_VGA:
            case MOVIE_SIZE_QVGA:
                CropRatio = IPL_SENCROPRATIO_4_3;
                ImgRatio.w = 4;
                ImgRatio.h = 3;
                break;

            case MOVIE_SIZE_1080P_FULLRES:
            case MOVIE_SIZE_1080P:
            case MOVIE_SIZE_720P:
            //case MOVIE_SIZE_WVGA:
            default:
                CropRatio = IPL_SENCROPRATIO_16_9;
                ImgRatio.w = 16;
                ImgRatio.h = 9;
                break;
        }
    }
    #endif

    DscMovie_Config(MOVIE_CFG_RATIO, CropRatio);

    //config view for primary display device
    MovieExe_CalcDispSize(&DispSize, &DeviceSize, &AspectSize, &ImgRatio);
    AppView_ConfigEnable(0, TRUE);
    AppView_ConfigDevice(0, DeviceSize.w, DeviceSize.h, AspectSize.w, AspectSize.h);
    AppView_ConfigBuffer(0, BufferSize.w, BufferSize.h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED);
    AppView_ConfigWindow(0, 0, 0, DeviceSize.w, DeviceSize.h);
    AppView_ConfigImgSize(0, DispSize.w, DispSize.h, ImgRatio.w, ImgRatio.h);
    #if PIP_VIEW_FUNC
    PipView_SetPrimaryImgRatio((USIZE*)&ImgRatio);
    #endif

    //config view for secondary display device
    CurrDevObj = GxVideo_GetDevice(DOUT2);
    if (CurrDevObj)
    {
        DeviceSize = GxVideo_GetDeviceSize(DOUT2);
        BufferSize = DeviceSize;
        AspectSize = _QueryDeviceModeAspect(CurrDevObj, GxVideo_QueryDeviceLastMode(CurrDevObj));
        MovieExe_CalcDispSize(&DispSize, &DeviceSize, &AspectSize, &ImgRatio);

        AppView_ConfigEnable(1, TRUE);
        AppView_ConfigDevice(1, DeviceSize.w, DeviceSize.h, AspectSize.w, AspectSize.h);
        AppView_ConfigBuffer(1, BufferSize.w, BufferSize.h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED);
        AppView_ConfigWindow(1, 0, 0, DeviceSize.w, DeviceSize.h);
        AppView_ConfigImgSize(1, DispSize.w, DispSize.h, ImgRatio.w, ImgRatio.h);
        DscMovie_Config(MOVIE_CFG_IMGPATH,MOVIE_DUAL_DISPLAY);
    }
    else
    {
        AppView_ConfigEnable(1, FALSE);
    }

#if(WIFI_AP_FUNC==ENABLE && MOVIE_LIVEVIEW==HTTP_LIVEVIEW)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        UINT32 w,h;
        AppView_ConfigEnable(1, TRUE);
        if(CropRatio==IPL_SENCROPRATIO_16_9)
        {
            w = ALIGN_CEIL_16(HTTP_MJPG_W_16);
            h = ALIGN_CEIL_16(HTTP_MJPG_H_9);
        }
        else
        {
            w = ALIGN_CEIL_16(HTTP_MJPG_W_4);
            h = ALIGN_CEIL_16(HTTP_MJPG_H_3);
        }
        AppView_ConfigBuffer(1, HTTP_MJPG_W_4, HTTP_MJPG_H_3, GX_IMAGE_PIXEL_FMT_YUV420_PACKED);
        AppView_ConfigImgSize(1, w, h, ImgRatio.w, ImgRatio.h);
        DscMovie_Config(MOVIE_CFG_IMGPATH,MOVIE_DUAL_DISPLAY);
    }
#endif

    //config src
    #if PIP_VIEW_FUNC
    gMovie_src.bDirectMode = FALSE;
    DscPhoto_RegChgDispSizeInforCB(PipView_ActivePrimaryImgRatio);
    #endif

    //#NT#2014/10/30#KS Hung -begin
    if (IsRotatePanel == TRUE)
    {
        if (System_IsPlugTVHDMI() == TRUE)
        {
            IsRotatePanel = FALSE; // plug in TV or HDMI and current display object is not a rotate panel.
            #if !PIP_VIEW_FUNC
            gMovie_src.bDirectMode = TRUE;
            #endif
        }
        else
        {
            gMovie_src.bDirectMode = FALSE;
        }
    }
    //#NT#2014/10/30#KS Hung -end

    AppView_ConfigSource(&gMovie_src);
    //open
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
        DISPSRV_FP_DRAW_CB fpOnDraw = MovieExe_OnDraw;
        Cmd.Idx = DISPSRV_CMD_IDX_SET_DRAW_CB;
        Cmd.In.pData = &fpOnDraw;
        Cmd.In.uiNumByte = sizeof(fpOnDraw);
        Cmd.Prop.bExitCmdFinish = TRUE;
        DispSrv_Cmd(&Cmd);
    }
    #endif
    #endif
    Movie_SetRecParam();

    // DIS can only be enabled as RSC off
    if (UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_ON &&
        UI_GetData(FL_MovieRSCIndex) == MOVIE_RSC_OFF)
    {
        IPL_AlgSetUIInfo(IPL_SEL_DISCOMP, SEL_DISCOMP_ON);
    }
    else
    {
        UI_SetData(FL_MovieDisIndex, MOVIE_DIS_OFF);
        IPL_AlgSetUIInfo(IPL_SEL_DISCOMP, SEL_DISCOMP_OFF);
    }

    //#NT#2013/02/27#Lincy Lin -begin
    //#NT#For capture in movie write file naming

    #if USE_FILEDB
    UI_SetData(FL_IsUseFileDB, 1);
    #else
    UI_SetData(FL_IsUseFileDB, 0);
    #endif
    useFileDB = UI_GetData(FL_IsUseFileDB);
    if (useFileDB)
    {

        //DscPhoto_Config(PHOTO_CFG_USE_FILEDB, 1);
        //Pool.Addr = OS_GetMempoolAddr(POOL_ID_FILEDB);
        //Pool.Size = POOL_SIZE_FILEDB;
        //DscPhoto_Config(PHOTO_CFG_FILEDB_MEM, (UINT32)&Pool);

        DscMovie_Config(MOVIE_CFG_USE_FILEDB, 1);
        Pool.Addr = OS_GetMempoolAddr(POOL_ID_FILEDB);
        Pool.Size = POOL_SIZE_FILEDB;
        DscMovie_Config(MOVIE_CFG_FILEDB_MEM, (UINT32)&Pool);
        DscPhoto_Config(PHOTO_CFG_USE_FILEDB, 1);

#if (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))

        // use Custom Naming Rule (to use different folder name for different path) for small size clone movie function

        // enable custom naming
        DscMovie_Config(MOVIE_CFG_CUSTOM_NAMING, ENABLE);

        // set root folder
        DscMovie_SetRootFolder(DV_ROOT_PATH);

        // set folder name of 1st movie path (main movie)
        DscMovie_SetMovieFolder(0, MOVIE_FOLDER);
        DscMovie_SetROFolder(0, MOVIE_RO_FOLDER);
        DscMovie_SetPhotoFolder(0, PHOTO_FOLDER);

        // set folder name of 2nd movie path (small size clone movie)
        DscMovie_SetMovieFolder(1, MOVIE_SHARE_FOLDER);
        DscMovie_SetROFolder(1, MOVIE_SHARE_RO_FOLDER);
        DscMovie_SetPhotoFolder(1, PHOTO_FOLDER);

        // set custom file name
        #if 0
        switch(SysGetFlag(FL_DATE_FORMAT))
        {
        case DATE_FORMAT_DMY:
            DscMovie_SetFileNamingYMD(FILE_NAMING_DATEFMT_DMY);
            break;
        case DATE_FORMAT_MDY:
            DscMovie_SetFileNamingYMD(FILE_NAMING_DATEFMT_MDY);
            break;
        case DATE_FORMAT_YMD:
        default:
            DscMovie_SetFileNamingYMD(FILE_NAMING_DATEFMT_YMD);
            break;
        }
        DscMovie_SetFileNamingMethod(FILE_NAMING_SEMIAUTO);
        DscMovie_SetFileNamingType(FILE_NAMING_SEMIAUTO_TYPE2);
        DscMovie_SetFileNameEndChar(0, DISABLE, NULL);
        DscMovie_SetFileNameEndChar(1, DISABLE, NULL);
        DscMovie_SetFileNameSerialNum(0, DISABLE);
        DscMovie_SetFileNameSerialNum(1, DISABLE);
        #else // manual naming method
        DscMovie_SetFileNamingMethod(FILE_NAMING_MANUAL);
        DscMovie_RegMovieFileNamingCB(UIMovie_MovieFileNamingCB);
        DscMovie_RegPhotoFileNamingCB(UIMovie_PhotoFileNamingCB);
        DscMovie_RegEMRMovFileNamingCB(UIMovie_EMRMovFileNamingCB);
        #endif

		// Always add file to last
		NH_FileDB_Set_AlwaysAdd2Last(1);

#else

	#if 1 // sample code for FileDB Naming Rule
        DscMovie_SetFDBFolder(FILEDB_CARDV_ROOT);
        DscMovie_SetFDB_MovieFolder("MOVIE");
        DscMovie_SetFDB_ROFolder("EMR");
        DscPhoto_SetFDBFolder(FILEDB_CARDV_ROOT);
        DscPhoto_SetFDBPhotoFolder("PHOTO");

		#if (YOUKU_SDK == ENABLE)
        char RootFolder[NMC_OTHERS_MAX_LEN];
        snprintf(RootFolder,NMC_OTHERS_MAX_LEN,"A:\\%s\\",FILEDB_CARDV_ROOT);
        DscMovie_SetRootFolder(RootFolder);
        DscMovie_Config(MOVIE_CFG_CUSTOM_NAMING, ENABLE);
        DscMovie_SetMovieFolder(1, WIFI_MOVIE_FOLDER);
        DscMovie_SetFileNamingMethod(FILE_NAMING_MANUAL);
        DscMovie_RegMovieFileNamingCB(UIMovie_MovieFileNamingCB);
        DscMovie_RegPhotoFileNamingCB(UIMovie_PhotoFileNamingCB);
        DscMovie_RegEMRMovFileNamingCB(UIMovie_EMRMovFileNamingCB);
        DscMovie_SetFileNameEndChar(0, DISABLE, NULL);
        DscMovie_SetFileNameEndChar(1, DISABLE, NULL);
        DscMovie_SetFileNameSerialNum(0, DISABLE);
        DscMovie_SetFileNameSerialNum(1, DISABLE);
		#endif
        //sample
        //DscMovie_SetFDBFolder("CARDV");
        //DscMovie_SetFDB_MovieFolder("MOVIE");
        //DscMovie_SetFDB_ROFolder("MOVIE\\RO");
        //set photo to RawEncTsk
        //DscPhoto_SetFDBFolder("CARDV");
        //DscPhoto_SetFDBPhotoFolder("PHOTO");
	#else // sample code for Custom Naming Rule
        // enable custom naming
        DscMovie_Config(MOVIE_CFG_CUSTOM_NAMING, ENABLE);

        // set root folder
        DscMovie_SetRootFolder("A:\\CarDV\\");

        // set folder name of 1st movie path
        DscMovie_SetMovieFolder(0, "Movie_Front");
        DscMovie_SetROFolder(0, "RO_Front");
        DscMovie_SetPhotoFolder(0, "Photo_Front");

        // set folder name of 2nd movie path
        DscMovie_SetMovieFolder(1, "Movie_Rear");
        DscMovie_SetROFolder(1, "RO_Rear");
        DscMovie_SetPhotoFolder(1, "Photo_Rear");

        // set custom file name
        #if 0 // manual naming method
        DscMovie_SetFileNamingMethod(FILE_NAMING_MANUAL);
        DscMovie_RegMovieFileNamingCB(UIMovie_MovieFileNamingCB);
        DscMovie_RegPhotoFileNamingCB(UIMovie_PhotoFileNamingCB);
        #elif 1 // semi-auto naming method
        switch(SysGetFlag(FL_DATE_FORMAT))
        {
        case DATE_FORMAT_DMY:
            DscMovie_SetFileNamingYMD(FILE_NAMING_DATEFMT_DMY);
            break;
        case DATE_FORMAT_MDY:
            DscMovie_SetFileNamingYMD(FILE_NAMING_DATEFMT_MDY);
            break;
        case DATE_FORMAT_YMD:
        default:
            DscMovie_SetFileNamingYMD(FILE_NAMING_DATEFMT_YMD);
            break;
        }
        DscMovie_SetFileNamingMethod(FILE_NAMING_SEMIAUTO);
        DscMovie_SetFileNamingType(FILE_NAMING_SEMIAUTO_TYPE2);
        DscMovie_SetFileNameEndChar(0, ENABLE, "A");
        DscMovie_SetFileNameEndChar(1, ENABLE, "B");
        DscMovie_SetFileNameSerialNum(0, DISABLE);
        DscMovie_SetFileNameSerialNum(1, DISABLE);
        #else // auto naming method => need to do nothing
        #endif
	#endif

#endif
    }
    else
    {
        DscMovie_Config(MOVIE_CFG_USE_FILEDB, 0);
        DscPhoto_Config(PHOTO_CFG_USE_FILEDB, 0);
    }
    //#NT#2013/02/27#Lincy Lin -end

    AF_RegisterCB(UIMovie_AFCB);                    // register AF callback function
    AFTsk_RegisterCAF((AF_PROC_CAF)caf_AutoFocusProcess);   // register CAF function
    DscMovie_RegDrawCB(MovieStamp_CopyData);        // register movie stamp callback function
    DscMovie_RegAudFilterCB(UIMovie_AudFilterProc); // register audio filtering callback function
    DscMovie_RegImmProcCB(UIMovie_ImmProc);         // register instant events processing callback function
    DscMovie_Set_MEDIAREC_VER(MEDIAREC_VER);
    DscMovie_Open();

    #if (UI_STYLE==UI_STYLE_DRIVER)
    #if(_GYRO_EXT_==_GYRO_EXT_NONE_)
    IPL_AlgSetUIInfo(IPL_SEL_3DNR, SEL_3DNR_ON);
    #else
    IPL_AlgSetUIInfo(IPL_SEL_3DNR, SEL_3DNR_OFF);
    //C H Lin - RSC_Modification: FOV flow
    if (SysGetFlag(FL_MovieRSCIndex)==MOVIE_RSC_ON)
        IPL_AlgSetUIInfo(IPL_SEL_RSC, SEL_RSC_ON);
    #endif
    //IPL_AlgSetUIInfo(IPL_SEL_IMAGEEFFECT, SEL_IMAGEEFFECT_WDR);
    IPL_AlgSetUIInfo(IPL_SEL_WDR, SEL_WDR_LV6);
    #else
    // disable IME 3DNR since it will conflict with RSC
    IPL_AlgSetUIInfo(IPL_SEL_3DNR, SEL_3DNR_OFF);
    #endif

#if(WIFI_AP_FUNC==ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        static BOOL bAutoRecordEn = FALSE;
        if (!bAutoRecordEn && UI_GetData(FL_WIFI_AUTO_RECORDING) == WIFI_AUTO_RECORDING_ON)
        {
            UINT32 MaxTime = UIFlowWiFiMovie_GetMaxRecTime();

            if((MaxTime<=2) && (SysGetFlag(FL_MOVIE_CYCLIC_REC) == MOVIE_CYCLICREC_OFF))
            {
                UI_SetData(FL_WIFI_MOVIE_FMT,WIFI_RTSP_LIVEVIEW);
                DBG_WRN("recTime<2 sec\r\n");
            }
            else if(MaxTime == 0)
            {
                UI_SetData(FL_WIFI_MOVIE_FMT,WIFI_RTSP_LIVEVIEW);
                DBG_WRN("recTime = 0 sec\r\n");
            }
            else
            {
                UI_SetData(FL_WIFI_MOVIE_FMT,WIFI_RTSP_REC);
                #if ((MOVIE_LIVEVIEW==HTTP_LIVEVIEW) ||(MOVIE_LIVEVIEW==DUAL_REC_HTTP_LIVEVIEW))
                Ux_PostEvent(NVTEVT_EXE_MOVIE_REC_START, 0);
                #endif
            }
            bAutoRecordEn = TRUE;

        }
        else
        {
            UI_SetData(FL_WIFI_MOVIE_FMT,WIFI_RTSP_LIVEVIEW);
        }
        #if(MOVIE_LIVEVIEW==RTSP_LIVEVIEW)
        Ux_SendEvent(&UIMovieRecObjCtrl, NVTEVT_START_RTSP, 0);
        Ux_PostEvent(NVTEVT_EXE_MOVIE_REC_START, 0);
        #else //((MOVIE_LIVEVIEW==HTTP_LIVEVIEW) ||(MOVIE_LIVEVIEW==DUAL_REC_HTTP_LIVEVIEW))
        PhotoExe_OpenHttpLiveView();
        UIAppHttp_Open();
        WifiCmd_Done(WIFIFLAG_MODE_DONE,E_OK);
        #endif
    }
#endif

#if (0)//(GSENSOR_FUNCTION == ENABLE)
    GSensor_open();
    SxTimer_SetFuncActive(SX_TIMER_DET_GSENSOR_ID, TRUE);
#endif

    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (0)//(GSENSOR_FUNCTION == ENABLE)
    SxTimer_SetFuncActive(SX_TIMER_DET_GSENSOR_ID, FALSE);
#endif

    #if MOVIE_FD_FUNC_
    // unlock FD
    FD_Lock(TRUE);
    FD_UnInit();
    //Flush FD event
    Ux_FlushEventByRange(NVTEVT_EXE_MOVIE_FDEND,NVTEVT_EXE_MOVIE_FDEND);
    #endif
#if(WIFI_AP_FUNC==ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        #if(MOVIE_LIVEVIEW==RTSP_LIVEVIEW)
        Ux_SendEvent(&UIMovieRecObjCtrl, NVTEVT_STOP_RTSP, 0);
        #else //((MOVIE_LIVEVIEW==HTTP_LIVEVIEW) ||(MOVIE_LIVEVIEW==DUAL_REC_HTTP_LIVEVIEW))
        UIAppHttp_Close();
        PhotoExe_CloseHttpLiveView();
        AppView_ConfigEnable(1, FALSE);
        #endif
    }
#endif
    DscMovie_Close();
    AppView_Close();
    {
      DISPSRV_CMD DispCmd= {0};

      memset(&DispCmd,0,sizeof(DISPSRV_CMD));
      //show last frame of live view
      DispCmd.Idx = DISPSRV_CMD_IDX_DIRECT_FRM_TO_TEMP;
      DispCmd.Prop.bExitCmdFinish=TRUE;
      DispSrv_Cmd(&DispCmd);
      //SwTimer_DelayMs(5000);
    }
    Sensor_Close(SENSOR_ID_1);
#if  (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
    if (g_bSensor2En)
    {
        Sensor_Close(SENSOR_ID_2);
        g_bSensor2En = FALSE;
    }
#endif

#if (GSENSOR_FUNCTION == ENABLE)
    //GSensor_close();
#endif

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnMovieSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    UINT32 uiQuality;
    UINT32 uiFrameRate;

    if(paramNum > 0)
        uiSelect = paramArray[0];

    uiQuality = UI_GetData(FL_MOVIE_QUALITY);
    if (UI_GetData(FL_MOVIE_DUAL_REC))
    {
        UI_SetData(FL_MOVIE_SIZE_DUAL, uiSelect);
        uiFrameRate = GetMovieFrameRate_2p(0, uiSelect);
    }
    else
    {
        UI_SetData(FL_MOVIE_SIZE, uiSelect);
        uiFrameRate = GetMovieFrameRate(uiSelect);
    }

    #if 0
    // set media parameters => unnecessary, since the parameters will be set in Movie_SetRecParam
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_WIDTH, GetMovieSizeWidth(uiSelect), 0, 0);
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_HEIGHT, GetMovieSizeHeight(uiSelect), 0, 0);
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_FRAMERATE, uiFrameRate, 0, 0);
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_TARGETRATE, GetMovieTargetBitrate(uiSelect, uiQuality), 0, 0);
    #else
    // need to set media record parameters in order to calculate remaining time
    Movie_SetRecParam();
    #endif

    // enable RSC if necessary (only support frame rate smaller than 30fps)
    #if(_GYRO_EXT_!=_GYRO_EXT_NONE_)
    if ((UI_GetData(FL_MovieRSCIndex) == MOVIE_RSC_ON) && (uiFrameRate <= 30))
    {
        DscMovie_Config(MOVIE_CFG_RSC, SEL_RSC_ON);
    }
    else
    {
        DscMovie_Config(MOVIE_CFG_RSC, SEL_RSC_OFF);
    }
    #endif

    // set IPL parameters
    if (uiSelect == MOVIE_SIZE_1080P_FULLRES) // for full resolution input 1080p30
    {
        IPL_AlgSetUIInfo(IPL_SEL_VIDEO_FPS,SEL_VIDEOFPS_FULLRES);
    }
    else
    {
        switch (uiFrameRate)
        {
        case 60: // 60fps
            IPL_AlgSetUIInfo(IPL_SEL_VIDEO_FPS,SEL_VIDEOFPS_60);
            break;

        case 120: // 120fps
            IPL_AlgSetUIInfo(IPL_SEL_VIDEO_FPS,SEL_VIDEOFPS_120);
            break;

        default: // 30fps
            IPL_AlgSetUIInfo(IPL_SEL_VIDEO_FPS,SEL_VIDEOFPS_30);
            break;
        }
    }

    return NVTEVT_CONSUME;
}
#if MOVIE_FD_FUNC_

static USIZE MovieExe_RatioSizeConvert(USIZE* devSize, USIZE* devRatio, USIZE* Imgratio)
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

static UINT32 GetMovieSizeRatio(void)
{
    switch (UI_GetData(FL_MOVIE_SIZE))
    {
        case MOVIE_SIZE_VGA:
        case MOVIE_SIZE_QVGA:
            return IMAGERATIO_4_3;

        case MOVIE_SIZE_1080P_FULLRES:
        case MOVIE_SIZE_1080P:
        case MOVIE_SIZE_720P:
        //case MOVIE_SIZE_WVGA:
        default:
            return IMAGERATIO_16_9;
    }
}
static void MovieExe_GetFDDispCord(URECT *dispCord)
{
    UINT32 ImageRatioIdx = 0;
    USIZE  ImageRatioSize={0};
    URECT  fdDispCoord;
    ISIZE  dev1size;
    ISIZE  dev1Ratio;
    USIZE  finalSize={0};
    UINT32 CurrDevObj;

    ImageRatioIdx = GetMovieSizeRatio();
    ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];

    //1.get current device size (current mode)
    dev1size = GxVideo_GetDeviceSize(DOUT1);
    //2.get current device aspect Ratio
    CurrDevObj = GxVideo_GetDevice(DOUT1);
    dev1Ratio = _QueryDeviceModeAspect(CurrDevObj, GxVideo_QueryDeviceLastMode(CurrDevObj));
    finalSize = MovieExe_RatioSizeConvert((USIZE *)&dev1size,(USIZE *)&dev1Ratio,&ImageRatioSize);
    fdDispCoord.w = finalSize.w;
    fdDispCoord.h = finalSize.h;
    if (finalSize.w == (UINT32)dev1size.w)
    {
        fdDispCoord.x = 0;
        fdDispCoord.y = (dev1size.h - finalSize.h)>>1;
    }
    else
    {
        fdDispCoord.y = 0;
        fdDispCoord.x = (dev1size.w - finalSize.w)>>1;

    }
    *dispCord = fdDispCoord;
}
#endif
//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnImageRatio(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    IPL_SENCROPRATIO CropRatio;
    UINT32  uiSelect = 0;
    UINT32  CurrDevObj;
    ISIZE   DispSize = {0};
    ISIZE   ImgRatio = {0};
    ISIZE   DeviceSize = {0};
    ISIZE   DeviceRatio = {0};

    if (paramNum > 0)
        uiSelect = paramArray[0]; // parameter 0: image size

    #if  (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
    {
        switch (uiSelect)
        {
        case MOVIE_SIZE_1080P_720P:
        case MOVIE_SIZE_720P_720P:
        default:
            CropRatio = IPL_SENCROPRATIO_16_9;
            ImgRatio.w = 16;
            ImgRatio.h = 9;
            break;
        }
    }
    #else
    {
        switch (uiSelect)
        {
        case MOVIE_SIZE_VGA:
        case MOVIE_SIZE_QVGA:
            CropRatio = IPL_SENCROPRATIO_4_3;
            ImgRatio.w = 4;
            ImgRatio.h = 3;
            break;

        case MOVIE_SIZE_1080P_FULLRES:
        case MOVIE_SIZE_1080P:
        case MOVIE_SIZE_720P:
        //case MOVIE_SIZE_WVGA:
        default:
            CropRatio = IPL_SENCROPRATIO_16_9;
            ImgRatio.w = 16;
            ImgRatio.h = 9;
            break;
        }
    }
    #endif
    DscMovie_Config(MOVIE_CFG_RATIO, CropRatio);
    #if MOVIE_FD_FUNC_
    {
        MovieExe_GetFDDispCord(&gMovieFdDispCord);
    }
    #endif

#if (MOVIE_TEST_ENABLE != ENABLE)
    CurrDevObj = GxVideo_GetDevice(DOUT1);
    DeviceSize = GxVideo_GetDeviceSize(DOUT1);
    DeviceRatio = _QueryDeviceModeAspect(CurrDevObj, GxVideo_QueryDeviceLastMode(CurrDevObj));
    MovieExe_CalcDispSize(&DispSize, &DeviceSize, &DeviceRatio, &ImgRatio);
    AppView_ConfigImgSize(0, DispSize.w, DispSize.h, ImgRatio.w, ImgRatio.h);
    #if PIP_VIEW_FUNC
    PipView_SetPrimaryImgRatio((USIZE*)&ImgRatio);
    #endif

    //2. Set IPL setting
    g_ImeInfo.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_IME_INFOR, (void *)&g_ImeInfo);

    g_ImgRatio.Id = IPL_ID_1;
    g_ImgRatio.PathId = IPL_IME_PATH_NONE;
    g_ImgRatio.PathId |= IPL_IME_PATH2;
    g_ImgRatio.CropRatio = CropRatio;
    g_ImgRatio.ImeP2.OutputEn = TRUE;
    g_ImgRatio.ImeP2.ImgSizeH = DispSize.w;
    g_ImgRatio.ImeP2.ImgSizeV = DispSize.h;
    g_ImgRatio.ImeP2.ImgSizeLOfs = DispSize.w;
    g_ImgRatio.ImeP2.ImgFmt = g_ImeInfo.ImeP2[0].type;

    CurrDevObj = GxVideo_GetDevice(DOUT2);
    if (CurrDevObj)
    {
        DeviceSize = GxVideo_GetDeviceSize(DOUT2);
        DeviceRatio = _QueryDeviceModeAspect(CurrDevObj, GxVideo_QueryDeviceLastMode(CurrDevObj));
        MovieExe_CalcDispSize(&DispSize, &DeviceSize, &DeviceRatio, &ImgRatio);
        AppView_ConfigImgSize(1, DispSize.w, DispSize.h, ImgRatio.w, ImgRatio.h);

        g_ImgRatio.PathId |= IPL_IME_PATH3;
        g_ImgRatio.ImeP3.OutputEn = TRUE;
        g_ImgRatio.ImeP3.ImgSizeH = DispSize.w;
        g_ImgRatio.ImeP3.ImgSizeV = DispSize.h;
        g_ImgRatio.ImeP3.ImgSizeLOfs = DispSize.w;
        g_ImgRatio.ImeP3.ImgFmt = g_ImeInfo.ImeP3[0].type;
    }
#if(WIFI_AP_FUNC==ENABLE && MOVIE_LIVEVIEW==HTTP_LIVEVIEW)
    {
        UINT32 w,h;
        if(CropRatio==IPL_SENCROPRATIO_16_9)
        {
            w = HTTP_MJPG_W_16;
            h = HTTP_MJPG_H_9;
        }
        else
        {
            w = HTTP_MJPG_W_4;
            h = HTTP_MJPG_H_3;
        }
        AppView_ConfigImgSize(1, w, h, ImgRatio.w, ImgRatio.h);

        g_ImgRatio.PathId |= IPL_IME_PATH3;
        g_ImgRatio.ImeP3.OutputEn = TRUE;
        g_ImgRatio.ImeP3.ImgSizeH = w;
        g_ImgRatio.ImeP3.ImgSizeV = h;
        g_ImgRatio.ImeP3.ImgSizeLOfs = w;
        g_ImgRatio.ImeP3.ImgFmt = g_ImeInfo.ImeP3[0].type;
    }
#endif
#if 0
    debug_msg("OutputEn      %d \r\n",g_ImgRatio.ImeP3.OutputEn  );
    debug_msg("ImgSizeH      %d \r\n",g_ImgRatio.ImeP3.ImgSizeH );
    debug_msg("ImgSizeV      %d \r\n",g_ImgRatio.ImeP3.ImgSizeV );
    debug_msg("ImgSizeLOfs   %d \r\n",g_ImgRatio.ImeP3.ImgSizeLOfs );
    debug_msg("ImgFmt        %d \r\n",g_ImgRatio.ImeP3.ImgFmt );
#endif

    IPL_SetCmd(IPL_SET_IMGRATIO, (void *)&g_ImgRatio);
    IPL_WaitCmdFinish();


#endif

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnMovieQuality(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiQuality = 0;
    UINT32 uiSize;

    if(paramNum > 0)
        uiQuality = paramArray[0];

    UI_SetData(FL_MOVIE_QUALITY, uiQuality);
    uiSize = UI_GetData(FL_MOVIE_SIZE);

    MovRec_ChangeParameter(MOVREC_RECPARAM_TARGETRATE, GetMovieTargetBitrate(uiSize, uiQuality), 0, 0);

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnWB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_WB, uiSelect);
    IPL_AlgSetUIInfo(IPL_SEL_WBMODE, Get_WBValue(uiSelect));

    return NVTEVT_CONSUME;
}

//#NT#2011/04/15#Photon Lin -begin
//#NT#Add AV sync mechanism
INT32 MovieExe_OnAVSyncCnt(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum > 0)
        uhSelect = paramArray[0];
#if _DEMO_TODO
    DbgMsg_UIMovExeIO(("+-MovieExe_OnAVSyncCnt:idx=%d,val=%d\r\n",uhSelect,GetMovieQualityValue(uhSelect)));

    UIMovRecObj_SetData(RECMOVIE_AYSYNCCNT, uhSelect);
#endif
    return NVTEVT_CONSUME;
}
//#NT#2011/04/15#Photon Lin -end

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnColor(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_COLOR_EFFECT, uiSelect);
    IPL_AlgSetUIInfo(IPL_SEL_IMAGEEFFECT, Get_ColorValue(uiSelect));

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnEV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_EV, uiSelect);
    IPL_AlgSetUIInfo(IPL_SEL_AEEV, Get_EVValue(uiSelect));

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnMovieAudio(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_AUDIO, uiSelect);

    if (uiSelect == MOVIE_AUDIO_OFF)
    {
        UIMovRecObj_SetData(RECMOVIE_AUD_VOLUME, RECMOVIE_AUD_OFF);
    }
    else
    {
        UIMovRecObj_SetData(RECMOVIE_AUD_VOLUME, RECMOVIE_AUD_ON);
    }

    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnAudPlayVolume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum > 0)
        uhSelect = paramArray[0];
    DbgMsg_UIMovExeIO(("+-MovieExe_OnAudPlayVolume:idx=%d,val=%d\r\n",uhSelect,GetMovieAudioVolumeValue(uhSelect)));
    //if (UI_GetData(FL_MovieAudioPlayIndex) != uhSelect)
    {
        UI_SetData(FL_MovieAudioPlayIndex,uhSelect);
        //#NT#2012/09/19#Calvin Chang#Porting Media Play Demo1 Flow -begin
        UIMovObj_SetAudPlayVolume(GetMovieAudioVolumeValue(uhSelect));
//#if _DEMO_TODO
//        UIMovObj_SetDataPlayMovie(PLAYMOVIE_AUD_VOLUME, GetMovieAudioVolumeValue(uhSelect));
//#endif
        //#NT#2012/09/19#Calvin Chang -end
    }
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnAudRecVolume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum > 0)
        uhSelect = paramArray[0];
    DbgMsg_UIMovExeIO(("+-MovieExe_OnAudRecVolume:idx=%d\r\n",uhSelect));
    if (UI_GetData(FL_MovieAudioRecIndex) != uhSelect)
    {
        UI_SetData(FL_MovieAudioRecIndex,uhSelect);
    }
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnDigitalZoom(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];
    DbgMsg_UIMovExeIO(("+-MovieExe_OnDigitalZoom:idx=%d\r\n",uhSelect));
    UI_SetData(FL_Dzoom, uhSelect);
    if (uhSelect == DZOOM_OFF)
    {
        UI_SetData(FL_DzoomIndex, DZOOM_10X);
    }
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnContAF(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_MovieContAFIndex,uhSelect);
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnMetering(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_MovieMeteringIndex,uhSelect);
#if _DEMO_TODO
    //set to photo obj
    AppPhoto_SetData(&UIPhotoObjCtrl,_AEMode,Get_MeteringValue(uhSelect));
#endif
    IPL_AlgSetUIInfo(IPL_SEL_AEMODE,Get_MeteringValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnDis(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
    {
        uiSelect = paramArray[0];
    }

    DBG_IND("idx=%d\r\n", uiSelect);

    UI_SetData(FL_MovieDisIndex, uiSelect);

    if (UI_GetData(FL_ModeIndex) == DSC_MODE_MOVIE)
    {
        if (uiSelect == MOVIE_DIS_ON)
        {
            // disable RSC
            UI_SetData(FL_MovieRSCIndex, MOVIE_RSC_OFF);
            DscMovie_Config(MOVIE_CFG_RSC, SEL_RSC_OFF);
            IPL_AlgSetUIInfo(IPL_SEL_RSC, SEL_RSC_OFF);

            // enable DIS
            IPL_AlgSetUIInfo(IPL_SEL_DISCOMP, SEL_DISCOMP_ON);
        }
        else
        {
            IPL_AlgSetUIInfo(IPL_SEL_DISCOMP, SEL_DISCOMP_OFF);
        }
    }
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnMCTF(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MovieMCTFIndex, uiSelect);

    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnRSC(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    UINT32 uiFrameRate;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MovieRSCIndex, uiSelect);

    if (UI_GetData(FL_MOVIE_DUAL_REC))
    {
        uiFrameRate = GetMovieFrameRate_2p(0, UI_GetData(FL_MOVIE_SIZE));
    }
    else
    {
        uiFrameRate = GetMovieFrameRate(UI_GetData(FL_MOVIE_SIZE));
    }

    if (uiSelect == MOVIE_RSC_ON)
    {
        // disable DIS
        UI_SetData(FL_MovieDisIndex, MOVIE_DIS_OFF);
        IPL_AlgSetUIInfo(IPL_SEL_DISCOMP, SEL_DISCOMP_OFF);

        // enable RSC => move to IPL mode change to VIDEOREC
        //IPL_AlgSetUIInfo(IPL_SEL_RSC, SEL_RSC_ON);

        // enable RSC if necessary (only support frame rate smaller than 30fps)
        if ((UI_GetData(FL_MovieRSCIndex) == MOVIE_RSC_ON) && (uiFrameRate <= 30))
        {
            DscMovie_Config(MOVIE_CFG_RSC, SEL_RSC_ON);
        }
        else
        {
            DscMovie_Config(MOVIE_CFG_RSC, SEL_RSC_OFF);
        }
    }
    else
    {
        DscMovie_Config(MOVIE_CFG_RSC, SEL_RSC_OFF);
        IPL_AlgSetUIInfo(IPL_SEL_RSC, SEL_RSC_OFF);
    }

    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnHDR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    if (uiSelect == MOVIE_HDR_ON)
    {
        UI_SetData(FL_MOVIE_HDR, MOVIE_HDR_ON);
        IPL_AlgSetUIInfo(IPL_SEL_IMAGEEFFECT, SEL_IMAGEEFFECT_WDR);
    }
    else
    {
        UI_SetData(FL_MOVIE_HDR, MOVIE_HDR_OFF);
        IPL_AlgSetUIInfo(IPL_SEL_IMAGEEFFECT, SEL_IMAGEEFFECT_OFF);
    }

    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnGdc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];
    DbgMsg_UIMovExeIO(("+-MovieExe_OnGdc:idx=%d\r\n",uhSelect));

    UI_SetData(FL_MovieGdcIndex,uhSelect);
#if(MOVIE_GDC_FUNC ==ENABLE)
    //#NT#2010/09/02#JeahYen -begin
    AppPhoto_SetData(&UIPhotoObjCtrl, _Prv2DGDC,
        (uhSelect==MOVIE_GDC_ON)?_Prv_2D_GDC_On:_Prv_2D_GDC_Off);
    //#NT#2010/09/02#JeahYen -end
#endif
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnSmear(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];
    DbgMsg_UIMovExeIO(("+-MovieExe_OnSmear:idx=%d\r\n",uhSelect));

    UI_SetData(FL_MovieSmearIndex,uhSelect);
#if(MOVIE_SMEAR_R_FUNC ==ENABLE)
    AppPhoto_SetData(&UIPhotoObjCtrl, _SmearModeEn,
        (uhSelect==MOVIE_SMEAR_ON)?_SmearMode_on:_SmearMode_off);
#endif
    return NVTEVT_CONSUME;
}

//#NT#2010/04/14#Photon Lin -begin
//#Add DIS event
/*
1. Turn ON movie DIS: after IPL is ready, call
      PhotoDisplay_SetMode(DISPLAY_MODE_DISVIEW, TRUE);
      PhotoDis_setMode(PHOTODIS_MODE_START, TRUE);

2. Turn OFF movie DIS: before switching IPL off, call
      PhotoDis_setMode(PHOTODIS_MODE_IDLE, TRUE);
      PhotoDisplay_SetMode(DISPLAY_MODE_VIDEO, TRUE);
*/
INT32 MovieExe_OnDisEnable(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0xFFFFFFFF;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DbgMsg_UIMovExeIO(("+-MovieExe_OnDisEnable:idx=%d\r\n",uhSelect));
    //if (UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_ON)
    {
        //#NT#2010/12/16#Photon Lin -begin
        //#Enhance DIS flow
        UI_SetData(FL_MovieDisEnableIndex,uhSelect);
#if _DEMO_TODO
        //#NT#2010/12/16#Photon Lin -end
        if (uhSelect == 1)
        {
            //#NT#2011/04/15#Photon Lin -begin
            //#NT#Add AV sync mechanism
            if (UI_GetData(FL_MovieGdcIndex)==1)
            {
                Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIEAVSYNC, 1, 4);
            }
            else
            {
                Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIEAVSYNC, 1, 3);
            }
            //#NT#2011/04/15#Photon Lin -end
            //#NT#2010/12/15#Jay -begin
//            PhotoDisplay_SetMode(DISPLAY_MODE_DISVIEW, TRUE);
//            PhotoDis_setMode(PHOTODIS_MODE_START, TRUE);
            PhotoDisplay_SetMode(DISPLAY_MODE_DISVIEW, FALSE);
            PhotoDis_setMode(PHOTODIS_MODE_START, FALSE);
            //#NT#2010/12/15#Jay -end

        }
        else if (uhSelect == 0)
        {
            //#NT#2011/04/15#Photon Lin -begin
            //#NT#Add AV sync mechanism
            if (UI_GetData(FL_MovieGdcIndex)==1)
            {
                Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIEAVSYNC, 1, 3);
            }
            else
            {
                Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIEAVSYNC, 1, 2);
            }
            //#NT#2011/04/15#Photon Lin -end
            idec_setV1En(IDE_ID_1, FALSE);
            PhotoDis_setMode(PHOTODIS_MODE_IDLE, TRUE);
            PhotoDisplay_SetMode(DISPLAY_MODE_VIDEO, TRUE);
        }
#endif
    }
    return NVTEVT_CONSUME;
}
//#NT#2010/04/14#Photon Lin -end

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnDateImprint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_DATEIMPRINT, uiSelect);

    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnGSENSOR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    UINT32 uiThres = 0;
    GSENSOR_OBJ *pGS_Obj = NULL;
    pGS_Obj = Gsensor_GetOptObj();
    if (paramNum)
    uiSelect = paramArray[0];
	
	 UI_SetData(FL_GSENSOR, uiSelect);
    if (pGS_Obj != NULL)
    {
        switch(uiSelect)
        {
            case GSENSOR_OFF:
                uiThres = GSENSOR_SENSITIVITY_OFF;
            break;
            case GSENSOR_LOW:
                uiThres = GSENSOR_SENSITIVITY_LOW;
            break;
            case GSENSOR_MED:
                uiThres = GSENSOR_SENSITIVITY_MED;
            break;
            case GSENSOR_HIGH:
                uiThres = GSENSOR_SENSITIVITY_HIGH;
            break;
            default:
                uiThres = GSENSOR_SENSITIVITY_OFF;
            break;
        }
        pGS_Obj->setThresHold(uiThres);
    }


    //debug_err(("MovieExe_OnGSENSOR =%d\r\n ",FL_GSENSOR));
    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnDualRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_DUAL_REC, uiSelect);

    // TO DO: set IPL
#if (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
    // forced set dual recording to FALSE if no 2nd sensor
    if (Sensor_CheckExtSensor() == FALSE)
    {
        UI_SetData(FL_MOVIE_DUAL_REC, FALSE);
    }
    else
    {
        if (uiSelect) // enable dual recording
        {
            IPL_Display_OpenTsk2();
            IPL_ResetDisplayCtrlFlow2();
        }
        else // disable dual recording
        {
            IPL_Display_CloseTsk2();
        }
    }
#endif

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnCyclicRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    UINT32 uiCyclicRecTime = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_CYCLIC_REC, uiSelect);

    switch (uiSelect)
    {
	case MOVIE_CYCLICREC_1MIN:
		uiCyclicRecTime = 60;
		break;

    case MOVIE_CYCLICREC_3MIN:
        uiCyclicRecTime = 180;
        break;

    case MOVIE_CYCLICREC_5MIN:
        uiCyclicRecTime = 300;
        break;

    case MOVIE_CYCLICREC_10MIN:
        uiCyclicRecTime = 600;
        break;

    case MOVIE_CYCLICREC_OFF:
        break;

    default:
        uiCyclicRecTime = 300;
        break;
    }

    if (uiSelect != MOVIE_CYCLICREC_OFF)
    {
        MovRec_ChangeParameter(MOVREC_RECPARAM_CUTSEC, uiCyclicRecTime, 0, 0);
    }
    else
    {
        MovRec_ChangeParameter(MOVREC_RECPARAM_CUTSEC, 1500, 0, 0); // 25 min
        MovRec_ChangeParameter(MOVREC_RECPARAM_ENDTYPE, MOVREC_ENDTYPE_CUT_TILLCARDFULL, 0, 0);
    }

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnMotionDet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_MOTION_DET, uiSelect);

    return NVTEVT_CONSUME;
}

//#NT#2010/11/25#Photon Lin -begin
//#Add feature of movie key
INT32 MovieExe_OnAutoRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhParam = 0xFFFFFFFF;
    UINT32 uhLastMode = 0xFFFFFFFF;
    if(paramNum>0)
    {
        uhParam = paramArray[0];
        uhLastMode = paramArray[1];
        DbgMsg_UIMovExeIO(("+-MovieExe_OnAutoRec:idx=%d\r\n",uhParam));

        UI_SetData(FL_MovieAutoRecIndex, uhParam);
        UI_SetData(FL_MovieAutoRecLastMode, uhLastMode);

    }
    else
    {
        debug_err(("^R %s, parameters error\r\n",__func__));
    }


    return NVTEVT_CONSUME;
}

//#NT#2010/11/25#Photon Lin -end

//#NT#2012/07/31#Hideo Lin -begin
//-------------------------------------------------------------------------------------------------
extern void MediaRec_SetIPLChangeCB(MediaRecIPLChangeCB *pIPLChangeCB); // temporary
void MovieExe_FakeIPLChangeCB(UINT32 mode, UINT32 param)
{
    UINT32  APP_RESERVE_SIZE = 1920 * 1080 * 15;
    UINT32  uiPoolAddr;

    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_APP);

    switch (mode)
    {
    case MOVREC_IPLCHG_VIDEO:
        // Re-allocate buffer for Media Recorder
        MovRec_ReAllocBuf2VA(uiPoolAddr + APP_RESERVE_SIZE, POOL_SIZE_APP - APP_RESERVE_SIZE);
        break;
    }
}

void Movie_SetQPInitLevel(void)
{
    UINT32  uiQPLevel = H264_QP_LEVEL_3;

    // To do: set H.264 QP initial level according to ISO value

    MovRec_ChangeParameter(MOVREC_RECPARAM_H264QPLEVEL, uiQPLevel, 0, 0);
}

#if (UI_STYLE==UI_STYLE_DRIVER)
extern GPSDATA    gpsdata;
#endif
INT32 MovieExe_OnRecStart(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiWidth, uiHeight, uiMovieSize;
    UINT32  uiUserDataAddr, uiUserDataSize = 0;
    UINT32  uiWidth2 = 0, uiHeight2 = 0;

    if (SysGetFlag(FL_MOVIE_DUAL_REC))
    {
        uiMovieSize = UI_GetData(FL_MOVIE_SIZE_DUAL);           // movie size
        uiWidth     = GetMovieSizeWidth_2p(0, uiMovieSize);     // image width
        uiHeight    = GetMovieSizeHeight_2p(0, uiMovieSize);    // image height
        uiWidth2    = GetMovieSizeWidth_2p(1, uiMovieSize);     // image width
        uiHeight2   = GetMovieSizeHeight_2p(1, uiMovieSize);    // image height
    }
    else
    {
        uiMovieSize = UI_GetData(FL_MOVIE_SIZE);        // movie size
        uiWidth     = GetMovieSizeWidth(uiMovieSize);   // image width
        uiHeight    = GetMovieSizeHeight(uiMovieSize);  // image height
    }

    //#NT#2013/05/15#Calvin Chang#Add customer's user data to MOV/MP4 file format -begin
    if(MovieUdta_MakeVendorUserData(&uiUserDataAddr, &uiUserDataSize))
    {
        // Enable custom user data
        MovRec_ChangeParameter(MOVREC_RECPARAM_EN_CUSTOMUDATA, TRUE, 0, 0);
        // Set user data
        MovRec_SetUserData((UINT32)uiUserDataAddr, uiUserDataSize);
    }
    //#NT#2013/05/15#Calvin Chang -end

    Movie_SetRecParam();
    Movie_SetQPInitLevel();

    #if (MOVIE_TEST_ENABLE == ENABLE)
    {// test codes for fixed YUV patterns mode (no IPL)!!!
        if (g_MediaRecTestMode.bEnable && g_MediaRecTestMode.uiVidSrcType == MEDIAREC_SRC_FIXED_YUV)
        {
            // set YUV information if using fixed YUV as video source
            Movie_SetYUV(uiMovieSize);
            MediaRec_SetTestMode(&g_MediaRecTestMode);

            // register callback
            MediaRec_SetIPLChangeCB(MovieExe_FakeIPLChangeCB);
        }
    }
    #endif

    // for GPS data recording test
    MovRec_ChangeParameter(MOVREC_RECPARAM_GPSON, TRUE, 0, 0);
    strcpy(gpsdata.IQVer,IQS_GetVerInfo());
    strcpy(gpsdata.IQBuildDate,IQS_GetBuildDate());
    MovRec_SetGPSData((UINT32)&gpsdata, sizeof(GPSDATA));

    //---------------------------------------------------------------------------------------------
    // setup movie date stamp if necessary
    //---------------------------------------------------------------------------------------------
    // Force to turn off date stamp if it's TV in project.
#if (_SENSORLIB_ == _SENSORLIB_VIRTUAL_ && _SENSORLIB2_ == _SENSORLIB2_CMOS_TVP5150_)
    {
        MovieStamp_Setup(
            0,
            STAMP_OFF,
            uiWidth,
            uiHeight,
            NULL);

        MovieStamp_Setup(
            1,
            STAMP_OFF,
            uiWidth2,
            uiHeight2,
            NULL);
    }
#else

	UINT32		uiStampAddr;
	UINT32		uiStampPos = STAMP_BOTTOM_RIGHT;
	UINT32		uiStampSetup = STAMP_OFF, uiStampSetup2 = STAMP_OFF;
	STAMP_COLOR StampColorBg = {RGB_GET_Y( 16,	16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,	16,  16)}; // date stamp background color
	STAMP_COLOR StampColorFr = {RGB_GET_Y( 16,	16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,	16,  16)}; // date stamp frame color
	STAMP_COLOR StampColorFg = {RGB_GET_Y(224, 224, 192), RGB_GET_U(224, 224, 192), RGB_GET_V(224, 224, 192)}; // date stamp foreground color
	WATERLOGO_BUFFER *pWaterLogo = NULL;

	if (IsAELog() || IsADASLog())
	{
		uiStampPos = STAMP_TOP_LEFT;
	}

	#if (WATERLOGO_FUNCTION == ENABLE)
	if (UI_GetData(FL_MOVIE_WATERLOGO) == MOVIE_WATERLOGO_ON)
	{
		pWaterLogo = (WATERLOGO_BUFFER *)&g_WaterLogo;
	}
	#endif

	if (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON)
	{
		uiStampSetup =	STAMP_ON |
						STAMP_AUTO |
						STAMP_DATE_TIME |
						uiStampPos |
						STAMP_POS_NORMAL |
						STAMP_BG_TRANSPARENT |
						STAMP_YY_MM_DD |
						STAMP_IMG_420UV;

		uiStampSetup2 =	STAMP_ON |
						STAMP_AUTO |
						STAMP_DATE_TIME |
						uiStampPos |
						STAMP_POS_NORMAL |
						STAMP_BG_TRANSPARENT |
						STAMP_YY_MM_DD |
						#if  (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
						STAMP_IMG_422UV;
						#else
						STAMP_IMG_420UV;
						#endif
	}


	// use POOL_ID_DATEIMPRINT as movie data stamp buffer
	uiStampAddr = OS_GetMempoolAddr(POOL_ID_DATEIMPRINT);

	MovieStamp_SetDataAddr(0, uiStampAddr);
	MovieStamp_SetColor(0, &StampColorBg, &StampColorFr, &StampColorFg);

	MovieStamp_SetDataAddr(1, uiStampAddr + POOL_SIZE_DATEIMPRINT/2);
	MovieStamp_SetColor(1, &StampColorBg, &StampColorFr, &StampColorFg);

	// movie stamp setup for 1st path
	MovieStamp_Setup(0, uiStampSetup, uiWidth, uiHeight, pWaterLogo);

	if (SysGetFlag(FL_MOVIE_DUAL_REC))
	{
		// movie stamp setup for 2nd path
		MovieStamp_Setup(1, uiStampSetup2, uiWidth2, uiHeight2, pWaterLogo);
	}

#endif

    // send command to UIMovieRecObj to start movie recording
    Ux_SendEvent(&UIMovieRecObjCtrl, NVTEVT_START_REC_MOVIE, 0);

    //#NT#2012/10/30#Calvin Chang#Generate Audio NR pattern by noises of zoom, focus and iris -begin
    #if (MOVIE_AUDIO_NR_PATTERN_ENABLE == ENABLE)
    BKG_PostEvent(NVTEVT_BKW_ANR_LENS_ACTION);
    #endif
    //#NT#2012/10/30#Calvin Chang -end

    //#NT#2012/10/23#Philex Lin - begin
    // disable auto power off/USB detect timer
    #if (UI_STYLE==UI_STYLE_DRIVER)
    KeyScan_EnableMisc(FALSE);
    #endif
    //#NT#2012/10/23#Philex Lin - end
#if(WIFI_AP_FUNC==ENABLE)
    #if(MOVIE_LIVEVIEW==RTSP_LIVEVIEW)

    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {   //movie change mode OK
        WifiCmd_Done(WIFIFLAG_MODE_DONE,E_OK);
    }
    #else //((MOVIE_LIVEVIEW==HTTP_LIVEVIEW) ||(MOVIE_LIVEVIEW==DUAL_REC_HTTP_LIVEVIEW))
        //HTTP liveview should not start record,don't wait record started
    #endif
#endif
    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
static UINT32 g_uiDoPIM = FALSE;
INT32 MovieExe_OnRecStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    // send command to UIMovieRecObj to stop movie recording
    Ux_SendEvent(&UIMovieRecObjCtrl, NVTEVT_STOP_REC_MOVIE, 0);
    //#NT#2012/10/23#Philex Lin - begin
    // Enable auto power off/USB detect timer
    #if (UI_STYLE==UI_STYLE_DRIVER)
    KeyScan_EnableMisc(TRUE);
    #endif
    //#NT#2012/10/23#Philex Lin - end

    if (g_uiDoPIM)
    {
        DscMovie_CloseIPL();
        ImgCap_StopCapture();
        ImgCap_WaitFinish();
        ImgCap_Close();
        DscMovie_OpenIPL();
        g_uiDoPIM = FALSE;
    }

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnRecPIM(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    // do picture in movie (capture full size still image as movie recording; still image will be processed after recording end)
    DBG_IND("Capture full size image!\r\n");
    g_uiDoPIM = TRUE;
    ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_INVID);
    ImgCap_SetUIInfo(CAP_SEL_PICNUM, 1);
    ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
    ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
    ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_H_SIZE, GetPhotoSizeWidth(0));  // 0 is max still image size
    ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_V_SIZE, GetPhotoSizeHeight(0)); // 0 is max still image size
    ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 2); // 2 raw image buffer with 12-bit resolution for 128MB DRAM
    ImgCap_SetUIInfo(CAP_SEL_JPG_BUF_NUM, 1); // 1 JPEG buffer
    ImgCap_StartCapture();

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnRecRawEnc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiSrcWidth, uiSrcHeight, uiDAR, uiMovieSize;
    UINT32  uiJpegWidth = 320, uiJpegHeight = 240;

    // do raw encode (capture still image as movie recording; still image size is equal to or smaller than video size;
    // still image can be processed during movie recording)
    DBG_IND("Capture image!\r\n");

    if (paramNum > 1)
    {
        uiJpegWidth  = paramArray[0];
        uiJpegHeight = paramArray[1];
    }

    uiMovieSize = UI_GetData(FL_MOVIE_SIZE);
    uiSrcWidth  = GetMovieSizeWidth(uiMovieSize);
    uiSrcHeight = GetMovieSizeHeight(uiMovieSize);
    uiDAR = GetMovieDispAspectRatio(uiMovieSize);

    if (uiDAR == VIDENC_DAR_16_9)
    {
        uiSrcWidth = (uiSrcHeight * 16) / 9;
    }

    // limit JPEG width
    if (uiJpegWidth > uiSrcWidth)
    {
        uiJpegWidth = uiSrcWidth;
    }
    if (uiJpegWidth < 320)
    {
        uiJpegWidth = 320;
    }

    // limit JPEG height
    if (uiJpegHeight > uiSrcHeight)
    {
        uiJpegHeight = uiSrcHeight;
    }
    if (uiJpegHeight < 240)
    {
        uiJpegHeight = 240;
    }
    MovRec_SetRawEncJpgSize(0, uiJpegWidth, uiJpegHeight);
    //MovRecEnc_ChangeParameter(RAWENC_JPEG_WIDTH,  uiJpegWidth,  0, 0);
    //RawEnc_ChangeParameter(RAWENC_JPEG_HEIGHT, uiJpegHeight, 0, 0);
    MovRec_TriggerRawEncode(0, 0);
    //RawEnc_TriggerEncode();

    return NVTEVT_CONSUME;
}
//#NT#2012/07/31#Hideo Lin -end

//#Assign parameters of AVIPLAY_OBJ for movie-play
INT32 MovieExe_InitPlayAvi(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _DEMO_TODO
    AVIPLAY_OBJ aviPlayObj;
    UINT32 uiPoolAddr;
    //#NT#2010/09/28#JeahYen -begin
    ISIZE BufferSize;
    ISIZE DeviceSize;
    //#NT#2010/09/28#JeahYen -end
    //#NT#2010/09/28#JeahYen -begin
    BufferSize = Movie_GetBufferSize();
    DeviceSize = GxVideo_GetDeviceSize(DOUT1);
    //#NT#2010/09/28#JeahYen -end
    //#NT#2010/09/28#Photon Lin -begin
    #if (MOVIE_SPECIAL_MEM_ALLOC == ENABLE)
    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO1);
    aviPlayObj.uiMemAddr          = uiPoolAddr+0x100000;   //To avoid buffer overwrite between frame buffer of playback and avi
    aviPlayObj.uiMemSize          = POOL_SIZE_DISP_VDO1+POOL_SIZE_DISP_VDO1TEMP+POOL_SIZE_SIDC+POOL_SIZE_APP-0x100000;
    #else
    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_SIDC);
    aviPlayObj.uiMemAddr          = uiPoolAddr;
    aviPlayObj.uiMemSize          = POOL_SIZE_SIDC+POOL_SIZE_APP;
    #endif
    //#NT#2010/09/28#Photon Lin -end
    //#NT#2010/09/28#JeahYen -begin
    aviPlayObj.uiDisplayFBWidth   = BufferSize.w;
    aviPlayObj.uiDisplayFBHeight  = BufferSize.h;
    //#NT#2010/09/28#JeahYen -end
    aviPlayObj.uiPanelWidth       = 320;
    aviPlayObj.uiPanelHeight      = 240;
    aviPlayObj.CallBackFunc = Play_MovieCB;
    aviPlayObj.bHWVideo = TRUE;
    aviPlayObj.bHWAudio = TRUE;
    #if (_IKEY_DATEVIEW_MODE_== ENABLE)
    //#NT#2011/01/11#Ben Wang -begin
    //#NT#add i key playback mode
    if(UI_GetData(FL_PlayIKeyModeIndex) == PLAY_BY_DATE)
        aviPlayObj.bPlayAllFiles = TRUE;
    else
    #endif
        aviPlayObj.bPlayAllFiles = FALSE;
    //#NT#2011/01/11#Ben Wang -end
    aviPlayObj.bPlayWithBG = FALSE;
    //#NT#2010/09/15#Lily Kao -begin
    //#Play a movie as a *.MOV file, uLaw audio-encode format
    if (PLAYMODE_MOVMJPG == AppPlay_GetData(PLAY_CURRMODE))
    {
        DbgMsg_UIMovExe(("Init-Play MOV\r\n"));
        aviPlayObj.uiMovieFmt = MOVIEPLAY_FORMAT_MOV;
        aviPlayObj.uiAudioFmt = MOVIEPLAY_AUD_FMT_ULAW;
    }
    else
    {
        DbgMsg_UIMovExe(("Init-Play AVI\r\n"));
        aviPlayObj.uiMovieFmt = MOVIEPLAY_FORMAT_AVI;
        aviPlayObj.uiAudioFmt = MOVIEPLAY_AUD_FMT_WAV;
    }
    //#NT#2010/09/15#Lily Kao -end

    Ux_SendEvent(&UIMoviePlayObjCtrl,NVTEVT_INIT_PLAY_MOVIE,1,(UINT32)&aviPlayObj);
#endif

    return NVTEVT_CONSUME;
}

//#NT#2010/10/05#Lily Kao -begin
//#Changeable Manufacture,Product names by project
INT32 MovieExe_UtilSetMovCompareCompManuName(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _DEMO_TODO
    MOVUtil_SetCompareCompManu(gMovieCompManuStr, MOV_COMP_MANU_STRING_LEN);
    MOVUtil_SetCompareCompName(gMovieCompNameStr, MOV_COMP_NAME_STRING_LEN);
#endif
    return NVTEVT_CONSUME;
}
//#NT#2010/10/05#Lily Kao -end

void MovieExe_IPLGetReadyCB2_Sample(UINT32 mode, UINT32 param);
NVT_MEDIAOBJ gMediaobj;

ER MovieExe_SetGetReadyCB2_Sample(void)
{
    NVT_MEDIAOBJ *pMobj;

    pMobj = &gMediaobj;
    pMobj->GetReadyCb2 = MovieExe_IPLGetReadyCB2_Sample;
    DscMovie_SetGetReadyCb2(pMobj);
    return E_OK;
}

void MovieExe_IPLGetReadyCB2_Sample(UINT32 mode, UINT32 param)
{
    MEDIAREC_READYBUF_INFO readyInfo;
    IPL_IME_BUF_ADDR CurInfo;

    switch (mode)
    {
    case MOVREC_IPLGET_READYBUF:

        CurInfo.Id = IPL_ID_1;
        IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&CurInfo);

        readyInfo.y = CurInfo.ImeP1.PixelAddr[0];
        readyInfo.cb = CurInfo.ImeP1.PixelAddr[1];
        //readyInfo.cr = CurInfo.ImeP1.PixelAddr[2];
        readyInfo.cr = CurInfo.ImeP1.PixelAddr[1]; // UV pack
        readyInfo.y_lot = CurInfo.ImeP1.Ch[0].LineOfs;
        readyInfo.uv_lot = CurInfo.ImeP1.Ch[1].LineOfs;
        readyInfo.uiBufID = CurInfo.ImeP1.PixelAddr[2]; // for IME ready buffer check
        if (readyInfo.y == 0)
        {
            DBG_DUMP("not ready..\r\n");
            return;
        }
        if (MovRec_PutReadyBufWithID(1, &readyInfo)!= E_OK)
        {
            DBG_DUMP("not ready22..\r\n");
            return;
        }

        //DBG_IND("ImeP1: y 0x%x, cb 0x%x, cr 0x%x\r\n", CurInfo.ImeP1.PixelAddr[0], CurInfo.ImeP1.PixelAddr[1], CurInfo.ImeP1.PixelAddr[2]);
        MovRec_GetReadyBufWithID(1, &readyInfo);
        MovRec_SetReadyBufWithID(1, &readyInfo);

        //DBG_DUMP("^G gg22\r\n");
        break;

    default:
        break;
    }
}

#if MOVIE_FD_FUNC_
static void MovieExe_CallBackUpdateInfo(UINT32 callBackEvent)
{
    VControl *pCurrnetWnd;

    Ux_GetFocusedWindow(&pCurrnetWnd);
    Ux_SendEvent(pCurrnetWnd,NVTEVT_UPDATE_INFO, 1, callBackEvent);
}
#endif
INT32 MovieExe_OnFDEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if MOVIE_FD_FUNC_
    {
        //Flush FD event before draw
        Ux_FlushEventByRange(NVTEVT_EXE_MOVIE_FDEND,NVTEVT_EXE_MOVIE_FDEND);
        MovieExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FDEND);
    }
    #endif
    return NVTEVT_CONSUME;

}
INT32 MovieExe_OnFD(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if MOVIE_FD_FUNC_
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",paramArray[0]);

    UI_SetData(FL_MOVIE_FD,uhSelect);
    if(uhSelect == FD_ON)
    {
        FD_Lock(FALSE);
    }
    else
    {
        FD_Lock(TRUE);
    }
    #endif
    return NVTEVT_CONSUME;
}
INT32 MovieExe_OnFOV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiSelect = 0;
    UINT32  index;
    ER      result;
    IPL_MODE_INFOR  IplModeInfo;
    IPL_DZOOM       IplDzoomInfo;

    DBG_IND("%d \r\n",paramArray[0]);
    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_FOV, uiSelect);

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

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnWaterLogo(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_WATERLOGO, uiSelect);

    return NVTEVT_CONSUME;
}

INT32 MoiveExe_OnPipView(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 param = 0;

    if (paramNum)
        param = paramArray[0];

    DBGD(param);

    PipView_SetStyle(param);

    return NVTEVT_CONSUME;
}

//#NT#2013/10/29#Isiah Chang -begin
//#Implement YUV merge mode of recording func.
void FlowMovie_RecSetYUVMergeMode(BOOL bEnable)
{
    g_bMovieRecYUVMergeEnable = bEnable;
}

BOOL FlowMovie_RecGetYUVMergeMode(void)
{
    return g_bMovieRecYUVMergeEnable;
}

void FlowMovie_RecSetYUVMergeRecInterval(UINT32 uiSec)
{
    g_uiMovieRecYUVMergeInterval = uiSec;
}

UINT32 FlowMovie_RecGetYUVMergeRecInterval(void)
{
    return g_uiMovieRecYUVMergeInterval;
}

void FlowMovie_RecSetYUVMergeRecCounter(UINT32 uiCount)
{
    g_uiMovieRecYUVMergeCounter = uiCount;
}

UINT32 FlowMovie_RecGetYUVMergeRecCounter(void)
{
    return g_uiMovieRecYUVMergeCounter;
}

void FlowMovie_RecYUVMergeCounterInc(void)
{
    g_uiMovieRecYUVMergeCounter++;
}
//#NT#2013/10/29#Isiah Chang -end

////////////////////////////////////////////////////////////

EVENT_ENTRY CustomMovieObjCmdMap[] =
{
    {NVTEVT_EXE_OPEN,               MovieExe_OnOpen                     },
    {NVTEVT_EXE_CLOSE,              MovieExe_OnClose                    },
    {NVTEVT_EXE_MOVIESIZE,          MovieExe_OnMovieSize                },
    {NVTEVT_EXE_IMAGE_RATIO,        MovieExe_OnImageRatio               },
    {NVTEVT_EXE_MOVIEQUALITY,       MovieExe_OnMovieQuality             },
    {NVTEVT_EXE_WB,                 MovieExe_OnWB                       },
    //#NT#2011/04/15#Photon Lin -begin
    //#NT#Add AV sync mechanism
    {NVTEVT_EXE_MOVIEAVSYNC,        MovieExe_OnAVSyncCnt                },
    //#NT#2011/04/15#Photon Lin -end
    {NVTEVT_EXE_MOVIECOLOR,         MovieExe_OnColor                    },
    {NVTEVT_EXE_MOVIE_EV,           MovieExe_OnEV                       },
    {NVTEVT_EXE_MOVIE_AUDIO,        MovieExe_OnMovieAudio               },
    {NVTEVT_EXE_MOVIEAUDPLAYVOLUME, MovieExe_OnAudPlayVolume            },
    {NVTEVT_EXE_MOVIEAUDRECVOLUME,  MovieExe_OnAudRecVolume             },
    {NVTEVT_EXE_MOVIEDZOOM,         MovieExe_OnDigitalZoom              },
    {NVTEVT_EXE_MOVIECONTAF,        MovieExe_OnContAF                   },
    {NVTEVT_EXE_MOVIEMETERING,      MovieExe_OnMetering                 },
    {NVTEVT_EXE_MOVIEDIS,           MovieExe_OnDis                      },
    {NVTEVT_EXE_MOVIE_MCTF,         MovieExe_OnMCTF                     },
    {NVTEVT_EXE_MOVIE_RSC,          MovieExe_OnRSC                      },
    {NVTEVT_EXE_MOVIE_HDR,          MovieExe_OnHDR                      },
    {NVTEVT_EXE_MOVIEGDC,           MovieExe_OnGdc                      },
    {NVTEVT_EXE_MOVIESMEAR,         MovieExe_OnSmear                    },
    {NVTEVT_EXE_MOVIEDIS_ENABLE,    MovieExe_OnDisEnable                },
    {NVTEVT_EXE_MOVIE_DATE_IMPRINT, MovieExe_OnDateImprint              },
    {NVTEVT_EXE_GSENSOR,             MovieExe_OnGSENSOR                 },
    {NVTEVT_EXE_DUAL_REC,           MovieExe_OnDualRec                  },
    {NVTEVT_EXE_CYCLIC_REC,         MovieExe_OnCyclicRec                },
    {NVTEVT_EXE_MOTION_DET,         MovieExe_OnMotionDet                },
    //#NT#2010/11/16#Photon Lin -begin
    //#Add feature of movie key
    {NVTEVT_EXE_MOVIE_AUTO_REC,     MovieExe_OnAutoRec                  },
    //#NT#2010/11/16#Photon Lin -end
    //#NT#2012/07/31#Hideo Lin -begin
    //#NT#For custom movie record/stop event
    {NVTEVT_EXE_MOVIE_REC_START,    MovieExe_OnRecStart                 },
    {NVTEVT_EXE_MOVIE_REC_STOP,     MovieExe_OnRecStop                  },
    {NVTEVT_EXE_MOVIE_REC_PIM,      MovieExe_OnRecPIM                   },
    {NVTEVT_EXE_MOVIE_REC_RAWENC,   MovieExe_OnRecRawEnc                },
    //#NT#2012/07/31#Hideo Lin -end
    {NVTEVT_EXE_INITPLAYAVI,        MovieExe_InitPlayAvi                },//#NT#2010/02/03#Lily Kao
    //#NT#2010/10/05#Lily Kao -begin
    //#Changeable Manufacture,Product names by project
    {NVTEVT_EXE_SET_MOV_MANUNAME,   MovieExe_UtilSetMovCompareCompManuName},
    //#NT#2010/10/05#Lily Kao -end
    {NVTEVT_EXE_MOVIE_FDEND,        MovieExe_OnFDEnd                    },
    {NVTEVT_EXE_MOVIE_FD,           MovieExe_OnFD                       },
    {NVTEVT_EXE_MOVIE_FOV,          MovieExe_OnFOV                      },
    {NVTEVT_EXE_MOVIE_WATERLOGO,	MovieExe_OnWaterLogo                },
    {NVTEVT_EXE_MOVIE_PIPVIEW,      MoiveExe_OnPipView                  },
    {NVTEVT_NULL,                   0},  //End of Command Map
};

CREATE_APP(CustomMovieObj,APP_SETUP)

