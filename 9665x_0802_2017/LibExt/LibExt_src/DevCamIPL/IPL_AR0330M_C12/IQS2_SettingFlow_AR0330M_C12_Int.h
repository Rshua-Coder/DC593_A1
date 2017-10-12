#ifndef IQS2_SETTINGFLOW_AR0330M_C12_INT_H
#define IQS2_SETTINGFLOW_AR0330M_C12_INT_H
#include "IPL_AR0330M_C12_int.h"
#if (IQS_VERSION == IQS_VERSION_2)
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
UINT32 IQS_AddGammaTab(UINT32 Data12Bit);
UINT32 IQS_InvGammaTab(UINT32 Data10Bit);
UINT32 IQS_SetIFEGain(IPL_PROC_ID Id, IFE_SUBFUNC *ptr);
#endif
#endif //SENSOR_FLOW_FUNC_H


