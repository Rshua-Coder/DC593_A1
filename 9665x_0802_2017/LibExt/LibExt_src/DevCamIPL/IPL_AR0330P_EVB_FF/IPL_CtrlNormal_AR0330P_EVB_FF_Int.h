#ifndef IPL_CTRLNORMAL_AR0330P_EVB_FF_H_
#define IPL_CTRLNORMAL_AR0330P_EVB_FF_H_

/**
    IPL_CtrlAR0330P_EVB_FFNormal_Int.h


    @file       IPL_CtrlAR0330P_EVB_FFNormal_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
typedef struct
{
    IPL_HAL_PARASET IPL_HalParamId[IPL_ID_MAX_NUM];
    fpEngInit fp_IFE2Init;
    fpEngInit fp_IFE2EngSetting;
    fpEngSubFunc fp_IpeCsub;
    fpEngSubFunc fp_IpeVsub;
    fpEngCLK fp_SIEClk;
    fpSIEPclkPad fp_SIEPclkSel;
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
} IPLCTRL_FLOWA_INFO;

void IPL_CtrlInitIpcInfoFlowA(UINT32 Id, IPC_INFO *IpcInfo);
void IPL_CtrlInitBufInfoFlowA(UINT32 Id, IPLBUF_ENG2DRAMINFO *BufInfo);
void IPL_CtrlPrvInitFlowA(UINT32 Id, IPLCTRL_INFO *CtrlInfo);
IPL_HAL_PARASET IPL_CtrlGetHalSetIdFlowA(UINT32 Id);
#endif //IPL_CTRLNORMAL_AR0330P_EVB_FF_H_
