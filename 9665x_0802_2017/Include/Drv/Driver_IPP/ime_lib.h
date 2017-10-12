/**
    Public header file for IME module.

    @file       ime_lib.h
    @ingroup    mIIPPIME
    
    @brief

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/


#ifndef _IME_LIB_H_
#define _IME_LIB_H_

#include "Type.h"

typedef void (*IMEIntCB)(UINT32);

//------------------------------------------------------------------------------
//enum data type

typedef enum _IME_CHANNEL_SEL
{
    IME_CHANNEL_Y   = 0,        ///< Y channel
    IME_CHANNEL_CB  = 1,        ///< CB channel
    IME_CHANNEL_CR  = 2,        ///< CR channel
    ENUM_DUMMY4WORD(IME_CHANNEL_SEL)
} IME_CHANNEL_SEL;


typedef enum _IME_BUFNUM_SEL
{
    IME_BUFNUM_0    = 0,        ///< using 1 buffer
    IME_BUFNUM_1    = 1,        ///< using 2 ping-pong buffers
    IME_BUFNUM_2    = 2,        ///< using 3 ping-pong buffers
    IME_BUFNUM_3    = 3,        ///< using 4 ping-pong buffers
    ENUM_DUMMY4WORD(IME_BUFNUM_SEL)
} IME_BUFNUM_SEL;


//------------------------------------------------------------------------------
/** \addtogroup mIIPPIME */
//@{

//define vairable
/**
    @name IME Define - IME Interrupt Enable and Status
*/
//@{

#define IME_INTE_STRP_END    0x10000000  ///< stripe-end enable
#define IME_INTE_FRM_END     0x80000000  ///< frame-end enable
#define IME_INTE_ALL         (IME_INTE_FRM_END)   ///< all interrupt enable


#define IME_INT_STRP_END    0x10000000  ///< stripe-end enable
#define IME_INT_FRM_END     0x80000000  ///< frame-end status
#define IME_INT_ALL         (IME_INT_FRM_END)   ///< all interrupt status

//@}

/**
    @name IME Define - IME Functions
*/
//@{

#define IME_YCC_TO_RGB  0x00010000  ///< Color space transformation from YCC to RGB
#define IME_RGB_TO_YCC  0x00020000  ///< Color space transformation from RGB to YCC
#define IME_CHRA        0x00040000  ///< Local chroma adaption
#define IME_CHRF        0x00080000  ///< Chroma filter
#define IME_RTX         0x00100000  ///< Retinex processing
#define IME_TDNR        0x00200000  ///< 3D noise reduction
#define IME_CCIR        0x00400000  ///< CCIR mode
#define IME_PTOI        0x00800000  ///< Progress to interlace
#define IME_RSC         0x01000000  ///< Rolling shutter correction
#define IME_GRNS        0x02000000  ///< Grain noise
#define IME_CSP         0x04000000  ///< Color space protection
#define IME_H_RSC       0x08000000  ///< Rolling shutter correction
#define IME_V_RSC       0x01000000  ///< Rolling shutter correction


//@}




/**
    @name IME enum - Engine Control
*/
//@{
typedef enum _IME_ENGINE_OP
{
  IME_ENGINE_STOP = 0,   ///< Stop engine
  IME_ENGINE_START = 1,  ///< Start engine
  ENUM_DUMMY4WORD(IME_ENGINE_OP)
} IME_ENGINE_OP;

typedef enum _IME_IPPB_EN
{
  IME_IPPB_DISABLE = 0,   ///< input ping-pong buffer disable
  IME_IPPB_ENABLE = 1,  ///< input ping-pong buffer enable
  ENUM_DUMMY4WORD(IME_IPPB_EN)
} IME_IPPB_EN;


typedef enum _IME_RESET_EN
{
  IME_RESET_DISABLE = 0,   ///< disable engine soft-reset
  IME_RESET_ENABLE = 1,    ///< enable engine soft-reset
  ENUM_DUMMY4WORD(IME_RESET_EN)
} IME_RESET_EN;


typedef enum _IME_INPUT_SRC_SEL
{
  IPE_To_IME  = 0,         ///< IPE to IME input port
  DRAM_To_IME = 1,         ///< DMA to IME input port
  ENUM_DUMMY4WORD(IME_INPUT_SRC_SEL)
} IME_INPUT_SRC_SEL;


typedef enum _IME_PPB_ID_RESET_EN
{
  IME_PPB_ID_RESET_DISABLE = 0,   ///< disable ping-pong buffer ID soft-reset
  IME_PPB_ID_RESET_ENABLE = 1,    ///< enable ping-pong buffer ID soft-reset
  ENUM_DUMMY4WORD(IME_PPB_ID_RESET_EN)
} IME_PPB_ID_RESET_EN;

typedef enum _IME_INPUT_SRC
{
    IME_INPUT_SRC_IPE2IME = 0,             ///< source IPE
    IME_INPUT_SRC_DMA2IME = 1,             ///< source DRAM
    ENUM_DUMMY4WORD(IME_INPUT_SRC)
} IME_INPUT_SRC;

typedef enum _IME_OUTPUT_PATH_EN
{
    IME_PATH_DISABLE = 0,           ///< disable output path
    IME_PATH_ENABLE = 1,            ///< enable output path
    ENUM_DUMMY4WORD(IME_OUTPUT_PATH_EN)
} IME_OUTPUT_PATH_EN;


typedef enum _IME_MODE_SEL
{  
  IPE2IME_FRAME_MODE   = 0,    ///< Input: IPE to IME mode,  Output: Frame mode
  DRAM2IME_FRAME_MODE  = 1,    ///< Input: Dram to IME mode,  Output: Frame mode 
  IPEDCE2IME_FRAME_MODE  = 4,  ///< Input: IPE-DCE to IME mode,  Output: Frame mode
  IPE2IME_CCIR_MODE      = 5,  ///< Input: IPE to IME CCIR mode,  Output: Frame mode
  ENUM_DUMMY4WORD(IME_MODE_SEL)
} IME_MODE_SEL;
//@}
#define _D2D            DRAM2IME_FRAME_MODE


//------------------------------------------------------------------------------
/**
    @name IME enum - Engine Input Control
*/
//@{
typedef enum _IME_STRIPE_MODE
{
  IME_STRIPE_AUTO_MODE = 0,    ///< Auto stripe information computation mode
  IME_STRIPE_USER_MODE = 1,    ///< User stripe information computation mode
  ENUM_DUMMY4WORD(IME_STRIPE_MODE)
} IME_STRIPE_MODE;


typedef enum _IME_H_STRIPE_OVLP_SEL
{
  IME_H_ST_OVLP_16P  = 0,    ///< Horizontal stripe overlap size: 16 pixels
  IME_H_ST_OVLP_24P  = 1,    ///< Horizontal stripe overlap size: 24 pixels
  IME_H_ST_OVLP_32P  = 2,    ///< Horizontal stripe overlap size: 32 pixels
  IME_H_ST_OVLP_USER = 3,    ///< Horizontal stripe overlap size: user define
  ENUM_DUMMY4WORD(IME_H_STRIPE_OVLP_SEL)
} IME_H_STRIPE_OVLP_SEL;


typedef enum _IME_INPUT_FORMAT_SEL
{
  IME_INPUT_YCC_444  = 0,         ///< YCC444 separate planar format for input
  IME_INPUT_YCC_422  = 1,         ///< YCC422 separate planar format for input
  IME_INPUT_YCC_420  = 2,         ///< YCC420 separate planar format for input
  IME_INPUT_RESERVED = 3,         ///< reversed
  IME_INPUT_YCCP_422 = 4,         ///< YCC422 UV-packing format for input
  IME_INPUT_YCCP_420 = 5,         ///< YCC420 UV-packing format for input
  IME_INPUT_Y_ONLY   = 6,         ///< Only Y channel for input
  IME_INPUT_RGB      = 7,         ///< RGB for input (YCC444)
  ENUM_DUMMY4WORD(IME_INPUT_FORMAT_SEL)
} IME_INPUT_FORMAT_SEL;


typedef enum _IME_SET_IN_BUF_SEL
{
  IME_SET_IN_BUF_0 = 0,   ///< select to set ping-pong buffer0 address
  IME_SET_IN_BUF_1 = 1,   ///< select to set ping-pong buffer1 address
  IME_SET_IN_BUF_BOTH = 2,   ///< select to set ping-pong buffer0 and buffer1 address
  ENUM_DUMMY4WORD(IME_SET_IN_BUF_SEL)
} IME_SET_IN_BUF_SEL;


typedef enum _IME_GET_IN_BUF_SEL
{
  _IME_GET_IN_BUF_0 = 0,   ///< select to get ping-pong buffer0 address
  _IME_GET_IN_BUF_1 = 1,   ///< select to get ping-pong buffer1 address
  _IME_GET_IN_BUF_BOTH = 2,   ///< select to set ping-pong buffer0 and buffer1 address
  ENUM_DUMMY4WORD(IME_GET_IN_BUF_SEL)
} IME_GET_IN_BUF_SEL;


//@}
#define IME_IN_FMT_444     IME_INPUT_YCC_444
#define IME_IN_FMT_422     IME_INPUT_YCC_422
#define IME_IN_FMT_420     IME_INPUT_YCC_420
#define IMEALG_INYUV444    IME_INPUT_YCC_444
#define IMEALG_INYUV422    IME_INPUT_YCC_422
#define IMEALG_INYUV420    IME_INPUT_YCC_420
#define IMEALG_INRGB444    IME_INPUT_RGB

//------------------------------------------------------------------------------
/**
    @name IME enum - Engine Output Control
*/
//@{
typedef enum _IME_PATH_SEL
{
  IME_PATH1_SEL = 1,   ///< operation for output path1
  IME_PATH2_SEL = 2,   ///< operation for output path2
  IME_PATH3_SEL = 4,   ///< operation for output path3
  IME_PATHA_SEL = 8,   ///< operation for all output path
  ENUM_DUMMY4WORD(IME_PATH_SEL)
} IME_PATH_SEL;


typedef enum _IME_OUTPUT_FORMAT_TYPE
{
  IME_OUTPUT_YCC_PLANAR   = 0,         ///< Y/Cb/Cr separate planar type
  IME_OUTPUT_YCC_UVPACKIN = 1,         ///< Y/UV-packing type
  ENUM_DUMMY4WORD(IME_OUTPUT_FORMAT_TYPE)
} IME_OUTPUT_FORMAT_TYPE;


typedef enum _IME_OUTPUT_IMG_FORMAT_SEL
{
  IME_OUTPUT_YCC_444     = 0,         ///< YCC444 format for output  
  IME_OUTPUT_YCC_422_COS = 1,         ///< YCC422 cosited format for output
  IME_OUTPUT_YCC_422_CEN = 2,         ///< YCC422 centered format for output
  IME_OUTPUT_YCC_420_COS = 3,         ///< YCC420 cosited format for output
  IME_OUTPUT_YCC_420_CEN = 4,         ///< YCC420 centered format for output
  IME_OUTPUT_Y_ONLY      = 6,         ///< Y only for output
  IME_OUTPUT_RGB_444     = 7,         ///< RGB444 format for output
  ENUM_DUMMY4WORD(IME_OUTPUT_IMG_FORMAT_SEL)
} IME_OUTPUT_IMG_FORMAT_SEL;


typedef enum _IME_SCALE_METHOD_SEL
{
    IMEALG_BICUBIC     = 0,        ///< bicubic interpolation
    IMEALG_BILINEAR    = 1,        ///< bilinear interpolation
    IMEALG_NEAREST     = 2,        ///< nearest interpolation
    IMEALG_INTEGRATION = 3,        ///< nearest interpolation
    ENUM_DUMMY4WORD(IME_SCALE_METHOD_SEL)
} IME_SCALE_METHOD_SEL;


typedef enum _IME_SCALE_FILTER_EN
{
    IME_SCALEFILTER_DISABLE = 0,   ///< disable scale filtering
    IME_SCALEFILTER_ENABLE = 1,    ///< enable scale filtering
    ENUM_DUMMY4WORD(IME_SCALE_FILTER_EN)
} IME_SCALE_FILTER_EN;


typedef enum _IME_SCALE_FILTER_COEF_MODE
{
  IME_SCALE_FILTER_COEF_AUTO_MODE = 0,    ///< Auto Scale filter coefficeint computation mode
  IME_SCALE_FILTER_COEF_USER_MODE = 1,    ///< User Scale filter coefficeint computation mode
  ENUM_DUMMY4WORD(IME_SCALE_FILTER_COEF_MODE)
} IME_SCALE_FILTER_COEF_MODE;


typedef enum _IME_PPB_NUM_SEL
{
  IME_OUTPUT_PPB_1 = 0,            ///< using 1 output buffer
  ENUM_DUMMY4WORD(IME_PPB_NUM_SEL)
} IME_PPB_NUM_SEL;


typedef enum _IME_OUT_YCHL_TYPE_SEL
{
  IME_OUT_YCHL_TYPE_Y = 0,   ///< Y channel output data type, for Y data
  IME_OUT_YCHL_TYPE_V = 1,   ///< Y channel output data type, for V data
  ENUM_DUMMY4WORD(IME_OUT_YCHL_TYPE_SEL)
} IME_OUT_YCHL_TYPE_SEL;


typedef enum _IME_OUT_DRAM_EN
{
  IME_OUT_DRAM_DISABLE = 0,   ///< output data to dram disable
  IME_OUT_DRAM_ENABLE = 1,   ///< output data to dram enable
  ENUM_DUMMY4WORD(IME_OUT_DRAM_EN)
} IME_OUT_DRAM_EN;



//@}
#define IME_OUT_FMT_444             IME_OUTPUT_YCC_444        
#define IME_OUT_FMT_422_COSITED     IME_OUTPUT_YCC_422_COS   
#define IME_OUT_FMT_422_CENTERED    IME_OUTPUT_YCC_422_CEN   
#define IME_OUT_FMT_420_COSITED     IME_OUTPUT_YCC_420_COS   
#define IME_OUT_FMT_420_CENTERED    IME_OUTPUT_YCC_420_CEN 
#define IMEALG_OUTYUV444            IME_OUTPUT_YCC_444
#define IMEALG_OUTYUV422_COS        IME_OUTPUT_YCC_422_COS
#define IMEALG_OUTYUV422_CEN        IME_OUTPUT_YCC_422_CEN
#define IMEALG_OUTYUV420_COS        IME_OUTPUT_YCC_420_COS
#define IMEALG_OUTYUV420_CEN        IME_OUTPUT_YCC_420_CEN
#define IMEALG_BAYER                IME_OUTPUT_BAYER
#define IMEALG_OUTRGB444            IME_OUTPUT_RGB444



//------------------------------------------------------------------------------
/**
    @name IME enum - Color Space Transformation Function
*/
//@{

typedef enum _IME_CST_EN
{
  IME_CST_DISABLE = 0,       ///< disable color space transformation
  IME_CST_ENABLE = 1,        ///< enable color space transformation
  ENUM_DUMMY4WORD(IME_CST_EN)
} IME_CST_EN;

//@}


//------------------------------------------------------------------------------
/**
    @name IME enum - Chroma Filter Function
*/
//@{
typedef enum _IME_CHRF_EN
{
  IME_CHRF_DISABLE = 0,         ///< disable chroma filter
  IME_CHRF_ENABLE = 1,          ///< enable chroma filter
  ENUM_DUMMY4WORD(IME_CHRF_EN)
} IME_CHRF_EN;


typedef enum _IME_CHRF_RANGE_EN
{
  IME_CHRF_RGF_DISABLE = 0,         ///< disable chroma filter with range domain
  IME_CHRF_RGF_ENABLE = 1,          ///< enable chroma filter with range domain
  ENUM_DUMMY4WORD(IME_CHRF_RANGE_EN)
} IME_CHRF_RANGE_EN;
//@}

//------------------------------------------------------------------------------
/**
    @name IME enum - Local Chroma Adaption Function
*/
//@{
typedef enum _IME_CHRA_EN
{
  IME_CHRA_DISABLE = 0,         ///< disable local chroma adaption
  IME_CHRA_ENABLE = 1,          ///< enable local chroma adaption
  ENUM_DUMMY4WORD(IME_CHRA_EN)
} IME_CHRA_EN;


typedef enum _IME_CHRA_PRC_SEL
{
  IME_CHRA_PRC_2 = 0,         ///< Local chroma adaption LUT precision 2
  IME_CHRA_PRC_4 = 1,         ///< Local chroma adaption LUT precision 4
  IME_CHRA_PRC_8 = 2,         ///< Local chroma adaption LUT precision 8
  ENUM_DUMMY4WORD(IME_CHRA_PRC_SEL)
} IME_CHRA_PRC_SEL;
//@}


//------------------------------------------------------------------------------
/**
    @name IME enum - Film Grain Function
*/
//@{
typedef enum _IME_FGN_EN
{
  IME_FMGRN_DISABLE = 0,         ///< disable film grain
  IME_FMGRN_ENABLE = 1,          ///< enable file grain
  ENUM_DUMMY4WORD(IME_FGN_EN)
} IME_FGN_EN;
//@}


//------------------------------------------------------------------------------
/**
    @name IME enum - Retinex Processing
*/
//@{
typedef enum _IME_RTX_EN
{
  IME_RTX_DISABLE = 0,         ///< disable retinex processing
  IME_RTX_ENABLE = 1,          ///< enable retinex processing
  ENUM_DUMMY4WORD(IME_RTX_EN)
} IME_RTX_EN;


typedef enum _IME_RTX_MODE
{
  IME_RTX_INNER_MODE = 0,         ///< retinex inner mode
  IME_RTX_OUTER_MODE = 1,        ///< retinex outer mode
  ENUM_DUMMY4WORD(IME_RTX_MODE)
} IME_RTX_MODE;
//@}


//------------------------------------------------------------------------------
/**
    @name IME enum - Color space protection enable
*/
//@{
typedef enum _IME_CSP_EN
{
  IME_CSP_DISABLE = 0,         ///< disable color space protection
  IME_CSP_ENABLE = 1,          ///< enable color space protection
  ENUM_DUMMY4WORD(IME_CSP_EN)
} IME_CSP_EN;
//@}


//------------------------------------------------------------------------------
/**
    @name IME enum - 3D Noise Reduction enable
*/
//@{
typedef enum _IME_TNR_EN
{
  IME_TDNR_DISABLE = 0,         ///< disable 3D noise reduction
  IME_TDNR_ENABLE = 1,          ///< enable 3D noise reduction
  ENUM_DUMMY4WORD(IME_TNR_EN)
} IME_TNR_EN;
//@}

//------------------------------------------------------------------------------

/**
    @name IME enum - Rolling shutter correction enable
*/
//@{
typedef enum _IME_RSC_EN
{
  IME_RSC_DISABLE = 0,         ///< disable rolling shutter correction enable
  IME_RSC_ENABLE = 1,          ///< enable rolling shutter correction enable
  ENUM_DUMMY4WORD(IME_RSC_EN)
} IME_RSC_EN;
//@}


/**
    @name IME enum - Progressive to interlaced operation
*/
//@{
typedef enum _IME_P2I_EN
{
  IME_P2I_DISABLE = 0,         ///< disable progressive to interlaced operation
  IME_P2I_ENABLE = 1,          ///< enable progressive to interlaced operation
  ENUM_DUMMY4WORD(IME_P2I_EN)
} IME_P2I_EN;
//@}

//------------------------------------------------------------------------------
/**
    @name IME enum - Clear Flag
*/
//@{
typedef enum _IME_FLAG_CLEAR_SEL
{
  IME_FLAG_NO_CLEAR = 0,    ///< No clear flag
  IME_FLAG_CLEAR    = 1,    ///< Clear flag
  ENUM_DUMMY4WORD(IME_FLAG_CLEAR_SEL)
} IME_FLAG_CLEAR_SEL;
//@}

//------------------------------------------------------------------------------

/** 
    @name IME enum - Scale enhancement selection
*/
//@{
typedef enum _IME_SCALE_ENH_SEL
{
    IME_SCALE_ENH_WITHOUT_1X = 0,   ///< Scaling enhancement if the ratio of input and output is not the same
    IME_SCALE_ENH_WITH_1X = 1,      ///< Scaling enhancement if the ratio of input and output is the same
    ENUM_DUMMY4WORD(IME_SCALE_ENH_SEL)
} IME_SCALE_ENH_SEL;


typedef enum _IME_SCALE_ENH_MODE
{
    IME_SCALE_ENH_MODE_AUTO = 0,  ///< Scaling enhancement, parameter setting by auto
    IME_SCALE_ENH_MODE_USER = 1,  ///< Scaling enhancement, parameter setting by user
    ENUM_DUMMY4WORD(IME_SCALE_ENH_MODE)
} IME_SCALE_ENH_MODE;
//@}


//------------------------------------------------------------------------------
/**
    @name IME enum - Get Engine Information Selection
*/
//@{
typedef enum _IME_GET_INFO_SEL
{  
  IME_GET_START_STATUS                = 0,      ///< get start status  
  IME_GET_STRIPE_MODE                 = 1,      ///< get stripe mode status
  IME_GET_INPUT_SRC                   = 2,      ///< get input source
  IME_GET_INPUT_FORMAT                = 3,      ///< get input format
  IME_GET_INT_ENABLE                  = 4,      ///< get interrupt enable information
  IME_GET_INT_STATUS                  = 5,       ///< get interrupt status
  IME_GET_BUSY_STATUS                 = 6,       ///< get ime busy status
  IME_GET_LOCKED_STATUS               = 7,       ///< get IME engine locked status
  
  IME_GET_P1_ENABLE_STATUS            = 21,      ///< get output path1 enable status
  IME_GET_P1_OUTPUT_FORMAT            = 22,      ///< get output path1 data format
  IME_GET_P1_OUTPUT_TYPE              = 23,      ///< get output path1 data format type
  IME_GET_P1_OUTPUT_YV_TYPE           = 24,      ///< get output path1 Y/V data
  IME_GET_P1_SCALE_METHOD             = 25,        ///< get output path1 scaling method
  IME_GET_P1_OUTPUT_H_SIZE            = 26,      ///< get output path1 horizontal size
  IME_GET_P1_OUTPUT_V_SIZE            = 27,      ///< get output path1 vertical size
  

  IME_GET_P2_ENABLE_STATUS            = 41,      ///< get output path2 enable status
  IME_GET_P2_OUTPUT_FORMAT            = 42,      ///< get output path2 data format
  IME_GET_P2_OUTPUT_TYPE              = 43,      ///< get output path2 data format type
  IME_GET_P2_OUTPUT_YV_TYPE           = 44,      ///< get output path2 Y/V data
  IME_GET_P2_SCALE_METHOD             = 45,        ///< get output path2 scaling method
  IME_GET_P2_OUTPUT_H_SIZE            = 46,      ///< get output path2 horizontal size
  IME_GET_P2_OUTPUT_V_SIZE            = 47,      ///< get output path2 vertical size
  

  IME_GET_P3_ENABLE_STATUS            = 61,      ///< get output path3 enable status
  IME_GET_P3_OUTPUT_FORMAT            = 62,      ///< get output path3 data format
  IME_GET_P3_OUTPUT_TYPE              = 63,      ///< get output path3 data format type
  IME_GET_P3_OUTPUT_YV_TYPE           = 64,      ///< get output path3 Y/V data
  IME_GET_P3_SCALE_METHOD             = 65,        ///< get output path3 scaling method  
  IME_GET_P3_OUTPUT_H_SIZE            = 66,      ///< get output path3 horizontal size
  IME_GET_P3_OUTPUT_V_SIZE            = 67,      ///< get output path3 vertical size
  
  IME_GET_FUNCTIONS_STATUS            = 91,      ///< get function enable status
  ENUM_DUMMY4WORD(IME_GET_INFO_SEL)

} IME_GET_INFO_SEL;
//@}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//structure data type

/**
    IME structure - Engine open object
*/
//@{
typedef struct _IME_OPENOBJ
{
  void (*FP_IMEISR_CB)(UINT32 intstatus);   ///< External ISR call-back function
  UINT32 uiImeClockSel;                     ///< Engine clock selection
} IME_OPENOBJ,*pIME_OPENOBJ;
//@}


/**
    IME structure - Image size setting structure
*/
//@{
typedef struct _IME_SIZE_INFO
{
  UINT32 SizeH;                  ///< horizontal size
  UINT32 SizeV;                  ///< vertical size
} IME_SIZE_INFO;
//@}


/**
    IME structure - Horizontal and vertical stripe setting structure
*/
//@{
typedef struct _IME_HV_STRIPE_INFO
{
  UINT32 uiHN;     ///< IME input first and middle stripe size for horizontal direction
  UINT32 uiHL;     ///< IME input last stripe size for horizontal direction
  UINT32 uiHM;     ///< IME input stripe number for horizontal direction
  IME_H_STRIPE_OVLP_SEL HOverlapSel;  ///< IME input stripe overlap size selection for horizontal direction
  UINT32 uiHOverlap;  ///< IME input stripe overlap size for horizontal direction
  
  UINT32 uiVN;     ///< IME input first and middle stripe size for vertical direction
  UINT32 uiVL;     ///< IME input last stripe size for vertical direction
  UINT32 uiVM;     ///< IME input stripe number for vertical direction
  IME_STRIPE_MODE StripeCalMode;  ///< IME input stripe information setting mode
} IME_HV_STRIPE_INFO;


typedef struct _IME_STRIPE_INFO
{
    UINT32    uiN;        ///< stripe size in each stripe
    UINT32    uiL;        ///< stripe size of last stripe
    UINT32    uiM;        ///< stripe number
} IME_STRIPE_INFO;

//@}



/**
    IME structure - Scale filter setting structure
*/
//@{
typedef struct _IME_SCALE_FILTER_INFO
{
  IME_SCALE_FILTER_EN        HScaleFilterEnable;           ///< scaling filter enable of horizontal direction
  UINT32                     uiHFilterCoef;     ///< scaling filter parameters of horizontal direction
  IME_SCALE_FILTER_EN        VScaleFilterEnable;           ///< scaling filter enable of vertical direction
  UINT32                     uiVFilterCoef;     ///< scaling filter parameters of vertical direction
  IME_SCALE_FILTER_COEF_MODE CoefCalMode;   ///< filter parameter mode
} IME_SCALE_FILTER_INFO;
//@}

/**
    IME structure - Scale enhancement setting structure
*/
//@{
typedef struct _IME_SCALE_ENH_INFO
{
  UINT32  uiEnhFactor;         ///< scaling enhancement factor
  UINT32  uiEnhBit;            ///< scaling enhancement normailizaion bit
  IME_SCALE_ENH_SEL  EnhType;   ///< scaling enhancement type
  IME_SCALE_ENH_MODE EnhMode;  ///< scaling enhancement mode
} IME_SCALE_ENH_INFO;
//@}




/**
    IME structure - DMA address setting structure
*/
//@{
typedef struct _IME_DMA_ADDR_INFO
{
  UINT32 uiAddrY;    ///< DMA address of Y channel
  UINT32 uiAddrCb;   ///< DMA address of Cb channel
  UINT32 uiAddrCr;   ///< DMA address of Cr channel
} IME_DMA_ADDR_INFO;
//@}

/**
    IME structure - Lineoffset setting structure
*/
//@{
typedef struct _IME_LINEOFFSET_INFO
{
  UINT32 uiLineOffsetY;   ///< line-offset of Y channel
  UINT32 uiLineOffsetCb;  ///< line-offset of Cb channel
} IME_LINEOFFSET_INFO;
//@}

/**
    IME structure - Input DMA address setting structure
*/
//@{
typedef struct _IME_INPUT_DMA_INFO
{
  IME_IPPB_EN       InBufEnable;    ///< input ping-pong buffer enable
  IME_SET_IN_BUF_SEL InBufSel;  ///< select to set which ping-pong buffer address
  IME_DMA_ADDR_INFO  InDMA0;     ///< input DMA channel0 address
  IME_DMA_ADDR_INFO  InDMA1;     ///< input DMA channel1 address
} IME_INPUT_DMA_INFO;
//@}

/**
    IME structure - Output DMA address setting structure
*/
//@{
typedef struct _IME_OUTPUT_DMA_INFO
{
  IME_PPB_NUM_SEL   OutBufNums;   ///< output buffer numbers selection
  IME_DMA_ADDR_INFO  OutDMA0;      ///< output DMA channel address of buffer 0
} IME_OUTPUT_DMA_INFO;
//@}


/**
  IME structure - Output DRAM enable setting structure
*/
//@{
typedef struct _IME_PATH_OUT_DRAM_EN
{
  UINT32           uiWhichPath;     ///< select which path is enabled, please use enum "IME_PATH_SEL" to select, for example WhichPath = (IME_PATH1_SEL | IME_PATH2_SEL)
  IME_OUT_DRAM_EN  P1OutDramEnable;  ///< path1 enable
  IME_OUT_DRAM_EN  P2OutDramEnable;  ///< path2 enable
  IME_OUT_DRAM_EN  P3OutDramEnable;  ///< path3 enable
} IME_PATH_OUT_DRAM_EN;
//@}

/**
  IME structure - Output path enable setting structure
*/
//@{
typedef struct _IME_PATH_OUT_EN
{
  UINT32              uiWhichPath;     ///< select which path is enabled, please use enum "IME_PATH_SEL" to select, for example WhichPath = (IME_PATH1_SEL | IME_PATH2_SEL)
  IME_OUTPUT_PATH_EN  P1Enable;  ///< path1 enable
  IME_OUTPUT_PATH_EN  P2Enable;  ///< path2 enable
  IME_OUTPUT_PATH_EN  P3Enable;  ///< path3 enable
} IME_PATH_OUT_EN;
//@}



/**
  IME structure - Chroma adaption input buffer address
*/
//@{
typedef struct _IME_CHRA_DMA_INFO
{
  IME_SET_IN_BUF_SEL LcaSetBufSel;   ///< select to set which buffer address
  UINT32             uiLcaBufAddr0;    ///< reference layer input ping-pong buffer0 address
  UINT32             uiLcaBufAddr1;    ///< reference layer input ping-pong buffer1 address  
} IME_CHRA_DMA_INFO;

/**
  IME structure - Retinex input buffer address
*/
//@{
typedef struct _IME_RTX_DMA_INFO
{
  IME_SET_IN_BUF_SEL RtxSetBufSel;   ///< select to set which buffer address
  UINT32 uiRtxBufAddr0;         ///< retinex input ping-pong buffer 0
  UINT32 uiRtxBufAddr1;         ///< retinex input ping-pong buffer 1  
} IME_RTX_DMA_INFO;





//=====================================================================================
// IQ function structure
/**
    IME structure - Color space transformation setting structure
*/
//@{
typedef struct _IME_CST_INFO
{
  IME_CST_EN   Rgb2YccEn; ///< Color space transformation enable, RGB to YCbCr
  IME_CST_EN   Ycc2RgbEn; ///< Color space transformation enable, YcbCr to RGB
} IME_CST_INFO;
//@}

/**
    IME structure - Chroma filter parameter setting structure
*/
//@{
typedef struct _IME_CHROMA_FILTER_INFO
{
  IME_CHRF_EN       ChrfEnable;        ///< chroma filter enable
  IME_CHRF_RANGE_EN ChrfRangeEnable;     ///< chroma filter - range domain enable
  UINT32            uiChrfSPW0;      ///< chroma filter - spatial domain weighting0
  UINT32            uiChrfSPW1;      ///< chroma filter - spatial domain weighting1
  UINT32            uiChefCoef;      ///< chroma filter - filtered Coefficient
  UINT32            *puiChrfRangeCbThs; ///< chroma filter - range threshold for Cb channel, 4 elements
  UINT32            *puiChrfRangeCrThs; ///< chroma filter - range threshold for Cr channel, 4 elements
  UINT32            *puiChrfRangeWts;   ///< chroma filter - range weightings, 5 elements
} IME_CHROMA_FILTER_INFO;
//@}


/**
    IME structure - Local Chroma Adaption parameter setting structure
*/
//@{
typedef struct _IME_CHROMA_ADAPTION_INFO
{
  IME_CHRA_EN           LcaEnable;          ///< loca chroma adaption enable
  IME_SIZE_INFO         OriImgSize;     ///< input image size
  IME_SIZE_INFO         LcaImgSize;     ///< input image size
  UINT32                uiLcaCbTh;        ///< local chroma adaption - threshold for Cb
  UINT32                uiLcaCrTh;        ///< local chroma adaption - threshold for Cr
  UINT32                uiLcaLutTh;       ///< local chroma adaption - LUT threshold
  UINT32                uiLcaWeight;      ///< local chroma adaption - reference layer weight
  IME_CHRA_PRC_SEL      LcaPrc;         ///< local chroma adaption - range domain enable
  UINT32                uiLcaLineoffset;  ///< local chroma adaption - reference layer input lineoffset
  IME_IPPB_EN           LcaPPBEnable;       ///< local chroma adaption - input ping-pong buffer enable  
  IME_CHRA_DMA_INFO     LcaBuf;         ///< local chroma adaption - input ping-pong buffer
  UINT32                *puiLcaLUT;        ///< local chroma adaption - 33 elements LUT
} IME_CHROMA_ADAPTION_INFO;
//@}


/**
    IME structure - Retinex processing parameter setting structure
*/
//@{
typedef struct _IME_RETINEX_PROC_INFO
{
  IME_RTX_EN RtxEnable;          ///< retinex processing enable
  IME_RTX_MODE RtxMode;      ///< retinex operation mode
  UINT32 uiInWidth;             ///< input image width
  UINT32 uiInHeight;            ///< input image height
  UINT32 uiRtxWidth;            ///< retinex reference image width
  UINT32 uiRtxHeight;           ///< retinex reference image height
  UINT32 uiRtxLineoffset;       ///< retinex reference image lineoffset
  IME_IPPB_EN RtxPPBEnable;      ///< retinex input ping-pong buffer enable
  IME_RTX_DMA_INFO RtxBuf;    ///< retinex input ping-pong buffer
  UINT32 uiRtxGblLitGain;       ///< retinex global gain for lighting part
  UINT32 uiRtxGblDrkGain;       ///< retinex global gain for dark part
  UINT32 uiRtxLclLitGain;       ///< retinex local gain for lighting part
  UINT32 uiRtxLclDrkGain;       ///< retinex local gain for dark part
  UINT32 uiRtxColorGainR;       ///< retinex color gain for R channel
  UINT32 uiRtxColorGainG;       ///< retinex color gain for G channel
  UINT32 uiRtxColorGainB;       ///< retinex color gain for B channel
  UINT32 uiRtxExagGain;         ///< retinex color gain for exaggerating effect
} IME_RETINEX_PROC_INFO;
//@}




/**
    IME structure - Film grain parameter setting structure
*/
//@{
typedef struct _IME_FILM_GRAIN_INFO
{
  IME_FGN_EN    FgnEnable;    ///< film grain enable
  UINT32        uiFgnLumNoiseDifTh;  ///< luminance-Noise difference threshold
  UINT32        uiFgnLumTh;          ///< luminance value threshold
  UINT32        uiFgnWtsP1;           ///< path1 weighting between pixel and noise value
  UINT32        uiFgnInitP1;          ///< Grain noise initial value for path1
  UINT32        uiFgnWtsP2;           ///< path2 weighting between pixel and noise value
  UINT32        uiFgnInitP2;          ///< Grain noise initial value for path2
  UINT32        uiFgnWtsP3;           ///< path3 weighting between pixel and noise value
  UINT32        uiFgnInitP3;          ///< Grain noise initial value for path3
} IME_FILM_GRAIN_INFO;
//@}

/**

*/
//@{
typedef struct _IME_CSP_INFO
{
  IME_CSP_EN CspEnable;  ///< color space protection enable
  UINT32 uiCspRatio;
} IME_CSP_INFO;
//@}

/**
    IME structure - 3D Noise Reduction structure
*/
//@{
typedef struct _IME_3DNR_INFO
{
  IME_TNR_EN TnrEnable;   ///< enable / disable 3D noise reduction
  UINT32 *puiTnrThY;     ///< thresholds for Y channel, 3 elements
  UINT32 *puiTnrWtsY;    ///< weightings for Y channel, 4 elements
  UINT32 *puiTnrThCb;     ///< thresholds for Cb channel, 3 elements
  UINT32 *puiTnrWtsCb;    ///< weightings for Cb channel, 4 elements
  UINT32 *puiTnrThCr;     ///< thresholds for Cr channel, 3 elements
  UINT32 *puiTnrWtsCr;    ///< weightings for Cr channel, 4 elements
  BOOL   bTnrIsSetBuf;  ///< set input buffer address or not
  UINT32 uiTnrAddrY;         ///< input Y channel
  UINT32 uiTnrAddrC;         ///< intput C channel
} IME_3DNR_INFO;
//@}

/**
    IME structure - Rolling shutter correction structure
*/
//@{
#if (_EMULATION_ == ENABLE)

typedef struct _IME_RSC_INFO
{
  IME_RSC_EN RscEnable;      ///< RSC enable / disable
  IME_RSC_EN RscHEnable;      ///< RSC enable / disable, only for FPGA verification
  IME_RSC_EN RscVEnable;      ///< RSC enable / disable, only for FPGA verification
  UINT32 uiRscCPFtrP1;     ///< RSC control-point interpolation factor for Path1
  UINT32 uiRscCPGainHP1;  ///< RSC Control-Point GAIN Factor for Path1
  UINT32 uiRscCPFtrP2;     ///< RSC control-point interpolation factor for Path2
  UINT32 uiRscCPGainHP2;  ///< RSC Control-Point GAIN Factor for Path1
  UINT32 uiRscCPFtrPos;     ///< RSC control-point interpolation position
  UINT32 *puiRscCPShftH;     ///< RSC control-point horizontal shift, 12 elements
  UINT32 *puiRscCPShftV;     ///< RSC control-point vertical shift, 12 elements
} IME_RSC_INFO;

#else

typedef struct _IME_RSC_INFO
{
  IME_RSC_EN RscEnable;      ///< RSC enable / disable
  UINT32 uiRscCPFtrP1;     ///< RSC control-point interpolation factor for Path1
  UINT32 uiRscCPGainHP1;  ///< RSC Control-Point GAIN Factor for Path1
  UINT32 uiRscCPFtrP2;     ///< RSC control-point interpolation factor for Path2
  UINT32 uiRscCPGainHP2;  ///< RSC Control-Point GAIN Factor for Path1
  UINT32 uiRscCPFtrPos;     ///< RSC control-point interpolation position
  UINT32 *puiRscCPShftH;     ///< RSC control-point horizontal shift, 12 elements
  UINT32 *puiRscCPShftV;     ///< RSC control-point vertical shift, 12 elements
} IME_RSC_INFO;

#endif
//@}

/**
    IME structure - Rolling shutter margin structure
*/
//@{
typedef struct _IME_RSC_MARGIN_INFO
{
  IME_RSC_EN  RscMarginCtrlEnable;  ///< RSC margin enable
  UINT32 uiRscMarginH;            ///< RSC margin horizontal size
  UINT32 uiRscMarginV;            ///< RSC margin vertical size
} IME_RSC_MARGIN_INFO;
//@}



//------------------------------------------------------------------------------
// global flow structure

/**
    IME structure - Engine IQ setting structure
*/
//@{
typedef struct _IME_IQ_FLOW_INFO
{ 
  IME_CHROMA_FILTER_INFO    *pChromaFilterInfo;   ///< chroma filter parameters 
  IME_CHROMA_ADAPTION_INFO  *pChromaAdaptionInfo; ///< chroma adaption parameters  
  IME_RETINEX_PROC_INFO     *pRetinexProcInfo;    ///< retinex processing prarmeters  
  IME_CST_INFO              *pColorSpaceTrans;    ///< color space transformation parameters
  IME_CSP_INFO              *pColorSpaceProt;     ///< color space protection
  IME_RSC_INFO              *pRSCInfo;            ///< rolling shutter correction parameters  
  IME_FILM_GRAIN_INFO       *pFilmGrainInfo;      ///< film grain parameters
  IME_3DNR_INFO             *pTnrInfo;           ///< 3D-NR parameters
  IME_P2I_EN                *pP2IInfo;            ///< progressive to interlaced parameter
} IME_IQ_FLOW_INFO;
//@}


/**
    IME structure - Output format setting structure
*/
//@{
typedef struct _IME_OUTPUT_FORMAT_INFO
{
    IME_OUTPUT_IMG_FORMAT_SEL  OutFormatSel;    ///< path output format
    IME_OUTPUT_FORMAT_TYPE     OutFormatTypeSel;  ///< paht output format type
} IME_OUTPUT_FORMAT_INFO;
//@}



/**
    IME structure - Input and output lineoffset setting structure
*/
//@{
typedef struct _IME_LINEOFS_INFO
{
    IME_LINEOFFSET_INFO     InLineoffset;        ///< input line offset
    IME_LINEOFFSET_INFO     P1OutLineoffset;    ///< output path1 lineoffset
    IME_LINEOFFSET_INFO     P2OutLineoffset;    ///< output path2 lineoffset
    IME_LINEOFFSET_INFO     P3OutLineoffset;    ///< output path2 lineoffset
}IME_LINEOFS_INFO;
//@}

/**
    IME structure - Output path operation setting structure
*/
//@{
typedef struct _IME_PATH_SWITCH
{
  IME_OUTPUT_PATH_EN PathEnable;
} IME_PATH_SWITCH;
//@}

/**
    IME structure - Output path crop Coordinate setting structure
*/
//@{
typedef struct _IME_CROP_INFO
{
  UINT32 uiCropX;
  UINT32 uiCropY;
} IME_CROP_INFO;
//@}


/**
    IME structure - Scaling factor
*/
//@{
typedef struct _IME_SCALE_FACTOR
{
  UINT32 uiScaleFactorH;
  UINT32 uiScaleFactorV;
} IME_SCALE_FACTOR;
//@}



/**
    IME structure - Engine flow setting structure
*/
//@{
typedef struct _IME_MODE_PRAM
{
  // input info
  IME_MODE_SEL              OperationMode;               ///< IME performing mode
  
  IME_SIZE_INFO             InSize;               ///< input image size
  IME_INPUT_FORMAT_SEL      InFormat;                ///< input image format
  IME_LINEOFFSET_INFO       InLineoffset;            ///< input line offset
  IME_INPUT_DMA_INFO        InAddr;               ///< input DMA address

  //output info
  IME_PATH_SWITCH           P1Enable;             ///< path1 enable
  IME_OUT_DRAM_EN           P1OutDramEnable;          ///< path1 output to dram enable
  IME_OUTPUT_FORMAT_INFO    P1OutFormat;            ///< path1 output image format
  IME_SIZE_INFO             P1OutScaleSize;     ///< path1 image scaling output size    
  IME_SCALE_FILTER_INFO     P1OutScaleFilter;   ///< path1 scaling filter parameters
  IME_SCALE_METHOD_SEL      P1OutScaleMethod;   ///< path1 scale method
  IME_SCALE_ENH_INFO        P1OutScaleEnh;      ///< path1 scale enhancement, for bicubic scaling method only
  IME_OUT_YCHL_TYPE_SEL     P1OutYChannelType;  ///< path1 output y channel data type selection
  IME_OUTPUT_DMA_INFO       P1OutAddr;           ///< path1 output DMA address
  IME_CROP_INFO             P1CropPos;           ///< path1 crop position
  IME_SIZE_INFO             P1OutSize;           ///< path1 image crop output size
  IME_LINEOFFSET_INFO       P1OutLineoffset;        ///< path1 output lineoffset
  IME_RSC_MARGIN_INFO       P1OutRscMargin;     ///< path1 rsc margin 
  

  IME_PATH_SWITCH           P2Enable;             ///< path2 enable
  IME_OUT_DRAM_EN           P2OutDramEnable;          ///< path2 output to dram enable
  IME_OUTPUT_FORMAT_INFO    P2OutFormat;            ///< path2 output image format
  IME_SIZE_INFO             P2OutScaleSize;     ///< path2 image scaling output size
  IME_SCALE_FILTER_INFO     P2OutScaleFilter;   ///< path2 scaling filter parameters
  IME_SCALE_METHOD_SEL      P2OutScaleMethod;   ///< path2 scale method
  IME_SCALE_ENH_INFO        P2OutScaleEnh;      ///< path2 scale enhancement, for bicubic scaling method only
  IME_OUT_YCHL_TYPE_SEL     P2OutYChannelType;  ///< path2 output y channel data type selection
  IME_OUTPUT_DMA_INFO       P2OutAddr;           ///< path2 output DMA address  
  IME_CROP_INFO             P2CropPos;           ///< path2 crop position
  IME_SIZE_INFO             P2OutSize;           ///< path2 image crop output size
  IME_LINEOFFSET_INFO       P2OutLineoffset;        ///< path2 output lineoffset
  IME_RSC_MARGIN_INFO       P2OutRscMargin;     ///< path1 rsc margin 
  
  
  IME_PATH_SWITCH           P3Enable;             ///< path3 enable
  IME_OUT_DRAM_EN           P3OutDramEnable;          ///< path3 output to dram enable
  IME_OUTPUT_FORMAT_INFO    P3OutFormat;            ///< path3 output image format
  IME_SIZE_INFO             P3OutScaleSize;     ///< path3 image scaling output size
  IME_SCALE_FILTER_INFO     P3OutScaleFilter;   ///< path3 scaling filter parameters
  IME_SCALE_METHOD_SEL      P3OutScaleMethod;   ///< path3 scale method
  IME_SCALE_ENH_INFO        P3OutScaleEnh;      ///< path3 scale enhancement, for bicubic scaling method only
  IME_OUT_YCHL_TYPE_SEL     P3OutYChannelType;  ///< path3 output y channel data type selection
  IME_OUTPUT_DMA_INFO       P3OutAddr;           ///< path3 output DMA address  
  IME_CROP_INFO             P3CropPos;           ///< path3 crop position
  IME_SIZE_INFO             P3OutSize;           ///< path3 image crop output size 
  IME_LINEOFFSET_INFO       P3OutLineoffset;        ///< path3 output lineoffset
  
  UINT32                    uiInterruptEnable;          ///< interrupt enable  
  
  IME_IQ_FLOW_INFO          *pImeIQInfo;            ///< IQ related parameters
}IME_MODE_PRAM,*pIME_MODE_PRAM;
//@}


/**
    IME structure - Change input/output image size structure
*/
//@{
typedef struct _IME_IO_CHG_SIZE
{
  IME_SIZE_INFO        InSize;            ///< input image size
  IME_LINEOFFSET_INFO  InLineoffset;         ///< input line offset
  
  UINT32               uiUpdateWhichPath;  ///< selected output path for update parameters, update_which_path = (IME_PATH1_SEL | IME_PATH2_SEL)
  
  IME_SIZE_INFO        P1OutScaleSize;  ///< output image size of path1
  IME_CROP_INFO        P1CropPos;        ///< crop position of path1
  IME_SIZE_INFO        P1OutSize;        ///< output image size of path1
  IME_LINEOFFSET_INFO  P1OutLineoffset;     ///<  output lineoffset of path1
  IME_SCALE_METHOD_SEL P1OutScaleMethod;  ///<  output scaling method for path1
  IME_RSC_MARGIN_INFO  P1OutRscMargin;     ///< rsc margin for path1
  
  IME_SIZE_INFO        P2OutScaleSize;  ///< output image size of path2
  IME_CROP_INFO        P2CropPos;        ///< crop position of path2
  IME_SIZE_INFO        P2OutSize;        ///< output image size of path2
  IME_LINEOFFSET_INFO  P2OutLineoffset;     ///<  output lineoffset of path2
  IME_SCALE_METHOD_SEL P2OutScaleMethod;  ///<  output scaling method for path2
  IME_RSC_MARGIN_INFO  P2OutRscMargin;     ///< rsc margin for path2

  IME_SIZE_INFO        P3OutScaleSize;  ///< output image size of path3
  IME_CROP_INFO        P3CropPos;        ///< crop position of path3
  IME_SIZE_INFO        P3OutSize;        ///< output image size of path3
  IME_LINEOFFSET_INFO  P3OutLineoffset;     ///<  output lineoffset of path3
  IME_SCALE_METHOD_SEL P3OutScaleMethod;  ///<  output scaling method for path3
  
} IME_IO_CHG_SIZE;
//@}

/**
  IME structure - Change output image format parameters
*/
typedef struct _IME_CHG_OUTPUT_FMT
{
 UINT32 uiUpdateWhichPath;                     ///< selected output path for update parameters, update_which_path = (IME_PATH1_SEL | IME_PATH2_SEL)

 IME_OUTPUT_FORMAT_INFO P1OutFormat;            ///< path1 output image format
 IME_OUT_YCHL_TYPE_SEL  P1OutYChannelType;  ///< path1 output y channel data type selection
 IME_LINEOFFSET_INFO    P1OutLineoffset;        ///< path1 output lineoffset

 IME_OUTPUT_FORMAT_INFO P2OutFormat;            ///< path2 output image format
 IME_OUT_YCHL_TYPE_SEL  P2OutYChannelType;  ///< path2 output y channel data type selection
 IME_LINEOFFSET_INFO    P2OutLineoffset;        ///< path2 output lineoffset

 IME_OUTPUT_FORMAT_INFO P3OutFormat;            ///< path3 output image format
 IME_OUT_YCHL_TYPE_SEL  P3OutYChannelType;  ///< path3 output y channel data type selection
 IME_LINEOFFSET_INFO    P3OutLineoffset;        ///< path3 output lineoffset
} IME_CHG_OUTPUT_FMT;

/**
    IME structure - Change output scaling enhancement parameters
*/
//@{
typedef struct _IME_OUTPUT_SCALE_ENH
{  
  UINT32               uiUpdateWhichPath;  ///< selected output path for update parameters, update_which_path = (IME_PATH1_SEL | IME_PATH2_SEL)
  IME_SCALE_ENH_INFO   P1OutScaleEnh;   ///< scale enhancement of path1
  IME_SCALE_ENH_INFO   P2OutScaleEnh;   ///< scale enhancement of path2
  IME_SCALE_ENH_INFO   P3OutScaleEnh;   ///< scale enhancement of path3 
} IME_OUTPUT_SCALE_ENH;
//@}



/**
    IME structure - Change output scaling filter parameters
*/
//@{
typedef struct _IME_OUTPUT_SCALE_FILTER
{  
  UINT32                  uiUpdateWhichPath;  ///< selected output path for update parameters, update_which_path = (IME_PATH1_SEL | IME_PATH2_SEL)
  IME_SCALE_FILTER_INFO   P1OutScaleFilter;   ///< scale filter of path1
  IME_SCALE_FILTER_INFO   P2OutScaleFilter;   ///< scale filter of path2
  IME_SCALE_FILTER_INFO   P3OutScaleFilter;   ///< scale filter of path3 
} IME_OUTPUT_SCALE_FILTER;
//@}


/**
    IME structure - Input DRAM Address structure
*/
//@{
typedef struct _IME_CHG_INPUT_DRAM_INFO
{
  IME_INPUT_DMA_INFO    InAddress;      ///< intput address
  UINT32                uiLineoffsetY;  ///< y channel lineoffset
  UINT32                uiLineoffsetC;  ///< cb/cr channel lineoffset
  UINT32                uiHeight;       ///< image height
  IME_INPUT_FORMAT_SEL  InFormat;       ///< input format selection
} IME_CHG_INPUT_DRAM_INFO;
//@}


/**
    IME structure - Output DRAM Address structure
*/
//@{
typedef struct _IME_CHG_OUTPUT_DRAM_INFO
{
  IME_OUTPUT_DMA_INFO       OutAddress;     ///< output address
  UINT32                    uiLineoffsetY;  ///< y channel lineoffset
  UINT32                    uiLineoffsetC;  ///< cb/cr channel lineoffset
  UINT32                    uiHeight;       ///< image height
  IME_OUTPUT_IMG_FORMAT_SEL OutFormat;      ///< output format selection
  IME_OUTPUT_FORMAT_TYPE    OutFormatType;  ///< output format type selection
} IME_CHG_OUTPUT_DRAM_INFO;
//@}


/**
    IME structure - Change Input/output DMA Address structure
*/
//@{
typedef struct _IME_IO_CHG_ADDR
{  
  IME_CHG_INPUT_DRAM_INFO   *InAddr;        ///< input DMA address  
  IME_CHG_OUTPUT_DRAM_INFO  *pP1OutAddr;    ///< output path1 DMA address  
  IME_CHG_OUTPUT_DRAM_INFO  *pP2OutAddr;    ///< output path2 DMA address  
  IME_CHG_OUTPUT_DRAM_INFO  *pP3OutAddr;    ///< output path3 DMA address
} IME_IO_CHG_ADDR;
//@}

/**
    IME structure - Get Overlap size
*/
//@{
typedef struct _IME_GET_OVERLAP_SIZE_INFO
{  
    IME_SIZE_INFO           InSize;   // input image size

    IME_PATH_SWITCH         P1Enable;             ///< path1 enable
    IME_SIZE_INFO           P1OutScaleSize;     ///< path1 image scaling output size    
    IME_SCALE_METHOD_SEL    P1OutScaleMethod;   ///< path1 scale method

    IME_PATH_SWITCH         P2Enable;             ///< path2 enable
    IME_SIZE_INFO           P2OutScaleSize;     ///< path2 image scaling output size    
    IME_SCALE_METHOD_SEL    P2OutScaleMethod;   ///< path2 scale method

    IME_PATH_SWITCH         P3Enable;             ///< path3 enable
    IME_SIZE_INFO           P3OutScaleSize;     ///< path3 image scaling output size    
    IME_SCALE_METHOD_SEL    P3OutScaleMethod;   ///< path3 scale method

    IME_RTX_EN              RtxEnable;                 ///< retinex processing enable
    IME_RTX_MODE            RtxMode;               ///< retinex operation mode

    IME_RSC_EN              RscEnable;                 ///< RSC enable / disable
  
} IME_GET_OVERLAP_SIZE_INFO;
//@}



//@}

//------------------------------------------------------------------------------
// flow control APIs
extern BOOL ime_isOpen(VOID);

/*
  Open IME engine

  This function should be called before calling any other functions.

  @param PIME_OpenInfo ISR callback function and clock selection.
  -@b Supported clocks: 48, 60, 80, 120, 160, 192, 220Mhz

  @return
        - @b E_OK: Done without error.
        - Others: Error occured.
*/
extern ER ime_open(IME_OPENOBJ *pImeOpenInfo);


extern ER ime_setMode(IME_MODE_PRAM *pEngineInfo);
extern ER ime_setStart(VOID);
extern ER ime_setReset(VOID);
extern ER ime_pause(VOID);
extern ER ime_close(VOID);

extern VOID ime_setUserStripeInfo(IME_HV_STRIPE_INFO *pStripeInfo);
extern IME_H_STRIPE_OVLP_SEL ime_getOverlapSize(IME_GET_OVERLAP_SIZE_INFO *pOverlapSizeInfo);

//------------------------------------------------------------------------------
// change engine parameter APIs
extern ER ime_chgInputAndOutputImageSize(IME_IO_CHG_SIZE *pIOSizeParam);
extern ER ime_chgOutputImageFormat(IME_CHG_OUTPUT_FMT *pOutFormatParam);
extern ER ime_chgInputAndOutputDmaAddress(IME_IO_CHG_ADDR *pIOAddrParam);
extern ER ime_chgOutputPathDramOutEnable(IME_PATH_OUT_DRAM_EN *pDramOutEnable);
extern ER ime_chgOutputPathEnable(IME_PATH_OUT_EN *pOutPathEnable);
extern ER ime_chgOutputScalingEnhancement(IME_OUTPUT_SCALE_ENH *pOutScaleEnhParam);
extern ER ime_chgAllFunctionsEnable(UINT32 uiFunEnable);
extern ER ime_chgColorSpaceTransformParam(IME_CST_INFO *pCstParam);
extern ER ime_chgColorSpaceProtectionParam(IME_CSP_INFO *pCspParam);
extern ER ime_chgChromaFilterParam(IME_CHROMA_FILTER_INFO *pChrfParam);
extern ER ime_chgChromaAdaptionParam(IME_CHROMA_ADAPTION_INFO *pChraParam);
extern ER ime_chgChromaAdaptionAddrs(UINT32 *puiChraAddr0, UINT32 *puiChraAddr1, IME_SET_IN_BUF_SEL *pBufSel);
extern ER ime_chgRetinexProcessingParam(IME_RETINEX_PROC_INFO *pRtxParam);
extern ER ime_chgRetinexProcessingAddrs(UINT32 *puiRtxAddr0, UINT32 *puiRtxAddr1, IME_SET_IN_BUF_SEL *pBufSel);
extern ER ime_chgFilmGrainNoiseParam(IME_FILM_GRAIN_INFO *pFgnParam);
extern ER ime_chgRollingShutterCorrectionParam(IME_RSC_INFO *pRscParam);
extern ER ime_chg3DNoiseReductionParam(IME_3DNR_INFO *pTnrParam);
extern ER ime_chg3DNoiseReductionAddrs(UINT32 *puiTnrAddrY, UINT32 *puiTnrAddrC);
extern ER ime_chgProgressiveToInterlaceParam(IME_P2I_EN *pEnable);
extern ER ime_chgOutputScalingFilter(IME_OUTPUT_SCALE_FILTER *pOutScaleFilterParam);

//------------------------------------------------------------------------------
// flag related functions
extern UINT32 ime_waitDone(VOID);
extern ER ime_waitFlagFrameEnd(IME_FLAG_CLEAR_SEL IsClearFlag);
extern VOID ime_clearFlagFrameEnd(VOID);
extern VOID ime_clrInterruptStatus(UINT32 uiIntrpStatus);
extern ER ime_clrIntFlag(VOID);


//---------------------------------------------------------------------------------------------
// getting engine info. functions
extern IME_STRIPE_INFO ime_getHstripe(VOID);
extern IME_STRIPE_INFO ime_getVstripe(VOID);
extern UINT32 ime_getInterruptStatus(VOID);
extern ER ime_getInputLineoffsetInfo(IME_LINEOFFSET_INFO *pInLineoffset);
extern UINT32 ime_getEngineInfo(IME_GET_INFO_SEL GetImeInfoSel);
extern ER ime_getInputDMAAddrInfoBuf0(IME_DMA_ADDR_INFO *pInAddr);
extern ER ime_getInputDMAAddrInfoBuf1(IME_DMA_ADDR_INFO *pInAddr);

//---------------------------------------------------------------------------------------------
// getting output path1 info.
extern ER ime_getOutputDMAAddrInfoPath1(IME_DMA_ADDR_INFO *pOutAddr, IME_PPB_NUM_SEL BufNumSel);
extern ER ime_getOutputLineoffsetInfoPath1(IME_LINEOFFSET_INFO *pOutLineoffset);
extern UINT32 ime_getOutputPath1Addr(IME_BUFNUM_SEL BufNumSel, IME_CHANNEL_SEL ChannelSel);
extern UINT32 ime_getActOutputPath1Addr(IME_CHANNEL_SEL ChannelSel);


//---------------------------------------------------------------------------------------------
// getting output path2 info.
extern ER ime_getOutputDMAAddrInfoPath2(IME_DMA_ADDR_INFO *pOutAddr, IME_PPB_NUM_SEL BufNumSel);
extern ER ime_getOutputLineoffsetInfoPath2(IME_LINEOFFSET_INFO *pOutLineoffset);
extern UINT32 ime_getOutputPath2Addr(IME_BUFNUM_SEL BufNumSel, IME_CHANNEL_SEL ChannelSel);
extern UINT32 ime_getActOutputPath2Addr(IME_CHANNEL_SEL ChannelSel);


//---------------------------------------------------------------------------------------------
// getting output path3 info.
extern ER ime_getOutputDMAAddrInfoPath3(IME_DMA_ADDR_INFO *pOutAddr, IME_PPB_NUM_SEL BufNumSel);
extern ER ime_getOutputLineoffsetInfoPath3(IME_LINEOFFSET_INFO *pOutLineoffset);
extern UINT32 ime_getOutputPath3Addr(IME_BUFNUM_SEL BufNumSel, IME_CHANNEL_SEL ChannelSel);
extern UINT32 ime_getActOutputPath3Addr(IME_CHANNEL_SEL ChannelSel);


//---------------------------------------------------------------------------------------------
// getting info. API for IQ related functions

extern UINT32 ime_getChromaAdaptionAddrInfo(IME_GET_IN_BUF_SEL DmaNumSel);
extern VOID ime_getChromaAdaptionImgSizeInfo(UINT32 *puiSizeH, UINT32 *puiSizeV, UINT32 *puiLineoffset);
extern UINT32 ime_getRetinexProcessingAddrInfo(IME_GET_IN_BUF_SEL DmaNumSel);
extern IME_RTX_MODE ime_getRetinexProcessingModeInfo(VOID);
extern VOID ime_getRetinexProcessingImgSizeInfo(UINT32 *puiSizeH, UINT32 *puiSizeV, UINT32 *puiLineoffset);
extern ER ime_get3DNoiseReductionAddrInfo(UINT32 *puiTnrAddrY, UINT32 *puiTnrAddrC);

extern IME_SCALE_FACTOR ime_getScalingFactor(IME_PATH_SEL PathSel);

extern UINT32 ime_getClockRate(VOID);

// reserved only for backward compatible, without such a function
//extern VOID ime_setSliceHeight(_IME_SLICE_HEIGHT_SEL uiSliceHeight);
// reserved only for backward compatible, without such a function
extern ER ime_triggerSlice(VOID); 
// reserved only for backward compatible, without such a function
extern ER ime_waitFalgSliceDone(IME_FLAG_CLEAR_SEL IsClearFlag);
// reserved only for backward compatible, without such a function
extern VOID ime_clrFlagSliceDone(VOID);




#endif
