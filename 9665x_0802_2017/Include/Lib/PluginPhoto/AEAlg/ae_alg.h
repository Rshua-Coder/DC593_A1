/**
    Auto Exposure Algorithm.

    depend on current Luminance to calculate LV, ISO, Exposure Time, and Iris.

    @file       Ae_Alg.h
    @ingroup    mILibAEAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AE_ALG_H_
#define _AE_ALG_H_


/**
     @name AE Alg. default value
     @note
*/
//@{
#define AEALG_TAB_END   0xffffffff      ///< AE table end flag

#define AEALG_BUF_SIZE  8

#define AEALG_PREDICT_POINT_MAX     3

#define AEALG_DYNAMIC_EXPECT_LUM_MAX  21

#define AE_BLK_WIN_NUM                  6           //divide image to 6 block

#define AE_GEN_CURVE_NODE_MAX           40
//@}

/**
     AEALG error code.

     @note
*/
typedef enum _AEALG_ER
{
    AEALG_OK = 0,         ///< AE OK
    AEALG_PAR,            ///< AE Parameter error
    AEALG_MAX,
    ENUM_DUMMY4WORD(AEALG_ER)
}AEALG_ER;

/**
     Iris index.

     big iris -> small iris
     AEALG_IRIS_0 -> AEALG_IRIS_1 -> ....... -> AEALG_IRIS_8
     @note
*/
typedef enum _AEALG_IRIS_IDX
{
    AEALG_IRIS_0 = 0, ///< iris index 0
    AEALG_IRIS_1,     ///< iris index 1
    AEALG_IRIS_2,     ///< iris index 2
    AEALG_IRIS_3,     ///< iris index 3
    AEALG_IRIS_4,     ///< iris index 4
    AEALG_IRIS_5,     ///< iris index 5
    AEALG_IRIS_6,     ///< iris index 6
    AEALG_IRIS_7,     ///< iris index 7
    AEALG_IRIS_8,     ///< iris index 8
    AEALG_IRIS_MAX,
    ENUM_DUMMY4WORD(AEALG_IRIS_IDX)
}AEALG_IRIS_IDX;


/**
     AE table information.

     AE table information.
     @note
*/
typedef enum _AEALG_TABLE_INFO
{
    AEALG_EV = 0,          ///< EV value
    AEALG_EXP,             ///< Exposure time
    AEALG_ISO,             ///< ISO value
    AEALG_IRIS,            ///< IRIS value
    AEALG_ADJ_TARGET,      ///< adjust flag
    AEALG_INFO_MAX,
    ENUM_DUMMY4WORD(AEALG_TABLE_INFO)
} AEALG_TABLE_INFO;

/**
     @name Ae adjust flag(AEALG_ADJ_TARGET)
     @note
*/
//@{
#define AEALG_ADJ_EXP           0x01    ///< Adjust exposure time
#define AEALG_ADJ_GAIN          0x02    ///< Adjust iso
#define AEALG_ADJ_IRIS          0x04    ///< Adjust iris idx
//@}

/**
     AEALG converge speed.

     @note
*/
typedef enum _AEALG_SPEED
{
    AEALG_SPEED_N3 = 0,         ///< speed -3
    AEALG_SPEED_N2,             ///< speed -2
    AEALG_SPEED_N1,             ///< speed -1
    AEALG_SPEED_NORMAL,         ///< normal
    AEALG_SPEED_P1,             ///< speed +1
    AEALG_SPEED_P2,             ///< speed +2
    AEALG_SPEED_P3,             ///< speed +3
    AEALG_SPEED_MAX,            ///< max number
    ENUM_DUMMY4WORD(AEALG_SPEED)
}AEALG_SPEED;

/**
     AE argument.

     AE argument.
     @note
*/
typedef struct _AEALG_ARG
{
    UINT32 EVValue;         ///< evvalue
    UINT32 ExpoTime;        ///< Exposure Time(us)
    UINT32 ISOGain;         ///< ISO value(50, 100, 200......)
    UINT32 Iris;            ///< Iris Value(idx)
}AEALG_ARG;


/**
     AE over exposure information.

     AE over exposure information.
     @note
*/
typedef struct _AEALG_REDUCE_OVER_EXPO_INFO
{
    UINT32 OutdoorEV;               ///< outdoor EV
    UINT32 IndoorEV;                ///< indoor EV
    UINT32 OutdoorMaxAdjY;          ///< outdoor max adj Y
    UINT32 IndoorMaxAdjY;           ///< indoor max adj Y
    UINT32 OverExpoTh;              ///< over exopsure threshold
    UINT32 UnderExpoTh;             ///< under exopsure threshold
    UINT32 AdjYStep;                ///< adjust Y step
    UINT32 AdjY;
    UINT32 AdjExpextY;
    UINT32 Data[AEALG_BUF_SIZE];
} AEALG_REDUCE_OVER_EXPO_INFO;


/**
     AE over exposure information.

     AE over exposure information.
     @note
*/
typedef struct _AEALG_REDUCE_OVER_EXPO_INFO2
{
    UINT32 EVAdjEn;                 ///< ev +0 or not
    UINT32 OutdoorEV;               ///< outdoor EV
    UINT32 IndoorEV;                ///< indoor EV
    UINT32 OutdoorMaxAdjY;          ///< outdoor max adj Y
    UINT32 IndoorMaxAdjY;           ///< indoor max adj Y
    UINT32 OverExpoTh;              ///< over exopsure threshold
    UINT32 UnderExpoTh;             ///< under exopsure threshold
    UINT32 AdjYStep;                ///< adjust Y step
    UINT32 AdjY;
    UINT32 AdjExpextY;
    UINT32 Data[AEALG_BUF_SIZE];
} AEALG_REDUCE_OVER_EXPO_INFO2;

/**
     AE luminance
     @note
*/
typedef struct _AE_LUMINANCE
{
    UINT32 raw;     //raw luminance
    UINT32 y;       //yuv luminance
} AE_LUMINANCE;

/**
     AE boundary
     @note
*/
typedef struct _AE_BOUNDARY
{
    UINT32 h;       //high
    UINT32 l;       //low
} AE_BOUNDARY;

/**
     AE histogram
     @note
*/
typedef struct _AE_HISTOGRAM
{
    UINT32 bin_num; //total bin number
    UINT32 data_num; //total data number;
    UINT32 *raw;    //raw histogram pointer
    UINT32 *y;      //yuv histogram pointer
} AE_HISTOGRAM;

/**
     AE weight item
     @note
*/
typedef struct _AE_WEIGHT
{
    UINT32 ratio_bound;  //ratio boundary
    UINT32 exp_w;     //expect Y ratio
} AE_WEIGHT;

/**
     AE converage table
     @note
*/
typedef struct _AE_CONV_TAB
{
    UINT32 thres_bound; // current average Y > thres_bound, using raw else yuv
    UINT32 weight_tab_num;
    AE_WEIGHT *weight_tab;
} AE_CONV_TAB;


typedef struct _AE_ENV_VALUE
{
    UINT32 LV;         // lv
    UINT32 EVValue;    // ev value
} AE_ENV_VALUE;

/**
     AE algorithm event.
     @note
*/

typedef UINT32 (*GET_OVER_EXPO_RATIO)(UINT32 HistMaxNum, UINT32 *Hist);
typedef UINT32 (*GET_FNO_RATIO)(AEALG_IRIS_IDX Idx);
typedef UINT32 (*GET_CURVE_OFS_RATIO)(void);
typedef struct _AEALG_EVENT_TAB
{
    //AEAlg_getratio event
    GET_OVER_EXPO_RATIO GetOverExpoRatio;

    //AEAlg_Allot event(Fno x Fno x 10)
    GET_FNO_RATIO GetFnoRatio;

    //AEAlg_Allot event((FnoA^2 / FnoB^2) x 100)
    //FnoA = (Wide, big iris)
    //FnoB = (Current Section, big iris)
    GET_CURVE_OFS_RATIO GetCurveOfsRatio;
}AEALG_EVENT_TAB;

/**
     AE Information.

     AE Information.
     @note
*/
typedef struct _AE_INFO
{
    UINT32 ReduceOverExpoModeEn;                ///< reduce over exposure mode switch
    UINT32 EVAdjEn;                             ///< ev +0 or not
    UINT32 LumY;                                ///< current luminance
    UINT32 MinEV;                               ///< min ev
    UINT32 MaxEV;                               ///< max ev
    UINT32 MinISOValue;                         ///< min iso
    UINT32 MaxISOValue;                         ///< max iso
    UINT32 CovRangeH;                           ///< cover range tolerance (100 + H) %
    UINT32 CovRangeL;                           ///< cover range tolerance (100 + L) %
    UINT32 HistBinNum;                          ///< histogram bin number
    UINT32 *Hist;                               ///< histogram buffer pointer
    UINT32 Reset;                               ///< Reset Flag
    UINT32(* PrvCurve)[ AEALG_INFO_MAX ];       ///< preview Ae curve
    UINT32(* CapCurve)[ AEALG_INFO_MAX ];       ///< capture Ae curve
    UINT32 PrvBiningMode;                       ///< bining mode 1x, 2x, 3x, 4x......(x 100)
    UINT32 CapBiningMode;                       ///< bining mode 1x, 2x, 3x, 4x......(x 100)
    UINT32 ExpectY;                             ///< expected luminance
    UINT32 AdjRatioH;                           ///< adjust ratio boundary
    UINT32 AdjRatioL;                           ///< adjust ratio boundary
    UINT32 EVPrvWeight;                         ///< previous weight
    UINT32 EVNewWeight;                         ///< last weight
    AEALG_REDUCE_OVER_EXPO_INFO OverExpoInfo;   ///< reduce over exposure information
    AEALG_EVENT_TAB EventTab;
    UINT32 Counter;         ///< ae stable counter
    UINT32 PrvEVValue;      ///< preview current ev value
    UINT32 PrvLV;           ///< preview current LV
    UINT32 CapEVValue;      ///< capture current ev value
    UINT32 CapLV;           ///< capture current LV
} AE_INFO;

/**
     AE sub-settings
     @note
*/

typedef struct _AE_OVEREXP_INFO
{
    AE_LUMINANCE AdjLum;                 ///< adjust luminace
    UINT32 CurCnt;
    UINT32 CurTotalStep;                 ///< current adjust total step
    UINT32 CurMaxStep;                   ///< current adjust Max step
    UINT32 CurMaxCnt;
    UINT32 CurMinCnt;
    UINT32 CurThreshold;
    UINT32 *AdjLumThresholdTab;          ///< over exposure luminance threshold table (LV0 ~ LV20, max index 21), AdjLum * x%
    UINT32 *AdjLumTab;                   ///< over exposure adjust luminance table (LV0 ~ LV20, max index 21, current expected Lum - adj lum)
    UINT32 *AdjMaxCntRatioTab;           ///< over exposure luminance counter table (LV0 ~ LV20, max index 21) (x / 1000)
    UINT32 *AdjMinCntRatioTab;           ///< over exposure luminance counter table (LV0 ~ LV20, max index 21) (x / 1000)
    UINT32 UserDefRatio;                 ///< if TRUE, CurCnt = GET_OVER_EXPO_RATIO()(user can set any value pass through callback function)
} AE_OVEREXP_INFO;

/**
     AE sub-settings
     @note
*/

typedef struct _AE_ADJHISTO_INFO
{
    AE_LUMINANCE AdjLum;                 ///< (adjust luminance)
    AE_LUMINANCE Lum;
    UINT32 *RatioTab;
    UINT32 AvgL;
    UINT32 AvgLCnt;
    UINT32 AvgH;
    UINT32 AvgHCnt;
} AE_ADJHISTO_INFO;


typedef UINT32 (*AEALG_ADD_GAMMA)(UINT32);
typedef UINT32 (*AEALG_INV_GAMMA)(UINT32);
typedef struct _AE_EXT_SET
{
    UINT32 *PrvLumRatioTab;                             ///< dynamic expected luminance table (LV0 ~ LV20, max index 21, expectLum x ratio / 100)
    AE_LUMINANCE DyLum;

    AE_OVEREXP_INFO OverExp;                            ///< sub function for reducing over exposure condition

    UINT32 AdjHistoEn;                                  ///< adjust histogram switch
    AE_ADJHISTO_INFO AdjHistoInfo;                      ///< adjust histogram range parameter

    UINT32 AdjCapEn;
    AE_LUMINANCE CapExpectLum;                          ///< expected luminance
    UINT32 *CapExpectTab;
} AE_EXT_SET;

/**
     AE Information.

     AE Information.
     @note
*/
typedef struct _AE_INFO2
{
    AE_LUMINANCE PrvExpectLum;                      ///< standard expected luminance
    AE_LUMINANCE PrvAvgLum;                         ///< standard average luminance
    AE_ENV_VALUE PrvAvgEV;                          ///< standard ev value for average luminance
    AE_BOUNDARY EV;                                 ///< max, min ev
    AE_BOUNDARY ISO;                                ///< max, min iso
    AE_BOUNDARY StableRange;                        ///< stable tolerance (100 +H -L) %
    AE_BOUNDARY StableRangeRF;                      ///< stable tolerance (100 +H -L) %
    AE_HISTOGRAM histogram;                         ///< histogram information
    UINT32(* PrvCurve)[AEALG_INFO_MAX];             ///< preview Ae curve
    UINT32(* CapCurve)[AEALG_INFO_MAX];             ///< capture Ae curve
    UINT32 FirstRunFlag;                            ///< ae first run flag
    UINT32 Reset;                                   ///< ae reset flag
    UINT32 Counter;                                 ///< ae stable counter
    UINT32 CounterRF;                               ///< ae stable counter
    AE_LUMINANCE CurExpectLum;                      ///< current expected luminance
    AE_LUMINANCE CurLum;                            ///< current luminance
    UINT32 CurConvDir;                              ///< preview converage direction
    AE_ENV_VALUE PrvParam[AEALG_PREDICT_POINT_MAX]; ///< preview parameter
    AE_ENV_VALUE CapParam;                          ///< capture parameter
    AEALG_EVENT_TAB EventTab;
    AEALG_SPEED ConvSpeed;                          ///< converage speed
    AE_CONV_TAB *ConvTab;                           ///< manual converge table
    AEALG_INV_GAMMA InvGammaFunc;                   ///< inverse gamma function
    AEALG_ADD_GAMMA AddGammaFunc;                   ///< additive gamma function
    UINT32 IsRawLum;
    UINT32 EVCompRatio;                             ///< ev comp ratio (raw * ratio /100)
    AE_EXT_SET ExtSet;                              ///< extend parameter for AE control
    UINT32 Rev[4];                                  ///< for debug do not access
} AE_INFO2;
/**
     AE Block Y information

     @note
*/
typedef struct _AE_BLK_INFO
{
    UINT32 BLKLum_En;                   ///Block Luminance calculate function switch
    UINT32 LV;                          ///current LV value
    UINT32 *Yacc;                       ///Y acc
    UINT32 *Gacc;                       ///G acc
    UINT32 CA_Num_X;                    ///CA window X number
    UINT32 CA_Num_Y;                    ///CA window Y number
    AE_BOUNDARY Raw_OverBound_Lum;      ///raw over  exposure luminance boundary (up/low bound)
    AE_BOUNDARY Raw_UnderBound_Lum;     ///raw under exposure luminance boundary (up/low bound)
    UINT32 PrvBlk_Num[AE_BLK_WIN_NUM];  ///previous luminance
    AEALG_ADD_GAMMA AddGammaFunc;       ///< additive gamma function
} AE_BLK_INFO;

/**

     AEAlg_allot function

     depend on EV value to calculate iso, exposure time, and iris.

     @note

     @param[in] EVValue    Current EVValue.
     @param[in] AECurve    AE Curve.
     @param[in] GET_FNO_RATIO callback function fp.
     @param[in] GET_CURVE_OFS_RATIO callback function fp.
     @param[out] Info    iso, exposure time, and iris.
     @return
         - @b AEALG_OK:   done with no error.
         - Others: Error occured.
*/
extern AEALG_ER AEAlg_Allot(UINT32 EVValue, UINT32 (*AeCurve)[AEALG_INFO_MAX], GET_FNO_RATIO GetFnoCB, GET_CURVE_OFS_RATIO GetCurveOfsCB, AEALG_ARG *Info);

/**

     converage luminance

     @note

     @param[in] AE_INFO(AEInfo)
     @param[out] preview iso, exposure time, iris
     @param[out] capture iso, exposure time, iris
     @return
         - @b void.
*/
extern void AEAlg_Converge(AE_INFO *AEInfo, AEALG_ARG *PrvInfo, AEALG_ARG *CapInfo, AEALG_ARG *PrvInfoTemp1, AEALG_ARG *PrvInfoTemp2);


/**

     converage luminance

     @note

     @param[in] AE_INFO(AEInfo)
     @param[in] UINT32
     @param[out] preview iso, exposure time, iris
     @param[out] capture iso, exposure time, iris
     @return
         - @b void.
*/
extern void AEAlg2_Converge(AE_INFO2 *AEInfo, UINT32 PointNum, AEALG_ARG *PrvInfo, AEALG_ARG *CapInfo);

/**

     convert ev value to lv value

     @note

     @param[in] ev vaule
     @return
         - @b lv value(x10).
*/
extern UINT32 AEAlg_Conv2LV(UINT32 EVValue);

/**

     calculate block based luminance

     @note

     @param[in] AE_BLK_INFO(AeBlkInfo)
     @param[in] Y and Raw CA, CA window number, LV
     @return
         - @b raw and y luminance
*/
extern AE_LUMINANCE AEAlg_GetBLKBasedLum(AE_BLK_INFO *AeBlkInfo);
extern UINT32 AE_BlkWriteDebugInfo(UINT32 Addr);
/**

     mdsc cb for debug use

     @note

     @return
         - @b bool.
*/
extern BOOL AEAlg2_MsdcCB(void);


typedef enum _AE_GEN_IDX
{
    AE_GEN_IDX_MIN = 0,
    AE_GEN_IDX_MAX,
    AE_GEN_IDX_NUM,
    ENUM_DUMMY4WORD(AE_GEN_IDX)
}AE_GEN_IDX;

typedef enum _AE_GEN_FREQ
{
    AE_GEN_FREQ_NONE = 0,
    AE_GEN_FREQ_27_5,
    AE_GEN_FREQ_50,
    AE_GEN_FREQ_60,
    AE_GEN_FREQ_NUM,
    ENUM_DUMMY4WORD(AE_GEN_FREQ)
}AE_GEN_FREQ;

typedef struct _AE_GEN_INFO
{
    UINT32 FnoRat;                          ///< fno * fno * 10
    UINT32 FnoIdx;                          ///< fno Idx
    UINT32 ExpT[AE_GEN_IDX_NUM];            ///< exp time(us), 0: min, 1:max
    UINT32 Iso[AE_GEN_IDX_NUM];             ///< iso, 0: min, 1:max
    AE_GEN_FREQ Freq;                       ///< anti flicker frequence 50 or 60
    UINT32 FreqNode;                        ///< anti flicker node 50Hz = x/100 => bit0 = x, bit1 = 1/100, bit2 = 2/100, bit3 = 3/100......
                                            ///<                   60Hz = x/120 => bit0 = x, bit1 = 1/120, bit2 = 2/120, bit3 = 3/120......
                                            ///< anti flicker node max 8 (1~8)
    UINT32 Node1stExpT;                     ///< exp time(us): ExpT[AE_GEN_IDX_MIN] <= Node1stExpT <= ExpT[AE_GEN_IDX_MAX]
    UINT32 Node2ndISO;                      ///< iso: Iso[AE_GEN_IDX_MIN] <= Node2ndISO <= Iso[AE_GEN_IDX_MAX]
} AE_GEN_INFO;

ER AEAlg_GenCurve(AE_GEN_INFO *Info, UINT32 *CurveAdr, UINT32 *MaxEV, UINT32 *MinEV);

#endif //_AE_ALG_H_
