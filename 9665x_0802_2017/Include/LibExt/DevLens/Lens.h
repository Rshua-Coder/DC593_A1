/**
    Public header file for Lens module.

    This file is the header file that define the API and data type for Lens module.

    @file       Lens.h
    @ingroup    mILens
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _LENS_H
#define _LENS_H

#include "Type.h"
#include "Debug.h"
#include "DxLens.h"

/**
     @addtogroup mILens
*/
//@{

typedef enum
{
    LENS_AFSTOP = 0,                ///< reserved
    LENS_AFMOVE,                    ///< reserved
    ENUM_DUMMY4WORD(LENS_AFSTATUS)
}LENS_AFSTATUS;

/**
    @name section name of PStore to save calibration data.

*/
//@{
#define PS_FOCUS_DATA            "FOCU"
//@}


typedef enum
{
    LENS_STATE_NONE,                ///< Initial state
    LENS_STATE_INIT_PART1,          ///< Lens is in INIT_PART1 state
    LENS_STATE_IDLE,                ///< Lens is in IDLE state
    LENS_STATE_ZOOMMOVING,          ///< Lens is in ZOOMMVING state
    LENS_STATE_FOCUSMOVING,         ///< Lens is in FOCUSMOVING state
    LENS_STATE_IRISMOVING,          ///< Lens is in IRISMOVING state
    LENS_STATE_SHUTTERMOVING,       ///< Lens is in SHUTTERMOVING state
    LENS_STATE_RESET,               ///< Lens is in RESET state
    LENS_STATE_POWER_OFF,           ///< Lens is in POWER_OFF state
    ENUM_DUMMY4WORD(LENS_STATE)
} LENS_STATE;


typedef enum
{
    //Lens API usage
    LENS_CB_INITZOOM1_START = 0,    ///< Callback to notify INIT_ZOOM_PART1 start
    LENS_CB_INITZOOM1_END,          ///< Callback to notify INIT_ZOOM_PART1 end
    LENS_CB_INITZOOM2_START,        ///< Callback to notify INIT_ZOOM_PART2 start
    LENS_CB_INITZOOM2_END,          ///< Callback to notify INIT_ZOOM_PART2 end
    LENS_CB_INITFOCUS_START,        ///< Callback to notify INITFOCUS_START
    LENS_CB_INITFOCUS_END,          ///< Callback to notify INITFOCUS_END
    LENS_CB_INITAPERTURE_START,     ///< Callback to notify INITAPERTURE_START
    LENS_CB_INITAPERTURE_END,       ///< Callback to notify INITAPERTURE_END
    LENS_CB_RESET_START,            ///< Callback to notify LENS_RESET start
    LENS_CB_RESET_END,              ///< Callback to notify LENS_RESET end
    LENS_CB_ZOOM_START,             ///< Callback to notify ZOOM starts moving
    LENS_CB_ZOOM_END,               ///< Callback to notify ZOOM stop
    LENS_CB_FOCUS_START,            ///< Callback to notify FOCUS starts moving
    LENS_CB_FOCUS_END,              ///< Callback to notify FOCUS stop
    LENS_CB_APERTURE_START,         ///< Callback to notify APERTURE starts moving
    LENS_CB_APERTURE_END,           ///< Callback to notify APERTURE stop
    LENS_CB_SHUTTER_START,          ///< Callback to notify SHUTTER starts moving
    LENS_CB_SHUTTER_END,            ///< Callback to notify SHUTTER stop
    //Task1 usage
    LENS_CB_CHGOZOOMSTEP,           ///< Callback to notify ZOOM section was changed
    LENS_CB_SHUTTER_PSEUDO_OPEN,    ///< reserved
    LENS_CB_SHUTTER_PSEUDO_CLOSE,   ///< reserved
    LENS_CB_FOCUSMOVE,              ///< reserved
    //Task2 usage
    LENS_CB_WRITE_STATUS,           ///< Callback to notify to record Lens status.
    LENS_CB_READ_STATUS,            ///< Callback to notify to read Lens status.
    ENUM_DUMMY4WORD(LENS_CB_MESSAGE)
} LENS_CB_MESSAGE;

typedef enum
{
    LENS_DUMMYLOAD_START,           ///< reserved
    LENS_DUMMYLOAD_END,             ///< reserved
    ENUM_DUMMY4WORD(LENS_DUMMYLOAD)
} LENS_DUMMYLOAD;

typedef enum
{
    LENS_INIT_ZOOM_PART1,           ///< Init ZOOM_PART1
    LENS_INIT_ZOOM_PART2,           ///< Init ZOOM_PART2
    LENS_INIT_APERTURE,             ///< Init APERTURE
    LENS_INIT_FOCUS,                ///< Init FOCUS
    ENUM_DUMMY4WORD(LENS_INIT_STATE)
} LENS_INIT_STATE;

/**
     Enumeration for get zoom section.
*/
typedef enum
{
    ZOOM_MIN_SECTION,         ///< Minimum section
    ZOOM_MAX_SECTION,         ///< Maximum section
    ZOOM_CUR_SECTION,         ///< Currect section
    ENUM_DUMMY4WORD(ZOOM_SECTION_CATEGORY)
} ZOOM_SECTION_CATEGORY;

/**
     Enumeration for get zoom position.
*/
typedef enum
{
    ZOOM_CUR_STEP,             ///< Current step
    ZOOM_CUR_SECTION_STEP,     ///< Step corresponding to currect section
    ENUM_DUMMY4WORD(ZOOM_STEP_CATEGORY)
} ZOOM_STEP_CATEGORY;

/**
     Enumeration for focus opeartion.
*/
typedef enum
{
    FOCUS_PREEXC   = 0x1,      ///< Focus with pre-excitation
    FOCUS_RUN      = 0x2,      ///< Focus with no-excitation
    FOCUS_POSTEXC  = 0x4,      ///< Focus with post-excitation
    ENUM_DUMMY4WORD(FOCUS_CATEGORY)
} FOCUS_CATEGORY;

extern ER         LensCtrl2_Open(PLENSCTRL_APPOBJ pLensCtrlObj);
extern ER         LensCtrl2_Close(void);
extern ER         LensCtrlBG_Open(PLENSCTRL_APPOBJ pLensCtrlObj);
extern ER         LensCtrlBG_Close(void);
extern void       LensCtrlBG_Iqs_SetFlowFunc(void);
extern void       LensCtrlBG_Focus_AfterZoomChanged(void);

extern ER         LensCtrl_Open(PLENSCTRL_APPOBJ pLensCtrlObj);
extern ER         LensCtrl_Close(void);
extern BOOL       Lens_OnCommand(CHAR *pcCmdStr);

/**
     Initialize lens module.

     Mount and initiate lens and motor module.

     @param[in] pLensCtrlObj    Lens control object contains desired lens and motor objects.
*/
extern void       Lens_Module_Init(PLENSCTRL_APPOBJ pLensCtrlObj);

/**
     Get state of lens module.

     @param[in] void
     @return Description of data returned.
         - @b LENS_STATE: state indication of lens module.
*/
extern LENS_STATE Lens_Module_GetState(void);

/**
     Initiate lens.

     @param[in] part                  (LENS_INIT_STATE)
                                       LENS_INIT_ZOOM_PART1, LENS_INIT_ZOOM_PART2, LENS_INIT_APERTURE or LENS_INIT_FOCUS.
     @return Description of data returned.
         - @b UINT32: ERR_OK, ERR_LENS_INIT_FAIL or ERR_OPERATION_FAIL.
*/
extern INT32      Lens_Init(LENS_INIT_STATE part);

extern INT32      Lens_UnInit(void);

/**
     Lens retract back.

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OPERATION_FAIL or ERR_OK.
*/
extern INT32      Lens_Retract(void);

/**
     Reset zoom and focus.

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OPERATION_FAIL or ERR_OK.
*/
extern INT32      Lens_Reset(void);

/**
     Set zoom speed.

     @param[in] category                  (ZOOM_SPEED_CATEGORY)
                                           ZOOM_SPEED_HIGH, ZOOM_SPEED_MEDIUM or ZOOM_SPEED_LOW.
     @return void
*/
extern void       Lens_Zoom_SetSpeed(ZOOM_SPEED_CATEGORY category);

/**
     Get zoom section.

     @param[in] category                  (ZOOM_SECTION_CATEGORY)
                                           ZOOM_MIN_SECTION, ZOOM_MAX_SECTION or ZOOM_CUR_SECTION.
     @return Description of data returned.
         - @b UINT32: section number from wide to tele.
*/
extern UINT32     Lens_Zoom_GetSection(ZOOM_SECTION_CATEGORY category);

/**
     Get zoom step.

     @param[in] category                   (ZOOM_STEP_CATEGORY)
                                           ZOOM_CUR_STEP or ZOOM_CUR_SECTION_STEP.
     @return Description of data returned.
         - @b UINT32: step position.
*/
extern UINT32     Lens_Zoom_GetStep(ZOOM_STEP_CATEGORY category);

/**
     Go to specific zoom section.

     @param[in] section    WIDE to TELE.
*/
extern void       Lens_Zoom_GoSection(UINT32 section);

/**
     Zoom in until Lens_Zoom_Stop().

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OPERATION_FAIL or ERR_OK.
*/
extern INT32      Lens_Zoom_In(void);

/**
     Zoom out until Lens_Zoom_Stop().

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OPERATION_FAIL or ERR_OK.
*/
extern INT32      Lens_Zoom_Out(void);

/**
     Stop zoom.

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OPERATION_FAIL or ERR_OK.
*/
extern INT32      Lens_Zoom_Stop(void);

/**
     Zoom retract.

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OPERATION_FAIL or ERR_OK.
*/
extern INT32      Lens_Zoom_Retract(void);

/**
     Get focal length.

     @param[in] void
     @return Description of data returned.
         - @b UINT32: focal length in 0.1mm unit.
*/
extern UINT32     Lens_Focus_GetLength(UINT32 ZoomSection);

/**
     Get focal length.

     @param[in] void
     @return Description of data returned.
         - @b UINT32: focal length in um unit.
*/
extern UINT32     Lens_Focus_GetLength_um(UINT32 ZoomSection);

/**
     Get current focus position.

     @param[in] void
     @return Description of data returned.
         - @b INT32: current focus position.
*/
extern INT32      Lens_Focus_GetPosition(void);

extern UINT32     Lens_Focus_GetDistTable(UINT32 Section);
extern INT32      Lens_Focus_GetDefaultTable(UINT32 Section, UINT32 FocusDistance);
extern INT32      Lens_Focus_GetTable(UINT32 Section, UINT32 FocusDistance);
extern INT32      Lens_Focus_SetCalData(INT16 *pCalTbl, UINT32 uiTblSize);
extern INT32      Lens_Focus_DoAction(FOCUS_CATEGORY category, INT32 position);
extern INT32      Lens_Focus_AfterZoomChanged(void);

/**
     Get current iris segment.

     @param[in] void
     @return Description of data returned.
         - @b IRIS_POS: current iris segment.
*/
extern IRIS_POS   Lens_Aperture_GetPosition(void);

/**
     Get FNO info.

     @param[in] ZoomSection             zoom section from Wide to Tele
     @param[in] IrisPos                 iris segment
     @return Description of data returned.
         - @b UINT32: FNO.
*/
extern UINT32     Lens_Aperture_GetFNO(UINT32 ZoomSection, UINT32 IrisPos);

/**
     Switch iris.

     @param[in] state     iris segment to be switched       IRIS_POS_BIG to IRIS_POS_SMALL.

     @return Description of data returned.
         - @b INT32: ERR_OPERATION_FAIL or ERR_OK.
*/
extern INT32      Lens_Aperture_Move(IRIS_POS Position);

/**
     Open/close shutter.

     @param[in] state                   MOTOR_SHUTTER_NORMAL for normal operation.
                                        MOTOR_SHUTTER_CAP for capture operation.
     @param[in] act                     SHUTTER_POS_OPEN or SHUTTER_POS_CLOSE.

     @return Description of data returned.
         - @b INT32: ERR_OPERATION_FAIL or ERR_OK.
*/
extern INT32      Lens_Shutter_Move(UINT32 state,UINT32 act);

/**
     Check status of lens specific signal.

     @param[in] signal                   LD_SIGNAL_ZPI, LD_SIGNAL_ZPR or LD_SIGNAL_FPI.

     @return Description of data returned.
         - @b UINT32: 1: high; 0: low.
*/
extern UINT32     Lens_CheckLDSignal(LD_SIGNAL signal);

extern void LensCtrlTsk(void);
extern void LensCtrlTsk2(void);
extern void LensCtrlBGTsk(void);
//@}
#endif
