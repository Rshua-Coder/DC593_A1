////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeUsbCharge.h"
#include "UIAppUsbCharge.h"
//UIControl
#include "UIFrameworkExt.h"
#if (UI_STYLE==UI_STYLE_DEMO)
#include "MenuCharger.h"
#endif
#define UI_SHOW_CHARGER_PERIOD     5000  //ms


int PRIMARY_MODE_USBCHARGE = -1;    ///< USB CHARGE

void ModeUsbCharge_Open(void);
void ModeUsbCharge_Close(void);

void ModeUsbCharge_Open(void)
{

    Input_ResetMask();
    USB_PlugInSetFunc();
    Ux_SetActiveApp(&CustomUSBChargeObjCtrl);
    Ux_SendEvent(&CustomUSBChargeObjCtrl, NVTEVT_EXE_OPEN, 0);
#if (UI_STYLE==UI_STYLE_DEMO)
    Ux_OpenWindow((VControl *)(&MenuChargerCtrl), 1, UI_SHOW_CHARGER_PERIOD);
#endif
}
void ModeUsbCharge_Close(void)
{
    Ux_SendEvent(&CustomUSBChargeObjCtrl, NVTEVT_EXE_CLOSE, 0);
#if (UI_STYLE==UI_STYLE_DEMO)
    Ux_CloseWindow((VControl *)(&MenuChargerCtrl), 0);
#endif
}

SYS_MODE gModeUsbCharge =
{
    "USBCHARGE",
    ModeUsbCharge_Open,
    ModeUsbCharge_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
