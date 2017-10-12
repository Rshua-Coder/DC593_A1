#ifndef UI_ANIMATION_H
#define UI_ANIMATION_H

#include "Type.h"
#include "UIFramework.h"
#include "UIGraphics.h"


typedef void (*GetCurPosCB)(UINT32);

typedef struct _UIAnimation
{
    IPOINT          startPoint;         // Animation start position
    IPOINT          endPoint;           // Animation end position
    Ux_RECT         curPos;             // Animation current drawing position
    UINT32          *pIconPool;         // Slide icons
    UINT32          uiTotalIconNum;     // Total number of slide icons to show
    UINT32          uiIntervalTime;     // Interval time between each slide icon
    UINT32          uiCurIndex;         // Current slide icon index for drawing
    VControl        *pCurStaticCtrl;    // One given area used to draw the next slide icon (position shift every interval time)
    VControl        *pLastStaticCtrl;   // One given area used to hide the last slide icon (position shift every interval time)
    GetCurPosCB     pGetCurPosCB;      // user can modify animation move algrithm
} UIAnimation;

extern VControl Animation1Ctrl;
extern VControl Animation2Ctrl;

//APIs
extern void AnimationStart(UIAnimation *pAnimation);
extern void AnimationStop(void);
extern void AnimationGetCurPos(UIAnimation *pAnimation);

#endif
