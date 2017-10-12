/**
    Motion Detection Algorithm.

    depend on CA status to detect the object motion

    @file       md_alg.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "type.h"
#include "md_sample_int.h"
#include "md_alg.h"
#include "md_api.h"
#include "IPL_AlgInfor.h"
#include "IPL_Cmd.h"

#define __MODULE__ MDALG
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"


static UINT32 MDAccTabR[MD_CAWIN_TOTAL];
static UINT32 MDAccTabG[MD_CAWIN_TOTAL];
static UINT32 MDAccTabB[MD_CAWIN_TOTAL];

static void MD_getCA(UINT32 *R, UINT32 *G, UINT32 *B, UINT32 Size)
{
    IPL_CA_INFOR CAInfo;
    static UINT16 RAcc[MD_CAWIN_TOTAL], GAcc[MD_CAWIN_TOTAL], BAcc[MD_CAWIN_TOTAL];
    UINT16 i;
    CAInfo.Id = IPL_ID_1;
    CAInfo.R = RAcc;
    CAInfo.G = GAcc;
    CAInfo.B = BAcc;
    CAInfo.Length = Size;
    IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CAInfo);
    for ( i=0; i< MD_CAWIN_TOTAL; i++ )
    {
        R[i] = (RAcc[i] >> 4);
        G[i] = (GAcc[i] >> 4);
        B[i] = (BAcc[i] >> 4);
    }
}

UINT32 MD_BlkDiff_Thr = MD_BLKDIFF_THR;
UINT32 MD_BlkCnt_Thr = MD_BLKCNT_THR;
UINT32 MD_DiffSum_Thr = MD_DIFFSUM_THR;
void MD_SetLevel(MD_LEVEL level)
{
    MD_BlkDiff_Thr = MD_BLKDIFF_THR * level / 100;
    MD_BlkCnt_Thr = MD_BLKCNT_THR * level / 100;
    MD_DiffSum_Thr = MD_DIFFSUM_THR * level / 100;
}
BOOL MD_Process(void)
{
    BOOL rslt;
    MDALG_INFO MdInfo = {0};
    MdInfo.CAWinX = MD_CAWIN_X;
    MdInfo.CAWinY = MD_CAWIN_Y;
    MdInfo.AccTab[MDALG_CH_R] = MDAccTabR;
    MdInfo.AccTab[MDALG_CH_G] = MDAccTabG;
    MdInfo.AccTab[MDALG_CH_B] = MDAccTabB;
    MdInfo.BlockDiffThres = MD_BlkDiff_Thr;
    MdInfo.BlockCntThres = MD_BlkCnt_Thr;
    MdInfo.TotalDiffThres = MD_DiffSum_Thr;


    MD_getCA(MdInfo.AccTab[MDALG_CH_R], MdInfo.AccTab[MDALG_CH_G], MdInfo.AccTab[MDALG_CH_B], MD_CAWIN_TOTAL);

    rslt = MDAlg_Start((UINT32)NULL, (UINT32)NULL, MdInfo);

    if (rslt == TRUE )
        DBG_ERR("^GObject Motion\r\n");
    //else
        //DBG_ERR("Silence environment\r\n");
    return rslt;
}
