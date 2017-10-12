#ifndef _SCREEN_160x120_H
#define _SCREEN_160x120_H

#include "Type.h"
#include "UICustomer.h"

#if (SCREEN_BMP == LOGO_5060)
//#NT#2009/10/21#Philex Lin - begin
#define SCREEN_BS_SIZE      20987//36347
extern const UINT8 g_uiScreen_160x120[];
#define SCREEN_PWROFF_SIZE      19049//0x33B4
extern const UINT8 g_uiScreen_PwrOff[];
#define SCREEN_SETUPBG_SIZE      19049//0x33B4
extern const UINT8 g_uiScreen_SetupBG[];
#endif

#endif
