/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       SysMain_Flow_Sleep.c
    @ingroup    mSystemFlow

    @brief      Sleep Flow

    @note
                1.提供change sleep的介面
                  System_ChangeSleep()
                  (a)System_ChangeSleep()執行
                    各level不同的control condition
                    包含通知不同device sleep, wakeup
                  (b)System_ChangeSleep()執行
                    各level不同的control condition
                    包含通知不同mode sleep, wakeup: System_ModeSleepEnter(), System_ModeSleepLeave()

    @date       2012/1/1
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "UICommon.h"
#include "AppLib.h"
#include "GxInput.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysSleep
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

/////////////////////////////////////////////////////////////////////////////

#if (DET_LCDDIR_FUNCTION == ENABLE)
extern void Display_DetLCDDir(void);
#endif

INT32       g_iSysSleepLevel = 0;

UINT32      g_CurKeySoundMask[3] = {0};

void System_ChangeSleep(UINT32 sleep_lv)
{
    UI_LockEvent();

    DBG_FUNC_BEGIN("\r\n");
    DBG_DUMP("^MSLEEP Begin (Lvl: 0x%x) ...\r\n", sleep_lv);

    //enter sleep
    if(g_iSysSleepLevel < (INT32)sleep_lv)
    {
        if(g_iSysSleepLevel < (INT32)sleep_lv)
        {
            if(g_iSysSleepLevel == 0)
            {
                //LCD device
                GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BRIGHTLVL, 0);
                //save key sound mask
                g_CurKeySoundMask[0] = Input_GetKeySoundMask(KEY_PRESS);
                g_CurKeySoundMask[1] = Input_GetKeySoundMask(KEY_CONTINUE);
                g_CurKeySoundMask[2] = Input_GetKeySoundMask(KEY_RELEASE);
                //mute key sound
                Input_SetKeySoundMask(KEY_PRESS, FLGKEY_KEY_MASK_NULL);
                Input_SetKeySoundMask(KEY_CONTINUE, FLGKEY_KEY_MASK_NULL);
                Input_SetKeySoundMask(KEY_RELEASE, FLGKEY_KEY_MASK_NULL);
                //enter LIGHT level
                g_iSysSleepLevel = 1;
            }
        }
        if(g_iSysSleepLevel < (INT32)sleep_lv)
        {
            if(g_iSysSleepLevel == 1)
            {
                //enter NORMAL level
                ///Step3: [App] Pause with wait Finish
                ///Step4: [DispDrv] Stop
                AppView_Pause();
                GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_SLEEP, TRUE); //turn off IDE all layers
                System_ModeSleepEnter();
                /*
                //IPL enter sleep, IPL related function are stop (include AE,AWB).
                // PhotoTsk is also wait for IPL resume, and its related function are stop, too (include FD,SD,DIS,ASCN).
                //User only need  to take care about function waiting for IPL engine.
                // If this kind of function is existed, user need to suspend it before call IPLCtrl_Sleep().
                IPLCtrl_Sleep(TRUE);
                //under sleep, cannot do flash charge
                Flash_OnSystem(SYSTEM_CMD_SLEEPENTER);
                //under sleep, FW need to stop AE,AWB, sensor also need to enter power-save mode
                Sensor_OnSystem(SYSTEM_CMD_SLEEPENTER);
                //under sleep, FW need to stop AF,CAF, lens also need to enter power-save mode
                Lens_OnSystem(SYSTEM_CMD_SLEEPENTER);
                */
                //GxLED_SetCtrl(KEYSCAN_LED_RED,SETLED_SPEED,GXLED_1SEC_LED_TOGGLE_CNT);
                //GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,FALSE);
                //GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,TRUE);
                GxLED_SetCtrl(KEYSCAN_LED_GREEN,SETLED_SPEED,GXLED_1SEC_LED_TOGGLE_CNT);
                GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,FALSE);
                GxLED_SetCtrl(KEYSCAN_LED_GREEN,SET_TOGGLE_LED,TRUE);
                SxTimer_SetFuncActive(SX_TIMER_DET_SYSTEM_BUSY_ID,FALSE);
                g_iSysSleepLevel = 2;
            }
        }
        if(g_iSysSleepLevel < (INT32)sleep_lv)
        {
            if(g_iSysSleepLevel == 2)
            {
                //enter DEEP level
                //GxVideo_CloseDevice(DOUT1); //TODO - turn off IDE engine
                g_iSysSleepLevel = 3;
            }
        }
    }
    //leave sleep
    else if(g_iSysSleepLevel > (INT32)sleep_lv)
    {
        if(g_iSysSleepLevel > (INT32)sleep_lv)
        {
            if(g_iSysSleepLevel == 3)
            {
                //leave DEEP level
                //GxVideo_OpenDevice(DOUT1, NewDevObj, DISP_LASTMODE); //TODO - turn on IDE engine
                g_iSysSleepLevel = 2;
            }
        }
        if(g_iSysSleepLevel > (INT32)sleep_lv)
        {
            if(g_iSysSleepLevel == 2)
            {
                //leave NORMAL level
                System_ModeSleepLeave();
                /*
                Lens_OnSystem(SYSTEM_CMD_SLEEPLEAVE);
                Sensor_OnSystem(SYSTEM_CMD_SLEEPLEAVE);
                Flash_OnSystem(SYSTEM_CMD_SLEEPLEAVE);
                IPLCtrl_Wakeup(TRUE);
                */
                ///Step6: [DispDrv] Play
                ///Step7: [App] Resume with wait Finish (need trigger one flip)
                AppView_Resume();
                GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_SLEEP, FALSE); //turn on IDE all layers
                //GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,TRUE);
                //GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,FALSE);
                GxLED_SetCtrl(KEYSCAN_LED_GREEN,SET_TOGGLE_LED,FALSE);
                GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,TRUE);
                SxTimer_SetFuncActive(SX_TIMER_DET_SYSTEM_BUSY_ID,TRUE);
                g_iSysSleepLevel = 1;
            }
        }
        if(g_iSysSleepLevel > (INT32)sleep_lv)
        {
            if(g_iSysSleepLevel == 1)
            {
                //leave LIGHT level
                #if (DET_LCDDIR_FUNCTION == ENABLE)
                GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_FORCEDETDIR, 1);
                //config video Direct
                Display_DetLCDDir();
                #endif
                //LCD device
                GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BRIGHTLVL, UI_GetData(FL_LCD_BRIGHTNESS)); //get last user config level
                //restore key sound mask
                Input_SetKeySoundMask(KEY_PRESS, g_CurKeySoundMask[0]);
                Input_SetKeySoundMask(KEY_CONTINUE, g_CurKeySoundMask[1]);
                Input_SetKeySoundMask(KEY_RELEASE, g_CurKeySoundMask[2]);
                g_iSysSleepLevel = 0;
            }
        }
    }

    DBG_DUMP("^MSLEEP End (Lvl: 0x%x)\r\n", g_iSysSleepLevel);
    DBG_FUNC_END("\r\n");

    UI_UnlockEvent();
}
