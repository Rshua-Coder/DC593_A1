/**
    IPL post proccess (Photo Frame).

    @file       IPL_PostProcPF_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_POST_PROC_PF_INT_H_
#define _IPL_POST_PROC_PF_INT_H_

#include "IPL_Utility.h"

extern BOOL IPL_PPostPF_IsLock(void);
extern ER IPL_PPostPF_Proc(IPL_YCC_IMG_INFO *Info, UINT32 WorkBufAddr, UINT32 WorkBufSize, UINT32 *pExtData);

#endif //_IPL_POST_PROC_DS_INT_H_
