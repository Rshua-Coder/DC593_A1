/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       DetKey.c
    @ingroup    mIPRJAPKeyIO

    @brief      Scan key, modedial
                Scan key, modedial

    @note       Nothing.

    @date       2009/04/22
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "DxCfg.h"
#include "IOCfg.h"

#include "DxInput.h"
#include "Debug.h"
#include "Rtc.h"
#include "KeyDef.h"

#define GPIO_KEY            ENABLE//DISABLE
#define ADC_KEY             DISABLE //ENABLE//DISABLE

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// ADC related
////////////////////////////////////////////////////////////////////////////////
// ADC related
//#NT#2009/11/16#Lincy -begin
#if (ADC_KEY == ENABLE)
#define VOLDET_KEY_ADC_RANGE         (110)
#define VOLDET_KEY_ADC_LVL0          (212-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_LVL1          (420-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_LVL2          (628-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_LVL3          (837-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_TH            (1023-VOLDET_KEY_ADC_RANGE)

#define VOLDET_KEY_LVL_UNKNOWN           0xFFFFFFFF
#define VOLDET_KEY_LVL_0                 0
#define VOLDET_KEY_LVL_1                 1
#define VOLDET_KEY_LVL_2                 2
#define VOLDET_KEY_LVL_3                 3
#define VOLDET_KEY_LVL_4                 4
#define VOLDET_KEY_LVL_5                 5
#endif

#define VOLDET_MS_ADC_RANGE         (60)
#define VOLDET_MS_ADC_LVL0          (0)
#define VOLDET_MS_ADC_LVL1          (262-VOLDET_MS_ADC_RANGE)
#define VOLDET_MS_ADC_LVL2          (525-VOLDET_MS_ADC_RANGE)
#define VOLDET_MS_ADC_LVL3          (788-VOLDET_MS_ADC_RANGE)
#define VOLDET_MS_ADC_TH            (788+VOLDET_MS_ADC_RANGE)

#define VOLDET_MS_LVL_UNKNOWN           0xFFFFFFFF
#define VOLDET_MS_LVL_0                 0
#define VOLDET_MS_LVL_1                 1
#define VOLDET_MS_LVL_2                 2
#define VOLDET_MS_LVL_3                 3
#define VOLDET_MS_LVL_4                 4
#define VOLDET_MS_LVL_5                 5

#if (ADC_KEY == ENABLE)
#if 0
static UINT32 VolDet_GetModeSwitch1ADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(ADC_CH_VOLDET_MS1);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(ADC_CH_VOLDET_MS1);

    return uiADCValue;
#else
    return adc_readData(ADC_CH_VOLDET_MS1);
#endif
}

static UINT32 VolDet_GetModeSwitch2ADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(ADC_CH_VOLDET_MS2);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(ADC_CH_VOLDET_MS2);

    return uiADCValue;
#else
    return adc_readData(ADC_CH_VOLDET_MS2);
#endif
}
#endif
static UINT32 VolDet_GetKey1ADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(ADC_CH_KEYDET_KEY1);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(ADC_CH_KEYDET_KEY1);

    return uiADCValue;
#else
    return adc_readData(ADC_CH_KEYDET_KEY1);
#endif
}

/**
  Get Mode Switch 1 voltage level

  Get Mode Switch 1 voltage level.

  @param void
  @return UINT32 Mode Switch 1, refer to VoltageDet.h -> VOLDET_MS_LVL_XXXX
*/
#if 0
static UINT32 VolDet_GetModeSwitch1Level(void)
{
    //static UINT32   uiPreMS1Lvl = VOLDET_MS_LVL_UNKNOWN;
    static UINT32   uiRetMS1Lvl;
    UINT32          uiMS1ADC, uiCurMS1Lvl;

    uiMS1ADC = VolDet_GetModeSwitch1ADC();

    if (uiMS1ADC < VOLDET_MS_ADC_TH) //768+30
    {
        if (uiMS1ADC > VOLDET_MS_ADC_LVL3) //768-30
        {
            uiCurMS1Lvl = VOLDET_MS_LVL_3;
        }
        else if (uiMS1ADC > VOLDET_MS_ADC_LVL2)//512-30
        {
            uiCurMS1Lvl = VOLDET_MS_LVL_2;
        }
        else if (uiMS1ADC > VOLDET_MS_ADC_LVL1)//256-30
        {
            uiCurMS1Lvl = VOLDET_MS_LVL_1;
        }
        else if (uiMS1ADC < VOLDET_MS_ADC_TH)// <30
        {
            uiCurMS1Lvl = VOLDET_MS_LVL_0;
        }
        else
        {
            uiCurMS1Lvl = VOLDET_MS_LVL_UNKNOWN;
        }
    }
    else
    {
        uiCurMS1Lvl = VOLDET_MS_LVL_UNKNOWN;
    }

    uiRetMS1Lvl = uiCurMS1Lvl;

    return uiRetMS1Lvl;
}

/**
  Get Mode Switch 2 voltage level

  Get Mode Switch 2 voltage level.

  @param void
  @return UINT32 Mode Switch 2, refer to VoltageDet.h -> VOLDET_MS_LVL_XXXX
*/
static UINT32 VolDet_GetModeSwitch2Level(void)
{
    static UINT32   uiRetMS2Lvl;
    UINT32          uiMS2ADC, uiCurMS2Lvl;

    uiMS2ADC = VolDet_GetModeSwitch2ADC();

    if (uiMS2ADC < VOLDET_MS_ADC_TH) //768+30
    {
        if (uiMS2ADC > VOLDET_MS_ADC_LVL3) //768-30
        {
            uiCurMS2Lvl = VOLDET_MS_LVL_3;
        }
        else if (uiMS2ADC > VOLDET_MS_ADC_LVL2) //512-30
        {
            uiCurMS2Lvl = VOLDET_MS_LVL_2;
        }
        else if (uiMS2ADC > VOLDET_MS_ADC_LVL1) //256-30
        {
            uiCurMS2Lvl = VOLDET_MS_LVL_1;
        }
        else if (uiMS2ADC < VOLDET_MS_ADC_TH) //<30
        {
            uiCurMS2Lvl = VOLDET_MS_LVL_0;
        }
        else
        {
            uiCurMS2Lvl = VOLDET_MS_LVL_UNKNOWN;
        }
    }
    else
    {
        uiCurMS2Lvl = VOLDET_MS_LVL_UNKNOWN;
    }

    uiRetMS2Lvl = uiCurMS2Lvl;

    return uiRetMS2Lvl;
}
#endif
/**
  Get ADC key voltage level

  Get  ADC key  2 voltage level.

  @param void
  @return UINT32 key level, refer to VoltageDet.h -> VOLDET_MS_LVL_XXXX
*/
static UINT32 VolDet_GetKey1Level(void)
{
    static UINT32   uiRetKey1Lvl;
    UINT32          uiKey1ADC, uiCurKey2Lvl;

    uiKey1ADC = VolDet_GetKey1ADC();
    DBG_IND("uiKey1ADC %d \r\n", uiKey1ADC);
    if (uiKey1ADC < VOLDET_KEY_ADC_TH)
    {
        if (uiKey1ADC > VOLDET_KEY_ADC_LVL3)
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_4;
        }
        else if (uiKey1ADC > VOLDET_KEY_ADC_LVL2)
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_3;
        }
        else if (uiKey1ADC > VOLDET_KEY_ADC_LVL1)
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_2;
        }
        else if (uiKey1ADC > VOLDET_KEY_ADC_LVL0)
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_1;
        }
        else
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_0;
        }
    }
    else
    {
        uiCurKey2Lvl = VOLDET_KEY_LVL_UNKNOWN;
    }

    uiRetKey1Lvl = uiCurKey2Lvl;

    return uiRetKey1Lvl;
}

/**
  Detect Mode Switch state.

  Detect Mode Switch state.

  @param void
  @return UINT32 Mode Switch state (DSC Mode)
*/
#if 0
static UINT32 VolDet_ModeSwitch(void)
{
    UINT32 uiModeSwitchCode = 0;
    UINT32 uiMS1Level, uiMS2Level;

    uiMS1Level = VolDet_GetModeSwitch1Level();
    uiMS2Level = VolDet_GetModeSwitch2Level();

    DBG_IND("uiMS1Level = %d, uiMS2Level = %d\r\n",uiMS1Level,uiMS2Level);

    if(uiMS1Level != VOLDET_MS_LVL_UNKNOWN)
    {
        if(uiMS1Level == VOLDET_MS_LVL_0)
        {
            uiModeSwitchCode = STATUS_KEY_LVL_7;//VOLDET_MS_MOVIE;
        }
        else if(uiMS1Level == VOLDET_MS_LVL_1)
        {
            uiModeSwitchCode = STATUS_KEY_LVL_8;//VOLDET_MS_ASCENE;
        }
        else if(uiMS1Level == VOLDET_MS_LVL_2)
        {
            uiModeSwitchCode = STATUS_KEY_LVL_1;//VOLDET_MS_PHOTO_AUTO;
        }
        else if(uiMS1Level == VOLDET_MS_LVL_3)
        {
            uiModeSwitchCode =  STATUS_KEY_LVL_2;//VOLDET_MS_PHOTO_MANUAL;
        }
    }
    else if(uiMS2Level != VOLDET_MS_LVL_UNKNOWN)
    {
        if(uiMS2Level == VOLDET_MS_LVL_0)
        {
            uiModeSwitchCode = STATUS_KEY_LVL_3;//VOLDET_MS_PHOTO_PORTRAIT;
        }
        else if(uiMS2Level == VOLDET_MS_LVL_1)
        {
            uiModeSwitchCode = STATUS_KEY_LVL_4;//VOLDET_MS_PHOTO_PANORAMA;
        }
        else if(uiMS2Level == VOLDET_MS_LVL_2)
        {
            uiModeSwitchCode = STATUS_KEY_LVL_5;//VOLDET_MS_PHOTO_SCENE;
        }
        else if(uiMS2Level == VOLDET_MS_LVL_3)
        {
            uiModeSwitchCode = STATUS_KEY_LVL_6;//VOLDET_MS_PHOTO_STAB;
        }

    }
    else
    {
        uiModeSwitchCode = STATUS_KEY_LVL_UNKNOWN;//VOLDET_MS_PHOTO_AUTO;
    }
    DBG_IND("uiModeSwitchCode = %d\r\n",uiModeSwitchCode);

    return uiModeSwitchCode;
}
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
// GPIO related

//static BOOL g_bIsShutter2Pressed = FALSE;



/**
  Delay between toggle GPIO pin of input/output

  Delay between toggle GPIO pin of input/output

  @param void
  @return void
*/
static void DrvKey_DetKeyDelay(void)
{
    gpio_readData(0);
    gpio_readData(0);
    gpio_readData(0);
    gpio_readData(0);
}


void DrvKey_Init(void)
{
}
/**
  Detect normal key is pressed or not.

  Detect normal key is pressed or not.
  Return key pressed status (refer to KeyDef.h)

  @param void
  @return UINT32
*/
UINT32 DrvKey_DetNormalKey(void)
{
    UINT32 uiKeyCode = 0;

#if 0 //(ADC_KEY == ENABLE)
    UINT32 uiKey1Lvl = VolDet_GetKey1Level();
    switch(uiKey1Lvl)
    {
        case VOLDET_KEY_LVL_UNKNOWN:
        default:
            break;
        case VOLDET_KEY_LVL_0:
            uiKeyCode |= FLGKEY_ENTER;
            break;
        case VOLDET_KEY_LVL_1:
            uiKeyCode |= FLGKEY_UP;
            break;
        case VOLDET_KEY_LVL_2:
            uiKeyCode |= FLGKEY_LEFT;
            break;
        case VOLDET_KEY_LVL_3:
            uiKeyCode |= FLGKEY_DOWN;
            break;
        case VOLDET_KEY_LVL_4:
            uiKeyCode |= FLGKEY_RIGHT;
            break;
    }
#endif
#if 0//(GPIO_KEY == ENABLE)
    if(!gpio_getPin(GPIO_KEY_UP))
    {
        uiKeyCode |= FLGKEY_UP;
    }
    if(!gpio_getPin(GPIO_KEY_DOWN))
    {
        uiKeyCode |= FLGKEY_DOWN;
    }
    if(!gpio_getPin(GPIO_KEY_LEFT))
    {
        uiKeyCode |= FLGKEY_LEFT;
    }
    if(!gpio_getPin(GPIO_KEY_RIGHT))
    {
        uiKeyCode |= FLGKEY_RIGHT;
    }
#endif
#if (GPIO_KEY == ENABLE)
    if(gpio_getPin(GPIO_KEY_SHUTTER1))
    {
        uiKeyCode |= FLGKEY_SHUTTER1;
    }
    if(gpio_getPin(GPIO_KEY_SHUTTER2))
    {
        uiKeyCode |= FLGKEY_SHUTTER2;
    }
    if(gpio_getPin(GPIO_KEY_ZOOMIN))
    {
        uiKeyCode |= FLGKEY_ZOOMIN;
    }
    if(gpio_getPin(GPIO_KEY_ZOOMOUT))
    {
        uiKeyCode |= FLGKEY_ZOOMOUT;
    }
    if(gpio_getPin(GPIO_KEY_MENU))
    {
        uiKeyCode |= FLGKEY_MENU;
    }
    if(gpio_getPin(GPIO_KEY_MODE))
    {
        uiKeyCode |= FLGKEY_MODE;
    }
    if (rtc_getPWR2Status())
    {
        uiKeyCode |= FLGKEY_PLAYBACK;
    }
#endif
/*
    //detect if power-on by press playback key
    if (!rtc_getPWR2Status())
    {
        uiKeyCode |= FLGKEY_PLAYBACK;
    }
*/
    DBG_IND("KEY=%08x\r\n", uiKeyCode);

    DrvKey_DetKeyDelay();
    return uiKeyCode;
}
/**
  Detect power key is pressed or not.

  Detect power key is pressed or not.
  Return key pressed status (refer to KeyDef.h)

  @param void
  @return UINT32
*/
UINT32 DrvKey_DetPowerKey(void)
{
    UINT32 uiKeyCode = 0;

    if(rtc_getPWRStatus())
    {
        uiKeyCode = FLGKEY_KEY_POWER;
        // Reset shutdown timer
        rtc_resetShutdownTimer();
    }

    return uiKeyCode;
}
UINT32 DrvKey_DetStatusKey(DX_STATUS_KEY_GROUP KeyGroup)
{
    UINT32 uiReturn = STATUS_KEY_LVL_UNKNOWN;
    switch (KeyGroup)
    {
        case DX_STATUS_KEY_GROUP1:
#if 0//(ADC_KEY == ENABLE)
            uiReturn = VolDet_ModeSwitch();
#endif
            break;

        case DX_STATUS_KEY_GROUP2:
            break;

        case DX_STATUS_KEY_GROUP3:
            break;

        case DX_STATUS_KEY_GROUP4:
            break;

        case DX_STATUS_KEY_GROUP5:
            break;

        default:
            DBG_ERR("[StatusKey]no this attribute");
            break;
    }
    return uiReturn;
}
