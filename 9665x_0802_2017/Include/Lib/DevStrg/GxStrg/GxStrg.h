/**
    Header file for storage mount and control.

    @ingroup    mILibGxStrg
    @file       GxStrg.h

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/
#ifndef _GXSTRG_H
#define _GXSTRG_H

#include "GxCommon.h"
#include "DxType.h"

/**
    @addtogroup mILibGxStrg
*/
//@{
//------------------------------------------------------------------------------
// Definitions
//------------------------------------------------------------------------------
/**
    @name   STRG_CB Types
    The event handler type of the storage callback
*/
//@{
#define STRG_CB_UNKNOWN         0   ///< The type for unknown card status
#define STRG_CB_INSERTED        1   ///< The type for calling after card inserted
#define STRG_CB_REMOVED         2   ///< The type for calling after card removed
#define STRG_CB_MOUNT_FINISH    3   ///< The type for calling after storage mounted
#define STRG_CB_UNMOUNT_FINISH  4   ///< The type for calling after storage unmounted
//@}

/**
    @name   FILE_CFG Flags
    The flags for configuring the file system
*/
//@{
#define FILE_CFG_BUF            0   ///< The flag for configuring the file system working buffer
#define FILE_CFG_SUPPORT_EXFAT  1   ///< The flag for configuring the file system exFAT support
//@}

//------------------------------------------------------------------------------
// Enumerations
//------------------------------------------------------------------------------
/**
    Storage Type
*/
typedef enum
{
    //Storage type
    PRIMARY_STGTYPE_START_ID = 0,   ///< No valid storage type is selected
    PRIMARY_STGTYPE_NAND,           ///< Storage type is on board NAND flash
    PRIMARY_STGTYPE_CARD,           ///< Storage type is card
    PRIMARY_STGTYPE_CARD2,          ///< Reserved for twin card slot, but not for current chip revision
    PRIMARY_STGTYPE_END_ID          ///< The end id of the enumerator PRIMARY_STGTYPE
}
PRIMARY_STGTYPE;

/**
    Card Status
*/
typedef enum
{
    CARD_POWER=0,                   ///< The value of the card power
    CARD_INSERT,                    ///< The status of the card inserted
    CARD_READONLY                   ///< The status of the card read-only
}Gx_Card;


//------------------------------------------------------------------------------
// Export Functions
//------------------------------------------------------------------------------
/**
    Set the initial parameters of the current storage device.

    You can set the working buffer address, size, and the exFAT support option.

    @param[in]  cfg     The type of the initial parameters. (Options: #FILE_CFG_BUF/ #FILE_CFG_SUPPORT_EXFAT)
    @param[in]  value   The value of the initial parameters you want to set.
        - @b For #FILE_CFG_BUF, the value format should be a pointer of a MEM_RANGE structure.
        - @b For #FILE_CFG_SUPPORT_EXFAT, the value should be a boolean value.
    @return     void
*/
extern void GxStrg_SetConfig(UINT32 cfg, UINT32 value);

/**
    Register the callback of the storage device

    @param[in]  fpStrgCB    The callback function pointer of the storage device
    @return     void.
*/
extern void GxStrg_RegCB(GX_CALLBACK_PTR fpStrgCB);

/**
    Open the storage device.

    It will create the file system task for the storage device.

    @param[in]  DevID       Reserved for future use.
    @param[in]  NewDevObj   The storage device object need to be mounted
    @return
        - @b TRUE   Success
        - @b FALSE  Fail
*/
extern BOOL GxStrg_OpenDevice(UINT32 DevID, DX_HANDLE NewDevObj);

/**
    Close the storage device.

    It will close the file system task for the storage device, and call the callback function registered by GxStrg_RegCB() with #STRG_CB_UNMOUNT_FINISH type.

    @param[in]  DevID       Reserved for future use.
    @return
        - @b TRUE   Success
        - @b FALSE  Fail
*/
extern BOOL GxStrg_CloseDevice(UINT32 DevID);


/**
    Get the object of the current storage device.

    @param[in]  DevID       Reserved for future use.
    @return     The pointer which points to the current storage device. If no device was opened, it will return NULL.
*/
extern DX_HANDLE GxStrg_GetDevice(UINT32 DevID);

/**
    Set the parameters of the current storage device.

    @param[in]  DevID       Reserved for future use.
    @param[in]  data        The type of the parameter you want to set. (Options: #CARD_POWER)
    @param[in]  value       The value of the parameter you want to set.
    @return     void.
*/
extern void GxStrg_SetDeviceCtrl(UINT32 DevID, UINT32 data, UINT32 value);

/**
    Get the status of the current storage device.

    @param[in]  DevID       Reserved for future use.
    @param[in]  data        The status of the device you want to get. (Options: #CARD_INSERT / #CARD_READONLY)
    @return     The result of the requested status
        - @b TRUE  (For CARD_INSERT, the card is inserted. For CARD_READONLY, the card is locked)
        - @b FALSE (For CARD_INSERT, the card is removed. For CARD_READONLY, the card is not locked)
*/
extern UINT32 GxStrg_GetDeviceCtrl(UINT32 DevID, UINT32 data);

/**
    Detect SD card is inserted or not.

    It will call the callback function registered by GxStrg_RegCB() with the corresponding STRG_CB type. (#STRG_CB_UNKNOWN / #STRG_CB_INSERTED / #STRG_CB_REMOVED)

    @param[in]  DevID       Reserved for future use.
    @return     void.
*/
extern void GxStrg_Det (UINT32 DevID);

//@}
#endif //_GXFILE_H
