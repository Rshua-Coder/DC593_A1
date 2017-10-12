/*
    Public header file for IFE module.

    This file is the header file that define the API and data type for IFE
    module.

    @file       ife_lib.h
    @ingroup    mIIPPIFE

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _IFE_LIB_650_H
#define _IFE_LIB_650_H

/**
    @addtogroup mIIPPIFE
*/
//@{

/*
    Define IFE function enable.
*/
//@{
#define IFE_ROWDEFC_EN                  0x00000010  ///< enable : row defect
#define IFE_OUTL_EN                     0x00000020  ///< enable : outlier
#define IFE_FILTER_EN                   0x00000040  ///< enable : filter
#define IFE_CRVMAP_EN                   0x00000080  ///< enable : curve mapping
#define IFE_CGAIN_EN                    0x00001000  ///< enable : color gain

//@}


/**
    Define IFE interrput enable.
*/
//@{
#define IFE_INTE_FRMEND                 0x00000001  ///< frame end interrupt
//#define IFE_INTE_BP1                  0x00000002  ///< break point 1 interrupt
#define IFE_INTE_ROWDEFFAIL             0x00000004  ///< row def conc interrupt
#define IFE_INTE_STRPEND                0x00000008  ///< H stripe end interrupt
#define IFE_INTE_ALL                    0x0000000D  ///< all interrupts
//@}

/**
    Define IFE interrput status.
*/
//@{
#define IFE_INT_FRMEND                 0x00000001  ///< frame end interrupt
//#define IFE_INT_BP1                  0x00000002  ///< break point 1 interrupt
#define IFE_INT_ROWDEFFAIL             0x00000004  ///< row def conc interrupt
#define IFE_INT_STRPEND                0x00000008  ///< H stripe end interrupt
#define IFE_INT_ALL                    0x0000000D  ///< all interrupts
//@}

/**
    IFE Function Selection.

    IFE Function Selection.
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_MODE_D2D      = 0,  ///< Input from DRAM and output to DRAM
    IFE_MODE_DIRECT   = 1,  ///< Direct path from PRE to IPE
    IFE_MODE_PRE_DRAM = 2,  ///< PRE input and DRAM output
    IFE_MODE_UNKNOWM  = 3,  ///< Unknown mode
    ENUM_DUMMY4WORD(IFE_MODE)
} IFE_MODE;
//@}

/**
    IFE Source Format.

    Set IFE input data format
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_FMT_NORMAL   = 0,           ///< Prograssive data input
    IFE_FMT_RAW      = 1,           ///< RAW data input
    IFE_FMT_UVPACK   = 2,           ///< UV packing input
    ENUM_DUMMY4WORD(IFE_SRCFMT)
} IFE_SRCFMT;
//@}

/**
    IFE Filter mode Selection.

    Set IFE filter mode
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_NORMAL_9x9 = 0,           ///< standard filter in 9x9 area
    IFE_SONLY_9x9  = 1,           ///< spatial filter only in 9x9 area
    ENUM_DUMMY4WORD(IFE_FILTMODE)
} IFE_FILTMODE;
//@}

/**
    IFE binning Selection.

    binning level selection
*/
//@{
typedef enum
{
    IFE_BINNING_DISABLE = 0,        ///< Disable binning
    IFE_BINNINGX2       = 1,        ///< binning x2
    IFE_BINNINGX4       = 2,        ///< binning x4
    IFE_BINNINGX8       = 3,        ///< binning x8
    IFE_BINNINGX16      = 4,        ///< binning x16
    IFE_BINNINGX32      = 5,        ///< binning x32
    IFE_BINNINGX64      = 6,        ///< binning x64
    IFE_BINNINGX128     = 7,        ///< binning x128
    ENUM_DUMMY4WORD(IFE_BINNSEL)
} IFE_BINNSEL;
//@}

/**
    IFE CFA Pattern Selection.

    Set IFE start pixel of the CFA pattern
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_SONLY_3x3 = 0,           ///< 3x3 spatial filter in 5x5 area
    IFE_SONLY_5x5 = 1,           ///< spatial filter only in 5x5 area
    ENUM_DUMMY4WORD(IFE_SONLYLEN)
} IFE_SONLYLEN;
//@}


/**
    IFE Delta of Curve Mapping Selection.

    Set IFE step size between two table entries.
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_DELTA_1 = 0,           ///< step size = 1
    IFE_DELTA_2 = 1,           ///< step size = 2
    IFE_DELTA_4 = 2,           ///< step size = 4
    ENUM_DUMMY4WORD(IFE_CRVMAP_DELTA)
} IFE_CRVMAP_DELTA;
//@}

/**
    IFE Outlier Count Selection.

    Set IFE number of neighbors to establish outlier condition.
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_EIGHT_NEIGHBOURS = 0,           ///< eight neighbours fall in the criteria
    IFE_SEVEN_NEIGHBOURS = 1,           ///< seven neighbours fall in the criteria
    ENUM_DUMMY4WORD(IFE_OUTL_SEL)
} IFE_OUTL_SEL;
//@}

/**
    IFE Row Defect Concealment Factor Selection.

    Set IFE factor in row defect concealment.
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_FACT_p5 = 0,           ///< factor = 0.5
    IFE_FACT_p25 = 1,          ///< factor = 0.25
    IFE_FACT_p0  = 2,          ///< factor = 0
    ENUM_DUMMY4WORD(IFE_ROWDEF_FACT)
} IFE_ROWDEF_FACT;
//@}

/**
    IFE Wait Mode Selection.

    IFE Wait Mode Selection.
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_WAIT    = 0,                ///< Wait for frame end
    IFE_NOWAIT  = 1,                ///< Do not wait for frame end
    ENUM_DUMMY4WORD(IFE_WAITMODE)
} IFE_WAITMODE;
//@}


/**
    IFE Bit Depth.

    Set IFE input/output bitdepth
*/
//@{
typedef enum
{
    IFE_8BIT    = 0,                ///< 8bit data,  image width should be 4x
    IFE_10BIT   = 1,                ///< 8bit data,  image width should be 16x
    IFE_12BIT   = 2,                ///< 12bit data, image width should be 8x
    IFE_16BIT   = 3,                ///< 16bit data, image width should be 4x
    ENUM_DUMMY4WORD(IFE_BITDEPTH)
} IFE_BITDEPTH;
//@}

/**
    Struct IFE open object.

    ISR callback function
*/
//@{
typedef struct _IFE_OPENOBJ
{
    VOID (*FP_IFEISR_CB)(UINT32 uiIntStatus); ///< isr callback function
    UINT32 uiIfeClockSel; ///< support 220/192/160/120/80/60/48 Mhz
} IFE_OPENOBJ;
//@}

/**
    Struct IFE Range Filter Setting.

    Parameters of range filter
*/
//@{
typedef struct _IFE_RANGESET
{
    UINT32 *pRngTh;            ///< range filter threshold
    UINT32 RngDelta;           ///< range filter threshold adjustment
} IFE_RANGESET;
//@}

/**
    Struct IFE Clamp and Weighting.

    Parameters of clamp and weight
*/
//@{
typedef struct _IFE_CLAMPWTSET
{
    UINT32 uiClampTh;             ///< clamp threshold
    UINT32 uiClampMul;            ///< weighting multiplier
    UINT32 uiClampDlt;            ///< clamp threshold adjustment
} IFE_CLAMPWTSET;
//@}

/**
    Struct IFE Curve Mapping.

    Parameters of curve mapping
*/
//@{
typedef struct _IFE_CRVMAPSET
{
    IFE_CRVMAP_DELTA CrvMapDelta;   ///< curve mapping delta
    INT32 iCrvMapInOffs;            ///< input offset of curve mapping
    INT32 iCrvMapOutOffs;           ///< output offset of curve mapping
    INT32 iCrvMapInOffs2;           ///< input offset of curve mapping2, used for Cr
    INT32 iCrvMapOutOffs2;          ///< output offset of curve mapping2, used for Cr
    UINT32 *pCrvMapGamma;           ///< curve mapping gamma
} IFE_CRVMAPSET;
//@}

/**
    Struct IFE Color Gain.

    Parameters of color gain
*/
//@{
typedef struct _IFE_CGAINSET
{
    UINT16 uiGain0;   ///< color gain of even line even pixel
    UINT16 uiGain1;   ///< color gain of even line odd pixel
    UINT16 uiGain2;   ///< color gain of odd line even pixel
    UINT16 uiGain3;   ///< color gain of odd line odd pixel
    UINT16 uiGainSel; ///< gain selection, 0: format 2.8, 1: format 3.7
} IFE_CGAINSET;
//@}


/**
    Struct IFE Outlier Filter.

    Parameters of outlier filter
*/
//@{
typedef struct _IFE_OUTLSET
{
    UINT32 *pOutlBriTh;           ///< outlier threshold for bright points
    UINT32 *pOutlDarkTh;          ///< outlier threshold for dark points
    IFE_OUTL_SEL OutlOp;          ///< outlier option for neighbour condition
} IFE_OUTLSET;
//@}

/**
    Struct IFE Row Defect Concealment.

    Parameters of row defect concealment
*/
//@{
typedef struct _IFE_ROWDEFSET
{
    IFE_ROWDEF_FACT RowDefFact; ///< factor of row defect concealment
    UINT32 uiStIdx;             ///< start index of row defect table
    UINT32 uiOffs;              ///< offset of row defect table entry
    UINT32 *pRowDefTbl;         ///< table of row defect concealment
} IFE_ROWDEFSET;
//@}

/**
    Struct IFE Filter Parameters.

    IFE filter detailed parameters.
*/
//@{
typedef struct _IFE_FILT_PARA
{
    IFE_SRCFMT      SrcFmt;         ///< source format select
    IFE_FILTMODE    FiltMode;       ///< filter mode select
    BOOL            bRowDefEn;      ///< enable of row defect concealment
    BOOL            bOutlEn;        ///< enable of outlier filter
    BOOL            bFilterEn;      ///< enable of IFE filter
    BOOL            bCrvMapEn;      ///< enable of curve mapping
    BOOL            bCgainEn;       ///< enable of color gain
    UINT32          *puiWeightS;    ///< spatial weighting
    IFE_RANGESET    RngThA;         ///< range filter threshold setting for filterA
    IFE_RANGESET    RngThB;         ///< range filter threshold setting for filterB
    UINT32          uiBilatW;       ///< range filter A and B weighting
    UINT32          uiRthW;         ///< range filter and spatial filter weighting
    IFE_BINNSEL     Binn;           ///< image binning
    BOOL            bSOnlyEn;       ///< spatial only enable
    IFE_SONLYLEN    SOnlyLen;       ///< filter length of spatial only
    IFE_CLAMPWTSET  ClampWtSet;     ///< clamp and weighting setting
    IFE_CRVMAPSET   CrvMapSet;      ///< curve mapping setting
    IFE_CGAINSET    CgainSet;       ///< color gain setting
    IFE_OUTLSET     OutlSet;        ///< outlier filter setting
    IFE_ROWDEF_FACT RowDefFact;     ///< factor of row defect concealment
    UINT32          uiRowDefStIdx;  ///< start index of row defect table
    UINT32          uiRowDefVIni;   ///< vertical start of cropped window
    UINT32          *puiRowDefTbl;  ///< table of row defect concealment
    UINT32          uiRepeat;       ///< repeat times
} IFE_FILT_PARA;
//@}

/**
    Struct IFE Filter Parameters.

    IFE Size related parameters.
*/
//@{
typedef struct _IFE_SIZE_PARA
{
    UINT32 uiWidth;               ///< image width
    UINT32 uiHeight;              ///< image height
    UINT32 uiOfsi;                ///< image input lineoffset
    UINT32 uiOfso;                ///< image output lineoffset
} IFE_SIZE_PARA;
//@}

/**
    Struct IFE Information.

    IFE parameter and function selection

\n  Used for ife_setMode()
*/
//@{
typedef struct _IFE_PARAM
{
    IFE_MODE        Mode;           ///< operation mode
    IFE_SRCFMT      SrcFmt;         ///< source format select
    IFE_FILTMODE    FiltMode;       ///< filter mode select
    IFE_BITDEPTH    InBit;          ///< input bit select
    IFE_BITDEPTH    OutBit;         ///< out bit select
    BOOL            bRowDefEn;      ///< enable of row defect concealment
    BOOL            bOutlEn;        ///< enable of outlier filter
    BOOL            bFilterEn;      ///< enable of IFE filter
    BOOL            bCrvMapEn;      ///< enable of curve mapping
    BOOL            bCgainEn;       ///< enable of color gain
    UINT32          *puiWeightS;    ///< spatial weighting
    IFE_RANGESET    RngThA;         ///< range filter threshold setting for filterA
    IFE_RANGESET    RngThB;         ///< range filter threshold setting for filterB
    UINT32          uiBilatW;       ///< range filter A and B weighting
    UINT32          uiRthW;         ///< range filter and spatial filter weighting
    IFE_BINNSEL     Binn;           ///< image binning
    BOOL            bSOnlyEn;       ///< spatial only enable
    IFE_SONLYLEN    SOnlyLen;       ///< filter length of spatial only
    IFE_CLAMPWTSET  ClampWtSet;     ///< clamp and weighting setting
    IFE_CRVMAPSET   CrvMapSet;      ///< curve mapping setting
    IFE_CGAINSET    CgainSet;       ///< color gain setting
    IFE_OUTLSET     OutlSet;        ///< outlier filter setting
    IFE_ROWDEF_FACT RowDefFact;     ///< factor of row defect concealment
    UINT32          uiRowDefStIdx;  ///< start index of row defect table
    UINT32          uiRowDefVIni;   ///< vertical start of cropped window
    UINT32          *puiRowDefTbl;  ///< table of row defect concealment
    UINT32          uiInAddr;       ///< input starting address
    UINT32          uiOutAddr;      ///< output starting address
    UINT32          uiWidth;        ///< image width
    UINT32          uiHeight;       ///< image height
    UINT32          uiInOfs;        ///< image input lineoffset
    UINT32          uiOutOfs;       ///< image output lineoffset
    UINT32          uiIntrEn;       ///< interrupt enable
} IFE_PARAM;
//@}

extern ER       ife_setMode(IFE_PARAM *pFilterInfo);
extern ID       ife_getLockStatus(VOID);
extern ER       ife_open(IFE_OPENOBJ *pObjCB);
extern BOOL     ife_isOpened(VOID);
extern ER       ife_close(VOID);
extern ER       ife_enableInt(UINT32 uiIntr);
extern UINT32   ife_getIntEnable(VOID);
extern UINT32   ife_getIntStatus(VOID);
extern ER       ife_clearInt(UINT32 uiIntr);
extern VOID     ife_waitFlagFrameEnd(VOID);
extern ER       ife_enableFunction(BOOL bEnable, UINT32 uiFunc);
extern BOOL     ife_checkFunctionEnable(UINT32 uiFunc);
extern IFE_MODE ife_checkOpMode(VOID);
extern UINT32 ife_getStripeCnt(VOID);
extern BOOL     ife_D2DisBusy(VOID);
extern VOID     ife_changeParam(IFE_FILT_PARA *pFiltPara);
extern VOID     ife_changeSize(IFE_SIZE_PARA *pSizePara);
extern ER       ife_start(VOID);
extern ER       ife_pause(VOID);
extern UINT32   ife_getClockRate(VOID);
//FPGA USE
extern ER ife_setOpMode(IFE_MODE Mode);
#endif

//@}
