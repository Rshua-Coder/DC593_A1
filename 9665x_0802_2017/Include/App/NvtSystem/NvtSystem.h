/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIDef.h
    @ingroup    mIPRJAPUIFlow

    @brief      UIDef Functions
                This file is the user interface ( for LIB callback function).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef _NVTSYSTEM_H
#define _NVTSYSTEM_H

#include "GxLib.h"
#include "AppView.h"
#include "NvtAppPhotoApi.h"
#include "NvtAppMovieApi.h"
#include "NvtAppPlayApi.h"
#include "NvtAppUsbApi.h"
#include "NvtSysInputApi.h"

extern void NvtSystem_InstallID(void) _SECTION(".kercfg_text");

extern void DscMain(UINT32 pUserMainProc);
extern void DscExit(void);

///////////////////////////////////////////////////////////////////////////////
//for power
extern void Power_CB(UINT32 event, UINT32 param1, UINT32 param2);

//for display
extern void Video_CB(UINT32 event, UINT32 param1, UINT32 param2);

//for strg
extern void Strg_CB(UINT32 event, UINT32 param1, UINT32 param2);
///////////////////////////////////////////////////////////////////////////////

#endif//_NVTSYSTEM_H

