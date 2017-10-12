/*
    IPL D2D Callback Function

    This file is the API of the D2D callback function.

    @file       IPL_D2DCBAR0330M_EVB_FF.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include "IPL_AR0330M_EVB_FF_Int.h"
//note: do not delete
//avoid compiler optimize "weak function"
WEAK_DUMMY_CALL(D2D, CTRL2)

static UINT32 PRE_PPROC_D2D_CB(PRE_SUBFUNC* subF, PRE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 PRE_CAP_D2D_CB(PRE_SUBFUNC* subF, PRE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

UINT32 PRE_D2D_CB(PRE_SUBFUNC* subF, PRE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = PRE_CAP_D2D_CB(subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = PRE_PPROC_D2D_CB(subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif

static UINT32 IFE2_PPROC_D2D_CB(IFE2_SUBFUNC* subF, IFE2_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{

    UINT32 Cnt = 1;
    IQS_FLOW_MSG IQSevent;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);
    switch(eventID)
    {
        case (IPL_FLOW_HDR_IFE2D2D | IPL_FLOW_Y):
            IQSevent = IQCB_PRV_IFE2_RETINEX;
            Cnt = IQS_GetIFE2RetinexTime(IQSevent);
            break;

        default:
            IQSevent = IQS_FLOW_MSG_NUM;
            DBG_ERR("event fail\r\n");
            break;
    }

    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info;
    Info.pIfe2_sub = subF;
    group.ife2Update = *Update;
    IQCB_SettingFlowFunc(IQSevent, &Info, &group);
    *Update = group.ife2Update;
    return Cnt;
}

static UINT32 IFE2_CAP_D2D_CB(IFE2_SUBFUNC* subF, IFE2_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 5;
    static UINT8 ife2LoopCnt=0;
    IQS_FLOW_MSG IQSevent;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);
    switch(eventID)
    {
        case (IPL_FLOW_IFE2REF_PRI | IPL_FLOW_Y):
            IQSevent = IQCB_D2D_IFE2_PRI_Y;
            Cnt = IQS_GetIFE2RetinexTime(IQSevent);
            if ( ife2LoopCnt <= 0 )
                ife2LoopCnt = Cnt;
            ife2LoopCnt--;
            break;

        case (IPL_FLOW_IFE2REF_QV | IPL_FLOW_Y):
            IQSevent = IQCB_D2D_IFE2_QV_Y;
            Cnt = IQS_GetIFE2RetinexTime(IQSevent);
            if ( ife2LoopCnt <= 0 )
                ife2LoopCnt = Cnt;
            ife2LoopCnt--;
            break;

        case (IPL_FLOW_IFE2REF_PRI | IPL_FLOW_U | IPL_FLOW_V):
            IQSevent = IQCB_D2D_IFE2_PRI_CC;
            Cnt = IQS_GetIFE2LcaTime(IQSevent);
            if ( ife2LoopCnt <= 0 )
                ife2LoopCnt = Cnt;
            ife2LoopCnt--;
            break;

        case (IPL_FLOW_IFE2REF_QV | IPL_FLOW_U | IPL_FLOW_V):
            IQSevent = IQCB_D2D_IFE2_QV_CC;
            Cnt = IQS_GetIFE2LcaTime(IQSevent);
            if ( ife2LoopCnt <= 0 )
                ife2LoopCnt = Cnt;
            ife2LoopCnt--;
            break;

        default:
            IQSevent = IQS_FLOW_MSG_NUM;
            DBG_ERR("event fail\r\n");
            break;
    }

    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info;
    Info.pIfe2_sub = subF;
    group.ife2Update = *Update;
    IQCB_SettingFlowFunc(IQSevent, &Info, &group);
    *Update = group.ife2Update;
    if ( IQSevent == IQCB_D2D_IFE2_QV_CC || IQSevent == IQCB_D2D_IFE2_PRI_CC )
    {
        DBG_IND("%x, %d %d\r\n", IQSevent, Cnt, ife2LoopCnt);
        if ( ife2LoopCnt!= Cnt-1 )
        {
            Info.pIfe2_sub->ife2FuncEn &= (~ChromaEn);
            DBG_IND("Close IFE2 Chroma\r\n");
        }
    }
#if IQS_DUMP_ENGREG
    *fpRegD = (fpRegDump)IQS_DumpEngRegister;
#endif
    return Cnt;
}

UINT32 IFE2_D2D_CB(IFE2_SUBFUNC* subF, IFE2_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = IFE2_CAP_D2D_CB(subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = IFE2_PPROC_D2D_CB(subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif

static UINT32 DCE_PPROC_D2D_CB(USIZE *pRef_1x,Coordinate *pRef_1xCen,DCE_SUBFUNC* subF, DCE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 DCE_CAP_D2D_CB(USIZE *pRef_1x,Coordinate *pRef_1xCen,DCE_SUBFUNC* subF, DCE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

UINT32 DCE_D2D_CB(USIZE *pRef_1x,Coordinate *pRef_1xCen,DCE_SUBFUNC* subF, DCE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = DCE_CAP_D2D_CB(pRef_1x, pRef_1xCen, subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = DCE_PPROC_D2D_CB(pRef_1x, pRef_1xCen, subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif

static UINT32 IPE_PPROC_D2D_CB(IPE_SUBFUNC* subF, IPE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 IPE_CAP_D2D_CB(IPE_SUBFUNC* subF, IPE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

UINT32 IPE_D2D_CB(IPE_SUBFUNC* subF, IPE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = IPE_CAP_D2D_CB(subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = IPE_PPROC_D2D_CB(subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif

static UINT32 IFE_PPROC_D2D_CB(IFE_SUBFUNC* subF, IFE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 IFE_CAP_D2D_CB(IFE_SUBFUNC* subF, IFE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 5;
    static UINT32 d2dCnt = 5;
    if(d2dCnt<=1)
        d2dCnt = Cnt;
    d2dCnt--;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);
    return Cnt;
}

UINT32 IFE_D2D_CB(IFE_SUBFUNC* subF, IFE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = IFE_CAP_D2D_CB(subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = IFE_PPROC_D2D_CB(subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif

static UINT32 IME_PPROC_D2D_CB(IME_SUBFUNC* subF, IME_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 IME_CAP_D2D_CB(IME_SUBFUNC* subF, IME_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;

    //IPL_FLOW_IMED2D_QV: Enable LCA
    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info;
    Info.pIme_sub = subF;
    group.imeUpdate = *Update;

    if (eventID == IPL_FLOW_IMED2D_QV)
        IQCB_SettingFlowFunc(IQCB_D2D_IME_QV, &Info, &group);
    if (eventID == IPL_FLOW_IMED2D_QVREF)
        IQCB_SettingFlowFunc(IQCB_D2D_IME_QVREF, &Info, &group);

    if ((eventID & (~IPL_FLOW_RESERVE))== IPL_FLOW_IMED2D_SCAL)
    {
        group.imeUpdate = IME_SubFEn;
        subF->imeFuncEn = 0;
    }


    *Update = group.imeUpdate;
#if IQS_DUMP_ENGREG
    *fpRegD = (fpRegDump)IQS_DumpEngRegister;
#endif
    return Cnt;
}

UINT32 IME_D2D_CB(IME_SUBFUNC* subF, IME_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = IME_CAP_D2D_CB(subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = IME_PPROC_D2D_CB(subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}
#if 0
#endif

static UINT32 RDE_PPROC_D2D_CB(RDE_SUBFUNC *subF, RDE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 RDE_CAP_D2D_CB(RDE_SUBFUNC *subF, RDE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    DBG_IND("EventID = 0x%.8x\r\n", eventID);
    switch(eventID)
    {
        case IPL_FLOW_RAWDEC:
            *Update |= RDE_INIT;
            subF->Dithering = RDE_ZERO;
            subF->OutputGain = RDE_GAIN0;
            subF->RDEFuncEn  = DEGammaEn;
            subF->GammaTbl = RawDecLut;
            break;

        default:
            DBG_ERR("event fail\r\n");
            break;
    }
    return 1;
}

UINT32 RDE_D2D_CB(RDE_SUBFUNC *subF, RDE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = RDE_CAP_D2D_CB(subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = RDE_PPROC_D2D_CB(subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif
static UINT32 PREIME_PPROC_D2D_PRECB(PRE_SUBFUNC* subF, PRE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 PREIME_CAP_D2D_PRECB(PRE_SUBFUNC* subF, PRE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    DBG_IND("EventID = 0x%.8x\r\n", eventID);
    switch(eventID)
    {
        case (IPL_FLOW_RAW2YCC_QV|IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V):
        case (IPL_FLOW_RAW2YCC_PRI|IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V):
            if (IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_CAPDF_EN) == ENABLE)
            {
                subF->Dfs.dfs.DFS_OP = A_MINUS_B;
                subF->Dfs.dfs.DFS_ThrSrc = Thr_SrcA;
                subF->Dfs.dfs.Threshold = 0xfff;
                subF->Dfs.dfs.bDpSym = 0;
                subF->Dfs.dfs.Shift = 0;
                subF->preFuncEn = DfsEn;
                *Update |= (PRE_DFS_|PRE_SubFEn);
            }
            break;

        default:
            DBG_ERR("event fail\r\n");
            break;

    }

    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info;
    Info.pPre_sub = subF;
    group.preUpdate = *Update;
    if ( (eventID & (~IPL_FLOW_RESERVE)) == IPL_FLOW_RAW2YCC_PRI )
        IQCB_SettingFlowFunc(IQCB_PREIME_D2D_PRE_PRI, &Info, &group);
    else
        IQCB_SettingFlowFunc(IQCB_PREIME_D2D_PRE_QV, &Info, &group);
    *Update = group.preUpdate;

#if IQS_DUMP_ENGREG
    *fpRegD = (fpRegDump)IQS_DumpEngRegister;
#endif

    return 1;
}

UINT32 PREIME_D2D_PRECB(PRE_SUBFUNC* subF, PRE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = PREIME_CAP_D2D_PRECB(subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = PREIME_PPROC_D2D_PRECB(subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif
static UINT32 PREIME_PPROC_D2D_IFECB(IFE_SUBFUNC* subF, IFE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}


static UINT32 PREIME_CAP_D2D_IFECB(IFE_SUBFUNC* subF, IFE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info;
    Info.pIfe_sub = subF;
    group.ifeUpdate = *Update;
    if ( (eventID & (~IPL_FLOW_RESERVE)) == IPL_FLOW_RAW2YCC_PRI )
        IQCB_SettingFlowFunc(IQCB_PREIME_D2D_IFE_PRI, &Info, &group);
    else
        IQCB_SettingFlowFunc(IQCB_PREIME_D2D_IFE_QV, &Info, &group);
    *Update = group.ifeUpdate;

    return 1;
}


UINT32 PREIME_D2D_IFECB(IFE_SUBFUNC* subF, IFE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = PREIME_CAP_D2D_IFECB(subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = PREIME_PPROC_D2D_IFECB(subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif

static UINT32 PREIME_PPROC_D2D_DCECB(USIZE *pRef_1x,Coordinate *pRef_1xCen,DCE_SUBFUNC* subF, DCE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}


static UINT32 PREIME_CAP_D2D_DCECB(USIZE *pRef_1x,Coordinate *pRef_1xCen,DCE_SUBFUNC* subF, DCE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 tmpSizeH, tmpSizeV;
    IQS_FLOW_MSG IQSevent;

    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    switch(eventID)
    {
        case (IPL_FLOW_RAW2YCC_QV|IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V):
            IPL_DzoomGetSIECropSize(IPL_ID_1, 0, &tmpSizeH, &tmpSizeV);
            pRef_1x->w = tmpSizeH;
            pRef_1x->h = tmpSizeV;
            //ethan test
            pRef_1xCen->x = pRef_1x->w/2;
            pRef_1xCen->y = pRef_1x->h/2;

            IQSevent = IQCB_PREIME_D2D_DCE_QV;
            break;

        case (IPL_FLOW_RAW2YCC_PRI|IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V):
            IPL_DzoomGetSIECropSize(IPL_ID_1, 0, &tmpSizeH, &tmpSizeV);
            pRef_1x->w = tmpSizeH;
            pRef_1x->h = tmpSizeV;
            //ethan test
            pRef_1xCen->x = pRef_1x->w/2;
            pRef_1xCen->y = pRef_1x->h/2;


            IQSevent = IQCB_PREIME_D2D_DCE_PRI;
            break;

        default:
            IQSevent = IQS_FLOW_MSG_NUM;
            DBG_ERR("event fail\r\n");
            break;
    }

    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info;
    Info.pDce_sub = subF;
    group.dceUpdate = *Update;
    IQCB_SettingFlowFunc(IQSevent, &Info, &group);
    *Update = group.dceUpdate;
    return 1;
}


UINT32 PREIME_D2D_DCECB(USIZE *pRef_1x,Coordinate *pRef_1xCen,DCE_SUBFUNC* subF, DCE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = PREIME_CAP_D2D_DCECB(pRef_1x, pRef_1xCen, subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = PREIME_PPROC_D2D_DCECB(pRef_1x, pRef_1xCen, subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}
#if 0
#endif

static UINT32 PREIME_PPROC_D2D_IPECB(IPE_SUBFUNC* subF,IPE_UPDATE *Update,UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 PREIME_CAP_D2D_IPECB(IPE_SUBFUNC* subF,IPE_UPDATE *Update,UINT32 eventID,fpRegDump *fpRegD)
{
    DBG_IND("EventID = 0x%.8x\r\n", eventID);
#if 0
    subF->IpeFuncEn = (IPE_CFAINTEEn | IPE_CSTEn | IPE_CCTRLEn);
    *Update |= IPE_SubFEn;
#endif

    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info;
    Info.pIpe_sub = subF;
    group.ipeUpdate = *Update;
    if ( (eventID & (~IPL_FLOW_RESERVE)) == IPL_FLOW_RAW2YCC_QV)
        IQCB_SettingFlowFunc(IQCB_PREIME_D2D_IPE_QV, &Info, &group);
    if ( (eventID & (~IPL_FLOW_RESERVE)) == IPL_FLOW_RAW2YCC_PRI)
        IQCB_SettingFlowFunc(IQCB_PREIME_D2D_IPE_PRI, &Info, &group);
    *Update = group.ipeUpdate;
    return 1;
}

UINT32 PREIME_D2D_IPECB(IPE_SUBFUNC* subF,IPE_UPDATE *Update,UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = PREIME_CAP_D2D_IPECB(subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = PREIME_PPROC_D2D_IPECB(subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}
#if 0
#endif

static UINT32 PREIME_PPROC_D2D_IMECB(IME_SUBFUNC* subF,IME_UPDATE *Update,UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 PREIME_CAP_D2D_IMECB(IME_SUBFUNC* subF,IME_UPDATE *Update,UINT32 eventID,fpRegDump *fpRegD)
{
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info;
    Info.pIme_sub = subF;
    group.imeUpdate = *Update;
    if ( (eventID & (~IPL_FLOW_RESERVE)) == IPL_FLOW_RAW2YCC_QV )
        IQCB_SettingFlowFunc(IQCB_PREIME_D2D_IME_QV, &Info, &group);
    if ( (eventID & (~IPL_FLOW_RESERVE)) == IPL_FLOW_RAW2YCC_PRI )
        IQCB_SettingFlowFunc(IQCB_PREIME_D2D_IME_PRI, &Info, &group);
    *Update = group.imeUpdate;
    return 1;
}

UINT32 PREIME_D2D_IMECB(IME_SUBFUNC* subF,IME_UPDATE *Update,UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = PREIME_CAP_D2D_IMECB(subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = PREIME_PPROC_D2D_IMECB(subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif
void _STRIPE_CB(USIZE *Ref_1x,Coordinate *Ref_1xCen,DCE_SUBFUNC* subF,UINT32 eventID)
{
    switch(eventID)
    {
        case IPL_FLOW_CAL_STRIPE:
            {
            UINT32 tmpSizeH, tmpSizeV;
            IQS_FLOW_MSG IQSevent;
            IPL_HAL_GROUP group = {0};
            ENG_SUBFUNC Info;

            Info.pDce_sub = subF;
            IPL_DzoomGetSIECropSize(IPL_ID_1, 0, &tmpSizeH, &tmpSizeV);
            Ref_1x->w = tmpSizeH;
            Ref_1x->h = tmpSizeV;
            Ref_1xCen->x = Ref_1x->w/2;
            Ref_1xCen->y = Ref_1x->h/2;

            IQSevent = IQCB_PREIME_D2D_DCE_QV;
            IQCB_SettingFlowFunc(IQSevent, &Info, &group);
            }
            break;

        default:
            DBG_ERR("event fail\r\n");
            break;
    }
}


