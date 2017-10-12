/**
    IPH module (IME sub function type define)

    @file       IPL_Hal_IME_Info.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_IME_Info_H
#define _IPL_Hal_IME_Info_H
#include "Type.h"
#include "IPL_Utility.h"
#include "pll.h"
/** \addtogroup mILibIPH */
//@{

#define IME_MaxParaSet  5
/**
    ime engine supported clk
*/
typedef enum _IPH_IMECLK
{
    IPH_IMECLK0 = 48,    ///< 48 mhz
    IPH_IMECLK1 = 60,    ///< 60 mhz
    IPH_IMECLK2 = 80,    ///< 80 mhz
    IPH_IMECLK3 = 120,   ///< 120 mhz
    IPH_IMECLK4 = 160,   ///< 160 mhz
    IPH_IMECLK5 = 192,   ///< 192 mhz
    IPH_IMECLK6 = 220,  ///< -
    ENUM_DUMMY4WORD(IPH_IMECLK)
}IPH_IMECLK;

/**
    IME in/out data format
*/
typedef IPL_YUV_IMG_TYPE IMEFMT;

typedef struct
{
    UINT32  Y;
    UINT32  Cb;
    UINT32  Cr;
}IMEADDR;

typedef struct
{
    IPL_YCC_CH_INFO Y;
    UINT32  Lineoffset_CC;
}IMESIZE;

/**
    IME interrupt enable enum.
*/
typedef enum _ImeInteEn
{
    IME__CLR        = 0,
    IME__INTE_STPE  = 0x20000000,   ///< enable interrupt: stipe end
    //IME__INTE_FMS   = 0x40000000,   ///< enable interrupt: frame start
    IME__INTE_FMD   = 0x80000000,   ///< enable interrupt: frame end
}ImeInteEn;

/**
    IME sub function enable
*/
typedef enum _ImeFuncEn
{
    ChromaFiltEn    = 0x00000001,   ///< enable function: chroma filter
    ChromaAdaptEn   = 0x00000002,   ///< enable function: chroma adaption (LCA)
    RetinexEn       = 0x00000004,   ///< enable function: Retinex (HDR)
    ColorSpaceProtEn= 0x00000010,   ///< enable function: color space transfor protection
    RollingSCEn     = 0x00000020,   ///< enable function: rolloing shutter correction
    FilmGrainEn     = 0x00000040,   ///< enable function: film grain
    IME3DNREn       = 0x00000080,   ///< enable function: 3D noise reduction (TDnr)
    IMEP2IEn        = 0x00000100,   ///< enable function: P frame to I frame
    RGBtoYCCEn      = 0x00000200,   ///< enable function: RGB spaec to YCC space
    YCCtoRGBEn      = 0x00000400,   ///< enable function: YCC space to RGB space
    ENUM_DUMMY4WORD(ImeFuncEn)
}ImeFuncEn;

/**
    stripe overlap enum
*/
typedef enum _IME_STPESEL
{
    IME_STPE_OVERLAP_16     = 0,    ///< overlap 16 pixels
    IME_STPE_OVERLAP_24     = 1,    ///< overlap 24 pixels
    IME_STPE_OVERLAP_32     = 2,    ///< overlap 32 pixels
    ENUM_DUMMY4WORD(IME_STPESEL)
}IME_STPESEL;

/**
    stripe unit
*/
typedef struct _IME_STRIPE
{
    UINT32  N;          ///< Size of stripe except for last one, Horn. unit : 16 pix, Ver. unit : 1 line
    UINT32  L;          ///< Size of last stripe, Horn. unit : 16 pix, Ver. unit : 1 line
    UINT32  M;          ///< Number of total stripe - 1
}IME_STRIPE;

/**
     ime scaling method
     @note integration is supported only when image scaling down.
*/
typedef enum _IME_SCALER
{
    IME_BICUBIC     = 0,    ///< bicubic interpolation
    IME_BILINEAR    = 1,    ///< bilinear interpolation
    IME_NEAREST     = 2,    ///< nearest interpolation
    IME_INTEGRATION = 3,    ///< integration interpolation,support only scale down

    _AUTO           = 0xff, ///< automatical calculation
    ENUM_DUMMY4WORD(IME_SCALER)
}IME_SCALER;

/**
    IME output configuration
*/
typedef struct _OUTCH
{
    BOOL    PathEn;         ///< path enable
    //BOOL    DmaOutEn;       ///< data output to dram enable
    IMEFMT  Fmt;            ///< format
    IMESIZE Size;           ///< scale output size
    struct
    {
        Coordinate  Start;  ///< -
        UINT32  SizeH;      ///< -
        UINT32  SizeV;      ///< -
    }Crop;                  ///< ime output crop window
    IMEADDR Addr;           ///< YCC address
    IME_SCALER ScalMethod;  ///< CbCr channel have no bicubic, scaling up have no integration(all 3 path).
    /*
    enum
    {
        Type_Y  = 0,    ///< Y channel output data type - Y
        Type_V  = 1,    ///< Y channel output data type - V
        ENUM_DUMMY4WORD(Y_CH_TypeSel)
    }Y_CH_TypeSel;
    */
}OUTCH;

/**
    IME output path
*/
typedef struct _IME_OUT
{
    OUTCH Path1;    ///< path1 output
    OUTCH Path2;    ///< path2 output (output fmt support only uv pack)
    OUTCH Path3;    ///< path3 output (output fmt support only uv pack)
}IME_OUT;

/**
    scaling quality(enhance) parameter set

    @note: only work when ScalMethod = _Bicubic
*/
typedef struct _IMESCALE_Enh
{
    UINT32  Factor;     ///< -
    UINT32  Norm_bit;   ///< -
}IMESCALE_Enh;

/**
    scaling quality(filter)parameter set
*/
typedef struct _IMESCALE_Filt
{
    BOOL    CoefUserDefEn;      ///< user define mode enable bit
    struct
    {
        BOOL    HsfEn;          ///< H direction scaling filter
        BOOL    VsfEn;          ///< V direction scaling filter
        UINT32  HsfCoef;        ///< H direction coefficient
        UINT32  VsfCoef;        ///< V direction coefficient
    }UserDefine;                ///< user defined
}IMESCALE_Filt;

/**
    Chroma filter parameter set
*/
typedef struct _IME_CHRA_FILT
{
    BOOL RangeFiltEn;           ///< enable range filter
    struct
    {
        UINT32  *pThres_Cb;     ///< Cb channel threshold , tab number:4, Data type: UINT32
        UINT32  *pThres_Cr;     ///< Cr channel threshold , tab number:4, Data type: UINT32
        UINT32  *pWeight;       ///< weights, tab number:5, Data type: UINT32
    }RangeFilt;                 ///< range filter
    struct
    {
        UINT32  Weight0;    ///< weighting0
        UINT32  Weight1;    ///< weighting1
        UINT32  Coef;       ///< coefficient
    }SpaceFilt;             ///< spatial filter
}IME_CHRA_FILT;

/**
    chroma adaption parameter set
*/
typedef struct _IME_CHRA_ADAP
{
    enum
    {
        Precision_2 = 0,    ///< LUT precision 2
        Precision_4 = 1,    ///< LUT precision 4
        Precision_8 = 2,    ///< LUT precision 8
        ENUM_DUMMY4WORD(LcaLutPre)
    }LcaLutPre;             ///< -

    UINT32  Thres_Cb;     ///< cb channel threshold
    UINT32  Thres_Cr;     ///< cr channel threshold
    UINT32  Thres_Lut;    ///< lut channel threshold

    UINT32  LcaWeight;      ///< reference layer weight
    UINT32  LcaLUT_TblAddr; ///< tab number:33

    UINT32  SubRatio;       ///< sub out size: size/(1<<(SubRatio))
}IME_CHRA_ADAP;

/**
    Retinex parameter set
*/
typedef struct _IME_RETINEX
{
    UINT32  gGain_B;    ///<global gain on bright part
    UINT32  gGain_D;    ///<global gain on dark part
    UINT32  lGain_B;    ///<local gain on bright part
    UINT32  lGain_D;    ///<local gain on dark part
    UINT32  cGain_R;    ///<retinex color gain on R
    UINT32  cGain_G;    ///<retinex color gain on G
    UINT32  cGain_B;    ///<retinex color gain on B
    UINT32  ExagGain;   ///<color gain for exaggerating effect

    UINT32  SubRatio;       ///< sub out size: size/(1<<(SubRatio))
}IME_RETINEX;

/**
    color space transfor parameter set
*/
typedef struct _IME_CSPACE
{
    BOOL    YUV2RGBEn; ///< enable bit for YUV domain transfor to RGB domain
    BOOL    RGB2YUVEn; ///< -
}IME_CSPACE;

/**
    color space transfor protection parameter set
*/
typedef struct _IME_CSPACE_P
{
    UINT32  CspaceProtRatio; ///< -
}IME_CSPACE_P;

/**
    rolling shutter correction parameter set
*/
typedef struct _IME_RSCorrect
{
    UINT32  ctpFactor_P1;       ///< RSC control-point interpolation factor for Path1
    UINT32  ctpFactor_P2;       ///< RSC control-point interpolation factor for Path2
    UINT32  ctpGain_P1;         ///< ime path1 RSC ctrl point gain factor
    UINT32  ctpGain_P2;         ///< ime path2 RSC ctrl point gain factor
    UINT32  ctpIntePos;         ///< RSC control-point interpolation position
    UINT32  *pctpShift_H;       ///< RSC control-point horizontal shift, tab num:12, Data type:UINT32
    UINT32  *pctpShift_V;       ///< RSC control-point vertical shift, tab num:12, Data type:UINT32
    UINT32  MarginH;            ///< control ime scaling factor
    UINT32  MarginV;            ///< control ime scaling factor
}IME_RSCorrect;

/**
    film grain parameter set
*/
typedef struct _IME_FG
{
    UINT32  LNDifThres;     ///< luminance-Noise difference threshold
    UINT32  LumiThres;      ///< luminance-Noise difference threshold

    UINT32  GNInit_p1;      ///< Grain noise initial value for path1
    UINT32  Weight_p1;      ///< path1 weighting between pixel and noise value
    UINT32  GNInit_p2;      ///< Grain noise initial value for path2
    UINT32  Weight_p2;      ///< path2 weighting between pixel and noise value
    UINT32  GNInit_p3;      ///< Grain noise initial value for path3
    UINT32  Weight_p3;      ///< path3 weighting between pixel and noise value
}IME_FG;

/**
    3D noise reduction prarmeter set
*/
typedef struct _IME_3D_NR
{
    UINT32  *pThresY;     ///< thresholds for Y channel, tab num:3, Data type:UINT32
    UINT32  *pWeightY;    ///< weightings for Y channel, tab num:4, Data type:UINT32
    UINT32  *pThresCb;    ///< thresholds for Cb channel, tab num:3, Data type:UINT32
    UINT32  *pWeightCb;   ///< weightings for Cb channel, tab num:4, Data type:UINT32
    UINT32  *pThresCr;    ///< thresholds for Cr channel, tab num:3, Data type:UINT32
    UINT32  *pWeightCr;   ///< weightings for Cr channel, tab num:4, Data type:UINT32
}IME_3D_NR;

typedef struct _IME_SUBFUNC
{
    ImeInteEn   imeInteEn;  ///< interrupt enable
    ImeFuncEn   imeFuncEn;  ///< function enable

    IME_CHRA_FILT   *pChromaFilter; ///< chorma filter
    struct
    {
        IME_CHRA_ADAP   *pChromaAdaption; ///< -
        //BOOL    PPBEn;
        UINT32  PPB0Addr;       ///< reference image address
        //UINT32  PPB1Addr;
        IPL_YCC_CH_INFO Size;   ///< reference image size
    }ChromaAdap;

    struct
    {
        enum
        {
            RTX_Inner   = 0,    ///< main image as input source
            RTX_Outer   = 1,    ///< input source from dram
            ENUM_DUMMY4WORD(RtxMode)
        }RtxMode;
        IME_RETINEX     *pRetinex; ///< -
        //BOOL    PPBEn;
        UINT32  PPB0Addr;       ///< reference image address
        //UINT32  PPB1Addr;
        IPL_YCC_CH_INFO Size;   ///< reference image size
    }Retinex;
    //IME_CSPACE      *pCspaceTrans;
    IME_CSPACE_P    *pCspaceP;  ///< color space transfor protection
    IME_RSCorrect   *pRsc;      ///< rolling shutter correction
    IME_FG          *pFilmGrain;    ///< film grain

    struct
    {
        UINT32  InputAddr_Y;        ///< Y channel inputaddr
        UINT32  InputAddr_UV;       ///< UV chanel inputaddr
        IME_3D_NR       *p3DNR;     ///< -
    }TDnr;                          ///< 3D NR

    IMESCALE_Enh    *pP1_Enh;       ///< scaler quality enhancement for path1
    IMESCALE_Enh    *pP2_Enh;       ///< scaler quality enhancement for path2
    IMESCALE_Enh    *pP3_Enh;       ///< scaler quality enhancement for path3

    IMESCALE_Filt   *pP1_Filt;      ///< scaling filter for path1
    IMESCALE_Filt   *pP2_Filt;      ///< scaling filter for path2
    IMESCALE_Filt   *pP3_Filt;      ///< scaling filter for path2
    //BOOL            P2IEn;
}IME_SUBFUNC;

#define SCALING_BASE    (10)
#define SCALING_LIMIT   (11 * SCALING_BASE) //overlap use 24 condition
#define IPH_IME_SCAL_METHOD(RSCEn,InH,InV,OutH,OutV,Method)    \
{\
    if(RSCEn)\
        *Method = IME_BICUBIC;\
    else\
    {\
        if( (InH >= OutH) && (InV >= OutV))\
        {\
            if( ( (OutH*SCALING_LIMIT) > (InH*SCALING_BASE) ) && ( (OutV*SCALING_LIMIT) > (InH*SCALING_BASE) ))\
                *Method = IME_INTEGRATION;\
        }\
        else\
            *Method = IME_BICUBIC;\
    }\
}
//@}
#endif
