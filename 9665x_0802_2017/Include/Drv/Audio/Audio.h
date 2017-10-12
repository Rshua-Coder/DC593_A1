/**
    Header file for Audio module

    This file is the header file that define the API and data type
    for Audio module

    @file       Audio.h
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _AUDIO_H
#define _AUDIO_H

#include "Type.h"

/**
    @addtogroup mISYSAud
*/
//@{

/**
    @name AUDIO Event


    @note For aud_enableEvent(), aud_disableEvent(), and pEventHandler in AUDIO_DEVICE_OBJ.
*/
//@{
#define AUDIO_EVENT_TCLATCH             0x00000001      ///< Time code latch event
#define AUDIO_EVENT_TCHIT               0x00000002      ///< Time code hit event
#define AUDIO_EVENT_DMADONE             0x00000004      ///< DMA finish transfering one buffer event
#define AUDIO_EVENT_BUF_FULL            0x00000008      ///< Buffer queue full event (in record mode)
#define AUDIO_EVENT_BUF_EMPTY           0x00000010      ///< Buffer queue empty event (in playback mode)
#define AUDIO_EVENT_DONEBUF_FULL        0x00000020      ///< Done buffer queue full event
#define AUDIO_EVENT_FIFO_ERROR          0x00000100      ///< FIFO Error
//@}


/**
    Audio codec selection

    @note For aud_switchCodec()
*/
typedef enum
{
    AUDIO_CODECSEL_DEFAULT,     ///< Default audio codec. Can be embeddec codec or external codec.
    AUDIO_CODECSEL_HDMI,        ///< HDMI audio

    ENUM_DUMMY4WORD(AUDIO_CODECSEL)
} AUDIO_CODECSEL;

/**
    AUDIO Format

    Audio Interface format

    @note For Fmt of AUDIO_SETTING
*/
typedef enum
{
    AUDIO_FMT_I2S,              ///< I2S
    AUDIO_FMT_AC97,             ///< AC97

    ENUM_DUMMY4WORD(AUDIO_FMT)
} AUDIO_FMT;

/**
    AUDIO Sampling Rate

    @note For SamplingRate of AUDIO_SETTING, AudioTargetSR of AUDIO_RESAMPLE_INFO, and aud_setSamplingRate()
*/
typedef enum
{
    AUDIO_SR_8000   = 8000,     ///< 8 KHz
    AUDIO_SR_11025  = 11025,    ///< 11.025 KHz
    AUDIO_SR_12000  = 12000,    ///< 12 KHz
    AUDIO_SR_16000  = 16000,    ///< 16 KHz
    AUDIO_SR_22050  = 22050,    ///< 22.05 KHz
    AUDIO_SR_24000  = 24000,    ///< 24 KHz
    AUDIO_SR_32000  = 32000,    ///< 32 KHz
    AUDIO_SR_44100  = 44100,    ///< 44.1 KHz
    AUDIO_SR_48000  = 48000,    ///< 48 KHz

    ENUM_DUMMY4WORD(AUDIO_SR)
} AUDIO_SR;

/**
    AUDIO Channel

    @note For AUDIO_SETTING, and aud_setChannel()
*/
typedef enum
{
    AUDIO_CH_LEFT,              ///< Left
    AUDIO_CH_RIGHT,             ///< Right
    AUDIO_CH_STEREO,            ///< Stereo
    AUDIO_CH_MONO,              ///< Mono two channel

    ENUM_DUMMY4WORD(AUDIO_CH)
} AUDIO_CH;

/**
    AUDIO Volume

    @note For aud_setVolume()
*/
typedef enum
{
    AUDIO_VOL_MUTE,             ///< Volume mute
    AUDIO_VOL_0,                ///< Volume 0
    AUDIO_VOL_1,                ///< Volume 1
    AUDIO_VOL_2,                ///< Volume 2 (Default)
    AUDIO_VOL_3,                ///< Volume 3
    AUDIO_VOL_4,                ///< Volume 4
    AUDIO_VOL_5,                ///< Volume 5
    AUDIO_VOL_6,                ///< Volume 6
    AUDIO_VOL_7,                ///< Volume 7
    AUDIO_VOL_8,                ///< Volume 8
    AUDIO_VOL_9,                ///< Volume 9
    AUDIO_VOL_10,               ///< Volume 10
    AUDIO_VOL_11,               ///< Volume 11
    AUDIO_VOL_12,               ///< Volume 12
    AUDIO_VOL_13,               ///< Volume 13
    AUDIO_VOL_14,               ///< Volume 14
    AUDIO_VOL_15,               ///< Volume 15
    AUDIO_VOL_16,               ///< Volume 16
    AUDIO_VOL_17,               ///< Volume 17
    AUDIO_VOL_18,               ///< Volume 18
    AUDIO_VOL_19,               ///< Volume 19
    AUDIO_VOL_20,               ///< Volume 20
    AUDIO_VOL_21,               ///< Volume 21
    AUDIO_VOL_22,               ///< Volume 22
    AUDIO_VOL_23,               ///< Volume 23
    AUDIO_VOL_24,               ///< Volume 24
    AUDIO_VOL_25,               ///< Volume 25
    AUDIO_VOL_26,               ///< Volume 26
    AUDIO_VOL_27,               ///< Volume 27
    AUDIO_VOL_28,               ///< Volume 28
    AUDIO_VOL_29,               ///< Volume 29
    AUDIO_VOL_30,               ///< Volume 30
    AUDIO_VOL_31,               ///< Volume 31
    AUDIO_VOL_32,               ///< Volume 32
    AUDIO_VOL_33,               ///< Volume 33
    AUDIO_VOL_34,               ///< Volume 34
    AUDIO_VOL_35,               ///< Volume 35
    AUDIO_VOL_36,               ///< Volume 36
    AUDIO_VOL_37,               ///< Volume 37
    AUDIO_VOL_38,               ///< Volume 38
    AUDIO_VOL_39,               ///< Volume 39
    AUDIO_VOL_40,               ///< Volume 40
    AUDIO_VOL_41,               ///< Volume 41
    AUDIO_VOL_42,               ///< Volume 42
    AUDIO_VOL_43,               ///< Volume 43
    AUDIO_VOL_44,               ///< Volume 44
    AUDIO_VOL_45,               ///< Volume 45
    AUDIO_VOL_46,               ///< Volume 46
    AUDIO_VOL_47,               ///< Volume 47
    AUDIO_VOL_48,               ///< Volume 48
    AUDIO_VOL_49,               ///< Volume 49
    AUDIO_VOL_50,               ///< Volume 50
    AUDIO_VOL_51,               ///< Volume 51
    AUDIO_VOL_52,               ///< Volume 52
    AUDIO_VOL_53,               ///< Volume 53
    AUDIO_VOL_54,               ///< Volume 54
    AUDIO_VOL_55,               ///< Volume 55
    AUDIO_VOL_56,               ///< Volume 56
    AUDIO_VOL_57,               ///< Volume 57
    AUDIO_VOL_58,               ///< Volume 58
    AUDIO_VOL_59,               ///< Volume 59
    AUDIO_VOL_60,               ///< Volume 60
    AUDIO_VOL_61,               ///< Volume 61
    AUDIO_VOL_62,               ///< Volume 62
    AUDIO_VOL_63,               ///< Volume 63

    ENUM_DUMMY4WORD(AUDIO_VOL)
} AUDIO_VOL;

/**
    AUDIO Gain

    @note For aud_setGain()
*/
typedef enum
{
    AUDIO_GAIN_MUTE,            ///< Gain mute
    AUDIO_GAIN_0,               ///< Gain 0
    AUDIO_GAIN_1,               ///< Gain 1
    AUDIO_GAIN_2,               ///< Gain 2 (Default)
    AUDIO_GAIN_3,               ///< Gain 3
    AUDIO_GAIN_4,               ///< Gain 4
    AUDIO_GAIN_5,               ///< Gain 5
    AUDIO_GAIN_6,               ///< Gain 6
    AUDIO_GAIN_7,               ///< Gain 7

    ENUM_DUMMY4WORD(AUDIO_GAIN)
} AUDIO_GAIN;

/**
    AUDIO Total Volume Level

    @note For aud_setTotalVolLevel()
*/
typedef enum
{
    AUDIO_VOL_LEVEL8  =  8,            ///< total  8 volume level
    AUDIO_VOL_LEVEL64 = 64,            ///< total 64 volume level

    ENUM_DUMMY4WORD(AUDIO_VOL_LEVEL)
} AUDIO_VOL_LEVEL;

/**
    AUDIO Record Source

    @note For AUDIO_SETTING, and aud_setRecordSource()
*/
typedef enum
{
    AUDIO_RECSRC_MIC,           ///< Microphone
    AUDIO_RECSRC_CD,            ///< CD In
    AUDIO_RECSRC_VIDEO,         ///< Video In
    AUDIO_RECSRC_AUX,           ///< Aux In
    AUDIO_RECSRC_LINE,          ///< Line In
    AUDIO_RECSRC_STEREO_MIX,    ///< Stereo Mix
    AUDIO_RECSRC_MONO_MIX,      ///< Mono Mix
    AUDIO_RECSRC_PHONE,         ///< Phone In

    ENUM_DUMMY4WORD(AUDIO_RECSRC)
} AUDIO_RECSRC;

/**
    AUDIO Output/playback path

    @note For AUDIO_SETTING, and aud_setOutput()
*/
typedef enum
{
    AUDIO_OUTPUT_SPK,           ///< Speaker
    AUDIO_OUTPUT_HP,            ///< Headphone
    AUDIO_OUTPUT_LINE,          ///< Line Out
    AUDIO_OUTPUT_MONO,          ///< Mono Out
    AUDIO_OUTPUT_NONE,          ///< Disable output

    ENUM_DUMMY4WORD(AUDIO_OUTPUT)
} AUDIO_OUTPUT;

/**
    AUDIO Effect

    @note For aud_setEffect()
*/
typedef enum
{
    AUDIO_EFFECT_NONE,          ///< No special audio effect
    AUDIO_EFFECT_3D_HALF,       ///< 50% 3D effect
    AUDIO_EFFECT_3D_FULL,       ///< 100% 3D effect
    AUDIO_EFFECT_ROCK,          ///< Rock
    AUDIO_EFFECT_POP,           ///< POP
    AUDIO_EFFECT_JAZZ,          ///< Jazz
    AUDIO_EFFECT_CLASSICAL,     ///< Classical
    AUDIO_EFFECT_DNACE,         ///< Dance
    AUDIO_EFFECT_HEAVY,         ///< Heavy
    AUDIO_EFFECT_DISCO,         ///< Disco
    AUDIO_EFFECT_SOFT,          ///< Soft
    AUDIO_EFFECT_LIVE,          ///< Live
    AUDIO_EFFECT_HALL,          ///< Hall

    ENUM_DUMMY4WORD(AUDIO_EFFECT)
} AUDIO_EFFECT;

/**
    AUDIO I2S clock ratio

    @note For AUDIO_I2S_SETTING, and aud_setI2SClockRatio()
*/
typedef enum
{
    AUDIO_I2SCLKR_256FS_32BIT,  ///< SystemClk = 256 * FrameSync, FrameSync = 32 BitClk
    AUDIO_I2SCLKR_256FS_64BIT,  ///< SystemClk = 256 * FrameSync, FrameSync = 64 BitClk
    AUDIO_I2SCLKR_384FS_32BIT,  ///< SystemClk = 384 * FrameSync, FrameSync = 32 BitClk
    AUDIO_I2SCLKR_384FS_48BIT,  ///< SystemClk = 384 * FrameSync, FrameSync = 48 BitClk
    AUDIO_I2SCLKR_384FS_96BIT,  ///< SystemClk = 384 * FrameSync, FrameSync = 96 BitClk
    AUDIO_I2SCLKR_512FS_32BIT,  ///< SystemClk = 512 * FrameSync, FrameSync = 32 BitClk
    AUDIO_I2SCLKR_512FS_64BIT,  ///< SystemClk = 512 * FrameSync, FrameSync = 64 BitClk
    AUDIO_I2SCLKR_512FS_128BIT, ///< SystemClk = 512 * FrameSync, FrameSync = 128 BitClk
    AUDIO_I2SCLKR_768FS_32BIT,  ///< SystemClk = 768 * FrameSync, FrameSync = 32 BitClk
    AUDIO_I2SCLKR_768FS_48BIT,  ///< SystemClk = 768 * FrameSync, FrameSync = 48 BitClk
    AUDIO_I2SCLKR_768FS_64BIT,  ///< SystemClk = 768 * FrameSync, FrameSync = 64 BitClk
    AUDIO_I2SCLKR_768FS_192BIT, ///< SystemClk = 768 * FrameSync, FrameSync = 192 BitClk
    AUDIO_I2SCLKR_1024FS_32BIT, ///< SystemClk = 1024 * FrameSync, FrameSync = 32 BitClk
    AUDIO_I2SCLKR_1024FS_64BIT, ///< SystemClk = 1024 * FrameSync, FrameSync = 64 BitClk
    AUDIO_I2SCLKR_1024FS_128BIT,///< SystemClk = 1024 * FrameSync, FrameSync = 128 BitClk
    AUDIO_I2SCLKR_1024FS_256BIT,///< SystemClk = 1024 * FrameSync, FrameSync = 256 BitClk

    ENUM_DUMMY4WORD(AUDIO_I2SCLKR)
} AUDIO_I2SCLKR;

/**
    AUDIO Clock Rate

    @note For AUDIO_CLK_SETTING
*/
typedef enum
{
    AUDIO_CLK_24576,            ///< 24.576 MHz
    AUDIO_CLK_12288,            ///< 12.288 MHz
    AUDIO_CLK_8192,             ///< 8.192 MHz
    AUDIO_CLK_6144,             ///< 6.144 MHz
    AUDIO_CLK_4096,             ///< 4.096 MHz
    AUDIO_CLK_3072,             ///< 3.072 MHz
    AUDIO_CLK_2048,             ///< 2.048 MHz
    AUDIO_CLK_11289,            ///< 11.289 MHz
    AUDIO_CLK_5644,             ///< 5.644 MHz
    AUDIO_CLK_2822,             ///< 2.822 MHz

    ENUM_DUMMY4WORD(AUDIO_CLK)
} AUDIO_CLK;

/**
    AUDIO I2S format

    @note For AUDIO_I2S_SETTING, and aud_setI2SFormat()
*/
typedef enum
{
    AUDIO_I2SFMT_STANDARD,      ///< I2S Standard
    AUDIO_I2SFMT_DSP,           ///< I2S DSP mode
    AUDIO_I2SFMT_LIKE_MSB,      ///< I2S Like, MSB justified
    AUDIO_I2SFMT_LIKE_LSB,      ///< I2S Like, LSB justified

    ENUM_DUMMY4WORD(AUDIO_I2SFMT)
} AUDIO_I2SFMT;

/**
    AUDIO I2S Control

    @note For AUDIO_DEVICE_OBJ
*/
typedef enum
{
    AUDIO_I2SCTRL_SIF,          ///< Use SIF to control I2S codec
    AUDIO_I2SCTRL_I2C,          ///< Use I2C to control I2S codec
    AUDIO_I2SCTRL_GPIO_SIF,     ///< Use GPIO to simulate SIF to control I2S codec
    AUDIO_I2SCTRL_GPIO_I2C,     ///< Use GPIO to simulate I2C to control I2S codec
    AUDIO_I2SCTRL_NONE,         ///< Not a I2S codec

    ENUM_DUMMY4WORD(AUDIO_I2SCTRL)
} AUDIO_I2SCTRL;

/**
    AUDIO Clock Select

    @note For aud_setParameter() with AUDIO_PARAMETER_ADO_CLK_SEL or AUDIO_PARAMETER_HDMI_ADO_CLK_SEL
*/
typedef enum
{
    AUDIO_CLK_SEL_CLKMUX,       ///< Audio clock from clock mux
    AUDIO_CLK_SEL_PLL_DIV,      ///< Audio clock is divided from audio PLL

    ENUM_DUMMY4WORD(AUDIO_CLK_SEL)
} AUDIO_CLK_SEL;

/**
    AUDIO Message Select

    @note For aud_setParameter() with AUDIO_PARAMETER_DBG_MSG_LEVEL
*/
typedef enum
{
    AUDIO_MSG_LVL_0,            ///< Only output message when error is detected
    AUDIO_MSG_LVL_1,            ///< Output all debug messages

    ENUM_DUMMY4WORD(AUDIO_MSG_LVL)
} AUDIO_MSG_LVL;

/**
    AUDIO Codec Feature

    @note For aud_setFeature()
*/
typedef enum
{
    AUDIO_FEATURE_ALC,                      ///< [For default codec] Auto level control (Recording path) (Default disabled)
    AUDIO_FEATURE_MICBOOST,                 ///< [For default codec] Microphone boost (Default disabled)
    AUDIO_FEATURE_NG_WITH_MICBOOST,         ///< [For default codec] Noise gate function with microphone boost (internal or external) (Default disabled)
    AUDIO_FEATURE_PLAYBACK_PCM_EXPAND,      ///< [For common audio] PCM data expand.Audio controller will duplicate mono PCM data(DRAM data) and playback to stereo channel. (Default enabled)
    AUDIO_FEATURE_RECORD_PCM_EXPAND,        ///< [For common audio] PCM data expand.Audio controller will record mono channel PCM data and duplicate to DRAM. (Default disabled)
    AUDIO_FEATURE_SPK_PWR_ALWAYSON,         ///< [For default codec] Speaker power always on (Default disabled)
    AUDIO_FEATURE_HP_PWR_ALWAYSON,          ///< [For default codec] Head Phone power always on (Default enabled)
    AUDIO_FEATURE_LINE_PWR_ALWAYSON,        ///< [For default codec] LineOut power always on (Default enabled)
    AUDIO_FEATURE_INTERFACE_ALWAYS_ACTIVE,  ///< [For common audio] Audio interface always active (Default disabled)
    AUDIO_FEATURE_CHECK_PLAY_SAMPLING_RATE, ///< [for default/HDMI codec] Check sampling rate (AUDIO_PARAMETER_CHECKED_PLAY_SAMPLING_RATE) in playback mode.
                                            ///< (Default disabled)
    AUDIO_FEATURE_SPK_LEFT_EN,              ///< [For default codec] Speaker left channel enable/disable (Default enabled)
    AUDIO_FEATURE_SPK_RIGHT_EN,             ///< [For default codec] Speaker right channel enable/disable (Default enabled)
    AUDIO_FEATURE_PLAY2ALLCODEC,            ///< [For common audio] Playback to all codec (Default disabled)
    AUDIO_FEATURE_DISCONNECT_HDMI,          ///< [For common audio] Disconnect HDMI from DAI (Default is disabled)
    AUDIO_FEATURE_NOISEGATE_EN,             ///< [For default codec] Audio Record Noise Gate function enable/disable (Default enabled).
                                            ///< This setting is valid only if AUDIO_FEATURE_ALC is enabled.
    AUDIO_FEATURE_AUTOPINMUX,               ///< [For common audio] Control I2S pinmux (except MCLK) when aud_open()/aud_close() (Default disabled)
    AUDIO_FEATURE_MCLK_AUTOPINMUX,          ///< [For common audio] Control I2S MCLK pinmux when aud_open()/aud_close() (Default disabled)

    AUDIO_FEATURE_HP_DEPOP_EN,              ///< Headphone path Depop function enable/disable. This is valid only if HP power always ON. The Default value is DISABLE.
    AUDIO_FEATURE_LINE_DEPOP_EN,            ///< Lineout path Depop function enable/disable. This is valid only if Lineout power always ON. The Default value is DISABLE.
    AUDIO_FEATURE_SWDISCHARGE_EN,           ///< After the HP/Lineout power always on is turning off. This field can select to enable/disable the output path pull down by the SW control.
                                            ///< The Default value is ENABLE.
    AUDIO_FEATURE_RECORD_PWR_ALWAYSON,      ///< [For default codec] Record power always on (Default disabled)

    // For emulation
    AUDIO_FEATURE_DEBUG_MODE_EN,            //< debug mode (Default disabled)

    ENUM_DUMMY4WORD(AUDIO_FEATURE)
} AUDIO_FEATURE;

/**
    AUDIO Codec Parameter

    This definition is used in aud_setParameter() to assign new parameters to specified function.

    @note For aud_setParameter()
*/
typedef enum
{
    AUDIO_PARAMETER_NOISEGAIN,                  ///< [For default codec] Noise gain. (Valid value: 0x0~0xF)\n
                                                ///< The Noise gate Target Level = ALC_LEVEL - (NOISE_GATE_THRESHOLD - SIGNAL_LEVEL)*(1 + uiSetting)
    AUDIO_PARAMETER_NOISETHD_WITH_BOOST,        ///< [For default codec] NoiseGate threshold when boost is Enabled. Please use the enumeration "EAC_NG_THRESHOLD" as input parameter uiSetting for embedded audio codec.
    AUDIO_PARAMETER_NOISETHD_WITHOUT_BOOST,     ///< [For default codec] NoiseGate threshold when boost is Disabled. Please use the enumeration "EAC_NG_THRESHOLD" as input parameter uiSetting for embedded audio codec.
    AUDIO_PARAMETER_NOISEGATE_TIME_RESOLUTION,  ///< [For default codec] ALC Time Resolution for NoiseGate. Please use "EAC_ALC_TRESO_BASIS" as input paramter. The default value is EAC_ALC_TRESO_BASIS_10000US.
    AUDIO_PARAMETER_ALC_MAXGAIN,                ///< [For default codec] ALC max gain. Please use the enumeration "EAC_ALC_MAXGAIN" as input parameter uiSetting for embedded audio codec.
    AUDIO_PARAMETER_ALC_MINGAIN,                ///< [For default codec] ALC minimum gain. Please use the enumeration "EAC_ALC_MINGAIN" as input parameter uiSetting for embedded audio codec.
    AUDIO_PARAMETER_ALC_ATTACK_TIME,            ///< [For default codec] ALC Attack Time length. Please reference to eac.h for EAC_CONFIG_AD detailed desceiptions of the input value.
    AUDIO_PARAMETER_ALC_DECAY_TIME,             ///< [For default codec] ALC Decay Time length. Please reference to eac.h for EAC_CONFIG_AD detailed desceiptions of the input value.
    AUDIO_PARAMETER_ALC_HOLD_TIME,              ///< [For default codec] ALC Hold Time length. Please reference to eac.h for EAC_CONFIG_AD detailed desceiptions of the input value.
    AUDIO_PARAMETER_ALC_TIME_RESOLUTION,        ///< [For default codec] ALC Basis Time Resolution for Attack/Decay Time Calculation. Default value is 0x2FFFFF.
    AUDIO_PARAMETER_CHECKED_PLAY_SAMPLING_RATE, ///< [For common audio] Check playback sampling rate when AUDIO_FEATURE_CHECK_PLAY_SAMPLING_RATE is enabled.
                                                ///< (Valid setting is AUDIO_SR) (Default is AUDIO_SR_32000)
    AUDIO_PARAMETER_ADO_CLK_SEL,                ///< [For common audio] Audio clock source select
                                                ///<  - @b AUDIO_CLK_SEL_CLKMUX (Default)
                                                ///<  - @b AUDIO_CLK_SEL_PLL_DIV
    AUDIO_PARAMETER_HDMI_ADO_CLK_SEL,           ///< [For common audio] HDMI Audio clock source select
                                                ///<  - @b AUDIO_CLK_SEL_CLKMUX (Default)
                                                ///<  - @b AUDIO_CLK_SEL_PLL_DIV
    AUDIO_PARAMETER_BOOST_GAIN,                 ///< [For default codec] Recording PGA gain select. This would be activate when setting feature AUDIO_FEATURE_MICBOOST enabled.
                                                ///< Please use EAC_PGABOOST_SEL_20DB/ EAC_PGABOOST_SEL_15DB/ EAC_PGABOOST_SEL_10DB as input parameter uiSetting.
    AUDIO_PARAMETER_RECORD_DIGITAL_GAIN,        ///< [For default codec] Recording Digital Gain. Value range 0x1~0xFF which mapping to -97.0dB ~ +30.0dB(+0.5dB each step). Value 0x0 is mute.
    AUDIO_PARAMETER_DCCAN_RESOLUTION,           ///< [For default codec] Recording DC Cancellation Resolution. Please use the enumeration "EAC_DCCAN_RESO" as input parameter uiSetting for embedded audio codec.
    AUDIO_PARAMETER_RECORD_DELAY,               ///< [For default codec] After record start(aud_record), the MIC_BAIS would be enabled and the external regulator needs some stable time before record really start.
                                                ///< This delay is used to prevent recording this pop noise. The default value is 200 (mini-seconds). This setting unit is mini-seconds.
    AUDIO_PARAMETER_DBG_MSG_LEVEL,              ///< [For common audio]Debug message output select
                                                ///< - @b AUDIO_MSG_LVL_0 (Default): only output error message
                                                ///< - @b AUDIO_MSG_LVL_1: output message when API is invoked
    AUDIO_PARAMETER_MAXMINGAIN_OFS,             ///< [For default codec] This paramter can adjust the ALC Max/Min Gain by +2.5 to -3dB simutaneously with 0.5dB per step.
                                                ///< Please use "EAC_DGAIN" as input parameter. Only +2.5 to -3dB is allowed in current implementation.
    AUDIO_PARAMETER_PLAYDGAIN,                  ///< [For default codec] This parameter can adjust the audio playback additional Digital gain.
                                                ///< Please use AUDIO_PLAYDGAIN as input parameter, The default value is AUDIO_PLAYDGAIN_0DB.
    AUDIO_PARAMETER_DAGAIN,                     ///< [For default codec] This parameter can adjust the embedded audio codec DA default gain.
                                                ///< The formula is "-(0xFF - Cfg_Value)/2" dB. 0xFF is 0dB. The default value is 0xF6 which means -4.5dB.

    // For emulation
    AUDIO_PARAMETER_DEBUG_BUF_ADDR,             //< Debug buffer DRAM address
    AUDIO_PARAMETER_DEBUG_BUF_SIZE,             //< Debug buffer size

    ENUM_DUMMY4WORD(AUDIO_PARAMETER)
} AUDIO_PARAMETER;

/**
    AUDIO Codec Type

    @note For codecType of AUDIO_CODEC_FUNC
*/
typedef enum
{
    AUDIO_CODEC_TYPE_EMBEDDED,          ///< Embedded audio codec
    AUDIO_CODEC_TYPE_HDMI,              ///< HDMI audio codec
    AUDIO_CODEC_TYPE_EXTERNAL,          ///< External audio codec connected with I2S

    ENUM_DUMMY4WORD(AUDIO_CODEC_TYPE)
} AUDIO_CODEC_TYPE;

/**
    AUDIO I2S Setting

    @note For I2S of AUDIO_SETTING
*/
typedef struct
{
    AUDIO_I2SCLKR       ClkRatio;       ///< Clock Ratio
    AUDIO_I2SFMT        I2SFmt;         ///< I2S format
    BOOL                bMaster;        ///< Audio Controller Master/Slave Mode
                                        ///< - @b TRUE: Master mode
                                        ///< - @b FALSE: Slave mode
} AUDIO_I2S_SETTING;

/**
    AUDIO Clock Setting

    @note For Clock of AUDIO_SETTING
*/
typedef struct
{
    BOOL                bClkExt;        ///< Clock Source
                                        ///< - @b TRUE: External clock source
                                        ///< - @b FALSE: Internal clock source
    AUDIO_CLK           Clk;            ///< Clock Rate (valid when bClkExt is FALSE)
} AUDIO_CLK_SETTING;

/**
    AUDIO Setting

    @note For aud_init()
*/
typedef struct
{
    AUDIO_CLK_SETTING   Clock;          ///< Audio clock setting
    AUDIO_I2S_SETTING   I2S;            ///< I2S Setting
    AUDIO_FMT           Fmt;            ///< Audio interface format
    AUDIO_SR            SamplingRate;   ///< Sampling Rate
    AUDIO_CH            Channel;        ///< Audio channel
    AUDIO_RECSRC        RecSrc;         ///< Record Source
    AUDIO_OUTPUT        Output;         ///< Output/playback Path
    BOOL                bEmbedded;      ///< Embedded audio codec or not
                                        ///< - @b TRUE: Use embedded codec
                                        ///< - @b FALSE: Use external codec
} AUDIO_SETTING, *PAUDIO_SETTING;

/**
    AUDIO Buffer Queue

    @note For aud_addBufferToQueue(), aud_getDoneBufferFromQueue(), and aud_getCurrentBufferFromQueue()
*/
typedef struct _AUDIO_BUF_QUEUE
{
    UINT32                  uiAddress;  ///< Buffer Starting Address (Unit: byte) (Word alignment)
    UINT32                  uiSize;     ///< Buffer Size (Unit: byte) (Word Alignment)
    UINT32                  uiValidSize;///< Valid PCM data size (Unit: byte).
                                        ///< Returend by aud_getDoneBufferFromQueue()
    struct _AUDIO_BUF_QUEUE *pNext;     ///< Next queue element
} AUDIO_BUF_QUEUE, *PAUDIO_BUF_QUEUE;

/**
    AUDIO Device Object

    @note For aud_open(), and aud_getDeviceObject()
*/
typedef struct
{
    void            (*pEventHandler)(UINT32 EventID);   ///< EventHandler function pointer, set to NULL if you don't want to handle audio event
    UINT32          uiGPIOColdReset;                    ///< GPIO pin number of AC97 Cold Reset pin
    AUDIO_I2SCTRL   uiI2SCtrl;                          ///< I2S Control Interface
    UINT32          uiChannel;                          ///< SIF channel number to control I2S codec, only valid when uiI2SCtrl == AUDIO_I2SCTRL_SIF, ADC channel for ADC recording
    UINT32          uiGPIOData;                         ///< GPIO pin number for SIF DATA / I2C SDA, only valid when uiI2SCtrl == AUDIO_I2SCTRL_GPIO_SIF or AUDIO_I2SCTRL_GPIO_I2C
    UINT32          uiGPIOClk;                          ///< GPIO pin number for SIF CLK / I2C SCL, only valid when uiI2SCtrl == AUDIO_I2SCTRL_GPIO_SIF or AUDIO_I2SCTRL_GPIO_I2C
    UINT32          uiGPIOCS;                           ///< GPIO pin number for SIF CS, only valid when uiI2SCtrl == AUDIO_I2SCTRL_GPIO_SIF
    UINT32          uiADCZero;                          ///< ADC value of PCM data = 0
} AUDIO_DEVICE_OBJ, *PAUDIO_DEVICE_OBJ;

/**
    Audio re-sampling information
*/
typedef struct
{
    BOOL            bResample;          ///< Enable/Disable playback re-sampling function
                                        ///< - @b TRUE: enable re-sampling
                                        ///< - @b FALSE: disable re-sampling
    UINT32          uiDstBufAddr;       ///< buffer address for re-sampling (unit: byte) (Word alignment)
    UINT32          uiDstBufSize;       ///< buffer size for re-sampling, must be target sampling rate * 4 * 4
    AUDIO_SR        AudioTargetSR;      ///< Target Sampling rate after re-sampling (i.e. actual sampling rate on DAI)
} AUDIO_RESAMPLE_INFO, *PAUDIO_RESAMPLE_INFO;

/**
    Add Audio playback additional Digital Gain

    This is used in aud_setParameter(AUDIO_PARAMETER_PLAYDGAIN) to assign additional playback Digital Gain.
*/
typedef enum
{
    AUDIO_PLAYDGAIN_0DB,                ///< Do not add any additional digital gain to audio playback. (Default)
    AUDIO_PLAYDGAIN_P6DB,               ///< Add additional  +6.0dB digital gain to audio playback.
    AUDIO_PLAYDGAIN_P12DB,              ///< Add additional +12.0dB digital gain to audio playback.
    AUDIO_PLAYDGAIN_P18DB,              ///< Add additional +18.0dB digital gain to audio playback.

    ENUM_DUMMY4WORD(AUDIO_PLAYDGAIN)
}AUDIO_PLAYDGAIN;


/**
    Audio CODEC function table

    Function table exported from audio codec to audio library.
    Users don't need to care contents/member of this structure.

    @note For audExtCodec_getFP(), aud_setExtCodec()
*/
typedef struct
{
    AUDIO_CODEC_TYPE codecType;

    void    (*setDevObj)(PAUDIO_DEVICE_OBJ pAudDevObj);
    void    (*init)(PAUDIO_SETTING pAudio);
    void    (*setRecordSource)(AUDIO_RECSRC RecSrc);
    void    (*setOutput)(AUDIO_OUTPUT Output);
    void    (*setSamplingRate)(AUDIO_SR SamplingRate);
    void    (*setChannel)(AUDIO_CH Channel);
    void    (*setVolume)(AUDIO_VOL Vol);
    void    (*setGain)(AUDIO_GAIN Gain);
    void    (*setGainDB)(FLOAT db);
    void    (*setEffect)(AUDIO_EFFECT Effect);
    BOOL    (*setFeature)(AUDIO_FEATURE Feature, BOOL bEnable);
    void    (*stop)(void);
    void    (*recordPreset)(void);
    void    (*record)(void);
    void    (*playback)(void);
    void    (*setFormat)(AUDIO_I2SFMT I2SFmt);
    void    (*setClockRatio)(AUDIO_I2SCLKR I2SCLKRatio);
    BOOL    (*sendCommand)(UINT32 uiReg, UINT32 uiData);
    BOOL    (*readData)(UINT32 uiReg, UINT32 *puiData);
    BOOL    (*setParameter)(AUDIO_PARAMETER Parameter, UINT32 uiSetting);
    BOOL    (*chkSamplingRate)(void);
    void    (*open)(void);
    void    (*close)(void);
} AUDIO_CODEC_FUNC, *PAUDIO_CODEC_FUNC;

// API
extern ER       aud_open(PAUDIO_DEVICE_OBJ pDevObj);
extern ER       aud_close(void);
extern BOOL     aud_getLockStatus(void);
extern void     aud_getDeviceObject(PAUDIO_DEVICE_OBJ pDevObj);

extern void     aud_init(PAUDIO_SETTING pAudio);

extern void     aud_setRecordSource(AUDIO_RECSRC RecSrc);
extern void     aud_setOutput(AUDIO_OUTPUT Output);
extern void     aud_setSamplingRate(AUDIO_SR SamplingRate);
extern void     aud_setChannel(AUDIO_CH Channel);
extern void     aud_setTotalVolLevel(AUDIO_VOL_LEVEL Audio_VolLevel);
extern AUDIO_VOL_LEVEL  aud_getTotalVolLevel(void);
extern void     aud_setVolume(AUDIO_VOL Vol);
extern AUDIO_VOL aud_getVolume(void);
extern void     aud_setGain(AUDIO_GAIN Gain);
extern void     aud_setGainDB(FLOAT fDB);
extern void     aud_setEffect(AUDIO_EFFECT Effect);
extern void     aud_setFeature(AUDIO_FEATURE Feature, BOOL bEnable);
extern void     aud_setParameter(AUDIO_PARAMETER Parameter, UINT32 uiSetting);

extern void     aud_setI2SFormat(AUDIO_I2SFMT I2SFmt);
extern void     aud_setI2SClockRatio(AUDIO_I2SCLKR I2SCLKRatio);

extern void             aud_resetBufferQueue(void);
extern BOOL             aud_isBufferQueueFull(void);
extern BOOL             aud_addBufferToQueue(PAUDIO_BUF_QUEUE pAudioBufQueue);
extern PAUDIO_BUF_QUEUE aud_getDoneBufferFromQueue(void);
extern PAUDIO_BUF_QUEUE aud_getCurrentBufferFromQueue(void);

extern void     aud_stop(void);
extern void     aud_recordPreset(void);
extern void     aud_record(BOOL bAVSync, BOOL bTCLatchEvent, BOOL bTCHitEvent);
extern ER       aud_playback(BOOL bTCLatchEvent, BOOL bTCHitEvent);
extern void     aud_pause(void);
extern void     aud_resume(void);

extern void     aud_setTimecodeTrigger(UINT32 uiTrigger);
extern void     aud_setTimecodeOffset(UINT32 uiOffset);
extern UINT32   aud_getTimecodeValue(void);

extern BOOL     aud_isPlaybackMode(void);
extern BOOL     aud_isBusy(void);

extern void     aud_sendAC97Command(BOOL bWriteCmd, UINT32 uiRegIdx, UINT32 uiData);
extern void     aud_readAC97Data(UINT32 *puiRegIdx, UINT32 *puiData);
extern void     aud_coldresetAC97(void);
extern void     aud_warmresetAC97(void);

extern BOOL     aud_sendI2SCommand(UINT32 uiRegIdx, UINT32 uiData);
extern BOOL     aud_readI2SData(UINT32 uiRegIdx, UINT32 *puiData);

extern void     aud_enableEvent(UINT32 uiEvent);
extern void     aud_disableEvent(UINT32 uiEvent);

extern BOOL     aud_setResampleInfo(PAUDIO_RESAMPLE_INFO pResampleInfo);

extern void     aud_setExtCodec(PAUDIO_CODEC_FUNC pAudioCodecFunc);
extern void     aud_switchCodec(AUDIO_CODECSEL AudioCodec);

extern void     aud_printSetting(void);

//@}

#endif
