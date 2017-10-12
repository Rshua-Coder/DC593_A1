/**
    IPH module (IFE2 type define)

    @file       IPL_Hal_IFE2.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_IFE2_H
#define _IPL_Hal_IFE2_H
#include "IPL_Hal_IFE2_Info.h"
#include "IPL_ISREvent.h"
/** \addtogroup mILibIPH */
//@{

/**
     Query enum

     read from hw register.
*/
typedef enum _IPH_IFE2_QUE
{
    IFE2_Q_InAddr       = 0x00000001,   ///< read Input addr from reg.      Data type: IFE2_REG_STATUS
    IFE2_Q_OutAddr      = 0x00000002,   ///< read Output addr from reg.     Data type: IFE2_REG_STATUS
    IFE2_Q_IOSize       = 0x00000004,   ///< read IN/OUT size from reg.     Data type: IFE2_REG_STATUS
    IFE2_Q_Fmt          = 0x00000008,   ///< read Data format from reg.     Data type: IFE2_REG_STATUS
    ENUM_DUMMY4WORD(IPH_IFE2_QUE)
}IPH_IFE2_QUE;

/**
     query result

     read from hw register
*/
typedef struct _IFE2_REG_STATUS
{
    UINT32 InAddr;  ///< -
    UINT32 OutAddr; ///< -
    struct
    {
        UINT32 Width;   ///< -
        UINT32 Height;  ///< -
        UINT32 Lineoffset;  ///< -
    }Size;  ///< -
    IFE2_FMT Fmt;   ///< -
}IFE2_REG_STATUS;

/**
     enum of IFE2 update selection
*/
typedef enum _IFE2_UPDATE
{
    IFE2_InAddr_Lineoft = 0x00000001,       ///< update input addr/lineoffset
    IFE2_OutAddr_Lineoft= 0x00000002,       ///< update output addr/lineoffset
    IFE2_InHV           = 0x00000004,       ///< update input size
    IFE2_SubFEn         = 0x00000010,       ///< update func enable/disable
    IFE2_Chroma_        = 0x00000040,       ///< update chroma adjust
    IFE2_AvgFilter_     = 0x00000080,       ///< update average filter
    IFE2_INIT           = 0x00000100,       ///< init ife2
    //IFE2_par            = 0xffffff1f,       ///< update all parameter excluding IQ relative parameter
    //IFE2_par            = 0xffffffff,       ///< update all parameter including IQ relative parameter
    ENUM_DUMMY4WORD(IFE2_UPDATE)
}IFE2_UPDATE;

/**
     enum of ife2 read selection

     read from inner table, not from hw register.
*/
typedef enum _IFE2_READ
{
    IFE2_R_IO           = 0x00000001,   ///< read in/out info. exclude IFE2_SUBFUNC,        Data type: IFE2
    IFE2_R_SubFEn       = 0X00000002,   ///< read func enable/disable,                      Data type: IFE2FuncEn
    IFE2_R_Chroma_      = 0x00000003,   ///< read va detection/accumulation,                Data type: IFE2_Chroma
    IFE2_R_AvgFilter_   = 0X00000004,   ///< read independent va window,                    Data type: IFE2_AvgFilter
    IFE2_R_INTEEn       = 0x00000005,   ///< read interruppt enabe bit,                     Data type: IFE2_INTE_EN
    ENUM_DUMMY4WORD(IFE2_READ)
}IFE2_READ;


/**
     IFE2 configuration

     all ife2 configuration including all IFE2 sub function
*/
typedef struct _IFE2_Manager
{
    IFE2_DBMODE     DebugMode;      ///< ife2 operation mode
    IFE2_FMT        Fmt;            ///< ife2 input data format
    UINT32          InAddr;         ///< ife2 dma input address
    IFE2_Size       InSize;         ///< ife2 input size
    UINT32          OutAddr;        ///< ife2 dma output address
    UINT32          OutLineoffset;  ///< ife2 output lineofs

    IFE2_SUBFUNC    *pSubFunc;      ///< ife2 sub function pointer
    //reverse
    //UINT32 rev;                     //reserve for IPH paraset idx
}IFE2_Manager,*pIFE2_Manager;

/**
     IFE2 configuration
*/
typedef IFE2_Manager IFE2;

//@}
#endif