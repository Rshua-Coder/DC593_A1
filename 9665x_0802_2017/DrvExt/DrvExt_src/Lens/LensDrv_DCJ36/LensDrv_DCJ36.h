#ifndef _LENSDRV_DCJ36_H
#define _LENSDRV_DCJ36_H

#include "Type.h"

#define ISR_USAGE
//#define GPIO_INT_MCTLA      GPIO_INT_15

//--------------------------------------------------------------------------------------------------------------------------
//Part of Zoom
//--------------------------------------------------------------------------------------------------------------------------

#define LENSZPR_ADC_THRESHOLD  2048

#define ZOOM_SECTION_TELE   11
#define ZOOM_SECTION_WIDE   1
#define FOCUS_DISTANCE      12
#define ZOOM_STOPPER_COUNT      1150
#define ZOOM_GARAGE_COUNT       1080
#define ZOOM_STANDARD_A         327//[A]
#define ZOOM_STANDARD_D         1033

#define ZOOM_BL_STEP            70
#define ZOOM_POS_INNER_STOP     (-277)
#define ZOOM_POS_GARAGE         (-266)
#define ZOOM_POS_HOME_PR        0
#define ZOOM_POS_WIDE           71
#define ZOOM_POS_TELE           743
#define ZOOM_POS_OUTER_STOP     760

#define ZOOM_BWD_INTERVAL       10

#define ZPI_RISING_EDGE_TRIGGER 1 // 1: Rising edge trigger; 0: Falling edge trigger.

//--------------------------------------------------------------------------------------------------------------------------
//Part of Focus
//--------------------------------------------------------------------------------------------------------------------------
#define FOCUSPI_OUTSDIE_LEVEL           1
#define FOCUSPI_INSDIE_LEVEL            0

#define FOCUS_RANGE_START               -57    //(-277-250)/2
#define FOCUS_RANGE_END                 1146   //Near linit stopper

#define FOCUS_POS_HOME_DEF       0
#define FOCUS_POS_INNER_STOP     (FOCUS_POS_HOME_DEF - 87)
#define FOCUS_POS_GARAGE         (FOCUS_POS_HOME_DEF - 67)
#define FOCUS_POS_FAR            (FOCUS_POS_HOME_DEF - 67)//NOte: Far-point is behind Garage
#define FOCUS_POS_HOME_PR        (FOCUS_POS_HOME_DEF-0)
#define FOCUS_POS_NORMAL_WIDE    364//For ZP1, Inf
#define FOCUS_POS_NEAR           382//For ZP1, 0.623m
#define FOCUS_POS_OUTER_STOP     1146 // (FOCUS_POS_HOME_DEF+793)

#define FOCUS_BWD_LIMIT_OUTSIDE_PI  ((FOCUS_POS_OUTER_STOP-FOCUS_POS_HOME_PR)+20)    //limitation for  FPI search.
#define FOCUS_FWD_LIMIT_INSIDE_PI   ((FOCUS_POS_HOME_PR-FOCUS_POS_INNER_STOP)+20+300)//limitation for  FPI search.
#define FOCUS_BWD_INTERVAL          10
#define FOCUS_FWD_INTERVAL          1

#define FOCUS_BL_STEP               28

#define FOCUS_SPEED                 667//1// equal to (1000000/FOCUS_SPEED)pps

#define IRIS_TOTAL_STEP             25 // 25 -> 0 = 25 steps.
#define IRIS_TOTAL_SEGMENT          6  // 1.8, 2.0, 2.8, 4.0, 5.6, 8.0.

typedef enum {
    ZOOM_STATUS_NULL = 0,
    ZOOM_STATUS_ZB_RETRACT,
    ZOOM_STATUS_FIND_HP,
    ZOOM_STATUS_ZC_2_ZB,
    ZOOM_STATUS_FW_BRAKE,
    ZOOM_STATUS_BW_BRAKE,
    ZOOM_STATUS_MOVE,
    ZOOM_STATUS_IDLE,
    ZOOM_STATUS_ERROR,
    ENUM_DUMMY4WORD(LENSDRV_DCJ36_ZOOM_STATUS)
} LENSDRV_DCJ36_ZOOM_STATUS;

typedef struct
{
    //#NT#Patch for precision improve
    #if (PRECISION_UPGRADE)
    ISR_USAGE volatile UINT32         IntTypePol;
    #endif
    ISR_USAGE volatile UINT32         ZoomStatus;
              volatile UINT32         ZoomWideCnt;
    ISR_USAGE volatile UINT32         ZoomDir;
    ISR_USAGE volatile UINT32         ZoomLastDir;
    ISR_USAGE volatile INT32          ZoomStep;
    ISR_USAGE volatile UINT32         StepMove;
    ISR_USAGE volatile UINT32         StepCnt;
    ISR_USAGE volatile UINT32         ZoomSection;
    ISR_USAGE volatile UINT32         ZoomIsr;
    ISR_USAGE volatile UINT32         ZommSpdSlwDownDly;
    ISR_USAGE volatile UINT32         ZoomHP;
    ISR_USAGE volatile UINT32         ZoomInDuty1;
    ISR_USAGE volatile UINT32         ZoomInDuty2;
    ISR_USAGE volatile UINT32         ZoomOutDuty1;
    ISR_USAGE volatile UINT32         ZoomOutDuty2;
    ISR_USAGE volatile UINT32         ZoomOutDuty3;
    ISR_USAGE volatile UINT32         ZoomOutDuty4;
    ISR_USAGE volatile UINT32         ZoomInitFlag;
    ISR_USAGE volatile BOOL           bZoomBoundaryAutoCheck;
    ISR_USAGE volatile BOOL           bDoOptZoomDraw;
                       BOOL           bFocusAfterZoom;
                       BOOL           bFocusOnceZoomChanged;
                       INT32          FocusPosition;
                       UINT32         AperturePos;
                       UINT32         ZPI;
                       UINT32         ZPR;
                       UINT32         FPI;
                       UINT32         ZPIINT;
}LENSDRV_DCJ36_PARA;


#endif


