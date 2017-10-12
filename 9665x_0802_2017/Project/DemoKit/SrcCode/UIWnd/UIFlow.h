/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIFlow.h
    @ingroup    mIPRJAPUIFlow

    @brief      UI Flow Functions
                This file is the user interface ( for interchange flow control).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef _UIFLOW_H
#define _UIFLOW_H

#include "SysCfg.h"
//UIApp
#include "SysCommon.h"
#include "AppCommon.h"
#include "UIAppMovie.h"
#if 0
#include "UIBackgroundObj.h"
#include "UIPhotoObj.h"
#include "UIMovieObj.h"
#include "UIPlayObj.h"
#include "UISystemObj.h"
#include "UIPrintObj.h"
#include "UIPlayStamp.h"
#include "UILensObj.h"
#endif

#if (UI_STYLE == UI_STYLE_DRIVER)
    #include "UIFlow_DRIVE.h"
#endif

//UI Display
#include "UIGraphics.h"
#include "UIResource.h"

#include "platform.h"

#include "Debug.h"

#include "DCF.h"

//#NT#2012/8/14#philex -begin
// add the following include files temp,
#include "FlowPanInit.h"
#include "FlowPanCap.h"
#include "MenuDatetime.h"
#include "FlowPlay.h"
#include "FlowCommon.h"
#include "DialogWarn.h"
#include "FlowPhotoParams.h"
#include "UIFlowWndPhoto.h"

//#NT#2012/8/14#philex -end


#define TIMER_HALF_SEC               500
#define TIMER_ONE_SEC               1000
#define TIMER_TWO_SEC               2000


// Hideo test: general macros
#define MAKE_WORD(l, h)     ((UINT16)(((UINT8)(l)) | ((UINT16)((UINT8)(h))) << 8))
#define MAKE_LONG(l, h)     ((UINT32)(((UINT16)(l)) | ((UINT32)((UINT16)(h))) << 16))
#define LO_LONG(x)          ((UINT32)(x))
#define HI_LONG(x)          ((UINT32)((x>>32) & ((1ULL<<32) - 1)))
#define LO_WORD(l)          ((UINT16)(l))
#define HI_WORD(l)          ((UINT16)(((UINT32)(l) >> 16) & 0xFFFF))
#define LO_BYTE(w)          ((UINT8)(w))
#define HI_BYTE(w)          ((UINT8)(((UINT16)(w) >> 8) & 0xFF))
#define BIT(b, x)           ( ((1 << (b)) & (x)) >> (b) )
#define BITS(s, e, x)       ( (((((1 << ((e)-(s) + 1)) - 1) << (s)) & (x)) >> (s)) )
#define MAKE_EVEN(x)        ((x) & ~1)
#define MAKE_QUAD(x)        ((x) & ~3)

#define LIMIT(var,min,max)  (var) = ((var) < (min)) ? (min) : \
                                    (((var) > (max)) ? (max) : (var))

#define LIMITR(var,min,max) (var) = ((var) > (max)) ? (min) : \
                                    (((var) < (min)) ? (max) : (var))

#define SWAP_BYTES(x)       (MAKE_WORD( HI_BYTE(x), LO_BYTE(x)))

// Swaps words and bytes.
// For example, SWAP_WORDS(0x33221100) = 0x00112233
#define SWAP_WORDS(x)       (MAKE_LONG( MAKE_WORD( HI_BYTE(HI_WORD(x)),   \
                                                   LO_BYTE(HI_WORD(x))),  \
                                        MAKE_WORD( HI_BYTE(LO_WORD(x)),   \
                                                   LO_BYTE(LO_WORD(x)))))

#define ELEMS_OF_ARRAY(x)   (sizeof(x) / sizeof(x[0]))

#endif
