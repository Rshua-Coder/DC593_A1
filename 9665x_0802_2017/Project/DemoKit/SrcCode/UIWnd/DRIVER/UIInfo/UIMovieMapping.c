#include "MovieInterface.h"
#include "UIMovieObj.h"
#include "UIMovieInfo.h"
#include "audio.h"
#include "SysMain.h"

#define __MODULE__          UIMovieMapping
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

typedef struct
{
    UINT32  uiWidth;
    UINT32  uiHeight;
    UINT32  uiVidFrameRate;
    UINT32  uiTargetBitrate;
    UINT32  uiDispAspectRatio;
} MOVIE_SIZE_PARAM;

//Supporting FR 30 & 15
//static UINT32 uiMovieFRTable[2]={30,15};

static MOVIE_SIZE_PARAM g_MovieRecSizeTable[MOVIE_SIZE_ID_MAX] =
{
#if 1 // real sensor
    #if (_SENSORLIB_ == _SENSORLIB_VIRTUAL_ && _SENSORLIB2_ == _SENSORLIB2_CMOS_TVP5150_)
    { 720,   480,   30,     400*1024,   VIDENC_DAR_DEFAULT  },  // 480P,  30fps,  400KB/sec
    { 720,   480,   30,     400*1024,   VIDENC_DAR_DEFAULT  },  // 480P,  30fps,  400KB/sec
    { 720,   480,   30,     400*1024,   VIDENC_DAR_DEFAULT  },  // 480P,  30fps,  400KB/sec
    { 720,   480,   30,     400*1024,   VIDENC_DAR_DEFAULT  },  // 480P,  30fps,  400KB/sec
    #else
	{1920,	1080,	30, 	1200*1024,	VIDENC_DAR_DEFAULT	},	// 1080P, 30fps,  1200KB/sec
    {1280,  720,    30,     800*1024,   VIDENC_DAR_DEFAULT  },  // 720P,  60fps,  1600KB/sec
    { 848,  480,    30,     250*1024,   VIDENC_DAR_DEFAULT  },  // WVGA,  60fps,  800KB/sec
    { 640,  480,    30,     200*1024,   VIDENC_DAR_DEFAULT  },  // VGA,   120fps, 1200KB/sec
//  {1440,  1080,   30,     1450*1024,  VIDENC_DAR_16_9     },  // 1080P, 30fps,  1800KB/sec
    //{ 320,  240,    120,    600*1024,   VIDENC_DAR_DEFAULT  }   // QVGA,  120fps, 600KB/sec
    #endif
#else // FPGA
    {1920,  1080,   2,      120*1024,   VIDENC_DAR_DEFAULT  },  // 1080P, 2fps,   120KB/sec
    {1280,  720,    2,      53*1024,    VIDENC_DAR_DEFAULT  },  // 720P,  2fps,   53KB/sec
    { 854,  480,    3,      54*1024,    VIDENC_DAR_DEFAULT  },  // WVGA,  3fps,   54KB/sec
    { 640,  480,    3,      40*1024,    VIDENC_DAR_DEFAULT  },  // VGA,   3fps,   40KB/sec
    { 320,  240,    3,      20*1024,    VIDENC_DAR_DEFAULT  }   // QVGA,  3fps,   20KB/sec
#endif
};

// 2-path image source
//in wifi mode path 1 for movie size,path 2 for rtsp recording
#if 0
static MOVIE_SIZE_PARAM g_MovieSizeTable_2p[2][MOVIE_SIZE_ID_MAX] =
{
    {
    {1440,  1080,   30,     1350*1024,  VIDENC_DAR_16_9     },  // 1080P, 30fps,  1350KB/sec, DAR 16:9
    {1280,  720,    30,     800*1024,   VIDENC_DAR_DEFAULT  },  // 720P,  60fps,  1600KB/sec
    { 848,  480,    30,     400*1024,   VIDENC_DAR_DEFAULT  },  // WVGA,  60fps,  800KB/sec
    { 640,  480,    30,     400*1024,   VIDENC_DAR_DEFAULT  },  // VGA,   120fps, 1200KB/sec
    },

    {
    {1280,   720,   30,     800*1024,   VIDENC_DAR_DEFAULT  },  // 720P,  30fps,   800KB/sec, DAR default
    //{ 960,   720,   30,      600*1024,  VIDENC_DAR_16_9     },  // 720P,  30fps,   600KB/sec, DAR 16:9
    {1280,   720,   30,     800*1024,   VIDENC_DAR_DEFAULT  },  // 720P,  30fps,   800KB/sec, DAR default
    //{ 960,   720,   30,      600*1024,  VIDENC_DAR_16_9     },  // 720P,  30fps,   600KB/sec, DAR 16:9
    { 848,  480,    30,     400*1024,   VIDENC_DAR_DEFAULT  },  // WVGA,  60fps,  800KB/sec
    { 640,  480,    30,     400*1024,   VIDENC_DAR_DEFAULT  },  // VGA,   120fps, 1200KB/sec
    }
};
#else // Setup preview resolution for Wi-Fi APP.
static MOVIE_SIZE_PARAM g_WiFiMovieAppPreviewSizeTable[WIFI_MOVIE_APP_PREVIEW_SIZE_ID_MAX] =
{
    #if (_SENSORLIB_ == _SENSORLIB_VIRTUAL_ && _SENSORLIB2_ == _SENSORLIB2_CMOS_TVP5150_)
    { 720,   480,   30,     400*1024,   VIDENC_DAR_DEFAULT  },  // 480P,  30fps,  400KB/sec
    { 720,   480,   30,     400*1024,   VIDENC_DAR_DEFAULT  },  // 480P,  30fps,  400KB/sec
    { 720,   480,   30,     400*1024,   VIDENC_DAR_DEFAULT  },  // 480P,  30fps,  400KB/sec
    { 720,   480,   30,     400*1024,   VIDENC_DAR_DEFAULT  },  // 480P,  30fps,  400KB/sec
    { 720,   480,   30,     400*1024,   VIDENC_DAR_DEFAULT  },  // 480P,  30fps,  400KB/sec
    #else
    {1280,   720,   30,     540*1024,   VIDENC_DAR_DEFAULT  },  // 720P,  30fps,  540KB/sec
    { 848,   480,   30,     240*1024,   VIDENC_DAR_DEFAULT  },  // WVGA,  30fps,  240KB/sec
    { 640,   480,   30,     180*1024,   VIDENC_DAR_DEFAULT  },  // VGA,   30fps,  180KB/sec
    { 640,   360,   30,     135*1024,   VIDENC_DAR_DEFAULT  },  // 360P,  30fps,  135KB/sec
    { 320,   240,   30,      50*1024,   VIDENC_DAR_DEFAULT  }   // QVGA,  30fps,  50KB/sec
    #endif

};

#if(WIFI_AP_FUNC==ENABLE && MOVIE_LIVEVIEW==DUAL_REC_HTTP_LIVEVIEW)
#if (_SENSOR2_720P_)
static MOVIE_SIZE_PARAM g_MovieSizeTable_2p[2][MOVIE_SIZE_DUAL_ID_MAX] =
{
    {
    {1440,  1080,   30,     1350*1024,  VIDENC_DAR_16_9     },  // 1080P, 30fps,  1200KB/sec, DAR 16:9
    {1280,   720,   30,      800*1024,  VIDENC_DAR_DEFAULT  }   // 720P,  30fps,   800kB/sec, DAR default
    },

    {
    {1280,   720,   30,      800*1024,  VIDENC_DAR_DEFAULT  },  // 720P,  30fps,   800KB/sec, DAR default
    {1280,   720,   30,      800*1024,  VIDENC_DAR_DEFAULT  }   // 720P,  30fps,   800KB/sec, DAR default
    }
};
#else // sensor2 = D1
static MOVIE_SIZE_PARAM g_MovieSizeTable_2p[2][MOVIE_SIZE_DUAL_ID_MAX] =
{
    {
    {1920,  1080,   30,     1450*1024,  VIDENC_DAR_DEFAULT  },  // 1080P, 30fps,  1450KB/sec, DAR default
    {1280,   720,   30,      800*1024,  VIDENC_DAR_DEFAULT  }   // 720P,  30fps,   800kB/sec, DAR default
    },

    {
    {720,    480,   30,      400*1024,  VIDENC_DAR_DEFAULT  },  // 480P,  30fps,   400KB/sec, DAR default
    {720,    480,   30,      400*1024,  VIDENC_DAR_DEFAULT  }   // 480P,  30fps,   400KB/sec, DAR default
    }
};
#endif
#endif
#endif

//only use path 1 for rtsp liveview
#if 0
static MOVIE_SIZE_PARAM g_RTSPLiveviewSizeTable_2p[2][MOVIE_SIZE_ID_MAX] =
{
    {
    { 960,   720,   30,      600*1024,  VIDENC_DAR_16_9     },  // 720P,  30fps,   600KB/sec, DAR 16:9
    { 960,   720,   30,      600*1024,  VIDENC_DAR_16_9     },  // 720P,  30fps,   600KB/sec, DAR 16:9
    { 640,   360,   60,      450*1024,  VIDENC_DAR_DEFAULT  },  // 360P,  60fps,   450KB/sec, DAR default
    { 320,   240,  120,      300*1024,  VIDENC_DAR_DEFAULT  }   // QVGA,  120fps,  300KB/sec, DAR default
    },

    {
    { 960,   720,   30,      600*1024,  VIDENC_DAR_16_9     },  // 720P,  30fps,   600KB/sec, DAR 16:9
    { 960,   720,   30,      600*1024,  VIDENC_DAR_16_9     },  // 720P,  30fps,   600KB/sec, DAR 16:9
    { 640,   360,   60,      450*1024,  VIDENC_DAR_DEFAULT  },  // 360P,  60fps,   450KB/sec, DAR default
    { 320,   240,  120,      300*1024,  VIDENC_DAR_DEFAULT  }   // QVGA,  120fps,  300KB/sec, DAR default
    }
};
#endif

#if (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
// small clone movie table
static MOVIE_SIZE_PARAM g_SmallCloneMovieTable[MOVIE_SIZE_ID_MAX] =
{
    { 848,  480,    30,     400*1024,   VIDENC_DAR_DEFAULT  },  // WVGA,  30fps,  400KB/sec
    { 848,  480,    30,     400*1024,   VIDENC_DAR_DEFAULT  },  // WVGA,  30fps,  400KB/sec
    { 848,  480,    30,     400*1024,   VIDENC_DAR_DEFAULT  },  // WVGA,  30fps,  400KB/sec
    { 640,  480,    30,     300*1024,   VIDENC_DAR_DEFAULT  },  // VGA,   30fps,  300KB/sec
};
#endif

#if 0
static UINT32 uiMovieQuaTable[3]=
{
    MOVOBJ_REC_QUALITY_BEST,
    MOVOBJ_REC_QUALITY_NORMAL,
    MOVOBJ_REC_QUALITY_DRAFT
};
#endif

#if 0
UINT32 GetMovieQualityValue(UINT32 uiIndex)
{
    return uiMovieQuaTable[uiIndex];
}
#endif

UINT32 GetMovieSizeWidth(UINT32 uiIndex)
{
    return g_MovieRecSizeTable[uiIndex].uiWidth;
}
UINT32 GetMovieSizeHeight(UINT32 uiIndex)
{
    return g_MovieRecSizeTable[uiIndex].uiHeight;
}
UINT32 GetMovieFrameRate(UINT32 uiIndex)
{
    return g_MovieRecSizeTable[uiIndex].uiVidFrameRate;
}
UINT32 GetMovieTargetBitrate(UINT32 uiSize, UINT32 uiQuality)
{
    // [TO DO]: set different target bit rate according to quality setting (if necessary)
    return g_MovieRecSizeTable[uiSize].uiTargetBitrate;
}
void SetMovieTargetBitrate(UINT32 uiType, UINT32 uiSize, UINT32 uiBitRate)
{
    if(uiType == 0)
    {
        g_WiFiMovieAppPreviewSizeTable[uiSize].uiTargetBitrate = uiBitRate;
    }
    else
    {
        g_MovieRecSizeTable[uiSize].uiTargetBitrate = uiBitRate;
    }
}
UINT32 GetMovieDispAspectRatio(UINT32 uiIndex)
{
    return g_MovieRecSizeTable[uiIndex].uiDispAspectRatio;
}

// 2-path image source
UINT32 GetMovieSizeWidth_2p(UINT32 uiPath, UINT32 uiIndex)
{
#if(WIFI_AP_FUNC==ENABLE && MOVIE_LIVEVIEW==DUAL_REC_HTTP_LIVEVIEW)
    return g_MovieSizeTable_2p[uiPath][uiIndex].uiWidth;
#else
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        if(UI_GetData(FL_WIFI_MOVIE_FMT)==WIFI_RTSP_LIVEVIEW || uiPath == 1)
            return g_WiFiMovieAppPreviewSizeTable[UI_GetData(FL_WIFI_MOVIE_APP_PREVIEW_SIZE)].uiWidth;
        else
            return g_MovieRecSizeTable[uiIndex].uiWidth;
    }
    else
    {
#if (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
        if (uiPath == 1)
            return g_SmallCloneMovieTable[uiIndex].uiWidth;
        else
#endif
            return g_MovieRecSizeTable[uiIndex].uiWidth;
    }
#endif
}
UINT32 GetMovieSizeHeight_2p(UINT32 uiPath, UINT32 uiIndex)
{
#if(WIFI_AP_FUNC==ENABLE && MOVIE_LIVEVIEW==DUAL_REC_HTTP_LIVEVIEW)
    return g_MovieSizeTable_2p[uiPath][uiIndex].uiHeight;
#else
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        if(UI_GetData(FL_WIFI_MOVIE_FMT)==WIFI_RTSP_LIVEVIEW || uiPath == 1)
            return g_WiFiMovieAppPreviewSizeTable[UI_GetData(FL_WIFI_MOVIE_APP_PREVIEW_SIZE)].uiHeight;
        else
            return g_MovieRecSizeTable[uiIndex].uiHeight;
    }
    else
    {
#if (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
        if (uiPath == 1)
            return g_SmallCloneMovieTable[uiIndex].uiHeight;
        else
#endif
            return g_MovieRecSizeTable[uiIndex].uiHeight;
    }
#endif
}
UINT32 GetMovieFrameRate_2p(UINT32 uiPath, UINT32 uiIndex)
{
#if(WIFI_AP_FUNC==ENABLE && MOVIE_LIVEVIEW==DUAL_REC_HTTP_LIVEVIEW)
    return g_MovieSizeTable_2p[uiPath][uiIndex].uiVidFrameRate;
#else
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        if(UI_GetData(FL_WIFI_MOVIE_FMT)==WIFI_RTSP_LIVEVIEW || uiPath == 1)
            return g_WiFiMovieAppPreviewSizeTable[UI_GetData(FL_WIFI_MOVIE_APP_PREVIEW_SIZE)].uiVidFrameRate;
        else
            return g_MovieRecSizeTable[uiIndex].uiVidFrameRate;
    }
    else
    {
#if (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
        if (uiPath == 1)
            return g_SmallCloneMovieTable[uiIndex].uiVidFrameRate;
        else
#endif
            return g_MovieRecSizeTable[uiIndex].uiVidFrameRate;
    }
#endif
}
UINT32 GetMovieTargetBitrate_2p(UINT32 uiPath, UINT32 uiIndex)
{
#if(WIFI_AP_FUNC==ENABLE && MOVIE_LIVEVIEW==DUAL_REC_HTTP_LIVEVIEW)
    return g_MovieSizeTable_2p[uiPath][uiIndex].uiTargetBitrate;
#else
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        if(UI_GetData(FL_WIFI_MOVIE_FMT)==WIFI_RTSP_LIVEVIEW || uiPath == 1)
            return g_WiFiMovieAppPreviewSizeTable[UI_GetData(FL_WIFI_MOVIE_APP_PREVIEW_SIZE)].uiTargetBitrate;
        else
            return g_MovieRecSizeTable[uiIndex].uiTargetBitrate;
    }
    else
    {
#if (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
        if (uiPath == 1)
            return g_SmallCloneMovieTable[uiIndex].uiTargetBitrate;
        else
#endif
            return g_MovieRecSizeTable[uiIndex].uiTargetBitrate;
    }
#endif
}
UINT32 GetMovieDispAspectRatio_2p(UINT32 uiPath, UINT32 uiIndex)
{
#if(WIFI_AP_FUNC==ENABLE && MOVIE_LIVEVIEW==DUAL_REC_HTTP_LIVEVIEW)
    return g_MovieSizeTable_2p[uiPath][uiIndex].uiDispAspectRatio;
#else
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        if(UI_GetData(FL_WIFI_MOVIE_FMT)==WIFI_RTSP_LIVEVIEW || uiPath == 1)
            return g_WiFiMovieAppPreviewSizeTable[UI_GetData(FL_WIFI_MOVIE_APP_PREVIEW_SIZE)].uiDispAspectRatio;
        else
            return g_MovieRecSizeTable[uiIndex].uiDispAspectRatio;
    }
    else
    {
#if (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
        if (uiPath == 1)
            return g_SmallCloneMovieTable[uiIndex].uiDispAspectRatio;
        else
#endif
            return g_MovieRecSizeTable[uiIndex].uiDispAspectRatio;
    }
#endif
}

UINT32 GetMovieAudioVolumeValue(UINT32 uiIndex)
{
    UINT32 audVol = AUDIO_VOL_7;
    switch (uiIndex)
    {
        case MOVIE_AUDIO_VOL_MUTE:
            audVol = AUDIO_VOL_MUTE;
            break;
        case MOVIE_AUDIO_VOL_0:
            audVol = AUDIO_VOL_0;
            break;
        case MOVIE_AUDIO_VOL_1:
            audVol = AUDIO_VOL_1;
            break;
        case MOVIE_AUDIO_VOL_2:
            audVol = AUDIO_VOL_2;
            break;
        case MOVIE_AUDIO_VOL_3:
            audVol = AUDIO_VOL_3;
            break;
        case MOVIE_AUDIO_VOL_4:
            audVol = AUDIO_VOL_4;
            break;
        case MOVIE_AUDIO_VOL_5:
            audVol = AUDIO_VOL_5;
            break;
        case MOVIE_AUDIO_VOL_6:
            audVol = AUDIO_VOL_6;
            break;
        case MOVIE_AUDIO_VOL_7:
        default:
            audVol = AUDIO_VOL_7;
            break;
    }
    return audVol;
}

UINT32 Get_MovieFormatValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case WIFI_RTSP_LIVEVIEW:
            return MEDIAREC_LIVEVIEW;
        case WIFI_RTSP_REC:
            return MEDIAREC_AUD_VID_BOTH;
        default:
            return MEDIAREC_AUD_VID_BOTH;
    }
}
