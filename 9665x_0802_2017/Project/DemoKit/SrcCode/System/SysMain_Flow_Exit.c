/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       SysMain_Flow_Exit.c
    @ingroup    mSystemFlow

    @brief      PowerOff Flow

    @note
                1.提供power off的介面
                  System_PowerOff()
                  (a)System_PowerOff()執行
                    各mode不同的control condition
                    包含通知不同device exit
                  (b)System_PowerOff()執行
                    各mode不同的control condition
                    包含控制不同device之間相互dependent的order

    @date       2012/1/1
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
#include "GxSystem.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysExit
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

/////////////////////////////////////////////////////////////////////////////
#include "DevMan.h"
//#include "NvtSystem.h"
#include "Utility.h"

/////////////////////////////////////////////////////////////////////////////

INT32 g_iSysPowerOffMode = SYS_POWEROFF_NORMAL;

void System_PowerOff(UINT32 pwrOffType)
{
    g_iSysPowerOffMode = pwrOffType;
    Ux_SendEvent(0, NVTEVT_SYSTEM_SHUTDOWN, 1, 0); //shutdown start
}

void _System_PowerOff(void)
{
    //register int i;
    DBG_FUNC_BEGIN("\r\n");

#if _MIPS_TODO
    //when flow is very slow after case POWERON_CB_END: g_bIsInitSystemFinish = TRUE;
    //press power off key will cause system hang
    //=> should wait for power on finish before power off
    if((pwrOffType == SYS_POWEROFF_NORMAL))
    {
        ID currTid = 0;
        get_tid(&currTid);
        if (INITTSK_ID != currTid)
        {
            InitMain_WaitFinish();
        }
    }
#endif


#if 0
    ///////////////////////////////////////////
    if(System_GetState(SYS_STATE_POWEROFF) == SYS_POWEROFF_NORMAL)
    {
        //"normal power-off sequence"
        DBG_MSG("Power Off Sequence = Normal\r\n");

        //shut down FW subsystems
            DevMan[SYS]->CmdAsync(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[CARD]->CmdAsync(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[SENSOR]->CmdAsync(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[LENS]->CmdAsync(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[CARD]->WaitSync(DEVMAN_SYNC_EXIT_POSTCLOSE);//wait until Card finish
            DevMan[NAND]->CmdAsync(DEVMAN_CMD_EXIT, 0, 0);

        //shut down HW devices
            DevMan[AUDIO]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[USB]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[DISP]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
    }
    else if(System_GetState(SYS_STATE_POWEROFF) == SYS_POWEROFF_SAFE)
    {
        //"safe power-off sequence"
        DBG_MSG("Power Off Sequence = Safe\r\n");

        //shut down FW subsystems
            DevMan[SYS]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[CARD]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[NAND]->CmdAsync(DEVMAN_CMD_EXIT, 0, 0);

        //shut down HW devices
            DevMan[AUDIO]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[USB]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[SENSOR]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[LENS]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[DISP]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
    }
    else if(System_GetState(SYS_STATE_POWEROFF) == SYS_POWEROFF_LOWPOWER)
    {
        //"lowpower power-off sequence"
        DBG_MSG("Power Off Sequence = LowPower\r\n");

        //shut down FW subsystems
            DevMan[SYS]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            //DevMan[CARD]->SetSync(DEVMAN_CMD_EXIT, 0, 0);

        //shut down HW devices
            DevMan[AUDIO]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            //DevMan[USB]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            //DevMan[SENSOR]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            //DevMan[LENS]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[DISP]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
    }
    else if(System_GetState(SYS_STATE_POWEROFF) == SYS_POWEROFF_CHARGE)
    {
        //"charge power-off sequence"
        DBG_MSG("Power Off Sequence = Charge\r\n");

        //shut down FW subsystems
            DevMan[SYS]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            //DevMan[CARD]->SetSync(DEVMAN_CMD_EXIT, 0, 0);

        //shut down HW devices
            DevMan[AUDIO]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[USB]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            //DevMan[SENSOR]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            //DevMan[LENS]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
            DevMan[DISP]->Cmd(DEVMAN_CMD_EXIT, 0, 0);
    }

    ///////////////////////////////////////////
    //wait until all device manager exit finish
    for(i=0;i<DEVICE_ID_MAX;i++)
        DevMan[i]->WaitFinish(DEVMAN_CMD_EXIT);

    ///////////////////////////////////////////
    //close all device manager
    for(i=0;i<DEVICE_ID_MAX;i++)
        DevMan[i]->Close();
#endif
    DBG_FUNC_END("\r\n");
}

////////////////////////////////////////////////////////////////////////////////
#include "DxSys.h"
#include "DrvExt.h" //Dx_OnCommand, Dx_ConfigIO()
#include "Utility.h"
#if(WIFI_AP_FUNC==ENABLE)
#include "WifiAppCmd.h"
#endif

// ShutDown Start
void GxSystem_ShutDownStart(void)
{
    //PowerOff_CB(POWEROFF_CB_START);
    DBG_IND("POWEROFF_CB_START\r\n");
#if(WIFI_AP_FUNC==ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
        WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_POWER_OFF);
#endif

}

// ShutDown End
void GxSystem_ShutDownEnd(void)
{
    // Power off
    //PowerOff_CB(POWEROFF_CB_END);
    BOOL bUSBCntStd = FALSE;
    extern BOOL bUSBPreStd;
    DBG_DUMP("^MSystem Shutdown end\r\n");
    DBG_DUMP("\r\n");
    bUSBCntStd = GxUSB_GetIsUSBPlug();
    //DBGD(bUSBPreStd);
    //DBGD(bUSBCntStd);
    if(bUSBCntStd&&(!bUSBPreStd))
    {
       GxSystem_Reboot();
    }
    DBG_IND("POWEROFF_CB_END\r\n");

    #if 0
    //Delay SxCmd close until here
    //Cmd_OnSystem(SYSTEM_CMD_POWEROFF);
    #endif

    /* shut down messages */
    DBG_DUMP("\r\n");

    GxLED_SetCtrl(KEYSCAN_LED_FCS,TURNON_LED,FALSE);
    GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,FALSE);
    GxLED_SetCtrl(KEYSCAN_LED_RED,TURNON_LED,FALSE);
    //Enable_AutoReset();

    //DxSys_Exit();
}


