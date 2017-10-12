#include "DxCfg.h"
#include "IOCfg.h"

#include "DxPower.h"
#include "Utility.h"

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxPwr
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


#define TEMPDET_FUNCTION DISABLE
#define TEMPDET_TEST     DISABLE
#define DUMMY_LOAD                   1
#define BATT_SLIDE_WINDOW_COUNT      8

// ADC = * 1023 * 0.89 * v/4.2v   ;
// V = ADC*4.2/910
#define VOLDET_BATTERY_420V         635
#define VOLDET_BATTERY_390V         594//600
#define VOLDET_BATTERY_380V         588
#define VOLDET_BATTERY_375V         584
#define VOLDET_BATTERY_370V         579
#define VOLDET_BATTERY_365V         564
#define VOLDET_BATTERY_360V         556

DummyLoadType dummyLoadData[11];

//***********************************************
//*
//*    Battery Rule depend on Model
//*
//***********************************************

//#define VOLDET_BATTERY_ADC_TH       0

#if TEMPDET_TEST
INT32 temperature_value=0;
#endif

#if USB_CHARGE_FUNCTION
static INT32  gTempValue = 25;
#endif

#if USB_CHARGE_FUNCTION
static UINT32  u32BattChargeCurrent = BATTERY_CHARGE_CURRENT_LOW;
#endif



//------------------ Battery Status Level -------------------//
#define  BATT_LEVEL_COUNT  4




#define  LENS_MOVE_MAX_COUNT         10

static UINT32  LiBattAdcLevelValue[BATT_LEVEL_COUNT]={
VOLDET_BATTERY_360V,
VOLDET_BATTERY_370V,
VOLDET_BATTERY_380V,
VOLDET_BATTERY_390V,
};

static UINT32* pBattAdcLevelValue= &LiBattAdcLevelValue[0];
static UINT32  uiBattADCSlideWin[BATT_SLIDE_WINDOW_COUNT]={0};
static UINT8   uiBattSlideIdx = 0;
static UINT8   uiCurSlideWinCnt = 0;
static INT32   iBattAdcCalOffset = 0;
#if USB_CHARGE_FUNCTION
static UINT32  u32BattChargeCurrent = BATTERY_CHARGE_CURRENT_LOW;
#endif

#if USB_CHARGE_FUNCTION
static INT32   DrvPower_GetTempCompentation(INT32 tempValue);
#endif

//static UINT32 lensMoveCount = 0;


static UINT32  bDummyLoadPwrOff = FALSE;

static DX_CALLBACK_PTR   g_fpDxPowerCB = NULL;

/**
  Get battery voltage ADC value

  Get battery voltage ADC value

  @param void
  @return UINT32 ADC value
*/
static UINT32 DrvPower_GetBatteryADC(void)
{
    UINT32 uiADCValue;

    uiADCValue = (adc_readData(ADC_CH_VOLDET_BATTERY));
    return uiADCValue;

}
//#NT#2009/09/02#Lincy Lin -begin
//#Add function for check battery overheat
/**
  Get battery

  Get battery Temperature ADC value

  @param void
  @return UINT32 ADC value
*/
static BOOL DrvPower_IsBattOverHeat(void)
{
    return FALSE;
}
//#NT#2009/09/02#Lincy Lin -end




/**
  Get battery voltage level

  Get battery voltage level.
  If battery voltage level is DRVPWR_BATTERY_LVL_EMPTY, it means
  that you have to power off the system.

  @param void
  @return UINT32 Battery Level, refer to VoltageDet.h -> VOLDET_BATTERY_LVL_XXXX
*/

#define ADC_CH_DEBUG 0
static UINT32 DrvPower_GetBatteryLevel(void)
{
    static UINT32   uiPreBatteryLvl    = DRVPWR_BATTERY_LVL_UNKNOWN;
    static UINT32   uiPreBatteryADC    = 0;
    static UINT32   uiRetBatteryLvl;
    static UINT32   uiEmptycount =0;
    static UINT32   uiCount = 0;
    UINT32          uiCurBatteryADC, uiCurBatteryLvl,i;
    BOOL            isMatch = FALSE;

    uiCurBatteryLvl = 0;
    if(uiPreBatteryLvl== DRVPWR_BATTERY_LVL_UNKNOWN)
    {
        uiCurBatteryADC = DrvPower_GetBatteryADC();
        uiPreBatteryADC = DrvPower_GetBatteryADC()-1;
        for (i = 0;i<BATT_SLIDE_WINDOW_COUNT;i++)
        {
            uiBattADCSlideWin[i] = uiCurBatteryADC;
            //debug_msg("AVG=%d\r\n",uiCurBatteryADC);
        }
    }
    else
    {

        uiCurSlideWinCnt = BATT_SLIDE_WINDOW_COUNT;
        uiBattADCSlideWin[uiBattSlideIdx++] = DrvPower_GetBatteryADC();
        if (uiBattSlideIdx >= uiCurSlideWinCnt)
        {
            uiBattSlideIdx = 0;
        }
        uiCurBatteryADC = 0;
        for (i = 0;i<uiCurSlideWinCnt;i++)
        {
            uiCurBatteryADC+=uiBattADCSlideWin[i];
            DBG_MSG("A[%d]=%d,",i,uiBattADCSlideWin[i]);
        }
        uiCurBatteryADC/=uiCurSlideWinCnt;
        //debug_msg("AVG=%d",uiCurBatteryADC);
        //debug_msg(", V=%d",uiCurBatteryADC*42/6350);
        //debug_msg(".%02d\r\n",(uiCurBatteryADC*420/635)%100);
    }

#if ADC_CH_DEBUG
    debug_msg("^G ******uiCurBatteryADC:%d ******\r\n",uiCurBatteryADC);
    return DRVPWR_BATTERY_LVL_3;
#endif

    //DBG_IND("%d,%d,%d\r\n",VOLDET_BATTERY_ADC_LVL0,VOLDET_BATTERY_ADC_LVL1,VOLDET_BATTERY_ADC_LVL2);
    // Rising
    if (uiCurBatteryADC > uiPreBatteryADC)
    {
        for (i=BATT_LEVEL_COUNT;i>0;i--)
        {
            if (uiCurBatteryADC > pBattAdcLevelValue[i-1])
            {
                uiCurBatteryLvl = i;
                isMatch = TRUE;
                break;
            }
        }
        if (isMatch != TRUE)
        {
            uiCurBatteryLvl = 0;
        }
    }
    // Falling
    else
    {
        for (i=BATT_LEVEL_COUNT;i>0;i--)
        {
            if (uiCurBatteryADC > pBattAdcLevelValue[i-1])
            {
                uiCurBatteryLvl = i;
                isMatch = TRUE;
                break;
            }
        }
        if (isMatch != TRUE)
        {
            uiCurBatteryLvl = 0;
        }
    }
    // Debounce
    if ((uiCurBatteryLvl == uiPreBatteryLvl) ||
        (uiPreBatteryLvl == DRVPWR_BATTERY_LVL_UNKNOWN))
    {
        uiRetBatteryLvl = uiCurBatteryLvl;
    }
    uiPreBatteryLvl = uiCurBatteryLvl;
    uiPreBatteryADC = uiCurBatteryADC;

    if(uiCount % 2 == 0)
    {
        uiRetBatteryLvl = uiPreBatteryLvl;
    }
    uiCount++;
    //
    if(uiEmptycount|| uiRetBatteryLvl == DRVPWR_BATTERY_LVL_0)
    {
        uiEmptycount++;
        if (uiEmptycount >= 5)
        {
            return DRVPWR_BATTERY_LVL_EMPTY;
        }
    }

    return uiRetBatteryLvl;
}


void DrvPower_PowerOnInit(void)
{
    #if USB_CHARGE_FUNCTION
    gTempValue = DrvPower_BattTempGet();
    #endif
}

UINT32 DrvPower_DummyLoad(void)
{
    return TRUE;
}

static void DrvPower_BattADC_Calibration(BOOL enable)
{
}

BOOL Battery_isChargeFull(void)
{
    BOOL bvalue = FALSE;
    return bvalue = gpio_getPin(GPIO_USB_CHG_FULL);
}

void Battery_Charge_SetMode(BOOL bMode)
{
    if(bMode)
    {
        gpio_setPin(GPIO_CHG_CS_PCTL);
    }
    else
    {
        gpio_clearPin(GPIO_CHG_CS_PCTL);
    }
}

#if USB_CHARGE_FUNCTION
static void DrvPower_EnableChargeIC(BOOL bCharge)
{

}

static void DrvPower_ChargeBattEn(BOOL bCharge)
{

}


static void DrvPower_ChargeCurrentSet(UINT32 Current)
{

    u32BattChargeCurrent = Current;
}

static UINT32 DrvPower_ChargeCurrentGet(void)
{
    return u32BattChargeCurrent;
}

static void DrvPower_ChargeISet(BOOL isHigh)
{

}

static BOOL DrvPower_ChargeIGet(void)
{
    return 0;
}

static void DrvPower_ChargeVSet(BOOL isHigh)
{

}

static BOOL DrvPower_ChargeVGet(void)
{
    return 0;
}


static BOOL DrvPower_IsUnderCharge(void)
{
    return 0;
}

static BOOL DrvPower_IsUSBChargeOK(void)
{
    BOOL bvalue = FALSE;
    bvalue = gpio_getPin(GPIO_USB_CHG_FULL);
    return bvalue;
}

static BOOL DrvPower_IsBattIn(void)
{
    return TRUE;

}

static BOOL DrvPower_IsDeadBatt(void)
{
    return FALSE;
}

static BOOL DrvPower_IsNeedRecharge(void)
{
    return FALSE;
}

INT32 DrvPower_BattTempGet(void)
{
    return 25;
}


static INT32 DrvPower_GetTempCompentation(INT32 tempValue)
{
    return 0;
}
#endif

void   DrvPower_SetControl(DRVPWR_CTRL DrvPwrCtrl, UINT32 value)
{
    DBG_IND("DrvPwrCtrl(%d), value(%d)",DrvPwrCtrl,value);
    switch (DrvPwrCtrl)
    {
        case DRVPWR_CTRL_BATTERY_CALIBRATION_EN:
            DrvPower_BattADC_Calibration((BOOL)value);
            break;

        case DRVPWR_CTRL_BATTERY_ADC_CAL_OFFSET:
            iBattAdcCalOffset = (INT32)value;
            break;

        #if USB_CHARGE_FUNCTION
        case DRVPWR_CTRL_BATTERY_CHARGE_EN:
            DrvPower_ChargeBattEn((BOOL)value);
            break;
        case DRVPWR_CTRL_BATTERY_CHARGE_CURRENT:
            DrvPower_ChargeCurrentSet(value);
            break;

        case DRVPWR_CTRL_BATTERY_CHARGE_ISET:
            DrvPower_ChargeISet((BOOL)value);
            break;

        case DRVPWR_CTRL_BATTERY_CHARGE_VSET:
            DrvPower_ChargeVSet((BOOL)value);
            break;

        case DRVPWR_CTRL_ENABLE_CHARGEIC:
            DrvPower_EnableChargeIC((BOOL)value);
            break;

        #endif

        default:
            DBG_ERR("DrvPwrCtrl(%d)",DrvPwrCtrl);
            break;
    }
}

UINT32  DrvPower_GetControl(DRVPWR_CTRL DrvPwrCtrl)
{
    UINT32 rvalue = 0;
    switch (DrvPwrCtrl)
    {
        case DRVPWR_CTRL_BATTERY_LEVEL:
            rvalue = DrvPower_GetBatteryLevel();
            break;
        case DRVPWR_CTRL_BATTERY_ADC_VALUE:
            rvalue = DrvPower_GetBatteryADC();
            break;
        case DRVPWR_CTRL_BATTERY_ADC_CAL_OFFSET:
            rvalue = iBattAdcCalOffset;
            break;
        case DRVPWR_CTRL_IS_DUMMUYLOAD_POWEROFF:
            rvalue = bDummyLoadPwrOff;
            break;
        case DRVPWR_CTRL_IS_BATT_OVERHEAT:
            rvalue = (UINT32)DrvPower_IsBattOverHeat();
            break;
        #if USB_CHARGE_FUNCTION
        case DRVPWR_CTRL_IS_BATT_INSERT:
            rvalue = (UINT32)DrvPower_IsBattIn();
            break;
        case DRVPWR_CTRL_IS_DEAD_BATT:
            rvalue = (UINT32)DrvPower_IsDeadBatt();
            break;
        case DRVPWR_CTRL_IS_NEED_RECHARGE:
            rvalue = (UINT32)DrvPower_IsNeedRecharge();
            break;
        case DRVPWR_CTRL_BATTERY_CHARGE_EN:
            rvalue = (UINT32)DrvPower_IsUnderCharge();
            break;
        case DRVPWR_CTRL_BATTERY_CHARGE_OK:
            rvalue = (UINT32)DrvPower_IsUSBChargeOK();
            break;
        case DRVPWR_CTRL_BATTERY_CHARGE_CURRENT:
            rvalue = DrvPower_ChargeCurrentGet();
            break;
        case DRVPWR_CTRL_BATTERY_CHARGE_ISET:
            rvalue = (UINT32)DrvPower_ChargeIGet();
            break;
        case DRVPWR_CTRL_BATTERY_CHARGE_VSET:
            rvalue = (UINT32)DrvPower_ChargeVGet();
            break;
        /*case DRVPWR_CTRL_BATTERY_TEMPERATURE:
            rvalue = DrvPower_BattTempGet();
            break;
        */
        #else
        case DRVPWR_CTRL_BATTERY_CHARGE_EN:
            rvalue = FALSE;
            break;
        #endif
        default:
            DBG_ERR("DrvPwrCtrl(%d)",DrvPwrCtrl);
            break;
    }
    return rvalue;
}


void   DrvPower_RegCB(DX_CALLBACK_PTR fpDxPowerCB)
{

    g_fpDxPowerCB = fpDxPowerCB;
}


