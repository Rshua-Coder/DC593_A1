#ifndef IQS_IE_SETTINGTABLE_IMX322LQJ_EVB_FF_H
#define IQS_IE_SETTINGTABLE_IMX322LQJ_EVB_FF_H
#include "IQS_SettingFlow.h"

#define IE_HDR_LEVEL 0

extern IQS_COLOR_EFFECT_PARAM ColorEffectTable[SEL_IMAGEEFFECT_MAX_CNT + 1];
extern IQS_IE_COLORPENCIL_PARAM IE_ColorPencil_Param[Total_ISONum];
extern IQS_IE_PENCILSKETCH_PARAM IE_PencilSketch_Param[Total_ISONum];
extern UINT32 IE_FISYEYE_LUT[65];
extern IQS_COLOR_EFFECT_PARAM ColorEffectTable[SEL_IMAGEEFFECT_MAX_CNT + 1];
extern IME_RETINEX IME_RetInex[1];
extern AvgFilt IFE2_AvgFilter_WDR[Total_ISONum];

#endif

