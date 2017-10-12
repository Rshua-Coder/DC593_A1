/**
    @file       NvtAppUsbEvent.h
    @ingroup    mINvtSystem

    @brief      NVT system events for USB.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef NVT_USB_EVENT_H
#define NVT_USB_EVENT_H

#include "NVTEvent.h"

/**
    @addtogroup mINvtSystem
*/
//@{

/**
    The event of print status.
*/
typedef enum _NVT_USBPRINT_EVENT
{
    NVTEVT_CB_PRINT_INITDONE = APP_USBPRINT_EVENT_BASE,///< Init OK.
    NVTEVT_CB_PRINT_INITFAILED,                        ///< Init failed.
    NVTEVT_CB_PRINT_OK,                                ///< Print OK.
    NVTEVT_CB_PRINT_FAIL,                              ///< Print failed.
    NVTEVT_CB_PRINT_NOINK,                             ///< No ink.
    NVTEVT_CB_PRINT_PAPEROUT,                          ///< Paper out.
    NVTEVT_CB_PRINT_CLOSESESSION,                      ///< Close session.
    NVTEVT_CB_PRINT_NOTIFYJOB,                         ///< Notify Job.
    NVTEVT_CB_PRINT_RESULTS                            ///< Print result.
}NVT_USBPRINT_EVENT;

/**
    The event of print result.
*/
typedef enum _PRINT_RESULTS_SET
{
    PRINT_RESULTS_PRINT_OK = 0,
    PRINT_RESULTS_PRINT_WARN,
    PRINT_RESULTS_PRINT_ERROR,
    PRINT_RESULTS_PAPER_WARN,
    PRINT_RESULTS_PAPER_ERROR,
    PRINT_RESULTS_INK_WARN,
    PRINT_RESULTS_INK_ERROR,
    PRINT_RESULTS_HW_WARN,
    PRINT_RESULTS_HW_ERROR,
    PRINT_RESULTS_FILE_WARN,
    PRINT_RESULTS_FILE_ERROR,
    PRINT_RESULTS_PAPER_EMPTY, //11
    PRINT_RESULTS_PAPER_JAM,
    PRINT_RESULTS_PAPER_LOAD,
    PRINT_RESULTS_PAPER_NEAR_EMPTY,
    PRINT_RESULTS_PAPER_NOT_SUPPORT,
    PRINT_RESULTS_INK_LOW,
    PRINT_RESULTS_INK_EMPTY,
    PRINT_RESULTS_PTR_ERROR,
    PRINT_RESULTS_PTR_UNAVAILABLE,
    PRINT_RESULTS_PTR_OPEN,
    PRINT_RESULTS_PTR_HEAD,//21
    PRINT_RESULTS_PTR_INK,
    PRINT_RESULTS_PTR_CART,
    PRINT_RESULTS_PRINT_QUANTITY,
    PRINT_RESULTS_PRT_CLOSEESSION,
    PRINT_RESULTS_MAX
}PRINT_RESULTS_SET;

//@}
#endif //NVT_USB_EVENT_H
