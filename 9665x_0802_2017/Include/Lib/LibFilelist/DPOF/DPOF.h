/**
    DPOF module.

    This file contains the utilitis of Digital Print Order Format.

    @file       DPOF.h
    @ingroup    mIDPOF

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _DPOF_H
#define _DPOF_H

/**
    @addtogroup mIDPOF
*/
//@{

/**
    @name   DPOF Module Status

    Definition for DPOF Module Opened or Not
*/
//@{
typedef enum _DPOF_STATUS
{
    DPOF_STATUS_UNINITIALED   =      0,   ///< DPOF module not opened yet
    DPOF_STATUS_INITIALED     =      1,   ///< DPOF module opened
    ENUM_DUMMY4WORD(DPOF_STATUS)
}DPOF_STATUS;
//@}


/**
    @name   DPOF File Modification

    Definition for update DPOF File or not
*/
//@{
typedef enum _DPOF_CLOSE_MODE
{
    DPOF_NO_UPDATE         =         2,   ///< Not Update DPOF file
    DPOF_UPDATE            =         1,   ///< Update DPOF file
    ENUM_DUMMY4WORD(DPOF_CLOSE_MODE)
}DPOF_CLOSE_MODE;
//@}

/**
    @name   Returned-Value of DPOF APIs

    Definition for DPOF APIs' returned value
*/
//@{
typedef enum _DPOF_ER
{
    DPOF_OK                 =        0x0000,  ///< Success
    DPOF_FAIL               =        0x0001,  ///< Fail
    DPOF_NO_FILE            =        0x0011,  ///< No such file exists
    DPOF_NOT_JPEG           =        0x0101,  ///< Not a jpeg file
    DPOF_DSC_UNKNOW         =        0xFF  ,  ///< DPOF module not opened
    ENUM_DUMMY4WORD(DPOF_ER)
}DPOF_ER;
//@}

/**
     @name DPOF buffer size.
*/
//@{
#define DPOF_BUFFER_SIZE                0x00040000      ///< Needed for DPOF working buffer.
//@}

/**
    @name   DPOF Header Element

    Enumerate DPOF header elements.
*/
//@{
typedef enum _DPOF_HDR_ITEM
{
    DPOF_HDR_REV,   ///<:Revision of DPOF header
    DPOF_HDR_CRT    ///<:Creator of DPOF header
}DPOF_HDR_ITEM;
//@}

/**
    @name   DPOF Access Type

    Enumerate DPOF Access Type.
*/
//@{
typedef enum _DPOF_ACCESS_TYPE
{
    DPOF_GET,   ///<:Get the data
    DPOF_SET    ///<:Set the data
}DPOF_ACCESS_TYPE;
//@}

/**
    @name   DPOF Date Format

    Enumerate DPOF Date Format.
*/
//@{
typedef enum _DPOF_DSC_DATE_FORMAT
{
    DPOF_DSC_DATE_YMD,  ///< Year/Month/Date
    DPOF_DSC_DATE_DMY,  ///< Date/Month/Year
    DPOF_DSC_DATE_MDY   ///< Month/Date/Year
}DPOF_DSC_DATE_FORMAT;
//@}

/**
    @name   DPOF Date-Time Format

    Enumerate DPOF Date-Time Format.
*/
//@{
typedef enum _DPOF_DSC_FORMAT
{
    DPOF_DSC_DATE_TIME, ///< Both Date and Hour
    DPOF_DSC_DATE,      ///< Date only
    DPOF_DSC_TIME       ///< Time only
}DPOF_DSC_FORMAT;
//@}


/**
    Structure for DPOF Setting-Object

*/
//@{
typedef struct _DPOF_INFO_OBJ
{
    UINT16  uiQty;  ///< Qualtity of an image
    UINT8   uiDSC;  ///< Date-Print or not
    UINT8   rev;    ///< reserved
    UINT16  dirId;  ///< directory ID
    UINT16  fileId; ///< file ID
}DPOF_INFO_OBJ, *PDPOF_INFO_OBJ;
//@}


/**
     Install DPOF semaphore id

     This is a default setting API. Users can create their own task id, semphore id,
     flag id to adjust own stack size and task priority as the following:

     @code
     {
        UINT32 SEMID_DPOF = 0;

        void Install_UserDPOFID(void)
        {
            OS_CONFIG_SEMPHORE(SEMID_DPOF, 0, 1, 1);
        }
     }
     @endcode
*/
extern void DPOF_InstallID(void) _SECTION(".kercfg_text");

//////////////////////////////////////////////////////////////////////////
/**
    DPOF open.

    Initiallize DPOF module.

    @note The needed buffer size is #DPOF_BUFFER_SIZE.
    @param[in] buffAddr Buffer address for DPOF module.
    @return
        - @b DPOF_OK:  Succeed.
        - @b DPOF_FAIL:  Failed.
*//////////////////////////////////////////////////////////////////////////
extern DPOF_ER DPOF_Open(UINT32 buffAddr);

//////////////////////////////////////////////////////////////////////////
/**
    Close DPOF module.

    @param[in] mode Close mechanism: #_DPOF_CLOSE_MODE.
    @return
        - @b DPOF_OK:  Succeed.
        - @b DPOF_FAIL:  Failed.
*//////////////////////////////////////////////////////////////////////////
extern DPOF_ER DPOF_Close(DPOF_CLOSE_MODE mode);

//////////////////////////////////////////////////////////////////////////
/**
    Set print-quantities of a picture

    @param[in] fileID File ID of the picture.
    @param[in] Qty Print quantity of the picture and maximum value is 255.
    @param[in] DirID Directory ID of the picture.

    @return
        - @b DPOF_OK: Change quantity of a single job ok.
        - @b DPOF_FAIL: Change quantity of a single job fail.
        - @b DPOF_NO_FILE: There is no such a file.
        - @b DPOF_NOT_JPEG: This is not a JPEG file.
*//////////////////////////////////////////////////////////////////////////
extern DPOF_ER DPOF_SetQty(UINT16 fileID, UINT16 Qty, UINT16 DirID);

//////////////////////////////////////////////////////////////////////////
/**
    Set the date print for all images

    @param[in] Confirm
             - @b 0: don't print date
             - @b 1: print date

    @return
        - @b DPOF_OK:  Succeed.
*//////////////////////////////////////////////////////////////////////////
extern DPOF_ER DPOF_SetDate(UINT8 Confirm);

//////////////////////////////////////////////////////////////////////////
/**
    Delete whole DPOF setting and DPOF File.

    @return Return void
*//////////////////////////////////////////////////////////////////////////
extern void DPOF_DeleteAll_and_DeleteMISC(void);

//////////////////////////////////////////////////////////////////////////
/**
    Set DPOF for all images.

    @param[in] Confirm TURE = yes!
    @param[in] Qty Print quantity, max = DPOF_QTY_MAX(255).
    @param[in] Date Print date or not.
        - @b 0: Don't print date.
        - @b 1: Print date.
    @return
        - @b DPOF_OK:  Succeed.
        - @b DPOF_FAIL:  Failed.
*//////////////////////////////////////////////////////////////////////////
extern DPOF_ER DPOF_SetAll(UINT8 Confirm, UINT16 Qty, UINT8 Date);

//////////////////////////////////////////////////////////////////////////
/**
    Check if all images are set to DPOF file.

    @return
        - @b 0: No, one or more images are not set to DPOF file.
        - @b 1: Yes, all images are set to DPOF file.
*//////////////////////////////////////////////////////////////////////////
extern UINT8 DPOF_GetAll(void);

//////////////////////////////////////////////////////////////////////////
/**
    Search DPOF quantity of a specific picture.

    @param[in] fileID File ID of the picture.
    @param[in] DirID Directory id of the picture.

    @return The quantity of a specific picture.
*//////////////////////////////////////////////////////////////////////////
extern UINT16 DPOF_GetQty(UINT16 fileID, UINT16 DirID);

//////////////////////////////////////////////////////////////////////////
/**
    Set DPOF date string.

    Print dpof date of a picture or not.\n
    If users want to print their own date, they can put their date-string\n
    pointer in the parameter pDscStr. Otherwise pDscStr shall be null.

    @param[in] fileID File ID of the picture.
    @param[in] Confirm 0: no print date; non-zero: print date.
    @param[in] DirID Directory ID of the picture.
    @param[in] pDscStr Pointer to the date-string.\n
                If the definition "DPOF_DATE_FROM_UI" is set to 1, \n
                the date-string will be retrieved from *pDscStr and \n
                it means the date-string can be decided by UI.

    @return
        - @b DPOF_OK: Change date-print of a single job ok.
        - @b DPOF_FAIL: Not find a dpof job setting.
        - @b DPOF_NO_FILE: There is no such a file
*//////////////////////////////////////////////////////////////////////////
extern DPOF_ER DPOF_SetOneDateStr(UINT16 fileID, UINT8 Confirm, UINT16 DirID, const char *pDscStr);

//////////////////////////////////////////////////////////////////////////
/**
    Get total images number with DPOF setting.

    @return The number of images with DPOF.
*//////////////////////////////////////////////////////////////////////////
extern UINT16 DPOF_GetTotalJobs(void);

//////////////////////////////////////////////////////////////////////////
/**
    Search DPOF setting information of a specific picture.

    @param[in] fileID File ID of the picture.
    @param[in] DirID Directory ID of the picture.
    @param[out] pDPOFInfo Pointer to the specific picture's dpof-info.

    @return
        - @b DPOF_OK: Got the specified picture's dpof-info.
        - @b DPOF_FAIL: Not found the specified picture's dpof-setting.
*//////////////////////////////////////////////////////////////////////////
extern DPOF_ER DPOF_GetDPOFInfo(UINT16 fileID, UINT16 DirID, DPOF_INFO_OBJ *pDPOFInfo);

//////////////////////////////////////////////////////////////////////////
/**
    Get or Set DPOF header.

    @param[in] item DPOF Header Element: #_DPOF_HDR_ITEM.
    @param[in] accType Access type: #_DPOF_ACCESS_TYPE.
    @param[in,out] pRetVal Pointer to the returned data or set data.\n
                    If accType is DPOF_GET, pRetVal will be the returned data.\n
                    If accType is DPOF_SET, pRetVal will be the set data.
    @param[in,out] length Data length.\n
                    If accType is DPOF_GET, length will be length of the returned data.
                    If accType is DPOF_SET, length will be length of the set data.

    @return
        - @b DPOF_OK:  Succeed.
        - @b DPOF_FAIL:  Failed.
*//////////////////////////////////////////////////////////////////////////
extern DPOF_ER DPOF_AccessHeaderItem(DPOF_HDR_ITEM item, DPOF_ACCESS_TYPE accType, char *pRetVal, UINT32 length);

//////////////////////////////////////////////////////////////////////////
/**
    Set date-time format in the DPOF file.

    @param[in] dscFormat Dsc format: #_DPOF_DSC_FORMAT.
    @return Return void
*//////////////////////////////////////////////////////////////////////////
extern void DPOF_SetDSCFormat(DPOF_DSC_FORMAT dscFormat);

//////////////////////////////////////////////////////////////////////////
/**
    Get date-time format in the DPOF file.

    @return Dsc format: #_DPOF_DSC_FORMAT.
*//////////////////////////////////////////////////////////////////////////
extern DPOF_DSC_FORMAT DPOF_GetDSCFormat(void);

//////////////////////////////////////////////////////////////////////////
/**
    Set date format in the DPOF file.

    @param[in] dateFormat Date format: #_DPOF_DSC_DATE_FORMAT.
*//////////////////////////////////////////////////////////////////////////
extern void DPOF_SetDSCDateFormat(DPOF_DSC_DATE_FORMAT dateFormat);

//////////////////////////////////////////////////////////////////////////
/**
    Get date format in the DPOF file.

    @return Date format: #_DPOF_DSC_DATE_FORMAT.
*//////////////////////////////////////////////////////////////////////////
extern DPOF_DSC_DATE_FORMAT DPOF_GetDSCDateFormat(void);

//////////////////////////////////////////////////////////////////////////
/**
    Check current DCF file set to DPOF or not.

    @param[in] fileID File ID of the picture.
    @param[in] DirID Directory ID of the picture.

    @return Return
        - @b DPOF_NO_FILE: Wrong fileID and DirID.
        - @b DPOF_NOT_JPEG: Not a JPG file.
        - @b DPOF_FAIL: Not set to DPOF file.
        - @b DPOF_OK: Set to DPOF file.
*//////////////////////////////////////////////////////////////////////////
extern DPOF_ER  DPOF_IsCurrExist(UINT16 fileID, UINT16 DirID);

//////////////////////////////////////////////////////////////////////////
/**
    Check if there is any DPOF setting with date-string.

    @return
        - @b 0: No.
        - @b non-zero: Yes.
*//////////////////////////////////////////////////////////////////////////
extern UINT16 DPOF_GetTotalDatePrt(void);

//////////////////////////////////////////////////////////////////////////
/**
    Count the total print number in the DPOF file.

    @return The total print number.
*//////////////////////////////////////////////////////////////////////////
extern UINT32 DPOF_GetTotalPrints(void);

//////////////////////////////////////////////////////////////////////////
/**
    DPOF cropping setting

    @param[in] fileId File ID of the picture.
    @param[in] dirId Directory ID of the picture.
    @param[in] cropX X ordinate.
    @param[in] cropY Y ordinate.
    @param[in] cropW Width.
    @param[in] cropH Height.

    @return
        - @b DPOF_OK: Success.
        - @b DPOF_NOT_JPEG: This is not a jpg file.
        - @b DPOF_FAIL: Not find a dpof job setting.
        - @b DPOF_NO_FILE: There is no such a file.
*//////////////////////////////////////////////////////////////////////////
extern DPOF_ER DPOF_SetCrop(UINT16 fileId, UINT16 dirId, UINT16 cropX, UINT16 cropY, UINT16 cropW, UINT16 cropH);

//////////////////////////////////////////////////////////////////////////
/**
    DPOF module is opened or not.

    @return DPOF Module Status: #_DPOF_STATUS.
*//////////////////////////////////////////////////////////////////////////
extern DPOF_STATUS DPOF_IsOpened(void);

//////////////////////////////////////////////////////////////////////////
/**
    Get total DPOF information.

    @param[out] pDpofInfo Pointer to DpofInfo object: #_DPOF_INFO_OBJ.
    @param[in] num Object number.
    @return Total prints.
*//////////////////////////////////////////////////////////////////////////
extern UINT32 DPOF_GetTotalDPOFInfo(DPOF_INFO_OBJ *pDpofInfo, UINT16 num);
//@}
#endif //_DPOF_H
