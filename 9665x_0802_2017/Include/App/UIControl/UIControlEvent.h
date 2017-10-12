/**
    @file       UIControlEvent.h
    @ingroup    mIUIControl

    @brief      Declare events of UI control

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.
*/

#ifndef UICONTROL_EVENT_H
#define UICONTROL_EVENT_H

#include "NVTEvent.h"


/**
    @addtogroup mIUIControl
*/
//@{

/**
     Get event if key event type.

     @param[in] e  Event value.

     @return
        - @b TRUE event is key event type
        - @b FALSE event is not key event type
*/
#define IS_KEY_EVENT(e)                 IS_TYPE_EVENT(e, DEV_KEY_EVENT_BASE)

/**
     Get event if touch event type.

     @param[in] e  Event value.

     @return
        - @b TRUE event is touch event type
        - @b FALSE event is not touch event type
*/
#define IS_TOUCH_EVENT(e)               IS_TYPE_EVENT(e, DEV_TOUCH_EVENT_BASE)

/**
     Get event if touch event type.

     @param[in] e     Event value.
     @param[in] emin  Event minimum value.
     @param[in] emax  Event maximum value.

     @return
        - @b TRUE event is between minimun and maximun
        - @b FALSE event is out of range
*/
#define IN_RANGE_EVENT(e,emin,emax)     (((e) > (emin)) && ((e) < (emax)))

/**
    UI Control event values
*/
enum
{
    NVTEVT_OPEN_WINDOW      = 128, ///< Open window command
    NVTEVT_CLOSE_WINDOW,           ///< Close window command
    NVTEVT_CLOSE_WINDOW_CLEAR,     ///< Close window command with clear screen
    NVTEVT_CHILD_CLOSE,            ///< Close child window command
    NVTEVT_REDRAW,                 ///< Redraw screen command
    NVTEVT_BEGIN_CTRL,             ///< Begin control command
    NVTEVT_END_CTRL,               ///< End control command
    NVTEVT_BACKGROUND_DONE,        ///< Background done command
    NVTEVT_OUT_RANGE,              ///< Current event out of control rectangle,for touch panel event
    NVTEVT_SYSCMD_MAX              ///< Max values of system event. nvtusercommand adds new event after this value.
    //MAX = 138
};


//@}
#endif
