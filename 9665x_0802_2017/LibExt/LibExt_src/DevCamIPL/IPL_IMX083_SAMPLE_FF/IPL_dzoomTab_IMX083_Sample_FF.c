/**
    IPL dzoom table sample.

    @file       IPL_dzoomTableSample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "IPL_IMX083_Sample_FF_Int.h"

//for IMX083CQK mode 5
const UINT32 VDOZOOM_INFOR_MODE_1_TABLE[10][DZOOM_ITEM_MAX] =
{
    //sie         sie
    //crop out    ch0 out      pre in
    {1536,  384,  1536,  384,  1536,  384}, //0
    {1472,  368,  1472,  368,  1472,  368}, //1
    {1408,  352,  1408,  352,  1408,  352}, //2
    {1344,  336,  1344,  336,  1344,  336}, //3
    {1280,  320,  1280,  320,  1280,  320}, //4
    {1216,  304,  1216,  304,  1216,  304}, //5
    {1168,  292,  1168,  292,  1168,  292}, //6
    {1120,  280,  1120,  280,  1120,  280}, //7
    {1072,  268,  1072,  268,  1072,  268}, //8
    {1024,  256,  1024,  256,  1024,  256}, //9
};

//for IMX083CQK mode 3A
const UINT32 VDOZOOM_INFOR_MODE_2_TABLE[10][DZOOM_ITEM_MAX] =
{
    //sie        sie
    //crop out   ch0 out     pre in
    {1536,  864, 1536,  864, 1536,  864}, //0
    {1472,  824, 1472,  824, 1472,  824}, //1
    {1408,  788, 1408,  788, 1408,  788}, //2
    {1344,  752, 1344,  752, 1344,  752}, //3
    {1280,  720, 1280,  720, 1280,  720}, //4
    {1216,  688, 1216,  688, 1216,  688}, //5
    {1168,  656, 1168,  656, 1168,  656}, //6
    {1120,  628, 1120,  628, 1120,  628}, //7
    {1072,  600, 1072,  600, 1072,  600}, //8
    {1024,  572, 1024,  572, 1024,  572}, //9


};

//for IMX083CQK mode 2A
const UINT32 VDOZOOM_INFOR_MODE_3_TABLE[10][DZOOM_ITEM_MAX] =
{
    //sie        sie
    //crop out   ch0 out     pre in
    {2304, 1728, 2304, 1728, 2304, 1728}, //0
    {2208, 1652, 2208, 1652, 2208, 1652}, //1
    {2096, 1576, 2096, 1576, 2096, 1576}, //2
    {2016, 1508, 2016, 1508, 2016, 1508}, //3
    {1920, 1440, 1920, 1440, 1920, 1440}, //4
    {1840, 1376, 1840, 1376, 1840, 1376}, //5
    {1744, 1312, 1744, 1312, 1744, 1312}, //6
    {1680, 1256, 1680, 1256, 1680, 1256}, //7
    {1600, 1200, 1600, 1200, 1600, 1200}, //8
    {1520, 1144, 1520, 1144, 1520, 1144}, //9
};

//for IMX083CQK mode 2
const UINT32 VDOZOOM_INFOR_MODE_4_TABLE[10][DZOOM_ITEM_MAX] =
{
    //sie        sie
    //crop out   ch0 out     pre in
    {2304, 1296, 2304, 1296, 2304, 1296}, //0
    {2208, 1240, 2208, 1240, 2208, 1240}, //1
    {2096, 1184, 2096, 1184, 2096, 1184}, //2
    {2016, 1128, 2016, 1128, 2016, 1128}, //3
    {1920, 1080, 1920, 1080, 1920, 1080}, //4
    {1840, 1032, 1840, 1032, 1840, 1032}, //5
    {1744,  984, 1744,  984, 1744,  984}, //6
    {1680,  940, 1680,  940, 1680,  940}, //7
    {1600,  900, 1600,  900, 1600,  900}, //8
    {1520,  860, 1520,  860, 1520,  860}, //9
};

//for IMX083CQK mode 1
const UINT32 VDOZOOM_INFOR_MODE_5_TABLE[10][DZOOM_ITEM_MAX] =
{
    //sie        sie
    //crop out   ch0 out     pre in
    {4608, 3456, 4608, 3456, 4608, 3456}, //0
    {4400, 3300, 4400, 3300, 4400, 3300}, //1
    {4208, 3152, 4208, 3152, 4208, 3152}, //2
    {4016, 3012, 4016, 3012, 4016, 3012}, //3
    {3840, 2880, 3840, 2880, 3840, 2880}, //4
    {3664, 2748, 3664, 2748, 3664, 2748}, //5
    {3504, 2628, 3504, 2628, 3504, 2628}, //6
    {3344, 2508, 3344, 2508, 3344, 2508}, //7
    {3200, 2396, 3200, 2396, 3200, 2396}, //8
    {3056, 2288, 3056, 2288, 3056, 2288}, //9

};

//for IMX083CQK mode 0
const UINT32 VDOZOOM_INFOR_MODE_6_TABLE[10][DZOOM_ITEM_MAX] =
{
    //sie        sie
    //crop out   ch0 out     pre in
    {4608, 3456, 4608, 3456, 4608, 3456}, //0
    {4400, 3300, 4400, 3300, 4400, 3300}, //1
    {4208, 3152, 4208, 3152, 4208, 3152}, //2
    {4016, 3012, 4016, 3012, 4016, 3012}, //3
    {3840, 2880, 3840, 2880, 3840, 2880}, //4
    {3664, 2748, 3664, 2748, 3664, 2748}, //5
    {3504, 2628, 3504, 2628, 3504, 2628}, //6
    {3344, 2508, 3344, 2508, 3344, 2508}, //7
    {3200, 2396, 3200, 2396, 3200, 2396}, //8
    {3056, 2288, 3056, 2288, 3056, 2288}, //9
};

UINT32* SenMode2Tbl(UINT32 SenMode, UINT32 *DzMaxidx)
{
    UINT32 *Ptr;
    switch(SenMode)
    {
        case SENSOR_MODE_1:
            Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_1_TABLE[0][0];
            *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_1_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            break;

        case SENSOR_MODE_2:
            Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_2_TABLE[0][0];
            *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_2_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            break;

        case SENSOR_MODE_3:
            Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_3_TABLE[0][0];
            *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_3_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            break;

        case SENSOR_MODE_4:
            Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_4_TABLE[0][0];
            *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_4_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            break;

        case SENSOR_MODE_5:
            Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_5_TABLE[0][0];
            *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_5_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            break;

        case SENSOR_MODE_6:
            Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_6_TABLE[0][0];
            *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_6_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            break;

        default:
            Ptr = NULL;
            *DzMaxidx = 0;
            break;
    }
    return Ptr;
}
