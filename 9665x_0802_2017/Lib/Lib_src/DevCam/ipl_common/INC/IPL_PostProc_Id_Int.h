/**
    IPL PostProc Task id.

    .

    @file       IPL_PostProc_id_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_POSTPROC_ID_INT_H_
#define _IPL_POSTPROC_ID_INT_H_

#include "Type.h"

extern void Install_IPLPProcID(void) _SECTION(".kercfg_text");

extern UINT32 _SECTION(".kercfg_data") IPL_PPROC_TaskID;
extern UINT32 _SECTION(".kercfg_data") IPL_PPROC_FlgID;

#endif //_IPL_POSTPROC_ID_INT_H_
