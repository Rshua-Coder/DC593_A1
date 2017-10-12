/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       UVAC.h
    @ingroup    mISYSUVAC

    @brief      USB PCC device class API header.
                USB PCC device class API header.

*/

#ifndef _UVAC_H
#define _UVAC_H

/*-----------------------------------
    Includes
-----------------------------------*/
#include "Type.h"



#define UVAC_1UVC_1UAC              0
#define UVAC_2UVC_2UAC              (UVAC_1UVC_1UAC + 1)
#define UVAC_UVC_UAC_DEV_SET        UVAC_1UVC_1UAC   //UVAC_2UVC_2UAC  //
#if (UVAC_UVC_UAC_DEV_SET >  UVAC_1UVC_1UAC)   //UVAC_2UVC_2UAC  //
#define UVAC_WINUSB_INTF            DISABLE
#define UVC_VEND_BULKIN             DISABLE
#else
#define UVAC_WINUSB_INTF            DISABLE   //ENABLE   //DISABLE
#if UVAC_WINUSB_INTF
#define UVC_VEND_BULKIN             ENABLE   //DISABLE
#else
#define UVC_VEND_BULKIN             DISABLE
#endif
#define USB_STAND_IF_LENGTH         0x09
#define USB_WINUSB_IF_ALT0          0
#define USB_WINUSB_IF_CLASS_CODE    0xFF  //14
#define USB_WINUSB_IF_SUBCLASS_CODE 0     //2
#define USB_WINUSB_IF_PROTOCOL      0
#define USB_WINUSB_IF_EP_CNT        1
#define USB_WINUSB_IF_EP_ADDR       USB_EP_EP7I_ADDRESS    //0x87
//0:video-control-1; 1:video-stream-1
//2:video-control-2; 3:video-stream-2
//4:AOF WinUSB
//4:audio-control-1, 5:audio-stream-1
//6:audio-control-2, 7:audio-stream-2
#define USB_WINUSB_IF_IDX           4
#define USB_WINUSB_PROTOCOL         0
#define USB_WINUSB_IF_STRING_IDX    0

#define USB_WINUSB_STAND_ENDPOINT_LENGTH       0x07
#define USB_WINUSB_ENDPOINT_ATTR               USB_EP_ATR_BULK
#define USB_WINUSB_EP_PACKET                   0x200
#define USB_WINUSB_INTERVAL          USB_EP_BULK_INTERVAL

#endif

#define UVAC_VID_RESO_MAX_CNT       12


#if (UVAC_UVC_UAC_DEV_SET > UVAC_1UVC_1UAC)
#define UVC_METHOD3          DISABLE  //==>this limitation is for <= 650
#define UVC_METHOD3_CMDTEST  DISABLE //ENABLE
#else
#if UVAC_WINUSB_INTF
#define UVC_METHOD3          ENABLE   //must be enable
#else
#define UVC_METHOD3          DISABLE   //ENABLE
#endif
#define UVC_METHOD3_CMDTEST  DISABLE //ENABLE
#endif

#if (ENABLE == UVC_METHOD3_CMDTEST)
#define UVAC_TEST_CMD_CAPIMG_SIZE   0xA00000  //10M
#else
#define UVAC_TEST_CMD_CAPIMG_SIZE   0
#endif

#define UVAC_VENDREQ_IQ_MASK        0xE0    //0xE0 ~ 0xFF
#define UVAC_VENDREQ_CUST_MASK      0xC0    //0xC0 ~ 0xDF

#define SET_CUR                             0x01
#define GET_CUR                             0x81
#define GET_MIN                             0x82
#define GET_MAX                             0x83
#define GET_RES                             0x84
#define GET_LEN                             0x85
#define GET_INFO                            0x86
#define GET_DEF                             0x87


/**
    \addtogroup mISYSUVAC
@{
*/

/**
    Video format for UVC, UV packed only.
*/
typedef enum _UVAC_VIDEO_FORMAT
{
    UVAC_VIDEO_FORMAT_422,                 ///< YCbCr 422, UV packed.
    UVAC_VIDEO_FORMAT_420,                 ///< YCbCr 420, UV packed.
    ENUM_DUMMY4WORD(UVAC_VIDEO_FORMAT)
} UVAC_VIDEO_FORMAT;

/**
    Video frame triggered type.
*/
typedef enum _UVAC_TRIG_GETVID_TYPE
{
    UVAC_TRIG_GETVID_TYPE_AUD,              ///< Video frame trigged by audio driver.
    UVAC_TRIG_GETVID_TYPE_TIMER,            ///< Video frame trigged by timer.
    UVAC_TRIG_GETVID_TYPE_OTHER,            ///< Reserved.
    ENUM_DUMMY4WORD(UVAC_TRIG_GETVID_TYPE)
}UVAC_TRIG_GETVID_TYPE;

/**
    UVAC configuration identifier.

    This definition is used in UVAC_SetConfig() to assgin new configuration to specified ID function.
*/
typedef enum _UVAC_CONFIG_ID
{
    UVAC_CONFIG_H264_TARGET_SIZE,           ///< H.264 stream target size(Byte/Sec). Default value is 0x100000.
    UVAC_CONFIG_H264_3DNR_LEVEL,            ///< H.264 3DNR settings (0 to 7). 0 is disabled and 7 is strongest. Default value is 0.
    UVAC_CONFIG_MJPG_TARGET_SIZE,           ///< Motion JPEG stream target size(Byte/Sec). Default value is 0x300000.
    UVAC_CONFIG_VID_TRIG_TYPE,              ///< Video trigger type, refer to UVAC_TRIG_GETVID_TYPE. Default value is UVAC_TRIG_GETVID_TYPE_AUD.
    UVAC_CONFIG_VENDOR_CALLBACK,            ///< The callback function for USB Vendor Request
    UVAC_CONFIG_VENDOR_IQ_CALLBACK,         ///< The callback function for USB Vendor IQ Request
    UVAC_CONFIG_EU_VENDCMDCB_START,         ///< The callback function for USB Vendor Command of Extention Unit, idx0 no use
    UVAC_CONFIG_EU_VENDCMDCB_ID01,          ///> Id1 is Reserved for IQ Vendor
    UVAC_CONFIG_EU_VENDCMDCB_ID02,          ///>
    UVAC_CONFIG_EU_VENDCMDCB_ID03,
    UVAC_CONFIG_EU_VENDCMDCB_ID04,
    UVAC_CONFIG_EU_VENDCMDCB_ID05,
    UVAC_CONFIG_EU_VENDCMDCB_ID06,
    UVAC_CONFIG_EU_VENDCMDCB_ID07,
    UVAC_CONFIG_EU_VENDCMDCB_ID08,
    #if UVAC_WINUSB_INTF
    UVAC_CONFIG_WINUSB_CB,
    #endif
    UVAC_CONFIG_EU_VENDCMDCB_END,
    ENUM_DUMMY4WORD(UVAC_CONFIG_ID)
} UVAC_CONFIG_ID;

#define UVAC_EU_VENDCMD_CNT     (UVAC_CONFIG_EU_VENDCMDCB_END - UVAC_CONFIG_EU_VENDCMDCB_START)
/**
    UVAC encode type.
*/
typedef enum _UVAC_CODEC_TYPE_
{
    UVAC_CODEC_TYPE_MJPG,
    UVAC_CODEC_TYPE_H264,
    UVAC_CODEC_TYPE_MAX
} UVAC_CODEC_TYPE;

/**
    UVAC H264 Target Byte Rate Configuration

    This definition is used in UVAC_SetConfig() to set the value of UVAC_CONFIG_H264_TARGET_SIZE.
*/
typedef enum _UVAC_H264_TBR_
{
    UVAC_H264_TBR_1M,                      ///< Set H.264 TBR to be 1M bytes.
    UVAC_H264_TBR_1_5_M,                   ///< Set H.264 TBR to be 1.5M bytes.
    UVAC_H264_TBR_2M,                      ///< Set H.264 TBR to be 2M bytes.
    ENUM_DUMMY4WORD(UVAC_H264_TBR)
} UVAC_H264_TBR;

/**
    Callback functions prototype that UVAC task used.

    These definitions are used in the structure UVAC_INFO and would be used during UVAC_Open() to specify the
    UVAC_Task needed information.
*/
//@{
typedef UINT32 (*UVAC_STARTVIDEOCB)(UINT32 VideoWidth, UINT32 VideoHeight, UVAC_VIDEO_FORMAT VideoFmt); ///< Callback for starting video.
//typedef UINT32 (*UVAC_GETVIDEOBUFCB)(UINT32 *yAddr, UINT32 *cbAddr, UINT32 *crAddr, UINT32 *lineOfsY);  ///< Callback for getting video buffer.
typedef UINT32 (*UVAC_GETVIDEOBUFCB)(UINT32 *yAddr, UINT32 *cbAddr, UINT32 *crAddr, UINT32 *lineOfsY, UVAC_CODEC_TYPE encType);  ///< Callback for getting video buffer.
//typedef UINT32 (*UVAC_STOPVIDEOCB)(void);
typedef UINT32 (*UVAC_STOPVIDEOCB)(UINT32 isClosed);
typedef UINT32 (*UVAC_CHGVIDEORESO)(UINT32 VideoWidth, UINT32 VideoHeight, UVAC_CODEC_TYPE encType); ///< Callback for changing video resolution.
///< Callback for stopping video.
typedef UINT8 (*UVAC_EUVENDCMDCB)(UINT32 ControlCode, UINT8 CS, UINT8 *pData, UINT32 *pDataLen);
#if UVAC_WINUSB_INTF
typedef UINT8 (*UVAC_WINUSBCLSREQCB)(UINT32 ControlCode, UINT8 CS, UINT8 *pData, UINT32 *pDataLen);
#endif

//@}

/**
    Structure used for UVAC Device Vendor Request Callback API.

    This structure is used in the callback prototype UVAC_VENDOR_REQ_CB.
*/
typedef struct
{
    BOOL      bHostToDevice;      ///< Data direction.
                                  ///< - @b TRUE: USB Host sent data to USB Device.
                                  ///< - @b FALSE: USB Host will get data from USB Device.
    UINT8     uiReguest;          ///< The bRequest field in USB Device Requests.
    UINT16    uiValue;            ///< The wValue field in USB Device Requests.
    UINT16    uiIndex;            ///< The wIndex field in USB Device Requests.
    UINT32    uiDataAddr;         ///< Data address.
    UINT32    uiDataSize;         ///< Data size, in byte, maximun is 64. If there is no data to transfer, this should be zero.
} UVAC_VENDOR_PARAM, *PUVAC_VENDOR_PARAM;

/**
    Callback function prototype for UVAC Device Vendor Request.
    @return
     - @UINT32 0:  Ok.
     - @UINT32 1,2,...: TBD.
*/
typedef UINT32 (*UVAC_VENDOR_REQ_CB)(PUVAC_VENDOR_PARAM pParam);




/**
    String Descriptor
*/
typedef  struct _UVAC_STRING_DESC
{
    UINT8       bLength;                ///< size of String Descriptor
    UINT8       bDescriptorType;        ///< String Descriptor type
    UINT8       bString[254];           ///< UNICODE stirng
} UVAC_STRING_DESC;

/**
    Information needed for UVAC module.

    This structure is used in UVAC_Open() to specify the UVAC task needed information for normal working
    and user customization callback.
*/
typedef struct {

    UVAC_STRING_DESC    *pManuStringDesc;    ///< USB Manufacturer String Descriptor ptr
    UVAC_STRING_DESC    *pProdStringDesc;    ///< USB Product String Descriptor ptr
    UVAC_STRING_DESC    *pSerialStringDesc;  ///< USB Serial Number String Descriptor ptr
    UINT16              VID;                 ///< Customized USB Vendor ID
    UINT16              PID;                 ///< Customized USB Product ID
    UINT32              UvacMemAdr;          ///< Buffer address for USB operation
    UINT32              UvacMemSize;         ///< Buffer size for USB operation
    UVAC_STARTVIDEOCB   fpStartVideoCB;      ///< Callback function to start video
    UVAC_GETVIDEOBUFCB  fpGetVideoBufCB;     ///< Callback function to get video buffer
    UVAC_STOPVIDEOCB    fpStopVideoCB;       ///< Callback function to stop video buffer
    UVAC_CHGVIDEORESO   fpChgVideoReso;
} UVAC_INFO;

/**
    Video Resolution.

    This structure is used in UVAC_ConfigVidReso(..) to set supported video resolution, and
    it must be set before UVAC_Open() is called.
*/
typedef struct _UVAC_VID_RESO_
{
    UINT32 width;
    UINT32 height;
}UVAC_VID_RESO, *PUVAC_VID_RESO;

/**
    Open UVAC(USB VIDEO/AUDIO-Device-Class) module.

    Open UVAC(USB VIDEO/AUDIO-Device-Class) module. Before using this API to open UVAC task, the user must use UVAC_InstallID()
    to retrieve the system task ID and the flag ID for UVAC Task usage to keep the task normal working.

    @param[in] pClassInfo Information needed for opening UVAC Task. The user must prepare all the information needed.

    @return
     - @b E_OK:  The UVAC Task open done and success.
     - @b E_SYS: Buffer allocation failed.
     - @b E_PAR: Parameter error.
*/
extern UINT32   UVAC_Open(UVAC_INFO *pClassInfo);

/**
    Close UVAC module.
*/
extern void     UVAC_Close(void);

/**
    Get needed buffer size for UVAC module.
*/
extern UINT32 UVAC_GetNeedMemSize(void);

/**
    Set UVAC configuration

    Assign new configuration of the specified ConfigID.

    @param[in] ConfigID         Configuration identifier
    @param[in] Value            Configuration context for ConfigID
*/
extern void UVAC_SetConfig(UVAC_CONFIG_ID ConfigID, UINT32 Value);

/**
     Install UVAC task,flag and semaphore id

     This is a default setting API. Users can create their own task id, semphore id,
     flag id to adjust own stack size and task priority as the following:

     @code
     {
        #define PRI_UVACVIDEO          10
        #define PRI_UVCISOIN           (PRI_UVACVIDEO + 2)
        #define PRI_UACISOIN           (PRI_UVACVIDEO + 1)
        #define STKSIZE_UVACVIDEO           2048
        #define STKSIZE_UVCISOIN            1024
        #define STKSIZE_UACISOIN            1024

        UINT32 FLG_ID_UVAC = 0;
        UINT32 UVACVIDEOTSK_ID = 0;
        UINT32 UVACISOINTSK_ID = 0;
        UINT32 UACISOINTSK_ID = 0;
        extern void UVAC_VideoTsk(void);
        extern void UVAC_VideoTsk2(void);
        extern void UVAC_IsoInTsk(void);
        void UVAC_InstallID(void)
        {
            OS_CONFIG_FLAG(FLG_ID_UVAC);
            OS_CONFIG_TASK(UVACVIDEOTSK_ID,   PRI_UVACVIDEO,  STKSIZE_UVACVIDEO,  UVAC_VideoTsk);
            OS_CONFIG_TASK(UVACVIDEOTSK_ID2,   PRI_UVACVIDEO,  STKSIZE_UVACVIDEO,  UVAC_VideoTsk2);
            OS_CONFIG_TASK(UVACISOINTSK_ID,   PRI_UVCISOIN,   STKSIZE_UVACISOIN,  UVAC_IsoInTsk);
        }
     }
     @endcode
*/
extern void UVAC_InstallID(void) _SECTION(".kercfg_text");

extern BOOL UVAC_ChkModInstalled(void);
/**
     Version

*/
extern CHAR *UVAC_GetVerInfo(void);

/**
     Build Date

*/
extern CHAR *UVAC_GetBuildDate(void);


extern void UVAC_InstallCmd(void);

#if (ENABLE == UVC_METHOD3)
extern void UVC_SetTestImgM3(UINT32 imgAddr, UINT32 imgSize);
#endif

#if (ENABLE == UVC_VEND_BULKIN)
extern void UVC_SetTestImg(UINT32 imgAddr, UINT32 imgSize);
#endif


/**
    Set supported resolutions for a video stream.

    Set supported resolutions, width and height, for a video stream.
    The maximun supported set is UVAC_VID_RESO_MAX_CNT, 12.
    This is optional because it defaultly has 8 resolutions.

    @param[in] pVidReso       point to an array with structure, UVAC_VID_RESO.
    @param[in] cnt            valid count of element in the array, pVidReso.
*/
extern ER UVAC_ConfigVidReso(PUVAC_VID_RESO pVidReso, UINT32 cnt);

//@}

#endif  // _UVAC_H

