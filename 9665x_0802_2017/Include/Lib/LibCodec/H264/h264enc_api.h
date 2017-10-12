#ifndef _H264_API_H_
#define _H264_API_H_

#include "Type.h"
#include "h264_def.h"
#define H264ENC_IN_MAX          2   /*!<  maximum input source number for H.264 encoding simultaneously. */



typedef struct _H2643dnrCfg{
   //  Luma Config //
   UINT32 uiTY_Weight[3][3];    ///< Temporal Filter
   UINT32 uiRY_TH[3];           ///< Range Filter

   // Chroma Config //
   UINT32 uiTC_Weight[3][3];    ///< Temporal Filter
   UINT32 uiRC_TH[3];           ///< Range Filter

   // Clamp Config //
   UINT32 uiClampY_TH;
   UINT32 uiClampY_Div;
   UINT32 uiClampUV_TH;
   UINT32 uiClampUV_Div;
}H2643dnrCfg, *PH2643dnrCfg;

typedef struct _H264Enc3dnrCfg{
   BOOL b3dnrEn;
   //  Threshold and Weight Config //
   UINT32 uiTH[3];
   UINT32 uiTH_Weight[3];
   H2643dnrCfg s3dnrP2P;
   H2643dnrCfg s3dnrMCTF;
}H264Enc3dnrCfg, *PH264Enc3dnrCfg;


typedef struct _H264ENC_INIT
{
    UINT32          uiWidth;            ///< Original source width, ex: 1920 x 1080, width = 1920
    UINT32          uiHeight;           ///< Original source height, ex: 1920 x 1080, height = 1080 (not 1088)
    UINT32          uiSarWidth;         ///< Sample Aspect Ratio width, ex: 1920 x 1080, width = 1920
    UINT32          uiSarHeight;        ///< Sample Aspect Ratio height, ex: 1920 x 1080, height = 1080 (not 1088)
    UINT32          uiYLineOffset;      ///< Y line offset
    UINT32          uiUVLineOffset;     ///< UV line offset
    UINT32          uiBitRate;          ///< Bit rate (bits per second)
    UINT32          uiFrameRate;        ///< frame rate (frames per second * 1000)
    UINT32          uiEncBufAddr;       ///< H264 encoder internal buffer starting address
    UINT32          uiEncBufSize;       ///< H264 encoder internal buffer size
    PROFILE_TYPE    uiEncProfile;       ///< H.264 encoder profile (baseline, main, high profile)
    UINT32          uiEncGopFrameNum;   ///< H.264 encoder GOP frame number
//    H264Enc3dnrCfg  eEnc3dnrCfg;        ///< H.264 3dnr config
    UINT32          uiPFrmNum;          ///< H.264 P frame number in one Gop
    UINT32          uiBFrmNum;          ///< H.264 B frame number between I/P frame
    INT32           iRcMaxQp;           ///< H.264 Rate Control's max QP
    UINT8           ucIQP;              ///< H.264 Rate Control's Init IQP
    UINT8           ucPQP;              ///< H.264 Rate Control's Init PQP
    UINT8           ucBQP;              ///< H.264 Rate Control's Init BQP
    INT32           iQPTH;              ///< H.264 Rate Control's QP change Threshold
    UINT8           ucDisableDB;        ///< H.264 disable_filter_idc (0=Filter, 1=No Filter, 2=Slice Mode)
    INT8            cDBAlpha;           ///< H.264 deblocking Alpha & C0 offset div. 2, {-6, -5, ... 0, +1, .. +6}
    INT8            cDBBeta;            ///< H.264 deblocking Beta offset div. 2, {-6, -5, ... 0, +1, .. +6}
    UINT32          uiIPWeight;         ///< H.264 Rate Control's Init I_WEIGHT and P_WEIGHT
                                        ///<    (ex: I_WEIGHT=8,P_WEIGHT=2 => uiIPWeight = 0x00080002)
    UINT32          uiSVCFlag;          ///< H.264 SVC Flag, 0 : disable 1:enable
    UINT32          uiSVCLayer;         ///< H.264 SVC Layer, range : 1,2,3
    UINT32          ui2PMode;           ///< H.264 2P mode, 0 : normal, 1 : 2P (2P encode at the same time)
} H264ENC_INIT, *PH264ENC_INIT;


typedef struct
{
    UINT32      uiSrcYAddr[H264ENC_IN_MAX];     ///< Source Y address
    UINT32      uiSrcUVAddr[H264ENC_IN_MAX];    ///< Source UV address
    UINT32      uiBSAddr[H264ENC_IN_MAX];       ///< Encoded bit-stream address
    UINT32      uiBSSize[H264ENC_IN_MAX];       ///< Encoded bit-stream size
    UINT32      uiMaxBSLen;                     ///< Maximum bit-stream length
    SLICE_TYPE  uiEncFrameType;                 ///< Encode frame type (I, Idr, P, or B)
    UINT32      uiEncFrameNum;                  ///< Encode frame number (single, 2-B, ...)
    UINT32      uiSVCSize[H264ENC_IN_MAX];      ///< SVC header size
    UINT32      uiTemporalId[H264ENC_IN_MAX];
} H264ENC_INFO, *PH264ENC_INFO;

typedef struct
{
    UINT32         uiEncId;
    void           *pContext;
} H264ENC_VAR,     *PH264ENC_VAR;


extern INT32 h264Enc_initEncoder (PH264ENC_INIT pH264EncInit,H264ENC_VAR *pH264EncVar);
extern INT32 h264Enc_getH264Header(UINT32 *puiAddr, UINT32 *puiLen,H264ENC_VAR *pH264EncVar);
extern INT32 h264Enc_encodeOnePicture(PH264ENC_INFO pH264EncInfo,H264ENC_VAR *pH264EncVar);
extern INT32 h264Enc_IsEncodeFinished(H264ENC_VAR *pH264EncVar);
extern INT32 h264Enc_getResult(PH264ENC_INFO pH264EncInfo,H264ENC_VAR *pH264EncVar);
extern void h264Enc_stopEncoder(H264ENC_VAR *pH264EncVar);
extern INT32 h264Enc_setup3DNR(PH264Enc3dnrCfg  pEnc3dnrCfg,H264ENC_VAR *pH264EncVar);
extern INT32 h264Enc_waitEncodeFinished(H264ENC_VAR *pH264EncVar);
extern UINT32 h264Enc_AskEncodeFinished(H264ENC_VAR *pH264EncVar);
extern void h264Enc_resetHW(void);
extern UINT32 h264Enc_getminmem(UINT32 wid, UINT32 hei);

#endif

