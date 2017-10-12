/**
    IPH module (SIE2 type define)

    @file       IPL_Hal_SIE2.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_SIE2_H
#define _IPL_Hal_SIE2_H
#include "IPL_Hal_SIE_info.h"
#include "IPL_Hal_SIE2_Info.h"
#include "IPL_ISREvent.h"
/** \addtogroup mILibIPH */
//@{

/**
    query enum

    read from hw register
*/
typedef enum _IPH_SIE2_QUE
{
    SIE2_REG_CARST           = 0x00000001,   ///< read CA result(R,G,B average)                     Data type: SIE_CARST
    SIE2_REG_CAWIN           = 0x00000003,   ///< read CA window                                     Data type: ACC_WIN
    SIE2_REG_Ch0CurRdyAddr   = 0x00000005,   ///< read sie2 current dma ready address                 Data type: UINT32
    SIE2_REG_Ch0CurRdyIdx    = 0x00000006,   ///< read sie2 current dma ready buffer idx              Data type: UINT32
    ENUM_DUMMY4WORD(IPH_SIE2_QUE)
}IPH_SIE2_QUE;

/**
     enum of SIE update selection
*/
typedef enum _SIE2_UPDATE
{
    SIE2_OutAddr_Ch0 = 0x00000001,   ///< update ch0 outaddr
    SIE2_OutAddr_CA  = 0x00000002,   ///< update CA outaddr
    SIE2_IOSize      = 0x00000008,   ///< update crop size/start pix/CFA,ch0 output size/lifeoffset
    SIE2_SubFEn      = 0x00000010,   ///< update func enable/disable
    SIE2_DPC_        = 0X00000020,   ///< update defect pixel concealment
    SIE2_COLD_       = 0X00000080,   ///< update column defect
    SIE2_OBAVG_      = 0x00000100,   ///< update ob avg ( ob detection)
    SIE2_OBOFS_      = 0X00000200,   ///< update ob offset
    SIE2_ECS_        = 0X00002000,   ///< update ECS/including ECS table addr
    SIE2_DGAIN_      = 0x00004000,   ///< update Digital gain
    SIE2_VIG_        = 0x00008000,   ///< update VIG
    SIE2_CA_         = 0X00010000,   ///< update CACC,not include addr
    SIE2_DVI_        = 0x00020000,   ///< update DVI(CCIR)
    SIE2_CH0Out_SRC  = 0x00100000,   ///< update dma out ch0 source
    SIE2_BP_         = 0x00400000,   ///< update BreakPoint1/2/3,ref 1/2/3 timing
    SIE2_BURSTLENG   = 0x01000000,   ///< update output ch0 dram burst length
    SIE2_INIT        = 0x04000000,   ///< init SIE2
    ENUM_DUMMY4WORD(SIE2_UPDATE)
}SIE2_UPDATE;

/**
     enum of SIE read selection

     read from inner table, not from hw register.
*/
typedef enum _SIE2_READ
{
    SIE2_R_IO        = 0x00000001,   ///< read act_win coordinate,cfa,signal,ch0/1 dma addr,PPBNUM
                                    ///< ,actmode,trigmode exclude SIE2_SUBFUNC,             Data type: SIE2
    SIE2_R_Out       = 0x00000002,   ///< read sie2 ch0 dma size/lineofs/bit              Data type: SIE2
    SIE2_R_InSize    = 0x00000003,   ///< read act window size,crop window info              Data type: SIE2_WINDOW
    SIE2_R_SubFEn    = 0x00000004,   ///< read func enable/disable,                          Data type: Sie2FuncEn
    SIE2_R_DPC_      = 0X00000005,   ///< read defect pixel concealment,                     Data type: SIE2_DPC
    SIE2_R_COLD_     = 0X00000007,   ///< read column defect                                 Data type: SIE2_ColumnDef
    SIE2_R_OBAVG_    = 0x00000008,   ///< read ob avg                                        Data type: SIE2_OBAVG
    SIE2_R_OBOFS_    = 0X00000009,   ///< read ob offset                                     Data type: SIE2_OBSUB
    SIE2_R_ECS_      = 0X00000013,   ///< read ECS                                           Data type: SIE2_EmbeddedCS
    SIE2_R_DGAIN_    = 0x00000014,   ///< read Digital gain                                  Data type: UINT32
    SIE2_R_VIG_      = 0x00000015,   ///< read VIG  para. set                                Data type: SIE2_CA_VIG
    SIE2_R_CA_       = 0X00000016,   ///< read CACC para. set,not include addr               Data type: SIE2_CACC
    SIE2_R_BP_       = 0x00000020,   ///< read BreakPoint1/2/3,ref 1/2/3 timing              Data type: SIE2_BP
    SIE2_R_INTEEn_   = 0x00000022,   ///< read sie interrupt enable bit                      Data type: SIE2_INTE_EN
    SIE2_R_CCIR_     = 0x00000023,   ///< read sie ccir info                                 Data type: SIE2__DVI
    SIE2_R_CH0_SRC   = 0x00000024,   ///< read ch0 output source                             Data type: CH0_SRC
    //SIE2_R_CARST     = 0x00000025,   ///< read CA result(R,G,B average),IPL_HAL_PARASET don't care. Data type: SIE2_CARST
    //SIE2_R_RAWENC_BS = 0x00000026,   ///< read rawEncode Bitstream size,IPL_HAL_PARASET don't care.  Data type: UINT32
    ENUM_DUMMY4WORD(SIE2_READ)
}SIE2_READ;

/**
     SIE configuration

     all SIE configuration including all SIE sub function
*/
typedef struct _SIE2_Manager
{
    IPP_SIE2_SIGNAL  Sie2_Signal; ///< sensor information

    struct
    {
        SIE_Dma_Out Out;        ///< -
        SIE_PPBNum  Sie2PPBNum;  ///< output buffer control
        CH0_SRC     Src;        ///< output selection
    }Ch0;                          ///< sie channel_0 output

    struct
    {
        SIE_CH1_Dma_Out Out;    ///< -
        SIE_PPBNum CH1_PPBNum;  ///< -
    }CACC_Addr;                          ///< sie channel_1 output ( Color accumulation output)

    SIE2_WINDOW  IMG_Window;     ///< act/crop wiondow
    UINT32  DmaOutSizeH;        ///< this size map to scaling output size,
    SIE2_BURST_LENG  BurstLeng_Ch0;  ///< sie ch0 dram burst length
    SIE2_SUBFUNC *pSubFunc;      ///< -

    //reverse
    //UINT32 rev;                     //reserve for IPH paraset idx
}SIE2_Manager,*pSIE2_Manager;

/**
     SIE configuration
*/
typedef SIE2_Manager SIE2;

//@}
#endif
