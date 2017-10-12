/*
    System Output Callback

    System Callback for Output Module.

    @file       SysOutput_Exe.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"


//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysOutputExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
int SX_TIMER_DET_LED_ID = -1;
SX_TIMER_ITEM(Output_LED, GxLED_ToggleLED, 1, TRUE)

void System_OnOutputInit(void)
{
    //PHASE-1 : Init & Open Drv or DrvExt
    {
        //1.設定init值
        //2.設定CB值,
        //3.註冊SxJob服務 ---------> System Job
        //4.註冊SxTimer服務 ---------> Detect Job
        SX_TIMER_DET_LED_ID = SxTimer_AddItem(&Timer_Output_LED);
    }
    //PHASE-2 : Init & Open Lib or LibExt
    {
    }
}

void System_OnOutputExit(void)
{
    //PHASE-2 : Close Lib or LibExt
    {
    }
    //PHASE-1 : Close Drv or DrvExt
    {
    }
}
