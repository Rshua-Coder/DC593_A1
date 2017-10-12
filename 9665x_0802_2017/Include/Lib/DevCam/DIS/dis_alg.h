/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2015.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: DIS Library Module                                                  *
* Description:                                                             *
* Author: Connie Yu                                                         *
****************************************************************************/

/**
    DIS lib

    Sample module detailed description.

    @file       dis_alg.h
    @ingroup    mILibDIS
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

/** \addtogroup DISLib */
//@{

#ifndef _DIS_ALG_H_
#define _DIS_ALG_H_

#include "dis_alg_fw.h"

extern void dis_initialize(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void dis_process(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void dis_end(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern UINT32 dis_getTriggerStatus(void);
extern UINT32 dis_GetPrvMaxBuffer(void);
extern void dis_setDisInfor(DISALG_PARAM *disInfo);
extern void dis_roi_setInputInfo(DIS_ROIGMV_IN* roiIn);
extern DIS_ROIGMV_OUT dis_roi_getResult(void);

#endif

