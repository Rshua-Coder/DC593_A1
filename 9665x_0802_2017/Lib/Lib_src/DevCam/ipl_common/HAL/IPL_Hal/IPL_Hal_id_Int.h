/**
    IPL_Hal

    .

    @file       IPL_Hal_id.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_HAL_ID_INT_H_
#define _IPL_HAL_ID_INT_H_

#include "Type.h"

#define FLGPTN_IPH_SIE  FLGPTN_BIT(0)
#define FLGPTN_IPH_PRE  FLGPTN_BIT(1)
#define FLGPTN_IPH_IFE  FLGPTN_BIT(2)
#define FLGPTN_IPH_DCE  FLGPTN_BIT(3)
#define FLGPTN_IPH_IPE  FLGPTN_BIT(4)
#define FLGPTN_IPH_IFE2 FLGPTN_BIT(5)
#define FLGPTN_IPH_IME  FLGPTN_BIT(6)
#define FLGPTN_IPH_RDE  FLGPTN_BIT(7)
#define FLGPTN_IPH_SIE2 FLGPTN_BIT(8)
#define FLGPTN_IPH_ALL  0X7FFFFFFF

#define FLGPTN_IPH_ERR  FLGPTN_BIT(31)

extern void Install_IPHID(void) _SECTION(".kercfg_text");

extern UINT32 _SECTION(".kercfg_data") m_IPH_FLGID;
#endif //_IPL_HAL_ID_H_ä