/**
    Public header file for capture information

    This file is the header file that define the API and data type for capture information.

    @file       imgCapInfor.h
    @ingroup    mIAppCap
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IMG_CAPINFOR_H_
#define _IMG_CAPINFOR_H_

#include "type.h"
#include "ErrorNo.h"

#define IMG_CAP_INFOR_ERR 0xffffffff    ///< error code(ImgCap_GetUIInfo())
#define IMG_CAP_TAG       0x12340000

/** \addtogroup mIAppCap */
//@{

/**
    capture informatin selection index(ImgCap_SetUIInfo() or ImgCap_GetUIInfo())
*/
typedef enum _CAP_SEL_ITEM
{
    /************* CAP_SEL_ITEM start *************/
    CAP_SEL_QUALITY = IMG_CAP_TAG, ///<jpeg encode quality, data type depend on JPEG (0 ~ 100)
    CAP_SEL_DRIVEMODE,             ///<drive mode, data type #SEL_DRIVEMODE.
    CAP_SEL_PICNUM,                ///<picutre number, data type NULL.
    CAP_SEL_FILEFMT,               ///<save file format, data type #SEL_FILEFMT.
    CAP_SEL_REENCTYPE,             ///<jpeg bit rate contorl type, data type #SEL_REENCTYPE.
    CAP_SEL_FILEBUFMODE,           ///<jpeg buffer mode, data type #SEL_FILEBUFMODE
    CAP_SEL_FILEBUFSIZE,           ///<jpeg buffer size(byte), only valid in #SEL_FILEBUFMODE_MANUAL mode
    CAP_SEL_IMG_RATIO,             ///<image ratio(16:9, 4:3,....), data type #IPL_SENCROPRATIO
    CAP_SEL_IMG_RATIO2,            ///<image ratio(16:9, 4:3,....), data type #IPL_SENCROPRATIO
    CAP_SEL_JPG_IMG_H_SIZE,        ///<jpeg pixel size H(%16 == 0)
    CAP_SEL_JPG_IMG_V_SIZE,        ///<jpeg pixel size V(% 8 == 0)
    CAP_SEL_JPGFMT,                ///<jpeg format, data type #SEL_JPGFMT
    CAP_SEL_JPG2_IMG_H_SIZE,       ///<jpeg pixel size H(%16 == 0)
    CAP_SEL_JPG2_IMG_V_SIZE,       ///<jpeg pixel size V(% 8 == 0)
    CAP_SEL_DISPTYPE,              ///<quick view display type, data type #SEL_DISPTYPE
    CAL_SEL_DATASTAMP,             ///<capture data stamp on/off
    CAP_SEL_THUMB_BUF_H_SIZE,      ///<thumb buffer pixel size H
    CAP_SEL_THUMB_BUF_V_SIZE,      ///<thumb buffer pixel size V
    CAP_SEL_THUMB_IMG_H_SIZE,      ///<thumb image pixel size H
    CAP_SEL_THUMB_IMG_V_SIZE,      ///<thumb image pixel size V
    CAP_SEL_THUMB_FMT,             ///<thumb format, data type #IPL_YUV_IMG_TYPE
    CAP_SEL_THUMB2_IMG_H_SIZE,     ///<thumb image pixel size H
    CAP_SEL_THUMB2_IMG_V_SIZE,     ///<thumb image pixel size V
    CAP_SEL_SCREEN_IMG,            ///<screenail image add or not
    CAP_SEL_SCREEN_IMG_H_SIZE,     ///<screenail image pixel size H
    CAP_SEL_SCREEN_IMG_V_SIZE,     ///<screenail image pixel size V
    CAP_SEL_SCREEN_FMT,            ///<screenail format, data type #IPL_YUV_IMG_TYPE
    CAP_SEL_SCREEN2_IMG_H_SIZE,    ///<screenail image pixel size H
    CAP_SEL_SCREEN2_IMG_V_SIZE,    ///<screenail image pixel size V
    CAP_SEL_QV_IMG_H_SIZE,         ///<quick view image pixel size H
    CAP_SEL_QV_IMG_V_SIZE,         ///<quick view image pixel size V
    CAP_SEL_QV_FMT,                ///<quick view format, data type #IPL_YUV_IMG_TYPE
    CAP_SEL_QV2_IMG_H_SIZE,        ///<quick view image pixel size H
    CAP_SEL_QV2_IMG_V_SIZE,        ///<quick view image pixel size V
    CAP_SEL_PRI_FMT,               ///<main image format, data type #IPL_YUV_IMG_TYPE
    CAP_SEL_RAW_BUF_NUM,           ///<raw data buffer number
    CAP_SEL_JPG_BUF_NUM,           ///<jpeg buffer number
    CAP_SEL_YUV_BUF_NUM,           ///<yuv buffer number
    CAP_SEL_IPL_ID,                ///<designate ipl id to get raw(default IPL_ID_1, only valid in dual sensor mode), data type #IPL_PROC_ID
    CAP_SEL_ENCRATIO,              ///<raw encode ratio, 50% ~ 80%, disable 100%
    CAP_SEL_SCREENBUFSIZE,         ///<screenail bit stream buffer size(byte)
    CAP_SEL_MSGBUFSIZE,            ///<debug message buffer size(byte)
    CAP_SEL_RHO_INITQF,            ///<bit rate contorl:RHO initial Q
    CAP_SEL_RHO_TARGETSIZE,        ///<bit rate contorl:RHO target bit stream size(byte)
    CAP_SEL_RHO_LBOUNDSIZE,        ///<bit rate contorl:RHO min bit stream size < target bit stream size(byte)
    CAP_SEL_RHO_HBOUNDSIZE,        ///<bit rate contorl:RHO max bit stream size > target bit stream size(byte)
    CAP_SEL_RHO_RETRYCNT,          ///<bit rate contorl:RHO retry number
    /************* CAP_SEL_ITEM end *************/

    /************* CAP_SEL_ITEM_INT start *************/
    /*************      DO NOT MODIFY     *************/
    CAP_SEL_QV_REF_V_EN,                            //qv ref image v process enable/disable
    CAP_SEL_QV_REF_V_H_SIZE,                        //qv ref image v pixel size H
    CAP_SEL_QV_REF_V_V_SIZE,                        //qv ref image v pixel size V
    CAP_SEL_QV_REF_V_FMT,                           //qv ref image v format
    CAP_SEL_QV_REF_CC_EN,                           //qv ref image cc process enable/disable
    CAP_SEL_QV_REF_CC_H_SIZE,                       //qv ref image cc pixel size H
    CAP_SEL_QV_REF_CC_V_SIZE,                       //qv ref image cc pixel size V
    CAP_SEL_QV_REF_CC_FMT,                          //qv ref image cc format
    CAP_SEL_PRI_REF_V_EN,                           //pri ref image v process enable/disable
    CAP_SEL_PRI_REF_V_H_SIZE,                       //pri ref image v pixel size H
    CAP_SEL_PRI_REF_V_V_SIZE,                       //pri ref image v pixel size V
    CAP_SEL_PRI_REF_V_FMT,                          //pri ref image v format
    CAP_SEL_PRI_REF_CC_EN,                          //pri ref image cc process enable/disable
    CAP_SEL_PRI_REF_CC_H_SIZE,                      //pri ref image cc pixel size H
    CAP_SEL_PRI_REF_CC_V_SIZE,                      //pri ref image cc pixel size V
    CAP_SEL_PRI_REF_CC_FMT,                         //pri ref image cc format
    CAP_SEL_DF_EN,                                  //dark frame mode enable/disable
    CAP_SEL_PRVPIPE,                                //ipl return flow
    CAP_SEL_CH2_REF_EN,                             //sie out channel 2 enable/disable
    CAP_SEL_MULTI_SCALE_EN,                         //multi-scale enable/disable
    CAP_SEL_MULTI_SCALE_PATH,                       //multi-scale path(0: do nothing, 1: SCR -> thumb, 2: SCR -> vga -> thumb, 3: Pri -> vga -> thumb, 4~N not support)
    CAP_SEL_MULTI_SCALE_H_SIZE,                     //multi-scale pixel size H
    CAP_SEL_MULTI_SCALE_V_SIZE,                     //multi-scale pixel size V
    CAP_SEL_MULTI_SCALE_FMT,                        //multi-scale image format
    CAP_SEL_PRE_CAL_STRIPE,                         //pre-calculate eng stripe
    CAP_SEL_CCIR_FLOW_EN,                           //only for CCIR flow
    CAP_SEL_CCIR_FMT,                               //CCIR format
    /*************      DO NOT MODIFY     *************/
    /************* CAP_SEL_ITEM_INT end ***************/
    CAP_SEL_MAX_CNT,
    ENUM_DUMMY4WORD(CAP_SEL_ITEM)
} CAP_SEL_ITEM;

#define CAP_SEL_MAX_ITEM  (CAP_SEL_MAX_CNT - IMG_CAP_TAG)

/**
    capture drive mode
*/
typedef enum _SEL_DRIVEMODE
{
    SEL_DRIVEMODE_SINGLE = 0,               ///<invalid parameter: #CAP_SEL_PICNUM,
    SEL_DRIVEMODE_BURST,
    SEL_DRIVEMODE_INFBURST,                 ///<invalid parameter: #CAP_SEL_PICNUM,
    SEL_DRIVEMODE_EBURST,
    SEL_DRIVEMODE_EINFBURST,                ///<invalid parameter: #CAP_SEL_PICNUM,
    SEL_DRIVEMODE_SMALLBURST16,             ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
    SEL_DRIVEMODE_AEBREAKETING,
    SEL_DRIVEMODE_AWBBREAKETING,
    SEL_DRIVEMODE_PRESHOT,                  ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
    SEL_DRIVEMODE_COUPLESHOT,               ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
    SEL_DRIVEMODE_ALBUMSHOT,                ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
    SEL_DRIVEMODE_DIS,                      ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
    SEL_DRIVEMODE_PANCAPTURE,               ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
    SEL_DRIVEMODE_LAST,                     ///<invalid parameter: #CAP_SEL_PICNUM,
    SEL_DRIVEMODE_HIGHSPEED,                ///<invalid parameter: #CAP_SEL_PICNUM,
    SEL_DRIVEMODE_HIGHSPEED1,
    SEL_DRIVEMODE_IMGEFFECT,                ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
    SEL_DRIVEMODE_HDR,                      ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
    SEL_DRIVEMODE_MULFMHDR,                 ///<invalid parameter: #CAP_SEL_PICNUM (>1), #CAP_SEL_RAW_BUF_NUM
    SEL_DRIVEMODE_CAP3D,                    ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
    SEL_DRIVEMODE_INVID,                    ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_JPG_BUF_NUM
    SEL_DRIVEMODE_SIM,                      ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
    SEL_DRIVEMODE_MULFMANTISHAKE,           ///<invalid parameter: #CAP_SEL_PICNUM (>1), #CAP_SEL_RAW_BUF_NUM
    SEL_DRIVEMODE_DUALSENSOR,               ///<invalid parameter: #CAP_SEL_IPL_ID
    SEL_DRIVEMODE_CUSTOMER1,
    SEL_DRIVEMODE_CUSTOMER2,
    SEL_DRIVEMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_DRIVEMODE)
} SEL_DRIVEMODE;

/**
    save file format
*/
typedef enum _SEL_FILEFMT
{
    SEL_FILEFMT_NONE = 0,                   ///< save none
    SEL_FILEFMT_RAW,                        ///< save raw(bayer)
    SEL_FILEFMT_JPG,                        ///< save jpg
    SEL_FILEFMT_RAW_JPG,                    ///< save raw(bayer) + jpg
    SEL_FILEFMT_MPO,
    SEL_FILEFMT_RAW_MPO,
    SEL_FILEFMT_RAW_LJPG,
    SEL_FILEFMT_RAW_RJPG,
    SEL_FILEFMT_RAW_LJPG_MPO,
    SEL_FILEFMT_RAW_RJPG_MPO,
    SEL_FILEFMT_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_FILEFMT)
} SEL_FILEFMT;

/**
    bit rate control method
*/
typedef enum _SEL_REENCTYPE
{
    SEL_REENCTYPE_FIXQ = 0,                 ///< depend on #CAP_SEL_QUALITY, encode jpeg file
    SEL_REENCTYPE_RHO,                      ///< depend on CAP_SEL_RHO_xxxxx parameters, encode jpeg file
    SEL_REENCTYPE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_REENCTYPE)
} SEL_REENCTYPE;

/**
    allocate buffer method for Jpeg file
*/
typedef enum _SEL_FILEBUFMODE
{
    SEL_FILEBUFMODE_AUTO = 0,               ///< depend on remain memory, to assign jpeg buffer size
    SEL_FILEBUFMODE_MANUAL,                 ///< depend on #CAP_SEL_FILEBUFSIZE value, to assign jpeg buffer size
    SEL_FILEBUFMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_FILEBUFMODE)
}SEL_FILEBUFMODE;

/**
    jpeg sampling mode
*/
typedef enum _SEL_JPGFMT
{
    SEL_JPGFMT_444 = 0,                 ///< jpeg format 444
    SEL_JPGFMT_422,                     ///< jpeg format 422
    SEL_JPGFMT_420,                     ///< jpeg format 420
    SEL_JPGFMT_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_JPGFMT)
}SEL_JPGFMT;

/**
    quick view display type
*/
typedef enum _SEL_DISPTYPE
{
    SEL_DISP_SINGLE = 0,                ///<-
    SEL_DISP_DUAL,                      ///<-
    SEL_DISP_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_DISPTYPE)
}SEL_DISPTYPE;

/**
    datastamp switch(on/off)
*/
typedef enum _SEL_DATASTAMP
{
    SEL_DATASTAMP_ON = 0,               ///< cap datastamp on
    SEL_DATASTAMP_OFF,                  ///< cap datastamp off
    SEL_DATASTAMP_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_DATASTAMP)
}SEL_DATASTAMP;

/**
    capture picture nubmer
*/
typedef enum _SEL_PICNUM
{
    SEL_PICNUM_INF = 0xfffffffe,        ///<-
    SEL_PICNUM_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_PICNUM)
}SEL_PICNUM;


/**
    screennail switch(on/off)
*/

typedef enum _SEL_SCREEN_IMG
{
    SEL_SCREEN_IMG_ON = 0,          ///<-
    SEL_SCREEN_IMG_OFF,             ///<-
    SEL_SCREEN_IMG_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_SCREEN_IMG)
}SEL_SCREEN_IMG;

/**
     Get Cap UI Information

     @param[in] SelIdx    UI Select item

     @return UINT32
         - @b select item information.

    Example:
    @code
    {
        DebugInfo.TotalSize = ImgCap_GetUIInfo(CAP_SEL_MSGBUFSIZE);
    }
    @endcode

*/
extern UINT32 ImgCap_GetUIInfo(CAP_SEL_ITEM SelIdx);

/**
     Set Cap UI Information

     @param[in] SelIdx    UI Select item
     @param[in] Value     item value
     @return ER
         - @b E_OK
         - @b E_SYS

    Example:
    @code
    {
        ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_HIGHSPEED);
    }
    @endcode

*/
extern ER ImgCap_SetUIInfo(CAP_SEL_ITEM SelIdx, UINT32 Value);

//@}
#endif //_IMG_CAPINFOR_H_
