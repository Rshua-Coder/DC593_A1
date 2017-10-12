/**
    LDWS detection library.

    @file       ldws_ALG.h
    @ingroup    mILibLDWS_Nvt

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _LDWS_ALG_H
#define _LDWS_ALG_H

/**
    @addtogroup mILibLDWS_Nvt
*/

/**
     @name LDWSreturn status
*/
//@{
#define LDWSALG_STA_OK                  (0)                   ///<  the status is ok, for normal case
#define LDWSALG_STA_ERROR               (-1)                  ///<  some error occurred
//@}

/**
    LDWS departure info
*/
//@{
typedef enum
{
    LDWS_DEPARTURE_NONE  = 0,     ///<  no departure
    LDWS_DEPARTURE_LEFT  = 1,     ///<  left departure
    LDWS_DEPARTURE_RIGHT = 2      ///<  right departure
} LDWS_DEPARTURE_DIR;
//@}

/**
    LDWS failure info
*/
//@{
typedef enum
{
    LDWS_FAILURE_TRUE   = 1,      ///<  Enter failure mode
    LDWS_FAILURE_FALSE = 0        ///<  Not in failure mode
} LDWS_FAILURE_STATUS;
//@}

/**
  LDWS initial parameters structure
*/
//@{
typedef struct _LDWS_INIT_PARMS
{
    UINT32 uiFocalLen;               ///<  focal length, unit:mm
    INT32  ik;                       ///<  ratio of focal length
    INT32  ieu;                      ///<  eu = k*focal length
    INT32  iev;                      ///<  ev = k*focal length
    INT32  ieuslope;                 ///<  position of vanishing point
    INT32  iInitRow;                 ///<  initial row of detection, the (0,0) is on left right
    INT32  iLsensitivity;            ///<  warning sensitivity of left lane
    INT32  iRsensitivity;            ///<  warning sensitivity of right lane
    INT32  iThreshold1;             ///<  threshold of left lane detection
    INT32  iThreshold2;             ///<  threshold of right lane detection
    UINT32 uiHs;                     ///<  distance from camera to ground of car
    INT32  iFsizeDq2;                ///<  this value affect the warning timing, it can be adjusted according to the value of focal length, ex: focal length = 6mm, Fsize_DQ2 =100, focal length = 12mm, Fsize_DQ2 = 200
    UINT32 uiCamera2CarDistanceCm;   ///<  distance from camera to front of the car
} LDWS_INIT_PARMS;
//@}

/**
     LDWS detection input structure.
*/
//@{
typedef struct _LDWS_SRCIMG_INFO
{
    UINT32 uiLdSrcBufAddr;        ///<  input buffer address
    UINT32 uiLdSrcWidth;          ///<  width of input image
    UINT32 uiLdSrcHeight;         ///<  height of input image
    UINT32 uiLdSrcLineofs;        ///<  lineofset of input image
    UINT32 uiLdRoiWidth;          ///<  width of ROI region for LDWS
    UINT32 uiLdRoiHeight;         ///<  height of ROI region for LDWS
    UINT32 uiLdRoiSx;             ///<  start x coordinate of ROI region
    UINT32 uiLdRoiSy;             ///<  start y coordinate of ROI region
} LDWS_SRCIMG_INFO;
//@}

/**
                 uiLdSrcWidth
 -------------------------------------------------
 - - (uiLdRoiSx, uiLdRoiSy)                      -
 -   -                                           -
 -     ------------uiLdRoiWidth-----------       -
 -     -                                 -       -
 -     -   (iLx1,iLy1)*    * (iRx1,iRy1) -       -
 -     -                          uiLdRoiHeight  - uiLdSrcHeight
 -     -                                 -       -
 -     -(iLx2,iLy2)*           * (iRx2,iRy2)-    -
 -     ----------------------------------        -
 -                                               -
 -                                               -
 -------------------------------------------------
*/
//@{
typedef struct _LDWS_RESULTINFO
{
    LDWS_SRCIMG_INFO        LdSrcImgInfo;  ///<  source image information
    INT32                   iLx1;          ///<  pt1's x coordinate of left lane
    INT32                   iLy1;          ///<  pt1's y coordinate of left lane
    INT32                   iLx2;          ///<  pt2's x coordinate of left lane
    INT32                   iLy2;          ///<  pt2's y coordinate of left lane
    INT32                   iRx1;          ///<  pt1's x coordinate of right lane
    INT32                   iRy1;          ///<  pt1's y coordinate of right lane
    INT32                   iRx2;          ///<  pt2's x coordinate of right lane
    INT32                   iRy2;          ///<  pt2's y coordinate of right lane
    LDWS_FAILURE_STATUS     Failure;       ///<  Failure mode
    LDWS_DEPARTURE_DIR      DepartureDir;  ///<  departure direction
    UINT32                  uiEnerUpdateMode;
    UINT32                  uiVPYNew;
    UINT32                  uiVPYDef;
} LDWS_RESULT_INFO;
//@}


/**
    Set initial parameters into LDWS Lib.
*/
extern void LDWS_setInitParms(LDWS_INIT_PARMS LDWSSysParamter);

/**
    LDWS Detection
*/
extern UINT32 LDWS_Detection(LDWS_SRCIMG_INFO Src_Img_info, UINT32 InputUsableMemAddr, LDWS_INIT_PARMS  LDWSSysParamter, LDWS_RESULT_INFO *LDWS_INFO);

/**
    Set LDWS initial flag
*/
extern void LDWS_setInitFlag(UINT32 Flag);

/**
    Check LDWS initial flag
*/
extern UINT32 LDWS_checkInitFlag(void);


#endif

