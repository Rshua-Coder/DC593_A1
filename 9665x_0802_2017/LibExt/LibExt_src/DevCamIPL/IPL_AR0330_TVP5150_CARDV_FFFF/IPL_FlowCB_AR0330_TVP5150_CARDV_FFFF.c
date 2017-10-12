/*
    IPL flow Callback Function

    This file is the API of the flow callback function.

    @file       IPL_FlowCBAR0330_TVP5150_CARDV_FFFF.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#include "IPL_AR0330_TVP5150_CARDV_FFFF_Int.h"

void IPL_FCB_GetCapRawNormal(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawSim(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawInVid(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawEBurst(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawCCIR(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawDualSensor(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
extern void IPL_FCB_FlashTrig(FLASH_INFOR *Data);
extern void IPL_FCB_MshutterClose(UINT32 uiEvent);
extern void IPL_FCB_MshutterOpen(void);

typedef void (*CapFP)(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);

static FLASH_INFOR MainFlashInfo = {0};

CapFP CaptureFP[IPLCTRL_CAP_FLOW_MAX] =
{
    NULL,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawEBurst,
    IPL_FCB_GetCapRawInVid,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawSim,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawCCIR,
    IPL_FCB_GetCapRawDualSensor,
};

void IPL_FCB_PrepareCapAE(IPL_PROC_ID Id)
{

}

void IPL_FCB_FlashTrig(FLASH_INFOR *Data)
{
    IPL_HAL_GROUP Update;
    IPC_IQparam IQ;
    SIE_SUBFUNC SubF;
    SIE_FLASH   FSH;

    Update.sieUpdate = (SIE_FLASH_PAR|SIE_FLASH_START);
    FSH.OutPolarity = Data->TriggerPolarity;
    FSH.InPhase = _RISING;
    FSH.TrigMode = Data->TriggerMode;
    FSH.DelayLine = Data->DelayLine;
    FSH.Delay2Pixel = Data->DelayPixel;
    FSH.Width = Data->FlashWidth;

    SubF.pFlash = &FSH;
    IQ.Idx = IPL_CtrlGetHalSetId(IPL_ID_1);
    IQ.pGrp = &Update;
    IQ.EngSubF.pSie_sub = &SubF;
    IPL_CTRLRunTimeChg_IQ(IPC_SIE_IQparam,(void*)&IQ);
}

void IPL_FCB_PreFlash(IPL_PROC_ID Id)
{
    IPL_CBMsgProc(IPL_CBMSG_PREFLASH, NULL);
    MainFlashInfo = flash_PreFlashFlow(IPL_FCB_FlashTrig);
    IPL_CBMsgProc(IPL_CBMSG_PREFLASHEND, NULL);
}

void IPL_FCB_GetCapRawNormal(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    EXPOSURE_SETTING ExpTSetting;
    GAIN_SETTING Gain;
    SENSOR_MODE Mode;
    UINT32 Ratio;
    UINT32 capiso,capext,iris;
    SENSOR_ID SenId;
    SenId = IPL_UTI_CONV2_SEN_ID(Id);

    //change sensor mode to capture
    IPC_WaitVD(TRUE,Id);
    Sensor_ChgMode(SenId, IPL_AlgGetUIInfo(IPL_SEL_CAPSENMODE));
    IPC_WaitVD(FALSE,Id);
    AE_GetCapAEArg(&capiso, &capext, &iris);
    if (MainFlashInfo.TriggerEnable == ENABLE)
    {
        capiso = MainFlashInfo.Flash_Cap_ISO;
        capext = MainFlashInfo.Flash_Cap_ExpoTime;
    }

    UINT32 PrvBin, CapBin;
    IQS_GetBinningFactor(&PrvBin, &CapBin);

    Sensor_GetCurMode(SenId, &Mode);
    Ratio = Sensor_GetExpoSetting(SenId, capext, Mode, &ExpTSetting);
    Sensor_GetGainSetting(SenId, (capiso * Ratio / 100), &Gain);
    Sensor_SetExpTime(SenId, &ExpTSetting);
    Sensor_SetGain(SenId, &Gain);

    IPC_WaitVD(FALSE,Id);
    IPL_CTRLRunTimeChg(IPC_SIE_, (void*)Info);
    if (MainFlashInfo.TriggerEnable == ENABLE)
    {
        IPL_FCB_FlashTrig(&MainFlashInfo);
    }

    IPC_WaitSieActS(TRUE,Id);

    IPL_CTRLStop(IPC_RESUME_SIE);
	//#NT#2017/07/17#liwei cheng -begin
    //#NT# solve getraw timg issue
    IPC_WaitSieActS(TRUE,Id);
	//#NT#2017/07/17#liwei cheng -end
    IPL_CTRLStop(IPC_PAUSE_SIE);
    IPC_WaitSieActS(TRUE,Id);
}

void IPL_FCB_GetCapRawCCIR(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    IPC_WaitVD(TRUE,Id);
    IPL_CTRLRunTimeChg(IPC_SIE2_, (void*)Info);
	IPC_WaitSieActS(TRUE,Id);
    IPL_CTRLStop(IPC_RESUME_SIE2);
	//#NT#2017/07/17#liwei cheng -begin
    //#NT# solve getraw timg issue
    IPC_WaitSieActS(TRUE,Id);
	//#NT#2017/07/17#liwei cheng -end
    IPL_CTRLStop(IPC_PAUSE_SIE2);
    IPC_WaitSieActS(TRUE,Id);
}

void IPL_FCB_GetCapRawDualSensor(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    if (Id == IPL_ID_1)
    {
        IPL_FCB_GetCapRawNormal(Id, Info, ExtInfo);
    }
    else if (Id == IPL_ID_2)
    {
        IPL_FCB_GetCapRawCCIR(Id, Info, ExtInfo);
    }
    else
    {
        DBG_ERR("not support \r\n");
    }
}


void IPL_FCB_GetCapRawInVid(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    IPL_HAL_GROUP Update;
    IPC_PREInAddr preinfo;
    Update.sieUpdate = Info->sieUpdate;

    IPC_WaitVD(TRUE,Id);
    IPH_SIE_ChgParam(Info->Idx, Update, Info->psie);
    IPH_ENG_Load(Id,IPL_HAL_SIE, Info->Idx, Update);

    IPC_WaitVD(FALSE,Id);
    IPL_CTRLStop(IPC_PAUSE_PRE);
    //check Ch0 need scale or not
    if ((Info->psie->DmaOutSizeH == Info->psie->IMG_Window.Crop.SizeH) &&
        (Info->psie->DmaOutSizeV == Info->psie->IMG_Window.Crop.SizeV))
    {
        Info->psie->Ch0.Src = NonScaled_Img;
    }
    else
    {
        Info->psie->Ch0.Src = CH0_Scaled_Img;
    }
    Info->psie->Ch0.Out.PPB0_Addr = ExtInfo->Param[1];
    Info->psie->Ch0.Out.PPB1_Addr = ExtInfo->Param[0];
    Info->psie->Ch0.Out.LineOffset = ExtInfo->Param[2];
    Info->psie->BurstLeng_Ch0 = ExtInfo->Param[3];
    Info->psie->IMG_Window.Crop.StartPix.x = ExtInfo->Param[4];
    Info->psie->IMG_Window.Crop.StartPix.y = ExtInfo->Param[5];
    Info->psie->IMG_Window.Crop.SizeH = ExtInfo->Param[6];
    Info->psie->IMG_Window.Crop.SizeV = ExtInfo->Param[7];
    IPH_SIE_ChgParam(Info->Idx, Update, Info->psie);
    IPH_ENG_Load(Id, IPL_HAL_SIE, Info->Idx, Update);

    IPC_WaitVD(FALSE,Id);

    preinfo.Idx = Info->Idx;
    preinfo.PPB0Addr = ExtInfo->Param[9];
    preinfo.PPB1Addr = ExtInfo->Param[8];
    IPL_CTRLRunTimeChg(IPC_Chg_PRE_INAddr,(void*)&preinfo);

    IPL_CTRLStop(IPC_RESUME_PRE);
}

void IPL_FCB_GetCapRawEBurst(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    EXPOSURE_SETTING ExpTSetting;
    GAIN_SETTING Gain;
    SENSOR_MODE Mode;
    UINT32 Ratio;
    UINT32 capiso,capext,iris;
    SENSOR_ID SenId;

    if (ExtInfo->Param[0] == 0)
    {
        //change sensor mode to capture
        IPC_WaitVD(TRUE,Id);
        SenId = IPL_UTI_CONV2_SEN_ID(Id);
        Sensor_ChgMode(SenId, IPL_AlgGetUIInfo(IPL_SEL_CAPSENMODE));
        AE_GetCapAEArg(&capiso, &capext, &iris);
        Sensor_GetCurMode(SenId, &Mode);
        Ratio = Sensor_GetExpoSetting(SenId, capext, Mode, &ExpTSetting);
        Sensor_GetGainSetting(SenId, (capiso * Ratio / 100), &Gain);
        Sensor_SetExpTime(SenId, &ExpTSetting);
        Sensor_SetGain(SenId, &Gain);
        IPL_CTRLRunTimeChg(IPC_SIE_, (void*)Info);
        IPC_WaitSieActS(TRUE,Id);
        IPL_CTRLStop(IPC_RESUME_SIE);
		//#NT#2017/07/17#liwei cheng -begin
	    //#NT# solve getraw timg issue
	    IPC_WaitSieActS(TRUE,Id);
		//#NT#2017/07/17#liwei cheng -end
    }
    else if (ExtInfo->Param[0] == 1)
    {
        IPL_CTRLStop(IPC_PAUSE_SIE);
        IPL_CTRLRunTimeChg(IPC_SIE_, (void*)Info);
        IPL_CTRLStop(IPC_RESUME_SIE);
		//#NT#2017/07/17#liwei cheng -begin
	    //#NT# solve getraw timg issue
	    IPC_WaitSieActS(TRUE,Id);
		//#NT#2017/07/17#liwei cheng -end
    }
    else if (ExtInfo->Param[0] == 2)
    {
        IPL_CTRLStop(IPC_PAUSE_SIE);
        IPC_WaitSieActS(TRUE,Id);
    }
}

void IPL_FCB_GetCapRaw(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    if (CaptureFP[IPL_CtrlGetInfor(Id, IPLCTRL_CAPFLOW)] != NULL)
    {
        if (IPLCTRL_CAP_FLOW_EBURST == IPL_CtrlGetInfor(Id, IPLCTRL_CAPFLOW))
        {
            if (ExtInfo->Param[0] != 2)
            {
                IPL_CBMsgProc(IPL_CBMSG_SHUTTER_SOUND, NULL);
            }
        }
        else
        {
            IPL_CBMsgProc(IPL_CBMSG_SHUTTER_SOUND, NULL);
        }
        CaptureFP[IPL_CtrlGetInfor(Id, IPLCTRL_CAPFLOW)](Id, Info, ExtInfo);
    }
    else
    {
        DBG_ERR("capture flow(%d) FP null\r\n", IPL_CtrlGetInfor(Id, IPLCTRL_CAPFLOW));
    }
}

void IPL_FCB_GetCapRawSim(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
}


#if AWB_CAPRAW_EMB_DEBUG_INFO
AWBALG_DEBUG_INFO awbDebugInfo;
#endif

void IPL_FCB_SetCapAWB(IPL_PROC_ID Id, IPC_SIE_Info *Info)
{
    //To Do: capture awb
    //..............
    DBG_ERR("Raw Size = %d %d %d\r\n", IPL_CtrlGetInfor((UINT32)Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H),
                            IPL_CtrlGetInfor((UINT32)Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V),
                            IPL_CtrlGetInfor((UINT32)Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS));

#if AWB_CAPRAW_EMB_DEBUG_INFO
    UINT32 Rg, Gg, Bg;
    AWB_GetColorGian(&Rg, &Gg, &Bg);
    awbDebugInfo.StartTag    = 0x12345678;
    awbDebugInfo.ImgWidth    = IPL_CtrlGetInfor((UINT32)Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H);
    awbDebugInfo.ImgHeight   = IPL_CtrlGetInfor((UINT32)Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V);
    awbDebugInfo.ImgBitDepth = IPL_CtrlGetInfor((UINT32)Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS);
    awbDebugInfo.CurRgain = Rg;
    awbDebugInfo.CurGgain = Gg;
    awbDebugInfo.CurBgain = Bg;
    awbDebugInfo.PrevRgain = Rg;
    awbDebugInfo.PrevGgain = Gg;
    awbDebugInfo.PrevBgain = Bg;
    awbDebugInfo.CapRgain = Rg;
    awbDebugInfo.CapGgain = Gg;
    awbDebugInfo.CapBgain = Bg;
    awbDebugInfo.EV = AE_GetCapEVValue();
    awbDebugInfo.LV = AE_GetCapEVValue();
    AWB_getCA(awbDebugInfo.CA_R, awbDebugInfo.CA_G, awbDebugInfo.CA_B, 1024);
    memcpy((void*)Info->psie->Ch0.Out.PPB0_Addr, &awbDebugInfo, sizeof(AWBALG_DEBUG_INFO));
#endif
}

void IPL_FCB_Alg3DNR(UINT32 SelIdx, UINT32 Value)
{
    IPL_HAL_GROUP ImeGroup = {0};
    IPL_HAL_READ readBit = {0};
    IME_SUBFUNC ImeSubFunc = {0};
    IPL_MODE Mode;
    IPC_IQparam IQinfo;
    Mode = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_CUR_MODE);

    if ((Mode != IPL_MODE_PREVIEW) &&
        (Mode != IPL_MODE_VIDEO) &&
        (Mode != IPL_MODE_VIDEOREC))
    {
        return;
    }
    readBit.imeRead = IME_R_SubFEn;
    IPH_ENG_Read(IPL_ID_1,IPL_HAL_IME, IPL_CtrlGetHalSetId(IPL_ID_1), readBit, (void*)&ImeSubFunc.imeFuncEn);

    if (Value == SEL_3DNR_ON)
    {
        ImeSubFunc.imeFuncEn |= IME3DNREn;
        DBG_ERR("^G3DNR on..\r\n");
    }
    else
    {
        ImeSubFunc.imeFuncEn &= ~IME3DNREn;
        DBG_ERR("^G3DNR off..\r\n");

    }

    ImeGroup.imeUpdate = IME_SubFEn;
    IQinfo.Idx = IPL_CtrlGetHalSetId(IPL_ID_1);
    IQinfo.pGrp = &ImeGroup;
    IQinfo.EngSubF.pIme_sub = &ImeSubFunc;
    IPL_CTRLRunTimeChg_IQ(IPC_IME_IQparam,(void*)&IQinfo);
}

void IPL_FCB_AlgGDC(UINT32 SelIdx, UINT32 Value)
{
    IPL_MODE Mode;

    Mode = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_CUR_MODE);

    if ((Mode != IPL_MODE_PREVIEW) &&
        (Mode != IPL_MODE_VIDEO) &&
        (Mode != IPL_MODE_VIDEOREC))
    {
        return;
    }

    if (Value == SEL_GDCCOMP_ON)
    {

    }
    else
    {
    }

    DBG_ERR("runtime set DCE interface not ready\r\n");
}

void IPL_FCB_AlgDIS(UINT32 SelIdx, UINT32 Value)
{
    UINT32 Buf, Size;
    IPL_IME_INFOR ImeInfo;
    IPL_SET_IMGRATIO_DATA RatioInfo;
    IPL_MODE Mode;

    Mode = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_CUR_MODE);

    if ((Mode != IPL_MODE_PREVIEW) &&
        (Mode != IPL_MODE_VIDEO) &&
        (Mode != IPL_MODE_VIDEOREC))
    {
        return;
    }

    if (Value == SEL_DISCOMP_OFF)
    {
        IQS_SettingFlowFunc(IQS_DISCOMP_OFF);
        IPL_CtrlSetInfor(IPL_ID_1, IPLCTRL_DIS_EN, DISABLE);
    }
    else if (Value == SEL_DISCOMP_ON)
    {
        IPL_BufGet(IPL_ID_1, IPLBUF_IPEETH_1, &Buf, &Size);
        if (Size != 0)
        {
            IQS_SettingFlowFunc(IQS_DISCOMP_ON);
            IPL_CtrlSetInfor(IPL_ID_1, IPLCTRL_DIS_EN, ENABLE);
        }
        else
        {
            return;
        }
    }

    ImeInfo.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_IME_INFOR, (void *)&ImeInfo);

    RatioInfo.Id = IPL_ID_1;
    RatioInfo.CropRatio = IPL_SENCROPRATIO_NONE;
    RatioInfo.PathId = IPL_IME_PATH_NONE;
    if (ImeInfo.P1Enable == ENABLE)
    {
        RatioInfo.PathId |= IPL_IME_PATH1;
        RatioInfo.ImeP1.OutputEn = ImeInfo.P1Enable;
        RatioInfo.ImeP1.ImgSizeH = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IME_P1_ORG_SIZE_H);
        RatioInfo.ImeP1.ImgSizeV = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IME_P1_ORG_SIZE_V);
        RatioInfo.ImeP1.ImgSizeLOfs = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IME_P1_ORG_SIZE_HLOS);
        RatioInfo.ImeP1.ImgFmt = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IME_P1_FMT);
    }

    if (ImeInfo.P2Enable == ENABLE)
    {
        RatioInfo.PathId |= IPL_IME_PATH2;
        RatioInfo.ImeP2.OutputEn = ImeInfo.P2Enable;
        RatioInfo.ImeP2.ImgSizeH = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IME_P2_ORG_SIZE_H);
        RatioInfo.ImeP2.ImgSizeV = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IME_P2_ORG_SIZE_V);
        RatioInfo.ImeP2.ImgSizeLOfs = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IME_P2_ORG_SIZE_HLOS);
        RatioInfo.ImeP2.ImgFmt = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IME_P2_FMT);
    }

    if (ImeInfo.P3Enable == ENABLE)
    {
        RatioInfo.PathId |= IPL_IME_PATH3;
        RatioInfo.ImeP3.OutputEn = ImeInfo.P3Enable;
        RatioInfo.ImeP3.ImgSizeH = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IME_P3_ORG_SIZE_H);
        RatioInfo.ImeP3.ImgSizeV = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IME_P3_ORG_SIZE_V);
        RatioInfo.ImeP3.ImgSizeLOfs = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IME_P3_ORG_SIZE_HLOS);
        RatioInfo.ImeP3.ImgFmt = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IME_P3_FMT);
    }
    IPL_SetCmd(IPL_SET_IMGRATIO, (void *)&RatioInfo);
    IPL_WaitCmdFinish();
}

void IPL_FCB_AlgIE(UINT32 SelIdx, UINT32 Value)
{
    IQS_SettingFlowFunc(IQS_IMAGE_EFFECT);
    if ( IPL_AlgGetUIInfo(IPL_SEL_IMAGEEFFECT) == SEL_IMAGEEFFECT_WDR )
        DBG_ERR("^GWDR ON..\r\n");
    else//if ( IPL_AlgGetUIInfo(IPL_SEL_IMAGEEFFECT) == SEL_IMAGEEFFECT_OFF )
        DBG_ERR("^GWDR OFF..\r\n");
}

void IPL_FCB_AlgRSC(UINT32 SelIdx, UINT32 Value)
{
    if (Value == SEL_RSC_ON)
    {
        //rsc on
    }
    else if (Value == SEL_RSC_OFF)
    {
        //rsc off
    }
}

void IPL_FCB_AlgWDR(UINT32 SelIdx, UINT32 Value)
{
    if ( Value >= SEL_WDR_MAX_CNT )
        DBG_ERR("IPL_SEL_WDR is out of range\r\n");
    DBG_ERR("^GIPL_FCB_AlgWDR = %d..\r\n", Value);
}

void IPL_FCB_MshutterClose(UINT32 uiEvent)
{

}

void IPL_FCB_MshutterOpen(void)
{

}

