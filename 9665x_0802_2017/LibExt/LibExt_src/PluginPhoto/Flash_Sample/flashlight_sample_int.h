/**
    Flashlight sample flow.

    @file       Flashlight_sample_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _FLASHLIGHT_SAMPLE_H_
#define _FLASHLIGHT_SAMPLE_H_

#include "Flashlight_api.h"

#define PRE_FLASH_WIDTH 15//20
#define REDEYE_WIDTH 10//20
#define FLASH_TRIGGERPOINT 70 // LV7.0

#define FLASH_DEF_RGAIN 346
#define FLASH_DEF_GGAIN 256
#define FLASH_DEF_BGAIN 266

#define FlashWinX 8
#define FlashWinY 8
#define Flash_CAWinX 32
#define Flash_CAWinY 32
#define Flash_TARGET_Y 50//100

#define FlashRgain 550
#define FlashGgain 256
#define FlashBgain 320

////////////////////////////////////////////////////////////////////////////////
///////////////////////           Information API            ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
    Pre-flash flow, use to calculate main flashlight

    @param[in] FLASH_PARA_SET the function point to do preflash
    @return FLASH_INFOR
        - @b main flashlight parameters
*/
extern FLASH_INFOR flash_PreFlashFlow(FLASH_PARA_SET FlashSetting_api);


/**
    Red eye reduction flow, use preflash to avoid red eye.

    @param[in] FLASH_PARA_SET the function point to do preflash
    @return None
*/
extern void flash_RedEyeReduct(FLASH_PARA_SET FlashSetting_api);


/**
    Refer to UI & AE condition and decide enable/disable flashlight

    @return None
*/
extern void flash_Evaluate(void);

#endif
