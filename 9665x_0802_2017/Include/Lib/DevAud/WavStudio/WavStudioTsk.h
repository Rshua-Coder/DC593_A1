/**
    WavStudio module.

    @file       WavStudioTsk.h
    @ingroup    mILIBWAVSTUDIO

    @brief      Header file of WavStudio task.

                APIs/structure/macro declaration of WavStudio task.

    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

*/

/**
    @addtogroup mILIBWAVSTUDIO
*/
//@{

#ifndef _WAVSTUDIOTSK_H
#define _WAVSTUDIOTSK_H

#include "SysKer.h"
//#include "Audio.h"
#include "WAVLib.h"


//move from WavStudioID.h
extern void WavStudio_InstallID(void) _SECTION(".kercfg_text");


/**
     @name Wave Record Time
*/
//@{
#define WAVSTUDIO_DIR_FILE_AUTO             WAV_DIR_FILE_AUTO   // Create new WAV file automatically based on DCF naming rule
//@}


/**
    @name Wave file type
*/
//@{
#define WAVSTUDIO_FILE_SINGLE               0   ///< WAV file type standalone in separate wave file
#define WAVSTUDIO_FILE_EMBEDDED             1   ///< WAV file type embedded in JPEG EXIF header
//@}

/**
    @name Wave studio event IDs
*/
//@{
#define WAVSTUDIO_EVENT_RECORDING           0x00000001      ///< Start to record
#define WAVSTUDIO_EVENT_PLAYING             0x00000002      ///< Start to play
#define WAVSTUDIO_EVENT_STOPPED             0x00000004      ///< Stopped. (If in recording mode all data is saved)
#define WAVSTUDIO_EVENT_PAUSED              0x00000008      ///< Paused
#define WAVSTUDIO_EVENT_RESUMED             0x00000010      ///< Resumed
#define WAVSTUDIO_EVENT_SAVING              0x00000020      ///< Stop recording and save data in RAM to card
#define WAVSTUDIO_EVENT_DURATION            0x00000040      ///< 1 second passed
#define WAVSTUDIO_EVENT_DISKFULL            0x00000080      ///< Disk is full
#define WAVSTUDIO_EVENT_BUFFULL             0x00000100      ///< Buffer is full
#define WAVSTUDIO_EVENT_INVALIDFILE         0x00000200      ///< Not a valid file when playing
#define WAVSTUDIO_EVENT_RWERROR             0x00000400      ///< Read/Write error
#define WAVSTUDIO_EVENT_4G_BOUND            0x00001000      ///< Limit the maximum byte size of a wav file to 4G.
//@}

/**
    @name Wave studio task status
*/
//@{
#define WAVSTUDIO_STS_CLOSED                0x00000001      ///< Task is not opened
#define WAVSTUDIO_STS_STOPPED               0x00000002      ///< Stopped (No recording or playing)
#define WAVSTUDIO_STS_RECORDING             0x00000004      ///< Recording now
#define WAVSTUDIO_STS_PLAYING               0x00000008      ///< Playing now
#define WAVSTUDIO_STS_PAUSED                0x00000010      ///< Paused
#define WAVSTUDIO_STS_SAVING                0x00000020      ///< Saving remaining data in RAM to card
//@}

/**
    Wave studio event callback function prototype
*/
typedef void (*WAVSTUDIO_CB)(UINT32);

/**
    Wave studio data format
*/
typedef struct
{
    UINT32          uiBitsPerSample;    ///< BitsPerSample. PCM could be 8 or 16, ADPCM is always 4
    AUDIO_CODEC     AudioCodec;         ///< Audio codec type
    AUDIO_SR        AudioSamplingRate;  ///< Audio sampling rate
    AUDIO_CH        AudioChannel;       ///< Audio Channel selection
#if (WAVFILE_ASF_ENABLE == ENABLE)
    WAVFILETYPE     FileFormat;
#endif
    AUDIO_VOL       AudioVolume;
    AUDIO_GAIN      AudioGain;
} WAVSTUDIO_FMT, *PWAVSTUDIO_FMT;

/**
    Wave studio duration record
*/
typedef struct
{
    volatile UINT32 uiElapsed;      ///< Elapse time
    volatile UINT32 uiRemaining;    ///< Remain time
} WAVSTUDIO_DURATION, *PWAVSTUDIO_DURATION;

/**
    Wave studio memory allocation
*/
typedef struct
{
    UINT32          uiMemoryAddr;   ///< Memory address for wave record/playback
    UINT32          uiMemorySize;   ///< Memory size for wave record/playback
    WAVSTUDIO_CB    WavStudioCB;    ///< Wave studio record/playback callback
} WAVSTUDIO_APPOBJ, *PWAVSTUDIO_APPOBJ;

/**
    Wave studio file information of DCF format
*/
typedef struct
{
    UINT32          uiDirID;    ///< DCF directory ID
    UINT32          uiFileID;   ///< DCF file ID
    UINT32          uiType;     ///< Wave file type
                                ///< - @b WAVSTUDIO_FILE_SINGLE
                                ///< - @b WAVSTUDIO_FILE_EMBEDDED
} WAVSTUDIO_FILEINFO, *PWAVSTUDIO_FILEINFO;


/**
    Open WAV studio task.

    Start WAV studio task.

    @param[in] pWavObj  Wav Studio application object
    @return Open status
        - @b E_SYS: Task is already opened
        - @b E_NOMEM: Memory size is not enough
        - @b E_OK: No error
*/
extern ER       WavStudio_Open(PWAVSTUDIO_APPOBJ pWavObj);

/**
    Close WAV studio task.

    Close WAV studio task.

    @return Close status
        - @b E_SYS: Task is already closed
        - @b E_OK: No error
*/
extern ER       WavStudio_Close(void);

/**
    Get status.

    Get WAV studio task status.

    @return WAV studio task status, please refer to WavStudioTsk.h
        - @b WAVSTUDIO_STS_RECORDING
        - @b WAVSTUDIO_STS_PLAYING
        - @b WAVSTUDIO_STS_PAUSED
        - @b WAVSTUDIO_STS_STOPPED
        - @b WAVSTUDIO_STS_SAVING
        - @b WAVSTUDIO_STS_CLOSED
*/
extern UINT32   WavStudio_GetStatus(void);

/**
    Start to record.

    Start to record new WAV file.

    @return
        - @b TRUE: Start to record
        - @b FALSE: WAV studio task is not in stopped status
*/
extern BOOL     WavStudio_Record(void);

/**
    Start to play.

    Start to play WAV file.

    @return BOOL
        - @b TRUE: Start to play
        - @b FALSE: WAV studio task is not in stopped status
*/
extern BOOL     WavStudio_Play(void);

/**
    Stop recording / playing.

    Stop recording / playing.

    @return
        - @b TRUE: Stop recording / playing
        - @b FALSE: WAV studio task doesn't recording or playing
*/
extern BOOL     WavStudio_Stop(void);

/**
    Pause recording / playing.

    Pause recording / playing.

    @return
        -@ TRUE: Pause recording / playing
        -@ FALSE: Not recording / playing
*/
extern BOOL     WavStudio_Pause(void);

/**
    Resume recording / playing.

    Resume recording / playing from pause status.

    @return
        - @b TRUE: Resume from paused status
        - @b FALSE: Not in paused status
*/
extern BOOL     WavStudio_Resume(void);

/**
    Get duration

    Get elapse and remaining duration.
    If you call this API when WAV studio is stopped, the remaining time
    will be the total available time for recording.

    @param[out] pWavDuration    Duration information
    @return void
*/
extern void     WavStudio_GetDuration(PWAVSTUDIO_DURATION pWavDuration);

/**
    Get Wave length

    Get total wav length.

    @return wave length. Unit: second
*/
extern UINT32   WavStudio_GetWavLength(void);

/**
    Set File ID, DIR ID and file type.

    Set File ID, DIR ID and file type.\n

    For recording, if File or Dir ID is WAVSTUDIO_DIR_FILE_AUTO,
    WAV studio will create new file based on DCF naming rule.\n

    For playing, you have to assign the correct File/DIR ID.\n

    The file type could be single wave file (WAVSTUDIO_FILE_SINGLE)
    or embedded JPEG file (WAVSTUDIO_FILE_EMBEDDED).\n

    @note You have to call this API every time when you want to play or record.

    @param[in] pWavFileInfo     File/DIR ID and type information
    @return
        - @b TRUE: Set file / DIR ID and file type OK
        - @b FALSE: Some information is not valid
*/
extern BOOL     WavStudio_SetFile(PWAVSTUDIO_FILEINFO pWavFileInfo);

/**
    Delete WAV file by DIR ID and File ID

    Delete WAV file by DIR ID and File ID.

    @param[in] pWavFileInfo     File/DIR ID information
    @return
        - @b TRUE: Delete file OK
        - @b FALSE: Can't delete WAV file
*/
extern BOOL     WavStudio_DelFile(PWAVSTUDIO_FILEINFO pWavFileInfo);

/**
    Set WAV file format.

    Set WAV file format.

    @param[in] pWavFmt  WAV file format
    @return
        - @b TRUE: Set file format OK
        - @b FALSE: Some information is not valid
*/
extern BOOL     WavStudio_SetFormat(PWAVSTUDIO_FMT pWavFmt);

/**
    Set maximum recording time.

    Set maximum recording time.\n

    If the recording time reach the maximum recording time,
    WAV studio will stop recording automatically.\n

    If the maximum recording time is WAVSTUDIO_MAX_RECTIME_INFINITE,
    WAV studio will stop recording until disk is full.\n

    If the recording time is 0, the default value (30 Secs) will be applied.

    @param[in] uiSecond   Maximum recording time
    @return void
*/
extern void     WavStudio_SetMaxRecTime(UINT32 uiSecond);

/**
    Set user data for recording.

    Set user data for recording "single WAV file". If the user data address
    or size is zero, user data will be disabled. Otherwise, user data will
    be inserted into WAV header.

    @note User data is disabled by default and data size must be word alignment.

    @param[in] uiDataAddr     User data address
    @param[in] uiDataSize     User data size (must be word alignment)
    @return void
*/
extern void     WavStudio_SetUserData(UINT32 uiDataAddr, UINT32 uiDataSize);

/*
    WAV studio task

    WAV studio task

    @return void
*/extern void WavStudioTsk(void);


/**
    Get maximum recording time.

    Get maximum recording time.\n

    Default value is 30 Seconds.

    @param[in] void
    @return wave-record max time. Unit: second
*/
extern UINT32 WavStudio_GetMaxRecTime(void);

#endif

//@}
