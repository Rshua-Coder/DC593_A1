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
#include "KeyDef.h"
#include "Rtc.h"
#include "IrRx.h"
#include "IrRxParser.h"
#include "GSensor.h"

#define GPIO_KEY            ENABLE
#define ADC_KEY             DISABLE
#define IRRX_KEY            DISABLE//DISABLE

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
#define VOLDET_KEY_ADC_RANGE        (5)
#define VOLDET_KEY_ADC_LVL0         (0)
#define VOLDET_KEY_ADC_LVL1         (5-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_LVL2         (250-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_TH           (250+VOLDET_KEY_ADC_RANGE)

#define VOLDET_KEY_LVL_UNKNOWN      0xFFFFFFFF
#define VOLDET_KEY_LVL_MENU         0
#define VOLDET_KEY_LVL_REC          1
#define VOLDET_KEY_LVL_UP           2
#define VOLDET_KEY_LVL_DOWN         3
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
static UINT32 VolDet_GetKey1ADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(ADC_CH_VOLDET_KEY1);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(ADC_CH_VOLDET_KEY1);

    return uiADCValue;
#else
    return adc_readData(ADC_CH_VOLDET_KEY1);
#endif
}

static UINT32 VolDet_GetKey2ADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(ADC_CH_VOLDET_KEY2);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(ADC_CH_VOLDET_KEY2);

    return uiADCValue;
#else
    return adc_readData(ADC_CH_VOLDET_KEY2);
#endif
}


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
    if (uiKey1ADC <1000)
        DBG_IND("uiKey1ADC %d \r\n", uiKey1ADC);
    if (uiKey1ADC < VOLDET_KEY_ADC_TH)
    {
        if (uiKey1ADC >= VOLDET_KEY_ADC_LVL2)
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_DOWN;
        }
        else if (uiKey1ADC >= VOLDET_KEY_ADC_LVL1)
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_UP;
        }
        else
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_UNKNOWN;
        }
    }
    else
    {
        uiCurKey2Lvl = VOLDET_KEY_LVL_UNKNOWN;
    }

    uiRetKey1Lvl = uiCurKey2Lvl;

    return uiRetKey1Lvl;
}

static UINT32 VolDet_GetKey2Level(void)
{
    static UINT32   uiRetKey1Lvl;
    UINT32          uiKey2ADC, uiCurKey2Lvl;

    uiKey2ADC = VolDet_GetKey2ADC();
    if (uiKey2ADC <1000)
        DBG_IND("uiKey2ADC %d \r\n", uiKey2ADC);
    if (uiKey2ADC < VOLDET_KEY_ADC_TH)
    {
        if (uiKey2ADC >= VOLDET_KEY_ADC_LVL2)
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_REC;
        }
        else if (uiKey2ADC >= VOLDET_KEY_ADC_LVL1)
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_MENU;
        }
        else
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_UNKNOWN;
        }
    }
    else
    {
        uiCurKey2Lvl = VOLDET_KEY_LVL_UNKNOWN;
    }

    uiRetKey1Lvl = uiCurKey2Lvl;

    return uiRetKey1Lvl;
}

#endif

/**
  Get IrRx key

  Get IrRx key.

  @param void
  @return UINT32 key level, refer to VoltageDet.h -> VOLDET_MS_LVL_XXXX
*/
#if (IRRX_KEY == ENABLE)
static BOOL IrRxDet_Key(UINT32 *uiKey)
{
  IRRX_DATA_QUEUE IrRxGetDataQ = {0, 0, 0};
  static UINT32 uiIrRxData = 0;
  static UINT32 uiPreIrRxData = 0;

    if(irrx_dequeueData((PIRRX_DATA_QUEUE)&IrRxGetDataQ))
    {
        uiIrRxData = IrRxGetDataQ.uiRxData;
        debug_ind(("IrRx(%010lu): CODE: 0x%08X\r\n", IrRxGetDataQ.uiRxTime, uiIrRxData));

        *uiKey = IrRx_ParseKey(g_pIrCodeTb, uiIrRxData);
        uiPreIrRxData = *uiKey;
         return TRUE;

    } else if (uiPreIrRxData!=0)
    {
        // return IR key one more time because keyscan time interval is smaller than IR key scan time interval
        // and this will get zero value when keyscan task get data the sencondary time
        *uiKey = uiPreIrRxData;
        // clear uiPreIrRxData
        uiPreIrRxData = 0;
        return TRUE;
    }
    *uiKey = 0;
    return FALSE;
}
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

static UINT32 KeyScan_DetGsensorEvent(void)
{
#if 1
    Gsensor_Data *GS_Data=NULL;
    UINT32 uiKeyCode = 0;
    static BOOL gcal=FALSE;

    //if ((GxUSB_GetConnectType()!= USB_CONNECT_PC) && (GxUSB_GetConnectType() != USB_CONNECT_CHARGING_PC))
         gcal = GSensor_GetData(GS_Data);

    if(gcal==TRUE)
    {
        //uiKeyCode |= FLGKEY_CUSTOM2;
        // debug_err(("KEY FLGKEY_CUSTOM1\r\n"));
    }

    return uiKeyCode;
#endif
    }

void DrvKey_Init(void)
{
}


BOOL GPIOMap_DetMovieSize(void)
{
  // debug_err(("GPIOMap_DetMovieSize =%d!\r\n",gpio_getPin(GPIO_KEY_VIDEO_SW)));
   return (BOOL)(gpio_getPin(GPIO_KEY_VIDEO_SW)? FALSE : TRUE);
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
     UINT32 uiKeyCode = 0,temp=0;

#if (IRRX_KEY==ENABLE)
    static UINT32 uiIrRxKey = 0;
#endif

#if (ADC_KEY == ENABLE)
    if (VolDet_GetKey1Level() == VOLDET_KEY_LVL_UP)
    {
        uiKeyCode |= FLGKEY_UP;
    }
    if (VolDet_GetKey1Level() == VOLDET_KEY_LVL_DOWN)
    {
        uiKeyCode |= FLGKEY_DOWN;
    }
    if (VolDet_GetKey2Level() == VOLDET_KEY_LVL_MENU)
    {
        uiKeyCode |= FLGKEY_MENU;
    }
    if (VolDet_GetKey2Level() == VOLDET_KEY_LVL_REC)
    {
        uiKeyCode |= FLGKEY_SHUTTER2;
    }
#endif
#if (GPIO_KEY == ENABLE)
    if(!gpio_getPin(GPIO_KEY_SHUTTER2))
    {
        uiKeyCode |= FLGKEY_SHUTTER2;
    }
#endif

    //detect if power-on by press playback key
    if (rtc_getPWRStatus())
    {
        uiKeyCode |= FLGKEY_CUSTOM1;
    }

    DBG_IND("KEY=%08x\r\n", uiKeyCode);
    temp = KeyScan_DetGsensorEvent();
    uiKeyCode |= temp;

#if (IRRX_KEY==ENABLE)
        if (IrRxDet_Key(&uiIrRxKey)==TRUE)
        {
            uiKeyCode |= uiIrRxKey;
        }
#endif
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
    static UINT32 count=0;
    if(rtc_getPWRStatus())
    {
        rtc_resetShutdownTimer();
        if(++count>15)
        // Reset shutdown timer
        uiKeyCode = FLGKEY_KEY_POWER;
    }
    else
    {
        count = 0;
    }
    return uiKeyCode;
}

UINT32 DrvKey_DetStatusKey(DX_STATUS_KEY_GROUP KeyGroup)
{
    UINT32 uiReturn = STATUS_KEY_LVL_UNKNOWN;
    switch (KeyGroup)
    {
        case DX_STATUS_KEY_GROUP1:
        if(gpio_getPin(GPIO_KEY_MODE_SW)==TRUE)
            uiReturn= STATUS_KEY_LVL_1;
        else
            uiReturn= STATUS_KEY_LVL_2;
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

void GPIOMap_CtlMotorStatus(BOOL En)
{
    if(En)
        gpio_setPin(GPIO_MOTOR_CTL);
    else
        gpio_clearPin(GPIO_MOTOR_CTL);
}

void GPIOMap_CtlWIFISHDN(BOOL En)
{
    if(En)
        gpio_setPin(GPIO_WIFI_SHDN);
    else
        gpio_clearPin(GPIO_WIFI_SHDN);

}

