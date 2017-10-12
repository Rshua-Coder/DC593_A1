#ifndef NVTHDR_LIB_H
#define NVTHDR_LIB_H

#include <type.h>

//---------------------------------------------------------------------------------------------

/**
    HDR library data for external usage
*/
typedef struct _HDR_DATA_INFO
{
    UINT32 uiInputDataAddr[6];              ///> start address of UV packed input, use cache address
    UINT32 uiWorkBufStartAddr;              ///> Start address of working buffer, use cache address
    UINT32 uiWorkBufSize;                   ///> working buffer size
    UINT32 uiWidth;                         ///> input image width
    UINT32 uiHeight;                        ///> input image height
    UINT32 uiEvGap;                         ///> EvGap if input images, 1 & 2 are available
} HDR_DATA_INFO;

/**
    HDR FD detect image struct
*/
typedef struct _HDR_FD_IMG
{
    UINT32 uiAddr;                          ///> start address of image buffer
    UINT32 uiLofst;                         ///> line offset of image buffer
    UINT32 uiWidth;                         ///> width of image
    UINT32 uiHeight;                        ///> height of image
} HDR_FD_IMG;

/**
    HDR Face result struct
*/
typedef struct _HDR_FACE_DATA
{
    UINT16  x;                               ///< result start x
    UINT16  y;                               ///< result start y
    UINT16  w;                               ///< result width
    UINT16  h;                               ///< result height
} HDR_FACE_DATA;

/**
    Status of all HDR process
*/
typedef enum
{
    NVTHDR_SUCCESS=0,                          ///> HDR process success
    NVTHDR_FAIL=1                              ///> HDR process fail
} HDR_STATUS;


/**
    HDR FD callback

    pFdImg
        the 3 input images(YUV420) information provided by NvtHDR library
        for face detection, [0]->Y, [1]->U, [2]->V
    pFaceData
        HDR_FACE_DATA pointer that points to the
        pre-allocated arrays that save the start coordinate
        and size of the detected faces
    pFaceNum
        UINT32 pointer that points to the pre-allocated variable
        that saves the number of the detected faces
*/
typedef void (*NVTHDR_FDCB)(HDR_FD_IMG pFdImg[], HDR_FACE_DATA *pFaceData, UINT32 *pFaceNum);


//---------------------------------------------------------------------------------------------

/**
    HDR library API to get working buffer size.
    Please allocate enough buffer according to the returned size.
*/
extern UINT32 NvtHDR_GetWorkBufSize(UINT32 uiWidth, UINT32 uiHeight);

/**
    HDR library API to do all HDR process.
    Library will proceed accord to the parameters provided
    in the HDR_DATA_INFO object.
*/
extern UINT32 NvtHDR_DoAllProcess(HDR_DATA_INFO *pHdrParam);

/**
    HDR library API to get allocated output data address.
    Please define a UINT32*3 array to save address for 1
    YCC420 output images. The order of index coorespond to
    the plane of output data is:
    0~2 -> Y0 Cb0 Cr0
*/
extern VOID NvtHDR_GetOutputDataAddr(UINT32 *puiOutputDataAddr);

/**
    HDR library API to register Face detection call back function
    The prototype of call back function please see above
*/
extern void NvtHDR_FD_RegisterCB(NVTHDR_FDCB CB);

#endif
