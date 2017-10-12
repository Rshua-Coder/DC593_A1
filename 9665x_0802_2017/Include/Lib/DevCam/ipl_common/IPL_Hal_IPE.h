/**
    IPH module (IPE type define)

    @file       IPL_Hal_IPE.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_IPE_H
#define _IPL_Hal_IPE_H
#include "IPL_Hal_IPE_Info.h"
#include "IPL_ISREvent.h"
/** \addtogroup mILibIPH */
//@{

/**
    query enum

    read from hw register
*/
typedef enum _IPH_IPE_QUE
{
    //IPE_Q_CSubRdyID       = 0x00000001,
    //IPE_Q_VSubRdyID       = 0x00000002,
    IPE_Q_EthOutRdyID       = 0x00000003,   ///< Eth out current ready ID,                          Data type: UINT32
    IPE_VA_DMA_RST          = 0x00000004,   ///< read VA (DMA)result                                Data type: IPE_VA_DramRst
    IPE_VA_INDWIN_RST       = 0x00000005,   ///< read VA (Indepent window)result                    Data type: IPE__VA_IND_RST
    ENUM_DUMMY4WORD(IPH_IPE_QUE)
}IPH_IPE_QUE;

/**
     enum of IPE update selection
*/
typedef enum _IPE_UPDATE
{
    IPE_IN_ADDR     = 0x00000001,   ///< update input addr
    IPE_OUT_ADDR    = 0x00000002,   ///< update output addr
    IPE_IN_SIZE     = 0x00000004,   ///< update input size/lineofs
    IPE_OUT_SIZE    = 0x00000008,   ///< update output lineofs(dma out only)
    IPE_SubFEn      = 0x00000010,   ///< update func enable/disable
    IPE_VA_         = 0x00000020,   ///< update va detection/accumulation
    IPE_INDVA_      = 0x00000040,   ///< update independent va window
    IPE_THRES_      = 0x00000080,   ///< update edge output(dma out) threshold(excluding IPE_THRES_Addr)
    IPE_THRES_Addr  = 0x00000100,   ///< update edge output(dma out) address,lineofs,height
    IPE_CG_Gain_    = 0x00000200,   ///< update color gain
    IPE_CG_Ofs_     = 0x00000400,   ///< update offset(bayer stage)
    IPE_CG_GEffect_ = 0x00000800,   ///< update color gain output inverse/mask
    IPE_GBal_       = 0x00001000,   ///< update g channel balance
    IPE_CFAinter_   = 0x00002000,   ///< update cfa interpolation
    IPE_EdgeExt_    = 0x00004000,   ///< update edge extraction
    IPE_EdgeEnh_    = 0x00008000,   ///< update positive/negative edge enhance/inverse
    //IPE_EdgeInv_    = 0x00010000,   ///< update positive/negative edge inverse
    IPE_EdgeMap_    = 0x00010000,   ///< update edge map
    IPE_RGBLpf_     = 0x00020000,   ///< update rgb low pass
    IPE_CC_         = 0x00040000,   ///< update color correction
    IPE_CST_        = 0X00080000,   ///< update color space transfor
    IPE_C_Ctrl_     = 0x00100000,   ///< update color control
    IPE_C_Con_      = 0x00200000,   ///< update color contrast(Y/CC)
    IPE_C_Ofs_      = 0x00400000,   ///< update Cb/Cr offset
    IPE_RANDNoise_  = 0x00800000,   ///< update YCC random noise
    IPE_YCCMask_    = 0x01000000,   ///< update YCC mask
    IPE_YCCFix_     = 0x02000000,   ///< update YCC fix replace
    IPE_CSTP_       = 0X04000000,   ///< update color space transfor protection
    IPE_3DCC_       = 0X08000000,   ///< update 3d color correction dram addr
    IPE_RGBGAMMA_   = 0X10000000,   ///< update rgb gamma dram addr
    IPE_CSUB_       = 0x20000000,   ///< update CbCr subsample output,size/addr
    IPE_VSUB_       = 0x40000000,   ///< update V subsample output,size/addr
    IPE_INIT        = 0x80000000,   ///< init ipe
    //IPE_par         = 0x8000001f,   ///< update all parameter excluding IQ relative parameter
    //IPE_par         = 0xffffffff,   ///< update all parameter including IQ relative parameter
    ENUM_DUMMY4WORD(IPE_UPDATE)
} IPE_UPDATE;

/**
     enum of IPE read selection

     read from inner table, not from hw register.
*/
typedef enum _IPE_READ
{
    IPE_R_IO        = 0x00000001,   ///< read in/out info. exclude IPE_SUBFUNC,         Data type: IPE
    IPE_R_SubFEn    = 0X00000002,   ///< read func enable/disable,                      Data type: IPEFUNCEN
    IPE_R_VA_       = 0x00000003,   ///< read va detection/accumulation,                Data type: IPE__VA
    IPE_R_INDVA_    = 0X00000004,   ///< read independent va window,                    Data type: IPE__VA_IND
    IPE_R_VA_WIN    = 0x00000005,   ///< read va window                                 Data type: VA_WIN
    IPE_R_ETH_THRES_= 0x00000006,   ///< read edge threshold output,threshold only      Data type: IPE_EDGETHRE
    IPE_R_ETH_ADDR_ = 0x00000007,   ///< read edge threshold output,addr,lineofs,V only Data type: IPE_ETH_DMA
    IPE_R_CG_Gain_  = 0x00000008,   ///< read color gain/offset/mask,                   Data type: IPE_CG
    //IPE_R_CG_Ofs_   = 0X00000008,   ///< read offset(bayer stage),                      Data type: IPE_CG
    //IPE_R_CG_Inv_   = 0X00000009,   ///< read color gain output inverse,                Data type: IPE_CG
    //IPE_R_CG_Mask_  = 0x00000020,   ///< read gain output mask,                         Data type: IPE_CG
    IPE_R_GBal_     = 0x00000009,   ///< read g channel balance,                        Data type: IPE_GBAL
    IPE_R_CFAinter_ = 0x00000010,   ///< read cfa interpolation,                        Data type: IPE_CFAINTER
    IPE_R_EdgeExt_  = 0x00000011,   ///< read edge extraction,                          Data type: IPE_EdgeExt
    IPE_R_EdgeEnh_  = 0x00000012,   ///< read positive/negative edge enhance/inverse,   Data type: IPE_EdgeEnh
    IPE_R_EdgeMap_  = 0x00000013,   ///< read edge map,                                 Data type: IPE_EdgeMap
    IPE_R_RGBLpf_   = 0x00000014,   ///< read rgb low pass,                             Data type: IPE_RGBLPF
    IPE_R_CC_       = 0x00000015,   ///< read color correction,                         Data type: IPE_CC
    IPE_R_CST_      = 0x00000016,   ///< read color space transfor,                     Data type: IPE_CST
    IPE_R_C_Ctrl_   = 0x00000017,   ///< read color control,                            Data type: IPE_CCTRL
    IPE_R_C_Con_    = 0x00000018,   ///< read color contrast(Y/CC),                     Data type: IPE_CCON
    IPE_R_C_Ofs_    = 0x00000019,   ///< read Cb/Cr offset,                             Data type: IPE_CCOFS
    IPE_R_RANDNoise_= 0x00000020,   ///< read YCC random noise,                         Data type: IPE_RAND_NR
    IPE_R_YCCMask_  = 0x00000021,   ///< read YCC mask,                                 Data type: IPE_YCCMASK
    IPE_R_YCCFix_   = 0x00000022,   ///< read YCC fix replace,                          Data type: IPE_YCCFIX
    IPE_R_CSTP_     = 0x00000023,   ///< read color space transfor protection,          Data type: IPE_CSTP
    IPE_R_3DCC_     = 0x00000024,   ///< read 3d color correction dram addr,            Data type: UINT32
    IPE_R_RGBGAMMA_ = 0x00000025,   ///< read rgb gamma dram addr,                      Data type: UINT32
    IPE_R_CSUB_     = 0x00000026,   ///< read CbCr subsample output,                    Data type: IPE_CC_SUBOUT
    IPE_R_VSUB_     = 0x00000027,   ///< read V subsample output,                       Data type: IPE_V_SUBOUT
    IPE_R_INTEEn    = 0x00000028,   ///< read interruppt enabe bit,                     Data type: IPEINTE
    IPE_R_VAADDR    = 0x00000029,   ///< read VA current address,                       Data type: UINT32
    //IPE_R_EthOutRdyAddr = 0x00000900,   ///< Eth out current ready address,,IPL_HAL_PARASET don't care. Data type:UINT32

    ENUM_DUMMY4WORD(IPE_READ)
}IPE_READ;


/**
    IPE VA PPB Set selection
*/
typedef enum _IPE_PPBSet_Sel
{
    IPE_VA_SetCurAddr = 0,
    IPE_VA_SetAvlbAddr = 1,
    ENUM_DUMMY4WORD(IPE_PPBSet_SEL)
}IPE_PPBSet_SEL;


/**
     IPE configuration

     all IPE configuration including all IPE sub function
*/
typedef struct _IPE_Manager
{
    struct
    {
        IPESRC IPE_Src;         ///< ipe input source

        enum
        {
            IPE_MSTRP   = 0,    ///< Multi stripe mode,according to HN, HL, HM, VN, VL, VM setting
            IPE_ASTRP   = 1,    ///< Auto stripe mode,calculates stripe sizes automatically
            IPE_DVI     = 2,    ///< DVI mode
            ENUM_DUMMY4WORD(IPEMode)
        }IPEMode;               ///< ipe operation mode

        struct
        {
            struct
            {
                //BOOL ManualEn;      ///< -
                IPE_STRIPE  Horn;   ///< Input image H stripe
                //IPE_STRIPE  Ver;    ///< Input image V stripe
            }Manual;
            IPE_STPESEL HornSel;    ///< overlap selection
            //IPE_STPESEL VerSel;
        }STRP;                      ///< effective when IPEMode == IPE_MSTRP && EmbeddedDCEEn == FALSE

        IPESIZE Size;               ///< in/out size
        IPE_PackBus packBus;        ///< input data bit-depth
        IPE_CFA Cfa;                ///< input cfa
        struct
        {
            BOOL    PPBEn;          ///< pinpong buffer ctrl enable
            UINT32  PPB0;           ///< address of PPB0
            UINT32  PPB1;           ///< address of PPB1
        }ADDR;                      ///< dram input
    }IPE_In;

    struct
    {
        BOOL    Output2IMEEn;       ///< main img output to ime directly
        struct
        {
            BOOL    Output2DMAEn;   ///< main img output to dram
            IPEOutFmt IPEFmt;       ///< output format
            IPEDATADROP YCCDropSel; ///< YCC sub-sample selection
            //addr
            BOOL    PPBEn;          ///< pinpong buffer ctrl enable
            UINT32  PPB0_Y;         ///< PPB0 Y address
            UINT32  PPB0_UV;        ///<      UV address
            UINT32  PPB1_Y;         ///< -
            UINT32  PPB1_UV;        ///< -
            //lineoffset
            UINT32  Lineofs_Y;      ///< -
            UINT32  Lineofs_UV;     ///< -
        }DMAOut;                    ///< dram output
    }IPE_Out;                       ///< ipe output information
    BOOL    EmbeddedDCEEn;          ///< enable DCE embedded in IPE

    IPE_SUBFUNC *pSubFunc;          ///< -
    //reverse
    UINT32 rev;                     //reserve for IPH paraset idx
}IPE_Manager,*pIPE_Manager;

/**
     IPE configuration
*/
typedef IPE_Manager IPE;

//@}
#endif
