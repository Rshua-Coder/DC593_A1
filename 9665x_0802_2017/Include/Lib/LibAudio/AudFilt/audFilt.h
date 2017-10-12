/**
    Audio Filtering library Driver header file.

    This file is the driver of Audio Filtering library.

    @file       audFilt.h
    @ingroup    mIAudFilt
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _AUDFILT_H
#define _AUDFILT_H

#define IM_DBG  ENABLE
#if 0//!IM_DBG
// For Reference only
#define ENUM_DUMMY4WORD(name)   E_##name = 0x10000000
typedef unsigned long           UINT32;     // Unsigned 32 bits data type
typedef signed long             INT32;      // Signed 32 bits data type
typedef unsigned int            BOOL;
/*
#ifndef TRUE
#define TRUE (0 == 0)
#endif
#ifndef FALSE
#define FALSE (!TRUE)
#endif
*/
#else
#include "Type.h"
#endif

/**
    @addtogroup mIAudFilt
*/
//@{

/**
    Audio Filter Maximum Section Number

    This value multiplied by 2 would also become the maximum filter orders.
*/
//@{
#define AUDFILT_MAX_SECNO  5
//@}

/**
    The Audio Filter Channel Number Select

    This field is used in the structure AUDFILT_INIT in AudFilt_Open() API.
*/
typedef enum
{
    AUDFILT_CH_MONO   = 1,  ///< Audio Filter uses MONO in/out
    AUDFILT_CH_STEREO,      ///< Audio Filter uses Stereo in/out

    ENUM_DUMMY4WORD(AUDFILT_CHANNEL)
}AUDFILT_CHANNEL;


/**
    The sturcture for Audio Filter initialization

    This field is used in APIs: AudFilt_Open().
*/
typedef struct
{
    AUDFILT_CHANNEL     FiltCH;         ///< The Number of Audio channel.
    BOOL                bSmoothEn;      ///< Enable/Disable for the Smooth function during the transition of filter Enable/Disable.

}AUDFILT_INIT,*PAUDFILT_INIT;


/**
    Audio Filter Selection Identification.

    Audio Filter Selection Identification.
    This field is used in APIs: AudFilt_SetConfig() / AudFilt_EnableFilt().
*/
typedef enum
{
    AUDFILT_SEL_NOTCH1,     ///< 1st Notch Filter Selection ID
    AUDFILT_SEL_NOTCH2,     ///< 2nd Notch Filter Selection ID
    AUDFILT_SEL_NOTCH3,     ///< 3rd Notch Filter Selection ID
    AUDFILT_SEL_NOTCH4,     ///< 4th Notch Filter Selection ID
    AUDFILT_SEL_NOTCH5,     ///< 5th Notch Filter Selection ID
    AUDFILT_SEL_HIGHPASS1,  ///< 1st Highpass Filter Selection ID
    AUDFILT_SEL_BANDSTOP1,  ///< 1st BandStop Filter Selection ID
    AUDFILT_SEL_LOWPASS1,   ///< 1st Lowpass Filter Selection ID

    AUDFILT_SEL_TOTAL,      // Total Number of the filter Selection
    ENUM_DUMMY4WORD(AUDFILT_SELECTION)
}AUDFILT_SELECTION;

/**
    Audio Filter Coefficient Remap

    These definitions are used in AudFilt_SetConfig() to remap the filer coefficients into designed format.
*/
//@{
#define AUDFILT_ORDER(x)    (((x)+1)&0xE)                             ///< The filter order can only use 2/4/6/8/10 in current design.
#define AUDFILT_GAIN(x)     ((INT32)((FLOAT)((FLOAT)(x))*65536.0))    ///< Remap floating point input coefficients into fixed point value. The input value allowed range is from -32768.0 to 32767.999999.(Q15.16)
#define AUDFILT_COEF(x)     ((INT32)((FLOAT)((FLOAT)(x))*134217728.0))///< Remap floating point input coefficients into fixed point value. The input value allowed range is from -16.0 to 15.999999.(Q4.27)
//@}

/**
    Audio Filter Section Coefficients Select

    These definitions are used in AudFilt_SetConfig() to select the IIR coefficients B0/B1/B2/A0/A1/A2 for member piFiltCoef.
*/
typedef enum
{
    AUDFILT_COEFSEL_B0,     ///< IIR Section Coefficients B0
    AUDFILT_COEFSEL_B1,     ///< IIR Section Coefficients B1
    AUDFILT_COEFSEL_B2,     ///< IIR Section Coefficients B2
    AUDFILT_COEFSEL_A0,     ///< IIR Section Coefficients A0
    AUDFILT_COEFSEL_A1,     ///< IIR Section Coefficients A1
    AUDFILT_COEFSEL_A2,     ///< IIR Section Coefficients A2

    AUDFILT_COEFSEL_TOTAL,  ///< Coefficient Total Number of the Filter Section
    ENUM_DUMMY4WORD(AUDFILT_COEFSEL)
}AUDFILT_COEFSEL;


/**
    Structure for Filter configurations

    This Structure is used for Filter configurations, such as filter-order and Filter-Coeficients.
    The filter order can support for 2/ 4/ 6/ 8/ 10 only in current implementation.
*/
typedef struct
{
    UINT32      uiFiltOrder;                        ///< Available Filter Order in current Design is 2/ 4/ 6/ 8/ 10.
    INT32       iTotalGain;                         ///< This is the section output total-Gain and Format is "Q15.16".

    INT32       piFiltCoef[AUDFILT_MAX_SECNO][AUDFILT_COEFSEL_TOTAL];   ///< One Section is 2 order of the IIR Filter.
                                                    ///< The 6 Parameter in order are b0,b1,b2,a0,a1,a2 in filter design.
                                                    ///< Each Parameter's format is "Q4.27".
    INT32       piSectionGain[AUDFILT_MAX_SECNO];   ///< The Section Input Gain Format is "Q15.16".
}AUDFILT_CONFIG,*PAUDFILT_CONFIG;

/**
    Structure for audio filtering input BitStream information

    Structure for audio filtering input BitStream information, such as BS length and BS buffer address.
*/
typedef struct
{
    UINT32      uiBsAddrIn;     ///< The Audio input BitStream buffer Address.
    UINT32      uiBsAddrOut;    ///< The Audio output BitStream buffer Address.

    UINT32      uiBsLength;     ///< The Audio input BitStream Length in Byte count.
                                ///< This size must be multiples of 256 Samples.(MONO:512Bytes.  STEREO:1024Bytes)
}AUDFILT_BITSTREAM,*PAUDFILT_BITSTREAM;



//
//  Export APIs
//
extern BOOL     AudFilt_Open(PAUDFILT_INIT pFiltInit);
extern BOOL     AudFilt_IsOpened(void);
extern BOOL     AudFilt_Close(void);
extern BOOL     AudFilt_Init(void);

extern BOOL     AudFilt_SetConfig(AUDFILT_SELECTION FiltSel, PAUDFILT_CONFIG pFiltConfig);
extern BOOL     AudFilt_EnableFilt(AUDFILT_SELECTION FiltSel, BOOL bEnable);

extern BOOL     AudFilt_ApplyFiltering(PAUDFILT_BITSTREAM  pAudBitStream);

//@}

#endif
