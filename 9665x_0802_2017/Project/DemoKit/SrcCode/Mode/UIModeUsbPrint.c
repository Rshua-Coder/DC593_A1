////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIPrinterObj.h"
#include "UIAppUsbPrint.h"
#include "UIModeUsbPrint.h"
//UIControl
#include "UIFrameworkExt.h"
#if (UI_STYLE==UI_STYLE_DEMO)
#include "MenuPrintMain.h"
#endif
#include "DeviceCtrl.h"
#include "UIBackgroundObj.h"
int PRIMARY_MODE_PRINTER = -1;    ///< USB Pictbridge/Printer

void ModeUsbPrint_Open(void);
void ModeUsbPrint_Close(void);

void ModeUsbPrint_Open(void)
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
        Ux_OpenWindow((VControl *)(&MenuPrintMainCtrl), 0);
#endif
    }
    else
    {
        debug_msg("PrintOpen:%d\r\n",retV);
    }
    //#NT#2010/12/13#Lily Kao -end
}
void ModeUsbPrint_Close(void)
{
    //#NT#2010/11/09#Lily Kao -begin
    //stop waiting printer
    BackgroundWait_PrintModeIsClosed(TRUE);
    //#NT#2010/11/09#Lily Kao -end
    Ux_SendEvent(&CustomPrintObjCtrl, NVTEVT_EXE_CLOSE, 0);
#if (UI_STYLE==UI_STYLE_DEMO)
    Ux_CloseWindow((VControl *)(&MenuPrintMainCtrl), 0);
#endif

}


SYS_MODE gModeUsbPrint =
{
    "USBPRINT",
    ModeUsbPrint_Open,
    ModeUsbPrint_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
