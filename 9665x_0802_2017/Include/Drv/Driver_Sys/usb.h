/**
    Header file for USB device

    This file is the header file for USB device module driver

    @file       usb.h
    @ingroup    mIUSBDevice
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _USB_H
#define _USB_H

#include <Type.h>

#include "Driver.h"
#include "usb_desc.h"
#include "usb_define.h"
#include "usb_compat.h"  // for backward compatibility

/**
    @addtogroup mIUSBDevice
*/
//@{

/**
    @name   USB max dma length

    @note For usb_setEPRead(), usb_setEPWrite(), usb_readEndpoint(), usb_writeEndpoint()
*/
//@{
#define USB_MAX_DMA_LENGTH      0x1ffff         ///< Max dma length: 128KB-1
//@}

/**
    @name USB generic call back prototype

    @param[in] UINT32   event for this call back

    @return void
*/
//@{
typedef void (*USB_GENERIC_CB)(UINT32);
//@}

/**
    USB controller mode
*/
typedef enum
{
    USB_CTRL_MODE_DEVICE,                       ///< USB device mode
    USB_CTRL_MODE_HOST,                         ///< USB host mode

    ENUM_DUMMY4WORD(USB_CTRL_MODE)
} USB_CTRL_MODE;


/**
    Endpoint Block number used

    Describe endpoint receive/transmit data with manner of single/ping-pong/tripple buffer

    @note For BlkNum of USB_ENP_CONFIG
*/
typedef enum
{
    BLKNUM_NOT_USE,                             ///< block not used
    BLKNUM_SINGLE,                              ///< use single block
    BLKNUM_DOUBLE,                              ///< use double block
    BLKNUM_TRIPLE,                              ///< use triple block
    ENUM_DUMMY4WORD(USB_EP_BLKNUM)
} USB_EP_BLKNUM;

/**
    Endpoint direction

    @note For Direction of USB_ENP_CONFIG
*/
typedef enum
{
    EP_DIR_NOT_USE,                             ///< Endpoint direction not set
    EP_DIR_IN,                                  ///< Endpoint direction is IN
    EP_DIR_OUT,                                 ///< Endpoint direction is OUT
    ENUM_DUMMY4WORD(USB_EP_DIR)
} USB_EP_DIR;

/**
    Endpoint transfer type

    @note For TrnsferType of USB_ENP_CONFIG
*/
typedef enum
{
    EP_TYPE_NOT_USE,                            ///< Endpoint transfer type not set
    EP_TYPE_ISOCHRONOUS,                        ///< Endpoint transfer type is isochronous
    EP_TYPE_BULK,                               ///< Endpoint transfer type is bulk
    EP_TYPE_INTERRUPT,                          ///< Endpoint transfer type is interrupt
    ENUM_DUMMY4WORD(USB_EP_TYPE)
} USB_EP_TYPE;

/**
    transaction number for high bandwidth endpoint

    @note For HighBandWidth of USB_ENP_CONFIG
*/
typedef enum
{
    HBW_NOT,                                    ///< not high bandwidth
    HBW_1,                                      ///< one transaction
    HBW_2,                                      ///< two transaction
    HBW_3,                                      ///< three transaction
    ENUM_DUMMY4WORD(USB_EP_HBW)
} USB_EP_HBW;

/**
    USB controller state

    @note For usb_setControllerState(), usb_getControllerState()
*/
typedef enum
{
    USB_CONTROLLER_STATE_NONE,                  ///< USB controller state is none
    USB_CONTROLLER_STATE_RESET,                 ///< USB controller state is reset
    USB_CONTROLLER_STATE_SUSPEND,               ///< USB controller state is suspend
    ENUM_DUMMY4WORD(USB_CONTROLLER_STATE)
} USB_CONTROLLER_STATE;

/**
    USB End Point number
*/
typedef enum
{
    USB_EP0,                                    ///< USB End Point number 0
    USB_EP1,                                    ///< USB End Point number 1
    USB_EP2,                                    ///< USB End Point number 2
    USB_EP3,                                    ///< USB End Point number 3
    USB_EP4,                                    ///< USB End Point number 4
    USB_EP5,                                    ///< USB End Point number 5
    USB_EP6,                                    ///< USB End Point number 6
    USB_EP7,                                    ///< USB End Point number 7
    USB_EP8,                                    ///< USB End Point number 8
    USB_EP_NOT_USE = 0xF,
    ENUM_DUMMY4WORD(USB_EP)
} USB_EP;

//#NT#2011/03/23#HH Chuang -begin
//#NT#add TX driving adjustment
/**
    USB pad ID

    @note for usb_setDrivingSink(), usb_getDrivingSink()
*/
typedef enum
{
    USB_PAD_GROUP_FSLS,             ///< PAD setting for FS/LS circuit
    USB_PAD_GROUP_HS,               ///< PAD setting for HS circuit

    USB_PAD_GROUP_NUM,
    ENUM_DUMMY4WORD(USB_PAD_GROUP)
} USB_PAD_GROUP;

/**
    USB driving/sink level

    @note for usb_setDrivingSink(), usb_getDrivingSink()
*/
typedef enum
{
    USB_PAD_DRIVING_LV0,            ///< USB driving/sink level0: FS/LS 60uA, HS 18.66mA
    USB_PAD_DRIVING_LV1,            ///< USB driving/sink level1: FS/LS 80uA, HS 19.55mA (default value of high speed circuit)
    USB_PAD_DRIVING_LV2,            ///< USB driving/sink level2: FS/LS 100uA, HS 20.88mA (default value of full speed circuit)
    USB_PAD_DRIVING_LV3,            ///< USB driving/sink level3: FS/LS 120uA, HS 22.22mA

    USB_PAD_DRIVING_NUM,
    ENUM_DUMMY4WORD(USB_PAD_DRIVING)
} USB_PAD_DRIVING;

/**
    USB slew rate

    @note for usb_setDrivingSink(), usb_getDrivingSink()
*/
typedef enum
{
    USB_PAD_SLEW_LV0,               ///< USB slew rate level0: slowest
    USB_PAD_SLEW_LV1,               ///< USB slew rate level1 (default value of full speed circuit)
    USB_PAD_SLEW_LV2,               ///< USB slew rate level2
    USB_PAD_SLEW_LV3,               ///< USB slew rate level3: fastest

    USB_PAD_SLEW_NUM,
    ENUM_DUMMY4WORD(USB_PAD_SLEW)
} USB_PAD_SLEW;
//#NT#2011/03/23#HH Chuang -end

/**
    USB squelch level

    @note for usb_setConfig(), usb_getConfig() with USB_CONFIG_ID_SQUELCH_LEVEL
*/
typedef enum
{
    USB_SQUELCH_LEVEL_LV0,          ///< USB squelch level 0
    USB_SQUELCH_LEVEL_LV1,          ///< USB squelch level 1
    USB_SQUELCH_LEVEL_LV2,          ///< USB squelch level 2
    USB_SQUELCH_LEVEL_LV3,          ///< USB squelch level 3 (default value)
    USB_SQUELCH_LEVEL_LV4,          ///< USB squelch level 4
    USB_SQUELCH_LEVEL_LV5,          ///< USB squelch level 5
    USB_SQUELCH_LEVEL_LV6,          ///< USB squelch level 6
    USB_SQUELCH_LEVEL_LV7,          ///< USB squelch level 7

    ENUM_DUMMY4WORD(USB_SQUELCH_LEVEL)
} USB_SQUELCH_LEVEL;

/**
    USB swing level

    USB high speed swing level

    @note for usb_setConfig(), usb_getConfig() with USB_CONFIG_ID_HS_SWING
*/
typedef enum
{
    USB_SWING_LEVEL_LV0,            ///< USB swing level 0
    USB_SWING_LEVEL_LV1,            ///< USB swing level 1
    USB_SWING_LEVEL_LV2,            ///< USB swing level 2
    USB_SWING_LEVEL_LV3,            ///< USB swing level 3
    USB_SWING_LEVEL_LV4,            ///< USB swing level 4
    USB_SWING_LEVEL_LV5,            ///< USB swing level 5
    USB_SWING_LEVEL_LV6,            ///< USB swing level 6
    USB_SWING_LEVEL_LV7,            ///< USB swing level 7 (default value)

    ENUM_DUMMY4WORD(USB_SWING_LEVEL)
} USB_SWING_LEVEL;

/**
    USB reference current level

    USB reference current level

    @note for usb_setConfig(), usb_getConfig() with USB_CONFIG_ID_REF_CURRENT
*/
typedef enum
{
    USB_IREF_LEVEL_LV0,             ///< USB reference current level 0
    USB_IREF_LEVEL_LV1,             ///< USB reference current level 1
    USB_IREF_LEVEL_LV2,             ///< USB reference current level 2 (default)
    USB_IREF_LEVEL_LV3,             ///< USB reference current level 3

    ENUM_DUMMY4WORD(USB_IREF_LEVEL)
} USB_IREF_LEVEL;

/**
    Endpoint configuration

    @note For EPxConfig_HS and EPxConfig_FS of USB_MNG
*/
typedef  struct Endpoint_Config {
    BOOL           Enable;                      ///< enable this endpoint or not
                                                ///< - @b TRUE: this endpoint descriptor is enabled
                                                ///< - @b FALSE: this endpoint descriptor is disabled
    UINT32         BlkSize;                     ///< block size : inform usb controller allocate 512/1024 bytes block for this endpoint
                                                ///< - @b 512: MaxPktSize <= 512 bytes
                                                ///< - @b 1024: MaxPktSize > 512 bytes
    USB_EP_BLKNUM  BlkNum;                      ///< block number : single/double/triple
    USB_EP_DIR     Direction;                   ///< IN or OUT drection
    USB_EP_TYPE    TrnsferType;                 ///< bulk/interrupt/isochronous
    UINT32         MaxPktSize;                  ///< max packet size (unit: byte)
    USB_EP_HBW     HighBandWidth;               ///< transzction number for high bandwidth endpoint
} USB_ENP_CONFIG;


/**
    USB Management Infomation Table

    @note For global variable gUSBMng which should be correctly filled before usb_open()
*/
typedef struct {
    USB_DEVICE_DESC  *pDevDesc;                 ///< USB Device Descriptor ptr
    USB_CONFIG_DESC  *pConfigDesc;              //< USB Config Descriptor ptr
    USB_CONFIG_DESC  *pConfigOthersDesc;        //< USB Config Descriptor ptr
    USB_CONFIG_DESC  *pConfigDescHS;            ///< USB HighSpeed Descriptor
    USB_CONFIG_DESC  *pConfigDescFS;            ///< USB FullSpeed Descriptor
    USB_CONFIG_DESC  *pConfigDescFSOther;       ///< USB HighSpeed Descriptor
    USB_CONFIG_DESC  *pConfigDescHSOther;       ///< USB FullSpeed Descriptor
    USB_ENP_CONFIG   EPxConfig_HS[8];           ///< set Highspeed endpoint configuration
    USB_ENP_CONFIG   EPxConfig_FS[8];           ///< set Fullspeed endpoint configuration
    USB_STRING_DESC  *pStringDesc[4];           ///< USB String Descriptor ptr
    USB_DEVICE_DESC  *pDevQualiDesc;            ///< USB Qualifier ptr; High speed vs low speed.
    UINT16           bNumConfigurations;        ///< configuration number
    UINT16           bNumStrings;               ///< string number
    void             (*fpOpen_Needed_FIFO)(void);  ///< callback function to enable/disable FIFO
    void             (*fpEvent_CB)(UINT32 uEvent); ///< ISR callback function
    USB_DEV_TYPE     USBType;                   ///< USB_MSD, USB_SIDC, USB_PCC

    //For the USB device mode require to know device enter suspend mode.
    //If the USB device don't care suspend mode, set this callback function as NULL.
    FP               fUSB_SuspendFun;           ///< callback function pointer for suspend
} USB_MNG;


/**
    Interface and Endpoint Descriptor data

    (OBSOLETE)

    @note For pIf_Ep_Data of USB_CFG_IF_DESC_INTF
*/
typedef struct {
    USB_INTERFACE_DESC  InterfaceDesc;          ///< Interface Descriptor
    USB_ENDPOINT_DESC   *pEndpointDesc;         ///< pointer of EndpointDescriptor
} USB_IF_EP_DESC_DATA;


/**
    Interface of Configuration and Interface Descriptor

    (OBSOLETE)

    @note For pCfg_If_Intf of USB_CFG_IF_DESC_INTF
*/
typedef struct {
    UINT8               bInterfaceNumber;       ///< Interface Number
    UINT8               bNumAltSet;             ///< Number of AlternateSetting
    USB_IF_EP_DESC_DATA *pIf_Ep_Data;           ///< pointer of Interface and
                                                ///< EndpointDescriptor data
} USB_CFG_IF_DESC_INTF;

/**
    Configuration Descriptor data

    (OBSOLETE)

    @note Unkown usage
*/
typedef struct {
    USB_CONFIG_DESC       ConfigDesc;           ///< Config Descriptor
    USB_CFG_IF_DESC_INTF  *pCfg_If_Intf;        ///< interface between configuration and interface descriptor
} USB_CONFIG_DESC_DATA;

/**
    current configuration

    (OBSOLETE)

    @note Unkown usage
*/
typedef struct {
    UINT8   config;                             ///< current config number
    UINT8   alt[1];                             ///< current alternate settings
} USB_CONFIG;

/**
    device request

    @note For DeviceRequest of CONTROL_XFER
*/
typedef struct {
    UINT8       bmRequestType;                  ///< request type
    UINT8       bRequest;                       ///< request
    UINT16      wValue;                         ///< value of bRequest defined
    UINT16      wIndex;                         ///< value of bRequest defined
    UINT16      wLength;                        ///< data length of data stage
} USB_DEVICE_REQUEST;

/**
    control transfer

    @note For global variable ControlData which pass control transfer information between usb driver and class library
*/
typedef struct {
    UINT8          *pData;                      ///< data pointer to CX IN data
    USB_DEVICE_REQUEST DeviceRequest;           ///< device request
    UINT16         wLength;                     ///< length
    UINT16         reserved;                    ///< reserved
} CONTROL_XFER;

/**
    setup data

    (OBSOLETE)

    @note Unkown usage
*/
typedef struct {
    USB_DEVICE_REQUEST  DevReq;                                  ///< device request
    UINT8               SetupData[512];                          ///< received setup data
    UINT8               DevDescData[USB_DEV_LENGTH];             ///< device descriptor
    UINT8               CfgDescData[USB_CONFIG_DESC_TOTAL_SIZE]; ///< config descriptor
    UINT8               *pCfgDescData[USB_NUM_CONFIG];           ///< pointer of config descriptor
} USB_SETUP_DATA;

/**
    USB configuration identifier

    @note For usb_setConfig(), usb_getConfig()
*/
typedef enum
{
    USB_CONFIG_ID_TX_PWR_SAVING,            ///< Select TX power saving mode
                                            ///< Context can be:
                                            ///< - @b TRUE: enable power saving mode(Defalut value)
                                            ///< - @b FALSE: disable power saving mode
    USB_CONFIG_ID_STANDARD_CHARGER,         ///< Select standard charger support
                                            ///< Context can be:
                                            ///< - @b TRUE: USB standard charger (Default value)
                                            ///< - @b FALSE: non-standard charger
    USB_CONFIG_ID_SQUELCH_LEVEL,            ///< Select squelch level. Context can be USB_SQUELCH_LEVEL
                                            ///< - @b USB_SQUELCH_LEVEL_LV0
                                            ///< - @b USB_SQUELCH_LEVEL_LV1
                                            ///< - @b USB_SQUELCH_LEVEL_LV2
                                            ///< - @b USB_SQUELCH_LEVEL_LV3 (default)
                                            ///< - @b USB_SQUELCH_LEVEL_LV4
                                            ///< - @b USB_SQUELCH_LEVEL_LV5
                                            ///< - @b USB_SQUELCH_LEVEL_LV6
                                            ///< - @b USB_SQUELCH_LEVEL_LV7
    USB_CONFIG_ID_HS_SWING,                 ///< Select high speed TX Swing
                                            ///< Context can be:
                                            ///< - @b USB_SWING_LEVEL_LV0
                                            ///< - @b USB_SWING_LEVEL_LV1
                                            ///< - @b USB_SWING_LEVEL_LV2
                                            ///< - @b USB_SWING_LEVEL_LV3
                                            ///< - @b USB_SWING_LEVEL_LV4
                                            ///< - @b USB_SWING_LEVEL_LV5
                                            ///< - @b USB_SWING_LEVEL_LV6
                                            ///< - @b USB_SWING_LEVEL_LV7 (default)
    USB_CONFIG_ID_REF_CURRENT,              ///< Select reference current
                                            ///< Context can be:
                                            ///< - @b USB_IREF_LEVEL_LV0
                                            ///< - @b USB_IREF_LEVEL_LV1
                                            ///< - @b USB_IREF_LEVEL_LV2 (default)
                                            ///< - @b USB_IREF_LEVEL_LV3

    USB_CONFIG_ID_ENG_FOTG_TEST_MODE,       //< (Engineer Usage) Select FOTG IP test mode
                                            ///< Context can be:
                                            ///< - @b TRUE: enable test mode
                                            ///< - @b FALSE: disable test mode (Default value)

    ENUM_DUMMY4WORD(USB_CONFIG_ID)
} USB_CONFIG_ID;

/**
    USB call back list

    @note For usb_setCallBack()
*/
typedef enum
{
    USB_CALLBACK_CHARGING_EVENT,            ///< usb charging event call back (Designed for project layer)
                                            ///< (SUGGESTED to be invoked in system init phase)
                                            ///< The installed call back can receive following event:
                                            ///< - @b USB_CHARGE_EVENT_2P5MA
                                            ///< - @b USB_CHARGE_EVENT_100MA
                                            ///< - @b USB_CHARGE_EVENT_500MA
                                            ///< - @b USB_CHARGE_EVENT_5A
#if 1
    USB_CALLBACK_CX_VENDOR_REQUEST_0,       ///< usb control pipe vendor request 0 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 0
    USB_CALLBACK_CX_VENDOR_REQUEST_1,       ///< usb control pipe vendor request 1 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 1
    USB_CALLBACK_CX_VENDOR_REQUEST_2,       ///< usb control pipe vendor request 2 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 2
    USB_CALLBACK_CX_VENDOR_REQUEST_3,       ///< usb control pipe vendor request 3 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 3
    USB_CALLBACK_CX_VENDOR_REQUEST_4,       ///< usb control pipe vendor request 4 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 4
    USB_CALLBACK_CX_VENDOR_REQUEST_5,       ///< usb control pipe vendor request 5 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 5
    USB_CALLBACK_CX_VENDOR_REQUEST_6,       ///< usb control pipe vendor request 6 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 6
    USB_CALLBACK_CX_VENDOR_REQUEST_7,       ///< usb control pipe vendor request 7 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 7
    USB_CALLBACK_CX_VENDOR_REQUEST_8,       ///< usb control pipe vendor request 8 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 8
    USB_CALLBACK_CX_VENDOR_REQUEST_9,       ///< usb control pipe vendor request 9 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 9
    USB_CALLBACK_CX_VENDOR_REQUEST_10,      ///< usb control pipe vendor request 10 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 10
    USB_CALLBACK_CX_VENDOR_REQUEST_11,      ///< usb control pipe vendor request 11 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 11
    USB_CALLBACK_CX_VENDOR_REQUEST_12,      ///< usb control pipe vendor request 12 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 12
    USB_CALLBACK_CX_VENDOR_REQUEST_13,      ///< usb control pipe vendor request 13 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 13
    USB_CALLBACK_CX_VENDOR_REQUEST_14,      ///< usb control pipe vendor request 14 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 14
    USB_CALLBACK_CX_VENDOR_REQUEST_15,      ///< usb control pipe vendor request 15 call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
                                            ///< The installed call back will be invoked when vendor request is received
                                            ///< with (bRequest & 0xF) is 15
#endif
    USB_CALLBACK_CX_CLASS_REQUEST,          ///< usb control pipe class request call back (Designed for usb class)
                                            ///< The installed call back doesn't need to receive parameter
    USB_CALLBACK_SET_INTERFACE,             ///< usb set interface call back (Designed for project/class layer)
                                            ///< (SUGGESTED to be invoked in class init phase)
                                            ///< The installed call back can receive USB_SETINTERFACE_EVENT
    USB_CALLBACK_CX_VENDOR_REQUEST,         ///< usb control pipe vendor request call back (Designed for usb class)
                                            ///< The installed call back should process ControlData by itself
                                            ///< The installed call back will be invoked when vendor request is received


    ENUM_DUMMY4WORD(USB_CALLBACK)
} USB_CALLBACK;


/**
    USB set interface event

    @note For USB_CALLBACK_SET_INTERFACE
*/
typedef enum
{
    USB_SETINTERFACE_EVENT_INT00_ALT00,                     ///< interface 0/alternate 0 selected
    USB_SETINTERFACE_EVENT_INT00_ALT01,                     ///< interface 0/alternate 1 selected
    USB_SETINTERFACE_EVENT_INT00_ALT02,                     ///< interface 0/alternate 2 selected
    USB_SETINTERFACE_EVENT_INT00_ALT03,                     ///< interface 0/alternate 3 selected

    USB_SETINTERFACE_EVENT_INT01_ALT00 = 0x10000,           ///< interface 1/alternate 0 selected
    USB_SETINTERFACE_EVENT_INT01_ALT01,                     ///< interface 1/alternate 1 selected
    USB_SETINTERFACE_EVENT_INT01_ALT02,                     ///< interface 1/alternate 2 selected
    USB_SETINTERFACE_EVENT_INT01_ALT03,                     ///< interface 1/alternate 3 selected

    USB_SETINTERFACE_EVENT_INT02_ALT00 = 0x20000,           ///< interface 2/alternate 0 selected
    USB_SETINTERFACE_EVENT_INT02_ALT01,                     ///< interface 2/alternate 1 selected
    USB_SETINTERFACE_EVENT_INT02_ALT02,                     ///< interface 2/alternate 2 selected
    USB_SETINTERFACE_EVENT_INT02_ALT03,                     ///< interface 2/alternate 3 selected

    USB_SETINTERFACE_EVENT_INT03_ALT00 = 0x30000,           ///< interface 3/alternate 0 selected
    USB_SETINTERFACE_EVENT_INT03_ALT01,                     ///< interface 3/alternate 1 selected
    USB_SETINTERFACE_EVENT_INT03_ALT02,                     ///< interface 3/alternate 2 selected
    USB_SETINTERFACE_EVENT_INT03_ALT03,                     ///< interface 3/alternate 3 selected

    USB_SETINTERFACE_EVENT_INT04_ALT00 = 0x40000,           ///< interface 4/alternate 0 selected
    USB_SETINTERFACE_EVENT_INT04_ALT01,                     ///< interface 4/alternate 1 selected
    USB_SETINTERFACE_EVENT_INT04_ALT02,                     ///< interface 4/alternate 2 selected
    USB_SETINTERFACE_EVENT_INT04_ALT03,                     ///< interface 4/alternate 3 selected

    USB_SETINTERFACE_EVENT_INT05_ALT00 = 0x50000,           ///< interface 5/alternate 0 selected
    USB_SETINTERFACE_EVENT_INT05_ALT01,                     ///< interface 5/alternate 1 selected
    USB_SETINTERFACE_EVENT_INT05_ALT02,                     ///< interface 5/alternate 2 selected
    USB_SETINTERFACE_EVENT_INT05_ALT03,                     ///< interface 5/alternate 3 selected

    USB_SETINTERFACE_EVENT_INT06_ALT00 = 0x60000,           ///< interface 6/alternate 0 selected
    USB_SETINTERFACE_EVENT_INT06_ALT01,                     ///< interface 6/alternate 1 selected
    USB_SETINTERFACE_EVENT_INT06_ALT02,                     ///< interface 6/alternate 2 selected
    USB_SETINTERFACE_EVENT_INT06_ALT03,                     ///< interface 6/alternate 3 selected

    USB_SETINTERFACE_EVENT_INT07_ALT00 = 0x70000,           ///< interface 7/alternate 0 selected
    USB_SETINTERFACE_EVENT_INT07_ALT01,                     ///< interface 7/alternate 1 selected
    USB_SETINTERFACE_EVENT_INT07_ALT02,                     ///< interface 7/alternate 2 selected
    USB_SETINTERFACE_EVENT_INT07_ALT03,                     ///< interface 7/alternate 3 selected

    ENUM_DUMMY4WORD(USB_SETINTERFACE_EVENT)
} USB_SETINTERFACE_EVENT;


/**
    USB chargering event

    @note For USB_CALLBACK_CHARGING_EVENT
*/
typedef enum
{
    USB_CHARGE_EVENT_SUSPEND,                               ///< suspend
    USB_CHARGE_EVENT_DISCONNECT = USB_CHARGE_EVENT_SUSPEND, ///< disconnect (disable D+ pull up)
    USB_CHARGE_EVENT_2P5MA = USB_CHARGE_EVENT_SUSPEND,      ///< currently can draw up to 2.5mA
    USB_CHARGE_EVENT_CONNECT,                               ///< connect, but not configured (enable D+ pull up)
    USB_CHARGE_EVENT_100MA = USB_CHARGE_EVENT_CONNECT,      ///< currently can draw up to 100mA
    USB_CHARGE_EVENT_CONFIGURED,                            ///< connect and configured by host
    USB_CHARGE_EVENT_500MA = USB_CHARGE_EVENT_CONFIGURED,   ///< currently can draw up to 500mA
    USB_CHARGE_EVENT_CHARGER,                               ///< charger connected
    USB_CHARGE_EVENT_CHARGING_PC = USB_CHARGE_EVENT_CHARGER,///< connect to charging downstream port PC
    USB_CHARGE_EVENT_1P5A = USB_CHARGE_EVENT_CHARGING_PC,   ///< currently can draw up to 1.5A


    ENUM_DUMMY4WORD(USB_CHARGE_EVENT)
} USB_CHARGE_EVENT;

/**
    USB charger detect state

    @note For usb_chkCharger()
*/
typedef enum
{
    USB_CHARGER_STS_NONE,                               ///< No charger detected
    USB_CHARGER_STS_STANDARD_PC = USB_CHARGER_STS_NONE, ///< Standard PC
    USB_CHARGER_STS_CHARGING_DOWNSTREAM_PORT,           ///< Charging downstream port
    USB_CHARGER_STS_CHARGING_PC = USB_CHARGER_STS_CHARGING_DOWNSTREAM_PORT, ///< Charger capable PC

    USB_CHARGER_STS_CHARGER,                            ///< Charger detected

    USB_CHARGER_STS_DISCONNECT = 0x8000,                ///< USB cable is removed when charger detection is applied

    ENUM_DUMMY4WORD(USB_CHARGER_STS)
} USB_CHARGER_STS;


/**
    gUSBMng

    USB management information
    This variable should be correctly filled before usb_open() is invoked.
*/
extern USB_MNG  gUSBMng;

/**
    ControlData

    Control Transfer information

    - If SETUP token is received, usb driver may pass ControlData to class library for vendor control transfer
    - If class library needs return CX_IN data, it should fill ControlData then invoke usb_RetSetupData()
*/
extern CONTROL_XFER  ControlData;

extern FP *gfpVendorRequest;
extern FP gfpClassRequest;


// USB Driver API, in <usb.c>
extern ER usb_open(void);
extern USB_CHARGER_STS usb_chkCharger(UINT32 uiDebounce);
extern void usb_PowerOnInit(BOOL bHighSpeed);
extern BOOL usb_StateChange(void);
extern ER usb_close(void);
extern void usb_setVbusi(BOOL bPlug);
extern void usb_SetStandByWakeup(DRV_CB fpIsr);
extern USB_CONTROLLER_STATE usb_getControllerState(void);

extern ER usb_setDrivingSink(USB_PAD_GROUP usbPad, USB_PAD_DRIVING driving);
extern ER usb_getDrivingSink(USB_PAD_GROUP usbPad, USB_PAD_DRIVING* driving);
extern ER usb_setSlewRate(USB_PAD_GROUP usbPad, USB_PAD_SLEW rate);
extern ER usb_getSlewRate(USB_PAD_GROUP usbPad, USB_PAD_SLEW* rate);

extern BOOL usb_chkConfiguration(void);
extern ER usb_setConfig(USB_CONFIG_ID configID, UINT32 configContext);
extern UINT32 usb_getConfig(USB_CONFIG_ID configID);
extern ER usb_setCallBack(USB_CALLBACK callBackID, USB_GENERIC_CB pCallBack);

// USB read /write functions, in <usb_EPConfig.c>
extern void usb_RetSetupData(void);
extern ER usb_readEndpoint(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);
extern ER usb_writeEndpoint(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);
extern ER usb_abortEndpoint(USB_EP EPn);
extern ER usb_setEPRead(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);
extern ER usb_setEPWrite(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);
extern ER usb_waitEPDone(USB_EP EPn, UINT32 *pDMALen);
// USB endpoint and FIFO setting functions
extern void usb_maskEPINT(USB_EP EPn);
extern void usb_unmaskEPINT(USB_EP EPn);
extern void usb_setEP0Done(void);
extern void usb_setEP0Stall(void);
extern void usb_setEPinStall(USB_EP EPn);
extern void usb_setEPoutStall(USB_EP EPn);
extern void usb_clrEPFIFO(USB_EP EPn);
extern void usb_clrFIFO(void);
extern void usb_setTX0Byte(USB_EP EPn);
extern UINT32 usb_getEPByteCount(USB_EP EPn);
extern BOOL usb_chkEPEmpty(USB_EP EPn);
extern BOOL usb_chkEPBusy(USB_EP EPn);
extern UINT32 usb_getSOF(void);

// USB misc functions, in <usb_misc.c>
extern void usb_EnableHighSpeed(BOOL bHighSpeed);
extern BOOL usb_isHighSpeedEnabled(void);
void usb_SetDevDescUSBVerRelNum(BOOL Speed, UINT16 RelNum);

// USB internal task, in <usb_ClsDrvTsk.c>
extern void USBCtlStartTsk(void);
extern void USBSetupHalTsk(void);

extern void usb_regClsReqHdlr(FP usbClsReqHdlr);//#NT#2010/02/26#Lily Kao

//@}

#endif // _USB_H
