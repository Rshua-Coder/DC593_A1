/**
    The Slide in to out Effect.

    @file       SlideEffect_R2VIn2Out.h
    @ingroup    mILibSlideEffect

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _SLIDEEFFECT_R2VIN2OUT_H
#define _SLIDEEFFECT_R2VIN2OUT_H

#include "SlideEffect.h"

/**
    @addtogroup mILibSlideEffect
*/

/**
    Get SlideEffect R2VIn2Out library version number.

    @return the version number string.
*/
CHAR *SE_R2VIn2Out_GetVerInfo(void);

/**
    Get SlideEffect R2VIn2Out library build date information.

    @return the library build date string.
*/
CHAR *SE_R2VIn2Out_GetBuildDate(void);

/**
     The Slide in to out Effect.

     This API will divide the image with 2 areas in vertical and slide from image center to outside.

     @param[in]  DispIdx: the display index.
     @param[in]  pDrawImg: the image buffer to put the effect result.
     @param[in]  CurrFrameIdx: the frame index to run.

     @return the slide effect activity defined in PBX_SLIDE_EFFECT_ACT.
*/
extern INT32   SE_R2VIn2Out(PBX_SE_DISP_IDX DispIdx, PIMG_BUF pDrawImg, UINT32 CurrFrameIdx);

#endif


