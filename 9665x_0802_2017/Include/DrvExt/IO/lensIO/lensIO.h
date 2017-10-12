/**
    Header file of lens IO module driver

    This file is the header file of lens IO module

    @file       lensIO.h
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#ifndef _LENS_IO_H
#define _LENS_IO_H

#include "Type.h"
#include "gpio.h"
#include "timer.h"
#include "spi.h"
#include "ErrorNo.h"

/**
    Bit order select
*/
typedef enum
{
    LENSIO_BIT_ORDER_LSB,           ///< LSB bit first transmitted
    LENSIO_BIT_ORDER_MSB,           ///< MSB bit first transmitted

    ENUM_DUMMY4WORD(LENSIO_BIT_ORDER)
} LENSIO_BIT_ORDER;

/**
    Bus Mode select
*/
typedef enum
{
    LENSIO_BUSMODE_0,               ///< Mode 0
    LENSIO_BUSMODE_3,               ///< Mode 3

    ENUM_DUMMY4WORD(LENSIO_BUSMODE)
} LENSIO_BUSMODE;

/**
    Lens IO Configuration Identifier

    @note For lensIO_setConfig()
*/
typedef enum
{
    LENSIO_CONFIG_ID_FREQ,          ///< Bus Frequency. Unit: Hz (default: 100000)
                                    ///< When bus frequency <= 500KHz, GPIO will be used to simulate lens IO protocol
                                    ///< When bus frequency >= 1.875MHz, SPI HW will be used to simulate lens IO protocol

    LENSIO_CONFIG_ID_CS_GPIO,       ///< GPIO for CS
    LENSIO_CONFIG_ID_INT_GPIO,      ///< GPIO for INT
    LENSIO_CONFIG_ID_CLK_GPIO,      ///< GPIO for clock
    LENSIO_CONFIG_ID_DATA_GPIO,     ///< GPIO for data
    LENSIO_CONFIG_ID_DATAOUT_GPIO,  ///< GPIO for data out
    LENSIO_CONFIG_ID_DATAIN_GPIO,   ///< GPIO for data in

    LENSIO_CONFIG_ID_BIT_ORDER,     ///< LSB bit or MSB bit first transmitted on bus
                                    ///< - @b LENSIO_BIT_ORDER_LSB: LSB first (default)
                                    ///< - @b LENSIO_BIT_ORDER_MSB: MSB first

    LENSIO_CONFIG_ID_LOCK_CPU,      ///< Lock CPU behavior (when GPIO is used to simulate lens IO protocol)
                                    ///< - @b FALSE: don't lock CPU when toggle clock(CPU has better utilization)
                                    ///< - @b TRUE: lock CPU when toggle clock (clock has minimum jitter) (default)

    LENSIO_CONFIG_ID_SPI_ID,        ///< SPI controller ID if H/W SPI is used
                                    ///< - @b SPI_ID_1: SPI 1
                                    ///< - @b SPI_ID_2: SPI 2 (default)
                                    ///< - @b SPI_ID_3: SPI 3

    LENSIO_CONFIG_ID_BUSMODE,       ///< CLK/Data Mode
                                    ///< - @b LENSIO_BUSMODE_0: mode 0 (default)
                                    ///< - @b LENSIO_BUSMODE_3: mode 3

    ENUM_DUMMY4WORD(LENSIO_CONFIG_ID)
} LENSIO_CONFIG_ID;


extern ER       lensIO_open(void);
extern ER       lensIO_close(void);
extern void     lensIO_setCSActive(BOOL bCSActive);
extern ER       lensIO_sendByte(UINT32 uiData);
extern ER       lensIO_receiveByte(UINT32* pData);
//extern ER       lensIO_readData(UINT32 uiCommand, UINT32 uiReadLength, UINT8* pBuf);
extern ER       lensIO_setConfig(LENSIO_CONFIG_ID configID, UINT32 configContext);

#endif
