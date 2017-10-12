/**
    HW clock utility.

    HW clock utility.

    @file       HwClock.h
    @ingroup    mIUtilHWClock
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _HWCLOCK_H
#define _HWCLOCK_H

#include "Type.h"
#include <time.h>

/**
    @addtogroup mIUtilHWClock
*/
//@{

/**
     Check power of hw clock

     Check power of hw clock

     @return Power is losted or not.
*/
extern BOOL HwClock_IsPowerLost(void);

/**
     Get current time

     Get current time

     @return Get current time.
*/
extern struct tm HwClock_GetCurrentTime(void);

/**
     Set current time

     Set current time

     @param[in] ctv     Set current time.
*/
extern void HwClock_SetCurrentTime(struct tm ctv);

/**
     Get alarm time

     Get alarm time

     @return Get alarm time.
*/
extern struct tm HwClock_GetAlarmTime(void);

/**
     Set alarm time

     Set alarm time

     @param[in] ctv     Set alarm time.
     @param[in] pEventCB     Event callback function pointer.
*/
extern void HwClock_SetAlarmTime(struct tm ctv, FP pEventCB);

/**
     Get power alarm time

     Get power alarm time

     @return Power alarm time.
*/
extern struct tm HwClock_GetPowerAlarmTime(void);

/**
     Set power alarm time

     Set power alarm time

     @param[in] ctv     Set power alarm time.
*/
extern void HwClock_SetPowerAlarmTime(struct tm ctv);

/**
     Enable or disable power alarm time

     Enable or disable power alarm time

     @param[in] bEnable     Enable or disable.
*/
extern void HwClock_EnablePowerAlarm(BOOL bEnable);

/**
     Check power alarm time is enable or disable.

     Check power alarm time is enable or disable.

     @return Enable or disable.
*/
extern BOOL HwClock_IsEnablePowerAlarm(void);

/**
     Calculate maximum days number of given (year, month).

     Calculate maximum days number of given (year, month).

     @param[in] year      Year.
     @param[in] month     Month

     @return Days number.
*/
extern INT32 TimeUtil_CalcMonthDays(INT32 year, INT32 month);

/**
     Convert date-time value to days number.

     Convert date-time value to days number.

     @param[in] ctv     Date time value.

     @return Days number. [second precison]
*/
extern UINT32 TimeUtil_Value2Days(struct tm ctv);

/**
     Convert days number to date-time value.

     Convert days number to date-time value.

     @param[in] days    Days number.

     @return Date time value. [day precison]
*/
extern struct tm TimeUtil_Days2Value(UINT32 days);

/**
     Calculate sum of date-time value and another differece

     Calculate sum of date-time value and another differece

     @param[in] ctv     Date time value.
     @param[in] diff    Difference (date time value).

     @return Sum (date time value). [second precison]
*/
extern struct tm TimeUtil_ValueAddDiff(struct tm ctv, struct tm diff);

/**
    Install uart command to SxCmd

    Install uart command to SxCmd
*/
extern void HwClock_InstallCmd(void);

/**
    Install uart command to SxCmd (internal API)

    Install uart command to SxCmd (internal API)
*/
extern void HwClk_InstallCmd(void);

//@}

#endif //_HWCLOCK_H
