/**
    Public header file for ISE module.

    @file       ime_lib.h
    @ingroup    mIIPPISE
    
    @brief

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/


#ifndef _ISE_LIB_H_
#define _ISE_LIB_H_

#include "Type.h"


typedef void (*ISEIntCB)(UINT32);


//========================================================================
// extern data type

#define ISE_INTE_CLR         0x00000000  ///< disable interrupt
#define ISE_INTE_FRMEND      0x80000000  ///< frame-end interrupt enable
#define ISE_INT_FRMEND       0x80000000  ///< frame-end interrupt status


/**
    @name ISE enum - Input and output packing format selection
*/
//@{
typedef enum _ISE_IO_PACK_SEL
{
  ISE_Y8 = 0,        ///< Y-8bit packing
  ISE_Y4 = 1,        ///< Y-4bit packing
  ISE_UVP = 2,       ///< UV packing
  ISE_RGB565 = 3,    ///< RGB565 packing
  ENUM_DUMMY4WORD(ISE_IO_PACK_SEL)
} ISE_IO_PACK_SEL;
//@}

/**
    @name ISE enum - Image scaling method selection
*/
//@{
typedef enum _ISE_SCALE_METHOD
{
    ISE_BILINEAR    = 1,        ///< bilinear interpolation
    ISE_NEAREST  = 2,        ///< nearest interpolation
    ENUM_DUMMY4WORD(ISE_SCALE_METHOD)
} ISE_SCALE_METHOD;
//@}

/**
    @name ISE enum - Burst length selection
*/
//@{
typedef enum _ISE_BURST_SEL
{
  ISE_BURST_32WORD = 0,  ///< burst 32 word
  ISE_BURST_16WORD = 1,  ///< burst 16 word
  ENUM_DUMMY4WORD(ISE_BURST_SEL)
} ISE_BURST_SEL;
//@}


/**
    @name ISE enum - Overlap width selection
*/
//@{
typedef enum _ISE_OVERLAP_SEL
{
  ISE_OVERLAP_HW_AUTO = 0,  ///< overlap width selection, HW auto
  ISE_OVERLAP_FW_SET  = 1,  ///< overlap width selection, FW setting
  ENUM_DUMMY4WORD(ISE_OVERLAP_SEL)
} ISE_OVERLAP_SEL;
//@}


/**
    @name ISE enum - Get engine information selection
*/
//@{
typedef enum _ISE_GET_INFO_SEL
{
  ISE_GET_IN_H_SIZE       = 0,   ///< get input width size
  ISE_GET_IN_V_SIZE       = 1,   ///< get input height size
  ISE_GET_IN_OFS          = 2,   ///< get input lineoffset size
  ISE_GET_IN_ADDR         = 3,   ///< get input buffer address
  ISE_GET_OUT_H_SIZE      = 4,   ///< get output width size
  ISE_GET_OUT_V_SIZE      = 5,   ///< get output height size
  ISE_GET_OUT_OFS         = 6,   ///< get output lineoffset
  ISE_GET_OUT_ADDR        = 7,   ///< get output buffer address
  ISE_GET_INTERRUPT       = 8,   ///< get interrupt status
  ISE_GET_INTERRUPT_EN    = 9,   ///< get interrupt enable
  ENUM_DUMMY4WORD(ISE_GET_INFO_SEL)
} ISE_GET_INFO_SEL;
//@}

/**
    @name ISE enum - Clear Flag
*/
//@{
typedef enum _ISE_FLAG_CLEAR_SEL
{
  ISE_FLAG_NO_CLEAR = 0,    ///< No clear flag
  ISE_FLAG_CLEAR    = 1,    ///< Clear flag
  ENUM_DUMMY4WORD(ISE_FLAG_CLEAR_SEL)
} ISE_FLAG_CLEAR_SEL;
//@}



/**
    ISE structure - Engine open object
*/
//@{
typedef struct _ISE_OPENOBJ
{
  void (*FP_ISEISR_CB)(UINT32 intstatus);   ///< External ISR call-back function
  UINT32 uiIseClockSel;                      ///< Engine clock selection
} ISE_OPENOBJ,*pISE_OPENOBJ;
//@}



/**
    ISE structure - Overlap size setting structure
*/
typedef struct _ISE_STRIPE_OVERLAP
{
  ISE_OVERLAP_SEL  OverlapMode;     ///< input stripe overlap mode
  UINT32           uiOverlapSize;   ///< input stripe overlap size if using FW setting mode
} ISE_STRIPE_OVERLAP;


/**
    ISE structure - Engine flow setting structure
*/
//@{
typedef struct _ISE_MODE_PRAM
{
  ISE_IO_PACK_SEL  IOPackFormat;          ///< input and output packing format
  ISE_SCALE_METHOD ScaleMethod;       ///< scaling method selection
  ISE_STRIPE_OVERLAP   SetStripeOvlapSize; ///< input stripe overlap size
  
  UINT32 uiInWidth;                    ///< input image width
  UINT32 uiInHeight;                   ///< input image height
  UINT32 uiInLineoffset;               ///< input lineoffset
  UINT32 uiInAddr;                     ///< input buffer address

  UINT32 uiOutWidth;                   ///< output image width
  UINT32 uiOutHeight;                  ///< output image height
  UINT32 uiOutLineoffset;              ///< output lineoffset
  UINT32 uiOutAddr;                    ///< output buffer address
}ISE_MODE_PRAM,*pISE_MODE_PRAM;
//@}



//========================================================================
// extern APIs
extern BOOL ise_isOpen(VOID);

/*
  Open ISE engine

  This function should be called before calling any other functions.

  @param PIME_OpenInfo ISR callback function and clock selection.
        -@b Supported clocks: 48, 60, 80, 120, 160, 192, 220Mhz

  @return
        - @b E_OK: Done without error.
        - Others: Error occured.
*/
extern ER ise_open(ISE_OPENOBJ *pIME_OpenInfo);

extern ER ise_setMode(ISE_MODE_PRAM *pEngineInfo);
extern ER ise_start(VOID);
extern ER ise_reset(VOID);
extern ER ise_pause(VOID);
extern ER ise_close(VOID);

extern ER ise_waitFlagFrameEnd(ISE_FLAG_CLEAR_SEL IsFlagClr);
extern ER ise_clrIntFlag(VOID);

extern UINT32 ise_getEngineInfo(ISE_GET_INFO_SEL GetInfoSel);

extern UINT32 ise_getClockRate(VOID); 

#if (_EMULATION_ == ENABLE)
extern VOID ise_setUserScaleFilterParamss(UINT32 uiHFilterEn, UINT32 uiHFilterCoef, UINT32 uiVFilterEn, UINT32 uiVFilterCoef);
#endif

#endif

