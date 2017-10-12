/**
    The Drape up to down Effect.

    @file       SlideEffect_R5HDrapeUp2Down.h
    @ingroup    mILibSlideEffect

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _SLIDEEFFECT_R5HDRAPEUP2DOWN_H
#define _SLIDEEFFECT_R5HDRAPEUP2DOWN_H

#include "SlideEffect.h"

/**
    @addtogroup mILibSlideEffect
*/


/**
     The Drape Effect.

     This API will divide the image with 5 areas in horizontal and drape the next image from up to down.

     @param[in]  DispIdx: the display index.
     @param[in]  pDrawImg: the image buffer to put the effect result.
     @param[in]  CurrFrameIdx: the frame index to run.

     @return the slide effect activity defined in PBX_SLIDE_EFFECT_ACT.
*/
extern INT32   SE_R5HDrapeUp2Down(PBX_SE_DISP_IDX DispIdx, PIMG_BUF pDrawImg, UINT32 CurrFrameIdx);

#endif


