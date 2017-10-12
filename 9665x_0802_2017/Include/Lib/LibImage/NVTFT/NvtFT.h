#ifndef __NVTFT_H__
#define __NVTFT_H__

#include <Type.h>
#include <debug.h>
#include "fde_lib.h"
#include "GxImage.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    sSize imgSize;
    unsigned char* pData;
    int lineoffset;
}sMat;

extern void Trk_InitTracking( IMG_BUF *imgBuf, sMat *pSrcFrame, UINT32 *pBuff );
extern void Trk_GetTargetModel( FACE *pFace, UINT32 nFace, sMat *pFrm );
extern UINT32 Trk_TrackFaces( FACE *pFace, sMat *pFrm );

#ifdef __cplusplus
}
#endif

#endif
