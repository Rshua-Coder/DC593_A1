/**
    IPL post proccess task.

    @file       IPL_PostProcTask_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_POST_PROC_TASK_INT_H_
#define _IPL_POST_PROC_TASK_INT_H_

#include "type.h"
#include "IPL_Utility.h"

typedef ER (*IPL_PPROC_FP)(IPL_YCC_IMG_INFO *Info, UINT32 WorkBufAddr, UINT32 WorkBufSize, UINT32 *pExtData);
typedef BOOL (*IPL_PPROCISLOCK_FP)(void);
typedef struct
{
    UINT32 bufaddr;
    UINT32 bufsize;
    IPL_PPROC_FP PProcfp;
    IPL_PPROCISLOCK_FP IsLockfp;
    UINT32 ExtData[5];
}IPL_PPROC_INFO;

/**
    post process information
*/
typedef struct _POST_PROC_INFO
{
    IPL_PROC_ID Id;
    IPL_YCC_IMG_INFO ImgInfo;
} POST_PROC_INFO;

extern ER IPL_PProc_UnLoadAllFuncs(void);
extern ER IPL_PProc_LoadFunc2(IPL_PROC_ID Id, IPL_PPROC_INFO *PProcInfo);
extern ER IPL_PProc_SetCmd2(IPL_PROC_ID Id, IPL_YCC_IMG_INFO *Info);
extern ER IPL_PProc_Open(void);
extern ER IPL_PProc_Close(void);


#define IPL_PProc_LoadFunc(PProcInfo) IPL_PProc_LoadFunc2(IPL_ID_1, PProcInfo)
#define IPL_PProc_SetCmd(Info) IPL_PProc_SetCmd2(IPL_ID_1, Info)

extern void IPL_PProc_Tsk(void);
#endif //_IPL_POST_PROC_TASK_INT_H_