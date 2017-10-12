#include <string.h>
#include "Type.h"
#include "Kernel.h"
#include "Utility.h"
#include "Lens.h"
#include "LensAPI.h"
#include "LensCtrlTsk.h"
#include "GxFlash.h"
#include "PStore.h"
#include "SxCmd.h"
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          LENSAPI
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define LENS_SEMID_INIT_VALUE    0xFFFFFFFF

static LENSCTRL_APPOBJ gLensCtrlObj;
static LENS_STATE uiLensModuleState = LENS_STATE_NONE;
static UINT32 Lens_semid = LENS_SEMID_INIT_VALUE;
extern SX_CMD_ENTRY lens[];

void Lens_Module_Init(PLENSCTRL_APPOBJ pLensCtrlObj)
{
    if(pLensCtrlObj->pLensDev == NULL||
       pLensCtrlObj->pLens == NULL||
       pLensCtrlObj->pMotor == NULL)
    {
        DBG_ERR("%s param error!!\r\n", __func__);
        return;
    }

    //Assign semaphore id
    Lens_semid = gLensCtrlObj.semid = SEMID_LENS_CTLID;

    memcpy(&gLensCtrlObj, pLensCtrlObj, sizeof(LENSCTRL_APPOBJ));

    //init motor driver
    gLensCtrlObj.pMotor->init(pLensCtrlObj->pLensDev);

    //init lens driver
    gLensCtrlObj.pLens->init(pLensCtrlObj->pLensDev,pLensCtrlObj->pMotor);


    //open lens task
    LensCtrl_Open(pLensCtrlObj);
    LensCtrl2_Open(pLensCtrlObj);
    LensCtrlBG_Open(pLensCtrlObj);

    SxCmd_AddTable(lens);
}


static void Lens_Module_SetState(LENS_STATE state)
{
    if (Lens_semid != LENS_SEMID_INIT_VALUE)
    {
        wai_sem(Lens_semid);
    }
    else
    {
        DBG_ERR("%s Lens_semid init error\r\n", __func__);
    }

    uiLensModuleState = state;

    if (Lens_semid != LENS_SEMID_INIT_VALUE)
    {
        sig_sem(Lens_semid);
    }

}

LENS_STATE Lens_Module_GetState(void)
{
    return uiLensModuleState;
}

INT32 Lens_Init(LENS_INIT_STATE part)
{
    INT32 ERROR_STATUS;

    switch(part)
    {
        case LENS_INIT_ZOOM_PART1:
            //Step 1. Initiate zoom part1
            DBG_MSG("Start of LENS_INIT_ZOOM_PART1\r\n");
            //Callback function
            //TM_Begin(LENS_ZOOM_INIT);
            if(gLensCtrlObj.APICB!=NULL)
            {
                gLensCtrlObj.APICB(LENS_CB_INITZOOM1_START, NULL);
            }

            gLensCtrlObj.pLens->zoom_initPart1();

            //Callback function
            if(gLensCtrlObj.APICB!=NULL)
            {
                gLensCtrlObj.APICB(LENS_CB_INITZOOM1_END, NULL);
            }

            Lens_Module_SetState(LENS_STATE_INIT_PART1);
            DBG_MSG("End of LENS_STATE_INIT_PART1\r\n");    // --> LENS_STATE_INIT_PART1
            return ERR_OK;

        case LENS_INIT_ZOOM_PART2:
            //Step 2. Initiate zoom part2
            DBG_MSG("Start of LENS_INIT_ZOOM_PART2\r\n");

            //TM_Begin(LENS_ZOOM_INITWAIT);
            //Callback function
            if(gLensCtrlObj.APICB!=NULL)
            {
                gLensCtrlObj.APICB(LENS_CB_INITZOOM2_START, NULL);
            }

            ERROR_STATUS = gLensCtrlObj.pLens->zoom_initPart2();

            //Callback function
            if(gLensCtrlObj.APICB!=NULL)
            {
                gLensCtrlObj.APICB(LENS_CB_INITZOOM2_END, NULL);
            }

            //TM_End(LENS_ZOOM_INITWAIT);

            DBG_MSG("End of LENS_INIT_ZOOM_PART2\r\n");

            return ERROR_STATUS;

        case LENS_INIT_APERTURE:
            //Step 3. Initiate aperture
            DBG_MSG("Start of LENS_INIT_APERTURE\r\n");
            //TM_Begin(LENS_APER_INIT);

            //Callback function
            if(gLensCtrlObj.APICB!=NULL)
            {
                gLensCtrlObj.APICB(LENS_CB_INITAPERTURE_START, NULL);
            }
            gLensCtrlObj.pLens->aperture_init();
            //#NT#2010/09/23#Jeffery Chuang -begin
            //Add shutter init here until add new api

            gLensCtrlObj.pMotor->shutter_setState(MOTOR_SHUTTER_NORMAL,OPEN);
            //#NT#2010/09/23#Jeffery Chuang -end

            //Callback function
            if(gLensCtrlObj.APICB!=NULL)
            {
                gLensCtrlObj.APICB(LENS_CB_INITAPERTURE_END, NULL);
            }

            //TM_End(LENS_APER_INIT);

            DBG_MSG("End of LENS_INIT_APERTURE\r\n");
            return ERR_OK;

        case LENS_INIT_FOCUS:
            //TM_Begin(LENS_FOCUS_INIT);
            DBG_MSG("Start of LENS_INIT_FOCUS\r\n");

            //Callback function
            if(gLensCtrlObj.APICB!=NULL)
            {
                gLensCtrlObj.APICB(LENS_CB_INITFOCUS_START, NULL);
            }

            #if 1 // Could be migrated to other place in the near future.
            {
                PPSTORE_SECTION_HANDLE  pSection;
                INT16 iTmpIdx[15] = {0};
                UINT32 uiCalTblSize = Lens_Zoom_GetSection(ZOOM_MAX_SECTION)/* Table size of DemoKit is '11'. */, i;

                if(sizeof(iTmpIdx) >= uiCalTblSize)
                {
                    if ((pSection = PStore_OpenSection(PS_FOCUS_DATA, PS_RDWR)) != E_PS_SECHDLER)
                    {
                        PStore_ReadSection((UINT8 *)&iTmpIdx, 0, uiCalTblSize * sizeof(INT16), pSection);
                        PStore_CloseSection(pSection);

                        for(i = 0; i < uiCalTblSize; i++)
                        {
                            DBG_MSG("^GZoom sec_%d: peak_idx:%d\r\n", i+1, iTmpIdx[i]);
                        }
                        Lens_Focus_SetCalData(iTmpIdx, uiCalTblSize);
                    }
                    else
                        DBG_ERR("No focus calibration data!\r\n");
                }
                else
                {
                    DBG_ERR("Buf size isn't enough for AF cal data!\r\n");
                }
            }
            #endif

            gLensCtrlObj.pLens->makeAFTable();

            ERROR_STATUS = gLensCtrlObj.pLens->focus_init();

            //Callback function
            if(gLensCtrlObj.APICB!=NULL)
            {
                gLensCtrlObj.APICB(LENS_CB_INITFOCUS_END, NULL);
            }

            Lens_Module_SetState(LENS_STATE_IDLE);
            DBG_MSG("LENS_STATE_IDLE\r\n");    // --> LENS_STATE_IDLE
            DBG_MSG("End of LENS_INIT_FOCUS\r\n");
            //TM_End(LENS_FOCUS2_INIT);

            return ERROR_STATUS;


        default:
            DBG_ERR("Init wrong state\r\n");
            return ERR_OPERATION_FAIL;
    }


}

INT32 Lens_UnInit(void)
{
    if (Lens_Module_GetState() != LENS_STATE_NONE)
    {
        DBG_ERR("%s, state incorrect, (%d)\r\n",__func__,Lens_Module_GetState());
        return ERR_OPERATION_FAIL;
    }
    DBG_MSG("%s\r\n",__func__);

    Lens_Module_SetState(LENS_STATE_POWER_OFF);
    DBG_MSG("LENS_STATE_POWER_OFF\r\n");    // --> LENS_STATE_POWER_OFF

    gLensCtrlObj.pLens->lensPowerOff();

    return ERR_OK;
}

INT32 Lens_Retract(void)
{
    INT32 ERROR_STATUS;

    ERROR_STATUS = gLensCtrlObj.pLens->lensRetract();

    gLensCtrlObj.pMotor->shutter_setState(MOTOR_SHUTTER_NORMAL,CLOSE);

    Lens_Module_SetState(LENS_STATE_NONE);

    DBG_MSG("LENS_STATE_NONE\r\n");    // --> LENS_STATE_NONE

    return ERROR_STATUS;
}

INT32 Lens_Reset(void)
{
    INT32 status;

    if (Lens_Module_GetState() != LENS_STATE_IDLE)
    {
        DBG_ERR("%s, state incorrect, (%d)\r\n",__func__,Lens_Module_GetState());
        return ERR_OPERATION_FAIL;
    }
    DBG_MSG("%s\r\n",__func__);

    Lens_Module_SetState(LENS_STATE_RESET);

    //Callback function
    if(gLensCtrlObj.APICB!=NULL)
    {
        gLensCtrlObj.APICB(LENS_CB_RESET_START, NULL);
    }

    status = gLensCtrlObj.pLens->lensReset();

    //Callback function
    if(gLensCtrlObj.APICB!=NULL)
    {
        gLensCtrlObj.APICB(LENS_CB_RESET_END, NULL);
    }

    Lens_Module_SetState(LENS_STATE_IDLE);
    DBG_MSG("LENS_STATE_IDLE\r\n");    // --> LENS_STATE_IDLE

    return status;
}

#if 0
void Lens_Zoom_SetBoundaryCheck(BOOL en)
{
    gLensCtrlObj.pLens->zoom_setBoundaryCheck(en);
}
#endif

void Lens_Zoom_SetSpeed(ZOOM_SPEED_CATEGORY category)
{
    gLensCtrlObj.pLens->zoom_setSpeed(category);
}

UINT32 Lens_Zoom_GetSection(ZOOM_SECTION_CATEGORY category)
{
    switch(category)
    {
        case ZOOM_MIN_SECTION:
            return gLensCtrlObj.pLens->zoom_getMinSection();
        case ZOOM_MAX_SECTION:
            return gLensCtrlObj.pLens->zoom_getMaxSection();
        case ZOOM_CUR_SECTION:
        default:
            //#NT#2011/01/20#Photon Lin -begin
            //#NT#Add flow control
            if ((Lens_Module_GetState() == LENS_STATE_INIT_PART1)||(Lens_Module_GetState() == LENS_STATE_NONE))
            {
                return gLensCtrlObj.pLens->zoom_getMinSection();
            }
            else
            {
                return gLensCtrlObj.pLens->zoom_getSection();
            }
            //#NT#2011/01/20#Photon Lin -end

    }
}

UINT32 Lens_Zoom_GetStep(ZOOM_STEP_CATEGORY category)
{
    switch(category)
    {
        case ZOOM_CUR_SECTION_STEP:
            return gLensCtrlObj.pLens->zoom_getSectionStep();
        case ZOOM_CUR_STEP:
        default:
            return gLensCtrlObj.pLens->zoom_getStep();
    }
}

void Lens_Zoom_GoSection(UINT32 section)
{
    if(gLensCtrlObj.pLens->zoom_goSection2) // Without backlash compensation.
        gLensCtrlObj.pLens->zoom_goSection2(section);
}

INT32 Lens_Zoom_In(void)
{
    if (Lens_Module_GetState() != LENS_STATE_IDLE)
    {
        DBG_ERR("%s, state incorrect, (%d)\r\n",__func__,Lens_Module_GetState());
        return ERR_OPERATION_FAIL;
    }
    DBG_MSG("%s\r\n",__func__);

    Lens_Module_SetState(LENS_STATE_ZOOMMOVING);
    DBG_MSG("LENS_STATE_ZOOMMOVING\r\n");    // --> LENS_STATE_ZOOMMOVING

    //Callback function
    if(gLensCtrlObj.APICB!=NULL)
    {
        gLensCtrlObj.APICB(LENS_CB_ZOOM_START, NULL);
    }

    gLensCtrlObj.pLens->zoom_press(ZOOM_IN);

    return ERR_OK;
}

INT32 Lens_Zoom_Out(void)
{
    if (Lens_Module_GetState() != LENS_STATE_IDLE)
    {
        DBG_ERR("%s, state incorrect, (%d)\r\n",__func__,Lens_Module_GetState());
        return ERR_OPERATION_FAIL;
    }
    DBG_MSG("%s\r\n",__func__);

    Lens_Module_SetState(LENS_STATE_ZOOMMOVING);
    DBG_MSG("LENS_STATE_ZOOMMOVING\r\n");    // --> LENS_STATE_ZOOMMOVING

    //Callback function
    if(gLensCtrlObj.APICB!=NULL)
    {
        gLensCtrlObj.APICB(LENS_CB_ZOOM_START, NULL);
    }

    gLensCtrlObj.pLens->zoom_press(ZOOM_OUT);

    return ERR_OK;
}

INT32 Lens_Zoom_Stop(void)
{
    UINT32 Status;

    //#NT#2012/07/10#Isiah Chang -begin
    //#NT#Correct Lens module state handler.
    #if 0
    if ((Lens_Module_GetState() == LENS_STATE_IDLE) || (Lens_Module_GetState() != LENS_STATE_ZOOMMOVING))
    {
        DBG_ERR("%s, state incorrect, (%d)\r\n",__func__,Lens_Module_GetState());
        return ERR_OPERATION_FAIL;
    }
    #else
    switch(Lens_Module_GetState())
    {
    case LENS_STATE_IDLE:
        return ERR_OK;
    case LENS_STATE_ZOOMMOVING:
    case LENS_STATE_FOCUSMOVING:
        break;
    default:
        DBG_ERR("%s, state incorrect, (%d)\r\n",__func__,Lens_Module_GetState());
        return ERR_OPERATION_FAIL;
    }
    #endif
    //#NT#2012/07/10#Isiah Chang -end
    DBG_MSG("%s\r\n",__func__);

    Status = gLensCtrlObj.pLens->zoom_release();

    //Callback function
    if(gLensCtrlObj.APICB!=NULL)
    {
        gLensCtrlObj.APICB(LENS_CB_ZOOM_END, NULL);
    }

    Lens_Module_SetState(LENS_STATE_IDLE);
    DBG_MSG("LENS_STATE_IDLE\r\n");    // --> LENS_STATE_IDLE

    if((Status>gLensCtrlObj.pLens->zoom_getMaxSection()) || (Status<gLensCtrlObj.pLens->zoom_getMinSection()))
        return ERR_OPERATION_FAIL;
    else
        return ERR_OK;
}

INT32 Lens_Zoom_Retract(void)
{
    INT32 Status;
    switch(Lens_Module_GetState())
    {
    //Mark this case, due to boot with playback, then shut down, lens must be retract
    //case LENS_STATE_NONE:
    case LENS_STATE_ZOOMMOVING:
    case LENS_STATE_FOCUSMOVING:
    case LENS_STATE_IRISMOVING:
    case LENS_STATE_SHUTTERMOVING:
    //#NT#2011/04/08#Photon Lin -begin
    //#NT#Improve flow
    case LENS_STATE_NONE:
    //#NT#2011/04/08#Photon Lin -end
        DBG_ERR("%s, state incorrect, (%d)\r\n",__func__,Lens_Module_GetState());
        return ERR_OPERATION_FAIL;
    default:
        break;

    }
    DBG_MSG("%s\r\n",__func__);

    Lens_Module_SetState(LENS_STATE_ZOOMMOVING);
    DBG_MSG("LENS_STATE_ZOOMMOVING\r\n");    // --> LENS_STATE_ZOOMMOVING

    //Callback function
    if(gLensCtrlObj.APICB!=NULL)
    {
        gLensCtrlObj.APICB(LENS_CB_ZOOM_START, NULL);
    }

    Status = gLensCtrlObj.pLens->zoom_retract();
    //#NT#2010/09/23#Jeffery Chuang -begin
    //Add shutter init here until add new api
    gLensCtrlObj.pMotor->shutter_setState(MOTOR_SHUTTER_NORMAL,CLOSE);
    //#NT#2010/09/23#Jeffery Chuang -end

    //Callback function
    if(gLensCtrlObj.APICB!=NULL)
    {
        gLensCtrlObj.APICB(LENS_CB_ZOOM_END, NULL);
    }

    Lens_Module_SetState(LENS_STATE_NONE);
    DBG_MSG("LENS_STATE_NONE\r\n");    // --> LENS_STATE_NONE

    return Status;
}

/*
INT32 Lens_Zoom_Reset(void)
{
    INT32 Status;
    switch(Lens_Module_GetState())
    {
    case LENS_STATE_NONE:
    case LENS_STATE_ZOOMMOVING:
    case LENS_STATE_FOCUSMOVING:
    case LENS_STATE_IRISMOVING:
    case LENS_STATE_SHUTTERMOVING:
        DBG_ERR("State incorrect in Lens_Zoom_Reset(), (%d)\r\n",Lens_Module_GetState());
        return ERR_OPERATION_FAIL;
    default:
        break;

    }
    DBG_MSG("Lens_Zoom_Reset()\r\n");

    Lens_Module_SetState(LENS_STATE_ZOOMMOVING);
    DBG_MSG("LENS_STATE_ZOOMMOVING\r\n");    // --> LENS_STATE_ZOOMMOVING

    //Callback function
    if(gLensCtrlObj.APICB!=NULL)
    {
        gLensCtrlObj.APICB(LENS_CB_ZOOM_START, NULL);
    }

    Status = gLensCtrlObj.pLens->zoom_reset();

    //Callback function
    if(gLensCtrlObj.APICB!=NULL)
    {
        gLensCtrlObj.APICB(LENS_CB_ZOOM_END, NULL);
    }

    Lens_Module_SetState(LENS_STATE_NONE);
    DBG_MSG("LENS_STATE_NONE\r\n");    // --> LENS_STATE_NONE

    return Status;

}
*/

UINT32 Lens_Focus_GetLength(UINT32 ZoomSection)
{
    if(gLensCtrlObj.pLens)
        return (gLensCtrlObj.pLens->focus_getLength(ZoomSection) / 100);
    else
    {
        DBG_ERR("Lens Obj not registered!\r\n");
        return 0;
    }
}

UINT32 Lens_Focus_GetLength_um(UINT32 ZoomSection)
{
    if(gLensCtrlObj.pLens)
        return gLensCtrlObj.pLens->focus_getLength(ZoomSection);
    else
    {
        DBG_ERR("Lens Obj not registered!\r\n");
        return 0;
    }
}

INT32 Lens_Focus_GetPosition(void)
{
    if(gLensCtrlObj.pLens)
        return gLensCtrlObj.pLens->focus_getPostion();
    else
    {
        DBG_ERR("Lens Obj not registered!\r\n");
        return 0;
    }
}

UINT32 Lens_Focus_GetDistTable(UINT32 Section)
{
    if(gLensCtrlObj.pLens)
        return gLensCtrlObj.pLens->focus_getDistTable(Section);
    else
    {
        DBG_ERR("Lens Obj not registered!\r\n");
        return 0;
    }
}

INT32 Lens_Focus_GetDefaultTable(UINT32 Section, UINT32 FocusDistance)
{
    if(gLensCtrlObj.pLens)
        return gLensCtrlObj.pLens->focus_getDefaultTable(Section, FocusDistance);
    else
    {
        DBG_ERR("Lens Obj not registered!\r\n");
        return 0;
    }
}

INT32 Lens_Focus_GetTable(UINT32 Section, UINT32 FocusDistance)
{
    if(gLensCtrlObj.pLens)
        return gLensCtrlObj.pLens->focus_getTable(Section, FocusDistance);
    else
    {
        DBG_ERR("Lens Obj not registered!\r\n");
        return 0;
    }
}

INT32 Lens_Focus_SetCalData(INT16 *pCalTbl, UINT32 uiTblSize)
{
    if(gLensCtrlObj.pLens)
    {
        return gLensCtrlObj.pLens->setCalData(pCalTbl, uiTblSize);
    }
    else
    {
        DBG_ERR("Lens Obj not registered!\r\n");
        return ERR_OPERATION_FAIL;
    }
}

#if 0
INT32 Lens_Focus_GetRange(FOCUS_RANGE range)
{
    if(gLensCtrlObj.pLens)
        return gLensCtrlObj.pLens->focus_getRange(range);
    else
    {
        DBG_ERR("Lens Obj not registered!\r\n");
        return 0;
    }
}
#endif

INT32 Lens_Focus_DoAction(FOCUS_CATEGORY category, INT32 position)
{

    if (Lens_Module_GetState() != LENS_STATE_IDLE)
    {
        if(Lens_Module_GetState() != LENS_STATE_ZOOMMOVING)
        {
            DBG_ERR("%s, state incorrect, (%d)\r\n",__func__,Lens_Module_GetState());
            return ERR_OPERATION_FAIL;

        }
    }
    DBG_MSG("%s\r\n",__func__);
    if(Lens_Module_GetState() != LENS_STATE_ZOOMMOVING)
    {
        Lens_Module_SetState(LENS_STATE_FOCUSMOVING);
        DBG_MSG("LENS_STATE_FOCUSMOVING\r\n");    // --> LENS_STATE_FOCUSMOVING
    }

    //#NT#2010/08/19#Jeffery Chuang -begin
    //ADD direction for new MD
    if (category&FOCUS_PREEXC)
    {
        //Callback function
        if(gLensCtrlObj.APICB!=NULL)
        {
            gLensCtrlObj.APICB(LENS_CB_FOCUS_START, NULL);
        }

        if(position >= Lens_Focus_GetPosition())
            gLensCtrlObj.pLens->focus_doExc(ON,MOTOR_FOCUS_FWD);
        else
            gLensCtrlObj.pLens->focus_doExc(ON,MOTOR_FOCUS_BWD);
    }

    if (category&FOCUS_RUN)
    {
        gLensCtrlObj.pLens->focus_go2(position);
    }

    if (category&FOCUS_POSTEXC)
    {
        gLensCtrlObj.pLens->focus_doExc(OFF,0);

        //Callback function
        if(gLensCtrlObj.APICB!=NULL)
        {
            gLensCtrlObj.APICB(LENS_CB_FOCUS_END, NULL);
        }
    }

    //#NT#2010/08/19#Jeffery Chuang -end
    if(Lens_Module_GetState() != LENS_STATE_ZOOMMOVING)
    {
        Lens_Module_SetState(LENS_STATE_IDLE);
        DBG_MSG("LENS_STATE_IDLE\r\n");    // --> LENS_STATE_IDLE
    }
    return ERR_OK;
}

INT32 Lens_Focus_AfterZoomChanged(void)
{

    if (Lens_Module_GetState() != LENS_STATE_IDLE)
    {
        if(Lens_Module_GetState() != LENS_STATE_ZOOMMOVING)
        {
            DBG_ERR("%s, state incorrect, (%d)\r\n",__func__,Lens_Module_GetState());
            return ERR_OPERATION_FAIL;

        }
    }
    DBG_MSG("%s\r\n",__func__);
    if(Lens_Module_GetState() != LENS_STATE_ZOOMMOVING)
    {
        Lens_Module_SetState(LENS_STATE_FOCUSMOVING);
        DBG_MSG("LENS_STATE_FOCUSMOVING\r\n");    // --> LENS_STATE_FOCUSMOVING
    }

    //gLensCtrlObj.pLens->focus_afterZoomMoving(0);

    //if (category&FOCUS_PREEXC)
    {
        //Callback function
        if(gLensCtrlObj.APICB!=NULL)
        {
            gLensCtrlObj.APICB(LENS_CB_FOCUS_START, NULL);
        }

        gLensCtrlObj.pLens->focus_doExc(ON, 0);
    }

    /* Move focus to 2M distance. */
    #if 0
    gLensCtrlObj.pLens->focus_go2(Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 2));
    #else
    //gLensCtrlObj.pLens->focus_afterZoomMoving(0);
    gLensCtrlObj.pLens->focus_afterZoomMoving(1);
    #endif

    //#NT#2010/08/19#Jeffery Chuang -end
    if(Lens_Module_GetState() != LENS_STATE_ZOOMMOVING)
    {
        gLensCtrlObj.pLens->focus_doExc(OFF, 0);
        Lens_Module_SetState(LENS_STATE_IDLE);
        DBG_MSG("LENS_STATE_IDLE\r\n");    // --> LENS_STATE_IDLE
    }
    else // Don't turn off motor driver power.
        LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_OFF,0);

    //Callback function
    if(gLensCtrlObj.APICB!=NULL)
    {
        gLensCtrlObj.APICB(LENS_CB_FOCUS_END, NULL);
    }

    return ERR_OK;
}

#if 0
INT32 Lens_Focus_Retract(FOCUS_RETRACT_POSITION position)
{
    INT32 retValue;
    //Callback function
    if(gLensCtrlObj.APICB!=NULL)
    {
        gLensCtrlObj.APICB(LENS_CB_FOCUS_START, NULL);
    }

    //return gLensCtrlObj.pLens->focus_retract(position);
    retValue = gLensCtrlObj.pLens->focus_retract(position);

    //Callback function
    if(gLensCtrlObj.APICB!=NULL)
    {
        gLensCtrlObj.APICB(LENS_CB_FOCUS_END, NULL);
    }
    return retValue;

}
#endif

IRIS_POS Lens_Aperture_GetPosition(void)
{
    if(gLensCtrlObj.pLens)
        return gLensCtrlObj.pLens->aperture_getPosition();
    else
    {
        DBG_ERR("Lens Obj not registered!\r\n");
        return 0;
    }
}

UINT32 Lens_Aperture_GetFNO(UINT32 ZoomSection,UINT32 IrisPos)
{
    if(gLensCtrlObj.pLens)
        return gLensCtrlObj.pLens->aperture_getFNO(ZoomSection,IrisPos);
    else
    {
        DBG_ERR("Lens Obj not registered!\r\n");
        return 0;
    }
}

INT32 Lens_Aperture_Move(IRIS_POS Position)
{
    if (Lens_Module_GetState() != LENS_STATE_IDLE)
    {
        DBG_ERR("%s, State incorrect, (%d)\r\n",__func__,Lens_Module_GetState());
        return ERR_OPERATION_FAIL;
    }
    DBG_MSG("%s\r\n",__func__);

    Lens_Module_SetState(LENS_STATE_IRISMOVING);
    DBG_MSG("LENS_STATE_IRISMOVING\r\n");    // --> LENS_STATE_IRISMOVING

    //Callback function
    if(gLensCtrlObj.APICB!=NULL)
    {
        gLensCtrlObj.APICB(LENS_CB_APERTURE_START, NULL);
    }

    gLensCtrlObj.pLens->aperture_go2Position(Position);

    //Callback function
    if(gLensCtrlObj.APICB!=NULL)
    {
        gLensCtrlObj.APICB(LENS_CB_APERTURE_END, NULL);
    }

    Lens_Module_SetState(LENS_STATE_IDLE);
    DBG_MSG("LENS_STATE_IDLE\r\n");    // --> LENS_STATE_IDLE

    return ERR_OK;
}

INT32 Lens_Shutter_Move(UINT32 state,UINT32 act)
{
    //#NT#2010/10/14#Photon Lin -begin
    //#Add pseudo move for shutter
    if ((state == MOTOR_SHUTTER_PSEUDO_OPEN) || (state == MOTOR_SHUTTER_PSEUDO_CLOSE))
    {
        gLensCtrlObj.pLens->shutter_setState(state,act);
    }
    else
    {
        if (Lens_Module_GetState() != LENS_STATE_IDLE)
        {
            DBG_ERR("%s, state incorrect, (%d)\r\n",__func__,Lens_Module_GetState());
            return ERR_OPERATION_FAIL;
        }
        DBG_MSG("%s\r\n",__func__);

        Lens_Module_SetState(LENS_STATE_SHUTTERMOVING);
        DBG_MSG("LENS_STATE_SHUTTERMOVING\r\n");    // --> LENS_STATE_SHUTTERMOVING

        //Callback function
        if(gLensCtrlObj.APICB!=NULL)
        {
            gLensCtrlObj.APICB(LENS_CB_SHUTTER_START, NULL);
        }

        gLensCtrlObj.pLens->shutter_setState(state,act);

        //Callback function
        if(gLensCtrlObj.APICB!=NULL)
        {
            gLensCtrlObj.APICB(LENS_CB_SHUTTER_END, NULL);
        }

        Lens_Module_SetState(LENS_STATE_IDLE);
        DBG_MSG("LENS_STATE_IDLE\r\n");    // --> LENS_STATE_IDLE
    }
    //#NT#2010/10/14#Photon Lin -end

    return ERR_OK;
}

#if 0
void Lens_SetMDSignal(MD_SIGNAL signal, BOOL on)
{
    if(gLensCtrlObj.pMotor)
        gLensCtrlObj.pMotor->setSignal(signal,on);
    else
    {
        DBG_ERR("Motor Obj not registered!\r\n");
    }
}
#endif

UINT32 Lens_CheckLDSignal(LD_SIGNAL signal)
{
    if(gLensCtrlObj.pLens)
        return gLensCtrlObj.pLens->getSignal(signal);
    else
    {
        DBG_ERR("Lens Obj not registered!\r\n");
        return 0;
    }
}

/// Lens command
BOOL Cmd_lens_zoom_init(CHAR* strCmd);
BOOL Cmd_lens_focus_iris_init(CHAR* strCmd);
BOOL Cmd_lens_retract(CHAR* strCmd);
BOOL Cmd_lens_rpi(CHAR* strCmd);
BOOL Cmd_lens_gzs(CHAR* strCmd);

SXCMD_BEGIN(lens, "lens command")
SXCMD_ITEM("lis", Cmd_lens_zoom_init, "zoom init")
SXCMD_ITEM("lie", Cmd_lens_focus_iris_init, "focus and iris init")
SXCMD_ITEM("lretract", Cmd_lens_retract, "lens retract")
SXCMD_ITEM("rpi", Cmd_lens_rpi, "check zpi and fpr")
SXCMD_ITEM("gzs", Cmd_lens_gzs, "check current zoom position")
SXCMD_END()

BOOL Cmd_lens_zoom_init(CHAR* strCmd)
{
    Lens_Init(LENS_INIT_ZOOM_PART1);
    Lens_Init(LENS_INIT_ZOOM_PART2);
    debug_msg("Zoom Init...\r\n");

    return TRUE;
}

BOOL Cmd_lens_focus_iris_init(CHAR* strCmd)
{
    Lens_Init(LENS_INIT_APERTURE);
    Lens_Init(LENS_INIT_FOCUS);
    debug_msg("Focus_iris Init...\r\n");

    return TRUE;
}

BOOL Cmd_lens_retract(CHAR* strCmd)
{
    Lens_Retract();
    debug_msg("Lens Retract...\r\n");

    return TRUE;
}

BOOL Cmd_lens_rpi(CHAR* strCmd)
{
    debug_msg("^R check signal ZPR&FPI \r\n");
    LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
    LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);
    LensCtrl_SetSignal(MD_SIGNAL_FPI,ON);

    Delay_DelayMs(10); // Delay to get stable signals.

    debug_err(("zpr=%d\r\n",Lens_CheckLDSignal(LD_SIGNAL_ZPR)));
    debug_err(("fpi=%d\r\n",Lens_CheckLDSignal(LD_SIGNAL_FPI)));

    LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
    LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
    LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

    return TRUE;
}

BOOL Cmd_lens_gzs(CHAR* strCmd)
{
    debug_err(("Get Zoom Step= #%d, *%d, %d\r\n", Lens_Zoom_GetSection(ZOOM_CUR_SECTION),
                                                              Lens_Zoom_GetStep(ZOOM_CUR_SECTION_STEP),
                                                              Lens_Zoom_GetStep(ZOOM_CUR_STEP)));

    return TRUE;
}

