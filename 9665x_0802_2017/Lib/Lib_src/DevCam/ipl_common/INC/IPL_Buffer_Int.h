#ifndef _IPL_BUFFER_INT_H_
#define _IPL_BUFFER_INT_H_
/**
    IPL_Buffer_Int.h


    @file       IPL_Buffer_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "type.h"
#include "ErrorNo.h"
#include "IPL_Utility.h"

#define IPL_BUF_ELEMENT_MAX 128
#define IPL_BUF_NO_USE 0xffffffff

typedef struct
{
    IPL_PROC_ID Id;     ///< ipl process ID
    UINT32 BufAddr;     ///< ipl buffer start address
    UINT32 BufSize;     ///< ipl buffer total size
} IPL_BUF_OBJ_INFO;

/**
     initial buffer

     @note reset all array information.
           when user want to re-assign buffer, it can be executed at change mode flow.

     @param[in] IPL_BUF_OBJ_INFO buffer obj

     @return (ER)
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern ER IPL_BufInit(IPL_BUF_OBJ_INFO *Obj);

/**
     get current total buffer

     note: it can be executed after IPL_BufInit

     @param[in] IPL_PROC_ID ipl process id
     @param[out] UINT32 buffer start address
     @param[out] UINT32 buffer total size

     @return (ER)
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern ER IPL_BufGetResInfo(IPL_PROC_ID Id, UINT32 *BufAddr, UINT32 *BufSize);

/**
     set sub buffer information

     @param[in] IPL_PROC_ID ipl process id
     @param[in] UINT32 buffer idx, depend on IPL lib
     @param[in] UINT32 buffer start address
     @param[in] UINT32 buffer total size
     @param[in] CHAR   buffer name(for debug)
     @return (ER)
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern ER IPL_BufSet(IPL_PROC_ID Id, UINT32 Idx, UINT32 BufAddr, UINT32 BufSize, CHAR *Name);

/**
     get sub buffer information

     @param[in] IPL_PROC_ID ipl process id
     @param[in] UINT32 buffer idx, depend on IPL lib
     @param[out] UINT32 buffer start address
     @param[out] UINT32 buffer total size
     @return (ER)
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern ER IPL_BufGet(IPL_PROC_ID Id, UINT32 Idx, UINT32 *BufAddr, UINT32 *BufSize);

/**
     un-initial buffer

     @note unload buffer information. it can be executed at mode close.

     @param[in] IPL_PROC_ID ipl process ID

     @return (ER)
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern ER IPL_BufUnInit(IPL_PROC_ID Id);

/**
     dump current buffer array information

     @param[in] IPL_PROC_ID ipl process ID

     @return [NONE]
*/
extern void IPL_BufDump(IPL_PROC_ID Id);

#endif //_IPL_BUFFER_INT_H_
