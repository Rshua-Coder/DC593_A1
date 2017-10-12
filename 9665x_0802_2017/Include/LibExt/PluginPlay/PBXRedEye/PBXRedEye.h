/**
    Playback Edit plugin module.

    All function for playback Edit, This module can be a plug-in to Application Playback.

    @file       PBXRedEye.h
    @ingroup    mILibPbEditPlugIn

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _PBXREDEYE_H
#define _PBXREDEYE_H

#include "Type.h"
#include "PBXEdit.h"

/**
    @addtogroup mILibPbEditPlugIn
*/

/**
     Remove redeye in a image.

     This function will remove redeye in a image, the maximum redeye number can be removed is 20.

     @param[in] p_pbxEditParam description.

     @note: p_pbxEditParam->paramNum -> the parameter number should be 0.

     @return
         - @b E_PAR:     input parameter error
         - @b E_NOMEM:   input working buffer memory not enouth
         - @b E_OK:      result ok

     Example:
     @code
     {
         PBX_EDIT_PARAM  EditParam ={0};
         IMG_BUF         OutImg;
         ER              RedEyertnValue;

         EditParam.pSrcImg = &DecodedBuf;
         EditParam.pOutImg = &OutImg;
         EditParam.workBuff = tmpBuff;
         EditParam.workBufSize = WORKBUFF_SIZE;
         EditParam.paramNum = 0;
         PBXEdit_SetFunc(PBXEdit_RedEye_Remove);
         RedEyertnValue = PBXEdit_Apply(&EditParam);
         if (RedEyertnValue!=E_OK)
         {
            DBG_ERR("redeye remove\r\n");
         }
     }
     @endcode
*/
extern ER    PBXEdit_RedEye_Remove(PPBX_EDIT_PARAM p_pbxEditParam);

#endif

//@}
