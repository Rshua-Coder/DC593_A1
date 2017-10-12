#ifndef _LENSMOTOR_FF1_H
#define _LENSMOTOR_FF1_H
#include "Type.h"
#include "Pad.h"
#include "pwm.h"

#define TOTAL_IRIS_PHASE 2
#define TOTAL_SHUTTER_PHASE 2

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

/**
    Motor object.
    All motor controls will be process via motor object.
*/
typedef struct _MOTOR_PVT_OBJ
{
    //GPIO related var
    UINT32        uiPinIN[MOTOR_MAX_INPUT_PIN];
}MOTOR_PVT_OBJ, *PMOTOR_PVT_OBJ;

#endif
