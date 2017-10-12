/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DetLED.c
    @ingroup    mIPRJAPKeyIO

    @brief      Control LED on/off
                Control LED on/off/toggle

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "DxCfg.h"
#include "IOCfg.h"

#include "DxOutput.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//#NT#set FCS LED brightness level
UINT32   FcsLedLvl = LCSBRT_LVL_03;
UINT32   FcsLedPrdVal[LCSBRT_LVL_SETTING_MAX]=
{
    14,
    15,
    16,
    17,
    18,
    19,
    20
};

////////////////////////////////////////////////////////////////////////////////
// GPIO related

/**
  Turn on LED

  Turn on LED.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_RED
    GPIOMAP_LED_FCS //FOCUS
  @return void
*/
void LED_TurnOnLED(UINT32 uiLED)
{

    DBG_IND("LED_TurnOnLED %d\r\n",uiLED);
    switch (uiLED)
    {
    case GPIOMAP_LED_GREEN:
    #if (LED_GREEN_CTRL == LED_GREEN_BY_GPIO)
        gpio_setPin(GPIO_GREEN_LED);
    #endif
    break;

    case GPIOMAP_LED_RED:
    #if (LED_RED_CTRL == LED_RED_BY_GPIO)
        gpio_setPin(GPIO_RED_LED);
    #endif
    break;

    case GPIOMAP_LED_FCS:
    #if (LED_FOCUS_CTRL == LED_FOCUS_BY_GPIO)
        gpio_setPin(GPIO_FOCUS_LED);
    #endif
    #if (LED_FOCUS_CTRL == LED_FOCUS_BY_PWM)
    {
        //set FOCUS LED brightness level
        PWM_CFG PWMInfo;
        if(pwm_open(PWMID_FOCUS_LED)!=E_OK)
        {
            pwm_stop(PWMID_FOCUS_LED);
            pwm_close(PWMID_FOCUS_LED, TRUE);
            pwm_open(PWMID_FOCUS_LED);
        }
        PWMInfo.uiDiv=220;
        PWMInfo.uiPrd=22;
        PWMInfo.uiFall=FcsLedPrdVal[FcsLedLvl];
        PWMInfo.uiOnCycle=0;
        PWMInfo.uiInv=0;
        PWMInfo.uiRise=0;
        pwm_set(PWMID_FOCUS_LED, &PWMInfo);
        pwm_reload(PWMID_FOCUS_LED);
    }
    #endif

    default:
    break;
    }
}

/**
  Turn off LED

  Turn off LED.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_RED
    GPIOMAP_LED_FCS
  @return void
*/
void LED_TurnOffLED(UINT32 uiLED)
{
    DBG_IND("LED_TurnOffLED %d\r\n",uiLED);
    switch (uiLED)
    {
    case GPIOMAP_LED_GREEN:
    #if (LED_GREEN_CTRL == LED_GREEN_BY_GPIO)
        gpio_clearPin(GPIO_GREEN_LED);
    #endif
    break;

    case GPIOMAP_LED_RED:
    #if (LED_RED_CTRL == LED_RED_BY_GPIO)
        gpio_clearPin(GPIO_RED_LED);
    #endif
    break;

    case GPIOMAP_LED_FCS:
    #if (LED_FOCUS_CTRL == LED_FOCUS_BY_GPIO)
        gpio_clearPin(GPIO_FOCUS_LED);
    #endif
    #if (LED_FOCUS_CTRL == LED_FOCUS_BY_PWM)
    {
        //set FOCUS LED brightness level
        PWM_CFG PWMInfo;
        PWMInfo.uiDiv=220;
        PWMInfo.uiPrd=0;
        PWMInfo.uiFall=0;
        PWMInfo.uiOnCycle=0;
        PWMInfo.uiInv=0;
        PWMInfo.uiRise=0;
        pwm_set(PWMID_FOCUS_LED, &PWMInfo);
        pwm_reload(PWMID_FOCUS_LED);
    }
    #endif
    default:
    break;
    }
}

/**
  Check whether LED is on or not

  Check whether LED is on or not.
  Return TRUE if LED is on, return FALSE if LED is off.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_RED
    GPIOMAP_LED_FCS
  @return BOOL
*/
BOOL LED_IsLEDOn(UINT32 uiLED)
{
    BOOL bvalue = FALSE;
    DBG_IND("LED_IsLEDOn %d\r\n",uiLED);
    switch (uiLED)
    {
    case GPIOMAP_LED_GREEN:
    #if (LED_GREEN_CTRL == LED_GREEN_BY_GPIO)
        bvalue = gpio_getPin(GPIO_GREEN_LED);
    #endif
        break;
    case GPIOMAP_LED_RED:
    #if (LED_RED_CTRL == LED_RED_BY_GPIO)
        bvalue = gpio_getPin(GPIO_RED_LED);
    #endif
        break;
    case GPIOMAP_LED_FCS:
    #if (LED_FOCUS_CTRL == LED_FOCUS_BY_GPIO)
        bvalue = gpio_getPin(GPIO_FOCUS_LED);
    #endif
        break;
    default:
        bvalue = FALSE;
        break;
    }
    return bvalue;
}

//set FCS LED brightness level
void LED_SetFcsLevel(UINT32 uiLevel)
{
    #if (LED_FOCUS_CTRL == LED_FOCUS_BY_PWM)
    FcsLedLvl = uiLevel;
    #endif
}
//@}
