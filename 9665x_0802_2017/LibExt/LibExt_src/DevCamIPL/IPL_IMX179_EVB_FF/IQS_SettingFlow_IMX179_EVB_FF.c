#ifndef _IQS_SETTINGFLOW_IMX179_EVB_FF_C
#define _IQS_SETTINGFLOW_IMX179_EVB_FF_C
#include "IPL_IMX179_EVB_FF_Int.h"
#include "ae_alg.h"
#include "SceneRenderLib.h"
#include "ImgStatLib.h"


extern UINT32 AE_getCurLum(UINT32 LumBit);

#define SEL_CONTRAST_MAX_CNT 5
INT8 Contrast[SEL_CONTRAST_MAX_CNT] = { 40, 20,  0,  -20, -40 };
UINT32 Contrast_Value = 2;
void IQS_SetContrast(UINT32 value)
{
    if ( value >= SEL_CONTRAST_MAX_CNT)
        return ;

    Contrast_Value = value;
}

PRE_VIG     IQ_PRE_Vig;
DCE_Distortion IQ_DCE_Distortion;

IPE_EDGEEXT     IQ_IPE_EdgeExt;
IPE_EDGEMAP     IQ_IPE_EdgeMap;
IPE_EDGEENH     IQ_IPE_EdgeEnh;
IPE_CC          IQ_IPE_CC;
IPE_CCTRL       IQ_IPE_Cctrl;
IPE_CCOFS       IQ_IPE_CbCrofs;
IPE_CCON        IQ_IPE_YCCcon;
IPE_3DCC        IQ_IPE_3DCC;
IPE_GAMMA       IQ_IPE_RGBGAMMA;
IPE_YCCFIX      IQ_IPE_YCFixEffect;
IPE_CSTP        IQ_IPE_CSTP;
IFE2_AvgFilter  IQ_IFE2_AvgFilter;
UINT32          IQ_IFE_rngth_NLM[4];
UINT32          IQ_IFE_rngth_Bil[4];
IFE_FILT        IQ_IFE_Filter_param;
EdgeMap         IQ_IPE_EDMap;
INT8            IQ_Saturation[SEL_SATURATION_MAX_CNT];
INT16           IQ_Normal_CC[9];
IMESCALE_Filt   IQ_P2Filt;

SceneRender_RTXGainInfo GetRtxGainInfo;
UINT16 WDR_Racc[1024],WDR_Gacc[1024],WDR_Bacc[1024];
void WDR_VideoGainAdjust(SceneRender_RTXGainInfo* GetRtxGainInfo);

///////////////////////////////////////////////
// Body
///////////////////////////////////////////////


static UINT32 IQS_GetCapIsoIndex(void)
{
    UINT32 IsoIdx;

    UINT32 ISO, ExpT, Iris;
    AE_GetCapAEArg(&ISO, &ExpT, &Iris);

    if ( ISO <= 150 )
        IsoIdx =  CAPISO0100;
    else if ( ISO <= 300 )
        IsoIdx =  CAPISO0200;
    else if ( ISO <= 600 )
        IsoIdx =  CAPISO0400;
    else if ( ISO <= 1200 )
        IsoIdx =  CAPISO0800;
    else
        IsoIdx =  CAPISO1600;

    UINT32 UD_ISOIndex, UD_SenModeIndex, UD_ZoomIndex;
    UserDefineIQIndexGet(&UD_ISOIndex, &UD_SenModeIndex, &UD_ZoomIndex);
    if ( UD_ISOIndex != IQS_UD_INDEX_NOSET )
        IsoIdx = UD_ISOIndex;

    if ( IsoIdx >= Total_ISONum )
    {
        IsoIdx = CAPISO0100;
    }
    return IsoIdx;
}


static UINT32 IQS_GetPrvIsoIndex(void)
{
    UINT32 IsoIdx;

    UINT32 ISO, ExpT, Iris;
    AE_GetPrvAEArg(&ISO, &ExpT, &Iris);

    if ( ISO <= 100 )
    {
        IsoIdx =  PRVISO0100;
    }
    else if ( ISO <= 200 )
    {
        IsoIdx =  PRVISO0200;
    }
    else if ( ISO <= 400 )
    {
        IsoIdx =  PRVISO0400;
    }
    else if ( ISO <= 800 )
    {
        IsoIdx =  PRVISO0800;
    }
    else if ( ISO <= 1600 )
    {
        IsoIdx =  PRVISO1600;
    }
    else
    {
        IsoIdx =  PRVISO3200;
    }

    DBG_IND("ISO = %d, Idx = %d\r\n", ISO, IsoIdx);
    UINT32 UD_ISOIndex, UD_SenModeIndex, UD_ZoomIndex;
    UserDefineIQIndexGet(&UD_ISOIndex, &UD_SenModeIndex, &UD_ZoomIndex);
    if ( UD_ISOIndex != IQS_UD_INDEX_NOSET )
        IsoIdx = UD_ISOIndex;

    if ( IsoIdx >= Total_ISONum )
    {
        IsoIdx = PRVISO0100;
    }
    return IsoIdx;
}

static int IQS_Intpl(int Index, int LowValue, int HighValue, int MinIndex, int MaxIndex)
{
    if ( Index <= MinIndex)
        return LowValue;
    else if ( Index >= MaxIndex)
        return HighValue;
    else
        return LowValue + ((HighValue - LowValue)) * ((Index  - MinIndex)) / ((MaxIndex - MinIndex+1));
}


static BOOL IQS_GetIsoRange( UINT32 MsgID, UINT32 *iso, UINT32 *isoL, UINT32 *isoH, UINT32 *ISO_IdxL, UINT32 *ISO_IdxH )
{
    if ( (MsgID >= IQCB_CAP_PRI_START && MsgID <= IQCB_CAP_PRI_END) ||
         (MsgID >= IQCB_CAP_QV_START  && MsgID <= IQCB_CAP_QV_END) )
    {
        return FALSE;
    }

    UINT32 ISO, ExpT, Iris;
    AE_GetPrvAEArg(&ISO, &ExpT, &Iris);
    *iso = ISO;
    if ( *iso <= 100 )
    {
        *isoL = 100;
        *isoH = 100;
        *ISO_IdxL =  PRVISO0100;
        *ISO_IdxH =  PRVISO0100;
    }
    else if ( *iso <= 200 )
    {
        *isoL = 100;
        *isoH = 200;
        *ISO_IdxL =  PRVISO0100;
        *ISO_IdxH =  PRVISO0200;
    }
    else if ( *iso <= 400 )
    {
        *isoL = 200;
        *isoH = 400;
        *ISO_IdxL =  PRVISO0200;
        *ISO_IdxH =  PRVISO0400;
    }
    else if ( *iso <= 800 )
    {
        *isoL = 400;
        *isoH = 800;
        *ISO_IdxL =  PRVISO0400;
        *ISO_IdxH =  PRVISO0800;
    }
    else if ( *iso <= 1600 )
    {
        *isoL = 800;
        *isoH = 1600;
        *ISO_IdxL =  PRVISO0800;
        *ISO_IdxH =  PRVISO1600;
    }
    else
    {
        *isoL = 1600;
        *isoH = 3200;
        *ISO_IdxL =  PRVISO1600;
        *ISO_IdxH =  PRVISO3200;
    }
    return TRUE;
}

static UINT32 IQS_GetISOIdx(IQS_FLOW_MSG MsgID)
{
    UINT32 IsoIdx;

    if (( MsgID >= IQCB_CAP_PRI_START && MsgID <= IQCB_CAP_PRI_END ) || (MsgID >= IQCB_CAP_QV_START  && MsgID <= IQCB_CAP_QV_END))
        IsoIdx = IQS_GetCapIsoIndex();
    else
        IsoIdx = IQS_GetPrvIsoIndex();
    return IsoIdx;
}

static UINT32 IQS_GetZoomIdx(IQS_FLOW_MSG MsgID)
{
    UINT32 ZoomIdx;
#if 0
    ZoomIdx = Lens_Zoom_GetSection(ZOOM_CUR_SECTION);
#endif

    ZoomIdx = ZOOMNUM0;

    UINT32 UD_ISOIndex, UD_SenModeIndex, UD_ZoomIndex;
    UserDefineIQIndexGet(&UD_ISOIndex, &UD_SenModeIndex, &UD_ZoomIndex);
    if ( UD_ZoomIndex != IQS_UD_INDEX_NOSET )
        ZoomIdx = UD_ZoomIndex;

    if ( ZoomIdx >= Total_ZOOMNum )
    {
        ZoomIdx = ZOOMNUM0;
    }

    return ZoomIdx;
}

static UINT32 IQS_GetSenModeIdx(IQS_FLOW_MSG MsgID)
{
    UINT32 SenModeIdx;
    SENSOR_MODE mode;
    if ( MsgID >= IQCB_CAP_PRI_START && MsgID <= IQCB_CAP_PRI_END )
        mode = (SENSOR_MODE)IPL_AlgGetUIInfo(IPL_SEL_CAPSENMODE);
    else
        mode = (SENSOR_MODE)IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE);

    SenModeIdx = SENMODENUM0;

    UINT32 UD_ISOIndex, UD_SenModeIndex, UD_ZoomIndex;
    UserDefineIQIndexGet(&UD_ISOIndex, &UD_SenModeIndex, &UD_ZoomIndex);
    if ( UD_SenModeIndex != IQS_UD_INDEX_NOSET )
        SenModeIdx = UD_SenModeIndex;

    if ( SenModeIdx >= Total_SENMODENum )
    {
        SenModeIdx = SENMODENUM0;
    }
    return SenModeIdx;
}

static ER PRE_IQparam(IQS_FLOW_MSG MsgID, PRE_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    UINT32 ISO_Value, Zoom_Value, SenMode_Value;

    ISO_Value = IQS_GetISOIdx(MsgID);
    Zoom_Value = IQS_GetZoomIdx(MsgID);
    SenMode_Value = IQS_GetSenModeIdx(MsgID);

    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
        case IQCB_PRV_PRE_INIT:
        case IQCB_PREIME_D2D_PRE_PRI:
        case IQCB_PREIME_D2D_PRE_QV:
            //enable function
            subf->preFuncEn |= (
                                 0 //VigEn              ///< enable function: Vignette shading compensation
                                 );

            //Setting parameter
            group->preUpdate |= (
                                PRE_SubFEn      |    ///< update func enable/disable
                                PRE_VIG_
                              );
            if ( MsgID == IQCB_PREIME_D2D_PRE_PRI || MsgID == IQCB_PREIME_D2D_PRE_QV )
                subf->preFuncEn |= VigEn;              ///< enable function: Vignette shading compensation

            //////////////////
            // VIG Gain
            //////////////////
            #if 0
            IQ_PRE_Vig.Center.R     = PRE_ShadingCenter[SenMode_Value].R;
            IQ_PRE_Vig.Center.Gr    = PRE_ShadingCenter[SenMode_Value].Gr;
            IQ_PRE_Vig.Center.Gb    = PRE_ShadingCenter[SenMode_Value].Gb;
            IQ_PRE_Vig.Center.B     = PRE_ShadingCenter[SenMode_Value].B;
            #else
            if ( MsgID == IQCB_PREIME_D2D_PRE_PRI ||  MsgID == IQCB_PREIME_D2D_PRE_QV ) //capture
            {
                IQ_PRE_Vig.Center.R.x = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H) >> 1;
                IQ_PRE_Vig.Center.R.y = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V) >> 1;
                IQ_PRE_Vig.Center.Gr  = IQ_PRE_Vig.Center.R;
                IQ_PRE_Vig.Center.B    = IQ_PRE_Vig.Center.R;
                IQ_PRE_Vig.Center.Gb  = IQ_PRE_Vig.Center.R;
            }
            else  //preview
            {
                IQ_PRE_Vig.Center.R.x = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_PRE_IN_SIZE_H) >> 1;
                IQ_PRE_Vig.Center.R.y = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_PRE_IN_SIZE_V) >> 1;
                IQ_PRE_Vig.Center.Gr  = IQ_PRE_Vig.Center.R;
                IQ_PRE_Vig.Center.B    = IQ_PRE_Vig.Center.R;
                IQ_PRE_Vig.Center.Gb  = IQ_PRE_Vig.Center.R;
            }
            #endif

            IQ_PRE_Vig.DcExt.FuncEn = PRE_ShadingParam[SenMode_Value].FuncEn;
            IQ_PRE_Vig.DcExt.Th1    = PRE_ShadingParam[SenMode_Value].Th1;
            IQ_PRE_Vig.DcExt.Th2    = PRE_ShadingParam[SenMode_Value].Th2;
            IQ_PRE_Vig.Mul          = PRE_ShadingParam[SenMode_Value].Mul;
            IQ_PRE_Vig.T            = PRE_ShadingParam[SenMode_Value].T;
            IQ_PRE_Vig.TabGain      = PRE_ShadingParam[SenMode_Value].TabGain;
            IQ_PRE_Vig.XDIV         = PRE_ShadingParam[SenMode_Value].XDIV;
            IQ_PRE_Vig.YDIV         = PRE_ShadingParam[SenMode_Value].YDIV;
            IQ_PRE_Vig.bDither      = PRE_ShadingParam[SenMode_Value].bDither;
            IQ_PRE_Vig.DitherRstEn  = PRE_ShadingParam[SenMode_Value].DitherRstEn;

            IQ_PRE_Vig.R_LUT_TblAddr = (UINT32)PRE_ShadingLut[Zoom_Value];
            IQ_PRE_Vig.G_LUT_TblAddr = (UINT32)PRE_ShadingLut[Zoom_Value];
            IQ_PRE_Vig.B_LUT_TblAddr = (UINT32)PRE_ShadingLut[Zoom_Value];

            subf->pVig  = &IQ_PRE_Vig;

            break;

        case IQS_OZOOM_END:
            //enable function
            //Setting parameter
            group->preUpdate |= (
                                0 //PRE_VIG_
                              );
            //////////////////
            // VIG Gain
            //////////////////
            #if 0
            IQ_PRE_Vig.Center.R     = PRE_ShadingCenter[SenMode_Value].R;
            IQ_PRE_Vig.Center.Gr    = PRE_ShadingCenter[SenMode_Value].Gr;
            IQ_PRE_Vig.Center.Gb    = PRE_ShadingCenter[SenMode_Value].Gb;
            IQ_PRE_Vig.Center.B     = PRE_ShadingCenter[SenMode_Value].B;
            #else
            IQ_PRE_Vig.Center.R.x = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_PRE_IN_SIZE_H) >> 1;
            IQ_PRE_Vig.Center.R.y = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_PRE_IN_SIZE_V) >> 1;
            IQ_PRE_Vig.Center.Gr  = IQ_PRE_Vig.Center.R;
            IQ_PRE_Vig.Center.B    = IQ_PRE_Vig.Center.R;
            IQ_PRE_Vig.Center.Gb  = IQ_PRE_Vig.Center.R;
            #endif
            IQ_PRE_Vig.DcExt.FuncEn = PRE_ShadingParam[SenMode_Value].FuncEn;
            IQ_PRE_Vig.DcExt.Th1    = PRE_ShadingParam[SenMode_Value].Th1;
            IQ_PRE_Vig.DcExt.Th2    = PRE_ShadingParam[SenMode_Value].Th2;
            IQ_PRE_Vig.Mul          = PRE_ShadingParam[SenMode_Value].Mul;
            IQ_PRE_Vig.T            = PRE_ShadingParam[SenMode_Value].T;
            IQ_PRE_Vig.TabGain      = PRE_ShadingParam[SenMode_Value].TabGain;
            IQ_PRE_Vig.XDIV         = PRE_ShadingParam[SenMode_Value].XDIV;
            IQ_PRE_Vig.YDIV         = PRE_ShadingParam[SenMode_Value].YDIV;
            IQ_PRE_Vig.bDither      = PRE_ShadingParam[SenMode_Value].bDither;
            IQ_PRE_Vig.DitherRstEn  = PRE_ShadingParam[SenMode_Value].DitherRstEn;

            IQ_PRE_Vig.R_LUT_TblAddr = (UINT32)PRE_ShadingLut[SenMode_Value][Zoom_Value];
            IQ_PRE_Vig.G_LUT_TblAddr = (UINT32)PRE_ShadingLut[SenMode_Value][Zoom_Value];
            IQ_PRE_Vig.B_LUT_TblAddr = (UINT32)PRE_ShadingLut[SenMode_Value][Zoom_Value];

            subf->pVig  = &IQ_PRE_Vig;
            break;

        case IQS_AE_END:
        case IQS_AWB_END:
        case IQS_AF_START:
        case IQS_AF_END:
        case IQS_IMAGE_EFFECT:
            break;
        default :
            DBG_ERR("PRE_IQparam() param mode(%d) selection error!\r\n",MsgID);
            return E_SYS;
    }
    return E_OK;
}

static ER DCE_IQparam(IQS_FLOW_MSG MsgID, DCE_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    UINT32 ISO_Value, Zoom_Value, SenMode_Value,IMEffect_Value;

    IMEffect_Value = IPL_AlgGetUIInfo(IPL_SEL_IMAGEEFFECT);
    ISO_Value = IQS_GetISOIdx(MsgID);
    Zoom_Value = IQS_GetZoomIdx(MsgID);
    SenMode_Value = IQS_GetSenModeIdx(MsgID);

    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
        case IQS_OZOOM_END:
        case IQCB_PRV_DCE_INIT:
        case IQCB_PREIME_D2D_DCE_QV:
        case IQCB_PREIME_D2D_DCE_PRI:
        case IQS_IMAGE_EFFECT:
            //enable function

            //Setting parameter
            group->dceUpdate |= (
                                DCE_DisT           | ///< update H/V distance factor/ratio selection
                                DCE_CAB            | ///< update color aberration factor
                                DCE_LUT            | ///< update LUT table address
                                DCE_FOV            | ///< update FOV
                                DCE_ENH            | ///< update enhancement
                                DCE_DISTOR           ///< update distortion parameter set
                              );
            if ( MsgID == IQCB_PREIME_D2D_DCE_QV )
            {
                IQ_DCE_Distortion.Center.x = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H)>>1;
                IQ_DCE_Distortion.Center.y = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V)>>1;
            }
            else if ( MsgID == IQCB_PREIME_D2D_DCE_PRI )
            {
                IQ_DCE_Distortion.Center.x = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H)>>1;
                IQ_DCE_Distortion.Center.y = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V)>>1;
            }
            else  //preview
            {
                IQ_DCE_Distortion.Center.x = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_PRE_IN_SIZE_H)>>1;
                IQ_DCE_Distortion.Center.y = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_PRE_IN_SIZE_V)>>1;
            }
            IQ_DCE_Distortion.Fov = DCE_Fov[Zoom_Value];
            IQ_DCE_Distortion.Enh = DCE_Enh[Zoom_Value];
            IQ_DCE_Distortion.ColorAber = DCE_ColorAbbration[Zoom_Value];
            IQ_DCE_Distortion.LUT_TblAddr = (UINT32)DCE_2D_LUT[Zoom_Value];
            IQ_DCE_Distortion.LutType = DCE_GetTableType(DCE_2D_LUT[Zoom_Value]);
            subf->pDistortion = &IQ_DCE_Distortion;
            subf->Dce_Dist = DCE_Dist[SenMode_Value];
            subf->Radious = DCE_Radious[SenMode_Value];

            switch (IMEffect_Value)
            {
                case SEL_IMAGEEFFECT_FISHEYE:
                    group->dceUpdate |= ( DCE_LUT | DCE_LUTType );
                    IQ_DCE_Distortion.LUT_TblAddr = (UINT32)IE_FISYEYE_LUT;
                    IQ_DCE_Distortion.LutType = NON_STRICTLY_INC_DEC; //DCE_GetTableType(IE_FISYEYE_LUT);
                    subf->pDistortion = &IQ_DCE_Distortion;
                    break;
                default:
                    break;
            }
            break;

        case IQS_AE_END:
        case IQS_AWB_END:
        case IQS_AF_START:
        case IQS_AF_END:
            break;
        default :
            DBG_ERR("DCE_IQparam() param mode(%d) selection error!\r\n",MsgID);
            return E_SYS;
    }
    return E_OK;
}

static ER IFE_IQparam(IQS_FLOW_MSG MsgID, IFE_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    UINT32 ISO_Value, Zoom_Value, SenMode_Value;
    ISO_Value = IQS_GetISOIdx(MsgID);
    Zoom_Value = IQS_GetZoomIdx(MsgID);
    SenMode_Value = IQS_GetSenModeIdx(MsgID);

    UINT32 iso, isoH, isoL, ISO_IdxL, ISO_IdxH, i;
    if ( IQS_GetIsoRange( MsgID, &iso, &isoL, &isoH, &ISO_IdxL, &ISO_IdxH ) == TRUE )
    {
        memcpy(&IQ_IFE_Filter_param, &IFE_Filter_param[ISO_Value], sizeof(IFE_FILT));
        IQ_IFE_Filter_param.Range.A.Delta = IQS_Intpl((int)iso, (int)IFE_Filter_param[ISO_IdxL].Range.A.Delta, (int)IFE_Filter_param[ISO_IdxH].Range.A.Delta, (int)isoL, (int)isoH);
        IQ_IFE_Filter_param.Range.B.Delta = IQS_Intpl((int)iso, (int)IFE_Filter_param[ISO_IdxL].Range.B.Delta, (int)IFE_Filter_param[ISO_IdxH].Range.B.Delta, (int)isoL, (int)isoH);
        IQ_IFE_Filter_param.Range.Bilat_w = IQS_Intpl((int)iso, (int)IFE_Filter_param[ISO_IdxL].Range.Bilat_w, (int)IFE_Filter_param[ISO_IdxH].Range.Bilat_w, (int)isoL, (int)isoH);

        for ( i = 0; i < 4; i++ )
        {
            IQ_IFE_rngth_NLM[i] = IQS_Intpl((int)iso, (int)IFE_rngth_NLM[ISO_IdxL][i], (int)IFE_rngth_NLM[ISO_IdxH][i], (int)isoL, (int)isoH);
            IQ_IFE_rngth_Bil[i] = IQS_Intpl((int)iso, (int)IFE_rngth_Bil[ISO_IdxL][i], (int)IFE_rngth_Bil[ISO_IdxH][i], (int)isoL, (int)isoH);
        }
        //DBG_ERR("%d %d %d %d %d\r\n", iso, ISO_IdxL, ISO_IdxH, isoL, isoH);
        //DBG_ERR("Range %d %d %d\r\n", IQ_IFE_Filter_param.Range.A.Delta, IQ_IFE_Filter_param.Range.B.Delta, IQ_IFE_Filter_param.Range.Bilat_w);
        //DBG_ERR("IQ_IFE_rngth_NLM %d %d %d %d\r\n", IQ_IFE_rngth_NLM[0], IQ_IFE_rngth_NLM[1], IQ_IFE_rngth_NLM[2], IQ_IFE_rngth_NLM[3]);
        //DBG_ERR("IQ_IFE_rngth_Bil %d %d %d %d\r\n", IQ_IFE_rngth_Bil[0], IQ_IFE_rngth_Bil[1], IQ_IFE_rngth_Bil[2], IQ_IFE_rngth_Bil[3]);
    }
    else
    {
        memcpy(IQ_IFE_rngth_NLM, IFE_rngth_NLM[ISO_Value], sizeof(UINT32)*4);
        memcpy(IQ_IFE_rngth_Bil, IFE_rngth_Bil[ISO_Value], sizeof(UINT32)*4);
        memcpy(&IQ_IFE_Filter_param, &IFE_Filter_param[ISO_Value], sizeof(IFE_FILT));
    }
    switch((UINT32)MsgID)
    {
    case IQS_MANUAL_TRIG:
        case IQCB_PRV_IFE_INIT:
        case IQS_AE_END:
        case IQCB_PREIME_D2D_IFE_PRI:
        case IQCB_PREIME_D2D_IFE_QV:
            //enable function
            subf->ifeFuncEn |= FilterEn;
            if ( IFE_Func[ISO_Value].OUTLER == ENABLE )
                subf->ifeFuncEn |= OutlierEn;
            else
                subf->ifeFuncEn &= (~OutlierEn);

            //Setting parameter
            group->ifeUpdate |= (
                                    IFE_SubFEn      |       ///< update func enable/disable
                                    IFE_Outl_       |       ///< update outlier
                                    IFE_Cgain_      |       ///< update color gain
                                    IFE_Filter_            ///< update noise reduction filter
                              );

            //////////////////
            // IFE
            //////////////////
            subf->pOutl    = &IFE_Outl_param[ISO_Value];

            subf->pCrvMap  = &IFE_CurveMap_param_ycc;

            subf->pFilter  = &IQ_IFE_Filter_param;

            subf->pFilter->Range.A.pThres = IQ_IFE_rngth_NLM;
            subf->pFilter->Range.B.pThres = IQ_IFE_rngth_Bil;
            subf->pFilter->Bin = 0;
            subf->CGain.R  = 256;
            subf->CGain.Gr = 256;
            subf->CGain.Gb = 256;
            subf->CGain.B  = 256;
            subf->CGain.IfeGainSel = _2_8;
            subf->CGain.cfa = IPL_RAW_PIX_R;
            break;

        case IQS_AWB_END:
        case IQS_AF_START:
        case IQS_AF_END:
        case IQS_OZOOM_END:
        case IQS_IMAGE_EFFECT:
            break;
        default :
            DBG_ERR("IFE_IQparam() param mode(%d) selection error!\r\n",MsgID);
            return E_SYS;
    }
    return E_OK;
}



static ER IPE_CC_Adjust(INT16 *IQ_CC)
{
#if 0
    UINT32 RGain, GGain, BGain;
    AWB_GetColorGian(&RGain, &GGain, &BGain);

    //7300K
    int  DSP_CCM_HCT_RGAIN  = 570;
    int  DSP_CCM_HCT_BGAIN  = 340;
    double IPE_HCT_CC[9] = {
      1.55078, -0.74219,  0.18750,
     -0.27734,  1.51172, -0.23438,
     -0.01953, -0.69531,  1.71484,
    };

    //6000K
    int  DSP_CCM_MCT_RGAIN  = 540;
    int  DSP_CCM_MCT_BGAIN  = 404;
    double IPE_MCT_CC[9] = {
      1.55078, -0.74219,  0.18750,
     -0.27734,  1.51172, -0.23438,
     -0.01953, -0.69531,  1.71484,
    };

    //2500K
    int  DSP_CCM_LCT_RGAIN  = 292;
    int  DSP_CCM_LCT_BGAIN  = 682;
    double IPE_LCT_CC[9] = {
     1.16765, -0.34508,  0.16257,
    -0.10366,  1.23492, -0.12825,
     0.07537, -0.31391,  1.22770
    };

    int i;
    double ccr[9];
    double HCT_S, MCT_S, LCT_S;
    double ct = 256 * BGain / RGain;

    HCT_S = DSP_CCM_HCT_BGAIN / DSP_CCM_HCT_RGAIN;
    MCT_S = DSP_CCM_MCT_BGAIN / DSP_CCM_MCT_RGAIN;
    LCT_S = DSP_CCM_LCT_BGAIN / DSP_CCM_LCT_RGAIN;

    if ( ct >= HCT_S )
    {
        for ( i = 0; i<9; i++ )
            ccr[i] = IPE_HCT_CC[i];
    }
    else if ( ct <= LCT_S )
    {
        for ( i = 0; i<9; i++ )
            ccr[i] = IPE_LCT_CC[i];
    }
    else if ( HCT_S >= ct && ct >= MCT_S )
    {
        for ( i = 0; i<9; i++ )
            ccr[i] = IPE_HCT_CC[i] + (IPE_MCT_CC[i]-IPE_HCT_CC[i])*(HCT_S-ct)/(HCT_S - MCT_S);
    }
    else if ( MCT_S >= ct && ct >= LCT_S )
    {
        for ( i = 0; i<9; i++ )
            ccr[i] = IPE_LCT_CC[i] + (IPE_MCT_CC[i]-IPE_LCT_CC[i])*(LCT_S-ct)/(LCT_S - MCT_S);
    }
    else
    {
        for ( i = 0; i<9; i++ )
            ccr[i] = IPE_MCT_CC[i];
    }
    for ( i = 0; i<9; i++ )
    {
        if ( ccr[i] < 0 )
            IQ_CC[i] = (int)(ccr[i]*256) + 0x800;
        else
            IQ_CC[i] = (int)(ccr[i]*256);
    }
//DBG_ERR("%x %x %x\r\n", IQ_CC[0], IQ_CC[1], IQ_CC[2]);
//DBG_ERR("%x %x %x\r\n", IQ_CC[3], IQ_CC[4], IQ_CC[5]);
//DBG_ERR("%x %x %x\r\n", IQ_CC[6], IQ_CC[7], IQ_CC[8]);
#else
  memcpy(IQ_CC, Normal_CC, sizeof(INT16)*9);
#endif
    return E_OK;
}

static ER IPE_IQparam(IQS_FLOW_MSG MsgID, IPE_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    UINT32 ISO_Value, Zoom_Value, Sharpness_Value, Saturation_Value, IMEffect_Value;
    UINT32 RGain, GGain, BGain;
    ER ret = E_OK;
    //#NT#2013/12/25#Jarkko Chang -begin
    //#NT# add for switch movie/photo gamma curve: DODLS500W video mode will always use WDR_Gamma
    IPL_MODE_INFOR Info;

    Info.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);
    //#NT#2013/12/25#Jarkko Chang -end

    IMEffect_Value = IPL_AlgGetUIInfo(IPL_SEL_IMAGEEFFECT);
    Sharpness_Value = IPL_AlgGetUIInfo(IPL_SEL_SHARPNESS);
    Saturation_Value = IPL_AlgGetUIInfo(IPL_SEL_SATURATION);
    ISO_Value = IQS_GetISOIdx(MsgID);
    Zoom_Value = IQS_GetZoomIdx(MsgID);

    IPE_CC_Adjust((INT16*)IQ_Normal_CC);

    UINT32 iso, isoH, isoL, ISO_IdxL, ISO_IdxH, i;
    if ( IQS_GetIsoRange( MsgID, &iso, &isoL, &isoH, &ISO_IdxL, &ISO_IdxH ) == TRUE )
    {
        memcpy(&IQ_IPE_EdgeEnh, &(IPE_EdgeEnh[ISO_Value]), sizeof(IPE_EDGEENH));
        IQ_IPE_EdgeEnh.EnhN = IQS_Intpl((int)iso, (int)IPE_EdgeEnh[ISO_IdxL].EnhN, (int)IPE_EdgeEnh[ISO_IdxH].EnhN, (int)isoL, (int)isoH);
        IQ_IPE_EdgeEnh.EnhP = IQS_Intpl((int)iso, (int)IPE_EdgeEnh[ISO_IdxL].EnhP, (int)IPE_EdgeEnh[ISO_IdxH].EnhP, (int)isoL, (int)isoH);


        memcpy(&IQ_IPE_EDMap, &(IPE_EDMap[ISO_Value]), sizeof(EdgeMap));
        IQ_IPE_EDMap.EthrL = IQS_Intpl((int)iso, (int)IPE_EDMap[ISO_IdxL].EthrL, (int)IPE_EDMap[ISO_IdxH].EthrL, (int)isoL, (int)isoH);
        IQ_IPE_EDMap.EtabL = IQS_Intpl((int)iso, (int)IPE_EDMap[ISO_IdxL].EtabL, (int)IPE_EDMap[ISO_IdxH].EtabL, (int)isoL, (int)isoH);

        memcpy(IQ_Saturation, Saturation[ISO_Value], sizeof(INT8)*SEL_SATURATION_MAX_CNT);
        for ( i = 0; i < SEL_SATURATION_MAX_CNT; i++ )
        {
            IQ_Saturation[i] = IQS_Intpl((int)iso, (int)Saturation[ISO_IdxL][i], (int)Saturation[ISO_IdxH][i], (int)isoL, (int)isoH);
        }
        //DBG_ERR("%d %d %d %d %d %d\r\n", iso, IQ_IPE_EdgeEnh.EnhN, IQ_IPE_EdgeEnh.EnhP, IQ_IPE_EDMap.EthrL, IQ_IPE_EDMap.EtabL, IQ_Saturation[1]);
    }
    else
    {
        memcpy(&IQ_IPE_EdgeEnh, &(IPE_EdgeEnh[ISO_Value]), sizeof(IPE_EDGEENH));
        memcpy(&IQ_IPE_EDMap, &(IPE_EDMap[ISO_Value]), sizeof(EdgeMap));
        memcpy(IQ_Saturation, Saturation[ISO_Value], sizeof(INT8)*SEL_SATURATION_MAX_CNT);
    }

    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
        case IQCB_PRV_IPE_INIT:
        case IQS_AE_END:
        case IQS_IMAGE_EFFECT:
        case IQCB_PREIME_D2D_IPE_PRI:
        case IQCB_PREIME_D2D_IPE_QV:
             //enable function
            //leak random noise enable
            subf->IpeFuncEn |= (
                                 IPE_CgainEn     |      ///< Color gain function enable
                                 IPE_GBALEn      |      ///< GBAL function enable
                                 IPE_CFAINTEEn   |      ///< CFA interpolation function enable
                                 IPE_RGBLPEn     |      ///< RGB low-pass filter function enable
                                 IPE_CCREn       |      ///< Color correction function enable
                                 IPE_CSTEn       |      ///< Color space transform function enable, RGB->YCC
                                 IPE_CSTPEn      |      ///< Color space transform protect function enable, RGB->YCC
                                 IPE_Y_ENHEn     |      ///< Color component edge enhance function enable, sub-function of IPE_CADJEn
                                 IPE_RGBGAMMAEn  |      ///< RGB Gamma function enable
                                 //IPE_3DCCREn     |      ///< 3D color correction function enable
                                 IPE_CCTRLEn     |      ///< Color control function enable
                                 IPE_HADJEn      |      ///< Hue adjustment function enable
                                 IPE_CADJEn      |      ///< Color component adjust function enable
                                 IPE_Y_CONEn     |      ///< Color component Y contrast adjust function enable, sub-function of IPE_CADJEn
                                 IPE_CC_CONEn    |      ///< Color component CbCr contrast adjust function enable, sub-function of IPE_CADJEn
                                 IPE_YCC_STTEn   |      ///< Color component YCbCr substitution function enable, sub-function of IPE_CADJEn
                                 0
                               );

            //Setting parameter
            group->ipeUpdate |= (
                                IPE_SubFEn      |    ///< update func enable/disable
                                IPE_GBal_       |    ///< update g channel balance
                                IPE_CFAinter_   |    ///< update cfa interpolation
                                IPE_RGBLpf_     |    ///< update rgb low pass
                                IPE_CC_         |    ///< update color correction
                                IPE_CSTP_       |    ///< update color space transfor protection
                                IPE_EdgeExt_    |    ///< update edge extraction
                                IPE_EdgeEnh_    |    ///< update positive/negative edge enhance/inverse
                                IPE_EdgeMap_    |    ///< update edge map
                                IPE_C_Ctrl_     |    ///< update color control
                                IPE_C_Con_      |    ///< update color contrast(Y/CC)
                                IPE_C_Ofs_      |    ///< update Cb/Cr offset
                                IPE_YCCFix_     |    ///< update YCC fix replace
                                IPE_THRES_      |    ///For DIS
                                0
                              );

            if ( MsgID == IQCB_PRV_IPE_INIT || MsgID == IQCB_PREIME_D2D_IPE_PRI || MsgID == IQCB_PREIME_D2D_IPE_QV )
            {
                group->ipeUpdate |= ( IPE_CG_Gain_ | IPE_CG_Ofs_ | IPE_CG_GEffect_ );
            }

            if ( MsgID == IQCB_PRV_IPE_INIT || MsgID == IQCB_PREIME_D2D_IPE_PRI || MsgID == IQCB_PREIME_D2D_IPE_QV || MsgID == IQS_MANUAL_TRIG )
            {
                group->ipeUpdate |= ( IPE_RGBGAMMA_ | IPE_3DCC_);
            }

            //////////////////
            // AWb Gain
            //////////////////
            AWB_GetColorGian(&RGain, &GGain, &BGain);
            subf->Cg.Gain.IpeGainSel = d2f8;
            subf->Cg.Gain.R  = RGain;
            subf->Cg.Gain.Gr = GGain;
            subf->Cg.Gain.Gb = GGain;
            subf->Cg.Gain.B  = BGain;
            subf->Cg.Ofs.R  = (UINT32)IPE_ColorGain_Offset[ISO_Value][0];
            subf->Cg.Ofs.Gr = (UINT32)IPE_ColorGain_Offset[ISO_Value][1];
            subf->Cg.Ofs.Gb = (UINT32)IPE_ColorGain_Offset[ISO_Value][2];
            subf->Cg.Ofs.B  = (UINT32)IPE_ColorGain_Offset[ISO_Value][3];
            subf->Cg.Mask.GainMask = 0x3ff;

            //////////////////
            // GBal
            //////////////////
            subf->pGbal = &(IPE_Gbal[ISO_Value]);
            subf->pCFAInterpolation = &IPE_CFAInter[ISO_Value];

            //////////////////
            // Edge Ext
            //////////////////
            subf->pedgeExt          = &IPE_EdgeExt[ISO_Value];

            //////////////////
            // Edge Map
            //////////////////
            IQ_IPE_EdgeMap.edMap = IPE_EDMap[ISO_Value];
            IQ_IPE_EdgeMap.esMap = IPE_ESymMap[ISO_Value];
            IQ_IPE_EdgeMap.ESTAB_TblAddr = (UINT32)IPE_EStab[ISO_Value];
            IQ_IPE_EdgeMap.EMapOut = _EEXT;
            subf->pedgeMap          = &IQ_IPE_EdgeMap;

            IQ_IPE_EdgeMap.edMap = IQ_IPE_EDMap;

            //////////////////
            // Edge Enh
            //////////////////
            IQ_IPE_EdgeEnh.EnhP = (IQ_IPE_EdgeEnh.EnhP * Sharpness[ISO_Value][Sharpness_Value])>>7;
            IQ_IPE_EdgeEnh.EnhN = (IQ_IPE_EdgeEnh.EnhN * Sharpness[ISO_Value][Sharpness_Value])>>7;
            subf->pedgeEnh          = &IQ_IPE_EdgeEnh;

            //////////////////
            // RGB lpf
            //////////////////
            subf->prgblpf           = &IPE_RGBLpf[ISO_Value];

            //////////////////
            // Color Matrix
            //////////////////
            IQ_IPE_CC.CCaccu = d2f8;
            IQ_IPE_CC.CCcoef_TblAddr = (UINT32)&IQ_Normal_CC;
            IQ_IPE_CC.SupSel = toIdentity;
            IQ_IPE_CC.Fstab_TblAddr = (UINT32)CC_fmStab[ISO_Value];
            IQ_IPE_CC.Fdtab_TblAddr = (UINT32)CC_fmDtab[ISO_Value];
            subf->pCC = &IQ_IPE_CC;

            //////////////////
            // Color Control
            //////////////////
            IQ_IPE_Cctrl.HueTab_TblAddr = (UINT32)IPE_hueTab;
            IQ_IPE_Cctrl.SatTab_TblAddr = (UINT32)IPE_satTab;
            IQ_IPE_Cctrl.IntTab_TblAddr = (UINT32)IPE_intTab;
            IQ_IPE_Cctrl.EdgTab_TblAddr = (UINT32)IPE_edgTab;
            IQ_IPE_Cctrl.DDSTab_TblAddr = (UINT32)IPE_ddsTab;
            IQ_IPE_Cctrl.IntOfs = ColorEffectTable[IMEffect_Value].IntOfs;
            IQ_IPE_Cctrl.SatOfs = (ColorEffectTable[IMEffect_Value].SatOfs + IQ_Saturation[Saturation_Value]);
            IQ_IPE_Cctrl.HueRotateEn = ColorEffectTable[IMEffect_Value].HueRotateEn;
            IQ_IPE_Cctrl.Suppress.EdgSel = ESD_OUT;
            IQ_IPE_Cctrl.Suppress.VdetDiv = 0x60;
            subf->pCctrl = &IQ_IPE_Cctrl;

            /////////////////
            //Image Effect
            /////////////////
            IQ_IPE_CbCrofs.Cbofs = ColorEffectTable[IMEffect_Value].CbOfs;
            IQ_IPE_CbCrofs.Crofs = ColorEffectTable[IMEffect_Value].CrOfs;
            IQ_IPE_YCCcon.YCon = ColorEffectTable[IMEffect_Value].YCon + Contrast[Contrast_Value];
            IQ_IPE_YCCcon.UVCon = ColorEffectTable[IMEffect_Value].CCon;
            subf->pCbCrofs = &IQ_IPE_CbCrofs;
            subf->pYCCcon = &IQ_IPE_YCCcon;

            subf->pRandNoise = &IPE_NoiseParam;

            //subf->pyccmask = IPE_yccMask;
            //subf->pCST;

            ////////////////////////////
            // Color Space Protect
            ////////////////////////////
            IQ_IPE_CSTP.CSTPratio = 8;
            subf->pCstp = &IQ_IPE_CSTP;

            ////////////////////////////
            // YCC fix effect
            ////////////////////////////
            IQ_IPE_YCFixEffect = IPE_YCFixEffect;
            subf->pYccFix = &IQ_IPE_YCFixEffect;

            //subf->CCout;
            //subf->Vout;

            //////////////////
            // Color Control
            //////////////////
            IQ_IPE_RGBGAMMA.GammaTbl_TblAddr = (UINT32)IPE_GammaReorgon((UINT32 *)GammaLUT_128Tab, (UINT32 *)GammaLUT_128Tab, (UINT32 *)GammaLUT_128Tab);
            subf->pGamma = &IQ_IPE_RGBGAMMA;

            //////////////////
            // 3D Color Control
            //////////////////
            IQ_IPE_3DCC.TCC_TblAddr = (UINT32)IPE_3DCCLut;
            subf->p3d_cc = &IQ_IPE_3DCC;

            //////////////////
            // DIS
            //////////////////
            subf->EdgeThreshold.pEdgeThreshold = &IPE_EdgeThre[ISO_Value];

            ///////////////////
            // Image Effect
            ///////////////////
            switch (IMEffect_Value)
            {

                case SEL_IMAGEEFFECT_BW:
                case SEL_IMAGEEFFECT_SEPIA:
                case SEL_IMAGEEFFECT_VIVID:
                case SEL_IMAGEEFFECT_NEUTRAL:
                case SEL_IMAGEEFFECT_LOWSHARPING:
                case SEL_IMAGEEFFECT_FASION2:
                case SEL_IMAGEEFFECT_CUSTOMMANUAL:
                case SEL_IMAGEEFFECT_RED:
                case SEL_IMAGEEFFECT_GREEN:
                case SEL_IMAGEEFFECT_BLUE:
                case SEL_IMAGEEFFECT_YELLOW:
                case SEL_IMAGEEFFECT_PURPLE:
                    subf->pCctrl->IntOfs = ColorEffectTable[IMEffect_Value].IntOfs;
                    subf->pCctrl->SatOfs = (ColorEffectTable[IMEffect_Value].SatOfs + IQ_Saturation[Saturation_Value]);

                    subf->pCctrl->HueRotateEn = ColorEffectTable[IMEffect_Value].HueRotateEn;
                    subf->pCctrl->Suppress.EdgSel = ESD_OUT;
                    subf->pCctrl->Suppress.VdetDiv = 0x60;
                    subf->pCbCrofs->Cbofs = ColorEffectTable[IMEffect_Value].CbOfs;
                    subf->pCbCrofs->Crofs = ColorEffectTable[IMEffect_Value].CrOfs;
                    subf->pYCCcon->YCon   = ColorEffectTable[IMEffect_Value].YCon;
                    subf->pYCCcon->UVCon  = ColorEffectTable[IMEffect_Value].CCon;
                    break;
                case SEL_IMAGEEFFECT_COLORPENCIL:
                    subf->pYccFix->Yth1.EdgeThres = IE_ColorPencil_Param[ISO_Value].EdgeThres;
                    subf->pYccFix->Yth1.Threshold = IE_ColorPencil_Param[ISO_Value].Threshold;
                    subf->pYccFix->Yth1.HIT_FixOutputEn = IE_ColorPencil_Param[ISO_Value].HIT_FixOutputEn;
                    subf->pYccFix->Yth1.NONEHIT_FixOutputEn = IE_ColorPencil_Param[ISO_Value].NONEHIT_FixOutputEn;
                    subf->pYccFix->Yth1.HIT_value = IE_ColorPencil_Param[ISO_Value].HIT_value;
                    subf->pYccFix->Yth1.NONHIT_value = IE_ColorPencil_Param[ISO_Value].NONHIT_value;
                    break;
                case SEL_IMAGEEFFECT_SKETCH:
                    subf->pYccFix->Yth1.EdgeThres = IE_PencilSketch_Param[ISO_Value].EdgeThres;
                    subf->pYccFix->Yth1.Threshold = IE_PencilSketch_Param[ISO_Value].Threshold;
                    subf->pYccFix->Yth1.HIT_FixOutputEn = IE_PencilSketch_Param[ISO_Value].HIT_FixOutputEn;
                    subf->pYccFix->Yth1.NONEHIT_FixOutputEn = IE_PencilSketch_Param[ISO_Value].NONEHIT_FixOutputEn;
                    subf->pYccFix->Yth1.HIT_value = IE_PencilSketch_Param[ISO_Value].HIT_value;
                    subf->pYccFix->Yth1.NONHIT_value = IE_PencilSketch_Param[ISO_Value].NONHIT_value;

                    subf->pedgeEnh->InvP = IE_PencilSketch_Param[ISO_Value].EInvP;
                    subf->pedgeEnh->InvN = IE_PencilSketch_Param[ISO_Value].EInvN;

                    subf->pCbCrofs->Cbofs = IE_PencilSketch_Param[ISO_Value].CbOfs;
                    subf->pCbCrofs->Crofs = IE_PencilSketch_Param[ISO_Value].CrOfs;
                    subf->pYCCcon->UVCon  = IE_PencilSketch_Param[ISO_Value].CCon;

                    break;
                case SEL_IMAGEEFFECT_FISHEYE:
                case SEL_IMAGEEFFECT_WDR:
                default:
                    break;
            }
            break;

        case IQS_AWB_END:
            //Setting parameter
            group->ipeUpdate |= ( IPE_CG_Gain_ | IPE_CG_Ofs_ | IPE_CG_GEffect_ );
            AWB_GetColorGian(&RGain, &GGain, &BGain);
            subf->Cg.Gain.IpeGainSel = d2f8;
            subf->Cg.Gain.R  = RGain;
            subf->Cg.Gain.Gr = GGain;
            subf->Cg.Gain.Gb = GGain;
            subf->Cg.Gain.B  = BGain;
            subf->Cg.Ofs.R  = (UINT32)IPE_ColorGain_Offset[ISO_Value][0];
            subf->Cg.Ofs.Gr = (UINT32)IPE_ColorGain_Offset[ISO_Value][1];
            subf->Cg.Ofs.Gb = (UINT32)IPE_ColorGain_Offset[ISO_Value][2];
            subf->Cg.Ofs.B  = (UINT32)IPE_ColorGain_Offset[ISO_Value][3];
            subf->Cg.Mask.GainMask = 0x3ff;


            IQ_IPE_CC.CCaccu = d2f8;
            IQ_IPE_CC.CCcoef_TblAddr = (UINT32)&IQ_Normal_CC;
            IQ_IPE_CC.SupSel = toIdentity;
            subf->pCC = &IQ_IPE_CC;
            group->ipeUpdate |= ( IPE_CC_ );
            break;

        case IQS_DISCOMP_ON:
            subf->IpeFuncEn |= ( IPE_EDGETHREEn );
            group->ipeUpdate |= ( IPE_SubFEn );
            break;
        case IQS_DISCOMP_OFF:
            subf->IpeFuncEn &= ~( IPE_EDGETHREEn );
            group->ipeUpdate |= ( IPE_SubFEn );
            break;

        case IQS_AF_START:
            group->ipeUpdate |= ( IPE_SubFEn | IPE_EdgeExt_ | IPE_EdgeEnh_ | IPE_EdgeMap_ | IPE_VA_ );
            subf->IpeFuncEn |= ( IPE_VACCEn );

            //////////////////
            // Edge Ext
            //////////////////
            subf->pedgeExt = &IPE_EdgeExt[ISO_Value];

            //////////////////
            // Edge Map
            //////////////////
            IQ_IPE_EdgeMap.edMap = IPE_EDMap[ISO_Value];
            IQ_IPE_EdgeMap.esMap = IPE_ESymMap[ISO_Value];
            IQ_IPE_EdgeMap.ESTAB_TblAddr = (UINT32)IPE_EStab[ISO_Value];
            IQ_IPE_EdgeMap.EMapOut = _EEXT;
            subf->pedgeMap = &IQ_IPE_EdgeMap;

            //////////////////
            // Edge Enh
            //////////////////
            IQ_IPE_EdgeEnh = IPE_EdgeEnh[ISO_Value];
            subf->pedgeEnh = &IQ_IPE_EdgeEnh;

            //////////////////
            // VA
            //////////////////
            subf->va.pVa = &IPE_VAParam[ISO_Value];
            break;

        case IQS_AF_END:
            group->ipeUpdate |= ( IPE_SubFEn  | IPE_EdgeExt_ | IPE_EdgeEnh_ | IPE_EdgeMap_ );
            subf->IpeFuncEn &= ~( IPE_VA_ );

            //////////////////
            // Edge Ext
            //////////////////
            subf->pedgeExt          = &IPE_EdgeExt[ISO_Value];

            //////////////////
            // Edge Map
            //////////////////
            IQ_IPE_EdgeMap.edMap = IPE_EDMap[ISO_Value];
            IQ_IPE_EdgeMap.esMap = IPE_ESymMap[ISO_Value];
            IQ_IPE_EdgeMap.ESTAB_TblAddr = (UINT32)IPE_EStab[ISO_Value];
            IQ_IPE_EdgeMap.EMapOut = _EEXT;
            subf->pedgeMap          = &IQ_IPE_EdgeMap;

            //////////////////
            // Edge Enh
            //////////////////
            IQ_IPE_EdgeEnh = IPE_EdgeEnh[ISO_Value];
            IQ_IPE_EdgeEnh.EnhP = (IQ_IPE_EdgeEnh.EnhP * Sharpness[ISO_Value][Sharpness_Value])>>7;
            IQ_IPE_EdgeEnh.EnhN = (IQ_IPE_EdgeEnh.EnhN * Sharpness[ISO_Value][Sharpness_Value])>>7;
            subf->pedgeEnh          = &IQ_IPE_EdgeEnh;
            break;

        case IQS_OZOOM_END:
            break;
        default :
            DBG_ERR("IPE_IQparam() param mode(%d) selection error!\r\n",MsgID);
            ret = E_SYS;
         break;
    }
    return ret;
}

static ER IME_IQparam(IQS_FLOW_MSG MsgID, IME_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    UINT32 ISO_Value, Zoom_Value, SenMode_Value, IMEffect_Value;
    UINT32 WDR_Value = 0;

    IMEffect_Value = IPL_AlgGetUIInfo(IPL_SEL_IMAGEEFFECT);
    ISO_Value = IQS_GetISOIdx(MsgID);
    Zoom_Value = IQS_GetZoomIdx(MsgID);
    SenMode_Value = IQS_GetSenModeIdx(MsgID);

    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
        case IQCB_PRV_IME_INIT:
            if ( IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IPE_V_EN) == ENABLE )
            {
                DBG_IND("PRV Enable RetinexEn\r\n");
                subf->imeFuncEn |= (RetinexEn);
                group->imeUpdate |= (IME_Retinex_);
            }
            if ( IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IPE_LCA_EN) == ENABLE &&
                 IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IPEIFE2_LCA_EN) == ENABLE )
            {
                DBG_IND("PRV Enable ChromaAdaptEn\r\n");
                subf->imeFuncEn |= (ChromaAdaptEn);
                group->imeUpdate |= (IME_ChromaA_);
            }
            subf->imeFuncEn |= ( ChromaFiltEn );
            group->imeUpdate |= ( IME_SubFEn | IME_ChromaF_ | IME_3DNR_ );
            subf->TDnr.p3DNR    = &IME_3DNR[ISO_Value];
            subf->pChromaFilter = &IME_Chroma_Filter_param[ISO_Value];

            subf->Retinex.RtxMode = RTX_Outer;
            subf->Retinex.pRetinex = &IME_RetInex[WDR_Value];
            subf->ChromaAdap.pChromaAdaption = &IME_Chroma_Adapt_param[ISO_Value];
            switch (IMEffect_Value)
            {
                case SEL_IMAGEEFFECT_WDR:
                    subf->imeFuncEn |= (RetinexEn | RGBtoYCCEn | YCCtoRGBEn);
                    group->imeUpdate |= (IME_Retinex_);
                    subf->Retinex.RtxMode = RTX_Inner;
                    break;
                default:
                    break;
            }
#if 0
            group->imeUpdate |= IME_SCALEQ_F_P2;
            IQ_P2Filt.CoefUserDefEn = ENABLE;
            IQ_P2Filt.UserDefine.HsfEn = ENABLE;
            IQ_P2Filt.UserDefine.VsfEn = ENABLE;
            IQ_P2Filt.UserDefine.HsfCoef = 0;
            IQ_P2Filt.UserDefine.VsfCoef = 0;
            subf->pP2_Filt = &IQ_P2Filt;
#endif
            break;
        case IQS_AE_END:
            subf->imeFuncEn |= ( ChromaFiltEn );
            group->imeUpdate |= ( IME_SubFEn | IME_ChromaF_ | IME_3DNR_ );
            subf->TDnr.p3DNR    = &IME_3DNR[ISO_Value];
            subf->pChromaFilter = &IME_Chroma_Filter_param[ISO_Value];
            subf->ChromaAdap.pChromaAdaption = &IME_Chroma_Adapt_param[ISO_Value];

            break;
        case IQCB_PRV_IME_LCA:
            if ( IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IPE_LCA_EN) == ENABLE &&
                 IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IPEIFE2_LCA_EN) == ENABLE )
            {
                subf->imeFuncEn |= (ChromaAdaptEn);
                group->imeUpdate |= (IME_ChromaA_);
            }
            subf->ChromaAdap.pChromaAdaption = &IME_Chroma_Adapt_param[ISO_Value];
            break;
        case IQCB_PRV_IME_RETINEX:
            if ( IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IPE_V_EN) == ENABLE )
            {
                DBG_IND("Post process flow, RetinexEn Enable\r\n");
                subf->imeFuncEn |= (RetinexEn);
                group->imeUpdate |= (IME_Retinex_);
                subf->Retinex.RtxMode = RTX_Outer;
                subf->Retinex.pRetinex = &IME_RetInex[WDR_Value];
            }
            break;
        case IQCB_D2D_IME_QVREF:
        case IQCB_PREIME_D2D_IME_QV:
            subf->imeFuncEn &= ~(RetinexEn | ChromaAdaptEn);
            group->imeUpdate |= ( IME_SubFEn );
            break;
        case IQCB_PREIME_D2D_IME_PRI:
        case IQCB_D2D_IME_QV:
            subf->imeFuncEn |= ( ChromaFiltEn );
            group->imeUpdate |= ( IME_SubFEn | IME_ChromaF_ );

            if (MsgID == IQCB_PREIME_D2D_IME_PRI)
            {
                if (IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_CAP_PRI_V_EN) == ENABLE)
                {
                    subf->imeFuncEn |= (RetinexEn);
                    group->imeUpdate |= (IME_Retinex_);
                }

                if (IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_CAP_PRI_CC_EN) == ENABLE)
                {
                    subf->imeFuncEn |= (ChromaAdaptEn);
                    group->imeUpdate |= (IME_ChromaA_);
                    DBG_IND("IME_ChromaA_ = enable, %d\r\n", ISO_Value);
                }
                //if ( (IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IPE_LCA_EN) == DISABLE) && (group->imeUpdate &IME_ChromaA_)  )
                //{
                //    DBG_ERR("IME_IQparam(): Need to allocate buffer for LCA!\r\n");
                //}
                //subf->imeFuncEn |= (IME3DNREn);
                //group->imeUpdate |= (IME_3DNR_);
            }
            else if (MsgID == IQCB_D2D_IME_QV)
            {
                if (IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_CAP_QV_V_EN) == ENABLE)
                {
                    subf->imeFuncEn |= (RetinexEn);
                    group->imeUpdate |= (IME_Retinex_);
                }

                if (IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_CAP_QV_CC_EN) == ENABLE)
                {
                    subf->imeFuncEn |= (ChromaAdaptEn);
                    group->imeUpdate |= (IME_ChromaA_);
                    DBG_IND("IME_ChromaA_ = enable, %d\r\n", ISO_Value);
                }
            }

            //////////////////
            // IME
            //////////////////
            subf->TDnr.p3DNR    = &IME_3DNR[ISO_Value];
            subf->pChromaFilter = &IME_Chroma_Filter_param[ISO_Value];
            subf->ChromaAdap.pChromaAdaption = &IME_Chroma_Adapt_param[ISO_Value];
            DBG_IND("Lca Weight = %d\r\n", subf->ChromaAdap.pChromaAdaption->LcaWeight);
            if ( IMEffect_Value == SEL_IMAGEEFFECT_WDR )
            {   //WDR
                subf->Retinex.RtxMode = RTX_Outer;
                subf->Retinex.pRetinex = &IME_RetInex[WDR_Value];
            }
            else
            {   //MFR
                subf->Retinex.RtxMode = RTX_Outer;
                subf->Retinex.pRetinex = &IME_mfr[ISO_Value];
            }
            break;
        case IQS_AWB_END:
            switch (IMEffect_Value)
            {
                case SEL_IMAGEEFFECT_WDR:

                    subf->imeFuncEn |= (RetinexEn | RGBtoYCCEn | YCCtoRGBEn);
                    group->imeUpdate |= (IME_SubFEn | IME_Retinex_);
                    subf->Retinex.RtxMode = RTX_Inner;
                    IME_RetInex[WDR_Value].gGain_D  = GetRtxGainInfo.uiGDGain;
                    IME_RetInex[WDR_Value].gGain_B  = GetRtxGainInfo.uiGLGain;
                    IME_RetInex[WDR_Value].lGain_D  = GetRtxGainInfo.uiLDGain;
                    IME_RetInex[WDR_Value].lGain_B  = GetRtxGainInfo.uiLLGain;
                    IME_RetInex[WDR_Value].cGain_R  = GetRtxGainInfo.uiRGain;
                    IME_RetInex[WDR_Value].cGain_G  = GetRtxGainInfo.uiGGain;
                    IME_RetInex[WDR_Value].cGain_B  = GetRtxGainInfo.uiBGain;
                    IME_RetInex[WDR_Value].ExagGain = GetRtxGainInfo.uiEGain;

                    subf->Retinex.pRetinex = &IME_RetInex[WDR_Value];
                    break;
                default:
                    break;
            }
            break;
        case IQS_AF_START:
        case IQS_AF_END:
        case IQS_OZOOM_END:
            break;
        case IQS_IMAGE_EFFECT:
            // Normal Flow,
            switch (IMEffect_Value)
            {
                case SEL_IMAGEEFFECT_WDR:
                    subf->imeFuncEn |= (RetinexEn | RGBtoYCCEn | YCCtoRGBEn);
                    group->imeUpdate |= (IME_SubFEn | IME_Retinex_);
                    subf->Retinex.RtxMode = RTX_Inner;
                    subf->Retinex.pRetinex = &IME_RetInex[WDR_Value];
                    break;
                default:
                    subf->imeFuncEn &= (~RetinexEn);
                    group->imeUpdate |= (IME_SubFEn);
                    break;
            }
            break;
        default :
            DBG_ERR("IME_IQparam() param mode(%d) selection error!\r\n",MsgID);
            return E_SYS;
    }
    return E_OK;
}

static ER IFE2_IQparam(IQS_FLOW_MSG MsgID, IFE2_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    UINT32 ISO_Value, IMEffect_Value;
    IMEffect_Value = IPL_AlgGetUIInfo(IPL_SEL_IMAGEEFFECT);
    ISO_Value = IQS_GetISOIdx(MsgID);
    switch((UINT32)MsgID)
    {
        case IQCB_D2D_IFE2_PRI_Y:
        case IQCB_D2D_IFE2_QV_Y:
        case IQCB_PRV_IFE2_RETINEX:
            if ( IMEffect_Value == SEL_IMAGEEFFECT_WDR )
            {   //WDR
                subf->ife2FuncEn |= ( AvgFiltEn );
                subf->ife2FuncEn &= ( ~IFE2_Chroma_);
                group->ife2Update |= ( IFE2_SubFEn | IFE2_AvgFilter_ );
                IQ_IFE2_AvgFilter.Nonpack_Filter = IFE2_AvgFilter_WDR[ISO_Value];
                subf->pAvgFilter = &IQ_IFE2_AvgFilter;
            }
            else
            {   //MFR
                subf->ife2FuncEn  |= ( AvgFiltEn );
                subf->ife2FuncEn &= ( ~IFE2_Chroma_);
                group->ife2Update |= ( IFE2_SubFEn | IFE2_AvgFilter_ );
                IQ_IFE2_AvgFilter.Nonpack_Filter = IFE2_AvgFilter_y_mfr[ISO_Value];
                subf->pAvgFilter = &IQ_IFE2_AvgFilter;
            }
            break;

        case IQCB_PRV_IFE2_INIT_CC:
        case IQCB_PRV_IFE2_CC:
            subf->ife2FuncEn |= ( AvgFiltEn);
            group->ife2Update |= ( IFE2_SubFEn | IFE2_AvgFilter_ );
            if ( IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IPE_LCA_EN) == ENABLE &&
                 IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IPEIFE2_LCA_EN) == ENABLE )
            {
                subf->ife2FuncEn |= ( ChromaEn );
                group->ife2Update |= ( IFE2_Chroma_ );
            }
            IQ_IFE2_AvgFilter.UVpack.Pack_UFilter = IFE2_AvgFilter_cb[ISO_Value];
            IQ_IFE2_AvgFilter.UVpack.Pack_VFilter = IFE2_AvgFilter_cr[ISO_Value];
            subf->pAvgFilter = &IQ_IFE2_AvgFilter;
            subf->pChromaAdj = &IFE2_Chroma_param[ISO_Value];
            break;
        case IQCB_D2D_IFE2_QV_CC:
        case IQCB_D2D_IFE2_PRI_CC:
            subf->ife2FuncEn |= ( ChromaEn | AvgFiltEn);
            group->ife2Update |= ( IFE2_SubFEn | IFE2_Chroma_ | IFE2_AvgFilter_ );
            IQ_IFE2_AvgFilter.UVpack.Pack_UFilter = IFE2_AvgFilter_cb[ISO_Value];
            IQ_IFE2_AvgFilter.UVpack.Pack_VFilter = IFE2_AvgFilter_cr[ISO_Value];
            subf->pAvgFilter = &IQ_IFE2_AvgFilter;
            subf->pChromaAdj = &IFE2_Chroma_param[ISO_Value];
            break;
        case IQS_MANUAL_TRIG:
        case IQS_AE_END:
        case IQS_AWB_END:
        case IQS_AF_START:
        case IQS_AF_END:
        case IQS_OZOOM_END:
        case IQS_IMAGE_EFFECT:
            break;
        default:
            DBG_ERR("IFE2_IQparam() param mode(%d) selection error!\r\n",MsgID);
            break;
    }

    return E_OK;
}

///////////////////////////////////////////////
// FlowSetting Interface
///////////////////////////////////////////////
static ER IQS_SIE_IQparam(IQS_FLOW_MSG MsgID)
{
    return E_OK;
}

static ER IQS_PRE_IQparam(IQS_FLOW_MSG MsgID)
{
    ER ret = E_OK;
    PRE_SUBFUNC preSubf = {0};
    PRE pre;
    IPL_HAL_PARASET idx = IPL_CtrlGetHalSetId(IPL_ID_1);
    IPL_HAL_READ    readItem = {0};
    IPL_HAL_GROUP   group = {0};
    IPC_IQparam     Ipc_IQ;
    WAI_PRESEM;

    pre.pSubFunc = &preSubf;
    readItem.preRead = PRE_R_SubEn;
    IPH_ENG_Read(IPL_ID_1,IPL_HAL_PRE, idx, readItem, (void*)&preSubf.preFuncEn);

    ret = PRE_IQparam(MsgID, &preSubf, &group);
    if ( ret != E_OK )
    {
        SIG_PRESEM;
        return ret;
    }

    //IPH_PRE_ChgParam(idx, group, &pre);
    //IPH_ENG_Load(IPL_HAL_PRE, idx, group);
    Ipc_IQ.EngSubF.pPre_sub = &preSubf;
    Ipc_IQ.pGrp = &group;
    Ipc_IQ.Idx = idx;
    IPL_CTRLRunTimeChg_IQ(IPC_PRE_IQparam,(void*)&Ipc_IQ);
    SIG_PRESEM;
    return ret;
}

static ER IQS_DCE_IQparam(IQS_FLOW_MSG MsgID)
{
    DCE_SUBFUNC dceSubf = {0};
    IPL_HAL_GROUP group = {0};
    IPC_IQparam dce_info = {0};

    DCE_IQparam(MsgID, &dceSubf,&group);

    dce_info.EngSubF.pDce_sub = &dceSubf;
    dce_info.Idx = IPL_CtrlGetHalSetId(IPL_ID_1);
    dce_info.pGrp = &group;

    IPL_CTRLRunTimeChg(IPC_Chg_DCE,(void*)&dce_info);
    return E_OK;
}

static ER IQS_IFE_IQparam(IQS_FLOW_MSG MsgID)
{
    ER ret = E_OK;
    IFE_SUBFUNC ifeSubf = {0};
    IFE ife;
    IPL_HAL_PARASET idx = IPL_CtrlGetHalSetId(IPL_ID_1);
    IPL_HAL_READ    readBit = {0};
    IPL_HAL_GROUP   group = {0};
    IPC_IQparam     Ipc_IQ = {0};
    WAI_IFESEM;
    ife.pSubFunc = &ifeSubf;
    readBit.ifeRead = IFE_R_SubFEn;
    IPH_ENG_Read(IPL_ID_1,IPL_HAL_IFE, idx, readBit, (void*)&ifeSubf.ifeFuncEn);

    ret = IFE_IQparam(MsgID, &ifeSubf, &group);
    if ( ret != E_OK )
    {
        SIG_IFESEM;
        return ret;
    }

    //IPH_IFE_ChgParam(idx, group, &ife);
    //IPH_ENG_Load(IPL_HAL_IFE, idx, group);
    Ipc_IQ.EngSubF.pIfe_sub = &ifeSubf;
    Ipc_IQ.pGrp = &group;
    Ipc_IQ.Idx = idx;
    IPL_CTRLRunTimeChg_IQ(IPC_IFE_IQparam,(void*)&Ipc_IQ);
    SIG_IFESEM;
    return E_OK;
}

static ER IQS_IPE_IQparam(IQS_FLOW_MSG MsgID)
{
    ER ret = E_OK;
    IPE_SUBFUNC ipeSubf = {0};
    IPE ipe;
    IPL_HAL_PARASET idx = IPL_CtrlGetHalSetId(IPL_ID_1);
    IPL_HAL_READ    readBit = {0};
    IPL_HAL_GROUP   group = {0};
    IPC_IQparam     Ipc_IQ = {0};
    WAI_IPESEM;

    ipe.pSubFunc = &ipeSubf;
    //read previous function enable
    readBit.ipeRead = IPE_R_SubFEn;
    IPH_ENG_Read(IPL_ID_1,IPL_HAL_IPE, idx, readBit, (void*)&ipeSubf.IpeFuncEn);

    ret = IPE_IQparam(MsgID, &ipeSubf, &group);
    if ( ret != E_OK )
    {
        SIG_IPESEM;
        return ret;
    }

    Ipc_IQ.EngSubF.pIpe_sub = &ipeSubf;
    Ipc_IQ.pGrp = &group;
    Ipc_IQ.Idx = idx;
    IPL_CTRLRunTimeChg_IQ(IPC_IPE_IQparam,(void*)&Ipc_IQ);
    SIG_IPESEM;
    return E_OK;
}


static ER IQS_IFE2_IQparam(IQS_FLOW_MSG MsgID)
{
    ER ret = E_OK;
    IFE2_SUBFUNC ife2Subf = {0};
    IFE2 ife2;
    IPL_HAL_PARASET idx = IPL_CtrlGetHalSetId(IPL_ID_1);
    IPL_HAL_READ    readBit = {0};
    IPL_HAL_GROUP   group = {0};
    IPC_IQparam     Ipc_IQ = {0};
    WAI_IFESEM;
    ife2.pSubFunc = &ife2Subf;
    readBit.ifeRead = IFE2_R_SubFEn;
    IPH_ENG_Read(IPL_ID_1,IPL_HAL_IFE2, idx, readBit, (void*)&ife2Subf.ife2FuncEn);

    ret = IFE2_IQparam(MsgID, &ife2Subf, &group);
    if ( ret != E_OK )
    {
        SIG_IFESEM;
        return ret;
    }

    Ipc_IQ.EngSubF.pIfe2_sub = &ife2Subf;
    Ipc_IQ.pGrp = &group;
    Ipc_IQ.Idx = idx;
    IPL_CTRLRunTimeChg_IQ(IPC_IFE2_IQparam,(void*)&Ipc_IQ);
    SIG_IFESEM;
    return E_OK;
}

static ER IQS_IME_IQparam(IQS_FLOW_MSG MsgID)
{
    ER ret = E_OK;
    IME_SUBFUNC imeSubf = {0};
    IME ime;
    IPL_HAL_PARASET idx = IPL_CtrlGetHalSetId(IPL_ID_1);
    IPL_HAL_READ    readBit = {0};
    IPL_HAL_GROUP   group = {0};
    IPC_IQparam     Ipc_IQ = {0};
    WAI_IMESEM;

    ime.pSubFunc = &imeSubf;
    readBit.imeRead = IME_R_SubFEn;
    IPH_ENG_Read(IPL_ID_1,IPL_HAL_IME, idx, readBit, (void*)&imeSubf.imeFuncEn);

    ret = IME_IQparam(MsgID, &imeSubf, &group);
    if ( ret != E_OK )
    {
        SIG_IMESEM;
        return ret;
    }

    //IPH_IME_ChgParam(idx, group, &ime);
    //IPH_ENG_Load(IPL_HAL_IME, idx, group);
    Ipc_IQ.EngSubF.pIme_sub = &imeSubf;
    Ipc_IQ.pGrp = &group;
    Ipc_IQ.Idx = idx;
    IPL_CTRLRunTimeChg_IQ(IPC_IME_IQparam,(void*)&Ipc_IQ);
    SIG_IMESEM;
    return E_OK;
}

ER IQS_SettingFlowFunc(IQS_FLOW_MSG MsgID)
{
    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
            IQS_SIE_IQparam(MsgID);
            IQS_PRE_IQparam(MsgID);
            IQS_DCE_IQparam(MsgID);
            IQS_IFE_IQparam(MsgID);
            IQS_IPE_IQparam(MsgID);
            IQS_IFE2_IQparam(MsgID);
            IQS_IME_IQparam(MsgID);
            break;
        case IQS_AWB_END:
            WDR_VideoGainAdjust(&GetRtxGainInfo);

            IQS_IPE_IQparam(MsgID);
            IQS_IME_IQparam(MsgID);
            break;
        case IQS_AE_END:

            ////////////////////////////////////////////////////////////////////////
            // Can not change IFE2 & IME LCA here
            // move to IQCB_PRV_IME_LCA & IQCB_PRV_IFE2_INIT_CC & IQCB_PRV_IFE2_CC
            ////////////////////////////////////////////////////////////////////////
            IQS_IFE_IQparam(MsgID);
            IQS_IPE_IQparam(MsgID);
            IQS_IME_IQparam(MsgID);
            break;
        case IQS_OZOOM_END:
            IQS_PRE_IQparam(MsgID);
            IQS_DCE_IQparam(MsgID);
            break;
        case IQS_DISCOMP_ON:
        case IQS_DISCOMP_OFF:
            IQS_IPE_IQparam(MsgID);
            break;
        case IQS_AF_START:
        case IQS_AF_END:
            break;

        case IQS_IMAGE_EFFECT:
            IQS_DCE_IQparam(MsgID);
            IQS_IPE_IQparam(MsgID);
            IQS_IME_IQparam(MsgID);
            break;
        default:
            DBG_ERR("^R-E- MsgID(%x)\r\n", MsgID);
            break;
    }
    return E_OK;
}


///////////////////////////////////////////////
// CBSetting Interface
///////////////////////////////////////////////

ER IQCB_SettingFlowFunc(IQS_FLOW_MSG MsgID, ENG_SUBFUNC *Info, IPL_HAL_GROUP* group)
{
    switch((UINT32)MsgID)
    {
        //preview
        case IQCB_PRV_SIE_INIT:
            break;

        case IQCB_PRV_IPE_INIT:
        case IQCB_PREIME_D2D_IPE_PRI:
        case IQCB_PREIME_D2D_IPE_QV:
            IPE_IQparam(MsgID, Info->pIpe_sub, group);
            break;

        case IQCB_PRV_PRE_INIT:
        case IQCB_PREIME_D2D_PRE_PRI:
        case IQCB_PREIME_D2D_PRE_QV:
            PRE_IQparam(MsgID, Info->pPre_sub, group);
            break;

        case IQCB_PRV_DCE_INIT:
        case IQCB_PREIME_D2D_DCE_QV:
        case IQCB_PREIME_D2D_DCE_PRI:
            DCE_IQparam(MsgID, Info->pDce_sub, group);
            break;

        case IQCB_PRV_IFE_INIT:
        case IQCB_PREIME_D2D_IFE_PRI:
        case IQCB_PREIME_D2D_IFE_QV:
            IFE_IQparam(MsgID, Info->pIfe_sub, group);
            break;

        case IQCB_PRV_IME_INIT:
        case IQCB_PRV_IME_LCA:
        case IQCB_PRV_IME_RETINEX:
            IME_IQparam(MsgID, Info->pIme_sub, group);
            break;

        case IQCB_D2D_IME_QVREF:
        case IQCB_D2D_IME_QV:  //FQV LCA & HDR
        case IQCB_PREIME_D2D_IME_QV:  //disbale LCA
        case IQCB_PREIME_D2D_IME_PRI: //enable LCA
            IME_IQparam(MsgID, Info->pIme_sub, group);
            break;

        case IQCB_PRV_IFE2_INIT_CC:
        case IQCB_PRV_IFE2_CC:
        case IQCB_PRV_IFE2_RETINEX:
        case IQCB_D2D_IFE2_QV_Y:
        case IQCB_D2D_IFE2_QV_CC:
        case IQCB_D2D_IFE2_PRI_Y:
        case IQCB_D2D_IFE2_PRI_CC:
            IFE2_IQparam(MsgID, Info->pIfe2_sub, group);
            break;

        default:
            DBG_ERR("^R-E- MsgID(%x)\r\n", MsgID);
            break;
    }
    return E_OK;
}


///////////////////////////////////////////////
// Flow Export
///////////////////////////////////////////////
UINT32 IQS_GetIFE2RetinexTime(IQS_FLOW_MSG MsgID) //PRV & CAP
{
    UINT32 ISO_Value, IMEffect_Value;
    IMEffect_Value = IPL_AlgGetUIInfo(IPL_SEL_IMAGEEFFECT);
    ISO_Value = IQS_GetISOIdx(MsgID);
    if ( IMEffect_Value == SEL_IMAGEEFFECT_WDR )
        return IFE2_Filter_Times[ISO_Value].RTXTimes;
    else
        return IFE2_Filter_Times[ISO_Value].MFRTimes;
}

UINT32 IQS_GetIFE2LcaTime(IQS_FLOW_MSG MsgID)  //PRV & CAP
{
    UINT32 ISO_Value;
    ISO_Value = IQS_GetISOIdx(MsgID);
    ////DBG_IND("LCATimes = %d", IFE2_Filter_Times[ISO_Value].LCATimes);
    return IFE2_Filter_Times[ISO_Value].LCATimes;
}

//preview
UINT32 IQS_GetRetinexMinRatio(void)
{
    //get IQS min chroma adaption ratio in streaming mode
    //for IPL prepare buffer
    return IME_RetInex[0].SubRatio;
}

UINT32 IQS_GetChromaAdaptMinRatio(void)
{
    //get IQS min chroma adaption ratio in streaming mode
    //for IPL prepare buffer
    UINT32 MinRatio = 0xf;
    UINT32 i;
    for ( i=0; i<Total_ISONum; i++ )
    {
        if ( MinRatio > IME_Chroma_Adapt_param[i].SubRatio )
            MinRatio= IME_Chroma_Adapt_param[i].SubRatio;
    }
    return MinRatio;
}

UINT32 IQS_PRV_IsChromaAdaptEn(void)
{
    //get IQS chroma adaption on/off in streaming mode
    //for IPL prepare buffer
    return ENABLE;
}

UINT32 IQS_CAPPRI_GetLCARefImgRatio(void)
{
    UINT32 ISO_Value;
    ISO_Value  = IQS_GetISOIdx(IQCB_CAP_PRI_START);
    return IME_Chroma_Adapt_param[ISO_Value].SubRatio;
}

UINT32 IQS_CAPQV_GetLCARefImgRatio(void)
{
    UINT32 ISO_Value;
    ISO_Value  = IQS_GetISOIdx(IQCB_CAP_QV_START);
    return IME_Chroma_Adapt_param[ISO_Value].SubRatio;
}

UINT32 IQS_CAPPRI_GetRetinexRefImgRatio(void)
{
    UINT32 ISO_Value, IMEffect_Value;
    IMEffect_Value = IPL_AlgGetUIInfo(IPL_SEL_IMAGEEFFECT);
    ISO_Value  = IQS_GetISOIdx(IQCB_CAP_PRI_START);
    if ( IMEffect_Value == SEL_IMAGEEFFECT_WDR )
    {
        return IME_RetInex[0].SubRatio;
    }
    else
    {
        return IME_mfr[ISO_Value].SubRatio;
    }
}

UINT32 IQS_CAPQV_GetRetinexRefImgRatio(void)
{
    UINT32 ISO_Value, IMEffect_Value;
    IMEffect_Value = IPL_AlgGetUIInfo(IPL_SEL_IMAGEEFFECT);
    ISO_Value  = IQS_GetISOIdx(IQCB_CAP_QV_START);
    if ( IMEffect_Value == SEL_IMAGEEFFECT_WDR )
    {
        return IME_RetInex[0].SubRatio;
    }
    else
    {
        return IME_mfr[ISO_Value].SubRatio;
    }
}

UINT32 IQS_CAPQV_IsChromaAdaptEn(void)
{
    UINT32 ISO_Value;
    ISO_Value  = IQS_GetISOIdx(IQCB_CAP_QV_START);
    if ( IFE2_Filter_Times[ISO_Value].LCATimes == 0 )
    {
        return DISABLE;
    }
    else
    {
        return ENABLE;
    }
}

UINT32 IQS_CAPQV_IsRetinexEn(void)
{
    //get IQS chroma adaption on/off in streaming mode
    //for IPL prepare buffer
    UINT32 ISO_Value, IMEffect_Value;
    IMEffect_Value = IPL_AlgGetUIInfo(IPL_SEL_IMAGEEFFECT);
    ISO_Value  = IQS_GetISOIdx(IQCB_CAP_QV_START);
    if ( IMEffect_Value == SEL_IMAGEEFFECT_WDR )
    {
        if ( IFE2_Filter_Times[ISO_Value].RTXTimes == 0 )
        {
            DBG_ERR("^RMust set RTXTimes for SEL_IMAGEEFFECT_WDR\r\n");
            return DISABLE;
        }
        else
            return ENABLE;
    }
    else
    {
        if ( IFE2_Filter_Times[ISO_Value].MFRTimes == 0 )
        {
            return DISABLE;
        }
        else
        {
            return ENABLE;
        }
    }
}

UINT32 IQS_CAPPRI_IsChromaAdaptEn(void)
{
    UINT32 ISO_Value;
    ISO_Value  = IQS_GetISOIdx(IQCB_CAP_PRI_START);
    if ( IFE2_Filter_Times[ISO_Value].LCATimes == 0 )
    {
        return DISABLE;
    }
    else
    {
        return ENABLE;
    }
}

UINT32 IQS_CAPPRI_IsRetinexEn(void)
{
    //get IQS chroma adaption on/off in streaming mode
    //for IPL prepare buffer
    UINT32 ISO_Value, IMEffect_Value;
    IMEffect_Value = IPL_AlgGetUIInfo(IPL_SEL_IMAGEEFFECT);
    ISO_Value  = IQS_GetISOIdx(IQCB_CAP_PRI_START);
    if ( IMEffect_Value == SEL_IMAGEEFFECT_WDR )
    {
        if ( IFE2_Filter_Times[ISO_Value].RTXTimes == 0 )
        {
            DBG_ERR("^RMust set RTXTimes for SEL_IMAGEEFFECT_WDR\r\n");
            return DISABLE;
        }
        else
        {
            return ENABLE;
        }
    }
    else
    {
        if ( IFE2_Filter_Times[ISO_Value].MFRTimes == 0 )
        {
            return DISABLE;
        }
        else
        {
            return ENABLE;
        }
    }
}


void WDR_VideoGainAdjust(SceneRender_RTXGainInfo* GetRtxGainInfo)
{
    SceneRender_RTXCalInfo CalRtxInfo;

    Img_GetCA_Result(IPL_ID_1,WDR_Racc,WDR_Gacc,WDR_Bacc,1024);

    // retinex gain
    CalRtxInfo.puiLuma = WDR_Gacc;
    CalRtxInfo.uiLumaBit = 12;
    CalRtxInfo.uiSizeH = 32;
    CalRtxInfo.uiSizeV = 32;
    CalRtxInfo.puiIPEGamma = GammaLUT_128Tab;
    UINT32 ISO, ExpT, Iris;
    AE_GetPrvAEArg(&ISO, &ExpT, &Iris);

    if ( ISO < 100 )
        CalRtxInfo.IsoIdx = WDR_LOCAL_STRENGTH_1;
    else if ( ISO < 200 )
        CalRtxInfo.IsoIdx = WDR_LOCAL_STRENGTH_2;
    else if ( ISO < 400 )
        CalRtxInfo.IsoIdx = WDR_LOCAL_STRENGTH_3;
    else if ( ISO < 800 )
        CalRtxInfo.IsoIdx = WDR_LOCAL_STRENGTH_4;
    else if ( ISO < 1600 )
        CalRtxInfo.IsoIdx = WDR_LOCAL_STRENGTH_5;
    else
        CalRtxInfo.IsoIdx = WDR_LOCAL_STRENGTH_5;

    SR_CalRTXGain(&CalRtxInfo, GetRtxGainInfo);
}


UINT32 IQS_AddGammaTab(UINT32 Data12Bit)
{
    INT32 idx, YLum;
    INT32 LumIdx, LumIdx1;
    UINT32 data;

    data = Data12Bit;

    if (data < 1)
    {
        idx = 0;
        LumIdx = 0;
    }
    else
    {
        idx = (data * 128) / 4096;
        LumIdx = (idx * 4096) / 128;
    }
    LumIdx1 = ((idx + 1) * 4096) / 128;

    YLum = ((GammaLUT_128Tab[(idx + 1)] - GammaLUT_128Tab[idx]) * (data - LumIdx)) / (LumIdx1 - LumIdx);
    YLum += GammaLUT_128Tab[idx];

    if ((YLum < 0) || (YLum > 1024))
    {

        DBG_ERR("%d %d %d %d %d\r\n", idx, LumIdx, LumIdx1, GammaLUT_128Tab[idx], GammaLUT_128Tab[(idx + 1)]);

        DBG_ERR("data = %d YLum = %d\r\n", data, YLum);
    }
    return YLum;
}

UINT32 IQS_InvGammaTab(UINT32 Data10Bit)
{
    UINT32 i;
    UINT32 ChkFlag = FALSE;
    UINT32 Idx1 = 0, Idx = 0;
    UINT64 RawLum;
    for (i = 1; i < 130; i ++)
    {
        if (Data10Bit <= GammaLUT_128Tab[i])
        {
            Idx1 = i;
            Idx = (Idx1 - 1);
            ChkFlag = TRUE;
            break;
        }
    }

    if (ChkFlag == TRUE)
    {
        RawLum = (((Idx1 - Idx) * (Data10Bit - GammaLUT_128Tab[Idx])) << 10) / (GammaLUT_128Tab[Idx1] - GammaLUT_128Tab[Idx]);
        RawLum += (Idx << 10);
        RawLum = ((RawLum << 12) >> 7) >> 10;
        if (RawLum >= 4096)
        {
            DBG_ERR("%d %d %d %d %d\r\n", Idx, Idx1, Data10Bit, GammaLUT_128Tab[Idx], GammaLUT_128Tab[Idx1]);
        }
        return RawLum;
    }
    else
    {
        DBG_ERR("input overflow %d\r\n", Data10Bit);
        return 0;
    }
}

UINT32 IQS_SetIFEGain(IPL_PROC_ID Id, IFE_SUBFUNC *ptr)
{
    IFE ife;
    IPL_HAL_READ readBit = {0};
    IPL_HAL_GROUP group = {0};
    group.ifeUpdate = IFE_Cgain_|IFE_SubFEn;
    ife.pSubFunc = ptr;
    readBit.ifeRead = IFE_R_SubFEn;
    IPH_ENG_Read_ISR(IPL_ID_1,IPL_HAL_IFE, IPL_CtrlGetHalSetId(Id), readBit, (void*)&ife.pSubFunc->ifeFuncEn);
    ife.pSubFunc->ifeFuncEn |= CgainEn;
    IPH_ENG_ChgParam(Id, IPL_CtrlGetHalSetId(Id), IPL_HAL_IFE, group, &ife);
    IPH_ENG_Load(Id, IPL_HAL_IFE, IPL_CtrlGetHalSetId(Id), group);
    return 0;
}
#endif
