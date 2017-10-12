#include "IPL_Cal_Setting_MN34110_EVB_FF_Int.h"
#include "IPL_Cal_DataPassing_MN34110_EVB_FF_Int.h"
#if 0
    - sensor cfg
#endif
/*
HD/VD inverse
Sony: FALSE
Pana: TRUE

*/
Cal_SENSOR_INFO SensorMode[2][SENSOR_ID_MAX_NUM] = {
    {{SENSOR_MODE_9, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
    {{SENSOR_MODE_2, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
};


Cal_SENSOR_INFO SensorMode_AWB[1][SENSOR_ID_MAX_NUM] = {
    {{SENSOR_MODE_9, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
};

Cal_SENSOR_INFO SensorMode_ECS[2][SENSOR_ID_MAX_NUM] = {
    {{SENSOR_MODE_9, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
    {{SENSOR_MODE_2, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
};

Cal_SENSOR_INFO SensorMode_DP[2][SENSOR_ID_MAX_NUM] = {
    {{SENSOR_MODE_9, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
    {{SENSOR_MODE_2, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
};

Cal_SENSOR_INFO SensorMode_ISO[1][SENSOR_ID_MAX_NUM] = {
    {{SENSOR_MODE_9, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
};

Cal_SENSOR_INFO SensorMode_AE_Check[1][SENSOR_ID_MAX_NUM] = {
    {{SENSOR_MODE_9, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
};


UINT32 Section_Size[SectionNum][2] = {
    {SEC_AWB,   0x100},
    {SEC_ECS,   (65*65*4*2+0x100)},// 4:word per window, 2 sensor mode
    {SEC_DP,    (DP_MAX_CNT*4*2+0x100)}, // 4:word per pixel, 2:sensor mode
    {SEC_MSHUT, 0x100},
    {SEC_ISO,   0x100},
    {SEC_AECHK, 0x100}
};

#if 0
    - ae cfg
#endif
CAL_AE_INFO AESetting_ISO_ADJ[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_ADJ_ISOBASE,    38,    4166,  50, 270,    5,  40},
    {TRUE,  AE_ADJ_ISOBASE,    38,    4166,  50, 270,    5,  40},
};

CAL_AE_INFO AESetting_AE_Check[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_ADJ_TIME,    40,     10000, 100, 270,    5,  20},
    {TRUE,  AE_ADJ_TIME,    40,     10000, 100, 270,    5,  20},
};

CAL_AE_INFO AESetting_AWB_ADJ[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_ADJ_TIME,     80,    2500,  100, 270,    20, 20},
    {TRUE,  AE_ADJ_TIME,     80,    2500,  100, 270,    20, 20},
};

CAL_AE_INFO AESetting_ECS_ADJ[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_ADJ_GAIN,    150,   5000,  100, 270,    20, 20},
    {TRUE,  AE_ADJ_GAIN,    150,   5000,  100, 270,    20, 20},
};

CAL_AE_INFO AESetting_MSHT_ADJ[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_ADJ_GAIN,    130,    2500,  100, 270,    20, 20},
    {TRUE,  AE_ADJ_GAIN,    130,    2500,  100, 270,    20, 20},
};

CAL_AE_INFO AESetting_DP_W_ADJ[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_ADJ_GAIN,    120,    10000,  200, 270,     20, 10},
    {TRUE,  AE_ADJ_GAIN,    120,    10000,  200, 270,     20, 10},
};

CAL_AE_INFO AESetting_DP_B_ADJ[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_ADJ_TIME,    1, 160000, 200, 270, 20, 10},
    {TRUE,  AE_ADJ_TIME,    1, 160000, 200, 270, 20, 10},
};

CAL_AE_INFO AESetting_Prv[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_ADJ_GAIN,     38, 2500, 100, 270, 20, 10},
    {TRUE,  AE_ADJ_GAIN,     38, 2500, 100, 270, 20, 10},
};
#if 0
    - awb cal
#endif
UINT32 CAL_AWB_BIT = d2f8;
#if 0
    - ecs cal
#endif
CAL_ECS_INFO Cal_ecs_info = {
    ECS_D2F8, ecs65x65
};
#if 0
    - dp cal
#endif
CAL_DPC_THRES DPC_THRES_BRIGHT[SENSOR_ID_MAX_NUM] = {
    {12,32,32},
    {12,32,32},
};
CAL_DPC_THRES DPC_THRES_DARK[SENSOR_ID_MAX_NUM] = {
    {12,32,32},
    {12,32,32},
};
#if 0
    - DCE cfg
#endif
DIST CAL_DCE_Dist =
{
    0xfff,0xfff,Bar_Pin
};

UINT32 CAL_DCE_2D_LUT[65] =
{
    65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535
};

DCE_Distortion Cal_DCE_Distortion = {
    {0},                                    //Center
    {0x400,{DISABLE,1024,1024,1024}},       //FOV
    {96, 6, 16, 6},                         //Enh
    {0x1000,0x1000,0x1000},                 //aber
    (UINT32)CAL_DCE_2D_LUT,                 //lut table address
    0,                                      //lut table type,don't care
    FALSE
};