/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIDef.h
    @ingroup    mIPRJAPUIFlow

    @brief      UIDef Functions
                This file is the user interface ( for LIB callback function).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef NVT_PLAYBACK_API_H
#define NVT_PLAYBACK_API_H

#include "GxLib.h"
#include "MediaWriteLib.h"
#include "AudFilterAPI.h"

/**
     @name Configuration ID for DscPlay_Config().
*/
//@{
#define PLAYBACK_CFG_POOL       1   ///< Pool address and size
//@}
/**
    Configuration for playback mode.
    @param[in] config_id Configuration ID.
    @param[in] value Setting value.
*/
void DscPlay_Config(UINT32 config_id, UINT32 value);

/**
    Open playback mode.
*/
void DscPlay_Open(void);
/**
    Close playback mode.
*/
void DscPlay_Close(void);
extern VIEW_SRC gMoviePlay_src;
extern VIEW_SRC gPlay_src;

#endif//NVT_PLAYBACK_API_H

