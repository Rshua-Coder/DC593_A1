/**
    Auto white balance flow.

    ae flow.

    @file       awb_NT99231_EVB_FF.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <stdio.h>

#include "type.h"
#include "Awb_alg.h"
#include "Awb_NT99231_EVB_FF_int.h"
#include "Awb_Api.h"
#include "Ae_Api.h"
#include "IPL_AlgInfor.h"
#include "IPL_Cmd.h"
#include "IQS_SettingFlow.h"

#define __MODULE__ AWB
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

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
//static UINT32 RGain, GGain, BGain;
UINT32 RGain, GGain, BGain;
static UINT32 AWBCnt = 0;
static UINT32 DefRGain=AWB_DEF_RGAIN, DefGGain=AWB_DEF_GGAIN, DefBGain=AWB_DEF_BGAIN;


#define TBL_GREEN_MAX 1
UINT32 tblGreen[TBL_GREEN_MAX][3] =
{
    {143, 211, 500},
};

static UINT32 fpUserChkWhite(UINT32 R, UINT32 G, UINT32 B)
{
    UINT32 Weight=100;
    UINT32 Rgain, Bgain;
    UINT32 RGainDiff, BGainDiff, i;

    if (R == 0) R = 1;
    if (B == 0) B = 1;

    Rgain = 100 * G / B;
    Bgain = 100 * G / B;

    if ( AE_GetPrvEVValue() > 40960 )
    {
        //Remove Green@outdoor
        for ( i=0; i< TBL_GREEN_MAX; i++ )
        {
            if ( tblGreen[i][0] > Rgain )
                RGainDiff = tblGreen[i][0] - Rgain;
            else
                RGainDiff = Rgain - tblGreen[i][0];

            if ( tblGreen[i][1] > Bgain )
                BGainDiff = tblGreen[i][1] - Bgain;
            else
                BGainDiff = Bgain - tblGreen[i][1];

            if ( RGainDiff * BGainDiff < tblGreen[i][2] )
                Weight = 0;
        }
        //remove red build@outdoor
        if ( Rgain * 100 > 150 * Bgain )
            Weight = 0;
    }
    return Weight;
}

void AWB_Init(UINT32 CurrentStatus)
{
#if 0
    AWB_SetAlgMode(AWBALG_MODE_GRAYWORLD);
#endif
#if 0
    AwbInfo.FuncCtrl = (AWB_FUNCCTRL_R1_PPROC_EN | AWB_FUNCCTRL_R1_CTTAB_EN | AWB_FUNCCTRL_R1_STABLE_EN);
    AWB_SetAlgMode(AWBALG_MODE_NVTALG);
#endif
#if 0
    AWB_SetAlgMode(AWBALG_MODE_NVTALG_R2);
    AwbInfo.FuncCtrl = (AWB_FUNCCTRL_R2_CTTAB_EN | AWB_FUNCCTRL_R2_STABLE_EN | AWB_FUNCCTRL_R2_MULTICT_EN);
#endif
#if 1
    AwbInfo.FuncCtrl = (AWB_FUNCCTRL_R1_CTTAB_EN | AWB_FUNCCTRL_R1_STABLE_EN);
    AWB_SetAlgMode(AWBALG_MODE_NVTALG);
#endif

    AwbInfo.KRgain = AWB_DEF_KRGAIN;
    AwbInfo.KBgain = AWB_DEF_KBGAIN;

#if 0
    CAL_DATA CalInfo;
    Cal_GetCalData(&CalInfo);

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
    AwbInfo.MinWTotal = 20;

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
}

void AWB_Process(UINT32 CurrentStatus)
{
    AWB_MODE Mode;

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
    }
    else
    {
        AWB_manualProc(Mode);
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
#if 0
extern void SenFP_ShowOSDString(CHAR *pString, UINT16 uiX, UINT16 uiY, UINT16 uiColor);
extern void SenFP_ClearOSD(void);
#endif

extern UINT32 OB_Mul;
//------------------------------------------------------------------------------
void AWB_autoProc(AWBALG_INFO *Info)
{
    UINT32 NewRGain, NewGGain, NewBGain;
    AWBALG_ER ER_code;
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

        RGain = (NewRGain * 3 + OldRGain*7) / 10;
        GGain = (NewGGain * 3 + OldGGain*7) / 10;
        BGain = (NewBGain * 3 + OldBGain*7) / 10;
    }
    else
    {
        RGain = OldRGain;
        GGain = OldGGain;
        BGain = OldBGain;
    }

    if ( (AWBCnt % 12) == 0 )
    {
        if (AWBUartMsg == ENABLE)
        {
            DBG_ERR("ER_code = %d, AWBCnt = %d, GAIN = (%3d %3d %3d)\r\n", ER_code, AWBCnt, RGain, GGain, BGain);
        }
#if 0
        char pString[512];
        SenFP_ClearOSD();
        sprintf(pString, "%3d %3d %3d", RGain, GGain, BGain);
        SenFP_ShowOSDString(pString, 20, 60, 4);
#endif
    }
    AWB_setGain(RGain, GGain, BGain);
    DefRGain = RGain;
    DefGGain = GGain;
    DefBGain = BGain;
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
        R[i] = (RAcc[i] >> 4);
        G[i] = (GAcc[i] >> 4);
        B[i] = (BAcc[i] >> 4);
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


    Addr += sprintf((char *)Addr, "RGain = \t%ld\r\nGGain = \t%ld\r\nBGain = \t%ld\r\n", RGain, GGain, BGain);
    Addr += sprintf((char *)Addr, "Weight Table:\r\n");

    for(j = 0; j < AWB_WIN_Y; j++)
    {
        Addr += sprintf((char *)Addr,"W  \t: ");

        for (i = 0; i < AWB_WIN_X; i++)
        {
            Addr += sprintf((char *)Addr,"%3ld\t", AwbInfo.FlagTab[i + (j * AWB_WIN_X)]);
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

