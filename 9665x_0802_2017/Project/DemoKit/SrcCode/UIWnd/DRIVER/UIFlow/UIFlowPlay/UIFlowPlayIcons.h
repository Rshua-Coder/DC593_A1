
#ifndef __UIFlowWndPlayIcons_H
#define __UIFlowWndPlayIcons_H

#include "type.h"

extern void FlowPB_UpdateIcons(BOOL bShow);
extern void FlowPB_IconDrawMovPlay(BOOL bShow);
extern void FlowPB_IconDrawMovPlayTime(BOOL bShow);
extern void FlowPB_ClearAllThumbIcon(void);
extern void FlowPB_ShowAllThumbIcon(void);

extern void FlowPB_IconDrawMovPlayVolumn(UINT32 uiVolumn);
extern void FlowPB_IconHideMovPlayVolumn(void);

extern void FlowPB_IconDrawMovSpeed(void);
extern void FlowPB_IconHideMovSpeed(VControl *pCtrl);


#endif //__UIFlowWndPlayIcons_H
