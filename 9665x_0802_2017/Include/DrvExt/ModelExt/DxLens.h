#ifndef _DXLENS_H_
#define _DXLENS_H_
/**
    DxLens.h

    Lens common interface
    @file       DxLens.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

/**
    @name lens operation error code.

     Error code to indicate condition.
*/
//@{
#define ERR_OK                  0
#define ERR_LENS_INIT_FAIL      (5)
#define ERR_OPERATION_FAIL      (-1)
//@}

/**
    @name shutter control input parameter.

     macro for shutter open/close usage.
*/
//@{
#define OPEN                    1
#define CLOSE                   0
//@}

/**
    @name zoom in/out control parameter.

     macro for zoom in/out control.
*/
//@{
#define ZOOM_IN                 1
#define ZOOM_OUT                0
//@}

/**
    @name zoom inside/outside judgement.

     macro to judge zoom is inside or ouside of HP.
*/
//@{
#define ZOOM_FLAG_OUTSIDE       0
#define ZOOM_FLAG_INSIDE        1
//@}

/**
    @name zoom change notification to DCE.

     1: Enable zoom change notification to DCE.
*/
//@{
#define ZOOM_ENABLE_DCE_NOTIFY  0
//@}

/**
     GPIOArray index.

     Index used to map Lens GPIO. Used in DrvLens_GetDevice().
*/
typedef enum
{
    LENS_IO_MODE_1,
    LENS_IO_MODE_2,
    LENS_IO_MODE_3,
    LENS_IO_IN_0,
    LENS_IO_IN_1,
    LENS_IO_IN_2,
    LENS_IO_IN_3,
    LENS_IO_IN_4,
    LENS_IO_IN_5,
    LENS_IO_IN_6,
    LENS_IO_IN_7,
    LENS_IO_IN_8,
    LENS_IO_IN_9,
    LENS_IO_IN_10,
    LENS_IO_IN_11,

    LENS_IO_PWR,
    LENS_IO_ZOOM_PI,
    LENS_IO_ZOOM_PR,
    LENS_IO_FOCUS_PI,
    LENS_IO_ZOOM_PIINT,
    LENS_IO_FOCUS_PIINT,
    LENS_IO_MOT_EXT1,
    LENS_IO_MOT_EXT2,
    LENS_IO_MOT_EXT3,
    LENS_IO_MAX,
    ENUM_DUMMY4WORD(LENS_MODULE_IO_MAP)
} LENS_MODULE_IO_MAP;

/**
    Reserved.
*/
typedef enum
{
    LENS_ADC_CH_1,
    LENS_ADC_CH_2,
    LENS_ADC_CH_3,
    LENS_ADC_CH_4,
    LENS_ADC_CH_5,
    LENS_ADC_CH_6,
    LENS_ADC_CH_7,
    LENS_ADC_CH_MAX,
    ENUM_DUMMY4WORD(LENS_MODULE_ADC_MAP)
} LENS_MODULE_ADC_MAP;

/**
    Lens device object.
*/
typedef struct _LENS_DEVICE_OBJ
{
    UINT32                  *pGPIOArray;            ///< GPIO pin array for HW connection of DSP, motor driver and lens
    UINT32                  *pADCArray;             ///< Reserved
} LENS_DEVICE_OBJ, *PLENS_DEVICE_OBJ;

/**
    Zoom speed configuration.
*/
typedef enum
{
    ZOOM_SPEED_HIGH,               //Zoom speed hight
    ZOOM_SPEED_MEDIUM,             //Zoom speed medium
    ZOOM_SPEED_LOW,                //Zoom speed low
    ENUM_DUMMY4WORD(ZOOM_SPEED_CATEGORY)
} ZOOM_SPEED_CATEGORY;

/**
     Enumeration to get af searching range.
*/
typedef enum
{
    RANGE_START,               ///< Start of searching range.
    RANGE_END,                 ///< End of searching range.
    ENUM_DUMMY4WORD(FOCUS_RANGE)
} FOCUS_RANGE;

typedef enum
{
    POS_GARAGE,                ///< Retract Focus to garage position.
    POS_HOME,                  ///< Retract Focus to home position.
    ENUM_DUMMY4WORD(FOCUS_RETRACT_POSITION)
} FOCUS_RETRACT_POSITION;

/**
     Iris segments.

     There're total 6 segments of iris to be chose.
*/
typedef enum
{
    IRIS_POS_BIG = 0,
    IRIS_POS_F1_8 = 0,    ///< Fno. 1.8
    IRIS_POS_F2_0,        ///< Fno. 2.0
    IRIS_POS_F2_8,        ///< Fno. 2.8
    IRIS_POS_F4_0,        ///< Fno. 4.0
    IRIS_POS_F5_6,        ///< Fno. 5.6
    IRIS_POS_F8_0,        ///< Fno. 8.0
    IRIS_POS_SMALL = IRIS_POS_F8_0,
    IRIS_POS_CLOSE,       ///< Reserved
    IRIS_POS_RESET,       ///< Iris is in reset position
    IRIS_POS_MAX,
    ENUM_DUMMY4WORD(IRIS_POS)
} IRIS_POS;

/**
     Index for Lens_Shutter_Move().

     Used to open/close shutter.
*/
typedef enum
{
    SHUTTER_POS_CLOSE = 0,  ///< Used to close shutter.
    SHUTTER_POS_OPEN,       ///< Used to open shutter.
    SHUTTER_POS_MAX,
    ENUM_DUMMY4WORD(SHUTTER_POS)
} SHUTTER_POS;

/**
     Index for LensCtrl_SetSignal().
*/
typedef enum
{
    MD_SIGNAL_STANDBY,      ///< Used to power on/off motor driver.
    MD_SIGNAL_ZPI,          ///< Used to enable ZPI signal.
    MD_SIGNAL_ZPR,          ///< Used to enable ZPR signal.
    MD_SIGNAL_FPI,          ///< Used to enable FPI signal.
    ENUM_DUMMY4WORD(MD_SIGNAL)
} MD_SIGNAL;

/**
     Index for Lens_CheckLDSignal().
*/
typedef enum
{
    LD_SIGNAL_ZPI,          ///< Used to get ZPI status.
    LD_SIGNAL_ZPR,          ///< Used to get ZPR status.
    LD_SIGNAL_FPI,          ///< Used to get FPI status.
    ENUM_DUMMY4WORD(LD_SIGNAL)
} LD_SIGNAL;



/**
     Index for LensCtrlTsk().
*/
typedef enum
{
    MOTOR_ZOOM_BRAKE,       ///< Used to break zoom.
    MOTOR_ZOOM_FWD,         ///< Used to forward zoom.
    MOTOR_ZOOM_BWD,         ///< Used to backward zoom.
    MOTOR_ZOOM_OFF,         ///< Used to turn off power for zoom.
    MOTOR_ZOOM_CHGSPEED,
    ENUM_DUMMY4WORD(MOTOR_ZOOM_ACT)
} MOTOR_ZOOM_ACT;

/**
     Index for LensCtrl_Shutter_SetState().
*/
typedef enum
{
    MOTOR_SHUTTER_NORMAL,         ///< Normal opeartion for shutter
    MOTOR_SHUTTER_CAP,            ///< Capture operation for shutter
    MOTOR_SHUTTER_DUMMYLOAD,      ///< Reserved
    MOTOR_SHUTTER_PSEUDO_OPEN,    ///< Reserved
    MOTOR_SHUTTER_PSEUDO_CLOSE,   ///< Reserved
    ENUM_DUMMY4WORD(MOTOR_SHUTTER_ACT)
} MOTOR_SHUTTER_ACT;

/**
     Index for LensCtrl_Aperture_SetState().
*/
typedef enum
{
    MOTOR_APERTURE_NORMAL,         ///< Reserved
    MOTOR_APERTURE_DUMMYLOAD,      ///< Reserved
    MOTOR_APERTURE_FWD,            ///< Used to forward aperture.
    MOTOR_APERTURE_BWD,            ///< Used to backward aperture.
    MOTOR_APERTURE_EXCITE_ON,      ///< Used to pre-excite on aperture motor.
    MOTOR_APERTURE_EXCITE_OFF,     ///< Used to excite off aperture motor.
    MOTOR_APERTURE_RESET,          ///< Used to reset aperture position.
    ENUM_DUMMY4WORD(MOTOR_APERTURE_ACT)
} MOTOR_APERTURE_ACT;

typedef enum
{
    SHUTTER_DUMMYLOAD_START,       ///< Reserved
    SHUTTER_DUMMYLOAD_END,         ///< Reserved
    ENUM_DUMMY4WORD(SHUTTER_DUMMYLOAD_ACT)
} SHUTTER_DUMMYLOAD_ACT;

typedef enum
{
    APERTURE_DUMMYLOAD_START,      ///< Reserved
    APERTURE_DUMMYLOAD_END,        ///< Reserved
    ENUM_DUMMY4WORD(APERTURE_DUMMYLOAD_ACT)
} APERTURE_DUMMYLOAD_ACT;

/**
     Index for LensCtrl_Focus_SetState().
*/
typedef enum
{
    MOTOR_FOCUS_FWD,            ///< Used to forward focus.
    MOTOR_FOCUS_BWD,            ///< Used to backward focus.
    MOTOR_FOCUS_EXCITE_ON,      ///< Used to pre-excite on focus motor.
    MOTOR_FOCUS_EXCITE_OFF,     ///< Used to excite off focus motor.
    MOTOR_FOCUS_CHECK_EXT,      ///< Used to forward focus.
    MOTOR_FOCUS_SETSPEED,       ///< Reserved
    MOTOR_FOCUS_MICRO_STEP_EN,  ///< Used to enable/disable u-step.
    ENUM_DUMMY4WORD(MOTOR_FOCUS_ACT)
} MOTOR_FOCUS_ACT;

/**
    The callback function of lens
*/
typedef void (*FPLENSCALLBACK)(UINT32 MessageID, UINT32 *Parameter);

//------------------------------------------------------------------------------
// Structures
//------------------------------------------------------------------------------
/**
    The configuration object for motor (LensCtrlObj.pMotor = motor_getObject();).
*/
typedef struct _MOTOR_TAB
{
    //Common
    void          (*init)(PLENS_DEVICE_OBJ);
    void          (*setSignal)(MD_SIGNAL,UINT32);                   ///<set signal
    //Zoom
    void          (*zoom_setSpeed)(UINT32);                         ///<set zoom speed duty
    UINT32        (*zoom_getSpeed)(void);                           ///<get zoom speed
    void          (*zoom_setState)(MOTOR_ZOOM_ACT, UINT32);         ///<zoom action
    BOOL          (*zoom_checkPI)(void);                            ///<zoom PI check
    //Focus
    BOOL          (*focus_setState)(MOTOR_FOCUS_ACT, UINT32);       ///<focus action
    //Aperture
    void          (*aperture_setState)(MOTOR_APERTURE_ACT,UINT32);  ///<aperture action
    //Shutter
    void          (*shutter_setState)(MOTOR_SHUTTER_ACT,UINT32);    ///<shutter action
} MOTOR_TAB, *PMOTOR_TAB;

/**
    For DrvExt layer APIs to invoke LibExt layer APIs(LensCtrl_Open, LensCtrl2_Open).
*/
typedef struct
{
    void       (*lensctrltsk_setsignal)(MD_SIGNAL, BOOL);
    void       (*lensctrltsk_zoomoff)(void);
    void       (*lensctrltsk_zoombrake)(void);
    void       (*lensctrltsk_zoomforward)(void);
    void       (*lensctrltsk_zoomreverse)(void);
    void       (*lensctrltsk_zoomchange)(void);
    void       (*lensctrltsk_zoomlockspeed)(BOOL);
    void       (*lensctrltsk_zoomchangespeed)(UINT32);
    UINT32     (*lensctrltsk_zoomgetspeed)(void);
    void       (*lensctrltsk_focus_setstate)(MOTOR_FOCUS_ACT,  UINT32);
    BOOL       (*lensctrltsk_focus_getresult)(void);
    void       (*lensctrltsk_aperture_setstate)(MOTOR_APERTURE_ACT, UINT32);
    void       (*lensctrltsk_shutter_setstate)(MOTOR_SHUTTER_ACT, UINT32);

    void       (*lensctrltsk2_writelensstatus)(UINT32);
    UINT32     (*lensctrltsk2_readlensstatus)(void);
    void       (*lensctrltsk2_waitidle)(void);
    void       (*lensctrltskBG_waitidle)(void);
    BOOL       (*lensctrltskBG_checkidle)(void);
    void       (*lensctrltskBG_iqs_setflowfunc)(void);
    void       (*lensctrltskBG_focus_afterzoomchanged)(void);
} LENSCTRL_TASKAPIOBJ, *PLENSCTRL_TASKAPIOBJ;

/**
    The configuration object for motor (LensCtrlObj.pLens = lens_getObject();).
*/
typedef struct _LENS_TAB
{
    //Common
    void                (*init)(PLENS_DEVICE_OBJ,PMOTOR_TAB);           ///< Init lens and mount motor object
    UINT32              (*getSignal)(LD_SIGNAL);                        ///< get signal
    //Zoom
    void                (*zoom_initPart1)(void);                        ///< Init ZOOM_PART1
    INT32               (*zoom_initPart2)(void);                        ///< Init ZOOM_PART2
    void                (*zoom_setBoundaryCheck)(BOOL);                 ///< Reserved
    void                (*zoom_setSpeed)(ZOOM_SPEED_CATEGORY);          ///< Set ZOOM speed
    UINT32              (*zoom_getMinSection)(void);                    ///< Get minimum ZOOM section
    UINT32              (*zoom_getMaxSection)(void);                    ///< Get maximum ZOOM section
    UINT32              (*zoom_getSection)(void);                       ///< Get current ZOOM section
    UINT32              (*zoom_getStep)(void);                          ///< Get current ZOOM step
    UINT32              (*zoom_getSectionStep)(void);                   ///< Get current ZOOM section step
    void                (*zoom_goSection)(UINT32);                      ///< Reserved
    void                (*zoom_goSection2)(UINT32);                     ///< Go to specific zoom section without backlash compensation
    void                (*zoom_goStep)(INT32);                          ///< Reserved
    INT32               (*zoom_press)(UINT32);                          ///< ZOOM in or out
    UINT32              (*zoom_release)(void);                          ///< ZOOM stop
    INT32               (*zoom_retract)(void);                          ///< ZOOM retract
    //Focus
    INT32               (*focus_init)(void);                            ///< Init FOCUS
    UINT32              (*focus_getLength)(UINT32);                     ///< Get focal length
    INT32               (*focus_getPostion)(void);                      ///< Get current Focus position
    UINT32              (*focus_getDistTable)(UINT32);                  ///< Get table of object distance
    INT32               (*focus_getDefaultTable)(UINT32,UINT32);        ///< Get default table of focus step for each zoom section
    INT32               (*focus_getTable)(UINT32,UINT32);               ///< Get table of focus step for each zoom section
    INT32               (*focus_getRange)(FOCUS_RANGE);                 ///< Reserved
    void                (*focus_doExc)(BOOL,UINT32);                    ///< Focus with pre-excitation or post-excitation.
    void                (*focus_go2)(INT32);                            ///< Go to specific Focus position
    INT32               (*focus_retract)(FOCUS_RETRACT_POSITION);       ///< Focus retract
    //Iris
    void                (*aperture_init)(void);                         ///< Init Aperture
    IRIS_POS            (*aperture_getPosition)(void);                  ///< Get current Aperture position
    UINT32              (*aperture_getFNO)(UINT32,UINT32);              ///< Get FNO
    void                (*aperture_go2Position)(IRIS_POS);              ///< Go to specific Aperture position
    //Shutter
    void                (*shutter_setState)(MOTOR_SHUTTER_ACT,UINT32);  ///< Set Shutter state for normal and capture operations.
    //Misc
    INT32               (*lensRetract)(void);                           ///< Lens retract
    INT32               (*lensReset)(void);                             ///< Lens reset
    void                (*makeAFTable)(void);                           ///< Make AF table
    INT32               (*setCalData)(INT16 *, UINT32);                 ///< Load Focus calibration data
    void                (*focus_afterZoomMoving)(UINT32);               ///< Move zoom once zoom section is changed
    INT32               (*lensPowerOff)(void);                          ///< Turn off lens power
    PMOTOR_TAB          pMotor;                                         ///< Motor object pointer
    PLENSCTRL_TASKAPIOBJ pLensCtrlTskObj;                               ///< For DrvExt layer APIs to invoke LibExt layer APIs(LensCtrl_Open, LensCtrl2_Open).

} LENS_TAB, *PLENS_TAB;

/**
    The configuration object for Lens Control Object (Lens_Module_Init(DrvLens_GetLensCtrlObj());).
*/
typedef struct
{
    PLENS_TAB            pLens;                                             ///< Mount Lens object
    PMOTOR_TAB           pMotor;                                            ///< Mount Motor object
    PLENS_DEVICE_OBJ     pLensDev;                                          ///< Mount Lens IO object
    void                 (*Task1CB)(UINT32 MessageID, UINT32 *Parameter);   ///< Callback to notify ZOOM section was changed
    void                 (*Task2CB)(UINT32 MessageID, UINT32 *Parameter);   ///< Callback to notify UI to record or read Lens status.
    void                 (*APICB)(UINT32 MessageID, UINT32 *Parameter);     ///< Callback to notify UI current state of Lens
    UINT32               semid;                                             ///< Semaphore used for lens module
} LENSCTRL_APPOBJ, *PLENSCTRL_APPOBJ;

/**
    Power on Lens

    @return void
*/
void DrvLens_PowerOn(void);

/**
    Power off Lens

    @return void
*/
void DrvLens_PowerOff(void);

/**
    Get Lens Device

    @return void
*/
PLENS_DEVICE_OBJ DrvLens_GetDevice(void);

/**
    Mount Lens and Motor objects.

    @return void
*/
void DrvLens_PowerOn_Init(void);

/**
    To retrieve Lens Control object.

    @return PLENSCTRL_APPOBJ    Point to Lens Control object
*/
extern PLENSCTRL_APPOBJ DrvLens_GetLensCtrlObj(void);

/**
    To retrieve Motor Object.

    @return PMOTOR_TAB    Point to Motor object
*/
extern PMOTOR_TAB motor_getObject(void);

/**
    To retrieve Lens object.

    @return PMOTOR_TAB    Point to Lens object
*/
extern PLENS_TAB  lens_getObject(void);

#endif //_DXLENS_H_

