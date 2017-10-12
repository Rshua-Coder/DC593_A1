////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeMovie.h"
#include "UIAppMovie.h"
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

//UIWnd
#if (UI_STYLE==UI_STYLE_DRIVER)
#include "UIFlow.h"
#else
#include "FlowMovie.h"
#endif
#include "UIModeWiFi.h"

//UIInfo
#include "UIInfo.h"

//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
#include "UIGraphics.h"
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
////////////////////////////////////////////////////////////////////////////////
#include "DeviceCtrl.h"

#if (_CALIBRATION_MODE_ == ENABLE)
#include "EngineerMode.h"
#endif

#define __MODULE__          UIModeMovie
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
extern INT32 MovieExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern BOOL g_bPreviewFirstTime;
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end

int PRIMARY_MODE_MOVIE = -1;      ///< Movie

//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
void FlowMovie_Open_FastSensor(void)
{
    //speed up: 提前做change mode時sensor要做的事情
    g_bPreviewFirstTime = TRUE; //turn on fast path
    //TM_Begin(APP_OPEN);
    // Init Movie app and open
    MovieExe_OnOpen(0, 0, 0);
    //TM_End(APP_OPEN);

    //TM_Begin(DEVCTRL_CCD);
    DevCtrl_ModeMovie_FastSensor();
    //TM_End(DEVCTRL_CCD);
    DevCtrl_ModePhoto_WaitFinish();
}

void FlowMovie_Open_FastLens(void)
{
    //TM_Begin(DEVCTRL_LENS);
    //Lens_OnSystem(LENS_CMD_POWERON_ADVANCE);
    DevCtrl_Lens_PowerOn_Advance(); //simple path

    //speed up: 提前做change mode時lens要做的事情
    g_bPreviewFirstTime = TRUE; //turn on fast path
    DevCtrl_ModeMovie_FastLens();
    //TM_End(DEVCTRL_LENS);
}
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end

//#NT#2016/05/20#Hideo Lin -begin
//#NT#Check sensor version
extern INT32 Cal_GetSensorVersion(void);
BOOL ModeMovie_CheckSensorVersion(void)
{
#if (_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_)

    INT32   iSensorVer;
    CHAR    cMessage[40] = {0};

    iSensorVer = Cal_GetSensorVersion();

    if ((iSensorVer != 3) && (iSensorVer != 4) && (iSensorVer != 5))
    {
        UI_ClearOSD(CLRID_IDX_BLUE);
        snprintf(cMessage, sizeof(cMessage), "Wrong sensor version: %d", (int)iSensorVer);
        UI_DrawOsdString(cMessage, 45, 70, CLRID_IDX_WHITE, FALSE);
        UI_DrawOsdString("Press any key to turn off!", 45, 100, CLRID_IDX_WHITE, FALSE);
        while (1)
        {
            if (GxKey_GetData(GXKEY_NORMAL_KEY) & 0xffffffff)
            {
                break;
            }
        }
        Ux_PostEvent(NVTEVT_KEY_POWER, 1, NVTEVT_KEY_PRESS);
        return FALSE;
    }

#endif
    return TRUE;
}
//#NT#2016/05/20#Hideo Lin -end

void ModeMovie_Open(void);
void ModeMovie_Close(void);

void ModeMovie_Open(void)
{
    Input_ResetMask();

    // Init movie app and open
    Ux_SetActiveApp(&CustomMovieObjCtrl);
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
    if(g_bPreviewFirstTime)
    {
        //skip
        //GxSystem_Mark("APP-SKIP\r\n");
    }
    else
    {
        //TM_Begin(DEVCTRL_CCD);
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModeMovie();
        //TM_End(DEVCTRL_CCD);
        //TM_Begin(APP_OPEN);
        // Init Photo app and open
        Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);
        //TM_End(APP_OPEN);
    }

    //#NT#2016/05/20#Hideo Lin -begin
    //#NT#Check sensor version
    //if (ModeMovie_CheckSensorVersion() == FALSE)
    //    return;
    //#NT#2016/05/20#Hideo Lin -end

    if(!g_bPreviewFirstTime)
    {
        DevCtrl_ModeMovie_WaitFinish();
    }

    //TM_Begin(UI_OPENWIN);
    UI_SetData(FL_ModeIndex, UI_GetData(FL_NextMode));
    /*
    //disable video1
    UI_Show(UI_SHOW_PREVIEW, TRUE);
    //enable video2
    UI_Show(UI_SHOW_QUICKVIEW, FALSE);
    */

    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        UI_OpenWifiWnd();
    }
    #if (_CALIBRATION_MODE_ == ENABLE)
    else if (IsEngineerModeOpened())
    {
        EngineerMode_WndOpen();
    }
    #endif
    else
    {
        gMovData.State = MOV_ST_VIEW;
        Ux_OpenWindow((VControl *)(&UIFlowWndMovieCtrl), 0);
    }
    //TM_End(UI_OPENWIN);

    g_bPreviewFirstTime = FALSE; //turn off fast path
    //TM_End(OFF_TO_CAPTURE);
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
}
void ModeMovie_Close(void)
{
    UINT32 i;

    //Ux_CloseWindowClear((VControl *)(&FlowMovieCtrl), 0);
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        UI_CloseWifiWnd();
    }
    #if (_CALIBRATION_MODE_ == ENABLE)
    else if (IsEngineerModeOpened())
    {
        EngineerMode_WndClose();
    }
    #endif
    else
    {
        Ux_CloseWindow((VControl *)(&UIFlowWndMovieCtrl), 0);
    }

    //#NT#2010/12/13#Photon Lin -begin
    //#Modify flow of DIS
    #if (MOVIE_DIS == ENABLE)
    Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_MOVIEDIS_ENABLE,1,0);
    #endif
    //#NT#2010/12/13#Photon Lin -end

    {
        for(i=0; i<500; i++)
        {
            if (!BKG_GetTskBusy())
            {
                break;
            }
            SwTimer_DelayMs(100);
        }
        if (i==500)
        {
            debug_err(("^R Wait background task idle timeout \r\n"));
        }
    }
    //#NT#2011/03/30#Photon Lin -end
    Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);
}

void FlowMovie_OnChangeSize(ISIZE *pSize, ISIZE *pOldSize);

SYS_MODE gModeMovie =
{
    "MOVIE",
    ModeMovie_Open,
    ModeMovie_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
