#ifndef _SYS_MODE
#define _SYS_MODE

#include "Type.h"

//////////////////////////////////////////////////////

typedef struct _SYS_MODE
{
    char* strName;
    void (*pfMode_OnOpen)(void);
    void (*pfMode_OnClose)(void);
    void (*pfMode_OnSleepEnter)(void);
    void (*pfMode_OnSleepLeave)(void);
    void (*pfMode_OnPauseDMA)(void);
    void (*pfMode_OnResumeDMA)(void);
}
SYS_MODE;

INT32 System_AddMode(SYS_MODE* pMode);

//////////////////////////////////////////////////////

#define SYS_STATE_POWERON       0   //default = SYS_POWERON_NORMAL
#define SYS_STATE_POWEROFF      1   //default = SYS_POWEROFF_NORMAL
#define SYS_STATE_CURRMODE      2
#define SYS_STATE_PREVMODE      3
#define SYS_STATE_NEXTMODE      4
#define SYS_STATE_ACTIVE        5
#define SYS_STATE_SLEEPLEVEL    6
#define SYS_STATE_CURRSUBMODE   7
#define SYS_STATE_PREVSUBMODE   8
#define SYS_STATE_NEXTSUBMODE   9
#define SYS_STATE_MAX           10

//SYS_STATE_MODE
//SYS_STATE_PREVMODE
//SYS_STATE_NEXTMODE
#define SYS_MODE_UNKNOWN          ((INT32)-1)

//SYS_STATE_POWERON
#define SYS_POWERON_NORMAL      0x00
#define SYS_POWERON_LOWPOWER    0x01
#define SYS_POWERON_SAFE        0x02
#define SYS_POWERON_CHARGE      0x03

//SYS_STATE_POWEROFF
#define SYS_POWEROFF_NORMAL     0x00
#define SYS_POWEROFF_LOWPOWER   0x01
#define SYS_POWEROFF_SAFE       0x02
#define SYS_POWEROFF_CHARGE     0x03
#define SYS_POWEROFF_BATTEMPTY  0x04
#define SYS_POWEROFF_LENSERROR  0x05

#define SYS_SUBMODE_NORMAL      0
#define SYS_SUBMODE_WIFI        1

extern void System_PowerOn(UINT32 pwrOnType);
extern void System_PowerOff(UINT32 pwrOffType);
extern void System_ChangeSleep(UINT32 sleep_lv);
extern void System_ChangeMode(UINT32 mode_id);

extern void System_ModeOpen(UINT32 mode_id);
extern void System_ModeClose(void);
extern void System_ModeSleepEnter(void);
extern void System_ModeSleepLeave(void);

extern INT32 System_GetState(INT32 state);
extern void System_SetState(INT32 state, INT32 value);

extern void System_ChangeCPUSpeed(int speed_id);
extern void System_ChangeDMASpeed(int speed_id);

extern void UserMainProc(void);

#endif
