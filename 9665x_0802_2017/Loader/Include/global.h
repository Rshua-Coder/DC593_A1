/**
    Global utility header file

    Global utility header file

    @file       global.h
    @ingroup    mISYSUtil
    @note       Nothing
*/

#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "constant.h"

/**
    @addtogroup mISYSUtil
*/
//@{

extern UINT32   bitCount(UINT32 data);
extern void     UTL_setDrvTmpBufferAddress(UINT32 addr);
extern UINT32   UTL_getDrvTmpBufferAddress(void);
extern char*    Dec2HexStr(UINT32 data);

//@}

#endif
