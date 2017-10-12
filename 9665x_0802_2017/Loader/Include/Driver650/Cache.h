/**
    CPU module driver.

    This file is the driver of CPU module.

    @file       Cache.h
    @ingroup    mIHALCore
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _CACHE_H
#define _CACHE_H
#include "constant.h"

/**
    @addtogroup mIHALCore
*/
//@{

/**
    @name CPU clean invalidate all DCache

    Clean and invalidate all data cache

    @return void
*/
//@{
#define CPUCleanInvalidateDCacheAll()       cpu_cleanInvalidateDCacheAll()          ///< clean invalidate all DCache
//@}

/**
    @name CPU clean invalidate DCache

    Clean and invalidate data cache
    Cache line associated with input address will be clean and invalidated

    @param[in] addr     data address to be clean and invalidated

    @return void
*/
//@{
#define CPUCleanInvalidateDCache(addr)      cpu_cleanInvalidateDCache(addr)         ///< clean invalidate DCache
//@}

/**
    @name CPU clean invalidate DCache block

    Clean and invalidate data cache block
    Cache line associated with input region will be clean and invalidated

    @param[in] m        starting data address to be clean and invalidated
    @param[in] n        end data address to be clean and invalidated

    @return void
*/
//@{
#define CPUCleanInvalidateDCacheBlock(m,n)  cpu_cleanInvalidateDCacheBlock(m, n)    ///< clean invalidate DCache block
//@}

/**
    @name CPU invalidate DCache

    Invalidate data cache
    Cache line associated with input address will be invalidated

    @param[in] addr     data address to be invalidated

    @return void
*/
//@{
#define CPUInvalidateDCache(m)              cpu_invalidateDCache(m)                 ///< invalidate DCache
//@}

/**
    @name CPU invalidate DCache block

    Invalidate data cache block
    Cache line associated with input region will be invalidated

    @param[in] m        starting data address to be invalidated
    @param[in] n        end data address to be invalidated

    @return void
*/
//@{
#define CPUInvalidateDCacheBlock(m,n)       cpu_invalidateDCacheBlock(m,n)          ///< invalidate DCache block
//@}

/**
    @name CPU clean DCache

    Clean data cache
    Cache line associated with input address will be clean

    @param[in] addr     data address to be clean

    @return void
*/
//@{
#define CPUCleanDCache(m)                   cpu_cleanDCache(m)                      ///< clean DCache
//@}

/**
    @name CPU clean DCache block

    Clean data cache block
    Cache line associated with input region will be clean

    @param[in] m        starting data address to be clean
    @param[in] n        end data address to be clean

    @return void
*/
//@{
#define CPUCleanDCacheBlock(m, n)           cpu_cleanDCacheBlock(m, n)              ///< clean DCache block
//@}

/**
    @name CPU invalidate all DCache

    Invalidate all data cache

    @return void
*/
//@{
#define CPUInvalidateDCacheAll()            cpu_invalidateDCacheAll()               ///< invalidate all DCache
//@}


extern BOOL     CPUChkDCacheEnabled(UINT32 addr);
extern void     CPUInvalidateICacheAll(void) __attribute__ ((section (".part1"), far));
//extern void     CPUInvalidateICache(UINT32 addr);
extern void     CPUInvalidateICacheBlock(UINT32 start, UINT32 end);

#if 0
extern void     CPUInvalidateDCacheAll(void);
extern void     CPUInvalidateDCache(UINT32 addr);
extern void     CPUInvalidateDCacheBlock(UINT32 start, UINT32 end);
#endif
extern void     cpu_invalidateDCacheBlock(UINT32 start, UINT32 end);
extern void     cpu_invalidateDCache(UINT32 addr);
extern void     cpu_invalidateDCacheAll(void);
extern void     cpu_cleanDCache(UINT32 addr);
extern void     cpu_cleanDCacheBlock(UINT32 start, UINT32 end);

extern void     cpu_cleanInvalidateDCacheAll(void)__attribute__ ((section (".part1"), far));
extern void     cpu_cleanInvalidateDCache(UINT32 addr);
extern void     cpu_cleanInvalidateDCacheBlock(UINT32 start, UINT32 end);

extern void     CPUflushReadCache(UINT32 uiStartAddr, UINT32 uiLength);
extern void     CPUflushWriteCache(UINT32 uiStartAddr, UINT32 uiLength);



#endif

//@}
