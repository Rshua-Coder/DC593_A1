/**
    The Drape up to down Effect.

    @file       SlideEffect_SlideinRight2Left.h
    @ingroup    mILibSlideEffect

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _SLIDEEFFECT_SLIDEINRIGHT2LEFT_H
#define _SLIDEEFFECT_SLIDEINRIGHT2LEFT_H

#include "SlideEffect.h"

/**
     The Slide in Effect.

     This API will slide in the next image from right to left.

     @param[in]  DispIdx: the display index.
     @param[in]  pDrawImg: the image buffer to put the effect result.
     @param[in]  CurrFrameIdx: the frame index to run.

     @return the slide effect activity defined in PBX_SLIDE_EFFECT_ACT.
*/
extern INT32   SE_SlideinRight2Left(PBX_SE_DISP_IDX DispIdx, PIMG_BUF pDrawImg, UINT32 CurrFrameIdx);
#endif


