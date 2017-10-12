/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       AF_Lib.h
    @ingroup    mILibAFAlg

    @brief      auto focus function header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA
    @date       2012/09/18
*/
#ifndef _AF_LIB_H
#define _AF_LIB_H

#include "Type.h"

typedef enum {
    AF_STA_OK,
    AF_STA_FAIL,
    AF_STA_TIMEOUT,
    AF_STA_TOO_DARK
} _AF_STATUS_;

//Add the callback function to detect GPIO
typedef BOOL (*FPAF_DETGPIOKEY)(void);
typedef UINT32 (*FPCALLBACK_GETVA)(UINT8);

#define AF_FOCUS_ARRAY_SIZE     100 // 48
#define AF_FOCUS_TOTAL_WIN      9

typedef struct {
    INT32       StartIdx;
    INT32       EndIdx;
    INT32       MaxIdx;
    UINT32      MaxVA;
    UINT32      AF_time;
    INT32       AF_GuessIdx;
    INT32       Step[AF_FOCUS_ARRAY_SIZE];
    UINT32      Value[AF_FOCUS_ARRAY_SIZE];
    UINT32      VACnt[AF_FOCUS_ARRAY_SIZE];
    UINT32      AF_Window;
    UINT32      MultiValue[AF_FOCUS_TOTAL_WIN][AF_FOCUS_ARRAY_SIZE];
} AF_INFO, *PAF_INFO;

typedef struct {
    BOOL        bEnable;
    UINT32      Step[AF_FOCUS_ARRAY_SIZE];
    UINT32      Value[AF_FOCUS_ARRAY_SIZE];
} AF_GUESS_INFO, *PAF_GUESS_INFO;

extern void     AF_ResetAFInfo(void);

#endif
