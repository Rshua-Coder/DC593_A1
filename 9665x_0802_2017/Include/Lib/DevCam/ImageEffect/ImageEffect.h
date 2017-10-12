/**
    Image effect module(support image processing like Toy photo, fish eye, ...etc.)


    @file       ImageEffect.h
    @ingroup    mILibIE
    @note

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _IMAGEEFFECT_H_
#define _IMAGEEFFECT_H_
#include "Type.h"
#include "ipl_utility.h"
/** \addtogroup mILibIE */
//@{
typedef void (*FP_IECB)(UINT32);

typedef enum _IE_INDEX
{
    IE_INDEX_NONE       = 0,
    IE_INDEX_SHADING    = 1,    ///< access Y domain only
    IE_INDEX_FANTASY    = 2,
    IE_INDEX_DISTORTION = 3,
    IE_INDEX_MIRROR     = 4,
    IE_INDEX_ROCK       = 5,    ///< Rock mode, but Y,Cb,Cr are all binarized.
    IE_INDEX_SKETCH     = 6,
    IE_INDEX_MINIATURE  = 7,
    IE_INDEX_SPARKLE    = 8,
    IE_INDEX_MAX,
    ENUM_DUMMY4WORD(IE_INDEX)
}IE_INDEX;

typedef struct  _IE_Dither
{
    BOOL bEn;
}IE_Dither;

typedef IPL_YCC_CH_INFO IE_IMG_SIZE;

typedef struct
{
    UINT32      Addr;
    IE_IMG_SIZE Size;
}IE_IMG_PLAN;

typedef struct
{
    IE_IMG_PLAN         Y;
    UINT32              UVAddr;
    UINT32              UVLineofs;
    IPL_YUV_IMG_TYPE    Fmt;
}IE_PATTERN;

typedef struct  _IE_Synthesize
{
    enum
    {
        _blendEn,       ///< alpha blending
        _pblendEn,      ///< plane blending
        _multipleEn,    ///< A * B >> bit[2,0]
        ENUM_DUMMY4WORD(Syn_method)
    }Syn_method;
    union
    {
        struct
        {
            UINT32 WA;
            UINT32 WB;
            UINT32 Thres;
        }Blending;
        struct
        {
            FP_IECB     fpPlaneGen;     ///< use fw to create plane pattern, if fpPlaneGen == NULL, use PatImg
            IE_IMG_PLAN PatImg;         ///< existed plane
            UINT32      PatImg_CCAdr[2];   ///< CC plane starting address
        }PlaneBlending;
        struct
        {
            UINT32 Div;                 ///< range:0~7
        }Multipling;
    }Syn_par;
}IE_Synthesize;

typedef struct
{
    UINT32  Thres;  ///< if input > Thres, input = Hit. otherwise, input = NonHit;
    UINT32  Hit;    ///< -
    UINT32  NonHit; ///< -
}IE_BINARIZE;
//////////////////////////////////////////////////////////////////////////////////////////////////////
/**
    Shading, map to IE_INDEX_SHADING
*/
typedef struct _IE_SHADING
{
    BOOL            HWEn;           ///< using Grph if enable, otherwise, using CPU to replace Grph
    IE_IMG_PLAN     SrcImg;         ///< source image
    UINT32          UVAdr;          ///< UV address
    UINT32          CCLineofs;      ///< lineofs
    IPL_YUV_IMG_TYPE    Fmt;        ///< YCC format
    struct
    {
        FP_IECB     fpShadingGen;   ///< use fw to create shading pattern, if fpShadingGen == NULL, use PatImg
        IE_IMG_PLAN PatImg;         ///< existed shading pattern
    }ShadingImg;                    ///< shading pattern
    IE_Synthesize   SynMethod;      ///< synthesis method of Shading pattern on SrcImg
    IE_Dither       Dithering;      ///< -
    UINT32          WorkingBufAdr;     ///< working buffer for Shading
    UINT32          DestinationBufAdr;  ///< output result buffer address
    UINT32          DestinationUVAdr;  ///< output result buffer address
}IE_SHADING;
/**
    Fantasy, map to IE_INDEX_FANTASY
*/
typedef struct _IE_FANTASY
{
    BOOL            HWEn;
    struct
    {
        IE_IMG_PLAN     Y;          ///< Y image
        UINT32          CCLineofs;
        UINT32          CbAdr;         ///< Cb image, size h/w can be ignored ,excluding lineoffset
        UINT32          CrAdr;         ///< Cr image, size h/w can be ignored ,excluding lineoffset
        IPL_YUV_IMG_TYPE    Fmt;    ///< Ycc format
    }FantasySrc;                    ///< input image
    UINT32*         pLumi_Curve;    ///< gamma curve, NULL if it's not needed,  talbe tab num:65, Data type:UINT8
    UINT32          BlurRatio;      ///< control blur level of the BlurImg, range:1 ~ 3
    IE_Synthesize   SynMethod;      ///< synthesis method SrcImg and BlurImg
    UINT32          WorkingBufAdr;     ///< working buffer for Shading
    UINT32          DestinationBufAdr;  ///< output result buffer address
    UINT32          DestinationCbAdr;  ///< output result buffer address
    UINT32          DestinationCrAdr;  ///< output result buffer address
}IE_FANTASY;
/**
    Distortion, map to IE_INDEX_DISTORTION
*/
typedef struct _IE_DISTORTION
{
    struct
    {
        IE_IMG_PLAN     Y;          ///< Y image
        IE_IMG_PLAN     UV;         ///< uv image, size h/w can be ignored ,excluding lineoffset
        IPL_YUV_IMG_TYPE    Fmt;    ///< Ycc format
    }DistortionSrc;                 ///< input image
    enum
    {
        _IE_barrel      = 0,
        _IE_pincushion  = 1,
        _IE_perspective = 2,
        ENUM_DUMMY4WORD(DistortionType)
    }DistortionType;
    UINT32* pDistortionTbl;         ///< distortion table pointer
    UINT32  DestinationBufAdr;      ///< output result buffer address
    UINT32          DestinationUVAdr;  ///< output result buffer address
}IE_DISTORTION;
/**
    MIRROR, map to IE_INDEX_MIRROR
*/
typedef struct _IE_MIRROR
{
    BOOL        HWEn;
    struct
    {
        IE_IMG_PLAN     Y;          ///< Y image
        IE_IMG_PLAN     UV;         ///< uv image, size h/w can be ignored ,excluding lineoffset
        IPL_YUV_IMG_TYPE    Fmt;    ///< Ycc format
    }MirrorSrc;
    UINT32  MirrorLine_Position;    ///< range: 50 ~ 99;
    enum
    {
        _IE_Mirror_H    = 0,        ///< mirrorline in Horn. direction
        _IE_Mirror_V    = 1,        ///< mirrorline in Ver. direction
        ENUM_DUMMY4WORD(MirrorLine_Dir)
    }MirrorLine_Dir;                ///< -
    UINT32  DestinationBufAdr;      ///< output result buffer address, if DestinationBufAdr == 0, processing on SrcImg
    UINT32  DestinationUVAdr;  ///< output result buffer address
}IE_MIRROR;
/**
     Rock, map to IE_INDEX_ROCK
*/
typedef struct _IE_ROCK
{
    struct
    {
        IE_IMG_PLAN     Y;          ///< Y image
        IE_IMG_PLAN     UV;         ///< uv image, size h/w can be ignored ,excluding lineoffset
        IPL_YUV_IMG_TYPE    Fmt;    ///< Ycc format
    }RockSrc;                       ///< -
    IE_BINARIZE Y;                  ///< Y luminance thres
    UINT32      FixCb;              ///< if FixCb and FixCr are both 0, skip Cb Cr processin.
    UINT32      FixCr;              ///< -
    UINT32  DestinationBufAdr;      ///< output result buffer address, if DestinationBufAdr == 0, processing on SrcImg
    UINT32  DestinationUVAdr;  ///< output result buffer address
}IE_ROCK;
/**
    Sketch, map to IE_INDEX_SKETCH
*/

typedef struct  _IE_SKETCH
{
    struct
    {
        IE_IMG_PLAN     Y;          ///< Y image
        IE_IMG_PLAN     UV;         ///< uv image, size h/w can be ignored ,excluding lineoffset
        IPL_YUV_IMG_TYPE    Fmt;    ///< Ycc format
    }SketchSrc;                     ///< -
    IE_BINARIZE Y;                  ///< Y luminance threshold, if(input > Y.Thres) && (Y_edge > Y_Edge_Thres), input == Y.Hit. otherwise,input == Y.NonHit.
    enum
    {
        _SketchKernel3x3,
        _SketchKernel5x5,
        _SketchKernel7x7,
        ENUM_DUMMY4WORD(SketchKernel)
    }SketchKernel;
    UINT32  Y_Edge_Thres;           ///< Y edge threshold
    UINT32  Edge_enhance;           ///< Legal range: 0~2, 1 is contrast no enhancement, 2:enhance to maximum, 0: reduce to minimum.
    UINT32  Contrast;               ///< legal range: 0~10, 5 is contrast no enhancement, 10:enhance to maximum, 1: reduce to minimum.
    UINT32  FixCb;                  ///< if FixCb and FixCr are both 0, skip Cb Cr processin.
    UINT32  FixCr;                  ///< -
    UINT32  DestinationBufAdr;      ///< output result buffer address, if DestinationBufAdr == 0, processing on SrcImg
    UINT32  DestinationUVAdr;       ///< output result buffer address
}IE_SKETCH;
/**
    Sparkle, map to IE_INDEX_SPARKLE
*/
typedef struct _IE_SPARKLE
{
    struct
    {
        IE_IMG_PLAN     Y;          ///< Y image
        IE_IMG_PLAN     UV;         ///< uv image, size h/w can be ignored ,excluding lineoffset
        IPL_YUV_IMG_TYPE    Fmt;    ///< Ycc format
    }SparkleSrc;
    UINT32  ScaleDownRatio;         ///< scaling source image down, inorder to speed up.
    struct
    {
        FP_IECB         fpPatGen;       ///< use fw to create pattern, if fpPlaneGen == NULL, use PatImg
        IE_PATTERN*     pPatImg;        ///< sparkle pattern, (tab num : PatNum)
        UINT8           PatNum;
        UINT8*          pSparkleMax;    ///< percentage of how many candidates will be pasted a pattern on. (tab num: PatNum)
    }SparklePat;
    struct
    {
        //IRECT       *pTbl;          ///< extra block regions, NULL if no need.
        struct
        {
            BOOL    bEn;            ///< enable faceDetection
            UINT32  EnlargeFaceRect;///< enlarge face rectangle ,   unit: percentage
        }FaceDet;                   ///< enable face detection and pattern won't stamp on face position.
    }stampBlock;                    ///< pattern won't be stamp on these blocks
    UINT32 WorkingBufAdr;
    UINT32 DestinationBufAdr;       ///< output result buffer address, if DestinationBufAdr == 0, processing on SrcImg
    UINT32          DestinationUVAdr;  ///< output result buffer address
    UINT8           EdgeThres;      ///< edge threshold, equation plz ref. below
    UINT8*          pYThresTbl;     ///< lumi threshold(lumi need to be increment), tab num: PatNum.
                                    ///< if (in_edge > EdgeThres) && (in_Y > pYThresTbl[i]) && (in_Y < pYThresTbl[i+1])
                                    ///< paste pPatImg[i] on
}IE_SPARKLE;

typedef union _IE_Scenario
{
    IE_SHADING      Shading;
    IE_FANTASY      Fantasy;
    IE_DISTORTION   Distortion;
    IE_MIRROR       Mirror;
    IE_ROCK         Rock;
    IE_SKETCH       Sketch;
    IE_FANTASY      Miniature;
    IE_SPARKLE      Sparkle;
}IE_Scenario,*pIE_Scenario;
//////////////////////////////////////////////////////////////////////////////////////////////////////
extern ER IE_Query(IE_INDEX IE_idx, pIE_Scenario pScenario,UINT32* pWorkingBuf);
extern ER IE_Open(IE_INDEX IE_idx,FP_IECB fpCb);
extern ER IE_SetMode(IE_INDEX IE_idx,pIE_Scenario pScenario);
extern ER IE_Start(IE_INDEX IE_idx);
//extern ER IE_WaitFinish(IE_INDEX IE_idx);
extern ER IE_Close(IE_INDEX IE_idx);

//install OS id
extern void IE_InstallID(void) _SECTION(".kercfg_text");
//test cmd
extern void IE_RegCmdTab(void);
//@}
#endif  //_IMAGEEFFECT_H_