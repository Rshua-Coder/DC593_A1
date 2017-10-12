/**
    IPH module (DCE type define)

    @file       IPL_Hal_DCE.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_DCE_H
#define _IPL_Hal_DCE_H
#include "IPL_Hal_DCE_Info.h"
#include "IPL_ISREvent.h"

/** \addtogroup mILibIPH */
//@{

typedef enum _IPH_DCE_QUE
{
    ENUM_DUMMY4WORD(IPH_DCE_QUE)
}IPH_DCE_QUE;

/**
     enum of DCE update selection
*/
typedef enum _DCE_UPDATE
{
    DCE_IN_Addr        = 0x00000001,    ///< update YUV Input addr
    DCE_OUT_Addr       = 0x00000002,    ///< update YUV output addr
    DCE_IO_Size        = 0x00000004,    ///< update in/out size/lineoffset
    DCE_STRIPE         = 0x00000008,    ///< update Stripe Increment/decrement pixel number/stripe overlap selection/stripe table
    DCE_Center         = 0x00000010,    ///< update distortion center
    DCE_DisT           = 0x00000020,    ///< update H/V distance factor/ratio selection
    DCE_Radius         = 0x00000040,    ///< update radius norm term
    DCE_CAB            = 0x00000080,    ///< update color aberration factor
    DCE_LUT            = 0x00000100,    ///< update LUT table address
    DCE_LUTType        = 0x00000200,    ///< update LUT table type(inc/dec,or none)
    DCE_FOV            = 0x00000400,    ///< update FOV
    DCE_ENH            = 0x00000800,    ///< update enhancement
    DCE_H_FILT         = 0X00001000,    ///< update h direction filter enable/disable(UV only)
    DCE_DISTOR         = 0x00001fb0,    ///< update distortion parameter set
    DCE_INIT           = 0x00002000,    ///< init DCE
    //DCE_par            = 0xfffff00f,    ///< update all parameter excluding IQ relative parameter
    //DCE_par            = 0xffffffff,    ///< update all parameter including IQ relative parameter
    ENUM_DUMMY4WORD(DCE_UPDATE)
} DCE_UPDATE;

/**
     enum of DCE read selection

     read from inner table, not from hw register.
*/
typedef enum _DCE_READ
{
    DCE_R_IO        = 0x00000001,   ///< read in/out info. exclude DCE_SUBFUNC,             Data type: DCE
    DCE_R_Center    = 0X00000002,   ///< read distortion center,                            Data type: Coordinate
    DCE_R_DisT      = 0x00000003,   ///< read H/V distance factor/ratio selection,          Data type: DIST
    DCE_R_Radious   = 0X00000004,   ///< read radius norm term,                             Data type: Fact_Norm
    DCE_R_CAB       = 0x00000005,   ///< read color aberration factor,                      Data type: Aberation
    DCE_R_LUT       = 0X00000006,   ///< read LUT table address,                            Data type: UINT32
    DCE_R_FOV       = 0X00000007,   ///< read FOV,                                          Data type: FOV
    DCE_R_ENH       = 0X00000008,   ///< read enhancement,                                  Data type: EnH
    DCE_R_H_FILT    = 0x00000009,   ///< read H direction filter Enable                     Data type: BOOL
    DCE_R_INTEEn    = 0x00000010,   ///< read interruppt enabe bit                          Data type: DceInteEn
    DCE_R_LUTTYPE   = 0x00000011,   ///< read dce lut table type(inc/dec, none)             Data type: DCE_LutType
    DCE_R_STRPE     = 0x00000012,   ///< read stripe table address,                         Data type: UINT32
    ENUM_DUMMY4WORD(DCE_READ)
}DCE_READ;

/**
    DCE stripe configuration
*/
typedef struct _DCE_Stripe
{
    UINT32  *stripeTbl;             ///< stripe table, tab num: 16, Data type: UINT32
    UINT32  IncNum;                 ///< max input horizontal stripe Increment pixel number
    UINT32  DecNum;                 ///< max input horizontal stripe Decrement pixel number
    enum
    {
        First_STP_POS_OFS   = 0,    ///< Input starts from DCE calculated position
        First_STP_POS_NONOFS= 1,    ///< Input starts from 0
        ENUM_DUMMY4WORD(StrPos)
    }StrPos;                        ///< Input first stripe start position selection
    enum
    {
        H_OVERLAP_16    = 0,        ///< overlap 16 pixels
        H_OVERLAP_24    = 1,        ///< -
        H_OVERLAP_32    = 2,        ///< -
        ENUM_DUMMY4WORD(Out_STP_Overlap)
    }Out_STP_Overlap;               ///< Output stripe overlap selection
}DCE_Stripe;

/**
     DCE configuration

     all DCE configuration including all DCE sub function
*/
typedef struct _DCE_Manager
{
    enum
    {
        DCE_EMIPE   = 0,            ///< embedded in ipe
        DCE_D2D     = 1,            ///< dram to dram
        ENUM_DUMMY4WORD(DCE_Ctrl)
    }DCE_Ctrl;                      ///< -

    USIZE       Size;               ///< in/out size
    DCE_Stripe  Stripe;             ///< stripe configuration

    struct
    {
        UINT32  Addr_Y;             ///< -
        UINT32  Addr_UV;            ///< -
        UINT32  Lineoffset_Y;       ///< -
        UINT32  Lineoffset_UV;      ///< -
        DCEFMT  Fmt;                ///< -
    } Dram_In;                      ///< source which input from dram

    struct
    {
        UINT32  Addr_Y;             ///< -
        UINT32  Addr_UV;            ///< -
        UINT32  Lineoffset_Y;       ///< -
        UINT32  Lineoffset_UV;      ///< -
    } Dram_Out;                     ///< output data to dram

    DCE_SUBFUNC *pSubFunc;          ///< -
    //reverse
    //UINT32 rev;                     //reserve for IPH paraset idx
}DCE_Manager,*pDCE_Manager;

/**
     DCE configuration
*/
typedef DCE_Manager DCE;

//@}
#endif
