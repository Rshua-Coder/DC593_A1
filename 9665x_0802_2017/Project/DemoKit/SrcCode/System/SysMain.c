////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "AppLib.h"

#include "FileSysTsk.h"
//#include "UIInfo.h"

//Mode
#include "UIModeMain.h"
#include "UIModePhoto.h"
#include "UIModeMovie.h"
#include "UIModePlayback.h"
#include "UIModeUsbDisk.h"
#include "UIModeUsbPrint.h"
#include "UIModeUsbPTP.h"
#include "UIModeUsbCam.h"
#include "UIModeUsbMenu.h"
#if (UI_STYLE==UI_STYLE_DRIVER)
#include "usb.h"
#include "audio.h"
#endif
#include "DeviceCtrl.h"
#include "DxSensor.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysMain
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


//BOOL g_bPreviewFirstTime = TRUE; //fast path
BOOL g_bPreviewFirstTime = FALSE; //normal path
BOOL g_bIsInitSystemFinish = TRUE;
BOOL bFirstStable = TRUE;

//int SX_TIMER_DET_FLASH_ID = -1;
int SX_TIMER_BURNIN_AUTORUN_ID = -1;
//int SX_TIMER_DET_RECHARGE_ID = -1;
//int SX_TIMER_DET_CLOSE_LENS_ID = -1;


//////////////////////////////////////
void DevCtrl_ModeUSBPCC(void)
{
    DrvSensor_TurnOffPower();
    SwTimer_DelayMs(200);
    DrvSensor_TurnOnPower();
    DevCtrl_Lens(TRUE);
}
void DevCtrl_ModeUSBPCC_Remove(void)
{
    DrvSensor_TurnOffPower();
}
//////////////////////////////////////
void DevCtrl_ModeUSBMSDC(void)
{
    DrvSensor_TurnOffPower();
    DevCtrl_Lens(FALSE);
}
//////////////////////////////////////
void DevCtrl_ModeUSBSIDC(void)
{
    DrvSensor_TurnOffPower();
    DevCtrl_Lens(FALSE);
}
//////////////////////////////////////
void DevCtrl_ModeUSBMenu(void)
{
}

//////////////////////////////////////

extern void GxSystem_BootStart(void);
extern void GxSystem_BootEnd(void);
extern void GxSystem_ShutDownStart(void);
extern void GxSystem_ShutDownEnd(void);

void UserWaitEvent(NVTEVT wait_evt, UINT32 *wait_paramNum, UINT32 *wait_paramArray)
{
    NVTEVT evt;
    UINT32 paramNum;
    UINT32 paramArray[MAX_MESSAGE_PARAM_NUM];
    BOOL bQuit = FALSE;

    while(!bQuit)
    {
        Ux_WaitEvent(&evt, &paramNum, paramArray);
        if(evt)
        {
            INT32 result = NVTEVT_PASS;
            DBG_IND("^YMSG: get event %08x!\r\n", evt);

            if(evt == wait_evt)
            {
                (*wait_paramNum) = paramNum;
                memcpy(wait_paramArray, paramArray, sizeof(UINT32)*MAX_MESSAGE_PARAM_NUM);
                result = NVTEVT_CONSUME;
                bQuit = TRUE;
            }
            if(IS_APP_EVENT(evt))
            {
                result = Ux_AppDispatchMessage(evt, paramNum, paramArray);
            }
            {
            VControl* pObj;
            if(Ux_GetRootWindow(&pObj) != NVTRET_OK)
                continue;
            }
            if(result != NVTEVT_CONSUME)
            {
                result = Ux_WndDispatchMessage(evt, paramNum, paramArray);
            }
            Ux_Redraw();
        }
    }
}

extern void Install_AppObj(void);
extern void System_OnTimerInit(void);
extern void System_OnStrgInit_INMEM(void);
extern void System_OnStrgInit_EXMEM(void);
extern void System_OnStrgInit_FWS(void);
extern void System_OnStrgInit_FS(void);
extern void System_OnStrgInit_PS(void);
extern void System_OnVideoInit(void);
extern void System_OnAudioInit(void);
extern void System_OnUsbInit(void);
extern void System_OnLensInit(void);
extern void System_OnSensorInit(void);
extern void System_OnPowerInit(void);
extern void System_OnInputInit(void);
extern void System_OnOutputInit(void);

extern void System_OnTimerExit(void);
extern void System_OnStrgExit_INMEM(void);
extern void System_OnStrgExit_EXMEM(void);
extern void System_OnStrgExit_FWS(void);
extern void System_OnStrgExit_FS(void);
extern void System_OnStrgExit_PS(void);
extern void System_OnVideoExit(void);
extern void System_OnAudioExit(void);
extern void System_OnUsbExit(void);
extern void System_OnLensExit(void);
extern void System_OnSensorExit(void);
extern void System_OnPowerExit(void);
extern void System_OnInputExit(void);
extern void System_OnOutputExit(void);

extern UINT32 UserSection_Order_full[];
extern void UserSection_LoadCb(const UINT32 Idx);

extern void System_OnStrg_DownloadFW(UINT32* SecOrderTable, void (*LoadCallback)(const UINT32 Idx));


BOOL bUI_Quit = FALSE;
void UI_Quit(void){bUI_Quit = TRUE;}

/////////////////////////////////////////////////////////////////////////////

void System_InstallMode(void)
{
    //register project mode
    PRIMARY_MODE_MAIN = System_AddMode(&gModeMain);
    PRIMARY_MODE_PLAYBACK = System_AddMode(&gModePlay);
    PRIMARY_MODE_PHOTO = System_AddMode(&gModePhoto);
    PRIMARY_MODE_MOVIE = System_AddMode(&gModeMovie);
    PRIMARY_MODE_USBMSDC = System_AddMode(&gModeUsbDisk);
    PRIMARY_MODE_USBMENU = System_AddMode(&gModeUsbMenu);
    PRIMARY_MODE_USBPCC = System_AddMode(&gModeUsbCam);
}

/////////////////////////////////////////////////////////////////////////////
INT32 System_UserKeyFilter(NVTEVT evt, UINT32 paramNum, UINT32 *paramArray);
INT32 System_UserTouchFilter(NVTEVT evt, UINT32 paramNum, UINT32 *paramArray);

BOOL gIsUSBChargePreCheck = FALSE;

#if (POWERON_TESTFLOW == ENABLE)
void System_OnStrgInit_p1(void);
//1.senser power
#endif

extern void System_OnPowerPreInit(void);

//#NT#2012/09/13#Philex Lin -begin
#if (UI_STYLE==UI_STYLE_DRIVER)
#if (LONG_STARTUP == ENABLE)
#include "Timer.h"
#define RESET_SHUTDOWN_TIMER_INTERVAL   100 //100ms
#define RESET_SHUTDOWN_TIMER_COUNT       20
static volatile UINT32  guiResetShutDownTimerID;
static volatile UINT32  guiResetSDTimerCount = 0;
void SysInit_ResetShutDownTimerHdl(UINT32 uiEvent)
{
   // checking whether power butter pressing or not
   if (rtc_getPWRStatus()==FALSE)
   {
      rtc_poweroffPWR();
      while (1)
      {
          debug_err(("*"));
      }
   } else {
       if (guiResetSDTimerCount > RESET_SHUTDOWN_TIMER_COUNT)
       {
         // over 2 sec
         timer_close(guiResetShutDownTimerID);
       } else {
         guiResetSDTimerCount++;
         // reset shutdown timer once
         rtc_resetShutdownTimer();
       }
   }
}

void SysInit_ResetShutDownTimer(void)
{
  // reset shutdown timer first
  rtc_resetShutdownTimer();

  if (timer_open((UINT *)&guiResetShutDownTimerID, SysInit_ResetShutDownTimerHdl) != E_OK)
     return;

  timer_cfg(guiResetShutDownTimerID, RESET_SHUTDOWN_TIMER_INTERVAL*1000, TIMER_MODE_FREE_RUN | TIMER_MODE_ENABLE_INT, TIMER_STATE_PLAY);
}
#endif
#endif
//#NT#2012/09/13#Philex Lin -end

void UserMainProc(void)
{
    NVTEVT evt;
    UINT32 paramNum;
    UINT32 paramArray[MAX_MESSAGE_PARAM_NUM];
    debug_msg("event loop - begin!\r\n");

#if (POWERON_TESTFLOW == ENABLE)
    //System_OnPowerPreInit();
    System_InstallMode();
    System_OnTimerInit();
    System_OnOutputInit();
    GxSystem_BootStart();
    if(GxSystem_GetPowerOnSource() == GX_PWRON_SRC_PWR_VBUS)
    {
        GxUSB_Init(TRUE);
    }
    if(gIsUSBChargePreCheck)
    {
        UINT32 uiChargeCurrent = USB_CHARGE_CURRENT_2P5MA;
        System_OnUsbInit();
        System_OnStrgInit_INMEM();
        System_OnStrgInit_EXMEM();
        Install_AppObj(); //must install before any event dispatch
        UI_UnlockEvent();
        debug_msg("wait USB - begin\r\n");
        do
        {
            UserWaitEvent(NVTEVT_USB_CHARGE_CURRENT, &paramNum, paramArray); //(paramNum == 1 && paramArray[0] == 0) //power-on begin
            uiChargeCurrent = paramArray[0];
            debug_msg("USB charging current(%d)\r\n",paramArray[0]);
        }
        while(uiChargeCurrent < USB_CHARGE_CURRENT_500MA);
        debug_msg("wait USB - end\r\n");
        //SxCmd_DoCommand("dma eclk");
        GxLED_SetCtrl(KEYSCAN_LED_FCS,TURNON_LED,TRUE);
    }
    //UserWaitEvent(NVTEVT_SYSTEM_BOOT, &paramNum, paramArray); //(paramNum == 1 && paramArray[0] == 0) //power-on begin

    //////////////////////////////////////////////////////////////
    // Direct boot flow

    //Init Device & Lib
    SysDbgUtCmd_Init(); //Init Once DbgUt
    System_OnVideoInit();
    System_OnAudioInit();
    if(!gIsUSBChargePreCheck)
    {
    System_OnStrgInit_INMEM();
    System_OnStrgInit_EXMEM();
    System_OnStrgInit_FWS();
    System_OnStrgInit_PS();
    System_OnStrgInit_FS();
    System_OnUsbInit();
    }
    System_OnLensInit();
    System_OnSensorInit();
    System_OnPowerInit();
    System_OnInputInit();

    //////////////////////////////////////////////////////////////

    //Init object list for UIControl event process
    if(!gIsUSBChargePreCheck)
        Install_AppObj(); //must install before any event dispatch

    //UserWaitEvent(NVTEVT_SYSTEM_BOOT, &paramNum, paramArray); //(paramNum == 1 && paramArray[0] == 0) //power-on begin
    Ux_SendEvent(0, NVTEVT_SYSTEM_BOOT, 1, 1);

#else

    usb_setConfig(USB_CONFIG_ID_STANDARD_CHARGER, FALSE);

    System_OnStrgInit_INMEM();
    System_OnStrgInit_FWS();
    System_OnStrg_DownloadFW(UserSection_Order_full, UserSection_LoadCb);
    System_OnPowerPreInit();
    System_InstallMode();
    System_OnTimerInit();
    System_OnOutputInit();
    GxSystem_BootStart();
    //UserWaitEvent(NVTEVT_SYSTEM_BOOT, &paramNum, paramArray); //(paramNum == 1 && paramArray[0] == 0) //power-on begin

    //////////////////////////////////////////////////////////////
    // Direct boot flow

    //Init Device & Lib
    //#NT#2013/2/20#Philex Lin - begin
    // for pressing longer to power up system
    #if (UI_STYLE==UI_STYLE_DRIVER)
      #if (LONG_STARTUP == ENABLE)
       SysInit_ResetShutDownTimer();
       while (guiResetSDTimerCount < RESET_SHUTDOWN_TIMER_COUNT)
        ;
       System_OnVideoInit();
      #else
       System_OnVideoInit();
      #endif
    #else
    System_OnVideoInit();
    #endif
    //#NT#2013/2/20#Philex Lin - end
    System_OnAudioInit();
    System_OnStrgInit_EXMEM();
    System_OnStrgInit_PS();
    System_OnStrgInit_FS();
    System_OnUsbInit();

    #if 1//(LENS_FUNCTION == ENABLE)
    System_OnLensInit();
    #endif
    System_OnSensorInit();
    System_OnPowerInit();
    System_OnInputInit();

    //////////////////////////////////////////////////////////////

    //Init object list for UIControl event process
    Install_AppObj(); //must install before any event dispatch

    #if (UI_STYLE==UI_STYLE_DRIVER)
        aud_setTotalVolLevel(AUDIO_VOL_LEVEL8);
        #if (UPDATE_CFG == UPDATE_CFG_YES)
        // Do UI_LoadCfgFile() function
        UI_LoadCfgFile();
        #endif
    #endif
    //UserWaitEvent(NVTEVT_SYSTEM_BOOT, &paramNum, paramArray); //(paramNum == 1 && paramArray[0] == 0) //power-on begin
    Ux_SendEvent(0, NVTEVT_SYSTEM_BOOT, 1, 1);
#endif

    while(!bUI_Quit)
    {
        Ux_WaitEvent(&evt, &paramNum, paramArray);
        if(evt)
        {
            INT32 result = NVTEVT_PASS;
            DBG_IND("^YMSG: get event %08x!\r\n", evt);

            if(IS_KEY_EVENT(evt))
                result = System_UserKeyFilter(evt, paramNum, paramArray);
            #if (TOUCH_FUNCTION == ENABLE)
            if(result != NVTEVT_CONSUME)
            {
                if(IS_TOUCH_EVENT(evt))
                    result = System_UserTouchFilter(evt, paramNum, paramArray);
            }
            #endif
            if(result != NVTEVT_CONSUME)
            {
                if(IS_APP_EVENT(evt))
                    result = Ux_AppDispatchMessage(evt, paramNum, paramArray);
            }
            if(result != NVTEVT_CONSUME)
                result = Ux_WndDispatchMessage(evt, paramNum, paramArray);

            Ux_Redraw();

			//#NT#2017/03/01#KCHong -begin
			//#NT#Avoid system hang-up when Ist losing event. Check Ist losing event here!
			extern void dev_checkIstLosingEvent(void);
			dev_checkIstLosingEvent();
			//#NT#2017/03/01#KCHong -end

#if 0 //background task wait done flag,need to remove
            // A special case.
            // UI background is busy until finishing the event handler of BACKGROUND_DONE
            if (evt == NVTEVT_BACKGROUND_DONE)
            {
                BKG_Done();
            }
#endif
            //return result;
        }
    }

    //////////////////////////////////////////////////////////////
    //System_PowerOff(SYS_POWEROFF_NORMAL);

    //Exit Lib & Device
    System_OnVideoExit();
    System_OnAudioExit();
    System_OnStrgExit_FS();
    System_OnStrgExit_PS();
    System_OnStrgExit_FWS();
    System_OnStrgExit_EXMEM();
    System_OnStrgExit_INMEM();
    System_OnUsbExit();
    System_OnLensExit();
    System_OnSensorExit();
    System_OnPowerExit();
    System_OnInputExit();
    System_OnOutputExit();
    System_OnTimerExit();

    //////////////////////////////////////////////////////////////
    GxSystem_ShutDownEnd();
    GxSystem_PowerOff();
    debug_msg("event loop - end!\r\n");
}



