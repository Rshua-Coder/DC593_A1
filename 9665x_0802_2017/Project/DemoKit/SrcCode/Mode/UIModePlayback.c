////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIAppPlay.h"
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

#if _DEMO_TODO
#include "PlaybackTsk.h"
#include "UIModePlayback.h"
#include "Display.h"
#include "Disp_rotate.h"
#include "DpofVendor.h"
#endif
#if _DEMO_TODO
#include "WavStudioTsk.h"
#endif
//UIWnd
#include "FlowPlay.h"
#if (UI_STYLE==UI_STYLE_DRIVER)
#include "UIFlow.h"
#endif
#include "UIModeWiFi.h"
//UIInfo
#include "UIPlayInfo.h"
#include "UIInfo.h"

#include "DeviceCtrl.h"
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
#include "UIGraphics.h"
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
#include "MediaPlayAPI.h"

#define  USE_ROT_TASK 0


int PRIMARY_MODE_PLAYBACK = -1;      ///< Playback

void ModePlay_Open(void);
void ModePlay_Close(void);
#if _DEMO_TODO
// Slide effect speed table
static UINT8 SlideSpeedTable[PBX_SLIDE_EFFECT_IMAGE_STEP] = {0};
#endif

void ModePlay_Open(void)
{

    Input_ResetMask();

//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
    // Control CCD/LCD power on/off, lens, shutter...
    //TM_Begin(DEVCTRL_CCD);
    DevCtrl_ModePlayback();
    //TM_End(DEVCTRL_CCD);

    // Init playback app and open
    Ux_SetActiveApp(&CustomPlayObjCtrl);
    //TM_Begin(APP_OPEN);
    Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);
    //TM_End(APP_OPEN);

    //TM_Begin(UI_OPENWIN);
    //#NT#2009/12/29#Lincy Lin -begin
    UI_SetData(FL_ModeIndex, UI_GetData(FL_NextMode));
    //#NT#2009/12/29#Lincy Lin -end
    //#NT#2010/12/14#Ben Wang -begin
    //#NT#add i key playback mode
    #if (_IKEY_DATEVIEW_MODE_== ENABLE)
    //#NT#2011/01/11#Ben Wang -begin
    //add parameter to specify opening flow
    if(UI_GetData(FL_PlayIKeyModeIndex) == PLAY_BY_DATE)
        Ux_OpenWindow((VControl *)(&FlowPlayDateCtrl), 1, TRUE);
    //#NT#2011/01/11#Ben Wang -end
    else
    #endif
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        UI_OpenWifiWnd();
    }
    else
    {
#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_OpenWindow((VControl *)(&UIFlowWndPlayCtrl), 0);
#else
        Ux_OpenWindow((VControl *)(&FlowPlayCtrl), 0);
#endif
    }
    //#NT#2010/12/14#Ben Wang -end
    //TM_End(UI_OPENWIN);

    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, TRUE);

    //TM_End("OFF_TO_CAPTURE");
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
}
void ModePlay_Close(void)
{
    //#NT#2010/11/29#Ben Wang -begin
    //#NT#Close root win by function instead of by win name because FlowPlayCtrl may not always be the root win.
#if (UI_STYLE!=UI_STYLE_DRIVER)
    VControl *p2ndWndCtrl = 0;
#endif

    //Ux_CloseWindowClear((VControl *)(&FlowPlayCtrl), 0);
    //Ux_CloseWindow((VControl *)(&FlowPlayCtrl), 0);
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
         UI_CloseWifiWnd();
    }
    else
    {

#if (UI_STYLE==UI_STYLE_DRIVER)
        Ux_CloseWindowClear((VControl *)(&UIFlowWndPlayCtrl), 0);
#else

        Ux_GetRootWindow(&p2ndWndCtrl);
        Ux_CloseWindow(p2ndWndCtrl,0);
#endif
    }
    //set default play by name
    //if(PRIMARY_MODE_PLAYBACK != System_GetState(SYS_STATE_NEXTMODE))
    //    UI_SetData(FL_PlayIKeyModeIndex, PLAY_BY_NAME);
    //#NT#2010/11/29#Ben Wang -end
    Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);

}


SYS_MODE gModePlay =
{
    "PLAYBACK",
    ModePlay_Open,
    ModePlay_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
