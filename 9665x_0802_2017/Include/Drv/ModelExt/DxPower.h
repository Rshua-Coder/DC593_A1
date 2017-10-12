/**
    System power IO control module.

    This module is the driver of GxPower system. It handles the IO control of battery ADC detection, battery insert and battery charge.

    @file       DxPower.h
    @ingroup    mIDrvPowerCtrl

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _DXPOWER_H_
#define _DXPOWER_H_

#include "type.h"

/**
    @addtogroup mIDrvPowerCtrl
*/
//@{

///////////////////////////////////////////////////////////////////////////////
// Battery
///////////////////////////////////////////////////////////////////////////////

/**
     @name Battery temperature definition
*/
//@{
#define BATTERY_TEMPERATURE_UNKNOWN     -255         ///<  battery not suppot temperature detection
//@}

/**
     @name Battery level definition
*/
//@{
#define DRVPWR_BATTERY_LVL_UNKNOWN      0xFFFFFFFF   ///<  battery level is unknown, the initial value before battery detection start
#define DRVPWR_BATTERY_LVL_EMPTY        5            ///<  battery is empty
#define DRVPWR_BATTERY_LVL_0            0            ///<  battery level 0
#define DRVPWR_BATTERY_LVL_1            1            ///<  battery level 1
#define DRVPWR_BATTERY_LVL_2            2            ///<  battery level 2
#define DRVPWR_BATTERY_LVL_3            3            ///<  battery level 3
#define DRVPWR_BATTERY_LVL_4            4            ///<  battery level 4
//@}

/** Event handler function prototype */
typedef UINT32 (*DX_CALLBACK_PTR)(UINT32 event, UINT32 param1, UINT32 param2);


/**
    Battery charge current value
*/
typedef enum
{
    BATTERY_CHARGE_CURRENT_LOW = 0,                    ///<  charge current low
    BATTERY_CHARGE_CURRENT_MEDIUM,                     ///<  charge current medimum
    BATTERY_CHARGE_CURRENT_HIGH,                       ///<  charge current high
    ENUM_DUMMY4WORD(BATTERY_CHARGE_CURRENT)
}   BATTERY_CHARGE_CURRENT;


////////////////////////////////////////////////////////////////////////////////
/**
     Dirver Power control ID
*/
typedef enum
{
    DRVPWR_CTRL_BATTERY_LEVEL = 0,                      ///< Read battery level   (RO)
    DRVPWR_CTRL_BATTERY_ADC_VALUE,                      ///< Read current battery adc value (RO)
    DRVPWR_CTRL_BATTERY_CALIBRATION_EN,                 ///< Enable or disable battery calibraion mode (RW)
    DRVPWR_CTRL_BATTERY_ADC_CAL_OFFSET,                 ///< Batt ADC calibration offset value(RW)
    DRVPWR_CTRL_IS_DUMMUYLOAD_POWEROFF,                 ///< if dummy load check battery fail need to power off (RO)
    DRVPWR_CTRL_IS_BATT_OVERHEAT,                       ///< if battery temperature overheat (RO)
    DRVPWR_CTRL_IS_BATT_INSERT,                         ///< battery insert status (RO)
    DRVPWR_CTRL_IS_DEAD_BATT,                           ///< battery power status (RO)
    DRVPWR_CTRL_IS_NEED_RECHARGE,                       ///< check battery ADC status if need recharge (RO)
    DRVPWR_CTRL_BATTERY_CHARGE_EN,                      ///< battery charge function enable, disable
    DRVPWR_CTRL_BATTERY_CHARGE_CURRENT,                 ///< battery charge current set ,get(RW)
    DRVPWR_CTRL_BATTERY_CHARGE_ISET,                    ///< battery charge current set ,get(RW)
    DRVPWR_CTRL_BATTERY_CHARGE_VSET,                    ///< battery charge voltage set ,get(RW)
    DRVPWR_CTRL_BATTERY_CHARGE_OK,                      ///< battery charge ok status get (RO)
    DRVPWR_CTRL_BATTERY_TEMPERATURE,                    ///< battery temperature get (RO)
    DRVPWR_CTRL_ENABLE_CHARGEIC,
    DRVPWR_CTRL_PWRON_SOURCE,                           ///< poweron source get (RO)
    DRVPWR_CTRL_PWR_EN,                                 ///< set 0 to power off (WO)
    DRVPWR_CTRL_PIR_EN,                                 ///< PIR enable (WO)
    DRVPWR_CTRL_PWR_ALM,                                ///< power alarm (WO)
    ENUM_DUMMY4WORD(DRVPWR_CTRL)
}DRVPWR_CTRL;

/**
     @name Driver Power Callback event
*/
//@{
#define DRVPWR_CB_IS_LENS_MOVING             0          ///<  Check is lens moving
#define DRVPWR_CB_DUMMY_LOAD_START           1          ///<  Dummy load start
#define DRVPWR_CB_DUMMY_LOAD_END             2          ///<  Dummy load end
//@}

/**
    Dummy load Info
*/
typedef struct _DummyLoadType
{
    UINT32 deltaV;                                      ///<  The delta voltage of dummy load
    UINT32 Ave_V;                                       ///<  The average battery voltage after dummy load stop
}DummyLoadType;

/**
     Driver power on init.

*/
extern void   DrvPower_PowerOnInit(void);

/**
    Do the dummy loading.

    The dummy load is to check the battery internal resistance and the battery voltage can power on or not.
    User can use the shutter to do the dummy load if the shutter coil is constant current when excited.
    When the dummy load is opened the battery voltage will have a drop and after the dummy load is closed the battery
    voltage will increase gradually.

    @return  TRUE:  battery status is ok can power on.
             FALSE: battery status is not ok can't power on.
*/
extern UINT32 DrvPower_DummyLoad(void);

/**
    Get DrvPower setting value.

    @param[in] DrvPwrCtrl:  setting ID
    @return the setting value.

*/
extern UINT32 DrvPower_GetControl(DRVPWR_CTRL DrvPwrCtrl);

/**
    Set DrvPower setting value.

    @param[in] DrvPwrCtrl:  setting ID
    @param[in] value:  the setting value

*/
extern void   DrvPower_SetControl(DRVPWR_CTRL DrvPwrCtrl, UINT32 value);

/**
    Get battery temperature value.

    @return the battery temperature value.
*/
extern INT32  DrvPower_BattTempGet(void);

/**
    Get PCB  board ver  value.

    @return the PCB  board ver value.
*/
extern INT32  DrvPower_PCB_VER_Get(void);

/**
     Register callback function.

     @note User must register callback funcion then can receive the Driver Power Callback event DRVPWR_CB_XXX.

     @param[in] fpDxPowerCB: the callback function pointer.
*/
extern void   DrvPower_RegCB(DX_CALLBACK_PTR fpDxPowerCB);

//@}
#endif //_DXPOWER_H_

