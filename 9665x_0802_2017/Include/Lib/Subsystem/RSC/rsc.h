/**
    Rolling Shutter Correction.

    @file       rsc.h
    @ingroup    mILibRSC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _RSC_H
#define _RSC_H

#include "Type.h"
#include "ime_lib.h"

/**
    @addtogroup
*/
//@{

typedef enum
{
    RSC_FUNC_RSC_EIS=0,     ///< RSC on,  EIS on
    RSC_FUNC_RSC_ONLY,      ///< RSC on,  EIS off
    RSC_FUNC_EIS_ONLY,      ///< RSC off, EIS on
    RSC_FUNC_NULL,          ///< RSC off, EIS off
    RSC_FUNC_MAX_CNT_DEF,
    ENUM_DUMMY4WORD(RSC_FUNC_SELECTION)
} RSC_FUNC_SELECTION;

typedef enum
{
    RSC_GYRO_DATA_INT16,    ///< as signed 16 bits integer
    RSC_GYRO_DATA_IS2000A,  ///< InvenSense 2000A
    RSC_GYRO_DATA_ISIDG2020,///< InvenSense IDG2020
    ENUM_DUMMY4WORD(RSC_GYRO_DATA_FORMAT)
} RSC_GYRO_DATA_FORMAT;

typedef enum
{
    RSC_GYRO_DIR_UP_RIGHT,      ///< X+ = up    , Y+ = right
    RSC_GYRO_DIR_UP_LEFT,       ///< X+ = up    , Y+ = left
    RSC_GYRO_DIR_DOWN_RIGHT,    ///< X+ = down  , Y+ = right
    RSC_GYRO_DIR_DOWN_LEFT,     ///< X+ = down  , Y+ = left
    RSC_GYRO_DIR_RIGHT_UP,      ///< X+ = right , Y+ = up
    RSC_GYRO_DIR_RIGHT_DOWN,    ///< X+ = right , Y+ = down
    RSC_GYRO_DIR_LEFT_UP,       ///< X+ = left  , Y+ = up
    RSC_GYRO_DIR_LEFT_DOWN,     ///< X+ = left  , Y+ = down
    RSC_GYRO_DIR_MAX_CNT_DEF,
    ENUM_DUMMY4WORD(RSC_GYRO_DIRECTION)
} RSC_GYRO_DIRECTION;


typedef enum
{
    RSC_BEHAVIOR_NORMAL,    ///< Apply normal RSC
    RSC_BEHAVIOR_ABORT,     ///< Abort RSC(for current frame)
    ENUM_DUMMY4WORD(RSC_BEHAVIOR_SELECTION)
} RSC_BEHAVIOR_SELECTION;

typedef enum
{
    RSC_INFO_VAL_GD_CNT,            ///< value - current gyro-data count ( the index of next coming gyro-data )
    RSC_INFO_VAL_GD_CNT_CUR_FRM,    ///< value - current gyro-data count in current frame
    RSC_INFO_VAL_FAM_CNT,           ///< value - current frame count

    RSC_INFO_SET_CONFIG,            ///< setting - RSC_CONFIG_SETTING
    RSC_INFO_SET_GYRO,              ///< setting - RSC_GYRO_SETTING
    RSC_INFO_SET_LENS,              ///< setting - RSC_LENS_SETTING
    RSC_INFO_SET_SENSOR_DIMENSION,  ///< setting - RSC_SENSOR_DIMENSION_SETTING
    RSC_INFO_SET_SENSOR_TIMING,     ///< setting - RSC_SENSOR_TIMING_SETTING
    RSC_INFO_SET_SENSOR_EXPOSIRE,   ///< setting - RSC_SENSOR_EXPOSURE_SETTING
    RSC_INFO_SET_IPP_SIE,           ///< setting - RSC_IPP_SIE_SETTING
    RSC_INFO_SET_IPP_DCE,           ///< setting - RSC_IPP_DCE_SETTING
    RSC_INFO_SET_IPP_IME,           ///< setting - RSC_IPP_IME_SETTING
    RSC_INFO_SET_IPP_STP,           ///< setting - RSC_IPP_STP_SETTING

    RSC_INFO_PAR_INTPL,             ///< parameter - interpolation
    RSC_INFO_PAR_CP,                ///< parameter - control point
    RSC_INFO_PAR_IMG_STB_MT,        ///< parameter - EIS motion
    RSC_INFO_PAR_IME_RSC,           ///< parameter - IME RSC PARAMETERS
    ENUM_DUMMY4WORD(RSC_INFO_SELECTION)
} RSC_INFO_SELECTION;


typedef enum
{
    RSC_ISSTS_STILL,                ///< Camera is still
    RSC_ISSTS_TREMBLE,              ///< Camera is under small vibration
    RSC_ISSTS_SHAKE,                ///< Camera is under large vibration
    RSC_ISSTS_PANNING,              ///< Camera is under panning
    RSC_ISSTS_UNKNOWN,              ///< Camera is under unknown behavior
    ENUM_DUMMY4WORD(RSC_IS_STATUS)
} RSC_IS_STATUS;


typedef struct
{
    UINT32 uiMarginRatioH;          ///< the horizontal margin-ratio in %(ratio=(total-crop-out)/total)
    UINT32 uiMarginRatioV;          ///< the vertical   margin-ratio in %(ratio=(total-crop-out)/total)
    UINT32 uiStability;             ///< the magnitude of image-stability, range=0~100; higher value, more stable; 50 for default
    UINT32 uiGdPerFrm;              ///< the number of gyro-data in a frame; It must be 8
    RSC_FUNC_SELECTION funcSelect;  ///< the selection of RSC-Lib function
} RSC_CONFIG_SETTING;

typedef struct
{
    RSC_GYRO_DATA_FORMAT gdFormat;  ///< Gyro data format
    RSC_GYRO_DIRECTION   GyroDir;   ///< Gyro direction
    UINT32 uiDataUnit;              ///< Gyro data unit, in degree/65536sec
    UINT32 uiCalGain;               ///< calibration gain, in 1/1024
    INT32  iCalOfsX;                ///< X-calibration-offset/zero-rate-output(for subtraction), in gyro data unit
    INT32  iCalOfsY;                ///< Y-calibration-offset/zero-rate-output(for subtraction), in gyro data unit
} RSC_GYRO_SETTING;

typedef struct
{
    UINT32 uiFocalLength;   ///< Focal length for current lens position, in um; for wide optical-zoom
} RSC_LENS_SETTING;

typedef struct
{
    UINT32 uiSnrDmH;        ///< The horizontal size of sensor-active-pixel-region, in um
    UINT32 uiSnrDmV;        ///< The vertical size of sensor-active-pixel-region, in um
} RSC_SENSOR_DIMENSION_SETTING;

typedef struct
{
    UINT32 uiFrmTimeUs; ///< Frame time, in us
    UINT32 uiStartLnUs; ///< the time 1st line of active-pixel-region image is transferd, in us
    UINT32 uiEndLnUs;   ///< the time last line of active-pixel-region image is transferd, in us
    UINT32 uiTransDlyUs;///< the time between the-end-of-exposure and transfering, in us; zero for unknown
    UINT32 uiSnrOutSzH; ///< The Sensor output horizontal size, active-pixel-region
    UINT32 uiSnrOutSzV; ///< The Sensor output vertical size, active-pixel-region
} RSC_SENSOR_TIMING_SETTING;

typedef struct
{
    UINT32 uiExpTimeUs; ///< The exposure time, in us
} RSC_SENSOR_EXPOSURE_SETTING;

typedef struct
{
    UINT32 uiSieCrpSzH; ///< The SIE crop horizontal size
    UINT32 uiSieCrpSzV; ///< The SIE crop vertical size
    UINT32 uiSieOutSzH; ///< The SIE output horizontal size; after scaling if any
    UINT32 uiSieOutSzV; ///< The SIE output vertical size; after scaling if any
} RSC_IPP_SIE_SETTING;

typedef struct
{
    UINT32  CenterX;        ///< Distortion center X
    UINT32  CenterY;        ///< Distortion center Y
    UINT32  FovGain;        ///< Field of view setting; 1024 as 1x
    UINT32  LUT_TblAddr;    ///< table addr(tab num:65, Data type:UINT32)
    UINT32  DistFactX;      ///< X input distance factor for oval shape modeling
    UINT32  DistFactY;      ///< Y input distance factor for oval shape modeling
    UINT32  RadiousFactor;  ///< normalize factor
    UINT32  RadiousBit;     ///< normalize bit
} RSC_IPP_DCE_SETTING;

typedef struct
{
    IME_SIZE_INFO szIn;     ///< IME input size
    IME_SIZE_INFO szP1Scl;  ///< IME path-1 scaling size, (0,0) for channel-off
    IME_SIZE_INFO szP2Scl;  ///< IME path-2 scaling size, (0,0) for channel-off
    IME_SIZE_INFO szP1Out;  ///< IME path-1 output size, (0,0) for channel-off
    IME_SIZE_INFO szP2Out;  ///< IME path-2 output size, (0,0) for channel-off
} RSC_IPP_IME_SETTING;

typedef struct
{
    UINT32 uiMinStpSzH;     ///< IME input stripe minimal width
} RSC_IPP_STP_SETTING;


typedef struct
{
    RSC_BEHAVIOR_SELECTION   bhSelect;  ///< Abort current frame
} RSC_ACTION_SETTING;


typedef struct
{
    UINT32 uiStPos;     ///< IME RSC control point interpolation starting position
    UINT32 uiIntplFtP1; ///< IME RSC control point interpolation scaling-factor for path 1
    UINT32 uiIntplFtP2; ///< IME RSC control point interpolation scaling-factor for path 2
    UINT32 uiCpHGainP1; ///< IME RSC control point horizontal gain for path 1
    UINT32 uiCpHGainP2; ///< IME RSC control point horizontal gain for path 2
} RSC_INTERPOLATE_PARAM;


typedef struct
{
    UINT16 puiGD[8][2]; ///< gyro-data
    UINT32 uiNum;       ///< gyro-data number, max=8
} RSC_GYRO_DATA;


typedef struct
{
    INT16 piMotionTotal[2];     ///< Image stabilization motion, total motion
    INT16 piMotionExternal[2];  ///< Image stabilization motion, external motion, which should be compensated outside IME
    INT16 piMotionInternal[2];  ///< Image stabilization motion, external motion, which would be compensated inside IME by RSC
} RSC_IMG_STB_MT_PARAM;

typedef struct
{
    UINT16 puiCP[12][2];    ///< control points
} RSC_CONTROLPOINT_PARAM;


typedef struct
{
    UINT32 uiFrmCnt;    ///< frame count, since the 1st one after power-on
    UINT32 uiGdCnt;     ///< gyro data count, in the current frame
} RSC_PROCESS_STATUS;


typedef struct _RSC_OPENOBJ{
    RSC_CONFIG_SETTING              ConfigSet;  ///< configuration
    RSC_GYRO_SETTING                GyroSet;    ///< gyro information
    RSC_LENS_SETTING                LensSet;    ///< lens information
    RSC_SENSOR_DIMENSION_SETTING    SnrDmSet;   ///< sensor dimension information
    RSC_SENSOR_TIMING_SETTING       SnrTmSet;   ///< sensor timing information
    RSC_SENSOR_EXPOSURE_SETTING     SnrExpSet;  ///< sensor exposure-time information
    RSC_IPP_SIE_SETTING             IppSieSet;  ///< IPP-SIE information
    RSC_IPP_DCE_SETTING             IppDceSet;  ///< IPP-DCE information
    RSC_IPP_IME_SETTING             IppImeSet;  ///< IPP-IME information
    RSC_IPP_STP_SETTING             IppStpSet;  ///< IPP-Stripe information
} RSC_OPENOBJ;


extern ER rsc_open(RSC_OPENOBJ *popenObj);
extern ER rsc_close(void);
extern ER rsc_chgConfigSet(RSC_CONFIG_SETTING *pConfigSet);
extern ER rsc_chgGyroSet(RSC_GYRO_SETTING *pGyroSet);
extern ER rsc_chgLensSet(RSC_LENS_SETTING *pLensSet);
extern ER rsc_chgSensorDimensionSet(RSC_SENSOR_DIMENSION_SETTING *pSnrDmSet);
extern ER rsc_chgSensorTimingSet(RSC_SENSOR_TIMING_SETTING *pSnrTmSet);
extern ER rsc_chgSensorExposureSet(RSC_SENSOR_EXPOSURE_SETTING *pSnrExpSet);
extern ER rsc_chgIppSieSet(RSC_IPP_SIE_SETTING *pIppSieSet);
extern ER rsc_chgIppDceSet(RSC_IPP_DCE_SETTING *pIppDceSet);
extern ER rsc_chgIppImeSet(RSC_IPP_IME_SETTING *pIppImeSet);
extern ER rsc_chgIppStpSet(RSC_IPP_STP_SETTING *pIppStpSet);
//mode-switch function
extern ER rsc_chgAllSet(RSC_OPENOBJ *popenObj);
//~mode-switch function

//removed//extern ER rsc_setOperation(void);
extern ER rsc_setGyroData(RSC_GYRO_DATA *pGyroData);
extern ER rsc_setAction(RSC_ACTION_SETTING *pActionSet);
extern ER rsc_setTestData(UINT32 uiIdx, INT32 iData);
extern ER rsc_getTestData(UINT32 uiIdx, INT32 *piData);

extern UINT32 rsc_getInfo(RSC_INFO_SELECTION InfoSelect, void *pObj);

//@}



#endif//_RSC_H
