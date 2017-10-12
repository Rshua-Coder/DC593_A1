/**
    @file       bl_func.h
    @brief      Header file for main function

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _BL_MAIN_H
#define _BL_MAIN_H

#include "main.h"
#include "IOReg.h"
#include "GPIO.h"

//------------------------------------------------------------------------------
// Compiler option
//
// These compiler option are define in MakeConfig.txt
//!!!!!!!!!!!!!!!!!!! DO NOT MODIFY COMPILER OPTION HERE !!!!!!!!!!!!!!!!!!!!
//------------------------------------------------------------------------------
// Define SPECIAL_KEY_UPDATE == ENABLE will require user to press special key
// to update firmware
#ifndef SPECIAL_KEY_UPDATE
#define SPECIAL_KEY_UPDATE          DISABLE
#endif

// Define LED_FUNCTION == ENABLE will enable LED on/off in boot loader
#define LED_FUNCTION                DISABLE

// FW checking method, support checksum & CRC now
//#define FW_CHECK_NOCHECK            0
//#define FW_CHECK_CHECKSUM           1
//#define FW_CHECK_CRC                2
#ifndef FW_CHECK_METHOD
#define FW_CHECK_METHOD             FW_CHECK_CHECKSUM
#endif

///#define FW_CHECK_INSTRUCTION0       0xE59FF018

// Optional pseudo string for checksum & CRC checking
#define FW_CHECK_PSEUDO_STR_EN      DISABLE
#if (FW_CHECK_PSEUDO_STR_EN == ENABLE)
    // Must sync to MakeConfig.txt
    #define FW_CHECK_PSEUDO_STR     "DSC_FW"
#endif

// Determine if need to support compressable main code
#ifndef _MAINCODE_COMPRESS_
#define _MAINCODE_COMPRESS_ 0
#endif

#ifndef STORAGEINT_SPI
#define STORAGEINT_SPI      0
#endif

//#NT#2011/06/30#Steven Wang -begin
//#NT#NTK self test flow enable / disable
#define FW_EXCEED_16MB_TEST         DISABLE
//#NT#2011/06/30#Steven Wang -end

//------------------------------------------------------------------------------
// Global definition
//------------------------------------------------------------------------------
/*
   FW Code Size definiton (Defined in MakeConfig.txt)
   User can redefine this size in MakeConfig.txt,
   If you want to redefine this size, Please consider nand driver of main code.
*/
// FW_CODE_SIZE Definition ==> FW Maximun binary size storage in NAND
#ifndef FW_MAX_CODE_SIZE
    #define FW_MAX_CODE_SIZE        0x00700000      // Just for C code editor reference
#endif
// FW_CODE_SIZE Definition
#define CALFW_MAX_CODE_SIZE         0X00300000      // Maximun Calibration code size (9MB)

#define LOADER_CODE_SIZE            0x4000      // Fixed to 16 KB
//#NT#2011/06/10#Scottie -begin
//#NT#Remove, modify the mechanism of recording FW_CODE_SIZE & CALFW_CODE_SIZE

#define RESERVED_DATA_SIZE          0x4000      // Fixed to 16 KB

//#NT#2011/06/10#Scottie -end

#define SDRAM_Start_FW              0x80000000  // DRAM FW starting address

#define SRAM_Start_FW               0xB07F0000  // SRAM FW starting address

#define SRAM_TAG                    0xB0000000  // SRAM tag

// Loader checking
#define LOADER_TAG_OFFSET           0x30
#define LOADER_TAG_VALUE            0xAA55

//#NT#2011/06/30#Steven Wang -begin
//#NT#Checksum offset
///#define CHECKSUM_OFFSET             0x1C
#define CHECKSUM_TAG                0xAA55
//#NT#2011/06/30#Steven Wang -end

// Config table tag value (for USB flow)
#define CFG_TABLE_TAG               (0x4B54564E)        // 'N' 'V' 'T' 'K'

#define COMPRESS_TAG_VALUE          (0x314C4342)        // use to distinguish full compressed main code

#define SPI_FLASH_IDENTIFY          (0x46495053)        //'S''P''I''F'

#define BOOTVER_SET(OutMaj,OutMin,InnMaj,InnMin)  (((OutMaj)<<24)|((OutMin)<<16)|((InnMaj)<<8)|((InnMin)))

#define BOOTVER_GETOuterMajor(Ver)  (((Ver)&0xFF000000)>>24)
#define BOOTVER_GETOuterMinor(Ver)  (((Ver)&0x00FF0000)>>16)
#define BOOTVER_GETInnerMajor(Ver)  (((Ver)&0x0000FF00)>>8)
#define BOOTVER_GETInnerMinor(Ver)  (((Ver)&0x000000FF))

// Define loader version (_LDR_VER_) in MakeConfig.txt
#define BOOTLOADER_OUTERMAJOR       (((_LDR_VER_)&0xFF000000)>>24)
#define BOOTLOADER_OUTERMINOR       (((_LDR_VER_)&0x00FF0000)>>16)
#define BOOTLOADER_INNERMAJOR       (((_LDR_VER_)&0x0000FF00)>>8)
#define BOOTLOADER_INNERMINOR       (((_LDR_VER_)&0x000000FF))

// Binary tag value
#define FW_BINARY_TAG               0xAA55544E  // 'N' 'T' 0x55 0xAA

// CAL FW size exit tag
#define CAL_SIZE_TAG                0x5A534C43  // CLSZ
// FW size exit tag
#define FW_SIZE_TAG                 0x5A535746  // FWSZ

// Reserved area FW size offset
#define RESERVED_AREA_FW_SIZE_OFFSET    0x00
// Reserved area CAL size offset
#define RESERVED_AREA_CAL_SIZE_OFFSET   0x04
// Reserved area FW size tag offset
#define RESERVED_AREA_FW_TAG_OFFSET     0x08
// Reserved area CAL size tag offset
#define RESERVED_AREA_CAL_TAG_OFFSET    0x0C

// Operation of boot loader
#define FUNC_UPDATE_FW              0x01        // Read FW code from card and update to NAND and run
#define FUNC_UPDATE_LOADER          0x02        // Read boot loader code from card and update to NAND
#define FUNC_RUN_CARD               0x04        // Read FW code from card and run
#define FUNC_RUN_FLASH              0x08        // Read FW code from internal flash and run
#define FUNC_UPDATE_CAL             0X10        // Update CAL data
#define FUNC_UPDATE_FW_DONE         0x0100      // update f/w to NAND/SPI done

// API to turn on / off LED
#if (LED_FUNCTION == ENABLE)
// Red LED (DGPIO 6)
// Green LED (DGPIO 9)
#define TurnOff_RedLED()            OUTREG32(CPE_GPIO_BASE + 0x4C, 0x00000040)
#define TurnOn_RedLED()             OUTREG32(CPE_GPIO_BASE + 0x38, 0x00000040)
#define TurnOff_GreenLED()          OUTREG32(CPE_GPIO_BASE + 0x4C, 0x00000200)
#define TurnOn_GreenLED()           OUTREG32(CPE_GPIO_BASE + 0x38, 0x00000200)
#else
#define TurnOff_RedLED()
#define TurnOn_RedLED()
#define TurnOff_GreenLED()
#define TurnOn_GreenLED()
#endif


#if (STORAGEINT_SPI == 1)
    #define StartNandBlkUpdateLoader    0
    #define StartNandBlkReserved        0
    //#NT#2012/12/17#HH Chuang -begin
    //#NT#Sync spi erase unit to best access size of f/w storage object
    #define StartNandBlkUpdateFW        4
    //#NT#2012/12/17#HH Chuang -end
#else

  #if (_PARAMETER_IN_NAND_BLK2_ == ENABLE)
    #define StartNandBlkUpdateLoader    0
    #define StartNandBlkReserved        2
    #define StartNandBlkUpdateFW        (NAND_TOLERANT_BAD_BLK + StartNandBlkReserved)
  #else
    #define StartNandBlkUpdateLoader    0
    #define StartNandBlkUpdateFW        2
  #endif

#endif

#define NAND_TOLERANT_BAD_BLK       80
//#NT#2010/11/17#Scottie -begin
//#NT#Refine the NAND block related code & reduce code size
#define NAND_BLOCK_SIZE             0x4000
#define FW_RESERVE_BLOCK            (FW_MAX_CODE_SIZE / NAND_BLOCK_SIZE)
#define StartNandBlkUpdateCALFW     FW_RESERVE_BLOCK
//#NT#2010/11/17#Scottie -end

#if (FW_CHECK_METHOD == FW_CHECK_CRC)
static UINT32 g_uiCRCTable[256];
#endif

// Card detect GPIO offset
#define DGPIO_CARD_DETECT           (2 | GPIO_IS_DGPIO)


#define CHECK_CARD_EXIST            (ENABLE)
#if (CHECK_CARD_EXIST == ENABLE)
// MC16 default is config as input and pull up
#define bl_checkCardExist()         (gpio_getPin(DGPIO_CARD_DETECT) == 0 ? TRUE : FALSE)
#else
#define bl_checkCardExist()         TRUE
#endif



//#NT#2010/05/21#Steven Wang -begin
//#NT#Check if FW size > EXEC base address
extern UINT32 _load_BOOT_EXEC_start_base;
//#NT#2010/05/21#Steven Wang -end

//------------------------------------------------------------------------------
// Exported functions
//------------------------------------------------------------------------------
extern int bl_mainFlow(void) __attribute__ ((far));
extern UINT32 BaseOfStack;
#endif
