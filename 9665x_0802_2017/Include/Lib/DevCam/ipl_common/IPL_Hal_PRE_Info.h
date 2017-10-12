/**
    IPH module (PRE sub function type define)

    @file       IPL_Hal_PRE_Info.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_Hal_PRE_Info_H
#define _IPL_Hal_PRE_Info_H
#include "Type.h"
#include "IPL_Utility.h"
#include "pll.h"
/** \addtogroup mILibIPH */
//@{

#define PRE_MaxParaSet  5
/**
    pre engine supported clk
*/
typedef enum _IPH_PRECLK
{
    IPH_PRECLK0 = 48,    ///< 48 mhz
    IPH_PRECLK1 = 60,    ///< 60 mhz
    IPH_PRECLK2 = 80,    ///< 80 mhz
    IPH_PRECLK3 = 120,   ///< 120 mhz
    IPH_PRECLK4 = 160,   ///< 160 mhz
    IPH_PRECLK5 = 192,   ///< 192 mhz
    IPH_PRECLK6 = 220,  ///< -
    ENUM_DUMMY4WORD(IPH_PRECLK)
}IPH_PRECLK;

/**
    cross talk compensation gain
*/
typedef struct _CTCGain
{
    UINT32  Hor;    ///< Horizontal
    UINT32  Ver;    ///< Vertical
    UINT32  Dia;    ///< Diagonal
}CTCGain;

/**
    PRE interrupt enable enum.
*/
typedef enum _PRE_INTE_EN
{
    PRE__CLR        = 0,
    PRE__INTE_FMS   = 0x00000001,   ///< enable interrupt: frame start
    PRE__INTE_FMD   = 0x00000002,   ///< enable interrupt: frame end
    PRE__INTE_HSTPE = 0x00000004,   ///< enable interrupt: Horizontal stripe end
    PRE__INTE_DPCWAR= 0x00000008,   ///< enable interrupt: Embedded Defect concealment warning,warning happened at continus bp.(more than 2 bp)
    PRE__INTE_ALL   = 0xffffffff,   ///< enable all interrupt
    ENUM_DUMMY4WORD(PRE_INTE_EN)
}PRE_INTE_EN;

/**
    PRE sub function enable
*/
typedef enum _PreFuncEn
{
    DfsEn   = 0x00000002,   ///< enable function: dark frame subtraction
    DivEn   = 0x00000004,   ///< enable function: division arrangement
    EMDpcEn = 0x00000008,   ///< enable function: Embedded defect pixel concealment
    //CtcEn   = 0x00000010,   ///< enable function: Cross-Talk compensation
    VigEn   = 0x00000020,   ///< enable function: Vignette shading compensation
    CofsEn  = 0x00000040,   ///< enable function: Color-Offset compensation
    ENUM_DUMMY4WORD(PreFuncEn)
} PreFuncEn;

/**
    PRE data bitdepth
*/
typedef IPL_RAW_BIT PRE_PackBus;

/**
    pre bayer CFA
*/
typedef IPL_RAW_PIX PRE_CFA;

/**
    PRE input information
*/
typedef IPL_RAW_IMG_INFO PRE_IN_DMA;
/*
typedef struct
{
    IPL_RAW_PIX cfa;
    UINT32  Hsize;
    UINT32  Vsize;
    UINT32  Lineoffset;
    PRE_PackBus pack;

    UINT32  PPB0_Addr;
}PRE_IN_DMA;
*/

/**
    pre dma input information
*/
typedef struct _PRE_DMA_IN
{
    PRE_IN_DMA  InDma;              ///< -
    enum
    {
        PRE_BUF1   = 0,             ///< 1 buffer
        PRE_BUF2   = 1,             ///< 2 ping pong buffer
        ENUM_DUMMY4WORD(PreBufNum)
    }PreBufNum;                     ///<buf number
    UINT32  PPB1_Addr;              ///<needed when BufNum == _BUF2
}PRE_DMA_IN ;

/**
    pre dma output information
*/
typedef struct _PRE_OUT
{
        UINT32  Addr;           ///< must be needed if PRE output to dram
        UINT32  Lineoffset;     ///< -
        PRE_PackBus pack;       ///< -
}PRE_OUT;

/**
    pre dark frame subtraction
*/
typedef struct _PRE_DFS
{
    enum
    {
        B_MINUS_A       = 0,///< A-B
        A_MINUS_B       = 1,///< B-A
        A_PLUS_B        = 2,///< A+B
        A_MUL_B_SHIFT   = 3,///< A*B>>shift, shift vaule is at below.
        ENUM_DUMMY4WORD(DFS_OP)
    }DFS_OP;                ///< dfs operation
    enum
    {
        Thr_SrcA    = 0,    ///< A as the source for threshold
        Thr_SrcB    = 1,    ///< B as the source for threshold
        ENUM_DUMMY4WORD(DFS_ThrSrc)
    }DFS_ThrSrc;            ///< source selection

    UINT32  Threshold;  ///<threshold for operation
    BOOL    bDpSym;     ///<DP symbol reservation
    UINT32  Shift;      ///<shift value for operation

    //UINT32  In_DmaCh1_PPB0_Addr;        ///<DmaCh1 Input information
    //UINT32  In_DmaCh1_PPB0_Lineoffset;  ///<DmaCh1 constraint: size must be the same with DmaCh0
}PRE_DFS;

#define DivCycle    2

/**
    pre division arrangement

    pre supprot max:5 divisions within 10 cycle-period.
*/
typedef struct _PRE_DIV
{
    enum
    {
        PRE_PROGRESSIVE_DIV = 0,            ///< progressive data type
        PRE_2_DIV       = 1,                ///< 2 division
        PRE_3_DIV       = 2,                ///< 3 division
        PRE_4_DIV       = 3,                ///< 4 division
        PRE_5_DIV       = 4,                ///< 5 division
        PRE_MAX_DIV     = 5,
        ENUM_DUMMY4WORD(PreDivNum)
    }PreDivNum;                             ///<division number
    UINT32 Id[DivCycle][PRE_MAX_DIV];       ///<division index array
}PRE_DIV;

/**
    pre embedded defect concealment(work with sie embedded defect concealment)
*/
typedef struct _PRE_EMDPC
{
    UINT32  Mark;       ///<Embedded DP Mark value, must same with sie
    enum
    {
        __50_Percent  = 0,    ///< cross-channel weighting = 50% for defect concealment
        __25_Percent  = 1,    ///< cross-channel weighting = 25% for defect concealment
        __0_Percent   = 2,    ///< cross-channel weighting =  0% for defect concealment
        ENUM_DUMMY4WORD(PRE_EMDPC_WEIGHT)
    }PRE_EMDPC_WEIGHT;     ///<concealment weighting for cross-channel
}PRE_EMDPC;

/**
    pre cross-talk compensation
*/
typedef struct _PRE_CTC
{
    CTCGain Rgain;  ///<R channel Horizontal/Vertical/Diagonal gain
    CTCGain Grgain; ///<Gr channel Horizontal/Vertical/Diagonal gain
    CTCGain Gbgain; ///<Gb channel Horizontal/Vertical/Diagonal gain
    CTCGain Bgain;  ///<B channel Horizontal/Vertical/Diagonal gain
}PRE_CTC;

/**
    pre vignette shading compensation
*/
typedef struct _PRE_VIG
{
    struct
    {
        BOOL    FuncEn;     ///< enable bit
        UINT32  Th1;        ///< threshold 1
        UINT32  Th2;        ///< threshold 2
    }DcExt;                 ///< dc extraction

    struct
    {
        Coordinate  R;  ///< R channel
        Coordinate  Gr; ///< Gr channel
        Coordinate  Gb; ///< Gb channel
        Coordinate  B;  ///< B channel
    }Center;            ///< center coordinate

    UINT32  Mul;            ///<distance gain. unsigned 2.6
    UINT32  T;              ///<disabled area
    UINT32  TabGain;        ///<gain factor
    UINT32  XDIV;           ///<scaling factor in x direction. unsigned 0.6
    UINT32  YDIV;           ///<scaling factor in y direction. unsigned 0.6
    UINT32  R_LUT_TblAddr;  ///<LUT of R channel(tab num:17, Data type:UINT8)
    UINT32  G_LUT_TblAddr;  ///<LUT of G channel(tab num:17, Data type:UINT8)
    UINT32  B_LUT_TblAddr;  ///<LUT of B channel(tab num:17, Data type:UINT8)

    BOOL    bDither;        ///<dithering enable/disable
    BOOL    DitherRstEn;    ///<dithering reset enable
}PRE_VIG;

/**
    pre color offset
*/
typedef struct _PRE_COFS
{
    UINT32  R;  ///<R channel offset
    UINT32  Gr; ///<Gr channel offset
    UINT32  Gb; ///<Gb channel offset
    UINT32  B;  ///<B channel offset
}PRE_COFS;

/**
     pre sub function configuration.
*/
typedef struct _PRE_SUBFUNC
{
    PRE_INTE_EN InteEn;         ///< interrupt enable
    PreFuncEn   preFuncEn;      ///< function enable
    struct
    {
        PRE_DFS dfs;            ///< -
        UINT32  InAddr;        ///<DmaCh1 Input information
        UINT32  InLineoffset;  ///<DmaCh1 constraint: size must be the same with DmaCh0
    }Dfs;                       ///< darm frame configuration
    PRE_DIV Div;                ///< sensor division configuration
    PRE_EMDPC EmDpc;            ///< embedded defect concealment configuration

    PRE_CTC     *pCtc;          ///< cross talk configuration
    PRE_VIG     *pVig;          ///< vignette configuration
    PRE_COFS    *pCofs;         ///< color offset configuration
}PRE_SUBFUNC;

//@}
#endif
