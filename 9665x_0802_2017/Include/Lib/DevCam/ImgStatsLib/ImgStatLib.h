/**
    Image Statistic

    Image Statistic API.

    @file       ImgStatLib.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IMG_STAT_LIB_H_
#define _IMG_STAT_LIB_H_

#include "Type.h"

typedef enum
{
    IMGSTAT_TYPE_PLANAR = 0,
    IMGSTAT_TYPE_BAYER_PROGRESSIVE,
    ENUM_DUMMY4WORD(IMGSTAT_TYPE)
} IMGSTAT_TYPE;

typedef enum
{
    IMGSTAT_STPIX_8 = 0,
    IMGSTAT_STPIX_10,
    IMGSTAT_STPIX_12,
    IMGSTAT_STPIX_16,
    ENUM_DUMMY4WORD(IMGSTAT_BIT)
} IMGSTAT_BIT;

typedef enum
{
    IMGSTAT_STPIX_R = 0,
    IMGSTAT_STPIX_GR,
    IMGSTAT_STPIX_GB,
    IMGSTAT_STPIX_B,
    ENUM_DUMMY4WORD(IMGSTAT_STPIX)
} IMGSTAT_STPIX;

typedef struct
{
    IMGSTAT_TYPE Type;      ///< image type
    UINT32 Addr;            ///< image addr
    UINT32 HSize;           ///< image H Size
    UINT32 VSize;           ///< image V Size
    UINT32 LineOfs;         ///< image line offset
    IMGSTAT_STPIX StPix;    ///< start pixel, only used on bayer format
    IMGSTAT_BIT Bit;        ///< bit number, only used on bayer format
    UINT32 SubRate;         ///< sub sample rate
    UINT32 WinHNum;         ///< CA window H number
    UINT32 WinVNum;         ///< CA window V number
    UINT32 BinNum;          ///< histogram bin number
} IMGSTAT_INFO;

typedef struct
{
    UINT32 *RData;          ///< the array pointer, index must be equal WinHNum x WinVNum
    UINT32 *GData;          ///< the array pointer, index must be equal WinHNum x WinVNum
    UINT32 *BData;          ///< the array pointer, index must be equal WinHNum x WinVNum
} IMGSTAT_CA_INFO;

typedef struct
{
    UINT32 *YData;          ///< the array pointer, index must be equal WinHNum x WinVNum
} IMGSTAT_Y_INFO;

typedef struct
{
    UINT32 *HistData;       ///< the array pointer, index must be equal BinNum
} IMGSTAT_HISTO_INFO;


extern void ImgStat_CalBayerImg(IMGSTAT_INFO *In, IMGSTAT_CA_INFO *CAOut, IMGSTAT_HISTO_INFO *HistOut);
extern void ImgStat_CalYImg(IMGSTAT_INFO *In, IMGSTAT_Y_INFO *YOut, IMGSTAT_HISTO_INFO *HistOut);
extern void Img_GetCA_Result(IPL_PROC_ID IPL_ID, UINT16 *Rdata, UINT16 *Gdata, UINT16 *Bdata, UINT32 WinNum);
#endif //_IMG_STAT_LIB_H_