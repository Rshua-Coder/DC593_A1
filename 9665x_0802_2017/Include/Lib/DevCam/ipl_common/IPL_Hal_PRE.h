/**
    IPH module (PRE type define)

    @file       IPL_Hal_PRE.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_PRE_H
#define _IPL_Hal_PRE_H
#include "IPL_Hal_PRE_Info.h"
#include "IPL_ISREvent.h"
/** \addtogroup mILibIPH */
//@{

/**
     Query enum

     read from hw register.
*/
typedef enum _IPH_PRE_QUE
{
    PRE_REG_Ch0PPB0InAddr = 0x00000001, ///< read pre ch0 input PPB0 address,   Data type: UINT32
    PRE_REG_Ch0PPB1InAddr = 0x00000002, ///< read pre ch0 input PPB1 address,   Data type: UINT32
    ENUM_DUMMY4WORD(IPH_PRE_QUE)
}IPH_PRE_QUE;


/**
     enum of PRE update selection
*/
typedef enum _PRE_UPDATE
{
    PRE_InAddr         = 0x00000001,    ///< update Input addr
    PRE_OutAddr        = 0x00000002,    ///< update output addr
    PRE_IOSize         = 0x00000004,    ///< update in size/lineoffset/CFA,output lineoffset
    PRE_SubFEn         = 0x00000008,    ///< update func enable/disable
    PRE_DFS_           = 0x00000200,    ///< update dark frame substraction
    PRE_DIV_           = 0x00000400,    ///< update division
    PRE_EMDPC_         = 0x00000800,    ///< update embedded defect pixel concealment
    PRE_CTC_           = 0x00001000,    ///< update cross talk compensation
    PRE_VIG_           = 0x00002000,    ///< update vig
    PRE_COFS_          = 0x00004000,    ///< update color offset
    PRE_INIT           = 0x00008000,    ///< init pre
    //PRE_par            = 0xffff00ff,    ///< update all parameter excluding above parameters
    //PRE_par            = 0xffffffff,    ///< update all parameter including above parameters
    ENUM_DUMMY4WORD(PRE_UPDATE)
}PRE_UPDATE;

/**
     enum of pre read selection

     read from inner table, not from hw register.
*/
typedef enum _PRE_READ
{
    PRE_R_IO        = 0x00000001,   ///< read out info. exclude PRE_SUBFUNC,                Data type: PRE
    PRE_R_DMAIn     = 0x00000002,   ///< read dma input info.                               Data type: PRE
    PRE_R_SubEn     = 0x00000003,   ///< read func enable/disable,                          Data type: PreFuncEn
    PRE_R_DFS_      = 0X00000004,   ///< read dark frame subtraction,                       Data type: PRE_DFS
    PRE_R_EMDPC_    = 0x00000005,   ///< read embedded defect pixel concealment,            Data type: PRE_EMDPC
    PRE_R_DIV_      = 0X00000006,   ///< read division arrangement                          Data type: PRE_DIV
    PRE_R_CTC_      = 0x00000007,   ///< read cross-talk compensation                       Data type: PRE_CTC
    PRE_R_VIG_      = 0X00000008,   ///< read vignette shading compensation                 Data type: PRE_VIG
    PRE_R_COFS_     = 0X00000009,   ///< read color offset                                  Data type: PRE_COFS
    PRE_R_INTEEn    = 0x00000010,   ///< read interruppt enabe bit                          Data type: PRE_INTE_EN
    ENUM_DUMMY4WORD(PRE_READ)
}PRE_READ;

/**
     PRE configuration

     all PRE configuration including all PRE sub function
*/
typedef struct _PRE_Manager
{
    PRE_DMA_IN In_DmaCh0;           ///<DmaCh0 Input information

    PRE_OUT Out;                    ///<Output information

    enum
    {
        PRE_Ctrl_D2D    = 0,    ///< Dram-PRE-Dram
        PRE_Ctrl_P1     = 1,    ///< Dram-PRE-IFE-Dram
        PRE_Ctrl_P2     = 2,    ///< Dram-PRE-IFE-IPE/DCT-IME-Dram
        ENUM_DUMMY4WORD(PRE_Ctrl)
    }PRE_Ctrl;                  ///< pre ctrl mode

    enum
    {
        PRE_Trig_SIE    = 0,    ///< SIE Auto Trigger
        PRE_Trig_FW     = 1,    ///< FW  Ctrl
        ENUM_DUMMY4WORD(PRE_Trig)
    }PRE_Trig;                  ///< pre trig mode

    PRE_SUBFUNC* pSubFunc;      ///< -

    //reverse
    //UINT32 rev;                     //reserve for IPH paraset idx
}PRE_Manager,*pPRE_Manager;

/**
     PRE configuration
*/
typedef PRE_Manager PRE;
//@}
#endif