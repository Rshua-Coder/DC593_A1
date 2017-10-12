#ifndef _IPL_CAL_SETTING_IMX083_SAMPLE_FF_INT_H_
#define _IPL_CAL_SETTING_IMX083_SAMPLE_FF_INT_H_
#include "IPL_Calibration_3a.h"
#include "IPL_Hal.h"

#define BYPASS  0xffffffff
#define CFGEND  (BYPASS-1)

typedef struct
{
    UINT32 SenMode;
    UINT32 HDinv;
    UINT32 VDinv;
    UINT32 VDPhase;
    UINT32 HDPhase;
    UINT32 DataPhase;
    UINT32 CropShrinkX; ///< offset crop size according sensor signal,must be multiple of 4, avoid bayer start pixel shifted
    UINT32 CropShrinkY; ///< offset crop size according sensor signal,must be multiple of 4, avoid bayer start pixel shifted
}Cal_SENSOR_INFO;

#define DP_MAX_CNT  (160000) // 1/100

#define CalSensorModeNum    (4)
extern Cal_SENSOR_INFO SensorMode[4][SENSOR_ID_MAX_NUM];
extern Cal_SENSOR_INFO SensorMode_AWB[1][SENSOR_ID_MAX_NUM];
extern Cal_SENSOR_INFO SensorMode_ECS[1][SENSOR_ID_MAX_NUM];
extern Cal_SENSOR_INFO SensorMode_DP[1][SENSOR_ID_MAX_NUM];
extern Cal_SENSOR_INFO SensorMode_ISO[1][SENSOR_ID_MAX_NUM];

#define SectionNum  5
extern UINT32 Section_Size[SectionNum][2];

extern CAL_AE_INFO AESetting_ISO_ADJ[SENSOR_ID_MAX_NUM];
extern CAL_AE_INFO AESetting_AWB_ADJ[SENSOR_ID_MAX_NUM];
extern CAL_AE_INFO AESetting_ECS_ADJ[SENSOR_ID_MAX_NUM];
extern CAL_AE_INFO AESetting_MSHT_ADJ[SENSOR_ID_MAX_NUM];
extern CAL_AE_INFO AESetting_DP_W_ADJ[SENSOR_ID_MAX_NUM];
extern CAL_AE_INFO AESetting_DP_B_ADJ[SENSOR_ID_MAX_NUM];
extern CAL_AE_INFO AESetting_Prv[SENSOR_ID_MAX_NUM];

typedef struct
{
    UINT32 Threshold;   ///< DP detect threshold
    UINT32 Block_X;     ///< DP search block X,(must be even,because of bayer fmt)
    UINT32 Block_Y;     ///< DP search block Y,(must be even,because of bayer fmt)
}CAL_DPC_THRES;

typedef struct
{
    ECS_BitRatio    Ecs_bit;
    ECSMAP_ENUM     Ecs_map;
}CAL_ECS_INFO;

extern UINT32 CAL_AWB_BIT;
extern CAL_ECS_INFO  Cal_ecs_info;
extern CAL_DPC_THRES DPC_THRES_BRIGHT[SENSOR_ID_MAX_NUM];
extern CAL_DPC_THRES DPC_THRES_DARK[SENSOR_ID_MAX_NUM];

extern DIST CAL_DCE_Dist;
extern UINT32 CAL_DCE_2D_LUT[LutTblSize];
extern DCE_Distortion Cal_DCE_Distortion;
#endif