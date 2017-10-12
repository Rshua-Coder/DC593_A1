/*
    Gyro Driver InvenSense IDG2020.

    @file       GyroDrv_EWTS9PDN21.c
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

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          GyroDrv
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define GDCONST_GD_NUM_PER_FRM (8)

typedef enum
{
    GYRO_IDLE   = 0,        ///< idle
    GYRO_READY  = 1,        ///< ready for read/write
    GYRO_RUN    = 2,        ///< running
    ENUM_DUMMY4WORD(GYRO_STATUS)
} GYRO_STATUS;


static GYRO_STATUS g_GyroStatus = GYRO_IDLE;
static GYRO_OP_MODE g_GyroOpMode = GYRO_OP_MODE_SINGLE_ACCESS;
static UINT32 g_uiGyroFrPeriod =   0;
static UINT32 g_uiGyroFrNumber =   0;
static SPI_ID g_TestSpiID = SPI_ID_3;
const static UINT32 g_uiReadFrequency = 10000000;//13298; // 10 MHz for read
const static UINT32 g_uiWriteFrequency = 1875000; // 1.875 MHz for write (temp, actually it should be 1MHz)
static INT16  g_xOffset = 0;
static INT16  g_yOffset = 0;
static void (*pGyroCB)(void) = NULL;

#define GDI2_GB_NUM (32)
static UINT32 puiGyroBufX[GDI2_GB_NUM][3]={0};
static UINT32 puiGyroBufY[GDI2_GB_NUM][3]={0};
static UINT32 uiGyroBufInIdx = 0, uiGyroBufOutIdx = 0;

static BOOL   bFirstDoCal = FALSE;

#define REG_GYRO_OUTPUTA             (0x7C)
#define REG_GYRO_OUTPUTB             (0x78)

#define ZeroOffsetMax                 54000
#define ZeroOffsetMin                 11500
#define CalSampleCount                100


static BOOL Gyro_Calibration(void)
{
 UINT32 TempX,TempY,GyroX=0,GyroY=0,index,retry=2;
 BOOL   bResult = FALSE;

    // Do calibration for GyroX/GyroY
    if (bFirstDoCal==FALSE)
    {
        bFirstDoCal = TRUE;
        while (retry)
        {
            for (index=0;index<CalSampleCount;index++)
            {
                gyro_readReg(REG_GYRO_OUTPUTA,&TempX);
                gyro_readReg(REG_GYRO_OUTPUTB,&TempY);
                if (TempX>ZeroOffsetMax||TempX<ZeroOffsetMin||TempY>ZeroOffsetMax||TempY<ZeroOffsetMin)
                {
                    DBG_ERR("GyroX:%d,GyroY:%d\r\n",TempX,TempY);
                    retry--;
                    break;
                }

                GyroX += TempX;
                GyroY += TempY;
            }

            if (index>=CalSampleCount)
            {
                bResult = TRUE;
                retry = 0;
            }
        }

        if (bResult==TRUE)
        {
            GyroX /= CalSampleCount;  g_xOffset = (INT16)(GyroX-32768)*-1;
            GyroY /= CalSampleCount;  g_xOffset = (INT16)(GyroX-32768)*-1;
            debug_msg("Calibration Done,offset X:%d,Y:%d\r\n",g_xOffset,g_yOffset);
        } else {
            debug_msg("Wrong GyroX/GyroY data generate!! \r\n");
        }
    }

    return bResult;
}

ER gyro_open(GYRO_OPENOBJ *openObj)
{

    // state check: only while idle
    if(g_GyroStatus!=GYRO_IDLE)
    {
        DBG_ERR("GYRO ERR: gyro_open, g_GyroStatus=%d!!\r\n", g_GyroStatus);
        return E_SYS;
    }

    uiGyroBufOutIdx = 0;
    uiGyroBufInIdx = 0;

    // parameter check
    if (openObj && openObj->FP_GYRO_CB)
    {
        pGyroCB = openObj->FP_GYRO_CB;
    }

    // power on gyro
    // always power-on

    // initializa gyro
    {
        spi_open(g_TestSpiID);
        spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_ENG_GYRO_UNIT, FALSE);  // normal mode
        spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_FREQ, g_uiReadFrequency);
        spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_BUSMODE, SPI_MODE_3);
        spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_MSB_LSB, SPI_MSB);
        spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_CS_ACT_LEVEL, SPI_CS_ACT_LEVEL_LOW);
        spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_CS_CK_DLY, 2);
        spi_setTransferLen(g_TestSpiID, SPI_TRANSFER_LEN_1BYTE);
        spi_close(g_TestSpiID);
    }

    // state change: ready for gyro
    g_GyroStatus = GYRO_READY;

    // Do gyro calibration
    Gyro_Calibration();

    return E_OK;
}


ER gyro_close(void)
{
    // state check: only while idle
    if(g_GyroStatus!=GYRO_READY)
    {
        DBG_ERR("GYRO ERR: gyro_close, g_GyroStatus=%d!!\r\n", g_GyroStatus);
        return E_SYS;
    }

    // stop/pause gyro
    // not-yet

    // power off gyro
    // always power-on

    // state change: ready for gyro
    g_GyroStatus = GYRO_IDLE;

    return E_OK;
}

ER gyro_readReg(UINT32 uiAddr, UINT32 * puiData)
{
  UINT32 uiRecv[3],index;


    // state check: only while ready
    if(g_GyroStatus!=GYRO_READY)
    {
        DBG_ERR("GYRO ERR: gyro_readReg, g_GyroStatus=%d!!\r\n", g_GyroStatus);
        return E_SYS;
    }

    if (uiAddr!=REG_GYRO_OUTPUTA&&uiAddr!=REG_GYRO_OUTPUTB)
    {
        DBG_ERR("only support register 0x7c/0x78 read\r\n");
        return E_SYS;
    }

    // state change: ready -> run
    g_GyroStatus = GYRO_RUN;

    // read register
    spi_open(g_TestSpiID);

    spi_setCSActive(g_TestSpiID, TRUE);
    spi_setTransferLen(g_TestSpiID, SPI_TRANSFER_LEN_1BYTE);
    // write register cmd/read data simultaneously
    spi_writeReadSingle(g_TestSpiID, uiAddr, &uiRecv[0]);
    // read register data
    for (index=0;index<3;index++)
        spi_readSingle(g_TestSpiID,&uiRecv[index]);

    spi_setCSActive(g_TestSpiID, FALSE);
    spi_close(g_TestSpiID);

    if (uiRecv[2] ==  0x53)
    {
        *puiData = ((uiRecv[0]<<8)&0xFF00)|(uiRecv[1]&0x00FF);
    } else {
        DBG_ERR(" error register value\r\n");
        *puiData = 0;
    }

    // state change: run -> ready
    g_GyroStatus = GYRO_READY;
    return E_OK;
}

ER gyro_writeReg(UINT32 uiAddr, UINT32 uiData)
{
    return E_OK;
}

ER gyro_setFreeRunParam(GYRO_FREE_RUN_PARAM *frParam)
{
    // state check: only while idle
    //not-yet

    g_uiGyroFrPeriod = frParam->uiPeriodUs;
    g_uiGyroFrNumber = frParam->uiDataNum;

    return E_OK;
}


static void gyro_HandleGyroEvent(SPI_GYRO_INT gyroInt)
{
    ER Status;
    UINT32 i, j, k;
    GYRO_BUF_QUEUE gyroData;

    switch (gyroInt)
    {
    case SPI_GYRO_INT_SYNC_END:
        //GD_ERR_MSG("uiGyroBufInIdx %d\r\n", uiGyroBufInIdx);
        j = uiGyroBufInIdx;
        for (i=0; i<g_uiGyroFrNumber; i++)
        {
            // 0x7C
            Status = spi_getGyroData(g_TestSpiID, &gyroData);
            if(Status!=E_OK)
            {
                gyroData.uiFrameID      = 9;
                gyroData.uiDataID       = 9;
                gyroData.vRecvWord[0]   = 9;
                gyroData.vRecvWord[1]   = 9;
            }

            k = j%GDI2_GB_NUM;
            puiGyroBufX[k][0] = gyroData.uiFrameID;
            puiGyroBufX[k][1] = gyroData.uiDataID;

            //  x =  byte 1 | byte2 (byte 0 is address, we ignore it)
            puiGyroBufX[k][2] = (gyroData.vRecvWord[0]&0xFF00) | ((gyroData.vRecvWord[0]>>16)&0xFF);

            // 0x78
            Status = spi_getGyroData(g_TestSpiID, &gyroData);
            if(Status!=E_OK)
            {
                gyroData.uiFrameID      = 9;
                gyroData.uiDataID       = 9;
                gyroData.vRecvWord[0]   = 9;
                gyroData.vRecvWord[1]   = 9;
            }

            puiGyroBufY[k][0] = gyroData.uiFrameID;
            puiGyroBufY[k][1] = gyroData.uiDataID;
            //  x =  byte 1 | byte2 (byte 0 is address, we ignore it)
            puiGyroBufY[k][2] = (gyroData.vRecvWord[0]&0xFF00) | ((gyroData.vRecvWord[0]>>16)&0xFF);
            j++;
            //debug_msg("(X,Y)=(%x,%x)\r\n",puiGyroBufX[k][2],puiGyroBufY[k][2]);
        }
        uiGyroBufInIdx = j;
        break;
    case SPI_GYRO_INT_OVERRUN:
        DBG_ERR("SPI_GYRO_INT_OVERRUN!!\r\n");
        //set_flg(FLG_ID_IDG2020, IDG2020_FLG_HW_OVERRUN);
        break;
    case SPI_GYRO_INT_SEQ_ERR:
        DBG_ERR("SPI_GYRO_INT_SEQ_ERR!!\r\n");
        //set_flg(FLG_ID_IDG2020, IDG2020_FLG_SEQ_ERR);
        break;

    case SPI_GYRO_INT_QUEUE_THRESHOLD:
        DBG_ERR("SPI_GYRO_INT_QUEUE_THRESHOLD!!\r\n");
        //set_flg(FLG_ID_IDG2020, IDG2020_FLG_QUEUE_THRESHOLD);
        break;
    case SPI_GYRO_INT_QUEUE_OVERRUN:
        DBG_ERR("SPI_GYRO_INT_QUEUE_OVERRUN!!\r\n");
        //set_flg(FLG_ID_IDG2020, IDG2020_FLG_QUEUE_OVERRUN);
        break;
    default:
        DBG_ERR("%d\r\n",gyroInt);
        break;
    }


    if(pGyroCB!=NULL)
    {
        pGyroCB();
    }
}

static ER gyro_setFreeRun(void)
{
    UINT32 i;
    SPI_GYRO_INFO gyroInfo= {0};

    //
    // 1. Prepare data buffer to store polled gyroscope data
    //
    //useless in new driver//uiDumpCount = 0;
    //useless in new driver//pNextBuf = (UINT32*)uiDumpBufAddr;

    //
    // 2. Start SPI gyro polling
    //
    gyroInfo.gyroMode = SPI_GYRO_MODE_SIE_SYNC;

//    gyroInfo.uiTransferCount = g_uiGyroFrNumber/2;  // 4 Transfer => Total 8 CS in one frame
    gyroInfo.uiTransferCount = g_uiGyroFrNumber;  // 4 Transfer => Total 8 CS in one frame
    gyroInfo.uiOpInterval       = 1;//g_uiGyroFrPeriod/g_uiGyroFrNumber;
    gyroInfo.uiTransferInterval = g_uiGyroFrPeriod/g_uiGyroFrNumber;
    gyroInfo.uiOp0Length = 4;
    gyroInfo.vOp0OutData[0] = REG_GYRO_OUTPUTA;
    for (i=1; i<gyroInfo.uiOp0Length; i++)
    {
        gyroInfo.vOp0OutData[i] = 0;
    }

    gyroInfo.uiOp1Length = 4;
    gyroInfo.vOp1OutData[0] = REG_GYRO_OUTPUTB;
    for (i=1; i<gyroInfo.uiOp1Length; i++)
    {
        gyroInfo.vOp1OutData[i] = 0;
    }

    gyroInfo.pEventHandler = gyro_HandleGyroEvent;

    spi_open(g_TestSpiID);

    spi_setTransferLen(g_TestSpiID, SPI_TRANSFER_LEN_1BYTE);

    spi_startGyro(g_TestSpiID, &gyroInfo);


    return E_OK;
}

void gyro_RSCInfoInit(GYRO_RSCINFOINIT *pRSCInfoInfo)
{
    // please refer to IPL_Ctrl_RSC_Int.h file for the GDFmt/FDDir definition
    pRSCInfoInfo->GDFmt = 0;//_INT16
    //pRSCInfoInfo->GDDir = 4;//_RSC_GYRO_DIR_RIGHT_UP
    pRSCInfoInfo->GDDir = 3;//_RSC_GYRO_DIR_RIGHT_UP
    pRSCInfoInfo->GDunit = (65536/50);
}

ER gyro_setMode(GYRO_OP_MODE opMode)
{
    DBG_IND("gyro_setMode %d!!\r\n", opMode);

    // state check: only while idle
    if((g_GyroStatus!=GYRO_READY) && (g_GyroOpMode!=GYRO_OP_MODE_FREE_RUN))
    {
        DBG_ERR("gyro_setMode, g_GyroOpMode = %d, g_GyroStatus=%d!!\r\n", g_GyroOpMode, g_GyroStatus);
        return E_SYS;
    }

    if(opMode==GYRO_OP_MODE_SINGLE_ACCESS)
    {
        if(g_GyroOpMode==GYRO_OP_MODE_SINGLE_ACCESS)
        {
            DBG_ERR("gyro_setMode, already single access mode!!\r\n");
            return E_SYS;
        }
        else if(g_GyroOpMode==GYRO_OP_MODE_FREE_RUN)
        {
            // disable free run
            spi_stopGyro(g_TestSpiID);

            g_GyroOpMode = GYRO_OP_MODE_SINGLE_ACCESS;
            g_GyroStatus = GYRO_READY;
        }
        else
        {
            DBG_ERR("gyro_setMode, g_GyroOpMode=%d!!\r\n", opMode);
            return E_SYS;
        }
    }
    else if(opMode==GYRO_OP_MODE_FREE_RUN)
    {
        if(g_GyroOpMode==GYRO_OP_MODE_FREE_RUN)
        {
            DBG_ERR("gyro_setMode, already free run mode!!\r\n");
            return E_SYS;
        }
        else if(g_GyroOpMode==GYRO_OP_MODE_SINGLE_ACCESS)
        {
            if(g_uiGyroFrPeriod==0)
            {
                DBG_ERR("gyro_setMode, run mode param is not set!!\r\n");
                return E_SYS;
            }
            // enable free run
            gyro_setFreeRun();

            g_GyroOpMode = GYRO_OP_MODE_FREE_RUN;
            g_GyroStatus = GYRO_RUN;
        }
        else
        {
            DBG_ERR("gyro_setMode, g_GyroOpMode=%d!!\r\n", opMode);
            return E_SYS;
        }
    }
    else
    {
        DBG_ERR("gyro_setMode, opMode=%d!!\r\n", opMode);
        return E_SYS;
    }

    return E_OK;
}

static void gyro_AdjustDataOffset(UINT32* gx, UINT32* gy)
{
    INT32  tempX,tempY;

    tempX = *gx;
    if ((INT32)tempX + (INT32)g_xOffset > 54000)
        tempX = 54000;
    else if ((INT32)tempX + (INT32)g_xOffset < 11500)
        tempX = 11500;
    else
        tempX +=g_xOffset;

    tempY = *gy;
    if ((INT32)tempY + (INT32)g_yOffset > 54000)
        tempY = 54000;
    else if ((INT32)tempY + (INT32)g_yOffset < 11500)
        tempY = 11500;
    else
        tempY +=g_yOffset;

    tempX -= 32768;
    tempY -= 32768;

    DBG_IND("gx=%x, gy=%x, g_xOffset = %ld, g_yOffset = %ld, tempX=%d, tempY=%d\r\n",*gx,*gy,g_xOffset,g_yOffset,tempX,tempY);

    *gx = (INT16)(-tempX);
    *gy = (INT16)(tempY);

    DBG_IND("gx=%ld, gy=%ld \r\n",*gx,*gy);

}


ER gyro_getFreeRunData(UINT32 *puiNum, UINT32 *puiDatX, UINT32 *puiDatY)
{
    UINT32 i, j;//, k;
    // state check: only while run
    if(g_GyroStatus!=GYRO_RUN)
    {
        DBG_ERR("gyro_getGyroData, g_GyroStatus=%d!!\r\n", g_GyroStatus);
        return E_SYS;
    }

    // op mode check: only while free run mode
    if(g_GyroOpMode!=GYRO_OP_MODE_FREE_RUN)
    {
        DBG_ERR("gyro_getGyroData, not in free run mode!!\r\n", g_GyroOpMode);
        return E_SYS;
    }

    // normal job
    //GD_ERR_MSG("uiGyroBufOutIdx %d\r\n", uiGyroBufOutIdx);
    if((uiGyroBufOutIdx+g_uiGyroFrNumber)==uiGyroBufInIdx)
    {
        j = uiGyroBufOutIdx;
        for (i=0; i<g_uiGyroFrNumber; i++)
        {
            UINT32 k;
            k = j%GDI2_GB_NUM;
            *(puiDatX+i) = puiGyroBufX[k][2];
            *(puiDatY+i) = puiGyroBufY[k][2];
            gyro_AdjustDataOffset(puiDatX+i,puiDatY+i);
            j++;
        }
        uiGyroBufOutIdx = j;
        *puiNum = g_uiGyroFrNumber;
    }
    else if((uiGyroBufOutIdx+g_uiGyroFrNumber)>uiGyroBufInIdx)
    {
        DBG_ERR("asking for future data!!\r\n");
        memset((void *)puiDatX, 0, g_uiGyroFrNumber*sizeof(UINT32)>>1);
        memset((void *)puiDatY, 0, g_uiGyroFrNumber*sizeof(UINT32)>>1);
        *puiNum = 0;
    }
    else if((uiGyroBufOutIdx+g_uiGyroFrNumber)<uiGyroBufInIdx)
    {
        DBG_ERR("asking for past data!!\r\n");
        
        uiGyroBufOutIdx =  uiGyroBufInIdx;
        memset((void *)puiDatX, 0, g_uiGyroFrNumber*sizeof(UINT32)>>1);
        memset((void *)puiDatY, 0, g_uiGyroFrNumber*sizeof(UINT32)>>1);        
        *puiNum = g_uiGyroFrNumber;
    }

    // state change: run -> run
    g_GyroStatus = GYRO_RUN;
    return E_OK;
}

void gyro_setCalZeroOffset(INT16 xOffset, INT16 yOffset)
{
    g_xOffset = xOffset;
    g_yOffset = yOffset;
    DBG_IND("xOffset = %d, yOffset = %d\r\n",xOffset,yOffset);
}


