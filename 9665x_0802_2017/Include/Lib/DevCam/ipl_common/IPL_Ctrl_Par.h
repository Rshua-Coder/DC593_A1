/**
    IPL_Ctrl module(type define)

    @file       IPL_Ctrl_Par.h
    @ingroup    mILibIPH
    @note

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _IPL_CTRL_PAR_H_
#define _IPL_CTRL_PAR_H_
#include "IPL_Hal.h"
/** \addtogroup mILibIPH */
//@{

/**
     callback when pipeline output image dma ready

     Detailed description.
     @note (Optional)

     @param[in]     ImgIdx      source index of current output image during this streaming.
     @param[in]     fp_CB       pointer of PostProc. synchronization function
     @param[in,out] path1       in:current ready img addr and size. out: next frame output addr.
     @param[in,out] path2       in:current ready img addr and size. out: next frame output addr.
     @param[in,out] path3       in:current ready img addr and size. out: next frame output addr.
     @return Description: none
*/
typedef void (*fp_ImgRdy)(UINT32 ImgIdx,                                       \
    IPL_YCC_IMG_INFO* path1,IPL_YCC_IMG_INFO* path2,IPL_YCC_IMG_INFO* path3,fpCallBack fp_CB);
typedef void (*fp_ImgRdy2)(UINT32 ImgIdx,                                       \
    IPL_YCC_IMG_INFO* path1,IPL_YCC_IMG_INFO* path2);
typedef IPH_CLK (*fpEngCLK)(void);
typedef UINT32 (*fpSIEPclkPad)(void);
typedef UINT32 (*fpEngInit)(ENG_SUBFUNC*,IPL_HAL_GROUP*);//eng_subfunc and IPL_HAL_GROUP is output
typedef BOOL (*fpRegDump)(UINT32);
typedef UINT32(*fpccirFmt)(SIE2__DVI*);

/*
    return type is cascaded to void* from indicated SubFunction
    ex:Ime adaption is from IME_CHRA_ADAP* to void*
*/
typedef void*   (*fpEngSubFunc)(BOOL *bFuncEn);//bFuncEn is output


/**
     optional structure of ICF_FLOW

     this sturcture is optional configuration,ipl can start without these configuration.
     @note:
\n      fp_DceInit can't be NULL!! Because of hw stripe constraint.
\n      fp_SIEClk can't be NULL when sensor interface is LVDS!!  because LVDS + SIE hw constraint!
\n      if ime path1 and path3 need to be enabled during IPL running, IPL must be initialized with ime p1 and p3 output address
*/
typedef struct _IPLC_EXT
{
    struct _SIE_CH2
    {
        CH2_SRC Src;            ///< ch2 output format
        union
        {
            SIE_Dma_Out VAout;      ///< VA output info
            SIE_Dma_Out SubImgout;  ///< subimage info
        }Out;                   ///< -
    }SIE_CH2;                   ///< sie output ch2
    BOOL             IME_P1_ENABLE;         ///< enable path1 dma output,path1 and path3 is optional,default is disable
    BOOL             IME_P3_ENABLE;         ///< enable path3 dma output
    IPL_YCC_IMG_INFO IME_P1_PPB0;           ///< ppb0 output address
    IPL_YCC_IMG_INFO IME_P3_PPB0;           ///< ppb0 output address
    fpEngCLK fp_SIEClk;                     ///< engine clk CB, default clk will be applied to when CB is NULL!
    fpSIEPclkPad fp_SIEPclkSel;             ///< sie engine pclk selection CB
    fpEngCLK fp_PREClk;                     ///< -
    fpEngCLK fp_IFEClk;                     ///< -
    fpEngCLK fp_DCEClk;                     ///< -
    fpEngCLK fp_IPEClk;                     ///< -
    fpEngCLK fp_IFE2Clk;                    ///< -
    fpEngCLK fp_IMEClk;                     ///< -
    fpEngInit fp_SieInit;                   ///< engine sub-func initial CB, default cfg will be applied to when CB is NULL!
    fpEngInit fp_PreInit;                   ///< -
    fpEngInit fp_IfeInit;                   ///< -
    fpEngInit fp_DceInit;                   ///< -
    fpEngInit fp_IpeInit;                   ///< -
    fpEngInit fp_ImeInit;                   ///< -
}IPLC_EXT;

/**
     basic configuration of ICF_FLOW

     these configuration must be fulfilled, and address can't be 0!
     @note (Optional)
*/
typedef struct _IPLC_BASEINFO
{
    SieACTMode       SIE_Act;           ///< sie and sensor communication interface
    SIE_WINDOW       SIE_IN;            ///< sie input window
    IPP_SIE_SIGNAL   SenSorIn;          ///< sensor input format
    SIE_BURST_LENG   Sie_BL_Ch0;        ///< sie ch0 dram burst length
    IPL_RAW_IMG_INFO SIE_PPB0;          ///< out
    UINT32           SIE_PPB1_ADDR;     ///< out
    CH0_SRC          SIE_Src;           ///< sie output data source

    IPL_RAW_IMG_INFO PRE_PPB0;          ///< pre dma in
    UINT32           PRE_PPB1_ADDR;     ///< pre dma in

    USIZE            DCE_Ref_1X;        ///< sensor output size(for dce reference)
    Coordinate       DCE_Ref_1XCenter;  ///< distortion center of sensor output
    UINT32           IPE_VA_ADDR;       ///< ipe va dma output address
    UINT32           IPE_VA_ADDR1;      ///< ipe va dma ppb address

    IPL_YCC_IMG_INFO IME_P2_PPB0;       ///< ime path2 dma output info.path2 always enable,for disp

}IPLC_BASEINFO,*pIPLC_BASEINFO;

/**
     configuration for IPL calibration mode

     this structure only refer to IPL calibration mode!
*/
typedef struct _IPLC_INFO
{
    IPLC_BASEINFO   *pBaseInfo;     ///< -
    IPLC_EXT        *pExtInfo;      ///< -
    SIE_CH1_Dma_Out  CA_PPB;        ///< -

    SIE_UPDATE      *psieUpdate;    ///< cap only
    SIE             *psie;          ///< cap only
}IPLC_INFO,*pIPLC_INFO;

//@}
#endif