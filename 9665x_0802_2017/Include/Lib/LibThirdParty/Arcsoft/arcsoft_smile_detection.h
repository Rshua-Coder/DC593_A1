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


/******************************************************************************
 * In order to detailedly understand "asvloffscreen.h", which defines ASVLOFFSCREEN structure
 * and the general color formats, please refer to the corresponding document. In the current
 * smile detection version, only "ASVL_PAF_RGB24_B8G8R8", "ASVL_PAF_RGB16_B5G6R5", "ASVL_PAF_YVYU",
 * "ASVL_PAF_YUYV", "ASVL_PAF_I420", "ASVL_PAF_RGB16_R5G6B5", "ASVL_PAF_UYVY", "ASVL_PAF_P8_YUYV",
 * "ASVL_PAF_NV12", "ASVL_PAF_NV21", "ASVL_PAF_GRAY", "ASVL_PAF_I422H", "ASVL_PAF_VYUY", "ASVL_PAF_YV12",
 * "ASVL_PAF_LPI422H" and "ASVL_PAF_RGB32_B8G8R8A8" are supported.
 *
********************************************************************************/

#ifndef _ASSD_SMILEDETECTION_
#define _ASSD_SMILEDETECTION_

#include "amcomdef.h"
#include "asvloffscreen.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef MVoid*          ASSD_ENGINE;

#define ASSD_CALLBACK_USER_ABORT        0x1
#define MERR_FACE_TOOSMALL              0x7000
#define MERR_CONFIG_MISMATCH            0x8000
#define MERR_ENGINE_NONZERO             0x9000

/* Face orientation code */
enum ASSD_ORIENTCODE{
    ASSD_FOC_0          = 0x1,      /* 0 degree */
    ASSD_FOC_90         = 0x2,      /* 90 degree */
    ASSD_FOC_270        = 0x3,      /* 270 degree */
    ASSD_FOC_180        = 0x4,      /* 180 degree */
    ASSD_FOC_30         = 0x5,      /* 30 degree*/
    ASSD_FOC_60         = 0x6,      /* 60 degree*/
    ASSD_FOC_120        = 0x7,      /* 120 degree*/
    ASSD_FOC_150        = 0x8,      /* 150 degree*/
    ASSD_FOC_210        = 0x9,      /* 210 degree*/
    ASSD_FOC_240        = 0xa,      /* 240 degree*/
    ASSD_FOC_300        = 0xb,      /* 300 degree*/
    ASSD_FOC_330        = 0xc       /* 330 degree*/
};

/* Enable and disable flag */
enum ASSD_MASKFLAG{
    ASSD_DISABLE    = 0x0,          /* Disable the procedure */
    ASSD_ENABLE     = 0x1           /* Enable the procedure*/
};

/* Smile sensitivity */
enum ASSD_SMILESENSITIVITY{
    ASSD_SENSELEVEL0 = 0x0,     /*highest sensitivity*/
    ASSD_SENSELEVEL1 = 0x1,     /*higher sensitivity(recommend)*/
    ASSD_SENSELEVEL2 = 0x2,     /*middle sensitivity*/
    ASSD_SENSELEVEL3 = 0x3,     /*lower sensitivity*/
    ASSD_SENSELEVEL4 = 0x4      /*lowest sensitivity*/
};
// configure options. In this version, "lSize", "lSmileLevel" and "lKidPriorityFlag" must be configured by users.
typedef struct{
    MLong   lSize;              /* The size of this struct, equal to sizeof(ASSD_CONFIGOPTIONS) */
    MLong   lSmileLevel;        /* User defined smile sensitivity level, can be set as one item of ASSD_SMILESENSITIVITY*/
    MLong   lKidPriorityFlag;   /* Kid priority , can be set as one item of ASSD_MASKFLAG*/
}ASSD_CONFIGOPTIONS, *LPASSD_CONFIGOPTIONS;

/* The information of input faces */
typedef struct{
    MRECT               *pFaceRectArray;        /* The bounding box of face*/
    MLong               *pFaceOrientArray;      /* The orientation of face, can be set as one item of ASSD_ORIENTCODE */
    MLong               lFaceNumber;            /* number of faces detected*/
}ASSD_FACEINPUT, *LPASSD_FACEINPUT;

/* The results of smile detection */
typedef struct{
    MRECT           *pSmileRectArray;   /*The smile bounding box array with same length as pFaceRectArray in ASSD_FACEINPUT.Only rect information with flag equal to "1" is valid.*/
    MLong           *pFlagArrary;       /*The detection result flag array with same length as pFaceRectArray in ASSD_FACEINPUT."1" representing smile is detected, otherwise the flag is set to "0" */
    MLong           lFaceNumber;        /*The length of pSmileRectArray in RECT unit.*/
    MLong           lSmileNumber;       /*The number of smiles totally detected.*/
}ASSD_SMILEDETECTIONRESULT, *LPASSD_SMILEDETECTIONRESULT;

/* The definition of detection version */
typedef struct
{
    MLong lCodebase;            /* Codebase version number */
    MLong lMajor;               /* major version number */
    MLong lMinor;               /* minor version number*/
    MLong lBuild;               /* Build version number, increasable only*/
    MTChar Version[20];         /* version in string form */
    MTChar BuildDate[20];       /* latest build Date */
    MTChar CopyRight[50];       /* copyright */
} ArcSoft_Smile_Detection_Version;

/************************************************************************
 * This function is implemented by the caller, registered with
 * any time-consuming processing functions, and will be called
 * periodically during processing so the caller application can
 * obtain the operation status (i.e., to draw a progress bar),
 * as well as determine whether the operation should be canceled or not
 ************************************************************************/
typedef MRESULT (*ASSD_FNPROGRESS) (
    MVoid       *pParam1,               /*[in] Not used*/
    MVoid       *pParam2                /*[in] Caller-defined data*/
);

/************************************************************************
 * The function is used to Initialize the smile detection module.
 * The handle will be stored in detection interface.
 ************************************************************************/
MRESULT ASSD_InitSmileEngine(               /* return MOK if success, otherwise fail*/
    MHandle                     hMemMgr,    /* [in]  The handle of memory manager*/
    ASSD_ENGINE                 *pEngine,   /* [out]  Pointer pointing to a smile detection engine*/
    LPASSD_CONFIGOPTIONS        pConfig     /* [in]  Configure some parameters*/
);

/************************************************************************
 * The function is used to detect smile on preview mode automatically.
 ************************************************************************/
MRESULT ASSD_SmileDetect(                       /* return MOK if success, otherwise fail*/
    ASSD_ENGINE                 Engine,         /* [in]  Detect engine*/
    LPASVLOFFSCREEN             pImgInfo,       /* [in]  The original image data*/
    LPASSD_FACEINPUT            pFaceRes,       /* [in]  The face position*/
    LPASSD_SMILEDETECTIONRESULT pSmileRes,      /* [out] The detection result*/
    ASSD_FNPROGRESS             fnCallback,     /* [in]  The callback function*/
    MVoid                       *pParam         /* [in]  Caller-specific data that will be passed into the callback function*/
);

/************************************************************************
 * The function is used to detect smile on static image input mode  automatically.
 ************************************************************************/
MRESULT ASSD_SmileDetect_STATICIMG(             /* return MOK if success, otherwise fail*/
    ASSD_ENGINE                 Engine,         /* [in]  Detect engine*/
    LPASVLOFFSCREEN             pImgInfo,       /* [in]  The original image data*/
    LPASSD_FACEINPUT            pFaceRes,       /* [in]  The face position*/
    LPASSD_SMILEDETECTIONRESULT pSmileRes,      /* [out] The detection result*/
    ASSD_FNPROGRESS             fnCallback,     /* [in]  The callback function*/
    MVoid                       *pParam         /* [in]  Caller-specific data that will be passed into the callback function*/
);

/************************************************************************
 * The function is used to release the smile detection module.
 ************************************************************************/
MRESULT ASSD_UninitSmileEngine(     /* return MOK if success, otherwise fail*/
    ASSD_ENGINE         Engine      /* [in]  Pointer pointing to an ASSD_ENGINE structure containing the data of smile detection engine*/
);

/************************************************************************
 * The function used to get version information of smile detection library.
 ************************************************************************/
//const ArcSoft_Smile_Detection_Version * ArcSoft_Smile_Detection_GetVersion();

#ifdef __cplusplus
}
#endif

#endif
