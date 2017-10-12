     /*----------------------------------------------------------------------------------------------
*
* This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary and
* confidential information.
*
* The information and code contained in this file is only for authorized ArcSoft employees
* to design, create, modify, or review.
*
* DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER AUTHORIZATION.
*
* If you are not an intended recipient of this file, you must not copy, distribute, modify,
* or take any action in reliance on it.
*
* If you have received this file in error, please immediately notify ArcSoft and
* permanently delete the original and any copy of any file and any printout thereof.
*
*-------------------------------------------------------------------------------------------------*/

/*************************************************************************************************
**
**  Copyright (c) 2007 by ArcSoft Inc.
**
**  Name            : Arc_autoredeye.h
**
**  Purpose         : A red-eye removal interface.
**
**  Additional      :
**
**------------------------------------------------------------------------------------------------
**
**  Maintenance History:
**
**************************************************************************************************/

#ifndef __ARCSOFT_RED_EYE_REMOVE_H__
#define __ARCSOFT_RED_EYE_REMOVE_H__

#include "amcomdef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ARER_OK                     (0)
#define ARER_ERR_PARAMETER          (-2)
#define ARER_ERR_NOT_SUPPORT        (-3)
#define ARER_ERR_MEMORY             (-4)
#define ARER_ERR_INTERRUPT          (-7)
#define ARER_ERR_TIMEOUT            (-8)

#define ARER_PAF_BGR888             (0)
#define ARER_PAF_RGB888             (1)
#define ARER_PAF_RGB565             (8)
#define ARER_PAF_YYCbCr8888         (12)
#define ARER_PAF_YCbYCr8888         (13)
#define ARER_PAF_CbYCrY8888         (14)
#define ARER_PAF_YCbCr444_P         (51)
#define ARER_PAF_YCbCr422_P         (52)
#define ARER_PAF_YCbCr420_P         (53)
#define ARER_PAF_YCbCr422_YPUV      (54)
#define ARER_PAF_YCBCR_422VD        (55)
#define ARER_PAF_YCbCr422_P8        (56)
#define ARER_PAF_YCbCr420_YPVU      (57)
#define ARER_PAF_YCbCr420_YPUV      (58)


#define ARER_DETECT_ONLYFACE        (0)
#define ARER_DETECT_SIMPLE          (1)
#define ARER_DETECT_BALANCED        (2)
#define ARER_DETECT_FULL            (3)




typedef struct _tag_ARER_Version
{
    MLong lMajor;           /* The major version ID. */
    MLong lMinor;           /* The minor version ID. */
    MLong lBuild;           /* The build ID, platform dependent. */
    MLong lRevision;        /* The revision ID, increasable only. */
    MTChar *Version;        /* Version in string format. */
    MTChar *BuildDate;      /* The latest build date. */
    MTChar *CopyRight;      /* copyright */
} ARER_VERSION;


typedef struct _tag_ARER_OFFSCREEN {
    MLong       lWidth;                 /* Off-screen width */
    MLong       lHeight;                /* Off-screen height */
    MLong       lPixelArrayFormat;      /* Format of pixel array */
    union
    {
        struct
        {
            MLong lLineBytes;           /* Line length of off-screen in bytes for chunky data format */
            MByte *pPixel;              /* Off-screen pixel array in chunky */
        } chunky;
        struct
        {
            MLong lLinebytesArray[4];   /* Line length of off-screen in bytes for planar data format */
            MByte *pPixelArray[4];      /* Off-screen pixel array in planar */
        } planar;
    } pixelArray;
} ARER_OFFSCREEN, * LPARER_OFFSCREEN;


typedef struct _tag_ARER_IMAGEATTRIB
{
    MUInt32 ulFlashStatus;              /* Whether the flash is open or not.  1 if open and 0 if close or unknown. Reserved. */
    MUInt32 ulFlashStrength;            /* The flash strength in GN (Guide Number).  0 if unknown. Reserved. */
    MUInt32 ulFocalLength;              /* The focal length of the lens, in 0.1 of mm.  0 if unknown and 0xFFFFFFFF if infinite. */
    MUInt32 ulFov;                      /* The field of view, in 0.1 degree.  0 if unknown. */
    MUInt32 ulISO;                      /* The ISO value when taking photo.  0 if unknown. */
    MInt32  lExposureValue;             /* The exposure value herein means exposure compensation; it is adjustment in 1/3EV.  For example, -1 stands for -1/3EV, 3 stands for 1EV, and so on. 0 if unknown or no exposure compensation. */
    MUInt32 ulSubjectDistance;          /* The distance to the subject, in cm.  0 if unknown and 0xFFFFFFFF if infinite. */
    MUInt32 ulEyeSize;                  /* The Minimal pupil size in pixel(3.14*Radius*Radius).  0 if detect all eyes. */
    MUInt32 ulOrientation;              /* The orientation.  0 if unknown.  Reserved. */
    MUInt32 ulOrgWidth;                 /* Image width of the original image. 0 if unknown. */
    MUInt32 ulOrgHeight;                /* Image height of the original image. 0 if unknown. */
    MPOINT  ptOffset;                   /* Offset from left-top corner of original image.  Reserved. */
} ARER_IMAGEATTRIB, *LPARER_IMAGEATTRIB;

typedef struct _tag_ARER_EYEINFO
{
    MLong   lCx;                        /* X-coordination of eye position (in pixels). */
    MLong   lCy;                        /* Y-coordination of eye position (in pixels). */
    MLong   lRadius;                    /* Radius of eye (in pixels). */
    MLong   lDarkness;                  /* Correction darken strength, range from 0  to 100. For example, 50 means 50% darken strength. */
    MLong   lAggressive;                /* Correction aggressiveness strength, range from 0  to 200. For example, 100 means normal aggressiveness (base, 100%), 150 means 150% aggressiveness of the base. */
    MDWord  dwAttrib[19];               /* Reserved for red-eye correction. */
} EYEINFO, *LPEYEINFO, ARER_EYEINFO, *LPARER_EYEINFO;

typedef struct _tag_ARER_FACEINFO
{
    MRECT       rcFace;                 /*Face boundary.*/
    MUInt32     ulOrient;               /*Face orientation, 0 if unknown. Reserved.*/
    MUInt32     ulType;                 /*Reserved.*/
} ARER_FACEINFO, *LPARER_FACEINFO;

typedef struct _tag_ARER_MEMBLOCK
{
    MVoid       *pMemBlock;             /*The address of working memory.*/
    MUInt32     ulMemBlockSize;         /*The size of working memory.*/
    MUInt32     ulMemType;              /*The type of working memory. 0 means the fastest one, 1 means the secondly fast one and so on.*/
} ARER_MEMBLOCK, *LPARER_MEMBLOCK;


typedef struct _tag_ARER_MEMHANDLE
{
    MLong           lMemBlockNum;       /*The number of available working memory block. 4 is the maximum capacity.*/
    ARER_MEMBLOCK   tMemBlock[4];       /*Available working memory block.*/
} ARER_MEMHANDLE, *LPARER_MEMHANDLE;



/************************************************************************
* Description   : This function is implemented by the caller, registered with
*                 any time-consuming processing functions, and will be called
*                 periodically during processing so the caller application can
*                 obtain the operation status (i.e., to draw a progress bar),
*                 as well as determine whether the operation should be canceled or not.
* Parameters    :
*                 lProgress [in]:
*                       The percentage of the current operation. 10 means 10%, and so on.
*                 pUserParam    [in]:
*                       Caller-defined data. It should be registered when calling
*                       the process functions and will be passed to the callback
*                       without modification. The library itself does not use this data at all.
* Return        :
*                 ARER_OK
*                       Everything is OK. Continue further processing.
*                 ARER_ERR_TIMEOUT
*                       Stop further process after callback returns and return the result of all previous work.
*                 ARER_ERR_INTERRUPT
*                       Direct the process should be canceled after callback returns.
************************************************************************/
typedef MRESULT (*ARER_FNPROGRESS) (
                                    MLong lProgress,    /* The percentage of the current operation. 10 means 10%, and so on. */
                                    MVoid* pUserParam   /* Caller-defined data. */
);


/*******************************************Routine*Public*****************************************\
* ARER_GetVersion
* Description   : This function tells the SDK version by returning the reference of ARER_VERSION.
* Parameters    :
* Return        : The address of ARER_VERSION reference.
* Remarks       : N/A
*
\**************************************************************************************************/
const ARER_VERSION *ARER_GetVersion (MVoid);





/*******************************************Routine*Public*****************************************\
* ARER_AutoDetect
* Description   : The function automatically detects red-eyes in the input image and outputs eye
*                 information.
* Parameters    :
*   pInputImg       [in]:
*       The memory storing the input image. The library will detect the red-eye on this image buffer.
*   pImgAttrib      [in]:
*       The additional image information for correction. Including the original image size before
*       scaling, focal length, flash strength, etc. NULL if unavailable.
*   pMemHandle      [in]:
*       The working buffer for the library to do red-eye detection/correction processing. The caller
*       is responsible for allocating and freeing the memory.
*   pEyeInfo        [out]:
*       Pointer to the address of eye information array.
*   ulMaxCapacity   [in]:
*       Maximum capacity of eye information array.
*   fnCallback      [in]:
*       The CALLBACK function, which is used for interruption when accessing time limitation reached.
*       It could be NULL.
*   pUserParam      [in]:
*       Caller-specific data that will be passed into the callback function. It could be NULL.
*   pFaces          [in]:
*       The input faces region. NULL if unavailable.
*   nFaces          [in]:
*       Input face number.
*   nDetectMode     [in]:
*       The redeye detect mode. Could be one of
*       RER_DETECT_ONLYFACE / ARER_DETECT_SIMPLE /  ARER_DETECT_BALANCED / ARER_DETECT_FULL.
* Return        :
*   Non-negative number
*       The number of red-eyes detected.
*   ARER_ERR_PARAMETER
*       pInputImg or pMemHandle or pEyeInfo is NULL, or the specification of pInputImg is wrong.
*   ARER_ERR_NOT_SUPPORT
*       The pixel array format is not supported by the SDK, or the current API is not functional.
*   ARER_ERR_MEMORY
*       Insufficiency of working memory.
*   ARER_ERR_INTERRUPT
*       Function was interrupted by callback function.
* Remarks       :
*   [1] The return value of callback function could be one of the followings:
*       ARER_OK:
*           Everything is OK, SDK will continue to do further processing.
*       ARER_ERR_TIMEOUT:
*           Maximum processing time reached, SDK will not return this value but stop further processing.
*           For example: if N red-eyes have been found when ARER_ERR_TIMEOUT received, the returned value
*           of this API will be ARER_OK and these N red-eyes will be corrected before escape processing.
*       ARER_ERR_INTERRUPT:
*           SDK will totally stop processing and return the same error code that callback returned.
*   [2] If callback function parameter passed to API is MNull, then callback function will be disabled.
*   [3] The number of detected eyes will not exceed the limitation of ulMaxcapacity.
\**************************************************************************************************/
MRESULT ARER_AutoDetect(    LPARER_OFFSCREEN        pInputImg,
                            LPARER_IMAGEATTRIB      pImgAttrib,
                            LPARER_MEMHANDLE        pMemHandle,
                            LPARER_EYEINFO          pEyeInfo,
                            MUInt32                 ulMaxCapacity,
                            ARER_FNPROGRESS         fnCallback,
                            MVoid                   *pUserParam,
                            LPARER_FACEINFO         pFaces,
                            MUInt32                 ulFaces,
                            MInt32                  nDetectMode);

/*******************************************Routine*Public*****************************************\
* ARER_AutoDetect_WS
* Description   : The function automatically detects red-eyes in the input images and outputs eye
*                 information. The input images includes whole image and small image, this function
*                 will ensure detected eyes in two images are the same.
* Parameters    :
*   pWholeImg           [in]:
*       The memory storing the whole image. The library will detect the red-eye on this image buffer.
*   pWholeImgAttrib     [in]:
*       The additional whole image information for correction. Including the original image size before
*       scaling, focal length, flash strength, etc. NULL if unavailable.
*   pWholeImgEyeInfo    [out]:
*       Pointer to the address of eye information array for whole image.
*   pWholeImgFaces      [in]:
*       The input faces region for whole image. NULL if unavailable.
*   ulWholeImgFaces     [in]:
*       Input face number for whole image.
*   nWholeImgDetectMode [in]:
*       The redeye detect mode for whole image. Could be one of
*       RER_DETECT_ONLYFACE / ARER_DETECT_SIMPLE /  ARER_DETECT_BALANCED / ARER_DETECT_FULL.
*   pSmallImg           [in]:
*       The memory storing the small image. The library will detect the red-eye on this image buffer.
*   pSmallImgAttrib     [in]:
*       The additional small image information for correction. Including the original image size before
*       scaling, focal length, flash strength, etc. NULL if unavailable.
*   pSmallImgEyeInfo    [out]:
*       Pointer to the address of eye information array for small image.
*   pSmallImgFaces      [in]:
*       The input faces region for small image. NULL if unavailable.
*   ulSmallImgFaces     [in]:
*       Input face number for small image.
*   nSmallImgDetectMode [in]:
*       The redeye detect mode for small image. Could be one of
*       RER_DETECT_ONLYFACE / ARER_DETECT_SIMPLE /  ARER_DETECT_BALANCED / ARER_DETECT_FULL.
*   pMemHandle          [in]:
*       The working buffer for the library to do red-eye detection/correction processing. The caller
*       is responsible for allocating and freeing the memory.
*   ulMaxCapacity       [in]:
*       Maximum capacity of eye information array for both images.
*   fnCallback          [in]:
*       The CALLBACK function for both images, which is used for interruption when accessing time limitation reached.
*       It could be NULL.
*   pUserParam          [in]:
*       Caller-specific data that will be passed into the callback function for both images. It could be NULL.
* Return        :
*   Non-negative number
*       The number of red-eyes detected.
*   ARER_ERR_PARAMETER
*       pWholeImg or pSmallImg or pMemHandle or pEyeInfo is NULL, or the specification of pWholeImg or pSmallImg is wrong.
*   ARER_ERR_NOT_SUPPORT
*       The pixel array format is not supported by the SDK, or the current API is not functional.
*   ARER_ERR_MEMORY
*       Insufficiency of working memory.
*   ARER_ERR_INTERRUPT
*       Function was interrupted by callback function.
* Remarks       :
*   [1] The return value of callback function could be one of the followings:
*       ARER_OK:
*           Everything is OK, SDK will continue to do further processing.
*       ARER_ERR_TIMEOUT:
*           Maximum processing time reached, SDK will not return this value but stop further processing.
*           For example: if N red-eyes have been found when ARER_ERR_TIMEOUT received, the returned value
*           of this API will be ARER_OK and these N red-eyes will be corrected before escape processing.
*       ARER_ERR_INTERRUPT:
*           SDK will totally stop processing and return the same error code that callback returned.
*   [2] If callback function parameter passed to API is MNull, then callback function will be disabled.
*   [3] The number of detected eyes will not exceed the limitation of ulMaxcapacity.
\**************************************************************************************************/
MRESULT ARER_AutoDetect_WS (
                         LPARER_OFFSCREEN       pWholeImg,
                         LPARER_IMAGEATTRIB     pWholeImgAttrib,
                         LPARER_EYEINFO         pWholeImgEyeInfo,
                         LPARER_FACEINFO        pWholeImgFaces,
                         MUInt32                ulWholeImgFaces,
                         MInt32                 nWholeImgDetectMode,
                         LPARER_OFFSCREEN       pSmallImg,
                         LPARER_IMAGEATTRIB     pSmallImgAttrib,
                         LPARER_EYEINFO         pSmallImgEyeInfo,
                         LPARER_FACEINFO        pSmallImgFaces,
                         MUInt32                ulSmallImgFaces,
                         MInt32                 nSmallImgDetectMode,
                         LPARER_MEMHANDLE       pMemHandle,
                         MUInt32                ulMaxCapacity,
                         ARER_FNPROGRESS        fnCallback,
                         MVoid                  *pUserParam);

/*******************************************Routine*Public*****************************************\
* ARER_AutoCorrect
* Description   :   The function automatically corrects all detected red-eyes (which is stored in
*                   the eye information array) in the input image.
* Parameters    :
*   pInputImg       [in/out]:
*       The memory storing the input image. The library will detect the red-eye on this image buffer.
*   pImgAttrib      [in]:
*       The additional image information for correction. NULL if unavailable.
*   pMemHandle      [in]:
*       The working buffer for the library to do red-eye detection/correction processing. The caller
*       is responsible for allocating and freeing the memory.
*   pEyeInfo        [in]:
*       Pointer to the address of eye information array.
*   ulNumEyes       [in]:
*       Number of elements that stored in eye information array.
*   ulStartLine [in]:
*       The start line of the strip image.
* Return        :
*   ARER_OK
*       Function succeeds.
*   ARER_ERR_PARAMETER
*       pInputImg or pWorkMem or pEyeInfo is NULL or the specification of pInputImg is wrong.
*   ARER_ERR_NOT_SUPPORT
*       The pixel array format is not supported by the SDK, or the current API is not functional.
*   ARER_ERR_MEMORY
*       Insufficiency of working memory.
* Remarks       :
*   [1] The x and y coordination of red-eye that stored in the eye information array could be
*       negative, which means this eye is outsize of the correction region after cropping operation.
*       SDK will only correct those pixels inside the image.
*   [2] The range of pEyeInfo->lDarkness is [0, 100]. User could modify this parameter to set different
*       darken strength. 0 means NOT darken red-eye, 100 means full strength darken and number "x"
*       means x% darken strength.
*   [3] If the value of lDarkness is smaller than 0, SDK will regard darken strength as 0; if the value
*       of lDarkness is greater than 100, SDK will regard darken strength as 100.
*   [4] The range of pEyeInfo->lAggressive is [0, 200]. User could modify this parameter to set the
*       aggressiveness of correction strategy. 100 means the default aggressiveness (base), any number
*       less than 100 means less aggressive than the default one and any number greater than 100 means
*       more aggressive than the default one.
*   [5] If the value of lAggressive is small than 0, SDK will regard the aggressiveness as 0; if the value
*       of lAggressive is greater than 200, SDK will regard aggressiveness as 200.
\**************************************************************************************************/
MRESULT ARER_AutoCorrect(   LPARER_OFFSCREEN        pInputImg,
                            LPARER_IMAGEATTRIB      pImgAttrib,
                            LPARER_MEMHANDLE        pMemHandle,
                            LPARER_EYEINFO          pEyeInfo,
                            MUInt32                 ulNumEyes,
                            MUInt32                 ulStartLine);

/*******************************************Routine*Public*****************************************\
* ARER_AutoCorrect_RB
* Description   :   The function automatically corrects all reliable red-eyes (which is stored in the
*                   eye information array) in the input image, and returns the NO. of the next line to
*                   be corrected.
* Parameters    :
*   pInputImgArray      [in/out]:
*       Pointer to the address of input image array. The library will detect the red-eye on this images
*   ulNumImgs       [in]:
*       number of elements that stored in image array
*   pImgAttrib      [in]:
*       The additional image information for correction. NULL if unavailable.
*   pMemHandle      [in]:
*       The working buffer for the library to do red-eye detection/correction processing. The caller
*       is responsible for allocating and freeing the memory.
*   pEyeInfo        [in]:
*       Pointer to the address of eye information array.
*   ulNumEyes       [in]:
*       Number of elements that stored in eye information array.
*   ulStartLine [in]:
*       The start line of the strip image.
*   ulMaxRollback [in]
*       The maximum line number for rollback. MUST
*   pulNextStartLine[out]
*       If success, point to the address of next start line index.
* Return        :
*   ARER_OK
*       Function succeeds.
*   ARER_ERR_PARAMETER
*       pInputImgArray or pWorkMem or pEyeInfo or pulNextStartLine is NULL or the specification of pInputImgArray is wrong.
*   ARER_ERR_NOT_SUPPORT
*       The pixel array format is not supported by the SDK, or the current API is not functional.
*   ARER_ERR_MEMORY
*       Insufficiency of working memory.
* Remarks       :
*   [1] The x and y coordination of red-eye that stored in the eye information array could be negative,
*       which means this eye is outsize of the correction region after cropping operation. SDK will only
*       correct those pixels inside the image.
*   [2] In the band correction mode, pInputImgArray contains the band images and ulStartLine is the line
*       offset of the first band in the whole image. In the whole image correction mode, ulStartLine is 0.
*   [3] ulMaxRollback should NOT exceed the total lines of input image bands.
*   [4] The start line of each image band MUST be even.
*   [5] The image bands in pInputImgArray MUST be in successive order.
\**************************************************************************************************/
MRESULT ARER_AutoCorrect_RB(LPARER_OFFSCREEN        pInputImgArray, /*first address of image array*/
                            MUInt32                ulNumImgs,  /*image numbers*/
                            LPARER_IMAGEATTRIB      pImgAttrib,
                            LPARER_MEMHANDLE        pMemHandle,
                            LPARER_EYEINFO          pEyeInfo,
                             MUInt32                    ulNumEyes,
                             MUInt32                    ulStartLine,
                             MUInt32                ulMaxRollback, /*maximum number of rollback line*/
                             MUInt32*               pulNextStartLine /*next start line required*/
                            );


/*******************************************Routine*Public*****************************************\
* ARER_ManualDetect
* Description   : The function automatically detects one red-eyes in the input image and outputs eye
*                 information.
* Parameters    :
*   pInputImg       [in]:
*       The memory storing the input image. The library will detect the red-eye on this image buffer.
*   pImgAttrib      [in]:
*       The additional image information for correction. Including the original image size before
*       scaling, focal length, flash strength, etc. NULL if unavailable. Reserved.
*   pMemHandle      [in]:
*       The working buffer for the library to do red-eye detection/correction processing. The caller
*       is responsible for allocating and freeing the memory.
*   pEyeInfo        [out]:
*       Pointer to the address of eye information array.
*   ulMaxCapacity   [in]:
*       Maximum capacity of eye information array, must be bigger than 1.
*   fnCallback      [in]:
*       The CALLBACK function, which is used for interruption when accessing time limitation reached.
*       It could be NULL. Reserved.
*   pUserParam      [in]:
*       Caller-specific data that will be passed into the callback function. It could be NULL. Reserved
*   rcRect          [in]:
*       The input search region for single one-click removal.
*   nDetectMode     [in]:
*       The manual red-eye detect mode/option. Reserved.
* Return        :
*   Non-negative number
*       The number of red-eyes detected.
*   ARER_ERR_PARAMETER
*       pInputImg or pMemHandle or pEyeInfo is NULL, or the specification of pInputImg is wrong.
*   ARER_ERR_NOT_SUPPORT
*       The pixel array format is not supported by the SDK, or the current API is not functional.
*   ARER_ERR_MEMORY
*       Insufficiency of working memory.
*   ARER_ERR_INTERRUPT
*       Function was interrupted by callback function.
* Remarks       :
*   [1] The return value of callback function could be one of the followings:
*       ARER_OK:
*           Everything is OK, SDK will continue to do further processing.
*       ARER_ERR_TIMEOUT:
*           Maximum processing time reached, SDK will not return this value but stop further processing.
*           For example: if N red-eyes have been found when ARER_ERR_TIMEOUT received, the returned value
*           of this API will be ARER_OK and these N red-eyes will be corrected before escape processing.
*       ARER_ERR_INTERRUPT:
*           SDK will totally stop processing and return the same error code that callback returned.
*   [2] If callback function parameter passed to API is MNull, then callback function will be disabled.
*   [3] The number of detected eyes will not exceed 1.
\**************************************************************************************************/
MRESULT ARER_ManualDetect(  LPARER_OFFSCREEN        pInputImg,
                            LPARER_IMAGEATTRIB      pImgAttrib,
                            LPARER_MEMHANDLE        pMemHandle,
                            LPARER_EYEINFO          pEyeInfo,
                            MUInt32                 ulMaxCapacity,
                            ARER_FNPROGRESS         fnCallback,
                            MVoid                   *pUserParam,
                            MRECT                   rcRect,
                            MInt32                  nDetectMode);


/*******************************************Routine*Public*****************************************\
* ARER_GetDefaultRECT
* Description   : The function automatically returns a specific area around one click point.
* Parameters    :
*   nImageWidth     [in]:
*       The width of image.
*   nImageHeight    [in]:
*       The height of image.
*   nClickPoint     [in]:
*       Click point inside the image, which is used to estimate one surrounding rectangle area.
*   pRect           [out]:
*       The specified area around the click point for manual red-eye removal.
* Return        :
*   ARER_OK
*       Succeed.
*   ARER_ERR_PARAMETER
*       The image size is illegal or click point is outside of image.
* Remarks       :
*   [1] This is a default sample. User can specify RECT in different cases.
\**************************************************************************************************/
MRESULT ARER_GetDefaultRECT (MInt32     nImageWidth,
                             MInt32     nImageHeight,
                             MPOINT     nClickPoint,
                             PMRECT     pRect);


#ifdef __cplusplus
}
#endif


#endif/*__ARCSOFT_RED_EYE_REMOVE_H__*/

