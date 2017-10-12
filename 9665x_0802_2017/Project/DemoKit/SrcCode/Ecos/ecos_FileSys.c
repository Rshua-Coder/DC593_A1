//eCos header
#include <cyg/fs/nvtfs.h>
#include <dirent.h> //for listdir
#include <unistd.h>

//uItron header
#include <stdio.h>
#include "ecos_FileSys.h"
#include "fs_file_op.h"
#include "FileSysTsk.h"

//debug header and flag
#define __MODULE__          eCos_FileSys
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

//global define
#define JD_1_JAN_1970 2440588 // 1 Jan 1970 in Julian day number

//private function prototype
static char* cyg_FileSys_GetVerInfo(void);
static char* cyg_FileSys_GetBuildDate(void);
static int cyg_FileSys_OpenFile( const char *path, int oflag, bool bIsAbsPath);//open (with path)
static int cyg_FileSys_CloseFile( int fd );//close
static int cyg_FileSys_ReadFile( int fd, void *buf, ssize_t len );//read
static int cyg_FileSys_WriteFile( int fd, const void *buf, ssize_t len );//write
static int cyg_FileSys_SeekFile( int fd, off64_t pos, int whence );//lseek
static int cyg_FileSys_StatFile( int fd, struct stat *pStat);//fstat
static int cyg_FileSys_FlushFile( int fd );//fsync
static int cyg_FileSys_DeleteFile( const char *path, bool bIsAbsPath);//unlink (with path)
static int cyg_FileSys_RenameFile( const char *pNewName, const char *path, bool bIsAbsPath);//rename (with path)

static int cyg_FileSys_MakeDir( const char *path, bool bIsAbsPath );//mkdir (with path)
static int cyg_FileSys_DeleteDir( const char *path, bool bIsAbsPath );//rmdir (with path)

static int cyg_FileSys_Stat( const char *path, struct stat *pStat, bool bIsAbsPath);//stat (with path)
static int cyg_FileSys_GetAttrib( const char *path, cyg_uint32 *pAttrib, bool bIsAbsPath);//cyg_fs_get_attrib (with path)
static int cyg_FileSys_SetAttrib( const char *path, cyg_uint32 uiAttrib, bool bIsAbsPath);//cyg_fs_get_attrib (with path)

static char* cyg_fs_getcwd( void );//getcwd
static int cyg_fs_chdir( const char *path, bool bIsAbsPath );//chdir (with path)

static int cyg_fs_SearchFileOpen( const char *path, bool bIsAbsPath);//opendir (with path)
static int cyg_fs_SearchFile( int fd, char *pName, ssize_t len);//readdir
static int cyg_fs_SearchFileClose( int fd );//closedir
static int cyg_fs_SearchFileRewind( int fd );//rewinddir

//global variable
static FileSys_CBS g_FileSys_CBS = {&cyg_FileSys_GetVerInfo,
                                    &cyg_FileSys_GetBuildDate,
                                    &cyg_FileSys_OpenFile,
                                    &cyg_FileSys_CloseFile,
                                    &cyg_FileSys_ReadFile,
                                    &cyg_FileSys_WriteFile,
                                    &cyg_FileSys_SeekFile,
                                    &cyg_FileSys_StatFile,
                                    &cyg_FileSys_FlushFile,
                                    &cyg_FileSys_DeleteFile,
                                    &cyg_FileSys_RenameFile,
                                    &cyg_FileSys_MakeDir,
                                    &cyg_FileSys_DeleteDir,
                                    &cyg_FileSys_Stat,
                                    &cyg_FileSys_GetAttrib,
                                    &cyg_FileSys_SetAttrib,
                                    &cyg_fs_getcwd,
                                    &cyg_fs_chdir,
                                    &cyg_fs_SearchFileOpen,
                                    &cyg_fs_SearchFile,
                                    &cyg_fs_SearchFileRewind,
                                    &cyg_fs_SearchFileClose,
                                   };

//function definition
static BOOL MakeFileSysPath(char* pOutPath, const char* pInPath, bool bIsRoot)
{
    char* pScan;

    if (NULL == pOutPath)
    {
        DBG_ERR("pOutPath is NULL");
        return FALSE;
    }

    if (NULL == pInPath)
    {
        DBG_ERR("pInPath is NULL");
        return FALSE;
    }

    if( strlen(pInPath) > KFS_LONGFILENAME_MAX_LENG)
    {
        DBG_ERR("strlen(pInPath) = %d > KFS_LONGFILENAME_MAX_LENG", strlen(pInPath));
        return FALSE;
    }

    //add A:\\ to the head
    if(bIsRoot)
        sprintf(pOutPath, "A:\\%s", pInPath);
    else
    {
        //sprintf(pOutPath, "%s\\%s", fs_getcwd(), pInPath);
        DBG_ERR("Relative path is not supported\r\n");
        return FALSE;
    }

    //change slash to backslash (/ => \)
    pScan = pOutPath;
    while('\0' != *pScan)
    {
        if('/' == *pScan)
            *pScan = '\\';
        pScan++;
    }

    DBG_IND("pOutPath = %s, pInPath = %s, bIsRoot = %d\r\n", pOutPath, pInPath, bIsRoot);
    return TRUE;
}

//pSrcName should be a wide char long name
//pDstName will return a single char long name
static BOOL WideCharToMultiByte(char* pDstName, UINT16* pSrcName)
{
    int i;

    //ToDo: we should return when non-ASCII code is found (value > 128)
    for(i=0; i < KFS_LONGFILENAME_MAX_LENG; i++)
    {
        pDstName[i] = (char)pSrcName[i];
        if(0 == pSrcName[i])
            break;
    }

    DBG_IND("pDstName :%s \r\n", pDstName);
    return TRUE;
}

static cyg_uint32 gdate_to_jdays(int day, int month, int year)
{
    cyg_uint32 jd;
    jd = day - 32075 + 1461*(year + 4800 + (month - 14)/12)/4 +
          367*(month - 2 - (month - 14)/12*12)/12 -
          3*((year + 4900 + (month - 14)/12)/100)/4;
    return jd;
}

static void jdays_to_gdate(cyg_uint32 jd, int *day, int *month, int *year)
{
    cyg_uint32 l, n, i, j;

    l = jd + 68569;
    n = (4 * l) / 146097;
    l = l - (146097 * n + 3) / 4;
    i = (4000 * (l + 1)) / 1461001;
    l = l - (1461 * i) / 4 + 31;
    j = (80 * l) / 2447;
    *day = l - (2447 * j) / 80;

    l = j / 11;
    *month = j + 2 - (12 * l);
    *year = 100 * (n - 49) + i + l;
}

void listdir( char *name, int statp)
{
    int err;
    DIR *dirp;
    int num=0;

    DBG_DUMP("^M<INFO>: reading directory %s\r\n",name);

    dirp = opendir( name );
    if( dirp == NULL ) DBG_ERR("dirp == NULL\r\n");

    for(;;)
    {
        struct dirent *entry = readdir( dirp );

        if( entry == NULL )
            break;
        num++;
        DBG_DUMP("^M<INFO>: entry %14s",entry->d_name);
        if( statp )
        {
            char fullname[PATH_MAX];
            struct stat sbuf;

            if( name[0] )
            {
                strcpy(fullname, name );
                if( !(name[0] == '/' && name[1] == 0 ) )
                    strcat(fullname, "/" );
            }
            else fullname[0] = 0;

            strcat(fullname, entry->d_name );

            err = stat( fullname, &sbuf );
            if( err < 0 )
            {
                if( errno == ENOSYS )
                    DBG_DUMP(" <no status available>");
                else DBG_ERR("stat return err = %d\r\n", err);
            }
            else
            {
                DBG_DUMP("^M [mode %08x ino %08x nlink %d size %d]",
                            sbuf.st_mode,sbuf.st_ino,sbuf.st_nlink,sbuf.st_size);
            }
        }

        DBG_DUMP("\r\n");
    }

    err = closedir( dirp );
    if( err < 0 ) DBG_ERR("closedir failed\r\n");
}

void date_unix2dos(cyg_uint32  unix_timestamp, cyg_uint16 *dos_time, cyg_uint16 *dos_date)
{
    cyg_uint32 jd;
    cyg_uint16 dtime, ddate;
    int        hour, min, sec;
    int        day, month, year;

    hour = (unix_timestamp / 3600) % 24;
    min  = (unix_timestamp / 60) % 60;
    sec  =  unix_timestamp % 60;

    jd = JD_1_JAN_1970 + unix_timestamp / (3600 * 24);
    jdays_to_gdate(jd, &day, &month, &year);

    DBG_IND("timestamp=%d date=%d:%d:%d %d-%d-%d\r\n", unix_timestamp, hour, min, sec, year, month, day);

    if (year < 1980)
        year = 1980;

    dtime = (hour << 11) | (min << 5) | (sec >> 1);
    ddate = ((year - 1980) << 9) | (month << 5) | day;

    DBG_IND("dos time=%d date=%d\r\n", dtime, ddate);

    if (NULL != dos_time) *dos_time = dtime;
    if (NULL != dos_date) *dos_date = ddate;
}

static cyg_uint32 date_dos2unix(cyg_uint16 dos_time, cyg_uint16 dos_date)
{
    cyg_uint32 ts;
    int        hour, min, sec;
    int        day, month, year;

    DBG_IND("dos time=%d date=%d\r\n", dos_time, dos_date);

    sec        = (dos_time & ((1<<5)-1)) * 2;
    dos_time >>= 5;
    min        = (dos_time & ((1<<6)-1));
    dos_time >>= 6;
    hour       = dos_time;

    day        = (dos_date & ((1<<5)-1));
    dos_date >>= 5;
    month      = (dos_date & ((1<<4)-1));
    dos_date >>= 4;
    year       = dos_date + 1980;

    ts  = gdate_to_jdays(day, month, year);
    ts -= JD_1_JAN_1970;
    ts  = (ts * 24 * 3600) + (sec + min * 60 + hour * 3600);

    DBG_IND("timestamp=%d date=%d:%d:%d %d-%d-%d\r\n", ts, hour, min, sec, year, month, day);

    return ts;
}

void cyg_FileSys_RegisterCB(void)
{
    FileSysECOS_RegisterCB(&g_FileSys_CBS);
}

static char* cyg_FileSys_GetVerInfo(void)
{
    return NULL;
}

static char* cyg_FileSys_GetBuildDate(void)
{
    return NULL;
}

//bIsRoot is used to tell whether the path is relative path or absolute path
static int cyg_FileSys_OpenFile( const char *path, int oflag, bool bIsAbsPath)
{
    CHAR    szFileSysPath[KFS_LONGNAME_PATH_MAX_LENG];
    FST_FILE hFile;

    if(FALSE == MakeFileSysPath(szFileSysPath, path, bIsAbsPath))
    {
        DBG_ERR("MakeFileSysPath failed, path = %s, bIsAbsPath = %d\r\n", path, bIsAbsPath);
        return 0;//return a NULL pointer for open failure
    }

    DBG_IND("szFileSysPath = %s, oflag = %d\r\n", szFileSysPath, oflag);

    hFile = FileSys_OpenFile(szFileSysPath, oflag);

    if(NULL != hFile)//success
        DBG_IND("hFile = 0x%X\r\n", hFile);//handle
    else//fail
        DBG_ERR("hFile = NULL\r\n");//NULL handle

    return (int)hFile;
}

static int cyg_FileSys_CloseFile( int fd )
{
    INT32 iRet;

    DBG_IND("Close handle = 0x%X\r\n", fd);

    iRet = FileSys_CloseFile((FST_FILE)fd);

    if(FST_STA_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_FileSys_ReadFile( int fd, void *buf, ssize_t len )
{
    UINT32 uiLen = len;
    INT32 iRet;

    DBG_IND("fd = 0x%X, buf = 0x%X, uiLen = %d\r\n", fd, buf, uiLen);

    iRet = FileSys_ReadFile((FST_FILE)fd, (UINT8 *)buf, &uiLen, FST_FLAG_NONE, NULL);

    if(FST_STA_OK == iRet)
    {//success
        DBG_IND("iRet = %d\r\n", iRet, uiLen);
        return uiLen;//return the bytes which are read
    }
    else if(FST_STA_OK != iRet && uiLen >= 0 && uiLen < (UINT32)len)
    {//partial success
        DBG_IND("iRet = %d, read len = %d/%d, omit fs warnings\r\n", iRet, uiLen, len);
        return uiLen;//return the bytes which are read
    }
    else
    {
        DBG_ERR("iRet = %d\r\n", iRet);
        return iRet;
    }
}

static int cyg_FileSys_WriteFile( int fd, const void *buf, ssize_t len )
{
    UINT32 uiLen = len;
    INT32 iRet;

    DBG_IND("fd = 0x%X, buf = 0x%X, uiLen = %d\r\n", fd, buf, uiLen);

    iRet = FileSys_WriteFile((FST_FILE)fd, (UINT8 *)buf, &uiLen, FST_FLAG_NONE, NULL);

    if(FST_STA_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_FileSys_SeekFile( int fd, off64_t pos, int whence )
{
    FST_SEEK_CMD SeekCmd = FST_SEEK_MAX_ID;
    INT32 iRet;

    //transform the seek starting position
    switch(whence)
    {
        case SEEK_SET:
            SeekCmd = FST_SEEK_SET;
            break;
        case SEEK_CUR:
            SeekCmd = FST_SEEK_CUR;
            break;
        case SEEK_END:
            SeekCmd = FST_SEEK_END;
            break;
        default:
            DBG_ERR("unsupported seek cmd: %d\r\n", whence);
    }

    DBG_IND("fd = 0x%X, pos = 0x%llX, SeekCmd = %d\r\n", fd, pos, SeekCmd);

    iRet = FileSys_SeekFile((FST_FILE)fd, (UINT64)pos, SeekCmd);

    if(FST_STA_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_FileSys_StatFile( int fd, struct stat *pStat)
{
    FST_FILE_STATUS fStatus;
    INT32   iRet;

    DBG_IND("fd = 0x%X, pStat = 0x%X\r\n", fd, pStat);

    iRet = FileSys_StatFile((FST_FILE)fd, &fStatus);

    if(FST_STA_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    if(FST_STA_OK == iRet)
    {
        if(M_IsDirectory(fStatus.uiAttrib))
            pStat->st_mode = __stat_mode_DIR;//a directory
        else
            pStat->st_mode = __stat_mode_REG|S_IRWXU|S_IRWXG|S_IRWXO;//a regular file and set all access rights

        pStat->st_size = fStatus.uiFileSize;
        pStat->st_atime = date_dos2unix( 0, fStatus.uiAccessDate);
        pStat->st_mtime = date_dos2unix( fStatus.uiModifiedTime, fStatus.uiModifiedDate);
        pStat->st_ctime = date_dos2unix( fStatus.uiCreatedTime, fStatus.uiCreatedDate);
    }

    return iRet;
}

static int cyg_FileSys_FlushFile( int fd )
{
    INT32 iRet;

    DBG_IND("fd = 0x%X\r\n", fd);

    iRet = FileSys_FlushFile((FST_FILE)fd);

    if(FST_STA_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_FileSys_DeleteFile( const char *path, bool bIsAbsPath)
{
    CHAR    szFileSysPath[KFS_LONGNAME_PATH_MAX_LENG];
    INT32   iRet;

    if(FALSE == MakeFileSysPath(szFileSysPath, path, bIsAbsPath))
    {
        DBG_ERR("MakeFileSysPath failed, path = %s, bIsAbsPath = %d\r\n", path, bIsAbsPath);
        return FST_STA_PARAM_ERR;
    }

    DBG_IND("szFileSysPath = %s\r\n", szFileSysPath);

    iRet = FileSys_DeleteFile(szFileSysPath);

    if(FST_STA_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_FileSys_RenameFile( const char *pNewName, const char *path, bool bIsAbsPath)
{
    CHAR    szFileSysPath[KFS_LONGNAME_PATH_MAX_LENG];
    INT32   iRet;

    if(FALSE == MakeFileSysPath(szFileSysPath, path, bIsAbsPath))
    {
        DBG_ERR("MakeFileSysPath failed, path = %s, bIsAbsPath = %d\r\n", path, bIsAbsPath);
        return FST_STA_PARAM_ERR;
    }

    DBG_IND("pNewname = %s, szFileSysPath = %s\r\n", pNewName, szFileSysPath);

    iRet = FileSys_RenameFile((char *)pNewName, szFileSysPath, FALSE);

    if(FST_STA_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_FileSys_MakeDir( const char *path, bool bIsAbsPath )
{
    CHAR    szFileSysPath[KFS_LONGNAME_PATH_MAX_LENG];
    INT32   iRet;

    if(FALSE == MakeFileSysPath(szFileSysPath, path, bIsAbsPath))
    {
        DBG_ERR("MakeFileSysPath failed, path = %s, bIsAbsPath = %d\r\n", path, bIsAbsPath);
        return FST_STA_PARAM_ERR;
    }

    DBG_IND("szFileSysPath = %s\r\n", szFileSysPath);

    iRet = FileSys_MakeDir(szFileSysPath);

    if(FST_STA_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_FileSys_DeleteDir( const char *path, bool bIsAbsPath )
{
    CHAR    szFileSysPath[KFS_LONGNAME_PATH_MAX_LENG];
    INT32   iRet;

    if(FALSE == MakeFileSysPath(szFileSysPath, path, bIsAbsPath))
    {
        DBG_ERR("MakeFileSysPath failed, path = %s, bIsAbsPath = %d\r\n", path, bIsAbsPath);
        return FST_STA_PARAM_ERR;
    }

    DBG_IND("szFileSysPath = %s\r\n", szFileSysPath);

    iRet = FileSys_DeleteDir(szFileSysPath);

    if(FST_STA_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_FileSys_Stat( const char *path, struct stat *pStat, bool bIsAbsPath)
{
    FST_FILE_STATUS fStatus;
    INT32           iRet;
    CHAR            szFileSysPath[KFS_LONGNAME_PATH_MAX_LENG];
    FST_FILE        hFile;

    if(0 == strcmp(path, "") && bIsAbsPath)
    {//the path is the same as root, return a fake DIR value
        memset(pStat, 0, sizeof(struct stat));
        pStat->st_mode = __stat_mode_DIR;
        return FST_STA_OK;
    }

    if(FALSE == MakeFileSysPath(szFileSysPath, path, bIsAbsPath))
    {
        DBG_ERR("MakeFileSysPath failed, path = %s, bIsAbsPath = %d\r\n", path, bIsAbsPath);
        return FST_STA_PARAM_ERR;
    }

    DBG_IND("szFileSysPath = %s, pStat = 0x%X\r\n", szFileSysPath, pStat);

    hFile = FileSys_OpenFile(szFileSysPath, FST_OPEN_READ);
    if(NULL == hFile)
    {
        DBG_ERR("File not found\r\n");
        return FST_STA_ERROR;
    }
    iRet = FileSys_StatFile(hFile, &fStatus);
    FileSys_CloseFile(hFile);

    if(FST_STA_OK == iRet)
    {//success
        DBG_IND("iRet = %d\r\n", iRet);
        if(M_IsDirectory(fStatus.uiAttrib))
            pStat->st_mode = __stat_mode_DIR;//a directory
        else
            pStat->st_mode = __stat_mode_REG|S_IRWXU|S_IRWXG|S_IRWXO;//a regular file and set all access rights

        pStat->st_size = fStatus.uiFileSize;
        pStat->st_atime = date_dos2unix( 0, fStatus.uiAccessDate);
        pStat->st_mtime = date_dos2unix( fStatus.uiModifiedTime, fStatus.uiModifiedDate);
        pStat->st_ctime = date_dos2unix( fStatus.uiCreatedTime, fStatus.uiCreatedDate);
    }
    else
    {//fail
        DBG_ERR("iRet = %d\r\n", iRet);
    }

    return iRet;
}

static int cyg_FileSys_GetAttrib( const char *path, cyg_uint32 *pAttrib, bool bIsAbsPath)
{
    CHAR            szFileSysPath[KFS_LONGNAME_PATH_MAX_LENG];
    INT32           iRet;

    if(FALSE == MakeFileSysPath(szFileSysPath, path, bIsAbsPath))
    {
        DBG_ERR("MakeFileSysPath failed, path = %s, bIsAbsPath = %d\r\n", path, bIsAbsPath);
        return FST_STA_PARAM_ERR;
    }

    DBG_IND("szFileSysPath = %s, pAttrib = 0x%X\r\n", szFileSysPath, pAttrib);

    *pAttrib = 0;//set to zero first, because of the difference between UINT8 and UINT32
    iRet = FileSys_GetAttrib( szFileSysPath, (UINT8 *)pAttrib);

    if(FST_STA_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_FileSys_SetAttrib( const char *path, cyg_uint32 uiAttrib, bool bIsAbsPath)
{
    CHAR            szFileSysPath[KFS_LONGNAME_PATH_MAX_LENG];
    INT32           iRet;

    if(FALSE == MakeFileSysPath(szFileSysPath, path, bIsAbsPath))
    {
        DBG_ERR("MakeFileSysPath failed, path = %s, bIsAbsPath = %d\r\n", path, bIsAbsPath);
        return FST_STA_PARAM_ERR;
    }

    DBG_IND("szFileSysPath = %s, uiAttrib = 0x%X\r\n", szFileSysPath, uiAttrib);

    iRet = FileSys_SetAttrib(szFileSysPath, FST_ATTRIB_READONLY, uiAttrib & FST_ATTRIB_READONLY);
    if(FST_STA_OK != iRet) DBG_ERR("iRet = %d (READONLY)\r\n", iRet);

    iRet = FileSys_SetAttrib(szFileSysPath, FST_ATTRIB_HIDDEN, uiAttrib & FST_ATTRIB_HIDDEN);
    if(FST_STA_OK != iRet) DBG_ERR("iRet = %d (HIDDEN)\r\n", iRet);

    iRet = FileSys_SetAttrib(szFileSysPath, FST_ATTRIB_SYSTEM, uiAttrib & FST_ATTRIB_SYSTEM);
    if(FST_STA_OK != iRet) DBG_ERR("iRet = %d (SYSTEM)\r\n", iRet);

    return iRet;
}

static char* cyg_fs_getcwd( void )
{
    return fs_getcwd();
}

static int cyg_fs_chdir( const char *path, bool bIsAbsPath )
{
    CHAR    szFileSysPath[KFS_LONGNAME_PATH_MAX_LENG];
    INT32   iRet;

    if(FALSE == MakeFileSysPath(szFileSysPath, path, bIsAbsPath))
    {
        DBG_ERR("MakeFileSysPath failed, path = %s, bIsAbsPath = %d\r\n", path, bIsAbsPath);
        return FST_STA_PARAM_ERR;
    }

    DBG_IND("szFileSysPath = %s\r\n", szFileSysPath);

    iRet = fs_chdir(szFileSysPath);

    if(FSS_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_fs_SearchFileOpen( const char *path, bool bIsAbsPath)
{
    FS_SEARCH_HDL   pSrhHDL = NULL;
    FIND_DATA       FindData;
    UINT16          wszFileName[KFS_LONGFILENAME_MAX_LENG];
    CHAR            szFileSysPath[KFS_LONGNAME_PATH_MAX_LENG];

    if(FALSE == MakeFileSysPath(szFileSysPath, path, bIsAbsPath))
    {
        DBG_ERR("MakeFileSysPath failed, path = %s, bIsAbsPath = %d\r\n", path, bIsAbsPath);
        return FST_STA_PARAM_ERR;
    }

    if(szFileSysPath[strlen(szFileSysPath)-1] != '\\')
        strcat(szFileSysPath, "\\");

    DBG_IND("szFileSysPath = %s\r\n", szFileSysPath);

    pSrhHDL = fs_SearchFileOpen(szFileSysPath, &FindData, KFS_HANDLEONLY_SEARCH, wszFileName);

    if(NULL != pSrhHDL)//success
        DBG_IND("hFile = 0x%X\r\n", pSrhHDL);//handle
    else//fail
        DBG_ERR("hFile = NULL\r\n");//NULL handle

    return (int)pSrhHDL;
}

static int cyg_fs_SearchFile( int fd, char *pName, ssize_t len)
{
    FIND_DATA   FindData;
    UINT16      wszFileName[KFS_LONGFILENAME_MAX_LENG];
    char        szFileName[KFS_LONGFILENAME_MAX_LENG];
    UINT32      uiLen;
    INT32       iRet;

    DBG_IND("fd = 0x%X\r\n", fd);

    iRet = fs_SearchFile((FS_SEARCH_HDL)fd, &FindData, KFS_FORWARD_SEARCH, wszFileName);

    if(FSS_OK == iRet)//success
    {
        DBG_IND("iRet = %d\r\n", iRet);
    }
    else if(FSS_NO_MORE_FILES == iRet)//reach end
    {
        DBG_IND("iRet = %d\r\n", iRet);
        *(char *)pName = '\0';
        return iRet;
    }
    else//fail
    {
        DBG_ERR("iRet = %d\r\n", iRet);
        *(char *)pName = '\0';
        return iRet;
    }

    //here we found an valid entry

    if(0 == *wszFileName)
    {//only short filename
        strcpy(szFileName, FindData.filename);
    }
    else
    {//long filename
        WideCharToMultiByte(szFileName, wszFileName);
    }

    uiLen = strlen(szFileName);
    DBG_IND("szFileName = %s, uiLen = %d\r\n", szFileName, uiLen);

    if(uiLen > (UINT32)len)
    {
        DBG_ERR("Got len (%d) > Requested len (%d)\r\n", uiLen, len);
        szFileName[len] = '\0';//set to the smaller requested len
    }

    strcpy((char*)pName, szFileName);

    return iRet;
}

static int cyg_fs_SearchFileClose( int fd )
{
    INT32 iRet;

    DBG_IND("Close handle = 0x%X\r\n", fd);

    iRet = fs_SearchFileClose((FS_SEARCH_HDL)fd);

    if(FSS_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_fs_SearchFileRewind( int fd )
{
    INT32 iRet;

    DBG_IND("Rewind handle = 0x%X\r\n", fd);

    iRet = fs_SearchFileRewind((FS_SEARCH_HDL)fd);

    if(FSS_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}
