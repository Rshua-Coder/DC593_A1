//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "MoviePlayTestWndRes.c"
#include "MoviePlayTestWnd.h"
#include "PrjCfg.h"
#include "UIFlow.h"
#include "UIMovieObj.h"
#include "SysCommon.h"
#include "SMediaPlayAPI.h"
#include "UIPlayComm.h"
#include "BurnInWnd.h"
#include "AudioTestWnd.h"

#define __MODULE__          MoviePlayTest
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

//---------------------MoviePlayTestWndCtrl Debug Definition -----------------------------
#define _MOVIEPLAYTESTWND_ERROR_MSG        1
#define _MOVIEPLAYTESTWND_TRACE_MSG        0

#if (_MOVIEPLAYTESTWND_ERROR_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_ERR))
#define MoviePlayTestWndErrMsg(...)            debug_msg ("^R MoviePlayTestWnd: "__VA_ARGS__)
#else
#define MoviePlayTestWndErrMsg(...)
#endif

#if (_MOVIEPLAYTESTWND_TRACE_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_TRC))
#define MoviePlayTestWndTraceMsg(...)          debug_msg ("^B MoviePlayTestWnd: "__VA_ARGS__)
#else
#define MoviePlayTestWndTraceMsg(...)
#endif

//---------------------MoviePlayTestWndCtrl Global Variables -----------------------------
extern INT32 g_iSysNextMode;

static BOOL     g_bMoviePlayWndOpen = FALSE;
static BOOL     g_bMoviePlay = FALSE;
static char     g_FilePath[64];
static UINT32   g_uiAudioVolume = 0;
static UINT32   g_uiMovieTotalSec = 0;
static UINT32   g_uiPlayReserveSec = 0;
static FST_FILE g_pMoviePlayFile = NULL;
static SMEDIAPLAY_USEROBJ g_MoviePlayObj = {0};

//---------------------MoviePlayTestWndCtrl Prototype Declaration  -----------------------

//---------------------MoviePlayTestWndCtrl Public API  ----------------------------------
void MoviePlayTestWnd_Close(void)
{
    if (g_bMoviePlayWndOpen)
    {
        Ux_CloseWindow((VControl *)(&MoviePlayTestWndCtrl), 0);
    }
}

void MoviePlayTestWnd_SetAudioVolume(UINT32 uiVolume)
{
    g_uiAudioVolume = uiVolume;
}

void MoviePlayTestWnd_SetPlayReserveSec(UINT32 uiSec)
{
    g_uiPlayReserveSec = uiSec;
}

//---------------------MoviePlayTestWndCtrl Private API  ---------------------------------

//---------------------MoviePlayTestWndCtrl Control List---------------------------
CTRL_LIST_BEGIN(MoviePlayTestWnd)
CTRL_LIST_END

//----------------------MoviePlayTestWndCtrl Event---------------------------
INT32 MoviePlayTestWnd_OnOpen(VControl *, UINT32, UINT32 *);
INT32 MoviePlayTestWnd_OnClose(VControl *, UINT32, UINT32 *);
INT32 MoviePlayTestWnd_OnKeyShutter2(VControl *, UINT32, UINT32 *);
INT32 MoviePlayTestWnd_OnMovieFinish(VControl *, UINT32, UINT32 *);
INT32 MoviePlayTestWnd_OnMovieOneSec(VControl *, UINT32, UINT32 *);
INT32 MoviePlayTestWnd_OnBattery(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(MoviePlayTestWnd)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,MoviePlayTestWnd_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,MoviePlayTestWnd_OnClose)
EVENT_ITEM(NVTEVT_KEY_LEFT,MoviePlayTestWnd_OnKeyShutter2)
EVENT_ITEM(NVTEVT_CB_MOVIE_FINISH,MoviePlayTestWnd_OnMovieFinish)
EVENT_ITEM(NVTEVT_CB_MOVIE_ONE_SEC,MoviePlayTestWnd_OnMovieOneSec)
EVENT_ITEM(NVTEVT_BATTERY,MoviePlayTestWnd_OnBattery)
EVENT_END

INT32 MoviePlayTestWnd_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    INT32   iStatus, iResult = PB_STA_DONE;
    UINT32  uiFileNum;

    g_bMoviePlayWndOpen = TRUE;
    g_bMoviePlay = FALSE;
    g_iSysNextMode = PRIMARY_MODE_PLAYBACK;
    Ux_SetActiveApp(&CustomPlayObjCtrl);
    Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);

    iStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);

    PB_GetParam(PBPRMID_TOTAL_FILE_COUNT, &uiFileNum);
#if (1)//for C10 series
    PB_OpenSpecFileBySeq(1, TRUE); // change sorting sequence to be large first, so it should start from 1st file
#else
    PB_OpenSpecFileBySeq(uiFileNum, TRUE);
#endif
    UIPlay_PlaySingle(PB_SINGLE_CURR);
    iStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);

    //disable video2 and enable video 1
    //??//Display_ShowPreview();

    UpdateVdoWinForPB();

    if (iStatus & PB_STA_NOIMAGE)
    {
        iResult = PB_STA_NOIMAGE;
    }
    else
    {
        UINT32 uiCurrMode;

        iStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);
        PB_GetParam(PBPRMID_PLAYBACK_MODE, &uiCurrMode);

        // read error or decode error
        if (iStatus & (PB_STA_ERR_FILE | PB_STA_ERR_DECODE))
        {
            iResult = PB_STA_ERR_FILE;
        }
    }

    Ux_FlushEventByRange(NVTEVT_KEY_EVT_START,NVTEVT_KEY_EVT_END);
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);

    Ux_Redraw();
    BurnIn_DrawLoopCount(TRUE);
    AudioTest_DrawData(TRUE);

    if (iResult == PB_STA_NOIMAGE)
    {
        BurnIn_DrawStatus("No Image!", CLRID_IDX_RED, TRUE);
        MoviePlayTestWnd_Close();
    }
    else if (iResult == PB_STA_ERR_FILE)
    {
        BurnIn_DrawStatus("File Error!", CLRID_IDX_RED, TRUE);
        MoviePlayTestWnd_Close();
    }
    else
    {
        Ux_PostEvent(NVTEVT_KEY_LEFT, 1, NVTEVT_KEY_PRESS); // post shutter2 event to play movie
    }

    return NVTEVT_CONSUME;
}

INT32 MoviePlayTestWnd_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);
    g_bMoviePlayWndOpen = FALSE;
    g_bMoviePlay = FALSE;

    return NVTEVT_CONSUME;
}

INT32 MoviePlayTestWnd_OnKeyShutter2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiPBFileFmt;
    UINT32  uiKeyAct;

    uiKeyAct = paramNum ? paramArray[0] : 0;

    if (uiKeyAct != NVTEVT_KEY_PRESS)
        return NVTEVT_CONSUME;

    if (g_bMoviePlay)
    {
        BurnIn_DrawStatus("Stop Play", CLRID_IDX_WHITE, TRUE);

        // stop playing
        Ux_SendEvent(&UIMoviePlayObjCtrl, NVTEVT_PAUSE_PLAY_MOVIE, 0);
        Ux_SendEvent(&UIMoviePlayObjCtrl, NVTEVT_CLOSE_PLAY_MOVIE, 0);

        if (g_pMoviePlayFile)
        {
            FileSys_CloseFile(g_pMoviePlayFile);
            g_pMoviePlayFile = NULL;
        }

        g_bMoviePlay = FALSE;

        // close media play test window
        Ux_CloseWindow((VControl *)(&MoviePlayTestWndCtrl), 0);
    }
    else
    {
        PB_GetParam(PBPRMID_CURR_FILEFMT, &uiPBFileFmt);
        if (uiPBFileFmt & (PBFMT_MOVMJPG | PBFMT_MP4))
        {
            g_uiMovieTotalSec = UIMovObj_GetDataPlayMovie(PLAYMOVIE_TOTAL_SECOND);
            DBG_DUMP("^C Movie total sec: %d\r\n", g_uiMovieTotalSec);

            if (g_pMoviePlayFile)
            {
                FileSys_CloseFile(g_pMoviePlayFile);
                g_pMoviePlayFile = NULL;
            }

            // get current file path
            PB_GetParam(PBPRMID_CURR_FILEPATH, (UINT32 *)g_FilePath);

            // open current video file
            g_pMoviePlayFile = FileSys_OpenFile(g_FilePath, FST_OPEN_READ);

            if (!g_pMoviePlayFile)
            {
                DBG_ERR("Open video file error!\r\n");
                return NVTEVT_CONSUME;
            }

            KeyScan_EnableMisc(FALSE);

            g_MoviePlayObj.hActMediafilehdl = g_pMoviePlayFile;
            g_MoviePlayObj.CallBackFunc     = Play_MovieCB;
            g_MoviePlayObj.uiMemAddr        = (UINT32)OS_GetMempoolAddr(POOL_ID_APP);
            g_MoviePlayObj.uiMemSize        = (UINT32)OS_GetMempoolSize(POOL_ID_APP);

            // flush event first
            Ux_FlushEventByRange(NVTEVT_KEY_EVT_START, NVTEVT_KEY_EVT_END);

            // start to play
            BurnIn_DrawStatus("Playing", CLRID_IDX_WHITE, TRUE);
            BurnIn_DrawTime(0);
            UIMovObj_SetAudPlayVolume(g_uiAudioVolume); // set play volume
            Ux_SendEvent(&UIMoviePlayObjCtrl, NVTEVT_INIT_PLAY_MOVIE, 1, &g_MoviePlayObj);
            Ux_SendEvent(&UIMoviePlayObjCtrl,
                         NVTEVT_PLAY_PLAY_MOVIE,
                         2,
                         SMEDIAPLAY_SPEED_NORMAL,
                         SMEDIAPLAY_DIR_FORWARD);

            g_bMoviePlay = TRUE;
        }
        else
        {
            // show no video file message and close window
            BurnIn_DrawStatus("No Video!", CLRID_IDX_RED, TRUE);
            MoviePlayTestWnd_Close();
        }
    }

    return NVTEVT_CONSUME;
}

INT32 MoviePlayTestWnd_OnMovieFinish(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    BurnIn_DrawStatus("Play Finish", CLRID_IDX_WHITE, TRUE);

    //Ux_SendEvent(&UIMoviePlayObjCtrl, NVTEVT_PAUSE_PLAY_MOVIE, 0);
    Ux_SendEvent(&UIMoviePlayObjCtrl, NVTEVT_CLOSE_PLAY_MOVIE, 0);

    if (g_pMoviePlayFile)
    {
        FileSys_CloseFile(g_pMoviePlayFile);
        g_pMoviePlayFile = NULL;
    }

    g_bMoviePlay = FALSE;

    // close media play test window
    Ux_CloseWindow((VControl *)(&MoviePlayTestWndCtrl), 0);

    return NVTEVT_CONSUME;
}

INT32 MoviePlayTestWnd_OnMovieOneSec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiPlayTime;

    uiPlayTime = SMediaPlay_GetCurrPlaySecs();
    BurnIn_DrawTime(uiPlayTime);
    AudioTest_DrawData(TRUE);

#if 1
    // TEST!!! stop playing before movie finish!
    if (g_uiMovieTotalSec > g_uiPlayReserveSec)
    {
        if (g_uiPlayReserveSec && (uiPlayTime == (g_uiMovieTotalSec - g_uiPlayReserveSec)))
        {
            Ux_PostEvent(NVTEVT_KEY_LEFT, 1, NVTEVT_KEY_PRESS); // post shutter2 event to stop movie play
        }
    }
#endif

    return NVTEVT_CONSUME;
}

INT32 MoviePlayTestWnd_OnBattery(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}

