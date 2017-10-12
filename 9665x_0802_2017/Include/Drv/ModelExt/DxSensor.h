#ifndef _DXSENSOR_H_
#define _DXSENSOR_H_
/**
    DxSensor.h

    Sensor common interface
    @file       DxSensor.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "sensor.h"

extern SENSOR_DRVTAB* DrvSensor_GetTab1st(void);
extern SENSOR_INIT_OBJ DrvSensor_GetObj1st(void);

extern SENSOR_DRVTAB* DrvSensor_GetTab2nd(void);
extern SENSOR_INIT_OBJ DrvSensor_GetObj2nd(void);

extern void DrvSensor_TurnOnPower(void);
extern void DrvSensor_TurnOffPower(void);
extern void DrvSensor_PowerSaveOff(void);
extern void DrvSensor_PowerSaveOn(void);
extern BOOL DrvSensor_Det2ndSensor(void);
#endif //_DXSENSOR_H_

