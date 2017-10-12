/**
    Slide Effect of push blending left to right.

    @file       SlideEffect_ClientLeft2Right.h
    @ingroup    mILibSlideEffect

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _SLIDEEFFECT_CLIENT_LEFT2RIGHT_H
#define _SLIDEEFFECT_CLIENT_LEFT2RIGHT_H

#include "SlideEffect.h"

/**
    @addtogroup mILibSlideEffect
*/


/**
     The push blending Effect.

     This API will push the next image from left to right with blending effect.

     @param[in]  DispIdx: the display index.
     @param[in]  pDrawImg: the image buffer to put the effect result.
     @param[in]  CurrFrameIdx: the frame index to run.

     @return the slide effect activity defined in PBX_SLIDE_EFFECT_ACT.
*/
extern INT32   SE_ClientLeft2Right(PBX_SE_DISP_IDX DispIdx, PIMG_BUF pDrawImg, UINT32 CurrFrameIdx);
#endif


