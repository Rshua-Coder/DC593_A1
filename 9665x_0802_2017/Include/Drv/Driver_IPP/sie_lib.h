/**
    Public header file for SIE module.

    @file       sie_lib.h
    @ingroup    mIIPPSIE

    @brief

    How to start:\n
    [Preview]\n
    1. sie_attach\n
    2. sie_open\n
    3. SIE_Operation = SIE_ENGINE_SET2PRV, sie_setMode\n
    4. SIE_Operation = SIE_ENGINE_SET2RUN, sie_setMode\n

    [Preview to Capture]\n
    1. SIE_operation = SIE_ENGINE_SET2PAUSE, sie_setMode\n
    2. SIE_Operation = SIE_ENGINE_SET2CAP, sie_setMode\n
    3. SIE_Operation = SIE_ENGINE_SET2RUN, sie_setMode\n
    4. SIE_operation = SIE_ENGINE_SET2PAUSE, sie_setMode\n


    How to stop:\n
    1. SIE_operation = SIE_ENGINE_SET2PAUSE, sie_setMode\n
    2. sie_close\n
    3. sie_detach\n


    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _SIE_LIB_H
#define _SIE_LIB_H

#define _SIE_LIB_H_FOR_FW_ (1)
#define _SIE_LIB_H_FOR_GEARGEN_ (0)

#if _SIE_LIB_H_FOR_FW_
// include files for FW
#include "Type.h"
#elif _SIE_LIB_H_FOR_GEARGEN_
// include files for GearGen
#include    "..\\type.h"
#endif//_SIE_LIB_H_FOR_GEARGEN_


/**
SIE Definition

@name   SIE_Define
*/
//@{

/*
    Define SIE Function Enable
*/
//@{
//#define SIE_ACT_EN                0x00000004   ///< enable function: Active window(Flow after active window)
#define SIE_RAWENC_EN             0x00000008   ///< enable function: Raw encoder
#define SIE_SPITRIG_EN            0x00000020   ///< enable function: SPI trigger
//Not for SIE//#define SIE_DVI_EN                0x00000040   ///< enable function: DVI
#define SIE_PATGEN_EN             0x00000080   ///< enable function: Pattern Generator
#define SIE_REORDER_H_EN          0x00000100   ///< enable function: Horizontal re-order
#define SIE_REORDER_V_EN          0x00000200   ///< enable function: Vertical re-order
#define SIE_DPC_EN                0x00000400   ///< enable function: Defect pixel concealment
#define SIE_EMBDPC_EN             0x00000800   ///< enable function: Embeded defect pixel mark
#define SIE_COLDEF_EN             0x00001000   ///< enable function: Column defect concealment
#define SIE_SMR_DETRED_EN         0x00002000   ///< enable function: Smear detection and reduction
#define SIE_OB_AVG_EN             0x00004000   ///< enable function: OB average
#define SIE_OB_SUB_SEL            0x00008000   ///< enable function: OB subtraction selection
#define SIE_OB_FRAME_AVG_EN       0x00010000   ///< enable function: OB-Frame average
#define SIE_OB_PLAME_SUB_EN       0x00020000   ///< enable function: OB-Plane subtraction
#define SIE_PFPC_EN               0x00040000   ///< enable function: PFPC
#define SIE_GAMMA_EN              0x00080000   ///< enable function: Gamma
#define SIE_ECS_EN                0x00100000   ///< enable function: Color shading compensation
#define SIE_DGAIN_EN              0x00200000   ///< enable function: Digital Gain
#define SIE_BS_H_EN               0x00400000   ///< enable function: Horizontal bayer scaling
#define SIE_BS_V_EN               0x00800000   ///< enable function: Vertical bayer scaling
#define SIE_VIG_EN                0x01000000   ///< enable function: vignette shading compensation
#define SIE_CA_EN                 0x02000000   ///< enable function: Color accumulation
#define SIE_VACC_EN               0x04000000   ///< enable function: Variation accumulation
#define SIE_FLD_ARR_EN            0x08000000   ///< enable function: Field arrangement
#define SIE_HFLIP_EN              0x10000000   ///< enable function: Horizontal flip
#define SIE_VFLIP_EN              0x20000000   ///< enable function: Vertical flip
#define SIE_DRAM_OUT0_EN          0x40000000   ///< enable function: Dram output channel 0
#define SIE_DRAM_OUT2_EN          0x80000000   ///< enable function: Dram output channel 2
//@}


/*
    Define SIE interrput enable.
*/
//@{
//removed and would be masked-out//#define SIE_INTE_EFLDEND              0x00000001   ///< enable interrupt: Even field end
//removed and would be masked-out//#define SIE_INTE_OFLDEND              0x00000002   ///< enable interrupt: Odd field end
#define SIE_INTE_FLDEND               0x00000004   ///< enable interrupt: Field end
//removed and would be masked-out//#define SIE_INTE_LN16END              0x00000008   ///< enable interrupt: 16 lines end
#define SIE_INTE_BP1                  0x00000010   ///< enable interrupt: Break point 1
#define SIE_INTE_BP2                  0x00000020   ///< enable interrupt: Break point 2
#define SIE_INTE_BP3                  0x00000040   ///< enable interrupt: Break point 3
#define SIE_INTE_ACTST                0x00000080   ///< enable interrupt: Active window start
//removed and would be masked-out//#define SIE_INTE_CRPST                0x00000100   ///< enable interrupt: Crop window start
//removed and would be masked-out//#define SIE_INTE_VDOFS_OVFL           0x00000200   ///< enable interrupt: VD offset overflow
//removed and would be masked-out//#define SIE_INTE_FLSH_TRG             0x00000400   ///< enable interrupt: Flash trigger
//removed and would be masked-out//#define SIE_INTE_MSH_TRG              0x00000800   ///< enable interrupt: Mshutter trigger
//removed and would be masked-out//#define SIE_INTE_FLSH_END             0x00001000   ///< enable interrupt: Flash end
//removed and would be masked-out//#define SIE_INTE_MSH_CLOSE_END        0x00002000   ///< enable interrupt: Mshutter close end
//removed and would be masked-out//#define SIE_INTE_MSH_OPEN_END         0x00004000   ///< enable interrupt: Mshutter open end
//removed and would be masked-out//#define SIE_INTE_DRAM_IN0_UDFL        0x00008000   ///< enable interrupt: Dram input channel 0 underflow
//removed and would be masked-out//#define SIE_INTE_DRAM_IN1_UDFL        0x00010000   ///< enable interrupt: Dram input channel 1 underflow
//removed and would be masked-out//#define SIE_INTE_DRAM_OUT0_OVFL       0x00020000   ///< enable interrupt: Dram output channel 0 overflow
//removed and would be masked-out//#define SIE_INTE_DRAM_OUT1_OVFL       0x00040000   ///< enable interrupt: Dram output channel 1 overflow
//removed and would be masked-out//#define SIE_INTE_DRAM_OUT2_OVFL       0x00080000   ///< enable interrupt: Dram output channel 2 overflow
#define SIE_INTE_DRAM_OUT0_END        0x00100000   ///< enable interrupt: Dram output channel 0 end
#define SIE_INTE_DRAM_OUT1_END        0x00200000   ///< enable interrupt: Dram output channel 1 end
#define SIE_INTE_DRAM_OUT2_END        0x00400000   ///< enable interrupt: Dram output channel 2 end
//removed and would be masked-out//#define SIE_INTE_DPCF                 0x00800000   ///< enable interrupt: Defect pixel concealment failure
//removed and would be masked-out//#define SIE_INTE_DPCWARN              0x01000000   ///< enable interrupt: Defect Pixel/Column concealment failure
//removed and would be masked-out//#define SIE_INTE_VAEND                0x02000000   ///< enable interrupt: VA output end
//removed and would be masked-out//#define SIE_INTE_MSH_ERR              0x04000000   ///< enable interrupt: Mshutter open/close control conflict
//removed and would be masked-out//#define SIE_INTE_RAWENC_VBFOVFL       0x08000000   ///< enable interrupt: Raw encoder virtual buffer overflow
//removed and would be masked-out//#define SIE_INTE_RAWENC_OUTOVFL       0x10000000   ///< enable interrupt: Raw encoder output buffer full
#define SIE_INTE_ALL                  0xffffffff   ///< enable interrupt: all
//@}

/*
    Define SIE interrput status.
*/
//@{
#define SIE_INT_EFLDEND              0x00000001   ///< interrupt status: Even field end
#define SIE_INT_OFLDEND              0x00000002   ///< interrupt status: Odd field end
#define SIE_INT_FLDEND               0x00000004   ///< interrupt status: Field end
#define SIE_INT_LN16END              0x00000008   ///< interrupt status: 16 lines end
#define SIE_INT_BP1                  0x00000010   ///< interrupt status: Break point 1
#define SIE_INT_BP2                  0x00000020   ///< interrupt status: Break point 2
#define SIE_INT_BP3                  0x00000040   ///< interrupt status: Break point 3
#define SIE_INT_ACTST                0x00000080   ///< interrupt status: Active window start
#define SIE_INT_CRPST                0x00000100   ///< interrupt status: Crop window start
#define SIE_INT_VDOFS_OVFL           0x00000200   ///< interrupt status: VD offset overflow
#define SIE_INT_FLSH_TRG             0x00000400   ///< interrupt status: Flash trigger
#define SIE_INT_MSH_TRG              0x00000800   ///< interrupt status: Mshutter trigger
#define SIE_INT_FLSH_END             0x00001000   ///< interrupt status: Flash end
#define SIE_INT_MSH_CLOSE_END        0x00002000   ///< interrupt status: Mshutter close end
#define SIE_INT_MSH_OPEN_END         0x00004000   ///< interrupt status: Mshutter open end
#define SIE_INT_DRAM_IN0_UDFL        0x00008000   ///< interrupt status: Dram input channel 0 underflow
#define SIE_INT_DRAM_IN1_UDFL        0x00010000   ///< interrupt status: Dram input channel 1 underflow
#define SIE_INT_DRAM_OUT0_OVFL       0x00020000   ///< interrupt status: Dram output channel 0 overflow
#define SIE_INT_DRAM_OUT1_OVFL       0x00040000   ///< interrupt status: Dram output channel 1 overflow
#define SIE_INT_DRAM_OUT2_OVFL       0x00080000   ///< interrupt status: Dram output channel 2 overflow
#define SIE_INT_DRAM_OUT0_END        0x00100000   ///< interrupt status: Dram output channel 0 end
#define SIE_INT_DRAM_OUT1_END        0x00200000   ///< interrupt status: Dram output channel 1 end
#define SIE_INT_DRAM_OUT2_END        0x00400000   ///< interrupt status: Dram output channel 2 end
#define SIE_INT_DPCF                 0x00800000   ///< interrupt status: Defect pixel concealment failure
#define SIE_INT_DPCWARN              0x01000000   ///< interrupt status: Defect Pixel/Column concealment failure
#define SIE_INT_VAEND                0x02000000   ///< interrupt status: VA output end
#define SIE_INT_MSH_ERR              0x04000000   ///< interrupt status: Mshutter open/close control conflict
#define SIE_INT_RAWENC_VBFOVFL       0x08000000   ///< interrupt status: Raw encoder virtual buffer overflow
#define SIE_INT_RAWENC_OUTOVFL       0x10000000   ///< interrupt status: Raw encoder output buffer full
#define SIE_INT_ALL                  0xffffffff   ///< interrupt status: all
//@}


//@}

/**
SIE Enumeration

@name   SIE_Enumeration
*/
//@{

typedef enum
{
    SIE_ROUND_FIXED  = 0,   ///< fixed rounding
    SIE_ROUND_RANDOM ,   ///< random rounding
    ENUM_DUMMY4WORD(SIE_ROUND_SEL)
} SIE_ROUND_SEL;


typedef enum
{
    SIE_FLG_VD         = 0,  ///< frame sync
    SIE_FLG_BP1        ,  ///< break point 1
    SIE_FLG_BP2        ,  ///< break point 1
    SIE_FLG_FLSH       ,  ///< flash assertion end
    ENUM_DUMMY4WORD(SIE_FLAG_SEL)
} SIE_FLAG_SEL;



typedef enum
{
    SIE_PHASE_RISING  = 0,  ///< rising edge latch/trigger
    SIE_PHASE_FALLING ,  ///< falling edge latch/trigger
    ENUM_DUMMY4WORD(SIE_PHASE)
} SIE_PHASE;

typedef enum
{
    SIE_POL_HIGH_ACTIVE  = 0,  ///< high level active
    SIE_POL_LOW_ACTIVE   ,  ///< low level active
    ENUM_DUMMY4WORD(SIE_POLARITY)
} SIE_POLARITY;


typedef enum
{
    SIE_TRIGSEL_CURR_FRMEND   = 0,      ///< Trigger PRE at current frame end
    SIE_TRIGSEL_NEXT_FRMSYNC      ,       ///< Trigger PRE at next frame start
    SIE_TRIGSEL_NEXT_ACTST       ,        ///< Trigger PRE at next active-window start
    ENUM_DUMMY4WORD(SIE_PRETRIGSEL)
} SIE_PRETRIGSEL;


typedef enum
{
    SIE_FEMODE_PARALLEL_SLAVE   = 0,      ///< SIE acts as slave for parallel-IF sensor
    SIE_FEMODE_PARALLEL_MASTER  ,      ///< SIE acts as master for parallel-IF sensor
    SIE_FEMODE_SERIAL           ,      ///< SIE/HighSpeedIf acts together for Serial-IF sensor
    ENUM_DUMMY4WORD(SIE_FRONTENDMODE)
} SIE_FRONTENDMODE;


typedef enum
{
    SIE_HDIV_PROGRESSIVE   = 0,      ///< Horizontal Division number = 1
    SIE_HDIV_2_DIV         ,      ///< Horizontal Division number = 2
    SIE_HDIV_3_DIV         ,      ///< Horizontal Division number = 3
    SIE_HDIV_4_DIV         ,      ///< Horizontal Division number = 4
    SIE_HDIV_5_DIV         ,      ///< Horizontal Division number = 5
    ENUM_DUMMY4WORD(SIE_HDIV_NUM)
} SIE_HDIV_NUM;

typedef enum
{
    SIE_REO_1_LINE    = 0,      ///< Re-order line number = 1
    SIE_REO_2_LINE    ,      ///< Re-order line number = 2
    SIE_REO_3_LINE    ,      ///< Re-order line number = 3
    SIE_REO_4_LINE    ,      ///< Re-order line number = 4
    ENUM_DUMMY4WORD(SIE_REO_LNNUM)
} SIE_REO_LNNUM;

typedef enum
{
    SIE_FLDARR_PROGRESSIVE     = 0,      ///< Field arrangement field number = 1
    SIE_FLDARR_2_FLD           ,      ///< Field arrangement field number = 2
    SIE_FLDARR_3_FLD           ,      ///< Field arrangement field number = 3
    SIE_FLDARR_4_FLD           ,      ///< Field arrangement field number = 4
    SIE_FLDARR_5_FLD           ,      ///< Field arrangement field number = 5
    SIE_FLDARR_6_FLD           ,      ///< Field arrangement field number = 6
    SIE_FLDARR_7_FLD           ,      ///< Field arrangement field number = 7
    SIE_FLDARR_8_FLD           ,      ///< Field arrangement field number = 8
    SIE_FLDARR_9_FLD           ,      ///< Field arrangement field number = 9
    SIE_FLDARR_10_FLD          ,      ///< Field arrangement field number = 10
    SIE_FLDARR_11_FLD          ,      ///< Field arrangement field number = 11
    SIE_FLDARR_12_FLD          ,      ///< Field arrangement field number = 12
    SIE_FLDARR_13_FLD          ,      ///< Field arrangement field number = 13
    SIE_FLDARR_14_FLD          ,      ///< Field arrangement field number = 14
    SIE_FLDARR_15_FLD          ,      ///< Field arrangement field number = 15
    SIE_FLDARR_16_FLD          ,      ///< Field arrangement field number = 16
    ENUM_DUMMY4WORD(SIE_FLDARR_FLDNUM)
} SIE_FLDARR_FLDNUM;

typedef enum
{
    SIE_PACKBUS_8  = 0, ///< 8 bits packing
    SIE_PACKBUS_10 , ///< 10 bits packing
    SIE_PACKBUS_12 , ///< 12 bits packing
    SIE_PACKBUS_16 , ///< 16 bits packing
    ENUM_DUMMY4WORD(SIE_PACKBUS)
} SIE_PACKBUS;


typedef enum
{
    SIE_BUF_NUM_1 = 0, ///< one buffer
    SIE_BUF_NUM_2 ,///< two ping-pong buffer
    ENUM_DUMMY4WORD(SIE_BUF_NUM)
} SIE_BUF_NUM;


typedef enum
{
    SIE_OUT0_SOURCE_CROP_IMG = 0, ///< SIE Dram output channel 0 source = cropped image
    SIE_OUT0_SOURCE_BS_IMG   ,   ///< SIE Dram output channel 0 source = cropped and scaled image
    ENUM_DUMMY4WORD(SIE_OUT0_SOURCE)
} SIE_OUT0_SOURCE;


typedef enum
{
    SIE_OUT2_SOURCE_VACC   = 0,   ///< SIE Dram output channel 2 source = variation accumulation
    SIE_OUT2_SOURCE_BS_IMG , ///< SIE Dram output channel 2 source = cropped and scaled image
    SIE_OUT2_SOURCE_SMRLN  ,  ///< SIE Dram output channel 2 source = smear line
    ENUM_DUMMY4WORD(SIE_OUT2_SOURCE)
} SIE_OUT2_SOURCE;

typedef enum
{
    SIE_DRAM_BURST_LENGTH_CH0_32WORD   = 0,   ///< SIE Dram burst length Ch0 32 words
    SIE_DRAM_BURST_LENGTH_CH0_64WORD , ///< SIE Dram burst length Ch0 64 words
    SIE_DRAM_BURST_LENGTH_CH0_96WORD , ///< SIE Dram burst length Ch0 96 words
    SIE_DRAM_BURST_LENGTH_CH0_128WORD , ///< SIE Dram burst length Ch0 128 words
    SIE_DRAM_BURST_LENGTH_CH0_16WORD , ///< SIE Dram burst length Ch0 16 words
    SIE_DRAM_BURST_LENGTH_CH0_24WORD , ///< SIE Dram burst length Ch0 24 words
    SIE_DRAM_BURST_LENGTH_CH0_48WORD , ///< SIE Dram burst length Ch0 48 words
    ENUM_DUMMY4WORD(SIE_DRAM_BURST_LENGTH_CH0)
} SIE_DRAM_BURST_LENGTH_CH0;

typedef enum
{
    SIE_DRAM_BURST_LENGTH_CH1_32WORD   = 0,   ///< SIE Dram burst length Ch1 32 words
    SIE_DRAM_BURST_LENGTH_CH1_24WORD , ///< SIE Dram burst length Ch1 24 words
    SIE_DRAM_BURST_LENGTH_CH1_16WORD , ///< SIE Dram burst length Ch1 16 words
    ENUM_DUMMY4WORD(SIE_DRAM_BURST_LENGTH_CH1)
} SIE_DRAM_BURST_LENGTH_CH1;

typedef enum
{
    SIE_DRAM_BURST_LENGTH_CH2_16WORD   = 0,   ///< SIE Dram burst length Ch2 16 words
    SIE_DRAM_BURST_LENGTH_CH2_24WORD , ///< SIE Dram burst length Ch2 24 words
    SIE_DRAM_BURST_LENGTH_CH2_32WORD , ///< SIE Dram burst length Ch2 32 words
    ENUM_DUMMY4WORD(SIE_DRAM_BURST_LENGTH_CH2)
} SIE_DRAM_BURST_LENGTH_CH2;

typedef enum
{
    SIE_DRAM_BURST_LENGTH_CH3_16WORD   = 0,   ///< SIE Dram burst length Ch3 16 words
    SIE_DRAM_BURST_LENGTH_CH3_24WORD , ///< SIE Dram burst length Ch3 24 words
    SIE_DRAM_BURST_LENGTH_CH3_32WORD , ///< SIE Dram burst length Ch3 32 words
    ENUM_DUMMY4WORD(SIE_DRAM_BURST_LENGTH_CH3)
} SIE_DRAM_BURST_LENGTH_CH3;

typedef enum
{
    SIE_DRAM_BURST_LENGTH_CH4_16WORD   = 0,   ///< SIE Dram burst length Ch4 16 words
    SIE_DRAM_BURST_LENGTH_CH4_24WORD , ///< SIE Dram burst length Ch4 24 words
    SIE_DRAM_BURST_LENGTH_CH4_32WORD , ///< SIE Dram burst length Ch4 32 words
    ENUM_DUMMY4WORD(SIE_DRAM_BURST_LENGTH_CH4)
} SIE_DRAM_BURST_LENGTH_CH4;

typedef enum
{
    SIE_FLSH_MODE_COUNT     = 0,    ///< flash mode: enable, count delay then trigger
    SIE_FLSH_MODE_GPIO      ,    ///< flash mode: enable, wait GPIO then trigger
    SIE_FLSH_MODE_IMD       ,    ///< flash mode: enable then trigger
    ENUM_DUMMY4WORD(SIE_FLSH_MODE)
} SIE_FLSH_MODE;

typedef enum
{
    SIE_LEVEL_MODE_LEVEL = 0, ///< level mode
    SIE_LEVEL_MODE_PULSE = 1, ///< pulse mode
    ENUM_DUMMY4WORD(SIE_LEVEL_MODE)
} SIE_LEVEL_MODE;


typedef enum
{
    SIE_MSH_MODE_COUNT     = 0,    ///< flash mode: enable, count delay then trigger
    SIE_MSH_MODE_GPIO      ,    ///< flash mode: enable, wait GPIO then trigger
    SIE_MSH_MODE_IMD       ,    ///< flash mode: enable then trigger
    ENUM_DUMMY4WORD(SIE_MSH_MODE)
} SIE_MSH_MODE;


typedef enum
{
    SIE_SPI_TRIG_SEL_BP3 = 0, ///< trigger SPI at BP3
    SIE_SPI_TRIG_SEL_VD  , ///< trigger SPI at VD
    ENUM_DUMMY4WORD(SIE_SPI_TRIG_SEL)
} SIE_SPI_TRIG_SEL;



typedef enum
{
    SIE_BP_REF_ORI  = 0,   ///< break point reference: original VD, HD
    SIE_BP_REF_OFS  ,   ///< break point reference: offset VD, HD
    ENUM_DUMMY4WORD(SIE_BP_REF)
} SIE_BP_REF;


//Not for SIE//typedef enum
//Not for SIE//{
//Not for SIE//    SIE_DVI_FORMAT_CCIR601  = 0,   ///< CCIR 601
//Not for SIE//    SIE_DVI_FORMAT_CCIR656  ,   ///< CCIR 656
//Not for SIE//    ENUM_DUMMY4WORD(SIE_DVI_FORMAT)
//Not for SIE//} SIE_DVI_FORMAT;

//Not for SIE//typedef enum
//Not for SIE//{
//Not for SIE//    SIE_DVI_MODE_SD  = 0,   ///< SD mode
//Not for SIE//    SIE_DVI_MODE_HD  ,   ///< HD mode
//Not for SIE//    ENUM_DUMMY4WORD(SIE_DVI_MODE)
//Not for SIE//} SIE_DVI_MODE;

//Not for SIE//typedef enum
//Not for SIE//{
//Not for SIE//    SIE_CCIR656_ACT_SEL_EAV  = 0,   ///< EAV code recognition
//Not for SIE//    SIE_CCIR656_ACT_SEL_ACT  ,   ///< Active window size setting
//Not for SIE//    ENUM_DUMMY4WORD(SIE_CCIR656_ACT_SEL)
//Not for SIE//} SIE_CCIR656_ACT_SEL;


typedef enum
{
    SIE_PAT_COLORBAR    = 1,   ///< Color bar
    SIE_PAT_RANDOM         ,     ///< Random
    SIE_PAT_FIXED          ,      ///< Fixed
    SIE_PAT_HINCREASE      ,  ///< 1D Increment
    SIE_PAT_HVINCREASE     , ///< 2D increment
    ENUM_DUMMY4WORD(SIE_PATGEN_MODE)
} SIE_PATGEN_MODE;


typedef enum
{
    SIE_DEF_FACT_50  = 0,   ///< defect concealment cross-channel weighting = 50%
    SIE_DEF_FACT_25  ,   ///< defect concealment cross-channel weighting = 25%
    SIE_DEF_FACT_0   ,   ///< defect concealment cross-channel weighting =  0%
    ENUM_DUMMY4WORD(SIE_DEF_FACT)
} SIE_DEF_FACT;


typedef enum
{
    SIE_SMR_CFSIZE_15  = 0,   ///< curve filter size = 15
    SIE_SMR_CFSIZE_13  ,   ///< curve filter size = 13
    SIE_SMR_CFSIZE_11  ,   ///< curve filter size = 11
    SIE_SMR_CFSIZE_9   ,   ///< curve filter size = 9
    SIE_SMR_CFSIZE_7   ,  ///< curve filter size = 7
    SIE_SMR_CFSIZE_5   ,   ///< curve filter size = 5
    SIE_SMR_CFSIZE_3   ,   ///< curve filter size = 3
    SIE_SMR_CFSIZE_1   ,   ///< curve filter size = 1 (no filtering)
    ENUM_DUMMY4WORD(SIE_SMR_CFSIZE)
} SIE_SMR_CFSIZE;



typedef enum
{
    SIE_SMR_TRANS_WIDTH_0  = 0,   ///< no transition
    SIE_SMR_TRANS_WIDTH_2  ,   ///< transition width 2 pixel
    SIE_SMR_TRANS_WIDTH_4  ,   ///< transition width 4 pixel
    SIE_SMR_TRANS_WIDTH_8  ,   ///< transition width 8 pixel
    ENUM_DUMMY4WORD(SIE_SMR_TRANS_WIDTH)
} SIE_SMR_TRANS_WIDTH;


typedef enum
{
    SIE_SMR_NODEINT_1  = 0,   ///< interval = 1 (no down-sample)
    SIE_SMR_NODEINT_2  ,   ///< interval = 2 (1/2 down-sample)
    //eliminated//SIE_SMR_NODEINT_4  ,   ///< interval = 4 (1/4 down-sample)
    ENUM_DUMMY4WORD(SIE_SMR_NODEINT)
} SIE_SMR_NODEINT;


typedef enum
{
    SIE_PFPC_MODE_4X8  = 0,   ///< 4x8 period
    SIE_PFPC_MODE_8X4  ,   ///< 8x4 period
    ENUM_DUMMY4WORD(SIE_PFPC_MODE)
} SIE_PFPC_MODE;



typedef enum
{
    SIE_ECS_MAPSIZE_129X129  = 0,   ///< 129x129
    SIE_ECS_MAPSIZE_65X65    ,   ///< 65x65
    SIE_ECS_MAPSIZE_49X49    ,   ///< 49x49
    ENUM_DUMMY4WORD(SIE_ECS_MAPSIZE)
} SIE_ECS_MAPSIZE;


typedef enum
{
    SIE_CA_OUT_1_WORD  = 0,   ///< one word per window; [10:0] R, [20:11] G, [31:21] B
    SIE_CA_OUT_2_WORD  ,   ///< two words per window; [15:0] R, [31:16] G, [15:0] B
    ENUM_DUMMY4WORD(SIE_CA_OUT_MODE)
} SIE_CA_OUT_MODE;


typedef enum
{
    SIE_VA_FLTR_5_TAP  = 0,   ///< filter [-1 0 2 0 -1]
    SIE_VA_FLTR_7_TAP  ,   ///< filter [-1 0 0 0 2 0 0 0 -1]
    ENUM_DUMMY4WORD(SIE_VA_FLTR)
} SIE_VA_FLTR;

typedef enum
{
    SIE_CFA_R = 0, ///< start pixel is R
    SIE_CFA_Gr = 1, ///< start pixel is GR
    SIE_CFA_Gb = 2, ///< start pixel is GB
    SIE_CFA_B = 3, ///< start pixel is B
    ENUM_DUMMY4WORD(SIE_CFAPAT)
} SIE_CFAPAT;



/**
    SIE Pixel-Clock Source Selection

    Structure of Pixel-Clock source SIE configuration.
*/
//@{
typedef enum
{
    SIE_PXCLKSRC_OFF = 0, ///< SIE pixel-clock disabled
    SIE_PXCLKSRC_PAD    , ///< SIE pixel-clock enabled, source as pixel-clock-pad
    SIE_PXCLKSRC_MCLK   , ///< SIE pixel-clock enabled, source as MCLK
} SIE_PXCLKSRC;
//@}

/**
    SIE Pixel-Clock Source Selection

    Structure of Pixel-Clock source SIE configuration.
*/
//@{
typedef enum
{
    SIE_CLKSRC_CURR = 0, ///< SIE clock source as current setting(no setting, no changing)
    SIE_CLKSRC_480     , ///< SIE clock source as 480MHz
    SIE_CLKSRC_PLL5    , ///< SIE clock source as PLL5
    SIE_CLKSRC_PLL6    , ///< SIE clock source as PLL6
    SIE_CLKSRC_PLL7    , ///< SIE clock source as PLL7
} SIE_CLKSRC;
//@}



/**
    @name SIE enum - Change Funciton Parameter Selection
*/
//@{
typedef enum
{
    SIE_CHG_SIZE            =  0,  ///< change input and output image size, refer to structure "SIE_CHG_SIZE_INFO"
    SIE_CHG_IO_ADDR         ,  ///< change input and output DMA Address, refer to structure "SIE_CHG_IO_ADDR_INFO"
    SIE_CHG_OUT_SRC         ,  ///< change output source, refer to structure "SIE_CHG_OUT_SRC_INFO"
    SIE_CHG_REORDER         ,  ///< change re-order parameters, refer to "SIE_REORDER"
    SIE_CHG_DP              ,  ///< change defect pixel parameters, refer to "SIE_DEF_PXL"
    SIE_CHG_DCOL            ,  ///< change defect column parameters, refer to "SIE_DEF_COL"
    SIE_CHG_OB_OFS          ,  ///< change OB (Offset/Detect) parameters, refer to "SIE_OB_SUB"
    SIE_CHG_SMEAR           ,  ///< change smear detection/reduction parameters, refer to "SIE_SMEAR"
    SIE_CHG_ECS             ,  ///< change color shading compensation parameters, refer to "SIE_ECS"
    SIE_CHG_DGAIN           ,  ///< change digital gain parameters, refer to "SIE_DGAIN"
    SIE_CHG_BS_H            ,  ///< change horizontal bayer scaling parameters, refer to "SIE_BS_H"
    SIE_CHG_BS_V            ,  ///< change vertical bayer scaling parameters, refer to "SIE_BS_V"
    SIE_CHG_FLD_ARR         ,  ///< change field arrangement parameters, refer to "SIE_FLDARR"
    SIE_CHG_VIG             ,  ///< change vignette shading compensation parameters, refer to "SIE_VIG_INFO"
    SIE_CHG_CA              ,  ///< change color accumulation parameters, refer to "SIE_CA"
    SIE_CHG_VA              ,  ///< change variation accumulation parameters, refer to "SIE_VA"
    SIE_CHG_MSH_PARAM       ,  ///< change Mshutter parameters, refer to "SIE_MSHTR_PARAM"
    SIE_CHG_MSH_CLOSE       ,  ///< trigger Mshutter close operation
    SIE_CHG_MSH_OPEN        ,  ///< trigger Mshutter open operation
    SIE_CHG_FLSH_PARAM      ,  ///< change Flashlight parameters, refer to "SIE_FLASH_PARAM"
    SIE_CHG_FLSH_START      ,  ///< trigger Flashlight flash operation
    SIE_CHG_RAW_ENC         ,  ///< change Raw Encoder parameters, refer to "SIE_RAW_ENC_ADJ"
    SIE_CHG_OB_DT           ,  ///< change OB Detection parameters, refer to "SIE_OBDT"
    SIE_CHG_OB_FRAME        ,  ///< change OB Frame detection parameters, refer to "SIE_OBF"
    SIE_CHG_OB_PLANE        ,  ///< change OB Plane subtraction parameters, refer to "SIE_OBP"
    SIE_CHG_PFPC            ,  ///< change PFPC parameters, refer to "SIE_PFPC"
    SIE_CHG_GAMMA           ,  ///< change Gamma parameters, refer to "SIE_GAMMA_TBL"
    SIE_CHG_FLIP            ,  ///< change H/V Flip parameters, refer to "SIE_FLIP"
    SIE_CHG_SPI_TRIG        ,  ///< change SPI Trigger parameters, refer to "SIE_SPI_TRIG_SEL"
    SIE_CHG_DRAM_BURST      ,  ///< change dram burst length
    SIE_CHG_EMBDP           ,  ///< change embedded-defect-pixel parameters, refer to "SIE_EMBDP"
    ENUM_DUMMY4WORD(SIE_CHANGE_FUN_PARAM_SEL)
} SIE_CHANGE_FUN_PARAM_SEL;
//@}

//@}

/**
SIE Struct

@name   SIE_Struct
*/
//@{

typedef struct
{
    UINT32    uiHdSync; ///< HD sync period
    UINT32    uiVdSync; ///< VD sync period
    UINT32    uiHdTotal; ///< HD total period
    UINT32    uiVdTotal; ///< VD total period
    BOOL      bHdInv;
    BOOL      bVdInv;
    UINT32    uiHdOffset;
    UINT32    uiVdOffset;
} SIE_HDVD;

typedef struct
{
    UINT16    uiStX1; ///< horizontal active start of the 1st division.
    UINT16    uiStX2; ///< horizontal active start of the 2nd division.
    UINT16    uiStX3; ///< horizontal active start of the 3rd division.
    UINT16    uiStX4; ///< horizontal active start of the 4th division.
    UINT16    uiStX5; ///< horizontal active start of the 5th division.
    UINT16    uiStY1; ///< vertical active start of the 1st field.
    UINT16    uiStY2; ///< vertical active start of the 2nd field.
    UINT16    uiStY3; ///< vertical active start of the 3rd field.
    UINT16    uiStY4; ///< vertical active start of the 4th field.
    UINT16    uiStY5; ///< vertical active start of the 5th field.
    UINT16    uiStY6; ///< vertical active start of the 6th field.
    UINT16    uiStY7; ///< vertical active start of the 7th field.
    UINT16    uiStY8; ///< vertical active start of the 8th field.
    UINT16    uiStY9; ///< vertical active start of the 9th field.
    UINT16    uiStY10; ///< vertical active start of the 10th field.
    UINT16    uiStY11; ///< vertical active start of the 11th field.
    UINT16    uiStY12; ///< vertical active start of the 12th field.
    UINT16    uiStY13; ///< vertical active start of the 13th field.
    UINT16    uiStY14; ///< vertical active start of the 14th field.
    UINT16    uiStY15; ///< vertical active start of the 15th field.
    UINT16    uiStY16; ///< vertical active start of the 16th field.
    UINT32    uiSzX; ///< horizontal size
    UINT32    uiSzY; ///< vertical size
    SIE_CFAPAT    CfaPat;///< CFA start pattern
} SIE_ACTWIN;


typedef struct
{
    UINT16    uiStX; ///< horizontal crop start
    UINT16    uiStY; ///< vertical crop start
    UINT32    uiSzX; ///< horizontal size
    UINT32    uiSzY; ///< vertical size
    SIE_CFAPAT    CfaPat;///< CFA start pattern
} SIE_CRPWIN;

typedef struct
{
    SIE_HDIV_NUM    uiDivNum; ///< horizontal division number
} SIE_HDIV;


typedef struct
{
    BOOL              bReoHEn;   ///< enable for h
    BOOL              bReoVEn;   ///< enable for v
    SIE_REO_LNNUM    ReoLineNum; ///< line number
    UINT32            uiStX;     ///< start pixel
    UINT8             uiReoIndex[8]; ///< re-order indexing
} SIE_REORDER;

typedef struct
{
    BOOL              bDpcEn;   ///< enable for defect pixel concealment
    UINT32            uiDpTblAddr;///< dp table address
    UINT32            uiDpTblSize;///< dp table Size, in bytes
} SIE_DEF_PXL;

typedef struct
{
    BOOL              bEmbDpcEn;   ///< enable for embedded-defect-pixel function
    UINT32            uiDpMark;    ///< defect mark value, in 12 bits
} SIE_EMBDP;

typedef struct
{
    BOOL                    bFldArrEn;     ///< enable
    SIE_FLDARR_FLDNUM      FldNum; ///< total number of fields. 0: 1 field; 1: 2 fields, ..., 15: 16 fields.
    UINT8                   uiFldIndex[16][2]; ///< field num of the M(out of 16) input field, Line N(out of 2); value = 1, 2, 3, 4...
} SIE_FLDARR;

typedef struct
{
    SIE_FLSH_MODE FlshMode;
    SIE_PHASE FlshInPhase;
    SIE_POLARITY FlshOutPol;
    UINT32 uiLineDelay;
    UINT32 uiPx2Delay;
    UINT32 uiFlashWidth;
} SIE_FLASH_PARAM;


typedef struct
{
    SIE_PHASE VdPhase; ///< VD phase
    SIE_PHASE HdPhase; ///< HD phase
    SIE_PHASE DataPhase; ///< Data phase
} SIE_SENSOR_PIN_PHASE;

typedef struct
{
    SIE_LEVEL_MODE MshLvlMode;
    BOOL bPinSwap;
    SIE_POLARITY MshOutPol;
    SIE_PHASE MshInPhase;
    SIE_MSH_MODE  MshCloseMode;
    UINT32 uiPulseWidth;
    UINT32 uiCloseFrameDelay;
    UINT32 uiCloseLineDelay;
    UINT32 uiClosePx2Delay;
} SIE_MSHTR_PARAM;


typedef struct
{
    UINT32 uiBp1;             ///< break point 1
    UINT32 uiBp2;             ///< break point 2
    UINT32 uiBp3;             ///< break point 3
    SIE_BP_REF BpRefBp1;    ///< break point 1 reference
    SIE_BP_REF BpRefBp2;    ///< break point 2 reference
    SIE_BP_REF BpRefBp3;    ///< break point 3 reference
} SIE_BREAKPOINT;

//Not for SIE//typedef struct
//Not for SIE//{
//Not for SIE//    SIE_DVI_FORMAT         DviFormat;             ///< format: 601 or 656
//Not for SIE//    SIE_DVI_MODE           DviMode;               ///< mode: SD or HD
//Not for SIE//    SIE_CCIR656_ACT_SEL    Ccir656ActSel;      ///< CCIR656 Active size selection
//Not for SIE//    BOOL                    bFieldEn;           ///< switch: start input from field with internal field signal=0
//Not for SIE//    BOOL                    bFieldInv;          ///< switch: inverse the polarity of internal field signal
//Not for SIE//    BOOL                    bCcir656InSwap;     ///< input Y/C channel pin swap for CCIR656
//Not for SIE//    BOOL                    bCcir656OutSwap;    ///< output Y/C channel pin swap for CCIR656
//Not for SIE//} SIE_DVI;


typedef struct
{
    BOOL        bDefColEn;      ///< enable
    UINT32      uiColNum;         ///< defect column number
    UINT16      uiDefColIndex[32];  ///< defect column index array
} SIE_DEF_COL;

typedef struct
{
    BOOL        bObdtEn; ///< enable
    UINT32      uiStX;    ///< window horizontal start
    UINT32      uiStY;    ///< window vertical start
    UINT32      uiSzX;    ///< window horizontal size
    UINT32      uiSzY;    ///< window vertical size
    UINT32      uiDivX;   ///< window horizontal subsample
    UINT32      uiThres0; ///< threshold 0
    UINT32      uiThres1; ///< threshold 1
} SIE_OBDT;


typedef struct
{
    UINT16      uiObOfs;
    UINT16      uiObGain;
} SIE_OB_SUB;


typedef struct
{
    BOOL        bObfEn;      ///< enable
    UINT32      uiTopX;       ///< top windows horizontal position (also for bottom windows)
    UINT32      uiTopY;       ///< top windows vertical position
    UINT32      uiTopIntvl;   ///< top windows horizontal interval (also for bottom windows)
    UINT32      uiThresold;      ///< threshold for all windows
    UINT32      uiNormFctr;   ///< normalization factor
    UINT32      uiLeftX;      ///< left windows horizontal position
    UINT32      uiLeftY;      ///< left windows vertical position (also for right windows)
    UINT32      uiLeftIntvl;  ///< left windows horizontal interval (also for right windows)
    UINT32      uiBotY;       ///< bottom windows vertical position
    UINT32      uiRightX;     ///< right windows horizontal position
} SIE_OBF;


typedef struct
{
    UINT8       uiTopRslt[8];      ///< top window result
    UINT8       uiLeftRslt[8];     ///< left window result
    UINT8       uiBotRslt[8];      ///< bottom window result
    UINT8       uiRightRslt[8];    ///< right window result
} SIE_OBF_RSLT;


typedef struct
{
    BOOL        bObpEn;      ///< enable
    UINT32      uiHSclFctr;       ///< horizontal scaling factor
    UINT32      uiVSclFctr;       ///< vertical scaling factor
    UINT32      uiShift;      ///< left shift bit number
    UINT8       *uiObpTbl;       ///< table (size=17x17=289)
} SIE_OBP;

typedef struct
{
    BOOL            bSmrEn;          ///< enable
    UINT32          uiTfMoveStp1;     ///< temporal filter move step 1
    UINT32          uiTfMoveStp2;     ///< temporal filter move step 2
    UINT32          uiSmrY;           ///< smear line vetical position
    UINT32          uiCfOutMax;       ///< curve filter output max
    UINT32          uiSmrLnSel;       ///< smear line selection for multi-line mode

    SIE_SMR_CFSIZE CfSize;         ///< curve filter size
    UINT32      uiCfTh1;     ///< curve filter threshold 1
    UINT32      uiCfTh2;     ///< curve filter threshold 2
    UINT32      uiOutlTh;     ///< outlier removal threshold
    UINT32      uiStaLevel;     ///< static level of optical black
    UINT32      uiStaLvlTh1;     ///< upper bound 1 of static level
    UINT32      uiStaLvlTh2;     ///< upper bound 2 of static level

    UINT32      uiSzThIn;         ///< safe zone in threshold
    UINT32      uiSzThOut;     ///< safe zone out threshold
    SIE_SMR_TRANS_WIDTH      TransWidth;     ///< transition width
    SIE_SMR_NODEINT          NodeInt;     ///< node interval (related to down sample rate)
    UINT32      uiTfTh1;     ///< temporal filter threshold 1
    UINT32      uiTfTh2;     ///< temporal filter threshold 2
    BOOL        bInputSmrLn;     ///< Input smear line from DRAM
    UINT32      uiReduWeight;     ///< smear reduction weight (0x80 as 100%)
    UINT32      uiReduMaxSrcVal;     ///< source value over this threshold remains
} SIE_SMEAR;


typedef struct
{
    BOOL            bPfpcEn;     ///< enable
    SIE_PFPC_MODE  PfpcMode;       ///< period mode
    UINT8           uiGainTbl[32];   ///< gain table
    UINT8           uiOfsTbl[32];    ///< offset table
} SIE_PFPC;


typedef struct
{
    BOOL            bGammaEn;    ///< enable
    BOOL            bGammaUpdate;///< Update Gamma LUT or not
    UINT16          *uiGammaTblR;   ///< R table
    UINT16          *uiGammaTblGr;  ///< Gr table
    UINT16          *uiGammaTblGb;  ///< Gb table
    UINT16          *uiGammaTblB;   ///< B table
} SIE_GAMMA_TBL;




typedef struct
{
    BOOL                bEcsEn;      ///< enable
    SIE_ECS_MAPSIZE    MapSizeSel; ///< vig compensation - map size
    UINT32              uiMapMulShift;///< vig compensation - multiplication shift number
    BOOL                bDthrEn;     ///< vig compensation - vignette dithering enable
    BOOL                bDthrRst;    ///< vig compensation - vignette dithering reset
    UINT32              uiDthrLvl;    ///< vig compensation - vignette dithering level; 0x0=bit[1:0], 0x7=bit[8:7]
    UINT32              uiHSclFctr;       ///< vig compensation - map horizontal scaling factor
    UINT32              uiVSclFctr;       ///< vig compensation - map vertical scaling factor
} SIE_ECS;

typedef struct
{
    BOOL                bDGainEn;           ///< enable
    UINT32              uiGainIn3P7Bit;   ///< digital gain in 3.7 bits format
} SIE_DGAIN;



typedef struct
{
    BOOL            bBsHEn;  ///< enable
    UINT32          uiIv;     ///< bayer scaling - integration value
    UINT32          uiSv;     ///< bayer scaling - skipping value
    UINT32          uiBvR;    ///< bayer scaling - begin value for R/Gb channel
    UINT32          uiBvB;    ///< bayer scaling - begin value for B/Gr channel
    UINT32          uiOutSize;///< bayer scaling - output size
    UINT32          uiDivM;   ///< bayer scaling - division parameter M
    UINT32          uiDivS;   ///< bayer scaling - division parameter S
} SIE_BS_H;


typedef struct
{
    BOOL            bBsVEn;  ///< enable
    UINT32          uiIv;     ///< bayer scaling - integration value
    UINT32          uiSv;     ///< bayer scaling - skipping value
    UINT32          uiBvR;    ///< bayer scaling - begin value for R/Gb channel
    UINT32          uiBvB;    ///< bayer scaling - begin value for B/Gr channel
    UINT32          uiOutSize;///< bayer scaling - output size
    UINT32          uiDivM;   ///< bayer scaling - division parameter M
    UINT32          uiDivS;   ///< bayer scaling - division parameter S
    UINT32          uiInShft; ///< bayer scaling - input shift
    UINT32          uiDivMRb[16];///< bayer scaling - division parameter M for R/B
    UINT32          uiDivSRb[16];///< bayer scaling - division parameter S for R/B
} SIE_BS_V;


typedef struct
{
    BOOL   bVigEn;       ///< enable
    UINT32 uiCntrX;      ///< vig compensation - x center
    UINT32 uiCntrY;      ///< vig compensation - y center
    UINT8 uiVigMul;      ///< vig compensation - vignette distance gain. unsigned 2.6
    UINT8 uiVigT;        ///< vig compensation - vignette disabled area
    UINT8 uiVigTabGain;  ///< vig compensation - vignette gain factor
    BOOL bVigDitherEn;  ///< vig compensation - vignette dithering enable
    //unnucessary after fld-arr//UINT8 uiYDist; ///< vignette Y subsampling factor
    UINT32 uiVigXDiv;    ///< vig compensation - vignette scaling factor in x direction. unsigned 0.6
    UINT32 uiVigYDiv;    ///< vig compensation - vignette scaling factor in y direction. unsigned 0.6
    UINT32 uiYDist;    ///< vig compensation - vertical consecutive line distance (for multi-field)
    UINT8 *puiVigLut;   ///< vig compensation - vignette LUT
} SIE_VIG_INFO;


typedef struct
{
    UINT32              uiImgXSize;     ///< vig compensation - image horizontal size
    UINT32              uiImgYSize;     ///< vig compensation - image vertical size
    UINT32              uiImgXCntr;     ///< vig compensation - image horizontal center
    UINT32              uiImgYCntr;     ///< vig compensation - image vertical center
    UINT32              uiImgXratio;    ///< vig compensation - image horizontalratio (16 from 16:9)
    UINT32              uiImgYratio;    ///< vig compensation - image vertical ratio (9 from 16:9)
} SIE_VIG_SETTING_ONE_IMG;

typedef struct
{
    UINT32              uiCaliImgSizeX;     ///< vig compensation - calibration image size X, in pixel
    UINT32              uiCaliImgSizeY;     ///< vig compensation - calibration image size Y, in pixel
    UINT32              uiCaliImgVigCntrX;  ///< vig compensation - calibration image center X, in pixel
    UINT32              uiCaliImgVigCntrY;  ///< vig compensation - calibration image center Y, in pixel
    UINT32              uiCaliImgRatioX;    ///< vig compensation - calibration image X:Y Ratio (ex: 16 for 16:9)
    UINT32              uiCaliImgRatioY;    ///< vig compensation - calibration image Y:Y Ratio (ex: 9  for 16:9)
    UINT32              uiCorrImgSizeX;     ///< vig compensation - correction  image size X, in pixel
    UINT32              uiCorrImgSizeY;     ///< vig compensation - correction  image size Y, in pixel
    UINT32              uiCorrImgSclRatioX; ///< vig compensation - correction  image Scaling (down) ratio X, in 1/1000 (ex: 500 for 1/2 scaling down)
    UINT32              uiCorrImgSclRatioY; ///< vig compensation - correction  image Scaling (down) ratio Y, in 1/1000 (ex: 500 for 1/2 scaling down)
    UINT32              uiCorrImgPosRatioX; ///< vig compensation - correction  image position ratio (compared to full image) X, in 1/1000 (ex: 500 for Nx Dzoom)
    UINT32              uiCorrImgPosRatioY; ///< vig compensation - correction  image position ratio (compared to full image) Y, in 1/1000 (ex: 500 for Nx Dzoom)
} SIE_VIG_SETTING_TWO_IMG;

typedef struct
{
    BOOL                bVaEn;       ///< enable
    UINT32              uiOutShift;   ///< variation accumulation - output shift
    UINT32              uiStX;        ///< variation accumulation - horizontal window start position
    UINT32              uiStY;        ///< variation accumulation - vertical window start position
    UINT32              uiSzX;        ///< variation accumulation - window horizontal size
    UINT32              uiSzY;        ///< variation accumulation - window vertical size
    UINT32              uiDivX;       ///< variation accumulation - horizontal subsample
    UINT32              uiDivY;       ///< variation accumulation - vertical subsample
    UINT32              uiWinNumX;    ///< variation accumulation - horizontal window number
    UINT32              uiWinNumY;    ///< variation accumulation - vertical window number
    UINT32              uiVaTh;  ///< variation accumulation - variation accumulation threshold
    SIE_VA_FLTR        VaFltrSel;    ///< variation accumulation - variation filter selection
} SIE_VA;

typedef struct
{
    UINT32              uiGroupStx;   ///< variation accumulation setting - horizontal window-group start position
    UINT32              uiGroupSty;   ///< variation accumulation setting - vertical window-group start position
    UINT32              uiGroupSzx;   ///< variation accumulation setting - window-group horizontal size
    UINT32              uiGroupSzy;   ///< variation accumulation setting - window-group vertical size
    UINT32              uiWinNumX;    ///< variation accumulation setting - horizontal window number
    UINT32              uiWinNumY;    ///< variation accumulation setting - vertical window number
    UINT32              uiVaTh;  ///< variation accumulation - variation accumulation threshold
    SIE_VA_FLTR        VaFltrSel;    ///< variation accumulation - variation filter selection
} SIE_VA_SETTING;

typedef struct
{
    BOOL                bCaEn;       ///< enable
    SIE_CA_OUT_MODE    CaOutMode;    ///< color accumulation - output mode
    UINT32              uiOutShift;   ///< color accumulation - output shift
    UINT32              uiStX;        ///< color accumulation - horizontal window start position
    UINT32              uiStY;        ///< color accumulation - vertical window start position
    UINT32              uiSzX;        ///< color accumulation - window horizontal size
    UINT32              uiSzY;        ///< color accumulation - window vertical size
    UINT32              uiDivX;       ///< color accumulation - horizontal subsample
    UINT32              uiDivY;       ///< color accumulation - vertical subsample
    UINT32              uiWinNumX;    ///< color accumulation - horizontal window number
    UINT32              uiWinNumY;    ///< color accumulation - vertical window number
} SIE_CA;

typedef struct
{
    SIE_CA_OUT_MODE    CaOutMode;    ///< color accumulation setting - output mode
    UINT32              uiGroupStx;   ///< color accumulation setting - horizontal window-group start position
    UINT32              uiGroupSty;   ///< color accumulation setting - vertical window-group start position
    UINT32              uiGroupSzx;   ///< color accumulation setting - window-group horizontal size
    UINT32              uiGroupSzy;   ///< color accumulation setting - window-group vertical size
    UINT32              uiWinNumX;    ///< color accumulation setting - horizontal window number
    UINT32              uiWinNumY;    ///< color accumulation setting - vertical window number
} SIE_CA_SETTING;


typedef struct
{
    BOOL                bHFlipEn;        ///< enable H flip
    BOOL                bVFlipEn;        ///< enable V flip
} SIE_FLIP;

typedef struct
{
    BOOL                bSpiTrgEn;   ///< enable
    SIE_SPI_TRIG_SEL       SpiTrgSel;     ///< SPI Trigger Timing
} SIE_SPI_TRIG;

typedef struct
{
    BOOL              bInRound;          ///< Raw encoder - input truncation or rounding
    SIE_ROUND_SEL    EncRoundSel;      ///< Raw encoder - encode rounding selection
    UINT32            uiEncRoundRI;       ///< Raw encoder - encode random rounding initial value
    UINT32            uiBsMaxSize;        ///< Raw encoder - maximum encoded output bit stream size in byte
    UINT32            uiBrcInitOffPrd;    ///< Raw encoder - BRC initial disabled line number
    UINT32            uiBrcRefBase;       ///< Raw encoder - BRC refresh interval base
    UINT32            uiBrcRefCnt;        ///< Raw encoder - BRC refresh interval count
    UINT32            uiBrcLvlInit;       ///< Raw encoder - BRC initial level setting
    UINT32            uiBrcQInit;         ///< Raw encoder - BRC initial Q setting
    UINT32            uiBrcSegBitNo;      ///< Raw encoder - BRC target encoded bit number per segment
    UINT32            uiVbfSizeTol;       ///< Raw encoder - Virtual buffer toerance for quality factor update
    UINT32            uiVbfSizeMax;       ///< Raw encoder - Maximum virtual buffer size for overflow and underflow
} SIE_RAW_ENC_PARAM;

typedef struct
{
    BOOL              bRawEncEn;         ///< enable
    UINT32            uiCompRate;         ///< Raw encoder - compression rate, in %, range 40~60%
    UINT32            uiBsMaxSize;        ///< Raw encoder - maximum encoded output bit stream size in byte
} SIE_RAW_ENC_ADJ;

typedef struct
{
    UINT8             uiQTbl[8][8];       ///< Raw encoder - Q table [BRC][LFN]
    UINT8             uiQMax[8];          ///< Raw encoder - Max Q table [LFN]
    UINT8             uiQMin[8];          ///< Raw encoder - Min Q table [LFN]
} SIE_RAW_ENC_TBL;

typedef struct
{
    UINT32            uiVbfOvflCnt;       ///< Raw encoder - accumulated bit count exceed maximum virtual buffer size count
    UINT32            uiVbfUdflCnt;       ///< Raw encoder - accumulated bit count exceed minimum virtual buffer size count
    UINT32            uiOutputSize;       ///< Raw encoder - accumulated output byte count
    UINT16            uiLfnLnCnt[8];        ///< Raw encoder - line count for max. LFN 0~7 per line
} SIE_RAW_ENC_STATUS;



/**
    SIE Partial Configuration for parameter change of IO Size

    Structure of IO Size SIE configuration.
*/
//@{
typedef struct
{
    SIE_CRPWIN               CrpInfo; ///< crop window info
    UINT32                   uiOut0Lofs;  ///< SIE output ch0 lineoffset
    UINT32                   uiOut2Lofs;  ///< SIE output ch2 lineoffset
} SIE_CHG_SIZE_INFO;
//@}

/**
    SIE Partial Configuration for parameter change of IO Size

    Structure of IO Size SIE configuration.
*/
//@{
typedef struct
{
    UINT32                   uiIn0Addr; ///< SIE input ch0 dram addr
    UINT32                   uiIn1Addr; ///< SIE input ch1 dram addr
    UINT32                   uiOut0Add0; ///< SIE output ch0 dram addr 0
    UINT32                   uiOut0Add1; ///< SIE output ch0 dram addr 1
    UINT32                   uiOut1Add0; ///< SIE output ch1 dram addr 0
    UINT32                   uiOut1Add1; ///< SIE output ch1 dram addr 1
    UINT32                   uiOut2Add0; ///< SIE output ch2 dram addr 0
    UINT32                   uiOut2Add1; ///< SIE output ch2 dram addr 1
    UINT32                   uiIn0Size; ///< SIE input ch0 dram size, in bytes, for cache-handle
    UINT32                   uiIn1Size; ///< SIE input ch1 dram size, in bytes, for cache-handle
    UINT32                   uiOut0Size; ///< SIE output ch0 dram size, in bytes, for cache-handle
    UINT32                   uiOut1Size; ///< SIE output ch1 dram size, in bytes, for cache-handle
    UINT32                   uiOut2Size; ///< SIE output ch2 dram size, in bytes, for cache-handle
} SIE_CHG_IO_ADDR_INFO;
//@}

/**
    SIE Partial Configuration for parameter change of Dram Burst Length

    Structure of IO Size SIE configuration.
*/
//@{
typedef struct
{
    SIE_DRAM_BURST_LENGTH_CH0   DramBstLengCh0; ///< SIE Dram Burst Length output ch1
} SIE_CHG_DRAM_BURST_INFO;
//@}

/**
    SIE Partial Configuration for parameter change of Output source

    Structure of output source SIE configuration.
*/
//@{
typedef struct
{
    BOOL                     bOut0Change;///< SIE output ch0 change switch
    BOOL                     bOut2Change;///< SIE output ch2 change switch
    SIE_OUT0_SOURCE         Out0Source; ///< SIE output ch0 source
    SIE_OUT2_SOURCE         Out2Source; ///< SIE output ch2 source
} SIE_CHG_OUT_SRC_INFO;
//@}


/**
    SIE Open Object.

    Open Object is used for opening SIE module.
    \n Used for sie_open()

*/
//@{
typedef struct _SIE_OPENOBJ{
    void (*pfSieIsrCb)(UINT32 uiIntpStatus); ///< isr callback function
    UINT32 uiSieClockRate;  ///< SIE clock rate in Hz; configure '0' to skip set-up
    SIE_PXCLKSRC PxclkSel;  ///< SIE pixel-clock source selection; valid in Parallel(Master/Slave) modes, invalid in Serial mode
    SIE_CLKSRC SieClkSel;   ///< SIE clock source selection
} SIE_OPENOBJ;
//@}


/**
    SIE Entire Configuration.

    Structure of entire SIE configuration.
*/
//@{
typedef struct
{
    SIE_SENSOR_PIN_PHASE    SnrPinPhase;
    SIE_PRETRIGSEL         PreTrigSel;
    SIE_FRONTENDMODE       FrontEndMode;
    UINT32                  uiFuncEn;
    UINT32                  uiIntrpEn;
    SIE_HDVD                SieVdHd;
    SIE_ACTWIN              ActParam;
    SIE_CRPWIN              CrpParam;
    SIE_HDIV                HDiv;
    SIE_REORDER             Reorder;
    SIE_FLDARR              FldArr;

    UINT32                   uiIn0Addr;
    UINT32                   uiIn0Size;

    UINT32                   uiIn1Addr;
    SIE_PACKBUS             In1PackBus;

    UINT32                   uiOut0Add0;
    UINT32                   uiOut0Add1;
    UINT32                   uiOut0Lofs;
    SIE_BUF_NUM             Out0BufNum;
    SIE_PACKBUS             Out0PackBus;
    SIE_OUT0_SOURCE         Out0Source;

    UINT32                   uiOut1Add0;
    UINT32                   uiOut1Add1;
    SIE_BUF_NUM             Out1BufNum;

    UINT32                   uiOut2Add0;
    UINT32                   uiOut2Add1;
    UINT32                   uiOut2Lofs;
    SIE_BUF_NUM             Out2BufNum;
    SIE_PACKBUS             Out2PackBus;
    SIE_OUT2_SOURCE         Out2Source;

    SIE_DRAM_BURST_LENGTH_CH0   DramBstLengCh0;
    SIE_DRAM_BURST_LENGTH_CH1   DramBstLengCh1;
    SIE_DRAM_BURST_LENGTH_CH2   DramBstLengCh2;
    SIE_DRAM_BURST_LENGTH_CH3   DramBstLengCh3;
    SIE_DRAM_BURST_LENGTH_CH4   DramBstLengCh4;

    SIE_BREAKPOINT           BreakPoint;

//Not for SIE//    SIE_DVI                 DviParam;
    SIE_PATGEN_MODE        PatGenMode;
    UINT32                  uiPatGenVal;
    UINT32                  uiDpMark;
    SIE_DEF_FACT           uiDefFact;
    SIE_DEF_COL             DefCol;

    SIE_OB_SUB              ObsubParam;
    SIE_OBDT                ObdtParam;
    SIE_OBF                 ObfParam;
    SIE_OBP                 ObpParam;
    SIE_SMEAR               SmrParam;
    SIE_PFPC                PfpcParam;
    SIE_ECS                 EcsParam;

    UINT32                  uiDGainIn3P7Bit;
    SIE_BS_H                BshParam;
    SIE_BS_V                BsvParam;
    SIE_VIG_INFO            VigParam;
    SIE_CA                  CaParam;
    SIE_VA                  VaParam;
    SIE_GAMMA_TBL               GammaParam;
    //UINT16                  *uiGammaTblR;//not-yet: should be deleted
    //UINT16                  *uiGammaTblGr;//not-yet: should be deleted
    //UINT16                  *uiGammaTblGb;//not-yet: should be deleted
    //UINT16                  *uiGammaTblB;//not-yet: should be deleted
    SIE_RAW_ENC_ADJ         RawEncAdj;
} SIE_MODE_PARAM;
//@}

//@}

/**
SIE API

@name   SIE_API
*/
//@{

// sie_lib.c
//driver-policy: SWRST is limited//extern ER sie_reset(void);//not-yet: should be removed
extern ER sie_open(SIE_OPENOBJ *pObjCB);
extern BOOL sie_isOpened(void);
extern ER sie_close(void);
extern ER sie_pause(void);
extern ER sie_start(void);
extern ER sie_setMode(SIE_MODE_PARAM *pSieParam);
extern ER sie_chgClock(UINT32 uiClkRate);
extern ER sie_chgClockSource(SIE_CLKSRC SieClkSource);
extern ER sie_openClock(void);
extern ER sie_closeClock(void);
extern BOOL sie_isClockOpened(void);
extern ER sie_chgParam(void *pParam, SIE_CHANGE_FUN_PARAM_SEL FunSel);
extern void sie_waitVD(UINT32 uiVdCnt);
extern void sie_waitBP1(BOOL bClrFlag);
extern void sie_waitBP2(BOOL bClrFlag);
extern void sie_waitFlashDone(BOOL bClrFlag);
extern void sie_waitMshtrDone(BOOL bClrFlag);
// sie_int.c
extern void sie_setReset(BOOL bReset);
extern void sie_setLoad(void);
extern void sie_setFlashStart(void);
extern void sie_setMshtrCloseStart(void);
extern void sie_setMshtrOpenStart(void);
extern BOOL sie_checkIntEnable(UINT32 uiSieIntrp);
extern UINT32 sie_getIntrStatus(void);
extern void sie_clrIntrStatus(UINT32 uiIntrpStatus);
extern void sie_getCrpWindow(SIE_CRPWIN *pCrpParam);
extern void sie_setOut0Add(UINT32 uiIndex, UINT32 uiAddr);
extern UINT32 sie_getOut0Add(UINT32 uiIndex);
extern void sie_setOut0Lofs(UINT32 uiLineoffset);
extern UINT32 sie_getOut0Lofs(void);
extern void sie_setFrontEndMode(SIE_FRONTENDMODE FrontEndMode);
extern void sie_setOut0PackBus(SIE_PACKBUS PackBus);
extern SIE_PACKBUS sie_getOut0PackBus(void);
extern void sie_getECS(SIE_ECS *pEcsParam);
extern void sie_setSmearRedu(SIE_SMEAR *pSmrParam);
extern void sie_getActCrpSizeReo(UINT32 *puiActSzxReo, UINT32 *puiCrpSzxReo);
extern SIE_PACKBUS sie_getIn1PackBus(void);
extern void sie_getCA(SIE_CA *pCaParam);
extern SIE_OUT0_SOURCE sie_getOut0Source(void);
extern UINT32 sie_getOut1Add(UINT32 uiIndex);
extern SIE_OUT2_SOURCE sie_getOut2Source(void);
extern void sie_getVA(SIE_VA *pVaParam);
extern void sie_getBSH(SIE_BS_H *pBshParam);
extern void sie_getBSV(SIE_BS_V *pBsvParam);
extern SIE_PACKBUS sie_getOut2PackBus(void);
extern void sie_getSensorSync(SIE_HDVD *pSieVdHd);
extern UINT32 sie_getOut2Lofs(void);
extern void sie_getReorder(SIE_REORDER *pReorder);
extern void sie_getFldArr(SIE_FLDARR *pFldArr);
extern void sie_calBSHScl(SIE_BS_H *pBshParam, UINT32 uiInSz, UINT32 uiOutSz, UINT32 uiLpf, UINT32 uiBinPwr);
extern void sie_calBSVScl(SIE_BS_V *pBsvParam, UINT32 uiInSz, UINT32 uiOutSz, UINT32 uiLpf, UINT32 uiBinPwr);
extern void sie_enableFunction(BOOL bEnable,UINT32 uiFunction);
extern void sie_setFunction(UINT32 uiFunction);
extern BOOL sie_checkFunctionEnable(UINT32 uiFunction);
extern void sie_calcCA(SIE_CA *pCaParam, SIE_CA_SETTING *pCaSetting);
extern void sie_calcVA(SIE_VA *pVaParam, SIE_VA_SETTING *pVaSetting);
extern void sie_calcVIG_OneImg(SIE_VIG_INFO *pVigParam, SIE_VIG_SETTING_ONE_IMG *pVigSettingOneImg);
extern void sie_calcVIG_TwoImg(SIE_VIG_INFO *pVigParam, SIE_VIG_SETTING_TWO_IMG *pVigSettingTwoImg);
extern void sie_getCAResult(UINT16 *puiBufR, UINT16 *puiBufG, UINT16 *puiBufB);
extern void sie_getCAResultManual(UINT16 *puiBufR, UINT16 *puiBufG, UINT16 *puiBufB, SIE_CA *pCaParam, UINT32 uiBufAddr);
extern void sie_getVAResult(UINT16 *puiBufVa);
extern void sie_getRawEncRslt(SIE_RAW_ENC_STATUS *pRawEncStatus);
extern void sie_getRawEncRsltLastFrame(SIE_RAW_ENC_STATUS *pRawEncStatus);
extern UINT32 sie_getCurrentOutputBuffID(void);
extern void sie_getSnrPinPhase(SIE_SENSOR_PIN_PHASE *pSnrPinPhase);
extern UINT32 sie_getOBDTAvg0(void);
extern void sie_getOBFRslt(SIE_OBF_RSLT *pObfRslt);
extern void sie_calOBPScl(SIE_OBP *pObpParam, UINT32 uiActSzX, UINT32 uiActSzY);
extern void sie_calECSScl(SIE_ECS *pEcsParam, UINT32 uiActSzX, UINT32 uiActSzY);
extern void sie_getSmearRedu(SIE_SMEAR *pSmrParam);
extern UINT32 sie_getClockRate(void);
#endif
//@}

//@}


