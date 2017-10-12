/*
    Public header file for RDE module.

    This file is the header file that define the API and data type for RDE
    module.

    @file       rde_lib.h
    @ingroup    mIIPPRDE
    @note

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _RDE_LIB_H
#define _RDE_LIB_H

#include    "Type.h"//a header for basic variable type

/**
    @addtogroup mIIPPRDE
*/
//@{

/**
    Define RDE interrput enable.
*/
//@{
#define RDE_INTE_FRMEND                 0x00000001  ///< Enable interrupt of frame end
#define RDE_INTE_CODERR                 0x00000002  ///< Enable interrupt of coding error
#define RDE_INTE_BS_UDFL_ERR            0x00000004  ///< Enable interrupt of bitstream size underflow error
#define RDE_INTE_BS_OVFL_ERR            0x00000008  ///< Enable interrupt of bitstream size overflow error
#define RDE_INTE_ALL                    0x0000000F  ///< Enable all interrupts
//@}

/**
    Define RDE interrput status.
*/
//@{
#define RDE_INT_FRMEND                 0x00000001  ///< Status of interrupt of frame end
#define RDE_INT_CODERR                 0x00000002  ///< Status of interrupt of coding error
#define RDE_INT_BS_UDFL_ERR            0x00000004  ///< Status of interrupt of bitstream size underflow error
#define RDE_INT_BS_OVFL_ERR            0x00000008  ///< Status of interrupt of bitstream size overflow error
#define RDE_INT_ALL                    0x0000000F  ///< Status of all interrupts
//@}

/*----------- engine control enumeration ------------*/

/**
    State of State Machine
*/
//@{
typedef enum
{
    RDE_ENGINE_IDLE  = 0,        ///< Idel state
    RDE_ENGINE_READY = 1,        ///< Ready state
    RDE_ENGINE_PAUSE = 2,        ///< Pause state
    RDE_ENGINE_RUN   = 3,        ///< Run state

    ENUM_DUMMY4WORD(RDE_STATE_MACHINE)
} RDE_STATE_MACHINE;
//@}

/**
    Operations of engine action
*/
//@{
typedef enum
{
    RDE_OP_OPEN          = 0,    ///< Open engine
    RDE_OP_CLOSE         = 1,    ///< Close engine
    RDE_OP_SETPARAM      = 2,    ///< Set parameter
    RDE_OP_SET2READY     = 3,    ///< Set to ready
    RDE_OP_START         = 4,    ///< Start engine
    RDE_OP_PAUSE         = 5,    ///< Pause engine

    ENUM_DUMMY4WORD(RDE_ACTION_OP)
} RDE_ACTION_OP;
//@}

/*----------- engine driver structure -------------*/
/*
    RDE Open Object - Open Object is used for opening RDE module.
*/
typedef struct _RDE_OPENOBJ
{
    void (*FP_RDEISR_CB)(UINT32 uiIntStatus); ///< isr callback function
    UINT32 RDE_CLOCKSEL;
} RDE_OPENOBJ;

/**
    RDE output setting

    Structure of settings of output function
*/
//@{
typedef struct _RDE_FUNC_EN_STRUCT
{
    BOOL bHFlipEn;  ///< Enable horizontal flip
    BOOL bVFlipEn;  ///< Enable vertical flip
    BOOL bDegammaEn; ///< Enable degamma
} RDE_FUNC_EN;
//@}

/**
    RDE DMA in info

    Structure of setting of DMA input
*/
//@{
typedef struct _RDE_DMA_IN_INFO_STRUCT
{
    UINT32 uiBsiStartAddr;  ///< Start address of input bit stream
    UINT32 uiBsiLength;      ///< Length of input Bit stream
} RDE_DMA_IN_INFO;
//@}

/**
    RDE DMA out info

    Structure of setting of DMA output
*/
//@{
typedef struct _RDE_DMA_OUT_INFO_STRUCT
{
    UINT32 uiImgoStartAddr;     ///< Start address of output image
    UINT32 uiImgoLineOfst;      ///< Line offset of output image
} RDE_DMA_OUT_INFO;
//@}

/**
    RDE image size info

    Structure of target image size
*/
//@{
typedef struct _RDE_IMG_SIZE_STRUCT
{
    UINT32 uiActWidth;   ///< Width of output image
    UINT32 uiActHeight;  ///< height of output image
} RDE_IMG_SIZE;
//@}

/**
    RDE engine parameter for library

    Structure of all avaliable engine parameter
*/
//@{
typedef struct _RDE_PARAM_STRUCT
{
    RDE_FUNC_EN FuncEn;             ///< Enable function of RDE
    UINT32 uiOutGain;               ///< Output gain
    BOOL bOutPadSel;                ///< Padding method of output
    RDE_DMA_IN_INFO  DmaInInfo;     ///< Info of DMA input
    RDE_DMA_OUT_INFO DmaOutInfo;    ///< Info of DMA output
    RDE_IMG_SIZE TrgtSizeInfo;      ///< Size info of output image
    UINT32 *puiDegammaTblPtr;        ///< Pointer of degamma table
} RDE_PARAM;
//@}


// external function declaration
extern UINT32 rde_getIntStatus(void);
extern ER rde_enableInt(UINT32 uiIntrEn);
extern ER rde_clearIntStatus(UINT32 uiIntrStatus);
extern ER rde_waitFrameEnd(void);
extern ER rde_clearFrameEndFlag(void);
extern ER rde_setFunctionEnable(RDE_FUNC_EN *FuncEn);
extern ER rde_setOutGain(UINT32 uiOutGain);
extern ER rde_setOutPadSel(BOOL bOutPadSel);
extern ER rde_setDMAInInfo(UINT32 uiBsiStartAddr, UINT32 uiBsiLength);
extern ER rde_getDMAInInfo(UINT32 *puiBsiStartAddrPtr, UINT32 *puiBsiLengthPtr);
extern ER rde_setDMAOutInfo(UINT32 uiImgoStartAddr, UINT32 uiImgoLineOfst);
extern ER rde_getDMAOutInfo(UINT32 *puiImgoStartAddrPtr, UINT32 *puiImgoLineOfstPtr);
extern UINT32 rde_getCurBsLength(void);
extern UINT32 rde_getClockRate(VOID);//130515: Add this for PLL change
//Available clock rate(MHz):
//48, 60, 80, 120, 160, 192, 220
extern ER rde_setMode(RDE_PARAM *pRdeParamObj);
extern ER rde_open(RDE_OPENOBJ *pRdeObjCB);
extern ER rde_start(void);
extern ER rde_pause(void);
extern ER rde_close(void);

#endif

//@}
