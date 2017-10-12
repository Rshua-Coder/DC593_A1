/**
    IPL post proccess task.

    @file       IPL_PostProcTask_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_POST_PROC_HDR_INT_H_
#define _IPL_POST_PROC_HDR_INT_H_

#include "IPL_Utility.h"

typedef struct
{
    IPL_YCC_CH_INFO InImg;
    IPL_YUV_IMG_TYPE InImgtype;
    IPL_YCC_CH_INFO refImg;
} PPOST_HDR_INFO;

extern BOOL IPL_PPostHDR_IsLock(void);
extern ER IPL_PPostHDR_Proc(IPL_YCC_IMG_INFO *Info, UINT32 WorkBufAddr, UINT32 WorkBufSize, UINT32 *pExtData);
extern UINT32 IPL_PPostHDR_GetMaxWorkBufSize(PPOST_HDR_INFO *Info);

#endif //_IPL_POST_PROC_HDR_INT_H_
