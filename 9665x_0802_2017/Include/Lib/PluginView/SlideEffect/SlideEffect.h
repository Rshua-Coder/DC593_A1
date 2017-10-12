/**
    Playback Slide Effect plugin module.

    All functions for Slide effect, This module can be a plug-in to Application Playback.

    @file       SlideEffect.h
    @ingroup    mILibSlideEffect

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _SLIDEEFFECT_H
#define _SLIDEEFFECT_H

#include "Type.h"
#include "GxImage.h"

/**
    @addtogroup mILibSlideEffect
*/

#define PBX_SLIDE_EFFECT_IMAGE_STEP                 8       ///< slide effect separate to 8 steps(8 frames)
#define PBX_SLIDE_EFFECT_FRAME_START_IDX            1       ///< slide effect frame index start from 1
#define PBX_SLIDE_EFFECT_NONE                       0       ///< slide effect function is NULL



/**
     Slide effect activity.

     The activity will be returned after running each slide effect step.

*/
typedef enum _PBX_SLIDE_EFFECT_ACT
{
    PBX_SLIDE_EFFECT_ACT_CONTINUE = 0x00,          ///< the slide effect not reaches the last frame, need to continue.
    PBX_SLIDE_EFFECT_ACT_STOP     = 0x01,          ///< the slide effect is stopped by user callback command.
    PBX_SLIDE_EFFECT_ACT_END      = 0x02,          ///< the slide effect is running complete(reaches the last frame).
    PBX_SLIDE_EFFECT_ACT_ERROR    = 0x03,          ///< some errors happened when running slide effect, may be the work buffer size is too small.
    ENUM_DUMMY4WORD(PBX_SLIDE_EFFECT_ACT)
} PBX_SLIDE_EFFECT_ACT;

/**
     Slide effect display index.

     The display index is to support dual display.

*/
typedef enum _PBX_SE_DISP_IDX{
    PBX_SE_DISP_IDX_PRIMARY = 0,            ///< primary source
    PBX_SE_DISP_IDX_SECONDARY,              ///< secondary display
    PBX_SE_DISP_IDX_MAX_NUM,
    ENUM_DUMMY4WORD(PBX_SE_DISP_IDX)
}PBX_SE_DISP_IDX;


/**
     Slide effect init parameters.

*/
typedef struct _PBX_SLIDEEFFECT_OBJ
{
    PIMG_BUF    pNextImg;                   ///< [in]next image to display
    PIMG_BUF    pCurrImg;                   ///< [in]current display image
    UINT32      uiWorkBuf;                  ///< [in]working buffer
    UINT32      uiWorkBufSize;              ///< [in]working buffer size
    UINT8       *SpeedTable;                ///< [in]the delay time of each slide effect frame, UINT8 SpeedTable[PBX_SLIDE_EFFECT_IMAGE_STEP];
} PBX_SLIDEEFFECT_OBJ, *PPBX_SLIDEEFFECT_OBJ;

/**
    Slide effect funcion proto type
*/
typedef INT32 (*PBX_SLIDE_EFFECT_FUNC)(PBX_SE_DISP_IDX DispIdx, PIMG_BUF pDrawImg, UINT32 CurrFrameIdx);

/**
    Callback funcion proto type
*/
typedef PBX_SLIDE_EFFECT_ACT (*PBX_SLIDE_EFFECT_CB)(void);

/**
    Global variable for storing Callback function.
*/
extern PBX_SLIDE_EFFECT_CB    gSlideEffectCB;
/**
    Global variable for storing slide effect init parameters.
*/
extern PBX_SLIDEEFFECT_OBJ    gSlideEffectObj[PBX_SE_DISP_IDX_MAX_NUM];


/**
     Set Slide Effect function.

     This function will register the slide effect funtion for running later.

     @param[in]  p_pbxSlideEffectFunc:  the slide effect function pointer.

     Example:
     @code
     {
        PBX_SlideEffect_SetEffect(SE_Fade);
     }
     @endcode
*/
extern void    PBX_SlideEffect_SetEffect(PBX_SLIDE_EFFECT_FUNC  p_pbxSlideEffectFunc);

/**
     Register slide effect callback function.

     User can register the callback function to immediately stop the slide effect when slide effect is running.

     @param[in]  pbxSlideEffectCB   the callback function pointer.

     Example:
     @code
     {
        PBX_SLIDE_EFFECT_ACT  slideCB(void)
        {
             if user press stop key
                 return PBX_SLIDE_EFFECT_ACT_STOP;
             else
                 return PBX_SLIDE_EFFECT_ACT_CONTINUE;
        }
        PBX_SlideEffect_SetCB(slideCB);
     }
     @endcode
*/
extern void PBX_SlideEffect_SetCB(PBX_SLIDE_EFFECT_CB   pbxSlideEffectCB);

/**
     Init Slide Effect parameters.

     User must calls this API to init parameters when user want to run the first frame of slide effect.\n

     @param[in]  DispIdx: the display index.
     @param[in]  p_pbxSEObj: the init parameters.
*/
extern void PBX_SlideEffect_Init(PBX_SE_DISP_IDX DispIdx, PPBX_SLIDEEFFECT_OBJ p_pbxSEObj);

/**
     Run the Slide Effect.

     This function will run the slide effect function that register in PBX_SlideEffect_SetEffect().\n

     @note This API just runs the slide effect one step. For the normally case, we separate the slide effect to 8 steps or 16 steps.\n
           So user need to use a while loop to call the API until the API returns  the value other than PBX_SLIDE_EFFECT_ACT_ CONTINUE.

     @param[in]  DispIdx: the display index.
     @param[in]  pDrawImg: the image buffer to put the effect result.
     @param[in]  CurrFrameIdx: the frame index to run.

     @return the slide effect activity defined in PBX_SLIDE_EFFECT_ACT.
         - @b PBX_SLIDE_EFFECT_ACT_CONTINUE:  the slide effect not reaches the last frame, need to continue.
         - @b PBX_SLIDE_EFFECT_ACT_STOP:   the slide effect need to stop because of user callback to stop it.
         - @b PBX_SLIDE_EFFECT_ACT_END:    the slide effect is running complete (reaches the last frame).
         - @b PBX_SLIDE_EFFECT_ACT_ERROR:   some errors happened when running slide effect, may be the work buffer size is too small.

     Example:
     @code
     {
        currFrameIdx = PBX_SLIDE_EFFECT_FRAME_START_IDX;
        while (1)
        {
            if (currFrameIdx == PBX_SLIDE_EFFECT_FRAME_START_IDX)
            {
                slideObj.uiWorkBuf = workbuff;
                slideObj.uiWorkBufSize = buffSize;
                slideObj.pNextImg = &m_ImgDisp[1];
                slideObj.pCurrImg = &m_ImgDisp[0];
                PBX_SlideEffect_Init(PBX_SE_DISP_IDX_PRIMARY, &slideObj);
                PBX_SlideEffect_SetEffect((PBX_SLIDE_EFFECT_FUNC)SE_Fade);
            }
            er = PBX_SlideEffect_Run(PBX_SE_DISP_IDX_PRIMARY, pDrawImg,currFrameIdx);
            currFrameIdx++;
            if (er != PBX_SLIDE_EFFECT_ACT_CONTINUE)
            {
                break;
            }
        }
     }
     @endcode
*/
extern INT32 PBX_SlideEffect_Run(PBX_SE_DISP_IDX DispIdx, PIMG_BUF pDrawImg, UINT32 CurrFrameIdx);


#endif


