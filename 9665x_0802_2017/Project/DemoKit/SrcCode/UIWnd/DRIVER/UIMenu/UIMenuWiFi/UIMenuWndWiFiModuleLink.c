//This source code is generated by UI Designer Studio.
#ifdef __ECOS
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "NVTToolCommand.h"
#include "UIMenuWndWiFiModuleLinkRes.c"
#include "UIFlow.h"
#include "UIAppNetwork.h"
extern UIMenuStoreInfo  currentInfo;

//---------------------UIMenuWndWiFiModuleLinkCtrl Debug Definition -----------------------------
#define _UIMENUWNDWIFIMODULELINK_ERROR_MSG        1
#define _UIMENUWNDWIFIMODULELINK_TRACE_MSG        0

#if (_UIMENUWNDWIFIMODULELINK_ERROR_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_ERR))
#define UIMenuWndWiFiModuleLinkErrMsg(...)            debug_msg ("^R UIMenuWndWiFiModuleLink: "__VA_ARGS__)
#else
#define UIMenuWndWiFiModuleLinkErrMsg(...)
#endif

#if (_UIMENUWNDWIFIMODULELINK_TRACE_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_TRC))
#define UIMenuWndWiFiModuleLinkTraceMsg(...)          debug_msg ("^B UIMenuWndWiFiModuleLink: "__VA_ARGS__)
#else
#define UIMenuWndWiFiModuleLinkTraceMsg(...)
#endif


#define MENU_WIFI_PRESS_MASK        (FLGKEY_KEY_MASK_DEFAULT)
#define MENU_WIFI_RELEASE_MASK      (FLGKEY_MODE |FLGKEY_MENU |FLGKEY_SHUTTER2) 
#define MENU_WIFI_CONTINUE_MASK     FLGKEY_KEY_CONT_MASK_DEFAULT


//---------------------UIMenuWndWiFiModuleLinkCtrl Global Variables -----------------------------
//---------------------UIMenuWndWiFiModuleLinkCtrl Prototype Declaration  -----------------------

//---------------------UIMenuWndWiFiModuleLinkCtrl Public API  ----------------------------------

//---------------------UIMenuWndWiFiModuleLinkCtrl Private API  ---------------------------------
void UIMenuWndWiFiModuleLink_UpdateData(void);

//---------------------UIMenuWndWiFiModuleLinkCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndWiFiModuleLink)
CTRL_LIST_ITEM(UIMenuWndWiFiModuleLink_StaticICN_WiFi)
CTRL_LIST_ITEM(UIMenuWndWiFiModuleLink_StatusTXT_WiFiMode)
CTRL_LIST_ITEM(UIMenuWndWiFiModuleLink_StaticTXT_SSID)
CTRL_LIST_ITEM(UIMenuWndWiFiModuleLink_StaticTXT_Key)
CTRL_LIST_ITEM(UIMenuWndWiFiModuleLink_StaticTXT_Cancel)
CTRL_LIST_END

//----------------------UIMenuWndWiFiModuleLinkCtrl Event---------------------------
INT32 UIMenuWndWiFiModuleLink_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiModuleLink_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiModuleLink_OnChildClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiModuleLink_OnBattery(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiModuleLink_OnTimer(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiModuleLink_OnMovieFull(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiModuleLink_OnMovieWrError(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiModuleLink_OnStorageSlow(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiModuleLink_OnDeleteFiles(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiModuleLink_OnDeleteROFiles(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiModuleLink_OnWiFiState(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiModeLink_Tab_Authorized_OK(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiModeLink_Wifi_Close(VControl *, UINT32, UINT32 *);



EVENT_BEGIN(UIMenuWndWiFiModuleLink)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndWiFiModuleLink_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndWiFiModuleLink_OnClose)
EVENT_ITEM(NVTEVT_CHILD_CLOSE,UIMenuWndWiFiModuleLink_OnChildClose)
EVENT_ITEM(NVTEVT_BATTERY,UIMenuWndWiFiModuleLink_OnBattery)
EVENT_ITEM(NVTEVT_TIMER,UIMenuWndWiFiModuleLink_OnTimer)
EVENT_ITEM(NVTEVT_CB_MOVIE_OVERTIME,UIMenuWndWiFiModuleLink_OnMovieFull) // the same handling as storage full (may need to show special message)
EVENT_ITEM(NVTEVT_CB_MOVIE_FULL,UIMenuWndWiFiModuleLink_OnMovieFull)
EVENT_ITEM(NVTEVT_CB_MOVIE_LOOPREC_FULL,UIMenuWndWiFiModuleLink_OnMovieFull)
EVENT_ITEM(NVTEVT_CB_MOVIE_WR_ERROR,UIMenuWndWiFiModuleLink_OnMovieWrError)
EVENT_ITEM(NVTEVT_CB_MOVIE_SLOW,UIMenuWndWiFiModuleLink_OnStorageSlow)
EVENT_ITEM(NVTEVT_CB_MOVIE_DELETE_FILES,UIMenuWndWiFiModuleLink_OnDeleteFiles)
EVENT_ITEM(NVTEVT_CB_MOVIE_DELETE_RO_FILES,UIMenuWndWiFiModuleLink_OnDeleteROFiles)
EVENT_ITEM(NVTEVT_WIFI_STATE,UIMenuWndWiFiModuleLink_OnWiFiState)
EVENT_ITEM(NVTEVT_WIFI_AUTHORIZED_OK,UIMenuWndWiFiModeLink_Tab_Authorized_OK)
EVENT_ITEM(NVTEVT_KEY_SHUTTER2,UIMenuWndWiFiModeLink_Wifi_Close)
EVENT_END



void UIMenuWndWiFiModuleLink_UpdateData(void)
{
    static char buf1[32],buf2[32];
    char *pMacAddr;

    if(UI_GetData(FL_WIFI_LINK)==WIFI_LINK_OK)
    {
        if(UI_GetData(FL_NetWorkMode)==NETWORK_AP_MODE || UI_GetData(FL_NetWorkMode)==NET_STATION_MODE)
        {
            if(currentInfo.strSSID[0] == 0)
            {
                #if (MAC_APPEN_SSID==ENABLE)
                pMacAddr = (char*)UINet_GetMAC();
                sprintf(buf1,"SSID:%s% 02x%02x%02x%02x%02x%02x", UINet_GetSSID() ,pMacAddr[0], pMacAddr[1], pMacAddr[2]
                                                            ,pMacAddr[3], pMacAddr[4], pMacAddr[5]);
                #else
                sprintf(buf1,"SSID:%s",UINet_GetSSID());
                #endif
            }
            else
            {
                sprintf(buf1,"SSID:%s",UINet_GetSSID());
            }


            sprintf(buf2,"PWD:%s",UINet_GetPASSPHRASE());
            UxStatic_SetData(&UIMenuWndWiFiModuleLink_StaticTXT_SSIDCtrl,STATIC_VALUE,Txt_Pointer(buf1));
            UxStatic_SetData(&UIMenuWndWiFiModuleLink_StaticTXT_KeyCtrl,STATIC_VALUE,Txt_Pointer(buf2));
        }
    }
    else
    {
        sprintf(buf1,"%s:Fail",TXT_WIFI_CONNECT);
        UxStatic_SetData(&UIMenuWndWiFiModuleLink_StaticTXT_SSIDCtrl,STATIC_VALUE,Txt_Pointer(buf1));
        UxStatic_SetData(&UIMenuWndWiFiModuleLink_StaticTXT_KeyCtrl,STATIC_VALUE,STRID_NULL);
    }
}
INT32 UIMenuWndWiFiModuleLink_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	Input_SetKeyMask(KEY_PRESS, MENU_WIFI_PRESS_MASK);
    Input_SetKeyMask(KEY_RELEASE, MENU_WIFI_RELEASE_MASK);
    Input_SetKeyMask(KEY_CONTINUE, MENU_WIFI_CONTINUE_MASK);	
	UxStatic_SetData(&UIMenuWndWiFiModuleLink_StaticTXT_CancelCtrl,STATIC_VALUE,STRID_CANCEL);	//drh 20170919
    UIMenuWndWiFiModuleLink_UpdateData();
    //UxTab_SetData(&UIMenuWndWiFiModeLink_Tab_RefreshAndWiFiOffCtrl, TAB_FOCUS, 0);
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiModuleLink_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiModuleLink_OnChildClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiModuleLink_UpdateData();
    Ux_DefaultEvent(pCtrl,NVTEVT_CHILD_CLOSE,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiModuleLink_OnBattery(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiModuleLink_OnTimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_TIMER,paramNum,paramArray);
    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiModuleLink_OnWiFiState(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(paramNum==2)
    {
        if(UI_GetData(FL_NetWorkMode) == NET_AP_MODE)
        {
            UIMenuWndWiFiModuleLinkTraceMsg("ap  state %d result %d\r\n",paramArray[0],paramArray[1]);
            UINT32 state = paramArray[0];
            INT32 result = paramArray[1];
            // wifi phase status
            if(state == NET_STATE_WIFI_CB)
            {
                if(result == NVT_WIFI_AP_READY)
                {
                    UIMenuWndWiFiModuleLinkTraceMsg("ap mode OK\r\n");
                }
                else if(result == NVT_WIFI_LINK_STATUS_CONNECTED)
                {
                	debug_msg("^Y drh ap NVT_WIFI_LINK_STATUS_CONNECTED\r\n");
                    UIMenuWndWiFiModuleLinkTraceMsg("ap state %d result %d\r\n",paramArray[0],paramArray[1]);
                }

            }
        }
        else // station mode
        {
            UIMenuWndWiFiModuleLinkTraceMsg("sta  state %d result %d\r\n",paramArray[0],paramArray[1]);
            UINT32 state = paramArray[0];
            INT32 result = paramArray[1];

            if(state == NET_STATE_DHCP_START)
            {
                if(result == 0)
                {
                    UIMenuWndWiFiModuleLinkTraceMsg("station mode OK\r\n");
                }
                else
                {
                    UIMenuWndWiFiModuleLinkTraceMsg("DHCP fail or wifi module failed\r\n");
                }
            }
            else if (state == NET_STATE_WIFI_CB)
            {
                if (result == NVT_WIFI_LINK_STATUS_CONNECTED)
                {
					debug_msg("^G drh NVT_WIFI_LINK_STATUS_CONNECTED\r\n");
                }
                else
                {
					debug_msg("^G drh NVT_WIFI_LINK_STATUS_DISCONNECTED\r\n");
                }
            }
            else if(state == NET_STATE_WIFI_CONNECTING)
            {
                if(result == NET_CONNECTING_TIMEOUT)
                {
                    UIMenuWndWiFiModuleLinkTraceMsg("wifi module failed\r\n");
                }
            }
        }
    }
    else
    {
        UIMenuWndWiFiModuleLinkErrMsg("err paramNum %d\r\n",paramNum);
    }
    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiModeLink_Wifi_Close(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32  uiKeyAct;
	
    uiKeyAct = paramArray[0];

	debug_msg("^Y UIMenuWndWiFiModeLink_Wifi_Close\r\n");
	switch (uiKeyAct)
    {
      	case NVTEVT_KEY_PRESS:
			debug_msg("^Y UIMenuWndWiFiModeLink_Wifi_Close2\r\n");
			UI_SetData(FL_WIFI_LINK,WIFI_LINK_DISCONNECTING);
 	 		Ux_SendEvent(0,NVTEVT_EXE_WIFI_STOP,0);
   			Ux_PostEvent(NVTEVT_SYSTEM_MODE, 2, System_GetState(SYS_STATE_CURRMODE),SYS_SUBMODE_NORMAL);
			break;
	}
}

//----------------------UIMenuWndWiFiModuleLink_StaticICN_WiFiCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndWiFiModuleLink_StaticICN_WiFi)
EVENT_END

//----------------------UIMenuWndWiFiModuleLink_StatusTXT_WiFiModeCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndWiFiModuleLink_StatusTXT_WiFiMode)
EVENT_END

//----------------------UIMenuWndWiFiModuleLink_StaticTXT_SSIDCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndWiFiModuleLink_StaticTXT_SSID)
EVENT_END

//----------------------UIMenuWndWiFiModuleLink_StaticTXT_KeyCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndWiFiModuleLink_StaticTXT_Key)
EVENT_END

//----------------------UIMenuWndWiFiModuleLink_StaticTXT_CancelCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndWiFiModuleLink_StaticTXT_Cancel)
EVENT_END

//----------------------UIMenuWndWiFiModeLink_Tab_RefreshAndWiFiOffCtrl Event---------------------------
INT32 UIMenuWndWiFiModeLink_Tab_RefreshAndWiFiOff_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiModeLink_Tab_RefreshAndWiFiOff_OnKeyRight(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiModeLink_Tab_RefreshAndWiFiOff_OnKeyEnter(VControl *, UINT32, UINT32 *);


INT32 UIMenuWndWiFiModeLink_Tab_RefreshAndWiFiOff_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiKeyAct;

    uiKeyAct = paramArray[0];
    switch(uiKeyAct)
    {
    case NVTEVT_KEY_PRESS:
        Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
        break;
    }

    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiModeLink_Tab_RefreshAndWiFiOff_OnKeyRight(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiKeyAct;

    uiKeyAct = paramArray[0];
    switch(uiKeyAct)
    {
    case NVTEVT_KEY_PRESS:
        Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
        break;
    }

    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiModeLink_Tab_RefreshAndWiFiOff_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if 0
    UINT32  uiKeyAct;

    uiKeyAct = paramArray[0];
    switch(uiKeyAct)
    {
    case NVTEVT_KEY_PRESS:
        switch(UxTab_GetData(&UIMenuWndWiFiModeLink_Tab_RefreshAndWiFiOffCtrl, TAB_FOCUS))
        {
        case UIMenuWndWiFiModeLink_Tab_RefreshAndWiFiOff_Button_Refresh:
            break;
        case UIMenuWndWiFiModeLink_Tab_RefreshAndWiFiOff_Button_WiFiOff:
            if(UI_GetData(FL_WIFI_LINK)==WIFI_LINK_OK)
            {   //change to normal mode for close app
                Ux_PostEvent(NVTEVT_SYSTEM_MODE, 2, System_GetState(SYS_STATE_CURRMODE),SYS_SUBMODE_NORMAL);
                //should close network application,then stop wifi
                Ux_PostEvent(NVTEVT_EXE_WIFI_STOP, 0);
            }
            else
            {
                Ux_CloseWindow(&UIMenuWndWiFiModuleLinkCtrl,0);
            }
            break;
        }
        break;
    }
#endif

    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiModeLink_Tab_Authorized_OK(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_OpenWindow(&UIMenuWndWiFiMobileLinkOKCtrl, 0);

    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiModuleLink_OnMovieFull(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndWiFiMovie_OnMovieFull(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiModuleLink_OnMovieWrError(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndWiFiMovie_OnMovieWrError(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiModuleLink_OnStorageSlow(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndWiFiMovie_OnStorageSlow(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiModuleLink_OnDeleteFiles(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndWiFiMovie_OnDeleteFiles(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiModuleLink_OnDeleteROFiles(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndWiFiMovie_OnDeleteROFiles(pCtrl, paramNum, paramArray);
}
#endif

