/*
    Header file for USB driver backward compatibility

    Header file for USB driver backward compatibility.

    @file       usb_compat.h
    @ingroup    mIUSBDevice
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _USB_COMPAT_H
#define _USB_COMPAT_H

#include "Type.h"

/**
    @addtogroup mIUSBDevice
*/
//@{

//-------------------------------------------------
// USB Compatible driver API
//-------------------------------------------------
#define USBStateChange()            usb_StateChange()
#define USBPowerOnInit(bHighSpeed)  usb_PowerOnInit(bHighSpeed)
//@}
#endif //_USB_COMPAT_H