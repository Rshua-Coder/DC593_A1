/**
    IPH module (type define)

    @file       IPL_Hal.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef IPL_HAL_H_
#define IPL_HAL_H_
#include "IPL_Hal_IFE2.h"
#include "IPL_Hal_PRE.h"
#include "IPL_Hal_IFE.h"
#include "IPL_Hal_SIE.h"
#include "IPL_Hal_DCE.h"
#include "IPL_Hal_IPE.h"
#include "IPL_Hal_IME.h"
#include "IPL_Hal_RDE.h"
#include "IPL_Hal_SIE2.h"
/** \addtogroup mILibIPH */
//@{

/**
     engine supported by IPL_Hal
*/
typedef enum _IPL_HAL_ENG
{
    IPL_HAL_UNKNOW  = 0,
    IPL_HAL_SIE     = 1,    ///< SIE
    IPL_HAL_IPE     = 2,    ///< IPE
    IPL_HAL_IME     = 3,    ///< IME
    IPL_HAL_IFE     = 4,    ///< IFE
    IPL_HAL_IFE2    = 5,    ///< IFE2
    IPL_HAL_PRE     = 6,    ///< PRE
    IPL_HAL_DCE     = 7,    ///< DCE
    IPL_HAL_RDE     = 8,    ///< RDE
    IPL_HAL_SIE2    = 9,    ///< SIE2
    IPL_HAL_MAX     = 10,
    ENUM_DUMMY4WORD(IPL_HAL_ENG)
}IPL_HAL_ENG;

/**
    engine clk
    @note : sie engine clk set to 0 when the protocal of sensor data transition is LVDS
*/
typedef union _IPH_CLK
{
    UINT32      SIEclk;     ///< SIE clock
    IPH_PRECLK  PREclk;     ///< PRE clock
    IPH_IPECLK  IPEclk;     ///< IPE clock
    IPH_DCECLK  DCEclk;     ///< DCE clock
    IPH_IFECLK  IFEclk;     ///< IFE clock
    IPH_IFE2CLK IFE2clk;    ///< IFE2 clock
    IPH_IMECLK  IMEclk;     ///< IME clock
    IPH_RDECLK  RDEclk;     ///< RDE clock
    UINT32      SIE2clk;    ///< SIE2 clock
    UINT32 EngineClk;
}IPH_CLK;

/**
     engine para. update selection
*/
typedef union _IPL_HAL_GROUP
{
    SIE_UPDATE  sieUpdate;  ///< SIE update selection
    PRE_UPDATE  preUpdate;  ///< PRE update selection
    IFE_UPDATE  ifeUpdate;  ///< IFE update selection
    IPE_UPDATE  ipeUpdate;  ///< IPE update selection
    DCE_UPDATE  dceUpdate;  ///< DCE update selection
    IFE2_UPDATE ife2Update; ///< IFE2 update selection
    IME_UPDATE  imeUpdate;  ///< IME update selection
    RDE_UPDATE  rdeUpdate;  ///< RDE update selection
    SIE2_UPDATE sie2Update; ///< SIE2 update selection
    UINT32 data;
}IPL_HAL_GROUP;

/**
     engine sub-function
*/
typedef struct _ENG_SUBFUNC
{
    SIE_SUBFUNC*    pSie_sub;   ///< -
    PRE_SUBFUNC*    pPre_sub;   ///< -
    IFE_SUBFUNC*    pIfe_sub;   ///< -
    DCE_SUBFUNC*    pDce_sub;   ///< -
    IPE_SUBFUNC*    pIpe_sub;   ///< -
    IFE2_SUBFUNC*   pIfe2_sub;  ///< -
    IME_SUBFUNC*    pIme_sub;   ///< -
    RDE_SUBFUNC*    pRde_sub;   ///< -
    SIE2_SUBFUNC*   pSie2_sub;  ///< -
}ENG_SUBFUNC;

#if 0
//mark, no use
/**
    engine struct union
*/
typedef union
{
    SIE sie;
    PRE pre;
    IFE ife;
    DCE dce;
    IPE ipe;
    IFE2 ife2;
    IME ime;
}IPH_ENG_UNION;
#endif


#define min(a,b)    ((a<b)?a:b)
#define MinParaSet  min(min(min(min(min(min(min(min(SIE_MaxParaSet,PRE_MaxParaSet),IFE_MaxParaSet),DCE_MaxParaSet),IPE_MaxParaSet),IFE2_MaxParaSet),IME_MaxParaSet),RDE_MaxParaSet),SIE2_MaxParaSet)

/**
     Index of parameter set supported by IPL_Hal
*/
typedef enum _IPL_HAL_PARASET
{
    IPL_HAL_PAR0    = 0,        ///< -
    IPL_HAL_PAR1    = 1,        ///< -
    IPL_HAL_PAR2    = 2,        ///< -
    IPL_HAL_PAR3    = 3,        ///< -
    IPL_HAL_PAR4    = 4,        ///< -
    IPL_HAL_PARMAX  = MinParaSet,
    ENUM_DUMMY4WORD(IPL_HAL_PARASET)
}IPL_HAL_PARASET;

//@}
#endif