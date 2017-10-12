/**
    Auto Exposure parameter.

    ae parameter.

    @file       awb_sample_param.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "type.h"
#include "Awb_alg.h"
#include "Awb_OV2710_EVB_FF_int.h"




#define PEACH_ALG_OUTDOOR_EV_VALUE 4096 //EV12
#define PEACH_ALG_INDOOR_EV_VALUE  1024 //EV10
#define PEACH_ALG_MAX_EV_VALUE     (1<<26)
#define PEACH_ALG_MIN_EV_VALUE     (1<<4)

AWBALG_ELEMET WhiteIndoorElement[] =
{
    {AWBALG_TYPE_Y,      200,   5, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RG,     157,  62, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_BG,     250, 120, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RBG,    363,  94, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RBGSUM, 335, 231, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RXBG,   254, 129, { 0, 0}, { 0, 0}},
    {AWBALG_TAB_END,       0,   0, { 0, 0}, { 0, 0}}
};

AWBALG_ELEMET WhiteOutdoorElement[] =
{
    {AWBALG_TYPE_Y,      200,   5, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RG,     157,  62, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_BG,     250, 120, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RBG,    363,  94, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RBGSUM, 335, 231, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RXBG,   254, 129, { 0, 0}, { 0, 0}},
    {AWBALG_TAB_END,       0,   0, { 0, 0}, { 0, 0}}
};

AWBALG_TAB AwbWhiteTAB[AWB_MAX_WHITE_TAB] =
{
    {PEACH_ALG_INDOOR_EV_VALUE, PEACH_ALG_MIN_EV_VALUE,     WhiteIndoorElement,  AWBALG_ENV_InDoor},
    {PEACH_ALG_MAX_EV_VALUE,    PEACH_ALG_OUTDOOR_EV_VALUE, WhiteOutdoorElement, AWBALG_ENV_OutDoor}
};

AWBALG_PDLTAB AwbPDLTAB[AWB_MAX_PDL_TAB]=
{
//rgain, bgain, Range()
//=====, =====, ==========, =========================
//{180, 500, 20},
//{ 535,   483,   36,}, //_EX_D},
//{ 415,   535,   34,}, //_EX_N},
//{ 368,   687,   39,}, //_TL84},
//{ 388,   577,   35,}, //_DNP},
//{ 612,   490,   40,}, //_Office},
//{ 451,   454,   20,}, //DNP
//{570, 359, 20},  //6900K
//{496, 426, 20},  //5000K
//{421, 626, 20},  //3786K
//{337, 628, 20},  //3239K
//{275, 743, 20},  //2460K

};

AWBALG_CT_PARAM AwbCTParam =
{
    16384,                              // HCTMaxEV;
    1024,                               // HCTMinEV;
    {9999, 256},   // HCTMaxGain;
    {9999, 256},   // HCTMinGain;
    16384,                              // LCTMaxEV;
    1024,                               // LCTMinEV;
    {0, 256},   // LCTMaxGain;
    {0, 256},   // LCTMinGain;
};

AWBALG_POSTPROC_PARAM AwbPostParam =
{
    100,  100,                              // HCTRatio; //100~200
    {334, 351},   // HCTMaxGain;
    {365, 414},   // HCTMinGain;
    120,  120,                              // LCTRatio;  //100~200
    {300, 420},   // LCTMaxGain;
    {176, 582},   // LCTMinGain;
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


