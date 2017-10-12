/**
    Flash light Algorithm.

    

    @file       flash_Alg.h
    @ingroup    mILibFlashLightAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _FLASH_ALG_H_
#define _FLASH_ALG_H_

typedef struct FLASH_RETURN_INFO_STRUCT
{
    UINT32 BackLum;
    UINT32 PreflashLum;
    UINT32 CaptureBackLum;
    UINT32 CaptureFlashCompensateLum;
} FLASH_RETURN_INFO;


typedef struct FLASH_INPUTPARAM_STRUCT
{
    UINT32 PreviewExpoTime;
    UINT32 CaptureExpoTime;
    UINT16 *BackGroundBlockY;
    UINT16 *PeflashBlockY;
    UINT32 TargetY;
    UINT32 PreviewBinning;
    UINT32 CaptureBinning;
    UINT32 *WeightingTable;
    UINT32 *ExpectYTbale;
    UINT32 *PreflashTbale;
    UINT32 *FlashCurveTbale;
    UINT32 CurveTbl_RowNum;
    UINT32 CurveTbl_ColNum;
    UINT32 BlockWinX;
    UINT32 BlockWinY;
    FLASH_RETURN_INFO FlashReturnInfo;
} FLASH_INPUTPARAM;

/**

     get main flash width (us)

     @note

     @param[in/out] Flash alogrithm information.
     @return
         - @b main flash width.
*/
extern UINT32 flash_AEConverge(FLASH_INPUTPARAM *InputParam);

#endif //_FLASH_ALG_H_
