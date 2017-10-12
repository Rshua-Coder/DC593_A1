
//XXXXXXXXXXXXXXX 2009/06/11 Lily Kao -begin
#include "DxCfg.h"
#include "IOCfg.h"

#include "Debug.h"
#include "Audio.h"
#include "eac.h"


//#NT#2013/03/06#Lily Kao -begin
//#NT#
#include "DxSound.h"
#include "mIType.h"
#include "ErrorNo.h"
#include "Audio.h"
//#NT#2013/06/14#Calvin Chang#Get HDMI sample rate -begin
#include "hdmitx.h"
//#NT#2013/06/14#Calvin Chang -end

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define DRVSOUND_OUTDEV_CONFIG_CNT          5
#define DRVSOUND_MAX_VOL_LVL                100

typedef enum
{
    DRVSOUND_VOUT_LCD,
    DRVSOUND_VOUT_TV,
    DRVSOUND_VOUT_HDMI,
    DRVSOUND_VOUT_LCDTV,
    DRVSOUND_VOUT_LCDHDMI,
    ENUM_DUMMY4WORD(DRVSOUND_VOUT)
}DRVSOUND_VOUT;

typedef struct
{
    DRVSOUND_VOUT       vidOutType;
    AUDIO_CH            audChannel;
    BOOL                monoExpand;
}DRVSOUND_OUTDEV_CONFIG;

typedef enum
{
    DRVSOUND_SR_8000   = 8000,     ///< 8 KHz
    DRVSOUND_SR_11025  = 11025,    ///< 11.025 KHz
    DRVSOUND_SR_12000  = 12000,    ///< 12 KHz
    DRVSOUND_SR_16000  = 16000,    ///< 16 KHz
    DRVSOUND_SR_22050  = 22050,    ///< 22.05 KHz
    DRVSOUND_SR_24000  = 24000,    ///< 24 KHz
    DRVSOUND_SR_32000  = 32000,    ///< 32 KHz
    DRVSOUND_SR_44100  = 44100,    ///< 44.1 KHz
    DRVSOUND_SR_48000  = 48000,    ///< 48 KHz
    ENUM_DUMMY4WORD(DRVSOUND_SR)
}DRVSOUND_SR;

DRVSOUND_OUTDEV_CONFIG gDxSndAudOutDevConfigTbl[DRVSOUND_OUTDEV_CONFIG_CNT]=
{
    {DRVSOUND_VOUT_LCD , AUDIO_CH_LEFT, TRUE},
    {DRVSOUND_VOUT_TV, AUDIO_CH_LEFT, TRUE},
    {DRVSOUND_VOUT_HDMI, AUDIO_CH_LEFT, FALSE},
    {DRVSOUND_VOUT_LCDTV , AUDIO_CH_LEFT, TRUE},
    {DRVSOUND_VOUT_LCDHDMI , AUDIO_CH_LEFT, TRUE},
};

DRVSOUND_OUTDEV_CONFIG *gpCurDrvAudOutDev = &gDxSndAudOutDevConfigTbl[0];
UINT32 gCurDrvSndVol = 0xFFFFFFFF;
AUDIO_DEVICE_OBJ    gDrvSoundAudDev = {0};


//========== Debug Messsage =======
#ifdef NDEBUG  //release version
#define _DRVSOUND_DBG_                       0
#else
#define _DRVSOUND_DBG_                       3
#endif

#if (_DRVSOUND_DBG_ > 2)
#define DbgMsg_DrvSoundIO(msg)         DBG_IND msg
#define DbgMsg_DrvSound(msg)           DBG_IND msg
#define DbgCode_DrvSound(x)            x

_INLINE void DrvSound_DmpOutDevConfig(void)
{
    DBG_IND("\r\n");
    DBG_IND("audCodec= %d \r\n",  gpCurDrvAudOutDev->vidOutType);
    DBG_IND("audChannel= %d \r\n",gpCurDrvAudOutDev->audChannel);
    DBG_IND("monoExpand= %d \r\n",gpCurDrvAudOutDev->monoExpand);
}

#else

#define DbgMsg_DrvSoundIO(msg)
#define DbgMsg_DrvSound(msg)
#define DbgCode_DrvSound(x)

_INLINE void DrvSound_DmpOutDevConfig(void){}

#endif


ER DrvSound_SetOutDevConfig(UINT32 outDevIdx)
{
    ER retV = E_OK;
    UINT32 openAud = FALSE;
    DbgMsg_DrvSoundIO(("+%s:idx=%d\r\n",__func__,outDevIdx));

    if (outDevIdx >= DRVSOUND_OUTDEV_CONFIG_CNT)
    {
        DBG_ERR("%s:idx=%d\r\n",__func__,outDevIdx);
        return retV;
    }
    gpCurDrvAudOutDev = &gDxSndAudOutDevConfigTbl[outDevIdx];
    DrvSound_DmpOutDevConfig();
    if (aud_getLockStatus() == NO_TASK_LOCKED)
    {
        aud_getDeviceObject(&gDrvSoundAudDev);
        retV = aud_open(&gDrvSoundAudDev);
        if (retV != E_OK)
        {
            DBG_ERR("%s: Aud open fail\r\n",__func__);
            return retV;
        }
        openAud = TRUE;
    }

    //[Config audio Codec & Output]
    //Speaker is connected to IC Speaker Output
    //TV Audio is connected to IC HP_R Output
    if(gpCurDrvAudOutDev->vidOutType == DRVSOUND_VOUT_LCD)
    {
        DBG_IND("video: device=LCD\r\n");
        DBG_IND("audio: codec=DEFAULT device=SPK\r\n");
        aud_switchCodec(AUDIO_CODECSEL_DEFAULT);
        aud_setOutput(AUDIO_OUTPUT_SPK); //or AUDIO_OUTPUT_MONO
        aud_setFeature(AUDIO_FEATURE_LINE_PWR_ALWAYSON, FALSE);
        aud_setFeature(AUDIO_FEATURE_INTERFACE_ALWAYS_ACTIVE, FALSE);
        aud_setFeature(AUDIO_FEATURE_DISCONNECT_HDMI, TRUE);
        aud_setFeature(AUDIO_FEATURE_PLAY2ALLCODEC, FALSE);
    }
    else
    if(gpCurDrvAudOutDev->vidOutType == DRVSOUND_VOUT_TV)
    {
        DBG_IND("video: device=TV\r\n");
        DBG_IND("audio: codec=DEFAULT device=LINE\r\n");
        aud_switchCodec(AUDIO_CODECSEL_DEFAULT);
        aud_setOutput(AUDIO_OUTPUT_LINE); //or AUDIO_OUTPUT_HP
        aud_setFeature(AUDIO_FEATURE_LINE_PWR_ALWAYSON, TRUE);
        aud_setFeature(AUDIO_FEATURE_INTERFACE_ALWAYS_ACTIVE, FALSE);
        aud_setFeature(AUDIO_FEATURE_DISCONNECT_HDMI, TRUE);
        aud_setFeature(AUDIO_FEATURE_PLAY2ALLCODEC, FALSE);
    }
    else
    if(gpCurDrvAudOutDev->vidOutType == DRVSOUND_VOUT_HDMI)
    {
        DBG_IND("video: device=HDMI\r\n");
        DBG_IND("audio => codec=HDMI device=NONE\r\n");
        aud_switchCodec(AUDIO_CODECSEL_HDMI);
        aud_setOutput(AUDIO_OUTPUT_NONE);
        aud_setFeature(AUDIO_FEATURE_LINE_PWR_ALWAYSON, FALSE);
        aud_setFeature(AUDIO_FEATURE_INTERFACE_ALWAYS_ACTIVE, TRUE);
        aud_setFeature(AUDIO_FEATURE_DISCONNECT_HDMI,  FALSE);
        aud_setFeature(AUDIO_FEATURE_PLAY2ALLCODEC, FALSE);
        // 後續 aud_setSamplingRate() 設定的 sampling rate 須等同 HDMI display object 端設定的 sampling rate
        // 不然需要用 aud_setResampleInfo() 啟動 audio resample
    }
    else
    if(gpCurDrvAudOutDev->vidOutType == DRVSOUND_VOUT_LCDTV)
    {
        DBG_IND("video: device=LCD & TV\r\n");
        DBG_IND("audio: codec=DEFAULT device=SPK\r\n");
        aud_switchCodec(AUDIO_CODECSEL_DEFAULT);
        aud_setOutput(AUDIO_OUTPUT_LINE);
        aud_setFeature(AUDIO_FEATURE_LINE_PWR_ALWAYSON, TRUE);
        aud_setFeature(AUDIO_FEATURE_INTERFACE_ALWAYS_ACTIVE, FALSE);
        aud_setFeature(AUDIO_FEATURE_DISCONNECT_HDMI, TRUE);
        aud_setFeature(AUDIO_FEATURE_PLAY2ALLCODEC, FALSE);
    }
    else
    if(gpCurDrvAudOutDev->vidOutType == DRVSOUND_VOUT_LCDHDMI)
    {
        DBG_IND("video: device=LCD & HDMI\r\n");
        DBG_IND("audio: codec=DEFAULT device=SPK\r\n");
        aud_switchCodec(AUDIO_CODECSEL_DEFAULT);
        aud_setOutput(AUDIO_OUTPUT_SPK);
        aud_setFeature(AUDIO_FEATURE_LINE_PWR_ALWAYSON, FALSE);
        aud_setFeature(AUDIO_FEATURE_INTERFACE_ALWAYS_ACTIVE, TRUE);
        aud_setFeature(AUDIO_FEATURE_DISCONNECT_HDMI, FALSE);
        aud_setFeature(AUDIO_FEATURE_PLAY2ALLCODEC, TRUE);
        // 後續 aud_setSamplingRate() 設定的 sampling rate 須等同 HDMI display object 端設定的 sampling rate
        // 不然需要用 aud_setResampleInfo() 啟動 audio resample
    }

    //[Config audio Channel]
    //TODO ==> GxSound:
    //  project can assign witch channel is for MONO sound play
    //  project can assign if expend MONO sound play as STEREO
    if (AUDIO_CH_STEREO == gpCurDrvAudOutDev->audChannel)
    {
        aud_setChannel(AUDIO_CH_STEREO);
        aud_setFeature(AUDIO_FEATURE_PLAYBACK_PCM_EXPAND, FALSE);
    }
    else
    {
        aud_setChannel(gpCurDrvAudOutDev->audChannel);
        aud_setFeature(AUDIO_FEATURE_PLAYBACK_PCM_EXPAND, gpCurDrvAudOutDev->monoExpand);
    }
    if (openAud == TRUE)
    {
        aud_close();
    }

    DbgMsg_DrvSoundIO(("-%s:idx=%d,audOpen=%d\r\n",__func__,outDevIdx,openAud));
    return retV;
}
UINT32 DrvSound_GetVol(void)
{
    return gCurDrvSndVol;
}
// vol: 0 ~ 100
ER DrvSound_SetVol(UINT32 vol)
{
    ER retV = E_OK;
    AUDIO_VOL_LEVEL maxVolLvl = 0;
    UINT32 tmpVol = 0;
    UINT32 openAud = 0;

    DbgMsg_DrvSoundIO(("+%s:vol=%d\r\n",__func__,vol));
    DbgCode_DrvSound(if (aud_getLockStatus() == NO_TASK_LOCKED) DBG_ERR("%s: AudDrv not open\r\n",__func__);)

    if (aud_getLockStatus() == NO_TASK_LOCKED)
    {
        aud_getDeviceObject(&gDrvSoundAudDev);
        aud_open(&gDrvSoundAudDev);
        openAud = TRUE;
    }
    gCurDrvSndVol = vol;
    if (vol == 0)
    {
        aud_setVolume(AUDIO_VOL_MUTE);
    }
    else
    {
        maxVolLvl = aud_getTotalVolLevel();
        tmpVol = (vol * maxVolLvl)/100;
        tmpVol = (tmpVol < maxVolLvl) ? (tmpVol + 1) : tmpVol;
        DbgMsg_DrvSoundIO(("%s:Cal=%d,max=%d,set=%d\r\n",__func__,gCurDrvSndVol,maxVolLvl,tmpVol));
        if (maxVolLvl == AUDIO_VOL_LEVEL8)
        {
            if (tmpVol >= AUDIO_VOL_LEVEL8)
            {
                tmpVol = AUDIO_VOL_7;
            }
        }
        else
        {
            if (tmpVol >= AUDIO_VOL_LEVEL64)
            {
                tmpVol = AUDIO_VOL_63;
            }
        }
        aud_setVolume(tmpVol);
    }
    if (openAud == TRUE)
    {
        aud_close();
    }
    DbgMsg_DrvSoundIO(("-%s:finalVol=%d,max=%d,set=%d,opened=%d\r\n",__func__,gCurDrvSndVol,maxVolLvl,tmpVol,openAud));
    return retV;
}

ER DrvSound_SetSR(UINT32 audSR)
{
    ER retV = E_OK;
    DbgMsg_DrvSoundIO(("+%s:sr=%d\r\n",__func__,audSR));


    DbgCode_DrvSound(if (aud_getLockStatus() == NO_TASK_LOCKED) DBG_ERR("%s: AudDrv not open\r\n",__func__);)
    if (DRVSOUND_SR_8000 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_8000);
    }
    else if (DRVSOUND_SR_11025 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_11025);
    }
    else if (DRVSOUND_SR_12000 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_12000);
    }
    else if (DRVSOUND_SR_16000 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_16000);
    }
    else if (DRVSOUND_SR_22050 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_22050);
    }
    else if (DRVSOUND_SR_24000 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_24000);
    }
    else if (DRVSOUND_SR_32000 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_32000);
    }
    else if (DRVSOUND_SR_44100 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_44100);
    }
    else
    {
        aud_setSamplingRate(AUDIO_SR_48000);
    }
    DbgMsg_DrvSoundIO(("-%s:sr=%d\r\n",__func__,audSR));
    return retV;
}
ER DrvSound_SetCh(AUDIO_CH audCh, UINT32 monoExpand)
{
    ER retV = E_OK;
    DbgMsg_DrvSoundIO(("+%s:ch=%d,isMono=%d\r\n",__func__,audCh, monoExpand));


    DbgCode_DrvSound(if (aud_getLockStatus() == NO_TASK_LOCKED) DBG_ERR("%s: AudDrv not open\r\n",__func__);)
    aud_setChannel(audCh);
    if (AUDIO_CH_STEREO == audCh)
    {
        aud_setChannel(AUDIO_CH_STEREO);
        aud_setFeature(AUDIO_FEATURE_PLAYBACK_PCM_EXPAND, FALSE);
    }
    else
    {
        aud_setChannel(audCh);
        aud_setFeature(AUDIO_FEATURE_PLAYBACK_PCM_EXPAND, monoExpand);
    }
    DbgMsg_DrvSoundIO(("-%s:ch=%d,isMono=%d\r\n",__func__,audCh, monoExpand));
    return retV;
}
//#NT#2013/03/06#Lily Kao -end

/**
  Detect Audio is plugging in or unplugged

  Detect Audio is plugging in or unplugged.
  Return TRUE if Audio is plugging in, FALSE if Audio is unplugged.

  @param void
  @return BOOL: TRUE -> Audio is plugging in, FALSE -> Audio is unplugged
*/
/*
BOOL GPIOMap_DetAudio(void)
{
    return FALSE;
}
*/

/**
  Detect which kind of Audio Output Device is plugging in or unplugged
  No Sample code for detecting now
  @param void
  @return AUDIO_OUTPUT
        AUDIO_OUTPUT_SPK,           ///< Speaker
        AUDIO_OUTPUT_HP,            ///< Headphone
        AUDIO_OUTPUT_LINE,          ///< Line Out
        AUDIO_OUTPUT_MONO           ///< Mono Out
*/
AUDIO_OUTPUT GPIOMap_DetAudioOutDevice(void)
{
    AUDIO_OUTPUT audiOut = AUDIO_OUTPUT_SPK;

    //detect audio output device ... here
    //XXXXXXXXXXXXXXX 2009/06/15 Lily Kao -begin
    //DBG_IND("GPIOMap_DetAudioOutDevice:audiOut=%d\r\n",audiOut);
    //XXXXXXXXXXXXXXX 2009/06/15 Lily Kao -end

    return audiOut;
}

//XXXXXXXXXXXXXXX 2009/06/12 Lily Kao -begin
/*
void GPIOMap_DetSetAudioOutDevice(void)
{
    AUDIO_OUTPUT audiOut = GPIOMap_DetAudioOutDevice();

    GxSoud_SetAudioOutDevice(audiOut);
    return;
}
*/
//XXXXXXXXXXXXXXX 2009/06/12 Lily Kao -end

/**
  Turn on External Amplifier

  If bEn == TRUE, Turn on External Amplifier
  If bEn == FALSE, Turn off External Amplifier(mute).

  @param BOOL bEn: TRUE -> turn on, FALSE -> turn off
  @return void
*/
void GPIOMap_TurnOnExtrAmp(BOOL bEn)
{
    DBG_IND("++GPIOMap_TurnOnExtrAmp:%x\r\n", bEn);
    if (bEn == TRUE)
    {
        //gpio_setPin(xxx);
    }
    else
    {
        //gpio_clearPin(xxx);
    }
    DBG_IND("--GPIOMap_TurnOnExtrAmp:%x\r\n", bEn);
}

//XXXXXXXXXXXXXXX 2009/06/11 Lily Kao -begin


//XXXXXXXXXXXXXXX 2009/06/16 Lily Kao -begin
#if( _AUDIOCODEC_EXT_ == _AUDIOCODEC_EXT_NONE_)//no external codec, use embeded codec
void DrvSound_SetAudioDevInitData(AUDIO_DEVICE_OBJ  *pAudioDevice, AUDIO_SETTING *pAudioSetting)
{
    DBG_IND("++DrvSound_SetAudioDevInitData\r\n");
    // Default ALC off, MIC BOOST on
    // Each task using Audio must set the corresponding EventHandler if desired
    pAudioDevice->pEventHandler       = NULL;
    // The following settings are "Don't care" for NT96432 built-in audio codec.
    pAudioDevice->uiGPIOColdReset     = 0;
    pAudioDevice->uiI2SCtrl           = AUDIO_I2SCTRL_NONE;
    pAudioDevice->uiChannel           = 0;
    pAudioDevice->uiGPIOData          = 0;
    pAudioDevice->uiGPIOClk           = 0;
    pAudioDevice->uiGPIOCS            = 0;
    pAudioDevice->uiADCZero           = 0;
//#NT#2010/03/16#Scottie -begin
//#NT#Add uiIfCloseAud to decide if close audio power when calling aud_stop
//#NT#2010/03/17#Scottie -begin
//#NT#Move to Audio driver (default 433: TRUE, 450: N/A)
//    pAudioDevice->uiIfCloseAud        = TRUE;
//#NT#2010/03/17#Scottie -end
//#NT#2010/03/16#Scottie -end

    // The following settings are for NT96432 built-in audio codec.
    // Don't modify the value that is "must be".
    // Set default sampling rate, channel, etc.
    pAudioSetting->Clock.bClkExt      = FALSE;                        // Must be
    pAudioSetting->Clock.Clk          = AUDIO_CLK_12288;              // Clock = Sampling rate * 256
    pAudioSetting->I2S.bMaster        = TRUE;                         // Must be
    pAudioSetting->I2S.I2SFmt         = AUDIO_I2SFMT_STANDARD;        // Must be
    pAudioSetting->I2S.ClkRatio       = AUDIO_I2SCLKR_256FS_32BIT;    // Must be
    pAudioSetting->Fmt                = AUDIO_FMT_I2S;                // Must be
    pAudioSetting->SamplingRate       = AUDIO_SR_48000;               // Sampling rate = Clock / 256
    pAudioSetting->Channel            = AUDIO_CH_RIGHT;               // Audio channel
    pAudioSetting->RecSrc             = AUDIO_RECSRC_MIC;             // Must be
    pAudioSetting->Output             = GPIOMap_DetAudioOutDevice(); //AUDIO_OUTPUT_SPK;
    pAudioSetting->bEmbedded          = TRUE;                         // Must be, if you want to use the audio I/O pin
    DBG_IND("--DrvSound_DefaultAUdioSettingData\r\n");
}
#elif( _AUDIOCODEC_EXT_ == _AUDIOCODEC_EXT_WM8978_)//init for WM8978
void DrvSound_SetAudioDevInitData(AUDIO_DEVICE_OBJ  *pAudioDevice, AUDIO_SETTING *pAudioSetting)
{
    AUDIO_CODEC_FUNC AudCodecFunc;
    DBG_IND("++DrvSound_SetAudioDevInitData\r\n");
    //static AUDIO_CODEC_FUNC gHdmiCodecFunc, gExtCodecFunc;
//    AUDIO_CODEC_FUNC    AudCodecFunc;

    pAudioDevice->pEventHandler       = NULL;
    // The following settings are "Don't care" for NT96650 built-in audio codec.
    pAudioDevice->uiGPIOColdReset     = 0;
    pAudioDevice->uiI2SCtrl           = AUDIO_I2SCTRL_GPIO_I2C;
//    pAudioDevice->uiI2SCtrl           = AUDIO_I2SCTRL_NONE;           // Don't care
    pAudioDevice->uiChannel           = 0;
    pAudioDevice->uiGPIOData          = P_GPIO_17;//P_GPIO_28;
    pAudioDevice->uiGPIOClk           = P_GPIO_18;//P_GPIO_29;
    pAudioDevice->uiGPIOCS            = 0;
    pAudioDevice->uiADCZero           = 0;
    pAudioSetting->Clock.bClkExt      = FALSE;                        // Must be
    pAudioSetting->I2S.bMaster        = TRUE;                         // Must be
    pAudioSetting->I2S.I2SFmt         = AUDIO_I2SFMT_STANDARD;        // Must be
    pAudioSetting->I2S.ClkRatio       = AUDIO_I2SCLKR_256FS_32BIT;    // Must be
    pAudioSetting->Fmt                = AUDIO_FMT_I2S;                // Must be
    // The following settings are for NT96650 built-in audio codec.
    // Don't modify the value that is "must be".
    // Set default sampling rate, channel, etc.
//    pAudioSetting->Clock.bClkExt      = FALSE;                        // Must be
    pAudioSetting->Clock.Clk          = AUDIO_CLK_12288;              // Clock = Sampling rate * 256
//    pAudioSetting->I2S.bMaster        = TRUE;                         // Must be
//    pAudioSetting->I2S.I2SFmt         = AUDIO_I2SFMT_LIKE_LSB;        // Must be
//    pAudioSetting->I2S.ClkRatio       = AUDIO_I2SCLKR_256FS_32BIT;    // Must be
//    pAudioSetting->Fmt                = AUDIO_FMT_I2S;                // Must be
    pAudioSetting->SamplingRate       = AUDIO_SR_48000;               // Sampling rate = Clock / 256
    pAudioSetting->Channel            = AUDIO_CH_RIGHT;               // Audio channel
    pAudioSetting->RecSrc             = AUDIO_RECSRC_MIC;             // Must be
    pAudioSetting->Output             = AUDIO_OUTPUT_SPK;             // Output source
    pAudioSetting->bEmbedded          = TRUE;                         // Must be. (Don't care in audio lib)

    audExtCodec_getFP(&AudCodecFunc);
    aud_setExtCodec(&AudCodecFunc);
    DBG_IND("--DrvSound_DefaultAUdioSettingData\r\n");
}
#else
void DrvSound_SetAudioDevInitData(AUDIO_DEVICE_OBJ  *pAudioDevice, AUDIO_SETTING *pAudioSetting)
{
    DBG_FATAL("AUDIO SETTING ERROR!\r\n");
}
#endif


void DrvSound_SetAudioFeature(void)
{
    DBG_IND("++DrvSound_SetAudioFeature\r\n");

    //
    // Note: These default audio recoding setting is good for reference board.
    //       For audio recording quaility fine-tuning request, please help to connect Novatek Engineer.
    // Enable ALC/Noise Gate function
    aud_setParameter(AUDIO_PARAMETER_NOISEGAIN,                                         0x1);
    aud_setParameter(AUDIO_PARAMETER_NOISEGATE_TIME_RESOLUTION, EAC_ALC_TRESO_BASIS_45000US);
    aud_setParameter(AUDIO_PARAMETER_ALC_TIME_RESOLUTION,        EAC_ALC_TRESO_BASIS_5000US);
    aud_setParameter(AUDIO_PARAMETER_ALC_ATTACK_TIME,                                   0x4);
    aud_setParameter(AUDIO_PARAMETER_ALC_DECAY_TIME,                                    0x5);

    //#NT#2013/6/6#Philex Lin-begin
    // for improving audio quality
    aud_setParameter(AUDIO_PARAMETER_BOOST_GAIN,                      EAC_PGABOOST_SEL_10DB);
    aud_setParameter(AUDIO_PARAMETER_ALC_MINGAIN,                   EAC_ALC_MINGAIN_P3P0_DB);

    aud_setFeature(AUDIO_FEATURE_ALC,                                                  TRUE);
    //#NT#2015/3/16#Philex Lin-begin
    // fixed 錄音背景音時大時小
    //aud_setFeature(AUDIO_FEATURE_NG_WITH_MICBOOST,                                     TRUE);
    aud_setFeature(AUDIO_FEATURE_NG_WITH_MICBOOST,                                     FALSE);
    aud_setParameter(AUDIO_PARAMETER_NOISETHD_WITH_BOOST,          EAC_NG_THRESHOLD_N58P5_DB);
    //#NT#2015/3/16#Philex Lin-end
    aud_setFeature(AUDIO_FEATURE_MICBOOST,                                             TRUE);
    //#NT#2013/6/6#Philex Lin-end

    /*
    // Disable MIC BOOST (If have Pre-Amp circuit)
    aud_setFeature(AUDIO_FEATURE_MICBOOST, FALSE);
    */

    // Set default Gain
    aud_setGain(AUDIO_GAIN_7);
    //#NT#2011/1/21#Jeah Yen -begin
    aud_setVolume(AUDIO_VOL_63);
    //#NT#2011/1/21#Jeah Yen -end

    DBG_IND("--DrvSound_SetAudioFeature\r\n");
}

//#NT#2013/06/14#Calvin Chang#Get HDMI sample rate -begin
UINT32 DrvSound_GetHDMIAudSR(void)
{
    UINT32        audsr;
    HDMI_AUDIOFMT hdmisr;

    hdmisr = hdmitx_getConfig(HDMI_CONFIG_ID_AUDIO);

    switch (hdmisr)
    {
        case HDMI_AUDIO32KHZ:
            audsr = 32000;
            break;
        case HDMI_AUDIO44_1KHZ:
            audsr = 44100;
            break;
        case HDMI_AUDIO48KHZ:
            audsr = 48000;
            break;
        default:
            DBG_ERR("hdmi sample is %d\r\n", hdmisr);
            audsr = 0;
            break;
    }

    return  audsr;

}
//#NT#2013/06/14#Calvin Chang -end

// vol: 0 ~ 100
ER DrvSound_SetAudGain(UINT32 vol)
{
    AUDIO_DEVICE_OBJ AudioDevObj;
    BOOL             bAudioALCEn = TRUE;
    UINT32           uAudGain    = AUDIO_GAIN_MUTE;
    UINT32           ustep       = 0;
    UINT32           upercgain   = 0;

    // Map the percentage of volume to Audio Gain
    switch (vol)
    {
        case 0: //0% (Mute)
        {
            bAudioALCEn = FALSE;
            uAudGain    = AUDIO_GAIN_MUTE;
        }
        break;

        case 100://100% (Maximum)
        {
            uAudGain = AUDIO_GAIN_7;
        }
        break;

        default:
        {
            ustep = (100 / (UINT32)AUDIO_GAIN_7); // AUDIO_GAIN_7 is maximum value

            upercgain = ustep;

            while (upercgain < vol)
            {
                upercgain += ustep;
                uAudGain++;
            }
        }
        break;
    }

    if (aud_getLockStatus() == TASK_LOCKED)
    {
        aud_setFeature(AUDIO_FEATURE_ALC, bAudioALCEn);
        aud_setGain(uAudGain);
    }
    else
    {
        aud_getDeviceObject(&AudioDevObj);
        aud_open(&AudioDevObj);
        aud_setFeature(AUDIO_FEATURE_ALC, bAudioALCEn);
        aud_setGain(uAudGain);
        aud_close();
    }

    return E_OK;

}

ER DrvSound_SetSR_Resample(UINT32 audSR, UINT32 uiWorkBufAddr, UINT32 uiWorkBufSize)
{
    ER                   retV = E_OK;
    AUDIO_RESAMPLE_INFO  ResampleInfo={0};
    BOOL                 bResampleEnable=FALSE;
    AUDIO_SR             uiTargetAudRate;
    HDMI_AUDIOFMT        uiHDMIAudRate;

    DbgMsg_DrvSoundIO(("+%s:sr=%d\r\n",__func__,audSR));

    DbgCode_DrvSound(if (aud_getLockStatus() == NO_TASK_LOCKED) DBG_ERR("%s: AudDrv not open\r\n",__func__);)

    // Set Audio
    if (DRVSOUND_SR_8000 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_8000);
    }
    else if (DRVSOUND_SR_11025 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_11025);
    }
    else if (DRVSOUND_SR_12000 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_12000);
    }
    else if (DRVSOUND_SR_16000 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_16000);
    }
    else if (DRVSOUND_SR_22050 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_22050);
    }
    else if (DRVSOUND_SR_24000 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_24000);
    }
    else if (DRVSOUND_SR_32000 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_32000);
    }
    else if (DRVSOUND_SR_44100 >= audSR)
    {
        aud_setSamplingRate(AUDIO_SR_44100);
    }
    else
    {
        aud_setSamplingRate(AUDIO_SR_48000);
    }
    DbgMsg_DrvSoundIO(("-%s:sr=%d\r\n",__func__,audSR));

    // Set HDMI & Resample
    switch (audSR)
    {
        case DRVSOUND_SR_8000:
        {
            bResampleEnable = TRUE;
            uiTargetAudRate = AUDIO_SR_32000;
            uiHDMIAudRate   = HDMI_AUDIO32KHZ;
            break;
        }

        case DRVSOUND_SR_11025:
        {
            bResampleEnable = TRUE;
            uiTargetAudRate = AUDIO_SR_44100;
            uiHDMIAudRate   = HDMI_AUDIO44_1KHZ;
            break;
        }

        case DRVSOUND_SR_12000:
        {
            bResampleEnable = TRUE;
            uiTargetAudRate = AUDIO_SR_32000;
            uiHDMIAudRate   = HDMI_AUDIO32KHZ;
            break;
        }

        case DRVSOUND_SR_16000:
        {
            bResampleEnable = TRUE;
            uiTargetAudRate = AUDIO_SR_32000;
            uiHDMIAudRate   = HDMI_AUDIO32KHZ;
            break;
        }

        case DRVSOUND_SR_22050:
        {
            bResampleEnable = TRUE;
            uiTargetAudRate = AUDIO_SR_44100;
            uiHDMIAudRate   = HDMI_AUDIO44_1KHZ;
            break;
        }

        case DRVSOUND_SR_24000:
        {
            bResampleEnable = TRUE;
            uiTargetAudRate = AUDIO_SR_48000;
            uiHDMIAudRate   = HDMI_AUDIO48KHZ;
            break;
        }

        case DRVSOUND_SR_32000:
        {
            bResampleEnable = FALSE;
            uiTargetAudRate = AUDIO_SR_32000;
            uiHDMIAudRate   = HDMI_AUDIO32KHZ;
            break;
        }

        case DRVSOUND_SR_44100:
        {
            bResampleEnable = FALSE;
            uiTargetAudRate = AUDIO_SR_44100;
            uiHDMIAudRate   = HDMI_AUDIO44_1KHZ;
            break;
        }

        default:
        case DRVSOUND_SR_48000:
        {
            bResampleEnable = FALSE;
            uiTargetAudRate = AUDIO_SR_48000;
            uiHDMIAudRate   = HDMI_AUDIO48KHZ;
            break;
        }
    }

    if (uiWorkBufSize < (uiTargetAudRate*4*4))
    {
        DBG_ERR("%s Buffer size is not enough!\r\n",__func__);
        return E_SYS;
    }

    // Set HDMI audio rate
    //if(hdmitx_open())
    {
        hdmitx_setConfig(HDMI_CONFIG_ID_AUDIO, uiHDMIAudRate);
    }

    // Set Resample
    ResampleInfo.bResample     = bResampleEnable;
    ResampleInfo.AudioTargetSR = uiTargetAudRate;
    ResampleInfo.uiDstBufAddr  = uiWorkBufAddr;
    ResampleInfo.uiDstBufSize  = uiWorkBufSize;

    aud_setResampleInfo(&ResampleInfo);

    return retV;
}

//XXXXXXXXXXXXXXX 2009/06/16 Lily Kao -end

