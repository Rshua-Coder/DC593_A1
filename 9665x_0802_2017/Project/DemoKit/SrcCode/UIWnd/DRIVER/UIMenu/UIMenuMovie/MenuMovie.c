#include <stdio.h>
#include "UIFlow.h"

// --------------------------------------------------------------------------
// OPTIONS
// --------------------------------------------------------------------------
// Movie Size Options
#if (UPDATE_CFG == UPDATE_CFG_YES)
TM_OPTION gTM_OPTIONS_MOVIE_SIZE[MOVIE_SIZE_ID_MAX] = {0};
#else
TMDEF_BEGIN_OPTIONS(MOVIE_SIZE)
    TMDEF_OPTION_TEXT(MOVIE_SIZE_1080FHD)
    TMDEF_OPTION_TEXT(MOVIE_SIZE_720P)
    TMDEF_OPTION_TEXT(MOVIE_SIZE_WVGA)
    TMDEF_OPTION_TEXT(MOVIE_SIZE_VGA)
//    TMDEF_OPTION_TEXT(MOVIE_SIZE_1440P)
TMDEF_END_OPTIONS()
#endif

// Cyclic recroding Options
TMDEF_BEGIN_OPTIONS(MOVIE_CYCLIC_REC)
    TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_OFF)
    TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_1MIN)
    TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_3MIN)
    TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_5MIN)
    TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_10MIN)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_MOTION_DET)
    TMDEF_OPTION_TEXT(MOVIE_MOTION_DET_OFF)
    TMDEF_OPTION_TEXT(MOVIE_MOTION_DET_ON)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_HDR)
    TMDEF_OPTION_TEXT(MOVIE_HDR_OFF)
    TMDEF_OPTION_TEXT(MOVIE_HDR_ON)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_AUDIO)
    TMDEF_OPTION_TEXT(MOVIE_AUDIO_OFF)
    TMDEF_OPTION_TEXT(MOVIE_AUDIO_ON)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_DATEIMPRINT)
    TMDEF_OPTION_TEXT(MOVIE_DATEIMPRINT_OFF)
    TMDEF_OPTION_TEXT(MOVIE_DATEIMPRINT_ON)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(GSENSOR)
    TMDEF_OPTION_TEXT(GSENSOR_OFF)
    TMDEF_OPTION_TEXT(GSENSOR_LOW)
    TMDEF_OPTION_TEXT(GSENSOR_MED)
    TMDEF_OPTION_TEXT(GSENSOR_HIGH)
TMDEF_END_OPTIONS()

// Park wake up Options
TMDEF_BEGIN_OPTIONS(PARK_WAKEUP)
    TMDEF_OPTION_TEXT(PARK_WAKEUP_OFF)
    TMDEF_OPTION_TEXT(PARK_WAKEUP_ON)
TMDEF_END_OPTIONS()

// --------------------------------------------------------------------------
// ITEMS
// --------------------------------------------------------------------------
// Movie Menu Items
TMDEF_BEGIN_ITEMS(MOVIE)
    TMDEF_ITEM_TEXTID(MOVIE_SIZE)
    TMDEF_ITEM_TEXTID(MOVIE_CYCLIC_REC)
    TMDEF_ITEM_TEXTID(MOVIE_HDR)
    //TMDEF_ITEM_TEXTID(MOVIE_EV)
    
    TMDEF_ITEM_TEXTID(EV)
    TMDEF_ITEM_TEXTID(MOVIE_MOTION_DET)
    TMDEF_ITEM_TEXTID(MOVIE_AUDIO)
    TMDEF_ITEM_TEXTID(MOVIE_DATEIMPRINT)
    TMDEF_ITEM_TEXTID(GSENSOR)
#if 1//PARK_WAKEUP
    TMDEF_ITEM_TEXTID(PARK_WAKEUP)
#endif
TMDEF_END_ITEMS()

// --------------------------------------------------------------------------
// PAGES
// --------------------------------------------------------------------------
// Movie Menu Pages
TMDEF_BEGIN_PAGES(MOVIE)
    TMDEF_PAGE_TEXT_ICON(MOVIE)
    TMDEF_PAGE_TEXT_ICON(SETUP)
TMDEF_END_PAGES()

TMDEF_EMNU(gMovieMenu, MOVIE, Movie_MenuCallback)

// --------------------------------------------------------------------------
// Menu Callback
// --------------------------------------------------------------------------
int Movie_MenuCallback(UINT32 uiMessage, UINT32 uiParam)
{
    UINT16  uwItemId;
    UINT16  uwOption;

    if (uiMessage == TMM_CONFIRM_OPTION)
    {
        uwItemId = LO_WORD(uiParam);
        uwOption = HI_WORD(uiParam);

        switch (uwItemId)
        {
        case IDM_MOVIE_SIZE:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIESIZE, 1, uwOption);
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_IMAGE_RATIO, 1, uwOption);
            break;

        case IDM_EV:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_EV, 1, uwOption);
            break;

        case IDM_MOVIE_HDR:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_HDR, 1, uwOption);
            break;

        case IDM_MOVIE_CYCLIC_REC:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_CYCLIC_REC, 1, uwOption);
            break;

        case IDM_MOVIE_MOTION_DET:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOTION_DET, 1, uwOption);
            break;

        case IDM_MOVIE_AUDIO:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_AUDIO, 1, uwOption);
            break;

        case IDM_MOVIE_DATEIMPRINT:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_DATE_IMPRINT, 1, uwOption);
            break;

        case IDM_GSENSOR:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_GSENSOR,1,uwOption);
            break;
		case IDM_PARK_WAKEUP:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_PARK_WAKEUP,1,uwOption);
        break;

        default:
            Setup_MenuCallback(uiMessage, uiParam);
            break;
        }
    }

    return TMF_PROCESSED;
}
