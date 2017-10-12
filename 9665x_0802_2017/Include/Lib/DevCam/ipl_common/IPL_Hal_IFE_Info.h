/**
    IPH module (IFE sub function type define)

    @file       IPL_Hal_IFE_Info.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_Hal_IFE_Info_H
#define _IPL_Hal_IFE_Info_H
#include "Type.h"
#include "IPL_Utility.h"
#include "pll.h"
/** \addtogroup mILibIPH */
//@{

#define IFE_MaxParaSet  5

/**
    engine clk
*/
typedef enum _IPH_IFECLK
{
    IPH_IFECLK0 = 48,    ///< 48 mhz
    IPH_IFECLK1 = 60,    ///< 60 mhz
    IPH_IFECLK2 = 80,    ///< 80 mhz
    IPH_IFECLK3 = 120,   ///< 120 mhz
    IPH_IFECLK4 = 160,   ///< 160 mhz
    IPH_IFECLK5 = 192,   ///< 192 mhz
    IPH_IFECLK6 = 220,  ///< -
    ENUM_DUMMY4WORD(IPH_IFECLK)
}IPH_IFECLK;

/**
    ife data format
*/
typedef enum _IFEFMT
{
    IFE_Normal  = 0,    ///< non uv pack
    IFE_RAW     = 1,    ///< byaer
    IFE_UVPack  = 2,    ///< uv packed
    ENUM_DUMMY4WORD(IFEFMT)
}IFEFMT;

/**
    IFE data bitdepth
*/
typedef IPL_RAW_BIT IFE_PackBus;
/**
    IFE bayer CFA
*/
typedef IPL_RAW_PIX IFE_CFA;

/**
    IFE interrupt enable enum
*/
typedef enum _IfeInteEn
{
    IFE__CLR        = 0,
    IFE__INTE_FMD   = 0x00000001,   ///< frame end interrupt
    //IFE__INTE_BP1   = 0X00000002,   ///< break point 1 interrupt
    IFE__INTE_ROWDEF= 0x00000004,   ///< row def concealment warning interrupt(happened at continus raw defect more than 2 rows)
    IFE__INTE_HSTPE = 0X00000008,   ///< H stripe end interrupt
    IFE__INTE_ALL   = 0x0000000f,   ///< all interrupts
    ENUM_DUMMY4WORD(IfeInteEn)
} IfeInteEn;

/**
    IFE sub function enable enum
*/
typedef enum _IfeFuncEn
{
    OutlierEn   = 0x00000001,       ///< enable outLier
    RowDefEn    = 0x00000002,       ///< enable row defect concealment
    CurvemapEn  = 0x00000004,       ///< enable courve map
    CgainEn     = 0x00000008,       ///< enable color gain
    FilterEn    = 0x00000010,       ///< enable bilateral filter
    ENUM_DUMMY4WORD(IfeFuncEn)
}IfeFuncEn;

/**
    curve map offset

    @note if data is uv pack, fulfill UVpack, otherwise, fulfill Nonpack
*/
typedef union _CRVOFS
{
    struct
    {
        UINT32  Cb;         ///< -
        UINT32  Cr;         ///< -
    }UVpack;                ///< uv-pack format
    UINT32  Nonpack;        ///< non-pack format
}CRVOFS;

/**
    range filter
*/
typedef struct _RangeCFG
{
    UINT32  *pThres;    ///< range filter threshold, tab num:4, Data type: UINT32
    UINT32  Delta;      ///< range filter threshold adjustment
}RangeCFG;

/**
    IFE outlier func
*/
typedef struct _IFE_OUTL
{
    UINT32* pBrightThres;       ///< tab num:5 Data type:UINT32
                                ///< 0: avg<32 , (bit range: 8-bit)
                                ///< 1: avg<64
                                ///< 2: avg<128
                                ///< 3: max:255
                                ///< 4: for interpolation
    UINT32* pDarkThres;         ///< same as above
    enum
    {
        _8_NB   = 0,        ///< eight neighbours fall in the criteria
        _7_NB   = 1,        ///< seven neighbours fall in the criteria
        ENUM_DUMMY4WORD(IFE_OUTLCNT)
    }IFE_OUTLCNT;           ///< outlier option for neighbour condition
}IFE_OUTL;

/**
    IFE Row Defect concealment
*/
//#define RowDefMaxNum 16
typedef struct _IFE_ROWDEF
{
    UINT32  StartPixel_Y;               ///< vertical start coordinate(crop window start pixel is origin)
    //UINT32  RowDef_Y[RowDefMaxNum];     ///< y coordinate of each row
    UINT32  RowDef_Y_TblAddr;           ///< table (tab num:16, Data type:UINT32)
}IFE_ROWDEF;

/**
    IFE Curve map
*/
typedef struct _IFE_CRVMAP
{
    enum
    {
        _1_LUT  = 0,        ///< step size:1
        _2_LUT  = 1,        ///< step size:2
        _4_LUT  = 2,        ///< step size:4
        ENUM_DUMMY4WORD(CRVMapDelt)
    }CRVMapDelt;            ///< step size per lut

    CRVOFS  InOffset;       ///< input offset value
    CRVOFS  OutOffset;      ///< output offset value
    UINT32  Gamma_TblAddr;      ///< tab number:65, Data type: UINT32
}IFE_CRVMAP;

/**
    IFE color gain
*/
typedef struct _IFE_CG
{
    UINT16  R;          ///< -
    UINT16  Gr;         ///< -
    UINT16  Gb;         ///< -
    UINT16  B;          ///< -
    IFE_CFA cfa;        ///< -
    enum
    {
        _2_8    = 0,    ///< 2bits decimal, 8bits fraction
        _3_7    = 1,    ///< 3bits decimal, 7bits fraction
        ENUM_DUMMY4WORD(IfeGainSel)
    }IfeGainSel;        ///< gain bit field
}IFE_CG;

/**
    IFE bilateral filter
*/
typedef struct _IFE_FILT
{
    enum
    {
        Normal  = 0,        ///< standard filter(biletral filter)
        SONLY   = 1,        ///< spatial filter only(g filter)
        ENUM_DUMMY4WORD(IFEFiltMode)
    }IFEFiltMode;           ///< bilateral mode
    struct
    {
        BOOL    sOnlyEn;    ///< spatial filter only
        enum
        {
            Filt_3X3    = 0,    ///<    convolution window:3x3
            Filt_5x5    = 1,    ///<    5x5
            ENUM_DUMMY4WORD(sOnlyFiltLeng)
        }sOnlyFiltLeng;     ///< filter length when spatial only enable
        UINT32  *pWeight;   ///< spatial weighting, tab num: 6, Data type: UINT32
    }Spatial;               ///< spatial filter
    struct
    {
        RangeCFG A;     ///< -
        RangeCFG B;     ///< -
        UINT32  Bilat_w; ///< range filter A and B weighting
    }Range;             ///< range filter
    struct
    {
        UINT32  Th;     ///< threshold
        UINT32  Mul;    ///< weighting multiplier
        UINT32  Dlt;    ///< threshold adjustment
    }Clamp;             ///< clamp configuration
    UINT32  Rth_W;      ///< range filter and spatial filter weighting
    UINT32  Bin;            ///< range:0~7.     ( Denominator: 2<<bin  )
}IFE_FILT;

/**
     IFE sub function configuration.
*/
typedef struct _IFE_SUBFUNC
{
    IfeInteEn   ifeInteEn;      ///< ife interrupt enable
    //UINT32      BP1;                    ///< break point x coordinate
    IfeFuncEn   ifeFuncEn;      ///< ife function enable

    IFE_ROWDEF  RowDef;         ///< row defect configuration
    IFE_CG      CGain;          ///< color gain configuration

    IFE_OUTL    *pOutl;         ///< outlier configuration
    IFE_CRVMAP  *pCrvMap;       ///< courve map configuration
    IFE_FILT    *pFilter;       ///< bilateral configuration
}IFE_SUBFUNC;
//@}
#endif
