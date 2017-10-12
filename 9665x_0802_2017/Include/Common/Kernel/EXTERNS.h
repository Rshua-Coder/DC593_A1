/*
    Extern variables.

    @file       Externs.h
    @ingroup    mIKernelCore
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _EXTERNS_H
#define _EXTERNS_H

#include "SysCall.h"
#include "Que.h"

#ifdef __ECOS
// eCos system

//-----------------------------------------------------------------------------
// External variables
//-----------------------------------------------------------------------------
// Task
extern const int            gtcbnum;
// Semaphore
extern const int            gsemnum;
// Flag
extern const int            gflgnum;
// Mailbox
extern const int            gmbxnum;
extern const int            gbufnum;
// Fixed memory pool
extern VP                   gfremem;
extern VP                   gendmem;
extern const int            gfplnum;

#else
// uITRON system

//-----------------------------------------------------------------------------
// External variables
//-----------------------------------------------------------------------------
// Scheduling flags
extern volatile union sch   gschflg;

// Time
extern struct dlq           gtsvque;

// Interrupt
extern FP                   ALL_INT_tbl[];
extern const int            gAllINTNums;

// Task
extern struct dcq           grdyque;
extern struct tcb*          gruntcb;
extern struct tcb*          gtcbadr[];
extern struct tcb           gtcbarr[];
extern const int            gtcbnum;

// Semaphore
extern struct sem           gsemarr[];
extern struct sem*          gsemadr[];
extern const int            gsemnum;

// Flag
extern struct flg*          gflgadr[];
extern struct flg           gflgarr[];
extern const int            gflgnum;

// Mailbox
extern struct buf*          gbuffre;
extern struct buf           gbufarr[];
extern struct mbx           gmbxarr[];
extern struct mbx*          gmbxadr[];
extern const int            gmbxnum;
extern const int            gbufnum;

// Fixed memory pool
extern struct fpl           gfplarr[];
extern struct fpl*          gfpladr[];
extern VP                   gfremem;
extern VP                   gendmem;
extern const int            gfplnum;

#endif

#endif
