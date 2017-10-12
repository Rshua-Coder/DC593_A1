#include "LensMotor_Common.h"
#include "LensMotor_G2007.h"
#include "Utility.h"
#include "GPIO.h"
#include "sif.h"
#include "top.h"
#include "ErrorNo.h"
#include "pwm.h"
#include "DxLens.h"
#include "Debug.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          LENSMOTORG2007
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define MD_DELAYMS(x)   Delay_DelayMs(x)
#define MD_DELAYUS(x)   Delay_DelayUs(x)

#define SET_GPIO            1
#define CLEAR_GPIO          0

#define ZOOM_PWM_ENABLE          1
#define MOTOR_PWM_THRESHOLD_EN   0
#define IRIS_USTEP_ENABLE        1

#define DCJ36_ZOOM_SPEED         64
#define MOTOR_PWM_PRIOD_CLK_DIV  18 // 120

static void     motorG2007_init(PLENS_DEVICE_OBJ pLensDev);
static void     motorG2007_setSignal(MD_SIGNAL signal, UINT32 on);
static void     motorG2007_SetMDReg(UINT32 value);
static BOOL     motorG2007_zoom_checkPI(void);
static void     motorG2007_zoom_setState(MOTOR_ZOOM_ACT state, UINT32 param1);
static void     motorG2007_zoom_setSpeed(UINT32 uiSpeed);
static UINT32   motorG2007_zoom_getSpeed(void);
static void     motorG2007_zoom_moveForward(void);
static void     motorG2007_zoom_moveReverse(void);
static void     motorG2007_zoom_moveBreak(void);
static void     motorG2007_zoom_moveOff(void);
static BOOL     motorG2007_focus_setState(MOTOR_FOCUS_ACT state, UINT32 param1);
static void     motorG2007_aperture_setState(MOTOR_APERTURE_ACT state, UINT32 param1);
static void     motorG2007_shutter_setState(MOTOR_SHUTTER_ACT state, UINT32 param1);
#if ZOOM_PWM_ENABLE
static UINT32   motorG2007_signal_enablePWM(UINT32 PWMIdx, UINT32 Div, UINT32 Prd, UINT32 OnCnt, UINT32 Inv);
#endif
static void     motorG2007_signal_setGPIO(BOOL set, UINT32 gpio);

static MOTOR_TAB gmotorG2007Obj =
{
    motorG2007_init,
    motorG2007_setSignal,
    motorG2007_zoom_setSpeed,
    motorG2007_zoom_getSpeed,
    motorG2007_zoom_setState,
    motorG2007_zoom_checkPI,
    motorG2007_focus_setState,
    motorG2007_aperture_setState,
    motorG2007_shutter_setState
};

static MOTOR_PVT_OBJ gG2007PvtObj =
{
    //GPIO related var
    {0}, //uiPinIN[MOTOR_MAX_INPUT_PIN];

    0,  //uiPinOutputZPI; //DSP output to MD
    0,  //uiPinOutputZPR; //DSP output to MD
    0,  //uiPinOutputFPI; //DSP output to MD
    //Other info
    0,  //uiHWPower;
    0,  //uiZoomStatus;
    100,  //uiZoomSpeed;
    1,  //uiFocusPhase;
    IRIS_INIT_PHASE,  //uiIrisPhase;
    1   // enable u-step default.
};

//static MD_STATUS   g_MDStatus = {0} ;

static MD_REGS Regs = {0x0000,       //A0
                       0x4000,       //A1
                       0x8000,       //A2
                       0xC000,       //A3
                      };

typedef struct _PWM_4CH_
{
    UINT32  uiCh_A1;
    UINT32  uiCh_A2;
    UINT32  uiCh_B1;
    UINT32  uiCh_B2;
}PWM_4CH;

// 4 represent 4 step
static PWM_4CH g_pwmChannel[TOTAL_FOCUS_PHASE] =
{
#if 0
    //AF_A1 AF_B1   AF_A2  AF_B2
    {   0,    4,      2,     6  }, //Step0
    {   2,    6,      4,     0  }, //Step1
    {   4,    0,      6,     2  }, //Step2
    {   6,    2,      0,     4  }, //Step3
#else
    //AF_A1 AF_B1   AF_A2  AF_B2
   #if 0
    {   1,    5,      3,     7  }, //Step0
    {   3,    7,      5,     1  }, //Step1
    {   5,    1,      7,     3  }, //Step2
    {   7,    3,      1,     5  }, //Step3
   #else
    {   1,    5,      3,     7  }, //Step0
    {   2,    6,      4,     0  },
    {   3,    7,      5,     1  }, //Step1
    {   4,    0,      6,     2  },
    {   5,    1,      7,     3  }, //Step2
    {   6,    2,      0,     4  },
    {   7,    3,      1,     5  }, //Step3
    {   0,    4,      2,     6  },

   #endif
#endif
};

static void motorG2007_initSIF(void)
{
#if 0
    SIF_CTRL    SIFCtrl;

    pinmux_enable_sif(MOTOR_G2007_PINMUX_SIF_CH);
    sif_open();

    SIFCtrl.uiMode          = SIF_MODE_0010;
    SIFCtrl.bSyncEn         = 0;
    SIFCtrl.uiSyncSrc       = SIF_SYNCSRC_NONE;
    SIFCtrl.uiSenDuration   = 2;
    SIFCtrl.uiLength        = 16;
    SIFCtrl.uiClkDiv        = 5; //48/2/(Div+1)
    SIFCtrl.bMSB            = TRUE;
    sif_setControl(MOTOR_G2007_SIF_CH, &SIFCtrl);
#else
    //#NT#2012/07/19#Isiah Chang -begin
    //#NT#Must confirm all settings with HW in the near future.
    sif_open(MOTOR_G2007_SIF_CH);

    #if 0
    pinmux_enable_sif(MOTOR_G2007_PINMUX_SIF_CH);
    #endif

    sif_setConfig(MOTOR_G2007_SIF_CH, SIF_CONFIG_ID_BUSMODE,  SIF_MODE_02);
    // Bus clock = 4.8 MHz
    sif_setConfig(MOTOR_G2007_SIF_CH, SIF_CONFIG_ID_BUSCLOCK, 4800000);
    // 16 half bus clock = 2666.66 ns
    sif_setConfig(MOTOR_G2007_SIF_CH, SIF_CONFIG_ID_SEND,     2667);
    // 0 half bus clock = 0 ns
    sif_setConfig(MOTOR_G2007_SIF_CH, SIF_CONFIG_ID_SENS,     300);
    // 0 half bus clock = 0 ns
    sif_setConfig(MOTOR_G2007_SIF_CH, SIF_CONFIG_ID_SENH,     200);
    // 16 bits per transmission
    sif_setConfig(MOTOR_G2007_SIF_CH, SIF_CONFIG_ID_LENGTH,   16);
    // MSb shift out first
    sif_setConfig(MOTOR_G2007_SIF_CH, SIF_CONFIG_ID_DIR,      SIF_DIR_MSB);
    // Delay between successive transmission
    sif_setConfig(MOTOR_G2007_SIF_CH, SIF_CONFIG_ID_DELAY,    300);
    //#NT#2012/07/19#Isiah Chang -end
#endif
}

static void motorG2007_initGPIO(PLENS_DEVICE_OBJ pLensDev)
{
    UINT32 i, ui8InPinIdx = LENS_IO_IN_0;

    for(i = 0; i < MOTOR_MAX_INPUT_PIN; i++)
    {
        gG2007PvtObj.uiPinIN[i]   =  pLensDev->pGPIOArray[ui8InPinIdx + i];
    }

    gG2007PvtObj.uiPinOutputZPI   =  pLensDev->pGPIOArray[LENS_IO_ZOOM_PI];
    gG2007PvtObj.uiPinOutputZPR   =  pLensDev->pGPIOArray[LENS_IO_ZOOM_PR];
    gG2007PvtObj.uiPinOutputFPI   =  pLensDev->pGPIOArray[LENS_IO_FOCUS_PI];

}

/**
    motor init

    Init FF pin assignment

    @param None.

    @return None.
*/
static void motorG2007_init(PLENS_DEVICE_OBJ pLensDev)
{
    motorG2007_initGPIO(pLensDev);
    motorG2007_initSIF();
    gG2007PvtObj.uiZoomStatus = DCM_OFF;
    gG2007PvtObj.uiIrisPhase = IRIS_INIT_PHASE; // Reset iris phase(refer to spec. 7.5 IRIS STEP CONTROL TABLE).

    return;
}

/**
    Get G2007 motor object

    @return G2007 motor object pointer

*/
PMOTOR_TAB motor_getObject(void)
{
    DBG_MSG("%s\r\n", __func__);

    return &gmotorG2007Obj;
}

static void motorG2007_setSignal(MD_SIGNAL signal, UINT32 on)
{
    switch(signal)
    {
    case MD_SIGNAL_STANDBY:
        DBG_MSG("motorG2007_setSignal MD_SIGNAL_STANDBY %d\r\n",on);
        Regs.A3.Bit.PS = on;
        motorG2007_SetMDReg(Regs.A3.Reg);
    break;

    case MD_SIGNAL_ZPI:
        DBG_MSG("motorG2007_setSignal MD_SIGNAL_ZPI %d\r\n",on);
        Regs.A3.Bit.PS = 1;
        motorG2007_SetMDReg(Regs.A3.Reg);

        Regs.A2.Bit.PI2_Control = on;
        motorG2007_SetMDReg(Regs.A2.Reg);
    break;

    case MD_SIGNAL_ZPR:
        DBG_MSG("motorG2007_setSignal MD_SIGNAL_ZPR %d\r\n",on);
        Regs.A3.Bit.PS = 1;
        motorG2007_SetMDReg(Regs.A3.Reg);

        Regs.A2.Bit.PI1_Control = on;
        motorG2007_SetMDReg(Regs.A2.Reg);
    break;

    case MD_SIGNAL_FPI:
        DBG_MSG("motorG2007_setSignal MD_SIGNAL_FPI %d\r\n",on);
        Regs.A3.Bit.PS = 1;
        motorG2007_SetMDReg(Regs.A3.Reg);

        Regs.A2.Bit.PI3_Control = on;
        motorG2007_SetMDReg(Regs.A2.Reg);
    break;

    default:
        break;

    }
}

static void motorG2007_SetMDReg(UINT32 value)
{
    DBG_MSG("SIF%d cmd = 0x%.4X\r\n", MOTOR_G2007_SIF_CH, value);
    value = (value << 16);

    sif_send(MOTOR_G2007_SIF_CH, value, 0, 0);
    sif_waitTransferEnd(MOTOR_G2007_SIF_CH);
}

static BOOL motorG2007_zoom_checkPI(void)
{
    return FALSE;
}

static void motorG2007_Zoom_ConfigReg(DCM_STATUS status)
{
    switch(status)
    {
    case DCM_FORWARD:
    case DCM_REVERSE:
        Regs.A3.Bit.PS = 1;                     //Power on
        motorG2007_SetMDReg(Regs.A3.Reg);

        Regs.A0.Bit.CH5_Mode_Setup = 5;         //Parallel
        motorG2007_SetMDReg(Regs.A0.Reg);

        Regs.A1.Bit.CH56_Disable = 1;           //Enable CH5/6
        motorG2007_SetMDReg(Regs.A1.Reg);

        #if 1
        Regs.A2.Bit.CH5_DAC = 2;                // 4.56V
        #else // Try to slow down zoom speed.
        Regs.A2.Bit.CH5_DAC = 0xF;                // 3V
        #endif
        Regs.A2.Bit.CH5_CC_CV_FS = 2;           // 0/1:CC, 2: CV, 3: FS
        motorG2007_SetMDReg(Regs.A2.Reg);
    break;
    default:
    case DCM_BRAKE:
    case DCM_OFF:
        Regs.A1.Bit.CH56_Disable = 0;       //Disable
        motorG2007_SetMDReg(Regs.A1.Reg);

    break;
    }

}

static void motorG2007_zoom_setState(MOTOR_ZOOM_ACT state, UINT32 param1)
{
    DBG_MSG("%s, state=%d, param1=%d\n\r", __func__, state, param1);
    switch(state)
    {
        case MOTOR_ZOOM_BRAKE:
            motorG2007_zoom_moveBreak();
            break;
        case MOTOR_ZOOM_FWD:
            motorG2007_zoom_moveForward();
            break;
        case MOTOR_ZOOM_BWD:
            motorG2007_zoom_moveReverse();
            break;
        case MOTOR_ZOOM_OFF:
            motorG2007_zoom_moveOff();
            break;
        default:
            DBG_ERR("%s,UNKNOW\n\r", __func__);
            break;
    }

}
static void motorG2007_zoom_setSpeed(UINT32 uiSpeed)
{
    if(uiSpeed>SPEED_FULL)
        uiSpeed = SPEED_FULL;

    gG2007PvtObj.uiZoomSpeed = uiSpeed ;

#if 0 // Shouldn't move zoom in this function.
    if(gG2007PvtObj.uiZoomStatus == DCM_FORWARD)
    {
        motorG2007_zoom_moveForward();
    }
    else if(gG2007PvtObj.uiZoomStatus == DCM_REVERSE)
    {
        motorG2007_zoom_moveReverse();
    }
#endif

}
static UINT32 motorG2007_zoom_getSpeed(void)
{
    return gG2007PvtObj.uiZoomSpeed;
}

static void motorG2007_zoom_moveForward(void)
{

    if(gG2007PvtObj.uiZoomStatus == DCM_REVERSE)
    {
        DBG_ERR("%s, status error (%d)\n\r", __func__,gG2007PvtObj.uiZoomStatus);
        return;
    }

    DBG_MSG("motorG2007_zoom_moveForward\r\n");
    motorG2007_Zoom_ConfigReg(DCM_FORWARD);

#if !ZOOM_PWM_ENABLE
    motorG2007_signal_setGPIO(CLEAR_GPIO, gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_ZOOM_P]);
    motorG2007_signal_setGPIO(SET_GPIO, gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_ZOOM_N]);

#else
    //DBG_MSG("^RFZS:%d\r\n", gG2007PvtObj.uiZoomSpeed);
    motorG2007_signal_enablePWM(MOTOR_G2007_PWMID_ZOOM_PIN1, MOTOR_PWM_PRIOD_CLK_DIV, SPEED_FULL, gG2007PvtObj.uiZoomSpeed, 0);
    motorG2007_signal_enablePWM(MOTOR_G2007_PWMID_ZOOM_PIN0, MOTOR_PWM_PRIOD_CLK_DIV, SPEED_FULL, 0, 0);  // always low.
#endif

    gG2007PvtObj.uiZoomStatus = DCM_FORWARD;
}

static void motorG2007_zoom_moveReverse(void)
{

    if(gG2007PvtObj.uiZoomStatus == DCM_FORWARD)
    {
        DBG_ERR("%s, status error (%d)\n\r", __func__,gG2007PvtObj.uiZoomStatus);
        return;
    }

    DBG_MSG("motorG2007_zoom_moveReverse\r\n");
    motorG2007_Zoom_ConfigReg(DCM_REVERSE);

#if !ZOOM_PWM_ENABLE
    motorG2007_signal_setGPIO(SET_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_ZOOM_P]);
    motorG2007_signal_setGPIO(CLEAR_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_ZOOM_N]);
#else
    //DBG_MSG("^RRZS:%d\r\n", gG2007PvtObj.uiZoomSpeed);
    motorG2007_signal_enablePWM(MOTOR_G2007_PWMID_ZOOM_PIN0, MOTOR_PWM_PRIOD_CLK_DIV, SPEED_FULL, gG2007PvtObj.uiZoomSpeed, 0);
    motorG2007_signal_enablePWM(MOTOR_G2007_PWMID_ZOOM_PIN1, MOTOR_PWM_PRIOD_CLK_DIV, SPEED_FULL, 0, 0);  // always low.
#endif

    gG2007PvtObj.uiZoomStatus = DCM_REVERSE;
}

static void motorG2007_zoom_moveBreak(void)
{
    if(gG2007PvtObj.uiZoomStatus == DCM_BRAKE)
    {
        //DBG_ERR("%s, status error (%d)\n\r", __func__,gG2007PvtObj.uiZoomStatus);
        return;
    }

    DBG_MSG("motorG2007_zoom_moveBreak\r\n");
    motorG2007_Zoom_ConfigReg(DCM_BRAKE);

#if ZOOM_PWM_ENABLE
    motorG2007_signal_enablePWM(MOTOR_G2007_PWMID_ZOOM_PIN0, MOTOR_PWM_PRIOD_CLK_DIV, SPEED_FULL, SPEED_FULL, 0);  // always high.
    motorG2007_signal_enablePWM(MOTOR_G2007_PWMID_ZOOM_PIN1, MOTOR_PWM_PRIOD_CLK_DIV, SPEED_FULL, SPEED_FULL/*gG2007PvtObj.uiZoomSpeed*/, 0); // always high.
#else
    motorG2007_signal_setGPIO(SET_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_ZOOM_P]);
    motorG2007_signal_setGPIO(SET_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_ZOOM_N]);
#endif

    gG2007PvtObj.uiZoomStatus = DCM_BRAKE;
}

static void motorG2007_zoom_moveOff(void)
{
    if(gG2007PvtObj.uiZoomStatus == DCM_OFF)
    {
        DBG_ERR("%s, status error (%d)\n\r", __func__,gG2007PvtObj.uiZoomStatus);
        return;
    }

    DBG_MSG("motorG2007_zoom_moveOff\r\n");
    motorG2007_Zoom_ConfigReg(DCM_OFF);

#if ZOOM_PWM_ENABLE
    pwm_close(MOTOR_G2007_PWMID_ZOOM_PIN0, TRUE);
    pwm_close(MOTOR_G2007_PWMID_ZOOM_PIN1, TRUE);
#else
    motorG2007_signal_setGPIO(CLEAR_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_ZOOM_P]);
    motorG2007_signal_setGPIO(CLEAR_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_ZOOM_N]);
#endif

    gG2007PvtObj.uiZoomStatus = DCM_OFF;
}

static void motorG2007_focus_movePhase(UINT32 phase)
{
    UINT32 index, reg=0xFFFFFFFF;
    UINT8 IN1,IN2,IN3,IN4;
    DBG_MSG("%s, phase=%d \n\r", __func__, phase);

    index = gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_AF_AP]/(1<<5) ;
    reg = gpio_readData(index);

    IN1 = gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_AF_AP] - P_GPIO_SHIFT_BASE ;
    IN1 &= (32-1);
    IN2 = gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_AF_AN] - P_GPIO_SHIFT_BASE ;
    IN2 &= (32-1);
    IN3 = gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_AF_BP] - P_GPIO_SHIFT_BASE ;
    IN3 &= (32-1);
    IN4 = gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_AF_BN] - P_GPIO_SHIFT_BASE ;
    IN4 &= (32-1);

    switch(phase)
    {
    case 1:
        reg |= (1<<IN1) ;
        reg &= ~(1<<IN2) ;
        reg |= (1<<IN3) ;
        reg &= ~(1<<IN4) ;
    break;
    case 2:
        reg |= (1<<IN1) ;
        reg &= ~(1<<IN2) ;
        reg &= ~(1<<IN3) ;
        reg &= ~(1<<IN4) ;
    break;
    case 3:
        reg |= (1<<IN1) ;
        reg &= ~(1<<IN2) ;
        reg &= ~(1<<IN3) ;
        reg |= (1<<IN4) ;
    break;
    case 4:
        reg &= ~(1<<IN1) ;
        reg &= ~(1<<IN2) ;
        reg &= ~(1<<IN3) ;
        reg |= (1<<IN4) ;
    break;
    case 5:
        reg &= ~(1<<IN1) ;
        reg |= (1<<IN2) ;
        reg &= ~(1<<IN3) ;
        reg |= (1<<IN4) ;
    break;
    case 6:
        reg &= ~(1<<IN1) ;
        reg |= (1<<IN2) ;
        reg &= ~(1<<IN3) ;
        reg &= ~(1<<IN4) ;
    break;
    case 7:
        reg &= ~(1<<IN1) ;
        reg |= (1<<IN2) ;
        reg |= (1<<IN3) ;
        reg &= ~(1<<IN4) ;
    break;
    case 8:
        reg &= ~(1<<IN1) ;
        reg &= ~(1<<IN2) ;
        reg |= (1<<IN3) ;
        reg &= ~(1<<IN4) ;
    break;

    case 0xFFFFFFFF:
        reg &= ~(1<<IN1) ;
        reg &= ~(1<<IN2) ;
        reg &= ~(1<<IN3) ;
        reg &= ~(1<<IN4) ;
    break;
    default:
        DBG_ERR("Focus Phase ERR (%d)\r\n",phase);
    break;
    }
    gpio_writeData(index, reg);

}

#define FOCUS_BOOT_STEP_COUNT    (4 << 1)
#define FOCUS_INIT_STEP_COUNT    (1 << 1)

/*
static UINT32 motor_G2007_focus_get_cur_phase(void)
{
    return gG2007PvtObj.uiFocusPhase;
}

static void motor_G2007_focus_set_cur_phase(UINT32 uiCurPhase)
{
    gG2007PvtObj.uiFocusPhase = uiCurPhase;
}
*/

static BOOL motorG2007_focus_setState(MOTOR_FOCUS_ACT state, UINT32 param1)
{
    static BOOL bBootStepOn = TRUE;
    static BOOL bInitStepOn = TRUE;
    static BOOL bFastStepOn = FALSE;
    static UINT8 byBootStepCount = 0;
    static UINT8 byInitStepCount = 0;
    MS_CH_PHASE_CFG msSetChPhCfg;
    MSCOMMON_CFG    msCommonCfg;

    switch(state)
    {
    case MOTOR_FOCUS_FWD:
        if(gG2007PvtObj.bMicroStepEn)
        {
            msCommonCfg.uiDir = 1;
            msCommonCfg.uiOnCycle = 1; //uiStepCnt;

            msCommonCfg.uiStepPerPhase = ((TOTAL_08_STEP_PER_PHASE) << 3/*uiStepPerPhase*/);
            msCommonCfg.uiPhaseType = PWM_MS_1_2_PHASE_TYPE;
            #if MOTOR_PWM_THRESHOLD_EN
            msCommonCfg.uiThresholdEn = 1;//0;
            #else
            msCommonCfg.uiThresholdEn = 0;//0;
            #endif
            msCommonCfg.uiThreshold = 0;

            msSetChPhCfg.uiCH0_Phase = g_pwmChannel[gG2007PvtObj.uiFocusPhase-1].uiCh_A1;
            msSetChPhCfg.uiCH1_Phase = g_pwmChannel[gG2007PvtObj.uiFocusPhase-1].uiCh_A2;
            msSetChPhCfg.uiCH2_Phase = g_pwmChannel[gG2007PvtObj.uiFocusPhase-1].uiCh_B1;
            msSetChPhCfg.uiCH3_Phase = g_pwmChannel[gG2007PvtObj.uiFocusPhase-1].uiCh_B2;
        }

        if(gG2007PvtObj.uiFocusPhase == 1)
            gG2007PvtObj.uiFocusPhase = TOTAL_FOCUS_PHASE;
        else
            gG2007PvtObj.uiFocusPhase--;

        if(gG2007PvtObj.bMicroStepEn)
        {
            if(bInitStepOn || bBootStepOn)
            {
                if(pwm_mstepConfigSet(PWM_MS_SET_1, &msSetChPhCfg, &msCommonCfg) != E_OK)
                {
                    DBG_MSG("MS set config error\r\n");
                    break;
                }
                //gpio_setPin(D_GPIO_1);
                pwm_mstepEnableSet(PWM_MS_SET_1); // Trigger first warm up step.
                pwm_waitSet(PWM_MS_SET_1);
                Delay_DelayMs(1); /* Delay to let slow step workable. */
                //gpio_clearPin(D_GPIO_1);


                if(bBootStepOn)
                {
                    //DBG_ERR("#FFI:%d\r\n", byBootStepCount);
                    if (++byBootStepCount >= FOCUS_BOOT_STEP_COUNT)
                    {
                        bBootStepOn = FALSE;
                        bFastStepOn = TRUE;
                    }
                }
                else //(bInitStepOn)
                {
                    //DBG_ERR("#FFI:%d\r\n", byInitStepCount);
                    if (++byInitStepCount >= FOCUS_INIT_STEP_COUNT)
                    {
                        bInitStepOn = FALSE;
                        bFastStepOn = TRUE;
                    }
                }
            }
            else
            {
                if(bFastStepOn) // Change PWM clock speed.
                {
                    #if 1
                    //gpio_setPin(D_GPIO_1);
                    pwm_setStop(PWM_MS_SET_1);
                    //gpio_clearPin(D_GPIO_1);

                    pwm_msSetClockDiv(PWM_MS_SET_1, 9);
                    #else // Slow down if lost step.
                    //pwm_msSetClockDiv(PWM_MS_SET_1, 10);
                    #endif

                    bFastStepOn = FALSE;
                }

                if(pwm_msSetConfig(PWM_MS_SET_1, &msSetChPhCfg, &msCommonCfg) != E_OK)
                {
                    DBG_MSG("MS set config error\r\n");
                    break;
                }
                pwm_pwmEnableSet(PWM_MS_SET_1);
                pwm_mstepEnableSet(PWM_MS_SET_1);
                pwm_waitSet(PWM_MS_SET_1);
            }
        }
        else
        {
            motorG2007_focus_movePhase(gG2007PvtObj.uiFocusPhase);

            if(bInitStepOn || bBootStepOn)
            {
                MD_DELAYUS(2500); // Boot/Init step.
                if(bBootStepOn)
                {
                    //DBG_ERR("#FFI:%d\r\n", byBootStepCount);
                    if (++byBootStepCount >= FOCUS_BOOT_STEP_COUNT)
                        bBootStepOn = FALSE;
                }
                else //(bInitStepOn)
                {
                    //DBG_ERR("#FFI:%d\r\n", byInitStepCount);
                    if (++byInitStepCount >= FOCUS_INIT_STEP_COUNT)
                        bInitStepOn = FALSE;
                }
            }
           else
                MD_DELAYMS(1); // Consecutive step.
        }

        break;
    case MOTOR_FOCUS_BWD:
        if(gG2007PvtObj.bMicroStepEn)
        {
            msCommonCfg.uiDir = 0;
            msCommonCfg.uiOnCycle = 1; //uiStepCnt;

            msCommonCfg.uiStepPerPhase = ((TOTAL_08_STEP_PER_PHASE) << 3/*uiStepPerPhase*/);
            msCommonCfg.uiPhaseType = PWM_MS_1_2_PHASE_TYPE;
            #if MOTOR_PWM_THRESHOLD_EN
            msCommonCfg.uiThresholdEn = 1;//0;
            #else
            msCommonCfg.uiThresholdEn = 0;//0;
            #endif
            msCommonCfg.uiThreshold = 0;

            msSetChPhCfg.uiCH0_Phase = g_pwmChannel[gG2007PvtObj.uiFocusPhase-1].uiCh_A1;
            msSetChPhCfg.uiCH1_Phase = g_pwmChannel[gG2007PvtObj.uiFocusPhase-1].uiCh_A2;
            msSetChPhCfg.uiCH2_Phase = g_pwmChannel[gG2007PvtObj.uiFocusPhase-1].uiCh_B1;
            msSetChPhCfg.uiCH3_Phase = g_pwmChannel[gG2007PvtObj.uiFocusPhase-1].uiCh_B2;
        }

        if(gG2007PvtObj.uiFocusPhase >= (TOTAL_FOCUS_PHASE))
            gG2007PvtObj.uiFocusPhase = 1;
        else
            gG2007PvtObj.uiFocusPhase++;

        if(gG2007PvtObj.bMicroStepEn)
        {
            if(bInitStepOn || bBootStepOn) // Change PWM clock speed.
            {
                if(pwm_mstepConfigSet(PWM_MS_SET_1, &msSetChPhCfg, &msCommonCfg) != E_OK)
                {
                    DBG_MSG("MS set config error\r\n");
                    break;
                }

                pwm_mstepEnableSet(PWM_MS_SET_1); // Trigger first warm up step.
                pwm_waitSet(PWM_MS_SET_1);
                Delay_DelayMs(1);

                if(bBootStepOn)
                {
                    //DBG_ERR("#FFI:%d\r\n", byBootStepCount);
                    if (++byBootStepCount >= FOCUS_BOOT_STEP_COUNT)
                    {
                        bBootStepOn = FALSE;
                        bFastStepOn = TRUE;
                    }
                }
                else //(bInitStepOn)
                {
                    //DBG_ERR("#FFI:%d\r\n", byInitStepCount);
                    if (++byInitStepCount >= FOCUS_INIT_STEP_COUNT)
                    {
                        bInitStepOn = FALSE;
                        bFastStepOn = TRUE;
                    }
                }
            }
            else
            {
                if(bFastStepOn)
                {
                    #if 1
                    pwm_setStop(PWM_MS_SET_1);
                    pwm_msSetClockDiv(PWM_MS_SET_1, 9);
                    #else // Slow down if lost step.
                    //pwm_msSetClockDiv(PWM_MS_SET_1, 10);
                    #endif

                    bFastStepOn = FALSE;
                }

                if(pwm_msSetConfig(PWM_MS_SET_1, &msSetChPhCfg, &msCommonCfg) != E_OK)
                {
                    DBG_MSG("MS set config error\r\n");
                    break;
                }
                pwm_pwmEnableSet(PWM_MS_SET_1);
                pwm_mstepEnableSet(PWM_MS_SET_1);
                pwm_waitSet(PWM_MS_SET_1);
            }
        }
        else
        {
        motorG2007_focus_movePhase(gG2007PvtObj.uiFocusPhase);

        if(bInitStepOn || bBootStepOn)
        {
            MD_DELAYUS(2500); // Boot/Init step.
            if(bBootStepOn)
            {
                //DBG_ERR("#FBI:%d\r\n", byBootStepCount);
                if(++byBootStepCount >= FOCUS_BOOT_STEP_COUNT)
                    bBootStepOn = FALSE;
            }
            else //(bInitStepOn)
            {
                //DBG_ERR("*FBI:%d\r\n", byInitStepCount);
                if (++byInitStepCount >= FOCUS_INIT_STEP_COUNT)
                    bInitStepOn = FALSE;
            }
        }
        else
            MD_DELAYMS(1); // Consecutive step.
        }
        break;
    case MOTOR_FOCUS_EXCITE_ON:
        //gpio_setDir(D_GPIO_1, GPIO_DIR_OUTPUT);
        //gpio_clearPin(D_GPIO_1);

        Regs.A3.Bit.PS = 1;                     //Power on
        motorG2007_SetMDReg(Regs.A3.Reg);

        Regs.A0.Bit.CH12_Mode_Setup = 5;        //Parallel mode
        Regs.A0.Bit.CH1234_Brake_Sel = 1;       // LL brake
        motorG2007_SetMDReg(Regs.A0.Reg);

        Regs.A1.Bit.CH12_DAC = 2;               // 4.56V
        Regs.A1.Bit.CH12_CV_FS = 0;             // 0: CV, 1: FS
        Regs.A1.Bit.CH12_Disable = 1;           //Enable CH12
        motorG2007_SetMDReg(Regs.A1.Reg);

        /* u-step control */
        if(gG2007PvtObj.bMicroStepEn)
        {
            msCommonCfg.uiDir = param1;

            #if 0
            if (uiStepCnt <= 4)
                msCommonCfg.uiOnCycle = uiStepCnt;
            else
                msCommonCfg.uiOnCycle = 4; //uiStepCnt;
            #else
                msCommonCfg.uiOnCycle = 1; //uiStepCnt;
            #endif

            msCommonCfg.uiStepPerPhase = ((TOTAL_08_STEP_PER_PHASE) << 3/*uiStepPerPhase*/);
            msCommonCfg.uiPhaseType = PWM_MS_1_2_PHASE_TYPE;
            #if MOTOR_PWM_THRESHOLD_EN
            msCommonCfg.uiThresholdEn = 1;//0;
            #else
            msCommonCfg.uiThresholdEn = 0;//0;
            #endif
            msCommonCfg.uiThreshold = 0;

            msSetChPhCfg.uiCH0_Phase = g_pwmChannel[gG2007PvtObj.uiFocusPhase-1].uiCh_A1;
            msSetChPhCfg.uiCH1_Phase = g_pwmChannel[gG2007PvtObj.uiFocusPhase-1].uiCh_A2;
            msSetChPhCfg.uiCH2_Phase = g_pwmChannel[gG2007PvtObj.uiFocusPhase-1].uiCh_B1;
            msSetChPhCfg.uiCH3_Phase = g_pwmChannel[gG2007PvtObj.uiFocusPhase-1].uiCh_B2;

            #if 1
            pwm_msSetClockDiv(PWM_MS_SET_1, 23);
            #else // Slow down if lost step.
            pwm_msSetClockDiv(PWM_MS_SET_1, 25);
            #endif

            if(pwm_openSet(PWM_MS_SET_1) != E_OK)
            {
                DBG_MSG("MS set open error\r\n");
                break;
            }

            if(pwm_mstepConfigSet(PWM_MS_SET_1, &msSetChPhCfg, &msCommonCfg) != E_OK)
            {
                DBG_MSG("MS set config error\r\n");
                break;
            }

            pwm_pwmEnableSet(PWM_MS_SET_1); // Trigger pre-excite phase.
            MD_DELAYMS(5);
        }
        else
        {
            motorG2007_focus_movePhase(gG2007PvtObj.uiFocusPhase);
            MD_DELAYMS(5);
        }

        bInitStepOn = TRUE;
        byInitStepCount = 0;
        break;
    case MOTOR_FOCUS_EXCITE_OFF:
        if(gG2007PvtObj.bMicroStepEn)
        {
            MD_DELAYMS(5);
            pwm_closeSet(PWM_MS_SET_1, TRUE);
        }
        else
        {
            motorG2007_focus_movePhase(gG2007PvtObj.uiFocusPhase);
            MD_DELAYMS(5);
            motorG2007_focus_movePhase(0xFFFFFFFF); // Pull all pins low to save power.

            Regs.A1.Bit.CH12_Disable = 0;           //Disable CH12
            motorG2007_SetMDReg(Regs.A1.Reg);
        }
        bInitStepOn = TRUE;
        byInitStepCount = 0;
        break;
    case MOTOR_FOCUS_MICRO_STEP_EN: // enable/disable u-step.
        gG2007PvtObj.bMicroStepEn = param1;
        break;
    default:
        DBG_ERR("%s, parameter error (%d)\n\r", __func__,state);
        break;
    }

    return TRUE;

}

#if !IRIS_USTEP_ENABLE
static void motorG2007_aperture_movePhase(UINT32 phase)
{
    UINT32 index, reg=0xFFFFFFFF;
    UINT8 IN1,IN2,IN3,IN4;
    DBG_MSG("%s, phase=%d \n\r", __func__, phase);

    index = gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRIS_AP]/(1<<5) ;
    reg = gpio_readData(index);

    IN1 = gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRIS_AP] - P_GPIO_SHIFT_BASE ;
    IN1 &= (32-1);
    IN2 = gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRIS_AN] - P_GPIO_SHIFT_BASE ;
    IN2 &= (32-1);
    IN3 = gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRIS_BP] - P_GPIO_SHIFT_BASE ;
    IN3 &= (32-1);
    IN4 = gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRIS_BN] - P_GPIO_SHIFT_BASE ;
    IN4 &= (32-1);

    switch(phase)
    {
    case 1:
        reg |= (1<<IN1) ;
        reg &= ~(1<<IN3) ;
        reg &= ~(1<<IN2) ;
        reg &= ~(1<<IN4) ;
    break;
    case 2:
        reg |= (1<<IN1) ;
        reg &= ~(1<<IN3) ;
        reg &= ~(1<<IN2) ;
        reg |= (1<<IN4) ;
    break;
    case 3:
        reg &= ~(1<<IN1) ;
        reg &= ~(1<<IN3) ;
        reg &= ~(1<<IN2) ;
        reg |= (1<<IN4) ;
    break;
    case 4:
        reg &= ~(1<<IN1) ;
        reg &= ~(1<<IN3) ;
        reg |= (1<<IN2) ;
        reg |= (1<<IN4) ;
    break;
    case 5:
        reg &= ~(1<<IN1) ;
        reg &= ~(1<<IN3) ;
        reg |= (1<<IN2) ;
        reg &= ~(1<<IN4) ;
    break;
    case 6:
        reg &= ~(1<<IN1) ;
        reg |= (1<<IN3) ;
        reg |= (1<<IN2) ;
        reg &= ~(1<<IN4) ;
    break;
    case 7:
        reg &= ~(1<<IN1) ;
        reg |= (1<<IN3) ;
        reg &= ~(1<<IN2) ;
        reg &= ~(1<<IN4) ;
    break;
    case 8:
        reg |= (1<<IN1) ;
        reg |= (1<<IN3) ;
        reg &= ~(1<<IN2) ;
        reg &= ~(1<<IN4) ;
    break;

    case 0xFFFFFFFF:  // Pull all pins low to save power.
        reg &= ~(1<<IN1) ;
        reg &= ~(1<<IN3) ;
        reg &= ~(1<<IN2) ;
        reg &= ~(1<<IN4) ;
    break;
    default:
        DBG_ERR("Aperture Phase ERR (%d)\r\n",phase);
    break;
    }
    gpio_writeData(index, reg);

}
#endif

static void motorG2007_aperture_setNormalState(UINT32 phase)
{
    if(phase>TOTAL_IRIS_PHASE)
    {
        DBG_ERR("%s, Iris phase error (%d)\n\r", __func__,phase);
        return;
    }
    DBG_MSG("motorG2007_aperture_setNormalState %d\r\n",phase);

#if 0
    motorG2007_setSignal(MD_SIGNAL_STANDBY,ON);  //Power on
//#NT#2011/02/24#Jeffery Chuang -begin
//This mode will not effect Iris switch
    Regs.A0.Bit.CH34_Mode_Setup = 5;          //parallel mode
//    Regs.A0.Bit.CH123_XOR_Control = 0;      // 0: normal mode, 11: CH13/23 exclusive control mode
    motorG2007_SetMDReg(Regs.A0.Reg);
//#NT#2011/02/24#Jeffery Chuang -end

//    Regs.A0.Bit.CH3_Mode_Setup = 4;           //Serial
//    motorG2007_SetMDReg(Regs.A0.Reg);

    Regs.A1.Bit.CH34_Disable = 1;             //Enable CH3/4
//    Regs.A1.Bit.CH4_Disable = 0;            //Disable CH4
    Regs.A1.Bit.CH34_DAC = 5;                 // 3.72V
    Regs.A1.Bit.CH34_CV_FS = 0;               // 0: CV, 1: FS
    motorG2007_SetMDReg(Regs.A1.Reg);

//    Regs.A3.Bit.CH12_Disable = 0;           //Disable CH12
    Regs.A3.Bit.PS = 1;                       //Power on
    switch (phase)
    {
    case IRIS_POS_BIG:
        Regs.A3.Bit.IN3A = 1;              //Big iris
        Regs.A3.Bit.IN3B = 0;
        break;

    case IRIS_POS_SMALL:
        Regs.A3.Bit.IN3A = 0;              //Small iris
        Regs.A3.Bit.IN3B = 1;
        break;

    case IRIS_POS_CLOSE:
        Regs.A3.Bit.IN3A = 0;              //High Z status
        Regs.A3.Bit.IN3B = 0;
        break;
    }

    motorG2007_SetMDReg(Regs.A3.Reg);

    //#NT#2010/12/03#Lincy Lin -begin
    //MD_DELAYMS(10);
    MD_DELAYMS(15);
    //#NT#2010/12/03#Lincy Lin -end
    Regs.A3.Bit.IN3A = 1;                  //Brake
    Regs.A3.Bit.IN3B = 1;
    //Regs.A3.Bit.CH12_Disable = 0;          //Disable CH12
    motorG2007_SetMDReg(Regs.A3.Reg);
#else

#endif

}

static void motorG2007_aperture_dummyLoadState(UINT32 phase)
{
#if 0
    DBG_MSG("motorG2007_aperture_dummyLoadState %d\r\n",phase);
    motorG2007_setSignal(MD_SIGNAL_STANDBY,ON);  //Power on

    Regs.A0.Bit.CH3_Mode_Setup = 4;         //Serial
    motorG2007_SetMDReg(Regs.A0.Reg);

    Regs.A1.Bit.CH35_Disable = 1;           //Enable CH3/5
    // dummy load just set 4.2V
    Regs.A1.Bit.CH35_DAC = 3;               // 4.2V
    Regs.A1.Bit.CH35_XOR_Control = 1;       //Exclusive control mode
    motorG2007_SetMDReg(Regs.A1.Reg);
    Regs.A3.Bit.PS = 1;                     //Power on
    motorG2007_SetMDReg(Regs.A3.Reg);

    //Regs.A1.Bit.CH4_Disable = 1;            //Enable CH4
    //Regs.A2.Bit.CH4_DAC = 8;                // 220mV
    //Regs.A0.Bit.CH4_Mode_Setup = 5;         //Parallel mode
    //Regs.A1.Bit.Input_Pin_Control = 4;      //IN1/IN2 : IN3A/IN3B
    //motorG2007_SetMDReg(Regs.A1.Reg);
    //motorG2007_SetMDReg(Regs.A2.Reg);
    //Regs.A3.Bit.CH4_CC_FS = 0;              // 0: CC, 1: FS
    Regs.A3.Bit.CH12_Disable = 0;           //Disable CH12


    switch (phase)
    {
        case APERTURE_DUMMYLOAD_START:
            Regs.A3.Bit.IN3A = 1;              //Big iris
            Regs.A3.Bit.IN3B = 0;
            motorG2007_SetMDReg(Regs.A3.Reg);
            break;

        case APERTURE_DUMMYLOAD_END:
            /*
            Regs.A3.Bit.IN3A = 0;              //High Z status
            Regs.A3.Bit.IN3B = 0;
            motorG2007_SetMDReg(Regs.A3.Reg);
            */
            Regs.A3.Bit.IN3A = 1;                  //Brake
            Regs.A3.Bit.IN3B = 1;
            Regs.A3.Bit.CH12_Disable = 0;          //Disable CH12
            motorG2007_SetMDReg(Regs.A3.Reg);
            break;
    }
#endif
}

static void motorG2007_aperture_setState(MOTOR_APERTURE_ACT state, UINT32 param1)
{
#if IRIS_USTEP_ENABLE
    MS_CH_PHASE_CFG msSetChPhCfg;
    MSCOMMON_CFG    msCommonCfg;
#endif

    DBG_MSG("%s, state=%d, param1=%d\n\r", __func__, state, param1);

#if 0
    switch(state)
    {
        case MOTOR_APERTURE_NORMAL:
            motorG2007_aperture_setNormalState(param1);
            break;
        case MOTOR_APERTURE_DUMMYLOAD:
            motorG2007_aperture_dummyLoadState(param1);
            break;
        default:
            DBG_ERR("%s,UNKNOW\n\r", __func__);
            break;
    }
#else
    switch(state)
    {
    case MOTOR_APERTURE_RESET: // Reset iris phase(refer to spec. 7.5 IRIS STEP CONTROL TABLE).
        gG2007PvtObj.uiIrisPhase = IRIS_INIT_PHASE;
        break;
    case MOTOR_APERTURE_NORMAL:
        motorG2007_aperture_setNormalState(param1);
        break;
    case MOTOR_APERTURE_DUMMYLOAD:
        motorG2007_aperture_dummyLoadState(param1);
        break;
    case MOTOR_APERTURE_FWD:
        #if IRIS_USTEP_ENABLE
            msCommonCfg.uiDir = 1;
            msCommonCfg.uiOnCycle = 1; //uiStepCnt;

            msCommonCfg.uiStepPerPhase = ((TOTAL_08_STEP_PER_PHASE) << 3/*uiStepPerPhase*/);
            msCommonCfg.uiPhaseType = PWM_MS_1_2_PHASE_TYPE;
            #if MOTOR_PWM_THRESHOLD_EN
            msCommonCfg.uiThresholdEn = 1;//0;
            #else
            msCommonCfg.uiThresholdEn = 0;//0;
            #endif
            msCommonCfg.uiThreshold = 0;

            msSetChPhCfg.uiCH0_Phase = g_pwmChannel[gG2007PvtObj.uiIrisPhase-1].uiCh_A1;
            msSetChPhCfg.uiCH1_Phase = g_pwmChannel[gG2007PvtObj.uiIrisPhase-1].uiCh_A2;
            msSetChPhCfg.uiCH2_Phase = g_pwmChannel[gG2007PvtObj.uiIrisPhase-1].uiCh_B1;
            msSetChPhCfg.uiCH3_Phase = g_pwmChannel[gG2007PvtObj.uiIrisPhase-1].uiCh_B2;
        #endif

        if(gG2007PvtObj.uiIrisPhase == 1)
            gG2007PvtObj.uiIrisPhase = TOTAL_IRIS_PHASE;
        else
            gG2007PvtObj.uiIrisPhase--;

        #if IRIS_USTEP_ENABLE
            if(pwm_mstepConfigSet(PWM_MS_SET_2, &msSetChPhCfg, &msCommonCfg) != E_OK)
            {
                DBG_MSG("MS set config error\r\n");
                break;
            }

            pwm_mstepEnableSet(PWM_MS_SET_2); // Trigger first warm up step.
            pwm_waitSet(PWM_MS_SET_2);
            Delay_DelayMs(1); /* Delay to let slow step workable. */
        #else
            motorG2007_aperture_movePhase(gG2007PvtObj.uiIrisPhase);
            MD_DELAYUS(2084);  //To match DC-J36 spec., 3.7v/480pps.
        #endif
    break;
    case MOTOR_APERTURE_BWD:
        #if IRIS_USTEP_ENABLE
            msCommonCfg.uiDir = 0;
            msCommonCfg.uiOnCycle = 1; //uiStepCnt;

            msCommonCfg.uiStepPerPhase = ((TOTAL_08_STEP_PER_PHASE) << 3/*uiStepPerPhase*/);
            msCommonCfg.uiPhaseType = PWM_MS_1_2_PHASE_TYPE;
            #if MOTOR_PWM_THRESHOLD_EN
            msCommonCfg.uiThresholdEn = 1;//0;
            #else
            msCommonCfg.uiThresholdEn = 0;//0;
            #endif
            msCommonCfg.uiThreshold = 0;

            msSetChPhCfg.uiCH0_Phase = g_pwmChannel[gG2007PvtObj.uiIrisPhase-1].uiCh_A1;
            msSetChPhCfg.uiCH1_Phase = g_pwmChannel[gG2007PvtObj.uiIrisPhase-1].uiCh_A2;
            msSetChPhCfg.uiCH2_Phase = g_pwmChannel[gG2007PvtObj.uiIrisPhase-1].uiCh_B1;
            msSetChPhCfg.uiCH3_Phase = g_pwmChannel[gG2007PvtObj.uiIrisPhase-1].uiCh_B2;
        #endif

        if(gG2007PvtObj.uiIrisPhase >= (TOTAL_IRIS_PHASE))
            gG2007PvtObj.uiIrisPhase = 1;
        else
            gG2007PvtObj.uiIrisPhase++;

        #if IRIS_USTEP_ENABLE
            if(pwm_mstepConfigSet(PWM_MS_SET_2, &msSetChPhCfg, &msCommonCfg) != E_OK)
            {
                DBG_MSG("MS set config error\r\n");
                break;
            }

            pwm_mstepEnableSet(PWM_MS_SET_2); // Trigger first warm up step.
            pwm_waitSet(PWM_MS_SET_2);
            Delay_DelayMs(1);
        #else
            motorG2007_aperture_movePhase(gG2007PvtObj.uiIrisPhase);
            MD_DELAYUS(2084);  //To match DC-J36 spec., 3.7v/480pps.
        #endif
    break;
    case MOTOR_APERTURE_EXCITE_ON:
        Regs.A3.Bit.PS = 1;                     //Power on
        motorG2007_SetMDReg(Regs.A3.Reg);
        Regs.A0.Bit.CH34_Mode_Setup = 5;        //Parallel mode
        Regs.A0.Bit.CH1234_Brake_Sel = 1;       // LL brake
        motorG2007_SetMDReg(Regs.A0.Reg);

        Regs.A1.Bit.CH34_DAC = 5;               // 3.72V, DC-J36 Lens spec defined 3.7V/480pps.
        Regs.A1.Bit.CH34_CV_FS = 0;             // 0: CV, 1: FS
        Regs.A1.Bit.CH34_Disable = 1;           //Enable CH34
        motorG2007_SetMDReg(Regs.A1.Reg);

        #if IRIS_USTEP_ENABLE
            /* u-step control */
            msCommonCfg.uiDir = param1;

            msCommonCfg.uiOnCycle = 1; //uiStepCnt;

            msCommonCfg.uiStepPerPhase = ((TOTAL_08_STEP_PER_PHASE) << 3/*uiStepPerPhase*/);
            msCommonCfg.uiPhaseType = PWM_MS_1_2_PHASE_TYPE;
            #if MOTOR_PWM_THRESHOLD_EN
            msCommonCfg.uiThresholdEn = 1;//0;
            #else
            msCommonCfg.uiThresholdEn = 0;//0;
            #endif
            msCommonCfg.uiThreshold = 0;

            msSetChPhCfg.uiCH0_Phase = g_pwmChannel[gG2007PvtObj.uiIrisPhase-1].uiCh_A1;
            msSetChPhCfg.uiCH1_Phase = g_pwmChannel[gG2007PvtObj.uiIrisPhase-1].uiCh_A2;
            msSetChPhCfg.uiCH2_Phase = g_pwmChannel[gG2007PvtObj.uiIrisPhase-1].uiCh_B1;
            msSetChPhCfg.uiCH3_Phase = g_pwmChannel[gG2007PvtObj.uiIrisPhase-1].uiCh_B2;

            pwm_msSetClockDiv(PWM_MS_SET_2, 23);

            if(pwm_openSet(PWM_MS_SET_2) != E_OK)
            {
                DBG_MSG("MS set open error\r\n");
                break;
            }

            if(pwm_mstepConfigSet(PWM_MS_SET_2, &msSetChPhCfg, &msCommonCfg) != E_OK)
            {
                DBG_MSG("MS set config error\r\n");
                break;
            }

            pwm_pwmEnableSet(PWM_MS_SET_2); // Trigger pre-excite phase.
            MD_DELAYMS(5);
        #else
            motorG2007_aperture_movePhase(gG2007PvtObj.uiIrisPhase);
            MD_DELAYMS(5); // Could be more short?
        #endif
    break;
    case MOTOR_APERTURE_EXCITE_OFF:
        #if IRIS_USTEP_ENABLE
            MD_DELAYMS(5);
            pwm_closeSet(PWM_MS_SET_2, TRUE);
        #else
            motorG2007_aperture_movePhase(gG2007PvtObj.uiIrisPhase);
            MD_DELAYMS(5); // Could be more short?
            motorG2007_aperture_movePhase(0xFFFFFFFF); // Pull all pins low to save power.
        #endif

        Regs.A1.Bit.CH34_Disable = 0;           //Disable CH34
        motorG2007_SetMDReg(Regs.A1.Reg);

    break;
    default:
        DBG_ERR("%s, parameter error (%d)\n\r", __func__,state);
    break;
    }
#endif

}


static void motorG2007_shutter_setNormalState(UINT32 phase)
{
    if(phase>TOTAL_SHUTTER_PHASE)
    {
        DBG_ERR("%s, Shutter phase error (%d)\n\r", __func__,phase);
        return;
    }
    DBG_MSG("motorG2007_shutter_setNormalState %d\r\n",phase);

    motorG2007_setSignal(MD_SIGNAL_STANDBY,ON);  //Power on

    Regs.A0.Bit.CH6_Mode_Setup = 5;         //Parallel mode
    motorG2007_SetMDReg(Regs.A0.Reg);

    Regs.A1.Bit.CH56_Disable = 1;           //Enable CH6
    motorG2007_SetMDReg(Regs.A1.Reg);

    Regs.A2.Bit.CH6_DAC = 4;                // 260mA
    Regs.A2.Bit.CH6_CC_FS = 0;              // 0: CC, 1: FS
    motorG2007_SetMDReg(Regs.A2.Reg);

    // switch to GPIO mode.
    pinmux_setPinmux(PINMUX_FUNC_ID_SN_MES0, PINMUX_SENSOR_SEL_INACTIVE);
    pinmux_setPinmux(PINMUX_FUNC_ID_SN_MES1, PINMUX_SENSOR_SEL_INACTIVE);

    if (phase == SHUTTER_POS_OPEN)  //Shutter open
    {
        motorG2007_signal_setGPIO(SET_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_P]);
        motorG2007_signal_setGPIO(CLEAR_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_N]);
    }
    else   //Shutter close
    {
        motorG2007_signal_setGPIO(SET_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_N]);
        motorG2007_signal_setGPIO(CLEAR_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_P]);
    }

    //#NT#2010/12/03#Lincy Lin -begin
    //MD_DELAYMS(10);
    MD_DELAYMS(15);
    //#NT#2010/12/03#Lincy Lin -end

    motorG2007_signal_setGPIO(CLEAR_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_P]);
    motorG2007_signal_setGPIO(CLEAR_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_N]);

    Regs.A1.Bit.CH56_Disable = 0;           //Enable CH56
    motorG2007_SetMDReg(Regs.A1.Reg);
    motorG2007_setSignal(MD_SIGNAL_STANDBY,OFF);   //Power off
}

static void motorG2007_shutter_setCapState(UINT32 uiMode)
{
#if 0
    static UINT32 uiShutterPinMode = 0;

    DBG_MSG("motorG2007_shutter_setCapState %d\r\n",uiMode);
    if(uiMode == 0x80)
    {
        uiShutterPinMode=0;
    }
    else if(uiMode == 0x81)
    {
        uiShutterPinMode=1;
    }

    if(uiShutterPinMode==0)
    {
        motorG2007_setSignal(MD_SIGNAL_STANDBY,ON);  //Power on

        Regs.A0.Bit.CH6_Mode_Setup = 5;         //Parallel mode
        motorG2007_SetMDReg(Regs.A0.Reg);

        Regs.A1.Bit.CH56_Disable = 1;           //Enable CH6
        motorG2007_SetMDReg(Regs.A1.Reg);

        Regs.A2.Bit.CH6_DAC = 4;                // 260mA
        Regs.A2.Bit.CH6_CC_FS = 0;           // 0: CC, 1: FS
        motorG2007_SetMDReg(Regs.A2.Reg);

        pinmux_setPinmux(PINMUX_FUNC_ID_SN_MES0, PINMUX_SENSOR_SEL_ACTIVE)
        pinmux_setPinmux(PINMUX_FUNC_ID_SN_MES1, PINMUX_SENSOR_SEL_ACTIVE)
    }
    else if(uiShutterPinMode ==1)
    {
        if(uiMode == 0x81)
        {
            motorG2007_shutter_setNormalState(0);
        }
        motorG2007_setSignal(MD_SIGNAL_STANDBY,ON);  //Power on
        Regs.A0.Bit.CH6_Mode_Setup = 6;              //Prohibition
        motorG2007_SetMDReg(Regs.A0.Reg);
    }
#else
    if(uiMode == 1)
    {
        motorG2007_setSignal(MD_SIGNAL_STANDBY,ON);  //Power on

        Regs.A0.Bit.CH6_Mode_Setup = 5;         //Parallel mode
        motorG2007_SetMDReg(Regs.A0.Reg);

        Regs.A1.Bit.CH56_Disable = 1;           //Enable CH6
        motorG2007_SetMDReg(Regs.A1.Reg);

        Regs.A2.Bit.CH6_DAC = 4;                // 260mA
        Regs.A2.Bit.CH6_CC_FS = 0;           // 0: CC, 1: FS
        motorG2007_SetMDReg(Regs.A2.Reg);

        pinmux_setPinmux(PINMUX_FUNC_ID_SN_MES0, PINMUX_SENSOR_SEL_ACTIVE);
        pinmux_setPinmux(PINMUX_FUNC_ID_SN_MES1, PINMUX_SENSOR_SEL_ACTIVE);
    }
    else
    {
        Regs.A1.Bit.CH56_Disable = 0;           //Enable CH56
        motorG2007_SetMDReg(Regs.A1.Reg);
        motorG2007_setSignal(MD_SIGNAL_STANDBY,OFF);   //Power off
    }
#endif
}

static void motorG2007_shutter_dummyLoadState(UINT32 status)
{
    DBG_MSG("motorG2007_shutter_dummyLoadState %d\r\n",status);
    motorG2007_setSignal(MD_SIGNAL_STANDBY,ON);  //Power on

    Regs.A0.Bit.CH6_Mode_Setup = 5;              //Parallel mode
    motorG2007_SetMDReg(Regs.A0.Reg);

    Regs.A1.Bit.CH56_Disable = 1;                //Enable CH6
    motorG2007_SetMDReg(Regs.A1.Reg);

    Regs.A2.Bit.CH6_DAC = 4;                     // 260mA
    Regs.A2.Bit.CH6_CC_FS = 0;                   // 0: CC, 1: FS
    motorG2007_SetMDReg(Regs.A2.Reg);

    // switch to GPIO mode.
    pinmux_setPinmux(PINMUX_FUNC_ID_SN_MES0, PINMUX_SENSOR_SEL_INACTIVE);
    pinmux_setPinmux(PINMUX_FUNC_ID_SN_MES1, PINMUX_SENSOR_SEL_INACTIVE);

    switch(status)
    {
    case SHUTTER_DUMMYLOAD_START:
        //Open shutter
        motorG2007_signal_setGPIO(SET_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_P]);
        motorG2007_signal_setGPIO(CLEAR_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_N]);
        break;
    case SHUTTER_DUMMYLOAD_END:
        motorG2007_signal_setGPIO(CLEAR_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_P]);
        motorG2007_signal_setGPIO(CLEAR_GPIO,gG2007PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_N]);
        break;
    default:
        DBG_ERR("%s, parameter error (%d)\n\r", __func__,status);
        break;
    }
}

static void motorG2007_shutter_setState(MOTOR_SHUTTER_ACT state, UINT32 param1)
{
    DBG_MSG("%s, state=%d, param1=%d\n\r", __func__, state, param1);
    switch(state)
    {
        case MOTOR_SHUTTER_NORMAL:
            motorG2007_shutter_setNormalState(param1);
            break;
        case MOTOR_SHUTTER_CAP:
            motorG2007_shutter_setCapState(param1);
            break;
        case MOTOR_SHUTTER_DUMMYLOAD:
            motorG2007_shutter_dummyLoadState(param1);
            break;
        default:
            DBG_ERR("%s,UNKNOW\n\r", __func__);
            break;
    }

}

/*---------------------------------------------------------------
                The Driver for PWM control
----------------------------------------------------------------*/
#if ZOOM_PWM_ENABLE
static UINT32 motorG2007_signal_enablePWM(UINT32 PWMIdx, UINT32 Div, UINT32 Prd, UINT32 OnCnt, UINT32 Inv)
{
    ER       erReturn;
    PWM_CFG  PWMInfo;

    //pll_setPWMClockRate(uiOffset, pPWMCfg->uiDiv);
    PWMInfo.uiDiv=Div;
    PWMInfo.uiPrd=Prd;
    PWMInfo.uiFall=OnCnt;
    PWMInfo.uiOnCycle=0;
    PWMInfo.uiInv=Inv;
    PWMInfo.uiRise=0;

    if((gG2007PvtObj.uiZoomStatus == DCM_OFF) /*|| (gG2007PvtObj.uiZoomStatus == DCM_BRAKE)*/)
    {
        if ((erReturn = pwm_open(PWMIdx)) != 0)
        {
            DBG_ERR("PWM open error!\r\n");
            pwm_stop(PWMIdx);
            pwm_close(PWMIdx, TRUE);
            if(pwm_open(PWMIdx)!=0)
            {
                DBG_ERR("PWM open error!\r\n");
                return erReturn;
            }
        }
    }

    if ((erReturn = pwm_set(PWMIdx, &PWMInfo)) != 0)
    {
        DBG_ERR("PWMSet error!\r\n");
        return erReturn;
    }

    if((gG2007PvtObj.uiZoomStatus == DCM_OFF) /*|| (gG2007PvtObj.uiZoomStatus == DCM_BRAKE)*/)
    {
        pwm_en(PWMIdx);
        //DBG_MSG("#pwm_en\r\n");
    }

    else
    {
        pwm_pwmReload(PWMIdx);
        //DBG_MSG("#pwm_reload\r\n");
    }

    return 0;
}

#if 0
static void motorG2007_signal_disablePWM(void)
{

    if(gG2007PvtObj.uiZoomStatus == DCM_FORWARD)
    {
        if (pwm_stop(MOTOR_G2007_PWMID_ZOOM_PIN1) != E_OK)
        {
            DBG_ERR("pwm_stop error! (%d)\r\n",MOTOR_G2007_PWMID_ZOOM_PIN1);
        }

        if (pwm_close(MOTOR_G2007_PWMID_ZOOM_PIN1, TRUE) != E_OK)
        {
            DBG_ERR("pwm_stop error! (%d)\r\n",MOTOR_G2007_PWMID_ZOOM_PIN1);
        }

        pinmux_disable_pwm(MOTOR_G2007_PINMUX_ZOOM_PIN1);
    }
    else if(gG2007PvtObj.uiZoomStatus == DCM_REVERSE)
    {

        if (pwm_stop(MOTOR_G2007_PWMID_ZOOM_PIN0) != E_OK)
        {
            DBG_ERR("pwm_stop error! (%d)\r\n",MOTOR_G2007_PWMID_ZOOM_PIN0);
        }

        if (pwm_close(MOTOR_G2007_PWMID_ZOOM_PIN0, TRUE) != E_OK)
        {
            DBG_ERR("pwm_close error! (%d)\r\n",MOTOR_G2007_PWMID_ZOOM_PIN0);
        }

        pinmux_disable_pwm(MOTOR_G2007_PINMUX_ZOOM_PIN0);

    }
    else
    {
        DBG_ERR("motorG2007_signal_disablePWM, zoom status error \r\n");
        return;
    }

}
#endif
#endif

static void motorG2007_signal_setGPIO(BOOL set, UINT32 gpio)
{
    DBG_MSG("%s , %s %d\r\n", __func__,(set == SET_GPIO)?"SET":"CLEAR",gpio);

    gpio_setDir(gpio, GPIO_DIR_OUTPUT);
    switch(set)
    {
        case SET_GPIO:
            gpio_setPin(gpio);
        break;
        case CLEAR_GPIO:
            gpio_clearPin(gpio);
        break;
    }
}

