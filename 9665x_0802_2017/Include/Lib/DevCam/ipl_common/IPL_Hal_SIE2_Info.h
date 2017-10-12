/**
    IPH module (SIE2 sub function type define)

    @file       IPL_Hal_SIE2_Info.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_SIE2_Info_H
#define _IPL_Hal_SIE2_Info_H
#include "Type.h"
#include "IPL_Utility.h"
#include "IPL_Hal_SIE_Info.h"
/** \addtogroup mILibIPH */
//@{
#define SIE2_MaxParaSet  5

/**
    sie2 interrupt enable
*/
typedef enum _SIE2_INTE_EN
{
    SIE2__CLR                 = 0,
    SIE2__INTE_EFLDEND        = 0x00000001,   ///< enable interrupt: Even field end
    SIE2__INTE_OFLDEND        = 0x00000002,   ///< enable interrupt: Odd field end
    SIE2__INTE_FLDEND         = 0x00000004,   ///< enable interrupt: Field end
    SIE2__INTE_BP1            = 0x00000010,   ///< enable interrupt: Break point 1
    SIE2__INTE_BP2            = 0x00000020,   ///< enable interrupt: Break point 2
    SIE2__INTE_BP3            = 0x00000040,   ///< enable interrupt: Break point 3
    SIE2__INTE_ACTST          = 0x00000080,   ///< enable interrupt: Active window start
    SIE2__INTE_CRPST          = 0x00000100,   ///< enable interrupt: Crop window start
    SIE2__INTE_DRAM_OUT0_END  = 0x00100000,   ///< enable interrupt: Dram output channel 0 end
    SIE2__INTE_DRAM_OUT1_END  = 0x00200000,   ///< enable interrupt: Dram output channel 1 end
    SIE2__INTE_DPCWARN        = 0x01000000,   ///< enable interrupt: Defect Pixel/Column concealment warning,happened at continus bp.(more than 2 bp)
    SIE2__INTE_ALL            = 0xffffffff,   ///< enable interrupt: all
}SIE2_INTE_EN;

/**
     SIE2 sub fucntion enable
*/
typedef enum _Sie2FuncEn
{
    DviEn_       = 0x00000040,   ///< enable DVI(CCIR)
    DpcEn_       = 0x00000400,   ///< enable Defect pixel concealment
    ColumnDefEn_ = 0x00001000,   ///< enable Column defect concealment
    OBAvgEn_     = 0x00004000,   ///< enable OB average ( ob detection)
    OBSubSelEn_  = 0x00008000,   ///< enable OB subtraction selection
    ECSEn_       = 0x00100000,   ///< enable Color shading compensation
    DGainEn_     = 0x00200000,   ///< enable Digital Gain
    VIGEn_       = 0x01000000,   ///< enable vignette shading compensation(for CA)
    CACCEn_      = 0x02000000,   ///< enable Color accumulation
    HFlipEn_     = 0x10000000,   ///< enable Horizontal flip
    VFlipEn_     = 0x20000000,   ///< enable Vertical flip
    DMAOutCh0En_ = 0x40000000,   ///< enable Dram output channel 0
}Sie2FuncEn;

/**
    DVI format
*/
typedef struct _SIE2__DVI
{
    enum
    {
        _CCIR601 = 0,   ///< -
        _CCIR656 = 1,   ///< -
        ENUM_DUMMY4WORD(SIEDVIFmt)
    }SIEDVIFmt;         ///< -

    enum
    {
        _SDMode = 0,    ///< -
        _HDMode = 1,    ///< -
        ENUM_DUMMY4WORD(DVIMode)
    }DVIMode;           ///< -

    enum
    {
        _EAV        = 0,    ///< EAV code recognition
        _ACTWindow  = 1,    ///< Active window size setting
        ENUM_DUMMY4WORD(CCIR656ActSel)
    }CCIR656ActSel;         ///< -

    BOOL    FldEn;          ///< start input from field with internal field signal=0
    BOOL    FldInvEn;       ///< inverse the polarity of internal field signal
    BOOL    CCIR656InSwapEn;    ///< input Y/C channel pin swap for CCIR656
    BOOL    CCIR656OutSwapEn;   ///< output Y/C channel pin swap for CCIR656
}SIE2__DVI;

/**
    SIE2 active and crop window
*/
typedef struct _SIE2_WINDOW
{
    struct
    {
        UINT16  DIV_X;      ///< x-direction active start coordinate
        UINT16  FLD_Y;      ///< y-direction active start coordinate
        UINT32  SizeH;      ///< active window size
        UINT32  SizeV;      ///< active window size
        SIE_CFA CFA;        ///< CFA pattern of active window 1st pixel
    }Act;                           ///< active window
    struct
    {
        Coordinate  StartPix;///<crop start coordinate
        UINT32  SizeH;      ///< crop window size
        UINT32  SizeV;      ///< crop window size
        SIE_CFA CFA;    ///< CFA pattern of crop window 1st pixel
    }Crop;                          ///< crop window
}SIE2_WINDOW;

/**
    SIE input data format and signal
*/
typedef struct _IPP_SIE2_SIGNAL
{
    struct
    {
        SIG_PHASE   VD;     ///< -
        SIG_PHASE   HD;     ///< -
        SIG_PHASE   Data;   ///< -
    }SensorPhase;           ///< signal phase

    struct
    {
        UINT32  HD_sync;    ///< HD sync period
        UINT32  HD_total;   ///< HD total period
        UINT32  HD_offset;  ///< HD offset
        BOOL    HD_ini;     ///< HD inverse
        UINT32  VD_total;   ///< VD total period
        UINT32  VD_sync;    ///< VD sync period
        UINT32  VD_offset;  ///< VD offset
        BOOL    VD_ini;     ///< VD inverse
    }Sie2_HDVD;              ///< HD/VD signal
}IPP_SIE2_SIGNAL;

/**
    SIE Dram output burst length
*/
typedef enum _SIE2_BURST_LENG
{
    _BURST_32   = 32,   ///< SIE Dram burst length  32 words
    _BURST_16   = 16,   ///< SIE Dram burst length  16 words
    _BURST_24   = 24,   ///< SIE Dram burst length  24 words
    ENUM_DUMMY4WORD(SIE2_BURST_LENG)
}SIE2_BURST_LENG;

/**
     SIE2 sub function configuration.
*/
typedef struct _SIE2_SUBFUNC
{
    SIE2_INTE_EN    InteEn; ///< interrupt enable
    Sie2FuncEn      sie2FuncEn;  ///< function enable
    SIE_BP          BP;         ///< break point configuration

    SIE_CACC        Ca;         ///< color acc. configuration
    SIE2__DVI       CCIR;       ///< CCIR format input
    SIE_DPC         Dpc;        ///< defect pixel configuration
    SIE_ColumnDef   ColumnDef;  ///< column defect concealment configuration
    SIE_OBDET       OBAvg;          ///< OB average dectection configuration
    SIE_OBSUB       OBSub;          ///< OB substration configuration
    SIE__DGAIN      DGain;      ///< digital gain in 3.7 bits format
    struct
    {
        UINT32  Inaddr;         ///< ecs table input addr(ch1)
        ECSMAP_ENUM     EcsMap; ///< ecs table size
        SIE_EmbeddedCS *pecs;   ///<
    }Ecs;

    SIE_CA_VIG  *pVig;      ///< CACC vignette configuration
    SIE_SCAL_PAR *pScale;   ///< bayer scaler configuration
}SIE2_SUBFUNC;
#endif
