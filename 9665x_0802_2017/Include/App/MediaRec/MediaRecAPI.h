/**
    Header file of Media Recorder Task

    Exported header file of media recorder task.

    @file       MediaRecAPI.h
    @ingroup    mIAPPMEDIAREC
    @note       add gMediaRecPwrOffPT
    @version    V1.02.005
    @date       2011/02/18

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _MEDIARECAPI_H
#define _MEDIARECAPI_H

#include "MediaWriteLib.h"
#include "Movieinterface_def.h"
#include "AVFile_MakerMov.h"
#include "AVFile_MakerAvi.h"
#include "MediaReadLib.h"
#include "AVFile_ParserMov.h"
#include "AVFile_ParserAvi.h"
#include "VideoEncode.h"

/**
    @addtogroup mIAPPMEDIAREC
*/
//@{

/**
    Stop type for MediaRec_Stop()
*/
typedef enum
{
    MEDIAREC_NO_WAIT  = 0x0,  ///< stop recording and no wait
    MEDIAREC_WAIT_END = 0x1,   ///< stop recording and wait this action finished
    ENUM_DUMMY4WORD(MEDIAREC_STOP_TYPE)
} MEDIAREC_STOP_TYPE;

/**
    check type for MediaRec_SetWriteCard()
*/
typedef enum
{
    MEDIAREC_WRITE_PATH1  = 0x0,  ///< write path 1 to card
    MEDIAREC_WRITE_PATH2  = 0x1,  ///< write path 2 to card
    ENUM_DUMMY4WORD(MEDIAREC_WRITECARD_TYPE)
} MEDIAREC_WRITECARD_TYPE;


#if 0
/**
    event type for CallbackOpenDISFunc( )
*/
typedef enum
{
    MEDIAREC_IPLMODE_PREVIEW  = 0x0,   // preview mode , only path1 works
    MEDIAREC_IPLMODE_VIDEO    = 0x1    // recording mode, path1/path2 work
} MEDIAREC_IPLMODE;
#endif

/**
    @name Media record event ID

    Callback ID of media recorder.
*/
//@{
#define     MEDIAREC_EVENT_RESULT_NORMAL        1   ///< Finish normally
#define     MEDIAREC_EVENT_RESULT_HW_ERR        2   ///< Hardware error
#define     MEDIAREC_EVENT_RESULT_FULL          3   ///< Card full
#define     MEDIAREC_EVENT_RESULT_SLOW          4   ///< Write card too slowly
#define     MEDIAREC_EVENT_RESULT_NOTREADY      5   ///< Not ready
#define     MEDIAREC_EVENT_THRESHOLD            6   ///< Threshold meets
#define     MEDIAREC_EVENT_RESULT_OVERTIME      7   ///< Overtime
#define     MEDIAREC_EVENT_RESULT_WRITE_ERR     10  ///< Filesystem write fails
#define     MEDIAREC_EVENT_STOPPED              11  ///< Callback when stops totally
#define     MEDIAREC_EVENT_FILECOMPLETE         13  ///< Write file completely
#define     MEDIAREC_EVENT_RESULT_CUT_FAIL      19  ///< Cut, overlap fail (write slowly)
//#define     MEDIAREC_EVENT_STOPTEST             20
#define     MEDIAREC_EVENT_CUSTOM_UDATA         21  ///< Set customized user data
#define     MEDIAREC_EVENT_ENCONE_OK            22  ///< enc one frame

#define     MEDIAREC_EVENT_H264BSCB_P0          23  ///< path1 video enc ok
#define     MEDIAREC_EVENT_H264BSCB_P1          24  ///< path2 video enc ok
#define     MEDIAREC_EVENT_RESULT_FLASHSTOP     25  ///< 30 sec after flash stop
#define     MEDIAREC_EVENT_AUDBSCB              26  ///< audio enc ok
#define     MEDIAREC_EVENT_LOOPREC_FULL         27  ///< no space for seamless recording
#define     MEDIAREC_EVENT_AUDPCM_CB            28  ///< audio PCM data callback event
#define     MEDIAREC_EVENT_DELETE_RO            29  ///< if readonly file is deleted
#define     MEDIAREC_EVENT_DELETE               30  ///< if any file is deleted
#define     MEDIAREC_EVENT_PCMAAC_CB            MOVREC_EVENT_PCMAAC_CB
#define     MEDIAREC_EVENT_START                0x80 ///< start recording
#define     MEDIAREC_EVENT_TIMELAPSE_ONESHOT    0x81 ///< timelapse oneshot
#define     MEDIAREC_EVENT_SLOWSTART            0x82 ///< slow stop begins
#define     MEDIAREC_EVENT_ADDONEVIDBS          0x83 ///< add one video BS
#define     MEDIAREC_EVENT_GETNAME_PATH0        0x84 ///< get custom filename
#define     MEDIAREC_EVENT_CUTNOW_FAIL          MOVREC_EVENT_CUTNOW_FAIL
//@}

//#NT#2012/09/12#Meg Lin -begin
/**
    @name Media ImmediatelyProc callback ID

    Callback ID of media ImmediatelyProc.
*/
//@{
#define     MEDIAREC_IMMPROC_CUTFILE_END        1  ///< end of one cut file, no parameter
#define     MEDIAREC_IMMPROC_DELETE_FILE        2  ///< end of deleting file, p1: directory ID, p2: file ID
#define     MEDIAREC_IMMPROC_ENC_SETTING        3  ///< special encoding parameters setting, such as H.264 3DNR
//@}
//#NT#2012/09/12#Meg Lin -end

/**
    @name Media record parameters

    Media recording changeable parameters.
*/
//@{
#define MEDIAREC_RECPARAM_WIDTH             MOVREC_RECPARAM_WIDTH       ///< width, p1 valid, p2 fileid
#define MEDIAREC_RECPARAM_HEIGHT            MOVREC_RECPARAM_HEIGHT      ///< height, p1 valid, p2 fileid
#define MEDIAREC_RECPARAM_FRAMERATE         MOVREC_RECPARAM_FRAMERATE   ///< frameRate, p1 valid
#define MEDIAREC_RECPARAM_QUALITY           MOVREC_RECPARAM_QUALITY     ///< quality, p1 valid
#define MEDIAREC_RECPARAM_TARGETRATE        MOVREC_RECPARAM_TARGETRATE  ///< target rate (bytes per second), p1 valid, p2 fileid
#define MEDIAREC_RECPARAM_GOLFSHOT_ON       MOVREC_RECPARAM_GOLFSHOT_ON ///< change recFormat to golfshot, p1 0 or 1
#define MEDIAREC_RECPARAM_CUTSEC            MOVREC_RECPARAM_CUTSEC      ///< seamless cut sec
#define MEDIAREC_RECPARAM_CUTSIZE           MOVREC_RECPARAM_CUTSIZE     ///< seamless cut size
#define MEDIAREC_RECPARAM_ENDTYPE           MOVREC_RECPARAM_ENDTYPE     ///< end type
#define MEDIAREC_RECPARAM_FLASH_ON          MOVREC_RECPARAM_FLASH_ON    ///< flash recording on/off, p1 0 or 1
#define MEDIAREC_RECPARAM_PWROFFPT          MOVREC_RECPARAM_PWROFFPT    ///< enable/disable poweroff_protection, default: on
#define MEDIAREC_RECPARAM_GPSON             MOVREC_RECPARAM_GPSON       ///< enable/disable gps info on, default: off
#define MEDIAREC_RECPARAM_TIMELAPSE         MOVREC_RECPARAM_TIMELAPSE   ///< set time lapse recording mode (ON/OFF)
#define MEDIAREC_RECPARAM_TIMELAPSE_TIME    MOVREC_RECPARAM_TIMELAPSE_TIME///< set time lapse recording time interval (in unit of ms, should be larger than 1/30 sec)
#define MEDIAREC_RECPARAM_THUMB_DATA        MOVREC_RECPARAM_THUMB_DATA  ///< movie thumbnail settings, p1:on/off, p2:width, p3:height
#define MEDIAREC_RECPARAM_AUD_SAMPLERATE    MOVREC_RECPARAM_AUD_SAMPLERATE///< audio sample rate, AUDIO_SR_8000 or others
#define MEDIAREC_RECPARAM_AUD_CHS           MOVREC_RECPARAM_AUD_CHS     ///< audio channels, 1 or 2
#define MEDIAREC_RECPARAM_AUD_SRC           MOVREC_RECPARAM_AUD_SRC     ///< audio source, MEDIAREC_AUDTYPE_LEFT or others
#define MEDIAREC_RECPARAM_AUD_CODEC         MOVREC_RECPARAM_AUD_CODEC   ///< audio codec type, MEDIAREC_ENC_PCM or others
#define MEDIAREC_RECPARAM_RECFORMAT         MOVREC_RECPARAM_RECFORMAT   ///< recording format, MEDIAREC_AUD_VID_BOTH or others
#define MEDIAREC_RECPARAM_EN_CUSTOMUDATA    MOVREC_RECPARAM_EN_CUSTOMUDATA///< enable custom user data or not
#define MEDIAREC_RECPARAM_VIDEOCODEC        MOVREC_RECPARAM_VIDEOCODEC  ///< video codec type, MEDIAREC_ENC_H264 or others
#define MEDIAREC_RECPARAM_H264GOPTYPE       MOVREC_RECPARAM_H264GOPTYPE ///< h264 GOP type, MEDIAREC_H264GOP_IPONLY or others
#define MEDIAREC_RECPARAM_H2643DNRLEVEL     MOVREC_RECPARAM_H2643DNRLEVEL///< h264 3DNR level
#define MEDIAREC_RECPARAM_RAWENC_Q          MOVREC_RECPARAM_RAWENC_Q    ///< Q value for raw-encoding
#define MEDIAREC_RECPARAM_EN_AUDFILTER      MOVREC_RECPARAM_EN_AUDFILTER///< enable audio filter or not
#define MEDIAREC_RECPARAM_DROP_FRAME_NUM    MOVREC_RECPARAM_DROP_FRAME_NUM///< drop frame number as start recording (for AV sync)
#define MEDIAREC_RECPARAM_H264QPLEVEL       MOVREC_RECPARAM_H264QPLEVEL ///< h264 QP initial level
//#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
#define MEDIAREC_RECPARAM_MOV_ROTATE        MOVREC_RECPARAM_MOV_ROTATE  ///< MOV/MP4 file rotation, MEDIAREC_MOV_ROTATE_0
//#NT#2013/04/17#Calvin Chang -end
#define MEDIAREC_RECPARAM_FILETYPE_2        MOVREC_RECPARAM_FILETYPE_2  ///< file1 type, MEDIAREC_AVI as default
#define MEDIAREC_RECPARAM_MERGEYUV          MOVREC_RECPARAM_MERGEYUV
#define MEDIAREC_RECPARAM_FILETYPE          MOVREC_RECPARAM_FILETYPE    ///< file type, MEDIAREC_MOV, MEDIAREC_AVI, MEDIAREC_MP4
#define MEDIAREC_RECPARAM_JPG_YUVFORMAT     MOVREC_RECPARAM_JPG_YUVFORMAT///< set jpeg yuv format
#define MEDIAREC_RECPARAM_MULTIREC_ON       MOVREC_RECPARAM_MULTIREC_ON
#define MEDIAREC_RECPARAM_LIVEVIEW          MOVREC_RECPARAM_LIVEVIEW    ///< set MEDIAREC_LIVEVIEW recformat 2013/10/02 Meg
#define MEDIAREC_RECPARAM_DAR               MOVREC_RECPARAM_DAR         ///< video display aspect ratio
#define MEDIAREC_RECPARAM_PRE_ENCODE        MOVREC_RECPARAM_PRE_ENCODE  ///< video pre-encode (for applying 3DNR to 1st frame)
#define MEDIAREC_RECPARAM_SENSOR_FPS        MOVREC_RECPARAM_SENSOR_FPS  ///< video sensor fps
#define MEDIAREC_RECPARAM_TINYMODE_ON       MOVREC_RECPARAM_TINYMODE_ON ///< tiny mode on/off, p1 0 or 1
#define MEDIAREC_RECPARAM_OVERLAP_ON        MOVREC_RECPARAM_OVERLAP_ON  ///< overlap on/off, p1 0 or 1 (default:on)

//@}

/**
    @name Valid video frame rate

    Valid video frame rate.
*/
//@{
#define MEDIAREC_VRATE_2            2  ///< 2 fps, only for MEDIAREC_FAST_FWD
#define MEDIAREC_VRATE_10           10 ///< 10 fps
#define MEDIAREC_VRATE_15           15 ///< 15 fps
#define MEDIAREC_VRATE_20           20 ///< 20 fps
#define MEDIAREC_VRATE_24           24 ///< 24 fps
#define MEDIAREC_VRATE_25           25 ///< 25 fps
#define MEDIAREC_VRATE_28           28 ///< 28 fps
#define MEDIAREC_VRATE_30           30 ///< 30 fps
#define MEDIAREC_VRATE_48           48  ///< 48 fps, only for 960p/720p/VGA/QVGA
#define MEDIAREC_VRATE_50           50  ///< 50 fps, only for 960p/720p/VGA/QVGA
#define MEDIAREC_VRATE_60           60  ///< 60 fps, only for 720p/VGA/QVGA
#define MEDIAREC_VRATE_120          120 ///< 120 fps, only for VGA/QVGA
#define MEDIAREC_VRATE_240          240 ///< 240 fps, only for QVGA
//@}

#if 0
/**
    @name Valid video quality

    Valid video quality.
*/
//@{
#define MEDIAREC_VQUALITY_BEST      1 ///< best
#define MEDIAREC_VQUALITY_NORMAL    2 ///< normal
#define MEDIAREC_VQUALITY_DRAFT     3 ///< draft
//@}
#endif

/**
    @name Valid file type

    Valid file type.
*/
//@{
#define MEDIAREC_AVI    MEDIA_FILEFORMAT_AVI    ///< .avi file
#define MEDIAREC_MOV    MEDIA_FILEFORMAT_MOV    ///< .mov file
#define MEDIAREC_MP4    MEDIA_FILEFORMAT_MP4    ///< .mp4 file
//@}

/**
    @name Valid video encode codec

    Valid video encode codec.
*/
//@{
#define MEDIAREC_ENC_JPG    MEDIAVIDENC_MJPG    ///< motion JPG
#define MEDIAREC_ENC_H264   MEDIAVIDENC_H264    ///< H.264
//@}

/**
    @name Valid audio encode codec

    Valid audio encode codec.
*/
//@{
#define MEDIAREC_ENC_PCM    MOVAUDENC_PCM   ///< PCM
#define MEDIAREC_ENC_AAC    MOVAUDENC_AAC   ///< AAC
//@}

/**
    @name Valid video input YCbCr format

    Valid video input YCbCr format.
*/
//@{
#define MEDIAREC_JPEG_FORMAT_420    0  ///< Y:Cb:Cr = 4:1:1
#define MEDIAREC_JPEG_FORMAT_422    1  ///< Y:Cb:Cr = 2:1:1
#define MEDIAREC_JPEG_FORMAT_444    2  ///< Y:Cb:Cr = 1:1:1
#define MEDIAREC_JPEG_FORMAT_100    3  ///< Y:Cb:Cr = 4:1:1
//@}

//#NT#2010/01/25#Meg Lin -begin
/**
    @name Valid counting type

    Valid counting type for MediaRec_Disk2SecondWithType.
*/
//@{
#define MEDIAREC_COUNTTYPE_FS    1  ///< get freespace in storage
#define MEDIAREC_COUNTTYPE_4G    2  ///< using 4G to calculate
#define MEDIAREC_COUNTTYPE_USER  3  ///< using given size to calculate
//@}
//#NT#2010/01/25#Meg Lin -end

#if 0
/**
    @name Valid save type

    Valid save type. Save data every one second or half second.
*/
//@{
#define MEDIAREC_SAVETYPE_ONE_SEC   1//save data every 1 sec
#define MEDIAREC_SAVETYPE_HALF_SEC  2//save data every 0.5 sec
//@}
#endif

/**
    @name Valid end type

    Valid recording end type. Normal end, cut overlap, or cut till card full.
*/
//@{
#define MEDIAREC_ENDTYPE_NORMAL             1 ///< normal end, such as press key to stop recording
#define MEDIAREC_ENDTYPE_CUTOVERLAP         2 ///< cut file and continue recording automatically; overlap the old files (cyclic recording)
#define MEDIAREC_ENDTYPE_CUT_TILLCARDFULL   3 ///< cut file and continue recording automatically till card full
#define MEDIAREC_ENDTYPE_MAX                3 ///< maximum number of recording end type
//@}

//#NT#2012/01/09#Hideo Lin -begin
//#NT#Add media recording source type (for test mode)
/**
    @name Media recording source type

    Media recording video source type: normal, fixed YUV pattern, bitstream;
    Media recording audio source type: normal, fixed PCM pattern.
*/
//@{
#define MEDIAREC_SRC_NORMAL         0   ///< normal source (video: sensor, audio: mic-in)
#define MEDIAREC_SRC_FIXED_YUV      1   ///< fixed YUV pattern (for video)
#define MEDIAREC_SRC_BS_ONLY        2   ///< bitstream only (for video)
#define MEDIAREC_SRC_FIXED_PCM      1   ///< fixed PCM pattern (for audio)
//@}
//#NT#2012/01/09#Hideo Lin -end

/**
    @name IPL mode change callback event

    Event ID for changing IPL mode callback function.
*/
//@{
#define MEDIAREC_IPLCHG_PREVIEW     1   ///< Set IPL to preview mode
#define MEDIAREC_IPLCHG_VIDEO       2   ///< Set IPL to video mode
//@}

/**
    @name Get IPL ready buffer callback event

    Event ID for getting IPL ready buffer callback function.
*/
//@{
#define MEDIAREC_IPLGET_READYBUF    1   ///< Get IPL ready buffer
//@}

/**
    @name media record information

    Media recording getting information.
*/
//@{
#define     MEDIAREC_GETINFO_WIDTH_HEIGHT   1   ///< p1: width (out), p2: height (out)
#define     MEDIAREC_GETINFO_VIDEO_CODEC    2   ///< p1: videocodec type(out), MEDIAREC_ENC_JPG or others//2012/10/15 Meg
                                                ///< p2: pathid (in), 0:path1 1:path2
#define     MEDIAREC_GETINFO_AUDIO_SR       3   ///< p1: audio samplerate(out), AUDIO_SR_32000 or others//2013/01/11 Meg
#define     MEDIAREC_GETINFO_AUDIO_CHS      4   ///< p1: audio channels(out), 1 or 2//2013/01/11 Meg
#define     MEDIAREC_GETINFO_AUDIO_BITS     5   ///< p1: audio bits per sample, 16
#define     MEDIAREC_GETINFO_H264_SPS       6   ///< p1: addr (out), p2: size (out), p3: num(0 or 1)
#define     MEDIAREC_GETINFO_VIDEO_FPS      7   ///< p1: v fps (out)
#define     MEDIAREC_GETINFO_WIDTH_HEIGHT_P2 8  ///< p1: width of Path2 (out), p2: height of Path2(out)
#define     MEDIAREC_GETINFO_AUDIO_CODEC    9   ///< p1: audio codec type of 1stpath(out), MEDIAREC_ENC_PCM or others
#define     MEDIAREC_GETINFO_SEAMLESSSEC    10  ///< p1: seamless Sec(out),
#define     MEDIAREC_GETINFO_RECFORMAT      11  ///< p1: recformat of 1stpath(out), MEDIAREC_AUD_VID_BOTH or others
//@}

/**
    @name media audio type information

    Media recording audio type information.
*/
//@{
#define     MEDIAREC_AUDTYPE_LEFT   1   ///< left
#define     MEDIAREC_AUDTYPE_RIGHT  2   ///< right
#define     MEDIAREC_AUDTYPE_STEREO 3   ///< stereo
#define     MEDIAREC_AUDTYPE_MONO   4   ///< mono
//@}

//#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
/**
    @name media rotation angle information

    Media recording roation angle information.
*/
//@{
#define     MEDIAREC_MOV_ROTATE_0       0   ///< angle 0
#define     MEDIAREC_MOV_ROTATE_90      1   ///< angle 90
#define     MEDIAREC_MOV_ROTATE_180     2   ///< angle 180
#define     MEDIAREC_MOV_ROTATE_270     3   ///< angle 270
//@}
//#NT#2013/04/17#Calvin Chang -end

/**
    Status type for MediaRec_GetStatus()
*/
typedef enum
{
    MEDIAREC_STATUS_NOT_OPENED        = 0x1,  ///< media recorder task is not open
    MEDIAREC_STATUS_OPENED_NOT_RECORD = 0x2,  ///< media recorder task is open, not recording
    MEDIAREC_STATUS_RECORDING         = 0x3,  ///< media recorder task is recording
    ENUM_DUMMY4WORD(MEDIAREC_STATUS_TYPE)
} MEDIAREC_STATUS_TYPE;

//#NT#2012/02/23#Meg Lin -begin
typedef struct
{
    UINT32      uiPCMAddr;          //in: address to put pcm
    UINT32      uiPCMSize;          //in: size to put pcm
    UINT32      uiAACBSAddr;        //in: address to put aac bitstream
    UINT32      uiAACBSSize;        //in: size to put aac bitstream
    UINT32      uiAudSampleRate;    //in: AUDIO_SR_8000
    UINT32      uiAudBitRate;       //in: target bitrate, MEDIA_AUDIOBR_96000
    UINT32      uiAudRawBlock;      //out: bytes to encode one audio BS (1024 samples for AAC)
    UINT32      uiAudChannels;      //in: channels, 1 or 2
    BOOL        bTestByFile;        //in: test by file (TRUE) or by microphone (FALSE)
    char        *pPCMFileName;      //in: the file name for writing PCM data
    char        *pAACFileName;      //in: the file name for writing AAC data
} MEDIAREC_AACETEST_OBJ;
//#NT#2012/02/23#Meg Lin -end

typedef struct
{
    UINT32      uiInAddr;           //in: address for AAC bs input
    UINT32      uiOutAddr;          //in: address for PCM output
    UINT32      uiAudSampleRate;    //in: AUDIO_SR_8000
    UINT32      uiAudChannels;      //in: channels, 1 or 2
    char        *pPCMFileName;      //in: the file name for writing PCM data
    char        *pAACFileName;      //in: the file name for reading AAC data
} MEDIAREC_AACDTEST_OBJ;

typedef enum
{
    RAW_FORMAT_PATH1 = 0x0,   ///< fileid 0
    RAW_FORMAT_PATH2 = 0x1,   ///< fileid 1
    ENUM_DUMMY4WORD(RAW_TRIGENC_PATH)
} RAW_TRIGENC_PATH;

typedef enum
{
    RAW_FORMAT_ONEPATH = 0x0,   ///< with fileid SMRAW_FORMAT_PATH1/SMRAW_FORMAT_PATH2
    RAW_FORMAT_ALLPATH = 0x1,   ///< valid, if multi-recording
    ENUM_DUMMY4WORD(RAW_TRIGENC_TYPE)
} RAW_TRIGENC_TYPE;

/**
     Install Task, Sem, Flag ID for MediaRec.

*/
extern void MediaRec_InstallID(void) _SECTION(".kercfg_text");

/*
    Public media recording functions
*/
//MediaRecTsk.c
/**
    Open media recorder task.

    Open media recorder task to prepare for recording.

    @param[in] param    the media recorder object

    @return
        - @b E_OK:  open media recorder task successfully
        - @b E_SYS: media recorder task is already opened
*/
extern ER       MediaRec_Open(MEDIAREC_OBJ *param);

/**
    Close media recorder task.

    Close media recorder related tasks and stop recording if necessary.

    @return
        - @b E_OK:  terminate media recorder task successfully
        - @b E_SYS: media recorder task is not opened
*/
extern ER       MediaRec_Close(void);

/**
    Start media recording.

    @return
        - @b E_OK:  start recording successfully
        - @b E_SYS: media recorder is recording or not ready
        - @b E_PAR: recorder setting error (such as recording format, video codec type, ...)
*/
extern ER       MediaRec_Record(void);

/**
    Stop media recording.

    @param[in] waitType waiting type as media recording stop, MEDIAREC_NO_WAIT or MEDIAREC_WAIT_END

    @return
        - @b E_OK:  stop recording successfully
        - @b E_SYS: media recorder is not recording
*/
extern ER       MediaRec_Stop(MEDIAREC_STOP_TYPE waitType);

/**
    Change media recorder parameters.

    Change media recorder parameters, such as width, height, frame rate, target bit rate, ..., and so on.

    @note

    @param[in] type parameter type
    @param[in] p1   parameter1
    @param[in] p2   parameter2
    @param[in] p3   parameter3

    @return
        - @b E_OK:  change successfully
        - @b E_PAR: parameter value error
*/
extern ER       MediaRec_ChangeParameter(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3);

/**
    Get media recorder parameter settings.

    Get media recorder parameter settings, only support getting width and height information currently.

    @note

    @param[in]  type    parameter type, only support MEDIAREC_GETINFO_WIDTH_HEIGHT currently
    @param[out] p1      parameter1 value
    @param[out] p2      parameter2 value
    @param[out] p3      parameter3 value

    @return
        - @b E_OK:  get parameter successfully
        - @b E_SYS: parameter address error
        - @b E_PAR: parameter type error
*/
extern ER       MediaRec_GetInfo(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3);

/**
    Set user data.

    Set user data for media container.

    @note The user data is in 'JUNK' chunk for AVI file, and in 'udta' atom for MOV/MP4 file.

    @param[in] addr user data starting address
    @param[in] size user data size

    @return void
*/
extern void     MediaRec_SetUserData(UINT32 addr, UINT32 size);

/**
    Estimate remaining time for media recording.

    Estimate remaining time in seconds according to storage device space with different counting type.

    @note This function should be called before media recording.

    @param[in] type counting type, MEDIAREC_COUNTTYPE_FS (according to storage free space),
                    MEDIAREC_COUNTTYPE_4G (FAT file size limitation), or MEDIAREC_COUNTTYPE_USER
                    (user defined space size, for special purpose)
    @param[in] size user defined space size, just for MEDIAREC_COUNTTYPE_USER type

    @return Estimated remaining time in seconds.
*/
extern UINT32   MediaRec_Disk2SecondWithType(UINT32 type, UINT32 size);//2010/01/25 Meg

/**
    Set restriction for recording time.

    Set restriction to calculate remaining time for media recording.

    @note

    @param[in] type     restriction type, MEDIAREC_RESTR_INDEXBUF or MEDIAREC_RESTR_FSLIMIT
    @param[in] value    set FALSE to disable, and others to enable the related restriction

    @return void
*/
extern void     MediaRec_SetDisk2SecRestriction(MEDIAREC_RESTR_TYPE type, UINT32 value);//2010/05/17 Meg Lin

/**
    Set free user data.

    Set free user data for special purpose.

    @note It's only valid for MOV/MP4 file. The data will be put in 'fre1' atom.

    @param[in] addr free data starting address
    @param[in] size free data size

    @return void
*/
extern void     MediaRec_SetFre1Data(UINT32 addr, UINT32 size);

/**
    Re-allocate buffer to media recorder.

    Re-allocate buffer for media video and audio recording usage.

    @note The buffer should be re-allocated before media recording.

    @param[in] memStart buffer starting address
    @param[in] memSize  buffer size

    @return
        - @b E_OK:  re-allocate buffer successfully
        - @b E_SYS: re-allocate buffer failed (memory is not enough)
*/
extern ER       MediaRec_ReAllocBuf2VA(UINT32 memStart, UINT32 memSize);

/**
    Stop media recording and set read-only.

    Stop media recording and set previous file, current file and next file to be read-only.

    @return
        - @b E_OK:  stop recording successfully
        - @b E_SYS: media recorder is not recording
*/
extern ER       MediaRec_StopAndReadOnly(void);//2012/02/17 Meg

/**
    To get media recorder status.

    @return
        - @b MEDIAREC_STATUS_NOT_OPENED:        media recorder is not opened
        - @b MEDIAREC_STATUS_OPENED_NOT_RECORD: media recorder is opened but not recording
        - @b MEDIAREC_STATUS_RECORDING:         media recorder is recording
*/
extern MEDIAREC_STATUS_TYPE   MediaRec_GetStatus(void);

/**
    Set audio volume for Media Recorder.

    @param[in] percent  the percentage of audio gain level (0% - 100%)
    @return
        - @b E_OK:  set successfully
*/
extern ER MedaiRec_SetAudioVolume(UINT32 percent);


extern void     MediaRec_Sleep(void);
extern void     MediaRec_Wakeup(void);
extern void     MediaRec_PauseDMA(void);
extern void     MediaRec_ResumeDMA(void);
extern void     MediaRec_ChangeDisplaySize(UINT32 uiDisp_w, UINT32 uiDisp_h);


//MediaVideoRecTsk.c
/**
    Set ready buffer for media recorder.

    Set ready buffer information, such as Y, Cb, Cr starting address and line offset for media recorder.

    @note This function is usually used in IPLGetReadyCB (get IPL ready buffer callback) of media recorder object.
          In the callback, get IPL ready buffer from IPL driver, then use this API to set to media recorder.

    @param[in] pInfo    ready buffer information for media recorder

    @return void
*/
extern void     MediaRec_ResetReadyBuf(void);
extern void     MediaRec_PutReadyBuf(MEDIAREC_READYBUF_INFO *pInfo);
extern void     MediaRec_GetReadyBuf(MEDIAREC_READYBUF_INFO *pInfo);
extern void     MediaRec_SetReadyBuf(MEDIAREC_READYBUF_INFO *pInfo);//2012/08/30 Meg
extern ER       MediaRec_PutReadyBufWithID(UINT32 fileid, MEDIAREC_READYBUF_INFO *pInfo);
extern void     MediaRec_GetReadyBufWithID(UINT32 fileid, MEDIAREC_READYBUF_INFO *pInfo);
extern void     MediaRec_SetReadyBufWithID(UINT32 fileid, MEDIAREC_READYBUF_INFO *pInfo);//2012/08/30 Meg


//MediaAudioRecTsk.c
extern void     MediaRec_Pause(void);
extern void     MediaRec_Resume(void);

//MediaRecFSTsk.c
/**
    Set GPS data.

    Set GPS data for media container.

    @note It's only valid for AVI file currently. The GPS data is put in 'JUNK' chunk per second.

    @param[in] addr GPS data starting address
    @param[in] size GPS data size

    @return void
*/
extern void     MediaRec_SetGPSData(UINT32 addr, UINT32 size);//2011/06/23 Meg

/**
    Set crash event.

    Set current recording file to be renamed and read-only.

    @note

    @return void
*/
extern void     MediaRec_SetCrash(void);//2011/06/27 Meg

/**
    Cancel crash event.

    Cancel crash event.

    @note

    @return void
*/
extern void     MediaRec_CancelCrash(void);//2013/11/18 Meg

/**
    Set event.

    Set current recording file to be renamed and read-only.

    @param[in] pathid 0 or 1
    @param[in] eventid MEDIAREC_CRASHTYPE_EV1 or others

    @return void
*/
extern void MediaRec_SetEvent(UINT32 pathid, UINT32 eventid);


/**
    To use same crash DID or not.

    To use same or different DCF directory ID for crash event.

    @note Set TRUE to use same directory ID, FALSE to use different directory ID.

    @param[in] on   turn on/off same crash DID function

    @return void
*/
extern void     MediaRec_SetTheSameCrashDid(UINT8 on);//2011/09/15 Meg

/**
    Set crash folder name.

    Set DCF folder name for crash event.

    @note It should follow the DCF naming rule, that is, 5 characters in 'A'~'Z', 'a'~'z', '0'~'9', and '_'.

    @param[in] pChar    pointer to the folder name

    @return void
*/
extern void     MediaRec_SetCrashFolderName(char *pChar);//2011/09/15 Meg

/**
    Set naming rule for Media Recorder.

    @param[in] pHdl    pointer to the naming rule
    @return
        - @b E_OK:  set successfully
*/
extern ER MediaRec_SetFSNamingHandler(MEDIANAMINGRULE *pHdl);


extern void MediaRecCmdTsk(void);
extern void MediaRecTsk(void);
extern void MediaVideoRecTsk(void);
extern void RawEncTsk(void);
extern void MediaAudioEncTsk(void);
extern void MediaRecFSTsk(void);
extern void MediaRecVideoStrmTsk(void);


extern void MediaRec_SetTestMode(MEDIAREC_TESTMODE *pTestMode);
extern void MediaRec_SetIPLChangeCB(MediaRecIPLChangeCB *pIPLChangeCB);
extern void MediaRec_VideoDoEnc(void);
extern void MediaRec_GetEncInfo(UINT32 puiParam[7]);
extern void MediaRec_SetFileHandle(FST_FILE fhdl, UINT32 fileid);
extern void MediaRecFS_SetWorkingID(UINT32 id);
extern UINT32   MediaRec_GetNowVideoFrames(void);

/**
    Open media recording error msg.

    @param[in] on  1 to open error msg
    @return void
*/
extern void MediaRec_OpenErrMsg(UINT8 on);

/**
    Reset media recorder containerMaker.

    @return void
*/
extern void MediaRec_ResetConMaker(void);

/**
    Open media recording warning msg.

    @param[in] on  1 to open warning msg
    @return void
*/
extern void MediaRec_OpenWrnMsg(UINT8 on);

/**
    Open media recording seamless recording msg.

    @param[in] on  1 to open seamless recording msg
    @return void
*/
extern void MediaRec_OpenCutMsg(UINT8 on);

/**
    Check media recorder if recording.

    @param[in] void
    @return
        - @b TRUE:  recording
        - @b FALSE: not recording
*/
extern BOOL MediaRec_IsRecording(void);

/**
    Set memory addr and size for MergeYUV.

    @param[in] addr memory address
    @param[in] size memory size
    @return void
*/
extern void MediaRec_SetGiveYUVBuf(UINT32 addr, UINT32 size);

/**
    Set readyYUV address for MergeYUV.

    @param[in] ptr    pointer to YUV info
    @return void
*/
extern void MediaRec_GiveYUV(MEDIAREC_READYBUF_INFO *ptr);

/**
    Set Extend crash.
    If seamless recording, this will set Crash after 30 sec.
    Max 3 times. If busy, return E_SYS.

    @param[in] void
    @return
        - @b E_OK: ok
        - @b E_SYS: busy
*/
extern ER MediaRec_SetExtendCrash(void);//2013/09/04 Meg

/**
    Dump MediaStatus.

    @param[in] pData    pointer to data,p1=width,p2=height,p3=bpp,p4=fps
    @return number of data
*/
extern UINT32 MediaRec_DumpDataStatus(UINT32* pData);//2013/08/15

/**
    Get lastest video bitstream addr,size.

    @param[in] frameBuf    pointer to video frame
    @return 0
*/
extern UINT32 MediaRec_GetLastV_BS(MEDIAREC_VIDBS_TYPE vidnum, MEDIAREC_MEM_RANGE* frameBuf);

/**
    Force MediaRec to check FS_DiskInfo.

    @param[in] void
    @return void
*/
extern void MediaRec_CheckFSDiskInfo(void);

/**
    Open media recording FSObj msg.

    @param[in] on  1 to open FSObj msg
    @return void
*/
extern void MediaRec_OpenFSOBJMsg(UINT8 on);

/**
    Open media recording file msg.

    @param[in] on  1 to open file msg
    @return void
*/
extern void MediaRec_OpenFileMsg(UINT8 on);

/**
    Set path2 write card or not.

    @param[in] on  1 to write path2 to card, if path2 enable
    @return void
*/
extern void MediaRec_SetPath2WriteCard(UINT32 on);


/**
    Set path2 IPL get ready buffer callback.

    @param[in] IPLCb  path2 IPL get ready buffer callback
    @return void
*/
extern void MediaRec_RegisterCB2(MediaRecIPLGetReadyCB IPLCb);

/**
    Minus filesize if some files written outside MediaREC.

    @param[in] size filesize, Must be cluster-alignment!!
    @return void
*/
extern void     MediaRec_MinusFilesize(UINT32 size);//2010/01/07 Meg

extern void MediaRec_StopPreRecordStartWriting(void);

/**
    Get H264 sps/pps for each video path.

    @param[in] fileid video path, 0 or 1
    @param[in] pAddr address to put sps/pps
    @param[in] pSize size of sps/pps
    @return void
*/
extern void     MediaRec_VidGetDesc(UINT32 fileid, UINT32 *pAddr, UINT32 *pSize);

/**
    Get minimum memory size for liveview mode.

    @param[in] void
    @return memory size
*/
extern UINT32 MediaRec_GetLiveViewMinMemSize(void);


/**
    Update setting to calculate disk2second.

    @param[in] void
    @return void
*/
extern void MediaRec_UpdateSetting2CaluSec(void);

/**
    Set Customized Data. (Put end of file with specific tag)

    @param[in] tag abcd=0x61626364
    @param[in] addr data address
    @param[in] size data size
    @return void
*/
extern void MediaRec_SetCustomData(UINT32 tag, UINT32 addr, UINT32 size);
extern void MediaRec_SetCustomDataWithID(UINT32 id, UINT32 tag, UINT32 addr, UINT32 size);

/**
    Set read-only percentage.

    @param[in] percent min=30, max=50
    @return void
*/
extern void MediaRec_SetROPercent(UINT32 percent);

/**
    Enable end-char for single recording.

    @param[in] value on/off
    @return void
*/
extern void MediaRec_EnableEndChar4SingleRec(UINT8 value);

/**
    Enable end-char for multiple recording.

    @param[in] value on/off
    @return void
*/
extern void MediaRec_EnableEndChar4MultiRec(UINT8 value);

/**
    Get lastest audio bitstream.

    @param[in] frameBuf output audio bs addr/size
    @return void
*/
extern void MediaRec_GetLastA_BS(MEDIAREC_MEM_RANGE* frameBuf);


/**
    Get lastest PCM for AAC encoding bitstream.

    @param[in] frameBuf output audio bs addr/size
    @return void
*/
extern void MediaRec_GetLastPCMAAC_BS(MEDIAREC_MEM_RANGE* frameBuf);

/**
    Get lastest movie filepath.

    @param[in] pPath output filepath, length=TEST_PATHLENGTH (80)
    @return void
*/
extern void MediaRec_GetLastFilePath(char *pPath);

/**
    Enable Check and delete read-only file when seamless recoring.

    @param[in] value on/off, default: on
    @return void
*/
extern void MediaRec_EnableCheckReadonly(UINT8 value);

/**
    Input settings to calculate recording seconds.

    @param[in] pSetting input recording setting
    @return recording seconds
*/
extern UINT32 MediaRec_CalcSecBySetting(MEDIAREC_CALCSEC_SETTING *pSetting);

/**
    Set max customdata size. Card capacity will minus this size when start recording.

    @param[in] fileID
    @param[in] maxSize
    @return void
*/
extern void MediaRec_SetCustomDataMaxSize(UINT32 fileid, UINT32 maxSize);

/**
    Set frameinfo size. Default: 4MB.

    @param[in] size
    @return void
*/
extern void MediaRec_SetFrameInfoSize(UINT32 size);

/**
    Trigger encode picture in movie. If trigger all paths, it takes more time..

    @param[in] type RAW_FORMAT_ONEPATH or RAW_FORMAT_ALLPATH
    @param[in] path RAW_FORMAT_PATH1 or RAW_FORMAT_PATH2
    @return
        - @b RAWENC_OK:                     encoding successfully
        - @b RAWENC_ERR_NOT_OPEN:           raw encode task is not opened
        - @b RAWENC_ERR_BUSY:               raw encode task is busy
        - @b RAWENC_ERR_IMG_NOT_READY:      YUV raw image is not ready
        - @b RAWENC_ERR_UNSAFE_TO_WRITE:    memory may be not enough for writing JPEG file
*/
extern INT32 RawEnc_TriggerEncodeWithType(RAW_TRIGENC_TYPE type, RAW_TRIGENC_PATH path);

/**
    Set this and last file as read-only.

    @param[in] void
    @return
        - @b MOVREC_SETCRASH_OK:            this and last both read-only
        - @b MOVREC_SETCRASH_NOLASTFILE:    this will be read-only (later), last not exist
        - @b MOVREC_SETCRASH_LASTFILERO:    this will be read-only (later), last has been RO already
*/
extern UINT32 MediaRec_SetReadonly_ThisandLastfile(void);

/**
    Set end char for each path. 'A' and 'B' as default value.

    @param[in] endchar A~Z, a~z
    @param[in] fileid  0 as path1, 1 as path2...
    @return
        - @b E_OK:  OK
        - @b E_SYS: repeat as others
*/
extern ER MediaRec_SetEndChar(char endchar, UINT32 fileid);

/**
    Delete files (including RO). It SHOULD NOT be executed when recording.

    @param[in] roDelete delete read-only files or not
    @param[in] roPercent  read-only max percent (30~50)
    @return void
*/
extern void MediaRec_Clean4Recording(UINT32 roDelete, UINT32 roPercent);

/**
    Set deleting read-only files or not.

    @param[in] value 0: No delete RO files, 1: as default
    @return void
*/
extern void MediaRec_SetDeleteReadonly(UINT32 value);

/**
    Get read-only files percent. (including freespace)

    @param[in] void
    @return RO percent
*/
extern UINT32 MediaRec_GetROPercent(void);

/**
    Set extend-crash-unit seconds. 20 or 30 seconds.

    @param[in] value, MOVREC_EXTENDUNIT_20 or MOVREC_EXTENDUNIT_30(default)
    @return void
*/
extern void MediaRec_SetExtendCrashUnit(MOVREC_EXTENDCRASH_UNIT value);

/**
    Set card write or not

    @param[in] path, 0 or 1, default: path0 = 1, path1 = 1
    @param[in] on, 0 or 1
    @return void
*/
extern void MediaRec_SetWriteCard(UINT32 path, UINT32 on);

/**
    Trigger write file for YUV merge mode.

    @param[in] void
    @return void
*/
extern void MediaRec_TriggerWriteFile(void);

/**
    Set H.264 Max Qp level.

    @param[in] level H264_MAX_QP_LEVEL_0 as default
    @return void
*/
extern void MediaRec_SetH264MaxQp(UINT32 pathid, UINT32 level);

/**
    if endtype is MEDIAREC_ENDTYPE_CUTOVERLAP, cut now!
    (if fails, callback MOVREC_EVENT_CUTNOW_FAIL

    @param[in] void
    @return void
*/
extern void MediaRec_SetCutNOW(void);

/**
    if recording format = MEDIAREC_GOLFSHOT
      playing framerate could be set MOVREC_GOLFSHOT_FR30 or others

    @param[in] void
    @return void
*/
extern void MediaRec_SetGolfshot_FrameRate(UINT32 value);

/**
    Set lastfile as read-only and move to RO folder.

    @param[in] void
    @return
        - @b MOVREC_SETCRASH_OK:            this and last both read-only
        - @b MOVREC_SETCRASH_NOLASTFILE:    last not exist
        - @b MOVREC_SETCRASH_LASTFILERO:    last has been RO already
*/
extern UINT32 MediaRec_SetCrash_Lastfile(void);

/**
    Set nextfile as read-only and move to RO folder.

    @param[in] void
    @return
        - @b MOVREC_SETCRASH_OK:            next file will be read-only
*/
extern UINT32 MediaRec_SetCrash_Nextfile(void);


#endif//_MEDIARECAPI_H

//@}
