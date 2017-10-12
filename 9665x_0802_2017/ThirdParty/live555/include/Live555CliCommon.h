#ifndef _LIVE555CLICOMMON_H
#define _LIVE555CLICOMMON_H

#define LIVE555CLI_INTERFACE_VER 0x17012412

#define LIVE555CLI_TMP_BUF_SIZE_VIDEO 0x100000
#define LIVE555CLI_TMP_BUF_SIZE_AUDIO 0x80000 

typedef enum _LIVE555CLI_FRM_CMD{
    LIVE555CLI_FRM_CMD_UNKNOWN = 0x00000000,    ///< reversed
    LIVE555CLI_FRM_CMD_FRAME   = 0x00000001,    ///< normal frame
    LIVE555CLI_FRM_CMD_PENDING = 0x00000002,    ///< not used
    LIVE555CLI_FRM_CMD_STOP    = 0x00000003,    ///< stop play
    LIVE555CLI_FRM_CMD_VIDEO_INFO = 0x00000004, ///< not used
    LIVE555CLI_FRM_CMD_AUDIO_INFO = 0x00000005, ///< not used
}LIVE555CLI_FRM_CMD;

typedef enum _LIVE555CLI_ICMD_IDX{
    LIVE555CLI_ICMD_IDX_UNKNOWN = 0,            ///< INPUT: NULL,                           OUTPUT: NULL
    LIVE555CLI_ICMD_IDX_OPEN_SESSION,           ///< INPUT: LIVE555CLI_STREAM_INFO          OUTPUT: NULL
    LIVE555CLI_ICMD_IDX_CLOSE_SESSION,          ///< INPUT: NULL                            OUTPUT: NULL
    LIVE555CLI_ICMD_IDX_SET_VIDEO,              ///< INPUT: LIVE555CLI_FRM                  OUTPUT: NULL
    LIVE555CLI_ICMD_IDX_SET_AUDIO,              ///< INPUT: LIVE555CLI_FRM                  OUTPUT: NULL
    LIVE555CLI_ICMD_IDX_PROCESS_END,            ///< INPUT: NULL                            OUTPUT: NULL
    LIVE555CLI_ICMD_IDX_MAX_COUNT,
}LIVE555CLI_ICMD_IDX;

typedef enum _LIVE555CLI_CODEC_VIDEO{
    LIVE555CLI_CODEC_VIDEO_UNKNOWN = 0,
    LIVE555CLI_CODEC_VIDEO_NONE,
    LIVE555CLI_CODEC_VIDEO_H264,
    LIVE555CLI_CODEC_VIDEO_MJPG,
	LIVE555CLI_CODEC_VIDEO_H265,
    LIVE555CLI_CODEC_VIDEO_MAX_COUNT,
}LIVE555CLI_CODEC_VIDEO;

typedef enum _LIVE555CLI_CODEC_AUDIO{
    LIVE555CLI_CODEC_AUDIO_UNKNOWN = 0,
    LIVE555CLI_CODEC_AUDIO_NONE,
    LIVE555CLI_CODEC_AUDIO_PCM,
    LIVE555CLI_CODEC_AUDIO_AAC,
    LIVE555CLI_CODEC_AUDIO_G711_ULAW,
    LIVE555CLI_CODEC_AUDIO_G711_ALAW,
    LIVE555CLI_CODEC_AUDIO_MAX_COUNT,
}LIVE555CLI_CODEC_AUDIO;

typedef struct _LIVE555CLI_ICMD {
    LIVE555CLI_ICMD_IDX CmdIdx;
    UINT32 uiInAddr;
    UINT32 uiInSize;
    UINT32 uiOutAddr;
    UINT32 uiOutSize;
    UINT32 uiResult;
} LIVE555CLI_ICMD;

typedef struct _LIVE555CLI_VIDEO_INFO{
    UINT32 uiMediaSrcID; //Always be '0'
    UINT32 uiCodec; //LIVE555CLI_CODEC_VIDEO
    UINT32 uiFps; //Not Used
    UINT32 uiTotoalFrame; //Not Used
	UINT8  pVPS[64];
	UINT32 uiSizeVPS;
    UINT8  pSPS[64];
    UINT32 uiSizeSPS;
    UINT8  pPPS[16];
    UINT32 uiSizePPS;
}LIVE555CLI_VIDEO_INFO;

typedef struct _LIVE555CLI_AUDIO_INFO{
    UINT32 uiMediaSrcID; //Always be '0'
    UINT32 uiCodec; //LIVE555CLI_CODEC_AUDIO
    UINT32 uiSamplePerSecond;
    UINT32 uiBitPerSample; //Not Used
    UINT32 uiChannelCnt;
    UINT32 uiTotalTimeInMs;
}LIVE555CLI_AUDIO_INFO;

typedef struct _LIVE555CLI_STREAM_INFO{
    LIVE555CLI_VIDEO_INFO vid;
    LIVE555CLI_AUDIO_INFO aud;
}LIVE555CLI_STREAM_INFO;

typedef struct _LIVE555CLI_FRM {
    UINT32 uiFrmCmd; //Always be '0'
    UINT32 uiPtsSec;
    UINT32 uiPtsUsec;
    UINT32 uiAddr;
    UINT32 uiSize;
}LIVE555CLI_FRM;

typedef void (*LIVE555CLI_FP_ICMD)(LIVE555CLI_ICMD* pCmd);

typedef struct _LIVE555CLI_CONFIG{
    UINT32 uiVersion;       // MUST BE LIVE555CLI_INTERFACE_VER
    LIVE555CLI_FP_ICMD fpCmd;  // Interface Command
    UINT32 uiWorkingAddr;
    UINT32 uiWorkingSize;
    UINT32 uiDisableVideo;
    UINT32 uiDisableAudio;
    char   szUrl[256];
}LIVE555CLI_CONFIG;

typedef struct _LIVE555CLI_IPC_MSG{
    long mtype;
    UINT32 uiIPC;           //LIVE555CLI_IPC
}LIVE555CLI_IPC_MSG;

#ifdef __cplusplus
extern "C"
{
#endif
    //for eCos650 Only
    extern INT32 Live555Cli_SetConfig(LIVE555CLI_CONFIG* pConfig);
    extern INT32 Live555Cli_Open(void);
    extern INT32 Live555Cli_Close(void);

    //for eCos660 Only
    extern INT32 Live555Cli_Open2(char* szCmdStr);
#ifdef __cplusplus
} //extern "C"
#endif

#endif
