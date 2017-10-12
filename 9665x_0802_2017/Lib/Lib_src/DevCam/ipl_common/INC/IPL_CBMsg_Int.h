#ifndef _IPL_CBMSG_INT_H_
#define _IPL_CBMSG_INT_H_
/**
    IPL_CBMsg_Int.h


    @file       IPL_CBMsg_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "type.h"
#include "IPL_CBMsg.h"

/**
     initial callback message function

     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern void IPL_CBMsgInit2(IPL_PROC_ID Id, IPL_CBMSG_FP fp);

/**
     un-initial callback message function

     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern void IPL_CBMsgUnInit(void);

/**
     send CB message
     @param[in] Id IPL process Id
     @param[in] IPL_CBMSG ipl callback message
     @param[in] void* data pointer
*/
extern void IPL_CBMsgProc2(IPL_PROC_ID Id, IPL_CBMSG Msg, void *Data);

//for compatible single process

#define IPL_CBMsgInit(fp) IPL_CBMsgInit2(IPL_ID_1, fp)
#define IPL_CBMsgProc(Msg, Data) IPL_CBMsgProc2(IPL_ID_1, Msg, Data)
#endif //_IPL_CBMSG_INT_H_
