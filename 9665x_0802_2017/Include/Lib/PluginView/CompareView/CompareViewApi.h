/**
    Compare View.

    Display two rectangle on display for compare two image.

    @file       CompareViewApi.h
    @ingroup    mCOMPVIEW

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _COMPAREVIEWAPI_H
#define _COMPAREVIEWAPI_H
#include "GxImage.h"
#include "DispSrvApi.h"

/**
    @addtogroup mCOMPVIEW
*/
//@{

/**
     @name COMPVIEW_STYLE
     @note COMPVIEW_INIT::uiStyle used for configure compare style
*/
//@{
#define COMPVIEW_STYLE_ROI_FOCUS        0x00000001 //actively focus to region of interesting and zoom to fit
#define COMPVIEW_STYLE_MOVE_LIMIT_ROI   0x00000002 //the moving range cannot be out of roi range.(COMPVIEW_STYLE_ROI_FOCUS needed)
//@}

/**
     Command index for CompView_Cmd
*/
typedef enum _COMPVIEW_CMD_IDX{
    COMPVIEW_CMD_IDX_UNKNOWN,   ///< unknown command index
    COMPVIEW_CMD_IDX_ZOOM,      ///< In:COMPVIEW_ZOOM_TYPE  Out:NULL. Zoom
    COMPVIEW_CMD_IDX_MOVE,      ///< In:COMPVIEW_MOVE_DIR   Out:NULL. Pan
    COMPVIEW_CMD_IDX_DISP_CHG,  ///< In:COMPVIEW_DISP_CHG   Out:NULL. Display change.
    COMPVIEW_CMD_IDX_GET_MAP_RECT, ///< In:NULL Out:COMPVIEW_MAP_RECT. Getting best calculated rectangle.
    ENUM_DUMMY4WORD(COMPVIEW_CMD)
}COMPVIEW_CMD_IDX;

/**
     Zoom type for COMPVIEW_CMD_IDX_ZOOM input
*/
typedef enum _COMPVIEW_ZOOM_TYPE{
    COMPVIEW_ZOOM_TYPE_IN,  ///< zoom in
    COMPVIEW_ZOOM_TYPE_OUT, ///< zoom out
    ENUM_DUMMY4WORD(COMPVIEW_ZOOM_TYPE)
}COMPVIEW_ZOOM_TYPE;

/**
     Moving direction for COMPVIEW_MOVE_DIR input
*/
typedef enum _COMPVIEW_MOVE_DIR{
    COMPVIEW_MOVE_DIR_LEFT, ///< pan to left
    COMPVIEW_MOVE_DIR_RIGHT,///< pan to right
    COMPVIEW_MOVE_DIR_UP,   ///< pan to up
    COMPVIEW_MOVE_DIR_DOWN, ///< pan to down
    ENUM_DUMMY4WORD(COMPVIEW_MOVE_DIR)
}COMPVIEW_MOVE_DIR;

/**
     Zoom description
*/
typedef struct _COMPVIEW_ZOOM_DESC{
    UINT32       uiRoiZoomIndex;    ///< the index of pTblZoomNumerator for calculating best fit form scaled region
    UINT32       uiNumOfZoom;       ///< numbers of pTblZoomNumerator
    UINT32*      pTblZoomNumerator; ///< zoom table of numerator
    UINT32       uiZoomDenominator; ///< the denominator of zoom table
}COMPVIEW_ZOOM_DESC;

/**
     Initial structure for CompView_Init
*/
typedef struct _COMPVIEW_INIT{
    UINT32       uiStyle;   ///< use COMPVIEW_STYLE_???, bitwise
    URECT        rcDisp[2]; ///< 2 windows layout, coordinate based on Frame Buffer Coordinate
    USIZE        DispRatio; ///< layout display ratio
    URECT        rcRoi;     ///< Region of interesting based on Source Image Coordinate (face location in source frame)
    IMG_BUF      ciTmpSrc;  ///< if uiStyle include COMPVIEW_STYLE_MOVE_LIMIT_ROI, a temp buffer that size is the same with ciSrc have to be set
    IMG_BUF      ciTmp;     ///< Fb 2X size
    COMPVIEW_ZOOM_DESC ZoomDesc; ///< User Zoom Table
    void        (*OnImageClean)(IMG_BUF* pFb); //!< (Optional) after each image clean we callback for user clean. it can be NULL
}COMPVIEW_INIT,*PCOMPVIEW_INIT;

/**
     Input/Output data packet for CompView_Cmd
*/
typedef struct _COMPVIEW_DATA{
    void*   pData;      ///< input/output data pointer
    UINT32  uiNumByte;  ///< input/output data size
}COMPVIEW_DATA,*PCOMPVIEW_DATA;

/**
     Input/Output data packet for CompView_Cmd
*/
typedef struct _COMPVIEW_MAP_RECT{
    URECT rcSrc;     ///< source rectangle
    URECT rcDst;     ///< destination rectangle
}COMPVIEW_MAP_RECT,*PCOMPVIEW_MAP_RECT;

/**
     Output data of COMPVIEW_CMD_IDX_GET_MAP_RECT
*/
typedef struct _COMPVIEW_CMD{
    COMPVIEW_CMD_IDX Idx;   ///< command index
    COMPVIEW_DATA    In;    ///< input data description (depend on Idx)
    COMPVIEW_DATA    Out;   ///< output data description (depend on Idx)
}COMPVIEW_CMD,*PCOMPVIEW_CMD;

/**
     Input data for COMPVIEW_CMD_IDX_DISP_CHG
*/
typedef struct _COMPVIEW_DISP_CHG{
    URECT        rcDisp[2]; ///< 2 windows layout, coordinate based on Frame Buffer Coordinate
    USIZE        DispRatio; ///< layout display ratio
    IMG_BUF      ciTmp;     ///<Fb 2X size
}COMPVIEW_DISP_CHG,*PCOMPVIEW_DISP_CHG;

/**
     Compare View Initial

     Before using CompView_Cmd(). This moudle have to be initial first.

     @param[in] pInit      refer to COMPVIEW_INIT
     @return Description of data returned.
         - @b TRUE:   initial success.
         - @b FALSE:  failed to initial.
*/
BOOL CompView_Init(COMPVIEW_INIT* pInit);

/**
     Compare View Command

     According COMPVIEW_CMD::Idx to operate compare view.

     @param[in] pCmd      refer to COMPVIEW_CMD.
     @return Description of data returned.
         - @b TRUE:   execute success.
         - @b FALSE:  failed to execute.
*/
BOOL CompView_Cmd(COMPVIEW_CMD* pCmd);

/**
     Compare View OnDraw callback.

     This module provides a OnDraw callback for plug into DispSrv.

     @param[in] pDraw      refer to DISPSRV_DRAW.
     @return Description of data returned.
         - @b E_OK:     execute success.
         - @b OTHERS:   failed to execute.
*/
INT32 CompView_OnDraw(DISPSRV_DRAW* pDraw);

//@}
#endif
