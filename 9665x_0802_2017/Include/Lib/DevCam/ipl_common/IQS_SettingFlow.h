/**
    Internal header file for IPP_sensor

    Header files for differnt IPP_sensor

    @file       IQS_SettingFlow.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IQS_SETTINGFLOW_H
#define _IQS_SETTINGFLOW_H

#include <string.h>
#include <stdio.h>

#include "IPL_AlgInfor.h"
#include "IPL_Hal_SIE.h"
#include "IPL_Hal_PRE.h"
#include "IPL_Hal_IFE.h"
#include "IPL_Hal_DCE.h"
#include "IPL_Hal_IPE.h"
#include "IPL_Hal_IFE2.h"
#include "IPL_Hal_IME.h"

#include "IPL_Hal_SIE_Info.h"
#include "IPL_Hal_PRE_Info.h"
#include "IPL_Hal_IFE_Info.h"
#include "IPL_Hal_DCE_Info.h"
#include "IPL_Hal_IPE_Info.h"
#include "IPL_Hal_IFE2_Info.h"
#include "IPL_Hal_IME_Info.h"


#include "Type.h"
#include "IPL_Hal.h"


typedef struct
{
    Coordinate  R;  ///<center of R
    Coordinate  Gr; ///<center of Gr
    Coordinate  Gb; ///<center of Gb
    Coordinate  B;  ///<center of B
} IQS_PRE_CENTER;

typedef struct
{
    BOOL    FuncEn;     ///<dc extraction enable
    UINT32  Th1;        ///<dc extraction threshold 1
    UINT32  Th2;        ///<dc extraction threshold 2
    UINT32  Mul;            ///<distance gain. unsigned 2.6
    UINT32  T;              ///<disabled area
    UINT32  TabGain;        ///<gain factor
    UINT32  XDIV;           ///<scaling factor in x direction. unsigned 0.6
    UINT32  YDIV;           ///<scaling factor in y direction. unsigned 0.6
    BOOL    bDither;        ///<dithering enable/disable
    BOOL    DitherRstEn;    ///<dithering reset enable
} IQS_PRE_VIGLUT_PARAM;

typedef struct
{
    IQS_PRE_CENTER Center;
    IQS_PRE_VIGLUT_PARAM VigLutParam;
}IQS_PRE_VIG;

typedef struct
{
    UINT8 OUTLER;
} IQS_IFE_FUNC;


typedef struct
{
    UINT8 CbOfs;    ///< Legal range : 0~255, Cb color offset, 128 : NO color offset
    UINT8 CrOfs;    ///< Legal range : 0~255, Cr color offset, 128 : NO color offset
    UINT8 YCon;    ///< Legal range : 0~255, Y contrast adjust, 128 : NO contrst adjust, Y' = (Y * Ycon) >> 7
    UINT8 CCon;    ///< Legal range : 0~255, CbCr contrast adjust, 128 : NO contrst adjust, C' = (C * CCon) >> 7
    INT16 IntOfs;    ///< Legal range : -128~127, Y offset, Y' = Y + Intofs
    INT16 SatOfs;   ///< Legal range : -128~127, color saturation adjust, 0 : NO color saturation adjust
    BOOL  HueRotateEn; ///< Hue adjust rotation option, 0 : NO rotation, 1 : CHUEM[n] LSB 2 bit = 0 : 0 degree rotation, 1 : 90 degree rotation, 2 bit = 2 : 180 degree rotation, 3 : 270 degree rotation
} IQS_COLOR_EFFECT_PARAM;


typedef struct
{
    //YTH1
    UINT32  EdgeThres;
    UINT32  Threshold;
    BOOL    HIT_FixOutputEn;        ///< replace output as HIT_value when (input>=Threshold)&&(edge>=EdgeThres), if enabled
    BOOL    NONEHIT_FixOutputEn;    ///< replace output as NONEHIT_value when (input<Threshold)||(edge<EdgeThres), if enabled
    UINT32  HIT_value;
    UINT32  NONHIT_value;
} IQS_IE_COLORPENCIL_PARAM;

typedef struct
{
    //YTH1
    UINT32  EdgeThres;
    UINT32  Threshold;
    BOOL    HIT_FixOutputEn;        ///< replace output as HIT_value when (input>=Threshold)&&(edge>=EdgeThres), if enabled
    BOOL    NONEHIT_FixOutputEn;    ///< replace output as NONEHIT_value when (input<Threshold)||(edge<EdgeThres), if enabled
    UINT32  HIT_value;
    UINT32  NONHIT_value;

    UINT8   CbOfs;    ///< Legal range : 0~255, Cb color offset, 128 : NO color offset
    UINT8   CrOfs;    ///< Legal range : 0~255, Cr color offset, 128 : NO color offset
    UINT8   CCon;    ///< Legal range : 0~255, CbCr contrast adjust, 128 : NO contrst adjust, C' = (C * CCon) >> 7
    BOOL    EInvP; ///< Legal range : 0~1, if EInvP is 1, positive edge(overshoot) is inverted to negative edge(undershoot)
    BOOL    EInvN; ///< Legal range : 0~1, if EInvN is 1, negative edge(undershoot) is inverted to positive edge(overshoot)
} IQS_IE_PENCILSKETCH_PARAM;

typedef struct
{
    UINT32 LCATimes;
    UINT32 RTXTimes;
    UINT32 MFRTimes;
} IQS_IFE2_FILTER_TIMES;

typedef enum
{
    VIG_TAB_GAIN_1X,
    VIG_TAB_GAIN_2X
} IQS_VIG_TAB_GAIN;

typedef enum
{
    IQCB_PRV_START = 0,
    IQCB_PRV_SIE_INIT,
    IQCB_PRV_PRE_INIT,
    IQCB_PRV_IPE_INIT,
    IQCB_PRV_IFE_INIT,
    IQCB_PRV_DCE_INIT,
    IQCB_PRV_IFE2_INIT_CC,
    IQCB_PRV_IME_INIT,
    IQCB_PRV_IFE2_CC,
    IQCB_PRV_IME_LCA,
    IQCB_PRV_IME_RETINEX,
    IQCB_PRV_IFE2_RETINEX,
    IQCB_PRV_END,

    IQCB_CAP_QV_START = 100,
    IQCB_PREIME_D2D_PRE_QV,     //For QV & PRI, pre--> Ime
    IQCB_PREIME_D2D_DCE_QV,
    IQCB_PREIME_D2D_IFE_QV,
    IQCB_PREIME_D2D_IPE_QV,
    IQCB_PREIME_D2D_IME_QV,  //disbale LCA
    IQCB_D2D_IME_QV,                 //For QV postprocess
    IQCB_D2D_IME_QVREF,
    IQCB_D2D_IFE2_QV_Y,
    IQCB_D2D_IFE2_QV_CC,    
    IQCB_CAP_QV_END,

    IQCB_CAP_PRI_START = 200,    
    IQCB_PREIME_D2D_PRE_PRI,   
    IQCB_PREIME_D2D_DCE_PRI,
    IQCB_PREIME_D2D_IFE_PRI,    
    IQCB_PREIME_D2D_IPE_PRI,
    IQCB_PREIME_D2D_IME_PRI, //enable LCA
    IQCB_D2D_IFE2_PRI_Y,
    IQCB_D2D_IFE2_PRI_CC,    
    IQCB_CAP_PRI_END,
    
    IQS_FLOW_START = 300,
    IQS_AE_END,
    IQS_AWB_END,
    IQS_AF_START,
    IQS_AF_END,
    IQS_OZOOM_END,
    IQS_DZOOM_END,
    IQS_DISCOMP_ON,
    IQS_DISCOMP_OFF,
    IQS_IMAGE_EFFECT,
    IQS_MANUAL_TRIG,
    IQS_REORG_APPINFO,
    IQS_FLOW_MSG_NUM
}IQS_FLOW_MSG;

ER IQS_SettingFlowFunc(IQS_FLOW_MSG MsgID);
ER IQCB_SettingFlowFunc(IQS_FLOW_MSG MsgID, ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);

#endif // EOF
