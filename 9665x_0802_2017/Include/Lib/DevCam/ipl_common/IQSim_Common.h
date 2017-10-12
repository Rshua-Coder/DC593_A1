#ifndef _IQSIM_COMMON_H_
#define _IQSIM_COMMON_H_
/**
    IQSim_COMMON.h


    @file       IQSim_COMMON.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "type.h"
#include "ErrorNo.h"

#include "iqs_settingflow.h"

typedef struct
{
    ISIZE Size;
    UINT32 BitNum;
    IPL_RAW_PIX cfa;
} IQSIM_RAW_INFO;


typedef struct
{
    ISIZE Size;
    UINT32 BitRate;
} IQSIM_JPG_INFO;

typedef struct
{
    IQSIM_RAW_INFO RawInfo;

    ////////////////////////////////////////////////
    //PRE
    ////////////////////////////////////////////////
    PreFuncEn  PRE_FuncEn;
    UINT8 PRE_ShadingLut[1][1][17];
    IQS_PRE_CENTER PRE_ShadingCenter[1];
    IQS_PRE_VIGLUT_PARAM PRE_ShadingParam[1];

    ////////////////////////////////////////////////
    // IFE
    ////////////////////////////////////////////////
    IfeFuncEn  IFE_FuncEn;
    UINT32 IFE_ColorGain[4];
    UINT32 IFE_Outl_Bri[1][5];
    UINT32 IFE_Outl_Dark[1][5];
    IFE_OUTL IFE_Outl_param[1];
    UINT32 IFE_weights[1][6];
    UINT32 IFE_rngth_NLM[1][4];
    UINT32 IFE_rngth_Bil[1][4];
    IFE_FILT IFE_Filter_param[1];
    UINT32 IFE_CurveLut_ycc[65];
    IFE_CRVMAP IFE_CurveMap_param_ycc;

    ////////////////////////////////////////////////
    // DCE
    ////////////////////////////////////////////////
    Fact_Norm   DCE_Radious[1];
    Coordinate DCE_CenterXY[1];
    DIST DCE_Dist[1];
    FOV DCE_Fov[1][1];
    EnH DCE_Enh[1][1];
    Aberation DCE_ColorAbbration[1][1];
    UINT32 DCE_2D_LUT[1][1][65];

    ////////////////////////////////////////////////
    // IPE
    ////////////////////////////////////////////////
    IPEFUNCEN IPE_FuncEn;
    UINT32 IPE_ColorGain[4];
    UINT8 IPE_ColorGain_Offset[4];
    UINT8 IPE_Gbal_Stab[1][16];
    UINT8 IPE_Gbal_Dtab[1][16];
    IPE_GBAL IPE_Gbal[1];
    IPE_CFAINTER IPE_CFAInter[1];
    INT16 IPE_edgeKernel[1][11];
    UINT8 IPE_EDirTab[1][8];
    IPE_DEE IPE_DirEdgeExt[1];
    IPE_ESD IPE_ESymDiff[1];
    IPE_EDGEEXT IPE_EdgeExt[1];
    IPE_EDGEENH IPE_EdgeEnh[1];
    UINT8 IPE_EDTab[1][16]; //EDLut
    UINT8 IPE_ESymTab[1][16];    //ESYMLUTL0
    UINT8 IPE_EStab[1][16];
    EdgeMap IPE_EDMap[1];
    EdgeMap IPE_ESymMap[1];
    IPE_RGBLPF IPE_RGBLpf[1];
    UINT16 Normal_CC[9];
    UINT8 CC_fmStab[1][16];
    UINT8 CC_fmDtab[1][16];
    UINT8 IPE_hueTab[24];
    INT32 IPE_satTab[24];
    INT32 IPE_intTab[24];
    UINT8 IPE_edgTab[24];
    UINT8 IPE_ddsTab[8];
    IPE_RAND_NR IPE_NoiseParam;
    UINT32 GammaLUT_128Tab[130];
    IQS_COLOR_EFFECT_PARAM ColorEffectTable[1];
    UINT32 IPE_VdetDiv[1];

    //For DIS
    IPE_EDGETHRE IPE_EdgeThre[1];
    IPE_YCCFIX IPE_YCFixEffect;
    IPE__VA IPE_VAParam[1];

    ////////////////////////////////////////////////
    // IFE2
    ////////////////////////////////////////////////
    IFE2FuncEn IFE2_Y_FuncEn;
    IFE2FuncEn IFE2_CC_FuncEn;
    IFE2_Chroma IFE2_Chroma_param[1];
    AvgFilt IFE2_AvgFilter_y[1];
    AvgFilt IFE2_AvgFilter_cb[1];
    AvgFilt IFE2_AvgFilter_cr[1];
    IQS_IFE2_FILTER_TIMES IFE2_Filter_Times[1];

    ////////////////////////////////////////////////
    // IME
    ////////////////////////////////////////////////
    ImeFuncEn IME_FuncEn;
    UINT32 IME_3DNR_ThresY[1][3];
    UINT32 IME_3DNR_WeightY[1][4];
    UINT32 IME_3DNR_ThresCC[1][3];
    UINT32 IME_3DNR_WeightCC[1][4];
    IME_3D_NR IME_3DNR[1];
    UINT32 IME_LCATbl[1][33];
    IME_CHRA_ADAP IME_Chroma_Adapt_param[1];
    UINT32 IME_ChromaFilt_rngth_cb[1][4];
    UINT32 IME_ChromaFilt_rngth_cr[1][4];
    UINT32 IME_ChromaFilt_rngWt[1][5];
    IME_CHRA_FILT IME_Chroma_Filter_param[1];
    IME_RETINEX IME_RetInex;

    ////////////////////////////////////////////////
    // 3D CC
    ////////////////////////////////////////////////
    UINT32 IPE_3DCCLut[900];
    UINT16 RawEncLut[129];
    SIE_GAMMA SieRawEncTab;
    UINT32 RawDecLut[129];
} IQSIM_CHINFO;


typedef struct
{
    char version[32];
    IQSIM_JPG_INFO IQSimJpgInfo;    
    IQSIM_CHINFO IQSimQVInfo;
    IQSIM_CHINFO IQSimPriInfo;    
} IQSIM_APPINFO;

IQSIM_CHINFO* IQSIM_GetChInfo(IQS_FLOW_MSG MsgID);
IQSIM_APPINFO* IQSIM_GetAppInfo(void);
void IQSIM_SetAppInfo(UINT32 BinAddr);
void IQSIM_ReorgAppInfo(void);
void IQSIM_SetRawAddr(UINT32 ch0, UINT32 ch2);
void IQSIM_GetRawAddr(UINT32 *ch0, UINT32 *ch2);

#endif //_IQS_UTILITY_H_
