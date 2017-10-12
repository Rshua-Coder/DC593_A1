/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SYSCOMMON.h
    @ingroup    mIPRJAPUIFlow

    @brief      UIDef Functions
                This file is the user interface ( for LIB callback function).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef _SYSCOMMON_H
#define _SYSCOMMON_H

//Kernel
#include "Type.h"
#include "SysKer.h"
#include "Debug.h"
//Lib
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
//#include "PStore.h"
#include "GxLib.h"
#include "FilesysTsk.h" //to replace GxFile
//NvtSystem
#include "NvtSystem.h"
#include "UIFramework.h"

//Config
#include "SysCfg.h"
#include "PrjCfg.h"

//C (Events)
#include "NVTToolCommand.h"
#include "NVTUserCommand.h"


/////////////////////////////////////////

#include "SysMain.h"

//#include "UISetup.h"
//#include "UIBackgroundObj.h"
//#include "UIInfo.h"
//#NT#2010/01/20#JeahYen -begin
//#include "UIMode.h"
//#NT#2010/01/20#JeahYen -end

//#NT#2010/07/14#Jeah Yen -begin
extern int SX_TIMER_DET_KEY_ID;
extern int SX_TIMER_DET_TOUCH_ID;
extern int SX_TIMER_DET_USB_ID;
extern int SX_TIMER_DET_PWR_ID;
extern int SX_TIMER_DET_MODE_ID;
extern int SX_TIMER_DET_TIMER_ID;
extern int SX_TIMER_DET_LED_ID;
extern int SX_TIMER_DET_STRG_ID;
extern int SX_TIMER_DET_TV_ID;
//#NT#2010/11/26#Photon Lin -begin
extern int SX_TIMER_DET_LCDREG_ID;
//#NT#2010/11/26#Photon Lin -end
extern int SX_TIMER_DET_BATT_ID;
extern int SX_TIMER_DET_SLEEP_ID;
extern int SX_TIMER_DET_AUTOPOWEROFF_ID;
extern int SX_TIMER_DET_AUDIODEV_ID;
extern int SX_TIMER_DET_FLASH_ID;
extern int SX_TIMER_AUTO_INPUT_ID;
extern int SX_TIMER_BURNIN_AUTORUN_ID;
extern int SX_TIMER_DET_RECHARGE_ID;
extern int SX_TIMER_DET_SYSTEM_BUSY_ID;
extern int SX_TIMER_DET_CLOSE_LENS_ID;
//extern int SX_TIMER_DET_BACKLIGHT;
extern int SX_TIMER_DET_EV_ID;
extern int SX_TIMER_DET_SHOWADC;
extern int SX_TIMER_DET_MEM;
extern int SX_TIMER_NET_CONNECT_ID;
//#NT#2010/07/14#Jeah Yen -end

#define AUTO_INPUT_TIMER_CNT    2

//#NT#2010/07/14#Jeah Yen -begin
//custom CB
extern void USB_CB(UINT32 event, UINT32 param1, UINT32 param2);
extern void USB_PlugInSetFunc(void);
extern void PST_CB(UINT32 event, UINT32 param1, UINT32 param2);
extern void Timer_CB(UINT32 event, UINT32 param1, UINT32 param2);
extern void Sound_CB(UINT32 event, UINT32 param1, UINT32 param2);
extern void LED_CB(UINT32 event, UINT32 param1, UINT32 param2);
extern void Lens_CB(UINT32 event, UINT32 param1, UINT32 param2);
extern void IPL_CB(UINT32 event, UINT32 param1, UINT32 param2);
//#NT#2010/07/14#Jeah Yen -end


//#NT#2010/07/20#Jeah Yen -begin
extern void Load_IPLInfo(void);
extern void Load_ResInfo(void);
//#NT#2010/07/20#Jeah Yen -begin
//#NT#2010/09/17#Jeah Yen -begin
extern void Load_DispLogo(void);
extern void Load_LangString(void);
//#NT#2010/09/17#Jeah Yen -end



void System_EnableCache(UINT32 pAddr,  INT32 lSize);
//#NT#2010/07/14#Jeah Yen -end

extern BOOL gIsUSBChargePreCheck;

//#NT#2010/07/14#Jeah Yen -begin
extern int PRIMARY_MODE_MAIN;   ///< Main
extern int PRIMARY_MODE_PLAYBACK;   ///< Playback
extern int PRIMARY_MODE_PHOTO;   ///< Photo preview
extern int PRIMARY_MODE_MOVIE;      ///< Movie preview
extern int PRIMARY_MODE_USBMSDC;    ///< USB MSDC
extern int PRIMARY_MODE_USBSIDC;    ///< USB SIDC/PTP
extern int PRIMARY_MODE_PRINTER;    ///< USB Pictbridge/Printer
extern int PRIMARY_MODE_USBPCC;     ///< USB PC camera
extern int PRIMARY_MODE_USBYOUTUBE;     ///< USB YouTube
//#NT#2010/12/15#Lily Kao -begin
extern int PRIMARY_MODE_USBCHARGE;     ///< USB CHARGE
//#NT#2010/12/15#Lily Kao -begin
extern int PRIMARY_MODE_USBMENU;     ///< USB MENU
#define PRIMARY_MODE_UNKNOWN    (-1)
//#NT#2010/07/14#Jeah Yen -end

//#NT#2012/10/02#Philex Lin -begin
#if (UI_STYLE==UI_STYLE_DRIVER)
extern int PRIMARY_MODE_CALIBRATION;   ///< Calibration
#endif
//#NT#2012/10/02#Philex Lin -end

//#NT#2010/12/22#Jeah Yen -begin
void System_PowerOn_FromChargeMode(BOOL bPB);
//#NT#2010/12/22#Jeah Yen -end


//custom Job
void System_DetBusy(void);
void System_DetCloseLens(void) ;
void System_ResetDetCloseLenCount(void);
void System_DetBackLight(void);
void System_DetShowADC(void);
//#NT#2010/11/25#Photon Lin -begin
//#NT#2010/08/02#Ben Wang -begin
//custom key sound setting
#define FLGKEY_SOUND_MASK_DEFAULT     (FLGKEY_KEY_MASK &~(FLGKEY_SHUTTER2 |FLGKEY_SHUTTER1 | FLGKEY_ZOOMIN | FLGKEY_ZOOMOUT | FLGKEY_MOVIE))
//#NT#2010/08/02#Ben Wang -end
//#NT#2010/11/25#Photon Lin -end
//utility function

#define SYS_POWEROFF_BATT_EMPTY            1
#define SYS_POWEROFF_LENS_ERROR            2

//#NT#2010/08/13#Lincy Lin -begin
//#NT#Add USB charge function
#define USB_PWRON_NONE             0
#define USB_PWRON_ADPIN_BATIN      1
#define USB_PWRON_ADPIN_BATOUT     2
#define USB_PWRON_ADPOUT_BATIN     3
#define USB_PWRON_ADPOUT_BATOUT    4

//#NT#2010/11/26#Jeah Yen -begin
#define USB_SRC_NONE            0
#define USB_SRC_USB_ADAPTER     1
#define USB_SRC_USB_PC          2
#define USB_SRC_USB_CHARGING_PC 3
//#NT#2010/12/15#Jeah Yen -begin
#define USB_SRC_UNKNOWN         0xffffffff
//#NT#2010/12/15#Jeah Yen -end
#define POWER_SRC_BATTERY       0
#define POWER_SRC_USB_ADAPTER   1
#define POWER_SRC_USB_PC        2
//#NT#2010/12/15#Jeah Yen -begin
#define POWER_SRC_UNKNOWN       0xffffffff
//#NT#2010/12/15#Jeah Yen -end

//#NT#2011/1/13#Jeah Yen -begin
//PowerOn src detect
extern void Power_UpdateSource(void);
extern UINT32 Power_GetSource(void);

//USB src detect
extern void USB_UpdateSource(void);
extern UINT32 USB_GetSource(void);

//USB MSDC NVT
extern void USB_SetForceMsdcNvt(BOOL bEn);
extern BOOL USB_GetIsMsdcNvt(void);

//USB insert detect
extern void USB_PowerOn_Start(void);
extern void USB_PowerOn_End(void);
extern void USB_PowerOff_Start(void);
extern void USB_PowerOff_End(void);

//CARD insert detect
extern void Storage_UpdateSource(void);
extern void Storage_PowerOn_Start(void);
extern void Storage_PowerOn_End(void);


#define UPDNAND_STS_FW_OK               0   // Update FW to NAND OK
#define UPDNAND_STS_FW_INVALID_STG      1   // Invalid source storage
#define UPDNAND_STS_FW_READ_ERR         2   // FW doesn't exist or read error
#define UPDNAND_STS_FW_READ_CHK_ERR     3   // Read FW checksum failed, might be error
#define UPDNAND_STS_FW_WRITE_ERR        4   // Write FW to NAND error
#define UPDNAND_STS_FW_READ2_ERR        5   // Read FW from NAND failed (for write checking)
#define UPDNAND_STS_FW_WRITE_CHK_ERR    6   // Write FW checksum failed

extern UINT32   System_OnStrg_UploadFW(UINT32 DevID);



//USB=>Battery charge flow
//System Reset Mode
#define SRM_DEFAULT                 0x00000000
#define SRM_NORMAL_PRESS_PWRKEY     0x0000AAAA
#define SRM_CHARGE_PRESS_PWRKEY     0x0000BBBB
#define SRM_CHARGE_PRESS_PLAYKEY    0x0000CCCC
extern void Power_StartUSBCharge(void);
extern void Power_StopUSBCharge(void);

//SysInput
extern void Input_ResetMask(void);


//

//custom flow
//void Display_ChangeDevice(UINT32 dev_id);

//#NT#2010/10/21#Lincy Lin -end
extern void AutoInput(void);
//#NT#2010/12/22#Jeah Yen -begin
//#NT#2013/01/29#Lincy Lin -begin
//#NT#
typedef enum _SPLASH_ID
{
    SPLASH_POWERON = 0,                 ///< power on logo
    SPLASH_POWEROFF,                    ///< power off logo
    ENUM_DUMMY4WORD(SPLASH_ID)
} SPLASH_ID;
extern void Display_ShowSplash(SPLASH_ID splash_id);
//#NT#2013/01/29#Lincy Lin -end

//#NT#2011/02/14#Steven feng -begin
typedef struct _SSWAV_Info_
{
    CHAR  uiPS_SectionName[5];
    char* ui_pFilePathName;
}SSWAV_Info;
//#NT#2011/02/14#Steven feng -end


typedef struct _Language_Info_
{
    UINT32  uiPS_SectionName;
    char* ui_pFilePathName;
}Language_Info;

#if(_LANG_STORE_ == _PSTORE_)
extern Language_Info g_LanguageInfo[LANGUAGE_SETTING_MAX];
extern UINT32 g_pLanguageSize_[LANGUAGE_SETTING_MAX];
#endif
extern BOOL g_bIsInitSystemFinish;


//#NT#2010/08/02#Jeah Yen -begin
void IPL_PowerOn_Load(void);
void IPL_PowerOn_OpenSensor(void);
void IPL_PowerOn_Apply(void);
void IPL_PowerOn_ChangeMode(void);
void IPL_PowerOff_CloseSensor(void);

void Lens_PowerOn_Load(void);
void Lens_PowerOn_Apply(void);
void Lens_PowerOn_ChangeMode(void);
//#NT#2010/08/02#Jeah Yen -end

extern void Sound_SetRepeat(UINT32 isTrue);
extern UINT32 Sound_GetRepeat(void);
extern void Sound_PollingPlayStop(UINT32 uiPollingTime);

extern void File_InitDCFID(void);
extern void File_ResetDCFID(void);
extern void File_LoadDCFID(void);
extern void File_SaveDCFID(void);
extern void File_IncPictCnt(void);

extern UINT32 Command_GetTempBuffer(UINT32 uiSize);


#endif//_SYSCOMMON_H
