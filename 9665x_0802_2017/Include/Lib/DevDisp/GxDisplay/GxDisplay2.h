#ifndef _GXDISPLAY2_H
#define _GXDISPLAY2_H

#include "GxCommon.h"
#include "GxDisplay.h"
#include "DispDef.h"
//#include "LCDTV.h"

/*

#define DISPLAY_CB_TVPLUG           1
#define DISPLAY_CB_TVUNPLUG         2
#define DISPLAY_CB_CHANGESIZE       3
#define DISPLAY_CB_SHOWSPLASH       4
#define DISPLAY_DEVCTRL_MODE        0
#define DISPLAY_DEVCTRL_SLEEP       1
#define DISPLAY_DEVCTRL_BACKLIGHT   2
//#NT#2009/07/10#Jeah Yen -begin
#define DISPLAY_DEVCTRL_BRIGHTLVL   3
//#NT#2009/07/10#Jeah Yen -end
//#NT#2010/03/08#Jeah Yen -begin
#define DISPLAY_DEVCTRL_FORCEDET    4
//#NT#2010/03/08#Jeah Yen -end
#define DISPLAY_DEVCTRL_IOTYPE      5

// LCD control object
//extern LCD_DEVICE_OBJ   g_LCDCtrlObj;

extern UINT32 gDevLCDObj;
extern UINT32 gDevTVObj;

typedef struct t_DISPLAY_CFG
{
    UINT32 uiDeviceLCD;
    UINT32 uiDeviceTV;
    UINT32 uiDeviceFirst;
    UINT32 uiReserved[5];
}
DISPLAY_CFG;

extern void Display_SetConfig(DISPLAY_CFG* pInitCfg);

//fast function
extern void Display_SetEnable(UINT8 LayerID, BOOL bEnable);
//#NT#2010/11/2#JeahYen -begin
extern void Display_SetBuffer(UINT8 LayerID, UINT32 w, UINT32 h, UINT32 off, UINT32 fmt, UINT32 uiBufY, UINT32 uiBufCb, UINT32 uiBufCr);
//#NT#2010/11/2#JeahYen -end
extern void Display_SetWindow(UINT8 LayerID, int x, int y, int w, int h);

extern BOOL Display_OnCommand(CHAR *pcCmdStr);
extern void Display_OnSystem(int cmd);
//extern void Display_CB(int event, UINT32 param1, UINT32 param2);
*/

/*
//#NT#2010/01/18#JeahYen -begin
BOOL GxDisplay_IsLockDevice(void);
//#NT#2010/11/26#Jeah Yen -begin
BOOL GxDisplay_LockDevice(void);
BOOL GxDisplay_UnlockDevice(void);
//#NT#2010/11/26#Jeah Yen -end

//avaliable in lock state
extern UINT32 GxDisplay_ChangeDevice(UINT32 NewDevObj);
extern UINT32 GxDisplay_LockDeviceDeep(void);
extern UINT32 GxDisplay_UnlockDeviceDeep(void);

//avaliable in unlock state
extern UINT32  GxVideo_GetDevice(void);
extern void GxVideo_SetDeviceCtrl(UINT32 data, UINT32 value);
extern UINT32 GxDisplay_GetDeviceCtrl(UINT32 data);
//#NT#2010/01/18#JeahYen -end

extern ISIZE GxVideo_GetDeviceSize(void);
//#NT#2010/09/28#Niven Cho -begin
extern void GxDisplay_SetDeviceSize(ISIZE sz);
//#NT#2010/09/28#Niven Cho -end
//#NT#2010/09/28#JeahYen -begin
extern INT32 GxDisplay_GetDeviceW(void);
extern INT32 GxDisplay_GetDeviceH(void);
//#NT#2010/09/28#JeahYen -end
extern UINT32 GxDisplay_GetDeviceMode(void);

extern void GxVideo_DetTV(void);

void GxDisplay_RegCB(GX_CALLBACK_PTR fpDispCB);

void GxDisplay_DumpInfo(void);
*/

#endif //_GXDISPLAY2_H
