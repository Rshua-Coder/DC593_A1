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
#include "Awb_AR0330M_C12_int.h"

#define CT_7500K_RGAIN 357
#define CT_7500K_BGAIN 263
#define CT_6000K_RGAIN 418
#define CT_6000K_BGAIN 394
#define CT_5000K_RGAIN 370
#define CT_5000K_BGAIN 441
#define CT_4000K_RGAIN 312
#define CT_4000K_BGAIN 565
#define CT_3000K_RGAIN 225
#define CT_3000K_BGAIN 751

#define PEACH_ALG_OUTDOOR_EV_VALUE 4096 //EV12
#define PEACH_ALG_INDOOR_EV_VALUE  1024 //EV10
#define PEACH_ALG_MAX_EV_VALUE     (1<<17)
#define PEACH_ALG_MIN_EV_VALUE     (1<<4)

/*
#define AWBALG_GAIN_BASE    100
    DataInfo.Rgain = AWBALG_GAIN_BASE * DataInfo.G / DataInfo.R;
    DataInfo.Bgain = AWBALG_GAIN_BASE * DataInfo.G / DataInfo.B;
    DataInfo.RBgain = AWBALG_GAIN_BASE * DataInfo.R / DataInfo.B;
    DataInfo.RBgainSum = DataInfo.Rgain + DataInfo.Bgain;
    DataInfo.Sat = AWBALG_Sat(DataInfo.R, DataInfo.G, DataInfo.B);
    DataInfo.RxBgain = DataInfo.Rgain * DataInfo.Bgain / AWBALG_GAIN_BASE;
    DataInfo.RBgainDiff = AWBALG_ABS((int)(DataInfo.Rgain-DataInfo.Bgain));
AWBALG_ELEMET WhiteIndoorElement[] =
{
    {AWBALG_TYPE_Y,      200,   5, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RG,     208,  82, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_BG,     390, 124, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RBG,    423,  70, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RBGSUM, 491, 258, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RXBG,   380, 240, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RBGDIF, 288,   5, { 0, 0}, { 0, 0}},
    {AWBALG_TAB_END,       0,   0, { 0, 0}, { 0, 0}}
};

*/

//#NT#2016/08/03#Jarkko Chang -begin
//#NT# update AWB threshold
AWBALG_ELEMET WhiteNightElement[] =
{
    {AWBALG_TYPE_Y,      220,   6, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RG,     205, 65, { 0, 0}, { 0, 0}},    //189,65, Org
    {AWBALG_TYPE_BG,     375, 116, { 0, 0}, { 0, 0}},   //360,116, Org
    {AWBALG_TYPE_RBG,    415,  18, { 0, 0}, { 0, 0}},   //399,18, Org
    {AWBALG_TYPE_RBGSUM, 443, 255, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RXBG,   385, 170, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RBGDIF, 588,   0, { 0, 0}, { 0, 0}},
    {AWBALG_TAB_END,       0,   0, { 0, 0}, { 0, 0}}
};

AWBALG_ELEMET WhiteIndoorElement[] =
{
    {AWBALG_TYPE_Y,      220,   8, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RG,     205,  74, { 0, 0}, { 0, 0}},   //189,74, Org
    {AWBALG_TYPE_BG,     375, 116, { 0, 0}, { 0, 0}},   //345,116, Org
    {AWBALG_TYPE_RBG,    415,  72, { 0, 0}, { 0, 0}},   //399,72, Org
    {AWBALG_TYPE_RBGSUM, 443, 255, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RXBG,   385, 177, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RBGDIF, 588,   0, { 0, 0}, { 0, 0}},
    {AWBALG_TAB_END,       0,   0, { 0, 0}, { 0, 0}}
};
//#NT#2016/08/03#Jarkko Chang -end

AWBALG_ELEMET WhiteOutdoorElement[] =
{
    {AWBALG_TYPE_Y,      200,   10, { 0, 0}, { 0, 0}},
    {AWBALG_TYPE_RG,     189, 124, { 0, 0}, { 0, 0}},//191 100
    {AWBALG_TYPE_BG,     193, 117, { 0, 0}, { 0, 0}},//247 116
    {AWBALG_TYPE_RBG,    140,  72, { 0, 0}, { 0, 0}},//149 72
    {AWBALG_TYPE_RBGSUM, 377, 253, { 0, 0}, { 0, 0}},//360 255
    {AWBALG_TYPE_RXBG,   340, 177, { 0, 0}, { 0, 0}},//328 180
    {AWBALG_TYPE_RBGDIF, 594, 0, { 0, 0}, { 0, 0}},
    {AWBALG_TAB_END,       0,   0, { 0, 0}, { 0, 0}}
};

AWBALG_TAB AwbWhiteTAB[AWB_MAX_WHITE_TAB] =
{
    {(1<<4), (1<<2),     WhiteNightElement,  AWBALG_ENV_Night},
    {PEACH_ALG_INDOOR_EV_VALUE, (1<<6),     WhiteIndoorElement,  AWBALG_ENV_InDoor},
    {PEACH_ALG_MAX_EV_VALUE,    PEACH_ALG_OUTDOOR_EV_VALUE, WhiteOutdoorElement, AWBALG_ENV_OutDoor}
};

AWBALG_PDLTAB AwbPDLTAB[AWB_MAX_PDL_TAB]=
{
//rgain, bgain, Range()
//=====, =====, ==========, =========================
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
    {CT_7500K_RGAIN, CT_7500K_BGAIN},   // HCTMaxGain;
    {CT_5000K_RGAIN, CT_5000K_BGAIN},   // HCTMinGain;
    16384,                              // LCTMaxEV;
    1024,                               // LCTMinEV;
    {CT_4000K_RGAIN, CT_4000K_BGAIN},   // LCTMaxGain;
    {CT_3000K_RGAIN, CT_3000K_BGAIN},   // LCTMinGain;
};

AWBALG_POSTPROC_PARAM AwbPostParam =
{
    100, 100,                           // HCTRatio; //100~200
    {CT_7500K_RGAIN, CT_7500K_BGAIN},   // HCTMaxGain;
    {CT_5000K_RGAIN, CT_5000K_BGAIN},   // HCTMinGain;
    105, 85,                               // LCTRatio;  //100~200
    {CT_4000K_RGAIN, CT_4000K_BGAIN},   // LCTMaxGain;
    {CT_3000K_RGAIN, CT_3000K_BGAIN},   // LCTMinGain;
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


