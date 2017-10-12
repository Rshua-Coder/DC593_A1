/**
    Animation of source rectangle scaling to destination.

    A plug-in for DispSrv, an animation. from a rectangle to another rectangle.

    @file       RectZoomApi.h
    @ingroup    mRECTZOOM

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _RECTZOOMAPI_H
#define _RECTZOOMAPI_H
#include "GxImage.h"
#include "DispSrvApi.h"

/**
    @addtogroup mRECTZOOM
*/
//@{

/**
     RectZoom inital structure.

     configure of animation behavior.
*/
typedef struct _RECTZOOM_INIT{
    URECT       rcFrom; ///< first frame will begin to scaled source image on rcFrom
    URECT       rcTo;   ///< last frame will end to scaled source image on rcTo
    UINT32      uiSteps; ///< the total counts of animation frame
    void        (*OnImageClean)(IMG_BUF* pFb); //!< (Optional) after each image clean we callback for user clean. it can be NULL
}RECTZOOM_INIT,*PRECTZOOM_INIT;

/**
     RectZoom Initial

     Before using plug RectZoom_OnDraw() into DispSrv, this moudle have to be initial first.

     @param[in] pInit      refer to RECTZOOM_INIT
     @return Description of data returned.
         - @b TRUE:   initial success.
         - @b FALSE:  failed to initial.
*/
BOOL RectZoom_Init(RECTZOOM_INIT* pInit);

/**
     RectZoom OnDraw callback.

     This module provides a OnDraw callback for plug into DispSrv.

     @param[in] pDraw      refer to DISPSRV_DRAW.
     @return Description of data returned.
         - @b E_OK:     execute success.
         - @b OTHERS:   failed to execute.
*/
INT32 RectZoom_OnDraw(DISPSRV_DRAW* pDraw);

//@}
#endif
