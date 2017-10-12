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
#define RF_KEY                ENABLE//DISABLE

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
#define VOLDET_KEY_ADC_RANGE        (100)
#define VOLDET_KEY_ADC_LVL0         (0)
#define VOLDET_KEY_ADC_LVL1         (0)//(5-VOLDET_KEY_ADC_RANGE)
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
static UINT32 KeyScan_DetGsensorEvent(void)
{
#if 1//(GSENSOR_FUNCTION == ENABLE)
    UINT32 uiKeyCode = 0;
    BOOL gcal=FALSE;

    Gsensor_Data *GS_Data;

    if(0)//(GSensor_Modual_Detect())
    {
        gcal=GSensor_GetData(GS_Data);
        if(gcal==TRUE)
        {
            uiKeyCode |= FLGKEY_CUSTOM1;
            // debug_err(("KEY FLGKEY_CUSTOM1\r\n"));
        }
    }
    return uiKeyCode;
#endif
}
static UINT32 ModeTimerCount=0;
static UINT32 KeyVideoPressTimerCount=0;
static UINT32 KeyPhotoPressTimerCount=0;
static UINT32 KeyVideoReleaseTimerCount=0;
static UINT32 KeyPhotoReleaseTimerCount=0;
static BOOL bVideoLongKey_Std = FALSE;
static BOOL bPhotoLongKey_Std = FALSE;
static BOOL LONGKEY_STD = FALSE;
static BOOL DrvKey_DetLongKeyStatus(void)
{
    return LONGKEY_STD;
}
static void DrvKey_SetLongKeyStatus(BOOL std)
{
    LONGKEY_STD = std;
}
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
    if(!gpio_getPin(GPIO_KEY_ENTER))
    {
        uiKeyCode |= FLGKEY_ENTER;
        //debug_err(("Enter!!!:%d\r\n",EnterTimerCount));
    }
    if(!gpio_getPin(GPIO_KEY_MODE))
    {
        uiKeyCode |= FLGKEY_MODE;

        if(DrvKey_DetLongKeyStatus() == FALSE)
            ++ModeTimerCount;
        if(ModeTimerCount > 100)
        {
            uiKeyCode |= FLGKEY_LONGMODE;
            //debug_msg("FLGKEY_LONGDOWN!!\r\n");
            if(ModeTimerCount > 102)           //key without jitter
            {
                DrvKey_SetLongKeyStatus(TRUE);
                ModeTimerCount = 0;
            }
         }
    }
    else
    {
        ModeTimerCount = 0;
        DrvKey_SetLongKeyStatus(FALSE);
    }

       //detect if power-on by press playback key
       if (rtc_getPWRStatus())
       {
           uiKeyCode |= FLGKEY_DOWN;
       }
#endif
/*
    //detect if power-on by press playback key
    if (!rtc_getPWR2Status())
    {
        uiKeyCode |= FLGKEY_PLAYBACK;
    }
*/
#if (IRRX_KEY==ENABLE)
        if (IrRxDet_Key(&uiIrRxKey)==TRUE)
        {
            uiKeyCode |= uiIrRxKey;
        }
#endif
    DBG_IND("KEY=%08x\r\n", uiKeyCode);
#if 1//(GSENSOR_FUNCTION == ENABLE)
    temp = KeyScan_DetGsensorEvent();
    uiKeyCode |= temp;
#endif
#if (RF_KEY==ENABLE)
        if (!gpio_getPin(GPIO_RF_STD))
        {
            if(!gpio_getPin(GPIO_RF_VIDEO_WIFI))
            {
                KeyVideoPressTimerCount++;
                if((KeyVideoPressTimerCount>80)&&(!bVideoLongKey_Std))
                {
                    //debug_msg("FLGKEY_WIFI  %d!!\r\n",KeyVideoPressTimerCount);
                    uiKeyCode |= FLGKEY_LONGMODE;
                    if(KeyVideoPressTimerCount>82)
                    {
                        bVideoLongKey_Std=TRUE;
                    }
                 }
            }
            else
        {
             if((KeyVideoPressTimerCount>0)&&(!bVideoLongKey_Std)&&(KeyVideoReleaseTimerCount<3))
             {
                 //debug_msg("FLGKEY_MOVIE   %d!!\r\n",KeyVideoPressTimerCount);
                 uiKeyCode |= FLGKEY_MOVIE;
                 KeyVideoReleaseTimerCount++;
             }
             else
             {
                 bVideoLongKey_Std=FALSE;
                 KeyVideoPressTimerCount = 0;
                 KeyVideoReleaseTimerCount=0;
             }
             }
              if(!gpio_getPin(GPIO_RF_PHOTO_POWER))
              {
              KeyPhotoPressTimerCount++;
                if((KeyPhotoPressTimerCount>25)&&(!bPhotoLongKey_Std))
                {
                 //debug_msg("FLGKEY_KEY_POWER  %d!!\r\n",KeyPhotoPressTimerCount);
                    uiKeyCode = FLGKEY_KEY_POWER;
                  if(KeyVideoPressTimerCount>82)
                  bPhotoLongKey_Std=TRUE;
                 }
        }
        else
        {
        if((KeyPhotoPressTimerCount>0)&&(!bPhotoLongKey_Std)&&(KeyPhotoReleaseTimerCount<3))
        {
                 //debug_msg("FLGKEY_PHOTO  %d!!\r\n",KeyPhotoPressTimerCount);
              uiKeyCode |= FLGKEY_PHOTO;
              KeyPhotoReleaseTimerCount++;
              }
             else
             {
                  bPhotoLongKey_Std=FALSE;
              KeyPhotoPressTimerCount = 0;
              KeyPhotoReleaseTimerCount=0;
             }
             }
         }
#endif
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
       if(++count >15)//35)
       {
            // Reset shutdown timer
            uiKeyCode = FLGKEY_KEY_POWER;
       }

    }
    else
    {
        count =0;
    }
    return uiKeyCode;
}

UINT32 DrvKey_DetStatusKey(DX_STATUS_KEY_GROUP KeyGroup)
{
    UINT32 uiReturn = STATUS_KEY_LVL_UNKNOWN;
    switch (KeyGroup)
    {
        case DX_STATUS_KEY_GROUP1:
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

#if 0//(RF_KEY==ENABLE)
static void RF_SetPairingMode(BOOL bStd)
{
    if(bStd)
    {
        //debug_msg("GPIO_RF_PAIRING SET HI\r\n");
        gpio_setPin(GPIO_RF_PAIRING);
    }
    else
    {
        //debug_msg("GPIO_RF_PAIRING SET LO\r\n");
        gpio_clearPin(GPIO_RF_PAIRING);
    }
}

static BOOL RF_GetPairingStd(void)
{
    if(!gpio_getPin(GPIO_RF_STD))
        return TRUE;
    else
    return FALSE;
}
#endif
