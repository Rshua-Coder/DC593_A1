/**
    Auto Exposure api.


    @file       ae_api.h
    @ingroup    mILibAEAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AE_API_H_
#define _AE_API_H_
#include "IPL_AlgInfor.h"


/**
     AE process event.
     @note
*/
typedef struct _AE_EVENT_TAB
{
    //AE_Process event
    UINT32 (*AeProcessEnd)(UINT32 ISO, UINT32 ExposureTime);

}AE_EVENT_TAB;



/**

     load event table

     @note

     @param[in] event table(AE_EVENT_TAB)
     @return
         - @b void.
*/
extern void AE_LoadEventTAB(AE_EVENT_TAB *EventTab);

/**

     ae init function
*/
extern void AE_Init (UINT32 CurrentStatus);


/**

     ae process function
     @note

     @param[in] current status
     @return
         - @b void.
*/
extern void AE_Process (UINT32 CurrentStatus);
/**

     ae Suspend function
     @note

     @param[in] current status
     @return
         - @b void.
*/
extern void AE_Suspend (UINT32 CurrentStatus);
/**

     ae get preview ae arg.

     @note

     @param[out] iso
     @param[out] exposure time
     @param[out] iris
*/
extern void AE_GetPrvAEArg(UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris);

/**

     ae get capture ae arg.

     @note

     @param[out] iso
     @param[out] exposure time
     @param[out] iris
*/
extern void AE_GetCapAEArg(UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris);


/**

     ae wait stable

     @note

     @param[in] stable count
*/
extern void AE_WaitStable(UINT32 count);

/**

     ae write debug information

     @note

     @param[in] buffer start address
     @return
         - @b buffer end address
*/
extern UINT32 AE_WriteDebugInfo(UINT32 Addr);

/**

     get histogram

     @note

     @param[in] buffer address
*/
extern void AE_GetHisto(UINT32 *Hist);


/**
    get preview ae ev value
    @note

    @return
        - @b EV value : environment value*10(ex: LV5=50, LV6=60 ...)
*/
extern UINT32 AE_GetPrvEVValue(void);

extern UINT32 AE_GetRealEVValue(void);

/**
    get capture ae ev value
    @note

    @return
        - @b EV value : environment value*10(ex: LV5=50, LV6=60 ...)
*/
extern UINT32 AE_GetCapEVValue(void);

/**

     start anti-flicker detection alg
*/
extern void AE_StartAFD(void);

/**
     ae wait preview stable
     @note

     @param[in] StableCnt : wait stable count
     @param[in] TimeOutFrame : time out frame
     @return
         - @b TRUE: ae already stable
         - @b FALSE: ae not stable and time out
*/
extern BOOL AE_Wait_Stable(UINT32 StableCnt, UINT32 TimeOutFrame);

/**

     ae get capture ae arg.

     @note

     @param[out] iso
     @param[out] exposure time (us)
     @param[out] Aperture : Current Fno (*10)
     @param[out] MaxApture : Max Fno (*10)
*/
void AE_GetExifInfo(UINT32 *ISO, UINT32 *ExpT, UINT32 *Aperture, UINT32 *MaxApture);


/**
     Get lum ratio form AEB steps
     @note

     @param[in] AEB_Step : EV steps for adjusting capture lum
     @return
         - @b EV adjust ratio
*/
extern UINT32 AEB_GetRatioFromEV(SEL_AEEV AEB_Step);

/**
     Set EV adjust ratio to capture
     @note

     @param[in] Ratio : EV adjust ratio
     @return
         - @b none
*/
extern void AEB_SetRatio2Cap(UINT32 Ratio);


/**
     Get current raw luminmance
     @note

     @param[in] LumBit : return data's bit
     @return
         - @b Luminmance : The average lum of raw image
*/
extern UINT32 AE_getCurLum(UINT32 LumBit);

extern void AE_set3DNRDbg(UINT32 level);

#endif //_AE_API_H_
