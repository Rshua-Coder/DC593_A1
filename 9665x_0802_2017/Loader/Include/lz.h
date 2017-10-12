/**
    Decompress library header file

    Decompress library header file

    @file       lz.h
    @ingroup    mISYSUtil
    @note       Nothing
*/
#ifndef __NVT_LZ_DEF__
#define __NVT_LZ_DEF__

#include "constant.h"

/**
    @addtogroup mISYSUtil
*/
//@{

extern void LZ_Uncompress(UINT8 *in, UINT8 *out, UINT32 insize)  __attribute__ ((section (".part1"), far));

//@}

#endif
