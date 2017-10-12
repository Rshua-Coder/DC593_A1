/**
    Image Effect api.


    @file       ie_api.h
    @ingroup    mILibAEAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IE_API_H_
#define _IE_API_H_

typedef struct
{
    UINT32 format;
    UINT32 width;
    UINT32 height;
    UINT32 pY;
    UINT32 pU;
    UINT32 pV;
} IE_YUV_INFO;

ER IE_PerformFilter(IE_YUV_INFO *srcImage, IE_YUV_INFO *patImage, IE_YUV_INFO *destImage, unsigned char *workBuf, long filterID);
#endif //_IE_API_H_
