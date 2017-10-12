/**
    Storage module driver.

    This file is the driver of storage module.

    @file       StorageDef.h
    @ingroup    mIDrvStorage
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _STORAGE_DEF_
#define _STORAGE_DEF_

#include "Constant.h"

/**
    @addtogroup mIDrvStorage
*/
//@{

/**
    @name SPI Quad read type

    @note for SPI_IDENTIFY
*/
//@{
#define     SPI_QUAD_NONE               0       ///< Not support
#define     SPI_QUAD_TYPE1              1       ///< Support quad read, QE(Quad Enable) bit is in Status Reigster[bit 6].
#define     SPI_QUAD_TYPE2              2       ///< Support quad read, QE(Quad Enable) bit  is in Status Register[bit 9].
//@}

/**
    SPI flash identify descriptor

    @note For STRG_IDENTIFY_CB
*/
typedef struct
{
    BOOL    bSupportEWSR;                       ///< EWSR(0x50) command capability
                                                ///< - @b TRUE: support EWSR command
                                                ///< - @b FALSE: NOT support EWSR command
    BOOL    bSupportAAI;                        ///< AAI program(0xAD) command capability
                                                ///< - @b TRUE: ONLY support AAI program command
                                                ///< - @b FALSE: NOT support AAI program command, but support page program command
    BOOL    bDualRead;                          ///< Dual read (0x3B) command capability
                                                ///< - @b TRUE: support dual read comand. (Command value should be 0x3B.)
                                                ///< - @b FALSE: NOT support dual read command
    UINT32  uiQuadReadType;                     ///< Quad read type capability. (ONLY support quad command 0xEB)
                                                ///< - @b SPI_QUAD_NONE: NOT support quad read command
                                                ///< - @b SPI_QUAD_TYPE1: Support quad read command, QE in Status Register bit 6
                                                ///< - @b SPI_QUAD_TYPE2: Support quad read command, QE in Status Register bit 9
    UINT32  uiFlashSize;                        ///< Flash total byte size. (unit: byte)
} SPI_IDENTIFY, *PSPI_IDENTIFY;

/**
    @name Storage Access CallBack

    Storage Access CallBack Prototype

    @return void
*/
//@{
typedef void (*STRG_ACCESS_CB)(void);   ///< Storage access call back prototype
//@}

/**
    @name Storage Identify CallBack

    Storage Identify CallBack Prototype

    @param[in] UINT32 uiMfgID   (JEDEC) manufacturer ID read from spi flash
    @param[in] UINT32 uiTypeID  (JEDEC) type ID read from spi flash
    @param[in] UINT32 uiCapacityID  (JEDEC) capacity ID read from spi flash
    @param[out] PSPI_IDENTIFY   flash identification returned to spi flash driver

    @return
        - @b TRUE: call back will handle identification of this flash. and PSPI_IDENTIFY will fill identifed information
        - @b FALSE: input ID is NOT supported/identified by call back
*/
//@{
typedef BOOL (*STRG_IDENTIFY_CB)(UINT32, UINT32, UINT32, PSPI_IDENTIFY);    ///< Storage identify call back prototype
//@}

/**
    @name Storage return value

    @note for flash_open(), flash_readSectors(), and flash_writeSectors()
*/
//@{
#define     E_OK                         0      ///< Success
#define     E_NAND_IDENTIFY_ERR         -1      ///< Read NAND ID (information) error
#define     E_NAND_INSUFFICINET_BLK     -2      ///< Bad block is too much to write data
#define     E_NAND_BLK_NOT_FOUND        -3      ///< Logic block number not found in physical block
#define     E_NAND_WRITE_PAGEDATA_ERR   -4      ///< Logic block number not found in physical block
#define     E_NAND_ERASEE_BLK_ERR       -5      ///< NAND erase block error
#define     E_NAND_READ_PAGE_ERR        -6      ///< NAND read page data error
#define     E_NAND_READ_BLK_ERR         -7      ///< NAND read block data error
#define     E_NAND_ID_NOT_SUPPORT       -8      ///< Not support NAND ID
#define     E_IDENTIFY_ERR              -9      ///< SPI IDENTIFY error
#define     E_SYS                       -10     ///< System Error
#define     E_OK_TABLE_FOUND             1      ///< Success & NAND table find
#define     E_OK_TABLE_NOT_FOUND         2      ///< Success & NAND table not find
//@}

/**
    @name Internal Storage Update Region

    @note for flash_readSectors(), and flash_writeSectors()
*/
//@{
#define     NAND_RW_LOADER               0      ///< Update Region is Loader (Update size is assumed 16 KB)
#define     NAND_RW_RESERVED             1      ///< Update Region is reserved area (Update size is assumed 16 KB)
#define     NAND_RW_FIRMWARE             2      ///< Update Region is FW (Update size is variable)
//@}

/**
    @name Internal Storage Configuration ID

    @note for flash_setConfig()
*/
//@{
#define     FLASH_CFG_ID_NONE                   0
#define     FLASH_CFG_ID_SPI_SUPPORT_4BITS      1   ///< Enable your project to support 4 bit SPI flash. ONLY enable when you ENSURE SPI_D0~SPI_D3 are connected to SPI flash in your PCB!!!
//@}

//#NT#2010/03/08#Steven Wang -begin
//#NT#Add in toggle LED in each write operation(NAND)
//#define     DATA_READ_OPERATION          1
//#define     DATA_WRITE_OPERATION         2
//#NT#2010/03/08#Steven Wang -end
//extern UINT32 _image_general_zi_zi_limit;
// Internal storage public API ( NAND / SPI flash)
/**
    Flash Open

    This function will initialize internal flash driver

    @note Internal flash is selected by STORAGEINT in MakeConfig.txt

    @return
        - @b E_OK: open success
        - @b E_IDENTIFY_ERR: identify fail (SPI path)
        - @b E_NAND_IDENTIFY_ERR: identify fail (NAND path)
        - @b E_NAND_ID_NOT_SUPPORT: not supported NAND flash
*/
extern INT32    flash_open(void);
/**
    Flash Close

    This function will close internal flash driver

    @note Internal flash is selected by STORAGEINT in MakeConfig.txt

    @return void
*/
extern void     flash_close(void);

/**
    Flash Read Sector

    This function will read data from internal flash

    @note Internal flash is selected by STORAGEINT in MakeConfig.txt

    @param[in] startblk         Start block of this read operation (unit: flash block)
    @param[in] length           Length of read operation (unit: byte)
    @param[out] buffer          Pointer to DRAM buffer to receive read data (word alignment)
    @param[in] updateType       Operation Access Region
                                - @b NAND_RW_LOADER: loader region
                                - @b NAND_RW_RESERVED: reserved area
                                - @b NAND_RW_FIRMWARE: f/w region

    @return
        - @b E_OK: read success
        - @b E_NAND_BLK_NOT_FOUND: can not find available physical block
        - @b E_NAND_READ_BLK_ERR: NAND access block error
        - @b E_NAND_READ_PAGE_ERR: NAND access page error
*/
extern INT32    flash_readSectors(UINT32 startblk, UINT32 length, UINT8 * buffer, UINT32 updateType);

/**
    Flash Write Sector

    This function will write data to internal flash

    @note Internal flash is selected by STORAGEINT in MakeConfig.txt

    @param[in] startblk         Start block of this write operation (unit: flash block)
    @param[in] length           Length of write operation (unit: byte)
    @param[in] buffer           Pointer to DRAM buffer of written data (word alignment)
    @param[in] updateType       Operation Access Region
                                - @b NAND_RW_LOADER: loader region
                                - @b NAND_RW_RESERVED: reserved area
                                - @b NAND_RW_FIRMWARE: f/w region

    @return
        - @b E_OK: read success
        - @b E_NAND_BLK_NOT_FOUND: can not find available physical block
        - @b E_NAND_ERASEE_BLK_ERR: erase block error
        - @b E_NAND_WRITE_PAGEDATA_ERR: page program error
        - @b E_NAND_INSUFFICINET_BLK: insufficient good block
*/
extern INT32    flash_writeSectors(UINT32 startblk, UINT32 length, UINT8 * buffer, UINT32 updateType);

/**
    Flash Get Block Size

    This function will return block size of attached internal flash

    @note Internal flash is selected by STORAGEINT in MakeConfig.txt

    @return block size (unit: byte)
*/
extern UINT32   flash_getBlockSize(void);

/**
    Flash Set Reserved Bad Block Number

    This function will set block count reserved to do bad block replacement

    @note Internal flash is selected by STORAGEINT in MakeConfig.txt

    @param[in] badBlkNum        reserved bad block count (unit: block)

    @return void
*/
extern void     flash_setReservedBadBlockNumber(UINT32 badBlkNum);

//#NT#2010/04/20#Steven Wang -begin
//#NT#Autumn reserved area from 2 ~ 23
/**
    Flash Set Reserved Area Block Number

    This function will set reserved area max block count

    @note Internal flash is selected by STORAGEINT in MakeConfig.txt

    @param[in] RsvMaxBlkNum     max reserved area block count (unit: block)

    @return void
*/
extern void     flash_setReservedAreaMaxBlockNumber(UINT32 RsvMaxBlkNum);
//#NT#2010/04/20#Steven Wang -end

/**
    Flash Get Total Size

    This function will return total size of attached internal flash

    @note Internal flash is selected by STORAGEINT in MakeConfig.txt

    @return total size (unit: byte)
*/
extern UINT32   flash_getTotalSize(void);

/**
    Install Flash Access CallBack

    This function will install access callback.
    The installed callback will be invoked when one block/page is programmed

    @note card is selected by STORAGEEXT in MakeConfig.txt

    @param[in] accessCB     installed callback

    @return void
*/
extern void     flash_installAccessCB(STRG_ACCESS_CB accessCB);

/**
    Install Flash Identify CallBack

    This function will install identification callback.
    The installed callback will be invoked when spi flash driver is opened.

    @note NAND driver will NOT invoke the installed call back

    @param[in] identifyCB   installed callback

    @return void
*/
extern void     flash_installIdentifyCB(STRG_IDENTIFY_CB identifyCB);

/**
    Flash Set Bus Frequency

    This function will set operating frequency of attached internal flash

    @note Internal flash is selected by STORAGEINT in MakeConfig.txt

    @param[in] uiFreq       Operating frequency
                            - @b SM_HOST_CLOCK_RATE_48_MHZ: 48 MHz for NAND
                            - @b SM_HOST_CLOCK_RATE_60_MHZ: 60 MHz for NAND (default of NAND)
                            - @b SM_HOST_CLOCK_RATE_96_MHZ: 96 MHz for NAND
                            - @b SPI_CLOCK_RATE_24_MHZ: 24 MHz for SPI (default of SPI)
                            - @b SPI_CLOCK_RATE_48_MHZ: 48 MHz for SPI
                            - @b SPI_CLOCK_RATE_60_MHZ: 60 MHz for SPI
                            - @b SPI_CLOCK_RATE_80_MHZ: 80 MHz for SPI

    @return void
*/
extern void     flash_setFrequency(UINT32 uiFreq);

/**
    Flash Set Configuration

    This function will set specific configuration for internal flash

    @note Internal flash is selected by STORAGEINT in MakeConfig.txt

    @param[in] uiConfigId   Configuration ID, can be:
                            - @b FLASH_CFG_ID_SPI_SUPPORT_4BITS: configure to support 4 bit SPI flash
    @param[in] uiContext    Configuration context for uiConfigId
                            When uiConfigId == FLASH_CFG_ID_SPI_SUPPORT_4BITS:
                            - @b TRUE: use 4 bit read when attached SPI flash support 4 bit read
                            - @b FALSE: won't use 4 bit read

    @return
        - @b E_OK: success
        - @b Else: fail
*/
extern INT32    flash_setConfig(UINT32 uiConfigId, UINT32 uiContext);

// External storage public API ( SDIO/MS/xD )
/**
    Open card driver

    This function will initialize card driver and register it to file system

    @note card is selected by STORAGEEXT in MakeConfig.txt

    @return
        - @b TRUE: open success
        - @b FALSE: open fail
*/
extern INT32    card_open(void);

/**
    Close card driver

    This function will close card driver

    @note card is selected by STORAGEEXT in MakeConfig.txt

    @return void
*/
extern void     card_close(void);

//@}

#endif
