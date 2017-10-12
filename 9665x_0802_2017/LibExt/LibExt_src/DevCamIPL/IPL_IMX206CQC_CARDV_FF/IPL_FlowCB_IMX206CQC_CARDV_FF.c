/*
    IPL flow Callback Function

    This file is the API of the flow callback function.

    @file       IPL_FlowCBIMX206CQC_CARDV_FF.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#define MS_CTRLBY_TIMER 1
#include "IPL_IMX206CQC_CARDV_FF_Int.h"
#include "delay.h"
#include "Lens.h"
#include "AE_Alg.h"
#include "LensCtrlTsk.h"
#include "sie_lib.h"
#if MS_CTRLBY_TIMER
#include "Timer.h"
#endif

void IPL_FCB_GetCapRawNormal(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawSim(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawInVid(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawEBurst(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);


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
    IPL_FCB_GetCapRawNormal
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
    //MainFlashInfo = flash_PreFlashFlow(IPL_FCB_FlashTrig);
    MainFlashInfo.TriggerEnable = DISABLE;
    IPL_CBMsgProc(IPL_CBMSG_PREFLASHEND, NULL);
}

void IPL_FCB_MshutterClose(UINT32 uiEvent)
{

}

void IPL_FCB_MshutterOpen(void)
{

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
    awbDebugInfo.CABits = AWB_ACC_BIT;
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
    else
        DBG_ERR("^GWDR OFF..\r\n");
}



BOOL RscFirstRunFlag = TRUE;

UINT32 RSC_STATUS;
BOOL RSCSTATUS_GET()
{
    return RSC_STATUS;
}
BOOL RSCSTATUS_SET(BOOL value)
{
    RSC_STATUS =  value;
    return TRUE;
}

static void IPL_FCB_RSCRunTimeChg(RSCCTRL_INFO *Info)
{
    UINT32 ISO, Iris;
    SENSOR_INFO SenInfor;

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(IPL_ID_1), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &SenInfor);

    Info->pSenInfo->VdT = (10000000 / SenInfor.Mode->FrameRate);
    Info->pSenInfo->StrLnT = SenInfor.Mode->StrLnT;
    Info->pSenInfo->EndLnT = SenInfor.Mode->EndLnT;
    Info->pSenInfo->TransDelyT = SenInfor.Mode->TransDelyT;
    Info->pSenInfo->OutSize.w = SenInfor.Mode->SenHD->DataSize;
    Info->pSenInfo->OutSize.h = SenInfor.Mode->SenVD->DataSize;

    *Info->pFL = Lens_Focus_GetLength_um(Lens_Zoom_GetSection(ZOOM_CUR_SECTION));

    //C H Lin - RSC_Modification: Add Focal Lenght Check
    if(*Info->pFL == 0) DBG_ERR("Focal Lenght Error!!\r\n");

    AE_GetPrvAEArg(&ISO, Info->pExpT, &Iris);

    if (RscFirstRunFlag == FALSE)
    {
        gyro_getFreeRunData(&Info->pGDTbl->DataCnt, Info->pGDTbl->pGyroX, Info->pGDTbl->pGyroY);
    }
    else
    {
        RscFirstRunFlag = FALSE;
    }

    Info->pDceInfo = NULL;
    Info->pIPLIn = NULL;
    Info->pIPLOut = NULL;
}

void IPL_FCB_AlgRSC(UINT32 SelIdx, UINT32 Value)
{
    IME_RSCorrect Rsc = {0};
    IME_SUBFUNC   ImeSubFunc = {0};
    IPC_IQparam ImeIQParam;
    IPL_HAL_GROUP GrpInfo;
    GYRO_OPENOBJ openObj;
    GYRO_FREE_RUN_PARAM frParam;
    static RSCCTRL_Q QInfo = {10, 50};
    RSCCTRL_INIT InitInfo = {0};
    SENSOR_INFO SenInfo;
    IPL_MODE Mode;
    IPL_DZOOM_INFOR CurInfo;
    IPL_DZOOM Info;
    GYRO_RSCINFOINIT gyrorscInit;
    //C H Lin - RSC_Modification: FOV flow
    static UINT32 chk_GyroStatus = 0;

    Mode = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_CUR_MODE);

    /*
    if ((Mode != IPL_MODE_PREVIEW) &&
        (Mode != IPL_MODE_VIDEO) &&
        (Mode != IPL_MODE_VIDEOREC))
    {

        DBG_ERR("Mode error!!\r\n");
        return;
    }
    */


    if (Value == SEL_RSC_ON)
    {

        if(RSCSTATUS_GET()==TRUE)
        {
            return;
        }


        Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(IPL_ID_1), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &SenInfo);

        InitInfo.Idx = IPL_CtrlGetHalSetId(IPL_ID_1);
        InitInfo.pRSC_Q = &QInfo;

        if (IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_RSC_EN) == ENABLE)
        {
            InitInfo.pRSC_Q->CompRatio = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_RSC_RATIO);
        }
        InitInfo.bEIS_En = DISABLE;
        InitInfo.GD_Num = 8;
        InitInfo.SenSize.w = SenInfo.CellWidth;
        InitInfo.SenSize.h = SenInfo.CellHeight;
        InitInfo.GyroRef.GDCalGain = 1024;
        InitInfo.GyroRef.GDCalOfs = 0;
        //C H Lin - RSC_Modification: Add gyro_RSCInfoInit
        //InitInfo.GyroRef.GDFmt = _ISIDG2020;
        //C H Lin - RSC_Modification: Add Gyro Direction
        //InitInfo.GyroRef.GDDir = _RSC_GYRO_DIR_UP_RIGHT;
        //InitInfo.GyroRef.GDunit = (65536/131);
        gyro_RSCInfoInit(&gyrorscInit);
        InitInfo.GyroRef.GDFmt = (RSCCTRL_GD_FMT)gyrorscInit.GDFmt;
        InitInfo.GyroRef.GDDir = (RSCCTRL_GYRO_DIRECTION)gyrorscInit.GDDir;
        InitInfo.GyroRef.GDunit = gyrorscInit.GDunit;
        InitInfo.fpRunTimeChg = IPL_FCB_RSCRunTimeChg;
        ICF_RSCCTRL_OPEN(&InitInfo);

        //C H Lin - RSC_Modification: FOV flow
        if(chk_GyroStatus != 1)
        {
            chk_GyroStatus = 1; //Gyro Open
            openObj.FP_GYRO_CB = ICF_RSCCTRL_TRIG;
            gyro_open(&openObj);
            frParam.uiPeriodUs = 10000000/SenInfo.Mode->FrameRate;
            frParam.uiDataNum = InitInfo.GD_Num;
            gyro_setFreeRunParam(&frParam);
            gyro_setMode(GYRO_OP_MODE_FREE_RUN);
        }

        RSCSTATUS_SET(TRUE);

    }
/*
    else if (Value == SEL_RSC_PAUSE)
    {

        if(RSCSTATUS_GET()==FALSE)
        {
            return;
        }
        //C H Lin - RSC_Modification: FOV flow
        chk_GyroStatus = 0;

        //rsc off
        ICF_RSCCTRL_CLOSE();
        // enter free-run form single-access
        gyro_setMode(GYRO_OP_MODE_SINGLE_ACCESS);
        // close gyro
        gyro_close();

        ImeIQParam.Idx = IPL_CtrlGetHalSetId(IPL_ID_1);
        //Rsc.MarginH = 0;
        //Rsc.MarginV = 0;
        ImeSubFunc.pRsc = &Rsc;
        ImeIQParam.EngSubF.pIme_sub = &ImeSubFunc;
        GrpInfo.imeUpdate = IME_RSC_;
        ImeIQParam.pGrp = &GrpInfo;
        IPL_CTRLRunTimeChg_IQ(IPC_IME_IQparam, (void *)&ImeIQParam);


        //wait IME frame(first frame ready)
        IPC_WaitImeFMD(TRUE);

        RscFirstRunFlag = TRUE;

        RSCSTATUS_SET(FALSE);
    }
*/
    else if (Value == SEL_RSC_OFF)
    {


        if(RSCSTATUS_GET()==TRUE)
        {
        //C H Lin - RSC_Modification: FOV flow
        chk_GyroStatus = 0;

        //rsc off
        ICF_RSCCTRL_CLOSE();
        // enter free-run form single-access
        gyro_setMode(GYRO_OP_MODE_SINGLE_ACCESS);
        // close gyro
        gyro_close();

        ImeIQParam.Idx = IPL_CtrlGetHalSetId(IPL_ID_1);
        Rsc.MarginH = 0;
        Rsc.MarginV = 0;
        ImeSubFunc.pRsc = &Rsc;
        ImeIQParam.EngSubF.pIme_sub = &ImeSubFunc;
        GrpInfo.imeUpdate = IME_RSC_;
        ImeIQParam.pGrp = &GrpInfo;
        IPL_CTRLRunTimeChg_IQ(IPC_IME_IQparam, (void *)&ImeIQParam);
        }
        CurInfo.Id = IPL_ID_1;
        IPL_GetCmd(IPL_GET_CUR_DZ_INFOR, (void *)&CurInfo);
        Info.Id = CurInfo.Id;
        Info.Idx = CurInfo.CurIdx;
        IPL_SetCmd(IPL_SET_DZOOM, (void *)&Info);
        IPL_WaitCmdFinish();
        //wait IME frame(first frame ready)
        IPC_WaitImeFMD(TRUE);

        RscFirstRunFlag = TRUE;
        RSCSTATUS_SET(FALSE);

    }
}

void IPL_FCB_AlgWDR(UINT32 SelIdx, UINT32 Value)
{
    if ( Value >= SEL_WDR_MAX_CNT )
        DBG_ERR("IPL_SEL_WDR is out of range\r\n");
    DBG_ERR("^GIPL_FCB_AlgWDR = %d..\r\n", Value);
}


