/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       LensAPI.h
    @ingroup    mISYSLens

    @brief      Lens API functions header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA
    @date       2012/09/17
*/

#ifndef _LENSAPI_H
#define _LENSAPI_H

#include "Lens.h"

//
//FLG_ID_LENSBG
//
#define FLGLENSBG_IQS_SETFLOWFUNC          0x00000001
#define FLGLENSBG_FOCUS_AFTER_ZOOM_CHANGED 0x00000002
#define FLGLENSBG_IDLE                     0x80000000
#define FLGLENSBG_ALL                      0xFFFFFFFF

extern UINT32 _SECTION(".kercfg_data") FLG_ID_LENS;
extern UINT32 _SECTION(".kercfg_data") LENSCTRLTSK_ID;
extern UINT32 _SECTION(".kercfg_data") SEMID_LENS_CTLID;

extern UINT32 _SECTION(".kercfg_data") FLG_ID_LENS2;
extern UINT32 _SECTION(".kercfg_data") LENSCTRLTSK2_ID;

extern UINT32 _SECTION(".kercfg_data") FLG_ID_LENSBG;
extern UINT32 _SECTION(".kercfg_data") LENSCTRLTSKBG_ID;

#endif
