/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       SysMain_Flow_Init.c
    @ingroup    mSystemFlow

    @brief      PowerOn Flow

    @note
                1.提供power on的介面
                  System_PowerOn()
                  (a)System_PowerOn()執行
                    各mode不同的control condition
                    包含通知不同device init
                  (b)System_PowerOn()執行
                    各mode不同的control condition
                    包含控制不同device之間相互dependent的order

    @date       2012/1/1
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "UICommon.h"
#include "AppLib.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysInit
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

/////////////////////////////////////////////////////////////////////////////
#include "DevMan.h"
//#include "DeviceSysMan.h"
//#include "DeviceNandMan.h"
//#include "NvtSystem.h"

/////////////////////////////////////////////////////////////////////////////
#if PIP_VIEW_FUNC
#include "PipView.h"
#endif

INT32 g_iSysPowerOnMode = SYS_POWERON_NORMAL;


/**
  System initialization

  Do system initialization

  @param void
  @return void
*/
void System_PowerOn(UINT32 pwrOnType)
{
    DBG_FUNC_BEGIN("\r\n");
    g_iSysPowerOnMode = pwrOnType;

    //register int i;
    Perf_Open();
    //TM_Begin(OFF_TO_CAPTURE);
    //TM_Begin(OFF_TO_PREVIEW);

#if _MIPS_TODO
    //For Main code size debuging
    {
        extern void ScfFile_Debug(void);
        ScfFile_Debug();
    }
#endif

#if 0
#if _MIPS_TODO
    //SystemCopyPart1();
#endif

#if _MIPS_TODO
    //if(!SystemIsPartialLoaded())
#endif
    {
        DBG_MSG("FW Full loaded, force used Safe mode.\r\n");
        //Full-load, Single-task
        System_SetState(SYS_STATE_POWERON, SYS_POWERON_SAFE); //power-on = safe mode
        System_SetState(SYS_STATE_POWEROFF, SYS_POWEROFF_SAFE); //power-off = safe mode
        System_SetState(SYS_STATE_PARTIAL_LOAD,SYSTEM_PARTIAL_LOAD_DISABLE); //partial-load = disable
    }

    ///////////////////////////////////////////
    // Start Device manager
    for(i=0;i<DEVICE_ID_MAX;i++)
        DevMan[i]->Open();

    ///////////////////////////////////////////
    if(System_GetState(SYS_STATE_POWERON) == SYS_POWERON_NORMAL)
    {
        //"normal power-on sequence"
        DBG_MSG("Power On Sequence = Normal\r\n");

        //PART-1
        //TM_Begin(RUN_PART_1);
            // Initialize HW devices
            DevMan[SYS]->CmdAsync(DEVMAN_CMD_INIT, 0, 0);
            DevMan[NAND]->CmdAsync(DEVMAN_CMD_INIT, 0, 0);
            DevMan[USB]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            DevMan[SYS]->WaitSync(DEVMAN_SYS_SYNC_PWRCHECK);//wait until PWR dummyload finish
            DevMan[SENSOR]->CmdAsync(DEVMAN_CMD_INIT, 0, 0);
            DevMan[LENS]->CmdAsync(DEVMAN_CMD_INIT, 0, 0);
            DevMan[DISP]->CmdAsync(DEVMAN_CMD_INIT, 0, 0);
        //TM_End(RUN_PART_1);

        //PART-2
        //TM_Begin(RUN_PART_2);
            #if _MIPS_TODO
            DevMan[NAND]->WaitSync(DEVMAN_NAND_SYNC_PCODE2);//wait until PART-2 finish
            #endif
            // Initialize HW devices
            DevMan[AUDIO]->Cmd(DEVMAN_CMD_INIT, 0, 0);
        //TM_End(RUN_PART_2);

        //PART-3
        //TM_Begin(RUN_PART_3);
            #if _MIPS_TODO
            DevMan[NAND]->WaitSync(DEVMAN_NAND_SYNC_PCODE3);//wait until PART-3 finish
            #endif
            // Initialize FW subsystems
            DevMan[CARD]->Cmd(DEVMAN_CMD_INIT, 0, 0);//need to wait for ENDCOPYPART3 (NAND/CARD share bus issue)
        //TM_End(RUN_PART_3);

    }
    else if(System_GetState(SYS_STATE_POWERON) == SYS_POWERON_SAFE)
    {
        //"safe power-on sequence"
        DBG_MSG("Power On Sequence = Safe\r\n");

        //PART-1
        //TM_Begin(RUN_PART_1);
            DevMan[SYS]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            DevMan[NAND]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            DevMan[CARD]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            //DevMan[SYS]->WaitSync(DEVMAN_SYS_SYNC_PWRCHECK);//wait until PWR dummyload finish
        //TM_End(RUN_PART_1);

        //PART-2
        //TM_Begin(RUN_PART_2);
            #if _MIPS_TODO
            DevMan[NAND]->WaitSync(DEVMAN_NAND_SYNC_PCODE2);//wait until PART-2 finish
            #endif
        //TM_End(RUN_PART_2);

        //PART-3
        //TM_Begin(RUN_PART_3);
            #if _MIPS_TODO
            DevMan[NAND]->WaitSync(DEVMAN_NAND_SYNC_PCODE3);//wait until PART-3 finish
            #endif
            // Initialize HW devices
            DevMan[USB]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            DevMan[AUDIO]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            DevMan[SENSOR]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            DevMan[LENS]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            DevMan[DISP]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            // Initialize FW subsystems
        //TM_End(RUN_PART_3);
    }
    else if(System_GetState(SYS_STATE_POWERON) == SYS_POWERON_LOWPOWER)
    {
        //"lowpower power-on sequence"
        DBG_MSG("Power On Sequence = LowPower\r\n");

        //PART-1
        //TM_Begin(RUN_PART_1);
            DevMan[SYS]->CmdAsync(DEVMAN_CMD_INIT, 0, 0);
            DevMan[NAND]->CmdAsync(DEVMAN_CMD_INIT, 0, 0);
            DevMan[SYS]->WaitSync(DEVMAN_SYS_SYNC_PWRCHECK);//wait until PWR dummyload finish
        //TM_End(RUN_PART_1);

        //PART-2
        //TM_Begin(RUN_PART_2);
            #if _MIPS_TODO
            DevMan[NAND]->WaitSync(DEVMAN_NAND_SYNC_PCODE2);//wait until PART-2 finish
            #endif
        //TM_End(RUN_PART_2);

        //PART-3
        //TM_Begin(RUN_PART_3);
            #if _MIPS_TODO
            DevMan[NAND]->WaitSync(DEVMAN_NAND_SYNC_PCODE3);//wait until PART-3 finish
            #endif
            // Initialize HW devices
            //DevMan[USB]->Cmd(DEVMAN_CMD_INIT, 0, 0); //no usb
            DevMan[AUDIO]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            //DevMan[SENSOR]->Cmd(DEVMAN_CMD_INIT, 0, 0); //no sensor
            //DevMan[LENS]->Cmd(DEVMAN_CMD_INIT, 0, 0); //no lens
            DevMan[DISP]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            // Initialize FW subsystems
            //DevMan[CARD]->Cmd(DEVMAN_CMD_INIT, 0, 0);//no file
        //TM_End(RUN_PART_3);
    }
    else if(System_GetState(SYS_STATE_POWERON) == SYS_POWERON_CHARGE)
    {
        //"charge power-on sequence"
        DBG_MSG("Power On Sequence = Charge\r\n");

        //PART-1
        //TM_Begin(RUN_PART_1);
            DevMan[SYS]->CmdAsync(DEVMAN_CMD_INIT, 0, 0);
            DevMan[NAND]->CmdAsync(DEVMAN_CMD_INIT, 0, 0);
            DevMan[SYS]->WaitSync(DEVMAN_SYS_SYNC_PWRCHECK);//wait until PWR dummyload finish
        //TM_End(RUN_PART_1);

        //PART-2
        //TM_Begin(RUN_PART_2);
            #if _MIPS_TODO
            DevMan[NAND]->WaitSync(DEVMAN_NAND_SYNC_PCODE2);//wait until PART-2 finish
            #endif
        //TM_End(RUN_PART_2);

        //PART-3
        //TM_Begin(RUN_PART_3);
            #if _MIPS_TODO
            DevMan[NAND]->WaitSync(DEVMAN_NAND_SYNC_PCODE3);//wait until PART-3 finish
            #endif
            // Initialize HW devices
            DevMan[USB]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            DevMan[AUDIO]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            //DevMan[SENSOR]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            //DevMan[LENS]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            DevMan[DISP]->Cmd(DEVMAN_CMD_INIT, 0, 0);
            // Initialize FW subsystems
            //DevMan[CARD]->Cmd(DEVMAN_CMD_INIT, 0, 0);
        //TM_End(RUN_PART_3);
    }

    ///////////////////////////////////////////
    //wait until all device manager init finish
    for(i=0;i<DEVICE_ID_MAX;i++)
        DevMan[i]->WaitFinish(DEVMAN_CMD_INIT);
#endif
    DBG_FUNC_END("\r\n");

    //avoid event in queue,and execute at the same time
    UI_UnlockEvent();
}

////////////////////////////////////////////////////////////////////////////////
#include "DxSys.h"
#include "DrvExt.h" //Dx_OnCommand, Dx_ConfigIO()
#include "Utility.h"
#include "ProjectInfo.h" //Prj version
/////////////////////////////////////////////////////////////////////////////
//Low level device module
extern void System_BindDeviceModule(void);

extern void SysMan_Config(void);
extern void NandMan_Config(void);
extern void UsbMan_Config(void);
extern void CardMan_Config(void);
extern void DispMan_Config(void);
extern void SensorMan_Config(void);
extern void LensMan_Config(void);
extern void AudioMan_Config(void);

extern UINT32 User_GetTempBuffer(UINT32 uiSize);
extern BOOL User_OnCommand(CHAR *pcCmdStr);
//#NT#2012/06/19#Calvin Chang#Port UserMedia to ExamSrv Mode -begin
//extern SX_CMD_ENTRY Cmd_mediatest[]; //user media test
//#NT#2012/06/19#Calvin Chang -end

#if 0
// for FW update FW - begin
#include "UiFwSrvTest_Cmd.h"
extern SX_CMD_ENTRY Cmd_libfs[]; //lib
// for FW update FW - end
#endif
extern SX_CMD_ENTRY mode[]; //system flow: app mode
extern SX_CMD_ENTRY user[];

void System_BindDeviceModule(void)
{
    debug_msg("bind - begin!\r\n");
#if 0
    // for FW update FW - begin
    SxCmd_AddTabel(UiFwSrvTest_GetCmdEntry());
    SxCmd_AddTabel(Cmd_libfs);
    // for FW update FW - end
#endif
    SxCmd_AddTable(mode);
    SxCmd_AddTable(user);
    #if PIP_VIEW_FUNC
    PipView_InstallCmd();
    #endif
#if 0
    ///////////////////////////////////////////
    // Config all device managers
    SysMan_Config();
    NandMan_Config();
    UsbMan_Config();
    CardMan_Config();
    DispMan_Config();
    SensorMan_Config();
    LensMan_Config();
    AudioMan_Config();
#endif
    //register Project level Command Parser Interface
    //System_AddSxCmd(User_OnCommand);
    debug_msg("bind - end!\r\n");
}


///////////////////////////////////////////////////////////////////////////////
//
//  START
//
///////////////////////////////////////////////////////////////////////////////
#include "ProjectInfo.h"
#include "BinInfo.h"
extern BININFO gBinInfo;

void GxSystem_BootStart(void)
{
        GxLED_SetCtrl(KEYSCAN_LED_GREEN,SETLED_SPEED,KEYSCAN_LED_TSPEED_NORMAL);
        GxLED_SetCtrl(KEYSCAN_LED_RED,SETLED_SPEED,KEYSCAN_LED_TSPEED_NORMAL);
        GxLED_SetCtrl(KEYSCAN_LED_FCS,SETLED_SPEED,KEYSCAN_LED_TSPEED_NORMAL);

        GxLED_SetCtrl(KEYSCAN_LED_FCS,TURNON_LED,FALSE);
        GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,FALSE);
        GxLED_SetCtrl(KEYSCAN_LED_RED,TURNON_LED,TRUE);

    //TM_Begin(BOOT_START);
    //TM_End(BOOT_START);
    //TM_Begin(START_CB);
    //PowerOn_CB(POWERON_CB_START); //callback for customize request
        /* version messages */
        DBG_DUMP("\r\n");
        DBG_DUMP("---------------------------------------------------------\r\n");
        DBG_DUMP("LD VERISON: %s\r\n", gBinInfo.ld.LdInfo_1);
#if (FW_VERSION_RELEASE == ENABLE)
        DBG_DUMP("FW VERISON: %s\r\n", gBinInfo.fw.FwInfo_1);
#else
        DBG_DUMP("FW --- Daily Build: %s\r\n", Prj_GetCheckinDate());
#endif
        //DBG_DUMP("FW Version: %s\r\n", Prj_GetVersionString());
        //DBG_DUMP("Model Name: %s\r\n", Prj_GetModelInfo());
        //DBG_DUMP("Release Date: %s\r\n", Prj_GetReleaseDate());
        /*
        DBG_MSG("- Project      ver: %s, build: %s\r\n", Prj_GetVerInfo(), Prj_GetBuildDateStd());
        SxCmd_DoCommand("comm ver"); //common (debug, kernel)
        SxCmd_DoCommand("drv ver"); //drv
        //DBG_MSG("- Application  ver: %s, build: %s\r\n", App_GetVerInfo(), App_GetBuildDate());
        DBG_MSG("- Filesys      ver: %s, build: %s\r\n", FileSys_GetVerInfo(), FileSys_GetBuildDate());
        //DBG_MSG("- FileDB       ver: %s, build: %s\r\n", FileDB_GetVerInfo(), FileDB_GetBuildDate());
        //DBG_MSG("- USIDC        ver: %s, build: %s\r\n", Sidc_GetVerInfo(), USIDC_GetBuildDate());
        */
        DBG_DUMP("---------------------------------------------------------\r\n");
        DBG_DUMP("\r\n");
        //DBG_DUMP("POWERON_CB_START\r\n");
        System_SetState(SYS_STATE_POWERON, SYS_POWERON_NORMAL); //power-on = normal mode
        System_SetState(SYS_STATE_POWEROFF, SYS_POWEROFF_NORMAL); //power-off = normal mode
    //TM_End(START_CB);
}

void GxSystem_PowerOnBreak(void)
{
    DBG_ERR("BREAK\r\n\r\n");

    /* shut down messages */
    DBG_DUMP("\r\n");

    GxLED_SetCtrl(KEYSCAN_LED_FCS,TURNON_LED,FALSE);
    GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,FALSE);
    GxLED_SetCtrl(KEYSCAN_LED_RED,TURNON_LED,FALSE);

    DxSys_Exit();
}


///////////////////////////////////////////////////////////////////////////////
//
//  END
//
///////////////////////////////////////////////////////////////////////////////

void GxSystem_BootEnd(void)
{
    //Enable_WP

    //TM_Begin(CB_END);
    //PowerOn_CB(POWERON_CB_END); //callback for customize request (before CB)
        DBG_IND("POWERON_CB_END\r\n");
    //TM_End(CB_END);

    //TM_Begin(BOOT_END);
/*
    {
        //TM_Begin(INIT_WAITA);
        INIT_WAITFLAG(FLGINIT_ENDA); //wait until InitA() finish ~~~ IPL
        //TM_End(INIT_WAITA);

        //TM_Begin(INIT_WAITC);
        INIT_WAITFLAG(FLGINIT_ENDC); //wait until InitC() finish ~~~ Lens
        //TM_End(INIT_WAITC);
    }
*/
    //TM_End(BOOT_END);
}

