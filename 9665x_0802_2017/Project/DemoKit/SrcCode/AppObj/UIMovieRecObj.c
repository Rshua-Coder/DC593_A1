#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "debug.h"
#include "UIMovieObj.h"
#include "AppLib.h"
#include "MovieInterface.h"
#include "PrjCfg.h"
#if(WIFI_AP_FUNC==ENABLE)
#include "RtspNvtApi.h"
#endif
#include "NvtSystem.h"

#define __MODULE__          UIMovieRecObj
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

#define UIMovRecObj_LibStartRec(x)                  MovRec_Record(x)
#define UIMovRecObj_LibStopRec(x)                   MovRec_Stop(x)
#define UIMovRecObj_LibDisk2Second(x)               MovRec_Disk2SecondWithType(MOVREC_COUNTTYPE_FS,0)
#define UIMovRecObj_LibSetAudioVolume(x)            MovRec_SetAudioVolume(x)

#if (_UIMOVOBJ_DBG_LVL_ > _UIMOVOBJ_DBG_CHK_)

void UIMovRecObj_DumpInitParam(UINT32 paramAddr)
{
}

#else

#define UIMovRecObj_DumpInitParam(x)

#endif


INT32 UIMovRecObj_Close(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMovRecObj_StartRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    MovRec_Disk2SecondWithType(MOVREC_COUNTTYPE_FS,0);
    UIMovRecObj_LibStartRec();
    return NVTEVT_CONSUME;
}
INT32 UIMovRecObj_StopRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMovRecObj_LibStopRec(MOVREC_WAIT_END);
    return NVTEVT_CONSUME;
}
INT32 UIMovRecObj_StartRtsp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if(WIFI_AP_FUNC==ENABLE)
    #if 0 // MEDIAREC_VER_3_0, File type must be MP4 + AAC.
    RTSPNVT_OPEN Open={0};
    Open.uiApiVer = RTSPNVT_API_VERSION;
    Open.Type = RTSPNVT_TYPE_MEDIA_REC_WITH_AUD;
    Open.uiPortNum = 554;
    Open.uiTypeOfService = RTSPNVT_IPTOS_THROUGHPUT;
    RtspNvt_Open(&Open);
    DscMovie_RegRTSPSetBSCB(RtspNvt_RTSPSetBS);
    #else
    RTSPNVT_OPEN Open={0};
    Open.uiApiVer = RTSPNVT_API_VERSION;
    Open.Type = RTSPNVT_TYPE_MEDIA_REC;
    Open.uiPortNum = 554;
    Open.uiTypeOfService = RTSPNVT_IPTOS_THROUGHPUT;
    RtspNvt_Open(&Open);
    DscMovie_RegRTSPSetBSCB(RtspNvt_RTSPSetBS);
    #endif
#endif
    return NVTEVT_CONSUME;
}
INT32 UIMovRecObj_StopRtsp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if(WIFI_AP_FUNC==ENABLE)
    DscMovie_RegRTSPSetBSCB(NULL);
    RtspNvt_Close();
#endif
    return NVTEVT_CONSUME;
}
UINT32 UIMovRecObj_GetData(RECMOVIE_GET_DATATYPE dataType)
{
    UINT32 retV = 0;

    switch(dataType)
    {
        case RECMOVIE_MAXSECOND:
            MovRec_SetDisk2SecRestriction(MEDIAREC_RESTR_INDEXBUF, 0);
            MovRec_SetDisk2SecRestriction(MEDIAREC_RESTR_FSLIMIT, 0);
            retV = UIMovRecObj_LibDisk2Second();
            DBG_IND("max sec %d\r\n", retV);
            break;

        default:
            DBG_IND("not support type %d\r\n", dataType);
            break;
    }

    return retV;
}
void UIMovRecObj_WaitReady(void)
{
}
void UIMovRecObj_SetData(RECMOVIE_SET_DATATYPE dataType, UINT32 data)
{
    #if 0
    AUDIO_DEVICE_OBJ AudioDevObj;
    BOOL bAudioALCEn = TRUE;
    #endif

    switch (dataType)
    {
    case RECMOVIE_AUD_VOLUME:

        #if 0
        UIMovRecObj_LibSetAudioVolume(data);
        #else // No matter Media 2.0 or 3.0. Use this API to tune audio volume.
        SMediaRec_SetAudioVolume(data);
        #endif
        #if 0
        if (data == RECMOVIE_AUD_OFF)
            bAudioALCEn = FALSE;

        if (aud_getLockStatus() == TASK_LOCKED)
        {
            aud_setFeature(AUDIO_FEATURE_ALC, bAudioALCEn);
            aud_setGain(data);
        }
        else
        {
            aud_getDeviceObject(&AudioDevObj);
            aud_open(&AudioDevObj);
            aud_setFeature(AUDIO_FEATURE_ALC, bAudioALCEn);
            aud_setGain(data);
            aud_close();
        }
        #endif
        break;

    default:
        break;
    }
}

////////////////////////////////////////////////////////////

EVENT_ENTRY UIMovieRecObjCmdMap[] =
{
    {NVTEVT_CLOSE_REC_MOVIE,                UIMovRecObj_Close},
    {NVTEVT_START_REC_MOVIE,                UIMovRecObj_StartRec},
    {NVTEVT_STOP_REC_MOVIE,                 UIMovRecObj_StopRec},
    {NVTEVT_START_RTSP,                     UIMovRecObj_StartRtsp},
    {NVTEVT_STOP_RTSP,                      UIMovRecObj_StopRtsp},

    {NVTEVT_NULL,                           0},  //End of Command Map
};

CREATE_APP(UIMovieRecObj,APP_SETUP)

//#NT#2010/09/16#Lily Kao -end

