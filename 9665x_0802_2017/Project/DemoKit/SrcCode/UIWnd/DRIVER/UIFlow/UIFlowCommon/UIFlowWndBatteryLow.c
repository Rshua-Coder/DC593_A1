//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIFlowWndBatteryLowRes.c"
#include "UIFlowWndBatteryLow.h"
#include "PrjCfg.h"
#include "SysMain.h"
#include "GxTimer.h"
//---------------------UIFlowWndBatteryLowCtrl Debug Definition -----------------------------
#define _UIFLOWWNDBATTERYLOW_ERROR_MSG        1
#define _UIFLOWWNDBATTERYLOW_TRACE_MSG        0

static UINT32 g_uiBatTimerID        = NULL_TIMER;


#if (_UIFLOWWNDBATTERYLOW_ERROR_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_ERR))
#define UIFlowWndBatteryLowErrMsg(...)            debug_msg ("^R UIFlowWndBatteryLow: "__VA_ARGS__)
#else
#define UIFlowWndBatteryLowErrMsg(...)
#endif

#if (_UIFLOWWNDBATTERYLOW_TRACE_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_TRC))
#define UIFlowWndBatteryLowTraceMsg(...)          debug_msg ("^B UIFlowWndBatteryLow: "__VA_ARGS__)
#else
#define UIFlowWndBatteryLowTraceMsg(...)
#endif

//---------------------UIFlowWndBatteryLowCtrl Global Variables -----------------------------

//---------------------UIFlowWndBatteryLowCtrl Prototype Declaration  -----------------------

//---------------------UIFlowWndBatteryLowCtrl Public API  ----------------------------------

//---------------------UIFlowWndBatteryLowCtrl Private API  ---------------------------------
//---------------------UIFlowWndBatteryLowCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndBatteryLow)
CTRL_LIST_ITEM(UIFlowWndBatteryLow_Static_Title)
CTRL_LIST_ITEM(UIFlowWndBatteryLow_Panel)
CTRL_LIST_ITEM(UIFlowWndBatteryLow_Second)
CTRL_LIST_ITEM(UIFlowWndBatteryLow_Switch)
CTRL_LIST_END

static UINT32 g_uiBatLow_Time = 15;
static UINT32 g_uiBatLow_TimeBuf[2] = {0};

//----------------------UIFlowWndBatteryLowCtrl Event---------------------------
INT32 UIFlowWndBatteryLow_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndBatteryLow_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndBatteryLow_OnACPlug(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndBatteryLow_OnTimer(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndBatteryLow)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIFlowWndBatteryLow_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIFlowWndBatteryLow_OnClose)
EVENT_ITEM(NVTEVT_AC_Plug,UIFlowWndBatteryLow_OnACPlug)
EVENT_ITEM(NVTEVT_TIMER,UIFlowWndBatteryLow_OnTimer)
EVENT_END

INT32 UIFlowWndBatteryLow_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	g_uiBatLow_Time = 15;
	UxCtrl_SetShow(&UIFlowWndBatteryLow_PanelCtrl , FALSE);
	 g_uiBatTimerID = GxTimer_StartTimer(1000, NVTEVT_1SEC_TIMER, CONTINUE);
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndBatteryLow_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	 if(g_uiBatTimerID != NULL_TIMER)
    {
        GxTimer_StopTimer(&g_uiBatTimerID);
    }
	 
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndBatteryLow_OnACPlug(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_CloseWindow(pCtrl, 0);//drh 20170925
    return NVTEVT_PASS;
}
INT32 UIFlowWndBatteryLow_OnTimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	 NVTEVT event;
	UINT32 CurMenuIndex = 0;
	CurMenuIndex = UxMenu_GetData( &UIFlowWndBatteryLow_SwitchCtrl,MNU_CURITM);
    event = paramArray[0];

	switch(event)
	{
		case NVTEVT_1SEC_TIMER:
			if(g_uiBatLow_Time){	
				g_uiBatLow_Time --;
				sprintf(&g_uiBatLow_TimeBuf,"%d",g_uiBatLow_Time);
				UxStatic_SetData(&UIFlowWndBatteryLow_SecondCtrl, STATIC_VALUE, Txt_Pointer(g_uiBatLow_TimeBuf));
			}
			else{
				if(CurMenuIndex == UIFlowWndBatteryLow_Switch_STRID_YES){
					System_PowerOff(SYS_POWEROFF_NORMAL);
				}
				else{
					Ux_CloseWindow(pCtrl, 0);//drh 20170925
				}
			}
		break;	
	}
}

//----------------------UIFlowWndBatteryLow_Static_TitleCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndBatteryLow_Static_Title)
EVENT_END

//----------------------UIFlowWndBatteryLow_PanelCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndBatteryLow_Panel)
EVENT_END

//----------------------UIFlowWndBatteryLow_Static_PowerOffYCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndBatteryLow_Static_PowerOffY)
EVENT_END

//----------------------UIFlowWndBatteryLow_Static_PowerOffNCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndBatteryLow_Static_PowerOffN)
EVENT_END

//----------------------UIFlowWndBatteryLow_SecondCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndBatteryLow_Second)
EVENT_END


INT32 UIFlowWndBatteryLow_Switch_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndBatteryLow_Switch_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndBatteryLow_Switch_OnKeyShutter2(VControl *, UINT32, UINT32 *);
//----------------------UIFlowWndBatteryLow_SwitchCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndBatteryLow_Switch)
EVENT_ITEM(NVTEVT_KEY_UP,UIFlowWndBatteryLow_Switch_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIFlowWndBatteryLow_Switch_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_SHUTTER2,UIFlowWndBatteryLow_Switch_OnKeyShutter2)
EVENT_END
INT32 UIFlowWndBatteryLow_Switch_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32  uiKeyAct;
	static BOOL bKey_Continue = FALSE;
	static INT32 bKey_Continue_Count = -1;
	
    uiKeyAct = paramArray[0];


	switch (uiKeyAct)
    {
      	case NVTEVT_KEY_PRESS:
			bKey_Continue = FALSE;
			bKey_Continue_Count = 0;
			break;

		 case NVTEVT_KEY_CONTINUE:
			bKey_Continue = TRUE;
			if(bKey_Continue_Count < 0)	break;
			bKey_Continue_Count ++ ;
			if(bKey_Continue_Count == 1) //the first time
			{
				;
			}
			break;

		 case NVTEVT_KEY_RELEASE:
			if(bKey_Continue == FALSE){	
				Ux_SendEvent(pCtrl, NVTEVT_PREVIOUS_ITEM, 0);
			}
			else if (bKey_Continue == TRUE){
				
			}
			bKey_Continue_Count = -1;
			break;
	}
	
    return NVTEVT_CONSUME;
    //return UIFlowWndMovie_OnExeZoomIn(pCtrl, paramNum, paramArray);
}

INT32 UIFlowWndBatteryLow_Switch_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32  uiKeyAct;
	static BOOL bKey_Continue = FALSE;
	static INT32 bKey_Continue_Count = -1;
	
    uiKeyAct = paramArray[0];
		
	switch (uiKeyAct)
    {
      	case NVTEVT_KEY_PRESS:
			bKey_Continue = FALSE;
			bKey_Continue_Count = 0;
			break;

		 case NVTEVT_KEY_CONTINUE:
			bKey_Continue = TRUE;
			if(bKey_Continue_Count < 0) break;
			bKey_Continue_Count ++ ;
			if(bKey_Continue_Count == 1) //the first time
			{
				;
			}
			break;

		 case NVTEVT_KEY_RELEASE:
			if(bKey_Continue == FALSE){	
				Ux_SendEvent(pCtrl, NVTEVT_NEXT_ITEM, 0);
			}
			else if(bKey_Continue == TRUE){
				
			}
			else{
				;
			}
			bKey_Continue_Count = -1;
			break;
	}
    return NVTEVT_CONSUME;
    //return UIFlowWndMovie_OnExeZoomOut(pCtrl, paramNum, paramArray);
}

INT32 UIFlowWndBatteryLow_Switch_OnKeyShutter2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

	UINT32  uiKeyAct;
	UINT32 CurMenuIndex = 0;
	CurMenuIndex = UxMenu_GetData( &UIFlowWndBatteryLow_SwitchCtrl,MNU_CURITM);
	uiKeyAct = paramArray[0];	
	switch (uiKeyAct)
    {
      	case NVTEVT_KEY_PRESS:
			if(CurMenuIndex == UIFlowWndBatteryLow_Switch_STRID_YES){
				System_PowerOff(SYS_POWEROFF_NORMAL);
			}
			else{
				Ux_CloseWindow(pCtrl, 0);//drh 20170925
			}
			break;
	}
}

