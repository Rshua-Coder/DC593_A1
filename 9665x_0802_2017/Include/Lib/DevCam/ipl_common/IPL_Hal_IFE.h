/**
    IPH module (IFE sub function type define)

    @file       IPL_Hal_IFE.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_IFE_H
#define _IPL_Hal_IFE_H
#include "IPL_Hal_IFE_Info.h"
#include "IPL_ISREvent.h"
/** \addtogroup mILibIPH */
//@{
/**
     query enum

     read from hw register.
*/
typedef enum _IPH_IFE_QUE
{
    ENUM_DUMMY4WORD(IPH_IFE_QUE)
}IPH_IFE_QUE;

/**
     enum of IFE update selection
*/
typedef enum _IFE_UPDATE
{
    IFE_IO_Addr     = 0X00000001,       ///< update input/output addr
    IFE_IO_Size     = 0x00000002,       ///< update input/output size/lineoffset
    IFE_SubFEn      = 0X00000004,       ///< update func enable/disable
    IFE_Outl_       = 0X00000008,       ///< update outlier
    IFE_RowDef_     = 0x00000010,       ///< update row defect concealment
    IFE_CrvMap_     = 0x00000020,       ///< update courve map
    IFE_Cgain_      = 0x00000040,       ///< update color gain
    IFE_Filter_     = 0x00000080,       ///< update noise reduction filter
    IFE_INIT        = 0x00000100,       ///< init ife
    //IFE_par         = 0xffffff07,       ///< update all parameter excluding IQ relative parameter
    //IFE_par         = 0xffffffff,       ///< update all parameter including IQ relative parameter
    ENUM_DUMMY4WORD(IFE_UPDATE)
}IFE_UPDATE;

/**
     enum of ife read selection

     read from inner table, not from hw register.
*/
typedef enum _IFE_READ
{
    IFE_R_IO        = 0x00000001,   ///< read in/out info. exclude IFE_SUBFUNC,             Data type: IFE
    IFE_R_SubFEn    = 0x00000002,   ///< read func enable/disable,                          Data type: IfeFuncEn
    IFE_R_OUTL_     = 0X00000003,   ///< read out liner,                                    Data type: IFE_OUTL
    IFE_R_ROWDEF_   = 0x00000004,   ///< read row defect concealment,                       Data type: IFE_ROWDEF
    IFE_R_CRVMAP_   = 0X00000005,   ///< read courve map,                                   Data type: IFE_CRVMAP
    IFE_R_CGAIN_    = 0x00000006,   ///< read digital color gain,                           Data type: IFE_CG
    IFE_R_FILTER_   = 0X00000007,   ///< read ife image filter,                             Data type: IFE_FILT
    IFE_R_INTEEn    = 0x00000008,   ///< read interruppt enabe bit                          Data type: IfeInteEn
    ENUM_DUMMY4WORD(IFE_READ)
}IFE_READ;

/**
     IFE configuration

     all IFE configuration including all IFE sub function
*/
typedef struct _IFE_Manager
{
    enum
    {
        IFE_D2D         = 0,    ///< Dram In,Dram Out
        IFE_DIRECT      = 1,    ///< Pre In, Ipe Out
        IFE_PRE2DRAM    = 2,    ///< Pre In,Dram Out
        ENUM_DUMMY4WORD(IFEmode)
    }IFEmode;                   ///< IFE op mode

    IFEFMT IFEFmt;              ///< ife in/out data format

    struct
    {
        UINT32  Addr;           ///< -
        UINT32  Width;          ///< -
        UINT32  Height;         ///< -
        UINT32  Lineoffset;     ///< -
        IFE_PackBus packBus;    ///< -
    }IFE_In;                    ///< ife input information

    struct
    {
        UINT32  Addr;           ///< -
        UINT32  Lineoffset;     ///< -
        IFE_PackBus packBus;    ///< -
    }IFE_Out;                   ///< ife output information

    IFE_SUBFUNC *pSubFunc;      ///< -
    //reverse
    //UINT32 rev;                     //reserve for IPH paraset idx
}IFE_Manager,*pIFE_Manager;

/**
     IFE configuration
*/
typedef IFE_Manager IFE;
//@}
#endif
