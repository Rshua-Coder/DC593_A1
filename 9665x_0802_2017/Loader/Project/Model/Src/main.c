/*
    Main control function

    This file is implement by user mode

    @file       main.c

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#include "main.h"
#include "bl_func.h"
#include "cache.h"

extern char _loader_exec_compres_start[];
extern char _loader_exec_compress_load_cpu_addr[];

//---------------------------------------------------------------------------
// Static function
//---------------------------------------------------------------------------
/*
    BL_RunPartOne

    Expend loader if use compress

    @return void
*/
static void BL_RunPartOne(void)
{
#if (_LOADER_COMPRESSED_ == ENABLE)
    UINT8 *in_addr = (UINT8 *)(_loader_exec_compress_load_cpu_addr);
    UINT8 *out_addr = (UINT8 *)(_loader_exec_compres_start);
    UINT32 uiCompressedOutSize;

    // Get decompress size within specific header
    uiCompressedOutSize = (((UINT32)*(in_addr + 12)) << 24) +
                          (((UINT32)*(in_addr + 13)) << 16) +
                          (((UINT32)*(in_addr + 14)) <<  8) +
                          ((UINT32)*(in_addr + 15));

    // Decompress loader image
    LZ_Uncompress((unsigned char *)(in_addr + LDC_HEADER_SIZE), (unsigned char *)(out_addr), uiCompressedOutSize);

    // Need to clean data cache
    CPUInvalidateICacheAll();
    CPUCleanInvalidateDCacheAll();
///    uart_putSystemUARTStr("\r\nUNZOK!");
#else
#endif
}

void BL_PostHandle(void)
{
    UINT32  uiParamCnt;
    UINT32  uiLoaderAddress;

    uiLoaderAddress = (UINT32)&_load_BOOT_EXEC_start_base;

    for(uiParamCnt = 0; uiParamCnt < 6*4; uiParamCnt +=4)
    {
        OUTREG32(DDR_PARAM_LEVEL_0_ADDR + uiParamCnt, INREG32((uiLoaderAddress) + DDR_LEVEL0_OFFSET + uiParamCnt));
    }

    for(uiParamCnt = 0; uiParamCnt < 6*4; uiParamCnt +=4)
    {
        OUTREG32(DDR_PARAM_LEVEL_1_ADDR + uiParamCnt, INREG32((uiLoaderAddress) + DDR_LEVEL1_OFFSET + uiParamCnt));
    }

    //invalid Instruciton and data cache
    CPUCleanInvalidateDCacheAll();
    CPUInvalidateICacheAll();
}

int main(void)
{
    int iReturn;

    // Run part one loader
    // If using compressed loader, just decompress the loader image at first!
    BL_RunPartOne();

    iReturn = bl_mainFlow();

    BL_PostHandle();

    return iReturn;
}
