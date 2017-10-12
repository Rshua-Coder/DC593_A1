/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       LensCtrlTsk.c
    @ingroup    mIPRJAPCfg

    @brief      Lens control task
                Handle event when external interrupt is issued

    @note       Nothing.

    @date       2012/10/5
*/


#include "Kernel.h"
#include <string.h>
#include "LensAPI.h"
#include "LensCtrlTsk.h"
#include "Utility.h"
#include "SysKer.h"
#include "LensCtrlFlag.h"
#include "IQS_SettingFlow.h"
#include "sie_lib.h"  //Must remove

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          LENCTRLTSK2
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

static LENSCTRL_APPOBJ      gLensCtrlObj;
static BOOL bLensCtrl2Opened = FALSE;
static BOOL bLensCtrlBGOpened = FALSE;

static UINT32 uiLensStatus = 0;

//Add focus moving during zoomin/out
static UINT32 uiFocusStep = 0;
//static UINT32 uiZoomSection = 1;


ER LensCtrl2_Open(PLENSCTRL_APPOBJ pLensCtrlObj)
{
    if (bLensCtrl2Opened == TRUE)
    {
        return E_SYS;
    }
    clr_flg(FLG_ID_LENS2, FLGLENS2_ALL);
    bLensCtrl2Opened = TRUE;

    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk2_writelensstatus = LensCtrl2_WriteLensStatus;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk2_readlensstatus = LensCtrl2_ReadLensStatus;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltsk2_waitidle = LensCtrl2_WaitIdle;

    memcpy(&gLensCtrlObj, pLensCtrlObj, sizeof(LENSCTRL_APPOBJ));
    sta_tsk(LENSCTRLTSK2_ID, 0);

    return E_OK;
}

ER LensCtrl2_Close(void)
{
    if (bLensCtrl2Opened == FALSE)
    {
        return E_SYS;
    }
    bLensCtrl2Opened = FALSE;

    ter_tsk(LENSCTRLTSK2_ID);

    return E_OK;
}

void LensCtrlTsk2(void)
{
    FLGPTN Flag;
    FLGPTN Mask;

    kent_tsk();
//Add focus moving during zoomin/out
    Mask = (FLGLENS2_WRITE_LENS_STATUS|FLGLENS2_READ_LENS_STATUS|FLGLENS2_FOCUSMOVE);

    while(1)
    {
        set_flg(FLG_ID_LENS2,FLGLENS2_IDLE);
        wai_flg(&Flag, FLG_ID_LENS2, Mask, TWF_ORW|TWF_CLR);
        clr_flg(FLG_ID_LENS2,FLGLENS2_IDLE);

        if(Flag & FLGLENS2_WRITE_LENS_STATUS)
        {
            if(gLensCtrlObj.Task2CB!=NULL)
            {
                gLensCtrlObj.Task2CB(LENS_CB_WRITE_STATUS, &uiLensStatus);
            }
        }

        if(Flag & FLGLENS2_READ_LENS_STATUS)
        {
            if(gLensCtrlObj.Task2CB!=NULL)
            {
                gLensCtrlObj.Task2CB(LENS_CB_READ_STATUS, &uiLensStatus);
            }
        }
        if(Flag & FLGLENS2_FOCUSMOVE)
        {
            if(gLensCtrlObj.Task2CB!=NULL)
            {
                gLensCtrlObj.Task2CB(LENS_CB_FOCUSMOVE, &uiFocusStep);
            }
        }
    }
}

void LensCtrl2_WriteLensStatus(UINT32 status)
{
    uiLensStatus = status;
    set_flg(FLG_ID_LENS2,FLGLENS2_WRITE_LENS_STATUS);
}

UINT32 LensCtrl2_ReadLensStatus(void)
{
    INT32  i = 1000;
    set_flg(FLG_ID_LENS2,FLGLENS2_READ_LENS_STATUS);

    //Due to task may be switched,
    //so using time out method to make sure flow of FLGLENS2_READ_LENS_STATUS must be finished
    for(;i>0;i--)
    {
        Delay_DelayMs(1);
        if (kchk_flg(FLG_ID_LENS2,FLGLENS2_IDLE))
        {
            break;
        }
        if(i==0)
        {
            DBG_ERR("Time out!\r\n");
        }
    }
    return uiLensStatus;

}

#if 0
void LensCtrl2_FocusGobyZoomStep(UINT32 FocusStep,UINT32 ZoomSection)
{
    uiFocusStep = FocusStep;
    if(uiZoomSection != ZoomSection)
    {
        uiZoomSection = ZoomSection;
        set_flg(FLG_ID_LENS2,FLGLENS2_FOCUSMOVE);
    }
}
#endif

void LensCtrl2_WaitIdle(void)
{
    FLGPTN  Flag;
    wai_flg(&Flag, FLG_ID_LENS2, FLGLENS2_IDLE , TWF_ORW);
}

ER LensCtrlBG_Open(PLENSCTRL_APPOBJ pLensCtrlObj)
{
    if (bLensCtrlBGOpened == TRUE)
    {
        return E_SYS;
    }
    clr_flg(FLG_ID_LENSBG, FLGLENSBG_ALL);
    bLensCtrlBGOpened = TRUE;
    //memcpy(&gLensCtrlObj, pLensCtrlBGObj, sizeof(LENSCTRL_APPOBJ));
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltskBG_waitidle = LensCtrlBG_WaitIdle;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltskBG_checkidle = LensCtrlBG_CheckIdle;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltskBG_iqs_setflowfunc = LensCtrlBG_WaitIdle;
    pLensCtrlObj->pLens->pLensCtrlTskObj->lensctrltskBG_focus_afterzoomchanged = LensCtrlBG_Focus_AfterZoomChanged;
    sta_tsk(LENSCTRLTSKBG_ID, 0);

    return E_OK;
}

ER LensCtrlBG_Close(void)
{
    if (bLensCtrlBGOpened == FALSE)
    {
        return E_SYS;
    }
    bLensCtrlBGOpened = FALSE;

    ter_tsk(LENSCTRLTSKBG_ID);

    return E_OK;
}

void LensCtrlBGTsk(void)
{
    FLGPTN Flag;
    FLGPTN Mask;

    kent_tsk();

    DBG_ERR("#LensCtrlBGTsk\r\n");
    Mask = (FLGLENSBG_IQS_SETFLOWFUNC | FLGLENSBG_FOCUS_AFTER_ZOOM_CHANGED);

    while(1)
    {
        set_flg(FLG_ID_LENSBG, FLGLENSBG_IDLE);
        DBG_MSG("^YBG_idle\r\n");
        wai_flg(&Flag, FLG_ID_LENSBG, Mask, TWF_ORW|TWF_CLR);
        clr_flg(FLG_ID_LENSBG, FLGLENSBG_IDLE);
        DBG_MSG("^YBG_busy\r\n");

        if(Flag & FLGLENSBG_IQS_SETFLOWFUNC)
        {
            // Do IQ DCE.
            IQS_SettingFlowFunc(IQS_OZOOM_END);
            DBG_ERR("#IQS_SettingFlowFunc\r\n");
        }

        if(Flag & FLGLENSBG_FOCUS_AFTER_ZOOM_CHANGED)
        {
            // Do focus once zoom changed.
            DBG_MSG("gfzc\r\n");
            Lens_Focus_AfterZoomChanged();
            DBG_MSG("#FAZC - Z%02d, F:%d\r\n", Lens_Zoom_GetSection(ZOOM_CUR_SECTION), Lens_Focus_GetPosition());
        }
    }
}

void LensCtrlBG_WaitIdle(void)
{
    FLGPTN  Flag;
    wai_flg(&Flag, FLG_ID_LENSBG, FLGLENSBG_IDLE , TWF_ORW);
}

BOOL LensCtrlBG_CheckIdle(void)
{
    return kchk_flg(FLG_ID_LENSBG, FLGLENSBG_IDLE);
}

void LensCtrlBG_Iqs_SetFlowFunc(void)
{
    set_flg(FLG_ID_LENSBG, FLGLENSBG_IQS_SETFLOWFUNC);
}

void LensCtrlBG_Focus_AfterZoomChanged(void)
{
    set_flg(FLG_ID_LENSBG, FLGLENSBG_FOCUS_AFTER_ZOOM_CHANGED);
    DBG_MSG("sfzc\r\n");
}

