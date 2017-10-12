#ifndef _IPL_DEBUG_INT_H_
#define _IPL_DEBUG_INT_H_
/**
    IPL_Debug_Int.h


    @file       IPL_Debug_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#define __MODULE__ IPL
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

#define WEAK_DUMMY_CALL(Name,Call)              \
void Name##IPL_DUMMY_FUNC (void);               \
void Name##IPL_DUMMY_FUNC (void)                \
{                                               \
    extern void Call##IPL_DUMMY_FUNC (void);    \
    Call##IPL_DUMMY_FUNC();                     \
}

#endif //_IPL_DEBUG_INT_H_
