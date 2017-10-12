#ifndef IQS_SETTINGFLOW_NT99231_EVB_FF_INT_H
#define IQS_SETTINGFLOW_NT99231_EVB_FF_INT_H

#include "IQS_SettingFlow.h"

UINT32 IQS_GetIFE2RetinexTime(IQS_FLOW_MSG MsgID);
UINT32 IQS_GetIFE2LcaTime(IQS_FLOW_MSG MsgID);
UINT32 IQS_GetRetinexMinRatio(void);
UINT32 IQS_GetChromaAdaptMinRatio(void);
UINT32 IQS_PRV_IsChromaAdaptEn(void);

UINT32 IQS_CAPPRI_IsChromaAdaptEn(void);
UINT32 IQS_CAPPRI_IsRetinexEn(void);
UINT32 IQS_CAPQV_IsChromaAdaptEn(void);
UINT32 IQS_CAPQV_IsRetinexEn(void);
UINT32 IQS_CAPPRI_GetLCARefImgRatio(void);
UINT32 IQS_CAPQV_GetLCARefImgRatio(void);
UINT32 IQS_CAPPRI_GetRetinexRefImgRatio(void);
UINT32 IQS_CAPQV_GetRetinexRefImgRatio(void);
extern void IQS_GetBinningFactor(UINT32 *PrvBin, UINT32 *CapBin);
extern UINT32 IfeBinning;
extern void IQS_SetContrast(UINT32 value);

#endif //SENSOR_FLOW_FUNC_H


