/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       LensCtrlTsk.c
    @ingroup    mIPRJAPCfg

    @brief      Lens control task
                Handle event when external interrupt is issued

    @note       Nothing.

    @date       2006/07/13
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "Kernel.h"
#include <string.h>
#include "SysKer.h"
#include "LensAPI.h"
#include "Utility.h"
#include "LensCtrlFlag.h"
#include "LensCtrlTsk.h"


///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          LENCTRLTSK
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------
static BOOL                 bLensCtrlOpened = FALSE;
static LENSCTRL_APPOBJ      gLensCtrlObj;
static LENSCTRL_PARAMOBJ    gLensCtrlParamObj ={
                                                0,                       //uiZoomSpeed
                                                FALSE,                   //bLockSpeed
                                                MOTOR_FOCUS_FWD,         //FocusState
                                                0,                       //uiFocusParam
                                                FALSE,                   //bFocusResult
                                                //#NT#2010/12/01#Lincy Lin -begin
                                                MOTOR_APERTURE_NORMAL,   //ApertureState
                                                //#NT#2010/12/01#Lincy Lin -end
                                                0,                       //uiAperturePhase
                                                MOTOR_SHUTTER_NORMAL,    //ShutterState
                                                0                        //uiShutterParam1
                                               };
/**
  Open Lens Control task

  Open Lens Control task.
  The parameter is not using now, just for compliance

  Return value is listed below:
  E_SYS: Task is already opened
  E_OK: No error

  @param LENSCTRL_APPOBJ pLensCtrlObj: Lens Control application object
  @return ER
*/
ER LensCtrl_Open(PLENSCTRL_APPOBJ pLensCtrlObj)
{

    if (bLensCtrlOpened == TRUE)
    {
        return E_SYS;
    }
    clr_flg(FLG_ID_LENS, FLGLENS_ALL);

    bLensCtrlOpened = TRUE;

    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk_setsignal = LensCtrl_SetSignal;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk_zoomoff = LensCtrl_ZoomOff;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk_zoombrake = LensCtrl_ZoomBrake;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk_zoomforward = LensCtrl_ZoomForward;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk_zoomreverse = LensCtrl_ZoomReverse;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk_zoomchange = LensCtrl_ZoomChange;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk_zoomlockspeed= LensCtrl_ZoomLockSpeed;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk_zoomchangespeed= LensCtrl_ZoomChangeSpeed;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk_zoomgetspeed = LensCtrl_ZoomGetSpeed;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk_focus_setstate = LensCtrl_Focus_SetState;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk_focus_getresult = LensCtrl_Focus_GetResult;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk_aperture_setstate= LensCtrl_Aperture_SetState;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk_shutter_setstate = LensCtrl_Shutter_SetState;
    memcpy(&gLensCtrlObj, pLensCtrlObj, sizeof(LENSCTRL_APPOBJ));

    sta_tsk(LENSCTRLTSK_ID, 0);

    return E_OK;
}

/**
  Close LensCtrl  task

  Close LensCtrl  task

  Return value is listed below:
  E_SYS: Task is already closed
  E_OK: No error

  @param void
  @return ER
*/
ER LensCtrl_Close(void)
{
    if (bLensCtrlOpened == FALSE)
    {
        return E_SYS;
    }

    bLensCtrlOpened = FALSE;

    ter_tsk(LENSCTRLTSK_ID);

    return E_OK;
}

/**
  Lens control task

  Lens control task to handle interrupt event.

  @param void
  @return void
*/
void LensCtrlTsk(void)
{
    FLGPTN  uiFlag;
    FLGPTN Mask;

    kent_tsk();

    Mask = (FLGLENS_ZOOMFWD|FLGLENS_ZOOMREV|FLGLENS_ZOOMBRK|FLGLENS_ZOOMOFF|
            FLGLENS_ZOOMCHANGE|FLGLENS_ZOOM_CHANGESPPED|FLGLENS_FOCUS_SETSTATE|FLGLENS_APERTURE_SETSTATE|FLGLENS_SHUTTER_SETSTATE);

    clr_flg(FLG_ID_LENS, Mask);
    while (1)
    {
        set_flg(FLG_ID_LENS, FLGLENS_IDLE);
        wai_flg(&uiFlag, FLG_ID_LENS, Mask , TWF_ORW|TWF_CLR);
        clr_flg(FLG_ID_LENS, FLGLENS_IDLE);

//Brake move to back
        DBG_MSG("0x%.8x",uiFlag);
        if(uiFlag & FLGLENS_ZOOMFWD)
        {
            gLensCtrlObj.pMotor->zoom_setState(MOTOR_ZOOM_FWD,0);
        }

        if(uiFlag & FLGLENS_ZOOMREV)
        {
            gLensCtrlObj.pMotor->zoom_setState(MOTOR_ZOOM_BWD,0);
        }

        if(uiFlag & FLGLENS_ZOOMOFF)
        {
            gLensCtrlObj.pMotor->zoom_setState(MOTOR_ZOOM_OFF,0);
        }

        if(uiFlag & FLGLENS_ZOOMCHANGE)
        {
            if(gLensCtrlObj.Task1CB!=NULL)
            {
                gLensCtrlObj.Task1CB(LENS_CB_CHGOZOOMSTEP, NULL);
            }
        }

        if(uiFlag & FLGLENS_ZOOM_CHANGESPPED)
        {
            gLensCtrlObj.pMotor->zoom_setSpeed(gLensCtrlParamObj.uiZoomSpeed);
        }

        if(uiFlag & FLGLENS_ZOOMBRK)
        {
            gLensCtrlObj.pMotor->zoom_setState(MOTOR_ZOOM_BRAKE,0);
        }

        if(uiFlag & FLGLENS_FOCUS_SETSTATE)
        {
            gLensCtrlParamObj.bFocusResult = gLensCtrlObj.pMotor->focus_setState(gLensCtrlParamObj.FocusState,gLensCtrlParamObj.uiFocusParam);
        }

        if(uiFlag & FLGLENS_APERTURE_SETSTATE)
        {
            //gLensCtrlObj.pMotor->aperture_setState(gLensCtrlParamObj.uiAperturePhase);
            gLensCtrlObj.pMotor->aperture_setState(gLensCtrlParamObj.ApertureState, gLensCtrlParamObj.uiAperturePhase);
        }

        if(uiFlag & FLGLENS_SHUTTER_SETSTATE)
        {
            switch(gLensCtrlParamObj.ShutterState)
            {
                case MOTOR_SHUTTER_PSEUDO_OPEN:
                    if(gLensCtrlObj.Task1CB!=NULL)
                    {
                        gLensCtrlObj.Task1CB(LENS_CB_SHUTTER_PSEUDO_OPEN, NULL);
                    }
                break;
                case MOTOR_SHUTTER_PSEUDO_CLOSE:
                    if(gLensCtrlObj.Task1CB!=NULL)
                    {
                        gLensCtrlObj.Task1CB(LENS_CB_SHUTTER_PSEUDO_CLOSE, NULL);
                    }
                break;
                default:
                    gLensCtrlObj.pMotor->shutter_setState(gLensCtrlParamObj.ShutterState,gLensCtrlParamObj.uiShutterParam1);
                break;
            }
        }
    }
}

void LensCtrl_SetSignal(MD_SIGNAL signal, BOOL on)
{
    gLensCtrlObj.pMotor->setSignal(signal,on);
}

void LensCtrl_ZoomOff(void)
{
    set_flg(FLG_ID_LENS,FLGLENS_ZOOMOFF);
}

void LensCtrl_ZoomBrake(void)
{
    set_flg(FLG_ID_LENS,FLGLENS_ZOOMBRK);
}

void LensCtrl_ZoomForward(void)
{
    set_flg(FLG_ID_LENS,FLGLENS_ZOOMFWD);
}

void LensCtrl_ZoomReverse(void)
{
    set_flg(FLG_ID_LENS,FLGLENS_ZOOMREV);
}

void LensCtrl_ZoomChange(void)
{
    set_flg(FLG_ID_LENS,FLGLENS_ZOOMCHANGE);
}

void LensCtrl_ZoomLockSpeed(BOOL bLock)
{
    gLensCtrlParamObj.bLockSpeed = bLock;
}

void LensCtrl_ZoomChangeSpeed(UINT32 speed)
{

    //check lock
    if(gLensCtrlParamObj.bLockSpeed)
    {
        return;
    }

    //check current speed
    if(gLensCtrlObj.pMotor->zoom_getSpeed() == speed && gLensCtrlObj.pMotor->zoom_getSpeed()!=100)
    {
        return;
    }

    gLensCtrlParamObj.uiZoomSpeed = speed;
    set_flg(FLG_ID_LENS,FLGLENS_ZOOM_CHANGESPPED);
}

UINT32 LensCtrl_ZoomGetSpeed(void)
{
    return gLensCtrlObj.pMotor->zoom_getSpeed();
}

void LensCtrl_Focus_SetState(MOTOR_FOCUS_ACT state, UINT32 param1)
{
    FLGPTN  Flag;

    gLensCtrlParamObj.FocusState = state;
    gLensCtrlParamObj.uiFocusParam = param1;
    wai_flg(&Flag, FLG_ID_LENS, FLGLENS_IDLE , TWF_ORW|TWF_CLR);
    set_flg(FLG_ID_LENS,FLGLENS_FOCUS_SETSTATE);
    wai_flg(&Flag, FLG_ID_LENS, FLGLENS_IDLE , TWF_ORW);
}

BOOL LensCtrl_Focus_GetResult(void)
{
    return gLensCtrlParamObj.bFocusResult;
}

void LensCtrl_Aperture_SetState(MOTOR_APERTURE_ACT state, UINT32 phase)
{
    FLGPTN  Flag;

    gLensCtrlParamObj.ApertureState = state;
    gLensCtrlParamObj.uiAperturePhase = phase;
    wai_flg(&Flag, FLG_ID_LENS, FLGLENS_IDLE , TWF_ORW|TWF_CLR);
    set_flg(FLG_ID_LENS,FLGLENS_APERTURE_SETSTATE);
    wai_flg(&Flag, FLG_ID_LENS, FLGLENS_IDLE , TWF_ORW);
}

void LensCtrl_Shutter_SetState(MOTOR_SHUTTER_ACT state, UINT32 param1)
{
    FLGPTN  Flag;

    gLensCtrlParamObj.ShutterState = state;
    gLensCtrlParamObj.uiShutterParam1 = param1;
    wai_flg(&Flag, FLG_ID_LENS, FLGLENS_IDLE , TWF_ORW|TWF_CLR);
    set_flg(FLG_ID_LENS,FLGLENS_SHUTTER_SETSTATE);
    if (!(state == MOTOR_SHUTTER_PSEUDO_OPEN || state == MOTOR_SHUTTER_PSEUDO_CLOSE))
    {
        wai_flg(&Flag, FLG_ID_LENS, FLGLENS_IDLE , TWF_ORW);
    }
}

