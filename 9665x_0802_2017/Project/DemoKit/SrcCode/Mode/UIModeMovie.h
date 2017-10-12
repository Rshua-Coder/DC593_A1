/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIMode.h
    @ingroup    mIPRJAPUIFlow

    @brief      UI Mode Functions
                This file is the user interface ( for interchange flow control).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef _UIMODEMOVIE_H
#define _UIMODEMOVIE_H

#include "SysMain.h"
extern SYS_MODE gModeMovie;
extern int PRIMARY_MODE_MOVIE;   ///< Movie

//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
extern void FlowMovie_Open_FastSensor(void);
extern void FlowMovie_Open_FastLens(void);
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end

#endif //_UIMODEMOVIE_H
