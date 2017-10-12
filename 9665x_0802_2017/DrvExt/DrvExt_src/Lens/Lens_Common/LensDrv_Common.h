#ifndef _LENSDRV_COMMON_H
#define _LENSDRV_COMMON_H
#include "Type.h"


#define PRECISION_UPGRADE DISABLE


#define DIRECTION_FWD 0
#define DIRECTION_BWD 1

#define ZOOMPR_HI_LEVEL                 1
#define ZOOMPR_LOW_LEVEL                0

#define ZOOM_SPEED_100     100
#define ZOOM_SPEED_90      90
#define ZOOM_SPEED_80      80
#define ZOOM_SPEED_75      75
#define ZOOM_SPEED_65      65
#define ZOOM_SPEED_63      63
#define ZOOM_SPEED_60      60
#define ZOOM_SPEED_50      50
#define ZOOM_SPEED_48      48
#define ZOOM_SPEED_40      40
#define ZOOM_SPEED_35      35
#define ZOOM_SPEED_30      30
#define ZOOM_SPEED_25      25
#define ZOOM_SPEED_22      22
#define ZOOM_SPEED_20      20
#define ZOOM_SPEED_10      10

#define ZOOM_COUNT_36          36
#define ZOOM_COUNT_30          30
#define ZOOM_COUNT_26          26
#define ZOOM_COUNT_18          18
#define ZOOM_COUNT_12          12
#define ZOOM_COUNT_10          10
#define ZOOM_COUNT_6           6
#define ZOOM_COUNT_4           4
#define ZOOM_COUNT_2           2
#define ZOOM_COUNT_1           1
#define ZOOM_COMPENSATION_8    8
#define ZOOM_COMPENSATION_12   12

typedef enum __LENS_ZOOM_Isr {
    ZOOM_Isr_Open = 0x00,
    ZOOM_Isr_Normal,
    ZOOM_Isr_Close,
    ZOOM_Isr_AbnormalClose,
    ZOOM_Isr_Null

} LENS_ZOOM_Isr;

//For calibration usage
#define CAL_USAGE

#endif
