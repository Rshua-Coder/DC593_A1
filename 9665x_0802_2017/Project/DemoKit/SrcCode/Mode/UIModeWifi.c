#if defined(__ECOS)

#include "UIMenuWndWiFiModuleLink.h"
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
#include "UIMenuWndWiFiMobileLinkOK.h"
//#NT#2016/03/23#Isiah Chang -end
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "SysMain.h"
#include "UIAppPhoto.h"


void UI_OpenWifiWnd(void)
{
    if(System_GetState(SYS_STATE_CURRSUBMODE)!=System_GetState(SYS_STATE_PREVSUBMODE))
    {
        //#NT#2016/03/23#Isiah Chang -begin
        //#NT#add new Wi-Fi UI flow.
        #if(WIFI_UI_FLOW_VER == WIFI_UI_VER_1_0)
            Ux_OpenWindow(&UIMenuWndWiFiModuleLinkCtrl,0);
        #elif(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
            Ux_OpenWindow(&UIMenuWndWiFiMobileLinkOKCtrl,0);
        #endif
        //#NT#2016/03/23#Isiah Chang -end
    }
}

void UI_CloseWifiWnd(void)
{
    if(System_GetState(SYS_STATE_CURRSUBMODE)!=System_GetState(SYS_STATE_NEXTSUBMODE))
    {
        //#NT#2016/03/23#Isiah Chang -begin
        //#NT#add new Wi-Fi UI flow.
        #if(WIFI_UI_FLOW_VER == WIFI_UI_VER_1_0)
            Ux_CloseWindow(&UIMenuWndWiFiModuleLinkCtrl,0);
        #elif(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
            Ux_CloseWindow(&UIMenuWndWiFiMobileLinkOKCtrl,0);
        #endif
        //#NT#2016/03/23#Isiah Chang -end
    }
}

void UIMenuWndWiFi_Photo_OnOpen(void)
{
    UINT32 StartFunc = 0;

    /* UIFlowWndPhoto_InitCfgSetting() */
    Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_SELFTIMER,   1, SysGetFlag(FL_SELFTIMER));
    Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_QUALITY,     1, SysGetFlag(FL_QUALITY));
    Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_ISO,         1, SysGetFlag(FL_ISO));
    Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_EV,          1, SysGetFlag(FL_EV));
    Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_WB,          1, SysGetFlag(FL_WB));
    Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_COLOR,       1, SysGetFlag(FL_COLOR_EFFECT));
    Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_SHARPNESS,   1, SysGetFlag(FL_SHARPNESS));
    Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_FD,          1, SysGetFlag(FL_FD));
    Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_DATE_PRINT,  1, SysGetFlag(FL_DATE_STAMP));
    Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_SIZE,        1, SysGetFlag(FL_PHOTO_SIZE));

    /* UIFlowWndPhoto_OnExeSetImageRation() */
    if (UI_GetData(FL_PHOTO_SIZE)==PHOTO_SIZE_2MHD)
    {
         Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_IMAGE_RATIO,1,IMAGERATIO_16_9);
    } else {
         Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_IMAGE_RATIO,1,IMAGERATIO_4_3);
    }

    // set FL_CONTINUE_SHOT to off
    SysSetFlag(FL_CONTINUE_SHOT,CONTINUE_SHOT_OFF);

    /* FlowPhoto_InitStartupFuncs() */
    StartFunc |=UIAPP_PHOTO_AE;
    StartFunc |=UIAPP_PHOTO_AWB;
    Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_START_FUNC, 2, StartFunc, UIAPP_PHOTO_NOWAITIDLE);
}

#else
void UI_OpenWifiWnd(void)
{
}

void UI_CloseWifiWnd(void)
{
}

void UIMenuWndWiFi_Photo_OnOpen(void)
{
}
#endif
