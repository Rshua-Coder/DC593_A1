#ifndef _PIPVIEW_H
#define _PIPVIEW_H

#include "DispSrvApi.h"

typedef enum _PIP_SRC_RATIO{
    PIP_SRC_RATIO_4_3,
    PIP_SRC_RATIO_16_9,
    PIP_SRC_RATIO_3_2,
    PIP_SRC_RATIO_MAX_CNT,
    ENUM_DUMMY4WORD(PIP_SRC_RATIO)
}PIP_SRC_RATIO;

typedef enum _PIP_DST_RATIO{
    PIP_DST_RATIO_4_3,
    PIP_DST_RATIO_16_9,
    PIP_DST_RATIO_MAX_CNT,
    ENUM_DUMMY4WORD(PIP_DST_RATIO)
}PIP_DST_RATIO;

typedef enum _PIP_STYLE{
    PIP_STYLE_1T1F,   //Only Path 1 fully on Top
    PIP_STYLE_1T1B2S, //IPL Path 1 on Top, Path 1 Big Screen, Path 2 Small Screen
    PIP_STYLE_1T1S2B, //IPL Path 1 on Top, Path 1 Small Screen, Path 2 Big Screen
    PIP_STYLE_2T2F,   //Only Path 2 fully on Top
    PIP_STYLE_2T1B2S, //IPL Path 2 on Top, Path 1 Big Screen, Path 2 Small Screen
    PIP_STYLE_2T1S2B, //IPL Path 2 on Top, Path 1 Small Screen, Path 2 Big Screen
    ENUM_DUMMY4WORD(PIP_STYLE)
}PIP_STYLE;

void PipView_SetStyle(PIP_STYLE PipStyle);
void PipView_SetPrimaryImgRatio(USIZE* pSize);
void PipView_ActivePrimaryImgRatio(void);
INT32 PipView_OnDraw(DISPSRV_DRAW* pDraw); //PIP = Picture In Picture
void PipView_InstallCmd(void);
void PipView_Get_OnDrawBuf(IMG_BUF **pDstPIP); // For Wi-Fi mode use.
#endif