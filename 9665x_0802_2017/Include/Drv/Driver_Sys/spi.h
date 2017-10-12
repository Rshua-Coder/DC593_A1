/**
    Header file of SPI module driver

    This file is the header file of SPI module

    @file       SPI.h
    @ingroup    mIIOSPI
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _SPI_H
#define _SPI_H

#include "Type.h"

/**
    @addtogroup mIIOSPI
*/
//@{

/**
    SPI max transfer count definition. (unit: word)

    @note For spi_writeReadData()
*/
//@{
//#define SPI_MAX_TRANSFER_CNT    (0xFFFFFF)  ///< max transfer count for spi_writeReadData()
//@}

#if 0
/**
    @name SPI gyro mode call back prototype

    @note For pEventHandler of SPI_GYRO_INFO

    @param[in] gyroSts      gyro status. Can be:
                            - @b SPI_GYRO_INT_SYNC_END
                            - @b SPI_GYRO_INT_OVERRUN
                            - @b SPI_GYRO_INT_SEQ_ERR
    @param[in] uiOpIntSts   gyro OP complete status. Can be ORed of SPI_GYRO_OP_INT

    @return void
*/
//@{
typedef void (*SPI_GYRO_CB)(SPI_GYRO_INT gyroSts);
//typedef void (*SPI_GYRO_CB)(UINT32 uiGyroSts, UINT32 uiOpIntSts);
//@}
#endif

/**
    SPI ID
*/
typedef enum
{
    SPI_ID_1,                   ///< SPI 1
    SPI_ID_2,                   ///< SPI 2
    SPI_ID_3,                   ///< SPI 3

    SPI_ID_COUNT,               ///< SPI ID count

    ENUM_DUMMY4WORD(SPI_ID)
} SPI_ID;

/**
    SPI IO CONTROL ID
*/
typedef enum
{
    ENUM_DUMMY4WORD(SPI_IOCTRL)
} SPI_IOCTRL;

/**
    SPI Configuration Identifier

    @note For spi_setConfig()
*/
typedef enum
{
    SPI_CONFIG_ID_BUSMODE,          ///< SPI Bus Mode. Context can be:
                                    ///< - @b SPI_MODE_0 (default)
                                    ///< - @b SPI_MODE_1
                                    ///< - @b SPI_MODE_2
                                    ///< - @b SPI_MODE_3
    SPI_CONFIG_ID_FREQ,             ///< SPI Bus Frequency. Unit: Hz (default: 24000000)
    SPI_CONFIG_ID_MSB_LSB,          ///< SPI MSB/LSB select. Context can be:
                                    ///< - @b SPI_MSB: MSB (bit) first
                                    ///< - @b SPI_LSB: LSB (bit) first (default)
    SPI_CONFIG_ID_WIDE_BUS_ORDER,   ///< Bit order in wide bus mode. Context can be:
                                    ///< - @b SPI_WIDE_BUS_ORDER_NORMAL: normal order (default)
                                    ///< - @b SPI_WIDE_BUS_ORDER_INVERT: inverted order
    SPI_CONFIG_ID_CS_ACT_LEVEL,     ///< CS active level select. Context can be:
                                    ///< - @b SPI_CS_ACT_LEVEL_LOW: low active (default)
                                    ///< - @b SPI_CS_ACT_LEVEL_HIGH: high active
    SPI_CONFIG_ID_CS_CK_DLY,        ///< CS edge to Clock edge delay. unit: clock
    SPI_CONFIG_ID_DO_HZ_EN,         ///< SPI DO output Hi-Z control
                                    ///< - @b TRUE: SPI DO be hi-Z when no data is needed to output
                                    ///< - @b FALSE: SPI DO will be driven by SPI controller event no data is needed to output (default)
    SPI_CONFIG_ID_AUTOPINMUX,       ///< SPI auto pinmux control
                                    ///< - @b TRUE: SPI will enable pinmux (according to pinmux_init()) when spi_open(), disable pinmux when spi_close()
                                    ///< - @b FALSE: SPI driver will not alter pinmux(default)


    ////////// Engineering Usage ///////////////////
    SPI_CONFIG_ID_ENG_PKT_COUNT,    //< engineer mode: packet count for spi_writeSingle(), spi_readSingle(), spi_writeReadSingle().
                                    //< Context can be:
                                    //< - @b 1: 1 packet
                                    //< - @b 2: 2 packet
                                    //< - @b 4: 4 packet
    SPI_CONFIG_ID_ENG_MSB_LSB,      //< engineer mode: MSB/LSB. Context can be:
                                    //< - @b SPI_MSB: MSB (bit) first
                                    //< - @b SPI_LSB: LSB (bit) first
    SPI_CONFIG_ID_ENG_DMA_ABORT,    //< engineer mode: DMA abort. Context can be:
                                    //< - @b FALSE: no DMA abort after DMA is triggered
                                    //< - @b TRUE: test DMA abort after DMA is triggered
    SPI_CONFIG_ID_ENG_GYRO_UNIT,    //< engineer mode: gyro delay unit. Context can be:
                                    //< - @b FALSE: unit is us (default value)
                                    //< - @b TRUE: unit is SPI CLK

    ENUM_DUMMY4WORD(SPI_CONFIG_ID)
} SPI_CONFIG_ID;

/**
    SPI mode

    @note For spi_setConfig(SPI_ID, SPI_CONFIG_ID_BUSMODE, )
*/
typedef enum
{
    SPI_MODE_0,                 ///< MODE 0
    SPI_MODE_1,                 ///< MODE 1
    SPI_MODE_2,                 ///< MODE 2
    SPI_MODE_3,                 ///< MODE 3

    ENUM_DUMMY4WORD(SPI_MODE)
} SPI_MODE;

/**
    SPI LSB/MSB select

    @note For spi_setConfig(SPI_ID, SPI_CONFIG_ID_MSB_LSB, )
*/
typedef enum
{
    SPI_MSB,                    ///< MSb first
    SPI_LSB,                    ///< LSb first

    ENUM_DUMMY4WORD(SPI_LSB_MSB)
} SPI_LSB_MSB;

/**
    SPI CS active level

    @note For spi_setConfig(SPI_ID, SPI_CONFIG_ID_CS_ACT_LEVEL, )
*/
typedef enum
{
    SPI_CS_ACT_LEVEL_LOW,       ///< CS is low active
    SPI_CS_ACT_LEVEL_HIGH,      ///< CS is high active

    ENUM_DUMMY4WORD(SPI_CS_ACT_LEVEL)
} SPI_CS_ACT_LEVEL;

/*
    SPI bus width

    @note For spi_setBusWidth()
*/
typedef enum
{
    SPI_BUS_WIDTH_1_BIT,        //< Bus width is 1 bit
    SPI_BUS_WIDTH_2_BITS,       //< Bus width is 2 bits
    SPI_BUS_WIDTH_4_BITS,       //< Bus width is 4 bits

    ENUM_DUMMY4WORD(SPI_BUS_WIDTH)
} SPI_BUS_WIDTH;

/**
    SPI wide bus order

    @note For spi_setConfig(SPI_ID, SPI_CONFIG_ID_WIDE_BUS_ORDER, )
*/
typedef enum
{
    SPI_WIDE_BUS_ORDER_NORMAL,  ///< Normal order. Transmit sequence is SPI_IO3, SPI_IO2, SPI_IO1, SPI_IO0.
    SPI_WIDE_BUS_ORDER_INVERT,  ///< Invert order. Transmit sequence is SPI_IO0, SPI_IO1, SPI_IO2, SPI_IO3.

    ENUM_DUMMY4WORD(SPI_WIDE_BUS_ORDER)
} SPI_WIDE_BUS_ORDER;

/**
    SPI transfer length

    @note For spi_setTransferLen()
*/
typedef enum
{
    SPI_TRANSFER_LEN_1BYTE,     ///< 1 byte
    SPI_TRANSFER_LEN_2BYTES,    ///< 2 bytes

    ENUM_DUMMY4WORD(SPI_TRANSFER_LEN)
} SPI_TRANSFER_LEN;

/**
    SPI gyro mode

    @note For gyroMode of SPI_GYRO_INFO
*/
typedef enum
{
    SPI_GYRO_MODE_SIE_SYNC,             ///< SIE sync mode
    SPI_GYRO_MODE_ONE_SHOT,             ///< one shot mode
    SPI_GYRO_MODE_FREE_RUN,             ///< free run mode

    ENUM_DUMMY4WORD(SPI_GYRO_MODE)
} SPI_GYRO_MODE;

/**
    SPI GYRO interrupt

    @note For uiGyroSts of SPI_GYRO_CB
*/
typedef enum
{
    SPI_GYRO_INT_SYNC_END           = 0x00000001,   ///< End of one SIE sync. All data in this sync are completed.
    SPI_GYRO_INT_OVERRUN            = 0x00000002,   ///< HW FIFO overrun. FW should do error handling, ex: re-start gyro polling
    SPI_GYRO_INT_SEQ_ERR            = 0x00000004,   ///< Sequence error. Next SIE sync is too early.

    SPI_GYRO_INT_QUEUE_THRESHOLD    = 0x00000008,   ///< SW Queue exceed threshold. Upper layer should sink Gyro data ASAP.
    SPI_GYRO_INT_QUEUE_OVERRUN      = 0x00000010,   ///< Sw Queue overrun. FW should do error handling.

    ENUM_DUMMY4WORD(SPI_GYRO_INT)
} SPI_GYRO_INT;

/**
    SPI gyro OP interrupt

    @note For uiOpIntEnMsk of SPI_GYRO_INFO, uiOpIntSts of SPI_GYRO_CB
*/
typedef enum
{
    SPI_GYRO_OP_INT_T0_OP0 = 0x01,      ///< interrupt on Transfer[0]/Op[0]
    SPI_GYRO_OP_INT_T0_OP1 = 0x02,      ///< interrupt on Transfer[0]/Op[1]
    SPI_GYRO_OP_INT_T1_OP0 = 0x04,      ///< interrupt on Transfer[1]/Op[0]
    SPI_GYRO_OP_INT_T1_OP1 = 0x08,      ///< interrupt on Transfer[1]/Op[1]
    SPI_GYRO_OP_INT_T2_OP0 = 0x10,      ///< interrupt on Transfer[2]/Op[0]
    SPI_GYRO_OP_INT_T2_OP1 = 0x20,      ///< interrupt on Transfer[2]/Op[1]
    SPI_GYRO_OP_INT_T3_OP0 = 0x40,      ///< interrupt on Transfer[3]/Op[0]
    SPI_GYRO_OP_INT_T3_OP1 = 0x80,      ///< interrupt on Transfer[3]/Op[1]

    SPI_GYRO_OP_INT_T4_OP0 = 0x0100,    ///< interrupt on Transfer[4]/Op[0]
    SPI_GYRO_OP_INT_T4_OP1 = 0x0200,    ///< interrupt on Transfer[4]/Op[1]
    SPI_GYRO_OP_INT_T5_OP0 = 0x0400,    ///< interrupt on Transfer[5]/Op[0]
    SPI_GYRO_OP_INT_T5_OP1 = 0x0800,    ///< interrupt on Transfer[5]/Op[1]
    SPI_GYRO_OP_INT_T6_OP0 = 0x1000,    ///< interrupt on Transfer[6]/Op[0]
    SPI_GYRO_OP_INT_T6_OP1 = 0x2000,    ///< interrupt on Transfer[6]/Op[1]
    SPI_GYRO_OP_INT_T7_OP0 = 0x4000,    ///< interrupt on Transfer[7]/Op[0]
    SPI_GYRO_OP_INT_T7_OP1 = 0x8000,    ///< interrupt on Transfer[7]/Op[1]

    SPI_GYRO_OP_INT_T8_OP0 = 0x010000,  ///< interrupt on Transfer[8]/Op[0]
    SPI_GYRO_OP_INT_T8_OP1 = 0x020000,  ///< interrupt on Transfer[8]/Op[1]
    SPI_GYRO_OP_INT_T9_OP0 = 0x040000,  ///< interrupt on Transfer[9]/Op[0]
    SPI_GYRO_OP_INT_T9_OP1 = 0x080000,  ///< interrupt on Transfer[9]/Op[1]
    SPI_GYRO_OP_INT_T10_OP0 = 0x100000, ///< interrupt on Transfer[10]/Op[0]
    SPI_GYRO_OP_INT_T10_OP1 = 0x200000, ///< interrupt on Transfer[10]/Op[1]
    SPI_GYRO_OP_INT_T11_OP0 = 0x400000, ///< interrupt on Transfer[11]/Op[0]
    SPI_GYRO_OP_INT_T11_OP1 = 0x800000, ///< interrupt on Transfer[11]/Op[1]

    SPI_GYRO_OP_INT_T12_OP0 = 0x01000000,   ///< interrupt on Transfer[12]/Op[0]
    SPI_GYRO_OP_INT_T12_OP1 = 0x02000000,   ///< interrupt on Transfer[12]/Op[1]
    SPI_GYRO_OP_INT_T13_OP0 = 0x04000000,   ///< interrupt on Transfer[13]/Op[0]
    SPI_GYRO_OP_INT_T13_OP1 = 0x08000000,   ///< interrupt on Transfer[13]/Op[1]
    SPI_GYRO_OP_INT_T14_OP0 = 0x10000000,   ///< interrupt on Transfer[14]/Op[0]
    SPI_GYRO_OP_INT_T14_OP1 = 0x20000000,   ///< interrupt on Transfer[14]/Op[1]
    SPI_GYRO_OP_INT_T15_OP0 = 0x40000000,   ///< interrupt on Transfer[15]/Op[0]
    SPI_GYRO_OP_INT_T15_OP1 = 0x80000000,   ///< interrupt on Transfer[15]/Op[1]

    ENUM_DUMMY4WORD(SPI_GYRO_OP_INT)
} SPI_GYRO_OP_INT;

/*
    SPI initialization infomation

    (OBSOLETE structure)

    @note For spi_init()
*/
typedef struct
{
    BOOL        bCSActiveLow;   ///< CS polarity
                                ///< - TRUE: CS is active LOW
                                ///< - FALSE: CS is active HIGH
    BOOL        bMasterMode;    ///< Master mode
                                ///< - TRUE: master mode
    UINT32      uiFreq;         ///< Clock frequency (unit: Hz)
    SPI_MODE    spiMODE;        ///< SPI MODE select
                                ///< - SPI_MODE_0: mode 0
                                ///< - SPI_MODE_1: mode 1
                                ///< - SPI_MODE_2: mode 2
                                ///< - SPI_MODE_3: mode 3
    BOOL        bLSB;           ///< LSB mode select (bit order)
                                ///< - TRUE: LSB mode
                                ///< - FALSE: MSB mode
    SPI_WIDE_BUS_ORDER wideBusOrder;    ///< Bus order is wide bus mode (SPI_BUS_WIDTH_2_BITS or SPI_BUS_WIDTH_4_BITS)
                                        ///< - SPI_WIDE_BUS_ORDER_NORMAL: normal order
                                        ///< - SPI_WIDE_BUS_ORDER_INVERT: inverted order
//    SPI_PINMUX  pinmuxChannel;  ///< SPI pinmux channel
//                                ///< - SPI_PINMUX_STORAGE
//                                ///< - SPI_PINMUX_SERIAL
    UINT32      uiDelay;        ///< - Delay time between CS edge and first clock edge
    UINT32      uiHoldTime;     ///< - SPI delay time: 0~ 15
    UINT32      uiDuration;     ///< - SPI delay time: 0~ 15
} SPI_INIT_INFO, *PSPI_INIT_INFO;

/**
    GYRO Buffer Queue

    (Maybe OBSOLETED)
*/
typedef struct// __GYRO_BUF_QUEUE__
{
    UINT32  uiFrameID;          ///< record frame ID of this buffer
    UINT32  uiDataID;           ///< record Gyro data ID. Valid value: 0~31
    UINT32  vRecvWord[2];       ///< stores data received in one Gyro data transfer
//    struct __GYRO_BUF_QUEUE__* pNext;   ///< Next queue element
} GYRO_BUF_QUEUE, *PGYRO_BUF_QUEUE;

/**
    @name SPI gyro mode call back prototype

    @note For pEventHandler of SPI_GYRO_INFO

    @param[in] gyroSts      gyro status. Can be:
                            - @b SPI_GYRO_INT_SYNC_END
                            - @b SPI_GYRO_INT_OVERRUN
                            - @b SPI_GYRO_INT_SEQ_ERR
    @param[in] uiOpIntSts   gyro OP complete status. Can be ORed of SPI_GYRO_OP_INT

    @return void
*/
//@{
typedef void (*SPI_GYRO_CB)(SPI_GYRO_INT gyroSts);
//typedef void (*SPI_GYRO_CB)(UINT32 uiGyroSts, UINT32 uiOpIntSts);
//@}

/**
    SPI GYRO control infomation

    @note For spi_startGyro()
*/
typedef struct
{
    SPI_GYRO_MODE   gyroMode;   ///< Gyro trigger mode

    UINT32 uiOpInterval;        ///< Delay between 2 OP (unit: us)
    UINT32 uiTransferInterval;  ///< Delay between 2 transfer (unit: us)
    UINT32 uiTransferCount;     ///< total transfer in single run. valid value: 1~16

    UINT32 uiOp0Length;         ///< length of OP0. unit: byte. valid value: 1~8
    UINT32 uiOp1Length;         ///< length of OP1. unit: byte. valid value: 1~8

    UINT32 uiOpIntEnMsk;        //<  (OBSOLETE) Interrupt enable mask of OP. Should be ORed of SPI_GYRO_OP_INT

    UINT8 vOp0OutData[8];       ///< stores data to be output in OP0
    UINT8 vOp1OutData[8];       ///< stores data to be output in OP1

    void  (*pEventHandler)(SPI_GYRO_INT gyroSts);   ///< EventHandler function pointer, set to NULL if you don't want to handle audio event
} SPI_GYRO_INFO, *PSPI_GYRO_INFO;


extern ER   spi_open(SPI_ID spiID);
extern ER   spi_close(SPI_ID spiID);
extern BOOL spi_isOpened(SPI_ID spiID);
extern ER   spi_init(SPI_ID spiID, PSPI_INIT_INFO pSpiInitInfo);

extern ER   spi_setTransferLen(SPI_ID spiID, SPI_TRANSFER_LEN length);
extern void spi_setCSActive(SPI_ID spiID, BOOL bCSActive);
extern ER   spi_writeReadData(SPI_ID spiID, UINT32 uiWordCount, UINT32* pTxBuf, UINT32* pRxBuf, BOOL bDmaMode);
extern ER   spi_waitDataDone(SPI_ID spiID);
extern ER   spi_writeSingle(SPI_ID spiID, UINT32 uiTxWord);
extern ER   spi_readSingle(SPI_ID spiID, UINT32* pRxWord);
extern ER   spi_writeReadSingle(SPI_ID spiID, UINT32 uiTxWord, UINT32* pRxWord);

extern ER   spi_startGyro(SPI_ID spiID, SPI_GYRO_INFO* pGyroInfo);
extern ER   spi_stopGyro(SPI_ID spiID);
//extern ER   spi_getGyroFifoCnt(SPI_ID spiID, UINT32* puiCnt);
//extern ER   spi_readGyroFifo(SPI_ID spiID, UINT32 uiCnt, UINT32* pBuf);
//extern ER   spi_readGyroCounter(SPI_ID spiID, UINT32* puiTransfer, UINT32* puiOp);
extern UINT32 spi_getGyroQueueCount(SPI_ID spiID);
extern ER   spi_getGyroData(SPI_ID spiID, PGYRO_BUF_QUEUE pGyroData);

extern ER   spi_ioctrl(SPI_ID spiID, SPI_IOCTRL ctrlID, UINT32 context);
extern ER   spi_setConfig(SPI_ID spiID, SPI_CONFIG_ID configID, UINT32 configContext);
extern ER   spi_enBitMatch(SPI_ID spiID, UINT32 uiCmd, UINT32 uiBitPosition, BOOL bWaitValue);
extern ER   spi_waitBitMatch(SPI_ID spiID, UINT32 uiTimeoutMs);
extern ER   spi_setBusWidth(SPI_ID spiID, SPI_BUS_WIDTH busWidth);
//@}

#endif
