/**
    DMA/CPU Configuration and Driver Initail

    Configure DMA/CPU Speed.

    @file       DxSys.h
    @ingroup    mDXSYS

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DXDSP_H_
#define _DXDSP_H_

#include "Type.h"
#include "SxCmd.h"

/**
    @addtogroup mDXSYS
*/
//@{

/**
     Preare initail.

     Here has to call by _main() at first line to prepare inital hardware.
*/
extern void DxSys_Entry(void);

/**
     Check DRAM size

     Check project DRAM setting match DRAM configuration.

     @note Here has to call by _main() before calling DxSys_Init().

     @param[in] dramTotal DRAM size by byte.

     @code
     {
        DxSys_Validate(MEM_DRAM_SIZE);
     }
     @endcode
*/
extern void DxSys_Validate(UINT32 dramTotal);

/**
     Initialize system

     Initialize essential HW.

     @note Here already call by GxSystem at system startup.
*/
extern void DxSys_Init(void);

/**
     Exit system

     At last step of power off flow, call it to power off.

     @note Here already call by GxSystem at system power off flow.
*/
extern void DxSys_Exit(void); //exit driver

/**
     @name Engine clock id
     @note eng_id for DxSys_GetCfgClk()
*/
//@{
#define CFG_GRPH_CLK  1 ///< graph clock
#define CFG_GRPH2_CLK 2 ///< graph-22 clock
#define CFG_ISE_CLK   3 ///< ise clock
#define CFG_IME_CLK   4 ///< ime clock
#define CFG_JPEG_CLK  5 ///< jpeg clock
#define CFG_FDE_CLK   6 ///< fde clock
//@}

/**
     @name CPU speed id
     @note speed_id for DxSys_ChangeCPUSpeed()
*/
//@{
#define CPUSPEED_LOW        0 ///< speed low
#define CPUSPEED_NORMAL     1 ///< speed normal
#define CPUSPEED_HIGH       2 ///< speed high
//@}

/**
     @name DMA speed id
     @note speed_id for DxSys_ChangeDMASpeed() and DxSys_DumpAndVerifyClk()
*/
//@{
#define DMASPEED_LOW        0 ///< speed low
#define DMASPEED_NORMAL     1 ///< speed normal
#define DMASPEED_HIGH       2 ///< speed high
//@}

/**
     Configure DMA/CPU speed

     User needs to implement this function at DspCfg.c. During system startup,
     GxSystem will call it to initialize DMA/CPU speed.
*/
extern void DxSys_CfgClk(void);

/**
     Get engine speed clock

     User needs to implement this function at DspCfg.c to provide a getting information.

     @param[in] uiEngId Engine clock id, refer to CFG_???_CLK
     @return speed PLL_CLKSEL_???
*/
extern UINT32 DxSys_GetCfgClk(UINT32 uiEngId);

/**
     Change CPU speed

     Change current the CPU speed

     @param[in] speed_id CPUSPEED_???
*/
extern void DxSys_ChangeCPUSpeed(UINT32 uiSpeedId);

/**
     Change DMA speed

     Change current the DMA speed

     @param[in] speed_id DMASPEED_???
*/
extern void DxSys_ChangeDMASpeed(UINT32 uiSpeedId);

/**
     Get DMA speed

     Get current the DMA speed

     @return speed_id DMASPEED_???
*/
extern UINT32 DxSys_GetDMASpeed(void);

/**
     Get CPU speed

     Get current the CPU speed

     @return speed_id CPUSPEED_???
*/
extern UINT32 DxSys_GetCPUSpeed(void);

/**
     Dump CPU, OCP, APB and DMA clock

     Show CPU, OCP, APB and DMA clock rate on UART
*/
extern void DxSys_DumpClk(void);

/**
     Confirm CPU, OCP, APB and DMA clock

     Confirm CPU, OCP, APB and DMA clock rate is the same with user setting.

     @note use in DxSys_CfgClk user implement to confirm the clock you want.

     param[in] uiCPUClk_user CPU clock by MHz
     param[in] uiOCPClk_user OCP clock by MHz
     param[in] uiAPBClk_user APB clock by MHz
     param[in] uiSpeedId DMA Speed by DMASPEED_?, Only is support High and Low currently
*/
extern void DxSys_DumpAndVerifyClk(UINT32 uiCPUClk_user, UINT32 uiOCPClk_user, UINT32 uiAPBClk_user, UINT32 uiSpeedId);

/**
    Dump all engine clock

    Show current all engines clock on UART
*/
extern void DxSys_DumpEngineClk(void);

/**
    Dump all engine dma priority

    Show current all engines dma priority on UART
*/
extern void DxSys_DumpEngineDma(void);

/**
    Dump engine info of display

    Show engine info of display

     param[in] id   device id
*/
extern void DxSys_DumpDispEngineInfo(UINT32 id);

/**
     Dump data status of display.

     Dump data status of display.
     @note Nothing
*/
extern UINT32 DxSys_DumpDispDataStatus(UINT32* pData);

/**
    Start watch dog

    Enable watch dog, if any system crash is occured, the system will reset.
*/
extern void DxSys_WDT_Start(void);

/**
    get watch dog reset counts

    The counts of the system reseted.
*/
extern UINT32 DxSys_WDT_GetResetCount(void);

/**
    Reset system

    Use watch dog to reset system right away.
*/
extern void DxSys_WDT_Reset(void);

/**
     exception information

     information during exception. used in fp_exceptionCB tbl.
*/
typedef enum
{
    EXP_CAUSE=0x0,                          ///< Exception cause
    EXP_PC,                                 ///< Exception program counter
    EXP_SRSCTL,                             ///< Shadow register number
    EXP_ISR_FLAG,                           ///< Program is in xxx_isr(module hook on driver ISR)
    EXP_INFO_NUM,
    ENUM_DUMMY4WORD(EXP_INFO)
} EXP_INFO;

/**
     Exception callback function table

     The structure is used by DxSys_RegExpHandler to notify the exception to user
*/
typedef struct _EXP_HANDLER
{
    void (*fp_exceptionCB)(UINT32 *tbl); ///< exception inform callback
    void (*fp_stackoverflowCB)(UINT32 tcb);///< stack overflow callback
}
EXP_HANDLER,*PEXP_HANDLER;

/**
     Register exception handler

     Register exception callback function table

     @param[in] h clock id, refer to @ref EXP_HANDLER
*/
extern void DxSys_RegExpHandler(EXP_HANDLER* h);

/**
     Dump capture driver/h.w setting

     Project can always call it, it takes care dump information activitily.
*/
extern void DxSys_DumpCaptureDrvInfo(void);

/**
    Install SxCmd

    Install uart command to SxCmd
*/
extern void DxSys_InstallCmd(void);

//@}
#endif //_DXDSP_H_
