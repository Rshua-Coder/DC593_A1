#ifndef _GENDATEIMPRINTDATA_H
#define _GENDATEIMPRINTDATA_H

/** \addtogroup Unknown*/
//@{
#include "ImgCapCBMsg.h"

void UiDateImprint_SetEnable(BOOL bEnable);
void UiDateImprint_UpdateDate(void);
BOOL UiDateImprint_GenData(IMG_CAP_DATASTAMP_INFO* pInfo);
void UiDateImprint_InitBuff(void);
#endif
