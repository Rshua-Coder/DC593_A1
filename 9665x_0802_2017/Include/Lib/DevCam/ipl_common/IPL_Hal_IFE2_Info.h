/**
    IPH module (IFE2 sub function type define)

    @file       IPL_Hal_IFE2_Info.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_Hal_IFE2_Info_H
#define _IPL_Hal_IFE2_Info_H
#include "Type.h"
#include "IPL_Utility.h"
#include "pll.h"
/** \addtogroup mILibIPH */
//@{

#define IFE2_MaxParaSet 5

/**
    ife2 engine supported clk
*/
typedef enum _IPH_IFE2CLK
{
    IPH_IFE2CLK0 = 48,  ///< 48 mhz
    IPH_IFE2CLK1 = 60,  ///< 60 mhz
    IPH_IFE2CLK2 = 80,  ///< 80 mhz
    IPH_IFE2CLK3 = 120, ///< 120 mhz
    IPH_IFE2CLK4 = 160, ///< 160 mhz
    IPH_IFE2CLK5 = 192, ///< 192 mhz
    IPH_IFE2CLK6 = 220,///< -
    ENUM_DUMMY4WORD(IPH_IFE2CLK)
}IPH_IFE2CLK;

/**
    IFE2 interrupt enable enum.
*/
typedef enum _IFE2_INTE_EN
{
    IFE2__CLR        =   0,
    IFE2__INTE_FMD   =   0x00000001,    ///< enable ife2 frame end interrupt
    IFE2__INTE_ALL   =   0x0000000F,    ///< enable ife2 all interrup
    ENUM_DUMMY4WORD(IFE2_INTE_EN)
}IFE2_INTE_EN;

/**
     IFE2 operation enum.

     Mode_cnt and Mode_Wight is for debug only.
*/
typedef enum _IFE2_DBMODE
{
        Mode_Normal   = 0,           ///< Normal Operation Mode
        Mode_Cnt      = 1,           ///< Debug Mode, output count
        Mode_Wight    = 2,           ///< Debug Mode, output weight
        ENUM_DUMMY4WORD(IFE2_DBMODE)
}IFE2_DBMODE;

/**
    convolution filter size
*/
typedef enum _IFE2_FiltSize
{
    IFE2_FLTR_7X7 = 0,              ///<    window size:7x7
    IFE2_FLTR_5X5 = 1,              ///<    window size:5x5
    IFE2_FLTR_3X3 = 2,              ///<    window size:3x3
    ENUM_DUMMY4WORD(IFE2_FiltSize)
}IFE2_FiltSize;

/**
    ife2 IN/OUT size
*/
typedef IPL_YCC_CH_INFO IFE2_Size;

/**
     IFE2 input data format
*/
typedef enum _IFE2_FMT
{
    IFE2_UVpack = 0,        ///< U/V packed
    IFE2_Non_pack = 1,      ///< U/V are separated planer
    ENUM_DUMMY4WORD(IFE2_FMT)
}IFE2_FMT;

/**
    ife2 sub function enable
*/
typedef enum _IFE2FuncEn
{
    ChromaEn    = 0x00000001,   ///< enable chroma adjust
    AvgFiltEn   = 0x00000002,   ///< enable average filter
    ENUM_DUMMY4WORD(IFE2FuncEn)
}IFE2FuncEn;

/**
    IFE2 chroma adjust

    only support input data format is U/V packed
    @note:  only work when sub-function 'ChromaEn' is set.
*/
typedef struct _IFE2_Chroma
{
    struct
    {
        UINT32 Cb;          ///< -
        UINT32 Cr;          ///< -
    }ref;                   ///< reference
    UINT32 varTh[4];         ///< valid:0~128
    UINT32 varWt[5];         ///< valud:0~128
}IFE2_Chroma;

/**
    2d average filter
*/
typedef struct _AvgFilt
{
    UINT32 cenWt;           ///< valid:0~32
    UINT32 avgTh[2];        ///< average threshold
    UINT32 cntTh;           ///< counting threhold
    IFE2_FiltSize fltSize;  ///< filter window size
}AvgFilt;

/**
     IFE2 average filter.

     if ife2 input data format is U/V packed, plz fulfill 'UVpack',
     otherwise, plz fulfill 'Nonpack_Filter'!

     @note: only work when sub-function 'AvgFiltEn' is set.
*/
typedef union _IFE2_AvgFilter
{
    struct
    {
        AvgFilt    Pack_UFilter;  ///< UVpack U data
        AvgFilt    Pack_VFilter;  ///< UVpack V data
    }UVpack;                    ///< UV-pack foramt
    AvgFilt    Nonpack_Filter;    ///< non-UVpack format
}IFE2_AvgFilter;

/**
     ife2 sub function configuration.
*/
typedef struct _IFE2_SUBFUNC
{
    IFE2_INTE_EN    InteEn;         ///< ife2 interrupt enable

    IFE2FuncEn      ife2FuncEn;     ///< ife2 function enable
    IFE2_Chroma     *pChromaAdj;    ///< UVpack only para set
    IFE2_AvgFilter  *pAvgFilter;    ///< avgfilter para set
}IFE2_SUBFUNC;

//@}
#endif