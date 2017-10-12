/**
    Header file for Audio module Extension Features

    This file is the header file that define the API and data type
    for Audio module Extension Features

    @file       AudExt.h
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

#ifndef _AUDEXT_H
#define _AUDEXT_H

#include "Audio.h"
#include "Type.h"


//
// Exporting useful APIs
//
extern ER               audext_open(PAUDIO_DEVICE_OBJ pDevObj);
extern ER               audext_close(void);
extern BOOL             audext_getLockStatus(void);
extern void             audext_getDeviceObject(PAUDIO_DEVICE_OBJ pDevObj);
extern BOOL             audext_isBusy(void);

extern void             audext_setOutput(AUDIO_OUTPUT Output);
extern void             audext_setSamplingRate(AUDIO_SR SamplingRate);
extern void             audext_setChannel(AUDIO_CH Channel);
extern void             audext_setTotalVolLevel(AUDIO_VOL_LEVEL Audio_VolLevel);
extern AUDIO_VOL_LEVEL  audext_getTotalVolLevel(void);
extern void             audext_setVolume(AUDIO_VOL Vol);
extern AUDIO_VOL        audext_getVolume(void);

extern ER               audext_playback(BOOL bTCLatchEvent, BOOL bTCHitEvent);
extern void             audext_pause(void);
extern void             audext_resume(void);
extern void             audext_stop(void);
extern BOOL             audext_setResampleInfo(PAUDIO_RESAMPLE_INFO pResampleInfo);

extern void             audext_resetBufferQueue(void);
extern BOOL             audext_addBufferToQueue(PAUDIO_BUF_QUEUE pAudioBufQueue);
extern BOOL             audext_isBufferQueueFull(void);
extern PAUDIO_BUF_QUEUE audext_getDoneBufferFromQueue(void);
extern PAUDIO_BUF_QUEUE audext_getCurrentBufferFromQueue(void);

//
// Exporting useless APIs (for API prototype compatible usage)
//
extern void             audext_setFeature(AUDIO_FEATURE Feature, BOOL bEnable);
extern void             audext_setParameter(AUDIO_PARAMETER Parameter, UINT32 uiSetting);
extern void             audext_setTimecodeTrigger(UINT32 uiTrigger);
extern void             audext_setTimecodeOffset(UINT32 uiOffset);
extern UINT32           audext_getTimecodeValue(void);
extern BOOL             audext_isPlaybackMode(void);

#endif
