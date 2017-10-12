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
#include "Awb_sample_int.h"
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

UINT32 AWB_CalInit = FALSE;

static UINT32 fpUserChkWhite(UINT32 R, UINT32 G, UINT32 B)
{
    return 100;
}


static UINT32 fpUserAdjGain(UINT32 *Rgain, UINT32 *Bgain)
{
    UINT32 oR, oB;
    UINT32 CurEV;
    UINT32 w = 0;
    CurEV = 1 << ((UINT32)(AE_GetPrvEVValue()/10));
    oR = *Rgain;
    oB = *Bgain;

    //Green@outdoor --> set near 5000K
#if 1
     UINT32 RxBgain = (oR*oB*100)/(256*256);
    //under shadow, include light green
    if ( CurEV > 800 && CurEV < 2048
         && ( 95 * oR < oB * 100 && oB * 100 < 140 * oR )
         && ( 266 < RxBgain && RxBgain < 600)
       )
    {
        *Rgain = 484;
        *Bgain = 411;
        w = 50;
    }
    //under sun,  exclude light green
    if ( CurEV > 2048
         && ( 95 * oR < oB * 100 && oB * 100 < 140 * oR )
         && ( 371 < RxBgain && RxBgain < 600)
       )
    {
        *Rgain = 484;
        *Bgain = 411;
        w = 50;
    }

#endif

/*
    #define AWB_LowBound_R 292
    #define AWB_LowBound_B 678
    #define AWB_LowBound_MAXEV  1024
    #define AWB_LowBound_MINEV  512
    if ( (*Bgain) * AWB_LowBound_R > AWB_LowBound_B * (*Rgain) ) //A-light (2800K)
    {
        int MinEV = AWB_LowBound_MINEV;
        int MaxEV = AWB_LowBound_MAXEV;
        int EVStep = (MaxEV-MinEV);
        if ( CurEV > MaxEV )
        {
            *Rgain = AWB_LowBound_R;
            *Bgain = AWB_LowBound_B;
            w = TRUE;
        }
        else if ( CurEV < MinEV )
        {

        }
        else
        {
            *Rgain = AWB_LowBound_R + ((int)*Rgain-AWB_LowBound_R) * (MaxEV-(int)CurEV)/EVStep;
            *Bgain = AWB_LowBound_B + ((int)*Bgain-AWB_LowBound_B) * (MaxEV-(int)CurEV)/EVStep;
            w = TRUE;
        }
    }
*/
    //DBG_IND("(Rgain, Ggain) = (%d, %d) --> (%d, %d)\n", oR, oB, *Rgain, *Bgain);

    return w;
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

void AWB_Init(UINT32 CurrentStatus)
{
//#NT#2013/09/05#Spark Chou -begin
//#NT#
    AWB_SetAlgMode(AWBALG_MODE_NVTALG_R2);
    AwbInfo.FuncCtrl = (AWB_FUNCCTRL_R2_PPROC_EN | AWB_FUNCCTRL_R2_CTTAB_EN | AWB_FUNCCTRL_R2_STABLE_EN | AWB_FUNCCTRL_R2_MULTICT_EN | AWB_FUNCCTRL_FACEMATCH_EN);

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

#if 1
    if ( AWB_CalInit == FALSE )
    {
        AwbInfo.KRgain = AWB_DEF_KRGAIN;
        AwbInfo.KBgain = AWB_DEF_KBGAIN;

        #define PS_FOCUS_DATA            "AWBD"
        PPSTORE_SECTION_HANDLE  pSection;
        UINT32 KAWBGain[2];
        //Using PStore to read / sys param
        if ((pSection = PStore_OpenSection(PS_FOCUS_DATA, PS_RDONLY)) != E_PS_SECHDLER)
        {
            PStore_ReadSection((UINT8 *)KAWBGain, 0, sizeof(KAWBGain), pSection);
            PStore_CloseSection(pSection);
            DBG_IND("Read awb calibration data ok!\r\n");
            AwbInfo.KRgain = KAWBGain[0];
            AwbInfo.KBgain = KAWBGain[1];
        }
        AWB_CalInit = TRUE;
    }
    DBG_ERR("KGain = %d %d\r\n", AwbInfo.KRgain, AwbInfo.KBgain);

#endif

#if 0
    if( (CalInfo.CalStatus_AWBGS == _AWB_Status) && (CalInfo.CalStatus_AWB == _AWB_Status))
    {
        AwbInfo.KRgain = CalInfo.Cal_AWBGain.Rgain*100/CalInfo.Cal_AWBGSGain.Rgain;
        AwbInfo.KBgain = CalInfo.Cal_AWBGain.Bgain*100/CalInfo.Cal_AWBGSGain.Bgain;
    }
#endif
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

//#NT#2013/09/05#Spark Chou -begin
//#NT#
    AwbInfo.StableGain.Tolerance = 268;  //5%
    AwbInfo.StableGain.Score = 0;
    AwbInfo.StableGain.RGain = DefRGain*AwbInfo.KRgain/100;
    AwbInfo.StableGain.BGain = DefBGain*AwbInfo.KBgain/100;

    AwbInfo.MinWTotal = AWB_W_THRES;
//#NT#2013/09/05#Spark Chou -end

    //--------------------------------------------------
    AwbInfo.CTParam = &AwbCTParam;
    AwbInfo.PostParam = &AwbPostParam;

    AwbInfo.Tab = AwbWhiteTAB;
    AwbInfo.TabCnt = AWB_WHITE_TAB_CNT;

    AwbInfo.PDLightTab = AwbPDLTAB;
    AwbInfo.PDLightTabCnt = AWB_PDL_TAB_CNT;
    //--------------------------------------------------


    AWBCnt = 0;
    AWBEnter = 1;

    RGain = OldRGain = DefRGain*AwbInfo.KRgain/100;;
    GGain = OldGGain = DefGGain;
    BGain = OldBGain = DefBGain*AwbInfo.KBgain/100;
    DBG_IND("ini Gain = %d %d %d\r\n", RGain, GGain, BGain);
    DBG_IND("ini old Gain = %d %d %d\r\n", OldRGain, OldGGain, OldBGain);

 //   IQS_SetMovieLogFP(AWB_SetMovieLog);
}


void AWB_Process(UINT32 CurrentStatus)
{
    AWB_MODE Mode;
    UINT32 AutoRGain = RGain;
    UINT32 AutoBGain = BGain;
    UINT32 fRGain, fGGain, fBGain, fCnt;

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
        AutoRGain = RGain;
        AutoBGain = BGain;
//#NT#2013/09/05#Spark Chou -begin
//#NT#
        if ( (AwbInfo.FuncCtrl & AWB_FUNCCTRL_FACEMATCH_EN)==AWB_FUNCCTRL_FACEMATCH_EN)
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
//#NT#2013/09/05#Spark Chou -end

        AWB_setGain(RGain, GGain, BGain);
        DefRGain = RGain;
        DefGGain = GGain;
        DefBGain = BGain;
    }
    else
    {
        AWB_manualProc(Mode);
    }

    if ( (AWBCnt % 12) == 0 )
    {
        if (AWBUartMsg == ENABLE)
        {
            DBG_ERR("AWBCnt = %d, GAIN = (%3d %3d %3d)\r\n", AWBCnt, RGain, GGain, BGain);
        }
#if 0
        char pString[512];
        SenFP_ClearOSD();
        sprintf(pString, "%3d %3d", AutoRGain, AutoBGain);
        SenFP_ShowOSDString(pString, 20, 40, 4);
        sprintf(pString, "%3d %3d", fRGain, fBGain);
        SenFP_ShowOSDString(pString, 20, 80, 4);
        sprintf(pString, "%3d %3d  %d", RGain, BGain, AwbInfo.CurEV);
        SenFP_ShowOSDString(pString, 20, 120, 4);
#endif
    }
}

void AWB_GetPrvGain(UINT32 *Rgain, UINT32 *Ggain, UINT32 *Bgain)
{
    *Rgain = RGain;
    *Ggain = GGain;
    *Bgain = BGain;
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

void AWB_autoProc(AWBALG_INFO *Info)
{
    UINT32 NewRGain, NewGGain, NewBGain;
    AWBALG_ER ER_code;
    UINT32 RGainDiff, BGainDiff, GainDiff;

    NewGGain = AWB_DEF_GGAIN;
    ER_code = AWBSim(Info, &NewRGain, &NewGGain, &NewBGain);
    if(ER_code == AWBALG_OK)
    {
        if (AWBEnter == 1)
        {
            AWBEnter = 0;
        }
        else
        {
            OldRGain = RGain;
            OldGGain = GGain;
            OldBGain = BGain;
        }
//#NT#2013/09/05#Spark Chou -begin
//#NT#
        if ( RGain > OldRGain ) RGainDiff = RGain - OldRGain;
                           else RGainDiff = OldRGain - RGain;
        if ( BGain > OldBGain ) BGainDiff = BGain - OldBGain;
                           else BGainDiff = OldBGain - BGain;
        if ( RGainDiff > BGainDiff ) GainDiff = RGainDiff;
                                else GainDiff = BGainDiff;
        UINT32 w1, w2;
        w1 = AWB_SMOOTH_WEIGHT_NEW_MAX;
        if ( GainDiff > AWB_SMOOTH_WEIGHT_PRE_MAX )
            w2 =AWB_SMOOTH_WEIGHT_PRE_MAX;
        else if ( GainDiff < AWB_SMOOTH_WEIGHT_PRE_MIN )
            w2 = AWB_SMOOTH_WEIGHT_PRE_MIN;
        else
            w2 = GainDiff;
//#NT#2013/09/05#Spark Chou -end
        RGain = (NewRGain * w1 + OldRGain*w2) / (w1+w2);
        GGain = (NewGGain * w1 + OldGGain*w2) / (w1+w2);
        BGain = (NewBGain * w1 + OldBGain*w2) / (w1+w2);
    }
    else
    {
        RGain = OldRGain;
        GGain = OldGGain;
        BGain = OldBGain;
    }
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
    if((RGain==0)||(GGain==0)||(BGain==0))
    {
        *Rg = AWB_DEF_RGAIN;
        *Gg = AWB_DEF_GGAIN;
        *Bg = AWB_DEF_BGAIN;
    }
    else
    {
        *Rg = RGain;
        *Gg = GGain;
        *Bg = BGain;
    }
}

void AWB_getCA(UINT32 *R, UINT32 *G, UINT32 *B, UINT32 Size)
{
    IPL_CA_INFOR CAInfo;
    static UINT16 RAcc[AWB_WIN_TOTAL], GAcc[AWB_WIN_TOTAL], BAcc[AWB_WIN_TOTAL];
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

    Addr += sprintf((char *)Addr, "AWBalg mode = %ld\t%08x\r\n", AWB_GetAlgMode(), AwbInfo.FuncCtrl);
    Addr += sprintf((char *)Addr, "Kgain = %ld\t%ld\r\n", AwbInfo.KRgain, AwbInfo.KBgain);
    Addr += sprintf((char *)Addr, "RGain = \t%ld\r\nGGain = \t%ld\r\nBGain = \t%ld\r\n", RGain, GGain, BGain);
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
            Addr += sprintf((char *)Addr,"%3ld %3ld %3ld\t",AwbInfo.AccTab[AWBALG_CH_R][j*AWB_WIN_Y+i],AwbInfo.AccTab[AWBALG_CH_G][j*AWB_WIN_Y+i],AwbInfo.AccTab[AWBALG_CH_B][j*AWB_WIN_Y+i] );
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
    //AWB_MODE AWBMode = AWB_getMode((UINT32)mode);
    //void AWB_getCA(UINT32 *R, UINT32 *G, UINT32 *B, UINT32 Size)
    //AWB_getCA(AwbInfo.AccTab[AWBALG_CH_R], AwbInfo.AccTab[AWBALG_CH_G], AwbInfo.AccTab[AWBALG_CH_B], AWB_WIN_TOTAL);

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
    //for ( y=0; y<AWB_WIN_Y; y++ )
    for ( y=10; y<20; y++ )
    {
        //for ( x=0; x<AWB_WIN_X; x++ )
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

