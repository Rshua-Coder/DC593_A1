/**
    GxSound module.

    For playing key sound.

    @file       GxSound.h
    @ingroup    mILIBGXSOUND
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _GXSOUND_H
#define _GXSOUND_H

#include "GxCommon.h"
//#include "Audio.h"
#include "pwm.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
    @addtogroup mILIBGXSOUND
*/
//@{


/**
     @name Operations on a sound
*/
//@{
#define SOUND_CTRL_STOP             0       //stop playing the current sound immediatly
#define SOUND_CTRL_PLAY             1       //play a sound
#define SOUND_CTRL_WAITSTOP         2       //stop playing the current sound and wait it finished
//@}


/**
     @name Maximum repeat-times for playing a sound
*/
//@{
#define SOUND_PLAY_MAX_TIMES        10
//@}


/**
     @name For Test only. Configure the output device.
*/
//@{
#define GXSND_TEST_FUNC             DISABLE //default is DISABLE
//@}

/**
     Attributes of the sound.

*/
typedef struct
{
    const UINT8 *puiData;   ///< points to the memory that a pcm sound data located
    UINT32      uiSize;     ///< how many bytes of the sound data
    //AUDIO_SR    sampleRate; ///< audio sample rate, ex: 44.1k, 32k, 48k,¡K.
    UINT32      sampleRate; ///< audio sample rate, ex: 44.1k, 32k, 48k,¡K.
    UINT32      isMono;     ///< mono or stereo
    UINT32      soundId;    ///< an unique Id for the sound, not its index in the sound table
} SOUND_DATA, *PSOUND_DATA;

/**
     Attributes Setting of the output devices.

     Configure the output devices of sound.
     @note: please reference the file,Audio.h, for the detail definitions of each element.
*/
#if 0
typedef struct
{
    AUDIO_CODECSEL      audCodec;   ///< default or hdmi codec
    AUDIO_OUTPUT        audOutType; ///< outout device
    AUDIO_CH            audChannel; ///< audio channel
    BOOL                monoExpand; ///< expand single channel to two channels or not
}GXSND_AUD_OUTDEV, *PGXSND_AUD_OUTDEV;
#endif
/**
     To-Do operations on a sound.

*/
typedef enum
{
    SOUND_PLAY_START = 0,       ///< start to play a sound
    SOUND_PLAY_STOP,            ///< stop playing a sound
    SOUND_PLAY_PAUSE,           ///< pause playing a sound
    SOUND_PLAY_RESUME,          ///< resume playing a sound
    SOUND_PLAY_MAX
}SOUND_PLAY_EVENT;

/**
     A callback event signals what is going.

     @note: Internal operations on a sound.
*/
typedef enum
{
    SOUND_CB_START = 0,
    SOUND_CB_STOP,
    SOUND_CB_OUTDEVCHG,
    SOUND_CB_MAX
}SOUND_CB_EVENT_TYPE;

/**
     PlaySound callback event function prototype.

*/
typedef void (*FPSOUNDCB)(UINT32 event, UINT32 param1, UINT32 param2);

/**
     Register a callback function.

     sets the callback function for those sound events,SOUND_CB_EVENT_TYPE.

     @param[in] fpSoundCB       PlaySound callback event function.
     @return    None.
*/
extern void GxSound_RegCB(FPSOUNDCB fpSoundCB);

/**
     Plays a sound, wav-pcm format, which is indicated by "pSndData".

     It is usually used to play a sound which is not in the registered sound table.

     @param[in] sndEvt     Start,stop,pause,or resume play a sound.
                           Please reference the type "SOUND_PLAY_EVENT".
     @param[in] pSndData   Point to the memory that the sound is located.
                           Please reference the structure,"SOUND_DATA".
     @param[in] isPCM      sound data is PCM-Only or Wav.

     @return
         - @b E_OK:    Ok
         - @b E_PAR:   Fail
*/
extern ER GxSound_ActOnSndNotInTbl(SOUND_PLAY_EVENT sndEvt,SOUND_DATA *pSndData, UINT32 isPCM);

/**
     Registers all key sounds in the format of "SOUND_DATA".

     @param[in] uiCount     How many elements in the sound table, "pSoundTable".
     @param[in] pSoundTable Points to an array in the structure of "SOUND_DATA".
                            Please reference the structure,"SOUND_DATA".

     @return
         - @b E_OK:    Ok
         - @b E_PAR:   Fail
*/
extern ER   GxSound_SetSoundTable(UINT32 uiCount, SOUND_DATA *pSoundTable);

/**
     Skip or keep playing a sound.
     @note It is not mute or close the audio driver.

     @param[in] bEn         TRUE=Play, FALSE=Not play
     @return    None.
*/
extern void GxSound_EnableSound(BOOL bEn);

/**
     Whether the sound-playing function is skipped or kept.

     @return
         - @b TRUE:    Keep
         - @b FALSE:   Skip
*/
extern BOOL GxSound_IsSoundEnabled(void);

/**
     Get current-set volume.

     @return
         - @b AUDIO_VOL   Volume level. Please reference the structure in the file, Audio.h.
*/
//extern AUDIO_VOL GxSound_GetVolume(void);

/**
     Get current-set volume.

     @return
         - @b UINT32   Volume level. 0 ~ 100
*/
extern UINT32 GxSound_GetVolume(void);


/**
     Set volume.
     Set volume for the next playing sound.

     @param[in] vol         Volume level. Please reference the structure in the file, Audio.h..

     @return    None
*/
//extern void GxSound_SetVolume(AUDIO_VOL vol);

/**
     Set the audio volume

     @param[in] vol   0 ~ 100.


     @return    None
*/
void GxSound_SetVolume(UINT32 vol);



/**
     Open the key-sound playing module.

     @return
         - @b E_OK:    Ok
         - @b E_SYS:   Fail
*/
extern ER GxSound_Open(void);

/**
     Close the key-sound playing module.

     @return
         - @b E_OK:    Ok
         - @b E_SYS:   Fail
*/
extern ER GxSound_Close(void);

/**
     Set the audio volume¡Bcodec¡Band features.

     @return    None
*/
extern ER GxSound_InitAudio(void);

#if SOUND_PLAY_MAX_TIMES
/**
     Play the sound with the "index" in registered sound table by the function "GxSound_SetSoundTable".

     @param[in] index       index in the registered sound table.
     @param[in] times       repeat times

     @return    None
*/
extern void GxSound_Play_Repeat(int index, UINT32 times);
#endif

/**
     Play the sound once with the "index" in registered sound table by the function "GxSound_SetSoundTable".

     @param[in] index       index in the registered sound table.

     @return    None
*/
extern void GxSound_Play(int index);

/**
     Stop playing the current sound immediately.

     @return    None
*/
extern void GxSound_Stop(void);

/**
     Stop playing the current sound but wait it finished.

     @return    None
*/
extern void GxSound_WaitStop(void);

/**
     Stop playing the current tone.

     @return    None
*/
extern void GxSound_PlayToneStop(void);

/**
     Play a tone.
     Play a tone that is specified by the parameters.

     @param[in] uiPWMId     PWM Id.
     @param[in] playTime    no use now
     @param[in] pPWMInfo    pwm related setting. Please reference it in the file, Pwm.h.

     @return    None
*/
extern void GxSound_PlayTone(UINT32 uiPWMId, UINT32 playTime, PWM_CFG *pPWMInfo);

/**
     Register a set of output devices.

     @param[in] pAudOutDev     An array of output deivces in the structure of "GXSND_AUD_OUTDEV".
     @param[in] tblElementCnt  Number of elements in the out-device table, "pAudOutDev".


     @return
         - @b E_OK:    Ok
         - @b E_PAR:   Fail
*/
//UINT32 GxSound_ConfigAudOutDev(PGXSND_AUD_OUTDEV pAudOutDev, UINT32 tblElementCnt);

/**
     Set the configuration index of play-sound audio device

     @param[in] outDevConfigIdx   index of audio configuration device.


     @return    None
*/
void GxSound_SetOutDevConfigIdx(UINT32 outDevConfigIdx);


/**
     Choose an output device in the registered output-device arrary.

     @param[in] tblIdx         Index of the set output device.


     @return
         - @b E_OK:    Ok
         - @b E_PAR:   Fail
*/
//UINT32 GxSound_SetAudOutDev(UINT32 tblIdx);


/**
     Install PlaySound task, flag and semaphore id

     This is a default setting API. Users can create their own task id, semphore id,
     flag id to adjust own stack size and task priority as the following:

     @code
     {
        #define PRI_PLAYSOUND            4
        #define STKSIZE_PLAYSOUND        1024

        UINT32 PLAYSOUNDTSK_ID = 0;
        UINT32 FLG_ID_SOUND = 0;

        void GxSound_InstallID(void)
        {
            OS_CONFIG_TASK(PLAYSOUNDTSK_ID,   PRI_PLAYSOUND,     STKSIZE_PLAYSOUND,      PlaySoundTsk);
            OS_CONFIG_FLAG(FLG_ID_SOUND);
        }

     }
     @endcode
*/
extern void GxSound_InstallID(void) _SECTION(".kercfg_text");

/**
     Backward compatible only.

*/
extern BOOL Sound_OnCommand(CHAR *pcCmdStr);

/**
     Backward compatible only.

*/
extern void Sound_OnSystem(int cmd);


#if (GXSND_TEST_FUNC == ENABLE)
/**
     Output device setrting.

     @note: only used for testing.

*/
typedef enum
{
    SOUND_OUTPUT_SPK = 0,   ///< speaker
    SOUND_OUTPUT_HP,        ///< head phone
    SOUND_OUTPUT_TV_LINE,   ///< line out
    SOUND_OUTPUT_HDMI,      ///< HDMI
    ENUM_DUMMY4WORD(SOUND_OUTPUT)
}SOUND_OUTPUT;

/**
     Set or Get channel-expand setting.

     @note: only used for testing.

*/
typedef enum
{
    SOUND_ACT_GET = 0,
    SOUND_ACT_SET,
    SOUND_ACT_MAX
}SOUND_ACT;

extern void GxSound_SetOutput(SOUND_OUTPUT audioOutDev);
extern void GxSoud_DetSetAudioOutDevice(void);
extern UINT32 GxSound_ActChannelExpand(SOUND_ACT act, UINT32 TorF);

#endif

/**
     Enable triggering PlaySoundTsk repeat playing sound.
     Enable repeat playing sound for codec which don't repeat playing buffer when DMA done.

     @param[in] bEn       enable.
     @return    None.
*/
extern void GxSound_EnableRepeatTrigger(BOOL bEn);

//@}


#ifdef __cplusplus
} //extern "C"
#endif

#endif //_GXSOUND_H

