/**
    Audio Echo Cancellation library header file

    This file is the global header of Audio Echo Cancellation library.

    @file       Aec.h
    @ingroup    mIAudEC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

#ifndef   _AEC_H
#define   _AEC_H

#include "Type.h"
#include "Audio.h"

/**
    @addtogroup mIAudEC
*/
//@{


/**
    Audio Echo Cancellation(AEC) Configuration Select ID

    This is used in Aec_SetConfig() as the select ID to assign new configurations
    for Audio Echo Cancellation(AEC) library.
*/
typedef enum
{
    AEC_CONFIG_ID_SAMPLERATE,       ///< Set SampleRate in Hertz. Please use AUDIO_SR as input
    AEC_CONFIG_ID_RECORD_CH_NO,     ///< Set Record Channel Number. 1 is MONO. 2 is Stereo.
    AEC_CONFIG_ID_PLAYBACK_CH_NO,   ///< Set Playback Channel Number. 1 is MONO. 2 is Stereo.

    AEC_CONFIG_ID_NOISE_CANEL_EN,   ///< Set Noise Suppression Enable/Disable. Default is ENABLE.
    AEC_CONFIG_ID_NOISE_CANCEL_LVL, ///< Defualt is -20dB. Suggest value range -3 ~ -40. Unit in dB.
    AEC_CONFIG_ID_ECHO_CANCEL_LVL,  ///< Defualt is -50dB. Suggest value range -30 ~ -60. Unit in dB.

    AEC_CONFIG_ID_RECORD_ALIGN,     ///< Add 0x00 to the record buffer to align the playback and record buffer.
                                    ///< Unit in samples. For SampleRate larger than 19.2KHz, the max value is 512 samples.
                                    ///< Otherwise(8KHz), the max value is 256 samples. Default is 0.
                                    ///< This is used during "playback start first" so we need to add delay to record buffer.
    AEC_CONFIG_ID_PLAYBACK_ALIGN,   ///< Add 0x00 to the playback buffer to align the playback and record buffer.
                                    ///< Unit in samples. For SampleRate larger than 19.2KHz, the max value is 512 samples.
                                    ///< Otherwise(8KHz), the max value is 256 samples. Default is 0.
                                    ///< This is used during "record start first" so we need to add delay to playback buffer.
    AEC_CONFIG_ID_LEAK_ESTIMTAE_EN, ///< Enable/Disable the AEC leak estimate.
    AEC_CONFIG_ID_LEAK_ESTIMTAE_VAL,///< Initial Condition of the leak estimate. Please use AEC_QCONSTANT16( 0.25 ~ 0.99) as input parameter.

	AEC_CONFIG_ID_FILTER_LEN,		///< Set AEC internal Filter Length. Set to 0 is using default value. Default is 0.
	AEC_CONFIG_ID_FRAME_SIZE,		///< Set AEC internal Frame Size. Set to 0 is using default value. Default is 0.

    AEC_CONFIG_ID_RESERVED_0,       // Default is 1. DO NOT MODIFY THIS.
    AEC_CONFIG_ID_RESERVED_1,       // Default is 1. DO NOT MODIFY THIS.
    AEC_CONFIG_ID_RESERVED_2,       // Default is 0. DO NOT MODIFY THIS.
    ENUM_DUMMY4WORD(AEC_CONFIG_ID)
}AEC_CONFIG_ID;



/**
    Structure for Audio Echo Cancellation(AEC) input BitStream(BS) information

    Structure for audio Echo Cancellation input BitStream information, such as BS length and BS buffer address.
    The uiBsLength must be multiple of 1024 samples. This is used in Aec_Run().
*/
typedef struct
{
    UINT32      uiBsAddrPlayIn;     ///< Audio Playback buffer address for AEC input.
    UINT32      uiBsAddrRecordIn;   ///< Audio Record buffer address for AEC input.
    UINT32      uiBsAddrOut;        ///< AEC output BitStream buffer Address.

    UINT32      uiBsLength;         ///< Audio Stream Length in Samples count.
                                    ///< This value must be multiples of 1024.

}AEC_BITSTREAM,*PAEC_BITSTREAM;


#define AEC_QCONSTANT16(x) ((INT32)(.5+(x)*(((INT32)1)<<(15))))


//
//  Exporting APIs
//

extern ER       Aec_Open(void);
extern BOOL     Aec_IsOpened(void);
extern ER       Aec_Close(void);
extern void     Aec_SetConfig(AEC_CONFIG_ID AecSel, INT32 iAecConfig);
extern BOOL     Aec_Init(void);
extern BOOL     Aec_Run(PAEC_BITSTREAM pAecIO);







/*
    AudNS Related Definitions
*/


/**
    Audio Noise Suppression(AudNS) Configuration Select ID

    This is used in Aec_SetConfig() as the select ID to assign new configurations
    for Audio Echo Cancellation(AEC) library.
*/
typedef enum
{
    AUDNS_CONFIG_ID_SAMPLERATE,       ///< Set SampleRate in Hertz. Please use AUDIO_SR as input
    AUDNS_CONFIG_ID_CHANNEL_NO,       ///< Set Record Channel Number. 1 is MONO. 2 is Stereo.

    AUDNS_CONFIG_ID_NOISE_CANCEL_LVL, ///< Defualt is -20dB. Suggest value range -3 ~ -40. Unit in dB.


    AUDNS_CONFIG_ID_RESERVED_1,       // Default is 1. DO NOT MODIFY THIS.
    AUDNS_CONFIG_ID_RESERVED_2,       // Default is 0. DO NOT MODIFY THIS.
    ENUM_DUMMY4WORD(AUDNS_CONFIG_ID)
}AUDNS_CONFIG_ID;

/**
    Structure for Audio Echo Cancellation(AEC) input BitStream(BS) information

    Structure for audio Echo Cancellation input BitStream information, such as BS length and BS buffer address.
    The uiBsLength must be multiple of 1024 samples. This is used in Aec_Run().
*/
typedef struct
{
    UINT32      uiBsAddrIn;         ///< Audio Recorded buffer address for AudNS input.
    UINT32      uiBsAddrOut;        ///< AudNS output BitStream buffer Address.

    UINT32      uiBsLength;         ///< Audio Stream Length in Samples count.
                                    ///< This value must be multiples of 1024.

}AUDNS_BITSTREAM,*PAUDNS_BITSTREAM;


//
//  Exporting APIs
//

extern ER       AudNS_Open(void);
extern BOOL     AudNS_IsOpened(void);
extern ER       AudNS_Close(void);
extern void     AudNS_SetConfig(AUDNS_CONFIG_ID AudNSSel, INT32 iAudNSConfig);
extern BOOL     AudNS_Init(void);
extern BOOL     AudNS_Run(PAUDNS_BITSTREAM pAudNSIO);

//@}
#endif
