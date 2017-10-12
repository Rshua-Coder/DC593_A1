/**
    @file       NvtAppMovieEvent.h
    @ingroup    mINvtSystem

    @brief      NVT system events for movie.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef NVT_MOVIE_EVENT_H
#define NVT_MOVIE_EVENT_H

#include "NVTEvent.h"

/**
    @addtogroup mINvtSystem
*/
//@{

/**
    The event of movie-record status.
*/
typedef enum
{
    NVTEVT_CB_MOVIE_CONTINUOUS_RECORD = APP_MOVIEREC_EVENT_BASE,
    NVTEVT_CB_MOVIE_REC_ONE_SEC,
    NVTEVT_CB_MOVIE_REC_FINISH,
    NVTEVT_CB_MOVIE_FULL,
    NVTEVT_CB_MOVIE_SLOW,
    NVTEVT_CB_MOVIE_SLOWMEDIA,  ///< slow media
    NVTEVT_CB_MOVIE_FILEACCESS, ///< file access with case of card full
    NVTEVT_CB_MOVIE_OVERTIME,
    NVTEVT_CB_MOVIE_PIM_READY,
    NVTEVT_CB_MOVIE_WR_ERROR,
    NVTEVT_CB_PREVIEWSTABLE,
    NVTEVT_CB_OZOOMSTEPCHG,
    NVTEVT_CB_DZOOMSTEPCHG,
    NVTEVT_CB_MOVIE_LOOPREC_FULL,
    NVTEVT_CB_MOVIE_DELETE_FILES,
    NVTEVT_CB_MOVIE_DELETE_RO_FILES,
    NVTEVT_CB_EMR_COMPLETED,
    NVTEVT_CB_MOVIE_STOPPED,
    NVTEVT_CB_MOVIE_SLOWSTART,
    NVTEVT_CB_MOVIE_MAX,
}NVT_MOVIEREC_EVENT;

typedef enum
{
    NVTEVT_CB_RAWENC_OK = NVTEVT_CB_MOVIE_MAX,
    NVTEVT_CB_RAWENC_ERR,
    NVTEVT_CB_RAWENC_WRITE_ERR,
    NVTEVT_CB_RAWENC_DCF_FULL,
    NVTEVT_CB_RAWENC_JPG_OK,
}NVT_RAWENC_EVENT;

//@}
#endif //NVT_MOVIE_EVENT_H
