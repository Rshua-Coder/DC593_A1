/**
    Auto Exposure flow.

    ae flow.

    @file       ae_sample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <stdio.h>

#include "type.h"
#include "IPL_AlgInfor.h"
#include "Ae_alg.h"
#include "Ae_OV8858_EVB_FF_int.h"
#include "Ae_Api.h"
#include "Ae_Task.h"
#include "Awb_Api.h"
#include "sie_lib.h"
#include "IPL_Cmd.h"
#include "IQS_SettingFlow.h"
#include "aeafd_alg.h"
#include "grph.h"
#include "ipl_ctrl.h"
#include "Lens.h"
#include "ImgStatLib.h"
#include "IQS_Utility.h"

#define __MODULE__ AE
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

extern UINT32 IQS_AddGammaTab(UINT32 Data12Bit);
extern UINT32 IQS_InvGammaTab(UINT32 Data12Bit);
extern UINT32 IQS_SetIFEGain(IPL_PROC_ID Id, IFE_SUBFUNC *ptr);
UINT32 AEUartMsg = DISABLE;

static UINT8 (*AeWin)[AE_WIN_X] = 0;
static UINT32 AeFlag = 0;
static AE_INFO2 AeInfo = {0};
static AE_EVENT_TAB AEEventTAB = {0};
static UINT32 AeHistBuf[AE_HIST_NUM];
static UINT32 AeHistBufY[AE_HIST_NUM];
static AEALG_ARG PrvOldAeArg, PrvNewAeArg[3], CapNewAeArg;
static UINT32 AECnt = 0;
static UINT32 AEStep = 0;
static GAIN_SETTING gGain[6];
static EXPOSURE_SETTING gExpTSetting[6];

#if (IFE_GAIN_ON == ENABLE)
volatile UINT32 IFEGainCntF = 0;
volatile UINT32 IFEGainCntR = 0;
UINT32 IFEGainCntR1 = 0;
volatile UINT32 IFEGainEn = DISABLE;
volatile IFE_SUBFUNC IFEGain[6] = {0};
#endif

static AE_BLK_INFO AeBlkInfo = {0};

#if 0
static UINT32 GAcc[AE_WIN_X * AE_WIN_Y] = {0};
#endif
/* -3, -2.7, -2.5, -2.3, -2, -1.7, -1.5, -1.3, -1, -0.7, -0.5, -0.3, 0, 0.3, 0.5, 0.7, 1, 1.3, 1.5, 1.7, 2.0, 2.3, 2.5, 2.7, 3  */
static UINT32 EVTable[SEL_AEEV_MAX_CNT] =
{  13, 16, 18, 20, 25, 31, 35, 40, 50, 63, 71, 79, 100, 126, 141, 159, 200, 252, 283, 317, 400, 504, 566, 635, 800};

static AEALG_EVENT_TAB AeAlgEventTab =
{
    GetOverExpoRatio,
    GetFnoRatio,
    NULL
};


UINT32 AEAFD_START = FALSE;
UINT32 AEAFD_CurrentFlickerType = AE_FLICKER_60HZ;
UINT16 AfdBuffer[CA_WIN_X*CA_WIN_Y*AEAFD_DETECT_CYCLE]; //32*32*6
UINT16 Racc[CA_WIN_X*CA_WIN_Y],Gacc[CA_WIN_X*CA_WIN_Y],Bacc[CA_WIN_X*CA_WIN_Y];
UINT32 Blk_Gacc[CA_WIN_X*CA_WIN_Y], Blk_Yacc[CA_WIN_X*CA_WIN_Y];
UINT32 AEB_Ratio = 100;

#if 0
static void AE_GetCASetInfo(UINT32 *WinXNum, UINT32 *WinYNum, UINT32 *SieCropSizeH, UINT32 *SieCropSizeV)
{
    IPL_CA_SET_INFOR CAInfo;

    CAInfo.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_CA_INFOR, (void *)&CAInfo);
    *WinXNum = CAInfo.WinXNum;
    *WinYNum = CAInfo.WinYNum;
    *SieCropSizeH = CAInfo.SieCropSizeH;
    *SieCropSizeV = CAInfo.SieCropSizeV;
}
#endif

void AE_LoadEventTAB(AE_EVENT_TAB *EventTab)
{
    AEEventTAB = *EventTab;
}

#if 0
//MovieStamp
static void AE_SetMovieLog(char* Buf, UINT32 BufLen)
{
    UINT32 idx = 0, Flag = 0;

    if (AEStep == 0)
    {
        idx = 2;
    }
    else if (AEStep == 1)
    {
        idx = 0;
    }
    else if (AEStep == 2)
    {
        idx = 1;
    }

    if (IPL_AlgGetUIInfo(IPL_SEL_IMAGEEFFECT) == SEL_IMAGEEFFECT_WDR)
    {
        Flag = 1;
    }
    sprintf(Buf, "%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\0",
                                                                    AeInfo.PrvAvgLum.y,
                                                                    AeInfo.CurExpectLum.y,
                                                                    AeInfo.CurExpectLum.raw,
                                                                    AeInfo.ExtSet.AdjHistoInfo.Lum.raw,
                                                                    AeInfo.ExtSet.DyLum.raw,
                                                                    AeInfo.ExtSet.OverExp.CurTotalStep,
                                                                    AeInfo.ExtSet.OverExp.CurCnt,
                                                                    AeInfo.PrvAvgEV.LV,
                                                                    AeInfo.PrvParam[idx].EVValue,
                                                                    PrvNewAeArg[idx].ExpoTime,
                                                                    PrvNewAeArg[idx].ISOGain,
                                                                    idx,
                                                                    Flag);
}
#endif

void AE_Init (UINT32 CurrentStatus)
{
    SENSOR_MODE Mode;
    EXPOSURE_SETTING ExpTSetting;
    GAIN_SETTING Gain;
    UINT32 Ratio;
    static BOOL Init_Flag = FALSE;

    //initial ae algorithm
    if (FALSE == Init_Flag)
    {
        Init_Flag = TRUE;
        AE_getInfo(&AeInfo);

        //get ae table
        AE_getPrvTab(AE_getIsoType(IPL_AlgGetUIInfo(IPL_SEL_ISOINDEX)), AE_getFlickerType(IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY)), &AeInfo);

        //get & save iso, exposure time, iris
        AEAlg_Allot(AeInfo.PrvAvgEV.EVValue, AeInfo.PrvCurve, AeInfo.EventTab.GetFnoRatio, AeInfo.EventTab.GetCurveOfsRatio, &PrvNewAeArg[0]);
    }
    else
    {
        //get ae table
        AE_getPrvTab(AE_getIsoType(IPL_AlgGetUIInfo(IPL_SEL_ISOINDEX)), AE_getFlickerType(IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY)), &AeInfo);

        //get & save iso, exposure time, iris
        AEAlg_Allot(AeInfo.PrvParam[0].EVValue, AeInfo.PrvCurve, AeInfo.EventTab.GetFnoRatio, AeInfo.EventTab.GetCurveOfsRatio, &PrvNewAeArg[0]);
    }

    AE_copyArg(&PrvNewAeArg[0], &PrvOldAeArg);
    AE_setFlag((AE_FLAG_SET_IRIS|AE_FLAG_SET_GAIN|AE_FLAG_SET_EXPT));

    Sensor_GetCurMode(SENSOR_ID_1, &Mode);
    Ratio = Sensor_GetExpoSetting(SENSOR_ID_1, PrvNewAeArg[0].ExpoTime, Mode, &ExpTSetting);
    Sensor_GetGainSetting(SENSOR_ID_1, (PrvNewAeArg[0].ISOGain * Ratio / 100), &Gain);

    AE_setExpT(&ExpTSetting);
    AE_setGain(&Gain);
    //AE_setIris(PrvNewAeArg[0].Iris);

    AECnt = 0;
    AEStep = 0;
    memset(gGain, 0xff, sizeof(gGain));
    memset(gExpTSetting, 0xff, sizeof(gExpTSetting));

#if (IFE_GAIN_ON == ENABLE)
    IFEGainCntF = 0;
    IFEGainCntR = 0;
    IFEGainCntR1 = 0;
    IFEGainEn = DISABLE;
#endif
    //IQS_SetMovieLogFP(AE_SetMovieLog);
}

#if (IFE_GAIN_ON == ENABLE)
static IFE_SUBFUNC* AE_GetIFEGain(void)
{
    IFE_SUBFUNC* Addr = (IFE_SUBFUNC*) &IFEGain[IFEGainCntR];
    //DBG_DUMP("GET:%d\r\n", IFEGainCntR);
    IFEGainCntR += 1;
    if (IFEGainCntR >= 6)
    {
        IFEGainCntR = 0;
    }
    return Addr;
}

void AE_Suspend (UINT32 CurrentStatus)
{
    if (CurrentStatus == AE_TASK_STATUS_PROC)
    {
        IFEGainEn = DISABLE;
    }
}
#else
void AE_Suspend (UINT32 CurrentStatus)
{
}
#endif


extern UINT32 uiISOGain, uiExpoTime;
void AE_Process (UINT32 CurrentStatus)
{
    UINT32 Ratio, i = 0, PredictPoint = 3;
    AE_RESET AeRst = AE_RESET_FALSE;
    SENSOR_MODE Mode;
    static UINT32 PrvMode = IPL_MODE_OFF;
    IPL_MODE_INFOR Info;
    static UINT32 InStrPos;
    static BOOL Switch_Flag = TRUE;

    // switch movie/photo ae
    Info.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);

    if (PrvMode != Info.Mode)
    {
        if ((Info.Mode == IPL_MODE_PREVIEW)||(Info.Mode == IPL_MODE_CAP))
        {
            AeBlkInfo.BLKLum_En = ENABLE;
            AeInfo.ExtSet.AdjHistoEn = DISABLE;
            AeInfo.ExtSet.PrvLumRatioTab = ExptY_Photo_Prv;
            AeInfo.ExtSet.OverExp.AdjLumTab = ExptY_Photo_OverExpoAdj;

            AeInfo.PrvExpectLum.raw = AE_PHOTO_EXPECT_Y;
            AeInfo.PrvExpectLum.y = IQS_AddGammaTab(AeInfo.PrvExpectLum.raw);
        }
        else if((Info.Mode == IPL_MODE_VIDEO)||(Info.Mode == IPL_MODE_VIDEOREC))
        {
            AeBlkInfo.BLKLum_En = DISABLE;
            AeInfo.ExtSet.AdjHistoEn = ENABLE;
            AeInfo.ExtSet.PrvLumRatioTab = ExptY_Mov_Prv;
            AeInfo.ExtSet.OverExp.AdjLumTab = ExptY_Mov_OverExpoAdj;

            AeInfo.PrvExpectLum.raw = AE_MOV_EXPECT_Y;
            AeInfo.PrvExpectLum.y = IQS_AddGammaTab(AeInfo.PrvExpectLum.raw);
        }
    }
    PrvMode = Info.Mode;

    AECnt ++;

    if (AECnt < 5)
    {
        return;
    }

    if ( uiISOGain!=0 )
    {
        PrvNewAeArg[0].ExpoTime = uiExpoTime;
        PrvNewAeArg[0].ISOGain = uiISOGain;

        Sensor_GetCurMode(SENSOR_ID_1, &Mode);
        Ratio = Sensor_GetExpoSetting(SENSOR_ID_1, uiExpoTime, Mode, &gExpTSetting[i]);
        Sensor_GetGainSetting(SENSOR_ID_1, uiISOGain, &gGain[i]);
        AE_setFlag(AE_FLAG_SET_GAIN|AE_FLAG_SET_EXPT);
        AE_setExpT(&gExpTSetting[0]);
        AE_setGain(&gGain[0]);
        IQS_SettingFlowFunc(IQS_AE_END);

        return ;
    }

    if (AEStep == 0)
    {
        AEStep = 1;

        // switch start
        if (AECnt%2 == 0)
        {
            Switch_Flag = TRUE;
            InStrPos = 0;
        }
        else
        {
            Switch_Flag = FALSE;
            InStrPos = 3;
        }

#if (IFE_GAIN_ON == ENABLE)
        if (IFEGainEn == ENABLE)
        {
            IQS_SetIFEGain(IPL_ID_1, AE_GetIFEGain());
        }
#endif
        //set ae window
        AeRst = AE_setWin(AE_getWinType(IPL_AlgGetUIInfo(IPL_SEL_AEMODE)));
        //get preview ae curve
        AeRst = AE_getPrvTab(AE_getIsoType(IPL_AlgGetUIInfo(IPL_SEL_ISOINDEX)), AE_getFlickerType(IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY)), &AeInfo);
        AeInfo.Reset = AeRst;

        //get capture ae curve
        AE_getCapTab(AE_getIsoType(IPL_AlgGetUIInfo(IPL_SEL_ISOINDEX)), AE_getFlickerType(IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY)), AE_getSceneType(IPL_AlgGetUIInfo(IPL_SEL_SCENEMODE)), &AeInfo);

        // get statistic informaiton
        AE_getStatisticInfo(&AeInfo);

        //comp ev
        AeInfo.EVCompRatio = EVTable[IPL_AlgGetUIInfo(IPL_SEL_AEEV)];

        //calculate ae
        AEAlg2_Converge(&AeInfo, PredictPoint, &PrvNewAeArg[0], &CapNewAeArg);

        Sensor_GetCurMode(SENSOR_ID_1, &Mode);

        for (i = 0; i < PredictPoint; i ++)
        {
            Ratio = Sensor_GetExpoSetting(SENSOR_ID_1, PrvNewAeArg[i].ExpoTime, Mode, &gExpTSetting[InStrPos+i]);
            PrvNewAeArg[i].ISOGain = (PrvNewAeArg[i].ISOGain * Ratio / 100);
#if (IFE_GAIN_ON == ENABLE)
            //DBG_DUMP("SET:%d\r\n", IFEGainCntF);
            if (PrvNewAeArg[i].ISOGain >= IFE_GAIN_MAXISO)
            {
                IFEGain[IFEGainCntF].CGain.R = (PrvNewAeArg[i].ISOGain * 256) / IFE_GAIN_MAXISO;
                IFEGain[IFEGainCntF].CGain.Gb = (PrvNewAeArg[i].ISOGain * 256) / IFE_GAIN_MAXISO;
                IFEGain[IFEGainCntF].CGain.Gr = (PrvNewAeArg[i].ISOGain * 256) / IFE_GAIN_MAXISO;
                IFEGain[IFEGainCntF].CGain.B = (PrvNewAeArg[i].ISOGain * 256) / IFE_GAIN_MAXISO;
                Sensor_GetGainSetting(SENSOR_ID_1, IFE_GAIN_MAXISO, &gGain[(InStrPos+i+GainExpTShiftNum)%6]);
            }
            else
            {
                IFEGain[IFEGainCntF].CGain.R = 256;
                IFEGain[IFEGainCntF].CGain.Gb = 256;
                IFEGain[IFEGainCntF].CGain.Gr = 256;
                IFEGain[IFEGainCntF].CGain.B = 256;
                Sensor_GetGainSetting(SENSOR_ID_1, PrvNewAeArg[i].ISOGain, &gGain[(InStrPos+i+GainExpTShiftNum)%6]);
            }

            IFEGain[IFEGainCntF].CGain.cfa = IPL_RAW_PIX_R;
            IFEGain[IFEGainCntF].CGain.IfeGainSel = _2_8;
            if (IFEGain[IFEGainCntF].CGain.R >= 1024)
            {
                IFEGain[IFEGainCntF].CGain.IfeGainSel = _3_7;
            }
            IFEGainCntF += 1;

            if (IFEGainCntF >= 6)
            {
                IFEGainCntF = 0;
            }
#else
            Sensor_GetGainSetting(SENSOR_ID_1, PrvNewAeArg[i].ISOGain, &gGain[(InStrPos+i+GainExpTShiftNum)%6]);
#endif
        }
        AE_setFlag(AE_FLAG_SET_GAIN|AE_FLAG_SET_EXPT);
        if (Switch_Flag == TRUE)
        {
            AE_setExpT(&gExpTSetting[0]);
            AE_setGain(&gGain[0]);
        }
        else
        {
            AE_setExpT(&gExpTSetting[3]);
            AE_setGain(&gGain[3]);
        }
    }
    else if (AEStep == 1)
    {

        AEStep = 2;
#if (IFE_GAIN_ON == ENABLE)
        if (IFEGainEn == ENABLE)
        {
            IQS_SetIFEGain(IPL_ID_1, AE_GetIFEGain());
        }
#endif
        // get statistic informaiton
        AE_getStatisticInfo(&AeInfo);

        //calculate ae
        AE_setFlag(AE_FLAG_SET_GAIN|AE_FLAG_SET_EXPT);
        if (Switch_Flag == TRUE)
        {
            AE_setExpT(&gExpTSetting[1]);
            AE_setGain(&gGain[1]);
        }
        else
        {
            AE_setExpT(&gExpTSetting[4]);
            AE_setGain(&gGain[4]);
        }
    }
    else if (AEStep == 2)
    {
        AEStep = 0;

#if (IFE_GAIN_ON == ENABLE)
        if (IFEGainEn == ENABLE)
        {
            IQS_SetIFEGain(IPL_ID_1, AE_GetIFEGain());
        }
        IFEGainEn = ENABLE;
#endif
        // get statistic informaiton
        AE_getStatisticInfo(&AeInfo);


        //calculate ae
        AE_setFlag(AE_FLAG_SET_GAIN|AE_FLAG_SET_EXPT);
        if (Switch_Flag == TRUE)
        {
            AE_setExpT(&gExpTSetting[2]);
            AE_setGain(&gGain[2]);
        }
        else
        {
            AE_setExpT(&gExpTSetting[5]);
            AE_setGain(&gGain[5]);
        }
    }
    IQS_SettingFlowFunc(IQS_AE_END);

    if (AEUartMsg == ENABLE)
    {
        if (AEStep == 0)
        {
            DBG_DUMP("%3d %3d %3d %8d %8d %3d %3d 2\r\n", AeInfo.PrvAvgLum.y, AeInfo.CurExpectLum.raw, AeInfo.CurLum.y, AeInfo.PrvParam[2].EVValue, PrvNewAeArg[2].ExpoTime, PrvNewAeArg[2].ISOGain, AeInfo.Counter);
        }
        else if (AEStep == 1)
        {
            DBG_DUMP("%3d %3d %3d %8d %8d %3d %3d 0\r\n", AeInfo.PrvAvgLum.y, AeInfo.CurExpectLum.raw, AeInfo.CurLum.y, AeInfo.PrvParam[0].EVValue, PrvNewAeArg[0].ExpoTime, PrvNewAeArg[0].ISOGain, AeInfo.Counter);
        }
        else if (AEStep == 2)
        {
            DBG_DUMP("%3d %3d %3d %8d %8d %3d %3d 1\r\n", AeInfo.PrvAvgLum.y, AeInfo.CurExpectLum.raw, AeInfo.CurLum.y, AeInfo.PrvParam[1].EVValue, PrvNewAeArg[1].ExpoTime, PrvNewAeArg[1].ISOGain, AeInfo.Counter);
        }
    }
}

void AE_GetPrvAEArg(UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris)
{
    *ExpT = PrvNewAeArg[0].ExpoTime;
    *ISO = PrvNewAeArg[0].ISOGain;
    *Iris = PrvNewAeArg[0].Iris;
}

void AE_GetCapAEArg(UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris)
{
    *ExpT = CapNewAeArg.ExpoTime;
    *ISO = CapNewAeArg.ISOGain;
    *Iris = CapNewAeArg.Iris;
}

void AE_WaitStable(UINT32 count)
{

}

UINT32 AE_WriteDebugInfo(UINT32 Addr)
{
    UINT32 i,j;
    Addr += sprintf((char *)Addr, "\r\n\r\n\r\nAE Info------------------------------------------------------------------------------------------------------------------------\r\n");
    Addr += sprintf((char *)Addr, "ExpectY = \t%ld\r\nAdd_Y = \t%ld\r\nLumY =  \t%ld\r\nStableCnt = \t%ld\r\n",
                                            AeInfo.CurExpectLum.y, AeInfo.ExtSet.OverExp.CurTotalStep, AeInfo.CurLum.y, AeInfo.Counter);

    Addr += sprintf((char *)Addr, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
                                            AeInfo.PrvAvgEV.EVValue, AeInfo.PrvAvgEV.LV);

    Addr += sprintf((char *)Addr, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
                                            AeInfo.PrvParam[0].EVValue, AeInfo.PrvParam[0].LV);

    Addr += sprintf((char *)Addr, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
                                            AeInfo.PrvParam[1].EVValue, AeInfo.PrvParam[1].LV);

    Addr += sprintf((char *)Addr, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
                                            AeInfo.PrvParam[2].EVValue, AeInfo.PrvParam[2].LV);

    Addr += sprintf((char *)Addr, "CapEV_Value = \t%ld\r\nCapLV = \t%ld\r\n",
                                            AeInfo.CapParam.EVValue, AeInfo.CapParam.LV);

    Addr += sprintf((char *)Addr, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
                                            PrvNewAeArg[0].ISOGain, PrvNewAeArg[0].ExpoTime, PrvNewAeArg[0].Iris);

    Addr += sprintf((char *)Addr, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
                                            PrvNewAeArg[1].ISOGain, PrvNewAeArg[1].ExpoTime, PrvNewAeArg[1].Iris);

    Addr += sprintf((char *)Addr, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
                                            PrvNewAeArg[2].ISOGain, PrvNewAeArg[2].ExpoTime, PrvNewAeArg[2].Iris);


    Addr += sprintf((char *)Addr, "Cap_ISOGain = \t%ld\r\nCap_ExpoTime = \t%ld\r\nCap_Iris = \t%ld\r\n",
                                            CapNewAeArg.ISOGain, CapNewAeArg.ExpoTime, CapNewAeArg.Iris);

    Addr += sprintf((char *)Addr, "UI setting: Metermode=%d, ISO=%d, EV bios = %d, Flash=%d, Scene=%d, Freq=%d, ImgEffect=%d, WB=%d\r\n",
                                            IPL_AlgGetUIInfo(IPL_SEL_AEMODE), IPL_AlgGetUIInfo(IPL_SEL_ISOINDEX), IPL_AlgGetUIInfo(IPL_SEL_AEEV), IPL_AlgGetUIInfo(IPL_SEL_FLASHMODE)
                                            ,IPL_AlgGetUIInfo(IPL_SEL_SCENEMODE),IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY),IPL_AlgGetUIInfo(IPL_SEL_IMAGEEFFECT),IPL_AlgGetUIInfo(IPL_SEL_WBMODE));

    Addr += sprintf((char *)Addr, "------------------------------\r\n");
    Addr = AE_BlkWriteDebugInfo(Addr);
    Addr += sprintf((char *)Addr, "------------------------------\r\n");
#if 0
    for(i=0;i<AE_HIST_NUM;i++)
    {
        Addr += sprintf((char *)Addr, "%d\r\n", AeInfo.histogram.raw[i]);
    }
    Addr += sprintf((char *)Addr, "\r\n");

    for(i=0;i<AE_HIST_NUM;i++)
    {
        Addr += sprintf((char *)Addr, "%d\r\n", AeInfo.histogram.y[i]);
    }
#endif

    for(j=0;j<CA_WIN_Y;j++)
    {
        for(i=0;i<CA_WIN_X;i++)
        {
            Addr += sprintf((char *)Addr, "%d\t", Gacc[j*CA_WIN_X+i]>>4);
        }
        Addr += sprintf((char *)Addr, "\r\n");
    }
    return Addr;
}

void AE_GetHisto(UINT32 *Hist)
{
    UINT32 i;
    for (i = 0; i < AE_HIST_NUM; i ++)
    {
        Hist[i] = AeInfo.histogram.y[i];
    }
}

UINT32 AE_GetPrvEVValue(void)
{
    return AeInfo.PrvParam[0].LV;
}

UINT32 AE_GetCapEVValue(void)
{
    return AeInfo.CapParam.LV;
}
#if 0
#endif
//------------------------------------------------------------------------------
UINT32 AE_judgePrvSetting(AEALG_ARG *CurInfo, AEALG_ARG *PrvInfo, UINT32 *Flag)
{
    UINT32 JudgeFlag;

    JudgeFlag = FALSE;
    *Flag = 0;

    if (PrvInfo->ExpoTime != CurInfo->ExpoTime)
    {
        *Flag |= AE_FLAG_SET_EXPT;
        JudgeFlag = TRUE;
    }

    if (PrvInfo->ISOGain != CurInfo->ISOGain)
    {
        *Flag |= AE_FLAG_SET_GAIN;
        JudgeFlag = TRUE;
    }

    if (PrvInfo->Iris != CurInfo->Iris)
    {
        *Flag |= AE_FLAG_SET_IRIS;
        JudgeFlag = TRUE;
    }

    if (JudgeFlag == TRUE)
    {
        return 1;
    }
    return 0;
}

UINT32 AE_getStatisticInfo(AE_INFO2 *AeInfo)
{
    UINT32 i, j;
    UINT32 RTotal, GTotal, BTotal, WTotal;
    UINT32 Ratio_X, Ratio_Y, HistInterval, HistIntervalY;
    UINT32 Rgain, Ggain, Bgain;
    UINT32 YTotal, R, G, B, Y;
    AE_LUMINANCE BlkAvgLum;

    Ratio_X = CA_WIN_X/AE_WIN_X;
    Ratio_Y = CA_WIN_Y/AE_WIN_Y;
    if(Ratio_X <1)
        Ratio_X = 1;
    if(Ratio_Y <1)
        Ratio_Y = 1;

    Img_GetCA_Result(IPL_ID_1,Racc,Gacc,Bacc,CA_WIN_X*CA_WIN_Y);

    AeBlkInfo.CA_Num_X = CA_WIN_X;
    AeBlkInfo.CA_Num_Y = CA_WIN_Y;
    memset(AeBlkInfo.Yacc, 0, CA_WIN_X*CA_WIN_Y*4);
    memset(AeBlkInfo.Gacc, 0, CA_WIN_X*CA_WIN_Y*4);

    //calculate ae weight
    RTotal = 0;
    GTotal = 0;
    BTotal = 0;
    WTotal = 0;
    YTotal = 0;

    HistInterval = (1<<12)/AE_HIST_NUM;
    HistIntervalY = (1<<10)/AE_HIST_NUM;

    memset(AeHistBuf, 0, AE_HIST_NUM*4);
    memset(AeHistBufY, 0, AE_HIST_NUM*4);

    AWB_GetColorGian(&Rgain, &Ggain, &Bgain);

#if (IFE_GAIN_ON == ENABLE)
    if (IFEGainEn == ENABLE)
    {
        UINT32 temp;
        //DBG_DUMP("GET1:%d\r\n", IFEGainCntR1);
        for (j = 0; j < (CA_WIN_Y * CA_WIN_X); j ++)
        {
            temp = Racc[j];
            temp = (temp * IFEGain[IFEGainCntR1].CGain.R) >> 8;
            if (temp > 4096)
            {
                temp = 4095;
            }
            Racc[j] = temp;


            temp = Bacc[j];
            temp = (temp * IFEGain[IFEGainCntR1].CGain.R) >> 8;
            if (temp > 4096)
            {
                temp = 4095;
            }
            Bacc[j] = temp;

            temp = Gacc[j];
            temp = (temp * IFEGain[IFEGainCntR1].CGain.R) >> 8;
            if (temp > 4096)
            {
                temp = 4095;
            }
            Gacc[j] = temp;
        }

        IFEGainCntR1 ++;
        if (IFEGainCntR1 >= 6)
        {
            IFEGainCntR1 = 0;
        }
    }
#endif


    for (j = 0; j < CA_WIN_Y; j ++)
    {
        for (i = 0; i < CA_WIN_X; i ++)
        {
            R = Racc[j * CA_WIN_X + i];
            G = Gacc[j * CA_WIN_X + i];
            B = Bacc[j * CA_WIN_X + i];


            R = ((R * Rgain) >> 8);
            G = G;
            B = ((B * Bgain) >> 8);

            if (R > 4095) R = 4095;
            if (G > 4095) G = 4095;
            if (B > 4095) B = 4095;

            R = IQS_AddGammaTab(R);
            G = IQS_AddGammaTab(G);
            B = IQS_AddGammaTab(B);

            Y = ((R * 3) + (G * 6) + (B * 1)) / 10;

            if (Y > 1024) Y = 1024;

            YTotal += (Y * AeWin[j/Ratio_Y][i/Ratio_X]);

            GTotal += (AeWin[j/Ratio_Y][i/Ratio_X] * Gacc[j * CA_WIN_X + i]);

            WTotal += AeWin[j/Ratio_Y][i/Ratio_X];
            AeBlkInfo.Yacc[j*CA_WIN_X+i] = Y;
            AeBlkInfo.Gacc[j*CA_WIN_X+i] = Gacc[j * CA_WIN_X + i];
            AeInfo->histogram.y[Y/HistIntervalY] ++;
            AeInfo->histogram.raw[Gacc[j * CA_WIN_X + i]/HistInterval] ++;
        }
    }

    AeInfo->PrvAvgLum.y = (YTotal / WTotal);
    AeInfo->PrvAvgLum.raw = (GTotal / WTotal);

    if (AeBlkInfo.BLKLum_En == ENABLE)
    {
        AeBlkInfo.LV = AeInfo->PrvParam[0].LV;
        BlkAvgLum = AEAlg_GetBLKBasedLum(&AeBlkInfo);
        AeInfo->PrvAvgLum.y = BlkAvgLum.y;
        AeInfo->PrvAvgLum.raw = BlkAvgLum.raw;
    }
    return 0;
}

void AE_getInfo(AE_INFO2 *AeInfo)
{
    memset((void *)AeInfo, 0, sizeof(AE_INFO2));

    AeInfo->PrvAvgEV.EVValue = AE_INIT_EV;
    AeInfo->PrvAvgEV.LV = AEAlg_Conv2LV(AeInfo->PrvAvgEV.EVValue);

    AeInfo->PrvExpectLum.raw = AE_MOV_EXPECT_Y;
    AeInfo->PrvExpectLum.y = IQS_AddGammaTab(AeInfo->PrvExpectLum.raw);

    AeInfo->StableRange.h = AE_COVER_RANGE_H;
    AeInfo->StableRange.l = AE_COVER_RANGE_L;

    AeInfo->histogram.bin_num = AE_HIST_NUM;
    AeInfo->histogram.data_num = (CA_WIN_X * CA_WIN_Y);
    AeInfo->histogram.raw = AeHistBuf;
    AeInfo->histogram.y = AeHistBufY;

    // block luminance calculate
    //used in photo preview and capture only
    AeBlkInfo.Yacc = Blk_Yacc;
    AeBlkInfo.Gacc = Blk_Gacc;
    AeBlkInfo.Raw_OverBound_Lum.h = IQS_InvGammaTab(AEBlk_OVER_RAW_UP_THR);
    AeBlkInfo.Raw_OverBound_Lum.l = IQS_InvGammaTab(AEBlk_OVER_RAW_LOW_THR);
    AeBlkInfo.Raw_UnderBound_Lum.h = IQS_InvGammaTab(AEBlk_UNDER_RAW_UP_THR);
    AeBlkInfo.Raw_UnderBound_Lum.l = IQS_InvGammaTab(AEBlk_UNDER_RAW_LOW_THR);
    AeBlkInfo.AddGammaFunc = IQS_AddGammaTab;
    AeBlkInfo.BLKLum_En = DISABLE;

    AeInfo->FirstRunFlag = TRUE;
    AeInfo->Reset = TRUE;
    AeInfo->Counter = 0;
    AeInfo->CurConvDir = 0;
    AeInfo->EventTab = AeAlgEventTab;
    AeInfo->ConvSpeed = AEALG_SPEED_P1;
    AeInfo->ConvTab = NULL;
    AeInfo->InvGammaFunc = IQS_InvGammaTab;
    AeInfo->AddGammaFunc = IQS_AddGammaTab;

    //ext
    AeInfo->ExtSet.PrvLumRatioTab = ExptY_Mov_Prv;

    AeInfo->ExtSet.OverExp.AdjLum.raw = AE_EXPECT_Y4;
    AeInfo->ExtSet.OverExp.AdjLum.y = IQS_AddGammaTab(AeInfo->ExtSet.OverExp.AdjLum.raw);
    AeInfo->ExtSet.OverExp.AdjLumTab = ExptY_Mov_OverExpoAdj;
    AeInfo->ExtSet.OverExp.AdjMaxCntRatioTab = ExptY_OverExpoMaxCnt;
    AeInfo->ExtSet.OverExp.AdjMinCntRatioTab = ExptY_OverExpoMinCnt;
    AeInfo->ExtSet.OverExp.AdjLumThresholdTab = ExptY_OverExpoThreshold;
    AeInfo->ExtSet.OverExp.CurCnt = 0;
    AeInfo->ExtSet.OverExp.CurTotalStep = 0;
    AeInfo->ExtSet.OverExp.CurMaxStep = 0;
    AeInfo->ExtSet.OverExp.CurMaxCnt = 0;
    AeInfo->ExtSet.OverExp.CurMinCnt = 0;
    AeInfo->ExtSet.OverExp.CurThreshold = 0;


    AeInfo->ExtSet.AdjHistoEn = ENABLE;
    AeInfo->ExtSet.AdjHistoInfo.RatioTab = Histo_Prv;
    AeInfo->ExtSet.AdjHistoInfo.AdjLum.raw = AE_EXPECT_Y2;
    AeInfo->ExtSet.AdjHistoInfo.AdjLum.y = IQS_AddGammaTab(AeInfo->ExtSet.AdjHistoInfo.AdjLum.raw);

    AeInfo->ExtSet.AdjCapEn = DISABLE;
    AeInfo->ExtSet.CapExpectLum.raw = AE_PHOTO_EXPECT_Y;
    AeInfo->ExtSet.CapExpectLum.y = IQS_AddGammaTab(AeInfo->ExtSet.CapExpectLum.raw);
    AeInfo->ExtSet.CapExpectTab = ExptY_Cap;

    //comp ev
    AeInfo->EVCompRatio = EVTable[IPL_AlgGetUIInfo(IPL_SEL_AEEV)];
}

AE_RESET AE_setWin(AE_WIN mode)
{
    static AE_WIN prvmode = 0xFF;
    if (mode == prvmode)
    {
        return AE_RESET_FALSE;
    }
    DBG_ERR("SetWin %d\r\n", mode);
    switch(mode)
    {
        case AE_WIN_CENTERWEIGHT:
            AeWin = (UINT8(*)[AE_WIN_X])AE_Win_CenterWeighted;
            break;

        case AE_WIN_SPOT:
            AeWin = (UINT8(*)[AE_WIN_X])AE_Win_Spot;
            break;

        case AE_WIN_MARTIX:
            AeWin = (UINT8(*)[AE_WIN_X])AE_Win_Multi;
            break;

        default:
        case AE_WIN_EVALUATIVE:
            AeWin = (UINT8(*)[AE_WIN_X])AE_Win_Evaluative;
            break;
    }
    prvmode = mode;
    return AE_RESET_TRUE;
}

UINT32 AE_getFlag(void)
{
    return AeFlag;
}

void AE_setFlag(UINT32 flag)
{
    AeFlag |= flag;
}

void AE_clrFlag(UINT32 flag)
{
    AeFlag &= ~flag;
}

void AE_copyArg(AEALG_ARG *src, AEALG_ARG *dest)
{
    *dest = *src;
}

void AE_reset (void)
{
    AeInfo.Reset = TRUE;
}

void AE_getCapTab(AE_ISO nISO, AE_FLICKER nFlicker, AE_SCENE nScene, AE_INFO2 *AeInfo)
{
    static UINT32 scene=0xff,iso=0xff,flicker=0xff;

    if((scene == nScene)&&(iso == nISO)&&(flicker == nFlicker))
    {
        return;
    }
    AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_AntiFlicker60;
    scene = nScene;
    iso = nISO;
    flicker = nFlicker;
    AE_reset();
}

AE_RESET AE_getPrvTab(AE_ISO nISO, AE_FLICKER nFlicker, AE_INFO2 *AeInfo)
{
    static AE_FLICKER nPreFlicker = 0xFF;
    if (nPreFlicker == nFlicker)
    {
        return AE_RESET_FALSE;
    }

    if(nFlicker == AE_FLICKER_50HZ)
    {
        switch(nISO)
        {
            case AE_ISO_AUTO:
            case AE_ISO_25:
            case AE_ISO_50:
            case AE_ISO_80:
            case AE_ISO_100:
            case AE_ISO_200:
            case AE_ISO_400:
            case AE_ISO_800:
            case AE_ISO_1600:
            case AE_ISO_3200:
            case AE_ISO_6400:
            default:
                AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_AntiFlicker50;
                AeInfo->EV.h = AE_MAX_EV;
                AeInfo->EV.l = AE_MIN_EV;
                AeInfo->ISO.h = AE_MAX_ISO;
                AeInfo->ISO.l = AE_MIN_ISO;
                break;
        }
    }
    else //if(nFlicker==_POWERFreq_60Hz)
    {
        switch(nISO)
        {
            case AE_ISO_AUTO:
            case AE_ISO_25:
            case AE_ISO_50:
            case AE_ISO_80:
            case AE_ISO_100:
            case AE_ISO_200:
            case AE_ISO_400:
            case AE_ISO_800:
            case AE_ISO_1600:
            case AE_ISO_3200:
            case AE_ISO_6400:
            default:
                AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_AntiFlicker60;
                AeInfo->EV.h = AE_MAX_EV;
                AeInfo->EV.l = AE_MIN_EV;
                AeInfo->ISO.h = AE_MAX_ISO;
                AeInfo->ISO.l = AE_MIN_ISO;
                break;
        }
    }
    nPreFlicker = nFlicker;

    return AE_RESET_TRUE;
}
#if 0
#endif
//------------------------------------------------------------------------------
UINT32 GetOverExpoRatio(UINT32 HistMaxNum, UINT32 *Hist)
{
    UINT32 Cnt;

    Cnt = (Hist[60] + Hist[61] + Hist[62] + Hist[63]) +
          (Hist[56] + Hist[57] + Hist[58] + Hist[59]) +
          (Hist[52] + Hist[53] + Hist[54] + Hist[55]) / 2; // >224  [224~239]+[240~225
    return Cnt;
}

UINT32 GetFnoRatio(AEALG_IRIS_IDX Idx)
{
#if 1
    return 100;
#else
    //IRIS_POS Iris_Step;
    UINT32 FnRatio;

    switch(Idx)
    {
        case AEALG_IRIS_0:
            //Iris_Step = IRIS_POS_F1_8;
            FnRatio = 53;// sepc is 32
            break;
        case AEALG_IRIS_1:
            //Iris_Step = IRIS_POS_F2_8;
            FnRatio = 80;
            break;
        case AEALG_IRIS_2:
            //Iris_Step = IRIS_POS_F4_0;
            FnRatio = 160;
            break;
        case AEALG_IRIS_3:
            //Iris_Step = IRIS_POS_F5_6;
            FnRatio = 320;
            break;
        case AEALG_IRIS_4:
            //Iris_Step = IRIS_POS_F8_0;
            FnRatio = 640;
            break;
        default:
            FnRatio = 80;
            break;
    }

    return FnRatio;
#endif
}
#if 0
#endif
//------------------------------------------------------------------------------

AE_SCENE AE_getSceneType(UINT32 Idx)
{
    if (Idx == SEL_SCENEMODE_AUTO) return AE_SCENE_AUTO;
    if (Idx == SEL_SCENEMODE_LANDSCAPE) return AE_SCENE_LANDSCAPE;
    if (Idx == SEL_SCENEMODE_POTRAIT) return AE_SCENE_POTRAIT;
    if (Idx == SEL_SCENEMODE_SPORT) return AE_SCENE_SPORT;
    if (Idx == SEL_SCENEMODE_NIGHTSCENE) return AE_SCENE_NIGHTSCENE;
    if (Idx == SEL_SCENEMODE_AV) return AE_SCENE_AV;
    if (Idx == SEL_SCENEMODE_TV) return AE_SCENE_TV;
    if (Idx == SEL_SCENEMODE_MANUAL) return AE_SCENE_MANUAL;
    if (Idx == SEL_SCENEMODE_NIGHTPOTRAIT) return AE_SCENE_NIGHTPOTRAIT;
    if (Idx == SEL_SCENEMODE_NIGHTLANDSCAPE) return AE_SCENE_NIGHTLANDSCAPE;
    if (Idx == SEL_SCENEMODE_PARTY) return AE_SCENE_PARTY;
    if (Idx == SEL_SCENEMODE_SUNSET) return AE_SCENE_SUNSET;
    if (Idx == SEL_SCENEMODE_BACKLIGHT) return AE_SCENE_BACKLIGHT;
    if (Idx == SEL_SCENEMODE_DOCUMENT) return AE_SCENE_DOCUMENT;
    if (Idx == SEL_SCENEMODE_FIREWORK) return AE_SCENE_FIREWORK;
    if (Idx == SEL_SCENEMODE_BEACH) return AE_SCENE_BEACH;
    if (Idx == SEL_SCENEMODE_BMODE) return AE_SCENE_BMODE;
    if (Idx == SEL_SCENEMODE_PROGRAM) return AE_SCENE_PROGRAM;
    if (Idx == SEL_SCENEMODE_HIGHISO) return AE_SCENE_HIGHISO;
    return AE_SCENE_AUTO;
}

AE_FLICKER AE_getFlickerType(UINT32 Idx)
{
    if (Idx == SEL_FREQUENCY_AUTO)
    {
        if ( AEAFD_GetRslt() == AEAFD_FLICKER_50Hz )
            AEAFD_CurrentFlickerType = AE_FLICKER_50HZ;
        if ( AEAFD_GetRslt() == AEAFD_FLICKER_60Hz )
            AEAFD_CurrentFlickerType = AE_FLICKER_60HZ;
        return AEAFD_CurrentFlickerType;
    }
    else if (Idx == SEL_FREQUENCY_50HZ)
    {
        return AE_FLICKER_50HZ;
    }
    else
    {
        return AE_FLICKER_60HZ;
    }
}

AE_ANTISHAKE AE_getAntiShakeLv(UINT32 Idx)
{

    return AE_ANTISHAKE_LV0;
}

AE_ISO AE_getIsoType(UINT32 Idx)
{
    if (Idx == SEL_ISOINDEX_AUTO) return AE_ISO_AUTO;
    if (Idx == SEL_ISOINDEX_50) return AE_ISO_50;
    if (Idx == SEL_ISOINDEX_80) return AE_ISO_80;
    if (Idx == SEL_ISOINDEX_100) return AE_ISO_100;
    if (Idx == SEL_ISOINDEX_200) return AE_ISO_200;
    if (Idx == SEL_ISOINDEX_400) return AE_ISO_400;
    if (Idx == SEL_ISOINDEX_800) return AE_ISO_800;
    if (Idx == SEL_ISOINDEX_1600) return AE_ISO_1600;
    if (Idx == SEL_ISOINDEX_3200) return AE_ISO_3200;
    if (Idx == SEL_ISOINDEX_6400) return AE_ISO_6400;
    return AE_ISO_AUTO;
}

AE_WIN AE_getWinType(UINT32 Idx)
{
    if (Idx == SEL_AEMODE_CENTERWEIGHTED) return AE_WIN_CENTERWEIGHT;
    if (Idx == SEL_AEMODE_SPOT) return AE_WIN_SPOT;
    if (Idx == SEL_AEMODE_MATRIX) return AE_WIN_MARTIX;
    if (Idx == SEL_AEMODE_EVALUATIVE) return AE_WIN_EVALUATIVE;
    return AE_WIN_CENTERWEIGHT;
}

#if 0
#endif
//------------------------------------------------------------------------------
UINT32 AE_getPrvBiningMode(void)
{
    SENSOR_INFO SensorInfo;
    SENSOR_MODE PrvSensorMode;

    PrvSensorMode = (SENSOR_MODE)IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE);
    Sensor_GetStatus(SENSOR_ID_1, PrvSensorMode, &SensorInfo);

    return SensorInfo.Mode->biningRatio;
}

UINT32 AE_getCapBiningMode(void)
{
    SENSOR_INFO SensorInfo;
    SENSOR_MODE CapSensorMode;

    CapSensorMode = (SENSOR_MODE)IPL_AlgGetUIInfo(IPL_SEL_CAPSENMODE);
    Sensor_GetStatus(SENSOR_ID_1, CapSensorMode, &SensorInfo);

    return SensorInfo.Mode->biningRatio;
}

void AE_setExpT(EXPOSURE_SETTING *ExpTSetting)
{
    if ((AE_getFlag() & AE_FLAG_SET_EXPT) == AE_FLAG_SET_EXPT)
    {
        if ((ExpTSetting->uiFrame== 0xffffffff) && (ExpTSetting->uiLine == 0xffffffff) && (ExpTSetting->uiPixel == 0xffffffff))
            DBG_MSG("Exposure Line Value Invalid\r\n");
        else
            Sensor_SetExpTime(SENSOR_ID_1,ExpTSetting);

        ExpTSetting->uiFrame = 0xffffffff;
        ExpTSetting->uiLine  = 0xffffffff;
        ExpTSetting->uiPixel = 0xffffffff;

        AE_clrFlag(AE_FLAG_SET_EXPT);
    }
}

void AE_setGain(GAIN_SETTING *Gain)
{
    if ((AE_getFlag() & AE_FLAG_SET_GAIN) == AE_FLAG_SET_GAIN)
    {
        if ((Gain->data1 == 0xffffffff) && (Gain->data2 == 0xffffffff) && (Gain->data2 == 0xffffffff))
            DBG_MSG("AGC Gain Value Invalid\r\n");
        else
            Sensor_SetGain(SENSOR_ID_1,Gain);

        Gain->data1 = 0xffffffff;
        Gain->data2 = 0xffffffff;
        Gain->data3 = 0xffffffff;

        AE_clrFlag(AE_FLAG_SET_GAIN);
    }
}

void AE_setIris(AEALG_IRIS_IDX Idx)
{
    IRIS_POS Iris_Step;

    if ((AE_getFlag() & AE_FLAG_SET_IRIS) == AE_FLAG_SET_IRIS)
    {
        switch(Idx)
        {
            case AEALG_IRIS_0:
                default:
                Iris_Step = IRIS_POS_F1_8;
                break;
            case AEALG_IRIS_1:
                Iris_Step = IRIS_POS_F2_8;
                break;
            case AEALG_IRIS_2:
                Iris_Step = IRIS_POS_F4_0;
                break;
            case AEALG_IRIS_3:
                Iris_Step = IRIS_POS_F5_6;
                break;
            case AEALG_IRIS_4:
                Iris_Step = IRIS_POS_F8_0;
                break;
        }
        if(Lens_Module_GetState() != LENS_STATE_IDLE)
        {
            DBG_MSG("Switch aperture fail\r\n");
        }
        else
        {
            Lens_Aperture_Move(Iris_Step);
            AE_clrFlag(AE_FLAG_SET_IRIS);
        }
    }
}

ER AE_setFrame(UINT32 Frame, UINT32 *VTotal, UINT32 *HTotal)
{
    ER rt;
    rt = Sensor_SetFrame(SENSOR_ID_1,Frame, VTotal, HTotal);
    if ( rt != E_OK )
        DBG_ERR("AE_setFrame error\r\n");
    return rt;
}

BOOL AE_Wait_Stable(UINT32 StableCnt, UINT32 TimeOutFrame)
{
    UINT32 TimeOutCnt=0;

    while(TimeOutCnt < TimeOutFrame)
    {
        if(AeInfo.Counter > StableCnt)
            return TRUE;
        else
            TimeOutCnt ++;
    }
    return FALSE;
}

void AE_StartAFD(void)
{
    AEAFD_START = TRUE;
}

void AE_GetExifInfo(UINT32 *ISO, UINT32 *ExpT, UINT32 *Aperture, UINT32 *MaxApture)
{
    UINT32 ApertureValue;

    switch(CapNewAeArg.Iris)
    {
        case AEALG_IRIS_0:
            default:
            ApertureValue = 18;
            break;
        case AEALG_IRIS_1:
            ApertureValue = 28;
            break;
        case AEALG_IRIS_2:
            ApertureValue = 40;
            break;
        case AEALG_IRIS_3:
            ApertureValue = 56;
            break;
        case AEALG_IRIS_4:
            ApertureValue = 80;
            break;
    }

    *ExpT = CapNewAeArg.ExpoTime;
    *ISO = CapNewAeArg.ISOGain;
    *Aperture = ApertureValue;
    *MaxApture = 18;
}

UINT32 AEB_GetRatioFromEV(SEL_AEEV AEB_Step)
{
    AEB_Ratio = EVTable[AEB_Step];
    return AEB_Ratio;
}

void AEB_SetRatio2Cap(UINT32 Ratio)
{

    AeInfo.CapParam.EVValue = AeInfo.PrvParam[0].EVValue * AE_EV_RATIO / Ratio;

    AEAlg_Allot(AeInfo.CapParam.EVValue, AeInfo.CapCurve, AeInfo.EventTab.GetFnoRatio, AeInfo.EventTab.GetCurveOfsRatio, &CapNewAeArg);
}


/*-------------- for debug in AE_Uart_Cmd.c ------------------*/
#if 0
BOOL Cmd_ae_SetET(CHAR* strCmd)
{
    UINT32 ET, LUM=0,i;
    SENSOR_MODE Mode;
    EXPOSURE_SETTING ExpTSetting;
    IPL_CA_INFOR CAInfo;


    AE_Lock(0, 1, ENABLE);
    sscanf(strCmd, "%d", &ET);
    Sensor_GetCurMode(SENSOR_ID_1, &Mode);

    Sensor_GetExpoSetting(SENSOR_ID_1, ET, Mode, &ExpTSetting);
    Sensor_SetExpTime(SENSOR_ID_1,&ExpTSetting);

    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    //LUM = AE_getStatisticInfo(0, NULL);

    CAInfo.Id = IPL_ID_1;
    CAInfo.R = Racc;
    CAInfo.G = Gacc;
    CAInfo.B = Bacc;
    CAInfo.Length = 1024;
    IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CAInfo);

    for(i=0;i<CAInfo.Length;i++)
    {
        //Racc[i] = (Racc[i]>>4)&0xff;
        Gacc[i] = (Gacc[i]>>4)&0xff;
        //Bacc[i] = (Bacc[i]>>4)&0xff;

        LUM += Gacc[i];
    }

    LUM /= CAInfo.Length;

    DBG_ERR("Lum=%d, ET=%d, Frame=%d, Line=%d, pixel=%d\r\n",LUM,ET,ExpTSetting.uiFrame,ExpTSetting.uiLine,ExpTSetting.uiPixel);

    return TRUE;
}

BOOL Cmd_ae_SetISO(CHAR* strCmd)
{
    UINT32 ISO, LUM=0,i;
    GAIN_SETTING Gain;
    IPL_CA_INFOR CAInfo;

    AE_Lock(0, 1, ENABLE);
    sscanf(strCmd, "%d", &ISO);


    Sensor_GetGainSetting(SENSOR_ID_1, ISO, &Gain);
    Sensor_SetGain(SENSOR_ID_1,&Gain);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    //LUM = AE_getStatisticInfo(0, NULL);

    CAInfo.Id = IPL_ID_1;
    CAInfo.R = Racc;
    CAInfo.G = Gacc;
    CAInfo.B = Bacc;
    CAInfo.Length = 1024;
    IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CAInfo);

    for(i=0;i<CAInfo.Length;i++)
    {
        Gacc[i] = (Gacc[i]>>4)&0xff;

        LUM += Gacc[i];
    }

    LUM /= CAInfo.Length;

    DBG_ERR("Lum=%d, ISO=%d, Again=%d, Dgain=%d, xxx=%d\r\n",LUM,ISO,Gain.data1,Gain.data2,Gain.data3);
    return TRUE;
}

BOOL Cmd_ae_SetIris(CHAR* strCmd)
{
    UINT32 Iris, LUM=0,i;
    IPL_CA_INFOR CAInfo;

    AE_Lock(0, 1, ENABLE);
    sscanf(strCmd, "%d", &Iris);

    AE_setFlag((AE_FLAG_SET_IRIS));
    AE_setIris(Iris);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    //LUM = AE_getStatisticInfo(0, NULL);

    CAInfo.Id = IPL_ID_1;
    CAInfo.R = Racc;
    CAInfo.G = Gacc;
    CAInfo.B = Bacc;
    CAInfo.Length = 1024;
    IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CAInfo);

    for(i=0;i<CAInfo.Length;i++)
    {
        Gacc[i] = (Gacc[i]>>4)&0xff;

        LUM += Gacc[i];
    }

    LUM /= CAInfo.Length;

    DBG_ERR("Lum=%d, Iris=%d\r\n",LUM,Iris);
    return TRUE;
}
#endif
/*--------------------------------------------------------*/

UINT32 AE_getCurLum(UINT32 LumBit)
{
    UINT32 Lum;

    if(LumBit < 12)
        Lum = AeInfo.PrvAvgLum.raw >> (12-LumBit);
    else
        Lum = AeInfo.PrvAvgLum.raw << (LumBit - 12);

    return Lum;
}

