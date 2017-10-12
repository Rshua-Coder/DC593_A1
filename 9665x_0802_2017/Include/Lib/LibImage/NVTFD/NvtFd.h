#ifndef __NVTFD__H__
#define __NVTFD__H__

#include <Type.h>
#include <debug.h>
#include <perf.h>
#include "FileSysTsk.h"

#include "ife_lib.h"
#include "ime_lib.h"
#include "grph.h"
#include "ise_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "Clock.h"
#include "Debug.h"
#include "DMA.h"
#include "Kernel.h"
#include "PLL.h"
#include "Sdio.h"
#include "top.h"
#include "type.h"
#include "Uart.h"
#include "Utility.h"
#include "IPL_Hal_IME_Info.h"
#include "fde_lib.h"
#include "GxImage.h"

//nvt face detection interfaces

/**
    Detect faces from HW candidiates.
    @param[in] imgBuf: gray image info.
    @param[in && out] pFace: HW candidates input and detected faces output
    @param[in] nFace: HW candidates number
    @param[in] pBuff: FD buff addr
    @return detected faces number
*/
extern UINT32 NvtFD_DetectFaces( IMG_BUF *imgBuf, FACE *pFace, UINT32 nFace, UINT32 pBuff );

/**
    Draw face boxes and save the image.
    @param[in] imgBuf: gray image info.
    @param[in] pFace: faces to draw
    @param[in] nFace: faces number
    @param[in] flag: flag for saving images(0 - HW, 1 - ARC, 2 - NVT)
    @return void
*/
extern void NvtFD_SaveResultImage( IMG_BUF *imgBuf, FACE *pFace, UINT32 nFace, UINT32 flag );



#endif

