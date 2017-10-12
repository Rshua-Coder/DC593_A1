/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       AF_Lib.c
    @ingroup    mISYSLens

    @brief      Auto focus library

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA2
    @date       2012/08/14
*/
#include <stdio.h>
#include "debug.h"
#include "SysKer.h"
#include "IPL_AlgInfor.h"
#include "Lens.h"
#include "AF_Lib.h"
#include "af_task.h"
#include "Utility.h"
//#include "ipe_lib.h"
#include "string.h"
//#include "sie_lib.h"
#include "Ipe_lib.h"
#include "fd_lib.h"
#include <stdlib.h>
#include "afdrv_common.h"
#include "af_api.h"
#include "af_alg.h"
#include "type.h"
#include "Af_alg.h"
#include "Af_api.h"
#include "ae_api.h"
#include "IPL_Cmd.h"
//#include "IPL_Hal_IPE.h"
//#include "IPL_Hal_IPE_Info.h"
#include "IPL_Ctrl.h"
#include "Perf.h"

#define AF_PRE_EXCITE_ALWAYS    DISABLE

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          AFSAMPLE
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define AF_SAMPLE_TODO    0
#define AF_ALG_FIND_PEAK_INTERPOLATION ENABLE
#define AF_PERF_MEASURE_EN             DISABLE

#define AF_VA_CURVE_DROP_THRESHOLD     0.8


/**
    IPL va output group id
*/
typedef enum
{
    VA_GRP0 = 0,
    VA_GRP1H = 1,
    VA_GRP1V = 2,
    VA_GRP2H = 3,
    VA_GRP2V = 4,
    ENUM_DUMMY4WORD(VA_GRP)
} VA_GRP;
static IPL_SET_VA_DATA g_tVA_Data;

#if AF_SAMPLE_TODO
static FPAF_DETGPIOKEY g_bfpAfDetShutter2 = NULL;
#endif

static UINT32 g_AFInfoIdx = 0;
static UINT32 g_AFGuessInfoIdx = 0;
static UINT32 g_AFMaxIdx = 0;
static BOOL   g_AFAlgCentralWinFound = 0;
static INT32 g_AFAlgMaxIdx = 0;
static AFDRV_MANUAL g_taf_manual = {0};
static UINT32 g_uiGetVAMoreTime = 0;
#if AF_PERF_MEASURE_EN
static UINT32 g_uiAFPerfTime = 0;
#endif

AF_INFO g_AFInfo = {0};
AF_GUESS_INFO g_AFGuessInfo = {0};
BOOL g_AFfocusforAllWin[AF_FOCUS_TOTAL_WIN] = {0};

AF_CBMSG_FP gAfCbFP = NULL; // Callback funtion to notify UI.

#if 0 // VA TBD.
AF_IPEINFO gAFIPEInfo={0};
#endif

INT32 Focus_TempComp_Near[] = {58,59,63,58,59,61,63,64,63,62,58,59,63};
INT32 Focus_TempComp_Far[] = {58,59,63,58,59,61,63,64,63,62,58,59,63};
static BOOL g_bAFChangeMode = TRUE;

UINT32 g_uiaf_peak_offset = 2;

void AF_ResetAFGuessInfo(void);

AF_AETRANSFERMODE AFdrv_GetAFModel(void)
{
    return Model_Normal;
}

void AFdrv_SetAFModeChange(BOOL EN)
{
    if(EN)
        g_bAFChangeMode = TRUE;
    else
        g_bAFChangeMode = FALSE;
}

BOOL AFdrv_CheckAFModeChange(void)
{
    return g_bAFChangeMode;
}

void AFdrv_AFPrepare(BOOL bOP, UINT32 step)
{
}



void AFdrv_AEPrepare(BOOL bOP, UINT32 step)
{
}

void AFdrv_SetManualRange(AFDRV_MANUAL ManualSet)
{
    g_taf_manual = ManualSet;
}

void AFdrv_GetCurrentRange(PAFALG_INFO_IN af_Obj, SEL_AFMODE AFMode)
{
    UINT8 uiZoomSection = Lens_Zoom_GetSection(ZOOM_CUR_SECTION);
    BOOL Compensate_enable = TRUE;

    //Step1
    switch(AFMode)
    {
        case SEL_AFMODE_SUPER_MACRO:
        case SEL_AFMODE_MACRO:
            af_Obj->iStartIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 6) - 20; // 50cm distance.;
            af_Obj->iEndIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 11) + 12; // 50cm distance.
            af_Obj->iStep = 8;
            af_Obj->iDefaultIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 6);
            Compensate_enable = FALSE;
            DBG_MSG("SEL_AFMODE_MACRO\r\n");
            break;

        case SEL_AFMODE_INIFINITE:
            af_Obj->iStartIdx = Lens_Focus_GetTable(uiZoomSection-1,0);
            af_Obj->iEndIdx = Lens_Focus_GetTable(uiZoomSection-1,1);
            af_Obj->iStep = 6+(4*(uiZoomSection-1))/12;
            //af_Obj->iStep = 12+(6*(uiZoomSection-1))/12;
            af_Obj->iDefaultIdx = Lens_Focus_GetTable(uiZoomSection-1,0);
            break;

        case SEL_AFMODE_MANUAL:
            af_Obj->iStartIdx = g_taf_manual.iManualStartIdx;
            af_Obj->iEndIdx = g_taf_manual.iManualEndIdx;
            af_Obj->iStep = g_taf_manual.iManualStep;
            af_Obj->iDefaultIdx = Lens_Focus_GetTable(uiZoomSection-1,2);
            Compensate_enable = FALSE;
            break;

        case SEL_AFMODE_CONTINUEFOCUS:
            af_Obj->iStartIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 0) - 12; // INF distance.
            af_Obj->iEndIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 11); // 50cm distance.
            break;

        case SEL_AFMODE_AUTO:
        default:
            af_Obj->iStartIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 0) - 40; // INF distance.
            af_Obj->iEndIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 6) + 16; // 50cm distance.
            af_Obj->iDefaultIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 2) ;
            if(uiZoomSection < 6)
                af_Obj->iStep = 4;
            else if(uiZoomSection < 8)
                af_Obj->iStep = 8;
            else
                af_Obj->iStep = 12;

            Compensate_enable = FALSE;
            DBG_MSG("#SEL_AFMODE_AUTO\r\n");
            break;
    }

    if(IPL_AlgGetUIInfo(IPL_SEL_AFWINDOW) == AFALG_WINDOW_CENTER)
        af_Obj->iOffsetStep = (g_uiaf_peak_offset & 0x03);
    else
        af_Obj->iOffsetStep = 2; //1;

    af_Obj->iCurIdx = af_Obj->iStartIdx;

    DBG_MSG("AF mode = %d; %d -> %d ,Step = %d, offset TblIdx = %d\r\n", AFMode, af_Obj->iStartIdx, af_Obj->iEndIdx, af_Obj->iStep, af_Obj->iOffsetStep);
}

UINT16 AFdrv_GetFocusDist(void)
{
#if 1
    UINT8 i;
    UINT32 uiDist;
    UINT8 uiZoomSecMinusOne;
    //#Rearrange lens driver
    uiZoomSecMinusOne = Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1;

    for(i=0; i<11; i++)
    {
         if(Lens_Focus_GetPosition()<Lens_Focus_GetTable(uiZoomSecMinusOne,i))
            break;
    }

    if(i==0)
    {
        uiDist= Lens_Focus_GetDistTable(0);
    }
    else if(i==11)
    {
        uiDist= Lens_Focus_GetDistTable(10);
    }
    else
    {
        UINT32 uiAval, uiBval, uiAintvl, uiBintvl;

        uiAval = Lens_Focus_GetDistTable(i-1);
        uiBval = Lens_Focus_GetDistTable(i);
        uiAintvl = Lens_Focus_GetPosition() - Lens_Focus_GetTable(uiZoomSecMinusOne,i-1);
        uiBintvl = Lens_Focus_GetTable(uiZoomSecMinusOne,i)-Lens_Focus_GetPosition();
        uiDist = (uiAval*uiBintvl + uiBval*uiAintvl)/(uiAintvl+uiBintvl);
    }

    return (UINT16)uiDist;
#else
    return 100;
#endif
}

UINT32 AF_WriteDebugInfo(UINT32 Addr)
{
    UINT32 i;
    //Get AF information
    PAFALG_INFO pGetafinfo = AFALG_GetINFO();
    UINT8 uiZoomSection = Lens_Zoom_GetSection(ZOOM_CUR_SECTION);

    Addr += sprintf((char *)Addr, "\r\n\r\n\r\nAF Info------------------------------------------------------------------------------------------------------------------------\r\n");

    Addr += sprintf((char *)Addr, "ZoomSec = \t%d\r\nOriPeak = \t%ld\r\nPredictPeak = \t%ld\r\nSts = \t%08lx\r\n",
                    uiZoomSection, g_AFMaxIdx, pGetafinfo->afalg_out.iPeak, pGetafinfo->afalg_out.uiStatus);

    Addr += sprintf((char *)Addr, "UI setting: AF mode=%d, AF Win=%d\r\n", IPL_AlgGetUIInfo(IPL_SEL_AFMODE), IPL_AlgGetUIInfo(IPL_SEL_AFWINDOW));

    Addr += sprintf((char *)Addr, "VA =\r\n");
    for(i = 0; i < g_AFInfoIdx && i < AF_FOCUS_ARRAY_SIZE; i++)
    {
        Addr += sprintf((char *)Addr, "%d\t", g_AFInfo.Value[i]);
    }

    Addr += sprintf((char *)Addr, "\r\nStep =\r\n");
    for(i = 0; i < g_AFInfoIdx && i < AF_FOCUS_ARRAY_SIZE; i++)
    {
        Addr += sprintf((char *)Addr, "%d\t", g_AFInfo.Step[i]);
    }

    Addr += sprintf((char *)Addr, "\r\n");

    return Addr;
}



void AF_SetManualRange(INT16 iStartIdx, INT16 iEndIdx, INT8 iStep)
{
    AFDRV_MANUAL Manual_Setting;

    Manual_Setting.iManualStartIdx = iStartIdx;
    Manual_Setting.iManualEndIdx = iEndIdx;
    Manual_Setting.iManualStep = iStep;
    AFdrv_SetManualRange(Manual_Setting);
}

void AF_GetResult(INT16* piStartIdx, INT16* piEndIdx, INT8* piStep, INT16* piOffsetStep, INT16 *piMaxIdx, UINT8 *puiSelectWin, UINT8 *puiStatus)
{
    PAFALG_INFO Getafinfo;
    Getafinfo = AFALG_GetINFO();
    if(piStartIdx!=NULL)
        *piStartIdx = Getafinfo->afalg_in.iStartIdx;
    if(piEndIdx!=NULL)
        *piEndIdx = Getafinfo->afalg_in.iEndIdx;
    if(piStep!=NULL)
        *piStep = Getafinfo->afalg_in.iStep;
    if(piMaxIdx!=NULL)
        *piMaxIdx = Getafinfo->afalg_out.iPeak;
    if(puiSelectWin!=NULL)
        *puiSelectWin = Getafinfo->afalg_out.uiSelectWin[0];
    if(piOffsetStep!=NULL)
        *piOffsetStep = Getafinfo->afalg_in.iOffsetStep;
    if(puiStatus!=NULL)
        *puiStatus = Getafinfo->afalg_out.uiStatus;
}

void AF_SetAlgSearchMode(AF_ALG_SEARCH_MODE af_alg_search_mode)
{
    PAFALG_INFO pGetafinfo = AFALG_GetINFO();

    if (af_alg_search_mode <= AF_ALG_SMODE_FALLNORMAL_NOFD)
        pGetafinfo->afalg_in.Search_mode = af_alg_search_mode;
    else
        DBG_ERR("AF alg search mode incorrect!\r\n");
}

AF_ALG_SEARCH_MODE AF_GetAlgSearchMode(void)
{
    PAFALG_INFO pGetafinfo = AFALG_GetINFO();

    return pGetafinfo->afalg_in.Search_mode;
}

void AF_RegisterCB(AF_CBMSG_FP CB)
{
    gAfCbFP = CB;
}

void AF_Init(UINT32 CurrentStatus)
{
    UINT32 uiStatus = 0, uiAFmode;
    PAFALG_INFO pGetafinfo = AFALG_GetINFO();

    DBG_MSG("#AF_Init\r\n");

    if(gAfCbFP)
        gAfCbFP(AF_CBMSG_AF_START, (void *)&uiStatus);

#if AF_PERF_MEASURE_EN
    Perf_Open();
    g_uiAFPerfTime = Perf_GetCurrent();
#endif

    /* Setup searching range. */
    uiAFmode = IPL_AlgGetUIInfo(IPL_SEL_AFMODE);
    AFdrv_GetCurrentRange(&pGetafinfo->afalg_in, uiAFmode);
    g_uiGetVAMoreTime = pGetafinfo->afalg_in.iOffsetStep;

    #if AF_SAMPLE_TODO
    if(gAlgMsg.FaceDetectNum>0 && SearchMode ==AF_ALG_SMODE_FALLNORMAL && SearchMode !=AF_ALG_SMODE_FALLOFF_STRONG)
    {
        pGetafinfo->afalg_in.bObj = TRUE;
    }
    else
    #endif
    {
        pGetafinfo->afalg_in.bObj = FALSE;
    }

    #if AF_SAMPLE_TODO
    //pGetafinfo->afalg_in.Search_mode = SearchMode;

    if((Lens_Zoom_GetSection(ZOOM_CUR_SECTION) >= (Lens_Zoom_GetSection(ZOOM_MAX_SECTION)-1))&&
        AFdrv_GetAFModel()==Model_4 )//Mode 4 need more search range in tele
    {
        pGetafinfo->afalg_in.Search_mode = AF_ALG_SMODE_FALLOFF_STRONG;
    }

    AFALG_SetParam();

    //For AE setting
    AE_SetAFData(pGetafinfo->afalg_in.iStep);
    #else
    switch(IPL_AlgGetUIInfo(IPL_SEL_AFWINDOW))
    {
        case SEL_AFWINDOW_AUTO:
            pGetafinfo->afalg_in.Window_sel = AFALG_WINDOW_AUTO;
            break;
        case SEL_AFWINDOW_MANUALPOSITION:
            pGetafinfo->afalg_in.Window_sel = AFALG_WINDOW_MANUIAL;
            break;
        case SEL_AFWINDOW_CENTER:
            pGetafinfo->afalg_in.Window_sel = AFALG_WINDOW_CENTER;
            break;
        default:
            DBG_ERR("ERROR!! Exception AFWindow!\r\n");
            break;
    }

    AF_SetAlgSearchMode(AF_ALG_SMODE_FALLNORMAL);
    AFALG_SetParam();
    AF_ResetAFGuessInfo();

    /* Setup IPE VA. */
    AF_VA_Setup();

    /* Reset AF info. Do it before AF started. */
    AF_ResetAFInfo();

    /* Move focus to the start idx of searching range. */
    Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, pGetafinfo->afalg_in.iStartIdx);
    IPC_WaitVD(TRUE,IPL_ID_1); // wait until VA stable.
    #endif
}

static void AF_SingleWinProcess(void)
{
    BOOL bAFDone = FALSE;
    INT16 idx;
    INT32 i32Step = 0;
    UINT32 tempVA = 0;
    UINT32 uiStatus;
    UINT32 uiG0, uiG1h, uiG1v, uiG2h, uiG2v;
    //Get AF information
    PAFALG_INFO pGetafinfo = AFALG_GetINFO();


    //If AF status not AFALG_RUN, break
    if(AFALG_CheckStatus() == AFALG_QUIT) // Quit AF by UI.
    {
        bAFDone = TRUE;
        DBG_MSG("Quit!\r\n");
    }
    else if(AFALG_CheckStatus() != AFALG_RUN)
        return;

    AF_GetVA_Result(&uiG0, &uiG1h, &uiG1v, &uiG2h, &uiG2v, VA_WIN_INDP0);
    #if 0
    tempVA = uiG0;
    #else
    tempVA = uiG2h + uiG2v;
    #endif

    idx = Lens_Focus_GetPosition(); // Get currnet focus position.
    DBG_MSG("s=%d, tVA=%d, mVA=%d\r\n",  idx, tempVA, g_AFInfo.MaxVA);

    if(g_AFInfoIdx < AF_FOCUS_ARRAY_SIZE)
    {
        g_AFInfo.Step[g_AFInfoIdx] = idx;
        g_AFInfo.Value[g_AFInfoIdx] = tempVA;
        g_AFInfo.VACnt[g_AFInfoIdx] = ipe_getVACnt();
        g_AFInfoIdx++;
    }

    if(tempVA > g_AFInfo.MaxVA)
    {
        g_AFMaxIdx = idx;
        g_AFInfo.MaxVA = tempVA;
        if(g_AFInfoIdx >= 1)
            g_AFInfo.MaxIdx = g_AFInfoIdx - 1;

        g_AFGuessInfo.bEnable = FALSE;
        //DBG_MSG("Found MaxVA!\r\n");
    }
    else
    {
        if(g_AFGuessInfo.bEnable)
        {
            g_AFGuessInfo.Value[1] = ((g_AFGuessInfo.Value[1] + g_AFInfo.Value[g_AFInfoIdx - 1]) >> 1);
            if (g_AFGuessInfo.Value[1] < (g_AFGuessInfo.Value[0] * AF_VA_CURVE_DROP_THRESHOLD))
            {
                bAFDone = TRUE;
                g_uiGetVAMoreTime = 0;
                DBG_MSG("^RVA value went down below threshold. Cur_2VA:%d, Max_2VA:%d\r\n", g_AFGuessInfo.Value[1], g_AFGuessInfo.Value[0]);
            }
            else
            {
                DBG_MSG("^GFake peak! VA value interpolation. Cur_2VA:%d, Max_2VA:%d\r\n", g_AFGuessInfo.Value[1], g_AFGuessInfo.Value[0]);
            }

            g_AFGuessInfo.bEnable = FALSE;
        }
        else if (tempVA < (g_AFInfo.MaxVA * AF_VA_CURVE_DROP_THRESHOLD) && tempVA > 1000) //0.6 is threshold.
        {
            if(!g_AFGuessInfo.bEnable && g_AFInfoIdx >= 2)
            {
                g_AFGuessInfo.Value[0] = ((g_AFInfo.Value[g_AFInfo.MaxIdx] + g_AFInfo.Value[g_AFInfo.MaxIdx + 1]) >> 1);
                g_AFGuessInfo.Value[1] = g_AFInfo.Value[g_AFInfoIdx - 1];
                g_AFGuessInfo.bEnable = TRUE;
                //DBG_MSG("^GFound peak!\r\n");
            }
        }
    }

    if( pGetafinfo->afalg_in.iCurIdx <= pGetafinfo->afalg_in.iEndIdx && !bAFDone)
    {
        #if 0
        IPC_WaitVD(FALSE,IPL_ID_1);
        #else // clear VD before wait.
        IPC_WaitVD(TRUE,IPL_ID_1);
        #endif

        pGetafinfo->afalg_in.iCurIdx += pGetafinfo->afalg_in.iStep;
        /* Move focus to next position. */
        #if AF_PRE_EXCITE_ALWAYS
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, pGetafinfo->afalg_in.iCurIdx);
        #else
        if(g_AFInfoIdx <= 1)
            Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN, pGetafinfo->afalg_in.iCurIdx);
        else
            Lens_Focus_DoAction(FOCUS_RUN, pGetafinfo->afalg_in.iCurIdx);
        #endif
    }
    else
    {
        if(pGetafinfo->afalg_in.iCurIdx > pGetafinfo->afalg_in.iEndIdx && g_uiGetVAMoreTime--)
        {
            //DBG_MSG("*Get boundary VA value\r\n");
            return;
        }

        /* VA result should be shifted to match actual focus position */
        for(idx = pGetafinfo->afalg_in.iOffsetStep; idx <= (INT32)g_AFInfoIdx; idx++)
        {
            if(g_AFInfo.MaxVA == g_AFInfo.Value[idx])
            {
                i32Step = idx - pGetafinfo->afalg_in.iOffsetStep;
                g_AFMaxIdx = g_AFInfo.Step[i32Step];
                DBG_MSG("OriStep:%d, ShiftStep:%d\r\n", g_AFInfo.Step[idx], g_AFMaxIdx);
                break;
            }
        }

        if((INT32)g_AFMaxIdx < (pGetafinfo->afalg_in.iStartIdx + (INT32)pGetafinfo->afalg_in.iStep))
        {
            pGetafinfo->afalg_out.uiStatus |= AFALG_STS_TOOFAR;
            pGetafinfo->afalg_out.iPeak = g_AFMaxIdx;
            DBG_ERR("^RToo far!\r\n");
        }
        else if(g_AFMaxIdx <= pGetafinfo->afalg_in.iEndIdx - pGetafinfo->afalg_in.iStep )
        {

            #if (AF_ALG_FIND_PEAK_INTERPOLATION == DISABLE)
            pGetafinfo->afalg_out.iPeak = g_AFMaxIdx;
            #else
            //i32Step = ( g_AFMaxIdx - pGetafinfo->afalg_in.iStartIdx ) / pGetafinfo->afalg_in.iStep;
            if(i32Step < AF_FOCUS_ARRAY_SIZE && i32Step > 0)
            {
                pGetafinfo->afalg_out.iPeak = AFALG_FindPeak( g_AFInfo.Value[i32Step + pGetafinfo->afalg_in.iOffsetStep - 1],
                                               g_AFInfo.MaxVA,//g_AFInfo.Value[i32Step],
                                               g_AFInfo.Value[i32Step + pGetafinfo->afalg_in.iOffsetStep + 1],
                                               g_AFMaxIdx,//g_AFInfo.Step[i32Step],
                                               pGetafinfo->afalg_in.iStep );
            }
            else
            {
                DBG_ERR("^RCan't do AF prediction:%d\r\n", i32Step);
                pGetafinfo->afalg_out.iPeak = g_AFMaxIdx;
            }
            #endif
        }
        else// if(piIdxMax[uiWinSelect] >= iEndIdx)
        {
            pGetafinfo->afalg_out.uiStatus |= AFALG_STS_TOONEAR;
            pGetafinfo->afalg_out.iPeak = g_AFMaxIdx;
            DBG_ERR("^RToo Near!\r\n");
        }

        DBG_MSG("^GPeak position: Orig:%d, Predict:%d, Sts:%08lx\r\n", g_AFMaxIdx, pGetafinfo->afalg_out.iPeak, pGetafinfo->afalg_out.uiStatus);

        /* Move focus to peak position. */
        if(AFALG_CheckStatus() == AFALG_QUIT)
            pGetafinfo->afalg_out.iPeak = g_AFMaxIdx;

        #if AF_PRE_EXCITE_ALWAYS
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, pGetafinfo->afalg_out.iPeak);
        #else
        if(pGetafinfo->afalg_in.iCurIdx > pGetafinfo->afalg_in.iStartIdx || (g_AFInfoIdx >= 1)) // Has been pre-excited.
            Lens_Focus_DoAction(FOCUS_RUN|FOCUS_POSTEXC, pGetafinfo->afalg_out.iPeak);
        #endif

        #if 0
        AF_Release(IPL_UTI_CONV2_AF_ID(IPL_ID_1), FALSE);
        #else
        AF_Pause(IPL_UTI_CONV2_AF_ID(IPL_ID_1), FALSE);

        if(gAfCbFP)
        {
            #if AF_PERF_MEASURE_EN
            g_uiAFPerfTime = Perf_GetCurrent() - g_uiAFPerfTime;
            Perf_Close();
            #endif
            uiStatus = AFALG_GetAFStatus();

            gAfCbFP(AF_CBMSG_AF_DONE, (void *)&uiStatus);
        }

        AFALG_SetStatus(AFALG_END);
        #endif
    }
}

static void AFALG_MultiWinProcess(void)
{
    PAFALG_INFO_IN pInData = &AFALG_GetINFO()->afalg_in;
    PAFALG_INFO_OUT pOutData = &AFALG_GetINFO()->afalg_out;
    PAFALG_PARAM pAfAlgParam = AFALG_GetAFALGPARAM();
    static UINT32  winsum = 0,win, Idx = 0, curve_cnt=0,curve_num = 0,Stop_cnt;
    static UINT32 puiVACurr[AF_TOTAL_WIN]={0};
    static UINT32 uiVATremble[AF_TOTAL_WIN]={0};
    UINT32 uiVATrembleTmp=0;
    UINT32 uiCntr, uiNext, uiLast;
    UINT32 uiX, uiY, uiStartXY, uiTempXY;
    static INT32  MaxFoucusPos[AF_TOTAL_WIN]={0};
    UINT32 step = 0;
    static UINT32 uiContinusFalling[AF_TOTAL_WIN]={0};
    static UINT32 uiVATremble_Cnt[AF_TOTAL_WIN]={0};
    BOOL   bAFDone = FALSE;


//    for(FocusPos=pInData->iStartIdx; FocusPos<=pInData->iEndIdx; FocusPos+=pInData->iStep)
//    {

    //If AF status not AFALG_RUN, break
    if(AFALG_CheckStatus() == AFALG_QUIT) // Quit AF by UI.
    {
        bAFDone = TRUE;
        DBG_MSG("Quit!\r\n");
    }
    else if(AFALG_CheckStatus() != AFALG_RUN)
        return;

    if(AFALG_GetInfoIdx() < 1)
    {
        memset(puiVACurr, 0, sizeof(puiVACurr));
        memset(uiVATremble, 0, sizeof(uiVATremble));
        memset(MaxFoucusPos, 0, sizeof(MaxFoucusPos));
        memset(uiContinusFalling, 0, sizeof(uiContinusFalling));
        memset(uiVATremble_Cnt, 0, sizeof(uiVATremble_Cnt));
        Idx = 0;
        Stop_cnt = 0;
        curve_cnt = 0;

        DBG_MSG("Reset alg info.\r\n");
        //AFALG_MSG("IdxOverRun:%d\r\n", pAfAlgParam->uiIdxOverRun);
    }


    // Get 16x16 VA value one time.
    AF_GetVA_Result(&(pAfAlgParam->afParam[Idx].uiHWVA[0]), NULL, NULL, NULL, NULL, VA_WIN_16X16);
    pAfAlgParam->afParam[Idx].iFocusPos = pInData->iCurIdx;

    for(win = 0;win < AF_TOTAL_WIN;win++)
    {
        //------------------------------------------------------------------
        // Step1: Check this window's score, if no score, skip it.
        //------------------------------------------------------------------
        if(pAfAlgParam->WindowScore[win] == 0)
            continue;

        //------------------------------------------------------------------
        // Step2: Record VA value
        //------------------------------------------------------------------
        #if 0
        puiVACurr[win] = g_fpGetVA(win);
        pAfAlgParam->afParam[Idx].uiALGVA[win] = puiVACurr[win] ;
        #else // Get va win from HW VA win(16x16).
        uiX = (win % AF_WIN_DIMENSION_W);
        uiY = (win / AF_WIN_DIMENSION_H);
        uiStartXY = (uiY * (AF_WIN_HW_H / AF_WIN_DIMENSION_H) * AF_WIN_HW_H) + (uiX * (AF_WIN_HW_W / AF_WIN_DIMENSION_W));
        uiY = (AF_WIN_HW_H / AF_WIN_DIMENSION_H);
        while(uiY--)
        {
            uiTempXY = uiStartXY;
            uiX = (AF_WIN_HW_W / AF_WIN_DIMENSION_W);
            while(uiX--)
            {
                //DBG_MSG("uiTempXY:%d, uiX:%d, uiY:%d\r\n", uiTempXY, uiX, uiY);
                if(uiStartXY < AF_VA_WINSIZE)
                    pAfAlgParam->afParam[Idx].uiALGVA[win] += pAfAlgParam->afParam[Idx].uiHWVA[uiTempXY++];
                else
                {
                    DBG_ERR("^RVA win index out of bound!\r\n");
                    pOutData->uiStatus |= AFALG_STS_NOTARGET;
                    return;
                }
            }
            uiStartXY += AF_WIN_HW_H;
        }

        puiVACurr[win] = pAfAlgParam->afParam[Idx].uiALGVA[win];
        #endif

        #if 0
        if(AE_GetLV()<5 && puiVACurr[4] < g_uiNOLightVA)
        {
            Stop_cnt++;
        }
        #endif
        //------------------------------------------------------------------
        // Step3: Record VAMax/ VAMin/ VALast
        //------------------------------------------------------------------
        if(puiVACurr[win] < pAfAlgParam->VAMin[win])
        {
            pAfAlgParam->VAMin[win] = puiVACurr[win];
            //IdxMin[win] = iFocusPos;
            //puiVAMaxBeforeMinBeforeMax[win] = AF_ALG_PARAM.VAMax[win];

        }

        if(puiVACurr[win] > pAfAlgParam->VAMax[win])
        {
            pAfAlgParam->VAMax[win] = puiVACurr[win];
            MaxFoucusPos[win] = pInData->iCurIdx;//FocusPos;
            uiVATremble_Cnt[win] = 0;
            //puiVAMinBeforeMax[win] = puiVAMin[win];
        }

        pAfAlgParam->VALast[win] = puiVACurr[win];

        //---------------------------------------------------------
        // Step4: Record Tremble and continues falling
        //---------------------------------------------------------
        if(Idx>=2)
        {
            uiLast = pAfAlgParam->afParam[Idx-2].uiALGVA[win];
            uiCntr = pAfAlgParam->afParam[Idx-1].uiALGVA[win];
            uiNext = pAfAlgParam->afParam[Idx].uiALGVA[win];

            if(uiLast>uiCntr && uiCntr<uiNext)
            {
                uiVATrembleTmp = (uiLast + uiNext)/2 - uiCntr;
                uiVATremble_Cnt[win]++;
                if(uiVATrembleTmp > uiVATremble[win])
                    uiVATremble[win] = uiVATrembleTmp;
            }

            if(uiCntr > uiNext)
                uiContinusFalling[win] ++;
            else
            {
                if(uiVATremble_Cnt[win] > pAfAlgParam->uiIdxOverRun)
                    uiContinusFalling[win] = pAfAlgParam->uiIdxOverRun;
                else
                    uiContinusFalling[win] = 0;
            }
        }

        #if 0
        if( (uiVATremble[win]<1000) && (pAfAlgParam->VAMax[win] > 50000) && (AFdrv_GetAFModel() == Model_4) )
        {
            pAfAlgParam->uiIdxOverRun = 3;
        }
        #endif

        //---------------------------------------------------------
        // Step5: Judge if AF end or not
        //---------------------------------------------------------
        //if(win == AF_DEFAULT_WIN) // debug purpose.
        if(( (UINT32)AFALG_ABS(pInData->iCurIdx - MaxFoucusPos[win])      >=  (pAfAlgParam->uiIdxOverRun*pInData->iStep)                                              )/*overRun-check*/
        &&( (pAfAlgParam->VAMax[win]-puiVACurr[win])*100  >   pAfAlgParam->afalg_threshold.uivibrate_th*pAfAlgParam->VAMax[win]                               )/*falling-check*/
        //&&( (pAfAlgParam->VAMax[win]-puiVACurr[win])*100  >   (pAfAlgParam->afalg_threshold.uivibrate_th*(2+((pInData->bObj)?3:0))/2)*pAfAlgParam->VAMax[win] )/*FD falling-check*/
        &&( (pAfAlgParam->VAMax[win]-puiVACurr[win])*10   >   (uiVATremble[win]*pAfAlgParam->afalg_threshold.uitremble_th)                                                                           )/*tremble-check*/
        &&(  pAfAlgParam->VAMax[win]                      >    pAfAlgParam->afalg_threshold.uimax_th                                                          )/*magnitude-check*/
        &&(  Idx                                          >=   pAfAlgParam->afalg_threshold.uirun_th                                                          )/*At least run this far*/
        &&(  uiContinusFalling[win]                       >=    pAfAlgParam->uiIdxOverRun                                                                      )/*At least falling this far continuously*/
        &&(  MaxFoucusPos[win]                            !=    pInData->iStartIdx                                                                            )/*First one not the max */
          ){
            if(pAfAlgParam->WindowScore[win] != 0)
            {
                pAfAlgParam->Curve[curve_cnt][curve_num] = win;
                pAfAlgParam->Curve[curve_cnt][++curve_num] = 0xff; // set the end first
                // Central win first.
                if(!g_AFAlgCentralWinFound)
                {
                    // Central win must be modified if dimension was changed.
                    if(win == 27 || win == 28 || win == 35 || win == 36)
                    {
                        g_AFAlgCentralWinFound = TRUE;
                        g_AFAlgMaxIdx = MaxFoucusPos[win];
                    }
                    else if(MaxFoucusPos[win] > g_AFAlgMaxIdx)
                        g_AFAlgMaxIdx = MaxFoucusPos[win];

                }

            }
            pAfAlgParam->WindowScore[win] = 0;

            //AFALG_MSG("^GBingo!- Win:%d, MaxPos:%d, curve_cnt:%d, curve_num:%d, VaMax:%d\r\n", win, MaxFoucusPos[win], curve_cnt, curve_num, pAfAlgParam->VAMax[win]);
        }
        //AFALG_MSG("Idx=%d, Win=%d, AF=%d, VA=%d, Tremble=%d, winsum=%d, algWin=%d, confall=%d\r\n",Idx,win,pAfAlgParam->afParam[Idx].iFocusPos,pAfAlgParam->VALast[win],uiVATremble[win],winsum,pAfAlgParam->WindowScore[win],uiContinusFalling[win]);
        //AFALG_MSG("Idx=%d, Win=%d, AF=%d, VA=%d, winsum=%d, algWin=%d\r\n",Idx,win,pAfAlgParam->afParam[Idx].iFocusPos,pAfAlgParam->VALast[win],winsum,pAfAlgParam->WindowScore[win]);

        winsum += pAfAlgParam->WindowScore[win];
    }

    DBG_MSG("FPos:%d, VACur[%d] = %d\r\n", pInData->iCurIdx, 35, pAfAlgParam->VAMax[35]);

    //AFALG_MSG("--------------------------------  %d   %d\r\n",curve_cnt,curve_num);
    if(curve_num != 0)
        curve_cnt++;

    if(winsum == 0 || Stop_cnt>2) // the focus curve all of windows are falling down
    {
        //AFALG_SetStatus(AFALG_END);
        //g_fpStopLens(LENS_AFSTOP);//Set Lens stop
        if(Stop_cnt>2)
            pOutData->uiStatus |= AFALG_STS_NOTARGET;

        bAFDone = TRUE;
    }

    Idx++;
    if(Idx >= AF_TOTAL_STEP)
    {
        DBG_ERR("ERROR!! AF step more than 256!!!\r\n");
        bAFDone = TRUE;
    }
//    }

    if(pInData->iCurIdx <= (pInData->iEndIdx + (pInData->iOffsetStep * (INT32)pInData->iStep)) && !bAFDone)
    {
        IPC_WaitVD(TRUE,IPL_ID_1);

        /* Move focus to next position. */
        pInData->iCurIdx += pInData->iStep;

        #if AF_PRE_EXCITE_ALWAYS
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, pInData->iCurIdx);
        #else
        if(AFALG_GetInfoIdx() < 1)
        {
            Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN, pInData->iCurIdx);
            AFALG_SetInfoIdx(AFALG_GetInfoIdx() + 1);
        }
        else
            Lens_Focus_DoAction(FOCUS_RUN, pInData->iCurIdx);
        #endif

        winsum = 0;
        curve_num = 0;
    }
    else
    {
        //---------------------------------------------------------
        // Step6: Select window if choose multi-AF
        //---------------------------------------------------------
        if(curve_cnt > 0)
        {
            for(win = (curve_cnt-1);; win--)
            {//Search back = Find Near-focus first
                for(step = 0;;step++)
                {
                    if(pAfAlgParam->Curve[win][step] == 0xff)
                        break;

                    #if 0
                    if(pAfAlgParam->WindowSelect == AF_NOSELECT_WIN)
                    #else // Find the Nearest-focus.
                    if(g_AFAlgMaxIdx == MaxFoucusPos[pAfAlgParam->Curve[win][step]])
                    #endif
                    {
                        pAfAlgParam->WindowSelect = pAfAlgParam->Curve[win][step];
                        pOutData->uiSelectWin[pAfAlgParam->Curve[win][step]] = TRUE;
                        //AFALG_MSG("^GSelWin:%d\r\n", pAfAlgParam->WindowSelect);
                    }
                }
                if(win == 0)
                    break;

                #if 0
                if(pAfAlgParam->WindowSelect == AF_NOSELECT_WIN)
                    continue;
                else
                    break;
                #endif
            }
        }
        if(pAfAlgParam->WindowSelect == AF_NOSELECT_WIN)
        {
            pAfAlgParam->WindowSelect = AF_DEFAULT_WIN;
            pOutData->uiStatus |= AFALG_STS_NOTARGET;
        }
        else
        {
            AFALG_SetStatus(AFALG_END);
        }
        //pOutData->uiSelectWin = pAfAlgParam->WindowSelect;
        //---------------------------------------------------------
        // Step7: Find Peak and record AF status
        //---------------------------------------------------------
        //Fullrange check (special case)
        if(pAfAlgParam->uiIdxOverRun == AF_TOTAL_STEP)
        {
            AFALG_SetStatus(AFALG_END);
        }
        if(MaxFoucusPos[pAfAlgParam->WindowSelect] < (pInData->iStartIdx + (INT32)pInData->iStep))
        {
            pOutData->uiStatus |= AFALG_STS_TOOFAR;
            DBG_ERR("^RToo Far!\r\n");
        }
        else if(( MaxFoucusPos[pAfAlgParam->WindowSelect] <= (pInData->iEndIdx - (INT32)(pInData->iStep*pAfAlgParam->uiIdxOverRun)))\
                || (pAfAlgParam->uiIdxOverRun == AF_TOTAL_STEP))
        {
            step = ( MaxFoucusPos[pAfAlgParam->WindowSelect] - pInData->iStartIdx ) / pInData->iStep;
            DBG_MSG("OriMaxPos:%d\r\n", pAfAlgParam->afParam[step].iFocusPos);
            if((INT32)step >= (pInData->iOffsetStep + 1))
            {
                step -= pInData->iOffsetStep;
                pOutData->iPeak = AFALG_FindPeak( pAfAlgParam->afParam[step-1].uiALGVA[pAfAlgParam->WindowSelect],
                                                   pAfAlgParam->afParam[step].uiALGVA[pAfAlgParam->WindowSelect],
                                                   pAfAlgParam->afParam[step+1].uiALGVA[pAfAlgParam->WindowSelect],
                                                   pAfAlgParam->afParam[step].iFocusPos,
                                                   pInData->iStep );

                DBG_MSG("Shited pos:%d\r\n", pAfAlgParam->afParam[step].iFocusPos);
                DBG_MSG("Interpolated pos:%d\r\n", pOutData->iPeak);
            }
            else
            {
                pOutData->iPeak = pAfAlgParam->afParam[step].iFocusPos;
                DBG_ERR("^RReached boundary\r\n");
            }
        }
        else// if(piIdxMax[uiWinSelect] >= iEndIdx)
        {
            pOutData->uiStatus |= AFALG_STS_TOONEAR;
            DBG_ERR("^RToo Near!\r\n");
        }
        DBG_MSG("AFALG=0x%.8x, Peak=%d,  overrun=%d, algstate=%d\r\n",pOutData->uiStatus,pOutData->iPeak,pAfAlgParam->uiIdxOverRun, AFALG_CheckStatus());
    /*
        if(pOutData->iPeak >= pInData->iEndIdx)
        {
            pOutData->iPeak = pInData->iEndIdx;
            pOutData->uiStatus |= AFALG_STS_TOONEAR;
        }
    */
        if(pAfAlgParam->VAMax[pAfAlgParam->WindowSelect] < pAfAlgParam->afalg_threshold.uimax_th)
        {
            pOutData->uiStatus |= AFALG_STS_LOWVA;
        }
        #if 0
        if((pAfAlgParam->VAMax[pAfAlgParam->WindowSelect]-pAfAlgParam->VALast[pAfAlgParam->WindowSelect])*10
            < (uiVATremble[pAfAlgParam->WindowSelect]*pAfAlgParam->afalg_threshold.uitremble_th))
        {
            pOutData->uiStatus |= AFALG_STS_TREMBLE;
        }
        if((pAfAlgParam->VAMax[pAfAlgParam->WindowSelect]-pAfAlgParam->VALast[pAfAlgParam->WindowSelect])*100
            < (pAfAlgParam->afalg_threshold.uivibrate_th*pAfAlgParam->VAMax[pAfAlgParam->WindowSelect]/4))
        {
            pOutData->uiStatus |= AFALG_STS_SMOOTH;
        }
        #endif
        //if(uiContinusFalling[pAfAlgParam->WindowSelect] < pAfAlgParam->iIdxOverRun)
        //{
        //    if(pAfAlgParam->iIdxOverRun != AF_TOTAL_STEP)//don't consider full range search
        //        pOutData->uiStatus |= AFALG_STS_VASMALL;
        //}
        //--------------------------------------------------------------------------
        // Step9: Peak Offset conpensated
        //--------------------------------------------------------------------------
        if(pOutData->uiStatus == AFALG_STS_SUCCESS && AFALG_CheckStatus() == AFALG_END)
        {
            //pOutData->iPeak += pInData->iOffsetStep;
        }
        else
        {
            if(MaxFoucusPos[pAfAlgParam->WindowSelect])
                pOutData->iPeak = MaxFoucusPos[pAfAlgParam->WindowSelect];
            else
                pOutData->iPeak = pInData->iDefaultIdx;
        }
        //For SN test use
        #if 0
        if(g_bSNTest)
            pOutData->iPeak = pInData->iStartIdx;
        #endif
        //--------------------------------------------------------------------------
        // Step10: Peak
        //--------------------------------------------------------------------------
        #if AF_PRE_EXCITE_ALWAYS
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, pOutData->iPeak);
        #else
        if(AFALG_GetInfoIdx() >= 1) // Has been pre-excited.
            Lens_Focus_DoAction(FOCUS_RUN|FOCUS_POSTEXC, pOutData->iPeak);
        #endif

        AF_Pause(IPL_UTI_CONV2_AF_ID(IPL_ID_1), FALSE);

        AFALG_SetStatus(AFALG_END);

        DBG_MSG("AFALG status=0x%.8x, Window=%d, Peak=%d, offset=%d\r\n",pOutData->uiStatus,pAfAlgParam->WindowSelect,pOutData->iPeak,pInData->iOffsetStep);
        //return pOutData->iPeak;
    }
}

void AF_Process(UINT32 CurrentStatus)
{
    if(IPL_AlgGetUIInfo(IPL_SEL_AFWINDOW) == AFALG_WINDOW_CENTER)
    {
        AF_SingleWinProcess();
    }
    else if(IPL_AlgGetUIInfo(IPL_SEL_AFWINDOW) == SEL_AFWINDOW_AUTO)
    {
        AFALG_MultiWinProcess();
        if(AFALG_CheckStatus() == AFALG_END)
        {
            if(gAfCbFP)
            {
                UINT32 uiStatus = AFALG_GetAFStatus();

                gAfCbFP(AF_CBMSG_AF_DONE, (void *)&uiStatus);
            }
        }
    }
    else
    {
        DBG_ERR("^R No AF window specified!\r\n");
    }
}

ER AF_Run(AF_PROC_ID Id, BOOL bWaitFinish)
{
    ER rt = E_OK;

    if(AF_GetCtrlObjStatus(Id) == AF_TASK_STATUS_OPEN)
    {
        if (AF_Start(Id, ENABLE) == E_OK)
            DBG_MSG("AF(%d) start ready\r\n", Id);
        else
            DBG_MSG("AF(%d) start failed\r\n", Id);

    }
    else
    {
        AF_WaitIdle(Id);
        DBG_MSG("#AF idle\r\n");
        if (AF_Resume(Id) == E_OK)
            DBG_MSG("AF(%d) resume ready\r\n", Id);
        else
            DBG_MSG("AF(%d) resume failed\r\n", Id);

    }

    if(bWaitFinish)
        AF_WaitIdle(Id);

    return rt;
}

ER AF_Release(AF_PROC_ID Id, BOOL bWaitFinish)
{
    ER er = E_OK;

    AFALG_SetStatus(AFALG_QUIT);

    if(bWaitFinish)
        AF_WaitIdle(Id);
#if 0
    if(gAfCbFP)
    {
        uiStatus = AFALG_GetAFStatus();

        gAfCbFP(AF_CBMSG_AF_DONE, (void *)&uiStatus);
    }
#else
    //DBG_MSG("AF process time:%d\r\n", (Perf_GetCurrent() - g_uiAFPerfTime) /1000);
#endif

    return er;
}

INT32 AF_SearchFullRange(INT16 startIdx, INT16 endIdx, UINT8 jumpStep, UINT32 *pMaxVA)
{
    INT16 idx, maxIdx = 0, i;
    UINT32 tempVA=0, maxVA=0;
    UINT32 uiG0, uiG1h, uiG1v, uiG2h, uiG2v;

    DBG_ERR("#AF_SearchFullRange\r\n");

    #if 0 // VA TBD.
    if(g_fpGetVariation == NULL)
    {
        DBG_ERR"Get Variation FP is null!!\r\n");
        return 0;
    }
    #endif

    //Lens_Focus_DoAction(FOCUS_PREEXC, startIdx);
    for(idx = startIdx, i = 0; idx <= (endIdx+jumpStep-1); idx += jumpStep, i++)
    {
        #if 0
        Lens_Focus_Goto2(MOTOR_POWERON_PI2_ONLY, idx);
        #else
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, idx);
        #endif

        //#NT#2012/05/29#KS Hung -end
        //IPC_WaitVD(TRUE,IPL_ID_1);
        //IPC_WaitVD(TRUE,IPL_ID_1);
        ipe_waitFrameEnd(IPE_CLRFLG);
        ipe_waitFrameEnd(IPE_CLRFLG);
        ipe_waitFrameEnd(IPE_CLRFLG);
        #if 0
        contrastCnt = ipe_getVACnt();

        while(contrastCnt == ipe_getVACnt())
            Delay_DelayMs(1);
        #endif

        DBG_ERR("#wait VD done\r\n");
        AF_GetVA_Result(&uiG0, &uiG1h, &uiG1v, &uiG2h, &uiG2v, VA_WIN_INDP0);
        tempVA = uiG0;

        DBG_ERR("%s: step=%d, tempVA=%d, maxVA=%d\r\n", __func__, idx, tempVA, maxVA);

        if(g_AFInfoIdx < AF_FOCUS_ARRAY_SIZE)
        {
            g_AFInfo.Step[g_AFInfoIdx] = idx;
            g_AFInfo.Value[g_AFInfoIdx] = tempVA;
            g_AFInfo.VACnt[g_AFInfoIdx] = ipe_getVACnt();
            g_AFInfoIdx++;
        }

        //#NT#2012/06/05#KS Hung -begin
        if(tempVA > maxVA)
        {
            maxIdx = idx;
            maxVA = tempVA;
        }
        else
        {
            if (tempVA < (maxVA * 0.2)) //0.2 is threshold.
            {
                DBG_ERR("find peak\r\n");
                break;
            }
        }
        //#NT#2012/06/05#KS Hung -end
    }
    //Lens_Focus_DoAction(FOCUS_POSTEXC, 0);

    //Lens_Focus_Goto(maxIdx);
    if(pMaxVA != NULL)
        *pMaxVA = maxVA;

    DBG_MSG("^R%s: maxIdx=%d, maxVA=%d\r\n", __func__, maxIdx, maxVA);

    return maxIdx;
}

void AF_ResetAFInfo(void)
{
    g_AFInfoIdx = 0;
    g_AFMaxIdx = 0;
    g_AFAlgCentralWinFound = 0;
    g_AFAlgMaxIdx = 0;
    memset(&g_AFInfo, 0, sizeof(AF_INFO));
}

void AF_ResetAFGuessInfo(void)
{
    g_AFGuessInfoIdx = 0;
    memset((void *)&g_AFGuessInfo, 0, sizeof(AF_GUESS_INFO));
}

static void AF_SetVA_window(VA_WINIDX VA_win, VA_WIN win_info)
{
    VA_WIN *WinSel;

    if(VA_win == VA_WIN_16X16)
    {
        //memcpy(&g_tVA_Data.VAWin16X16,&win_info,sizeof(VA_WIN));
        WinSel = &g_tVA_Data.VAWin16X16;
    }
    else if(VA_win == VA_WIN_INDP0)
    {
        //memcpy(&g_tVA_Data.VAWinInd.Win0 ,&win_info,sizeof(VA_WIN));
        WinSel = &g_tVA_Data.VAWinInd.Win0;
    }
    else if(VA_win == VA_WIN_INDP1)
    {
        //memcpy(&g_tVA_Data.VAWinInd.Win1 ,&win_info,sizeof(VA_WIN));
        WinSel = &g_tVA_Data.VAWinInd.Win1;
    }
    else if(VA_win == VA_WIN_INDP2)
    {
        //memcpy(&g_tVA_Data.VAWinInd.Win2 ,&win_info,sizeof(VA_WIN));
        WinSel = &g_tVA_Data.VAWinInd.Win2;
    }
    else
        WinSel = &g_tVA_Data.VAWin16X16;

    memcpy(WinSel ,&win_info, sizeof(VA_WIN));
}

static void AF_SetVA_Threshold(VA_GRP VA_grp, UINT32 THL, UINT32 THH)
{
    if(VA_grp == VA_GRP0)
    {
        g_tVA_Data.VASettings.Vacc.GRP0.High = THH;
        g_tVA_Data.VASettings.Vacc.GRP0.Low = THL;
    }
    else if(VA_grp == VA_GRP1H)
    {
        g_tVA_Data.VASettings.Vacc.GRP1.ThresH.High = THH;
        g_tVA_Data.VASettings.Vacc.GRP1.ThresH.Low = THL;
    }
    else if(VA_grp == VA_GRP1V)
    {
        g_tVA_Data.VASettings.Vacc.GRP1.ThresV.High = THH;
        g_tVA_Data.VASettings.Vacc.GRP1.ThresV.Low = THL;
    }
    else if(VA_grp == VA_GRP2H)
    {
        g_tVA_Data.VASettings.Vacc.GRP2.ThresH.High = THH;
        g_tVA_Data.VASettings.Vacc.GRP2.ThresH.Low = THL;
    }
    else if(VA_grp == VA_GRP2V)
    {
        g_tVA_Data.VASettings.Vacc.GRP2.ThresV.High = THH;
        g_tVA_Data.VASettings.Vacc.GRP2.ThresV.Low = THL;
    }
}

static void AF_SetVA_Enable(BOOL enable, VA_WINIDX VA_win)
{
    if(VA_win == VA_WIN_16X16)
        g_tVA_Data.VAOutEn = enable;
    else if(VA_win == VA_WIN_INDP0)
        g_tVA_Data.VAWinInd.Win0En = enable;
    else if(VA_win == VA_WIN_INDP1)
        g_tVA_Data.VAWinInd.Win1En = enable;
    else if(VA_win == VA_WIN_INDP2)
        g_tVA_Data.VAWinInd.Win2En = enable;
}

static void AF_SetVA_Grp0Src(EDGE_OUT Grp0, EDGE_SRC Grp1_2)
{
    g_tVA_Data.VASettings.Vacc.G0SRC = Grp0;
    g_tVA_Data.VASettings.Vdet.VdetSel = Grp1_2;
}

static void AF_SetVA_EdgeParam(VA_GRP VA_grp ,VD_KER edge_ker)
{
    VD_KER* GrpSel;

    if(VA_grp == VA_GRP1H)
        GrpSel = &g_tVA_Data.VASettings.Vdet.GRP1_H;
    else if(VA_grp == VA_GRP1V)
        GrpSel = &g_tVA_Data.VASettings.Vdet.GRP1_V;
    else if(VA_grp == VA_GRP2H)
        GrpSel = &g_tVA_Data.VASettings.Vdet.GRP2_H;
    else if(VA_grp == VA_GRP2V)
        GrpSel = &g_tVA_Data.VASettings.Vdet.GRP2_V;
    else
    {
        DBG_MSG("unknown VA_grp value\r\n");
        return;
    }

    memcpy(GrpSel ,&edge_ker, sizeof(VD_KER));
}

void AF_SetVAparam2IPL(void) // please call this api after setting whole parameters or changing parameters
{
    g_tVA_Data.VASettings.Vacc.ManualUpdateEn = VACC_AUTO_UPDATED;

    if(IPL_AlgGetUIInfo(IPL_SEL_AFWINDOW) == AFALG_WINDOW_CENTER)
        g_tVA_Data.VASettings.Vacc.VAOutSel = GRP0_1_2;
    else // Use Group 0 only if Muiti window AF.
        g_tVA_Data.VASettings.Vacc.VAOutSel = GRP0_only;

    IPL_SetCmd(IPL_SET_VA, (void *)&g_tVA_Data);
    IPL_WaitCmdFinish();
}

void AF_GetVA_Result(UINT32 *G0, UINT32 *G1H, UINT32 *G1V, UINT32 *G2H, UINT32 *G2V, VA_WINIDX WinIdx)
{
    IPL_VA_INFOR VAInfo;

    VAInfo.Grp0 = G0;
    VAInfo.Grp1H = G1H;
    VAInfo.Grp1V = G1V;
    VAInfo.Grp2H = G2H;
    VAInfo.Grp2V = G2V;
    VAInfo.VAWin = WinIdx;
    if(WinIdx == VA_WIN_16X16)
        VAInfo.Length = 256;
    else
        VAInfo.Length = 1;
    VAInfo.VAoutputFmt = g_tVA_Data.VASettings.Vacc.VAOutSel;

    IPL_GetCmd(IPL_GET_CUR_VA_INFOR, (void *)&VAInfo);
}

static UINT32 AF_CalVA_ThresholdbyISO(void)
{
//    UINT32 IsoIdx;
    UINT32 ISO, ExpT, Iris;
    UINT32 uiThreshold = 0;
    AE_GetPrvAEArg(&ISO, &ExpT, &Iris);

    if ( ISO <= 150 )
    {
        //IsoIdx =  PRVISO0100;
        uiThreshold = 0;
    }
    else if ( ISO <= 300 )
    {
        //IsoIdx =  PRVISO0200;
        uiThreshold = 0;
    }
    else if ( ISO <= 600 )
    {
        //IsoIdx =  PRVISO0400;
        uiThreshold = 0;
    }
    else if ( ISO <= 1200 )
    {
        //IsoIdx =  PRVISO0800;
        uiThreshold = 0;
    }
    else
    {
        //IsoIdx =  PRVISO1600;
        uiThreshold = 1;
    }

    DBG_MSG("ISO = %d, Th = %d\r\n", ISO, uiThreshold);

    return uiThreshold;
}

#define AF_VA_DIV          4
#define AF_VA_THRESHOLD    1
#define AF_VA_WIN_W        432 //511 maximum
#define AF_VA_WIN_H        255 //255 maximum
#define AF_VA_WIN_STX      ((Width - AF_VA_WIN_W) >> 1)
#define AF_VA_WIN_STY      ((Height - AF_VA_WIN_H) >> 1)
extern void IPL_DzoomGetPREInSize(UINT32 Id, UINT32 Idx, UINT32 *SizeH, UINT32 *SizeV);
extern UINT32 IPL_DzoomGetIndex(UINT32 Id);
void AF_VA_Setup(void)
{
    UINT32 Width, Height;
    VA_WIN VA_Win_Info;
    VD_KER VA_Ker;

    IPL_DzoomGetPREInSize(IPL_ID_1, IPL_DzoomGetIndex(IPL_ID_1), &Width, &Height);
    DBG_MSG("w:%d, h:%d, stx:%d, sty:%d\r\n", Width, Height, AF_VA_WIN_STX, AF_VA_WIN_STY);

    #if 0
    VA_Win_Info.stx = 6;
    VA_Win_Info.sty = 8;
    VA_Win_Info.szh = 20;
    VA_Win_Info.szv = 10;
    VA_Win_Info.vanf = 1;
    AF_SetVA_window(VA_WIN_16X16, VA_Win_Info);
    #else
    VA_Win_Info.stx = 0;
    VA_Win_Info.sty = 0;
    VA_Win_Info.szh = (Width >> 4);
    VA_Win_Info.szv = (Height >> 4);
    VA_Win_Info.vanf = 7; //1;
    AF_SetVA_window(VA_WIN_16X16, VA_Win_Info);
    #endif

    VA_Win_Info.stx = AF_VA_WIN_STX;
    VA_Win_Info.sty = AF_VA_WIN_STY;
    VA_Win_Info.szh = AF_VA_WIN_W;
    VA_Win_Info.szv = AF_VA_WIN_H;
    VA_Win_Info.vanf = 6; //1
    AF_SetVA_window(VA_WIN_INDP0, VA_Win_Info);

    VA_Win_Info.stx = 4;
    VA_Win_Info.sty = 6;
    VA_Win_Info.szh = 40;
    VA_Win_Info.szv = 20;
    VA_Win_Info.vanf = 1;
    AF_SetVA_window(VA_WIN_INDP1, VA_Win_Info);

    VA_Win_Info.stx = 20;
    VA_Win_Info.sty = 40;
    VA_Win_Info.szh = 20;
    VA_Win_Info.szv = 10;
    VA_Win_Info.vanf = 1;
    AF_SetVA_window(VA_WIN_INDP2, VA_Win_Info);

    #if 0
    AF_SetVA_Threshold(VA_GRP0, 2, 3);
    #else
    AF_SetVA_Threshold(VA_GRP0, AF_CalVA_ThresholdbyISO(), AF_CalVA_ThresholdbyISO());
    #endif
    AF_SetVA_Threshold(VA_GRP1H, AF_CalVA_ThresholdbyISO(), AF_CalVA_ThresholdbyISO());
    AF_SetVA_Threshold(VA_GRP1V, AF_CalVA_ThresholdbyISO(), AF_CalVA_ThresholdbyISO());
    AF_SetVA_Threshold(VA_GRP2H, AF_CalVA_ThresholdbyISO(), AF_CalVA_ThresholdbyISO());
    AF_SetVA_Threshold(VA_GRP2V, AF_CalVA_ThresholdbyISO(), AF_CalVA_ThresholdbyISO());
    AF_SetVA_Enable(1,VA_WIN_16X16);
    AF_SetVA_Enable(1,VA_WIN_INDP0);
    AF_SetVA_Enable(0,VA_WIN_INDP1);
    AF_SetVA_Enable(0,VA_WIN_INDP2);

    #if 0
    AF_SetVA_Grp0Src(EdgeKer_7x7_OUT, E_POS_GAMMA);
    #else
    AF_SetVA_Grp0Src(ESD_OUT, E_POS_GAMMA);
    #endif

    VA_Ker.VdFilt = Filtering_5;
    VA_Ker.Coef.A = 2;
    VA_Ker.Coef.B = -1;
    VA_Ker.Coef.C = 0;
    VA_Ker.Coef.D = 0;
    VA_Ker.Div = AF_VA_DIV;
    AF_SetVA_EdgeParam(VA_GRP1H, VA_Ker);

    VA_Ker.VdFilt = Filtering_5;
    VA_Ker.Coef.A = 2;
    VA_Ker.Coef.B = -1;
    VA_Ker.Coef.C = 0;
    VA_Ker.Coef.D = 0;
    VA_Ker.Div = AF_VA_DIV;
    AF_SetVA_EdgeParam(VA_GRP1V, VA_Ker);

    VA_Ker.VdFilt = Filtering_5;
    VA_Ker.Coef.A = 2;
    VA_Ker.Coef.B = 0;
    VA_Ker.Coef.C = -1;
    VA_Ker.Coef.D = 0;
    VA_Ker.Div = AF_VA_DIV;
    AF_SetVA_EdgeParam(VA_GRP2H, VA_Ker);

    VA_Ker.VdFilt = Filtering_5;
    VA_Ker.Coef.A = 2;
    VA_Ker.Coef.B = 0;
    VA_Ker.Coef.C = -1;
    VA_Ker.Coef.D = 0;
    VA_Ker.Div = AF_VA_DIV;
    AF_SetVA_EdgeParam(VA_GRP2V, VA_Ker);

    AF_SetVAparam2IPL();
}

