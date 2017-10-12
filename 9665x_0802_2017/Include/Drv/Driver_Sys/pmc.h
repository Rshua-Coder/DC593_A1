/**
    Header file for PMC module

    This file is the header file that define the API and data type for PMC module.

    @file       pmc.h
    @ingroup    mIMISCPMC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _PMC_H
#define _PMC_H

#include "Driver.h"

#define PMC_MODULE_PD1      0   ///< Power domain 1 starting ID
#define PMC_MODULE_PD2      32  ///< Power domain 2 starting ID
#define PMC_MODULE_PD3      64  ///< Power domain 3 starting ID (Reserved in NT96650)

/**
    @addtogroup mIMISCPMC
*/
//@{

/**
    PMC module ID

    PMC module ID.
*/
typedef enum
{
    PMC_MODULE_LVDS         = PMC_MODULE_PD1,   ///< LVDS / HiSPi
    PMC_MODULE_MIPI_CSI,                        ///< MIPI - CSI
    PMC_MODULE_MIPI_CSI2,                       ///< MIPI - CSI2
    PMC_MODULE_SIE,                             ///< SIE
    PMC_MODULE_SIE2,                            ///< SIE2
    PMC_MODULE_PRE,                             ///< PRE
    PMC_MODULE_IPE,                             ///< IPE
    PMC_MODULE_DIS,                             ///< DIS
    PMC_MODULE_IFE2,                            ///< IFE2
    PMC_MODULE_IME,                             ///< IME
    PMC_MODULE_ISE,                             ///< ISE
    PMC_MODULE_JPEG,                            ///< JPEG
    PMC_MODULE_GRAPHIC,                         ///< Graphic
    PMC_MODULE_GRAPHIC2,                        ///< Graphic2
    PMC_MODULE_FDE,                             ///< FDE
    PMC_MODULE_DCE,                             ///< DEC
    PMC_MODULE_IFE,                             ///< IFE
    PMC_MODULE_MI,                              ///< MI
    PMC_MODULE_MIPI_DSI,                        ///< MIPI - DSI
    PMC_MODULE_HDMI,                            ///< HDMI
    PMC_MODULE_TV,                              ///< TV
    PMC_MODULE_AFFINE,                          ///< Affine
    PMC_MODULE_H264         = PMC_MODULE_PD2,   ///< H264

    ENUM_DUMMY4WORD(PMC_MODULE)
} PMC_MODULE, *PPMC_MODULE;

/**
    PMC module status

    PMC module status.
*/
typedef enum
{
    PMC_PWRSTS_OFF,         ///< Power is OFF
    PMC_PWRSTS_ON,          ///< Power is ON

    ENUM_DUMMY4WORD(PMC_PWRSTS)
} PMC_PWRSTS, *PPMC_PWRSTS;

// PMC public APIs
extern PMC_PWRSTS  pmc_getPWRStatus(PMC_MODULE Module);

//@}

#endif
