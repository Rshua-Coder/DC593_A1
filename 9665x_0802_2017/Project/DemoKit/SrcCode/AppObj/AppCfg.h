/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       AppCfg.h
    @ingroup

    @brief

    @note       Nothing.
*/
#ifndef APP_CFG_H
#define APP_CFG_H

#include "Type.h"

//===========================================================================
// Module functions selection
// The following symbols may depends on these module functions selection.
// You must take care the library during linking time too.
//===========================================================================


////////////////////////////////////////////////////////////////////////////////
// Photo App
#define _IQ_BASE_NON_           0X80000000       /*disabl all Photo Tasks*/
#define _IQ_BASE_433_           0X00000001
#define _IQ_BASE_450_           0X00000002
#define _IQ_MODE_               _IQ_BASE_450_

////////////////////////////////////////////////////////////////////////////////
// Movie App
#define _MOVIE_ASFMPEG4_        0           // ASF MPEG-4
#define _MOVIE_AVIMJPEG_        1           // AVI MJPEG
#define _MOVIE_AVIMPEG4_        2           // AVI MPEG-4 (Not support)
#define _MOVIE_MP4MPEG4_        3           // MP4 MPEG-4 (Not support)
#define _MOVIE_MPEG1_           4
#define _MOVIE_MOVMJPG_         5
#define _MOVIE_MEDIA_           6
#define _MOVIE_MODE_            _MOVIE_AVIMJPEG_

////////////////////////////////////////////////////////////////////////////////
// Play App
#define _PLAYBACK_MODE_         ENABLE          // Enable/Disable Playback App functions

////////////////////////////////////////////////////////////////////////////////
// Music App
#define _MP3_MODE_              DISABLE         // Enable/Disable MP3 player function, 0 = Disable

////////////////////////////////////////////////////////////////////////////////
// Audio App
#define _WAV_FUNCTION_          ENABLE          // Enable/Disable WAV file record/playback function
#define _WAVSTUDIO_MODE_        ENABLE          // Enable/Disable WAV file record/playback function

////////////////////////////////////////////////////////////////////////////////
// Usb App
#define _USB_MODE_              ENABLE
////////////////////////////////////////////////////////////////////////////////
// GSP App
#define _GPS_RECEIVER_          DISABLE         // Enable/Disable GPS Reciver function

////////////////////////////////////////////////////////////////////////////////
// G-Sensor App
#define _G_SENSOR_              DISABLE          // Enable/Disable G Sensor Reciver function

////////////////////////////////////////////////////////////////////////////////
// Touch App
#define _TOUCH_                 DISABLE          // Enable/Disable Touch Reciver function

////////////////////////////////////////////////////////////////////////////////
// File System
#define _FILESYS_FAST_MOUNT     ENABLE          // Enable/DisableFile System Fast Mount function

#endif //APP_CFG_H