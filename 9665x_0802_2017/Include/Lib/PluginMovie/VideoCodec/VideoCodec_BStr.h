/**
    Header file of video codec BStr library

    Exported header file of video codec library.

    @file       VideoCodecBStr.h
    @ingroup    mIAVCODEC
    @note       Nothing.
    @version    V1.01.003
    @date       2010/05/05

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _VIDEOCODECBSTR_H
#define _VIDEOCODECBSTR_H

#include "Type.h"
#include "VideoEncode.h"
#include "VideoDecode.h"

/*
    Public funtions to get video codec object
*/
//@{
extern PVIDEO_DECODER MP_BStrJpgDec_getVideoDecodeObject(void);
extern PVIDEO_ENCODER MP_BStrJpgEnc_getVideoEncodeObject(void);
extern PVIDEO_ENCODER MP_BStrH264Enc_getVideoEncodeObject(void);


//@}
#endif

//@}



