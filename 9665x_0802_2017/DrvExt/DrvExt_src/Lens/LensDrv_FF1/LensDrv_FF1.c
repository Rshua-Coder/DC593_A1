
#include "SysKer.h"
#include "Timer.h"
#include "consts.h"
#include "flag.h"
#include "Interrupt.h"
#include "stdio.h"
#include "gpio.h"
#include "LensDrv_Common.h"
#include "LensDrv_FF1.h"
#include "DxLens.h"
#include "Utility.h"
#include "ADC.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          LENSDRVFF1
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define LD_DELAYMS(x)   Delay_DelayMs(x)
#define LD_DELAYUS(x)   Delay_DelayUs(x)


//static UINT32 Aperture_Pos_Tbl[IRIS_TOTAL_SEGMENT] = {5, 7, 10, 13, 16, 19};
static LENSDRV_FF1_PARA FF1_Para={0};

//Prototype declaration
static void        lensFF1_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab);
static UINT32      lensFF1_getSignal(LD_SIGNAL signal);
static void        lensFF1_Zoom_InitPart1(void);
static INT32       lensFF1_Zoom_InitPart2(void);
static void        lensFF1_Zoom_SetBoundaryCheck(BOOL en);
static void        lensFF1_Zoom_SetSpeed(ZOOM_SPEED_CATEGORY category);
static UINT32      lensFF1_Zoom_GetMinSection(void);
static UINT32      lensFF1_Zoom_GetMaxSection(void);
static UINT32      lensFF1_Zoom_GetSection(void);
static UINT32      lensFF1_Zoom_GetStep(void);
static UINT32      lensFF1_Zoom_GetSectionStep(void);
static void        lensFF1_Zoom_GoSection(UINT32 section);
static void        lensFF1_Zoom_GoStep(INT32 position);
static INT32       lensFF1_Zoom_Press(UINT32 zoom);
static UINT32      lensFF1_Zoom_Release(void);
static INT32       lensFF1_Zoom_Retract(void);
static INT32       lensFF1_Focus_Init(void);
static UINT32      lensFF1_Focus_GetLength(UINT32 ZoomSection);
static INT32       lensFF1_Focus_GetPosition(void);
static UINT32      lensFF1_Focus_GetDistTable(UINT32 section);
static INT32       lensFF1_Focus_GetDefultTable(UINT32 section, UINT32 FocusDistance);
static INT32       lensFF1_Focus_GetTable(UINT32 section, UINT32 FocusDistance);
static INT32       lensFF1_Focus_GetRange(FOCUS_RANGE range);
static void        lensFF1_Focus_DoEXC(BOOL on,UINT32 dir);
static void        lensFF1_Focus_Go2(INT32 Position);
static INT32       lensFF1_Focus_Reset(FOCUS_RETRACT_POSITION position);
static void        lensFF1_Aperture_Init(void);
static IRIS_POS    lensFF1_Aperture_GetPosition(void);
static UINT32      lensFF1_Aperture_GetFNO(UINT32 ZoomSection,UINT32 IrisPos);
static void        lensFF1_Aperture_Go2Pos(IRIS_POS position);
static void        lensFF1_shutter_setState(MOTOR_SHUTTER_ACT state, UINT32 param1);
static INT32       lensFF1_LensRetract(void);
static INT32       lensFF1_LensReset(void);
static INT32       lensFF1_LensPowerOff(void);
static void        lensFF1_MakeAFTable(void);
static INT32       lensFF1_SetCalData(INT16 *pCalTbl, UINT32 uiTblSize);
static void        lensFF1_Focus_AfterZoomMoving(UINT32 uiCmd);

static LENSCTRL_TASKAPIOBJ g_FF1_LensCtrlTskAPIObj = {NULL};

static LENS_TAB g_pFF1Obj =
{
    //Lens
    lensFF1_init,
    lensFF1_getSignal,
    //Zoom
    lensFF1_Zoom_InitPart1,
    lensFF1_Zoom_InitPart2,
    lensFF1_Zoom_SetBoundaryCheck,
    lensFF1_Zoom_SetSpeed,
    lensFF1_Zoom_GetMinSection,
    lensFF1_Zoom_GetMaxSection,
    lensFF1_Zoom_GetSection,
    lensFF1_Zoom_GetStep,
    lensFF1_Zoom_GetSectionStep,
    lensFF1_Zoom_GoSection,
    NULL,
    lensFF1_Zoom_GoStep,
    lensFF1_Zoom_Press,
    lensFF1_Zoom_Release,
    lensFF1_Zoom_Retract,
    //Focus
    lensFF1_Focus_Init,
    lensFF1_Focus_GetLength,
    lensFF1_Focus_GetPosition,
    lensFF1_Focus_GetDistTable,
    lensFF1_Focus_GetDefultTable,
    lensFF1_Focus_GetTable,
    lensFF1_Focus_GetRange,
    lensFF1_Focus_DoEXC,
    lensFF1_Focus_Go2,
    lensFF1_Focus_Reset,
    //Iris
    lensFF1_Aperture_Init,
    lensFF1_Aperture_GetPosition,
    lensFF1_Aperture_GetFNO,
    lensFF1_Aperture_Go2Pos,
    //Shutter
    lensFF1_shutter_setState,
    //Misc
    lensFF1_LensRetract,
    lensFF1_LensReset,
    lensFF1_MakeAFTable,
    lensFF1_SetCalData,
    lensFF1_Focus_AfterZoomMoving,
    lensFF1_LensPowerOff,
    NULL,
    &g_FF1_LensCtrlTskAPIObj // PLENSCTRL_TASKAPIOBJ

};

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for lens control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensFF1_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab)
{
    DBG_MSG("%s\r\n", __func__);
    g_pFF1Obj.pMotor = pMotorTab;
}

/**
    Get FF1 lens object

    @return FF1 lens object pointer

*/
PLENS_TAB lens_getObject(void)
{
    DBG_MSG("%s\r\n", __func__);

    return &g_pFF1Obj;
}
static UINT32 lensFF1_getSignal(LD_SIGNAL signal)
{
    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
//Part of Zoom
//--------------------------------------------------------------------------------------------------------------------------
static void lensFF1_Zoom_InitPart1(void)
{

}

static INT32 lensFF1_Zoom_InitPart2(void)
{
    return ERR_OK;
}

static void lensFF1_Zoom_SetBoundaryCheck(BOOL en)
{

}

static void lensFF1_Zoom_SetSpeed(ZOOM_SPEED_CATEGORY category)
{

}

static UINT32 lensFF1_Zoom_GetMinSection(void)
{
    return 0;
}

static UINT32 lensFF1_Zoom_GetMaxSection(void)
{
    return 0;
}

static UINT32 lensFF1_Zoom_GetSection(void)
{
    return 0;
}

static UINT32 lensFF1_Zoom_GetStep(void)
{
    return 0;
}

static UINT32 lensFF1_Zoom_GetSectionStep(void)
{
    return 0;
}

static void lensFF1_Zoom_GoSection(UINT32 section)
{

}

/* position: absolute position. */
static void lensFF1_Zoom_GoStep(INT32 position)
{

}

static INT32 lensFF1_Zoom_Press(UINT32 zoom)
{
    return ERR_OK;
}

static UINT32 lensFF1_Zoom_Release(void)
{

    return 0;
}

static INT32 lensFF1_Zoom_Retract(void)
{
    return ERR_OK;
}


//--------------------------------------------------------------------------------------------------------------------------
//Part of Focus
//--------------------------------------------------------------------------------------------------------------------------
static INT32 lensFF1_Focus_Init(void)
{
    return ERR_OK;
}
//um in unit.
static UINT32 lensFF1_Focus_GetLength(UINT32 ZoomSection)
{
    // Focal Length = 2.57mm
    //return 2570;
    return 3850;
    //return 0;
}

static INT32 lensFF1_Focus_GetPosition(void)
{
    return 0;
}

static UINT32 lensFF1_Focus_GetDistTable(UINT32 section)
{
    return 0;
}

static INT32 lensFF1_Focus_GetDefultTable(UINT32 section, UINT32 FocusDistance)
{
    return 0;
}

#if 0
UINT32 lensFF1_Focus_GetDist(void)
{
   return 0;
}
#endif

static INT32 lensFF1_Focus_GetTable(UINT32 section, UINT32 FocusDistance)
{
    return 0;
}

static INT32 lensFF1_Focus_GetRange(FOCUS_RANGE range)
{
    return 0;
}

static void lensFF1_Focus_DoEXC(BOOL on, UINT32 dir)
{

}

static void lensFF1_Focus_Go2(INT32 position)
{

}

static INT32 lensFF1_Focus_Reset(FOCUS_RETRACT_POSITION position)
{
    return ERR_OK;
}


//--------------------------------------------------------------------------------------------------------------------------
//Part of Aperture
//--------------------------------------------------------------------------------------------------------------------------
static void lensFF1_Aperture_Init(void)
{
    FF1_Para.AperturePos = IRIS_POS_BIG;
    lensFF1_Aperture_Go2Pos(IRIS_POS_BIG);
}

static IRIS_POS lensFF1_Aperture_GetPosition(void)
{
    return FF1_Para.AperturePos;
}

static UINT32 lensFF1_Aperture_GetFNO(UINT32 ZoomSection,UINT32 IrisPos)
{
    //return Aperture_Zoom_Tbl[ZoomSection][IrisPos];
    return 0;
}

static void lensFF1_Aperture_Go2Pos(IRIS_POS position)
{
    if ((position >= IRIS_POS_MAX))
    {
        DBG_ERR("%s, parameters error, (%d)\r\n",__func__,position);
    }
    else if(FF1_Para.AperturePos != position)
    {
        g_FF1_LensCtrlTskAPIObj.lensctrltsk_aperture_setstate(MOTOR_APERTURE_NORMAL, position);//LensCtrl_Aperture_SetState(MOTOR_APERTURE_NORMAL,position);

        FF1_Para.AperturePos = position;
    }
}

//--------------------------------------------------------------------------------------------------------------------------
//Part of Shutter
//--------------------------------------------------------------------------------------------------------------------------
static void lensFF1_shutter_setState(MOTOR_SHUTTER_ACT state, UINT32 param1)
{
    g_FF1_LensCtrlTskAPIObj.lensctrltsk_shutter_setstate(state, param1);//LensCtrl_Shutter_SetState(state, param1);
}

//--------------------------------------------------------------------------------------------------------------------------
//Part of misc.
//--------------------------------------------------------------------------------------------------------------------------
static INT32 lensFF1_LensRetract(void)
{
    return lensFF1_Zoom_Retract();
}

static INT32 lensFF1_LensReset(void)
{
    return ERR_OK;
}

static void lensFF1_MakeAFTable(void)
{

}

static INT32 lensFF1_SetCalData(INT16 *pCalTbl, UINT32 uiTblSize)
{
    return ERR_OK;
}

static void  lensFF1_Focus_AfterZoomMoving(UINT32 uiCmd)
{
}

static INT32 lensFF1_LensPowerOff(void)
{
    return ERR_OK;
}
