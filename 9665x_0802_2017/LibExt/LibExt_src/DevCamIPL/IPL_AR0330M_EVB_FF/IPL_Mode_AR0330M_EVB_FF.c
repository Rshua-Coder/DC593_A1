/*
    IPL mode API

    This file is IPL mode control.

    @file       IPL_ModeAR0330M.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include "IPL_AR0330M_EVB_FF_Int.h"

static void IPL_MapSensorMode(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 Video_FPS = IPL_AlgGetUIInfo(IPL_SEL_VIDEO_FPS);
    switch(ChgMode->Mode)
    {
        case IPL_MODE_VIDEOREC:
        case IPL_MODE_VIDEO:
            switch(Video_FPS)
            {
                case SEL_VIDEOFPS_30:
                    IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_3);
                    IPL_AlgSetUIInfo(IPL_SEL_CAPSENMODE, SENSOR_MODE_3);
                    break;
                case SEL_VIDEOFPS_60:
                    IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_2);
                    IPL_AlgSetUIInfo(IPL_SEL_CAPSENMODE, SENSOR_MODE_2);
                    break;
                case SEL_VIDEOFPS_120:
                    IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_5);
                    IPL_AlgSetUIInfo(IPL_SEL_CAPSENMODE, SENSOR_MODE_5);
                    break;
                default:
                    DBG_ERR("^RInvalidate video fps\r\n");
                    IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_3);
                    IPL_AlgSetUIInfo(IPL_SEL_CAPSENMODE, SENSOR_MODE_3);
                    break;
            }
            break;
        case IPL_MODE_PREVIEW:
        case IPL_MODE_CAP:
            IPL_AlgSetUIInfo(IPL_SEL_CAPSENMODE, SENSOR_MODE_4);
            IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_4);
            break;
        default:
            DBG_ERR("^Rsensor mode map fail %d\r\n", ChgMode->Mode);
            IPL_AlgSetUIInfo(IPL_SEL_CAPSENMODE, SENSOR_MODE_3);
            IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SENSOR_MODE_3);
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

        default:
            DBG_ERR("flow map fail %d\r\n", Flow);
            return 0;
    }
}

static ER IPL_Rec2Cap(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 i;
    SENSOR_INFO Info;
    IPLCTRL_INFO CtrlInfo;

    //need lock all AE or AWB ??
    for (i = 0; i < PrvMode->SenNum; i ++)
    {
        //lock 2A
        AE_Pause(IPL_UTI_CONV2_AE_ID(i), ENABLE);
        AWB_Pause(IPL_UTI_CONV2_AWB_ID(i), ENABLE);
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
    UINT32 i;

    //need lock all AE or AWB ??
    for (i = 0; i < PrvMode->SenNum; i ++)
    {
        //lock 2A
        AE_Resume(IPL_UTI_CONV2_AE_ID(i));
        AWB_Resume(IPL_UTI_CONV2_AWB_ID(i));
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
    UINT32 i, ZoomIdx[IPL_ID_MAX_NUM] = {0};
    IPL_ISR_OBJ_INFO IsrObjInfo;
    UINT32 *ZoomTab, ZoomTabMax, UsedSize = 0;
    SENSOR_INFO Info;
    IPLBUF_RST BufRst = {0};
    IPLCTRL_INFO CtrlInfo;
    IPC_INFO IpcInfo = {0};
    IPL_BUF_OBJ_INFO BufObj;
    IPLBUF_ENG2DRAMINFO BufInfo;

    //pause 2A
    for (i = 0; i < PrvMode->SenNum; i ++)
    {
        AE_Pause(IPL_UTI_CONV2_AE_ID(i), ENABLE);
        AWB_Pause(IPL_UTI_CONV2_AWB_ID(i), ENABLE);
    }

    //stop IPL
    IPL_FreezeDisplayImage(FALSE);
    IPL_CTRLStop(IPC_STOP_ALL);

    {
      extern void DispSrv_DirtySolution(void);
      DispSrv_DirtySolution();
    }

    IPL_CBMsgInit(ChgMode->CB_FP);

    UINT32 preSensorMode = IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE);
    IPL_MapSensorMode(ChgMode, PrvMode);

    //change sensor mode
    for (i = IPL_ID_1; i < PrvMode->SenNum; i ++)
    {
        if ( IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE) != preSensorMode )
            Sensor_ChgMode(IPL_UTI_CONV2_SEN_ID(i), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));
    }

    for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    {
        //get mode status
        Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(i), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &Info);

        //IPL data initial
        ZoomTab = SenMode2Tbl(IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &ZoomTabMax);

        IPL_DZoomSetInfor(i, ZoomTab, ZoomTabMax);
        ZoomIdx[i] = IPL_DzoomGetIndex(i);
        IPL_DzoomSetIndex(i, 0);

        IPL_CtrlSetInfor(i, IPLCTRL_SEN_PRVCROPRATIO, ChgMode->CropRatio);
        //decide preivew flow
        CtrlInfo.Info = &Info;
        CtrlInfo.ChgMode = ChgMode;
        IPL_CtrlPrvInit(i, &CtrlInfo);

        //allocate buffer & initial buffer size
        BufObj.Id = i;
        if (i == IPL_ID_1)
        {
            BufObj.BufAddr = ChgMode->BufAddr;
            BufObj.BufSize = ChgMode->BufSize;
            UsedSize = 0;
        }
        else
        {

            BufObj.BufAddr += BufRst.BufSize;

            if (BufObj.BufSize > BufRst.BufSize)
            {
                BufObj.BufSize -= BufRst.BufSize;
            }
            else
            {
                BufObj.BufSize = 0;
            }
        }
        IPL_BufInit(&BufObj);
        IPL_CtrlInitPrvBufInfo(BufObj.Id, ChgMode, &BufInfo);
        BufRst = IPL_BufAssign(IPL_MapICF2BufMode(IPL_CtrlGetInfor(i, IPLCTRL_PRVFLOW)), &BufInfo);
        UsedSize += BufRst.BufSize;
    }
    ChgMode->BufSize = UsedSize;

    //Load post process function, need Id ??
    IPL_CtrlInitPrvPProcInfo(IPL_ID_1, ChgMode->FuncEn);

    //reset display control
    IPL_ResetDisplayCtrlFlow();

    //initial digital zoom
    for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    {
        IPL_DzoomSetIndex(i, ZoomIdx[i]);
        IPL_CtrlPrvSetDZoomInfo(i);
    }
    IPL_CtrlInitPrvIpcInfo(&IpcInfo, &IsrObjInfo, ChgMode);
    IPL_CTRLRun(IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_PRVFLOW), &IpcInfo, &IsrObjInfo, IPL_CtrlPostImage);

    //wait IME frame(first frame ready)
    IPC_WaitImeFMD(TRUE);

    IPL_CBMsgProc(IPL_CBMSG_PREVIEW, NULL);

    //trigger AE & AWB
    for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    {
        AE_Resume(IPL_UTI_CONV2_AE_ID(i));
        AWB_Resume(IPL_UTI_CONV2_AWB_ID(i));
    }

    AE_WaitStable(3);
    IPL_CBMsgProc(IPL_CBMSG_PREVIEWSTABLE, NULL);
    return E_OK;
}

static ER IPL_Rec2Vid(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 i, ZoomIdx[IPL_ID_MAX_NUM] = {0};
    IPL_ISR_OBJ_INFO IsrObjInfo;
    UINT32 *ZoomTab, ZoomTabMax, UsedSize = 0;
    SENSOR_INFO Info;
    IPLBUF_RST BufRst = {0};
    IPLCTRL_INFO CtrlInfo;
    IPC_INFO IpcInfo = {0};
    IPL_BUF_OBJ_INFO BufObj;
    IPLBUF_ENG2DRAMINFO BufInfo;

    //pause 2A
    for (i = 0; i < PrvMode->SenNum; i ++)
    {
        AE_Pause(IPL_UTI_CONV2_AE_ID(i), ENABLE);
        AWB_Pause(IPL_UTI_CONV2_AWB_ID(i), ENABLE);
    }

    //stop IPL
    IPL_FreezeDisplayImage(FALSE);
    IPL_CTRLStop(IPC_STOP_ALL);

    {
      extern void DispSrv_DirtySolution(void);
      DispSrv_DirtySolution();
    }

    IPL_CBMsgInit(ChgMode->CB_FP);


    //decide sensor mode
    UINT32 preSensorMode = IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE);
    IPL_MapSensorMode(ChgMode, PrvMode);

    //change sensor mode
    for (i = IPL_ID_1; i < PrvMode->SenNum; i ++)
    {
        if ( IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE) != preSensorMode )
            Sensor_ChgMode(IPL_UTI_CONV2_SEN_ID(i), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));
    }

    for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    {
        //get mode status
        Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(i), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &Info);

        //IPL data initial
        ZoomTab = SenMode2Tbl(IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &ZoomTabMax);

        IPL_DZoomSetInfor(i, ZoomTab, ZoomTabMax);
        ZoomIdx[i] = IPL_DzoomGetIndex(i);
        IPL_DzoomSetIndex(i, 0);

        //decide preivew flow
        CtrlInfo.Info = &Info;
        CtrlInfo.ChgMode = ChgMode;
        IPL_CtrlPrvInit(i, &CtrlInfo);

        //allocate buffer & initial buffer size
        BufObj.Id = i;
        if (i == IPL_ID_1)
        {
            BufObj.BufAddr = ChgMode->BufAddr;
            BufObj.BufSize = ChgMode->BufSize;
            UsedSize = 0;
        }
        else
        {

            BufObj.BufAddr += BufRst.BufSize;

            if (BufObj.BufSize > BufRst.BufSize)
            {
                BufObj.BufSize -= BufRst.BufSize;
            }
            else
            {
                BufObj.BufSize = 0;
            }
        }
        IPL_BufInit(&BufObj);
        IPL_CtrlInitPrvBufInfo(BufObj.Id, ChgMode, &BufInfo);
        BufRst = IPL_BufAssign(IPL_MapICF2BufMode(IPL_CtrlGetInfor(i, IPLCTRL_PRVFLOW)), &BufInfo);
        UsedSize += BufRst.BufSize;
    }
    ChgMode->BufSize = UsedSize;

    //Load post process function, need Id ??
    IPL_CtrlInitPrvPProcInfo(IPL_ID_1, ChgMode->FuncEn);

    //reset display control
    IPL_ResetDisplayCtrlFlow();

    //initial digital zoom
    for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    {
        IPL_DzoomSetIndex(i, ZoomIdx[i]);
        IPL_CtrlPrvSetDZoomInfo(i);
    }
    IPL_CtrlInitPrvIpcInfo(&IpcInfo, &IsrObjInfo, ChgMode);
    IPL_CTRLRun(IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_PRVFLOW), &IpcInfo, &IsrObjInfo, IPL_CtrlPostImage);

    //wait IME frame(first frame ready)
    IPC_WaitImeFMD(TRUE);

    IPL_CBMsgProc(IPL_CBMSG_PREVIEW, NULL);

    //trigger AE & AWB
    for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    {
        AE_Resume(IPL_UTI_CONV2_AE_ID(i));
        AWB_Resume(IPL_UTI_CONV2_AWB_ID(i));
    }

    AE_WaitStable(3);
    IPL_CBMsgProc(IPL_CBMSG_PREVIEWSTABLE, NULL);
    return E_OK;
}

static ER IPL_Off2Prv(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 *ZoomTab, ZoomTabMax, i, UsedSize = 0;
    SENSOR_INFO Info;
    IPL_ISR_OBJ_INFO IsrObjInfo;
    IPL_BUF_OBJ_INFO BufObj;
    IPLBUF_ENG2DRAMINFO BufInfo;
    AE_OBJ_INFO AeObj = {0};
    AWB_OBJ_INFO AwbObj = {0};
    AF_OBJ_INFO AfObj = {0};
    IPLBUF_RST BufRst = {0};
    IPLCTRL_INFO CtrlInfo;
    IPC_INFO IpcInfo = {0};

    AlgSetEventFP(IPL_SEL_3DNR, IPL_FCB_Alg3DNR);
    AlgSetEventFP(IPL_SEL_GDCCOMP, IPL_FCB_AlgGDC);
    AlgSetEventFP(IPL_SEL_DISCOMP, IPL_FCB_AlgDIS);
    AlgSetEventFP(IPL_SEL_IMAGEEFFECT, IPL_FCB_AlgIE);
    AlgSetEventFP(IPL_SEL_RSC, IPL_FCB_AlgRSC);
    AlgSetEventFP(IPL_SEL_WDR, IPL_FCB_AlgWDR);

    IPL_CBMsgInit(ChgMode->CB_FP);

    //decide preview flow
    if ((ChgMode->SenNum == 0) || (ChgMode->SenNum > IPL_MAX_SEN_NUM))
    {
        DBG_ERR("SenNum fail %d\r\n", ChgMode->SenNum);
        return E_SYS;
    }
    else if (ChgMode->SenNum == 2)
    {
        //dual sensor flow
        IPL_CtrlSetInfor(IPL_ID_1, IPLCTRL_PRVFLOW, ICF_FLOW_C);
        IPL_CtrlSetInfor(IPL_ID_2, IPLCTRL_PRVFLOW, IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_PRVFLOW));

        AeObj.EventTab[IPL_UTI_CONV2_AE_ID(IPL_ID_1)].Start = AE_Init;
        AeObj.EventTab[IPL_UTI_CONV2_AE_ID(IPL_ID_1)].Process = AE_Process;
        AeObj.EventTab[IPL_UTI_CONV2_AE_ID(IPL_ID_1)].Pause = NULL;
        AeObj.EventTab[IPL_UTI_CONV2_AE_ID(IPL_ID_2)].Start = NULL;
        AeObj.EventTab[IPL_UTI_CONV2_AE_ID(IPL_ID_2)].Process = NULL;
        AeObj.EventTab[IPL_UTI_CONV2_AE_ID(IPL_ID_2)].Pause = NULL;

        AwbObj.EventTab[IPL_UTI_CONV2_AWB_ID(IPL_ID_1)].Start = AWB_Init;
        AwbObj.EventTab[IPL_UTI_CONV2_AWB_ID(IPL_ID_1)].Process = AWB_Process;
        AwbObj.EventTab[IPL_UTI_CONV2_AWB_ID(IPL_ID_1)].Pause = NULL;
        AwbObj.EventTab[IPL_UTI_CONV2_AWB_ID(IPL_ID_2)].Start = NULL;
        AwbObj.EventTab[IPL_UTI_CONV2_AWB_ID(IPL_ID_2)].Process = NULL;
        AwbObj.EventTab[IPL_UTI_CONV2_AWB_ID(IPL_ID_2)].Pause = NULL;

        AfObj.EventTab[IPL_UTI_CONV2_AF_ID(IPL_ID_1)].Start = AF_Init;
        AfObj.EventTab[IPL_UTI_CONV2_AF_ID(IPL_ID_1)].Process = AF_Process;
        AfObj.EventTab[IPL_UTI_CONV2_AF_ID(IPL_ID_1)].Pause = NULL;
        AfObj.EventTab[IPL_UTI_CONV2_AF_ID(IPL_ID_2)].Start = NULL;
        AfObj.EventTab[IPL_UTI_CONV2_AF_ID(IPL_ID_2)].Process = NULL;
        AfObj.EventTab[IPL_UTI_CONV2_AF_ID(IPL_ID_2)].Pause = NULL;
    }
    else
    {
        //single sensor flow
        IPL_CtrlSetInfor(IPL_ID_1, IPLCTRL_PRVFLOW, ICF_FLOW_C);

        AeObj.EventTab[IPL_UTI_CONV2_AE_ID(IPL_ID_1)].Start = AE_Init;
        AeObj.EventTab[IPL_UTI_CONV2_AE_ID(IPL_ID_1)].Process = AE_Process;
        AeObj.EventTab[IPL_UTI_CONV2_AE_ID(IPL_ID_1)].Pause = NULL;

        AwbObj.EventTab[IPL_UTI_CONV2_AWB_ID(IPL_ID_1)].Start = AWB_Init;
        AwbObj.EventTab[IPL_UTI_CONV2_AWB_ID(IPL_ID_1)].Process = AWB_Process;
        AwbObj.EventTab[IPL_UTI_CONV2_AWB_ID(IPL_ID_1)].Pause = NULL;

        AfObj.EventTab[IPL_UTI_CONV2_AF_ID(IPL_ID_1)].Start = NULL;
        AfObj.EventTab[IPL_UTI_CONV2_AF_ID(IPL_ID_1)].Process = NULL;
        AfObj.EventTab[IPL_UTI_CONV2_AF_ID(IPL_ID_1)].Pause = NULL;
    }

    IPL_MapSensorMode(ChgMode, PrvMode);

    //initial sensor mode
    for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    {
        Sensor_ChgMode(IPL_UTI_CONV2_SEN_ID(i), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));
    }

    //open AE & AWB
    AeObj.ProcNum = ChgMode->SenNum;
    AwbObj.ProcNum = ChgMode->SenNum;
    AfObj.ProcNum = ChgMode->SenNum;
    AE_Open(&AeObj);
    AWB_Open(&AwbObj);
    AF_Open(&AfObj);

    for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    {
        //get mode status
        Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(i), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &Info);

        //IPL data initial
        ZoomTab = SenMode2Tbl(IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &ZoomTabMax);

        IPL_DZoomSetInfor(i, ZoomTab, ZoomTabMax);
        IPL_DzoomSetIndex(i, 0);

        IPL_CtrlSetInfor(i, IPLCTRL_SEN_PRVCROPRATIO, ChgMode->CropRatio);
        //decide preivew flow
        CtrlInfo.Info = &Info;
        CtrlInfo.ChgMode = ChgMode;
        IPL_CtrlPrvInit(i, &CtrlInfo);

        //allocate buffer & initial buffer size
        BufObj.Id = i;
        if (i == IPL_ID_1)
        {
            BufObj.BufAddr = ChgMode->BufAddr;
            BufObj.BufSize = ChgMode->BufSize;
            UsedSize = 0;
        }
        else
        {

            BufObj.BufAddr += BufRst.BufSize;

            if (BufObj.BufSize > BufRst.BufSize)
            {
                BufObj.BufSize -= BufRst.BufSize;
            }
            else
            {
                BufObj.BufSize = 0;
            }
        }
        IPL_BufInit(&BufObj);
        IPL_CtrlInitPrvBufInfo(BufObj.Id, ChgMode, &BufInfo);
        BufRst = IPL_BufAssign(IPL_MapICF2BufMode(IPL_CtrlGetInfor(i, IPLCTRL_PRVFLOW)), &BufInfo);
        UsedSize += BufRst.BufSize;
    }
    ChgMode->BufSize = UsedSize;

    //Load post process function, need Id ??
    IPL_CtrlInitPrvPProcInfo(IPL_ID_1, ChgMode->FuncEn);

    //reset display control
    IPL_ResetDisplayCtrlFlow();

    //initial digital zoom
    for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    {
        IPL_CtrlPrvSetDZoomInfo(i);
    }

    IPL_CtrlInitPrvIpcInfo(&IpcInfo, &IsrObjInfo, ChgMode);

    IPL_CTRLRun(IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_PRVFLOW), &IpcInfo, &IsrObjInfo, IPL_CtrlPostImage);

    //wait IME frame(first frame ready)
    IPC_WaitImeFMD(TRUE);

    IPL_CBMsgProc(IPL_CBMSG_PREVIEW, NULL);

    //trigger AE & AWB
    for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    {
        AE_Start(IPL_UTI_CONV2_AE_ID(i), TRUE);
        AWB_Start(IPL_UTI_CONV2_AWB_ID(i), TRUE);
    }

    AE_WaitStable(3);
    IPL_CBMsgProc(IPL_CBMSG_PREVIEWSTABLE, NULL);

    return E_OK;
}

static ER IPL_Vid2Off(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    //close AWB
    AWB_Close();

    //close AE
    AE_Close();

    //un initial IPL
    IPL_FreezeDisplayImage(FALSE);
    IPL_CTRLStop(IPC_STOP_ALL);

    //uninitial interface/CB module
    IPL_CBMsgUnInit();

    if ((PrvMode->SenNum == 0) || (PrvMode->SenNum > IPL_MAX_SEN_NUM))
    {
        DBG_ERR("SenNum fail %d\r\n", PrvMode->SenNum);
    }
    else if (PrvMode->SenNum == 2)
    {
        IPL_BufUnInit(IPL_ID_1);
        IPL_BufUnInit(IPL_ID_2);
    }
    else
    {
        IPL_BufUnInit(IPL_ID_1);
    }
    return E_OK;
}

static ER IPL_Prv2Off(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    //close AWB
    AWB_Close();

    //close AE
    AE_Close();

    //close AF
    AF_Close();

    //un initial IPL
    IPL_FreezeDisplayImage(FALSE);
    IPL_CTRLStop(IPC_STOP_ALL);

    //uninitial interface/CB module
    IPL_CBMsgUnInit();

    if ((PrvMode->SenNum == 0) || (PrvMode->SenNum > IPL_MAX_SEN_NUM))
    {
        DBG_ERR("SenNum fail %d\r\n", PrvMode->SenNum);
    }
    else if (PrvMode->SenNum == 2)
    {
        IPL_BufUnInit(IPL_ID_1);
        IPL_BufUnInit(IPL_ID_2);
    }
    else
    {
        IPL_BufUnInit(IPL_ID_1);
    }
    return E_OK;
}


static ER IPL_Prv2Cap(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 *ZoomTab, ZoomTabMax, i;
    SENSOR_INFO Info;
    IPL_BUF_OBJ_INFO BufObj;
    IPLBUF_RST BufRst;
    IPLCTRL_INFO CtrlInfo;
    IPLBUF_ENG2DRAMINFO BufInfo;


    //need lock all AE or AWB ??
    for (i = 0; i < PrvMode->SenNum; i ++)
    {
        //lock 2A
        AE_Pause(IPL_UTI_CONV2_AE_ID(i), ENABLE);
        AWB_Pause(IPL_UTI_CONV2_AWB_ID(i), ENABLE);
    }

    //decide sensor mode
    IPL_MapSensorMode(ChgMode, PrvMode);

    //preAE setting
    IPL_FCB_PrepareCapAE(ChgMode->Id);

    //preflash setting
    IPL_FCB_PreFlash(ChgMode->Id);

    //stop IPL
    IPL_FreezeDisplayImage(FALSE);
    IPL_CTRLStop(IPC_STOP_YCC);
    IPC_WaitVD(FALSE,ChgMode->Id);
    IPL_CTRLStop(IPC_PAUSE_SIE);
    IPC_WaitVD(FALSE,ChgMode->Id);

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(ChgMode->Id), IPL_AlgGetUIInfo(IPL_SEL_CAPSENMODE), &Info);
    ZoomTab = SenMode2Tbl(IPL_AlgGetUIInfo(IPL_SEL_CAPSENMODE), &ZoomTabMax);
    IPL_DZoomSetInfor(ChgMode->Id, ZoomTab, ZoomTabMax);

    //initial capture parameter
    CtrlInfo.Info = &Info;
    CtrlInfo.ChgMode = ChgMode;
    IPL_CtrlCapInit(ChgMode->Id, ChgMode->EventId, &CtrlInfo);

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
    UINT32 *ZoomTab, ZoomTabMax, i, UsedSize = 0, ZoomIdx[IPL_ID_MAX_NUM] = {0};
    SENSOR_INFO Info;
    IPL_BUF_OBJ_INFO BufObj;
    IPLBUF_RST BufRst = {0};
    IPLCTRL_INFO CtrlInfo;
    IPLBUF_ENG2DRAMINFO BufInfo;
    IPL_ISR_OBJ_INFO IsrObjInfo;
    IPC_INFO IpcInfo = {0};
    IPL_IME_PATH_INFO PathInfo1[IPL_ID_MAX_NUM];
    IPL_IME_PATH_INFO PathInfo2[IPL_ID_MAX_NUM];
    IPL_IME_PATH_INFO PathInfo3[IPL_ID_MAX_NUM];

    IPL_CTRLStop(IPC_STOP_RAW);

    //initial sensor mode
    for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    {
        Sensor_ChgMode(IPL_UTI_CONV2_SEN_ID(i), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));
    }

    for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    {
        IPL_GetImePathORGInfo(ChgMode->Id, IPL_IME_PATH1, &PathInfo1[i]);
        IPL_GetImePathORGInfo(ChgMode->Id, IPL_IME_PATH2, &PathInfo2[i]);
        IPL_GetImePathORGInfo(ChgMode->Id, IPL_IME_PATH3, &PathInfo3[i]);

        //get mode status
        Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(i), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &Info);

        //IPL data initial
        ZoomTab = SenMode2Tbl(IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &ZoomTabMax);

        IPL_DZoomSetInfor(i, ZoomTab, ZoomTabMax);
        ZoomIdx[i] = IPL_DzoomGetIndex(i);
        IPL_DzoomSetIndex(i, 0);

        //decide preivew flow
        CtrlInfo.Info = &Info;
        CtrlInfo.ChgMode = ChgMode;
        IPL_CtrlPrvInit(i, &CtrlInfo);

        //allocate buffer & initial buffer size
        BufObj.Id = i;
        if (i == IPL_ID_1)
        {
            BufObj.BufAddr = ChgMode->BufAddr;
            BufObj.BufSize = ChgMode->BufSize;
            UsedSize = 0;
        }
        else
        {

            BufObj.BufAddr += BufRst.BufSize;

            if (BufObj.BufSize > BufRst.BufSize)
            {
                BufObj.BufSize -= BufRst.BufSize;
            }
            else
            {
                BufObj.BufSize = 0;
            }
        }
        IPL_BufInit(&BufObj);
        IPL_CtrlInitPrvBufInfo(BufObj.Id, ChgMode, &BufInfo);
        BufRst = IPL_BufAssign(IPL_MapICF2BufMode(IPL_CtrlGetInfor(i, IPLCTRL_PRVFLOW)), &BufInfo);
        UsedSize += BufRst.BufSize;
    }

    if (ChgMode->BufSize != UsedSize)
    {
        DBG_ERR("buffer mismatch(0x%.8x, 0x%.8x)\r\n", UsedSize, ChgMode->BufSize);
    }

    //initial digital zoom
    for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    {
        IPL_DzoomSetIndex(i, ZoomIdx[i]);
        IPL_CtrlPrvSetDZoomInfo(i);
        IPL_SetImePathInfo(ChgMode->Id, IPL_IME_PATH1, &PathInfo1[i]);
        IPL_SetImePathInfo(ChgMode->Id, IPL_IME_PATH2, &PathInfo2[i]);
        IPL_SetImePathInfo(ChgMode->Id, IPL_IME_PATH3, &PathInfo3[i]);
    }

    //reset display control
    IPL_ResetDisplayCtrlFlow();

    IPL_CtrlInitPrvIpcInfo(&IpcInfo, &IsrObjInfo, ChgMode);
    IPL_CTRLRun(IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_PRVFLOW), &IpcInfo, &IsrObjInfo, IPL_CtrlPostImage);

    //wait IME frame(first frame ready)
    IPC_WaitImeFMD(TRUE);

    //trigger AE & AWB
    for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    {
        AE_Resume(IPL_UTI_CONV2_AE_ID(i));
        AWB_Resume(IPL_UTI_CONV2_AWB_ID(i));
    }
    return E_OK;
}

static ER IPL_Cap2Off(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    DBG_IND("do nothing\r\n");
    return E_OK;
}

static ER IPL_Off2Cap(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    //need assigned raw 0 & raw 2 data information

    IPL_CtrlSetInfor(ChgMode->Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_SIM);
    ChgMode->BufSize = 0;
    return E_OK;
}


static IPL_CMD_CHGMODE_FP ChgModeAct[IPL_MODE_NUM][IPL_MODE_NUM] =
{
    /*IPL_MODE_OFF, IPL_MODE_PREVIEW,   IPL_MODE_VIDEO, IPL_MODE_VIDEOREC,  IPL_MODE_CAP*/
    {NULL,          IPL_Off2Prv,        NULL,           NULL,               IPL_Off2Cap},   /*IPL_MODE_OFF*/
    {IPL_Prv2Off,   NULL,               IPL_Prv2Vid,    NULL,               IPL_Prv2Cap},   /*IPL_MODE_PREVIEW*/
    {IPL_Vid2Off,   IPL_Vid2Prv,        NULL,           IPL_Vid2Rec,        NULL},          /*IPL_MODE_VIDEO*/
    {NULL,          NULL,               IPL_Rec2Vid,    NULL,               IPL_Rec2Cap},   /*IPL_MODE_VIDEOREC*/
    {IPL_Cap2Off,   IPL_Cap2Prv,        NULL,           IPL_Cap2Rec,        NULL},          /*IPL_MODE_CAP*/
};

IPL_CMD_CHGMODE_FP IPL_GetModeFp(IPL_MODE CurMode, IPL_MODE NextMode)
{
    if ((CurMode >= IPL_MODE_NUM) || (NextMode >= IPL_MODE_NUM))
    {
        return NULL;
    }
    return ChgModeAct[CurMode][NextMode];

}
