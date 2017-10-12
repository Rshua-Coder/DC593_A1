////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIAppPhoto.h"
#include "UIAppPlay.h"
#include "UIAppMovie.h"
#include "UISetup.h"
#include "UIInfo.h"
#include "UIPhotoInfo.h"
#include "ProjectInfo.h"
#include "DateTimeInfo.h"
#include "PStore.h"
//#include "NVTUserCommand.h"
#include "UIResource.h"
#include "UICfgDefault.h"
#include "DCF.h"
#include "PowerDef.h"
#include "usb.h"
#include "RTC.h"
#include "FileDB.h"
#include "NameRule_FileDB.h"
#include "BinInfo.h"
#include "Audio.h"
#include "UIConfig.h"
#include "EngineerMode.h"
#include "UsbDevDef.h"
#include "UIAppNetwork.h"
#ifdef __ECOS
#include "wps_api.h"
#endif
#include "DxPower.h"

////////////////////////////////////////////////////////////////////////////////
//---------------------UIInfo Debug Definition -----------------------------
#define _UIINFO_ERROR_MSG        1
#define _UIINFO_TRACE_MSG        0

#if (_UIINFO_ERROR_MSG && (PRJ_DBG_LVL>=PRJ_DBG_LVL_ERR))
#define UIInfoErrMsg(...)            debug_msg ("^R UIInfo: "__VA_ARGS__)
#else
#define UIInfoErrMsg(...)
#endif

#if (_UIINFO_TRACE_MSG && (PRJ_DBG_LVL>=PRJ_DBG_LVL_TRC))
#define UIInfoTraceMsg(...)          debug_msg ("^B UIInfo: "__VA_ARGS__)
#else
#define UIInfoTraceMsg(...)
#endif


#define SYSINFO_NAND    ENABLE //DISABLE

//---------------------UIInfo Global Variables -----------------------------

//static UINT32 UIParameter[FL_IndexInfoMAX] = {0}; //#NT#Init the array to zero.

//#NT#2010/09/17#Jeah Yen -begin
//UISysInfo sysInfo={0}; //#NT#Init the array to zero.
//#NT#2010/09/17#Jeah Yen -end

UIMenuStoreInfo  currentInfo={0}; //#NT#Init the array to zero.


BOOL bSysReady = 0;
BOOL bAutoSystemReset = FALSE;
//static BOOL bAutoReset = FALSE;
static UINT32 guiBatteryLevel       = UIDFT_BATTERY_LVL;

extern BININFO gBinInfo;
//---------------------UIInfo Prototype Declaration  -----------------------
//---------------------UIInfo Public API  ----------------------------------
//---------------------UIInfo Private API ----------------------------------
void SysSetFixedFlagSysInit(void);

//------------------------------------------------------------
extern BOOL Sensor_CheckExtSensor(void);

void Load_SysInfo(void)
{
//#NT#2010/09/17#Jeah Yen -begin
/*
    UIInfoTraceMsg("Read SYSINFO\r\n");
#if (SYSINFO_NAND==ENABLE)
    PStore_ReadReServedArea((UINT8*)&sysInfo,sizeof(UISysInfo));
#else
    //set to default value
    sysInfo.ubBeepPowerIndex = BEEP_TONE_1;
    sysInfo.ubBeepVolumnIndex = BEEP_VOL_2;
    sysInfo.ubWallpaperStartupIndex = WALLPAPER_STARTUP_GE;
    sysInfo.uhInfoSize = sizeof(sysInfo);
#endif
*/
//#NT#2010/09/17#Jeah Yen -end
}

void Save_SysInfo(void)
{
//#NT#2010/09/17#Jeah Yen -begin
/*
    UIInfoTraceMsg("Write SYSINFO\r\n");
#if (SYSINFO_NAND==ENABLE)
    PStore_WriteReServedArea((UINT8*)&sysInfo,sizeof(UISysInfo));
#endif
*/
//#NT#2010/09/17#Jeah Yen -end
}


//------------------------------------------------------------
CHAR previewEVStr[EV_SETTING_MAX][5]=
{
    "-2.0",
    "-1.7",
    "-1.3",
    "-1.0",
    "-0.7",
    "-0.3",
    "+0.0",
    "+0.3",
    "+0.7",
    "+1.0",
    "+1.3",
    "+1.7",
    "+2.0"
};

void UI_SetInitVolume(UINT32 volumeIndex)
{
    const static AUDIO_VOL volume_gain[BEEP_VOL_SETTING_MAX] =
        {AUDIO_GAIN_MUTE, AUDIO_GAIN_3, AUDIO_GAIN_5, AUDIO_GAIN_7};

    if(volumeIndex >= BEEP_VOL_SETTING_MAX)
        volumeIndex = BEEP_VOL_2; //set to default

    GxSound_SetVolume(volume_gain[volumeIndex]);
}

void UI_SetFileDBFileID(void)
{
#if USE_FILEDB
    RTC_DATE    Date;

    Date = rtc_getDate();
    if ((Date.s.year==currentInfo.cbYear)&&(Date.s.month==currentInfo.cbMonth)&&(Date.s.day==currentInfo.cbDay))
    {
        NH_FileDB_SetFileID(currentInfo.cbFileID);
    }
#endif
}

void UI_SaveRTCDate(void)
{
#if USE_FILEDB
    FILEDB_HANDLE fileDbHandle = 0;
    FILEDB_FILE_ATTR *pFileAttr;
    RTC_DATE    Date;
    CHAR datetime[10];
    UINT32 uiFileID;

    pFileAttr = FileDB_CurrFile(fileDbHandle);
    if (pFileAttr != NULL)
    {
        strncpy(datetime,pFileAttr->filename,4);
        datetime[4]='\0';
        Date.s.year = atoi(datetime);

        strncpy(datetime,pFileAttr->filename+5,2);
        datetime[2]='\0';
        Date.s.month = atoi(datetime);

        strncpy(datetime,pFileAttr->filename+7,2);
        datetime[2]='\0';
        Date.s.day = atoi(datetime);

        strncpy(datetime,pFileAttr->filename+17,3);
        datetime[3]='\0';
        uiFileID = atoi(datetime);

        currentInfo.cbYear = Date.s.year;
        currentInfo.cbMonth = Date.s.month;
        currentInfo.cbDay = Date.s.day;
        currentInfo.cbFileID = uiFileID;
    }
#endif
}

void UI_CheckProductSN(void)
{
    char* ReadSerialNum;
    ReadSerialNum =EngineerMode_ReadSerialNum();
    if(strlen(ReadSerialNum)==0)
    {
        if(currentInfo.strProductSN[0] == 0)
        {
            sprintf(currentInfo.strProductSN,"%s%s", "12345678",EngineerMode_ReadAPMAC());
        }
    }
    else
    {
        sprintf(currentInfo.strProductSN,"%s", ReadSerialNum);
    }
}

void Load_MenuInfo(void)
{
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32  result;
    UINT32  uiFWUpdate = 0;
    UIInfoTraceMsg("Read UIINFO\r\n");

    uiFWUpdate = (gBinInfo.ld.LdCtrl2 & LDCF_UPDATE_FW);
    // Read system info from PStore
    if ((pSection = PStore_OpenSection(PS_SYS_PARAM, PS_RDWR)) != E_PS_SECHDLER)
    {
        result = PStore_ReadSection((UINT8 *)&currentInfo, 0, sizeof(UIMenuStoreInfo), pSection);
        if (result != E_PS_OK)
        {
            UIInfoErrMsg("PStore sys param fail\r\n");
            currentInfo.uhInfoSize = sizeof(UIMenuStoreInfo);
            SysResetFlag();
            PStore_WriteSection((UINT8 *)&currentInfo,0,sizeof(UIMenuStoreInfo)*2,pSection);
        }
        PStore_CloseSection(pSection);
        //#NT#2013/3/15#Philex Lin - begin
        //Add do default setting when FW updates/FW version changed
        if(currentInfo.uhInfoSize != sizeof(currentInfo))
        {
            Reset_MenuInfo();
        } else if (uiFWUpdate)
        {
            Reset_MenuInfo();
        }
        //#NT#2013/3/15#Philex Lin - end

        bSysReady = 1;
    }
    else
    {
        Reset_MenuInfo();
        UIInfoErrMsg("PStore Read sys param fail\r\n");
    }
    bSysReady = 1;
    UI_CheckProductSN();

// Following test code will reset WiFi SSID in PStore.
// It only be used when burned eFuse mac to reset the SSID!!
#if 0
    EngineerMode_TurnOnWiFi(); // test, turn on WiFi
    Delay_DelayMs(5000);
    EngineerMode_SaveWiFiMAC();
#endif

    UIInfoTraceMsg("Read UIRES\r\n");
}

//#NT#2010/09/17#Jeah Yen -begin
void Load_LangString(void)
{
    //load language
    UIRes_InitReadLang(currentInfo.ubLanguageIndex);
}
//#NT#2010/09/17#Jeah Yen -end

void Save_MenuInfo()
{
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32 error=0;
    if(!bSysReady)
    {
        UIInfoErrMsg("PStore sys param not save before load pstore!!!\r\n");
        return;
    }
    UIInfoTraceMsg("Write UIINFO\r\n");
    //#NT#2013/3/15#Philex Lin - begin
    /*--- General ---*/
    strncpy(currentInfo.strSoftwareVer, Prj_GetVersionString(), (VER_STR_MAX - 1));
    currentInfo.strSoftwareVer[strlen(currentInfo.strSoftwareVer)] = '\0';
    currentInfo.uhInfoSize = sizeof(UIMenuStoreInfo);
    //#NT#2013/3/15#Philex Lin - end

    //Using PStore to read / sys param
    if ((pSection = PStore_OpenSection(PS_SYS_PARAM, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        PStore_WriteSection((UINT8 *)&currentInfo, 0, sizeof(UIMenuStoreInfo), pSection);
        PStore_CloseSection(pSection);
    }
    else
    {
        UIInfoErrMsg("Write sys param fail\r\n");
    }

    PStore_CheckSection(PS_SYS_PARAM, &error);

    if(error!=E_PS_OK)
    {
        UIInfoErrMsg("PStore error msg :%x\r\n",error);
        {
          UINT32 BufSize = 0x1800000;
          UINT8 *pBuf = (UINT8*)SxCmd_GetTempMem(BufSize);

            //open file system for save file
            //Dep_Fs_Init(TRUE,TRUE);
            PStore_Dump(pBuf,BufSize);
        }
        Ux_DumpEvents();
//        while (1);
    }

}

void Reset_MenuInfo(void)
{
    SysResetFlag();             // reset system flags
    SysExeMenuSettingFuncs();   // execute menu setting related functions

    /* After reset, save to NAND */
    Save_MenuInfo();
}

void Init_SysInfo(void)
{
    UIInfoTraceMsg("[Init_SysInfo][NAND update state]\r\n");
}

void Init_MenuInfo(void)
{
    UIInfoTraceMsg("[Init_MenuInfo][PStore update state]\r\n");
    UIInfoTraceMsg("Version %s  %d\r\n", currentInfo.pVersion, currentInfo.uhInfoSize);

    // reset to project defined default date time if RTC power lost
    if (HwClock_IsPowerLost())
    {
        DateTime_SaveToDefault();
    }

    SysCheckFlag();             // check flag (limit value)
    SysSetFixedFlag();          // set fixed flag
    SysSetFixedFlagSysInit();   // set fixed flag for system init only
    SysExeMenuSettingFuncs();   // execute menu setting related functions
}


UINT32 UI_GetData(UINT32 attribute)
{
    UIInfoTraceMsg("get UIParam[%d]\r\n", attribute);
    return currentInfo.UIParameter[attribute];
}
void UI_SetData(UINT32 attribute,UINT32 value)
{
    UIInfoTraceMsg("set UIParam[%d]= %d\r\n", attribute, value);
    currentInfo.UIParameter[attribute]=value;
}

//-----------------------------------------------------------------------------
// Get USB connect type but naming is backward compatible for NT96220
//-----------------------------------------------------------------------------
/**
  Return AC plug in or unplugged

  Return AC plug in or unplugged.
  TRUE  -> AC plug in.
  FALSE -> AC is unplugged.

  @param void
  @return BOOL: TRUE -> AC plug in, FALSE -> AC is unplugged
*/
BOOL KeyScan_IsACIn(void)
{
    BOOL bACStatus;
    UINT32 uiUSBConnectType = GxUSB_GetConnectType();
    if (uiUSBConnectType == USB_CONNECT_CHARGER)
        bACStatus = TRUE;
    else if (uiUSBConnectType == USB_CONNECT_CHARGING_PC)
        bACStatus = TRUE;
    else
        bACStatus = FALSE;

    return bACStatus;
}

//-----------------------------------------------------------------------------
// Auto Poweroff/USB detect/Enable Key sound scan items
//-----------------------------------------------------------------------------
void KeyScan_EnableMisc(BOOL bEn)
{
    if (bEn == FALSE)
    {
        // disable auto power off/USB detect timer
        // disable key tone flag
        GxPower_SetControl(GXPWR_CTRL_AUTOPOWEROFF_RESET,0);
        GxPower_SetControl(GXPWR_CTRL_SLEEP_RESET,0);
        SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID,FALSE);
        #if 0 // Do not disable USB detection timer.
        SxTimer_SetFuncActive(SX_TIMER_DET_USB_ID, FALSE);
        #endif
        UISound_EnableKey(FALSE);
    } else {
        // enable auto power off/USB detect timer
        // enable key tone flag
        //#NT#2012/11/08#Ben Wang -begin
        //#NT#Enable USB detection for unplugging.
        if (GxUSB_GetConnectType() == USB_CONNECT_NONE||GxUSB_GetConnectType() == USB_CONNECT_UNKNOWN)
        {
            SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID,TRUE);
            //SxTimer_SetFuncActive(SX_TIMER_DET_USB_ID, TRUE);
        } else
        {
            SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID,FALSE);
            //SxTimer_SetFuncActive(SX_TIMER_DET_USB_ID, FALSE);
        }
        SxTimer_SetFuncActive(SX_TIMER_DET_USB_ID, TRUE);
        //#NT#2012/11/08#Ben Wang -end
        UISound_EnableKey(TRUE);
    }
}


//-----------------------------------------------------------------------------
// Battery Level
//-----------------------------------------------------------------------------
UINT32 GetBatteryLevel(void)
{

    if (KeyScan_IsACIn())
    {
        guiBatteryLevel = BATTERY_CHARGE;
    } else {
        switch (DrvPower_GetControl(DRVPWR_CTRL_BATTERY_LEVEL))//(GxPower_GetControl(GXPWR_CTRL_BATTERY_LEVEL))
        {
          case VOLDET_BATTERY_LVL_4:
               guiBatteryLevel = BATTERY_FULL;
            break;
          case VOLDET_BATTERY_LVL_3:
               guiBatteryLevel = BATTERY_MED;
            break;
          case VOLDET_BATTERY_LVL_2:
               guiBatteryLevel = BATTERY_LOW;
            break;
          case VOLDET_BATTERY_LVL_1:
               guiBatteryLevel = BATTERY_EMPTY;
            break;

          case VOLDET_BATTERY_LVL_0:
               //guiBatteryLevel = BATTERY_EXHAUSTED;
               guiBatteryLevel = BATTERY_EMPTY;
            break;
        }
    }

    return guiBatteryLevel;
}

//-----------------------------------------------------------------------------
// Set/Get System Flags
//-----------------------------------------------------------------------------
void SysSetFlag(UINT32 uiFlag, UINT32 uiValue)
{
    currentInfo.UIParameter[uiFlag]=uiValue;
}

UINT32 SysGetFlag(UINT32 uiFlag)
{
    return (currentInfo.UIParameter[uiFlag]);
}

static void SysLimitFlag(UINT32 uiFlag, UINT32 uiMinId, UINT32 uiMaxId, UINT32 uiDefaultId)
{
    UINT32  uiValue = SysGetFlag(uiFlag);

    if ((uiValue < uiMinId) || (uiValue > uiMaxId-1))
    {
        uiValue = uiDefaultId;
    }

    SysSetFlag(uiFlag, uiValue);
}

void SysCheckFlag(void)
{
    // Check and limit system flag value
    // Photo
    SysLimitFlag(FL_PHOTO_SIZE,         0,  PHOTO_SIZE_ID_MAX,          DEFAULT_PHOTO_SIZE);
    SysLimitFlag(FL_SCENE,              0,  SCENE_ID_MAX,               DEFAULT_SCENE);
    SysLimitFlag(FL_QUALITY,            0,  QUALITY_ID_MAX,             DEFAULT_PHOTO_QUALITY);
    SysLimitFlag(FL_SELFTIMER,          0,  SELFTIMER_ID_MAX,           DEFAULT_SELFTIMER);
    SysLimitFlag(FL_ANTISHAKE,          0,  ANTISHAKE_ID_MAX,           DEFAULT_ANTISHAKING);
    SysLimitFlag(FL_FD,                 0,  FD_ID_MAX,                  DEFAULT_FD);
    SysLimitFlag(FL_WB,                 0,  WB_ID_MAX,                  DEFAULT_WB);
    SysLimitFlag(FL_COLOR_EFFECT,       0,  COLOR_EFFECT_ID_MAX,        DEFAULT_PHOTO_COLOR);
    SysLimitFlag(FL_CONTINUE_SHOT,      0,  CONTINUE_SHOT_SETTING_MAX,  DEFAULT_CONTINUE_SHOT);
    SysLimitFlag(FL_SHARPNESS,          0,  SHARPNESS_ID_MAX,           DEFAULT_SHARPNESS);
    SysLimitFlag(FL_EV,                 0,  EV_SETTING_MAX,             DEFAULT_EV);
    SysLimitFlag(FL_ISO,                0,  ISO_ID_MAX,                 DEFAULT_ISO);
    SysLimitFlag(FL_METERING,           0,  METERING_ID_MAX,            DEFAULT_METERING);
    SysLimitFlag(FL_PHOTO_VIVILINK,     0,  PHOTO_VIVILINK_ID_MAX,      DEFAULT_PHOTO_VIVILINK);
    SysLimitFlag(FL_FOV,                0,  FOV_ID_MAX,                 DEFAULT_FOV);

    // Movie
    SysLimitFlag(FL_MOVIE_SIZE,         0,  MOVIE_SIZE_ID_MAX,          DEFAULT_MOVIE_SIZE);
    SysLimitFlag(FL_MOVIE_SIZE_DUAL,    0,  MOVIE_SIZE_DUAL_ID_MAX,     DEFAULT_MOVIE_SIZE_DUAL);
    SysLimitFlag(FL_MOVIE_VIVILINK,     0,  MOVIE_VIVILINK_ID_MAX,      DEFAULT_MOVIE_VIVILINK);
    SysLimitFlag(FL_MOVIE_CYCLIC_REC,   0,  MOVIE_CYCLICREC_ID_MAX,     DEFAULT_MOVIE_CYCLICREC);
    SysLimitFlag(FL_MOVIE_MOTION_DET,   0,  MOVIE_MOTIONDET_ID_MAX,     DEFAULT_MOVIE_MOTION_DET);
    SysLimitFlag(FL_MOVIE_AUDIO,        0,  MOVIE_AUDIO_ID_MAX,         DEFAULT_MOVIE_AUDIO);
    SysLimitFlag(FL_MOVIE_DATEIMPRINT,  0,  MOVIE_DATEIMPRINT_ID_MAX,   DEFAULT_MOVIE_DATEIMPRINT);
    SysLimitFlag(FL_MOVIE_WATERLOGO,    0,  MOVIE_WATERLOGO_ID_MAX,     DEFAULT_MOVIE_WATERLOGO);
    SysLimitFlag(FL_MOVIE_EV,           0,  EV_SETTING_MAX,             DEFAULT_EV);
    SysLimitFlag(FL_MOVIE_HDR,          0,  MOVIE_HDR_ID_MAX,           DEFAULT_MOVIE_HDR);
    SysLimitFlag(FL_SENSOR_ROTATE,      0,  SEN_ROTATE_MAX,             DEFAULT_SENSOR_ROTATE);
    SysLimitFlag(FL_MovieMeteringIndex, 0,  METERING_ID_MAX,            DEFAULT_METERING);
    SysLimitFlag(FL_MOVIE_FOV,          0,  FOV_ID_MAX,                 DEFAULT_FOV);
	 SysLimitFlag(FL_PARK_WAKEUP,      0,  PARK_WAKEUP_ID_MAX,             DEFAULT_PARK_WAKEUP);
	
    // Playback
    SysLimitFlag(FL_PROTECT,            0,  FILE_PROTECT_ID_MAX,    DEFAULT_PROTECT);
    SysLimitFlag(FL_SLIDE_SHOW,         0,  SLIDE_SHOW_ID_MAX,      DEFAULT_SLIDE_SHOW);

    // System
    SysLimitFlag(FL_GSENSOR,            0,  GSENSOR_ID_MAX,         DEFAULT_GSENSOR);
    SysLimitFlag(FL_LANGUAGE,           0,  LANG_ID_MAX,            DEFAULT_LANGUAGE);
    SysLimitFlag(FL_DATE_TIME,          0,  DATE_TIME_ID_MAX,       DEFAULT_DATE_TIME);
    SysLimitFlag(FL_BEEP,               0,  BEEP_ID_MAX,            DEFAULT_BEEP);
    //#NT#2011/08/03#KS Hung -begin
    SysLimitFlag(FL_SILENT,             0,  SILENT_ID_MAX,          DEFAULT_SILENT);
    //#NT#2011/08/03#KS Hung -end
    SysLimitFlag(FL_AUTO_POWER_OFF,     0,  POWEROFF_SETTING_MAX,  DEFAULT_AUTO_POWER_OFF);
    SysLimitFlag(FL_FREQUENCY,          0,  FREQUENCY_ID_MAX,       DEFAULT_FREQUENCY);
    SysLimitFlag(FL_TV_MODE,            0,  TV_MODE_ID_MAX,         DEFAULT_TV_MODE);

    SysLimitFlag(FL_DATE_FORMAT,        0,  DATE_FORMAT_ID_MAX,     DEFAULT_DATE_FORMAT);
    SysLimitFlag(FL_OPENING_LOGO,       0,  OPENING_LOGO_ID_MAX,    DEFAULT_OPENING_LOGO);
    SysLimitFlag(FL_LCD_DISPLAY,        0,  DISPLAY_ID_MAX,         DEFAULT_LCD_DISPLAY);
    SysLimitFlag(FL_LCD_BRIGHTNESS,     0,  LCDBRT_LVL_ID_MAX,      DEFAULT_LCD_BRIGHTNESS);
    SysLimitFlag(FL_MACRO,              0,  MACRO_ID_MAX,           DEFAULT_MACRO);
    SysLimitFlag(FL_USB_MODE,           0,  USB_MODE_ID_MAX,        DEFAULT_USB_MODE);

    // Sepcial flags
    SysLimitFlag(FL_DCF_DIR_ID,         MIN_DCF_DIR_NUM,    MAX_DCF_DIR_NUM,    MIN_DCF_DIR_NUM);
    SysLimitFlag(FL_DCF_FILE_ID,        MIN_DCF_FILE_NUM,   MAX_DCF_FILE_NUM,   MIN_DCF_FILE_NUM);

    //Wi-Fi
    SysLimitFlag(FL_WIFI_MOVIE_APP_PREVIEW_SIZE, 0,  WIFI_MOVIE_APP_PREVIEW_SIZE_ID_MAX, DEFAULT_MOVIE_WIFI_APP_PREVIEW_SIZE);
    SysLimitFlag(FL_WIFI_AUTO_RECORDING, 0,  WIFI_AUTO_RECORDING_ID_MAX, DEFAULT_MOVIE_WIFI_AUTO_RECORDING_OPTION);
    SysLimitFlag(FL_NetWorkMode, 0, NETWORK_MODE_SETTING_MAX, DEFAULT_WIFI_MODE);
}

void SysSetFixedFlag(void)
{
    // Fixed some system flags as system reset

    // Photo
    SysSetFlag(FL_CONTINUE_SHOT,           DEFAULT_CONTINUE_SHOT);
    SysSetFlag(FL_SELFTIMER,               DEFAULT_SELFTIMER);

    // Movie
    #if (_GYRO_EXT_==_GYRO_EXT_NONE_)
    SysSetFlag(FL_MovieRSCIndex,           MOVIE_RSC_OFF); // forced disable RSC!
    #else
    SysSetFlag(FL_MovieRSCIndex,           MOVIE_RSC_ON); // forced enable RSC!
    #endif
#if (_MULTI_RECORD_FUNC_ == DISABLE)
    SysSetFlag(FL_MOVIE_DUAL_REC,          FALSE);
#else
    SysSetFlag(FL_MOVIE_DUAL_REC,          TRUE);
#endif

    // Playback
    SysSetFlag(FL_PROTECT,                 DEFAULT_PROTECT);

    // System
    SysSetFlag(FL_LCD_DISPLAY,             DEFAULT_LCD_DISPLAY);
    SysSetFlag(FL_LCD_BRIGHTNESS,          DEFAULT_LCD_BRIGHTNESS);
    SysSetFlag(FL_MovieDisIndex,           MOVIE_DIS_OFF);

#if 0 // unnecessary
    if (UI_GetData(FL_CardStatus) == CARD_REMOVED)
    {
        SysSetFlag(FL_PHOTO_SIZE, PHOTO_SIZE_5M);
        SysSetFlag(FL_MOVIE_SIZE, MOVIE_SIZE_720P);
    }
#endif
}

void SysSetFixedFlagSysInit(void)
{
    // Set fixed system flags (only on system init)
    SysSetFlag(FL_WIFI_LINK,            WIFI_LINK_NG);
    SysSetFlag(FL_NetWorkMode,          DEFAULT_WIFI_MODE);  // Reset Wi-Fi mode to AP mode.
}

void SysResetFlag(void)
{
    // Set system flag default value here
    // Photo
    SysSetFlag(FL_QUALITY,              DEFAULT_PHOTO_QUALITY);
    SysSetFlag(FL_COLOR_EFFECT,         DEFAULT_PHOTO_COLOR);
    SysSetFlag(FL_SCENE,                DEFAULT_SCENE);
    SysSetFlag(FL_EV,                   DEFAULT_EV);
    SysSetFlag(FL_ISO,                  DEFAULT_ISO);
    SysSetFlag(FL_WB,                   DEFAULT_WB);
    SysSetFlag(FL_METERING,             DEFAULT_METERING);
    SysSetFlag(FL_SHARPNESS,            DEFAULT_SHARPNESS);
    SysSetFlag(FL_SATURATION,           DEFAULT_SATURATION);
    SysSetFlag(FL_FD,                   DEFAULT_FD);
    SysSetFlag(FL_ANTISHAKE,            DEFAULT_ANTISHAKING);
    SysSetFlag(FL_QUICK_REVIEW,         DEFAULT_QUICK_REVIEW);
    SysSetFlag(FL_DATE_STAMP,           DEFAULT_DATE_STAMP);
    SysSetFlag(FL_SELFTIMER,            DEFAULT_SELFTIMER);
    SysSetFlag(FL_FLASH_MODE,           DEFAULT_FLASH_MODE);
    SysSetFlag(FL_PHOTO_VIVILINK,       DEFAULT_PHOTO_VIVILINK);
    SysSetFlag(FL_CONTINUE_SHOT ,       DEFAULT_CONTINUE_SHOT);
    SysSetFlag(FL_FOV,                  DEFAULT_FOV);

    // Movie
////    SysSetFlag(FL_MOVIE_SIZE,           DEFAULT_MOVIE_SIZE);
    SysSetFlag(FL_MOVIE_QUALITY,        DEFAULT_MOVIE_QUALITY);
    SysSetFlag(FL_MOVIE_COLOR,          DEFAULT_MOVIE_COLOR);
    SysSetFlag(FL_MOVIE_SELFTIMER,      DEFAULT_MOVIE_SELFTIMER);
//    SysSetFlag(FL_MOVIE_EV,             DEFAULT_MOVIE_EV);
    SysSetFlag(FL_MOVIE_VIVILINK,       DEFAULT_MOVIE_VIVILINK);
    SysSetFlag(FL_MOVIE_CYCLIC_REC,     DEFAULT_MOVIE_CYCLICREC);
    SysSetFlag(FL_MOVIE_TIMELAPSE_REC,  DEFAULT_MOVIE_TIMELAPSEREC);
    SysSetFlag(FL_MOVIE_MOTION_DET,     DEFAULT_MOVIE_MOTION_DET);
    SysSetFlag(FL_MOVIE_AUDIO,          DEFAULT_MOVIE_AUDIO);
    SysSetFlag(FL_MOVIE_DATEIMPRINT,    DEFAULT_MOVIE_DATEIMPRINT);
    SysSetFlag(FL_MOVIE_WATERLOGO,      DEFAULT_MOVIE_WATERLOGO);
    SysSetFlag(FL_SENSOR_ROTATE,        DEFAULT_SENSOR_ROTATE);
    SysSetFlag(FL_MOVIE_HDR,            DEFAULT_MOVIE_HDR);
    SysSetFlag(FL_MOVIE_EV,             DEFAULT_EV);
    SysSetFlag(FL_MovieMeteringIndex,   DEFAULT_METERING);
    SysSetFlag(FL_MOVIE_FOV,            DEFAULT_FOV);
	SysSetFlag(FL_PARK_WAKEUP,            DEFAULT_PARK_WAKEUP);
	SysSetFlag(FL_PARK_WAKEUP_SET, 			FALSE);
	
    // Playback
    SysSetFlag(FL_PROTECT,              DEFAULT_PROTECT);
    SysSetFlag(FL_SLIDE_SHOW,           DEFAULT_SLIDE_SHOW);

    // System
    SysSetFlag(FL_AUTO_POWER_OFF,       DEFAULT_AUTO_POWER_OFF);
    SysSetFlag(FL_DATE_TIME,            DEFAULT_DATE_TIME);
    SysSetFlag(FL_BEEP,                 DEFAULT_BEEP);
    SysSetFlag(FL_SILENT,               DEFAULT_SILENT);
    SysSetFlag(FL_LANGUAGE,             DEFAULT_LANGUAGE);
    SysSetFlag(FL_FREQUENCY,            DEFAULT_FREQUENCY);
    SysSetFlag(FL_TV_MODE,              DEFAULT_TV_MODE);
    SysSetFlag(FL_DATE_FORMAT,          DEFAULT_DATE_FORMAT);
    SysSetFlag(FL_OPENING_LOGO,         DEFAULT_OPENING_LOGO);

    SysSetFlag(FL_LCD_DISPLAY,          DEFAULT_LCD_DISPLAY);
    SysSetFlag(FL_LCD_BRIGHTNESS,       DEFAULT_LCD_BRIGHTNESS);
    SysSetFlag(FL_MACRO,                DEFAULT_MACRO);
    SysSetFlag(FL_USB_MODE,             DEFAULT_USB_MODE);
    SysSetFlag(FL_GSENSOR,              DEFAULT_GSENSOR);

    // Wi-Fi
    SysSetFlag(FL_WIFI_MOVIE_APP_PREVIEW_SIZE, DEFAULT_MOVIE_WIFI_APP_PREVIEW_SIZE);
    SysSetFlag(FL_WIFI_AUTO_RECORDING, DEFAULT_MOVIE_WIFI_AUTO_RECORDING_OPTION);
    SysSetFlag(FL_NetWorkMode, DEFAULT_WIFI_MODE); // Reset To Wi-Fi mode to AP mode.
    UINet_ResetPASSPHRASE();
    UINet_ResetSSID();

    SysSetFixedFlag();

#if (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
    if (Sensor_CheckExtSensor() == FALSE)
    {
        SysSetFlag(FL_MOVIE_DUAL_REC,   FALSE);
    }
#endif
}

void SysExeMenuSettingFuncs(void)
{
    /*--- Photo ---*/
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_QUALITY,        1,  SysGetFlag(FL_QUALITY));
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_ISO,            1,  SysGetFlag(FL_ISO));
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_EV,             1,  SysGetFlag(FL_EV));
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_WB,             1,  SysGetFlag(FL_WB));
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_COLOR,          1,  SysGetFlag(FL_COLOR_EFFECT));
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_METERING,       1,  SysGetFlag(FL_METERING));
//    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_SHARPNESS,      1,  SysGetFlag(FL_SHARPNESS));
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_FD,             1,  SysGetFlag(FL_FD));
//    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_ANTISHAKING,    1,  SysGetFlag(FL_ANTISHAKE));
//    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_QUICKREVIEW,    1,  SysGetFlag(FL_QUICK_REVIEW));
//    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_DATEIMPRINT,    1,  SysGetFlag(FL_DATE_STAMP));
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_SIZE,           1,  SysGetFlag(FL_PHOTO_SIZE));
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_SCENEMODE,      1,  SysGetFlag(FL_SCENE));

    /*--- Movie ---*/
    {
        UINT32 uiSize;
        uiSize = SysGetFlag(FL_MOVIE_DUAL_REC) ? SysGetFlag(FL_MOVIE_SIZE_DUAL) : SysGetFlag(FL_MOVIE_SIZE);
        Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_MOVIESIZE,  1,      uiSize);
    }
    Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_MOVIEQUALITY,   1,      SysGetFlag(FL_MOVIE_QUALITY));
    Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_MOVIECOLOR,     1,      SysGetFlag(FL_MOVIE_COLOR));
//    Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_EV,             1,      SysGetFlag(FL_MOVIE_EV));

    /*--- Setup ---*/
    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_LANGUAGE,      1,  SysGetFlag(FL_LANGUAGE));
    #if TV_FUNC
    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_TVFORMAT,      1,  SysGetFlag(FL_TV_MODE));
    #endif
    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_POWEROFF,      1,  SysGetFlag(FL_AUTO_POWER_OFF));
    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_BEEPKEY,       1,  SysGetFlag(FL_BEEP));
    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_DISPLAY,       1,  SysGetFlag(FL_LCD_DISPLAY));
    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_FREQ,          1,  SysGetFlag(FL_FREQUENCY));
    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_DATEFORMAT,    1,  SysGetFlag(FL_DATE_FORMAT));
//    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_OPENING,       1,  SysGetFlag(FL_OPENING_LOGO));

    /*--- Wi-Fi ---*/
    if(UI_GetData(FL_NetWorkMode) == NETWORK_AP_MODE)
    {
        if(currentInfo.strSSID[0] != 0) // Load SSID if existed.
        {
            Ux_SendEvent(&UISetupObjCtrl, NVTEVT_EXE_WIFI_SET_SSID, 1, currentInfo.strSSID);
        }

        if(currentInfo.strPASSPHRASE[0] != 0) // Load PASSPHARSE if existed.
        {
            Ux_SendEvent(&UISetupObjCtrl, NVTEVT_EXE_WIFI_SET_PASSPHRASE, 1, currentInfo.strPASSPHRASE);
        }
    }
    else if(UI_GetData(FL_NetWorkMode) == NETWORK_STATION_MODE)
    {
        char szTemp[WSC_MAX_SSID_LEN+MAX_WEP_KEY_LEN] = {0};

        if(currentInfo.strSSID_hotspot_1[0] && currentInfo.strPASSPHRASE_hotspot_1[0]) // Load SSID and passphrase if existed.
        {
            snprintf(szTemp, WSC_MAX_SSID_LEN+MAX_WEP_KEY_LEN, "%s:%s", currentInfo.strSSID_hotspot_1, currentInfo.strPASSPHRASE_hotspot_1);
            Ux_SendEvent(&UISetupObjCtrl, NVTEVT_EXE_WIFI_SEND_SSID_PASSPHRASE, 1, szTemp);
        }
    }
}

void SysReset_MenuInfo(void)
{
    SysResetFlag();        // reset system flags
    //#NT#2012/7/29#philex - begin
    SysExeMenuSettingFuncs();   // execute menu setting related functions
    //#NT#2012/7/29#philex - end

    /* After reset, save to NAND */
    Save_MenuInfo();
}


void Photo_SetUserIndex(PHOTO_USR_ITEM_T uiItem, UINT32 uhIndex)
{
    UIInfoTraceMsg("Photo_SetUserIndex: Item %d, index %d\r\n", uiItem, uhIndex);

    if(UI_GetData(FL_IsInitSettingFlag) == FALSE)
    {
        /* Update Photo Mode user defined settings data while init setting flag is off */
        switch(uiItem)
        {
            case PHOTO_USR_FLASH:
                currentInfo.ubFlashIndex = uhIndex;
                break;
            case PHOTO_USR_EV:
                currentInfo.ubEVIndex = uhIndex;
                break;
            case PHOTO_USR_AFWINDOW:
                currentInfo.ubAFWindowIndex = uhIndex;
                break;
            case PHOTO_USR_AFBEAM:
                currentInfo.ubAFBeamIndex = uhIndex;
                break;
            case PHOTO_USR_METERING:
                currentInfo.ubAEmode = uhIndex;
                break;
            case PHOTO_USR_TIMELAPSE:
                currentInfo.ubTimelapseIndex = uhIndex;
                break;
            case PHOTO_USR_CONTSHOT:
                currentInfo.ubContshotIndex = uhIndex;
                break;
            case PHOTO_USR_CONTAF:
                currentInfo.ubContAF = uhIndex;
                break;
            case PHOTO_USR_QREVIEW:
                currentInfo.ubQviewIndex = uhIndex;
                break;
            case PHOTO_USR_DZOOMSTATUS:
                currentInfo.ubDzoom      = uhIndex;
                break;
            case PHOTO_USR_SLOWSHUTTER:
                currentInfo.ubSlowShutter = uhIndex;
                break;
            case PHOTO_USR_DATEIMPRINT:
                currentInfo.ubDatePrint = uhIndex;
                break;
            case PHOTO_USR_BLINK:
                currentInfo.ubBlinkDet = uhIndex;
                break;
            case PHOTO_USR_PANDET:
                currentInfo.ubPanModeIndex = uhIndex;
                break;
            case PHOTO_USR_COLOR:
                currentInfo.ubColorIndex = uhIndex;
                break;
            case PHOTO_USR_SIZE:
                currentInfo.ubSizeIndex = uhIndex;
                break;
            case PHOTO_USR_QUALITY:
                currentInfo.ubQualityIndex = uhIndex;
                break;
            case PHOTO_USR_ISO:
                currentInfo.ubISOIndex = uhIndex;
                break;
            case PHOTO_USR_WB:
                currentInfo.ubWBIndex = uhIndex;
                break;
            //#NT#Store FD index after power off
            case PHOTO_USR_FD:
                currentInfo.ubFDIndex = uhIndex;
                break;
            /*
            case PHOTO_USR_SATURATION:
                currentInfo.ubSaturationIndex = uhIndex;
                break;
            case PHOTO_USR_SHARPNESS:
                currentInfo.ubSharpnessIndex = uhIndex;
                break;
            case PHOTO_USR_CONTRAST:
                currentInfo.ubContrastIndex = uhIndex;
                break;
            */
            case PHOTO_USR_DISPMODE:
                currentInfo.ubPhotoDispMode = uhIndex;
                break;
            default:
                UIInfoErrMsg("Photo_SetUserIndex: Unknown item %d\r\n", uiItem);
                break;
        }
    }
    else
    {
        /* While init setting flag is on, do NOT update Photo Mode user defined settings data */
        UIInfoTraceMsg("Photo_SetUserIndex: Init setting flag is ON, no update item %d\r\n", uiItem);
    }
}

UINT32 Photo_GetUserIndex(PHOTO_USR_ITEM_T uiItem)
{
    UINT32 uhUserIndex;

    switch(uiItem)
    {
        case PHOTO_USR_FLASH:
            uhUserIndex = currentInfo.ubFlashIndex;
            break;
        case PHOTO_USR_EV:
            uhUserIndex = currentInfo.ubEVIndex;
            break;
        case PHOTO_USR_AFWINDOW:
            uhUserIndex = currentInfo.ubAFWindowIndex;
            break;
        case PHOTO_USR_AFBEAM:
            uhUserIndex = currentInfo.ubAFBeamIndex;
            break;
        case PHOTO_USR_METERING:
            uhUserIndex = currentInfo.ubAEmode;
            break;
        case PHOTO_USR_TIMELAPSE:
            uhUserIndex = currentInfo.ubTimelapseIndex;
            break;
        case PHOTO_USR_CONTSHOT:
            uhUserIndex = currentInfo.ubContshotIndex;
            break;
        case PHOTO_USR_CONTAF:
            uhUserIndex = currentInfo.ubContAF;
            break;
        case PHOTO_USR_QREVIEW:
            uhUserIndex = currentInfo.ubQviewIndex;
            break;
        case PHOTO_USR_DZOOMSTATUS:
            uhUserIndex = currentInfo.ubDzoom;
            break;
        case PHOTO_USR_SLOWSHUTTER:
            uhUserIndex = currentInfo.ubSlowShutter;
            break;
        case PHOTO_USR_DATEIMPRINT:
            uhUserIndex = currentInfo.ubDatePrint;
            break;
        case PHOTO_USR_BLINK:
            uhUserIndex = currentInfo.ubBlinkDet;
            break;
        case PHOTO_USR_PANDET:
            uhUserIndex = currentInfo.ubPanModeIndex;
            break;
        case PHOTO_USR_COLOR:
            uhUserIndex = currentInfo.ubColorIndex;
            break;
        case PHOTO_USR_SIZE:
            uhUserIndex = currentInfo.ubSizeIndex;
            break;
        case PHOTO_USR_QUALITY:
            uhUserIndex = currentInfo.ubQualityIndex;
            break;
        case PHOTO_USR_ISO:
            uhUserIndex = currentInfo.ubISOIndex;
            break;
        case PHOTO_USR_WB:
            uhUserIndex = currentInfo.ubWBIndex;
            break;
        //#NT#Store FD index after power off
        case PHOTO_USR_FD:
            uhUserIndex = currentInfo.ubFDIndex;
            break;
        /*
        case PHOTO_USR_SATURATION:
            uhUserIndex = currentInfo.ubSaturationIndex;
            break;
        case PHOTO_USR_SHARPNESS:
            uhUserIndex = currentInfo.ubSharpnessIndex;
            break;
        case PHOTO_USR_CONTRAST:
            uhUserIndex = currentInfo.ubContrastIndex;
            break;
        */

        case PHOTO_USR_DISPMODE:
            uhUserIndex = currentInfo.ubPhotoDispMode;
            break;

        default:
            uhUserIndex = 0xFFFF;
            UIInfoErrMsg("Photo_GetUserIndex: Unknown item %d\r\n", uiItem);
            break;
    }
    UIInfoTraceMsg("Photo_GetUserIndex: Get item %d user index %d\r\n", uiItem, uhUserIndex);

    return uhUserIndex;
}

extern DX_HANDLE gDevTVObj;
extern DX_HANDLE gDevHDMIObj;

PLUG_DEV KeyScan_GetPlugDev(void)
{
    DX_HANDLE uiDevObj1,uiDevObj2;

    uiDevObj1 = (DX_HANDLE)GxVideo_GetDevice(DOUT1);
    uiDevObj2 = (DX_HANDLE)GxVideo_GetDevice(DOUT2);
    if (uiDevObj1 == gDevTVObj||uiDevObj2 == gDevTVObj)
    {
        return PLUG_TV;
    } else if (uiDevObj1 == gDevHDMIObj||uiDevObj2 == gDevHDMIObj)
    {
        return PLUG_HDMI;
    } else {
        return PLUG_OFF;
    }
}

//-----------------------------------------------------------------------------
// Update DCF Folder/File name Utility
//-----------------------------------------------------------------------------
#if (USE_FILEDB == DISABLE)
static BOOL FlowDCF_GetNextDCFFolderID(UINT32 *pDirID)
{
    BOOL ret = FALSE;
    UINT32 uiCurDCFID,uiDirCount;
    SDCFDIRINFO dirinfo;

    uiCurDCFID = DCF_GetDBInfo(DCF_INFO_CUR_DIR_ID);
    uiDirCount = 0;

    do {
        if (uiCurDCFID==MAX_DCF_DIR_NUM)
            uiCurDCFID = MIN_DCF_DIR_NUM;
        else
            uiCurDCFID++;

        uiDirCount++;
        if (!DCF_GetDirInfo(uiCurDCFID,&dirinfo))
        {
            *pDirID = uiCurDCFID;
            ret = TRUE;
            break;
        }
    } while (uiDirCount==(MAX_DCF_DIR_NUM-MIN_DCF_DIR_NUM));


    return ret;
}
#endif

void FlowDCF_UpdateName(void)
{
#if (USE_FILEDB == DISABLE)
    CHAR FolderName[6]="MEDIA";
    CHAR FileName[5]="PTDC";
    char DCFpathname[0x20];
    RTC_DATE  curDate;
    RTC_TIME  curTime;
    UINT32    uiAddr,uiNextDCFID;
    CHAR filePath[DCF_FULL_FILE_PATH_LEN];


    curDate = rtc_getDate();
    curTime = rtc_getTime();
    sprintf(FolderName,"%1d%02d%02d",curDate.s.year%0x0A,curDate.s.month,curDate.s.day);
    sprintf(FileName,"%02d%02d",curTime.s.hour,curTime.s.minute);

    uiAddr = DCF_GetDBInfo(DCF_INFO_DIR_FREE_CHARS);
    memcpy((UB*)DCFpathname,(UB*)uiAddr,DCF_DIR_NAME_LEN);
    if (strncmp(DCFpathname, FolderName, 5))
    {
         if (FlowDCF_GetNextDCFFolderID(&uiNextDCFID)==TRUE)
         {
             DCF_SetDirFreeChars(FolderName);
             DCF_MakeDirPath(uiNextDCFID,filePath);
             FileSys_MakeDir(filePath);
             DCF_SetNextID(uiNextDCFID,MIN_DCF_FILE_NUM);
         } else {
             debug_msg("DCF Folder full\r\n");
         }
    }

    // update DCF file name in current RTC time
    DCF_SetFileFreeChars(DCF_FILE_TYPE_ANYFORMAT, FileName);
#endif
}

