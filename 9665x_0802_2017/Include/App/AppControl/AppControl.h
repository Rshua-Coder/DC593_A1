#ifndef _APPCTRL_H_
#define _APPCTRL_H_

#include "Type.h"
#include "Debug.h"
#include "VControl.h"

//////////////////////////////////////////////////

// APP event class

typedef enum
{
    NVTEVT_SYS_MIN         = APP_EVENT_MASK,

    NVTEVT_SYSTEM_BOOT        = NVTEVT_SYS_MIN,            ///<
    NVTEVT_SYSTEM_SHUTDOWN,        ///<
    NVTEVT_SYSTEM_MODE,            ///< APP
    NVTEVT_SYSTEM_SLEEP,           ///<

    NVTEVT_EXE_OPEN,
    NVTEVT_EXE_CLOSE,

    NVTEVT_STRG_INSERT, //Param1=insert/remove, Param2=card-type: SD
    NVTEVT_STRG_REMOVE, //Param1=insert/remove, Param2=card-type: SD
    NVTEVT_STRG_DETACH, //Param1=status, Param2=card-type: SD, NAND
    NVTEVT_STRG_ATTACH, //Param1=status, Param2=card-type: SD, NAND

    NVTEVT_VIDEO_INSERT, //Param1=insert/remove, Param2=video-type: TV, HDMI
    NVTEVT_VIDEO_REMOVE, //Param1=insert/remove, Param2=video-type: TV, HDMI
    NVTEVT_VIDEO_DETACH, //Param1=status, Param2=video-type: LCD, TV, HDMI
    NVTEVT_VIDEO_ATTACH, //Param1=status, Param2=video-type: LCD, TV, HDMI

    NVTEVT_AUDIO_INSERT, //Param1=insert/remove, Param2=audio-type: LINEOUT, HEAR-PHONE, HDMI
    NVTEVT_AUDIO_REMOVE, //Param1=insert/remove, Param2=audio-type: LINEOUT, HEAR-PHONE, HDMI
    NVTEVT_AUDIO_DETACH, //Param1=status, Param2=audio-type: SPEAKER, LINEOUT, HEAR-PHONE, HDMI
    NVTEVT_AUDIO_ATTACH, //Param1=status, Param2=audio-type: SPEAKER, LINEOUT, HEAR-PHONE, HDMI

    NVTEVT_USB_INSERT, //Param1=USB connection type
    NVTEVT_USB_REMOVE, //Param1=USB connection type
    NVTEVT_USB_CHARGE_CURRENT, //Param1=available charging current

    NVTEVT_PWR_BATTEMPTY,
    NVTEVT_PWR_BATTCHANGE,
    NVTEVT_PWR_CHARGE_OK,
    NVTEVT_PWR_CHARGE_SUSPEND,
    NVTEVT_PWR_CHARGE_RESUME,

    NVTEVT_VIDEO_MODE, //Param1=status, Param2=video-type: LCD, TV, HDMI
    NVTEVT_VIDEO_DIR, //Param1=status, Param2=video-type: LCD, TV, HDMI

    NVTEVT_SYS_MAX
}
NVT_SYS_EVENT;

//////////////////////////////////////////////////

extern INT32 Ux_AppDispatchMessage(NVTEVT evt, UINT32 paramNum, UINT32 *paramArray);

#endif //_APPCTRL_H_
