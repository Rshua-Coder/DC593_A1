#ifndef _UIDISPLAY_H
#define _UIDISPLAY_H

#include "Type.h"
#include "UIDisplay.h"

//OSD1
extern BOOL View_Window(UINT32 cmd, UINT32 param1, UINT32 param2);
//OSD2
extern BOOL View_Info(UINT32 cmd, UINT32 param1, UINT32 param2);
//VDO1
//VDO2
extern BOOL View_Background(UINT32 cmd, UINT32 param1, UINT32 param2);
extern BOOL View_Background_CK(UINT32 cmd, UINT32 param1, UINT32 param2);

extern void View_Window_ConfigAttr(UINT32 attr, UINT32 value);
extern DC* View_Window_BeginRotate(DC* pDC);
extern void View_Window_EndRotate(void);


#endif // _UIDISPLAY_H

