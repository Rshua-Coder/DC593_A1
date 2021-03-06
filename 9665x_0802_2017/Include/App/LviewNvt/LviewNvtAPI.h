/////////////////////////////////////////////////////////////////
/*
    Copyright (c) 2013~  Novatek Microelectronics Corporation

    @file LviewNvtAPI.h

    @version

    @date

*//////////////////////////////////////////////////////////////////
#ifndef _LVIEWNVTAPI_H
#define _LVIEWNVTAPI_H

/**
    Video format for LviewNvt.
*/
typedef enum _LVIEW_VIDEO_FORMAT
{
    LVIEWNVT_VIDEO_FORMAT_422 = 0,               ///< YCbCr 422, UV packed.
    LVIEWNVT_VIDEO_FORMAT_420,                   ///< YCbCr 420, UV packed.
    ENUM_DUMMY4WORD(LVIEWNVT_VIDEO_FORMAT)
} LVIEWNVT_VIDEO_FORMAT;

/**
    Video frame rate.
*/
typedef enum _LVIEW_VIDEO_FRAMERATE
{
    LVIEWNVT_VIDEO_FRAMERATE_30 = 30,             ///< Video frame rate 30
    LVIEWNVT_VIDEO_FRAMERATE_24 = 24,             ///< Video frame rate 24
    LVIEWNVT_VIDEO_FRAMERATE_15 = 15,             ///< Video frame rate 15
    LVIEWNVT_VIDEO_FRAMERATE_12 = 12,             ///< Video frame rate 12
    ENUM_DUMMY4WORD(LVIEW_VIDEO_FRAMERATE)
} LVIEW_VIDEO_FRAMERATE;


/**
    Video Codec for LviewNvt.
*/
typedef enum _LVIEW_VIDEO_CODEC
{
    LVIEWNVT_VIDEO_CODEC_MJPG = 0,               ///< Video codec using MJPG
    LVIEWNVT_VIDEO_CODEC_H264,                   ///< Video codec using H264
    LVIEWNVT_VIDEO_CODEC_MAX,                    ///< Max count.
    ENUM_DUMMY4WORD(LVIEW_VIDEO_CODEC)
} LVIEW_VIDEO_CODEC;


/**
    Audio sampling rate
*/
typedef enum _LVIEWNVT_AUD_SR
{
    LVIEWNVT_AUD_SR_08K   = 8000,                 ///< audio sample rate 8  KHz
    LVIEWNVT_AUD_SR_16K   = 16000,                ///< audio sample rate 16 KHz
    LVIEWNVT_AUD_SR_24K   = 24000,                ///< audio sample rate 24 KHz
    LVIEWNVT_AUD_SR_32K   = 32000,                ///< audio sample rate 32 KHz
    ENUM_DUMMY4WORD(LVIEWNVT_AUD_SR)
} LVIEWNVT_AUD_SR;


/**
    Audio channel
*/
typedef enum _LVIEWNVT_AUD_CH
{
    LVIEWNVT_AUD_CH_LEFT,                         ///< Left
    LVIEWNVT_AUD_CH_RIGHT,                        ///< Right
    LVIEWNVT_AUD_CH_STEREO,                       ///< Stereo
    LVIEWNVT_AUD_CH_MONO,                         ///< Mono two channel
    ENUM_DUMMY4WORD(LVIEWNVT_AUD_CH)
} LVIEWNVT_AUD_CH;

/**
     @name audio bit per sample
*/
//@{
#define LVIEWNVT_AUD_16BIT_PER_SAMPLE       16      ///< audio 16 bit per sample
//@}

/**
     @name audio slice count per second
*/
//@{
#define LVIEWNVT_AUD_SLICE_COUNT_MIN        4       ///< audio min slice count per second.
#define LVIEWNVT_AUD_SLICE_COUNT_MAX      100       ///< audio max slice count per second.
//@}


/**
    Callback functions prototype that LviewNvt task used.

    These definitions are used in the structure LVIEWNVT_INFO and would be used during LviewNvt_Open() to specify the
    LviewNvt_VideoTsk needed callback function.
*/
//@{
typedef UINT32 (*LVIEWNVT_STARTVIDEOCB)(UINT32 videoWidth, UINT32 videoHeight, LVIEWNVT_VIDEO_FORMAT videoFmt); ///< Callback for starting video.
typedef UINT32 (*LVIEWNVT_GETVIDEOBUFCB)(UINT32 *yAddr, UINT32 *cbAddr, UINT32 *crAddr, UINT32 *lineOfsY);      ///< Callback for getting video buffer.
typedef UINT32 (*LVIEWNVT_STOPVIDEOCB)(void);                                                                   ///< Callback for stopping video.
typedef UINT32 (*LVIEWNVT_FRAMECB)(UINT32 frameIdx, MEM_RANGE* frameBuf);                                       ///< Callback for each video frame.
typedef UINT32 (*LVIEWNVT_AUDIOCB)(UINT32 audioIdx, MEM_RANGE* audioBuf);                                       ///< Callback for each audio slice.
typedef UINT32 (*LVIEWNVT_DRAWCB)(UINT32  yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineOfsY);                ///< Callback for draw image.
//@}

/**
    Information needed for LviewNvt module.

    This structure is used in LviewNvt_Open() to specify the LviewNvt_VideoTsk task needed information for normal working
    and user customization callback.
*/
typedef struct {

    UINT32                  workMemAdr;          ///< work buffer address
    UINT32                  workMemSize;         ///< work buffer size
    LVIEW_VIDEO_CODEC       videoCodec;          ///< Video Codec using. Default value is LVIEWNVT_VIDEO_CODEC_MIPG.
    UINT32                  targetSize;          ///< stream target size(Byte/Sec). Default value is 0x180000. (1.5MB)
    LVIEW_VIDEO_FRAMERATE   frameRate;           ///< streaming frame rate. Default value is 30.
    UINT32                  imgWidth;            ///< image width. Default value is 640.
    UINT32                  imgHeight;           ///< image height. Default value is 480.
    UINT32                  H2643DNRlevel;       ///< H.264 3DNR settings (0 to 7). 0 is disabled and 7 is strongest. Default value is 0.
    UINT32                  MJPGQueueFrameNum;   ///< Queue MJPG number , default value is 5.
    LVIEWNVT_STARTVIDEOCB   fpStartVideoCB;      ///< Callback function to start video
    LVIEWNVT_GETVIDEOBUFCB  fpGetVideoBufCB;     ///< Callback function to get video source buffer for encoding
    LVIEWNVT_STOPVIDEOCB    fpStopVideoCB;       ///< Callback function to stop video buffer
    LVIEWNVT_FRAMECB        fpFrameCB;           ///< Callback function of frame video data output
    BOOL                    isSupportAudio;      ///< If live view streaming support audio
    LVIEWNVT_AUD_SR         audioSampleRate;     ///< Audio sampling rate
    LVIEWNVT_AUD_CH         audioChannel;        ///< Audio channel
    UINT32                  audioBitPerSample;   ///< Audio bit per sample
    UINT32                  audioSliceCount;     ///< Audio slice count per second.
    LVIEWNVT_AUDIOCB        fpAudioCB;           ///< Callback function of audio data output
    LVIEWNVT_DRAWCB         fpDrawCB;            ///< Callback function for draw image on frame (ex: data stamp)
} LVIEWNVT_INFO;




/*-----------------------------------
    functions declare
-----------------------------------*/

/**
     Install LviewNvt flag and semaphore id.

*/
extern void       LviewNvt_InstallID(void) _SECTION(".kercfg_text");

/**
     Calculate LviewNvt required buffer size.

     Calculate LviewNvt required buffer size.

     @return the required buffer size.
*/
//extern UINT32     LviewNvt_CalcBuffSize(LVIEW_VIDEO_CODEC  videoCodec);

/**
    Open LviewNvt video task.

    This is the LviewNvt video task initial function.

    @param[in] pInfo  Init info for LviewNvt.
*/
extern INT32     LviewNvt_Open(LVIEWNVT_INFO *pInfo);

/**
    Close LviewNvt video task.

*/
extern void       LviewNvt_Close(void);

/*
    Start liveview data streaming.

    @return
         - @b E_OK: The operation is success.
         - @b E_SYS: has some error.
*/
extern INT32      LviewNvt_Start(void);

/*
    Stop liveview data streaming.

    @return
         - @b E_OK: The operation is success.
         - @b E_SYS: has some error.

*/
extern INT32      LviewNvt_Stop(void);


/*
    Get current liveview streaming JPG buffer address & size.

    @return
         - @b E_OK: The operation is success.
         - @b E_SYS: has some error.

*/
extern ER         LviewNvt_getCurDecJpgBuf(MEM_RANGE* pJpgBuf);

#endif  // _LVIEWVIDEOTSK_H

