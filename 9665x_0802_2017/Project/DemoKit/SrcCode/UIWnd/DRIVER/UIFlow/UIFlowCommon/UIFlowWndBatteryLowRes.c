//This source code is generated by UI Designer Studio.

#ifndef UIFLOWWNDBATTERYLOWRES_H
#define UIFLOWWNDBATTERYLOWRES_H

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIDisplayUtil.h"
//------------------------------------------------------------

SHOW_ITEM_RECT(CMD_Rectangle,Skin1Rect,0,0,319,239,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin1)
SHOW_MAP_BODY(Skin1Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin1,0,0,319,239)

SHOW_ITEM_RECT(CMD_Rectangle,Skin3Rect,0,0,159,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin3)
SHOW_MAP_BODY(Skin3Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin3,0,0,159,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text5,0,0,159,27,STRID_NULL,0,0,0,0,2,1,1,0,0,33,0,0,0,0)
SHOW_MAP_BEGIN(Normal2)
SHOW_MAP_BODY(Skin3)
SHOW_MAP_BODY(Text5)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal2,80,61,239,88)

SHOW_MAP_BEGIN(UIFlowWndBatteryLow_Static_Title)
SHOW_MAP_BODY(Normal2)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndBatteryLow_Static_Title)
CREATE_STATIC_DATA(UIFlowWndBatteryLow_Static_Title,STRID_BATLOW_POWEROFF)
CREATE_STATIC_CTRL(UIFlowWndBatteryLow_Static_Title,0,80,61,239,88)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin7Rect,0,0,159,49,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin7)
SHOW_MAP_BODY(Skin7Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin7,0,0,159,49)

SHOW_ITEM_RECT(CMD_Rectangle,Skin9Rect,0,0,39,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin9)
SHOW_MAP_BODY(Skin9Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin9,0,0,39,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text11,0,0,39,27,STRID_NULL,0,0,0,0,2,1,1,0,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal8)
SHOW_MAP_BODY(Skin9)
SHOW_MAP_BODY(Text11)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal8,101,111,140,138)

SHOW_MAP_BEGIN(UIFlowWndBatteryLow_Static_PowerOffY)
SHOW_MAP_BODY(Normal8)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndBatteryLow_Static_PowerOffY)
CREATE_STATIC_DATA(UIFlowWndBatteryLow_Static_PowerOffY,STRID_YES)
CREATE_STATIC_CTRL(UIFlowWndBatteryLow_Static_PowerOffY,0,27,0,66,27)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin14Rect,0,0,39,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin14)
SHOW_MAP_BODY(Skin14Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin14,0,0,39,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text16,0,0,39,27,STRID_NULL,0,0,0,0,2,1,1,0,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal13)
SHOW_MAP_BODY(Skin14)
SHOW_MAP_BODY(Text16)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal13,165,111,204,138)

SHOW_MAP_BEGIN(UIFlowWndBatteryLow_Static_PowerOffN)
SHOW_MAP_BODY(Normal13)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndBatteryLow_Static_PowerOffN)
CREATE_STATIC_DATA(UIFlowWndBatteryLow_Static_PowerOffN,STRID_NO)
CREATE_STATIC_CTRL(UIFlowWndBatteryLow_Static_PowerOffN,0,91,0,130,27)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIFlowWndBatteryLow_Panel)
SHOW_MAP_BODY(Skin7)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndBatteryLow_Panel)
CTRL_LIST_BEGIN(UIFlowWndBatteryLow_Panel)
CTRL_LIST_ITEM(UIFlowWndBatteryLow_Static_PowerOffY)
CTRL_LIST_ITEM(UIFlowWndBatteryLow_Static_PowerOffN)
CTRL_LIST_END

CREATE_TAB_DATA(UIFlowWndBatteryLow_Panel,0)

CREATE_TAB_CTRL(UIFlowWndBatteryLow_Panel,UIFlowWndBatteryLow_Panel,0,74,111,233,160)
SHOW_ITEM_RECT(CMD_Rectangle,Skin19Rect,0,0,91,19,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin19)
SHOW_MAP_BODY(Skin19Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin19,0,0,91,19)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text21,0,0,91,19,STRID_NULL,0,0,0,0,2,1,1,0,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal18)
SHOW_MAP_BODY(Skin19)
SHOW_MAP_BODY(Text21)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal18,102,91,193,110)

SHOW_MAP_BEGIN(UIFlowWndBatteryLow_Second)
SHOW_MAP_BODY(Normal18)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndBatteryLow_Second)
CREATE_STATIC_DATA(UIFlowWndBatteryLow_Second,STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndBatteryLow_Second,0,102,91,193,110)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin24Rect,0,0,91,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin24)
SHOW_MAP_BODY(Skin24Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin24,0,0,91,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text26,6,6,85,21,STRID_NULL,0,0,0,0,37,1,1,0,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal23)
SHOW_MAP_BODY(Skin24)
SHOW_MAP_BODY(Text26)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal23,73,109,164,136)

SHOW_ITEM_RECT(CMD_Rectangle,Skin28Rect,0,0,91,27,0,0,48,48,0,0,0)
SHOW_MAP_BEGIN(Skin28)
SHOW_MAP_BODY(Skin28Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin28,0,0,91,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text30,6,6,85,21,STRID_NULL,0,0,0,0,37,1,1,0,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused27)
SHOW_MAP_BODY(Skin28)
SHOW_MAP_BODY(Text30)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused27,73,109,164,136)

SHOW_MAP_BEGIN(Skin32)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin32,0,0,91,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text34,6,6,85,21,STRID_NULL,0,0,0,0,1,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable31)
SHOW_MAP_BODY(Skin32)
SHOW_MAP_BODY(Text34)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable31,73,109,164,136)

SHOW_MAP_BEGIN(Skin36)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin36,0,0,91,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text38,6,6,85,21,STRID_NULL,0,0,0,0,1,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable35)
SHOW_MAP_BODY(Skin36)
SHOW_MAP_BODY(Text38)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable35,73,109,164,136)

SHOW_MAP_BEGIN(Skin40)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin40,0,0,91,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text42,6,6,85,21,STRID_NULL,0,0,0,0,1,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed39)
SHOW_MAP_BODY(Skin40)
SHOW_MAP_BODY(Text42)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed39,73,109,164,136)

CREATE_MENU_ITEM_DATA(STRID_YES43,STRID_YES,ICONID_NULL,STATUS_ENABLE,NULL,NULL) 
CREATE_MENU_ITEM_DATA(STRID_NO44,STRID_NO,ICONID_NULL,STATUS_ENABLE,NULL,NULL) 
SHOW_MAP_BEGIN(UIFlowWndBatteryLow_Switch)
SHOW_MAP_BODY(Normal23)
SHOW_MAP_BODY(Focused27)
SHOW_MAP_BODY(Normal_Disable31)
SHOW_MAP_BODY(Focused_Disable35)
SHOW_MAP_BODY(Pressed39)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndBatteryLow_Switch)
MENU_LIST_BEGIN(UIFlowWndBatteryLow_Switch)
MENU_LIST_ITEM(STRID_YES43)
MENU_LIST_ITEM(STRID_NO44)
MENU_LIST_END

CREATE_MENU_DATA(UIFlowWndBatteryLow_Switch,0,2,MENU_LAYOUT_HORIZONTAL|MENU_DISABLE_SHOW|MENU_SCROLL_CYCLE|MENU_SCROLL_NEXT_PAGE|MENU_DISABLE_NORMAL|MENU_DRAW_IMAGE)

CREATE_MENU_CTRL(UIFlowWndBatteryLow_Switch,UIFlowWndBatteryLow_Switch,0,73,109,164,136)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIFlowWndBatteryLow)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndBatteryLow)
DECLARE_CTRL_LIST(UIFlowWndBatteryLow)
CREATE_CTRL(UIFlowWndBatteryLow,UIFlowWndBatteryLow,CTRL_WND,NULL,0 ,0,0,319,239)
#endif
