/**
    Header file for I2C module

    This file is the header file that define the API and data type for I2C module.

    @file       i2c.h
    @ingroup    mIIOI2C
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _I2C_H
#define _I2C_H

#include "Driver.h"

/**
    @addtogroup mIIOI2C
*/
//@{

/**
    I2C session ID

    I2C session ID for related APIs.
*/
typedef enum
{
    I2C_SES0,                           ///< I2C session 0
    I2C_SES1,                           ///< I2C session 1
    I2C_SES2,                           ///< I2C session 2
    I2C_SES3,                           ///< I2C session 3
    I2C_SES4,                           ///< I2C session 4
    I2C_SES5,                           ///< I2C session 5
    I2C_TOTAL_SESSION,                  ///< I2C total session number

    ENUM_DUMMY4WORD(I2C_SESSION)
} I2C_SESSION, *PI2C_SESSION;

// The following naming is obsoleted, don't use any more!
#if 1
    #define I2C_SEC0            I2C_SES0
    #define I2C_SEC1            I2C_SES1
    #define I2C_TOTAL_SECTION   I2C_TOTAL_SESSION
    #define I2C_SECTION         I2C_SESSION
    #define PI2C_SECTION        PI2C_SESSION
#endif

/**
    I2C mode

    I2C operating mode.
*/
typedef enum
{
    I2C_MODE_SLAVE,                     ///< Slave mode
    I2C_MODE_MASTER,                    ///< Master mode (Controller will output SCL)

    ENUM_DUMMY4WORD(I2C_MODE)
} I2C_MODE, *PI2C_MODE;

/**
    I2C pinmux

    I2C pinmux.
*/
typedef enum
{
    I2C_PINMUX_1ST,                     ///< 1st pinmux (I2C_1_XXX or I2C2_1_XXX)
    I2C_PINMUX_2ND,                     ///< 2nd pinmux (I2C_2_XXX or I2C2_2_XXX)

    ENUM_DUMMY4WORD(I2C_PINMUX)
} I2C_PINMUX, *PI2C_PINMUX;

/**
    I2C slave addressing mode

    I2C slave addressing mode.
*/
typedef enum
{
    I2C_SAR_MODE_7BITS,                 ///< 7 bits slave addressing mode
    I2C_SAR_MODE_10BITS,                ///< 10 bits slave addressing mode

    ENUM_DUMMY4WORD(I2C_SAR_MODE)
} I2C_SAR_MODE, *PI2C_SAR_MODE;

/**
    I2C slave address length

    I2C slave address length for i2c_waitSARMatch().
*/
typedef enum
{
    I2C_SAR_LEN_1BYTE = 1,              ///< 1 byte  SAR
    I2C_SAR_LEN_2BYTES,                 ///< 2 bytes SAR

    ENUM_DUMMY4WORD(I2C_SAR_LEN)
} I2C_SAR_LEN, *PI2C_SAR_LEN;

/**
    I2C master mode bus clock

    I2C master mode pre-defined bus clock
*/
typedef enum
{
    I2C_BUS_CLOCK_50KHZ     =   50000,  ///< 50 KHz
    I2C_BUS_CLOCK_100KHZ    =  100000,  ///< 100 KHz
    I2C_BUS_CLOCK_200KHZ    =  200000,  ///< 200 KHz
    I2C_BUS_CLOCK_400KHZ    =  400000,  ///< 400 KHz
    I2C_BUS_CLOCK_1MHZ      = 1000000,  ///< 1 MHz

    ENUM_DUMMY4WORD(I2C_BUS_CLOCK)
} I2C_BUS_CLOCK, *PI2C_BUS_CLOCK;

/**
    I2C transmit/receive byte counter for PIO mode

    I2C transmit/receive byte counter for PIO mode.
*/
typedef enum
{
    I2C_BYTE_CNT_1 = 1,
    I2C_BYTE_CNT_2,
    I2C_BYTE_CNT_3,
    I2C_BYTE_CNT_4,

    ENUM_DUMMY4WORD(I2C_BYTE_CNT)
} I2C_BYTE_CNT, *PI2C_BYTE_CNT;

/**
    I2C transmit/receive byte parameter for PIO mode

    I2C transmit/receive byte parameter for PIO mode.
*/
typedef enum
{
    I2C_BYTE_PARAM_NONE     = 0x00000000,   ///< Dummy, for slave transmit or master transmit without START or STOP
    I2C_BYTE_PARAM_ACK      = 0x00000000,   ///< Receive mode only, response ACK after data is received
    I2C_BYTE_PARAM_NACK     = 0x00000001,   ///< Receive mode only, response NACK after data is received
    I2C_BYTE_PARAM_START    = 0x00000004,   ///< Master mode only, generate START condition before transmit data
    I2C_BYTE_PARAM_STOP     = 0x00000008,   ///< Master mode only, generate STOP condition after data is transmitted or received
                                            ///< @note  Can't generate STOP and START at the same byte

    ENUM_DUMMY4WORD(I2C_BYTE_PARAM)
} I2C_BYTE_PARAM, *PI2C_BYTE_PARAM;

/**
    I2C transmit/receive byte data and parameter for PIO mode

    I2C transmit/receive byte data and parameter for PIO mode.
*/
typedef struct
{
    UINT32          uiValue;                ///< 8-bits data to transmit or receive
    I2C_BYTE_PARAM  Param;                  ///< Parameter to transmit or receive data
} I2C_BYTE, *PI2C_BYTE;

/**
    I2C transmit / receive parameter for PIO mode

    I2C transmit / receive parameter for PIO mode.
*/
typedef struct
{
    DRV_VER_INFO    VersionInfo;            ///< Driver version
    I2C_BYTE_CNT    ByteCount;              ///< How many bytes will be transmitted / received
    I2C_BYTE        Byte[4];                ///< Byte data and parameter
} I2C_DATA, *PI2C_DATA;

/**
    I2C Configuration ID

    I2C Configuration ID for i2c_setConfig() and i2c_getConfig()
*/
typedef enum
{
    I2C_CONFIG_ID_MODE,             ///< Mast or Slave mode, This is channel independent setting. Please config once after power on.
                                    ///< - @b I2C_MODE_SLAVE    : Slave mode
                                    ///< - @b I2C_MODE_MASTER   : Master mode (Default value)

    I2C_CONFIG_ID_BUSCLOCK,         ///< Master mode bus clock. Please use pre-defined bus clock if possible.
                                    ///< - @b I2C_BUS_CLOCK_50KHZ
                                    ///< - @b I2C_BUS_CLOCK_100KHZ
                                    ///< - @b I2C_BUS_CLOCK_200KHZ
                                    ///< - @b I2C_BUS_CLOCK_400KHZ
                                    ///< - @b I2C_BUS_CLOCK_1MHZ
                                    ///< @note  Valid value: 50,000 ~ 1,000,000 , unit: Hz.
                                    ///<        Real bus clock might lower than your setting.
                                    ///<        Please call i2c_getConfig(I2C_CONFIG_ID_BUSCLOCK) to confirm.

    I2C_CONFIG_ID_SCL_TIMEOUT,      ///< SCL timeout value. When SCL is kept low over this value, an event will be issued.
                                    ///< @note  Valid value: 819,200 ~ 838,041,600 , unit: ns. Default: 838,041,600 ns.
                                    ///<        Real timeout might greater than your setting.
                                    ///<        Please call i2c_getConfig(I2C_CONFIG_ID_SCL_TIMEOUT) to confirm.

    I2C_CONFIG_ID_BUSFREE_TIME,     ///< Bus free time between STOP and START condition.
                                    ///< @note  Valid value: 20 ~ 10,645 , unit: ns. Default: 4708 ns.
                                    ///<        Real timeout might greater than your setting.
                                    ///<        Please call i2c_getConfig(I2C_CONFIG_ID_BUSFREE_TIME) to confirm.

    I2C_CONFIG_ID_PINMUX,           ///< Select pinmux
                                    ///< - @b I2C_PINMUX_1ST (Default value)
                                    ///< - @b I2C_PINMUX_2ND

    I2C_CONFIG_ID_HANDLE_NACK,      ///< Handle NACK from slave
                                    ///< - @b TRUE  : Send STOP and back to idle (Default value)
                                    ///< - @b FALSE : Do nothing

    I2C_CONFIG_ID_SAR,              ///< Slave mode address
                                    ///< @note  Please refer to I2C specification for valid slave address

    I2C_CONFIG_ID_SAR_MODE,         ///< I2C slave addressing mode
                                    ///< - @b I2C_SAR_MODE_7BITS    : 7 bits addressing mode (Default value)
                                    ///< - @b I2C_SAR_MODE_10BITS   : 10 bits addressing mode

    I2C_CONFIG_ID_GC,               ///< Response general call at slave mode
                                    ///< - @b TRUE  : Response general call
                                    ///< - @b FALSE : Don't response general call (Default value)

    I2C_CONFIG_ID_CLKDIV = 0x1000,  ///< Master mode bus clock divider, Formula: SCL = 48MHz / ((2 * CLKDIV) + 6 + GSR)
                                    ///< @note  Reserved for engineering

    I2C_CONFIG_ID_TSR,              ///< Drive SDA after (TSR + 4) clock cycles when SCL goes low
                                    ///< @note  Reserved for engineering

    I2C_CONFIG_ID_GSR,              ///< Suppress glitch <= GSR clock cycles for SDA and SCL
                                    ///< @note  Reserved for engineering

    I2C_CONFIG_ID_SCLTIMEOUT_CHECK, ///< Enable/Disable the SCL Timeout Check. Default is enabled.

    ENUM_DUMMY4WORD(I2C_CONFIG_ID)
} I2C_CONFIG_ID, *PI2C_CONFIG_ID;

/**
    I2C status

    I2C status for transmit and receive. Only ONE of these base status could be returned.
*/
typedef enum
{
    I2C_STS_OK,                     ///< (Base status) Status OK
    I2C_STS_INVALID_PARAM,          ///< (Base status) Invalid parameter
    I2C_STS_NACK,                   ///< (Base status) Receive NACK
    I2C_STS_BUS_NOT_AVAILABLE,      ///< (Base status) Bus is not avaliable
    I2C_STS_AL,                     ///< (Base status) Arbitration lost
    I2C_STS_SAM,                    ///< (Base status) Slave address matched, for i2c_waitSARMatch()
    I2C_STS_GC,                     ///< (Base status) Receive general call, for i2c_waitSARMatch()

    I2C_STS_STOP = 0x80000000,      ///< (Extra status) Detect STOP at slave mode
                                    ///< You might get (I2C_STS_NACK | I2C_STS_STOP) or (I2C_STS_NACK | I2C_STS_STOP)

    ENUM_DUMMY4WORD(I2C_STS)
} I2C_STS, *PI2C_STS;

/**
    I2C R/W bit

    I2C R/W bit, could be read or write
*/
typedef enum
{
    I2C_RWBIT_WRITE,                ///< Write, indicate master-transmit or slave-receive mode
    I2C_RWBIT_READ,                 ///< Read, indicate master-receive or slave-transmit mode

    ENUM_DUMMY4WORD(I2C_RWBIT)
} I2C_RWBIT;

// I2C macro to convert SAR to data
// Convert 7-bits slave address to data
#define i2c_conv7bSARToData(S)      ((S) << 1)
// Convert 10-bits slave address to 1st byte data
#define i2c_conv10bSARTo1stData(S)  ((((S) & 0x300) >> 7) | 0xF0)
// Convert 10-bits slave address to 2nd byte data
#define i2c_conv10bSARTo2ndData(S)  ((S) & 0x0FF)

// Get base status from I2C_STS
#define i2c_getBaseStatus(S)        ((S) & ~0xF0000000)
// Get extra status from I2C_STS
#define i2c_getExtraStatus(S)       ((S) & 0xF0000000)

// I2C public APIs
extern ER           i2c_open(PI2C_SESSION pSession);
extern ER           i2c_close(I2C_SESSION Session);
extern BOOL         i2c_isOpened(I2C_SESSION Session);

extern ER           i2c_lock(I2C_SESSION Session);
extern ER           i2c_unlock(I2C_SESSION Session);

extern void         i2c_setConfig(I2C_SESSION Session, I2C_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32       i2c_getConfig(I2C_SESSION Session, I2C_CONFIG_ID ConfigID);

extern I2C_STS      i2c_transmit(PI2C_DATA pData);
extern I2C_STS      i2c_receive(PI2C_DATA pData);

extern I2C_RWBIT    i2c_getRWBit(void);

extern I2C_STS      i2c_transmitDMA(UINT32 uiAddr, UINT32 uiSize);
extern I2C_STS      i2c_receiveDMA(UINT32 uiAddr, UINT32 uiSize);

extern I2C_STS      i2c_waitSARMatch(I2C_SAR_LEN SARLen);

// I2C2 public APIs
extern ER           i2c2_open(PI2C_SESSION pSession);
extern ER           i2c2_close(I2C_SESSION Session);
extern BOOL         i2c2_isOpened(I2C_SESSION Session);

extern ER           i2c2_lock(I2C_SESSION Session);
extern ER           i2c2_unlock(I2C_SESSION Session);

extern void         i2c2_setConfig(I2C_SESSION Session, I2C_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32       i2c2_getConfig(I2C_SESSION Session, I2C_CONFIG_ID ConfigID);

extern I2C_STS      i2c2_transmit(PI2C_DATA pData);
extern I2C_STS      i2c2_receive(PI2C_DATA pData);

extern I2C_RWBIT    i2c2_getRWBit(void);

//@}

#endif
