/*******************************************************************************
Copyright(c) ArcSoft, All right reserved.

This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary
and confidential information.

The information and code contained in this file is only for authorized ArcSoft
employees to design, create, modify, or review.

DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER
AUTHORIZATION.

If you are not an intended recipient of this file, you must not copy,
distribute, modify, or take any action in reliance on it.

If you have received this file in error, please immediately notify ArcSoft and
permanently delete the original and any copy of any file and any printout
thereof.
*******************************************************************************/

#ifndef _ARC_FACETRACKING_H_
#define _ARC_FACETRACKING_H_

#include "amcomdef.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef MVoid*              AMFPAF_PIXELARRAY;
typedef MVoid**             AMFPAF_ENGINE;
typedef MLong               AMFPAF_PIXELARRAYFORMAT;
typedef MLong               AMFPAF_ORIENTPRI;
typedef MLong               AMFPAF_ORIENTCODE;
typedef MLong               AMFPAF_FACEOUTPRI;


typedef struct AMFPAF_InputFACE
{
    MUInt16  x;            // the x cooridnate of the top left of the bounding box of a face candidate
    MUInt16  y;            // the y cooridnate of the top left of the bounding box of a face candidate
    MUInt16  width;        // the width of the bounding box of a face candidate
    MUInt16  height;       // the height of the bounding box of a face candidate
    MUInt16  score;        // the score of a face candidate
    MUInt16  iFaceTag;     // the tag of a face candidate, 0 means frontal face. 1 means right profile face, 2 means left profile face.
} AMFPAF_InputFACE, *LPAMFPAF_pInputFACE;


// Color format definition
enum PixelArrayFormat{
    AMFPAF_PAF_GREY         = 0xc
};


typedef struct{
    MRECT           *rcFace;                // invalid
    MLong           nFace;                  // number of faces detected, if larger than 0, it means the candidate is face.
    MLong           lfaceOrient;            // invalid
} AMFPAF_FACERES, *LPAMFPAF_FACERES;


typedef struct{
    MLong               lWidth;             // image width
    MLong               lHeight;            // image height
    MLong               lPixelArrayFormat;  // Format of pixel array
    MLong               lLineBytes;         // Line width of image in bytes
} AMFPAF_IMAGEINFO, *LPAMFPAF_IMAGEINFO;

typedef struct
{
    MLong lCodebase;            // Codebase version number
    MLong lMajor;               // major version number
    MLong lMinor;               // minor version number
    MLong lBuild;               // Build version number, increasable only
    const MTChar *Version;      // version in string form
    const MTChar *BuildDate;    // latest build Date
    const MTChar *CopyRight;    // copyright
} ArcSoft_Face_Tracking_Version;

/************************************************************************
 * The function used to verify face candidates.
 ************************************************************************/

MRESULT AMFPAF_FaceVerification(            // return MOK if success, otherwise fail
     MHandle                hMemMgr,        // [in]  User defined memory manager
     AMFPAF_ENGINE          hEngine,        // [in]  Detect engine
     AMFPAF_PIXELARRAY      pImgData,       // [in]  The original image data
     LPAMFPAF_IMAGEINFO     pImgInfo,       // [in]  Pointing to the AMFPAF_IMAGEINFO structure containing input image information
     LPAMFPAF_FACERES       pFaceRes,       // [out] The detection result
     LPAMFPAF_pInputFACE    ifacetag        // [in]  Pointing to the AMFPAF_InputFACE structure containing input face candidate information
);
/************************************************************************
 * The function used to Initialize the face tracking engine.
 ************************************************************************/
MRESULT AMFPAF_InitialFaceEngine(
    MHandle             hMemMgr,        // [in] User defined memory manager
    LPAMFPAF_FACERES    pFaceRes,       // [out] The tracking result
    AMFPAF_ENGINE       *pEngine        // [out] pointing to the AMFPAF_ENGINE structure containing the tracking engine.
);

/************************************************************************
 * The function used to Uninitialize the detection module.
 ************************************************************************/
MRESULT AMFPAF_UninitialFaceEngine(
    MHandle hMemMgr,                    // [in] User defined memory manager
    AMFPAF_ENGINE *pEngine,             // [in] pointing to the AMFPAF_ENGINE structure containing the tracking engine.
    LPAMFPAF_FACERES pFaceRes           // [in] The tracking result
);

/************************************************************************
 * The function used to get version information of face tracking library.
 ************************************************************************/
//extern const ArcSoft_Face_Tracking_Version * ArcSoft_Face_Tracking_GetVersion();


#ifdef __cplusplus
}
#endif

#endif //_ARC_FACETRACKING_H_
