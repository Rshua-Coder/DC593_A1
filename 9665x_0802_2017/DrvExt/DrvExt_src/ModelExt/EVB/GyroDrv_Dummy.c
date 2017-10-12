/*
    Gyro Driver InvenSense IDG2020.

    @file       GyroDrv_IDG2020.c
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#include <stdio.h>
#include <String.h>
#include "Debug.h"
#include "type.h"
#include "ErrorNo.h"

#include "Debug.h"
#include "Cache.h"
#include "dma.h"
#include "GyroDrv.h"
#include "spi.h"
#include "SysKer.h"
#include "Delay.h"

#define GD_ERR_MSG(...)               debug_msg("^R\033[1m"__VA_ARGS__)
#define GD_WRN_MSG(...)               debug_msg("^Y\033[1m"__VA_ARGS__)
#define GD_MSG_MSG(...)               debug_msg("^M\033[1m"__VA_ARGS__)
#define GD_TST_MSG(...)               //debug_msg("^M\033[1m"__VA_ARGS__)


ER gyro_open(GYRO_OPENOBJ *openObj)
{
    return E_OK;
}


ER gyro_close(void)
{
    return E_OK;
}

ER gyro_readReg(UINT32 uiAddr, UINT32 * puiData)
{
    return E_OK;
}

ER gyro_writeReg(UINT32 uiAddr, UINT32 uiData)
{
    return E_OK;
}


ER gyro_setFreeRunParam(GYRO_FREE_RUN_PARAM *frParam)
{
    return E_OK;
}


ER gyro_setMode(GYRO_OP_MODE opMode)
{
    return E_OK;
}


ER gyro_getFreeRunData(UINT32 *puiNum, UINT32 *puiDatX, UINT32 *puiDatY)
{
    return E_OK;
}

void gyro_setCalZeroOffset(INT16 xOffset, INT16 yOffset)
{
}
void gyro_RSCInfoInit(GYRO_RSCINFOINIT *pRSCInfoInfo)
{

}

