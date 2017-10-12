/**
    data stamp

    Sample module detailed description.

    @file       datastamp.h
    @ingroup    mILibDataStamp
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DATA_STAMP_H_
#define _DATA_STAMP_H_

#include "type.h"
#include "GxImage.h"

/**
    DS_STAMP_INFOR
    Data-Stamp Information (Date-Image)
*/
typedef struct{
    UINT32  PosX;
    UINT32  PosY;
    IMG_BUF Img;
    UINT32  ColorKey;                ///< format 0x00YYUUVV
    UINT32  StampWeight;             ///< 0 ~ 255
} DS_STAMP_INFOR;


extern void DataStamp_Open(void);
extern UINT32 DataStamp_IsOpen(void);
extern void DataStamp_Close(void);
extern UINT32 DataStamp_GetMaxWorkBufSize(void);
extern ER DataStamp_SetInfor(DS_STAMP_INFOR *Infor);
extern DS_STAMP_INFOR DataStamp_GetInfor(void);
extern ER DataStamp_Imprint(IMG_BUF *Img, UINT32 WorkBuf, UINT32 WorkBufSize);

#endif //_DATA_STAMP_H_
