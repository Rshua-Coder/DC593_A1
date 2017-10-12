/*
    IPL command API

    This file is the API of the IPL control.

    @file       IPL_Cmd_AR0330_TVP5150_CARDV_FFFF.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include "IPL_AR0330_TVP5150_CARDV_FFFF_Int.h"
//note: do not delete
//avoid compiler optimize "weak function"
WEAK_DUMMY_CALL(CMD, CTRL1)

IPL_CMD_CHGMODE_FP IPL_ChgModeFCB(IPL_MODE CurMode, IPL_MODE NextMode)
{
    return IPL_GetModeFp(CurMode, NextMode);
}

IPL_IME_PATH IPL_SetImeFCB(IPL_IME_DATA *Info, IPC_IMEOut_Info *IpcInfoP1, IPC_IMEOut_Info *IpcInfoP2, IPC_IMEOut_Info *IpcInfoP3)
{
    IPL_IME_PATH rt = IPL_IME_PATH_NONE;
    UINT32 BufAddr, Size;
    IPL_IME_PATH_INFO tmpImeInfo;
    IPL_YCC_CH_INFO tmpInfo;

    IpcInfoP1->Idx = IPL_CtrlGetHalSetId(IPL_ID_1);
    IpcInfoP2->Idx = IPL_CtrlGetHalSetId(IPL_ID_1);
    IpcInfoP3->Idx = IPL_CtrlGetHalSetId(IPL_ID_1);

    IPL_BufGet(Info->Id, IPLBUF_IMEP1_1, &BufAddr, &Size);
    if ((Info->PathId & IPL_IME_PATH1) && (Size != 0))
    {
        //p1
        if (IPL_ChkImePathInfo(Info->Id, IPL_IME_PATH1, &Info->ImeP1) == TRUE)
        {
            IPL_SetImePathInfo(Info->Id, IPL_IME_PATH1, &Info->ImeP1);
            IPL_GetImePathInfo(Info->Id, IPL_IME_PATH1, &tmpImeInfo);
            IpcInfoP1->Path.PathEn = tmpImeInfo.OutputEn;
            IpcInfoP1->Path.Size.Y.Width = tmpImeInfo.ImgSizeH;
            IpcInfoP1->Path.Size.Y.Height = tmpImeInfo.ImgSizeV;
            IpcInfoP1->Path.Size.Y.LineOfs = tmpImeInfo.ImgSizeLOfs;
            IpcInfoP1->Path.Fmt = tmpImeInfo.ImgFmt;
            IpcInfoP1->Path.Crop.Start.x = 0;
            IpcInfoP1->Path.Crop.Start.y = 0;
            IpcInfoP1->Path.Crop.SizeH = IpcInfoP1->Path.Size.Y.Width;
            IpcInfoP1->Path.Crop.SizeV = IpcInfoP1->Path.Size.Y.Height;
            tmpInfo = IPL_UTI_Y_INFO_CONV2(IpcInfoP1->Path.Fmt, IpcInfoP1->Path.Size.Y);
            IpcInfoP1->Path.Size.Lineoffset_CC = tmpInfo.LineOfs;
            rt |= IPL_IME_PATH1;
        }
        else
        {
            DBG_IND("IPL_ChkImePathInfo IPL_IME_PATH1 fail\r\n");
        }
    }

    IPL_BufGet(Info->Id, IPLBUF_IMEP2_1, &BufAddr, &Size);
    if ((Info->PathId & IPL_IME_PATH2) && (Size != 0))
    {
        //p2
        if (IPL_ChkImePathInfo(Info->Id, IPL_IME_PATH2, &Info->ImeP2) == TRUE)
        {
            IPL_SetImePathInfo(Info->Id, IPL_IME_PATH2, &Info->ImeP2);
            IPL_GetImePathInfo(Info->Id, IPL_IME_PATH2, &tmpImeInfo);
            IpcInfoP2->Path.PathEn = tmpImeInfo.OutputEn;
            IpcInfoP2->Path.Size.Y.Width = tmpImeInfo.ImgSizeH;
            IpcInfoP2->Path.Size.Y.Height = tmpImeInfo.ImgSizeV;
            IpcInfoP2->Path.Size.Y.LineOfs = tmpImeInfo.ImgSizeLOfs;
            IpcInfoP2->Path.Fmt = tmpImeInfo.ImgFmt;
            IpcInfoP2->Path.Crop.Start.x = 0;
            IpcInfoP2->Path.Crop.Start.y = 0;
            IpcInfoP2->Path.Crop.SizeH = IpcInfoP2->Path.Size.Y.Width;
            IpcInfoP2->Path.Crop.SizeV = IpcInfoP2->Path.Size.Y.Height;
            tmpInfo = IPL_UTI_Y_INFO_CONV2(IpcInfoP2->Path.Fmt, IpcInfoP2->Path.Size.Y);
            IpcInfoP2->Path.Size.Lineoffset_CC = tmpInfo.LineOfs;
            rt |= IPL_IME_PATH2;
        }
        else
        {
            DBG_IND("IPL_ChkImePathInfo IPL_IME_PATH2 fail\r\n");
        }
    }

    IPL_BufGet(Info->Id, IPLBUF_IMEP3_1, &BufAddr, &Size);
    if ((Info->PathId & IPL_IME_PATH3) && (Size != 0))
    {
        //p3
        if (IPL_ChkImePathInfo(Info->Id, IPL_IME_PATH3, &Info->ImeP3) == TRUE)
        {
            IPL_SetImePathInfo(Info->Id, IPL_IME_PATH3, &Info->ImeP3);
            IPL_GetImePathInfo(Info->Id, IPL_IME_PATH3, &tmpImeInfo);
            IpcInfoP3->Path.PathEn = tmpImeInfo.OutputEn;
            IpcInfoP3->Path.Size.Y.Width = tmpImeInfo.ImgSizeH;
            IpcInfoP3->Path.Size.Y.Height = tmpImeInfo.ImgSizeV;
            IpcInfoP3->Path.Size.Y.LineOfs = tmpImeInfo.ImgSizeLOfs;
            IpcInfoP3->Path.Fmt = tmpImeInfo.ImgFmt;
            IpcInfoP3->Path.Crop.Start.x = 0;
            IpcInfoP3->Path.Crop.Start.y = 0;
            IpcInfoP3->Path.Crop.SizeH = IpcInfoP3->Path.Size.Y.Width;
            IpcInfoP3->Path.Crop.SizeV = IpcInfoP3->Path.Size.Y.Height;
            tmpInfo = IPL_UTI_Y_INFO_CONV2(IpcInfoP3->Path.Fmt, IpcInfoP3->Path.Size.Y);
            IpcInfoP3->Path.Size.Lineoffset_CC = tmpInfo.LineOfs;
            rt |= IPL_IME_PATH3;
        }
        else
        {
            DBG_IND("IPL_ChkImePathInfo IPL_IME_PATH3 fail\r\n");
        }
    }
    return rt;
}

void IPL_SetDZoomFCB(IPL_DZOOM *Info, IPC_FRONTEND *SetInfo)
{
    IPL_CtrlPrvSetDZoomInfo(Info->Id);
    IPL_CtrlPrvGetDZoomInfo(Info->Id, SetInfo);
}

SENSOR_MODE gSenCurMode;

void IPL_SetSleepFCB(IPL_SLEEP_INFO *Info)
{
    AE_Pause(IPL_UTI_CONV2_AE_ID(Info->Id), ENABLE);
    AWB_Pause(IPL_UTI_CONV2_AWB_ID(Info->Id), ENABLE);

    IPC_WaitVD(TRUE,Info->Id);
    IPL_CTRLStop(IPC_PAUSE_SIE);
    IPC_WaitVD(FALSE,Info->Id);
    ////IPL_CTRLStop(IPC_PAUSE_SIE2);
    ////IPC_WaitVD(FALSE,Info->Id);

    //Sensor_Sleep(SENSOR_ID_1);
    Sensor_GetCurMode(SENSOR_ID_1, &gSenCurMode);
    Sensor_Close(SENSOR_ID_1);
    DrvSensor_PowerSaveOn();
}

void IPL_SetWakeupFCB(IPL_WAKEUP_INFO *Info)
{
    SENSOR_INIT_OBJ SenObj;
    SENSOR_DRVTAB *SenTab;

    DrvSensor_PowerSaveOff();
    
    //Sensor_WakeUp(SENSOR_ID_1);
    SenObj = DrvSensor_GetObj1st();
    SenTab = DrvSensor_GetTab1st();
    Sensor_Open(SENSOR_ID_1, &SenObj, SenTab);
    Sensor_ChgMode(SENSOR_ID_1, gSenCurMode);

    ////IPL_ResetDisplayCtrlFlow();
    ////IPC_WaitVD(TRUE,Info->Id);

    IPL_CTRLStop(IPC_RESUME_SIE);
    IPC_WaitVD(FALSE,Info->Id);

    ////IPL_CTRLStop(IPC_RESUME_SIE2);
    ////IPC_WaitVD(FALSE,Info->Id);

    AE_Resume(IPL_UTI_CONV2_AE_ID(Info->Id));
    AWB_Resume(IPL_UTI_CONV2_AWB_ID(Info->Id));
}

static void IPL_GetCapRawFCB_NORMAL(IPL_GET_CAP_RAW_DATA *Info)
{
    IPC_SIE_Info SIEInfo = {0};
    SIE sie = {0};
    SIE_SUBFUNC SubFunc = {0};

    IPLCTRL_CAP_EXT_INFO ExtInfo;

    //prepare ipc sie information
    SIEInfo.Idx = IPL_CtrlGetHalSetId(Info->Id);
    SIEInfo.psie = &sie;
    SIEInfo.psie->pSubFunc = &SubFunc;

    IPL_CtrlInitCapIpcInfo(Info, &SIEInfo, &ExtInfo);

    IPL_FCB_GetCapRaw(Info->Id, &SIEInfo, &ExtInfo);

    //AWB setting
    IPL_FCB_SetCapAWB(Info->Id, &SIEInfo);
}

static void IPL_GetCapRawFCB_CCIR(IPL_GET_CAP_RAW_DATA *Info)
{
    IPC_SIE_Info SIEInfo = {0};
    SIE2 sie2 = {0};
    SIE2_SUBFUNC SubFunc2 = {0};

    IPLCTRL_CAP_EXT_INFO ExtInfo;

    //prepare ipc sie2 information
    SIEInfo.Idx = IPL_CtrlGetHalSetId(Info->Id);
    SIEInfo.psie2 = &sie2;
    SIEInfo.psie2->pSubFunc = &SubFunc2;

    IPL_CtrlInitCapIpcInfo_CCIR(Info, &SIEInfo, &ExtInfo);

    IPL_FCB_GetCapRaw(Info->Id, &SIEInfo, &ExtInfo);
}

void IPL_GetCapRawFCB(IPL_GET_CAP_RAW_DATA *Info)
{
    if (IPLCTRL_CAP_FLOW_CCIR == IPL_CtrlGetInfor(Info->Id, IPLCTRL_CAPFLOW))
    {
        IPL_GetCapRawFCB_CCIR(Info);
    }
    else if (IPLCTRL_CAP_FLOW_DUALSENSOR == IPL_CtrlGetInfor(Info->Id, IPLCTRL_CAPFLOW))
    {
        if (Info->Id == IPL_ID_1)
        {
            IPL_GetCapRawFCB_NORMAL(Info);
        }
        else if (Info->Id == IPL_ID_2)
        {
            IPL_GetCapRawFCB_CCIR(Info);
        }
        else
        {
            DBG_ERR("not support\r\n");
        }
    }
    else
    {
        IPL_GetCapRawFCB_NORMAL(Info);
    }
}

SIE_SUBFUNC gSieSubFunc = {0}; //for Pauase/Resume DMA
void IPL_SetPauseDMAFCB(void)
{
    SIE Sie = {0};
    SIE_SUBFUNC SieSubFunc = {0};
    IPL_HAL_GROUP SieGroup = {0};
    IPL_HAL_READ readBit = {0};

    AE_Pause(IPL_UTI_CONV2_AE_ID(IPL_ID_1), ENABLE);
    AWB_Pause(IPL_UTI_CONV2_AWB_ID(IPL_ID_1), ENABLE);

    Sie.pSubFunc = &SieSubFunc;
    readBit.sieRead = SIE_R_SubFEn;
    IPH_ENG_Read(IPL_ID_1,IPL_HAL_SIE, IPL_CtrlGetHalSetId(IPL_ID_1), readBit, (void*)&Sie.pSubFunc->sieFuncEn);
    gSieSubFunc.sieFuncEn = Sie.pSubFunc->sieFuncEn;
    SieGroup.sieUpdate = SIE_SubFEn;
    Sie.pSubFunc->sieFuncEn = 0;
    IPH_SIE_ChgParam(IPL_CtrlGetHalSetId(IPL_ID_1), SieGroup, &Sie);
    IPC_WaitVD(TRUE,IPL_ID_1);
    IPH_ENG_Load(IPL_ID_1,IPL_HAL_SIE, IPL_CtrlGetHalSetId(IPL_ID_1), SieGroup);
    IPL_CTRLStop(IPC_PAUSE_SIE);
    IPC_WaitVD(FALSE,IPL_ID_1);
}

void IPL_SetResumeDMAFCB(void)
{
    SIE Sie = {0};
    IPL_HAL_GROUP SieGroup = {0};

    SieGroup.sieUpdate = SIE_SubFEn;
    Sie.pSubFunc = &gSieSubFunc;
    IPL_ResetDisplayCtrlFlow();
    IPH_SIE_ChgParam(IPL_CtrlGetHalSetId(IPL_ID_1), SieGroup, &Sie);
    IPC_WaitVD(TRUE,IPL_ID_1);
    IPH_ENG_Load(IPL_ID_1,IPL_HAL_SIE, IPL_CtrlGetHalSetId(IPL_ID_1), SieGroup);
    IPL_CTRLStop(IPC_RESUME_SIE);
    IPC_WaitVD(FALSE,IPL_ID_1);

    AE_Resume(IPL_UTI_CONV2_AE_ID(IPL_ID_1));
    AWB_Resume(IPL_UTI_CONV2_AWB_ID(IPL_ID_1));
}

IPL_IME_PATH IPL_SetImgRatioFCB(IPL_SET_IMGRATIO_DATA *Info, IPC_FRONTEND *IpcInfo, IPC_IMEOut_Info *IpcInfoP1, IPC_IMEOut_Info *IpcInfoP2, IPC_IMEOut_Info *IpcInfoP3)
{
    IPL_IME_PATH rt = IPL_IME_PATH_NONE;
    UINT32 BufAddr, Size;
    IPL_IME_PATH_INFO tmpImeInfo;
    IPL_YCC_CH_INFO tmpInfo;

    if (Info->CropRatio != IPL_SENCROPRATIO_NONE)
    {
        IPL_CtrlSetInfor(Info->Id, IPLCTRL_SEN_PRVCROPRATIO, Info->CropRatio);
    }

    IPL_CtrlPrvSetDZoomInfo(Info->Id);
    IPL_CtrlPrvGetDZoomInfo(Info->Id, IpcInfo);

    IpcInfoP1->Idx = IPL_CtrlGetHalSetId(IPL_ID_1);
    IpcInfoP2->Idx = IPL_CtrlGetHalSetId(IPL_ID_1);
    IpcInfoP3->Idx = IPL_CtrlGetHalSetId(IPL_ID_1);

    IPL_BufGet(Info->Id, IPLBUF_IMEP1_1, &BufAddr, &Size);
    if ((Info->PathId & IPL_IME_PATH1) && (Size != 0))
    {
        //p1
        if (IPL_ChkImePathInfo(Info->Id, IPL_IME_PATH1, &Info->ImeP1) == TRUE)
        {
            IPL_SetImePathInfo(Info->Id, IPL_IME_PATH1, &Info->ImeP1);
            IPL_GetImePathInfo(Info->Id, IPL_IME_PATH1, &tmpImeInfo);
            IpcInfoP1->Path.PathEn = tmpImeInfo.OutputEn;
            IpcInfoP1->Path.Size.Y.Width = tmpImeInfo.ImgSizeH;
            IpcInfoP1->Path.Size.Y.Height = tmpImeInfo.ImgSizeV;
            IpcInfoP1->Path.Size.Y.LineOfs = tmpImeInfo.ImgSizeLOfs;
            IpcInfoP1->Path.Fmt = tmpImeInfo.ImgFmt;
            IpcInfoP1->Path.Crop.Start.x = 0;
            IpcInfoP1->Path.Crop.Start.y = 0;
            IpcInfoP1->Path.Crop.SizeH = IpcInfoP1->Path.Size.Y.Width;
            IpcInfoP1->Path.Crop.SizeV = IpcInfoP1->Path.Size.Y.Height;
            tmpInfo = IPL_UTI_Y_INFO_CONV2(IpcInfoP1->Path.Fmt, IpcInfoP1->Path.Size.Y);
            IpcInfoP1->Path.Size.Lineoffset_CC = tmpInfo.LineOfs;
            rt |= IPL_IME_PATH1;
        }
        else
        {
            DBG_IND("IPL_ChkImePathInfo IPL_IME_PATH1 fail\r\n");
        }
    }

    IPL_BufGet(Info->Id, IPLBUF_IMEP2_1, &BufAddr, &Size);
    if ((Info->PathId & IPL_IME_PATH2) && (Size != 0))
    {
        //p2
        if (IPL_ChkImePathInfo(Info->Id, IPL_IME_PATH2, &Info->ImeP2) == TRUE)
        {
            IPL_SetImePathInfo(Info->Id, IPL_IME_PATH2, &Info->ImeP2);
            IPL_GetImePathInfo(Info->Id, IPL_IME_PATH2, &tmpImeInfo);
            IpcInfoP2->Path.PathEn = tmpImeInfo.OutputEn;
            IpcInfoP2->Path.Size.Y.Width = tmpImeInfo.ImgSizeH;
            IpcInfoP2->Path.Size.Y.Height = tmpImeInfo.ImgSizeV;
            IpcInfoP2->Path.Size.Y.LineOfs = tmpImeInfo.ImgSizeLOfs;
            IpcInfoP2->Path.Fmt = tmpImeInfo.ImgFmt;
            IpcInfoP2->Path.Crop.Start.x = 0;
            IpcInfoP2->Path.Crop.Start.y = 0;
            IpcInfoP2->Path.Crop.SizeH = IpcInfoP2->Path.Size.Y.Width;
            IpcInfoP2->Path.Crop.SizeV = IpcInfoP2->Path.Size.Y.Height;
            tmpInfo = IPL_UTI_Y_INFO_CONV2(IpcInfoP2->Path.Fmt, IpcInfoP2->Path.Size.Y);
            IpcInfoP2->Path.Size.Lineoffset_CC = tmpInfo.LineOfs;
            rt |= IPL_IME_PATH2;
        }
        else
        {
            DBG_IND("IPL_ChkImePathInfo IPL_IME_PATH2 fail\r\n");
        }
    }

    IPL_BufGet(Info->Id, IPLBUF_IMEP3_1, &BufAddr, &Size);
    if ((Info->PathId & IPL_IME_PATH3) && (Size != 0))
    {
        //p3
        if (IPL_ChkImePathInfo(Info->Id, IPL_IME_PATH3, &Info->ImeP3) == TRUE)
        {
            IPL_SetImePathInfo(Info->Id, IPL_IME_PATH3, &Info->ImeP3);
            IPL_GetImePathInfo(Info->Id, IPL_IME_PATH3, &tmpImeInfo);
            IpcInfoP3->Path.PathEn = tmpImeInfo.OutputEn;
            IpcInfoP3->Path.Size.Y.Width = tmpImeInfo.ImgSizeH;
            IpcInfoP3->Path.Size.Y.Height = tmpImeInfo.ImgSizeV;
            IpcInfoP3->Path.Size.Y.LineOfs = tmpImeInfo.ImgSizeLOfs;
            IpcInfoP3->Path.Fmt = tmpImeInfo.ImgFmt;
            IpcInfoP3->Path.Crop.Start.x = 0;
            IpcInfoP3->Path.Crop.Start.y = 0;
            IpcInfoP3->Path.Crop.SizeH = IpcInfoP3->Path.Size.Y.Width;
            IpcInfoP3->Path.Crop.SizeV = IpcInfoP3->Path.Size.Y.Height;
            tmpInfo = IPL_UTI_Y_INFO_CONV2(IpcInfoP3->Path.Fmt, IpcInfoP3->Path.Size.Y);
            IpcInfoP3->Path.Size.Lineoffset_CC = tmpInfo.LineOfs;
            rt |= IPL_IME_PATH3;
        }
        else
        {
            DBG_IND("IPL_ChkImePathInfo IPL_IME_PATH3 fail\r\n");
        }
    }

    return rt;
}

void IPL_SetVAFCB(IPL_SET_VA_DATA *Info, IPL_HAL_PARASET *HalParaIdx, UINT32 *OutAddr, UINT32 *OutAddr1)
{
    UINT32 OutSize;

    *HalParaIdx = IPL_CtrlGetHalSetId(Info->Id);
    IPL_BufGet(Info->Id, IPLBUF_IPEVA_1, OutAddr, &OutSize);
    IPL_BufGet(Info->Id, IPLBUF_IPEVA_2, OutAddr1, &OutSize);
}

IPL_YCC_IMG_INFO IPL_GetImeCurInfoFCB(IPL_IME_PATH Id)
{
    IPL_YCC_IMG_INFO tmp = {0};
    if (Id == IPL_IME_PATH1)
    {
        tmp = IPL_GetIMEReadyBufferAddr(0);
    }
    else if (Id == IPL_IME_PATH2)
    {
        tmp = IPL_GetIMEReadyBufferAddr(1);
    }
    else if (Id == IPL_IME_PATH3)
    {
        tmp = IPL_GetIMEReadyBufferAddr(2);
    }
    else
    {
        DBG_ERR("IPL_GetImeCurInfoFCB fail\r\n");
    }
    return tmp;
}

IPL_YCC_IMG_INFO IPL_GetIme2CurInfoFCB(IPL_IME_PATH Id)
{
    IPL_YCC_IMG_INFO tmp = {0};
    if (Id == IPL_IME_PATH1)
    {
        tmp = IPL_GetReadyBufferAddr2(0);
    }
    else if (Id == IPL_IME_PATH2)
    {
        tmp = IPL_GetReadyBufferAddr2(1);
    }
    else if (Id == IPL_IME_PATH3)
    {
        tmp = IPL_GetReadyBufferAddr2(2);
    }
    else
    {
        DBG_ERR("IPL_GetImeCurInfoFCB fail\r\n");
    }
    return tmp;
}

IPL_YCC_IMG_INFO IPL_GetImeInfoFCB(IPL_IME_PATH Id, UINT32 BufId)
{
    IPL_YCC_IMG_INFO tmp = {0};
    if (Id == IPL_IME_PATH1)
    {
        tmp = IPL_GetSpecifiedIMEoutputAddr(0, BufId);
    }
    else if (Id == IPL_IME_PATH2)
    {
        tmp = IPL_GetSpecifiedIMEoutputAddr(1, BufId);
    }
    else if (Id == IPL_IME_PATH3)
    {
        tmp = IPL_GetSpecifiedIMEoutputAddr(2, BufId);
    }
    else
    {
        DBG_ERR("IPL_GetImeInfoFCB fail\r\n");
    }
    return tmp;
}


IPL_YCC_IMG_INFO IPL_GetIme2InfoFCB(IPL_PROC_ID Ipl_Id, IPL_IME_PATH Id, UINT32 BufId)
{
    IPL_YCC_IMG_INFO tmp = {0};
    if (Id == IPL_IME_PATH1)
    {
        tmp = IPL_GetSpecifiedIMEoutputAddr2(0, BufId);
        tmp.type = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_FMT);
        tmp.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_DISP_SIZE_H);
        tmp.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_DISP_SIZE_HLOS);
        tmp.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_DISP_SIZE_V);
        tmp.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(tmp.type, tmp.Ch[IPL_IMG_YUV_Y]);
        tmp.Ch[IPL_IMG_YUV_V] = tmp.Ch[IPL_IMG_YUV_U];
    }
    else if (Id == IPL_IME_PATH2)
    {
        tmp = IPL_GetSpecifiedIMEoutputAddr2(1, BufId);
        tmp.type = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_FMT);
        tmp.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_DISP_SIZE_H);
        tmp.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_DISP_SIZE_HLOS);
        tmp.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_DISP_SIZE_V);
        tmp.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(tmp.type, tmp.Ch[IPL_IMG_YUV_Y]);
        tmp.Ch[IPL_IMG_YUV_V] = tmp.Ch[IPL_IMG_YUV_U];
    }
    else if (Id == IPL_IME_PATH3)
    {
        tmp = IPL_GetSpecifiedIMEoutputAddr2(2, BufId);
        tmp.type = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_FMT);
        tmp.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_DISP_SIZE_H);
        tmp.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_DISP_SIZE_HLOS);
        tmp.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_DISP_SIZE_V);
        tmp.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(tmp.type, tmp.Ch[IPL_IMG_YUV_Y]);
        tmp.Ch[IPL_IMG_YUV_V] = tmp.Ch[IPL_IMG_YUV_U];
    }
    else
    {
        DBG_ERR("IPL_GetImeInfoFCB fail\r\n");
    }
    return tmp;
}

