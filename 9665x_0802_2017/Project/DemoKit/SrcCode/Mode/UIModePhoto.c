////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModePhoto.h"
#include "UIAppPhoto.h"
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

//UIInfo
#include "UIInfo.h"
#if (UI_STYLE==UI_STYLE_DRIVER)
#include "UIFlow.h"
#else
#include "FlowPhoto.h"
#include "FlowASPSCS.h"
#endif
#include "UIModeWiFi.h"
#include "FlowPanInit.h"
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
#include "UIGraphics.h"
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
#include "DeviceCtrl.h"
#if _DEMO_TODO
#include "PhotoTsk.h"
#include "ImgCaptureAPI.h"
//#NT#2010/07/26#Jeah Yen - begin
#include "PhotoDisplay2.h"
#include "GxFlash.h"
//#NT#2010/07/26#Jeah Yen - end
//#NT#2010/12/09#Jeffery Chuang -begin
//CAF Move to AFTsk
#include "AFTsk.h"
//#NT#2010/12/09#Jeffery Chuang -end
#include "PhotoDisTsk.h"
#endif

#if (_CALIBRATION_MODE_ == ENABLE)
#include "EngineerMode.h"
#endif

//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
extern INT32 PhotoExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern BOOL g_bPreviewFirstTime;
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
#if(_USR_WND_==ENABLE)
static UINT32 gWndID= 0;
#endif

int PRIMARY_MODE_PHOTO = -1;      ///< Photo


//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
void FlowPhoto_Open_FastSensor(void)
{
#if _DEMO_TODO
    // should wait until part-2 finish
    //#NT#2010/09/17#Jeah Yen -begin
    //TM_Begin(CCD_WAITP2);
    INIT_WAITFLAG(FLGINIT_ENDCOPYPART2);
    //TM_End(CCD_WAITP2);
    //#NT#2010/09/17#Jeah Yen -end
#endif

    //speed up: 提前做change mode時sensor要做的事情
    g_bPreviewFirstTime = TRUE; //turn on fast path
    //TM_Begin(APP_OPEN);
    // Init Photo app and open
    PhotoExe_OnOpen(0, 0, 0);
    //TM_End(APP_OPEN);

    //TM_Begin(DEVCTRL_CCD);
    DevCtrl_ModePhoto_FastSensor();
    //TM_End(DEVCTRL_CCD);
    DevCtrl_ModePhoto_WaitFinish();
}

void FlowPhoto_Open_FastLens(void)
{
    //TM_Begin(DEVCTRL_LENS);
    //Lens_OnSystem(LENS_CMD_POWERON_ADVANCE);
    DevCtrl_Lens_PowerOn_Advance(); //simple path

    //speed up: 提前做change mode時lens要做的事情
    g_bPreviewFirstTime = TRUE; //turn on fast path
    DevCtrl_ModePhoto_FastLens();
    //TM_End(DEVCTRL_LENS);
}
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end

void ModePhoto_Open(void);
void ModePhoto_Close(void);


void ModePhoto_Open(void)
{
    //--------------------------------------------------------------------------
    // The open flow should be: 1. DevCtrl 2. AppOpen 3. UIOpen
    //--------------------------------------------------------------------------

    Input_ResetMask();

    // Init Flow Window config for different DSC modes
    Ux_SetActiveApp(&CustomPhotoObjCtrl);
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
    //#NT#2010/03/02#JeahYen -begin
    if(g_bPreviewFirstTime)
    {
        //skip
        //GxSystem_Mark("APP-SKIP\r\n");
    }
    else
    {
        //TM_Begin(DEVCTRL_CCD);
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModePhoto();
        //TM_End(DEVCTRL_CCD);
        //TM_Begin(APP_OPEN);
        // Init Photo app and open
        Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);
        //TM_End(APP_OPEN);

    }
    if(!g_bPreviewFirstTime)
    {
        DevCtrl_ModePhoto_WaitFinish();
    }

    //TM_Begin(UI_OPENWIN);
    UI_SetData(FL_ModeIndex, UI_GetData(FL_NextMode));
/*
    //disable video1
    UI_Show(UI_SHOW_PREVIEW, TRUE);
    //enable video2
    UI_Show(UI_SHOW_QUICKVIEW, FALSE);
*/
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        UI_OpenWifiWnd();
        UIMenuWndWiFi_Photo_OnOpen();  // Init Photo mode related parameters.
    }
    #if (_CALIBRATION_MODE_ == ENABLE)
    else if (IsEngineerModeOpened())
    {
        EngineerMode_WndOpen();
    }
    #endif
    else
    {
#if(_USR_WND_==ENABLE)
    switch(UI_GetData(FL_ModeIndex))
    {
        case DSC_MODE_PHOTO_PANORAMA:
            Ux_OpenWindow((VControl *)(&FlowPanInitCtrl), 0);   //Test ASPSCS
            gWndID = (UINT32)&FlowPanInitCtrl;
            break;
        //#NT#2010/10/12#Photon Lin -begin
        //#Add AS/PS/CS mode
        #if (ASPSCS_FUNCTION==ENABLE)
        case DSC_MODE_PHOTO_PORTRAIT:
            Ux_OpenWindow((VControl *)(&FlowASPSCSCtrl), 0);   //Test ASPSCS
            gWndID = (UINT32)&FlowASPSCSCtrl;
            break;
        #endif
        //#NT#2010/10/12#Photon Lin -begin
        default:
            Ux_OpenWindow((VControl *)(&FlowPhotoCtrl), 0);
            gWndID = (UINT32)&FlowPhotoCtrl;
            break;
    }
#else
        Ux_OpenWindow((VControl *)(&UIFlowWndPhotoCtrl), 0);
#endif
    //TM_End(UI_OPENWIN);
    }

    g_bPreviewFirstTime = FALSE; //turn off fast path
    //TM_End(OFF_TO_CAPTURE);
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
}
void ModePhoto_Close(void)
{
    //--------------------------------------------------------------------------
    // The close flow should be: 1. UIClose 2. AppClose 3. DevCtrl
    //--------------------------------------------------------------------------
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        UI_CloseWifiWnd();
    }
    #if (_CALIBRATION_MODE_ == ENABLE)
    else if (IsEngineerModeOpened())
    {
        EngineerMode_WndClose();
    }
    #endif
    else
    {
#if(_USR_WND_==ENABLE)
        Ux_CloseWindowClear((VControl *)(gWndID), 0);
#else
        Ux_CloseWindowClear((VControl *)(&UIFlowWndPhotoCtrl), 0);
#endif
    }
    Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);
}

void FlowPhoto_OnChangeSize(ISIZE *pSize, ISIZE *pOldSize);

SYS_MODE gModePhoto =
{
    "PHOTO",
    ModePhoto_Open,
    ModePhoto_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
