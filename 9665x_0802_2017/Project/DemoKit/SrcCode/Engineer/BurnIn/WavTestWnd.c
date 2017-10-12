//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "WavTestWndRes.c"
#include "WavTestWnd.h"
#include "BurnInWnd.h"
#include "PrjCfg.h"
#include "FileSysTsk.h"
#include "FileDB.h"
#include "Audio.h"
#include "DxSound.h"
#include "WavStudioTsk.h"
#include "dma.h"
#include "UIFlow.h"

#define __MODULE__          WavTest
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

//---------------------WavTestWndCtrl Debug Definition -----------------------------
#define _WAVTESTWND_ERROR_MSG        1
#define _WAVTESTWND_TRACE_MSG        0

#if (_WAVTESTWND_ERROR_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_ERR))
#define WavTestWndErrMsg(...)            debug_msg ("^R WavTestWnd: "__VA_ARGS__)
#else
#define WavTestWndErrMsg(...)
#endif

#if (_WAVTESTWND_TRACE_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_TRC))
#define WavTestWndTraceMsg(...)          debug_msg ("^B WavTestWnd: "__VA_ARGS__)
#else
#define WavTestWndTraceMsg(...)
#endif

#define WAVTEST_INC_VOL     0
#define WAVTEST_DEC_VOL     1
#define WAVTEST_VOL_STEP    1
#define WAVTEST_VOL_MAX     100
#define WAVTEST_VOL_MIN     0

//---------------------WavTestWndCtrl Global Variables -----------------------------
static INT32    g_iAudioVolume = WAVTEST_VOL_MAX; // default
static BOOL     g_bWavPlayFinish = FALSE;
static UINT32   g_uiPlayTime;

//---------------------WavTestWndCtrl Prototype Declaration  -----------------------
void WavTest_DrawVolume(UINT32 vol);

//---------------------WavTestWndCtrl Public API  ----------------------------------
void WavTest_SetAudVolume(UINT32 vol)
{
    DrvSound_SetVol(vol);
    g_iAudioVolume = vol;
}

void WavTest_ChangeAudVolume(UINT32 mode)
{
    if (mode == WAVTEST_INC_VOL)
    {
        g_iAudioVolume += WAVTEST_VOL_STEP;
        if (g_iAudioVolume > WAVTEST_VOL_MAX)
            g_iAudioVolume = WAVTEST_VOL_MAX;
    }
    else
    {
        g_iAudioVolume -= WAVTEST_VOL_STEP;
        if (g_iAudioVolume < WAVTEST_VOL_MIN)
            g_iAudioVolume = WAVTEST_VOL_MIN;
    }

    DrvSound_SetVol((UINT32)g_iAudioVolume);
    WavTest_DrawVolume(g_iAudioVolume);
}

void WavTest_WavStudioCB(UINT32 uiEventID)
{
    //DBG_DUMP("^C WavStudioCB: %d\r\n", uiEventID);
    switch(uiEventID)
    {
    case WAVSTUDIO_EVENT_STOPPED:
        g_bWavPlayFinish = TRUE;
        //DBG_DUMP("WAVSTUDIO_EVENT_STOPPED\r\n");
        break;

    case WAVSTUDIO_EVENT_DURATION:
        g_uiPlayTime++;
        BurnIn_DrawTime(g_uiPlayTime);
        //DBG_DUMP("WAVSTUDIO_EVENT_DURATION\r\n");
        break;
    }
}

void WavTestWnd_PlayWav(void)
{
    // for 650 WavStudio lib (only support DCF format)
    #define WAV_PATH_MAX_LEN        60
    #define WAV_PLAY_COUNT_UNIT     100 // 100ms
    #define WAV_PLAY_TIMEOUT        600 // 600 * 100ms = 60s
    #define WAV_BUFFER_SIZE         0x400000 // 4MB

    ER                  err = E_OK;
    UINT32              uiKey;
    UINT32              uiPoolAddr, uiPoolSize;
    UINT32              i, j, uiWavBufAddr, uiWavBufSize, uiFileNum;
    UINT32              uiDirID, uiFileID, uiFileType;
    WAVSTUDIO_APPOBJ    WavObj;
    WAVSTUDIO_FILEINFO  WavFileInfo;

    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_APP);
    uiPoolSize = OS_GetMempoolSize(POOL_ID_APP);

    // ----------------------------------- <--- APP buffer starting address
    // | Reserved                        |
    // -----------------------------------
    // | WavStudio buffer                |
    // ----------------------------------- <--- APP buffer end address

    DCF_SetDirFreeChars(DCF_SUBPATH);
    DCF_SetFileFreeChars(DCF_FILE_TYPE_ANYFORMAT, DCF_IMAGE_NAME);
    DCF_SetParm(DCF_PRMID_SET_VALID_FILE_FMT, DCF_FILE_TYPE_WAV);
    DCF_ScanObj();
    Sound_PollingPlayStop(1000);

    uiWavBufSize = WAV_BUFFER_SIZE;
    uiWavBufAddr = uiPoolAddr + uiPoolSize - uiWavBufSize;

    // config WavStudio
    WavObj.uiMemoryAddr = uiWavBufAddr;
    WavObj.uiMemorySize = uiWavBufSize;
    WavObj.WavStudioCB  = (WAVSTUDIO_CB)WavTest_WavStudioCB;

    // open WavStudio
    err = WavStudio_Open(&WavObj);
    if (err != E_OK)
    {
        DBG_ERR("WavStudio open failed, %d\r\n", err);
        return;
    }

    uiFileNum = DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT);
    DBG_DUMP("DCF uiFileNum %d\r\n", uiFileNum);

    for (i = 1; i <= uiFileNum; i++)
    {
        if (DCF_GetObjInfo(i, &uiDirID, &uiFileID, &uiFileType) == TRUE)
        {
            DBG_DUMP("DCF dir ID %d, file ID %d, type %d\r\n", uiDirID, uiFileID, uiFileType);

            WavFileInfo.uiDirID     = uiDirID;
            WavFileInfo.uiFileID    = uiFileID;
            WavFileInfo.uiType      = WAVSTUDIO_FILE_SINGLE;
            WavStudio_SetFile(&WavFileInfo);

            // reset play time
            g_uiPlayTime = 0;
            BurnIn_DrawTime(0);

            // start to play
            g_bWavPlayFinish = FALSE;
            if (WavStudio_Play() == FALSE)
            {
                DBG_ERR("play DCF (%d, %d) fail!\r\n", uiDirID, uiFileID);
                continue;
            }

            // wait for WAV play finished
            j = 0;
            while (j < WAV_PLAY_TIMEOUT) // timeout: 1 min
            {
                if (g_bWavPlayFinish) // check video encode ready
                {
                    break;
                }

                Delay_DelayMs(WAV_PLAY_COUNT_UNIT);
                j++;

                if (1)//(j % 2)
                {
                    uiKey = GxKey_GetData(GXKEY_NORMAL_KEY);
                    if (uiKey & FLGKEY_UP)
                    {
                        WavTest_ChangeAudVolume(WAVTEST_INC_VOL);
                    }
                    else if (uiKey & FLGKEY_DOWN)
                    {
                        WavTest_ChangeAudVolume(WAVTEST_DEC_VOL);
                    }
                }
            }

            if (j == WAV_PLAY_TIMEOUT) // timeout
            {
                DBG_ERR("play wav timeout!\r\n");
                WavStudio_Stop();
            }

            DBG_DUMP("^C WAV play OK!\r\n");
        }
    }

    // close WavStudio
    WavStudio_Close();
}

void WavTest_DrawData(BOOL bClear)
{
    CHAR    cDataStr[30];

    memset((void *)cDataStr, 0, sizeof(cDataStr));
    snprintf(cDataStr, sizeof(cDataStr), "Wav Play Test");
    UI_DrawOsdString(cDataStr, 12, 12, CLRID_IDX_SKYBLUE, bClear);
}

void WavTest_DrawVolume(UINT32 vol)
{
    CHAR    cDataStr[30];

    memset((void *)cDataStr, 0, sizeof(cDataStr));
    snprintf(cDataStr, sizeof(cDataStr), "Volume: %d  ", (int)vol);
    UI_DrawOsdString(cDataStr, 12, 12+30*3, CLRID_IDX_WHITE, TRUE);
}

//---------------------WavTestWndCtrl Private API  ---------------------------------
//---------------------WavTestWndCtrl Control List---------------------------
CTRL_LIST_BEGIN(WavTestWnd)
CTRL_LIST_END

//----------------------WavTestWndCtrl Event---------------------------
INT32 WavTestWnd_OnOpen(VControl *, UINT32, UINT32 *);
INT32 WavTestWnd_OnClose(VControl *, UINT32, UINT32 *);
INT32 WavTestWnd_OnKeyShutter2(VControl *, UINT32, UINT32 *);
INT32 WavTestWnd_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 WavTestWnd_OnKeyDown(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(WavTestWnd)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,WavTestWnd_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,WavTestWnd_OnClose)
EVENT_ITEM(NVTEVT_KEY_LEFT,WavTestWnd_OnKeyShutter2)
EVENT_ITEM(NVTEVT_KEY_UP,WavTestWnd_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,WavTestWnd_OnKeyDown)
EVENT_END

INT32 WavTestWnd_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);

    Ux_FlushEventByRange(NVTEVT_KEY_EVT_START,NVTEVT_KEY_EVT_END);
    Ux_PostEvent(NVTEVT_KEY_LEFT, 1, NVTEVT_KEY_PRESS); // post shutter2 event to start playing

    return NVTEVT_CONSUME;
}

INT32 WavTestWnd_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}

INT32 WavTestWnd_OnKeyShutter2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiKeyAct;

    //DBG_DUMP("^C Shutter 2!!\r\n");
    uiKeyAct = paramNum ? paramArray[0] : 0;

    if (uiKeyAct != NVTEVT_KEY_PRESS)
        return NVTEVT_CONSUME;

    WavTest_DrawData(TRUE);
    BurnIn_DrawStatus("Playing", CLRID_IDX_WHITE, TRUE);
    BurnIn_DrawTips("                                        ");
    WavTest_SetAudVolume(g_iAudioVolume);
    WavTest_DrawVolume(g_iAudioVolume);
    WavTestWnd_PlayWav();
    BurnIn_DrawStatus("Test Finished!", CLRID_IDX_GREEN, TRUE);
    BurnIn_DrawTips("Press Shutter key to play again");
    Ux_FlushEventByRange(NVTEVT_KEY_EVT_START,NVTEVT_KEY_EVT_END);

    return NVTEVT_CONSUME;
}

INT32 WavTestWnd_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiKeyAct;

    uiKeyAct = paramNum ? paramArray[0] : 0;

    if (uiKeyAct != NVTEVT_KEY_PRESS)
        return NVTEVT_CONSUME;

    WavTest_ChangeAudVolume(WAVTEST_INC_VOL);
    return NVTEVT_CONSUME;
}

INT32 WavTestWnd_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiKeyAct;

    uiKeyAct = paramNum ? paramArray[0] : 0;

    if (uiKeyAct != NVTEVT_KEY_PRESS)
        return NVTEVT_CONSUME;

    WavTest_ChangeAudVolume(WAVTEST_DEC_VOL);
    return NVTEVT_CONSUME;
}
