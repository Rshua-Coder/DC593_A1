#ifndef IQS_SETTINGTABLE_AR0330_NT99141_MSDEMO_FFFF_INT_H
#define IQS_SETTINGTABLE_AR0330_NT99141_MSDEMO_FFFF_INT_H
#include "IQS_SettingFlow.h"

typedef enum
{
    PRVISO0100,
    PRVISO0200,
    PRVISO0400,
    PRVISO0800,
    PRVISO1600,
    CAPISO0100,
    CAPISO0200,
    CAPISO0400,
    CAPISO0800,
    CAPISO1600,
    Total_ISONum
}IQS_ISONumEnum;

typedef enum
{
    ZOOMNUM0,
    ZOOMNUM1,
    ZOOMNUM2,
    Total_ZOOMNum
}IQS_ZOOMNumEnum;

typedef enum
{
    SENMODENUM0,
    Total_SENMODENum
}IQS_SENMODENumEnum;

//UINT8 ShadingLut[Total_SENMODENum][Total_ZOOMNum][17];
//PRE_VIG PRE_ShadingTable[Total_SENMODENum][Total_ZOOMNum];
extern UINT8 PRE_ShadingLut[Total_SENMODENum][Total_ZOOMNum][17];
extern IQS_PRE_CENTER PRE_ShadingCenter[Total_SENMODENum];
extern IQS_PRE_VIGLUT_PARAM PRE_ShadingParam[Total_SENMODENum];

extern IQS_IFE_FUNC IFE_Func[Total_ISONum];
extern IFE_OUTL IFE_Outl_param[Total_ISONum];
extern IFE_FILT IFE_Filter_param[Total_ISONum];
extern IFE_CRVMAP IFE_CurveMap_param_ycc;
extern UINT32 IFE_CurveLut_ycc[65];
extern UINT32 IFE_rngth_NLM[Total_ISONum][4];
extern UINT32 IFE_rngth_Bil[Total_ISONum][4];

extern Fact_Norm   DCE_Radious[Total_SENMODENum];
extern Coordinate DCE_CenterXY[Total_SENMODENum];
extern DIST DCE_Dist[Total_SENMODENum];
extern FOV DCE_Fov[Total_SENMODENum][Total_ZOOMNum];
extern EnH DCE_Enh[Total_SENMODENum][Total_ZOOMNum];
extern Fact_Norm DCE_NormTerm[Total_SENMODENum];
extern Aberation DCE_ColorAbbration[Total_SENMODENum][Total_ZOOMNum];
extern UINT32 DCE_2D_LUT[Total_SENMODENum][Total_ZOOMNum][65];

extern UINT32 IPE_ColorGain_Offset[Total_ISONum][4];
extern IPE_GBAL IPE_Gbal[Total_ISONum];
extern IPE_CFAINTER IPE_CFAInter[Total_ISONum];
extern IPE_EDGEEXT IPE_EdgeExt[Total_ISONum];
extern INT16 IPE_edgeKernel[Total_ISONum][11];
extern UINT8 IPE_EDirTab[Total_ISONum][8];
extern IPE_EDGEENH IPE_EdgeEnh[Total_ISONum];
extern UINT8 IPE_EDTab[Total_ISONum][16];
extern UINT8 IPE_ESymTab[Total_ISONum][16];
extern UINT8 IPE_EStab[Total_ISONum][16];
//EdgeMap IPE_ESMap[Total_ISONum];
//EdgeMap IPE_ESymMap[Total_ISONum];
extern EdgeMap IPE_EDMap[Total_ISONum];
extern EdgeMap IPE_ESymMap[Total_ISONum];
//IPE_EdgeMap IPE_edgeMap[Total_ISONum];
extern IPE_RGBLPF IPE_RGBLpf[Total_ISONum];
extern UINT16 Normal_CC[9];
extern UINT8 CC_fmStab[Total_ISONum][16];
extern UINT8 CC_fmDtab[Total_ISONum][16];
extern UINT8 IPE_hueTab[24];
extern INT32 IPE_satTab[24];
extern INT32 IPE_intTab[24];
extern UINT8 IPE_edgTab[24];
extern UINT8 IPE_ddsTab[8];
extern IPE_RAND_NR IPE_NoiseParam;
extern UINT32 GammaLUT_128Tab[130];
extern UINT32 IPE_3DCCLut[900];
extern INT8 Saturation[Total_ISONum][SEL_SATURATION_MAX_CNT];
extern UINT8 Sharpness[Total_ISONum][SEL_SHARPNESS_MAX_CNT];
extern IPE_YCCFIX IPE_YCFixEffect;
extern IPE_EDGETHRE IPE_EdgeThre[Total_ISONum];
extern IPE__VA IPE_VAParam[Total_ISONum];
extern IQS_IFE2_FILTER_TIMES IFE2_Filter_Times[Total_ISONum];
extern IFE2_Chroma IFE2_Chroma_param[Total_ISONum];
extern AvgFilt IFE2_AvgFilter_cb[Total_ISONum];
extern AvgFilt IFE2_AvgFilter_cr[Total_ISONum];

extern IME_3D_NR IME_3DNR[Total_ISONum];
extern IME_CHRA_ADAP IME_Chroma_Adapt_param[Total_ISONum];
extern IME_CHRA_FILT IME_Chroma_Filter_param[Total_ISONum];

extern SIE_GAMMA SieRawEncTab;
extern UINT32 RawDecLut[129];

extern AvgFilt IFE2_AvgFilter_y_mfr[Total_ISONum];
extern IME_RETINEX IME_mfr[Total_ISONum];
#endif

