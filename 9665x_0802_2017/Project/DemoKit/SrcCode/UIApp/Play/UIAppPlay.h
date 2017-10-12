#ifndef _UIVIEWPLAY_H_
#define _UIVIEWPLAY_H_

#include "AppLib.h"

#include "UIPlayObj.h"

#include "UIPlayInfo.h"

//#NT#2009/10/29#Jeah Yen - begin

// APP event class

typedef enum
{
    NVTEVT_EXE_COPY2CARD    = NVTEVT_PLAY_MAX+1,
    NVTEVT_EXE_SLIDE,
    NVTEVT_EXE_VOICEMEMO,
    NVTEVT_EXE_PLAYEFFECT,
}
CUSTOM_PLAY_EVENT;
//#NT#2009/10/29#Jeah Yen - end


//Play Config
//#NT#2010/09/28#Jeah Yen -begin
extern ISIZE Play_GetBufferSize(void);
//#NT#2010/09/28#Jeah Yen -end

//Play Init
extern INT32 PlayExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PlayExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

//Play Exe
extern INT32 PlayExe_OnCopy2Card(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PlayExe_OnSlide(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PlayExe_OnVoiceMemo(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);



//#NT#2009/10/29#Jeah Yen - begin
extern VControl CustomPlayObjCtrl;
//#NT#2009/10/29#Jeah Yen - end

#endif //_UIVIEWPLAY_H_
