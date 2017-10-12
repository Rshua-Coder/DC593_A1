/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       IOCfg.c
    @ingroup    mIPRJAPCommonIO

    @brief      IO config module
                This file is the IO config module

    @note       Nothing.

    @date       2005/12/24
*/

/** \addtogroup mIPRJAPCommonIO */
//@{

#include "Type.h"
#include "DrvExt.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "debug.h"
#include "IOCfg.h"
#include "Utility.h"
#include "Pll.h"
#include "IOInit.h"
//#include "Timer.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (USE_VIO == ENABLE)
UINT32 Virtual_IO[VIO_MAX_ID] = {0};
UINT32 vio_getpin(UINT32 id){if(id>=VIO_MAX_ID)return 0; return Virtual_IO[id];}
void vio_setpin(UINT32 id, UINT32 v){if(id>=VIO_MAX_ID)return; Virtual_IO[id] = v;}
#endif

#define GPIO_SET_NONE           0xffffff
#define GPIO_SET_OUTPUT_LOW     0x0
#define GPIO_SET_OUTPUT_HI      0x1

GPIO_INIT_OBJ uiGPIOMapInitTab[] = {
    //CARD
    {  GPIO_CARD_POWER,         GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  GPIO_SET_NONE         }, // low active
    {  GPIO_CARD_DETECT,        GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_CARD_DETECT       }, // low active
    {  GPIO_CARD_WP,            GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_CARD_WP           }, // useless
    //LCD
    {  GPIO_LCD_BLG_PCTL,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_LCD_BLG_PCTL      }, // high active
    //LED
    {  GPIO_RED_LED,            GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   GPIO_SET_NONE         }, // low active
    {  GPIO_GREEN_LED,          GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   GPIO_SET_NONE         }, // low active
    //KEY
    {  GPIO_KEY_MODE,           GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_MODE          }, // useless
    //Sensor
    {  GPIO_SENSOR_PWM0,        GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST     }, // 1.8V, high active
    {  GPIO_SENSOR_PWM1,        GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST     }, // 2.8V, high active
    {  GPIO_SENSOR_PWM2,        GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST     }, // useless
    {  GPIO_SENSOR_RESET,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST     }, // low reset
    {  GPIO_SENSOR_STANDBY,     GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST     }, // useless
    //Wi-Fi power
    {  GPIO_WIFI_WAKE_HOST,     GPIO_DIR_INPUT,     PAD_PULLDOWN,         PAD_WIFI_WAKE_HOST    }, // useless (WiFi wake up host)
    {  GPIO_WIFI_REG_ON,        GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST     }, // WiFi module enable (high enable)
};

UINT32 totalGpioInitCount = sizeof(uiGPIOMapInitTab)/sizeof((uiGPIOMapInitTab)[0]);

#include "rtc.h"

//should be call after rtc_open()
void IO_GetPowerSrc(void)
{
    UINT32 pwrlost, pwsrc;
    pwrlost = rtc_isPowerLost();
    if(pwrlost)
    {
        DBG_DUMP("^GPowerOn Pwr Lost!\r\n"); //"firs time power-on" or "lost power of Gold capacitor"

        //should notify user to configure current date-time!
    }

    pwsrc = rtc_getPWROnSource();
    if(pwsrc == RTC_PWRON_SRC_PWR_SW)
    {
        DBG_DUMP("^GPowerOn Src = PWR key\r\n");
    }
    else if(pwsrc == RTC_PWRON_SRC_PWR_SW2)
    {
        DBG_DUMP("^GPowerOn Src = PB Key\r\n");
    }
    else if(pwsrc == RTC_PWRON_SRC_PWR_SW3)
    {
        DBG_DUMP("^GPowerOn Src = USB plug\r\n");
    }
    else if(pwsrc == RTC_PWRON_SRC_PWR_SW4)
    {
        DBG_DUMP("^GPowerOn Src = DC plug\r\n");
    }
    else if(pwsrc == RTC_PWRON_SRC_PWR_ALM)
    {
        DBG_DUMP("^GPowerOn Src = PWR alarm\r\n");
    }
}

/**
  Do GPIO initialization

  Initialize input/output pins, and pin status

  @param void
  @return void
*/
void IO_InitGPIO(void)
{
    UINT32 iSValue;

    DBG_IND("GPIO START\r\n");
    //--------------------------------------------------------------------
    // Open GPIO driver
    //--------------------------------------------------------------------
    #if 1 //_MIPS_TODO
    gpio_open();
    for(iSValue=0 ; iSValue<totalGpioInitCount ; iSValue++)
    {
        if (uiGPIOMapInitTab[iSValue].GpioDir == GPIO_DIR_INPUT)
        {
            gpio_setDir(uiGPIOMapInitTab[iSValue].GpioPin, GPIO_DIR_INPUT);
            pad_setPullUpDown(uiGPIOMapInitTab[iSValue].PadPin, uiGPIOMapInitTab[iSValue].PadDir);
        }
        else
        {
            gpio_setDir(uiGPIOMapInitTab[iSValue].GpioPin, GPIO_DIR_OUTPUT);
            if (uiGPIOMapInitTab[iSValue].PadDir == GPIO_SET_OUTPUT_HI)
            {
                gpio_setPin(uiGPIOMapInitTab[iSValue].GpioPin);
            }
            else
            {
                gpio_clearPin(uiGPIOMapInitTab[iSValue].GpioPin);
            }
        }
    }
    #endif

    //--------------------------------------------------------------------
    // Use Non-Used PWM to be Delay Timer
    //--------------------------------------------------------------------
    #if defined(PWMID_TIMER)
    Delay_setPwmChannels(PWMID_TIMER);
    #endif

    DBG_IND("GPIO END\r\n");

}

/**
  Initialize voltage detection

  Initialize voltage detection for battery and flash

  @param void
  @return void
*/
void IO_InitADC(void)
{
    if (adc_open(ADC_CH_VOLDET_BATTERY) != E_OK)
    {
        DBG_ERR("Can't open ADC channel for battery voltage detection\r\n");
        return;
    }

    if (adc_open(ADC_CH_VOLDET_KEY1) != E_OK)
    {
        DBG_ERR("Can't open ADC channel for key key1 detection\r\n");
        return;
    }
    if (adc_open(ADC_CH_VOLDET_KEY2) != E_OK)
    {
        DBG_ERR("Can't open ADC channel for key key1 detection\r\n");
        return;
    }

    //650 Range is 250K Hz ~ 2M Hz
    adc_setConfig(ADC_CONFIG_ID_OCLK_FREQ, 250000); //250K Hz

    //battery voltage detection
    adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
    adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
    adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_INTEN, FALSE);

    //key key1 detection
    adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
    adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
    adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_INTEN, FALSE);

    //key key2 detection
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_INTEN, FALSE);

    // Enable adc control logic
    adc_setEnable(TRUE);

    Delay_DelayMs(15); //wait ADC stable  //for pwr on speed up
}

#if _MIPS_TODO
static char ADCStr1[32];
static char ADCStr2[32];

char* VolDet_GetStatusString1(void)
{
    sprintf(ADCStr1, "A0=%ld,A1=%ld,A2=%ld,A3=%ld", adc_readData(0), adc_readData(1),adc_readData(2),adc_readData(3));
    return ADCStr1;
}

char* VolDet_GetStatusString2(void)
{
    sprintf(ADCStr2, "A4=%ld,A5=%ld,A6=%ld,A7=%ld", adc_readData(4), adc_readData(5),adc_readData(6),adc_readData(7));
    return ADCStr2;
}
#endif

void Dx_InitIO(void)  // Config IO for external device
{
    IO_InitPinmux(); //initial PINMUX config
    IO_InitGPIO(); //initial GPIO pin status
    IO_InitADC(); //initial ADC pin status

#if (_HDMITYPE_ == _HDMI_ON_)
    //these two pins are default pull-up for GPIO,but must turn off for HDMI DDC I2C
    pad_setPullUpDown(PAD_PIN_PGPIO28, PAD_NONE);
    pad_setPullUpDown(PAD_PIN_PGPIO29, PAD_NONE);
#endif
}

void Dx_UninitIO(void)  // Config IO for external device
{
    // Disable adc control logic
    adc_setEnable(FALSE);
    adc_close(ADC_CH_VOLDET_BATTERY);
    adc_close(ADC_CH_VOLDET_KEY1);
    adc_close(ADC_CH_VOLDET_KEY2);
}

