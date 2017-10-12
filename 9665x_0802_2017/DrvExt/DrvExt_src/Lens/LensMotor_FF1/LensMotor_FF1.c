#include "LensMotor_Common.h"
#include "LensMotor_FF1.h"
#include "Utility.h"
#include "GPIO.h"
#include "sif.h"
#include "top.h"
#include "ErrorNo.h"
#include "DxLens.h"
#include "Debug.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          LENSMOTORFF1
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define MD_DELAYMS(x)   Delay_DelayMs(x)
#define MD_DELAYUS(x)   Delay_DelayUs(x)
#define LENS_BINARY_PHASE(b3, b2, b1, b0)       ((b0)|((b1)<<1)|((b2)<<2)|((b3)<<3))

#define SET_GPIO            1
#define CLEAR_GPIO          0


static void     motorFF1_init(PLENS_DEVICE_OBJ pLensDev);
static void     motorFF1_setSignal(MD_SIGNAL signal, UINT32 on);
static BOOL     motorFF1_zoom_checkPI(void);
static void     motorFF1_zoom_setState(MOTOR_ZOOM_ACT state, UINT32 param1);
static void     motorFF1_zoom_setSpeed(UINT32 uiSpeed);
static UINT32   motorFF1_zoom_getSpeed(void);
static BOOL     motorFF1_focus_setState(MOTOR_FOCUS_ACT state, UINT32 param1);
static void     motorFF1_aperture_setState(MOTOR_APERTURE_ACT state, UINT32 param1);
static void     motorFF1_shutter_setState(MOTOR_SHUTTER_ACT state, UINT32 param1);
static void     motorFF1_signal_setGPIO(BOOL set, UINT32 gpio);

static MOTOR_TAB gmotorFF1Obj =
{
    motorFF1_init,
    motorFF1_setSignal,
    motorFF1_zoom_setSpeed,
    motorFF1_zoom_getSpeed,
    motorFF1_zoom_setState,
    motorFF1_zoom_checkPI,
    motorFF1_focus_setState,
    motorFF1_aperture_setState,
    motorFF1_shutter_setState
};

static MOTOR_PVT_OBJ gFF1PvtObj =
{
    //GPIO related var
    {0}, //uiPinIN[MOTOR_MAX_INPUT_PIN];
};

static void motorFF1_initGPIO(PLENS_DEVICE_OBJ pLensDev)
{
    UINT32 i, ui8InPinIdx = LENS_IO_IN_0;

    for(i = 0; i < MOTOR_MAX_INPUT_PIN; i++)
    {
        gFF1PvtObj.uiPinIN[i]   =  pLensDev->pGPIOArray[ui8InPinIdx + i];
    }
}

/**
    motor init

    Init FF1 pin assignment

    @param None.

    @return None.
*/
static void motorFF1_init(PLENS_DEVICE_OBJ pLensDev)
{
    motorFF1_initGPIO(pLensDev);

    return;
}

/**
    Get FF1 motor object

    @return FF1 motor object pointer

*/
PMOTOR_TAB motor_getObject(void)
{
    DBG_MSG("%s\r\n", __func__);

    return &gmotorFF1Obj;
}

static void motorFF1_setSignal(MD_SIGNAL signal, UINT32 on)
{

}

static BOOL motorFF1_zoom_checkPI(void)
{
    return FALSE;
}

static void motorFF1_zoom_setState(MOTOR_ZOOM_ACT state, UINT32 param1)
{

}
static void motorFF1_zoom_setSpeed(UINT32 uiSpeed)
{

}
static UINT32 motorFF1_zoom_getSpeed(void)
{
    return 0;
}

static BOOL motorFF1_focus_setState(MOTOR_FOCUS_ACT state, UINT32 param1)
{
    return TRUE;
}

static void motorFF1_aperture_setNormalState(UINT32 phase)
{
    DBG_MSG("motorFF1_aperture_setNormalState %d\r\n",phase);

    switch (phase)
    {
    case IRIS_POS_BIG:
        motorFF1_signal_setGPIO(SET_GPIO, gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRIS_AP]);
        motorFF1_signal_setGPIO(CLEAR_GPIO, gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRIS_AN]);
        break;

    case IRIS_POS_SMALL:
        motorFF1_signal_setGPIO(SET_GPIO, gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRIS_AN]);
        motorFF1_signal_setGPIO(CLEAR_GPIO, gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRIS_AP]);
        break;

    case IRIS_POS_CLOSE:
        motorFF1_signal_setGPIO(CLEAR_GPIO, gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRIS_AP]);
        motorFF1_signal_setGPIO(CLEAR_GPIO, gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRIS_AN]);
        break;
    default:
        DBG_ERR("%s, Iris phase error (%d)\n\r", __func__,phase);
        return;
    }

    MD_DELAYMS(15);

    motorFF1_signal_setGPIO(SET_GPIO, gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRIS_AP]);
    motorFF1_signal_setGPIO(SET_GPIO, gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRIS_AN]);
}

static void motorFF1_aperture_dummyLoadState(UINT32 phase)
{
#if 0
    DBG_MSG("motorFF1_aperture_dummyLoadState %d\r\n",phase);

    switch (phase)
    {
        case APERTURE_DUMMYLOAD_START:
            Regs.A3.Bit.IN3A = 1;              //Big iris
            Regs.A3.Bit.IN3B = 0;
            break;

        case APERTURE_DUMMYLOAD_END:
            /*
            Regs.A3.Bit.IN3A = 0;              //High Z status
            Regs.A3.Bit.IN3B = 0;
            motorFF1_SetMDReg(Regs.A3.Reg);
            */
            Regs.A3.Bit.IN3A = 1;                  //Brake
            Regs.A3.Bit.IN3B = 1;
            Regs.A3.Bit.CH12_Disable = 0;          //Disable CH12
            motorFF1_SetMDReg(Regs.A3.Reg);
            break;
    }
#endif
}

static void motorFF1_aperture_setState(MOTOR_APERTURE_ACT state, UINT32 param1)
{
    DBG_MSG("%s, state=%d, param1=%d\n\r", __func__, state, param1);

    switch(state)
    {
        case MOTOR_APERTURE_NORMAL:
            motorFF1_aperture_setNormalState(param1);
            break;
        case MOTOR_APERTURE_DUMMYLOAD:
            motorFF1_aperture_dummyLoadState(param1);
            break;
        default:
            DBG_ERR("%s,UNKNOW\n\r", __func__);
            break;
    }
}


static void motorFF1_shutter_setNormalState(UINT32 phase)
{
    if(phase>TOTAL_SHUTTER_PHASE)
    {
        DBG_ERR("%s, Shutter phase error (%d)\n\r", __func__,phase);
        return;
    }
    DBG_MSG("motorFF1_shutter_setNormalState %d\r\n",phase);

    if (phase == SHUTTER_POS_OPEN)  //Shutter open
    {
        motorFF1_signal_setGPIO(SET_GPIO,gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_P]);
        motorFF1_signal_setGPIO(CLEAR_GPIO,gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_N]);
    }
    else   //Shutter close
    {
        motorFF1_signal_setGPIO(SET_GPIO,gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_N]);
        motorFF1_signal_setGPIO(CLEAR_GPIO,gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_P]);
    }

    MD_DELAYMS(15);

    motorFF1_signal_setGPIO(CLEAR_GPIO,gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_P]);
    motorFF1_signal_setGPIO(CLEAR_GPIO,gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_N]);
}

static void motorFF1_shutter_setCapState(UINT32 uiMode)
{
    static UINT32 uiShutterPinMode = 0;

    DBG_MSG("motorFF1_shutter_setCapState %d\r\n",uiMode);
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

    }
    else if(uiShutterPinMode ==1)
    {
        if(uiMode == 0x81)
        {
            motorFF1_shutter_setNormalState(0);
        }
    }
}

static void motorFF1_shutter_dummyLoadState(UINT32 status)
{
    DBG_MSG("motorFF1_shutter_dummyLoadState %d\r\n",status);

    switch(status)
    {
    case SHUTTER_DUMMYLOAD_START:
        //Open shutter
        motorFF1_signal_setGPIO(SET_GPIO,gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_P]);
        motorFF1_signal_setGPIO(CLEAR_GPIO,gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_N]);
        break;
    case SHUTTER_DUMMYLOAD_END:
        motorFF1_signal_setGPIO(CLEAR_GPIO,gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_P]);
        motorFF1_signal_setGPIO(CLEAR_GPIO,gFF1PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_SH_N]);
        break;
    default:
        DBG_ERR("%s, parameter error (%d)\n\r", __func__,status);
        break;
    }
}

static void motorFF1_shutter_setState(MOTOR_SHUTTER_ACT state, UINT32 param1)
{
    DBG_MSG("%s, state=%d, param1=%d\n\r", __func__, state, param1);
    switch(state)
    {
        case MOTOR_SHUTTER_NORMAL:
            motorFF1_shutter_setNormalState(param1);
            break;
        case MOTOR_SHUTTER_CAP:
            motorFF1_shutter_setCapState(param1);
            break;
        case MOTOR_SHUTTER_DUMMYLOAD:
            motorFF1_shutter_dummyLoadState(param1);
            break;
        default:
            DBG_ERR("%s,UNKNOW\n\r", __func__);
            break;
    }

}

static void motorFF1_signal_setGPIO(BOOL set, UINT32 gpio)
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

