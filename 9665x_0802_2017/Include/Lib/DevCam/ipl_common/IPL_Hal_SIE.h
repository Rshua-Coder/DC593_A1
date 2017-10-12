/**
    IPH module (SIE type define)

    @file       IPL_Hal_SIE.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_SIE_H
#define _IPL_Hal_SIE_H
#include "IPL_Hal_SIE_Info.h"
#include "IPL_ISREvent.h"
/** \addtogroup mILibIPH */
//@{

/**
    query enum

    read from hw register
*/
typedef enum _IPH_SIE_QUE
{
    SIE_REG_CARST           = 0x00000001,   ///< read CA result(R,G,B average)                     Data type: SIE_CARST
    SIE_REG_VARST           = 0X00000002,   ///< read VA result()                                   Data type: SIE_VARST
    SIE_REG_CAWIN           = 0x00000003,   ///< read CA window                                     Data type: ACC_WIN
    SIE_REG_RAWENC          = 0x00000004,   ///< read rawEncode Bitstream size                      Data type: UINT32
    SIE_REG_Ch0CurRdyAddr   = 0x00000005,   ///< read sie current dma ready address                 Data type: UINT32
    SIE_REG_Ch0CurRdyIdx    = 0x00000006,   ///< read sie current dma ready buffer idx              Data type: UINT32
    SIE_REG_OBDETRST        = 0x00000007,   ///< read sie ob detection rst                          Data type: UINT32
    SIE_REG_Ch1CurRdyAddr   = 0x00000008,   ///< read sie current dma ready address                 Data type: UINT32
    ENUM_DUMMY4WORD(IPH_SIE_QUE)
}IPH_SIE_QUE;

/**
     enum of SIE update selection
*/
typedef enum _SIE_UPDATE
{
    SIE_OutAddr_Ch0 = 0x00000001,   ///< update ch0 outaddr
    SIE_OutAddr_CA  = 0x00000002,   ///< update CA outaddr
    SIE_OutAddr_Ch2 = 0x00000004,   ///< update ch2 outaddr
    SIE_IOSize      = 0x00000008,   ///< update crop size/start pix/CFA,ch0 output size/lifeoffset,ch2 output lineoffset
    SIE_SubFEn      = 0x00000010,   ///< update func enable/disable
    SIE_DPC_        = 0X00000020,   ///< update defect pixel concealment
    SIE_EMDPC_      = 0x00000040,   ///< update embedded defect pixel concealment
    SIE_COLD_       = 0X00000080,   ///< update column defect
    SIE_OBAVG_      = 0x00000100,   ///< update ob avg ( ob detection)
    SIE_OBOFS_      = 0X00000200,   ///< update ob offset
    SIE_OBF_        = 0X00000400,   ///< update ob frame
    SIE_OBP_        = 0X00000800,   ///< update ob plane
    SIE_SMR_        = 0X00001000,   ///< update smear
    SIE_ECS_        = 0X00002000,   ///< update ECS/including ECS table addr
    SIE_DGAIN_      = 0x00004000,   ///< update Digital gain
    SIE_VIG_        = 0x00008000,   ///< update VIG
    SIE_CA_         = 0X00010000,   ///< update CACC,not include addr
    SIE_VA_         = 0X00020000,   ///< update VACC
    SIE_GAMMA_      = 0X00040000,   ///< update 4-ch gamma
    SIE_PFPC_       = 0x00080000,   ///< update pfpc
    SIE_CH0Out_SRC  = 0x00100000,   ///< update dma out ch0 source
    SIE_CH2Out_SRC  = 0x00200000,   ///< update dma out ch2 source
    SIE_BP_         = 0x00400000,   ///< update BreakPoint1/2/3,ref 1/2/3 timing
    SIE_REORDER_    = 0x00800000,   ///< update reorder
    SIE_BURSTLENG   = 0x01000000,   ///< update output ch0 dram burst length
    SIE_FILD_       = 0x02000000,   ///< update field
    SIE_INIT        = 0x04000000,   ///< init sie
    //SIE_par         = 0x0630001f,   ///< update all parameter excluding above parameters
    //SIE_par         = 0x01ffffff,   ///< update all parameter including above parameters

    SIE_FLASH_PAR   = 0x08000000,   ///< update flash parameter
    SIE_FLASH_START = 0x10000000,   ///< trig sie flash start
    SIE_MSHUT_PAR   = 0x20000000,   ///< update mshutter parameter
    SIE_MSHUT_OPEN  = 0x40000000,   ///< trig sie mshutter open
    SIE_MSHUT_CLOSE = 0x80000000,   ///< trig sie mshutter close
}SIE_UPDATE;

/**
     enum of SIE read selection

     read from inner table, not from hw register.
*/
typedef enum _SIE_READ
{
    SIE_R_IO        = 0x00000001,   ///< read act_win coordinate,cfa,signal,ch0/1/2 dma addr,PPBNUM
                                    ///< ,actmode,trigmode exclude SIE_SUBFUNC,             Data type: SIE
    SIE_R_Out       = 0x00000002,   ///< read sie ch0/ch2 dma size/lineofs/bit              Data type: SIE
    SIE_R_InSize    = 0x00000003,   ///< read act window size,crop window info              Data type: SIE_WINDOW
    SIE_R_SubFEn    = 0x00000004,   ///< read func enable/disable,                          Data type: SieFuncEn
    SIE_R_DPC_      = 0X00000005,   ///< read defect pixel concealment,                     Data type: SIE_DPC
    SIE_R_EMDPC_    = 0x00000006,   ///< read embedded defect pixel concealment,            Data type: SIE_EMDPC
    SIE_R_COLD_     = 0X00000007,   ///< read column defect                                 Data type: SIE_ColumnDef
    SIE_R_OBAVG_    = 0x00000008,   ///< read ob avg                                        Data type: SIE_OBAVG
    SIE_R_OBOFS_    = 0X00000009,   ///< read ob offset                                     Data type: SIE_OBSUB
    SIE_R_OBF_      = 0X00000010,   ///< read ob frame                                      Data type: SIE_OBFAVG
    SIE_R_OBP_      = 0X00000011,   ///< read ob plane                                      Data type: SIE_OBPSUB
    SIE_R_SMR_      = 0X00000012,   ///< read smear                                         Data type: SIE_SMR
    SIE_R_ECS_      = 0X00000013,   ///< read ECS                                           Data type: SIE_EmbeddedCS
    SIE_R_DGAIN_    = 0x00000014,   ///< read Digital gain                                  Data type: UINT32
    SIE_R_VIG_      = 0x00000015,   ///< read VIG  para. set                                Data type: SIE_CA_VIG
    SIE_R_CA_       = 0X00000016,   ///< read CACC para. set,not include addr               Data type: SIE_CACC
    SIE_R_VA_       = 0X00000017,   ///< read VACC para. set                                Data type: SIE_VACC
    SIE_R_GAMMA_    = 0X00000018,   ///< read 4-ch gamma                                    Data type: SIE_GAMMA
    SIE_R_PFPC_     = 0x00000019,   ///< read pfpc                                          Data type: SIE_PFP
    SIE_R_BP_       = 0x00000020,   ///< read BreakPoint1/2/3,ref 1/2/3 timing              Data type: SIE_BP
    SIE_R_RAWEN_    = 0x00000021,   ///< read Raw encode                                    Data type: SIE_RAWENC
    SIE_R_INTEEn_   = 0x00000022,   ///< read sie interrupt enable bit                      Data type: SIE_INTE_EN
    //SIE_R_CCIR_     = 0x00000023,   ///< read sie ccir info                                 Data type: SIE__DVI
    SIE_R_CH0_SRC   = 0x00000024,   ///< read ch0 output source                             Data type: CH0_SRC
    SIE_R_CH2_SRC   = 0x00000025,   ///< read ch2 output source                             Data type: CH2_SRC
    SIE_R_CA_PARAM_ = 0x00000026,   ///< read ch1 infomration                               Data type: SIE_CAPARAM
    //SIE_R_CARST     = 0x00000025,   ///< read CA result(R,G,B average),IPL_HAL_PARASET don't care. Data type: SIE_CARST
    //SIE_R_RAWENC_BS = 0x00000026,   ///< read rawEncode Bitstream size,IPL_HAL_PARASET don't care.  Data type: UINT32
    ENUM_DUMMY4WORD(SIE_READ)
}SIE_READ;

/**
     SIE configuration

     all SIE configuration including all SIE sub function
*/
typedef struct _SIE_Manager
{
    IPP_SIE_SIGNAL  Sie_Signal; ///< sensor information
    SIEPreTrigSel   TrigSel;    ///< sie trig pre selection

    SieACTMode      ActMode;    ///< sie source selection
    struct
    {
        SIE_Dma_Out Out;        ///< -
        SIE_PPBNum  SiePPBNum;  ///< output buffer control
        CH0_SRC     Src;        ///< output selection
    }Ch0;                          ///< sie channel_0 output

    struct
    {
        SIE_Dma_Out Out;        ///< -
        SIE_PPBNum  SiePPBNum;  ///< -
        CH2_SRC     Src;        ///< -
    }Ch2;                          ///< sie channel_2 output

    struct
    {
        SIE_CH1_Dma_Out Out;    ///< -
        SIE_PPBNum CH1_PPBNum;  ///< -
    }CACC_Addr;                          ///< sie channel_1 output ( Color accumulation output)

    SIE_WINDOW  IMG_Window;     ///< act/crop wiondow
    UINT32  DmaOutSizeH;        ///< this size map to scaling output size,
    UINT32  DmaOutSizeV;        ///< this size map to scaling output size,
    SIE_BURST_LENG  BurstLeng_Ch0;  ///< sie ch0 dram burst length
    SIE_SUBFUNC *pSubFunc;      ///< -

    //reverse
    //UINT32 rev;                     //reserve for IPH paraset idx
}SIE_Manager,*pSIE_Manager;

/**
     SIE configuration
*/
typedef SIE_Manager SIE;

//@}
#endif
