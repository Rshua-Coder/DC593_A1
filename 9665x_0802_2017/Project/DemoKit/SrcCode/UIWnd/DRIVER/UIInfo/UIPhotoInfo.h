#ifndef UI_PHOTOINFO_H
#define UI_PHOTOINFO_H

#include "PrjCfg.h"
#include "UIInfo.h"
// user should check enum depend on different project

//-----------------------------------------------------------------------------
// Mode state
//-----------------------------------------------------------------------------
/*
typedef enum
{
    MODE_AUTO = 0,
    MODE_MANUAL,
    MODE_PANORAMA,
    MODE_STAB,
    MODE_SCENE,
    MODE_MOVIE,
    MODE_ASCN,
    MODE_PORTRAIT,
    MODE_SETTING_MAX
}MODE_SETTING;
*/

//-----------------------------------------------------------------------------
// Scene mode state
//-----------------------------------------------------------------------------
typedef enum
{
    SCENE_LANDSCAPE = 0,
//    SCENE_SPORTS,
//    SCENE_BEACH,
    SCENE_SUNSET,
    SCENE_FIREWORKS,
    SCENE_NIGHTLANDSCAPE,
    SCENE_NIGHTPOTRAIT,
//    SCENE_PARTY,
    SCENE_CHILDREN,
    SCENE_INDOOR,
    SCENE_SNOW,
    SCENE_ID,
    SCENE_MUSEUM,
    SCENE_PHOTOFRAME,
    SCENE_PANNINGSHOT,
    SCENE_TEXT,
    SCENE_LEAF,
    SCENE_GLASS,
    SCENE_FISHEYE,
    SCENE_SKETCH,
//    SCENE_PORTRAIT,
//    SCENE_AUTO,
    SCENE_BACKPORTRAIT,
    SCENE_FLOWER,
    SCENE_SETTING_MAX
}SCENE_SETTING;


//-----------------------------------------------------------------------------
// Image Effect mode
//-----------------------------------------------------------------------------
typedef enum
{
    IMAGEEFFECT_SKETCH = 0,
    IMAGEEFFECT_FISHEYE,
    IMAGEEFFECT_COLORPENCIL,
    IMAGEEFFECT_WDR,
    IMAGEEFFECT_ANTISHAKE,
    IMAGEEFFECT_HDR,
    IMAGEEFFECT_SETTING_MAX
}IMAGEEFFECT_SETTING;


//-----------------------------------------------------------------------------
// Macro State
//-----------------------------------------------------------------------------
#if 0
typedef enum
{
    MACRO_ON = 0x00,
    MACRO_OFF,
    MACRO_SETTING_MAX
}MACRO_SETTIMG;
#endif

//-----------------------------------------------------------------------------
// Selftimer Time
//-----------------------------------------------------------------------------
#if 0
typedef enum
{
    SELFTIMER_OFF = 0x00,
    SELFTIMER_ON_2S,
    SELFTIMER_ON_10S,
    //SELFTIMER_UNLIMITED,
    SELFTIMER_SETTING_MAX
}SELFTIMER_SETTIMG;
#endif

//-----------------------------------------------------------------------------
// Flash state
//-----------------------------------------------------------------------------
#if 0
typedef enum
{
    FLASH_OFF = 0,
    FLASH_ON,
    FLASH_SLOW_FLASH_EYE,
    FLASH_SLOW_FLASH,
    FLASH_RED_EYE,
    FLASH_AUTO,
    FLASH_SETTING_MAX
}FLASH_SETTING;
#endif

//-----------------------------------------------------------------------------
// Image Size
//-----------------------------------------------------------------------------
#if 0
#if(_MAX_IMAGE_SIZE_ == _MAX_IMAGE_SIZE_14M_)
typedef enum
{
    SIZE_14M = 0,
    SIZE_12M32,
    SIZE_10M169,
    SIZE_8M,
    SIZE_5M,
    SIZE_3M,
    SIZE_VGA,
    SIZE_SETTING_MAX
}SIZE_SETTING;
#elif(_MAX_IMAGE_SIZE_ == _MAX_IMAGE_SIZE_12M_)
typedef enum
{
    SIZE_12M = 0,
    SIZE_10M32,
    SIZE_9M169,
    SIZE_10M,
    SIZE_8M,
    SIZE_5M,
    SIZE_3M,
    SIZE_VGA,
    SIZE_SETTING_MAX
}SIZE_SETTING;
#else
typedef enum
{
    SIZE_10M = 0,
    SIZE_9M32,
    SIZE_8M,
    SIZE_7M169,
    SIZE_5M,
    SIZE_3M,
    SIZE_VGA,
    SIZE_SETTING_MAX
}SIZE_SETTING;
#endif
#else
typedef enum
{
    SIZE_12M = 0,
    SIZE_10M,
    SIZE_8M,
    SIZE_5M,
    SIZE_3M,
    SIZE_2M169,
    SIZE_VGA,
    SIZE_1M,
    SIZE_SETTING_MAX,
    // For compiling issue
    SIZE_14M = SIZE_12M, // for sharing common code with GS650
    SIZE_10M169 = SIZE_2M169, // for sharing common code with GS650
    SIZE_12M32 = SIZE_SETTING_MAX // for compiling issue
}SIZE_SETTING;
#endif

//-----------------------------------------------------------------------------
// Image Quality
//-----------------------------------------------------------------------------
typedef enum
{
    QUALITY_BQ = 0,
    QUALITY_FQ,
    QUALITY_NQ,
    QUALITY_SETTING_MAX
}QUALITY_SETTING;

//-----------------------------------------------------------------------------
// White Balance
//-----------------------------------------------------------------------------
#if 0
typedef enum
{
    WB_AUTO = 0,
    WB_DAYLIGHT,
    WB_CLOUDY,
    WB_FLUORESCENT1,
    WB_FLUORESCENT2,
    WB_INCAND,
    WB_MANUAL,
    WB_SETTING_MAX
}WB_SETTING;
#endif
//-----------------------------------------------------------------------------
// Image Color
//-----------------------------------------------------------------------------
typedef enum
{
    COLOR_STANDARD = 0,
    COLOR_VIVID,
    COLOR_BW,
    COLOR_SEPIA,
    /*
    COLOR_CBLUE,
    COLOR_CRED,
    COLOR_CGREEN,
    COLOR_CYELLOW,
    COLOR_CPURPLE,
    */
    COLOR_SETTING_MAX
}COLOR_SETTING;

//-----------------------------------------------------------------------------
// ISO
//-----------------------------------------------------------------------------
#if 0
typedef enum
{
    ISO_AUTO = 0,
    ISO_64,
    ISO_100,
    ISO_200,
    ISO_400,
    ISO_800,
    ISO_1600,
    ISO_3200,
    ISO_SETTING_MAX
}ISO_SETTING;
#endif

//-----------------------------------------------------------------------------
// Auto Focus
//-----------------------------------------------------------------------------
typedef enum
{
    AFWINDOW_SINGLE = 0,
    AFWINDOW_MULTI,
    AFWINDOW_SETTING_MAX
}AFWINDOW_SETTING;

//-----------------------------------------------------------------------------
// Cont. AF
//-----------------------------------------------------------------------------
typedef enum
{
    CONT_AF_ON = 0,
    CONT_AF_OFF,
    CONT_AF_SETTING_MAX
}CONT_AF_SETTING;

//-----------------------------------------------------------------------------
// AF Assist Beam
//-----------------------------------------------------------------------------
typedef enum
{
    AFBEAM_OFF = 0,
    AFBEAM_ON,
    AFBEAM_SETTING_MAX
}AFBEAN_SETTING;

//-----------------------------------------------------------------------------
// Metering
//-----------------------------------------------------------------------------
#if 0
typedef enum
{
    METERING_SPOT = 0,
    METERING_CENTER,
    METERING_AIAE,
    METERING_SETTING_MAX
}METERING_SETTING;
#endif


//-----------------------------------------------------------------------------
// Capture Mode
//-----------------------------------------------------------------------------
typedef enum
{
    CAPTUREMODE_BURST = 0,
    CAPTUREMODE_CONTINUOUS,
    CAPTUREMODE_POST_CONT,
    CAPTUREMODE_SETTING_MAX
}CAPTUREMODE_SETTING;

//-----------------------------------------------------------------------------
// Quick View
//-----------------------------------------------------------------------------
typedef enum
{
    PREVIEW_OFF = 0,
    PREVIEW_1_SEC,
    PREVIEW_2_SEC,
    PREVIEW_3_SEC,
    PREVIEW_SETTING_MAX
}PREVIEW_SETTING;

//-----------------------------------------------------------------------------
// Digital Zoom
//-----------------------------------------------------------------------------
typedef enum
{
    DZOOM_OFF = 0,
    DZOOM_ON,
    DZOOM_SETTING_MAX
}DZOOM_SETTING;

/*
//-----------------------------------------------------------------------------
// B shoot
//-----------------------------------------------------------------------------
typedef enum
{
    B_SHOOT_OFF = 0,
    B_SHOOT_ON,
    B_SHOOT_SETTING_MAX
}B_SHOOT_SETTING;
*/

//-----------------------------------------------------------------------------
// Blink detection
//-----------------------------------------------------------------------------
typedef enum
{
    BLINKDET_OFF = 0,
    BLINKDET_ON,
    BLINKDET_SETTING_MAX
}BLINKDET_SETTING;


//-----------------------------------------------------------------------------
// Date Imprint State
//-----------------------------------------------------------------------------
typedef enum
{
    DATEIMPRINT_OFF = 0,
    DATEIMPRINT_DATE,
    DATEIMPRINT_DATE_TIME,
    DATEIMPRINT_SETTING_MAX
}DATEIMPRINT_SETTING;


//-----------------------------------------------------------------------------
// Blink detection
//-----------------------------------------------------------------------------
typedef enum
{
    PAN_AUTO = 0,
    PAN_MANIAL,
    PAN_SETTING_MAX
}PAN_SETTING;


typedef enum
{
    DZOOM_10X = 0,   // 0
    DZOOM_12X,
    DZOOM_14X,
    DZOOM_16X,
    DZOOM_18X,
    DZOOM_20X,      // 5
    DZOOM_22X,
    DZOOM_24X,
    DZOOM_26X,
    DZOOM_28X,
    DZOOM_30X,      // 10
    DZOOM_32X,
    DZOOM_34X,
    DZOOM_36X,
    DZOOM_38X,
    DZOOM_40X,      // 15
    DZOOM_45X,
    DZOOM_50X,
    DZOOM_55X,
    DZOOM_60X,
    DZOOM_X_SETTING_MAX  // 20
}DZOOM_X_SETTING;



//-----------------------------------------------------------------------------
// Face Detection State
//-----------------------------------------------------------------------------
#if 0
typedef enum
{
    FD_OFF = 0x00,
    FD_ON,
    FD_SMILE,
    FD_SETTING_MAX
}FD_SETTIMG;
#endif

//-----------------------------------------------------------------------------
// Obj Track State
//-----------------------------------------------------------------------------
typedef enum
{
    OBJ_TRACK_OFF = 0x00,
    OBJ_TRACK_ON,
    OBJ_TRACK_SETTING_MAX
}OBJ_TRACK_SETTIMG;
// Cont Shot State
//-----------------------------------------------------------------------------
typedef enum
{
    CONT_SHOT_STATE_OFF = 0x00,
    CONT_SHOT_STATE_ON,
    CONT_SHOT_STATE_SETTING_MAX
}CONT_SHOT_STATE_SETTIMG;

//-----------------------------------------------------------------------------
// Cont Shot Type
//-----------------------------------------------------------------------------
typedef enum
{
    CONT_SHOT_OFF = 0,
    CONT_SHOT_INFINITE,
    CONT_SHOT_10,
    CONT_SHOT_3LAST,
    CONT_SHOT_TIMELAPSE,
    CONT_SHOT_SETTING_MAX
}CONT_SHOT_SETTING;

//-----------------------------------------------------------------------------
// Timelapse Type
//-----------------------------------------------------------------------------
typedef enum
{
    TIMELAPSE_30S,
    TIMELAPSE_1MIN,
    TIMELAPSE_5MIN,
    TIMELAPSE_10MIN,
    TIMELAPSE_SETTING_MAX
}TIMELAPSE_SETTING;

typedef enum
{
    ZOOM_IF_OPTICAL = 0,
    ZOOM_IF_DIGITAL,
    ZOOM_IF_BOTH
}ZOOM_IF_INDEX;

//-----------------------------------------------------------------------------
// B shutter, Slow Shutter State
//-----------------------------------------------------------------------------
typedef enum
{
    BSHUTTER_OFF = 0,
    BSHUTTER_SETTING_MIN = 2,
    BSHUTTER_SETTING_MAX = 30
}BSHUTTER_SETTING;

//-----------------------------------------------------------------------------
// Saturation Level
//-----------------------------------------------------------------------------
typedef enum
{
    SATURATION_P1 = 0,
    SATURATION_NORNAL,
    SATURATION_N1,
    SATURATION_SETTING_MAX
}SATURATION_SETTING;

//-----------------------------------------------------------------------------
// Sharpness Level
//-----------------------------------------------------------------------------
#if 0
typedef enum
{
    SHARPNESS_P2 = 0,
    SHARPNESS_P1,
    SHARPNESS_NORMAL,
    SHARPNESS_N1,
    SHARPNESS_N2,
    SHARPNESS_SETTING_MAX
}SHARPNESS_SETTING;
#endif

//-----------------------------------------------------------------------------
// Contrast Level
//-----------------------------------------------------------------------------
typedef enum
{
    CONTRAST_P4 = 0,
    CONTRAST_P3,
    CONTRAST_P2,
    CONTRAST_P1,
    CONTRAST_NORMAL,
    CONTRAST_N1,
    CONTRAST_N2,
    CONTRAST_N3,
    CONTRAST_N4,
    CONTRAST_SETTING_MAX
}CONTRAST_SETTING;

//-----------------------------------------------------------------------------
// Grid State
//-----------------------------------------------------------------------------
typedef enum
{
    GRID_OFF = 0,
    GRID_ON,
    GRID_SETTING_MAX
}GRID_SETTING;


//-----------------------------------------------------------------------------
// Histogram State
//-----------------------------------------------------------------------------
typedef enum
{
    HISTOGRAM_OFF = 0,
    HISTOGRAM_ON,
    HISTOGRAM_SETTING_MAX
}HISTOGRAM_SETTING;


//-----------------------------------------------------------------------------
// Image Ratio
//-----------------------------------------------------------------------------

typedef enum
{
    IMAGERATIO_9_16 = 0,
    IMAGERATIO_2_3,
    IMAGERATIO_3_4,
    IMAGERATIO_1_1,
    IMAGERATIO_4_3,
    IMAGERATIO_3_2,
    IMAGERATIO_16_9,
    IMAGERATIO_MAX_CNT,
    ENUM_DUMMY4WORD(IMAGERATIO_SETTING)
} IMAGERATIO_SETTING;


#endif //UI_PHOTOINFO_H

