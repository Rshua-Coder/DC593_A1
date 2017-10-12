/**
    Auto Exposure flow.

    ae flow.

    @file       ae_AR0330P_EVB_FF.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <stdio.h>

#include "type.h"
#include "IPL_AlgInfor.h"
#include "Ae_alg.h"
#include "Ae_OV2710_EVB_FF_int.h"
#include "Ae_Api.h"
#include "Ae_Task.h"
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
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

UINT32 AEUartMsg = DISABLE;

extern UINT32 uiISOGain, uiExpoTime, uiBinning;
extern void IQS_GetBinningFactor(UINT32 *PrvBin, UINT32 *CapBin);
UINT32 IfeBinning = 0;
UINT32 overExpoCnt = 0;

static UINT32 AE_AutoFlickerDetectProc(void);

static UINT8 (*AeWin)[AE_WIN_X] = 0;
static UINT32 AeFlag = 0;
static AE_INFO AeInfo = {0};
static AE_EVENT_TAB AEEventTAB = {0};
static UINT32 AeHistBuf[AE_HIST_NUM];
static AEALG_ARG PrvOldAeArg, PrvNewAeArg, CapNewAeArg, PrvNewAeArg2, PrvNewAeArg3;
static UINT32 AECnt = 0;

GAIN_SETTING AEGain;
INT32 SetGain1 = 0;
INT32 SetGain2 = 0;
INT32 SetGain3 = 0;

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
UINT32 AEB_Ratio = 100;

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

void AE_LoadEventTAB(AE_EVENT_TAB *EventTab)
{
    AEEventTAB = *EventTab;
}

static void AE_SetMovieLog(char* Buf, UINT32 BufLen)
{
    char msg[64];
    memset(msg, 0, sizeof(char)*64);
    sprintf(msg, "%d %d %d %d %d %d %d",            AeInfo.LumY, AeInfo.ExpectY, AeInfo.PrvEVValue,
                                                PrvNewAeArg.ISOGain, PrvNewAeArg.ExpoTime, IfeBinning,
                                                AeInfo.OverExpoInfo.AdjY);
    memcpy(Buf, msg, BufLen-1);
}

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
        AeInfo.PrvEVValue = AE_INIT_EV;
        AeInfo.PrvLV = AEAlg_Conv2LV(AeInfo.PrvEVValue);
        AeInfo.CapEVValue = AeInfo.PrvEVValue;// * AeInfo->CapBiningMode / AeInfo->PrvBiningMode;
        AeInfo.CapLV = AEAlg_Conv2LV(AeInfo.CapEVValue);
        Init_Flag = TRUE;
    }
    AE_getInfo(&AeInfo);

    //get ae table
    AE_getPrvTab(AE_getIsoType(IPL_AlgGetUIInfo(IPL_SEL_ISOINDEX)), AE_getFlickerType(IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY)), &AeInfo);

    //get & save iso, exposure time, iris
    if ( uiBinning == 0 )
        AEAlg_Allot(AeInfo.PrvEVValue, AeInfo.PrvCurve, AeInfo.EventTab.GetFnoRatio, AeInfo.EventTab.GetCurveOfsRatio, &PrvNewAeArg);
    AE_copyArg(&PrvNewAeArg, &PrvOldAeArg);
    AE_setFlag((AE_FLAG_SET_IRIS|AE_FLAG_SET_GAIN|AE_FLAG_SET_EXPT));


    UINT32 PrvBin, CapBin;
    IQS_GetBinningFactor(&PrvBin, &CapBin);
    uiBinning = PrvBin;

    Sensor_GetCurMode(SENSOR_ID_1, &Mode);
    Ratio = Sensor_GetExpoSetting(SENSOR_ID_1, PrvNewAeArg.ExpoTime, Mode, &ExpTSetting);
    Sensor_GetGainSetting(SENSOR_ID_1, (PrvNewAeArg.ISOGain * Ratio / 100) / (1<<uiBinning), &Gain);

    AE_setExpT(&ExpTSetting);
    AE_setGain(&Gain);
    AE_setIris(PrvNewAeArg.Iris);
    IQS_SettingFlowFunc(IQS_AE_END);


    IQS_SetMovieLogFP(AE_SetMovieLog);
    AECnt = 0;


}

void AE_Process (UINT32 CurrentStatus)
{
    UINT32 Flag,uiImeY, Ratio;
    AE_RESET AeRst = AE_RESET_FALSE;
    SENSOR_MODE Mode;
    EXPOSURE_SETTING ExpTSetting;
    GAIN_SETTING Gain;

#if 0
    //DBG_ERR("^GAlgInfor Hard Code here...\r\n");
    IPL_AlgSetUIInfo(IPL_SEL_SHARPNESS, SEL_SHARPNESS_NORMAL);
    IPL_AlgSetUIInfo(IPL_SEL_SATURATION, SEL_SATURATION_NORMAL);
    IPL_AlgSetUIInfo(IPL_SEL_FREQUENCY, SEL_FREQUENCY_50HZ);
    IPL_AlgSetUIInfo(IPL_SEL_AEMODE, SEL_AEMODE_EVALUATIVE);
    IPL_AlgSetUIInfo(IPL_SEL_AEEV, SEL_AEEV_00);
    IPL_AlgSetUIInfo(IPL_SEL_WBMODE, SEL_WBMODE_AUTO);
    IPL_AlgSetUIInfo(IPL_SEL_ISOINDEX, SEL_ISOINDEX_AUTO);
//    IPL_AlgSetUIInfo(IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_30);
    IPL_AlgSetUIInfo(IPL_SEL_FLASHMODE, SEL_FLASHMODE_FORCEOFF);
#endif
    AECnt ++;
    if (0 != (AECnt % 3))
    {
        if (SetGain2!=0 && SetGain3!=0 )
        {
            if ( (AECnt % 3) == 1 )
            {
                Sensor_GetGainSetting(SENSOR_ID_1, (SetGain2) / (1<<IfeBinning), &Gain);
            }
            else
            {
                Sensor_GetGainSetting(SENSOR_ID_1, (SetGain3) / (1<<IfeBinning), &Gain);
            }
            AE_setFlag(AE_FLAG_SET_GAIN);
            AE_setGain(&Gain);
        }
        return;
    }
    else
    {
        DBG_IND("AE_Process in %d\r\n", AECnt);
    }

    if ( AEAFD_START == TRUE )
    {
        AE_AutoFlickerDetectProc();
    }

    //Get ime Y,only used by print msg
    //GetActImgFBAddr_Path1(&_FB_Act);
    //uiImeY = aaa_EstimateY2(_FB_Act.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));
    uiImeY = 0;
    //set ae window
    AeRst = AE_setWin(AE_getWinType(IPL_AlgGetUIInfo(IPL_SEL_AEMODE)));
    //get preview ae curve
    AeRst = AE_getPrvTab(AE_getIsoType(IPL_AlgGetUIInfo(IPL_SEL_ISOINDEX)), AE_getFlickerType(IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY)), &AeInfo);
    AeInfo.Reset = AeRst;

    //get capture ae curve
    AE_getCapTab(AE_getIsoType(IPL_AlgGetUIInfo(IPL_SEL_ISOINDEX)), AE_getFlickerType(IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY)), AE_getSceneType(IPL_AlgGetUIInfo(IPL_SEL_SCENEMODE)), &AeInfo);

    // get statistic informaiton
    AeInfo.LumY = AE_getStatisticInfo(AE_HIST_NUM, AeInfo.Hist);

    AeInfo.ExpectY = (AE_EXPECT_Y * EVTable[IPL_AlgGetUIInfo(IPL_SEL_AEEV)]) / AE_EV_RATIO;

    /**** do something at here start****/
    //we can change ae information

    static UINT32 DisableDynaTarget = 0;
    static INT32  PreTargetYOfst = 0;
    static INT32  CurTargetYOfst = 0;
    static UINT32 TargetYIdx = 0;
    if ( DisableDynaTarget == 0 )
    {
        if ((TargetYIdx % 5) == 0)
        {

            UINT32 maxLV, minLV, curLV;
            INT32 TargetYOfst, maxTargetOfst;

            maxTargetOfst = (AE_EXPECT_Y - AE_EXPECT_Y_NIGHT); //AE_ADJ_EXPECT_Y_OFST_MAX;
            maxLV = AEAlg_Conv2LV(AE_ADJ_EXPECT_Y_EV_U);
            minLV = AEAlg_Conv2LV(AE_ADJ_EXPECT_Y_EV_L);
            curLV = AEAlg_Conv2LV(AeInfo.PrvEVValue);
            if ( curLV< minLV )
                TargetYOfst = maxTargetOfst;
            else if ( curLV > maxLV )
                TargetYOfst = 0;
            else
                TargetYOfst = maxTargetOfst - ( maxTargetOfst ) * ( curLV - minLV ) / ( maxLV - minLV );
            PreTargetYOfst = CurTargetYOfst;
            CurTargetYOfst = TargetYOfst;
            if ( (UINT32)AeInfo.ExpectY > (UINT32)(PreTargetYOfst + (CurTargetYOfst - PreTargetYOfst) / 5))
                AeInfo.ExpectY = AeInfo.ExpectY - (PreTargetYOfst + (CurTargetYOfst - PreTargetYOfst) / 5);
        }
        else
        {
            if ( (UINT32)(AeInfo.ExpectY) > (UINT32)(PreTargetYOfst + (CurTargetYOfst - PreTargetYOfst) * ((INT32)(TargetYIdx % 5)+1) / 5) )
                AeInfo.ExpectY = (UINT32)(AeInfo.ExpectY - (PreTargetYOfst + (CurTargetYOfst - PreTargetYOfst) * ((INT32)(TargetYIdx % 5)+1) / 5));
        }
        TargetYIdx++;
    }
    else
    {
        TargetYIdx = 0;
        PreTargetYOfst = 0;
        CurTargetYOfst = 0;
        TargetYIdx = 0;
        DisableDynaTarget = DisableDynaTarget - 1;
    }

    if ( AeInfo.ExpectY < AE_EXPECT_Y_MIN )
        AeInfo.ExpectY  = AE_EXPECT_Y_MIN;

    AeInfo.ExpectY = AeInfo.ExpectY / (1<<IfeBinning);

    /**** do something at here end  ****/


    //calculate ae
    AE_INFO tmpAeInfo;
    AEALG_ARG tmpAeArg1,tmpAeArg2, tmpAeArg3;
    memcpy(&tmpAeInfo, &AeInfo, sizeof(AE_INFO));
    memcpy(&tmpAeArg1, &PrvNewAeArg, sizeof(AEALG_ARG));
    memcpy(&tmpAeArg2, &PrvNewAeArg2, sizeof(AEALG_ARG));
    memcpy(&tmpAeArg3, &PrvNewAeArg3, sizeof(AEALG_ARG));
    AEAlg_Converge(&AeInfo, &PrvNewAeArg, &CapNewAeArg, &PrvNewAeArg2, &PrvNewAeArg3);

    UINT32 PrvBin, CapBin, preIfeBinn;
    preIfeBinn = IfeBinning;
    IQS_GetBinningFactor(&PrvBin, &CapBin);
    IfeBinning = PrvBin;

    if ( uiISOGain != 0 )
    {
        IfeBinning = uiBinning;
        PrvNewAeArg.ExpoTime = uiExpoTime;
        PrvNewAeArg.ISOGain = uiISOGain;
        Sensor_GetGainSetting(SENSOR_ID_1, (uiISOGain) / (1<<IfeBinning), &Gain);
        AE_setFlag(AE_FLAG_SET_EXPT | AE_FLAG_SET_GAIN);
        Sensor_GetCurMode(SENSOR_ID_1, &Mode);
        Sensor_GetExpoSetting(SENSOR_ID_1, uiExpoTime, Mode, &ExpTSetting);
        AE_setExpT(&ExpTSetting);
        AE_setGain(&Gain);
        IQS_SettingFlowFunc(IQS_AE_END);

        SetGain2 = 0;
        SetGain3 = 0;
        return ;
    }

    if ( IfeBinning != preIfeBinn )
    {
        Sensor_GetCurMode(SENSOR_ID_1, &Mode);
        Sensor_GetGainSetting(SENSOR_ID_1, (tmpAeArg3.ISOGain) / (1<<IfeBinning), &Gain);
        AE_setFlag(AE_FLAG_SET_GAIN);
        AE_setGain(&Gain);

//Aeg0
        IPC_WaitVD(FALSE,IPL_ID_1);
//Arg2
        IPC_WaitVD(FALSE,IPL_ID_1);
//Arg3
        IQS_SettingFlowFunc(IQS_AE_END);
        IPC_WaitVD(FALSE,IPL_ID_1);
//Arg2 + IFE

        memcpy(&AeInfo, &tmpAeInfo, sizeof(AE_INFO));
        AeInfo.LumY = AE_getStatisticInfo(AE_HIST_NUM, AeInfo.Hist);
        AeInfo.ExpectY = AeInfo.ExpectY / (1<<IfeBinning);
        AeInfo.PrvEVValue =  tmpAeArg3.EVValue;
        AeInfo.ReduceOverExpoModeEn = DISABLE;
        AEAlg_Converge(&AeInfo, &PrvNewAeArg, &CapNewAeArg, &PrvNewAeArg2, &PrvNewAeArg3);
        AeInfo.ReduceOverExpoModeEn = ENABLE;
        PrvNewAeArg2.ISOGain = 0;
        PrvNewAeArg3.ISOGain = 0;

        DisableDynaTarget = 15;
    }
#if 0
    UINT32 newISO = 300;
    PrvNewAeArg.ExpoTime = PrvNewAeArg.ExpoTime * PrvNewAeArg.ISOGain / newISO;
    PrvNewAeArg.ISOGain = newISO;
#endif
    AE_judgePrvSetting(&PrvNewAeArg, &PrvOldAeArg, &Flag);

    Sensor_GetCurMode(SENSOR_ID_1, &Mode);
    Ratio = Sensor_GetExpoSetting(SENSOR_ID_1, PrvNewAeArg.ExpoTime, Mode, &ExpTSetting);
    if ( PrvNewAeArg.ExpoTime!=PrvOldAeArg.ExpoTime )
    {
        SetGain1 = PrvNewAeArg.ISOGain;
        SetGain2 = 0;
        SetGain3 = 0;
    }
    else
    {
        SetGain1 = PrvNewAeArg.ISOGain;
        SetGain2 = PrvNewAeArg2.ISOGain * PrvNewAeArg2.ExpoTime / PrvNewAeArg.ExpoTime;
        SetGain3 = PrvNewAeArg3.ISOGain * PrvNewAeArg3.ExpoTime / PrvNewAeArg.ExpoTime;
    }

    Sensor_GetGainSetting(SENSOR_ID_1, (SetGain1 * Ratio / 100) / (1<<IfeBinning), &Gain);

    AE_copyArg(&PrvNewAeArg, &PrvOldAeArg);

    if (Ratio != 100 || IfeBinning != 0)
    {
        Flag |= AE_FLAG_SET_GAIN;
    }
    AE_setFlag(Flag);
    AE_setIris(PrvNewAeArg.Iris);
    IPC_WaitVD(FALSE,IPL_ID_1); //sync start
    AE_setExpT(&ExpTSetting);
    IPC_WaitVD(FALSE,IPL_ID_1);
    AE_setGain(&Gain);

    if (AEEventTAB.AeProcessEnd != NULL)
    {
        AEEventTAB.AeProcessEnd(PrvNewAeArg.ISOGain, PrvNewAeArg.ExpoTime);
    }


    IQS_SettingFlowFunc(IQS_AE_END);

    if (AEUartMsg == ENABLE)
    {
        if (0 == (AECnt % 3))
        {
            DBG_ERR("%3d %3d %8d %3d %7d, %3d, %d, %d %d %d\r\n", AeInfo.LumY, AeInfo.ExpectY, AeInfo.PrvEVValue,
                        PrvNewAeArg.ISOGain, PrvNewAeArg.ExpoTime, IfeBinning, CapNewAeArg.ISOGain, CapNewAeArg.ExpoTime, CapNewAeArg.Iris, overExpoCnt);
            //DBG_ERR("%3d %3d %3d %3d %d\r\n", overExpoCnt, AeInfo.OverExpoInfo.AdjExpextY, AeInfo.OverExpoInfo.AdjY, IfeBinning, AeInfo.OverExpoInfo.Data[6]);

            DBG_ERR("----------------------------\r\n");
        }
    }
#if 0
    static UINT32 ShowCnt;
    if ( (ShowCnt%10) == 0 )
    {
        char pString[512];
        SenFP_ClearOSD();
        sprintf(pString, "%3d %3d %3d", PrvNewAeArg.ISOGain, PrvNewAeArg.ExpoTime, IfeBinning);
        //sprintf(pString, "%d %d", IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY), AE_getFlickerType(IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY)));
        SenFP_ShowOSDString(pString, 20, 40, 4);
//        sprintf(pString, "%d  %3d", AeInfo.LumY, AeInfo.PrvLV);
        //sprintf(pString, "%d %d", IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY), AE_getFlickerType(IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY)));
//        SenFP_ShowOSDString(pString, 20, 60, 4);

    }
    ShowCnt++;
#endif
}

void AE_GetPrvAEArg(UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris)
{
    *ExpT = PrvNewAeArg.ExpoTime;
    *ISO = PrvNewAeArg.ISOGain;
    *Iris = PrvNewAeArg.Iris;
}

void AE_GetCapAEArg(UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris)
{
    *ExpT = CapNewAeArg.ExpoTime;
    *ISO = CapNewAeArg.ISOGain;
    *Iris = CapNewAeArg.Iris;
}

void AE_WaitStable(UINT32 count)
{
    AE_Wait_Stable(count, 10);
}

UINT32 AE_WriteDebugInfo(UINT32 Addr)
{
    Addr += sprintf((char *)Addr, "ExpectY = \t%ld\r\nAdd_Y = \t%ld\r\nLumY = \t%ld\r\n",
                                            AeInfo.OverExpoInfo.AdjExpextY, AeInfo.OverExpoInfo.AdjY, AeInfo.LumY);

    Addr += sprintf((char *)Addr, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
                                            AeInfo.PrvEVValue, AeInfo.PrvLV);

    Addr += sprintf((char *)Addr, "CapEV_Value = \t%ld\r\nCapLV = \t%ld\r\n",
                                            AeInfo.CapEVValue, AeInfo.CapLV);

    Addr += sprintf((char *)Addr, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
                                            PrvNewAeArg.ISOGain, PrvNewAeArg.ExpoTime, PrvNewAeArg.Iris);

    Addr += sprintf((char *)Addr, "Cap_ISOGain = \t%ld\r\nCap_ExpoTime = \t%ld\r\nCap_Iris = \t%ld\r\n",
                                            CapNewAeArg.ISOGain, CapNewAeArg.ExpoTime, CapNewAeArg.Iris);
    return Addr;
}

void AE_GetHisto(UINT32 *Hist)
{
    UINT32 i;
    for (i = 0; i < AE_HIST_NUM; i ++)
    {
        Hist[i] = AeInfo.Hist[i];
    }
}

#if 0
void AE_Cap2Prv(void)
{
    EXPOSURE_SETTING expoinfo;
    AE_setFlag((AE_FLAG_SET_IRIS|AE_FLAG_SET_GAIN|AE_FLAG_SET_EXPT));
    AE_setExpT(PrvNewAeArg.ExpoTime);
    Sensor_GetExpoSetting(&expoinfo);
    AE_setGain(PrvNewAeArg.ISOGain*expoinfo.uiExpoRatio/100);
    AE_setIris(PrvNewAeArg.Iris);
}
#endif

UINT32 AE_GetPrvEVValue(void)
{
    return AEAlg_Conv2LV(AeInfo.PrvEVValue);
}

UINT32 AE_GetCapEVValue(void)
{
    return AEAlg_Conv2LV(AeInfo.CapEVValue);
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

UINT32 AE_getStatisticInfo(UINT32 HistSize, UINT32 *Hist)
{
#if 0
    return 50;
#else
    UINT32 i, j;
    UINT32 RTotal, GTotal, BTotal, WTotal;//, Lum;
    UINT32 Ratio_X, Ratio_Y, HistInterval;

    Ratio_X = CA_WIN_X/AE_WIN_X;
    Ratio_Y = CA_WIN_Y/AE_WIN_Y;
    if(Ratio_X <1)
        Ratio_X = 1;
    if(Ratio_Y <1)
        Ratio_Y = 1;

    Img_GetCA_Result(IPL_ID_1,Racc,Gacc,Bacc,CA_WIN_X*CA_WIN_Y);

    //calculate ae weight
    RTotal = 0;
    GTotal = 0;
    BTotal = 0;
    WTotal = 0;
    memset(Hist, 0, AE_HIST_NUM*4);
    HistInterval = (1<<12)/AE_HIST_NUM;

    for (j = 0; j < CA_WIN_Y; j ++)
    {
        for (i = 0; i < CA_WIN_X; i ++)
        {
            GTotal += (AeWin[j/Ratio_Y][i/Ratio_X] * Gacc[j * CA_WIN_X + i]);
            WTotal += AeWin[j/Ratio_Y][i/Ratio_X];

            Hist[Gacc[j * CA_WIN_X + i]/HistInterval] ++;
        }
    }
    return (GTotal/WTotal);
#endif
}


void AE_getInfo(AE_INFO *AeInfo)
{
    UINT32 i;

    AeInfo->EVAdjEn = ENABLE;
    AeInfo->ReduceOverExpoModeEn = AE_REDUCE_OVER_EXPO_MODE;
    AeInfo->CovRangeH = AE_COVER_RANGE_H;
    AeInfo->CovRangeL = AE_COVER_RANGE_L;
    AeInfo->HistBinNum = AE_HIST_NUM;
    AeInfo->Hist = AeHistBuf;
    AeInfo->PrvBiningMode = AE_getPrvBiningMode();
    AeInfo->CapBiningMode = AE_getCapBiningMode();
    AeInfo->AdjRatioH = AE_ADJUST_RATIO_H;
    AeInfo->AdjRatioL = AE_ADJUST_RATIO_L;
    AeInfo->EVPrvWeight = AE_SMOOTH_PRV_W;
    AeInfo->EVNewWeight = AE_SMOOTH_NEW_W;
    AeInfo->ExpectY = AE_EXPECT_Y;
    AeInfo->Reset = TRUE;
    AeInfo->MinEV = AE_MIN_EV;
    AeInfo->MaxEV = AE_MAX_EV;
    AeInfo->MinISOValue = AE_MIN_ISO;
    AeInfo->MaxISOValue = AE_MAX_ISO;
    AeInfo->OverExpoInfo.AdjYStep = AE_ADJ_Y_STEP;
    AeInfo->OverExpoInfo.OutdoorEV = AE_ADJ_OUTDOOR_EV;
    AeInfo->OverExpoInfo.IndoorEV = AE_ADJ_INDOOR_EV;
    AeInfo->OverExpoInfo.OutdoorMaxAdjY = AE_ADJ_OUTDOOR_MAX_Y;
    AeInfo->OverExpoInfo.IndoorMaxAdjY = AE_ADJ_INDOOR_MAX_Y;
    AeInfo->OverExpoInfo.OverExpoTh = AE_ADJ_OVER_EXPO_TH;
    AeInfo->OverExpoInfo.UnderExpoTh = AE_ADJ_UNDER_EXPO_TH;
    AeInfo->OverExpoInfo.AdjY = 0;
    AeInfo->OverExpoInfo.AdjExpextY = 0;
    for (i = 0; i < AEALG_BUF_SIZE; i ++)
    {
        AeInfo->OverExpoInfo.Data[i] = 0x0; //data must be set zero at frist time
    }
    AeInfo->Counter = 0;
    AeInfo->EventTab = AeAlgEventTab;
}

AE_RESET AE_setWin(AE_WIN mode)
{
    static AE_WIN prvmode = 0xFF;
    if (mode == prvmode)
    {
        return AE_RESET_FALSE;
    }
    DBG_ERR("AE_WIN mode = %d\r\n", mode);
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

void AE_getCapTab(AE_ISO nISO, AE_FLICKER nFlicker, AE_SCENE nScene, AE_INFO *AeInfo)
{
    static UINT32 scene=0xff,iso=0xff,flicker=0xff;

    if((scene == nScene)&&(iso == nISO)&&(flicker == nFlicker))
    {
        return;
    }
    if(nFlicker == AE_FLICKER_50HZ)
    {
        switch(nISO)
        {
            case AE_ISO_50:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO50_50Hz;
                break;
            case AE_ISO_80:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO80_50Hz;
                break;
            case AE_ISO_100:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO100_50Hz;
                break;
            case AE_ISO_200:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO200_50Hz;
                break;
            case AE_ISO_400:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO400_50Hz;
                break;
            case AE_ISO_800:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO800_50Hz;
                break;
            case AE_ISO_1600:
            case AE_ISO_3200:
            case AE_ISO_6400:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO1600_50Hz;
                break;

            case AE_ISO_AUTO:
            default:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_Cap50;
                break;
        }
    }
    else//if(nFlicker==AE_FLICKER_60HZ)
    {
        switch(nISO)
        {
            case AE_ISO_50:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO50_60Hz;
                break;
            case AE_ISO_80:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO80_60Hz;
                break;
            case AE_ISO_100:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO100_60Hz;
                break;
            case AE_ISO_200:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO200_60Hz;
                break;
            case AE_ISO_400:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO400_60Hz;
                break;
            case AE_ISO_800:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO800_60Hz;
                break;
            case AE_ISO_1600:
            case AE_ISO_3200:
            case AE_ISO_6400:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO1600_60Hz;
                break;
            case AE_ISO_AUTO:
            default:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_Cap60;
                break;
        }
    }
    if (nScene == AE_SCENE_SPORT)
    {
        AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_Action;
    }
    else if (nScene == AE_SCENE_HIGHISO)
    {
        if(nFlicker == AE_FLICKER_50HZ)
        {
            AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO1600_50Hz;
        }
        else
        {
            AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO1600_60Hz;
        }
    }

    if (nScene != AE_ANTISHAKE_LV0)
    {
        if(nFlicker == AE_FLICKER_50HZ)
        {
            AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO800_50Hz;
        }
        else
        {
            AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO800_60Hz;
        }
    }

    scene = nScene;
    iso = nISO;
    flicker = nFlicker;
    AE_reset();
}

AE_RESET AE_getPrvTab(AE_ISO nISO, AE_FLICKER nFlicker, AE_INFO *AeInfo)
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
                AeInfo->MaxEV = AE_MAX_EV;
                AeInfo->MinEV = AE_MIN_EV;
                AeInfo->MaxISOValue = AE_MAX_ISO;
                AeInfo->MinISOValue = AE_MIN_ISO;
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
                AeInfo->MaxEV = AE_MAX_EV;
                AeInfo->MinEV = AE_MIN_EV;
                AeInfo->MaxISOValue = AE_MAX_ISO;
                AeInfo->MinISOValue = AE_MIN_ISO;
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

    if ( IfeBinning == 0 )
    {
        Cnt = (Hist[60] + Hist[61] + Hist[62] + Hist[63]) +
              (Hist[56] + Hist[57] + Hist[58] + Hist[59]) +
              (Hist[52] + Hist[53] + Hist[54] + Hist[55]) / 2; // >224  [224~239]+[240~225
    }
    else
    {
        Cnt = (Hist[60] + Hist[61] + Hist[62] + Hist[63]) +
              (Hist[56] + Hist[57] + Hist[58] + Hist[59]) +
              (Hist[52] + Hist[53] + Hist[54] + Hist[55]) +
              (Hist[48] + Hist[49] + Hist[50] + Hist[51]) +
              (Hist[44] + Hist[45] + Hist[46] + Hist[47]) +
              (Hist[40] + Hist[41] + Hist[42] + Hist[43]) +
              0
              ;
    }
    overExpoCnt = Cnt;
    return Cnt;
}

UINT32 GetFnoRatio(AEALG_IRIS_IDX Idx)
{
    return 100;
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
        Sensor_SetExpTime(SENSOR_ID_1,ExpTSetting);
        AE_clrFlag(AE_FLAG_SET_EXPT);

    }
}

void AE_setGain(GAIN_SETTING *Gain)
{
    if ((AE_getFlag() & AE_FLAG_SET_GAIN) == AE_FLAG_SET_GAIN)
    {
        Sensor_SetGain(SENSOR_ID_1,Gain);
        AE_clrFlag(AE_FLAG_SET_GAIN);
    }
}

void AE_setIris(AEALG_IRIS_IDX Idx)
{
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
        IPC_WaitVD(FALSE,IPL_ID_1);
    }
    return FALSE;
}

void AE_StartAFD(void)
{
    AEAFD_START = TRUE;
}

void AE_GetExifInfo(UINT32 *ISO, UINT32 *ExpT, UINT32 *Aperture, UINT32 *MaxApture)
{
    *ExpT = CapNewAeArg.ExpoTime;
    *ISO = CapNewAeArg.ISOGain;
    *Aperture = 18;
    *MaxApture = 18;
}

UINT32 AEB_GetRatioFromEV(SEL_AEEV AEB_Step)
{
    AEB_Ratio = EVTable[AEB_Step];
    return AEB_Ratio;
}

void AEB_SetRatio2Cap(UINT32 Ratio)
{

    AeInfo.CapEVValue = AeInfo.PrvEVValue*AE_EV_RATIO/Ratio;

    AEAlg_Allot(AeInfo.CapEVValue, AeInfo.CapCurve, AeInfo.EventTab.GetFnoRatio, AeInfo.EventTab.GetCurveOfsRatio, &CapNewAeArg);
}

/*--------------------------------------------------------*/

UINT32 AE_getCurLum(UINT32 LumBit)
{
    UINT32 Lum;

    if(LumBit < 12)
        Lum = AeInfo.LumY >> (12-LumBit);
    else
        Lum = AeInfo.LumY << (LumBit - 12);

    return Lum;
}



extern UINT32 AEAFD_ADJ_BlockMax, AEAFD_ADJ_BlockMin;
extern UINT32 AEAFD_ADJ_BlockCnt, AEAFD_ADJ_BlockAvg;

static UINT32 AE_AutoFlickerDetectProc(void)
{
    DBG_IND("AEAFD_START start\r\n");
    UINT32 Flag;
    SENSOR_MODE Mode;
    EXPOSURE_SETTING ExpTSetting;
    GAIN_SETTING Gain;

    AEAFD_FRAME AfdFrame;
    UINT32 AfdExpTime, VTotal, HTotal;
    UINT32 FrameCnt;
    UINT32 CAWinXNum, CAWinYNum, SieCropSizeH, SieCropSizeV;

    if ( AeInfo.LumY < (AEAFD_PROC_YLUM_MIN) || AeInfo.LumY > (AEAFD_PROC_YLUM_MAX) )
    {
        DBG_ERR("AvgY(%d) out of range(%d %d)\r\n", AeInfo.LumY, AEAFD_PROC_YLUM_MAX, AEAFD_PROC_YLUM_MIN);
        goto AEAFD_PROC_END;
    }

    //AEAFD_SetStatus(AEAFD_STATUS_READY);

    //calculate ae
    if ( AE_getFlickerType(IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY)) == AE_FLICKER_50HZ )
        AEAFD_GetAESetting(AEAFD_FLICKER_50Hz, &AfdExpTime, &AfdFrame);
    else
        AEAFD_GetAESetting(AEAFD_FLICKER_60Hz, &AfdExpTime, &AfdFrame);
    if ( AE_setFrame(AfdFrame, &HTotal, &VTotal) != E_OK )
    {
        DBG_ERR("AEAFD: sensor dont support to set frame\r\n");
        AEAFD_START = FALSE;
        goto AEAFD_PROC_END;
    }

    PrvNewAeArg.ExpoTime = AfdExpTime;
    PrvNewAeArg.ISOGain = PrvOldAeArg.ExpoTime * PrvOldAeArg.ISOGain / PrvNewAeArg.ExpoTime;

    DBG_IND("AE_FLICKER AE= %d %d\r\n", PrvOldAeArg.ExpoTime, PrvOldAeArg.ISOGain);
    DBG_IND("AE_FLICKER AE= %d %d\r\n", PrvNewAeArg.ExpoTime, PrvNewAeArg.ISOGain);

    AE_judgePrvSetting(&PrvNewAeArg, &PrvOldAeArg, &Flag);

    Sensor_GetCurMode(SENSOR_ID_1, &Mode);
    Sensor_GetExpoSetting(SENSOR_ID_1, PrvNewAeArg.ExpoTime, Mode, &ExpTSetting);
    Sensor_GetGainSetting(SENSOR_ID_1, PrvNewAeArg.ISOGain, &Gain);

    AE_setFlag(Flag);
    DBG_IND("AE Flag= %x\r\n", Flag);
    DBG_IND("ExpTSetting = %d  %d  %d\r\n", ExpTSetting.uiFrame, ExpTSetting.uiLine, ExpTSetting.uiPixel);
    AE_setExpT(&ExpTSetting);
    AE_setGain(&Gain);

    AE_GetCASetInfo(&CAWinXNum, &CAWinYNum, &SieCropSizeH, &SieCropSizeV);
    AEAFD_INFO aInfo;
    aInfo.VTotal = VTotal;
    aInfo.HTotal = HTotal;
    aInfo.CAWinSizeX = CAWinXNum;
    aInfo.CAWinSizeY = CAWinYNum;
    aInfo.CAWinStartX = AEAFD_CAStartX;
    aInfo.CAWinStartY = AEAFD_CAStartY;
    aInfo.CABuffer = (UINT32)AfdBuffer;
    aInfo.CABlkWidth = (UINT32)(SieCropSizeH/CAWinXNum);
    aInfo.CABlkHeight = (UINT32)(SieCropSizeV/CAWinYNum);
    aInfo.DebugFlag = AEAFD_FLG_DEBUG_LOG | AEAFD_FLG_ADJUST;
    aInfo.ScalingFactor = AEAFD_CAScalingFactor;
    aInfo.YLowerLimit = AEAFD_YLowerLimit;
    aInfo.YUpperLimit = AEAFD_YUpperLimit;
    aInfo.FOMiThreshold = AEAFD_FOMiThreshold;
    aInfo.FOMsThreshold = AEAFD_FOMsThreshold;
    aInfo.DetectUpperBound = AEAFD_DetectUpperBound;
    aInfo.DetectLowerBound = AEAFD_DetectLowerBound;
    AEAFD_Init( &aInfo);

AEAFD_PROC_LOOP:

    IPC_WaitVD(FALSE,IPL_ID_1);
    IPC_WaitVD(FALSE,IPL_ID_1);
    IPC_WaitVD(FALSE,IPL_ID_1);

    for ( FrameCnt=0; FrameCnt<AEAFD_DETECT_CYCLE; FrameCnt++ )
    {
        IPC_WaitVD(FALSE,IPL_ID_1);
        Img_GetCA_Result(IPL_ID_1, Racc,Gacc,Bacc,CAWinXNum*CAWinYNum);
        memcpy(AfdBuffer+FrameCnt*CAWinXNum*CAWinYNum, Gacc, sizeof(UINT16)*CAWinXNum*CAWinYNum);
    }

    AEAFD_process();

    //DBG_ERR("AE_FLICKER = %d\r\n", AE_getFlickerType(IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY)));
    DBG_IND("AE_FLICKER = %d\r\n", AEAFD_GetRslt());

    if ( aInfo.DebugFlag & AEAFD_FLG_ADJUST )
    {
#if 0
        char pString[512];
        SenFP_ClearOSD();
        sprintf(pString, "%d %d %d %d", AEAFD_ADJ_BlockMax, AEAFD_ADJ_BlockMin, AEAFD_ADJ_BlockAvg, AEAFD_ADJ_BlockCnt);
        SenFP_ShowOSDString(pString, 20, 80, 4);
#endif

        goto AEAFD_PROC_LOOP;
    }

    AEAFD_START = FALSE;

    PrvNewAeArg.ExpoTime = PrvOldAeArg.ExpoTime ;
    PrvNewAeArg.ISOGain = PrvOldAeArg.ISOGain;
    Flag |= AE_FLAG_SET_EXPT;
    Flag |= AE_FLAG_SET_GAIN;
    Sensor_GetCurMode(SENSOR_ID_1, &Mode);
    Sensor_GetExpoSetting(SENSOR_ID_1, PrvNewAeArg.ExpoTime, Mode, &ExpTSetting);
    Sensor_GetGainSetting(SENSOR_ID_1, PrvNewAeArg.ISOGain, &Gain);
    AE_setFlag(Flag);
    AE_setExpT(&ExpTSetting);
    AE_setGain(&Gain);
    IPC_WaitVD(FALSE,IPL_ID_1);
    IPC_WaitVD(FALSE,IPL_ID_1);
    IPC_WaitVD(FALSE,IPL_ID_1);
    Flag = 0;
AEAFD_PROC_END:;
    DBG_IND("AEAFD_START end\r\n");
    return TRUE;
}

