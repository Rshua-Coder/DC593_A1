/**
    PLL Configuration module header

    PLL Configuration module header file.

    @file       pll.h
    @ingroup    mIHALSysCG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _PLL_H
#define _PLL_H

#include "Type.h"

/** \addtogroup mIHALSysCG */
//@{

/**
    Clock Enable ID

    This is for pll_enableClock() and pll_disableClock().
*/
typedef enum
{
    DMA_CLK = 1,        ///< DMA(SDRAM) clock
    SIE_MCLK,           ///< SIE MCLK
    SIE_MCLK2,          ///< SIE MCLK2
    SIE_CLK,            ///< SIE clock
    SIE2_CLK,           ///< SIE2 Clock
    PRE_CLK,            ///< PRE clock
    IPE_CLK,            ///< IPE clock
    DIS_CLK = 8,        ///< DIS clock
    IME_CLK,            ///< IME clock
    FDE_CLK,            ///< FDE clock
    ISE_CLK,            ///< ISE clock
    SP_CLK,             ///< special clock
    IFE_CLK,            ///< IFE clock
    DCE_CLK,            ///< DCE clock
    IFE2_CLK,           ///< IFE2 clock
    IDE1_CLK = 16,      ///< IDE clock
    IDE2_CLK,           ///< IDE2 clock
    TV_CLK,             ///< TV clock
    MI_CLK = 20,        ///< MI clock
    CEC_CLK,            ///< CEC clock
    HDMI_CLK,           ///< HDMI clock
    CRYPTO_CLK,         ///< Crypto clock
    H264_CLK = 24,      ///< H.264 clock
    AFFINE_CLK,         ///< Affine clock
    JPG_CLK,            ///< JPEG clock
    GRAPH_CLK,          ///< Graphic clock
    GRAPH2_CLK,         ///< Graphic2 clock
    DAI_CLK,            ///< DAI clock
    EAC_A_ADC_CLK,      ///< EAC analog AD clock
    EAC_A_DAC_CLK,      ///< EAC analog DA clock

    NAND_CLK = 32,      ///< NAND clock
    SDIO_CLK = 34,      ///< SDIO clock
    SDIO2_CLK,          ///< SDIO2 clock
    I2C_CLK,            ///< I2C clock
    I2C2_CLK,           ///< I2C2 clock
    SPI_CLK,            ///< SPI clock
    SPI2_CLK,           ///< SPI2 clock
    SPI3_CLK = 40,      ///< SPI3 clock
    SIF_CLK,            ///< SIF clock
    UART_CLK,           ///< UART clock
    UART2_CLK,          ///< UART2 clock
    REMOTE_CLK,         ///< Remote clock
    ADC_CLK,            ///< ADC clock
    RTC_CLK = 48,       ///< RTC clock
    WDT_CLK ,           ///< WDT clock
    TMR_CLK,            ///< Timer clock
    USB_CLK,            ///< USB clock
    EAC_D_CLKEN,        ///< EAC digital clock

    LVDS_CLK = 64,      ///< LVDS clock
    MIPI_CLK,           ///< MIPI clock
    MIPI2_CLK,          ///< MIPI2 clock
    MIPI_DSI_CLK,       ///< MIPI DSI clock
    SIE_PXCLK,          ///< SIE PX clock
    SIE2_PXCLK,         ///< SIE2 PX clock
    RDE_CLK,            ///< RDE clock
    PWM_CCNT_CLK = 72,  ///< PWM CCNT clock
    PWM_CCNT0_CLK = 72, ///< PWM CCNT0 clock
    PWM_CCNT1_CLK,      ///< PWM CCNT1 clock
    PWM_CCNT2_CLK,      ///< PWM CCNT2 clock
    PWM_CCNT3_CLK,      ///< PWM CCNT3 clock

    PWM0_CLK = 96,      ///< PWM0 clock
    PWM1_CLK,           ///< PWM1 clock
    PWM2_CLK,           ///< PWM2 clock
    PWM3_CLK,           ///< PWM3 clock
    PWM4_CLK,           ///< PWM4 clock
    PWM5_CLK,           ///< PWM5 clock
    PWM6_CLK,           ///< PWM6 clock
    PWM7_CLK,           ///< PWM7 clock
    PWM8_CLK,           ///< PWM8 clock
    PWM9_CLK,           ///< PWM9 clock
    PWM10_CLK,          ///< PWM10 clock
    PWM11_CLK,          ///< PWM11 clock
    PWM12_CLK,          ///< PWM12 clock
    PWM13_CLK,          ///< PWM13 clock
    PWM14_CLK,          ///< PWM14 clock
    PWM15_CLK,          ///< PWM15 clock
    PWM16_CLK,          ///< PWM16 clock
    PWM17_CLK,          ///< PWM17 clock
    PWM18_CLK,          ///< PWM18 clock
    PWM19_CLK,          ///< PWM19 clock


    ENUM_DUMMY4WORD(CG_EN)
} CG_EN;


/**
    Clock Gating Select ID

    This is for pll_setClkAutoGating() / pll_clearClkAutoGating() /
    pll_getClkAutoGating() / pll_setPclkAutoGating() /
    pll_clearPclkAutoGating() / pll_getPclkAutoGating().
*/
typedef enum
{
    SIE_GCLK,           ///< Gating SIE clock
    PRE_GCLK,           ///< Gating PRE clock (Gating module clock only)
    IPE_GCLK,           ///< Gating IPE clock
    IME_GCLK,           ///< Gating IME clock
    DIS_GCLK,           ///< Gating DIS clock
    FDE_GCLK,           ///< Gating FDE clock
    DCE_GCLK,           ///< Gating DCE clock
    IFE_GCLK,           ///< Gating IFE clock
    GRA_GCLK = 8,       ///< Gating Graphic clock
    GRA2_GCLK,          ///< Gating Graphic2 clock
    IDE_GCLK,           ///< Gating IDE clock
    IDE2_GCLK,          ///< Gating IDE2 clock
    MI_GCLK,            ///< Gating MI clock
    HDMI_GCLK = 13,     ///< Gating HDMI PCLK (Gating PCLK only)
    IFE2_GCLK = 14,     ///< Gating IFE2 clock
    JPG_GCLK,           ///< Gating JPEG clock
    H264_GCLK = 16,     ///< Gating H264 clock
    DAI_GCLK,           ///< Gating DAI clock
    EAC_GCLK,           ///< Gating EAC clock
    NAND_GCLK,          ///< Gating xD/Nand clock
    SDIO_GCLK,          ///< Gating SDIO clock
    SDIO2_GCLK,         ///< Gating SDIO2 clock
    I2C_GCLK,           ///< Gating I2C clock
    I2C2_GCLK,          ///< Gating I2C2 clock
    SPI_GCLK = 24,      ///< Gating SPI clock
    SPI2_GCLK,          ///< Gating SPI2 clock
    SPI3_GCLK,          ///< Gating SPI3 clock
    SIF_GCLK,           ///< Gating SIF clock
    UART_GCLK,          ///< Gating UART clock
    UART2_GCLK,         ///< Gating UART2 clock
    RM_GCLK,            ///< Gating Remote clock
    ADC_GCLK,           ///< Gating ADC clock

    TMR_GCLK = 32,      ///< Gating TMR clock
    WDT_GCLK,           ///< Gating Watchdog clock
    LVDS_GCLK,          ///< Gating LVDS clock
    MIPI_GCLK,          ///< Gating MIPI clock
    MIPI2_GCLK,         ///< Gating MIPI2 clock
    TV_GCLK,            ///< Gating TV clock
    MIPI_DSI_GCLK,      ///< Gating MIPI DSI clock
    ISE_GCLK = 40,      ///< Gating ISE clock
    SIE2_GCLK,          ///< Gating SIE2 clock
    AFFINE_GCLK,        ///< Gating AFFINE clock
    PWM_GCLK,           ///< Gating PWM clock
    PWMCCNT_GCLK,       ///< Gating PWM CCNT clock (Gating module clock only)
    RDE_GCLK,           ///< Gating RDE clock

    GPIO_GCLK = 64,     ///< Gating GPIO PCLK (Gating PCLK only)
    INTC_GCLK,          ///< Gating INTC PCLK (Gating PCLK only)
    DMA_GCLK,           ///< Gating DMAC PCLK (Gating PCLK only)

    PCLKGAT_MAXNUM,

    ENUM_DUMMY4WORD(GATECLK)
}GATECLK;

/*
    @name Default Gating Clock Select definition

    This is for pll_configClkAutoGating() & pll_configPclkAutoGating().
*/
//@{
#define PLL_CLKSEL_DEFAULT_CLKGATE1     0x00000000
#define PLL_CLKSEL_DEFAULT_CLKGATE2     0x00000000
#define PLL_CLKSEL_DEFAULT_PCLKGATE1    0x00000000
#define PLL_CLKSEL_DEFAULT_PCLKGATE2    0x00000000
#define PLL_CLKSEL_DEFAULT_PCLKGATE3    0x00000000
//@}


/**
    PLL ID
*/
typedef enum
{
    PLL_ID_1,               ///< PLL1 (internal 480 MHz)
    PLL_ID_2,               ///< PLL2 (for video/HDMI)
    PLL_ID_3,               ///< PLL3 (for DMA)
    PLL_ID_4,               ///< PLL4 (for SSPLL)
    PLL_ID_5,               ///< PLL5 (for sensor/MCLK)
    PLL_ID_6,               ///< PLL6 (for IPP)
    PLL_ID_7,               ///< PLL7 (for audio)
    PLL_ID_8,               ///< PLL8 (for CPU)

    ENUM_DUMMY4WORD(PLL_ID)
} PLL_ID;

/**
    PLL Clock ID

    @note This is for pll_setClkFreq(), pll_getClkFreq(), pll_selectClkSrc(), pll_getClkSrc()
*/
typedef enum
{
    PLL_CLK_SIEMCLK,        ///< SIE MCLK
    PLL_CLK_SIEMCLK2,       ///< SIE MCLK2
    PLL_CLK_SPCLK,          ///< SP CLK

    ENUM_DUMMY4WORD(PLL_CLK)
} PLL_CLK;

/**
    PLL Clock Source

    @note This is for pll_selectClkSrc(), pll_getClkSrc()
*/
typedef enum
{
    PLL_CLKSRC_480,         ///< CLK source is 480MHz
    PLL_CLKSRC_PLL5,        ///< CLK source is PLL5


    PLL_CLKSRC_UNKNOWN,     ///< unkown clock source

    ENUM_DUMMY4WORD(PLL_CLKSRC)
} PLL_CLKSRC;


extern ER       pll_setPLL(PLL_ID id, UINT32 uiSetting);
extern BOOL     pll_getPLLEn(PLL_ID id);
extern ER       pll_setPLLEn(PLL_ID id, BOOL bEnable);
extern UINT32   pll_getPLLFreq(PLL_ID id);
extern ER       pll_setPLLSpreadSpectrum(PLL_ID id, UINT32 uiLowerFreq, UINT32 uiUpperFreq);
extern ER       pll_getPLLSpreadSpectrum(PLL_ID id, UINT32* puiLowerFreq, UINT32* puiUpperFreq);

extern void     pll_setPWMClockRate(UINT32 uiPWMNumber, UINT32 uiDiv);
extern BOOL     pll_isClockEnabled(CG_EN Num);

extern void     pll_setClkAutoGating(GATECLK ClkSel);
extern void     pll_clearClkAutoGating(GATECLK ClkSel);
extern BOOL     pll_getClkAutoGating(GATECLK ClkSel);

extern void     pll_setPclkAutoGating(GATECLK ClkSel);
extern void     pll_clearPclkAutoGating(GATECLK ClkSel);
extern BOOL     pll_getPclkAutoGating(GATECLK ClkSel);

extern ER       pll_setClkFreq(PLL_CLK clkID, UINT32 uiFreq);
extern ER       pll_getClkFreq(PLL_CLK clkID, UINT32* puiFreq);

extern ER       pll_selectClkSrc(PLL_CLK clkID, PLL_CLKSRC clkSrc);
extern ER       pll_getClkSrc(PLL_CLK clkID, PLL_CLKSRC* pClkSrc);

extern ER       pll_setClkEn(PLL_CLK clkID, BOOL bEnable);
extern ER       pll_getClkEn(PLL_CLK clkID, BOOL* pEnable);
//@}

#endif



