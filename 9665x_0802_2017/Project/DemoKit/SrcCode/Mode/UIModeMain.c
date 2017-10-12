////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeMain.h"
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

//UIWnd
//#include "UIMainWnd.h"
//UIInfo
#include "UIInfo.h"

#if (_CALIBRATION_MODE_ == ENABLE)
#include "EngineerMode.h"
#endif

int PRIMARY_MODE_MAIN = -1;      ///< Main

void ModeMain_Open(void);
void ModeMain_Close(void);

static void Main_PauseDisp(BOOL bWaitFinish)
{
    return;
}
static void Main_ResumeDisp(BOOL bWaitFinish)
{
    return;
}
VIEW_SRC gMain_src=
{
    TRUE, //BYPASS DISP-SRV, DIRECT OUTPUT TO DISPLAY
    DISPSRV_USER_HANDLE_PRIMARY,
    NULL,
    NULL,
    NULL,
    Main_PauseDisp,
    Main_ResumeDisp,
};

void ModeMain_Open(void)
{

    Ux_SetActiveApp(&UISetupObjCtrl);
    Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);


    #if (_CALIBRATION_MODE_ == ENABLE)
    if (EngineerMode_CheckEng())
    {
        EngineerMode_Open();

        // check if enter engineer mode
        if (IsEngineerModeOpened())
        {
            EngineerMode_WndOpen();
        }
    }
    #endif

    AppView_ConfigSource(&gMain_src);
    //open
    AppView_Open();

    //Ux_OpenWindow((VControl *)(&UIMainWndCtrl), 0);
}
void ModeMain_Close(void)
{
    //Ux_CloseWindowClear((VControl *)(&UIMainWndCtrl), 0);

    #if (_CALIBRATION_MODE_ == ENABLE)
    {
        // check if enter engineer mode
        if (IsEngineerModeOpened())
        {
            EngineerMode_WndClose();
        }
    }
    #endif

    AppView_Close();

    Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);
}

SYS_MODE gModeMain =
{
    "MAIN",
    ModeMain_Open,
    ModeMain_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
