#ifndef _IQS_SETTINGTABLE_GC2023P_TVP5150_CARDV_FFFF_C
#define _IQS_SETTINGTABLE_GC2023P_TVP5150_CARDV_FFFF_C
#include "IPL_GC2023P_TVP5150_CARDV_FFFF_Int.h"


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
      {ENABLE, 109, 155, 76, 0, VIG_TAB_GAIN_1X, 962, 962, FALSE, FALSE}
};

////////////////////////////////////////////////
// IFE
////////////////////////////////////////////////
IQS_IFE_FUNC IFE_Func[Total_ISONum] =
{
    {DISABLE},
    {ENABLE},
    {ENABLE},
    {ENABLE},
    {ENABLE},
    {ENABLE},
    {ENABLE},

    {DISABLE},
    {DISABLE},
    {DISABLE},
    {DISABLE},
    {DISABLE}
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
};


UINT32 IFE_rngth_NLM[Total_ISONum][4] =
{
    {1,1,2,3},//{0,0,0,0},
    {2,3,4,5},//{0,0,0,0},
    {2,4,5,6},//{1,1,2,3},//{1,3,4,5},//{1,1,2,3},
    {2,3,7,9},
    {4,5,9,17},
    {5,7,12,23},
    {6,9,15,25},
    //{8,11,14,17},//{6,9,12,16},

    {14,20,25,30},

    {1,1,2,3},
    {1,3,4,5},
    {2,3,5,7},
    {2,5,7,10},
    {3,6,9,12},
};

UINT32 IFE_rngth_Bil[Total_ISONum][4] =
{
   {1,1,2,3},
   {1,1,2,3},
   {1,3,4,5},//100
   {2,4,6,8},//200
   {3,5,7,10},//400
   {7,10,13,16},//800
   {9,13,18,25},//1600
   //{12,18,24,30},//3200

    {12,15,18,21},//3200

    { 5, 7, 9,11},
    { 7,10,13,16},
    { 9,13,17,21},
    {12,18,24,30},
    {16,22,28,34}
};

IFE_FILT IFE_Filter_param[Total_ISONum] =
{
    //Mode,    {sOnlyEn, sOnlyLen, weights},                  {weightr_NLM,                     weightr_Bil    ,              bilat_w},  clamp{Th,  Mul, Dlt}, Rth_w, Bin}
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO050]}, {{IFE_rngth_NLM[PRVISO050],  5}, {IFE_rngth_Bil[PRVISO050],  5}, 1},    { 0, 128,   0},   0,      0},//50
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0100]}, {{IFE_rngth_NLM[PRVISO0100], 40}, {IFE_rngth_Bil[PRVISO0100], 25}, 1},    { 0, 128,   0},   0,      0},//100
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0200]}, {{IFE_rngth_NLM[PRVISO0200], 55}, {IFE_rngth_Bil[PRVISO0200], 55}, 1},  { 0, 128,   0},   0,      0},//200
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0400]}, {{IFE_rngth_NLM[PRVISO0400], 55}, {IFE_rngth_Bil[PRVISO0400], 55}, 1},  { 0, 128,   0},   0,      0},//400
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO0800]}, {{IFE_rngth_NLM[PRVISO0800], 70}, {IFE_rngth_Bil[PRVISO0800], 70}, 1},  { 0, 128,   0},   0,      0},//800
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO1600]}, {{IFE_rngth_NLM[PRVISO1600], 80}, {IFE_rngth_Bil[PRVISO1600], 80}, 3},  { 0, 128,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO3200]}, {{IFE_rngth_NLM[PRVISO3200], 35}, {IFE_rngth_Bil[PRVISO3200], 80}, 3},  { 0, 128,   0},   0,      0},

    //{ Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO6400]}, {{IFE_rngth_NLM[PRVISO6400], 35}, {IFE_rngth_Bil[PRVISO6400], 70}, 3},  { 0, 128,   0},   0,      0},
    { Normal,  {DISABLE, Filt_3X3, IFE_weights[PRVISO6400]}, {{IFE_rngth_NLM[PRVISO6400], 30}, {IFE_rngth_Bil[PRVISO6400], 80}, 3},  { 0, 128,   0},   0,      0},

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
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},

    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
};

UINT32 IFE_Outl_Dark[Total_ISONum][5] =
{
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},

    {126, 184, 304, 420, 512},

    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
    {126, 184, 304, 420, 512},
};

IFE_OUTL IFE_Outl_param[Total_ISONum] =
{
    {IFE_Outl_Bri[PRVISO050], IFE_Outl_Dark[PRVISO050], _8_NB},
    {IFE_Outl_Bri[PRVISO0100], IFE_Outl_Dark[PRVISO0100], _8_NB},
    {IFE_Outl_Bri[PRVISO0200], IFE_Outl_Dark[PRVISO0200], _8_NB},
    {IFE_Outl_Bri[PRVISO0400], IFE_Outl_Dark[PRVISO0400], _8_NB},
    {IFE_Outl_Bri[PRVISO0800], IFE_Outl_Dark[PRVISO0800], _8_NB},
    {IFE_Outl_Bri[PRVISO1600], IFE_Outl_Dark[PRVISO1600], _8_NB},
    {IFE_Outl_Bri[PRVISO3200], IFE_Outl_Dark[PRVISO3200], _8_NB},

    {IFE_Outl_Bri[PRVISO6400], IFE_Outl_Dark[PRVISO6400], _8_NB},

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
//10 bits
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
};

UINT8 IPE_Gbal_Dtab[Total_ISONum][16] =
{
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
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
    {  5,   IPE_Gbal_Stab[PRVISO050],    IPE_Gbal_Dtab[PRVISO050]},
    {  7,   IPE_Gbal_Stab[PRVISO0100],    IPE_Gbal_Dtab[PRVISO0100]},
    {  10,   IPE_Gbal_Stab[PRVISO0200],    IPE_Gbal_Dtab[PRVISO0200]},
    {  10,   IPE_Gbal_Stab[PRVISO0400],    IPE_Gbal_Dtab[PRVISO0400]},
    {  30,   IPE_Gbal_Stab[PRVISO0800],    IPE_Gbal_Dtab[PRVISO0800]},
    {  1023,   IPE_Gbal_Stab[PRVISO1600],    IPE_Gbal_Dtab[PRVISO1600]},
    {  1023,   IPE_Gbal_Stab[PRVISO3200],    IPE_Gbal_Dtab[PRVISO3200]},

    {  1023,   IPE_Gbal_Stab[PRVISO6400],    IPE_Gbal_Dtab[PRVISO6400]},

    //Thres    {STab[16]}                 {DTab[16]}
    {  1023,   IPE_Gbal_Stab[CAPISO0100],    IPE_Gbal_Dtab[CAPISO0100]},
    {  1023,   IPE_Gbal_Stab[CAPISO0200],    IPE_Gbal_Dtab[CAPISO0200]},
    {  1023,   IPE_Gbal_Stab[CAPISO0400],    IPE_Gbal_Dtab[CAPISO0400]},
    {  1023,   IPE_Gbal_Stab[CAPISO0800],    IPE_Gbal_Dtab[CAPISO0800]},
    {  1023,   IPE_Gbal_Stab[CAPISO1600],    IPE_Gbal_Dtab[CAPISO1600]}
};

IPE_CFAINTER IPE_CFAInter[Total_ISONum] =
{
    //NsmarEdge,  NsmarDiff,   {GCEn, GCNBit, DIFNBit}
    {       0,            8,   {TRUE,      2,       0}},
    {       0,            8,   {TRUE,      2,       0}},
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
};

IPE_DEE IPE_DirEdgeExt[Total_ISONum] =
{
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO050]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0100]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0200]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0400]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO0800]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO1600]},
    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO3200]},

    {_EEXT1,   Div_2, (UINT32)&IPE_EDirTab[PRVISO6400]},

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
    {ESD_3X3,  LminusR,  UminusL,      0}
};

IPE_EDGEEXT IPE_EdgeExt[Total_ISONum] =
{
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO050],    0,   &IPE_DirEdgeExt[PRVISO050],   &IPE_ESymDiff[PRVISO050]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0100],    0,   &IPE_DirEdgeExt[PRVISO0100],   &IPE_ESymDiff[PRVISO0100]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0200],    0,   &IPE_DirEdgeExt[PRVISO0200],   &IPE_ESymDiff[PRVISO0200]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0400],    1,   &IPE_DirEdgeExt[PRVISO0400],   &IPE_ESymDiff[PRVISO0400]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO0800],    2,   &IPE_DirEdgeExt[PRVISO0800],   &IPE_ESymDiff[PRVISO0800]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO1600],    4,   &IPE_DirEdgeExt[PRVISO1600],   &IPE_ESymDiff[PRVISO1600]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO3200],    4,   &IPE_DirEdgeExt[PRVISO3200],   &IPE_ESymDiff[PRVISO3200]},

    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[PRVISO6400],    4,   &IPE_DirEdgeExt[PRVISO6400],   &IPE_ESymDiff[PRVISO6400]},

    //    GamSel, EdgeChSel,  EEXT_TblAddr,                       EWDIR    DED                          ESD}
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO0100],    2,   &IPE_DirEdgeExt[CAPISO0100],   &IPE_ESymDiff[CAPISO0100]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO0200],    2,   &IPE_DirEdgeExt[CAPISO0200],   &IPE_ESymDiff[CAPISO0200]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO0400],    2,   &IPE_DirEdgeExt[CAPISO0400],   &IPE_ESymDiff[CAPISO0400]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO0800],    3,   &IPE_DirEdgeExt[CAPISO0800],   &IPE_ESymDiff[CAPISO0800]},
    {E_POS_GAMMA, EDGE_Y_CH,  (UINT32)&IPE_edgeKernel[CAPISO1600],    4,   &IPE_DirEdgeExt[CAPISO1600],   &IPE_ESymDiff[CAPISO1600]}
};

IPE_EDGEENH IPE_EdgeEnh[Total_ISONum] =
{
#if 1
    {     230,  220,  FALSE, FALSE},
    {     220,  210,  FALSE, FALSE},
    {     200,  190,  FALSE, FALSE},
    {     200,  190,  FALSE, FALSE},
    {     190,  180,  FALSE, FALSE},
    {     160,  140,  FALSE, FALSE},
    {     120,  100,  FALSE, FALSE},
#endif
#if 0
    //0224
    {     170,  150,  FALSE, FALSE},
    {     150,  120,  FALSE, FALSE},
    {     150,  100,  FALSE, FALSE},
    {     140,   80,  FALSE, FALSE},
    {      90,   60,  FALSE, FALSE},
    {      60,   00,  FALSE, FALSE},

        //0314
    {     190,  190,  FALSE, FALSE},
    {     150,  150,  FALSE, FALSE},
    {     130,  130,  FALSE, FALSE},
    {     110,  110,  FALSE, FALSE},
    {     90,  80,  FALSE, FALSE},
    {     90,  80,  FALSE, FALSE},
#endif

    {     70,  60,  FALSE, FALSE},//3200

    //  EnhP,  EnhN,  InvP,  InvN
    {     120,  110,  FALSE, FALSE},
    {     110,  100,  FALSE, FALSE},
    {     100,  100,  FALSE, FALSE},
    {     100,  90,  FALSE, FALSE},
    {     90,  80,  FALSE, FALSE},
};

//EDLut
UINT8 IPE_EDTab[Total_ISONum][16] =
{
    {0, 128, 160, 160, 160, 192, 224, 255, 255, 228, 196, 160, 160, 128, 128, 128},
    {0, 128, 160, 160, 160, 192, 224, 255, 255, 228, 196, 160, 160, 128, 128, 128},
    {0, 128, 160, 160, 160, 192, 224, 255, 255, 228, 196, 160, 160, 128, 128, 128},
    {0, 128, 160, 160, 160, 192, 224, 255, 255, 228, 196, 160, 160, 128, 128, 128},
    {0, 128, 160, 160, 160, 192, 224, 255, 255, 228, 196, 160, 160, 128, 128, 128},
    {0, 128, 160, 160, 160, 192, 224, 255, 255, 228, 196, 160, 160, 128, 128, 128},
    {0, 128, 160, 160, 160, 192, 224, 255, 255, 228, 196, 160, 160, 128, 128, 128},

    {0, 160, 160, 192, 192, 192, 224, 255, 255, 255, 255, 255, 224, 192, 192, 192},

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
    {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}
};

UINT8 IPE_EStab[Total_ISONum][16] =
{
    {48, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {48, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {48, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {48, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {48, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {48, 60, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16,32, 48, 64, 64, 64, 64, 64, 64,  60, 56, 52, 48, 48, 48, 48},

    {32, 48, 64, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},

    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48},
    {16, 32, 48, 64, 64, 64, 64, 64, 64, 60, 56, 52, 48, 48, 48, 48}
};

EdgeMap IPE_EDMap[Total_ISONum] =
{
//{EM_InSel, EthrL, EthrH, EtabL, EtabH, TAB}
    { EMI_AVG,     1,    256,     0,     4, (UINT32)&IPE_EDTab[PRVISO050]},
    { EMI_AVG,     1,    256,     0,     4, (UINT32)&IPE_EDTab[PRVISO0100]},
    { EMI_AVG,     6,    256,     0,     4, (UINT32)&IPE_EDTab[PRVISO0200]},
    { EMI_AVG,    12,    256,     1,     4, (UINT32)&IPE_EDTab[PRVISO0400]},
    { EMI_AVG,    16,    256,     1,     4, (UINT32)&IPE_EDTab[PRVISO0800]},//{ EMI_AVG,    16,    256,     1,     4, (UINT32)&IPE_EDTab[PRVISO0800]},
    { EMI_AVG,    26,    256,     1,     4, (UINT32)&IPE_EDTab[PRVISO1600]},
    //{ EMI_AVG,    34,    256,     2,     4, (UINT32)&IPE_EDTab[PRVISO3200]},//{ EMI_AVG,    64,    256,     2,     4, (UINT32)&IPE_EDTab[PRVISO3200]},
    { EMI_AVG,    40,    256,     2,     4, (UINT32)&IPE_EDTab[PRVISO3200]},

    //{ EMI_AVG,    58,    256,     2,     4, (UINT32)&IPE_EDTab[PRVISO6400]},
    { EMI_AVG,   48,    256,     2,     4, (UINT32)&IPE_EDTab[PRVISO6400]},

    //{EM_InSel, EthrL, EthrH, EtabL, EtabH, TAB}
    { EMI_AVG,     4,    256,     0,     4, (UINT32)&IPE_EDTab[CAPISO0100]},
    { EMI_AVG,     4,    256,     0,     4, (UINT32)&IPE_EDTab[CAPISO0200]},
    { EMI_AVG,     8,    256,     1,     4, (UINT32)&IPE_EDTab[CAPISO0400]},
    { EMI_AVG,     8,    256,     2,     4, (UINT32)&IPE_EDTab[CAPISO0800]},
    { EMI_AVG,    12,    256,     3,     4, (UINT32)&IPE_EDTab[CAPISO1600]}
};

EdgeMap IPE_ESymMap[Total_ISONum] =
{
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO050]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0100]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0200]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0400]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO0800]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO1600]},
    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO3200]},

    { EMI_USD,     0,     0,     0,     0, (UINT32)&IPE_ESymTab[PRVISO6400]},

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
    {{    2,     0,       8,       16,    _3x3}, {   2,     0,       8,       16,    _3x3}, {   2,     0,       8,       16,    _3x3}},
    {{    2,     0,       8,       16,    _3x3}, {   2,     0,       8,       16,    _3x3}, {   2,     0,       8,       16,    _3x3}},
    {{    4,     0,       8,       16,    _3x3}, {   4,     0,       8,       16,    _3x3}, {   4,     0,       8,       16,    _3x3}},
    {{    4,     0,       8,       16,    _5x5}, {   4,     0,       8,       16,    _5x5}, {   4,     0,       8,       16,    _5x5}},
    {{    4,     0,       8,       16,    _5x5}, {   4,     0,       8,       16,    _5x5}, {   4,     0,       8,       16,    _5x5}},
    {{    4,     0,       8,       16,    _5x5}, {   4,     0,       8,       16,    _5x5}, {   4,     0,       8,       16,    _5x5}},
    {{    4,     0,       8,       16,    _5x5}, {   4,     0,       8,       16,    _5x5}, {   4,     0,       8,       16,    _5x5}},

    {{    6,     0,       8,       16,    _5x5}, {   6,     0,       8,       16,    _5x5}, {   6,     0,       8,       16,    _5x5}},

    //rlpfw,rsonlyw,rrangeth0,rrangeth1,rlpfsize,    glpfw,gsonlyw,grangeth0,grangeth1,glpfsize,    blpfw,bsonlyw,brangeth0,brangeth1,blpfsize
    {{    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}, {    4,     0,       8,       16,    _3x3}},
    {{    6,     0,       8,       16,    _3x3}, {    6,     0,       8,       16,    _3x3}, {    6,     0,       8,       16,    _3x3}},
    {{    7,     0,       8,       16,    _5x5}, {    7,     0,       8,       16,    _5x5}, {    7,     0,       8,       16,    _5x5}},
    {{    8,     0,       8,       16,    _7x7}, {    8,     0,       8,       16,    _7x7}, {    8,     0,       8,       16,    _7x7}},
    {{   10,     0,       8,       16,    _7x7}, {   10,     0,       4,       16,    _7x7}, {   10,     0,       8,       16,    _7x7}},
};

UINT16 Normal_CC[9] = {
   /* 0x271,0x61f,0x6f,
    0x736,0x246,0x784,
    0x49,0x65b,0x25c,
    0x018d,0x0742,0x0030,
    0x07b9,0x0183,0x07c4,
    0x07fb,0x074e,0x01b7,*/
0x21a,0x6a3,0x42,
0x777,0x1e2,0x7a7,
0x7de,0x708,0x21b,
};
UINT16 Normal_Day[9] = {
   /* 0x271,0x61f,0x6f,
    0x736,0x246,0x784,
    0x49,0x65b,0x25c,
    0x018d,0x0742,0x0030,
    0x07b9,0x0183,0x07c4,
    0x07fb,0x074e,0x01b7,*/
0x21a,0x6a3,0x42,
0x777,0x1e2,0x7a7,
0x7de,0x708,0x21b,

};
UINT16 Normal_CC_night[9] = {
   /* 0x271,0x61f,0x6f,
    0x736,0x246,0x784,
    0x49,0x65b,0x25c,
    0x018d,0x0742,0x0030,
    0x07b9,0x0183,0x07c4,
    0x07fb,0x074e,0x01b7,*/
0x21a,0x6a3,0x42,
0x777,0x1e2,0x7a7,
0x7de,0x708,0x21b,
};

//FSTAB
UINT8 CC_fmStab[Total_ISONum][16]=
{
    {16,8,0,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {16,8,0,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {16,8,0,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,96,255},
    {80,56,32,16,0,0,0,0,0,0,0,0,0,32,96,255},

    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,96,255},

    {32,16,8,0,0,0,0,0,0,0,0,0,0,32,96,255},
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
    128,128,128,128,128,128,128,128
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
    128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128
};

UINT8 IPE_ddsTab[8]=
{
    8,16,32,32,32,32,32,32
};

IPE_RAND_NR IPE_NoiseParam = { TRUE, 0, 0};


UINT32 GammaLUT_128Tab[130]={
0,21,40,60,86,111,134,163,191,217,246,272,293,314,335,353,375,390,406,419,440,454,467,
485,496,506,520,533,543,559,569,578,587,595,603,611,617,624,634,640,648,656,664,668,676,
684,688,696,700,708,716,720,728,732,740,748,752,760,764,772,779,781,789,791,800,804,808,
812,816,820,824,828,832,836,840,844,848,852,856,860,864,868,872,876,880,884,888,892,896,
896,900,904,908,912,912,916,920,924,924,928,932,936,936,940,944,948,952,952,956,960,964,
964,968,972,976,980,980,984,988,992,992,996,1000,1004,1008,1008,1012,1016,1023
/*
0,23,45,69,95,120,150,174,203,227,253,272,293,314,335,353,375,390,406,419,440,454,467,485,496,506,520,
533,543,559,569,578,587,595,603,611,617,624,634,640,648,656,664,668,676,684,688,696,700,708,716,720,728,
732,740,748,752,760,764,772,779,781,789,791,800,804,808,812,816,820,824,828,832,836,840,844,848,852,856,
860,864,868,872,876,880,884,888,892,896,896,900,904,908,912,912,916,920,924,924,928,932,936,936,940,944,
948,952,952,956,960,964,964,968,972,976,980,980,984,988,992,992,996,1000,1004,1008,1008,1012,1016,1023

0, 32, 68, 100, 136, 160, 184, 208, 236, 260,
284, 304, 320, 336, 352, 368, 380, 396, 412, 428,
444, 456, 468, 480, 488, 500, 508, 520, 528, 540,
548, 560, 568, 580, 588, 600, 608, 620, 628, 640,
652, 656, 664, 668, 676, 684, 688, 696, 700, 708,
716, 720, 728, 732, 740, 748, 752, 760, 764, 772,
780, 784, 792, 796, 800, 804, 808, 812, 816, 820,
824, 828, 832, 836, 840, 844, 848, 852, 856, 860,
864, 868, 872, 876, 880, 884, 888, 892, 896, 896,
900, 904, 908, 912, 912, 916, 920, 924, 924, 928,
932, 936, 936, 940, 944, 948, 952, 952, 956, 960,
964, 964, 968, 972, 976, 980, 980, 984, 988, 992,
992, 996, 1000, 1004, 1008, 1008, 1012, 1016, 1023

0,23,45,69,95,120,150,174,203,227,253,272,293,314,335,353,375,390,406,419,440,454,467,485,496,506,520,
533,543,559,569,578,587,595,603,611,617,624,634,640,648,656,664,668,676,684,688,696,700,708,716,720,728,
732,740,748,752,760,764,772,779,781,789,791,800,804,808,812,816,820,824,828,832,836,840,844,848,852,856,
860,864,868,872,876,880,884,888,892,896,896,900,904,908,912,912,916,920,924,924,928,932,936,936,940,944,
948,952,952,956,960,964,964,968,972,976,980,980,984,988,992,992,996,1000,1004,1008,1008,1012,1016,1023
0,44,86,125,159,190,218,243,265,286,304,
321,337,352,366,380,394,407,420,433,445,
457,469,480,491,502,512,523,533,543,552,
562,571,580,589,597,606,614,622,630,637,
645,652,659,666,672,679,685,691,697,703,
709,715,720,726,731,736,742,747,752,757,
762,767,771,776,780,785,790,794,799,803,
807,812,816,820,824,828,833,836,840,844,
848,852,856,859,863,867,871,875,878,882,
886,890,894,898,902,906,910,914,918,922,
925,929,933,937,940,944,947,951,954,958,
961,965,968,972,975,979,982,986,990,993,
997,1001,1004,1008,1012,1016,1019,1023,
*/
};
UINT32 IQS_GammaLut_Night[130]=
{
0,44,86,125,159,190,218,243,265,286,304,
321,337,352,366,380,394,407,420,433,445,
457,469,480,491,502,512,523,533,543,552,
562,571,580,589,597,606,614,622,630,637,
645,652,659,666,672,679,685,691,697,703,
709,715,720,726,731,736,742,747,752,757,
762,767,771,776,780,785,790,794,799,803,
807,812,816,820,824,828,833,836,840,844,
848,852,856,859,863,867,871,875,878,882,
886,890,894,898,902,906,910,914,918,922,
925,929,933,937,940,944,947,951,954,958,
961,965,968,972,975,979,982,986,990,993,
997,1001,1004,1008,1012,1016,1019,1023,

};

INT8 Saturation[Total_ISONum][SEL_SATURATION_MAX_CNT] = {
    {-20,    0,   20},//50
    {-20,    0,   20},//100
    {-20,    0,   20},//200
    {-20,  -10,   20},//400
    {-20,  -15,   20},//800
    {-20,  -20,   20},//1600
    {-25,  -25,   20},//3200

    {-20,    0,   20},//6400

   //N1,  Normal,  P1
    {-20,    0,   20},//100
    {-20,    0,   20},//200
    {-20,    0,   20},//400
    {-20,    0,   20},//800
    {-20,    0,   20},//1600
};

/*
INT8 Contrast[Total_ISONum][2] = {
    //Y,  C
    {15, 10},
    {10, 10},
    { 5,  5},
    { 0,  0},
    { 0,  0},
    { 0,  0},

    { 0,  0},

    { 0,  0},
    { 0,  0},
    { 0,  0},
    { 0,  0},
    { 0,  0},
};
*/

UINT8 Sharpness[Total_ISONum][SEL_SHARPNESS_MAX_CNT] = {
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

    { 1,       0,       0},

    //IFE2_CC, IFE2_V, MFR
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       0,       0},
    { 1,       0,       1},
    { 1,       0,       1},
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
      {  10, { 3,  7},      0, IFE2_FLTR_7X7},
      {  10, { 3,  7},      0, IFE2_FLTR_7X7},
      {  10, { 4,  9},      0, IFE2_FLTR_7X7},
      {  10, { 5, 11},      0, IFE2_FLTR_7X7},
      {  10, { 6, 13},      0, IFE2_FLTR_7X7},
      {  10, { 7, 15},      0, IFE2_FLTR_7X7},
      {  10, { 9, 18},      0, IFE2_FLTR_7X7},

      {  10, { 9, 18},      0, IFE2_FLTR_7X7},

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
      {  10, { 3,  7},      0, IFE2_FLTR_7X7},
      {  10, { 3,  7},      0, IFE2_FLTR_7X7},
      {  10, { 4,  9},      0, IFE2_FLTR_7X7},
      {  10, { 5, 11},      0, IFE2_FLTR_7X7},
      {  10, { 6, 13},      0, IFE2_FLTR_7X7},
      {  10, { 7, 15},      0, IFE2_FLTR_7X7},
      {  10, { 9, 18},      0, IFE2_FLTR_7X7},

      {  10, { 9, 18},      0, IFE2_FLTR_7X7},

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
    {7,4,2,0}
};


IME_3D_NR IME_3DNR[Total_ISONum] =
{
    //ThresY[3], WeightY[4], ThresCb[3], WeightCb[4], ThresCr[3], WeightCr[4],
     { IME_3DNR_ThresY[PRVISO050], IME_3DNR_WeightY[PRVISO050], IME_3DNR_ThresCC[PRVISO050], IME_3DNR_WeightCC[PRVISO050], IME_3DNR_ThresCC[PRVISO050], IME_3DNR_WeightCC[PRVISO050]},
     { IME_3DNR_ThresY[PRVISO0100], IME_3DNR_WeightY[PRVISO0100], IME_3DNR_ThresCC[PRVISO0100], IME_3DNR_WeightCC[PRVISO0100], IME_3DNR_ThresCC[PRVISO0100], IME_3DNR_WeightCC[PRVISO0100]},
     { IME_3DNR_ThresY[PRVISO0200], IME_3DNR_WeightY[PRVISO0200], IME_3DNR_ThresCC[PRVISO0200], IME_3DNR_WeightCC[PRVISO0200], IME_3DNR_ThresCC[PRVISO0200], IME_3DNR_WeightCC[PRVISO0200]},
     { IME_3DNR_ThresY[PRVISO0400], IME_3DNR_WeightY[PRVISO0400], IME_3DNR_ThresCC[PRVISO0400], IME_3DNR_WeightCC[PRVISO0400], IME_3DNR_ThresCC[PRVISO0400], IME_3DNR_WeightCC[PRVISO0400]},
     { IME_3DNR_ThresY[PRVISO0800], IME_3DNR_WeightY[PRVISO0800], IME_3DNR_ThresCC[PRVISO0800], IME_3DNR_WeightCC[PRVISO0800], IME_3DNR_ThresCC[PRVISO0800], IME_3DNR_WeightCC[PRVISO0800]},
     { IME_3DNR_ThresY[PRVISO1600], IME_3DNR_WeightY[PRVISO1600], IME_3DNR_ThresCC[PRVISO1600], IME_3DNR_WeightCC[PRVISO1600], IME_3DNR_ThresCC[PRVISO1600], IME_3DNR_WeightCC[PRVISO1600]},
     { IME_3DNR_ThresY[PRVISO3200], IME_3DNR_WeightY[PRVISO3200], IME_3DNR_ThresCC[PRVISO3200], IME_3DNR_WeightCC[PRVISO3200], IME_3DNR_ThresCC[PRVISO3200], IME_3DNR_WeightCC[PRVISO3200]},

     { IME_3DNR_ThresY[PRVISO6400], IME_3DNR_WeightY[PRVISO6400], IME_3DNR_ThresCC[PRVISO6400], IME_3DNR_WeightCC[PRVISO6400], IME_3DNR_ThresCC[PRVISO6400], IME_3DNR_WeightCC[PRVISO6400]},

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
};

IME_CHRA_ADAP IME_Chroma_Adapt_param[Total_ISONum] =
{
    {Precision_4,      6,       6,       0,         160, (UINT32)IME_LCATbl[PRVISO050], 20},
    {Precision_4,      6,       6,       0,         160, (UINT32)IME_LCATbl[PRVISO0100], 20},
    {Precision_4,      6,       6,       0,         176, (UINT32)IME_LCATbl[PRVISO0200], 20},
    {Precision_4,      6,       6,       0,         192, (UINT32)IME_LCATbl[PRVISO0400], 20},
    {Precision_4,      6,       6,       0,         216, (UINT32)IME_LCATbl[PRVISO0800], 20},
    {Precision_4,     12,      12,       0,         250, (UINT32)IME_LCATbl[PRVISO1600], 20},
    {Precision_4,     18,      18,       0,         250, (UINT32)IME_LCATbl[PRVISO3200], 20},

    {Precision_4,     12,      12,       0,         250, (UINT32)IME_LCATbl[PRVISO6400], 20},

    //LcaLutPre, Thres_Cb, Thres_Cr, Thres_Lut, LcaWeight, LcaLUT_TblAddr,      SubRatio
    {Precision_4,    255,      255,      0,         160, (UINT32)IME_LCATbl[CAPISO0100], 20},
    {Precision_4,    255,      255,      0,         176, (UINT32)IME_LCATbl[CAPISO0200], 20},
    {Precision_4,    255,      255,      0,         192, (UINT32)IME_LCATbl[CAPISO0400], 20},
    {Precision_4,    255,      255,      0,         216, (UINT32)IME_LCATbl[CAPISO0800], 20},
    {Precision_4,    255,      255,      0,         250, (UINT32)IME_LCATbl[CAPISO1600], 20},
};

UINT32 IME_ChromaFilt_rngth_cb[Total_ISONum][4] =
{
    { 1, 3, 5, 7},//50
    { 1, 3, 5, 7},//100
    { 1, 3, 5, 7},//200
    { 2, 4, 6, 8},//400
    { 3, 5, 7, 9},//800
    { 4, 6, 8,10},//1600
    { 4, 6, 8,10},//3200

    { 4, 6, 8,10},

    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
    { 1, 3, 5, 7},
};
UINT32 IME_ChromaFilt_rngth_cr[Total_ISONum][4] =
{
    { 1, 3, 5, 7},//50
    { 1, 3, 5, 7},//100
    { 1, 3, 5, 7},//200
    { 2, 4, 6, 8},//400
    { 3, 5, 7, 9},//800
    { 4, 6, 8,10},//1600
    { 4, 6, 8,10},//3200

    { 4, 6, 8,10},

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
    { 7, 3, 2, 1, 0},
    { 7, 3, 2, 1, 0},
    { 7, 3, 2, 1, 0},
    { 7, 5, 3, 1, 0},
};

IME_CHRA_FILT IME_Chroma_Filter_param[Total_ISONum] =
{
    //RangeFiltEn, RangeFilt, SpaceFilt
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO050], IME_ChromaFilt_rngth_cr[PRVISO050], IME_ChromaFilt_rngWt[PRVISO050]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0100], IME_ChromaFilt_rngth_cr[PRVISO0100], IME_ChromaFilt_rngWt[PRVISO0100]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0200], IME_ChromaFilt_rngth_cr[PRVISO0200], IME_ChromaFilt_rngWt[PRVISO0200]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0400], IME_ChromaFilt_rngth_cr[PRVISO0400], IME_ChromaFilt_rngWt[PRVISO0400]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO0800], IME_ChromaFilt_rngth_cr[PRVISO0800], IME_ChromaFilt_rngWt[PRVISO0800]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO1600], IME_ChromaFilt_rngth_cr[PRVISO1600], IME_ChromaFilt_rngWt[PRVISO1600]}, {2, 1, 32}},
    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO3200], IME_ChromaFilt_rngth_cr[PRVISO3200], IME_ChromaFilt_rngWt[PRVISO3200]}, {2, 1, 32}},

    {TRUE,         {IME_ChromaFilt_rngth_cb[PRVISO6400], IME_ChromaFilt_rngth_cr[PRVISO6400], IME_ChromaFilt_rngWt[PRVISO6400]}, {2, 1, 32}},

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
