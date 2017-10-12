#ifndef IPL_CTRLFWD_OV2710M_TVP5150_CARDV_FFFF_INT_H_
#define IPL_CTRLFWD_OV2710M_TVP5150_CARDV_FFFF_INT_H_

/**
    IPL_CtrlFWD_OV2710M_TVP5150_CARDV_FFFF_Int.h


    @file       IPL_CtrlFWD_OV2710M_TVP5150_CARDV_FFFF_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
typedef struct
{
    IPL_HAL_PARASET IPL_HalParamId[IPL_ID_MAX_NUM];
    fpEngInit fp_IFE2Init;
    fpEngInit fp_IFE2EngSetting;
    fpEngSubFunc fp_AdaptSetting;
    fpEngSubFunc fp_IpeCsub;
    fpEngSubFunc fp_IpeVsub;
    fpEngCLK fp_SIEClk;
    fpEngCLK fp_SIE2Clk;
    fpSIEPclkPad fp_SIEPclkSel;
    fpSIEPclkPad fp_SIE2PclkSel;
    fpEngCLK fp_PREClk;
    fpEngCLK fp_IFEClk;
    fpEngCLK fp_DCEClk;
    fpEngCLK fp_IPEClk;
    fpEngCLK fp_IFE2Clk;
    fpEngCLK fp_IMEClk;
    fpEngInit fp_SieInit;
    fpEngInit fp_PreInit;
    fpEngInit fp_IfeInit;
    fpEngInit fp_DceInit;
    fpEngInit fp_IpeInit;
    fpEngInit fp_ImeInit;
    fpccirFmt fp_ccirFmt;
} IPLCTRL_FLOWD_INFO;

void IPL_CtrlInitIpcInfoFlowD(UINT32 Id, IPC_INFO *IpcInfo);
void IPL_CtrlInitIpcInfoFlowD_CCIR(UINT32 Id, IPC_INFO *IpcInfo);
void IPL_CtrlInitBufInfoFlowD(UINT32 Id, IPLBUF_ENG2DRAMINFO *BufInfo);
void IPL_CtrlInitBufInfoFlowD_CCIR(UINT32 Id, IPLBUF_ENG2DRAMINFO *BufInfo);
void IPL_CtrlPrvInitFlowD(UINT32 Id, IPLCTRL_INFO *CtrlInfo);
void IPL_CtrlPrvInitFlowD_CCIR(UINT32 Id, IPLCTRL_INFO *CtrlInfo);
IPL_HAL_PARASET IPL_CtrlGetHalSetIdFlowD(UINT32 Id);
#endif //IPL_CTRLFWD_OV2710M_TVP5150_CARDV_FFFF_H_
