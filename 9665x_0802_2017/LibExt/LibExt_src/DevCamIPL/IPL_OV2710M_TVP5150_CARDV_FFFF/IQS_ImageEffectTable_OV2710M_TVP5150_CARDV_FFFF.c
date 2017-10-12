#ifndef _IQS_IE_SETTINGTABLE_OV2710M_TVP5150_CARDV_FFFF_C
#define _IQS_IE_SETTINGTABLE_OV2710M_TVP5150_CARDV_FFFF_C
#include "IPL_OV2710M_TVP5150_CARDV_FFFF_Int.h"

IQS_COLOR_EFFECT_PARAM ColorEffectTable[SEL_IMAGEEFFECT_MAX_CNT + 1] = {
  //CbOfs CrOfs YCon CCon IntOfs SatOfs HueRotate
    {128, 128,  128, 128,   0,     0,   FALSE },    //SEL_IMAGEEFFECT_OFF,
    {128, 128,  128,   0,   0,     0,   FALSE },    //SEL_IMAGEEFFECT_BW,
    {108, 146,  128,   0,   0,     0,   FALSE },    //SEL_IMAGEEFFECT_SEPIA,
    {128, 128,  128, 128,   0,    50,   FALSE },    //SEL_IMAGEEFFECT_VIVID,
    {128, 128,  128, 128,   0,     0,   FALSE },    //SEL_IMAGEEFFECT_NEUTRAL,
    {128, 128,  128, 128,   0,     0,   FALSE },    //SEL_IMAGEEFFECT_LOWSHARPING,
    {128, 128,  128, 128,   0,     0,   FALSE },    //SEL_IMAGEEFFECT_FASION1,
    {128, 128,  128, 128,   0,     0,   FALSE },    //SEL_IMAGEEFFECT_FASION2,
    {128, 128,  128, 128,   0,     0,   FALSE },    //SEL_IMAGEEFFECT_CUSTOMMANUAL
    {128, 128,  128, 128,   0,     0,   FALSE },    //SEL_IMAGEEFFECT_RED,
    {128, 128,  128, 128,   0,     0,   FALSE },    //SEL_IMAGEEFFECT_GREEN,
    {128, 128,  128, 128,   0,     0,   FALSE },    //SEL_IMAGEEFFECT_BLUE,
    {128, 128,  128, 128,   0,     0,   FALSE },    //SEL_IMAGEEFFECT_YELLOW,
    {128, 128,  128, 128,   0,     0,   FALSE },    //SEL_IMAGEEFFECT_PURPLE,

    {128, 128,  128, 128,   0,     0,   FALSE },    //
    {128, 128,  128, 128,   0,     0,   FALSE },    //
    {128, 128,  128, 128,   0,     0,   FALSE },    //
    {128, 128,  128, 128,   0,     0,   FALSE }     //
};

//////////////////////////////////
// Color pencil
//////////////////////////////////
IQS_IE_COLORPENCIL_PARAM IE_ColorPencil_Param[Total_ISONum] =
{
    { 5, 0, ENABLE, ENABLE, 0, 192},
    { 5, 0, ENABLE, ENABLE, 0, 192},
    { 5, 0, ENABLE, ENABLE, 0, 192},
    { 5, 0, ENABLE, ENABLE, 0, 192},
    { 5, 0, ENABLE, ENABLE, 0, 192},

    { 5, 0, ENABLE, ENABLE, 0, 192},
    { 5, 0, ENABLE, ENABLE, 0, 192},
    { 5, 0, ENABLE, ENABLE, 0, 192},
    { 5, 0, ENABLE, ENABLE, 0, 192},
    { 5, 0, ENABLE, ENABLE, 0, 192},
};

//////////////////////////////////
// Sketch
//////////////////////////////////
IQS_IE_PENCILSKETCH_PARAM IE_PencilSketch_Param[Total_ISONum] =
{
    //{Yth1,                          CbOfs, Crofs, CCon,  EInvP, EinvN
    { 10, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 10, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 10, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 10, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 10, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},


    //{Yth1,                          CbOfs, Crofs, CCon,  EInvP, EinvN
    { 10, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 10, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 10, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 10, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 10, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
};

//////////////////////////////////
// Fisheye
//////////////////////////////////
UINT32 IE_FISYEYE_LUT[65] =
{
#if 1
0x7fff, 0x8ffe, 0x969f, 0x9bb5, 0x9ffe, 0xa3c5, 0xa72f, 0xaa53,
0xad3f, 0xaffd, 0xb296, 0xb50e, 0xb76a, 0xb9ad, 0xbbdb, 0xbdf5,
0xbffd, 0xc1f5, 0xc3df, 0xc5bb, 0xc78a, 0xc94f, 0xcb08, 0xccb8,
0xce5f, 0xcffc, 0xd192, 0xd320, 0xd4a6, 0xd626, 0xd79f, 0xd912,
0xda7e, 0xdbe6, 0xdd47, 0xdea4, 0xdffc, 0xe14f, 0xe29d, 0xe3e7,
0xe52d, 0xe66f, 0xe7ad, 0xe8e7, 0xea1d, 0xeb50, 0xec80, 0xedac,
0xeed5, 0xeffb, 0xf11e, 0xf23e, 0xf35c, 0xf476, 0xf58e, 0xf6a4,
0xf7b7, 0xf8c7, 0xf9d5, 0xfae1, 0xfbea, 0xfcf2, 0xfdf7, 0xfefa, 0xfffe //, 0x0000,
#else
    30918,32985,34931,36763,38488,40112,41641,43080,
    44435,45711,46912,48043,49108,50110,51054,51942,
    52726,53458,54144,54784,55384,55943,56465,56952,
    57407,57830,58224,58591,58932,59249,59543,59816,
    60069,60302,60517,60716,60961,61193,61410,61615,
    61808,61989,62160,62322,62473,62616,62750,62876,
    62995,63107,63212,63311,63406,63494,63576,63654,
    63728,63797,63862,63923,63981,64036,64087,64135,
    64180 //,64223
#endif
};


//////////////////////////////////
// WDR
//////////////////////////////////
IME_RETINEX IME_RetInex[IQ_HDR_LEVEL_MAX] =
{
  //GL_GAIN,GD_GAIN,LL_GAIN,LD_GAIN,R_GAIN,G_GAIN,B_GAIN,ExGain, SubRatio;
  //{ 0x400,   0x3CC,  0x4CC,   0x4CC,   0x499,   0x499,    0x499,   0x000,  40},
  {1024,   1024,  1024,   1024,   1024,   1024,   1024,   20},
  {1044,    963,  1055,    993,   1024,   1024,   1024,   20},
  {1044,    963,  1055,    993,   1024,   1024,   1024,   20},
  {1065,    922,  1085,    963,   1024,   1024,   1024,   20},
  {1106,    840,  1147,    901,   1024,   1024,   1024,   20},
  {1126,    799,  1178,    870,   1024,   1024,   1024,   20},
  {1147,    758,  1208,    840,   1024,   1024,   1024,   20},
  {1188,    676,  1270,    778,   1024,   1024,   1024,   20},
  {1208,    635,  1300,    748,   1024,   1024,   1024,   20},
  {1229,    594,  1331,    717,   1024,   1024,   1024,   20}
};

//IFE2
UINT32 AvgFilter_avgTh_WDR[Total_ISONum][2] =
{
        { 5, 10},
        { 5, 10},
        { 5, 10},
        { 5, 10},
        { 5, 10},

        { 5, 10},
        { 5, 10},
        { 5, 10},
        { 5, 10},
        { 5, 10},
};

AvgFilt IFE2_AvgFilter_WDR[Total_ISONum] =
{
    //cenWt, avgTh,                                      cntTh, fltSize
      {  32, (UINT32)&AvgFilter_avgTh_WDR[PRVISO0100],       1, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[PRVISO0200],       1, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[PRVISO0400],       1, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[PRVISO0800],       1, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[PRVISO1600],       1, IFE2_FLTR_7X7},


    //cenWt, avgTh,                                      cntTh, fltSize
      {  32, (UINT32)&AvgFilter_avgTh_WDR[CAPISO0100],       1, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[CAPISO0200],       1, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[CAPISO0400],       1, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[CAPISO0800],       1, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[CAPISO1600],       1, IFE2_FLTR_7X7},
};

#endif
