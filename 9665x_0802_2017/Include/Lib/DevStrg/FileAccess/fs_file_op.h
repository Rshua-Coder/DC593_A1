////////////////////////////////////////////////////////////////
/**
    Header for File System Library

    This file maintain the C language file operation interface and search functions.

    @file       fs_file_op.h
    @ingroup    mILibFileAccess
    @note       For DMA access, every file access and size must fall into word boundary.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _FS_FILE_OP_H
#define _FS_FILE_OP_H
#include "type.h"

/**
    @addtogroup mILibFileAccess
*/
//@{
//--------------------------------------
// Constant definition
//--------------------------------------

/**
    The maximum number of files that the implementation guarantees can be open at the same time.
*/
#define KFS_FOPEN_MAX_NUM           6//4                                        ///< the maximum number of opened files.


/**
    The maximum number of search handle.
    @note KFS_FSEARCH_MAX_NUM, KFS_MAX_DIRECTORY_DEEP are related,should the same

*/
//#NT#2012/12/25#Nestor Yang -begin
//#NT# Increase folder deep limitation from 4 to 6 for MT code
//#define KFS_FSEARCH_MAX_NUM         4                                        ///< the maximum number of search handle
#define KFS_FSEARCH_MAX_NUM         6                                        ///< the maximum number of search handle
//#NT#2012/12/25#Nestor Yang -end

/**
    @name FS file name length definition
*/
//@{
#define KFS_MAX_DIRECTORY_DEEP      KFS_FSEARCH_MAX_NUM                       ///< the maximum directory deepth in path
#define KFS_PATH_MAX_LENG           3+ (9 * KFS_MAX_DIRECTORY_DEEP)           ///< the max full path length of directory, 3 is "A:\" , 9  is dir name 8 chars + "\" 1 char
#define KFS_FILENAME_MAX_LENG       12                                        ///< the max length of short file name, 8.3 format + 1 null terminal
#define KFS_FILEPATH_MAX_LENG       KFS_PATH_MAX_LENG + KFS_FILENAME_MAX_LENG ///< the max full path length of file
#define KFS_LONGFILENAME_MAX_LENG   (128)                                     ///< the max length of long file name
#define KFS_LONGNAME_PATH_MAX_LENG  (KFS_LONGFILENAME_MAX_LENG + 4)           ///< the max length of long file name path
//@}

/**
    @name FS seek command
*/
//@{
#define KFS_SEEK_SET                 0                        ///<  start of stream (see fseek)
#define KFS_SEEK_CUR                 1                        ///<  current position in stream (see fseek)
#define KFS_SEEK_END                 2                        ///<  end of stream (see fseek)
//@}

/**
    @name FS search file direction
*/
//@{
#define KFS_FORWARD_SEARCH          (0)                       ///<  search file forward
#define KFS_BACKWARD_SEARCH         (1)                       ///<  search file backward
#define KFS_HANDLEONLY_SEARCH       (2)                       ///<  only get a search handle, used by fs_SearchFileOpen()
//@}

/**
     @name FS API return status
*/
//@{
#define FSS_OFFSET                  (-256)
#define FSS_OK                      (0)                       ///< file system status ok
#define FSS_FAIL                    ((-1)+ FSS_OFFSET)        ///< status fail
#define FSS_BUSY                    ((-2)+ FSS_OFFSET)        ///< status busy
#define FSS_UNFORMAT                ((-3)+ FSS_OFFSET)        ///< storage is unformat. if realy want to use please format first
#define FSS_EXCESS_SIZE             ((-4)+ FSS_OFFSET)        ///< access area excess storage's size
#define FSS_CANT_ACCESS             ((-5)+ FSS_OFFSET)        ///< try to access reserved area
#define FSS_ERROR_PARAM             ((-6)+ FSS_OFFSET)        ///< parameter error
#define FSS_UNKNOW_FORMAT           ((-7)+ FSS_OFFSET)        ///< unknow format
#define FSS_DUPLEX_NAME             ((-8)+ FSS_OFFSET)        ///< find the same name in folder
#define FSS_NOMORE_SPACE            ((-9)+ FSS_OFFSET)        ///< nomore space
#define FSS_OPEN_TOO_MANY           ((-10)+ FSS_OFFSET)       ///< it open too many files
#define FSS_IS_FOLDER               ((-11)+ FSS_OFFSET)       ///< we want to open a folder
#define FSS_ILLEGAL_FILENAME        ((-12)+ FSS_OFFSET)       ///< filenaem is invalid, usually due to non 8.3 filename or file path deep
#define FSS_OPERATION_DENY          ((-13)+ FSS_OFFSET)       ///< don't allow this file operation.
#define FSS_FORMAT_FAT32            ((-14)+ FSS_OFFSET)       ///< FAT 32 format, not support so far
#define FSS_NO_MORE_FILES           ((-16)+ FSS_OFFSET)       ///< use in search file, it means no more file in folder/
#define FSS_CARD_ERROR              ((-17)+ FSS_OFFSET)       ///< card error
#define FSS_STRSIZE_MISMATCH        ((-18)+ FSS_OFFSET)       ///< Storage Size mismatch, check Nand total sectors match PBR Setting
#define FSS_FILE_NOTEXIST           ((-19)+ FSS_OFFSET)       ///< file is not exist
#define FSS_DATA_DAMAGE             ((-20)+ FSS_OFFSET)       ///< data damage
//@}


/**
     @name Directory entry attribute bit
*/
//@{
#define FS_ATTRIB_READ              0x01                     ///< bit 0, indicates the file is read only.
#define FS_ATTRIB_HIDDEN            0x02                     ///< bit 1, indicates the file should not be shown in normal directory listings.
#define FS_ATTRIB_SYSTEM            0x04                     ///< bit 2, indicates the file is an operating system file.
#define FS_ATTRIB_VOLUME            0x08                     ///< bit 3, indicates the entry is the label for the volume.
#define FS_ATTRIB_DIRECTORY         0x10                     ///< bit 4, indicates it is a directory
#define FS_ATTRIB_ACHIEVE           0x20                     ///< bit 5, indicates the file has been created or modified since the attribute was clear
//@}


/**
     @name FS attribute macro
*/
//@{
#define M_IsReadOnly(x)             ((x) & FS_ATTRIB_READ)          ///< if the file is read only.
#define M_IsHidden(x)               ((x) & FS_ATTRIB_HIDDEN)        ///< if the file is a hidden file.
#define M_IsSystem(x)               ((x) & FS_ATTRIB_SYSTEM)        ///< if the file is an operating system file.
#define M_IsVolumeLabel(x)          ((x) & FS_ATTRIB_VOLUME)        ///< if the entry is the label for the volume.
#define M_IsDirectory(x)            ((x) & FS_ATTRIB_DIRECTORY)     ///< if it is a directory.
#define M_IsArchive(x)              ((x) & FS_ATTRIB_ACHIEVE)       ///< if the file has been created or modified.
#define M_IsFile(x)                 (!((x) & FS_ATTRIB_DIRECTORY))  ///< if it is a file.
//@}

/**
    @name FS get date macro
    parse the date field to year, month, day
*/
//@{
#define FS_GET_DAY_FROM_DATE(x)     (x & 0x1F)                  ///<  get day from date
#define FS_GET_MONTH_FROM_DATE(x)   ((x >> 5) & 0x0F)           ///<  get month from date
#define FS_GET_YEAR_FROM_DATE(x)    (((x >> 9) & 0x7F)+1980)    ///<  get year from date
//@}

/**
    @name FS get time macro
    parse the time field to hour, minute, second
*/
//@{
#define FS_GET_SEC_FROM_TIME(x)     ((x & 0x001F)<<1)           ///<  get seconds from time
#define FS_GET_MIN_FROM_TIME(x)     ((x & 0x07E0)>>5)           ///<  get minutes from time
#define FS_GET_HOUR_FROM_TIME(x)    ((x & 0xF800)>>11)          ///<  get hours from time
//@}

/**
    Search File Handle.
*/
typedef UINT32*  FS_SEARCH_HDL;

/**
    The storage object pointer.
*/
typedef UINT32 HNVT_STRG;


//--------------------------------------
// structure define
//--------------------------------------


/**
    This structure contain the necessary parameter for file system operation.

*/
typedef struct _FS_INIT_PARAM
{
    UINT32       WorkBuf;               ///< the work buffer for filesys, should be 32 bytes align
    UINT32       WorkBufSize;           ///< work buffer size
    BOOL         bSupportExFAT;         ///< is support exfat or not
}FS_INIT_PARAM, *PFS_INIT_PARAM;



/**
    This sturcture contain data for found file.
*/
typedef struct _FIND_DATA
{
    char            FATMainName[8];     ///< Major file name.
    char            FATExtName[3];      ///< Extern file name.
    unsigned char   attrib;             ///< File attribute.
    unsigned char   Reserved;           ///< Reserved.
    unsigned char   creSecond;          ///< File created time.
    unsigned short  creTime;            ///< File created time. bit0~4: seconds/2, bit5~10: minutes, bit11~15: hours.
    unsigned short  creDate;            ///< File created date. bit0~4: day(1-31), bit5~8: month(1-12), bit9~15: year(0-127) add 1980 to convert
    unsigned short  lastAccessDate;     ///< The file latest access date.
    unsigned short  FstClusHI;          ///< The high 2 bytes of file start cluster.
    unsigned short  lastWriteTime;      ///< The latest time of write file.
    unsigned short  lastWriteDate;      ///< The latest date of write file.
    unsigned short  FstClusLO;          ///< The low 2 bytes of file start cluster.
    unsigned int    fileSize;           ///< The low 4 bytes of file size in bytes.
    //#NT#2012/08/20#Lincy Lin -begin
    //#NT#Support file size over 4GB
    unsigned int    fileSizeHI;         ///< The high 4 bytes of file size in bytes.
    //#NT#2012/08/20#Lincy Lin -end
    char            filename[16];       ///< The full filename.
    //#NT#2010/07/01#Brad Chen -begin
    //#NT# support exFAT
    UINT32          PartitionType;      ///< system ID, what kind of file system has been install on partition
    BOOL            bNoFatChain;       ///< if this file has a fat cluster chain. (only for exFAT)
    UINT32          ClusterCount;       ///< the total cluster count of this file
    //#NT#2010/07/01#Brad Chen -end
}FIND_DATA, *PFIND_DATA;


//--------------------------------------
// function declare
//--------------------------------------

//------------------------------------------------------------------------------
// directory api
//------------------------------------------------------------------------------
/**
    Get current working directory.

    @return current working directory string
*/
extern char*            fs_getcwd(void);
/**
    Set current working directory.

    @param[in] pPath    full path of the current path.
*/
extern void             fs_setcwd(const char *pPath);
/**
    Get parent directory.

    @param[in] pPath    full path of the current path.(i.e. there is the "\\" in the end. ex: "A:\\DCIM\\")
    @param[out] parentDir   return parentDir ,should memset before call this function,it would includ "\\",ex "A:\\"
    @return #FSS_OK, or #FSS_FAIL
*//////////////////////////////////////////////////////////////////////////
extern int              fs_dir_getParent(const char *pPath,char *parentDir);

/**
    Change directory.

    @param[in] pPath    relative path of the current directory.
    @return FSS_OK or FSS_FAIL.
*/
extern int              fs_chdir(char *pPath);


//------------------------------------------------------------------------------
// file manager api
//------------------------------------------------------------------------------
/**
    Searches a directory or file whose name matches the specified file name.

    @param[in] pPath        path of directory, must end with '\'
    @param[out] pFindData   a structure to storage found data
        only available when success.
    @param[in] Direction    #KFS_FORWARD_SEARCH, #KFS_BACKWARD_SEARCH, or #KFS_HANDLEONLY_SEARCH\n
        if the parameter is #KFS_FORWARD_SEARCH, the return FindData will be the first file in the folder.  \n
        if the parameter is #KFS_BACKWARD_SEARCH, the return FindData will be the last file in the folder.  \n
        if the parameter is #KFS_HANDLEONLY_SEARCH, the return FindData will be invalid.
    @param[in] pLongFilename    It can be NULL if long filename is unnecessary. \n
             This is a #KFS_LONGFILENAME_MAX_LENG numbers wide string point to store the returned long filename.\n
             If the file doesn't have the long filename, the first element will be NULL(string length 0).   \n
             If the Direction is #KFS_HANDLEONLY_SEARCH, the return pLongFilename will be invalid.
    @return If the function succeeds, the return value is a search handle and return NULL for error.
        used in a subsequent call to fs_SearchFile().

    Example:
    @code
    {
        __align(4) unsigned short gLongFilenameBuf[KFS_LONGFILENAME_MAX_LENG];

        function()
        {
            FIND_DATA       findData;
            unsigned short *pLongFilename;
            FS_SEARCH_HDL   pSearch;
            char searchPath[] = "A:\\DCIM\\";

            pLongFilename = gLongFilenameBuf;
            ...
            pSearch = fs_SearchFileOpen(searchPath, &findData, KFS_FORWARD_SEARCH ,pLongFilename);
            while(pSearch)
            {
                if(pLongFilename[0] != NULL)
                {
                    // have long filename.
                    ...
                }
                // do some thing here.
                ...
                if(fs_SearchFile(pSearch, &findData, KFS_FORWARD_SEARCH, pLongFilename)!= FSS_OK)
                {
                    fs_SearchFileClose(pSearch);
                    break;
                }
            }
        }
    }
    @endcode
*/
extern FS_SEARCH_HDL    fs_SearchFileOpen(char *pPath, FIND_DATA *pFindData, int Direction, unsigned short *pLongFilename);
/**
    The function continues a file search from a previous call to the fs_SearchFileOpen() function.

    @param[in] pSearch  the return handle of fs_SearchFileOpen();

    @param[out] pFindData   the Pointer to the #FIND_DATA structure that
            receives information about the found file or subdirectory

    @param[in] Direction    #KFS_FORWARD_SEARCH or #KFS_BACKWARD_SEARCH \n
        if the parameter is #KFS_FORWARD_SEARCH, the return pFindData will start from the first file in the folder. \n
        if the parameter is #KFS_BACKWARD_SEARCH, the return pFindData will start from the last file in the folder.

    @param[in] pLongFilename    can be NULL if long filename is unnecessary,
             this is a #KFS_LONGFILENAME_MAX_LENG numbers wide string point to store the returned long filename.
             If the file doesn't have the long filename, the first element will be NULL(string length 0).

    @return The function returns #FSS_OK , #FSS_NO_MORE_FILES or #FSS_FAIL.
*/
extern int              fs_SearchFile(FS_SEARCH_HDL pSearch, FIND_DATA *pFindData, int Direction, unsigned short *pLongFilename);

/**
    Close the specified search handle.

    @param[in] pSearch  This handle must have been previously opened by the fs_SearchFileOpen() function.

    @return #FSS_OK or #FSS_FAIL.
*//////////////////////////////////////////////////////////////////////////
extern int              fs_SearchFileClose(FS_SEARCH_HDL pSearch);

/**
    Rewind the specified search handle.

    @param[in] pSearch  This handle must have been previously opened by the fs_SearchFileOpen() function.

    @return #FSS_OK or #FSS_FAIL.
*//////////////////////////////////////////////////////////////////////////
extern int              fs_SearchFileRewind(FS_SEARCH_HDL pSearch);
//@}
#endif //_FS_FILE_OP_H

