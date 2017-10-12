/**
    Public header file for AF algorithm.

    This file is the header file that define the API and data type for AF algorithm.

    @file       af_alg.h
    @ingroup    mILibAFAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _AF_ALG_INT_H_
#define _AF_ALG_INT_H_

#include "Type.h"

/** \addtogroup mIAF */
//@{

#define AF_TOTAL_STEP           256
#define AF_WIN_HW_W             16
#define AF_WIN_HW_H             16
#define AF_WIN_DIMENSION_W      8
#define AF_WIN_DIMENSION_H      8
#define AF_TOTAL_WIN            (AF_WIN_DIMENSION_W * AF_WIN_DIMENSION_H)
#define AF_DEFAULT_WIN          (AF_TOTAL_WIN/2)
#define AF_NOSELECT_WIN         0xFF
#define AF_VA_WINSIZE (256)     ///<16x16

/**
    @name AF algorithm operation result.

     Status code to indicate condition.
*/
//@{
#define AFALG_STS_SUCCESS       0x0    ///< AF result: Success
#define AFALG_STS_TOONEAR       0x1    ///< AF result: Too near
#define AFALG_STS_TOOFAR        0x2    ///< AF result: Too far
#define AFALG_STS_TREMBLE       0x4    ///< AF result: Tremble
#define AFALG_STS_LOWVA         0x8    ///< AF result: Low VA
#define AFALG_STS_NOTARGET      0x10   ///< AF result: No target
#define AFALG_STS_SMOOTH        0x20   ///< AF result: Too smooth
#define AFALG_STS_VASMALL       0x40   ///< AF result: VA too small
//@}

/**
    @name AF algorithm utilities.

     Utilities for math calculation.
*/
//@{
#define AFALG_ABS(X)    (((X) < 0  ) ?-(X) : (X))
#define AFALG_MAX(A,B)  (((A) > (B)) ? (A) : (B))
#define AFALG_MIN(A,B)  (((A) < (B)) ? (A) : (B))
//@}

typedef enum
{
    AF_ALG_SMODE_FULLSEARCH = 0,
    AF_ALG_SMODE_FALLNORMAL,
    AF_ALG_SMODE_FALLOFF_STRONG,
    AF_ALG_SMODE_FALLNORMAL_NOFD,
    ENUM_DUMMY4WORD(AF_ALG_SEARCH_MODE)
}AF_ALG_SEARCH_MODE;

typedef enum {
    AFALG_WINDOW_CENTER,  ///< AF single window
    AFALG_WINDOW_AUTO,    ///< AF multi window
    AFALG_WINDOW_MANUIAL,
    ENUM_DUMMY4WORD(AFALG_WINDOW_SEL)
}AFALG_WINDOW_SEL;

typedef enum
{
    AFALG_STOP = 0,  ///< AF process status: Stop - initial state
    AFALG_RUN,       ///< AF process status: Run  - AF is running
    AFALG_END,       ///< AF process status: End  - AF is finished
    AFALG_QUIT,      ///< AF process status: Quit by UI
    ENUM_DUMMY4WORD(AFALG_STATUS)
}AFALG_STATUS;

typedef struct _AFALG_INFO_IN
{
    AFALG_WINDOW_SEL Window_sel;
    AF_ALG_SEARCH_MODE Search_mode;
    BOOL   bObj;
    INT32  iStartIdx;
    INT32  iEndIdx;
    INT32  iCurIdx;
    UINT32 iStep;
    INT32  iDefaultIdx;
    INT32  iOffsetStep;
}AFALG_INFO_IN, *PAFALG_INFO_IN;

typedef struct _AFALG_INFO_OUT
{
    INT32  iPeak;                       ///< AF peak position
    UINT32 uiSelectWin[AF_TOTAL_WIN];   ///< AF win found peak position
    UINT32 uiStatus;                    ///< AF result
}AFALG_INFO_OUT, *PAFALG_INFO_OUT;

typedef struct _AFALG_INFO
{
    AFALG_INFO_IN  afalg_in;
    AFALG_INFO_OUT afalg_out;
}AFALG_INFO, *PAFALG_INFO;

typedef struct _AF_PARAM
{
    UINT32 uiHWVA[AF_VA_WINSIZE];
    UINT32 uiALGVA[AF_TOTAL_WIN];
    INT32  iFocusPos;
}AF_PARAM;

typedef struct _AFALG_TH
{
    UINT32 uivibrate_th;
    UINT32 uimax_th;
    UINT32 uirun_th;
    UINT32 uitremble_th;
}AFALG_TH;

typedef struct _AFALG_PARAM{
    AFALG_INFO afalginfo;
    AF_PARAM afParam[AF_TOTAL_STEP];
    AFALG_TH afalg_threshold;
    UINT32 uiIdxOverRun;
    UINT32 WindowSelect;
    UINT32 VAMax[AF_TOTAL_WIN];
    UINT32 VAMin[AF_TOTAL_WIN];
    UINT32 VALast[AF_TOTAL_WIN];
    UINT32 WindowScore[AF_TOTAL_WIN];
    UINT32 Curve[AF_TOTAL_WIN][AF_TOTAL_WIN];
}AFALG_PARAM, *PAFALG_PARAM;

extern void AFALG_SetParam(void);

/**
     check af algorithm result.

     @param[in] void
     @return Description of data returned.
         - @b UINT32: AFALG_STS_SUCCESS,
                      AFALG_STS_TOONEAR,
                      AFALG_STS_TOOFAR,
*/
extern UINT32 AFALG_GetAFStatus(void);

extern PAFALG_INFO AFALG_GetINFO(void);
extern void AFALG_SetStatus(AFALG_STATUS Status);
extern UINT32 AFALG_GetInfoIdx(void);
extern void AFALG_SetInfoIdx(UINT32 uiIdx);
extern AFALG_STATUS AFALG_CheckStatus(void);
extern PAFALG_PARAM AFALG_GetAFALGPARAM(void);
extern INT32 AFALG_FindPeak(UINT32 uiVAn1, UINT32 uiVApk, UINT32 uiVAp1, INT32 iIdxpk, UINT32 uiStep);

//@}
#endif
