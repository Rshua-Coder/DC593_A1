/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       LensCtrlTsk.h
    @ingroup    mIPRJAPCfg

    @brief      Header file for lens control task

    @note       Nothing.

    @date       2012/09/13
*/

#ifndef _LENSCTRLTSK_H
#define _LENSCTRLTSK_H


#include "Type.h"

typedef struct
{
    UINT32                 uiZoomSpeed;          ///< zoom speed
    BOOL                   bLockSpeed;           ///< zoom speed is locked or not
    MOTOR_FOCUS_ACT        FocusState;           ///< Focus state
    UINT32                 uiFocusParam;         ///< Parameter used to set Focus state
    BOOL                   bFocusResult;         ///< Result of Focus set state function
    MOTOR_APERTURE_ACT     ApertureState;        ///< Result of Aperture set state function
    UINT32                 uiAperturePhase;      ///< Parameter used to set Aperture state
    MOTOR_SHUTTER_ACT      ShutterState;         ///< Shutter state
    UINT32                 uiShutterParam1;      ///< Parameter used to set Shutter state

} LENSCTRL_PARAMOBJ, *PLENSCTRL_PARAMOBJ;

/**
    Set lens signal(s)

    @param[in] signal MD_SIGNAL.
                      MD_SIGNAL_STANDBY,      ///< Used to power on/off motor driver.
                      MD_SIGNAL_ZPI,          ///< Used to enable ZPI signal.
                      MD_SIGNAL_ZPR,          ///< Used to enable ZPR signal.
                      MD_SIGNAL_FPI,          ///< Used to enable FPI signal.
    @return void
*/
void       LensCtrl_SetSignal(MD_SIGNAL signal, BOOL on);

/**
    Turn off zoom.

    @return void
*/
void       LensCtrl_ZoomOff(void);

/**
    Brake zoom.

    @return void
*/
void       LensCtrl_ZoomBrake(void);

/**
    Zoom forward.

    @return void
*/
void       LensCtrl_ZoomForward(void);

/**
    Zoom reverse.

    @return void
*/
void       LensCtrl_ZoomReverse(void);

/**
    To notify zoom section is changed.

    @return void
*/
void       LensCtrl_ZoomChange(void);

/**
    To lock zoom speed.

    @return void
*/
void       LensCtrl_ZoomLockSpeed(BOOL bLock);

/**
    To change zoom speed.
    @param[in] UINT32 duty cycle - 63 ~ 100.
    @return void
*/
void       LensCtrl_ZoomChangeSpeed(UINT32);

/**
    To get zoom speed.
    @param[in] void.
    @return UINT32 duty cycle - 63 ~ 100.
*/
UINT32     LensCtrl_ZoomGetSpeed(void);

/**
    Set Focus state

    @param[in] signal MOTOR_FOCUS_ACT.
                      MOTOR_FOCUS_FWD,            ///< Used to forward focus.
                      MOTOR_FOCUS_BWD,            ///< Used to backward focus.
                      MOTOR_FOCUS_EXCITE_ON,      ///< Used to pre-excite on focus motor.
                      MOTOR_FOCUS_EXCITE_OFF,     ///< Used to excite off focus motor.
                      MOTOR_FOCUS_CHECK_EXT,      ///< Used to forward focus.
                      MOTOR_FOCUS_SETSPEED,       ///< Reserved
                      MOTOR_FOCUS_MICRO_STEP_EN,  ///< Used to enable/disable u-step.    @return void
    @param[in] param1 parameter used to set state.
    @return void
*/
void       LensCtrl_Focus_SetState(MOTOR_FOCUS_ACT state, UINT32 param1);

/**
    To get result of Focus set state function.
    @param[in] void.
    @return BOOL TRUE means ok.
*/
BOOL       LensCtrl_Focus_GetResult(void);

/**
    Set Aperture state

    @param[in] signal MOTOR_APERTURE_ACT.
                      MOTOR_APERTURE_FWD,            ///< Used to forward aperture.
                      MOTOR_APERTURE_BWD,            ///< Used to backward aperture.
                      MOTOR_APERTURE_EXCITE_ON,      ///< Used to pre-excite on aperture motor.
                      MOTOR_APERTURE_EXCITE_OFF,     ///< Used to excite off aperture motor.
                      MOTOR_APERTURE_RESET,          ///< Used to reset aperture position.
    @param[in] phase parameter used to set state.
    @return void
*/
void       LensCtrl_Aperture_SetState(MOTOR_APERTURE_ACT state, UINT32 phase);

/**
    Set Shutter state

    @param[in] signal MOTOR_SHUTTER_ACT.
                      MOTOR_SHUTTER_NORMAL,         ///< Normal opeartion for shutter
                      MOTOR_SHUTTER_CAP,            ///< Capture operation for shutter
    @param[in] param1 parameter used to set state.
    @return void
*/
void       LensCtrl_Shutter_SetState(MOTOR_SHUTTER_ACT state, UINT32 param1);

/**
    Record lens status

    @param[in] status ZOOM_FLAG_OUTSIDE or ZOOM_FLAG_INSIDE.
    @return void
*/
void       LensCtrl2_WriteLensStatus(UINT32 status);

/**
    Record lens status

    @param[in] void
    @return UINT32 ZOOM_FLAG_OUTSIDE or ZOOM_FLAG_INSIDE.
*/
UINT32     LensCtrl2_ReadLensStatus(void);

/**
    Wait until LensCtrlTsk2 is idle.

    @return void
*/
void       LensCtrl2_WaitIdle(void);

/**
    Wait until LensCtrlBGTsk is idle.

    @return void
*/
void       LensCtrlBG_WaitIdle(void);

/**
    Check if LensCtrlBGTsk is idle.

    @return void
*/
BOOL       LensCtrlBG_CheckIdle(void);

/**
    Install LensCtrlTsk task, flag and semaphore ID

    @return void
*/
extern void LensCtrl_InstallID(void) _SECTION(".kercfg_text");

/**
    Install LensCtrlTsk2 and LensCtrlBGTsk tasks, flags and semaphore ID

    @return void
*/
extern void LensCtrl2_InstallID(void) _SECTION(".kercfg_text");

#endif
