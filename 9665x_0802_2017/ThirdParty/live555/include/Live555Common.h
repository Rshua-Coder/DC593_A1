#ifndef _LIVE555COMMON_H
#define _LIVE555COMMON_H

#define LIVE555_INTERFACE_VER 0x17012412

#define LIVE555_H264_TMP_BUF_HDR_SIZE 16 //reversed for RTP header - H264
#define LIVE555_H265_TMP_BUF_HDR_SIZE 16 //reversed for RTP header - H265
#define LIVE555_MJPG_TMP_BUF_HDR_SIZE 160 //reversed for RTP header - MJPG
#define LIVE555_MEDIA_SRC_MAX_CNT 16

/************************************************************************/
/* TOS                                                                  */
/************************************************************************/
#define LIVE555_IPTOS_LOWDELAY          0x10
#define LIVE555_IPTOS_THROUGHPUT        0x08
#define LIVE555_IPTOS_RELIABILITY       0x04
#define LIVE555_IPTOS_LOWCOST           0x02

/************************************************************************/
/* LIVE555_FRM_REP used to describe memory data.                        */
/************************************************************************/
typedef enum _LIVE555_FRM_CMD{
    LIVE555_FRM_CMD_UNKNOWN = 0x00000000,
    LIVE555_FRM_CMD_FRAME   = 0x00000001,
    LIVE555_FRM_CMD_PENDING = 0x00000002,
    LIVE555_FRM_CMD_STOP    = 0x00000003,
    LIVE555_FRM_CMD_VIDEO_INFO = 0x00000004,
    LIVE555_FRM_CMD_AUDIO_INFO = 0x00000005,
}LIVE555_FRM_CMD;

/************************************************************************/
/* Error Code for LIVE555_FP_LOCK_STRMID and LIVE555_FP_UNLOCK_STRMID   */
/************************************************************************/
#define LIVE555_ER_STRMID_OK 0
#define LIVE555_ER_STRMID_NO_RESOURCE ((UINT32)-1)
#define LIVE555_ER_STRMID_ONE_CONNECTION_ONLY ((UINT32)-2)

typedef enum _LIVE555_ICMD_IDX{
    LIVE555_ICMD_IDX_UNKNOWN = 0,           //INPUT: NULL,                          OUTPUT: NULL
    LIVE555_ICMD_IDX_PARSE_MEDIA_SRC_ID,    //INPUT: char[], (Path)                 OUTPUT: UINT32 (MediaSrcID)
    LIVE555_ICMD_IDX_OPEN_SESSION,          //INPUT: char[], (Path)                 OUTPUT: LIVE555_OPEN_SESSION_REP
    LIVE555_ICMD_IDX_CLOSE_SESSION,         //INPUT: UINT32, (MediaSrcID)           OUTPUT: NULL
    LIVE555_ICMD_IDX_SEEK,                  //INPUT: LIVE555_FRM_REQ                OUTPUT: UINT32
    LIVE555_ICMD_IDX_GET_VIDEO,             //INPUT: LIVE555_FRM_REQ                OUTPUT: LIVE555_FRM_REP
    LIVE555_ICMD_IDX_GET_AUDIO,             //INPUT: LIVE555_FRM_REQ                OUTPUT: LIVE555_FRM_REP
    LIVE555_ICMD_IDX_STRMID_LOCK,           //INPUT: UINT32  (MediaSrcID)           OUTPUT: UINT32 (StrmID)
    LIVE555_ICMD_IDX_STRMID_UNLOCK,         //INPUT: UINT32  (StrmID)               OUTPUT: NULL
    LIVE555_ICMD_IDX_SERVER_EVENT,          //INPUT: LIVE555_SERVER_EVENT           OUTPUT: NULL
    LIVE555_ICMD_IDX_PROCESS_END,           //INPUT: NULL                           OUTPUT: NULL
    LIVE555_ICMD_IDX_REQ_KEY_FRAME,         //INPUT: UINT32  (StrmID)               OUTPUT: NULL
    LIVE555_ICMD_IDX_REQ_FLUSH,             //INPUT: UINT32  (StrmID)               OUTPUT: NULL
    LIVE555_ICMD_IDX_MAX_COUNT,
}LIVE555_ICMD_IDX;

typedef enum _LIVE555_CODEC_VIDEO{
    LIVE555_CODEC_VIDEO_UNKNOWN = 0,
    LIVE555_CODEC_VIDEO_NONE,
    LIVE555_CODEC_VIDEO_H264,
    LIVE555_CODEC_VIDEO_MJPG,
	LIVE555_CODEC_VIDEO_H265,
    LIVE555_CODEC_VIDEO_MAX_COUNT,
}LIVE555_CODEC_VIDEO;

typedef enum _LIVE555_CODEC_AUDIO{
    LIVE555_CODEC_AUDIO_UNKNOWN = 0,
    LIVE555_CODEC_AUDIO_NONE,
    LIVE555_CODEC_AUDIO_PCM,
    LIVE555_CODEC_AUDIO_AAC,
    LIVE555_CODEC_AUDIO_G711_ULAW,
    LIVE555_CODEC_AUDIO_G711_ALAW,
    LIVE555_CODEC_AUDIO_MAX_COUNT,
}LIVE555_CODEC_AUDIO;

typedef struct _LIVE555_ICMD {
    LIVE555_ICMD_IDX CmdIdx;
    UINT32 uiInAddr;
    UINT32 uiInSize;
    UINT32 uiOutAddr;
    UINT32 uiOutSize;
    UINT32 uiResult;
} LIVE555_ICMD;

typedef struct _LIVE555_PATH {
    char Path[128];
} LIVE555_PATH;

typedef struct _LIVE555_VIDEO_INFO{
    UINT32 uiMediaSrcID;
    UINT32 uiCodec; //LIVE555_CODEC_VIDEO
    UINT32 uiFps;
    UINT32 uiTotoalFrame;
	UINT8  pVPS[64];
	UINT32 uiSizeVPS;
    UINT8  pSPS[64];
    UINT32 uiSizeSPS;
    UINT8  pPPS[16];
    UINT32 uiSizePPS;
}LIVE555_VIDEO_INFO;

typedef struct _LIVE555_AUDIO_INFO{
    UINT32 uiMediaSrcID;
    UINT32 uiCodec; //LIVE555_CODEC_AUDIO
    UINT32 uiSamplePerSecond;
    UINT32 uiBitPerSample;
    UINT32 uiChannelCnt;
    UINT32 uiTotalTimeInMs;
}LIVE555_AUDIO_INFO;

typedef struct _LIVE555_OPEN_SESSION_REP {
    LIVE555_VIDEO_INFO LvVdoInfo;
    LIVE555_AUDIO_INFO LvAudInfo;
}LIVE555_OPEN_SESSION_REP;

typedef struct _LIVE555_FRM_REQ {
    UINT32 uiStrmID;
    UINT32 uiFrameIdx;
} LIVE555_FRM_REQ;

typedef struct _LIVE555_FRM_REP_EXT_H264{
    UINT32 uiSvcTemporalId;
}LIVE555_FRM_REP_EXT_H264;

typedef struct _LIVE555_FRM_REP_EXT_MJPG{
    UINT32 uiReserved;
}LIVE555_FRM_REP_EXT_MJPG;

typedef struct _LIVE555_FRM_REP {
    UINT32 uiFrmCmd;
    UINT32 uiIndex;
    UINT32 uiAddr;
    UINT32 uiSize;
    union{
        LIVE555_FRM_REP_EXT_H264 H264;
        LIVE555_FRM_REP_EXT_MJPG MJPG;
    }Ext;
}LIVE555_FRM_REP;

typedef struct _LIVE555_BITRATE_REQ{
    UINT32 uiMediaSrcID;
    UINT32 uiStrmID;
    UINT32 uiBitrateLevel; //< (0 ~ 2). Use (Original_Bitrate * (3-BitrateLevel)) / 3;
}LIVE555_BITRATE_REQ;

typedef enum _LIVE555_SERVER_EVENT{
    LIVE555_SERVER_EVENT_OPTIONS,
    LIVE555_SERVER_EVENT_DESCRIBE,
    LIVE555_SERVER_EVENT_SETUP,
    LIVE555_SERVER_EVENT_TEARDOWN,
    LIVE555_SERVER_EVENT_PLAY,
    LIVE555_SERVER_EVENT_PAUSE,
    LIVE555_SERVER_EVENT_FRAME_START_I,
    LIVE555_SERVER_EVENT_FRAME_START_P,
    LIVE555_SERVER_EVENT_FRAME_END,
}LIVE555_SERVER_EVENT;

typedef enum _LIVE555_SERVER_TYPE{
    LIVE555_SERVER_TYPE_UNKNOWN,
    LIVE555_SERVER_TYPE_LIVEVIEW,
    LIVE555_SERVER_TYPE_PLAYBACK,
}LIVE555_SERVER_TYPE;

typedef void (*LIVE555_FP_ICMD)(LIVE555_ICMD* pCmd);

typedef struct _LIVE555_CONFIG{
    UINT32 uiVersion;       // MUST BE LIVE555_INTERFACE_VER
    LIVE555_FP_ICMD fpCmd;  // Interface Command
    UINT32 uiPortNum;       // Port Number, Normally is 554
    UINT32 uiSendBufSize;   // The Size of Sending socket, suggest 51200 Bytes
    UINT32 uiTypeOfService; // LIVE555_IPTOS_????
    LIVE555_SERVER_TYPE SeverType; //liveview or playback file
}LIVE555_CONFIG;

typedef struct _LIVE555_IPC_MSG{
    long mtype;
    UINT32 uiIPC;           //LIVE555_IPC
}LIVE555_IPC_MSG;

typedef enum _LIVE555_CLIENT_EVENT{
    LIVE555_CLIENT_EVENT_FAIL, //fail to open
    LIVE555_CLIENT_EVENT_DESC, //get video /audio description (sps/pps/channel/sample rate)
    LIVE555_CLIENT_EVENT_PLAY, //start playing
    LIVE555_CLIENT_EVENT_FRAME_VIDEO, //a video frame arrival
    LIVE555_CLIENT_EVENT_FRAME_AUDIO, //a audio frame arrival
}LIVE555_CLIENT_EVENT;

typedef void (*LIVE555_FP_CLIENT_EVENT)(LIVE555_CLIENT_EVENT client_event);

typedef struct _LIVE555_CLIENT_INFO{
    char*  url;
    LIVE555_FP_CLIENT_EVENT fpEvent;
}LIVE555_CLIENT_INFO;

#ifdef __cplusplus
extern "C"
{
#endif

    //for eCos650 Only
    extern INT32 Live555_SetConfig(LIVE555_CONFIG* pConfig);
    extern INT32 Live555_Open(void);
    extern INT32 Live555_Close(void);

    //for eCos660 Only
    extern INT32 Live555_Open2(char* szCmdStr);

    //for RTSP-Client (NOT SUPPORT)
    extern INT32 Live555_Client_Open(LIVE555_CLIENT_INFO* pInfo);
    extern INT32 Live555_Client_Close(void);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
