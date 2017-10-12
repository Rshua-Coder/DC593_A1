/**
    Header file for Interrupt module

    This file is the header file that define the API for Interrupt module.

    @file       Interrupt.h
    @ingroup    mIHALInterrupt
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include "Type.h"

/**
    @addtogroup mIHALInterrupt
*/
//@{

/**
    Interrupt module ID

    Interrupt module ID for int_enable() and int_disable().
*/
typedef enum
{
    INT_ID_00,  ///< Timer
    INT_ID_01,  ///< SIE, SIE2
    INT_ID_02,  ///< PRE
    INT_ID_03,  ///< IPE
    INT_ID_04,  ///< IME
    INT_ID_05,  ///< DCE, IFE, IFE2
    INT_ID_06,  ///< DIS, FDE, RDE
    INT_ID_07,  ///< DAI
    INT_ID_08,  ///< H.264
    INT_ID_09,  ///< JPEG
    INT_ID_10,  ///< Graphic, Graphic2, Affine
    INT_ID_11,  ///< ISE
    INT_ID_12,  ///< GPIO, Remote
    INT_ID_13,  ///< PWM
    INT_ID_14,  ///< USB
    INT_ID_15,  ///< NAND
    INT_ID_16,  ///< SDIO
    INT_ID_17,  ///< SDIO2
    INT_ID_18,  ///< DRAM Controller (DMA)
    INT_ID_19,  ///< SPI
    INT_ID_20,  ///< SPI2, SPI3
    INT_ID_21,  ///< SIF
    INT_ID_22,  ///< I2C, I2C2
    INT_ID_23,  ///< UART, UART2
    INT_ID_24,  ///< ADC
    INT_ID_25,  ///< IDE, IDE2
    INT_ID_26,  ///< MIPI-DSI, MI
    INT_ID_27,  ///< HDMI
    INT_ID_28,  ///< LVDS & HiSPi, MIPI-CSI, MIPI-CSI2
    INT_ID_29,  ///< RTC
    INT_ID_30,  ///< WDT
    INT_ID_31,  ///< CG

    INT_ID_MAX,
    ENUM_DUMMY4WORD(INT_ID)
} INT_ID;

extern UINT32   int_getEnable(void);
extern UINT32   int_getFlag(void);
extern INT_ID   int_getIRQId(void);
extern INT_ID   int_getDummyId(void);
extern UINT32   int_getLatency(void);

//@}

#endif
