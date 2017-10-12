/**
    Header file of user event task

    Declare system events of user task.

    @file       NVTEvent.h
    @ingroup    mINvtUser
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef NVT_EVENT_H
#define NVT_EVENT_H

/**
    @addtogroup mINvtUser
*/

//@{

/**
    @name Class of events

    @note For #NVTEVT.
*/
//@{
//Class --------------------------------------------
#define CLASS_EVENT_MASK                0xffff0000 ///< Mask for extract class of event

#define DEV_EVENT_MASK                  0x01000000 ///< Device class
#define LIB_EVENT_MASK                  0x02000000 ///< Library class
#define APP_EVENT_MASK                  0x04000000 ///< Application class
#define SYS_EVENT_MASK                  0x08000000 ///< System class
#define USER_EVENT_MASK                 0x80000000 ///< User-defined
//@}

/**
     Check is device class event.

     Check is device class event.
     @note Nothing.

     @param[in] e             event value.
*/
#define IS_DEV_EVENT(e)                 (((e) & DEV_EVENT_MASK))

/**
     Check is library class event.

     Check is library class event.
     @note Nothing.

     @param[in] e             event value.
*/
#define IS_LIB_EVENT(e)                 (((e) & LIB_EVENT_MASK))

/**
     Check is application class event.

     Check is application class event.
     @note Nothing.

     @param[in] e             event value.
*/
#define IS_APP_EVENT(e)                 (((e) & APP_EVENT_MASK))

/**
     Check is system class event.

     Check is system class event.
     @note Nothing.

     @param[in] e             event value.
*/
#define IS_SYS_EVENT(e)                 (((e) & SYS_EVENT_MASK))

/**
     Check is user-defined event.

     Check is user-defined event.
     @note Nothing.

     @param[in] e             event value.
*/
#define IS_USER_EVENT(e)                (((e) & USER_EVENT_MASK))


/**
    @name Type of events

    @note For #NVTEVT.
*/
//@{
//Type ---------------------------------------------
#define DEV_SYS_EVENT_BASE              0x01000000 ///< System device event
#define DEV_STRG_EVENT_BASE             0x01010000 ///< Storage device event
#define DEV_VOUT_EVENT_BASE             0x01020000 ///< Video-output device event
#define DEV_AOUT_EVENT_BASE             0x01030000 ///< Audio-output device event
#define DEV_VIN_EVENT_BASE              0x01040000 ///< ? device event
#define DEV_USB_EVENT_BASE              0x01050000 ///< Usb device event
#define DEV_KEY_EVENT_BASE              0x01060000 ///< Key-input device event
#define DEV_TOUCH_EVENT_BASE            0x01070000 ///< Touch-panel device event
#define DEV_POWER_EVENT_BASE            0x01080000 ///< Power-provider device event

#define LIB_TIMER_EVENT_BASE            0x02000000 ///< Timer library event
#define LIB_FILE_EVENT_BASE             0x02010000 ///< File library event
#define LIB_DISP_EVENT_BASE             0x02020000 ///< Display library event
#define LIB_SOUND_EVENT_BASE            0x02030000 ///< Sound library event
#define LIB_IPL_EVENT_BASE              0x02040000 ///< Image Pipe-line library event

#define APP_MAIN_EVENT_BASE             0x04000000 ///< Main applcation event
#define APP_PHOTOPLAY_EVENT_BASE        0x04010000 ///< Photo play applcation event
#define APP_PHOTOCAP_EVENT_BASE         0x04020000 ///< Photo capture applcation event
#define APP_MOVIEPLAY_EVENT_BASE        0x04030000 ///< Media play applcation event
#define APP_MOVIEREC_EVENT_BASE         0x04040000 ///< Media record applcation event
#define APP_USBPTP_EVENT_BASE           0x04050000 ///< Usb PTP/MTP applcation event
#define APP_USBCAM_EVENT_BASE           0x04060000 ///< Usb PCC applcation event
#define APP_USBDISK_EVENT_BASE          0x04070000 ///< Usb MSDC applcation event
#define APP_USBPRINT_EVENT_BASE         0x04080000 ///< Usb Pictbridge applcation event
//@}

/**
     Check is this type of event.

     Check is this type of event.
     @note Nothing.

     @param[in] e             event value.
     @param[in] base          event type.
*/
#define IS_TYPE_EVENT(e, base)          ( ((e)>=(base)) && ((e)<((base)+0x10000)) )


//Data ---------------------------------------------
/**
    @brief NVT event data type
*/
typedef UINT32 NVTEVT;


//Value --------------------------------------------
/**
    @brief NVT event values
*/
enum
{
    NVTEVT_NULL                         = 0,   ///< NULL
};


/**
    @brief Define the maximum number of parameters carried with a event
*/
#define MAX_MESSAGE_PARAM_NUM      3


//@}

#endif //NVT_EVENT_H
