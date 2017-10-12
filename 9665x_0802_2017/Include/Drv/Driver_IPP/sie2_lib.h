/**
    Public header file for SIE2 module.

    @file       sie2_lib.h
    @ingroup    mIIPPSIE

    @brief

    How to start:\n
    [Preview]\n
    1. sie2_attach\n
    2. sie2_open\n
    3. SIE2_Operation = SIE2_ENGINE_SET2PRV, sie2_setMode\n
    4. SIE2_Operation = SIE2_ENGINE_SET2RUN, sie2_setMode\n

    [Preview to Capture]\n
    1. SIE2_operation = SIE2_ENGINE_SET2PAUSE, sie2_setMode\n
    2. SIE2_Operation = SIE2_ENGINE_SET2CAP, sie2_setMode\n
    3. SIE2_Operation = SIE2_ENGINE_SET2RUN, sie2_setMode\n
    4. SIE2_operation = SIE2_ENGINE_SET2PAUSE, sie2_setMode\n


    How to stop:\n
    1. SIE2_operation = SIE2_ENGINE_SET2PAUSE, sie2_setMode\n
    2. sie2_close\n
    3. sie2_detach\n


    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _SIE2_LIB_H
#define _SIE2_LIB_H

#define _SIE2_LIB_H_FOR_FW_ (1)
#define _SIE2_LIB_H_FOR_GEARGEN_ (0)

#if _SIE2_LIB_H_FOR_FW_
// include files for FW
#include "Type.h"
#elif _SIE2_LIB_H_FOR_GEARGEN_
// include files for GearGen
#include    "..\\type.h"
#endif//_SIE2_LIB_H_FOR_GEARGEN_


/**
SIE2 Definition

@name   SIE2_Define
*/
//@{

/*
    Define SIE2 Function Enable
*/
//@{
//#define SIE2_ACT_EN                0x00000004   ///< enable function: Active window(Flow after active window)
//Not for SIE2//#define SIE2_RAWENC_EN             0x00000008   ///< enable function: Raw encoder
//Not for SIE2//#define SIE2_SPITRIG_EN            0x00000020   ///< enable function: SPI trigger
#define SIE2_DVI_EN                0x00000040   ///< enable function: DVI
#define SIE2_PATGEN_EN             0x00000080   ///< enable function: Pattern Generator
//Not for SIE2//#define SIE2_REORDER_H_EN          0x00000100   ///< enable function: Horizontal re-order
//Not for SIE2//#define SIE2_REORDER_V_EN          0x00000200   ///< enable function: Vertical re-order
#define SIE2_DPC_EN                0x00000400   ///< enable function: Defect pixel concealment
//Not for SIE2//#define SIE2_EMBDPC_EN             0x00000800   ///< enable function: Embeded defect pixel mark
#define SIE2_COLDEF_EN             0x00001000   ///< enable function: Column defect concealment
//Not for SIE2//#define SIE2_SMR_DETRED_EN         0x00002000   ///< enable function: Smear detection and reduction
#define SIE2_OB_AVG_EN             0x00004000   ///< enable function: OB average
#define SIE2_OB_SUB_SEL            0x00008000   ///< enable function: OB subtraction selection
//Not for SIE2//#define SIE2_OB_FRAME_AVG_EN       0x00010000   ///< enable function: OB-Frame average
//Not for SIE2//#define SIE2_OB_PLAME_SUB_EN       0x00020000   ///< enable function: OB-Plane subtraction
//Not for SIE2//#define SIE2_PFPC_EN               0x00040000   ///< enable function: PFPC
//Not for SIE2//#define SIE2_GAMMA_EN              0x00080000   ///< enable function: Gamma
#define SIE2_ECS_EN                0x00100000   ///< enable function: Color shading compensation
#define SIE2_DGAIN_EN              0x00200000   ///< enable function: Digital Gain
#define SIE2_BS_H_EN               0x00400000   ///< enable function: Horizontal bayer scaling
//Not for SIE2//#define SIE2_BS_V_EN               0x00800000   ///< enable function: Vertical bayer scaling
#define SIE2_VIG_EN                0x01000000   ///< enable function: vignette shading compensation
#define SIE2_CA_EN                 0x02000000   ///< enable function: Color accumulation
//Not for SIE2//#define SIE2_VACC_EN               0x04000000   ///< enable function: Variation accumulation
//Not for SIE2//#define SIE2_FLD_ARR_EN            0x08000000   ///< enable function: Field arrangement
#define SIE2_HFLIP_EN              0x10000000   ///< enable function: Horizontal flip
#define SIE2_VFLIP_EN              0x20000000   ///< enable function: Vertical flip
#define SIE2_DRAM_OUT0_EN          0x40000000   ///< enable function: Dram output channel 0
//Not for SIE2//#define SIE2_DRAM_OUT2_EN          0x80000000   ///< enable function: Dram output channel 2
//@}


/*
    Define SIE2 interrput enable.
*/
//@{
//removed and would be masked-out//#define SIE2_INTE_EFLDEND              0x00000001   ///< enable interrupt: Even field end
//removed and would be masked-out//#define SIE2_INTE_OFLDEND              0x00000002   ///< enable interrupt: Odd field end
#define SIE2_INTE_FLDEND               0x00000004   ///< enable interrupt: Field end
//removed and would be masked-out//#define SIE2_INTE_LN16END              0x00000008   ///< enable interrupt: 16 lines end
#define SIE2_INTE_BP1                  0x00000010   ///< enable interrupt: Break point 1
#define SIE2_INTE_BP2                  0x00000020   ///< enable interrupt: Break point 2
//removed and would be masked-out//#define SIE2_INTE_BP3                  0x00000040   ///< enable interrupt: Break point 3
#define SIE2_INTE_ACTST                0x00000080   ///< enable interrupt: Active window start
//removed and would be masked-out//#define SIE2_INTE_CRPST                0x00000100   ///< enable interrupt: Crop window start
//Not for SIE2//#define SIE2_INTE_VDOFS_OVFL           0x00000200   ///< enable interrupt: VD offset overflow
//Not for SIE2//#define SIE2_INTE_FLSH_TRG             0x00000400   ///< enable interrupt: Flash trigger
//Not for SIE2//#define SIE2_INTE_MSH_TRG              0x00000800   ///< enable interrupt: Mshutter trigger
//Not for SIE2//#define SIE2_INTE_FLSH_END             0x00001000   ///< enable interrupt: Flash end
//Not for SIE2//#define SIE2_INTE_MSH_CLOSE_END        0x00002000   ///< enable interrupt: Mshutter close end
//Not for SIE2//#define SIE2_INTE_MSH_OPEN_END         0x00004000   ///< enable interrupt: Mshutter open end
//removed and would be masked-out//#define SIE2_INTE_DRAM_IN0_UDFL        0x00008000   ///< enable interrupt: Dram input channel 0 underflow
//removed and would be masked-out//#define SIE2_INTE_DRAM_IN1_UDFL        0x00010000   ///< enable interrupt: Dram input channel 1 underflow
//removed and would be masked-out//#define SIE2_INTE_DRAM_OUT0_OVFL       0x00020000   ///< enable interrupt: Dram output channel 0 overflow
//removed and would be masked-out//#define SIE2_INTE_DRAM_OUT1_OVFL       0x00040000   ///< enable interrupt: Dram output channel 1 overflow
//Not for SIE2//#define SIE2_INTE_DRAM_OUT2_OVFL       0x00080000   ///< enable interrupt: Dram output channel 2 overflow
#define SIE2_INTE_DRAM_OUT0_END        0x00100000   ///< enable interrupt: Dram output channel 0 end
//removed and would be masked-out//#define SIE2_INTE_DRAM_OUT1_END        0x00200000   ///< enable interrupt: Dram output channel 1 end
//Not for SIE2//#define SIE2_INTE_DRAM_OUT2_END        0x00400000   ///< enable interrupt: Dram output channel 2 end
//removed and would be masked-out//#define SIE2_INTE_DPCF                 0x00800000   ///< enable interrupt: Defect pixel concealment failure
//removed and would be masked-out//#define SIE2_INTE_DPCWARN              0x01000000   ///< enable interrupt: Defect Pixel/Column concealment failure
//Not for SIE2//#define SIE2_INTE_VAEND                0x02000000   ///< enable interrupt: VA output end
//Not for SIE2//#define SIE2_INTE_MSH_ERR              0x04000000   ///< enable interrupt: Mshutter open/close control conflict
//Not for SIE2//#define SIE2_INTE_RAWENC_VBFOVFL       0x08000000   ///< enable interrupt: Raw encoder virtual buffer overflow
//Not for SIE2//#define SIE2_INTE_RAWENC_OUTOVFL       0x10000000   ///< enable interrupt: Raw encoder output buffer full
#define SIE2_INTE_ALL                  0xffffffff   ///< enable interrupt: all
//@}

/*
    Define SIE2 interrput status.
*/
//@{
#define SIE2_INT_EFLDEND              0x00000001   ///< interrupt status: Even field end
#define SIE2_INT_OFLDEND              0x00000002   ///< interrupt status: Odd field end
#define SIE2_INT_FLDEND               0x00000004   ///< interrupt status: Field end
#define SIE2_INT_LN16END              0x00000008   ///< interrupt status: 16 lines end
#define SIE2_INT_BP1                  0x00000010   ///< interrupt status: Break point 1
#define SIE2_INT_BP2                  0x00000020   ///< interrupt status: Break point 2
#define SIE2_INT_BP3                  0x00000040   ///< interrupt status: Break point 3
#define SIE2_INT_ACTST                0x00000080   ///< interrupt status: Active window start
#define SIE2_INT_CRPST                0x00000100   ///< interrupt status: Crop window start
//Not for SIE2//#define SIE2_INT_VDOFS_OVFL           0x00000200   ///< interrupt status: VD offset overflow
//Not for SIE2//#define SIE2_INT_FLSH_TRG             0x00000400   ///< interrupt status: Flash trigger
//Not for SIE2//#define SIE2_INT_MSH_TRG              0x00000800   ///< interrupt status: Mshutter trigger
//Not for SIE2//#define SIE2_INT_FLSH_END             0x00001000   ///< interrupt status: Flash end
//Not for SIE2//#define SIE2_INT_MSH_CLOSE_END        0x00002000   ///< interrupt status: Mshutter close end
//Not for SIE2//#define SIE2_INT_MSH_OPEN_END         0x00004000   ///< interrupt status: Mshutter open end
#define SIE2_INT_DRAM_IN0_UDFL        0x00008000   ///< interrupt status: Dram input channel 0 underflow
#define SIE2_INT_DRAM_IN1_UDFL        0x00010000   ///< interrupt status: Dram input channel 1 underflow
#define SIE2_INT_DRAM_OUT0_OVFL       0x00020000   ///< interrupt status: Dram output channel 0 overflow
#define SIE2_INT_DRAM_OUT1_OVFL       0x00040000   ///< interrupt status: Dram output channel 1 overflow
//Not for SIE2//#define SIE2_INT_DRAM_OUT2_OVFL       0x00080000   ///< interrupt status: Dram output channel 2 overflow
#define SIE2_INT_DRAM_OUT0_END        0x00100000   ///< interrupt status: Dram output channel 0 end
#define SIE2_INT_DRAM_OUT1_END        0x00200000   ///< interrupt status: Dram output channel 1 end
//Not for SIE2//#define SIE2_INT_DRAM_OUT2_END        0x00400000   ///< interrupt status: Dram output channel 2 end
#define SIE2_INT_DPCF                 0x00800000   ///< interrupt status: Defect pixel concealment failure
#define SIE2_INT_DPCWARN              0x01000000   ///< interrupt status: Defect Pixel/Column concealment failure
//Not for SIE2//#define SIE2_INT_VAEND                0x02000000   ///< interrupt status: VA output end
//Not for SIE2//#define SIE2_INT_MSH_ERR              0x04000000   ///< interrupt status: Mshutter open/close control conflict
//Not for SIE2//#define SIE2_INT_RAWENC_VBFOVFL       0x08000000   ///< interrupt status: Raw encoder virtual buffer overflow
//Not for SIE2//#define SIE2_INT_RAWENC_OUTOVFL       0x10000000   ///< interrupt status: Raw encoder output buffer full
#define SIE2_INT_ALL                  0xffffffff   ///< interrupt status: all
//@}


//@}

/**
SIE2 Enumeration

@name   SIE2_Enumeration
*/
//@{

//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_ROUND_FIXED  = 0,   ///< fixed rounding
//Not for SIE2//    SIE2_ROUND_RANDOM ,   ///< random rounding
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_ROUND_SEL)
//Not for SIE2//} SIE2_ROUND_SEL;


//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_FLG_VD         = 0,  ///< frame sync
//Not for SIE2//    SIE2_FLG_BP1        ,  ///< break point 1
//Not for SIE2//    SIE2_FLG_BP2        ,  ///< break point 1
//Not for SIE2//    SIE2_FLG_FLSH       ,  ///< flash assertion end
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_FLAG_SEL)
//Not for SIE2//} SIE2_FLAG_SEL;



typedef enum
{
    SIE2_PHASE_RISING  = 0,  ///< rising edge latch/trigger
    SIE2_PHASE_FALLING ,  ///< falling edge latch/trigger
    ENUM_DUMMY4WORD(SIE2_PHASE)
} SIE2_PHASE;

//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_POL_HIGH_ACTIVE  = 0,  ///< high level active
//Not for SIE2//    SIE2_POL_LOW_ACTIVE   ,  ///< low level active
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_POLARITY)
//Not for SIE2//} SIE2_POLARITY;


//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_TRIGSEL_CURR_FRMEND   = 0,      ///< Trigger PRE at current frame end
//Not for SIE2//    SIE2_TRIGSEL_NEXT_FRMSYNC      ,       ///< Trigger PRE at next frame start
//Not for SIE2//    SIE2_TRIGSEL_NEXT_ACTST       ,        ///< Trigger PRE at next active-window start
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_PRETRIGSEL)
//Not for SIE2//} SIE2_PRETRIGSEL;


typedef enum
{
    SIE2_FEMODE_PARALLEL_SLAVE   = 0,      ///< SIE2 acts as slave for parallel-IF sensor
    SIE2_FEMODE_PARALLEL_MASTER  ,      ///< SIE2 acts as master for parallel-IF sensor
    SIE2_FEMODE_SERIAL           ,      ///< SIE2/HighSpeedIf acts together for Serial-IF sensor
    ENUM_DUMMY4WORD(SIE2_FRONTENDMODE)
} SIE2_FRONTENDMODE;


//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_HDIV_PROGRESSIVE   = 0,      ///< Horizontal Division number = 1
//Not for SIE2//    SIE2_HDIV_2_DIV         ,      ///< Horizontal Division number = 2
//Not for SIE2//    SIE2_HDIV_3_DIV         ,      ///< Horizontal Division number = 3
//Not for SIE2//    SIE2_HDIV_4_DIV         ,      ///< Horizontal Division number = 4
//Not for SIE2//    SIE2_HDIV_5_DIV         ,      ///< Horizontal Division number = 5
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_HDIV_NUM)
//Not for SIE2//} SIE2_HDIV_NUM;

//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_REO_1_LINE    = 0,      ///< Re-order line number = 1
//Not for SIE2//    SIE2_REO_2_LINE    ,      ///< Re-order line number = 2
//Not for SIE2//    SIE2_REO_3_LINE    ,      ///< Re-order line number = 3
//Not for SIE2//    SIE2_REO_4_LINE    ,      ///< Re-order line number = 4
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_REO_LNNUM)
//Not for SIE2//} SIE2_REO_LNNUM;

//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_FLDARR_PROGRESSIVE     = 0,      ///< Field arrangement field number = 1
//Not for SIE2//    SIE2_FLDARR_2_FLD           ,      ///< Field arrangement field number = 2
//Not for SIE2//    SIE2_FLDARR_3_FLD           ,      ///< Field arrangement field number = 3
//Not for SIE2//    SIE2_FLDARR_4_FLD           ,      ///< Field arrangement field number = 4
//Not for SIE2//    SIE2_FLDARR_5_FLD           ,      ///< Field arrangement field number = 5
//Not for SIE2//    SIE2_FLDARR_6_FLD           ,      ///< Field arrangement field number = 6
//Not for SIE2//    SIE2_FLDARR_7_FLD           ,      ///< Field arrangement field number = 7
//Not for SIE2//    SIE2_FLDARR_8_FLD           ,      ///< Field arrangement field number = 8
//Not for SIE2//    SIE2_FLDARR_9_FLD           ,      ///< Field arrangement field number = 9
//Not for SIE2//    SIE2_FLDARR_10_FLD          ,      ///< Field arrangement field number = 10
//Not for SIE2//    SIE2_FLDARR_11_FLD          ,      ///< Field arrangement field number = 11
//Not for SIE2//    SIE2_FLDARR_12_FLD          ,      ///< Field arrangement field number = 12
//Not for SIE2//    SIE2_FLDARR_13_FLD          ,      ///< Field arrangement field number = 13
//Not for SIE2//    SIE2_FLDARR_14_FLD          ,      ///< Field arrangement field number = 14
//Not for SIE2//    SIE2_FLDARR_15_FLD          ,      ///< Field arrangement field number = 15
//Not for SIE2//    SIE2_FLDARR_16_FLD          ,      ///< Field arrangement field number = 16
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_FLDARR_FLDNUM)
//Not for SIE2//} SIE2_FLDARR_FLDNUM;

typedef enum
{
    SIE2_PACKBUS_8  = 0, ///< 8 bits packing
    SIE2_PACKBUS_10 , ///< 10 bits packing
    SIE2_PACKBUS_12 , ///< 12 bits packing
    SIE2_PACKBUS_16 , ///< 16 bits packing
    ENUM_DUMMY4WORD(SIE2_PACKBUS)
} SIE2_PACKBUS;


typedef enum
{
    SIE2_BUF_NUM_1 = 0, ///< one buffer
    SIE2_BUF_NUM_2 ,///< two ping-pong buffer
    ENUM_DUMMY4WORD(SIE2_BUF_NUM)
} SIE2_BUF_NUM;


typedef enum
{
    SIE2_OUT0_SOURCE_CROP_IMG = 0, ///< SIE2 Dram output channel 0 source = cropped image
    SIE2_OUT0_SOURCE_BS_IMG   ,   ///< SIE2 Dram output channel 0 source = cropped and scaled image
    ENUM_DUMMY4WORD(SIE2_OUT0_SOURCE)
} SIE2_OUT0_SOURCE;


//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_OUT2_SOURCE_VACC   = 0,   ///< SIE2 Dram output channel 2 source = variation accumulation
//Not for SIE2//    SIE2_OUT2_SOURCE_BS_IMG , ///< SIE2 Dram output channel 2 source = cropped and scaled image
//Not for SIE2//    SIE2_OUT2_SOURCE_SMRLN  ,  ///< SIE2 Dram output channel 2 source = smear line
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_OUT2_SOURCE)
//Not for SIE2//} SIE2_OUT2_SOURCE;

typedef enum
{
    SIE2_DRAM_BURST_LENGTH_CH0_32WORD   = 0,   ///< SIE2 Dram burst length Ch0 32 words
    SIE2_DRAM_BURST_LENGTH_CH0_24WORD , ///< SIE2 Dram burst length Ch0 24 words
    SIE2_DRAM_BURST_LENGTH_CH0_16WORD , ///< SIE2 Dram burst length Ch0 16 words
//NotForSie2//    SIE2_DRAM_BURST_LENGTH_CH0_XXWORD , ///< SIE2 Dram burst length Ch0 XX words
//NotForSie2//    SIE2_DRAM_BURST_LENGTH_CH0_XXWORD , ///< SIE2 Dram burst length Ch0 XX words
//NotForSie2//    SIE2_DRAM_BURST_LENGTH_CH0_XXWORD , ///< SIE2 Dram burst length Ch0 XX words
//NotForSie2//    SIE2_DRAM_BURST_LENGTH_CH0_XXWORD , ///< SIE2 Dram burst length Ch0 XX words
    ENUM_DUMMY4WORD(SIE2_DRAM_BURST_LENGTH_CH0)
} SIE2_DRAM_BURST_LENGTH_CH0;

typedef enum
{
    SIE2_DRAM_BURST_LENGTH_CH1_32WORD   = 0,   ///< SIE2 Dram burst length Ch1 32 words
    SIE2_DRAM_BURST_LENGTH_CH1_24WORD , ///< SIE2 Dram burst length Ch1 24 words
    SIE2_DRAM_BURST_LENGTH_CH1_16WORD , ///< SIE2 Dram burst length Ch1 16 words
    ENUM_DUMMY4WORD(SIE2_DRAM_BURST_LENGTH_CH1)
} SIE2_DRAM_BURST_LENGTH_CH1;

//NotForSie2//typedef enum
//NotForSie2//{
//NotForSie2//    SIE2_DRAM_BURST_LENGTH_CH2_32WORD   = 0,   ///< SIE2 Dram burst length Ch2 32 words
//NotForSie2//    SIE2_DRAM_BURST_LENGTH_CH2_24WORD , ///< SIE2 Dram burst length Ch2 24 words
//NotForSie2//    SIE2_DRAM_BURST_LENGTH_CH2_16WORD , ///< SIE2 Dram burst length Ch2 16 words
//NotForSie2//    ENUM_DUMMY4WORD(SIE2_DRAM_BURST_LENGTH_CH2)
//NotForSie2//} SIE2_DRAM_BURST_LENGTH_CH2;

typedef enum
{
    SIE2_DRAM_BURST_LENGTH_CH3_16WORD   = 0,   ///< SIE2 Dram burst length Ch3 16 words
    SIE2_DRAM_BURST_LENGTH_CH3_24WORD , ///< SIE2 Dram burst length Ch3 24 words
    SIE2_DRAM_BURST_LENGTH_CH3_32WORD , ///< SIE2 Dram burst length Ch3 32 words
    ENUM_DUMMY4WORD(SIE2_DRAM_BURST_LENGTH_CH3)
} SIE2_DRAM_BURST_LENGTH_CH3;

typedef enum
{
    SIE2_DRAM_BURST_LENGTH_CH4_16WORD   = 0,   ///< SIE2 Dram burst length Ch4 16 words
    SIE2_DRAM_BURST_LENGTH_CH4_24WORD , ///< SIE2 Dram burst length Ch4 24 words
    SIE2_DRAM_BURST_LENGTH_CH4_32WORD , ///< SIE2 Dram burst length Ch4 32 words
    ENUM_DUMMY4WORD(SIE2_DRAM_BURST_LENGTH_CH4)
} SIE2_DRAM_BURST_LENGTH_CH4;

//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_FLSH_MODE_COUNT     = 0,    ///< flash mode: enable, count delay then trigger
//Not for SIE2//    SIE2_FLSH_MODE_GPIO      ,    ///< flash mode: enable, wait GPIO then trigger
//Not for SIE2//    SIE2_FLSH_MODE_IMD       ,    ///< flash mode: enable then trigger
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_FLSH_MODE)
//Not for SIE2//} SIE2_FLSH_MODE;

//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_LEVEL_MODE_LEVEL = 0, ///< level mode
//Not for SIE2//    SIE2_LEVEL_MODE_PULSE = 1, ///< pulse mode
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_LEVEL_MODE)
//Not for SIE2//} SIE2_LEVEL_MODE;


//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_MSH_MODE_COUNT     = 0,    ///< flash mode: enable, count delay then trigger
//Not for SIE2//    SIE2_MSH_MODE_GPIO      ,    ///< flash mode: enable, wait GPIO then trigger
//Not for SIE2//    SIE2_MSH_MODE_IMD       ,    ///< flash mode: enable then trigger
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_MSH_MODE)
//Not for SIE2//} SIE2_MSH_MODE;


//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_SPI_TRIG_SEL_BP3 = 0, ///< trigger SPI at BP3
//Not for SIE2//    SIE2_SPI_TRIG_SEL_VD  , ///< trigger SPI at VD
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_SPI_TRIG_SEL)
//Not for SIE2//} SIE2_SPI_TRIG_SEL;



typedef enum
{
    SIE2_BP_REF_ORI  = 0,   ///< break point reference: original VD, HD
    SIE2_BP_REF_OFS  ,   ///< break point reference: offset VD, HD
    ENUM_DUMMY4WORD(SIE2_BP_REF)
} SIE2_BP_REF;


typedef enum
{
    SIE2_DVI_FORMAT_CCIR601  = 0,   ///< CCIR 601
    SIE2_DVI_FORMAT_CCIR656  ,   ///< CCIR 656
    ENUM_DUMMY4WORD(SIE2_DVI_FORMAT)
} SIE2_DVI_FORMAT;

typedef enum
{
    SIE2_DVI_MODE_SD  = 0,   ///< SD mode
    SIE2_DVI_MODE_HD  ,   ///< HD mode
    ENUM_DUMMY4WORD(SIE2_DVI_MODE)
} SIE2_DVI_MODE;

typedef enum
{
    SIE2_CCIR656_ACT_SEL_EAV  = 0,   ///< EAV code recognition
    SIE2_CCIR656_ACT_SEL_ACT  ,   ///< Active window size setting
    ENUM_DUMMY4WORD(SIE2_CCIR656_ACT_SEL)
} SIE2_CCIR656_ACT_SEL;


typedef enum
{
    SIE2_PAT_COLORBAR    = 1,   ///< Color bar
    SIE2_PAT_RANDOM         ,     ///< Random
    SIE2_PAT_FIXED          ,      ///< Fixed
    SIE2_PAT_HINCREASE      ,  ///< 1D Increment
    SIE2_PAT_HVINCREASE     , ///< 2D increment
    ENUM_DUMMY4WORD(SIE2_PATGEN_MODE)
} SIE2_PATGEN_MODE;


typedef enum
{
    SIE2_DEF_FACT_50  = 0,   ///< defect concealment cross-channel weighting = 50%
    SIE2_DEF_FACT_25  ,   ///< defect concealment cross-channel weighting = 25%
    SIE2_DEF_FACT_0   ,   ///< defect concealment cross-channel weighting =  0%
    ENUM_DUMMY4WORD(SIE2_DEF_FACT)
} SIE2_DEF_FACT;


//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_SMR_CFSIZE_15  = 0,   ///< curve filter size = 15
//Not for SIE2//    SIE2_SMR_CFSIZE_13  ,   ///< curve filter size = 13
//Not for SIE2//    SIE2_SMR_CFSIZE_11  ,   ///< curve filter size = 11
//Not for SIE2//    SIE2_SMR_CFSIZE_9   ,   ///< curve filter size = 9
//Not for SIE2//    SIE2_SMR_CFSIZE_7   ,  ///< curve filter size = 7
//Not for SIE2//    SIE2_SMR_CFSIZE_5   ,   ///< curve filter size = 5
//Not for SIE2//    SIE2_SMR_CFSIZE_3   ,   ///< curve filter size = 3
//Not for SIE2//    SIE2_SMR_CFSIZE_1   ,   ///< curve filter size = 1 (no filtering)
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_SMR_CFSIZE)
//Not for SIE2//} SIE2_SMR_CFSIZE;



//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_SMR_TRANS_WIDTH_0  = 0,   ///< no transition
//Not for SIE2//    SIE2_SMR_TRANS_WIDTH_2  ,   ///< transition width 2 pixel
//Not for SIE2//    SIE2_SMR_TRANS_WIDTH_4  ,   ///< transition width 4 pixel
//Not for SIE2//    SIE2_SMR_TRANS_WIDTH_8  ,   ///< transition width 8 pixel
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_SMR_TRANS_WIDTH)
//Not for SIE2//} SIE2_SMR_TRANS_WIDTH;


//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_SMR_NODEINT_1  = 0,   ///< interval = 1 (no down-sample)
//Not for SIE2//    SIE2_SMR_NODEINT_2  ,   ///< interval = 2 (1/2 down-sample)
//Not for SIE2//    SIE2_SMR_NODEINT_4  ,   ///< interval = 4 (1/4 down-sample)
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_SMR_NODEINT)
//Not for SIE2//} SIE2_SMR_NODEINT;


//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_PFPC_MODE_4X8  = 0,   ///< 4x8 period
//Not for SIE2//    SIE2_PFPC_MODE_8X4  ,   ///< 8x4 period
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_PFPC_MODE)
//Not for SIE2//} SIE2_PFPC_MODE;



typedef enum
{
    SIE2_ECS_MAPSIZE_129X129  = 0,   ///< 129x129
    SIE2_ECS_MAPSIZE_65X65    ,   ///< 65x65
    SIE2_ECS_MAPSIZE_49X49    ,   ///< 49x49
    ENUM_DUMMY4WORD(SIE2_ECS_MAPSIZE)
} SIE2_ECS_MAPSIZE;


typedef enum
{
    SIE2_CA_OUT_1_WORD  = 0,   ///< one word per window; [10:0] R, [20:11] G, [31:21] B
    SIE2_CA_OUT_2_WORD  ,   ///< two words per window; [15:0] R, [31:16] G, [15:0] B
    ENUM_DUMMY4WORD(SIE2_CA_OUT_MODE)
} SIE2_CA_OUT_MODE;


//Not for SIE2//typedef enum
//Not for SIE2//{
//Not for SIE2//    SIE2_VA_FLTR_5_TAP  = 0,   ///< filter [-1 0 2 0 -1]
//Not for SIE2//    SIE2_VA_FLTR_7_TAP  ,   ///< filter [-1 0 0 0 2 0 0 0 -1]
//Not for SIE2//    ENUM_DUMMY4WORD(SIE2_VA_FLTR)
//Not for SIE2//} SIE2_VA_FLTR;

typedef enum
{
    SIE2_CFA_R = 0, ///< start pixel is R
    SIE2_CFA_Gr = 1, ///< start pixel is GR
    SIE2_CFA_Gb = 2, ///< start pixel is GB
    SIE2_CFA_B = 3, ///< start pixel is B
    ENUM_DUMMY4WORD(SIE2_CFAPAT)
} SIE2_CFAPAT;



/**
    SIE2 Pixel-Clock Source Selection

    Structure of Pixel-Clock source SIE2 configuration.
*/
//@{
typedef enum
{
    SIE2_PXCLKSRC_OFF = 0, ///< SIE2 pixel-clock disabled
    SIE2_PXCLKSRC_PAD    , ///< SIE2 pixel-clock enabled, source as pixel-clock-pad
    SIE2_PXCLKSRC_MCLK   , ///< SIE2 pixel-clock enabled, source as MCLK2
} SIE2_PXCLKSRC;
//@}

/**
    SIE2 Pixel-Clock Source Selection

    Structure of Pixel-Clock source SIE2 configuration.
*/
//@{
typedef enum
{
    SIE2_CLKSRC_CURR = 0, ///< SIE2 clock source as current setting(no setting, no changing)
    SIE2_CLKSRC_480     , ///< SIE2 clock source as 480MHz
    SIE2_CLKSRC_PLL5    , ///< SIE2 clock source as PLL5
    SIE2_CLKSRC_PLL6    , ///< SIE2 clock source as PLL6
    SIE2_CLKSRC_PLL7    , ///< SIE2 clock source as PLL7
} SIE2_CLKSRC;
//@}



/**
    @name SIE2 enum - Change Funciton Parameter Selection
*/
//@{
typedef enum
{
    SIE2_CHG_SIZE            =  0,  ///< change input and output image size, refer to structure "SIE2_CHG_SIZE_INFO"
    SIE2_CHG_IO_ADDR         ,  ///< change input and output DMA Address, refer to structure "SIE2_CHG_IO_ADDR_INFO"
    SIE2_CHG_OUT_SRC         ,  ///< change output source, refer to structure "SIE2_CHG_OUT_SRC_INFO"
    //Not for SIE2//SIE2_CHG_REORDER         ,  ///< change re-order parameters, refer to "SIE2_REORDER"
    SIE2_CHG_DP              ,  ///< change defect pixel parameters, refer to "SIE2_DEF_PXL"
    SIE2_CHG_DCOL            ,  ///< change defect column parameters, refer to "SIE2_DEF_COL"
    SIE2_CHG_OB_OFS          ,  ///< change OB (Offset/Detect) parameters, refer to "SIE2_OB_SUB"
    //Not for SIE2//SIE2_CHG_SMEAR           ,  ///< change smear detection/reduction parameters, refer to "SIE2_SMEAR"
    SIE2_CHG_ECS             ,  ///< change color shading compensation parameters, refer to "SIE2_ECS"
    SIE2_CHG_DGAIN           ,  ///< change digital gain parameters, refer to "SIE2_DGAIN"
    SIE2_CHG_BS_H            ,  ///< change horizontal bayer scaling parameters, refer to "SIE2_BS_H"
    //Not for SIE2//SIE2_CHG_BS_V            ,  ///< change vertical bayer scaling parameters, refer to "SIE2_BS_V"
    //Not for SIE2//SIE2_CHG_FLD_ARR         ,  ///< change field arrangement parameters, refer to "SIE2_FLDARR"
    SIE2_CHG_VIG             ,  ///< change vignette shading compensation parameters, refer to "SIE2_VIG_INFO"
    SIE2_CHG_CA              ,  ///< change color accumulation parameters, refer to "SIE2_CA"
    //Not for SIE2//SIE2_CHG_VA              ,  ///< change variation accumulation parameters, refer to "SIE2_VA"
    //Not for SIE2//SIE2_CHG_MSH_PARAM       ,  ///< change Mshutter parameters, refer to "SIE2_MSHTR_PARAM"
    //Not for SIE2//SIE2_CHG_MSH_CLOSE       ,  ///< trigger Mshutter close operation
    //Not for SIE2//SIE2_CHG_MSH_OPEN        ,  ///< trigger Mshutter open operation
    //Not for SIE2//SIE2_CHG_FLSH_PARAM      ,  ///< change Flashlight parameters, refer to "SIE2_FLASH_PARAM"
    //Not for SIE2//SIE2_CHG_FLSH_START      ,  ///< trigger Flashlight flash operation
    //Not for SIE2//SIE2_CHG_RAW_ENC         ,  ///< change Raw Encoder parameters, refer to "SIE2_RAW_ENC_ADJ"
    SIE2_CHG_OB_DT           ,  ///< change OB Detection parameters, refer to "SIE2_OBDT"
    //Not for SIE2//SIE2_CHG_OB_FRAME        ,  ///< change OB Frame detection parameters, refer to "SIE2_OBF"
    //Not for SIE2//SIE2_CHG_OB_PLANE        ,  ///< change OB Plane subtraction parameters, refer to "SIE2_OBP"
    //Not for SIE2//SIE2_CHG_PFPC            ,  ///< change PFPC parameters, refer to "SIE2_PFPC"
    //Not for SIE2//SIE2_CHG_GAMMA           ,  ///< change Gamma parameters, refer to "SIE2_GAMMA_TBL"
    SIE2_CHG_FLIP            ,  ///< change H/V Flip parameters, refer to "SIE2_FLIP"
    //Not for SIE2//SIE2_CHG_SPI_TRIG        ,  ///< change SPI Trigger parameters, refer to "SIE2_SPI_TRIG_SEL"
    SIE2_CHG_DRAM_BURST      ,  ///< change dram burst length
    ENUM_DUMMY4WORD(SIE2_CHANGE_FUN_PARAM_SEL)
} SIE2_CHANGE_FUN_PARAM_SEL;
//@}

//@}

/**
SIE2 Struct

@name   SIE2_Struct
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
} SIE2_HDVD;

//@}

/**
SIE2 Struct

@name   SIE2_Struct
*/
//@{

typedef struct
{
    UINT16    uiStX1; ///< horizontal active start of the 1st division.
//Not for SIE2//    UINT16    uiStX2; ///< horizontal active start of the 2nd division.
//Not for SIE2//    UINT16    uiStX3; ///< horizontal active start of the 3rd division.
//Not for SIE2//    UINT16    uiStX4; ///< horizontal active start of the 4th division.
//Not for SIE2//    UINT16    uiStX5; ///< horizontal active start of the 5th division.
    UINT16    uiStY1; ///< vertical active start of the 1st field.
//Not for SIE2//    UINT16    uiStY2; ///< vertical active start of the 2nd field.
//Not for SIE2//    UINT16    uiStY3; ///< vertical active start of the 3rd field.
//Not for SIE2//    UINT16    uiStY4; ///< vertical active start of the 4th field.
//Not for SIE2//    UINT16    uiStY5; ///< vertical active start of the 5th field.
//Not for SIE2//    UINT16    uiStY6; ///< vertical active start of the 6th field.
//Not for SIE2//    UINT16    uiStY7; ///< vertical active start of the 7th field.
//Not for SIE2//    UINT16    uiStY8; ///< vertical active start of the 8th field.
//Not for SIE2//    UINT16    uiStY9; ///< vertical active start of the 9th field.
//Not for SIE2//    UINT16    uiStY10; ///< vertical active start of the 10th field.
//Not for SIE2//    UINT16    uiStY11; ///< vertical active start of the 11th field.
//Not for SIE2//    UINT16    uiStY12; ///< vertical active start of the 12th field.
//Not for SIE2//    UINT16    uiStY13; ///< vertical active start of the 13th field.
//Not for SIE2//    UINT16    uiStY14; ///< vertical active start of the 14th field.
//Not for SIE2//    UINT16    uiStY15; ///< vertical active start of the 15th field.
//Not for SIE2//    UINT16    uiStY16; ///< vertical active start of the 16th field.
    UINT32    uiSzX; ///< horizontal size
    UINT32    uiSzY; ///< vertical size
    SIE2_CFAPAT    CfaPat;///< CFA start pattern
} SIE2_ACTWIN;


typedef struct
{
    UINT16    uiStX; ///< horizontal crop start
    UINT16    uiStY; ///< vertical crop start
    UINT32    uiSzX; ///< horizontal size
    UINT32    uiSzY; ///< vertical size
    SIE2_CFAPAT    CfaPat;///< CFA start pattern
} SIE2_CRPWIN;

//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    SIE2_HDIV_NUM    uiDivNum; ///< horizontal division number
//Not for SIE2//} SIE2_HDIV;


//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    BOOL              bReoHEn;   ///< enable for h
//Not for SIE2//    BOOL              bReoVEn;   ///< enable for v
//Not for SIE2//    SIE2_REO_LNNUM    ReoLineNum; ///< line number
//Not for SIE2//    UINT32            uiStX;     ///< start pixel
//Not for SIE2//    UINT8             uiReoIndex[8]; ///< re-order indexing
//Not for SIE2//} SIE2_REORDER;

typedef struct
{
    BOOL              bDpcEn;   ///< enable for defect pixel concealment
    UINT32            uiDpTblAddr;///< dp table address
    UINT32            uiDpTblSize;///< dp table Size, in bytes
} SIE2_DEF_PXL;

//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    BOOL                    bFldArrEn;     ///< enable
//Not for SIE2//    SIE2_FLDARR_FLDNUM      FldNum; ///< total number of fields. 0: 1 field; 1: 2 fields, ..., 15: 16 fields.
//Not for SIE2//    UINT8                   uiFldIndex[16][2]; ///< field num of the M(out of 16) input field, Line N(out of 2); value = 1, 2, 3, 4...
//Not for SIE2//} SIE2_FLDARR;

//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    SIE2_FLSH_MODE FlshMode;
//Not for SIE2//    SIE2_PHASE FlshInPhase;
//Not for SIE2//    SIE2_POLARITY FlshOutPol;
//Not for SIE2//    UINT32 uiLineDelay;
//Not for SIE2//    UINT32 uiPx2Delay;
//Not for SIE2//    UINT32 uiFlashWidth;
//Not for SIE2//} SIE2_FLASH_PARAM;


typedef struct
{
    SIE2_PHASE VdPhase; ///< VD phase
    SIE2_PHASE HdPhase; ///< HD phase
    SIE2_PHASE DataPhase; ///< Data phase
} SIE2_SENSOR_PIN_PHASE;

//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    SIE2_LEVEL_MODE MshLvlMode;
//Not for SIE2//    BOOL bPinSwap;
//Not for SIE2//    SIE2_POLARITY MshOutPol;
//Not for SIE2//    SIE2_PHASE MshInPhase;
//Not for SIE2//    SIE2_MSH_MODE  MshCloseMode;
//Not for SIE2//    UINT32 uiPulseWidth;
//Not for SIE2//    UINT32 uiCloseFrameDelay;
//Not for SIE2//    UINT32 uiCloseLineDelay;
//Not for SIE2//    UINT32 uiClosePx2Delay;
//Not for SIE2//} SIE2_MSHTR_PARAM;


typedef struct
{
    UINT32 uiBp1;             ///< break point 1
    UINT32 uiBp2;             ///< break point 2
    UINT32 uiBp3;             ///< break point 3
    SIE2_BP_REF BpRefBp1;    ///< break point 1 reference
    SIE2_BP_REF BpRefBp2;    ///< break point 2 reference
    SIE2_BP_REF BpRefBp3;    ///< break point 3 reference
} SIE2_BREAKPOINT;

typedef struct
{
    SIE2_DVI_FORMAT         DviFormat;             ///< format: 601 or 656
    SIE2_DVI_MODE           DviMode;               ///< mode: SD or HD
    SIE2_CCIR656_ACT_SEL    Ccir656ActSel;      ///< CCIR656 Active size selection
    BOOL                    bFieldEn;           ///< switch: start input from field with internal field signal=0
    BOOL                    bFieldInv;          ///< switch: inverse the polarity of internal field signal
    BOOL                    bCcir656InSwap;     ///< input Y/C channel pin swap for CCIR656
    BOOL                    bCcir656OutSwap;    ///< output Y/C channel pin swap for CCIR656
} SIE2_DVI;


typedef struct
{
    BOOL        bDefColEn;      ///< enable
    UINT32      uiColNum;         ///< defect column number
    UINT16      uiDefColIndex[16];  ///< defect column index array
} SIE2_DEF_COL;

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
} SIE2_OBDT;


typedef struct
{
    UINT16      uiObOfs;
    UINT16      uiObGain;
} SIE2_OB_SUB;


//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    BOOL        bObfEn;      ///< enable
//Not for SIE2//    UINT32      uiTopX;       ///< top windows horizontal position (also for bottom windows)
//Not for SIE2//    UINT32      uiTopY;       ///< top windows vertical position
//Not for SIE2//    UINT32      uiTopIntvl;   ///< top windows horizontal interval (also for bottom windows)
//Not for SIE2//    UINT32      uiThresold;      ///< threshold for all windows
//Not for SIE2//    UINT32      uiNormFctr;   ///< normalization factor
//Not for SIE2//    UINT32      uiLeftX;      ///< left windows horizontal position
//Not for SIE2//    UINT32      uiLeftY;      ///< left windows vertical position (also for right windows)
//Not for SIE2//    UINT32      uiLeftIntvl;  ///< left windows horizontal interval (also for right windows)
//Not for SIE2//    UINT32      uiBotY;       ///< bottom windows vertical position
//Not for SIE2//    UINT32      uiRightX;     ///< right windows horizontal position
//Not for SIE2//} SIE2_OBF;


//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    UINT8       uiTopRslt[8];      ///< top window result
//Not for SIE2//    UINT8       uiLeftRslt[8];     ///< left window result
//Not for SIE2//    UINT8       uiBotRslt[8];      ///< bottom window result
//Not for SIE2//    UINT8       uiRightRslt[8];    ///< right window result
//Not for SIE2//} SIE2_OBF_RSLT;


//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    BOOL        bObpEn;      ///< enable
//Not for SIE2//    UINT32      uiHSclFctr;       ///< horizontal scaling factor
//Not for SIE2//    UINT32      uiVSclFctr;       ///< vertical scaling factor
//Not for SIE2//    UINT32      uiShift;      ///< left shift bit number
//Not for SIE2//    UINT8       *uiObpTbl;       ///< table (size=17x17=289)
//Not for SIE2//} SIE2_OBP;

//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    BOOL            bSmrEn;          ///< enable
//Not for SIE2//    UINT32          uiTfMoveStp1;     ///< temporal filter move step 1
//Not for SIE2//    UINT32          uiTfMoveStp2;     ///< temporal filter move step 2
//Not for SIE2//    UINT32          uiSmrY;           ///< smear line vetical position
//Not for SIE2//    UINT32          uiCfOutMax;       ///< curve filter output max
//Not for SIE2//    UINT32          uiSmrLnSel;       ///< smear line selection for multi-line mode
//Not for SIE2//
//Not for SIE2//    SIE2_SMR_CFSIZE CfSize;         ///< curve filter size
//Not for SIE2//    UINT32      uiCfTh1;     ///< curve filter threshold 1
//Not for SIE2//    UINT32      uiCfTh2;     ///< curve filter threshold 2
//Not for SIE2//    UINT32      uiOutlTh;     ///< outlier removal threshold
//Not for SIE2//    UINT32      uiStaLevel;     ///< static level of optical black
//Not for SIE2//    UINT32      uiStaLvlTh1;     ///< upper bound 1 of static level
//Not for SIE2//    UINT32      uiStaLvlTh2;     ///< upper bound 2 of static level
//Not for SIE2//
//Not for SIE2//    UINT32      uiSzThIn;         ///< safe zone in threshold
//Not for SIE2//    UINT32      uiSzThOut;     ///< safe zone out threshold
//Not for SIE2//    SIE2_SMR_TRANS_WIDTH      TransWidth;     ///< transition width
//Not for SIE2//    SIE2_SMR_NODEINT          NodeInt;     ///< node interval (related to down sample rate)
//Not for SIE2//    UINT32      uiTfTh1;     ///< temporal filter threshold 1
//Not for SIE2//    UINT32      uiTfTh2;     ///< temporal filter threshold 2
//Not for SIE2//    BOOL        bInputSmrLn;     ///< Input smear line from DRAM
//Not for SIE2//    UINT32      uiReduWeight;     ///< smear reduction weight (0x80 as 100%)
//Not for SIE2//    UINT32      uiReduMaxSrcVal;     ///< source value over this threshold remains
//Not for SIE2//} SIE2_SMEAR;


//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    BOOL            bPfpcEn;     ///< enable
//Not for SIE2//    SIE2_PFPC_MODE  PfpcMode;       ///< period mode
//Not for SIE2//    UINT8           uiGainTbl[32];   ///< gain table
//Not for SIE2//    UINT8           uiOfsTbl[32];    ///< offset table
//Not for SIE2//} SIE2_PFPC;


//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    BOOL            bGammaEn;    ///< enable
//Not for SIE2//    BOOL            bGammaUpdate;///< Update Gamma LUT or not
//Not for SIE2//    UINT16          *uiGammaTblR;   ///< R table
//Not for SIE2//    UINT16          *uiGammaTblGr;  ///< Gr table
//Not for SIE2//    UINT16          *uiGammaTblGb;  ///< Gb table
//Not for SIE2//    UINT16          *uiGammaTblB;   ///< B table
//Not for SIE2//} SIE2_GAMMA_TBL;




typedef struct
{
    BOOL                bEcsEn;      ///< enable
    SIE2_ECS_MAPSIZE    MapSizeSel; ///< vig compensation - map size
    UINT32              uiMapMulShift;///< vig compensation - multiplication shift number
    BOOL                bDthrEn;     ///< vig compensation - vignette dithering enable
    BOOL                bDthrRst;    ///< vig compensation - vignette dithering reset
    UINT32              uiDthrLvl;    ///< vig compensation - vignette dithering level; 0x0=bit[1:0], 0x7=bit[8:7]
    UINT32              uiHSclFctr;       ///< vig compensation - map horizontal scaling factor
    UINT32              uiVSclFctr;       ///< vig compensation - map vertical scaling factor
} SIE2_ECS;

typedef struct
{
    BOOL                bDGainEn;           ///< enable
    UINT32              uiGainIn3P7Bit;   ///< digital gain in 3.7 bits format
} SIE2_DGAIN;



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
} SIE2_BS_H;


//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    BOOL            bBsVEn;  ///< enable
//Not for SIE2//    UINT32          uiIv;     ///< bayer scaling - integration value
//Not for SIE2//    UINT32          uiSv;     ///< bayer scaling - skipping value
//Not for SIE2//    UINT32          uiBvR;    ///< bayer scaling - begin value for R/Gb channel
//Not for SIE2//    UINT32          uiBvB;    ///< bayer scaling - begin value for B/Gr channel
//Not for SIE2//    UINT32          uiOutSize;///< bayer scaling - output size
//Not for SIE2//    UINT32          uiDivM;   ///< bayer scaling - division parameter M
//Not for SIE2//    UINT32          uiDivS;   ///< bayer scaling - division parameter S
//Not for SIE2//    UINT32          uiInShft; ///< bayer scaling - input shift
//Not for SIE2//    UINT32          uiDivMRb[16];///< bayer scaling - division parameter M for R/B
//Not for SIE2//    UINT32          uiDivSRb[16];///< bayer scaling - division parameter S for R/B
//Not for SIE2//} SIE2_BS_V;


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
} SIE2_VIG_INFO;

typedef struct
{
    UINT32              uiImgXSize;     ///< vig compensation - image horizontal size
    UINT32              uiImgYSize;     ///< vig compensation - image vertical size
    UINT32              uiImgXCntr;     ///< vig compensation - image horizontal center
    UINT32              uiImgYCntr;     ///< vig compensation - image vertical center
    UINT32              uiImgXratio;    ///< vig compensation - image horizontalratio (16 from 16:9)
    UINT32              uiImgYratio;    ///< vig compensation - image vertical ratio (9 from 16:9)
} SIE2_VIG_SETTING_ONE_IMG;

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
} SIE2_VIG_SETTING_TWO_IMG;


//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    BOOL                bVaEn;       ///< enable
//Not for SIE2//    UINT32              uiOutShift;   ///< variation accumulation - output shift
//Not for SIE2//    UINT32              uiStX;        ///< variation accumulation - horizontal window start position
//Not for SIE2//    UINT32              uiStY;        ///< variation accumulation - vertical window start position
//Not for SIE2//    UINT32              uiSzX;        ///< variation accumulation - window horizontal size
//Not for SIE2//    UINT32              uiSzY;        ///< variation accumulation - window vertical size
//Not for SIE2//    UINT32              uiDivX;       ///< variation accumulation - horizontal subsample
//Not for SIE2//    UINT32              uiDivY;       ///< variation accumulation - vertical subsample
//Not for SIE2//    UINT32              uiWinNumX;    ///< variation accumulation - horizontal window number
//Not for SIE2//    UINT32              uiWinNumY;    ///< variation accumulation - vertical window number
//Not for SIE2//    UINT32              uiVaTh;  ///< variation accumulation - variation accumulation threshold
//Not for SIE2//    SIE2_VA_FLTR        VaFltrSel;    ///< variation accumulation - variation filter selection
//Not for SIE2//} SIE2_VA;

//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    UINT32              uiGroupStx;   ///< variation accumulation setting - horizontal window-group start position
//Not for SIE2//    UINT32              uiGroupSty;   ///< variation accumulation setting - vertical window-group start position
//Not for SIE2//    UINT32              uiGroupSzx;   ///< variation accumulation setting - window-group horizontal size
//Not for SIE2//    UINT32              uiGroupSzy;   ///< variation accumulation setting - window-group vertical size
//Not for SIE2//    UINT32              uiWinNumX;    ///< variation accumulation setting - horizontal window number
//Not for SIE2//    UINT32              uiWinNumY;    ///< variation accumulation setting - vertical window number
//Not for SIE2//    UINT32              uiVaTh;  ///< variation accumulation - variation accumulation threshold
//Not for SIE2//    SIE2_VA_FLTR        VaFltrSel;    ///< variation accumulation - variation filter selection
//Not for SIE2//} SIE2_VA_SETTING;

typedef struct
{
    BOOL                bCaEn;       ///< enable
    SIE2_CA_OUT_MODE    CaOutMode;    ///< color accumulation - output mode
    UINT32              uiOutShift;   ///< color accumulation - output shift
    UINT32              uiStX;        ///< color accumulation - horizontal window start position
    UINT32              uiStY;        ///< color accumulation - vertical window start position
    UINT32              uiSzX;        ///< color accumulation - window horizontal size
    UINT32              uiSzY;        ///< color accumulation - window vertical size
    UINT32              uiDivX;       ///< color accumulation - horizontal subsample
    UINT32              uiDivY;       ///< color accumulation - vertical subsample
    UINT32              uiWinNumX;    ///< color accumulation - horizontal window number
    UINT32              uiWinNumY;    ///< color accumulation - vertical window number
} SIE2_CA;

typedef struct
{
    SIE2_CA_OUT_MODE    CaOutMode;    ///< color accumulation setting - output mode
    UINT32              uiGroupStx;   ///< color accumulation setting - horizontal window-group start position
    UINT32              uiGroupSty;   ///< color accumulation setting - vertical window-group start position
    UINT32              uiGroupSzx;   ///< color accumulation setting - window-group horizontal size
    UINT32              uiGroupSzy;   ///< color accumulation setting - window-group vertical size
    UINT32              uiWinNumX;    ///< color accumulation setting - horizontal window number
    UINT32              uiWinNumY;    ///< color accumulation setting - vertical window number
} SIE2_CA_SETTING;


typedef struct
{
    BOOL                bHFlipEn;        ///< enable H flip
    BOOL                bVFlipEn;        ///< enable V flip
} SIE2_FLIP;

//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    BOOL                bSpiTrgEn;   ///< enable
//Not for SIE2//    SIE2_SPI_TRIG_SEL       SpiTrgSel;     ///< SPI Trigger Timing
//Not for SIE2//} SIE2_SPI_TRIG;

//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    BOOL              bInRound;          ///< Raw encoder - input truncation or rounding
//Not for SIE2//    SIE2_ROUND_SEL    EncRoundSel;      ///< Raw encoder - encode rounding selection
//Not for SIE2//    UINT32            uiEncRoundRI;       ///< Raw encoder - encode random rounding initial value
//Not for SIE2//    UINT32            uiBsMaxSize;        ///< Raw encoder - maximum encoded output bit stream size in byte
//Not for SIE2//    UINT32            uiBrcInitOffPrd;    ///< Raw encoder - BRC initial disabled line number
//Not for SIE2//    UINT32            uiBrcRefBase;       ///< Raw encoder - BRC refresh interval base
//Not for SIE2//    UINT32            uiBrcRefCnt;        ///< Raw encoder - BRC refresh interval count
//Not for SIE2//    UINT32            uiBrcLvlInit;       ///< Raw encoder - BRC initial level setting
//Not for SIE2//    UINT32            uiBrcQInit;         ///< Raw encoder - BRC initial Q setting
//Not for SIE2//    UINT32            uiBrcSegBitNo;      ///< Raw encoder - BRC target encoded bit number per segment
//Not for SIE2//    UINT32            uiVbfSizeTol;       ///< Raw encoder - Virtual buffer toerance for quality factor update
//Not for SIE2//    UINT32            uiVbfSizeMax;       ///< Raw encoder - Maximum virtual buffer size for overflow and underflow
//Not for SIE2//} SIE2_RAW_ENC_PARAM;

//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    BOOL              bRawEncEn;         ///< enable
//Not for SIE2//    UINT32            uiCompRate;         ///< Raw encoder - compression rate, in %, range 40~60%
//Not for SIE2//    UINT32            uiBsMaxSize;        ///< Raw encoder - maximum encoded output bit stream size in byte
//Not for SIE2//} SIE2_RAW_ENC_ADJ;

//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    UINT8             uiQTbl[8][8];       ///< Raw encoder - Q table [BRC][LFN]
//Not for SIE2//    UINT8             uiQMax[8];          ///< Raw encoder - Max Q table [LFN]
//Not for SIE2//    UINT8             uiQMin[8];          ///< Raw encoder - Min Q table [LFN]
//Not for SIE2//} SIE2_RAW_ENC_TBL;

//Not for SIE2//typedef struct
//Not for SIE2//{
//Not for SIE2//    UINT32            uiVbfOvflCnt;       ///< Raw encoder - accumulated bit count exceed maximum virtual buffer size count
//Not for SIE2//    UINT32            uiVbfUdflCnt;       ///< Raw encoder - accumulated bit count exceed minimum virtual buffer size count
//Not for SIE2//    UINT32            uiOutputSize;       ///< Raw encoder - accumulated output byte count
//Not for SIE2//    UINT16            uiLfnLnCnt[8];        ///< Raw encoder - line count for max. LFN 0~7 per line
//Not for SIE2//} SIE2_RAW_ENC_STATUS;



/**
    SIE2 Partial Configuration for parameter change of IO Size

    Structure of IO Size SIE2 configuration.
*/
//@{
typedef struct
{
    SIE2_CRPWIN               CrpInfo; ///< crop window info
    UINT32                   uiOut0Lofs;  ///< SIE2 output ch0 lineoffset
//Not for SIE2//    UINT32                   uiOut2Lofs;  ///< SIE2 output ch2 lineoffset
} SIE2_CHG_SIZE_INFO;
//@}

/**
    SIE2 Partial Configuration for parameter change of IO Size

    Structure of IO Size SIE2 configuration.
*/
//@{
typedef struct
{
    UINT32                   uiIn0Addr; ///< SIE2 input ch0 dram addr
    UINT32                   uiIn1Addr; ///< SIE2 input ch1 dram addr
    UINT32                   uiOut0Add0; ///< SIE2 output ch0 dram addr 0
    UINT32                   uiOut0Add1; ///< SIE2 output ch0 dram addr 1
    UINT32                   uiOut1Add0; ///< SIE2 output ch1 dram addr 0
    UINT32                   uiOut1Add1; ///< SIE2 output ch1 dram addr 1
    //Nor for SIE2//UINT32                   uiOut2Add0; ///< SIE2 output ch2 dram addr 0
    //Nor for SIE2//UINT32                   uiOut2Add1; ///< SIE2 output ch2 dram addr 1
    UINT32                   uiIn0Size; ///< SIE2 input ch0 dram size, in bytes
    UINT32                   uiIn1Size; ///< SIE2 input ch1 dram size, in bytes
    UINT32                   uiOut0Size; ///< SIE2 output ch0 dram size, in bytes
    UINT32                   uiOut1Size; ///< SIE2 output ch1 dram size, in bytes
    //Not for SIE2//UINT32                   uiOut2Size; ///< SIE2 output ch2 dram size, in bytes
} SIE2_CHG_IO_ADDR_INFO;
//@}

/**
    SIE2 Partial Configuration for parameter change of Dram Burst Length

    Structure of IO Size SIE configuration.
*/
//@{
typedef struct
{
    SIE2_DRAM_BURST_LENGTH_CH0   DramBstLengCh0; ///< SIE2 Dram Burst Length output ch1
} SIE2_CHG_DRAM_BURST_INFO;
//@}

/**
    SIE2 Partial Configuration for parameter change of Output source

    Structure of output source SIE2 configuration.
*/
//@{
typedef struct
{
    BOOL                      bOut0Change;///< SIE2 output ch0 change switch
    //Not for SIE2//BOOL                      bOut2Change;///< SIE2 output ch2 change switch
    SIE2_OUT0_SOURCE         Out0Source; ///< SIE2 output ch0 source
    //Not for SIE2//SIE2_OUT2_SOURCE         Out2Source; ///< SIE2 output ch2 source
} SIE2_CHG_OUT_SRC_INFO;
//@}


/**
    SIE2 Open Object.

    Open Object is used for opening SIE2 module.
    \n Used for sie2_open()

*/
//@{
typedef struct _SIE2_OPENOBJ{
    void (*pfSie2IsrCb)(UINT32 uiIntrpStatus); ///< isr callback function
    UINT32 uiSie2ClockRate;  ///< SIE2 clock rate in Hz; configure '0' to skip set-up
    SIE2_PXCLKSRC PxclkSel;  ///< SIE2 pixel-clock source selection; valid in Parallel(Master/Slave) modes, invalid in Serial mode
    SIE2_CLKSRC Sie2ClkSel;  ///< SIE2 clock source selection
} SIE2_OPENOBJ;
//@}


/**
    SIE2 Entire Configuration.

    Structure of entire SIE2 configuration.
*/
//@{
typedef struct
{
    SIE2_SENSOR_PIN_PHASE    SnrPinPhase;
    //Not for SIE2//SIE2_PRETRIGSEL         PreTrigSel;
    SIE2_FRONTENDMODE       FrontEndMode;
    UINT32                  uiFuncEn;
    UINT32                  uiIntrpEn;
    SIE2_HDVD                SieVdHd;
    SIE2_ACTWIN              ActParam;
    SIE2_CRPWIN              CrpParam;
    //Not for SIE2//SIE2_HDIV                HDiv;
    //Not for SIE2//SIE2_REORDER             Reorder;
    //Not for SIE2//SIE2_FLDARR              FldArr;

    UINT32                   uiIn0Addr;
    UINT32                   uiIn0Size;

    UINT32                   uiIn1Addr;
    SIE2_PACKBUS             In1PackBus;

    UINT32                   uiOut0Add0;
    UINT32                   uiOut0Add1;
    UINT32                   uiOut0Lofs;
    SIE2_BUF_NUM             Out0BufNum;
    SIE2_PACKBUS             Out0PackBus;
    SIE2_OUT0_SOURCE         Out0Source;

    UINT32                   uiOut1Add0;
    UINT32                   uiOut1Add1;
    SIE2_BUF_NUM             Out1BufNum;

    //Not for SIE2//UINT32                   uiOut2Add0;
    //Not for SIE2//UINT32                   uiOut2Add1;
    //Not for SIE2//UINT32                   uiOut2Lofs;
    //Not for SIE2//SIE2_BUF_NUM             Out2BufNum;
    //Not for SIE2//SIE2_PACKBUS             Out2PackBus;
    //Not for SIE2//SIE2_OUT2_SOURCE         Out2Source;

    SIE2_DRAM_BURST_LENGTH_CH0   DramBstLengCh0;
    SIE2_DRAM_BURST_LENGTH_CH1   DramBstLengCh1;
    //Not for SIE2//SIE2_DRAM_BURST_LENGTH_CH2   DramBstLengCh2;
    SIE2_DRAM_BURST_LENGTH_CH3   DramBstLengCh3;
    SIE2_DRAM_BURST_LENGTH_CH4   DramBstLengCh4;

    SIE2_BREAKPOINT           BreakPoint;

    SIE2_DVI                 DviParam;
    SIE2_PATGEN_MODE        PatGenMode;
    UINT32                  uiPatGenVal;
    UINT32                  uiDpMark;
    SIE2_DEF_FACT           uiDefFact;
    SIE2_DEF_COL             DefCol;

    SIE2_OB_SUB              ObsubParam;
    SIE2_OBDT                ObdtParam;
    //Not for SIE2//SIE2_OBF                 ObfParam;
    //Not for SIE2//SIE2_OBP                 ObpParam;
    //Not for SIE2//SIE2_SMEAR               SmrParam;
    //Not for SIE2//SIE2_PFPC                PfpcParam;
    SIE2_ECS                 EcsParam;

    UINT32                  uiDGainIn3P7Bit;
    SIE2_BS_H                BshParam;
    //Not for SIE2//SIE2_BS_V                BsvParam;
    SIE2_VIG_INFO            VigParam;
    SIE2_CA                  CaParam;
    //Not for SIE2//SIE2_VA                  VaParam;
    //Not for SIE2//SIE2_GAMMA_TBL               GammaParam;
    //UINT16                  *uiGammaTblR;//not-yet: should be deleted
    //UINT16                  *uiGammaTblGr;//not-yet: should be deleted
    //UINT16                  *uiGammaTblGb;//not-yet: should be deleted
    //UINT16                  *uiGammaTblB;//not-yet: should be deleted
    //Not for SIE2//SIE2_RAW_ENC_ADJ        RawEncAdj;
} SIE2_MODE_PARAM;
//@}

//@}

/**
SIE2 API

@name   SIE2_API
*/
//@{

// sie2_lib.c
//driver-policy: SWRST is limited//extern ER sie2_reset(void);//not-yet: should be removed
extern ER sie2_open(SIE2_OPENOBJ *pObjCB);
extern BOOL sie2_isOpened(void);
extern ER sie2_close(void);
extern ER sie2_pause(void);
extern ER sie2_start(void);
extern ER sie2_setMode(SIE2_MODE_PARAM *pSie2Param);
extern ER sie2_chgClock(UINT32 uiClkRate);
extern ER sie2_chgClockSource(SIE2_CLKSRC Sie2ClkSource);
extern ER sie2_openClock(void);
extern ER sie2_closeClock(void);
extern BOOL sie2_isClockOpened(void);
extern ER sie2_chgParam(void *pParam, SIE2_CHANGE_FUN_PARAM_SEL FunSel);
extern void sie2_waitVD(UINT32 uiVdCnt);
extern void sie2_waitBP1(BOOL bClrFlag);
extern void sie2_waitBP2(BOOL bClrFlag);
//Not for SIE2//extern void sie2_waitFlashDone(BOOL bClrFlag);
//Not for SIE2//extern void sie2_waitMshtrDone(BOOL bClrFlag);
// sie2_int.c
extern void sie2_setReset(BOOL bReset);
extern void sie2_setLoad(void);
//Not for SIE2//extern void sie2_setFlashStart(void);
//Not for SIE2//extern void sie2_setMshtrCloseStart(void);
//Not for SIE2//extern void sie2_setMshtrOpenStart(void);
extern BOOL sie2_checkIntEnable(UINT32 uiSie2Intrp);
extern UINT32 sie2_getIntrStatus(void);
extern void sie2_clrIntrStatus(UINT32 uiIntrpStatus);
extern void sie2_getCrpWindow(SIE2_CRPWIN *pCrpParam);
extern void sie2_setOut0Add(UINT32 uiIndex, UINT32 uiAddr);
extern UINT32 sie2_getOut0Add(UINT32 uiIndex);
extern void sie2_setOut0Lofs(UINT32 uiLineoffset);
extern UINT32 sie2_getOut0Lofs(void);
extern void sie2_setFrontEndMode(SIE2_FRONTENDMODE FrontEndMode);
extern void sie2_setOut0PackBus(SIE2_PACKBUS PackBus);
extern SIE2_PACKBUS sie2_getOut0PackBus(void);
extern void sie2_getECS(SIE2_ECS *pEcsParam);
//Not for SIE2//extern void sie2_setSmearRedu(SIE2_SMEAR *pSmrParam);
extern void sie2_getActCrpSizeReo(UINT32 *puiActSzxReo, UINT32 *puiCrpSzxReo);
extern SIE2_PACKBUS sie2_getIn1PackBus(void);
extern void sie2_getCA(SIE2_CA *pCaParam);
extern SIE2_OUT0_SOURCE sie2_getOut0Source(void);
extern UINT32 sie2_getOut1Add(UINT32 uiIndex);
//Not for SIE2//extern SIE2_OUT2_SOURCE sie2_getOut2Source(void);
//Not for SIE2//extern void sie2_getVA(SIE2_VA *pVaParam);
extern void sie2_getBSH(SIE2_BS_H *pBshParam);
//Not for SIE2//extern void sie2_getBSV(SIE2_BS_V *pBsvParam);
//Not for SIE2//extern SIE2_PACKBUS sie2_getOut2PackBus(void);
extern void sie2_getSensorSync(SIE2_HDVD *pSieVdHd);
//Not for SIE2//extern UINT32 sie2_getOut2Lofs(void);
//Not for SIE2//extern void sie2_getReorder(SIE2_REORDER *pReorder);
//Not for SIE2//extern void sie2_getFldArr(SIE2_FLDARR *pFldArr);
extern void sie2_calBSHScl(SIE2_BS_H *pBshParam, UINT32 uiInSz, UINT32 uiOutSz, UINT32 uiLpf, UINT32 uiBinPwr);
//Not for SIE2//extern void sie2_calBSVScl(SIE2_BS_V *pBsvParam, UINT32 uiInSz, UINT32 uiOutSz, UINT32 uiLpf, UINT32 uiBinPwr);
extern void sie2_enableFunction(BOOL bEnable,UINT32 uiFunction);
extern void sie2_setFunction(UINT32 uiFunction);
extern BOOL sie2_checkFunctionEnable(UINT32 uiFunction);
extern void sie2_calcCA(SIE2_CA *pCaParam, SIE2_CA_SETTING *pCaSetting);
//Not for SIE2//extern void sie2_calcVA(SIE2_VA *pVaParam, SIE2_VA_SETTING *pVaSetting);
extern void sie2_calcVIG_OneImg(SIE2_VIG_INFO *pVigParam, SIE2_VIG_SETTING_ONE_IMG *pVigSettingOneImg);
extern void sie2_calcVIG_TwoImg(SIE2_VIG_INFO *pVigParam, SIE2_VIG_SETTING_TWO_IMG *pVigSettingTwoImg);
extern void sie2_getCAResult(UINT16 *puiBufR, UINT16 *puiBufG, UINT16 *puiBufB);
extern void sie2_getCAResultManual(UINT16 *puiBufR, UINT16 *puiBufG, UINT16 *puiBufB, SIE2_CA *pCaParam, UINT32 uiBufAddr);
//Not for SIE2//extern void sie2_getVAResult(UINT16 *puiBufVa);
//Not for SIE2//extern void sie2_getRawEncRslt(SIE2_RAW_ENC_STATUS *pRawEncStatus);
//Not for SIE2//extern void sie2_getRawEncRsltLastFrame(SIE2_RAW_ENC_STATUS *pRawEncStatus);
extern UINT32 sie2_getCurrentOutputBuffID(void);
extern void sie2_getSnrPinPhase(SIE2_SENSOR_PIN_PHASE *pSnrPinPhase);
extern UINT32 sie2_getOBDTAvg0(void);
//Not for SIE2//extern void sie2_getOBFRslt(SIE2_OBF_RSLT *pObfRslt);
//Not for SIE2//extern void sie2_calOBPScl(SIE2_OBP *pObpParam, UINT32 uiActSzX, UINT32 uiActSzY);
extern void sie2_calECSScl(SIE2_ECS *pEcsParam, UINT32 uiActSzX, UINT32 uiActSzY);
//Not for SIE2//extern void sie2_getSmearRedu(SIE2_SMEAR *pSmrParam);
extern UINT32 sie2_getClockRate(void);
#endif
//@}

//@}


