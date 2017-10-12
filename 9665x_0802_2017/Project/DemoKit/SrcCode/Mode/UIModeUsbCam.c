////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeUsbCam.h"
#include "UIAppUsbCam.h"
//UIControl
#include "UIFrameworkExt.h"

#include "DeviceCtrl.h"
#if _DEMO_TODO
#include "USBPCC.h"
#endif
#if (UI_STYLE==UI_STYLE_DRIVER)
#include "UIFlowWndUSB.h"
#include "UIFlowWndUSBAPI.h"
#endif
int PRIMARY_MODE_USBPCC = -1;     ///< USB PC camera

void ModeUsbCam_Open(void);
void ModeUsbCam_Close(void);

void ModeUsbCam_Open(void)
{

    Input_ResetMask();
    USB_PlugInSetFunc();
    DevCtrl_ModeUSBPCC();
    //Ux_SetActiveApp(&CustomUSBPCCObjCtrl);
    Ux_SendEvent(&CustomUSBPCCObjCtrl, NVTEVT_EXE_OPEN, 0);

    #if (UI_STYLE==UI_STYLE_DRIVER)
    Ux_OpenWindow((VControl *)(&UIFlowWndUSBCtrl), 1, UIFlowWndUSB_PCC_MODE);
    #else
    //Ux_OpenWindow((VControl *)(&MenuPCCtrl), 1, PRIMARY_MODE_USBPCC);
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_SLEEP, TRUE);
    #endif

}
void ModeUsbCam_Close(void)
{
    Ux_SendEvent(&CustomUSBPCCObjCtrl, NVTEVT_EXE_CLOSE, 0);
    DevCtrl_ModeUSBPCC_Remove();
}

SYS_MODE gModeUsbCam =
{
    "USBCAM",
    ModeUsbCam_Open,
    ModeUsbCam_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
