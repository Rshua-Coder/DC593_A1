
#include "SysKer.h"
#include "Timer.h"
#include "consts.h"
#include "flag.h"
#include "Interrupt.h"
#include "stdio.h"
#include "gpio.h"
#include "DxLens.h"
#include "LensDrv_Common.h"
#include "LensDrv_DCJ36.h"
#include "Utility.h"
#include "ADC.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          LENSDRVDCJ36
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define LD_DELAYMS(x)   Delay_DelayMs(x)
#define LD_DELAYUS(x)   Delay_DelayUs(x)

static INT32 ZoomSection_Pulse[(ZOOM_SECTION_TELE+1)]=
{
    ZOOM_POS_GARAGE,        // -266
    ZOOM_POS_WIDE,          // 71
    246,                    // 246
    313,                    // 313
    353,                    // 353
    387,                    // 387
    421,                    // 421
    448,                    // 448
    508,                    // 508
    562,                    // 562
    622,                    // 622
    ZOOM_POS_TELE           // 743
};

//For AF use, need to confirm Macro range
#define NW FOCUS_POS_NORMAL_WIDE
static INT32 Focus_Tbl_Default[ZOOM_SECTION_TELE][FOCUS_DISTANCE]={
   //INF      3       2     1.2       1     0.8      0.5     0.3     0.2    0.15    0.10    0.05(m)
{   NW,     367,    369,    373,    375,    378,     386,    400,    418,    434,    466,    548},   //ZP1
{   395,    398,    401,    406,    409,    413,     425,    445,    470,    493,    537,    651},   //ZP2
{   471,    476,    480,    487,    491,    497,     514,    542,    575,    608,    668,    724},   //ZP3
{   529,    536,    541,    551,    556,    563,     585,    620,    664,    697,    771,    771},   //ZP4
{   508,    519,    526,    539,    546,    555,     585,    632,    690,    755,    755,    755},   //ZP5
{   482,    497,    506,    523,    531,    544,     580,    641,    735,    735,    735,    735},   //ZP6
{   469,    488,    499,    519,    529,    544,     588,    659,    725,    725,    725,    725},   //ZP7
{   442,    444,    446,    519,    533,    555,     618,    704,    704,    704,    704,    704},   //ZP8
{   404,    449,    470,    511,    531,    560,     642,    689,    689,    689,    689,    689},   //ZP9
{   360,    425,    453,    506,    532,    569,     673,    680,    680,    680,    680,    680},   //ZP10
{   325,    405,    441,    508,    540,    586,     680,    680,    680,    680,    680,    680},   //ZP11
};

static INT32 Focus_Tbl_Search_Range[ZOOM_SECTION_TELE][2] = {
    // Low bound, Up bound
#if 1
    { 90,         638 }, //ZP1
    { 127,        663 }, //ZP2
    { 218,        724 }, //ZP3
    { 287,        771 }, //ZP4
    { 261,        755 }, //ZP5
    { 229,        735 }, //ZP6
    { 213,        725 }, //ZP7
    { 180,        704 }, //ZP8
    { 133,        675 }, //ZP9
    { 78,         642 }, //ZP10
    { 132,        615 }, //ZP11
#else
    { (90 - 45)  >> 1,        (638 + 45 ) >> 1}, //ZP1
    { (127 - 45) >> 1,        (663 + 45 ) >> 1}, //ZP2
    { (218 - 45) >> 1,        (724 + 45 ) >> 1}, //ZP3
    { (287 - 45) >> 1,        (771 + 45 ) >> 1}, //ZP4
    { (261 - 45) >> 1,        (755 + 45 ) >> 1}, //ZP5
    { (229 - 45) >> 1,        (735 + 45 ) >> 1}, //ZP6
    { (213 - 45) >> 1,        (725 + 45 ) >> 1}, //ZP7
    { (180 - 45) >> 1,        (704 + 45 ) >> 1}, //ZP8
    { (133 - 45) >> 1,        (675 + 45 ) >> 1}, //ZP9
    { (78 - 45)  >> 1,        (642 + 45 ) >> 1}, //ZP10
    { (132 - 45) >> 1,        (615 + 45 ) >> 1}  //ZP11
#endif
};

static INT16 *g_pFocus_CalTbl = NULL;
static INT16 g_tFocus_CalTbl[ZOOM_SECTION_TELE] = { 0 };
static INT32 Focus_Tbl[ZOOM_SECTION_TELE][FOCUS_DISTANCE];
static UINT32 Focus_Dist_Tbl[FOCUS_DISTANCE] = {9999, 300, 200, 120, 100, 80, 50, 30, 20, 15, 10, 5};
static UINT32 Aperture_Zoom_Tbl[(ZOOM_SECTION_TELE+1)][2] = {
   {0,0},
   {35,98},
   {37,103},
   {39,109},
   {41,116},
   {44,122},
   {45,127},
   {47,131},
   {48,134},
   {49,137},
   {50,140},
   {51,142},
};

#if 0 // For GPIOs control.
static UINT32 Aperture_Pos_Tbl[IRIS_TOTAL_SEGMENT] = {5, 7, 10, 13, 16, 19};
#else // For uStepping control.
static UINT32 Aperture_Pos_Tbl[IRIS_TOTAL_SEGMENT] = {6, 8, 11, 14, 17, 20};
#endif

//um in unit.
static UINT32 FocusLength[(ZOOM_SECTION_TELE+1)] = {0, 4300, 5400, 6500, 7500, 8600, 9700, 10800, 12900, 15100, 17200, 19800};
static LENSDRV_DCJ36_PARA DCJ36_Para={0};

//Prototype declaration
static void        lensDCJ36_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab);
static UINT32      lensDCJ36_getSignal(LD_SIGNAL signal);
static void        lensDCJ36_setPIInt(BOOL enable);
static void        lensDCJ36_clearPIInt(void);
static BOOL        lensDCJ36_checkPIInt(void);
static void        lensDCJ36_Zoom_InitPart1(void);
static INT32       lensDCJ36_Zoom_InitPart2(void);
static void        lensDCJ36_Zoom_SetBoundaryCheck(BOOL en);
static void        lensDCJ36_Zoom_SetSpeed(ZOOM_SPEED_CATEGORY category);
static UINT32      lensDCJ36_Zoom_GetMinSection(void);
static UINT32      lensDCJ36_Zoom_GetMaxSection(void);
static UINT32      lensDCJ36_Zoom_GetSection(void);
static UINT32      lensDCJ36_Zoom_GetCurrentSection(void);
static UINT32      lensDCJ36_Zoom_GetStep(void);
static UINT32      lensDCJ36_Zoom_GetSectionStep(void);
static void        lensDCJ36_Zoom_GoSection(UINT32 section);
static void        lensDCJ36_Zoom_GoSection2(UINT32 section);
static void        lensDCJ36_Zoom_GoStep(INT32 position);
static INT32       lensDCJ36_Zoom_Press(UINT32 zoom);
static UINT32      lensDCJ36_Zoom_Release(void);
static INT32       lensDCJ36_Zoom_Retract(void);
static INT32       lensDCJ36_Focus_Init(void);
static UINT32      lensDCJ36_Focus_GetLength(UINT32 ZoomSection);
static INT32       lensDCJ36_Focus_GetPosition(void);
static UINT32      lensDCJ36_Focus_GetDistTable(UINT32 section);
static INT32       lensDCJ36_Focus_GetDefultTable(UINT32 section, UINT32 FocusDistance);
static INT32       lensDCJ36_Focus_GetTable(UINT32 section, UINT32 FocusDistance);
static INT32       lensDCJ36_Focus_GetRange(FOCUS_RANGE range);
static void        lensDCJ36_Focus_DoEXC(BOOL on,UINT32 dir);
static void        lensDCJ36_Focus_Go2(INT32 Position);
static INT32       lensDCJ36_Focus_Reset(FOCUS_RETRACT_POSITION position);
static void        lensDCJ36_Aperture_Init(void);
static IRIS_POS    lensDCJ36_Aperture_GetPosition(void);
static UINT32      lensDCJ36_Aperture_GetFNO(UINT32 ZoomSection,UINT32 IrisPos);
static void        lensDCJ36_Aperture_Go2Pos(IRIS_POS position);
static void        lensDCJ36_shutter_setState(MOTOR_SHUTTER_ACT state, UINT32 param1);
static INT32       lensDCJ36_LensRetract(void);
static INT32       lensDCJ36_LensReset(void);
static INT32       lensDCJ36_LensPowerOff(void);
static void        lensDCJ36_MakeAFTable(void);
static INT32       lensDCJ36_SetCalData(INT16 *pCalTbl, UINT32 uiTblSize);
static void        lensDCJ36_Zoom_ISR(UINT32 uiEvent);
//static INT32     lensDCJ36_Zoom_RescueInit(void);
//static INT32     lensDCJ36_Zoom_Home(void);
static INT32       lensDCJ36_Zoom_ForceToHome(void);
static UINT32      lensDCJ36_Focus_RescueInit(void);
static void        lensDCJ36_Focus_AfterZoomMoving(UINT32 uiCmd);
static void        lensDCJ36_Focus_RunStep(UINT32 dir,UINT32 step);
static INT32       lensDCJ36_abs(INT32 i);
INT32              lensDCJ36_Focus_getBL(void);

static LENSCTRL_TASKAPIOBJ g_DCJ36_LensCtrlTskAPIObj = {NULL};

static LENS_TAB g_pDCJ36Obj =
{
    //Lens
    lensDCJ36_init,
    lensDCJ36_getSignal,
    //Zoom
    lensDCJ36_Zoom_InitPart1,
    lensDCJ36_Zoom_InitPart2,
    lensDCJ36_Zoom_SetBoundaryCheck,
    lensDCJ36_Zoom_SetSpeed,
    lensDCJ36_Zoom_GetMinSection,
    lensDCJ36_Zoom_GetMaxSection,
    lensDCJ36_Zoom_GetSection,
    lensDCJ36_Zoom_GetStep,
    lensDCJ36_Zoom_GetSectionStep,
    lensDCJ36_Zoom_GoSection,
    lensDCJ36_Zoom_GoSection2,
    lensDCJ36_Zoom_GoStep,
    lensDCJ36_Zoom_Press,
    lensDCJ36_Zoom_Release,
    lensDCJ36_Zoom_Retract,
    //Focus
    lensDCJ36_Focus_Init,
    lensDCJ36_Focus_GetLength,
    lensDCJ36_Focus_GetPosition,
    lensDCJ36_Focus_GetDistTable,
    lensDCJ36_Focus_GetDefultTable,
    lensDCJ36_Focus_GetTable,
    lensDCJ36_Focus_GetRange,
    lensDCJ36_Focus_DoEXC,
    lensDCJ36_Focus_Go2,
    lensDCJ36_Focus_Reset,
    //Iris
    lensDCJ36_Aperture_Init,
    lensDCJ36_Aperture_GetPosition,
    lensDCJ36_Aperture_GetFNO,
    lensDCJ36_Aperture_Go2Pos,
    //Shutter
    lensDCJ36_shutter_setState,
    //Misc
    lensDCJ36_LensRetract,
    lensDCJ36_LensReset,
    lensDCJ36_MakeAFTable,
    lensDCJ36_SetCalData,
    lensDCJ36_Focus_AfterZoomMoving,
    lensDCJ36_LensPowerOff,
    NULL,
    &g_DCJ36_LensCtrlTskAPIObj // PLENSCTRL_TASKAPIOBJ
};


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for lens control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensDCJ36_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab)
{
    DBG_MSG("%s\r\n", __func__);
    g_pDCJ36Obj.pMotor = pMotorTab;

    DCJ36_Para.ZPI    =  pLensDev->pGPIOArray[LENS_IO_ZOOM_PI];
    DCJ36_Para.FPI    =  pLensDev->pGPIOArray[LENS_IO_FOCUS_PI];
    DCJ36_Para.ZPIINT =  pLensDev->pGPIOArray[LENS_IO_ZOOM_PIINT];

    DCJ36_Para.ZPR    =  pLensDev->pGPIOArray[LENS_IO_ZOOM_PR];
    DCJ36_Para.ZoomHP =  ZOOM_POS_HOME_PR;
    DCJ36_Para.ZommSpdSlwDownDly = 0;
    //#NT#Patch for precision improve
    #if (PRECISION_UPGRADE)
    DCJ36_Para.IntTypePol = GPIO_INTPOL_POSHIGH;
    #endif
    //Set high spped ad default speed
    lensDCJ36_Zoom_SetSpeed(ZOOM_SPEED_HIGH);

    /* Close ND lens */
    gpio_setPin(pLensDev->pGPIOArray[LENS_IO_MOT_EXT2]);
    gpio_clearPin(pLensDev->pGPIOArray[LENS_IO_MOT_EXT1]);
    Delay_DelayMs(15);
    gpio_clearPin(pLensDev->pGPIOArray[LENS_IO_MOT_EXT1]);
    gpio_clearPin(pLensDev->pGPIOArray[LENS_IO_MOT_EXT2]);
}

/**
    Get FF lens object

    @return FF lens object pointer

*/
PLENS_TAB lens_getObject(void)
{
    DBG_MSG("%s\r\n", __func__);

    return &g_pDCJ36Obj;
}

static UINT32 lensDCJ36_getSignal(LD_SIGNAL signal)
{
    switch (signal)
    {
    case LD_SIGNAL_ZPI:
        return gpio_getPin(DCJ36_Para.ZPI);

    case LD_SIGNAL_ZPR:
        return gpio_getPin(DCJ36_Para.ZPR);

    case LD_SIGNAL_FPI:
        if (gpio_getPin(DCJ36_Para.FPI))
        {
            return  FOCUSPI_OUTSDIE_LEVEL;
        }
        else
        {
            return  FOCUSPI_INSDIE_LEVEL;
        }

    default:
        DBG_MSG("%s, parameter error, (%d)\r\n",__func__,signal);
        return 0;
    }
}

static void lensDCJ36_setPIInt(BOOL enable)
{
    if(enable)
    {
        //#Clear int before set
        lensDCJ36_clearPIInt();
        gpio_enableInt(DCJ36_Para.ZPIINT);
    }
    else
    {
        gpio_disableInt(DCJ36_Para.ZPIINT);
    }
}

static void lensDCJ36_clearPIInt(void)
{
    gpio_clearIntStatus(DCJ36_Para.ZPIINT);
}

static BOOL lensDCJ36_checkPIInt(void)
{
    #if 0
    return gpio_getIntStatus(DCJ36_Para.ZPIINT);
    #else
    return lensDCJ36_getSignal(LD_SIGNAL_ZPI);
    #endif
}
//--------------------------------------------------------------------------------------------------------------------------
//Part of Zoom
//--------------------------------------------------------------------------------------------------------------------------
static void lensDCJ36_Zoom_InitFlag(void)
{
    #if 0 // Must be enabled in real chip environment.
    DCJ36_Para.ZoomInitFlag = LensCtrl2_ReadLensStatus();
    #else // Assume zoom is always inside at startup for the time being.
    DCJ36_Para.ZoomInitFlag = ZOOM_FLAG_INSIDE;
    #endif

    DBG_MSG("READ NAND~~%d\r\n",DCJ36_Para.ZoomInitFlag);

}

static void lensDCJ36_Zoom_InitParameters(void)
{
    DCJ36_Para.bZoomBoundaryAutoCheck=TRUE;
    DCJ36_Para.ZoomIsr = ZOOM_Isr_Open;
    DCJ36_Para.ZoomStatus = ZOOM_STATUS_NULL;

    gpio_open();
#if ZPI_RISING_EDGE_TRIGGER
    gpio_setIntTypePol(DCJ36_Para.ZPIINT, GPIO_INTTYPE_EDGE, GPIO_INTPOL_POSHIGH);
#else // Use falling edge detection temporarily!!! Must use rising edge on EVB!!!
    gpio_setIntTypePol(DCJ36_Para.ZPIINT, GPIO_INTTYPE_EDGE, GPIO_INTPOL_NEGLOW);
#endif
    gpio_setIntIsr(DCJ36_Para.ZPIINT, lensDCJ36_Zoom_ISR);
    gpio_enableInt(DCJ36_Para.ZPIINT);
    //#NT#Patch for precision improve
    #if (PRECISION_UPGRADE)
    DCJ36_Para.IntTypePol = GPIO_INTPOL_POSHIGH;
    #endif
}

static void lensDCJ36_Zoom_OpenISR(void)
{
    //In ZOOM_STATUS_ZB_RETRACT status, does not need to count step
    if(DCJ36_Para.ZoomDir == DIRECTION_FWD)
    {
        DCJ36_Para.ZoomStep++;
    }
    else
    {
        DCJ36_Para.ZoomStep--;
    }

    if (DCJ36_Para.ZoomStatus != ZOOM_STATUS_NULL)
    {
        switch(DCJ36_Para.ZoomStatus)
        {
        case ZOOM_STATUS_ZC_2_ZB:
            if(lensDCJ36_getSignal(LD_SIGNAL_ZPR) == ZOOMPR_LOW_LEVEL)
            {
                DCJ36_Para.ZoomStatus = ZOOM_STATUS_ZB_RETRACT;
                DBG_MSG("ZB\r\n");
            }
        break;
        case ZOOM_STATUS_ZB_RETRACT:
            if(lensDCJ36_getSignal(LD_SIGNAL_ZPR) == ZOOMPR_HI_LEVEL)  //Beyond HP
            {
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake(); //LensCtrl_ZoomBrake();
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomforward(); //LensCtrl_ZoomForward();
                DCJ36_Para.ZoomDir = DIRECTION_FWD;
                DCJ36_Para.ZoomStatus = ZOOM_STATUS_FIND_HP;
            }
        break;
        case ZOOM_STATUS_FIND_HP:
            if(lensDCJ36_getSignal(LD_SIGNAL_ZPR) == ZOOMPR_LOW_LEVEL)
            {
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk2_writelensstatus(ZOOM_FLAG_OUTSIDE);//LensCtrl2_WriteLensStatus(ZOOM_FLAG_OUTSIDE);
                //#NT#2010/12/17#Photon Lin -begin
                //#Modify flow of calibration
                DCJ36_Para.ZoomStep = DCJ36_Para.ZoomHP;
                //#NT#2010/12/17#Photon Lin -end
                DCJ36_Para.ZoomStatus = ZOOM_STATUS_FW_BRAKE;
            }
            #if 0
            else if(DCJ36_Para.ZoomStep == ZOOM_COUNT_12)
            {
                LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_100);   //PWM6
            }
            #endif
        break;
        case ZOOM_STATUS_FW_BRAKE:
            if(DCJ36_Para.ZoomStep >= (INT32)(ZoomSection_Pulse[1]-ZOOM_COUNT_10))   //Set Pre-off-count as 10
            {
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
                DCJ36_Para.ZoomStatus = ZOOM_STATUS_IDLE;
                DCJ36_Para.ZoomSection = ZOOM_SECTION_WIDE;
                DCJ36_Para.ZoomLastDir = DIRECTION_FWD;
                DCJ36_Para.ZoomIsr = ZOOM_Isr_Normal;
                //TM_End(LENS_ZOOM_INIT);
            }
            #if 0
            else if(DCJ36_Para.ZoomStep>=(INT32)(ZoomSection_Pulse[1]-ZOOM_COUNT_18))
            {
                LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_22);   //PWM8

            }
            else if(DCJ36_Para.ZoomStep>=(INT32)(ZoomSection_Pulse[1]-ZOOM_COUNT_30))
            {
                LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_63);   //PWM7
            }
            #endif
        break;

        default:
            DBG_ERR("Status error in lensDCJ36_Zoom_OpenISR() (%d)\r\n",DCJ36_Para.ZoomStatus);
        break;
        }

        DCJ36_Para.StepCnt = 0;
        DCJ36_Para.StepMove = 0xFFFFFFFF;
    }

}

static void lensDCJ36_Zoom_NormalISR(void)
{
    UINT32 uiBrakeStep = ZOOM_COUNT_12;

    if(g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomgetspeed() != ZOOM_SPEED_100)
        uiBrakeStep = ZOOM_COUNT_1;

    if(DCJ36_Para.ZoomDir == DIRECTION_FWD)
    {
        DCJ36_Para.ZoomStep++;
        if(DCJ36_Para.bZoomBoundaryAutoCheck)
        {
            if(DCJ36_Para.ZoomStep >= (INT32)( ZoomSection_Pulse[ZOOM_SECTION_TELE]-uiBrakeStep))
            {
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
                DCJ36_Para.ZoomStatus = ZOOM_STATUS_IDLE;
            }
        }
    }
    else
    {
        DCJ36_Para.ZoomStep--;
        if(DCJ36_Para.bZoomBoundaryAutoCheck)
        {
            if(DCJ36_Para.ZoomStep <= (INT32)( ZoomSection_Pulse[ZOOM_SECTION_WIDE]+uiBrakeStep))
            {
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
                DCJ36_Para.ZoomStatus = ZOOM_STATUS_IDLE;
            }
        }
    }
    //OpticalZoom Callback to Draw
    if(DCJ36_Para.bDoOptZoomDraw)
    {
        #if 0
        if(lensDCJ36_Zoom_GetSection() != DCJ36_Para.ZoomSection)
        #else
        if(lensDCJ36_Zoom_GetCurrentSection() != DCJ36_Para.ZoomSection)
        #endif
        {
            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomchange();//LensCtrl_ZoomChange();

            /* Trigger IQS DCE function if system has free time. */
            #if ZOOM_ENABLE_DCE_NOTIFY
            if(LensCtrlBG_CheckIdle())
                LensCtrlBG_Iqs_SetFlowFunc();
            #endif

            if(DCJ36_Para.bFocusOnceZoomChanged)
                g_DCJ36_LensCtrlTskAPIObj.lensctrltskBG_focus_afterzoomchanged();//LensCtrlBG_Focus_AfterZoomChanged();
        }
    }

    #if 0
    DCJ36_Para.ZoomSection = lensDCJ36_Zoom_GetSection();
    #else
    DCJ36_Para.ZoomSection = lensDCJ36_Zoom_GetCurrentSection();
    #endif

    DCJ36_Para.StepCnt++;
    if((DCJ36_Para.StepCnt+uiBrakeStep) >= (DCJ36_Para.StepMove/*+DCJ36_Para.ZommSpdSlwDownDly*/))
    {
        //DBG_MSG("^G cnt = %d, move = %d \r\n",DCJ36_Para.StepCnt,DCJ36_Para.StepMove);
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
        DCJ36_Para.ZoomStatus = ZOOM_STATUS_IDLE;
    }
    #if 0
    else if((DCJ36_Para.StepCnt+ZOOM_COUNT_6+ZOOM_COMPENSATION_8) == (DCJ36_Para.StepMove/*+DCJ36_Para.ZommSpdSlwDownDly*/))
    {
        //#NT#Add API for zoom duty change
        //Set high spped ad default speed
        LensCtrl_ZoomChangeSpeed(DCJ36_Para.ZoomInDuty2);
        //#NT#2011/03/18#Photon Lin -end
    }
    #endif
}

static void lensDCJ36_Zoom_CloseISR(void)
{
    static UINT32 zoomstep_GO = 0;

    if(DCJ36_Para.ZoomDir == DIRECTION_FWD)
    {
        DCJ36_Para.ZoomStep++;
    }
    else
    {
        DCJ36_Para.ZoomStep--;
    }

    DCJ36_Para.StepCnt++;

    if (DCJ36_Para.ZoomStatus != ZOOM_STATUS_NULL)
    {

        switch(DCJ36_Para.ZoomStatus)
        {
        case ZOOM_STATUS_ZC_2_ZB:
            if(lensDCJ36_getSignal(LD_SIGNAL_ZPR) == ZOOMPR_LOW_LEVEL)
            {
                DCJ36_Para.ZoomStatus = ZOOM_STATUS_ZB_RETRACT;
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomreverse();//LensCtrl_ZoomReverse();
            }

        break;
        case ZOOM_STATUS_ZB_RETRACT:
            if(lensDCJ36_getSignal(LD_SIGNAL_ZPR) == ZOOMPR_HI_LEVEL)
            {
                //#NT#2010/12/17#Photon Lin -begin
                //#Modify flow of calibration
                DCJ36_Para.ZoomStep = DCJ36_Para.ZoomHP;
                //#NT#2010/12/17#Photon Lin -end

                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk2_writelensstatus(ZOOM_FLAG_INSIDE);//LensCtrl2_WriteLensStatus(ZOOM_FLAG_INSIDE);

                DCJ36_Para.ZoomStatus = ZOOM_STATUS_BW_BRAKE;
                DCJ36_Para.StepCnt=0;
            }
        break;
        case ZOOM_STATUS_FIND_HP:
            if(lensDCJ36_getSignal(LD_SIGNAL_ZPR) == ZOOMPR_LOW_LEVEL)
            {
                //Not define in Spec.---------------------------
//                LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_30);
                zoomstep_GO++;
                if(zoomstep_GO>=(ZOOM_BWD_INTERVAL-4))
                {
//                    LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_10);
                }
                //----------------------------------------------
                if(zoomstep_GO>=ZOOM_BWD_INTERVAL)
                {
                    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
//                    LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_90);   //PWM9
                    DCJ36_Para.ZoomStatus = ZOOM_STATUS_ZB_RETRACT;
                    DCJ36_Para.ZoomDir = DIRECTION_BWD;
                    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomreverse();//LensCtrl_ZoomReverse();
                }
            }
        break;
        case ZOOM_STATUS_BW_BRAKE:
            if(DCJ36_Para.ZoomStep<=(INT32)(ZoomSection_Pulse[0]+ZOOM_COUNT_12))
            {
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
                DCJ36_Para.ZoomStatus = ZOOM_STATUS_NULL;
                DBG_MSG("Brake in Z CloseISR\r\n");
            }
            #if 0
            else if(DCJ36_Para.ZoomStep==(ZoomSection_Pulse[0]+ZOOM_COUNT_10))
            {
                LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_25);    //PWM10
            }
            #endif
        break;

        default:
            DBG_ERR("%s, status error, (%d)\r\n",__func__,DCJ36_Para.ZoomStatus);
        break;
        }
    }

}

static void lensDCJ36_Zoom_AbnormalCloseISR(void)
{
    if(DCJ36_Para.ZoomDir == DIRECTION_FWD)
        DCJ36_Para.ZoomStep++;
    else
        DCJ36_Para.ZoomStep--;
}


static void lensDCJ36_Zoom_ISR(UINT32 uiEvent)
{
    #if ZPI_RISING_EDGE_TRIGGER // Use falling edge detection temporarily!!! Must use rising edge on EVB!!!
    if(lensDCJ36_checkPIInt())
    #else
    if(!lensDCJ36_checkPIInt())
    #endif
    {
        //lensDCJ36_clearPIInt(); // no need on 96650 platform.
        //#NT#2011/03/14#Photon Lin -begin
        //#NT#Patch for precision improve
        #if (PRECISION_UPGRADE)
        if (DCJ36_Para.IntTypePol == GPIO_INTPOL_POSHIGH)
        {
            DCJ36_Para.IntTypePol = GPIO_INTPOL_NEGLOW;
        }
        else
        {
            DCJ36_Para.IntTypePol = GPIO_INTPOL_POSHIGH;
        }
        gpio_setIntTypePol(DCJ36_Para.ZPIINT, GPIO_INTTYPE_EDGE, DCJ36_Para.IntTypePol);
        #endif
        //#NT#2011/03/14#Photon Lin -end
        if (DCJ36_Para.ZoomIsr == ZOOM_Isr_Open)
            lensDCJ36_Zoom_OpenISR();
        else if(DCJ36_Para.ZoomIsr == ZOOM_Isr_Normal)
            lensDCJ36_Zoom_NormalISR();
        else if(DCJ36_Para.ZoomIsr == ZOOM_Isr_Close)
            lensDCJ36_Zoom_CloseISR();
        else if(DCJ36_Para.ZoomIsr == ZOOM_Isr_AbnormalClose)
            lensDCJ36_Zoom_AbnormalCloseISR();
    }
}

static void lensDCJ36_Zoom_InitPart1(void)
{
    UINT32 ZPRvalue;

    lensDCJ36_Zoom_InitParameters();
    lensDCJ36_Zoom_InitFlag();

    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,ON);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,ON);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,ON);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,ON);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);
    //#NT#2010/12/02#Photon Lin -begin
    //#Refine flow
    lensDCJ36_setPIInt(ON);
    LD_DELAYMS(2);
    //#NT#2010/12/02#Photon Lin -end

    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomchangespeed(ZOOM_SPEED_100);//LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_100);

    ZPRvalue = lensDCJ36_getSignal(LD_SIGNAL_ZPR);

    //IN ZONE A
    if(ZPRvalue == ZOOMPR_HI_LEVEL && DCJ36_Para.ZoomInitFlag == ZOOM_FLAG_INSIDE)
    {
        DCJ36_Para.ZoomStep = 0;
        DBG_MSG("ZONE_A\r\n");
//        LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_90);   //PWM5
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomforward();//LensCtrl_ZoomForward();
        DCJ36_Para.ZoomDir = DIRECTION_FWD;
        DCJ36_Para.ZoomStatus = ZOOM_STATUS_FIND_HP;
    }
    //IN ZONE C
    else if(ZPRvalue == ZOOMPR_HI_LEVEL && DCJ36_Para.ZoomInitFlag ==ZOOM_FLAG_OUTSIDE)
    {
        DBG_MSG("[Z_OC", 0);
        lensDCJ36_Focus_Reset(POS_GARAGE);
        DBG_MSG("ZONE_C\r\n");
        #if 0
        LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
        LensCtrl_SetSignal(MD_SIGNAL_ZPI,ON);
        LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);
        lensDCJ36_setPIInt(ON);
        Delay_DelayMs(2);
        #endif
//        LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_100);//ZOOM_SPEED_A_PWM
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomreverse();//LensCtrl_ZoomReverse();
        DCJ36_Para.ZoomDir = DIRECTION_BWD;
        DCJ36_Para.ZoomStatus = ZOOM_STATUS_ZC_2_ZB;

    }
    //IN ZONE B
    else if(ZPRvalue == ZOOMPR_LOW_LEVEL)
    {
        if(DCJ36_Para.ZoomInitFlag != ZOOM_FLAG_OUTSIDE)
        {
            //write flag
            DCJ36_Para.ZoomInitFlag = ZOOM_FLAG_OUTSIDE;
            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk2_writelensstatus(DCJ36_Para.ZoomInitFlag);//LensCtrl2_WriteLensStatus(DCJ36_Para.ZoomInitFlag);
        }

        DCJ36_Para.ZoomStep = 0xFFFF;    //Assign a huge value

        lensDCJ36_Focus_Reset(POS_GARAGE);
        DBG_MSG("ZONE_B\r\n");
        #if 0
        LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
        LensCtrl_SetSignal(MD_SIGNAL_ZPI,ON);
        lensDCJ36_setPIInt(ON);
        #endif
        //#NT#2010/12/02#Photon Lin -begin
        //#Refine flow
        LD_DELAYMS(2);
        //#NT#2010/12/02#Photon Lin -end
        //#NT#2010/12/10#Photon Lin -begin
        //#Refine to match reset control flow
//        LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_100);   //PWM 6
        //#NT#2010/12/10#Photon Lin -end
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomreverse();//LensCtrl_ZoomReverse();
        DCJ36_Para.ZoomDir = DIRECTION_BWD;
        DCJ36_Para.ZoomStatus = ZOOM_STATUS_ZB_RETRACT;
    }
    //NG
    else
    {
        DBG_ERR("ZOOM POS ERROR!!\r\n");
    }


}

static INT32 lensDCJ36_Zoom_InitPart2(void)
{
    UINT32 cnt = 0, Incnt = 0, Brake2Stable = 0;
    UINT32 ZoomInitTimeOut;
    INT32 iLastStep = 0x0FFFF;
    INT32 Prv_Zoomstep;

    ZoomInitTimeOut= 400 * 5;   // 2*5 seconds
    Prv_Zoomstep = DCJ36_Para.ZoomStep;

    DBG_ERR("#ZSts=0x%08lx\r\n", DCJ36_Para.ZoomStatus);

    while(DCJ36_Para.ZoomStatus != ZOOM_STATUS_IDLE)
    {
        //Reach the stopper
        if(Prv_Zoomstep == DCJ36_Para.ZoomStep && DCJ36_Para.ZoomDir == DIRECTION_BWD)
        {
            Incnt++;
        }

        Prv_Zoomstep = DCJ36_Para.ZoomStep;
        cnt++;
        LD_DELAYMS(5);

        if((cnt%100==0) /*&& (LensCtrl_ZoomGetSpeed()<=ZOOM_SPEED_10)*/)
        {
            if(DCJ36_Para.ZoomStep==iLastStep)
            {
                DBG_ERR("DCJ36_Para.ZoomStep = %d, cnt = %d\r\n",DCJ36_Para.ZoomStep,cnt);
                /* Zoom is in outside of zone C. Get back to HP. */
                if(DCJ36_Para.ZoomStatus == ZOOM_STATUS_FIND_HP)
                {
                    // Brake before focus started.
                    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
                    LD_DELAYMS(20);

                    lensDCJ36_Focus_Reset(POS_GARAGE);
                    LD_DELAYMS(10);

                    DBG_MSG("*ZONE_C\r\n");
                    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomreverse();//LensCtrl_ZoomReverse();
                    DCJ36_Para.ZoomDir = DIRECTION_BWD;
                    DCJ36_Para.ZoomStatus = ZOOM_STATUS_ZC_2_ZB;

                    cnt = 0;

                    continue;
                }
                else
                    cnt = 0xFFFF;

            }
            iLastStep = DCJ36_Para.ZoomStep;
        }
        if((cnt+40)>ZoomInitTimeOut)
        {
//            LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_100);
        }

        if(cnt>ZoomInitTimeOut || Incnt>60)
        {
            DBG_ERR("Zoom init timeout return %d, %d\r\n",cnt,Incnt);
            if(g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomgetspeed()<=ZOOM_SPEED_10)
            {
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
                DCJ36_Para.ZoomStatus = ZOOM_STATUS_IDLE;
                DCJ36_Para.ZoomSection = ZOOM_SECTION_WIDE;
                DCJ36_Para.ZoomLastDir = DIRECTION_FWD;
                DCJ36_Para.ZoomIsr = ZOOM_Isr_Normal;
            }
            else
            {
                //Stop Zoom
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
                //#NT#2010/12/02#Photon Lin -begin
                //#Refine flow
                lensDCJ36_setPIInt(OFF);
                LD_DELAYMS(2);
                //#NT#2010/12/02#Photon Lin -end
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
                //Move Focus Backward if outside
                lensDCJ36_Focus_DoEXC(ON, 0);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,ON);//LensCtrl_SetSignal(MD_SIGNAL_FPI,ON);
                LD_DELAYMS(5);

                //Retract focus
                if(lensDCJ36_getSignal(LD_SIGNAL_FPI) == FOCUSPI_OUTSDIE_LEVEL)
                {
                    BOOL bFindFocusPIErr=FALSE;
                    UINT32 uiCnt=0;
                    while(lensDCJ36_getSignal(LD_SIGNAL_FPI) == FOCUSPI_OUTSDIE_LEVEL)
                    {
                        lensDCJ36_Focus_RunStep(DIRECTION_BWD,1);
                        uiCnt++;
                        if(uiCnt>FOCUS_BWD_LIMIT_OUTSIDE_PI)
                        {
                            bFindFocusPIErr = TRUE;
                            break;
                        }
                    }
                    if(!bFindFocusPIErr)
                    {
                        lensDCJ36_Focus_RunStep(DIRECTION_BWD,(FOCUS_POS_HOME_PR-FOCUS_POS_GARAGE));
                    }
                }
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
                lensDCJ36_Focus_DoEXC(OFF, 0);
                //#NT#2010/12/02#Photon Lin -begin
                //#Refine flow
                lensDCJ36_setPIInt(OFF);
                LD_DELAYMS(2);
                //#NT#2010/12/02#Photon Lin -end
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

                return ERR_LENS_INIT_FAIL;

            }
        }
        Brake2Stable = 1;
    }

    if(Brake2Stable)
        LD_DELAYMS(20); //finetune for performance

    DBG_MSG("Zoom init_step = %d\r\n", DCJ36_Para.ZoomStep);

    DCJ36_Para.ZoomStatus= ZOOM_STATUS_IDLE;
    //#NT#2010/12/02#Photon Lin -begin
    //#Refine flow
    lensDCJ36_setPIInt(OFF);
    LD_DELAYMS(2);
    //#NT#2010/12/02#Photon Lin -end
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

    return ERR_OK;
}

static void lensDCJ36_Zoom_SetBoundaryCheck(BOOL en)
{
    DCJ36_Para.bZoomBoundaryAutoCheck = en;
}

//#NT#2011/03/18#Photon Lin -begin
//#NT#Add API for zoom duty change
static void lensDCJ36_Zoom_SetSpeed(ZOOM_SPEED_CATEGORY category)
{
    switch(category)
    {
        case ZOOM_SPEED_LOW:
            #if 0
            DCJ36_Para.ZoomInDuty1   = ZOOM_SPEED_48;
            DCJ36_Para.ZoomInDuty2   = ZOOM_SPEED_30;
            DCJ36_Para.ZoomOutDuty1  = ZOOM_SPEED_48;
            DCJ36_Para.ZoomOutDuty2  = ZOOM_SPEED_30;
            DCJ36_Para.ZoomOutDuty3  = ZOOM_SPEED_63;
            DCJ36_Para.ZoomOutDuty4  = ZOOM_SPEED_30;
            #else
            DCJ36_Para.ZoomInDuty1   = ZOOM_SPEED_63;
            #endif
            DBG_MSG(" %s, zoom speed low, %d \r\n",__func__,DCJ36_Para.ZoomInDuty1);

            break;
        case ZOOM_SPEED_MEDIUM:
            #if 0
            DCJ36_Para.ZoomInDuty1   = ZOOM_SPEED_48;
            DCJ36_Para.ZoomInDuty2   = ZOOM_SPEED_30;
            DCJ36_Para.ZoomOutDuty1  = ZOOM_SPEED_48;
            DCJ36_Para.ZoomOutDuty2  = ZOOM_SPEED_30;
            DCJ36_Para.ZoomOutDuty3  = ZOOM_SPEED_63;
            DCJ36_Para.ZoomOutDuty4  = ZOOM_SPEED_30;
            #else
            DCJ36_Para.ZoomInDuty1   = ZOOM_SPEED_100;
            #endif
            DBG_MSG(" %s, zoom speed medium, %d \r\n",__func__,DCJ36_Para.ZoomInDuty1);

            break;
        case ZOOM_SPEED_HIGH:
        default:
            #if 0
            DCJ36_Para.ZoomInDuty1   = ZOOM_SPEED_48;
            DCJ36_Para.ZoomInDuty2   = ZOOM_SPEED_30;
            DCJ36_Para.ZoomOutDuty1  = ZOOM_SPEED_48;
            DCJ36_Para.ZoomOutDuty2  = ZOOM_SPEED_30;
            DCJ36_Para.ZoomOutDuty3  = ZOOM_SPEED_63;
            DCJ36_Para.ZoomOutDuty4  = ZOOM_SPEED_30;
            #else
            DCJ36_Para.ZoomInDuty1   = ZOOM_SPEED_100;
            #endif
            DBG_MSG(" %s, zoom speed high, %d \r\n",__func__,DCJ36_Para.ZoomInDuty1);

            break;

    }
}
//#NT#2011/03/18#Photon Lin -end

static UINT32 lensDCJ36_Zoom_GetTargetSection(void)
{
    UINT32 section = ZOOM_SECTION_WIDE;

    if(DCJ36_Para.ZoomDir == DIRECTION_FWD)
    {
        for(section=ZOOM_SECTION_WIDE; section<=ZOOM_SECTION_TELE; section++)
        {
            if(DCJ36_Para.ZoomStep <= (INT32)(ZoomSection_Pulse[section]-ZOOM_COUNT_12))
            {
                if(lensDCJ36_Zoom_GetCurrentSection() == section)
                {
                    section++;
                    //DBG_MSG("Tsec:%d\r\n", section);
                }
                break;
            }
        }
    }
    else if(DCJ36_Para.ZoomDir == DIRECTION_BWD)
    {
        for(section=ZOOM_SECTION_TELE; section>=ZOOM_SECTION_WIDE; section--)
        {
            if(DCJ36_Para.ZoomStep >= (INT32)(ZoomSection_Pulse[section]+ZOOM_COUNT_12))
            {
                if(lensDCJ36_Zoom_GetCurrentSection() == section)
                {
                    section--;
                    //DBG_MSG("Tsec:%d\r\n", section);
                }
                break;
            }
        }
    }

    if(section>ZOOM_SECTION_TELE)
    {
        section = ZOOM_SECTION_TELE;
    }
    else if(section<ZOOM_SECTION_WIDE)
    {
        section = ZOOM_SECTION_WIDE;
    }

    return section;
}

static UINT32 lensDCJ36_Zoom_GetCurrentSection(void)
{
    UINT32 section;

    for(section=ZOOM_SECTION_WIDE; section<ZOOM_SECTION_TELE; section++)
    {
        if(DCJ36_Para.ZoomStep <= (INT32)((ZoomSection_Pulse[section]+ZoomSection_Pulse[section+1])/2))
            break;
    }
    return section;
}

static UINT32 lensDCJ36_Zoom_GetMinSection(void)
{
    return ZOOM_SECTION_WIDE;
}

static UINT32 lensDCJ36_Zoom_GetMaxSection(void)
{
    return ZOOM_SECTION_TELE;
}

static UINT32 lensDCJ36_Zoom_GetSection(void)
{
    UINT32 section;

    #if 0
    if(DCJ36_Para.ZoomStatus == ZOOM_STATUS_MOVE)
    {
        section = lensDCJ36_Zoom_GetTargetSection();
    }
    else
    #endif
    {
        section = lensDCJ36_Zoom_GetCurrentSection();
    }

    if(section>ZOOM_SECTION_TELE)
        section = ZOOM_SECTION_TELE;
    else if (section<ZOOM_SECTION_WIDE)
        section = ZOOM_SECTION_WIDE;

    return section;
}

static UINT32 lensDCJ36_Zoom_GetStep(void)
{
    return (DCJ36_Para.ZoomStep);
}

static UINT32 lensDCJ36_Zoom_GetSectionStep(void)
{
    return ZoomSection_Pulse[lensDCJ36_Zoom_GetSection()];
}

static INT32 lensDCJ36_Zoom_DoBL(UINT32 BLSteps, UINT32 ZoomStep)
{
    #define ZmDoBl2Ps_TimeOut (100*1000/200)
    UINT32 uiTimeCnt;

    //#NT#2012/07/16#Isiah Chang -begin
    //#NT#Check if zoom do stopped.
    #if 1
    INT32 Prv_Zoomstep, Incnt = 0;
    #endif
    //#NT#2012/07/16#Isiah Chang -end

    //#NT#2011/03/18#Photon Lin -begin
    //#NT#Add API for zoom duty change
//    LensCtrl_ZoomChangeSpeed(DCJ36_Para.ZoomOutDuty1);
    //#NT#2011/03/18#Photon Lin -end
    DCJ36_Para.ZommSpdSlwDownDly = 3;
    lensDCJ36_Zoom_GoStep(ZoomStep-BLSteps);
    LD_DELAYMS(5);
    uiTimeCnt=0;
    while(DCJ36_Para.ZoomStatus != ZOOM_STATUS_IDLE)
    {
        Delay_DelayUs(200);
        uiTimeCnt++;
        if(uiTimeCnt>ZmDoBl2Ps_TimeOut)
        {
//            LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_100);
        }
        if(uiTimeCnt>(ZmDoBl2Ps_TimeOut*10))
        {
            DBG_ERR("Do backlash time out\r\n");
            return ERR_OPERATION_FAIL;
        }
    }

    //#NT#2012/07/16#Isiah Chang -begin
    //#NT#Already brake in "lensDCJ36_Zoom_NormalISR()" before this moment.
    #if 0
    LensCtrl_ZoomBrake();
    LD_DELAYMS(30);
    #else
    //Check if zoom do stopped.
    Prv_Zoomstep = 0x0FFFFFFF;
    while(1)
    {
        if(Prv_Zoomstep == DCJ36_Para.ZoomStep)
            Incnt++;

        if(Incnt > 4)
            break;

        Prv_Zoomstep = DCJ36_Para.ZoomStep;

        LD_DELAYMS(5);
    }
    #endif
    //#NT#2012/07/16#Isiah Chang -end

    //#NT#2011/03/18#Photon Lin -begin
    //#NT#Add API for zoom duty change
//    LensCtrl_ZoomChangeSpeed(DCJ36_Para.ZoomOutDuty3);
    //#NT#2011/03/18#Photon Lin -end
    lensDCJ36_Zoom_GoStep(ZoomStep);
    LD_DELAYMS(5);
    uiTimeCnt=0;

    while(DCJ36_Para.ZoomStatus != ZOOM_STATUS_IDLE)
    {
        LD_DELAYUS(200);
        uiTimeCnt++;
        if(uiTimeCnt>ZmDoBl2Ps_TimeOut)
        {
//            LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_100);
        }
        if(uiTimeCnt>(ZmDoBl2Ps_TimeOut*10))
        {
            DBG_ERR("Do backlash time out\r\n");
            return ERR_OPERATION_FAIL;
        }
    }

    DCJ36_Para.ZommSpdSlwDownDly = 0;
    return ERR_OK;
}

static void lensDCJ36_Zoom_GoSection(UINT32 section)
{
    if(section>ZOOM_SECTION_TELE || section<ZOOM_SECTION_WIDE)
    {
        DBG_ERR("Incorrect section value (%d)\r\n",section);
        return;
    }
    lensDCJ36_Zoom_GoStep(ZoomSection_Pulse[section]);
}

static void lensDCJ36_Zoom_GoSection2(UINT32 section)
{
    UINT32 cnt = 0, ZoomCtrl_TimeOut = 3000;

    if(section>ZOOM_SECTION_TELE || section<ZOOM_SECTION_WIDE)
    {
        DBG_ERR("Incorrect section value (%d)\r\n",section);
        return;
    }
    lensDCJ36_Zoom_GoStep(ZoomSection_Pulse[section]);

    // Wait for idle state.
    while(1)
    {
        if(DCJ36_Para.ZoomStatus == ZOOM_STATUS_IDLE)
        {
            LD_DELAYMS(100); // Delay to ensure zoom do stopped!
            break;
        }
        else
        {
            LD_DELAYMS(1);
            cnt++;
            if(cnt>ZoomCtrl_TimeOut)
            {
                DBG_ERR("^RZoom_GoSection Timeout! - 1\r\n");
                break;
            }
        }
    }

    // To Do: Backlash compensation.

    // Turn off all Zoom related power signals.
    LD_DELAYMS(100); // Delay to ensure zoom do stopped!
    lensDCJ36_setPIInt(OFF);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
    LD_DELAYMS(2);
}

/* position: absolute position. */
static void lensDCJ36_Zoom_GoStep(INT32 position)
{
    INT32 step;
    UINT32 dir;

    if(DCJ36_Para.ZoomStep > position)
    {
        dir = DIRECTION_BWD;
        step = DCJ36_Para.ZoomStep - position;
    }
    else if(DCJ36_Para.ZoomStep < position)
    {
        dir = DIRECTION_FWD;
        step = position - DCJ36_Para.ZoomStep;
    }
    else
    {
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
        DCJ36_Para.ZoomStatus = ZOOM_STATUS_IDLE;
        return;
    }

    if(step>0xFFFF)
    {
        step = 0xFFFF;
    }

    //#NT#2011/04/18#Lincy Lin -begin
    //#NT#set zoom duty
//    LensCtrl_ZoomChangeSpeed(DCJ36_Para.ZoomInDuty1);
    //#NT#2011/04/18#Lincy Lin -end

    //#NT#2010/11/01#Photon Lin -begin
    //#Fine tune flow
    DCJ36_Para.ZoomStatus = ZOOM_STATUS_MOVE;
    if(DCJ36_Para.ZoomLastDir != dir)
    {
        //#NT#2012/07/09#Isiah Chang -begin
        //[Chris Chung] What does it do?
        //Ans: There is 40~50% probability to stop on PI signal, it will cause step miss when running opposite direction.
        #if (!ZPI_RISING_EDGE_TRIGGER) // Use falling edge detection temporarily!!! Must use rising edge on EVB!!!
        if(!lensDCJ36_getSignal(LD_SIGNAL_ZPI))
        {
            //#NT#2010/12/15#Lincy Lin -begin
            //#NT#
            if(dir == DIRECTION_FWD)
            {
                //DCJ36_Para.ZoomStep--;
                DCJ36_Para.ZoomStep++;
            }
            else
            {
                //DCJ36_Para.ZoomStep++;
                DCJ36_Para.ZoomStep--;
            }
            //#NT#2010/12/15#Lincy Lin -end
        }
        #else
        // if(g_pZoomPara.uiGPIOIntPol == GPIO_INTPOL_POSHIGH)
        if(lensDCJ36_getSignal(LD_SIGNAL_ZPI))
        {
            if(dir==DIRECTION_BWD)
            {
                DCJ36_Para.ZoomStep--;
            }
            else
            {
                DCJ36_Para.ZoomStep++;
            }
        }
        #endif
        //#NT#2012/07/09#Isiah Chang -end
    }

    DCJ36_Para.ZoomDir = dir;
    DCJ36_Para.StepMove = step;
    DCJ36_Para.StepCnt = 0;
    DCJ36_Para.ZoomLastDir = dir;

    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,ON);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,ON);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,ON);
    lensDCJ36_setPIInt(ON);
    //#NT#2010/12/02#Photon Lin -begin
    //#Refine flow
    LD_DELAYMS(2);
    //#NT#2010/12/02#Photon Lin -end

    if(dir == DIRECTION_FWD)
    {
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomforward();//LensCtrl_ZoomForward();
    }
    else
    {
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomreverse();//LensCtrl_ZoomReverse();
    }

}

static INT32 lensDCJ36_Zoom_Press(UINT32 zoom)
{
    UINT32 Step_Ori,Step_New;

    if(DCJ36_Para.ZoomStatus != ZOOM_STATUS_IDLE)
    {
        DBG_ERR("Zoom is still movining, (%d)\r\n",DCJ36_Para.ZoomStatus);
        return ERR_OPERATION_FAIL;
    }

    DCJ36_Para.bDoOptZoomDraw = TRUE;    //OpticalZoom Callback to Draw
    DCJ36_Para.ZoomSection = lensDCJ36_Zoom_GetCurrentSection();
    // API for zoom duty change
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomchangespeed(DCJ36_Para.ZoomInDuty1);//LensCtrl_ZoomChangeSpeed(DCJ36_Para.ZoomInDuty1);

    if(DCJ36_Para.bZoomBoundaryAutoCheck)
    {
        if((zoom == ZOOM_IN) && (DCJ36_Para.ZoomSection<ZOOM_SECTION_TELE))
        {
            DCJ36_Para.ZommSpdSlwDownDly = 7;    //Add zoom stability
            lensDCJ36_Focus_AfterZoomMoving(0);
            lensDCJ36_Zoom_GoSection(ZOOM_SECTION_TELE);
            #if 1
            DCJ36_Para.bFocusAfterZoom = TRUE;
            #else
            DCJ36_Para.bFocusAfterZoom = FALSE;
            #endif
            DCJ36_Para.bFocusOnceZoomChanged = TRUE;
        }
        else if((zoom == ZOOM_OUT) && DCJ36_Para.ZoomSection>ZOOM_SECTION_WIDE)
        {
            lensDCJ36_Focus_AfterZoomMoving(0);
            lensDCJ36_Zoom_GoSection(ZOOM_SECTION_WIDE);
            #if 1
            DCJ36_Para.bFocusAfterZoom = TRUE;
            #else
            DCJ36_Para.bFocusAfterZoom = FALSE;
            #endif
            DCJ36_Para.bFocusOnceZoomChanged = TRUE;
        }
        else
        {
            //#NT#2010/12/03#Photon Lin -begin
            //#Fine tune flow
            DCJ36_Para.ZoomStatus = ZOOM_STATUS_ERROR;
            //#NT#2010/12/03#Photon Lin -end
        }
    }
    else
    {
        if(zoom == ZOOM_IN)
        {
            lensDCJ36_Zoom_GoStep(0xFFFF);
            DCJ36_Para.bFocusAfterZoom = FALSE;
        }
        else if(zoom == ZOOM_OUT)
        {
            //#NT#2010/12/14#Photon Lin -begin
            lensDCJ36_Zoom_GoStep(-1000);
            //#NT#2010/12/14#Photon Lin -end
            DCJ36_Para.bFocusAfterZoom = FALSE;
        }
        else
        {
            //#NT#2010/12/03#Photon Lin -begin
            //#Fine tune flow
            DCJ36_Para.ZoomStatus = ZOOM_STATUS_ERROR;
            //#NT#2010/12/03#Photon Lin -end
        }
    }

    Step_Ori = DCJ36_Para.ZoomStep;
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomchange();//LensCtrl_ZoomChange();
    {
        UINT32 uiTimeMs=0;
        for(uiTimeMs=0; uiTimeMs<40; uiTimeMs++)
        {
            LD_DELAYMS(1);
            Step_New = DCJ36_Para.ZoomStep;
            if((lensDCJ36_abs((INT32)Step_New - (INT32)Step_Ori))>2)
                break;
        }
    }
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomchange();//LensCtrl_ZoomChange();
    Step_New = DCJ36_Para.ZoomStep;
    if((lensDCJ36_abs((INT32)Step_New - (INT32)Step_Ori)<2) && (!(DCJ36_Para.ZoomStatus == ZOOM_STATUS_IDLE)))
    {
        return ERR_OPERATION_FAIL;
    }
    else
    {
        return ERR_OK;
    }
}

static UINT32 lensDCJ36_Zoom_Release(void)
{
    UINT32 TargetSection;
    UINT32 cnt=0,ZoomCtrl_TimeOut = 2000;

    DCJ36_Para.bDoOptZoomDraw=FALSE;
    //#Fine tune flow
    if(DCJ36_Para.ZoomStatus == ZOOM_STATUS_ERROR)
    {
        lensDCJ36_setPIInt(OFF);
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
        LD_DELAYMS(2);
        DCJ36_Para.ZoomStatus = ZOOM_STATUS_IDLE;
        return DCJ36_Para.ZoomSection;
    }
    if(DCJ36_Para.ZoomStatus == ZOOM_STATUS_IDLE)
    {
        lensDCJ36_setPIInt(OFF);
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
        LD_DELAYMS(2);
    }

    cnt = 0;
    if(DCJ36_Para.bZoomBoundaryAutoCheck)
    {
        TargetSection = lensDCJ36_Zoom_GetTargetSection();
        if(DCJ36_Para.ZoomStatus != ZOOM_STATUS_IDLE)
        {
            //DBG_MSG("^RTargetSec%d, position:%d\r\n", TargetSection, ZoomSection_Pulse[TargetSection]);
            lensDCJ36_Zoom_GoStep(ZoomSection_Pulse[TargetSection]);
            while(1)
            {
                if(DCJ36_Para.ZoomStatus == ZOOM_STATUS_IDLE)
                {
                    break;
                }
                else //Avoid hang-up
                {
                    LD_DELAYMS(1);
                    cnt++;
                    if(cnt>ZoomCtrl_TimeOut)
                    {
//                        LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_90);
                        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomlockspeed(TRUE);//LensCtrl_ZoomLockSpeed(TRUE);
                        if(DCJ36_Para.ZoomDir == DIRECTION_FWD)
                        {
                            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomforward();//LensCtrl_ZoomForward();
                        }
                        else
                            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomreverse();//LensCtrl_ZoomReverse();
                        cnt=0;
                        while(1)
                        {
                            LD_DELAYMS(1);
                            if(DCJ36_Para.ZoomStatus == ZOOM_STATUS_IDLE)
                            {
                                break;
                            }
                            cnt++;
                            if(cnt>100)
                            {
                                break;
                            }
                        }
                        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomlockspeed(FALSE);//LensCtrl_ZoomLockSpeed(FALSE);
                        if(cnt<100)
                        {
                            break;//push-hard success
                        }

                        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
                        //#Refine flow
                        lensDCJ36_setPIInt(OFF);
                        LD_DELAYMS(2);
                        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
                        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

                        DBG_ERR("LensError\r\n");

                        if(g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomgetspeed() <= ZOOM_SPEED_10)
                        {
                            DCJ36_Para.ZoomStatus = ZOOM_STATUS_IDLE;
                        }
                        else
                        {
                            return 0xff;
                        }
                    }
                }

            }
        }

        cnt = 0;
        if(DCJ36_Para.ZoomDir == DIRECTION_BWD)
        {
            if(TargetSection == ZOOM_SECTION_WIDE)
            {
                DCJ36_Para.bZoomBoundaryAutoCheck=FALSE; // Don't do boundary check for BL compensation at wide position.
                if(lensDCJ36_Zoom_DoBL(ZOOM_BL_STEP, ZoomSection_Pulse[lensDCJ36_Zoom_GetCurrentSection()]) != ERR_OK)
                {
                    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
                    //#Refine flow
                    lensDCJ36_setPIInt(OFF);
                    LD_DELAYMS(2);
                    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
                    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
                    DBG_ERR("Do Backlash fail without auto check\r\n");
                    return 0xFF;
                }
                DCJ36_Para.bZoomBoundaryAutoCheck=TRUE;
            }
            else
            {
                if(lensDCJ36_Zoom_DoBL(ZOOM_BL_STEP, ZoomSection_Pulse[lensDCJ36_Zoom_GetCurrentSection()]) != ERR_OK)
                {
                    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
                    //#Refine flow
                    lensDCJ36_setPIInt(OFF);
                    LD_DELAYMS(2);
                    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
                    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
                    DBG_ERR("Do Backlash fail with auto check\r\n");
                    return 0xFF;
                }
            }
        }

        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomchange();//LensCtrl_ZoomChange();

        while(1)
        {
            if(DCJ36_Para.ZoomStatus == ZOOM_STATUS_IDLE)
            {
                LD_DELAYMS(100);
                break;
            }
            else
            {
                LD_DELAYMS(1);
                cnt++;
                if(cnt>ZoomCtrl_TimeOut)
                    break;
            }
        }
    }
    else
    {
        if(DCJ36_Para.ZoomStatus != ZOOM_STATUS_IDLE)
        {
            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
            //#Refine flow
            lensDCJ36_setPIInt(OFF);
            LD_DELAYMS(2);
            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
            DCJ36_Para.ZoomStatus = ZOOM_STATUS_IDLE;
        }
    }
    //#Refine flow
    lensDCJ36_setPIInt(OFF);
    LD_DELAYMS(2);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);

    /* Wait to ensure background focus task is idle. */
    //DBG_MSG("LensCtrlBG_WaitIdle - begin\r\n");
    DCJ36_Para.bFocusOnceZoomChanged = FALSE;
    g_DCJ36_LensCtrlTskAPIObj.lensctrltskBG_waitidle();//LensCtrlBG_WaitIdle();
    //DBG_MSG("LensCtrlBG_WaitIdle - end\r\n");

    lensDCJ36_Focus_DoEXC(ON,0);

    DCJ36_Para.ZoomSection = lensDCJ36_Zoom_GetCurrentSection();

    if(DCJ36_Para.bFocusAfterZoom)
    {
        lensDCJ36_Focus_AfterZoomMoving(1);
        DCJ36_Para.bFocusAfterZoom = FALSE;
    }

    lensDCJ36_Focus_DoEXC(OFF,0);

    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomchange();//LensCtrl_ZoomChange();

    /* Trigger IQS DCE function if system has free time. */
    #if ZOOM_ENABLE_DCE_NOTIFY
    //if(LensCtrlBG_CheckIdle())
        g_DCJ36_LensCtrlTskAPIObj.lensctrltskBG_iqs_setflowfunc();//LensCtrlBG_Iqs_SetFlowFunc();
    #endif

    return DCJ36_Para.ZoomSection;
}

static INT32 lensDCJ36_Zoom_Retract(void)
{
    UINT32 i=0;
    INT32 status = ERR_OK;
    UINT32 ZoomHomeTimeOut;

    if(lensDCJ36_Focus_Reset(POS_GARAGE) != ERR_OK)
    {
        DBG_ERR("%s, Focus to garage fail\n\r",__func__);
        return ERR_OPERATION_FAIL;
    }

    DBG_MSG("%s Focus to garage ok\n\r",__func__);

    //LD_DELAYMS(50);

    if(DCJ36_Para.ZoomStep<=(INT32)ZoomSection_Pulse[0])
    {
        return ERR_OPERATION_FAIL;
    }

    //Retract to position home
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,ON);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,ON);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,ON);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,ON);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);
    //#Refine flow
    lensDCJ36_setPIInt(ON);
    LD_DELAYMS(5);

    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomchangespeed(ZOOM_SPEED_100);//LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_100);

    DCJ36_Para.StepCnt = 0;
    DCJ36_Para.ZoomIsr = ZOOM_Isr_Close;

    //IN ZONE C
    if((lensDCJ36_getSignal(LD_SIGNAL_ZPR) == ZOOMPR_HI_LEVEL) && (g_DCJ36_LensCtrlTskAPIObj.lensctrltsk2_readlensstatus() ==ZOOM_FLAG_OUTSIDE))
    {
        DBG_MSG("ZONE C\r\n");
        DCJ36_Para.ZoomStatus = ZOOM_STATUS_ZC_2_ZB;
        DCJ36_Para.ZoomDir = DIRECTION_BWD;
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomreverse();//LensCtrl_ZoomReverse();
    }
    //IN ZONE B
    else if(lensDCJ36_getSignal(LD_SIGNAL_ZPR) == ZOOMPR_LOW_LEVEL)
    {
        DBG_MSG("LensDCJ36_Zoom_Home, zone B \r\n");
//        LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_100);     //PWM9
        DCJ36_Para.ZoomDir = DIRECTION_BWD;
        DCJ36_Para.ZoomStatus = ZOOM_STATUS_ZB_RETRACT;
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomreverse();//LensCtrl_ZoomReverse();
    }
    //IN ZONE A
    else if(lensDCJ36_getSignal(LD_SIGNAL_ZPR) == ZOOMPR_HI_LEVEL)
    {
        DBG_MSG("LensDCJ36_Zoom_Home, zone A \r\n");
//        LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_100);     //PWM6
        DCJ36_Para.ZoomDir = DIRECTION_FWD;
        DCJ36_Para.ZoomStatus = ZOOM_STATUS_FIND_HP;
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomforward();//LensCtrl_ZoomForward();
    }

    ZoomHomeTimeOut = 10000;

    while (DCJ36_Para.ZoomStatus!= ZOOM_STATUS_NULL)
    {
        LD_DELAYMS(1);

        if(i==10)  //error occur, must do some thing.. by
        {
//            LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_90);
        }
        if((i+200)>ZoomHomeTimeOut)
        {
//            LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_100);
        }
        if(++i > ZoomHomeTimeOut)  //error occur, must do some thing.. by
        {
            DBG_ERR("%s time out, force to home\r\n",__func__);
            if(lensDCJ36_Zoom_ForceToHome() != ERR_OK)
            {
                status = ERR_OPERATION_FAIL;
                DBG_ERR("%s, force to home fail\r\n",__func__);
                DCJ36_Para.ZoomStep = ZoomSection_Pulse[1];//set to wide for coming error-handle
            }
            else
            {
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
            }
            break;
        }
    }

    DBG_MSG("Zoom Step %d\r\n", DCJ36_Para.ZoomStep);

    DCJ36_Para.ZoomSection=0;
    //#Refine flow
    lensDCJ36_setPIInt(OFF);
    LD_DELAYMS(2);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
    lensDCJ36_setPIInt(OFF);

    //ZoomStatus = lensDCJ36_Zoom_Home();

    return status;

}

static INT32 lensDCJ36_Zoom_Reset(void)
{
    INT32 ERROR_STATUS;

    lensDCJ36_Zoom_InitPart1();
    ERROR_STATUS = lensDCJ36_Zoom_InitPart2();

    return ERROR_STATUS;
}

static INT32 lensDCJ36_Zoom_ForceToHome(void)
{
    UINT32 i = 0;
    INT32 status = ERR_OK;
    UINT32 ZoomHomeTimeOut;

//    LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_60);

    DCJ36_Para.StepCnt=0;
    {// Backward
        //UINT32 uiTimeCnt=0;
        DCJ36_Para.ZoomIsr = ZOOM_Isr_Null;

//        LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_30);
//        LD_DELAYMS(5);
//        LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_10);
        LD_DELAYMS(5);
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
        LD_DELAYMS(30);

        //IN ZONE C
        if((lensDCJ36_getSignal(LD_SIGNAL_ZPR) == ZOOMPR_HI_LEVEL) && (g_DCJ36_LensCtrlTskAPIObj.lensctrltsk2_readlensstatus() ==ZOOM_FLAG_OUTSIDE))
        {
            DBG_MSG(">ZONE C\r\n");
            DCJ36_Para.ZoomStatus = ZOOM_STATUS_ZC_2_ZB;
            DCJ36_Para.ZoomDir = DIRECTION_BWD;
            DCJ36_Para.ZoomIsr = ZOOM_Isr_Open;
            DCJ36_Para.StepCnt=0;
            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomreverse();//LensCtrl_ZoomReverse();
            LD_DELAYMS(5);
//            LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_90);
        }
        //IN ZONE B
        if(lensDCJ36_getSignal(LD_SIGNAL_ZPR) == ZOOMPR_LOW_LEVEL)
        {
            DBG_MSG(">ZONE_B\r\n");
            #if 0
            LensCtrl_ZoomForward();
            LD_DELAYMS(200);
            LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_30);
            LD_DELAYMS(5);
            LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_10);
            LD_DELAYMS(5);
            LensCtrl_ZoomBrake();
            LD_DELAYMS(30);
            #endif
            DCJ36_Para.ZoomDir = DIRECTION_BWD;
            DCJ36_Para.ZoomStatus = ZOOM_STATUS_ZB_RETRACT;
            DCJ36_Para.ZoomIsr = ZOOM_Isr_Open;
            DCJ36_Para.StepCnt=0;
//            LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_60);
            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomreverse();//LensCtrl_ZoomReverse();
            LD_DELAYMS(5);
//            LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_90);
        }
        //IN ZONE A
        else
        {
            DBG_MSG(">ZONE_A\r\n");
//            LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_90);   //PWM5
            DCJ36_Para.ZoomDir = DIRECTION_FWD;
            DCJ36_Para.ZoomStatus = ZOOM_STATUS_FIND_HP;
            DCJ36_Para.ZoomIsr = ZOOM_Isr_Open;
            DCJ36_Para.ZoomStep = 0;
            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomforward();//LensCtrl_ZoomForward();
        }

    }

    ZoomHomeTimeOut = 1000;

    while (DCJ36_Para.ZoomStatus != ZOOM_STATUS_IDLE)
    {
        LD_DELAYMS(1);
        if(++i > ZoomHomeTimeOut)
        {
            DBG_MSG("lensDCJ36_Zoom_RescueHome time out\r\n");
            status = ERR_OPERATION_FAIL;
            DCJ36_Para.ZoomStep = ZoomSection_Pulse[1];//set to wide for coming error-handle
            break;
        }
    }
    return status;
}

//--------------------------------------------------------------------------------------------------------------------------
//Part of Focus
//--------------------------------------------------------------------------------------------------------------------------
static INT32 lensDCJ36_Focus_Init(void)
{
    UINT32 sBoundary;

    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,ON);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,ON);//LensCtrl_SetSignal(MD_SIGNAL_FPI,ON);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,ON);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);
    LD_DELAYMS(2);

    if(lensDCJ36_getSignal(LD_SIGNAL_ZPR) == ZOOMPR_LOW_LEVEL) // Zoom is in ZB.
    {
        sBoundary = 0;

        lensDCJ36_Focus_DoEXC(ON,0);

        while(lensDCJ36_getSignal(LD_SIGNAL_FPI) == FOCUSPI_OUTSDIE_LEVEL)
        {
            lensDCJ36_Focus_RunStep(DIRECTION_BWD,10);
            sBoundary += 10;

            if(sBoundary > FOCUS_BWD_LIMIT_OUTSIDE_PI)
            {
                DBG_ERR("%s, time out happen and can not find PI\r\n", __func__);
                lensDCJ36_Focus_DoEXC(OFF,0);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
                return ERR_LENS_INIT_FAIL;
            }
        }

        sBoundary = 0;

        while(lensDCJ36_getSignal(LD_SIGNAL_FPI) == FOCUSPI_INSDIE_LEVEL)
        {
            lensDCJ36_Focus_RunStep(DIRECTION_FWD,1);
            sBoundary += 1;
            if(sBoundary > ((FOCUS_POS_HOME_PR-FOCUS_POS_GARAGE)+FOCUS_BL_STEP+8))
            {
                if(lensDCJ36_Focus_RescueInit()==0)
                {
                    break;
                }
                else
                {
                    DBG_ERR("%s, init fail\r\n", __func__);
                    lensDCJ36_Focus_DoEXC(OFF,0);
                    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
                    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
                    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
                    return ERR_LENS_INIT_FAIL;
                }
            }
        }

        DCJ36_Para.FocusPosition = FOCUS_POS_HOME_PR;

        //TM_End(LENS_FOCUS_INIT);
        //TM_Begin(LENS_FOCUS2_INIT);
        lensDCJ36_Focus_Go2(Focus_Tbl[0/*0: wide*/][2/*2: 2m*/]);
        lensDCJ36_Focus_DoEXC(OFF,0);

        DBG_MSG("Focus init step = %d\n\r",DCJ36_Para.FocusPosition);

    }

    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
    return ERR_OK;
}

static UINT32 lensDCJ36_Focus_RescueInit(void)
{
    UINT32 sBoundary;

    sBoundary = 0;
    while(lensDCJ36_getSignal(LD_SIGNAL_FPI) == FOCUSPI_INSDIE_LEVEL)
    {
        lensDCJ36_Focus_RunStep(DIRECTION_FWD,1);
        Delay_DelayUs(FOCUS_SPEED);
        sBoundary += 1;
        if(sBoundary > 350)
        {
            return 1;//Change Focus Init Return Value
        }
    }
    sBoundary = 0;
    while(lensDCJ36_getSignal(LD_SIGNAL_FPI) == FOCUSPI_OUTSDIE_LEVEL)
    {
        lensDCJ36_Focus_RunStep(DIRECTION_BWD,10);
        sBoundary += 10;
        if(sBoundary > FOCUS_BWD_LIMIT_OUTSIDE_PI)
        {
            return 2;//Change Focus Init Return Value
        }
    }

    lensDCJ36_Focus_RunStep(DIRECTION_BWD,10);

    sBoundary = 0;
    while(lensDCJ36_getSignal(LD_SIGNAL_FPI) == FOCUSPI_INSDIE_LEVEL)
    {
        lensDCJ36_Focus_RunStep(DIRECTION_FWD,1);
        sBoundary += 1;
        if(sBoundary > FOCUS_FWD_LIMIT_INSIDE_PI)
        {
            return 3;//Change Focus Init Return Value
        }
    }
    DCJ36_Para.FocusPosition = FOCUS_POS_HOME_PR;
    return 0;
}

static UINT32 lensDCJ36_Focus_GetStepByDist(UINT32 uiDistance)
{
    UINT32 i;
    UINT32 uiFocusPos;
    UINT32 uiZoomSecMinusOne;

    uiZoomSecMinusOne = lensDCJ36_Zoom_GetSection()-1;

#if 0
    if(uiZoomSecMinusOne == 0)
        return Focus_Tbl[0/*0: wide*/][2/*2: 2m*/];
#endif

    for(i=0; i<11; i++)
    {
        if(uiDistance>=Focus_Dist_Tbl[i])
            break;
    }
    if(i==0)
    {
        uiFocusPos= Focus_Tbl[uiZoomSecMinusOne][0];
    }
    else if(i==11)
    {
        uiFocusPos= Focus_Tbl[uiZoomSecMinusOne][10];
    }
    else
    {
        INT32 iAval, iBval, iAintvl, iBintvl;

        iAval = Focus_Tbl[uiZoomSecMinusOne][i-1];
        iBval = Focus_Tbl[uiZoomSecMinusOne][i];
        iAintvl = Focus_Dist_Tbl[i-1] - uiDistance;
        iBintvl = uiDistance - Focus_Dist_Tbl[i];
        uiFocusPos = (iAval*iBintvl + iBval*iAintvl)/(iAintvl+iBintvl);
    }
    return uiFocusPos;

}
//um in unit.
static UINT32 lensDCJ36_Focus_GetLength(UINT32 ZoomSection)
{
    return FocusLength[ZoomSection];
}

static INT32 lensDCJ36_Focus_GetPosition(void)
{
    return DCJ36_Para.FocusPosition;
}

static UINT32 lensDCJ36_Focus_GetDistTable(UINT32 section)
{
    return Focus_Dist_Tbl[section];
}

static INT32 lensDCJ36_Focus_GetDefultTable(UINT32 section, UINT32 FocusDistance)
{
    if (section>ZOOM_SECTION_TELE)
    {
        section = ZOOM_SECTION_TELE;
        DBG_ERR("%s, section error, (%d)r\n",__func__,section);
    }

    if (FocusDistance>FOCUS_DISTANCE)
    {
        FocusDistance = FOCUS_DISTANCE;
        DBG_ERR("%s, FocusDistance error, (%d)r\n",__func__,FocusDistance);
    }

    if (section < 1)
    {
        section = 0;
    }

    if (FocusDistance < 1)
    {
        FocusDistance = 0;
    }


    return Focus_Tbl_Default[section][FocusDistance];
}

static UINT32 lensDCJ36_Focus_GetDist(void)
{
    UINT32 i;
    UINT32 uiDist;
    UINT32 uiZoomSecMinusOne;
    UINT32 uiAval, uiBval, uiAintvl, uiBintvl;

    uiZoomSecMinusOne = lensDCJ36_Zoom_GetSection()-1;
    for(i=0; i<(FOCUS_DISTANCE-1); i++)
    {
         if(lensDCJ36_Focus_GetPosition()<lensDCJ36_Focus_GetTable(uiZoomSecMinusOne,i))
            break;
    }
    if(i==0)
    {
        uiDist= lensDCJ36_Focus_GetDistTable(0);
    }
    else if(i==(FOCUS_DISTANCE-1))
    {
        uiDist= lensDCJ36_Focus_GetDistTable(FOCUS_DISTANCE-2);
    }
    else
    {

        uiAval = lensDCJ36_Focus_GetDistTable(i-1);
        uiBval = lensDCJ36_Focus_GetDistTable(i);
        uiAintvl = lensDCJ36_Focus_GetPosition() - lensDCJ36_Focus_GetTable(uiZoomSecMinusOne,i-1);
        uiBintvl = lensDCJ36_Focus_GetTable(uiZoomSecMinusOne,i)-lensDCJ36_Focus_GetPosition();
        uiDist = (uiAval*uiBintvl + uiBval*uiAintvl)/(uiAintvl+uiBintvl);
    }
    return uiDist;
}

static INT32 lensDCJ36_Focus_GetTable(UINT32 section, UINT32 FocusDistance)
{
    if (section>ZOOM_SECTION_TELE)
    {
        DBG_ERR("%s, section error, (%d)r\n",__func__,section);
        section = ZOOM_SECTION_TELE;
    }

    if (FocusDistance>FOCUS_DISTANCE)
    {
        DBG_ERR("%s, FocusDistance error, (%d)\r\n",__func__,FocusDistance);
        FocusDistance = FOCUS_DISTANCE;
    }

    if (section < 1)
    {
        section = 0;
    }

    if (FocusDistance < 1)
    {
        FocusDistance = 0;
    }

    return Focus_Tbl[section][FocusDistance];
}

static INT32 lensDCJ36_Focus_GetRange(FOCUS_RANGE range)
{
    switch (range)
    {
    case RANGE_START:
        return Focus_Tbl_Search_Range[lensDCJ36_Zoom_GetCurrentSection()-1][0];
    case RANGE_END:
        return Focus_Tbl_Search_Range[lensDCJ36_Zoom_GetCurrentSection()-1][1];
    default:
        DBG_ERR("%s, parameter error, (%d)r\n",__func__,range);
        return FOCUS_RANGE_START;
    }
}



static void lensDCJ36_Focus_AfterZoomMoving(UINT32 uiCmd)
{
    static UINT32 uiDistance=0xFFFF;
    if(uiCmd == 0)
    {
        uiDistance = lensDCJ36_Focus_GetDist();
    }
    else if(uiCmd == 1)
    {
        lensDCJ36_Focus_Go2(lensDCJ36_Focus_GetStepByDist(uiDistance));
    }
}

#if 0
static void lensDCJ36_Focus_DoBL(UINT32 Steps)
{
    lensDCJ36_Focus_RunStep(DIRECTION_BWD,Steps);
    LD_DELAYMS(10);
    lensDCJ36_Focus_RunStep(DIRECTION_FWD,Steps);
}
#endif

static void lensDCJ36_Focus_DoEXC(BOOL on, UINT32 dir)
{
    if(on)
    {
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,ON);
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_focus_setstate(MOTOR_FOCUS_EXCITE_ON,0);//LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_ON,0);
    }
    else
    {
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_focus_setstate(MOTOR_FOCUS_EXCITE_OFF,0);//LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_OFF,0);
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);
    }
}

static void lensDCJ36_Focus_RunStep(UINT32 dir,UINT32 step)
{
    UINT32 Count = 0;

    while(Count < step)
    {
        if(dir==DIRECTION_BWD)
        {
            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_focus_setstate(MOTOR_FOCUS_BWD,0);//LensCtrl_Focus_SetState(MOTOR_FOCUS_BWD,0);
            DCJ36_Para.FocusPosition-=1;
            Count++;
        }
        else
        {
            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_focus_setstate(MOTOR_FOCUS_FWD,0);//LensCtrl_Focus_SetState(MOTOR_FOCUS_FWD,0);
            DCJ36_Para.FocusPosition+=1;
            Count++;
        }
    }
}

#define CAL_BL_STEP    5 // Calibration BL step is machine dependent.
static void lensDCJ36_Focus_Go2(INT32 position)
{
    UINT32 step, uiDir;
    static UINT32 uiLastDir = 0xFFFFFFFF;

    if(position>FOCUS_POS_OUTER_STOP)
    {
        DBG_ERR("%s, position incorrect, (%d)\r\n",__func__,position);
        return;
    }

    if(DCJ36_Para.FocusPosition > position)
    {
        uiDir = DIRECTION_BWD;
        step = DCJ36_Para.FocusPosition - position;
    }
    else
    {
        uiDir = DIRECTION_FWD;
        step = position - DCJ36_Para.FocusPosition;
    }

    // Do BL only when the first time in the reverse way.
    if(uiLastDir != uiDir && uiLastDir != 0xFFFFFFFF)
    {
        step += CAL_BL_STEP;
    }

    lensDCJ36_Focus_RunStep(uiDir,step);

    if(uiLastDir != uiDir && uiLastDir != 0xFFFFFFFF)
    {
        if(uiDir == DIRECTION_FWD)
            DCJ36_Para.FocusPosition -= CAL_BL_STEP;
        else
            DCJ36_Para.FocusPosition += CAL_BL_STEP;
    }

    uiLastDir = uiDir;
}

static INT32 lensDCJ36_Focus_Reset(FOCUS_RETRACT_POSITION position)
{
    UINT32 sBoundary;

    if(position!=POS_GARAGE)
    {
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,ON);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,ON);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,ON);
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,ON);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);
        //#NT#2010/12/02#Photon Lin -begin
        //#Refine flow
        lensDCJ36_setPIInt(ON);
        LD_DELAYMS(2);
        //#NT#2010/12/02#Photon Lin -end
        //Make sure zoom position outside
        if(lensDCJ36_getSignal(LD_SIGNAL_ZPR) == ZOOMPR_LOW_LEVEL)
        {
            //A indenpendent Zoom-Out-Process -begin
            lensDCJ36_setPIInt(OFF);
//            LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_60);
            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomforward();//LensCtrl_ZoomForward();
            //Check if zoom FWD more than 80 but signal not high->low
            LD_DELAYMS(160);

            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoombrake();//LensCtrl_ZoomBrake();
            LD_DELAYMS(5);
            lensDCJ36_setPIInt(ON);
        }
        //#NT#2010/12/02#Photon Lin -begin
        //#Refine flow
        lensDCJ36_setPIInt(OFF);
        LD_DELAYMS(2);
        //#NT#2010/12/02#Photon Lin -end
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

    }

    DBG_MSG("Focus to %s\r\n",(position == POS_GARAGE)?"POS_GARAGE":"POS_HOME");

    //Back through PI
    lensDCJ36_Focus_DoEXC(ON, 0);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,ON);//LensCtrl_SetSignal(MD_SIGNAL_FPI,ON);
    //find PI
    sBoundary = 0;
    while(lensDCJ36_getSignal(LD_SIGNAL_FPI) == FOCUSPI_INSDIE_LEVEL)
    {
        lensDCJ36_Focus_RunStep(DIRECTION_FWD,1);
        sBoundary += 1;

        if(sBoundary > FOCUS_FWD_LIMIT_INSIDE_PI)
        {
            DBG_MSG("B: %d>%d",sBoundary, FOCUS_FWD_LIMIT_INSIDE_PI);
            lensDCJ36_Focus_RunStep(DIRECTION_BWD,FOCUS_FWD_LIMIT_INSIDE_PI);
            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
            lensDCJ36_Focus_DoEXC(OFF, 0);
            return ERR_OPERATION_FAIL;
        }
    }

    while(lensDCJ36_getSignal(LD_SIGNAL_FPI) == FOCUSPI_OUTSDIE_LEVEL)
    {
        lensDCJ36_Focus_RunStep(DIRECTION_BWD,1);
        sBoundary += 1;
        if(sBoundary > FOCUS_BWD_LIMIT_OUTSIDE_PI)
        {
            g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
            lensDCJ36_Focus_DoEXC(OFF, 0);
            return ERR_OPERATION_FAIL;
        }
    }

    DCJ36_Para.FocusPosition = FOCUS_POS_HOME_PR;

    //Back to garage
    if (position == POS_GARAGE)
    {
        lensDCJ36_Focus_Go2(FOCUS_POS_GARAGE);
    }

    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
    lensDCJ36_Focus_DoEXC(OFF, 0);
    return ERR_OK;}


//--------------------------------------------------------------------------------------------------------------------------
//Part of Aperture
//--------------------------------------------------------------------------------------------------------------------------
static void lensDCJ36_Aperture_Init(void)
{
    // Reset iris to the position 0. Refer to spec. P.8.
    DCJ36_Para.AperturePos = IRIS_POS_MAX;
    lensDCJ36_Aperture_Go2Pos(IRIS_POS_RESET);

    // Move iris to pos. 5. Refer to spec. P.8.
    lensDCJ36_Aperture_Go2Pos(IRIS_POS_F1_8);
}

static IRIS_POS lensDCJ36_Aperture_GetPosition(void)
{
    return DCJ36_Para.AperturePos;
}

static UINT32 lensDCJ36_Aperture_GetFNO(UINT32 ZoomSection,UINT32 IrisPos)
{
    return Aperture_Zoom_Tbl[ZoomSection][IrisPos];
}

static void lensDCJ36_Aperture_Go2Pos(IRIS_POS position)
{
    UINT32 i, uiStep = 0;

    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,ON);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);

    if ((position >= IRIS_POS_MAX))
    {
        DBG_ERR("%s, parameters error, (%d)\r\n",__func__,position);
    }
    else if(DCJ36_Para.AperturePos != position)
    {
        #if 0
        //#NT#2010/12/01#Lincy Lin -begin
        //LensCtrl_Aperture_SetState(position);
        LensCtrl_Aperture_SetState(MOTOR_APERTURE_NORMAL,position);
        //#NT#2010/12/01#Lincy Lin -end
        #else
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,ON);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_aperture_setstate(MOTOR_APERTURE_EXCITE_ON,0);//LensCtrl_Aperture_SetState(MOTOR_APERTURE_EXCITE_ON,0);

        switch (position)
        {
            case IRIS_POS_RESET: // Reset iris.
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_aperture_setstate(MOTOR_APERTURE_RESET, 0);//LensCtrl_Aperture_SetState(MOTOR_APERTURE_RESET, 0); // Reset motor phase.
                for (i = 0; i < IRIS_TOTAL_STEP; i++)
                    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_aperture_setstate(MOTOR_APERTURE_FWD,0);//LensCtrl_Aperture_SetState(MOTOR_APERTURE_FWD,0);
            break;

            case IRIS_POS_F1_8:
            case IRIS_POS_F2_0:
            case IRIS_POS_F2_8:
            case IRIS_POS_F4_0:
            case IRIS_POS_F5_6:
            case IRIS_POS_F8_0:
                if(DCJ36_Para.AperturePos == IRIS_POS_RESET)
                {
                    uiStep = Aperture_Pos_Tbl[position];
                    if(uiStep > IRIS_TOTAL_STEP)
                    {
                        DBG_ERR("%s, swith iris to big fail \r\n",__func__);
                        return;
                    }

                    for (i = 0; i < uiStep; i++) // Switch to big iris(position 5).
                        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_aperture_setstate(MOTOR_APERTURE_BWD,0);//LensCtrl_Aperture_SetState(MOTOR_APERTURE_BWD,0);
                }
                else if(position > DCJ36_Para.AperturePos) // switch from bigger to smaller iris.
                {
                    uiStep = Aperture_Pos_Tbl[position] - Aperture_Pos_Tbl[DCJ36_Para.AperturePos];
                    if(uiStep > IRIS_TOTAL_STEP)
                    {
                        DBG_ERR("%s, swith iris to small fail \r\n",__func__);
                        return;
                    }

                    for (i = 0; i < uiStep; i++) // Switch to smaller iris.
                        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_aperture_setstate(MOTOR_APERTURE_BWD,0);//LensCtrl_Aperture_SetState(MOTOR_APERTURE_BWD,0);
                }
                else if(position < DCJ36_Para.AperturePos) // switch from smaller to bigger iris.
                {
                    uiStep = Aperture_Pos_Tbl[DCJ36_Para.AperturePos] - Aperture_Pos_Tbl[position];
                    if(uiStep > IRIS_TOTAL_STEP)
                    {
                        DBG_ERR("%s, swith iris to big fail \r\n",__func__);
                        return;
                    }

                    for (i = 0; i < uiStep; i++) // Switch to bigger iris.
                        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_aperture_setstate(MOTOR_APERTURE_FWD,0);//LensCtrl_Aperture_SetState(MOTOR_APERTURE_FWD,0);
                }
                else
                {
                    DBG_ERR("%s, Unknown iris value\r\n",__func__);
                }
            break;

            default:
                DBG_ERR("%s, Unsupport iris value\r\n",__func__);
            break;

        }

        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_aperture_setstate(MOTOR_APERTURE_EXCITE_OFF,0);//LensCtrl_Aperture_SetState(MOTOR_APERTURE_EXCITE_OFF,0);
        g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
        #endif

        DCJ36_Para.AperturePos = position;
    }

    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
}

//--------------------------------------------------------------------------------------------------------------------------
//Part of Shutter
//--------------------------------------------------------------------------------------------------------------------------
static void lensDCJ36_shutter_setState(MOTOR_SHUTTER_ACT state, UINT32 param1)
{
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_shutter_setstate(state,param1);//LensCtrl_Shutter_SetState(state,param1);
}

//--------------------------------------------------------------------------------------------------------------------------
//Part of misc.
//--------------------------------------------------------------------------------------------------------------------------
static INT32 lensDCJ36_LensRetract(void)
{
#if 0
    UINT32 i=0,cnt=0;
    INT32 status = ERR_OK;
    //Need to make sure focus go home
    lensDCJ36_Focus_Reset(POS_GARAGE);
    LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
    LensCtrl_SetSignal(MD_SIGNAL_ZPI,ON);
    LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);
    //#NT#2010/12/02#Photon Lin -begin
    //#Refine flow
    lensDCJ36_setPIInt(ON);
    LD_DELAYMS(2);
    //#NT#2010/12/02#Photon Lin -end
    DCJ36_Para.ZoomDir = DIRECTION_BWD;
    DCJ36_Para.ZoomIsr = ZOOM_Isr_Close;
    DCJ36_Para.ZoomStatus = ZOOM_STATUS_ZB_RETRACT;
    DCJ36_Para.ZoomStep = 266+ZoomSection_Pulse[ZOOM_SECTION_TELE];
    LensCtrl_ZoomChangeSpeed(ZOOM_SPEED_40);

    DCJ36_Para.StepCnt = 0;
    LensCtrl_ZoomReverse();

    LD_DELAYMS(10);

    while(1)
    {
        if(DCJ36_Para.ZoomStep == i)
        {
            cnt++;
            if(cnt > 5)
            {
                status = ERR_OPERATION_FAIL;
                break;
            }
        }
        else
        {
            cnt = 0;
        }
        i = DCJ36_Para.ZoomStep;
        LD_DELAYMS(5);
    }
    LD_DELAYMS(300);
    LensCtrl_ZoomBrake();
    DCJ36_Para.ZoomSection = 0;
    //#NT#2010/12/02#Photon Lin -begin
    //#Refine flow
    lensDCJ36_setPIInt(OFF);
    LD_DELAYMS(2);
    //#NT#2010/12/02#Photon Lin -end
    LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
    LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
    LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

    if (status == ERR_OK)
    {
        LensCtrl2_WriteLensStatus(ZOOM_FLAG_INSIDE);
    }

    return status;
#else
    return lensDCJ36_Zoom_Retract();
#endif
}

static INT32 lensDCJ36_LensReset(void)
{
    DBG_MSG("Lens reset\r\n");
    if(DCJ36_Para.ZoomStatus != ZOOM_STATUS_IDLE)
    {
        DBG_ERR("%s, error due to zoom is moving (%d) \r\n",__func__,DCJ36_Para.ZoomStatus);
        return ERR_OPERATION_FAIL;
    }

    DCJ36_Para.ZoomWideCnt = 0;

    // 1. Retract focus
    lensDCJ36_Focus_DoEXC(ON, 0);
    lensDCJ36_Focus_Go2(FOCUS_POS_HOME_PR-FOCUS_BL_STEP);
    lensDCJ36_Focus_DoEXC(OFF, 0);

    // 2. Reset zoom
    if(lensDCJ36_Zoom_Reset() != ERR_OK)
    {
        DBG_ERR("%s, error due to lensDCJ36_Zoom_Reset() fail \r\n",__func__);
        return ERR_OPERATION_FAIL;
    }

    // 3. Initiate focus
    lensDCJ36_Focus_DoEXC(ON, 0);
    lensDCJ36_Focus_Init();
    lensDCJ36_Focus_DoEXC(OFF, 0);

    return ERR_OK;
}

static INT32 lensDCJ36_LensPowerOff(void)
{
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_zoomoff();//LensCtrl_ZoomOff();

    return ERR_OK;
}

#if 0
static CAL_USAGE void lensDCJ36_UpdateAdjustData(void)
{
#if 0
    //#NT#2010/12/17#Photon Lin -begin
    //#Modify flow of calibration
    Cal_Struct *CalInfo;
    CalInfo = Cal_GetCalibrationData();

    if(CalInfo->CalStatus_ZoomStop2PR == _Zoom_Stop2PR_Status)
    {
        //#NT#2011/03/14#Photon Lin -begin
        //#NT#Patch for precision improve
        #if (PRECISION_UPGRADE)
        DBG_MSG("DCJ36: Loading Cal_A: %d \r\n",(CalInfo->Cal_Zoom_PR2Tele-ZOOM_STANDARD_D));
        DCJ36_Para.ZoomHP = ZOOM_STOPPER_COUNT-(CalInfo->Cal_Zoom_PR2Tele-ZOOM_STANDARD_D);
        #else
        DBG_MSG("DCJ36: Loading Cal_A: %d \r\n",(CalInfo->Cal_Zoom_PR2Tele-(ZOOM_STANDARD_D>>1)));
        DCJ36_Para.ZoomHP = (ZOOM_STOPPER_COUNT>>1)-(CalInfo->Cal_Zoom_PR2Tele-(ZOOM_STANDARD_D>>1));
        #endif
    }
    else
    {
        DBG_MSG("DCJ36: Loading ref_A: %d \r\n",ZOOM_STANDARD_A);
        DCJ36_Para.ZoomHP =  ZOOM_POS_HOME_PR;
    }
    //#NT#2010/12/17#Photon Lin -end
#endif
}
#endif

#if 0
static void lensDCJ36_SequenceScaleAndShift(INT32 *piModel,
                                                INT32 *piOutput,
                                                UINT8 uiLength,
                                                INT16 iSmblA,
                                                UINT16 uiIdxA,
                                                INT16 iSmblB,
                                                UINT16 uiIdxB,
                                                INT16 iSmblMax,
                                                INT16 iSmblMin)
{
    UINT32 i;
    INT32 iIntvlModel, iIntvlOutput;
    INT32 iRangeModel, iRangeOutput;
    INT32 iOutputSmbl;

    iRangeModel  = *(piModel+(uiIdxB)) - *(piModel+(uiIdxA));
    iRangeOutput = iSmblB - iSmblA;

    for(i=0; i<uiLength; i++)
    {//Assign other members
        if(i==uiIdxA)
        {
            iOutputSmbl = iSmblA;
        }
        else if(i==uiIdxB)
        {
            iOutputSmbl = iSmblB;
        }
        else if(i<uiIdxA)
        {//shift
            iIntvlModel = (INT32)*(piModel+(uiIdxA)) - (INT32)*(piModel+(i));
            iIntvlOutput = iIntvlModel;//No need to Scale//*iRangeOutput/iRangeModel;
            iOutputSmbl = (INT32)iSmblA - iIntvlOutput;
        }
        else if(i>uiIdxB)
        {//shift
            iIntvlModel = (INT32)*(piModel+(uiIdxB)) - (INT32)*(piModel+(i));
            iIntvlOutput = iIntvlModel;//No need to Scale//*iRangeOutput/iRangeModel;
            iOutputSmbl = (INT32)iSmblB - iIntvlOutput;
        }
        else
        {
            iIntvlModel = (INT32)*(piModel+(uiIdxA)) - (INT32)*(piModel+(i));
            iIntvlOutput = iIntvlModel*iRangeOutput/iRangeModel;
            iOutputSmbl = (INT32)iSmblA - iIntvlOutput;
        }

        if(iOutputSmbl>iSmblMax)
            iOutputSmbl = iSmblMax;
        else if(iOutputSmbl<iSmblMin)
            iOutputSmbl = iSmblMin;

        *(piOutput+(i)) = iOutputSmbl;
    }
}
#endif

static INT32 lensDCJ36_SetCalData(INT16 *pCalTbl, UINT32 uiTblSize)
{
    UINT32 i;

    if(pCalTbl && uiTblSize == ZOOM_SECTION_TELE)
    {
        g_pFocus_CalTbl = pCalTbl;

        for(i = 0; i < ZOOM_SECTION_TELE; i++)
        {
            g_tFocus_CalTbl[i] = g_pFocus_CalTbl[i];
            DBG_MSG("^YZoom sec_%d: INF peak_idx:%d\r\n", i+1, g_tFocus_CalTbl[i]);
        }
    }
    else
    {
        DBG_ERR("^RCalTbl = NULL or size NG!\r\n");
        return ERR_OPERATION_FAIL;
    }

    return ERR_OK;
}


static void lensDCJ36_MakeAFTable(void)
{
    UINT32 i, j;
    INT32  iStepOffset;
#if 0
    Cal_Struct *CalInfo;
#endif


    if(ZOOM_SECTION_TELE<0)
        return;

#if 0
    CalInfo = Cal_GetCalibrationData();

    if((CalInfo->CalStatus_FocusFar == _FF_Status) && (CalInfo->CalStatus_FocusNear == _NF_Status ))
    {
        for(i=0; i<ZOOM_SECTION_TELE; i++)
        {
            lensDCJ36_SequenceScaleAndShift(Focus_Tbl_Default[i],
                                          Focus_Tbl[i],
                                          12,
                                          CalInfo->Cal_Focus_FarTable[i],
                                          0,
                                          CalInfo->Cal_Focus_NearTable[i],
                                          5,// 6,
                                          FOCUS_RANGE_END,
                                          FOCUS_RANGE_START);
        }
    }
    else
#endif
    {
        // Load default table first.
        for(i=0; i<ZOOM_SECTION_TELE; i++)
        {
            for(j=0; j<FOCUS_DISTANCE; j++)
            {
                Focus_Tbl[i][j] = Focus_Tbl_Default[i][j];
            }
        }

        // If it's been calibrated, update Focus table based on 3M distance for each zoom section.
        if(g_pFocus_CalTbl)
        {
            DBG_MSG("Read focus calibration data ok\r\n");
            for(i = 0; i < ZOOM_SECTION_TELE; i++)
            {
                iStepOffset = (g_tFocus_CalTbl[i] - Focus_Tbl_Default[i][1]);
                DBG_MSG("^GZoom sec_%d, StepOffset:%d\r\n", i+1, iStepOffset);
                for(j = 0; j < FOCUS_DISTANCE; j++)
                {
                    Focus_Tbl[i][j] += iStepOffset;
                    DBG_MSG("^GZoom sec_%d: peak_idx:%d\r\n", i+1, Focus_Tbl[i][j]);
                }
            }
        }
        else
        {
            DBG_ERR("Read focus calibration data fail\r\n");
        }
    }
}

static INT32 lensDCJ36_abs(INT32 i)
{
    if (i >= 0)
    {
        return i;
    }
    return (-i);
}

#define FOCUS_BL_CALIBRATION_STEP   100
INT32 lensDCJ36_Focus_getBL(void)
{
    UINT32 sBoundary, uiBLStep = 0, count = 0;

    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,ON);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,ON);//LensCtrl_SetSignal(MD_SIGNAL_FPI,ON);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,ON);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);
    LD_DELAYMS(2);

    if(lensDCJ36_getSignal(LD_SIGNAL_ZPR) == ZOOMPR_LOW_LEVEL) // Zoom is in ZB.
    {
        sBoundary = 0;

        lensDCJ36_Focus_DoEXC(ON,0);

        while(lensDCJ36_getSignal(LD_SIGNAL_FPI) == FOCUSPI_OUTSDIE_LEVEL)
        {
            lensDCJ36_Focus_RunStep(DIRECTION_BWD,1);
            count += 1;
            if(count > FOCUS_BWD_LIMIT_OUTSIDE_PI)
            {
                DBG_ERR(": %d>%d", count, FOCUS_BWD_LIMIT_OUTSIDE_PI);
                lensDCJ36_Focus_DoEXC(OFF,0);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
                return E_SYS;
            }
        }

        if(count)
        {
            Delay_DelayMs(500);
        }

        count = 0;
        while(lensDCJ36_getSignal(LD_SIGNAL_FPI) == FOCUSPI_INSDIE_LEVEL)
        {
            lensDCJ36_Focus_RunStep(DIRECTION_FWD,1);
            count += 1;
            if(count > FOCUS_FWD_LIMIT_INSIDE_PI)
            {
                DBG_ERR(": %d>%d", count, FOCUS_FWD_LIMIT_INSIDE_PI);
                lensDCJ36_Focus_DoEXC(OFF,0);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
                return E_SYS;
            }
        }

        lensDCJ36_Focus_RunStep(DIRECTION_FWD, FOCUS_BL_CALIBRATION_STEP);
        Delay_DelayMs(500);

        count = 0;
        while(lensDCJ36_getSignal(LD_SIGNAL_FPI) == FOCUSPI_OUTSDIE_LEVEL)
        {
            lensDCJ36_Focus_RunStep(DIRECTION_BWD,1);
            count += 1;
            if(count > FOCUS_BWD_LIMIT_OUTSIDE_PI)
            {
                DBG_ERR(": %d>%d", count, FOCUS_BWD_LIMIT_OUTSIDE_PI);
                lensDCJ36_Focus_DoEXC(OFF,0);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
                g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
                return E_SYS;
            }
        }

        uiBLStep = (count - FOCUS_BL_CALIBRATION_STEP);
        if(count > FOCUS_BL_CALIBRATION_STEP)
        {
            //DBG_DUMP(": BL step = %d\r\n", uiBLStep);
        }
        else
            DBG_ERR(": count = %d\r\n", count);

        lensDCJ36_Focus_DoEXC(OFF,0);
    }
    else
    {
        DBG_ERR("Zoom position is improper\r\n");
    }

    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_ZPR,OFF);//LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_FPI,OFF);//LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
    g_DCJ36_LensCtrlTskAPIObj.lensctrltsk_setsignal(MD_SIGNAL_STANDBY,OFF);//LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

    return uiBLStep;
}

