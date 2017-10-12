
#ifndef _SCENE_RENDER_LIB_H_
#define _SCENE_RENDER_LIB_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "type.h"
#include "ErrorNo.h"
#include "debug.h"


typedef enum
{
  SR_INPUT_YCC_444  = 0,         ///< YCC444 separate planar format for input
  SR_INPUT_YCC_422  = 1,         ///< YCC422 separate planar format for input
  SR_INPUT_YCC_420  = 2,         ///< YCC420 separate planar format for input
  SR_INPUT_RESERVED = 3,         ///< reversed
  SR_INPUT_YCCP_422 = 4,         ///< YCC422 UV-packing format for input
  SR_INPUT_YCCP_420 = 5,         ///< YCC420 UV-packing format for input
  ENUM_DUMMY4WORD(SR_IMG_FORMAT_SEL)
} SR_IMG_FORMAT_SEL;



typedef enum
{
  HDR_LEVEL_1 = 0,
  HDR_LEVEL_2 = 1,
  HDR_LEVEL_3 = 2,
  HDR_LEVEL_4 = 3,
  HDR_LEVEL_5 = 4,
  HDR_LEVEL_6 = 5,
  HDR_LEVEL_7 = 6,
  HDR_LEVEL_8 = 7,
  HDR_LEVEL_MAX = 8,
  ENUM_DUMMY4WORD(HDR_LEVEL_SEL)
} HDR_LEVEL_SEL;


typedef enum
{
    WDR_LOCAL_STRENGTH_1  = 0,
    WDR_LOCAL_STRENGTH_2  = 1,
    WDR_LOCAL_STRENGTH_3  = 2,
    WDR_LOCAL_STRENGTH_4  = 3,
    WDR_LOCAL_STRENGTH_5  = 4,
    ENUM_DUMMY4WORD(WDR_LOCAL_STRENGTH_SEL)
} WDR_LOCAL_STRENGTH_SEL;


typedef struct _SceneRenderImageInfo
{
  UINT32 img_width;
  UINT32 img_height;
  UINT32 img_lineoffsetY;
  UINT32 img_lineoffsetC;
  SR_IMG_FORMAT_SEL  img_Fmt;
  UINT32 img_inAddrY;
  UINT32 img_inAddrCb;
  UINT32 img_inAddrCr;  // if input format is 3 planner
  UINT32 img_workStartAddr;
  UINT32 img_outAddrY;
  UINT32 img_outAddrCb;
  UINT32 img_outAddrCr;  // if output format is 3 planner
} SceneRenderImageInfo;

typedef struct _SceneRender_HDRInfo
{
  HDR_LEVEL_SEL hdrLevelSel;
  BOOL yFilterEn;
  BOOL cFilterEn;

  BOOL refAutoGen;
  UINT32 ref_width;        // if refAutoGen = TRUE to set
  UINT32 ref_height;       // if refAutoGen = TRUE to set
  UINT32 ref_lineoffset;   // if refAutoGen = TRUE to set
  UINT32 ref_addr;   // if refAutoGen = TRUE to set
} SceneRender_HDRInfo;


typedef struct _SceneRender_RTXGainInfo
{
    UINT32 uiGDGain;
    UINT32 uiGLGain;
    UINT32 uiLDGain;
    UINT32 uiLLGain;
    UINT32 uiRGain;
    UINT32 uiGGain;
    UINT32 uiBGain;
    UINT32 uiEGain;
} SceneRender_RTXGainInfo;


typedef struct _SceneRender_RTXCalInfo
{
    UINT16 *puiLuma;
    UINT32 uiLumaBit;
    UINT32 uiSizeH;
    UINT32 uiSizeV;
    UINT32 *puiIPEGamma;
    WDR_LOCAL_STRENGTH_SEL IsoIdx;
} SceneRender_RTXCalInfo;


extern BOOL SR_CalSceneRecognize(UINT32 uiWidth, UINT32 uiHeight, UINT32 uiLineoffset, UINT32 uiBufAddr);
extern UINT32 SR_CalWDRBufferSize(SceneRenderImageInfo *pImgInfo, SceneRender_HDRInfo *pWDRInfo);
extern ER SR_ProcessYccWDR(SceneRenderImageInfo *pImgInfo, SceneRender_HDRInfo *pWDRInfo);

extern UINT32 SR_CalLCABufferSize(SceneRenderImageInfo *pImgInfo);
extern ER SR_ProcessYccLCA(SceneRenderImageInfo *pImgInfo, UINT32 uiFilterTimes);


extern ER SR_CalRTXGain(SceneRender_RTXCalInfo *pCalRtxInfo, SceneRender_RTXGainInfo *pGetRtxGainInfo);

#endif




