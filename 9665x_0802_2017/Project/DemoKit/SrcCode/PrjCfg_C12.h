#ifndef _PRJCFG_C12_H_
#define _PRJCFG_C12_H_
#include "Type.h"
#include "SysKer.h"

//==============================================================================
//   GENERAL CONFIG
//==============================================================================
//..............................................................................
// LD version and name
#define LOADER_VERSION                  1
#define LOADER_VERSION_CHECK            DISABLE

//..............................................................................
// FW version and name
#define FW_VERSION_HIDDEN               "00.00" //最多32個bytes, 包含字符串結尾的'\0'//隱藏版本號
#define FW_UPDATE_NAME                  "A:\\FW96658A.bin"
#ifndef __ECOS
#define FW_VERSION_RELEASE              DISABLE //ENABLE
#define FW_VERSION_NUM                  "GSDEMO_0001_0027"
#else
#define FW_VERSION_RELEASE              DISABLE
#define FW_VERSION_NUM                  "CDV.001.001.003"
#endif
#define _CUSTOMER_DSC_                  _CUSTOMER_N_

//..............................................................................
// Memory Partition
#if defined(__arm )
#define MEM_DRAM_ADDR                   0x0
#elif defined(__GNUC__ )
#define MEM_DRAM_ADDR                   0x80000000  //cached region of MIPS
#endif

#define MEM_DRAM_SIZE                   ALIGN_CEIL_32(0x08000000) //DRAM size = 128MB (for FGPA)

//..............................................................................
// Internal Storage Partition

#define NAND_BLOCK_SIZE                 (16*1024L) //each block is 16K

//LOADER region is used to store Loader binary area (and some reserved data)
#define LOADER_SIZE                     (4*NAND_BLOCK_SIZE)   //64K, (4 blocks)
//BINFILE region is used to store FW binary area (and some calibration data)
#define BINFILE_SIZE                    (*((volatile UINT32*)0x78))  // Maximum size of FW binary file
//PSTORE region is used to store user custom data
#define PSTORE_SIZE                     0x00500000 // Size of PStore region  (default size 1M=0X100000)

#define NAND2_SIZE                      0x0     // Size of NAND2 region (for FileSystem)

#define NAND_TOTAL_SIZE                 0x08000000 //128MB
#define NAND_FWRSV_SIZE                 0x900000   //FWRSV_SIZE = LOADER+FW
#define NAND_FAT_SIZE                   (NAND_TOTAL_SIZE-PSTORE_SIZE-NAND_FWRSV_SIZE)

//==============================================================================
//   SYSTEM FUNCTIONS
//==============================================================================
//..............................................................................
// Debug
#define _MEM_OVERWRITE_CHK_             DISABLE
#define FETCH_LAST_MSG                  ENABLE
#define _WATCH_DOG_FUNC_                DISABLE

//..............................................................................
// Boot
#define POWERON_TRACE                   DISABLE //stop and wait for user enter cmd: "dsc boot"
#define POWERON_TESTFLOW                DISABLE //test of USB PC 100mA flow
#define _PWRON_WAIT_FILESYSTEM_         DISABLE
#define WAITPHOTO_FUNCTION              ENABLE
#define POWERONLOGO_FUNCTION            ENABLE
#define POWEROFFLOGO_FUNCTION           ENABLE
#define POWERONSOUND_FUNCTION           ENABLE
#define POWEROFFSOUND_FUNCTION          ENABLE
#define USER_LOGO_FUNCTION              ENABLE
#define _LOGO_                          _LOGO_NOVATEK_
#define LOGO_DISP_LAYER                 LAYER_VDO1
#define LOGO_FREEZE                     ENABLE
#define LENSERROFF_FUNCTION             DISABLE

//..............................................................................
// Performance Config
#define DMAHIGHSPEED_FUNCTION           ENABLE
#define CPUHIGHSPEED_FUNCTION           ENABLE

//..............................................................................
// Input Config
//touch
#define TOUCH_FUNCTION                  DISABLE //Auto Detect
//key
#define POWERKEY_FUNCTION               ENABLE //Auto Detect
#define NORMALKEY_FUNCTION              ENABLE //Auto Detect
#define STATUSKEY_FUNCTION              ENABLE //Auto Detect
#define MODEKEY_FUNCTION                ENABLE //Auto Detect

#define AUTOINPUT_FUNCTION              DISABLE

// copy card to card function
#define COPYCARD2CARD_FUNCTION          DISABLE

//..............................................................................
// Power-Saving Config
//battery level
#define PWRLEVEL_FUNCTION               ENABLE //Auto Detect
#define LOWPWROFF_FUNCTION              DISABLE //Auto Detect
//battery temp
#define TEMPERATURE_FUNCTION            DISABLE //Auto Detect
//auto power-off when battery level = empty
#define EMPTYPWROFF_FUNCTION            ENABLE
//auto sleep
#define AUTOSLEEP_FUNCTION              DISABLE //Auto Detect
//auto power-off
#define AUTOPWROFF_FUNCTION             ENABLE //Auto Detect

//..............................................................................
// Date-Time Config
#define DEF_YEAR                        2016
#define DEF_MONTH                       1
#define DEF_DAY                         1
#define MIN_YEAR                        2016
#define MAX_YEAR                        2050

//..............................................................................
// Display Config (for UI window, FD window and UI backgound)

// OSD size and format
#define DISPLAY_OSD_W                   320//640
#define DISPLAY_OSD_H                   240
#define DISPLAY_OSD_FMT                 PXLFMT_INDEX8 // 256 colors
#define DISPLAY_OSD_BPP                 8 // 8 for PXLFMT_INDEX8, 4 for PXLFMT_INDEX4
//#define DISPLAY_OSD_FMT                 PXLFMT_RGBA5658_PK // 16bpp-colors
//#define DISPLAY_OSD_BPP                 24 // 24 for PXLFMT_RGBA5658_PK

//UI tool layout size setting
#define TOOL_LAYOUT_W                   320//640
#define TOOL_LAYOUT_H                   240//480

#define OSD_USE_DOUBLE_BUFFER           ENABLE //use double buffer
#define OSD2_USE_DOUBLE_BUFFER          ENABLE //use double buffer
#define OSD_USE_ROTATE_BUFFER           DISABLE //support rotate screen

// PIP flip
#define PIP_1ST_PATH_FLIP               DISABLE
#define PIP_2ND_PATH_FLIP               DISABLE

// Display Config (for Photo App, Movie App and Play App)

// VDO size and format
#define DISPLAY_VDO_W                   320//640
#define DISPLAY_VDO_H                   240//480
#define DISPLAY_VDO_FMT                 PXLFMT_YUV422_PK
#define DISPLAY_VDO_BPP                 16

#define VDO_USE_DOUBLE_BUFFER           DISABLE //use double buffer
#define VDO2_USE_DOUBLE_BUFFER          ENABLE //use double buffer //#NT#2010/05/26#Janice Huang#[0010918]
#define VDO_USE_PHOTO_SOURCE            ENABLE  //display photo video streaming
#define VDO_USE_ROTATE_BUFFER           DISABLE  //use video rotate buffer

// Display Config (LCD or TV)

#define TVINSERT_FUNCTION               ENABLE //Auto Detect

#define DET_LCD_REG_FUNCTION            DISABLE
#define DET_LCDDIR_FUNCTION             DISABLE
#define TV_FUNC                         DISABLE
#define HDMI_FUNC                       DISABLE//ENABLE
//#define DUALDISP_FUNC                   DISABLE
//#define DISPLAY_169HD                   ENABLE
#define AUTO_BACKLIGHT_FUNCTION         DISABLE

#define DUALDISP_OFF                    0
#define DUALDISP_ONXOR                  1
#define DUALDISP_ONBOTH                 2
#define DUALDISP_FUNC                   DUALDISP_OFF


// Display Config (for DispSrv)

//LCD1: RGBD320 mode
#define LCD_MAX_W                       ALIGN_CEIL_16(320)  //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(240)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
//LCD2: RGBD320 mode
#define LCD2_MAX_W                      320  //device w of DISP_LCDMODE_xxx
#define LCD2_MAX_H                      240  //device h of DISP_LCDMODE_xxx
//TV: DISP_TVMODE_PAL mode
#define TV_MAX_W                        640  //device w of DISP_TVMODE_xxx
#define TV_MAX_H                        528  //device h of DISP_TVMODE_xxx
//HDMI: DISP_HDMIMODE_1920X1080I60 mode
#define HDMI_MAX_W                      ALIGN_CEIL_16(1920)  //device w of DISP_HDMIMODE_xxx, for H264 decoder, it must align to 16
#define HDMI_MAX_H                      ALIGN_CEIL_16(1088)  //device h of DISP_HDMIMODE_xxx, for H264 decoder, it must align to 16

#define VDO_BUFFER_SIZE_OPTIMIZATION    ENABLE //DISABLE //ENABLE for optimize buffer size; DISABLE for default buffer size

//..............................................................................
// Audio Config
#define BEEP_VOLUME_LOUDER              DISABLE
#define BEEP_VOLUME_SMALLER             DISABLE
#define _VOICEMEMO_MODE_                DISABLE         // Enable/Disable Voice Memo function
#define _VM_FORMAT_                     _VM_FORMAT_PCM16_

//..............................................................................
// Internal Storage Config
#define NAND_CUSTOM_SPEED               DISABLE //using custom speed
#define ONLY_SUPPORT_SD_NOT_SUPPORT_MMC ENABLE  //Not support MMC Cards

//..............................................................................
// External Storage Config
#define SDINSERT_FUNCTION               ENABLE //Auto Detect

//..............................................................................
// File Config

#define USE_FILEDB                      ENABLE
#define FILEDB_CARDV_ROOT               "CARDV"   // the max length should not exceeds 16
//..............................................................................
// DCF/EXIF Config
#define DCF_SUBPATH                     "NVTIM"
#define DCF_IMAGE_NAME                  "IMAG"
#define DCF_VIDEO_NAME                  "IMAG"
#define DCF_AUDIO_NAME                  "IMAG"
#define DCF_RAW_NAME                    "IMAG"
#define DCF_TIFF_NAME                   "IMAG"

//..............................................................................
// EXIF Config
// End string must be '\0', total char must be 31
#define EXIF_MAKER_NAME                 "Novatek"
// End string must be '\0', total char must be 31
#define EXIF_MODEL_NAME                 "96650"
// End string must be '\0', total char must be 31
#define EXIF_IMAGE_DESCROPTOIN          "NOVATEK CAMERA"

#define EXIF_SOFTWARE_VERSION           FW_VERSION_NUM    // <= 20 char (TagSoftVerLen)

//..............................................................................
// Movie Maker/Model name Config
// End string must be '\0', total char must plus 1
#define MOVAPP_COMP_MANU_STRING         "NVT-IM"
// End string must be '\0', total char must plus 1
#define MOVAPP_COMP_NAME_STRING         "CarDV-TURNKEY"
//..............................................................................

#define PRODUCT_NAME_STRING             "CarDV"
#define PRODUCT_SN_STRING               "12345678"
#define AP_NAME_STRING                  "CarDV_"

// Usb Config

#define USBINSERT_FUNCTION              ENABLE //Auto Detect

#define USB_DRIVING                     DISABLE
#define USB_SUPPORT_PLUGOFF_PWROFF      0x01

#define USB_VID                         0x0603
#define USB_PID_PCCAM                   0x8612 // not support pc cam
#define USB_PID_WRITE                   0x8614
#define USB_PID_PRINT                   0x8613
#define USB_PID_MSDC                    0x8611

#define USB_PRODUCT_REVISION            '1', '.', '0', '0'
#define USB_VENDER_DESC_STRING          'N', 0x00,'O', 0x00,'V', 0x00,'A', 0x00,'T', 0x00,'E', 0x00,'K', 0x00, 0x20, 0x00,0x00, 0x00 // NULL
#define USB_VENDER_DESC_STRING_LEN      0x09
#define USB_PRODUCT_DESC_STRING         'J', 0x00,'1', 0x00,'4', 0x00,'5', 0x00,'5', 0x00, 0x20, 0x00,0x00, 0x00 // NULL
#define USB_PRODUCT_DESC_STRING_LEN     0x07
#define USB_PRODUCT_STRING              'N','v','t','-','D','S','C'
#define USB_SERIAL_NUM_STRING           '9', '6', '4', '6', '0','0', '0', '0', '0', '0','0', '0', '1', '0', '0'
#define USB_SIDC_DESC_STRING            'J',0x00,'1',0x00,'4',0x00,'5',0x00,'5',0x00,0x20, 0x00,0x00, 0x00 // NULL
#define USB_SIDC_DESC_STRING_LEN        0x07
#define USB_MTP_FRIENDNAME_STRING       'J','1','4','5','5',0x00  //maximum length is 253 bytes
#define USB_MTP_FRIENDNAME_STRING_LEN   0x06                      //maximum length is 253 bytes
#define USB_VENDER_STRING               'N','O','V','A','T','E','K'
#define USB_VENDER_SIDC_DESC_STRING     'N', 0x00,'O', 0x00,'V', 0x00,'A', 0x00,'T', 0x00,'E', 0x00,'K', 0x00, 0x20, 0x00,0x00, 0x00 // NULL
#define USB_VENDER_SIDC_DESC_STRING_LEN 0x09

//..............................................................................
// Other Config

//G-sensor
#define GSENSOR_FUNCTION                ENABLE

//GPS
#define GPS_FUNCTION                    DISABLE

//#NT#2013/02/20#Philex Lin -begin
//#NT# flag for pressing power button longer to startup system or power off directly
#define LONG_STARTUP                    DISABLE     // Enable/Disable pressing power button to startup system
//#NT#2013/02/20#Philex Lin -end

//#NT#2011/10/14#Spark Chou -begin
//#NT# Video IQ debug info
//---------------------------------------------------------------------------
// For Video Debug Info
//---------------------------------------------------------------------------
#define _MOVIE_IQLOG_          DISABLE     // Enable/Disable Video IQLOG
//#NT#2011/10/14#Spark Chou -end

//==============================================================================
//   PLAYBACK FUNCTIONS
//==============================================================================

#define PLAY_BG_MOVIE                   DISABLE
#define PLAY_BG_THUMBNAIL               DISABLE
#define PLAY_BG_BADFILE                 ENABLE
#define _DATEFOLDER_MODE_               DISABLE
#define FISHEYETOPLAYBACK_BLACKBG       ENABLE
#define SLIDE_EFFECT_FUNC               DISABLE
#define SLIDESHOW_MUSIC_OPTION          DISABLE
#define WALLPAPER_FUNC                  DISABLE
#define _IKEY_DATEVIEW_MODE_            DISABLE

//==============================================================================
//   PHOTO FUNCTIONS
//==============================================================================

#define PHOTO_SET_IMAGE_RATIO           ENABLE
#define PHOTO3A_FUNCTION                ENABLE
#define TIMELAPSE_SHOT_SHOW_WAITING     ENABLE
#define PANCAPTURE_FUNCTION             DISABLE
#define SINGLE_SHOT_HIGH_QTY            ENABLE
#define _SMILE_CAPTURE_FUNC_            ENABLE
#define _TIMELAPSE_SHUT_CNTDOWN_FUNC_   ENABLE
#define PANCAPTURE_FUNCTION             DISABLE
#define PANCAPTURE_MODE                 PANCAPTURE_MODE_AUTO
#define PANCAPTURE_SIZE                 PANCAPTURE_SIZE_3M
#define EFFECT_3D_FUNCTION              ENABLE
#define BRC460_FUNCTION                 ENABLE
#define ASPSCS_FUNCTION                 DISABLE
#define EV_PREDICT_NO_OFFSET            DISABLE
#define OBJECT_TRACKING_FUNC            DISABLE
#define CIPAIMPROVE_FUNCTION            DISABLE
#define SCENE_SKETCH2HDR                DISABLE // SCENE_SKETCH is sketch mode
#define STILL_DIS_FUNC                  DISABLE
#define ASSIST_BEAM_FUNC                DISABLE
#define SMART_LIGHT_FUNC                ENABLE
#define PHOTO_CONAF_FUNC                DISABLE
#define MOVIE_CONAF_FUNC                DISABLE
#define BLINKDET_FUNC                   ENABLE
#define SMILEDET_FUNC                   ENABLE
#define SLOWSHOT_FUNC                   ENABLE
#define PANORAMA_FUNC                   ENABLE
#define THREEDNR_FUNC                   DISABLE
#define SUPER_RESOLUTION_FUNC           DISABLE
#define GDC_FUNC                        DISABLE
#define PRV_PREOUT_HIGH_QTY             ENABLE
#define FLASH_CHARGE_STOP_FUNC          DISABLE   // Hera14 HW bug , no battery insert can't charge for flash
#define AF_BEAM_EV_THRESHOLD            540
#define METER_AIAE                      DISABLE
#define STAB_FUNC                       DISABLE
#define HDR_FUNC                        DISABLE

//..............................................................................
#define _MAX_IMAGE_SIZE_                _MAX_IMAGE_SIZE_14M_

//..............................................................................
#define _MSHUTTER1_FUNC_                ENABLE

//..............................................................................
#define LENS_FUNCTION                   DISABLE
#define _LENSCTRLTSK1_                  ENABLE
#define _LENSCTRLTSK2_                  ENABLE
#define _IRIS_FUNC_                     ENABLE
#define LENS_PATCH_ZOOM_RESET           ENABLE

//..............................................................................
// Photo Frame Pstore Version Config
#define PF_PSTORE_VER                   0x00000105

//..............................................................................
#define _CALIBRATION_MODE_              DISABLE//ENABLE          // Enable/Disable Calibration
#define SHOW_ADC_ON_SCREEN              DISABLE
#define SPECIAL_KEY_RESET_FUNC          DISABLE
#define CAL_CHKRESULT_FUNC              DISABLE

//==============================================================================
//   MOVIE FUNCTIONS
//==============================================================================

#define MOVIE_CAF_DEFAULT_ON            DISABLE
#define MOVIE_RECORD_SECOND_30          DISABLE
#define MOVIE_SPECIAL_MEM_ALLOC         DISABLE
#define MOVIE_FORMAT                    MOVIE_FORMAT_AVI | MOVIE_FORMAT_MOV
#define MOVIE_DIS                       DISABLE
#define MOVIE_PIM                       DISABLE // picture in movie (capture full resolution image as movie recording)
#define MOVIE_WITH_OZOOM_FUNCTION       DISABLE
#define MOVIE_WITH_CAF_FUNCTION         DISABLE
#define MOVIE_CONTAF_SLOW_RATE          DISABLE
#define MOVIE_HD                        ENABLE
#define MOVIE_AVSYNC                    ENABLE
#define MOVIE_GDC_FUNC                  DISABLE
#define MOVIE_SMEAR_R_FUNC              DISABLE
#define MOVIE_DIS_FUNC                  DISABLE
#define MOVIE_DATASTAMP                 ENABLE
#define MOVIEPLAYSPEED8X                DISABLE

#define MOVIE_REC_CLASS4                DISABLE
#define MOVIE_FD_FUNC_                  DISABLE

//#User Data, EV
#define MOVIE_USERDATA_EV_TAG           0X5F54564E  //NVT_ ;exactly 4 bytes

#define WATERLOGO_FUNCTION              ENABLE

//==============================================================================
//   USBCAM FUNCTIONS
//==============================================================================

//==============================================================================
//   USBCHARGE FUNCTIONS
//==============================================================================
#define USB_CHARGE_FUNCTION             DISABLE
#define USB_CHARGE_VERIFY               DISABLE

//==============================================================================
//   USBDISK FUNCTIONS
//==============================================================================
#define _MSDC_MODE_                     _MSDC_SINGLE_

//==============================================================================
//   USBMSDC FUNCTIONS
//==============================================================================
#define MSDCVENDOR_NVT                  ENABLE

//==============================================================================
//   USBPRINT FUNCTIONS
//==============================================================================


//==============================================================================
//   UI FUNCTIONS
//==============================================================================

//..............................................................................
// UI Resource Storage Config
#define _LANG_STORE_                    _INSIDECODE_
#define _JPG_STORE_                     _INSIDECODE_

//..............................................................................
// UI Rendering Config
//image
#define DRAW_TXT_BY_ICON                ENABLE
//palette
#define PALETTE_PATCH_BRIGHTNESS        DISABLE
//string
#define MULTILANG_FUNCTION              DISABLE
#define ARABICLEFT_FUNCTION             DISABLE
#define ARABICRIGHT_FUNCTION            DISABLE
//font
#define USE_CHFONT_FUNC                 DISABLE

//..............................................................................
// Menu item Config(if no menu wnd ,remove all)
#define MENUTITLE12                     ENABLE
#define MENU_PTP_FUNC                   DISABLE
#define MENU_PRINTER_FUNC               DISABLE
#define MENU_PCCAM_FUNC                 ENABLE
#define MENU_DUALDISP_FUNC              DISABLE
#define SUBMENU_ARROW                   DISABLE
#define QFUNC_UI_NEW                    DISABLE

//..............................................................................
//this flag is for remove wnd or resource,create new project,if the file would refer to removed resource,need use this flag
#define _USR_WND_                       DISABLE      //if remove all wnd source code, need DISABLE  _USR_WND_
#if _FPGA_EMULATION_ == ENABLE
#define PST_FUNC                        ENABLE
#else
#define PST_FUNC                        ENABLE
#endif

#define LOW_POWER_TIMELAPSE_REC         DISABLE
#define FILEDB_TMLPS                    "TMLPS"         // the max length should not exceeds 16
#define FILEDB_TMLPS_ROOT               "A:\\TMLPS\\"   // the max length should not exceeds 16

#define HTTP_LIVEVIEW_FUNC              DISABLE
#define RTSP_LIVEVIEW_FUNC              DISABLE
#define WIFI_AP_FUNC                    ENABLE
#define WIFI_FTP_FUNC                   DISABLE

#define HTTP_MJPG_W_4                   640
#define HTTP_MJPG_H_3                   480

#define HTTP_MJPG_W_16                  640
#define HTTP_MJPG_H_9                   360
#define _SCREENNAIL_SIZE_               _SCREENNAIL_SIZE_VGA_

#define _QUICKVIEW_SIZE_                _QUICKVIEW_SIZE_SUIT_FOR_DISP


//RTSP_LIVEVIEW_FUNC depends on HTTP_LIVEVIEW_FUNC
#if(RTSP_LIVEVIEW_FUNC==ENABLE)
#undef HTTP_LIVEVIEW_FUNC
#define HTTP_LIVEVIEW_FUNC              ENABLE
#undef AUTOSLEEP_FUNCTION
#define AUTOSLEEP_FUNCTION              DISABLE
#undef AUTOPWROFF_FUNCTION
#define AUTOPWROFF_FUNCTION             DISABLE
#undef HTTP_MJPG_W_4
#undef HTTP_MJPG_H_3
#undef HTTP_MJPG_W_16
#undef HTTP_MJPG_H_9
#define HTTP_MJPG_W_4                   1920
#define HTTP_MJPG_H_3                   1080
#define HTTP_MJPG_W_16                  1920
#define HTTP_MJPG_H_9                   1080
#endif

#if(WIFI_AP_FUNC==ENABLE)
#undef HTTP_LIVEVIEW_FUNC
#define HTTP_LIVEVIEW_FUNC              ENABLE
#undef RTSP_LIVEVIEW_FUNC
#define RTSP_LIVEVIEW_FUNC              DISABLE  //for RTSP debug only

#define HTTP_LIVEVIEW                   1
#define RTSP_LIVEVIEW                   2
#define DUAL_REC_HTTP_LIVEVIEW          3
#define MOVIE_LIVEVIEW                  RTSP_LIVEVIEW

#if(MOVIE_LIVEVIEW==HTTP_LIVEVIEW)
#undef _MULTI_RECORD_FUNC_
#define _MULTI_RECORD_FUNC_             DISABLE
#else  //RTSP_LIVEVIEW and DUAL_REC_HTTP_LIVEVIEW
#undef _MULTI_RECORD_FUNC_
#define _MULTI_RECORD_FUNC_             ENABLE
#endif

#undef HTTP_MJPG_W_4
#undef HTTP_MJPG_H_3
#undef HTTP_MJPG_W_16
#undef HTTP_MJPG_H_9
#define HTTP_MJPG_W_4                   640
#define HTTP_MJPG_H_3                   480
#define HTTP_MJPG_W_16                  640
#define HTTP_MJPG_H_9                   360
#endif

#define WIFI_APP_MANUFACTURER           "NVT"
#define WIFI_APP_MODLE                  "CarDV_WiFi"
#define WIFI_APP_DOWNLOAD_URL           "http://115.29.201.46:8020/download/filedesc.xml"

//#NT#2016/05/03#Lincy Lin -begin
//#NT#Support logfile function
#define LOGFILE_FUNC                    DISABLE
//#NT#2016/05/03#Lincy Lin -end

//#NT#2016/08/08#Lincy Lin -begin
//#NT#Support HTTP liveview datastamp function
#define HTTP_LIVEVIEW_DATASTAMP_FUNC    DISABLE
//#NT#2016/08/08#Lincy Lin -end

//#NT#2016/10/31#YongChang -begin
//#NT#Support usocket client
#define USOCKET_CLIENT                  DISABLE
//#NT#2016/10/31#YongChang -end

//#NT#2016/10/31#YongChang -begin
//#NT#Auto switch to station mode after novacam connection
//#NT#USOCKET_CLIENT must be enabled if AUTO_AP_TO_STA is enabled
#define AUTO_AP_TO_STA                  DISABLE
//#NT#2016/10/31#YongChang -end
//#NT#2016/12/28#Charlie Chang -begin
//#NT# support json parser
#define JSON_PARSER		DISABLE
//#NT#2016/12/28#Charlie Chang -end
#endif //_PRJCFG_C12_H_
