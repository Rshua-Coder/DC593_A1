/**
    IPH module (IPE sub function type define)

    @file       IPL_Hal_IPE_Info.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_IPE_Info_H
#define _IPL_Hal_IPE_Info_H
#include "Type.h"
#include "IPL_Utility.h"
#include "pll.h"
/** \addtogroup mILibIPH */
//@{

#define IPE_MaxParaSet  5

/**
    ipe engine supported clk
*/
typedef enum _IPH_IPECLK
{
    IPH_IPECLK0 = 48,    ///< 48 mhz
    IPH_IPECLK1 = 60,    ///< 60 mhz
    IPH_IPECLK2 = 80,    ///< 80 mhz
    IPH_IPECLK3 = 120,   ///< 120 mhz
    IPH_IPECLK4 = 160,   ///< 160 mhz
    IPH_IPECLK5 = 192,   ///< 192 mhz
    IPH_IPECLK6 = 220,  ///< -
    ENUM_DUMMY4WORD(IPH_IPECLK)
}IPH_IPECLK;

/**
    stripe overlap enum
*/
typedef enum _IPE_STPESEL
{
    IPE_STPE_OVERLAP_OFF    = 0,    ///< disable overlap
    IPE_STPE_OVERLAP_16     = 1,    ///< overlap 16 pixels
    IPE_STPE_OVERLAP_32     = 2,    ///< overlap 32 pixels
    ENUM_DUMMY4WORD(IPE_STPESEL)
}IPE_STPESEL;

/*
    stripe size
*/
typedef struct _IPE_STRIPE
{
    UINT32  N;          ///< Size of stripe except for last one, Horn. unit : 16 pix, Ver. unit : 1 line
    UINT32  L;          ///< Size of last stripe, Horn. unit : 16 pix, Ver. unit : 1 line
    UINT32  M;          ///< Number of total stripe - 1
}IPE_STRIPE;

/**
    IPE in/out size
*/
typedef IPL_YCC_CH_INFO IPESIZE;

/**
    IPE input data bitdepth
*/
typedef IPL_RAW_BIT IPE_PackBus;

/**
    IPE input bayer CFA
*/
typedef IPL_RAW_PIX IPE_CFA;

/**
    IPE input source enum
*/
typedef enum _IPESRC
{
    IPE_SRC_DRAM    = 0,    ///< input from dram
    IPE_SRC_IFE     = 1,    ///< input from ife
    ENUM_DUMMY4WORD(IPESRC)
}IPESRC;

/**
    IPE output format
*/
typedef enum _IPEOutFmt
{
    IPE_YCC_422 = 0,    ///< 422 output
    IPE_YCC_420 = 1,    ///< 420 output
    IPE_Y       = 2,    ///< Y channel output only
    ENUM_DUMMY4WORD(IPEOutFmt)
}IPEOutFmt;

/**
    YCC sub sample selection
*/
typedef enum _IPEDATADROP
{
    Drop_R  = 0,    ///< drop right side pixel during subsampling When 422 or 420 format is chosen
    Drop_L  = 1,    ///< drop left side pixel during subsampling When 422 or 420 format is chosen
    ENUM_DUMMY4WORD(IPEDATADROP)
}IPEDATADROP;

// each tab include 2 steps
typedef struct _IPEGAMTAB
{
    UINT32    evenStep    :10;
    UINT32    oddStep     :10;
    UINT32                :12;    //don't care
}IPEGAMTAB;

typedef struct _IPE3DC
{
    UINT32    R   :10;
    UINT32    G   :10;
    UINT32    B   :10;
    UINT32        :2;     //don't care
}IPE3DC;

/**
    edge output source
*/
typedef enum _EDGE_OUT
{
    ESD_OUT         = 0,    ///< symmetry difference
    EdgeKer_5x5_OUT = 1,    ///< 5x5 edge extraction
    EdgeKer_7x7_OUT = 2,    ///< 7x7 edge extraction
    ENUM_DUMMY4WORD(EDGE_OUT)
}EDGE_OUT;

/**
    edge input source
*/
typedef enum _EDGE_SRC
{
    E_PRE_GAMMA = 0,    ///< before tone-remap
    E_POS_GAMMA = 1,    ///< after tone-remap
    ENUM_DUMMY4WORD(EDGE_SRC)
}EDGE_SRC;

/**
    variation detection kernel
*/
typedef struct _VD_KER
{
    enum
    {
        Filtering_0 = 0,        ///< 1
        Filtering_3 = 1,        ///< 3
        Filtering_5 = 2,        ///< 5
        Filtering_7 = 3,        ///< 7
        ENUM_DUMMY4WORD(VdFilt)
    }VdFilt;    ///< filter size
    struct
    {
        INT16  A;       ///< -
        INT16  B;       ///< -
        INT16  C;       ///< -
        INT16  D;       ///< -
    }Coef;                       ///< filter coef, [D,C,B,A,B,C,D]
    UINT32  Div;                ///< normalization factor (0~15)
}VD_KER;

/**
    variation detection
*/
typedef struct _IPE__VDET
{
    EDGE_SRC    VdetSel;    ///< input source
    VD_KER      GRP1_H;     ///< group1 horn.
    VD_KER      GRP1_V;     ///< group1 ver.
    VD_KER      GRP2_H;     ///< group2 horn.
    VD_KER      GRP2_V;     ///< group2 ver.
}IPE__VDET;

typedef struct _VA_WIN
{
    UINT32  stx;    ///< starting coordinate
    UINT32  sty;    ///< -
    UINT32  szh;    ///< size
    UINT32  szv;    ///< -
    UINT32  vanf;   ///< range:10~16
}VA_WIN;            ///< va window

/**
    low pass filter
*/
typedef struct _LPF
{
    UINT32  lpfw;       ///< low pass filter weighting
    UINT32  sonlyw;     ///< spatial only fitler weighting
    UINT32  rangeth0;   ///< range threshold
    UINT32  rangeth1;   ///< -
    enum
    {
        _3x3    = 0,    ///< 3x3 window
        _5x5    = 1,    ///< 5x5 window
        _7x7    = 2,    ///< 7x7 window
        ENUM_DUMMY4WORD(lpfsize)
    }lpfsize;           ///< filter size
}LPF;

/**
    10 bits data bit field
*/
typedef enum _bitratio
{
    d2f8   = 0, ///<2 bits decimal, 8bits fraction
    d3f7   = 1, ///<3 bits decimal, 7bits fraction
    ENUM_DUMMY4WORD(bitratio)
}bitratio;

/**
    threshold high/low value
*/
typedef struct _ThresLV
{
    UINT32  Low;    ///< -
    UINT32  High;   ///< -
}ThresLV;

/**
    edge map threshold
*/
typedef struct _EdgeMap
{
    enum
    {
        EMI_USD = 0,    ///< Select 5x5 user defined kernel output for edge mapping input
        EMI_E57 = 1,    ///< Select 3x3 fixed kernel output for edge mapping input
        EMI_AVG = 2,    ///< Select average of 5x5 user defined kernel and 3x3 fixed kernel for edge mapping input
        ENUM_DUMMY4WORD(EM_InSel)
    }EM_InSel;      ///< Edge mapping input selection
    UINT32  EthrL;  ///< Legal range : 0~1023, for edge mapping, if(Ein < EthrL){Eout=0}, else if(EthrA <= Ein <EthrH){Eout apply EtabL and EDtabA}, else if(Ein >= EthrB){Eout apply EtabH and EDtabB}
    UINT32  EthrH;  ///< Legal range : 0~1023, for edge mapping, if(Ein < EthrA){Eout=0}, else if(EthrA <= Ein <EthrB){Eout apply EtabL and EDtabA}, else if(Ein >= EthrH){Eout apply EtabH and EDtabB}
    UINT32  EtabL;  ///< Legal range : 0~6, for edge mapping, EDtabA table has 8 sections, each section width is (1 << etaA)
    UINT32  EtabH;  ///< Legal range : 0~6, for edge mapping, EDtabB table has 8 sections, each section width is (1 << etaB)
    UINT32  TAB_TblAddr;    ///< tab num:16, Data type:UINT8
}EdgeMap;

/**
    VA output group selection.
*/
typedef enum _VA_OUTSEL
{
    GRP0_only   = 0,    ///< Output group0
    GRP1_only   = 1,    ///< Output group1
    GRP0_1      = 2,    ///< Output group0 and group1
    GRP1_2      = 3,    ///< Output group1 and group2
    GRP0_1_2    = 4,    ///< Output group0 and group1 and group2
    ENUM_DUMMY4WORD(VA_OUTSEL)
}VA_OUTSEL;

/**
    ipe vacc info
*/
typedef struct _IPE__VACC
{
    BOOL        ManualUpdateEn; ///< -
    VA_OUTSEL   VAOutSel;   ///< va output group selection
    EDGE_OUT    G0SRC;  ///< group0 source
    ThresLV     GRP0;   ///< group0
    struct
    {
        ThresLV ThresH;  ///< horizontal
        ThresLV ThresV;  ///< vertical
    }GRP1;          ///< group 1
    struct
    {
        ThresLV ThresH; ///< horizontal
        ThresLV ThresV; ///< vertical
    }GRP2;          ///< group 2
}IPE__VACC;

/**
    ipe interrupt enable option
*/
typedef enum _IPEINTE
{
    IPE__CLR                = 0,
    //IPE__INTE_FMS           = 0X00000001,   ///< enable interrupt: frame start
    IPE__INTE_FMD           = 0X00000002,   ///< enable interrupt: frame done
    IPE__INTE_STPE          = 0x00000004,   ///< enable interrupt: current stripe done
    IPE__INTE_YCC_DMA_E     = 0x00000010,   ///< enable interrupt: YCC DRAM output done
    IPE__INTE_VA_DMA_E      = 0X00000020,   ///< enable interrupt: VA DRAM output done
    IPE__INTE_EDGET_DMA_E   = 0x00000040,   ///< enable interrupt: edge theshold DRAM output done
    IPE__INTE_CC_SUB_DMA_E  = 0x00000080,   ///< enable interrupt: CbCr subsample DRAM output done
    IPE__INTE_V_SUB_DMA_E   = 0x00000100,   ///< enable interrupt: V subsample DRAM output done
    IPE__INTE_DAM0_IN_E     = 0x00000200,   ///< enable interrupt: DRAM input done
    IPE__INTE_DAM1_IN_E     = 0x00000400,
    IPE__INTE_ALL           = 0X000003FF,   ///< All interrupt enable
    ENUM_DUMMY4WORD(IPEINTE)
}IPEINTE;

/**
    ipe function enable selection
*/
typedef enum _IPEFUNCEN
{
    IPE_CgainEn     = 0x00000001,       ///< Color gain function enable
    IPE_GBALEn      = 0x00000002,       ///< G balance function enable
    IPE_CFAINTEEn   = 0x00000004,       ///< CFA interpolation function enable
    IPE_RGBLPEn     = 0x00000008,       ///< RGB low-pass filter function enable
    IPE_RGBGAMMAEn  = 0x00000010,       ///< RGB Gamma function enable
    IPE_YGAMMAEn    = 0x00000020,       ///< Y Gamma function enable,(curve share with ife chroma curve table)
    IPE_CCREn       = 0x00000040,       ///< Color correction function enable
    IPE_3DCCREn     = 0x00000080,       ///< 3D color correction function enable
    IPE_CSTEn       = 0x00000100,       ///< Color space transform function enable, RGB->YCC
    IPE_CCTRLEn     = 0x00000200,       ///< Color control function enable
    IPE_HADJEn      = 0x00000400,       ///< Hue adjustment function enable
    IPE_CADJEn      = 0x00000800,       ///< Color component adjust function enable
        IPE_Y_ENHEn     = 0x00001000,   ///< Color component edge enhance function enable, sub-function of IPE_CADJEn
        IPE_Y_CONEn     = 0x00002000,   ///< Color component Y contrast adjust function enable, sub-function of IPE_CADJEn
        IPE_CC_CONEn    = 0x00004000,   ///< Color component CbCr contrast adjust function enable, sub-function of IPE_CADJEn
        IPE_YCC_STTEn   = 0x00008000,   ///< Color component YCbCr substitution function enable, sub-function of IPE_CADJEn
    IPE_CSTPEn      = 0x000010000,      ///< Color space transform protect function enable, RGB->YCC
    IPE_VACCEn      = 0x000020000,      ///< VA function enable
    IPE_EDGETHREEn  = 0x000040000,      ///< Edge threshold output enable
    IPE_CC_SUBEn    = 0x000080000,      ///< CbCr subsample output enable
    IPE_V_SUBEn     = 0x000100000,      ///< V subsample output enable
    ENUM_DUMMY4WORD(IPEFUNCEN)
}IPEFUNCEN;

/**
    ipe color gain/offset parameter set
*/
typedef struct _IPE_CG
{
    struct
    {
        bitratio    IpeGainSel; ///< -
        UINT32  R;              ///< R channel
        UINT32  Gr;             ///< -
        UINT32  Gb;             ///< -
        UINT32  B;              ///< -
    }Gain;                      ///< color gain

    struct
    {
        UINT32  R;  ///< 8bit ofs on R channel
        UINT32  Gr; ///< -
        UINT32  Gb; ///< -
        UINT32  B;  ///< -
    }Ofs;           ///< color offset

    struct
    {
        BOOL    INVEn;  ///< -
        BOOL    HINVEn; ///< -
    }Inv;               ///< -

    struct
    {
        UINT32  GainMask;   ///< -
    }Mask;              ///< -
}IPE_CG;

/**
    ipe g balance parameter set
*/
typedef struct _IPE_GBAL
{
    UINT32  Thres;          ///< Legal range : 0~1023, Gbal threshold for decision
    UINT8   *pSTab;         ///< Legal range : 0~7, Gbalstab table defines Gbal weighting for different luminance, 0 : NO Gbal, 7 : Max Gbal
                            ///< tab num:16, Data type:UINT8
    UINT8   *pDTab;         ///< Legal range : 0~7, Gbalstab table defines Gbal weighting for different variation, 0 : NO Gbal, 7 : Max Gbal
                            ///< tab num:16, Data type:UINT8
}IPE_GBAL;

/**
    CFA interpolation parameter set
*/
typedef struct _IPE_CFAINTER
{
    UINT32  NsmarEdge;      ///< Legal range : 0~1023, noise margin for G interpolation
    UINT32  NsmarDiff;      ///< Legal range : 0~1023, noise margin for G interpolation
    struct
    {
        BOOL    En;         ///< enable bit
        UINT32  NormBit;    ///< Legal range : 0~3, Normalize bit
        UINT32  DIFNormBit; ///< Legal range : 0~3, Normalize bit of difference decision
    }GC;                    ///< G channel correction
}IPE_CFAINTER;

/**
    directed edge extraction parameter set
*/
typedef struct _IPE_DEE
{
    enum
    {
        _EEXT0  = 0,    ///< Select smooth kernel for edge direction decision
        _EEXT1  = 1,    ///< Select center-weighted kernel for edge direction decision
        ENUM_DUMMY4WORD(KerSel)
    }KerSel;            ///< kernel selection (fixed kernel)
    enum
    {
        Div_0   = 0,    ///< Normal division
        Div_1   = 1,    ///< normal division - 1
        Div_2   = 2,    ///< normal division - 2
        Div_3   = 3,    ///< normal division - 3
        ENUM_DUMMY4WORD(EdgeDiv)
    }EdgeDiv;           ///< edge division for directed edge kernel
    UINT32  EDirTab_TblAddr;                ///<Legal range : 0~8, wieghting for directed edge, 0 : no direction decision, 8 : sensitive edge decision
}IPE_DEE;

/**
    symmetry different edge extraction
*/
typedef struct _IPE_ESD
{
    enum
    {
        ESD_3X3 = 0,    ///< 3x3 window
        ESD_5X5 = 1,    ///< 5x5 window
        ENUM_DUMMY4WORD(ESDSel)
    }ESDSel;            ///< ESD filter size
    enum
    {
        LminusR = 0,    ///< Select H subtraction method : left pixel minus right pixel
        RminusL = 1,    ///< Select H subtraction method : right pixel minus left pixel
        ENUM_DUMMY4WORD(ESD_Hopt)
    }ESD_Hopt;          ///< ESD Horn. operation
    enum
    {
        UminusL = 0,    ///< Select V subtraction method : upper pixel minus lower pixel
        LminusU = 1,    ///< Select V subtraction method : lower pixel minus upper pixel
        ENUM_DUMMY4WORD(ESD_Vopt)
    }ESD_Vopt;          ///< ESD Ver. operation
    enum
    {
        Div_0_   = 0,    ///< Normal division
        Div_1_   = 1,    ///< normal division - 1
        Div_2_   = 2,    ///< normal division - 2
        ENUM_DUMMY4WORD(DetDiv)
    }DetDiv;     ///< edge division for symmetry difference
}IPE_ESD;

/**
    IPE Edge extraction
*/
typedef struct _IPE_EDGEEXT
{
    EDGE_SRC    GamSel;     ///< source selection
    enum
    {
        EDGE_G_CH   = 0,    ///< G channel of bayer format
        EDGE_Y_CH   = 1,    ///< Y channel of YCC format
        ENUM_DUMMY4WORD(EdgeChSel)
    }EdgeChSel;
    UINT32  EEXT_TblAddr;   ///< user defined edge kernel , tab num:11, Data type:INT16 (idx 0~9 is edge kernal, idx 10 is div)

    UINT32  EWDir;          ///< Legal range : 0~16, 0 : 5x5 user defined edge , 16 : directed edge

    IPE_DEE *pDirectedEdge; ///< -
    IPE_ESD *pEdgeSymDiff;  ///< -
}IPE_EDGEEXT;

/**
    Edge Enhancement/Edge inverse
*/
typedef struct _IPE_EDGEENH
{
    UINT32  EnhP;   ///< positive edge enhancement
    UINT32  EnhN;   ///< negative edge enhancement
    BOOL    InvP;   ///< inverse positive edge to negative edge if true
    BOOL    InvN;   ///< inverse negative edge to positive edge if true
}IPE_EDGEENH;

/**
    IPE Edge mapping
*/
typedef struct _IPE_EDGEMAP
{
    EdgeMap edMap;          ///< edge mapping threshold
    EdgeMap esMap;          ///< edge symmetry difference mapping threshold
    UINT32  ESTAB_TblAddr;  ///< edge control luminance map(tab num:16,Data type:UINT8)
    enum
    {
        _EEXT   = 0,    ///< Select 5x5 user defined kernel
        _EEXT2  = 1,    ///< Select 3x3 fixed kernel
        ENUM_DUMMY4WORD(EMapOut)
    }EMapOut;           ///< kernel selection for edge mapping output
}IPE_EDGEMAP;

/**
    Edge threshold (dma output)
*/
typedef struct _IPE_EDGETHRE
{
    EDGE_OUT    Thre_Src;   ///< source selection
    UINT32  low;    ///< Lower bound threshold
    UINT32  mid;    ///< Middle bound threshold
    UINT32  high;   ///< Higher bound threshold
}IPE_EDGETHRE;

/**
    Edge threshold dma output configuration
*/
typedef struct _IPE_ETH_DMA
{
    UINT32  Th0Addr;    ///< address of buffer index 0
    UINT32  Th1Addr;    ///< -
    UINT32  Th2Addr;    ///< -
    UINT32  ThLineofs;  ///< -
    UINT32  Height;     ///< -
}IPE_ETH_DMA;

/**
    Variation detection and accumulation
*/
typedef struct _IPE__VA
{
    IPE__VACC Vacc; ///< accumulation
    IPE__VDET Vdet; ///< detection
}IPE__VA;

/**
    Variation accumulation independent window. (detection is based on VA)
*/
typedef struct _IPE__VA_IND
{
    BOOL    Win0En; ///< enable window0
    VA_WIN  Win0;   ///< -
    BOOL    Win1En; ///< enable window1
    VA_WIN  Win1;   ///< -
    BOOL    Win2En; ///< enable window2
    VA_WIN  Win2;   ///< -
}IPE__VA_IND;

/**
    IPE Variation indepent window result
*/
typedef struct _IPE__VA_IND_RST
{
    struct
    {
        BOOL    WinEn;  ///< enable bit
        UINT32  grp0;   ///< grp0 output
        UINT32  grp1h;  ///< grp1h output
        UINT32  grp1v;  ///< grp1v output
        UINT32  grp2h;  ///< grp2h output
        UINT32  grp2v;  ///< grp2v output
    }WIN0;              ///< windown 0 output
    struct
    {
        BOOL    WinEn;  ///< enable bit
        UINT32  grp0;   ///< grp0 output
        UINT32  grp1h;  ///< grp1h output
        UINT32  grp1v;  ///< grp1v output
        UINT32  grp2h;  ///< grp2h output
        UINT32  grp2v;  ///< grp2v output
    }WIN1;              ///< windown 1 output
    struct
    {
        BOOL    WinEn;  ///< enable bit
        UINT32  grp0;   ///< grp0 output
        UINT32  grp1h;  ///< grp1h output
        UINT32  grp1v;  ///< grp1v output
        UINT32  grp2h;  ///< grp2h output
        UINT32  grp2v;  ///< grp2v output
    }WIN2;              ///< windown 2 output
}IPE__VA_IND_RST;

#define VA_WinNum_H     16
#define VA_WinNum_V     16
#define VA_WinSize (VA_WinNum_H*VA_WinNum_V)///<16x16

/**
    passing va dma data result
*/
typedef struct _IPE_VA_DramRst
{
    UINT32 *pG0;        ///< tab num:256, Data type:UINT32 ,when VAOutSel = (GRP0_only | GRP0_1 | GRP0_1_2)
    UINT32 *pG1_h;      ///< tab num:256, Data type:UINT32 ,when VAOutSel = (GRP1_only | GRP0_1 | GRP1_2 | GRP0_1_2)
    UINT32 *pG1_v;      ///< tab num:256, Data type:UINT32 ,when VAOutSel = (GRP1_only | GRP0_1 | GRP1_2 | GRP0_1_2)
    UINT32 *pG2_h;      ///< tab num:256, Data type:UINT32 ,when VAOutSel = (GRP1_2 | GRP0_1_2)
    UINT32 *pG2_v;      ///< tab num:256, Data type:UINT32 ,when VAOutSel = (GRP1_2 | GRP0_1_2)
    UINT32 frameCnt;    ///< current frame count.
}IPE_VA_DramRst;

/**
    IPE edge threshold output

    current dma ready info
*/
typedef struct _IPE_ETH_DramRst
{
    UINT32  Addr;       ///< -
    UINT32  Lineofs;    ///< -
    UINT32  Width;      ///< -
    UINT32  Height;     ///< -
}IPE_ETH_DramRst;

/*
    IPE edge threshold output lineoffset calculate

    only 2 bit each pixel(H*2/8).
*/
#define ETH_LINEOFS(H)  ( IPL_ALIGN_ROUNDUP((H/4),4) )

/**
    RGB plane low pass filter
*/
typedef struct _IPE_RGBLPF
{
    LPF R;  ///< R channel
    LPF G;  ///< -
    LPF B;  ///< -
}IPE_RGBLPF;

/**
    color correction
*/
typedef struct _IPE_CC
{
    bitratio    CCaccu;     ///< bits map
    enum
    {
        toIdentity  = 0,    ///< target: identity matrix
        toGray      = 1,    ///< target: gray
        ENUM_DUMMY4WORD(SupSel)
    }SupSel;    ///< color supress option
    UINT32  CCcoef_TblAddr; ///< tab num:9, Data type:INT16
    UINT32  Fstab_TblAddr;   ///< tab num:16, Data type:UINT8
    UINT32  Fdtab_TblAddr;   ///< tab num:16, Data type:UINT8
}IPE_CC;

/**
    color space transfor parameter set
*/
typedef struct _IPE_CST
{
    UINT8   Coef0;  ///< -
    UINT8   Coef1;  ///< -
    UINT8   Coef2;  ///< -
    UINT8   Coef3;  ///< -
}IPE_CST;

/**
    color control parameter set
*/
typedef struct _IPE_CCTRL
{
    INT16   IntOfs;         ///< Legal range : -128~127, Y offset, Y' = Y + Intofs
    INT16   SatOfs;         ///< Legal range : -128~127, color saturation adjust, 0 : NO color saturation adjust
    BOOL    HueRotateEn;    ///< Hue adjust rotation option, 0 : NO rotation, 1 : depend on hue tab LSB 2 bit, 0 : 0 degree rotation, 1 : 90 degree rotation, 2 bit = 2 : 180 degree rotation, 3 : 270 degree rotation

    UINT32  HueTab_TblAddr; ///< Legal range : 0~255, 128 : NO hue adjust. (tab num:24, Data Type:UINT8)
    UINT32  SatTab_TblAddr; ///< Legal range : -128~127, 0 : NO sat adjust. (tab num:24, Data Type:INT32)
    UINT32  IntTab_TblAddr; ///< Legal range : -128~127, 0 : NO sat adjust. (tab num:24, Data Type:INT32)
    UINT32  EdgTab_TblAddr; ///< Legal range : 0~255, 128 : NO edge adjust.(tab num:24, Data Type:UINT8)
    UINT32  DDSTab_TblAddr; ///< Legal range : 0~255, weighting table for SatTab/IntTab/EdgTab. 0 : NO adjust, 32 : 1X adjust weighting
                            ///< (tab num:8, Data Type:UINT8)
    struct
    {
        EDGE_OUT    EdgSel; ///< -
        UINT8   VdetDiv;    ///< -
    }Suppress;              ///< color suppression edge ctrl
}IPE_CCTRL;

/*
    Cb Cr color offset,
*/
typedef struct _IPE_CCOFS
{
    UINT8   Cbofs;  ///< Legal range : 0~255, Cb color offset, 128 : NO color offset
    UINT8   Crofs;  ///< -
}IPE_CCOFS;

/**
    Y/CbCr contrast
*/
typedef struct _IPE_CCON
{
    UINT8           YCon;   ///< Legal range : 0~255, Y contrast adjust, 128 : NO contrst adjust, Y' = (Y * Ycon) >> 7
    UINT8           UVCon;   ///< Legal range : 0~255, CbCr contrast adjust, 128 : NO contrst adjust, C' = (C * CCon) >> 7
}IPE_CCON;

/**
    ipe random noise
*/
typedef struct _IPE_RAND_NR
{
    BOOL    RstEn;  ///< Legal range : 0~1, 0 : No reset for random pattern sequence, 1 : Frame start reset for random pattern seqeunce.
    UINT32  Y_lv;   ///< Legal range : 0~7, 0 : NO Y random noise, 7 : MAX Y random noise level
    UINT32  CC_lv;  ///< Legal range : 0~7, 0 : NO CbCr random noise, 7 : MAX CbCr random noise level
}IPE_RAND_NR;

/**
    color mask effect
*/
typedef struct _IPE_YCCMASK
{
    UINT8   Ymask;  ///< -
    UINT8   Cbmask; ///< -
    UINT8   Crmask; ///< -
}IPE_YCCMASK;

/**
    YCC fix effect
*/
typedef struct _IPE_YCCFIX
{
    struct
    {
        UINT32  EdgeThres;              ///< -
        UINT32  Threshold;              ///< -
        BOOL    HIT_FixOutputEn;        ///< replace output as HIT_value when (input>=Threshold)&&(edge>=EdgeThres), if enabled
        BOOL    NONEHIT_FixOutputEn;    ///< replace output as NONEHIT_value when (input<Threshold)||(edge<EdgeThres), if enabled
        UINT32  HIT_value;              ///< -
        UINT32  NONHIT_value;           ///< -
    }Yth1;                              ///< Y fix effect(with edge condition)

    struct
    {
        UINT32  Threshold;              ///< -
        BOOL    HIT_FixOutputEn;        ///< replace output as HIT_value when (input>=Threshold), if enabled
        BOOL    NONEHIT_FixOutputEn;    ///< replace output as NONEHIT_value when (input<Threshold), if enabled
        UINT32  HIT_value;              ///< -
        UINT32  NONHIT_value;           ///< -
    }Yth2;                              ///< Y fix effect

    struct
    {
        UINT32  EdgeThres;          ///< -
        ThresLV YThres;             ///< -
        ThresLV CbThres;            ///< -
        ThresLV CrThres;            ///< -
        BOOL    HIT_FixOutputEn;        ///< replace output as HIT_value when ((edge <= C_ETH) ||
                                        ///<    ((y >= C_YTH_L) && (y <= C_YTH_H) &&
                                        ///<    (cb >= C_CBTH_L) && (cb <= C_CBTH_H) &&
                                        ///<    (cr >= C_CRTH_L) && (cr <= C_CRTH_H))
                                        ///<    ), if enabled
        BOOL    NONEHIT_FixOutputEn;    ///< replace output as NONEHIT_value when above condition unstisfied, if enabled
        UINT32  Cb_HIT_value;       ///< -
        UINT32  Cb_NONHIT_value;    ///< -
        UINT32  Cr_HIT_value;       ///< -
        UINT32  Cr_NONHIT_value;    ///< -
    }CC;                                ///< CC fix effect
}IPE_YCCFIX;

/**
    color space transfor protection
*/
typedef struct _IPE_CSTP
{
    UINT8   CSTPratio;///< color space tranfor protection ratio
}IPE_CSTP;

/**
    CbCr subsample output parameter
*/
typedef struct _IPE_CC_SUBOUT
{
    struct
    {
        UINT32  H;  ///< -
        UINT32  V;  ///< -
        UINT32  Lineofs;    ///< -
    }Size;  ///< -
    UINT32 Addr;    ///< -
}IPE_CC_SUBOUT;

/**
    v channel output data enum
*/
typedef enum _VA_OutSel
{
    v_Ych_out   = 0,    ///< Y format
    v_Vch_out   = 1,    ///< V format
    v_ESD_out   = 2,    ///< ESD
    v_E5_out    = 3,    ///< 5x5 edge kernel
    v_E7_out    = 4,    ///< 7x7
    ENUM_DUMMY4WORD(VA_OutSel)
}VA_OutSel;

/**
    V subsample output parameter
*/
typedef struct _IPE_V_SUBOUT
{
    VA_OutSel   OutSel; ///< v channel output data selection
    struct
    {
        UINT32  H;  ///< -
        UINT32  V;  ///< -
        UINT32  Lineofs;    ///< -
    }Size;      ///< -
    UINT32  Addr;   ///< -
}IPE_V_SUBOUT;

/*
    IPE RGB gamma table,each gamma curve have 129 steps
*/
typedef struct _IPEGAMMA
{
    IPEGAMTAB  R[65];
    IPEGAMTAB  G[65];
    IPEGAMTAB  B[65];
}IPEGAMMA;

/**
    ipe R/G/B gamma curve
*/
typedef struct _IPE_GAMMA
{
    UINT32          GammaTbl_TblAddr;   ///< Data type: IPEGAMMA
}IPE_GAMMA;

/**
    ipe 3d color control
*/
typedef struct _IPE_3DCC
{
    UINT32          TCC_TblAddr;        ///<3d color control, (tab num:729, Data type: IPE3DC)
}IPE_3DCC;

/**
    IPE DVI input
*/
typedef struct _IPEDVI
{
    BOOL DVIEn;     ///< enable bit
    enum
    {
        CbYCrY  = 0,    ///< order is Cb, Y, Cr, Y
        CrYCbY  = 1,    ///< order is Cr, Y, Cb, Y
        YCrYCb  = 2,    ///< order is Y, Cr, Y, Cb
        YCbYCr  = 3,    ///< order is Y, Cb, Y, Cr
        ENUM_DUMMY4WORD(IPEDVIFmt)
    }IPEDVIFmt;         ///< DVI input format
    enum
    {
        HSUB_1X = 0,    ///< 640x480 input Y->640x480 output Y
        HSUB_2X = 1,    ///< 640x480 input Y->320x480 output Y
        HSUB_4X = 2,    ///< 640x480 input Y->160x480 output Y
        ENUM_DUMMY4WORD(DVISub)
    }DVISub;            ///< H subsample
}IPEDVI;   ///< effective if IPEMode is IPE_DVI

/**
     IPE sub function configuration.
*/
typedef struct _IPE_SUBFUNC
{
    IPEDVI  dvi;                    ///< DVI input(CCIR)
    IPEINTE IpeInteEn;              ///< interrupt enable

    IPEFUNCEN IpeFuncEn;            ///< function enable


    IPE_CG          Cg;             ///< color gain(Bayer)
    IPE_GBAL        *pGbal;         ///< G balance
    IPE_CFAINTER    *pCFAInterpolation; ///< CFA interpolation
    IPE_EDGEEXT     *pedgeExt;      ///< edge extraction
    IPE_EDGEENH     *pedgeEnh;      ///< edge enhancement
    IPE_EDGEMAP     *pedgeMap;      ///< edge mapping
    struct
    {
        IPE_EDGETHRE    *pEdgeThreshold;      ///< edge threshold
        IPE_ETH_DMA     EthDmaInfo;          ///< output dma configuration
    }EdgeThreshold;                 ///< edge dram output
    struct
    {
        IPE__VA          *pVa;      ///<
        VA_WIN  Window;             ///< va window
        UINT32  VAAddr;             ///< VA output addr
        UINT32  VAAddr1;            ///< VA output ppb addr
    }va;                            ///< variation dram output
    IPE__VA_IND      va_ind;        ///< variation independent window
    IPE_RGBLPF      *prgblpf;       ///< rgb low pass filter
    IPE_CC          *pCC;           ///< color correction
    IPE_CCTRL       *pCctrl;        ///< color control
    IPE_CCOFS       *pCbCrofs;      ///< Cb/Cr color offset
    IPE_CCON        *pYCCcon;       ///< Y/CC contrast
    IPE_RAND_NR     *pRandNoise;    ///< Random noise
    IPE_YCCMASK     *pyccmask;      ///< color mask
    IPE_CST         *pCST;          ///< color space transfor
    IPE_CSTP        *pCstp;         ///< color space transfor protection
    IPE_YCCFIX      *pYccFix;       ///< Y/CC fix
    IPE_GAMMA       *pGamma;        ///< RGB gamma
    IPE_3DCC        *p3d_cc;        ///< 3D color control
    IPE_CC_SUBOUT   CCout;          ///< CC image sub-sample out
    IPE_V_SUBOUT    Vout;           ///< V image sub-sample out
}IPE_SUBFUNC;

//@}
#endif
