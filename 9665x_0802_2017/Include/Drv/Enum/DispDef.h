
#ifndef _DISPDEF_H_
#define _DISPDEF_H_

#include "Type.h"

//Display device
#define PRIMARY_DISPTYPE_LCD    0
#define PRIMARY_DISPTYPE_TV     1   //Maybe NTSC, PAL or HDMI

#define DISP_LASTMODE    0xffffffff

//Display mode
typedef enum
{
    DISP_LCDMODE_RGB_SERIAL           =    0,     ///< LCD MODE is RGB Serial or UPS051
    DISP_LCDMODE_RGB_PARALL           =    1,     ///< LCD MODE is RGB Parallel (888)
    DISP_LCDMODE_YUV640               =    2,     ///< LCD MODE is YUV640
    DISP_LCDMODE_YUV720               =    3,     ///< LCD MODE is YUV720
    DISP_LCDMODE_RGBD360              =    4,     ///< LCD MODE is RGB Dummy 360
    DISP_LCDMODE_RGBD320              =    5,     ///< LCD MODE is RGB Dummy 320
    DISP_LCDMODE_RGB_THROUGH          =    6,     ///< LCD MODE is RGB through mode
    DISP_LCDMODE_CCIR601              =    7,     ///< LCD MODE is CCIR601
    DISP_LCDMODE_CCIR656              =    8,     ///< LCD MODE is CCIR656
    DISP_LCDMODE_RGB_PARALL666        =    9,     ///< LCD MODE is RGB Parallel 666
    DISP_LCDMODE_RGB_PARALL565        =   10,     ///< LCD MODE is RGB Parallel 565
    DISP_LCDMODE_RGB_PARALL_DELTA     =   11,     ///< LCD MODE is RGB Parallel Delta

    ENUM_DUMMY4WORD(DISP_LCDMODE)
} DISP_LCDMODE;

#define TVCLASS(c)          ((UINT32)(c)) & 0xff000000

typedef enum
{
    DISP_TVMODE_NTSC                = 0,          ///< TV MODE is NTSC (Video format is 640x448 i60 or p30)
    DISP_TVMODE_PAL                 = 1,          ///< TV MODE is PAL (Video format is 640x544 i50 or p25)

    ENUM_DUMMY4WORD(DISP_TVMODE)
} DISP_TVMODE;

typedef enum
{
    DISP_HDMIMODE_640X480P60         =1,     ///< HDMI Video format is 640x480 & Progressive 60fps
    DISP_HDMIMODE_720X480P60         =2,     ///< HDMI Video format is 720x480 & Progressive 60fps & 4:3
    DISP_HDMIMODE_720X480P60_16X9    =3,     ///< HDMI Video format is 720x480 & Progressive 60fps & 16:9
    DISP_HDMIMODE_1280X720P60        =4,     ///< HDMI Video format is 1280x720 & Progressive 60fps
    DISP_HDMIMODE_1920X1080I60       =5,     ///< HDMI Video format is 1920x1080 & Interlaced 60fps
    DISP_HDMIMODE_720X480I60         =6,     ///< HDMI Video format is 720x480 & Interlaced 60fps
    DISP_HDMIMODE_720X480I60_16X9    =7,     ///< HDMI Video format is 720x480 & Interlaced 60fps & 16:9
    DISP_HDMIMODE_720X240P60         =8,     ///< HDMI Video format is 720x240 & Progressive 60fps
    DISP_HDMIMODE_720X240P60_16X9    =9,     ///< HDMI Video format is 720x240 & Progressive 60fps & 16:9
    DISP_HDMIMODE_1440X480I60        =10,    ///< HDMI Video format is 1440x480 & Interlaced 60fps
    DISP_HDMIMODE_1440X480I60_16X9   =11,    ///< HDMI Video format is 1440x480 & Interlaced 60fps & 16:9
    DISP_HDMIMODE_1440X240P60        =12,    ///< HDMI Video format is 1440x240 & Progressive 60fps
    DISP_HDMIMODE_1440X240P60_16X9   =13,    ///< HDMI Video format is 1440x240 & Progressive 60fps & 16:9
    DISP_HDMIMODE_1440X480P60        =14,    ///< HDMI Video format is 1440x480 & Progressive 60fps
    DISP_HDMIMODE_1440X480P60_16X9   =15,    ///< HDMI Video format is 1440x480 & Progressive 60fps & 16:9
    DISP_HDMIMODE_1920X1080P60       =16,    ///< HDMI Video format is 1920x1080 & Progressive 60fps
    DISP_HDMIMODE_720X576P50         =17,    ///< HDMI Video format is 720x576 & Progressive 50fps
    DISP_HDMIMODE_720X576P50_16X9    =18,    ///< HDMI Video format is 720x576 & Progressive 50fps & 16:9
    DISP_HDMIMODE_1280X720P50        =19,    ///< HDMI Video format is 1280x720 & Progressive 50fps
    DISP_HDMIMODE_1920X1080I50       =20,    ///< HDMI Video format is 1920x1080 & Interlaced 50fps
    DISP_HDMIMODE_720X576I50         =21,    ///< HDMI Video format is 720x576 & Interlaced 50fps
    DISP_HDMIMODE_720X576I50_16X9    =22,    ///< HDMI Video format is 720x576 & Interlaced 50fps & 16:9
    DISP_HDMIMODE_720X288P50         =23,    ///< HDMI Video format is 720x288 & Progressive 50fps
    DISP_HDMIMODE_720X288P50_16X9    =24,    ///< HDMI Video format is 720x288 & Progressive 50fps & 16:9
    DISP_HDMIMODE_1440X576I50        =25,    ///< HDMI Video format is 1440x576 & Interlaced 50fps
    DISP_HDMIMODE_1440X576I50_16X9   =26,    ///< HDMI Video format is 1440x576 & Interlaced 50fps & 16:9
    DISP_HDMIMODE_1440X288P50        =27,    ///< HDMI Video format is 1440x288 & Progressive 50fps
    DISP_HDMIMODE_1440X288P50_16X9   =28,    ///< HDMI Video format is 1440x288 & Progressive 50fps & 16:9
    DISP_HDMIMODE_1440X576P50        =29,    ///< HDMI Video format is 1440x576 & Progressive 50fps
    DISP_HDMIMODE_1440X576P50_16X9   =30,    ///< HDMI Video format is 1440x576 & Progressive 50fps & 16:9
    DISP_HDMIMODE_1920X1080P50       =31,    ///< HDMI Video format is 1920x1080 & Progressive 50fps
    DISP_HDMIMODE_1920X1080P24       =32,    ///< HDMI Video format is 1920x1080 & Progressive 24fps
    DISP_HDMIMODE_1920X1080P25       =33,    ///< HDMI Video format is 1920x1080 & Progressive 25fps
    DISP_HDMIMODE_1920X1080P30       =34,    ///< HDMI Video format is 1920x1080 & Progressive 30fps
    DISP_HDMIMODE_1920X1080I50_VT1250=39,    ///< HDMI Video format is 1920x1080 & Interlaced  50fps & V-total is 1250 lines
    DISP_HDMIMODE_1920X1080I100      =40,    ///< HDMI Video format is 1920x1080 & Interlaced  100fps
    DISP_HDMIMODE_1280X720P100       =41,    ///< HDMI Video format is 1280X720  & Progressive 100fps
    DISP_HDMIMODE_720X576P100        =42,    ///< HDMI Video format is 720X576   & Progressive 100fps
    DISP_HDMIMODE_720X576P100_16X9   =43,    ///< HDMI Video format is 720X576   & Progressive 100fps & 16:9
    DISP_HDMIMODE_720X576I100        =44,    ///< HDMI Video format is 720X576  & Interlaced 100fps
    DISP_HDMIMODE_720X576I100_16X9   =45,    ///< HDMI Video format is 720X576  & Interlaced 100fps & 16:9
    DISP_HDMIMODE_1920X1080I120      =46,    ///< HDMI Video format is 1920X1080 & Interlaced 120fps
    DISP_HDMIMODE_1280X720P120       =47,    ///< HDMI Video format is 1280X720  & Progressive 120fps
    DISP_HDMIMODE_720X480P120        =48,    ///< HDMI Video format is 720X480   & Progressive 120fps
    DISP_HDMIMODE_720X480P120_16X9   =49,    ///< HDMI Video format is 720X480   & Progressive 120fps & 16:9
    DISP_HDMIMODE_720X480I120        =50,    ///< HDMI Video format is 720X480  & Interlaced 120fps
    DISP_HDMIMODE_720X480I120_16X9   =51,    ///< HDMI Video format is 720X480  & Interlaced 120fps & 16:9
    DISP_HDMIMODE_720X576P200        =52,    ///< HDMI Video format is 720X576  & Progressive 200fps
    DISP_HDMIMODE_720X576P200_16X9   =53,    ///< HDMI Video format is 720X576  & Progressive 200fps & 16:9
    DISP_HDMIMODE_720X576I200        =54,    ///< HDMI Video format is 720X576  & Interlaced 200fps
    DISP_HDMIMODE_720X576I200_16X9   =55,    ///< HDMI Video format is 720X576  & Interlaced 200fps & 16:9
    DISP_HDMIMODE_720X480P240        =56,    ///< HDMI Video format is 720X480  & Progressive 240fps
    DISP_HDMIMODE_720X480P240_16X9   =57,    ///< HDMI Video format is 720X480  & Progressive 240fps & 16:9
    DISP_HDMIMODE_720X480I240        =58,    ///< HDMI Video format is 720X480  & Interlaced 240fps
    DISP_HDMIMODE_720X480I240_16X9   =59,    ///< HDMI Video format is 720X480  & Interlaced 240fps & 16:9
    DISP_HDMIMODE_MAX                =60,

    ENUM_DUMMY4WORD(DISP_HDMIMODE)
} DISP_HDMIMODE;


typedef enum
{
    AUD_HDMIMODE_32KHZ               =0,     ///< HDMI input Audio is 32KHz
    AUD_HDMIMODE_44_1KHZ             =1,     ///< HDMI input Audio is 44.1KHz
    AUD_HDMIMODE_48KHZ               =2,     ///< HDMI input Audio is 48KHz

    ENUM_DUMMY4WORD(AUD_HDMIMODE)
} AUD_HDMIMODE;

#endif //_DISPDEF_H_

