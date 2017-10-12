#ifndef _IQS_IE_SETTINGTABLE_IMX083_C
#define _IQS_IE_SETTINGTABLE_IMX083_C
#include "IPL_IMX078CQK_Sample_FF_Int.h"

IQS_COLOR_EFFECT_PARAM ColorEffectTable[SEL_IMAGEEFFECT_MAX_CNT + 1] = {
  //CbOfs CrOfs YCon CCon IntOfs SatOfs HueRotate
    {128, 128,  128, 128,   0,    10,   FALSE },    //SEL_IMAGEEFFECT_OFF,
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
    { 5, 0, ENABLE, ENABLE, 0, 192},
};

//////////////////////////////////
// Sketch
//////////////////////////////////
IQS_IE_PENCILSKETCH_PARAM IE_PencilSketch_Param[Total_ISONum] =
{
    //{Yth1,                          CbOfs, Crofs, CCon,  EInvP, EinvN
    { 5, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 5, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 5, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 5, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 5, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},

    //{Yth1,                          CbOfs, Crofs, CCon,  EInvP, EinvN
    { 5, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 5, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 5, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 5, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},
    { 5, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},

    { 5, 0, ENABLE, ENABLE, 0, 255,   128,   128,    0, ENABLE, DISABLE},

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
0x7fff, 0x8ffe, 0x969f, 0x9bb5, 0x9ffe, 0xa3c5, 0xa72f, 0xaa53,
0xad3f, 0xaffd, 0xb296, 0xb50e, 0xb76a, 0xb9ad, 0xbbdb, 0xbdf5,
0xbffd, 0xc1f5, 0xc3df, 0xc5bb, 0xc78a, 0xc94f, 0xcb08, 0xccb8,
0xce5f, 0xcffc, 0xd192, 0xd320, 0xd4a6, 0xd626, 0xd79f, 0xd912,
0xda7e, 0xdbe6, 0xdd47, 0xdea4, 0xdffc, 0xe14f, 0xe29d, 0xe3e7,
0xe52d, 0xe66f, 0xe7ad, 0xe8e7, 0xea1d, 0xeb50, 0xec80, 0xedac,
0xeed5, 0xeffb, 0xf11e, 0xf23e, 0xf35c, 0xf476, 0xf58e, 0xf6a4,
0xf7b7, 0xf8c7, 0xf9d5, 0xfae1, 0xfbea, 0xfcf2, 0xfdf7, 0xfefa, 0xfffe //, 0x0000,
};


//////////////////////////////////
// WDR
//////////////////////////////////
IME_RETINEX IME_RetInex[HDR_LEVEL_MAX] =
{
  //GL_GAIN,GD_GAIN,LL_GAIN,LD_GAIN,R_GAIN,G_GAIN,B_GAIN,ExGain, SubRatio;
  //{ 0x400,   0x3CC,  0x4CC,   0x4CC,   0x499,   0x499,    0x499,   0x000,  40},

  { 0x466,   0x352,  0x49a,   0x49a,   0x4cd,   0x4cd,    0x4cd,   0x000,  25},

  { 0x400,   0x366,  0x4CC,   0x4CC,   0x499,   0x499,    0x499,   0x000,  40},
  { 0x499,   0x333,  0x500,   0x500,   0x500,   0x500,    0x500,   0x000,  25},
  { 0x500,   0x300,  0x533,   0x533,   0x533,   0x533,    0x533,   0x000,  25},
  { 0x533,   0x2CC,  0x566,   0x566,   0x599,   0x599,    0x599,   0x000,  19},
  { 0x599,   0x299,  0x5CC,   0x5CC,   0x5CC,   0x5CC,    0x5CC,   0x000,  19},
  { 0x599,   0x299,  0x633,   0x633,   0x699,   0x699,    0x699,   0x000,  16},
  { 0x599,   0x299,  0x533,   0x533,   0x733,   0x733,    0x733,   0x400,  12}
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
      {  32, (UINT32)&AvgFilter_avgTh_WDR[PRVISO0100],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[PRVISO0200],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[PRVISO0400],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[PRVISO0800],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[PRVISO1600],       0, IFE2_FLTR_7X7},

    //cenWt, avgTh,                                      cntTh, fltSize
      {  32, (UINT32)&AvgFilter_avgTh_WDR[VQ2ISO0100],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[VQ2ISO0200],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[VQ2ISO0400],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[VQ2ISO0800],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[VQ2ISO1600],       0, IFE2_FLTR_7X7},

      {  32, (UINT32)&AvgFilter_avgTh_WDR[VQ2ISO1600L],       0, IFE2_FLTR_7X7},

    //cenWt, avgTh,                                      cntTh, fltSize
      {  32, (UINT32)&AvgFilter_avgTh_WDR[QVISO0100],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[QVISO0200],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[QVISO0400],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[QVISO0800],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[QVISO1600],       0, IFE2_FLTR_7X7},

    //cenWt, avgTh,                                      cntTh, fltSize
      {  32, (UINT32)&AvgFilter_avgTh_WDR[CAPISO0100],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[CAPISO0200],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[CAPISO0400],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[CAPISO0800],       0, IFE2_FLTR_7X7},
      {  32, (UINT32)&AvgFilter_avgTh_WDR[CAPISO1600],       0, IFE2_FLTR_7X7},
};

#endif
