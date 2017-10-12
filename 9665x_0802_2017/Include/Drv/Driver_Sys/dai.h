/**
    @file       DAI.h
    @ingroup    mIAudDAI

    @brief      Header file for DAI module
                This file is the header file that define the API and data type
                for DAI module

    @note       Nothing.


    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

*/

/** \addtogroup mIAudDAI */
//@{

#ifndef _DAI_H
#define _DAI_H

#include "Type.h"
#include "Driver.h"

/**
    DAI Interrupt Enable

    @note For dai_setIntCtrl(), dai_getIntCtrl()
*/
typedef enum
{
    DAI_DISABLE_ALL_INT = 0x00,         ///< Disable all interrupt
    DAI_DMADONE_INT_EN = 0X01,          ///< DMA buffer done
    DAI_TCHIT_INT_EN = 0x02,            ///< Time code hit
    DAI_TCLATCH_INT_EN = 0x04,          ///< Time code latch
    DAI_DMA_STADR_LOAD_INT_EN = 0x08,   ///< DMA start address loaded
    DAI_FIFO_ERROR_INT_EN = 0x10,       ///< FIFO error
    DAI_STOP_STS_INT_EN = 0x20,         ///< DAI stopped

    ENUM_DUMMY4WORD(DAI_INTEN)
} DAI_INTEN;

/**
    DAI Interrupt Status

    @note For dai_clrIntStatus(), dai_getIntStatus()
*/
typedef enum
{
    DAI_DMADONE_INT_STS = 0X01,         ///< DMA buffer done
    DAI_TCHIT_INT_STS = 0x02,           ///< Time code hit
    DAI_TCLATCH_INT_STS = 0x04,         ///< Time code latch
    DAI_DMA_STADR_LOAD_INT_STS = 0x08,  ///< DMA start address loaded
    DAI_FIFO_ERROR_INT_STS = 0x10,      ///< FIFO error
    DAI_STOP_STS_INT_STS = 0x20,        ///< DAI stopped
    DAI_CLEAR_ALL_INT_STS = 0x3F,       ///< Clear all interrupt

    ENUM_DUMMY4WORD(DAI_INTSTS)
} DAI_INTSTS;


/**
    DAI format

    (OBSOLETE)

    @note Reserved for compatability
*/
typedef enum
{
    I2S,                ///< I2S
    AC97,               ///< AC97
    GPIO,

    ENUM_DUMMY4WORD(FRAME_SEL)
} FRAME_SEL;

/**
    DAI I2S clock ratio

    @note For dai_setI2SClkRatio()
*/
typedef enum
{
    DAI_I2SCLKR_256FS_32BIT,  ///< SystemClk = 256 * FrameSync, FrameSync = 32 BitClk
    DAI_I2SCLKR_256FS_64BIT,  ///< SystemClk = 256 * FrameSync, FrameSync = 64 BitClk
    DAI_I2SCLKR_384FS_32BIT,  ///< SystemClk = 384 * FrameSync, FrameSync = 32 BitClk
    DAI_I2SCLKR_384FS_48BIT,  ///< SystemClk = 384 * FrameSync, FrameSync = 48 BitClk
    DAI_I2SCLKR_384FS_96BIT,  ///< SystemClk = 384 * FrameSync, FrameSync = 96 BitClk
    DAI_I2SCLKR_512FS_32BIT,  ///< SystemClk = 512 * FrameSync, FrameSync = 32 BitClk
    DAI_I2SCLKR_512FS_64BIT,  ///< SystemClk = 512 * FrameSync, FrameSync = 64 BitClk
    DAI_I2SCLKR_512FS_128BIT, ///< SystemClk = 512 * FrameSync, FrameSync = 128 BitClk
    DAI_I2SCLKR_768FS_32BIT,  ///< SystemClk = 768 * FrameSync, FrameSync = 32 BitClk
    DAI_I2SCLKR_768FS_48BIT,  ///< SystemClk = 768 * FrameSync, FrameSync = 48 BitClk
    DAI_I2SCLKR_768FS_64BIT,  ///< SystemClk = 768 * FrameSync, FrameSync = 64 BitClk
    DAI_I2SCLKR_768FS_192BIT, ///< SystemClk = 768 * FrameSync, FrameSync = 192 BitClk
    DAI_I2SCLKR_1024FS_32BIT, ///< SystemClk = 1024 * FrameSync, FrameSync = 32 BitClk
    DAI_I2SCLKR_1024FS_64BIT, ///< SystemClk = 1024 * FrameSync, FrameSync = 64 BitClk
    DAI_I2SCLKR_1024FS_128BIT,///< SystemClk = 1024 * FrameSync, FrameSync = 128 BitClk
    DAI_I2SCLKR_1024FS_256BIT,///< SystemClk = 1024 * FrameSync, FrameSync = 256 BitClk

    ENUM_DUMMY4WORD(DAI_I2SCLKR)
} DAI_I2SCLKR;

/**
    DAI I2S Frame clock ratio

    @note For dai_setI2SFrameClkRatio()
*/
typedef enum
{
    DAI_I2SFRAMECLKR_32BIT,     ///< FrameSync = 32 BitClk
    DAI_I2SFRAMECLKR_48BIT,     ///< FrameSync = 48 BitClk
    DAI_I2SFRAMECLKR_64BIT,     ///< FrameSync = 64 BitClk
    DAI_I2SFRAMECLKR_96BIT,     ///< FrameSync = 96 BitClk
    DAI_I2SFRAMECLKR_128BIT,    ///< FrameSync = 128 BitClk
    DAI_I2SFRAMECLKR_192BIT,    ///< FrameSync = 192 BitClk
    DAI_I2SFRAMECLKR_256BIT,    ///< FrameSync = 256 BitClk

    ENUM_DUMMY4WORD(DAI_I2SFRAMECLKR)
} DAI_I2SFRAMECLKR;

/**
    DAI I2S format

    @note For dai_setI2SFmt()
*/
typedef enum
{
    DAI_I2SFMT_STANDARD,      ///< I2S Standard
    DAI_I2SFMT_DSP,           ///< I2S DSP mode
    DAI_I2SFMT_LIKE_MSB,      ///< I2S Like, MSB justified
    DAI_I2SFMT_LIKE_LSB,      ///< I2S Like, LSB justified

    ENUM_DUMMY4WORD(DAI_I2SFMT)
} DAI_I2SFMT;

/**
    DAI channel

    @note For dai_setCh()
*/
typedef enum
{
    DAI_CH_STEREO = 1,        ///< Stereo
    DAI_CH_MONO_LEFT,         ///< Left
    DAI_CH_MONO_RIGHT,        ///< Right

    ENUM_DUMMY4WORD(DAI_CH)
} DAI_CH;

/**
    DAI DRAM format

    @note For dai_setDram_SoundFmt()
*/
typedef enum
{
    DAI_DRAMPCM_STEREO,       ///< Stereo
    DAI_DRAMPCM_MONO,         ///< Mono

    ENUM_DUMMY4WORD(DAI_DRAMSOUNDFMT)
} DAI_DRAMSOUNDFMT;

/**
    DAI PCM length

    @note For dai_setDRAM_PCMLen()
*/
typedef enum
{
    DAI_DRAMPCM_8,            ///< 8bits
    DAI_DRAMPCM_16,           ///< 16bits

    ENUM_DUMMY4WORD(DAI_DRAMPCMLEN)
} DAI_DRAMPCMLEN;

/**
    DAI access mode

    @note For dai_setAccessMode()
*/
typedef enum
{
    DAI_ACCESS_PIO,           ///< PIO access mode
    DAI_ACCESS_DMA,           ///< DMA access mode
    ENUM_DUMMY4WORD(DAI_ACCESSMODE)
} DAI_ACCESSMODE;

/**
    DAI I2S OP mode

    @note For dai_setI2SOpMode()
*/
typedef enum
{
    DAI_OP_SLAVE,             ///< Slave mode
    DAI_OP_MASTER,            ///< Master mode

    ENUM_DUMMY4WORD(DAI_OPMODE)
} DAI_OPMODE;

/**
    DAI system clock source

    @note For dai_setCodecClkSrc()
*/
typedef enum
{
    DAI_CODESCK_INT,          ///< internal clock
    DAI_CODESCK_EXT,          ///< external clock

    ENUM_DUMMY4WORD(DAI_CODESCK)
} DAI_CODESCK;

/**
    DAI operation mode

    @note For dai_setDAIOp()
*/
typedef enum
{
    DAI_TX,                  ///< transmit mode
    DAI_RX,                  ///< receive mode

    ENUM_DUMMY4WORD(DAI_TXRX_OP)
} DAI_TXRX_OP;

// APIs in digital DAI interface

extern void dai_open(DRV_CB pIsrHdl);
extern ER   dai_lock(void);
extern ER   dai_unlock(void);
extern void dai_close(void);
extern void dai_setupIsrHdl(DRV_CB pIsrHdl);
extern void dai_setI2SClkRatio(DAI_I2SCLKR I2SCLKRatio);
extern void dai_setI2SFmt(DAI_I2SFMT I2SFmt);
extern void dai_setI2SOpMode(DAI_OPMODE I2SOpMode);
extern void dai_setCodecClkSrc(DAI_CODESCK CodecClkSrc);
extern void dai_setCodecExt(BOOL bEnable);
extern void dai_setHdmiTxEn(BOOL bEnable);
extern void dai_enableAVSync(BOOL bAvSyncEn);
extern void dai_enableDMA(BOOL bDmaEn);
extern void dai_stopDMA(void);
extern void dai_enableDAI(BOOL bEn);
extern void dai_enableTXRX(BOOL bEn);
extern void dai_stopTXRX(void);
extern void dai_startTXRX(void);
extern BOOL dai_isDAIEnable(void);
extern BOOL dai_isDMAEnable(void);
extern BOOL dai_isTXRXEnable(void);
extern void dai_setDAIOp(DAI_TXRX_OP DAITxRxOp);
extern BOOL dai_isTxmode(void);
extern BOOL dai_isRxmode(void);
extern void dai_setCh(DAI_CH DAICh);
extern void dai_setDram_SoundFmt(DAI_DRAMSOUNDFMT SoundFmt);
extern void dai_setDRAM_PCMLen(DAI_DRAMPCMLEN PCMLen);
extern BOOL dai_isBusy(void);
extern void dai_setIntCtrl(UINT32 uiIntCtrl);
extern UINT32 dai_getIntCtrl(void);
extern void dai_clrIntStatus(UINT32 uiIntSts);
extern void dai_clrTchitStatus(void);
extern UINT32 dai_getIntStatus(void);
extern void dai_setTimecodeTrigger(UINT32 uiTrigger);
extern UINT32 dai_getTimecodeTrigger(void);
extern void dai_setTimecodeOffset(UINT32 uiOffset);
extern UINT32 dai_getTimecodeValue(void);
extern void dai_setDMAPara(UINT32 uiStadr, UINT32 uiBufSz);
extern UINT32 dai_getDMACurrAdr(void);
extern void dai_setFlg(void);
extern void dai_clrFlg(void);

//@}
#endif



