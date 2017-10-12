///////////////////////////////////////////////////////////////////
/**
    File system module.

    The file system supports a lot of file operations of one stoarge. Including file open/read/write/close/copy/delete ...\n
    It also supports the disk format or disk information access.\n
    File system task is a independent task, through the file system, it will call low level storage driver to acceess storage.\n
    If there are errors occured and function call will never return(This situation may happen while card error).\n
    User can close file system task and open again to re-initialize the file system.

    @file       FileSysTsk.h
    @ingroup    mILibFileAccess
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

*/

#ifndef _FILESYS_TSK_H
#define _FILESYS_TSK_H
#include "fs_file_op.h"
#include "DxType.h"
/**
    @addtogroup mILibFileAccess
*/
//@{
#if 0
/**
    File system command
*/
typedef enum _FST_CMD_ID
{
    FST_CMD_CLOSE_FILE,                        ///< close file command
    FST_CMD_DELETE_FILE,                       ///< delete file command
    FST_CMD_FORMAT,                            ///< format command
    ENUM_DUMMY4WORD(FST_CMD_ID)
} FST_CMD_ID;

/**
    File system State ID
*/
typedef enum _FST_STATE_ID
{
    FST_STATE_PAUSE = 0,                   ///< Pause the file system, is usually used when pstore want to write data.
    FST_STATE_RESUME,                      ///< Resume the file system.
    FST_STATE_ABORT,                       ///< Abort the file system running command.
    FST_STATE_CANCEL_ABORT,                ///< Cancel the abort command.
    ENUM_DUMMY4WORD(FST_STATE_ID)
} FST_STATE_ID;
#endif
/**
    File system parameter ID
*/
typedef enum _FST_PARM_ID
{
    FST_PARM_SEGMENT_WRITE_MODE_EN = 0,    ///< if enable the segment write mode. (RW)
    FST_PARM_SEGMENT_READ_MODE_EN,         ///< if enable the segment read mode.  (RW)
    FST_PARM_SEGMENT_WRITE_SIZE,           ///< the segment write size.           (RW)
    FST_PARM_STRG_CLK_EN,                  ///< if enable the storage clock, enable means open memory bus, disable means close memory bus.  (WO)
    FST_PARM_TASK_STS,                     ///< check if file system task is busy.         (RO)
    FST_PARM_TASK_INIT_RDY,                ///< check if file system is initialized ready. (RO)
    FST_PARM_PERF_WRITE_MODE,              ///< get or set the performance mode (RW) (value 0:disable, 1:enable, 2:enable and dump log)
    ENUM_DUMMY4WORD(FST_PARM_ID)
} FST_PARM_ID;


/**
    Disk Info ID
*/
typedef enum _FST_INFO_ID
{
    FST_INFO_DISK_RDY = 0,                 ///< if the disk info is initialized ready
    FST_INFO_DISK_SIZE,                    ///< get the disk size
    FST_INFO_FREE_SPACE,                   ///< get the disk free space
    FST_INFO_VOLUME_ID,                    ///< the volume ID of this disk, normally is "A:\\"
    FST_INFO_CLUSTER_SIZE,                 ///< the cluster size of this disk
    FST_INFO_AVG_FREE_SIZE,                ///< the average size (KB) of each free section
    FST_INFO_DEFECT_PERCENTAGE,            ///< the defect percentage of the performance test (enabled by FileSys_SetParamEx with FST_PARM_PERF_WRITE_MODE)
    FST_INFO_PARTITION_TYPE,               ///< the partition type of the disk
    FST_INFO_IS_SDIO_ERR,                  ///< is sdio error or not
    ENUM_DUMMY4WORD(FST_INFO_ID)
} FST_INFO_ID;


/**
    File seek command
*/
typedef enum _FST_SEEK_CMD
{
    FST_SEEK_SET = 0,                      ///< seek from the start of file
    FST_SEEK_CUR,                          ///< seek from current position of file
    FST_SEEK_END,                          ///< seek from the end of file
    FST_SEEK_MAX_ID,                       ///< the total seek command number
    ENUM_DUMMY4WORD(FST_SEEK_CMD)
} FST_SEEK_CMD;


/**
    File DateTime
*/
typedef enum _FST_FILE_DATETIME_ID
{
    FST_FILE_DATETIME_YEAR = 0,            ///< the year of date, ex: 2012
    FST_FILE_DATETIME_MONTH,               ///< the month of date, value is from 1 ~12
    FST_FILE_DATETIME_DAY,                 ///< the day of date, value is from 1~31
    FST_FILE_DATETIME_HOUR,                ///< the hour of time, value is from 0~23
    FST_FILE_DATETIME_MINUTE,              ///< the minute of time, value is from 0~59
    FST_FILE_DATETIME_SECOND,              ///< the second of time, value is from 0~59
    FST_FILE_DATETIME_MAX_ID,              ///< the max id of datetime
    ENUM_DUMMY4WORD(FST_FILE_DATETIME_ID)
} FST_FILE_DATETIME_ID;



/**
     @name File system state
*/
//@{
//#define FST_CONDITION_UNINIT        0                   ///<   File system is uninitialized.
//#define FST_CONDITION_IDLE          1                   ///<   File system is in the idle status.
//#define FST_CONDITION_BUSY          2                   ///<   File system is running some command in the busy status.
//@}

/**
     @name File attribute
*/
//@{
#define FST_ATTRIB_READONLY         FS_ATTRIB_READ        ///<   Indicates the file is read only.
#define FST_ATTRIB_HIDDEN           FS_ATTRIB_HIDDEN      ///<   Indicates the file should not be shown in normal directory listings.
#define FST_ATTRIB_SYSTEM           FS_ATTRIB_SYSTEM      ///<   Indicates the file is an operating system file.
#define FST_ATTRIB_VOLUME           FS_ATTRIB_VOLUME      ///<   Indicates the file is the label for the volume.
#define FST_ATTRIB_DIRECTORY        FS_ATTRIB_DIRECTORY   ///<   Indicates it is a directory
#define FST_ATTRIB_ARCHIVE          FS_ATTRIB_ACHIEVE     ///<   Indicates the file has been created or modified since the attribute was clear.
//@}

/**
     @name File system return status
*/
//@{
#define FST_STA_OK                  (0)                   ///<  the status is ok
#define FST_STA_ACCESS_ONE          (1)                   ///<  one file has been processed, used in delete all callback function.
#define FST_STA_ERROR               (-1)                  ///<  some error happened
#define FST_STA_NO_FILE             (-2)                  ///<  can't find any files
#define FST_STA_BUSY                (-3)                  ///<  file system task is busing
#define FST_STA_EXIST_FILE          (-4)                  ///<  the file is already exist
#define FST_STA_UNKNOW_CMD          (-5)                  ///<  unknow command
#define FST_STA_PARAM_ERR           (-6)                  ///<  the input parameters are wrong
#define FST_STA_NOFREE_SPACE        (-7)                  ///<  there are no more free space
#define FST_STA_SIZE_ZERO           (-8)                  ///<  this file size is zero
#define FST_STA_TIME_OUT            (-9)                  ///<  used of wait command
#define FST_STA_DISKINIT_FAIL       (-10)                 ///<  The disk initialized fail.
#define FST_STA_DISK_UNFORMAT       (-11)                 ///<  storage is unformat. if realy want to use please format first
#define FST_STA_DISK_UNKNOWN_FORMAT (-12)                 ///<  unknow format
#define FST_STA_FUNC_NOT_READY      (-13)                 ///<  this function not ready
#define FST_STA_FAT_ERR             (-14)                 ///<  FAT may have some error because of detecting illegal cluster value access

#define FST_STA_FS_FAIL              FSS_FAIL             ///<  status fail
#define FST_STA_FS_BUSY              FSS_BUSY             ///<  status busy
#define FST_STA_FS_UNFORMAT          FSS_UNFORMAT         ///<  storage is unformat. if realy want to use please format first
#define FST_STA_FS_EXCESS_SIZE       FSS_EXCESS_SIZE      ///<  access area excess storage's size
#define FST_STA_FS_CANT_ACCESS       FSS_CANT_ACCESS      ///<  try to access reserved area
#define FST_STA_FS_ERROR_PARAM       FSS_ERROR_PARAM      ///<  parameter error
#define FST_STA_FS_UNKNOW_FORMAT     FSS_UNKNOW_FORMAT    ///<  unknow format
#define FST_STA_FS_DUPLEX_NAME       FSS_DUPLEX_NAME      ///<  find the same name in folder
#define FST_STA_FS_NOMORE_SPACE      FSS_NOMORE_SPACE     ///<  nomore space
#define FST_STA_FS_OPEN_TOO_MANY     FSS_OPEN_TOO_MANY    ///<  it open too many files
#define FST_STA_FS_IS_FOLDER         FSS_IS_FOLDER        ///<  we want to open a folder
#define FST_STA_FS_ILLEGAL_FILENAME  FSS_ILLEGAL_FILENAME ///<  filenaem is invalid, usually due to non 8.3 filename or file path deep
#define FST_STA_FS_NO_MORE_FILES     FSS_NO_MORE_FILES    ///<  use in search file, it means no more file in folder/
#define FST_STA_FILE_NOT_EXIST       FSS_FILE_NOTEXIST    ///<  use in file not exist
#define FST_STA_CARD_ERR             FSS_CARD_ERROR       ///<  card error

//@}

/**
     @name File system time out
*/
//@{
#define FST_TIME_NO_WAIT            (0)                   ///< no wait
#define FST_TIME_INFINITE           (0xFFFFFFFF)          ///< wait infinite
//@}

/**
     @name File system open file flag
*/
//@{
#define FST_OPEN_READ                     0x00000001      ///< open file with read mode
#define FST_OPEN_WRITE                    0x00000002      ///< open file with write mode
#define FST_OPEN_EXISTING                 0x00000004      ///< if exist, open; if not exist, return fail
#define FST_OPEN_ALWAYS                   0x00000008      ///< if exist, open; if not exist, create new
//#define FST_CREATE_NEW                    0x00000010    ///< if exist, return fail; if not exist, create new
#define FST_CREATE_ALWAYS                 0x00000020      ///< if exist, open and truncate it; if not exist, create new
#define FST_SPEEDUP_CONTACCESS            0x00000100      ///< read/write random position access can enable this flag for speed up access speed
//@}


/**
     @name File system r/w file flag
*/
//@{
#define FST_FLAG_NONE                     0x00000000      ///< flag none
#define FST_FLAG_ASYNC_MODE               0x00000001      ///< read/write file with asynchronous mode
//@}

/**
     @name FAT type.
*/
//@{
#define FST_FAT12                        (0x01)           ///< the storage is formatted as FAT12
#define FST_FAT16                        (0x02)           ///< the storage is formatted as FAT16
#define FST_FAT32                        (0x03)           ///< the storage is formatted as FAT32
#define FST_EXFAT                        (0x04)           ///< the storage is formatted as EXFAT
//@}


/**
    File Handle.
*/
typedef UINT32*  FST_FILE;

// --------------------------------------------------------------------
// Callback function
// --------------------------------------------------------------------

/**
    FileSys general callback funcion proto type.
*/
typedef void (*FileSys_CB)(UINT32 MessageID, UINT32 Param1, UINT32 Param2, UINT32 Param3);

/**
    FileSys scan directory callback funcion proto type.

    @param FIND_DATA *findDir  : The file/folder entry information
    @param BOOL *bContinue     : Indicate the file system if stop looking for directory content
    @param char *cLongname     : long filename
    @param UINT32 Param        : reserved parameter for future use.
*/
typedef void (*FileSys_ScanDirCB)(FIND_DATA *findDir, BOOL *bContinue, UINT16 *cLongname, UINT32 Param);
/**
    FileSys delete directory callback funcion proto type.

    @param FIND_DATA *findDir  : The file entry information
    @param BOOL *bDelete       : Indicate want to delete this file or not
    @param UINT32 Param1       : reserved parameter for future use.
    @param UINT32 Param2       : reserved parameter for future use.
*/
typedef void (*FileSys_DelDirCB)(FIND_DATA *findDir, BOOL *bDelete, UINT32 Param1, UINT32 Param2);

/**
    FileSys lock directory callback funcion proto type.

    @param FIND_DATA *findDir  : The file entry information
    @param BOOL *bApply        : Indicate want to apply the lock/unlock operation to this file.
    @param UINT32 Param1       : reserved parameter for future use.
    @param UINT32 Param2       : reserved parameter for future use.
*/
typedef void (*FileSys_LockDirCB)(FIND_DATA *findDir, BOOL *bApply, UINT32 Param1, UINT32 Param2);


/**
     Copy files information.

     Defines the information for copying a file from one storage to another storage.
*/
typedef struct _COPYTO_BYNAME_INFO
{
    DX_HANDLE   pSrcStrg;                     ///< the source storage object
    DX_HANDLE   pDstStrg;                     ///< the destionation storage object
    char        *pBuf;                        ///< the temp buffer for copy files
    UINT32      uiBufSize;                    ///< the temp buffer size
    char*       pSrcPath;                     ///< the source file full path
    char*       pDstPath;                     ///< the destionation file full path
    BOOL        bDelete;                      ///< if delete the source file after copy complete
}COPYTO_BYNAME_INFO, *PCOPYTO_BYNAME_INFO;

/**
     Initialize information.

     This structuer contain file system task initialize information.
*/
typedef struct _FILE_TSK_INIT_PARAM
{
    FS_INIT_PARAM   FSParam;                   ///< the init param for file system
    FileSys_CB      pDiskErrCB;                ///< Disk Error handle callback
} FILE_TSK_INIT_PARAM, *PFILE_TSK_INIT_PARAM;

/**
     File status information.

     This structure contains the file status.
*/
typedef struct _FST_FILE_STATUS
{
    UINT64      uiFileSize;                     ///< size of the file in bytes
    UINT16      uiAccessDate;                   ///< last access date
    UINT16      uiModifiedDate;                 ///< last modified time
    UINT16      uiModifiedTime;                 ///< last modified time
    UINT16      uiCreatedDate;                  ///< file created date
    UINT16      uiCreatedTime;                  ///< file created time
    UINT8       uiCreatedTime10ms;              ///< file created time (unit: 10 ms, value: 0~199)
    UINT8       uiAttrib;                       ///< file attribute
} FST_FILE_STATUS, *PFST_FILE_STATUS;


/**
     Install FileSys task,flag and semaphore id.

*/
extern void FileSys_InstallID(void) _SECTION(".kercfg_text");

// --------------------------------------------------------------------
// storage operation
// --------------------------------------------------------------------

/**
    Open file system task.

    This is the file system task initial function.  It will open file system task and parse the storage to get storage info,\n
    After this function, please call FileSys_WaitFinish() to wait file system task initialization finish. Only one storage is activing
    at the same time.
    @param[in] pTarStrgDXH  Which storage to active.
    @param[in] pInitParam   Initial parameters for the file system

    @return: #FST_STA_OK, or #FST_STA_PARAM_ERR. Even the function return #FST_STA_OK, it doesn't mean that the file system initialization is well.\n
             It just means ths task is opened success.
    Example:
    @code
    {
        void Card_InitCB(UINT32 uiMsgID, UINT32 uiP1, UINT32 uiP2, UINT32 uiP)
        {
            switch(uiMsgID)
            {
                case FST_STA_OK:
                    DBG_MSG("card mount OK\r\n");
                    // Scan DCF object.
                    DCF_ScanObj();
                    break;
                case FST_STA_DISK_UNFORMAT:
                    DBG_MSG("^Rcard mount FAIL: Unformat\r\n");
                    break;
                case FST_STA_DISK_UNKNOWN_FORMAT:
                    DBG_MSG("^Rcard mount FAIL: Unknown Format\r\n");
                    break;
                case FST_STA_CARD_ERR:
                    DBG_MSG("^Rcard mount FAIL: Card Error\r\n");
                    break;
                default:
                    DBG_MSG("^Rcard mount FAIL: ErrID=%d\r\n",status);
            }
        }
        CardInit()
        {
            #define POOL_SIZE_FS_BUFFER     (0x82000)+(0x80020))  // R/W buffer + FAT buffer  for support exFAT case
            #define POOL_SIZE_FS_BUFFER     (0x38400)+(0x80020))  // R/W buffer + FAT buffer  for not support exFAT case

            FILE_TSK_INIT_PARAM     Param;
            DX_HANDLE               pStrgDXH;

            memset(&Param, 0, sizeof(FILE_TSK_INIT_PARAM));
            pStrgDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1);

            uiPoolAddr = OS_GetMempoolAddr(POOL_ID_FS_BUFFER);
            Param.FSParam.WorkBuf = uiPoolAddr;
            Param.FSParam.WorkBufSize = (POOL_SIZE_FS_BUFFER);
            // support exFAT
            Param.FSParam.bSupportExFAT   = TRUE;
            Param.pDiskErrCB = (FileSys_CB)Card_InitCB;

            result = FileSys_Open(pStrgDXH, &Param);
            ......
            //do some thing here
            ...
            // call the function to wait init finish
            FileSys_WaitFinish();
        }
    }
    @endcode
*//////////////////////////////////////////////////////////////////////////
extern INT32    FileSys_Open(DX_HANDLE pTarStrgDXH, FILE_TSK_INIT_PARAM *pInitParam);

/**
    Close file system task.

    This API will close file system task and close all opened files.

    @param[in] TimeOut  How long to wait file system ready. In current version, only #FST_TIME_INFINITE and #FST_TIME_NO_WAIT are available.
    @note If #FST_TIME_NO_WAIT is assigned, it will not wait the file system enter idle state and directly close the file system task.
        So if the file system is executing some command, the command will be terminated.
    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        FileSys_Close(FST_TIME_NO_WAIT);
    }
    @endcode
*/
extern INT32    FileSys_Close(UINT32 TimeOut);

/**
    Get current storage.

    Get currently activated storage object.

    @param[out] pWhichObj   Return which storage is active.

    @return #FST_STA_OK, however, check pWhichObj if it is NULL when file system is not init yet.

    Example:
    @code
    {
        DX_HANDLE   pStrgDXH;

        FileSys_GetStrgObj(&pStrgDXH);
    }
    @endcode
*/
extern INT32    FileSys_GetStrgObj(DX_HANDLE *pStrgDXH);

/////////////////////////////////////////////////////////////////////////
/**
    Set disk label.

    Set disk label.

    @param[in] pLabelName   the label name must include disk name (A:\,B:\) \n
                            max label name is 11,if include disk name max lenght is 14.
    @return
         - @b #FST_STA_OK: The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        FileSys_SetDiskLabel("A:\\LABELTTT");
    }
    @endcode
*/
extern INT32    FileSys_SetDiskLabel(char* pLabelName);

/////////////////////////////////////////////////////////////////////////
/**
    Format specified storage.

    As known File System support FAT type such as FAT12, FAT16, FAT32 and EXFAT.
    This API will automatically choose the proper FAT type to format the storage by storage size.

    @param[in] pStrgDXH Which storage to format.
        if file system task had been inited, you can use NULL to foramt current active storage.
    @param[in] ChgDisk  After format, whether change active storage to formated storage.
    @return
         - @b #FST_STA_OK: The operation is success.
         - @b Other than #FST_STA_OK: has some error.


    Example:
    @code
    {
        DX_HANDLE   pStrgDXH;

        FileSys_GetStrgObj(&pStrgDXH);
        FileSys_FormatDisk(pStrgDXH, FALSE);
    }
    @endcode
*/
/////////////////////////////////////////////////////////////////////////
extern INT32    FileSys_FormatDisk(DX_HANDLE pStrgDXH, BOOL ChgDisk);

/**
    Change active storage.

    This API will change the active storage from one to another. Ex: from SD to NAND.

    @param[in] pStrgDXH The storage that wants to be activated.
    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        DX_HANDLE   pStrgDXH;

        pStrgDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1);
        FileSys_ChangeDisk(pStrgDXH);
    }
    @endcode
*/
extern INT32    FileSys_ChangeDisk(DX_HANDLE pStrgDXH);

/**
    Set some file system setting value.

    @param[in] param_id The parameter id.
    @param[in] value    The setting value.
    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.
*/
extern ER       FileSys_SetParam(FST_PARM_ID param_id, UINT32 value);

/**
    Get some file system setting value.

    @param[in] param_id The parameter id.
    @param[in] parm1    reserved parameter for future use.
    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.
*/
extern UINT32   FileSys_GetParam(FST_PARM_ID param_id, UINT32 parm1);

/**
    Get disk infomation.

    User can use this API to get the disk information of current storage, such as free space, total disk size, cluster size ...

    @note
    -# This API will automatically wait the disk initialized ready and then get the disk info when user want to get the info of
    #FST_INFO_DISK_SIZE or #FST_INFO_FREE_SPACE or #FST_INFO_CLUSTER_SIZE. So user don't need to call FileSys_WaitFinish() before this API.

    @param[in] info_id  The info that want to get.
    @return the result information.

    Example:
    @code
    {
        UINT64 DiskSize, FreeSpace, ClusterSize;

        DiskSize = FileSys_GetDiskInfo(FST_INFO_DISK_SIZE);
        FreeSpace = FileSys_GetDiskInfo(FST_INFO_FREE_SPACE);
        ClusterSize = FileSys_GetDiskInfo(FST_INFO_CLUSTER_SIZE);
    }
    @endcode


*/
extern UINT64   FileSys_GetDiskInfo(FST_INFO_ID info_id);

// --------------------------------------------------------------------
// wait ready
// --------------------------------------------------------------------
/**
    Wait FileSys command finish.

    The API would wait the filesys init finish & command finish.
    If the file system task is not opened, it will return error.

    @return
         - @b #FST_STA_OK : The operation is success.
         - @b #FST_STA_ERROR: FileSys Task is not opened.
*/

extern INT32    FileSys_WaitFinish(void);

// --------------------------------------------------------------------
// dir entry
// --------------------------------------------------------------------
/**
    Make a directory.

    The API would find the directory,if it is not exist,would create one.

    @note
    -# Only support Long File Name for FAT12/FAT16/FAT32.
    @param[in] pPath    full path of directory.
    @return
         - @b #FST_STA_OK : The operation is success.
         - @b other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        FileSys_MakeDir("A:\\DCIM\\300NVTIM\\");
    }
    @endcode

*/
extern INT32    FileSys_MakeDir(char *pPath);

///////////////////////////////////////////////////////////////////////
/**
    Get the files and folders of one specific directory.

    @note
    -# The process will not recursively get the files information of sub folders.
          Switching on bGetlong will degrade the performance of operation.
    -# Only support Long File Name for FAT12/FAT16/FAT32.

    @param[in] pPath    Full path and it must tail with '\'
    @param[in] DirCB    Callback function pointer. File system will pass the information to callback function.  \n
        The argument of callback function : \n
        FIND_DATA *findDir  : The file/folder entry information \n
        BOOL *bContinue     : Indicate the file system if stop looking for directory content    \n
        char *cLongname     : long filename \n
        UINT32 Param        : reserved parameter for future use.    \n
    @param[in] bGetlong Get the longfile name or not

    Example:
    @code
    {
        void Filesys_dirCB(FIND_DATA *findDir, BOOL *bContinue, UINT16 *cLongname, UINT32 Param)
        {
            *bContinue = TRUE;
            DBG_MSG("findDir->filename = %s\r\n",findDir->filename);
        }
        FileSys_ScanDir(path, Filesys_dirCB, FALSE);
    }
    @endcode
*/
extern INT32    FileSys_ScanDir(char *pPath, FileSys_ScanDirCB DirCB, BOOL bGetlong, UINT32 CBArg);

/**
    Delete the files of specific directory.

    @note
    -# The process will not be running recursively, and the process will just callback the file information,
    it will not callback the sub directory information.
    -# Only support Long File Name for FAT12/FAT16/FAT32.


    @param[in] pPath    Full path of directory.
    @param[in] CB       Callback function pointer. File system will pass the information to callback function.\n
        The argument of callback function : \n
        FIND_DATA *findDir  : The file entry information    \n
        BOOL *bDelete       : Indicate want to delete this file or not  \n
        If user set the CB as NULL, means bDelete is always TRUE, so all the files will be deleted. \n

    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        void UserFilesys_DelCB(FIND_DATA *findDir, BOOL *bDelete, UINT32 Param1, UINT32 Param2)
        {
            char *    pFileName;
            UINT8     attrib;

            pFileName = findDir->FATMainName;
            attrib    = findDir->attrib;
            if (!M_IsReadOnly(attrib)
                *bDelete = TRUE;
            else
                *bDelete = FALSE;
        }
        FileSys_DelDirFiles("A:\\DCIM\\100NVTIM", UserFilesys_DelCB);
    }
    @endcode
*/
extern INT32    FileSys_DelDirFiles(char *pPath, FileSys_DelDirCB CB);


/**
    Lock/Unlock the files of specific directory.

    @note
    -# The process will not be running recursively, and the process will just callback the file information,
    it will not callback the sub directory information.
    -# Only support Long File Name for FAT12/FAT16/FAT32.

    @param[in] pPath    Full path of directory.
    @param[in] isLock   Lock or Unlock it. (Lock means set the file attribute as read-only, Unlock means set file as non read-only)
    @param[in] CB       Callback function pointer. File system will pass the information to callback function.  \n
        The argument of callback function : \n
        FIND_DATA *findDir  : The file entry information    \n
        BOOL *bApply        : Indicate want to apply the lock/unlock operation to this file.    \n
        If user set the CB as NULL, means bApply is always TRUE, so all the files will be applyed the lock or unlock operation. \n
    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        void UserFilesys_LockCB(FIND_DATA *findDir, BOOL *bApply, UINT32 Param1, UINT32 Param2)
        {
            *bApply = TRUE;
        }
        FileSys_LockDirFiles("A:\\DCIM\\100NVTIM", TRUE, UserFilesys_LockCB);
    }
    @endcode
*/
extern INT32    FileSys_LockDirFiles(char *pPath, BOOL isLock, FileSys_LockDirCB CB);

// --------------------------------------------------------------------
// file entry
// --------------------------------------------------------------------
/**
    Get file long name.

    This api will get the long filename of specific file or folder.

    @note
    -# Only support Long File Name for FAT12/FAT16/FAT32.

    @param[in] *pPath   the full path of file or folder.
    @param[out] *wFileName  long filename of specified file or folder.  \n
                The wFileName must have at least #KFS_LONGFILENAME_MAX_LENG UINT16 double byte char width.  \n
    @return
         - @b #FST_STA_OK: The operation is success.
         - @b other than #FST_STA_OK: has some error.

*/
extern INT32    FileSys_GetLongName(char *pPath, UINT16 *wFileName);

/**
    Get file attribute.

    This api will get the attribute of specific file or folder.

    @note
    -# Only support Long File Name for FAT12/FAT16/FAT32.
    -# The attributes can be bitwise ORed.

    @param[in] pPath    the full path of file or folder.
    @param[out] pAttrib attribute of specific file or folder.  \n
              The attribute list :  \n
              #FST_ATTRIB_READONLY  \n
              #FST_ATTRIB_HIDDEN    \n
              #FST_ATTRIB_SYSTEM    \n
              #FST_ATTRIB_VOLUME    \n
              #FST_ATTRIB_DIRECTORY \n
              #FST_ATTRIB_ARCHIVE   \n
    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        FileSys_GetAttrib("A:\\attrib.txt",&uAttrib);
    }
    @endcode

*/
extern INT32    FileSys_GetAttrib(char *pPath,UINT8 *pAttrib);

/**
    Set file attribute.

    This api will set the attribute of specific file or folder.

    @note
    -# Only support Long File Name for FAT12/FAT16/FAT32.
    -# The attributes can be bitwise ORed.

    @param[in] pPath    the full path of file or folder.
    @param[in] Attrib   attribute of specific file or folder.    \n
             The attribute list :   \n
             #FST_ATTRIB_READONLY   \n
             #FST_ATTRIB_HIDDEN     \n
             #FST_ATTRIB_SYSTEM     \n
    @param[in] bSet     TRUE for add specified attribute to orginal attribute.
                       FALSE for clear specified attribute.
    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        FileSys_SetAttrib("A:\\attrib.txt",FST_ATTRIB_READONLY,TRUE);
    }
    @endcode

*/
extern INT32    FileSys_SetAttrib(char *pPath,UINT8 Attrib, BOOL bSet);

/**
    Get file Datetime.

    This api will get the create & modify time of specific file or folder.

    @note
    -# Only support Long File Name for FAT12/FAT16/FAT32.

    @param[in] pPath        the full path of file or folder.
    @param[out] creDateTime the file create date & time, ex: creDateTime[6] = {2012,5,5, 10, 9, 11};  -> y/m/d/h/m/s
    @param[out] modDateTime the file modified date & time, ex: modDateTime[6] = {2013,6,6, 13,12,14}; -> y/m/d/h/m/s

    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        UINT32   uiCreDateTime1[6] = {0};
        UINT32   uiModDateTime1[6] = {0};
        char     tmpfilename[50];

        sprintf(tmpfilename,"A:\\timetest.txt");
        FileSys_GetDateTime(tmpfilename, uiCreDateTime1,  uiModDateTime1);
    }
    @endcode
*/
extern INT32    FileSys_GetDateTime(char *pPath, UINT32  creDateTime[FST_FILE_DATETIME_MAX_ID], UINT32  modDateTime[FST_FILE_DATETIME_MAX_ID]);

/**
    Set file Datetime.

    This api will set the create & modify time of specific file or folder.

    @note
    -# Only support Long File Name for FAT12/FAT16/FAT32.

    @param[in] pPath        the full path of file or folder.
    @param[in] creDateTime  the file create date & time, ex: creDateTime[6] = {2012,5,5, 10, 9, 11};  -> y/m/d/h/m/s
    @param[in] modDateTime  the file modified date & time, ex: modDateTime[6] = {2013,6,6, 13,12,14}; -> y/m/d/h/m/s

    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        UINT32   uiCreDateTime1[6] = {2012,5,5, 10,9, 11};
        UINT32   uiModDateTime1[6] = {2013,6,6, 13,12,14};
        char     tmpfilename[50];

        sprintf(tmpfilename,"A:\\timetest.txt");
        FileSys_GetDateTime(tmpfilename, uiCreDateTime1,  uiModDateTime1);
    }
    @endcode
*/
extern INT32    FileSys_SetDateTime(char *pPath, UINT32  creDateTime[FST_FILE_DATETIME_MAX_ID], UINT32  modDateTime[FST_FILE_DATETIME_MAX_ID]);

/**
    Delete a file.

    Delete a file.

    @note
    -# Only support Long File Name for FAT12/FAT16/FAT32.

    @param[in] pPath    file your want to delete ,must be full path.
    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.

*/
extern INT32    FileSys_DeleteFile(char *pPath);

/**
    Delete a folder.

    Delete a folder.

    @note:
    -# Delete folder would recursively delete all files under the folder.
    -# Only support Long File Name for FAT12/FAT16/FAT32.

    @param[in] pPath    folder your want to delete ,must be full path.
    @return
         - @b #FST_STA_OK: The operation is success.
         - @b Other than #FST_STA_OK: has some error.

*/
extern INT32    FileSys_DeleteDir(char *pPath);

/**
    Rename a file.

    Rename a file.

    @note
    -# Only support Long File Name for FAT12/FAT16/FAT32.

    @param[in] *pNewname        new name of file, name only.
    @param[in] *pPath           the original full path of file that wants to be renamed ,must be full path.
    @param[in]  bIsOverwrite    If new file already exists, overwrite it or not.
    @return
         - @b #FST_STA_OK: The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        FileSys_RenameFile("attrib2.txt","A:\\attrib.txt" ,FALSE);
    }
    @endcode
*/
extern INT32    FileSys_RenameFile(char *pNewname, char *pPath, BOOL bIsOverwrite);

/**
    Rename a folder.

    Rename a folder.

    @note
    -# Only support Long File Name for FAT12/FAT16/FAT32.

    @param[in] *pNewname        new name of folder, name only.
    @param[in] *pPath           the original full path of folder that wants to be renamed ,must be full path.
    @param[in]  bIsOverwrite    If new foldername already exists, overwrite it or not.
    @return
         - @b #FST_STA_OK: The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        FileSys_RenameDir("DCAM","A:\\DCIM",FALSE);
    }
    @endcode
*/
extern INT32    FileSys_RenameDir(char *pNewname, char *pPath, BOOL bIsOverwrite);

/**
    Move a file.

    Move a file for one path to another.

    @note
    -# Only support Long File Name for FAT12/FAT16/FAT32.

    @param[in] pSrcPath the src full path of file.
    @param[in] pDstPath the dst full path of file.
    @return
         - @b #FST_STA_OK: The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        FileSys_MoveFile("A:\\2.jpg","A:\\temp\\2.jpg");
    }
    @endcode
*/
extern INT32    FileSys_MoveFile(char *pSrcPath, char *pDstPath);

// --------------------------------------------------------------------
// file length
// --------------------------------------------------------------------

/**
    Get file length.

    This API will open a file and get the file size then close a file. \n
    If the file is already opened, it will not open the file again but just return the file size.

    @note
    -# User can use another API to get file length if already got the file handle of a file.\n
    Firstly seek file to file end & then using FileSys_TellFile() to get the file length.
    -# Only support Long File Name for FAT12/FAT16/FAT32.

    @param[in] pPath    The file full path.
    @return the file size

*/
extern UINT64   FileSys_GetFileLen(char *pPath);

// --------------------------------------------------------------------
// file access
// --------------------------------------------------------------------

/**

    Register call back for write file.

    If user used this API to register callback function. User will receive the notification when any File System write file command end.\n
    It is usually used when the write file command is issued by the library layer or the application layer,\n
    but the project layer need to do something after the file write command end.

    @param[in] CB   The callback function pointer.

*/
extern void     FileSys_RegisterCB(FileSys_CB CB);

/**
    Open a file.

    Open a file in specified mode.
    @note
    -# When user want to open a file for write, the open flag should not be setted as #FST_OPEN_WRITE.\n
    It should be setted as #FST_OPEN_WRITE|#FST_OPEN_ALWAYS or #FST_OPEN_WRITE|#FST_CREATE_ALWAYS.
    -# Only support Long File Name for FAT12/FAT16/FAT32.

    @param[in] pPath    Full file path name to open.
    @param[in] Flag     File open mode. ex: #FST_OPEN_READ or #FST_OPEN_WRITE|#FST_OPEN_ALWAYS.
    @return If open succeed, it will return the file handle point else if fail it will return NULL.

    Example:
    @code
    {
        FST_FILE  filehdl;

        // case 1 : open file for read
        filehdl = FileSys_OpenFile("A:\\DCIM\\200NVTIM\\IMAG0100.RAW",FST_OPEN_READ);

        // case 2: open file for write
        filehdl = FileSys_OpenFile("A:\\DCIM\\200NVTIM\\IMAG0100.RAW",FST_OPEN_WRITE|FST_CREATE_ALWAYS);

        // case 3: open file for update
        filehdl = FileSys_OpenFile("A:\\DCIM\\200NVTIM\\IMAG0100.RAW",FST_OPEN_WRITE|FST_OPEN_ALWAYS);

    }
    @endcode
*/
extern FST_FILE  FileSys_OpenFile(char *pPath, UINT32 Flag);

/**
    Close a file.

    Close a file by file handle.

    @param[in] pFile    The file handle.
    @return
      - @b #FST_STA_OK : close file success.
      - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        FST_FILE  filehdl;

        filehdl = FileSys_OpenFile("A:\\DCIM\\200NVTIM\\IMAG0100.RAW",FST_OPEN_READ);
        FileSys_CloseFile(filehdl);
    }
    @endcode
*/
extern INT32    FileSys_CloseFile(FST_FILE pFile);

/**
    Read a file.

    The API will read specified size of a file from file current position.

    @param[in] pFile        The file handle.
    @param[in] pBuf         The buffer to put reading data.
    @param[in,out] pBufSize The size want to read. After reading complete the size will return really reading size.
    @param[in] Flag         FST_FLAG_XXXX is defined in r/w file flag. #FST_FLAG_ASYNC_MODE means read in asynchronous mode.
                            User can just fill #FST_FLAG_NONE if read in synchronous mode.
    @param[in] CB   Callback function is used in asynchronous mode. In synchronous mode just fill NULL. \n
                    UINT32  MessageID   : The read file result. \n
                    UINT32  Param1      : The readed file size. \n
                    UINT32  Param2      : reserved parameter for future use.    \n
                    UINT32  Param3      : reserved parameter for future use.    \n

    @return
      - @b #FST_STA_OK: read file success. (If #FST_FLAG_ASYNC_MODE is used, always return #FST_STA_OK)
      - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        UINT32 size = 0x20000;
        FST_FILE     filehdl;

        filehdl = FileSys_OpenFile("A:\\DCIM\\200NVTIM\\IMAG0100.RAW",FST_OPEN_READ);
        FileSys_ReadFile(filehdl, (UINT8 *)pBuf, &size, 0, NULL);
        FileSys_CloseFile(filehdl);
    }
    @endcode
*/
extern INT32    FileSys_ReadFile(FST_FILE pFile, UINT8 *pBuf, UINT32 *pBufSize, UINT32 Flag, FileSys_CB CB);

/**
    Write a file.

    The API will write specified size of data to a file from file current position.

    @param[in] pFile    The file handle.
    @param[in] pBuf     The data buffer.
    @param[in,out] pBufSize the size want to write. After writing complete the pBufSize value will be overwrited with write size.
                if the writed size != input buffer size means write command has some error happened.
    @param[in] Flag     FST_FLAG_XXXX is defined in r/w file flag. #FST_FLAG_ASYNC_MODE means write in asynchronous mode.
                   User can just fill #FST_FLAG_NONE if write in synchronous mode.
    @param[in] CB       Callback function is used in asynchronous mode. In synchronous mode just fill NULL.
                    The argument of callback function :
                    UINT32  MessageID   : The write file result.
                    UINT32  Param1      : The writed file size.
                    UINT32  Param2      : reserved parameter for future use.
                    UINT32  Param3      : reserved parameter for future use.
    @return
      - @b #FST_STA_OK: write file success. (If #FST_FLAG_ASYNC_MODE is used, always return #FST_STA_OK)
      - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        UINT32 size = 0x20000;
        FST_FILE     filehdl;

        filehdl = FileSys_OpenFile("A:\\DCIM\\200NVTIM\\IMAG0100.RAW",FST_OPEN_WRITE|FST_CREATE_ALWAYS);
        FileSys_WriteFile(filehdl, (UINT8 *)pBuf, &size, 0, NULL);
        FileSys_CloseFile(filehdl);
    }
    @endcode
*/

extern INT32    FileSys_WriteFile(FST_FILE pFile, UINT8 *pBuf, UINT32 *pBufSize, UINT32 Flag, FileSys_CB CB);
/**
    Seek a file.

    Sets the file's current position at the offset.

    @param[in] pFile        The file handle.
    @param[in] offset       The offset to seek.
    @param[in] fromwhere    seek from which position.
    @return
         - @b #FST_STA_OK: The operation is success.
         - @b Other than #FST_STA_OK: has some error.
    Example:
    @code
    {
        FST_FILE     filehdl;

        filehdl = FileSys_OpenFile("A:\\DCIM\\200NVTIM\\IMAG0100.RAW",FST_OPEN_READ);
        FileSys_SeekFile(filehdl, pos, FST_SEEK_SET);
    }
    @endcode
*/
extern INT32    FileSys_SeekFile(FST_FILE pFile, UINT64 offset, FST_SEEK_CMD fromwhere);
/**
    Tell the file position.

    Get the file's current position.

    @param[in] pFile    The file handle.
    @return the file current position.

    Example:
    @code
    {
        FST_FILE     filehdl;
        UINT32       pos =0, getpos=0;

        filehdl = FileSys_OpenFile("A:\\DCIM\\200NVTIM\\IMAG0100.RAW",FST_OPEN_READ);
        pos =100;
        FileSys_SeekFile(filehdl, pos, FST_SEEK_SET);
        getpos = FileSys_TellFile(filehdl);
    }
    @endcode
*/
extern UINT64   FileSys_TellFile(FST_FILE pFile);

/**
    Flush the file.

    This API will store the current file to FAT. Normally the file will be stored to FAT when file closed.\n
    User can use this API to store the file to FAT forcely but not close the file.

    @param[in] pFile    The file handle.
    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        FST_FILE     filehdl;
        UINT32       i,count =60;

        filehdl = FileSys_OpenFile("A:\\DCIM\\200NVTIM\\IMAG0100.AVI",FST_CREATE_ALWAYS|FST_OPEN_WRITE);
        loopwritesize = 0x100000;
        for(i=0;i<count;i++)
        {
            FileSys_WriteFile(filehdl, (UINT8*)(pBuf), &loopwritesize, 0, NULL);
            FileSys_FlushFile(filehdl);
        }
        FileSys_CloseFile(filehdl);
    }
    @endcode
*/
extern INT32    FileSys_FlushFile(FST_FILE pFile);

/**
    Get file status.

    This api will get the status of the specific file handle.

    @param[in] pFile        The file handle.
    @param[in] pBuf         The buffer to get the file status.

    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        FST_FILE_STATUS FileStat;
        FST_FILE        filehdl;

        filehdl = FileSys_OpenFile("A:\\DCIM\\200NVTIM\\IMAG0100.RAW",FST_OPEN_READ);
        FileSys_StatFile(filehdl, &FileStat);
        FileSys_CloseFile(filehdl);
    }
    @endcode
*/
extern INT32    FileSys_StatFile(FST_FILE pFile, FST_FILE_STATUS *pFileStat);
// --------------------------------------------------------------------
// util
// --------------------------------------------------------------------

/**
    Copy a file.

    Copy a file from one storage to another.

    @note
    -# Only support Long File Name for FAT12/FAT16/FAT32.

    @param[in] pCopyInfo    The copy information.
    @return
         - @b #FST_STA_OK: The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        CopyInfo.pSrcStrg = (HNVT_STRG)sdio_getStorageObject(STRG_OBJ_FAT1);
        CopyInfo.pDstStrg = (HNVT_STRG)nand_getStorageObject(STRG_OBJ_FAT1);
        CopyInfo.pSrcPath = SrcPath;
        CopyInfo.pDstPath = DstPath;
        CopyInfo.pBuf = (char *)pBuf;
        CopyInfo.uiBufSize = 0x1000000;
        CopyInfo.bDelete = TRUE;
        FileSys_CopyToByName(&CopyInfo);
    }
    @endcode

*/
extern INT32    FileSys_CopyToByName(COPYTO_BYNAME_INFO *pCopyInfo);

/**
    Install commands.

    Install File System commands into the shell system.
*/
extern void     FileSys_InstallCmd(void);

/**
    Truncate the file size.

    This api will truncate a file to a specified length.

    @note
    -# If the original size was larger than the new size, the extra data of the file is lost.
    -# If the original size was smaller than the new size, the file is extended and the extra data is set to zero.

    @param[in] pFile        The file handle.
    @param[in] NewSize      The new size to be truncated to.

    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        FST_FILE        filehdl;

        filehdl = FileSys_OpenFile("A:\\DCIM\\200NVTIM\\IMAG0100.RAW", FST_CREATE_ALWAYS|FST_OPEN_WRITE);
        FileSys_TruncFile(filehdl, 2*100*1024*1024);//truncate to 200MB
        FileSys_CloseFile(filehdl);
    }
    @endcode
*/
extern INT32    FileSys_TruncFile(FST_FILE pFile, UINT64 NewSize);

/**
    Allocate the file size.

    This api will allocate or deallocate the specified length to a file.

    @note
    -# If the original size was larger than the new size, the extra data of the file is lost.
    -# If the original size was smaller than the new size, the file is extended but the extra data remains dirty.

    @param[in] pFile        The file handle.
    @param[in] NewSize      The new size to be allocated or deallocated to.
    @param[in] Reserved1    Reserved for future use. Please set to 0.
    @param[in] Reserved2    Reserved for future use. Please set to 0.

    @return
         - @b #FST_STA_OK : The operation is success.
         - @b Other than #FST_STA_OK: has some error.

    Example:
    @code
    {
        FST_FILE        filehdl;

        filehdl = FileSys_OpenFile("A:\\DCIM\\200NVTIM\\IMAG0100.RAW", FST_CREATE_ALWAYS|FST_OPEN_WRITE);
        FileSys_AllocFile(filehdl, 2*100*1024*1024, 0, 0);//allocate to 200MB
        FileSys_CloseFile(filehdl);
    }
    @endcode
*/
extern INT32    FileSys_AllocFile(FST_FILE pFile, UINT64 NewSize, UINT32 Reserved1, UINT32 Reserved2);

//@}
#endif
