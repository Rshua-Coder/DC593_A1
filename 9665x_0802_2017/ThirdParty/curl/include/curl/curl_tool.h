#ifndef HEADER_CURL_TOOL_MYMAIN_H
#define HEADER_CURL_TOOL_MYMAIN_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2012, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------
typedef unsigned long       CURL_U32;                ///<  Unsigned 32 bits data type
typedef unsigned char       CURL_U8;                 ///<  Unsigned 8 bits data type

#define CURL_IPC_TOKEN_PATH    "curl"

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((CURL_U32)(CURL_U8)(ch0) | ((CURL_U32)(CURL_U8)(ch1) << 8) | ((CURL_U32)(CURL_U8)(ch2) << 16) | ((CURL_U32)(CURL_U8)(ch3) << 24 ))
#endif
#define CURL_IPCKEYID   MAKEFOURCC('C','U','R','L')


/**
    IPC message type.

*/
typedef enum _CURL_IPC_MSG_TYPE{
    CURL_IPC_MSG_TYPE_S2C = 1,         ///<   server to client message
    CURL_IPC_MSG_TYPE_C2S = 2,         ///<   client to server message
}CURL_IPC_MSG_TYPE;

/**
    IPC message command ID.


*/
typedef enum {

    CURL_IPC_CMDID_STARTED,
    CURL_IPC_CMDID_RUNCMD,
    CURL_IPC_CMDID_RUNCMD_ACK,
    CURL_IPC_CMDID_READ_CB,           ///<   read callback  (cpu2-> cpu1)
    CURL_IPC_CMDID_READ_CB_ACK,       ///<
    CURL_IPC_CMDID_WRITE_CB,          ///<   write callback  (cpu2-> cpu1)
    CURL_IPC_CMDID_WRITE_CB_ACK,      ///<

    CURL_IPC_CMDID_CLOSE = 0x80,      ///<   close    (cpu1-> cpu2)
    CURL_IPC_CMDID_CLOSE_ACK,         ///<   ack of CURL_IPC_CMDID_CLOSE
    CURL_IPC_CMDID_CLOSE_FINISH,


}CURL_IPC_CMD;

/**
    General IPC message.
*/
typedef struct _CURL_IPC_MSG{
    long       mtype;                  ///<   CURL_IPC_MSG_TYPE
    CURL_U32   CmdId;                  ///<   CURL_IPC_CMD
    CURL_U32   ShareMem;               ///<   share memory address
}CURL_IPC_MSG;



#define CURL_IPC_MSGSZ          (sizeof(CURL_IPC_MSG))


#ifdef __cplusplus
extern "C"
{
#endif
/**
    Start curl.

    @param[in] cmd:  Init command for curl.
      -s         maximum client session number


*/
extern int curl_open(char* cmd);

extern int curl_close(void);

/*
cmd
threadpriority

*/

extern int curl_cmd(char *cmd);


#ifdef __cplusplus
} //extern "C"
#endif

#endif /* HEADER_CURL_TOOL_MYMAIN_H */

