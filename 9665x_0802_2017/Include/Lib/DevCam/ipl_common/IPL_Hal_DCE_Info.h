/**
    IPH module (DCE sub function type define)

    @file       IPL_Hal_DCE_Info.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_Hal_DCE_Info_H
#define _IPL_Hal_DCE_Info_H
#include "Type.h"
#include "IPL_Utility.h"
#include "pll.h"
/** \addtogroup mILibIPH */
//@{
#define DCE_MaxParaSet  5
/**
    engine clk
*/
typedef enum _IPH_DCECLK
{
    IPH_DCECLK0 = 48,    ///< 48 mhz
    IPH_DCECLK1 = 60,    ///< 60 mhz
    IPH_DCECLK2 = 80,    ///< 80 mhz
    IPH_DCECLK3 = 120,   ///< 120 mhz
    IPH_DCECLK4 = 160,   ///< 160 mhz
    IPH_DCECLK5 = 192,   ///< 192 mhz
    IPH_DCECLK6 = 220,  ///< -
    ENUM_DUMMY4WORD(IPH_DCECLK)
}IPH_DCECLK;

/**
    Radius factor
*/
typedef struct _Fact_Norm
{
    BOOL    Manual;     ///< if Manual is FALSE, Factor and Bit will be calculated automatically.
    UINT32  Factor;     ///< normalize factor
    UINT32  Bit;        ///< normalize bit
}Fact_Norm;

/**
    DCE in/out format
*/
typedef enum _DCEFMT
{
    DCE_Y_8         = 0,    ///< Single Y plane, 8-bit mode
    DCE_UV_422_16B  = 1,    ///< UV 422 packed plane, 16-bit mode
    DCE_UV_420_16B  = 2,    ///< UV 420 packed plane, 16-bit mode
    DCE_YUV_422_16B = 3,    ///< YUV 422 packed plane
    DCE_YUV_420_16B = 4,    ///< YUV 420 packed plane
    ENUM_DUMMY4WORD(DCEFMT)
}DCEFMT;

/**
    sie interrupt enable
*/
typedef enum _DceInteEn
{
    DCE__CLR        = 0,
    DCE__INTE_FMS   = 0x00000001,   ///< frame start interrupt
    DCE__INTE_FMD   = 0X00000002,   ///< frame end interrupt
    DCE__INTE_STRPE = 0x00000004,   ///< current stripe end interrupt
    //DCE__INTE_ALL   = 0x0000000f,   ///< all interrupts
    ENUM_DUMMY4WORD(DceInteEn)
} DceInteEn;

/**
    Field of view
*/
typedef struct _FOV
{
    UINT32  GainR;      ///< correction gain for R channel
    struct
    {
        BOOL ManualEn;   ///< enable user mode for boundry process
        UINT32  R;      ///< boundary pixel of R channel,(enable when user mode enabled)
        UINT32  G;      ///< boundary pixel of G channel
        UINT32  B;      ///< boundary pixel of B channel
    }BoundryProcess;
}FOV;               ///< Field of view setting

/**
    Geometric enhancement
*/
typedef struct _EnH
{
    UINT32  Factor_Y; ///< factor
    UINT32  Norm_Y;   ///< normalize bit
    UINT32  Factor_UV; ///< factor
    UINT32  Norm_UV;   ///< normalize bit
}EnH;

/**
    Color aberration correction factor
*/
typedef struct _Aberation
{
    UINT32  Fact_R; ///< R channel
    UINT32  Fact_G; ///< -
    UINT32  Fact_B; ///< -
}Aberation;

/**
    horizontal/vertical distance factors and distortion selection
*/
typedef struct _DIST
{
    UINT32  Fact_X;         ///< X input distance factor for oval shape modeling
    UINT32  Fact_Y;         ///< Y input distance factor for oval shape modeling
    enum
    {
        Bar_Pin     = 0,    ///< Used for barrel and pincushion distortion
        Perspective = 1,    ///< Used for perspective distortion
        ENUM_DUMMY4WORD(DCE_DisSel)
    }DCE_DisSel;            ///< distortaion selection
}DIST;

/**
    Lut table curve type
*/
typedef enum _DCE_LutType
{
    NON_STRICTLY_INC_DEC    = 0,    ///< -
    STRICTLY_INC_DEC        = 1,    ///< strictly increment or strictly decrement
    ENUM_DUMMY4WORD(DCE_LutType)
}DCE_LutType;

#define LutTblSize  65
#define DCE_LutTableType(pLutTbl,pLutType)   \
{\
    UINT32 i=(LutTblSize-1);\
    *pLutType = STRICTLY_INC_DEC;\
    while(i--)\
    {\
        if( ( (pLutTbl[i] > pLutTbl[i-1]) && (pLutTbl[i+1] < pLutTbl[i]) ) ||       \
            ( (pLutTbl[i] < pLutTbl[i-1]) && (pLutTbl[i+1] > pLutTbl[i]) ) )        \
        {\
            *pLutType = NON_STRICTLY_INC_DEC;\
            break;\
        }\
        if(i==1)\
            break;\
    }\
}

/**
    distortion compensation
*/
typedef struct _DCE_Distortion
{
    Coordinate  Center;         ///< Distortion center
    FOV         Fov;            ///< Field of view setting
    EnH         Enh;            ///< Geometric enhancement term
    Aberation   ColorAber;      ///< Color aberration correction factor setting
    UINT32      LUT_TblAddr;    ///< table addr(tab num:65, Data type:UINT32)
    DCE_LutType LutType;        ///< lut table curve type, plz usr macro:LutTableType
    BOOL        H_FilterEn;     ///< ///< Horizontal filter for UV channel
}DCE_Distortion;

/**
     DCE sub function configuration.
*/
typedef struct _DCE_SUBFUNC
{
    DceInteEn   dceInteEn;  ///< interrupt enable

    DIST        Dce_Dist;   ///< Distance configuration
    Fact_Norm   Radious;    ///< GEO normaliztion factor and shift bit setting
    DCE_Distortion  *pDistortion;   ///< distortion configuration
}DCE_SUBFUNC;
//@}
#endif