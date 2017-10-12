/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIDef.h
    @ingroup    mIPRJAPUIFlow

    @brief      UIDef Functions
                This file is the user interface ( for LIB callback function).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef NVT_MOVIE_API_H
#define NVT_MOVIE_API_H

#include "GxLib.h"
#include "MediaWriteLib.h"
#include "AudFilterAPI.h"
#include "NameRule_Custom.h"


typedef void (MediaRecGetReadyCb2)(UINT32 mode, UINT32 param);
typedef UINT32 (MediaRecRTSPSetBS)(UINT32 p1, UINT32 p2, UINT32 p3);
typedef void (CustomFileNamingCB)(UINT32 uiPathId, char *pFileName);

typedef struct
{
    MediaRecGetReadyCb2    *GetReadyCb2;      ///< callback to get ready buffer
} NVT_MEDIAOBJ;

/**
    @name Media record event ID

    Callback ID of media recorder.
*/
//@{
#define MOVIE_CFG_POOL              1       //memory address for recording
#define MOVIE_CFG_CACHESIZE         2       //cache size, now 0
#define MOVIE_CFG_PIMSIZE           3       //external buffer for PIM, now 88M
#define MOVIE_CFG_RATIO             4       //crop ratio, IPL_SENCROPRATIO_4_3 or others
#define MOVIE_CFG_RSC               5       //RSC on/off, SEL_RSC_OFF or others
#define MOVIE_CFG_FUNCINFO          9       //PhotoFuncInfo_dummy
#define MOVIE_CFG_USE_FILEDB        10      //use fileDB or not, 0 or 1
#define MOVIE_CFG_FILEDB_MEM        11      //memory address for fileDB
#define MOVIE_CFG_MULTIREC          12      //multi-recording
#define MOVIE_CFG_MULTIREC_YUVCB    13      //multi-recording, getYUV CB
#define MOVIE_CFG_IMGPATH           14      //config different image size,refer to
#define MOVIE_CFG_FASTREC           15      //config fast recording
#define MOVIE_CFG_PHOTOCAP_RO       16      //config read-only for photo capture as movie recording
#define MOVIE_CFG_CUSTOM_NAMING     17      //use custom naming rule or not
#define MOVIE_CFG_IPL_CHGMODE_EN    101     //Enable IPL change mode or not.
//@}

#define FILE_NAMING_MANUAL          NMC_FILEPATH_METHOD_1
#define FILE_NAMING_SEMIAUTO        NMC_FILEPATH_METHOD_2
#define FILE_NAMING_AUTO            NMC_FILEPATH_METHOD_3
#define FILE_NAMING_METHOD_MAX      FILE_NAMING_AUTO

#define FILE_NAMING_SEMIAUTO_TYPE1  NAMERULECUS_TIMETYPE_1
#define FILE_NAMING_SEMIAUTO_TYPE2  NAMERULECUS_TIMETYPE_2
#define FILE_NAMING_SEMIAUTO_TYPE3  NAMERULECUS_TIMETYPE_3
#define FILE_NAMING_SEMIAUTO_MAX    FILE_NAMING_SEMIAUTO_TYPE3

// file naming date format
#define FILE_NAMING_DATEFMT_YMD     NAMERULECUS_YMDTYPE_YMD
#define FILE_NAMING_DATEFMT_MDY     NAMERULECUS_YMDTYPE_MDY
#define FILE_NAMING_DATEFMT_DMY     NAMERULECUS_YMDTYPE_DMY
#define FILE_NAMING_DATEFMT_MAX     FILE_NAMING_DATEFMT_DMY

typedef enum _MOVIE_IMGPATH_CFG
{
    MOVIE_NORMAL = 0,           //normal movie preview,movie record
    MOVIE_DUAL_SENSOR,          //dual sensor,2 image source from IPL_ID_1,IPL_ID_2
    MOVIE_WIFI_PREVIEW,         //wifi preview
    MOVIE_WIFI_RECORD,          //wifi record
    MOVIE_DUAL_DISPLAY, //dual display
    MOVIE_DUAL_SENSOR_2NDOFF,   //dual sensor, 2nd one is disconnected
    MOVIE_DUAL_SENSOR_2NDINACT, //dual sensor, 2nd one is connected but inactive
    MOVIE_UVC_RECORD,           //UVC and movie recording simultaneously
    MOVIE_WIFI_DUAL_SENSOR_1STINACT, //dual sensor, 1st one is connected but inactive
    MOVIE_WIFI_PREVIEW_SEN2_DISPONLY,  //wifi preview, sensor 2 display only
    MOVIE_WIFI_RECORD_SEN2_DISPONLY,   //wifi record, sensor 2 display only
    MOVIE_DUAL_REC_TEST = 0xFF  //dual movie recording test by using same image source
}MOVIE_IMGPAEH_CFG;

extern VIEW_SRC gMovie_src;

/**
    Config settings for movie recording.

    Config settings for movie recording, such as MOVIE_CFG_POOL or others.

    @param[in] config_id MOVIE_CFG_POOL or others.
    @param[in] value configuration value.

    @return void
*/
extern void DscMovie_Config(UINT32 config_id, UINT32 value);

/**
    Get config settings for movie recording.

    Get config settings for movie recording, such as MOVIE_CFG_IMGPATH or others.

    @param[in] config_id MOVIE_CFG_IMGPATH or others.

    @return configuration value
*/
extern UINT32 DscMovie_GetConfig(UINT32 config_id);

/**
    Open movie recorder.

    Open movie recorder.

    @param[in] void

    @return void
*/
extern void DscMovie_Open(void);

/**
    Close movie recorder.

    Close movie recorder.

    @param[in] void

    @return void
*/
extern void DscMovie_Close(void);

/**
    Start to record a movie file.

    Start to record a movie file.

    @param[in] void

    @return void
*/
extern void DscMovie_RecStart(void);

/**
    Stop recording and save a movie file.

    Stop recording and save a movie file.

    @param[in] void

    @return void
*/
extern void DscMovie_RecStop(void);

/**
    Open IPL.

    Open IPL.

    @param[in] void

    @return void
*/
extern void DscMovie_OpenIPL(void);

/**
    Close IPL.

    Close IPL.

    @param[in] void

    @return void
*/
extern void DscMovie_CloseIPL(void);

/**
    Set user maker data and model data for MOV files.

    Set user maker data and model data for MOV files.

    @param[in] p8Buf temp buffer to put user data
    @param[in] pcMakername address of MakerName
    @param[in] uMakersize size of MakerName
    @param[in] pcModelname address of ModelName
    @param[in] uModelsize size of ModelName

    @return void
*/
extern UINT32 DscMovie_MakeUserMakerModelData(UINT8 *p8Buf,
                                              unsigned char *pcMakername, UINT32 uMakersize,
                                              unsigned char *pcModelname, UINT32 uModelsize);
/**
    Register draw function for movie recording.

    Register callback function for data drawing (such as date stamp) as movie recording.

    @param[in] pDrawCB callback function to be registered

    @return void
*/
extern void DscMovie_RegDrawCB(MediaRecDrawStr *pDrawCB);

/**
    Register audio filter function for movie recording.

    Register callback function for audio filtering as movie recording.

    @param[in] pAudFilterCB callback function to be registered

    @return void
*/
extern void DscMovie_RegAudFilterCB(AUDFILTER_FILTER_CB pAudFilterCB);

/**
    Register callback function for processing events immediately.

    Register callback function for the events which should be processed immediately
    as movie recording (cannot use post event).

    @param[in] pImmProcCB callback function to be registered

    @return void
*/
extern void DscMovie_RegImmProcCB(MediaRecImmediatelyProc *pImmProcCB);

/**
    Register callback function for MediaPlay_Open().

    Register callback function for the events from Media Play Object

    @param[in] event_id  define Media Play Event ID

    @return void
*/
extern void Play_MovieCB(UINT32 event_id);
extern void DscMovie_SetGetReadyCb2(void *pobj);

/**
    Register callback function for RTSP to set video BS.

    Register callback function for RTSP to set video BS.

    @param[in] pImmProcCB callback function to be registered

    @return void
*/
extern void DscMovie_RegRTSPSetBSCB(MediaRecRTSPSetBS *pRTSPCB);

/**
    Register callback function for custom movie file naming.

    Register callback function for custom movie file naming.

    @param[in] pMovieFileNamingCB callback function to be registered

    @return void
*/
extern void DscMovie_RegMovieFileNamingCB(CustomFileNamingCB *pMovieFileNamingCB);

/**
    Register callback function for EMR movie file naming.

    Register callback function for EMR movie file naming.

    @param[in] pMovieFileNamingCB callback function to be registered

    @return void
*/
extern void DscMovie_RegEMRMovFileNamingCB(CustomFileNamingCB *pMovieFileNamingCB);

/**
    Register callback function for custom photo file naming.

    Register callback function for custom photo file naming.

    @param[in] pPhotoFileNamingCB callback function to be registered

    @return void
*/
extern void DscMovie_RegPhotoFileNamingCB(CustomFileNamingCB *pPhotoFileNamingCB);

/**
    Register FileDB folder name.

    Register FileDB folder name.

    @param[in] pStr folder name, default: CARDV

    @return void
*/
extern void DscMovie_SetFDBFolder(char *pStr);

/**
    Register FileDB readonly folder name.

    Register FileDB readonly folder name.

    @param[in] pStr folder name, default: MOVIE\RO

    @return void
*/
extern void DscMovie_SetFDB_ROFolder(char *pStr);

/**
    Register FileDB photo folder name. (for rawEnc)

    Register FileDB photo folder name. (for rawEnc)

    @param[in] pStr folder name, default: PHOTO

    @return void
*/
extern void DscMovie_SetFDB_PhotoFolder(char *pStr);

/**
    Register FileDB movie folder name.

    Register FileDB movie folder name.

    @param[in] pStr folder name, default: MOVIE

    @return void
*/
extern void DscMovie_SetFDB_MovieFolder(char *pStr);

/**
    Register root folder name of Custom Naming Rule.

    Register root folder name of Custom Naming Rule.

    @param[in] pStr folder name, default: "A:\\CarDV\\"

    @return void
*/
extern void DscMovie_SetRootFolder(char *pStr);

/**
    Register Read-Only folder name of Custom Naming Rule.

    Register Read-Only folder name of Custom Naming Rule.

    @param[in] uiPathId movie path ID
    @param[in] pStr folder name, default: "MOVIE\\RO"

    @return void
*/
extern void DscMovie_SetROFolder(UINT32 uiPathId, char *pStr);

/**
    Register Photo folder name of Custom Naming Rule.

    Register Photo folder name of Custom Naming Rule.

    @param[in] uiPathId movie path ID
    @param[in] pStr folder name, default: "PHOTO"

    @return void
*/
extern void DscMovie_SetPhotoFolder(UINT32 uiPathId, char *pStr);

/**
    Register Moive folder name of Custom Naming Rule.

    Register Movie folder name of Custom Naming Rule.

    @param[in] uiPathId movie path ID
    @param[in] pStr folder name, default: "MOVIE"

    @return void
*/
extern void DscMovie_SetMovieFolder(UINT32 uiPathId, char *pStr);

/**
    Set custom file naming method.

    Set custom file naming method.

    @param[in] uiMethod method: FILE_NAMING_MANUAL, FILE_NAMING_SEMIAUTO, FILE_NAMING_AUTO

    @return void
*/
extern void DscMovie_SetFileNamingMethod(UINT32 uiMethod);

/**
    Set file naming type of semi-auto custom file naming method.

    Set file naming type of semi-auto custom file naming method.

    @param[in] uiType file naming type: FILE_NAMING_SEMIAUTO_TYPE1, FILE_NAMING_SEMIAUTO_TYPE2, FILE_NAMING_SEMIAUTO_TYPE3

    @return void
*/
extern void DscMovie_SetFileNamingType(UINT32 uiType);

/**
    Set file naming YMD type of semi-auto custom file naming method.

    Set file naming YMD type of semi-auto custom file naming method.

    @param[in] uiYmdType YMD type: FILE_NAMING_DATEFMT_YMD, FILE_NAMING_DATEFMT_MDY, FILE_NAMING_DATEFMT_DMY

    @return void
*/
extern void DscMovie_SetFileNamingYMD(UINT32 uiYmdType);

/**
    Set file name end character of semi-auto custom file naming method.

    Set file name end character of semi-auto custom file naming method.

    @param[in] uiPathId recording path ID
    @param[in] bEnable  enable/disable end character
    @param[in] pStr     pointer to end character string

    @return void
*/
extern void DscMovie_SetFileNameEndChar(UINT32 uiPathId, BOOL bEnable, char *pStr);

/**
    Enable/disable file name serial number of semi-auto custom file naming method.

    Enable/disable file name serial number of semi-auto custom file naming method.

    @param[in] uiPathId recording path ID
    @param[in] bEnable  enable/disable file name serial number

    @return void
*/
extern void DscMovie_SetFileNameSerialNum(UINT32 uiPathId, BOOL bEnable);

/**
    Set not FDB init at DscMovie_Open.

    Set not FDB init at DscMovie_Open.

    @param[in] value  1: not FDB init

    @return void
*/
extern void Movie_FDB_SetNOTInit(UINT32 value);

/**
    Init FDB namerule for movie-recording.

    Init FDB namerule for movie-recording.

    @param[in] void

    @return
        - @b E_OK: success
*/
extern ER Movie_FDB_FastInit(void);

/**
    Check if FileDB is error in movie mode.

    Check if FileDB is error in movie mode.

    @param[in] void

    @return
        - @b E_OK: no error
        - @b E_SYS: FileDB error
*/
extern ER Movie_CheckFDB(void);

extern void RawEnc_GetPath(CHAR *pPath);

extern void DscMovie_Set_MEDIAREC_VER(UINT32 MediaVer);

extern UINT32 DscMovie_Get_MEDIAREC_VER(void);


/**
    Set FileDB sort type by SN number

    Set FileDB sort type by SN number.

    @param[in] pDelimStr    The delimiter string, e.g. underline "_", "AA"
    @param[in] nDelimCount  The delimiter count to find the serial number
    @param[in] nNumOfSn     The character number of the serial numberThe character number of the serial number

    @param[in] void
*/
extern void DscMovie_SetSortBySN(CHAR *pDelimStr, UINT32 nDelimCount, UINT32 nNumOfSn);


#endif//NVT_MOVIE_API_H

