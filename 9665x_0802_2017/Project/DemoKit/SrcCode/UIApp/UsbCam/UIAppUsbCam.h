/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIAppUsbCam.h
    @ingroup    mIPRJAPCfg

    @brief      Header file of UIAppUsbCam_Exe.c
                Header file of UIAppUsbCam_Exe.c

    @note       Nothing.

    @version    V1.00.000
    @author     Chris Hsu
    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _UIAPPUSBCAM_H
#define _UIAPPUSBCAM_H

#include "UIFramework.h"
#include "Type.h"
#if _DEMO_TODO
#include "UVAC.h"
#endif

extern void     AppInit_ModeUVAC(void);

extern VControl CustomUSBPCCObjCtrl;

#endif
//@}
