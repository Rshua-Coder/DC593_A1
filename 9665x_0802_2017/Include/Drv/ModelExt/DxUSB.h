/**
    DxInput module

    Physical layer to detect USB connection status. User should implement these APIs before using GxUSB.

    @file       DxUSB.h
    @ingroup    mIDxUSB

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _DXUSB_H_
#define _DXUSB_H_

#include "type.h"

/**
    @addtogroup mIDxUSB
*/
//@{

/**
    Get USB current status which is plugged in or unplugged.

    @return
         - @b TRUE: USB is plugged.
         - @b FALSE: USB is unplugged.
*/
extern BOOL DxUSB_GetIsUSBPlug(void);

/**
    Update USB connection type.

    Force to update connection type immediately.

    @return USB connection type, refer to USB connection type.
*/
extern UINT32 DxUSB_UpdateConnectType(void);
//@}
#endif //_DXUSB_H_