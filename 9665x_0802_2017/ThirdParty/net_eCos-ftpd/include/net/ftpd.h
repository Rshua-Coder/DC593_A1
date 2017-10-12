#ifndef FTPD_H
#define FTPD_H
#ifdef __cplusplus
extern "C" {
#endif

#include <pkgconf/net_ftpserver.h>
#include <limits.h>
#include "cyg/kernel/kapi.h"

typedef int (*check_password_t)(const char *, const char *); /* 0 for OK, non-null for error */

typedef enum
{
    FTPD_OP_NONE,

    FTPD_OP_RETR_START              = 0x100,
    FTPD_OP_RETR_PROGRESS,
    FTPD_OP_RETR_END,
    FTPD_OP_RETR_ERROR,

    FTPD_OP_STOR_START              = 0x200,
    FTPD_OP_STOR_PROGRESS,
    FTPD_OP_STOR_END,
    FTPD_OP_STOR_ERROR,

    FTPD_OP_APPE_START              = 0x300,
    FTPD_OP_APPE_PROGRESS,
    FTPD_OP_APPE_END,
    FTPD_OP_APPE_ERROR,


    FTPD_OP_UNDEFINED = 0x80000000
} FTPD_OP_ENUM;

/*
    ftpd_event_cb_t

    @param[in] FTPD_OP_ENUM     Operation
    @param[in] off64_t          current completed length
    @param[in] off64_t          expected total length (only valid when RETR)

    @return void
 */
typedef void (*ftpd_event_cb_t)(FTPD_OP_ENUM, off64_t, off64_t);

typedef struct ftp_server {
    /* Configurable part, memset to 0 and then configure */
    unsigned int port;

    unsigned int max_nr_of_clients;
    unsigned int nr_of_clients;
    unsigned int allow_guest;
    unsigned int firstport; /* first and last ports to use, if we're */
    unsigned int lastport;  /* packet filter friendly. */

    check_password_t check_pwd;
    ftpd_event_cb_t event_cb;

    char chrootdir[PATH_MAX + 1];   /* root of the visible filesystem */

    int thread_prio; /* Thread priority */
    unsigned int sockBufSize;       /* overwite sock option SO_SNDBUF */

    /* Internal data, don't touch */
    cyg_mutex_t mutex;
    struct ftp_session *active_sessions;
    volatile int stopping;
    volatile int running;

} ftp_server_t;


int ftpd_server( ftp_server_t * );
int ftpd_server_stop ( ftp_server_t * );

#ifdef __cplusplus
}
#endif
#endif
