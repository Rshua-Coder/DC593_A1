/**
    Auto white balance flow.

    ae flow.

    @file       awb_sample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <stdio.h>

#include "type.h"
#include "Awb_alg.h"
#include "Ae_alg.h"
#include "Awb_IMX322LQJ_EVB_FF_int.h"
#include "Awb_Api.h"
#include "Ae_Api.h"
#include "IPL_AlgInfor.h"
#include "IPL_Cmd.h"
#include "IQS_SettingFlow.h"
#include "pstore.h"
#include "fd_lib.h"

#define __MODULE__ AWB
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

#if 0
extern void SenFP_ShowOSDString(CHAR *pString, UINT16 uiX, UINT16 uiY, UINT16 uiColor);
extern void SenFP_ClearOSD(void);
#endif

UINT32 AWBUartMsg = DISABLE;
extern AWB_MODE AWB_getMode(UINT32 Idx);
extern void MWB_ReadColorGain(SEL_WBMODE mode, UINT32 *Rg,UINT32 *Gg,UINT32 *Bg);
extern void MWB_WriteColorGain(SEL_WBMODE mode, UINT32 Rg,UINT32 Gg,UINT32 Bg);
extern ER MWB_CalcCustomerColorGain(SEL_WBMODE mode, UINT32 *RGain,UINT32 *GGain,UINT32 *BGain);
void AWB_manualProc(AWB_MODE Idx);
void AWB_setGain(UINT32 RG, UINT32 GG, UINT32 BG);
void AWB_GetColorGian(UINT32 *Rg,UINT32 *Gg,UINT32 *Bg);
//void AWB_GetGWColorGain(UINT32 *Rg,UINT32 *Gg,UINT32 *Bg);

static AWBALG_INFO AwbInfo = {0};
static UINT32 AccTabR[AWB_WIN_TOTAL];
static UINT32 AccTabG[AWB_WIN_TOTAL];
static UINT32 AccTabB[AWB_WIN_TOTAL];
static UINT32 CalTabR[AWB_WIN_TOTAL];
static UINT32 CalTabG[AWB_WIN_TOTAL];
static UINT32 CalTabB[AWB_WIN_TOTAL];
static UINT32 WTab[AWB_WIN_TOTAL];
static UINT32 FlagTab[AWB_WIN_TOTAL];

static UINT32 AWBEnter = 1;
static UINT32 OldRGain, OldGGain, OldBGain;
static UINT32 RGain, GGain, BGain;
static UINT32 AWBCnt = 0;
static UINT32 DefRGain=AWB_DEF_RGAIN, DefGGain=AWB_DEF_GGAIN, DefBGain=AWB_DEF_BGAIN;
//static UINT32 GWRGain, GWGGain, GWBGain;
UINT32 AWB_CalInit = FALSE;
void AWB_GetCurCT(UINT32 id, UINT32 CurRgain, UINT32 CurBgain, UINT32 *CT);
UINT32 AWB_Intpl(int Index, int LowValue, int HighValue, int MinIndex, int MaxIndex);
UINT32 AWB_DumpCAInfo(UINT32 id);
UINT32 AWB_DumpFlagInfo(UINT32 id);
void AWB_get_mov_dbg(UINT16* result);

#define TBL_GREEN_MAX 1
UINT32 tblGreen[TBL_GREEN_MAX][3] =
{
    {143, 211, 500},
};
static UINT32 fpUserChkWhite(UINT32 R, UINT32 G, UINT32 B)
{
    return 100;
}


static UINT32 fpUserAdjGain(UINT32 *Rgain, UINT32 *Bgain)
{
    UINT32 w = 0;
    return w;
}
UINT32 AWB_Intpl(int Index, int LowValue, int HighValue, int MinIndex, int MaxIndex)
{
    if ( Index < MinIndex)
        return LowValue;
    else if ( Index > MaxIndex)
        return HighValue;
    else
    {
        int range = MaxIndex - MinIndex;
        if(HighValue < LowValue)
            return LowValue +((HighValue - LowValue) * (Index  - MinIndex)-(range>>1)) / range;
        else
            return LowValue +((HighValue - LowValue) * (Index  - MinIndex)+(range>>1)) / range;
    }
}
#if 0
static void AWB_SetMovieLog(char* Buf, UINT32 BufLen)
{
    char msg[64];
    memset(msg, 0, sizeof(char)*64);
    sprintf(msg, "%d %d ", RGain, BGain);
    memcpy(Buf, msg, BufLen-1);
}
#endif


UINT32 AWB_DumpCAInfo(UINT32 id)
{
    UINT32 i, j;
	UINT32 awb_WIN_X = AwbInfo.WinNumX;
    UINT32 awb_WIN_Y = AwbInfo.WinNumY;

    for(j = 0; j < awb_WIN_Y; j++)
    {
        DBG_DUMP("RGB  \t: ");

        for (i = 0; i < awb_WIN_X; i++)
        {
            DBG_DUMP("%3ld %3ld %3ld\t",AwbInfo.AccTab[AWBALG_CH_R][j*AWB_WIN_Y+i],AwbInfo.AccTab[AWBALG_CH_G][j*AWB_WIN_Y+i],AwbInfo.AccTab[AWBALG_CH_B][j*AWB_WIN_Y+i] );
        }

        DBG_DUMP( "\r\n");
    }
    return 0;
}

UINT32 AWB_DumpFlagInfo(UINT32 id)
{
    UINT32 i, j;
    UINT32 awb_WIN_X = AwbInfo.WinNumX;
    UINT32 awb_WIN_Y = AwbInfo.WinNumY;

    for(j = 0; j < awb_WIN_Y; j++)
    {
        DBG_DUMP("RGB  \t: ");

        for (i = 0; i < awb_WIN_X; i++)
        {
            DBG_DUMP("%3ld\t",AwbInfo.FlagTab[j*AWB_WIN_Y+i]);
        }

        DBG_DUMP( "\r\n");
    }
    return 0;
}
void AWB_Init(UINT32 CurrentStatus)
{
	static BOOL Init_Flag = FALSE;
//#NT#2013/09/05#Spark Chou -begin
//#NT#
#if 1//lydia 20150504
    AWB_SetAlgMode(AWBALG_MODE_NVTALG_R2);
    AwbInfo.FuncCtrl = (AWB_FUNCCTRL_R2_PPROC_EN | AWB_FUNCCTRL_R2_CTTAB_EN | AWB_FUNCCTRL_R2_STABLE_EN | AWB_FUNCCTRL_R2_MULTICT_EN);
#else
    AWB_SetAlgMode(AWBALG_MODE_NVTALG_R1);
    AwbInfo.FuncCtrl = (AWB_FUNCCTRL_R1_PPROC_EN | AWB_FUNCCTRL_R1_CTTAB_EN | AWB_FUNCCTRL_R1_STABLE_EN);// | AWB_FUNCCTRL_R2_MULTICT_EN);
#endif

    #if AWB_REFWHITE_REMAP
    //DBG_IND("AWB Alg REMAP Enable\r\n");
    AwbInfo.FuncCtrl |= AWB_FUNCCTRL_R2_REMAP_EN;
    AWBALG_RGB_INFO p1 = {28, 62, 46};
    AWBALG_RGB_INFO p2 = {23, 45, 28};
    AWBALG_RGB_INFO p3 = {42, 48, 18};
    AwbInfo.AwbRemapInfo.HCTInfo = p1;
    AwbInfo.AwbRemapInfo.MCTInfo = p2;
    AwbInfo.AwbRemapInfo.LCTInfo = p3;
    AWBALG_Remap_Init(&AwbInfo);
    #endif
//#NT#2013/09/05#Spark Chou -end
AwbInfo.KRgain = AwbInfo.KBgain = 100;
#if 0
    CAL_DATA CalInfo;
    Cal_GetCalData(&CalInfo);

    if( (CalInfo.CalStatus_AWBGS == _AWB_Status) && (CalInfo.CalStatus_AWB == _AWB_Status))
    {
        AwbInfo.KRgain = CalInfo.Cal_AWBGain.Rgain*100/CalInfo.Cal_AWBGSGain.Rgain;
        AwbInfo.KBgain = CalInfo.Cal_AWBGain.Bgain*100/CalInfo.Cal_AWBGSGain.Bgain;
    }
#endif

#if 0//CAL_AWB
    if ( AWB_CalInit == FALSE )
    {
        pCAL_TAG pTag;
        CAL_AWB_FMT* pFmt;

        pTag = GetCalData(SEC_AWB,IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));

        AwbInfo.KRgain = AWB_DEF_KRGAIN;
        AwbInfo.KBgain = AWB_DEF_KBGAIN;

        if (pTag != NULL)
        {
            DBG_IND("get AWB Calibration data\r\n");
            if(pTag->CAL_STATUS == _CAL_OK)
            {
                pFmt = (CAL_AWB_FMT*)&pTag->TagData;

                if (pFmt->Rg != 0 && pFmt->Bg != 0)
                {
                    AwbInfo.KRgain = pFmt->Rg;
                    AwbInfo.KBgain = pFmt->Bg;
                }
                else
                {
                    DBG_ERR("read AWB Calibration data Fail\r\n");
                    AwbInfo.KRgain = AWB_DEF_KRGAIN;
                    AwbInfo.KBgain = AWB_DEF_KBGAIN;
                }
                DBG_IND("AWB Cal Rg=%d, Bg=%d, SenMode=%d\r\n", pFmt->Rg,pFmt->Bg,IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));
            }
            else
            {
                DBG_ERR("cal status not OK\r\n");
                AwbInfo.KRgain = AWB_DEF_KRGAIN;
                AwbInfo.KBgain = AWB_DEF_KBGAIN;
            }
        }
        else
        {
            DBG_ERR("NO AWB cal data\r\n");
            AwbInfo.KRgain = AWB_DEF_KRGAIN;
            AwbInfo.KBgain = AWB_DEF_KBGAIN;
        }
        AWB_CalInit = TRUE;
    }
#else
    AwbInfo.KRgain = AWB_DEF_KRGAIN;
    AwbInfo.KBgain = AWB_DEF_KBGAIN;
#endif
    if ( Init_Flag == FALSE )
    {
        Init_Flag = TRUE;
        DefRGain = AWB_DEF_RGAIN*AwbInfo.KRgain/100;
        DefGGain = 256;
        DefBGain = AWB_DEF_BGAIN*AwbInfo.KBgain/100;
    }
    else
    {
        DefRGain = RGain;//AwbInfo.AWBGain.RGain;
        DefGGain = GGain;
        DefBGain = BGain;//AwbInfo.AWBGain.BGain;
    }
    AwbInfo.PreGain.RGain = DefRGain;
    AwbInfo.PreGain.BGain = DefBGain;
    RGain = DefRGain;//AwbInfo.AWBGain.RGain;
    GGain = DefGGain;
    BGain = DefBGain;
    //AwbInfo.AWBGain = AwbInfo.PreGain;
    AwbInfo.OutlR = AWB_OUTL_R;
    AwbInfo.OutlB = AWB_OUTL_B;
    AwbInfo.WinNumX = AWB_WIN_X;
    AwbInfo.WinNumY = AWB_WIN_Y;
    AwbInfo.AccTab[AWBALG_CH_R] = AccTabR;
    AwbInfo.AccTab[AWBALG_CH_G] = AccTabG;
    AwbInfo.AccTab[AWBALG_CH_B] = AccTabB;
    AwbInfo.CalTab[AWBALG_CH_R] = CalTabR;
    AwbInfo.CalTab[AWBALG_CH_G] = CalTabG;
    AwbInfo.CalTab[AWBALG_CH_B] = CalTabB;
    AwbInfo.FlagTab = FlagTab;
    AwbInfo.WTab = WTab;
    AwbInfo.EventFp.UserChkWhite = fpUserChkWhite;
    AwbInfo.EventFp.UserAdjGain  = fpUserAdjGain;

    AwbInfo.StableGain.Tolerance = 268;  //5%
    AwbInfo.StableGain.Score = 0;
    AwbInfo.StableGain.RGain = DefRGain;
   // AwbInfo.StableGain.GGain = 256;
    AwbInfo.StableGain.BGain = DefBGain;

    AwbInfo.MinWTotal = AWB_W_THRES;


    //--------------------------------------------------
    AwbInfo.CTParam = &AwbCTParam;
    AwbInfo.PostParam = &AwbPostParam;

    AwbInfo.Tab = AwbWhiteTAB;
    AwbInfo.TabCnt = AWB_WHITE_TAB_CNT;

    AwbInfo.PDLightTab = AwbPDLTAB;
    AwbInfo.PDLightTabCnt = AWB_PDL_TAB_CNT;
    //--------------------------------------------------

    if (AwbInfo.AwbConvSpeed == 0)
    {
        AwbInfo.AwbConvSpeed = 4;
    }
    if (AwbInfo.AwbConvStep == 0)
    {
        AwbInfo.AwbConvStep = 10;
    }
    AWBCnt = 0;
    AWBEnter = 1;
    AwbInfo.AwbWeightEn = TRUE;
    RGain = OldRGain = DefRGain;//*AwbInfo.KRgain/100;;
    GGain = OldGGain = DefGGain;
    BGain = OldBGain = DefBGain;//*AwbInfo.KBgain/100;
 //   DBG_IND("ini Gain = %d %d %d\r\n", RGain, GGain, BGain);
 //   DBG_IND("ini old Gain = %d %d %d\r\n", OldRGain, OldGGain, OldBGain);







 //   IQS_SetMovieLogFP(AWB_SetMovieLog);
}
/*
static void AWB_ChkLowCT(UINT32* Rg,UINT32* Bg)
{
    UINT32 RB_AdjM = 300,RB_Adjm = 180;
    UINT32 R_AdjRng = 25;
    UINT32 B_AdjRng = 30;
    UINT32 ratio=0;

    if( ((*Bg)*100/(*Rg)) >  RB_Adjm)//R*100/B
    {
        ratio = (((*Bg)*100/(*Rg)) - RB_Adjm)*100 /(RB_AdjM- RB_Adjm);
        R_AdjRng = R_AdjRng*ratio/100;
        B_AdjRng = B_AdjRng*ratio/100;
        (*Bg) = (*Bg)*(100-B_AdjRng)/100;
        (*Rg) = (*Rg)*(R_AdjRng+100)/100;
    }
}*/

void  AWB_GetCA_GrayWorldGain(AWBALG_INFO *pAWBInfo, UINT32 *RGain, UINT32 *GGain, UINT32 *BGain)
{
    UINT32 i;
    UINT32 RTotal, GTotal, BTotal, WTotal;

    RTotal = 0;
    GTotal = 0;
    BTotal = 0;
    WTotal = 0;
    for (i = 0; i < pAWBInfo->WinNumX*pAWBInfo->WinNumY; i ++)
    {
        {
            RTotal += (pAWBInfo->AccTab[AWBALG_CH_R][i]);
            GTotal += (pAWBInfo->AccTab[AWBALG_CH_G][i]);
            BTotal += (pAWBInfo->AccTab[AWBALG_CH_B][i]);
            WTotal += 1;
        }
    }
    *RGain = 256 * GTotal / RTotal;
    *GGain = 256;
    *BGain = 256 * GTotal / BTotal;
}


void AWB_Process(UINT32 CurrentStatus)
{
    AWB_MODE Mode;
   // UINT32 AutoRGain = RGain;
  //  UINT32 AutoBGain = BGain;
 //   UINT32 fRGain, fGGain, fBGain, fCnt;

    AWBCnt ++;
    if (0 != (AWBCnt % 4))
    {
        return;
    }
    else
    {
        DBG_IND("AWB_Process in %d\r\n", AWBCnt);
    }

    Mode = AWB_getMode(IPL_AlgGetUIInfo(IPL_SEL_WBMODE));

    if(Mode == AWB_MDOE_AUTO)
    {
        AwbInfo.CurEV =  1 << ((UINT32)(AE_GetPrvEVValue()/10));
        AwbInfo.CTParam = &AwbCTParam;
        AwbInfo.PostParam = &AwbPostParam;
        AwbInfo.Tab = AwbWhiteTAB;
        AwbInfo.TabCnt = AWB_WHITE_TAB_CNT;
        AwbInfo.PDLightTab = AwbPDLTAB;
        AwbInfo.PDLightTabCnt = AWB_PDL_TAB_CNT;

        AWB_getCA(AwbInfo.AccTab[AWBALG_CH_R], AwbInfo.AccTab[AWBALG_CH_G], AwbInfo.AccTab[AWBALG_CH_B], AWB_WIN_TOTAL);

        AWB_autoProc(&AwbInfo);

   //     AutoRGain = RGain;
    //    AutoBGain = BGain;
//#NT#2013/09/05#Spark Chou -begin
//#NT#
    /*    if ( (AwbInfo.FuncCtrl & AWB_FUNCCTRL_FACEMATCH_EN)==AWB_FUNCCTRL_FACEMATCH_EN)
        {
            //FaceColor Auto Adjust
            URECT TargetCoord;
            UINT32 FDCnt = 0, i;
            FD_FACE Face_rslt[FD_NUM_MAX];
            TargetCoord.x = 0;
            TargetCoord.y = 0;
            TargetCoord.w = AWB_FACEIMG_WIDTH;
            TargetCoord.h = AWB_FACEIMG_HEIGHT;
            FDCnt = FD_GetRsltFace(Face_rslt, &TargetCoord);
            AwbInfo.ColorBlockCnt = FDCnt;
            for ( i=0; i<FDCnt; i++ )
            {
                AwbInfo.ColorBlockTab[i].x = Face_rslt[i].x;
                AwbInfo.ColorBlockTab[i].y = Face_rslt[i].y;
                AwbInfo.ColorBlockTab[i].w = Face_rslt[i].w;
                AwbInfo.ColorBlockTab[i].h = Face_rslt[i].h;
            }

            fRGain = fGGain = fBGain = 256;
            fCnt = AWB_faceProc(&AwbInfo, &fRGain, &fGGain, &fBGain, AWB_FACEIMG_WIDTH, AWB_FACEIMG_HEIGHT);
            if ( fCnt!=0 &&
                 (RGain)*100 > (100-FACE_COLOR_TOLERANCE)*(fRGain) && (RGain)*100 < (100+FACE_COLOR_TOLERANCE)*(fRGain) &&
                 (BGain)*100 > (100-FACE_COLOR_TOLERANCE)*(fBGain) && (BGain)*100 < (100+FACE_COLOR_TOLERANCE)*(fBGain) )
            {
                RGain = ((RGain)*AWB_FACEBASE_AUTO_WEIGHT + (fRGain)*AWB_FACEBASE_FACE_WEIGHT)/(AWB_FACEBASE_AUTO_WEIGHT+AWB_FACEBASE_FACE_WEIGHT);
                GGain = ((GGain)*AWB_FACEBASE_AUTO_WEIGHT + (fGGain)*AWB_FACEBASE_FACE_WEIGHT)/(AWB_FACEBASE_AUTO_WEIGHT+AWB_FACEBASE_FACE_WEIGHT);
                BGain = ((BGain)*AWB_FACEBASE_AUTO_WEIGHT + (fBGain)*AWB_FACEBASE_FACE_WEIGHT)/(AWB_FACEBASE_AUTO_WEIGHT+AWB_FACEBASE_FACE_WEIGHT);
            }
        }
        */
//#NT#2013/09/05#Spark Chou -end
/*
//  Lydia  NT #20150520
    CfgRg = RGain;
    CfgBg = BGain;
    AWB_ChkLowCT(&CfgRg,&CfgBg);
   AWB_setGain(CfgRg, GGain, CfgBg);
//end

        AWB_setGain(RGain, GGain, BGain);
        DefRGain = RGain;
        DefGGain = GGain;
        DefBGain = BGain;*/
    }
    else
    {
        AWB_manualProc(Mode);
    }

    if ( (AWBCnt % 12) == 0 )
    {
        if (AWBUartMsg == ENABLE)
        {
            DBG_ERR("AWBlv = %d, GAIN = (%3d %3d %3d)\r\n", AE_GetPrvEVValue(), RGain, GGain, BGain);
        }
#if 0
{
        char pString[512];
        SenFP_ClearOSD();
        UINT32 ISO, ExpT, Iris;
        AE_GetPrvAEArg(&ISO, &ExpT, &Iris);
        sprintf(pString, "%3d %3d", ExpT, ISO, AwbInfo.CurEV);
        SenFP_ShowOSDString(pString, 20, 60, 4);
        sprintf(pString, "%3d %3d", RGain, BGain);
        SenFP_ShowOSDString(pString, 20, 120, 4);
}
#endif
    }
}

void AWB_GetPrvGain(UINT32 *Rgain, UINT32 *Ggain, UINT32 *Bgain)
{
    //UINT32 CfgRg=0,CfgBg=0;
#if 1
    *Rgain = RGain;
    *Ggain = GGain;
    *Bgain = BGain;
#else
        //judge Low CT
        CfgRg = RGain;
        CfgBg = BGain;
      AWB_ChkLowCT(&CfgRg,&CfgBg);
        *Rgain = CfgRg;
        *Ggain = GGain;
        *Bgain = CfgBg;
  #endif
}

#if 0
#endif

//------------------------------------------------------------------------------

//#NT#2013/09/05#Spark Chou -begin
//#NT#
UINT32 AWB_faceProc(AWBALG_INFO *pAWBInfo, UINT32 *RGain, UINT32 *GGain, UINT32 *BGain, UINT32 wImgWidth, UINT32 wImgHeight)
{

    UINT32 ret = AWBALG_FAIL;
    UINT32 i = 0 ;
    UINT32 RSum=0, GSum=0, BSum=0, PCnt=0;
    for ( i = 0; i<pAWBInfo->ColorBlockCnt; i++ )
    {
        UINT32 fRGain, fGGain, fBGain;
        ret = AWBColorMatchSim(pAWBInfo, &fRGain, &fGGain, &fBGain,
                               FACE_COLOR_R, FACE_COLOR_G, FACE_COLOR_B,
                               &pAWBInfo->ColorBlockTab[i], wImgWidth, wImgHeight);

        if ( ret == AWBALG_OK )
        {
            RSum += fRGain;
            GSum += fGGain;
            BSum += fBGain;
            PCnt++;
        }
    }
    if ( PCnt == 0 )
        return PCnt;

    *RGain = RSum / PCnt;
    *GGain = GSum / PCnt;
    *BGain = BSum / PCnt;

    DBG_IND("Gain After ColorMatch %d %d %d\r\n", *RGain, *GGain, *BGain);

    return PCnt;
}
//#NT#2013/09/05#Spark Chou -end

UINT16 wb_dbg[13];
UINT32 ct_return;
void AWB_autoProc(AWBALG_INFO *Info)
{
    UINT32 NewRGain, NewGGain, NewBGain;
    //UINT32 GWNewRGain, GWNewGGain, GWNewBGain;
    AWBALG_ER ER_code;
    //UINT32 RGainDiff, BGainDiff, GainDiff;
	NewRGain = DefRGain;
    NewGGain = AWB_DEF_GGAIN;
	NewBGain = DefBGain;
    //GWNewGGain = AWB_DEF_GGAIN;
    //AWB_GetCA_GrayWorldGain(Info, &GWNewRGain, &GWNewGGain, &GWNewBGain);
    ER_code = AWBSim(Info, &NewRGain, &NewGGain, &NewBGain);
    wb_dbg[12] = ER_code;
    if(ER_code == AWBALG_OK)
    {
        if (AWBEnter == 1)
        {
            AWBEnter = 0;
        }
        else
        {
            OldRGain = RGain = NewRGain;
            OldGGain = GGain = NewGGain;
            OldBGain = BGain = NewBGain;
        }
    Info->PreGain.RGain=RGain;
    Info->PreGain.BGain=BGain;
	if ( (AWBCnt % 3) == 0 )
    {
        if (AWBUartMsg == ENABLE)
        {
            DBG_ERR("ER_code = %d, AWBCnt = %d, GAIN = (%3d %3d %3d)\r\n", ER_code, AWBCnt, RGain, GGain, BGain);
        }
        UINT16 i;
        UINT32 awb_flag=0;
        wb_dbg[0]=0;
        wb_dbg[1]=0;
        wb_dbg[2]=0;
        wb_dbg[3]=0;
        wb_dbg[4]=0;
        wb_dbg[5]=0;
        wb_dbg[6]=0;
        wb_dbg[7]=0;
        wb_dbg[8]=0;
        wb_dbg[9]=0;
        wb_dbg[10]=0;
        wb_dbg[11]=0;
        wb_dbg[12]=0;

        for(i = 0; i < 1024; i++)
        {
          awb_flag = AwbInfo.FlagTab[i];

          switch (awb_flag)
          {
              case AWBALG_OK:       //awb_result[0]
                if(wb_dbg[0]<=999)wb_dbg[0]++;
                break;
              case AWBALG_Y_H:      //awb_result[1]
                if(wb_dbg[1]<=999)wb_dbg[1]++;
                break;

              case AWBALG_Y_L:      //awb_result[1]
                if(wb_dbg[2]<=999)wb_dbg[2]++;
                break;

              case AWBALG_RG_H:     //awb_result[2]
                if(wb_dbg[3]<=999)wb_dbg[3]++;
                break;

              case AWBALG_RG_L:     //awb_result[2]
                if(wb_dbg[4]<=999)wb_dbg[4]++;
                break;

              case AWBALG_BG_H:     //awb_result[3]
                if(wb_dbg[5]<=999)wb_dbg[5]++;
                break;

              case AWBALG_BG_L:     //awb_result[3]
                if(wb_dbg[6]<=999)wb_dbg[6]++;
                break;

              case AWBALG_RBG_H:    //awb_result[4]
                if(wb_dbg[7]<=999)wb_dbg[7]++;
                break;

              case AWBALG_RBG_L:    //awb_result[4]
                if(wb_dbg[8]<=999)wb_dbg[8]++;
                break;

              case AWBALG_RBGSUM_H: //awb_result[5]
                if(wb_dbg[9]<=999)wb_dbg[9]++;
                break;

              case AWBALG_RBGSUM_L: //awb_result[5]
                if(wb_dbg[10]<=999)wb_dbg[10]++;
                break;

              case AWBALG_RXBG_H:   //awb_result[6]
                if(wb_dbg[11]<=999)wb_dbg[11]++;
                break;

              case AWBALG_RXBG_L:   //awb_result[6]
                if(wb_dbg[12]<=999)wb_dbg[12]++;
                break;
          }

        }

     }
    }
    else
    {
        RGain = OldRGain;
        GGain = OldGGain;
        BGain = OldBGain;
	}
       AWB_setGain(RGain, GGain, BGain);
    DefRGain = RGain;
    DefGGain = GGain;
    DefBGain = BGain;

       // GWRGain = GWNewRGain;
//GWGGain = GWNewGGain;
     //   GWBGain = GWNewBGain;
}
void AWB_get_mov_dbg(UINT16* result)
{
    result[0] = wb_dbg[0];
    result[1] = ((wb_dbg[1]+5)/10)*100+((wb_dbg[2]+5)/10);
    result[2] = ((wb_dbg[3]+5)/10)*100+((wb_dbg[4]+5)/10);
    result[3] = ((wb_dbg[5]+5)/10)*100+((wb_dbg[6]+5)/10);
    result[4] = ((wb_dbg[7]+5)/10)*100+((wb_dbg[8]+5)/10);
    result[5] = ((wb_dbg[9]+5)/10)*100+((wb_dbg[10]+5)/10);
    result[6] = ((wb_dbg[11]+5)/10)*100+((wb_dbg[12]+5)/10);
}
void AWB_manualProc(AWB_MODE Idx)
{
    RGain = MwbTAB[Idx][0];
    GGain = AWB_DEF_GGAIN;
    BGain = MwbTAB[Idx][1];
    if ( Idx != AWB_MODE_CUSTOMER1 || Idx != AWB_MODE_CUSTOMER2 || Idx != AWB_MODE_CUSTOMER3 )
    {
        RGain = RGain*AwbInfo.KRgain/100;
        BGain = BGain*AwbInfo.KBgain/100;
    }
    AWB_setGain(RGain, GGain, BGain);
    DefRGain = RGain;
    DefGGain = GGain;
    DefBGain = BGain;
}

void AWB_setGain(UINT32 RG, UINT32 GG, UINT32 BG)
{
    RGain = RG;
    GGain = GG;
    BGain = BG;

    IQS_SettingFlowFunc(IQS_AWB_END);
}

void AWB_GetColorGian(UINT32 *Rg,UINT32 *Gg,UINT32 *Bg)
{
    //UINT32 CfgRg=0,CfgBg=0;
    if((RGain==0)||(GGain==0)||(BGain==0))
    {
        *Rg = AWB_DEF_RGAIN;
        *Gg = AWB_DEF_GGAIN;
        *Bg = AWB_DEF_BGAIN;
    }
    else
    {
    #if 1
        *Rg = RGain;
        *Gg = GGain;
        *Bg = BGain;
    #else
        //judge Low CT
        CfgRg = RGain*101/100;
        CfgBg = BGain*101/100;
      AWB_ChkLowCT(&CfgRg,&CfgBg);
        *Rg = CfgRg;
        *Gg = GGain;
        *Bg = CfgBg;

   #endif
    }
}
/*
void AWB_GetGWColorGain(UINT32 *Rg,UINT32 *Gg,UINT32 *Bg)
{
        *Rg = GWRGain;
        *Gg = GWGGain;
        *Bg = GWBGain;
}
*/
void AWB_getCA(UINT32 *R, UINT32 *G, UINT32 *B, UINT32 Size)
{
    static UINT16 RAcc[AWB_WIN_TOTAL], GAcc[AWB_WIN_TOTAL], BAcc[AWB_WIN_TOTAL];
    IPL_CA_INFOR CAInfo;
    UINT16 i;
    CAInfo.Id = IPL_ID_1;
    CAInfo.R = RAcc;
    CAInfo.G = GAcc;
    CAInfo.B = BAcc;
    CAInfo.Length = Size;
    IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CAInfo);
    for ( i=0; i< AWB_WIN_TOTAL; i++ )
    {
        R[i] = (RAcc[i]>>(12-AWB_ACC_BIT));
        G[i] = (GAcc[i]>>(12-AWB_ACC_BIT));
        B[i] = (BAcc[i]>>(12-AWB_ACC_BIT));
    }
}


AWB_MODE AWB_getMode(UINT32 Idx)
{
    if (Idx == SEL_WBMODE_AUTO) return AWB_MDOE_AUTO;
    if (Idx == SEL_WBMODE_DAYLIGHT) return AWB_MODE_DAYLIGHT;
    if (Idx == SEL_WBMODE_CLOUDY) return AWB_MODE_CLOUDY;
    if (Idx == SEL_WBMODE_TUNGSTEN) return AWB_MODE_TUNGSTEN;
    if (Idx == SEL_WBMODE_FLUORESCENT1) return AWB_MODE_FLUORESCENT1;
    if (Idx == SEL_WBMODE_FLUORESCENT2) return AWB_MODE_FLUORESCENT2;
    if (Idx == SEL_WBMODE_SUNSET) return AWB_MODE_SUNSET;
    if (Idx == SEL_WBMODE_BIRTHDAYPARTY) return AWB_MODE_BIRTHDAYPARTY;
    if (Idx == SEL_WBMODE_CUSTOMER1) return AWB_MODE_CUSTOMER1;
    if (Idx == SEL_WBMODE_CUSTOMER2) return AWB_MODE_CUSTOMER2;
    if (Idx == SEL_WBMODE_CUSTOMER3) return AWB_MODE_CUSTOMER3;
    return AWB_MDOE_AUTO;
}

UINT32 AWB_WriteDebugInfo(UINT32 Addr)
{
    UINT32 i, j;
    UINT32  Rg,Gg,Bg;
    AWB_GetColorGian(&Rg, &Gg, &Bg);
    Addr += sprintf((char *)Addr, "\r\nVersion: 20160727\r\nAWBalg mode = %ld\t%08x\r\n", AWB_GetAlgMode(), AwbInfo.FuncCtrl);
    Addr += sprintf((char *)Addr, "Kgain = %ld\t%ld\r\n", AwbInfo.KRgain, AwbInfo.KBgain);
    Addr += sprintf((char *)Addr, "RGain = \t%ld\r\nGGain = \t%ld\r\nBGain = \t%ld\r\n", RGain, GGain, BGain);
  Addr += sprintf((char *)Addr, "Real RGain = \t%ld\r\nGGain = \t%ld\r\nBGain = \t%ld\r\n", Rg, Gg, Bg);
    Addr += sprintf((char *)Addr, "Flag Table:\r\n");
    for(j = 0; j < AWB_WIN_Y; j++)
    {
        Addr += sprintf((char *)Addr,"F  \t: ");

        for (i = 0; i < AWB_WIN_X; i++)
        {
            Addr += sprintf((char *)Addr,"%3ld\t", AwbInfo.FlagTab[i + (j * AWB_WIN_X)]);
        }

        Addr += sprintf((char *)Addr,"\r\n");
    }

    Addr += sprintf((char *)Addr, "Weight Table:\r\n");
    for(j = 0; j < AWB_WIN_Y; j++)
    {
        Addr += sprintf((char *)Addr,"W  \t: ");

        for (i = 0; i < AWB_WIN_X; i++)
        {
            Addr += sprintf((char *)Addr,"%3ld\t", AwbInfo.WTab[i + (j * AWB_WIN_X)]);
        }

        Addr += sprintf((char *)Addr,"\r\n");
    }

    Addr += sprintf((char *)Addr,"\t");
    for(j = 0; j < 80; j++)
        Addr += sprintf((char *)Addr,"-");
    Addr += sprintf((char *)Addr,"\r\n");

    for(j = 0; j < AWB_WIN_Y; j++)
    {
        Addr += sprintf((char *)Addr,"RGB  \t: ");

        for (i = 0; i < AWB_WIN_X; i++)
        {
            Addr += sprintf((char *)Addr,"%3ld %3ld %3ld |\t",AwbInfo.AccTab[AWBALG_CH_R][j*AWB_WIN_Y+i],AwbInfo.AccTab[AWBALG_CH_G][j*AWB_WIN_Y+i],AwbInfo.AccTab[AWBALG_CH_B][j*AWB_WIN_Y+i] );
        }

        Addr += sprintf((char *)Addr,"\r\n");
    }
    for(j = 0; j < AWB_WIN_Y; j++)
    {
        Addr += sprintf((char *)Addr,"Cal_RGB  \t: ");

        for (i = 0; i < AWB_WIN_X; i++)
        {
            Addr += sprintf((char *)Addr,"%3ld %3ld %3ld\t",
                (AwbInfo.AccTab[AWBALG_CH_R][j*AWB_WIN_Y+i] * AwbInfo.KRgain) / 100,
                AwbInfo.AccTab[AWBALG_CH_G][j*AWB_WIN_Y+i],
                (AwbInfo.AccTab[AWBALG_CH_B][j*AWB_WIN_Y+i] * AwbInfo.KBgain) / 100);
        }

        Addr += sprintf((char *)Addr,"\r\n");
    }
    return Addr;
}

ER MWB_CalcCustomerColorGain(SEL_WBMODE mode, UINT32 *RGain,UINT32 *GGain,UINT32 *BGain)
{
    IPL_CA_INFOR CAInfo;
    static UINT16 RAcc[AWB_WIN_TOTAL], GAcc[AWB_WIN_TOTAL], BAcc[AWB_WIN_TOTAL];
    UINT16 i;
    CAInfo.Id = IPL_ID_1;
    CAInfo.R = RAcc;
    CAInfo.G = GAcc;
    CAInfo.B = BAcc;
    CAInfo.Length = AWB_WIN_TOTAL;
    IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CAInfo);
    for ( i=0; i< AWB_WIN_TOTAL; i++ )
    {
        AwbInfo.AccTab[AWBALG_CH_R][i] = RAcc[i];
        AwbInfo.AccTab[AWBALG_CH_G][i] = GAcc[i];
        AwbInfo.AccTab[AWBALG_CH_B][i] = BAcc[i];
    }


    UINT32 x, y, idx;
    UINT32 RTotal, GTotal, BTotal, WTotal;

    RTotal = 0;
    GTotal = 0;
    BTotal = 0;
    WTotal = 0;
    for ( y=10; y<20; y++ )
    {
        for ( x=10; x<20; x++ )
        {
            idx =  y * AWB_WIN_X + x;
            RTotal += AwbInfo.AccTab[AWBALG_CH_R][idx];
            GTotal += AwbInfo.AccTab[AWBALG_CH_G][idx];
            BTotal += AwbInfo.AccTab[AWBALG_CH_B][idx];
            WTotal += 1;
        }
    }
    if ( RTotal == 0 || BTotal == 0 )
        return E_SYS;
    *RGain = 256 * GTotal / RTotal;
    *GGain = 256;
    *BGain = 256 * GTotal / BTotal;

    DBG_IND("MWB Customer1: %d %d %d\r\n", *RGain, *GGain, *BGain);
    return E_OK;
}

void MWB_WriteColorGain(SEL_WBMODE mode, UINT32 Rg,UINT32 Gg,UINT32 Bg)
{
    AWB_MODE AWBMode = AWB_getMode((UINT32)mode);
    MwbTAB[AWBMode][0] = Rg;
    MwbTAB[AWBMode][1] = Bg;
}

void MWB_ReadColorGain(SEL_WBMODE mode, UINT32 *Rg,UINT32 *Gg,UINT32 *Bg)
{
    AWB_MODE AWBMode = AWB_getMode((UINT32)mode);
    *Rg = MwbTAB[AWBMode][0];
    *Gg = AWB_DEF_GGAIN;
    *Bg = MwbTAB[AWBMode][1];
}

