#include "Type.h"
#include "IPL_AlgInfor.h"
#include "ImgCapInfor.h"
#include "UIPhotoObj.h"
#include "UIPhotoInfo.h"
#include "UIMode.h"
#include "IPL_Cmd.h"
#include "UIFlow.h"

UINT32 JPGIMG_SIZE[PHOTO_SIZE_ID_MAX][2]=
{
    {4032,3024}, //12M
    {3648,2736}, //10M
    {3264,2448}, // 8M
    {2592,1944}, // 5M
    {2048,1536}, // 3M
    {1920,1080}, // 2MHD
    {640,480}, // VGA
    {1280,960}, // 1.3M
};

UINT32 JPGIMG2_SIZE[PHOTO_SIZE_ID_MAX][2]=
{
    {1280, 720}, //1280x720
    {1280, 720}, //1280x720
    {1280, 720}, //1280x720
    {1280, 720}, //1280x720
    {1280, 720}, //1280x720
    {1280, 720}, //1280x720
    {1280, 720}, //1280x720
    {1280, 720}, //1280x720
};

static const CHAR *g_pcSizeCfgTable[PHOTO_SIZE_ID_MAX] =
{
    "12M",
    "10M",
    "8M",
    "5M",
    "3M",
    "2MHD",
    "VGA",
    "1.3M",
};

const CHAR *Get_SizeString(UINT32 uiIndex)
{
    if(uiIndex < PHOTO_SIZE_ID_MAX)
        return g_pcSizeCfgTable[uiIndex];
    else
        return NULL;
}

UINT32 GetPhotoSizeWidth(UINT32 ubIndex)
{
    return JPGIMG_SIZE[ubIndex][0];
}

UINT32 GetPhotoSizeHeight(UINT32 ubIndex)
{
    return JPGIMG_SIZE[ubIndex][1];
}

UINT32 GetPhotoSizeWidth2(UINT32 ubIndex)
{
    return JPGIMG2_SIZE[ubIndex][0];
}

UINT32 GetPhotoSizeHeight2(UINT32 ubIndex)
{
    return JPGIMG2_SIZE[ubIndex][1];
}

/*************************************************
    Dzoom string handling
**************************************************/
// DZoom string
#define MAX_DZOOM_STR_LEN               16
static CHAR g_cDZoomRatioStr[MAX_DZOOM_STR_LEN] = {0};

CHAR *Get_DZoomRatioString(void)
{
    UINT32 m, n;
    UINT32 DzoomIndx;

    DzoomIndx = DZOOM_IDX_GET();
    m = (DzoomIndx+10)/10;
    n = DzoomIndx%10;
    sprintf(g_cDZoomRatioStr, "x%ld.%ld", m, n);
    return g_cDZoomRatioStr;
}

///////////////////////////////////////////////////////////////////
UINT32 Get_MacroIndexFromValue(UINT32 value)
{
    switch(value)
    {
        case SEL_AFMODE_MACRO:
            return MACRO_ON;
        case SEL_AFMODE_AUTO:
            return MACRO_OFF;
        default:
            debug_msg(("^R Parameter error in Get_FocusIndexFromValue()\r\n"));
            return MACRO_OFF;
    }
}

UINT32 Get_MacroValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case MACRO_ON:
            return SEL_AFMODE_MACRO;
        case MACRO_OFF:
            return SEL_AFMODE_AUTO;
        default:
            debug_msg(("^R Parameter error in Get_MacroValue()\r\n"));
            return SEL_AFMODE_AUTO;
    }

}

UINT32 Get_FlashValue(UINT32 uiIndex)
{
    switch (uiIndex)
    {
        case FLASH_AUTO:
            return SEL_FLASHMODE_AUTO;
        case FLASH_RED_EYE:
            return SEL_FLASHMODE_AUTOREDEYE;
        case FLASH_OFF:
            return SEL_FLASHMODE_FORCEOFF;
        case FLASH_ON:
            return SEL_FLASHMODE_FORCEON;
        default:
            debug_msg(("^R Parameter error in Get_FlashValue()\r\n"));
            return 0;
    }
}

UINT32 Get_FlashIndexFromValue(UINT32 value)
{
    switch(value)
    {
        case SEL_FLASHMODE_FORCEON:
            return FLASH_ON;
        case SEL_FLASHMODE_AUTO:
            return FLASH_AUTO;
        case SEL_FLASHMODE_AUTOREDEYE:
            return FLASH_RED_EYE;
        case  SEL_FLASHMODE_FORCEOFF:
            return FLASH_OFF;
        default:
            debug_msg(("^R Parameter error in Get_FlashIndexFromValue()\r\n"));
            return FLASH_OFF;
    }
}

UINT32 Get_EVValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case EV_N20:
            return SEL_AEEV_N2;
        case EV_N16:
            return SEL_AEEV_N5_3;
        case EV_N13:
            return SEL_AEEV_N4_3;
        case EV_N10:
            return SEL_AEEV_N1;
        case EV_N06:
            return SEL_AEEV_N2_3;
        case EV_N03:
            return SEL_AEEV_N1_3;
        case EV_00:
            return SEL_AEEV_00;
        case EV_P03:
            return SEL_AEEV_P1_3;
        case EV_P06:
            return SEL_AEEV_P2_3;
        case EV_P10:
            return SEL_AEEV_P1;
        case EV_P13:
            return SEL_AEEV_P4_3;
        case EV_P16:
            return SEL_AEEV_P5_3;
        case EV_P20:
            return SEL_AEEV_P2;
        default:
            debug_msg(("^R Parameter error in Get_EVValue()\r\n"));
            return SEL_AEEV_00;
    }
}

UINT32 Get_EVIndexFromValue(UINT32 value)
{
    switch(value)
    {
        case SEL_AEEV_N2:
            return EV_N20;
        case SEL_AEEV_N5_3:
            return EV_N16;
        case SEL_AEEV_N4_3:
            return EV_N13;
        case SEL_AEEV_N1:
            return EV_N10;
        case SEL_AEEV_N2_3:
            return EV_N06;
        case SEL_AEEV_N1_3:
            return EV_N03;
        case SEL_AEEV_00:
            return EV_00;
        case SEL_AEEV_P1_3:
            return EV_P03;
        case SEL_AEEV_P2_3:
            return EV_P06;
        case SEL_AEEV_P1:
            return EV_P10;
        case SEL_AEEV_P4_3:
            return EV_P13;
        case SEL_AEEV_P5_3:
            return EV_P16;
        case SEL_AEEV_P2:
            return EV_P20;
        default:
            debug_msg(("^R Parameter error in Get_EVIndexFromValue()\r\n"));
            return EV_00;

    }
}

UINT32 Get_SizeValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
//#NT#2012/07/24#Bowen Li -begin
#if 0
        #if(_MAX_IMAGE_SIZE_ == _MAX_IMAGE_SIZE_14M_)
        case SIZE_14M:
            return SEL_IMGSIZE_14M;
        case SIZE_12M32:
            return SEL_IMGSIZE_12M;
        case SIZE_10M169:
            return SEL_IMGSIZE_10M;
        #elif(_MAX_IMAGE_SIZE_ == _MAX_IMAGE_SIZE_12M_)
        case SIZE_12M:
            return SEL_IMGSIZE_12M;
        case SIZE_10M32:
            return SEL_IMGSIZE_10M;
        case SIZE_9M169:
            return SEL_IMGSIZE_9M;
        case SIZE_10M:
            return SEL_IMGSIZE_10M;
        #else
        case SIZE_10M:
            return SEL_IMGSIZE_10M;
        case SIZE_9M32:
            return SEL_IMGSIZE_9M;
        case SIZE_7M169:
            return SEL_IMGSIZE_7M;
        #endif
        case SIZE_8M:
            return SEL_IMGSIZE_8M;
        case SIZE_5M:
            return SEL_IMGSIZE_4_9M;
        case SIZE_3M:
            return SEL_IMGSIZE_3M;
        case SIZE_VGA:
            return SEL_IMGSIZE_0_3M;
#endif
//#NT#2012/07/24#Bowen Li -end
        default:
            debug_msg(("^R Parameter error in Get_SizeValue()\r\n"));
            return 0;
    }
}

UINT32 Get_QualityValue(UINT32 uhIndex)
{
#if 0
    switch(uhIndex)
    {
        case QUALITY_BQ:
            return 95;
        case QUALITY_FQ:
            return 87;
        case QUALITY_NQ:
            return 80;
        default:
            debug_msg(("^R Parameter error in Get_QualityValue()\r\n"));
            return 80;
    }
#endif

    switch(uhIndex)
    {
        case QUALITY_SUPER:
            return 95;
        case QUALITY_FINE:
            return 87;
        case QUALITY_NORMAL:
            return 80;
        default:
            debug_msg(("^R Parameter error in Get_QualityValue()\r\n"));
            return 80;
    }

}


UINT32 Get_WBValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case WB_AUTO:
            return SEL_WBMODE_AUTO;
        case WB_DAYLIGHT:
            return SEL_WBMODE_DAYLIGHT;
        case WB_CLOUDY:
            return SEL_WBMODE_CLOUDY;
     case WB_TUNGSTEN:
        return SEL_WBMODE_TUNGSTEN;
        case WB_FLUORESCENT:
            return SEL_WBMODE_FLUORESCENT1;
        default:
            debug_msg(("^R Parameter error in Get_WBValue()\r\n"));
            return SEL_WBMODE_AUTO;
    }
}

UINT32 Get_WBIndexFromValue(UINT32 value)
{
    switch(value)
    {
        case SEL_WBMODE_AUTO:
            return WB_AUTO;
        case SEL_WBMODE_DAYLIGHT:
            return WB_DAYLIGHT;
        case SEL_WBMODE_CLOUDY:
            return WB_CLOUDY;
        case SEL_WBMODE_FLUORESCENT1:
            return WB_FLUORESCENT;
        default:
            debug_msg(("^R Parameter error in Get_WBIndexFromValue()\r\n"));
            return WB_AUTO;
    }
}

UINT32 Get_ColorValue(UINT32 uhIndex)
{
#if 0
    switch(uhIndex)
    {
        case COLOR_STANDARD:
            return SEL_IMAGEEFFECT_OFF;
        case COLOR_VIVID:
            return SEL_IMAGEEFFECT_VIVID;
        case COLOR_SEPIA:
            return SEL_IMAGEEFFECT_SEPIA;
        case COLOR_BW:
            return SEL_IMAGEEFFECT_BW;
        default:
            debug_msg(("^R Parameter error in Get_ColorValue()\r\n"));
            return SEL_IMAGEEFFECT_OFF;
    }
#endif

    switch(uhIndex)
    {
        case COLOR_EFFECT_STANDARD:
            return SEL_IMAGEEFFECT_OFF;
        case COLOR_EFFECT_MONOCHROME:
            return SEL_IMAGEEFFECT_BW;
        case COLOR_EFFECT_SEPIA:
            return SEL_IMAGEEFFECT_SEPIA;
        default:
            debug_msg(("^R Parameter error in Get_ColorValue()\r\n"));
            return SEL_IMAGEEFFECT_OFF;
    }
}

UINT32 Get_ISOValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case ISO_AUTO:
            return SEL_ISOINDEX_AUTO;
        case ISO_100:
            return SEL_ISOINDEX_100;
        case ISO_200:
            return SEL_ISOINDEX_200;
        case ISO_400:
            return SEL_ISOINDEX_400;
        default:
            debug_msg(("^R Parameter error in Get_ISOValue()\r\n"));
            return SEL_ISOINDEX_AUTO;
    }
}


UINT32 Get_SceneModeValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
#if _DEMO_TODO
        case SCENE_SPORTS:
            return _SceneMode_Sport;
        case SCENE_INDOOR:
            return _SceneMode_Indoor;
        case SCENE_SNOW:
            return _SceneMode_Snow;
        case SCENE_FIREWORKS:
            return _SceneMode_Firework;
        case SCENE_MUSEUM:
            return _SceneMode_Museum;
        case SCENE_NIGHTLANDSCAPE:
            return _SceneMode_NightLandscape;
        case SCENE_CHILDREN:
            return _SceneMode_Children;
        case SCENE_LEAF:
            return _SceneMode_Leaf;
        case SCENE_SUNSET:
            return _SceneMode_SunSet;
        case SCENE_GLASS:
            return _SceneMode_Glass;
        case SCENE_LANDSCAPE:
            return _SceneMode_Landscape;
        case SCENE_NIGHTPOTRAIT:
            return _SceneMode_NightPotrait;
        case SCENE_PORTRAIT:
            return _SceneMode_Potrait;
        case SCENE_AUTO:
            return _SceneMode_Auto;
        case SCENE_BACKPORTRAIT:
            return _SceneMode_BackLight;
        case SCENE_FLOWER:
            return _SceneMode_Marco;
        case SCENE_TEXT:
            return _SceneMode_Text;
        case SCENE_FISHEYE:
            //return _SceneMode_FishEye;
            return _SceneMode_Auto;
        case SCENE_ID:
            return _SceneMode_ID;
        case SCENE_PHOTOFRAME:
            return _SceneMode_PhotoFrame;
        case SCENE_PANNINGSHOT:
            return _SceneMode_PanningShot;
        case SCENE_BEACH:
            return _SceneMode_Beach;
        case SCENE_PARTY:
            return _SceneMode_Party;
        case SCENE_SKETCH:
#if(SCENE_SKETCH2HDR==ENABLE)
            return _SceneMode_Auto;
#else
            return _SceneMode_Sketch;
#endif
        default:
            debug_msg(("^R Parameter error in Get_SceneModeValue()\r\n"));
            return _SceneMode_Auto;
#else
        default:
            debug_msg(("^R Parameter error in Get_SceneModeValue()\r\n"));
            return 0;
#endif
    }
}
UINT32 Get_SceneModeIndexFromValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
#if _DEMO_TODO
    case _SceneMode_Landscape:
        return SCENE_LANDSCAPE;
    case _SceneMode_Potrait:
        return SCENE_PORTRAIT;
    case _SceneMode_NightPotrait:
        return SCENE_NIGHTPOTRAIT;
    case _SceneMode_BackLight:
        return SCENE_BACKPORTRAIT;
    case _SceneMode_NightLandscape:
        return SCENE_NIGHTLANDSCAPE;;
    case _SceneMode_Marco:
        return SCENE_FLOWER;
    case _SceneMode_Auto:
        return SCENE_AUTO;
#endif
    default:
        debug_msg(("^R Parameter error in Get_SceneModeIndex()\r\n"));
        return SCENE_AUTO;
    }
}

UINT32 Get_MeteringValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case METERING_SPOT:
            return SEL_AEMODE_SPOT;
        case METERING_CENTER:
            return SEL_AEMODE_CENTERWEIGHTED;
        case METERING_AIAE:
            return SEL_AEMODE_EVALUATIVE;
        default:
            return SEL_AEMODE_EVALUATIVE;

    }
}

UINT32 Get_BShutterValue(UINT32 uhIndex)
{
    switch (uhIndex)
    {
#if _DEMO_TODO
        case BSHUTTER_OFF:
            return _Expos_Manual_Off;
        case 2:
            return _Expos_2S;
        case 3:
            return _Expos_3S;
        case 4:
            return _Expos_4S;
        case 5:
            return _Expos_5S;
        case 6:
            return _Expos_6S;
        case 7:
            return _Expos_7S;
        case 8:
            return _Expos_8S;
        case 9:
            return _Expos_9S;
        case 10:
            return _Expos_10S;
        case 11:
            return _Expos_11S;
        case 12:
            return _Expos_12S;
        case 13:
            return _Expos_13S;
        case 14:
            return _Expos_14S;
        case 15:
            return _Expos_15S;
        case 16:
            return _Expos_16S;
        case 17:
            return _Expos_17S;
        case 18:
            return _Expos_18S;
        case 19:
            return _Expos_19S;
        case 20:
            return _Expos_20S;
        case 21:
            return _Expos_21S;
        case 22:
            return _Expos_22S;
        case 23:
            return _Expos_23S;
        case 24:
            return _Expos_24S;
        case 25:
            return _Expos_25S;
        case 26:
            return _Expos_26S;
        case 27:
            return _Expos_27S;
        case 28:
            return _Expos_28S;
        case 29:
            return _Expos_29S;
        case 30:
            return _Expos_30S;
        default:
            return _Expos_Manual_Off;
#else
        default:
            debug_msg(("^R Parameter error in Get_BShutterValue()\r\n"));
            return 0;
#endif
    }
}
//#NT#2010/01/13#Lincy -begin
UINT32 Get_AFWindowIndexFromValue(UINT32 value)
{
    switch(value)
    {
        case SEL_AFWINDOW_CENTER:
            return AFWINDOW_SINGLE;
        case SEL_AFWINDOW_AUTO:
            return AFWINDOW_MULTI;
        default:
            debug_msg(("^R Parameter error in Get_AFWindowIndexFromValue()\r\n"));
            return AFWINDOW_SINGLE;
    }
}
UINT32 Get_AFWindowValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case AFWINDOW_SINGLE:
            return SEL_AFWINDOW_CENTER;
        case AFWINDOW_MULTI:
            return SEL_AFWINDOW_AUTO;
        default:
            debug_msg(("^R Parameter error in Get_AFWindowValue()\r\n"));
            return AFWINDOW_SINGLE;
    }
}
//#NT#2010/01/13#Lincy -end

//#NT#2010/02/02#Lincy Lin -begin
UINT32 Get_BlinkValue(UINT32 value)
{
    switch(value)
    {
#if _DEMO_TODO
        case BLINKDET_OFF:
            return _BlinkDetectionMode_Off;
        case BLINKDET_ON:
            return _BlinkDetectionMode_On;
        default:
            debug_msg(("^R Parameter error in Get_BlinkValue()\r\n"));
            return _BlinkDetectionMode_Off;
#else
        default:
            debug_msg(("^R Parameter error in Get_BlinkValue()\r\n"));
            return 0;
#endif
    }
}
//#NT#2010/02/02#Lincy Lin -end

UINT32 Get_DSCmodeFromValue(UINT32 value)
{
    switch(value)
    {
        //#NT#2010/03/04#Lincy Lin -begin
        #if (MODEKEY_FUNCTION == ENABLE)
        case DSC_MODE_PHOTO_AUTO:
            return SEL_DSCMODE_MANUAL;
        #else
        case DSC_MODE_PHOTO_AUTO:
            return SEL_DSCMODE_AUTO;
        #endif
        //#NT#2010/03/04#Lincy Lin -end
        case DSC_MODE_PHOTO_MANUAL:
            return SEL_DSCMODE_MANUAL;
#if _DEMO_TODO
        case DSC_MODE_PHOTO_ASCN:
            return _DSC_AutoScene;
        case DSC_MODE_PHOTO_PANORAMA:
            return _DSC_Panorama;
        case DSC_MODE_PHOTO_SCENE:
            return _DSC_Scene;
        case DSC_MODE_PHOTO_STAB:
            return _DSC_Stabilization;
        case DSC_MODE_MOVIE:
            return _DSC_Movie;
        case DSC_MODE_PHOTO_PORTRAIT:
            return _DSC_Portrait;
        case DSC_MODE_PLAYBACK:
            return _DSC_Playback;
        default:
            return _DSC_Auto;
#else
        default:
            debug_msg(("^R Parameter error in Get_DscmodeFromValue()\r\n"));
            return 0;
#endif
    }
}

UINT32 Get_DateImprintScaleValue(UINT32 value)
{
    return 6;
}

UINT32 Get_ImageRatioValue(UINT32 value)
{
    switch (value)
    {
        case IMAGERATIO_9_16:
             return IPL_SENCROPRATIO_9_16;
        case IMAGERATIO_3_4:
             return IPL_SENCROPRATIO_3_4;
        case IMAGERATIO_1_1:
             return IPL_SENCROPRATIO_1_1;
        case IMAGERATIO_4_3:
             return IPL_SENCROPRATIO_4_3;
        case IMAGERATIO_3_2:
             return IPL_SENCROPRATIO_3_2;
        case IMAGERATIO_16_9:
             return IPL_SENCROPRATIO_16_9;
        default:
            debug_msg(("^R Parameter error in Get_ImageRatioValue()\r\n"));
            return 0;
    }
}

UINT32 Get_FDImageRatioValue(UINT32 value)
{
    switch (value)
    {
        case IMAGERATIO_4_3:
             return FD_IMAGERATIO_4_3;
        case IMAGERATIO_3_2:
             return FD_IMAGERATIO_3_2;
        case IMAGERATIO_16_9:
             return FD_IMAGERATIO_16_9;
        default:
            debug_msg(("^R Parameter error in Get_FDImageRatioValue()\r\n"));
            return FD_IMAGERATIO_4_3;
    }
}

//#NT#2012/8/16#Philex - begin
// add mapping functions for photo menu
UINT32 Get_DatePrintValue(UINT32 value)
{
    switch (value)
    {
        case DATE_STAMP_OFF:
             return SEL_DATASTAMP_OFF;
        case DATE_STAMP_DATE:
        case DATE_STAMP_DATE_TIME:
             return SEL_DATASTAMP_ON;
        default:
            debug_msg(("^R Parameter error in Get_DatePrintValue()\r\n"));
            return SEL_DATASTAMP_OFF;
    }
}

UINT32 Get_DscModeValue(UINT32 value)
{
    // only return auto DSCMode for CarDV
    return SEL_DSCMODE_AUTO;
}

UINT32 Get_SharpnessValue(UINT32 value)
{
    switch (value)
    {
        case SHARPNESS_SHARP:
             return SEL_SHARPNESS_P1;
        case SHARPNESS_NORMAL:
             return SEL_SHARPNESS_NORMAL;
        case SHARPNESS_SOFT:
             return SEL_SHARPNESS_N1;

        default:
            debug_msg(("^R Parameter error in Get_SharpnessValue()\r\n"));
            return SEL_SHARPNESS_NORMAL;
    }
}

UINT32 Get_SaturationValue(UINT32 value)
{
    switch (value)
    {
        case SATURATION_HIGH:
             return SEL_SATURATION_P1;
        case SATURATION_NORMAL:
             return SEL_SATURATION_NORMAL;
        case SATURATION_LOW:
             return SEL_SATURATION_N1;
        default:
            debug_msg(("^R Parameter error in Get_SaturationValue()\r\n"));
            return SEL_SATURATION_NORMAL;
    }
}
//#NT#2012/8/16#Philex - end

UINT32 Get_ImageEffectValue(UINT32 value)
{

    switch (value)
    {
        case IMAGEEFFECT_SKETCH:
             return SEL_IMAGEEFFECT_SKETCH;

        case IMAGEEFFECT_FISHEYE:
             return SEL_IMAGEEFFECT_FISHEYE;

        case IMAGEEFFECT_COLORPENCIL:
             return SEL_IMAGEEFFECT_COLORPENCIL;

        case IMAGEEFFECT_WDR:
             return SEL_IMAGEEFFECT_WDR;

        case IMAGEEFFECT_ANTISHAKE:
             return SEL_IMAGEEFFECT_OFF;

        case IMAGEEFFECT_HDR:
             return SEL_IMAGEEFFECT_OFF;

        default:
             return SEL_IMAGEEFFECT_OFF;
    }
}

