/*
    System Main Callback

    System Callback for System Module.

    @file       SysMain_Exe.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "UICommon.h"
#include "AppLib.h"
#if (UI_STYLE==UI_STYLE_DEMO)
#include "DialogWarn.h"
#endif

#if (UI_STYLE==UI_STYLE_DRIVER)
#include "SoundData.h"
#include "IrRx.h"
#if (GPS_FUNCTION == ENABLE)
#include "GPS.h"
#endif
#endif
#include "DCF.h"
#include "UsbDevDef.h"
//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#include "GSensor.h"
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysMainExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

extern void GxSystem_BootStart(void);
extern void GxSystem_BootEnd(void);
extern void GxSystem_ShutDownStart(void);
extern void GxSystem_ShutDownEnd(void);

extern VControl SystemObjCtrl;
extern VControl UISetupObjCtrl;
extern VControl* gUxCtrlTypeList[];
extern VControl* gAppCtrlTypeList[];

void Install_AppObj(void)
{
    //Init object list for UIControl event process

    //register UI control type (from UIControl Lib)
    Ux_SetUITypeList(gUxCtrlTypeList, CTRL_EX_TYPE_MAX - CTRL_TYPE_MIN);

    //register App control type (from AppControl Lib)
    gAppCtrlTypeList[APP_BASE - APP_TYPE_MIN] = &SystemObjCtrl; //App base type = SystemObjCtrl
    gAppCtrlTypeList[APP_SETUP - APP_TYPE_MIN] = &UISetupObjCtrl; //App setup type = UISetupObjCtrl
    Ux_SetAppTypeList(gAppCtrlTypeList, APP_TYPE_MAX - APP_TYPE_MIN);

    //set default active app = current system object
    Ux_SetActiveApp(&UISetupObjCtrl);
}

///////////////////////////////////////////////////////////////////////
#include "GxTimer.h"

int SX_TIMER_DET_TIMER_ID = -1;
void Timer_CB(UINT32 event, UINT32 param1, UINT32 param2);

SX_TIMER_ITEM(System_CntTimer, GxTimer_CountTimer, 1, FALSE)

void System_OnTimerInit(void)
{
    //PHASE-1 : Init & Open Drv or DrvExt
    {
    GxTimer_RegCB(Timer_CB);         //Register CB function of GxTimer
    SX_TIMER_DET_TIMER_ID = SxTimer_AddItem(&Timer_System_CntTimer);
    //5.µù¥USxCmdªA°È ---------> Cmd Function
    //HwClock_InstallCmd();
    }
    {
    //PHASE-2 : Init & Open Lib or LibExt
    }
}

void System_OnTimerExit(void)
{
    //PHASE-2 : Close Lib or LibExt
    {
    }
    //PHASE-1 : Close Drv or DrvExt
    {
    }
}

///////////////////////////////////////////////////////////////////////
//System flow event

#include "GxPower.h"

extern UINT32 Input_GroupStatus2Event(UINT32 status);
extern BOOL g_bConsumeStatus;
extern BOOL gbIsPlayBackModePowerOn;


#if (POWERON_TESTFLOW == ENABLE)
BOOL System_Is_USBPC_DeadBattery(void)
{
    if(GxPower_GetControl(GXPWR_CTRL_IS_BATT_INSERT))
        return FALSE;
    /*
    if(!GxPower_GetControl(GXPWR_CTRL_IS_BATT_INSERT))
        return FALSE;
    if(!GxPower_GetControl(GXPWR_CTRL_IS_DEAD_BATT))
        return FALSE;
    */
    if(GxSystem_GetPowerOnSource() == GX_PWRON_SRC_PWR_VBUS)
    {
        if(GxUSB_GetIsUSBPlug())
        {
            GxUSB_UpdateConnectType();
            if(GxUSB_GetConnectType() == USB_CONNECT_PC)
            {
                debug_msg("^GUSBPC+DeadBatt\r\n");
                return TRUE;
            }
        }
    }
    return FALSE;
}
#endif

INT32 System_OnBoot(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(paramNum == 1 && paramArray[0] == 0) //power-on start
    {
        //GxSystem_BootStart();
    }
    if(paramNum == 1 && paramArray[0] == 1) //power-on end
    {
        UINT32 paramNum;
        UINT32 paramArray[MAX_MESSAGE_PARAM_NUM];
        #if (UI_STYLE==UI_STYLE_DRIVER)
        //#NT#2012/10/18#Philex Lin - begin
        UINT32 uiChangeTo;
        UINT32 uiFSStatus;
        //#NT#2012/10/18#Philex Lin - end
        #if (_IR_REMOTE_ != _IR_REMOTE_NONE_)
        IRRX_DEVICE_OBJ IrRxDevice;
        #endif
        #endif

        GxSystem_BootEnd();

        //FilesysWaitFinish(); //wait strg init finish
        UserWaitEvent(NVTEVT_STRG_ATTACH, &paramNum, paramArray); //(paramNum == 1 && paramArray[0] == 0) //power-on begin

        #if (UI_STYLE==UI_STYLE_DRIVER)
        uiFSStatus = UI_GetData(FL_FSStatus);
        #endif

        //wait disp init finish
        //wait audio init finish
        //wait usb init finish
        #if PST_FUNC
        Load_MenuInfo();
        Init_MenuInfo();

        //#NT#2014/5/15#Philex Lin - Begin
        // set FL_DualDisp flag first depending on project requirements
        #if (DUALDISP_FUNC==DUALDISP_OFF)
        SysSetFlag(FL_DualDisp,DUALDISP_OFF);
        #elif (DUALDISP_FUNC==DUALDISP_ONXOR)
        SysSetFlag(FL_DualDisp,DUALDISP_ONXOR);
        #else
        SysSetFlag(FL_DualDisp,DUALDISP_ONBOTH);
        #endif
        //#NT#2014/5/15#Philex Lin - End
        #endif
        SxTimer_SetFuncActive(SX_TIMER_DET_TIMER_ID,TRUE);
        BKG_SetExecFuncTable(gBackgroundExtFuncTable);

#if (POWERON_TESTFLOW == ENABLE)
        //init GxUSB module
        if(GxSystem_GetPowerOnSource() == GX_PWRON_SRC_PWR_VBUS)
        {
            //GxUSB_Init(TRUE);
        }
        else
        {
            GxUSB_Init(FALSE);
        }
#else
        //init GxUSB module
        if(GxSystem_GetPowerOnSource() == GX_PWRON_SRC_PWR_VBUS)
        {
            GxUSB_Init(TRUE);
        }
        else
        {
            GxUSB_Init(FALSE);
        }
#endif
        //get first app
        #if (_IR_REMOTE_ != _IR_REMOTE_NONE_)
        // Open IrRx driver
        irrx_open((PIRRX_DEVICE_OBJ)&IrRxDevice);
        #endif
        //#NT#2012/10/23#Philex Lin-begin
        // init Volume to max,100
        GxSound_SetVolume(100);
        // add power up sound
        UISound_Play(DEMOSOUND_SOUND_POWERON_TONE);
        //#NT#2012/10/23#Philex Lin-end

        UI_SetFileDBFileID();

        //#NT#2012/10/29#Philex Lin-begin
        // update card status again
        if (GxStrg_GetDeviceCtrl(0, CARD_READONLY))
        {
            UI_SetData(FL_CardStatus, CARD_LOCKED);
        }
        else if (GxStrg_GetDeviceCtrl(0, CARD_INSERT))
        {
            UI_SetData(FL_CardStatus, CARD_INSERTED);
        }
        else
        {
            UI_SetData(FL_CardStatus, CARD_REMOVED);
        }
        //#NT#2012/10/29#Philex Lin-end
        UI_SetData(FL_FSStatus,uiFSStatus);
        //#NT#2013/1/6#Philex Lin-begin
        #if (GPS_FUNCTION == ENABLE)
        GPSRec_Open(UART_BAUDRATE_9600);
        #endif
        //#NT#2013/1/6#Philex Lin-end
        #if 0
        // Init LED_
        GxLED_SetCtrl(KEYSCAN_LED_GREEN,SETLED_SPEED,GXLED_1SEC_LED_TOGGLE_CNT/2);
        GxLED_SetCtrl(KEYSCAN_LED_RED,SETLED_SPEED,GXLED_1SEC_LED_TOGGLE_CNT/5);
        GxLED_SetCtrl(KEYSCAN_LED_FCS,SETLED_SPEED,GXLED_1SEC_LED_TOGGLE_CNT/10);
        SxTimer_SetFuncActive(SX_TIMER_DET_LED_ID,TRUE);
        GxLED_SetCtrl(KEYSCAN_LED_FCS,SET_TOGGLE_LED,FALSE);
        GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,FALSE);
        GxLED_SetCtrl(KEYSCAN_LED_GREEN,SET_TOGGLE_LED,TRUE);
        #endif
        //open first app mode
        uiChangeTo = PRIMARY_MODE_MOVIE;
        //Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MAIN);
        //Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PLAYBACK);
#if(LOW_POWER_TIMELAPSE_REC == ENABLE)
        //if(UI_GetData(FL_LOW_POWER_TIMELAPSE_REC)&& rtc_isPWRAlarm())
        if(rtc_isPWRAlarm())
        {
            DBG_DUMP("Power On RTC and Change to Photo Mode\r\n");
            uiChangeTo = PRIMARY_MODE_PHOTO;
        }
        //uiChangeTo = PRIMARY_MODE_PLAYBACK;
#endif
        Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, uiChangeTo);
        //Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
        UI_SetData(FL_NextMode,uiChangeTo);
    }


    return NVTEVT_CONSUME;
}

extern void UI_Quit(void);
BOOL bUSBPreStd = FALSE;

INT32 System_OnShutdown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    bUSBPreStd = GxUSB_GetIsUSBPlug();
	
    if(paramNum == 1 && paramArray[0] == 0) //power-off begin
    {
    	debug_msg("^Ydrh System_OnShutdown1\r\n");
    	#if 1		
		System_OnStrgInit_PS();
    	DCT_FlowMovie_SetParkWakeUp();
        Save_MenuInfo();
		Delay_DelayMs(100);
		#endif
		debug_msg("^Ydrh System_OnShutdown2\r\n");
        GxSystem_ShutDownStart();

        #if (UI_STYLE==UI_STYLE_DRIVER)
        //#NT#2012/10/23#Philex Lin-begin
        // add power off sound. play power off sound before UI_LockEvent() at the end of this function.
        //UISound_Play(DEMOSOUND_SOUND_POWERON_TONE);
        //#NT#2012/10/23#Philex Lin-end
        // save RTC date data and file ID
        UI_SaveRTCDate();
        #endif

        Display_SetEnable(LAYER_VDO1, FALSE); //turn off VDO1 to avoid garbage frame.

        //debug_msg("^Gq0\r\n");
        //close last app mode
        Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, SYS_MODE_UNKNOWN);
        //debug_msg("^Gq1\r\n");

        //#NT#2013/01/23#Lincy Lin -begin
        //#NT#Add low battery, lens error power off tone and screen
        if(System_GetState(SYS_STATE_POWEROFF) == SYS_POWEROFF_BATTEMPTY)
        {
            DBG_ERR("low battery **********\r\n");
            #if(_USR_WND_==ENABLE)
            #if (UI_STYLE==UI_STYLE_DEMO)
            if(!gIsUSBChargePreCheck)
            {
                UISound_Play(PLAYSOUND_SOUND_BATT_WARNING_TONE);
                SwTimer_DelayMs(1000);
                Ux_OpenWindow(&DialogWarnCtrl, 2, WARN_BATT_EMPTY, WARN_ALWAYS);
                Ux_Redraw();
                SwTimer_DelayMs(2000);
            }
            #endif
            #endif
        }
        else if(System_GetState(SYS_STATE_POWEROFF) == SYS_POWEROFF_LENSERROR)
        {
            #if(_USR_WND_==ENABLE)
            #if (UI_STYLE==UI_STYLE_DEMO)
            Ux_OpenWindow(&DialogWarnCtrl, 2, WARN_LENS_ERROR, WARN_ALWAYS);
            Ux_Redraw();
            GxLED_SetCtrl(KEYSCAN_LED_RED,TURNON_LED,TRUE);
            UISound_Play(PLAYSOUND_SOUND_WARNING_TONE);
            SwTimer_DelayMs(1500);
            UISound_Play(PLAYSOUND_SOUND_WARNING_TONE);
            SwTimer_DelayMs(1500);
            GxLED_SetCtrl(KEYSCAN_LED_RED,TURNON_LED,FALSE);
            #endif
            #endif
        }
        //#NT#2013/01/23#Lincy Lin -end
        else
        {
            #if (POWEROFFLOGO_FUNCTION == ENABLE)
            GxDisplay_Set(LAYER_OSD1, LAYER_STATE_ENABLE, 0); //turn off OSD1
            GxDisplay_Set(LAYER_OSD2, LAYER_STATE_ENABLE, 0); //turn off OSD2
            GxDisplay_Flush(LAYER_OSD1);
            GxDisplay_Flush(LAYER_OSD2);
            GxDisplay_Set(LAYER_VDO1, LAYER_STATE_ENABLE, 0); //turn off VDO1
            GxDisplay_Flush(LAYER_VDO1);
            Display_ShowSplash(SPLASH_POWEROFF);
            SwTimer_DelayMs(500);
            #endif
        }

        //Save_MenuInfo();
        //debug_msg("^Gq2\r\n");
        #if (UI_STYLE==UI_STYLE_DEMO)
        //debug_msg("^Gq3\r\n");
            #if (POWEROFFSOUND_FUNCTION ==  ENABLE)
            {
                UISound_Play(PLAYSOUND_SOUND_POWEROFF_TONE);
            }
            #endif
        #endif

        #if (UI_STYLE==UI_STYLE_DRIVER)
        // add power off sound
        UISound_EnableKey(TRUE);
        UISound_Play(DEMOSOUND_SOUND_POWERON_TONE);
        #endif

        #if (GSENSOR_FUNCTION == ENABLE)
        GSensor_I2C_Lock();
        #endif

        //debug_msg("^Gq4\r\n");
        UI_LockEvent();

        UI_Quit(); //quit from event loop
        //debug_msg("^Gq5\r\n");
    }
    if(paramNum == 1 && paramArray[0] == 1) //power-off end
    {
        //GxSystem_ShutDownEnd();
        //bQuit = TRUE;
    }
    return NVTEVT_CONSUME;
}

INT32 System_OnMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    int mode_id;
    if(paramNum < 1)
        return NVTEVT_CONSUME;

    mode_id = paramArray[0];
    if(paramNum ==2)
    {
        DBGD(paramArray[1]);
        System_SetState(SYS_STATE_NEXTSUBMODE,paramArray[1]);
    }
    System_ChangeMode(mode_id);

    return NVTEVT_CONSUME;
}

INT32 System_OnSleepLevel(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    int sleep_lv;
    if(paramNum < 1)
        return NVTEVT_CONSUME;

    sleep_lv = paramArray[0];
    System_ChangeSleep(sleep_lv);

    return NVTEVT_CONSUME;
}

INT32 System_OnAppOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //call USER_MODE close
    //  1. Ux_CloseWindow();
    //  2. Ux_SendEvent(0,NVTEVT_EXE_CLOSE,paramNum,paramArray);
    //         System_ChangeMode(NULL, FALSE); //close mode
    //  3. Ux_SetActiveApp(0);
    //  4. wait mode close finish
    //         DevCtrl();

    return NVTEVT_CONSUME;
}

INT32 System_OnAppClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //call USER_MODE open
    //  1. DevCtrl();
    //  2. Ux_SetActiveApp(&AppObj);
    //  3. Ux_SendEvent(0,NVTEVT_EXE_OPEN,paramNum,paramArray);
    //         System_ChangeMode(mode, FALSE); //open mode
    //             wait device control finish
    //  4. Ux_OpenWindow();
    //         wait mode open finish

    return NVTEVT_CONSUME;
}

void Timer_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
    Ux_PostEvent(NVTEVT_TIMER, 1, event);
}

///////////////////////////////////////////////////////////////////////////////
//Device flow event
INT32 System_OnVideoInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnVideoRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnVideoAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnVideoDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnAudioInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnAudioRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnAudioAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnAudioDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnStrgInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnStrgRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnStrgAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnStrgDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnUsbInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnUsbRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnUsbChargeCurrent(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnBattEmpty(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnBattChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnPwrChargeOK(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnPwrChargeSuspend(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnPwrChargeResume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

INT32 System_OnVideoDir(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnVideoMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

/*
INT32 System_OnDefAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //  PWR-KEY:
    //  NORMAL-KEY:
    //  STATUS-KEY:
    //  TOUCH:
    //  LED:
    //  BATTERY:
    return NVTEVT_CONSUME;
}

INT32 System_OnSensorAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //  SENSOR
    return NVTEVT_CONSUME;
}

INT32 System_OnLensAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //  LENS
    return NVTEVT_CONSUME;
}
*/

EVENT_ENTRY SystemObjCmdMap[] =
{
////////////////////////////////////////////////////////////

    //System flow event
    {NVTEVT_SYSTEM_BOOT,             System_OnBoot              },
    {NVTEVT_SYSTEM_SHUTDOWN,         System_OnShutdown          },
    {NVTEVT_SYSTEM_MODE,             System_OnMode              },
    {NVTEVT_SYSTEM_SLEEP,            System_OnSleepLevel        },
    //App common event
    {NVTEVT_EXE_OPEN,                System_OnAppOpen           },
    {NVTEVT_EXE_CLOSE,               System_OnAppClose          },

    //Video device event
    {NVTEVT_VIDEO_INSERT,            System_OnVideoInsert       },
    {NVTEVT_VIDEO_REMOVE,            System_OnVideoRemove       },
    {NVTEVT_VIDEO_ATTACH,            System_OnVideoAttach       },
    {NVTEVT_VIDEO_DETACH,            System_OnVideoDetach       },
    {NVTEVT_VIDEO_MODE,              System_OnVideoMode         },
    {NVTEVT_VIDEO_DIR,               System_OnVideoDir          },
    //Audio device event
    {NVTEVT_AUDIO_INSERT,            System_OnAudioInsert       },
    {NVTEVT_AUDIO_REMOVE,            System_OnAudioRemove       },
    {NVTEVT_AUDIO_ATTACH,            System_OnAudioAttach       },
    {NVTEVT_AUDIO_DETACH,            System_OnAudioDetach       },
    //Storage device event
    {NVTEVT_STRG_INSERT,             System_OnStrgInsert        },
    {NVTEVT_STRG_REMOVE,             System_OnStrgRemove        },
    {NVTEVT_STRG_ATTACH,             System_OnStrgAttach        },
    {NVTEVT_STRG_DETACH,             System_OnStrgDetach        },
    //Usb device event
    {NVTEVT_USB_INSERT,              System_OnUsbInsert         },
    {NVTEVT_USB_REMOVE,              System_OnUsbRemove         },
    {NVTEVT_USB_CHARGE_CURRENT,      System_OnUsbChargeCurrent  },
    //power device event
    {NVTEVT_PWR_BATTEMPTY,           System_OnBattEmpty         },
    {NVTEVT_PWR_BATTCHANGE,          System_OnBattChange        },
    {NVTEVT_PWR_CHARGE_OK,           System_OnPwrChargeOK       },
    {NVTEVT_PWR_CHARGE_SUSPEND,      System_OnPwrChargeSuspend  },
    {NVTEVT_PWR_CHARGE_RESUME,       System_OnPwrChargeResume   },

    //Other device event
    //{NVTEVT_DEFAULT_ATTACH,        System_OnDefAttach         },
    //{NVTEVT_SENSOR_ATTACH,         System_OnSensorAttach      },
    //{NVTEVT_LENS_ATTACH,           System_OnLensAttach        },

    {NVTEVT_NULL,                    0},  //End of Command Map
};

CREATE_APP(SystemObj, 0)

