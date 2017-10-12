#ifndef _IPL_CALIBRATION_ALG_H_
#define _IPL_CALIBRATION_ALG_H_
/**
    ipl calibration algorithm


    @file       ipl_calibration_alg.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#include "Type.h"
#include "errorno.h"
#include "IPL_Hal.h"

/** \addtogroup mILibCal */
//@{

/**
    defect pixel concealment data structure
*/
typedef struct _DPC_ALG
{
    IPL_RAW_IMG_INFO    Bayer;          ///< bayer image information
    UINT32              BayField;       ///< field num of current bayer image
    Coordinate          BayAct_Str;     ///< active window starting coordinate of current bayer image
    Coordinate          BayCrop_Str;    ///< crop window starting coordinate of current bayer image

    UINT32 DPAddr;      ///< DRAM address of DP data
    UINT32 Count;       ///< DP counter.(0: start counting at 0, 100: start counting at 100)
    UINT32 MaxCount;    ///< max defect pixel number
    UINT32 Threshold;   ///< DP detect threshold
    UINT32 Block_X;     ///< DP search block X,(must be even,because of bayer fmt)
    UINT32 Block_Y;     ///< DP search block Y,(must be even,because of bayer fmt)
}DPC_ALG;

/**
    defect pixel concealment data structure
*/
typedef struct _DPC_ALG_INFO
{
    DPC_ALG DPC;         ///< -

    ///< skipping area of current bayer image(defect pixel won't be concealed within this area)
    UINT32  SkipXL;      ///< left side
    UINT32  SkipXR;      ///< right side
    UINT32  SkipYU;      ///< up side
    UINT32  SkipYD;      ///< down side
} DPC_ALG_INFO, *pDPC_ALG_INFO;

/**
    calibration algorithm operation structure
    @note plz refer to Cal_AlgEvent()
*/
typedef struct _CAL_ALG_OBJ
{
    UINT32  (*fpDPC_Bayer)(pDPC_ALG_INFO pRaw,BOOL* bOF);///<plz refer to Cal_AlgEvent()
    void    (*fpDPC_Sort)(UINT32* pTbl,UINT32 num);///<plz refer to Cal_AlgEvent()
    UINT32  (*fpDPC_ArgFmt)(UINT32* pTbl,UINT32 num,UINT32* pYcnt);///<plz refer to Cal_AlgEvent()
    //#NT#2013/08/27#Jarkko Chang -begin
    //#NT# add to check ECS gain is overflow or not
    BOOL    (*fpECS_Tbl)(IPL_RAW_IMG_INFO* pRaw,UINT32 CompensationRate,UINT32 Tbladdr,UINT32 WinNumH,UINT32 WinNumV,ECS_BitRatio BitSel);///<plz refer to Cal_AlgEvent()
    //#NT#2013/08/27#Jarkko Chang -end
    void    (*fpSet_ECS_Valid_Table)(UINT32* User_ECSValidTable,UINT32 WinNumH,UINT32 WinNumV, BOOL bValidtable);
    UINT32* (*fpGet_ECS_Y_Vaule)(UINT32 WinNumH,UINT32 WinNumV);
    void    (*fpAWB_AVG)(IPL_RAW_IMG_INFO* pRaw,UINT32* pAvgRg,UINT32* pAvgBg,bitratio BitSel);///<plz refer to Cal_AlgEvent()
}CAL_ALG_OBJ,*pCAL_ALG_OBJ;

/**
     calibration algorithm operation


     @return Description of data returned.
        - @b fpDPC_Bayer:   search defect pixel on bayer image.
\n                      input para:
\n                          pRaw:   image information
\n                          bOF:    TRUE if need to serch whole image even if DP counter is over maximum.
\n                      return:
\n                          the number of defect pixel.
        - @b fpDPC_Sort:    sorting the coordinate of defect pixels which are searched out.
\n                      input para:
\n                          pTbl:   dp coordinate table pointer
\n                          num:    dp number.
        - @b fpDPC_ArgFmt:  arrange DP table format to fit NT96650
\n                      input para:
\n                          pTbl:   dp coordinate table pointer
\n                          num:    dp number.
\n                      output para:
\n                          pYcnt:  line count of dp table after arrangement
\n                      return:
\n                           total table length after arrangement(unit: byte)
        - @b fpECS_Tbl:     calculate color shading table
\n                      input para:
\n                          pRaw:   image information
\n                          CompensationRate:  color shading compensation percentage
\n                          Tbladdr:    ECS table address
\n                          WinNumH:    H size of ecs table
\n                          WinNumV:    V size of ecs table
\n                          BitSel:     bit selection of ecs result
        - @b fpAWB_AVG:  calculate current bayer image R/B gain
\n                      input para:
\n                          pRaw:   image information
\n                          BitSel: bit selection of R/B gain
\n                      output para:
\n                          pAvgRg,pAvgBg:  average R/B gain
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern pCAL_ALG_OBJ Cal_AlgEvent(void);
//@}
#endif
