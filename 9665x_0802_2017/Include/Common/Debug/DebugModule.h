/**
    Module debug message

    Use this debug message suit for show debug message with module tag. User can
    operate filter to select message you want.

    @file       DebugModule.h
    @ingroup    mISYSDebug
    @note

    Usage:
    @code
    [module.c]
    #define __MODULE__    MyLib
    #define __DBGLVL__    1 // 0=OFF, 1=ERROR, 2=TRACE
    #define __DBGFLT__    "*" // *=All, [mark1]=CustomClass
    #include "DebugModule.h"

    func()
    {
        DBG_ERR("My msg\r\n");
        DBG_MSG("My msg\r\n");
    }

    __MODULE__ = your module name (C Language Symbol)


    __DBGLVL__ = 0 : OFF mode, show nothing

    __DBGLVL__ = 1 : ERROR mode, show err, wrn

    __DBGLVL__ = 2 : TRACE mode, show err, wrn, ind, msg and func
                     and ind, msg and func can be filtering by __DBGFLT__ settings

    __DBGFLT__ = "*" : see all messages

    __DBGFLT__ = "[mark1]" : see only CustomClass [mark1] messages

    __DBGFLT__ = "*[mark1]" : see general  messages & CustomClass [mark1] messages



    NOTE:
    -----------------------------------------------------------
    API             __DBGLVL__=0  __DBGLVL__=1  __DBGLVL__=2
    -----------------------------------------------------------
    DBG_FATAL()           v             v             v
    -----------------------------------------------------------
    DBG_ERR()             -             v             v
    -----------------------------------------------------------
    DBG_WRN()             -             v             v
    -----------------------------------------------------------
    DBG_DUMP()            -             v             v
    -----------------------------------------------------------
    DBG_IND()             -             -             v(*)
    -----------------------------------------------------------
    DBG_MSG()             -             -             v(*)
    -----------------------------------------------------------
    DBG_FUNC_BEGIN()      -             -             v(*)
    DBG_FUNC_CHK()
    DBG_FUNC_END()
    -----------------------------------------------------------
    DBG_D()               -             -             v
    DBG_X()
    DBG_S()
    DBG_P()
    DBG_EOL()
    -----------------------------------------------------------
     v : effective
    (*): support filtering by __DBGFLT__ settings
    ERR,WRN,IND,FUNC: output additional FILE, FUNC, LINE info

    //TM - TIMEMARK: record to global timing record
    Usage:

    [module.c]
    #define __MODULE__    MyLib
    #define __PERFTM__    1 //1:Enable TIMEMARK,0:Disable TIMEMARK
    #include "DebugModule.h"

    func()
    {
        TM_BEGIN(name);
        TimerDelay(100);
        TM_END(name);

        TM_DUMP(MyLib); //dump all TM result of MyLib
    }

    Output:
    name    11104510
    name    12110270

    //BM - BENCHMARK: record to local benchmark record
    Usage:

    [module.h]
    BM_VAL_DECLARE(name);

    [module.c]
    #define __MODULE__    MyLib
    #define __PERFBM__    1 //1:Enable BENCHMARK,0:Disable BENCHMARK
    #include "DebugModule.h"
    BM_VAL_DEFINE(name);

    func()
    {
        BM_BEGIN(name);
        TimerDelay(100);
        BM_END(name);
        BM_BEGIN(name2);
        TimerDelay(200);
        BM_END(name2);

        BM_DUMP_HEADER();
        BM_DUMP(name); //dump BM:name result of MyLib
        BM_DUMP(name2); //dump BM:name result of MyLib
    }

    Output:
    >Total-Time,   Count,  Avg-Time, [Module::Func]
    >------------------------------------------------------------
    >  1005.040,       1,   100.040, [MyLib::name]
    >  1005.038,       1,   100.038, [MyLib::name2]
    @endcode

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef __DEBUG_MODULE__
#define __DEBUG_MODULE__

#include "SysKer.h"
#include "Debug.h"
#include "Perf.h"
#include <string.h>
#include <stdlib.h>

#define DBG_SETTINGS  1

#ifndef __MODULE__
#define __MODULE__ __SHORT_FILE__
#endif

#ifndef __DBGLVL__
#undef DBG_SETTINGS
#define DBG_SETTINGS  0
#endif

#ifndef __DBGFLT__
#undef DBG_SETTINGS
#define DBG_SETTINGS  0
#endif

#if (DBG_SETTINGS==0)
#warning Please define your __MODULE__,__DBGLVL__ and __DBGFLT__ before include "DebugModule.h"
#warning Ex:
#warning #define __MODULE__    MyLib
#warning #define __DBGLVL__    1 //0=OFF, 1=ERROR, 2=TRACE
#warning #define __DBGFLT__    "*" //*=All, [mark1]=CustomClass
#warning include "DebugModule.h"
#error (see above)
#endif

#define _TOKEN_STR(a)  # a
#define TOKEN_STR(a)  _TOKEN_STR(a)
#define _TOKEN_BIND(a, b)  a ## b
#define TOKEN_BIND(a, b)  _TOKEN_BIND(a, b)

#ifdef __DBGMONO__
#define DBG_COLOR_RED ""
#define DBG_COLOR_BLUE ""
#define DBG_COLOR_YELLOW ""
#else
#define DBG_COLOR_RED "^R"
#define DBG_COLOR_BLUE "^B"
#define DBG_COLOR_YELLOW "^Y"
#endif

/**
    @def DBG_FUNC_BEGIN(fmtstr)
    @brief Show function begin

    @def DBG_FUNC_CHK(fmtstr)
    @brief Show function check

    @def DBG_FUNC_END(fmtstr)
    @brief Show function end

    @def DBG_FATAL(fmtstr)
    @brief Show error message

    @def DBG_ERR(fmtstr)
    @brief Show function begin

    @def DBG_WRN(fmtstr)
    @brief Show warning message

    @def DBG_IND(fmtstr)
    @brief Show indiction message

    @def DBG_DUMP(fmtstr)
    @brief Show message without moudle tag in any __DBGLVL__

    @def DBG_MSG(fmtstr)
    @brief Show message without module tag when __DBGLVL__ > 1

    @def DBG_D(fmtstr)
    @brief Show a variable decimal value

    @def DBG_X(fmtstr)
    @brief Show a variable hex value

    @def DBG_S(fmtstr)
    @brief Show a variable string value

    @def DBG_P(fmtstr)
    @brief Show a variable hex value with 4 chars format

    @def DBG_EOL(fmtstr)
    @brief Show a change line tag

*/
#if (__DBGLVL__>=2)  //LEVEL = TRACE

#define DBG_FUNC_BEGIN(fmtstr) debug_msg_filter(__DBGFLT__, DBG_COLOR_BLUE "%s():begin {%s/0x%08x} " fmtstr, \
    __func__, __TASK__, __CALL__)

#define DBG_FUNC_CHK(fmtstr) debug_msg_filter(__DBGFLT__, DBG_COLOR_BLUE "%s():chk " fmtstr, \
    __func__)

#define DBG_FUNC_END(fmtstr) debug_msg_filter(__DBGFLT__, DBG_COLOR_BLUE "%s():end " fmtstr, \
    __func__)

#define DBG_FATAL(fmtstr, args...) do{ debug_msg(DBG_COLOR_RED "FATAL:%s() " fmtstr, \
    __func__, ##args); exit(EXIT_FAILURE); } while(0)

#define DBG_ERR(fmtstr, args...) debug_msg(DBG_COLOR_RED "ERR:%s() " fmtstr, \
    __func__, ##args)

#define DBG_WRN(fmtstr, args...) debug_msg(DBG_COLOR_YELLOW "WRN:%s() " fmtstr, \
    __func__, ##args)

#define DBG_IND(fmtstr, args...) debug_msg_filter(__DBGFLT__, "%s(): " fmtstr, \
    __func__, ##args)

#define DBG_DUMP(fmtstr, args...) debug_msg(fmtstr, ##args)

#define DBG_MSG(fmtstr, args...) debug_msg_filter(__DBGFLT__, fmtstr, ##args)

#define DBG_D(var) debug_msg("%s=%d ", #var, (int)(var))
#define DBG_X(var) debug_msg("%s=%x ", #var, (int)(var))
#define DBG_S(var) debug_msg("%s=%s ", #var, (char*)(var))
#define DBG_P(var) debug_msg("%s=%08x ", #var, (void*)(var))
#define DBG_EOL() debug_msg("\r\n") ///<

#elif (__DBGLVL__==1)  //LEVEL = ERROR

#define DBG_FUNC_BEGIN(fmtstr) do{}while(0)

#define DBG_FUNC_CHK(fmtstr) do{}while(0)

#define DBG_FUNC_END(fmtstr) do{}while(0)

#define DBG_FATAL(fmtstr, args...) do{ debug_msg(DBG_COLOR_RED "FATAL:%s() " fmtstr, \
    __func__, ##args); exit(EXIT_FAILURE); } while(0)

#define DBG_ERR(fmtstr, args...) debug_msg(DBG_COLOR_RED "ERR:%s() " fmtstr, \
    __func__, ##args)

#define DBG_WRN(fmtstr, args...) debug_msg(DBG_COLOR_YELLOW "WRN:%s() " fmtstr, \
    __func__, ##args)

#define DBG_IND(fmtstr, args...) do{}while(0)

#define DBG_DUMP(fmtstr, args...) debug_msg(fmtstr, ##args)

#define DBG_MSG(fmtstr, args...) do{}while(0)

#define DBG_D(var) do{}while(0)
#define DBG_X(var) do{}while(0)
#define DBG_S(var) do{}while(0)
#define DBG_P(var) do{}while(0)
#define DBG_EOL() do{}while(0)

#else //(__DBGLVL__==0)  //LEVEL = OFF

#define DBG_FUNC_BEGIN(fmtstr) do{}while(0)

#define DBG_FUNC_CHK(fmtstr) do{}while(0)

#define DBG_FUNC_END(fmtstr) do{}while(0)

#define DBG_FATAL(fmtstr, args...) do{ debug_msg(DBG_COLOR_RED "FATAL:%s() " fmtstr, \
    __func__, ##args); exit(EXIT_FAILURE); } while(0)

#define DBG_ERR(fmtstr, args...) do{}while(0)

#define DBG_WRN(fmtstr, args...) do{}while(0)

#define DBG_IND(fmtstr, args...) do{}while(0)

#define DBG_DUMP(fmtstr, args...) do{}while(0)

#define DBG_MSG(fmtstr, args...) do{}while(0)

#define DBG_D(var) do{}while(0)
#define DBG_X(var) do{}while(0)
#define DBG_S(var) do{}while(0)
#define DBG_P(var) do{}while(0)
#define DBG_EOL() do{}while(0)

#endif


#ifndef NDEBUG //build debug

#define __assert2(e,func) \
    ((void)debug_msg(DBG_COLOR_RED "ASSERT FAIL:%s() %s\n", func,e), abort(), 0)
#define ASSERT(e) \
    ((void) ((e) ? 0 : __assert2 (#e, __func__)))

#else  //build release

#define ASSERT(ignore) ((void)0)

#endif



#ifdef __PERFTM__
#if (__PERFTM__>=1)  //LEVEL = ENABLE
#define TM_BEGIN(name)  \
    TM_Begin(TOKEN_STR(__MODULE__), TOKEN_STR(name))

#define TM_END(name) \
    TM_End(TOKEN_STR(__MODULE__), TOKEN_STR(name))

#define TM_CLEAR() \
    TM_Clear()

#define TM_DUMP(module) \
    TM_Dump(TOKEN_STR(module))

#else //(__PERFTM__==0)  //LEVEL = DISABLE

#define TM_BEGIN(name)
#define TM_END(name)
#define TM_CLEAR()
#define TM_DUMP(module)

#endif
#else

#define TM_BEGIN(name)
#define TM_END(name)
#define TM_CLEAR()
#define TM_DUMP(module)

#endif

#ifdef __PERFBM__
#if (__PERFBM__>=1)  //LEVEL = ENABLE
#define BM_VAL_DEFINE(name) \
    BENCHMARK TOKEN_BIND( BM_, name) = { TOKEN_STR(__MODULE__), TOKEN_STR(name), 0, 0, 0}

#define BM_BEGIN(name) \
    BM_Begin( & TOKEN_BIND( BM_, name))

#define BM_END(name) \
    BM_End( & TOKEN_BIND( BM_, name))

#define BM_CLEAR(name) \
    BM_Clear( & TOKEN_BIND( BM_, name))

#define BM_VAL_DECLARE(name) \
    extern BENCHMARK TOKEN_BIND( BM_, name)

#define BM_DUMP_HEADER() \
    BM_Dump_Header()

#define BM_DUMP(name) \
    BM_Dump( & TOKEN_BIND( BM_, name))

#else //(__PERFBM__==0)  //LEVEL = DISABLE

#define BM_VAL_DEFINE(name)
#define BM_BEGIN(name)
#define BM_END(name)
#define BM_CLEAR(name)
#define BM_VAL_DECLARE(name)
#define BM_DUMP_HEADER()
#define BM_DUMP(name)

#endif
#else

#define BM_VAL_DEFINE(name)
#define BM_BEGIN(name)
#define BM_END(name)
#define BM_CLEAR(name)
#define BM_VAL_DECLARE(name)
#define BM_DUMP_HEADER()
#define BM_DUMP(name)

#endif

#endif //__DEBUG_MODULE__

