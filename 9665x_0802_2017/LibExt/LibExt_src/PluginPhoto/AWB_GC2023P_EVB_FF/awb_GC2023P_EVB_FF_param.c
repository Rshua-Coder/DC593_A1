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
#include "Awb_GC2023P_EVB_FF_int.h"

#define AWB_CT_7500K_RGAIN 316
#define AWB_CT_7500K_BGAIN 274

#define AWB_CT_6000K_RGAIN 328
#define AWB_CT_6000K_BGAIN 330

#define AWB_CT_5000K_RGAIN 276
#define AWB_CT_5000K_BGAIN 343

#define AWB_CT_3000K_RGAIN 153  //227 //*
#define AWB_CT_3000K_BGAIN 554//865

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
AWBALG_ELEMET WhiteNightElement[] =
{
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     143,  52, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     348,  96, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    650,  77, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 394, 206, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   206, 111, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};
//0.97, 0.03, 0.07
AWBALG_ELEMET WhiteIndoorElement[] =
{
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     143,  52, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     249,  96, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    427,  77, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 313, 206, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   188, 111, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteOutdoorElement[] =
{
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     143, 96, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     151, 96, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    140,  77, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 287, 206, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   188, 117, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};
#endif


AWBALG_TAB AwbWhiteTAB[AWB_MAX_WHITE_TAB] =
{
    {(1<<4), (1<<2),     WhiteNightElement,  AWBALG_ENV_Night},
    {(1<<10),  (1<<6),  WhiteIndoorElement,  AWBALG_ENV_InDoor},
    {(1<<28),  (1<<14), WhiteOutdoorElement, AWBALG_ENV_OutDoor}
};

AWBALG_PDLTAB AwbPDLTAB[AWB_MAX_PDL_TAB]=
{


};

AWBALG_CT_PARAM AwbCTParam =
{
    (1<<14),      // HCTMaxEV; LV14
    (1<<10),      // HCTMinEV; LV8
    {AWB_CT_6000K_RGAIN, AWB_CT_6000K_BGAIN},   // HCTMinGain; 5500K
    {AWB_CT_7500K_RGAIN, AWB_CT_7500K_BGAIN},   // HCTMaxGain; 8500K
    (1<<14),      // LCTMaxEV; LV14
    (1<<10),      // LCTMinEV; LV8
    {AWB_CT_5000K_RGAIN, AWB_CT_5000K_BGAIN},   // LCTMaxGain; 4000K
    {AWB_CT_3000K_RGAIN, AWB_CT_3000K_BGAIN},   // LCTMinGain; 2300K
};

AWBALG_POSTPROC_PARAM AwbPostParam =
{
    100,  100,        // HCTRatio; //100~200
    {AWB_CT_6000K_RGAIN, AWB_CT_6000K_BGAIN},   // HCTMaxGain;
    {AWB_CT_7500K_RGAIN, AWB_CT_7500K_BGAIN},   // HCTMinGain;
    100,  100,        // LCTRatio;  //100~200
    {AWB_CT_5000K_RGAIN, AWB_CT_5000K_BGAIN},   // LCTMaxGain; 4500K
    {AWB_CT_3000K_RGAIN, AWB_CT_3000K_BGAIN},   // LCTMinGain; 2556K
};

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


