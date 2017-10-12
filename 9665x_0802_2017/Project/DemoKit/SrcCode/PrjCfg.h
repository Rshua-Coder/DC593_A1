/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       PrjCfg.h
    @ingroup

    @brief

    @note       Nothing.
*/

#ifndef _PRJCFG_H_
#define _PRJCFG_H_

#include "Type.h"
#include "debug.h"

#include "PrjCfg_Comm.h"
#if (_MODEL_DSC_ == _MODEL_DEMO1_)
#include "PrjCfg_Demo.h"
#elif (_MODEL_DSC_ == _MODEL_EVB_)
#include "PrjCfg_EVB.h"
#elif (_MODEL_DSC_ == _MODEL_HTKW01_)
#include "PrjCfg_HTKW01.h"
#elif (_MODEL_DSC_ == _MODEL_B30W_)
#include "PrjCfg_HTKW01.h"
#elif (_MODEL_DSC_ == _MODEL_HDV02W_)
#include "PrjCfg_HTKW01.h"
#elif (_MODEL_DSC_ == _MODEL_DUAL_NOCABLE_)
#include "PrjCfg_DUAL_NOCABLE.h"
#elif (_MODEL_DSC_ == _MODEL_DUAL_)
#include "PrjCfg_DUAL.h"
#elif (_MODEL_DSC_ == _MODEL_DV658_)
#include "PrjCfg_DV658.h"
#elif (_MODEL_DSC_ == _MODEL_RAPOO_01_)
#include "PrjCfg_RAPOO_01.h"
#elif (_MODEL_DSC_ == _MODEL_WD6_)
#include "PrjCfg_WD6.h"
#elif (_MODEL_DSC_ == _MODEL_IPCAM1_EVB_)
#include "PrjCfg_IPCAM1_EVB.h"
#elif (_MODEL_DSC_ == _MODEL_DUAL_TVP5150_)
#include "PrjCfg_DUAL_TVP5150.h"
#elif (_MODEL_DSC_ == _MODEL_CUBIC_MINI_)
#include "PrjCfg_CUBIC_MINI.h"
#elif (_MODEL_DSC_ == _MODEL_C12_)
#include "PrjCfg_C12.h"
#elif (_MODEL_DSC_ == _MODEL_CARDV_650_)
#include "PrjCfg_CarDV_650.h"
#else //Unknown MODEL
#warning Unknown MODEL?
#warning Please assign your "MODEL" in include "ModelConfig.txt"
#error (see above)
#endif

#endif //_PRJCFG_H_
