#include "SysCommon.h"
#include "GxFlash.h" //for GxFlash_SuspendCharge(), GxFlash_ResumeCharge()
#include "UIInfo.h" //for UI_SetData(), UI_GetData()
#include "UIMode.h"
#include "UIModePhoto.h"
#include "UIAppPhoto.h"
#include "Lens.h"
#include "LensInfo.h"
#include "SwTimer.h"

#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DevCtrlLens
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

/////////////////////////////////////////////////////////////////////////////
// Power On Flow
/////////////////////////////////////////////////////////////////////////////

//#NT#2010/08/02#Jeah Yen -begin
void Lens_PowerOn_Load(void)
{
    #if (LENS_FUNCTION == ENABLE)
    //  Read lens init flag and set it.
    UINT32 lensFlag=0;
    lensFlag = Load_LensInfo();
    Set_LensFlag(lensFlag);
    #endif
}

void Lens_PowerOn_Apply(void)
{
    #if (LENS_FUNCTION == ENABLE)
    //if(System_GetState(SYS_STATE_POWERON) != SYS_POWERON_LOWPOWER)
    {
        #if _DEMO_TODO
        // should wait pstore init and read calibration data
        //#NT#2010/09/17#Jeah Yen -begin
        TM_Begin(LENS_WAITPST);
        INIT_WAITFLAG(FLGINIT_BEGINCOPYPART2); //wait until PST finish and Info is loaded
        TM_End(LENS_WAITPST);
        //#NT#2010/09/17#Jeah Yen -end
        TM_Begin(LENSINFO_APPLY);
        //apply lens info
        ApplyCalData(CAL_LENS_DATA);
        TM_End(LENSINFO_APPLY);
        #endif
    }
    #endif
}
void Lens_PowerOn_ChangeMode(void)
{
    #if (LENS_FUNCTION == ENABLE)
    if(System_GetState(SYS_STATE_POWERON) == SYS_POWERON_NORMAL)
    {
        INT32 preview_mode;

        preview_mode = FlowMode_IsPowerOnPreview();
        if(preview_mode == 1) //photo mode
            FlowPhoto_Open_FastLens();
    }
    #endif
}

//#NT#2010/08/02#Jeah Yen -end

/////////////////////////////////////////////////////////////////////////////



BOOL gLensNeedAdvance = FALSE;

BOOL DevCtrl_Lens_PowerOn_IsUnderAdvance(void)
{
    #if (LENS_FUNCTION == ENABLE)
    return gLensNeedAdvance;
    #else
    return 0;
    #endif
}

void DevCtrl_Lens_PowerOn_Advance(void)
{
    //TM_Begin(LENS_ZOOM);
    #if (LENS_FUNCTION == ENABLE)
    Lens_Init(LENS_INIT_ZOOM_PART1);
    gLensNeedAdvance = TRUE;
    #endif
}

/////////////////////////////////////////////////////////////////////////////
// Power Off Flow
/////////////////////////////////////////////////////////////////////////////

BOOL DevCtrl_Lens_PowerOff_IsNeedRetract(void)
{
    #if (LENS_FUNCTION == ENABLE)
    DBG_IND("\r\n");
    return ((Lens_Module_GetState() != LENS_STATE_NONE) || (Get_LensFlag() == ZOOM_FLAG_OUTSIDE));
    #else
    return 0;
    #endif
}

void DevCtrl_Lens_PowerOff_Retract(void)
{
    #if (LENS_FUNCTION == ENABLE)
    DBG_IND("\r\n");
    if (Lens_Retract()!=ERR_OK)
    {
        DBG_ERR("Lens error\r\n");
    }
    else if(!Get_LensFlag() && (!GxPower_GetControl(GXPWR_CTRL_IS_DUMMUYLOAD_POWEROFF)))
    {
        Lens_Init(LENS_INIT_ZOOM_PART1);
        Lens_Init(LENS_INIT_ZOOM_PART2);
        Lens_Init(LENS_INIT_APERTURE);
        if (Lens_Retract()!=ERR_OK)
        {
            DBG_ERR("Lens error2\r\n");
        }

    }
    #endif
}

void DevCtrl_Lens_PowerOff_Exit(void)
{
    #if (LENS_FUNCTION == ENABLE)
    #endif
}

/////////////////////////////////////////////////////////////////////////////
// Playback Mode Control Flow
/////////////////////////////////////////////////////////////////////////////

void DevCtrl_Lens_Playback_Retract(void)
{
    #if (LENS_FUNCTION == ENABLE)
    BOOL bLensInitErr = FALSE;

    // During lens retract, does not detect key
    SxTimer_SetFuncActive(SX_TIMER_DET_KEY_ID,FALSE);
    bLensInitErr = Lens_Retract();
    SxTimer_SetFuncActive(SX_TIMER_DET_KEY_ID,TRUE);
    if(bLensInitErr)
    {
        DBG_ERR("\r\n");
        #if (LENSERROFF_FUNCTION == ENABLE)
        System_PowerOff(SYS_POWEROFF_LENSERROR);
        #endif
    }
    UI_SetData(FL_LensOpen, FALSE);
    SxTimer_SetFuncActive(SX_TIMER_DET_CLOSE_LENS_ID,FALSE);
    #endif
}

/////////////////////////////////////////////////////////////////////////////
// Photo Mode Control Flow
/////////////////////////////////////////////////////////////////////////////

extern BOOL g_bPreviewFirstTime;
BOOL    g_bLensInitErr  = FALSE;

/**
  Control the devices for Photo mode

  Control the devices before entering Photo mode.

  @param void
  @return void
*/
BOOL DevCtrl_Lens(BOOL bIsOpen)
{
    #if (LENS_FUNCTION == ENABLE)
    BOOL returnValue = TRUE;
    GxFlash_SuspendCharge();
    if (bIsOpen)
    {
        if (!UI_GetData(FL_LensOpen) && !g_bPreviewFirstTime)
        {
            //TM_Begin(LENS_ZOOM);
            Lens_Init(LENS_INIT_ZOOM_PART1);
            UI_SetData(FL_LensOpen, TRUE);
            SxTimer_SetFuncActive(SX_TIMER_DET_CLOSE_LENS_ID,TRUE);
            Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_ZOOM,2, UI_ZOOM_CTRL_RESET_DZOOM, FALSE);

        }
        if(Lens_Module_GetState()!=LENS_STATE_IDLE)
        {
            BOOL    bLensInitErr  = TRUE;
            INT32   retValue = ERR_OK;
            retValue = Lens_Init(LENS_INIT_ZOOM_PART2);

            //TM_End(LENS_ZOOM);
            if (retValue == ERR_OK)
            {
                Lens_Init(LENS_INIT_APERTURE);

                //TM_Begin(LENS_FOCUS);
                if(Lens_Init(LENS_INIT_FOCUS)==ERR_OK)
                {
                    bLensInitErr = FALSE;
                }
                //TM_End(LENS_FOCUS);
            }
            else if(retValue == ERR_LENS_INIT_FAIL)
            {
                DBG_ERR("Initiate zoom part 2 fail, try again\r\n");

                DBG_IND("Lens retract\r\n");
                Lens_Retract();

                DBG_IND("Initiate zoom part 1\r\n");
                Lens_Init(LENS_INIT_ZOOM_PART1);
                UI_SetData(FL_LensOpen, TRUE);
                SxTimer_SetFuncActive(SX_TIMER_DET_CLOSE_LENS_ID,TRUE);
                DBG_IND("Initiate zoom part 2\r\n");

                if (Lens_Init(LENS_INIT_ZOOM_PART2) == ERR_OK)
                {
                    DBG_IND("Initiate iris\r\n");

                    Lens_Init(LENS_INIT_APERTURE);

                    DBG_IND("Initiate focus\r\n");
                    if (Lens_Init(LENS_INIT_FOCUS) == ERR_OK)
                    {
                        bLensInitErr = FALSE;
                    }
                    else
                    {
                        bLensInitErr = TRUE;
                    }
                }
                else
                {
                    bLensInitErr = TRUE;
                }

            }
            if(bLensInitErr)
            {
                #if (LENSERROFF_FUNCTION == ENABLE)
                DBG_ERR("Initiation fail -> power off \r\n");
                g_bLensInitErr = TRUE;
                System_PowerOff(SYS_POWEROFF_LENSERROR);
                returnValue = FALSE;
                #endif
            }
        }
    }
    else
    {
        //#NT#Modified., whiling Startup with Photo but usb plugin quickly, lens not retract.
        if (UI_GetData(FL_LensOpen))
        {
            if(Lens_Module_GetState()==LENS_STATE_INIT_PART1)
            {
                INT32 nRetry = 20; //Retry 2 sec
                while((Lens_Module_GetState()==LENS_STATE_INIT_PART1) && (nRetry-- >= 0))
                {
                    SwTimer_DelayMs(10);
                }

                if(nRetry<0)
                    DBG_ERR("lens init time out!\r\n");
            }
            if(Lens_Retract()!=ERR_OK)
            {
                DBG_ERR("Lens error3\r\n");
                #if (LENSERROFF_FUNCTION == ENABLE)
                System_PowerOff(SYS_POWEROFF_LENSERROR);
                returnValue = FALSE;
                #endif
            }

            UI_SetData(FL_LensOpen, FALSE);
            SxTimer_SetFuncActive(SX_TIMER_DET_CLOSE_LENS_ID,FALSE);

        }
        // Fix USB power on can't retract lens if previous lens is outside.
        else if(!Get_LensFlag() && (!GxPower_GetControl(GXPWR_CTRL_IS_DUMMUYLOAD_POWEROFF)))
        {
            Lens_Init(LENS_INIT_ZOOM_PART1);
            Lens_Init(LENS_INIT_ZOOM_PART2);
            Lens_Init(LENS_INIT_APERTURE);

            if(Lens_Retract()!=ERR_OK)
            {
                DBG_ERR("Lens error4\r\n");
            }
        }
    }
    GxFlash_ResumeCharge();
    return returnValue;

    #else
    return TRUE;
    #endif
}
