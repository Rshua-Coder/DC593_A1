/**
    Copyright   Novatek Microelectronics Corp. 2007. All rights reserved.

    @file       NEC_IRRM.c
    @ingroup    mISYSIrRx

    @brief      NEC IR remote control protocol driver. Use hardware IR remote controller version.
                This file is the driver for NEC IR remote control protocol.

    @note       Nothing.

    @version    V1.00.000
    @author     Stephen Wang
    @date       2007/12/26
*/

/** \addtogroup mISYSIrRx */
//@{

#include <string.h>
#include "Debug.h"
#include "IrRx.h"
#include "Remote.h"
#include "ErrorNo.h"

// ----------------------------------------------------------------------------
// Global Variables
// ----------------------------------------------------------------------------
static BOOL bIrRxOpened = FALSE;

static IRRX_DEVICE_OBJ IrRxDeviceObj;
IRRX_DATA_INFO g_IrRxDataInfo;

static IRRX_DATA_QUEUE IrRxPutDataQ = {0, 0, 0};

static BOOL bIsDataPicked = TRUE;

//IRRM_DEVOBJ IrrmDevObj;

// ----------------------------------------------------------------------------
// Function Declarations
// ----------------------------------------------------------------------------

/**
  Callback of IR Remote receiver ISR.

  Note that the ISR flow design is based on NEC IR remote control protocol.

  Description:

  Step A: Whenever entering the ISR, we recognize it a burst (protocol view) or not.
          Mark the time stamp, and then we can get the width of the previous
          low/high pulse by calculating the time difference.

  >> Rising edge (protocol view) handling:
  Step B:
          B-1: Is a Leader Code?
               B-1a) HIGH(9ms) + LOW(4.5ms)  -> 1st Leader Code
               B-1b) HIGH(9ms) + LOW(2.25ms) -> Repeat Code

          B-2: If already got a Leader Code, and uiBitNo < 32, then check it's a data bit or not.
               B-2a) HIGH(0.56ms) + LOW(0.56ms) -> DATA '0'
               B-2b) HIGH(0.56ms) + LOW(1.69ms) -> DATA '1'

  >> Falling edge (protocol view) handling:
  Step C:
          C-1: Is a Stop Bit?
               C-1a) 1st Leader Code + 32bits data + HIGH(0.56ms)
               C-1b) Repeat Code + HIGH(0.56ms)

  Step D: Change the next IR receiver GPIO interrupt trigger polariy.


  @param void
  @return void
*/
static void irrx_isr(UINT32 uiCallID)
{
    REMOTE_PATTERN irdata;

    if(uiCallID & REMOTE_INT_RD)
    {
        debug_ind(("(RM)Received Code!!\r\n"));
        remote_getDataCommand(&irdata);

        g_IrRxDataInfo.bRepeatCode = FALSE;

        g_IrRxDataInfo.bStopBit = TRUE;

        g_IrRxDataInfo.uiRxDataBits    = irdata.uiLow;//IRRM_DWLow(irdata);

        g_IrRxDataInfo.uiRxDataTime = 0;    //???

        // Put the data in IR receiver data queue
        IrRxPutDataQ.uiRxData = g_IrRxDataInfo.uiRxDataBits;
        IrRxPutDataQ.uiRxTime = g_IrRxDataInfo.uiRxDataTime;

        irrx_enqueueData((PIRRX_DATA_QUEUE)&IrRxPutDataQ);

        bIsDataPicked = FALSE;

        return;
    }

    if (uiCallID & REMOTE_INT_REPEAT)
    {
        debug_ind(("(RM)Repeat Code!!\r\n"));
        g_IrRxDataInfo.bRepeatCode = TRUE;
        // enqueue previous IrRxData
        irrx_enqueueData((PIRRX_DATA_QUEUE)&IrRxPutDataQ);
        return;
    }

    if (uiCallID & REMOTE_INT_ERR)
    {
        debug_msg("Data Err!!\r\n");
        return ;
    }
}

/**
  Open IR receiver and start a reference timer.

  Open IR receiver and start a reference timer.

  @param pIrRxDevObj: IrRx device object pointer
  @return E_OK if success, E_SYS if already opened or timer is not available
*/
ER irrx_open(PIRRX_DEVICE_OBJ pIrRxDevObj)
{
    // Already opened?
    if(bIrRxOpened)
    {
        debug_err(("irrx_open: IrRx is already opened!\r\n"));
        return E_SYS;
    }

    bIrRxOpened = TRUE;

    // Save IrRx device object setting
    memcpy((void *)&IrRxDeviceObj, (void *)pIrRxDevObj, sizeof(IRRX_DEVICE_OBJ));

    remote_open(irrx_isr);  // Open 時順便掛 CB Function, Event 用 CB 通知,
    remote_setEnable(FALSE);

    remote_setConfig(REMOTE_CONFIG_ID_DATA_LENGTH,       32);
    remote_setConfig(REMOTE_CONFIG_ID_INPUT_INVERSE,     TRUE);
    remote_setConfig(REMOTE_CONFIG_ID_THRESHOLD_SEL,    REMOTE_SPACE_DET);
    remote_setConfig(REMOTE_CONFIG_ID_DATA_ORDER,         REMOTE_DATA_LSB);    //  LSB / MSB 可以自行選擇

    remote_setConfig(REMOTE_CONFIG_ID_HEADER_DETECT,    REMOTE_FORCE_DETECT_HEADER);
    remote_setConfig(REMOTE_CONFIG_ID_LOGIC_TH,               1100);
    remote_setConfig(REMOTE_CONFIG_ID_ERROR_TH,              20000);
    remote_setConfig(REMOTE_CONFIG_ID_HEADER_TH,            6000);
    remote_setConfig(REMOTE_CONFIG_ID_GSR_TH,                  62);

    remote_setConfig(REMOTE_CONFIG_ID_REPEAT_DET_EN,    ENABLE);
    remote_setConfig(REMOTE_CONFIG_ID_REPEAT_TH,           3000);

    remote_setInterruptEnable(REMOTE_INT_RD | REMOTE_INT_ERR | REMOTE_INT_REPEAT);

    // Init IR receiver data queue
    irrx_initDataQueue();

    // Start IR remote control receive
    remote_setEnable(TRUE);

    return E_OK;
}

/**
  Close IR receiver and release the reference timer.

  Close IR receiver and release the reference timer.

  @param void
  @return E_OK if success
*/
ER irrx_close(void)
{
    if(!bIrRxOpened)
        return E_OK;

    // Stop
    remote_setEnable(FALSE);

    remote_close();

    bIrRxOpened = FALSE;
    return E_OK;
}

/**
  IR data expiration check, to verify IR signal is stop sending or not.

  For outside task periodic polling.

  @param void
  @return void
*/
void irrx_checkDataExp(void)
{
    //now it is hardware controlled, no need this polling loop
}

/**
  Get IrRx received data bits.

  For outside task picking up the received data contents. (If w/o IrRx data queue)

  @param void
  @return IrRx received data bits
*/
UINT32 irrx_getDataContent(void)
{
    bIsDataPicked = TRUE;
    return g_IrRxDataInfo.uiRxDataBits;
}

/**
  Check whether one data frame is fully received ok or not.

  For outside task checking if there is one data frame ready to pick. (If w/o IrRx data queue)

  @param void
  @return TRUE: Data is ready
*/
BOOL irrx_isDataReady(void)
{
    BOOL bResult = FALSE;

    if(g_IrRxDataInfo.bStopBit && !bIsDataPicked)
        bResult = TRUE;

    return bResult;
}

/**
  Check whether a Repeat Code is received or not.

  For outside task checking if there is one Repeat Code need to report. (If w/o IrRx data queue)

  @param void
  @return TRUE: Repeat Code is still sending
*/
BOOL irrx_isDataRepeat(void)
{
    return g_IrRxDataInfo.bRepeatCode;
}




//@}
