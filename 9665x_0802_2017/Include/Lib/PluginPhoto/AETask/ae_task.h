/**
    Public header file for AE

    This file is the header file that define the API and data type for AE.

    @file       Ae_task.h
    @ingroup    mILibAEAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _AE_TASK_H_
#define _AE_TASK_H_

#include "Type.h"
#include "ErrorNo.h"

/** \addtogroup mIAE */
//@{

/**
    ae process id
*/
typedef enum
{
    AE_ID_1 = 0,                ///< ae id 1
    //AE_ID_2,                  ///< ae id 2
    AE_ID_MAX_NUM,
    ENUM_DUMMY4WORD(AE_PROC_ID)
} AE_PROC_ID;

#define MAX_AE_PROC_NUM      AE_ID_MAX_NUM

/**
    ae status
*/
typedef enum
{
    AE_TASK_STATUS_CLOSE = 0,
    AE_TASK_STATUS_OPEN = 1,
    AE_TASK_STATUS_START = 2,
    AE_TASK_STATUS_PROC = 3,
    AE_TASK_STATUS_PAUSE = 4,
    ENUM_DUMMY4WORD(AE_TASK_STATUS)
} AE_TASK_STATUS;

/**
    ae event function type
*/
typedef void (*AE_EVENT_FP)(UINT32 CurrentStatus);

/**
    ae event function type
*/
typedef UINT32 (*AE_LOCK_EVENT_FP)(UINT32);
/**
    ae event

    control state:

        AE_Open:    do nothing
        AE_Start:   Start Event -> Process Event -> Process Event -> Process Event....
        AE_Pause:   ....->Process Event -> Process Event -> Process Pause
        AE_Resume:  Start Event -> Process Event -> Process Event -> Process Event....
*/
typedef struct
{
    AE_EVENT_FP Start;      ///< start event
    AE_EVENT_FP Process;    ///< process event
    AE_EVENT_FP Pause;      ///< pause event
} AE_EVENT_OBJ;

/**
    ae input object
*/
typedef struct
{
    UINT32 ProcNum;                         ///< process number
    AE_EVENT_OBJ EventTab[AE_ID_MAX_NUM];   ///< event table
} AE_OBJ_INFO;

/**
     Ae lock/unlock for user suspend ae process

     @param[in] Id          ae process id (AE_PROC_ID)
     @param[in] flag        lock flag: enable, unlock flag: disable
     @param[in] WaitFinish  wait suspend flow finish
*/
extern void AE_Lock(AE_PROC_ID Id, BOOL flag, BOOL WaitFinish);

/**
     check ae is lock or not

     @param[in] Id          ae process id (AE_PROC_ID)
     @return Description of data returned.
         - @b BOOL: true lock, false unlock
*/
extern BOOL AE_IsLock(AE_PROC_ID Id);

/**
     trigger event, for IPL hook

     @param[in] Id          ae process id (AE_PROC_ID)
*/
extern void AE_TrigEvent(AE_PROC_ID Id);

/**
     open ae process

     @param[in] Obj         ae process event
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER AE_Open(AE_OBJ_INFO *Obj);

/**
     close ae process

     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER AE_Close(void);

/**
     start ae process

     @param[in] Id          ae process id (AE_PROC_ID)
     @param[in] WaitFinish  wait start event finish
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER AE_Start(AE_PROC_ID Id, BOOL WaitFinish);

/**
     resume ae process(for IPL control)

     @param[in] Id          ae process id (AE_PROC_ID)
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER AE_Resume(AE_PROC_ID Id);

/**
     pause ae process(for IPL control)

     @param[in] Id          ae process id (AE_PROC_ID)
     @param[in] WaitFinish  wait pause event finish
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER AE_Pause(AE_PROC_ID Id, BOOL WaitFinish);

/**
    install AE control id
*/
extern void AE_InstallID(void) _SECTION(".kercfg_text");


extern void AE_RegSpecifiedEvent(AE_PROC_ID Id, AE_LOCK_EVENT_FP event);

extern void AE_DBGLock(void);
extern void AE_DBGUnlock(void);

//@}
#endif //_AE_TASK_H_
