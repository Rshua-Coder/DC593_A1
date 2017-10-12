/**
    Device/Driver Types

    The definition for Dx group APIs.

    @file       DxType.h
    @ingroup    mDX

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/


#ifndef _DX_TYPE_H
#define _DX_TYPE_H

#include "Type.h"

/**
     @addtogroup mDX
*/
//@{

#define DX_CLASS_MASK           0xffff0000 ///< the mask of DX_CLASS used bit

#define DX_CLASS_NULL           0x00000000 ///< unknown dx class
#if 0 //internal driver not be used currently.
//internal devices
#define DX_CLASS_SYSTEM         0x00010000
#define DX_CLASS_GRPH2D         0x00020000
#define DX_CLASS_CODECJPEG      0x00030000
#define DX_CLASS_CODECH264      0x00040000
#define DX_CLASS_DISPLAY        0x00050000
#define DX_CLASS_AUDIO          0x00060000
#define DX_CLASS_STORAGE        0x00070000
#define DX_CLASS_USB            0x00080000
#define DX_CLASS_SENSOR         0x00090000
#endif

//external devices
#define DX_CLASS_DISPLAY_EXT    0x00150000 ///< used in Dx_GetObject to get a class of display object like LCD,TV,HDMI
#define DX_CLASS_AUDIO_EXT      0x00160000 ///< used in Dx_GetObject to get a class of audio object (not in use,currently)
#define DX_CLASS_STORAGE_EXT    0x00170000 ///< used in Dx_GetObject to get a class of storage object like NAND,SPI,SD
#define DX_CLASS_USB_EXT        0x00180000 ///< used in Dx_GetObject to get a class of USB object (not in use,currently)
#define DX_CLASS_SENSOR_EXT     0x00190000 ///< used in Dx_GetObject to get a class of sensor object (not in use,currently)
#define DX_CLASS_POWER_EXT      0x001a0000 ///< used in Dx_GetObject to get a class of power object (not in use,currently)
#define DX_CLASS_INPUT_EXT      0x001b0000 ///< used in Dx_GetObject to get a class of input object (not in use,currently)
#define DX_CLASS_STATUS_EXT     0x001c0000 ///< used in Dx_GetObject to get a class of status object (not in use,currently)

#define DX_TYPE_MASK            0x0000ffff ///< the mask of DX_TYPE used bit

/**
     @name display ext types
*/
//@{
#define DX_TYPE_LCD             0x00000000 ///< used in DX_CLASS_DISPLAY_EXT to get LCD object
#define DX_TYPE_TVOUT           0x00000001 ///< used in DX_CLASS_DISPLAY_EXT to get TV object
#define DX_TYPE_HDMIOUT         0x00000002 ///< used in DX_CLASS_DISPLAY_EXT to get HDMI object
//@}

#if 0 //udio ext types not be used currently.
//audio ext types
#define DX_TYPE_SPEAKER         0x00000000
#define DX_TYPE_LINEOUT         0x00000001
#endif

/**
     @name storage ext types
*/
//@{
#define DX_TYPE_NAND0           0x00000000 ///< used in DX_CLASS_STORAGE_EXT to get 1st nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND1           0x00000001 ///< used in DX_CLASS_STORAGE_EXT to get 2nd nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND2           0x00000002 ///< used in DX_CLASS_STORAGE_EXT to get 3rd nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND3           0x00000003 ///< used in DX_CLASS_STORAGE_EXT to get 4th nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND4           0x00000004 ///< used in DX_CLASS_STORAGE_EXT to get 5th nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND5           0x00000005 ///< used in DX_CLASS_STORAGE_EXT to get 6th nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND6           0x00000006 ///< used in DX_CLASS_STORAGE_EXT to get 7th nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND7           0x00000007 ///< used in DX_CLASS_STORAGE_EXT to get 8th nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND8           0x00000008 ///< used in DX_CLASS_STORAGE_EXT to get 9th nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND9           0x00000009 ///< used in DX_CLASS_STORAGE_EXT to get 10th nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND10          0x0000000a ///< used in DX_CLASS_STORAGE_EXT to get 11th nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND11          0x0000000b ///< used in DX_CLASS_STORAGE_EXT to get 12th nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND12          0x0000000c ///< used in DX_CLASS_STORAGE_EXT to get 13th nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND13          0x0000000d ///< used in DX_CLASS_STORAGE_EXT to get 14th nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND14          0x0000000e ///< used in DX_CLASS_STORAGE_EXT to get 15th nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_NAND15          0x0000000f ///< used in DX_CLASS_STORAGE_EXT to get 16th nand partition mapping to DxStorage_Nand.c
#define DX_TYPE_CARD0           0x00000010 ///< used in DX_CLASS_STORAGE_EXT to get 1st card device mapping to DxStorage_Nand.c
#define DX_TYPE_CARD1           0x00000011 ///< used in DX_CLASS_STORAGE_EXT to get 2nd card device mapping to DxStorage_Nand.c
#define DX_TYPE_CARD2           0x00000012 ///< used in DX_CLASS_STORAGE_EXT to get 3rd card device mapping to DxStorage_Nand.c
#define DX_TYPE_CARD3           0x00000013 ///< used in DX_CLASS_STORAGE_EXT to get 4th card device mapping to DxStorage_Nand.c
#define DX_TYPE_RAMDISK0        0x00000020 ///< used in DX_CLASS_STORAGE_EXT to get 1st ramdisk device mapping to DxStorage_Nand.c
#define DX_TYPE_RAMDISK1        0x00000021 ///< used in DX_CLASS_STORAGE_EXT to get 2nd ramdisk device mapping to DxStorage_Nand.c
#define DX_TYPE_RAMDISK2        0x00000022 ///< used in DX_CLASS_STORAGE_EXT to get 3rd ramdisk device mapping to DxStorage_Nand.c
#define DX_TYPE_RAMDISK3        0x00000023 ///< used in DX_CLASS_STORAGE_EXT to get 4th ramdisk device mapping to DxStorage_Nand.c
#define DX_TYPE_OUTLED          0x00000030 ///< used in DX_CLASS_STATUS_EXT
#define DX_TYPE_OUTIRCUT        0x00000031 ///< used in DX_CLASS_STATUS_EXT
//@}

#if 0 //usb ext types not be used currently.
#define DX_TYPE_USBPC           0x00000000
#define DX_TYPE_CHARGER         0x00000001
#endif


#if 0 //power ext types not be used currently.
#define DX_TYPE_BATTERY         0x00000000
#define DX_TYPE_DCIN            0x00000001
#endif

typedef struct _DX_SIGN
{
    UINT32 sign; //Signature
}
DX_SIGN, *PDX_SIGN;

typedef PDX_SIGN DX_HANDLE; ///< DX object handle to operate any Dx APIs

//@}
#endif //_DX_TYPE_H
