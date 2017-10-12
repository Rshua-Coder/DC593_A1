#ifndef _DXSOUND_H_
#define _DXSOUND_H_

#include "type.h"
#include "Audio.h"


///////////////////////////////////////////////////////////////////////////////
// Audio
///////////////////////////////////////////////////////////////////////////////
extern AUDIO_OUTPUT GPIOMap_DetAudioOutDevice(void);
extern void GPIOMap_TurnOnExtrAmp(BOOL bEn);
extern void DrvSound_SetAudioDevInitData(AUDIO_DEVICE_OBJ  *pAudioDevice, AUDIO_SETTING *pAudioSetting);
extern void DrvSound_SetAudioFeature(void);


extern ER   DrvSound_SetOutDevConfig(UINT32 outDevIdx);
extern ER   DrvSound_SetVol(UINT32 vol);// 0 ~ 100
extern ER   DrvSound_SetSR(UINT32 audSR);
extern ER   DrvSound_SetCh(AUDIO_CH audCh, UINT32 monoExpand);
extern UINT32 DrvSound_GetVol(void);
extern UINT32 DrvSound_GetHDMIAudSR(void);
extern ER   DrvSound_SetAudGain(UINT32 vol);
extern ER   DrvSound_SetSR_Resample(UINT32 audSR, UINT32 uiWorkBufAddr, UINT32 uiWorkBufSize);
extern ER   DrvSound_SetAmplifer(UINT32 enable);

#endif //_DXSOUND_H_

