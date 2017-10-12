/**
    Continuous Auto Focus api.


    @file       Caf_sample.h
    @ingroup    mILibAFAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _CAF_SAMPLE_H
#define _CAF_SAMPLE_H

#define CAF_TOTAL_STEP  90 //30

typedef struct _CAF_PARAM
{
    UINT32 uiStatus[CAF_TOTAL_STEP];
    INT32  iFocusPos[CAF_TOTAL_STEP];
    UINT32 uiVAValue[CAF_TOTAL_STEP];
    UINT32 Index_Cnt;
}CAF_PARAM,*PCAF_PARAM;

extern void caf_AutoFocusProcess(void);
extern void caf_setInitFlag(BOOL flag);
extern void caf_VAPprepare(BOOL flag);
//extern UINT16 caf_GetFocusDist(void);
extern UINT32 caf_ChkHist(UINT32 Percent, UINT32 Step);
extern BOOL caf_GetVAFlag(void);
extern void caf_SetVAFlag(BOOL EN);
extern PCAF_PARAM caf_GetINFO(void);
extern void caf_PrintCAFInfo(void);

#endif // _CAF_SAMPLE_H
