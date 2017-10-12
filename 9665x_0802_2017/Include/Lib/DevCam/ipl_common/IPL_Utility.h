/**
    Public header file for IPL utility

    This file is the header file that define the API and data type for IPL utility tool.

    @file       IPL_Utility.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _IPL_UTILITY_H_
#define _IPL_UTILITY_H_

#include "type.h"
#include "ErrorNo.h"

/** \addtogroup mIIPL */
//@{

/**
    ipl flow CB function
*/
typedef void(*fpCallBack)(void);

/**
    IPL flow message(channel id)
*/
//@{
typedef enum _IPL_FLOW_PROC_CH_MSG
{
    IPL_FLOW_Y = 0x00000001,                ///< process Y channel
    IPL_FLOW_U = 0x00000002,                ///< process U channel
    IPL_FLOW_V = 0x00000004,                ///< process V channel
    ENUM_DUMMY4WORD(IPL_FLOW_PROC_CH_MSG)
} IPL_FLOW_PROC_CH_MSG;

/**
    IPL flow message(flow id)
*/
typedef enum _IPL_FLOW_PROC_MSG
{
    IPL_FLOW_RESERVE        = 0x000000ff,  ///< reserve LSB 8 bit for IPL_FLOW_PROC_CH_MSG or other information

    //0x100 ~ 0x1f00 for cpature d2d flow
    IPL_FLOW_RAW2YCC_QV     = 0x00000100,  ///< process QV: raw 2 ycc/ref image + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)
    IPL_FLOW_IFE2REF_QV     = 0x00000200,  ///< process QV: ref image(IFE2) + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)
    IPL_FLOW_IFE2REF_PRI    = 0x00000300,  ///< process PRI: ref image(IFE2) + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)
    IPL_FLOW_IMED2D_QV      = 0x00000400,  ///< process QV: D2D ime image + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)
    IPL_FLOW_IMED2D_QVREF   = 0x00000500,  ///< process QV: D2D gen. qv ref image + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)
    IPL_FLOW_RAW2YCC_PRI    = 0x00000600,  ///< process PRI: raw 2 ycc image + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)
    IPL_FLOW_RAWDEC         = 0x00000700,  ///< process PRI: raw dec
    IPL_FLOW_IMED2D_SCAL    = 0x00000800,  ///< process ime d2d scale(pure) + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)
    IPL_FLOW_CAL_STRIPE     = 0x00000900,  ///< pre-calculate stripe (special case)

    //0x2000 ~ 0x1f000 for get raw flow
    IPL_FLOW_NOR_GETRAW             = 0x00002000,  ///< process Get raw
    IPL_FLOW_HDR_GETRAW             = 0x00003000,  ///< process Get raw
    IPL_FLOW_VID_GETRAW             = 0x00004000,  ///< process Get raw
    IPL_FLOW_HS_GETRAW              = 0x00005000,  ///< process Get raw
    IPL_FLOW_3D_GETRAW              = 0x00006000,  ///< process Get raw
    IPL_FLOW_EB_GETRAW              = 0x00007000,  ///< process Get raw
    IPL_FLOW_AEB_GETRAW             = 0x00008000,  ///< process Get raw
    IPL_FLOW_AWB_GETRAW             = 0x00009000,  ///< process Get raw
    IPL_FLOW_SIM_GETRAW             = 0x0000A000,  ///< process Get raw
    IPL_FLOW_MFHDR_GETRAW           = 0x0000B000,  ///< process Get raw
    IPL_FLOW_MFANTISHAKE_GETRAW     = 0x0000C000,  ///< process Get raw
    IPL_FLOW_CCIR_GETRAW            = 0x0000D000,  ///< process Get raw
    IPL_FLOW_DUALSENSOR_GETRAW      = 0x0000E000,  ///< process Get raw

    //0x20000 ~ 0x1f0000 for post process flow
    IPL_FLOW_HDR_IFE2D2D    = 0x00020000,  ///< post process WDR IFE2 d2d + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)
    IPL_FLOW_HDR_IMED2D     = 0x00030000,  ///< post process WDR ime d2d

    ENUM_DUMMY4WORD(IPL_FLOW_PROC_MSG)
} IPL_FLOW_PROC_MSG;
//@}


#define IPL_FLOW_CAPD2D_MIN (0x00000100)
#define IPL_FLOW_CAPD2D_MAX (0x00001f00)

#define IPL_FLOW_GETRAW_MIN (0x00002000)
#define IPL_FLOW_GETRAW_MAX (0x0001f000)

#define IPL_FLOW_PPROC_MIN (0x000020000)
#define IPL_FLOW_PPROC_MAX (0x0001f0000)

/**
    IPL process ID
*/
typedef enum _IPL_PROC_ID
{
    IPL_ID_1 = 0,                       ///< process id 0
    IPL_ID_2,                           ///< process id 1
    IPL_ID_MAX_NUM,
    ENUM_DUMMY4WORD(IPL_PROC_ID)
} IPL_PROC_ID;

#define IPL_MAX_SEN_NUM IPL_ID_MAX_NUM  ///< ipl max process number


/**
    YUV channel index
*/
//@{
typedef enum
{
    IPL_IMG_YUV_Y = 0,                  ///< process channel Y
    IPL_IMG_YUV_U,                      ///< process channel U
    IPL_IMG_YUV_V,                      ///< process channel V
    ENUM_DUMMY4WORD(IPL_IMG_YUV_CH)
} IPL_IMG_YUV_CH;
#define IPL_IMG_YUV_UV IPL_IMG_YUV_U    ///< process channel UV for packed format
//@}

/**
    Image buffer address
    for IPL_UTI_SET_CONT_BUFINFO function (buf[0]: image start address, buf[1]: ignore, buf[2]: ignore)
    for IPL_UTI_SET_Y_UVPACK_SEC_BUFINFO function (buf[0]: Y address, buf[1]: UV address, buf[2]: ignore)
    for IPL_UTI_SET_YUVP_SEC_BUFINFO function (buf[0]: Y address, buf[1]: U address, buf[2]: V address)
*/
typedef struct _IPL_IMG_BUF_INFO
{
    UINT32 Buf[3];      ///< buffer address
} IPL_IMG_BUF_INFO;


/**
    IPL YCC format
*/
typedef enum _IPL_YUV_IMG_TYPE
{
    /* 3 channel */
    IPL_IMG_YUV444 = 0,                 ///< 444 planar
    IPL_IMG_YUV422,                     ///< 422 planar
    IPL_IMG_YUV420,                     ///< 420 planar

    /* 2 channel */
    IPL_IMG_Y_PACK_UV444 = 100,         ///< 444 format y planar UV pack UVUVUVUVUV.....
    IPL_IMG_Y_PACK_UV422,               ///< 422 format y planar UV pack UVUVUVUVUV.....
    IPL_IMG_Y_PACK_UV420,               ///< 420 format y planar UV pack UVUVUVUVUV.....

    /* 2 channel */
    IPL_IMG_V_PACK_UV444 = 200,         ///< 444 format v planar UV pack UVUVUVUVUV.....
    IPL_IMG_V_PACK_UV422,               ///< 422 format v planar UV pack UVUVUVUVUV.....
    IPL_IMG_V_PACK_UV420,               ///< 420 format v planar UV pack UVUVUVUVUV.....

    /* extend */
    IPL_IMG_Y            = 300,         ///< Only Y information
    //IPL_IMG_RGB,                        ///< R G B format
    //IPL_IMG_BAYER,                      ///< R Gr Gb B format
    IPL_IMG_YUYV422,                    ///< 422 format YUYVYUYVYUYVYUYV........
    ENUM_DUMMY4WORD(IPL_YUV_IMG_TYPE)
} IPL_YUV_IMG_TYPE;

/**
    channel information
*/
typedef struct _IPL_YCC_CH_INFO
{
    UINT32 Width;       ///< image width(pixel)
    UINT32 Height;      ///< image height(pixel)
    UINT32 LineOfs;     ///< image line offset(byte)
} IPL_YCC_CH_INFO;

/**
    IPL YCC Image Information
*/
typedef struct _IPL_YCC_IMG_INFO
{
    IPL_YUV_IMG_TYPE type;  ///< image type
    UINT32 PixelAddr[3];    ///< pixel YUV address
    IPL_YCC_CH_INFO Ch[3];  ///< image channel information
} IPL_YCC_IMG_INFO;

/**
    IPL RAW Image bits
*/
typedef enum _IPL_RAW_BIT
{
    IPL_RAW_BIT_8 = 8,      ///< 8 bits
    IPL_RAW_BIT_10 = 10,    ///< 10 bits
    IPL_RAW_BIT_12 = 12,    ///< 12 bits
    IPL_RAW_BIT_16 = 16,    ///< 16 bits
    IPL_RAW_BIT_MAX_CNT,
    ENUM_DUMMY4WORD(IPL_RAW_BIT)
} IPL_RAW_BIT;

/**
    IPL RAW Image start pixel
*/
typedef enum _IPL_RAW_PIX
{
    IPL_RAW_PIX_R = 0,      ///< start pixel R
    IPL_RAW_PIX_GR,         ///< start pixel Gr
    IPL_RAW_PIX_GB,         ///< start pixel Gb
    IPL_RAW_PIX_B,          ///< start pixel B
    IPL_RAW_PIX_MAX_CNT,
    ENUM_DUMMY4WORD(IPL_RAW_PIX)
} IPL_RAW_PIX;

/**
    IPL RAW Image Information
*/
typedef struct _IPL_RAW_IMG_INFO
{
    UINT32 PixelAddr;       ///< image buffer address
    UINT32 Width;           ///< image width(pixel)
    UINT32 Height;          ///< image height(pixel)
    UINT32 LineOffset;      ///< image line offset(byte)
    IPL_RAW_BIT Bit;        ///< bits /pixel
    IPL_RAW_PIX STPix;      ///< image start pixel
} IPL_RAW_IMG_INFO;

/**
    mage coordinates
    origin of coordinates is left-top point.
*/
typedef struct _Coordinate
{
    UINT32  x;              ///< x point
    UINT32  y;              ///< y point
}Coordinate;


#define IMG_DIS_RATIO_UINT  100     ///< dis scale uint
#define IMG_LCA_HDR_RATIO_UINT 100  ///< Lca/Hdr scale uint


/**

     IPL_UTI_Y_INFO_CONV2

     @note convert Y information to UV pack format

     @param[in] Type        image type
     @param[in] YInfor      y channel information
     @return
         - @b IPL_YCC_CH_INFO:  UV channel information
*/
extern IPL_YCC_CH_INFO IPL_UTI_Y_INFO_CONV2(IPL_YUV_IMG_TYPE Type, IPL_YCC_CH_INFO YInfor);

/**

     IPL_UTI_SET_CONT_BUFINFO

     @note depend on buffer information to set YCC address

     @param[in] Type            image type
     @param[in] BufInfo         buffer address(only reference buf[0])
     @param[in,out] YCCInfo    input: image information, output: image address
     @return
         - @b void.

*/
extern void IPL_UTI_SET_CONT_BUFINFO(IPL_YUV_IMG_TYPE Type, IPL_IMG_BUF_INFO BufInfo, IPL_YCC_IMG_INFO *YCCInfo);

/**

     IPL_UTI_SET_YUVP_SEC_BUFINFO

     @note depend on buffer information to set YCC address

     @param[in] BufInfo         buffer address(Y: buf[0], U: buf[1], V: buf[2])
     @param[out] YCCInfo        output: image address
     @return
         - @b void.
*/
#define IPL_UTI_SET_YUVP_SEC_BUFINFO(BufInfo, YCCInfo) \
{ \
    YCCInfo.PixelAddr[0] = BufInfo.Buf[0]; \
    YCCInfo.PixelAddr[1] = BufInfo.Buf[1]; \
    YCCInfo.PixelAddr[2] = BufInfo.Buf[2]; \
}

/**

     IPL_UTI_SET_Y_UVPACK_SEC_BUFINFO

     @note depend on buffer information to set YCC address

     @param[in] BufInfo         buffer address(Y: buf[0], UV: buf[1])
     @param[out] YCCInfo        output: image address
     @return
         - @b void.
*/
#define IPL_UTI_SET_Y_UVPACK_SEC_BUFINFO(BufInfo, YCCInfo) \
{ \
    YCCInfo.PixelAddr[0] = BufInfo.Buf[0]; \
    YCCInfo.PixelAddr[1] = BufInfo.Buf[1]; \
    YCCInfo.PixelAddr[2] = BufInfo.Buf[1]; \
}

/**

     IPL_YUV_SIZE

     @note depend on Y width, height and format to calculate buffer size
     @param[in] Type    image type
     @param[in] YSizeH  image width(pixel)
     @param[in] YSizeV  image height(pixel)
     @return
         - @b UINT32    image buffer size
*/
extern UINT32 IPL_YUV_SIZE(IPL_YUV_IMG_TYPE Type, UINT32 YSizeH, UINT32 YSizeV);

/**

     IPL_UTI_RAW_SIZE

     @note depend on raw with, height and bits to calculate image size
     @param[in] Bit     raw bit number(IPL_RAW_BIT)
     @param[in] SizeH   raw width(pixel)
     @param[in] SizeV   raw height(pixel)
     @return
         - @b UINT32    raw image size
*/
#define IPL_UTI_RAW_SIZE(Bit, SizeH, SizeV) ((SizeH * SizeV * Bit) / IPL_RAW_BIT_8)

/**

     IPL_UTI_RAW_BUF_SIZE

     @note depend on raw with, height, and line offset to calculate buffer size
     @param[in] SizeH   raw width(pixel)
     @param[in] LineOFS raw line offset(byte)
     @param[in] SizeV   raw height(pixel)
     @return
         - @b UINT32    raw buffer size
*/
#define IPL_UTI_RAW_BUF_SIZE(SizeH, LineOFS, SizeV) (LineOFS * SizeV)


/**

     IPL_UTI_RAWH_CONV2_LOFS

     @note depend on raw width & bit convert to line offset(4 word align)
     @param[in] SizeH   pixel
     @param[in] Bit     IPL_RAW_BIT)
     @return
         - @b UINT32    line offset(byte)
*/
#define IPL_UTI_RAWH_CONV2_LOFS(SizeH, Bit) (((((SizeH * Bit) / IPL_RAW_BIT_8) + 3) / 4) * 4)

/**

     IPL_UTI_CONV_ALGINFO_RAW_BIT

     @note convert alg iformation(SEL_RAW_BITDEPTH) to IPL_RAW_BIT index
     @param[in] Idx     SEL_RAW_BITDEPTH
     @return
         - @b UINT32    bit(IPL_RAW_BIT)
*/
extern UINT32 IPL_UTI_CONV_ALGINFO_RAW_BIT(UINT32 Idx);

/**

     IPL_UTI_CONV_SEN_RATIO

     @note convert sensor iformation(SENSOR_RATIO) to IPL_SENCROPRATIO index
     @param[in] Idx     SENSOR_RATIO
     @return
         - @b UINT32    ipl ratio(IPL_SENCROPRATIO)
*/
extern UINT32 IPL_UTI_CONV_SEN_RATIO(UINT32 Idx);

/**

     IPL_UTI_CONV_SEN_STPIX

     @note convert sensor start pix iformation(SENSOR_STPIX) to IPL index(IPL_RAW_PIX)
     @param[in] Idx     SENSOR_STPIX
     @return
         - @b UINT32    raw start pixel(IPL_RAW_PIX)
*/
extern UINT32 IPL_UTI_CONV_SEN_STPIX(UINT32 Idx);

/**

     IPL_UTI_CONV2_SEN_ID

     @note convert IPL ID(IPL_PROC_ID) to Sensor ID(SENSOR_ID)
     @param[in] Idx     IPL_PROC_ID
     @return
         - @b UINT32    sensor id(SENSOR_ID)
*/
extern UINT32 IPL_UTI_CONV2_SEN_ID(UINT32 Idx);

/**
     IPL_UTI_CONV2_AE_ID

     @note convert IPL ID(IPL_PROC_ID) to AE ID(AE_PROC_ID)
     @param[in] Idx     IPL_PROC_ID
     @return
         - @b UINT32    ae process id(AE_PROC_ID)
*/
extern UINT32 IPL_UTI_CONV2_AE_ID(UINT32 Idx);

/**
     IPL_UTI_CONV2_AWB_ID

     @note convert IPL ID(IPL_PROC_ID) to AWB ID(AWB_PROC_ID)
     @param[in] Idx     IPL_PROC_ID
     @return
         - @b UINT32    awb process id(AWB_PROC_ID)
*/
extern UINT32 IPL_UTI_CONV2_AWB_ID(UINT32 Idx);

/**
     IPL_UTI_CONV2_AF_ID

     @note convert IPL ID(IPL_PROC_ID) to AF ID(AF_PROC_ID)
     @param[in] Idx     IPL_PROC_ID
     @return
         - @b UINT32    af process id(AF_PROC_ID)
*/
extern UINT32 IPL_UTI_CONV2_AF_ID(UINT32 Idx);

/**
     IPL_UTI_CONV2_DS_FMT

     @note convert IPL format(IPL_YUV_IMG_TYPE) to Data Stamp format(DS_YCC_FORMAT)
     @param[in] type    IPL_YUV_IMG_TYPE
     @return
         - @b UINT32    data stamp format
*/
extern UINT32 IPL_UTI_CONV2_DS_FMT(UINT32 type);

/**
     IPL_UTI_CONV2_PF_FMT

     @note convert IPL format(IPL_YUV_IMG_TYPE) to Photo Frame format(PF_YCC_FORMAT)
     @param[in] type    IPL_YUV_IMG_TYPE
     @return
         - @b UINT32    photo frame format
*/
extern UINT32 IPL_UTI_CONV2_PF_FMT(UINT32 type);

/**
     IPL_UTI_SENCROPRATIO_ADJ_VSIZE

     @note adjust image v size by image ratio
     @param[in] ImgRatioIdx     SEL_IMAGERATIO
     @param[in] DefRatioIdx     SEL_IMAGERATIO
     @param[in] InSize          pixel
     @param[in] Align           align value
     @return
         - @b UINT32    OutSize(pixel)
*/
extern UINT32 IPL_UTI_SENCROPRATIO_ADJ_VSIZE(UINT32 ImgRatioIdx, UINT32 DefRatioIdx, UINT32 InSize, UINT32 Align);

/**
     IPL_UTI_SENCROPRATIO_ADJ_HSIZE

     @note adjust image h size by image ratio
     @param[in] ImgRatioIdx     SEL_IMAGERATIO
     @param[in] DefRatioIdx     SEL_IMAGERATIO
     @param[in] InSize          pixel
     @param[in] Align           align value
     @return
         - @b UINT32    OutSize(pixel)
*/
extern UINT32 IPL_UTI_SENCROPRATIO_ADJ_HSIZE(UINT32 ImgRatioIdx, UINT32 DefRatioIdx, UINT32 InSize, UINT32 Align);

/**
     IPL_UTI_DIS_ADJ_HSIZE

     @note adjust image h size by dis ratio
     @param[in] InSize      pixel
     @param[in] Ratio       ratio value
     @param[in] Align       align value
     @return
         - @b UINT32    OutSize(pixel)
*/
extern UINT32 IPL_UTI_DIS_ADJ_HSIZE(UINT32 InSize, UINT32 Ratio, UINT32 Align);

/**
     IPL_UTI_DIS_ADJ_VSIZE

     @note adjust image v size by dis ratio
     @param[in] InSize      pixel
     @param[in] Ratio       ratio value
     @param[in] Align       align value
     @return
         - @b UINT32    OutSize(pixel)
*/
extern UINT32 IPL_UTI_DIS_ADJ_VSIZE(UINT32 InSize, UINT32 Ratio, UINT32 Align);

/**
     IPL_UTI_IPESUB_ADJ_HSIZE

     @note adjust image h size by iq ratio
     @param[in] InSize      pixel
     @param[in] Ratio       ratio value
     @param[in] Align       align value
     @return
         - @b UINT32    OutSize(pixel)
*/
extern UINT32 IPL_UTI_IPESUB_ADJ_HSIZE(UINT32 InSize, UINT32 Ratio, UINT32 Align);

/**
     IPL_UTI_IPESUB_ADJ_VSIZE

     @note adjust image v size by dis ratio
     @param[in] InSize      pixel
     @param[in] Ratio       ratio value
     @param[in] Align       align value
     @return
         - @b UINT32    OutSize(pixel)
*/
extern UINT32 IPL_UTI_IPESUB_ADJ_VSIZE(UINT32 InSize, UINT32 Ratio, UINT32 Align);

#define IPL_UTI_IMESUB_ADJ_HSIZE IPL_UTI_IPESUB_ADJ_HSIZE
#define IPL_UTI_IMESUB_ADJ_VSIZE IPL_UTI_IPESUB_ADJ_VSIZE

#define IPL_UTI_SIESUB_ADJ_HSIZE IPL_UTI_IPESUB_ADJ_HSIZE
#define IPL_UTI_SIESUB_ADJ_VSIZE IPL_UTI_IPESUB_ADJ_VSIZE

#define IPL_ALIGN_ROUNDUP(x, y) ((((x) + (y) - 1) / (y)) * (y))
#define IPL_ALIGN_ROUNDDOWN(x, y) (((x) / (y)) * (y))
#define IPL_ALIGN_ROUND(x, y) ((((x) + ((y)/2)) / (y)) * (y))

//@}
#endif //_IPL_UTILITY_H_
