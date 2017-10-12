/**
    Header file for Audio Codec module

    This file is the header file that define the API and data type
    for Audio Codec module.

    @file       AudioCodec.h
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _AUDIOCODEC_H
#define _AUDIOCODEC_H

#include "Audio.h"

/**
    @addtogroup mISYSAud
*/
//@{

extern void     audExtCodec_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc);
extern void     audExtAIC3254_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc); // temp here
extern void     audExtCMedia_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc);
extern void     audExtALC5670_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc);

/**
    Get audio library version information

    Get audio library version information. Return pointer of 9 characters (include
    null terminated character). Example: 2.00.002

    @return Pointer of 8 characters of version information + null terminated character.
*/
extern CHAR     *audWM8978_getVerInfo(void);

/**
    Get audio library build date and time

    Get audio library build date and time. Return pointer of 22 characters (include
    null terminated character). Example: Mar 19 2009, 18:29:28

    @return Pointer of 21 characters of build date and time + null terminated character.
*/
extern CHAR     *audWM8978_getBuildDate(void);

//@}

#endif

