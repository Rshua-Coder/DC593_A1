#include "SysCommon.h"
#include "SysMain.h"
#include "DeviceCtrl.h"
#include "DxSensor.h"

//////////////////////////////////////
void DevCtrl_ModeMovie_FastSensor(void)
{
}
void DevCtrl_ModeMovie_WaitFinish(void)
{
}
void DevCtrl_ModeMovie_FastLens(void)
{
}
void DevCtrl_ModeMovie(void)
{
    UINT32 uiPrevMode = System_GetState(SYS_STATE_PREVMODE);
    debug_msg("^MMODE %d->%d\r\n", uiPrevMode, PRIMARY_MODE_MOVIE);
    if(((INT32)uiPrevMode != PRIMARY_MODE_PHOTO)
    && ((INT32)uiPrevMode != PRIMARY_MODE_MOVIE))
    {
        DrvSensor_TurnOnPower();
    }
    DevCtrl_Lens(TRUE);
}

