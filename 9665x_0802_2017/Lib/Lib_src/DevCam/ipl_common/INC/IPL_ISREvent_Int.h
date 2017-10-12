#ifndef _IPL_ISREVENT_INT_H_
#define _IPL_ISREVENT_INT_H_
/**
    IPL_ISREvent_Int.h


    @file       IPL_ISREvent_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "IPL_Utility.h"
#include "IPL_IsrEvent.h"
#include "IPL_Cmd.h"

typedef struct
{
    IPL_ISR_EVENT_FP FP[IPL_ISR_MAX_CNT];
} IPL_ISR_EVENT_TAB;

typedef struct
{
    UINT32 ProcNum;
    IPL_ISR_EVENT_TAB EventTab[IPL_ID_MAX_NUM];     ///<for IPL_lib inner modules
    IPL_ISR_EVENT_TAB CB_FP[IPL_ID_MAX_NUM];        ///<for modules those are not in IPL_lib
} IPL_ISR_OBJ_INFO;

extern IPL_ISR_EVENT_FP EventTab[IPL_ID_MAX_NUM][IPL_ISR_MAX_CNT];      ///<for IPL_lib inner modules
extern IPL_ISR_EVENT_FP EventCBTab[IPL_ID_MAX_NUM][IPL_ISR_MAX_CNT];    ///<for modules those are not in IPL_lib

void IPL_ISRInit(IPL_ISR_OBJ_INFO *Obj);
/**
    for IPL_lib inner modules
*/
//void IPL_ISRProc(UINT32 Id, IPL_ISR IPLModule, UINT32 IntStatus);
#define IPL_ISRProc(Id,IPLModule,IntStatus,idx) \
{                                               \
    if (EventTab[Id][IPLModule] != NULL)        \
    {                                           \
        EventTab[Id][IPLModule](Id,IntStatus,idx); \
    }                                           \
}

/**
    for modules those are not in IPL_lib
*/
//void IPL_ISRCBProc(IPL_ISR ISR, UINT32 IntStatus);
#define IPL_ISRCBProc(Id,ISR,IntStatus,idx)     \
{                                               \
    if (EventCBTab[Id][ISR] != NULL)            \
    {                                           \
        EventCBTab[Id][ISR](Id,IntStatus,idx);     \
    }                                           \
}
void IPL_ISRUnInit(IPL_ISR isr);




#define IPL_ISR_Par_BEG(grp)    \
    typedef struct              \
    {
        #define IPL_ISR_Par_ADD(name)   \
        UINT32 var_##name;

#define IPL_ISR_Par_END(grp)    \
    }T_##grp;                   \
    static volatile T_##grp   VAR_##grp = {0};

#define IPL_ISR_Par_W(grp,name,value)   \
    VAR_##grp.var_##name = value;

#define IPL_ISR_Par_R(grp,name) VAR_##grp.var_##name

#endif //_IPL_ISREVENT_INT_H_
