/**
    IPL Ctrl module(streaming)

    This controller define several operation to control image pipeline.
\n  Run: start IPL in defined mode.
\n  Stop: stop IPL.
\n  RunTimeChg: chg IPL configuration when IPL in Running stage.


    @file       IPL_Ctrl_Int.h
    @ingroup    mIIPL_Ctrl
    @note
\n              preview usage example:
\n              {
\n                  IPL_CTRL_OpenTsk();
\n                  ...
\n                  que = IPL_CTRLQuery(ICF_FLOW_A);
\n                  ...
\n                  IPL_CTRLRun(ICF_FLOW_A,...);
\n                  ...
\n                  IPL_CTRLRunTimeChg(IPC_Chg_ImeP1Out,...);
\n                  IPL_CTRLRunTimeChg(IPC_Chg_ImeP3Out,...);
\n                  IPL_CTRLRunTimeChg(IPC_Chg_Crop,...);
\n                  IPL_CTRLRunTimeChg(IPC_Chg_Load,...);
\n                  ...
\n                  IPL_CTRLRunTimeChg(IPC_SIE_IQparam,...);
\n                  ...
\n                  IPL_CTRLStop(IPC_STOP_ALL);
\n                  ...
\n                  IPL_CTRL_CloseTsk();
\n
\n               }
\n              preview -> capture -> preview usage example:
\n              {
\n                  IPL_CTRL_OpenTsk();
\n                  ...
\n                  que = IPL_CTRLQuery(ICF_FLOW_A);
\n                  ...
\n                  IPL_CTRLRun(ICF_FLOW_A,...);
\n                  ...
\n                  IPL_CTRLStop(IPC_STOP_YCC);
\n                  ...
\n                  IPL_CTRLStop(IPC_PAUSE_SIE);
\n                  ...
\n                  IPL_CTRLRunTimeChg(IPC_SIE_,...);
\n                  IPL_CTRLStop(IPC_RESUME_SIE);
\n                  ...
\n                  IPL_CTRLStop(IPC_STOP_RAW);
\n                  ...
\n                  IPL_CTRLRun(ICF_FLOW_A,...);
\n               }

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _IPL_CTRL_INT_H
#define _IPL_CTRL_INT_H
#include "IPL_Ctrl_flow_Int.h"
#include "IPL_ISREvent_Int.h"
/** \addtogroup mIIPL_Ctrl */
//@{
/**
     IPL_Ctrl information

     query IPL_Ctrl relative information
*/
typedef union _IPC_QINFO
{
    QUE_FLOW_A          QUE_FA;         ///< buffer number needed by flow A
    QUE_FLOW_B          QUE_FB;         ///< buffer number needed by Flow B
    QUE_FLOW_C          QUE_FC;         ///< buffer number needed by Flow C
    QUE_FLOW_D          QUE_FD;         ///< buffer number needed by Flow C
    IPE_CC_SUBOUT       Rdy_Csub;       ///< current ready CC sub-image
    IPE_V_SUBOUT        Rdy_Vsub;       ///< current ready V sub-image
    IPE_ETH_DramRst     Rdy_Eth;        ///< current ready edge threshold output
    PRE_DMA_IN          PreDmaIn_Rdy;   ///< current ready pre input ch0 info
    IPL_YCC_IMG_INFO    CCIR_Rdy;       ///< current ready CCIR output info
    IPE__VA_IND_RST     indVa;
    SIE_CH0PARAM        Rdy_CH0Param;   ///< current ready sie ch0 information
    SIE_CAPARAM         Rdy_CAParam;    ///< current ready ca window information(not statistic data)
    UINT32              FrameCnt;       ///< current frame count
}IPC_QINFO,*PIPC_QINFO;

/**
     IPL_Ctrl configuration

     @configure IPL when ipl_start
*/
typedef union _IPC_INFO
{
    ICF_FLOW_A_STRUCT      ICF_FA;  ///< Flow A
    ICF_FLOW_B_STRUCT      ICF_FB;  ///< Flow B
    ICF_FLOW_C_STRUCT      ICF_FC;  ///< Flow C
    ICF_FLOW_D_STRUCT      ICF_FD;  ///< Flow D
}IPC_INFO,*PIPC_INFO;

/**
     Control IPL start to run
     @note
     IPL can run in 3 modes,plz reference IPLCTRL_FLOW, this api will open and then start IPL engines, and configure
     inner status into "running_status"
\n   IPL must be in all_stop_status when call it.
\n   IPL engines are including : SIE,PRE,IFE,DCE,IPE,IFE2,IME.

     @param[in] flow       start IPL by indicatied flow.
     @param[in] info       info. for IPL,each flow have corresponding data structure
                           ex:
                           fulfill "pInfo->ICF_FA" if flow is "FLOW_A"
     @param[in] Obj        isr notification CB,call back at each engine isr occurred
     @param[in] fpImg      ime frame end notification CB.

     @return Description of data returned.
         - @b E_OK:        IPL start.
         - @b E_SYS:       ICF task not opened or some of IPL engines not close yet, or IPL not in stop_status.
     Example: (Optional)
     @code
     {
        IPC_INFO info;
        info.ICF_FA
        ...

        IPL_CTRL_OpenTsk();
        ...
        IPL_CTRLRun(ICF_FLOW_A,&info,NULL,NULL);
        ...
     }
     @endcode
*/
extern ER IPL_CTRLRun(IPLCTRL_FLOW flow, IPC_INFO* pInfo,IPL_ISR_OBJ_INFO *pObj,fp_ImgRdy fpImg);
/**
     Control IPL start to run2
     @note
     IPL can run in 3 modes,plz reference IPLCTRL_FLOW, this api will open and then start IPL engines, and configure
     inner status into "running_status"
\n   IPL must be in all_stop_status when call it.
\n   IPL engines are including : SIE,PRE,IFE,DCE,IPE,IFE2,IME.

     @param[in] flow       start IPL by indicatied flow.
     @param[in] info       info. for IPL,each flow have corresponding data structure
                           ex:
                           fulfill "pInfo->ICF_FA" if flow is "FLOW_A"
     @param[in] Obj        isr notification CB,call back at each engine isr occurred
     @param[in] fpImg      ime frame end notification CB for display 1.
     @param[in] fpImg2     ime frame end notification CB for display 2.

     @return Description of data returned.
         - @b E_OK:        IPL start.
         - @b E_SYS:       ICF task not opened or some of IPL engines not close yet, or IPL not in stop_status.
     Example: (Optional)
     @code
     {
        IPC_INFO info;
        info.ICF_FA
        ...

        IPL_CTRL_OpenTsk();
        ...
        IPL_CTRLRun(ICF_FLOW_A,&info,NULL,NULL);
        ...
     }
     @endcode
*/
extern ER IPL_CTRLRun2(IPLCTRL_FLOW flow, IPC_INFO* pInfo,IPL_ISR_OBJ_INFO *pObj,fp_ImgRdy fpImg,fp_ImgRdy2 fpImg2);
/**
     Control IPL to stop from running_status
     @note
     pause IPL and close engine from running_status. and uninstall isr notification CB.
\n   detail state mechine plz reference enum:IPC_STOP

     @param[in] mode       stop engine according to choosed mode, detail operation plz reference enum:IPC_STOP

     @return Description of data returned.
         - @b E_OK:    IPL stop.
         - @b E_SYS:   IPL already stopped,or state mechine error.
     Example: (Optional)
     @code
     {
        IPL_CTRLRun(ICF_FLOW_A,&info,NULL,NULL);
        ...
        IPL_CTRLStop(IPC_STOP_ALL);
        ...
        IPL_CTRL_CloseTsk();
     }
     @endcode
*/
extern ER IPL_CTRLStop(IPC_STOP mode);

/**
     get IPL_Ctrl information
     @note
     get IPL_Ctrl information according input enum.
\n   (no association with IPL in run/stop status.)
\n   if flow == ICF_FLOWA, return information in IPC_QINFO->QUE_FA
\n   if flow == QUE_CSUB_Rdy, return information in IPC_QINFO->Rdy_Csub,
     if return address = 0, means that sub-image do not output to dram yet!

     @param[in] event       input which event need to be queried

     @return Description of data returned.
         - @b IPC_QINFO:

     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern IPC_QINFO IPL_CTRLQuery(IPLCTRL_QUERY event);

/**
     Change IPL setting when IPL in running_status
     @note
     change IPL engine during IPL in running status, all Chg will load into engine registor.
     this api will wait until all configuration is latched,excluding
     "IPC_Chg_ImeP1Out","IPC_Chg_ImeP2Out","IPC_Chg_ImeP3Out","IPC_Chg_Crop","IPC_SIE_".
     detail plz reference enum:IPC_RUNTIMECHG !

     @param[in] Chg       input operation
     @param[in] ptr       operation info., data type plz reference enum:IPC_RUNTIMECHG

     @return Description of data returned.
         - @b E_OK:    IPL new configuration is latched
         - @b E_SYS:   IPL is not in running_status or state mechine error!
     Example: (Optional)
     @code
     {
        IPL_CTRL_OpenTsk();
        ...
        IPL_CTRLRun(ICF_FLOW_A,&info,NULL,NULL);
        ...

        IPL_CTRLRunTimeChg(IPC_Chg_ImeP2Out,(void*)&IPC_IMEOut_Info);
        IPL_CTRLRunTimeChg(IPC_Chg_Crop,(void*)&IPC_IMEOut_Info);
        IPL_CTRLRunTimeChg(IPC_Chg_Load,NULL);
        ...
        IPL_CTRLRunTimeChg(IPC_SIE_IQparam,(void*)&IPC_IQparam);
        ...
        IPL_CTRLStop(IPC_STOP_ALL);
        ...
        IPL_CTRL_CloseTsk();
     }
     @endcode
*/
extern ER IPL_CTRLRunTimeChg(IPC_RUNTIMECHG Chg,void* ptr);

/**
     Change IPL IQ setting when IPL in running_status

     @note
     all enum:IPC_RUNTIMECHG_IQ have no timing control.

     @param[in] Chg       input operation
     @param[in] ptr       operation info., data type plz reference enum:IPC_RUNTIMECHG_IQ

     @return Description of data returned.
         - @b E_OK:    IPL new configuration is latched
         - @b E_SYS:   IPL is not in running_status or state mechine error!

     @endcode
*/
extern ER IPL_CTRLRunTimeChg_IQ(IPC_RUNTIMECHG_IQ Chg,void* ptr);

/**
     open icf task

     open and initial ICF_Task, this task is dedicated to control IPL.

     @return Description of data returned.
         - @b E_OK:    Task open and in idle status
         - @b E_SYS:   task is already opened
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void IPL_CTRL_OpenTsk(void);

/**
     close icf task

     un-registor all ipl control operations those are hooked on this task, and close task.

     @return Description of data returned.
         - @b E_OK:    Task close
         - @b E_SYS:   task is already closed
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void IPL_CTRL_CloseTsk(void);



/*
    IPC_D2D_STRIPE callback function
*/
extern void _STRIPE_CB(USIZE *Ref_1x,Coordinate *Ref_1xCen,DCE_SUBFUNC* subF,UINT32 eventID) __attribute__ ((weak));

/*
    for dce stripe table pre-calculation

    @param[in] SrcImg    source image width and height
    @param[in] pImeOut   ptr need to be available if ime is piped into dce d2d operation, otherwise,
\n                       give NULL to this ptr.
    @param[in] eventID   input event.
*/
extern MEM_RANGE* IPC_D2D_STRIPE(USIZE SrcImg,UINT32 eventID);
//@}
#endif