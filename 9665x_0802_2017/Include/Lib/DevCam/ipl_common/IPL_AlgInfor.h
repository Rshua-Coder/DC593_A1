/**
    Public header file for image setting information

    This file is the header file that define the API and data type for image information.

    @file       IPL_AlgInfor.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IPL_ALGINFOR_H_
#define _IPL_ALGINFOR_H_

#include "type.h"
#include "IPL_Utility.h"
#include "ErrorNo.h"

/** \addtogroup mIIPL */
//@{

#define IPL_INFOR_ERR   0xffffffff    ///< error code(IPL_AlgGetUIInfo())

#define SEL_NO_CHANG    0xF0


#define IPL_SEL_TAG   0x43210000

/**
    IPL informatin selection index(IPL_AlgSetUIInfo() or IPL_AlgGetUIInfo())
*/
typedef enum _IPL_SEL_ITEM
{
    IPL_SEL_DSCMODE = IPL_SEL_TAG,      ///<dsc mode, data type #SEL_DSCMODE
    IPL_SEL_SCENEMODE,                  ///<scene mode, data type #SEL_SCENEMODE
    IPL_SEL_SHARPNESS,                  ///<sharpness, data type #SEL_SHARPNESS
    IPL_SEL_SATURATION,                 ///<saturation, data type #SEL_SATURATION
    IPL_SEL_FLASHMODE,                  ///<flash mode, data type #SEL_FLASHMODE
    IPL_SEL_FREQUENCY,                  ///<ae anti-frequency, data type #SEL_FREQUENCY
    IPL_SEL_IMAGEEFFECT,                ///<color effect, data type #SEL_IMAGEEFFECT
    IPL_SEL_AEMODE,                     ///<ae detect mode, data type #SEL_AEMODE
    IPL_SEL_AEEV,                       ///<ae ev compensation, data type #SEL_AEEV
    IPL_SEL_WBMODE,                     ///<wb mode, data type #SEL_WBMODE
    IPL_SEL_AFMODE,                     ///<af mode, data type #SEL_AFMODE
    IPL_SEL_AFWINDOW,                   ///<af window selection, data type #SEL_AFWINDOW
    IPL_SEL_AFPOSITION,                 ///<af position, data type, #SEL_AFPOSITION
    IPL_SEL_ISOINDEX,                   ///<ae iso index, data type #SEL_ISOINDEX

    IPL_SEL_RAW_BITDEPTH_PRV,           ///<prv mode raw bits, data type #SEL_RAW_BITDEPTH
    IPL_SEL_RAW_BITDEPTH_CAP,           ///<cap mode raw bits, data type #SEL_RAW_BITDEPTH

    IPL_SEL_IMAGEFLIP,                  ///<image flip mode, data type #SEL_IMAGEFLIP

    IPL_SEL_STAMP,                      ///<prv stamp mode lock or unlock, data type #SEL_STAMP
    IPL_SEL_IEMODE,                     ///<prv image effect. data type #SEL_IEMODE
    IPL_SEL_3DNR,                       //prv 3DNR on/off(for demo), data type SEL_3DNR
    IPL_SEL_GDCCOMP,                    ///<prv GDC on/off, data type #SEL_GDCCOMP
    IPL_SEL_DISCOMP,                    ///<prv DIS on/off, data type #SEL_DISCOMP
    IPL_SEL_PHOTOFRAME,                 ///<prv photo frame on/off, data type #SEL_PHOTOFRAME

    IPL_SEL_PRVSENMODE,                 ///<prv sensor mode
    IPL_SEL_CAPSENMODE,                 ///<cap sensor mode
    IPL_SEL_COMMENT,                    ///<Scene mode comment
    IPL_SEL_VIDEO_FPS,                  ///<Video FPS
    IPL_SEL_VIDEO_SIZE,                 ///<Video SIZE
    IPL_SEL_SUPERRESOL,                 ///<cap super resolution on/off data type #SEL_SUPERRESOL
    IPL_SEL_RSC,                        ///<rolling shutter function on/off
    IPL_SEL_WDR,                        ///<WDR level
    IPL_SEL_MAX_CNT,
    ENUM_DUMMY4WORD(IPL_SEL_ITEM)
} IPL_SEL_ITEM;

#define IPL_SEL_MAX_ITEM  (IPL_SEL_MAX_CNT - IPL_SEL_TAG)

/**
    dsc mode
*/
typedef enum _SEL_DSCMODE
{
    SEL_DSCMODE_AUTO = 0,           ///<-
    SEL_DSCMODE_MANUAL,             ///<-
    SEL_DSCMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_DSCMODE)
} SEL_DSCMODE;

/**
    scene mode
*/
typedef enum _SEL_SCENEMODE
{
    SEL_SCENEMODE_AUTO = 0,             ///<-
    SEL_SCENEMODE_LANDSCAPE,            ///<-
    SEL_SCENEMODE_POTRAIT,              ///<-
    SEL_SCENEMODE_SPORT,                ///<-
    SEL_SCENEMODE_NIGHTSCENE,           ///<-
    SEL_SCENEMODE_AV,                   ///<-
    SEL_SCENEMODE_TV,                   ///<-
    SEL_SCENEMODE_MANUAL,               ///<-
    SEL_SCENEMODE_NIGHTPOTRAIT,         ///<-
    SEL_SCENEMODE_NIGHTLANDSCAPE,       ///<-
    SEL_SCENEMODE_PARTY,                ///<-
    SEL_SCENEMODE_SUNSET,               ///<-
    SEL_SCENEMODE_BACKLIGHT,            ///<-
    SEL_SCENEMODE_DOCUMENT,             ///<-
    SEL_SCENEMODE_FIREWORK,             ///<-
    SEL_SCENEMODE_BEACH,                ///<-
    SEL_SCENEMODE_BMODE,                ///<-
    SEL_SCENEMODE_PROGRAM,              ///<-
    SEL_SCENEMODE_HIGHISO,              ///<-
    SEL_SCENEMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_SCENEMODE)
} SEL_SCENEMODE;


/**
    sharpness
*/
typedef enum _SEL_SHARPNESS
{
    SEL_SHARPNESS_P1 = 0,           ///< sharpness +1
    SEL_SHARPNESS_NORMAL,           ///< sharpness +0
    SEL_SHARPNESS_N1,               ///< sharpness -1
    SEL_SHARPNESS_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_SHARPNESS)
} SEL_SHARPNESS;

/**
    saturation
*/
typedef enum _SEL_SATURATION
{
    SEL_SATURATION_P1 = 0,          ///< saturation +1
    SEL_SATURATION_NORMAL,          ///< saturation +0
    SEL_SATURATION_N1,              ///< saturation -1
    SEL_SATURATION_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_SATURATION)
} SEL_SATURATION;

/**
    flash mode
*/
typedef enum _SEL_FLASHMODE
{
    SEL_FLASHMODE_AUTO = 0,         ///< auto
    SEL_FLASHMODE_FORCEOFF,         ///< force off
    SEL_FLASHMODE_FORCEON,          ///< force on
    SEL_FLASHMODE_AUTOREDEYE,       ///< auto + red eye
    SEL_FLASHMODE_FORCEONREDEYE,    ///< force on + red eye
    SEL_FLASHMODE_NIGHTSTROBE,      ///< night strobe
    SEL_FLASHMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_FLASHMODE)
} SEL_FLASHMODE;

/**
    frequency
*/
typedef enum _SEL_FREQUENCY
{
    SEL_FREQUENCY_50HZ = 0,          ///< anti-freq 50Hz
    SEL_FREQUENCY_60HZ,              ///< anti-freq 60Hz
    SEL_FREQUENCY_AUTO,              ///< auto detect current freq
    SEL_FREQUENCY_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_FREQUENCY)
} SEL_FREQUENCY;

/**
    image effect
*/
typedef enum _SEL_IMAGEEFFECT
{
    SEL_IMAGEEFFECT_OFF = 0,            ///<-
    SEL_IMAGEEFFECT_BW,                 ///<-
    SEL_IMAGEEFFECT_SEPIA,              ///<-
    SEL_IMAGEEFFECT_VIVID,              ///<-
    SEL_IMAGEEFFECT_NEUTRAL,            ///<-
    SEL_IMAGEEFFECT_LOWSHARPING,        ///<-
    SEL_IMAGEEFFECT_ROCK,            ///<-
    SEL_IMAGEEFFECT_FASION2,            ///<-
    SEL_IMAGEEFFECT_CUSTOMMANUAL,       ///<-
    SEL_IMAGEEFFECT_RED,                ///<-
    SEL_IMAGEEFFECT_GREEN,              ///<-
    SEL_IMAGEEFFECT_BLUE,               ///<-
    SEL_IMAGEEFFECT_YELLOW,             ///<-
    SEL_IMAGEEFFECT_PURPLE,             ///<-
    SEL_IMAGEEFFECT_SKETCH,             ///<-
    SEL_IMAGEEFFECT_FISHEYE,            ///<-
    SEL_IMAGEEFFECT_COLORPENCIL,        ///<-
    SEL_IMAGEEFFECT_WDR,                ///<-
    SEL_IMAGEEFFECT_NEGATIVE_DEFOG,     ///<-
    SEL_IMAGEEFFECT_JAPANSTYLE,         ///<-
    SEL_IMAGEEFFECT_COOLGREEN,          ///<-
    SEL_IMAGEEFFECT_WARMYELLOW,         ///<-
    SEL_IMAGEEFFECT_D2D_SHADING,
    SEL_IMAGEEFFECT_D2D_FANTASY,
    SEL_IMAGEEFFECT_D2D_FISHEYE,
    SEL_IMAGEEFFECT_D2D_MIRROR,
    SEL_IMAGEEFFECT_D2D_ROCK,
    SEL_IMAGEEFFECT_D2D_SKETCH,
    SEL_IMAGEEFFECT_D2D_MINIATURE,
    SEL_IMAGEEFFECT_D2D_SPARKLE,

    SEL_IMAGEEFFECT_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_IMAGEEFFECT)
} SEL_IMAGEEFFECT;


/**
    Ae detect metrix
*/

typedef enum _SEL_AEMODE
{
    SEL_AEMODE_CENTERWEIGHTED = 0,      ///< center weighting
    SEL_AEMODE_SPOT,                    ///< spot
    SEL_AEMODE_MATRIX,                  ///< matic
    SEL_AEMODE_EVALUATIVE,              ///<-
    SEL_AEMODE_FACEDETECTION,           ///< falce
    SEL_AEMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_AEMODE)
} SEL_AEMODE;

/**
    SEL_AEEV
*/
typedef enum _SEL_AEEV
{
    SEL_AEEV_N3 = 0,            ///< ev -3
    SEL_AEEV_N8_3,              ///< ev -8/3
    SEL_AEEV_N5_2,              ///< ev -5/2
    SEL_AEEV_N7_3,              ///< ev -7/3
    SEL_AEEV_N2,                ///< ev -2
    SEL_AEEV_N5_3,              ///< ev -5/3
    SEL_AEEV_N3_2,              ///< ev -3/2
    SEL_AEEV_N4_3,              ///< ev -4/3
    SEL_AEEV_N1,                ///< ev -1
    SEL_AEEV_N2_3,              ///< ev -2/3
    SEL_AEEV_N1_2,              ///< ev -1/2
    SEL_AEEV_N1_3,              ///< ev -1/3
    SEL_AEEV_00,                ///< ev +0
    SEL_AEEV_P1_3,              ///< ev +1/3
    SEL_AEEV_P1_2,              ///< ev +1/2
    SEL_AEEV_P2_3,              ///< ev +2/3
    SEL_AEEV_P1,                ///< ev +1
    SEL_AEEV_P4_3,              ///< ev +4/3
    SEL_AEEV_P3_2,              ///< ev +3/2
    SEL_AEEV_P5_3,              ///< ev +5/3
    SEL_AEEV_P2,                ///< ev +2
    SEL_AEEV_P7_3,              ///< ev +7/3
    SEL_AEEV_P5_2,              ///< ev +5/2
    SEL_AEEV_P8_3,              ///< ev +8/3
    SEL_AEEV_P3,                ///< ev +3
    SEL_AEEV_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_AEEV)
} SEL_AEEV;


/**
    white balance mode
*/
typedef enum _SEL_WBMODE
{
    SEL_WBMODE_AUTO = 0,        ///<auto mode
    SEL_WBMODE_DAYLIGHT,        ///<daylight mode
    SEL_WBMODE_CLOUDY,          ///<cloudy mode
    SEL_WBMODE_TUNGSTEN,        ///<tugsten mode
    SEL_WBMODE_FLUORESCENT1,    ///<FL1 mode
    SEL_WBMODE_FLUORESCENT2,    ///<FL2 mode
    SEL_WBMODE_SUNSET,          ///<sunset mode
    SEL_WBMODE_BIRTHDAYPARTY,   ///<party mode
    SEL_WBMODE_CUSTOMER1,       ///<-
    SEL_WBMODE_CUSTOMER2,       ///<-
    SEL_WBMODE_CUSTOMER3,       ///<-
    SEL_WBMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_WBMODE)
}SEL_WBMODE;

/**
    focus mode
*/
typedef enum _SEL_AFMODE
{
    SEL_AFMODE_AUTO = 0,        ///<auto focus
    SEL_AFMODE_MACRO,           ///<marco mode
    SEL_AFMODE_SUPER_MACRO,     ///<super marco mode
    SEL_AFMODE_INIFINITE,       ///<-
    SEL_AFMODE_CONTINUEFOCUS,   ///<-
    SEL_AFMODE_FACEDETECTION,   ///<-
    SEL_AFMODE_MANUAL,          ///<-
    SEL_AFMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_AFMODE)
} SEL_AFMODE;


/**
    select af window mode
*/
typedef enum _SEL_AFWINDOW
{
    SEL_AFWINDOW_CENTER = 0,        ///<center
    SEL_AFWINDOW_AUTO,              ///<auto
    SEL_AFWINDOW_MANUALPOSITION,    ///<manual position. ref. #SEL_AFPOSITION
    SEL_AFWINDOW_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_AFWINDOW)
} SEL_AFWINDOW;

/**
    af window position
*/
typedef enum _SEL_AFPOSITION
{
    SEL_AFPOSITION_1 = 0,           ///<-
    SEL_AFPOSITION_2,               ///<-
    SEL_AFPOSITION_3,               ///<-
    SEL_AFPOSITION_4,               ///<-
    SEL_AFPOSITION_5,               ///<-
    SEL_AFPOSITION_6,               ///<-
    SEL_AFPOSITION_7,               ///<-
    SEL_AFPOSITION_8,               ///<-
    SEL_AFPOSITION_9,               ///<-
    SEL_AFPOSITION_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_AFPOSITION)
} SEL_AFPOSITION;


/**
    iso index
*/
typedef enum _SEL_ISOINDEX
{
    SEL_ISOINDEX_AUTO = 0,      ///<-
    SEL_ISOINDEX_50,            ///<-
    SEL_ISOINDEX_80,            ///<-
    SEL_ISOINDEX_100,           ///<-
    SEL_ISOINDEX_200,           ///<-
    SEL_ISOINDEX_400,           ///<-
    SEL_ISOINDEX_800,           ///<-
    SEL_ISOINDEX_1600,          ///<-
    SEL_ISOINDEX_3200,          ///<-
    SEL_ISOINDEX_6400,          ///<-
    SEL_ISOINDEX_12800,         ///<-
    SEL_ISOINDEX_25600,         ///<-
    SEL_ISOINDEX_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_ISOINDEX)
} SEL_ISOINDEX;


/**
    bayer bit number
*/
typedef enum _SEL_RAW_BITDEPTH
{
    SEL_RAW_BITDEPTH_8 = 0,     ///<-
    SEL_RAW_BITDEPTH_10,        ///<-
    SEL_RAW_BITDEPTH_12,        ///<-
    SEL_RAW_BITDEPTH_16,        ///<-
    SEL_RAW_BITDEPTH_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_RAW_BITDEPTH)
} SEL_RAW_BITDEPTH;


typedef enum _SEL_IMAGEFLIP
{
    SEL_IMAGEFLIP_OFF = 0,
    SEL_IMAGEFLIP_H,
    SEL_IMAGEFLIP_V,
    SEL_IMAGEFLIP_H_V,
    SEL_IMAGEFLIP_SEN_H,
    SEL_IMAGEFLIP_SEN_V,
    SEL_IMAGEFLIP_SEN_H_V,
    SEL_IMAGEFLIP_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_IMAGEFLIP)
} SEL_IMAGEFLIP;

/**
    data stamp on/off
*/
typedef enum _SEL_STAMP
{
    SEL_STAMP_OFF = 0,      ///<-
    SEL_STAMP_ON,           ///<-
    SEL_STAMP_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_STAMP)
} SEL_STAMP;

typedef enum _SEL_IEMODE
{
    SEL_IEMODE_NONE = 0,
    SEL_IEMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_IEMODE)
} SEL_IEMODE;

typedef enum _SEL_3DNR
{
    SEL_3DNR_OFF = 0,
    SEL_3DNR_ON,
    SEL_3DNR_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_3DNR)
} SEL_3DNR;

/**
    gdc on/off
*/
typedef enum _SEL_GDCCOMP
{
    SEL_GDCCOMP_OFF = 0,    ///<-
    SEL_GDCCOMP_ON,         ///<-
    SEL_GDCCOMP_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_GDCCOMP)
} SEL_GDCCOMP;

/**
    dis on/off
*/
typedef enum _SEL_DISCOMP
{
    SEL_DISCOMP_OFF = 0,    ///<-
    SEL_DISCOMP_ON,         ///<-
    SEL_DISCOMP_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_DISCOMP)
} SEL_DISCOMP;

typedef struct    t_SceneModeInfo{
    UINT32    SceneMode;
    UINT32    FlashMode;
    UINT32    EVComp;
    UINT32    Saturation;
    UINT32    Sharpness;
    UINT32    WBMode;
    UINT32    Focus;
    UINT32    Comment;
}  SceneModeInfo;


typedef enum
{
    SEL_COMMENT_NONE = 0x00,
    SEL_ENHANCE_SKIN = 0x01,
    SEL_ENHANCE_RED = 0x02,
    SEL_ENHANCE_GREEN = 0x04,
    SEL_ENHANCE_BLUE = 0x08,
    SEL_ENHANCE_CONTRAST = 0x10,
    SEL_SOFT_SKIN = 0x20,
    SEL_SCENECOM_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_SCENEMODE_COMMENT)
} SEL_SCENEMODE_COMMENT;

/**
    photo frame on/off
*/
typedef enum _SEL_PHOTOFRAME
{
    SEL_PHOTOFRAME_OFF = 0,     ///<-
    SEL_PHOTOFRAME_ON,          ///<-
    SEL_PHOTOFRAME_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_PHOTOFRAME)
} SEL_PHOTOFRAME;

/**
    video resolution
*/
typedef enum _SEL_VIDEOSIZE
{
    SEL_VIDEOSIZE_1920x1080 = 0,    ///<-
    SEL_VIDEOSIZE_1280x960,         ///<-
    SEL_VIDEOSIZE_1280x720,         ///<-
    SEL_VIDEOSIZE_848x480,          ///<-
    SEL_VIDEOSIZE_640x480,          ///<-
    SEL_VIDEOSIZE_FULLRES,          ///<-
    SEL_VIDEOSIZE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_VIDEOSIZE)
} SEL_VIDEOSIZE;

/**
    video fps
*/
typedef enum _SEL_VIDEOFPS
{
    SEL_VIDEOFPS_10 = 0,        ///<-
    SEL_VIDEOFPS_15,            ///<-
    SEL_VIDEOFPS_20,            ///<-
    SEL_VIDEOFPS_24,            ///<-
    SEL_VIDEOFPS_25,            ///<-
    SEL_VIDEOFPS_30,            ///<-
    SEL_VIDEOFPS_45,            ///<-
    SEL_VIDEOFPS_50,            ///<-
    SEL_VIDEOFPS_60,            ///<-
    SEL_VIDEOFPS_100,           ///<-
    SEL_VIDEOFPS_120,           ///<-
    SEL_VIDEOFPS_150,           ///<-
    SEL_VIDEOFPS_200,           ///<-
    SEL_VIDEOFPS_240,           ///<-
    SEL_VIDEOFPS_FULLRES,       ///<-
    SEL_VIDEOFPS_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_VIDEOFPS)
} SEL_VIDEOFPS;

/**
    super resolution on/off
*/
typedef enum
{
    SEL_SUPERRESOL_OFF = 0,     ///<-
    SEL_SUPERRESOL_ON,          ///<-
    SEL_SUPERRESOL_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_SUPERRESOL)
} SEL_SUPERRESOL;

/**
    rolling shutter correction on/off
*/
typedef enum
{
    SEL_RSC_OFF = 0,            ///<-
    SEL_RSC_ON,                 ///<-
    //#NT#2015/01/29#liwei cheng -begin
    //#NT#
    SEL_RSC_PAUSE,
    //#NT#2015/01/29#liwei cheng -end
    SEL_RSC_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_RSC)
} SEL_RSC;

/**
    wdr level
*/
typedef enum _SEL_WDR
{
    SEL_WDR_LV0 = 0,        ///<-
    SEL_WDR_LV1,            ///<-
    SEL_WDR_LV2,            ///<-
    SEL_WDR_LV3,            ///<-
    SEL_WDR_LV4,            ///<-
    SEL_WDR_LV5,            ///<-
    SEL_WDR_LV6,            ///<-
    SEL_WDR_LV7,            ///<-
    SEL_WDR_LV8,            ///<-
    SEL_WDR_LV9,            ///<-
    SEL_WDR_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_WDR)
} SEL_WDR;


/**
     IPL Get UI Information

     @param[in] Id    IPL process id
     @param[in] SelIdx    UI Select item

     @return
        -b UINT32: select item information.
*/
extern UINT32 IPL_AlgGetUIInfo2(IPL_PROC_ID Id, IPL_SEL_ITEM SelIdx);

/**
     IPL Set UI Infomation

     @param[in] Id    IPL process id
     @param[in] SelIdx    UI Select item
     @param[in] Value     information for select item

     @return Description of data returned.
         - @b E_OK
         - @b E_SYS
*/
extern ER IPL_AlgSetUIInfo2(IPL_PROC_ID Id, IPL_SEL_ITEM SelIdx, UINT32 Value);

//for compatible single process
#define IPL_AlgGetUIInfo(SelIdx) IPL_AlgGetUIInfo2(IPL_ID_1, SelIdx)
#define IPL_AlgSetUIInfo(SelIdx, Value) IPL_AlgSetUIInfo2(IPL_ID_1, SelIdx, Value)

//@}
#endif //_IPL_ALGINFOR_H_
