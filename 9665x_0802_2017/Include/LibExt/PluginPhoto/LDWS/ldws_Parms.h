/**
    LDWS Parameters library.

    @file       ldws_parms.h
    @ingroup    mILibLDWS

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/
#ifndef _LDWS_PARMS_H
#define _LDWS_PARMS_H


#include "ldws_alg.h"

/**
     @name LDWS_PARMS return status
*/
//@{
#define LDWSPARMS_STA_OK                  (0)                   ///<  the status is ok, for normal case
#define LDWSPARMS_STA_ERROR               (-1)                  ///<  some error occurred
//@}

/**
     LDWS detection callback message.
*/
//@{
typedef enum _LDWS_CBMSG
{
    LDWS_CBMSG_WARNNING = 0,       ///< user will receive this notification when the lane departure warning occur
    LDWS_CBMSG_MAX_CNT,            ///< total callback message numbers
    ENUM_DUMMY4WORD(LDWS_CBMSG)
} LDWS_CBMSG;
//@}

/**
    LDWS callback funcion proto type
*/
typedef void (*LDWS_CBMSG_FP)(LDWS_CBMSG uiMsgID, void *Data);

/**
     Calculate LDWS detection required buffer size.

     @return the required buffer size.
*/
extern UINT32 LdwsParms_CalcBuffSize(void);

/**
     Init LDWS Detect engine.

     This function is used to initialize smile detection buffer and set the process rate.
*/
extern INT32 LdwsParms_Init(MEM_RANGE *buf, UINT32 ProcessRate);

/**
     Uninit LDWS engine.

     This function is used to release LDWS Detect engine.
*/
extern INT32 LdwsParms_UnInit(void);

/**
     Process the LDWS detection.

     This function will find simles in the result of LDWS detection.
     and then detect if any LDW
*/
extern INT32 LdwsParms_Process(void);

/**
     Register callback function.
*/
extern void LdwsParms_RegisterCB(LDWS_CBMSG_FP CB);

/**
    Set location of vanishing point
*/
extern UINT32 LdwsParms_setVanishingPtPos(UINT32 VanishPty);

/**
    Get location of vanishing point
*/
extern UINT32 LdwsParms_getVanishingPtPos(void);

/**
    Get LDWS status
*/
extern LDWS_RESULT_INFO LdwsParms_getLdwsStatus(void);

#endif

