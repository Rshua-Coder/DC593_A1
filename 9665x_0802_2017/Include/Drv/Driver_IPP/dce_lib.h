/**
    Public header file of DCE engine

    This file is the public hearder file of DCE engine, \n
    including APIs' declaration and data type's definition
    
    @file              dce_lib.h
    
    @ingroup       mIIPPDCE      
    
    @notes           None

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _DCE_LIB_H_
#define _DCE_LIB_H_

#include "Type.h"
#include "ErrorNo.h"

//*********************************************************************************************************

/**
    @addtogroup mIIPPDCE
*/
//@{

/**
    @name DCE engine - Address offset
*/
//@{
#define DCE_HSIZE_IODIFF  8    ///< Basic DMA address offset
//@}


/**
    @name DCE engine - Image Size Unit
*/
//@{
#define DCE_SIZE_UNIT  8    ///< Basic image size unit
//@}


/**
    @name DCE engine - Interrput and releated enable define
*/
//@{
#define DCE_INT_FRMST     0x00000001    ///< Start of frame
#define DCE_INT_FRMEND    0x00000002    ///< End of frame
#define DCE_INT_STEND     0x00000004    ///< End of current stripe
#define DCE_INT_OVFL1     0x00000008    ///< Internal buffer overflow, 8 column or cross channel difference
#define DCE_INT_OVFL2     0x00000010    ///< Internal buffer overflow, same channel backward skip
#define DCE_INT_ALL       (DCE_INT_FRMST | DCE_INT_FRMEND | DCE_INT_STEND | DCE_INT_OVFL1 | DCE_INT_OVFL2)

#define DCE_INTE_FRMST    0x00000001    ///< Start of frame interrupt enable
#define DCE_INTE_FRMEND   0x00000002    ///< End of frame interrupt enable
#define DCE_INTE_STEND    0x00000004    ///< End of current stripe interrupt enable
#define DCE_INTE_OVFL1    0x00000008    ///< Internal buffer overflow, 8 column or cross channel difference interrupt enable
#define DCE_INTE_OVFL2    0x00000010    ///< Internal buffer overflow, same channel backward skip interrupt enable
#define DCE_INTE_ALL      (DCE_INTE_FRMST | DCE_INTE_FRMEND | DCE_INTE_STEND | DCE_INTE_OVFL1 | DCE_INTE_OVFL2)

//@}




/**
    @name DCE engine - Stop via ISR massage define
*/
//@{
//#NT#2010/02/11#Linkin Huang -begin
//#NT# add to stop DCE via ISR

//#NT#2010/05/11#Linkin Huang -begin
//#NT# for closing DCE related to BP
#define DCESTOP_FLAG_IDLE    0x00000001    ///< Idel: no operator is performed
#define DCESTOP_FLAG_READY   0x00000010    ///< Ready: ready to perform operator
#define DCESTOP_FLAG_SET     0x00000100    ///< Set : stop DCE operator is performed
//#NT#2010/05/11#Linkin Huang -end
//#NT#2010/02/11#Linkin Huang -end
//@}


//######################################################
// the following for enum define

/**
    @name DCE enum - Interrupt enable selection
*/
//@{
typedef enum _DCE_EN_OPT
{
  DCE_EN_DISABLE = 0,   ///< Disable operator
  DCE_EN_ENABLE = 1,    ///< Enable operator
  ENUM_DUMMY4WORD(DCE_EN_OPT)
} DCE_EN_OPT;
//@}


/**
    @name DCE enum - Operating mode
*/
//@{
typedef enum _DCE_OP_MODE
{
  DCE_OP_IPE = 0,  ///< Embedded in IPE operation
  DCE_OP_D2D = 1,  ///< DRAM input/output operation
  ENUM_DUMMY4WORD(DCE_OP_MODE)
} DCE_OP_MODE;
//@}


/**
    @name DCE enum - Normalization factor and shift bit computation selection
*/
//@{
typedef enum _DCE_NORM_TERM_CAL
{
  NORM_TERM_DEFAULT     = 0,   ///< normlization term using default compuation model
  NORM_TERM_USER_DEFINE = 1,   ///< normlization term using user defined compuation model
  ENUM_DUMMY4WORD(DCE_NORM_TERM_CAL)
} DCE_NORM_TERM_CAL;
//@}

/*
    @name DCE enum - UV Horizontal filter enable
*/
//@{
typedef enum _DCE_UV_HFILTER_EN
{
  UV_HFILTER_DISABLE = 0,    ///< disable uv horizontal filter
  UV_HFILTER_ENABLE  = 1,    ///< enable uv horizontal filter
  ENUM_DUMMY4WORD(DCE_UV_HFILTER_EN)
} DCE_UV_HFILTER_EN;
//@}



#if 0
/*
    @name DCE enum - Input packing bits selection
*/
//
typedef enum _DCE_INPUT_PACKIN
{
  DCE_12BIT_INPUT  = 0,    ///< 12-Bits packing
  DCE_8BIT_INPUT   = 2,    ///< 8-Bits packing
  DCE_16BIT_INPUT  = 3,    ///< 16-Bits packing
  ENUM_DUMMY4WORD(em_DCE_INPUT_PACKIN)
} DCE_INPUT_PACKIN;
//
#endif

#if 0
/*
    @name DCE enum - Output packing bits selection
*/
//
typedef enum _DCE_OUTPUT_PACKIN
{
  DCE_12BIT_OUTPUT = 0,    ///< 12-Bits packing
  DCE_8BIT_OUTPUT  = 2,    ///< 8-Bits packing
  DCE_16BIT_OUTPUT = 3,    ///< 16-Bits packing
  ENUM_DUMMY4WORD(em_DCE_OUTPUT_PACKIN)
} DCE_OUTPUT_PACKIN;
//
#endif


#if 0
/*
    @name DCE enum - Color filter array (CFA) selection
*/
//
typedef enum _DCE_CFAPAT
{
  CFA_RGGB = 0,    ///< Starting pixel: R
  CFA_GRBG = 1,    ///< Starting pixel: Gr
  CFA_GBRG = 2,    ///< Starting pixel: Gb
  CFA_BGGR = 3,    ///< Starting pixel: B
  ENUM_DUMMY4WORD(em_DCE_CFAPAT)
} DCE_CFAPAT;
//
#endif


/**
    @name DCE enum - Distortion type selection
*/
//@{
typedef enum _DCE_DIST_SEL
{
  DIST_SQU = 0,   ///< Used for barrel and pincushion distortion
  DIS_NORM = 1,   ///< Ued for perspective distortion
  ENUM_DUMMY4WORD(DCE_DIST_SEL)
} DCE_DIST_SEL;
//@}

#if 0
/*
    @name DCE enum - Correction term enable selection
*/
//
typedef enum _DCE_CORRECTION_EN
{
  GEO_COR_DISABLE = 0,    ///< Disable correction term
  GEO_COR_ENABLE = 1,     ///< Enable correction term
  ENUM_DUMMY4WORD(em_DCE_CORRECTION_EN)
} DCE_CORRECTION_EN;
//
#endif

#if 0
/*
    @name DCE enum - Adding pixel selection for correction term
*/
//
typedef enum _DCE_ADD_PIX
{
  GEO_ADD_1PIX = 0,    ///< Adding 1 pixel
  GEO_ADD_2PIX = 1,    ///< Adding 2 pixels
  ENUM_DUMMY4WORD(em_DCE_ADD_PIX)
} DCE_ADD_PIX;
//
#endif

#if 0
/*
    @name DCE enum - Division selection for correction term
*/
//@{
typedef enum _DCE_DIV
{
  GEO_DIV_NO = 0,    ///< Without division
  GEO_DIV_B2 = 1,    ///< Division by 2
  ENUM_DUMMY4WORD(em_DCE_DIV)
} DCE_DIV;
//
#endif



/**
    @name DCE enum - Input DMA channel selection
*/
//@{
typedef enum _DRAM_IN_PATH
{
  SINGLE_IN  = 1,  ///< Only one input channel is used
  PPB_IN     = 2,  ///< Two input channels are used
  ENUM_DUMMY4WORD(DRAM_IN_PATH)
} DRAM_IN_PATH;
//@}


/**
    @name DCE enum - Output DMA channel selection
*/
//@{
typedef enum _DRAM_OUT_PATH
{
  SINGLE_OUT = 1,    ///< Only one output channel is used
  PPB_OUT    = 2,    ///< Two output channels are used
  //#NT#2010/03/06#Linkin Huang -begin
  //#NT# removed for NT96460
  //DRT_TO_IFE = 3,           ///< Direct output to IFE engine
  //#NT#2010/03/06#Linkin Huang -end
  ENUM_DUMMY4WORD(DRAM_OUT_PATH)
} DRAM_OUT_PATH;
//@}


/**
    @name DCE enum - Wait frame-end selection
*/
//@{
typedef enum _DCE_D2D_FRAME_END_SEL
{
  DCE_D2D_FRAME_END_NOWAIT = 0,    ///< Wait DCE frame-end
  DCE_D2D_FRAME_END_WAIT   = 1,    ///< No wait DCE frame-end
  ENUM_DUMMY4WORD(DCE_D2D_FRAME_END_SEL)
} DCE_D2D_FRAME_END_SEL;
//@}


/**
    @name DCE enum - Get information selectioin
*/
//@{
typedef enum _DCE_GET_INFO_SEL
{
  DCE_GET_ENGINE_STATUS    = 0,    ///< Get the engien status
  DCE_GET_STPCNT           = 1,    ///< Get processed stripe counter  
  DCE_GET_CURR_IN_ADDR_Y   = 2,    ///< Get input Y channel address of current frame
  DCE_GET_CURR_IN_ADDR_UV  = 3,    ///< Get input UV channel address of current frame
  DCE_GET_IN_OFFSET_Y      = 4,    ///< Get input Y chanel line-offset
  DCE_GET_IN_OFFSET_UV     = 5,    ///< Get input UV chanel line-offset    
  DCE_GET_CURR_OUT_ADDR_Y  = 6,    ///< Get output Y channel address of current frame
  DCE_GET_CURR_OUT_ADDR_UV = 7,    ///< Get output UV channel address of current frame
  DCE_GET_OUT_OFFSET_Y     = 8,    ///< Get output Y channel line-offset
  DCE_GET_OUT_OFFSET_UV    = 9,    ///< Get output UV channel line-offset  
  DCE_GET_MACHINE_STATUS   = 10,   ///< Get the status of state machine
  DCE_GET_OPEN_STATE       = 11,   ///< Get engine open state
  DCE_GET_IMAGE_WIDTH      = 12,   ///< Get image's width
  DCE_GET_IMAGE_HEIGHT     = 13,   ///< Get image's height  
  DCE_GET_START_STATUS     = 14,   ///< Get dce start status
  //#NT#2010/09/07#Linkin Huang -begin
  //#NT# add to get info.
  DCE_GET_XDIST_FACTOR     = 15,   ///< Get X input distance factor
  DCE_GET_YDIST_FACTOR     = 16,   ///< Get Y input distance factor
  //#NT#2010/09/07#Linkin Huang -end
  DCE_GET_DATA_FMT         = 17,   ///< Get image data format

  //DCE_PACKBITS_IN    = 2,    // Get input packing bits
  //DCE_CURR_IN_ID     = 3,    // Get input ID of current frame
  //DCE_CURR_OUT_ID      = 6,    ///< Get output ID of current frame
  //DCE_PACKBITS_OUT   = 6,    // Get output packing bits
  //DCE_IN_DRAM_SRC    = 10,   // Get input source
  //DCE_OUT_DRAM_DST   = 11,   // Get output destination,  //removed for NT96460
  //DCE_IN_ADDR1       = 16,   // Get the address of input DMA channel 1
  //DCE_IN_ADDR2       = 17,   // Get the address of input DMA channel 2
  //DCE_OUT_ADDR1      = 18,   // Get the address of output DMA channel 1
  //DCE_OUT_ADDR2      = 19,   // Get the address of output DMA channel 2
  //DCE_OUT_HV_ADDR    = 20,   // Get the address of HV output DMA channel
  //DCE_HANDLE_MODE    = 21,   // Get the handel mode
  ENUM_DUMMY4WORD(DCE_GET_INFO_SEL)
} DCE_GET_INFO_SEL;
//@}



/**
    @name DCE enum - Clear flag selection
*/
//@{
typedef enum _DCE_FLAG_CLEAR_SEL
{
  DCE_FLAG_NO_CLEAR = 0,    ///< No clear flag
  DCE_FLAG_CLEAR    = 1,    ///< Clear flag
  ENUM_DUMMY4WORD(DCE_FLAG_CLEAR_SEL)
} DCE_FLAG_CLEAR_SEL;
//@}

//#NT#2010/02/11#Linkin Huang -begin
//#NT# add pause type selection
/**
    @name DCE enum - Pause Type Selection
*/
//@{
typedef enum _DCE_PAUSE_WAIT_FLAG_SEL
{
  DCE_PAUSE_WAIT_FLAG    = 0,    ///< Used for Waiting DCE Flag case
  DCE_PAUSE_NO_WAIT_FLAG  = 1,    ///< Used for No-waiting DCE Flag case
  ENUM_DUMMY4WORD(DCE_PAUSE_WAIT_FLAG_SEL)
} DCE_PAUSE_WAIT_FLAG_SEL;
//@}
//#define PAUSE_NORM  DCE_PAUSE_WAIT_FLAG
//#define PAUSE_ISR   DCE_PAUSE_NO_WAIT_FLAG
//#NT#2010/02/11#Linkin Huang -end


#if 0
/*
    @name DCE enum - Handle direction selection
*/
//
typedef enum _DCE_HandleMode
{
  HANDLE_HOZTAL_MODE  = 0,    ///< Handle horizontal direction mode
  HANDLE_VERTAL_MODE  = 1,    ///< Handle vertical direction mode
  HANDLE_BI_MODE      = 2,    ///< Handle bi-direction (horizontal and vertical) mode
  ENUM_DUMMY4WORD(em_DCE_HandleMode)
} DCE_HandleMode;
//
#endif


/**
    @name DCE enum - FOV boundary process selection
*/
//@{
typedef enum _DCE_FOV_BOUND_SEL
{
  DCE_FOV_BS_AUTO  = 0,    ///< FOV boundary process method, auto mode
  DCE_FOV_BS_USER  = 1,    ///< FOV boundary process method, user mode
  ENUM_DUMMY4WORD(DCE_FOV_BOUND_SEL)
} DCE_FOV_BOUND_SEL;
//@}

/**
    @name DCE enum - Input data format
*/
//@{
typedef enum _DCE_IN_FORMAT_SEL
{
  DCE_IN_S_Y  = 0,             ///< Single Y plane, 8-bit mode
  DCE_IN_422_UV_16BIT  = 1,    ///< UV 422 packed plane, 16-bit mode
  DCE_IN_420_UV_16BIT  = 2,    ///< UV 420 packed plane, 16-bit mode
  DCE_IN_422_YUV_16BIT = 3,    ///< YUV 422 packed plane
  DCE_IN_420_YUV_16BIT = 4,    ///< YUV 420 packed plane
  ENUM_DUMMY4WORD(DCE_IN_FORMAT_SEL)
} DCE_IN_FORMAT_SEL;
//@}

/**
    @name DCE enum - Horizontal first stripe start position
*/
//@{
typedef enum _DCE_FIRST_STRIPE_POS_SEL
{
  DCE_FIRST_STP_POS_CAL  = 0,   ///< Input starts from DCE calculated position
  DCE_FIRST_STP_POS_0    = 1,   ///< Input starts from 0
  ENUM_DUMMY4WORD(DCE_FIRST_STRIPE_POS_SEL)
} DCE_FIRST_STRIPE_POS_SEL;

//@}


/**
    @name DCE enum - Horizontal stripe overlap selection
*/
//@{
typedef enum _DCE_H_OVERLAP_SEL
{
  DCE_HOVLSP_16P    = 0,   ///< 16 pixels overlap
  DCE_HOVLSP_24P    = 1,   ///< 24 pixels overlap
  DCE_HOVLSP_32P    = 2,   ///< 32 pixels overlap
  ENUM_DUMMY4WORD(DCE_H_OVERLAP_SEL)
} DCE_H_OVERLAP_SEL;
//@}



//######################################################
// the following for structure define


/*
    DCE structure - Input DMA mode setting
*/
//@{
typedef struct _DCE_INPUT_DMA_BUF_INFO
{
  //DRAM_IN_PATH in_path;  // Set input path
  //UINT32 in_dram_sai_0;       // Set the first input buffer address
  //UINT32 in_dram_sai_1;     // Set the second input buffer address

  UINT32 uiInAddrY0;    ///< intput buffer address for Y channel
  UINT32 uiInAddrUV0;   ///< input buffer address for UV channel
} DCE_INPUT_DMA_BUF_INFO;
//@}


/**
    DCE structure - Output DMA mode setting
*/
//@{
typedef struct _DCE_OUTPUT_DMA_BUF_INFO
{
  //DRAM_OUT_PATH out_path;  // Set output path
  //UINT32 out_dram_sao_0;      // Set the first output buffer address
  //UINT32 out_dram_sao_1;      // Set the second output buffer address
  //UINT32 HV_MODE_ADDR;        // Set the thired output buffer address

  UINT32 uiOutAddrY0;    ///< intput buffer address for Y channel
  UINT32 uiOutAddrUV0;   ///< input buffer address for UV channel
} DCE_OUTPUT_DMA_BUF_INFO;
//@}


#if 0
/*
    DCE structure - Interrupt enable setting
*/
//
typedef struct _DCE_INTERRTUP_EN
{
  DCE_EN_OPT act_op;       ///< Select operation to desired interrupt
} DCE_INTERRTUP_EN;
//
#endif


/**
    DCE structure - Image size as well as input and output line-offset setting
*/
//@{
typedef struct _DCE_IMAGE_SIZE
{
  UINT32 uiImgWidth;         ///< Image width
  UINT32 uiImgHeight;        ///< Image height
  UINT32 uiInLineOffsetY;     ///< Input Y channel line-offset
  UINT32 uiInLineOffsetUV;    ///< Input UV channel line-offset
  UINT32 uiOutLineOffsetY;    ///< Output Y channel line-offset
  UINT32 uiOutLineOffsetUV;   ///< Output UV channel line-offset
} DCE_IMAGE_SIZE;
//@}


/**
    DCE structure - Geomatric distortion center setting
*/
//@{
typedef struct _DCE_GEO_XY_CENTER
{
  UINT32 uiCenterX;  ///< The center of geomatric distortion compensation at horizontal direction
  UINT32 uiCenterY;  ///< The center of geomatric distortion compensation at vertical direction
} DCE_GEO_XY_CENTER;
//@}


/**
    DCE structure - Color aberration correction factor setting
*/
//@{
typedef struct _DCE_ABRTN_FACTOR
{
  UINT32 uiAbrFactorR;    ///< Correction factor for R channel LUT
  UINT32 uiAbrFactorG;    ///< Correction factor for G channel LUT
  UINT32 uiAbrFactorB;    ///< Correction factor for B channel LUT
} DCE_ABRTN_FACTOR;
//@}


/**
    DCE structure - Distortion distance factor setting
*/
//@{
typedef struct _DCE_IN_DISTANCE_FACTOR
{
  UINT32 uiDistX;               ///< X input distance factor for oval shape modeling
  UINT32 uiDistY;               ///< Y input distance factor for oval shape modeling
  DCE_DIST_SEL DistSel;   ///< Distance ratio
} DCE_IN_DISTANCE_FACTOR;
//@}

#if 0
/*
    DCE structure - Correction term setting
*/
//
typedef struct
{
  em_DCE_CORRECTION_EN  ct_en;    ///< Correction term enable; GEO_COR_DISABLE: disable, GEO_COR_ENABLE: enable
  UINT8                 ct_norm;  ///< Normalization
  em_DCE_ADD_PIX        pix_add;  ///< Pixel adding; GEO_ADD_1PIX: 1 pixel, GEO_ADD_2PIX: 2 pixel
  em_DCE_DIV            div;      ///< Division; GEO_DIV_NO: no division, GEO_DIV_B2: division by 2
} _CORRECTION_TREM;
//
#endif

#if 0
/*
    DCE structure - Correction gain setting
*/
//
typedef struct
{
  UINT32 r_corr_gain;    ///< Correction gain for R channel
  UINT32 gr_corr_gain;   ///< Correction gain for Gr channel
  UINT32 gb_corr_gain;   ///< Correction gain for Gb channel
  UINT32 b_corr_gain;    ///< Correction gain for B channel
} _CORRCTION_GAIN;
//
#endif


/**
    DCE structure - Field of view (FOV) setting
*/
//@{
typedef struct _DCE_GEO_FOV_INFO
{
  UINT32 uiGeoFovGain;              ///< Correction gain for R channel
  DCE_FOV_BOUND_SEL GeoFovBoundSel;  ///< FOV boundary process method, if geo_fov_bs = DCE_FOV_BS_USER, set geo_fov_boundr/g/b
  UINT32 uiGeoFovBoundR;            ///< FOV boundary pixel of R channel
  UINT32 uiGeoFovBoundG;            ///< FOV boundary pixel of G channel
  UINT32 uiGeoFovBoundB;            ///< FOV boundary pixel of B channel
} DCE_GEO_FOV_INFO;
//@}

/**
    DCE structure - Geometric enhancement term
*/
//@{
typedef struct _DCE_GEO_ENH_INFO
{
  UINT32 uiGeoEnhFactY;  ///< Geometric enhancement term factor for Y channel
  UINT32 uiGeoEnhBitY;  ///< Geometric enhancement term normalize bit for Y channel
  UINT32 uiGeoEnhFactUV;  ///< Geometric enhancement term factor for UV channel
  UINT32 uiGeoEnhBitUV;  ///< Geometric enhancement term normalize bit for UV channel
} DCE_GEO_ENH_INFO;
//@}


/**
    DCE structure - Distance normalization setting
*/
//@{
typedef struct _DCE_RADIUS_NORM_INFO
{
  UINT32 uiGeoNormFact;    ///< Radius normalization factor
  UINT32 uiGeoNormBit;     ///< Radius normalization shift bit
} DCE_RADIUS_NORM_INFO;
//@}


/**
    DCE structure - Engine open object
*/
//@{
typedef struct _DCE_OPENOBJ
{
  void (*FP_DCEISR_CB)(UINT32 uiIntStatus);   ///< External ISR call-back function
  UINT32 uiDceClockSel;                      ///< Engine clock selection
} DCE_OPENOBJ,*pDCE_OPENOBJ;
//@}

#if 0
/*
    DCE structure - Input and Output DMA mode and Interrupt enable setting
*/
//
typedef struct
{
  DCE_INPUT_DMA_BUF_INFO        InDMAInfo;      ///< Set DMA input mode selection
  DCE_OUTPUT_DMA_BUF_INFO       OutDMAInfo;     ///< Set DMA output mode selection
  //em_DCE_INPUT_PACKIN        pack_busi;    ///< Input packing bits selection
  //em_DCE_OUTPUT_PACKIN       pack_buso;    ///< Onput packing bits selection
  //em_DCE_HandleMode          PrcsdMode;   ///< Process Mode selection
  //em_DCE_CFAPAT              dce_cfapat;   ///< Color filter array selection
  //UINT32                     bp;           ///< break-point setting
  UINT32                     uiInterruptEnable;       ///< DCE interrupt enable selection 
} DCE_STATIC_PARAM_INFO;
//
#endif

/**
    DCE structure - Function control setting
*/
//@{
typedef struct _DCE_FUN_INFO
{
  DCE_OP_MODE        OperationMode;  ///< Operation mode  
  DCE_IN_FORMAT_SEL  InFormat;    ///< DRAM input data format
} DCE_FUN_INFO;
//@}

/**
    DCE structure - Run-time changeable setting
*/
//@{
typedef struct _DCE_DYNAMIC_PARAM_INFO
{
  DCE_GEO_FOV_INFO        FovParam;       ///< Field of view setting
  DCE_GEO_ENH_INFO        EnhParam;       ///< Geometric enhancement term
  DCE_RADIUS_NORM_INFO    DsistNorm;    ///< GEO normaliztion factor and shift bit setting
  DCE_ABRTN_FACTOR        AbrtFact;     ///< Color aberration correction factor setting
  //_CORRECTION_TREM      corr_term;    ///< Correction term setting
  //_CORRCTION_GAIN       corr_gain;    ///< Correction gain setting
  UINT32                  *puiGeoLut;     ///< 66 elements LUT table  
  DCE_NORM_TERM_CAL       NormTermCal;  ///< Normlization factor and shift bit computation selection
  DCE_UV_HFILTER_EN       FilterEnable;    ///< Horizontal filter enable for UV channel
} DCE_DYNAMIC_PARAM_INFO;
//@}


/**
    DCE structure - Engain basic setting
*/
//@{
typedef struct _DCE_PARAM_INFO
{
  DCE_IMAGE_SIZE            ImgInfo;            ///< Image information
  DCE_FUN_INFO              EngineCtrl;         ///< engine control
  UINT32                    *puiStripes;         ///< stripe size, 16 elements
  UINT32                    uiIncPxlNums;       ///< max input horizontal stripe Increment pixel number
  UINT32                    uiDecPxlNums;       ///< max input horizontal stripe Decrement pixel number
  DCE_FIRST_STRIPE_POS_SEL  FirstStripePos;     ///< Input first stripe start position selection
  DCE_H_OVERLAP_SEL         HOverlapSel;        ///< Output stripe overlap selection
  DCE_INPUT_DMA_BUF_INFO    InDMAInfo;          ///< Set DMA input mode selection
  DCE_OUTPUT_DMA_BUF_INFO   OutDMAInfo;         ///< Set DMA output mode selection    
  DCE_GEO_XY_CENTER         CenterXY;           ///< Distortion center setting
  DCE_IN_DISTANCE_FACTOR    DistSelXY;          ///< Input horizontal and vertical distance factors and ratio selection  
  UINT32                    uiInterruptEnable;    ///< DCE interrupt enable selection 
  DCE_DYNAMIC_PARAM_INFO    *pSetDceIQInfo;      ///< IQ related variables
} DCE_PARAM_INFO;
//@}


/**
    DCE structure - Distance normalization Computation Information
*/
//@{
typedef struct _DCE_DIST_NORM_INFO
{
  UINT32 uiWidth;           ///< Image width
  UINT32 uiHeight;          ///< Image height
  UINT32 uiGeoCenterX;      ///< Compensation center for horizontal direction
  UINT32 uiGeoCenterY;      ///< Compensation center for vertical direction
  UINT32 uiDistFactorX;     ///< distance factor for horizontal direction
  UINT32 uiDistFactorY;     ///< distance factor for vertical direction
  UINT32 *puiGdcLut;        ///< Geometric Distortion Compensation LUT
  UINT32 uiDistRatio;       ///< distance ratio, 4096 = 1X
} DCE_DIST_NORM_INFO;
//@}



//######################################################
/* extern Function Declaration */
// main extern functions


/*
    Open DCE engine

   When ones would like to use DCE engine, this function must be called firstly before calling any function

   @param[in] pDCE_OpenInfo  Engine open object, containing clock selection as well as external ISR call-back function
   -@b Supported clocks: 48, 60, 80, 120, 160, 192, 220Mhz

    @return ER Erroc code to check engine opening status\n
                    -@b E_OK  Execute function, success.\n
                    -@b E_SYS  Engine open, fail.\n
*/
extern ER dce_open(DCE_OPENOBJ *pOpenInfo);
extern ER dce_close(VOID);
extern ER dce_setMode(DCE_PARAM_INFO *pSetModeInfo);
extern ER dce_start(VOID);
//#NT#2010/04/27#Linkin Huang -begin
//#NT# DCE crash issue
extern ER dce_pause(DCE_PAUSE_WAIT_FLAG_SEL PauseTypeSel);
//#NT#2010/04/27#Linkin Huang -end
extern ER dce_reset(DCE_EN_OPT Operation);
extern VOID dce_resetHardware(VOID);
extern ER dce_SetClock(UINT32 uiSetClockRate);

extern ER dce_start_pause_DirectMode(VOID);


extern ER dce_chgFullImageSize(DCE_IMAGE_SIZE *pChgImgSizeParam);
extern ER dce_chgImageCenter(DCE_GEO_XY_CENTER *pChgCentParam);
extern ER dce_chgDistNormlization(DCE_RADIUS_NORM_INFO *pChgDisNormParam);
extern ER dce_chgAberration(DCE_ABRTN_FACTOR *pChgAberParam);
extern ER dce_chgDistanceFactor(DCE_IN_DISTANCE_FACTOR *pChgDistFatrParam);
extern ER dce_chgFOV(DCE_GEO_FOV_INFO *pChgFOVParam);
extern ER dce_chgEnhancement(DCE_GEO_ENH_INFO *pChgEnhParam);
extern ER dce_chgDistortionLUT(UINT32 *pChgLutParam);
extern ER dce_chgOutputStripe(UINT32 *pChgStripeParam);
extern ER dce_chgBoundaryPixelNums(UINT32 uiIncrPxls, UINT32 uiDecrPxls);
extern ER dce_setUVHorizontalFilterEnable(DCE_UV_HFILTER_EN *pChgFilterEnParam);



// wait and clear frame-end IPL flag
extern VOID dce_waitFlagFrameEnd(DCE_FLAG_CLEAR_SEL FlagClear);
extern VOID dce_clearFlagFrameEnd(VOID);

// wait and clear frame-start IPL flag
extern VOID dce_waitFlagFrameStart(DCE_FLAG_CLEAR_SEL FlagClear);
extern VOID dce_clearFlagFrameStart(VOID);

// wait and clear BP's IPL flag
//extern void dce_waitFlagBP(DCE_FLAG_CLEAR_SEL isClear);
//extern void dce_clrFlagBP(void);

// get information
extern UINT32 dce_getInfomation(DCE_GET_INFO_SEL GetDceInfo);
extern UINT32 dce_getIntStatus(VOID);
extern UINT32 dce_getIntEnableStatus(VOID);
extern VOID dce_getOutputStripes(UINT32 *puiGetDceOutStps);

//######################################################
/* extern Function Declaration */
// other extern functions
extern VOID dce_waitFrameEnd(VOID);

//#NT#2010/02/11#Linkin Huang -begin
//#NT# add to stop DCE via ISR
extern VOID dce_clrStopDCEFlag(UINT32 uiFlag);
extern UINT32 dce_getStopDCEFlag(VOID);
extern VOID dce_setStopDCEFlag(UINT32 uiFlag);
extern VOID dce_setStop(VOID);
//#NT#2010/02/11#Linkin Huang -end

//#NT#2010/03/10#Linkin Huang -begin
//#NT# new function, add for computing the buffer size of GDC
extern UINT32 dce_calBufferSize(UINT32 uiLineOffset, UINT32 uiImgHeight);
//#NT#2010/03/10#Linkin Huang -end

extern UINT32 dce_getClockRate(VOID);


/*
//#NT#2010/11/18#Linkin Huang -begin
//#NT# change DCE engine clock
ER dce_chgEngineClock(UINT32 DCE_CLOCKSEL);
//#NT#2010/11/18#Linkin Huang -end
*/

#endif
//@}












