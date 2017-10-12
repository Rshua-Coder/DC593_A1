/**
    Ctrl fw AR0330_TVP5150_CARDV_FFFF.

    @file       IPL_CtrlAR0330_TVP5150_CARDV_FFFF.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "IPL_AR0330_TVP5150_CARDV_FFFF_Int.h"

UINT32 IPL_IFE2InitCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IFE2EngCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
void* IPL_AdaptSettingCBFlowC(BOOL* bAdaptEn);
void* IPL_IpeCsubCBFlowC(BOOL *bFuncEn);
void* IPL_IpeVsubCBFlowC(BOOL *bFuncEn);
IPH_CLK IPL_SIEClkCBFlowC(void);
UINT32 IPL_SIEPclkSelCBFlowC(void);
IPH_CLK IPL_PREClkCBFlowC(void);
IPH_CLK IPL_IFEClkCBFlowC(void);
IPH_CLK IPL_DCEClkCBFlowC(void);
IPH_CLK IPL_IPEClkCBFlowC(void);
IPH_CLK IPL_IFE2ClkCBFlowC(void);
IPH_CLK IPL_IMEClkCBFlowC(void);
UINT32 IPL_SIEInitCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_PREInitCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IFEInitCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_DCEInitCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IPEInitCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IMEInitCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);

IPLCTRL_FLOWC_INFO IPL_FlowCInfo =
{
    {IPL_HAL_PAR0, IPL_HAL_PAR1},

    IPL_IFE2InitCBFlowC,
    IPL_IFE2EngCBFlowC,
    IPL_AdaptSettingCBFlowC,
    IPL_IpeCsubCBFlowC,
    IPL_IpeVsubCBFlowC,
    IPL_SIEClkCBFlowC,
    IPL_SIEPclkSelCBFlowC,
    IPL_PREClkCBFlowC,
    IPL_IFEClkCBFlowC,
    IPL_DCEClkCBFlowC,
    IPL_IPEClkCBFlowC,
    IPL_IFE2ClkCBFlowC,
    IPL_IMEClkCBFlowC,

    IPL_SIEInitCBFlowC,
    IPL_PREInitCBFlowC,
    IPL_IFEInitCBFlowC,
    IPL_DCEInitCBFlowC,
    IPL_IPEInitCBFlowC,
    IPL_IMEInitCBFlowC,
};

static IPLCTRL_FLOWC_INFO* IPL_GetCtrlFlowCInfo(void)
{
    return &IPL_FlowCInfo;
}

IPL_HAL_PARASET IPL_CtrlGetHalSetIdFlowC(UINT32 Id)
{
    return IPL_GetCtrlFlowCInfo()->IPL_HalParamId[Id];
}

UINT32 IPL_IFE2InitCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    UINT32 Cnt;
    //DBG_ERR("IPL_IFE2InitCBFlowC\r\n");
    IQCB_SettingFlowFunc(IQCB_PRV_IFE2_INIT_CC, Info, group);
    Cnt = IQS_GetIFE2LcaTime(IQCB_PRV_IFE2_INIT_CC);
    return Cnt;
}
UINT32 IPL_IFE2EngCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    //DBG_ERR("IPL_IFE2EngCBFlowC\r\n");
    IQCB_SettingFlowFunc(IQCB_PRV_IFE2_CC, Info, group);
    return 1;
}


void* IPL_AdaptSettingCBFlowC(BOOL* bAdaptEn)
{
    ENG_SUBFUNC Info;
    IME_SUBFUNC Ime_sub = {0};
    IPL_HAL_GROUP group;

    //DBG_ERR("IPL_AdaptSettingCBFlowC\r\n");
    Info.pIme_sub = &Ime_sub;
    IQCB_SettingFlowFunc(IQCB_PRV_IME_LCA, &Info, &group);
    if ( (Info.pIme_sub->imeFuncEn & ChromaAdaptEn) != 0 )
    {
        *bAdaptEn = TRUE;
        return (void*)Info.pIme_sub->ChromaAdap.pChromaAdaption;
    }
    *bAdaptEn = FALSE;
    return NULL;
}

void* IPL_IpeCsubCBFlowC(BOOL *bFuncEn)
{
    *bFuncEn = FALSE;
    return NULL;
}

void* IPL_IpeVsubCBFlowC(BOOL *bFuncEn)
{
    *bFuncEn = FALSE;
    return NULL;
}

#if 0
#endif
IPH_CLK IPL_SIEClkCBFlowC(void)
{
    IPH_CLK clk;
    SENSOR_INFO SenInfo;

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(IPL_ID_1), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &SenInfo);

    if ((SenInfo.DataType == SENSOR_DATA_LVDS) || (SenInfo.DataType == SENSOR_DATA_MIPI))
    {
        clk.SIEclk = 0;
    }
    else if (SenInfo.DataType == SENSOR_DATA_DUMMY)
    {
        clk.SIEclk = 27000000;
    }
    else
    {
        clk.SIEclk = 240000000;
        DBG_ERR("SIEclk = %d\r\n", clk.SIEclk);
    }
    return clk;
}

UINT32 IPL_SIEPclkSelCBFlowC(void)
{
    return IPH_SIEPXCLK_OFF;
}

IPH_CLK IPL_PREClkCBFlowC(void)
{
    IPH_CLK clk;
    clk.PREclk = IPH_PRECLK6;
    return clk;
}

IPH_CLK IPL_IFEClkCBFlowC(void)
{
    IPH_CLK clk;
    clk.IFEclk = IPH_IFECLK6;
    return clk;
}

IPH_CLK IPL_DCEClkCBFlowC(void)
{
    IPH_CLK clk;
    clk.DCEclk = IPH_DCECLK6;
    return clk;
}

IPH_CLK IPL_IPEClkCBFlowC(void)
{
    IPH_CLK clk;
    clk.IPEclk = IPH_IPECLK6;
    return clk;
}

IPH_CLK IPL_IFE2ClkCBFlowC(void)
{
    IPH_CLK clk;
    clk.IFE2clk = IPH_IFE2CLK6;
    return clk;
}

IPH_CLK IPL_IMEClkCBFlowC(void)
{
    IPH_CLK clk;
    clk.IMEclk = IPH_IMECLK6;
    return clk;
}
#if 0
#endif

UINT32 IPL_SIEInitCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    SIE_UPDATE *Update;

    //init info structure
    Update = (SIE_UPDATE *)group;

    IPL_SIESetOB_FCB(IPL_ID_1, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, Update, &Info->pSie_sub->OBAvg, &Info->pSie_sub->OBSub);
    IPL_SIESetSMR_FCB(IPL_ID_1, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, Update, &Info->pSie_sub->OBAvg.SmrActThres, &Info->pSie_sub->pSmr);
    IPL_SIESetCA_FCB(IPL_ID_1, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, Update, &Info->pSie_sub->Ca);
    IPL_SIESetCAVIG_FCB(IPL_ID_1, &Info->pSie_sub->sieFuncEn, Update, &Info->pSie_sub->pVig);
    IPL_SIESetBP_FCB(IPL_ID_1, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, Update, Info->pSie_sub);
    IPL_SIESetDMAOutSrc_FCB(IPL_ID_1, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, Update);
    IPL_SIESetECS_FCB(IPL_ID_1,&Info->pSie_sub->sieFuncEn,&Info->pSie_sub->InteEn,Update,Info->pSie_sub);
    *Update |= SIE_SubFEn;

    IQCB_SettingFlowFunc(IQCB_PRV_SIE_INIT, Info, group);
    return 1;
}

UINT32 IPL_PREInitCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IQCB_SettingFlowFunc(IQCB_PRV_PRE_INIT, Info, group);
    return 1;
}

UINT32 IPL_IFEInitCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IQCB_SettingFlowFunc(IQCB_PRV_IFE_INIT, Info, group);
    return 1;
}

UINT32 IPL_DCEInitCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IQCB_SettingFlowFunc(IQCB_PRV_DCE_INIT, Info, group);
    return 1;
}

UINT32 IPL_IPEInitCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IPE_UPDATE *Update;

    //init info structure
    Update = (IPE_UPDATE *)group;

    IQCB_SettingFlowFunc(IQCB_PRV_IPE_INIT, Info, group);
    IPL_IPESetEthOut_FCB(IPL_ID_1, &Info->pIpe_sub->IpeFuncEn, &Info->pIpe_sub->IpeInteEn, Update, Info->pIpe_sub);
    IPL_IPESetCCOut_FCB(IPL_ID_1, &Info->pIpe_sub->IpeFuncEn, &Info->pIpe_sub->IpeInteEn, Update, Info->pIpe_sub);
    return 1;
}

UINT32 IPL_IMEInitCBFlowC(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    static IME_RSCorrect RSC = {0};

    if (IPL_AlgGetUIInfo(IPL_SEL_3DNR) == SEL_3DNR_ON)
    {
        Info->pIme_sub->imeFuncEn |= IME3DNREn;
    }
    IQCB_SettingFlowFunc(IQCB_PRV_IME_INIT, Info, group);

    if (IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_RSC_EN) == ENABLE)
    {
        Info->pIme_sub->pRsc = &RSC;
        Info->pIme_sub->pRsc->MarginH = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_RSC_RATIO);
        Info->pIme_sub->pRsc->MarginV = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_RSC_RATIO);
        group->imeUpdate |= IME_RSC_;
    }
    return 1;
}

#if 0
#endif
void IPL_CtrlInitIpcInfoFlowC(UINT32 Id, IPC_INFO *IpcInfo)
{
    SENSOR_INFO SenInfo;
    IPLCTRL_FLOWC_INFO *FlowInfo;

    FlowInfo = IPL_GetCtrlFlowCInfo();

    IpcInfo->ICF_FC.ParaIdx = FlowInfo->IPL_HalParamId[Id];

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &SenInfo);

    IPL_CtrlInitIpc_SIEBurstLength(Id, &SenInfo, &IpcInfo->ICF_FC.BaseInfo.Sie_BL_Ch0);
    IPL_CtrlInitIpc_SIEAct(Id, &SenInfo, &IpcInfo->ICF_FC.BaseInfo.SIE_Act);
    IPL_CtrlInitIpc_SIEIn(Id, &SenInfo, &IpcInfo->ICF_FC.BaseInfo.SIE_IN);
    IPL_CtrlInitIpc_SIESignal(Id, &SenInfo, TRUE, TRUE, _RISING, _RISING, _RISING, &IpcInfo->ICF_FC.BaseInfo.SenSorIn);

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_SIECh0PPB(Id, &IpcInfo->ICF_FC.BaseInfo.SIE_PPB0, &IpcInfo->ICF_FC.BaseInfo.SIE_PPB1_ADDR, NULL);
        if ((IpcInfo->ICF_FC.BaseInfo.SIE_PPB0.Width == IpcInfo->ICF_FC.BaseInfo.SIE_IN.Crop.SizeH) &&
            (IpcInfo->ICF_FC.BaseInfo.SIE_PPB0.Height == IpcInfo->ICF_FC.BaseInfo.SIE_IN.Crop.SizeV))
        {
            IpcInfo->ICF_FC.BaseInfo.SIE_Src = NonScaled_Img;
        }
        else
        {
            IpcInfo->ICF_FC.BaseInfo.SIE_Src = CH0_Scaled_Img;
        }
    }
    else
    {
        IpcInfo->ICF_FC.BaseInfo.SIE_Src = NonScaled_Img;
    }

    IPL_CtrlInitIpc_SIECaPPB(Id, &IpcInfo->ICF_FC.CA_PPB);
    IPL_CtrlInitIpc_PREInPPB(Id, &IpcInfo->ICF_FC.BaseInfo.PRE_PPB0, &IpcInfo->ICF_FC.BaseInfo.PRE_PPB1_ADDR);

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_LCA_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_IPELcaPPB(Id, &IpcInfo->ICF_FC.IPE_PPB0_LCA_ADDR, &IpcInfo->ICF_FC.IPE_PPB1_LCA_ADDR, NULL, NULL);
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_VA_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_IPEVaPPB(Id, &IpcInfo->ICF_FC.BaseInfo.IPE_VA_ADDR, &IpcInfo->ICF_FC.BaseInfo.IPE_VA_ADDR1);
    }
    IPL_CtrlInitIpc_DCEIn(Id, &SenInfo, &IpcInfo->ICF_FC.BaseInfo.DCE_Ref_1X, &IpcInfo->ICF_FC.BaseInfo.DCE_Ref_1XCenter);

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IPEIFE2_LCA_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_IFE2LcaPPB(Id, &IpcInfo->ICF_FC.IFE2_PPB0_ADDR, &IpcInfo->ICF_FC.IFE2_PPB1_ADDR);
    }
    IPL_CtrlInitIpc_IMEPxOutPPB(Id, IPLCTRL_IME_P2_EN, &IpcInfo->ICF_FC.BaseInfo.IME_P2_PPB0);
    IPL_CtrlInitIpc_DispPxPPB(Id, IPLCTRL_IME_P2_EN);

    IpcInfo->ICF_FC.Ext.IME_P1_ENABLE = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_EN);
    IPL_CtrlInitIpc_IMEPxOutPPB(Id, IPLCTRL_IME_P1_EN, &IpcInfo->ICF_FC.Ext.IME_P1_PPB0);
    IPL_CtrlInitIpc_DispPxPPB(Id, IPLCTRL_IME_P1_EN);

    IpcInfo->ICF_FC.Ext.IME_P3_ENABLE = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_EN);
    IPL_CtrlInitIpc_IMEPxOutPPB(Id, IPLCTRL_IME_P3_EN, &IpcInfo->ICF_FC.Ext.IME_P3_PPB0);
    IPL_CtrlInitIpc_DispPxPPB(Id, IPLCTRL_IME_P3_EN);

    //for channel 2 output
    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_RAW_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_SIECh2PPB(Id, &IpcInfo->ICF_FC.Ext.SIE_CH2.Out.SubImgout, &IpcInfo->ICF_FC.BaseInfo.SIE_PPB0.Width, &IpcInfo->ICF_FC.BaseInfo.SIE_PPB0.Height);
        IpcInfo->ICF_FC.Ext.SIE_CH2.Src = CH2_Scaled_Img;
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_SIEVaPPB(Id, &IpcInfo->ICF_FC.Ext.SIE_CH2.Out.VAout);
        IpcInfo->ICF_FC.Ext.SIE_CH2.Src = VACC;
    }

    //IFE2 callback
    IpcInfo->ICF_FC.fp_Ife2Init = FlowInfo->fp_IFE2Init;
    IpcInfo->ICF_FC.fp_Ife2Setting = FlowInfo->fp_IFE2EngSetting;
    IpcInfo->ICF_FC.fp_AdaptSetting = FlowInfo->fp_AdaptSetting;

    //IPE Subout callback
    IpcInfo->ICF_FC.fp_IpeCsub = FlowInfo->fp_IpeCsub;
    IpcInfo->ICF_FC.fp_IpeVsub = FlowInfo->fp_IpeVsub;

    //engine clock
    IpcInfo->ICF_FC.Ext.fp_SIEClk = FlowInfo->fp_SIEClk;
    IpcInfo->ICF_FC.Ext.fp_SIEPclkSel = FlowInfo->fp_SIEPclkSel;
    IpcInfo->ICF_FC.Ext.fp_PREClk = FlowInfo->fp_PREClk;
    IpcInfo->ICF_FC.Ext.fp_IFEClk = FlowInfo->fp_IFEClk;
    IpcInfo->ICF_FC.Ext.fp_IFE2Clk = FlowInfo->fp_IFE2Clk;
    IpcInfo->ICF_FC.Ext.fp_DCEClk = FlowInfo->fp_DCEClk;
    IpcInfo->ICF_FC.Ext.fp_IPEClk = FlowInfo->fp_IPEClk;
    IpcInfo->ICF_FC.Ext.fp_IMEClk = FlowInfo->fp_IMEClk;

    //engine setting
    IpcInfo->ICF_FC.Ext.fp_SieInit = FlowInfo->fp_SieInit;
    IpcInfo->ICF_FC.Ext.fp_PreInit = FlowInfo->fp_PreInit;
    IpcInfo->ICF_FC.Ext.fp_IfeInit = FlowInfo->fp_IfeInit;
    IpcInfo->ICF_FC.Ext.fp_DceInit = FlowInfo->fp_DceInit;
    IpcInfo->ICF_FC.Ext.fp_IpeInit = FlowInfo->fp_IpeInit;
    IpcInfo->ICF_FC.Ext.fp_ImeInit = FlowInfo->fp_ImeInit;
}

#if 0
#endif
void IPL_CtrlInitBufInfoFlowC(UINT32 Id, IPLBUF_ENG2DRAMINFO *BufInfo)
{
    //reset parameter
    memset((void *)BufInfo, 0x0, sizeof(IPLBUF_ENG2DRAMINFO));

    BufInfo->SIECh0En = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_EN);
    if (BufInfo->SIECh0En == ENABLE)
    {
        BufInfo->SIEOutCh0.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_BUFNUM);
        BufInfo->SIEOutCh0.Width = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
        BufInfo->SIEOutCh0.Height = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V);
        BufInfo->SIEOutCh0.Bit = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT);
        BufInfo->SIEOutCh0.LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS);
    }

    BufInfo->SIECh2En = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_RAW_EN);
    if (BufInfo->SIECh2En == ENABLE)
    {
        BufInfo->SIEOutCh2.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_RAW_BUFNUM);
        BufInfo->SIEOutCh2.Width = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_RAW_SIZE_H);
        BufInfo->SIEOutCh2.Height = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_RAW_SIZE_V);
        BufInfo->SIEOutCh2.Bit = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_PRVBIT);
        BufInfo->SIEOutCh2.LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_RAW_SIZE_HLOS);
    }

    BufInfo->SIECAEn = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_EN);
    if (BufInfo->SIECAEn == ENABLE)
    {
        BufInfo->SIECABufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_BUFNUM);
        BufInfo->SIECABufSize = IPL_CtrlGetSIECABufInfo(IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_TYPE),
                                                         IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_WINXNUM),
                                                         IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_WINYNUM));
    }

    BufInfo->SIEVAEn = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_EN);
    if (BufInfo->SIEVAEn == ENABLE)
    {
        BufInfo->SIEVABufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_BUFNUM);
        BufInfo->SIEVASize = IPL_CtrlGetSIEVABufInfo();
    }

    BufInfo->IPEOutEn = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_EN);
    if (BufInfo->IPEOutEn == ENABLE)
    {
        BufInfo->IPEOut.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_BUFNUM);
        BufInfo->IPEOut.Fmt = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_FMT);
        BufInfo->IPEOut.YWidth = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_SIZE_H);
        BufInfo->IPEOut.YLineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_SIZE_HLOS);
        BufInfo->IPEOut.YHeight = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_SIZE_V);
    }

    BufInfo->IPELCAEn = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_LCA_EN);
    if (BufInfo->IPELCAEn == ENABLE)
    {
        BufInfo->IPELCABufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_LCA_BUFNUM);
        BufInfo->IPELCABufSize = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_LCA_SIZE_HLOS) *
                                 IPL_CtrlGetInfor(Id, IPLCTRL_IPE_LCA_SIZE_V) *
                                 2 * BufInfo->IPELCABufNum;
    }

    BufInfo->IPEVEn = DISABLE;

    BufInfo->IPEVAEn = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_VA_EN);
    if (BufInfo->IPEVAEn == ENABLE)
    {
        BufInfo->IPEVABufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_VA_BUFNUM);
        BufInfo->IPEVASize = IPL_CtrlGetIPEVABufInfo();
    }

    BufInfo->IPEEthEn = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_ETH_EN);
    if (BufInfo->IPEEthEn == ENABLE)
    {
        BufInfo->IPEEthSize = (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_ETH_SIZE_HLOS) * IPL_CtrlGetInfor(Id, IPLCTRL_IPE_ETH_SIZE_V));
    }

    BufInfo->IPEIFE2LCAEn = IPL_CtrlGetInfor(Id, IPLCTRL_IPEIFE2_LCA_EN);
    if (BufInfo->IPEIFE2LCAEn == ENABLE)
    {
        BufInfo->IPEIFE2LCABufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IPEIFE2_LCA_BUFNUM);
        BufInfo->IPEIFE2LCABufSize = BufInfo->IPELCABufSize;
    }

    //path buffer always allocation
    BufInfo->IMEPath1En = ENABLE;
    BufInfo->IMEPath1.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUFNUM);
    BufInfo->IMEPath1.YWidth = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_H);
    BufInfo->IMEPath1.YHeight = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_V);
    BufInfo->IMEPath1.YLineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_H);
    BufInfo->IMEPath1.Fmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_FMT);

    BufInfo->IMEPath2En = ENABLE;
    BufInfo->IMEPath2.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUFNUM);
    BufInfo->IMEPath2.YWidth = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_H);
    BufInfo->IMEPath2.YHeight = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_V);
    BufInfo->IMEPath2.YLineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_H);
    BufInfo->IMEPath2.Fmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_FMT);

    BufInfo->IMEPath3En = ENABLE;
    BufInfo->IMEPath3.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUFNUM);
    BufInfo->IMEPath3.YWidth = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_H);
    BufInfo->IMEPath3.YHeight = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_V);
    BufInfo->IMEPath3.YLineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_H);
    BufInfo->IMEPath3.Fmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_FMT);
}
#if 0
#endif
void IPL_CtrlPrvInitFlowC(UINT32 Id, IPLCTRL_INFO *CtrlInfo)
{
    UINT32 Width, Height;
    IPC_QINFO IpcInfo;

    IpcInfo = IPL_CTRLQuery(ICF_FLOWC);

    IPL_CtrlSetInfor(Id, IPLCTRL_PRVAUTO_OB_EN, DISABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_PRVAUTO_SMR_EN, DISABLE);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVSTPIX, IPL_UTI_CONV_SEN_STPIX(CtrlInfo->Info->Mode->StPix));

    IPL_DzoomGetSIECropSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V, Height);

    //SIE ch0
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_EN, ENABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_BUFNUM, IpcInfo.QUE_FC.nBUF_SIEPPB);
    IPL_DzoomGetSIEOutSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT, IPL_UTI_CONV_ALGINFO_RAW_BIT(IPL_AlgGetUIInfo(IPL_SEL_RAW_BITDEPTH_PRV)));
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width, IPL_UTI_CONV_ALGINFO_RAW_BIT(IPL_AlgGetUIInfo(IPL_SEL_RAW_BITDEPTH_PRV))));
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V, Height);

    //SIE ch2
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_RAW_EN, DISABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_RAW_BUFNUM, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_RAW_SIZE_H, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_PRVBIT, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_RAW_SIZE_HLOS, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_RAW_SIZE_V, 0);

    //SIE ch1
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CA_EN, ENABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CA_TYPE, IPLCTRL_CA_TYPE_64BIT);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CA_WINXNUM, 32);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CA_WINYNUM, 32);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CA_BUFNUM, IpcInfo.QUE_FC.nBUF_CAPPB);

    //PRE IN
    IPL_DzoomGetPREInSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);
    IPL_CtrlSetInfor(Id, IPLCTRL_PRE_IN_BIT, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT));
    IPL_CtrlSetInfor(Id, IPLCTRL_PRE_IN_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_PRE_IN_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width, IPL_UTI_CONV_ALGINFO_RAW_BIT(IPL_AlgGetUIInfo(IPL_SEL_RAW_BITDEPTH_PRV))));
    IPL_CtrlSetInfor(Id, IPLCTRL_PRE_IN_SIZE_V, Height);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_VA_EN, DISABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_VA_BUFNUM, 0);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_ECS_EN, DISABLE);

    //IPE
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_EN, DISABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_FMT, IPL_IMG_Y_PACK_UV422);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_BUFNUM, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_H, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_HLOS, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_V, 0);

    if (CtrlInfo->ChgMode->FuncEn & IPL_FUNC_VA)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_VA_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_VA_BUFNUM, IpcInfo.QUE_FC.nBUF_IPE_VAPPB);
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_VA_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_VA_BUFNUM, 0);
    }

    //DIS
    if (CtrlInfo->ChgMode->FuncEn & IPL_FUNC_DIS)
    {

        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_ETH_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_ETH_SIZE_H, (IPL_CtrlGetInfor(Id, IPLCTRL_PRE_IN_SIZE_H) * 2 / 8));
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_ETH_SIZE_HLOS, IPL_ALIGN_ROUNDUP(IPL_CtrlGetInfor(Id, IPLCTRL_IPE_ETH_SIZE_H), 4));
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_ETH_SIZE_V, IPL_CtrlGetInfor(Id, IPLCTRL_PRE_IN_SIZE_V));
        if (IPL_AlgGetUIInfo(IPL_SEL_DISCOMP) == SEL_DISCOMP_ON)
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_DIS_EN, ENABLE);
        }
        else
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_DIS_EN, DISABLE);
        }
        IPL_CtrlSetInfor(Id, IPLCTRL_DIS_RATIO, dis_getDisViewRatio());
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_ETH_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_ETH_SIZE_H, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_ETH_SIZE_HLOS, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_ETH_SIZE_V, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_DIS_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_DIS_RATIO, IMG_DIS_RATIO_UINT);
    }

    /*************** force disable *********************/
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_EN, DISABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_BUFNUM, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_SIZE_H, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_SIZE_HLOS, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_SIZE_V, 0);
    /*************** force disable *********************/


    if (IQS_PRV_IsChromaAdaptEn())
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_BUFNUM, IpcInfo.QUE_FC.nBUF_IPE_LCAPPB);

        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_SIZE_H, IPL_UTI_IPESUB_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_PRE_IN_SIZE_H), IQS_GetChromaAdaptMinRatio(), 4));
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_SIZE_HLOS, (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_LCA_SIZE_H) << 1));
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_SIZE_V, IPL_UTI_IPESUB_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_PRE_IN_SIZE_V), IQS_GetChromaAdaptMinRatio(), 4));

        IPL_CtrlSetInfor(Id, IPLCTRL_IPEIFE2_LCA_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPEIFE2_LCA_BUFNUM, IpcInfo.QUE_FC.nBUF_IFE2PPB);
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_BUFNUM, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_SIZE_H, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_SIZE_HLOS, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_SIZE_V, 0);

        IPL_CtrlSetInfor(Id, IPLCTRL_IPEIFE2_LCA_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPEIFE2_LCA_BUFNUM, 0);
    }


    //IME P1
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_EN, CtrlInfo->ChgMode->ImgP1En);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_FMT, CtrlInfo->ChgMode->ImgFmt_P1);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_BUFNUM, IPL_GetDisplayCtrlBufferNum());
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_H, IPL_UTI_DIS_ADJ_HSIZE(CtrlInfo->ChgMode->BufSizeH_P1, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16));
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_V, IPL_UTI_DIS_ADJ_VSIZE(CtrlInfo->ChgMode->BufSizeV_P1, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 4));

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_EN) == ENABLE)
    {
        if ((CtrlInfo->ChgMode->BufSizeH_P1 * CtrlInfo->ChgMode->BufSizeV_P1) < (CtrlInfo->ChgMode->ImgSizeH_P1 * CtrlInfo->ChgMode->ImgSizeV_P1))
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_H, CtrlInfo->ChgMode->BufSizeH_P1);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_HLOS, CtrlInfo->ChgMode->BufSizeH_P1);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_V, CtrlInfo->ChgMode->BufSizeV_P1);

            DBG_ERR("buffer(%d %d) size < image(%d %d) size\r\n", CtrlInfo->ChgMode->BufSizeH_P1,
                    CtrlInfo->ChgMode->BufSizeV_P1, CtrlInfo->ChgMode->ImgSizeH_P1, CtrlInfo->ChgMode->ImgSizeV_P1);
        }
        else
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_H, CtrlInfo->ChgMode->ImgSizeH_P1);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_HLOS, CtrlInfo->ChgMode->ImgSizeH_P1);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_V, CtrlInfo->ChgMode->ImgSizeV_P1);
        }
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_H, CtrlInfo->ChgMode->BufSizeH_P1);
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_HLOS, CtrlInfo->ChgMode->BufSizeH_P1);
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_V, CtrlInfo->ChgMode->BufSizeV_P1);
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_DIS_EN) == ENABLE)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_H, IPL_UTI_DIS_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_H), IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16));
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_HLOS, IPL_UTI_DIS_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_HLOS), IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16));
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_V, IPL_UTI_DIS_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_V), IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 4));
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_H, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_H));
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_HLOS, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_HLOS));
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_V, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_V));
    }

    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_DISP_SIZE_H, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_H));
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_DISP_SIZE_HLOS, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_HLOS));
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_DISP_SIZE_V, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_V));

    //IME P2
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_EN, ENABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_FMT, CtrlInfo->ChgMode->ImgFmt_Disp);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_BUFNUM, IPL_GetDisplayCtrlBufferNum());
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_H, IPL_UTI_DIS_ADJ_HSIZE(CtrlInfo->ChgMode->BufSizeH_Disp, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16));
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_V, IPL_UTI_DIS_ADJ_VSIZE(CtrlInfo->ChgMode->BufSizeV_Disp, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 4));

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_EN) == ENABLE)
    {
        if ((CtrlInfo->ChgMode->BufSizeH_Disp * CtrlInfo->ChgMode->BufSizeV_Disp) < (CtrlInfo->ChgMode->ImgSizeH_Disp * CtrlInfo->ChgMode->ImgSizeV_Disp))
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_H, CtrlInfo->ChgMode->BufSizeH_Disp);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_HLOS, CtrlInfo->ChgMode->BufSizeH_Disp);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_V, CtrlInfo->ChgMode->BufSizeV_Disp);

            DBG_ERR("buffer(%d %d) size < image(%d %d) size\r\n", CtrlInfo->ChgMode->BufSizeH_Disp,
                    CtrlInfo->ChgMode->BufSizeV_Disp, CtrlInfo->ChgMode->ImgSizeH_Disp, CtrlInfo->ChgMode->ImgSizeV_Disp);
        }
        else
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_H, CtrlInfo->ChgMode->ImgSizeH_Disp);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_HLOS, CtrlInfo->ChgMode->ImgSizeH_Disp);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_V, CtrlInfo->ChgMode->ImgSizeV_Disp);
        }
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_H, CtrlInfo->ChgMode->BufSizeH_Disp);
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_HLOS, CtrlInfo->ChgMode->BufSizeH_Disp);
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_V, CtrlInfo->ChgMode->BufSizeV_Disp);
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_DIS_EN) == ENABLE)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_H, IPL_UTI_DIS_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_H), IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16));
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_HLOS, IPL_UTI_DIS_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_HLOS), IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16));
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_V, IPL_UTI_DIS_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_V), IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 4));
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_H, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_H));
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_HLOS, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_HLOS));
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_V, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_V));
    }

    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_DISP_SIZE_H, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_H));
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_DISP_SIZE_HLOS, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_HLOS));
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_DISP_SIZE_V, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_V));


    //IME P3
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_EN, CtrlInfo->ChgMode->ImgP3En);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_FMT, CtrlInfo->ChgMode->ImgFmt_P3);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_BUFNUM, IPL_GetDisplayCtrlBufferNum3());
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_H, IPL_UTI_DIS_ADJ_HSIZE(CtrlInfo->ChgMode->BufSizeH_P3, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16));
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_V, IPL_UTI_DIS_ADJ_VSIZE(CtrlInfo->ChgMode->BufSizeV_P3, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 4));

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_EN) == ENABLE)
    {
        if ((CtrlInfo->ChgMode->BufSizeH_P3 * CtrlInfo->ChgMode->BufSizeV_P3) < (CtrlInfo->ChgMode->ImgSizeH_P3 * CtrlInfo->ChgMode->ImgSizeV_P3))
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_H, CtrlInfo->ChgMode->BufSizeH_P3);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_HLOS, CtrlInfo->ChgMode->BufSizeH_P3);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_V, CtrlInfo->ChgMode->BufSizeV_P3);

            DBG_ERR("buffer(%d %d) size < image(%d %d) size\r\n", CtrlInfo->ChgMode->BufSizeH_P3,
                    CtrlInfo->ChgMode->BufSizeV_P3, CtrlInfo->ChgMode->ImgSizeH_P3, CtrlInfo->ChgMode->ImgSizeV_P3);
        }
        else
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_H, CtrlInfo->ChgMode->ImgSizeH_P3);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_HLOS, CtrlInfo->ChgMode->ImgSizeH_P3);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_V, CtrlInfo->ChgMode->ImgSizeV_P3);
        }
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_H, CtrlInfo->ChgMode->BufSizeH_P3);
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_HLOS, CtrlInfo->ChgMode->BufSizeH_P3);
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_V, CtrlInfo->ChgMode->BufSizeV_P3);
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_DIS_EN) == ENABLE)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_H, IPL_UTI_DIS_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_H), IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16));
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_HLOS, IPL_UTI_DIS_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_HLOS), IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16));
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_V, IPL_UTI_DIS_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_V), IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 4));
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_H, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_H));
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_HLOS, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_HLOS));
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_V, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_V));
    }


    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_DISP_SIZE_H, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_H));
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_DISP_SIZE_HLOS, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_HLOS));
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_DISP_SIZE_V, IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_V));
}
