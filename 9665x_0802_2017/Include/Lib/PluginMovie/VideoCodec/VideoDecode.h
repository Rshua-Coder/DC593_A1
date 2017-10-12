/**
    Header file of video decoding codec library

    Exported header file of video decoding codec library.

    @file       VideoDecode.h
    @ingroup    mIAVCODEC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _VIDEODECODE_H
#define _VIDEODECODE_H


#include <stdio.h>
#include <string.h>
#include "type.h"
#include "KERNEL.h"
/**
    @addtogroup mIAVCODEC
*/
//@{

//#NT#2012/04/06#Calvin Chang -begin
//Search Index table solution in Parse 1st video & pre-decode video frames for new H264 driver
/**
    @name Video pre-decode frame number definition

    Definition of video pre-decode frame buffer number.
*/
//@{
#define VIDDEC_PREDEC_FRMNUM    3           ///< Video Pre-decode Frame Number
//@}
//#NT#2012/04/06#Calvin Chang -end

/**
    @name type for CustomizeFunc() in VIDEO_DECODER

    type for CustomizeFunc() in VIDEO_DECODER.
*/
//@{
#define MEDIAVID_CUSTOM_DECODE1ST        0x01    ///< p1:(MEDIA_DSP_1STV *) object(out)
//#define MEDIAVID_CUSTOM_DECODE_SPECPIC   0x02    // no used
//@}

/**

    Type defintion struture of getting video decoding codec library information

    Type defintion struture of getting video decoding codec library information.

    @note format: ER (*GetInfo)(VIDCODEC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);
*/
typedef enum
{
    MEDIAVID_GETINFO_NOWYCBCR   =0x1,       ///< p1:y addr(out), p2:cb addr(out), p3: cr addr(out)
    MEDIAVID_GETINFO_PREVYCBCR  =0x2,       ///< p1:prev y addr(out), p2: prev cb addr(out), p3: prev cr addr(out)
    //#NT#2012/03/29#Calvin Chang -begin
    //#NT#Media Player for new H264 driver
    MEDIAVID_GETINFO_PREDECFRAMENUM = 0x03, ///< p1:Pre-Decode Frame Numeber(out)
    //#NT#2012/03/29#Calvin Chang -end
    //#NT#2013/03/25#Calvin Chang#Support speed up level (trigger video decode by timer) -begin
    MEDIAVID_GETINFO_IFREAMENUMINSEC = 0x04, ///< p1:I-frame numeber in sec(out)
    //#NT#2013/03/25#Calvin Chang -end
    MEDIAVID_GETINFO_CURRDISPFRMNUM = 0x05, ///< p1: Current display frame number(out)
    MEDIAVID_GETINFO_OUTPUTYCBCR,           ///< p1:y addr(out), p2:cb addr(out), p3: cr addr(out)
    ENUM_DUMMY4WORD(VIDCODEC_GETINFO_TYPE)
} VIDCODEC_GETINFO_TYPE;

/**
    Type defintion struture of setting video decoding codec library information

    Type defintion struture of setting information to video decoding codec decoding codec library.

    @note format: ER (*SetInfo)(UINT32 type, UINT32 param1, UINT32 param2, UINT32 param3);
*/
typedef enum
{
    MEDIAVID_SETINFO_264Addr  =0x81,          ///< p1:address for H264 description (in)
    MEDIAVID_SETINFO_264YAddr =0x82,          ///< p1:y addr(in), p2:UV addr(in)
    MEDIAVID_SETINFO_CLOSEBUS =0x83,          ///< No parameters, stop decoder.
    //MEDIAVID_SETINFO_264BBPLAY=0x84,           // No Used
    //MEDIAVID_SETINFO_RESETGOP =0x85,           // No Used
    //#NT#2012/03/29#Calvin Chang -begin
    //#NT#Media Player for new H264 driver
    MEDIAVID_SETINFO_FRAMERATE=0x86,          ///< p1:frame rate for reorder queue
    MEDIAENC_SETINFO_PREDECODE=0x87,          ///< p1:y memory address(in), p2:memeory size(in), p3: pre-decode frame number, pre-decode image for H.264 with B-frame
    MEDIAVID_SETINFO_PLAYCHANGE=0x88,         ///< No parameters, inform Media Plug-in the Display frame order is changed.
    MEDIAVID_SETINFO_DECODEONE_WAITDONE=0x89, ///< No parameters, decode One Frame and Wait Done
    MEDIAVID_SETINFO_REFRESH_IMGBUF=0x90,     ///< No parameters, refresh H264 Image Buffer Queue
    MEDIAVID_SETINFO_PREDECFRAMENUM=0x91,     ///< p1: pre decode frame number(in)
    //#NT#2012/03/29#Calvin Chang -end
    //#NT#2014/02/27#Calvin Chang#Raw Data Buffer handle by application/project -begin
    MEDIAVID_SETINFO_OUTPUTRAWMODE=0x92,      ///< p1:enable/disable output RAW mode
    MEDIAVID_SETINFO_RAWDATAADDR=0x93,        ///< p1:RAW Y addr(in), p2:RAW UV addr(in)
    //#NT#2014/02/27#Calvin Chang -end
    MEDIAVID_SETINFO_H264GOPNUM=0x94,         ///< p1:h264 GOP number(in)
    ENUM_DUMMY4WORD(VIDCODEC_SETINFO_TYPE)
} VIDCODEC_SETINFO_TYPE;


/**
    Type defintion struture of 1st video frame decode information

    Type defintion struture of 1st video frame decode information.

    @note used for Media Plug-in CustomizeFunc(MEDIAVID_CUSTOM_DECODE1ST)
*/
typedef struct
{
    UINT32 addr;                               ///< [in] Video frame addr
    UINT32 size;                               ///< [in] Video frame size
    UINT32 decodeBuf;                          ///< [in] Output raw addr
    UINT32 decodeBufSize;                      ///< [in] Max size for outputing raw
    UINT32 y_Addr;                             ///< [out] Output Y addr
    UINT32 cb_Addr;                            ///< [out] Output cb addr
    UINT32 cr_Addr;                            ///< [out] Output cr addr
    UINT32 width;                              ///< [out] Video frame width
    UINT32 height;                             ///< [out] Video frame height
    UINT32 h264DescAddr;                       ///< [in] H264 description addr
    UINT32 h264DescLen;                        ///< [in] H264 description length
    UINT32 jpgType;                            ///< [out] Jpg type, JPG_FMT_YUV420 or others
    //#NT#2012/04/06#Calvin Chang -begin
    //Search Index table solution in Parse 1st video & Pre-decode frames for new H264 driver
    UINT32 h264decfrmnum;                      ///< [in] Decode Frame numeber
    UINT32 h264frmaddr[VIDDEC_PREDEC_FRMNUM];  ///< [in] Video frames position
    UINT32 h264frmsize[VIDDEC_PREDEC_FRMNUM];  ///< [in] Video frames size
    //#NT#2012/04/06#Calvin Chang -end
} MEDIA_DSP_1STV;

/**
    Type defintion struture of video frame data information

    Type defintion struture of video frame data YCbCr buffer information.
*/
typedef struct
{
    UINT32 yAddr;   ///< Y address
    UINT32 cbAddr;  ///< CB address
    UINT32 crAddr;  ///< CR address
} VIDEO_YCBCRADDR;

/**
    Type defintion struture of video display setting information

    Type defintion struture of video display setting information.

    @note used for Initialize()
*/
typedef struct
{
    UINT32 firstFramePos;   ///<[in] First video frame position
    UINT32 firstFrameSize;  ///<[in] First video frame size
    UINT32 rawAddr;         ///<[in] After decoded, Y addr (cb, cr counted)
    UINT32 rawSize;         ///<[in] Size for YCbCr
    UINT32 rawWidth;        ///<[in] Raw width
    UINT32 rawHeight;       ///<[in] Raw height
    UINT32 rawType;         ///<[in] Raw type, (jdcfg.fileformat)
    UINT32 rawCbAddr;       ///<[out] After decoded, cb addr
    UINT32 rawCrAddr;       ///<[out] After decoded, cr addr
    UINT32 h264DescAddr;    ///<[in] H264 description addr
    UINT32 h264DescLen;     ///<[in] H264 description length
    UINT32 jpgType;         ///<[in] Jpg type
} VIDEO_DISPLAYINFO;

/*
typedef struct
{
    UINT32 wantNum;     //[in] wantNum, want starts from 1
    UINT32 bsAddr;      //[in] bitstream addr
    UINT32 bsSize;      //[in] bitstream totalsize
    UINT32 outYaddr;    //[in/out] y raw data output addr
    UINT32 ylineoft;    //[in] y lineoffset
    UINT32 height;      //[in] height
    UINT32 h264DescAddr;//[in] h264 desc addr
    UINT32 h264DescSize;//[in] h264 desc size
    UINT32 endRawAddr;  //[out] used raw end addr
    UINT32 outUVaddr;   //[out] uv raw data output addr
} MEDIA_DEC_SPECIFICPIC_INFO;
*/


/**
    Type defintion struture of video decoder object

    Type defintion struture of video decoder object functions.
*/
typedef struct
{
    ER (*Initialize)(UINT32 fileAddr, VIDEO_DISPLAYINFO *pobj);                                   ///< Initialize decoder
    ER (*GetInfo)(VIDCODEC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3); ///< Get information from decoder
    ER (*SetInfo)(VIDCODEC_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);       ///< Set information to decoder
    ER (*DecodeOne)(UINT32 type, UINT32 fileAddr, UINT32 p2, UINT32 *p3);                         ///< Decode one frame, p1: frame addr(in), p2: frame size(in), p3: frame number for H264 (in)
    ER (*WaitDecodeDone)(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3);                        ///< Wait decoding finished, p1: E_OK or not(out)
    ER (*CustomizeFunc)(UINT32 type, void *pobj);///< customize function, if needed
    UINT32 checkID;///< check ID for decoders
} VIDEO_DECODER, *PVIDEO_DECODER;


//@}

#endif //_VIDEODECODE_H

