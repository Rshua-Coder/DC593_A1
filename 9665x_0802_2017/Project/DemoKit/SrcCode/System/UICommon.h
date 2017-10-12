/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UICommon.h
    @ingroup    mIPRJAPUIFlow

    @brief      UI Common Interfaces
                This file is the user interface ( for LIB callback function).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef _UICOMMON_H
#define _UICOMMON_H

//V (UI)
#include "UIFrameworkExt.h"
#include "UIDisplayUtil.h"
#include "UIGraphics.h"
#include "UIDisplay.h"
#include "UIAnimation.h"
#include "GxRect.h"
#include "CoordTs.h"
#include "KeyDef.h"
#include "GxInput.h"

//C (INPUT EVENTS)
///////////////////////////////////////////////////////////////////////////////
//SysInput
typedef struct _INPUT_KEY
{
    UINT32 flag;
    UINT32 event;
    char* name;
}
INPUT_KEY;
typedef struct _INPUT_TOUCH
{
    UINT32 flag;
    UINT32 event;
    char* name;
}
INPUT_TOUCH;

#define INLIB_NKEY_TABLE        1
#define INLIB_SKEY_TABLE        2
#define INLIB_TOUCH_TABLE       3
void Input_SetEventTable(int i, const void* pTable);
#define INLIB_CB_PKEY_FILTER    0
#define INLIB_CB_NKEY_FILTER    1
#define INLIB_CB_SKEY_FILTER    2
#define INLIB_CB_TOUCH_FILTER   3
#define INLIB_CB_PLAYSOUND      4
extern UINT32 InputLib_CB(UINT32 event, UINT32 param1, UINT32 param2);
#define INLIB_NKEY              1
#define INLIB_NKEY_CONT         2
#define INLIB_NKEY_REL          3
#define INLIB_SKEY              4
#define INLIB_TOUCH             5

extern void Input_SetAnyKeyUnlock(BOOL en);
extern void Input_SetKeyMask(KEY_STATUS uiMode, UINT32 uiMask);
extern UINT32 Input_GetKeyMask(KEY_STATUS uiMode);
extern void Input_SetKeySoundMask(KEY_STATUS uiMode, UINT32 uiMask);
extern UINT32 Input_GetKeySoundMask(KEY_STATUS uiMode);
extern void Input_SetTouchMask(UINT32 uiMask);
extern void Input_SetTouchSoundMask(UINT32 uiMask);
extern UINT32 Input_GetTouchSoundMask(void);
extern UINT32 Input_GetCurrStatus(UINT32 status);
extern UINT32 Input_Key2Mode(UINT32 keys);


extern RESULT UI_ClearRect(DC* pDestDC,LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);
extern void UI_DrawUserShowText(UIScreen ScreenObj,ITEM_BASE* pOrgShowObj);
extern void UI_DrawUserShowText2(UIScreen ScreenObj,ITEM_BASE* pOrgShowObj);
extern void UI_Set2StaticStrPos(VControl *pLeftStrCtrl,VControl *pRightStrCtrl);

//--------------------------------------------------------------------------------------
//  Control (unlock/lock detect flow and device control)
//--------------------------------------------------------------------------------------
extern void UI_LockAutoSleep(void);
extern void UI_UnlockAutoSleep(void);
extern void UI_LockAutoPWROff(void);
extern void UI_UnlockAutoPWROff(void);
extern void UI_LockStatusEvent(void);
extern void UI_LockEvent(void);
extern void UI_UnlockEvent(void);
extern UINT32 UI_IsForceLockAutoSleep(void);
extern UINT32 UI_IsForceLockAutoPWROff(void);
extern UINT32 UI_IsForceLockStatus(void);
extern UINT32 UI_IsForceLock(void);

#if _DEMO_TODO
//#NT#2011/03/17#Jeah Yen -begin
extern BOOL g_bIsExitSystemWithPowerKey;
extern BOOL g_bIsExitSystemWithPBKey;
//#NT#2011/03/17#Jeah Yen -end
#endif

extern void UserWaitEvent(NVTEVT wait_evt, UINT32 *wait_paramNum, UINT32 *wait_paramArray);
//#NT#2014/07/08#Philex Lin -begin
extern BOOL System_IsPlugTVHDMI(void);
//#NT#2014/07/08#Philex Lin -end
BOOL IsDualHDMIPlugIn(void);
#endif //_UICOMMON_H_

