////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeUsbPTP.h"
#include "UIAppUsbPrint.h"
//UIControl
#include "UIFrameworkExt.h"
#if (UI_STYLE==UI_STYLE_DEMO)
#include "MenuPC.h"
#endif
#include "DeviceCtrl.h"

int PRIMARY_MODE_USBSIDC = -1;    ///< USB SIDC/PTP

void ModeUsbPTP_Open(void);
void ModeUsbPTP_Close(void);

void ModeUsbPTP_Open(void)
{
    INT32 retV;

    Input_ResetMask();
    DevCtrl_ModeUSBSIDC();
    USB_PlugInSetFunc();
    //Ux_SetActiveApp(&CustomPrintObjCtrl);

    //#NT#2010/12/13#Lily Kao -begin
    retV = Ux_SendEvent(&CustomPrintObjCtrl, NVTEVT_EXE_OPEN, 0);
    if (retV != E_TMOUT)
    {
#if (UI_STYLE==UI_STYLE_DEMO)
        Ux_OpenWindow((VControl *)(&MenuPCCtrl), 1, PRIMARY_MODE_USBSIDC);
#endif
    }
    else
    {
        debug_msg("PTPOpen:%d\r\n",retV);
    }
    //#NT#2010/12/13#Lily Kao -end


}
void ModeUsbPTP_Close(void)
{
#if (UI_STYLE==UI_STYLE_DEMO)
    Ux_CloseWindow((VControl *)(&MenuPCCtrl), 0);
#endif
    Ux_SendEvent(&CustomPrintObjCtrl, NVTEVT_EXE_CLOSE, 0);//Ux_GetActiveApp()
}

SYS_MODE gModeUsbPTP =
{
    "USBPTP",
    ModeUsbPTP_Open,
    ModeUsbPTP_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
