/**
    Gyro Driver InvenSense IDG2020.

    @file       GyroDrv.h
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _GYRO_DRV_IDG2020_H
#define _GYRO_DRV_IDG2020_H

#include "Type.h"
#include "spi.h"

typedef enum
{
    GYRO_OP_MODE_SINGLE_ACCESS,       ///< gyro in single access mode, read/write register one by one
    GYRO_OP_MODE_FREE_RUN,       ///< gyro in free run mode, read register after periodic trigger automatically
    ENUM_DUMMY4WORD(GYRO_OP_MODE)
} GYRO_OP_MODE;


typedef struct GYRO_FREE_RUN_PARAM{
    UINT32 uiPeriodUs;  ///< free run period in Us
    UINT32 uiDataNum;   ///< gyro data number in a period
} GYRO_FREE_RUN_PARAM;

typedef struct GYRO_OPENOBJ{
    void (*FP_GYRO_CB)(void); ///< gyro callback function
} GYRO_OPENOBJ;

typedef struct GYRO_STARTOBJ {
    SPI_ID SpiID;
    SPI_GYRO_INFO *pSPIGyroInfo;
}GYRO_STARTOBJ;

typedef struct GYRO_RSCINFOINIT {
    UINT32 GDFmt;
    UINT32 GDDir;
    UINT32 GDunit;
}GYRO_RSCINFOINIT;

extern ER gyro_open(GYRO_OPENOBJ *openObj);
extern ER gyro_close(void);
extern ER gyro_readReg(UINT32 uiAddr, UINT32 * puiData);
extern ER gyro_writeReg(UINT32 uiAddr, UINT32 uiData);
extern ER gyro_setFreeRunParam(GYRO_FREE_RUN_PARAM *frParam);
extern ER gyro_setMode(GYRO_OP_MODE opMode);
extern ER gyro_getFreeRunData(UINT32 *puiNum, UINT32 *puiDatX, UINT32 *puiDatY);
extern void gyro_setCalZeroOffset(INT16 xOffset, INT16 yOffset);

extern void gyro_FreeRunStart(GYRO_STARTOBJ *pgyroobj);
extern void gyro_RSCInfoInit(GYRO_RSCINFOINIT *pgyrorscinit);


#endif// _GYRO_DRV_IDG2020_H

