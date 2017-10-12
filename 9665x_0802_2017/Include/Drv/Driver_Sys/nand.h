/**
    @file       NAND.h

    @ingroup    mIStgNAND

    @brief      Nand flash driver export variables and function prototypes.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

*/

#ifndef _NAND_H
#define _NAND_H

#ifndef _NAND2_SUPPORT_
#define _NAND2_SUPPORT_ 1
#endif

#include "StrgDef.h"

/**
    @addtogroup mIStgNAND
*/
//@{

//------------------------------------------------------------------------------
// compatible with earily code base
//------------------------------------------------------------------------------
/**
    @name Backward compatible APIs

    For old version NAND driver
*/
//@{
#define EraseAllNANDBlockMaping()           nand_eraseAllPartitionBlockMapping(FALSE, FALSE)
//@}

// -----------------------------------------------------------------------------
// NAND driver error code
// -----------------------------------------------------------------------------
//
#define NAND_ERROR_OK                       0x00000000
#define NAND_ERROR_INSUFFICIENT_FREE_BLOCK  0x00010000
#define NAND_ERROR_TOO_MANY_BAD_UNITS       0x00080000 //Too many bad blocks result in write protection rasen.(write and erase is prohibited)
#define NAND_ERROR_DATA_STATUS              0x00000001 //Data status is corrupt
#define NAND_ERROR_ECC_FIELD1               0x00000002
#define NAND_ERROR_ECC_FIELD2               0x00000004
#define NAND_ERROR_DOUBLE_MAPPING           0x00000008
#define NAND_ERROR_UNCORRECT_ECC            0x00000080
#define NAND_ERROR_READ_ERROR               0x00000010
#define NAND_ERROR_WRITE_ERROR              0x00000020
#define NAND_ERROR_DEVICE_ERROR             0x00000040


/**
    NAND write one reserved block param
*/
typedef struct
{
    UINT32  uiLogicBlock;
    UINT32  uiPhysicalBlock;
} NAND_WRITERSVCFG, *PNAND_WRITERSVCFG;

/*
    NAND specific operation condition definition
\n  Some condition can not process anytime. Need turn on/off flag

    @note for nand_setFeature(), nand_getFeature()
*/
typedef enum
{
    NAND_ERASE_LOADER,      //< TRUE: valid for erasing loader area
    NAND_ERASE_FW,          //< TRUE: valid for erasing FW area
    NAND_ERASE_PSTORE,      //< TRUE: valid for erasing Pstore area
    NAND_MARK_PSTORE,       //< TRUE: valid for mark bad block at Pstore area
    ENUM_DUMMY4WORD(NAND_FEATRUE)
} NAND_FEATRUE;

/**
    NAND configuration identifier

    @note For nand_setConfig()
*/
typedef enum
{
    NAND_CONFIG_ID_FREQ,                    ///< NAND module clock (Unit: MHz), MUST config before storage object open, active after open
                                            ///< Context can be one of the following:
                                            ///< - @b 48    : 48 MHz (Default value)
                                            ///< - @b 60    : 60 MHz
                                            ///< - @b 96    : 96 MHz
    NAND_CONFIG_ID_TIMING0,                 ///< NAND controller timing0 register, MUST config before storage object open and need config timing1 at the same time
                                            ///< Context is UINT32 value
    NAND_CONFIG_ID_TIMING1,                 ///< NAND controller timing1 register, MUST config before storage object open and need config timing0 at the same time
                                            ///< Context is UINT32 value

    NAND_CONFIG_ID_DELAY_LATCH,             ///< Context can be one of the following:
                                            ///< - @b TRUE  : 0.5T delay latch
                                            ///< - @b FALSE : 0.0T delay latch
                                            ///<   @note: TRUE equal to 0.5 tRP

    NAND_CONFIG_ID_SET_INTEN,               ///< Context can be one of the following:
                                            ///< - @b TRUE  : Enable interrupt mode
                                            ///< - @b FALSE : Disable interrupt mode
                                            ///<   @note: Need config before any access of storage object hook on NAND device open\n
                                            ///<          such as STRG_SET_PARTITION_SIZE, STRG_SET_PARTITION_RSV_SIZE, \n
                                            ///<          STRG_GET_BEST_ACCESS_SIZE, STRG_GET_SECTOR_SIZE, STRG_GET_DEVICE_PHY_SIZE,\n
                                            ///<          STRG_GET_MEMORY_SIZE_REQ
                                            ///< @code
                                            ///     if(nand_setConfig(NAND_CONFIG_ID_SET_INTEN, TRUE) != E_OK)
                                            ///     {
                                            ///         ERROR;
                                            ///     }
                                            ///     //Enable Interrupt mode
                                            //      if(nand_setConfig(NAND_CONFIG_ID_SET_INTEN, FALSE) != E_OK)
                                            ///     {
                                            ///         ERROR;
                                            ///     }
                                            ///     //Disable Interrupt mode
                                            ///  }
                                            ///  @endcode
    NAND_CONFIG_ID_AUTOPINMUX,              ///< Context can be one of the following:
                                            ///< - @b TRUE  : disable pinmux when NAND driver close
                                            ///< - @b FALSE : not disable pinmux when NAND driver close

    ENUM_DUMMY4WORD(NAND_CONFIG_ID)
} NAND_CONFIG_ID;

extern ER           nand_emuReadPageSpareDataTotal(UINT32 *spare0,UINT32 *spare1, UINT32 *spare2, UINT32 *spare3, UINT32 pageAddress);
extern ER           nand_setFeature(NAND_FEATRUE Nand_Feature, UINT32 uiParameter);
#if _NAND2_SUPPORT_
extern PSTRG_TAB nand2_getStorageObject(void);
extern ER        nand2_writeSectors(INT8 *pcBuf, UINT32 ulSecNum, UINT32 ulSctCnt);
extern ER        nand2_readSectors(INT8 *pcBuf, UINT32 ulSecNum, UINT32 ulSctCnt);
extern void      nand2_setSize(UINT32 size);
ER               nand2_format(void);
void             nand2_FormatEraseNAND(void);
#endif
extern ER           nand_setConfig(NAND_CONFIG_ID ConfigID, UINT32 uiConfig);
extern void         nand_dumpInfo(void);
extern void         nand_dumpAllData(UINT32 uiBuf, UINT32  uiSize);
extern void         nand_eraseAllPartitionBlockMapping(BOOL bForceErMarked, BOOL bForceErDefected);
extern PSTORAGE_OBJ nand_getStorageObject(STRG_OBJ_ID strgObjID);

//@}

#endif // _NAND_H


