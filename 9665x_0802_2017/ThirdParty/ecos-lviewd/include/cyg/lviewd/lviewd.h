#ifndef CYGONCE_NET_LVIEW_LVIEWD_H
#define CYGONCE_NET_LVIEW_LVIEWD_H
/* =================================================================
 *
 *      lviewd.h
 *
 *      A simple live view streaming server
 *
 * =================================================================
 */

#include <pkgconf/system.h>
#include <pkgconf/isoinfra.h>
#include <cyg/hal/hal_tables.h>
#include <stdio.h>

#define CYG_LVIEW_STATUS_CLIENT_REQUEST           0   // client has request comimg in
#define CYG_LVIEW_STATUS_SERVER_RESPONSE_START    1   // server send response live view data start
#define CYG_LVIEW_STATUS_SERVER_RESPONSE_END      2   // server send response live view data end
#define CYG_LVIEW_STATUS_CLIENT_DISCONNECT        3   // client disconnect.


typedef int cyg_lviewd_getJpg(int* jpgAddr, int* jpgSize);
typedef void cyg_lviewd_notify(int status);
typedef cyg_uint32 cyg_lviewd_getCurrTime(void);
typedef void cyg_lviewd_hwmem_memcpy(cyg_uint32 uiDst, cyg_uint32 uiSrc, cyg_uint32 uiSize);

typedef struct
{
    cyg_lviewd_getJpg       *getJpg;        // get jpg function handler
    cyg_lviewd_notify       *notify;        // notify the status
    cyg_lviewd_hwmem_memcpy *hwmem_memcpy;  // Hw memcpy API
    int                     portNum;        // server port number
    int                     threadPriority; // server thread priority
    int                     maxJpgSize;     // support max jpg size
    int                     frameRate;      // live view streaming frame rate, default is 30
    int                     sockbufSize;// socket buffer size
    int                     tos;            // type of service
    void                   *arg;            // reserved for future use
} cyg_lviewd_install_obj;


__externC void cyg_lviewd_install( cyg_lviewd_install_obj*  pObj);  // install some callback function & config some settings

__externC void cyg_lviewd_startup(void); // Start lview daemon

__externC void cyg_lviewd_stop(void);    // Stop lview daemon



/* ----------------------------------------------------------------- */
#endif /* CYGONCE_NET_LVIEW_LVIEWD_H                                  */
/* end of lviewd.h                                                   */

