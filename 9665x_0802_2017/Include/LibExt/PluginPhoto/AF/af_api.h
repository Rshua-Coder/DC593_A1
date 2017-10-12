/**
    Public header file for AF API.

    This file is the header file that define the API and data type for AF.

    @file       af_api.h
    @ingroup    mILibAFAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _AF_LIB_H_
#define _AF_LIB_H_

#include "debug.h"
#include "af_alg.h"
#include "af_task.h"
#include "afdrv_common.h"
#include "IPL_Cmd.h"

/**
     @addtogroup mISYSAlg
*/
//@{


/**
     AF callback message.

*/
typedef enum
{
    AF_CBMSG_AF_START = 0,     ///< user will receive this notification when AF process is started.
    AF_CBMSG_AF_DONE = 1,      ///< user will receive this notification when AF process is end.
    AF_CBMSG_MAX_CNT,          ///< total callback message count
    ENUM_DUMMY4WORD(AF_CBMSG)
} AF_CBMSG;

/**
    AF callback funcion proto type
*/
typedef void (*AF_CBMSG_FP)(AF_CBMSG uiMsgID, void *Data);

/**
     Set AF searching mode.

     @param[in] af_alg_search_mode       AF_ALG_SMODE_FULLSEARCH: full search.
                                         AF_ALG_SMODE_FALLNORMAL: break if VA curve fall down.
     @return
         - @b void.
*/
extern void AF_SetAlgSearchMode(AF_ALG_SEARCH_MODE af_alg_search_mode);

/**
     Get AF searching mode.

     @return Description of data returned.
         - @b AF_ALG_SEARCH_MODE:   AF_ALG_SMODE_FULLSEARCH,
                                    AF_ALG_SMODE_FALLNORMAL
*/
extern AF_ALG_SEARCH_MODE AF_GetAlgSearchMode(void);

/**
     Register callback function.

     @note User must register callback funcion then can receive the callback message notification when AF done.

     @param[in] CB: the callback function pointer.
*/
extern void AF_RegisterCB(AF_CBMSG_FP CB);

/**
     af init function

     @param[in] CurrentStatus       Reserved.
     @return Description of data returned.
         - @b void.
*/
extern void AF_Init(UINT32 CurrentStatus);

/**

     af process function
     @note

     @param[in] current status      Reserved.
     @return
         - @b void.
*/
extern void AF_Process(UINT32 CurrentStatus);

/**
     run af process(for IPL control)

     @param[in] Id          af process id (AF_PROC_ID)
     @param[in] WaitFinish  wait af process finish
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER AF_Run(AF_PROC_ID Id, BOOL WaitFinish);

/**
     release af process(for IPL control)

     @param[in] Id          af process id (AF_PROC_ID)
     @param[in] WaitFinish  wait af release finish
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER AF_Release(AF_PROC_ID Id, BOOL bWaitFinish);

/**

     Setup IPE modue to retrieve VA.
     @note

     @return
         - @b void.
*/
extern void AF_VA_Setup(void);

/**
     Get IPE VA value for specific window.

     @param[in] *G0         Group0 VA value
     @param[in] *G1H        Group1 Horizontal VA value
     @param[in] *G1V        Group1 Vertical VA value
     @param[in] *G2H        Group2 Horizontal VA value
     @param[in] *G2V        Group2 Vertical VA value
     @param[in] WinIdx      VA_WINIDX
                            VA_WIN_16X16 = 0,               ///< va 16 * 16 window
                            VA_WIN_INDP0,                   ///< va independent window 0
                            VA_WIN_INDP1,                   ///< va independent window 1
                            VA_WIN_INDP2,                   ///< va independent window 2

     @return
         - @b void.
*/
extern void AF_GetVA_Result(UINT32 *G0, UINT32 *G1H, UINT32 *G1V, UINT32 *G2H, UINT32 *G2V, VA_WINIDX WinIdx);

/**

     Call this api after setting whole parameters or changing parameters
     @note

     @return
         - @b void.
*/
extern void AF_SetVAparam2IPL(void);

/**
     Get IPE VA value for specific window.

     @param[in] *piStartIdx         Start position of AF searching range.
     @param[in] *piEndIdx           End position of AF searching range.
     @param[in] *piStep             Focus step(s) to be increased each time.
     @param[in] *piOffsetStep       Corresponding valid VA value will be retrieved after OffsetStep time of VD(s).
     @param[in] *piMaxIdx           Focus position of peak point of VA curve.
     @param[in] *puiSelectWin       VA window(s) which found peak point.
     @param[in] *puiStatus          AF result.

     @return
         - @b void.
*/
extern void AF_GetResult(INT16* piStartIdx, INT16* piEndIdx, INT8* piStep, INT16* piOffsetStep, INT16 *piMaxIdx, UINT8 *puiSelectWin, UINT8 *puiStatus);
extern void AF_SetManualRange(INT16 iStartIdx, INT16 iEndIdx, INT8 iStep);
extern INT32 AF_SearchFullRange(INT16 startIdx, INT16 endIdx, UINT8 jumpStep, UINT32 *pMaxVA);

/**

     Write AF debugging info at the end of JPEG file.

     @param[in] Addr         Address to save AF debugging info at the end of JPEG file.

     @return
         - @b void.
*/
extern UINT32 AF_WriteDebugInfo(UINT32 Addr);

//@}
#endif
