/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DeviceCtrl.h
    @ingroup    mIPRJAPCfg

    @brief      Header file of DeviceCtrl.c
                Header file of DeviceCtrl.c

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _DEVICECTRL_H
#define _DEVICECTRL_H

#include "Type.h"
//#include "SysCfg.h"

#define DEVCTRL_IPLMODE_OFF             0
#define DEVCTRL_IPLMODE_IDLE            1
#define DEVCTRL_IPLMODE_PREVIEW         2
#define DEVCTRL_IPLMODE_PCC             3
#define DEVCTRL_IPLMODE_VIDEO           4
#define DEVCTRL_IPLMODE_VIDEOREC        5
#define DEVCTRL_IPLMODE_CAP             6

extern void     DevCtrl_SetIPLMode(UINT32 uiMode);
extern UINT32   DevCtrl_GetIPLMode(void);

extern void     DevCtrl_ModePlayback(void);
extern void     DevCtrl_ModePhoto(void);
//#NT#2010/06/01#Jeah Yen -FASTBOOT- -begin
extern void     DevCtrl_ModePhoto_FastSensor(void);
extern void     DevCtrl_ModePhoto_FastLens(void);
extern void     DevCtrl_ModePhoto_WaitFinish(void);
//#NT#2010/06/01#Jeah Yen -FASTBOOT- -end
extern void     DevCtrl_ModeMovie(void);
//#NT#2010/06/01#Jeah Yen -FASTBOOT- -begin
extern void     DevCtrl_ModeMovie_FastSensor(void);
extern void     DevCtrl_ModeMovie_FastLens(void);
extern void     DevCtrl_ModeMovie_WaitFinish(void);
//#NT#2010/06/01#Jeah Yen -FASTBOOT- -end
extern void     DevCtrl_ModeUSBMSDC(void);
extern void     DevCtrl_ModeUSBMSDC_Remove(void);
extern void     DevCtrl_ModeUSBPCC(void);
extern void     DevCtrl_ModeUSBPCC_Remove(void);
extern void     DevCtrl_ModeUSBSIDC(void);
extern void     DevCtrl_ModeUSBSIDC_Remove(void);
//#NT#2010/12/14#Lincy Lin -begin
//#NT#Add lens error check
extern BOOL     DevCtrl_Lens(BOOL bIsOpen);
//#NT#2010/12/14#Lincy Lin -end
//#NT#2010/03/01#Jeah Yen -begin
//#NT#2010/07/30#Jeah Yen -begin
//#NT#2010/09/09#Photon Lin -begin
//#Arrange callback function of lens
extern void     DrvLens_RegCtrlCB(void* pLensTask1CB, void* pLensTask2CB, void* pLensAPICB);
//#NT#2010/09/09#Photon Lin -end
//#NT#2010/07/30#Jeah Yen -end
extern void     DevCtrl_Lens_PowerOn_Advance(void);
extern BOOL     DevCtrl_Lens_PowerOn_IsUnderAdvance(void);
extern BOOL     DevCtrl_Lens_PowerOff_IsNeedRetract(void);
extern void     DevCtrl_Lens_PowerOff_Retract(void);
extern void     DevCtrl_Lens_PowerOff_Exit(void);
extern void     DevCtrl_Lens_Playback_Retract(void);
//#NT#2010/03/01#Jeah Yen -end

#endif

//@}
