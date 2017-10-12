#ifndef CYGONCE_NET_HFS_H
#define CYGONCE_NET_HFS_H
/* =================================================================
 *
 *      hfs.h
 *
 *      A simple http file server.
 *
 * =================================================================
 */

#include <stdio.h>

//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------
typedef cyg_uint32       HFS_U32;                ///<  Unsigned 32 bits data type
typedef unsigned char    HFS_U8;                 ///<  Unsigned 8 bits data type

/**
     @name HFS notify status
*/
//@{
#define CYG_HFS_STATUS_CLIENT_REQUEST           0   ///<  client has request comimg in
#define CYG_HFS_STATUS_SERVER_RESPONSE_START    1   ///<  server send response data start
#define CYG_HFS_STATUS_SERVER_RESPONSE_END      2   ///<  server send response data end
#define CYG_HFS_STATUS_CLIENT_DISCONNECT        3   ///<  client disconnect.
#define CYG_HFS_STATUS_SERVER_EXIT              4   ///<  notify that the server is exit
//@}

/**
     @name HFS get custom data return value
*/
//@{
#define CYG_HFS_CB_GETDATA_RETURN_ERROR        -1   ///<  has error
#define CYG_HFS_CB_GETDATA_RETURN_OK           0    ///<  ok get all data
#define CYG_HFS_CB_GETDATA_RETURN_CONTINUE     1    ///<  has more data need to get

#define CYG_HFS_CB_GETDATA_SEGMENT_ERROR_BREAK         0xFFFFFFFF    ///<  hfs error break, the connection may be closed

//@}


/**
     @name HFS list dir return value
*/
//@{
#define CYG_HFS_LIST_DIR_OPEN_FAIL             -1   ///<  has error when open dir
#define CYG_HFS_LIST_DIR_OK                    0    ///<  ok get all data
#define CYG_HFS_LIST_DIR_CONTINUE              1    ///<  has more data need to list
//@}


/**
     @name HFS upload file result
*/
//@{
#define CYG_HFS_UPLOAD_OK                      0    ///<  upload file ok
#define CYG_HFS_UPLOAD_FAIL_FILE_EXIST         -1   ///<  upload file fail because of file exist
#define CYG_HFS_UPLOAD_FAIL_RECEIVE_ERROR      -2   ///<  receive data has some error
#define CYG_HFS_UPLOAD_FAIL_WRITE_ERROR        -3   ///<  write file has some error
#define CYG_HFS_UPLOAD_FAIL_FILENAME_EMPTY     -4   ///<  file name is emtpy
#define CYG_HFS_UPLOAD_FAIL_MD5_CHK_FAIL       -5   ///<  upload file MD5 check fail
//@}

/**
     @name HFS download file result
*/
//@{
#define CYG_HFS_DOWNLOAD_OK                      0    ///<  download file ok
#define CYG_HFS_DOWNLOAD_FILE_NOT_EXIST         -1    ///<  download file not exist
#define CYG_HFS_DOWNLOAD_FAIL                   -2    ///<  download file fail
//@}

/**
     @name HFS delete file result
*/
//@{
#define CYG_HFS_DELETE_OK                      0    ///<  delete file ok
#define CYG_HFS_DELETE_FAIL                    -1   ///<  delete file fail
//@}


/**
     @name HFS maximum length
*/
//@{
#define CYG_HFS_ROOT_DIR_MAXLEN                64   ///<  root dir max path length
#define CYG_HFS_KEY_MAXLEN                     64   ///<  key max length
#define CYG_HFS_NAME_MAXLEN                    32   ///<  user name max length
#define CYG_HFS_PWD_MAXLEN                     32   ///<  user passwd max length
#define CYG_HFS_REQUEST_PATH_MAXLEN            128  ///<  request url path max length
#define CYG_HFS_FILE_PATH_MAXLEN               128  ///<  file path max length
#define CYG_HFS_MIMETYPE_MAXLEN                40   ///<  mime type max length
#define CYG_HFS_USER_QUERY_MAXLEN              24   ///<  client query string max length
#define CYG_HFS_CUSTOM_STR_MAXLEN              24   // custom string max length
//@}

/**
    HFS put data status.

*/
typedef enum _HFS_PUT_STATUS{
    HFS_PUT_STATUS_CONTINUE               =   0, ///< still have data need to put
    HFS_PUT_STATUS_FINISH                 =   1, ///< put data finish
    HFS_PUT_STATUS_ERR                    =  -1, ///< some error happened
}HFS_PUT_STATUS;


/**
     @name HFS header callback return result
*/
//@{
#define CYG_HFS_CB_HEADER_RETURN_DEFAULT       0    ///<  the http request need to be handle by hfs internal
#define CYG_HFS_CB_HEADER_RETURN_CUSTOM        1    ///<  the http request is handled by customer
#define CYG_HFS_CB_HEADER_RETURN_REDIRECT      2    ///<  the http request need to redirect
#define CYG_HFS_CB_HEADER_RETURN_ERROR        -1    ///<  the http request has some error
//@}

typedef void cyg_hfs_notify(int status);
typedef int  cyg_hfs_getCustomData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
typedef int  cyg_hfs_check_password(const char *username, const char *password, char *key,char* questionstr);
typedef int  cyg_hfs_gen_dirlist_html(const char *path, cyg_uint32 bufAddr, cyg_uint32* bufSize, const char* usr, const char* pwd, cyg_uint32 segmentCount);
typedef int  cyg_hfs_client_query(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
typedef int  cyg_hfs_upload_result_cb(int result, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType);
typedef int  cyg_hfs_delete_result_cb(int result, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType);
typedef int  cyg_hfs_putCustomData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32 bufSize, cyg_uint32 segmentCount, HFS_PUT_STATUS putStatus);
typedef int  cyg_hfs_header_cb(cyg_uint32 headerAddr, cyg_uint32 headerSize, char* filepath, char* mimeType, void *reserved);  ///< Callback function for http header parse custom.
typedef void cyg_hfs_download_result_cb(int result, char* path);///< Callback function for download command complete


typedef struct
{
    cyg_hfs_notify          *notify;        // notify the status
    cyg_hfs_getCustomData   *getCustomData; // get custom data
    cyg_hfs_check_password  *check_pwd;     // check password callback function
    int                     portNum;        // server port number
    int                     threadPriority; // server thread priority
    char                    rootdir[CYG_HFS_ROOT_DIR_MAXLEN + 1];   /* root of the visible filesystem */
    char                    key[CYG_HFS_KEY_MAXLEN+1];
    int                     sockbufSize;    // socket buffer size
    int                     tos;            // type of service
    char                    clientQueryStr[CYG_HFS_USER_QUERY_MAXLEN]; // client query string
    cyg_hfs_client_query    *clientQueryCb; // client query callback function
    int                     timeoutCnt;     // timeout counter for send & receive , time base is 0
    cyg_hfs_upload_result_cb *uploadResultCb;
    cyg_hfs_delete_result_cb *deleteResultCb;
    int                     isSupportHttps;                              ///<  if want to support https
    int                     httpsPortNum;
    cyg_hfs_putCustomData   *putCustomData; // put custom data
    char                    customStr[CYG_HFS_CUSTOM_STR_MAXLEN+1];  ///<  custom string
    cyg_hfs_header_cb       *headerCb;                                   ///<  Callback function for header data parse by customer
    cyg_hfs_download_result_cb *downloadResultCb;                        ///<  Callback function for download command complete
    cyg_hfs_gen_dirlist_html *genDirListHtml;
    HFS_U32                 maxClientNum;                                ///<  maximum client connection at the same time
} cyg_hfs_open_obj;

__externC void cyg_hfs_open(cyg_hfs_open_obj*  pObj);         // Start hfs server

__externC void cyg_hfs_close(void);                           // Stop hfs server



/* ----------------------------------------------------------------- */
#endif /* CYGONCE_NET_HFS_H                                          */
/* end of hfs.h                                                      */

