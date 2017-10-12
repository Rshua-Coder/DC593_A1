/*
    System Sound Callback

    System Callback for Sound Module.

    @file       SysSoundCB.c
    @ingroup    mIPRJSYS

    @note       這個檔案負責一件事

                1.接收Sound callback的介面
                  Sound_CB()
                  可能的event有:
                     SYSTEM_CB_CONFIG
                     SOUND_CB_START
                       會呼叫GxFlash_SuspendCharge();
                     SOUND_CB_STOP
                       會呼叫GxFlash_ResumeCharge();
                     SOUND_CB_OUTDEVCHG
                       會呼叫GxSound_SetOutput()


    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/
#include "SysCommon.h"
#include "GxSound.h"
#include "Debug.h"
#include "GxSound.h"
#include "UIInfo.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__              SysSoundCB
#define __DBGLVL__              1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__              "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//---------------------SysSoundCB Global Variables -----------------------------
//---------------------SysSoundCB Prototype Declaration  -----------------------
//---------------------SysSoundCB Public API  ----------------------------------
//---------------------SysSoundCB Private API ----------------------------------

int SX_TIMER_DET_AUDIODEV_ID = -1;
extern BOOL g_bIsInitSystemFinish;
BOOL g_bSoundPlaying = FALSE;

UINT32 guiSndRepeat = FALSE;

void Sound_SetRepeat(UINT32 isTrue)
{
    guiSndRepeat = isTrue;
}

UINT32 Sound_GetRepeat(void)
{
    return guiSndRepeat;
}

BOOL Sound_IsPlaying(void)
{
    return g_bSoundPlaying;
}

void Sound_PollingPlayStop(UINT32 uiPollingTime)
{
    UINT32 uiPollingCount = 0;
    UINT32 uiDelayUnit = 50; // 50ms
    UINT32 uiDelayCount;

    if (UI_GetData(FL_BEEP) == BEEP_OFF)
        return;

    uiDelayCount = uiPollingTime / uiDelayUnit;
    if (uiDelayCount < 1)
        uiDelayCount = 1;

    while (Sound_IsPlaying())
    {
        Delay_DelayMs(uiDelayUnit);
        uiPollingCount++;
        if (uiPollingCount == uiDelayCount)
            break;
    }
}

void Sound_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
    UINT32 audOutDev = param1;

    DBG_IND("++Sound_CB:event=%d,p1=%d,p2=%d\r\n",event,param1,param2);

    switch(event)
    {
    case SYSTEM_CB_CONFIG:
        //2.設定CB值,
        //3.註冊SxJob服務 ---------> System Job
        //4.註冊SxTimer服務 ---------> Detect Job
        //SX_TIMER_DET_AUDIODEV_ID = System_AddSxTimer(GxSoud_DetSetAudioOutDevice, 25, "GxSoud_DetAudioOutDevice");
        //5.註冊SxCmd服務 ---------> Cmd Function
        //System_AddSxCmd(Sound_OnCommand); //GxSound
        break;

    case SOUND_CB_START:
        g_bSoundPlaying = TRUE;
        break;

    case SOUND_CB_STOP:
        g_bSoundPlaying = FALSE;
        break;

    case SOUND_CB_OUTDEVCHG:
        GxSound_SetOutDevConfigIdx(audOutDev);
        break;

    default:
        DBG_ERR("Not Support SOUND_CB event=%d\r\n",event);
        break;
    }

    DBG_IND("--Sound_CB:event=%d\r\n",event);
}

