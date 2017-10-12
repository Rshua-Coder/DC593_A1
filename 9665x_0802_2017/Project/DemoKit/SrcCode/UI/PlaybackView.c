#include "PrjCfg.h"
#include "UIDisplay.h"
//#include "PlaybackView.h"

//#NT#2011/01/31#Ben Wang -begin
//#NT#Add screen control to sync display with OSD
#include "UIPlayObj.h"
//#include "UIPlayComm.h"
//#NT#2011/01/31#Ben Wang -end

#define UIDebug_Show   debug_ind

//VDO1
BOOL View_Playback(UINT32 cmd, UINT32 param1, UINT32 param2)
{
    if(cmd == SHOWCMD_BEGINDRAW)
    {
        UIDebug_Show(("^Gbegin VDO1 src=PLAYBACK\r\n"));
        GxDisplay_SetSwapEffect(LAYER_VDO1, SWAPEFFECT_DISCARD); //disable double buffer
        return TRUE;
    }
    if(cmd == SHOWCMD_ENDDRAW)
    {
        UIDebug_Show(("^Gend VDO1 src=PLAYBACK\r\n"));
        //#NT#2011/01/31#Ben Wang -begin
        //#NT#Add screen control to sync display with OSD
        Ux_SendEvent(&UIPlayObjCtrl, NVTEVT_SCREEN_CTRL, 1, PLAY_FLUSH_SCREEN);
        // TODO: JEAH REMOVED for porting - begin
        //UpdateVdoWinForPB();
        // TODO: JEAH REMOVED for porting - end
        //#NT#2011/01/31#Ben Wang -end
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1COMPARE, COMPARE_KEY_NEVER);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1COLORKEY, 0);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1ALPHA, OSD_OPAQUE_100);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1BLEND, BLEND_DISABLE);
        return TRUE;
    }
    if(cmd == SHOWCMD_SETSIZE)
    {
        //ISIZE *pSize = (ISIZE*)param2;
        //Display_SetWindow(LAYER_VDO1, 0, 0, pSize->w, pSize->h); //change window size
        //FlowPlay_OnChangeSize(pSize, 0);
        return TRUE;
    }
    return FALSE;
}




