/**
    @file       NvtAppPhotoEvent.h
    @ingroup    mINvtSystem

    @brief      NVT system events for photo mode.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/


#ifndef NVT_PHOTO_EVENT_H
#define NVT_PHOTO_EVENT_H

#include "NVTEvent.h"
/**
    @addtogroup mINvtSystem
*/
//@{

/**
    Photo system event.
*/
typedef enum
{
    NVTEVT_CB_ZOOM = APP_PHOTOCAP_EVENT_BASE,                 ///< Optical or Digital Zoom moving change step.
    NVTEVT_CALLBACK,                                          ///< General callback event of photo mode
    NVTEVT_ALGMSG_FDEND,                                      ///< face detection end
    NVTEVT_ALGMSG_SDEND,                                      ///< smile detection end
    NVTEVT_ALGMSG_FOCUSEND,                                   ///< auto focus move end
    NVTEVT_ALGMSG_ASCEND,                                     ///< auto scene detection end
    NVTEVT_ALGMSG_FLASH,                                      ///< capture flash trigger end
    NVTEVT_ALGMSG_QVSTART,                                    ///< capture quick view image generated
    NVTEVT_ALGMSG_JPGOK,                                      ///< capture image compress JPG OK
    NVTEVT_ALGMSG_CAPFSTOK,                                   ///< capture image save to file System OK
    NVTEVT_ALGMSG_CAPTUREEND,                                 ///< capture command end
    NVTEVT_ALGMSG_SLOWSHUTTER,                                ///< slow shutter
    NVTEVT_ALGMSG_MWBCALEND,                                  ///< calculate manual WB end
    NVTEVT_ALGMSG_PDEND,                                      ///< pan capture detection end
    NVTEVT_ALGMSG_PROGRESSEND,                                ///< pan capture progress end
    NVTEVT_ALGMSG_DZOOMSTEP,                                  ///< Digital Zoom moving change one step.
    NVTEVT_ALGMSG_PREVIEW_STABLE,                             ///< preview stable
}NVT_PHOTOCAP_EVENT;

//@}
#endif //NVT_PHOTO_EVENT_H

