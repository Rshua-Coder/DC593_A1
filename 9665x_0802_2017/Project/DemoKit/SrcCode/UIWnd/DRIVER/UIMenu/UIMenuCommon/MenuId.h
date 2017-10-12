#ifndef __MENUID_H
#define __MENUID_H

#include "UIResource.h"

// Menu Item ID
enum _MENU_ID
{
    IDM_NULL,

    // Photo Page
    IDM_SELFTIMER,
    IDM_PHOTO_SIZE,
    IDM_CONTINUE_SHOT,
    IDM_QUALITY,
    IDM_SHARPNESS,
    IDM_WB,
    IDM_COLOR_EFFECT,
    IDM_ISO,
    IDM_EV,
    IDM_FD,
    IDM_ANTISHAKE,
    IDM_QUICK_REVIEW,
    IDM_DATE_STAMP,

    // Movie Page
    IDM_MOVIE_SIZE,
    IDM_MOVIE_CYCLIC_REC,
    IDM_MOVIE_HDR,
    IDM_MOVIE_EV,
    IDM_MOVIE_MOTION_DET,
    IDM_MOVIE_AUDIO,
    IDM_MOVIE_DATEIMPRINT,
    IDM_MOVIE_QUALITY,
    IDM_MOVIE_WB,
    IDM_MOVIE_COLOR,
    IDM_GSENSOR,
    IDM_PARK_WAKEUP,

    // Playback Page
    IDM_DELETE,
    IDM_PROTECT,
    IDM_ROTATE,
    IDM_SLIDE_SHOW,
    IDM_DPOF,

    // Setup Page
    IDM_DATE_TIME,
    IDM_AUTO_POWER_OFF,
    IDM_BEEP,
    IDM_SILENT,
    IDM_LANGUAGE,
    IDM_TV_MODE,
    IDM_FREQUENCY,
    IDM_SENSOR_ROTATE,
    IDM_FORMAT,
    IDM_DEFAULT,
    IDM_OPENING_LOGO,
    IDM_WIFI,
    IDM_VERSION
};

// Menu String ID
enum _MENU_IDS
{
    // Photo Page
    IDS_SELFTIMER           = STRID_CAP_MODE,
    IDS_PHOTO_SIZE          = STRID_RESOLUTION,
    IDS_QUALITY             = STRID_QUALITY,
    IDS_CONTINUE_SHOT       = STRID_CAP_BURST,
    IDS_EV                  = STRID_EXPOSURE,
    IDS_METERING            = STRID_METERING,
    IDS_WB                  = STRID_WB,
    IDS_ISO                 = STRID_ISO,
    IDS_COLOR_EFFECT        = STRID_COLOR,
    IDS_SHARPNESS           = STRID_SHARPNESS,
    IDS_FACE                = STRID_FACE_DET,
    IDS_FD                  = STRID_FACE_DET,
    IDS_ANTISHAKE           = STRID_ANTI_SHAKING,
    IDS_QUICK_REVIEW        = STRID_QUICK_VIEW,
    IDS_DATE_STAMP          = STRID_DATE_STAMP,
    IDS_PHOTO               = STRID_STILL,

    // Photo Capture Options
    IDS_SELFTIMER_SINGLE    = STRID_CAP_SINGLE,
    IDS_SELFTIMER_2SEC      = STRID_CAP_TIMER2S,
    IDS_SELFTIMER_5SEC      = STRID_CAP_TIMER5S,
    IDS_SELFTIMER_10SEC     = STRID_CAP_TIMER10S,

    // Photo Size Options
    IDS_PHOTO_SIZE_15M      = STRID_NULL_,
    IDS_PHOTO_SIZE_14M      = STRID_NULL_,
    IDS_PHOTO_SIZE_12M      = STRID_12MWXH,
    IDS_PHOTO_SIZE_10M      = STRID_10MWXH,
    IDS_PHOTO_SIZE_8M       = STRID_8MWXH,
    IDS_PHOTO_SIZE_7M       = STRID_7MWXH,
    IDS_PHOTO_SIZE_7MHD     = STRID_NULL_,
    IDS_PHOTO_SIZE_5M       = STRID_5MWXH,
    IDS_PHOTO_SIZE_4M       = STRID_4MWXH,
    IDS_PHOTO_SIZE_3M       = STRID_3MWXH,
    IDS_PHOTO_SIZE_2M       = STRID_2MWXH,
    IDS_PHOTO_SIZE_2MHD     = STRID_2MHDWXH,
    IDS_PHOTO_SIZE_1M       = STRID_1MWXH,
    IDS_PHOTO_SIZE_VGA      = STRID_VGAWXH,

    //Continue shot
    IDS_CONTINUE_SHOT_OFF      = STRID_OFF,
    IDS_CONTINUE_SHOT_ON      = STRID_ON,

    // Metering Options
    IDS_AVERAGE             = STRID_METER_AVG,
    IDS_CENTER              = STRID_METER_CENTER,
    IDS_SPOT                = STRID_METER_SPOT,

    //ISO
    IDS_ISO_AUTO            = STRID_AUTO,
    IDS_ISO_100             = STRID_ISO100,
    IDS_ISO_200             = STRID_ISO200,
    IDS_ISO_400             = STRID_ISO400,

    // Sharpness Options
    IDS_SHARP_STRONG        = STRID_STRONG,
    IDS_SHARP_NORMAL        = STRID_NORMAL,
    IDS_SHARP_SOFT          = STRID_SOFT,

    // WB Options
    IDS_WB_AUTO             = STRID_AUTO,
    IDS_WB_DAYLIGHT         = STRID_WB_DAY,
    IDS_WB_CLOUDY           = STRID_WB_CLOUDY,
    IDS_WB_TUNGSTEN         = STRID_WB_TUNGSTEN,
    IDS_WB_FLUORESCENT      = STRID_WB_FLUORESCENT,

    // Color Effect Options
    IDS_COLOR_EFFECT_STANDARD     = STRID_COLOR,
    IDS_COLOR_EFFECT_MONOCHROME   = STRID_COLOR_BW,
    IDS_COLOR_EFFECT_SEPIA        = STRID_COLOR_SEPIA,

    // EV Options
    IDS_EV_P20              = STRID_EV_P2P0,
    IDS_EV_P16              = STRID_EV_P1P6,
    IDS_EV_P13              = STRID_EV_P1P3,
    IDS_EV_P10              = STRID_EV_P1P0,
    IDS_EV_P06              = STRID_EV_P0P6,
    IDS_EV_P03              = STRID_EV_P0P3,
    IDS_EV_00               = STRID_EV_P0P0,
    IDS_EV_N03              = STRID_EV_M0P3,
    IDS_EV_N06              = STRID_EV_M0P6,
    IDS_EV_N10              = STRID_EV_M1P0,
    IDS_EV_N13              = STRID_EV_M1P3,
    IDS_EV_N16              = STRID_EV_M1P6,
    IDS_EV_N20              = STRID_EV_M2P0,

    // Face Detection Options
    IDS_FD_OFF              = STRID_OFF,
    IDS_FD_ON               = STRID_FACE_DET,
    IDS_FD_SMILE            = STRID_SMILE_DET,

    // Anti-shaking Options
    IDS_ANTISHAKE_OFF       = STRID_OFF,
    IDS_ANTISHAKE_ON        = STRID_ON,

    // Quick View Time Options
    IDS_QV_OFF              = STRID_OFF,
    IDS_QV_2SEC             = STRID_2SEC,
    IDS_QV_5SEC             = STRID_5SEC,

    // Date Stamp Options
    IDS_DATE_STAMP_OFF      = STRID_OFF,
    IDS_DATE_STAMP_DATE     = STRID_DATE,
    IDS_DATE_STAMP_DATETIME = STRID_DATE_TIME,

    // Movie Page
    IDS_MOVIE_SIZE          = STRID_RESOLUTION,
    IDS_MOVIE_EV            = STRID_EXPOSURE,
    IDS_MOVIE_CYCLIC_REC    = STRID_CYCLIC_REC,
    IDS_MOVIE_HDR           = STRID_HDR,
    IDS_MOVIE_MOTION_DET    = STRID_MOTION_DET,
    IDS_MOVIE_AUDIO         = STRID_RECORD_AUDIO,
    IDS_MOVIE_DATEIMPRINT   = STRID_DATE_STAMP,
    IDS_MOVIE_QUALITY       = STRID_QUALITY,
    IDS_MOVIE_WB            = STRID_WB,
    IDS_MOVIE_COLOR         = STRID_COLOR,
    IDS_MOVIE               = STRID_MOVIE,
    IDS_GSENSOR               = STRID_GSENSOR_2,
    IDS_PARK_WAKEUP     = STRID_PARK_WAKEUP,//STRID_PARK_WAKEUP
    // Movie Size Options
    IDS_MOVIE_SIZE_1080FHD  = STRID_1080FHDWXH,
    IDS_MOVIE_SIZE_1080P    = STRID_1080PWXH,
    IDS_MOVIE_SIZE_1440P    = STRID_1080PWXH,
    IDS_MOVIE_SIZE_720P     = STRID_720PWXH,
    IDS_MOVIE_SIZE_WVGA     = STRID_WVGAWXH,
    IDS_MOVIE_SIZE_VGA      = STRID_VGAWXH,
    IDS_MOVIE_SIZE_QVGA     = STRID_QVGAWXH,

    // EV Options
    IDS_MOVIE_EV_P20        = STRID_EV_P2P0,
    IDS_MOVIE_EV_P16        = STRID_EV_P1P6,
    IDS_MOVIE_EV_P13        = STRID_EV_P1P3,
    IDS_MOVIE_EV_P10        = STRID_EV_P1P0,
    IDS_MOVIE_EV_P06        = STRID_EV_P0P6,
    IDS_MOVIE_EV_P03        = STRID_EV_P0P3,
    IDS_MOVIE_EV_00         = STRID_EV_P0P0,
    IDS_MOVIE_EV_N03        = STRID_EV_M0P3,
    IDS_MOVIE_EV_N06        = STRID_EV_M0P6,
    IDS_MOVIE_EV_N10        = STRID_EV_M1P0,
    IDS_MOVIE_EV_N13        = STRID_EV_M1P3,
    IDS_MOVIE_EV_N16        = STRID_EV_M1P6,
    IDS_MOVIE_EV_N20        = STRID_EV_M2P0,


    // Movie loop rec Options
    IDS_MOVIE_CYCLIC_REC_OFF    = STRID_OFF,
	IDS_MOVIE_CYCLIC_REC_1MIN	= STRID_1MIN,
    IDS_MOVIE_CYCLIC_REC_3MIN   = STRID_3MIN,
    IDS_MOVIE_CYCLIC_REC_5MIN   = STRID_5MIN,
    IDS_MOVIE_CYCLIC_REC_10MIN  = STRID_10MIN,

    // Movie motion detect Options
    IDS_MOVIE_MOTION_DET_OFF  = STRID_OFF,
    IDS_MOVIE_MOTION_DET_ON   = STRID_ON,

    // Movie HDR options
    IDS_MOVIE_HDR_OFF  = STRID_OFF,
    IDS_MOVIE_HDR_ON   = STRID_ON,

    // Movie audio Options
    IDS_MOVIE_AUDIO_OFF     = STRID_OFF,
    IDS_MOVIE_AUDIO_ON      = STRID_ON,

    // Movie dateImprint Options
    IDS_MOVIE_DATEIMPRINT_OFF = STRID_OFF,
    IDS_MOVIE_DATEIMPRINT_ON  = STRID_ON,

    // Movie G-sensor Options
    IDS_GSENSOR_OFF         = STRID_OFF,
    IDS_GSENSOR_LOW         = STRID_LOW,
    IDS_GSENSOR_MED         = STRID_MED,
    IDS_GSENSOR_HIGH        = STRID_HIGH,
    // Movie Quality Options
    IDS_QUALITY_FINE        = STRID_FINE,
    IDS_QUALITY_NORMAL      = STRID_NORMAL,
    IDS_QUALITY_ECONOMY     = STRID_ECONOMY,

    // Park wake up
    IDS_PARK_WAKEUP_ON			= STRID_ON,
    IDS_PARK_WAKEUP_OFF			= STRID_OFF,
    // Playback Page
    IDS_DELETE              = STRID_DELETE,
    IDS_PROTECT             = STRID_PROTECT,
    IDS_SLIDE_SHOW          = STRID_SLIDE_SHOW,
    IDS_ROTATE              = STRID_ROTATE,
    IDS_DPOF                = STRID_DPOF,
    IDS_PLAYBACK            = STRID_PLAYBACK,

    // Protect Options
    IDS_PROTECT_ONE         = STRID_LOCKONE,
    IDS_UNPROTECT_ONE       = STRID_UNLOCKONE,
    IDS_PROTECT_ALL         = STRID_LOCKALL,
    IDS_UNPROTECT_ALL       = STRID_UNLOCKALL,

    // Rotate Options
    IDS_ROTATE_90           = STRID_ROTATE_90,
    IDS_ROTATE_180          = STRID_ROTATE_180,
    IDS_ROTATE_270          = STRID_ROTATE_270,

    // Slide Show Options
    IDS_SLIDE_SHOW_2SEC     = STRID_2SEC,
    IDS_SLIDE_SHOW_5SEC     = STRID_5SEC,
    IDS_SLIDE_SHOW_8SEC     = STRID_8SEC,

    // Setup Page
    IDS_DATE_TIME           = STRID_DATE_TIME,
    IDS_AUTO_POWER_OFF      = STRID_AUTO_OFF,
    IDS_BEEP                = STRID_BEEPER,
    IDS_SILENT              = STRID_BEEPER,
    IDS_LANGUAGE            = STRID_LANGUAGE,
    IDS_TV_MODE             = STRID_TV_MODE,
    IDS_FREQUENCY           = STRID_FREQUENCY,
    IDS_SENSOR_ROTATE       = STRID_SENSOR_ROTATE,
    IDS_FORMAT              = STRID_FORMAT,
    IDS_DEFAULT             = STRID_DEFAULT_SETTING,
    IDS_OPENING_LOGO        = STRID_LOGO_DISPLAY,
    IDS_VERSION             = STRID_VERSION,
    IDS_WIFI                = STRID_WIFI,
    IDS_SETUP               = STRID_SETUP,

    // Power Saving Options
    IDS_OFF                 = STRID_OFF,
    IDS_1MIN                = STRID_AUTO_OFF_1MIN,
    IDS_3MIN                = STRID_AUTO_OFF_3MIN,
    IDS_5MIN                = STRID_AUTO_OFF_5MIN,
    IDS_10MIN               = STRID_AUTO_OFF_10MIN,

    // Beep Options
    IDS_SILENT_ON            = STRID_OFF,
    IDS_SILENT_OFF           = STRID_ON,

    IDS_BEEP_OFF            = STRID_OFF,
    IDS_BEEP_ON             = STRID_ON,

    // Language Options
    IDS_LANG_EN             = STRID_LANG_EN,
    IDS_LANG_DE             = STRID_LANG_DE,
    IDS_LANG_FR             = STRID_LANG_FR,
    IDS_LANG_ES             = STRID_LANG_ES,
    IDS_LANG_IT             = STRID_LANG_IT,
    IDS_LANG_PO             = STRID_LANG_PO,
    IDS_LANG_SC             = STRID_LANG_SC,
    IDS_LANG_TC             = STRID_LANG_TC,
    IDS_LANG_JP             = STRID_LANG_JP,
    IDS_LANG_RU             = STRID_LANG_RU,
    IDS_LANG_DU             = STRID_NULL_,
    IDS_LANG_TU             = STRID_NULL_,
    IDS_LANG_PB             = STRID_NULL_,
    IDS_LANG_YI             = STRID_NULL_,
    IDS_LANG_PL             = STRID_NULL_,


    // TV Mode Options
    IDS_TV_NTSC             = STRID_TV_NTSC,
    IDS_TV_PAL              = STRID_TV_PAL,

    // Frequency Options
    IDS_50HZ                = STRID_50HZ,
    IDS_60HZ                = STRID_60HZ,

    // Sensor Rotate Options
    IDS_SENSOR_ROTATE_OFF   = STRID_OFF,
    IDS_SENSOR_ROTATE_ON    = STRID_ON,
    // Screen Saver
    IDS_LCDOFF_OFF       =  STRID_OFF,
    IDS_LCDOFF_1MIN       =  STRID_1MIN,
    IDS_LCDOFF_3MIN       =  STRID_3MIN,
    IDS_LCDOFF_5MIN      =  STRID_5MIN,
    IDS_LCDOFF_10MIN       =  STRID_10MIN,

    // Wi-Fi Options
    IDS_WIFI_OFF            = STRID_OFF,
    IDS_WIFI_ON             = STRID_ON,

    // Opening Logo Display Options
    IDS_LOGO_OFF            = STRID_OFF,
    IDS_LOGO_ON             = STRID_ON
};

// Menu Icon ID
enum _MENU_IDI
{
    // Photo Page
    IDI_SELFTIMER           = ICON_MODE_CAPTURE,
    IDI_PHOTO_SIZE          = ICON_RESOLUTION,
    IDI_QUALITY             = ICON_QUALITY,
    IDI_CONTINUE_SHOT       = ICON_SEQUENCE_NO,
    IDI_EV                  = ICON_EV,
    IDI_METERING            = ICON_METERING,
    IDI_WB                  = ICON_WB_AUTO,
    IDI_ISO                 = ICON_ISO,
    IDI_COLOR_EFFECT        = ICON_COLOR,
    IDI_SHARPNESS           = ICON_SHARPNESS,
    IDI_FD                  = ICON_FACE_ON,
    IDI_ANTISHAKE           = ICON_SHAKE_ON,
    IDI_QUICK_REVIEW        = ICON_QUICK_REVIEW,
    IDI_DATE_STAMP          = ICON_DATE_PRINT,
    IDI_PHOTO               = ICON_MENU_STILL_ON,       // Photo menu enable icon
    IDIX_PHOTO              = ICON_MENU_STILL_OFF,      // Photo menu disable icon

    // Movie size options
    IDI_MOVIE_SIZE_1080FHD  = ICON_OK,
    IDI_MOVIE_SIZE_1080P    = ICON_OK,
    IDI_MOVIE_SIZE_1440P    = ICON_OK,
    IDI_MOVIE_SIZE_720P     = ICON_OK,
    IDI_MOVIE_SIZE_WVGA     = ICON_OK,
    IDI_MOVIE_SIZE_VGA      = ICON_OK,
    IDI_MOVIE_SIZE_QVGA     = ICON_OK,

    // Photo Size Options
    IDI_PHOTO_SIZE_15M      = ICON_OK,
    IDI_PHOTO_SIZE_14M      = ICON_OK,
    IDI_PHOTO_SIZE_12M      = ICON_OK,
    IDI_PHOTO_SIZE_10M      = ICON_OK,
    IDI_PHOTO_SIZE_8M       = ICON_OK,
    IDI_PHOTO_SIZE_7M       = ICON_OK,
    IDI_PHOTO_SIZE_7MHD     = ICON_OK,
    IDI_PHOTO_SIZE_5M       = ICON_OK,
    IDI_PHOTO_SIZE_4M       = ICON_OK,
    IDI_PHOTO_SIZE_3M       = ICON_OK,
    IDI_PHOTO_SIZE_2M       = ICON_OK,
    IDI_PHOTO_SIZE_1M       = ICON_OK,
    IDI_PHOTO_SIZE_2MHD     = ICON_OK,
    IDI_PHOTO_SIZE_VGA      = ICON_OK,

    // Movie Page
    IDI_MOVIE_SIZE          = ICON_RESOLUTION,
    IDI_MOVIE_EV            = ICON_EV,
    IDI_MOVIE_CYCLIC_REC    = ICON_CYCLIC_REC,
    IDI_MOVIE_HDR           = ICON_MENU_WDR,
    IDI_MOVIE_MOTION_DET    = ICON_MENU_FLASH_REC,
    IDI_MOVIE_AUDIO         = ICON_VOICE,
    IDI_MOVIE_DATEIMPRINT   = ICON_DATE_STAMPING,
    IDI_MOVIE_QUALITY       = ICON_QUALITY,
    IDI_MOVIE_WB            = ICON_WB_AUTO,
    IDI_MOVIE_COLOR         = ICON_COLOR,
    IDI_MOVIE               = ICON_MENU_VIDEO_ON,       // Movie menu enable icon
    IDIX_MOVIE              = ICON_MENU_VIDEO_OFF,      // Movie menu disable icon
    IDI_GSENSOR              = ICON_G_SENSOR,      // Movie menu disable icon
    IDI_PARK_WAKEUP             = ICON_DDD_ON,      // Movie menu disable icon
    // Playback Page
    IDI_DELETE              = ICON_DELETE,
    IDI_PROTECT             = ICON_PROTECT,
    IDI_SLIDE_SHOW          = ICON_SLIDESHOW,
    IDI_ROTATE              = ICON_ROTATE,
    IDI_DPOF                = ICON_DPOF,
    IDI_PLAYBACK            = ICON_MENU_PLAYBACK_ON,    // Playback menu enable icon
    IDIX_PLAYBACK           = ICON_MENU_PLAYBACK_OFF,   // Playback menu disable icon

    // Setup Page
    IDI_DATE_TIME           = ICON_DATE_TIME,
    IDI_AUTO_POWER_OFF      = ICON_POWER_OFF,
    IDI_SILENT              = ICON_VOICE,
    IDI_BEEP                = ICON_VOICE,
    IDI_LANGUAGE            = ICON_LANGUAGE,
    IDI_FREQUENCY           = ICON_HZ,
    IDI_TV_MODE             = ICON_TV_MODE,
    IDI_SENSOR_ROTATE       = ICON_COPY_FROM_CARD,
    IDI_FORMAT              = ICON_FORMAT,
    IDI_DEFAULT             = ICON_DEFAULT,
//    IDI_OPENING_LOGO        = ICON_LOGO_DISPLAY,
    IDI_WIFI                = ICON_WIFI_ON,
    IDI_VERSION             = ICON_VERSION,
    IDI_SETUP               = ICON_MENU_SETUP_ON,       // Setup menu enable icon
    IDIX_SETUP              = ICON_MENU_SETUP_OFF,       // Setup menu disable icon

    // Language Options
    IDI_LANG_EN             = ICON_OK,
    IDI_LANG_FR             = ICON_OK,
    IDI_LANG_ES             = ICON_OK,
    IDI_LANG_PO             = ICON_OK,
    IDI_LANG_DE             = ICON_OK,
    IDI_LANG_DU             = ICON_OK,
    IDI_LANG_IT             = ICON_OK,
    IDI_LANG_RU             = ICON_OK,
    IDI_LANG_TU             = ICON_OK,
    IDI_LANG_JP             = ICON_OK,
    IDI_LANG_TC             = ICON_OK,
    IDI_LANG_SC             = ICON_OK,
    IDI_LANG_PB             = ICON_OK,
    IDI_LANG_YI             = ICON_OK,
    IDI_LANG_PL             = ICON_OK

};

#endif // __MENUID_H

