/**
    The Push Fade left to right Effect.

    @file       SlideEffect_PushFadeRight2Left.h
    @ingroup    mILibSlideEffect

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _SLIDEEFFECT_PUSHFADERIGHT2LEFT_H
#define _SLIDEEFFECT_PUSHFADERIGHT2LEFT_H

#include "SlideEffect.h"

/**
    @addtogroup mILibSlideEffect
*/

/**
     The Push Fade Effect.

     This API will push the next image from right to left with fade effect.

     @param[in]  DispIdx: the display index.
     @param[in]  pDrawImg: the image buffer to put the effect result.
     @param[in]  CurrFrameIdx: the frame index to run.

     @return the slide effect activity defined in PBX_SLIDE_EFFECT_ACT.
*/
extern INT32   SE_PushFadeRight2Left(PBX_SE_DISP_IDX DispIdx, PIMG_BUF pDrawImg, UINT32 CurrFrameIdx);


#endif


