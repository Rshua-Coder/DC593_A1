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

    @file       Dis_alg_fw.h
    @ingroup    mILibDIS
    @note       DIS FW process.

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

/** \addtogroup DISLib */
//@{

#ifndef _DIS_ALG_FW_H_
#define _DIS_ALG_FW_H_

#include "dis_lib.h"


//#define MDIS_PRV_MAX_SIZE    2304* 1296 * 2 * 2 ///< MAXSIZE*1.44*FMT_422*2
#define DISLIB_VECTOR_NORM      12      ///< output vector norm factor(2^12)
#define DIS_ABS(x)                  ((x)>=0?(x):(-(x)))

/**
    DIS Motion Vector.

    Structure of directional motion vector elements.
*/
//@{
typedef struct _DISALG_MOTION_VECTOR
{
    INT32 iX; ///< x component
    INT32 iY; ///< y component
} DISALG_MOTION_VECTOR;
//@}

/**
    DIS ALG Entire Configuration

    Structure of DIS ALG parameters when one wants to configure this module.
*/
//@{
typedef struct _DISALG_PARAM_STRUCT
{
    UINT32 InSizeH;                     ///< input horizontal pixels
    UINT32 InSizeV;                     ///< input vertical pixels
    UINT32 InLineOfs;                   ///< input line offset
    UINT32 InAdd0;                      ///< input starting address 0
    UINT32 InAdd1;                      ///< input starting address 1
    UINT32 InAdd2;                      ///< input starting address 2
    UINT16 frame_cnt;                   ///< frame count
} DISALG_PARAM;
//@}

/**
    DIS ALG result Vector information

    Structure of DIS ALG result vector information.
*/
//@{
typedef struct _DISALG_VECTOR_STRUCT
{
    DISALG_MOTION_VECTOR vector;///< result vector
    UINT16 frame_cnt;           ///< frame count
    UINT32 score;               ///< 1: useable, 0: not sure
} DISALG_VECTOR;
//@}

/**
    DIS Global Motion Information

    Structure of DIS module parameters when calculating global motion.
*/
//@{
typedef struct DIS_GM_STRUCT
{
    MOTION_VECTOR     *pImv;    ///< integral motion vector
    COMPENSATION_INFO *pComp;   ///< compensation information
    DIS_MVSCORE_LEVEL ScoreLv;  ///< Level of MV score screening
    DIS_STICKY_LEVEL  StickyLv; ///< Stickiness level of compensation
    MOTION_INFOR *pMV;          ///< Motion verctors
} DIS_GM;
//@}

#if 0
/**
    Motion in tracking.

    Structure of information of a moving object.
*/
//@{
typedef struct _TRACK_MOTION_INFOR
{
    INT32   iTrkX;         ///< x component
    INT32   iTrkY;         ///< y component
    UINT32  uiPosX;        ///< left
    UINT32  uiPosY;        ///< top
    BOOL    bMovObj;       ///< if it's a moving object
} TRACK_MOTION_INFOR;
//@}
#endif

/**
    DIS Global Motion Vector in ROI

    Structure of input coordinate and size.
*/
//@{
typedef struct _DIS_ROIGMV_IN_STRUCT
{
    UINT8 uiStartX;              ///< input x coordinate, 0~100%
    UINT8 uiStartY;              ///< input y coordinate, 0~100%
    UINT8 uiSizeX;               ///< input width, 0~100%
    UINT8 uiSizeY;               ///< input height, 0~100%
} DIS_ROIGMV_IN;
//@}

/**
    DIS Global Motion Vector in ROI

    Structure of output global motion vector.
*/
//@{
typedef struct _DIS_ROIGMV_OUT_STRUCT
{
    DISALG_MOTION_VECTOR vector; ///< output result vector
    BOOL  bValidVec;             ///< if the result vector is valid
} DIS_ROIGMV_OUT;
//@}

/*
extern void dis_initialize(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void dis_process(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void dis_end(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern UINT32 dis_getTriggerStatus(void);
*/
extern void disFw_initialize(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void disFw_process(MEM_RANGE *buf, MEM_RANGE *cachebuf, MOTION_INFOR *pMV);
extern void disFw_end(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void disFw_setDisInfor(DISALG_PARAM *disInfo);
extern UINT32 disFw_GetPrvMaxBuffer(void);
extern void disFw_accumUpdateProcess(MOTION_INFOR *pMV);
extern UINT16 dis_getDisViewRatio(void);
extern void dis_setDisViewRatio(UINT16 ratio);
//extern DISALG_MOTION_VECTOR dis_getIdeCorrVec(void);
extern ER dis_getFrameCorrVec(DISALG_VECTOR *Corr, UINT16 frame_cnt);
extern VOID dis_getOisDetMotVec(DIS_SGM *pIV);
extern UINT32 dis_accessOisOnOff(DIS_ACCESS_TYPE AccType, UINT32 uiIsOisOn);
extern UINT32 dis_setOisDetMotVec(INT32 iX, INT32 iY);
extern DIS_ROIGMV_OUT disFw_getRoiMotVec(DIS_ROIGMV_IN *pRoiIn, MOTION_INFOR *MV);

#endif

