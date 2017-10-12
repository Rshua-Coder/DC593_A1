/**
    Header file for DMA module.

    This file is the header file that define the API for DMA module.

    @file       DMA.h
    @ingroup    mIHALDMA
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _DMA_H
#define _DMA_H

#include "Type.h"

/**
    @addtogroup mIHALDMA
*/

/**
    Check DRAM address is cacheable or not

    Check DRAM address is cacheable or not.

    @param[in] addr     DRAM address

    @return Cacheable or not
        - @b TRUE   : Cacheable
        - @b FALSE  : Non-cacheable
*/
#define dma_isCacheAddr(addr)           ((((addr) & 0x20000000) == 0x20000000) ? FALSE : TRUE)

/**
    Translate DRAM address to cacheable address.

    Translate DRAM address to cacheable address.

    @param[in] addr     DRAM address

    @return Cacheable DRAM address
*/
#define dma_getCacheAddr(addr)          (((addr) & 0x1FFFFFFF) | 0x80000000)

/**
    Flush read (Device to CPU) cache with checking address is cacheable or not.

    In DMA operation, if buffer is cacheable, we have to flush read buffer before
    DMA operation to make sure CPU will read correct data.

    @param[in] uiAddr   DRAM address
    @param[in] uiSize   DRAM size
*/
#define dma_flushReadCache(uiAddr, uiSize) \
{ \
    if (dma_isCacheAddr(uiAddr) == TRUE) \
    { \
        dma_flushReadCacheWithoutCheck(uiAddr, uiSize); \
    } \
}

/**
    Flush write (CPU to Device) cache with checking address is cacheable or not.

    In DMA operation, if buffer is cacheable, we have to flush write buffer before
    DMA operation to make sure DMA will send correct data.

    @param[in] uiAddr   DRAM address
    @param[in] uiSize   DRAM size
*/
#define dma_flushWriteCache(uiAddr, uiSize) \
{ \
    if (dma_isCacheAddr(uiAddr) == TRUE) \
    { \
        dma_flushWriteCacheWithoutCheck(uiAddr, uiSize); \
    } \
}


/**
    Flush read (Device to CPU) cache where lineoffset not equal to width

    Once if case like these two condition as show below, need call
    dma_flushReadCacheWidthNEQLineOffset() instead of dma_flushReadCache()

    Example:
    @code
    {
        case(1): image lineoffset != width
         image width
         <-------------->
        start
         +--------------+----+
         |--------------|xxxx|
         |--------------|xxxx|
         |--------------|xxxx|
         |--------------|xxxx|
         +--------------+----+<--end
         <------------------->
         image line offset

         case(2):
            (a) CPU put data into _addr
            (b) Engine read from _addr and process
            (c) Engine write(output) data back to _addr
            (d) CPU read back from _addr
    }
    @endcode

    @param[in] uiAddr   DRAM address
    @param[in] uiSize   DRAM size
*/
#define dma_flushReadCacheWidthNEQLineOffset(uiAddr, uiSize) \
{ \
    if (dma_isCacheAddr(uiAddr) == TRUE) \
    { \
        dma_flushReadCacheWidthNEQLineOffsetWithoutCheck((uiAddr), (uiSize)); \
    } \
}

/**
    DRAM type encoding

    DRAM type encoding returned by dma_getDramType()
*/
typedef enum
{
    DDR_TYPE_DDR2,          ///< DDR3 DRAM
    DDR_TYPE_DDR3,          ///< DDR2 DRAM
    ENUM_DUMMY4WORD(DMA_DDR_TYPE)
} DMA_DDR_TYPE;

extern DMA_DDR_TYPE     dma_getDramType(void);
extern BOOL             dma_flushReadCacheWithoutCheck(UINT32 uiStartAddr, UINT32 uiLength);
extern BOOL             dma_flushWriteCacheWithoutCheck(UINT32 uiStartAddr, UINT32 uiLength);
extern BOOL             dma_flushReadCacheWidthNEQLineOffsetWithoutCheck(UINT32 uiStartAddr, UINT32 uiLength);
extern UINT32           dma_getDramBaseAddr(void);

//@}

#endif
