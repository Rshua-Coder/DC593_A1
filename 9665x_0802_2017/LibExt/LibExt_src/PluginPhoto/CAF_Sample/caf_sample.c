#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Type.h"
#include "debug.h"
#include "ipe_lib.h"
#include "Lens.h"
#include "af_alg.h"
#include "Utility.h"
#include "GxFlash.h"
#include "IPL_Cmd.h"
#include "IPL_Ctrl.h"
#include "ae_api.h"
#include "af_api.h"
#include "afdrv_common.h"
#include "caf_sample.h"
#include "ImgStatLib.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          CAFSAMPLE
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


/*-------------------------------------------------------------------------

                  Internal Structure

---------------------------------------------------------------------------*/
typedef enum
{
    CAF_DIRECTION_UNKNOW,
    CAF_DIRECTION_FWD,            ///< Used to forward focus.
    CAF_DIRECTION_BWD,            ///< Used to backward focus.
} CAF_DIRECT;


typedef struct _CAFALG_PARAM
{
    MOTOR_FOCUS_ACT RT_AF_Direction;
    CAF_DIRECT Init_Direction;
    UINT32 RT_AF_Status;
    INT16  RT_AF_MinIndex;
    INT16  RT_AF_MaxIndex;
    UINT32 RT_AF_Step;
    UINT32 RT_AF_BestIdx;
    BOOL   Inv_Direction;
    UINT32 Damp_CNT;
    UINT32 DampCnt_TH;
    UINT32 Boundary_CNT;
    UINT32 Boundary_TH;
    BOOL b_Falling;
}CAFALG_PARAM;

#define MOTO_EXC_ALWAYS 1

static CAF_PARAM Caf_Param = {0};
static CAFALG_PARAM CafAlg_Param = {0};
static UINT32 VACnt = 0, LastAF_Value = 0;

static BOOL InitialFlag = TRUE;
static BOOL Scene_Change = FALSE;
//static BOOL FocusStep_Strong = FALSE;
static UINT32 Scene_Change_CNT = 0;
static UINT32 Ae_Stable_CNT = 0;
static BOOL VAEnableFlag = FALSE;
static UINT32 ZoomSection = 1;
static BOOL CAF_Stable = FALSE;
static UINT32 VA_Stable_CNT = 0, VA_Change_CNT = 0;
static UINT32 CAF_MaxVA= 0, CAF_MinVA=0xffff;
CAF_DIRECT AF_SrchDir[5] = {CAF_DIRECTION_UNKNOW, CAF_DIRECTION_UNKNOW, CAF_DIRECTION_UNKNOW, CAF_DIRECTION_UNKNOW, CAF_DIRECTION_UNKNOW};
UINT32 Dir_Idx = 0, PrvFocusPosition = 0, CurFocusPosition = 0;


#if 0 
static BOOL FocusTrack_Enable = FALSE;
static UINT32 Tracking_StableCnt=0;
#endif
void caf_setInitFlag(BOOL flag);

/*-------------------------------------------------------------------------

                  Internal static function

---------------------------------------------------------------------------*/
static void caf_SetSearchRange(INT16 MinIndex, INT16 MaxIndex)
{
    INT16 IdxSwitch;

    if (MaxIndex < MinIndex)
    {
        IdxSwitch = MaxIndex;
        MaxIndex = MinIndex;
        MinIndex = IdxSwitch;
    }
    CafAlg_Param.RT_AF_MinIndex = MinIndex;
    CafAlg_Param.RT_AF_MaxIndex = MaxIndex;
}

static void caf_InitParam(void)
{
    CafAlg_Param.RT_AF_Status = 0;
    Caf_Param.Index_Cnt = 0;
    CafAlg_Param.Damp_CNT = 0;
    CafAlg_Param.b_Falling = FALSE;
    VA_Stable_CNT = 0;
    Ae_Stable_CNT = 0;
    Scene_Change_CNT = 0;

    if(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)>=(Lens_Zoom_GetSection(ZOOM_MAX_SECTION)-1))
        CafAlg_Param.DampCnt_TH = 3;
    else
        CafAlg_Param.DampCnt_TH = 2;

    CafAlg_Param.Boundary_TH = 2;

    CAF_MaxVA= 0;
    CAF_MinVA=0xffff;
}

static void caf_Initial(void)
{
    AFALG_INFO_IN alg_info;


    /* Setup IPE VA. */
    AF_VA_Setup();
    AFdrv_GetCurrentRange(&alg_info, SEL_AFMODE_CONTINUEFOCUS);
    caf_SetSearchRange(alg_info.iStartIdx, alg_info.iEndIdx);
    //DBG_MSG("RTAF_MinIndex = %d RTAF_MaxIndex = %d\r\n", RTAF_MinIndex, RTAF_MaxIndex);

    caf_InitParam();
}

static UINT32 caf_GetFocusStep(void)
{
    UINT16 CAF_Step[11] = {3, 4, 5, 6, 7, 9, 11, 14, 17, 20, 20};
    //UINT16 CAF_Step[11] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};


    return CAF_Step[Lens_Zoom_GetSection(ZOOM_CUR_SECTION)];
}

static INT16 caf_GetNextFocusPos(MOTOR_FOCUS_ACT dir, UINT32 step)
{
    INT16 NextIdx;
    if(dir == MOTOR_FOCUS_FWD)
        NextIdx=Lens_Focus_GetPosition() + step;
    else
        NextIdx=Lens_Focus_GetPosition() - step;

    if(NextIdx > CafAlg_Param.RT_AF_MaxIndex)
        NextIdx = CafAlg_Param.RT_AF_MaxIndex;
    else if(NextIdx < CafAlg_Param.RT_AF_MinIndex)
        NextIdx = CafAlg_Param.RT_AF_MinIndex;

    DBG_MSG("LENS GO %d %d %d\r\n",NextIdx,CafAlg_Param.RT_AF_MaxIndex,CafAlg_Param.RT_AF_MinIndex);

    return NextIdx;
}

static UINT32 caf_GetMaxIndex(void)
{
    UINT32 i, BestIdx = Caf_Param.Index_Cnt, VAMAX = 0;

    //Sometimes 0 is unstable
    for(i=2; i<=Caf_Param.Index_Cnt; i++)
    {
        if(Caf_Param.uiVAValue[i] > VAMAX)
        {
            VAMAX = Caf_Param.uiVAValue[i];
            BestIdx = i;
        }
        //DBG_MSG("i %d VA %d\r\n",i,Caf_Param.uiVAValue[i]);
    }
    DBG_MSG("MAX =%d VA %d\r\n",BestIdx,Caf_Param.uiVAValue[BestIdx]);
    return BestIdx;
}

static void caf_AddIndex(void)
{
    if(Caf_Param.Index_Cnt >= (CAF_TOTAL_STEP-1))
        Caf_Param.Index_Cnt = 0;

    Caf_Param.Index_Cnt ++ ;
}


static void caf_Algorithm(UINT32 AF_Value)
{
    UINT32 VAFirst = 0, VASecond = 0, VALast = 0;
    INT16 NextIdx = 0, BestIdx;
    BOOL ValidCurve = FALSE;

    CafAlg_Param.RT_AF_Step = caf_GetFocusStep();

    //DBG_MSG("min=%d, max=%d\r\n",CafAlg_Param.RT_AF_MinIndex,CafAlg_Param.RT_AF_MaxIndex);
    DBG_MSG("%d: Status=%d, Focus=%d, VA=%d, MinVA=%d, MaxVa=%d\r\n",Caf_Param.Index_Cnt,CafAlg_Param.RT_AF_Status,Lens_Focus_GetPosition(),AF_Value, CAF_MinVA, CAF_MaxVA);
    Caf_Param.uiStatus[Caf_Param.Index_Cnt] = CafAlg_Param.RT_AF_Status;
    Caf_Param.iFocusPos[Caf_Param.Index_Cnt] = Lens_Focus_GetPosition();
    Caf_Param.uiVAValue[Caf_Param.Index_Cnt] = AF_Value;


    if((AF_Value > CAF_MaxVA) && (Caf_Param.Index_Cnt >= 2))
        CAF_MaxVA = AF_Value;

    if((AF_Value < CAF_MinVA) && (Caf_Param.Index_Cnt >= 2))
        CAF_MinVA = AF_Value;

    if((CAF_MaxVA*100) > (CAF_MinVA*101))
        ValidCurve = TRUE;

    
    switch(CafAlg_Param.RT_AF_Status)
    {
        //---------------------------------------------------------
        // Step0: Get VA and Guess direction
        //---------------------------------------------------------
        case 0:
            if(!CafAlg_Param.Inv_Direction)
            {
                switch(CafAlg_Param.Init_Direction)
                {
                    case CAF_DIRECTION_FWD:
                        CafAlg_Param.RT_AF_Direction = MOTOR_FOCUS_FWD;
                        break;
                    case CAF_DIRECTION_BWD:
                        CafAlg_Param.RT_AF_Direction = MOTOR_FOCUS_BWD;
                        break;
                    case CAF_DIRECTION_UNKNOW:
                        if(AFdrv_GetFocusDist()>80)
                            CafAlg_Param.RT_AF_Direction = MOTOR_FOCUS_FWD;
                        else
                            CafAlg_Param.RT_AF_Direction = MOTOR_FOCUS_BWD;
                        break;
                }
                DBG_MSG("af %d\r\n",AFdrv_GetFocusDist());
            }
            else
            {
                CafAlg_Param.Inv_Direction = FALSE;
            }

            NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction, CafAlg_Param.RT_AF_Step);

#if MOTO_EXC_ALWAYS
            Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, NextIdx);
#else
            Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN, NextIdx);
#endif

            CafAlg_Param.RT_AF_Status ++;
            break;
        case 1:
            //---------------------------------------------------------
            // Step1: Continue Go step and record VA
            //---------------------------------------------------------
            NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,(CafAlg_Param.RT_AF_Step));

#if MOTO_EXC_ALWAYS
            Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, NextIdx);
#else
            Lens_Focus_DoAction(FOCUS_RUN, NextIdx);
#endif
            CafAlg_Param.RT_AF_Status++;
            break;
        case 2:
            //---------------------------------------------------------
            // Step2: Continue Go step and record VA
            //---------------------------------------------------------
            NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,(CafAlg_Param.RT_AF_Step));

#if MOTO_EXC_ALWAYS
            Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, NextIdx);
#else
            Lens_Focus_DoAction(FOCUS_RUN, NextIdx);
#endif
            CafAlg_Param.RT_AF_Status ++;
            break;

        case 3:
            //---------------------------------------------------------
            // Step3: Continue Go step and record VA
            //---------------------------------------------------------
            NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,(CafAlg_Param.RT_AF_Step));

#if MOTO_EXC_ALWAYS
            Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, NextIdx);
#else
            Lens_Focus_DoAction(FOCUS_RUN, NextIdx);
#endif
            CafAlg_Param.RT_AF_Status ++;
            break;
        case 4:
            //---------------------------------------------------------
            // Step4: record VA and judge direction
            //---------------------------------------------------------
            if(Caf_Param.Index_Cnt >= 4)
            {
                //VAFirst = (Caf_Param.uiVAValue[Caf_Param.Index_Cnt-2]);
                //VASecond = (Caf_Param.uiVAValue[Caf_Param.Index_Cnt-2]+Caf_Param.uiVAValue[Caf_Param.Index_Cnt-1])/2;
                //VALast = (Caf_Param.uiVAValue[Caf_Param.Index_Cnt-1]+Caf_Param.uiVAValue[Caf_Param.Index_Cnt])/2;
                VAFirst = Caf_Param.uiVAValue[Caf_Param.Index_Cnt-2];
                VASecond = Caf_Param.uiVAValue[Caf_Param.Index_Cnt-1];
                VALast = Caf_Param.uiVAValue[Caf_Param.Index_Cnt];
            }

            //VA curve is falling, direction is wrong, invert and back to 0!.
            if( (VALast < VASecond) && (VASecond < VAFirst) )
            {
                if(CafAlg_Param.b_Falling == FALSE)
                {
                    CafAlg_Param.RT_AF_Direction = (CafAlg_Param.RT_AF_Direction+1)%2;
                    NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,(CafAlg_Param.RT_AF_Step*2));
#if MOTO_EXC_ALWAYS
                    Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, NextIdx);
#else
                    Lens_Focus_DoAction(FOCUS_RUN|FOCUS_POSTEXC, NextIdx);
#endif
                    CafAlg_Param.Inv_Direction = TRUE;
                    CafAlg_Param.RT_AF_Status = 0;
                    CafAlg_Param.b_Falling = TRUE;
                }
                else
                {
                    CafAlg_Param.b_Falling = FALSE;
                }
            }
            //VA curve is rising, direction is right, keep going and go to 4!
            else if( (VALast >= VASecond) && (VASecond >= VAFirst) )
            {
                NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,CafAlg_Param.RT_AF_Step);
                
#if MOTO_EXC_ALWAYS
                Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, NextIdx);
#else
                Lens_Focus_DoAction(FOCUS_RUN, NextIdx);
#endif
                CafAlg_Param.RT_AF_Status ++;
            }
            else if( (VALast < VASecond) && (VASecond > VAFirst) )
            {
                if(ValidCurve == TRUE)
                {
                    CafAlg_Param.RT_AF_BestIdx = caf_GetMaxIndex();
                    CafAlg_Param.RT_AF_Status = 6;
                }
                else
                {
                    CafAlg_Param.Damp_CNT ++;
                    if (CafAlg_Param.Damp_CNT > CafAlg_Param.DampCnt_TH)
                    {
                        CafAlg_Param.RT_AF_BestIdx = caf_GetMaxIndex();
                        CafAlg_Param.Damp_CNT = 0;
                        CafAlg_Param.RT_AF_Status = 6;
                    }
                }
            }
            //VA curve is damping, keep going to check again
            else
            {
                CafAlg_Param.Damp_CNT ++;
                //More than damping (TH) times, check max VA index.
                if (CafAlg_Param.Damp_CNT > CafAlg_Param.DampCnt_TH)
                {
                    CafAlg_Param.RT_AF_BestIdx = caf_GetMaxIndex();
                    CafAlg_Param.RT_AF_Status = 6;
                    CafAlg_Param.Damp_CNT = 0;
                }
                else
                {
                    NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,(CafAlg_Param.RT_AF_Step));
                    
#if MOTO_EXC_ALWAYS
                    Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, NextIdx);
#else
                    Lens_Focus_DoAction(FOCUS_RUN, NextIdx);
#endif
                }
            }
            break;
        case 5:
            //---------------------------------------------------------
            // Step5: VA is rising, keep going until falling.
            //---------------------------------------------------------
            if(Caf_Param.Index_Cnt >= 4)
            {
                VAFirst = Caf_Param.uiVAValue[Caf_Param.Index_Cnt-2];
                VASecond = Caf_Param.uiVAValue[Caf_Param.Index_Cnt-1];
                VALast = Caf_Param.uiVAValue[Caf_Param.Index_Cnt];
            }

            //VA curve is falling, Find Best index!.
            if( (VALast < VASecond) && (VASecond < VAFirst) )
            {
                if(ValidCurve == TRUE)
                {
                CafAlg_Param.RT_AF_BestIdx = caf_GetMaxIndex();
                    CafAlg_Param.RT_AF_Status = 6;
                }
                else
                {
                    CafAlg_Param.Damp_CNT ++;
                    if (CafAlg_Param.Damp_CNT > CafAlg_Param.DampCnt_TH)
                    {
                        CafAlg_Param.RT_AF_BestIdx = caf_GetMaxIndex();
                        CafAlg_Param.RT_AF_Status = 6;
                        CafAlg_Param.Damp_CNT = 0;
                    }
                }
            }
            //VA curve is rising, keep going.
            else if( (VALast >= VASecond) && (VASecond >= VAFirst) )
            {
                NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,CafAlg_Param.RT_AF_Step);
                if(NextIdx >= CafAlg_Param.RT_AF_MaxIndex || NextIdx <= CafAlg_Param.RT_AF_MinIndex)
                {
                    if(CafAlg_Param.Boundary_CNT > CafAlg_Param.Boundary_TH)
                    {
                        CafAlg_Param.RT_AF_BestIdx = Caf_Param.Index_Cnt -1;
                        CafAlg_Param.RT_AF_Status = 6;
                        CafAlg_Param.Boundary_CNT = 0;
                    }
                    else
                    {
                        //caf_setInitFlag(TRUE);
                    }
                    DBG_ERR("CAF: Focus out of boundary %d!!!!\r\n",NextIdx);
                    CafAlg_Param.Boundary_CNT++;
                }
#if MOTO_EXC_ALWAYS
                Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, NextIdx);
#else
                Lens_Focus_DoAction(FOCUS_RUN, NextIdx);
#endif
            }
            //VA curve is damping, keep going to check again
            else
            {
                CafAlg_Param.Damp_CNT ++;
                //More than damping (TH) times, check max VA index.
                if (CafAlg_Param.Damp_CNT > CafAlg_Param.DampCnt_TH)
                {
                    CafAlg_Param.RT_AF_BestIdx = caf_GetMaxIndex();
                    CafAlg_Param.RT_AF_Status = 6;
                    CafAlg_Param.Damp_CNT = 0;
                }
                else
                {
                    NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,(CafAlg_Param.RT_AF_Step));
                    if(NextIdx >= CafAlg_Param.RT_AF_MaxIndex || NextIdx <= CafAlg_Param.RT_AF_MinIndex)
                    {
                        if(CafAlg_Param.Boundary_CNT > CafAlg_Param.Boundary_TH)
                        {
                            CafAlg_Param.RT_AF_BestIdx = Caf_Param.Index_Cnt -1;
                            CafAlg_Param.RT_AF_Status = 6;
                            CafAlg_Param.Boundary_CNT = 0;
                        }
                        else
                        {
                            //caf_setInitFlag(TRUE);
                        }
                        DBG_ERR("CAF: Focus out of boundary %d!!!!\r\n",NextIdx);
                        CafAlg_Param.Boundary_CNT++;
                    }
#if MOTO_EXC_ALWAYS
                    Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, NextIdx);
#else
                    Lens_Focus_DoAction(FOCUS_RUN, NextIdx);
#endif
                }
            }
            break;
        case 6:

            if(CafAlg_Param.RT_AF_Direction == MOTOR_FOCUS_FWD)
                BestIdx = AFALG_FindPeak(Caf_Param.uiVAValue[CafAlg_Param.RT_AF_BestIdx-1], Caf_Param.uiVAValue[CafAlg_Param.RT_AF_BestIdx], Caf_Param.uiVAValue[CafAlg_Param.RT_AF_BestIdx+1], Caf_Param.iFocusPos[CafAlg_Param.RT_AF_BestIdx-2], CafAlg_Param.RT_AF_Step);
            else
                BestIdx = AFALG_FindPeak(Caf_Param.uiVAValue[CafAlg_Param.RT_AF_BestIdx+1], Caf_Param.uiVAValue[CafAlg_Param.RT_AF_BestIdx], Caf_Param.uiVAValue[CafAlg_Param.RT_AF_BestIdx-1], Caf_Param.iFocusPos[CafAlg_Param.RT_AF_BestIdx-2], CafAlg_Param.RT_AF_Step);

#if MOTO_EXC_ALWAYS
            Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, BestIdx);
#else
            Lens_Focus_DoAction(FOCUS_RUN|FOCUS_POSTEXC, BestIdx);
#endif
            CafAlg_Param.RT_AF_Status ++;
            LastAF_Value = Caf_Param.uiVAValue[CafAlg_Param.RT_AF_BestIdx];
            DBG_MSG("MAXIdx=%d, BestFocus=%d\r\n",CafAlg_Param.RT_AF_BestIdx, BestIdx);
            break;

        case 7:
            if ((AF_Value > LastAF_Value * 50 / 100) && (AF_Value < LastAF_Value * 150 / 100))
            {

            }
            else
            {
                Scene_Change = TRUE;
            }

            //LastAF_Value = AF_Value;
            break;
    }
    VACnt = GetIpeVACnt();
    caf_AddIndex();
}

/*-------------------------------------------------------------------------

                  external function

---------------------------------------------------------------------------*/
PCAF_PARAM caf_GetINFO(void)
{
    return &Caf_Param;
}

void caf_PrintCAFInfo(void)
{
    UINT32 i;

    DBG_MSG("Focus: ");
    for(i=0; i<=Caf_Param.Index_Cnt; i++)
    {
        DBG_MSG("%d\t",Caf_Param.iFocusPos[i]);
    }
    DBG_MSG("\r\n");
    DBG_MSG("VA: ");
    for(i=0; i<=Caf_Param.Index_Cnt; i++)
    {
        DBG_MSG("%d\t",Caf_Param.uiVAValue[i]);
    }
}

void caf_setInitFlag(BOOL flag)
{
    InitialFlag = flag;
}

BOOL caf_GetVAFlag(void)
{
    return VAEnableFlag;
}

void caf_SetVAFlag(BOOL EN)
{
    VAEnableFlag = EN;
}

void caf_VAPprepare(BOOL flag)
{
}


#define Center_Block_Start 5 // center blocks = 22 * 22 = 484
#define Center_Block_End 27 // outer blocks = 1024-484 = 540
UINT16 Cur_r[1024],Cur_g[1024],Cur_b[1024];   //ca 8x8 windows
UINT16 Pre_g[1024];
static void MoveDetection(UINT32 DiffThreshold, UINT32 *TotalDiff, UINT32 *CenterDiff)
{
    UINT32 i, j,Diff, DiffCnt=0, DiffCnt_center=0,idx;

    Img_GetCA_Result(IPL_ID_1,Cur_r,Cur_g,Cur_b,1024);

    for(j=0;j<32;j++)
    {
        for(i=0; i<32; i++)
        {
            idx = j*32+i;
            if(Pre_g[idx]>Cur_g[idx])
                Diff = (Pre_g[idx]-Cur_g[idx])>>4;
            else
                Diff = (Cur_g[idx]-Pre_g[idx])>>4;

            if(Diff > DiffThreshold)
            {
                DiffCnt ++;
                if((j>=Center_Block_Start) && (j<Center_Block_End) && (i>=Center_Block_Start) && (i<Center_Block_End))
                    DiffCnt_center ++;
            }
            Pre_g[idx] = Cur_g[idx];
        }
    }
    *TotalDiff = DiffCnt;
    *CenterDiff = DiffCnt_center;
}

#include "IPL_Utility.h"

UINT32 LastStableEV=0, CurStableEV=0, LastStableVA=0;
void caf_AutoFocusProcess(void)
{
    static UINT32 PrevEV = 99999;
    UINT32 CurEV, CurVA;
    UINT32 uiG0, uiG1h, uiG1v, uiG2h, uiG2v;
    UINT32 TotalDiff, CenterDiff;
    static UINT32 StableCnt = 0;


    MoveDetection(5, &TotalDiff, &CenterDiff);

    //DBG_ERR(("Diff Total=%d, Center=%d\r\n",TotalDiff,CenterDiff));

    CurEV = AE_GetPrvEVValue(); // 70 = LV7.0
    #if 0
    CurVA = caf_GetVA();
    #else // Should do AF_VA_Setup() first?
    AF_GetVA_Result(&uiG0, &uiG1h, &uiG1v, &uiG2h, &uiG2v, VA_WIN_INDP0);
    CurVA = uiG0;
    #endif
//DBG_ERR(("%d %d\r\n",CurEV,PrevEV));
    if (Lens_Zoom_GetSection(ZOOM_CUR_SECTION)!=ZoomSection)
    {
        ZoomSection = Lens_Zoom_GetSection(ZOOM_CUR_SECTION);
        InitialFlag = TRUE;
        //CAF_Stable = FALSE;
        //DBG_MSG("----->Zoom Change %d\r\n",ZoomSection);
    }
    if(CafAlg_Param.RT_AF_Status == 7)
    {
        if(((TotalDiff - CenterDiff) < 20) && (CenterDiff > (30+ZoomSection*3)))
        {
            InitialFlag = TRUE;
            //DBG_ERR("1: %d %d\r\n",CenterDiff, (TotalDiff - CenterDiff));
        }
        else
        {
            if((CurVA < (LastStableVA*10/(12+ZoomSection/2))) || (CurVA > (LastStableVA*(12+ZoomSection/2)/10)))
            {
                if((VA_Change_CNT > (10+ZoomSection)) && ((TotalDiff - CenterDiff)<20))
                    InitialFlag = TRUE;
                else
                    VA_Change_CNT++;
                //DBG_ERR("2: %d %d %d %d\r\n",VA_Change_CNT,(TotalDiff - CenterDiff),CurVA,LastStableVA);
            }
            else
                VA_Change_CNT = 0;

            //if(VA_Stable_CNT ++ == 4)
            //    LastStableVA = CurVA;
        }


        if((CurEV > (PrevEV-1)) && (CurEV < (PrevEV+1)))
        {
            //DBG_ERR(("3: %d\r\n",Ae_Stable_CNT));
            Ae_Stable_CNT++;
            if((Ae_Stable_CNT > (5+ZoomSection)) && ((TotalDiff - CenterDiff)<20))
            {
                CurStableEV = CurEV;

                if((CurStableEV < (LastStableEV-1)) || (CurStableEV > (LastStableEV+1)))
                {
                    InitialFlag = TRUE;
                    //DBG_ERR("3: %d %d %d\r\n",CurStableEV,LastStableEV, Ae_Stable_CNT);
                    LastStableEV = CurStableEV;
                }
            }
        }
        else
            Ae_Stable_CNT = 0;
    }
    else
    {
        VA_Stable_CNT = 0;
        VA_Change_CNT = 0;
        Ae_Stable_CNT = 0;
    }


    if (InitialFlag == TRUE)
    {
        PrvFocusPosition = Lens_Focus_GetPosition();
        //PrvDir
        if(Dir_Idx < 2)
        CafAlg_Param.Init_Direction = CAF_DIRECTION_UNKNOW;
        else if((AF_SrchDir[(Dir_Idx-2)%5] == CAF_DIRECTION_FWD) && (AF_SrchDir[(Dir_Idx-1)%5] == CAF_DIRECTION_FWD))
            CafAlg_Param.Init_Direction = CAF_DIRECTION_FWD;
        else if((AF_SrchDir[(Dir_Idx-2)%5] == CAF_DIRECTION_BWD) && (AF_SrchDir[(Dir_Idx-1)%5] == CAF_DIRECTION_BWD))
            CafAlg_Param.Init_Direction = CAF_DIRECTION_BWD;
        else
            CafAlg_Param.Init_Direction = CAF_DIRECTION_UNKNOW;
            
        
        DBG_MSG("----->reset init, Dir=%d\r\n", CafAlg_Param.Init_Direction);
        CurStableEV = CurEV;
        VA_Change_CNT = 0;
        caf_Initial();

        //AvgY = caf_calHist();
        InitialFlag = FALSE;
        CAF_Stable = FALSE;
        StableCnt = 0;
    }
    else if(CAF_Stable == FALSE)
    {
        GxFlash_SuspendCharge();
        caf_Algorithm(CurVA);
        GxFlash_ResumeCharge();
        if(CafAlg_Param.RT_AF_Status == 7)
        {
            CAF_Stable = TRUE;
            LastStableVA = CAF_MaxVA;
            PrevEV = CurEV;
            CurFocusPosition = Lens_Focus_GetPosition();

            if(CurFocusPosition >= PrvFocusPosition)
                AF_SrchDir[Dir_Idx%5] = CAF_DIRECTION_FWD;
            else
                AF_SrchDir[Dir_Idx%5] = CAF_DIRECTION_BWD;
            Dir_Idx ++;
        }
    }
}

#if 0
static BOOL IsTrackingEn(void)
{
    return FocusTrack_Enable;
}

static void SetTrackingFocusEnable(BOOL TrackingEn)
{
    FocusTrack_Enable = TrackingEn;
}

static UINT32 GetTrackingStableCnt(void)
{
    return Tracking_StableCnt;
}
#endif

