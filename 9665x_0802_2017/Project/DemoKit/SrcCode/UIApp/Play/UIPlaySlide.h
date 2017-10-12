#ifndef _UIVIEWSLIDE_H_
#define _UIVIEWSLIDE_H_

#include "SlideEffect.h"
#include "GxGfx.h"

//#NT#2010/03/06#JeahYen -begin
typedef struct _UIPlay_SlideView
{
    UINT32 scr_w;
    UINT32 scr_h;
    UINT32 x;
    UINT32 y;
    UINT32 w;
    UINT32 h;
    IMAGE* pMyImg1;
    IMAGE* pMyImg2;
}
UIPlay_SlideView;

extern void UISlide_OpenView(UIPlay_SlideView* pViewCfg, BOOL bOpenFromPB);
extern void UISlide_CloseView(BOOL bReturnToPB);

extern void UISlide_LoadData(void);
extern void UISlide_DrawPreview(INT32 iEffect);
extern void UISlide_OnChangeSize(ISIZE *pSize, ISIZE *pOldSize);
//#NT#2010/03/06#JeahYen -end

#endif //_UIVIEWSLIDE_H_

