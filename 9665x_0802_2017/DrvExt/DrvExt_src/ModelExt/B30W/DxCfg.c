#include "DrvExt.h"
#include "Dx.h"
#include "DxCommon.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//IO
/*
[PinmuxCfg.c]
pinmux

[GpioMapping.c]
cgpio x32 (Storage)
pgpio x64 (Peripheral)
sgpio x30 (SENSOR)
lgpio x28 (LCD)
dgpio x9 (Dedicated)
pwm x20
pwm_ccnt x3

[VoltageDet.c]
adc x8

[SerialComm.c]
i2c x2 x2
sif x1 x6
csi x2 x4
lvds x1 x10
spi x3
uart x2
*/

extern DX_OBJECT gDevLCD;
extern DX_OBJECT gDevTV;
extern DX_OBJECT gDevHDMI;
extern DX_OBJECT gDevNAND0;
extern DX_OBJECT gDevNAND1;
extern DX_OBJECT gDevNAND3;
extern DX_OBJECT gDevCARD1;

DX_HANDLE Dx_GetObject(UINT32 DxClassType)  // Query device object
{
    UINT32 hDevice = 0;

    switch(DxClassType & DX_CLASS_MASK)
    {
    //external device
    case DX_CLASS_DISPLAY_EXT:
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_LCD)
            hDevice = (UINT32)(&gDevLCD);
        else
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_TVOUT)
            hDevice = (UINT32)(&gDevTV);
        else
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_HDMIOUT)
            hDevice = (UINT32)(&gDevHDMI);
        break;
    case DX_CLASS_AUDIO_EXT:
        break;
    case DX_CLASS_SENSOR_EXT:
        break;
    case DX_CLASS_STORAGE_EXT:
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_NAND0)
            hDevice = (UINT32)(&gDevNAND0);
        else
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_NAND1)
            hDevice = (UINT32)(&gDevNAND1);
        else
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_NAND3)
            hDevice = (UINT32)(&gDevNAND3);
        else
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_CARD1)
            hDevice = (UINT32)(&gDevCARD1);
        break;
    case DX_CLASS_POWER_EXT:
        break;
    case DX_CLASS_INPUT_EXT:
        break;
    case DX_CLASS_STATUS_EXT:
        break;
    }

    if(hDevice == 0)
    {
        DBG_ERR("[Dx] GetObject %08x failed\r\n", DxClassType);
    }
    return (DX_HANDLE)hDevice;
}



