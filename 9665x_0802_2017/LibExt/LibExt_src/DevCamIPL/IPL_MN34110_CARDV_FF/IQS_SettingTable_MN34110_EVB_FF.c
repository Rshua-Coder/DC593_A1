#ifndef _IQS_SETTINGTABLE_MN34110_C
#define _IQS_SETTINGTABLE_MN34110_C
#include "IPL_MN34110_EVB_FF_Int.h"


////////////////////////////////////////////////
//PRE
////////////////////////////////////////////////
UINT8 PRE_ShadingLut[Total_ZOOMNum][17] =
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};



IQS_PRE_CENTER PRE_ShadingCenter[Total_SENMODENum] =
{
    {{2160, 1458}, {2160, 1458}, {2160, 1458}, {2160, 1458}}
};

IQS_PRE_VIGLUT_PARAM PRE_ShadingParam[Total_SENMODENum] =
{
    //DCExtEn, DCExtTh1, DCExtTh2, Mul,      T,   TabGain,            XDIV, YDIV,  DitherEn, DitherRstEn
    //{  DISABLE, 0,        0,        0x40,     0,   VIG_TAB_GAIN_2X,    1024,  1024, FALSE,    FALSE }
      {ENABLE, 109, 155, 76, 0, VIG_TAB_GAIN_1X, 962, 962, FALSE, FALSE}
};

////////////////////////////////////////////////
// IFE
////////////////////////////////////////////////
IQS_IFE_FUNC IFE_Func[Total_ISONum] =
{
    {ENABLE},
    {ENABLE},
    {ENABLE},
    {ENABLE},
    {ENABLE},

    {DISABLE},
    {DISABLE},
    {DISABLE},
    {ENABLE},
    {ENABLE},

    {ENABLE},

    {DISABLE},
    {DISABLE},
    {DISABLE},
    {ENABLE},
    {ENABLE},

    {DISABLE},
    {DISABLE},
    {DISABLE},
    {ENABLE},
    {ENABLE}
};


UINT32 IFE_weights[Total_ISONum][6] =
{
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},

    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {28, 23, 17, 9, 7, 3},
    {24, 16,  9, 2, 1, 0},

    {24, 16,  9, 2, 1, 0},

    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},

    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3}
};


UINT32 IFE_rngth_NLM[Total_ISONum][4] =
{
    {1,1,2,3},
    {1,3,4,5},
    {3,5,7,9},
    {4,7,9,12},
    {6,9,12,16},


    {1,1,2,3},
    {1,1,2,3},
    {1,3,4,5},
    {2,3,5,7},
    {3,6,9,12},

    {3,6,9,12},

    {2, 4,  6,  8},
    {3, 6,  9,  12},
    {5, 10, 15, 20},
    {8, 16, 24, 32},
    {12,24, 36, 48},

    {2, 4,  6,  8},
    {3, 6,  9,  12},
    {5, 10, 15, 20},
    {8, 16, 24, 32},
    {12,24, 36, 48}
};

UINT32 IFE_rngth_Bil[Total_ISONum][4] =
{
    {1,3,4,5},
    {2,4,6,8},
    {3,5,7,10},
    {5,8,12,16},
    {8,12,16,20},

    {1,3,4,5},
    {1,3,4,5},
    {3,5,8,10},
    {3,7,10,13},
    {6,12,18,24},

    {6,12,18,24},

    { 4, 8, 12, 16},
    { 6, 12, 18, 24},
    {10, 20, 30, 40},
    {16, 32, 48, 64},
    {12, 24, 36, 48},

    { 4, 8, 12, 16},
    { 6, 12, 18, 24},
    {10, 20, 30, 40},
    {16, 32, 48, 64},
    {12, 24, 36, 48}
};

IFE_FILT IFE_Filter_param[Total_ISONum] =
{
    //Mode,    {sOnlyEn, sOnlyLen, weights},                  {weightr_NLM,                     weightr_Bil    ,              bilat_w},  clamp{Th,  Mul, Dlt}, Rth_w, Bin}
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0100]}, {{IFE_rngth_NLM[PRVISO0100], 11}, {IFE_rngth_Bil[PRVISO0100], 17}, 1},  { 0, 128,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0200]}, {{IFE_rngth_NLM[PRVISO0200], 16}, {IFE_rngth_Bil[PRVISO0200], 32}, 1},  { 0, 128,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0400]}, {{IFE_rngth_NLM[PRVISO0400], 20}, {IFE_rngth_Bil[PRVISO0400], 40}, 1},  { 0, 128,   0},   2,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0800]}, {{IFE_rngth_NLM[PRVISO0800], 25}, {IFE_rngth_Bil[PRVISO0800], 50}, 2},  { 0, 128,   0},   3,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO1600]}, {{IFE_rngth_NLM[PRVISO1600], 30}, {IFE_rngth_Bil[PRVISO1600], 60}, 3},  { 0, 128,   0},   4,      0}, // 1125

    //Mode,    {sOnlyEn, sOnlyLen, weights},                  {weightr_NLM,                     weightr_Bil    ,              bilat_w},  clamp{Th,  Mul, Dlt}, Rth_w, Bin}
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[VQ2ISO0100]}, {{IFE_rngth_NLM[VQ2ISO0100], 11}, {IFE_rngth_Bil[VQ2ISO0100], 17}, 1},  { 0, 128,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[VQ2ISO0200]}, {{IFE_rngth_NLM[VQ2ISO0200], 11}, {IFE_rngth_Bil[VQ2ISO0200], 17}, 1},  { 0, 128,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[VQ2ISO0400]}, {{IFE_rngth_NLM[VQ2ISO0400], 16}, {IFE_rngth_Bil[VQ2ISO0400], 32}, 1},  { 0, 128,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[VQ2ISO0800]}, {{IFE_rngth_NLM[VQ2ISO0800], 20}, {IFE_rngth_Bil[VQ2ISO0800], 40}, 2},  { 3, 128,   0},   2,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[VQ2ISO1600]}, {{IFE_rngth_NLM[VQ2ISO1600], 30}, {IFE_rngth_Bil[VQ2ISO1600], 60}, 4},  { 5, 128,   0},  10,      0},

    { Normal,  {DISABLE, Filt_3X3, IFE_weights[VQ2ISO1600L]}, {{IFE_rngth_NLM[VQ2ISO1600L], 30}, {IFE_rngth_Bil[VQ2ISO1600L], 60}, 4},  { 5, 128,   0},  10,      0},

    //Mode,    {sOnlyEn, sOnlyLen, weights},                  {weightr_NLM,                     weightr_Bil    ,              bilat_w},  clamp{Th,  Mul, Dlt}, Rth_w, Bin}
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[QVISO0100]}, {{IFE_rngth_NLM[QVISO0100], 37}, {IFE_rngth_Bil[QVISO0100], 74 }, 1},  { 3, 0x80,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[QVISO0200]}, {{IFE_rngth_NLM[QVISO0200], 50}, {IFE_rngth_Bil[QVISO0200], 100}, 2},  { 4, 0x80,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[QVISO0400]}, {{IFE_rngth_NLM[QVISO0400], 67}, {IFE_rngth_Bil[QVISO0400], 134}, 4},  { 8, 0x80,   0},   1,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[QVISO0800]}, {{IFE_rngth_NLM[QVISO0800], 89}, {IFE_rngth_Bil[QVISO0800], 178}, 4},  { 9, 0x80,   0},   2,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[QVISO1600]}, {{IFE_rngth_NLM[QVISO1600], 126},{IFE_rngth_Bil[QVISO1600], 126}, 4},  { 9, 0x80,   0},   2,      0},

    //Mode,    {sOnlyEn, sOnlyLen, weights},                  {weightr_NLM,                     weightr_Bil    ,              bilat_w},  clamp{Th,  Mul, Dlt}, Rth_w, Bin}
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[CAPISO0100]}, {{IFE_rngth_NLM[CAPISO0100], 37}, {IFE_rngth_Bil[CAPISO0100], 74 }, 1},  { 3, 0x80,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[CAPISO0200]}, {{IFE_rngth_NLM[CAPISO0200], 50}, {IFE_rngth_Bil[CAPISO0200], 100}, 2},  { 4, 0x80,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[CAPISO0400]}, {{IFE_rngth_NLM[CAPISO0400], 67}, {IFE_rngth_Bil[CAPISO0400], 134}, 4},  { 8, 0x80,   0},   1,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[CAPISO0800]}, {{IFE_rngth_NLM[CAPISO0800], 89}, {IFE_rngth_Bil[CAPISO0800], 178}, 4},  { 9, 0x80,   0},   2,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[CAPISO1600]}, {{IFE_rngth_NLM[CAPISO1600], 126},{IFE_rngth_Bil[CAPISO1600], 126}, 4},  { 9, 0x80,   0},   2,      0}
};

UINT32 IFE_Outl_Bri[Total_ISONum][5] =
{
    {128, 128, 128, 128, 128},
    {128, 128, 128, 128, 128},
    {128, 128, 128, 128, 128},
    {128, 128, 128, 128, 128},
    {128, 128, 128, 128, 128},

    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},

    {126, 184, 304, 420, 512},

    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {200, 300, 350, 420, 512},
    {200, 300, 350, 420, 512},

    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {200, 300, 350, 420, 512},
    {200, 300, 350, 420, 512}
};

UINT32 IFE_Outl_Dark[Total_ISONum][5] =
{
    {128, 128, 128, 128, 128},
    {128, 128, 128, 128, 128},
    {128, 128, 128, 128, 128},
    {128, 128, 128, 128, 128},
    {128, 128, 128, 128, 128},

    {189, 276, 456, 630, 768},
    {189, 276, 456, 630, 768},
    {189, 276, 456, 630, 768},
    {189, 276, 456, 630, 768},
    {189, 276, 456, 630, 768},

    {189, 276, 456, 630, 768},

    {189, 276, 456, 630, 768},
    {189, 276, 456, 630, 768},
    {189, 276, 456, 630, 768},
    {300, 450, 525, 630, 768},
    {300, 450, 525, 630, 768},

    {189, 276, 456, 630, 768},
    {189, 276, 456, 630, 768},
    {189, 276, 456, 630, 768},
    {300, 450, 525, 630, 768},
    {300, 450, 525, 630, 768}
};

IFE_OUTL IFE_Outl_param[Total_ISONum] =
{
    {IFE_Outl_Bri[PRVISO0100], IFE_Outl_Dark[PRVISO0100], _8_NB},
    {IFE_Outl_Bri[PRVISO0200], IFE_Outl_Dark[PRVISO0200], _8_NB},
    {IFE_Outl_Bri[PRVISO0400], IFE_Outl_Dark[PRVISO0400], _8_NB},
    {IFE_Outl_Bri[PRVISO0800], IFE_Outl_Dark[PRVISO0800], _8_NB},
    {IFE_Outl_Bri[PRVISO1600], IFE_Outl_Dark[PRVISO1600], _8_NB},

    {IFE_Outl_Bri[VQ2ISO0100], IFE_Outl_Dark[VQ2ISO0100], _8_NB},
    {IFE_Outl_Bri[VQ2ISO0200], IFE_Outl_Dark[VQ2ISO0200], _8_NB},
    {IFE_Outl_Bri[VQ2ISO0400], IFE_Outl_Dark[VQ2ISO0400], _8_NB},
    {IFE_Outl_Bri[VQ2ISO0800], IFE_Outl_Dark[VQ2ISO0800], _8_NB},
    {IFE_Outl_Bri[VQ2ISO1600], IFE_Outl_Dark[VQ2ISO1600], _8_NB},

    {IFE_Outl_Bri[VQ2ISO1600L], IFE_Outl_Dark[VQ2ISO1600L], _8_NB},

    //BrightThres,          DarkThres,             IFE_OUTLCNT
    {IFE_Outl_Bri[QVISO0100], IFE_Outl_Dark[QVISO0100], _8_NB},
    {IFE_Outl_Bri[QVISO0200], IFE_Outl_Dark[QVISO0200], _8_NB},
    {IFE_Outl_Bri[QVISO0400], IFE_Outl_Dark[QVISO0400], _8_NB},
    {IFE_Outl_Bri[QVISO0800], IFE_Outl_Dark[QVISO0800], _8_NB},
    {IFE_Outl_Bri[QVISO1600], IFE_Outl_Dark[QVISO1600], _8_NB},

    //BrightThres,          DarkThres,             IFE_OUTLCNT
    {IFE_Outl_Bri[CAPISO0100], IFE_Outl_Dark[CAPISO0100], _8_NB},
    {IFE_Outl_Bri[CAPISO0200], IFE_Outl_Dark[CAPISO0200], _8_NB},
    {IFE_Outl_Bri[CAPISO0400], IFE_Outl_Dark[CAPISO0400], _8_NB},
    {IFE_Outl_Bri[CAPISO0800], IFE_Outl_Dark[CAPISO0800], _8_NB},
    {IFE_Outl_Bri[CAPISO1600], IFE_Outl_Dark[CAPISO1600], _8_NB},
};

//Curve Lut for IFE YCC only
UINT32 IFE_CurveLut_ycc[65] =
{
      0,   4,   8,  12,  16,  20,  24,  28,  32,  36,
     40,  44,  48,  52,  56,  60,  64,  68,  72,  76,
     80,  84,  88,  92,  96, 100, 104, 108, 112, 116,
    120, 124, 128, 132, 136, 140, 144, 148, 152, 156,
    160, 164, 168, 172, 176, 180, 184, 188, 192, 196,
    200, 204, 208, 212, 216, 220, 224, 228, 232, 236,
    240, 244, 248, 252, 255,
};

IFE_CRVMAP IFE_CurveMap_param_ycc =
{
    //CRVMapDelt, In{cb, cr}, Out{cb, cr}, Gamma_TblAddr
      _1_LUT,        {0,  0},    { 0,  0}, (UINT32)IFE_CurveLut_ycc
};

////////////////////////////////////////////////
// DCE
////////////////////////////////////////////////
Fact_Norm   DCE_Radious[Total_SENMODENum] =
{
    //Manual_NormTermEn, Factor, Bit
    {DISABLE,                0,    0}
};

DIST DCE_Dist[Total_SENMODENum] =
{
    {0xfff,0xfff,Bar_Pin}
};

FOV DCE_Fov[Total_ZOOMNum] =
{
    {0x400,{DISABLE,1024,1024,1024}}
};

EnH DCE_Enh[Total_ZOOMNum] =
{
    //YEnh,      UVEnh
    { 0,   6,   16,   6  }
};

Aberation DCE_ColorAbbration[Total_ZOOMNum] =
{
    {0x1000, 0x1000, 0x1000},
};

UINT32 DCE_2D_LUT[Total_ZOOMNum][65] =
{
    {65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535},
};

////////////////////////////////////////////////
// IPE
////////////////////////////////////////////////
UINT32 IPE_ColorGain_Offset[Total_ISONum][4] =
{
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},

    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},

    {0,0,0,0},

    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};

UINT8 IPE_Gbal_Stab[Total_ISONum][16] =
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},


    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

UINT8 IPE_Gbal_Dtab[Total_ISONum][16] =
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

    {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

    {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

    {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

IPE_GBAL IPE_Gbal[Total_ISONum] =
{
    //Thres    {STab[16]}                 {DTab[16]}
    {  1023,   IPE_Gbal_Stab[PRVISO0100],    IPE_Gbal_Dtab[PRVISO0100]},
    {  1023,   IPE_Gbal_Stab[PRVISO0200],    IPE_Gbal_Dtab[PRVISO0200]},
    {  1023,   IPE_Gbal_Stab[PRVISO0400],    IPE_Gbal_Dtab[PRVISO0400]},
    {  1023,   IPE_Gbal_Stab[PRVISO0800],    IPE_Gbal_Dtab[PRVISO0800]},
    {  1023,   IPE_Gbal_Stab[PRVISO1600],    IPE_Gbal_Dtab[PRVISO1600]},

    //Thres    {STab[16]}                 {DTab[16]}
    {  1023,   IPE_Gbal_Stab[VQ2ISO0100],    IPE_Gbal_Dtab[VQ2ISO0100]},
    {  1023,   IPE_Gbal_Stab[VQ2ISO0200],    IPE_Gbal_Dtab[VQ2ISO0200]},
    {  1023,   IPE_Gbal_Stab[VQ2ISO0400],    IPE_Gbal_Dtab[VQ2ISO0400]},
    {  1023,   IPE_Gbal_Stab[VQ2ISO0800],    IPE_Gbal_Dtab[VQ2ISO0800]},
    {  1023,   IPE_Gbal_Stab[VQ2ISO1600],    IPE_Gbal_Dtab[VQ2ISO1600]},

    {  1023,   IPE_Gbal_Stab[VQ2ISO1600L],    IPE_Gbal_Dtab[VQ2ISO1600L]},

    //Thres    {STab[16]}                 {DTab[16]}
    {  1023,   IPE_Gbal_Stab[QVISO0100],    IPE_Gbal_Dtab[QVISO0100]},
    {  1023,   IPE_Gbal_Stab[QVISO0200],    IPE_Gbal_Dtab[QVISO0200]},
    {  1023,   IPE_Gbal_Stab[QVISO0400],    IPE_Gbal_Dtab[QVISO0400]},
    {  1023,   IPE_Gbal_Stab[QVISO0800],    IPE_Gbal_Dtab[QVISO0800]},
    {  1023,   IPE_Gbal_Stab[QVISO1600],    IPE_Gbal_Dtab[QVISO1600]},

    //Thres    {STab[16]}                 {DTab[16]}
    {  1023,   IPE_Gbal_Stab[CAPISO0100],   IPE_Gbal_Dtab[CAPISO0100]},
    {  1023,   IPE_Gbal_Stab[CAPISO0200],   IPE_Gbal_Dtab[CAPISO0200]},
    {  1023,   IPE_Gbal_Stab[CAPISO0400],   IPE_Gbal_Dtab[CAPISO0400]},
    {  1023,   IPE_Gbal_Stab[CAPISO0800],   IPE_Gbal_Dtab[CAPISO0800]},
    {  1023,   IPE_Gbal_Stab[CAPISO1600],   IPE_Gbal_Dtab[CAPISO1600]}

};



IPE_CFAINTER IPE_CFAInter[Total_ISONum] =
{
    //NsmarEdge,  NsmarDiff,   {GCEn, GCNBit, DIFNBit}
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},

    //NsmarEdge,  NsmarDiff,   {GCEn, GCNBit, DIFNBit}
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},

    {       0,            8,   {TRUE,      2,       0}},

    //NsmarEdge,  NsmarDiff,   {GCEn, GCNBit, DIFNBit}
    {       0,            8,   {TRUE,      1,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       8,            0,   {TRUE,      2,       0}},
    {       8,            0,   {TRUE,      2,       0}},

    //NsmarEdge,  NsmarDiff,   {GCEn, GCNBit, DIFNBit}
    {       0,            8,   {TRUE,      1,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       8,            0,   {TRUE,      2,       0}},
    {       8,            0,   {TRUE,      2,       0}}
};


INT16 IPE_edgeKernel[Total_ISONum][11] =
{
    //eext0 ~ eext9, eextdiv
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x009},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x009},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x009},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x009},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x009},

    //eext0 ~ eext9, eextdiv
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x009},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x009},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x009},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x009},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x009},

    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x009},

    //eext0 ~ eext9, eextdiv
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x00a},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x00a},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x00a},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x00a},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x00a},

    //eext0 ~ eext9, eextdiv
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x00a},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x00a},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x00a},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x00a},
    { 0x110, 0x02c, 0x3b8, 0x3e1, 0x3da, 0x004, 0x014, 0x00f, 0x006, 0x001, 0x00a}
};

UINT8 IPE_EDirTab[Total_ISONum][8]=
{
    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},

    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},

    {0,2,3,4,5,5,4,2},

    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},
    {0,1,2,3,4,4,4,2},

    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},
    {0,1,2,3,4,4,4,2}
};

IPE_DEE IPE_DirEdgeExt[Total_ISONum] =
{
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0100]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0200]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0400]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0800]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO1600]},

    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[VQ2ISO0100]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[VQ2ISO0200]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[VQ2ISO0400]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[VQ2ISO0800]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[VQ2ISO1600]},

    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[VQ2ISO1600L]},

    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[QVISO0100]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[QVISO0200]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[QVISO0400]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[QVISO0800]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[QVISO1600]},

    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[CAPISO0100]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[CAPISO0200]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[CAPISO0400]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[CAPISO0800]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[CAPISO1600]}
};

IPE_ESD IPE_ESymDiff[Total_ISONum] =
{
    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},

    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},

    {ESD_3X3,  LminusR,  UminusL,      0},

    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},

    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0},
    {ESD_3X3,  LminusR,  UminusL,      0}
};

IPE_EDGEEXT IPE_EdgeExt[Total_ISONum] =
{
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0100],    0,   &IPE_DirEdgeExt[PRVISO0100],   &IPE_ESymDiff[PRVISO0100]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0200],    0,   &IPE_DirEdgeExt[PRVISO0200],   &IPE_ESymDiff[PRVISO0200]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0400],    1,   &IPE_DirEdgeExt[PRVISO0400],   &IPE_ESymDiff[PRVISO0400]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0800],    2,   &IPE_DirEdgeExt[PRVISO0800],   &IPE_ESymDiff[PRVISO0800]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO1600],    3,   &IPE_DirEdgeExt[PRVISO1600],   &IPE_ESymDiff[PRVISO1600]},

    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[VQ2ISO0100],    0,   &IPE_DirEdgeExt[VQ2ISO0100],   &IPE_ESymDiff[VQ2ISO0100]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[VQ2ISO0200],    0,   &IPE_DirEdgeExt[VQ2ISO0200],   &IPE_ESymDiff[VQ2ISO0200]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[VQ2ISO0400],    0,   &IPE_DirEdgeExt[VQ2ISO0400],   &IPE_ESymDiff[VQ2ISO0400]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[VQ2ISO0800],    0,   &IPE_DirEdgeExt[VQ2ISO0800],   &IPE_ESymDiff[VQ2ISO0800]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[VQ2ISO1600],    4,   &IPE_DirEdgeExt[VQ2ISO1600],   &IPE_ESymDiff[VQ2ISO1600]},

    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[VQ2ISO1600L],    0,   &IPE_DirEdgeExt[VQ2ISO1600L],   &IPE_ESymDiff[VQ2ISO1600L]},

    //    GamSel, EdgeChSel,  EEXT_TblAddr,                       EWDIR    DED                          ESD}
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[QVISO0100],    2,   &IPE_DirEdgeExt[QVISO0100],   &IPE_ESymDiff[QVISO0100]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[QVISO0200],    2,   &IPE_DirEdgeExt[QVISO0200],   &IPE_ESymDiff[QVISO0200]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[QVISO0400],    2,   &IPE_DirEdgeExt[QVISO0400],   &IPE_ESymDiff[QVISO0400]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[QVISO0800],    3,   &IPE_DirEdgeExt[QVISO0800],   &IPE_ESymDiff[QVISO0800]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[QVISO1600],    4,   &IPE_DirEdgeExt[QVISO1600],   &IPE_ESymDiff[QVISO1600]},

    //    GamSel, EdgeChSel,  EEXT_TblAddr,                       EWDIR    DED                          ESD}
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO0100],    2,   &IPE_DirEdgeExt[CAPISO0100],   &IPE_ESymDiff[CAPISO0100]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO0200],    2,   &IPE_DirEdgeExt[CAPISO0200],   &IPE_ESymDiff[CAPISO0200]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO0400],    2,   &IPE_DirEdgeExt[CAPISO0400],   &IPE_ESymDiff[CAPISO0400]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO0800],    3,   &IPE_DirEdgeExt[CAPISO0800],   &IPE_ESymDiff[CAPISO0800]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO1600],    4,   &IPE_DirEdgeExt[CAPISO1600],   &IPE_ESymDiff[CAPISO1600]}
};

IPE_EDGEENH IPE_EdgeEnh[Total_ISONum] =
{
    {     400,  400,  FALSE, FALSE},
    {     300,  300,  FALSE, FALSE},
    {     200,  100,  FALSE, FALSE},
    {     150,   80,  FALSE, FALSE},
    {     100,   50,  FALSE, FALSE},

    {     400,  400,  FALSE, FALSE},
    {     300,  300,  FALSE, FALSE},
    {     200,  200,  FALSE, FALSE},
    {     160,  160,  FALSE, FALSE},
    {     140,  140,  FALSE, FALSE},

    {     112,  112,  FALSE, FALSE},

    //  EnhP,  EnhN,  InvP,  InvN
    {     128, 128,  FALSE, FALSE},
    {     112, 112,  FALSE, FALSE},
    {     112, 112,  FALSE, FALSE},
    {     128, 128,  FALSE, FALSE},
    {     136, 136,  FALSE, FALSE},

    //  EnhP,  EnhN,  InvP,  InvN
    {     128, 128,  FALSE, FALSE},
    {     112, 112,  FALSE, FALSE},
    {     112, 112,  FALSE, FALSE},
    {     128, 128,  FALSE, FALSE},
    {     136, 136,  FALSE, FALSE}
};

//EDLut
UINT8 IPE_EDTab[Total_ISONum][16] =
{
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},

    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},

    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},

    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},

    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 128, 160, 192, 224, 255, 255, 228, 196, 160, 128, 96, 64, 32}
};

//ESYMLUTL0
UINT8 IPE_ESymTab[Total_ISONum][16] =
{
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},

    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},

    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},

    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},

    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}
};

UINT8 IPE_EStab[Total_ISONum][16] =
{
    {56, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {56, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {56, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {56, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {56, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},

    {56, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {56, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {56, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {56, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {56, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},

    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},

    {56, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {48, 56, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48},

    {56, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {48, 56, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48}
};

EdgeMap IPE_EDMap[Total_ISONum] =
{
    //{EM_InSel, EthrL, EthrH, EtabL, EtabH, TAB}
    { EMI_AVG,     0,    256,     0,     4, (UINT32)&IPE_EDTab[PRVISO0100]},
    { EMI_AVG,     4,    256,     0,     4, (UINT32)&IPE_EDTab[PRVISO0200]},
    { EMI_AVG,     8,    256,     1,     4, (UINT32)&IPE_EDTab[PRVISO0400]},
    { EMI_AVG,    12,    256,     1,     4, (UINT32)&IPE_EDTab[PRVISO0800]},
    { EMI_AVG,    20,    256,     1,     4, (UINT32)&IPE_EDTab[PRVISO1600]},

    //{EM_InSel, EthrL, EthrH, EtabL, EtabH, TAB}
    { EMI_AVG,     0,    256,     0,     4, (UINT32)&IPE_EDTab[VQ2ISO0100]},
    { EMI_AVG,     0,    256,     0,     4, (UINT32)&IPE_EDTab[VQ2ISO0200]},
    { EMI_AVG,     0,    256,     0,     4, (UINT32)&IPE_EDTab[VQ2ISO0400]},
    { EMI_AVG,     0,    256,     0,     4, (UINT32)&IPE_EDTab[VQ2ISO0800]},
    { EMI_AVG,     0,    256,     0,     4, (UINT32)&IPE_EDTab[VQ2ISO1600]},

    { EMI_AVG,     0,    256,     1,     4, (UINT32)&IPE_EDTab[VQ2ISO1600L]},

    //{EM_InSel, EthrL, EthrH, EtabL, EtabH, TAB}
    { EMI_AVG,     0,    256,     0,     4, (UINT32)&IPE_EDTab[QVISO0100]},
    { EMI_AVG,     0,    256,     1,     4, (UINT32)&IPE_EDTab[QVISO0200]},
    { EMI_AVG,     0,    256,     1,     4, (UINT32)&IPE_EDTab[QVISO0400]},
    { EMI_AVG,     0,    256,     1,     4, (UINT32)&IPE_EDTab[QVISO0800]},
    { EMI_AVG,     0,    256,     1,     4, (UINT32)&IPE_EDTab[QVISO1600]},

    //{EM_InSel, EthrL, EthrH, EtabL, EtabH, TAB}
    { EMI_AVG,     0,    256,     0,     4, (UINT32)&IPE_EDTab[CAPISO0100]},
    { EMI_AVG,     0,    256,     1,     4, (UINT32)&IPE_EDTab[CAPISO0200]},
    { EMI_AVG,     0,    256,     1,     4, (UINT32)&IPE_EDTab[CAPISO0400]},
    { EMI_AVG,     0,    256,     1,     4, (UINT32)&IPE_EDTab[CAPISO0800]},
    { EMI_AVG,     0,    256,     1,     4, (UINT32)&IPE_EDTab[CAPISO1600]}
};

EdgeMap IPE_ESymMap[Total_ISONum] =
{
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0100]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0200]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0400]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0800]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO1600]},

    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[VQ2ISO0100]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[VQ2ISO0200]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[VQ2ISO0400]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[VQ2ISO0800]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[VQ2ISO1600]},

    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[VQ2ISO1600L]},

    //{EM_InSel, EthrL, EthrH, EtabL, EtabH, TAB}
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[QVISO0100]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[QVISO0200]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[QVISO0400]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[QVISO0800]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[QVISO1600]},

    //{EM_InSel, EthrL, EthrH, EtabL, EtabH, TAB}
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[CAPISO0100]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[CAPISO0200]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[CAPISO0400]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[CAPISO0800]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[CAPISO1600]}
};

IPE_RGBLPF IPE_RGBLpf[Total_ISONum] =
{
    //  lpfw,sonlyw,rangeth0,rangeth1,lpfsize;
    {{    2,     0,       8,       16,    _3x3}, {    2,     0,       8,       16,    _3x3}, {    2,     0,       8,       16,    _3x3}},
    {{    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}},
    {{    6,     0,       8,       16,    _5x5}, {    6,     0,       8,       16,    _5x5}, {    6,     0,       8,       16,    _5x5}},
    {{    6,     0,       8,       16,    _7x7}, {    6,     0,       8,       16,    _7x7}, {    6,     0,       8,       16,    _7x7}},
    {{    6,     0,       8,       16,    _7x7}, {    6,     0,       8,       16,    _7x7}, {    6,     0,       8,       16,    _7x7}},

    //  lpfw,sonlyw,rangeth0,rangeth1,lpfsize;
    {{    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}},
    {{    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}},
    {{    6,     0,       8,       16,    _3x3}, {    6,     0,       8,       16,    _3x3}, {    6,     0,       8,       16,    _3x3}},
    {{   12,     0,       8,       16,    _5x5}, {   12,     0,       8,       16,    _5x5}, {   12,     0,       8,       16,    _5x5}},
    {{   15,     0,       4,       28,    _7x7}, {   15,     0,       4,       28,    _7x7}, {   15,     0,       4,       28,    _7x7}},

    {{   15,     0,       4,       28,    _7x7}, {   15,     0,       4,       28,    _7x7}, {   15,     0,       4,       28,    _7x7}},

    //rlpfw,rsonlyw,rrangeth0,rrangeth1,rlpfsize,    glpfw,gsonlyw,grangeth0,grangeth1,glpfsize,    blpfw,bsonlyw,brangeth0,brangeth1,blpfsize
    {{    4,      0,      8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}},
    {{    6,      0,      8,       16,    _3x3}, {    6,     0,       8,       16,    _3x3}, {    6,     0,       8,       16,    _3x3}},
    {{   10,      0,     10,       20,    _5x5}, {   10,     0,      10,       20,    _5x5}, {   10,     0,      10,       20,    _5x5}},
    {{   12,      0,     12,       24,    _7x7}, {   12,     0,      12,       24,    _7x7}, {   12,     0,      12,       24,    _7x7}},
    {{   15,     15,     16,       32,    _7x7}, {   15,    15,      16,       32,    _7x7}, {   15,    15,      16,       32,    _7x7}},

    //rlpfw,rsonlyw,rrangeth0,rrangeth1,rlpfsize,    glpfw,gsonlyw,grangeth0,grangeth1,glpfsize,    blpfw,bsonlyw,brangeth0,brangeth1,blpfsize
    {{    4,      0,      8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}},
    {{    6,      0,      8,       16,    _3x3}, {    6,     0,       8,       16,    _3x3}, {    6,     0,       8,       16,    _3x3}},
    {{   10,      0,     10,       20,    _5x5}, {   10,     0,      10,       20,    _5x5}, {   10,     0,      10,       20,    _5x5}},
    {{   12,      0,     12,       24,    _7x7}, {   12,     0,      12,       24,    _7x7}, {   12,     0,      12,       24,    _7x7}},
    {{   15,     15,     16,       32,    _7x7}, {   15,    15,      16,       32,    _7x7}, {   15,    15,      16,       32,    _7x7}}
};

UINT16 Normal_CC[9] = {
#if 0
    0x01c9, 0x073a, 0x07fd,
    0x07a1, 0x0179, 0x07e6,
    0x07db, 0x0796, 0x018f
#elif 1
    0x018d,0x0742,0x0030,
    0x07b9,0x0183,0x07c4,
    0x07fb,0x074e,0x01b7,
#else

//1.1x
// 2.07170, -1.02748, -0.04422,
//-0.50265,  1.66360, -0.16094,
//-0.22269, -0.72207,  1.94471,
0x212, 0x6f9, 0x7f5,
0x780, 0x1a9, 0x7d7,
0x7c7, 0x748, 0x1f1,

/*
//1.2x
// 2.29736, -1.14323, -0.15413,
//-0.60343,  1.72324, -0.11980,
//-0.29561, -0.72560,  2.02117,
0x024c, 0x06dc, 0x07d9,
0x0766, 0x01b9, 0x07e2,
0x07b5, 0x0747, 0x0205,
*/
#endif
};

//FSTAB
UINT8 CC_fmStab[Total_ISONum][16]=
{
    { 8, 0,0,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {16, 8,0,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},

    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},

    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},

    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},

    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255}
};

//FDTAB
UINT8 CC_fmDtab[Total_ISONum][16]=
{
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},

    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},

    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},

    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},

    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0}
};

UINT8 IPE_hueTab[24]=
{
    128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128
};

//CSTAM
INT32 IPE_satTab[24]=
{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

//CINTM
INT32 IPE_intTab[24]=
{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

//CEDGM
UINT8 IPE_edgTab[24]=
{
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128
};

UINT8 IPE_ddsTab[8]=
{
    8,16,32,32,32,32,32,32
};

IPE_RAND_NR IPE_NoiseParam = { TRUE, 0, 0};

//#NT#2013/12/25#Jarkko Chang -begin
//#NT# add for switch movie/photo gamma curve: DODLS500W video mode will always use WDR_Gamma
UINT32 GammaLUT_128Tab_Mov[130]={
0, 48, 96, 144, 188, 228, 264, 296, 324, 348,
372, 392, 408, 424, 440, 452, 468, 480, 492, 508,
516, 528, 540, 552, 560, 568, 580, 588, 596, 604,
612, 620, 628, 636, 644, 652, 660, 664, 672, 680,
684, 692, 696, 704, 708, 716, 720, 728, 732, 736,
744, 748, 752, 760, 764, 768, 776, 780, 784, 788,
792, 800, 804, 808, 812, 816, 820, 824, 828, 832,
836, 840, 844, 848, 852, 856, 860, 864, 868, 872,
872, 876, 880, 884, 888, 892, 896, 900, 904, 904,
908, 912, 916, 920, 924, 928, 928, 932, 936, 940,
944, 944, 948, 952, 956, 956, 960, 964, 968, 968,
972, 976, 976, 980, 984, 984, 988, 992, 996, 996,
1000, 1004, 1004, 1008, 1012, 1012, 1016, 1020, 1023,
};

UINT32 GammaLUT_128Tab_Photo[130]={
0, 36, 68, 100, 136, 168, 200, 232, 264, 292,
320, 348, 372, 396, 416, 436, 452, 472, 484, 500,
512, 524, 536, 548, 556, 568, 576, 584, 592, 604,
612, 620, 628, 636, 644, 648, 656, 664, 672, 676,
684, 692, 696, 704, 708, 716, 720, 728, 732, 736,
744, 748, 752, 760, 764, 768, 772, 780, 784, 788,
792, 796, 804, 808, 812, 816, 820, 824, 828, 832,
836, 840, 844, 848, 852, 856, 860, 864, 868, 872,
876, 880, 884, 888, 888, 892, 896, 900, 904, 908,
912, 912, 916, 920, 924, 928, 928, 932, 936, 940,
940, 944, 948, 952, 952, 956, 960, 964, 964, 968,
972, 976, 976, 980, 984, 984, 988, 992, 992, 996,
1000, 1004, 1004, 1008, 1012, 1012, 1016, 1020, 1023, 1023
};
//#NT#2013/12/25#Jarkko Chang -end

INT8  Saturation[Total_ISONum][SEL_SATURATION_MAX_CNT] = {
    {-20,   0,   20},
    {-20,   0,   20},
    {-27,  -7,   13},
    {-32, -17,    3},
    {-47, -37,  -17},

    {-20,   0,   20},
    {-20,   0,   20},
    {-20,   0,   20},
    {-20,   0,   20},
    {-20,   0,   20},

    {-20,   0,   20},

    {-20,   0,   20},
    {-20,   0,   20},
    {-20,   0,   20},
    {-20,   0,   20},
    {-20,   0,   20},

    {-20,   0,   20},
    {-20,   0,   20},
    {-20,   0,   20},
    {-20,   0,   20},
    {-20,   0,   20},

};


UINT8 Sharpness[Total_ISONum][SEL_SHARPNESS_MAX_CNT] = {
    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},

    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},

    {32,   128,   255},

    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},

   //P1,  Normal,  N1
    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},
};

//For DIS
IPE_EDGETHRE IPE_EdgeThre[Total_ISONum] = {
    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},

    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},

    {EdgeKer_5x5_OUT, 32, 128, 192},

    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},

    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192},
    {EdgeKer_5x5_OUT, 32, 128, 192}
};

IPE_YCCFIX IPE_YCFixEffect =
{
  //yth1{ETHY, YTH1,  HIT1SEL,NHIT1SEL,HIT,NHIT},
        {0,      0,    FALSE,  FALSE,    0,   0},
  //yth2{YTH2,HIT2SEL,NHIT2SEL,HIT,NHIT},
        {0,    FALSE,  FALSE,    0,   0},
  //  CC{ETH,  YTH,  CBTH,   CRTH, HITSEL,NHITSEL,CB_HIT,CB_NHIT,CR_HIT,CR_NHIT}
        {0,   {0,0}, {0,0},  {0,0}, FALSE,  FALSE,     0,      0,     0,     0}
};

IPE__VA IPE_VAParam[Total_ISONum] =
{   //VACC , VDET
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},

    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},

    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},

    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},

    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}},
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}}
};

////////////////////////////////////////////////
// IFE2
////////////////////////////////////////////////

IFE2_Chroma IFE2_Chroma_param[Total_ISONum] =
{
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},

    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},

    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},

    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},

    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  3,  5,  7}, {128, 96, 64, 32, 0}}
};

IQS_IFE2_FILTER_TIMES IFE2_Filter_Times[Total_ISONum] =
{
    //IFE2_CC, IFE2_V, MFR
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       0,       0},

    //IFE2_CC, IFE2_V, MFR
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       0,       0},

    { 1,       0,       0},

    //IFE2_CC, IFE2_V, MFR
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       0,       0},

    //IFE2_CC, IFE2_V, MFR
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       0,       1},
    { 1,       0,       1}
};

//wdr
AvgFilt IFE2_AvgFilter_y[Total_ISONum] =
{
    //cenWt, avgTh,     cntTh, fltSize
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},

    //cenWt, avgTh,     cntTh, fltSize
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},

      {  32, { 5, 10},      0, IFE2_FLTR_7X7},

    //cenWt, avgTh,     cntTh, fltSize
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},

    //cenWt, avgTh,     cntTh, fltSize
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
};

//lca
AvgFilt IFE2_AvgFilter_cb[Total_ISONum] =
{
    //cenWt, avgTh,    cntTh, fltSize
      {  10, { 2,  5},     0, IFE2_FLTR_5X5},
      {  10, { 3,  7},     0, IFE2_FLTR_5X5},
      {  10, { 3,  7},     0, IFE2_FLTR_7X7},
      {  10, { 3,  7},     0, IFE2_FLTR_7X7},
      {  10, { 3,  7},     0, IFE2_FLTR_7X7},

    //cenWt, avgTh,    cntTh, fltSize
      {  10, { 2,  5},     0, IFE2_FLTR_5X5},
      {  10, { 2,  5},     0, IFE2_FLTR_5X5},
      {  10, { 3,  7},     0, IFE2_FLTR_5X5},
      {  10, { 3,  7},     0, IFE2_FLTR_7X7},
      {  10, { 3,  7},     0, IFE2_FLTR_7X7},

      {  10, { 3,  7},     0, IFE2_FLTR_7X7},

    //cenWt, avgTh,     cntTh, fltSize
      {  10, { 8, 15},      0, IFE2_FLTR_7X7},
      {  10, {10, 16},      0, IFE2_FLTR_7X7},
      {  10, {12, 18},      0, IFE2_FLTR_7X7},
      {  10, {13, 18},      0, IFE2_FLTR_7X7},
      {  10, {16, 20},      1, IFE2_FLTR_7X7},

    //cenWt, avgTh,     cntTh, fltSize
      {  10, { 8, 15},      0, IFE2_FLTR_7X7},
      {  10, {10, 16},      0, IFE2_FLTR_7X7},
      {  10, {12, 18},      0, IFE2_FLTR_7X7},
      {  10, {13, 18},      0, IFE2_FLTR_7X7},
      {  10, {16, 20},      1, IFE2_FLTR_7X7},
};

AvgFilt IFE2_AvgFilter_cr[Total_ISONum] =
{
    //cenWt, avgTh,    cntTh, fltSize
      {  10, { 2,  5},     0, IFE2_FLTR_5X5},
      {  10, { 3,  7},     0, IFE2_FLTR_5X5},
      {  10, { 3,  7},     0, IFE2_FLTR_7X7},
      {  10, { 3,  7},     0, IFE2_FLTR_7X7},
      {  10, { 3,  7},     0, IFE2_FLTR_7X7},

    //cenWt, avgTh,    cntTh, fltSize
      {  10, { 2,  5},     0, IFE2_FLTR_5X5},
      {  10, { 2,  5},     0, IFE2_FLTR_5X5},
      {  10, { 3,  7},     0, IFE2_FLTR_5X5},
      {  10, { 3,  7},     0, IFE2_FLTR_7X7},
      {  10, { 3,  7},     0, IFE2_FLTR_7X7},

      {  10, { 3,  7},     0, IFE2_FLTR_7X7},

    //cenWt, avgTh,     cntTh, fltSize
      {  10, { 8, 15},      0, IFE2_FLTR_7X7},
      {  10, {10, 16},      0, IFE2_FLTR_7X7},
      {  10, {12, 18},      0, IFE2_FLTR_7X7},
      {  10, {13, 18},      0, IFE2_FLTR_7X7},
      {  10, {16, 20},      0, IFE2_FLTR_7X7},
    //cenWt, avgTh,     cntTh, fltSize
      {  10, { 8, 15},      0, IFE2_FLTR_7X7},
      {  10, {10, 16},      0, IFE2_FLTR_7X7},
      {  10, {12, 18},      0, IFE2_FLTR_7X7},
      {  10, {13, 18},      0, IFE2_FLTR_7X7},
      {  10, {16, 20},      0, IFE2_FLTR_7X7},
};

//mfr
AvgFilt IFE2_AvgFilter_y_mfr[Total_ISONum] =
{
    //cenWt, avgTh,     cntTh, fltSize
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},

    //cenWt, avgTh,     cntTh, fltSize
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},
      {  32, { 5, 10},      0, IFE2_FLTR_7X7},

      {  32, { 5, 10},      0, IFE2_FLTR_7X7},

    //cenWt, avgTh,     cntTh, fltSize
      {  0, { 0, 0},      0, IFE2_FLTR_7X7},
      {  0, { 0, 0},      0, IFE2_FLTR_7X7},
      {  0, { 0, 0},      0, IFE2_FLTR_7X7},
      { 16, { 6,10},      0, IFE2_FLTR_7X7},
      { 16, {10,15},      0, IFE2_FLTR_7X7},

    //cenWt, avgTh,     cntTh, fltSize
      {  0, { 0, 0},      0, IFE2_FLTR_7X7},
      {  0, { 0, 0},      0, IFE2_FLTR_7X7},
      {  0, { 0, 0},      0, IFE2_FLTR_7X7},
      { 16, { 6,10},      0, IFE2_FLTR_7X7},
      { 16, {10,15},      0, IFE2_FLTR_7X7}
};

////////////////////////////////////////////////
// IME
////////////////////////////////////////////////
UINT32 IME_3DNR_ThresY[Total_ISONum][3] =
{
    {2,4,6},
    {2,4,6},
    {2,4,6},
    {2,4,6},
    {2,4,6},

    {2,4,6},
    {2,4,6},
    {2,4,6},
    {2,4,6},
    {2,4,6},

    {2,4,6},

    {2,4,6},
    {2,4,6},
    {2,4,6},
    {2,4,6},
    {2,4,6},


    {2,4,6},
    {2,4,6},
    {2,4,6},
    {2,4,6},
    {2,4,6}
};

UINT32 IME_3DNR_WeightY[Total_ISONum][4] =
{
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},

    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},

    {7,4,2,0},

    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},

    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0}
};

UINT32 IME_3DNR_ThresCC[Total_ISONum][3] =
{
    {4,8,12},
    {4,8,12},
    {4,8,12},
    {4,8,12},
    {4,8,12},

    {4,8,12},
    {4,8,12},
    {4,8,12},
    {4,8,12},
    {4,8,12},

    {4,8,12},

    {4,8,12},
    {4,8,12},
    {4,8,12},
    {4,8,12},
    {4,8,12},

    {4,8,12},
    {4,8,12},
    {4,8,12},
    {4,8,12},
    {4,8,12}
};

UINT32 IME_3DNR_WeightCC[Total_ISONum][4] =
{
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},

    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},

    {7,4,2,0},

    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},

    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0},
    {7,4,2,0}
};


IME_3D_NR IME_3DNR[Total_ISONum] =
{
     { IME_3DNR_ThresY[PRVISO0100], IME_3DNR_WeightY[PRVISO0100], IME_3DNR_ThresCC[PRVISO0100], IME_3DNR_WeightCC[PRVISO0100], IME_3DNR_ThresCC[PRVISO0100], IME_3DNR_WeightCC[PRVISO0100]},
     { IME_3DNR_ThresY[PRVISO0200], IME_3DNR_WeightY[PRVISO0200], IME_3DNR_ThresCC[PRVISO0200], IME_3DNR_WeightCC[PRVISO0200], IME_3DNR_ThresCC[PRVISO0200], IME_3DNR_WeightCC[PRVISO0200]},
     { IME_3DNR_ThresY[PRVISO0400], IME_3DNR_WeightY[PRVISO0400], IME_3DNR_ThresCC[PRVISO0400], IME_3DNR_WeightCC[PRVISO0400], IME_3DNR_ThresCC[PRVISO0400], IME_3DNR_WeightCC[PRVISO0400]},
     { IME_3DNR_ThresY[PRVISO0800], IME_3DNR_WeightY[PRVISO0800], IME_3DNR_ThresCC[PRVISO0800], IME_3DNR_WeightCC[PRVISO0800], IME_3DNR_ThresCC[PRVISO0800], IME_3DNR_WeightCC[PRVISO0800]},
     { IME_3DNR_ThresY[PRVISO1600], IME_3DNR_WeightY[PRVISO1600], IME_3DNR_ThresCC[PRVISO1600], IME_3DNR_WeightCC[PRVISO1600], IME_3DNR_ThresCC[PRVISO1600], IME_3DNR_WeightCC[PRVISO1600]},

     { IME_3DNR_ThresY[VQ2ISO0100], IME_3DNR_WeightY[VQ2ISO0100], IME_3DNR_ThresCC[VQ2ISO0100], IME_3DNR_WeightCC[VQ2ISO0100], IME_3DNR_ThresCC[VQ2ISO0100], IME_3DNR_WeightCC[VQ2ISO0100]},
     { IME_3DNR_ThresY[VQ2ISO0200], IME_3DNR_WeightY[VQ2ISO0200], IME_3DNR_ThresCC[VQ2ISO0200], IME_3DNR_WeightCC[VQ2ISO0200], IME_3DNR_ThresCC[VQ2ISO0200], IME_3DNR_WeightCC[VQ2ISO0200]},
     { IME_3DNR_ThresY[VQ2ISO0400], IME_3DNR_WeightY[VQ2ISO0400], IME_3DNR_ThresCC[VQ2ISO0400], IME_3DNR_WeightCC[VQ2ISO0400], IME_3DNR_ThresCC[VQ2ISO0400], IME_3DNR_WeightCC[VQ2ISO0400]},
     { IME_3DNR_ThresY[VQ2ISO0800], IME_3DNR_WeightY[VQ2ISO0800], IME_3DNR_ThresCC[VQ2ISO0800], IME_3DNR_WeightCC[VQ2ISO0800], IME_3DNR_ThresCC[VQ2ISO0800], IME_3DNR_WeightCC[VQ2ISO0800]},
     { IME_3DNR_ThresY[VQ2ISO1600], IME_3DNR_WeightY[VQ2ISO1600], IME_3DNR_ThresCC[VQ2ISO1600], IME_3DNR_WeightCC[VQ2ISO1600], IME_3DNR_ThresCC[VQ2ISO1600], IME_3DNR_WeightCC[VQ2ISO1600]},

     { IME_3DNR_ThresY[VQ2ISO1600L], IME_3DNR_WeightY[VQ2ISO1600L], IME_3DNR_ThresCC[VQ2ISO1600L], IME_3DNR_WeightCC[VQ2ISO1600L], IME_3DNR_ThresCC[VQ2ISO1600L], IME_3DNR_WeightCC[VQ2ISO1600L]},

    //ThresY[3], WeightY[4], ThresCb[3], WeightCb[4], ThresCr[3], WeightCr[4],
     { IME_3DNR_ThresY[CAPISO0100], IME_3DNR_WeightY[QVISO0100], IME_3DNR_ThresCC[QVISO0100], IME_3DNR_WeightCC[QVISO0100], IME_3DNR_ThresCC[QVISO0100], IME_3DNR_WeightCC[QVISO0100]},
     { IME_3DNR_ThresY[CAPISO0200], IME_3DNR_WeightY[QVISO0200], IME_3DNR_ThresCC[QVISO0200], IME_3DNR_WeightCC[QVISO0200], IME_3DNR_ThresCC[QVISO0200], IME_3DNR_WeightCC[QVISO0200]},
     { IME_3DNR_ThresY[CAPISO0400], IME_3DNR_WeightY[QVISO0400], IME_3DNR_ThresCC[QVISO0400], IME_3DNR_WeightCC[QVISO0400], IME_3DNR_ThresCC[QVISO0400], IME_3DNR_WeightCC[QVISO0400]},
     { IME_3DNR_ThresY[CAPISO0800], IME_3DNR_WeightY[QVISO0800], IME_3DNR_ThresCC[QVISO0800], IME_3DNR_WeightCC[QVISO0800], IME_3DNR_ThresCC[QVISO0800], IME_3DNR_WeightCC[QVISO0800]},
     { IME_3DNR_ThresY[CAPISO1600], IME_3DNR_WeightY[QVISO1600], IME_3DNR_ThresCC[QVISO1600], IME_3DNR_WeightCC[QVISO1600], IME_3DNR_ThresCC[QVISO1600], IME_3DNR_WeightCC[QVISO1600]},

    //ThresY[3], WeightY[4], ThresCb[3], WeightCb[4], ThresCr[3], WeightCr[4],
     { IME_3DNR_ThresY[CAPISO0100], IME_3DNR_WeightY[CAPISO0100], IME_3DNR_ThresCC[CAPISO0100], IME_3DNR_WeightCC[CAPISO0100], IME_3DNR_ThresCC[CAPISO0100], IME_3DNR_WeightCC[CAPISO0100]},
     { IME_3DNR_ThresY[CAPISO0200], IME_3DNR_WeightY[CAPISO0200], IME_3DNR_ThresCC[CAPISO0200], IME_3DNR_WeightCC[CAPISO0200], IME_3DNR_ThresCC[CAPISO0200], IME_3DNR_WeightCC[CAPISO0200]},
     { IME_3DNR_ThresY[CAPISO0400], IME_3DNR_WeightY[CAPISO0400], IME_3DNR_ThresCC[CAPISO0400], IME_3DNR_WeightCC[CAPISO0400], IME_3DNR_ThresCC[CAPISO0400], IME_3DNR_WeightCC[CAPISO0400]},
     { IME_3DNR_ThresY[CAPISO0800], IME_3DNR_WeightY[CAPISO0800], IME_3DNR_ThresCC[CAPISO0800], IME_3DNR_WeightCC[CAPISO0800], IME_3DNR_ThresCC[CAPISO0800], IME_3DNR_WeightCC[CAPISO0800]},
     { IME_3DNR_ThresY[CAPISO1600], IME_3DNR_WeightY[CAPISO1600], IME_3DNR_ThresCC[CAPISO1600], IME_3DNR_WeightCC[CAPISO1600], IME_3DNR_ThresCC[CAPISO1600], IME_3DNR_WeightCC[CAPISO1600]}
};

UINT32 IME_LCATbl[Total_ISONum][33] =
{
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},

    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},

    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},

    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},


    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 127},
};

IME_CHRA_ADAP IME_Chroma_Adapt_param[Total_ISONum] =
{
    {Precision_4,      6,       6,      0,          88, (UINT32)IME_LCATbl[PRVISO0100], 20},
    {Precision_4,      6,       6,      0,         112, (UINT32)IME_LCATbl[PRVISO0200], 20},
    {Precision_4,      6,       6,      0,         134, (UINT32)IME_LCATbl[PRVISO0400], 20},
    {Precision_4,      6,       6,      0,         160, (UINT32)IME_LCATbl[PRVISO0800], 20},
    {Precision_4,      6,       6,      0,         184, (UINT32)IME_LCATbl[PRVISO1600], 20},

    {Precision_4,      6,       6,      0,          88, (UINT32)IME_LCATbl[VQ2ISO0100], 20},
    {Precision_4,      6,       6,      0,          88, (UINT32)IME_LCATbl[VQ2ISO0100], 20},
    {Precision_4,      6,       6,      0,         112, (UINT32)IME_LCATbl[VQ2ISO0200], 20},
    {Precision_4,      6,       6,      0,         134, (UINT32)IME_LCATbl[VQ2ISO0400], 20},
    {Precision_4,      6,       6,      0,         184, (UINT32)IME_LCATbl[VQ2ISO1600], 20},

    {Precision_4,      6,       6,      0,         184, (UINT32)IME_LCATbl[VQ2ISO1600L], 20},

    //LcaLutPre, Thres_Cb, Thres_Cr, Thres_Lut, LcaWeight, LcaLUT_TblAddr,      SubRatio
    {Precision_4,    255,      255,      0,         160, (UINT32)IME_LCATbl[QVISO0100], 20},
    {Precision_4,    255,      255,      0,         192, (UINT32)IME_LCATbl[QVISO0200], 20},
    {Precision_4,    255,      255,      0,         232, (UINT32)IME_LCATbl[QVISO0400], 20},
    {Precision_4,    255,      255,      0,         232, (UINT32)IME_LCATbl[QVISO0800], 20},
    {Precision_4,    255,      255,      0,         255, (UINT32)IME_LCATbl[QVISO1600], 20},


    //LcaLutPre, Thres_Cb, Thres_Cr, Thres_Lut, LcaWeight, LcaLUT_TblAddr,      SubRatio
    {Precision_4,    255,      255,      0,         160, (UINT32)IME_LCATbl[QVISO0100], 20},
    {Precision_4,    255,      255,      0,         192, (UINT32)IME_LCATbl[QVISO0200], 20},
    {Precision_4,    255,      255,      0,         232, (UINT32)IME_LCATbl[QVISO0400], 20},
    {Precision_4,    255,      255,      0,         232, (UINT32)IME_LCATbl[QVISO0800], 20},
    {Precision_4,    255,      255,      0,         255, (UINT32)IME_LCATbl[QVISO1600], 20}
};

UINT32 IME_ChromaFilt_rngth_cb[Total_ISONum][4] =
{
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},

    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 2, 4, 6, 8},

    { 2, 4, 6, 8},

    { 2, 4, 6, 8},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},
    { 4, 8,12,16},

    { 2, 4, 6, 8},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},
    { 4, 8,12,16}
};
UINT32 IME_ChromaFilt_rngth_cr[Total_ISONum][4] =
{
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},

    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 2, 4, 6, 8},

    { 2, 4, 6, 8},

    { 2, 4, 6, 8},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},
    { 4, 8,12,16},

    { 2, 4, 6, 8},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},
    { 4, 8,12,16}
};
UINT32 IME_ChromaFilt_rngWt[Total_ISONum][5] =
{
    { 7, 5, 3, 1, 0},
    { 7, 5, 3, 1, 0},
    { 7, 5, 3, 1, 0},
    { 7, 5, 3, 1, 0},
    { 7, 5, 3, 1, 0},

    { 7, 5, 3, 1, 0},
    { 7, 5, 3, 1, 0},
    { 7, 5, 3, 1, 0},
    { 7, 5, 3, 1, 0},
    { 7, 5, 3, 1, 0},

    { 7, 5, 3, 1, 0},

    { 7, 5, 3, 1, 0},
    { 7, 3, 2, 1, 0},
    { 7, 3, 2, 1, 0},
    { 7, 3, 2, 1, 0},
    { 7, 5, 3, 1, 0},

    { 7, 5, 3, 1, 0},
    { 7, 3, 2, 1, 0},
    { 7, 3, 2, 1, 0},
    { 7, 3, 2, 1, 0},
    { 7, 5, 3, 1, 0},
};

IME_CHRA_FILT IME_Chroma_Filter_param[Total_ISONum] =
{
    //RangeFiltEn, RangeFilt, SpaceFilt
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0100], IME_ChromaFilt_rngth_cr[PRVISO0100], IME_ChromaFilt_rngWt[PRVISO0100]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0200], IME_ChromaFilt_rngth_cr[PRVISO0200], IME_ChromaFilt_rngWt[PRVISO0200]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0400], IME_ChromaFilt_rngth_cr[PRVISO0400], IME_ChromaFilt_rngWt[PRVISO0400]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0800], IME_ChromaFilt_rngth_cr[PRVISO0800], IME_ChromaFilt_rngWt[PRVISO0800]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO1600], IME_ChromaFilt_rngth_cr[PRVISO1600], IME_ChromaFilt_rngWt[PRVISO1600]}, {2, 1, 32}},

    //RangeFiltEn, RangeFilt, SpaceFilt
    {TRUE,         {IME_ChromaFilt_rngth_cb[VQ2ISO0100], IME_ChromaFilt_rngth_cr[VQ2ISO0100], IME_ChromaFilt_rngWt[VQ2ISO0100]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[VQ2ISO0200], IME_ChromaFilt_rngth_cr[VQ2ISO0200], IME_ChromaFilt_rngWt[VQ2ISO0200]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[VQ2ISO0400], IME_ChromaFilt_rngth_cr[VQ2ISO0400], IME_ChromaFilt_rngWt[VQ2ISO0400]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[VQ2ISO0800], IME_ChromaFilt_rngth_cr[VQ2ISO0800], IME_ChromaFilt_rngWt[VQ2ISO0800]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[VQ2ISO1600], IME_ChromaFilt_rngth_cr[VQ2ISO1600], IME_ChromaFilt_rngWt[VQ2ISO1600]}, {2, 1, 32}},

    {TRUE,         {IME_ChromaFilt_rngth_cb[VQ2ISO1600L], IME_ChromaFilt_rngth_cr[VQ2ISO1600L], IME_ChromaFilt_rngWt[VQ2ISO1600L]}, {2, 1, 32}},

    //RangeFiltEn, RangeFilt, SpaceFilt
    {TRUE,         {IME_ChromaFilt_rngth_cb[QVISO0100], IME_ChromaFilt_rngth_cr[QVISO0100], IME_ChromaFilt_rngWt[QVISO0100]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[QVISO0200], IME_ChromaFilt_rngth_cr[QVISO0200], IME_ChromaFilt_rngWt[QVISO0200]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[QVISO0400], IME_ChromaFilt_rngth_cr[QVISO0400], IME_ChromaFilt_rngWt[QVISO0400]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[QVISO0800], IME_ChromaFilt_rngth_cr[QVISO0800], IME_ChromaFilt_rngWt[QVISO0800]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[QVISO1600], IME_ChromaFilt_rngth_cr[QVISO1600], IME_ChromaFilt_rngWt[QVISO1600]}, {2, 1, 32}},

    //RangeFiltEn, RangeFilt, SpaceFilt
    {TRUE,         {IME_ChromaFilt_rngth_cb[CAPISO0100], IME_ChromaFilt_rngth_cr[CAPISO0100], IME_ChromaFilt_rngWt[CAPISO0100]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[CAPISO0200], IME_ChromaFilt_rngth_cr[CAPISO0200], IME_ChromaFilt_rngWt[CAPISO0200]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[CAPISO0400], IME_ChromaFilt_rngth_cr[CAPISO0400], IME_ChromaFilt_rngWt[CAPISO0400]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[CAPISO0800], IME_ChromaFilt_rngth_cr[CAPISO0800], IME_ChromaFilt_rngWt[CAPISO0800]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[CAPISO1600], IME_ChromaFilt_rngth_cr[CAPISO1600], IME_ChromaFilt_rngWt[CAPISO1600]}, {2, 1, 32}},
};

IME_RETINEX IME_mfr[Total_ISONum] =
{
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},

  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},

  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},

  //GL_GAIN,GD_GAIN,LL_GAIN,LD_GAIN,R_GAIN,G_GAIN,B_GAIN,ExGain, SubRatio;
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},
  {    1024, 1024,  564,  564, 1024, 1024, 1024, 0, 50},
  {    1024, 1024,  563,  563, 1024, 1024, 1024, 0, 50},

  //GL_GAIN,GD_GAIN,LL_GAIN,LD_GAIN,R_GAIN,G_GAIN,B_GAIN,ExGain, SubRatio;
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},
  {    1024, 1024,  564,  564, 1024, 1024, 1024, 0, 50},
  {    1024, 1024,  563,  563, 1024, 1024, 1024, 0, 50}
};

////////////////////////////////////////////////
// 3D CC
////////////////////////////////////////////////
UINT32 IPE_3DCCLut[900] =
{
0x00000000, 0x00000040, 0x00000080, 0x000000c0, 0x000000ff, 0x00004000, 0x00004040, 0x00004080,
0x000040c0, 0x000040ff, 0x00008000, 0x00008040, 0x00008080, 0x000080c0, 0x000080ff, 0x0000c000,
0x0000c040, 0x0000c080, 0x0000c0c0, 0x0000c0ff, 0x0000ff00, 0x0000ff40, 0x0000ff80, 0x0000ffc0,
0x0000ffff, 0x00400000, 0x00400040, 0x00400080, 0x004000c0, 0x004000ff, 0x00404000, 0x00404040,
0x00404080, 0x004040c0, 0x004040ff, 0x00408000, 0x00408040, 0x00408080, 0x004080c0, 0x004080ff,
0x0040c000, 0x0040c040, 0x0040c080, 0x0040c0c0, 0x0040c0ff, 0x0040ff00, 0x0040ff40, 0x0040ff80,
0x0040ffc0, 0x0040ffff, 0x00800000, 0x00800040, 0x00800080, 0x008000c0, 0x008000ff, 0x00804000,
0x00804040, 0x00804080, 0x008040c0, 0x008040ff, 0x00808000, 0x00808040, 0x00808080, 0x008080c0,
0x008080ff, 0x0080c000, 0x0080c040, 0x0080c080, 0x0080c0c0, 0x0080c0ff, 0x0080ff00, 0x0080ff40,
0x0080ff80, 0x0080ffc0, 0x0080ffff, 0x00c00000, 0x00c00040, 0x00c00080, 0x00c000c0, 0x00c000ff,
0x00c04000, 0x00c04040, 0x00c04080, 0x00c040c0, 0x00c040ff, 0x00c08000, 0x00c08040, 0x00c08080,
0x00c080c0, 0x00c080ff, 0x00c0c000, 0x00c0c040, 0x00c0c080, 0x00c0c0c0, 0x00c0c0ff, 0x00c0ff00,
0x00c0ff40, 0x00c0ff80, 0x00c0ffc0, 0x00c0ffff, 0x00ff0000, 0x00ff0040, 0x00ff0080, 0x00ff00c0,
0x00ff00ff, 0x00ff4000, 0x00ff4040, 0x00ff4080, 0x00ff40c0, 0x00ff40ff, 0x00ff8000, 0x00ff8040,
0x00ff8080, 0x00ff80c0, 0x00ff80ff, 0x00ffc000, 0x00ffc040, 0x00ffc080, 0x00ffc0c0, 0x00ffc0ff,
0x00ffff00, 0x00ffff40, 0x00ffff80, 0x00ffffc0, 0x00ffffff, 0x00000020, 0x00000060, 0x000000a0,
0x000000e0, 0x00000000, 0x00004020, 0x00004060, 0x000040a0, 0x000040e0, 0x00000000, 0x00008020,
0x00008060, 0x000080a0, 0x000080e0, 0x00000000, 0x0000c020, 0x0000c060, 0x0000c0a0, 0x0000c0e0,
0x00000000, 0x0000ff20, 0x0000ff60, 0x0000ffa0, 0x0000ffe0, 0x00000000, 0x00400020, 0x00400060,
0x004000a0, 0x004000e0, 0x00000000, 0x00404020, 0x00404060, 0x004040a0, 0x004040e0, 0x00000000,
0x00408020, 0x00408060, 0x004080a0, 0x004080e0, 0x00000000, 0x0040c020, 0x0040c060, 0x0040c0a0,
0x0040c0e0, 0x00000000, 0x0040ff20, 0x0040ff60, 0x0040ffa0, 0x0040ffe0, 0x00000000, 0x00800020,
0x00800060, 0x008000a0, 0x008000e0, 0x00000000, 0x00804020, 0x00804060, 0x008040a0, 0x008040e0,
0x00000000, 0x00808020, 0x00808060, 0x008080a0, 0x008080e0, 0x00000000, 0x0080c020, 0x0080c060,
0x0080c0a0, 0x0080c0e0, 0x00000000, 0x0080ff20, 0x0080ff60, 0x0080ffa0, 0x0080ffe0, 0x00000000,
0x00c00020, 0x00c00060, 0x00c000a0, 0x00c000e0, 0x00000000, 0x00c04020, 0x00c04060, 0x00c040a0,
0x00c040e0, 0x00000000, 0x00c08020, 0x00c08060, 0x00c080a0, 0x00c080e0, 0x00000000, 0x00c0c020,
0x00c0c060, 0x00c0c0a0, 0x00c0c0e0, 0x00000000, 0x00c0ff20, 0x00c0ff60, 0x00c0ffa0, 0x00c0ffe0,
0x00000000, 0x00ff0020, 0x00ff0060, 0x00ff00a0, 0x00ff00e0, 0x00000000, 0x00ff4020, 0x00ff4060,
0x00ff40a0, 0x00ff40e0, 0x00000000, 0x00ff8020, 0x00ff8060, 0x00ff80a0, 0x00ff80e0, 0x00000000,
0x00ffc020, 0x00ffc060, 0x00ffc0a0, 0x00ffc0e0, 0x00000000, 0x00ffff20, 0x00ffff60, 0x00ffffa0,
0x00ffffe0, 0x00000000, 0x00002000, 0x00002040, 0x00002080, 0x000020c0, 0x000020ff, 0x00006000,
0x00006040, 0x00006080, 0x000060c0, 0x000060ff, 0x0000a000, 0x0000a040, 0x0000a080, 0x0000a0c0,
0x0000a0ff, 0x0000e000, 0x0000e040, 0x0000e080, 0x0000e0c0, 0x0000e0ff, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00402000, 0x00402040, 0x00402080, 0x004020c0, 0x004020ff,
0x00406000, 0x00406040, 0x00406080, 0x004060c0, 0x004060ff, 0x0040a000, 0x0040a040, 0x0040a080,
0x0040a0c0, 0x0040a0ff, 0x0040e000, 0x0040e040, 0x0040e080, 0x0040e0c0, 0x0040e0ff, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00802000, 0x00802040, 0x00802080, 0x008020c0,
0x008020ff, 0x00806000, 0x00806040, 0x00806080, 0x008060c0, 0x008060ff, 0x0080a000, 0x0080a040,
0x0080a080, 0x0080a0c0, 0x0080a0ff, 0x0080e000, 0x0080e040, 0x0080e080, 0x0080e0c0, 0x0080e0ff,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00c02000, 0x00c02040, 0x00c02080,
0x00c020c0, 0x00c020ff, 0x00c06000, 0x00c06040, 0x00c06080, 0x00c060c0, 0x00c060ff, 0x00c0a000,
0x00c0a040, 0x00c0a080, 0x00c0a0c0, 0x00c0a0ff, 0x00c0e000, 0x00c0e040, 0x00c0e080, 0x00c0e0c0,
0x00c0e0ff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ff2000, 0x00ff2040,
0x00ff2080, 0x00ff20c0, 0x00ff20ff, 0x00ff6000, 0x00ff6040, 0x00ff6080, 0x00ff60c0, 0x00ff60ff,
0x00ffa000, 0x00ffa040, 0x00ffa080, 0x00ffa0c0, 0x00ffa0ff, 0x00ffe000, 0x00ffe040, 0x00ffe080,
0x00ffe0c0, 0x00ffe0ff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00002020,
0x00002060, 0x000020a0, 0x000020e0, 0x00000000, 0x00006020, 0x00006060, 0x000060a0, 0x000060e0,
0x00000000, 0x0000a020, 0x0000a060, 0x0000a0a0, 0x0000a0e0, 0x00000000, 0x0000e020, 0x0000e060,
0x0000e0a0, 0x0000e0e0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00402020, 0x00402060, 0x004020a0, 0x004020e0, 0x00000000, 0x00406020, 0x00406060, 0x004060a0,
0x004060e0, 0x00000000, 0x0040a020, 0x0040a060, 0x0040a0a0, 0x0040a0e0, 0x00000000, 0x0040e020,
0x0040e060, 0x0040e0a0, 0x0040e0e0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00802020, 0x00802060, 0x008020a0, 0x008020e0, 0x00000000, 0x00806020, 0x00806060,
0x008060a0, 0x008060e0, 0x00000000, 0x0080a020, 0x0080a060, 0x0080a0a0, 0x0080a0e0, 0x00000000,
0x0080e020, 0x0080e060, 0x0080e0a0, 0x0080e0e0, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00c02020, 0x00c02060, 0x00c020a0, 0x00c020e0, 0x00000000, 0x00c06020,
0x00c06060, 0x00c060a0, 0x00c060e0, 0x00000000, 0x00c0a020, 0x00c0a060, 0x00c0a0a0, 0x00c0a0e0,
0x00000000, 0x00c0e020, 0x00c0e060, 0x00c0e0a0, 0x00c0e0e0, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00ff2020, 0x00ff2060, 0x00ff20a0, 0x00ff20e0, 0x00000000,
0x00ff6020, 0x00ff6060, 0x00ff60a0, 0x00ff60e0, 0x00000000, 0x00ffa020, 0x00ffa060, 0x00ffa0a0,
0x00ffa0e0, 0x00000000, 0x00ffe020, 0x00ffe060, 0x00ffe0a0, 0x00ffe0e0, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00200000, 0x00200040, 0x00200080, 0x002000c0,
0x002000ff, 0x00204000, 0x00204040, 0x00204080, 0x002040c0, 0x002040ff, 0x00208000, 0x00208040,
0x00208080, 0x002080c0, 0x002080ff, 0x0020c000, 0x0020c040, 0x0020c080, 0x0020c0c0, 0x0020c0ff,
0x0020ff00, 0x0020ff40, 0x0020ff80, 0x0020ffc0, 0x0020ffff, 0x00600000, 0x00600040, 0x00600080,
0x006000c0, 0x006000ff, 0x00604000, 0x00604040, 0x00604080, 0x006040c0, 0x006040ff, 0x00608000,
0x00608040, 0x00608080, 0x006080c0, 0x006080ff, 0x0060c000, 0x0060c040, 0x0060c080, 0x0060c0c0,
0x0060c0ff, 0x0060ff00, 0x0060ff40, 0x0060ff80, 0x0060ffc0, 0x0060ffff, 0x00a00000, 0x00a00040,
0x00a00080, 0x00a000c0, 0x00a000ff, 0x00a04000, 0x00a04040, 0x00a04080, 0x00a040c0, 0x00a040ff,
0x00a08000, 0x00a08040, 0x00a08080, 0x00a080c0, 0x00a080ff, 0x00a0c000, 0x00a0c040, 0x00a0c080,
0x00a0c0c0, 0x00a0c0ff, 0x00a0ff00, 0x00a0ff40, 0x00a0ff80, 0x00a0ffc0, 0x00a0ffff, 0x00e00000,
0x00e00040, 0x00e00080, 0x00e000c0, 0x00e000ff, 0x00e04000, 0x00e04040, 0x00e04080, 0x00e040c0,
0x00e040ff, 0x00e08000, 0x00e08040, 0x00e08080, 0x00e080c0, 0x00e080ff, 0x00e0c000, 0x00e0c040,
0x00e0c080, 0x00e0c0c0, 0x00e0c0ff, 0x00e0ff00, 0x00e0ff40, 0x00e0ff80, 0x00e0ffc0, 0x00e0ffff,
0x00200020, 0x00200060, 0x002000a0, 0x002000e0, 0x00000000, 0x00204020, 0x00204060, 0x002040a0,
0x002040e0, 0x00000000, 0x00208020, 0x00208060, 0x002080a0, 0x002080e0, 0x00000000, 0x0020c020,
0x0020c060, 0x0020c0a0, 0x0020c0e0, 0x00000000, 0x0020ff20, 0x0020ff60, 0x0020ffa0, 0x0020ffe0,
0x00000000, 0x00600020, 0x00600060, 0x006000a0, 0x006000e0, 0x00000000, 0x00604020, 0x00604060,
0x006040a0, 0x006040e0, 0x00000000, 0x00608020, 0x00608060, 0x006080a0, 0x006080e0, 0x00000000,
0x0060c020, 0x0060c060, 0x0060c0a0, 0x0060c0e0, 0x00000000, 0x0060ff20, 0x0060ff60, 0x0060ffa0,
0x0060ffe0, 0x00000000, 0x00a00020, 0x00a00060, 0x00a000a0, 0x00a000e0, 0x00000000, 0x00a04020,
0x00a04060, 0x00a040a0, 0x00a040e0, 0x00000000, 0x00a08020, 0x00a08060, 0x00a080a0, 0x00a080e0,
0x00000000, 0x00a0c020, 0x00a0c060, 0x00a0c0a0, 0x00a0c0e0, 0x00000000, 0x00a0ff20, 0x00a0ff60,
0x00a0ffa0, 0x00a0ffe0, 0x00000000, 0x00e00020, 0x00e00060, 0x00e000a0, 0x00e000e0, 0x00000000,
0x00e04020, 0x00e04060, 0x00e040a0, 0x00e040e0, 0x00000000, 0x00e08020, 0x00e08060, 0x00e080a0,
0x00e080e0, 0x00000000, 0x00e0c020, 0x00e0c060, 0x00e0c0a0, 0x00e0c0e0, 0x00000000, 0x00e0ff20,
0x00e0ff60, 0x00e0ffa0, 0x00e0ffe0, 0x00000000, 0x00202000, 0x00202040, 0x00202080, 0x002020c0,
0x002020ff, 0x00206000, 0x00206040, 0x00206080, 0x002060c0, 0x002060ff, 0x0020a000, 0x0020a040,
0x0020a080, 0x0020a0c0, 0x0020a0ff, 0x0020e000, 0x0020e040, 0x0020e080, 0x0020e0c0, 0x0020e0ff,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00602000, 0x00602040, 0x00602080,
0x006020c0, 0x006020ff, 0x00606000, 0x00606040, 0x00606080, 0x006060c0, 0x006060ff, 0x0060a000,
0x0060a040, 0x0060a080, 0x0060a0c0, 0x0060a0ff, 0x0060e000, 0x0060e040, 0x0060e080, 0x0060e0c0,
0x0060e0ff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00a02000, 0x00a02040,
0x00a02080, 0x00a020c0, 0x00a020ff, 0x00a06000, 0x00a06040, 0x00a06080, 0x00a060c0, 0x00a060ff,
0x00a0a000, 0x00a0a040, 0x00a0a080, 0x00a0a0c0, 0x00a0a0ff, 0x00a0e000, 0x00a0e040, 0x00a0e080,
0x00a0e0c0, 0x00a0e0ff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00e02000,
0x00e02040, 0x00e02080, 0x00e020c0, 0x00e020ff, 0x00e06000, 0x00e06040, 0x00e06080, 0x00e060c0,
0x00e060ff, 0x00e0a000, 0x00e0a040, 0x00e0a080, 0x00e0a0c0, 0x00e0a0ff, 0x00e0e000, 0x00e0e040,
0x00e0e080, 0x00e0e0c0, 0x00e0e0ff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00202020, 0x00202060, 0x002020a0, 0x002020e0, 0x00000000, 0x00206020, 0x00206060, 0x002060a0,
0x002060e0, 0x00000000, 0x0020a020, 0x0020a060, 0x0020a0a0, 0x0020a0e0, 0x00000000, 0x0020e020,
0x0020e060, 0x0020e0a0, 0x0020e0e0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00602020, 0x00602060, 0x006020a0, 0x006020e0, 0x00000000, 0x00606020, 0x00606060,
0x006060a0, 0x006060e0, 0x00000000, 0x0060a020, 0x0060a060, 0x0060a0a0, 0x0060a0e0, 0x00000000,
0x0060e020, 0x0060e060, 0x0060e0a0, 0x0060e0e0, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00a02020, 0x00a02060, 0x00a020a0, 0x00a020e0, 0x00000000, 0x00a06020,
0x00a06060, 0x00a060a0, 0x00a060e0, 0x00000000, 0x00a0a020, 0x00a0a060, 0x00a0a0a0, 0x00a0a0e0,
0x00000000, 0x00a0e020, 0x00a0e060, 0x00a0e0a0, 0x00a0e0e0, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00e02020, 0x00e02060, 0x00e020a0, 0x00e020e0, 0x00000000,
0x00e06020, 0x00e06060, 0x00e060a0, 0x00e060e0, 0x00000000, 0x00e0a020, 0x00e0a060, 0x00e0a0a0,
0x00e0a0e0, 0x00000000, 0x00e0e020, 0x00e0e060, 0x00e0e0a0, 0x00e0e0e0, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000
};

UINT16 RawEncLut[129] =
{
       0,  128,  256,  384,  512,  640,  768,  896, 1024, 1086, 1144, 1200, 1254, 1305, 1354, 1402,
    1448, 1492, 1536, 1578, 1619, 1659, 1698, 1736, 1773, 1810, 1846, 1881, 1915, 1949, 1982, 2015,
    2048, 2079, 2111, 2141, 2172, 2202, 2231, 2260, 2289, 2318, 2346, 2374, 2401, 2428, 2455, 2482,
    2508, 2534, 2560, 2585, 2610, 2635, 2660, 2684, 2709, 2733, 2757, 2780, 2804, 2827, 2850, 2873,
    2896, 2918, 2941, 2963, 2985, 3007, 3029, 3050, 3072, 3093, 3114, 3135, 3156, 3176, 3197, 3217,
    3238, 3258, 3278, 3298, 3318, 3337, 3357, 3376, 3396, 3415, 3434, 3453, 3472, 3491, 3510, 3528,
    3547, 3565, 3584, 3602, 3620, 3638, 3656, 3674, 3692, 3709, 3727, 3744, 3762, 3779, 3797, 3814,
    3831, 3848, 3865, 3882, 3899, 3916, 3932, 3949, 3965, 3982, 3998, 4015, 4031, 4047, 4063, 4079,
    4095
};

SIE_GAMMA SieRawEncTab = {(UINT32)&RawEncLut, (UINT32)&RawEncLut, (UINT32)&RawEncLut, (UINT32)&RawEncLut};


UINT32 RawDecLut[129] =
{
      0,     8,   16,   24,   32,   40,   48,   56,   64,   72,   80,   88,   96,  104,  112,  120,
     128,  136,  144,  152,  160,  168,  176,  184,  192,  200,  208,  216,  224,  232,  240,  248,
     256,  272,  289,  306,  324,  342,  361,  380,  400,  420,  441,  462,  484,  506,  529,  552,
     576,  600,  625,  650,  676,  702,  729,  756,  784,  812,  841,  870,  900,  930,  961,  992,
    1024, 1056, 1089, 1122, 1156, 1190, 1225, 1260, 1296, 1332, 1369, 1406, 1444, 1482, 1521, 1560,
    1600, 1640, 1681, 1722, 1764, 1806, 1849, 1892, 1936, 1980, 2025, 2070, 2116, 2162, 2209, 2256,
    2304, 2352, 2401, 2450, 2500, 2550, 2601, 2652, 2704, 2756, 2809, 2862, 2916, 2970, 3025, 3080,
    3136, 3192, 3249, 3306, 3364, 3422, 3481, 3540, 3600, 3660, 3721, 3782, 3844, 3906, 3969, 4032,
    4095
};

#endif