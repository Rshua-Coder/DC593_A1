#ifndef IPL_CTRL_OV2710M_TVP5150_CARDV_FFFF_H_
#define IPL_CTRL_OV2710M_TVP5150_CARDV_FFFF_H_

/**
    IPL_CtrlOV2710M_TVP5150_CARDV_FFFF_Int.h


    @file       IPL_CtrlOV2710M_TVP5150_CARDV_FFFF_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

/**
    information for Image-Pipeline
    Add ones if necessary
*/
typedef enum
{
    IPLCTRL_CCIR_EN = IPLCTRL_MAX_CNT_DEF,
    IPLCTRL_MAX_CNT,
    ENUM_DUMMY4WORD(IPLCTRL_IDX)
} IPLCTRL_IDX;

//check IPLCTRL_IDX range
STATIC_ASSERT((IPLCTRL_MAX_CNT <= IPL_CTRL_ELEMENT_MAX));

/**
    set information for Image-Pipeline
    Add ones if necessary
*/
typedef enum
{
    IPLCTRL_CAP_FLOW_NONE = 0,
    IPLCTRL_CAP_FLOW_NORMAL,
    IPLCTRL_CAP_FLOW_HDR,
    IPLCTRL_CAP_FLOW_3D,
    IPLCTRL_CAP_FLOW_HS,
    IPLCTRL_CAP_FLOW_EBURST,
    IPLCTRL_CAP_FLOW_INVID,
    IPLCTRL_CAP_FLOW_AEB,
    IPLCTRL_CAP_FLOW_AWB,
    IPLCTRL_CAP_FLOW_SIM,
    IPLCTRL_CAP_FLOW_MFHDR,
    IPLCTRL_CAP_FLOW_MFANTISHAKE,
    IPLCTRL_CAP_FLOW_CCIR,
    IPLCTRL_CAP_FLOW_DUALSENSOR,
    IPLCTRL_CAP_FLOW_MAX,
    ENUM_DUMMY4WORD(IPLCTRL_CAP_FLOW)
} IPLCTRL_CAP_FLOW;

typedef enum
{
    IPLCTRL_CA_TYPE_32BIT = 0,
    IPLCTRL_CA_TYPE_64BIT,
    IPLCTRL_CA_TYPE_MAX,
    ENUM_DUMMY4WORD(IPLCTRL_CA_TYPE)
}IPLCTRL_CA_TYPE;

typedef enum
{
    IPL_OB_FIX = 0,
    IPL_OB_AUTO,
    ENUM_DUMMY4WORD(IPL_OB_TYPE)
} IPL_OB_TYPE;

typedef struct
{
    SENSOR_INFO *Info;
    IPL_CHGMODE_DATA *ChgMode;
} IPLCTRL_INFO;

typedef struct
{
    UINT32 Param[10];
} IPLCTRL_CAP_EXT_INFO;

void IPL_CtrlInitPrvPProcInfo(UINT32 Id, IPL_POST_PROC_FUNC_EN FuncEn);

void IPL_CtrlInitCapIpcInfo(IPL_GET_CAP_RAW_DATA *CapInfo, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_CtrlInitCapIpcInfo_CCIR(IPL_GET_CAP_RAW_DATA *CapInfo, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_CtrlInitPrvIpcInfo(IPL_PROC_ID Id, IPC_INFO *IpcInfo, IPL_ISR_OBJ_INFO *IsrInfo, IPL_CHGMODE_DATA *ChgMode);
void IPL_CtrlInitPrvIpcInfo_CCIR(IPL_PROC_ID Id, IPC_INFO *IpcInfo, IPL_ISR_OBJ_INFO *IsrInfo, IPL_CHGMODE_DATA *ChgMode);

void IPL_CtrlInitPrvBufInfo(UINT32 Id, IPL_CHGMODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *BufInfo);
void IPL_CtrlInitCapBufInfo(UINT32 Id, IPL_CHGMODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *BufInfo);
void IPL_CtrlInitPrvBufInfo_CCIR(UINT32 Id, IPL_CHGMODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *BufInfo);
void IPL_CtrlInitPrvExtBufInfo(UINT32 Id, IPL_CHGMODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *BufInfo);
void IPL_CtrlPrvInit(UINT32 Id, IPLCTRL_INFO *CtrlInfo);
void IPL_CtrlPrvInit_CCIR(UINT32 Id, IPLCTRL_INFO *CtrlInfo);
void IPL_CtrlCapInit(UINT32 Id, UINT32 EventId, IPLCTRL_INFO *CtrlInfo);
void IPL_CtrlCapInit_CCIR(UINT32 Id, UINT32 EventId, IPLCTRL_INFO *CtrlInfo);

UINT32 IPL_CtrlGetSIECABufInfo(IPLCTRL_CA_TYPE type, UINT32 WinXNum, UINT32 WinYNum);
UINT32 IPL_CtrlGetSIEVABufInfo(void);
UINT32 IPL_CtrlGetIPEVABufInfo(void);

void IPL_CtrlPrvSetDZoomInfo(UINT32 Id);
void IPL_CtrlPrvGetDZoomInfo(UINT32 Id, IPC_Dzoom_Info *Info);
BOOL IPL_ChkImePathInfo(UINT32 Id, IPL_IME_PATH PathId, IPL_IME_PATH_INFO *Info);
void IPL_SetImePathInfo(UINT32 Id, IPL_IME_PATH PathId, IPL_IME_PATH_INFO *Info);
void IPL_GetImePathInfo(UINT32 Id, IPL_IME_PATH PathId, IPL_IME_PATH_INFO *Info);
void IPL_GetImePathORGInfo(UINT32 Id, IPL_IME_PATH PathId, IPL_IME_PATH_INFO *Info);

//sie flow callback
void IPL_SIESetOB_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_OBAVG *OBAvg, SIE_OBSUB *OBSub);
void IPL_SIESetSMR_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, UINT32 *SmrActThres, SIE_SMR **Smr);
void IPL_SIESetCA_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_CACC *cacc);
void IPL_SIESetCAVIG_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_UPDATE *Update, SIE_CA_VIG **cavig);
void IPL_SIESetBP_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *Sub);
void IPL_SIESetDMAOutSrc_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update);
void IPL_SIESetECS_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *subs);

//ipe flow callback
void IPL_IPESetEthOut_FCB(UINT32 Id, IPEFUNCEN *FuncEn, IPEINTE *InteEn, IPE_UPDATE *Update, IPE_SUBFUNC *Sub);
void IPL_IPESetVOut_FCB(UINT32 Id, IPEFUNCEN *FuncEn, IPEINTE *InteEn, IPE_UPDATE *Update, IPE_SUBFUNC *Sub);
void IPL_IPESetCCOut_FCB(UINT32 Id, IPEFUNCEN *FuncEn, IPEINTE *InteEn, IPE_UPDATE *Update, IPE_SUBFUNC *Sub);
void IPL_SIE2CCIR_FCB(UINT32 Id, SIE2__DVI* pDvi);


void IPL_CtrlInitIpc_SIE2In(UINT32 Id, SENSOR_INFO *SenInfo, SIE2_WINDOW *Win);
void IPL_CtrlInitIpc_SIE2Signal(UINT32 Id, SENSOR_INFO *SenInfo, UINT32 HDInv, UINT32 VDInv, UINT32 VDPhase, UINT32 HDPhase, UINT32 DPhase, IPP_SIE2_SIGNAL *Signal);
void IPL_CtrlInitIpc_SIE2Ch0PPB(UINT32 Id, UINT32 *PP1Addr, UINT32 *PP2Addr);

void IPL_CtrlInitIpc_SIE2In_Cap(UINT32 Id,  SIE2_UPDATE *Update, SIE2_WINDOW *Win);
void IPL_CtrlInitIpc_SIE2Signal_Cap(UINT32 Id, SIE2_UPDATE *Update, UINT32 HDInv, UINT32 VDInv, UINT32 VDPhase, UINT32 HDPhase, UINT32 DPhase, IPP_SIE2_SIGNAL *Signal);
void IPL_CtrlInitIpc_SIE2Ch0PPB_Cap(UINT32 Id, SIE2_UPDATE *Update, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV);

void IPL_CtrlInitIpc_SIETrigSel_Cap(UINT32 Id, SIE_UPDATE *Update, SIEPreTrigSel *TrigSel);
void IPL_CtrlInitIpc_SIEBurstLength_Cap(UINT32 Id, SIE_UPDATE *Update, SENSOR_INFO *SenInfo, SIE_BURST_LENG *Length);
void IPL_CtrlInitIpc_SIEAct_Cap(UINT32 Id, SIE_UPDATE *Update, SieACTMode *ActMode);
void IPL_CtrlInitIpc_SIESignal_Cap(UINT32 Id, SIE_UPDATE *Update, UINT32 HDInv, UINT32 VDInv, UINT32 VDPhase, UINT32 HDPhase, UINT32 DPhase, IPP_SIE_SIGNAL *Signal);
void IPL_CtrlInitIpc_SIECh0PPB_Cap(UINT32 Id, SIE_UPDATE *Update, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV);
void IPL_CtrlInitIpc_SIECh2PPB_Cap(UINT32 Id, SIE_UPDATE *Update, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV);
void IPL_CtrlInitIpc_SIEIn_Cap(UINT32 Id, SIE_UPDATE *Update, SIE_WINDOW *Win);
void IPL_CtrlInitIpc_SIEOB_Cap(UINT32 Id, SIE_UPDATE *Update, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_OBAVG *OBAvg, SIE_OBSUB *OBSub);
void IPL_CtrlInitIpc_SIEGamma_Cap(UINT32 Id,  SIE_UPDATE *Update, SieFuncEn *FuncEn, SIE_GAMMA **pGamma);
void IPL_CtrlInitIpc_SIERawEnc_Cap(UINT32 Id,  SIE_UPDATE *Update, SieFuncEn *FuncEn, SIE_RAWENC *RawEnc, UINT32 MaxBufSize, UINT32 Ratio);

void IPL_CtrlInitIpc_SIEAct(UINT32 Id, SENSOR_INFO *SenInfo, SieACTMode *ActMode);
void IPL_CtrlInitIpc_SIEBurstLength(UINT32 Id, SENSOR_INFO *SenInfo, SIE_BURST_LENG *Length);
void IPL_CtrlInitIpc_SIEIn(UINT32 Id, SENSOR_INFO *SenInfo, SIE_WINDOW *Win);
void IPL_CtrlInitIpc_SIESignal(UINT32 Id, SENSOR_INFO *SenInfo, UINT32 HDInv, UINT32 VDInv, UINT32 VDPhase, UINT32 HDPhase, UINT32 DPhase, IPP_SIE_SIGNAL *Signal);
void IPL_CtrlInitIpc_SIECh0PPB(UINT32 Id, IPL_RAW_IMG_INFO *PPB0, UINT32 *PP1Addr, UINT32 *PP2Addr);
void IPL_CtrlInitIpc_SIECh2PPB(UINT32 Id, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV);
void IPL_CtrlInitIpc_SIEVaPPB(UINT32 Id, SIE_Dma_Out *Out);
void IPL_CtrlInitIpc_SIECaPPB(UINT32 Id, SIE_CH1_Dma_Out *DmaOut);
void IPL_CtrlInitIpc_PREInPPB(UINT32 Id, IPL_RAW_IMG_INFO *PPB0, UINT32 *PP1Addr);
void IPL_CtrlInitIpc_DCEIn(UINT32 Id, SENSOR_INFO *SenInfo, USIZE* pRef_1x, Coordinate *pCenter_1x);
void IPL_CtrlInitIpc_IPELcaPPB(UINT32 Id, UINT32 *PP0Addr, UINT32 *PP1Addr, UINT32 *PP2Addr, UINT32 *PP3Addr);
void IPL_CtrlInitIpc_IPEVPPB(UINT32 Id, UINT32 *PP0Addr, UINT32 *PP1Addr, UINT32 *PP2Addr, UINT32 *PP3Addr);
void IPL_CtrlInitIpc_IPEVaPPB(UINT32 Id, UINT32 *PP0Addr, UINT32 *PP1Addr);
void IPL_CtrlInitIpc_IFE2LcaPPB(UINT32 Id, UINT32 *PP0Addr, UINT32 *PP1Addr);
void IPL_CtrlInitIpc_IMEPxOutPPB(UINT32 Id, UINT32 PathEnIdx, IPL_YCC_IMG_INFO *Px_PPB0);
void IPL_CtrlInitIpc_DispPxPPB(UINT32 Id, UINT32 PathEnIdx);
void IPL_CtrlInitIpc_DispCtrlEn(UINT32 Id, UINT32 PathEnIdx, UINT32 EnFlag);

IPL_HAL_PARASET IPL_CtrlGetHalSetId(UINT32 Id);
IPL_HAL_PARASET IPL_CtrlGetHalSetIdPProc(UINT32 Id);
#endif //IPL_CTRL_OV2710M_TVP5150_CARDV_FFFF_H_
