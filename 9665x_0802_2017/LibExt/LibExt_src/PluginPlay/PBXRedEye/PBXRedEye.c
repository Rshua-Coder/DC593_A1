/**
    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

    @file       PBX_Edit.c
    @ingroup    mISYSPlaySS

    @brief      All function for Playback file handle
                This module can be a plug-in to Application Playback

    @note       Nothing.

    @version    V0.00.001
    @author     Lincy Lin
    @date       2011/12/15
*/

/** \addtogroup mISYSPlaySS */
//@{

#include <string.h>
#include "PBXRedEye.h"
#include "dma.h"
#include "arcsoft_redeye_remove.h"
#include "NvtVerInfo.h"

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          PBXRedEye
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


#define MAX_EYE_NUM   20
static ARER_EYEINFO   eyeInfo[MAX_EYE_NUM] = {0};

NVTVER_VERSION_ENTRY(PBXEdit_RedEye, 1, 00, 002, 00)


ER    PBXEdit_RedEye_Remove(PPBX_EDIT_PARAM p_pbxEditParam)  /* paramNum = 0;  */
{
    UINT32    Buff,BuffSize;
    PIMG_BUF  pSrcImg;
    PIMG_BUF  pDstImg;
    INT32     nRERNum;
    ARER_MEMHANDLE      tMemHandle;
    ARER_OFFSCREEN      tImg;
    ARER_IMAGEATTRIB    tImgAttrib;
    INT32               liStart;
    LPARER_EYEINFO      pEyeInfo = NULL;
    UINT32              p_cacheY,p_cacheUV,p_temp_cacheSize;

    if (p_pbxEditParam == NULL)
    {
        DBG_ERR("p_pbxEditParam is Null\r\n");
        return E_PAR;
    }
    pSrcImg = p_pbxEditParam->pSrcImg;
    pDstImg = p_pbxEditParam->pOutImg;
    Buff    = p_pbxEditParam->workBuff;
    BuffSize = p_pbxEditParam->workBufSize;

    if (pSrcImg == NULL)
    {
        DBG_ERR("pSrcImg is Null\r\n");
        return E_PAR;
    }
    if (pDstImg == NULL)
    {
        DBG_ERR("pOutImg is Null\r\n");
        return E_PAR;
    }
    if (Buff == 0)
    {
        DBG_ERR("workBuff is Null\r\n");
        return E_PAR;
    }
    if (BuffSize == 0)
    {
        DBG_ERR("workBufSize is Null\r\n");
        return E_PAR;
    }
    if (p_pbxEditParam->paramNum != 0)
    {
        DBG_ERR("Param Num %d\r\n",p_pbxEditParam->paramNum);
        return E_PAR;
    }
    DBG_IND("Img.W=%04d,H=%04d,PxlFmt=%d, buf=0x%x,bufSize=0x%x\r\n",pSrcImg->Width,pSrcImg->Height,pSrcImg->PxlFmt,Buff,BuffSize);


    pEyeInfo = (LPARER_EYEINFO) &eyeInfo;
    tMemHandle.lMemBlockNum = 1;

    //tMemHandle.tMemBlock[0].pMemBlock = (void*)Buff;
    tMemHandle.tMemBlock[0].pMemBlock = (UINT8*)dma_getCacheAddr(Buff);
    tMemHandle.tMemBlock[0].ulMemBlockSize = BuffSize;
    tMemHandle.tMemBlock[0].ulMemType = 0;  // fastest memory

    // initialize input image
    tImg.lWidth = pSrcImg->Width;
    tImg.lHeight = pSrcImg->Height;
    if(pSrcImg->PxlFmt == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
    {
        tImg.lPixelArrayFormat = ARER_PAF_YCbCr420_YPUV;
    }
    else if(pSrcImg->PxlFmt == GX_IMAGE_PIXEL_FMT_YUV422_PACKED)
    {
        tImg.lPixelArrayFormat = ARER_PAF_YCbCr422_YPUV;
    }
    else
    {
        DBG_ERR("pixel format %d\r\n",pSrcImg->PxlFmt);
        return E_PAR;
    }

    p_cacheY = dma_getCacheAddr(pSrcImg->PxlAddr[0]);
    p_cacheUV = dma_getCacheAddr(pSrcImg->PxlAddr[1]);
    p_temp_cacheSize = GxImg_CalcRequireSize(pSrcImg->Width, pSrcImg->Height, pSrcImg->PxlFmt, pSrcImg->LineOffset[0]);

    dma_flushReadCache((UINT32)p_cacheY,p_temp_cacheSize);
    tImg.pixelArray.planar.pPixelArray[0] = (UINT8 *)p_cacheY;
    tImg.pixelArray.planar.pPixelArray[1] = (UINT8 *)p_cacheUV;
    tImg.pixelArray.planar.lLinebytesArray[0] = pSrcImg->LineOffset[0];
    tImg.pixelArray.planar.lLinebytesArray[1] = pSrcImg->LineOffset[1];

    nRERNum = ARER_AutoDetect(&tImg, NULL, &tMemHandle, pEyeInfo, MAX_EYE_NUM, NULL, &liStart, NULL, 0, ARER_DETECT_FULL);

    DBG_IND("pMemBlock = 0x%x, pPixelArray[0] = 0x%x, pPixelArray[1]=0x%x\r\n", tMemHandle.tMemBlock[0].pMemBlock,tImg.pixelArray.planar.pPixelArray[0],tImg.pixelArray.planar.pPixelArray[1]);

    memcpy(pDstImg,pSrcImg,sizeof(IMG_BUF));
    if (nRERNum > 0)
    {
        DBG_IND("Detect %d red eye\r\n", nRERNum);
        ARER_AutoCorrect(&tImg, &tImgAttrib, &tMemHandle, pEyeInfo, nRERNum, 0);
        dma_flushWriteCache((UINT32)p_cacheY,p_temp_cacheSize);
    }
    else if (nRERNum == 0)
    {
        DBG_WRN("Not find red eye\r\n");
        return E_SYS;
    }
    else
    {
        DBG_ERR("Red eye Detect error, code = %d\r\n", nRERNum);
        return E_SYS;
    }
    return E_OK;
}

