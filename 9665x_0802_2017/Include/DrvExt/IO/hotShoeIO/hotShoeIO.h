/**
    Header file of hot shoe IO module driver

    This file is the header file of hot shoe IO module

    @file       hotshoeIO.h
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#ifndef _HOTSHOE_IO_H
#define _HOTSHOE_IO_H

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
    HOTSHOEIO_BIT_ORDER_LSB,        ///< LSB bit first transmitted
    HOTSHOEIO_BIT_ORDER_MSB,        ///< MSB bit first transmitted

    ENUM_DUMMY4WORD(HOTSHOEIO_BIT_ORDER)
} HOTSHOEIO_BIT_ORDER;

/**
    Bus Mode select
*/
typedef enum
{
    HOTSHOEIO_BUSMODE_0,            ///< Mode 0
    HOTSHOEIO_BUSMODE_3,            ///< Mode 3

    ENUM_DUMMY4WORD(HOTSHOEIO_BUSMODE)
} HOTSHOEIO_BUSMODE;

/**
    Hot shoe IO Configuration Identifier

    @note For hotShoeIO_setConfig()
*/
typedef enum
{
    HOTSHOEIO_CONFIG_ID_FREQ,       ///< Bus Frequency. Unit: Hz (default: 100000)
                                    ///< When bus frequency < 1.875MHz, GPIO will be used to simulate hot shoe IO protocol
                                    ///< When bus frequency >= 1.875MHz, SPI HW will be used to simulate hot shoe IO protocol

    HOTSHOEIO_CONFIG_ID_CS_GPIO,    ///< GPIO for CS
    HOTSHOEIO_CONFIG_ID_CLK_GPIO,   ///< GPIO for clock
    HOTSHOEIO_CONFIG_ID_DATA_GPIO,  ///< GPIO for data (Exclusive with HOTSHOEIO_CONFIG_ID_DATAOUT_GPIO/HOTSHOEIO_CONFIG_ID_DATAIN_GPIO)
    HOTSHOEIO_CONFIG_ID_DATAOUT_GPIO, ///< GPIO for data out (Exclusive with HOTSHOEIO_CONFIG_ID_DATA_GPIO)
    HOTSHOEIO_CONFIG_ID_DATAIN_GPIO,///< GPIO for data in (Exclusive with HOTSHOEIO_CONFIG_ID_DATA_GPIO)

    HOTSHOEIO_CONFIG_ID_BIT_ORDER,  ///< LSB bit or MSB bit first transmitted on bus
                                    ///< - @b HOTSHOEIO_BIT_ORDER_LSB: LSB first (default)
                                    ///< - @b HOTSHOEIO_BIT_ORDER_MSB: MSB first

    HOTSHOEIO_CONFIG_ID_LOCK_CPU,   ///< Lock CPU behavior (when GPIO is used to simulate hot shoe IO protocol)
                                    ///< - @b FALSE: don't lock CPU when toggle clock(CPU has better utilization)
                                    ///< - @b TRUE: lock CPU when toggle clock (clock has minimum jitter) (default)

    HOTSHOEIO_CONFIG_ID_SPI_ID,     ///< SPI controller ID if H/W SPI is used
                                    ///< - @b SPI_ID_1: SPI 1
                                    ///< - @b SPI_ID_2: SPI 2 (default)
                                    ///< - @b SPI_ID_3: SPI 3

    HOTSHOEIO_CONFIG_ID_BUSMODE,    ///< CLK/Data Mode
                                    ///< - @b HOTSHOEIO_BUSMODE_0: mode 0 (default)
                                    ///< - @b HOTSHOEIO_BUSMODE_3: mode 3

    ENUM_DUMMY4WORD(HOTSHOEIO_CONFIG_ID)
} HOTSHOEIO_CONFIG_ID;


extern ER       hotShoeIO_open(void);
extern ER       hotShoeIO_close(void);
extern void     hotShoeIO_setCSActive(BOOL bCSActive);
extern ER       hotShoeIO_sendByte(UINT32 uiData);
extern ER       hotShoeIO_receiveByte(UINT32* pData);
extern ER       hotShoeIO_setConfig(HOTSHOEIO_CONFIG_ID configID, UINT32 configContext);

#endif
