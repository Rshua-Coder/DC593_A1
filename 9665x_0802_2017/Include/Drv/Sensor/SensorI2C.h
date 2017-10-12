/**
    @file       SensorI2C.h

    @ingroup    mIDrvSensor
    @brief      Define sensor internal function
    @note      Sensor Driver APIs

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/
#ifndef _SENSOR_I2C_H
#define _SENSOR_I2C_H


#include "type.h"
#include "i2c.h"
#include "top.h"


/**
    I2C physical channel
*/
typedef enum
{
    SENSOR_I2C1_1ST = 0,          ///< I2C channel 1
    SENSOR_I2C1_2ND,              ///< I2C channel 1_2
    SENSOR_I2C2_1ST,              ///< I2C channel 2
    SENSOR_I2C2_2ND,              ///< I2C channel 2_2
    SENSOR_I2C_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_I2C)
} SENSOR_I2C;


typedef enum
{
    I2CFMT_1B1B,
    I2CFMT_1B2B,
    I2CFMT_2B1B,
    I2CFMT_2B2B,
    ENUM_DUMMY4WORD(I2CFMT)
} I2CFMT;
void Sensor_I2C_Open(SENSOR_I2C I2cBus, PI2C_SECTION pSection);
void Sensor_I2C_SetConfig(SENSOR_I2C I2cBus, I2C_SECTION Section);
void Sensor_I2C_SetPinmux(SENSOR_I2C I2cBus, I2C_SECTION Section);
void Sensor_I2C_Close(SENSOR_I2C I2cBus, I2C_SECTION Section);
ER Sensor_I2C_Lock(SENSOR_I2C I2cBus, I2C_SECTION Section);
ER Sensor_I2C_Unlock(SENSOR_I2C I2cBus, I2C_SECTION Section);
I2C_STS Sensor_I2C_Receive(SENSOR_I2C I2cBus, UINT32 *value, BOOL bNACK, BOOL bStop);
I2C_STS Sensor_I2C_Transmit(SENSOR_I2C I2cBus, UINT32 value, BOOL bStart, BOOL bStop);
I2C_STS Sensor_I2CSet_Transmit(SENSOR_I2C I2cBus, UINT32 i2c_w_addr, UINT32 regaddr, UINT32 value, I2CFMT I2CFmt);
I2C_STS Sensor_I2CSet_Receive(SENSOR_I2C I2cBus, UINT32 i2c_w_addr, UINT32 i2c_r_addr, UINT32 regaddr, UINT32 *value, I2CFMT I2CFmt);

#endif