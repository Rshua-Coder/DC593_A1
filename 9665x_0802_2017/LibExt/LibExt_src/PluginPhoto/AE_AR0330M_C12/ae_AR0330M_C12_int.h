/**
    Auto Exposure.


    @file       ae_sample_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AE_AR0330M_C12_INT_H_
#define _AE_AR0330M_C12_INT_H_

#include "Sensor.h"

/**
     @name AE default value
     @note
*/
//@{
#define GainExpTShiftNum            0                           ///< Gain and ExpT Setting Timing VD shift number
#define CA_WIN_X                    32// 8                      ///< ae window x
#define CA_WIN_Y                    32// 8                      ///< ae window y
#define AE_WIN_X                    8                           ///< ae window x
#define AE_WIN_Y                    8                           ///< ae window y
#define AE_MAX_EV                   ((1 << 18) * 10)            ///< ae max EV
#define AE_MIN_EV                   ((1 << 0) * 10)             ///< ae min EV
#define AE_INIT_EV                  ((1 << 8) * 10)             ///< ae init EV
#define AE_COVER_RANGE_H            3                           ///< cover range H
#define AE_COVER_RANGE_L            3                           ///< cover range L
#define AE_COVER_RANGE_RF_H        100                           ///< cover range H rougth
#define AE_COVER_RANGE_RF_L        100                           ///< cover range L rougth
//#define AE_MOV_EXPECT_Y             640                         ///< expectY 100 //for video recording
//#define AE_MOV_EXPECT_Y            542// 512                         ///< expectY 100 //for video recording
#define AE_MOV_EXPECT_Y             496                         ///< expectY 100 //for video recording
#define AE_PHOTO_EXPECT_Y           496//  512                         ///< expectY 128 //for photo mode
#define AE_EXPECT_Y2                560//763//905//(911)             ///< histogram expectY2 150
#define AE_EXPECT_Y4                880//2604//(2572)                ///< (Luminance Y) 220

#define AE_MAX_ISO                  1800//3200 //3200 //1600                        ///< max iso
#define AE_MIN_ISO                  150//50                          ///< min iso

#define AE_EV_RATIO                 (100)
#define AE_HIST_NUM                 128                         ///< historgram bin number


#define AE_FNO_RATIO                (19 * 19 / 10)

#define AE_60FREQ_NODE              ((1 << 1) | (1 << 2) | (1 << 4))
#define AE_50FREQ_NODE              ((1 << 1) | (1 << 2) | (1 << 3))

#define AE_MAX_EXPT_33333           33333 //us
#define AE_MAX_EXPT_30000           30000 //us
#define AE_MAX_EXPT_16666           16666 //us
#define AE_MAX_EXPT_10000           10000 //us

#define AE_MIN_EXPT_30              76 //us for 30 fps & 15 fps



#define AEAFD_PROC_YLUM_MAX         (60<<4)
#define AEAFD_PROC_YLUM_MIN         (30<<4)

//liminance threshold
#define AEBlk_OVER_RAW_UP_THR       220<<2
#define AEBlk_OVER_RAW_LOW_THR      200<<2
#define AEBlk_UNDER_RAW_UP_THR      40<<2
#define AEBlk_UNDER_RAW_LOW_THR     24<<2
//@}


#define IFE_GAIN_ON         DISABLE
#define IFE_GAIN_MAXISO     800

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

extern UINT32 ExptY_Mov_Prv[AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_Photo_Prv[AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 Histo_Prv[AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_Mov_OverExpoAdj[AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_Photo_OverExpoAdj[AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_Cap[AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_OverExpoThreshold[AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_OverExpoMaxCnt[AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_OverExpoMinCnt[AEALG_DYNAMIC_EXPECT_LUM_MAX];

extern UINT8 AE_Win_Adaptive[AE_WIN_Y][AE_WIN_X];
extern const UINT8 AE_Win_CenterWeighted[AE_WIN_Y][AE_WIN_X];
extern const UINT8 AE_Win_Spot[AE_WIN_Y][AE_WIN_X];
extern const UINT8 AE_Win_Multi[AE_WIN_Y][AE_WIN_X];
extern const UINT8 AE_Win_Evaluative[AE_WIN_Y][AE_WIN_X];

extern const UINT8 AE_OverExpo_Win_Normal[AE_WIN_Y][AE_WIN_X];

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

     @param[in/out] AEINFO
     @return
         - @b TRUE: current luminance.
*/
extern UINT32 AE_getStatisticInfo(AE_INFO2 *AeInfo);

/**

     ae initial informaiton

     @note

     @param[out] ae information
*/
extern void AE_getInfo(AE_INFO2 *AeInfo);

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
extern void AE_getCapTab(AE_ISO nISO, AE_FLICKER nFlicker, AE_SCENE nScene, AE_INFO2 *AeInfo);

/**

     ae get preview curve and information
     @note

     @param[in] iso parameter
     @param[in] flicker parameter
     @param[out] ae information
*/
extern AE_RESET AE_getPrvTab(AE_ISO nISO, AE_FLICKER nFlicker, AE_INFO2 *AeInfo);

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
