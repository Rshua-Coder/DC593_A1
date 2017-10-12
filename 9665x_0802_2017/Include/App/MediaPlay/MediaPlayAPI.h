/**
    Header file of Media Player Task

    Exported header file of media Player task.

    @file       MediaPlayAPI.h
    @ingroup    mIAPPMEDIAPLAY
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _MEDIAPLAYAPI_H
#define _MEDIAPLAYAPI_H

#include <stdio.h>
#include <string.h>
#include "type.h"
#include "KERNEL.h"
#include "SysKer.h"
#include "VideoDecode.h"
#include "MediaReadLib.h"
#include "AVFile_ParserMov.h"
#include "AVFile_ParserAvi.h"

/**
    @addtogroup mIAPPMEDIAPLAY
*/
//@{

/**
    @name status of media player

    Status of media player
*/
//@{
#define MEDIAPLAY_STATUS_PAUSE        1       ///< Pause Status
#define MEDIAPLAY_STATUS_OTHER        2       ///< Other Status
#define MEDIAPLAY_STATUS_PLAYING      3       ///< Playing Status
#define MEDIAPLAY_STATUS_STOPPED      4       ///< Stop Status
#define MEDIAPLAY_STATUS_STEPBYSTEP   5       ///< Step by Step Status
//@}

/**
    @name media playing EVENT ID

    Callback ID of media player.
*/
//@{
#define MEDIAPLAY_EVENT_FINISH            1   ///< Finish
#define MEDIAPLAY_EVENT_ONE_SECOND        2   ///< One second arrives
#define MEDIAPLAY_EVENT_ONE_VIDEOFRAME    3   ///< Decode one video-frame
#define MEDIAPLAY_EVENT_START             4   ///< Start mjpgMov playback
#define MEDIAPLAY_EVENT_STOPATEND         5   ///< Not Used
//#NT#2009/12/01#Meg Lin# -begin
//#NT#fixbug: decode error, end
#define MEDIAPLAY_EVENT_VID_ERROR         6   ///< decode video frame error
//#NT#2009/12/01#Meg Lin# -end
#define MEDIAPLAY_EVENT_ONE_DISPLAYFRAME  7   ///< One display frame
#define MEDIAPLAY_EVENT_MEDIAINFO_READY   8   ///< media information ready
#define MEDIAPLAY_EVENT_CLOSE             9   ///< media close
#define MEDIAPLAY_EVENT_PAUSE             10  ///< media pause
#define MEDIAPLAY_EVENT_PLAYING           11  ///< media playing
#define MEDIAPLAY_EVENT_CURR_VIDFRAME     12  ///< current video frame stream data ready
#define MEDIAPLAY_EVENT_CURR_AUDFRAME     13  ///< current audio frame stream data ready
#define MEDIAPLAY_EVENT_FILE_READ_ERROR   14  ///< file read error
//@}


//#NT#2012/08/27#Meg Lin -begin
/**
    @name type of playing direction

    Type of playing direction
*/
//@{
#define MEDIAPLAY_DIR_FORWARD   1             ///< Forward Direction
#define MEDIAPLAY_DIR_BACKWARD  2             ///< Backward Direction
//@}
//#NT#2012/08/27#Meg Lin -end

/**
    Speed type definition

    Media play speed definition

    @note for MediaPlay_SetSpeed()
*/
typedef enum
{
    MEDIAPLAY_SPEED_NORMAL  = 0x0,       ///< normal speed
    //#NT#2013/03/25#Calvin Chang#Support speed up level (trigger video decode by timer) -begin
    MEDIAPLAY_SPEED_2X      = 0x02,      ///< 2x   speed, trigger by timer (Play 2*I-frame number in second)
    MEDIAPLAY_SPEED_4X      = 0x04,      ///< 4x   speed, trigger by timer (Play 4*I-frame number in second)
    MEDIAPLAY_SPEED_8X      = 0x08,      ///< 8x   speed, trigger by timer (Play 8*I-frame number in second)
    MEDIAPLAY_SPEED_16X     = 0x10,      ///< 16x  speed, trigger by timer (Play 8*I-frame number & skip 1 I-frames in second)
    MEDIAPLAY_SPEED_32X     = 0x20,      ///< 32x  speed, trigger by timer (Play 8*I-frame number & skip 3 I-frames in second)
    MEDIAPLAY_SPEED_64X     = 0x40,      ///< 64x  speed, trigger by timer (Play 8*I-frame number & skip 7 I-frames in second)
    //#NT#2013/03/25#Calvin Chang -end
    MEDIAPLAY_SPEED_15X     = 0x21,      ///< 15x   speed, trigger 15/1  = 15X
    MEDIAPLAY_SPEED_7_5X    = 0x22,      ///< 7.5x  speed, trigger 15/2  = 7.5X
    MEDIAPLAY_SPEED_5X      = 0x23,      ///< 5x    speed, trigger 15/3  = 5X
    MEDIAPLAY_SPEED_3_75X   = 0x24,      ///< 3.75x speed, trigger 15/4  = 3.75X
    MEDIAPLAY_SPEED_3X      = 0x25,      ///< 3x    speed, trigger 15/5  = 3X
    MEDIAPLAY_SPEED_2_5X    = 0x26,      ///< 2.5x  speed, trigger 15/6  = 2.5X
    MEDIAPLAY_SPEED_1_5X    = 0x2a,      ///< 1.5x  speed, trigger 15/10 = 1.5X
    MEDIAPLAY_SPEED_30X     = 0x1E,      ///< 30x   speed, trigger 30/1  = 30X
    MEDIAPLAY_SPEED_45X     = 0x2D,      ///< 45x   speed, trigger 45/1  = 45X
    MEDIAPLAY_SPEED_60X     = 0x5A,      ///< 60x   speed, trigger 60/1  = 60X
    ENUM_DUMMY4WORD(MEDIAPLAY_SPEEDUP_TYPE)
} MEDIAPLAY_SPEEDUP_TYPE;


/**
    Struture of Video Decoder

    Input structure for media play open.

    @note for MediaPlay_Open()
*/
typedef struct _MEDIAPLAY_OBJ {
    UINT32      uiMemAddr;                          ///<[in] Data buffer starting address
    UINT32      uiMemSize;                          ///<[in] Data buffer size
    void        (*CallBackFunc)(UINT32 event_id);   ///<[in] Callback function
    //#NT#2012/07/17#Calvin Chang#Porting New FST System -begin
    FST_FILE    hActMediafilehdl;                   ///<[in] Opened Active Media File Handler
    //#NT#2012/07/17#Calvin Chang -end
    BOOL        bDisableAudio;                      ///<[in] Force to disable audio
} MEDIAPLAY_OBJ;


/**
    Struture of Video/Audio Frame position and size

    Struture of Video/Audio Frame position and size

*/
typedef struct _MEDIAPLAY_FRAMEPOS {
    UINT32 Addr;
    UINT32 Size;
} MEDIAPLAY_FARMEPOS;


/**
     Install MediaPlay task, flag and semaphore id.

     Install media play task, flag and semaphore id.

     @return void
*/
extern void     MediaPlay_InstallID(void) _SECTION(".kercfg_text");

/**
    @name   Media player controlling API
*/
//@{

/**
    Open media player task.

    Open media player task. Prepare to play media files.

    @param[in] pMediaPlayObj    the media playing object.

    @return
     - @b E_OK:     open successfully.
     - @b E_SYS:    media player task is already open.
     - @b E_PAR:    invalid input parameters.
*/
extern ER       MediaPlay_Open(MEDIAPLAY_OBJ    *pMediaPlayObj);

/**
    Close media player.

    Stop playing and close media player.

    @return
     - @b E_OK:     close successfully.
     - @b E_SYS:    media player task is closed.
*/
extern ER       MediaPlay_Close(void);

/**
    Start media playing and Set Speed.

    Start the media playing and set the speed, the direction and audio volume. If media player is not open, do nothing.

    @param[in] ucSpeedUpLevel   the speed up level type.
    @param[in] dir              the playing direction.

    @return void
*/
extern void     MediaPlay_SetSpeed(MEDIAPLAY_SPEEDUP_TYPE ucSpeedUpLevel, UINT32 dir);

/**
    Pause media playing.

    Pause media playing. If media player is not open, do nothing.

    @return void
*/
extern void     MediaPlay_Pause(void);

/**
    Stop media playing.

    Stop media playing. If media player is not open, do nothing.

    @return void
*/
extern void     MediaPlay_Stop(void);

//MediaPlayRecovery.c
/**
    Video File Recovery Function API

    File recovery API for power-cut porection.

    @param[in] pFilePath   video file full path name
    @param[in] memAddr     memory address
    @param[in] size        memory size
    @return
     - @b E_OK:     Recover the video file successfully.
     - @b E_SYS:    Recovery function is failed.
*/
extern ER       MediaPlay_FileRecovery(char *pFilePath, UINT32 memAddr, UINT32 size);

/**
    Get now playing seconds of the media file.

    Get now playing seconds of the media file.

    @return now playing seconds of the media file.
*/
extern UINT32   MediaPlay_GetNowSecond(void);

/**
    Get total playing times in seconds of the media file.

    Get total playing times in seconds of the media file..

    @return total playing times in seconds of the media file
*/
extern UINT32   MediaPlay_GetTotalSecs(void);

/**
    Get now playing seconds of the media file.

    Get now playing seconds of the media file.

    @return now playing seconds of the media file
*/
extern UINT32   MediaPlay_GetCurrPlaySecs(void);

/**
    Get now playing video frame of the media file.

    Get now playing video frame of the media file.

    @return now playing video frame of the media file
*/
extern UINT32 MediaPlay_GetCurrFrame(void);

/**
    Get total video frames of the media file.

    Get total video frames of the media file after header is parsed.

    @return total video frames of the media file.
*/
extern UINT32   MediaPlay_GetTotalVideoFrame(void);

/**
    Get video frames rate.

    Get video frames rate of the media file after header is parsed.

    @return total video frames rate.
*/
extern UINT32   MediaPlay_GetVideoFrameRate(void);


//#NT#2012/04/19#Calvin Chang#Support System APIs -begin
/**
    Enter Sleep mode

    Enter Sleep mode

    @return void
*/
extern void     MediaPlay_Sleep(void);

/**
    Leave Sleep mode

    Leave Sleep mode

    @return void
*/
extern void     MediaPlay_Wakeup(void);

/**
    Pause DMA

    Pause DMA

    @return void
*/
extern void     MediaPlay_PauseDMA(void);

/**
    Resume DMA

    Resume DMA

    @return void
*/
extern void     MediaPlay_ResumeDMA(void);
//#NT#2012/04/19#Calvin Chang -end
//#NT#2012/06/20#Calvin Chang#Support TV Flow APIs -begin
/**
    Pause Display

    Pause Display

    @return void
*/
extern void     MediaPlay_PauseDisp(BOOL bWaitFinish);

/**
    Resume Display

    Resume Display

    @return void
*/
extern void     MediaPlay_ResumeDisp(BOOL bWaitFinish);
//#NT#2012/06/20#Calvin Chang -end

/**
    Wait player ready after MediaPlay_Play().

    Wait player ready after MediaPlay_Play().

    @return
     - @b E_OK:     player is ready.
     - @b E_SYS:    media player task is not open or not media files.
     - @b E_TMOUT:  something error.
*/
extern ER       MediaPlay_WaitReady(void);

/**
    Set Audio Volume.

    Set audio volume level.

    @param[in] vol              the audio volume.

    @return void
*/
extern void     MediaPlay_SetAudVolume(UINT32 vol);


/**
    Get Audio Volume.

    Get audio volume level.

    @return [out] vol              the audio volume.
*/
extern UINT32   MediaPlay_GetAudVolume(void);


/**
    Restart to 1st frame.

    To restart to 1st frame. This function is to restart and play UI flow.

    @return void
*/
extern void     MediaPlay_RestartTo1stFrame(void);

/**
    Jump to video frame to specific frame.

    Jump to video frame to specific frame.

    @param [in]  frameIndex: Frame of named index number to playback.

    @return jump I-frame index number
*/
extern UINT32     MediaPlay_JumpVideoToSpecificFrame(UINT32 frameIndex);

/**
    Decode and Dump 1 GOP Raw Data

    Decode and dump 1 GOP raw data.

    @param [in]  frameIndex: Frame of named index number to playback.

    @return jump I-frame index number
*/
extern UINT32 MediaPlay_DecodeAndDumpOneGopRawData(UINT32 frameIndex);


/**
     Set the configuration index of Media Play Application

     @param[in] outDevConfigIdx   index of audio configuration device.


     @return    None
*/
extern void     MediaPlay_SetOutDevConfigIdx(UINT32 outDevConfigIdx);


/**
    Get Media Information.

    Get Video/Audio Media Information.

    @return MEDIA_FIRST_INFO pointer
*/
extern MEDIA_FIRST_INFO* MediaPlay_GetMediaInfo(void);

/**
    Get This Video Frame Offset.

    Get This Video Frame Offset.

    @param  [in] offset    buffer start address of BStream read from file
    @param  [in] framenum  frame of named index number to playback

    @return void
*/
extern void     MediaPlay_GetThisVideoFrameOffset(UINT32 *offset, UINT32 framenum);


/**
    Enable Get Display Frame YUV buffer event

    Enable Get Display Frame YUV buffer event (MEDIAPLAY_EVENT_ONE_DISPLAYFRAME)

    @param  [in] enable    if ture, callback the event MEDIAPLAY_EVENT_ONE_DISPLAYFRAME

    @return void
*/
extern void     MediaPlay_SetOneDispFrameEvent(BOOL enable);


/**
    Get Now Display Frame YUV Address

    Get Now Display Frame YUV Address

    @param  [in] py: Y plane address pointer
    @param  [in] pu: U plane address pointer
    @param  [in] pv: V plane address pointer

    @return void
*/
extern void     MediaPlay_GetNowDisplayFrame(UINT32 *py, UINT32 *pu, UINT32 *pv);

/**
    Get Display Scale Buffer

    Get Display Scale Buffer

    @param  [in] Addr   address pointer
    @param  [in] Size   size pointer

    @return void
*/
extern void MediaPlay_GetDispScaleBuffer(UINT32 *Addr, UINT32 *Size);

/**
    Get Display Ratio Scale Buffer

    Get Display Ratio Scale Buffer

    @param  [in] Addr   address pointer
    @param  [in] Size   size pointer

    @return void
*/
extern void MediaPlay_GetDispRatioScaleBuffer(UINT32 * Addr, UINT32 * Size);

/**
    Play Forward Step by Step

    Play Forward Step by Step

    @param  [in] MEDIAPLAY_SPEEDUP_TYPE Speed Level

    @return void
*/
extern void     MediaPlay_FWDStep(MEDIAPLAY_SPEEDUP_TYPE ucSpeedUpLevel);

/**
    Play Backward Step by Step

    Play Backward Step by Step

    @param  [in] MEDIAPLAY_SPEEDUP_TYPE Speed Level

    @return void
*/
extern void     MediaPlay_BWDStep(MEDIAPLAY_SPEEDUP_TYPE ucSpeedUpLevel);

/**
    Play Forward One Step by Step

    Play Forward Step by Step

    @return void
*/
extern void     MediaPlay_FWDOneStep(void);

/**
    Play Backward One Step by Step

    Play Backward One Step by Step

    @return void
*/
extern void     MediaPlay_BWDOneStep(void);

/**
    Get the Current Video Frame Data Address and Size

    Get the current video frame data address and size

    param  [in] VFrmInfo: return video frame position and size (MEDIAPLAY_FARMEPOS)

    @return void
*/
extern void   MediaPlay_GetVideoFrameData(MEDIAPLAY_FARMEPOS *VFrmPos);

/**
    Get the Current Audio Frame Data Address and Size

    Get the current audio frame data address and size

    param  [in] AFrmInfo: return audio frame data addresss and size (MEDIAPLAY_FARMEPOS)

    @return void
*/
extern void   MediaPlay_GetAudioFrameData(MEDIAPLAY_FARMEPOS *AFrmPos);

/**
    Enable Get Video/Audio Frame Stream Data event

    Enable Get Video/Audio Frame Stream Data event (MEDIAPLAY_EVENT_CURR_VIDFRAME/MEDIAPLAY_EVENT_CURR_AUDFRAME)

    @param  [in] enable: If ture, callback the event MEDIAPLAY_EVENT_CURR_VIDFRAME/MEDIAPLAY_EVENT_CURR_AUDFRAME

    @return void
*/
extern void   MediaPlay_SetVAFrameStrmDataEvent(BOOL enable);

/**
    Get create/modification time in MOV file

    Get create/modification time in MOV file

    param  [in] *pctime: create time pointer
    param  [in] *pmtime: modification time pointer

    @return void
*/
extern void MediaPlay_GetCreateModifyTime(UINT32 *pctime, UINT32 *pmtime);


extern UINT32 MediaPlay_DumpDataStatus(UINT32* pData);

/**
    Enable Audio Resample Function

    Enable audio resample function

    @param  [in] enable: If ture, enable audio resample function

    @return void
*/
extern void MediaPlay_EnableAudioResample(BOOL enable);


//@}

//*****[Legacy Structure/API Definition ]******
typedef struct _MEDIA_GET1STV
{
    UINT32 addr;                 ///<[in] File addr (at least 8 bytes)
    UINT32 size;                 ///<[in] Free size
    UINT32 validType;            ///<[in] MEDIA_FILEFORMAT_MOV or others
    //#NT#2012/07/17#Calvin Chang#Porting New FST System -begin
    FST_FILE hActMediafilehdl;   ///<[in] Opened Active Media File Handler
    //#NT#2012/07/17#Calvin Chang -end
} MEDIA_GET1STV;

typedef struct
{
    UINT32 uiNowWid;    ///<[in] ide buffer width
    UINT32 uiNowHei;    ///<[in] ide buffer height
    UINT32 uiNowFormat; ///<[out] ide format, MEDIAPLAY_VID_422 or others
    UINT32 uiIdeWid;    ///<[in] ide window width
    UINT32 uiIdeHei;    ///<[in] ide window height
} MEDIALAY_FBINFO;

extern ER       MediaPlay_FWD(void);
extern ER       MediaPlay_BWD(void);
extern void     MediaPlay_Show1stVideo(MEDIA_GET1STV *pinfo, UINT32 handle, MEDIA_FIRST_INFO *ptr);
extern ER       MediaPlay_SpeedUp(MEDIAPLAY_SPEEDUP_TYPE ucSpeedUpLevel);
extern ER       MediaPlay_WaitFFBW1XFinish(void);
extern ER       MediaPlay_WaitStepFinish(void);
extern ER       MediaPlay_SetDecode1stParam(MEDIA_PLAY_1ST_INFO *ptr);
extern ER       MediaPlay_GetDecode1stParam(MEDIA_PLAY_1ST_INFO *ptr);
extern void     MediaPlay_SetVideoDecObj(VIDEO_DECODER *ptr);

extern ER       MediaPlay_SetPlayFile(char *path);
extern ER       MediaPlay_Play(MEDIALAY_FBINFO *pFbinfo);
extern UINT32   MediaPlay_GetStatus(void);
extern void     MediaPlay_SetVideoWaitFSDataOK(UINT8 value);//2010/09/29 Meg Lin
extern void     MediaPlayVideo_Step(void);

//@}

#endif//_MEDIAPLAYAPI_H


