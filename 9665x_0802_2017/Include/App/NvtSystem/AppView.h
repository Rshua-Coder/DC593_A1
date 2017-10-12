#ifndef _APPVIEW_H_
#define _APPVIEW_H_

#include "Type.h"
#include "GxImage.h"
#include "DispSrvAPI.h"

/*
Struct

APP
    + APPVIEW
        + VIEWPORT -> DEVID
        + VIEWPORT -> DEVID
*/

/*
Flow

1.OpenApp/CloseApp flow
DeviceOpen()
AppView_SetEnable(DevID, TRUE);
AppView_SetBuffer(DevID, w, h, fmt);
AppView_SetWindow(DevID, x,y,w,h, dir);
AppView_SetSource(src);
AppView_Open(); -------- alloc disp buffer
AppOpen();
 :
AppClose();
AppView_Close();
DeviceClose()

2.ChangeDispDevice or ChangeDispMode flow
AppView_Pause();
DeviceClose()
  :
AppView_SetEnable(DevID, TRUE);
AppView_SetBuffer(DevID, w, h, fmt);
AppView_SetWindow(DevID, x,y,w,h, dir);
DeviceOpen()
AppView_Resume(); -------- alloc disp buffer

3.ChangeAppView flow
AppView_Pause();
  :
AppView_SetEnable(DevID, TRUE);
AppView_SetBuffer(DevID, w, h, fmt);
AppView_SetWindow(DevID, x,y,w,h, dir);
AppView_SetSource(src);
AppView_Resume(); -------- alloc disp buffer
*/


/*
    //device
    UINT32 DispDev;
    UINT32 DispMode; //get device mode (size, aspect-ratio)

*/
/*
typedef struct _VIEWPORT
{
    //disp-buffer
    USIZE BufferSize; //default = SCREEN_FULL
    UINT32 uiBufferFmt; //default = YUV420
    //disp-window
    URECT Window; //default = SCREEN_FULL <---------- if it need fill after AppTask open, must be update by CB
    UINT32 uiDir; //default = 0
}
VIEWPORT;
*/

//view source - provided by app mode
typedef struct _VIEW_SRC
{
    BOOL bDirectMode;
    DISPSRV_USER_HANDLE UserHandle;
    INT32 (*fpDrawCb)(DISPSRV_DRAW* pDraw);
    void (*fpOpenView)(BOOL bWaitFinish);
    void (*fpCloseView)(BOOL bWaitFinish);
    void (*fpPauseView)(BOOL bWaitFinish);
    void (*fpResumeView)(BOOL bWaitFinish);
}
VIEW_SRC;

//open DispSrv
extern void AppView_Init(void);
//close DispSrv
extern void AppView_Exit(void);
//set disp-working-buffer
extern void AppView_ConfigWorkingBuffer(UINT32 addr, UINT32 size);
//set disp-enable
extern void AppView_ConfigEnable(UINT32 DevID, BOOL bEn);
//set disp-size
extern void AppView_ConfigDevice(UINT32 DevID, UINT32 w, UINT32 h, UINT32 aw, UINT32 ah);
//set disp-buffer
extern void AppView_ConfigBuffer(UINT32 DevID, UINT32 w, UINT32 h, UINT32 fmt);
//set disp-Image
extern void AppView_ConfigImgSize(UINT32 DevID, UINT32 w, UINT32 h, UINT32 ratioW, UINT32 ratioH);
//set disp-window
extern void AppView_ConfigWindow(UINT32 DevID, UINT32 x, UINT32 y, UINT32 w, UINT32 h);
//set disp-attr (rotate, ...)
extern void AppView_ConfigAttr(UINT32 DevID, UINT32 attr, UINT32 value);
//set disp-source
extern void AppView_ConfigSource(VIEW_SRC* src);
//get disp buffer size
extern USIZE AppView_GetBufferSize(UINT32 DevID);
//open
extern void AppView_Open(void); //alloc disp buffer
//close
extern void AppView_Close(void); //free disp buffer
//pause
extern void AppView_Pause(void); //free disp buffer
//resume
extern void AppView_Resume(void); //alloc disp buffer

//buffer
extern BOOL AppView_CalcBestFitInsideDisp(UINT32 DevID, USIZE* pImageAspectRatio, USIZE* pBufferSize, URECT* pWindowRect);
extern void AppView_UpdateWindow(UINT32 DevID);



#endif //_APPVIEW_H_
