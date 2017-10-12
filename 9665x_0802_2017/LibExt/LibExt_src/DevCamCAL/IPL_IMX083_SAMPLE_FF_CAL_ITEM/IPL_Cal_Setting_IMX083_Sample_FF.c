#include "IPL_Cal_Setting_IMX083_Sample_FF_Int.h"
#include "ipl_cal_datapassing_IMX083_Sample_FF_int.h"
#if 0
    - sensor cfg
#endif
/*
HD/VD inverse
Sony: FALSE
Pana: TRUE

*/
Cal_SENSOR_INFO SensorMode[4][SENSOR_ID_MAX_NUM] = {
    {{SENSOR_MODE_1, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
    {{SENSOR_MODE_2, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
    {{SENSOR_MODE_3, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
    {{SENSOR_MODE_4, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
};


Cal_SENSOR_INFO SensorMode_AWB[1][SENSOR_ID_MAX_NUM] = {
    {{SENSOR_MODE_1, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
};

Cal_SENSOR_INFO SensorMode_ECS[1][SENSOR_ID_MAX_NUM] = {
    {{SENSOR_MODE_1, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
    //{{SENSOR_MODE_2, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
};

Cal_SENSOR_INFO SensorMode_DP[1][SENSOR_ID_MAX_NUM] = {
    {{SENSOR_MODE_1, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
    //{{SENSOR_MODE_2, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
    //{{SENSOR_MODE_3, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
    //{{SENSOR_MODE_4, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
};

Cal_SENSOR_INFO SensorMode_ISO[1][SENSOR_ID_MAX_NUM] = {
    {{SENSOR_MODE_1, FALSE,FALSE,_RISING,_RISING,_RISING,0,0} , {BYPASS,0,0,0,0,0,0,0}},
};


UINT32 Section_Size[SectionNum][2] = {
    {SEC_AWB,   0x100},
    {SEC_ECS,   (65*65*4*2+0x100)},// 4:word per window, 2 sensor mode
    {SEC_DP,    (DP_MAX_CNT*4*4+0x100)}, // 4:word per pixel, 4:sensor mode
    {SEC_MSHUT, 0x100},
    {SEC_ISO,   0x100}
};

#if 0
    - ae cfg
#endif
CAL_AE_INFO AESetting_ISO_ADJ[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_ADJ_ISOBASE,  60,    33333,  100,  100,  5,  20},
    {TRUE,  AE_ADJ_ISOBASE,  60,    33333,  100,  100,  5,  20},
};

CAL_AE_INFO AESetting_AWB_ADJ[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_ADJ_TIME,    120,    33333,  100,  100,  20, 20},
    {TRUE,  AE_ADJ_TIME,    120,    33333,  100,  100,  20, 20},
};

CAL_AE_INFO AESetting_ECS_ADJ[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_ADJ_TIME,    130,    33333,  100,  100,  20, 20},
    {TRUE,  AE_ADJ_TIME,    130,    33333,  100,  100,  20, 20},
};

CAL_AE_INFO AESetting_MSHT_ADJ[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_ADJ_GAIN,    130,    33333,  100,  100,  20, 20},
    {TRUE,  AE_ADJ_GAIN,    130,    33333,  100,  100,  20, 20},
};

CAL_AE_INFO AESetting_DP_W_ADJ[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_ADJ_TIME,    120,    33333,  100,  100,  20, 20},
    {TRUE,  AE_ADJ_TIME,    120,    33333,  100,  100,  20, 20},
};

CAL_AE_INFO AESetting_DP_B_ADJ[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_FIX_ALL,    10,      33333,  100,  100,  20, 20},
    {TRUE,  AE_FIX_ALL,    10,      33333,  100,  100,  20, 20},
};

CAL_AE_INFO AESetting_Prv[SENSOR_ID_MAX_NUM] = {
    {TRUE,  AE_ADJ_TIME,    130,    33333,  100,  100,  20, 20},
    {TRUE,  AE_ADJ_TIME,    130,    33333,  100,  100,  20, 20},
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
    {8,32,32},
    {8,32,32},
};
CAL_DPC_THRES DPC_THRES_DARK[SENSOR_ID_MAX_NUM] = {
    {8,32,32},
    {8,32,32},
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