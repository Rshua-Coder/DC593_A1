/**
    The Fade Slide Effect.

    @file       SlideEffect_Fade.h
    @ingroup    mILibSlideEffect

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _SLIDEEFFECT_FADE_H
#define _SLIDEEFFECT_FADE_H

#include "SlideEffect.h"

/**
    @addtogroup mILibSlideEffect
*/


/**
     The Fade Effect.

     @param[in]  DispIdx: the display index.
     @param[in]  pDrawImg: the image buffer to put the effect result.
     @param[in]  CurrFrameIdx: the frame index to run.

     @return the slide effect activity defined in PBX_SLIDE_EFFECT_ACT.
*/
extern INT32   SE_Fade(PBX_SE_DISP_IDX DispIdx, PIMG_BUF pDrawImg, UINT32 CurrFrameIdx);


#endif


