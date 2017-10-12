/**
    Auto Exposure.


    @file       ae_sample_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AE_SAMPLE_INT_H_
#define _AE_SAMPLE_INT_H_

#include "Sensor.h"

/**
     @name AE default value
     @note
*/
//@{
#define AE_RATE                     3                           ///< ae rate
#define CA_WIN_X                    32// 8                      ///< ae window x
#define CA_WIN_Y                    32// 8                      ///< ae window y
#define AE_WIN_X                    8                           ///< ae window x
#define AE_WIN_Y                    8                           ///< ae window y
#define AE_MAX_EV                   ((1 << 18) * 10)            ///< ae max EV
#define AE_MIN_EV                   ((1 << 3) * 10)             ///< ae min EV
#define AE_INIT_EV                  ((1 << 8) * 10)             ///< ae init EV
#define AE_REDUCE_OVER_EXPO_MODE    ENABLE                      ///< reduce over exposure mode switch
#define AE_COVER_RANGE_H            5                           ///< cover range H
#define AE_COVER_RANGE_L            5                           ///< cover range L
#define AE_ADJUST_RATIO_H           400//200                         ///< adjust ratio H
#define AE_ADJUST_RATIO_L           25//50                          ///< adjust ratio L
#define AE_SMOOTH_PRV_W             2                           ///< weighting
#define AE_SMOOTH_NEW_W             5                           ///< weighting
#define AE_EXPECT_Y                 (40<<4)                     ///< expectY
#define AE_MAX_ISO                  1600                         ///< max iso
#define AE_MIN_ISO                  50                          ///< min iso

#define AE_ADJ_Y_STEP               (1<<4)                           ///< adjust Y step
#define AE_ADJ_OUTDOOR_EV           ((1 << 11) * 10)            ///< outdoor boundary
#define AE_ADJ_INDOOR_EV            ((1 << 10) * 10)            ///< indoor boundary
#define AE_ADJ_OUTDOOR_MAX_Y        (6<<4)                          ///< adjust outdoor max y
#define AE_ADJ_INDOOR_MAX_Y         (10<<4)                          ///< adjust indoor max y
#define AE_ADJ_OVER_EXPO_TH         (9 * 5)                     ///< adjust over exposure threshold
#define AE_ADJ_UNDER_EXPO_TH        (9 * 5 / 3)                 ///< adjust under exposure threshold
#define AE_EV_RATIO                 (100)
#define AE_HIST_NUM                 64                          ///< historgram bin number

#define AEAFD_PROC_YLUM_MAX         (40<<4)
#define AEAFD_PROC_YLUM_MIN         (20<<4)
#define AEAFD_CAStartX              0
#define AEAFD_CAStartY              0
#define AEAFD_CAScalingFactor       49
#define AEAFD_YLowerLimit           10
#define AEAFD_YUpperLimit           245
#define AEAFD_FOMiThreshold         60
#define AEAFD_FOMsThreshold         80
#define AEAFD_DetectUpperBound      64   ///< ratio to adjust the threshold, 128 is 100%
#define AEAFD_DetectLowerBound      26  ///< ratio to adjust the threshold, 128 is 100%
//@}


/**
     @name AE flag
     @note
*/
//@{
#define AE_FLAG_SET_EXPT    (1 << 0)
#define AE_FLAG_SET_GAIN    (1 << 1)
#define AE_FLAG_SET_IRIS    (1 << 2)
//@}


/**
     AE flicker type.

     @note
*/
typedef enum _AE_FLICKER
{
    AE_FLICKER_50HZ = 0, ///< flicker 50Mz
    AE_FLICKER_60HZ,     ///< flicker 60Mz
    AE_FLICKER_MAX,
    ENUM_DUMMY4WORD(AE_FLICKER)
}AE_FLICKER;

/**
     AE ISO type.

     @note
*/
typedef enum _AE_ISO
{
    AE_ISO_AUTO = 0,   ///< ISO_AUTO
    AE_ISO_25,         ///< ISO_25
    AE_ISO_50,         ///< ISO_50
    AE_ISO_80,         ///< ISO_80
    AE_ISO_100,        ///< ISO_100
    AE_ISO_200,        ///< ISO_200
    AE_ISO_400,        ///< ISO_400
    AE_ISO_800,        ///< ISO_800
    AE_ISO_1600,       ///< ISO_1600
    AE_ISO_3200,       ///< ISO_3200
    AE_ISO_6400,       ///< ISO_6400
    AE_ISO_MAX,
    ENUM_DUMMY4WORD(AE_ISO)
}AE_ISO;

/**
     AE win type.

     @note
*/
typedef enum _AE_WIN
{
    AE_WIN_CENTERWEIGHT = 0,    ///< ae center weighting
    AE_WIN_SPOT,                ///< ae spot weighting
    AE_WIN_MARTIX,              ///< ae matrix weighting
    AE_WIN_EVALUATIVE,          ///< ae evaluative weighting
    AE_WIN_MAX,
    ENUM_DUMMY4WORD(AE_WIN)
}AE_WIN;

/**
     AE scene type.

     @note
*/
typedef enum _AE_SCENE
{
    AE_SCENE_AUTO = 0,          ///< auto
    AE_SCENE_LANDSCAPE,         ///< landscape
    AE_SCENE_POTRAIT,           ///< potrait
    AE_SCENE_SPORT,
    AE_SCENE_NIGHTSCENE,
    AE_SCENE_AV,
    AE_SCENE_TV,
    AE_SCENE_MANUAL,
    AE_SCENE_NIGHTPOTRAIT,
    AE_SCENE_NIGHTLANDSCAPE,
    AE_SCENE_PARTY,
    AE_SCENE_SUNSET,
    AE_SCENE_BACKLIGHT,
    AE_SCENE_DOCUMENT,
    AE_SCENE_FIREWORK,
    AE_SCENE_BEACH,
    AE_SCENE_BMODE,
    AE_SCENE_PROGRAM,
    AE_SCENE_HIGHISO,
    AE_SCENE_MAX,
    ENUM_DUMMY4WORD(AE_SCENE)
}AE_SCENE;


/**
     AE anti-shake level.

     @note
*/
typedef enum _AE_ANTISHAKE
{
    AE_ANTISHAKE_LV0 = 0,           ///< disable
    AE_ANTISHAKE_LV1,
    AE_ANTISHAKE_LV2,
    AE_ANTISHAKE_MAX,
    ENUM_DUMMY4WORD(AE_ANTISHAKE)
}AE_ANTISHAKE;


/**
     AE Reset Flag.

     @note
*/
typedef enum _AE_RESET
{
    AE_RESET_FALSE = 0, ///<
    AE_RESET_TRUE,     ///< set ae reset
    AE_RESET_MAX,
    ENUM_DUMMY4WORD(AE_RESET)
}AE_RESET;

extern UINT8 AE_Win_Adaptive[AE_WIN_Y][AE_WIN_X];
extern const UINT8 AE_Win_CenterWeighted[AE_WIN_Y][AE_WIN_X];
extern const UINT8 AE_Win_Spot[AE_WIN_Y][AE_WIN_X];
extern const UINT8 AE_Win_Multi[AE_WIN_Y][AE_WIN_X];
extern const UINT8 AE_Win_Evaluative[AE_WIN_Y][AE_WIN_X];

extern const UINT32 AeCurve_AntiFlicker60[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_AntiFlicker50[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO50_60Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO50_50Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO80_60Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO80_50Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO100_60Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO100_50Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO200_60Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO200_50Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO400_60Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO400_50Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO800_60Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO800_50Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO1600_60Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO1600_50Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_Landscape[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_NightLandscape[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_Action[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_Potrait[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_NightPotrait[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_Cap50[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_Cap60[][AEALG_INFO_MAX];
/**

     judge preview current and previous setting

     @note

     @param[in] new setting
     @param[in] old setting
     @param[out] update flag
     @return
         - @b TRUE: update setting.
*/
extern UINT32 AE_judgePrvSetting(AEALG_ARG *CurInfo, AEALG_ARG *PrvInfo, UINT32 *Flag);

/**

     get statisic information

     @note

     @param[in] histogram size
     @param[out] histogram
     @return
         - @b TRUE: current luminance.
*/
extern UINT32 AE_getStatisticInfo(UINT32 HistSize, UINT32 *Hist);

/**

     ae initial informaiton

     @note

     @param[out] ae information
*/
extern void AE_getInfo(AE_INFO *AeInfo);

/**

     set ae window

     @note

     @param[out] ae information
*/
extern AE_RESET AE_setWin(AE_WIN mode);

/**

     get ae flag

     @note
     @return
         - @b ae flag.
*/
extern UINT32 AE_getFlag(void);

/**

     set ae flag

     @note

     @param[in] ae flag
*/
extern void AE_setFlag(UINT32 flag);

/**

     clear ae flag

     @note

     @param[in] ae flag
*/
extern void AE_clrFlag(UINT32 flag);

/**

     copy ae parameter
     @note

     @param[in] source ae parameter
     @param[out] dest ae parameter
*/
extern void AE_copyArg(AEALG_ARG *src, AEALG_ARG *dest);

/**

     ae reset flag
     @note
*/
extern void AE_reset (void);

/**

     ae get capture curve and information
     @note

     @param[in] iso parameter
     @param[in] flicker parameter
     @param[in] scene parameter
     @param[out] ae information
*/
extern void AE_getCapTab(AE_ISO nISO, AE_FLICKER nFlicker, AE_SCENE nScene, AE_INFO *AeInfo);

/**

     ae get preview curve and information
     @note

     @param[in] iso parameter
     @param[in] flicker parameter
     @param[out] ae information
*/
extern AE_RESET AE_getPrvTab(AE_ISO nISO, AE_FLICKER nFlicker, AE_INFO *AeInfo);

/**

     get over exposure ratio
     @note

     @param[in] histogram size
     @param[out] histogram
     @return
         - @b over exposure cnt.
*/
extern UINT32 GetOverExpoRatio(UINT32 HistMaxNum, UINT32 *Hist);

/**

     get fno ratio
     @note

     @param[in] iris index
     @return
         - @b fno ratio.
*/
extern UINT32 GetFnoRatio(AEALG_IRIS_IDX Idx);

/**

     scene map function
     @note

     @param[in] Scene index
     @return
         - @b Scene type.
*/
extern AE_SCENE AE_getSceneType(UINT32 Idx);

/**
     flicker map function
     @note

     @param[in] flicker index
     @return
         - @b flicker type.
*/
extern AE_FLICKER AE_getFlickerType(UINT32 Idx);

/**
     anti-shake map function
     @note

     @param[in] anti-shake index
     @return
         - @b anti-shake type.
*/
extern AE_ANTISHAKE AE_getAntiShakeLv(UINT32 Idx);

/**
     iso map function
     @note

     @param[in] iso index
     @return
         - @b iso type.
*/
extern AE_ISO AE_getIsoType(UINT32 Idx);

/**
     window map function
     @note

     @param[in] window index
     @return
         - @b window type.
*/
extern AE_WIN AE_getWinType(UINT32 Idx);

/**
     get preview sensor bining mode (binning ratio * 100)
     @note

     @return
         - @b bining mode
*/
extern UINT32 AE_getPrvBiningMode(void);

/**
     get capture sensor bining mode (binning ratio * 100)
     @note

     @return
         - @b bining mode
*/
extern UINT32 AE_getCapBiningMode(void);

/**
     ae set exposure time
     @note
     @param[in] exposure time
*/
extern void AE_setExpT(EXPOSURE_SETTING *ExpTSetting);

/**
     ae set iso gain
     @note
     @param[in] iso gain
*/
extern void AE_setGain(GAIN_SETTING *Gain);

/**
     ae set iris
     @note
     @param[in] iris index
*/
extern void AE_setIris(AEALG_IRIS_IDX Idx);

/**
     ae wait preview stable
     @note

     @param[in] StableCnt : wait stable count
     @param[in] TimeOutFrame : time out frame
     @return
         - @b TRUE: ae already stable
         - @b FALSE: ae not stable and time out
*/
extern BOOL AE_Wait_Stable(UINT32 StableCnt, UINT32 TimeOutFrame);

extern ER AE_setFrame(UINT32 Frame, UINT32 *VTotal, UINT32 *HTotal);

#endif //_AE_SAMPLE_INT_H_
