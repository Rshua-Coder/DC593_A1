/*
    System Audio Callback

    System Callback for Audio Module.

    @file       SysLens_Audio.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"
#include "GxSound.h"
#include "MediaPlayAPI.h"
#if (UI_STYLE==UI_STYLE_DEMO)
#include "Demo_sound.h"
#else
#include "SoundData.h"
#endif
#include "Dx.h"
#include "DxType.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysAudioExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
void System_SetAudioOutput(void);
void Sound_CB(UINT32 event, UINT32 param1, UINT32 param2);
extern DX_HANDLE gDevLCDObj;
extern DX_HANDLE gDevTVObj;
extern DX_HANDLE gDevHDMIObj;

void System_OnAudioInit(void)
{
    //PHASE-1 : Init & Open Drv or DrvExt
    {
        GxSound_RegCB(Sound_CB);         //Register CB function of GxSound
        UISound_RegTable();
        if (E_OK == GxSound_InitAudio())
        {
            GxSound_Open();
        }
    }
    //PHASE-2 : Init & Open Lib or LibExt
    {
        //config audio Output
        System_SetAudioOutput();
    }
}

void System_OnAudioExit(void)
{
    //PHASE-2 : Close Lib or LibExt
    {
    }
    //PHASE-1 : Close Drv or DrvExt
    {
        GxSound_Close();
    }
}

///////////////////////////////////////////////////////////////////////
void System_SetAudioOutput(void)
{
    DX_HANDLE VideoDevObj1, VideoDevObj2;
    VideoDevObj1 = (DX_HANDLE)GxVideo_GetDevice(DOUT1);
    VideoDevObj2 = (DX_HANDLE)GxVideo_GetDevice(DOUT2);
    if((VideoDevObj1 == gDevLCDObj) && (VideoDevObj2 == 0)) //case 1
    {
        // set audio output device entry for LCD case
        //GxSound_SetOutDevConfigIdx(0);
        //MediaPlay_SetOutDevConfigIdx(0);
        
        GxSound_SetOutDevConfigIdx(1);//drh 20170919
        MediaPlay_SetOutDevConfigIdx(1);
    }
    if((VideoDevObj1 == gDevTVObj) && (VideoDevObj2 == 0)) //case 2
    {
        // set audio output device entry for TV case
        GxSound_SetOutDevConfigIdx(1);
        MediaPlay_SetOutDevConfigIdx(1);
    }
    if((VideoDevObj1 == gDevHDMIObj) && (VideoDevObj2 == 0)) //case 3
    {
        // set audio output device entry for HDMI case
        GxSound_SetOutDevConfigIdx(2);
        MediaPlay_SetOutDevConfigIdx(2);
    }
    if((VideoDevObj1 == gDevLCDObj) && (VideoDevObj2 == gDevTVObj)) //case 4
    {
        // set audio output device entry for LCD & TV case
        GxSound_SetOutDevConfigIdx(3);
        MediaPlay_SetOutDevConfigIdx(3);
    }
    if((VideoDevObj1 == gDevLCDObj) && (VideoDevObj2 == gDevHDMIObj)) //case 5
    {
        // set audio output device entry for LCD & HDMI case
        GxSound_SetOutDevConfigIdx(4);
        MediaPlay_SetOutDevConfigIdx(4);
    }
}


///////////////////////////////////////////////////////////////////////
//Device flow event

INT32 System_OnAudioInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}

INT32 System_OnAudioRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}


INT32 System_OnAudioAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //  AUDIO: if(boot)->play boot sound
    //  AUDIO: if(boot)->start audio srv
    return NVTEVT_CONSUME;
}

INT32 System_OnAudioDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}


