/**
    @file       main.h
    @brief      Header file for main function

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _MAIN_H
#define _MAIN_H

#include "constant.h"
#include "fuart.h"

#if (_LOADER_COMPRESSED_ == ENABLE)
// header size for compressed loader (16 bytes)
#define LDC_HEADER_SIZE     16

#include "lz.h"
#endif

#define DDR_PARAM_LEVEL_0_ADDR      (BOOT_ROM_RAM_TEMP_ADDR)

#define DDR_PARAM_LEVEL_1_ADDR      (DDR_PARAM_LEVEL_0_ADDR + 6 * 4)

#define DDR_PARAM_LEVEL_2_ADDR      (DDR_PARAM_LEVEL_0_ADDR + 6 * 4 * 2)


// Represent
// SPI : 0x46495053     //'S''P''I''F'
//NAND : (1)            // Table found
//       (2)            // Table not found

#define NAND_TABLE_FLAG_ADDR        (DDR_PARAM_LEVEL_0_ADDR + 6 * 4 * 3)

#define NAND_TABLE_VERSION_ADDR     (NAND_TABLE_FLAG_ADDR + 4)

#define DDR_LEVEL0_OFFSET           0x10

#define DDR_LEVEL1_OFFSET           0x200

extern int main(void) __attribute__ ((section (".part1")));
extern void BL_PostHandle(void) __attribute__ ((far));

#endif
