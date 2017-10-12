/**
    Photo Frame

    Sample module detailed description.

    @file       PhotoFrame.h
    @ingroup    mILibPhotoFrame
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _PHOTO_FRAME_H_
#define _PHOTO_FRAME_H_

#include "type.h"

typedef enum {
    PF_YCC_CH_Y = 0,
    PF_YCC_CH_U,
    PF_YCC_CH_V,
    PF_YCC_CH_MAX,
    ENUM_DUMMY4WORD(PF_YCC_CH)
} PF_YCC_CH;

#define PF_YCC_CH_UV PF_YCC_CH_U

typedef enum {
    PF_YCCFORMAT_422 = 0,
    PF_YCCFORMAT_420,
    PF_YCCFORMAT_422_PACK,
    PF_YCCFORMAT_420_PACK,
    PF_YCCFORMAT_MAXIDX,
    ENUM_DUMMY4WORD(PF_YCC_FORMAT)
} PF_YCC_FORMAT;


typedef struct
{
    UINT32 Addr[PF_YCC_CH_MAX];
    UINT32 LineOffset[PF_YCC_CH_MAX];
    UINT32 Width[PF_YCC_CH_MAX];
    UINT32 Height[PF_YCC_CH_MAX];
    PF_YCC_FORMAT Format;
} PF_IMG_INFOR;
/**
    PF_BLENDING_INFOR
    Photo-Frame Information (Date-Image)
*/
typedef struct{
    UINT32 PosX;
    UINT32 PosY;
    PF_IMG_INFOR Img;
    UINT32 ColorKey;                ///< format 0x00YYUUVV
    UINT32 PFWeighting;             ///< 0 ~ 255
} PF_BLENDING_INFOR;


extern void PhotoFrame_Open(void);
extern UINT32 PhotoFrame_IsOpen(void);
extern void PhotoFrame_Close(void);
extern UINT32 PhotoFrame_GetMaxWorkBufSize(void);
extern ER PhotoFrame_SetInfor(PF_BLENDING_INFOR *Infor);
extern ER PhotoFrame_Imprint(PF_IMG_INFOR *Img, UINT32 WorkBuf, UINT32 WorkBufSize);
extern PF_BLENDING_INFOR PhotoFrame_GetInfor(void);
#endif //_DATA_STAMP_H_
