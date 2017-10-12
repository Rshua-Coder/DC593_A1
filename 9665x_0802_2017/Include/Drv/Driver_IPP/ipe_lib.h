/**
    IPE module driver

    NT96450 IPE driver extern header file.

    @file       Ipe_lib.h
    @ingroup    mIIPPIPE
    @note
\n IPE Driver has 4 states (Idel, Ready, Pause, Run)
\n Idle state: No operation, and IPE semaphore is not occupied.
\n Ready state: No operation, IPE parameters are not configured yet, but IPE semaphore is occupied.
\n Pause state: No operation, IPE parameters are already configured for operation, IPE semaphore is occupied.
\n Run state: In operation, some parameters are allowed to be changed dynamically, IPE semaphore is occupied.
\n ipe_open    Get IPE semaphore before operation starts
\n
\n Function list:
\n ipe_open() Get IPE semaphore before operation starts
\n ipe_close()   Release IPE semaphore after operation ends
\n ipe_changeSize()  Set all Dzoom size related params
\n ipe_changeParam() Set IQ related params
\n ipe_changeInput() Set input address, sync selection, and PPB control
\n ipe_changeOutput()    Set output source, address, and line offset
\n ipe_setMode() Set all IPE params, including size, IQ param, I/O
\n ipe_start()   Trigger IPE start
\n ipe_pause()   Trigger IPE stop
\n ipe_reset()   SW reset IPE
\n ipe_enableDMAOut()    Turn on/off IPE output DMA channel
\n ipe_clrFrameEnd() Clear IPE frame end flag
\n ipe_waitFrameEnd()    Wait IPE frame end flag
\n ipe_clrDMAEnd()   Clear IPE DMA done flag
\n ipe_waitDMAEnd()  Wait IPE DMA done flag
\n
\n Dram to Dram usage example:
\n (1)ipe_open()
\n (2)ipe_setMode()
\n (3)ipe_start()
\n (4)ipe_waitFrameEnd()
\n (5)ipe_close()

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _IPE_LIB_H
#define _IPE_LIB_H

#define IPE_DEBUG   0
#define IPE_DEBUG_INFO   0

#define GetIpeVACnt ipe_getVACnt

/** \addtogroup mIIPPIPE */
//@{

/**
    @name Define IPE function enable.
*/
//@{
#define IPE_CGAIN_EN                   0x00000001  ///< Color gain function enable
#define IPE_GBAL_EN                    0x00000002  ///< GBAL function enable
#define IPE_CINT_EN                    0x00000004  ///< CFA interpolation function enable
#define IPE_RGBLPF_EN                  0x00000008  ///< RGB low-pass filter function enable
#define IPE_RGBGAMMA_EN                0x00000010  ///< RGB Gamma function enable
#define IPE_YGAMMA_EN                  0x00000020  ///< Y Gamma function enable
#define IPE_CCR_EN                     0x00000100  ///< Color correction function enable
#define IPE_CC3D_EN                    0x00000200  ///< 3D color correction function enable
#define IPE_CST_EN                     0x00000400  ///< Color space transform function enable, RGB->YCC
#define IPE_CTRL_EN                    0x00001000  ///< Color control function enable
#define IPE_HADJ_EN                    0x00002000  ///< Hue adjustment function enable
#define IPE_CADJ_EN                    0x00004000  ///< Color component adjust function enable
#define IPE_CADJ_YENH_EN               0x00008000  ///< Color component edge enhance function enable, sub-function of IPE_CADJ_EN
#define IPE_CADJ_YCON_EN               0x00010000  ///< Color component Y contrast adjust function enable, sub-function of IPE_CADJ_EN
#define IPE_CADJ_CCON_EN               0x00020000  ///< Color component CbCr contrast adjust function enable, sub-function of IPE_CADJ_EN
#define IPE_YCTH_EN                    0x00040000  ///< Color component YCbCr substitution function enable, sub-function of IPE_CADJ_EN
#define IPE_CSTP_EN                    0x00080000  ///< Color space transform protect function enable, RGB->YCC
#define IPE_VACC_EN                    0x00100000  ///< VA function enable
#define IPE_THOUT_EN                   0x00200000  ///< Edge threshold output enable
#define IPE_CSUBOUT_EN                 0x00400000  ///< CbCr subsample output enable
#define IPE_VSUBOUT_EN                 0x00800000  ///< V subsample output enable
//@}

/**
    @name Define IPE interrput enable.
*/
//@{
#define IPE_INTE_IPESTART              0x00000001  ///< Interrupt enable for triggering IPE start
#define IPE_INTE_FRM                   0x00000002  ///< Interrupt enable for frame done
#define IPE_INTE_ST                    0x00000004  ///< Interrupt enable for current stripe done
#define IPE_INTE_YCOUTEND              0x00000010  ///< Interrupt enable for YC DRAM output done
#define IPE_INTE_VAOUTEND              0x00000020  ///< Interrupt enable for VA DRAM output done
#define IPE_INTE_THOUTEND              0x00000040  ///< Interrupt enable for edge theshold DRAM output done
#define IPE_INTE_CSUBOUTEND            0x00000080  ///< Interrupt enable for CbCr subsample DRAM output done
#define IPE_INTE_VSUBOUTEND            0x00000100  ///< Interrupt enable for V subsample DRAM output done
#define IPE_INTE_DMAIN0END             0x00000200  ///< Interrupt enable for DRAM input done
#define IPE_INTE_DMAIN1END             0x00000400  ///< Interrupt enable for DRAM input done
#define IPE_INTE_ALL                   0x000007f7  ///< All interrupt enable
//@}

/**
    @name Define IPE interrput status.
*/
//@{
#define IPE_INT_IPESTART              0x00000001  ///< Interrupt enable for triggering IPE start
#define IPE_INT_FRM                   0x00000002  ///< Interrupt enable for frame done
#define IPE_INT_ST                    0x00000004  ///< Interrupt enable for current stripe done
#define IPE_INT_YCOUTEND              0x00000010  ///< Interrupt enable for YC DRAM output done
#define IPE_INT_VAOUTEND              0x00000020  ///< Interrupt enable for VA DRAM output done
#define IPE_INT_THOUTEND              0x00000040  ///< Interrupt enable for edge theshold DRAM output done
#define IPE_INT_CSUBOUTEND            0x00000080  ///< Interrupt enable for CbCr subsample DRAM output done
#define IPE_INT_VSUBOUTEND            0x00000100  ///< Interrupt enable for V subsample DRAM output done
#define IPE_INT_DMAIN0END             0x00000200  ///< Interrupt enable for DRAM input done
#define IPE_INT_DMAIN1END             0x00000400  ///< Interrupt enable for DRAM input done
#define IPE_INT_ALL                   0x000007f7  ///< All interrupt enable
//@}

/**
    @name Define IPE engine value.
*/
//@{
#define IPE_HSIZE_IODIFF     0

#define VAC_WIN8                        8  ///< VA window number 8
#define VAC_WIN16                       16  ///< VA window number 16
#define IPE_VAC_HNF_MINIMUN             2  ///< Minimum VA hnf value
#define IPE_VACC_VALUE_MSK              0x0003ffff  ///< VA DRAM data, contrast value mask
#define IPE_VACC_NUM_MSK                0xfffc0000  ///< VA DRAM data, threshold number mask
#define IPE_MAXVAWORD      512  ///< Maximum VA DRAM output word
#define IPE_VA_MAXH_WIN8                    1030  ///< Maximum VA horizontal size when VA window number is 8x8
#define IPE_VA_MAXH_WIN16                  2038  ///< Maximum VA horizontal size when VA window number is 16x16

#define IPE_MAX_COLOR_OFFSET    255
#define IPE_MAX_COLOR_GAIN      1023
//@}

/**
    @name Define IPE function setting selection.
*/
//@{
#define IPE_SET_NONE                0x00000000  ///< Select don't update ANY parameters
#define IPE_SET_CG_GAIN             0x00000001  ///< Select to update color gain parameters
#define IPE_SET_CG_OFS              0x00000002  ///< Select to update color offset parameters
#define IPE_SET_CG_INVERSE          0x00000004  ///< Select to update inverse effect parameters
#define IPE_SET_CG_MASK             0x00000008  ///< Select to update inverse effect parameters
#define IPE_SET_GBAL                0x00000010  ///< Select to update NR parameters
#define IPE_SET_CFA                 0x00000020  ///< Select to update CFA parameters
#define IPE_SET_EDGEEXT             0x00000040  ///< Select to update edge related parameters
#define IPE_SET_EDGETH              0x00000080  ///< Select to update edge related parameters
#define IPE_SET_EDGEPROC            0x00000100  ///< Select to update edge related parameters
#define IPE_SET_VA                  0x00000200  ///< Select to update VA parameters
#define IPE_SET_INDEPVA             0x00000400  ///< Select to update VA parameters
#define IPE_SET_RGBLPF              0x00000800  ///< Select to update color correction parameters
#define IPE_SET_COLORCORRECT        0x00001000  ///< Select to update color correction parameters
#define IPE_SET_CST                 0x00002000  ///< Select to update color correction parameters
#define IPE_SET_CCTRL               0x00004000  ///< Select to update color correction parameters
#define IPE_SET_CADJ_EENH           0x00008000  ///< Select to update color correction parameters
#define IPE_SET_CADJ_EINV           0x00010000  ///< Select to update color correction parameters
#define IPE_SET_CADJ_YCCON          0x00020000  ///< Select to update color correction parameters
#define IPE_SET_CADJ_COFS           0x00040000  ///< Select to update color correction parameters
#define IPE_SET_CADJ_RAND           0x00080000  ///< Select to update color correction parameters
#define IPE_SET_CADJ_FIXTH          0x00100000  ///< Select to update color correction parameters
#define IPE_SET_CADJ_MASK           0x00200000  ///< Select to update color correction parameters
#define IPE_SET_CSTP                0x00400000  ///< Select to update color correction parameters
#define IPE_SET_CSUB                0x00800000  ///< Select to update color correction parameters
#define IPE_SET_VSUB                0x01000000  ///< Select to update color correction parameters
#define IPE_SET_GAMMA               0x02000000  ///< Select to update color correction parameters
#define IPE_SET_CC3D                0x04000000  ///< Select to update color correction parameters

#define IPE_SET_ALL                 0xffffffff  ///< Select to update ALL IQ parameters
//@}

/**
    @name Enum IPE input control.
*/
//@{

/**
    IPE DRAM input SRAM update selection

\n Used for ipe_changeParam().
*/
typedef enum
{
    IPE_READ_LUT = 0,        ///< IPE read SRAM
    DMA_WRITE_LUT = 1,       ///< DMA write SRAM
    CPU_READ_LUT = 2,        ///< CPU read SRAM
    ENUM_DUMMY4WORD(IPE_RW_SRAM_ENUM)
} IPE_RW_SRAM_ENUM;

/**
    IPE input Bayer data start CFA selection

    Select intput Bayer raw data start channel - R, Gr, Gb, B.
\n Used for ipe_changeSize().
*/
typedef enum
{
    IPE_PATR = 0,           ///< Start channel is R for input Bayer raw data
    IPE_PATGR = 1,        ///< Start channel is Gr for input Bayer raw data
    IPE_PATGB = 2,        ///< Start channel is Gb for input Bayer raw data
    IPE_PATB = 3,           ///< Start channel is B for input Bayer raw data
    ENUM_DUMMY4WORD(IPE_CFA_PAT_ENUM)
} IPE_CFA_PAT_ENUM;

/**
    IPE input data bit number selection

    Select intput data bit number - 8, 10, 12, 16 bit.
\n Used for ipe_changeInput().
*/
typedef enum
{
    IPE_PACK_08BIT = 0,     ///< Input data fornat is 8 bit pack
    IPE_PACK_10BIT = 1,     ///< Input data fornat is 10 bit pack
    IPE_PACK_12BIT = 2,     ///< Input data fornat is 12 bit pack
    IPE_PACK_16BIT = 3,      ///< Input data fornat is 16 bit pack
    ENUM_DUMMY4WORD(IPE_PACK_BIT_ENUM)
} IPE_PACK_BIT_ENUM;

/**
    IPE input source engine selection

    Select intput source engine
*/
typedef enum
{
    SRC_DRAM = 0,        ///< IPE input from DRAM
    SRC_IFE = 1,         ///< IPE input from IFE
    ENUM_DUMMY4WORD(IPE_IN_SRC_ENUM)
} IPE_IN_SRC_ENUM;

/**
    IPE D2D input data type selection

    Select IPE intput data type, this selection is effective only when IPE is D2D mode(ipe_Dram2Dram())
\n Used for ipe_Dram2Dram().
*/
typedef enum
{
    BAYER_IN = 0,                       ///< Input data type is Bayer raw
    SINGLE_CHANNEL_IN = 1,      ///< Input data type is single channel planar data
    ENUM_DUMMY4WORD(IPE_D2D_IN_FORMAT_ENUM)
} IPE_D2D_IN_FORMAT_ENUM;
//@}

/**
    @name Enum IPE operation mode.
*/
//@{

/**
    IPE stripe mode selection

    Select IPE intput stripe mode
\n Used for ipe_changeSize().
*/
typedef enum
{
    AUTO_FRAME_MODE = 0,        ///<  Auto frame mode, IPE processes multi-stripes according to HN, HL, HM, VN, VL, VM setting
    DVI_MODE = 1,               ///<  DVI mode, IPE processes CCIR601 data format(Y Cb Y Cr) in single stripe mode according to HL and VL setting
    ENUM_DUMMY4WORD(IPE_STRIPE_ENUM)
} IPE_STRIPE_ENUM;

/**
    IPE embed DCE selection

    Select CE embed in IPE mode
\n Used for ipe_changeSize().
*/
typedef enum
{
    DCE_EMBED_DISABLE = 0,        ///<  IPE independent operation
    DCE_EMBED_ENABLE = 1,        ///<  DCE embed in IPE operation
    ENUM_DUMMY4WORD(IPE_DCE_EMBED_ENUM)
} IPE_DCE_EMBED_ENUM;

//@}

/**
    @name Enum IPE DVI.
*/
//@{

/**
    IPE DVI input start pixel selection

    Select IPE input start pixel for DVI mode
*/
typedef enum
{
    DVI_CBYCRY = 0,         ///< DVI input order is Cb, Y, Cr, Y
    DVI_CRYCBY = 1,         ///< DVI input order is Cr, Y, Cb, Y
    DVI_YCRYCB = 2,         ///< DVI input order is Y, Cr, Y, Cb
    DVI_YCBYCR = 3,          ///< DVI input order is Y, Cb, Y, Cr
    ENUM_DUMMY4WORD(IPE_DVI_FORMAT_IN_ENUM)
} IPE_DVI_FORMAT_IN_ENUM;

/**
    IPE DVI input start pixel selection

    Select IPE horizontal subsample ratio for DVI mode
*/
typedef enum
{
    DVI_HSUB1 = 0,          ///< 1X H subsample, 640x480 input Y->640x480 output Y
    DVI_HSUB2 = 1,          ///< 2X H subsample, 640x480 input Y->320x480 output Y
    DVI_HSUB4 = 2,           ///< 4X H subsample, 640x480 input Y->160x480 output Y
    ENUM_DUMMY4WORD(IPE_DVI_HSUB_OUT_ENUM)
} IPE_DVI_HSUB_OUT_ENUM;

//@}

/**
    @name Enum IPE output control.
*/
//@{

/**
    IPE output YCC format selection

    Select IPE YCC output format
\n Used for ipe_changeOutput().
*/
typedef enum
{
    IPE_YCC422 = 0,         ///< Select YCC 422 output format
    IPE_YCC420 = 1,          ///< Select YCC 420 output format
    IPE_YONLY = 2,
    ENUM_DUMMY4WORD(IPE_OUT_FORMAT_ENUM)
} IPE_OUT_FORMAT_ENUM;

/**
    IPE output YCC subsample selection

    Select IPE YCC 422 420 subsample method
\n Used for ipe_changeOutput().
*/
typedef enum
{
    YCC_DROP_RIGHT = 0,         ///< When 422 or 420 format is chosen, drop right pixel for subsample
    YCC_DROP_LEFT = 1,         ///< When 422 or 420 format is chosen, drop left pixel for subsample
    ENUM_DUMMY4WORD(IPE_HSUB_SEL_ENUM)
} IPE_HSUB_SEL_ENUM;

//@}

/**
    @name Enum IPE color channel.
*/
//@{

/**
    IPE color gain range selection

    Select IPE color gain range
\n Used for ipe_changeParam().
*/
typedef enum
{
    CGRANGE_2_8 = 0,
    CGRANGE_3_7 = 1,
    ENUM_DUMMY4WORD(IPE_CG_RANGE_ENUM)
} IPE_CG_RANGE_ENUM;

/**
    IPE Gamma color channel selection

    Select IPE Gamma color channel
\n Used for ipe_changeParam().
*/
typedef enum
{
    IPE_R_CHANNEL = 0,      ///< Select R channle for setting
    IPE_G_CHANNEL = 1,     ///< Select G channle for setting
    IPE_B_CHANNEL = 2,      ///< Select B channle for setting
    ENUM_DUMMY4WORD(IPE_CHANNEL_ENUM)
} IPE_CHANNEL_ENUM;

//@}

/**
    @name Enum IPE overlap enable.
*/
//@{

/**
    IPE multi-stripe overlap selection

    Select IPE overlap method in multi-stripe mode
\n Used for ipe_changeSize().
*/
typedef enum
{
    IPE_OLAP_OFF = 0,       ///< Disable stripe overlap
    IPE_OLAP_16pix = 1,         ///< Enable stripe overlap, overlap must be enabled when direct output to IME
    IPE_OLAP_32pix = 2,         ///< Enable stripe overlap, overlap must be enabled when direct output to IME
    ENUM_DUMMY4WORD(IPE_OLAP_ENUM)
} IPE_OLAP_ENUM;

//@}

/**
    @name Enum IPE VA param.
*/
//@{

typedef enum
{
    PRE_GAMMA = 0,
    POST_GAMMA = 1,
    ENUM_DUMMY4WORD(IPE_PRE_POST_GAM_ENUM)
} IPE_PRE_POST_GAM_ENUM;

typedef enum
{
    NO_FILTER = 0,
    TAP3_FILTER = 1,
    TAP5_FILTER = 2,
    TAP7_FILTER = 3,
    ENUM_DUMMY4WORD(IPE_VD_FILT_ENUM)
} IPE_VD_FILT_ENUM;

typedef enum
{
    OUT_GRP0 = 0,
    OUT_GRP1 = 1,
    OUT_GRP01 = 2,
    OUT_GRP12 = 3,
    OUT_GRP012 = 4,
    ENUM_DUMMY4WORD(IPE_VA_OUT_SEL_ENUM)
} IPE_VA_OUT_SEL_ENUM;

/**
    IPE variation accumulation input data type selection

    Select input data type for VA
\n Used for ipe_changeParam().
*/
typedef enum
{
    VAG0_ESD = 0,
    VAG0_E5 = 1,
    VAG0_E7 = 2,
    ENUM_DUMMY4WORD(IPE_VA_G0_SRC_ENUM)
} IPE_VA_G0_SRC_ENUM;

/**
    IPE variation accumulation DRAM update mode selection

    Select DRAM update mode for VA
\n Used for ipe_changeParam().
*/
typedef enum
{
    VACC_AUTO_UPDATED = 0,       ///< VA result is updated to DRAM every frame
    VACC_MAN_UPDATED = 1,          ///< VA result is updated to DRAM after interrupt status IPE_INT_DMAEND cleared
    ENUM_DUMMY4WORD(IPE_VA_MODE_ENUM)
} IPE_VA_MODE_ENUM;

//@}

/**
    @name Enum IPE edge param.
*/
//@{

/**
    IPE edge extract gamma selection

    Select pre or post gamma for edge extract
\n Used for ipe_changeParam().
*/
typedef enum
{
    EEXT_PRE_GAM = 0,       ///< Select pre-gamma edge extraction
    EEXT_POST_GAM = 1,     ///< Select post-gamma edge extraction
    ENUM_DUMMY4WORD(IPE_EEXT_GAM_ENUM)
} IPE_EEXT_GAM_ENUM;

/**
    IPE edge extract channel selection

    Select G or Y for edge extract
\n Used for ipe_changeParam().
*/
typedef enum
{
    EEXT_G_CHANNEL = 0,       ///< Select G edge extraction
    EEXT_Y_CHANNEL = 1,     ///< Select Y edge extraction
    ENUM_DUMMY4WORD(IPE_EEXT_CH_ENUM)
} IPE_EEXT_CH_ENUM;

/**
    IPE edge mapping input selection

    Select intput data for edge mapping
\n Used for ipe_changeParam().
*/
typedef enum
{
    EIN_ESD = 0,       ///< Select 5x5 user defined kernel output for edge mapping input
    EIN_E57 = 1,       ///< Select 3x3 fixed kernel output for edge mapping input
    EIN_AVG = 2,       ///< Select average of EEXT and EEXT2 for edge mapping input
    ENUM_DUMMY4WORD(IPE_EMAP_IN_ENUM)
} IPE_EMAP_IN_ENUM;

/**
    IPE edge mapping output selection

    Select output data for edge mapping
\n Used for ipe_changeParam().
*/
typedef enum
{
    EOUT_EEXT = 0,      ///< Select 5x5 user defined kernel output for edge mapping output
    EOUT_EEXT2 = 1,     ///< Select 3x3 fixed kernel output for edge mapping output
    ENUM_DUMMY4WORD(IPE_EMAP_OUT_ENUM)
} IPE_EMAP_OUT_ENUM;

/**
    IPE edge direction kernel selection

    Select kernel for edge direction decision
*/
typedef enum
{
    EDIR_EEXT0 = 0,      ///< Select smooth kernel for edge direction decision
    EDIR_EEXT1 = 1,      ///< Select center-weighted kernel for edge direction decision
    ENUM_DUMMY4WORD(IPE_EDIR_EEXT_ENUM)
} IPE_EDIR_EEXT_ENUM;

typedef enum
{
    ETHSRC_ESD = 0,
    ETHSRC_E5 = 1,
    ETHSRC_E7 = 2,
    ENUM_DUMMY4WORD(IPE_ETH_SRC_ENUM)
} IPE_ETH_SRC_ENUM;

typedef enum
{
    ESYM_3X3 = 0,
    ESYM_5X5 = 1,
    ENUM_DUMMY4WORD(IPE_ESYM_SEL_ENUM)
} IPE_ESYM_SEL_ENUM;

/**
    IPE fixed H edge kernel subtraction selection

    Select subtraction method for fixed 3x3 H edge kernel
\n Used for ipe_changeParam().
*/
typedef enum
{
    HOPT_LMINUSR = 0,       ///< Select H subtraction method : left pixel minus right pixel
    HOPT_RMINUSL = 1,        ///< Select H subtraction method : right pixel minus left pixel
    ENUM_DUMMY4WORD(IPE_ESYM_HOPT_ENUM)
} IPE_ESYM_HOPT_ENUM;

/**
    IPE fixed V edge kernel subtraction selection

    Select subtraction method for fixed 3x3 V edge kernel
\n Used for ipe_changeParam().
*/
typedef enum
{
    HOPT_UMINUSD = 0,       ///< Select V subtraction method : upper pixel minus lower pixel
    HOPT_DMINUSU = 1,        ///< Select V subtraction method : lower pixel minus upper pixel
    ENUM_DUMMY4WORD(IPE_ESYM_VOPT_ENUM)
} IPE_ESYM_VOPT_ENUM;

//@}

typedef enum
{
    LPFSIZE_3X3 = 0,
    LPFSIZE_5X5 = 1,
    LPFSIZE_7X7 = 2,
    ENUM_DUMMY4WORD(IPE_LPF_SIZE_ENUM)
} IPE_LPF_SIZE_ENUM;

typedef enum
{
    CCRANGE_2_8 = 0,
    CCRANGE_3_7 = 1,
    ENUM_DUMMY4WORD(IPE_CC_RANGE_ENUM)
} IPE_CC_RANGE_ENUM;

typedef enum
{
    CC2_IDENTITY = 0,
    CC2_GRAY = 1,
    ENUM_DUMMY4WORD(IPE_CC2_SEL_ENUM)
} IPE_CC2_SEL_ENUM;

typedef enum
{
    CCTRL_ESD = 0,
    CCTRL_E5 = 1,
    CCTRL_E7 = 2,
    ENUM_DUMMY4WORD(IPE_CCTRL_SEL_ENUM)
} IPE_CCTRL_SEL_ENUM;

typedef enum
{
    YCTH_ORIGINAL_VALUE = 0,
    YCTH_REGISTER_VALUE = 1,
    ENUM_DUMMY4WORD(IPE_YCTH_SEL_ENUM)
} IPE_YCTH_SEL_ENUM;

typedef enum
{
    VSUB_VCH = 0,
    VSUB_YCH = 1,
    VSUB_ESD = 2,
    VSUB_E5 = 3,
    VSUB_E7 = 4,
    ENUM_DUMMY4WORD(IPE_VSUB_CH_SEL_ENUM)
} IPE_VSUB_CH_SEL_ENUM;

/**
    @name Enum IPE wait frame end selection.
*/
//@{

/**
    IPE wait frame-end selection

    Select clear flag or not before waiting for IPE frame-end
\n Used for ipe_waitFrameEnd().
*/
typedef enum
{
    IPE_NOCLRFLG = 0,       ///< Select not to clear previous flag before waiting for frame-end
    IPE_CLRFLG = 1,             ///< Select to clear previous flag before waiting for frame-end
    ENUM_DUMMY4WORD(IPE_CLR_FLG_ENUM)
} IPE_CLR_FLG_ENUM;
//@}

/**
    @name Enum IPE stripe mode for external selection.
*/
//@{

/**
    IPE stripe mode selection

    Select stripe mode for IPE operation
\n Used for ipe_changeSize().
*/
typedef enum
{
    IPE_MSTP = 0,          ///<  Multi stripe mode, IPE processes multi-stripes according to HN, HL, HM, VN, VL, VM setting
    IPE_AUTOSTP = 1,    ///<  Auto stripe mode, based on input sizes, IPE driver automatically decides stripe modes and calculates stripe sizes
    IPE_DVIMODE = 2,     ///<  DVI mode, IPE processes CCIR601 data format in single stripe mode according to HL and VL setting
    ENUM_DUMMY4WORD(IPE_STRIPE_MODE_ENUM)
} IPE_STRIPE_MODE_ENUM;
//@}

/**
    @name Enum IPE function setting mode.
*/
//@{
/**
    IPE function set/enable/disable selection

    Select IPE function update mode
\n Used for ipe_changeParam().
*/
typedef enum
{
    IPE_FUNC_ENABLE = 0,        ///< Enable functions, FuncEn |= FuncSel, 1s in FuncSel are enabled
    IPE_FUNC_DISABLE = 1,       ///< Disable functions, FuncEn &= (~FuncSel), 1s in FuncSel are disabled
    IPE_FUNC_SET = 2,               ///< Set functions, FuncEn = FuncSel, 1s in FuncSel are enabled while 0s in FuncSel are disabled
    IPE_FUNC_NOUPDATE = 3,     ///< Don't update functions, FuncEn = FuncEn, disregard FuncSel
    ENUM_DUMMY4WORD(IPE_FUNC_UPDATE_ENUM)
} IPE_FUNC_UPDATE_ENUM;
//@}

/**
    @name Enum IPE wait selection.
*/
//@{
/**
    IPE D2D wait frame-end selection

    Select wait frame-end or not for Dram2Dram operation
\n Used for ipe_Dram2Dram().
*/
typedef enum
{
    IPE_WAIT = 0,       ///< Wait for frame-end before exit api
    IPE_NOWAIT = 1,   ///< Don't wait for frame-end in api, external ipe_Dram2DramWaitEnd() should be called to make sure frame operation ends
    ENUM_DUMMY4WORD(IPE_WAIT_ENUM)
} IPE_WAIT_ENUM;
//@}

/**
    @name Enum IPE VA hnf.
*/
//@{
/**
    IPE VA hnf auto/manual selection

    Select IPE VA h normalization method
\n Used for aaa_GetVariation().
*/
typedef enum
{
    IPE_HNF_AUTO = 0,       ///< IPE driver automatically calculates hnf according to sizes
    IPE_HNF_MANUAL,        ///< IPE driver accepts manual hnf input
    ENUM_DUMMY4WORD(IPE_VA_HNF_SEL_ENUM)
} IPE_VA_HNF_SEL_ENUM;
//@}

//######################################################
// the following for structure define

/**
    IPE structure - IPE size.
*/
//@{
typedef struct _STR_IMG_SIZE
{
    UINT32    uiHSize;    ///< Horizontal image size, unit : pix
    UINT32    uiVSize;    ///< Vertical image size, uint : line
} STR_IMG_SIZE;
//@}

/**
    IPE structure - IPE stripe.
*/
//@{
typedef struct _STR_STRIPE_INFOR
{
    UINT32    uiN;        ///< Size of stripe except for last one, H unit : 16 pix, V unit : 1 line
    UINT32    uiL;        ///< Size of last stripe, H unit : 16 pix, V unit : 1 line
    UINT32    uiM;        ///< Number of total stripe - 1
    IPE_OLAP_ENUM    Olap; ///< Overlap enable selection for multi-stripe mode
} STR_STRIPE_INFOR;
//@}

/**
    IPE structure - IPE color channel.
*/
//@{
typedef struct _STR_COLOR_INFOR
{
    UINT32 uiChR;     ///< R channel information
    UINT32 uiChGr;    ///< Gr channel information
    UINT32 uiChGb;   ///< Gb channel information
    UINT32 uiChB;     ///< B channel information
} STR_COLOR_INFOR;
//@}

/**
    IPE structure - IPE Esd.
*/
//@{
typedef struct _STR_ESD_INFOR
{
    IPE_ESYM_SEL_ENUM EsymSel;
    IPE_ESYM_HOPT_ENUM EsymHopt;
    IPE_ESYM_VOPT_ENUM EsymVopt;
    UINT32 EsymDetDiv;
} STR_ESD_INFOR;
//@}

/**
    IPE structure - IPE threshold.
*/
//@{
typedef struct _STR_TH_INFOR
{
    IPE_ETH_SRC_ENUM ThSrc;
    UINT32    uiThLow;   ///< Lower bound threshold
    UINT32    uiThMid;   ///< Middle bound threshold
    UINT32    uiThHigh;  ///< Higher bound threshold
} STR_TH_INFOR;
//@}

/**
    IPE structure - IPE edge map
*/
//@{
typedef struct _STR_IPE_EDGEMAP_INFOR
{
    UINT32     uiEthrLow;                   ///< Legal range : 0~1023, for edge mapping, if(Ein < EthrA){Eout=0}, else if(EthrA <= Ein <EthrB){Eout apply EtabA and EDtabA}, else if(Ein >= EthrB){Eout apply EtabB and EDtabB}
    UINT32     uiEthrHigh;                   ///< Legal range : 0~1023, for edge mapping, if(Ein < EthrA){Eout=0}, else if(EthrA <= Ein <EthrB){Eout apply EtabA and EDtabA}, else if(Ein >= EthrB){Eout apply EtabB and EDtabB}
    UINT32     uiEtabLow;                  ///< Legal range : 0~6, for edge mapping, EDtabA table has 8 sections, each section width is (1 << etaA)
    UINT32     uiEtabHigh;                  ///< Legal range : 0~6, for edge mapping, EDtabB table has 8 sections, each section width is (1 << etaB)
    IPE_EMAP_IN_ENUM EinSel;     ///< Edge mapping input selection
} STR_IPE_EDGEMAP_INFOR;
//@}

/**
    IPE structure - IPE VD
*/
//@{
typedef struct _STR_VD_KER_INFOR
{
    IPE_VD_FILT_ENUM FiltSize;
    INT16 iAcoeff;
    INT16 iBcoeff;
    INT16 iCcoeff;
    INT16 iDcoeff;
    UINT32 uiDiv;
} STR_VD_KER_INFOR;

typedef struct _STR_VD_INFOR
{
    IPE_PRE_POST_GAM_ENUM GamSel;
    STR_VD_KER_INFOR *pKerH1;
    STR_VD_KER_INFOR *pKerV1;
    STR_VD_KER_INFOR *pKerH2;
    STR_VD_KER_INFOR *pKerV2;
} STR_VD_INFOR;
//@}

/**
    IPE structure - IPE VA.
*/
//@{
typedef struct _STR_VATH_INFOR
{
    UINT32    uiVaThLow;
    UINT32    uiVaThHigh;
} STR_VATH_INFOR;

typedef struct _STR_VAWIN_INFOR
{
    UINT32    uiStX;
    UINT32    uiStY;
    UINT32    uiHsz;
    UINT32    uiVsz;
    UINT32    uiVaNf;
} STR_VAWIN_INFOR;

typedef struct _STR_VA_INFOR
{
    IPE_VA_MODE_ENUM    VaccMode;                   ///< VA DRAM output mode selection
    IPE_VA_OUT_SEL_ENUM     VaccOutSel;
    IPE_VA_G0_SRC_ENUM   VaccG0Src;
    STR_VATH_INFOR    ThG0;
    STR_VATH_INFOR    ThG1h;
    STR_VATH_INFOR    ThG1v;
    STR_VATH_INFOR    ThG2h;
    STR_VATH_INFOR    ThG2v;
    STR_VAWIN_INFOR   WinInfor;
} STR_VA_INFOR;

typedef struct _STR_INDEP_VA_INFOR
{
    BOOL bWin0En;
    STR_VAWIN_INFOR Win0Infor;
    BOOL bWin1En;
    STR_VAWIN_INFOR Win1Infor;
    BOOL bWin2En;
    STR_VAWIN_INFOR Win2Infor;
} STR_INDEP_VA_INFOR;

typedef struct _STR_VAWIN_DATA
{
    UINT32    uiGrp0;
    UINT32    uiGrp1h;
    UINT32    uiGrp1v;
    UINT32    uiGrp2h;
    UINT32    uiGrp2v;
} STR_VAWIN_DATA;

typedef struct _STR_INDEP_VA_DATA
{
    STR_VAWIN_DATA Win0Data;
    STR_VAWIN_DATA Win1Data;
    STR_VAWIN_DATA Win2Data;
} STR_INDEP_VA_DATA;
//@}

typedef struct _STR_RGBLPF_INFOR
{
    UINT32 uiLpfW;
    UINT32 uiSonlyW;
    UINT32 uiRangeTh0;
    UINT32 uiRangeTh1;
    IPE_LPF_SIZE_ENUM FiltSize;
} STR_RGBLPF_INFOR;

typedef struct _STR_YTH1_INFOR
{
    UINT32 uiYth;
    UINT32 uiEth;
    IPE_YCTH_SEL_ENUM HitSel;
    IPE_YCTH_SEL_ENUM NHitSel;
    UINT32 uiHitValue;
    UINT32 uiNHitValue;
} STR_YTH1_INFOR;

typedef struct _STR_YTH2_INFOR
{
    UINT32 uiYth;
    IPE_YCTH_SEL_ENUM HitSel;
    IPE_YCTH_SEL_ENUM NHitSel;
    UINT32 uiHitValue;
    UINT32 uiNHitValue;
} STR_YTH2_INFOR;

typedef struct _STR_CTH_INFOR
{
    UINT32 uiEth;
    UINT32 uiYthL;
    UINT32 uiYthH;
    UINT32 uiCBthL;
    UINT32 uiCBthH;
    UINT32 uiCRthL;
    UINT32 uiCRthH;
    IPE_YCTH_SEL_ENUM HitSel;
    IPE_YCTH_SEL_ENUM NHitSel;
    UINT32 uiCBHitValue;
    UINT32 uiCBNHitValue;
    UINT32 uiCRHitValue;
    UINT32 uiCRNHitValue;
} STR_CTH_INFOR;

typedef struct _STR_CSUBOUT_INFOR
{
    UINT32 uiHIntFact;
    UINT32 uiHFracFact;
    UINT32 uiHsize;
    UINT32 uiVIntFact;
    UINT32 uiVFracFact;
    UINT32 uiVsize;
} STR_CSUBOUT_INFOR;

typedef struct _STR_VSUBOUT_INFOR
{
    IPE_VSUB_CH_SEL_ENUM ChSel;
    UINT32 uiHIntFact;
    UINT32 uiHFracFact;
    UINT32 uiHsize;
    UINT32 uiVIntFact;
    UINT32 uiVFracFact;
    UINT32 uiVsize;
} STR_VSUBOUT_INFOR;

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------


/**
    IPE structure - IPE CG Ofs
*/
//@{
typedef struct _IPE_CGOFS_PARAM
{
    IPE_CG_RANGE_ENUM GainRange;
    STR_COLOR_INFOR ColorGain;  ///< 4 channel color gain
    STR_COLOR_INFOR ColorOfs;   ///< 4 channel color offset
    BOOL bInv;
    BOOL bHinv;
    UINT32 uiMask;
} IPE_CGOFS_PARAM;
//@}

/**
    IPE structure - IPE Gbal.
*/
//@{
typedef struct _IPE_GBAL_PARAM
{
    UINT32 uiGbalThr;  ///< Legal range : 0~1023, Gbal threshold for decision
    UINT8 *puiGbalStab; ///< Legal range : 0~7, Gbalstab table defines Gbal weighting for different luminance, 0 : NO Gbal, 7 : Max Gbal
    UINT8 *puiGbalDtab; ///< Legal range : 0~7, Gbalstab table defines Gbal weighting for different variation, 0 : NO Gbal, 7 : Max Gbal
} IPE_GBAL_PARAM;
//@}

/**
    IPE structure - IPE CFA.
*/
//@{
typedef struct _IPE_CFA_PARAM
{
    UINT32      uiNsmarEdge;          ///< Legal range : 0~1023, noise margin for G interpolation, if(neighbor G variation < Nsmar){NO G edge judgment in interpolation}
    UINT32      uiNsmarDiff;          ///< Legal range : 0~1023, noise margin for G interpolation, if(neighbor G variation < Nsmar){NO G edge judgment in interpolation}
    BOOL        bGCEn;                ///< Legal range : 0~1, G channel correction enable bit
    UINT32      uiGCNbit;             ///< Legal range : 0~3, Normalize bit of G correction
    UINT32      uiDIFNbit;            ///< Legal range : 0~3, Normalize bit of difference decision
} IPE_CFA_PARAM;
//@}

/**
    IPE structure - IPE edge extraction.
*/
//@{
typedef struct _IPE_EEXT_PARAM
{
    IPE_EEXT_GAM_ENUM EextGamSel;
    IPE_EEXT_CH_ENUM EextCHSel;
    INT16 *piEdgeKerDiv;        ///< Edge extraction kernel setting
    UINT32 uiEWDir;             ///< Legal range : 0~16, 0 : 5x5 user defined edge , 16 : directed edge
    IPE_EDIR_EEXT_ENUM EDirSel;    ///< Edge kernel selection for directed edge
    UINT32 uiEDirDiv;           ///< Legal range : 0~3, 0: Normal division for directed edge kernel, 1: -1 division, 2: -2 division, 3: -3 division
    UINT8 *puiEDirTab;          ///< Legal range : 0~8, wieghting for directed edge, 0 : no direction decision, 8 : sensitive edge decision
    STR_ESD_INFOR *EsdInfo;
} IPE_EEXT_PARAM;
//@}

/**
    IPE structure - IPE edge threshold.
*/
//@{
typedef struct _IPE_ETH_PARAM
{
    STR_TH_INFOR *pEthInfor;
    UINT32 uiAdrTh0;
    UINT32 uiAdrTh1;
    UINT32 uiAdrTh2;
    UINT32 uiLofsTh;
    UINT32 uiOutHeight;
} IPE_ETH_PARAM;
//@}

/**
    IPE structure - IPE VA.
*/
//@{
typedef struct _IPE_VA_PARAM
{
    STR_VA_INFOR *pVAInfor;
    STR_VD_INFOR *pVDInfor;
    UINT32 uiAdrVA;
} IPE_VA_PARAM;
//@}

/**
    IPE structure - IPE edge process.
*/
//@{
typedef struct _IPE_EPROC_PARAM
{
    STR_IPE_EDGEMAP_INFOR *pEdgeMapTh;
    UINT8 *puiEdgeMapLut;
    STR_IPE_EDGEMAP_INFOR *pEsdMapTh;
    UINT8 *puiEsdMapLut;
    UINT8 *puiESMapLut;
    IPE_EMAP_OUT_ENUM EOutSel;
} IPE_EPROC_PARAM;
//@}

/**
    IPE structure - IPE RGB LPF
*/
//@{
typedef struct _IPE_RGBLPF_PARAM
{
    STR_RGBLPF_INFOR    RlpfInfor;
    STR_RGBLPF_INFOR    GlpfInfor;
    STR_RGBLPF_INFOR    BlpfInfor;
} IPE_RGBLPF_PARAM;
//@}

/**
    IPE structure - IPE CC.
*/
//@{
typedef struct _IPE_CC_PARAM
{
    IPE_CC_RANGE_ENUM CCRange;
    IPE_CC2_SEL_ENUM CC2Sel;
    INT16 *piCCcoef;
    UINT8 *puiFstab;
    UINT8 *puiFdtab;
} IPE_CC_PARAM;
//@}

/**
    IPE structure - IPE CST.
*/
//@{
typedef struct _IPE_CST_PARAM
{
    UINT8 uiCSTCoef0;
    UINT8 uiCSTCoef1;
    UINT8 uiCSTCoef2;
    UINT8 uiCSTCoef3;
} IPE_CST_PARAM;
//@}

/**
    IPE structure - IPE CCTRL.
*/
//@{
typedef struct _IPE_CCTRL_PARAM
{
    INT16 iIntOfs;    ///< Legal range : -128~127, Y offset, Y' = Y + Intofs
    INT16 iSatOfs;    ///< Legal range : -128~127, color saturation adjust, 0 : NO color saturation adjust
    BOOL bHueRotate;  ///< Hue adjust rotation option, 0 : NO rotation, 1 : CHUEM[n] LSB 2 bit = 0 : 0 degree rotation, 1 : 90 degree rotation, 2 bit = 2 : 180 degree rotation, 3 : 270 degree rotation
    IPE_CCTRL_SEL_ENUM CctrlSel;
    UINT8 uiVdetDiv;
    UINT8 *puiHueTab; ///< Legal range : 0~255, color control hue adjust table, 128 : NO hue adjust
    INT32 *piSatTab; ///< Legal range : -128~127, color control sat adjust table, 0 : NO sat adjust, -128 : Min sat suppress, 127 : Max sat enhance
    INT32 *piIntTab; ///< Legal range : -128~127, color control int adjust table, 0 : NO int adjust, -128 : Min int suppress, 127 : Max int enhance
    UINT8 *puiEdgTab; ///< Legal range : 0~255, color control edge adjust table, 128 : NO edge adjust
    UINT8 *puiDDSTab; ///< Legal range : 0~255, color control weighting table for SatTab, IntTab, and EdgTab, 0 : NO adjust, 32 : 1X adjust weighting
} IPE_CCTRL_PARAM;
//@}

/**
    IPE structure - IPE CADJ Eenh.
*/
//@{
typedef struct _IPE_CADJ_EENH_PARAM
{
    UINT32 uiEenhP;
    UINT32 uiEenhN;
} IPE_CADJ_EENH_PARAM;
//@}

/**
    IPE structure - IPE CADJ edge inverse.
*/
//@{
typedef struct _IPE_CADJ_EINV_PARAM
{
    BOOL bEInvP; ///< Legal range : 0~1, if EInvP is 1, positive edge(overshoot) is inverted to negative edge(undershoot)
    BOOL bEInvN; ///< Legal range : 0~1, if EInvN is 1, negative edge(undershoot) is inverted to positive edge(overshoot)
} IPE_CADJ_EINV_PARAM;
//@}

/**
    IPE structure - IPE CADJ YC contrast.
*/
//@{
typedef struct _IPE_CADJ_YCCON_PARAM
{
    UINT8 uiYCon;    ///< Legal range : 0~255, Y contrast adjust, 128 : NO contrst adjust, Y' = (Y * Ycon) >> 7
    UINT8 uiCCon;    ///< Legal range : 0~255, CbCr contrast adjust, 128 : NO contrst adjust, C' = (C * CCon) >> 7
} IPE_CADJ_YCCON_PARAM;
//@}

/**
    IPE structure - IPE CADJ CbCr offset.
*/
//@{
typedef struct _IPE_CADJ_COFS_PARAM
{
    UINT8 uiCbOfs;    ///< Legal range : 0~255, Cb color offset, 128 : NO color offset
    UINT8 uiCrOfs;    ///< Legal range : 0~255, Cr color offset, 128 : NO color offset
} IPE_CADJ_COFS_PARAM;
//@}

/**
    IPE structure - IPE noise effect.
*/
//@{
typedef struct _IPE_CADJ_RAND_PARAM
{
    UINT32 uiYRandLevel;  ///< Legal range : 0~7, 0 : NO Y random noise, 7 : MAX Y random noise level
    UINT32 uiCRandLevel;  ///< Legal range : 0~7, 0 : NO CbCr random noise, 7 : MAX CbCr random noise level
    BOOL bRandReset;     ///< Legal range : 0~1, 0 : No reset for random pattern sequence, 1 : Frame start reset for random pattern seqeunce.
} IPE_CADJ_RAND_PARAM;
//@}

/**
    IPE structure - IPE YC fix effect.
*/
//@{
typedef struct _IPE_CADJ_FIXTH_PARAM
{
    STR_YTH1_INFOR *pYth1;
    STR_YTH2_INFOR *pYth2;
    STR_CTH_INFOR *pCth;
} IPE_CADJ_FIXTH_PARAM;
//@}

/**
    IPE structure - IPE YC mask effect.
*/
//@{
typedef struct _IPE_CADJ_MASK_PARAM
{
    UINT8 uiYmask;
    UINT8 uiCBmask;
    UINT8 uiCRmask;
} IPE_CADJ_MASK_PARAM;
//@}

/**
    IPE structure - IPE CSTP effect.
*/
//@{
typedef struct _IPE_CSTP_PARAM
{
    UINT8 uiCSTPRatio;
} IPE_CSTP_PARAM;
//@}

/**
    IPE structure - IPE CSUB.
*/
//@{
typedef struct _IPE_CSUB_PARAM
{
    UINT32 uiInSizeH;
    UINT32 uiInSizeV;
    UINT32 uiOutSizeH;
    UINT32 uiOutSizeV;
    UINT32 uiAdrSub0;
    UINT32 uiOfsSub;
} IPE_CSUB_PARAM;
//@}

/**
    IPE structure - IPE VSUB.
*/
//@{
typedef struct _IPE_VSUB_PARAM
{
    IPE_VSUB_CH_SEL_ENUM ChSel;
    UINT32 uiInSizeH;
    UINT32 uiInSizeV;
    UINT32 uiOutSizeH;
    UINT32 uiOutSizeV;
    UINT32 uiAdrSub0;
    UINT32 uiOfsSub;
} IPE_VSUB_PARAM;
//@}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------

/**
    IPE structure - IPE open object.
*/
//@{
typedef struct _IPE_OPENOBJ
{
    void (*FP_IPEISR_CB)(UINT32 intstatus); ///< Call back function pointer to hook with IPE interrupt service routine
    UINT32 uiIpeClockSel;                   ///< IPE engine clock select, support 48, 60, 80, 120, 160, 192, 220 MHz
} IPE_OPENOBJ,*PIPE_OPENOBJ;
//@}

/**
    IPE structure - IPE input info.
*/
//@{
typedef struct _IPE_INPUTINFO
{
    IPE_IN_SRC_ENUM InSrc;
    STR_IMG_SIZE InSize;          ///< Input image H V size, effective if StripeMode is IPE_SSTP or IPE_AUTOSTP. If StripeMode is IPE_MSTP, HSTRIPE and VSTRIPE must be input
    IPE_CFA_PAT_ENUM CfaPat;        ///< IPE input Bayer data start CFA selection
    IPE_DCE_EMBED_ENUM EmbedDCEEn;
    IPE_STRIPE_MODE_ENUM StripeMode; ///< IPE stripe mode selection
    STR_STRIPE_INFOR HStripe;     ///< Input image H stripe, effective if StripeMode is IPE_MSTP, stripe info should be calculated to match IME stripe
    STR_STRIPE_INFOR VStripe;     ///< Input image V stripe, effective if StripeMode is IPE_MSTP, stripe info should be calculated to match IME stripe
    UINT32 uiAdr0;                  ///< Legal range : multiples of 4(word aligned), IPE input address ID 0
    UINT32 uiAdr1;                  ///< Legal range : multiples of 4(word aligned), IPE input address ID 1
    IPE_PACK_BIT_ENUM PackBit;      ///< IPE input data bit number selection
    UINT32 uiLofsIn;                ///< Legal range : multiples of 4(word aligned), IPE input image DRAM line offset
    BOOL bPPBEn;                   ///< Legal range : 0~1, input ping-pong buffer enable selection, if PPBEn is 1, auto handshake between IPE and PPBISrc engine is enabled
    IPE_DVI_FORMAT_IN_ENUM DVIFormat;
    IPE_DVI_HSUB_OUT_ENUM DVIHsub;
} IPE_INPUTINFO,*PIPE_INPUTINFO;
//@}

/**
    IPE structure - IPE output info.
*/
//@{
typedef struct _IPE_OUTYCINFO
{
    BOOL bOutYCToIME;
    BOOL bOutYCToDRAM;
    IPE_OUT_FORMAT_ENUM YCFormat;       ///< IPE DRAM output source data selection
    IPE_HSUB_SEL_ENUM SubHSel;      ///< IPE output YCC subsample selection
    UINT32 uiAdrY0;                 ///< Legal range : multiples of 4(word aligned), DRAM output address, data type selected by OutSrcSel
    UINT32 uiAdrC0;                 ///< Legal range : multiples of 4(word aligned), DRAM output address, data type selected by OutSrcSel
    UINT32 uiAdrY1;                 ///< Legal range : multiples of 4(word aligned), DRAM output address, data type selected by OutSrcSel
    UINT32 uiAdrC1;                 ///< Legal range : multiples of 4(word aligned), DRAM output address, data type selected by OutSrcSel
    UINT32 uiLofsY;                ///< Legal range : multiples of 4(word aligned), DRAM output address, data type selected by OutSrcSel
    UINT32 uiLofsC;                ///< Legal range : multiples of 4(word aligned), DRAM output address, data type selected by OutSrcSel
    UINT32 uiOutHeight;
    BOOL bPPBEn;                   ///< Legal range : 0~1, input ping-pong buffer enable selection, if PPBEn is 1, auto handshake between IPE and PPBISrc engine is enabled
 } IPE_OUTYCINFO,*PIPE_OUTYCINFO;
//@}

/**
    IPE structure - IPE IQ info.
*/
//@{
typedef struct _IPE_IQINFO
{
    UINT32 uiFuncSel;                                   ///< IPE function set/enable/disable selection, combined usage with FuncUpdateSel
    IPE_FUNC_UPDATE_ENUM FuncUpdateSel;                 ///< IPE function set/enable/disable selection
    UINT32 uiParamUpdateSel;                            ///< IPE parameters update selection, combined usage with definition IPE_SET_XXX
    IPE_CGOFS_PARAM *pCGOfsParam;
    IPE_GBAL_PARAM *pGbalParam;
    IPE_CFA_PARAM *pCfaParam;                         ///< CFA related parameters
    IPE_EEXT_PARAM *pEextParam;
    IPE_ETH_PARAM *pEthParam;
    IPE_EPROC_PARAM *pEprocParam;
    IPE_VA_PARAM *pVaParam;                                 ///< VA related parameters
    STR_INDEP_VA_INFOR *pIndVaParam;
    IPE_RGBLPF_PARAM *pRGBLpfParam;
    IPE_CC_PARAM *pCCParam;
    IPE_CST_PARAM *pCSTParam;
    IPE_CCTRL_PARAM *pCCtrlParam;
    IPE_CADJ_EENH_PARAM *pCAdjEenhParam;
    IPE_CADJ_EINV_PARAM *pCAdjEinvParam;
    IPE_CADJ_YCCON_PARAM *pCAdjYCConParam;
    IPE_CADJ_COFS_PARAM *pCAdjCofsParam;
    IPE_CADJ_RAND_PARAM *pCAdjRandParam;
    IPE_CADJ_FIXTH_PARAM *pCAdjFixThParam;
    IPE_CADJ_MASK_PARAM *pCAdjMaskParam;
    IPE_CSTP_PARAM *pCstpParam;
    IPE_CSUB_PARAM *pCsubParam;
    IPE_VSUB_PARAM *pVsubParam;
    UINT32 uiGammaAddr;
    UINT32 uiCC3DAddr;
} IPE_IQINFO,*PIPE_IQINFO;
//@}

/**
    IPE structure - IPE mode info.
*/
//@{
typedef struct _IPE_MODEINFO
{
    UINT32 uiIntEn;                           ///< Interrupt enable selection
    IPE_INPUTINFO InInfo;           ///< IPE DRAM input related parameters
    IPE_OUTYCINFO OutInfo;    ///< IPE DRAM output related parameters
    IPE_IQINFO IQInfo;                 ///< IPE IQ related parameters
} IPE_MODEINFO,*PIPE_MODEINFO;
//@}

extern void ipe_clrFrameEnd(void);
extern void ipe_waitFrameEnd(IPE_CLR_FLG_ENUM ClrFlgSel);
extern void ipe_clrEthOutEnd(void);
extern void ipe_waitEthOutEnd(IPE_CLR_FLG_ENUM ClrFlgSel);
extern void ipe_clrVAOutEnd(void);
extern void ipe_waitVAOutEnd(IPE_CLR_FLG_ENUM ClrFlgSel);
extern UINT16 ipe_getVACnt(void);
extern void ipe_clrCsubOutEnd(void);
extern void ipe_waitCsubOutEnd(IPE_CLR_FLG_ENUM ClrFlgSel);
extern void ipe_clrVsubOutEnd(void);
extern void ipe_waitVsubOutEnd(IPE_CLR_FLG_ENUM ClrFlgSel);
extern void ipe_clrGammaInEnd(void);
extern void ipe_waitGammaInEnd(void);
extern void ipe_clr3DLutInEnd(void);
extern void ipe_wait3DLutInEnd(void);
extern ER ipe_open(IPE_OPENOBJ *PIPE_OpenInfo);
extern ER ipe_close(void);
extern UINT32 ipe_getClockRate(void);
extern ER ipe_changeParam(IPE_IQINFO *IQinfo);
extern ER ipe_changeInterrupt(UINT32 IntEn);
extern ER ipe_changeInput(IPE_INPUTINFO *InInfo);
extern ER ipe_changeOutYC(IPE_OUTYCINFO *OutInfo);
extern ER ipe_setMode(IPE_MODEINFO *ModeInfo);
extern ER ipe_start(void);
extern ER ipe_pause(void);
extern ER ipe_reset(BOOL ResetEn);
extern ER ipe_loadGamma(void);
extern ER ipe_load3DLut(void);
extern ER ipe_readGamma(BOOL EnableCPURead);
extern ER ipe_read3DLut(BOOL EnableCPURead);

extern UINT32 ipe_getVersion(void);
extern UINT32 ipe_getIntStatus(void);
extern void ipe_getVA_Infor(STR_VA_INFOR *VA_Infor);
extern void ipe_getIndepVarAccInfo(STR_INDEP_VA_INFOR *indvainfor);
extern void ipe_getIndepVarData(STR_INDEP_VA_DATA *indvadata);
extern UINT32 ipe_getCFAPat(void);
extern STR_STRIPE_INFOR ipe_getHstripe(void);
extern STR_STRIPE_INFOR ipe_getVstripe(void);
extern BOOL ipe_checkFunctionEnable(UINT32 ipe_Function);
extern UINT32 ipe_getDebugStatus(void);
extern void ipe_clearInt(unsigned long intr);
extern BOOL ipe_checkBusy(void);
extern UINT32 ipe_getEdgeThCnt(void);
extern UINT16 ipe_getFrmCnt(void);
extern UINT32 ipe_getIntEnable(void);
extern UINT32 ipe_getPPBOutID(void);
extern UINT32 ipe_getEthPPBOutID(void);
extern UINT32 ipe_getInSource(void);
extern UINT32 ipe_getPPBInID(void);
//@}

#endif
