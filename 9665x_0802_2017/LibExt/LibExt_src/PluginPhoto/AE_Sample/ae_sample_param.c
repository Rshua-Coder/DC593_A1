/**
    Auto Exposure parameter.

    ae parameter.

    @file       ae_sample_param.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "type.h"
#include "Ae_alg.h"
#include "Ae_sample_int.h"

UINT8 AE_Win_Adaptive[AE_WIN_Y][AE_WIN_X] =
{
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1}
};

const UINT8 AE_Win_CenterWeighted[AE_WIN_Y][AE_WIN_X] =
{
    {1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,1},
    {1,2,3,3,3,3,2,1},
    {1,2,3,4,4,3,2,1},
    {1,2,3,4,4,3,2,1},
    {1,2,3,3,3,3,2,1},
    {1,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,1}
};

const UINT8 AE_Win_Spot[AE_WIN_Y][AE_WIN_X] =
{
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

const UINT8 AE_Win_Multi[AE_WIN_Y][AE_WIN_X] =
{
    {3,3,2,1,1,2,3,3},
    {3,3,2,2,2,2,3,3},
    {2,2,6,6,6,6,2,2},
    {1,2,6,8,8,6,2,1},
    {1,2,6,8,8,6,2,1},
    {2,2,6,6,6,6,2,2},
    {3,3,2,2,2,2,3,3},
    {3,3,2,1,1,2,3,3}
};

const UINT8 AE_Win_Evaluative[AE_WIN_Y][AE_WIN_X] =
{
    {1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,1},
    {1,2,3,3,3,3,2,1},
    {1,2,3,4,4,3,2,1},
    {1,2,4,4,4,4,2,1},
    {1,2,3,4,4,3,2,1},
    {1,2,2,3,3,2,2,1},
    {1,1,1,1,1,1,1,1}
};

//Anti-flicker:60Hz
const UINT32 AeCurve_AntiFlicker60[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,        50,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {40960,     8333,        50,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {20480,     8333,       100,    AEALG_IRIS_2,          AEALG_ADJ_GAIN},
    {10240,     8333,       100,    AEALG_IRIS_1,          AEALG_ADJ_GAIN|AEALG_ADJ_IRIS},
    {5120,     16666,       100,    AEALG_IRIS_1,          AEALG_ADJ_GAIN},
    {2560,     16666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN|AEALG_ADJ_IRIS},
    {1280,     33333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {640,      33333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {320,      33333,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {160,      33333,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {80,       33333,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},                // 1/7.5*2
    {0,       533333,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}


};

//Anti-flicker:50Hz
const UINT32 AeCurve_AntiFlicker50[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,        50,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {34133,    10000,        50,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {17066,    10000,       100,    AEALG_IRIS_2,          AEALG_ADJ_GAIN},
    {8533,     10000,       100,    AEALG_IRIS_1,          AEALG_ADJ_GAIN|AEALG_ADJ_IRIS},
    {4266,     20000,       100,    AEALG_IRIS_1,          AEALG_ADJ_GAIN},
    {2133,     20000,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN|AEALG_ADJ_IRIS},
    {1066,     40000,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {533,      40000,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {267,      40000,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {133,      40000,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {67,       40000,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {0,       320000,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};


const UINT32 AeCurve_ISO50_60Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,        50,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {40960,     8333,        50,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {20480,    16666,        50,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {10240,    16666,        50,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {5120,     33333,        50,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {2560,     33333,        50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {1280,     66666,        50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {640,     133333,        50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {320,     266666,        50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {160,     533333,        50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {80,     1066666,        50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},                // 1/7.5*2
    {0,      2133333,        50,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO50_50Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,        50,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {34133,    10000,        50,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {17066,    20000,        50,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {8533,     20000,        50,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {4266,     40000,        50,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {2133,     40000,        50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {1066,     80000,        50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {533,     160000,        50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {267,     320000,        50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {133,     640000,        50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {67,     1280000,        50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {0,      2560000,        50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};


const UINT32 AeCurve_ISO80_60Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,        80,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {25600,     8333,        80,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {12800,    16666,        80,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {6400,     16666,        80,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {3200,     33333,        80,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {1600,     33333,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {800,      66666,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {400,     133333,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {200,     266666,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {100,     533333,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {50,     1066666,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},                // 1/7.5*2
    {0,      2133333,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO80_50Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,        80,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {21333,    10000,        80,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {10666,    20000,        80,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {5333,     20000,        80,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {2666,     40000,        80,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {1333,     40000,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {666,      80000,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {333,     160000,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {166,     320000,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {83,      640000,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {41,     1280000,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {0,      2560000,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO100_60Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {40960,     4167,       100,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {20480,     8333,       100,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {10240,     8333,       100,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {5120,     16666,       100,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {2560,     16666,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {1280,     33333,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {640,      66666,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {320,     133333,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {160,     266666,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {80,      533333,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},                // 1/7.5*2
    {0,      1066666,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO100_50Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {34133,     5000,       100,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {17066,    10000,       100,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {8533,     10000,       100,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {4266,     20000,       100,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {2133,     20000,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {1066,     40000,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {533,      80000,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {267,     160000,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {133,     320000,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {67,      640000,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {0,      1280000,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO200_60Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       200,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {40960,     2083,       200,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {20480,     4167,       200,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {10240,     4167,       200,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {5120,      8333,       200,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {2560,      8333,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {1280,     16666,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {640,      33333,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {320,      66666,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {160,     133333,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {80,      266666,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},                // 1/7.5*2
    {0,       533333,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO200_50Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       200,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {34133,     2500,       200,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {17066,     5000,       200,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {8533,      5000,       200,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {4266,     10000,       200,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {2133,     10000,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {1066,     20000,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {533,      40000,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {267,      80000,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {133,     160000,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {67,      320000,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {0,       640000,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO400_60Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       400,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {40960,     1041,       400,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {20480,     2083,       400,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {10240,     2083,       400,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {5120,      4167,       400,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {2560,      4167,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {1280,      8333,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {640,      16666,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {320,      33333,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {160,      66666,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {80,      133333,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},                // 1/7.5*2
    {0,       266666,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO400_50Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       400,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {34133,     1250,       400,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {17066,     2500,       400,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {8533,      2500,       400,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {4266,      5000,       400,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {2133,      5000,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {1066,     10000,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {533,      20000,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {267,      40000,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {133,      80000,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {67,      160000,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {0,       320000,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO800_60Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       800,    AEALG_IRIS_3,          AEALG_ADJ_EXP},
    {40960,     1041,       800,    AEALG_IRIS_3,          AEALG_ADJ_EXP},
    {20480,     2083,       800,    AEALG_IRIS_3,          AEALG_ADJ_EXP},
    {10240,     2083,       800,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {5120,      4167,       800,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {2560,      4167,       800,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {1280,      8333,       800,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {640,       8333,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {320,      16666,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {160,      33333,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {80,       66666,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},                // 1/7.5*2
    {0,       133333,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO800_50Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       800,    AEALG_IRIS_3,          AEALG_ADJ_EXP},
    {34133,     1250,       800,    AEALG_IRIS_3,          AEALG_ADJ_EXP},
    {17066,     2500,       800,    AEALG_IRIS_3,          AEALG_ADJ_EXP},
    {8533,      2500,       800,    AEALG_IRIS_3,          AEALG_ADJ_EXP},
    {4266,      5000,       800,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {2133,      5000,       800,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {1066,     10000,       800,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {533,      10000,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {267,      20000,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {133,      40000,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {67,       80000,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {0,       160000,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO1600_60Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,      1600,    AEALG_IRIS_4,          AEALG_ADJ_EXP},
    {40960,     1041,      1600,    AEALG_IRIS_4,          AEALG_ADJ_EXP},
    {20480,     1041,      1600,    AEALG_IRIS_3,          AEALG_ADJ_EXP},
    {10240,     2083,      1600,    AEALG_IRIS_3,          AEALG_ADJ_EXP},
    {5120,      2083,      1600,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {2560,      4167,      1600,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {1280,      4167,      1600,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {640,       8333,      1600,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {320,       8333,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {160,      16666,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {80,       33333,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP},                // 1/7.5*2
    {0,        66666,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO1600_50Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,      1600,    AEALG_IRIS_4,          AEALG_ADJ_EXP},
    {34133,     1250,      1600,    AEALG_IRIS_4,          AEALG_ADJ_EXP},
    {17066,     1250,      1600,    AEALG_IRIS_3,          AEALG_ADJ_EXP},
    {8533,      2500,      1600,    AEALG_IRIS_3,          AEALG_ADJ_EXP},
    {4266,      2500,      1600,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {2133,      5000,      1600,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {1066,      5000,      1600,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {533,      10000,      1600,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {267,      10000,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {133,      20000,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {67,       40000,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {0,        80000,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_Landscape[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,        50,    AEALG_IRIS_1,          AEALG_ADJ_EXP},
    {10240,    33333,        50,    AEALG_IRIS_1,          AEALG_ADJ_EXP}, // 1/60*2
    {5120,     33333,       100,    AEALG_IRIS_1,          AEALG_ADJ_GAIN}, // 1/60*2
    {2560,     33333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN}, // 1/60*2
    {1280,     33333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {640,      33333,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {320,      33333,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {160,      66666,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {  0,     133333,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_NightLandscape[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,     8333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN}, // 1/60*2
    {5120,     16666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN}, // 1/60*2
    {2560,     33333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN}, // 1/60*2
    {1280,     66666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {640,     133333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {320,     266667,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {160,     533333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    { 80,     533333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {  0,     533333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_Action[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,     8333,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},//
    {5120,      8333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {2560,      8333,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/30*2
    {1280,     16666,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {640,      33333,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {320,      33333,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {160,      66666,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {  0,     133333,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {AEALG_TAB_END, 0, 0, 0, 0}
};


const UINT32 AeCurve_Potrait[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,     8333,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/60*2
    {5120,      8333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/60*2
    {2560,     16666,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/30*2
    {1280,     33333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {640,      33333,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {320,      33333,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {160,      66666,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {  0,     133333,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_NightPotrait[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,     8333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN}, // 1/60*2
    {5120,     16666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/60*2
    {2560,     33333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/30*2
    {1280,     66666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {640,     133333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {320,     266666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {160,     533333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    { 80,     533333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {  0,     533333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {AEALG_TAB_END, 0, 0, 0, 0}
};

//Anti-flicker:60Hz
const UINT32 AeCurve_Cap60[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,        50,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {40960,     8333,        50,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {20480,     8333,       100,    AEALG_IRIS_2,          AEALG_ADJ_GAIN},
    {10240,     8333,       100,    AEALG_IRIS_1,          AEALG_ADJ_GAIN},
    {5120,     16666,       100,    AEALG_IRIS_1,          AEALG_ADJ_GAIN},
    {2560,     16666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {1280,     33333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {640,      33333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {320,      33333,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {160,      33333,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {80,       33333,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},                // 1/7.5*2
    {0,       533333,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}


};

//Anti-flicker:50Hz
const UINT32 AeCurve_Cap50[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,        50,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {34133,    10000,        50,    AEALG_IRIS_2,          AEALG_ADJ_EXP},
    {17066,    10000,       100,    AEALG_IRIS_2,          AEALG_ADJ_GAIN},
    {8533,     10000,       100,    AEALG_IRIS_1,          AEALG_ADJ_GAIN},
    {4266,     20000,       100,    AEALG_IRIS_1,          AEALG_ADJ_GAIN},
    {2133,     20000,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {1066,     40000,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {533,      40000,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {267,      40000,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {133,      40000,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {67,       40000,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {0,       320000,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};
