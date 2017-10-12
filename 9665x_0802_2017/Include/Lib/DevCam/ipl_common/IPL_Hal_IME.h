/**
    IPH module (IME type define)

    @file       IPL_Hal_IME.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_IME_H
#define _IPL_Hal_IME_H
#include "IPL_Hal_IME_Info.h"
#include "IPL_ISREvent.h"
/** \addtogroup mILibIPH */
//@{

/**
    query enum

    read from hw register
*/
typedef enum _IPH_IME_QUE
{
    //CurRdyAddr_p1 = 0x00000001,
    //CurRdyAddr_p2 = 0x00000002,
    //CurRdyAddr_p3 = 0x00000004,
    ENUM_DUMMY4WORD(IPH_IME_QUE)
}IPH_IME_QUE;

/**
     enum of IME update selection
*/
typedef enum _IME_UPDATE
{
    IME_IN_Addr     = 0X00000001,   ///< update input addr
    IME_OUT_ADDR_P1 = 0x00000002,   ///< update path1 output addr
    IME_OUT_ADDR_P2 = 0x00000004,   ///< update path2 output addr
    IME_OUT_ADDR_P3 = 0x00000008,   ///< update path3 output addr
    IME_IN_SIZE     = 0X00000010,   ///< update input size/lineoffset
    IME_OUT_SIZE_P1 = 0X00000020,   ///< update path1 output scale size/lineoffset/crop window
    IME_OUT_SIZE_P2 = 0X00000040,   ///< update path2 output scale size/lineoffset/crop window
    IME_OUT_SIZE_P3 = 0X00000080,   ///< update path3 output scale size/lineoffset/crop window
    IME_OUT_FMT_P1  = 0X00000100,   ///< update path1 output format/pack type/Y channel date type/lineoffset
    IME_OUT_FMT_P2  = 0x00000200,   ///< update path2 output format/Y channel date type/lineoffset
    IME_OUT_FMT_P3  = 0x00000400,   ///< update path3 output format/Y channel date type/lineoffset
    IME_SubFEn      = 0X00000800,   ///< update function enable/disable
    IME_PathEn      = 0x00001000,   ///< update path1/path2/path3 enable/disable
    //IME_DMAEn       = 0x00002000,   ///< update p1/p2/p3 dram_out enable/disable
    IME_ChromaA_Addr= 0x00004000,   ///< update chroma adaption input addr
    IME_Retinex_Addr= 0x00008000,   ///< update retinex input addr
    IME_ChromaF_    = 0x00010000,   ///< update chroma filter
    IME_ChromaA_    = 0x00020000,   ///< update chroma adaption(excluding lca addr)
    IME_Retinex_    = 0x00040000,   ///< update retinex(excluding retinex addr)
    IME_CSpaceP_    = 0x00080000,   ///< update color space transfor protection
    IME_RSC_        = 0x00100000,   ///< update rolling shutter correction
    IME_FilmG_      = 0x00200000,   ///< update film grain
    IME_3DNR_Addr   = 0x00400000,   ///< update 3d noise reduction input addr
    IME_3DNR_       = 0X00800000,   ///< update 3d noise reduction(excluding 3dnr addr)
    IME_SCALEQ_Enh_P1   = 0x01000000,   ///< update path1 scale enhancement parameter set
    IME_SCALEQ_Enh_P2   = 0X02000000,   ///< update path2 scale enhancement parameter set
    IME_SCALEQ_Enh_P3   = 0X04000000,   ///< update path3 scale enhancement parameter set
    IME_SCALEQ_F_P1 = 0x08000000,   ///< update path1 scale filter parameter set
    IME_SCALEQ_F_P2 = 0x10000000,   ///< update path2 scale filter parameter set
    IME_SCALEQ_F_P3 = 0x20000000,   ///< update path3 scale filter parameter set
    IME_SCALE_METHOD    = 0x40000000,   ///< update path1/2/3 scale method
    //IME_CSpace_     = 0x04000000,   ///< update color space tranfor
    //IME_P2I_        = 0X40000000,   ///< update P2I
    IME_INIT        = 0x80000000,   ///< init ime
    //IME_par         = 0xf000ffff,   ///< update all parameter excluding IQ relative parameter
    //IME_par         = 0xffffffff,   ///< update all parameter including IQ relative parameter
    ENUM_DUMMY4WORD(IME_UPDATE)
} IME_UPDATE;

/**
     enum of IME read selection

     read from inner table, not from hw register.
*/
typedef enum _IME_READ
{
    IME_R_IO            = 0x00000001,   ///< read input info. exclude IME_SUBFUNC,          Data type: IME
    IME_R_SubFEn        = 0X00000002,   ///< read func enable/disable,                      Data type: ImeFuncEn
    IME_R_PathEn        = 0x00000003,   ///< read path enable/disable.                      Data type: IME_OUT
    IME_R_ChromaF_      = 0x00000004,   ///< read chromaFilter,                             Data type: IME_CHRA_FILT
    IME_R_ChromaA_      = 0X00000005,   ///< read chromaAdaption,                           Data type: IME_CHRA_ADAP
    IME_R_ChromaA_Addr_ = 0x00000006,   ///< read chormaAdaption input address              Data type: UINT32
    IME_R_Retinex_      = 0X00000007,   ///< read RETINEX parameter set,                    Data type: IME_RETINEX
    IME_R_Retinex_Addr_ = 0x00000008,   ///< read Retinex input address                     Data type: UINT32
    IME_R_CSpaceP_      = 0X00000009,   ///< read color space protection,                   Data type: IME_CSPACE_P
    IME_R_RSC_          = 0X00000010,   ///< read rolling shutter correct,                  Data type: IME_RSCorrect
    IME_R_FilmG_        = 0X00000011,   ///< read film grain,                               Data type: IME_FG
    //IME_R_3DNR_         = 0X00000012,   ///< read 3d noise reduction,                       Data type: IME_3D_NR
    IME_R_CSpace_       = 0X00000013,   ///< read color space transfor,                     Data type: IME_CSPACE
    IME_R_SCALEQ_Enh_P1 = 0X00000014,   ///< read path1 scaling enhancement                 Data type: IMESCALE_Enh
    IME_R_SCALEQ_Enh_P2 = 0X00000015,   ///< read path2 scaling enhancement                 Data type: IMESCALE_Enh
    IME_R_SCALEQ_Enh_P3 = 0X00000016,   ///< read path3 scaling enhancement                 Data type: IMESCALE_Enh
    IME_R_SCALEQ_F_P1   = 0X00000017,   ///< read path1 scaling filter                      Data type: IMESCALE_Filt
    IME_R_SCALEQ_F_P2   = 0X00000018,   ///< read path2 scaling filter                      Data type: IMESCALE_Filt
    IME_R_SCALEQ_F_P3   = 0X00000019,   ///< read path3 scaling filter                      Data type: IMESCALE_Filt
    IME_R_Path1Info     = 0x00000020,   ///< read path1 size/fmt/crop                       Data type: OUTCH
    IME_R_Path2Info     = 0x00000021,   ///< read path2 size/fmt/crop                       Data type: OUTCH
    IME_R_Path3Info     = 0x00000022,   ///< read path3 size/fmt/crop                       Data type: OUTCH
    IME_R_Path1Addr     = 0x00000023,   ///< read path1 output address                      Data type: IMEADDR
    IME_R_Path2Addr     = 0x00000024,   ///< read path2 output address                      Data type: IMEADDR
    IME_R_Path3Addr     = 0x00000025,   ///< read path3 output address                      Data type: IMEADDR
    //IME_R_P2I_          = 0X00000020,   ///< read independent va window,                    Data type: BOOL
    IME_R_INTEEn        = 0x00000030,   ///< read interruppt enabe bit,                     Data type: ImeInteEn
    ENUM_DUMMY4WORD(IME_READ)
}IME_READ;

/**
     IME configuration

     all IME configuration including all IME sub function
*/
typedef struct _IME_Manager
{
    enum
    {
        IME_Ctrl_Direct     = 0,    ///< IPE in,Dram out(DCE not embedded in IPE)
        IME_Ctrl_D2D        = 1,    ///< Dram in,Dram out
        IME_Ctrl_Direct_DCE = 4,    ///< dce embedded in IPE mode
        IME_Ctrl_CCIR_Direct= 5,    ///< CCIR input
        ENUM_DUMMY4WORD(IME_Ctrl)
    }IME_Ctrl;                      ///< IME operation mode

    struct
    {
        struct
        {
            BOOL ManualEn;      ///< -
            IME_STRIPE  Horn;   ///< Input image H stripe
            //IME_STRIPE  Ver;    ///< Input image V stripe
        }Manual;
        IME_STPESEL HornSel;    ///< stripe overlap selection
        //IME_STPESEL VerSel;
    }STRP;                      ///< effective when (IME_Ctrl != IME_Ctrl_Direct_DCE)

    struct
    {
        struct
        {
            BOOL    PPBEn;  ///< pinpong buffer mech. enable
            IMEADDR PPB0;   ///< PPB0 address
            IMEADDR PPB1;   ///< -
        }ADDR;              ///< address
        IMESIZE Size;       ///< input size
        IMEFMT Fmt;         ///< input format
    }IME_In;                ///< ime input information

    IME_OUT Ime_Out;        ///< ime output info

    IME_SUBFUNC     *pSubFunc;  ///< -
    //reverse
    //UINT32 rev;                     //reserve for IPH paraset idx
}IME_Manager,*pIME_Manager;

/**
     IME configuration
*/
typedef IME_Manager IME;
//@}
#endif
