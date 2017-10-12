/*
    Copyright   Novatek Microelectronics Corp. 2007. All rights reserved.

    @file       IrRx_queue.c
    @ingroup    mISYSIrRx

    @brief      IR remote control protocol driver queue management library. 
                This file is the queue library for IR remote control protocol driver.

    @note       Nothing.

    @version    V1.00.000
    @author     JustinLee
    @date       2007/01/22
*/

/** \addtogroup mISYSIrRx */
//@{

#include <string.h>
#include "Debug.h"
#include "IrRx.h"

static IRRX_DATA_QUEUE IrRxDataQueue[IRRX_DATA_BUF_NUM];
static PIRRX_DATA_QUEUE pIrRxQFront = NULL;
static PIRRX_DATA_QUEUE pIrRxQRear = NULL;

/**
  Initialize IR receiver data queue.

  FIFO, circular linked list queue.

  QFront: Where a data to GET from.
  QRear : Where a data to PUT to.

  @param void
  @return void
*/
void irrx_initDataQueue(void)
{
    UINT32 i;

    memset((void *)&IrRxDataQueue[0], 0, sizeof(IRRX_DATA_QUEUE)*IRRX_DATA_BUF_NUM);

    for(i = 0; i < IRRX_DATA_BUF_NUM; i++)
    {
        IrRxDataQueue[i].pNext = &IrRxDataQueue[(i+1) % IRRX_DATA_BUF_NUM];
    }

    pIrRxQFront = (PIRRX_DATA_QUEUE)&IrRxDataQueue[0];
    pIrRxQRear  = (PIRRX_DATA_QUEUE)&IrRxDataQueue[0];
}

/**
  Check IR receiver data queue is full or not.

  This function checks IR receiver data queue is full or not.
  
  @param void
  @return TRUE: Data queue is full
*/
BOOL irrx_isDataQueueFull(void)
{
    return (BOOL)(pIrRxQRear->pNext == pIrRxQFront);
}

/**
  Put data to IR receiver data queue.

  Put data to the current QRear.
  
  @param pPutDataQ: Pointer of a data gotta to be added in queue 
  @return TRUE: Data added in queue, FALSE: Queue is full
*/
BOOL irrx_enqueueData(PIRRX_DATA_QUEUE pPutDataQ)
{
    if(!pPutDataQ)
        return FALSE;

    // Is queue full?
    if(pIrRxQRear->pNext == pIrRxQFront)
        return FALSE;

    // Put input data to queue
    pIrRxQRear->uiRxData = pPutDataQ->uiRxData;
    pIrRxQRear->uiRxTime = pPutDataQ->uiRxTime;    

    // Move pIrRxQRear to next
    pIrRxQRear = pIrRxQRear->pNext;
    
    return TRUE;
}

/**
  Get data from IR receiver data queue.

  Get data from the current QFront.
  
  @param pGetDataQ: Pointer of a data buffer used to record the available data from queue 
  @return TRUE: Data copied out from queue, FALSE: Queue is empty
*/
BOOL irrx_dequeueData(PIRRX_DATA_QUEUE pGetDataQ)
{
    if(!pGetDataQ)
        return FALSE;
        
    // Is queue empty?
    if(pIrRxQFront == pIrRxQRear)
        return FALSE;

    // Get data from queue to input data buffer
    pGetDataQ->uiRxData = pIrRxQFront->uiRxData;
    pGetDataQ->uiRxTime = pIrRxQFront->uiRxTime;    
    pGetDataQ->pNext = pIrRxQFront->pNext;    

    // Move pIrRxQFront to next    
    pIrRxQFront = pIrRxQFront->pNext;

    return TRUE;
}

