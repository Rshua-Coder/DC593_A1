/**
    @file       NvtAppPlayEvent.h
    @ingroup    mINvtSystem

    @brief      NVT system events for playback.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef NVT_PLAYBACK_EVENT_H
#define NVT_PLAYBACK_EVENT_H

#include "NVTEvent.h"

/**
    @addtogroup mINvtSystem
*/
//@{

/**
    Playback system event.
*/
typedef enum
{
    NVTEVT_CB_SLIDE_PREPAREDATA = APP_PHOTOPLAY_EVENT_BASE, ///< Change display buffer size on slideshow.
    NVTEVT_CB_WAV_STOP                                      ///< Wav application stopped.
}NVT_PHOTOPLAY_EVENT;

/**
    Movie play system event.
*/
typedef enum
{
    NVTEVT_CB_MOVIE_START = APP_MOVIEPLAY_EVENT_BASE, ///< Movie start.
    NVTEVT_CB_MOVIE_ONE_SEC,                          ///< Posted every one second on movie recoding.
    NVTEVT_CB_MOVIE_FINISH,                           ///< Movie finished.
    NVTEVT_CB_MOVIE_ONE_VIDEOFRAME,                   ///< The first video frame done.
}NVT_MOVIEPLAY_EVENT;

//@}
#endif //NVT_PLAYBACK_EVENT_H
