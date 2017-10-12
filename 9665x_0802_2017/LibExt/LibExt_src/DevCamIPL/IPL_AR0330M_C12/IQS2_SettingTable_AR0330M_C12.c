#ifndef _IQS2_SETTINGTABLE_AR0330M_C12_C
#define _IQS2_SETTINGTABLE_AR0330M_C12_C
#include "IPL_AR0330M_C12_Int.h"
#if (IQS_VERSION == IQS_VERSION_2)

////////////////////////////////////////////////
//PRE
////////////////////////////////////////////////
UINT8 PRE_ShadingLut[Total_SENMODENum][Total_ZOOMNum][17] =
{
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0, 16, 32, 64,128,255,255,255,255,255,255,255,255,255,255,255,255},
    }
};



IQS_PRE_CENTER PRE_ShadingCenter[Total_SENMODENum] =
{
    {{640, 480},{640, 480},{640, 480},{640, 480}}
};

IQS_PRE_VIGLUT_PARAM PRE_ShadingParam[Total_SENMODENum] =
{
    //DCExtEn, DCExtTh1, DCExtTh2, Mul,      T,   TabGain,            XDIV, YDIV,  DitherEn, DitherRstEn
   {  DISABLE, 0,        0,        0x40,     0,   VIG_TAB_GAIN_2X,    1024,  1024, FALSE,    FALSE }
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
    {ENABLE},
    {ENABLE},

    {ENABLE},
    {ENABLE},
    {ENABLE},
    {ENABLE},
    {ENABLE},

    {ENABLE},

    {ENABLE},
    {ENABLE},
    {ENABLE},
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
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},

    {31, 23, 17, 9, 7, 3},

    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
    {31, 23, 17, 9, 7, 3},
};


UINT32 IFE_rngth_NLM[Total_ISONum][4] =
{
    {0,1,2,2},
    {0,1,2,2},
    {1,2,3,4},
    {2,4,6,8}, //{1,2,3,4},
    {3,5,7,9},
    {4,8,12,15},
    {6,12,18,24},

    {1,1,2,3},
    {1,1,2,3},
    {2,3,4,5}, //{1,2,3,4},
    {2,3,5,7},
    {2,5,7,10},

    {2,5,7,10},

    {1,1,2,3},
    {1,3,4,5},
    {2,3,5,7},
    {2,5,7,10},
    {3,6,9,12},
};

UINT32 IFE_rngth_Bil[Total_ISONum][4] =
{
    { 1, 2, 3, 4},
    { 1, 2, 3, 4},
    { 2, 3, 3, 4},//{ 3, 3, 4, 5},
    { 3, 4, 5, 6},//{ 5, 7, 7, 9},
    { 4, 8,12, 14},//{ 5, 9,15, 20},{ 7,10,12,14},
    { 7,16,23,31},
    { 11,25,36,48},


    { 1, 2, 4, 5},
    { 3, 4, 5, 6},
    { 4, 6, 8,10}, //{ 5, 7, 9,11},//{ 6, 8,10,12},
    { 6, 8,10,12}, //{ 7, 9,11,13},
    { 8,10,12,14},

    { 8,10,12,14},

    {2,2,4,6},
    {2,6,8,10},
    {4,6,10,14},
    {4,10,14,20},
    {6,12,18,24},
};

IFE_FILT IFE_Filter_param[Total_ISONum] =
{
    //Mode,    {sOnlyEn, sOnlyLen, weights},                  {weightr_NLM,                     weightr_Bil    ,              bilat_w},  clamp{Th,  Mul, Dlt}, Rth_w, Bin}
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0125]}, {{IFE_rngth_NLM[PRVISO0125], 10}, {IFE_rngth_Bil[PRVISO0125], 24},  1},  { 2, 80,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0200]}, {{IFE_rngth_NLM[PRVISO0200], 10}, {IFE_rngth_Bil[PRVISO0200], 24},  1},  { 2, 80,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0400]}, {{IFE_rngth_NLM[PRVISO0400], 24}, {IFE_rngth_Bil[PRVISO0400], 28},  1},  { 0, 90,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0800]}, {{IFE_rngth_NLM[PRVISO0800], 25}, {IFE_rngth_Bil[PRVISO0800], 30},  1},  { 1, 80,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO1600]}, {{IFE_rngth_NLM[PRVISO1600], 28}, {IFE_rngth_Bil[PRVISO1600], 30},  2},  { 4, 64,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO3200]}, {{IFE_rngth_NLM[PRVISO3200], 35}, {IFE_rngth_Bil[PRVISO3200], 64},  3},  { 0, 128,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO6400]}, {{IFE_rngth_NLM[PRVISO6400], 35}, {IFE_rngth_Bil[PRVISO6400], 64},  3},  { 0, 128,   0},   0,      0},

    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0100B2]}, {{IFE_rngth_NLM[PRVISO0100B2], 11}, {IFE_rngth_Bil[PRVISO0100B2], 50},  1},  { 0, 128,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0200B2]}, {{IFE_rngth_NLM[PRVISO0200B2], 11}, {IFE_rngth_Bil[PRVISO0200B2], 50},  1},  { 0, 128,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0400B2]}, {{IFE_rngth_NLM[PRVISO0400B2], 22}, {IFE_rngth_Bil[PRVISO0400B2], 50},  1},  { 2, 64,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0800B2]}, {{IFE_rngth_NLM[PRVISO0800B2], 22}, {IFE_rngth_Bil[PRVISO0800B2], 50},  1},  { 16, 64,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO1600B2]}, {{IFE_rngth_NLM[PRVISO1600B2], 25}, {IFE_rngth_Bil[PRVISO1600B2], 50},  1},  { 12, 40,   0},   0,      0},

    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVLOWLIGHT]}, {{IFE_rngth_NLM[PRVLOWLIGHT], 29}, {IFE_rngth_Bil[PRVLOWLIGHT],50}, 1},  { 0, 64,   0},  0,      0},

    //Mode,    {sOnlyEn, sOnlyLen, weights},                  {weightr_NLM,                     weightr_Bil    ,              bilat_w},  clamp{Th,  Mul, Dlt}, Rth_w, Bin}
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[CAPISO0100]}, {{IFE_rngth_NLM[CAPISO0100], 11}, {IFE_rngth_Bil[CAPISO0100],  50}, 1},  { 0, 0x80,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[CAPISO0200]}, {{IFE_rngth_NLM[CAPISO0200], 16}, {IFE_rngth_Bil[CAPISO0200],  60}, 3},  { 0, 0x80,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[CAPISO0400]}, {{IFE_rngth_NLM[CAPISO0400], 20}, {IFE_rngth_Bil[CAPISO0400],  75}, 7},  { 3, 0x80,   0},   2,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[CAPISO0800]}, {{IFE_rngth_NLM[CAPISO0800], 25}, {IFE_rngth_Bil[CAPISO0800],  90},11},  { 4, 0x80,   0},   5,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[CAPISO1600]}, {{IFE_rngth_NLM[CAPISO1600], 30}, {IFE_rngth_Bil[CAPISO1600], 110},15},  { 5, 0x80,   0},  10,      0}

};

UINT32 IFE_Outl_Bri[Total_ISONum][5] =
{
    // 8 ~ 16
    { 600, 876, 1446, 1998, 2436},
    { 400, 584, 964, 1332, 1624},
    { 200, 338, 523, 899, 1218},
    {  80, 219, 362, 700, 809},
    {  32,  64, 96, 320, 540},
    {  32,  32,  64,  64,  64},
    {  32,  32,  64,  64,  64},

    {  64,  64,  64,  64,  64},
    {  64,  64,  64,  64,  64},
    {  64,  64,  64,  64,  64},
    {  64,  64,  64,  64,  64},
    {  64,  64,  64,  64,  64},

    {  64,  64,  64,  64,  64},

    { 600, 876, 1446, 1998, 2436},
    { 400, 584, 964, 1332, 1624},
    { 300, 438, 723, 999, 1218},
    {  80, 219, 362, 500, 609},
    {  32,  64, 96, 120, 140},

};

UINT32 IFE_Outl_Dark[Total_ISONum][5] =
{
    { 1024, 1514, 2669, 3297, 3854},
    { 600, 876, 1446, 1998, 2436},
    { 450, 657, 960, 1499, 1827},
    { 225, 329, 512, 1009, 1827},
    { 128, 160, 512,  1009,  1827},
    {  64,  64,  64,  64,  64},
    {  64,  64,  64,  64,  64},

    {  64,  64,  64,  64,  64},
    {  64,  64,  64,  64,  64},
    {  64,  64,  64,  64,  64},
    {  64,  64,  64,  64,  64},
    {  64,  64,  64,  64,  64},

    {  64,  64,  64,  64,  64},

    { 1024, 1514, 2669, 3297, 3854},
    { 600, 876, 1446, 1998, 2436},
    { 450, 657, 1085, 1499, 1827},
    { 225, 329, 542, 1009, 1827},
    { 128, 160, 1085,  1998,  2436},
};

IFE_OUTL IFE_Outl_param[Total_ISONum] =
{
    {IFE_Outl_Bri[PRVISO0125], IFE_Outl_Dark[PRVISO0125], _8_NB},
    {IFE_Outl_Bri[PRVISO0200], IFE_Outl_Dark[PRVISO0200], _8_NB},
    {IFE_Outl_Bri[PRVISO0400], IFE_Outl_Dark[PRVISO0400], _8_NB},
    {IFE_Outl_Bri[PRVISO0800], IFE_Outl_Dark[PRVISO0800], _8_NB},
    {IFE_Outl_Bri[PRVISO1600], IFE_Outl_Dark[PRVISO1600], _7_NB},
    {IFE_Outl_Bri[PRVISO3200], IFE_Outl_Dark[PRVISO3200], _7_NB},
    {IFE_Outl_Bri[PRVISO6400], IFE_Outl_Dark[PRVISO6400], _7_NB},

    {IFE_Outl_Bri[PRVISO0100B2], IFE_Outl_Dark[PRVISO0100B2], _8_NB},
    {IFE_Outl_Bri[PRVISO0200B2], IFE_Outl_Dark[PRVISO0200B2], _8_NB},
    {IFE_Outl_Bri[PRVISO0400B2], IFE_Outl_Dark[PRVISO0400B2], _8_NB},
    {IFE_Outl_Bri[PRVISO0800B2], IFE_Outl_Dark[PRVISO0800B2], _8_NB},
    {IFE_Outl_Bri[PRVISO1600B2], IFE_Outl_Dark[PRVISO1600B2], _8_NB},

    {IFE_Outl_Bri[PRVLOWLIGHT], IFE_Outl_Dark[PRVLOWLIGHT], _8_NB},

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

FOV DCE_Fov[Total_SENMODENum][Total_ZOOMNum] =
{

    {
        {0x400,{DISABLE,1024,1024,1024}},
        {0x400,{DISABLE,1024,1024,1024}},
        {0x400,{DISABLE,1024,1024,1024}}
    }
};

EnH DCE_Enh[Total_SENMODENum][Total_ZOOMNum] =
{

    {
        //YEnh,      UVEnh
        { 96,   6,   16,   6  },
        { 96,   6,   16,   6  },
        { 96,   6,   16,   6  }
    }
};

Aberation DCE_ColorAbbration[Total_SENMODENum][Total_ZOOMNum] =
{
    {
        {0x1000,0x1000,0x1000},
        {0x1000,0x1000,0x1000},
        {0x1000,0x1000,0x1000}
    }

};

UINT32 DCE_2D_LUT[Total_SENMODENum][Total_ZOOMNum][65] =
{
    {//SenMode0
        {65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535},
        {59135,59235,59335,59435,59535,59635,59735,59835,59935,60035,60135,60235,60335,60435,60535,60635,60735,60835,60935,61035,61135,61235,61335,61435,61535,61635,61735,61835,61935,62035,62135,62235,62335,62435,62535,62635,62735,62835,62935,63035,63135,63235,63335,63435,63535,63635,63735,63835,63935,64035,64135,64235,64335,64435,64535,64635,64735,64835,64935,65035,65135,65235,65335,65435,65535},
        {65535,65535,65531,65524,65516,65504,65490,65473,65454,65433,65409,65382,65353,65322,65288,65252,65213,65172,65129,65083,65036,64986,64933,64879,64823,64764,64703,64641,64576,64509,64441,64370,64298,64224,64148,64071,63992,63911,63828,63744,63659,63572,63484,63394,63303,63210,63117,63022,62925,62828,62730,62630,62530,62428,62326,62222,62118,62013,61907,61800,61692,61584,61475,61365,61255},
    }
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
    {2,2,2,2},
    {3,3,3,3},
    {4,4,4,4},
    {4,4,4,4},

    {4,4,4,4},

    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};
UINT8 IPE_Gbal_Stab[Total_ISONum][16] =
{
    {4,2,1,1,1,0,0,0,0,0,0,0,0,1,2,3},
    {4,2,1,1,1,0,0,0,0,0,0,0,0,1,2,3},
    {4,3,2,1,1,0,0,0,0,0,0,0,0,1,2,3},
    {4,3,2,1,1,0,0,0,0,0,0,0,0,1,2,3},
    {5,4,3,2,1,0,0,0,0,0,0,0,0,1,2,3},
    {15,5,3,2,1,0,0,0,0,0,0,0,0,1,2,3},
    {15,6,4,1,1,0,0,0,0,0,0,0,0,1,2,3},

    {3,2,1,0,0,0,0,0,0,0,0,0,0,1,2,3},
    {3,2,1,0,0,0,0,0,0,0,0,0,0,1,2,3},
    {3,2,1,0,0,0,0,0,0,0,0,0,0,1,2,3},
    {3,2,1,0,0,0,0,0,0,0,0,0,0,1,2,3},
    {3,2,1,0,0,0,0,0,0,0,0,0,0,1,2,3},

    {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

    {2,1,1,0,0,0,0,0,0,0,0,0,0,1,2,3},
    {3,2,1,0,0,0,0,0,0,0,0,0,0,1,2,3},
    {4,3,2,1,0,0,0,0,0,0,0,0,0,1,2,3},
    {4,3,2,1,0,0,0,0,0,0,0,0,0,1,2,3},
    {5,4,3,2,1,0,0,0,0,0,0,0,0,1,2,3},
};

UINT8 IPE_Gbal_Dtab[Total_ISONum][16] =
{
    {3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {5,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {6,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {9,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {15,10,5,2,1,0,0,0,0,0,0,0,0,0,0,0},
    {15,10,5,2,1,0,0,0,0,0,0,0,0,0,0,0},

    {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//{10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

    {15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

    {3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {4,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

IPE_GBAL IPE_Gbal[Total_ISONum] =
{
    //Thres    {STab[16]}                 {DTab[16]}
    {  8,   IPE_Gbal_Stab[PRVISO0125],    IPE_Gbal_Dtab[PRVISO0125]},
    {  8,   IPE_Gbal_Stab[PRVISO0200],    IPE_Gbal_Dtab[PRVISO0200]},//8
    {  12,   IPE_Gbal_Stab[PRVISO0400],    IPE_Gbal_Dtab[PRVISO0400]},//11
    {  15,   IPE_Gbal_Stab[PRVISO0800],    IPE_Gbal_Dtab[PRVISO0800]},
    {  30,   IPE_Gbal_Stab[PRVISO1600],    IPE_Gbal_Dtab[PRVISO1600]},
    {  72,   IPE_Gbal_Stab[PRVISO3200],    IPE_Gbal_Dtab[PRVISO3200]},
    {  256,   IPE_Gbal_Stab[PRVISO6400],    IPE_Gbal_Dtab[PRVISO6400]},

    //Thres    {STab[16]}                 {DTab[16]}
    {  1023,   IPE_Gbal_Stab[PRVISO0100B2],    IPE_Gbal_Dtab[PRVISO0100B2]},
    {  1023,   IPE_Gbal_Stab[PRVISO0200B2],    IPE_Gbal_Dtab[PRVISO0200B2]},
    {  1023,   IPE_Gbal_Stab[PRVISO0400B2],    IPE_Gbal_Dtab[PRVISO0400B2]},
    {  1023,   IPE_Gbal_Stab[PRVISO0800B2],    IPE_Gbal_Dtab[PRVISO0800B2]},
    {  1023,   IPE_Gbal_Stab[PRVISO1600B2],    IPE_Gbal_Dtab[PRVISO1600B2]},

    {  1023,   IPE_Gbal_Stab[PRVLOWLIGHT],    IPE_Gbal_Dtab[PRVLOWLIGHT]},

    //Thres    {STab[16]}                 {DTab[16]}
    {  5,   IPE_Gbal_Stab[CAPISO0100],    IPE_Gbal_Dtab[CAPISO0100]},
    {  6,   IPE_Gbal_Stab[CAPISO0200],    IPE_Gbal_Dtab[CAPISO0200]},
    {  11,   IPE_Gbal_Stab[CAPISO0400],    IPE_Gbal_Dtab[CAPISO0400]},
    {  15,   IPE_Gbal_Stab[CAPISO0800],    IPE_Gbal_Dtab[CAPISO0800]},
    {  30,   IPE_Gbal_Stab[CAPISO1600],    IPE_Gbal_Dtab[CAPISO1600]}

};



IPE_CFAINTER IPE_CFAInter[Total_ISONum] =
{
    //NsmarEdge,  NsmarDiff,   {GCEn, GCNBit, DIFNBit}
    {       0,            8,   {TRUE,      0,       0}},
    {       0,            8,   {TRUE,      0,       0}},
    {       0,            8,   {TRUE,      1,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      3,       0}},
    {       0,            8,   {TRUE,      3,       0}},

    //NsmarEdge,  NsmarDiff,   {GCEn, GCNBit, DIFNBit}
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},

    {       0,            8,   {TRUE,      2,       0}},

    //NsmarEdge,  NsmarDiff,   {GCEn, GCNBit, DIFNBit}
    {       0,            8,   {TRUE,      2,       0}},
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
    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},
    {0,2,3,4,5,5,4,2},
};

IPE_DEE IPE_DirEdgeExt[Total_ISONum] =
{
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0125]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0200]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0400]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0800]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO1600]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO3200]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO6400]},

    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0100B2]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0200B2]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0400B2]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0800B2]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO1600B2]},

    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVLOWLIGHT]},

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
    {ESD_3X3,  LminusR,  UminusL,      0}
};

IPE_EDGEEXT IPE_EdgeExt[Total_ISONum] =
{
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0125],    2,   &IPE_DirEdgeExt[PRVISO0125],   &IPE_ESymDiff[PRVISO0125]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0200],    2,   &IPE_DirEdgeExt[PRVISO0200],   &IPE_ESymDiff[PRVISO0200]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0400],    2,   &IPE_DirEdgeExt[PRVISO0400],   &IPE_ESymDiff[PRVISO0400]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0800],    2,   &IPE_DirEdgeExt[PRVISO0800],   &IPE_ESymDiff[PRVISO0800]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO1600],    2,   &IPE_DirEdgeExt[PRVISO1600],   &IPE_ESymDiff[PRVISO1600]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO3200],    2,   &IPE_DirEdgeExt[PRVISO3200],   &IPE_ESymDiff[PRVISO3200]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO6400],    2,   &IPE_DirEdgeExt[PRVISO6400],   &IPE_ESymDiff[PRVISO6400]},

    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0100B2],    2,   &IPE_DirEdgeExt[PRVISO0100B2],   &IPE_ESymDiff[PRVISO0100B2]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0200B2],    2,   &IPE_DirEdgeExt[PRVISO0200B2],   &IPE_ESymDiff[PRVISO0200B2]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0400B2],    2,   &IPE_DirEdgeExt[PRVISO0400B2],   &IPE_ESymDiff[PRVISO0400B2]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0800B2],    2,   &IPE_DirEdgeExt[PRVISO0800B2],   &IPE_ESymDiff[PRVISO0800B2]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO1600B2],    2,   &IPE_DirEdgeExt[PRVISO1600B2],   &IPE_ESymDiff[PRVISO1600B2]},

    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVLOWLIGHT],    2,   &IPE_DirEdgeExt[PRVLOWLIGHT],   &IPE_ESymDiff[PRVLOWLIGHT]},

    //    GamSel, EdgeChSel,  EEXT_TblAddr,                       EWDIR    DED                          ESD}
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO0100],    2,   &IPE_DirEdgeExt[CAPISO0100],   &IPE_ESymDiff[CAPISO0100]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO0200],    2,   &IPE_DirEdgeExt[CAPISO0200],   &IPE_ESymDiff[CAPISO0200]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO0400],    2,   &IPE_DirEdgeExt[CAPISO0400],   &IPE_ESymDiff[CAPISO0400]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO0800],    3,   &IPE_DirEdgeExt[CAPISO0800],   &IPE_ESymDiff[CAPISO0800]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO1600],    4,   &IPE_DirEdgeExt[CAPISO1600],   &IPE_ESymDiff[CAPISO1600]}
};

IPE_EDGEENH IPE_EdgeEnh[Total_ISONum] =
{
    {     255,  275,  FALSE, FALSE},//255 280
    {     250,  265,  FALSE, FALSE},//250 270
    {     230,  240,  FALSE, FALSE},
    {     150,  160,  FALSE, FALSE},
    {     140,  140,  FALSE, FALSE},//{     122,  122,  FALSE, FALSE},
    {      85,  90,  FALSE, FALSE},
    {      85,  100,  FALSE, FALSE},

    {     220,  160,  FALSE, FALSE},
    {     220,  160,  FALSE, FALSE},
    {     200,  140,  FALSE, FALSE},
    {     180,  100,  FALSE, FALSE},
    {     160,  100,  FALSE, FALSE},//{     200,  140,  FALSE, FALSE},

    {     100,   60,  FALSE, FALSE},

    //  EnhP,  EnhN,  InvP,  InvN
    {     200,  220,  FALSE, FALSE},
    {     150,  160,  FALSE, FALSE},
    {     144,  144,  FALSE, FALSE},
    {     122,  122,  FALSE, FALSE},
    {     112,  112,  FALSE, FALSE},
};

//EDLut
UINT8 IPE_EDTab[Total_ISONum][16] =
{
    {0, 64,128, 255, 255, 255, 255, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 56,128, 255, 255, 255, 255, 255, 255, 228, 196, 160, 128, 96, 64, 32},
    {0, 48, 96, 192, 255, 255, 255, 255, 255, 228, 196, 160, 128, 96, 64, 32},
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
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}
};

UINT8 IPE_EStab[Total_ISONum][16] =
{
    {56, 64, 64, 64, 64, 64, 64, 64, 64, 62, 60, 58, 56, 52, 50, 48},
    {56, 64, 64, 64, 64, 64, 64, 64, 64, 60, 60, 58, 56, 52, 50, 48},
    {54, 64, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {48, 64, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {12, 30, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {8,  16, 32, 48, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {8,  16, 32, 48, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},

    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},

    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},

    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48}
};

EdgeMap IPE_EDMap[Total_ISONum] =
{
    //{EM_InSel, EthrL, EthrH, EtabL, EtabH, TAB}
    { EMI_AVG,     9,    256,     1,     4, (UINT32)&IPE_EDTab[PRVISO0125]},
    { EMI_AVG,     9,    256,     1,     4, (UINT32)&IPE_EDTab[PRVISO0200]},
    { EMI_AVG,     13,    256,     1,     4, (UINT32)&IPE_EDTab[PRVISO0400]},
    { EMI_AVG,     13,    256,     1,     4, (UINT32)&IPE_EDTab[PRVISO0800]},
    { EMI_AVG,     30,    256,     1,     4, (UINT32)&IPE_EDTab[PRVISO1600]},
    { EMI_AVG,     40,    256,     3,     4, (UINT32)&IPE_EDTab[PRVISO3200]},
    { EMI_AVG,     40,    256,     4,     4, (UINT32)&IPE_EDTab[PRVISO6400]},

    { EMI_AVG,      4,    256,     0,     4, (UINT32)&IPE_EDTab[PRVISO0100B2]},
    { EMI_AVG,      4,    256,     0,     4, (UINT32)&IPE_EDTab[PRVISO0200B2]},
    { EMI_AVG,      8,    256,     1,     4, (UINT32)&IPE_EDTab[PRVISO0400B2]},
    { EMI_AVG,     24,    256,     1,     4, (UINT32)&IPE_EDTab[PRVISO0800B2]},
    { EMI_AVG,     24,    256,     2,     4, (UINT32)&IPE_EDTab[PRVISO1600B2]},

    { EMI_AVG,     24,    256,     2,     4, (UINT32)&IPE_EDTab[PRVLOWLIGHT]},

    //{EM_InSel, EthrL, EthrH, EtabL, EtabH, TAB}
    { EMI_AVG,     4,    256,     0,     4, (UINT32)&IPE_EDTab[CAPISO0100]},
    { EMI_AVG,     4,    256,     0,     4, (UINT32)&IPE_EDTab[CAPISO0200]},
    { EMI_AVG,     8,    256,     1,     4, (UINT32)&IPE_EDTab[CAPISO0400]},
    { EMI_AVG,     8,    256,     2,     4, (UINT32)&IPE_EDTab[CAPISO0800]},
    { EMI_AVG,    12,    256,     3,     4, (UINT32)&IPE_EDTab[CAPISO1600]}
};

EdgeMap IPE_ESymMap[Total_ISONum] =
{
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0125]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0200]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0400]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0800]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO1600]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO3200]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO6400]},

    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0100B2]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0200B2]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0400B2]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0800B2]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO1600B2]},

    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVLOWLIGHT]},

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
    {{    2,     0,       8,       16,    _5x5}, {    2,     0,       8,       16,    _5x5}, {    2,     0,       8,       16,    _5x5}},
    {{    5,     0,       8,       16,    _5x5}, {    5,     0,       8,       16,    _5x5}, {    5,     0,       8,       16,    _5x5}},
    {{    6,     0,       8,       16,    _5x5}, {    6,     0,       8,       16,    _5x5}, {    6,     0,       8,       16,    _5x5}},
    {{    8,     0,       8,       16,    _7x7}, {    8,     0,       8,       16,    _7x7}, {    8,     0,       8,       16,    _7x7}},
    {{   10,     0,       8,       16,    _7x7}, {   10,     0,       4,       16,    _7x7}, {   10,     0,       8,       16,    _7x7}},
    {{   10,     0,       8,       16,    _7x7}, {   10,     0,       4,       16,    _7x7}, {   10,     0,       8,       16,    _7x7}},

    //  lpfw,sonlyw,rangeth0,rangeth1,lpfsize;
    {{    2,     0,       8,       16,    _3x3}, {    2,     0,       8,       16,    _3x3}, {    2,     0,       8,       16,    _3x3}},
    {{    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}},
    {{    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}},
    {{    4,     0,       8,       16,    _5x5}, {    4,     0,       8,       16,    _5x5}, {    4,     0,       8,       16,    _5x5}},
    {{    4,     0,       8,       16,    _5x5}, {    4,     0,       8,       16,    _5x5}, {    4,     0,       8,       16,    _5x5}},

    {{    4,     0,       8,       16,    _5x5}, {    4,     0,       8,       16,    _5x5}, {    4,     0,       8,       16,    _5x5}},

    //rlpfw,rsonlyw,rrangeth0,rrangeth1,rlpfsize,    glpfw,gsonlyw,grangeth0,grangeth1,glpfsize,    blpfw,bsonlyw,brangeth0,brangeth1,blpfsize
    {{    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}},
    {{    6,     0,       8,       16,    _3x3}, {    6,     0,       8,       16,    _3x3}, {    6,     0,       8,       16,    _3x3}},
    {{    7,     0,       8,       16,    _5x5}, {    7,     0,       8,       16,    _5x5}, {    7,     0,       8,       16,    _5x5}},
    {{    8,     0,       8,       16,    _7x7}, {    8,     0,       8,       16,    _7x7}, {    8,     0,       8,       16,    _7x7}},
    {{   10,     0,       8,       16,    _7x7}, {   10,     0,       4,       16,    _7x7}, {   10,     0,       8,       16,    _7x7}},
};

UINT16 Normal_CC[9] = {
0x1ae,      0x774,      0x7de,
0x7a4,      0x170,      0x7ec,
0x12,        0x72c,      0x1c1,
};
UINT16 Normal_Day[9] = {
0x1ae,      0x774,      0x7de,
0x7a4,      0x170,      0x7ec,
0x12,        0x72c,      0x1c1,
};
UINT16 Normal_CC_night[9] = {
0x17c,  0x790,  0x7f5,
0x7be,  0x152,  0x7f0,
0x11,   0x77e,  0x171,
};
//FSTAB
UINT8 CC_fmStab[Total_ISONum][16]=
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {8,0,0,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {64,32,16,8,4,2,0,0,0,0,0,0,0,32,96,255},
    {255,255,192,128,64,32,0,0,0,0,0,0,0,32,96,255},
    {255,255,255,255,128,64,32,0,0,0,0,0,0,32,96,255},
    {255,255,255,255,128,64,32,0,0,0,0,0,0,32,96,255},

    { 0, 0,0,0,0,0,0,0,0,0,0,0,0,32,96,255},
    { 8, 0,0,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {16, 8,0,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {16, 8,0,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,96,255},

    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,96,255},

    {0,0,0,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,96,255}
};

//FDTAB
UINT8 CC_fmDtab[Total_ISONum][16]=
{
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0},
    {255,192,64,32,16,0,0,0,0,0,0,0,0,0,0,0},
    {255,255,255,192,128,64,0,0,0,0,0,0,0,0,0,0},
    {255,255,255,192,128,64,0,0,0,0,0,0,0,0,0,0},

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
    128,128,128,128,128,128,128,128,
    128,128,128,128,128,128,128,128,
    128,128,128,128,128,128,128,128,


};

//CSTAM
INT32 IPE_satTab[24]=
{
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

//CINTM
INT32 IPE_intTab[24]=
{
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};

//CEDGM
UINT8 IPE_edgTab[24]=
{
    128,128,128,128,128,128,
    128,128,128,128,128,128,
    128,128,128,128,128,128,
    128,128,128,128,128,128
};

UINT8 IPE_ddsTab[8]=
{
    8,16,32,32,32,32,32,32
    //32,32,32,32,32,32,32,32
};

IPE_RAND_NR IPE_NoiseParam = { TRUE, 0, 0};


UINT32 GammaLUT_128Tab[130]={
        0,26,65,105,145,175,205,235,267,299,319,
    347,371,391,411,432,452,468,484,500,520,
    536,548,564,576,592,600,612,624,636,648,
    656,668,676,684,692,700,712,720,726,732,
    740,748,754,760,766,770,776,782,786,792,
    796,800,808,812,818,820,824,828,832,836,
    840,844,848,852,856,860,864,868,872,876,
    880,886,886,888,892,894,896,900,904,906,
    908,916,918,920,924,926,927,931,934,935,
    938,939,943,946,947,954,954,954,955,959,
    962,963,966,970,970,974,978,978,982,982,
    986,986,990,990,994,994,998,998,1002,1002,
    1006,1006,1010,1014,1014,1014,1015,1023,1023
};
UINT32 IQS_GammaLut_Night[130]=
{
    0, 44, 84, 124, 164, 196, 228, 256, 284, 308,
    332, 352, 372, 388, 404, 420, 436, 448, 460, 472,
    480, 488, 500, 508, 512, 520, 528, 532, 540, 544,
    548, 556, 560, 564, 572, 576, 580, 588, 592, 596,
    600, 608, 612, 616, 620, 624, 632, 636, 640, 644,
    648, 652, 660, 664, 668, 672, 676, 680, 684, 688,
    692, 700, 704, 708, 712, 716, 720, 724, 732, 736,
    740, 744, 748, 752, 760, 764, 768, 772, 776, 780,
    788, 792, 796, 800, 804, 808, 812, 816, 824, 828,
    832, 836, 840, 844, 848, 852, 860, 864, 868, 872,
    876, 884, 888, 892, 896, 900, 908, 912, 916, 920,
    928, 932, 936, 944, 948, 952, 956, 964, 968, 972,
    976, 984, 988, 992, 1000, 1004, 1008, 1012, 1023, 1023

};

//UINT8 Saturation[Total_ISONum][SEL_SATURATION_MAX_CNT] = {
INT8 Saturation[Total_ISONum][SEL_SATURATION_MAX_CNT] = {
    {-20,    15,   30},
    {-20,    7,   20},
    {-20,    0,   20},
    {-35,   -15,   5},
    {-35,   -28,   5},
    {-40,   -40,   0},
    {-40,   -45,   0},

/*
    {-30,   -10,  10},
    {-40,   -20,   0},
    {-50,   -30, -10},
    {-70,   -50, -30},
    {-70,   -50, -30},
*/
    {-30,   -10,  10},
    {-40,   -20,   0},
    {-50,   -30, -10},
    {-50,   -30, -10},
    {-50,   -30, -10},


    {-70,   -50, -30},

   //N1,  Normal,  P1
    {-20,    0,   20},
    {-20,    0,   20},
    {-30,   -10,  10},
    {-35,   -15,   5},
    {-40,   -20,   0},
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

   //N1,  Normal,  P1
    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},
    {32,   128,   255},
};

//For DIS
IPE_EDGETHRE IPE_EdgeThre[Total_ISONum] = {
    {EdgeKer_5x5_OUT, 0, 128, 255},
    {EdgeKer_5x5_OUT, 0, 128, 255},
    {EdgeKer_5x5_OUT, 0, 128, 255},
    {EdgeKer_5x5_OUT, 0, 128, 255},
    {EdgeKer_5x5_OUT, 0, 128, 255},
    {EdgeKer_5x5_OUT, 0, 128, 255},
    {EdgeKer_5x5_OUT, 0, 128, 255},

    {EdgeKer_5x5_OUT, 0, 128, 255},
    {EdgeKer_5x5_OUT, 0, 128, 255},
    {EdgeKer_5x5_OUT, 0, 128, 255},
    {EdgeKer_5x5_OUT, 0, 128, 255},
    {EdgeKer_5x5_OUT, 0, 128, 255},

    {EdgeKer_5x5_OUT, 0, 128, 255},

    {EdgeKer_5x5_OUT, 0, 128, 255},
    {EdgeKer_5x5_OUT, 0, 128, 255},
    {EdgeKer_5x5_OUT, 0, 128, 255},
    {EdgeKer_5x5_OUT, 0, 128, 255},
    {EdgeKer_5x5_OUT, 0, 128, 255}
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
    {{TRUE, GRP0_1_2, EdgeKer_7x7_OUT, {0, 0}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}, {E_PRE_GAMMA, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}, {Filtering_0, {0, 0, 0, 0}, 0}}}
};

////////////////////////////////////////////////
// IFE2
////////////////////////////////////////////////

IFE2_Chroma IFE2_Chroma_param[Total_ISONum] =
{
    {{128, 128}, {1,  4,  7,  9}, {128, 115, 90, 64, 0}},
    {{128, 128}, {1,  4,  7,  9}, {128, 115, 90, 64, 0}},
    {{128, 128}, {1,  4,  7,  9}, {128, 115, 90, 64, 0}},
    {{128, 128}, {1,  4,  7,  9}, {128, 115, 90, 64, 0}},
    {{128, 128}, {1,  4,  7,  9}, {128, 115, 90, 64, 0}},
    {{128, 128}, {1,  4,  7,  9}, {128, 96, 64, 32, 0}},
    {{128, 128}, {1,  4,  7,  9}, {128, 96, 64, 32, 0}},

    {{128, 128}, {1,  4,  7,  9}, {128, 115, 90, 64, 0}},
    {{128, 128}, {1,  4,  7,  9}, {128, 115, 90, 64, 0}},
    {{128, 128}, {1,  4,  7,  9}, {128, 115, 90, 64, 0}},
    {{128, 128}, {1,  4,  7,  9}, {128, 115, 90, 64, 0}},
    {{128, 128}, {1,  4,  7,  9}, {128, 115, 90, 64, 0}},

    {{128, 128}, {1,  4,  7,  9}, {128, 115, 90, 64, 0}},

    {{128, 128}, {1,  2,  3,  4}, {128, 115, 90, 64, 0}},
    {{128, 128}, {2,  4,  6,  8}, {128, 115, 90, 64, 0}},
    {{128, 128}, {3,  5,  8, 10}, {128, 115, 90, 64, 0}},
    {{128, 128}, {3,  6,  9, 12}, {128, 115, 90, 64, 0}},
    {{128, 128}, {6, 10, 14, 18}, {128, 115, 90, 64, 0}}
};

IQS_IFE2_FILTER_TIMES IFE2_Filter_Times[Total_ISONum] =
{
    //IFE2_CC, IFE2_V, MFR
    { 1,       0,       0},
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

    { 1,       0,       0},

    //IFE2_CC, IFE2_V, MFR
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       1,       1},
    { 2,       2,       1}
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
};

//lca
AvgFilt IFE2_AvgFilter_cb[Total_ISONum] =
{
    //cenWt, avgTh,    cntTh, fltSize
      {  20, { 3,  7},      0, IFE2_FLTR_7X7},
      {  20, { 3,  7},      0, IFE2_FLTR_7X7},
      {  10, { 4,  9},      0, IFE2_FLTR_7X7},
      {  10, { 5, 12},      0, IFE2_FLTR_7X7},
      {  10, { 12, 24},      0, IFE2_FLTR_7X7},
      {  10, { 22, 27},      0, IFE2_FLTR_7X7},
      {  10, { 22, 27},      0, IFE2_FLTR_7X7},

    //cenWt, avgTh,    cntTh, fltSize
      {  10, { 3,  7},      0, IFE2_FLTR_7X7},
      {  10, { 4,  9},      0, IFE2_FLTR_7X7},
      {  10, { 5, 11},      0, IFE2_FLTR_7X7},
      {  10, { 6, 13},      0, IFE2_FLTR_7X7},
      {  10, { 7, 15},      0, IFE2_FLTR_7X7},

      {  10, { 7, 15},      0, IFE2_FLTR_7X7},

    //cenWt, avgTh,     cntTh, fltSize
      {  10, { 5, 12},      0, IFE2_FLTR_7X7},
      {  10, { 5, 12},      0, IFE2_FLTR_7X7},
      {  10, { 5, 12},      0, IFE2_FLTR_7X7},
      {  10, { 5, 12},      0, IFE2_FLTR_7X7},
      {  10, { 5, 12},      0, IFE2_FLTR_7X7},
};

AvgFilt IFE2_AvgFilter_cr[Total_ISONum] =
{
    //cenWt, avgTh,    cntTh, fltSize
      {  20, { 3,  7},      0, IFE2_FLTR_7X7},
      {  20, { 3,  7},      0, IFE2_FLTR_7X7},
      {  10, { 4,  9},      0, IFE2_FLTR_7X7},
      {  10, { 5, 12},      0, IFE2_FLTR_7X7},
      {  10, { 12, 24},      0, IFE2_FLTR_7X7},
      {  10, { 22, 27},      0, IFE2_FLTR_7X7},
      {  10, { 22, 27},      0, IFE2_FLTR_7X7},

    //cenWt, avgTh,    cntTh, fltSize
      {  10, { 3,  7},      0, IFE2_FLTR_7X7},
      {  10, { 4,  9},      0, IFE2_FLTR_7X7},
      {  10, { 5, 11},      0, IFE2_FLTR_7X7},
      {  10, { 6, 13},      0, IFE2_FLTR_7X7},
      {  10, { 7, 15},      0, IFE2_FLTR_7X7},

      {  10, { 7, 15},      0, IFE2_FLTR_7X7},

    //cenWt, avgTh,     cntTh, fltSize
      {  10, { 5, 12},      0, IFE2_FLTR_7X7},
      {  10, { 5, 12},      0, IFE2_FLTR_7X7},
      {  10, { 5, 12},      0, IFE2_FLTR_7X7},
      {  10, { 5, 12},      0, IFE2_FLTR_7X7},
      {  10, { 5, 12},      0, IFE2_FLTR_7X7},
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
};

////////////////////////////////////////////////
// IME
////////////////////////////////////////////////
UINT32 IME_3DNR_ThresY[Total_ISONum][3] =
{
    {1,2,3},//{2,4,6},
    {1,2,3},
    {1,2,3},
    {1,2,3},
    {2,4,6},
    {2,4,6},
    {2,4,6},

    {1,2,3},
    {1,2,3},
    {1,2,3},
    {1,2,3},
    {1,2,3},

    {1,2,3},

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
    {7,4,2,0}
};

UINT32 IME_3DNR_ThresCC[Total_ISONum][3] =
{
    {4,8,12},
    {4,8,12},
    {4,8,12},
    {8,16,24},
    {8,16,24},
    {8,16,24},
    {8,16,24},

    {4,8,12},
    {4,8,12},
    {8,16,24},
    {8,16,24},
    {8,16,24},

    {8,16,24},

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
    {7,4,2,0}
};


IME_3D_NR IME_3DNR[Total_ISONum] =
{
     { IME_3DNR_ThresY[PRVISO0125], IME_3DNR_WeightY[PRVISO0125], IME_3DNR_ThresCC[PRVISO0125], IME_3DNR_WeightCC[PRVISO0125], IME_3DNR_ThresCC[PRVISO0125], IME_3DNR_WeightCC[PRVISO0125]},
     { IME_3DNR_ThresY[PRVISO0200], IME_3DNR_WeightY[PRVISO0200], IME_3DNR_ThresCC[PRVISO0200], IME_3DNR_WeightCC[PRVISO0200], IME_3DNR_ThresCC[PRVISO0200], IME_3DNR_WeightCC[PRVISO0200]},
     { IME_3DNR_ThresY[PRVISO0400], IME_3DNR_WeightY[PRVISO0400], IME_3DNR_ThresCC[PRVISO0400], IME_3DNR_WeightCC[PRVISO0400], IME_3DNR_ThresCC[PRVISO0400], IME_3DNR_WeightCC[PRVISO0400]},
     { IME_3DNR_ThresY[PRVISO0800], IME_3DNR_WeightY[PRVISO0800], IME_3DNR_ThresCC[PRVISO0800], IME_3DNR_WeightCC[PRVISO0800], IME_3DNR_ThresCC[PRVISO0800], IME_3DNR_WeightCC[PRVISO0800]},
     { IME_3DNR_ThresY[PRVISO1600], IME_3DNR_WeightY[PRVISO1600], IME_3DNR_ThresCC[PRVISO1600], IME_3DNR_WeightCC[PRVISO1600], IME_3DNR_ThresCC[PRVISO1600], IME_3DNR_WeightCC[PRVISO1600]},
     { IME_3DNR_ThresY[PRVISO3200], IME_3DNR_WeightY[PRVISO3200], IME_3DNR_ThresCC[PRVISO3200], IME_3DNR_WeightCC[PRVISO3200], IME_3DNR_ThresCC[PRVISO3200], IME_3DNR_WeightCC[PRVISO3200]},
     { IME_3DNR_ThresY[PRVISO6400], IME_3DNR_WeightY[PRVISO6400], IME_3DNR_ThresCC[PRVISO6400], IME_3DNR_WeightCC[PRVISO6400], IME_3DNR_ThresCC[PRVISO6400], IME_3DNR_WeightCC[PRVISO6400]},

     { IME_3DNR_ThresY[PRVISO0100B2], IME_3DNR_WeightY[PRVISO0100B2], IME_3DNR_ThresCC[PRVISO0100B2], IME_3DNR_WeightCC[PRVISO0100B2], IME_3DNR_ThresCC[PRVISO0100B2], IME_3DNR_WeightCC[PRVISO0100B2]},
     { IME_3DNR_ThresY[PRVISO0200B2], IME_3DNR_WeightY[PRVISO0200B2], IME_3DNR_ThresCC[PRVISO0200B2], IME_3DNR_WeightCC[PRVISO0200B2], IME_3DNR_ThresCC[PRVISO0200B2], IME_3DNR_WeightCC[PRVISO0200B2]},
     { IME_3DNR_ThresY[PRVISO0400B2], IME_3DNR_WeightY[PRVISO0400B2], IME_3DNR_ThresCC[PRVISO0400B2], IME_3DNR_WeightCC[PRVISO0400B2], IME_3DNR_ThresCC[PRVISO0400B2], IME_3DNR_WeightCC[PRVISO0400B2]},
     { IME_3DNR_ThresY[PRVISO0800B2], IME_3DNR_WeightY[PRVISO0800B2], IME_3DNR_ThresCC[PRVISO0800B2], IME_3DNR_WeightCC[PRVISO0800B2], IME_3DNR_ThresCC[PRVISO0800B2], IME_3DNR_WeightCC[PRVISO0800B2]},
     { IME_3DNR_ThresY[PRVISO1600B2], IME_3DNR_WeightY[PRVISO1600B2], IME_3DNR_ThresCC[PRVISO1600B2], IME_3DNR_WeightCC[PRVISO1600B2], IME_3DNR_ThresCC[PRVISO1600B2], IME_3DNR_WeightCC[PRVISO1600B2]},

     { IME_3DNR_ThresY[PRVLOWLIGHT], IME_3DNR_WeightY[PRVLOWLIGHT], IME_3DNR_ThresCC[PRVLOWLIGHT], IME_3DNR_WeightCC[PRVLOWLIGHT], IME_3DNR_ThresCC[PRVLOWLIGHT], IME_3DNR_WeightCC[PRVLOWLIGHT]},

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
};

IME_CHRA_ADAP IME_Chroma_Adapt_param[Total_ISONum] =
{
    {Precision_4,      6,       6,       0,         100, (UINT32)IME_LCATbl[PRVISO0125], 20},
    {Precision_4,      6,       6,       0,         128, (UINT32)IME_LCATbl[PRVISO0200], 20},
    {Precision_4,      6,       6,       0,         176, (UINT32)IME_LCATbl[PRVISO0400], 20},
    {Precision_4,      6,       6,       0,         178, (UINT32)IME_LCATbl[PRVISO0800], 20},
    {Precision_4,      7,       7,       0,         180, (UINT32)IME_LCATbl[PRVISO1600], 20},
    {Precision_4,      10,      10,       0,         190, (UINT32)IME_LCATbl[PRVISO3200], 20},
    {Precision_4,      10,       10,       0,         250, (UINT32)IME_LCATbl[PRVISO6400], 20},

    {Precision_4,      6,       6,       0,         160, (UINT32)IME_LCATbl[PRVISO0100B2], 20},
    {Precision_4,      6,       6,       0,         176, (UINT32)IME_LCATbl[PRVISO0200B2], 20},
    {Precision_4,      6,       6,       0,         192, (UINT32)IME_LCATbl[PRVISO0400B2], 20},
    {Precision_4,      6,       6,       0,         216, (UINT32)IME_LCATbl[PRVISO0800B2], 20},
    {Precision_4,      6,       6,       0,         250, (UINT32)IME_LCATbl[PRVISO1600B2], 20},

    {Precision_4,      6,       6,       0,         250, (UINT32)IME_LCATbl[PRVLOWLIGHT], 20},

    //LcaLutPre, Thres_Cb, Thres_Cr, Thres_Lut, LcaWeight, LcaLUT_TblAddr,      SubRatio
    {Precision_4,    255,      255,      0,         160, (UINT32)IME_LCATbl[CAPISO0100], 20},
    {Precision_4,    255,      255,      0,         176, (UINT32)IME_LCATbl[CAPISO0200], 20},
    {Precision_4,    255,      255,      0,         192, (UINT32)IME_LCATbl[CAPISO0400], 20},
    {Precision_4,    255,      255,      0,         216, (UINT32)IME_LCATbl[CAPISO0800], 20},
    {Precision_4,    255,      255,      0,         250, (UINT32)IME_LCATbl[CAPISO1600], 20},
};

UINT32 IME_ChromaFilt_rngth_cb[Total_ISONum][4] =
{
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},

    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},

    { 2, 4, 6, 8},

    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
};
UINT32 IME_ChromaFilt_rngth_cr[Total_ISONum][4] =
{
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},

    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 2, 4, 6, 8},
    { 2, 4, 6, 8},

    { 2, 4, 6, 8},

    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
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
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0125], IME_ChromaFilt_rngth_cr[PRVISO0125], IME_ChromaFilt_rngWt[PRVISO0125]}, {2, 1, 0}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0200], IME_ChromaFilt_rngth_cr[PRVISO0200], IME_ChromaFilt_rngWt[PRVISO0200]}, {2, 1, 0}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0400], IME_ChromaFilt_rngth_cr[PRVISO0400], IME_ChromaFilt_rngWt[PRVISO0400]}, {2, 1, 0}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0800], IME_ChromaFilt_rngth_cr[PRVISO0800], IME_ChromaFilt_rngWt[PRVISO0800]}, {2, 1, 0}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO1600], IME_ChromaFilt_rngth_cr[PRVISO1600], IME_ChromaFilt_rngWt[PRVISO1600]}, {2, 1, 0}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO3200], IME_ChromaFilt_rngth_cr[PRVISO3200], IME_ChromaFilt_rngWt[PRVISO3200]}, {2, 1, 0}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO6400], IME_ChromaFilt_rngth_cr[PRVISO6400], IME_ChromaFilt_rngWt[PRVISO6400]}, {2, 1, 0}},

    //RangeFiltEn, RangeFilt, SpaceFilt
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0100B2], IME_ChromaFilt_rngth_cr[PRVISO0100B2], IME_ChromaFilt_rngWt[PRVISO0100B2]}, {2, 1, 0}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0200B2], IME_ChromaFilt_rngth_cr[PRVISO0200B2], IME_ChromaFilt_rngWt[PRVISO0200B2]}, {2, 1, 0}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0400B2], IME_ChromaFilt_rngth_cr[PRVISO0400B2], IME_ChromaFilt_rngWt[PRVISO0400B2]}, {2, 1, 0}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0800B2], IME_ChromaFilt_rngth_cr[PRVISO0800B2], IME_ChromaFilt_rngWt[PRVISO0800B2]}, {2, 1, 0}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO1600B2], IME_ChromaFilt_rngth_cr[PRVISO1600B2], IME_ChromaFilt_rngWt[PRVISO1600B2]}, {2, 1, 0}},

    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVLOWLIGHT], IME_ChromaFilt_rngth_cr[PRVLOWLIGHT], IME_ChromaFilt_rngWt[PRVLOWLIGHT]}, {2, 1, 0}},

    //RangeFiltEn, RangeFilt, SpaceFilt
    {TRUE,         {IME_ChromaFilt_rngth_cb[CAPISO0100], IME_ChromaFilt_rngth_cr[CAPISO0100], IME_ChromaFilt_rngWt[CAPISO0100]}, {2, 1, 0}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[CAPISO0200], IME_ChromaFilt_rngth_cr[CAPISO0200], IME_ChromaFilt_rngWt[CAPISO0200]}, {1, 1, 0}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[CAPISO0400], IME_ChromaFilt_rngth_cr[CAPISO0400], IME_ChromaFilt_rngWt[CAPISO0400]}, {1, 1, 0}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[CAPISO0800], IME_ChromaFilt_rngth_cr[CAPISO0800], IME_ChromaFilt_rngWt[CAPISO0800]}, {1, 1, 0}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[CAPISO1600], IME_ChromaFilt_rngth_cr[CAPISO1600], IME_ChromaFilt_rngWt[CAPISO1600]}, {2, 1, 0}},
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
  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},

  {    1024, 1024, 1024, 1024, 1024, 1024, 1024, 0, 50},

  //GL_GAIN,GD_GAIN,LL_GAIN,LD_GAIN,R_GAIN,G_GAIN,B_GAIN,ExGain, SubRatio;
  {    1024,  1024,   563,    563,   1024,  1024,  1024,   0,    50},
  {    1024,  1024,   563,    563,   1024,  1024,  1024,   0,    50},
  {    1024,  1024,   563,    563,   1024,  1024,  1024,   0,    50},
  {    1024,  1024,   563,    563,   1024,  1024,  1024,   0,    50},
  {    1024,  1024,   563,    563,   1024,  1024,  1024,   0,    50}
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
#endif
