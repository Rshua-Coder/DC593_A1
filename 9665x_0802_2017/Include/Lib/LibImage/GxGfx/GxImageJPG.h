/**
    JPG related utility.

    JPG related utility.

    @file       GxImageJPG.h
    @ingroup    mIGxgfx
    @note       Nothing

    COLOR_ITEM; color value

    PALETTE_ITEM; Palette color value

    MAPPING_ITEM; Color index mapping value

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/


/*
*/

#ifndef _GX_IMAGEJPG_H_
#define _GX_IMAGEJPG_H_

#include "GxCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
    @addtogroup mIGxgfx
*/
//@{
//--------------------------------------------------------------------------------------
/**
     Get jpg information from jpg bitStream.

     @param[in] JpegBitStream          Jpg BitStream data.
     @param[in] uiBitStreamSize        Jpg BitStream size.
     @param[out] pW                    Jpg width.
     @param[out] pH                    Jpg height.
     @param[out] pFMT                  Jpg format.

     @return
        - @b TRUE if success
        - @b FALSE if fail
*/
BOOL Jpeg_DecodeInfo(const UINT8* JpegBitStream, UINT32 uiBitStreamSize,
        UINT16* pW,  UINT16* pH, UINT16* pFMT);

/**
     Decode jpeg image
     @note  pBuf must 32 byte alignment

     @param[in] JpegBitStream          Jpg BitStream data.
     @param[in] uiBitStreamSize        Jpg BitStream size.
     @param[in] pBuf                   Destination YUV raw image buffer address.
     @param[in] uiBufSize              Destination buffer size.

     @return
        - @b TRUE if success
        - @b FALSE if fail
*/
BOOL Jpeg_DecodeImage(const UINT8* JpegBitStream, UINT32 uiBitStreamSize,
        UINT8* pBuf, UINT32 uiBufSize);

/**
     Get jpeg image YUV from decoded YUV raw image buffer

     @param[in] w             Jpg width.
     @param[in] h             Jpg height.
     @param[in] fmt           Jpg format.
     @param[in] pBuf          Destination YUV raw image buffer address.
     @param[in] uiOutAddrY    Destination Y buffer address
     @param[in] uiOutAddrCb   Destination Cb buffer address
     @param[in] uiOutAddrCr   Destination Cr buffer address

     @return
        - @b TRUE if success
        - @b FALSE if fail
*/
BOOL Jpeg_GetImageAddr(UINT16 w, UINT16 h, UINT16 fmt,
        UINT8* pBuf, UINT32* uiOutAddrY, UINT32* uiOutAddrCb, UINT32* uiOutAddrCr);

//@}

#ifdef __cplusplus
} //extern "C"
#endif


#endif//_GX_IMAGEJPG_H_
