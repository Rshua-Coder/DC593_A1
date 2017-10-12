/**
    Header file of video codec MJPG library

    Exported header file of video codec library.

    @file       VideoCodecMJPG.h
    @ingroup    mIAVCODEC
    @note       Nothing.
    @version    V1.01.003
    @date       2010/05/05

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _VIDEOCODECMJPG_H
#define _VIDEOCODECMJPG_H

#include "Type.h"
#include "VideoEncode.h"
#include "VideoDecode.h"

/*
    Public funtions to get video codec object
*/
//@{
extern PVIDEO_ENCODER MP_MjpgEnc_getVideoEncodeObject(void);
extern PVIDEO_DECODER MP_MjpgDec_getVideoDecodeObject(void);
extern PVIDEO_ENCODER MP_MjpgEnc_getVideoObject(VIDENC_ID VidEncId);

//@}
#endif

