#include "SysKer.h"
#include "GxSystem.h"
#include "SysCfg.h"
#include "stdlib.h"
#include "string.h" //for strlen(), strcpy(), strncmp()
#include "NvtSystem.h"
#include "FilesysTsk.h"
#include "DrvExt.h"
#include "SysMain.h"
#include "DbgUtApi.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          main
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (_INSTRUMENT_FUNCTION_ == 1)
#include <stdio.h>
#include "Debug.h"
#include "Platform.h"

//Reference: http://blog.linux.org.tw/~jserv/archives/001870.html

extern UINT32 user_SecLoad[10];

void _STOPTRACE __cyg_profile_func_enter(void *this_func, void *call_site)
{
    debug_msg(">>> func = %08x, call = %08x ", this_func, call_site);

    //check partial-load
    {
        UINT32 addr = (UINT32)this_func;
        int i;
        for(i=0;i<10;i++)
        {
            //check inside section id
            if((addr >= OS_GetMemAddr(i)) && (addr < OS_GetMemAddr(i)+OS_GetMemSize(i)))
            {
                if(user_SecLoad[i] == 1)
                {
                    debug_msg("secID=%d\r\n", user_SecLoad[i]);
                }
                else
                {
                    debug_msg("secID=%d ^RNOT LOADED YET!\r\n", user_SecLoad[i]);
                }
                continue;
            }
        }
    }

    debug_msg("\r\n");
}
void _STOPTRACE __cyg_profile_func_exit(void *this_func, void *call_site)
{
    //debug_msg("<<< func = %08x, call = %08x\r\n", this_func, call_site);
}
#endif

extern UINT32 User_GetTempBuffer(UINT32 uiSize);
extern BOOL User_CommandFilter(CHAR* pcCmdStr);
extern void System_BindDeviceModule(void);

/**
    main

    Main routine of system. This will be the first C-routine
*/

extern SX_CMD_ENTRY dsc[]; //SysCmdCB.c
extern SX_CMD_ENTRY dx[]; //drvext

FILEOP_TAB fileOpTab={
FileSys_OpenFile,
FileSys_CloseFile,
FileSys_WriteFile,
(FST_OPEN_WRITE|FST_OPEN_ALWAYS)
};

#include "DxSys.h"
#ifdef __ECOS
#include <cyg/infra/maincmd.h>
static BOOL eCos_Cmd(CHAR* pStr)
{
    if(pStr[0]=='?')
    {
        listmaincmd();
    }
    else
    {
        runmaincmd(pStr);
    }
    return TRUE;
}
#endif
int NvtMain(int argc, char *argv[])
{
    debug_msg("Hello, World!\r\n");

    //Init DbgUt to measure usage of CPU/DMA
    {
        DBGUT_INIT DbgUtInit = {0};
        DbgUtInit.uiApiVer = DBGUT_API_VERSION;
        DbgUt_Init(&DbgUtInit);
    }

    // Start SxCmd
    SxCmd_Open();
    //register user command filter
    SxCmd_RegCmdFilterCB(User_CommandFilter);
    //register Project level Memory Provide API for Command Parser Interface
    SxCmd_RegTempMemFunc(User_GetTempBuffer);
    //add: dsc boot cmd, dsc shutdown cmd
#ifdef __ECOS
    SxCmd_RegeCosCB(eCos_Cmd);
#endif
    SxCmd_AddTable(dsc); //SysCmdCB.c
#if (POWERON_TRACE == ENABLE)
    //////////////////////////////////////////
    // Wait for some on call GxSystem_PowerOn()
    GxSystem_WaitForPowerOn();
#endif
    //add:
    //OS_InstallCmd();
    SxCmd_AddTable(dx);  //drvext
    debug_reg_fileFunc(&fileOpTab);  //register debug dump opertaion table
#if (POWERON_TESTFLOW == ENABLE)
    //SxCmd_DoCommand("dma eclk");
#endif
    //////////////////////////////////////// DO NOT CHANGE THESE CODE - BEGIN //////////////
    //TM_Begin(IO_INIT);
    Dx_InitIO();
    //TM_End(IO_INIT);
    //////////////////////////////////////// DO NOT CHANGE THESE CODE - END ////////////////
    // Start system service, hook devices, user task
    debug_msg("Enter DSC\r\n");
    ///////////////////////////////////////////
    //call user to config Mode

    ///////////////////////////////////////////
    //call all devices to config Lib (also config SxJob/SxTimer/SxCmd)
    System_BindDeviceModule();
    DscMain((UINT32)UserMainProc);
    // Start Dsc System
    //System_PowerOn(SYS_POWERON_NORMAL);

    //////////////////////////////////////////
    // Wait for some on call GxSystem_PowerOff()
    GxSystem_WaitForPowerOff();
    // End user task, device,  system service
    debug_msg("EXIT DSC\r\n");
    DscExit();

    //////////////////////////////////////// DO NOT CHANGE THESE CODE - BEGIN //////////////
    //TM_Begin(IO_INIT);
    Dx_UninitIO();
    //TM_End(IO_INIT);
    //////////////////////////////////////// DO NOT CHANGE THESE CODE - END ////////////////
#if (POWERON_TESTFLOW == ENABLE)
    //SxCmd_DoCommand("dma eclk");
#endif

    // End SxCmd
    //SxCmd_DoCommand("ker dump");
    SxCmd_Close();

    debug_msg("Byebye, World!\r\n");

    return 1;
}


