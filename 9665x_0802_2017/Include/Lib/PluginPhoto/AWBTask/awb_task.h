/**
    Public header file for AWB

    This file is the header file that define the API and data type for AWB.

    @file       Awb_task.h
    @ingroup    mILibAWBAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _AWB_TASK_H_
#define _AWB_TASK_H_

#include "Type.h"
#include "ErrorNo.h"

/** \addtogroup mIAWB */
//@{

/**
    awb process id
*/
typedef enum
{
    AWB_ID_1 = 0,                ///< awb id 1
    //AWB_ID_2,                  ///< awb id 2
    AWB_ID_MAX_NUM,
    ENUM_DUMMY4WORD(AWB_PROC_ID)
} AWB_PROC_ID;

#define MAX_AWB_PROC_NUM      AWB_ID_MAX_NUM

/**
    awb status
*/
typedef enum
{
    AWB_TASK_STATUS_CLOSE = 0,
    AWB_TASK_STATUS_OPEN = 1,
    AWB_TASK_STATUS_START = 2,
    AWB_TASK_STATUS_PROC = 3,
    AWB_TASK_STATUS_PAUSE = 4,
    ENUM_DUMMY4WORD(AWB_TASK_STATUS)
} AWB_TASK_STATUS;

/**
    awb event function type
*/
typedef void (*AWB_EVENT_FP)(UINT32 CurrentStatus);

/**
    awb event

    control state:

        AWB_Open:    do nothing
        AWB_Start:   Start Event -> Process Event -> Process Event -> Process Event....
        AWB_Pause:   ....->Process Event -> Process Event -> Process Pause
        AWB_Resume:  Start Event -> Process Event -> Process Event -> Process Event....
*/
typedef struct
{
    AWB_EVENT_FP Start;      ///< start event
    AWB_EVENT_FP Process;    ///< process event
    AWB_EVENT_FP Pause;      ///< pause event
} AWB_EVENT_OBJ;

/**
    awb input object
*/
typedef struct
{
    UINT32 ProcNum;                             ///< process number
    AWB_EVENT_OBJ EventTab[AWB_ID_MAX_NUM];     ///< event table
} AWB_OBJ_INFO;

/**
     Awb lock/unlock for user suspend ae process

     @param[in] Id          awb process id (AWB_PROC_ID)
     @param[in] flag        lock flag: enable, unlock flag: disable
     @param[in] WaitFinish  wait suspend flow finish
*/
extern void AWB_Lock(AWB_PROC_ID Id, BOOL flag, BOOL WaitFinish);

/**
     check awb is lock or not

     @param[in] Id          awb process id (AWB_PROC_ID)
     @return Description of data returned.
         - @b BOOL: true lock, false unlock
*/
extern BOOL AWB_IsLock(AWB_PROC_ID Id);

/**
     trigger event, for IPL hook

     @param[in] Id          awb process id (AWB_PROC_ID)
*/
extern void AWB_TrigEvent(AWB_PROC_ID Id);

/**
     open awb process

     @param[in] Obj         awb process event
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER AWB_Open(AWB_OBJ_INFO *Obj);

/**
     close awb process

     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER AWB_Close(void);

/**
     start awb process

     @param[in] Id          awb process id (AWB_PROC_ID)
     @param[in] WaitFinish  wait start event finish
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER AWB_Start(AWB_PROC_ID Id, BOOL WaitFinish);

/**
     resume awb process(for IPL control)

     @param[in] Id          awb process id (AWB_PROC_ID)
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER AWB_Resume(AWB_PROC_ID Id);

/**
     pause awb process(for IPL control)

     @param[in] Id          awb process id (AWB_PROC_ID)
     @param[in] WaitFinish  wait pause event finish
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER AWB_Pause(AWB_PROC_ID Id, BOOL WaitFinish);


/**
    install AWB control id
*/

extern void AWB_InstallID(void) _SECTION(".kercfg_text");
//@}
typedef UINT32 (*AWB_DUMP_FP)(UINT32 Id);
extern void CA_DumpFP(AWB_DUMP_FP fp);
extern void Flag_DumpFP(AWB_DUMP_FP fp);

#endif //_AWB_TASK_H_
