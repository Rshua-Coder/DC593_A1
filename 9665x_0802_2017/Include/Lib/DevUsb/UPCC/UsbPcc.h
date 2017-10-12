/**
    UVC device class Header File

    This is the UVC device class header file

    @file       USBPCC.h
    @ingroup    mILibUsbPCC
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _USBPCC_H
#define _USBPCC_H


#include "Type.h"
#include "usb_desc.h"
#include "usb_define.h"

/**
    @addtogroup mILibUsbPCC
*/
//@{

/**
    USB PCC support max. size
*/
typedef enum {
    PCC_MAX_SIZE_VGA,       ///< UPCC max. size VGA
    PCC_MAX_SIZE_HD         ///< UPCC max. size HD
} PCC_MAX_SIZE;

/**
    USB PCC support data format
*/
typedef enum {
    UPCC_VIDEO_FORMAT_422,  ///< UPCC support format 422
    UPCC_VIDEO_FORMAT_420,  ///< UPCC support format 420
    ENUM_DUMMY4WORD(UPCC_VIDEO_FORMAT)
}UPCC_VIDEO_FORMAT;


//for LSC PC calibration
typedef void   (*UVC_SETCAPRAWCB)(UINT32 *rawAddr, UINT32 *rawWidth, UINT32 *rawHeight, UINT32 *rawBitDepth, UINT32 *jpgAddr, UINT32 *jpgSize);
typedef void   (*UVC_SETCAPCB)(UINT32 *jpgAddr, UINT32 *jpgSize);

typedef UINT32 (*UVC_STARTVIDEOCB)(UINT32 videoWidth, UINT32 videoHeight, UPCC_VIDEO_FORMAT videoFmt);
typedef UINT32 (*UVC_GETVIDEOBUFCB)(UINT32 *yAddr, UINT32 *cbAddr, UINT32 *crAddr);
typedef UINT32 (*UVC_STOPVIDEOCB)(void);

typedef struct {

    USB_STRING_DESC     *pManuStringDesc;    ///< USB Manufacturer String Descriptor ptr
    USB_STRING_DESC     *pProdStringDesc;    ///< USB Product String Descriptor ptr
    USB_STRING_DESC     *pSerialStringDesc;  ///< USB Serial Number String Descriptor ptr
    UINT16              VID;                 ///< Customized USB Vendor ID
    UINT16              PID;                 ///< Customized USB Product ID
    UINT32              UsbPccMemAdr;        ///< Buffer address for USB operation
    UINT32              UsbPccMemSize;       ///< Buffer size for USB operation
    //#NT#2009/07/09#Chris Chung -begin
    //#NT#add to control PCC vendor function
    BOOL                bEnableVendorFunc;   ///< Enable vendor function
    //#NT#2009/07/09#Chris Chung -end
    //#NT#2011/01/05#Hideo Lin -begin
    //#NT#For PCC maximum size setting
    PCC_MAX_SIZE        PccMaxSize;          ///< Max. support size
    //#NT#2011/01/05#Hideo Lin -end

    //******************************************************************************
    // Flash CallBack, Feb 10, 2006 Added.
    //******************************************************************************
    UINT32    (*BurnIn)(UINT32 BlockID, UINT8 *Buf, UINT32 Length);   ///< CallBack Function for Nane Burn-In
    UINT32    (*IsFinish)(void);                                      ///< CallBack Function for Check Ready
    UINT32    (*FlashRead)(UINT32 BlockID, UINT32 Offset, UINT32 *Size, UINT8 *RBuf);      ///< CallBack Function for flash read

    //#NT#2008/08/14#Irene -begin
    //#NT#CallBack Function for Vendor Request
    // New:
    // 1. Add a callback function for vendor request
    UINT32    (*VendorProcess)(UINT8 bReguest, UINT16 wValue, UINT16 wIndex, UINT16 wLength, UINT8 *bBuf); ///< CallBack Function for vendor request
    //#NT#2008/08/14#Irene -end

    UVC_STARTVIDEOCB    UPccStartVideo;     ///< CallBack Function for start IPP
    UVC_GETVIDEOBUFCB   UPccGetVideoBuf;    ///< CallBack Function for get video buffer address
    UVC_STOPVIDEOCB     UPccStopVideo;      ///< CallBack Function for stop IPP
} USB_PCC_INFO;


extern ER       Uvc_Open(USB_PCC_INFO *pClassInfo);
extern void     Uvc_Close(void);
//extern void     Uvc_VideoTsk(void);

//for LSC PC calibration
extern void     Uvc_RegSetCapRawCB(UVC_SETCAPRAWCB fp);
extern void     Uvc_RegSetCapCB(UVC_SETCAPCB fp);

extern ER       Uvc_RegVendorCmd(UINT8 startReq, UINT8 endReq);

extern void     Uvc_InstallID(void) _SECTION(".kercfg_text");

//@}


//
//  Backward compatible function, prepare to Delete
//
#define UPccOpen(pClassInfo)                      Uvc_Open(pClassInfo)
#define UPccClose()                               Uvc_Close()
#define USBPccRegSetPCC2CapRawCB(fp)              Uvc_RegSetCapRawCB(fp)
#define USBPccRegSetPCC2CapCB(fp)                 Uvc_RegSetCapCB(fp)
#define USBPccRegisterVendorCmd(startReq, endReq) Uvc_RegVendorCmd(startReq, endReq)
#define UPccRegisterVendorCmd(startReq, endReq)   Uvc_RegVendorCmd(startReq, endReq)

#define UPccStartVideoCB(videoWidth, videoHeight, videoFmt) UVC_STARTVIDEOCB(videoWidth, videoHeight, videoFmt)
#define UPccGetVideoBufCB(yAddr, cbAddr, crAddr)(yAddr, cbAddr, crAddr)  UVC_GETVIDEOBUFCB(yAddr, cbAddr, crAddr)
#define UPccStopVideoCB()                         UVC_STOPVIDEOCB()
#define FPSETPCC2CAPRAWCB(rawAddr, rawWidth, rawHeight, rawBitDepth, jpgAddr, jpgSize) UVC_SETCAPRAWCB(rawAddr, rawWidth, rawHeight, rawBitDepth, jpgAddr, jpgSize)
#define FPSETPCC2CAPCB(jpgAddr, jpgSize)          UVC_SETCAPCB(jpgAddr, jpgSize)

#define Install_UvcID()         Uvc_InstallID()

#endif  // _USBPCC_H

