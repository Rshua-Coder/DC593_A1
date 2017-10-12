/**
    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.

    @file       DxUSB.c
    @ingroup    mIDxUSB

    @brief      Detect USB connection status.

    @date       2013/04/30
*/

#include "UsbDevDef.h"
#include "usb.h"
#include "DxUSB.h"
#include "Delay.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxUSB
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

BOOL DxUSB_GetIsUSBPlug(void)
{
    return (usb_StateChange());
}

UINT32 DxUSB_UpdateConnectType(void)
{
    USB_CHARGER_STS RetUSB;
    UINT32  uiUSBCurrType;
    UINT32  uiTimeOutCnt = 0;

    if(!DxUSB_GetIsUSBPlug())
    {
        uiUSBCurrType = USB_CONNECT_NONE;
    }
    else
    {
        //#NT#2014/10/16#KS Hung -begin
        #if 1
        uiTimeOutCnt = 0;
        do
        {
            RetUSB = usb_chkCharger(0);
            if (RetUSB == USB_CHARGER_STS_CHARGER)
            {
                break;
            }
            else
            {
                Delay_DelayMs(100);
            }
        }while ((uiTimeOutCnt++) <= 20);
        #else
        RetUSB = usb_chkCharger(0);
        #endif
        //#NT#2014/10/16#KS Hung -end

        switch (RetUSB)
        {
            case USB_CHARGER_STS_NONE:
                DBG_DUMP("CONNECT to PC\r\n");
                uiUSBCurrType = USB_CONNECT_PC;
                break;
            case USB_CHARGER_STS_CHARGING_DOWNSTREAM_PORT:
                DBG_DUMP("CONNECT to Charging PC\r\n");
                uiUSBCurrType = USB_CONNECT_CHARGING_PC;
                break;
            case USB_CHARGER_STS_CHARGER:
                DBG_DUMP("CONNECT to CHARGER\r\n");
                uiUSBCurrType = USB_CONNECT_CHARGER;
                break;
            default:
                DBG_ERR("CONNECTION UNKNOWN!\r\n");
                uiUSBCurrType = USB_CONNECT_UNKNOWN;
                break;
        }
    }
    return uiUSBCurrType;
}

