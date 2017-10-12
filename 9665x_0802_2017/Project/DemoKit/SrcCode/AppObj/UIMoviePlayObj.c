
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "debug.h"
#include "UIMovieObj.h"
#include "PlaybackTsk.h"
#include "AppLib.h"
#include "GxVideoFile.h"
#include "DCF.h"
//#NT#2012/08/30#Calvin Chang#Porting Media Framework on UIAPP-UIMoviePlayObj -begin
//#NT#2012/08/30#Calvin Chang -end

#if 1 // Use SMedia 3.0
#include "SMediaPlayAPI.h"
#define MediaPlay_Open(x)                        SMediaPlay_Open((PSMEDIAPLAY_USEROBJ)x)
#define MediaPlay_WaitReady(x)                   SMediaPlay_WaitReady(x)
#define MediaPlay_Close(x)                       SMediaPlay_Close(x)
#define MediaPlay_Pause(x)                       SMediaPlay_PausePlay(x)
#define MediaPlay_GetMediaInfo(x)                SMediaPlay_GetMediaInfo(x)
#define MediaPlay_GetCurrFrame(x)                SMediaPlay_GetCurrVideoFrame(x)
#define MediaPlay_GetNowDisplayFrame(x,y,z)      SMediaPlay_GetNowDisplayFrame(x,y,z)
#define MediaPlay_StartPlay(x,y)                 SMediaPlay_StartPlay(x,y)
#define MediaPlay_FWDOneStep(x)                  SMediaPlay_VideoOneStepPlay(SMEDIAPLAY_DIR_FORWARD)
#define MediaPlay_BWDOneStep(x)                  SMediaPlay_VideoOneStepPlay(SMEDIAPLAY_DIR_BACKWARD)
#define MediaPlay_GetVideoFrameRate(x)           SMediaPlay_GetVideoFrameRate(x)
#define MediaPlay_GetTotalVideoFrame(x)          SMediaPlay_GetTotalVideoFrame(x)
#define MediaPlay_GetCreateModifyTime(x,y)       SMediaPlay_GetCreateModifyTime(x,y)
#define MediaPlay_GetThisVideoFrameOffset(x,y)   SMediaPlay_GetThisVideoFrameOffset(x,y)
#define MediaPlay_JumpVideoToSpecificFrame(x)    SMediaPlay_JumpVideoToSpecFramePlay(x)
#define MediaPlay_RestartTo1stFrame(x)           SMediaPlay_Restart1stFrmPlay(x)
#define MediaPlay_SetOneDispFrameEvent(x)
#define MediaPlay_Video_Step(x)
#define MediaPlay_WaitStepFinish(x)
#define MediaPlay_SetAudVolume(x)                //SMediaPlay_SetAudVolume(x)
#define MediaPlay_GetCurrPlaySecs(x)             SMediaPlay_GetCurrPlaySecs(x)
#else // Use Media 2.0
#include "MediaPlayAPI.h"
#endif


GXVIDEO_INFO g_MovieInfo;
UINT32 UIMovObj_LibGetTotalSecs(void)
{
    PB_GetParam(PBPRMID_INFO_VDO, (UINT32 *)&g_MovieInfo);

    return (g_MovieInfo.uiToltalSecs);
}
UINT32 UIMovObj_LibGetEV(void)
{
    return 0;
}
UINT32 UIMovObj_LibGetVideoWidth(void)
{
    PB_GetParam(PBPRMID_INFO_VDO, (UINT32 *)&g_MovieInfo);

    return (g_MovieInfo.uiVidWidth);
}

INT32 UIMovObj_InitPlayMovie(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    INT32       retV = 0;

    //#NT#2012/08/30#Calvin Chang#Porting Media Framework on UIAPP-UIMoviePlayObj -begin
    static SMEDIAPLAY_USEROBJ SMediaPlayObj;

    DbgMsg_UIMovObjIO(("+%s:pN=0x%x\r\n",__func__,paramNum));
    if (paramNum == 0)
    {
        debug_err(("%s:paramenter wrong\r\n",__func__));
        return E_PAR;
    }

    // Close
    MediaPlay_Close();

    memcpy((void *)&SMediaPlayObj,(void *)*paramArray, sizeof(SMEDIAPLAY_USEROBJ));

    // Open
    MediaPlay_Open(&SMediaPlayObj);
    retV = MediaPlay_WaitReady();

    if (retV != E_OK)
    {
        debug_err(("UIMovObj_InitPlayMovie is Failed!!\r\n "));
        MediaPlay_Close();
        return retV;
    }
    //#NT#2012/08/30#Calvin Chang -end

    return retV;
}

INT32 UIMovObj_ClosePlayMovie(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2012/08/30#Calvin Chang#Porting Media Framework on UIAPP-UIMoviePlayObj -begin
    MediaPlay_Pause();
    MediaPlay_Close();
    //#NT#2012/08/30#Calvin Chang -end

    return NVTEVT_CONSUME;
}

INT32 UIMovObj_PausePlayMovie(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2012/08/30#Calvin Chang#Porting Media Framework on UIAPP-UIMoviePlayObj -begin
    MediaPlay_Pause();
    //#NT#2012/08/30#Calvin Chang -end

    return NVTEVT_CONSUME;
}

//#NT#2012/08/30#Calvin Chang#Porting Media Framework on UIAPP-UIMoviePlayObj -begin
INT32 UIMovObj_PlayMovie(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSpeedLevel, direction;

    if (paramNum != 2)
    {
        return NVTEVT_CONSUME;
    }

    uiSpeedLevel = paramArray[0];
    direction    = paramArray[1];

    MediaPlay_StartPlay(uiSpeedLevel, direction);

    return NVTEVT_CONSUME;
}
//#NT#2012/08/30#Calvin Chang -end

UINT32 UIMovObj_GetDataPlayMovie(PLAYMOVIE_GET_DATATYPE dataType)
{
    UINT32 retV = 0;
    switch(dataType)
    {
        case PLAYMOVIE_DIRID:
            retV = DCF_GetDBInfo(DCF_INFO_CUR_DIR_ID);
            break;
        case PLAYMOVIE_FILEID:
            retV = DCF_GetDBInfo(DCF_INFO_CUR_FILE_ID);
            break;
        case PLAYMOVIE_TOTAL_SECOND:
            retV = UIMovObj_LibGetTotalSecs();
            break;
        case PLAYMOVIE_EV:
            retV = UIMovObj_LibGetEV();
            break;
        case PLAYMOVIE_WIDTH:
            retV = UIMovObj_LibGetVideoWidth();
            break;
        case PLAYMOVIE_LOCKSTATUS:
            {
                UINT32 CurIndex,DirectoryID,FilenameID,uiThisFileFormat;
                CHAR   filePath[DCF_FULL_FILE_PATH_LEN];
                BOOL   ReturnValue;
                UINT8  uAttrib = 0;

                CurIndex = DCF_GetDBInfo(DCF_INFO_CUR_INDEX);
                DCF_GetObjInfo(CurIndex,&DirectoryID, &FilenameID, &uiThisFileFormat);
                DCF_GetObjPath(CurIndex,uiThisFileFormat, filePath);
                FileSys_GetAttrib(filePath,&uAttrib);
                if (M_IsReadOnly(uAttrib))
                {
                    ReturnValue = TRUE;
                }
                else
                {
                    ReturnValue = FALSE;
                }
                retV = ReturnValue;
            }
            break;
        case PLAYMOVIE_FILENUM:
            retV = DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT);
            break;
        //#NT#2012/09/19#Calvin Chang#Porting Media Play Demo1 Flow -begin
        case PLAYMOVIE_OPENED:
            retV = TRUE; // Always return true.
            break;
        case PLAYMOVIE_CURR_SECOND:
            retV = MediaPlay_GetCurrPlaySecs();
            break;
        case PLAYMOVIE_CURR_FRAME:
            retV = MediaPlay_GetCurrFrame();
            break;
        case PLAYMOVIE_TOTAL_FRAME:
            retV = MediaPlay_GetTotalVideoFrame();
            break;
        //#NT#2012/09/19#Calvin Chang -end

        default:
            DbgMsg_UIMovObjIO(("UIMovObj_GetDataPlayMovie:Unknown type=%d\r\n", dataType));
            break;
    }
    DbgMsg_UIMovObjIO(("-%s:type=0x%x,v=0x%x\r\n",__func__,dataType,retV));
    return retV;
}

//#NT#2012/09/19#Calvin Chang#Porting Media Play Demo1 Flow -begin
INT32 UIMovObj_SetAudPlayVolume(UINT32 vol)
{
    MediaPlay_SetAudVolume(vol);

    return NVTEVT_CONSUME;
}

INT32 UIMovObj_SwitchFirstFrameMovie(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    MediaPlay_RestartTo1stFrame();

    return NVTEVT_CONSUME;
}
//#NT#2012/09/19#Calvin Chang -end

//#NT#2013/03/21#Calvin Chang#Porting Media Play Demo1 Flow -begin
INT32 UIMovObj_PlayStepFwdMovie(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSpeedLevel;

    if (paramNum != 1)
    {
        return NVTEVT_CONSUME;
    }

    uiSpeedLevel = paramArray[0];

    //MediaPlay_FWDStep((MEDIAPLAY_SPEEDUP_TYPE)uiSpeedLevel);
    MediaPlay_FWDOneStep(SMEDIAPLAY_DIR_FORWARD);
    return NVTEVT_CONSUME;
}
INT32 UIMovObj_PlayStepBwdMovie(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSpeedLevel;

    if (paramNum != 1)
    {
        return NVTEVT_CONSUME;
    }

    uiSpeedLevel = paramArray[0];

    //MediaPlay_BWDStep((MEDIAPLAY_SPEEDUP_TYPE)uiSpeedLevel);
    MediaPlay_BWDOneStep(SMEDIAPLAY_DIR_BACKWARD);
    return NVTEVT_CONSUME;
}
//#NT#2013/03/21#Calvin Chang -end




INT32 UIMovObj_CloseReopenPlayMovie(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    INT32 retV = 0;
    return retV;
}

#if (_UIMOVOBJ_DBG_LVL_ > _UIMOVOBJ_DBG_CHK_)
void UIMovObj_DumpInitPlayParam(AVIPLAY_OBJ *pMovPlayObj)
{
}

#else

#define UIMovObj_DumpInitPlayParam(x)

#endif
INT32 UIMovObj_PlayFwdMovie(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMovObj_PlayBwdMovie(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMovObj_SwitchLastFrameMovie(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
void UIMovObj_SetDataPlayMovie(PLAYMOVIE_SET_DATATYPE dataType, UINT32 data)
{
    return;
}

INT32 UIMovObj_ChangeSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}


////////////////////////////////////////////////////////////

EVENT_ENTRY UIMoviePlayObjCmdMap[] =
{
    {NVTEVT_INIT_PLAY_MOVIE,                UIMovObj_InitPlayMovie},
    {NVTEVT_CLOSE_PLAY_MOVIE,               UIMovObj_ClosePlayMovie},
    {NVTEVT_CLOSEREOPEN_PLAY_MOVIE,         UIMovObj_CloseReopenPlayMovie},
    {NVTEVT_PAUSE_PLAY_MOVIE,               UIMovObj_PausePlayMovie},
    //#NT#2012/08/30#Calvin Chang#Porting Media Framework on UIAPP-UIMoviePlayObj -begin
    {NVTEVT_PLAY_PLAY_MOVIE,                UIMovObj_PlayMovie},
    //#NT#2012/08/30#Calvin Chang -end
    {NVTEVT_FWDPLAY_MOVIE,                  UIMovObj_PlayFwdMovie},
    {NVTEVT_BWDPLAY_MOVIE,                  UIMovObj_PlayBwdMovie},
    {NVTEVT_FWDSTEPPLAY_MOVIE,              UIMovObj_PlayStepFwdMovie},
    {NVTEVT_BWDSTEPPLAY_MOVIE,              UIMovObj_PlayStepBwdMovie},
    {NVTEVT_SWITCH_FIRST_FRAME_MOVIE,       UIMovObj_SwitchFirstFrameMovie},
    {NVTEVT_SWITCH_LAST_FRAME_MOVIE,        UIMovObj_SwitchLastFrameMovie},
    {NVTEVT_CHANGE_MOVSIZE,                 UIMovObj_ChangeSize},
    {NVTEVT_NULL,                           0},  //End of Command Map
};

CREATE_APP(UIMoviePlayObj,APP_SETUP)

//#NT#2010/09/16#Lily Kao -end

