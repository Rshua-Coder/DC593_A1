/**
    DevMan, APIs declare.

    @file       DevManApi.h
    @ingroup    mDEVMAN
    @note       --

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _DEVMANAPI_H
#define _DEVMANAPI_H

#define DEVMAN_API_VERSION 0x11012008U
#define DEVMAN_SIZE        120 //30 words  ///< The byte size information for control


/**
   Error Code.

   Any error code occured will display on uart or return.
*/
typedef enum _DEVMAN_ER{
    DEVMAN_ER_OK                =   0, ///< no error
    DEVMAN_ER_NOT_INIT          =  -1, ///< DevMan_Init() is not called or not success
    DEVMAN_ER_SYS               =  -2, ///< unknown system error
    DEVMAN_ER_LOCK              =  -3, ///< failed to lock API
    DEVMAN_ER_UNLOCK            =  -4, ///< failed to unlock API
    DEVMAN_ER_PARAM             =  -5, ///< wrong Parameters
    DEVMAN_ER_API_VERSION       =  -6, ///< wrong API version
    DEVMAN_ER_STA_TASK          =  -7, ///< failed to call sta_tsk
    DEVMAN_ER_WAIT_IDLE         =  -8, ///< failed to call wai_flg for idle
    DEVMAN_ER_SET_IDLE          =  -9, ///< failed to call set_flg to idle
    DEVMAN_ER_NOT_IN_IDLE       = -10, ///< if Prop.bEnterOnlyIdle is true, but system is not idle
    DEVMAN_ER_STATE             = -11, ///< service state machine is not correctly.
    DEVMAN_ER_INVALID_CALL      = -12, ///< invald to call this API
    DEVMAN_ER_OUT_DATA_VOLATILE = -13, ///< the parameter DEVMAN_CMD.out maybe volatile. (suggest to bExitCmdFinish=TRUE)
    DEVMAN_ER_CMD_MAXDATA       = -14, ///< internal error. all cmd data structure have to declare in DEVMAN_CMD_MAXDATA
    DEVMAN_ER_INVALID_CMD_IDX   = -15, ///< invalid command index
    DEVMAN_ER_CMD_MAP_NULL      = -16, ///< internal error. cmd function map to a null pointer
    DEVMAN_ER_CMD_NOT_MATCH     = -17, ///< internal error. g_DevManCallTbl is not match to DEVMAN_CMD_IDX
    DEVMAN_ER_CMD_IN_DATA       = -18, ///< wrong DEVMAN_CMD.In Data
    DEVMAN_ER_CMD_OUT_DATA      = -19, ///< wrong DEVMAN_CMD.Out Data
    ENUM_DUMMY4WORD(DEVMAN_ER)
}DEVMAN_ER;

/**
    Warning Code.

    The warning codes is only displayed via uart.
*/
typedef enum _DEVMAN_WR{
    DEVMAN_WR_OK                =   0, ///< no warning
    DEVMAN_WR_INIT_TWICE        =  -1, ///< DevMan_Init() is called twice
    DEVMAN_WR_ALREADY_CLOSED    =  -2, ///< service is already closed.
    DEVMAN_WR_SUSPEND_TWICE     =  -3, ///< DevMan_Suspend maybe is called twice
    DEVMAN_WR_NOT_IN_SUSPEND    =  -4, ///< call DevMan_Resume, but service isn't in suspend state before.
    DEVMAN_WR_CMD_SKIP          =  -5, ///< existing a command is skipped
    ENUM_DUMMY4WORD(DEVMAN_WR)
}DEVMAN_WR;

/**
    State Machine.

    Indicate the task state.
*/
typedef enum _DEVMAN_STATE{
    DEVMAN_STATE_UNKNOWN    = 0,    ///< unknown. only existing service is not opened.
    DEVMAN_STATE_OPEN_BEGIN,        ///< start to open a task
    DEVMAN_STATE_OPEN_END,          ///< finished to do open flow
    DEVMAN_STATE_CLOSE_BEGIN,       ///< start to close
    DEVMAN_STATE_CLOSE_END,         ///< task is terminated.
    DEVMAN_STATE_SUSPEND_BEGIN,     ///< start to enter suspend state
    DEVMAN_STATE_SUSPEND_END,       ///< task is in suspend state
    DEVMAN_STATE_RESUME_BEGIN,      ///< start to resume service
    DEVMAN_STATE_RESUME_END,        ///< finished to resume, task start to recevice command
    DEVMAN_STATE_CMD_BEGIN,         ///< start to do a command
    DEVMAN_STATE_CMD_END,           ///< command finished
    DEVMAN_STATE_CMD_CB_BEGIN,      ///< start to call a callback when command finished
    DEVMAN_STATE_CMD_CB_END,        ///< exit to callback when command finished
    DEVMAN_STATE_STATUS_CB_BEGIN,   ///< start to call a callback when status change
    DEVMAN_STATE_STATUS_CB_END,     ///< exit to callback when status change
    DEVMAN_STATE_ER_CB_BEGIN,       ///< start to call a callback when error occured
    DEVMAN_STATE_ER_CB_END,         ///< exit to callback when error occured
    DEVMAN_STATE_MAX_NUM,           ///< total state numbers
    ENUM_DUMMY4WORD(DEVMAN_STATE)
}DEVMAN_STATE;

#if 0
typedef void (*DEVMAN_FP_STATE_CB)(DEVMAN_STATE State); ///< State callback. If there's any state changed, the service will callback out.
#else
/**
    State information.

    If there's any state changed, the service will callback out.

    @param[out] cmd State occured.
    @param[out] param1 (unused).
    @param[out] param2 (unused).
*/
typedef void (*DEVMAN_FP_STATE_CB)(UINT32 cmd, UINT32 param1, UINT32 param2);
#endif
typedef void (*DEVMAN_FP_ERROR_CB)(DEVMAN_ER er); ///< Error callback. If there's any error occured, the service will callback out.

#if 0
/**
    Finish information.

    When DEVMAN_PROP.fpFinishCb assign a callback, the data will send to this
    callback after command finish.
*/
typedef struct _DEVMAN_FINISH{
    UINT32          Idx;        ///< Finished command index
    DEVMAN_ER       er;         ///< command finish result.
    VOID*           pUserData;  ///< If DEVMAN_PROP.pUserData is assigned a userdata pointer. service will take it to DEVMAN_PROP.fpFinishCb
}DEVMAN_FINISH,*PDEVMAN_FINISH;

typedef void (*DEVMAN_CB_FINISH)(const DEVMAN_FINISH* pInfo); ///< Command finish callback. When any command is finished, the service will callback out.
#else
/**
    Finish information.

    When DEVMAN_PROP.fpFinishCb assign a callback, the data will send to this
    callback after command finish.

    @param[out] cmd Finished command index.
    @param[out] param1 Finish result.
    @param[out] param2 If DEVMAN_PROP.pUserData is assigned a userdata pointer. service will take it to DEVMAN_PROP.fpFinishCb
*/
typedef void (*DEVMAN_CB_FINISH)(UINT32 cmd, UINT32 param1, UINT32 param2);
#endif

#if 0
/**
    Command Property.

    Indicate how to run the current command.
*/
typedef struct _DEVMAN_PROP{
    BOOL                bEnterOnlyIdle; ///< do job only system idle. if system is busy, the DEVMAN_ER_NOT_IN_IDLE returned.
    BOOL                bExitCmdFinish; ///< exit API when job finished.
    DEVMAN_CB_FINISH    fpFinishCb;     ///< (Option) system will call the callback when job is finished
    VOID*               pUserData;      ///< (Option) when CbFinish is called, assigned userdata can get from CbFinish parameter.
}DEVMAN_PROP,*PDEVMAN_PROP;
#endif

/**
    Command Input/Output Data.

    Including data pointer and data size depend on DEVMAN_CMD_IDX.
*/
typedef struct _DEVMAN_DATA{
    VOID*   pData;      ///< input/output data pointer
    UINT32  uiNumByte;  ///< input/output data size
}DEVMAN_DATA,*PDEVMAN_DATA;

/**
    Command Description.

    The necessary information for running a command via DevMan_Cmd().
*/
typedef struct _DEVMAN_CMD{
#if 0
    UINT32          Idx;    ///< index
    DEVMAN_PROP     Prop;   ///< property
    DEVMAN_DATA     In;     ///< input data description (depend on Idx)
    DEVMAN_DATA     Out;    ///< output data description (depend on Idx)
#else
    UINT32          Idx;    ///< index
    DEVMAN_DATA     In;     ///< input data description (depend on Idx)
    DEVMAN_DATA     Out;    ///< output data description (depend on Idx)
    BOOL            bWaitFinish; ///< exit API when job finished.
#endif
}DEVMAN_CMD,*PDEVMAN_CMD;

/**
    Cmd function pointer.

    Type of control cmd function pointer.
*/
typedef DEVMAN_ER (*DEVMAN_FP_CMD)(const DEVMAN_CMD* pCmd); ///< for Command Function Table


/**
    Command Function Table Element
*/
typedef struct _DEVMAN_CMD_DESC{
    UINT32          Idx;            ///< Command Index
    DEVMAN_FP_CMD   fpCmd;          ///< mapped function pointer
    UINT32          uiNumByteIn;    ///< require data in size
    UINT32          uiNumByteOut;   ///< require data out size
}DEVMAN_CMD_DESC;


/**
    Initial Data.

    For system inital via DevMan_Init().
*/
typedef struct _DEVMAN_INIT{
    //Interface
    UINT32              uiApiVer;   ///< just assign to DEVMAN_API_VERSION
    //Task
    ID                  TaskID;     ///< given a task id form project layer creation
    ID                  SemID;      ///< given a semaphore id form project layer creation
    ID                  FlagID;     ///< given a flag id form project layer creation
#if 1
    //Command Table
    UINT32              nCmdTbl;    ///< number of command function
    const DEVMAN_CMD_DESC* fpCmdTbl;///< command function table
#endif
    //Command CB
    DEVMAN_FP_STATE_CB  fpStateCb;  ///< (Option) system will call the callback when state change
    DEVMAN_FP_ERROR_CB  fpErrorCb;  ///< (Option) system will call the callback if there is any error occured (for debug)
#if 1
    DEVMAN_CB_FINISH    fpFinishCb; ///< (Option) system will call the callback when job is finished
    //User Data
    VOID*               pUserData;  ///< (Option) General working context.
#endif
}DEVMAN_INIT,*PDEVMAN_INIT;


/**
    Control handle.

    Type of control handle.
*/
typedef void* DEVMAN_HANDLE;

/**
    DevMan main task.

    Task body. Just set it into user_systasktabl in SysCfg.c. The stack size
    suggest to 4096KB. If there are many callbacks have plug-in, the stack
    size maybe need to more.
*/
extern VOID DevManTsk(DEVMAN_HANDLE hCtrl);

/**
    Initialize DevMan service.

    For using the DevMan service, the task id, flag id, semphore id have to
    prepare by project layer. Assign these ids to the DEVMAN_INIT strcuture
    when system starup (just set once).

    @param[in] pInit necessary inital data.
    @param[in] pCtrlBuf necessary working buffer for control (pointer to buffer size = SIZE_OF_DEVMAN).
    @param[out] phCtrl return new handle.
    @return
        - @b DEVMAN_ER_OK: inital success.
        - @b DEVMAN_ER_NOT_INIT: pInit/pCtrlBuf/phCtrl pointer not ready.
        - @b DEVMAN_WR_INIT_TWICE: aleady initialed.
        - @b DEVMAN_ER_API_VERSION: DevManApi.h and AppDevMan.a not match.
*/
extern DEVMAN_ER DevMan_Init(const DEVMAN_INIT* pInit, void* pCtrlBuf, DEVMAN_HANDLE* phCtrl);

/**
    Start to open the DevMan service.

    After DevMan service is initialized, call DevMan_Open to start the
    service.

    @return
        - @b DEVMAN_ER_OK: open success.
        - @b DEVMAN_ER_STATE: may DevMan_Init does not be called yet.
        - @b DEVMAN_ER_STA_TASK: failed to start task(sta_tsk failed).
*/
extern DEVMAN_ER DevMan_Open(DEVMAN_HANDLE hCtrl);

/**
    Stop to open the DevMan service.

    When DevMan service is running, call DevMan_Close to terminate the task
    service.

    @note: Don't call DevMan_Close in any DevMan service callback. In
    callback, the DevMan_AsyncClose is allowed to call to stop recevice receive
    data but task is not terminated. The DevMan_Close need to be called outside.

    @return
        - @b DEVMAN_ER_OK: close success.
        - @b DEVMAN_ER_INVALID_CALL: call DevMan_Close in callback.
        - @b DEVMAN_ER_STA_TASK: failed to start task(sta_tsk failed).
*/
extern DEVMAN_ER DevMan_Close(DEVMAN_HANDLE hCtrl);

/**
    Suspend the service task.

    When DevMan service is running, call DevMan_Suspend to stop recevice
    command. Any command will be skiped by service task. The function exit after
    task finish his suspend.

    @return
        - @b DEVMAN_ER_OK: suspend success.
        - @b DEVMAN_ER_STATE: may DevMan_Open does not be opened yet.
*/
extern DEVMAN_ER DevMan_Suspend(DEVMAN_HANDLE hCtrl);

/**
    Resume the service task.

    When DevMan service is suspended, call DevMan_Resume to start to
    recevice command. The function exit after task finish his resume.

    @return
        - @b DEVMAN_ER_OK: suspend success.
        - @b DEVMAN_ER_STATE: may DevMan_Open does not be opened yet.
*/
extern DEVMAN_ER DevMan_Resume(DEVMAN_HANDLE hCtrl);

/**
    Set a command action to DevMan service.

    Configure DEVMAN_CMD and send it to the service to do some action via
    DevMan_Cmd.

    @param[in] pCmd command description.
    @return
        - @b DEVMAN_ER_OK: send command success.
        - @b DEVMAN_ER_CMD_NOT_MATCH: internal error. Please contact to module
             maintenance staff.
        - @b DEVMAN_ER_CMD_IN_DATA: the DEVMAN_CMD.In maybe assing wrong object
             / structure
        - @b DEVMAN_ER_CMD_IN_DATA: the DEVMAN_CMD.Out maybe assing wrong object
             / structure
        - @b DEVMAN_ER_STATE: may DevMan_Open does not be opened yet.
        - @b DEVMAN_ER_NOT_IN_IDLE: If DEVMAN_CMD.DEVMAN_PROP.bEnterOnlyIdle set
             to TRUE and service is not in idle. Notice you this command is
             skipped.
        - @b DEVMAN_ER_OUT_DATA_VOLATILE: If the command include the output data
             and set DEVMAN_CMD.DEVMAN_PROP.bExitCmdFinish to FALSE, the finish
             callback have to plug-in a callback function to DEVMAN_CMD
             .DEVMAN_PROP.fpFinishCb and DEVMAN_CMD.Out have to point to a
             global data structure.
        - @b DEVMAN_ER_CMD_MAXDATA: internal error. Please contact to module
             maintenance staff.
*/
extern DEVMAN_ER DevMan_Cmd(DEVMAN_HANDLE hCtrl, const DEVMAN_CMD* pCmd);

/**
    Suspend task service, but exit this function without finish suspend.

    This function is for advance. If you don't want to wait DevMan_Suspend()
    finish too long and you want to pipe some flow, you can do the flow that
    DevMan_AsyncSuspend() -> Do Something -> DevMan_Suspend();

    @note: DevMan_Suspend() is also called necessary to confirm the suspend is
           finished.

    @return
        - @b DEVMAN_ER_OK: suspend success.
        - @b DEVMAN_ER_STATE: may DevMan_Open does not be opened yet.
*/
extern DEVMAN_ER DevMan_AsyncSuspend(DEVMAN_HANDLE hCtrl);

/**
    Stop task service, but exit this function without finish close.

    This function is for advance. If you don't want to wait DevMan_Close()
    finish too long and you want to pipe some flow, you can do the flow that
    DevMan_AsyncClose() -> Do Something -> DevMan_Close();

    @note: DevMan_AsyncClose() is also called necessary to confirm the suspend is
           finished.

    @return
        - @b DEVMAN_ER_OK: suspend success.
        - @b DEVMAN_ER_STATE: may DevMan_Open does not be opened yet.
*/
extern DEVMAN_ER DevMan_AsyncClose(DEVMAN_HANDLE hCtrl);

/**
    Dump DevMan application library information.

    The information includes version, build date, last error code, last warning
    code, last command, last state and check internal data.

    @return
        - @b DEVMAN_ER_OK: suspend success.
*/
extern DEVMAN_ER DevMan_DumpInfo(DEVMAN_HANDLE hCtrl);

/**
    Get DevMan application version information

    Get version. Normally, the format is 9 characters (include null terminated
    character). Example: 1.00.001

    @return Version string with null terminated character.
*/
extern CHAR* DevMan_GetVerInfo(void);

/**
    Get DevMan application build date and time

    Get build date and time. Return a string (include null terminated
    character). Example: Jan 01 2011, 23:59:59

    @return Date time string with null terminated character.
*/
extern CHAR* DevMan_GetBuildDate(void);
#endif

