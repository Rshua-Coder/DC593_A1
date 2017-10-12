#ifndef NVTNS_LIB_H
#define NVTNS_LIB_H

#include "type.h"


//---------------------------------------------------------------------------------------------

/**
    Status of all NightShot process
*/
typedef enum
{
    NVTNS_SUCCESS=0,                          ///> NightShot process is successful
    NVTNS_FAIL=1                              ///> NightShot process is terminated
} NS_STATUS;

/**
    Level of NightShot NR LEVEL
*/
typedef enum
{
    NR_LEVEL_LOW = 0,
    NR_LEVEL_MIDDLE = 1,
    NR_LEVEL_HIGH = 2
} NR_LEVEL;

/**
    Input of NightShot library data
*/
typedef struct _NS_DATA_INFO
{
    UINT32 uiInputDataAddr[8];   ///> plane start address for all input, 4 Imgs
    UINT32 uiWorkBufStartAddr;   ///> Start address of working buffer
    UINT32 uiWidth;              ///> input image width
    UINT32 uiHeight;             ///> input image height
    BOOL bAutoRefEn;
    UINT32 uiRefIdx;             ///> reference image index
    BOOL bYuvDeghostEn;
} NS_DATA_INFO;

//---------------------------------------------------------------------------------------------
/**
    NightShot library API to get working buffer size.
    Note that this size does not include buffer of source images.
*/
extern UINT32 NvtNS_GetWorkBufSize(UINT32 uiWidth, UINT32 uiHeight);

/**
    NightShot library API to do all NightShot process.
    Library will proceed accord to the parameters provided
    in the NS_DATA_INFO object.
*/
extern NS_STATUS NvtNS_DoAllProcess(NS_DATA_INFO *pNsParam);

/**
    NightShot library API to get allocated output data address.
    Please define a UINT32*3 array to save address for 1
    YCC420 output images. The order of index coorespond to
    the plane of output data is:
    0~2 -> Y0 Cb0 Cr0
*/
extern VOID NvtNS_GetOutputDataAddr(UINT32 *puiOutputDataAddr);

#endif//header guard
