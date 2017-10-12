/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIDef.h
    @ingroup    mIPRJAPUIFlow

    @brief      UIDef Functions
                This file is the user interface ( for LIB callback function).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef NVT_USB_API_H
#define NVT_USB_API_H

#include "GxLib.h"

//for usb
extern void USB_CB(UINT32 event, UINT32 param1, UINT32 param2);


/**
    The Callback function for PictB_RegCBFunction(USIDCPTR_CB CB).

    This callback funtion will post the event of #_NVT_USBPRINT_EVENT.

    @param[in] PrintInfo the callback event
*/
extern void PrinterFlowCBFunc(UINT8 PrintInfo);

#endif//NVT_USB_API_H

