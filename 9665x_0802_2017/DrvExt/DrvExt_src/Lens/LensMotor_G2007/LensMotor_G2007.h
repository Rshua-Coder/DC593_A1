#ifndef _LENSMOTOR_G2007_H
#define _LENSMOTOR_G2007_H
#include "Type.h"
#include "Pad.h"
#include "pwm.h"

#if 0
#define TOTAL_FOCUS_PHASE 4 // 2-2phase
#else
#define TOTAL_FOCUS_PHASE 8 // 1-2phase
#endif
#define TOTAL_IRIS_PHASE 8
#define IRIS_INIT_PHASE 2
#define TOTAL_SHUTTER_PHASE 2

#define MOTOR_G2007_PWMID_ZOOM_PIN0               PWMID_2
#define MOTOR_G2007_PWMID_ZOOM_PIN1               PWMID_3
#define MOTOR_G2007_PINMUX_ZOOM_PIN0              PINMUX_PWM2
#define MOTOR_G2007_PINMUX_ZOOM_PIN1              PINMUX_PWM3

#define MOTOR_G2007_SIF_CH                       SIF_CH2
#define MOTOR_G2007_PINMUX_SIF_CH                PINMUX_SIF_CH2

#define MOTOR_MAX_INPUT_PIN                      12
#define MOTOR_CTRL_PIN_IDX_SH_P                  10 //IN6A
#define MOTOR_CTRL_PIN_IDX_SH_N                  11 //IN6B
#define MOTOR_CTRL_PIN_IDX_ZOOM_P                8  //IN5A
#define MOTOR_CTRL_PIN_IDX_ZOOM_N                9  //IN5B
#define MOTOR_CTRL_PIN_IDX_AF_AP                 0  //IN1A
#define MOTOR_CTRL_PIN_IDX_AF_AN                 1  //IN1B
#define MOTOR_CTRL_PIN_IDX_AF_BP                 2  //IN2A
#define MOTOR_CTRL_PIN_IDX_AF_BN                 3  //IN2B
#define MOTOR_CTRL_PIN_IDX_IRIS_AP               4  //IN3A
#define MOTOR_CTRL_PIN_IDX_IRIS_AN               5  //IN3B
#define MOTOR_CTRL_PIN_IDX_IRIS_BP               6  //IN4A
#define MOTOR_CTRL_PIN_IDX_IRIS_BN               7  //IN4B


#define MDREGDEF_BEGIN(name)     \
typedef union                    \
{                                \
    UINT16      Reg;             \
    struct                       \
    {                            \

#define MDREGDEF_BIT(field, bits)\
    unsigned field : bits;

#define MDREGDEF_END(name)       \
    } Bit;                       \
} T_##name;


//A0
MDREGDEF_BEGIN(MDCTRL_A0)
    MDREGDEF_BIT(CH56_STM_Sel        , 1)
    MDREGDEF_BIT(CH1234_Brake_Sel    , 1)
    MDREGDEF_BIT(CH6_Mode_Setup      , 3)
    MDREGDEF_BIT(CH5_Mode_Setup      , 3)
    MDREGDEF_BIT(CH34_Mode_Setup     , 3)
    MDREGDEF_BIT(CH12_Mode_Setup     , 3)
    MDREGDEF_BIT(ADDR                , 2)
MDREGDEF_END(MDCTRL_A0)

//A1
MDREGDEF_BEGIN(MDCTRL_A1)
    MDREGDEF_BIT(CC_weak_Exc         , 1)
    MDREGDEF_BIT(CH56_Disable        , 1)
    MDREGDEF_BIT(CH34_Disable        , 1)
    MDREGDEF_BIT(CH12_Disable        , 1)
    MDREGDEF_BIT(CH34_CV_FS          , 1)
    MDREGDEF_BIT(CH12_CV_FS          , 1)
    MDREGDEF_BIT(CH34_DAC            , 4)
    MDREGDEF_BIT(CH12_DAC            , 4)
    MDREGDEF_BIT(ADDR                , 2)
MDREGDEF_END(MDCTRL_A1)

//A2
MDREGDEF_BEGIN(MDCTRL_A2)
    MDREGDEF_BIT(PI3_Control         , 1)
    MDREGDEF_BIT(PI2_Control         , 1)
    MDREGDEF_BIT(PI1_Control         , 1)
    MDREGDEF_BIT(CH6_CC_FS           , 1)
    MDREGDEF_BIT(CH5_CC_CV_FS        , 2)
    MDREGDEF_BIT(CH6_DAC             , 4)
    MDREGDEF_BIT(CH5_DAC             , 4)
    MDREGDEF_BIT(ADDR                , 2)
MDREGDEF_END(MDCTRL_A2)

//A3

MDREGDEF_BEGIN(MDCTRL_A3)
    MDREGDEF_BIT(SW_Reset            , 1)
    MDREGDEF_BIT(PS                  , 1)
    MDREGDEF_BIT(C6S2                , 1)
    MDREGDEF_BIT(C6S1                , 1)
    MDREGDEF_BIT(C5S2                , 1)
    MDREGDEF_BIT(C5S1                , 1)
    MDREGDEF_BIT(C4S2                , 1)
    MDREGDEF_BIT(C4S1                , 1)
    MDREGDEF_BIT(C3S2                , 1)
    MDREGDEF_BIT(C3S1                , 1)
    MDREGDEF_BIT(C2S2                , 1)
    MDREGDEF_BIT(C2S1                , 1)
    MDREGDEF_BIT(C1S2                , 1)
    MDREGDEF_BIT(C1S1                , 1)
    MDREGDEF_BIT(ADDR                , 2)
MDREGDEF_END(MDCTRL_A3)

typedef struct
{
    T_MDCTRL_A0         A0;
    T_MDCTRL_A1         A1;
    T_MDCTRL_A2         A2;
    T_MDCTRL_A3         A3;
}MD_REGS;

/**
    Motor object.
    All motor controls will be process via motor object.
*/
typedef struct _MOTOR_PVT_OBJ
{
    //GPIO related var
    UINT32        uiPinIN[MOTOR_MAX_INPUT_PIN];
    UINT32        uiPinOutputZPI; //DSP output to MD
    UINT32        uiPinOutputZPR; //DSP output to MD
    UINT32        uiPinOutputFPI; //DSP output to MD
    //Other info
    BOOL          uiHWPower;
    DCM_STATUS    uiZoomStatus;
    UINT32        uiZoomSpeed;
    UINT32        uiFocusPhase;
    UINT32        uiIrisPhase;
    BOOL          bMicroStepEn;
}MOTOR_PVT_OBJ, *PMOTOR_PVT_OBJ;

#endif
