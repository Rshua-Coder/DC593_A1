/*
    System Video Callback

    System Callback for Video Module.

    @file       SysVideo_Exe.c
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

#include "DxType.h"
#include "Dx.h"
#include "DxApi.h"

#include "DxDisplay.h"
#include "DispSrvAPI.h"


#include "UIGraphics.h"
#include "GxSound.h"
#include "UIInfo.h"
#include "GxVideo.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysVideoExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

BOOL g_bDualUI = FALSE;
DX_HANDLE gDevLCDObj = 0;
DX_HANDLE gDevTVObj = 0;
DX_HANDLE gDevHDMIObj = 0;
extern SX_CMD_ENTRY Cmd_lcd[]; //device
//extern SX_CMD_ENTRY Cmd_tv[]; //device
//extern SX_CMD_ENTRY Cmd_hdmi[]; //device

void Display_DetTV(void);
void Display_DetStatus(void);
void Display_DetLCDDir(void);
int SX_TIMER_DET_TV_ID = -1;
int SX_TIMER_DET_LCDREG_ID = -1;
int SX_TIMER_DET_LCDDIR_ID = -1;
#if (TVINSERT_FUNCTION == ENABLE)
SX_TIMER_ITEM(Display_DetTV, Display_DetTV, 25, FALSE)
#endif
#if (DET_LCD_REG_FUNCTION == ENABLE)
SX_TIMER_ITEM(Display_DetStatus, Display_DetStatus, 100, FALSE)
#endif
#if (DET_LCDDIR_FUNCTION == ENABLE)
SX_TIMER_ITEM(Display_DetLCDDir, Display_DetLCDDir, 100, FALSE)
#endif

void UI_ShowJPG(UINT8 LayerID, UINT32 uiJPGData, UINT32 uiJPGSize, UINT32 uiTempBuf);

void System_SetAudioOutput(void);

extern void Video_CB(UINT32 event, UINT32 param1, UINT32 param2);

void System_OnVideoInit(void)
{
    DX_HANDLE cDispDev = 0;
    //PHASE-1 : Init & Open Drv or DrvExt
    {
        GxVideo_RegCB(Video_CB);         //Register CB function of GxVideo

        gDevLCDObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_LCD);
        DBG_MSG("LCD = %08x\r\n", gDevLCDObj);
        gDevTVObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_TVOUT);
        DBG_MSG("TV = %08x\r\n", gDevTVObj);
        gDevHDMIObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_HDMIOUT);
        DBG_MSG("HDMI = %08x\r\n", gDevHDMIObj);

        GxVideo_InitDevice(DOUT1);
        GxVideo_InitDevice(DOUT2);

        cDispDev = gDevLCDObj;
        if(cDispDev)
        {
            //Dx_Init(cDispDev, 0, Display_CB, DISPLAY_VER);
            Dx_Init(cDispDev, 0, 0, DISPLAY_VER);
        }
        cDispDev = gDevTVObj;
        if(cDispDev)
        {
            //Dx_Init(cDispDev, 0, Display_CB, DISPLAY_VER);
            Dx_Init(cDispDev, 0, 0, DISPLAY_VER);
        }
        cDispDev = gDevHDMIObj;
        if(cDispDev)
        {
            //Dx_Init(cDispDev, 0, Display_CB, DISPLAY_VER);
            Dx_Init(cDispDev, 0, 0, DISPLAY_VER);
        }
        //5.註冊SxCmd服務 ---------> Cmd Function
        //SxCmd_AddTabel(Cmd_lcd);
        //SxCmd_AddTabel(Cmd_tv);
        //SxCmd_AddTabel(Cmd_hdmi);
        //GxVideo_InstallCmd();
    }
    //PHASE-2 : Init & Open Lib or LibExt
    {
        //Config SxJob,SxTimer,SxCmd
        //3.註冊SxJob服務 ---------> System Job
        //SX_JOB_DISPLAY = System_AddSxJob(Display_Change_Job);
        //SX_JOB_DISPMODE = System_AddSxJob(Display_ChangeMode_Job);
        //4.註冊SxTimer服務 ---------> Detect Job
        #if (TVINSERT_FUNCTION == ENABLE)
        SX_TIMER_DET_TV_ID = SxTimer_AddItem(&Timer_Display_DetTV);
        #endif
        #if (DET_LCD_REG_FUNCTION == ENABLE)
        SX_TIMER_DET_LCDREG_ID = SxTimer_AddItem(&Timer_Display_DetStatus);
        #endif
        #if (DET_LCDDIR_FUNCTION == ENABLE)
        SX_TIMER_DET_LCDDIR_ID = SxTimer_AddItem(&Timer_Display_DetLCDDir);
        #endif

        //5.註冊SxCmd服務 ---------> Cmd Function

        //config video Output
        cDispDev = gDevLCDObj;
        GxVideo_OpenDevice(DOUT1, (UINT32)cDispDev, DISP_LASTMODE);//open LCD first
        GxVideo_DumpInfo();
        #if (DET_LCDDIR_FUNCTION == ENABLE)
        //config video Direct
        Display_DetLCDDir();
        #endif

        Ux_PostEvent(NVTEVT_VIDEO_ATTACH, 1, 1); // attach

        #if (TVINSERT_FUNCTION == ENABLE)
        SxTimer_SetFuncActive(SX_TIMER_DET_TV_ID, TRUE);
        #endif
        #if (DET_LCDDIR_FUNCTION == ENABLE)
        SxTimer_SetFuncActive(SX_TIMER_DET_LCDDIR_ID, TRUE);
        #endif

        //DispSrv_InstallCmd();
        AppView_Init();
        {
        #if(VDO_USE_ROTATE_BUFFER == ENABLE)
            UINT32 uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO1);
            UINT32 uiPoolSize = OS_GetMempoolSize(POOL_ID_DISP_VDO1)+OS_GetMempoolSize(POOL_ID_DISP_VDO1TEMP)+OS_GetMempoolSize(POOL_ID_DISP_VDO1TEMP2);
            AppView_ConfigWorkingBuffer(uiPoolAddr, uiPoolSize);
            if((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevLCDObj)
            {
                //AppView_ConfigAttr(0, 0, DISPSRV_ROTATE_90); //enable rotate 90
                AppView_ConfigAttr(0, 0, DISPSRV_ROTATE_0); //enable rotate 90
            }
        #else
            UINT32 uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO1);
            UINT32 uiPoolSize = OS_GetMempoolSize(POOL_ID_DISP_VDO1)+OS_GetMempoolSize(POOL_ID_DISP_VDO1TEMP);
            AppView_ConfigWorkingBuffer(uiPoolAddr, uiPoolSize);
        #endif
        }
        {
        #if(OSD_USE_ROTATE_BUFFER == ENABLE)
            if((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevLCDObj)
            {
                View_Window_ConfigAttr(0, 1); //enable rotate 90
            }
        #endif
        }
    }
    ////////////////////////////////////////////////////////////////
    {
        ISIZE             DeviceSize;
        //Init Display
        DeviceSize = GxVideo_GetDeviceSize(DOUT1); //Get LCD size
        GxDisplay_Init(DOUT1, DeviceSize.w, DeviceSize.h);
        DeviceSize = GxVideo_GetDeviceSize(DOUT2); //Get LCD size
        GxDisplay_Init(DOUT2, DeviceSize.w, DeviceSize.h);
    }
}

void System_OnVideoExit(void)
{
    //PHASE-2 : Close Lib or LibExt
    {
        // Turn off LCD backlight
        GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
        GxVideo_SetDeviceCtrl(DOUT2, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);

        AppView_Exit();

        GxVideo_CloseDevice(DOUT1);
        GxVideo_CloseDevice(DOUT2);

        GxVideo_ExitDevice(DOUT1);
        GxVideo_ExitDevice(DOUT2);

        Ux_PostEvent(NVTEVT_VIDEO_DETACH, 1, 0); // detach
    }
    //PHASE-1 : Close Drv or DrvExt
    {
    }
}

///////////////////////////////////////////////////////////////////////
void Display_DetTV(void)
{
#if _MIPS_TODO
    if(!UI_IsForceLock())
#endif
    {
        DX_HANDLE pDispDev1 = Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_TVOUT);
        GxVideo_DetInsert((UINT32)pDispDev1, 0);
        DX_HANDLE pDispDev2 = Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_HDMIOUT);
        GxVideo_DetInsert((UINT32)pDispDev2, 1);
    }
}

#if (DET_LCD_REG_FUNCTION == ENABLE)
void Display_DetStatus(void)
{
    LCD_RegRestore();
}
#endif

#if (DET_LCDDIR_FUNCTION == ENABLE)
void Display_DetLCDDir(void)
{
#if _MIPS_TODO
    if(!UI_IsForceLock())
#endif
    {
        DX_HANDLE pDispDev1 = Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_LCD);
        GxVideo_DetDir((UINT32)pDispDev1, 0);
    }
}
#endif


///////////////////////////////////////////////////////////////////////

//Device flow event
ISIZE _QueryDeviceModeAspect(UINT32 NewDevObj, UINT32 mode)
{
    ISIZE AspectSize;

    //overwrite buffer size to reduce BW
    if(((DX_HANDLE)NewDevObj) == gDevLCDObj)
    {
    #if (DISPLAY_169HD == ENABLE)
    AspectSize.w = 16;
    AspectSize.h = 9;
    #else
    AspectSize.w = 4;
    AspectSize.h = 3;
    #endif
    }
    if(((DX_HANDLE)NewDevObj) == gDevTVObj)
    {
    AspectSize.w = 4;
    AspectSize.h = 3;
    }
    if(((DX_HANDLE)NewDevObj) == gDevHDMIObj)
    {
    AspectSize.w = 16;
    AspectSize.h = 9;
    }

    return AspectSize;
}


//TV flow
    //close device
    ///Step3: [App] Pause with wait Finish
    ///Step4: [DispDrv] Stop
    ///Step5: [DispDrv] Set DispSrv Disp Buffer
    //  1.disp src buffer size, fmt
    //  2.disp dest window (x,y,w,h)
    //  3.disp dest window dir
    //  4.disp physical aspect ratio (sx,sy)
    ///Step6: [DispDrv] Play
    ///Step7: [App] Resume with wait Finish (need trigger one flip)
    //open device
void AppView_DisableVideo(UINT32 DevID)
{
    AppView_ConfigEnable(DevID, FALSE);
}

void AppView_EnableVideo(UINT32 DevID, UINT32 CurDevObj)
{
    UINT32 VideoMode;
    ISIZE DeviceSize;
    ISIZE AspectSize;
    ISIZE BufferSize;

    //1.get last video mode
    VideoMode = GxVideo_QueryDeviceLastMode(CurDevObj);
    //2.get its size
    DeviceSize = GxVideo_QueryDeviceModeSize(CurDevObj, VideoMode);
    BufferSize = DeviceSize;
    #if (_FPGA_EMULATION_ == ENABLE)
    //overwrite buffer size to reduce BW
    if(CurDevObj == gDevLCDObj)
    {
    BufferSize.w = 320;
    BufferSize.h = 240;
    }
    if(CurDevObj == gDevTVObj)
    {
    BufferSize.w = 320;
    BufferSize.h = 240;
    }
    if(CurDevObj == gDevHDMIObj)
    {
    BufferSize.w = 360;
    BufferSize.h = 240;
    }
    #endif
    //3.get its Aspect Ratio
    AspectSize = _QueryDeviceModeAspect((UINT32)CurDevObj, VideoMode);

    DBG_MSG("disp[%d] => device %08x\r\n", DevID, CurDevObj);
    DBG_MSG("-devicemode = %d\r\n", VideoMode);
    DBG_MSG("-devicesize = %dx%d\r\n", DeviceSize.w, DeviceSize.h);
    DBG_MSG("-aspectsize = %dx%d\r\n", AspectSize.w, AspectSize.h);
    DBG_MSG("-buffersize = %dx%d\r\n", BufferSize.w, BufferSize.h);

    //config view
    #if(VDO_USE_ROTATE_BUFFER == ENABLE)
    if((DevID == 0) && (CurDevObj == gDevLCDObj))
    {
        AppView_ConfigAttr(DevID, 0, DISPSRV_ROTATE_90); //enable rotate 90
    }
    else
    {
        AppView_ConfigAttr(DevID, 0, DISPSRV_ROTATE_NO_HANDLE); //disable rotate 90
    }
    #endif
    AppView_ConfigEnable(DevID, TRUE);
    AppView_ConfigDevice(DevID, DeviceSize.w, DeviceSize.h, AspectSize.w, AspectSize.h);
    AppView_ConfigBuffer(DevID, BufferSize.w, BufferSize.h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED); //maybe equal to "device size", or calc by auto-aspect-ratio
    AppView_ConfigWindow(DevID, 0, 0, DeviceSize.w, DeviceSize.h); //maybe full to "device size", or calc by auto-aspect-ratio
}

void AppView_ChangeVideoMode(UINT32 DevID, UINT32 CurDevObj, UINT32 VideoMode)
{
    //UINT32 VideoMode;
    ISIZE DeviceSize;
    ISIZE BufferSize;
    ISIZE AspectSize;

    DeviceSize = GxVideo_QueryDeviceModeSize(CurDevObj, VideoMode);
    BufferSize = DeviceSize;
    #if (_FPGA_EMULATION_ == ENABLE)
    //overwrite buffer size to reduce BW
    if(CurDevObj == gDevLCDObj)
    {
    BufferSize.w = 320;
    BufferSize.h = 240;
    }
    if(CurDevObj == gDevTVObj)
    {
    BufferSize.w = 320;
    BufferSize.h = 240;
    }
    if(CurDevObj == gDevHDMIObj)
    {
    BufferSize.w = 360;
    BufferSize.h = 240;
    }
    #endif
    //3.get its Aspect Ratio
    AspectSize = _QueryDeviceModeAspect((UINT32)CurDevObj, VideoMode);

    DBG_MSG("disp[%d] => device %08x\r\n", DevID, CurDevObj);
    DBG_MSG("-devicemode = %d\r\n", VideoMode);
    DBG_MSG("-devicesize = %dx%d\r\n", DeviceSize.w, DeviceSize.h);
    DBG_MSG("-aspectsize = %dx%d\r\n", AspectSize.w, AspectSize.h);
    DBG_MSG("-buffersize = %dx%d\r\n", BufferSize.w, BufferSize.h);

    //config view
    #if(VDO_USE_ROTATE_BUFFER == ENABLE)
    if((DevID == 0) && (CurDevObj == gDevLCDObj))
    {
        AppView_ConfigAttr(DevID, 0, DISPSRV_ROTATE_90); //enable rotate 90
    }
    else
    {
        AppView_ConfigAttr(DevID, 0, DISPSRV_ROTATE_NO_HANDLE); //disable rotate 90
    }
    #endif
    AppView_ConfigEnable(DevID, TRUE);
    AppView_ConfigDevice(DevID, DeviceSize.w, DeviceSize.h, AspectSize.w, AspectSize.h);
    AppView_ConfigBuffer(DevID, BufferSize.w, BufferSize.h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED);
    AppView_ConfigWindow(DevID, 0, 0, DeviceSize.w, DeviceSize.h);
}

void UI_DisableVideo(UINT32 DevID)
{
    //TODO
}

void UI_EnableVideo(UINT32 DevID, UINT32 CurDevObj)
{
    UINT32 VideoMode;
    ISIZE DeviceSize;
    //ISIZE BufferSize;

    //1.get last video mode
    VideoMode = GxVideo_QueryDeviceLastMode(CurDevObj);
    //2.get its size
    DeviceSize = GxVideo_QueryDeviceModeSize(CurDevObj, VideoMode);

    DBG_MSG("disp[%d] => device %08x\r\n", DevID, CurDevObj);
    DBG_MSG("-devicemode = %d\r\n", VideoMode);
    DBG_MSG("-devicesize = %dx%d\r\n", DeviceSize.w, DeviceSize.h);

    #if(OSD_USE_ROTATE_BUFFER == ENABLE)
    if((DevID == 0) && (CurDevObj == gDevLCDObj))
    {
        View_Window_ConfigAttr(0, 1); //enable rotate 90
    }
    else
    {
        View_Window_ConfigAttr(0, 0); //disable rotate 90
    }
    #endif
    ///Resize OSD
    UI_OnChangeSize(DevID, &DeviceSize);
}

void UI_ChangeVideoMode(UINT32 DevID, UINT32 CurDevObj, UINT32 VideoMode)
{
    ISIZE DeviceSize;
    //ISIZE BufferSize;

    //2.get its size
    DeviceSize = GxVideo_QueryDeviceModeSize(CurDevObj, VideoMode);

    DBG_MSG("disp[%d] => device %08x\r\n", DevID, CurDevObj);
    DBG_MSG("-devicemode = %d\r\n", VideoMode);
    DBG_MSG("-devicesize = %dx%d\r\n", DeviceSize.w, DeviceSize.h);

    #if(OSD_USE_ROTATE_BUFFER == ENABLE)
    if((DevID == 0) && (CurDevObj == gDevLCDObj))
    {
        View_Window_ConfigAttr(0, 1); //enable rotate 90
    }
    else
    {
        View_Window_ConfigAttr(0, 0); //disable rotate 90
    }
    #endif
    ///Resize OSD
    UI_OnChangeSize(DevID, &DeviceSize);
}



extern void System_ResetPowerSaveCount(void);
void _DD_SyncLayerEn(UINT32 DevID, UINT32 DevID2);

INT32 System_OnVideoInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //IMPORTANT NOTE: to support HDMI, user must enable PIN_I2C_CFG_CH2 in PinmuxCfg.c

    DX_HANDLE OldDevObj = gDevLCDObj;
    DX_HANDLE NewDevObj = 0;
    BOOL bNeedDualPlugIn = (UI_GetData(FL_DualDisp) == DUALDISP_ONBOTH);

    if(paramNum != 1)
        return NVTEVT_CONSUME;

        {
            //Relationship between TV and sleep:
            //    begin TV pluging flow ->  wakeup sleep, BUT do not turn on display and backlight
            //    end TV pluging flow ->  stop Sleep counting, turn on display and backlight
            if(GxPower_GetControl(GXPWR_CTRL_SLEEP_LEVEL) > 0)  // if sleep level > 0
            {
                System_ChangeSleep(0); //enforce trigger system wakeup
            }
        }

    UI_LockEvent();

    NewDevObj = (DX_HANDLE)(paramArray[0]);

    //close device
    DBG_MSG("^Y===================================== video plug\r\n");
    if(NewDevObj == gDevTVObj)
    {
        DBG_MSG("^Ydev=TV\r\n");
    }
    if(NewDevObj == gDevHDMIObj)
    {
        DBG_MSG("^Ydev=HDMI\r\n");
    }
    DBG_MSG("old = %08x\r\n", OldDevObj);
    DBG_MSG("new = %08x\r\n", NewDevObj);

    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
//==================================================================== disp off
    GxVideo_DumpInfo();
    if(!bNeedDualPlugIn)
    {
    GxVideo_CloseDevice(DOUT1);
    }
    else
    {
    GxVideo_CloseDevice(DOUT1);
    }
//==================================================================== disp off

        {
            //Relationship between TV and sleep:
            //    begin TV pluging flow ->  wakeup sleep, BUT do not turn on display and backlight
            //    end TV pluging flow ->  stop Sleep counting, turn on display and backlight
            System_ResetPowerSaveCount();
            UI_LockAutoSleep(); //stop sleep counting
            UI_LockAutoPWROff(); //stop power-save counting
            // custom flow - end
        }

    ///Step3: [App] Pause with wait Finish
    ///Step4: [DispDrv] Stop
    AppView_Pause();

//==================================================================== disp on
    //open device
    if(!bNeedDualPlugIn)
    {
    g_bDualUI = FALSE;
    Dx_Setconfig(OldDevObj, DISPLAY_CFG_DUAL, FALSE);
    GxVideo_OpenDevice(DOUT1, (UINT32)NewDevObj, DISP_LASTMODE); //change device when lock
    }
    else
    {
    g_bDualUI = TRUE;
    Dx_Setconfig(OldDevObj, DISPLAY_CFG_DUAL, TRUE);
    _DD_SyncLayerEn(DOUT2, DOUT1);
    GxVideo_OpenDevice(DOUT1, (UINT32)OldDevObj, DISP_LASTMODE); //change device when lock
    GxVideo_OpenDevice(DOUT2, (UINT32)NewDevObj, DISP_LASTMODE); //change device when lock
    }
    GxVideo_DumpInfo();
    DBG_MSG("^YTV plug ok\r\n");

    //config audio Output
    System_SetAudioOutput();
//==================================================================== disp on
    //GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
    if(!bNeedDualPlugIn)
    {
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
    }
    else
    {
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
    GxVideo_SetDeviceCtrl(DOUT2, DISPLAY_DEVCTRL_BACKLIGHT, FALSE); //???
    }

    if(!bNeedDualPlugIn)
    {
    AppView_EnableVideo(0, (UINT32)NewDevObj); //TV or HDMI
    }
    else
    {
    AppView_EnableVideo(0, (UINT32)OldDevObj); //LCD
    AppView_EnableVideo(1, (UINT32)NewDevObj); //TV or HDMI
    }

    ///Step6: [DispDrv] Play
    ///Step7: [App] Resume with wait Finish (need trigger one flip)
    AppView_Resume();

    if(!bNeedDualPlugIn)
    {
    UI_EnableVideo(0, (UINT32)NewDevObj); //TV or HDMI
    }
    else
    {
    DBG_MSG("UI Resize IDE1 OSD1\r\n");
    UI_EnableVideo(0, (UINT32)OldDevObj); //LCD
    DBG_MSG("UI Resize IDE2 OSD1\r\n");
    UI_EnableVideo(1, (UINT32)NewDevObj); //TV or HDMI
    }

    UI_UnlockEvent();

    //restart current UI mode
    //#NT#2012/11/5#Philex Lin - begin
    // restore CarDV current UI mode
    #if (UI_STYLE==UI_STYLE_DRIVER)
    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_CHANGEDSCMODE,1,DSCMODE_CHGTO_CURR);
    #else
    {
        UINT32 uiDscMode = UI_GetData(FL_ModeIndex);
        UINT32 uiOriPreMode;
        switch(uiDscMode)
        {
        case DSC_MODE_PHOTO_AUTO:
        case DSC_MODE_PHOTO_MANUAL:
        case DSC_MODE_PHOTO_SCENE:
            UI_Switch_DscMode(uiDscMode,DSCMODE_SWITCH_FORCE,DSCMODE_SWITCHDIR_DONT_CARE);
            break;
        case DSC_MODE_MOVIE:
            UI_Switch_DscMode(uiDscMode,DSCMODE_SWITCH_FORCE,DSCMODE_SWITCHDIR_DONT_CARE);
            break;
        case DSC_MODE_PLAYBACK:
            //#NT#2012/10/26#Ben Wang -begin
            //#NT#reserve original FL_PreMode
            uiOriPreMode = UI_GetData(FL_PreMode);
            UI_Switch_DscMode(uiDscMode,DSCMODE_SWITCH_FORCE,DSCMODE_SWITCHDIR_DONT_CARE);
            UI_SetData(FL_PreMode,uiOriPreMode);
            //#NT#2012/10/26#Ben Wang -end
            break;
        }
    }
    #endif
    #if (DET_LCDDIR_FUNCTION == ENABLE)
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_FORCEDETDIR, 1);
    //config video Direct
    Display_DetLCDDir();
    #endif
    //#NT#2012/11/5#Philex Lin - end
    if(!bNeedDualPlugIn)
    {
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, TRUE);
    }
    else
    {
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, TRUE);
    GxVideo_SetDeviceCtrl(DOUT2, DISPLAY_DEVCTRL_BACKLIGHT, TRUE); //???
    }

    return NVTEVT_CONSUME;
}

//#NT#2014/07/08#Philex Lin -begin
BOOL System_IsPlugTVHDMI(void)
{
  UINT32 uiDevObj1,uiDevObj2;

    uiDevObj1 = GxVideo_GetDevice(DOUT1);
    uiDevObj2 = GxVideo_GetDevice(DOUT2);
    if (uiDevObj1 == (UINT32)gDevTVObj||uiDevObj2 == (UINT32)gDevTVObj)
    {
        return TRUE;
    } else if (uiDevObj1 == (UINT32)gDevHDMIObj||uiDevObj2 == (UINT32)gDevHDMIObj)
    {
        return TRUE;
    } else {
        return FALSE;
    }
}
//#NT#2014/07/08#Philex Lin -end

BOOL IsDualPlugIn(void)
{
    UINT32 DevObj1, DevObj2;
    DevObj1 = GxVideo_GetDevice(DOUT1);
    DevObj2 = GxVideo_GetDevice(DOUT2);
    if((DevObj1 != 0) && (DevObj2 != 0))
        return TRUE;
    else
        return FALSE;
}

BOOL IsDualHDMIPlugIn(void)
{
    if((GxVideo_GetDevice(DOUT1) != 0)&&(GxVideo_GetDevice(DOUT2) != 0))
    {
        if (((DX_HANDLE)GxVideo_GetDevice(DOUT2))==gDevHDMIObj)
            return TRUE;
        else
            return FALSE;
    } else
        return FALSE;
}

INT32 System_OnVideoRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DX_HANDLE OldDevObj;
    DX_HANDLE NewDevObj = gDevLCDObj;

    if(paramNum != 1)
        return NVTEVT_CONSUME;

    UI_LockEvent();

    OldDevObj = (DX_HANDLE)(paramArray[0]);

    //#NT#2014/1/17#Philex Lin - begin
    // turn off RSC first if it is truning on before closing sensor object
    #if (UI_STYLE==UI_STYLE_DRIVER)
    #if(_GYRO_EXT_!=_GYRO_EXT_NONE_)
    if (SysGetFlag(FL_MovieRSCIndex)==MOVIE_RSC_ON)
    {
        // turn off RSC function while quitting movie mode
        IPL_AlgSetUIInfo(IPL_SEL_RSC, SEL_RSC_OFF);
    }
    #endif
    #endif
    //#NT#2014/1/17#Philex Lin - end

    //close device
    DBG_MSG("^Y===================================== video unplug\r\n");
    if(OldDevObj == gDevTVObj)
    {
        DBG_MSG("^Ydev=TV\r\n");
    }
    if(OldDevObj == gDevHDMIObj)
    {
        DBG_MSG("^Ydev=HDMI\r\n");
    }
    DBG_MSG("old = %08x\r\n", OldDevObj);
    DBG_MSG("new = %08x\r\n", NewDevObj);

//==================================================================== disp off
    GxVideo_DumpInfo();
    if(!IsDualPlugIn())
    {
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
    GxVideo_CloseDevice(DOUT1);
    }
    else
    {
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
    GxVideo_SetDeviceCtrl(DOUT2, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
    GxVideo_CloseDevice(DOUT1);
    GxVideo_CloseDevice(DOUT2);
    }
//==================================================================== disp off

    ///Step3: [App] Pause with wait Finish
    ///Step4: [DispDrv] Stop
    AppView_Pause();
//==================================================================== disp on
    //open device
    if(!IsDualPlugIn())
    {
    g_bDualUI = FALSE;
    Dx_Setconfig(NewDevObj, DISPLAY_CFG_DUAL, FALSE);
    GxVideo_OpenDevice(DOUT1, (UINT32)NewDevObj, DISP_LASTMODE); //change device when lock
    }
    else
    {
    g_bDualUI = FALSE;
    Dx_Setconfig(NewDevObj, DISPLAY_CFG_DUAL, FALSE);
    GxVideo_OpenDevice(DOUT1, (UINT32)NewDevObj, DISP_LASTMODE); //change device when lock
    }
    GxVideo_DumpInfo();
    DBG_MSG("^YTV unplug ok\r\n");

    //config audio Output
    System_SetAudioOutput();

//==================================================================== disp on
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);

    if(!IsDualPlugIn())
    {
    AppView_EnableVideo(0, (UINT32)NewDevObj); //LCD
    }
    else
    {
    AppView_EnableVideo(0, (UINT32)NewDevObj); //LCD
    AppView_DisableVideo(1); //TV or HDMI
    }

    ///Step6: [DispDrv] Play
    ///Step7: [App] Resume with wait Finish (need trigger one flip)
    AppView_Resume();

    if(!IsDualPlugIn())
    {
    UI_EnableVideo(0, (UINT32)NewDevObj); //LCD
    }
    else
    {
    DBG_MSG("UI Resize IDE1 OSD1\r\n");
    UI_EnableVideo(0, (UINT32)NewDevObj); //LCD
    UI_DisableVideo(1); //TV or HDMI
    }

        {
            //Relationship between TV and sleep:
            //    begin TV unplugin flow ->  wakeup sleep, BUT do not turn on display and backlight
            //    end TV unplugin flow ->  start Sleep counting, turn on display and backlight

            // custom flow - begin
            UI_UnlockAutoSleep(); //start sleep counting
            UI_UnlockAutoPWROff(); //start power-save counting
            // custom flow - end
        }

    UI_UnlockEvent();

    //restart current UI mode
    //#NT#2012/11/5#Philex Lin - begin
    // restore CarDV current UI mode
    #if (UI_STYLE==UI_STYLE_DRIVER)
    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_CHANGEDSCMODE,1,DSCMODE_CHGTO_CURR);
    #else
    {
        UINT32 uiDscMode = UI_GetData(FL_ModeIndex);
        UINT32 uiOriPreMode;
        switch(uiDscMode)
        {
        case DSC_MODE_PHOTO_AUTO:
        case DSC_MODE_PHOTO_MANUAL:
        case DSC_MODE_PHOTO_SCENE:
            UI_Switch_DscMode(uiDscMode,DSCMODE_SWITCH_FORCE,DSCMODE_SWITCHDIR_DONT_CARE);
            break;
        case DSC_MODE_MOVIE:
            UI_Switch_DscMode(uiDscMode,DSCMODE_SWITCH_FORCE,DSCMODE_SWITCHDIR_DONT_CARE);
            break;
        case DSC_MODE_PLAYBACK:
            //#NT#2012/10/26#Ben Wang -begin
            //#NT#reserve original FL_PreMode
            uiOriPreMode = UI_GetData(FL_PreMode);
            UI_Switch_DscMode(uiDscMode,DSCMODE_SWITCH_FORCE,DSCMODE_SWITCHDIR_DONT_CARE);
            UI_SetData(FL_PreMode,uiOriPreMode);
            //#NT#2012/10/26#Ben Wang -end
            break;
        }
    }
    #endif
    //#NT#2012/11/5#Philex Lin - end
    #if (DET_LCDDIR_FUNCTION == ENABLE)
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_FORCEDETDIR, 1);
    //config video Direct
    Display_DetLCDDir();
    #endif
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, TRUE);

    return NVTEVT_CONSUME;
}

INT32 System_OnVideoAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //  DISP: if(boot)->show boot logo/animation
    //  DISP: if(boot)->start disp srv
    //if(1st_attach)
    {
        //TM_Begin(UI_INIT);
#if (POWERON_TESTFLOW == ENABLE)
        if(!gIsUSBChargePreCheck)
        {
        Display_ShowSplash(SPLASH_POWERON);
        GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, TRUE);
        }
#else
        Display_ShowSplash(SPLASH_POWERON);
        Delay_DelayMs(30); // delay some time to avoid LCD flicker as power on
        GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, TRUE);
#endif
        UI_GfxInit();
        UI_DispInit();
        UI_ControlShowInit();
        //TM_End(UI_INIT);
    }
    return NVTEVT_CONSUME;
}

INT32 System_OnVideoDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}

#if (DET_LCDDIR_FUNCTION == ENABLE)
#if 0
#include "Display.h"
extern PDISP_OBJ       pDispObj[2];
#endif
#endif

INT32 System_OnVideoDir(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (DET_LCDDIR_FUNCTION == ENABLE)
    UINT32 CurDir = 0;
    UINT32 CurDevID = 0;
    DX_HANDLE VideoDevObj1;
    BOOL bDoRotate180 = FALSE;
    CurDevID = paramArray[0];
    CurDir = paramArray[1];
    debug_msg("CurDevID = %08x\r\n", CurDevID);
    debug_msg("CurDir = %d\r\n", CurDir);
    VideoDevObj1 = (DX_HANDLE)GxVideo_GetDevice(DOUT1);

    if(VideoDevObj1 != gDevLCDObj)
        return NVTEVT_CONSUME;

    switch(CurDir)
    {
    //Self-Capture 自拍
    case 0x00: bDoRotate180 = TRUE; break;
    //Open 打開
    case 0x01: bDoRotate180 = FALSE; break;
    //Normal 反收
    case 0x02: bDoRotate180 = TRUE; break;
    //Close 關閉
    case 0x03: bDoRotate180 = FALSE; break;
    }
    #if 0
    {
    DISPLAYER_PARAM LyrParam;
    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));
    if(bDoRotate180)
        LyrParam.SEL.SET_OUTDIR.BufOutDir = DISPOUTDIR_ROT_180;
    else
        LyrParam.SEL.SET_OUTDIR.BufOutDir = DISPOUTDIR_NORMAL;
    pDispObj[DOUT1]->dispLyrCtrl(DISPLAYER_OSD1,DISPLAYER_OP_SET_OUTDIR,&LyrParam); //it will call disp_translateBufAddress
    pDispObj[DOUT1]->dispLyrCtrl(DISPLAYER_OSD2,DISPLAYER_OP_SET_OUTDIR,&LyrParam); //it will call disp_translateBufAddress
    pDispObj[DOUT1]->dispLyrCtrl(DISPLAYER_VDO1,DISPLAYER_OP_SET_OUTDIR,&LyrParam); //it will call disp_translateBufAddress
    pDispObj[DOUT1]->dispLyrCtrl(DISPLAYER_VDO2,DISPLAYER_OP_SET_OUTDIR,&LyrParam); //it will call disp_translateBufAddress
    pDispObj[DOUT1]->load(TRUE);
    }
    #else
    if(bDoRotate180)
    {
        GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_DIRECT, MIRROR_X|MIRROR_Y);
    }
    else
    {
        GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_DIRECT, MIRROR_DISCARD);
    }
    #endif
#endif
    return NVTEVT_CONSUME;
}

INT32 System_OnVideoMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DX_HANDLE CurDevObj = 0;
    DX_HANDLE NewDevObj = 0;
    UINT32 VideoMode = 0; //new display

    DBG_MSG("SetMode!\r\n");

    CurDevObj = (DX_HANDLE)GxVideo_GetDevice(DOUT1);

    if(paramNum < 2)
        return NVTEVT_CONSUME;

    NewDevObj = (DX_HANDLE)(paramArray[0]);
    VideoMode = paramArray[1];

    DBG_MSG("cur = %08x\r\n", CurDevObj);
    DBG_MSG("new = %08x\r\n", NewDevObj);
    if(NewDevObj != CurDevObj)
    {
        DBG_IND("Not Current Device! Only config mode for next time.\r\n");
        GxVideo_ConfigDeviceFirstMode((UINT32)NewDevObj, VideoMode);

        return NVTEVT_CONSUME;
    }

    if(CurDevObj == gDevLCDObj)
    {
        DBG_MSG("^Y===================================== LCD Mode\r\n");
    }
    if(CurDevObj == gDevTVObj)
    {
        DBG_MSG("^Y===================================== TV Mode\r\n");
    }
    if(CurDevObj == gDevHDMIObj)
    {
        DBG_MSG("^Y===================================== HDMI Mode\r\n");
    }

    UI_LockEvent();

    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
//==================================================================== disp off
    GxVideo_DumpInfo();
    //change mode immediately
    GxVideo_CloseDevice(DOUT1);
//==================================================================== disp off

    AppView_Pause();

//==================================================================== disp on
    GxVideo_OpenDevice(DOUT1, (UINT32)CurDevObj, VideoMode);
    GxVideo_DumpInfo();
//==================================================================== disp on
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);

    AppView_ChangeVideoMode(0, (UINT32)CurDevObj, VideoMode);
    AppView_Resume();

    UI_ChangeVideoMode(0, (UINT32)CurDevObj, VideoMode); //LCD

    DBG_IND("^YChange mode ok\r\n");

    UI_UnlockEvent();

    //#NT#2012/11/5#Philex Lin - begin
    // restore CarDV current UI mode
    #if (UI_STYLE==UI_STYLE_DRIVER)
    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_CHANGEDSCMODE,1,DSCMODE_CHGTO_CURR);
    #else
    //restart current UI mode
    {
        UINT32 uiDscMode = UI_GetData(FL_ModeIndex);
        UINT32 uiOriPreMode;
        switch(uiDscMode)
        {
        case DSC_MODE_PHOTO_AUTO:
        case DSC_MODE_PHOTO_MANUAL:
        case DSC_MODE_PHOTO_SCENE:
            UI_Switch_DscMode(uiDscMode,DSCMODE_SWITCH_FORCE,DSCMODE_SWITCHDIR_DONT_CARE);
            break;
        case DSC_MODE_MOVIE:
            UI_Switch_DscMode(uiDscMode,DSCMODE_SWITCH_FORCE,DSCMODE_SWITCHDIR_DONT_CARE);
            break;
        case DSC_MODE_PLAYBACK:
            //#NT#2012/10/26#Ben Wang -begin
            //#NT#reserve original FL_PreMode
            uiOriPreMode = UI_GetData(FL_PreMode);
            UI_Switch_DscMode(uiDscMode,DSCMODE_SWITCH_FORCE,DSCMODE_SWITCHDIR_DONT_CARE);
            UI_SetData(FL_PreMode,uiOriPreMode);
            //#NT#2012/10/26#Ben Wang -end
            break;
        }
    }
    #endif
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, TRUE);

    return NVTEVT_CONSUME;
}



/////////////////////////////////////////////////////////////////////////////
#include "GxDisplay.h"
#include "GxImageJPG.h"
#if (UI_STYLE==UI_STYLE_DRIVER)
#include "UIFlow.h"
#else
#include "Demo_Logo.h"
#endif


extern UINT32   p_gLogoJPG;
extern UINT32   p_gLogoSize;

//extern BOOL gIsUSBChargePreCheck;
//DISPLAY_CFG disp_cfg = {0};


void Load_DispLogo(void)
{
    DBG_IND("Logo = Def Logo\r\n");
#if _MIPS_TODO
    UIRes_InitDefWP();

#if (USER_LOGO_FUNCTION == ENABLE)
    if( UI_GetData(FL_WallpaperStartupIndex) == 0)
    {
        DBG_IND("Logo = User Logo\r\n");
        DBG_IND("Read User Logo - begin\r\n");
        UIRes_InitReadWP_fast(WP_LOGO);
        DBG_IND("Read User Logo - end\r\n");
    }
#endif
#endif
}

void UI_ShowJPG(UINT8 LayerID, UINT32 uiJPGData, UINT32 uiJPGSize, UINT32 uiTempBuf)
{
    UINT16 w=0,h=0,fmt=0;
    UINT32 uiOutAddrY, uiOutAddrCb, uiOutAddrCr;
    ISIZE             DeviceSize;
    DeviceSize = GxVideo_GetDeviceSize(DOUT1);
    #if(VDO_USE_ROTATE_BUFFER == ENABLE)
    GX_IMAGE_PIXEL_FMT GxImgFMT = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
    IMG_BUF   DispImg = {0};
    IMG_BUF   DispOutImg = {0};
    UINT32    uiPoolAddr,uiPoolSize;

    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO2TEMP);
    uiPoolSize = OS_GetMempoolSize(POOL_ID_DISP_VDO2TEMP);
    #endif

#if 1
    Jpeg_DecodeInfo((const UINT8*)uiJPGData, uiJPGSize, &w,  &h, &fmt);
    DBG_IND("Splash data=%08x, size=%08x, w=%d, h=%d, fmt=%x\r\n", uiJPGData, uiJPGSize, w, h, fmt);
    Jpeg_GetImageAddr(w, h, fmt, (UINT8*)uiTempBuf, &uiOutAddrY, &uiOutAddrCb, &uiOutAddrCr);
    DBG_IND("output dataY=%08x, dataY=%08x, dataY=%08x\r\n", uiOutAddrY, uiOutAddrCb, uiOutAddrCr);
    Jpeg_DecodeImage((UINT8*)uiJPGData, uiJPGSize, (UINT8*)uiTempBuf, w*h*2);


    if(LayerID == LAYER_VDO1) //VDO1
    {
        #if(VDO_USE_ROTATE_BUFFER == ENABLE)
        if (fmt == PXLFMT_YUV422_PK)
            GxImg_InitBuf(&DispImg,w,h,GX_IMAGE_PIXEL_FMT_YUV422, GXIMAGE_LINEOFFSET_ALIGN(16),uiTempBuf,w*h*2);
        else
            GxImg_InitBuf(&DispImg,w,h,GX_IMAGE_PIXEL_FMT_YUV420, GXIMAGE_LINEOFFSET_ALIGN(16),uiTempBuf,w*h*2);
        GxImg_RotateData(&DispImg,uiPoolAddr,uiPoolSize,GX_IMAGE_ROTATE_90,&DispOutImg);
        Display_SetBuffer(LAYER_VDO1, h, w, h, PXLFMT_YUV420_PK, DispOutImg.PxlAddr[0], DispOutImg.PxlAddr[1], DispOutImg.PxlAddr[2]);
        Display_SetWindow(LAYER_VDO1, 0, 0, DeviceSize.h, DeviceSize.w);
        #else
        Display_SetBuffer(LAYER_VDO1, w, h, w, fmt, uiOutAddrY, uiOutAddrCb, uiOutAddrCr);
        Display_SetWindow(LAYER_VDO1, 0, 0, DeviceSize.w, DeviceSize.h);
        #endif
        Display_SetEnable(LAYER_VDO1, TRUE);
        Display_SetEnable(LAYER_VDO2, FALSE);
    }
    if(LayerID == LAYER_VDO2) //VDO2
    {
        Display_SetBuffer(LAYER_VDO2, w, h, w, fmt, uiOutAddrY, uiOutAddrCb, uiOutAddrCr);
        Display_SetWindow(LAYER_VDO2, 0, 0, DeviceSize.w, DeviceSize.h);
        Display_SetEnable(LAYER_VDO1, FALSE);
        Display_SetEnable(LAYER_VDO2, TRUE);
    }
#endif
}

void Display_ShowSplash(SPLASH_ID splash_id)
{
    UINT32  uiPoolAddr;

    //TM_Begin(DISP_SHOWLOGO);

    //get temp buffer
    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO2);
    if (uiPoolAddr == 0)
    {
        return;
    }

#if 1
    //load default logo
#if (UI_STYLE==UI_STYLE_DRIVER)
    if (splash_id == SPLASH_POWERON)
    {
        p_gLogoJPG = (UINT32)g_ucBGOpening;
        p_gLogoSize = Logo_getBGOpening_size();
    } else {
        p_gLogoJPG = (UINT32)g_ucBGGoodbye;
        p_gLogoSize = Logo_getBGGoodbye_size();
    }
#else
    if (splash_id == SPLASH_POWERON)
    {
        p_gLogoJPG = (UINT32)gDemo_Logo;
        p_gLogoSize = DEMO_LOGO_SIZE;
    }
    else
    {
        p_gLogoJPG = (UINT32)gDemoPwroff_Logo;
        p_gLogoSize = DEMO_PWROFFLOGO_SIZE;
    }
#endif
    //show logo
    DBG_IND("Show Logo\r\n");
    UI_ShowJPG(LOGO_DISP_LAYER, (UINT32)p_gLogoJPG, p_gLogoSize, uiPoolAddr);
#endif
    //TM_End(DISP_SHOWLOGO);
}

