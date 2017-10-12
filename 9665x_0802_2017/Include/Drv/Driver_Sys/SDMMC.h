/**
    @file       Sdmmc.h
    @ingroup    mIStgSDIO

    @brief      SD/MMC Card driver header

    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

*/

/*
    @addtogroup mIStgSDIO
@{
*/

#ifndef _SDMMC_H
#define _SDMMC_H

#include "StrgDef.h"
//#NT#2007/04/17#Jerry Ko - Begin
//#Added for separate controller and card define
// Memory Card Structure definition
#include "SdmmcDesc.h"
//#NT#2007/04/17#Jerry Ko - End    

//#NT#2008/04/22#HH Chuang -begin
//#NT#detect card by call back routine installed from upper layer
typedef BOOL (*SDMMC_CARD_DETECT_CB)(void);
//#NT#2008/04/22#HH Chuang -end

//------------------------------------------------------------
// The general api for the SDMMC device driver
//------------------------------------------------------------
//#NT#2007/03/15#Ethan Wang -begin
extern PSTRG_TAB    sdmmc_getStorageObject(void);
//#NT#2007/03/15#Ethan Wang -end

// -- Card detect functions --
extern UINT32   sdmmc_getCardExist(void);
extern UINT32   sdmmc_getCardWriteProt(void);
//#NT#2008/04/22#HH Chuang -begin
//#NT#detect card by call back routine installed from upper layer
extern void     sdmmc_setDetectCardExistHdl(SDMMC_CARD_DETECT_CB pHdl);
extern void     sdmmc_setDetectCardProtectHdl(SDMMC_CARD_DETECT_CB pHdl);
//#NT#2008/04/22#HH Chuang -end

// -- Get card register functions --
extern UINT32               sdmmc_getOCRRegister(void);
extern PSD_CID_STRUCT       sdmmc_getCIDRegister(void);
extern PMMC_CID_STRUCT      sdmmc_getCIDRegisterMMC(void);
extern PSDMMC_CSD_STRUCT    sdmmc_getCSDRegister(void);
//#NT#2007/02/12#Ethan Wang -begin
extern PSD_SCR_STRUCT       sdmmc_getSCRRegister(void);
extern PSD_STATUS_STRUCT    sdmmc_getSSRRegister(void);
//#NT#2007/02/12#Ethan Wang -end

// -- Get card info functions --
extern PSDMMC_INFO_STRUCT   sdmmc_getCardInformation(void);
//#NT#2009/02/05#HH Chuang -begin
//#NT#add printing sd card information
extern void sdmmc_printCardInformation(void);
//#NT#2009/02/05#HH Chuang -end

#endif // _SDMMC_H

/**
@}
*/
