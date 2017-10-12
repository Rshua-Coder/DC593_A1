#include "DxCfg.h"
#include "IOCfg.h"
#include "DxPower.h"
#include "Utility.h"
#include "DxFlash.h"

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxPwr
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


#define CHARGE_FUNCTION  DISABLE
#define TEMPDET_FUNCTION DISABLE
#define TEMPDET_TEST     DISABLE
#define ADC_TEST         ENABLE
#define DUMMY_LOAD                   0
#define BATT_SLIDE_WINDOW_COUNT      8

#if (CHARGE_FUNCTION == ENABLE)
#include "RT5024_i2c.h"
#endif
#define VOLDET_BATTERY_420V         3128
#define VOLDET_BATTERY_415V         3090
#define VOLDET_BATTERY_410V         3053
#define VOLDET_BATTERY_405V         3016
#define VOLDET_BATTERY_400V         2979
#define VOLDET_BATTERY_390V         2904
#define VOLDET_BATTERY_380V         2830
#define VOLDET_BATTERY_378V         2815
#define VOLDET_BATTERY_377V         2807
#define VOLDET_BATTERY_376V         2799
#define VOLDET_BATTERY_375V         2792
#define VOLDET_BATTERY_374V         2785
#define VOLDET_BATTERY_373V         2777
#define VOLDET_BATTERY_372V         2770
#define VOLDET_BATTERY_371V         2762
#define VOLDET_BATTERY_370V         2755
#define VOLDET_BATTERY_369V         2748
#define VOLDET_BATTERY_367V         2733
#define VOLDET_BATTERY_366V         2725
#define VOLDET_BATTERY_360V         2681
#define VOLDET_BATTERY_355V         2643
#define VOLDET_BATTERY_350V         2606
#define VOLDET_BATTERY_340V         2532
#define VOLDET_BATTERY_004V         30
#define CHARGE_ADC_OFFSET           100
#define LENS_ADC_OFFSET             95
#define LENS_ADC_OFFSET2            50

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


static INT32  gTempValue = 25;
#if ADC_TEST
UINT32 gAdcValue = 3000;
#endif

//------------------ Battery Status Level -------------------//
#define  BATT_LEVEL_COUNT  4

static UINT32  LiBattAdcLevelValue[BATT_LEVEL_COUNT]={
VOLDET_BATTERY_355V,
VOLDET_BATTERY_371V,
VOLDET_BATTERY_376V,
VOLDET_BATTERY_380V,
};



#define  DUMMY_LOAD_OFFSETV          VOLDET_BATTERY_004V
#define  LENS_MOVE_MAX_COUNT         10

static UINT32* pBattAdcLevelValue = &LiBattAdcLevelValue[0];;
static UINT32  uiBattADCSlideWin[BATT_SLIDE_WINDOW_COUNT]={0};
static UINT8   uiBattSlideIdx = 0;
static UINT8   uiCurSlideWinCnt = 0;
static INT32   iBattAdcCalOffset = 0;
#if USB_CHARGE_FUNCTION
static UINT32  u32BattChargeCurrent = BATTERY_CHARGE_CURRENT_LOW;
#endif
#if DUMMY_LOAD
static void    DrvPower_dummy_process(UINT32 *V1,UINT32 *V2, BOOL bIris);
#endif
static INT32   DrvPower_GetTempCompentation(INT32 tempValue);

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
    INT32  TempCompentationADC;

    #if ADC_TEST
    uiADCValue = gAdcValue;
    #else
    uiADCValue = (adc_readData(ADC_CH_VOLDET_BATTERY));
    #endif
    {
        TempCompentationADC = DrvPower_GetTempCompentation(gTempValue);
        DBG_MSG("Temp= %d\r\n",gTempValue);
        if (DrvFlash_IsCharge())
        {
            //DBG_MSG("ADC = %d,",uiADCValue);
            uiADCValue+= (CHARGE_ADC_OFFSET+TempCompentationADC);
            DBG_MSG("Charge ADC+= %d\r\n",CHARGE_ADC_OFFSET+TempCompentationADC);
        }
        else
        {
            if (g_fpDxPowerCB)
            {
                if(g_fpDxPowerCB(DRVPWR_CB_IS_LENS_MOVING,0, 0))
                {
                    DBG_MSG("ADC = %d,",uiADCValue);
                    uiADCValue+= (LENS_ADC_OFFSET+TempCompentationADC);
                    DBG_MSG("lens2 ADC+= %d\r\n",LENS_ADC_OFFSET+TempCompentationADC);

                }

            }
        }
        uiADCValue+=(iBattAdcCalOffset/2);
        return uiADCValue;
    }
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
            DBG_MSG("AVG=%d\r\n",uiCurBatteryADC);
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
        DBG_MSG("AVG=%d",uiCurBatteryADC);
        DBG_MSG(", V=%d",uiCurBatteryADC*42/9100);
        DBG_MSG(".%02d\r\n",(uiCurBatteryADC*42/91)%100);
    }

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
        if (uiEmptycount == 4)
        {
            return DRVPWR_BATTERY_LVL_EMPTY;
        }
    }

    return uiRetBatteryLvl;
}


void DrvPower_PowerOnInit(void)
{
    pBattAdcLevelValue = &LiBattAdcLevelValue[0];
    #if USB_CHARGE_FUNCTION
    gTempValue = DrvPower_BattTempGet();
    #endif
}

UINT32 DrvPower_DummyLoad(void)
{
#if DUMMY_LOAD
    UINT32 Ave_V, deltaV, V1,V2;

    DrvPower_dummy_process(&V1,&V2,FALSE);
    deltaV = V2-V1;
    Ave_V = V1;
    dummyLoadData[0].deltaV = deltaV;
    dummyLoadData[0].Ave_V = Ave_V;


    if (dummyLoadData[0].Ave_V < (LiBattAdcLevelValue[0]+DUMMY_LOAD_OFFSETV))
    {
        bDummyLoadPwrOff = TRUE;

    }
    else
    {
        bDummyLoadPwrOff = FALSE;
    }
    DBG_IND("Ave_V=%d ; deltaV=%d , bDummyLoadPwrOff=%d\r\n", Ave_V,deltaV,bDummyLoadPwrOff);
    return (!bDummyLoadPwrOff);
#else
    return TRUE;
#endif
}
#if DUMMY_LOAD
static void DrvPower_dummy_process(UINT32 *V1,UINT32 *V2, BOOL bIris)
{
    UINT32 i,Va;
    UINT32 Vtotal, DetectLoop = 25;
    float  Vtmp1,Vtmp2,Vtmp3;

    if (g_fpDxPowerCB)
    {
        gpio_clearPin(GPIO_LENS_MD_RST); // Enable G2007 motor driver.GPIO_LENS_MD_RST
        Delay_DelayMs(5);
        gpio_setPin(GPIO_LENS_MD_RST); // Enable G2007 motor driver.GPIO_LENS_MD_RST
        g_fpDxPowerCB(DRVPWR_CB_DUMMY_LOAD_START,0,0);
    }
    Delay_DelayMs(40);

    Vtotal = 0;
    for(i=0;i< DetectLoop ;i++)   //sample 25 times.
    {
      Va = DrvPower_GetBatteryADC();  //sample average value.(In loading)
      Delay_DelayUs(200);
      Vtotal += Va;
    }
    *V1 = Vtotal/DetectLoop;
    if (g_fpDxPowerCB)
    {
        g_fpDxPowerCB(DRVPWR_CB_DUMMY_LOAD_END,0,0);
    }
    Delay_DelayMs(5);
    Vtotal = 0;
    for(i=0;i< DetectLoop ;i++)   //sample 25 times.
    {
      Va = DrvPower_GetBatteryADC();  //sample average value.(after loading)
      Delay_DelayUs(200);
      Vtotal += Va;
    }

    *V2 = Vtotal/DetectLoop;


    Vtmp1 = (float)(*V1)*3.3*1.39/4096;
    Vtmp2 = (float)(*V2)*3.3*1.39/4096;


    Vtmp3 = Vtmp2 - Vtmp1;

    DBG_IND("delta=(%1.3f, %d) Ave_V:%1.3f V2:%1.3f, %d, %d, Cal = %d \r\n",Vtmp3,(*V2)-(*V1),Vtmp1,Vtmp2,*V1,*V2,iBattAdcCalOffset);

}
#endif

static void DrvPower_BattADC_Calibration(BOOL enable)
{
}

#if USB_CHARGE_FUNCTION
static void DrvPower_EnableChargeIC(BOOL bCharge)
{

}

static void DrvPower_ChargeBattEn(BOOL bCharge)
{
#if (CHARGE_FUNCTION == ENABLE)
    if(bCharge)
        PwrIC_DoCmd(cmd_Charge_start, 0, 0);
    else
        PwrIC_DoCmd(cmd_Charge_stop, 0, 0);
#endif
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
#if (CHARGE_FUNCTION == ENABLE)
    UINT32 r;
    PwrIC_DoCmd(cmd_Charge_geten, 0, &r);
    if((r & CHARGE_EN_OFF) != 0)
        return FALSE;
    PwrIC_DoCmd(cmd_Charge_getstat, 0, &r);
    if((r & CHARGE_STAT_MASK) != 0)
        return FALSE;
    return TRUE;
#endif
    return 0;
}

static BOOL DrvPower_IsUSBChargeOK(void)
{
#if (CHARGE_FUNCTION == ENABLE)
    UINT32 r;
    PwrIC_DoCmd(cmd_Charge_getstat, 0, &r);
    if((r & CHARGE_STAT_MASK) != CHARGE_STAT_FULL)
        return FALSE;
    return TRUE;
#endif
    return 0;
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

#endif
static INT32 DrvPower_GetTempCompentation(INT32 tempValue)
{
    return 0;
}

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

