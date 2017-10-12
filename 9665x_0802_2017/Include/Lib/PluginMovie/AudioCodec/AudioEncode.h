/**
    Header file of audio encoding codec library

    Exported header file of audio encoding codec library.

    @file       AudioEncode.h
    @ingroup    mIAVCODEC
    @note       Nothing.
    @version    V1.01.003
    @date       2010/05/05

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _AUDIOENCODE_H
#define _AUDIOENCODE_H

#include "Type.h"
#include "kernel.h"
#include <stdio.h>
#include <string.h>
/**
    @addtogroup mIAVCODEC
*/
//@{

#define AAC_RAW_BLOCK  1024 //encode one aac frame needs 1024 samples

#define MEDIAENCAUD_TYPE_ENCLAST 1//enc last, struct = AUD_CUSTOM_ENCLAST_INFO//2012/07/09 Meg

/**
    type of getting information from audio encoding codec library.
    format: ER (*GetInfo)(AUDCOENC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);
*/
typedef enum
{
    MEDIAENC_GETINFO_AUDENCPARAM = 0x1,  ///< aud encoding parmater. p1: MEDIAREC_AUDENC_PARAM pointer(out)
    MEDIAENC_GETINFO_AUDENCBLOCK = 0x2,  ///< bytes to encode one audio BS. p1: samples per block (out), ex. 1024 samples for AAC
    ENUM_DUMMY4WORD(AUDCOENC_GETINFO_TYPE)
} AUDCOENC_GETINFO_TYPE;

/**
    type of setting information to audio encoding codec library.
    format: ER (*SetInfo)(AUDCOENC_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);
*/
typedef enum
{
    MEDIAENC_SETINFO_AUDRAWADDR     = 0x1, ///< p1: address of input raw data(in)
    MEDIAENC_SETINFO_AUDSAMPLERATE  = 0x2, ///< p1: sampleRate(in)
    MEDIAENC_SETINFO_AUDBITRATE     = 0x3, ///< p1: audio encoding bit rate(in)
    MEDIAENC_SETINFO_AUDCHANNELS    = 0x4, ///< p1: encoding channels (in)
    MEDIAENC_SETINFO_AUDSAVETYPE    = 0x5, ///< p1: 1 one sec, 2 half sec
    MEDIAENC_SETINFO_AUD_AACSETCFG  = 0x81,///< set aac config, no parameters
    MEDIAENC_SETINFO_AUD_AACSTOPFREQ= 0x82,///< p1: aac stop frequency
    ENUM_DUMMY4WORD(AUDCOENC_SETINFO_TYPE)
} AUDCOENC_SETINFO_TYPE;

/**
    Audio encoding parameters structure for EncodeOne()
*/
typedef struct
{
    UINT32 encAddr;    ///< in:codec needed memory
    UINT32 encSize;    ///< in:size for codec memory
    UINT32 rawAddr;    ///< in:encoding audio raw address
    UINT32 sampleRate; ///< in:input sample rate
    UINT32 bitRate;    ///< in:wanted bit rate (bits per second)
    UINT32 nChannels;  ///< in:input channels
    UINT32 saveType;   ///< in:save type, 1 one sec, 2 half sec
    UINT32 chunksize;  ///< out: size of one second audio raw data (bytes)
    UINT32 thisSize;   ///< out: size of encoding frame (bytes)
    UINT32 usedRawSize;///< out: size of used raw data (bytes)
    UINT32 encOutSize; ///< out: if half sec, half chunksize
    UINT32 stopFreq;    ///< in: stop frequency
} MEDIAREC_AUDENC_PARAM;

typedef struct
{
    UINT32 bsOutputAddr; ///< in: bitstream output addr
    UINT32 inputAddr;    ///< in: raw input addr
    UINT32 inputSize;    ///< in: raw input size
    UINT32 thisSize;     ///< out: size of encoding frame (bytes)
    UINT32 usedRawSize;  ///< out: size of used raw data (bytes)

} AUD_CUSTOM_ENCLAST_INFO;//2012/07/09 Meg

/**
    Struture of audio encoder
*/
typedef struct
{
    ER (*Initialize)(void);  ///< initializing codec
    ER (*GetInfo)(AUDCOENC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);  ///< get information from codec library
    ER (*SetInfo)(AUDCOENC_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3); ///< set parameters to codec library
    ER (*EncodeOne)(UINT32 type, UINT32 outputAddr, UINT32 *pSize, MEDIAREC_AUDENC_PARAM *ptr); ///< encoding one audio frame
    ER (*CustomizeFunc)(UINT32 type, void *pobj); ///< codec customize function
    UINT32 checkID;     ///< check ID
} AUDIO_ENCODER, *PAUDIO_ENCODER;


/*
    Public funtions to get audio encoding object
*/
//@{
//extern PAUDIO_ENCODER MP_AACEnc_getEncodeObject(void);
//extern PAUDIO_ENCODER MP_PCMEnc_getEncodeObject(void);
//extern PAUDIO_ENCODER MP_AACEnc_getNoAlignEncodeObject(void);//2012/02/23 Meg

//@}
#endif

//@}
