///////////////////////////////////////////////////////////////////
/**
    LogFile module.

    The module supports to store the uart log messages to a stoarge(ex: SD card).

    @file       LogFile.h
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

*/
#ifndef _LOGFILE_H
#define _LOGFILE_H

#define LOGFILE_ROOT_DIR_MAX_LEN        32         ///<  LogFile root directory maximum length
#define LOGFILE_MAX_FILENUM             32         ///<  LogFile maximum file number
#define LOGFILE_DEFAULT_MAX_FILENUM     32         ///<  LogFile default maximum file number
#define LOGFILE_MAX_FILESIZE            0x200000   ///<  LogFile maximum file size of each file
#define LOGFILE_MIN_FILESIZE            0x004000   ///<  LogFile minimun file size of each file
#define LOGFILE_DEFAULT_MAX_FILESIZE    0x100000   ///<  LogFile default maximum file size of each file

/**
   Warning Code.

   Any warning code occured will display on uart or return.
*/
typedef enum {
    LOGFILE_WR_OK                =   0, ///< no warning
    LOGFILE_WR_ALREADY_OPEN      =  -1, ///< LogFile_Open called twice
    LOGFILE_WR_NOT_OPEN          =  -2, ///< LogFile_Open not to call before.
    ENUM_DUMMY4WORD(LOGFILE_WR)
}LOGFILE_WR;

/**
   Console output type.

   Any warning code occured will display on uart or return.
*/
typedef enum {
    LOGFILE_CON_UART_AND_STORE   =   0, ///< console output to uart and storage.
    LOGFILE_CON_STORE_ONLY,             ///< console just output to storage.
    LOGFILE_CON_NONE,                   ///< console intput/output none.
    LOGFILE_CON_MAX = LOGFILE_CON_NONE,
    ENUM_DUMMY4WORD(LOGFILE_CON)
}LOGFILE_CON;

/**
    Init parameters for LogFile module.

    This structure is used in LogFile_Open() to specify the LogFile module needed information for normal working.
*/
typedef struct
{
    CHAR                    rootDir[LOGFILE_ROOT_DIR_MAX_LEN+1];       ///<  the root directory path of log files
    UINT32                  maxFileNum;                                ///<  the maximum log files number that can keep in storage, can not exceeds LOGFILE_MAX_FILENUM.
                                                                       ///<  0 means use default, the default value is LOGFILE_DEFAULT_MAX_FILENUM.
    UINT32                  maxFileSize;                               ///<  the maximum filesize of each log file, can not exceeds LOGFILE_MAX_FILESIZE.
                                                                       ///<  0 means use default, the default value is LOGFILE_DEFAULT_MAX_FILESIZE.
    BOOL                    isPreAllocAllFiles;                        ///<  if pre-alloc all log files when log file folder is emtpy.
} LOGFILE_OPEN;


/**
     Register and set LogFile console.

*/
extern void       LogFile_SetConsole(LOGFILE_CON  ConType);

/**
     Install LogFile flag and semaphore id.

*/
extern void       LogFile_InstallID(void) _SECTION(".kercfg_text");

/**
    Open LogFile task.

    This is the LogFile task initial function.
    Before calling this API, need to call LogFile_SetConsole firstly.

    @param[in] pOpen:  Init info for LogFile.

    @return
        - @b E_OK: initial success.
        - @b E_PAR: the initial parameter has some error.

    Example:
    @code
    {
        LOGFILE_OPEN    logOpenParm={0};
        UINT32          maxFileNum = 10;
        UINT32          maxFileSize = 0x40000; // 256KB
        CHAR            rootDir[LOGFILE_ROOT_DIR_MAX_LEN+1]="A:\\Novatek\\LOG\\";

        logOpenParm.maxFileNum = maxFileNum;
        logOpenParm.maxFileSize = maxFileSize;
        strncpy(logOpenParm.rootDir,rootDir,LOGFILE_ROOT_DIR_MAX_LEN);
        LogFile_Open(&logOpenParm);
    }
    @endcode

*/
extern ER         LogFile_Open(LOGFILE_OPEN* pOpen);


/**
    Close LogFile task.

    @return
        - @b E_OK: Close success.

*/
extern ER         LogFile_Close(void);

/**
    Re-Open LogFile task.

    Keep the original settings and re-open LogFile task.
    Before calling this API, need to call LogFile_Open firstly.
    When user format the SD card need to call LogFile_ReOpen() to notify logfile re-open.

    @param[in] pOpen:  Init info for LogFile.

    @return
        - @b E_OK: initial success.
        - @b E_PAR: the initial parameter has some error.

*/
extern ER         LogFile_ReOpen(void);
/**
    Suspend LogFile write file operation.

    @return
        - @b E_OK: Suspend success.

*/
extern ER         LogFile_Suspend(void);

/**
    Resume LogFile write file operation.

    @return
        - @b E_OK: Resume success.

*/
extern ER         LogFile_Resume(void);

/**
    Dump some internal variables and status of LogFile module for debug using.

    @return None.
*/
extern void       LogFile_DumpDebugInfo(void);

#endif //_LOGFILE_H
