#ifndef _IPL_CMD_GC2023P_TVP5150_CARDV_FFFF_INT_H_
#define _IPL_CMD_GC2023P_TVP5150_CARDV_FFFF_INT_H_
/**
    IPL_CmdIMX322LQJ_TVP5150_CARDV_FFFF_Int.h


    @file       IPL_CmdIMX322LQJ_TVP5150_CARDV_FFFF_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

/**
    user add extend set command at here. transfer data must be smaller than IPL_SET_CMD_DATA
*/
typedef enum
{
    IPL_EXT_SET_MAX_CNT = IPL_SET_MAX_CNT_DEF,
    ENUM_DUMMY4WORD(IPL_EXT_SET_CMD)
} IPL_EXT_SET_CMD;

//check IPL_EXT_SET_CMD range
STATIC_ASSERT((IPL_EXT_SET_MAX_CNT <= IPL_SET_CMD_SWITCH_TAB));

/**
    user add extend get command at here.
*/
typedef enum
{
    IPL_EXT_GET_MAX_CNT = IPL_GET_MAX_CNT_DEF,
    ENUM_DUMMY4WORD(IPL_EXT_GET_CMD)
} IPL_EXT_GET_CMD;

//check IPL_EXT_GET_CMD range
STATIC_ASSERT((IPL_EXT_GET_MAX_CNT <= IPL_GET_CMD_SWITCH_TAB));



#endif //_IPL_CMDIMX322LQJ_TVP5150_CARDV_FFFF_INT_H_
