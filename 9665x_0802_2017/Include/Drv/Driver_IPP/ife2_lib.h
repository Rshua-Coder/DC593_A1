/*
    Public header file for IFE2 module.

    This file is the header file that define the API and data type for IFE2
    module.

    @file       ife2_lib.h
    @ingroup    mIIPPIFE

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IFE2_LIB_650_H
#define _IFE2_LIB_650_H

#define _IFE2_LIB_H_FOR_FW_ (1)
#define _IFE2_LIB_H_FOR_GEARGEN_ (0)

#if _IFE2_LIB_H_FOR_FW_
// include files for FW
#include "Type.h"
#elif _IFE2_LIB_H_FOR_GEARGEN_
// include files for GearGen
#include    "..\\type.h"
#endif//_IFE2_LIB_H_FOR_GEARGEN_

/**
    @addtogroup mIIPPIFE
*/
//@{

/**
IFE2 Definition

@name   IFE2_Define
*/
//@{

/*
    Define IFE2 function enable.
*/
//@{
#define IFE2_CHROMA_ADJ_EN                  0x00000001  ///< enable function: row defect concealment
#define IFE2_AVG_FILTER_EN                     0x00000002  ///< enable function: outlier

//@}


/**
    Define IFE2 interrput enable.
*/
//@{
#define IFE2_INTE_FRMEND                 0x00000001  ///< frame end interrupt
#define IFE2_INTE_ALL                    0x0000000F  ///< all interrupts
//@}

/**
    Define IFE2 interrput status.
*/
//@{
#define IFE2_INT_FRMEND                 0x00000001  ///< frame end interrupt
#define IFE2_INT_ALL                    0x0000000F  ///< all interrupts
//@}

//@}

/**
IFE2 Enumeration

@name   IFE2_Enumeration
*/
//@{

/**
    IFE2 Source Format.

    Set IFE2 input data format
\n  Used for ife2_setMode()
*/
//@{
typedef enum
{
    IFE2_FMT_UVPACKING   = 0,           ///< UV Packing
    IFE2_FMT_SINGLE      = 1,           ///< Single Channel
    ENUM_DUMMY4WORD(IFE2_SRCFMT)
} IFE2_SRCFMT;
//@}

/**
    IFE2 Filter Debug Mode.

    Set IFE2 filter debug mode
\n  Used for ife2_setMode()
*/
//@{
typedef enum
{
    IFE2_FLTR_DEBUG_MODE_NORMAL   = 0,           ///< Normal Operation Mode
    IFE2_FLTR_DEBUG_MODE_COUNT      = 1,           ///< Debug Mode, output count
    IFE2_FLTR_DEBUG_MODE_WEIGHT      = 2,           ///< Debug Mode, output weight
    ENUM_DUMMY4WORD(IFE2_FLTRDEBUGMODE)
} IFE2_FLTRDEBUGMODE;
//@}

/**
    IFE2 Filter Size Selection.

    Set IFE2 filter size
\n  Used for ife2_setMode()
*/
//@{
typedef enum
{
    IFE2_FLTR_SIZE_7x7 = 0,           ///< 7x7 window
    IFE2_FLTR_SIZE_5x5 = 1,           ///< 5x5 window
    IFE2_FLTR_SIZE_3x3 = 2,           ///< 3x3 window
    ENUM_DUMMY4WORD(IFE2_FILTSIZE)
} IFE2_FILTSIZE;
//@}

/**
    IFE Wait Mode Selection.

    IFE Wait Mode Selection.
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE2_WAIT    = 0,                ///< Wait for frame end
    IFE2_NOWAIT  = 1,                ///< Do not wait for frame end
    ENUM_DUMMY4WORD(IFE2_WAITMODE)
} IFE2_WAITMODE;
//@}

//@}

/**
IFE2 Struct

@name   IFE2_Struct
*/
//@{

/**
    Struct IFE open object.

    ISR callback function
*/
//@{
typedef struct _IFE2_OPENOBJ
{
    void (*pfIfe2IsrCb)(UINT32 uiIntpStatus); ///< isr callback function
    UINT32 uiIfe2ClkSel;                    ///< Engine clock-rate in MHz; Selection: 48/60/80/120/160/192/220/240
} IFE2_OPENOBJ;
//@}

/**
    Struct IFE2 Chroma Adjust Setting.

    Parameters of Chroma Adjust
*/
//@{
typedef struct _IFE2_CHROMAADJSET
{
    UINT32 uiRefCb;           ///< Cb reference
    UINT32 uiRefCr;           ///< Cr reference
    UINT32 uiVarTh[4];           ///< variance threshold
    UINT32 uiVarWt[5];           ///< variance weight
} IFE2_CHROMAADJSET;
//@}

/**
    Struct IFE2 Average Filter Setting.

    Parameters of Average Filter
*/
//@{
typedef struct _IFE2_AVGFILTERSET
{
    UINT32 uiCntrWt;           ///< center weight
    UINT32 uiAvgTh[2];           ///< average threshold
    UINT32 uiCntTh;           ///< count threshold
    IFE2_FILTSIZE FltrSz;           ///< filter size
} IFE2_AVGFILTERSET;
//@}

/**
    Struct IFE Information.

    IFE parameter and function selection

\n  Used for ife_setMode()
*/
//@{
typedef struct _IFE2_PARAM
{
    BOOL bChromaAdjEn;                ///< enable of chroma adjust
    BOOL bAvgFltrEn;                ///< enable of average filter
    IFE2_SRCFMT     SrcFmt;         ///< source format select
    IFE2_FLTRDEBUGMODE     FltrDbgMd;         ///< average filter debug mode select
    UINT32 uiHeight;              ///< image height
    UINT32 uiWidth;               ///< image width
    UINT32 uiInAddr;             ///< input starting address
    UINT32 uiInOfs;                ///< image input lineoffset
    UINT32 uiOutAddr;            ///< output starting address
    UINT32 uiOutOfs;                ///< image output lineoffset
    UINT32 uiIntrpEn;              ///< interrupt enable
    IFE2_CHROMAADJSET ChromaAdjSet;    ///< chroma adjust setting
    IFE2_AVGFILTERSET AvgFltrCbSet;    ///< average filter Cb setting
    IFE2_AVGFILTERSET AvgFltrCrSet;    ///< average filter Cr setting
} IFE2_PARAM;
//@}

//@}

/**
IFE2 API

@name   IFE2_API
*/
//@{

extern ER       ife2_open(IFE2_OPENOBJ *pObjCB);
extern ER       ife2_close(void);
extern BOOL     ife2_isOpened(void);

extern ER       ife2_setMode(IFE2_PARAM *pIfe2Param);
extern void     ife2_waitFlagFrameEnd(void);
extern void     ife2_clearFlagFrameEnd(void);
extern BOOL     ife2_checkFlagFrameEnd(void);

extern UINT32   ife2_getIntStatus(void);
extern ER       ife2_clearInt(UINT32 uiIntrp);
extern ER       ife2_disableInt(UINT32 uiIntrp);
extern ER       ife2_enableInt(UINT32 uiIntrp);

extern ER      ife2_setDmaInAddr(UINT32 uiInAddr, UINT32 uiInOfs);
extern ER ife2_getDmaInAddr(UINT32 *puiInAddr, UINT32 *puiInOfs);
extern ER      ife2_setDmaOutAddr(UINT32 uiOutAddr, UINT32 uiOutOfs);
extern ER ife2_getDmaOutAddr(UINT32 *puiOutAddr, UINT32 *puiOutOfs);

extern ER ife2_setInSize(UINT32 uiHSize, UINT32 uiVSize);
extern ER ife2_getInSize(UINT32 *puiHSize, UINT32 *puiVSize);
extern ER ife2_getSrcFmt(IFE2_SRCFMT *pSrcFmt);

extern ER      ife2_start(IFE2_WAITMODE Ife2WaitMode);
//moved to ife2_int.h//extern ER ife2_clr(void);
extern ER       ife2_enableFunction(BOOL bEnable, UINT32 uiFunc);
extern ER ife2_pause(void);

extern ER ife2_changeChromaAdj(IFE2_CHROMAADJSET *pChromaAdjSet, BOOL bEn);
extern ER ife2_changeAvgFltr(IFE2_AVGFILTERSET *pAvgFltrCbSet, IFE2_AVGFILTERSET *pAvgFltrCrSet, BOOL bEn);

extern UINT32 ife2_getClockRate(void);

#endif
//@}

//@}
