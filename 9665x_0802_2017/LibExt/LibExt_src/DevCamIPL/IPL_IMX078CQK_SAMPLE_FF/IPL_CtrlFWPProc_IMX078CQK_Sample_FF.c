/**
    Ctrl sample FW+PProc.

    @file       IPL_CtrlSampleFWPProc.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "IPL_IMX078CQK_Sample_FF_Int.h"

UINT32 IPL_IFE2InitCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IFE2EngCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
void* IPL_AdaptSettingCBFlowB(BOOL* bAdaptEn);
void* IPL_RetinexSettingCBFlowB(BOOL* bRetinexEn);
void* IPL_IpeCsubCBFlowB(BOOL *bFuncEn);
void* IPL_IpeVsubCBFlowB(BOOL *bFuncEn);
IPH_CLK IPL_SIEClkCBFlowB(void);
UINT32 IPL_SIEPclkSelCBFlowB(void);
IPH_CLK IPL_PREClkCBFlowB(void);
IPH_CLK IPL_IFEClkCBFlowB(void);
IPH_CLK IPL_DCEClkCBFlowB(void);
IPH_CLK IPL_IPEClkCBFlowB(void);
IPH_CLK IPL_IFE2ClkCBFlowB(void);
IPH_CLK IPL_IMEClkCBFlowB(void);
UINT32 IPL_SIEInitCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_PREInitCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IFEInitCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_DCEInitCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IPEInitCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IMEInitCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);

IPLCTRL_FLOWB_INFO IPL_FlowBInfo =
{
    {IPL_HAL_PAR0, IPL_HAL_PAR1},
    IPL_HAL_PAR2,

    IPL_IFE2InitCBFlowB,
    IPL_IFE2EngCBFlowB,
    IPL_AdaptSettingCBFlowB,
    IPL_RetinexSettingCBFlowB,
    IPL_IpeCsubCBFlowB,
    IPL_IpeVsubCBFlowB,
    IPL_SIEClkCBFlowB,
    IPL_SIEPclkSelCBFlowB,
    IPL_PREClkCBFlowB,
    IPL_IFEClkCBFlowB,
    IPL_DCEClkCBFlowB,
    IPL_IPEClkCBFlowB,
    IPL_IFE2ClkCBFlowB,
    IPL_IMEClkCBFlowB,

    IPL_SIEInitCBFlowB,
    IPL_PREInitCBFlowB,
    IPL_IFEInitCBFlowB,
    IPL_DCEInitCBFlowB,
    IPL_IPEInitCBFlowB,
    IPL_IMEInitCBFlowB
};

static IPLCTRL_FLOWB_INFO* IPL_GetCtrlFlowBInfo(void)
{
    return &IPL_FlowBInfo;
}

IPL_HAL_PARASET IPL_CtrlGetHalSetIdFlowB(UINT32 Id)
{
    return IPL_GetCtrlFlowBInfo()->IPL_HalParamId[Id];
}

IPL_HAL_PARASET IPL_CtrlGetHalSetIdPProcFlowB(void)
{
    return IPL_GetCtrlFlowBInfo()->PPost_HalParamId;
}

UINT32 IPL_IFE2InitCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    //DBG_ERR("IPL_IFE2InitCBFlowB\r\n");
    IQCB_SettingFlowFunc(IQCB_PRV_IFE2_INIT_CC, Info, group);
    return 1;
}
UINT32 IPL_IFE2EngCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    //DBG_ERR("IPL_IFE2EngCBFlowB\r\n");
    IQCB_SettingFlowFunc(IQCB_PRV_IFE2_CC, Info, group);
    return 1;
}


void* IPL_AdaptSettingCBFlowB(BOOL* bAdaptEn)
{
    ENG_SUBFUNC Info;
    IME_SUBFUNC Ime_sub = {0};
    IPL_HAL_GROUP group;

    //DBG_ERR("IPL_AdaptSettingCBFlowB\r\n");
    Info.pIme_sub = &Ime_sub;
    IQCB_SettingFlowFunc(IQCB_PRV_IME_LCA, &Info, &group);
    if ( (Info.pIme_sub->imeFuncEn & ChromaAdaptEn) != 0 )
    {
        *bAdaptEn = TRUE;
        return (void*)Info.pIme_sub->ChromaAdap.pChromaAdaption;
    }
    else
    {
        *bAdaptEn = FALSE;
        return NULL;
    }
}

void* IPL_RetinexSettingCBFlowB(BOOL* bRetinexEn)
{

    //*bRetinexEn = FALSE;
    //return NULL;
    //*bRetinexEn = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IPE_V_EN);
    //return &IME_RetInex;

    ENG_SUBFUNC Info;
    IME_SUBFUNC Ime_sub = {0};
    IPL_HAL_GROUP group;
    //DBG_ERR("IPL_RetinexSettingCBFlowB\r\n");
    Info.pIme_sub = &Ime_sub;
    IQCB_SettingFlowFunc(IQCB_PRV_IME_RETINEX, &Info, &group);
    if ( (Info.pIme_sub->imeFuncEn & RetinexEn) != 0 )
    {
        *bRetinexEn = TRUE;
    }
    else
    {
        *bRetinexEn = FALSE;
    }
    return (void*)Info.pIme_sub->Retinex.pRetinex;
}

void* IPL_IpeCsubCBFlowB(BOOL *bFuncEn)
{
    *bFuncEn = FALSE;
    return NULL;
}

void* IPL_IpeVsubCBFlowB(BOOL *bFuncEn)
{
    static IPE_V_SUBOUT VoutInfo = {0};
    ENG_SUBFUNC Info;
    IME_SUBFUNC Ime_sub = {0};
    IPL_HAL_GROUP group;

    if (IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IPE_V_EN) == ENABLE)
    {
        Info.pIme_sub = &Ime_sub;
        IQCB_SettingFlowFunc(IQCB_PRV_IME_RETINEX, &Info, &group);

        VoutInfo.OutSel = v_Vch_out;
        VoutInfo.Size.H = IPL_UTI_IPESUB_ADJ_HSIZE(IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_PRE_IN_SIZE_H), Info.pIme_sub->Retinex.pRetinex->SubRatio, 4);
        VoutInfo.Size.Lineofs = (VoutInfo.Size.H << 1);
        VoutInfo.Size.V = IPL_UTI_IPESUB_ADJ_VSIZE(IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_PRE_IN_SIZE_V), Info.pIme_sub->Retinex.pRetinex->SubRatio, 4);
        *bFuncEn = TRUE;
        return (void *)&VoutInfo;
    }
    else
    {
        *bFuncEn = FALSE;
        return NULL;
    }
}
#if 0
#endif
IPH_CLK IPL_SIEClkCBFlowB(void)
{
    IPH_CLK clk;
    SENSOR_INFO SenInfo;

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(IPL_ID_1), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &SenInfo);

    if (SenInfo.DataType == SENSOR_DATA_LVDS)
    {
        clk.SIEclk = 0;
    }
    else if (SenInfo.DataType == SENSOR_DATA_DUMMY)
    {
        clk.SIEclk = 27000000;
    }
    else
    {
        //set sie clk for parallel sensor
        //clk.SIEclk = 0;
    }
    return clk;
}

UINT32 IPL_SIEPclkSelCBFlowB(void)
{
    return IPH_SIEPXCLK_OFF;
}

IPH_CLK IPL_PREClkCBFlowB(void)
{
    IPH_CLK clk;
    clk.PREclk = IPH_PRECLK6;
    return clk;
}

IPH_CLK IPL_IFEClkCBFlowB(void)
{
    IPH_CLK clk;
    clk.IFEclk = IPH_IFECLK6;
    return clk;
}

IPH_CLK IPL_DCEClkCBFlowB(void)
{
    IPH_CLK clk;
    clk.DCEclk = IPH_DCECLK6;
    return clk;
}

IPH_CLK IPL_IPEClkCBFlowB(void)
{
    IPH_CLK clk;
    clk.IPEclk = IPH_IPECLK6;
    return clk;
}

IPH_CLK IPL_IFE2ClkCBFlowB(void)
{
    IPH_CLK clk;
    clk.IFE2clk = IPH_IFE2CLK6;
    return clk;
}

IPH_CLK IPL_IMEClkCBFlowB(void)
{
    IPH_CLK clk;
    clk.IMEclk = IPH_IMECLK6;
    return clk;
}
#if 0
#endif

UINT32 IPL_SIEInitCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
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

UINT32 IPL_PREInitCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IQCB_SettingFlowFunc(IQCB_PRV_PRE_INIT, Info, group);
    return 1;
}

UINT32 IPL_IFEInitCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IQCB_SettingFlowFunc(IQCB_PRV_IFE_INIT, Info, group);
    return 1;
}

UINT32 IPL_DCEInitCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IQCB_SettingFlowFunc(IQCB_PRV_DCE_INIT, Info, group);
    return 1;
}
UINT32 IPL_IPEInitCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IPE_UPDATE *Update;

    //init info structure
    Update = (IPE_UPDATE *)group;

    IQCB_SettingFlowFunc(IQCB_PRV_IPE_INIT, Info, group);
    IPL_IPESetEthOut_FCB(IPL_ID_1, &Info->pIpe_sub->IpeFuncEn, &Info->pIpe_sub->IpeInteEn, Update, Info->pIpe_sub);

    IPL_IPESetCCOut_FCB(IPL_ID_1, &Info->pIpe_sub->IpeFuncEn, &Info->pIpe_sub->IpeInteEn, Update, Info->pIpe_sub);
    IPL_IPESetVOut_FCB(IPL_ID_1, &Info->pIpe_sub->IpeFuncEn, &Info->pIpe_sub->IpeInteEn, Update, Info->pIpe_sub);
    return 1;
}

UINT32 IPL_IMEInitCBFlowB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
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
void IPL_CtrlInitIpcInfoFlowB(UINT32 Id, IPC_INFO *IpcInfo)
{
    SENSOR_INFO SenInfo;
    IPLCTRL_FLOWB_INFO *FlowInfo;

    FlowInfo = IPL_GetCtrlFlowBInfo();

    IpcInfo->ICF_FB.ParaIdx = FlowInfo->IPL_HalParamId[Id];

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &SenInfo);

    IPL_CtrlInitIpc_SIEBurstLength(Id, &SenInfo, &IpcInfo->ICF_FB.BaseInfo.Sie_BL_Ch0);
    IPL_CtrlInitIpc_SIEAct(Id, &SenInfo, &IpcInfo->ICF_FB.BaseInfo.SIE_Act);
    IPL_CtrlInitIpc_SIEIn(Id, &SenInfo, &IpcInfo->ICF_FB.BaseInfo.SIE_IN);
    IPL_CtrlInitIpc_SIESignal(Id, &SenInfo, FALSE, FALSE, _RISING, _RISING, _RISING, &IpcInfo->ICF_FB.BaseInfo.SenSorIn);

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_SIECh0PPB(Id, &IpcInfo->ICF_FB.BaseInfo.SIE_PPB0, &IpcInfo->ICF_FB.BaseInfo.SIE_PPB1_ADDR, NULL);
        if ((IpcInfo->ICF_FB.BaseInfo.SIE_PPB0.Width == IpcInfo->ICF_FB.BaseInfo.SIE_IN.Crop.SizeH) &&
            (IpcInfo->ICF_FB.BaseInfo.SIE_PPB0.Height == IpcInfo->ICF_FB.BaseInfo.SIE_IN.Crop.SizeV))
        {
            IpcInfo->ICF_FB.BaseInfo.SIE_Src = NonScaled_Img;
        }
        else
        {
            IpcInfo->ICF_FB.BaseInfo.SIE_Src = CH0_Scaled_Img;
        }
    }
    else
    {
        IpcInfo->ICF_FB.BaseInfo.SIE_Src = NonScaled_Img;
    }

    IPL_CtrlInitIpc_SIECaPPB(Id, &IpcInfo->ICF_FB.CA_PPB);
    IPL_CtrlInitIpc_PREInPPB(Id, &IpcInfo->ICF_FB.BaseInfo.PRE_PPB0, &IpcInfo->ICF_FB.BaseInfo.PRE_PPB1_ADDR);

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_LCA_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_IPELcaPPB(Id, &IpcInfo->ICF_FB.IPE_PPB0_LCA_ADDR, &IpcInfo->ICF_FB.IPE_PPB1_LCA_ADDR, NULL, NULL);
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_V_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_IPEVPPB(Id, &IpcInfo->ICF_FB.IPE_PPB0_HDR_ADDR, &IpcInfo->ICF_FB.IPE_PPB1_HDR_ADDR, NULL, NULL);
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_VA_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_IPEVaPPB(Id, &IpcInfo->ICF_FB.BaseInfo.IPE_VA_ADDR, &IpcInfo->ICF_FB.BaseInfo.IPE_VA_ADDR1);
    }
    IPL_CtrlInitIpc_DCEIn(Id, &SenInfo, &IpcInfo->ICF_FB.BaseInfo.DCE_Ref_1X, &IpcInfo->ICF_FB.BaseInfo.DCE_Ref_1XCenter);

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IPEIFE2_LCA_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_IFE2LcaPPB(Id, &IpcInfo->ICF_FB.IFE2_PPB0_ADDR, &IpcInfo->ICF_FB.IFE2_PPB1_ADDR);
    }
    IPL_CtrlInitIpc_IMEPxOutPPB(Id, IPLCTRL_IME_P2_EN, &IpcInfo->ICF_FB.BaseInfo.IME_P2_PPB0);
    IPL_CtrlInitIpc_DispPxPPB(Id, IPLCTRL_IME_P2_EN);

    IpcInfo->ICF_FB.Ext.IME_P1_ENABLE = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_EN);
    IPL_CtrlInitIpc_IMEPxOutPPB(Id, IPLCTRL_IME_P1_EN, &IpcInfo->ICF_FB.Ext.IME_P1_PPB0);
    IPL_CtrlInitIpc_DispPxPPB(Id, IPLCTRL_IME_P1_EN);

    IpcInfo->ICF_FB.Ext.IME_P3_ENABLE = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_EN);
    IPL_CtrlInitIpc_IMEPxOutPPB(Id, IPLCTRL_IME_P3_EN, &IpcInfo->ICF_FB.Ext.IME_P3_PPB0);
    IPL_CtrlInitIpc_DispPxPPB(Id, IPLCTRL_IME_P3_EN);

    //for channel 2 output
    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_RAW_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_SIECh2PPB(Id, &IpcInfo->ICF_FB.Ext.SIE_CH2.Out.SubImgout, &IpcInfo->ICF_FB.BaseInfo.SIE_PPB0.Width, &IpcInfo->ICF_FB.BaseInfo.SIE_PPB0.Height);
        IpcInfo->ICF_FB.Ext.SIE_CH2.Src = CH2_Scaled_Img;
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_SIEVaPPB(Id, &IpcInfo->ICF_FB.Ext.SIE_CH2.Out.VAout);
        IpcInfo->ICF_FB.Ext.SIE_CH2.Src = VACC;
    }

    //IFE2 callback
    IpcInfo->ICF_FB.fp_Ife2Init = FlowInfo->fp_IFE2Init;
    IpcInfo->ICF_FB.fp_Ife2Setting = FlowInfo->fp_IFE2EngSetting;
    IpcInfo->ICF_FB.fp_AdaptSetting = FlowInfo->fp_AdaptSetting;
    IpcInfo->ICF_FB.fp_RetinexSetting = FlowInfo->fp_RetinexSetting;

    //IPE Subout callback
    IpcInfo->ICF_FB.fp_IpeCsub = FlowInfo->fp_IpeCsub;
    IpcInfo->ICF_FB.fp_IpeVsub = FlowInfo->fp_IpeVsub;

    //engine clock
    IpcInfo->ICF_FB.Ext.fp_SIEClk = FlowInfo->fp_SIEClk;
    IpcInfo->ICF_FB.Ext.fp_SIEPclkSel = FlowInfo->fp_SIEPclkSel;
    IpcInfo->ICF_FB.Ext.fp_PREClk = FlowInfo->fp_PREClk;
    IpcInfo->ICF_FB.Ext.fp_IFEClk = FlowInfo->fp_IFEClk;
    IpcInfo->ICF_FB.Ext.fp_IFE2Clk = FlowInfo->fp_IFE2Clk;
    IpcInfo->ICF_FB.Ext.fp_DCEClk = FlowInfo->fp_DCEClk;
    IpcInfo->ICF_FB.Ext.fp_IPEClk = FlowInfo->fp_IPEClk;
    IpcInfo->ICF_FB.Ext.fp_IMEClk = FlowInfo->fp_IMEClk;

    //engine setting
    IpcInfo->ICF_FB.Ext.fp_SieInit = FlowInfo->fp_SieInit;
    IpcInfo->ICF_FB.Ext.fp_PreInit = FlowInfo->fp_PreInit;
    IpcInfo->ICF_FB.Ext.fp_IfeInit = FlowInfo->fp_IfeInit;
    IpcInfo->ICF_FB.Ext.fp_DceInit = FlowInfo->fp_DceInit;
    IpcInfo->ICF_FB.Ext.fp_IpeInit = FlowInfo->fp_IpeInit;
    IpcInfo->ICF_FB.Ext.fp_ImeInit = FlowInfo->fp_ImeInit;
}
#if 0
#endif
void IPL_CtrlInitBufInfoFlowB(UINT32 Id, IPLBUF_ENG2DRAMINFO *BufInfo)
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

    BufInfo->IPEVEn = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_V_EN);
    if (BufInfo->IPEVEn == ENABLE)
    {
        BufInfo->IPEVBufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_V_BUFNUM);
        BufInfo->IPEVBufSize = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_V_SIZE_HLOS) *
                               IPL_CtrlGetInfor(Id, IPLCTRL_IPE_V_SIZE_V) *
                               BufInfo->IPEVBufNum;
    }

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

void IPL_CtrlPrvInitFlowB(UINT32 Id, IPLCTRL_INFO *CtrlInfo)
{
    UINT32 Width, Height;
    IPC_QINFO IpcInfo;

    IpcInfo = IPL_CTRLQuery(ICF_FLOWB);

    IPL_CtrlSetInfor(Id, IPLCTRL_PRVAUTO_OB_EN, DISABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_PRVAUTO_SMR_EN, DISABLE);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVSTPIX, IPL_UTI_CONV_SEN_STPIX(CtrlInfo->Info->Mode->StPix));

    IPL_DzoomGetSIECropSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V, Height);

    //SIE ch0
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_EN, ENABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_BUFNUM, IpcInfo.QUE_FB.nBUF_SIEPPB);
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
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CA_BUFNUM, IpcInfo.QUE_FB.nBUF_CAPPB);

    //PRE IN
    IPL_DzoomGetPREInSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);
    IPL_CtrlSetInfor(Id, IPLCTRL_PRE_IN_BIT, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT));
    IPL_CtrlSetInfor(Id, IPLCTRL_PRE_IN_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_PRE_IN_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width, IPL_UTI_CONV_ALGINFO_RAW_BIT(IPL_AlgGetUIInfo(IPL_SEL_RAW_BITDEPTH_PRV))));
    IPL_CtrlSetInfor(Id, IPLCTRL_PRE_IN_SIZE_V, Height);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_VA_EN, DISABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_VA_BUFNUM, 0);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_ECS_EN, ENABLE);

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
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_VA_BUFNUM, IpcInfo.QUE_FB.nBUF_IPE_VAPPB);
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

    if (CtrlInfo->ChgMode->FuncEn & IPL_FUNC_IMGEFFECT_WDR)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_BUFNUM, IpcInfo.QUE_FB.nBUF_IPE_HDRPPB);

        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_SIZE_H, IPL_UTI_IPESUB_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_PRE_IN_SIZE_H), IQS_GetRetinexMinRatio(), 4));
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_SIZE_HLOS, (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_V_SIZE_H) << 1));
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_SIZE_V, IPL_UTI_IPESUB_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_PRE_IN_SIZE_V), IQS_GetRetinexMinRatio(), 4));
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_BUFNUM, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_SIZE_H, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_SIZE_HLOS, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_V_SIZE_V, 0);
    }

    if (IQS_PRV_IsChromaAdaptEn())
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_BUFNUM, IpcInfo.QUE_FB.nBUF_IPE_LCAPPB);

        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_SIZE_H, IPL_UTI_IPESUB_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_PRE_IN_SIZE_H), IQS_GetChromaAdaptMinRatio(), 4));
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_SIZE_HLOS, (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_LCA_SIZE_H) << 1));
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_LCA_SIZE_V, IPL_UTI_IPESUB_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_PRE_IN_SIZE_V), IQS_GetChromaAdaptMinRatio(), 4));

        IPL_CtrlSetInfor(Id, IPLCTRL_IPEIFE2_LCA_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPEIFE2_LCA_BUFNUM, IpcInfo.QUE_FB.nBUF_IFE2PPB);
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
