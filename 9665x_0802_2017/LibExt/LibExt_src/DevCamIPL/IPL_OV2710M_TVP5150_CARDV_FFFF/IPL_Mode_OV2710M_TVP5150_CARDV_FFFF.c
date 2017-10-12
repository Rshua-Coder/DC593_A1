/*
    IPL mode API

    This file is IPL mode control.

    @file       IPL_Mode_AR0330_TVP5150_CARDV_FFFF.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include "IPL_OV2710M_TVP5150_CARDV_FFFF_Int.h"

static void IPL_MapSensorMode(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 Video_FPS = IPL_AlgGetUIInfo(IPL_SEL_VIDEO_FPS);
    DBG_ERR("Video_FPS %d\r\n", Video_FPS);
    switch(ChgMode->Mode)
    {
        case IPL_MODE_VIDEOREC:
        case IPL_MODE_VIDEO:
            switch(Video_FPS)
            {
                case SEL_VIDEOFPS_25:
                    IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_1);
                    IPL_AlgSetUIInfo(IPL_SEL_CAPSENMODE, SENSOR_MODE_1);
                case SEL_VIDEOFPS_30:
                    IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_2);
                    IPL_AlgSetUIInfo(IPL_SEL_CAPSENMODE, SENSOR_MODE_2);
                    break;
                case SEL_VIDEOFPS_60:
                    IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_3);
                    IPL_AlgSetUIInfo(IPL_SEL_CAPSENMODE, SENSOR_MODE_3);
                    break;
                case SEL_VIDEOFPS_120:
                    IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_4);
                    IPL_AlgSetUIInfo(IPL_SEL_CAPSENMODE, SENSOR_MODE_4);
                    break;
                default:
                    DBG_ERR("^RInvalidate video fps\r\n");
                    IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_2);
                    IPL_AlgSetUIInfo(IPL_SEL_CAPSENMODE, SENSOR_MODE_2);
                    break;
            }
            break;
        case IPL_MODE_PREVIEW:
        case IPL_MODE_CAP:
            IPL_AlgSetUIInfo(IPL_SEL_CAPSENMODE, SENSOR_MODE_2);
            IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_2);
            break;
        default:
            DBG_ERR("^Rsensor mode map fail %d\r\n", ChgMode->Mode);
            IPL_AlgSetUIInfo(IPL_SEL_CAPSENMODE, SENSOR_MODE_2);
            IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_2);
            break;
    }

    IPL_AlgSetUIInfo(IPL_SEL_RAW_BITDEPTH_PRV, SEL_RAW_BITDEPTH_10);
    IPL_AlgSetUIInfo(IPL_SEL_RAW_BITDEPTH_CAP, SEL_RAW_BITDEPTH_10);
}


static IPLBUF_MODE IPL_MapICF2BufMode(IPLCTRL_FLOW Flow)
{
    switch(Flow)
    {
        case ICF_FLOW_A:
            return IPLBUF_MODE_FLOW_A;

        case ICF_FLOW_B:
            return IPLBUF_MODE_FLOW_B;

        case ICF_FLOW_C:
            return IPLBUF_MODE_FLOW_C;

        case ICF_FLOW_D:
            return IPLBUF_MODE_FLOW_D;

        default:
            DBG_ERR("flow map fail %d\r\n", Flow);
            return 0;
    }
}

static IPLBUF_RST IPL_Init_Proc(IPL_PROC_ID Id, IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode,
                                IPL_ISR_OBJ_INFO *IsrObjInfo, IPC_INFO *IpcInfo, AE_OBJ_INFO *AeObj,
                                AWB_OBJ_INFO *AwbObj, AF_OBJ_INFO *AfObj, UINT32 RevDZFlag, UINT32 Cap2PrvFlag)
{
    UINT32 *ZoomTab, ZoomTabMax, DZIdx = 0;
    SENSOR_INFO Info;
    IPL_BUF_OBJ_INFO BufObj;
    IPLBUF_ENG2DRAMINFO BufInfo;
    IPLBUF_RST BufRst = {0};
    IPLCTRL_INFO CtrlInfo;
    IPL_IME_PATH_INFO PathInfo1;
    IPL_IME_PATH_INFO PathInfo2;
    IPL_IME_PATH_INFO PathInfo3;

    if (Cap2PrvFlag == TRUE)
    {
        IPL_GetImePathORGInfo(ChgMode->Id, IPL_IME_PATH1, &PathInfo1);
        IPL_GetImePathORGInfo(ChgMode->Id, IPL_IME_PATH2, &PathInfo2);
        IPL_GetImePathORGInfo(ChgMode->Id, IPL_IME_PATH3, &PathInfo3);
    }


    //IPL_AlgSetUIInfo2(Id, IPL_SEL_PRVSENMODE, SENSOR_MODE_4);
    //IPL_AlgSetUIInfo2(Id, IPL_SEL_RAW_BITDEPTH_PRV, SEL_RAW_BITDEPTH_10);

    IPL_AlgSetEventFP(Id, IPL_SEL_3DNR, IPL_FCB_Alg3DNR);
    IPL_AlgSetEventFP(Id, IPL_SEL_GDCCOMP, IPL_FCB_AlgGDC);
    IPL_AlgSetEventFP(Id, IPL_SEL_DISCOMP, IPL_FCB_AlgDIS);
    IPL_AlgSetEventFP(Id, IPL_SEL_IMAGEEFFECT, IPL_FCB_AlgIE);
    IPL_AlgSetEventFP(Id, IPL_SEL_RSC, IPL_FCB_AlgRSC);
    IPL_AlgSetEventFP(Id, IPL_SEL_WDR, IPL_FCB_AlgWDR);

    IPL_CBMsgInit2(Id, ChgMode->CB_FP);

    //single sensor flow
    IPL_CtrlSetInfor(Id, IPLCTRL_PRVFLOW, ICF_FLOW_D);

    AeObj->EventTab[IPL_UTI_CONV2_AE_ID(Id)].Start = AE_Init;
    AeObj->EventTab[IPL_UTI_CONV2_AE_ID(Id)].Process = AE_Process;
    AeObj->EventTab[IPL_UTI_CONV2_AE_ID(Id)].Pause = NULL;

    AwbObj->EventTab[IPL_UTI_CONV2_AWB_ID(Id)].Start = AWB_Init;
    AwbObj->EventTab[IPL_UTI_CONV2_AWB_ID(Id)].Process = AWB_Process;
    AwbObj->EventTab[IPL_UTI_CONV2_AWB_ID(Id)].Pause = NULL;

    AfObj->EventTab[IPL_UTI_CONV2_AF_ID(Id)].Start = NULL;
    AfObj->EventTab[IPL_UTI_CONV2_AF_ID(Id)].Process = NULL;
    AfObj->EventTab[IPL_UTI_CONV2_AF_ID(Id)].Pause = NULL;

    Sensor_ChgMode(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo2(Id, IPL_SEL_PRVSENMODE));
    //get mode status
    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo2(Id, IPL_SEL_PRVSENMODE), &Info);
    //IPL data initial
    ZoomTab = SenMode2Tbl(Id, IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &ZoomTabMax);

    IPL_DZoomSetInfor(Id, ZoomTab, ZoomTabMax);
    if (RevDZFlag == TRUE)
    {
        DZIdx = IPL_DzoomGetIndex(Id);
    }
    IPL_DzoomSetIndex(Id, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO, ChgMode->CropRatio);

    //decide preivew flow
    CtrlInfo.Info = &Info;
    CtrlInfo.ChgMode = ChgMode;
    IPL_CtrlPrvInit(Id, &CtrlInfo);

    BufObj.Id = Id;
    BufObj.BufAddr = ChgMode->BufAddr;
    BufObj.BufSize = ChgMode->BufSize;
    IPL_BufInit(&BufObj);
    IPL_CtrlInitPrvBufInfo(BufObj.Id, ChgMode, &BufInfo);
    IPL_CtrlInitPrvExtBufInfo(BufObj.Id, ChgMode, &BufInfo);
    BufRst = IPL_BufAssign(IPL_MapICF2BufMode(IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW)), &BufInfo);
    IPL_CtrlInitPrvPProcInfo(Id, ChgMode->FuncEn);
    if (RevDZFlag == TRUE)
    {
        IPL_DzoomSetIndex(Id, DZIdx);
    }
    IPL_CtrlPrvSetDZoomInfo(Id);
    if (Cap2PrvFlag == TRUE)
    {
        IPL_SetImePathInfo(ChgMode->Id, IPL_IME_PATH1, &PathInfo1);
        IPL_SetImePathInfo(ChgMode->Id, IPL_IME_PATH2, &PathInfo2);
        IPL_SetImePathInfo(ChgMode->Id, IPL_IME_PATH3, &PathInfo3);
    }
    IPL_CtrlInitPrvIpcInfo(Id, IpcInfo, IsrObjInfo, ChgMode);
    IPL_ResetDisplayCtrlFlow();
    return BufRst;
}


static IPLBUF_RST IPL_Init_Proc_CCIR(IPL_PROC_ID Id, IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode,
                                    IPL_ISR_OBJ_INFO *IsrObjInfo, IPC_INFO *IpcInfo, AE_OBJ_INFO *AeObj, AWB_OBJ_INFO *AwbObj, AF_OBJ_INFO *AfObj, UINT32 RevDZFlag)
{
    UINT32 *ZoomTab, ZoomTabMax;
    SENSOR_INFO Info;
    IPL_BUF_OBJ_INFO BufObj;
    IPLBUF_ENG2DRAMINFO BufInfo;
    IPLBUF_RST BufRst = {0};
    IPLCTRL_INFO CtrlInfo;

    IPL_AlgSetUIInfo2(Id, IPL_SEL_PRVSENMODE, SENSOR_MODE_1);
    IPL_AlgSetUIInfo2(Id, IPL_SEL_CAPSENMODE, SENSOR_MODE_1);

    IPL_AlgSetUIInfo2(Id, IPL_SEL_RAW_BITDEPTH_PRV, SEL_RAW_BITDEPTH_8);
    IPL_AlgSetUIInfo2(Id, IPL_SEL_RAW_BITDEPTH_CAP, SEL_RAW_BITDEPTH_8);

    IPL_CBMsgInit2(Id, NULL);
    //single sensor flow
    IPL_CtrlSetInfor(Id, IPLCTRL_PRVFLOW, ICF_FLOW_D);

    Sensor_ChgMode(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo2(Id, IPL_SEL_PRVSENMODE));

    //get mode status
    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo2(Id, IPL_SEL_PRVSENMODE), &Info);
    //IPL data initial
    ZoomTab = SenMode2Tbl(Id, IPL_AlgGetUIInfo2(Id, IPL_SEL_PRVSENMODE), &ZoomTabMax);

    IPL_DZoomSetInfor(Id, ZoomTab, ZoomTabMax);
    IPL_DzoomSetIndex(Id, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO, ChgMode->CropRatio);

    //decide preivew flow
    CtrlInfo.Info = &Info;
    CtrlInfo.ChgMode = ChgMode;
    IPL_CtrlPrvInit_CCIR(Id, &CtrlInfo);

    BufObj.Id = Id;
    BufObj.BufAddr = ChgMode->BufAddr;
    BufObj.BufSize = ChgMode->BufSize;
    IPL_BufInit(&BufObj);
    IPL_CtrlInitPrvBufInfo_CCIR(BufObj.Id, ChgMode, &BufInfo);
    BufRst = IPL_BufAssign(IPL_MapICF2BufMode(IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW)), &BufInfo);
    IPL_CtrlInitPrvPProcInfo(Id, ChgMode->FuncEn);
    IPL_CtrlInitPrvIpcInfo_CCIR(Id, IpcInfo, IsrObjInfo, ChgMode);
    IPL_ResetDisplayCtrlFlow2();
    return BufRst;
}


static ER IPL_Rec2Cap(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    SENSOR_INFO Info;
    IPLCTRL_INFO CtrlInfo;

    if (PrvMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
    {
        //pause 2A
        AE_Pause(IPL_UTI_CONV2_AE_ID(IPL_ID_1), ENABLE);
        AWB_Pause(IPL_UTI_CONV2_AWB_ID(IPL_ID_1), ENABLE);
    }

    //initial capture parameter
    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(ChgMode->Id), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &Info);
    CtrlInfo.Info = &Info;
    CtrlInfo.ChgMode = ChgMode;
    IPL_CtrlCapInit(ChgMode->Id, ChgMode->EventId, &CtrlInfo);
    ChgMode->BufSize = 0;
    return E_OK;
}

static ER IPL_Cap2Rec(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    if (ChgMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
    {
        AE_Resume(IPL_UTI_CONV2_AE_ID(IPL_ID_1));
        AWB_Resume(IPL_UTI_CONV2_AWB_ID(IPL_ID_1));
    }
    return E_OK;
}

static ER IPL_Prv2Vid(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    DBG_IND("no change(the same as prv mode)\r\n");
    ChgMode->BufSize = PrvMode->BufSize;
    return E_OK;
}

static ER IPL_Vid2Prv(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    DBG_IND("no change(the same as prv mode)\r\n");
    ChgMode->BufSize = PrvMode->BufSize;
    return E_OK;
}

static ER IPL_Vid2Rec(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 OrgAddr;
    UINT32 UsedSize;
    IPL_ISR_OBJ_INFO IsrObjInfo = {0};
    AE_OBJ_INFO AeObj = {0};
    AWB_OBJ_INFO AwbObj = {0};
    AF_OBJ_INFO AfObj = {0};
    IPC_INFO IpcInfo = {0};
    IPLBUF_RST BufRst;

    if (PrvMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
    {
        //pause 2A
        AE_Pause(IPL_UTI_CONV2_AE_ID(IPL_ID_1), ENABLE);
        AWB_Pause(IPL_UTI_CONV2_AWB_ID(IPL_ID_1), ENABLE);
    }

    //stop IPL
    if (PrvMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
    {
        IPL_FreezeDisplayImage(FALSE);
    }
    if (PrvMode->ProcSenId[IPL_ID_2] != SENSOR_ID_NONE)
    {
        IPL_FreezeDisplayImage2(FALSE);
    }
    IPL_CTRLStop(IPC_STOP_ALL);

    //UINT32 preSensorMode = IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE);

    if (ChgMode->SenNum == 2)
    {
        if ((ChgMode->ProcSenId[IPL_ID_1] == SENSOR_ID_NONE) && (ChgMode->ProcSenId[IPL_ID_2] == SENSOR_ID_NONE))
        {
            DBG_ERR("fail unsupport condition\r\n");
        }
        else
        {
            //mipi + CCIR
            OrgAddr = ChgMode->BufAddr;
            UsedSize = 0;
            BufRst.BufSize = 0;
            BufRst.er_code = E_OK;
            if (ChgMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
            {
                IPL_MapSensorMode(ChgMode, PrvMode);
                BufRst = IPL_Init_Proc(IPL_ID_1, ChgMode, PrvMode, &IsrObjInfo, &IpcInfo, &AeObj, &AwbObj, &AfObj, TRUE, FALSE);
                //IPL_BufDump(IPL_ID_1);
            }
            UsedSize += BufRst.BufSize;
            ChgMode->BufAddr = ChgMode->BufAddr + BufRst.BufSize;
            ChgMode->BufSize = ChgMode->BufSize - BufRst.BufSize;

            BufRst.BufSize = 0;
            BufRst.er_code = E_OK;
            if (ChgMode->ProcSenId[IPL_ID_2] != SENSOR_ID_NONE)
            {
                BufRst = IPL_Init_Proc_CCIR(IPL_ID_2, ChgMode, PrvMode, &IsrObjInfo, &IpcInfo, &AeObj, &AwbObj, &AfObj, FALSE);
                //IPL_BufDump(IPL_ID_2);
            }
            UsedSize += BufRst.BufSize;

            ChgMode->BufAddr = OrgAddr;
            ChgMode->BufSize = UsedSize;

            if (ChgMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
            {
                AE_Resume(IPL_UTI_CONV2_AE_ID(IPL_ID_1));
                AWB_Resume(IPL_UTI_CONV2_AWB_ID(IPL_ID_1));
                IsrObjInfo.ProcNum = 1;
            }
        }
    }
    else
    {
        //only one sensor
    }
    DBG_ERR("Sensor Mode %d\r\n", IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));

    IPL_CTRLRun2(ICF_FLOW_D, &IpcInfo, &IsrObjInfo, IPL_CtrlPostImage,IPL_CtrlPostImage2);
    IPC_WaitImeFMD(TRUE);

    IPL_CBMsgProc(IPL_CBMSG_PREVIEW, NULL);

    AE_WaitStable(3);
    IPL_CBMsgProc(IPL_CBMSG_PREVIEWSTABLE, NULL);
    return E_OK;
}

static ER IPL_Rec2Vid(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 OrgAddr;
    UINT32 UsedSize;
    IPL_ISR_OBJ_INFO IsrObjInfo = {0};
    AE_OBJ_INFO AeObj = {0};
    AWB_OBJ_INFO AwbObj = {0};
    AF_OBJ_INFO AfObj = {0};
    IPC_INFO IpcInfo = {0};
    IPLBUF_RST BufRst;

    if (PrvMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
    {
        //pause 2A
        AE_Pause(IPL_UTI_CONV2_AE_ID(IPL_ID_1), ENABLE);
        AWB_Pause(IPL_UTI_CONV2_AWB_ID(IPL_ID_1), ENABLE);
    }

    //stop IPL
    if (PrvMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
    {
        IPL_FreezeDisplayImage(FALSE);
    }
    if (PrvMode->ProcSenId[IPL_ID_2] != SENSOR_ID_NONE)
    {
        IPL_FreezeDisplayImage2(FALSE);
    }
    IPL_CTRLStop(IPC_STOP_ALL);

    if (ChgMode->SenNum == 2)
    {
        if ((ChgMode->ProcSenId[IPL_ID_1] == SENSOR_ID_NONE) && (ChgMode->ProcSenId[IPL_ID_2] == SENSOR_ID_NONE))
        {
            DBG_ERR("fail unsupport condition\r\n");
        }
        else
        {
            //mipi + CCIR
            OrgAddr = ChgMode->BufAddr;
            UsedSize = 0;
            BufRst.BufSize = 0;
            BufRst.er_code = E_OK;
            if (ChgMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
            {
                //IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_4);
                IPL_MapSensorMode(ChgMode, PrvMode);
                IPL_AlgSetUIInfo(IPL_SEL_RAW_BITDEPTH_PRV, SEL_RAW_BITDEPTH_10);
                BufRst = IPL_Init_Proc(IPL_ID_1, ChgMode, PrvMode, &IsrObjInfo, &IpcInfo, &AeObj, &AwbObj, &AfObj, TRUE, FALSE);
                //IPL_BufDump(IPL_ID_1);
            }
            UsedSize += BufRst.BufSize;
            ChgMode->BufAddr = ChgMode->BufAddr + BufRst.BufSize;
            ChgMode->BufSize = ChgMode->BufSize - BufRst.BufSize;

            BufRst.BufSize = 0;
            BufRst.er_code = E_OK;
            if (ChgMode->ProcSenId[IPL_ID_2] != SENSOR_ID_NONE)
            {
                BufRst = IPL_Init_Proc_CCIR(IPL_ID_2, ChgMode, PrvMode, &IsrObjInfo, &IpcInfo, &AeObj, &AwbObj, &AfObj, FALSE);
                //IPL_BufDump(IPL_ID_2);
            }
            UsedSize += BufRst.BufSize;

            ChgMode->BufAddr = OrgAddr;
            ChgMode->BufSize = UsedSize;

            if (ChgMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
            {
                AE_Resume(IPL_UTI_CONV2_AE_ID(IPL_ID_1));
                AWB_Resume(IPL_UTI_CONV2_AWB_ID(IPL_ID_1));
                IsrObjInfo.ProcNum = 1;
            }
        }
    }
    else
    {
        //only one sensor
    }

    IPL_CTRLRun2(ICF_FLOW_D, &IpcInfo, &IsrObjInfo, IPL_CtrlPostImage,IPL_CtrlPostImage2);
    IPC_WaitImeFMD(TRUE);

    IPL_CBMsgProc(IPL_CBMSG_PREVIEW, NULL);

    AE_WaitStable(3);
    IPL_CBMsgProc(IPL_CBMSG_PREVIEWSTABLE, NULL);
    return E_OK;
}

static ER IPL_Off2Prv(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 OrgAddr;
    UINT32 UsedSize;
    IPL_ISR_OBJ_INFO IsrObjInfo = {0};
    AE_OBJ_INFO AeObj = {0};
    AWB_OBJ_INFO AwbObj = {0};
    AF_OBJ_INFO AfObj = {0};
    IPC_INFO IpcInfo = {0};
    IPLBUF_RST BufRst;

    if (ChgMode->SenNum == 2)
    {
        if ((ChgMode->ProcSenId[IPL_ID_1] == SENSOR_ID_NONE) && (ChgMode->ProcSenId[IPL_ID_2] == SENSOR_ID_NONE))
        {
            DBG_ERR("fail unsupport condition\r\n");
        }
        else
        {
            //mipi + CCIR
            OrgAddr = ChgMode->BufAddr;
            UsedSize = 0;
            BufRst.BufSize = 0;
            BufRst.er_code = E_OK;
            if (ChgMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
            {
                //IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_4);
                IPL_MapSensorMode(ChgMode, PrvMode);
                IPL_AlgSetUIInfo(IPL_SEL_RAW_BITDEPTH_PRV, SEL_RAW_BITDEPTH_10);
                BufRst = IPL_Init_Proc(IPL_ID_1, ChgMode, PrvMode, &IsrObjInfo, &IpcInfo, &AeObj, &AwbObj, &AfObj, FALSE, FALSE);
                //IPL_BufDump(IPL_ID_1);
            }
            UsedSize += BufRst.BufSize;
            ChgMode->BufAddr = ChgMode->BufAddr + BufRst.BufSize;
            ChgMode->BufSize = ChgMode->BufSize - BufRst.BufSize;

            BufRst.BufSize = 0;
            BufRst.er_code = E_OK;
            if (ChgMode->ProcSenId[IPL_ID_2] != SENSOR_ID_NONE)
            {
                BufRst = IPL_Init_Proc_CCIR(IPL_ID_2, ChgMode, PrvMode, &IsrObjInfo, &IpcInfo, &AeObj, &AwbObj, &AfObj, FALSE);
                //IPL_BufDump(IPL_ID_2);
            }
            UsedSize += BufRst.BufSize;

            ChgMode->BufAddr = OrgAddr;
            ChgMode->BufSize = UsedSize;

            if (ChgMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
            {
                //open AE & AWB
                AeObj.ProcNum = 1;
                AwbObj.ProcNum = 1;
                AE_Open(&AeObj);
                AWB_Open(&AwbObj);

                //trigger AE & AWB
                AE_Start(IPL_UTI_CONV2_AE_ID(IPL_ID_1), TRUE);
                AWB_Start(IPL_UTI_CONV2_AWB_ID(IPL_ID_1), TRUE);
                IsrObjInfo.ProcNum = 1;
            }
        }
    }
    else
    {
        //only one sensor
    }

    IPL_CTRLRun2(ICF_FLOW_D, &IpcInfo, &IsrObjInfo, IPL_CtrlPostImage, IPL_CtrlPostImage2);
    IPC_WaitImeFMD(TRUE);

    IPL_CBMsgProc(IPL_CBMSG_PREVIEW, NULL);

    AE_WaitStable(3);
    IPL_CBMsgProc(IPL_CBMSG_PREVIEWSTABLE, NULL);
    return E_OK;
}

static ER IPL_Vid2Off(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{

    if (PrvMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
    {
        //close AWB
        AWB_Close();
        //close AE
        AE_Close();
    }

    //un initial IPL
    if (PrvMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
    {
        IPL_FreezeDisplayImage(FALSE);
    }

    if (PrvMode->ProcSenId[IPL_ID_2] != SENSOR_ID_NONE)
    {
        IPL_FreezeDisplayImage2(FALSE);
    }

    IPL_CTRLStop(IPC_STOP_ALL);
    IPL_CBMsgUnInit();

    if (PrvMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
    {
        IPL_BufUnInit(IPL_ID_1);
    }

    if (PrvMode->ProcSenId[IPL_ID_2] != SENSOR_ID_NONE)
    {
        IPL_BufUnInit(IPL_ID_2);
    }
    return E_OK;
}

static ER IPL_Prv2Off(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    if (ChgMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
    {
        //close AWB
        AWB_Close();
        //close AE
        AE_Close();
    }

    //un initial IPL
    if (PrvMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
    {
        IPL_FreezeDisplayImage(FALSE);
    }

    if (PrvMode->ProcSenId[IPL_ID_2] != SENSOR_ID_NONE)
    {
        IPL_FreezeDisplayImage2(FALSE);
    }

    IPL_CTRLStop(IPC_STOP_ALL);
    IPL_CBMsgUnInit();

    if (PrvMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
    {
        IPL_BufUnInit(IPL_ID_1);
    }

    if (PrvMode->ProcSenId[IPL_ID_2] != SENSOR_ID_NONE)
    {
        IPL_BufUnInit(IPL_ID_2);
    }
    return E_OK;
}


static ER IPL_Prv2Cap(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 *ZoomTab, ZoomTabMax;
    SENSOR_INFO Info;
    IPL_BUF_OBJ_INFO BufObj;
    IPLBUF_RST BufRst;
    IPLCTRL_INFO CtrlInfo;
    IPLBUF_ENG2DRAMINFO BufInfo;

    //IPL_AlgSetUIInfo2(IPL_ID_1, IPL_SEL_CAPSENMODE, SENSOR_MODE_4);
    IPL_MapSensorMode(ChgMode, PrvMode);
    IPL_AlgSetUIInfo2(IPL_ID_1, IPL_SEL_RAW_BITDEPTH_CAP, SEL_RAW_BITDEPTH_10);

    if (PrvMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
    {
        //pause 2A
        AE_Pause(IPL_UTI_CONV2_AE_ID(IPL_ID_1), ENABLE);
        AWB_Pause(IPL_UTI_CONV2_AWB_ID(IPL_ID_1), ENABLE);
    }

    //preAE setting
    IPL_FCB_PrepareCapAE(ChgMode->Id);

    //preflash setting
    IPL_FCB_PreFlash(ChgMode->Id);

    //stop IPL
    IPL_FreezeDisplayImage(FALSE);
    IPL_FreezeDisplayImage2(FALSE);

    IPL_CTRLStop(IPC_STOP_YCC);

    if (PrvMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
    {
        IPC_WaitVD(FALSE, IPL_ID_1);
        IPL_CTRLStop(IPC_PAUSE_SIE);
        IPC_WaitVD(FALSE, IPL_ID_1);
    }

    if (PrvMode->ProcSenId[IPL_ID_2] != SENSOR_ID_NONE)
    {
        IPC_WaitVD(FALSE, IPL_ID_2);
        IPL_CTRLStop(IPC_PAUSE_SIE2);
        IPC_WaitVD(FALSE, IPL_ID_2);
    }

    //get mode status
    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(ChgMode->Id), IPL_AlgGetUIInfo2(ChgMode->Id, IPL_SEL_CAPSENMODE), &Info);
    //IPL data initial
    ZoomTab = SenMode2Tbl(ChgMode->Id, IPL_AlgGetUIInfo2(ChgMode->Id, IPL_SEL_CAPSENMODE), &ZoomTabMax);
    IPL_DZoomSetInfor(ChgMode->Id, ZoomTab, ZoomTabMax);

    //initial capture parameter
    CtrlInfo.Info = &Info;
    CtrlInfo.ChgMode = ChgMode;
    if (ChgMode->EventId == IPL_FLOW_CCIR_GETRAW)
    {
        IPL_CtrlCapInit_CCIR(ChgMode->Id, ChgMode->EventId, &CtrlInfo);
    }
    else if (ChgMode->EventId == IPL_FLOW_DUALSENSOR_GETRAW)
    {
        IPL_CtrlCapInit_CCIR(IPL_ID_2, ChgMode->EventId, &CtrlInfo);
        IPL_CtrlCapInit(IPL_ID_1, ChgMode->EventId, &CtrlInfo);
    }
    else
    {
    IPL_CtrlCapInit(ChgMode->Id, ChgMode->EventId, &CtrlInfo);
    }

    //conver to cap buffer
    BufObj.Id = ChgMode->Id;
    BufObj.BufAddr = ChgMode->BufAddr;
    BufObj.BufSize = ChgMode->BufSize;
    IPL_BufInit(&BufObj);
    IPL_CtrlInitCapBufInfo(BufObj.Id, ChgMode, &BufInfo);
    BufRst = IPL_BufAssign(IPLBUF_MODE_CAP, &BufInfo);
    ChgMode->BufSize = BufRst.BufSize;
    return E_OK;
}

static ER IPL_Cap2Prv(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 OrgAddr;
    UINT32 UsedSize;
    IPL_ISR_OBJ_INFO IsrObjInfo = {0};
    AE_OBJ_INFO AeObj = {0};
    AWB_OBJ_INFO AwbObj = {0};
    AF_OBJ_INFO AfObj = {0};
    IPC_INFO IpcInfo = {0};
    IPLBUF_RST BufRst;

    IPL_CTRLStop(IPC_STOP_RAW);

    if (ChgMode->SenNum == 2)
    {
        if ((ChgMode->ProcSenId[IPL_ID_1] == SENSOR_ID_NONE) && (ChgMode->ProcSenId[IPL_ID_2] == SENSOR_ID_NONE))
        {
            DBG_ERR("fail unsupport condition\r\n");
        }
        else
        {
            //mipi + CCIR
            OrgAddr = ChgMode->BufAddr;
            UsedSize = 0;
            BufRst.BufSize = 0;
            BufRst.er_code = E_OK;
            if (ChgMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
            {
                //IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_4);
                IPL_MapSensorMode(ChgMode, PrvMode);
                IPL_AlgSetUIInfo(IPL_SEL_RAW_BITDEPTH_PRV, SEL_RAW_BITDEPTH_10);
                BufRst = IPL_Init_Proc(IPL_ID_1, ChgMode, PrvMode, &IsrObjInfo, &IpcInfo, &AeObj, &AwbObj, &AfObj, FALSE, TRUE);
                //IPL_BufDump(IPL_ID_1);
            }
            UsedSize += BufRst.BufSize;
            ChgMode->BufAddr = ChgMode->BufAddr + BufRst.BufSize;
            ChgMode->BufSize = ChgMode->BufSize - BufRst.BufSize;

            BufRst.BufSize = 0;
            BufRst.er_code = E_OK;
            if (ChgMode->ProcSenId[IPL_ID_2] != SENSOR_ID_NONE)
            {
                BufRst = IPL_Init_Proc_CCIR(IPL_ID_2, ChgMode, PrvMode, &IsrObjInfo, &IpcInfo, &AeObj, &AwbObj, &AfObj, FALSE);
                //IPL_BufDump(IPL_ID_2);
            }
            UsedSize += BufRst.BufSize;

            ChgMode->BufAddr = OrgAddr;
            ChgMode->BufSize = UsedSize;

            if (ChgMode->ProcSenId[IPL_ID_1] != SENSOR_ID_NONE)
            {
                //resume AE & AWB
                AE_Resume(IPL_UTI_CONV2_AE_ID(IPL_ID_1));
                AWB_Resume(IPL_UTI_CONV2_AWB_ID(IPL_ID_1));
                IsrObjInfo.ProcNum = 1;
            }
        }
    }
    else
    {
        //only one sensor
    }

    IPL_CTRLRun2(ICF_FLOW_D, &IpcInfo, &IsrObjInfo, IPL_CtrlPostImage, IPL_CtrlPostImage2);
    IPC_WaitImeFMD(TRUE);

    IPL_CBMsgProc(IPL_CBMSG_PREVIEW, NULL);

    AE_WaitStable(3);
    IPL_CBMsgProc(IPL_CBMSG_PREVIEWSTABLE, NULL);
    return E_OK;
}

static IPL_CMD_CHGMODE_FP ChgModeAct[IPL_MODE_NUM][IPL_MODE_NUM] =
{
    /*IPL_MODE_OFF, IPL_MODE_PREVIEW,   IPL_MODE_VIDEO, IPL_MODE_VIDEOREC,  IPL_MODE_CAP*/
    {NULL,          IPL_Off2Prv,        NULL,           NULL,               NULL},          /*IPL_MODE_OFF*/
    {IPL_Prv2Off,   NULL,               IPL_Prv2Vid,    NULL,               IPL_Prv2Cap},   /*IPL_MODE_PREVIEW*/
    {IPL_Vid2Off,   IPL_Vid2Prv,        NULL,           IPL_Vid2Rec,        NULL},          /*IPL_MODE_VIDEO*/
    {NULL,          NULL,               IPL_Rec2Vid,    NULL,               IPL_Rec2Cap},   /*IPL_MODE_VIDEOREC*/
    {NULL,          IPL_Cap2Prv,        NULL,           IPL_Cap2Rec,        NULL},          /*IPL_MODE_CAP*/
};

IPL_CMD_CHGMODE_FP IPL_GetModeFp(IPL_MODE CurMode, IPL_MODE NextMode)
{
    if ((CurMode >= IPL_MODE_NUM) || (NextMode >= IPL_MODE_NUM))
    {
        return NULL;
    }
    return ChgModeAct[CurMode][NextMode];

}
