/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       LensCtrlFlag.h
    @ingroup    mIPRJAPCfg

    @brief      Header file for lens control task

    @note       Nothing.

    @date       2010/05/19
*/

#ifndef _LENSCTRLFLAG_H
#define _LENSCTRLFLAG_H

/** \addtogroup mIPRJAPCfg */
//@{

//
//FLG_ID_LENS
//
#define FLGLENS_ZOOMFWD                 0x00000001        ///< Zoom forward
#define FLGLENS_ZOOMREV                 0x00000002        ///< Zoom reverse
#define FLGLENS_ZOOMBRK                 0x00000004        ///< Zoom brake
#define FLGLENS_ZOOMOFF                 0x00000008        ///< Zoom off
#define FLGLENS_ZOOMCHANGE              0x00000010        ///< Zoom section is changed
#define FLGLENS_ZOOM_CHANGESPPED        0x00000040        ///< Zoom speed change
#define FLGLENS_FOCUS_SETSTATE          0x00000100        ///< set Focus state
#define FLGLENS_APERTURE_SETSTATE       0x00000200        ///< set Aperture state
#define FLGLENS_SHUTTER_SETSTATE        0x00000400        ///< set Shutter state
#define FLGLENS_IDLE                    0x80000000        ///< LensCtrlTsk is idle

#define FLGLENS_ALL                     0xFFFFFFFF        ///< Flag mask
//
//FLG_ID_LENS2
//
#define FLGLENS2_WRITE_LENS_STATUS      0x00000001        ///< Record lens status
#define FLGLENS2_READ_LENS_STATUS       0x00000002        ///< Read lens status
#define FLGLENS2_FOCUSMOVE              0x00000004        ///< Reserved
#define FLGLENS2_IDLE                   0x80000000        ///< LensCtrlTsk2 is idle

#define FLGLENS2_ALL                    0xFFFFFFFF        ///< Flag mask

//@}
#endif

