/**
    IPH module (RDE sub function type define)

    @file       IPL_Hal_RDE_Info.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_RDE_Info_H_
#define _IPL_Hal_RDE_Info_H_
#include "Type.h"
#include "IPL_Utility.h"
#include "pll.h"
/** \addtogroup mILibIPH */
//@{
#define RDE_MaxParaSet  5

/**
    RDE engine supported clk
*/
typedef enum _IPH_RDECLK
{
    IPH_RDECLK0 = 48,    ///< 48 MHZ
    IPH_RDECLK1 = 60,    ///< 60 MHZ
    IPH_RDECLK2 = 80,    ///< 80 MHZ
    IPH_RDECLK3 = 120,   ///< 120 MHZ
    IPH_RDECLK4 = 160,   ///< 160 MHZ
    IPH_RDECLK5 = 192,   ///< 192 MHZ
    IPH_RDECLK6 = 220,  ///< PLL6
    ENUM_DUMMY4WORD(IPH_RDECLK)
}IPH_RDECLK;

/**
    RDE interrupt enable
*/
typedef enum _RDE_INTE_EN
{
    RDE__CLR        = 0,
    RDE__INTE_FMD   = 0x00000001,   ///< enable interrupt: frame end
    RDE__INTE_ERR   = 0x0000000E,   ///< enable interrupt: decode error
    RDE__INTE_ALL   = 0x0000000f,   ///< enable interrupt: enable all interrupt
    ENUM_DUMMY4WORD(RDE_INTE_EN)
}RDE_INTE_EN;

/**
    RDE sub function enable
*/
typedef enum _RDEFuncEn
{
    RDE_VFlipEn     =   0x00000001, ///< enable function: Vertical flip
    RDE_HFlipEn     =   0x00000002, ///< enable function: Horizontal flip
    DEGammaEn       =   0x00000004, ///< enable function: Decode gamma
    ENUM_DUMMY4WORD(RDEFuncEn)
}RDEFuncEn;

/**
    padding selection(dithering)
*/
typedef enum _RDE_PAD
{
    RDE_Random  = 0,    ///< random
    RDE_ZERO    = 1,    ///< padding 0
    ENUM_DUMMY4WORD(RDE_PAD)
}RDE_PAD;

/**
    digital gain on bayer dadta
*/
typedef enum _RDE_GAIN
{
    RDE_GAIN0   = 0,    ///<    shift 0-bit to right
    RDE_GAIN1   = 1,    ///<    shift 1-bit to right
    RDE_GAIN2   = 2,    ///<    shift 2-bit to right
    RDE_GAIN3   = 3,    ///<    shift 3-bit to right
    ENUM_DUMMY4WORD(RDE_GAIN)
}RDE_GAIN;

typedef struct
{
    UINT32 BS_Addr;
    UINT32 BS_Size;
}RDE_IN;

typedef struct
{
    UINT32 Bayer_Addr;
    UINT32 Bayer_H;
    UINT32 Bayer_V;
    UINT32 Bayer_Lineofs;
}RDE_OUT;

typedef struct _RDE_SUBFUNC
{
    RDE_INTE_EN InteEn;     ///< interrupt enable

    RDEFuncEn   RDEFuncEn;  ///< sub function enable

    UINT32*     GammaTbl;   ///< type:UINT32, tab num: 129
    RDE_GAIN    OutputGain; ///< digital gain on Bayer img
    RDE_PAD     Dithering;  ///< dithering selection
}RDE_SUBFUNC;

//@}
#endif