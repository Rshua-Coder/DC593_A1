/**
    Ctrl AR0330_TVP5150_CARDV_FFFF.

    @file       IPL_CtrlAR0330_TVP5150_CARDV_FFFF.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "IPL_OV2710M_TVP5150_CARDV_FFFF_Int.h"
//note: do not delete
//avoid compiler optimize "weak function"
WEAK_DUMMY_CALL(CTRL1, CMD)
WEAK_DUMMY_CALL(CTRL2, D2D)

void IPL_SIESetOB_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_OBAVG *OBAvg, SIE_OBSUB *OBSub)
{
    SENSOR_INFO SenInfo;

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo2(Id, IPL_SEL_PRVSENMODE), &SenInfo);

    if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVAUTO_OB_EN) == DISABLE)
    {
        OBSub->Offset = 0xa0;  //iso100: 0xa0, iso1600: 0x90
        OBSub->DtGain = 0x20;
        *FuncEn |= OBSubSelEn;
        *Update |= (SIE_OBOFS_ | SIE_SubFEn);
    }
    else
    {
        OBAvg->StartPix.x = SenInfo.Mode->OB.TopWin[0];
        OBAvg->StartPix.y = SenInfo.Mode->OB.TopWin[1];
        OBAvg->SizeH = (SenInfo.Mode->OB.TopWin[2] - SenInfo.Mode->OB.TopWin[0]);
        OBAvg->SizeV = (SenInfo.Mode->OB.TopWin[3] - SenInfo.Mode->OB.TopWin[1]);
        OBAvg->AvgActThres = 0x80;
        OBSub->Offset = 0;
        OBSub->DtGain = 0x20;
        *FuncEn |= (OBSubSelEn | OBAvgEn);
        *Update |= (SIE_OBOFS_ | SIE_OBAVG_ | SIE_SubFEn);
    }
}

void IPL_SIESetSMR_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, UINT32 *SmrActThres, SIE_SMR **Smr)
{
    if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVAUTO_SMR_EN) == ENABLE)
    {
        *SmrActThres = 0x80;
        *FuncEn |= SMREn;
        *Update |= (SIE_SMR_ | SIE_SubFEn);
    }
    else
    {
        *Smr = NULL;
        *FuncEn &= ~SMREn;
        *Update |= SIE_SubFEn;
    }
}

void IPL_SIESetCA_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_CACC *cacc)
{
    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_EN) == ENABLE)
    {
        if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_TYPE) == IPLCTRL_CA_TYPE_32BIT)
        {
            cacc->CAPack = _32BIT;
        }
        else if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_TYPE) == IPLCTRL_CA_TYPE_64BIT)
        {
            cacc->CAPack = _64BIT;
        }
        cacc->CA_WIN.Start.x = 0;
        cacc->CA_WIN.Start.y = 0;
        cacc->CA_WIN.WinNum_X = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_WINXNUM);
        cacc->CA_WIN.WinNum_Y = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_WINYNUM);
        cacc->CA_WIN.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H);
        cacc->CA_WIN.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V);
        *FuncEn |= CACCEn;
        *Update |= (SIE_CA_ | SIE_SubFEn);
    }
    else
    {
        *FuncEn &= ~CACCEn;
        *Update |= SIE_SubFEn;
    }
}

void IPL_SIESetCAVIG_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_UPDATE *Update, SIE_CA_VIG **cavig)
{
    DBG_ERR("CA VIG Setting not ready\r\n");
    *cavig = NULL;
    //*FuncEn |= VIGEn;
    //*Update |= (SIE_VIG_ | SIE_SubFEn);
}

#define harryisgod2
#ifdef harryisgod2
UINT32 EcsTbl[65][65];
SIE_EmbeddedCS ECS_Param;

static void InitEcsTbl(void)
{
    UINT32 i,j,Rgain,Ggain,Bgain,Data;

    for(j=0;j<65;j++)
    {
        for(i=0;i<65;i++)
        {
#if 1
            Rgain = 0x100;
            Ggain = 0x100;
            Bgain = 0x100;
#else
            Rgain = 0x100*165/100;
            Ggain = 0x100;
            Bgain = 0x100*170/100;
#endif
            Data = (Bgain & 0x3FF) | ((Ggain & 0x3FF)<<10) | ((Rgain & 0x3FF)<<20);
            EcsTbl[j][i] = Data;
        }
    }
    ECS_Param.BitSel = ECS_D2F8;
    ECS_Param.DthrEn = DISABLE;
    ECS_Param.DthrRst = DISABLE;
    ECS_Param.ECSDthrLv = 0;
}
#endif

void IPL_SIESetECS_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *subs)
{
    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_ECS_EN) == ENABLE)
    {
        InitEcsTbl();
        subs->Ecs.pecs = &ECS_Param;
        subs->Ecs.Inaddr = (UINT32)&EcsTbl[0][0];
        subs->Ecs.EcsMap = ecs65x65;

        *FuncEn |= ECSEn;
        *Update |= (SIE_ECS_ | SIE_SubFEn);
    }
    else
    {
        *FuncEn &= ~ECSEn;
        *Update |= SIE_SubFEn;
    }
}


void IPL_SIESetBP_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *Sub)
{
    //bp
    Sub->BP.BP1 = 10;
    Sub->BP.BP2 = 20;
    Sub->BP.BP3 = 30;
    Sub->BP.BP1_ref = FALSE;
    Sub->BP.BP2_ref = FALSE;
    Sub->BP.BP3_ref = FALSE;
    *InteEn |= (SIE__INTE_BP1|SIE__INTE_BP2|SIE__INTE_BP3);
    *Update |= (SIE_BP_ | SIE_SubFEn);
}

void IPL_SIESetDMAOutSrc_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update)
{
    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_EN) == ENABLE)
    {
        *FuncEn |= DMAOutCh0En;
        *Update |= (SIE_SubFEn|SIE_CH0Out_SRC);
    }

    if ((IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_RAW_EN) == ENABLE) ||
        (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_EN) == ENABLE))
    {
        *FuncEn |= DMAOutCh2En;
        *Update |= (SIE_SubFEn|SIE_CH2Out_SRC);
    }
}
#if 0
#endif
void IPL_IPESetEthOut_FCB(UINT32 Id, IPEFUNCEN *FuncEn, IPEINTE *InteEn, IPE_UPDATE *Update, IPE_SUBFUNC *Sub)
{
    UINT32 BufSize;

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_ETH_EN) == ENABLE)
    {
        IPL_BufGet(Id, IPLBUF_IPEETH_1, &Sub->EdgeThreshold.EthDmaInfo.Th0Addr, &BufSize);
        IPL_BufGet(Id, IPLBUF_IPEETH_2, &Sub->EdgeThreshold.EthDmaInfo.Th1Addr, &BufSize);
        IPL_BufGet(Id, IPLBUF_IPEETH_3, &Sub->EdgeThreshold.EthDmaInfo.Th2Addr, &BufSize);

        *FuncEn |= (IPE_EDGETHREEn);
        *InteEn |= (IPE__INTE_EDGET_DMA_E);
        *Update |= (IPE_THRES_Addr|IPE_SubFEn);
    }
    else
    {
        *FuncEn &= ~(IPE_EDGETHREEn);
        *InteEn &= ~(IPE__INTE_EDGET_DMA_E);
        *Update |= (IPE_SubFEn);
    }
}

void IPL_IPESetVOut_FCB(UINT32 Id, IPEFUNCEN *FuncEn, IPEINTE *InteEn, IPE_UPDATE *Update, IPE_SUBFUNC *Sub)
{
    if (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_V_EN) == ENABLE)
    {
        Sub->Vout.OutSel = v_Vch_out;
        *FuncEn |= (IPE_V_SUBEn);
        *Update |= (IPE_SubFEn);
    }
}

void IPL_IPESetCCOut_FCB(UINT32 Id, IPEFUNCEN *FuncEn, IPEINTE *InteEn, IPE_UPDATE *Update, IPE_SUBFUNC *Sub)
{
    if (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_LCA_EN) == ENABLE)
    {
        *FuncEn |= (IPE_CC_SUBEn);
        *Update |= (IPE_SubFEn);
    }
}

void IPL_SIE2CCIR_FCB(UINT32 Id, SIE2__DVI* pDvi)
{
    SENSOR_INFO SenInfo;
    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo2(Id, IPL_SEL_PRVSENMODE), &SenInfo);

    if (SenInfo.Mode->DVI->Fmt == SENSOR_DVI_CCIR601)
    {
        pDvi->SIEDVIFmt = _CCIR601;
    }
    else if (SenInfo.Mode->DVI->Fmt == SENSOR_DVI_CCIR656)
    {
        pDvi->SIEDVIFmt = _CCIR601;
    }
    else
    {
        DBG_ERR("unsupport input = %d\r\n", SenInfo.Mode->DVI->Fmt);
    }

    if (SenInfo.Mode->DVI->Mode == SENSOR_DVI_MODE_SD)
    {
        pDvi->DVIMode = _SDMode;
    }
    else if (SenInfo.Mode->DVI->Mode == SENSOR_DVI_MODE_HD)
    {
        pDvi->DVIMode = _HDMode;
    }
    else
    {
        DBG_ERR("unsupport input = %d\r\n", SenInfo.Mode->DVI->Mode);
    }

    if (SenInfo.Mode->DVI->ActSel == SENSOR_DVI_SEL_ACTWIN)
    {
        pDvi->CCIR656ActSel = _ACTWindow;
    }
    else if (SenInfo.Mode->DVI->ActSel == SENSOR_DVI_SEL_EAV)
    {
        pDvi->CCIR656ActSel = _EAV;
    }
    else
    {
        DBG_ERR("unsupport input = %d\r\n", SenInfo.Mode->DVI->ActSel);
    }

    pDvi->FldEn = SenInfo.Mode->DVI->FldEn;
    pDvi->FldInvEn = SenInfo.Mode->DVI->FldInvEn;
    pDvi->CCIR656InSwapEn = SenInfo.Mode->DVI->InSwapEn;
    pDvi->CCIR656OutSwapEn = SenInfo.Mode->DVI->OutSwapEn;

}
#if 0
#endif
void IPL_CtrlInitIpc_SIE2In(UINT32 Id, SENSOR_INFO *SenInfo, SIE2_WINDOW *Win)
{

    Win->Act.DIV_X = SenInfo->Mode->TransHD[0].DataStart;
    Win->Act.FLD_Y = SenInfo->Mode->TransVD[0].DataStart;
    Win->Act.SizeH = SenInfo->Mode->TransHD[0].DataSize;
    Win->Act.SizeV = SenInfo->Mode->TransVD[0].DataSize;
    Win->Act.CFA = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVSTPIX);
    Win->Crop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H);
    Win->Crop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V);
    Win->Crop.CFA = Win->Act.CFA;

    if ((Win->Act.SizeH < Win->Crop.SizeH) || (Win->Act.SizeV < Win->Crop.SizeV))
    {
        DBG_ERR("error act size (%d %d) < crop Size(%d %d)\r\n", Win->Act.SizeH, Win->Act.SizeV, Win->Crop.SizeH, Win->Crop.SizeV);
        Win->Crop.StartPix.x = 0;
        Win->Crop.StartPix.y = 0;
    }
    else
    {
        Win->Crop.StartPix.x = (Win->Act.SizeH - Win->Crop.SizeH) >> 1;
        Win->Crop.StartPix.y = (Win->Act.SizeV - Win->Crop.SizeV) >> 1;
    }
}

void IPL_CtrlInitIpc_SIE2Signal(UINT32 Id, SENSOR_INFO *SenInfo, UINT32 HDInv, UINT32 VDInv, UINT32 VDPhase, UINT32 HDPhase, UINT32 DPhase, IPP_SIE2_SIGNAL *Signal)
{
    Signal->Sie2_HDVD.HD_ini = HDInv;
    Signal->Sie2_HDVD.HD_offset = 0;
    Signal->Sie2_HDVD.HD_sync = SenInfo->Mode->TransHD[0].Sync;
    Signal->Sie2_HDVD.HD_total = SenInfo->Mode->TransHD[0].Period;

    Signal->Sie2_HDVD.VD_ini = VDInv;
    Signal->Sie2_HDVD.VD_offset = 0;
    Signal->Sie2_HDVD.VD_sync = SenInfo->Mode->TransVD[0].Sync;
    Signal->Sie2_HDVD.VD_total = SenInfo->Mode->TransVD[0].Period;

    Signal->SensorPhase.VD = VDPhase;
    Signal->SensorPhase.HD = HDPhase;
    Signal->SensorPhase.Data = DPhase;
}

void IPL_CtrlInitIpc_SIE2Ch0PPB(UINT32 Id, UINT32 *PP1Addr, UINT32 *PP2Addr)
{
    UINT32 BufSize;

    if (PP1Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_SIERAW_CH0_1, PP1Addr, &BufSize);
    }

    if (PP2Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_SIERAW_CH0_2, PP2Addr, &BufSize);
    }
}


#if 0
#endif
void IPL_CtrlInitIpc_SIEAct(UINT32 Id, SENSOR_INFO *SenInfo, SieACTMode *ActMode)
{
    if ((SenInfo->DataType == SENSOR_DATA_LVDS) || (SenInfo->DataType == SENSOR_DATA_MIPI))
    {
        *ActMode = Serial;
    }
    else if (SenInfo->DataType == SENSOR_DATA_PARALLEL)
    {
        if (SenInfo->SigType == SENSOR_SIGNAL_MASTER)
        {
            *ActMode = Para_M;
        }
        else if (SenInfo->SigType == SENSOR_SIGNAL_SLAVE)
        {
            *ActMode = Para_S;
        }
        else
        {
            DBG_ERR("unsupport signal type %d\r\n", SenInfo->SigType);
        }
    }
    else if (SenInfo->DataType == SENSOR_DATA_DUMMY)
    {
        *ActMode = PATGEN;
    }
    else
    {
        DBG_ERR("unsupport Data Type %d\r\n", SenInfo->DataType);
    }
}

void IPL_CtrlInitIpc_SIEBurstLength(UINT32 Id, SENSOR_INFO *SenInfo, SIE_BURST_LENG *Length)
{
    *Length = IPL_CtrlGetInfor(Id, IPLCTRL_PRVBURSTLENGTH);
}

void IPL_CtrlInitIpc_SIEIn(UINT32 Id, SENSOR_INFO *SenInfo, SIE_WINDOW *Win)
{
    UINT32 i;
    for (i = 0; i < SIE_MAX_DIV; i ++)
    {
        Win->Act.DIV_X[i] = 0;
    }

    for (i = 0; i < SenInfo->Mode->DIVNum; i ++)
    {
        Win->Act.DIV_X[i] = SenInfo->Mode->TransHD[i].DataStart;
    }

    for (i = 0; i < MAX_FLD; i ++)
    {
        Win->Act.FLD_Y[i] = 0;
    }

    for (i = 0; i < SenInfo->Mode->FieldNum; i ++)
    {
        Win->Act.FLD_Y[i] = SenInfo->Mode->TransVD[i].DataStart;
    }

    Win->Act.SizeH = SenInfo->Mode->TransHD[0].DataSize;
    Win->Act.SizeV = SenInfo->Mode->TransVD[0].DataSize;
    Win->Act.CFA = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVSTPIX);

    Win->Crop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H);
    Win->Crop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V);
    Win->Crop.CFA = Win->Act.CFA;

    if ((Win->Act.SizeH < Win->Crop.SizeH) || (Win->Act.SizeV < Win->Crop.SizeV))
    {
        DBG_ERR("error act size (%d %d) < crop Size(%d %d)\r\n", Win->Act.SizeH, Win->Act.SizeV, Win->Crop.SizeH, Win->Crop.SizeV);
        Win->Crop.StartPix.x = 0;
        Win->Crop.StartPix.y = 0;
    }
    else
    {
        Win->Crop.StartPix.x = (Win->Act.SizeH - Win->Crop.SizeH) >> 1;
        Win->Crop.StartPix.y = (Win->Act.SizeV - Win->Crop.SizeV) >> 1;
    }
}

void IPL_CtrlInitIpc_SIESignal(UINT32 Id, SENSOR_INFO *SenInfo, UINT32 HDInv, UINT32 VDInv, UINT32 VDPhase, UINT32 HDPhase, UINT32 DPhase, IPP_SIE_SIGNAL *Signal)
{
    UINT32 i, j;
    Signal->Sie_HDVD.HD_ini = HDInv;
    Signal->Sie_HDVD.HD_offset = 0;
    Signal->Sie_HDVD.HD_sync = SenInfo->Mode->TransHD[0].Sync;
    Signal->Sie_HDVD.HD_total = SenInfo->Mode->TransHD[0].Period;

    Signal->Sie_HDVD.VD_ini = VDInv;
    Signal->Sie_HDVD.VD_offset = 0;
    Signal->Sie_HDVD.VD_sync = SenInfo->Mode->TransVD[0].Sync;
    Signal->Sie_HDVD.VD_total = SenInfo->Mode->TransVD[0].Period;

    Signal->SensorPhase.VD = VDPhase;
    Signal->SensorPhase.HD = HDPhase;
    Signal->SensorPhase.Data = DPhase;

    Signal->Sie_ReOrder.Reorder_VEn = DISABLE;

    Signal->Sie_FLD.SieFldNum = SenInfo->Mode->FieldNum - 1;
    for (i = 0; i < FieldCycle; i ++)
    {
        for (j = 0; j < MAX_FLD; j ++)
        {
            if (SenInfo->Mode->FieldTab == NULL)
            {
                Signal->Sie_FLD.FldLine[i][j] = 0;
            }
            else
            {
                Signal->Sie_FLD.FldLine[i][j] = SenInfo->Mode->FieldTab->Map[(i * MAX_FLD) + j] - 1;
            }
        }
    }

    Signal->SieDivNum = SenInfo->Mode->DIVNum - 1;
}

void IPL_CtrlInitIpc_SIECh0PPB(UINT32 Id, IPL_RAW_IMG_INFO *PPB0, UINT32 *PP1Addr, UINT32 *PP2Addr)
{
    UINT32 BufSize;

    IPL_BufGet(Id, IPLBUF_SIERAW_CH0_1, &PPB0->PixelAddr, &BufSize);

    PPB0->Width = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
    PPB0->Height = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V);
    PPB0->Bit = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT);
    PPB0->LineOffset = IPL_UTI_RAWH_CONV2_LOFS(PPB0->Width, PPB0->Bit);
    PPB0->STPix = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVSTPIX);

    if (PP1Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_SIERAW_CH0_2, PP1Addr, &BufSize);
    }

    if (PP2Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_SIERAW_CH0_3, PP2Addr, &BufSize);
    }
}

void IPL_CtrlInitIpc_SIECh2PPB(UINT32 Id, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV)
{
    UINT32 BufSize;

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_RAW_BUFNUM) == 1)
    {
        IPL_BufGet(Id, IPLBUF_SIERAW_CH2_1, &Out->PPB0_Addr, &BufSize);
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_RAW_BUFNUM) == 2)
    {
        IPL_BufGet(Id, IPLBUF_SIERAW_CH2_1, &Out->PPB0_Addr, &BufSize);
        IPL_BufGet(Id, IPLBUF_SIERAW_CH2_2, &Out->PPB1_Addr, &BufSize);
    }
    *OutH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_RAW_SIZE_H);
    *OutV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_RAW_SIZE_V);
    Out->packBus = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_PRVBIT);
    Out->LineOffset = IPL_UTI_RAWH_CONV2_LOFS(*OutH, Out->packBus);
}

void IPL_CtrlInitIpc_SIEVaPPB(UINT32 Id, SIE_Dma_Out *Out)
{
    UINT32 BufSize;

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_BUFNUM) == 1)
    {
        IPL_BufGet(Id, IPLBUF_SIEVA_1, &Out->PPB0_Addr, &BufSize);
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_BUFNUM) == 2)
    {
        IPL_BufGet(Id, IPLBUF_SIEVA_1, &Out->PPB0_Addr, &BufSize);
        IPL_BufGet(Id, IPLBUF_SIEVA_2, &Out->PPB1_Addr, &BufSize);
    }
}

void IPL_CtrlInitIpc_SIECaPPB(UINT32 Id, SIE_CH1_Dma_Out *DmaOut)
{
    UINT32 BufSize;
    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_BUFNUM) == 1)
    {
        IPL_BufGet(Id, IPLBUF_SIECA_1, &DmaOut->PPB0_Addr, &BufSize);
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_BUFNUM) == 2)
    {
        IPL_BufGet(Id, IPLBUF_SIECA_1, &DmaOut->PPB0_Addr, &BufSize);
        IPL_BufGet(Id, IPLBUF_SIECA_2, &DmaOut->PPB1_Addr, &BufSize);
    }
    else
    {
        DBG_ERR("CA PPB buffer error %d\r\n", IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_BUFNUM));
    }
}

void IPL_CtrlInitIpc_PREInPPB(UINT32 Id, IPL_RAW_IMG_INFO *PPB0, UINT32 *PP1Addr)
{
    UINT32 BufSize;

    IPL_BufGet(Id, IPLBUF_SIERAW_CH0_1, &PPB0->PixelAddr, &BufSize);

    if (PP1Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_SIERAW_CH0_2, PP1Addr, &BufSize);
    }

    PPB0->Width = IPL_CtrlGetInfor(Id, IPLCTRL_PRE_IN_SIZE_H);
    PPB0->Height = IPL_CtrlGetInfor(Id, IPLCTRL_PRE_IN_SIZE_V);
    PPB0->Bit = IPL_CtrlGetInfor(Id, IPLCTRL_PRE_IN_BIT);
    PPB0->LineOffset = IPL_CtrlGetInfor(Id, IPLCTRL_PRE_IN_SIZE_HLOS);
    PPB0->STPix = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVSTPIX);

}

void IPL_CtrlInitIpc_DCEIn(UINT32 Id, SENSOR_INFO *SenInfo, USIZE* pRef_1x, Coordinate *pCenter_1x)
{
    if (SenInfo->DataType == SENSOR_DATA_DUMMY)
    {
        pRef_1x->w = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H);
        pRef_1x->h = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V);
    }
    else
    {
        pRef_1x->w = SenInfo->Mode->TransHD[0].DataSize;
        pRef_1x->h = SenInfo->Mode->TransVD[0].DataSize;
    }
    pCenter_1x->x = (pRef_1x->w >> 1);
    pCenter_1x->y = (pRef_1x->h >> 1);
}

void IPL_CtrlInitIpc_IPELcaPPB(UINT32 Id, UINT32 *PP0Addr, UINT32 *PP1Addr, UINT32 *PP2Addr, UINT32 *PP3Addr)
{
    UINT32 BufSize;

    if (PP0Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IPELCA_1, PP0Addr, &BufSize);
    }

    if (PP1Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IPELCA_2, PP1Addr, &BufSize);
    }

    if (PP2Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IPELCA_3, PP2Addr, &BufSize);
    }

    if (PP3Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IPELCA_4, PP3Addr, &BufSize);
    }
}

void IPL_CtrlInitIpc_IPEVPPB(UINT32 Id, UINT32 *PP0Addr, UINT32 *PP1Addr, UINT32 *PP2Addr, UINT32 *PP3Addr)
{
    UINT32 BufSize;

    if (PP0Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IPEV_1, PP0Addr, &BufSize);
    }

    if (PP1Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IPEV_2, PP1Addr, &BufSize);
    }

    if (PP2Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IPEV_3, PP2Addr, &BufSize);
    }

    if (PP3Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IPEV_4, PP3Addr, &BufSize);
    }
}

void IPL_CtrlInitIpc_IPEVaPPB(UINT32 Id, UINT32 *PP0Addr, UINT32 *PP1Addr)
{
    UINT32 BufSize;

    if (PP0Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IPEVA_1, PP0Addr, &BufSize);
    }

    if (PP1Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IPEVA_2, PP1Addr, &BufSize);
    }
}

void IPL_CtrlInitIpc_IFE2LcaPPB(UINT32 Id, UINT32 *PP0Addr, UINT32 *PP1Addr)
{
    UINT32 BufSize;

    if (PP0Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IPEIFE2LCA_1, PP0Addr, &BufSize);
    }

    if (PP1Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IPEIFE2LCA_2, PP1Addr, &BufSize);
    }
}

void IPL_CtrlInitIpc_IMEPxOutPPB(UINT32 Id, UINT32 PathEnIdx, IPL_YCC_IMG_INFO *Px_PPB0)
{
    UINT32 BufSize, Px;
    IPL_IMG_BUF_INFO BufInfo;
    IPL_YCC_IMG_INFO YCCInfo, YCCAddrInfo;

    UINT32 PxIdx[3][7] =
    {
        {IPLCTRL_IME_P1_FMT, IPLCTRL_IME_P1_BUF_SIZE_H, IPLCTRL_IME_P1_BUF_SIZE_V, IPLCTRL_IME_P1_OUT_SIZE_H, IPLCTRL_IME_P1_OUT_SIZE_V, IPLCTRL_IME_P1_OUT_SIZE_HLOS, IPLBUF_IMEP1_1},
        {IPLCTRL_IME_P2_FMT, IPLCTRL_IME_P2_BUF_SIZE_H, IPLCTRL_IME_P2_BUF_SIZE_V, IPLCTRL_IME_P2_OUT_SIZE_H, IPLCTRL_IME_P2_OUT_SIZE_V, IPLCTRL_IME_P2_OUT_SIZE_HLOS, IPLBUF_IMEP2_1},
        {IPLCTRL_IME_P3_FMT, IPLCTRL_IME_P3_BUF_SIZE_H, IPLCTRL_IME_P3_BUF_SIZE_V, IPLCTRL_IME_P3_OUT_SIZE_H, IPLCTRL_IME_P3_OUT_SIZE_V, IPLCTRL_IME_P3_OUT_SIZE_HLOS, IPLBUF_IMEP3_1},
    };

    if (PathEnIdx == IPLCTRL_IME_P1_EN) Px = 0;
    else if (PathEnIdx == IPLCTRL_IME_P2_EN) Px = 1;
    else if (PathEnIdx == IPLCTRL_IME_P3_EN) Px = 2;
    else
    {
        DBG_ERR("PathEnIdx error %d\r\n", PathEnIdx);
        return;
    }


    //calculate address
    YCCAddrInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][0]);
    YCCAddrInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][1]);
    YCCAddrInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][2]);
    YCCAddrInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, PxIdx[Px][1]);
    YCCAddrInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCAddrInfo.type, YCCAddrInfo.Ch[IPL_IMG_YUV_Y]);
    YCCAddrInfo.Ch[IPL_IMG_YUV_V] = YCCAddrInfo.Ch[IPL_IMG_YUV_U];
    IPL_BufGet(Id, PxIdx[Px][6], &BufInfo.Buf[0], &BufSize);
    BufInfo.Buf[1] = 0;
    BufInfo.Buf[2] = 0;
    IPL_UTI_SET_CONT_BUFINFO(YCCAddrInfo.type, BufInfo, &YCCAddrInfo);

    //calculate ycc information
    YCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][0]);
    YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][3]);
    YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][4]);
    YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, PxIdx[Px][5]);
    YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
    YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];

    YCCInfo.PixelAddr[IPL_IMG_YUV_Y] = YCCAddrInfo.PixelAddr[IPL_IMG_YUV_Y];
    YCCInfo.PixelAddr[IPL_IMG_YUV_U] = YCCAddrInfo.PixelAddr[IPL_IMG_YUV_U];
    YCCInfo.PixelAddr[IPL_IMG_YUV_V] = YCCAddrInfo.PixelAddr[IPL_IMG_YUV_V];

    *Px_PPB0 = YCCInfo;
}

void IPL_CtrlInitIpc_DispPxPPB(UINT32 Id, UINT32 PathEnIdx)
{
    UINT32 BufSize, Px, i;
    IPL_IMG_BUF_INFO BufInfo;
    IPL_YCC_IMG_INFO YCCInfo;

    UINT32 PxIdx[3][7] =
    {
        {IPLCTRL_IME_P1_BUFNUM, IPLCTRL_IME_P1_FMT, IPLCTRL_IME_P1_BUF_SIZE_H, IPLCTRL_IME_P1_BUF_SIZE_V, IPLBUF_IMEP1_1, IPLCTRL_IME_P1_DISP_SIZE_H, IPLCTRL_IME_P1_DISP_SIZE_V},
        {IPLCTRL_IME_P2_BUFNUM, IPLCTRL_IME_P2_FMT, IPLCTRL_IME_P2_BUF_SIZE_H, IPLCTRL_IME_P2_BUF_SIZE_V, IPLBUF_IMEP2_1, IPLCTRL_IME_P2_DISP_SIZE_H, IPLCTRL_IME_P2_DISP_SIZE_V},
        {IPLCTRL_IME_P3_BUFNUM, IPLCTRL_IME_P3_FMT, IPLCTRL_IME_P3_BUF_SIZE_H, IPLCTRL_IME_P3_BUF_SIZE_V, IPLBUF_IMEP3_1, IPLCTRL_IME_P3_DISP_SIZE_H, IPLCTRL_IME_P3_DISP_SIZE_V},
    };


    if (PathEnIdx == IPLCTRL_IME_P1_EN) Px = 0;
    else if (PathEnIdx == IPLCTRL_IME_P2_EN) Px = 1;
    else if (PathEnIdx == IPLCTRL_IME_P3_EN) Px = 2;
    else
    {
        DBG_ERR("PathEnIdx error %d\r\n", PathEnIdx);
        return;
    }

    YCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][1]);
    YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][2]);
    YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, PxIdx[Px][2]);
    YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][3]);
    YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
    YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];


    //set dispaly buffer
    for (i = 0; i < IPL_CtrlGetInfor(Id, PxIdx[Px][0]); i ++)
    {
        IPL_BufGet(Id, (PxIdx[Px][4] + i), &BufInfo.Buf[0], &BufSize);
        BufInfo.Buf[1] = 0;
        BufInfo.Buf[2] = 0;
        IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, BufInfo, &YCCInfo);
        if (Id == IPL_ID_1)
        {
            IPL_InitDisplayBuffer(Px, i, &YCCInfo);
        }
        else if (Id == IPL_ID_2)
        {
            IPL_InitDisplayBuffer2(Px, i, &YCCInfo);
        }
        else
        {
            DBG_ERR("unsupport id %d\r\n", Id);
        }
    }

    YCCInfo.Ch[0].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][5]);
    YCCInfo.Ch[0].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][6]);

    if (Id == IPL_ID_1)
    {
        IPL_SetDeviceInputInfo(Px, YCCInfo.Ch[0]);
    }
    else if (Id == IPL_ID_2)
    {
        IPL_SetDeviceInputInfo2(Px, YCCInfo.Ch[0]);
    }
    else
    {
        DBG_ERR("unsupport id %d\r\n", Id);
    }
}

void IPL_CtrlInitIpc_DispCtrlEn(UINT32 Id, UINT32 PathEnIdx, UINT32 EnFlag)
{
    UINT32 Px;

    if (PathEnIdx == IPLCTRL_IME_P1_EN) Px = 0;
    else if (PathEnIdx == IPLCTRL_IME_P2_EN) Px = 1;
    else if (PathEnIdx == IPLCTRL_IME_P3_EN) Px = 2;
    else
    {
        DBG_ERR("PathEnIdx error %d\r\n", PathEnIdx);
        return;
    }

    IPL_SetPostImageProcessOnOff(Px, EnFlag);
}

#if 0
#endif

void IPL_CtrlInitIpc_SIE2In_Cap(UINT32 Id,  SIE2_UPDATE *Update, SIE2_WINDOW *Win)
{
    SENSOR_INFO SenInfo;

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo2(Id, IPL_SEL_CAPSENMODE), &SenInfo);

    Win->Act.DIV_X = SenInfo.Mode->TransHD[0].DataStart;
    Win->Act.FLD_Y = SenInfo.Mode->TransVD[0].DataStart;

    Win->Act.SizeH = SenInfo.Mode->TransHD[0].DataSize;
    Win->Act.SizeV = SenInfo.Mode->TransVD[0].DataSize;
    Win->Act.CFA = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CAPSTPIX);

    Win->Crop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CCIR_CAP_RAW_SIZE_H);
    Win->Crop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CCIR_CAP_RAW_SIZE_V);
    Win->Crop.CFA = Win->Act.CFA;

    if ((Win->Act.SizeH < Win->Crop.SizeH) || (Win->Act.SizeV < Win->Crop.SizeV))
    {
        DBG_ERR("error act size (%d %d) < crop Size(%d %d)\r\n", Win->Act.SizeH, Win->Act.SizeV, Win->Crop.SizeH, Win->Crop.SizeV);
        Win->Crop.StartPix.x = 0;
        Win->Crop.StartPix.y = 0;
    }
    else
    {
        Win->Crop.StartPix.x = (Win->Act.SizeH - Win->Crop.SizeH) >> 1;
        Win->Crop.StartPix.y = (Win->Act.SizeV - Win->Crop.SizeV) >> 1;
    }
    *Update |= SIE_IOSize;
}

#if 0
#endif

void IPL_CtrlInitIpc_SIE2Signal_Cap(UINT32 Id, SIE2_UPDATE *Update, UINT32 HDInv, UINT32 VDInv, UINT32 VDPhase, UINT32 HDPhase, UINT32 DPhase, IPP_SIE2_SIGNAL *Signal)
{
    SENSOR_INFO SenInfo;

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo2(Id, IPL_SEL_CAPSENMODE), &SenInfo);

    Signal->Sie2_HDVD.HD_ini = HDInv;
    Signal->Sie2_HDVD.HD_offset = 0;
    Signal->Sie2_HDVD.HD_sync = SenInfo.Mode->TransHD[0].Sync;
    Signal->Sie2_HDVD.HD_total = SenInfo.Mode->TransHD[0].Period;

    Signal->Sie2_HDVD.VD_ini = VDInv;
    Signal->Sie2_HDVD.VD_offset = 0;
    Signal->Sie2_HDVD.VD_sync = SenInfo.Mode->TransVD[0].Sync;
    Signal->Sie2_HDVD.VD_total = SenInfo.Mode->TransVD[0].Period;

    Signal->SensorPhase.VD = VDPhase;
    Signal->SensorPhase.HD = HDPhase;
    Signal->SensorPhase.Data = DPhase;

    *Update |= SIE2_INIT;
}

void IPL_CtrlInitIpc_SIE2Ch0PPB_Cap(UINT32 Id, SIE2_UPDATE *Update, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV)
{
    *OutH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CCIR_CAP_RAW_SIZE_H);
    *OutV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CCIR_CAP_RAW_SIZE_V);
    Out->packBus = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CCIR_CAPBIT);
    Out->LineOffset = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CCIR_CAP_RAW_SIZE_HLOS);
    *Update |= SIE2_IOSize;
}


void IPL_CtrlInitIpc_SIETrigSel_Cap(UINT32 Id, SIE_UPDATE *Update, SIEPreTrigSel *TrigSel)
{
    *Update |= SIE_INIT;
    *TrigSel = Cur_FLDD;
}

void IPL_CtrlInitIpc_SIEBurstLength_Cap(UINT32 Id, SIE_UPDATE *Update, SENSOR_INFO *SenInfo, SIE_BURST_LENG *Length)
{
    *Update |= SIE_BURSTLENG;
    *Length = IPL_CtrlGetInfor(Id, IPLCTRL_CAPBURSTLENGTH);
}

void IPL_CtrlInitIpc_SIEAct_Cap(UINT32 Id, SIE_UPDATE *Update, SieACTMode *ActMode)
{
    SENSOR_INFO SenInfo;

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo2(Id, IPL_SEL_CAPSENMODE), &SenInfo);

    if ((SenInfo.DataType == SENSOR_DATA_LVDS) || (SenInfo.DataType == SENSOR_DATA_MIPI))
    {
        *ActMode = Serial;
        *Update |= SIE_INIT;
    }
    else if (SenInfo.DataType == SENSOR_DATA_PARALLEL)
    {
        if (SenInfo.SigType == SENSOR_SIGNAL_MASTER)
        {
            *ActMode = Para_M;
            *Update |= SIE_INIT;
        }
        else if (SenInfo.SigType == SENSOR_SIGNAL_SLAVE)
        {
            *ActMode = Para_S;
            *Update |= SIE_INIT;
        }
        else
        {
            DBG_ERR("unsupport signal type %d\r\n", SenInfo.SigType);
        }
    }
    else if (SenInfo.DataType == SENSOR_DATA_DUMMY)
    {
        *ActMode = PATGEN;
        *Update |= SIE_INIT;
    }
    else
    {
        DBG_ERR("unsupport Data Type %d\r\n", SenInfo.DataType);
    }
}

void IPL_CtrlInitIpc_SIESignal_Cap(UINT32 Id, SIE_UPDATE *Update, UINT32 HDInv, UINT32 VDInv, UINT32 VDPhase, UINT32 HDPhase, UINT32 DPhase, IPP_SIE_SIGNAL *Signal)
{
    UINT32 i, j;
    SENSOR_INFO SenInfo;

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo2(Id, IPL_SEL_CAPSENMODE), &SenInfo);

    Signal->Sie_HDVD.HD_ini = HDInv;
    Signal->Sie_HDVD.HD_offset = 0;
    Signal->Sie_HDVD.HD_sync = SenInfo.Mode->TransHD[0].Sync;
    Signal->Sie_HDVD.HD_total = SenInfo.Mode->TransHD[0].Period;

    Signal->Sie_HDVD.VD_ini = VDInv;
    Signal->Sie_HDVD.VD_offset = 0;
    Signal->Sie_HDVD.VD_sync = SenInfo.Mode->TransVD[0].Sync;
    Signal->Sie_HDVD.VD_total = SenInfo.Mode->TransVD[0].Period;

    Signal->SensorPhase.VD = VDPhase;
    Signal->SensorPhase.HD = HDPhase;
    Signal->SensorPhase.Data = DPhase;

    Signal->Sie_ReOrder.Reorder_VEn = DISABLE;
    Signal->Sie_FLD.SieFldNum = SenInfo.Mode->FieldNum - 1;
    for (i = 0; i < FieldCycle; i ++)
    {
        for (j = 0; j < MAX_FLD; j ++)
        {
            if (SenInfo.Mode->FieldTab == NULL)
            {
                Signal->Sie_FLD.FldLine[i][j] = 0;
            }
            else
            {
                Signal->Sie_FLD.FldLine[i][j] = SenInfo.Mode->FieldTab->Map[(i * MAX_FLD) + j] - 1;
            }
        }
    }

    Signal->SieDivNum = SenInfo.Mode->DIVNum - 1;
    *Update |= (SIE_FILD_|SIE_INIT);
}

void IPL_CtrlInitIpc_SIECh0PPB_Cap(UINT32 Id, SIE_UPDATE *Update, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV)
{
    *OutH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H);
    *OutV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V);
    Out->packBus = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAPBIT);
    Out->LineOffset = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS);
    *Update |= SIE_IOSize;
}

void IPL_CtrlInitIpc_SIECh2PPB_Cap(UINT32 Id,  SIE_UPDATE *Update, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV)
{
    *OutH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_CAP_RAW_SIZE_H);
    *OutV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_CAP_RAW_SIZE_V);
    Out->packBus = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_CAPBIT);
    Out->LineOffset = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_CAP_RAW_SIZE_HLOS);
    *Update |= SIE_IOSize;
}
void IPL_CtrlInitIpc_SIEIn_Cap(UINT32 Id,  SIE_UPDATE *Update, SIE_WINDOW *Win)
{
    UINT32 i;
    SENSOR_INFO SenInfo;

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo2(Id, IPL_SEL_CAPSENMODE), &SenInfo);

    for (i = 0; i < SIE_MAX_DIV; i ++)
    {
        Win->Act.DIV_X[i] = 0;
    }

    for (i = 0; i < SenInfo.Mode->DIVNum; i ++)
    {
        Win->Act.DIV_X[i] = SenInfo.Mode->TransHD[i].DataStart;
    }

    for (i = 0; i < MAX_FLD; i ++)
    {
        Win->Act.FLD_Y[i] = 0;
    }

    for (i = 0; i < SenInfo.Mode->FieldNum; i ++)
    {
        Win->Act.FLD_Y[i] = SenInfo.Mode->TransVD[i].DataStart;
    }

    Win->Act.SizeH = SenInfo.Mode->TransHD[0].DataSize;
    Win->Act.SizeV = SenInfo.Mode->TransVD[0].DataSize;
    Win->Act.CFA = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CAPSTPIX);

    Win->Crop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CAPCROP_SIZE_H);
    Win->Crop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CAPCROP_SIZE_V);
    Win->Crop.CFA = Win->Act.CFA;

    if ((Win->Act.SizeH < Win->Crop.SizeH) || (Win->Act.SizeV < Win->Crop.SizeV))
    {
        DBG_ERR("error act size (%d %d) < crop Size(%d %d)\r\n", Win->Act.SizeH, Win->Act.SizeV, Win->Crop.SizeH, Win->Crop.SizeV);
        Win->Crop.StartPix.x = 0;
        Win->Crop.StartPix.y = 0;
    }
    else
    {
        Win->Crop.StartPix.x = (Win->Act.SizeH - Win->Crop.SizeH) >> 1;
        Win->Crop.StartPix.y = (Win->Act.SizeV - Win->Crop.SizeV) >> 1;
    }
    *Update |= SIE_IOSize;
}

void IPL_CtrlInitIpc_SIEOB_Cap(UINT32 Id,  SIE_UPDATE *Update, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_OBAVG *OBAvg, SIE_OBSUB *OBSub)
{
    SENSOR_INFO SenInfo;

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo2(Id, IPL_SEL_CAPSENMODE), &SenInfo);

    if (IPL_CtrlGetInfor(Id, IPLCTRL_CAPAUTO_OB_EN) == DISABLE)
    {
        //OBSub->Offset = 0xC0;
        OBSub->Offset = 0xa8;
        OBSub->DtGain = 0x20;
        *FuncEn |= OBSubSelEn;
        *Update |= (SIE_OBOFS_ | SIE_SubFEn);

    }
    else
    {
        OBAvg->StartPix.x = SenInfo.Mode->OB.TopWin[0];
        OBAvg->StartPix.y = SenInfo.Mode->OB.TopWin[1];
        OBAvg->SizeH = (SenInfo.Mode->OB.TopWin[2] - SenInfo.Mode->OB.TopWin[0]);
        OBAvg->SizeV = (SenInfo.Mode->OB.TopWin[3] - SenInfo.Mode->OB.TopWin[1]);
        OBAvg->AvgActThres = 0x80;
        OBSub->Offset = 0;
        OBSub->DtGain = 0x20;
        *FuncEn |= (OBSubSelEn | OBAvgEn);
        *Update |= (SIE_OBOFS_ | SIE_OBAVG_ | SIE_SubFEn);
    }
}

void IPL_CtrlInitIpc_SIEGamma_Cap(UINT32 Id,  SIE_UPDATE *Update, SieFuncEn *FuncEn, SIE_GAMMA **pGamma)
{
    *Update |= SIE_GAMMA_;
    *FuncEn |= GammaEn;

    *pGamma = &SieRawEncTab;
}

void IPL_CtrlInitIpc_SIERawEnc_Cap(UINT32 Id,  SIE_UPDATE *Update, SieFuncEn *FuncEn, SIE_RAWENC *RawEnc, UINT32 MaxBufSize, UINT32 Ratio)
{
    *Update |= SIE_INIT;
    *FuncEn |= RawEncEn;

    RawEnc->CompMaxSize = MaxBufSize;
    RawEnc->CompRate = Ratio;
}

#if 0
#endif
void IPL_CtrlInitPrvPProcInfo(UINT32 Id, IPL_POST_PROC_FUNC_EN FuncEn)
{
    IPL_PPROC_INFO Info = {0};

    if (FuncEn & IPL_FUNC_DISP)
    {
        IPL_CtrlInitIpc_DispCtrlEn(Id, IPLCTRL_IME_P1_EN, ENABLE);
        IPL_CtrlInitIpc_DispCtrlEn(Id, IPLCTRL_IME_P2_EN, ENABLE);
        IPL_CtrlInitIpc_DispCtrlEn(Id, IPLCTRL_IME_P3_EN, ENABLE);
        IPL_Display_Resume();
    }
    else
    {
        IPL_Display_Pause();
    }

    if ((FuncEn & IPL_FUNC_IMGEFFECT_WDR) && (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_B))
    {
        IPL_BufGet(Id, IPLBUF_IMGEFFECT_WDR, &Info.bufaddr, &Info.bufsize);
        Info.ExtData[0] = IPL_CtrlGetHalSetIdPProc(Id);
        Info.IsLockfp = IPL_PPostHDR_IsLock;
        Info.PProcfp = IPL_PPostHDR_Proc;
        IPL_PProc_LoadFunc2(Id, &Info);
    }


    if (FuncEn & IPL_FUNC_PHOTOFRAME)
    {
        PhotoFrame_Open();
        IPL_BufGet(Id, IPLBUF_PHOTOFRAME, &Info.bufaddr, &Info.bufsize);
        Info.IsLockfp = IPL_PPostPF_IsLock;
        Info.PProcfp = IPL_PPostPF_Proc;
        IPL_PProc_LoadFunc2(Id, &Info);
        {
            UINT32 i,j, data,temp;

            for(j=0;j<240;j++)
            {
                temp = (255-j);
                data = temp | (temp<<8) | (temp<<16) | (temp<<24);
                for(i=0;i<320;i+=4)
                    *(UINT32 *)(Info.bufaddr + j*320 +i) = data;
            }
        }
    }

    if (FuncEn & IPL_FUNC_STAMP)
    {
        IPL_BufGet(Id, IPLBUF_STAMP, &Info.bufaddr, &Info.bufsize);
        Info.IsLockfp = IPL_PPostDS_IsLock;
        Info.PProcfp = IPL_PPostDS_Proc;
        IPL_PProc_LoadFunc2(Id, &Info);
    }

    if (FuncEn & IPL_FUNC_RSC)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_RSC_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_RSC_RATIO, 10);
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_RSC_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_RSC_RATIO, 0);
    }

}


#if 0
#endif
void IPL_CtrlInitCapIpcInfo(IPL_GET_CAP_RAW_DATA *CapInfo, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    static UINT32 RawEncFirstFlag = TRUE;
    UINT32 Ch0HSize, Ch0VSize;
    IPL_HAL_READ readBit = {0};
    SENSOR_INFO SenInfo;
    SIE_UPDATE tmpUpdate;
    SIE_WINDOW tmpWin;
    IPC_QINFO QInfo;

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(CapInfo->Id), IPL_AlgGetUIInfo2(CapInfo->Id, IPL_SEL_CAPSENMODE), &SenInfo);

    if (IPLCTRL_CAP_FLOW_INVID == IPL_CtrlGetInfor(CapInfo->Id, IPLCTRL_CAPFLOW))
    {
        //read sie original settings
        readBit.sieRead = SIE_R_SubFEn;
        IPH_ENG_Read(CapInfo->Id, IPL_HAL_SIE, IPL_CtrlGetHalSetId(CapInfo->Id), readBit, (void *)&Info->psie->pSubFunc->sieFuncEn);
        readBit.sieRead = SIE_R_IO;
        IPH_ENG_Read(CapInfo->Id, IPL_HAL_SIE, IPL_CtrlGetHalSetId(CapInfo->Id), readBit, (void *)Info->psie);
        readBit.sieRead = SIE_R_Out;
        IPH_ENG_Read(CapInfo->Id, IPL_HAL_SIE, IPL_CtrlGetHalSetId(CapInfo->Id), readBit, (void *)Info->psie);

        readBit.sieRead = SIE_R_InSize;
        IPH_ENG_Read(CapInfo->Id, IPL_HAL_SIE, IPL_CtrlGetHalSetId(CapInfo->Id), readBit, (void *)&Info->psie->IMG_Window);

        //read pre dram in information(special case)
        QInfo = IPL_CTRLQuery(ICF_PREIO_RDY);

        /**************** save last sie information ****************/
        ExtInfo->Param[0] = Info->psie->Ch0.Out.PPB0_Addr;
        ExtInfo->Param[1] = Info->psie->Ch0.Out.PPB1_Addr;
        ExtInfo->Param[2] = Info->psie->Ch0.Out.LineOffset;
        ExtInfo->Param[3] = IPL_CtrlGetInfor(CapInfo->Id, IPLCTRL_PRVBURSTLENGTH);
        ExtInfo->Param[4] = Info->psie->IMG_Window.Crop.StartPix.x;
        ExtInfo->Param[5] = Info->psie->IMG_Window.Crop.StartPix.y;
        ExtInfo->Param[6] = Info->psie->IMG_Window.Crop.SizeH;
        ExtInfo->Param[7] = Info->psie->IMG_Window.Crop.SizeV;
        ExtInfo->Param[8] = QInfo.PreDmaIn_Rdy.InDma.PixelAddr;
        ExtInfo->Param[9] = QInfo.PreDmaIn_Rdy.PPB1_Addr;
        /**************** load new sie information ****************/
        IPL_CtrlInitIpc_SIEBurstLength_Cap(CapInfo->Id, &Info->sieUpdate, &SenInfo, &Info->psie->BurstLeng_Ch0);
        IPL_CtrlInitIpc_SIEIn_Cap(CapInfo->Id, &tmpUpdate, &tmpWin);
        Info->psie->IMG_Window.Crop.StartPix.x = tmpWin.Crop.StartPix.x;
        Info->psie->IMG_Window.Crop.StartPix.y = tmpWin.Crop.StartPix.y;
        Info->psie->IMG_Window.Crop.SizeH = tmpWin.Crop.SizeH;
        Info->psie->IMG_Window.Crop.SizeV = tmpWin.Crop.SizeV;

        IPL_CtrlInitIpc_SIECh0PPB_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->Ch0.Out, &Ch0HSize, &Ch0VSize);
        Info->psie->Ch0.Src = NonScaled_Img;
        Info->sieUpdate |= SIE_CH0Out_SRC;
    }
    else if (IPLCTRL_CAP_FLOW_SIM == IPL_CtrlGetInfor(CapInfo->Id, IPLCTRL_CAPFLOW))
    {
    }
    else if (IPLCTRL_CAP_FLOW_CCIR == IPL_CtrlGetInfor(CapInfo->Id, IPLCTRL_CAPFLOW))
    {
        if (CapInfo->OutCh0En == ENABLE)
        {
            Info->psie2->Ch0.Out.PPB0_Addr = CapInfo->BufAddrCh0;
            Info->psie2->Ch0.Out.PPB1_Addr = CapInfo->BufAddrCh0;
            Info->sie2Update |= SIE2_OutAddr_Ch0;
        }
    }
    else
    {
        IPL_CtrlInitIpc_SIEBurstLength_Cap(CapInfo->Id, &Info->sieUpdate, &SenInfo, &Info->psie->BurstLeng_Ch0);

        if (IPLCTRL_CAP_FLOW_EBURST == IPL_CtrlGetInfor(CapInfo->Id, IPLCTRL_CAPFLOW))
        {
            /**************** load cap task information ****************/
            ExtInfo->Param[0] = CapInfo->res[0];

            if (ExtInfo->Param[0] == 0)
            {
                RawEncFirstFlag = TRUE;
            }
            else if (ExtInfo->Param[0] == 1)
            {
                IPL_CtrlInitIpc_SIEGamma_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->pSubFunc->sieFuncEn, &Info->psie->pSubFunc->pGamma);
                IPL_CtrlInitIpc_SIERawEnc_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->pSubFunc->sieFuncEn,
                                                &Info->psie->pSubFunc->RawEnc, CapInfo->CompressMaxSize, CapInfo->CompressRatio);

                //only update gamma at first time
                if (RawEncFirstFlag == FALSE)
                {
                    Info->sieUpdate &= ~SIE_GAMMA_;
                }
                RawEncFirstFlag = FALSE;
            }
        }

        IPL_SIESetECS_FCB(CapInfo->Id, &Info->psie->pSubFunc->sieFuncEn, &Info->psie->pSubFunc->InteEn, &Info->sieUpdate, Info->psie->pSubFunc);


        IPL_CtrlInitIpc_SIETrigSel_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->TrigSel);
        //AR0330M
        //#if (_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_)
        IPL_CtrlInitIpc_SIESignal_Cap(CapInfo->Id, &Info->sieUpdate, TRUE, TRUE, _RISING, _RISING, _RISING, &Info->psie->Sie_Signal);
        //#else
        //IPL_CtrlInitIpc_SIESignal_Cap(CapInfo->Id, &Info->sieUpdate, FALSE, FALSE, _RISING, _RISING, _RISING, &Info->psie->Sie_Signal);
        //#endif

        IPL_CtrlInitIpc_SIEAct_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->ActMode);
        IPL_CtrlInitIpc_SIECh0PPB_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->Ch0.Out, &Ch0HSize, &Ch0VSize);
        IPL_CtrlInitIpc_SIECh2PPB_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->Ch2.Out, &Info->psie->DmaOutSizeH, &Info->psie->DmaOutSizeV);
        IPL_CtrlInitIpc_SIEIn_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->IMG_Window);
        Info->psie->Ch0.Src = NonScaled_Img;
        Info->psie->Ch2.Src = CH2_Scaled_Img;
        if ((Info->psie->IMG_Window.Crop.SizeH != Ch0HSize) || (Info->psie->IMG_Window.Crop.SizeV != Ch0VSize))
        {
            DBG_ERR("error ch0 output size (%d %d) < crop Size(%d %d)\r\n", Ch0HSize, Ch0VSize, Info->psie->IMG_Window.Crop.SizeH, Info->psie->IMG_Window.Crop.SizeV);
        }
        IPL_CtrlInitIpc_SIEOB_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->pSubFunc->sieFuncEn, &Info->psie->pSubFunc->InteEn, &Info->psie->pSubFunc->OBAvg, &Info->psie->pSubFunc->OBSub);

        Info->psie->pSubFunc->InteEn = SIE__INTE_FLDEND;
        Info->sieUpdate |= (SIE_CH0Out_SRC|SIE_CH2Out_SRC|SIE_SubFEn);
    }
	//#NT#2017/07/17#liwei cheng -begin
    //#NT# solve getraw timg issue
	Info->psie->pSubFunc->InteEn |= SIE__INTE_ACTST;
	//#NT#2017/07/17#liwei cheng -end

    if (CapInfo->OutCh0En == ENABLE)
    {
        Info->psie->Ch0.Out.PPB0_Addr = CapInfo->BufAddrCh0;
        Info->psie->Ch0.Out.PPB1_Addr = CapInfo->BufAddrCh0;
        Info->psie->pSubFunc->sieFuncEn |= DMAOutCh0En;
        Info->sieUpdate |= SIE_OutAddr_Ch0;
    }

    if (CapInfo->OutCh2En == ENABLE)
    {
        Info->psie->Ch2.Out.PPB0_Addr = CapInfo->BufAddrCh2;
        Info->psie->Ch2.Out.PPB1_Addr = CapInfo->BufAddrCh2;
        Info->psie->pSubFunc->sieFuncEn |= DMAOutCh2En;
        Info->sieUpdate |= SIE_OutAddr_Ch2;
    }
}

void IPL_CtrlInitCapIpcInfo_CCIR(IPL_GET_CAP_RAW_DATA *CapInfo, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    UINT32 Height = 0;
    if ((IPLCTRL_CAP_FLOW_CCIR == IPL_CtrlGetInfor(CapInfo->Id, IPLCTRL_CAPFLOW)) ||
        (IPLCTRL_CAP_FLOW_DUALSENSOR == IPL_CtrlGetInfor(CapInfo->Id, IPLCTRL_CAPFLOW)))
    {
        IPL_CtrlInitIpc_SIE2In_Cap(CapInfo->Id, &Info->sie2Update, &Info->psie2->IMG_Window);
        IPL_CtrlInitIpc_SIE2Signal_Cap(CapInfo->Id, &Info->sie2Update, TRUE, TRUE, _FALLING, _FALLING, _RISING, &Info->psie2->Sie2_Signal);
        IPL_CtrlInitIpc_SIE2Ch0PPB_Cap(CapInfo->Id, &Info->sie2Update, &Info->psie2->Ch0.Out, &Info->psie2->DmaOutSizeH, &Height);
        IPL_SIE2CCIR_FCB(CapInfo->Id, &Info->psie2->pSubFunc->CCIR);
        Info->psie2->Ch0.Src = NonScaled_Img;

        Info->psie2->pSubFunc->InteEn |= SIE2__INTE_FLDEND;
        Info->psie2->pSubFunc->sie2FuncEn |= DviEn_;
        Info->sie2Update |= (SIE2_SubFEn|SIE2_CH0Out_SRC|SIE2_DVI_);

        if (CapInfo->OutCh0En == ENABLE)
        {
            Info->psie2->pSubFunc->sie2FuncEn |= DMAOutCh0En_;
            Info->psie2->Ch0.Out.PPB0_Addr = CapInfo->BufAddrCh0;
            Info->psie2->Ch0.Out.PPB1_Addr = CapInfo->BufAddrCh0;
            Info->sie2Update |= SIE2_OutAddr_Ch0;
        }
    }
    else
    {
        DBG_ERR("flow error\r\n");
    }
}

void IPL_CtrlInitPrvIpcInfo(IPL_PROC_ID Id, IPC_INFO *IpcInfo, IPL_ISR_OBJ_INFO *IsrInfo, IPL_CHGMODE_DATA *ChgMode)
{
    UINT32 i;

    if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_A)
    {
        IPL_CtrlInitIpcInfoFlowA(Id, IpcInfo);
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_B)
    {
        IPL_CtrlInitIpcInfoFlowB(Id, IpcInfo);
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_C)
    {
        IPL_CtrlInitIpcInfoFlowC(Id, IpcInfo);
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_D)
    {
        IPL_CtrlInitIpcInfoFlowD(Id, IpcInfo);
    }
    else
    {
        DBG_ERR("unsupport IPL flow %d\r\n",IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW));
    }

    IsrInfo->EventTab[Id].FP[IPL_ISR_SIE] = IPL_IsrSIECB;
    IsrInfo->EventTab[Id].FP[IPL_ISR_IFE] = NULL;
    IsrInfo->EventTab[Id].FP[IPL_ISR_IPE] = IPL_IsrIPECB;
    IsrInfo->EventTab[Id].FP[IPL_ISR_IME] = IPL_IsrIMECB;
    IsrInfo->EventTab[Id].FP[IPL_ISR_PRE] = NULL;
    IsrInfo->EventTab[Id].FP[IPL_ISR_IFE2] = NULL;
    IsrInfo->EventTab[Id].FP[IPL_ISR_DCE] = NULL;
    IsrInfo->EventTab[Id].FP[IPL_ISR_RDE] = NULL;
    for (i = 0; i < IPL_ISR_MAX_CNT; i ++)
    {
        IsrInfo->CB_FP[Id].FP[i] = ChgMode->ISR_CB_FP[i];
    }
}

void IPL_CtrlInitPrvIpcInfo_CCIR(IPL_PROC_ID Id, IPC_INFO *IpcInfo, IPL_ISR_OBJ_INFO *IsrInfo, IPL_CHGMODE_DATA *ChgMode)
{
    UINT32 i;

    if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_D)
    {
        IPL_CtrlInitIpcInfoFlowD_CCIR(Id, IpcInfo);
    }
    else
    {
        DBG_ERR("unsupport IPL flow %d\r\n",IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW));
    }

    IsrInfo->EventTab[Id].FP[IPL_ISR_SIE] = NULL;
    IsrInfo->EventTab[Id].FP[IPL_ISR_IFE] = NULL;
    IsrInfo->EventTab[Id].FP[IPL_ISR_IPE] = NULL;
    IsrInfo->EventTab[Id].FP[IPL_ISR_IME] = NULL;
    IsrInfo->EventTab[Id].FP[IPL_ISR_PRE] = NULL;
    IsrInfo->EventTab[Id].FP[IPL_ISR_IFE2] = NULL;
    IsrInfo->EventTab[Id].FP[IPL_ISR_DCE] = NULL;
    IsrInfo->EventTab[Id].FP[IPL_ISR_RDE] = NULL;
    for (i = 0; i < IPL_ISR_MAX_CNT; i ++)
    {
        IsrInfo->CB_FP[Id].FP[i] = NULL;
    }
}

#if 0
#endif
UINT32 IPL_CtrlGetSIECABufInfo(IPLCTRL_CA_TYPE type, UINT32 WinXNum, UINT32 WinYNum)
{
    UINT32 Ratio = 0;
    if (type == IPLCTRL_CA_TYPE_32BIT)
    {
        Ratio = 4;
    }
    else if (type == IPLCTRL_CA_TYPE_64BIT)
    {
        Ratio = 8;
    }
    else
    {
        DBG_ERR("unsupport CA type %d\r\n", type);
    }
    return (WinXNum * WinYNum * Ratio);
}

UINT32 IPL_CtrlGetSIEVABufInfo(void)
{
    return (16 * 16 * 2 * 4);

}

UINT32 IPL_CtrlGetIPEVABufInfo(void)
{
    return (16 * 16 * 3 * 4);
}

void IPL_GetImePathORGInfo(UINT32 Id, IPL_IME_PATH PathId, IPL_IME_PATH_INFO *Info)
{
    if (Info == NULL)
    {
        DBG_ERR("input fail\r\n");
        return;
    }

    if (PathId == IPL_IME_PATH1)
    {
        Info->OutputEn = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_EN);
        Info->ImgSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_H);
        Info->ImgSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_V);
        Info->ImgSizeLOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_HLOS);
        Info->ImgFmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_FMT);
    }
    else if (PathId == IPL_IME_PATH2)
    {
        Info->OutputEn = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_EN);
        Info->ImgSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_H);
        Info->ImgSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_V);
        Info->ImgSizeLOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_HLOS);
        Info->ImgFmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_FMT);
    }
    else if (PathId == IPL_IME_PATH3)
    {
        Info->OutputEn = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_EN);
        Info->ImgSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_H);
        Info->ImgSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_V);
        Info->ImgSizeLOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_HLOS);
        Info->ImgFmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_FMT);
    }
    else
    {
        DBG_ERR("path id fail %d\r\n", PathId);
    }
}

void IPL_GetImePathInfo(UINT32 Id, IPL_IME_PATH PathId, IPL_IME_PATH_INFO *Info)
{
    if (Info == NULL)
    {
        DBG_ERR("input fail\r\n");
        return;
    }

    if (PathId == IPL_IME_PATH1)
    {
        Info->OutputEn = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_EN);
        Info->ImgSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_H);
        Info->ImgSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_V);
        Info->ImgSizeLOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_HLOS);
        Info->ImgFmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_FMT);
    }
    else if (PathId == IPL_IME_PATH2)
    {
        Info->OutputEn = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_EN);
        Info->ImgSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_H);
        Info->ImgSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_V);
        Info->ImgSizeLOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_HLOS);
        Info->ImgFmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_FMT);
    }
    else if (PathId == IPL_IME_PATH3)
    {
        Info->OutputEn = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_EN);
        Info->ImgSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_H);
        Info->ImgSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_V);
        Info->ImgSizeLOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_HLOS);
        Info->ImgFmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_FMT);
    }
    else
    {
        DBG_ERR("path id fail %d\r\n", PathId);
    }
}

BOOL IPL_ChkImePathInfo(UINT32 Id, IPL_IME_PATH PathId, IPL_IME_PATH_INFO *Info)
{
    IPL_YCC_IMG_INFO orgInfo = {0}, newInfo = {0};
    UINT32 orgYSize, orgUVSize, newYSize, newUVSize;

    if (Info == NULL)
    {
        DBG_ERR("input fail\r\n");
        return FALSE;
    }

    if (PathId == IPL_IME_PATH1)
    {
        if (Info->OutputEn == DISABLE)
        {
            return TRUE;
        }
        //buf
        orgInfo.type = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_FMT);
        orgInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_H);
        orgInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_H);
        orgInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_V);
        orgInfo.Ch[IPL_IMG_YUV_UV] = IPL_UTI_Y_INFO_CONV2(orgInfo.type, orgInfo.Ch[IPL_IMG_YUV_Y]);

        //new
        newInfo.type = Info->ImgFmt;
        newInfo.Ch[IPL_IMG_YUV_Y].Width = Info->ImgSizeH;
        newInfo.Ch[IPL_IMG_YUV_Y].LineOfs = Info->ImgSizeLOfs;
        newInfo.Ch[IPL_IMG_YUV_Y].Height = Info->ImgSizeV;
        newInfo.Ch[IPL_IMG_YUV_UV] = IPL_UTI_Y_INFO_CONV2(newInfo.type, newInfo.Ch[IPL_IMG_YUV_Y]);
    }
    else if (PathId == IPL_IME_PATH2)
    {
        if (Info->OutputEn == DISABLE)
        {
            return TRUE;
        }
        //buf
        orgInfo.type = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_FMT);
        orgInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_H);
        orgInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_H);
        orgInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_V);
        orgInfo.Ch[IPL_IMG_YUV_UV] = IPL_UTI_Y_INFO_CONV2(orgInfo.type, orgInfo.Ch[IPL_IMG_YUV_Y]);

        //new
        newInfo.type = Info->ImgFmt;
        newInfo.Ch[IPL_IMG_YUV_Y].Width = Info->ImgSizeH;
        newInfo.Ch[IPL_IMG_YUV_Y].LineOfs = Info->ImgSizeLOfs;
        newInfo.Ch[IPL_IMG_YUV_Y].Height = Info->ImgSizeV;
        newInfo.Ch[IPL_IMG_YUV_UV] = IPL_UTI_Y_INFO_CONV2(newInfo.type, newInfo.Ch[IPL_IMG_YUV_Y]);
    }
    else if (PathId == IPL_IME_PATH3)
    {
        if (Info->OutputEn == DISABLE)
        {
            return TRUE;
        }
        //org
        orgInfo.type = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_FMT);
        orgInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_H);
        orgInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_H);
        orgInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_V);
        orgInfo.Ch[IPL_IMG_YUV_UV] = IPL_UTI_Y_INFO_CONV2(orgInfo.type, orgInfo.Ch[IPL_IMG_YUV_Y]);

        //new
        newInfo.type = Info->ImgFmt;
        newInfo.Ch[IPL_IMG_YUV_Y].Width = Info->ImgSizeH;
        newInfo.Ch[IPL_IMG_YUV_Y].LineOfs = Info->ImgSizeLOfs;
        newInfo.Ch[IPL_IMG_YUV_Y].Height = Info->ImgSizeV;
        newInfo.Ch[IPL_IMG_YUV_UV] = IPL_UTI_Y_INFO_CONV2(newInfo.type, newInfo.Ch[IPL_IMG_YUV_Y]);
    }
    else
    {
        DBG_ERR("path id fail %d\r\n", PathId);
        return FALSE;
    }

    orgYSize = (orgInfo.Ch[IPL_IMG_YUV_Y].LineOfs * orgInfo.Ch[IPL_IMG_YUV_Y].Height);
    orgUVSize = (orgInfo.Ch[IPL_IMG_YUV_UV].LineOfs * orgInfo.Ch[IPL_IMG_YUV_UV].Height);

    newYSize = (newInfo.Ch[IPL_IMG_YUV_Y].LineOfs * newInfo.Ch[IPL_IMG_YUV_Y].Height);
    newUVSize = (newInfo.Ch[IPL_IMG_YUV_UV].LineOfs * newInfo.Ch[IPL_IMG_YUV_UV].Height);


    if ((orgYSize >= newYSize) && (orgUVSize >= newUVSize))
    {
        return TRUE;
    }
    DBG_ERR("buffer overflow Y(org 0x%.8x > new 0x%.8x)\r\n", orgYSize, newYSize);
    DBG_ERR("buffer overflow UV(org 0x%.8x > new 0x%.8x)\r\n", orgUVSize, newUVSize);
    return FALSE;
}

void IPL_SetImePathInfo(UINT32 Id, IPL_IME_PATH PathId, IPL_IME_PATH_INFO *Info)
{
    IPL_YCC_CH_INFO SizeInfo;
    UINT32 Width, Height, LineOfs;

    if (Info == NULL)
    {
        DBG_ERR("input fail\r\n");
        return;
    }
    if (PathId == IPL_IME_PATH1)
    {
        if (Info->OutputEn == ENABLE)
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_H, Info->ImgSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_V, Info->ImgSizeV);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_HLOS, Info->ImgSizeLOfs);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_FMT, Info->ImgFmt);

            Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_H);
            LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_HLOS);
            Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_V);

            if (IPL_CtrlGetInfor(Id, IPLCTRL_DIS_EN) == ENABLE)
            {
                Width = IPL_UTI_DIS_ADJ_HSIZE(Width, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16);
                LineOfs = IPL_UTI_DIS_ADJ_HSIZE(LineOfs, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16);
                Height = IPL_UTI_DIS_ADJ_VSIZE(Height, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 4);
            }
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_H, Width);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_HLOS, LineOfs);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_V, Height);


            Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_H);
            LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_HLOS);
            Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_ORG_SIZE_V);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_DISP_SIZE_H, Width);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_DISP_SIZE_HLOS, LineOfs);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_DISP_SIZE_V, Height);

            SizeInfo.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_DISP_SIZE_H);
            SizeInfo.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_DISP_SIZE_V);
            IPL_SetDeviceInputInfo(0, SizeInfo);
        }
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_EN, Info->OutputEn);
    }
    else if (PathId == IPL_IME_PATH2)
    {
        if (Info->OutputEn == ENABLE)
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_H, Info->ImgSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_V, Info->ImgSizeV);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_HLOS, Info->ImgSizeLOfs);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_FMT, Info->ImgFmt);

            Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_H);
            LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_HLOS);
            Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_V);

            if (IPL_CtrlGetInfor(Id, IPLCTRL_DIS_EN) == ENABLE)
            {
                Width = IPL_UTI_DIS_ADJ_HSIZE(Width, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16);
                LineOfs = IPL_UTI_DIS_ADJ_HSIZE(LineOfs, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16);
                Height = IPL_UTI_DIS_ADJ_VSIZE(Height, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 4);
            }
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_H, Width);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_HLOS, LineOfs);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_V, Height);


            Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_H);
            LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_HLOS);
            Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_ORG_SIZE_V);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_DISP_SIZE_H, Width);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_DISP_SIZE_HLOS, LineOfs);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_DISP_SIZE_V, Height);

            SizeInfo.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_DISP_SIZE_H);
            SizeInfo.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_DISP_SIZE_V);
            IPL_SetDeviceInputInfo(1, SizeInfo);
        }
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_EN, Info->OutputEn);
    }
    else if (PathId == IPL_IME_PATH3)
    {
        if (Info->OutputEn == ENABLE)
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_H, Info->ImgSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_V, Info->ImgSizeV);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_HLOS, Info->ImgSizeLOfs);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_FMT, Info->ImgFmt);

            Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_H);
            LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_HLOS);
            Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_V);

            if (IPL_CtrlGetInfor(Id, IPLCTRL_DIS_EN) == ENABLE)
            {
                Width = IPL_UTI_DIS_ADJ_HSIZE(Width, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16);
                LineOfs = IPL_UTI_DIS_ADJ_HSIZE(LineOfs, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 16);
                Height = IPL_UTI_DIS_ADJ_VSIZE(Height, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 4);
            }
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_H, Width);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_HLOS, LineOfs);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_V, Height);


            Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_H);
            LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_HLOS);
            Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_ORG_SIZE_V);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_DISP_SIZE_H, Width);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_DISP_SIZE_HLOS, LineOfs);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_DISP_SIZE_V, Height);

            SizeInfo.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_DISP_SIZE_H);
            SizeInfo.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_DISP_SIZE_V);
            IPL_SetDeviceInputInfo(2, SizeInfo);
        }
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_EN, Info->OutputEn);
    }
    else
    {
        DBG_ERR("path id fail %d\r\n", PathId);
    }
}

void IPL_CtrlPrvSetDZoomInfo(UINT32 Id)
{
    SENSOR_INFO Info;
    UINT32 Width, Height;

    //initial sensor parameter
    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo2(Id, IPL_SEL_PRVSENMODE), &Info);

    //SIE crop
    IPL_DzoomGetSIECropSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);

    Width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio), Width, 4);
    Height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio), Height, 4);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V, Height);

    //SIE ch0
    IPL_DzoomGetSIEOutSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);

    Width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio), Width, 4);
    Height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio), Height, 4);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT, IPL_UTI_CONV_ALGINFO_RAW_BIT(IPL_AlgGetUIInfo2(Id, IPL_SEL_RAW_BITDEPTH_PRV)));
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width, IPL_UTI_CONV_ALGINFO_RAW_BIT(IPL_AlgGetUIInfo2(Id, IPL_SEL_RAW_BITDEPTH_PRV))));
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V, Height);

    //SIE ch2
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_RAW_SIZE_H, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_PRVBIT, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_RAW_SIZE_HLOS, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_RAW_SIZE_V, 0);

    //PRE IN
    IPL_DzoomGetPREInSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);

    Width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio), Width, 4);
    Height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio), Height, 4);

    IPL_CtrlSetInfor(Id, IPLCTRL_PRE_IN_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_PRE_IN_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width, IPL_UTI_CONV_ALGINFO_RAW_BIT(IPL_AlgGetUIInfo2(Id, IPL_SEL_RAW_BITDEPTH_PRV))));
    IPL_CtrlSetInfor(Id, IPLCTRL_PRE_IN_SIZE_V, Height);
}

void IPL_CtrlPrvGetDZoomInfo(UINT32 Id, IPC_Dzoom_Info *Info)
{
    SENSOR_INFO SenInfo;

    //initial sensor parameter
    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo2(Id, IPL_SEL_PRVSENMODE), &SenInfo);

    Info->Idx = IPL_CtrlGetHalSetId(Id);
    Info->sieCropHini = (SenInfo.Mode->TransHD->DataSize - IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H)) >> 1;
    Info->sieCropVini = (SenInfo.Mode->TransVD->DataSize - IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V)) >> 1;
    Info->sieCropH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H);
    Info->sieCropV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V);
    Info->sieOutH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
    Info->sieOutV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V);
    Info->sieOutLineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS);
    Info->preInH = IPL_CtrlGetInfor(Id, IPLCTRL_PRE_IN_SIZE_H);
    Info->preInV = IPL_CtrlGetInfor(Id, IPLCTRL_PRE_IN_SIZE_V);
    Info->preInLoneOfs = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS);
    Info->bChgCAParam = (BOOL)IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_EN);
}

#if 0
#endif

void IPL_CtrlInitPrvExtBufInfo(UINT32 Id, IPL_CHGMODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *BufInfo)
{
    PPOST_HDR_INFO HdrInfo = {0};

    BufInfo->IEffectWDREn = DISABLE;
    if ((ChgMode->FuncEn & IPL_FUNC_IMGEFFECT_WDR) && (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_B))
    {
        BufInfo->IEffectWDREn = ENABLE;

        HdrInfo.InImg.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_H);
        HdrInfo.InImg.LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_H);
        HdrInfo.InImg.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_V);
        HdrInfo.InImgtype = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_FMT);
        HdrInfo.refImg.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_V_SIZE_H);
        HdrInfo.refImg.LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_V_SIZE_HLOS);
        HdrInfo.refImg.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_V_SIZE_V);
        BufInfo->IEffectWDRBufSize = IPL_PPostHDR_GetMaxWorkBufSize(&HdrInfo);
    }

    BufInfo->StampEn = DISABLE;
    if (ChgMode->FuncEn & IPL_FUNC_STAMP)
    {
        BufInfo->StampEn = ENABLE;
        BufInfo->StampBufSize = DataStamp_GetMaxWorkBufSize();
    }

    BufInfo->AlbumEn = DISABLE;
    if (ChgMode->FuncEn & IPL_FUNC_ALBUM)
    {
        BufInfo->AlbumEn = ENABLE;
    }
    if (ChgMode->FuncEn & IPL_FUNC_PHOTOFRAME)
    {
        BufInfo->PhotoFrameEn = ENABLE;
        BufInfo->PhotoFrameBufSize = (320*240*2);
    }
}

void IPL_CtrlInitCapBufInfo(UINT32 Id, IPL_CHGMODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *BufInfo)
{
}

void IPL_CtrlInitPrvBufInfo(UINT32 Id, IPL_CHGMODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *BufInfo)
{
    if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_A)
    {
        IPL_CtrlInitBufInfoFlowA(Id, BufInfo);
        BufInfo->Id = Id;
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_B)
    {
        IPL_CtrlInitBufInfoFlowB(Id, BufInfo);
        BufInfo->Id = Id;
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_C)
    {
        IPL_CtrlInitBufInfoFlowC(Id, BufInfo);
        BufInfo->Id = Id;
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_D)
    {
        IPL_CtrlInitBufInfoFlowD(Id, BufInfo);
        BufInfo->Id = Id;
    }
    else
    {
        DBG_ERR("unsupport IPL flow %d\r\n",IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW));
    }
}

void IPL_CtrlInitPrvBufInfo_CCIR(UINT32 Id, IPL_CHGMODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *BufInfo)
{
    if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_D)
    {
        IPL_CtrlInitBufInfoFlowD_CCIR(Id, BufInfo);
        BufInfo->Id = Id;
    }
    else
    {
        DBG_ERR("unsupport IPL flow %d\r\n",IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW));
    }
}
#if 0
#endif
void IPL_CtrlCapInit(UINT32 Id, UINT32 EventId, IPLCTRL_INFO *CtrlInfo)
{
    UINT32 Width, Height, BitNum;

    IPL_CtrlSetInfor(Id, IPLCTRL_CAPBURSTLENGTH, _BURST_64WORD);
    if (EventId == IPL_FLOW_NOR_GETRAW)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_NORMAL);
    }
    else if (EventId == IPL_FLOW_HDR_GETRAW)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_HDR);
    }
    else if (EventId == IPL_FLOW_VID_GETRAW)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPBURSTLENGTH, _BURST_128WORD);
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_INVID);
    }
    else if (EventId == IPL_FLOW_HS_GETRAW)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_HS);
    }
    else if (EventId == IPL_FLOW_3D_GETRAW)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_3D);
    }
    else if (EventId == IPL_FLOW_EB_GETRAW)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_EBURST);
    }
    else if (EventId == IPL_FLOW_AEB_GETRAW)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_AEB);
    }
    else if (EventId == IPL_FLOW_AWB_GETRAW)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_AWB);
    }
    else if (EventId == IPL_FLOW_MFHDR_GETRAW)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_MFHDR);
    }
    else if (EventId == IPL_FLOW_MFANTISHAKE_GETRAW)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_MFANTISHAKE);
    }
    else if (EventId == IPL_FLOW_DUALSENSOR_GETRAW)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_DUALSENSOR);
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_NONE);
        DBG_ERR("unsupport EventId flow %d\r\n",EventId);
    }

    IPL_CtrlSetInfor(Id, IPLCTRL_SEN_CAPCROPRATIO, CtrlInfo->ChgMode->CropRatio);

    IPL_CtrlSetInfor(Id, IPLCTRL_CAPAUTO_OB_EN, DISABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_CAPDF_EN, DISABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_CAP_SUBRATIO, 100);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CAPSTPIX, IPL_UTI_CONV_SEN_STPIX(CtrlInfo->Info->Mode->StPix));

    IPL_CtrlSetInfor(Id, IPLCTRL_CAP_PRI_V_EN, IQS_CAPPRI_IsRetinexEn());
    IPL_CtrlSetInfor(Id, IPLCTRL_CAP_PRI_V_SUBRATIO, IQS_CAPPRI_GetRetinexRefImgRatio());

    IPL_CtrlSetInfor(Id, IPLCTRL_CAP_PRI_CC_EN, IQS_CAPPRI_IsChromaAdaptEn());
    IPL_CtrlSetInfor(Id, IPLCTRL_CAP_PRI_CC_SUBRATIO, IQS_CAPPRI_GetLCARefImgRatio());

    IPL_CtrlSetInfor(Id, IPLCTRL_CAP_QV_V_EN, IQS_CAPQV_IsRetinexEn());
    IPL_CtrlSetInfor(Id, IPLCTRL_CAP_QV_V_SUBRATIO, IQS_CAPQV_GetRetinexRefImgRatio());

    IPL_CtrlSetInfor(Id, IPLCTRL_CAP_QV_CC_EN, IQS_CAPQV_IsChromaAdaptEn());
    IPL_CtrlSetInfor(Id, IPLCTRL_CAP_QV_CC_SUBRATIO, IQS_CAPQV_GetLCARefImgRatio());

    IPL_DzoomGetSIECropSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);

    Width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_CAPCROPRATIO), IPL_UTI_CONV_SEN_RATIO(CtrlInfo->Info->Mode->Ratio), Width, 4);
    Height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_CAPCROPRATIO), IPL_UTI_CONV_SEN_RATIO(CtrlInfo->Info->Mode->Ratio), Height, 4);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CAPCROP_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CAPCROP_SIZE_V, Height);

    if (EventId == IPL_FLOW_VID_GETRAW)
    {
        IPL_DzoomGetSIECropSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);
    }
    else
    {
        IPL_DzoomGetSIEOutSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);
    }

    Width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_CAPCROPRATIO), IPL_UTI_CONV_SEN_RATIO(CtrlInfo->Info->Mode->Ratio), Width, 4);
    Height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_CAPCROPRATIO), IPL_UTI_CONV_SEN_RATIO(CtrlInfo->Info->Mode->Ratio), Height, 4);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_BUFNUM, 1);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H, Width);

    if (EventId == IPL_FLOW_EB_GETRAW)
    {
        BitNum = IPL_RAW_BIT_12;
    }
    else
    {
        BitNum = IPL_UTI_CONV_ALGINFO_RAW_BIT(IPL_AlgGetUIInfo2(Id, IPL_SEL_RAW_BITDEPTH_CAP));
    }

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAPBIT, BitNum);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width, BitNum));
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V, Height);

    if (EventId == IPL_FLOW_VID_GETRAW)
    {
        IPL_DzoomGetSIECropSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);
    }
    else
    {
        IPL_DzoomGetSIEOutSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);
    }

    Width = IPL_UTI_SIESUB_ADJ_HSIZE(Width, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_CAP_SUBRATIO), 4);
    Height = IPL_UTI_SIESUB_ADJ_VSIZE(Height, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_CAP_SUBRATIO), 4);
    Width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_CAPCROPRATIO), IPL_UTI_CONV_SEN_RATIO(CtrlInfo->Info->Mode->Ratio), Width, 4);
    Height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_CAPCROPRATIO), IPL_UTI_CONV_SEN_RATIO(CtrlInfo->Info->Mode->Ratio), Height, 4);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_RAW_BUFNUM, 1);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_CAP_RAW_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_CAPBIT, BitNum);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_CAP_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width, BitNum));
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_CAP_RAW_SIZE_V, Height);

    if (EventId == IPL_FLOW_VID_GETRAW)
    {
        return;
    }
}

void IPL_CtrlCapInit_CCIR(UINT32 Id, UINT32 EventId, IPLCTRL_INFO *CtrlInfo)
{
    UINT32 Width, Height, BitNum;

    if (EventId == IPL_FLOW_CCIR_GETRAW)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_CCIR);
    }
    else if (EventId == IPL_FLOW_DUALSENSOR_GETRAW)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_DUALSENSOR);
    }
    else
    {
        DBG_ERR("unsupport flow\r\n");
    }
    IPL_DzoomGetSIEOutSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);


    BitNum = IPL_UTI_CONV_ALGINFO_RAW_BIT(IPL_AlgGetUIInfo2(Id, IPL_SEL_RAW_BITDEPTH_CAP));

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CCIR_CAP_RAW_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CCIR_CAPBIT, BitNum);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CCIR_CAP_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width, BitNum));
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CCIR_CAP_RAW_SIZE_V, Height);
}

void IPL_CtrlPrvInit(UINT32 Id, IPLCTRL_INFO *CtrlInfo)
{
    IPL_CtrlSetInfor(Id, IPLCTRL_PRVBURSTLENGTH, _BURST_64WORD);
    if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_A)
    {
        IPL_CtrlPrvInitFlowA(Id, CtrlInfo);
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_B)
    {
        IPL_CtrlPrvInitFlowB(Id, CtrlInfo);
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_C)
    {
        IPL_CtrlPrvInitFlowC(Id, CtrlInfo);
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_D)
    {
        IPL_CtrlPrvInitFlowD(Id, CtrlInfo);
    }
    else
    {
        DBG_ERR("unsupport IPL flow %d\r\n",IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW));
    }
}

void IPL_CtrlPrvInit_CCIR(UINT32 Id, IPLCTRL_INFO *CtrlInfo)
{
    if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_D)
    {
        IPL_CtrlPrvInitFlowD_CCIR(Id, CtrlInfo);
    }
    else
    {
        DBG_ERR("unsupport IPL flow %d\r\n",IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW));
    }
}

#if 0
#endif

IPL_HAL_PARASET IPL_CtrlGetHalSetId(UINT32 Id)
{

    IPL_MODE Mode;

    Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

    if ((IPLCTRL_CAP_FLOW_INVID == IPL_CtrlGetInfor(Id, IPLCTRL_CAPFLOW)) && (Mode == IPL_MODE_CAP))
    {
        if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_A)
        {
            return IPL_CtrlGetHalSetIdFlowA(Id);
        }
        else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_B)
        {
            return IPL_CtrlGetHalSetIdFlowB(Id);
        }
        else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_C)
        {
            return IPL_CtrlGetHalSetIdFlowC(Id);
        }
        else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_D)
        {
            return IPL_CtrlGetHalSetIdFlowD(Id);
        }
        else
        {
            DBG_ERR("unsupport IPL flow %d\r\n",IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW));
            return IPL_HAL_PAR4;
        }
    }
    else
    {
        if (Mode == IPL_MODE_CAP)
        {
            return IPL_HAL_PAR4;
        }
        else
        {
            if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_A)
            {
                return IPL_CtrlGetHalSetIdFlowA(Id);
            }
            else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_B)
            {
                return IPL_CtrlGetHalSetIdFlowB(Id);
            }
            else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_C)
            {
                return IPL_CtrlGetHalSetIdFlowC(Id);
            }
            else if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_D)
            {
                return IPL_CtrlGetHalSetIdFlowD(Id);
            }
            else
            {
                DBG_ERR("unsupport IPL flow %d\r\n",IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW));
                return IPL_HAL_PAR4;
            }
        }
    }
}

IPL_HAL_PARASET IPL_CtrlGetHalSetIdPProc(UINT32 Id)
{
    IPL_MODE Mode;

    Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

    if ((Mode != IPL_MODE_CAP) && (Mode != IPL_MODE_OFF) && (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_B))
    {
        return IPL_CtrlGetHalSetIdPProcFlowB();
    }
    else
    {
        DBG_ERR("unsupport IPL flow %d\r\n",IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW));
        return IPL_HAL_PAR4;
    }
}
