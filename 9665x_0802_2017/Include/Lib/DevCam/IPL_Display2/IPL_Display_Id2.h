/**
    IPL display Task id 2.

    .

    @file       IPL_display_id_int2.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_DISPLAY_ID_INT_H_2_
#define _IPL_DISPLAY_ID_INT_H_2_

#include "Type.h"

extern void IPL_Display_InstallID2(void) _SECTION(".kercfg_text");

extern UINT32 _SECTION(".kercfg_data ") m_uiDisplayCtrl_TskID2;
extern UINT32 _SECTION(".kercfg_data ") gDisplayCtrl_FlgID2;

#endif //_IPL_DISPLAY_ID_INT_H_
