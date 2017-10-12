/**
    Public header file for Cal USB mode

    This file is the header file that define the API and data type for Cal USB.

    @file       IPL_CalUSB_Task.h
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _IPL_CALUSB_TASK_H_
#define _IPL_CALUSB_TASK_H_

#include "Type.h"

/**
    install Cal USB control id
*/
extern void CalUSB_InstallID(void) _SECTION(".kercfg_text");
//@}
#endif //_IPL_CALUSB_TASK_H_