////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeUsbDisk.h"
#include "UIAppUsbDisk.h"
//UIControl
#include "UIFrameworkExt.h"

#include "DeviceCtrl.h"
#if (UI_STYLE==UI_STYLE_DEMO)
#include "MenuPC.h"
#include "MenuUSBCharge.h"
#endif
#if (UI_STYLE==UI_STYLE_DRIVER)
#include "UIFlowWndUSB.h"
#include "UIFlowWndUSBAPI.h"
#endif


int PRIMARY_MODE_USBMSDC = -1;    ///< USB MSDC

void ModeUsbDisk_Open(void);
void ModeUsbDisk_Close(void);

void ModeUsbDisk_Open(void)
{
    INT32 retV;

    Input_ResetMask();
#if (POWERON_TESTFLOW == ENABLE)
    if(!gIsUSBChargePreCheck)
    {
    DevCtrl_ModeUSBMSDC();
    }
#else
    DevCtrl_ModeUSBMSDC();
#endif
    USB_PlugInSetFunc();

    //Ux_SetActiveApp(&CustomMSDCObjCtrl);

    //#NT#2010/12/13#Lily Kao -begin
    retV = Ux_SendEvent(&CustomMSDCObjCtrl, NVTEVT_EXE_OPEN, 0);
    if (retV != E_TMOUT)
    {
#if (UI_STYLE==UI_STYLE_DEMO)
#if (POWERON_TESTFLOW == ENABLE)
    if(!gIsUSBChargePreCheck)
    {
    Ux_OpenWindow((VControl *)(&MenuPCCtrl), 1, PRIMARY_MODE_USBMSDC);
    }
#else
    Ux_OpenWindow((VControl *)(&MenuPCCtrl), 1, PRIMARY_MODE_USBMSDC);
#endif
#elif (UI_STYLE==UI_STYLE_DRIVER)
    Ux_OpenWindow((VControl *)(&UIFlowWndUSBCtrl), 1, UIFlowWndUSB_MSDC_MODE);
#endif
    }
    else
    {
        debug_msg("MSDCOpen TO:%d\r\n",retV);
#if (UI_STYLE==UI_STYLE_DEMO)
        Ux_OpenWindow((VControl *)(&MenuUSBChargeCtrl), 1, PRIMARY_MODE_USBMSDC);
#endif
    }
    //#NT#2010/12/13#Lily Kao -end
#if _DEMO_TODO
    pLastStrgDev = GxStrg_GetDevice(0);
//#NT#2009/07/01#Brad Chen -end
    //#NT#2010/12/28#Jeah Yen -begin
    if(pLastStrgDev == NULL)
    {
        //PowerOn_CB() POWERON_CB_SXOPEN 沒有提早啟動File System
        DBG_DUMP("MSDC: Wait for FS start...\r\n");
        FileSys_WaitFinish();

        //update
        pLastStrgDev = GxStrg_GetDevice(0);
    }
    //#NT#2010/12/28#Jeah Yen -end


    //usb_enableTestMode(FALSE);

    //#NT#2010/12/13#Lily Kao -begin
    AppInit_ModeUSBMSDC();
#endif
}
void ModeUsbDisk_Close(void)
{
    Ux_SendEvent(&CustomMSDCObjCtrl, NVTEVT_EXE_CLOSE, 0);
#if (UI_STYLE==UI_STYLE_DEMO)
    Ux_CloseWindow((VControl *)(&MenuPCCtrl), 0);
#endif
#if _DEMO_TODO
    Msdc_Close();
    GxStrg_OpenDevice(0, pLastStrgDev);
#endif
}

SYS_MODE gModeUsbDisk =
{
    "USBDISK",
    ModeUsbDisk_Open,
    ModeUsbDisk_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
