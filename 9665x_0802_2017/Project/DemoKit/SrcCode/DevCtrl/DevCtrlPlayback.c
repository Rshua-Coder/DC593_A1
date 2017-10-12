#include "SysCommon.h"
#include "SysMain.h"
#include "DeviceCtrl.h"
#include "DxSensor.h"

//////////////////////////////////////
void DevCtrl_ModePlayback(void)
{
    UINT32 uiPrevMode = System_GetState(SYS_STATE_PREVMODE);
    debug_msg("^MMODE %d->%d\r\n", uiPrevMode, PRIMARY_MODE_PLAYBACK);
    DrvSensor_TurnOffPower();
}

