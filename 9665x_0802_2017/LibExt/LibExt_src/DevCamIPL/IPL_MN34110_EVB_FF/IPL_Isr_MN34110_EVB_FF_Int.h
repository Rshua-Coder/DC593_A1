#ifndef IPL_ISR_MN34110_H_
#define IPL_ISR_MN34110_H_
/**
    IPL_IsrSample_Int.h


    @file       IPL_IsrSample_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

extern void IPL_IsrSIECB(IPL_PROC_ID Id, UINT32 IntStatus,UINT32 idx);
extern void IPL_IsrIFECB(IPL_PROC_ID Id, UINT32 IntStatus,UINT32 idx);
extern void IPL_IsrIPECB(IPL_PROC_ID Id, UINT32 IntStatus,UINT32 idx);
extern void IPL_IsrIMECB(IPL_PROC_ID Id, UINT32 IntStatus,UINT32 idx);

#endif //IPL_ISR_SAMPLE_H_