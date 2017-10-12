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


/**
  Open IR receiver and start a reference timer.

  Open IR receiver and start a reference timer.

  @param pIrRxDevObj: IrRx device object pointer
  @return E_OK if success, E_SYS if already opened or timer is not available
*/
ER irrx_open(PIRRX_DEVICE_OBJ pIrRxDevObj)
{
    debug_msg(("Open Dummy IRRM! \r\n"));

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
    debug_msg(("Close Dummy IRRM! \r\n"));
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
    return 0x00;
}

/**
  Check whether one data frame is fully received ok or not.

  For outside task checking if there is one data frame ready to pick. (If w/o IrRx data queue)

  @param void
  @return TRUE: Data is ready
*/
BOOL irrx_isDataReady(void)
{
    return FALSE;
}

/**
  Check whether a Repeat Code is received or not.

  For outside task checking if there is one Repeat Code need to report. (If w/o IrRx data queue)

  @param void
  @return TRUE: Repeat Code is still sending
*/
BOOL irrx_isDataRepeat(void)
{
    return FALSE;
}




//@}
