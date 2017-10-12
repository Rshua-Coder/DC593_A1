/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppVerInfo.h
    @ingroup

    @brief      Header file of application version and build date information
                Header file of application version and build date information

    @note       Nothing.

    @version    V1.00.000
    @author     Chris Hsu
    @date       2005/12/28
*/

/** \addtogroup */
//@{

#ifndef _APPVERINFO_H
#define _APPVERINFO_H

#include "Type.h"

extern UINT8 *App_GetVerInfo(void);
extern UINT8 *App_GetBuildDate(void);

#endif

//@}
