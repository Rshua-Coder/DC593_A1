#ifndef _LENSMOTOR_COMMON_H
#define _LENSMOTOR_COMMON_H

typedef enum _DCM_STATUS
{
    DCM_OFF = 0,
    DCM_FORWARD,
    DCM_REVERSE,
    DCM_BRAKE,
    DCM_ERROR,
    DCM_MAXIDX
}DCM_STATUS;//DC Motor Status

typedef enum _MOTOR_SPEED
{
    SPEED_OFF  =   0,
    SPEED_FULL = 100,
    SPEED_MAXIDX
} MOTOR_SPEED;//Motor Speed

#endif
