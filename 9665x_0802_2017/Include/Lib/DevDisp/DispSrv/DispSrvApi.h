/**
    Display Service

    Video 1 display service for application.

    @file       DispSrvApi.h
    @ingroup    mDISPSRV

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DISPSRVAPI_H
#define _DISPSRVAPI_H
#include "GxImage.h"

/**
    @addtogroup mDISPSRV
*/
//@{

#define DISPSRV_API_VERSION 0x12062217U ///< API version
#define DISPSRV_LOCK_NEWEST 0xFFFFFFFFU ///< for DISPSRV_FP_LOCK_CB to lock newest frame

/**
   Error Code.

   Any error code occured will display on uart or return.
*/
typedef enum _DISPSRV_ER{
    DISPSRV_ER_OK                =   0, ///< no error
    DISPSRV_ER_NOT_INIT          =  -1, ///< DispSrv_Init() is not called or not success
    DISPSRV_ER_SYS               =  -2, ///< unknown system error
    DISPSRV_ER_LOCK              =  -3, ///< failed to lock API
    DISPSRV_ER_UNLOCK            =  -4, ///< failed to unlock API
    DISPSRV_ER_PARAM             =  -5, ///< wrong Parameters
    DISPSRV_ER_API_VERSION       =  -6, ///< wrong API version
    DISPSRV_ER_STA_TASK          =  -7, ///< failed to call sta_tsk
    DISPSRV_ER_WAIT_IDLE         =  -8, ///< failed to call wai_flg for idle
    DISPSRV_ER_SET_IDLE          =  -9, ///< failed to call set_flg to idle
    DISPSRV_ER_NOT_IN_IDLE       = -10, ///< if Prop.bEnterOnlyIdle is true, but system is not idle
    DISPSRV_ER_STATE             = -11, ///< service state machine is not correctly.
    DISPSRV_ER_INVALID_CALL      = -12, ///< invald to call this API
    DISPSRV_ER_OUT_DATA_VOLATILE = -13, ///< the parameter DISPSRV_CMD.out maybe volatile. (suggest to bExitCmdFinish=TRUE)
    DISPSRV_ER_CMD_MAXDATA       = -14, ///< internal error. all cmd data structure have to declare in DISPSRV_CMD_MAXDATA
    DISPSRV_ER_INVALID_CMD_IDX   = -15, ///< invalid command index
    DISPSRV_ER_CMD_MAP_NULL      = -16, ///< internal error. cmd function map to a null pointer
    DISPSRV_ER_CMD_NOT_MATCH     = -17, ///< internal error. g_DispSrvCallTbl is not match to DISPSRV_CMD_IDX
    DISPSRV_ER_CMD_IN_DATA       = -18, ///< wrong DISPSRV_CMD.In Data
    DISPSRV_ER_CMD_OUT_DATA      = -19, ///< wrong DISPSRV_CMD.Out Data
    DISPSRV_ER_ZERO_NUM_BUF      = -20, ///< NumDisp or NumSrc cannot be zero
    DISPSRV_ER_CANNOT_ON_PLAY    = -21, ///< this command have to use on pause or stop state
    DISPSRV_ER_INTERNAL_EXCEPTION= -22, ///< internal exception
    DISPSRV_ER_SRC_NOT_INIT      = -23, ///< DISPSRV_CMD_IDX_SET_SRC have to call first
    DISPSRV_ER_DISP_NOT_INIT     = -24, ///< DISPSRV_CMD_IDX_SET_DISP have to call first
    DISPSRV_ER_INVALID_FIRST_FRAME = -25, ///< direct mode not support to first frame
    DISPSRV_ER_FPDRACB_NULL      = -26, ///< fpDrawCb cannot NULL
    DISPSRV_ER_FORMAT_MUST_UV420 = -27, ///< color format have to be UV Pack 420
    DISPSRV_ER_DISP_2ND_BUF_FEW  = -28, ///< secondary disp buffer have to large 1 (you may set ScaleBaseIdx = DISPSRV_DISP_IDX_SECONDARY
    DISPSRV_ER_ROT_DIRECT_INVALID= -29, ///< in rotation mode, not support to direct mode.
    DISPSRV_ER_CANNOT_HANDLE_BOTH= -30, ///< the mode set user handle both, but the source is not dual
    DISPSRV_ER_ANCHOR_STATE      = -31, ///< DISPSRV_CMD_IDX_ANCHOR_DISP_TOP only accept in between stop and (setmode,setdisp)
    DISPSRV_ER_FP_NULL           = -32, ///< the input function pointer cannot be NULL
    DISPSRV_ER_ASPECT_RATIO      = -33, ///< invalid aspect ratio
    DISPSRV_ER_SRC_NOT_DUAL      = -34, ///< in direct mode, if disp config to dual, the source also have to be dual
    DISPSRV_ER_INDIRECT_ONLY     = -35, ///< this command is only to use in direct mode
    ENUM_DUMMY4WORD(DISPSRV_ER)
}DISPSRV_ER;

/**
    Warning Code.

    The warning codes is only displayed via uart.
*/
typedef enum _DISPSRV_WR{
    DISPSRV_WR_OK               =   0, ///< no warning
    DISPSRV_WR_INIT_TWICE       =  -1, ///< DispSrv_Init() is called twice
    DISPSRV_WR_ALREADY_CLOSED   =  -2, ///< service is already closed.
    DISPSRV_WR_SUSPEND_TWICE    =  -3, ///< DispSrv_Suspend maybe is called twice
    DISPSRV_WR_NOT_IN_SUSPEND   =  -4, ///< call DispSrv_Resume, but service isn't in suspend state before.
    DISPSRV_WR_CMD_SKIP         =  -5, ///< existing a command is skipped
    DISPSRV_WR_PAUSE_STATE      =  -6, ///< run state already on pause state
    DISPSRV_WR_STOP_STATE       =  -7, ///< run state already on stop state
    DISPSRV_WR_SKIP_TRIGGER     =  -8, ///< the current trigger is skipped due to on not run state
    DISPSRV_WR_NULL_DRAW_CB     =  -9, ///< in indirect mode, the OnDraw callback should be not NULL
    ENUM_DUMMY4WORD(DISPSRV_WR)
}DISPSRV_WR;


/**
    State Machine.

    Indicate the task state.
*/
typedef enum _DISPSRV_STATE{
    DISPSRV_STATE_UNKNOWN    = 0,    ///< unknown. only existing service is not opened.
    DISPSRV_STATE_OPEN_BEGIN,        ///< start to open a task
    DISPSRV_STATE_OPEN_END,          ///< finished to do open flow
    DISPSRV_STATE_CLOSE_BEGIN,       ///< start to close
    DISPSRV_STATE_CLOSE_END,         ///< task is terminated.
    DISPSRV_STATE_SUSPEND_BEGIN,     ///< start to enter suspend state
    DISPSRV_STATE_SUSPEND_END,       ///< task is in suspend state
    DISPSRV_STATE_RESUME_BEGIN,      ///< start to resume service
    DISPSRV_STATE_RESUME_END,        ///< finished to resume, task start to recevice command
    DISPSRV_STATE_CMD_BEGIN,         ///< start to do a command
    DISPSRV_STATE_CMD_END,           ///< command finished
    DISPSRV_STATE_CMD_CB_BEGIN,      ///< start to call a callback when command finished
    DISPSRV_STATE_CMD_CB_END,        ///< exit to callback when command finished
    DISPSRV_STATE_STATUS_CB_BEGIN,   ///< start to call a callback when status change
    DISPSRV_STATE_STATUS_CB_END,     ///< exit to callback when status change
    DISPSRV_STATE_ER_CB_BEGIN,       ///< start to call a callback when error occured
    DISPSRV_STATE_ER_CB_END,         ///< exit to callback when error occured
    DISPSRV_STATE_MAX_NUM,           ///< total state numbers
    ENUM_DUMMY4WORD(DISPSRV_STATE)
}DISPSRV_STATE;

/**
    Command Index.

    The command of DispSrv service support. Set this index to the DISPSRV_CMD
    and call the DispSrv_Cmd to do command when service is opened.
*/
typedef enum _DISPSRV_CMD_IDX{
    DISPSRV_CMD_IDX_UNKNOWN  = 0,       ///< unknown command. maybe forget to set DISPSRV_CMD.Idx
    DISPSRV_CMD_IDX_PLAY,               ///< start to receive trigger signal (In:NULL, Out:NULL)
    DISPSRV_CMD_IDX_PAUSE,              ///< pause the receive (In:NULL, Out:NULL)
    DISPSRV_CMD_IDX_STOP,               ///< stop and reset the DispSrv condition (In:NULL, Out:NULL)
    DISPSRV_CMD_IDX_SET_DISP,           ///< set display output (In:DISPSRV_DISP_CFG, Out:NULL), !!STOP State requirement!!
    DISPSRV_CMD_IDX_GET_DISP,           ///< get display output setting (In:NULL, Out:DISPSRV_DISP_CFG)
    DISPSRV_CMD_IDX_SET_SRC,            ///< set source input image (In:DISPSRV_SRC_CFG, Out:NULL), !!STOP State requirement!!
    DISPSRV_CMD_IDX_GET_SRC,            ///< get source input image setting(In:NULL, Out:DISPSRV_SRC_CFG)
    DISPSRV_CMD_IDX_SET_MODE,           ///< Set display mode (In:DISPSRV_MODE, Out:NULL), !!STOP State requirement!!, and Run Command with bExitCmdFinish=TRUE
    DISPSRV_CMD_IDX_GET_MODE,           ///< Get current mode setting (In:no, Out:DISPSRV_MODE)
    DISPSRV_CMD_IDX_SET_DRAW_CB,        ///< set draw callback (In:DISPSRV_FP_DRAW_CB, Out:NULL)
    DISPSRV_CMD_IDX_GET_DRAW_CB,        ///< get draw callback (In:NULL, Out:DISPSRV_FP_DRAW_CB)
    DISPSRV_CMD_IDX_SET_USER_STATE,     ///< Set the user state (In:UINT32, Out:NULL)
    DISPSRV_CMD_IDX_GET_USER_STATE,     ///< Get the user state (In:NULL, Out:UINT32)
    DISPSRV_CMD_IDX_GET_CURR_DISPIDX,   ///< get current displayed buffer index (In:NULL, Out:UINT32)
    DISPSRV_CMD_IDX_GET_RUN_STATE,      ///< Get current run state (In:NULL, Out:DISPSRV_RUN_STATE)
    DISPSRV_CMD_IDX_ANCHOR_DISP_TOP,    ///< indirect mode used only. after dispsrv stopped, if dispidx=1, dispsrv will change it to 0 with copy frame (In:NULL, Out:NULL)
    DISPSRV_CMD_IDX_DIRECT_FRM_TO_TEMP, ///< only does use it after stop with indirect mode. it copy last src frame to frame buffer(must be available), and then show it for Src App prepare next mode frame. (In:NULL, Out:NULL)
    DISPSRV_CMD_IDX_DISP_PRIMARY_AS_IDE_N, ///< primary disp maps to ide 0 or 1.(In:UINT32, Out:NULL)
    DISPSRV_CMD_IDX_MAX_NUM,            ///< Number of commands
    ENUM_DUMMY4WORD(DISPSRV_CMD_IDX)
}DISPSRV_CMD_IDX;


/**
     Mode index
*/
typedef enum _DISPSRV_MODE_IDX{
    DISPSRV_MODE_IDX_UNKNOWN = 0,     ///< Unknown mode
    DISPSRV_MODE_IDX_SINGLE_DIRECT,   ///< Firmware handshake
    DISPSRV_MODE_IDX_SINGLE_INDIRECT, ///< User can plug-in a customer draw
    DISPSRV_MODE_IDX_MAX_NUM,         ///< Number of mode
    ENUM_DUMMY4WORD(DISPSRV_MODE_IDX)
}DISPSRV_MODE_IDX;


/**
     Rotation type

     for DISPSRV_DISP_DESC
*/
typedef enum _DISPSRV_ROTATE{
    DISPSRV_ROTATE_0 = 0,       ///< No rotation
    DISPSRV_ROTATE_90,          ///< rotate 90 degree
    DISPSRV_ROTATE_90_VERT,     ///< rotate 90 degree + vertical flip
    DISPSRV_ROTATE_180,         ///< rotate 180 degree
    DISPSRV_ROTATE_270,         ///< rotate 270 degree
    DISPSRV_ROTATE_270_VERT,    ///< rotate 270 degree  + vertical flip
    DISPSRV_ROTATE_HORZ,        ///< horzional flip
    DISPSRV_ROTATE_VERT,        ///< vertical flip
    DISPSRV_ROTATE_NO_HANDLE,   ///< user handle ide(n) output dir. dispsrv will not set output dir to ide(n) in cases of 0,180,horz,vert.
    DISPSRV_ROTATE_MAX_NUM,     ///< Number of rotation item
    ENUM_DUMMY4WORD(DISPSRV_ROTATE)
}DISPSRV_ROTATE;

/**
     Running state

     Use the command of DISPSRV_CMD_IDX_GET_RUN_STATE to get DispSrv current state
*/
typedef enum _DISPSRV_RUN_STATE{
    DISPSRV_RUN_STATE_UNKNOWN = 0,  ///< state on unknown
    DISPSRV_RUN_STATE_PLAY,         ///< state on play
    DISPSRV_RUN_STATE_PAUSE,        ///< state on pause
    DISPSRV_RUN_STATE_STOP,         ///< state on stop
    ENUM_DUMMY4WORD(DISPSRV_RUN_STATE)
}DISPSRV_RUN_STATE;

/**
     Display index

     There are two display engines have be supported.
*/
typedef enum _DISPSRV_DISP_IDX{
    DISPSRV_DISP_IDX_PRIMARY= 0,///< primary display
    DISPSRV_DISP_IDX_SECONDARY, ///< secondary display
    DISPSRV_DISP_IDX_MAX_NUM,
    ENUM_DUMMY4WORD(DISPSRV_DISP_IDX)
}DISPSRV_DISP_IDX;

/**
     Source buffer index

     There are two source buffers can be supported.
*/
typedef enum _DISPSRV_SRC_IDX{
    DISPSRV_SRC_IDX_PRIMARY= 0, ///< primary source
    DISPSRV_SRC_IDX_SECONDARY,  ///< secondary display
    DISPSRV_SRC_IDX_MAX_NUM,
    ENUM_DUMMY4WORD(DISPSRV_SRC_IDX)
}DISPSRV_SRC_IDX;

/**
     User handle method

     In OnDraw callback., the methods to generated display output.
*/
typedef enum _DISPSRV_USER_HANDLE{
    DISPSRV_USER_HANDLE_PRIMARY,    ///<In condition of daul mode, set it to DISPSRV_DISP_IDX_PRIMARY means scale primary to secondary.
    DISPSRV_USER_HANDLE_SECONDARY,  ///<If set it to DISPSRV_DISP_IDX_SECONDARY, the uiNumDisp of secondary must be 2
    DISPSRV_USER_HANDLE_BOTH,       ///<In condition of dual mode, user have to draw both pImg[0] and pImg[1]
    ENUM_DUMMY4WORD(DISPSRV_USER_HANDLE)
}DISPSRV_USER_HANDLE;

/**
    Display Event

    Indicate the display state.
*/
typedef enum _DISPSRV_DISP_EVENT{
    DISPSRV_DISP_EVENT_UNKNOWN,    ///< Invalid Event
    DISPSRV_DISP_EVENT_DISP_STABLE,///< Notify the first frame is ready after display configure / set mode finish
    ENUM_DUMMY4WORD(DISPSRV_DISP_EVENT)
}DISPSRV_DISP_EVENT;

typedef void (*DISPSRV_FP_STATE_CB)(DISPSRV_STATE State); ///< State callback. If there's any state changed, the service will callback out.
typedef void (*DISPSRV_FP_ERROR_CB)(DISPSRV_ER er); ///< Error callback. If there's any error occured, the service will callback out.
typedef void (*DISPSRV_FP_EVENT_CB)(DISPSRV_DISP_EVENT event); ///< Display callback. refer to DISPSRV_DISP_EVENT

/**
    Initial Data.

    For system inital via DispSrv_Init().
*/
typedef struct _DISPSRV_INIT{
    UINT32              uiApiVer;   ///< just assign to DISPSRV_API_VERSION
    ID                  TaskID;     ///< given a task id form project layer creation (can set 0 to use system default)
    ID                  SemID;      ///< given a semaphore id form project layer creation (can set 0 to use system default)
    ID                  FlagID;     ///< given a flag id form project layer creation (can set 0 to use system default)
    DISPSRV_FP_STATE_CB fpStateCb;  ///< (Optional) system will call the callback when state change
    DISPSRV_FP_ERROR_CB fpErrorCb;  ///< (Optional) if there is error occured, callback will be performed for debug
}DISPSRV_INIT,*PDISPSRV_INIT;

/**
    Finish information.

    When DISPSRV_PROP.fpFinishCb assign a callback, the data will send to this
    callback after command finish.
*/
typedef struct _DISPSRV_FINISH{
    DISPSRV_CMD_IDX  Idx;        ///< finished command index
    DISPSRV_ER       er;         ///< command finish result.
    VOID*            pUserData;  ///< if DISPSRV_PROP.pUserData is assigned a userdata pointer. service will take it to DISPSRV_PROP.fpFinishCb
}DISPSRV_FINISH,*PDISPSRV_FINISH;

typedef void (*DISPSRV_CB_FINISH)(const DISPSRV_FINISH* pInfo); ///< Command finish callback. When any command is finished, the service will callback out.
/**
    Command Property.

    Indicate how to run the current command.
*/
typedef struct _DISPSRV_PROP{
    BOOL                bEnterOnlyIdle; ///< do job only system idle. if system is busy, the DISPSRV_ER_NOT_IN_IDLE returned.
    BOOL                bExitCmdFinish; ///< exit API when job finished.
    DISPSRV_CB_FINISH   fpFinishCb;     ///< (Optional) system will call the callback when job is finished
    VOID*               pUserData;      ///< (Optional) when CbFinish is called, assigned userdata can get from CbFinish parameter.
}DISPSRV_PROP,*PDISPSRV_PROP;

/**
    Command Input/Output Data.

    Including data pointer and data size depend on DISPSRV_CMD_IDX.
*/
typedef struct _DISPSRV_DATA{
    VOID*   pData;      ///< input/output data pointer
    UINT32  uiNumByte;  ///< input/output data size
}DISPSRV_DATA,*PDISPSRV_DATA;

/**
    Command Description.

    The necessary information for running a command via DispSrv_Cmd().
*/
typedef struct _DISPSRV_CMD{
    DISPSRV_CMD_IDX  Idx;    ///< index
    DISPSRV_PROP     Prop;   ///< property
    DISPSRV_DATA     In;     ///< input data description (depend on Idx)
    DISPSRV_DATA     Out;    ///< output data description (depend on Idx)
}DISPSRV_CMD,*PDISPSRV_CMD;

/**
    Display description

    Input data for
*/
typedef struct _DISPSRV_DISP_DESC
{
    DISPSRV_ROTATE          Deg;        ///< Degree of rotation (if Deg=90 or 270, the system image format musb be !!!"UV Pack 420"!!!
    URECT                   rcWnd;      ///< display window, normally is same as buffer
    USIZE                   AspectRatio;///< Display aspect ratio, (eg. disp buffer is 960x240, but the aspect ratio is 4:3
    USIZE                   DeviceSize; ///< Device size to meet HSync and VSync for calculating best buffer size.(the value normally are 960x480,960x240,640x480,640x240 for LCD)
    UINT32                  uiNumDisp;  ///< Number of display working buffer
    IMG_BUF*                pDisp;      ///< Display buffer (Array) !!Must be Global Array!!
    IMG_BUF*                pRot;       ///< (Optional) Rotation buffer for double buffer mechanism. the same is same with pDisp !!Must be Global Array!!
}DISPSRV_DISP_DESC,*PDISPSRV_DISP_DESC;

/**
    Display Configuration

    Input data for
*/
typedef struct _DISPSRV_DISP_CFG{
    BOOL                    bDual;                          ///< multi-display
    DISPSRV_FP_EVENT_CB     fpEvent;                        ///< Event callback
    DISPSRV_DISP_DESC       Desc[DISPSRV_DISP_IDX_MAX_NUM]; ///< primary and secondary description
}DISPSRV_DISP_CFG,*PDISPSRV_DISP_CFG;

/**
    Source description
*/
typedef struct _DISPSRV_SRC_DESC
{
    UINT32              uiNumSrc;     ///< Total source frame count
    IMG_BUF*            pSrc;         ///< Source frame description (Array) !!Must be Global Array!!
    URECT*              pSrcRect;     ///< (Optional) Source frame region (Array) !!Must be Global Array!! (dynamic change for DIS)
}DISPSRV_SRC_DESC,*PDISPSRV_SRC_DESC;


typedef INT32 (*DISPSRV_FP_LOCK_CB)(UINT32* puiLocked,UINT32 uiIdx); ///< interface for locking source buffer id. when uiIdx=DISPSRC_LOCK_NEWEST, callback give a newest frame
typedef INT32 (*DISPSRV_FP_UNLOCK_CB)(UINT32 uiIdx);                 ///< interface for unlocking source buffer id
/**
    Source Configuration
*/
typedef struct _DISPSRV_SRC_CFG
{
    BOOL                    bDual;                                  ///< multi-source for multi-display (IPP-Only)
    DISPSRV_FP_LOCK_CB      fpLockCb[DISPSRV_SRC_IDX_MAX_NUM];      ///< locking source frame
    DISPSRV_FP_UNLOCK_CB    fpUnlockCb[DISPSRV_SRC_IDX_MAX_NUM];    ///< unlocking source frame
    DISPSRV_SRC_DESC        Desc[DISPSRV_SRC_IDX_MAX_NUM];          ///< source buffer description
}DISPSRV_SRC_CFG,*PDISPSRV_SRC_CFG;


typedef DISPSRV_ER (*DISPSRV_FP_FLIP_CB)(BOOL bWaitFinish); ///< flip callback, call it can filp frame
typedef DISPSRV_ER (*DISPSRV_FP_GETDRAW_CB)(PIMG_BUF* pImg); /// get draw destination buffer index
/**
    Draw Configuration
*/
typedef struct _DISPSRV_DRAW{
    BOOL                    bDualSrc;                       ///< indicate pDS is dual
    BOOL                    bDualDst;                       ///< indicate pDD is dual
    BOOL                    bDualHandle;                    ///< indiction pImg is dual, and draw callback have to draw these output image
    DISPSRV_SRC_DESC*       pDS[DISPSRV_SRC_IDX_MAX_NUM];   ///< source buffer description for reference
    DISPSRV_DISP_DESC*      pDD[DISPSRV_DISP_IDX_MAX_NUM];  ///< main display buffer description for reference
    PIMG_BUF                pImg[2];                        ///< display buffer for using currently.
    DISPSRV_DISP_IDX        IdxDs[2];                       ///< the index of indicating pImg[] used pDD
    DISPSRV_FP_LOCK_CB      fpLock[DISPSRV_SRC_IDX_MAX_NUM];///< Api for lock source frame
    DISPSRV_FP_UNLOCK_CB    fpUnlock[DISPSRV_SRC_IDX_MAX_NUM];///< Api for unlock source frame
    DISPSRV_FP_GETDRAW_CB   fpGetDraw;                      ///< Api for get next draw buffer
    DISPSRV_FP_FLIP_CB      fpFlip;                         ///< flip frame, set TRUE to wait finish, FALSE without wait finish
    UINT32                  uiFrameCnt;                     ///< Frame count
    UINT32                  uiUserState;                    ///< Reversed for customer using
}DISPSRV_DRAW,*PDISPSRV_DRAW;

typedef INT32 (*DISPSRV_FP_DRAW_CB)(DISPSRV_DRAW* pDraw);

/**
    Fisrt Frame

    A frame description for mode descritpion to assign a initial frame on screen.
*/
typedef struct _DISPSRV_FIRST_FRAME{

    IMG_BUF                Img;     ///< Display this frame first before starting trigger draw
    USIZE                  sz;      ///< the the screen size of frame based on.
    URECT                  rc;      ///< the rect of frame based on.
}DISPSRV_FIRST_FRAME,*PDISPSRV_FIRST_FRAME;

/**
    Mode description
*/
typedef struct _DISPSRV_MODE
{
    DISPSRV_MODE_IDX        IdxMode;                ///< Mode index
    DISPSRV_USER_HANDLE     UserHandle;             ///< User handle method
    DISPSRV_FIRST_FRAME*    pFirstFrame;            ///(Optional) Display this frame first before starting trigger draw
}DISPSRV_MODE,*PDISPSRV_MODE;

/**
     Install Dispsrv task,flag and semaphore id

     This is default install API. User can create own task id, semphore id,
     flag id to adjust own stack size and task priority as following:

     @code
     {
        #define PRI_DISPSRV             4
        #define STKSIZE_DISPSRV         2048

        UINT32 DISPSRV_TSK_ID = 0;
        UINT32 DISPSRV_FLG_ID = 0;
        UINT32 DISPSRV_SEM_ID = 0;

        void Install_UserDispSrvID(void)
        {
            OS_CONFIG_TASK(DISPSRV_TSK_ID,PRI_DISPSRV,STKSIZE_DISPSRV,DispSrvTsk);
            OS_CONFIG_FLAG(DISPSRV_FLG_ID);
            OS_CONFIG_SEMPHORE(DISPSRV_SEM_ID,0,1,1);
        }
     }
     @endcode
*/
extern void DispSrv_InstallID(void) _SECTION(".kercfg_text");

/**
    Display service main task.

    Task body. Just set it into user_systasktabl in SysCfg.c. The stack size
    suggest to 4096KB. If there are many callbacks have plug-in, the stack
    size maybe need to more.
*/
extern VOID DispSrvTsk(VOID);

/**
    Initialize DispSrv service.

    For using the DispSrv service, the task id, flag id, semphore id have to
    prepare by project layer. Assign these ids to the DISPSRV_INIT strcuture
    when system starup (just set once).

    @param[in] pInit necessary inital data.
    @return
        - @b DISPSRV_ER_OK: inital success.
        - @b DISPSRV_ER_API_VERSION: DispSrvApi.h and AppDispSrv.a not match.
*/
extern DISPSRV_ER DispSrv_Init(const DISPSRV_INIT* pInit);

/**
    Start to open the DispSrv service.

    After DispSrv service is initialized, call DispSrv_Open to start the
    service.

    @return
        - @b DISPSRV_ER_OK: open success.
        - @b DISPSRV_ER_STATE: may DispSrv_Init does not be called yet.
        - @b DISPSRV_ER_STA_TASK: failed to start task(sta_tsk failed).
*/
extern DISPSRV_ER DispSrv_Open(VOID);

/**
    Stop to open the DispSrv service.

    When DispSrv service is running, call DispSrv_Close to terminate the task
    service.

    @note: Don't call DispSrv_Close in any DispSrv service callback. In
    callback, the DispSrv_AsyncClose is allowed to call to stop recevice receive
    data but task is not terminated. The DispSrv_Close need to be called outside.

    @return
        - @b DISPSRV_ER_OK: close success.
        - @b DISPSRV_ER_INVALID_CALL: call DispSrv_Close in callback.
        - @b DISPSRV_ER_STA_TASK: failed to start task(sta_tsk failed).
*/
extern DISPSRV_ER DispSrv_Close(VOID);

/**
    Suspend the service task.

    When DispSrv service is running, call DispSrv_Suspend to stop recevice
    command. Any command will be skiped by service task. The function exit after
    task finish his suspend.

    @return
        - @b DISPSRV_ER_OK: suspend success.
        - @b DISPSRV_ER_STATE: may DispSrv_Open does not be opened yet.
*/
extern DISPSRV_ER DispSrv_Suspend(VOID);

/**
    Resume the service task.

    When DispSrv service is suspended, call DispSrv_Resume to start to
    recevice command. The function exit after task finish his resume.

    @return
        - @b DISPSRV_ER_OK: suspend success.
        - @b DISPSRV_ER_STATE: may DispSrv_Open does not be opened yet.
*/
extern DISPSRV_ER DispSrv_Resume(VOID);

/**
    Set a command action to DispSrv service.

    Configure DISPSRV_CMD and send it to the service to do some action via
    DispSrv_Cmd.

    @param[in] pCmd command description.
    @return
        - @b DISPSRV_ER_OK: send command success.
        - @b DISPSRV_ER_CMD_NOT_MATCH: internal error. Please contact to module
             maintenance staff.
        - @b DISPSRV_ER_CMD_IN_DATA: the DISPSRV_CMD.In maybe assing wrong object
             / structure
        - @b DISPSRV_ER_CMD_IN_DATA: the DISPSRV_CMD.Out maybe assing wrong object
             / structure
        - @b DISPSRV_ER_STATE: may DispSrv_Open does not be opened yet.
        - @b DISPSRV_ER_NOT_IN_IDLE: If DISPSRV_CMD.DISPSRV_PROP.bEnterOnlyIdle set
             to TRUE and service is not in idle. Notice you this command is
             skipped.
        - @b DISPSRV_ER_OUT_DATA_VOLATILE: If the command include the output data
             and set DISPSRV_CMD.DISPSRV_PROP.bExitCmdFinish to FALSE, the finish
             callback have to plug-in a callback function to DISPSRV_CMD
             .DISPSRV_PROP.fpFinishCb and DISPSRV_CMD.Out have to point to a
             global data structure.
        - @b DISPSRV_ER_CMD_MAXDATA: internal error. Please contact to module
             maintenance staff.
*/
extern DISPSRV_ER DispSrv_Cmd(const DISPSRV_CMD* pCmd);

/**
    Suspend task service, but exit this function without finish suspend.

    This function is for advance. If you don't want to wait DispSrv_Suspend()
    finish too long and you want to pipe some flow, you can do the flow that
    DispSrv_AsyncSuspend() -> Do Something -> DispSrv_Suspend();

    @note: DispSrv_Suspend() is also called necessary to confirm the suspend is
           finished.

    @return
        - @b DISPSRV_ER_OK: suspend success.
        - @b DISPSRV_ER_STATE: may DispSrv_Open does not be opened yet.
*/
extern DISPSRV_ER DispSrv_AsyncSuspend(VOID);

/**
    Stop task service, but exit this function without finish close.

    This function is for advance. If you don't want to wait DispSrv_Close()
    finish too long and you want to pipe some flow, you can do the flow that
    DispSrv_AsyncClose() -> Do Something -> DispSrv_Close();

    @note: DispSrv_AsyncClose() is also called necessary to confirm the suspend is
           finished.

    @return
        - @b DISPSRV_ER_OK: suspend success.
        - @b DISPSRV_ER_STATE: may DispSrv_Open does not be opened yet.
*/
extern DISPSRV_ER DispSrv_AsyncClose(VOID);

/**
    Dump DispSrv application library information.

    The information includes version, build date, last error code, last warning
    code, last command, last state and check internal data.

    @return
        - @b DISPSRV_ER_OK: suspend success.
*/
extern DISPSRV_ER DispSrv_DumpInfo(VOID);

/**
    Trigger next frame

    For calling by each timer isr or frame end to trigger DispSrv to draw next
    frame.

    - @b DISPSRV_ER_OK: trigger success.
*/
extern DISPSRV_ER DispSrv_TriggerNext(void);

/**
    Install SxCmd

    Install uart command to SxCmd
*/
extern void DispSrv_InstallCmd(void);

//@}
#endif

