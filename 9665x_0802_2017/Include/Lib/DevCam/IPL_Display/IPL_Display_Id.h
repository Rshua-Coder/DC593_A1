/**
    IPL display Task id.

    .

    @file       IPL_display_id_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_DISPLAY_ID_INT_H_
#define _IPL_DISPLAY_ID_INT_H_

#include "Type.h"

extern void IPL_Display_InstallID(void) _SECTION(".kercfg_text");

extern UINT32 _SECTION(".kercfg_data ") m_uiDisplayCtrl_TskID;
extern UINT32 _SECTION(".kercfg_data ") gDisplayCtrl_FlgID;

#endif //_IPL_DISPLAY_ID_INT_H_
