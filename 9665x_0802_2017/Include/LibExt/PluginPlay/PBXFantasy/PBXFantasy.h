/**
    Playback Edit plugin module.

    All function for playback Edit, This module can be a plug-in to Application Playback.

    @file       PBXFantasy.h
    @ingroup    mILibPbEditPlugIn

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/
#ifndef _PBXFANTASY_H
#define _PBXFANTASY_H

#include "Type.h"
#include "PBXEdit.h"

/**
    @addtogroup mILibPbEditPlugIn
*/

/**
     Edit the image with a fantasy effect.

     Edit the image with a fantasy effect.

     @param[in] p_pbxEditParam description.

     @note: p_pbxEditParam->paramNum -> the parameter number should be 1.\n
            p_pbxEditParam->param[0] -> is the weighting value , range is  0 <= weighting <= 255\n
            the need workbuff size is 2*imageSize;

     @return
         - @b E_PAR:     input parameter error
         - @b E_NOMEM:   input working buffer memory not enouth
         - @b E_OK:      result ok

     Example:
     @code
     {
        PBX_EDIT_PARAM  EditParam ={0};
        IMG_BUF         OutImg;
        ER              Ret;
        UINT32          weighting = 128;
        UINT32          Param[1]={0};

        EditParam.pSrcImg = &DecodedBuf;
        EditParam.pOutImg = &OutImg;
        EditParam.workBuff = tmpBuff;
        EditParam.workBufSize = WORKBUFF_SIZE;
        EditParam.paramNum = 1;
        Param[0] = weighting;
        EditParam.param = Param;
        PBXEdit_SetFunc(PBXEdit_Fantasy);
        Ret = PBXEdit_Apply(&EditParam);
     }
     @endcode
*/
extern ER    PBXEdit_Fantasy(PPBX_EDIT_PARAM p_pbxEditParam);

#endif

//@}
