/*
    System Lens Callback

    System Callback for Lens Module.

    @file       SysLens_Exe.c
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
#include "lens.h"
#include "GxFlash.h"
#include "DeviceCtrl.h"
//#NT#2012/11/07#Calvin Chang#API function for Lens state updating -begin
#include "UIAppMovie.h"
//#NT#2012/11/07#Calvin Chang -end

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysLensExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
extern SX_CMD_ENTRY Cmd_lens[]; //device (lens)

int SX_TIMER_DET_CLOSE_LENS_ID = -1;

SX_TIMER_ITEM(System_DetCloseLens, System_DetCloseLens, 50, TRUE)

extern void DrvLens_PowerOn(void);
extern void DrvLens_PowerOff(void);
void Photo_LensCB(UINT32 MessageID, UINT32 *Parameter);
void LensAPICB(UINT32 MessageID, UINT32 *Parameter);

void System_OnLensInit(void)
{
    //PHASE-1 : Init & Open Drv or DrvExt
    {
#if LENS_FUNCTION
        DrvLens_RegCtrlCB((void*)Photo_LensCB, (void*)NULL, (void*)LensAPICB);
#endif
    }
    //PHASE-2 : Init & Open Lib or LibExt
    {
#if 1 //LENS_FUNCTION
    if(System_GetState(SYS_STATE_POWERON) != SYS_POWERON_LOWPOWER)
    {
        DrvLens_PowerOn();
        Lens_Module_Init(DrvLens_GetLensCtrlObj());
    }
    SX_TIMER_DET_CLOSE_LENS_ID = SxTimer_AddItem(&Timer_System_DetCloseLens);
#endif
    }
}

void System_OnLensExit(void)
{
    //PHASE-2 : Close Lib or LibExt
    {
    DBG_IND("power off state=%d\r\n",System_GetState(SYS_STATE_POWEROFF));
#if LENS_FUNCTION
    //if(System_GetState(SYS_STATE_POWEROFF) != SYS_POWEROFF_LOWPOWER)
    {
        if(DevCtrl_Lens_PowerOff_IsNeedRetract())
        {
            DevCtrl_Lens_PowerOff_Retract();
        }
    }
#endif
    }
    //PHASE-1 : Close Drv or DrvExt
    {
    }
}

BOOL    bIsShuterPseudoopen = FALSE;

void Photo_LensCB(UINT32 MessageID, UINT32 *Parameter)
{
    switch (MessageID)
    {
        case LENS_CB_CHGOZOOMSTEP:
            Ux_PostEvent(NVTEVT_CB_ZOOM, 0);
            break;
        case LENS_CB_SHUTTER_PSEUDO_OPEN:
            DBG_IND("===LENS_CB_SHUTTER_PSEUDO_OPEN===\r\n");
            //GxFlash_SuspendCharge();
            bIsShuterPseudoopen = TRUE;
            break;
        case LENS_CB_SHUTTER_PSEUDO_CLOSE:
            DBG_IND("===LENS_CB_SHUTTER_PSEUDO_CLOSE===\r\n");
            //GxFlash_ResumeCharge();
            bIsShuterPseudoopen = FALSE;
            break;
    }
}

void LensAPICB(UINT32 MessageID, UINT32 *Parameter)
{
    DBG_IND("MessageID=%d\r\n",MessageID);
    switch(MessageID)
    {
        case LENS_CB_INITZOOM1_START:
        case LENS_CB_INITZOOM2_START:
        case LENS_CB_INITFOCUS_START:
        case LENS_CB_INITAPERTURE_START:
        case LENS_CB_ZOOM_START:
        case LENS_CB_FOCUS_START:
        case LENS_CB_APERTURE_START:
        case LENS_CB_SHUTTER_START:
            GxFlash_SuspendCharge();
            //#NT#2012/11/07#Calvin Chang#API function for Lens state updating -begin
            UIMovie_UpdateLensState(MessageID);
            //#NT#2012/11/07#Calvin Chang -end
            break;
        case LENS_CB_INITZOOM1_END:
        case LENS_CB_INITZOOM2_END:
        case LENS_CB_INITFOCUS_END:
        case LENS_CB_INITAPERTURE_END:
        case LENS_CB_ZOOM_END:
        case LENS_CB_FOCUS_END:
        case LENS_CB_APERTURE_END:
        case LENS_CB_SHUTTER_END:
            GxFlash_ResumeCharge();
            //#NT#2012/11/07#Calvin Chang#API function for Lens state updating -begin
            UIMovie_UpdateLensState(MessageID);
            //#NT#2012/11/07#Calvin Chang -end
            break;
        case LENS_CB_RESET_START:
            GxFlash_SuspendCharge();
            #if (LENS_PATCH_ZOOM_RESET == ENABLE)
            #if _DEMO_TODO
            sie_pause();
            #endif
            #endif

            break;
        case LENS_CB_RESET_END:
            #if (LENS_PATCH_ZOOM_RESET == ENABLE)
            #if _DEMO_TODO
            sie_start();
            #endif
            #endif
            GxFlash_ResumeCharge();
            break;
        default:
            DBG_ERR("Parameter error in LensAPICB() (%d)\r\n",MessageID);
            break;
    }
}


static UINT32 g_uiPBCloseLensCnt = 0;

void System_DetCloseLens(void)
{
    #define CLOSE_LENS_MAX_COUNT  10   //10secs
    g_uiPBCloseLensCnt++;

    DBG_IND("Cnt=%d\r\n", g_uiPBCloseLensCnt);

    if( (UI_GetData(FL_ModeIndex) == DSC_MODE_PLAYBACK) && (UI_GetData(FL_NextMode) == DSC_MODE_PLAYBACK)  && UI_GetData(FL_LensOpen)&&(g_uiPBCloseLensCnt >=CLOSE_LENS_MAX_COUNT))
    {
        Ux_FlushEventByRange(NVTEVT_EXE_PB_RETRACT_LENS,NVTEVT_EXE_PB_RETRACT_LENS);
        Ux_PostEvent(NVTEVT_EXE_PB_RETRACT_LENS,0);
    }

    if((g_uiPBCloseLensCnt >=CLOSE_LENS_MAX_COUNT)||(UI_GetData(FL_ModeIndex) != DSC_MODE_PLAYBACK) || UI_GetData(FL_NextMode) != DSC_MODE_PLAYBACK) //10secs
        g_uiPBCloseLensCnt = 0;
}

void System_ResetDetCloseLenCount(void)
{
    g_uiPBCloseLensCnt = 0;
}
