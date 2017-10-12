#include "DxFlash.h"
#include "DxCfg.h"
#include "IOCfg.h"

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxFlash
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

////////////////////////////////////////////////////////////////////////////////
// GPIO related

/**
  Charge flash or stop charging flash
  @param BOOL bCharge: TRUE -> Charge flash, FALSE -> Stop charging flash
  @param BOOL bHighCurrent: TRUE -> Charging with high current, FALSE -> Charging with low current
  @return void
*/
void DrvFlash_ChargeFlash(BOOL bCharge, BOOL bHighCurrent)
{
    /*
    if (bCharge == TRUE)
    {
        gpio_setPin(GPIO_FLASH_CHARGE);

    }
    else
    {
        gpio_clearPin(GPIO_FLASH_CHARGE);
    }
    */
}


BOOL DrvFlash_IsCharge(void)
{
    //return (BOOL) gpio_getPin(GPIO_FLASH_CHARGE);
    return FALSE;
}
/**
  Check whether flash is charging or not

  Check whether flash is charging or not.
  Return TRUE if flash is charging, otherwise return FALSE.

  @param void
  @return BOOL
*/
BOOL DrvFlash_IsFlashRdy(void)
{
    //return (BOOL)!gpio_getPin(GPIO_FLASH_RDY);
    return TRUE;
}

void DrvFlash_TriggerFlash(BOOL bEn)
{
    /*
    if (bEn)
    {
        pinmux_select_flctr(PINMUX_GPIO);
        gpio_setPin(GPIO_FLASH_TRIGGER);
    }
    else
    {
        gpio_clearPin(GPIO_FLASH_TRIGGER);
        pinmux_select_flctr(PINMUX_FLCTR);
    }
    */
}


