#ifndef _IPL_ALGINFOR_INT_H_
#define _IPL_ALGINFOR_INT_H_
/**
    IPL_AlgInfor_Int.h


    @file       IPL_AlgInfor_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "type.h"
#include "ErrorNo.h"
#include "IPL_AlgInfor.h"
#include "IPL_Utility.h"

typedef void (*IPL_INFOR_EVENT_FP)(UINT32 SelIdx, UINT32 Value);

/**
     register event callback function

     @note when Using IPL_AlgSetUIInfo to change Alg UI information, the callback function can be executed.

     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern ER IPL_AlgSetEventFP(IPL_PROC_ID Id, IPL_SEL_ITEM SelIdx, IPL_INFOR_EVENT_FP fp);

/**
     initial Alg UI information

     @note reset all array information.
           when user want to re-assign buffer, it can be executed at change mode flow.

     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern ER IPL_AlgUIInfoInit(void);

/**
     un-initial Alg UI information

     @note unload Alg UI information. it can be executed at mode close.

     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern ER IPL_AlgUIInfoUnInit(void);


//for compatible single process
#define AlgSetEventFP(SelIdx, fp) IPL_AlgSetEventFP(IPL_ID_1, SelIdx, fp)



#endif //_IPL_ALGINFOR_INT_H_
