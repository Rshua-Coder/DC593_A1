/**
    Auto white balance api.


    @file       awb_api.h
    @ingroup    mILibAWBAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AWB_API_H_
#define _AWB_API_H_

#include "IPL_AlgInfor.h"
typedef struct {

    UINT32 RGain;
    UINT32 GGain;
    UINT32 BGain;
    UINT32 ColorTemperature;
} AWBStatus;

extern void AWB_GetStatus(UINT32 Id, AWBStatus *pAWBStatus);
/**

     awb init function
*/
extern void AWB_Init(UINT32 CurrentStatus);

/**

     awb process function
*/
extern void AWB_Process(UINT32 CurrentStatus);


/**

     awb get current gain.

     @note

     @param[out] R gain
     @param[out] G gain
     @param[out] B gain
*/
extern void AWB_GetColorGian(UINT32 *Rg,UINT32 *Gg,UINT32 *Bg);


/**

     awb write debug information

     @note

     @param[in] buffer start address
     @return
         - @b buffer end address
*/
extern UINT32 AWB_WriteDebugInfo(UINT32 Addr);

/**
    MWB Customer Process
*/
extern ER MWB_CalcCustomerColorGain(SEL_WBMODE mode, UINT32 *RGain,UINT32 *GGain,UINT32 *BGain);
extern void MWB_WriteColorGain(SEL_WBMODE mode, UINT32 Rg,UINT32 Gg,UINT32 Bg);
extern void MWB_ReadColorGain(SEL_WBMODE mode, UINT32 *Rg,UINT32 *Gg,UINT32 *Bg);
#endif //_AWB_API_H_
