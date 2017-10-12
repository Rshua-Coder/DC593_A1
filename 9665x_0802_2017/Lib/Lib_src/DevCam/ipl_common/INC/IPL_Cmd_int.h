#ifndef _IPL_CMD_INT_H_
#define _IPL_CMD_INT_H_
/**
    IPL_Cmd_Int.h


    @file       IPL_Cmd_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "type.h"
#include "IPL_Cmd.h"
#include "IPL_Ctrl_Flow_Int.h"
#include "IPL_AlgInfor_Int.h"
#include "Sensor.h"

typedef ER (*IPL_CMD_CHGMODE_FP)(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode);

typedef ER (*IPL_CMD_PROC_FP)(void *Data);

typedef struct
{
    UINT32 TotalCnt;            //total table count
    IPL_CMD_PROC_FP *ProcFp;    //IPL_CMD_PROC_FP array address
} IPL_CMD_INFO;

/**

     IPL_GetCmdSETTab

     @note get command table information

     @return
         - @b IPL_CMD_INFO.
*/
extern IPL_CMD_INFO IPL_GetCmdSETTab(void);

/**

     IPL_GetCmdGETTab

     @note get command table information

     @return
         - @b IPL_CMD_INFO.
*/
extern IPL_CMD_INFO IPL_GetCmdGETTab(void);

/**

     IPL_GetExtCmdSETTab

     @note get extend command table information

     @return
         - @b IPL_CMD_INFO.
*/
extern IPL_CMD_INFO IPL_GetExtCmdSETTab(void) __attribute__ ((weak));

/**

     IPL_GetExtCmdGETTab

     @note get extend command table information

     @return
         - @b IPL_CMD_INFO.
*/
extern IPL_CMD_INFO IPL_GetExtCmdGETTab(void) __attribute__ ((weak));

/**

     IPL_GetChgModeFp

     @note need return  ProcFP

     @return
         - @b IPL_CMD_CHGMODE_FP.
*/
extern IPL_CMD_CHGMODE_FP IPL_ChgModeFCB(IPL_MODE CurMode, IPL_MODE NextMode) __attribute__ ((weak));

/**

     IPL_SetImeFCB

     @note set ime information
     @param[in] IPL_IME_DATA(ime information)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @return
         - @b IPL_IME_PATH(modify path).
*/
extern IPL_IME_PATH IPL_SetImeFCB(IPL_IME_DATA *Info, IPC_IMEOut_Info *IpcInfoP1, IPC_IMEOut_Info *IpcInfoP2, IPC_IMEOut_Info *IpcInfoP3) __attribute__ ((weak));

/**

     IPL_SetDZoomFCB

     @note get digital zoom information
     @param[in] IPL_DZOOM(digital zoom information)
     @param[in/out] IPC_Dzoom_Info(no need assign memory)
     @return
         - @b void.
*/
extern void IPL_SetDZoomFCB(IPL_DZOOM *Info, IPC_Dzoom_Info *SetInfo) __attribute__ ((weak));

/**

     IPL_SetSleepFCB

     @note enter sleep mode
     @param[in] IPL_SLEEP_INFO(enter sleep mode information)
     @return
         - @b void.
*/
extern void IPL_SetSleepFCB(IPL_SLEEP_INFO *Info) __attribute__ ((weak));

/**

     IPL_SetWakeupFCB

     @note exit sleep mode
     @param[in] IPL_WAKEUP_INFO(exit sleep mode information)
     @return
         - @b void.
*/
extern void IPL_SetWakeupFCB(IPL_WAKEUP_INFO *Info) __attribute__ ((weak));

/**
     IPL_GetCapRawFCB

     @note get cap raw flow
     @param[in] IPL_GET_CAP_RAW_DATA
     @return
         - @b void.
*/
extern void IPL_GetCapRawFCB(IPL_GET_CAP_RAW_DATA *Info) __attribute__ ((weak));

/**
     IPL_SetPauseDMAFCB

     @note set pause dma
     @param[in] void
     @return
         - @b void.
*/
extern void IPL_SetPauseDMAFCB(void) __attribute__ ((weak));

/**
     IPL_SetResumeDMAFCB

     @note set resume dma
     @param[in] void
     @return
         - @b void.
*/
extern void IPL_SetResumeDMAFCB(void) __attribute__ ((weak));


/**

     IPL_SetImgRatioFCB

     @note set sensor crop & ime out information
     @param[in] IPL_SET_IMGRATIO_DATA
     @param[out] IPC_FRONTEND(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @return
         - @b IPL_IME_PATH(modify path).
*/
extern IPL_IME_PATH IPL_SetImgRatioFCB(IPL_SET_IMGRATIO_DATA *Info, IPC_FRONTEND *IpcInfo, IPC_IMEOut_Info *IpcInfoP1, IPC_IMEOut_Info *IpcInfoP2, IPC_IMEOut_Info *IpcInfoP3) __attribute__ ((weak));

/**

     IPL_SetVAFCB

     @note set va information
     @param[in] Info                va information
     @param[in, out] HalParaIdx     hal parameter index
     @param[in, out] OutAddr        va output buffer address
     @param[in, out] OutAddr1        va output buffer address
*/
extern void IPL_SetVAFCB(IPL_SET_VA_DATA *Info, IPL_HAL_PARASET *HalParaIdx, UINT32 *OutAddr, UINT32 *OutAddr1)__attribute__ ((weak));

/**

     IPL_GetImeCurInfoFCB

     @note get ime current information
     @param[in] IPL_IME_PATH  path id
     @return
         - @b IPL_YCC_IMG_INFO(address).

*/
extern IPL_YCC_IMG_INFO IPL_GetImeCurInfoFCB(IPL_IME_PATH Id) __attribute__ ((weak));


/**

     IPL_GetIme2CurInfoFCB

     @note get ime current information
     @param[in] IPL_IME_PATH  path id
     @return
         - @b IPL_YCC_IMG_INFO(address).

*/
extern IPL_YCC_IMG_INFO IPL_GetIme2CurInfoFCB(IPL_IME_PATH Id) __attribute__ ((weak));

/**

     IPL_GetImeInfoFCB

     @note get ime information
     @param[in] IPL_IME_PATH  path id
     @param[in] UINT32  ime buffer id
     @return
         - @b IPL_YCC_IMG_INFO(address).

*/
extern IPL_YCC_IMG_INFO IPL_GetImeInfoFCB(IPL_IME_PATH Id, UINT32 BufId) __attribute__ ((weak));

/**

     IPL_GetImeInfo2FCB

     @note get ime information
     @param[in] IPL_IPL_ID ipl id
     @param[in] IPL_IME_PATH  path id
     @param[in] UINT32  ime buffer id
     @return
         - @b IPL_YCC_IMG_INFO(address).

*/
extern IPL_YCC_IMG_INFO IPL_GetIme2InfoFCB(IPL_PROC_ID Ipl_Id, IPL_IME_PATH Id, UINT32 BufId) __attribute__ ((weak));
/**

     IPL_GetIPLInfoFCB

     @note get IPL information
     @param[out] sensor information
     @param[out] ime information
     @param[out] raw information
     @param[out] ime information
     @return
         - @b IPL_YCC_IMG_INFO(address).

*/
extern UINT32 IPL_GetIPLInfoFCB(SENSOR_INFO *SenInfo, IPL_IME_INFOR *ImeInfo, IPL_PRE_RAW_INFO *RawInfo, ImeFuncEn *FuncEn, UINT32 *DisOnOff) __attribute__ ((weak));
#endif //_IPL_CMD_INT_H_

