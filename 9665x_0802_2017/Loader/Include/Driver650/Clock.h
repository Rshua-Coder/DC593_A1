/**
    System Clock APIs header file

    System Clock APIs header.

    @file       clock.h
    @ingroup    mIHALSysCG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef __CLOCK_H
#define __CLOCK_H

#include "IOReg.h"

/** \addtogroup mIHALSysCG */
//@{

/**
    @name NAND Clock Rate

    NAND (SM) Clock Rate

    @note for flash_setFrequency()
*/
//@{
#define SM_HOST_CLOCK_RATE_48_MHZ   (0x00 << 0)     ///< NAND clock rate is 48 MHz
#define SM_HOST_CLOCK_RATE_60_MHZ   (0x01 << 0)     ///< NAND clock rate is 60 MHz
#define SM_HOST_CLOCK_RATE_96_MHZ   (0x02 << 0)     ///< NAND clock rate is 96 MHz
#define SM_HOST_CLOCK_RATE_MASK     (0x07 << 0)
//@}

/**
    @name SPI Clock Rate

    SPI Clock Rate

    @note for flash_setFrequency()
*/
//@{
#define SPI_CLOCK_RATE_24_MHZ       (19 << 0)       ///< SPI clock rate is 24 MHz
#define SPI_CLOCK_RATE_48_MHZ       (9 << 0)        ///< SPI clock rate is 48 MHz
#define SPI_CLOCK_RATE_60_MHZ       (7 << 0)        ///< SPI clock rate is 60 MHz
#define SPI_CLOCK_RATE_80_MHZ       (5 << 0)        ///< SPI clock rate is 80 MHz
//@}

//@}

#endif  // __CLOCK_H
