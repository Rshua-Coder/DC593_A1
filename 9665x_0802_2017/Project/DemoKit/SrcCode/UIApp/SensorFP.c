/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SensorFP.c
    @ingroup    mIPRJAPCfg

    @brief      Implement some functions for sensor object.
                These functions are required (?) for sensor object when Open_Sensor()

    @note       Nothing.

    @date       2007/01/23
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "SysCfg.h"
#include "GxSystem.h"
#if _DEMO_TODO
#include "SensorFP.h"
#endif
#include "GxFlash.h"
#include "GxDC.h"
#include "Uiresource.h"
#include "uiDisplay.h"
#include "UIGraphics.h"

//#NT#2007/05/04#Steven Wang -end
/**
  Function for sensor object

  Function for sensor object.
  Charge Flash.

  @param BOOL bCharge: Charge flash or not
  @param BOOL bHighCurrent: Charge flash with high current or not
  @return void
*/
void SenFP_ChargeFlash(BOOL bCharge, BOOL bHighCurrent)
{
    //XXXXXXXXXXXXXXX 2009/05/20# Lincy - begin
    //GxFlash_ChargeFlash(bCharge, bHighCurrent);
    //XXXXXXXXXXXXXXX 2009/05/20# Lincy - end
}

/**
  Function for sensor object

  Function for sensor object.
  Check if flash is ready.

  @param void
  @return BOOL: TRUE if flahs is ready
*/
BOOL SenFP_IsFlashReady(void)
{
    return GxFlash_IsFlashReady();
}

/**
  Function for sensor object

  Function for sensor object.
  Get Flash ADC value.

  @param void
  @return UINT32 Flash ADC value
*/
UINT32 SenFP_GetFlashADCValue(void)
{
    debug_err(("SenFP_GetFlashADCValue is empty function\r\n"));
    return 0;
}

extern void SenFP_ShowOSDString(CHAR *pString, UINT16 uiX, UINT16 uiY, UINT16 uiColor);
void SenFP_ShowOSDString(CHAR *pString, UINT16 uiX, UINT16 uiY, UINT16 uiColor)
{
    #if 1
    DC**     pDCList;
    pDCList = (DC**)UI_BeginScreen();
    GxGfx_SetTextColor(uiColor, _OSD_INDEX_GRAY, 0);
    GxGfx_Text(pDCList[GxGfx_OSD], uiX, uiY, pString);
    UI_EndScreen((UINT32)pDCList);
    #else
    debug_err(("SenFP_ShowOSDString is empty function\r\n"));
    #endif
}

/**
  Function for sensor object

  Function for sensor object.
  Clear OSD.

  @param void
  @return void
*/
extern void SenFP_ClearOSD(void);
void SenFP_ClearOSD(void)
{
    #if 1
    DC**     pDCList;
    pDCList = (DC**)UI_BeginScreen();
    GxGfx_Clear(pDCList[GxGfx_OSD]);
    UI_EndScreen((UINT32)pDCList);
    #else
    debug_err(("SenFP_ClearOSD is empty function\r\n"));
    #endif
}

/**
  Function for sensor object

  Function for sensor object.
  Power off.

  @param BOOL bLensRet: Retract Lens or not
  @param BOOL bWriteNand: Write data to NAND or not
  @return void
*/
void SenFP_PowerOff(BOOL bLensRet, BOOL bWriteNand)
{
    #if 0
    GxSystem_PowerOff();
    #else
    debug_err(("SenFP_PowerOff is empty function\r\n"));
    #endif
}


//#NT#2009/10/07#Lincy Lin -begin
//#Temp add for linking error
#if 0
UINT16 Get_FDIndex(void)
{
    debug_err(("Get_FDIndex is empty function\r\n"));
}
#endif

//#NT#2009/11/18#Lincy Lin -begin
/*
void LensCtrl_SetAFJob(INT16 iFocusPos)
{
    debug_err(("LensCtrl_SetAFJob is empty function\r\n"));
}
UINT32 LensCtrl_CheckAFJob(BOOL bWait)
{
    debug_err(("LensCtrl_CheckAFJob is empty function\r\n"));
}
*/
//#NT#2009/11/18#Lincy Lin -end
//#NT#2009/10/07#Lincy Lin -end

//@}
