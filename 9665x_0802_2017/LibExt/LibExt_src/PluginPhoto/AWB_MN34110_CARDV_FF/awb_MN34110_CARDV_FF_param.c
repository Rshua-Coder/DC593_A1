/**
    Auto Exposure parameter.

    ae parameter.

    @file       ae_sample_param.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "type.h"
#include "Awb_alg.h"
#include "Awb_MN34110_CARDV_FF_int.h"

#if 0
#define AWB_CT_8500K_RGAIN 423
#define AWB_CT_8500K_BGAIN 285

#define AWB_CT_7000K_RGAIN 440
#define AWB_CT_7000K_BGAIN 354

#define AWB_CT_5500K_RGAIN 457
#define AWB_CT_5500K_BGAIN 423

#define AWB_CT_5000K_RGAIN 390
#define AWB_CT_5000K_BGAIN 421

#define AWB_CT_4000K_RGAIN 318
#define AWB_CT_4000K_BGAIN 520

#define AWB_CT_2500K_RGAIN 246
#define AWB_CT_2500K_BGAIN 619
#else
#define AWB_CT_8500K_RGAIN 454
#define AWB_CT_8500K_BGAIN 317

#define AWB_CT_7000K_RGAIN 455
#define AWB_CT_7000K_BGAIN 356

#define AWB_CT_5500K_RGAIN 391
#define AWB_CT_5500K_BGAIN 416

//#define AWB_CT_5000K_RGAIN 390
//#define AWB_CT_5000K_BGAIN 421

#define AWB_CT_4000K_RGAIN 365
#define AWB_CT_4000K_BGAIN 590

#define AWB_CT_3000K_RGAIN 270
#define AWB_CT_3000K_BGAIN 677

#define AWB_CT_2500K_RGAIN 237
#define AWB_CT_2500K_BGAIN 711
#endif
//#NT#2013/09/05#Spark Chou -begin
//#NT#
#if AWB_REFWHITE_REMAP
AWBALG_ELEMET WhiteIndoorElement[] =
{
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (160<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     255,  86, {  5, 10}, {  5, 10}},
    {AWBALG_TYPE_BG,     180, 140, {  5, 10}, {  5, 10}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteOutdoorElement[] =
{
    {AWBALG_TYPE_Y,      (160<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     141,  86, {  5, 10}, {  5, 10}},
    {AWBALG_TYPE_BG,     180, 140, {  5, 10}, {  5, 10}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

#else
//0.97, 0.03, 0.07
AWBALG_ELEMET WhiteIndoorElement[] =
{
#if 0
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     196,  72, {  6, 15}, {  3,  7}},
    {AWBALG_TYPE_BG,     296, 124, {  8, 18}, {  4,  9}},
    {AWBALG_TYPE_RBG,    366,  72, {  7, 16}, {  1,  4}},
    {AWBALG_TYPE_RBGSUM, 392, 279, { 11, 26}, { 10, 24}},
    {AWBALG_TYPE_RXBG,   325, 191, { 10, 23}, {  8, 20}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
    //oriignal
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     198,  81, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     309, 125, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    379,  71, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 398, 283, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   330, 204, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
#endif
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     204,  82, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     304, 123, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    330,  71, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 409, 279, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   358, 213, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteOutdoorElement[] =
{
    #if 0
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     196, 115, {  6, 15}, {  3,  7}},
    {AWBALG_TYPE_BG,     207, 124, {  8, 18}, {  4,  9}},
    {AWBALG_TYPE_RBG,    161,  72, {  7, 16}, {  1,  4}},
    {AWBALG_TYPE_RBGSUM, 378, 279, { 11, 26}, { 10, 24}},
    {AWBALG_TYPE_RXBG,   325, 214, { 10, 23}, {  8, 20}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
    //original
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     196, 156, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     183, 125, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    115,  71, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 381, 283, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   330, 219, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
    #endif
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     204, 127, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     253, 153, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    177,  79, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 409, 301, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   358, 221, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}

};
#endif
//#NT#2013/09/05#Spark Chou -end

AWBALG_TAB AwbWhiteTAB[AWB_MAX_WHITE_TAB] =
{
    {(1<<10),  (1<<4),  WhiteIndoorElement,  AWBALG_ENV_InDoor},
    {(1<<28),  (1<<14), WhiteOutdoorElement, AWBALG_ENV_OutDoor}
};

AWBALG_PDLTAB AwbPDLTAB[AWB_MAX_PDL_TAB]=
{

};

//#NT#2013/09/05#Spark Chou -begin
//#NT#
AWBALG_CT_PARAM AwbCTParam =
{
    (1<<14),      // HCTMaxEV; LV14
    (1<<10),      // HCTMinEV; LV8
    {AWB_CT_5500K_RGAIN, AWB_CT_5500K_BGAIN},   // HCTMinGain; 5500K
    {AWB_CT_8500K_RGAIN, AWB_CT_8500K_BGAIN},   // HCTMaxGain; 8500K
    (1<<14),      // LCTMaxEV; LV14
    (1<<10),      // LCTMinEV; LV8
    {AWB_CT_4000K_RGAIN, AWB_CT_4000K_BGAIN},   // LCTMaxGain; 4000K
    {AWB_CT_2500K_RGAIN, AWB_CT_2500K_BGAIN},   // LCTMinGain; 2300K
};

AWBALG_POSTPROC_PARAM AwbPostParam =
{
    100,  100,        // HCTRatio; //100~200
    {AWB_CT_7000K_RGAIN, AWB_CT_7000K_BGAIN},   // HCTMaxGain;
    {AWB_CT_8500K_RGAIN, AWB_CT_8500K_BGAIN},   // HCTMinGain;
    120,  90,        // LCTRatio;  //100~200
    {AWB_CT_3000K_RGAIN, AWB_CT_3000K_BGAIN},   // LCTMaxGain; 4500K
    {AWB_CT_2500K_RGAIN, AWB_CT_2500K_BGAIN},   // LCTMinGain; 2556K
};
//#NT#2013/09/05#Spark Chou -end

//const UINT32 MwbTAB[AWB_MODE_MAX][2] =
UINT32 MwbTAB[AWB_MODE_MAX][2] =
{
    { 256, 256},
    { 438, 297},//AWB_MODE_DAYLIGHT
    { 375, 340},//AWB_MODE_CLOUDY
    { (197*17/10), (583*7/10)},//AWB_MODE_TUNGSTEN
    { 322, 409},//AWB_MODE_FLUORESCENT1
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256}
};


