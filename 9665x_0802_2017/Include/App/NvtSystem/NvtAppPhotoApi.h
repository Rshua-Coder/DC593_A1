/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       NvtAppPhotoApi.h
    @ingroup    mINvtSystem

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef NVT_PHOTO_API_H
#define NVT_PHOTO_API_H

#include "GxLib.h"
#include "IPL_Cmd.h"
#include "ImgCapCBMsg.h"
#include "ImgCaptureAPI.h"
#include "Fd_lib.h"
#include "Sd_lib.h"
#include "AppView.h"
#include "NvtAppMovieApi.h"

/**
    @addtogroup mINvtSystem
*/
//@{


/**
     @name Configuration ID for DscPhoto_Config().
*/
//@{
#define PHOTO_CFG_POOL          1           ///<  Pool address & size
#define PHOTO_CFG_CACHESIZE     2           ///<  cache pool address & size
#define PHOTO_CFG_RATIO         3           ///<  photo image ratio
#define PHOTO_CFG_FUNCINFO      7           ///<  register function for running on photo task, ex: such as FD ,SD
#define PHOTO_CFG_USE_FILEDB    8           ///<  if using fileDB filelist
#define PHOTO_CFG_FILEDB_MEM    9           ///<  fileDB work buffer
#define PHOTO_CFG_IMGPATH       10          ///<  config different image size
#define PHOTO_CFG_CUSTOM_NAMING 11          ///<  use custom naming rule or not
//@}

typedef enum _PHOTO_IMGPATH_CFG
{
    PHOTO_NORMAL = 0,           //normal photo mode
    PHOTO_DUAL_SENSOR,          //dual sensor, 2 image source from IPL_ID_1, IPL_ID_2
    PHOTO_DUAL_SENSOR_2NDOFF,   //dual sensor, 2nd one is disconnected
    PHOTO_DUAL_SENSOR_2NDINACT  //dual sensor, 2nd one is connected but inactive
} PHOTO_IMGPAEH_CFG;

extern VIEW_SRC gPhoto_src;

/**
    DateImprint string generate callback function description.

*/
typedef void (*DATE_STR_CB_FP)(void);

/**
    DateImprint picture generate callback function description.

*/
typedef void (*DATE_PIC_CB_FP)(IMG_CAP_DATASTAMP_INFO*  Info);

/**
    Exif data writing callback function description.

*/
typedef void (*WRITE_EXIF_CB_FP)(IMG_CAP_EXIF_DATA* Data);

/**
    Shutter sound callback function description.

*/
typedef void (*SHUTTER_SOUND_CB_FP)(void);

/**
    JPG write debug info callback function description.

*/
typedef void (*WRITE_DEBUG_CB_FP)(IMG_CAP_DEBUG_INFO* DebugInfo);

/**
    Multi-frame anti-shake callback function description.

*/
typedef void (*STAB_CB_FP)(IMG_CAP_MFANTISHAKE_INFO* info);

/**
    Multi-frame HDR callback function description.
*/
typedef void (*HDR_CB_FP)(IMG_CAP_MFHDR_INFO* info);


/**
    Get one raw image end callback function description.
*/
typedef void (*ONE_RAWEND_CB_FP)(void);

/**
    IPL change display size callback function description.

*/
typedef void (*CHGDISPSIZEINFOR_CB_FP)(void);



/**
    Register callback function for Generate DateImprint string.

    @param[in] fp callback function to be registered

    @return void
*/
extern void DscPhoto_RegDateStrCB(DATE_STR_CB_FP fp);

/**
    Register callback function for Generate DateImprint picture.

    @param[in] fp callback function to be registered

    @return void
*/
extern void DscPhoto_RegDatePicCB(DATE_PIC_CB_FP fp);

/**
    Register callback function for Exif data writing.

    @param[in] fp callback function to be registered

    @return void
*/
extern void DscPhoto_RegWriteExifCB(WRITE_EXIF_CB_FP fp);

/**
    Register callback function for playing shutter sound.

    @param[in] fp callback function to be registered

    @return void
*/
extern void DscPhoto_RegShutterSoundCB(SHUTTER_SOUND_CB_FP fp);

/**
    Register callback function for writing debug info in JPEG.

    @param[in] fp callback function to be registered

    @return void
*/
extern void DscPhoto_RegWriteDebugCB(WRITE_DEBUG_CB_FP fp);

/**
    Register callback function for handling Multi-frame anti-shake.

    @param[in] fp callback function to be registered

    @return void
*/
extern void DscPhoto_RegStabCB(STAB_CB_FP fp);

/**
    Register callback function for handling Multi-frame HDR.

    @param[in] fp callback function to be registered

    @return void
*/
extern void DscPhoto_RegHDRCB(HDR_CB_FP fp);

/**
    Register callback function for notifying get image raw end.

    @param[in] fp callback function to be registered

    @return void
*/
extern void DscPhoto_RegOneRawEndCB(ONE_RAWEND_CB_FP fp);

/**
    Register callback function for changing display size.

    @param[in] fp callback function to be registered

    @return void
*/
extern void DscPhoto_RegChgDispSizeInforCB(CHGDISPSIZEINFOR_CB_FP fp);

/**
    Config settings for Photo mode.

    Config settings for Photo mode, such as PHOTO_CFG_POOL or others.

    @param[in] config_id PHOTO_CFG_POOL or others.
    @param[in] value configuration value.

    @return void

    Example:
    @code
    {
        PHOTO_FUNC_INFO PhotoFuncInfo_dummy =
        {
            {
            PHOTO_ID_1,             ///< function hook process Id.
            PHOTO_ISR_SIE_VD,       ///< isr tigger event
            NULL,                   ///< init fp, only execute at first process
            NULL,                   ///< process fp
            NULL,                   ///< process end fp
            NULL,                   ///< get working buffer fp
            NULL,                   ///< get working cache buffer fp
            },
            NULL  ///< point next Function Obj, last Function must be set NULL
        };

        PHOTO_FUNC_INFO PhotoFuncInfo_fd =
        {
            {
            PHOTO_ID_2,             ///< function hook process Id.
            PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
            Photo_FDInit,           ///< init fp, only execute at first process
            Photo_FDProcess,        ///< process fp
            NULL,                   ///< process end fp
            FD_CalcBuffSize,        ///< get working buffer fp
            FD_CalcCacheBuffSize,   ///< get working cache buffer fp
            },
            NULL  ///< point next Function Obj, last Function must be set NULL
        };

        // config photo memory pool
        Pool.Addr = OS_GetMempoolAddr(POOL_ID_APP);
        Pool.Size = POOL_SIZE_APP;
        DscPhoto_Config(PHOTO_CFG_POOL, (UINT32)&Pool);

        phead = &PhotoFuncInfo_dummy;
        phead->pNext = NULL; //mark end
        {
            PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_fd;
            PHOTO_FUNC_INFO* pcurr = 0;
            pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
            pcurr->pNext = pfunc; //append this func
            pfunc->pNext = NULL; //mark end
        }
        // config FD function for running on photo task
        DscPhoto_Config(PHOTO_CFG_FUNCINFO, (UINT32)phead);
        // config cache memory
        DscPhoto_Config(PHOTO_CFG_CACHESIZE, 2000*1024);
        // config not to use FileDB file list
        DscPhoto_Config(PHOTO_CFG_USE_FILEDB, 0);
        // config photo live view ratio 4:3
        DscPhoto_Config(PHOTO_CFG_RATIO,IPL_SENCROPRATIO_4_3);
    }
    @endcode
*/
extern void DscPhoto_Config(UINT32 config_id, UINT32 value);

/**
    Open photo mode.

    Open photo mode.

    @param[in] void

    @return void
*/
extern void DscPhoto_Open(void);

/**
    Close photo mode.

    Close photo mode.

    @param[in] void

    @return void
*/
extern void DscPhoto_Close(void);

/**
    Face detection callback function.

    Face detection callback function for post event NVTEVT_ALGMSG_FDEND to UIframework task when face detection end.

    @param[in] uiMsgID callback message ID
    @param[in] Data callback message data

    @return void
*/
extern void Photo_FdCB(FD_CBMSG uiMsgID, void *Data);

/**
    Smile detection callback function.

    Smile detection callback function for post event NVTEVT_ALGMSG_SDEND to UIframework task when smile detection end.

    @param[in] uiMsgID callback message ID
    @param[in] Data callback message data

    @return void
*/
extern void Photo_SdCB(SD_CBMSG uiMsgID, void *Data);


/**
    Register FileDB folder name.

    Register FileDB folder name.

    @param[in] pStr folder name, default: CARDV

    @return void
*/
extern void DscPhoto_SetFDBFolder(CHAR *pStr);

/**
    Register FileDB photo folder name.

    Register FileDB photo folder name.

    @param[in] pStr photo folder name, default: Photo

    @return void
*/
extern void DscPhoto_SetFDBPhotoFolder(CHAR *pStr);

/**
    Get last write file path of photo mode.

    Get last write file path of photo mode.

    @return Write file full path
*/
extern CHAR* DscPhoto_GetLastWriteFilePath(void);

/**
    Register root folder name of Custom Naming Rule.

    Register root folder name of Custom Naming Rule.

    @param[in] pStr folder name, default: "A:\\CarDV\\"

    @return void
*/
extern void DscPhoto_SetRootFolder(char *pStr);

/**
    Register Photo folder name of Custom Naming Rule.

    Register Photo folder name of Custom Naming Rule.

    @param[in] uiPathId movie path ID
    @param[in] pStr folder name, default: "PHOTO"

    @return void
*/
extern void DscPhoto_SetPhotoFolder(UINT32 uiPathId, char *pStr);

/**
    Register callback function for custom photo file naming.

    Register callback function for custom photo file naming.

    @param[in] pPhotoFileNamingCB callback function to be registered

    @return void
*/
extern void DscPhoto_RegPhotoFileNamingCB(CustomFileNamingCB *pPhotoFileNamingCB);

/**
    Set custom file naming method for Photo mode.

    Set custom file naming method for Photo mode.

    @param[in] uiMethod method: FILE_NAMING_MANUAL, FILE_NAMING_SEMIAUTO, FILE_NAMING_AUTO

    @return void
*/
extern void DscPhoto_SetFileNamingMethod(UINT32 uiMethod);

/**
    Set file naming type of semi-auto custom file naming method for Photo mode.

    Set file naming type of semi-auto custom file naming method for Photo mode.

    @param[in] uiType file naming type: FILE_NAMING_SEMIAUTO_TYPE1, FILE_NAMING_SEMIAUTO_TYPE2, FILE_NAMING_SEMIAUTO_TYPE3

    @return void
*/
extern void DscPhoto_SetFileNamingType(UINT32 uiType);

/**
    Set file naming YMD type of semi-auto custom file naming method for Photo mode.

    Set file naming YMD type of semi-auto custom file naming method for Photo mode.

    @param[in] uiYmdType YMD type: FILE_NAMING_DATEFMT_YMD, FILE_NAMING_DATEFMT_MDY, FILE_NAMING_DATEFMT_DMY

    @return void
*/
extern void DscPhoto_SetFileNamingYMD(UINT32 uiYmdType);

/**
    Set file name end character of semi-auto custom file naming method for Photo mode.

    Set file name end character of semi-auto custom file naming method for Photo mode.

    @param[in] uiPathId recording path ID
    @param[in] bEnable  enable/disable end character
    @param[in] pStr     pointer to end character string

    @return void
*/
extern void DscPhoto_SetFileNameEndChar(UINT32 uiPathId, BOOL bEnable, char *pStr);

/**
    Enable/disable file name serial number of semi-auto custom file naming method for Photo mode.

    Enable/disable file name serial number of semi-auto custom file naming method for Photo mode.

    @param[in] uiPathId recording path ID
    @param[in] bEnable  enable/disable file name serial number

    @return void
*/
extern void DscPhoto_SetFileNameSerialNum(UINT32 uiPathId, BOOL bEnable);

/**
    Set FileDB sort type by SN number

    Set FileDB sort type by SN number.

    @param[in] pDelimStr    The delimiter string, e.g. underline "_", "AA"
    @param[in] nDelimCount  The delimiter count to find the serial number
    @param[in] nNumOfSn     The character number of the serial numberThe character number of the serial number

    @param[in] void
*/
extern void DscPhoto_SetSortBySN(CHAR *pDelimStr, UINT32 nDelimCount, UINT32 nNumOfSn);

//@}
#endif//NVT_PHOTO_API_H

