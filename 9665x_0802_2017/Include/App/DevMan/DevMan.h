/**
    DevMan, Service command function declare

    @file       DeviceMan.c
    @ingroup    mDEVMAN

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _DEVICEMAN_H
#define _DEVICEMAN_H

#include "SysKer.h"
#include "DevManApi.h"

/**
    Device Index.

    Device name for different devices.
*/
typedef enum _DEVICE_ID
{
    SYS,     // Power, Input, Output, Misc
    NAND,    // PST
    DISP,    // Display
    SENSOR,  // Sensor(IPL, IME, GE)
    LENS,    // Lens
    CARD,    // Storage, File
    AUDIO,   // Sound
    USB,     // USB
    DEVICE_ID_MAX,     // total device numbers
    ENUM_DUMMY4WORD(DEVICE_ID)
}DEVICE_ID;

/**
    Command Index.

    The command of DevMan service support. Set this index to the DEVMAN_CMD
    and call the DevMan_Cmd to do command when service is opened.
*/
typedef enum _DEVMAN_COMMON_CMD{
    DEVMAN_CMD_UNKNOWN  = 0,    ///< unknown command
    DEVMAN_CMD_INIT,            ///<
    DEVMAN_CMD_EXIT,            ///<
    DEVMAN_CMD_CHANGE,          ///<
    DEVMAN_CMD_SETMODE,         ///<
    DEVMAN_CMD_SETSLEEP,        ///<
    DEVMAN_CMD_SETACTIVE,       ///<
    DEVMAN_CMD_MAX,             ///< total command numbers
    ENUM_DUMMY4WORD(DEVMAN_COMMON_CMD)
}DEVMAN_COMMON_CMD;

/**
    Sync Index.

    Control flag to sync command detail execution.
*/
typedef enum _DEVMAN_COMMON_SYNC{

    DEVMAN_SYNC_INIT_BEGIN  = 0,
    DEVMAN_SYNC_INIT_POSTOPEN,
    DEVMAN_SYNC_INIT_END,
    DEVMAN_SYNC_EXIT_BEGIN,
    DEVMAN_SYNC_EXIT_POSTCLOSE,
    DEVMAN_SYNC_EXIT_END,
    DEVMAN_SYNC_MAX,            ///< total command numbers
    ENUM_DUMMY4WORD(DEVMAN_COMMON_SYNC)
}DEVMAN_COMMON_SYNC;


typedef struct _DEVMAN
{
    char* strName;
    DEVMAN_HANDLE h;
    void (*Open)(void);                                     ///< OPEN
    void (*Close)(void);                                    ///< CLOSE
    void (*Cmd)(UINT32 cmd, UINT32 param1, UINT32 param2);  ///< TRIGGER CMD, SYNC MODE
    void (*CmdAsync)(UINT32 cmd, UINT32 param1, UINT32 param2);  ///< TRIGGER CMD, ASYNC MODE
    void (*WaitFinish)(UINT32 cmd);                         ///< WAIT CMD FINISH
    void (*ClrSync)(UINT32 sync);
    void (*SetSync)(UINT32 sync);
    void (*WaitSync)(UINT32 sync);
}DEVMAN;

extern DEVMAN* DevMan[DEVICE_ID_MAX];

/**

Example 1:

    DevMan[SYS]->Open();
      :
    DevMan[SYS]->Close();

Example 2:

    DevMan[SYS]->Cmd(DEVMAN_CMD_INIT, 0, 0);

Example 3:

    DevMan[SYS]->CmdAsync(DEVMAN_CMD_INIT, 0, 0);
    DevMan[SYS]->WaitFinish(DEVMAN_CMD_INIT);

Example 4:

    DevMan[SYS]->ClrSync(SYNC_OP);
    DevMan[SYS]->CmdAsync(DEVMAN_CMD_INIT, 0, 0);
      :
    DevMan[SYS]->SetSync(SYNC_OP);
      :
    DevMan[SYS]->WaitSync(SYNC_OP);
      :
    DevMan[SYS]->WaitFinish(DEVMAN_CMD_INIT);

*/


#if 0
//power on CB (post-action event)
#define POWERON_CB_START        0
#define POWERON_CB_STGOPEN      7
#define POWERON_CB_COPYPART2    8
#define POWERON_CB_COPYPART3    9
#define POWERON_CB_IPLOPEN      10
#define POWERON_CB_LENSOPEN     11
#define POWERON_CB_PWROPEN      12
#define POWERON_CB_IOOPEN       13
#define POWERON_CB_PSTOPEN      20
#define POWERON_CB_DISPOPEN     30
#define POWERON_CB_USBOPEN      40
#define POWERON_CB_SOUNDOPEN    50
#define POWERON_CB_FILEOPEN     60
#define POWERON_CB_SYSOPEN       80
#define POWERON_CB_END          100

//power off CB (pre-action event)
#define POWEROFF_CB_START       100
#define POWEROFF_CB_SYSCLOSE    80
#define POWEROFF_CB_FILECLOSE   60
#define POWEROFF_CB_SOUNDCLOSE  50
#define POWEROFF_CB_USBCLOSE    40
#define POWEROFF_CB_DISPCLOSE   30
#define POWEROFF_CB_PSTCLOSE    20
#define POWEROFF_CB_IOCLOSE     13
#define POWEROFF_CB_PWRCLOSE    12
#define POWEROFF_CB_LENSCLOSE   11
#define POWEROFF_CB_IPLCLOSE    10
#define POWEROFF_CB_STGCLOSE    9
#define POWEROFF_CB_END         0
#endif

#endif //_DEVICEMAN_H
