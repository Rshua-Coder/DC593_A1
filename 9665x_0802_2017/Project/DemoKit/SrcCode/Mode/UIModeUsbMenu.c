////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
//UIControl
#include "UIFrameworkExt.h"
#include "DeviceCtrl.h"
#if (UI_STYLE==UI_STYLE_DRIVER)
#include "UIMenuWndUSB.h"
#endif

int PRIMARY_MODE_USBMENU = -1;

void ModeUsbMenu_Open(void);
void ModeUsbMenu_Close(void);

void ModeUsbMenu_Open(void)
{
#if (UI_STYLE==UI_STYLE_DRIVER)
    Input_ResetMask();
    Ux_OpenWindow(&UIMenuWndUSBCtrl, 0);
#endif
}
void ModeUsbMenu_Close(void)
{
#if (UI_STYLE==UI_STYLE_DRIVER)
    Ux_CloseWindow(&UIMenuWndUSBCtrl, 0);
#endif
}

SYS_MODE gModeUsbMenu =
{
    "USBMenu",
    ModeUsbMenu_Open,
    ModeUsbMenu_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
