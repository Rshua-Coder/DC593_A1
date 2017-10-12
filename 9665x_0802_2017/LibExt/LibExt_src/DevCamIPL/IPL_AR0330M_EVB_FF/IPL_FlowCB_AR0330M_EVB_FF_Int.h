#ifndef IPL_FLOWCB_AR0330M_EVB_FF_INT_H_
#define IPL_FLOWCB_AR0330M_EVB_FF_INT_H_
/*
    IPL flow Callback Function

    This file is the API of the flow callback function.

    @file       IPL_FlowCBAR0330M_EVB_FF.h
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

/**
     IPL flow callback function
*/

/**
     prepare cap AE function

     @note (Optional)

     @param[in] IPL Id
*/
void IPL_FCB_PrepareCapAE(IPL_PROC_ID Id);

/**
     preflash flow function

     @note (Optional)

     @param[in] IPL Id
*/
void IPL_FCB_PreFlash(IPL_PROC_ID Id);

/**
     capture flow(Sensor -> SIE -> Dram:bayer pattern)

     @note (Optional)

     @param[in] IPL Id
     @param[in] SIE settings
*/
void IPL_FCB_GetCapRaw(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);


/**
     set capture AWB gain

     @note (Optional)

     @param[in] IPL Id
*/
void IPL_FCB_SetCapAWB(IPL_PROC_ID Id, IPC_SIE_Info *Info);

/**
     Alg Information CB
*/
void IPL_FCB_Alg3DNR(UINT32 SelIdx, UINT32 Value);
void IPL_FCB_AlgGDC(UINT32 SelIdx, UINT32 Value);
void IPL_FCB_AlgDIS(UINT32 SelIdx, UINT32 Value);
void IPL_FCB_AlgIE(UINT32 SelIdx, UINT32 Value);
void IPL_FCB_AlgRSC(UINT32 SelIdx, UINT32 Value);
void IPL_FCB_AlgWDR(UINT32 SelIdx, UINT32 Value);
#endif //IPL_FLOWCB_AR0330M_EVB_FF_INT_H_
