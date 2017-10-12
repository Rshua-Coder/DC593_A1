#ifndef _H264_H_
#define _H264_H_

#include "Type.h"

#define H264_PIC_FSH_INT        0x00000001  /*!< Picture Finish Interrupt. */

typedef enum _H264Status{
    H264_STATUS_ERROR_SRC_BUF_IS_NOT_USED = -12,  /*!< Current source buffer is not used. */
    H264_STATUS_ERROR_YADDR_INVALID       = -11,  /*!< Luma address of current picture is invalid. */
    H264_STATUS_ERROR_DISPLAY_FULL        = -10,  /*!< All display-buffer is full. */
    H264_STATUS_ERROR_DISPLAY_EMPTY       = -9,   /*!< There is no extra buffer to be displayed. */
    H264_STATUS_ERROR_REC_BUF_FULL        = -8,   /*!< All recontruction buffer is full. */
    H264_STATUS_ERROR_REGOBJ_EMPTY        = -7,   /*!< There is no register object for h264 to process. */
    H264_STATUS_ERROR_REGOBJ_FULL         = -6,
    H264_STATUS_ERROR_NO_BS_BUF           = -5,   /*!< h264 module is already closed. */
    H264_STATUS_ERROR_NO_SRC_BUF          = -4,   /*!< There is no free source buffer. */
    H264_STATUS_ERROR_CLOSED              = -3,   /*!< h264 module is already closed. */
    H264_STATUS_ERROR_OPENED              = -2,   /*!< h264 module open error. */
    H264_STATUS_ERROR                     = -1,   /*!< Error. */
    H264_STATUS_OK                        = 0,    /*!< OK. */
    ENUM_DUMMY4WORD(H264Status),
}H264Status;

typedef struct _H264YuvAddr{
    UINT32 uiYAddr;
    UINT32 uiUVAddr;
}H264YuvAddr;

typedef struct _H264_3dnrUnit{
    UINT32 uiCfgI;
    UINT32 uiCfgII;
    UINT32 uiCfgIII;
    UINT32 uiCfgIV;
    UINT32 uiCfgV;
}H264_3dnrUnit;

typedef struct _H2643dnr{
    UINT32 uiSeqCfg;

    H264_3dnrUnit e3dnrP2P;
    H264_3dnrUnit e3dnrMCTF;
}H2643dnr;

typedef struct _H264RecAndRefYuvAddr{
    H264YuvAddr eRecAddr[4];
    H264YuvAddr eRefAddr[4];
}H264RecAndRefYuvAddr;

typedef struct _H264PicUnit{
    UINT32 uiPicSize;
    UINT32 uiSrcPicYHeight;
    UINT32 uiPicCfg;
    UINT32 uiSliceCfgI;
    UINT32 uiSliceCfgII;
    UINT32 uiSliceCfgIII;
    UINT32 uiSliceCfgIV;
    UINT32 uiSliceCfgV;
    UINT32 uiSliceCfgVI;
    UINT32 uiRecAndRefLineOffset;
    UINT32 uiSrcLineOffset[4];
    UINT32 uiPhyBsdmaCmdBufAddr[4];
    UINT32 uiVirBsdmaCmdBufAddr[4];
    UINT32 uiHwBsAddr[4];
    H264YuvAddr eSrcYuvAddr[4];
    UINT32 uiDecMaxLen[4];
}H264PicUnit;

typedef struct _H264RegSet{
    UINT32 uiSeqCfg;

    H2643dnr e3dnr[2];

    UINT32 uiSrcScaleDownRate;
    H264YuvAddr eOutputSrcYuvAddr[4];

    H264PicUnit ePicUnit[2];

    H264RecAndRefYuvAddr eRecAndRefYuvAddr[2];
    UINT32 uiColInfoAddr;
    UINT32 uiUriAddr;
    UINT32 uiRecUrAddr;
    UINT32 uiSliceLenAddr[2][4];
    UINT32 uiMaxBSLen;        // Maximum bit-stream length
}H264RegSet;


typedef enum
{
    H264_CONFIG_ID_FREQ,                    ///< H264 module clock (Unit: MHz), will be active on succeeding h264_open().
                                            ///< Context can be one of the following:
                                            ///< - @b 120   : 120 MHz
                                            ///< - @b 160   : 160 MHz
                                            ///< - @b 192   : 192 MHz (Default value)
} H264_CONFIG_ID;

extern ER           h264_setConfig(H264_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32       h264_getConfig(H264_CONFIG_ID ConfigID);
extern H264Status   h264_open(void);
extern H264Status   h264_open2(void);
extern H264Status   h264_close(void);
extern void         h264_resetINT(void);
extern UINT32       h264_waitINT(void);
extern UINT32       h264_checkINT(void);
extern void         h264_resetIntStatus(void);
extern H264Status   h264_close2(void);
// Encode //
extern void         h264_setEncInitReg(H264RegSet *pH264RegSet);
extern void         h264_setEncPicReg(H264RegSet *pH264RegSet,UINT32 uiEncNum);
extern UINT32       h264_exit_isr(void);
// Decode //
extern void         h264_setDecInitReg(H264RegSet *pH264RegSet);
extern void         h264_setDecPicReg(H264RegSet *pH264RegSet,UINT32 uiDecNum);

// Sequence 0 //
extern UINT32       h264_getSeq0PicBsLen0Reg(void);
extern UINT32       h264_getSeq0PicBsLen1Reg(void);
extern UINT32       h264_getSeq0PicBsLen2Reg(void);
extern UINT32       h264_getSeq0PicBsLen3Reg(void);
extern UINT32       h264_getSeq0RcSumQStep0(void);
extern UINT32       h264_getSeq0RcSumQStep1(void);
extern UINT32       h264_getSeq0RcSumQStep2(void);
extern UINT32       h264_getSeq0RcSumQStep3(void);
extern UINT32       h264_getSeq0PicCost0(void);
extern UINT32       h264_getSeq0PicCost1(void);
extern UINT32       h264_getSeq0PicCost2(void);
extern UINT32       h264_getSeq0PicCost3(void);

// Sequence 1 //
extern UINT32       h264_getSeq1PicBsLen0Reg(void);
extern UINT32       h264_getSeq1PicBsLen1Reg(void);

// Set Memory Buffer Addr //
extern void         h264_setMemAddrReg(H264RegSet *pH264RegSet);
extern void         h264_setSeq0BsdmaCmdBufAddr0En(void);
extern void         h264_setSeq0BsdmaCmdBufAddr1En(void);

// Debug //
extern void         h264_setPicOutputColInfoEnable(void);
extern void         h264_setPicOutputRecEnable(void);
extern void         h264_setPicOutputColInfoDisable(void);
extern void         h264_setPicOutputRecDisable(void);
extern void         h264_setDebugSel(UINT32 uiSelect);
extern UINT32       h264_getSeq0CheckSum0(void);
extern UINT32       h264_getSeq0CheckSum1(void);
extern UINT32       h264_getSeq0CheckSum2(void);
extern UINT32       h264_getSeq0CheckSum3(void);
extern UINT32       h264_getSeq0BsCheckSum0(void);
extern UINT32       h264_getSeq0BsCheckSum1(void);

extern void         h264_resetHW(void);
#endif
