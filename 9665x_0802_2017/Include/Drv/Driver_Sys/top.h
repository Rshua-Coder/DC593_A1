/**
    TOP controller header

    Sets the pinmux of each module.

    @file       top.h
    @ingroup    mIHALSysTOP
    @note       Refer NT96650 data sheet for PIN/PAD naming

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#ifndef _TOP_H
#define _TOP_H

#include "Type.h"

//#define TOP_NEW_API                 (0)

/**
    @addtogroup mIHALSysTOP
*/
//@{

/**
    Debug port select ID

    Debug port select value for pinmux_select_debugport().
*/
typedef enum
{
    PINMUX_DEBUGPORT_AMBARD =       0x0000,     ///< AMBA RD
    PINMUX_DEBUGPORT_AMBAWR =       0x0001,     ///< AMBA WR
    PINMUX_DEBUGPORT_AHB =          0x0002,     ///< AHB
    PINMUX_DEBUGPORT_AHBC0 =        0x0003,     ///< AHB C0
    PINMUX_DEBUGPORT_AHBC1 =        0x0004,     ///< AHB C1
    PINMUX_DEBUGPORT_APBBRG =       0x0005,     ///< APB BRG
    PINMUX_DEBUGPORT_DDRIO =        0x0006,     ///< DDRIO
    PINMUX_DEBUGPORT_DDR =          0x0007,     ///< DDR
    PINMUX_DEBUGPORT_USB =          0x0008,     ///< USB
    PINMUX_DEBUGPORT_CKG =          0x0009,     ///< CKGen
    PINMUX_DEBUGPORT_SIE =          0x000A,     ///< SIE
    PINMUX_DEBUGPORT_IPE =          0x000B,     ///< IPE
    PINMUX_DEBUGPORT_IME =          0x000C,     ///< IME
    PINMUX_DEBUGPORT_IDE =          0x000D,     ///< IDE
    PINMUX_DEBUGPORT_DIS =          0x000E,     ///< DIS
    PINMUX_DEBUGPORT_FDE =          0x000F,     ///< FDE
    PINMUX_DEBUGPORT_JPEG =         0x0010,     ///< JPEG
    PINMUX_DEBUGPORT_H264 =         0x0011,     ///< H.264
    PINMUX_DEBUGPORT_ADM =          0x0012,     ///< ADC
    PINMUX_DEBUGPORT_SDIO1 =        0x0013,     ///< SDIO1
    PINMUX_DEBUGPORT_SDIO2 =        0x0014,     ///< SDIO2
    PINMUX_DEBUGPORT_AUDIO =        0x0015,     ///< AUDIO
    PINMUX_DEBUGPORT_IFE2 =         0x0016,     ///< IFE2
    PINMUX_DEBUGPORT_SPI =          0x0017,     ///< SPI
    PINMUX_DEBUGPORT_GRAPHIC =      0x0018,     ///< GRAPHIC
    PINMUX_DEBUGPORT_SMMC =         0x0019,     ///< XD/NAND/SMC
    PINMUX_DEBUGPORT_SSP =          0x001A,     ///< SSP (DAI)
    PINMUX_DEBUGPORT_SIF =          0x001B,     ///< SIF
    PINMUX_DEBUGPORT_HDMI =         0x001C,     ///< HDMI
    PINMUX_DEBUGPORT_REMOTE =       0x001D,     ///< Remote
    PINMUX_DEBUGPORT_I2C =          0x001E,     ///< I2C
    PINMUX_DEBUGPORT_LVDS =         0x001F,     ///< LVDS
    PINMUX_DEBUGPORT_DCE =          0x0020,     ///< DCE
    PINMUX_DEBUGPORT_IFE =          0x0021,     ///< IFE
    PINMUX_DEBUGPORT_SDIO1_FIFO =   0x0022,     ///< SDIO1 FIFO
    PINMUX_DEBUGPORT_SDIO2_FIFO =   0x0023,     ///< SDIO2 FIFO
    PINMUX_DEBUGPORT_BMC =          0x0024,     ///< BMC
    PINMUX_DEBUGPORT_IRDA1 =        0x0025,     ///< IRDA1
    PINMUX_DEBUGPORT_IRDA2 =        0x0026,     ///< IRDA2
    PINMUX_DEBUGPORT_TIMER =        0x0027,     ///< Timer
    PINMUX_DEBUGPORT_CRYPTO =       0x0028,     ///< Crypto
    PINMUX_DEBUGMIPI_OCP =          0x0029,     ///< OCP
    PINMUX_DEBUGMIPI_DSI =          0x002A,     ///< MIPI DSI
    PINMUX_DEBUGMIPI_CSI =          0x002B,     ///< MIPI CSI
    PINMUX_DEBUGPORT_PWM =          0x002C,     ///< PWM
    PINMUX_DEBUGPORT_IDE2 =         0x002D,     ///< IDE2
    PINMUX_DEBUGPORT_GRAPHIC2 =     0x002E,     ///< GRAPHIC2
    PINMUX_DEBUGPORT_AFFINE =       0x002F,     ///< Affine
    PINMUX_DEBUGPORT_PRE =          0x0030,     ///< PRE
    PINMUX_DEBUGPORT_ISE =          0x0031,     ///< ISE
    PINMUX_DEBUGPORT_INTC =         0x0032,     ///< INTC
    PINMUX_DEBUGMIPI_CSI2 =         0x0033,     ///< MIPI CSI2
    PINMUX_DEBUGMIPI_SIE2 =         0x0034,     ///< SIE2
    PINMUX_DEBUGPORT_I2C2 =         0x0035,     ///< I2C2
    PINMUX_DEBUGPORT_SPI2 =         0x0036,     ///< SPI2
    PINMUX_DEBUGPORT_SPI3 =         0x0037,     ///< SPI3

    PINMUX_DEBUGPORT_GROUP_NONE =   0x0000,     ///< No debug port is output
    PINMUX_DEBUGPORT_GROUP1 =       0x0100,     ///< Output debug port to MC[18..0]
    PINMUX_DEBUGPORT_GROUP2 =       0x0200,     ///< Output debug port to LCD[18..0]

    ENUM_DUMMY4WORD(PINMUX_DEBUGPORT)
} PINMUX_DEBUGPORT;

/**
    OCP Debug port select ID

    Debug port select value for pinmux_select_ocp_debugport().
*/
typedef enum
{
    PINMUX_DEBUGPORT_OCPCMD_DMA_APB =   0x0,    ///< OCP CMD vs. DMA/APB
    PINMUX_DEBUGPORT_OCPCMD_HANDSHAKE = 0x1,    ///< OCP CMD handshaking
    PINMUX_DEBUGPORT_OCP_DMA =          0x2,    ///< OCP vs. DMA
    PINMUX_DEBUGPORT_OCP_DEBUG_3 =      0x3,    ///< OCP debug 3
    PINMUX_DEBUGPORT_OCP_DEBUG_4 =      0x4,    ///< OCP debug 4

    ENUM_DUMMY4WORD(PINMUX_DEBUGPORT_OCP)
} PINMUX_DEBUGPORT_OCP;

/**
    Function group

    @note For pinmux_init()
*/
typedef enum
{
    PIN_FUNC_SDIO,      ///< SDIO. Configuration refers to PIN_SDIO_CFG.
    PIN_FUNC_SDIO2,     ///< SDIO2. Configuration refers to PIN_SDIO_CFG.
    PIN_FUNC_NAND,      ///< NAND. Configuration refers to PIN_NAND_CFG.
    PIN_FUNC_SENSOR,    ///< sensor interface. Configuration refers to PIN_SENSOR_CFG.
    PIN_FUNC_SENSOR2,   ///< sensor2 interface. Configuration refers to PIN_SENSOR2_CFG.
    PIN_FUNC_MIPI_LVDS, ///< MIPI/LVDS interface configuration. Configuration refers to PIN_MIPI_LVDS_CFG.
    PIN_FUNC_I2C,       ///< I2C. Configuration refers to PIN_I2C_CFG.
    PIN_FUNC_SIF,       ///< SIF. Configuration refers to PIN_SIF_CFG.
    PIN_FUNC_UART,      ///< UART. Configuration refers to PIN_UART_CFG.
    PIN_FUNC_SPI,       ///< SPI. Configuration refers to PIN_SPI_CFG.
    PIN_FUNC_REMOTE,    ///< REMOTE. Configuration refers to PIN_REMOTE_CFG.
    PIN_FUNC_PWM,       ///< PWM. Configuration refers to PIN_PWM_CFG.
    PIN_FUNC_AUDIO,     ///< AUDIO. Configuration refers to PIN_PWM_CFG.
    PIN_FUNC_LCD,       ///< LCD interface. Configuration refers to PINMUX_LCDINIT, PINMUX_PMI_CFG, PINMUX_DISPMUX_SEL.
    PIN_FUNC_LCD2,      ///< LCD2 interface. Configuration refers to PINMUX_LCDINIT, PINMUX_PMI_CFG, PINMUX_DISPMUX_SEL.
    PIN_FUNC_TV,        ///< TV interface. Configuration refers to PINMUX_TV_HDMI_CFG.
    PIN_FUNC_HDMI,      ///< HDMI. Configuration refers to PINMUX_LCDINIT, PINMUX_TV_HDMI_CFG, PINMUX_HDMI_CFG.
    PIN_FUNC_USB,       ///< USB. Configuration refers to PINMUX_USB_CFG.

    ENUM_DUMMY4WORD(PIN_FUNC)
} PIN_FUNC;

/**
    PIN config for SDIO

    @note For pinmux_init() with PIN_FUNC_SDIO or PIN_FUNC_SDIO2.\n
            For example, you can use {PIN_FUNC_SDIO, PIN_SDIO_CFG_1ST_PINMUX|PIN_SDIO_CFG_4BITS}\n
            to declare SDIO is 4 bits and located in 1st pinmux location.
*/
typedef enum
{
    PIN_SDIO_CFG_NONE,
    PIN_SDIO_CFG_4BITS = 0x01,      ///< 4 bits wide
    PIN_SDIO_CFG_8BITS = 0x02,      ///< 8 bits wide

    PIN_SDIO_CFG_1ST_PINMUX = 0x00, ///< 1st pinmux location
                                    ///< For SDIO: enable SD_CLK/SD_CMD/SD_D[0..3] on MC[16..21] (C_GPIO[16..21])
                                    ///< For SDIO2: enable SDIO_CLK/SDIO_CMD/SDIO_D[0..3] on MC[22..31] (C_GPIO[22..31])
    PIN_SDIO_CFG_2ND_PINMUX = 0x10, ///< 2nd pinmux location
                                    ///< For SDIO: enable SD_CLK/SD_CMD/SD_D[0..3] on MC[16..21] (C_GPIO[16..21]) (SDIO only has one pinmux pad group)
                                    ///< For SDIO2: When 4 bits, enable eMMC_D[0..3]/eMMC_CLK/eMMC_CMD on MC[0..3]/MC9/MC11 (C_GPIO[0..3], 9, 11)
                                    ///<            When 8 bits, enable eMMC_D[0..7]/eMMC_CLK/eMMC_CMD on MC[0..7]/MC9/MC11 (C_GPIO[0..7], 9, 11)

    ENUM_DUMMY4WORD(PIN_SDIO_CFG)
} PIN_SDIO_CFG;

/**
    PIN config for NAND

    @note For pinmux_init() with PIN_FUNC_NAND.\n
            For example, you can use {PIN_FUNC_NAND, PIN_NAND_CFG_1CS} to declare NAND with 1 CS.
*/
typedef enum
{
    PIN_NAND_CFG_NONE,
    PIN_NAND_CFG_1CS = 0x01,        ///< 1 chip select. Enable NAND_D[0..7]/NAND_CS0/NAND_WE/NAND_RE/NAND_CLE/NAND_ALE/NAND_WP/NAND_RDY on MC[0..8]/MC[10..15] (C_GPIO[0..8], C_GPIO[10..15])
    PIN_NAND_CFG_2CS = 0x02,        ///< 2 chip select. Enable NAND_D[0..7]/NAND_CS[0..1]/NAND_WE/NAND_RE/NAND_CLE/NAND_ALE/NAND_WP/NAND_RDY on MC[0..8]/MC[10..15] (C_GPIO[0..15])
    ENUM_DUMMY4WORD(PIN_NAND_CFG)
} PIN_NAND_CFG;

/**
    PIN config for Sensor

    @note For pinmux_init() with PIN_FUNC_SENSOR.\n
            For example, you can use {PIN_FUNC_SENSOR, PIN_SENSOR_CFG_10BITS|PIN_SENSOR_CFG_SHUTTER|PIN_SENSOR_CFG_MCLK}\n
            to declare sensor interface is 10 bits sensor, has SHUTTER and MCLK.
*/
typedef enum
{
    PIN_SENSOR_CFG_NONE,
    PIN_SENSOR_CFG_8BITS = 0x01,        ///< 8 bits sensor. Enable SN_D[4..11]/SN_PXCLK/SN_VD/SN_HD on HSI_D[2..4]N/HSI_D[2..4]P/HSI_CK0N/HSI_CK0P/SN_PXCLK/SN_VD/SN_HD (S_GPI[4..11]/S_GPI[25..27])
    PIN_SENSOR_CFG_10BITS = 0x02,       ///< 10 bits sensor. Enable SN_D[2..11]/SN_PXCLK/SN_VD/SN_HD on HSI_D[1..4]N/HSI_D[1..4]P/HSI_CK0N/HSI_CK0P/SN_PXCLK/SN_VD/SN_HD (S_GPI[2..11]/S_GPI[25..27])
    PIN_SENSOR_CFG_12BITS = 0x04,       ///< 12 bits sensor. Enable SN_D[0..11]/SN_PXCLK/SN_VD/SN_HD on HSI_D[0..4]N/HSI_D[0..4]P/HSI_CK0N/HSI_CK0P/SN_PXCLK/SN_VD/SN_HD (S_GPI[0..11]/S_GPI[25..27])
    PIN_SENSOR_CFG_MIPI = 0x20,         ///< MIPI sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
    PIN_SENSOR_CFG_LVDS = 0x40,         ///< LVDS sensor PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
    PIN_SENSOR_CFG_LVDS_VDHD = 0x80,    ///< Assign SN_VD/SN_HD (S_GPI[26..27]) to LVDS controller (Enable this when NT96650 LVDS is master and need to output VD/HD signal to LVDS slave sensor)

    PIN_SENSOR_CFG_SHUTTER = 0x100,     ///< Enable SN_SHUTTER on SN_DGPIO[7] (SN_DGPIO[7])
    PIN_SENSOR_CFG_MCLK = 0x200,        ///< Enable SN_MCLK on SN_MCLK (S_GPIO[24])
    PIN_SENSOR_CFG_MCLK2 = 0x400,       ///< Enable SN_MCLK2 on SN_CS (P_GPIO[56])
    PIN_SENSOR_CFG_MES0 = 0x800,        ///< Enable ME_SHUT0 on PWM0 (P_GPIO[36])
    PIN_SENSOR_CFG_MES0_2ND = 0x1000,   ///< Enable (2nd) ME_SHUT0 on PWM16 (P_GPIO[52])
    PIN_SENSOR_CFG_MES1 = 0x2000,       ///< Enable ME_SHUT1 on PWM1 (P_GPIO[37])
    PIN_SENSOR_CFG_MES1_2ND = 0x4000,   ///< Enable (2nd) ME_SHUT1 on PWM17 (P_GPIO[53])
    PIN_SENSOR_CFG_FLCTR = 0x8000,      ///< Enable FL_TRIG on FL_TRIG (S_GPIO[28])
    PIN_SENSOR_CFG_STROBE = 0x10000,    ///< Enable SN_FLASH on SN_DGPIO[6] (SN_DGPIO[6])
    PIN_SENSOR_CFG_SPCLK = 0x20000,     ///< Enable SP_CLK on SP_CLK (S_GPIO[29]

    PIN_SENSOR_CFG_DIS_VDHD = 0x40000,  ///< Force VD/HD to GPIO (normally VD/HD is automatically switched to SIE)

    ENUM_DUMMY4WORD(PIN_SENSOR_CFG)
} PIN_SENSOR_CFG;

/**
    PIN config for Sensor2

    @note For pinmux_init() with PIN_FUNC_SENSOR2.\n
            For example, you can use {PIN_FUNC_SENSOR2, PIN_SENSOR2_CFG_CCIR8BITS}\n
            to declare sensor2 interface connect a CCIR 8 bits sensor.
*/
typedef enum
{
    PIN_SENSOR2_CFG_NONE,
    PIN_SENSOR2_CFG_CCIR8BITS = 0x08,   ///< CCIR 8 bits sensor. Enable CCIR_YC[0..7]/CCIR_VD/CCIR_HD/CCIR_CLK on HSI_D[5..9]N/HSI_D[5..9]P/SN_DGPIO[4] (S_GPI[12..21]/SN_DGPIO[4])
    PIN_SENSOR2_CFG_CCIR16BITS = 0x10,  ///< CCIR 16 bits sensor. Enable CCIR_Y[0..7]/CCIR_C[0..7]/CCIR_VD/CCIR_HD/CCIR_CLK on HSI_D[2..9]N/HSI_D[2..9]P/SN_DGPIO[4] (S_GPI[4..21]/SN_DGPIO[4]).
    PIN_SENSOR2_CFG_MIPI = 0x20,        ///< MIPI sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
    PIN_SENSOR2_CFG_LVDS = 0x40,        ///< LVDS sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
    PIN_SENSOR2_CFG_LVDS_VDHD = 0x80,   ///< Assign SN_VD/SN_HD (S_GPIO[26..27]) to LVDS controller

    ENUM_DUMMY4WORD(PIN_SENSOR2_CFG)
} PIN_SENSOR2_CFG;

/**
    PIN config for MIPI/LVDS

    @note For pinmux_init() with PIN_FUNC_MIPI_LVDS.\n
            For example, you can use {PIN_FUNC_MIPI_LVDS, PIN_MIPI_LVDS_CFG_CLK0|PIN_MIPI_LVDS_CFG_DAT0|PIN_MIPI_LVDS_CFG_DAT4|PIN_MIPI_LVDS_CFG_DAT7|PIN_MIPI_LVDS_CFG_DAT9}\n
            to declare LVDS/MIPI connect sensor with CLK0/D0/D4/D7/D9 lanes.
*/
typedef enum
{
    PIN_MIPI_LVDS_CFG_NONE,

    PIN_MIPI_LVDS_CFG_CLK0 = 0x1,       ///< CLK lane 0. Enable HSI_CK0N/HSI_CK0P for LVDS/CSI/CSI2.
    PIN_MIPI_LVDS_CFG_CLK1 = 0x2,       ///< CLK lane 1. Enable HSI_CK1N/HSI_CK1P for LVDS/CSI/CSI2.

    PIN_MIPI_LVDS_CFG_DAT0 = 0x100,     ///< DATA lane 0. Enable HSI_D0N/HSI_D0P for LVDS/CSI/CSI2.
    PIN_MIPI_LVDS_CFG_DAT1 = 0x200,     ///< DATA lane 1. Enable HSI_D1N/HSI_D1P for LVDS/CSI/CSI2.
    PIN_MIPI_LVDS_CFG_DAT2 = 0x400,     ///< DATA lane 2. Enable HSI_D2N/HSI_D2P for LVDS/CSI/CSI2.
    PIN_MIPI_LVDS_CFG_DAT3 = 0x800,     ///< DATA lane 3. Enable HSI_D3N/HSI_D3P for LVDS/CSI/CSI2.
    PIN_MIPI_LVDS_CFG_DAT4 = 0x1000,    ///< DATA lane 4. Enable HSI_D4N/HSI_D4P for LVDS/CSI/CSI2.
    PIN_MIPI_LVDS_CFG_DAT5 = 0x2000,    ///< DATA lane 5. Enable HSI_D5N/HSI_D5P for LVDS/CSI/CSI2.
    PIN_MIPI_LVDS_CFG_DAT6 = 0x4000,    ///< DATA lane 6. Enable HSI_D6N/HSI_D6P for LVDS/CSI/CSI2.
    PIN_MIPI_LVDS_CFG_DAT7 = 0x8000,    ///< DATA lane 7. Enable HSI_D7N/HSI_D7P for LVDS/CSI/CSI2.
    PIN_MIPI_LVDS_CFG_DAT8 = 0x10000,   ///< DATA lane 8. Enable HSI_D8N/HSI_D8P for LVDS/CSI/CSI2.
    PIN_MIPI_LVDS_CFG_DAT9 = 0x20000,   ///< DATA lane 9. Enable HSI_D9N/HSI_D9P for LVDS/CSI/CSI2.

    ENUM_DUMMY4WORD(PIN_MIPI_LVDS_CFG)
} PIN_MIPI_LVDS_CFG;

/**
    PIN config for I2C

    @note For pinmux_init() with PIN_FUNC_I2C.\n
            For example, you can use {PIN_FUNC_I2C, PIN_I2C_CFG_CH1|PIN_I2C_CFG_CH1_2ND_PINMUX}\n
            to declare I2C channel1 both output to 1st and 2nd pinmux location.
*/
typedef enum
{
    PIN_I2C_CFG_NONE,
    PIN_I2C_CFG_CH1 = 0x01,             ///< Enable channel 1. Enable I2C_SDA/I2C_SCL (P_GPIO[0..1])
    PIN_I2C_CFG_CH1_2ND_PINMUX = 0x02,  ///< Enable 2nd pinmux of channel 1. Enable I2C_SCL/I2C_SDA on SN_SCK/SN_DAT (P_GPIO[57..58])
    PIN_I2C_CFG_CH2 = 0x10,             ///< Enable channel 2. Enable DDC_SDA/DDC_SCL (P_GPIO[28..29])
    PIN_I2C_CFG_CH2_2ND_PINMUX = 0x20,  ///< Enable 2nd pinmux of channel 2. Enable I2C2_SDA/I2C2_SCL on SN_HD/SN_VD (S_GPIO[26..27])

    ENUM_DUMMY4WORD(PIN_I2C_CFG)
} PIN_I2C_CFG;

/**
    PIN config for SIF

    *note For pinmux_init() with PIN_FUNC_SIF.\n
            For example, you can use {PIN_FUNC_SIF, PIN_SIF_CFG_CH0|PIN_SIF_CFG_CH2}\n
            to declare SIF channel0 and channel2 pinmux are enabled.
*/
typedef enum
{
    PIN_SIF_CFG_NONE,

    PIN_SIF_CFG_CH0 = 0x01,             ///< Enable SIF channel 0. Enable SN_CS/SN_SCK/SN_DAT (P_GPIO[56..58])

    PIN_SIF_CFG_CH1 = 0x02,             ///< Enable SIF channel 1. Enable LCD_CS/LCD_CK/LCD_DAT (P_GPIO[59..61])
    PIN_SIF_CFG_CH1_2ND_PINMUX = 0x04,  ///< Enable 2nd pinmux of SIF channel 1. (This will over-write setting of PIN_SIF_CFG_CH1)
                                        ///< Enable SB_CS1/SB_SCK1/SB_DAT1 on LCD[20..22] (L_GPIO[20..22])

    PIN_SIF_CFG_CH2 = 0x08,             ///< Enable SIF channel 2. Enable SB_CS2/SB_CK23/SB_DAT23 (P_GPIO[7]/P_GPIO[9..10])

    PIN_SIF_CFG_CH3 = 0x10,             ///< Enable SIF channel 3. Enable SB_CS3/SB_CK23/SB_DAT23 (P_GPIO[8..10])

    PIN_SIF_CFG_CH4 = 0x20,             ///< Enable SIF channel 4. Enable SB_CS4/SB_CK45/SB_DAT45 (P_GPIO[11]/P_GPIO[13..14])

    PIN_SIF_CFG_CH5 = 0x40,             ///< Enable SIF channel 5. Enable SB_CS5/SB_CK45/SB_DAT45 (P_GPIO[12..14])

    ENUM_DUMMY4WORD(PIN_SIF_CFG)
} PIN_SIF_CFG;

/**
    PIN config for UART

    @note For pinmux_init() with PIN_FUNC_UART.\n
            For example, you can use {PIN_FUNC_UART, PIN_UART_CFG_CH1}\n
            to declare UART1 pinmux is enabled.
*/
typedef enum
{
    PIN_UART_CFG_NONE,

    PIN_UART_CFG_CH1 = 0x01,            ///< Enable channel 1. Enable UART_TX/UART_RX (P_GPIO[15..16])

    PIN_UART_CFG_CH2 = 0x02,            ///< Enable channel 2. Enable UART2_TX/UART2_RX (P_GPIO[17..18])
    PIN_UART_CFG_CH2_CTSRTS = 0x04,     ///< Enable channel 2 HW handshake. Enable UART2_RTS/UART2_CTS (P_GPIO[19..20])

    ENUM_DUMMY4WORD(PIN_UART_CFG)
} PIN_UART_CFG;

/**
    PIN config for SPI

    @note For pinmux_init() with PIN_FUNC_SPI.\n
            For example, you can use {PIN_FUNC_SPI, PIN_SPI_CFG_CH1|PIN_SPI_CFG_CH3}\n
            to declare SPI channel1 and channel3 are enabled.
*/
typedef enum
{
    PIN_SPI_CFG_NONE,

    PIN_SPI_CFG_CH1 = 0x01,         ///< Enable channel 1. Enable SPI_DO/SPI_DI/SPI_CLK/SPI_CS on MC[0..2]/MC8 (C_GPIO[0..2]/C_GPIO[8])
    PIN_SPI_CFG_CH1_4BITS = 0x02,   ///< Enable channel 1 with 4 bits mode. Enable SPI_D[0..3]/SPI_CLK/SPI_CS on MC[0..4]/MC8 (C_GPIO[0..4]/C_GPIO[8])
    PIN_SPI_CFG_CH1_2ND_PINMUX = 0x04,///< Enable 2nd pinmux of channel 1. Enable SPI_CLK/SPI_CS/SPI_DI/SPI_DO on MC[22..25] (C_GPIO[22..25])

    PIN_SPI_CFG_CH2 = 0x10,         ///< Enable channel 2. Enable SPI2_CS/SPI2_CLK/SPI2_DO/SPI2_DI on SPI_CS/SPI_CLK/SPI_DO/SPI_DI(P_GPIO[21..24])
    PIN_SPI_CFG_CH2_2ND_PINMUX = 0x20,///< Enable 2nd pinmux of channel 2. Enable SPI2_CS/SPI2_CLK/SPI2_DO/SPI2_DI on UART2_TX/UART2_RX/UART2_RTS/UART2_CTS (P_GPIO[17..20])

    PIN_SPI_CFG_CH3 = 0x100,        ///< Enable channel 3. Eanble SPI3_CS/SPI3_DI/SPI3_CLK/SPI3_DO on SB_CS2/SB_CS3/SB_CK23/SB_DAT23 (P_GPIO[7..10])
    PIN_SPI_CFG_CH3_2ND_PINMUX = 0x200,///< Enable 2nd pinmux of channel 3. Enable SPI3_CLK/SPI3_CS/SPI3_DO/SPI3_DI on PWM[4..7] (P_GPIO[40..43])
    PIN_SPI_CFG_CH3_3RD_PINMUX = 0x400,///< Enable 3rd pinmux of channel 3. Enable SPI3_CS/SPI3_CLK/SPI3_DO/SPI3_DI on SN_CS/SN_SCK/SN_DAT/SN_DGPIO[6] (P_GPIO[56..58], SN_DGPIO[6])

    ENUM_DUMMY4WORD(PIN_SPI_CFG)
} PIN_SPI_CFG;

/**
    PIN config for REMOTE

    @note For pinmux_init() with PIN_FUNC_REMOTE.\n
            For example, you can use {PIN_FUNC_REMOTE, PIN_REMOTE_CFG_CH1}\n
            to declare remote pinmux is enabled.
*/
typedef enum
{
    PIN_REMOTE_CFG_NONE,
    PIN_REMOTE_CFG_CH1,                 ///< Enable remote rx channel 1. Enable REMOTE_RX (on P_GPIO[25])

    ENUM_DUMMY4WORD(PIN_REMOTE_CFG)
} PIN_REMOTE_CFG;

/**
    PIN config for PWM

    @note For pinmux_init() with PIN_FUNC_PWM.\n
            For example, you can use {PIN_FUNC_PWM, PIN_PWM_CFG_PWM0|PIN_PWM_CFG_PWM1|PIN_PWM_CFG_PWM2|PIN_PWM_CFG_PWM3}\n
            to declare your system need PWM channel0/1/2/3.
*/
typedef enum
{
    PIN_PWM_CFG_NONE,
    PIN_PWM_CFG_PWM0 = 0x01,            ///< Enable PWM0 (on P_GPIO[36])
    PIN_PWM_CFG_PWM1 = 0x02,            ///< Enable PWM1 (on P_GPIO[37])
    PIN_PWM_CFG_PWM2 = 0x04,            ///< Enable PWM2 (on P_GPIO[38])
    PIN_PWM_CFG_PWM3 = 0x08,            ///< Enable PWM3 (on P_GPIO[39])
    PIN_PWM_CFG_PWM4 = 0x10,            ///< Enable PWM4 (on P_GPIO[40])
    PIN_PWM_CFG_PWM5 = 0x20,            ///< Enable PWM5 (on P_GPIO[41])
    PIN_PWM_CFG_PWM6 = 0x40,            ///< Enable PWM6 (on P_GPIO[42])
    PIN_PWM_CFG_PWM7 = 0x80,            ///< Enable PWM7 (on P_GPIO[43])
    PIN_PWM_CFG_PWM8 = 0x100,           ///< Enable PWM8 (on P_GPIO[44])
    PIN_PWM_CFG_PWM9 = 0x200,           ///< Enable PWM9 (on P_GPIO[45])
    PIN_PWM_CFG_PWM10= 0x400,           ///< Enable PWM10 (on P_GPIO[46])
    PIN_PWM_CFG_PWM11= 0x800,           ///< Enable PWM11 (on P_GPIO[47])
    PIN_PWM_CFG_PWM12= 0x1000,          ///< Enable PWM12 (on P_GPIO[48])
    PIN_PWM_CFG_PWM13= 0x2000,          ///< Enable PWM13 (on P_GPIO[49])
    PIN_PWM_CFG_PWM14= 0x4000,          ///< Enable PWM14 (on P_GPIO[50])
    PIN_PWM_CFG_PWM15= 0x8000,          ///< Enable PWM15 (on P_GPIO[51])
    PIN_PWM_CFG_PWM16= 0x10000,         ///< Enable PWM16 (on P_GPIO[52])
    PIN_PWM_CFG_PWM17= 0x20000,         ///< Enable PWM17 (on P_GPIO[53])
    PIN_PWM_CFG_PWM18= 0x40000,         ///< Enable PWM18 (on P_GPIO[54])
    PIN_PWM_CFG_PWM19= 0x80000,         ///< Enable PWM19 (on P_GPIO[55])

    PIN_PWM_CFG_CCNT = 0x1000000,       ///< Enable PICNT1 (on DGPIO[0])
    PIN_PWM_CFG_CCNT2= 0x2000000,       ///< Enable PICNT2 (on DGPIO[1])
    PIN_PWM_CFG_CCNT3= 0x4000000,       ///< Enable PICNT3 on REMOTE_RX (P_GPIO[25])
    PIN_PWM_CFG_CCNT4= 0x8000000,       ///< Enable PICNT4 on SP_CLK (S_GPIO[29])

    ENUM_DUMMY4WORD(PIN_PWM_CFG)
} PIN_PWM_CFG;

/**
    PIN config for AUDIO

    @note For pinmux_init() with PIN_FUNC_AUDIO.\n
            For example, you can use {PIN_FUNC_AUDIO, PIN_AUDIO_CFG_I2S|PIN_AUDIO_CFG_MCLK}\n
            to declare pinmux of I2S and I2S MCLK are enabled.
*/
typedef enum
{
    PIN_AUDIO_CFG_NONE,
    PIN_AUDIO_CFG_I2S = 0x01,           ///< Enable I2S channel. Enable I2S_BCK/I2S_FCK/I2S_DO/I2S_DI/ (on P_GPIO[2..5])
    PIN_AUDIO_CFG_I2S_2ND_PINMUX = 0x02,///< Enable 2nd pinmux of I2S channel. Enable I2S_BCLK/I2S_SYNC/I2S_DO/I2S_DI on MC[23..26] (C_GPIO[23..26])

    PIN_AUDIO_CFG_MCLK = 0x10,          ///< Enable audio MCLK. Enable I2S_MCLK (on P_GPIO[6])
    PIN_AUDIO_CFG_MCLK_2ND_PINMUX = 0x20,///< Enable 2nd pinmux of audio MCLK. Enable I2S_MCLK on MC22 (C_GPIO[22])

    ENUM_DUMMY4WORD(PIN_AUDIO_CFG)
} PIN_AUDIO_CFG;

/**
    PIN config of LCD modes

    @note For pinmux_init() with PIN_FUNC_LCD or PIN_FUNC_LCD2.\n
            For example, you can use {PIN_FUNC_LCD, PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_RGB_SERIAL}\n
            to tell display object that PIN_FUNC_LCD is located on primary LCD pinmux,\n
            and it's LCD mode is RGB serial.
*/
typedef enum
{
    PINMUX_LCDMODE_RGB_SERIAL           =    0,     ///< LCD MODE is RGB Serial or UPS051
    PINMUX_LCDMODE_RGB_PARALL           =    1,     ///< LCD MODE is RGB Parallel (888)
    PINMUX_LCDMODE_YUV640               =    2,     ///< LCD MODE is YUV640
    PINMUX_LCDMODE_YUV720               =    3,     ///< LCD MODE is YUV720
    PINMUX_LCDMODE_RGBD360              =    4,     ///< LCD MODE is RGB Dummy 360
    PINMUX_LCDMODE_RGBD320              =    5,     ///< LCD MODE is RGB Dummy 320
    PINMUX_LCDMODE_RGB_THROUGH          =    6,     ///< LCD MODE is RGB through mode
    PINMUX_LCDMODE_CCIR601              =    7,     ///< LCD MODE is CCIR601
    PINMUX_LCDMODE_CCIR656              =    8,     ///< LCD MODE is CCIR656
    PINMUX_LCDMODE_RGB_PARALL666        =    9,     ///< LCD MODE is RGB Parallel 666
    PINMUX_LCDMODE_RGB_PARALL565        =   10,     ///< LCD MODE is RGB Parallel 565
    PINMUX_LCDMODE_RGB_PARALL_DELTA     =   11,     ///< LCD MODE is RGB Parallel Delta
    PINMUX_LCDMODE_MIPI                 =   12,     ///< LCD MODE is MIPI Display

    PINMUX_LCDMODE_MI_OFFSET            = 32,       ///< Memory LCD MODE offset
    PINMUX_LCDMODE_MI_FMT0              = 32+0,     ///< LCD MODE is Memory(Parallel Interface) 8bits
    PINMUX_LCDMODE_MI_FMT1              = 32+1,     ///< LCD MODE is Memory(Parallel Interface) 8bits
    PINMUX_LCDMODE_MI_FMT2              = 32+2,     ///< LCD MODE is Memory(Parallel Interface) 8bits
    PINMUX_LCDMODE_MI_FMT3              = 32+3,     ///< LCD MODE is Memory(Parallel Interface) 8bits
    PINMUX_LCDMODE_MI_FMT4              = 32+4,     ///< LCD MODE is Memory(Parallel Interface) 8bits
    PINMUX_LCDMODE_MI_FMT5              = 32+5,     ///< LCD MODE is Memory(Parallel Interface) 8bits
    PINMUX_LCDMODE_MI_FMT6              = 32+6,     ///< LCD MODE is Memory(Parallel Interface) 8bits
    PINMUX_LCDMODE_MI_FMT7              = 32+7,     ///< LCD MODE is Memory(Parallel Interface) 9bits
    PINMUX_LCDMODE_MI_FMT8              = 32+8,     ///< LCD MODE is Memory(Parallel Interface) 16bits
    PINMUX_LCDMODE_MI_FMT9              = 32+9,     ///< LCD MODE is Memory(Parallel Interface) 16bits
    PINMUX_LCDMODE_MI_FMT10             = 32+10,    ///< LCD MODE is Memory(Parallel Interface) 18bits
    PINMUX_LCDMODE_MI_FMT11             = 32+11,    ///< LCD MODE is Memory(Parallel Interface)  8bits
    PINMUX_LCDMODE_MI_FMT12             = 32+12,    ///< LCD MODE is Memory(Parallel Interface) 16bits
    PINMUX_LCDMODE_MI_FMT13             = 32+13,    ///< LCD MODE is Memory(Parallel Interface) 16bits
    PINMUX_LCDMODE_MI_FMT14             = 32+14,    ///< LCD MODE is Memory(Parallel Interface) 16bits
    PINMUX_LCDMODE_MI_SERIAL_BI         = 32+20,    ///< LCD MODE is Serial Interface bi-direction
    PINMUX_LCDMODE_MI_SERIAL_SEP        = 32+21,    ///< LCD MODE is Serial Interface separation

    PINMUX_LCDMODE_AUTO_PINMUX          = 0x01<<22, ///< Set display device to GPIO mode when display device is closed. Select this filed will inform display object to switch to GPIO when display device is closed.


    PINMUX_HDMIMODE_OFFSET              = 64,       ///< HDMI MODE offset
    PINMUX_HDMIMODE_640X480P60          = 64+1,     ///< HDMI Video format is 640x480 & Progressive 60fps
    PINMUX_HDMIMODE_720X480P60          = 64+2,     ///< HDMI Video format is 720x480 & Progressive 60fps & 4:3
    PINMUX_HDMIMODE_720X480P60_16X9     = 64+3,     ///< HDMI Video format is 720x480 & Progressive 60fps & 16:9
    PINMUX_HDMIMODE_1280X720P60         = 64+4,     ///< HDMI Video format is 1280x720 & Progressive 60fps
    PINMUX_HDMIMODE_1920X1080I60        = 64+5,     ///< HDMI Video format is 1920x1080 & Interlaced 60fps
    PINMUX_HDMIMODE_720X480I60          = 64+6,     ///< HDMI Video format is 720x480 & Interlaced 60fps
    PINMUX_HDMIMODE_720X480I60_16X9     = 64+7,     ///< HDMI Video format is 720x480 & Interlaced 60fps & 16:9
    PINMUX_HDMIMODE_720X240P60          = 64+8,     ///< HDMI Video format is 720x240 & Progressive 60fps
    PINMUX_HDMIMODE_720X240P60_16X9     = 64+9,     ///< HDMI Video format is 720x240 & Progressive 60fps & 16:9
    PINMUX_HDMIMODE_1440X480I60         = 64+10,    ///< HDMI Video format is 1440x480 & Interlaced 60fps
    PINMUX_HDMIMODE_1440X480I60_16X9    = 64+11,    ///< HDMI Video format is 1440x480 & Interlaced 60fps & 16:9
    PINMUX_HDMIMODE_1440X240P60         = 64+12,    ///< HDMI Video format is 1440x240 & Progressive 60fps
    PINMUX_HDMIMODE_1440X240P60_16X9    = 64+13,    ///< HDMI Video format is 1440x240 & Progressive 60fps & 16:9
    PINMUX_HDMIMODE_1440X480P60         = 64+14,    ///< HDMI Video format is 1440x480 & Progressive 60fps
    PINMUX_HDMIMODE_1440X480P60_16X9    = 64+15,    ///< HDMI Video format is 1440x480 & Progressive 60fps & 16:9
    PINMUX_HDMIMODE_720X576P50          = 64+17,    ///< HDMI Video format is 720x576 & Progressive 50fps
    PINMUX_HDMIMODE_720X576P50_16X9     = 64+18,    ///< HDMI Video format is 720x576 & Progressive 50fps & 16:9
    PINMUX_HDMIMODE_1280X720P50         = 64+19,    ///< HDMI Video format is 1280x720 & Progressive 50fps
    PINMUX_HDMIMODE_1920X1080I50        = 64+20,    ///< HDMI Video format is 1920x1080 & Interlaced 50fps
    PINMUX_HDMIMODE_720X576I50          = 64+21,    ///< HDMI Video format is 720x576 & Interlaced 50fps
    PINMUX_HDMIMODE_720X576I50_16X9     = 64+22,    ///< HDMI Video format is 720x576 & Interlaced 50fps & 16:9
    PINMUX_HDMIMODE_720X288P50          = 64+23,    ///< HDMI Video format is 720x288 & Progressive 50fps
    PINMUX_HDMIMODE_720X288P50_16X9     = 64+24,    ///< HDMI Video format is 720x288 & Progressive 50fps & 16:9
    PINMUX_HDMIMODE_1440X576I50         = 64+25,    ///< HDMI Video format is 1440x576 & Interlaced 50fps
    PINMUX_HDMIMODE_1440X576I50_16X9    = 64+26,    ///< HDMI Video format is 1440x576 & Interlaced 50fps & 16:9
    PINMUX_HDMIMODE_1440X288P50         = 64+27,    ///< HDMI Video format is 1440x288 & Progressive 50fps
    PINMUX_HDMIMODE_1440X288P50_16X9    = 64+28,    ///< HDMI Video format is 1440x288 & Progressive 50fps & 16:9
    PINMUX_HDMIMODE_1440X576P50         = 64+29,    ///< HDMI Video format is 1440x576 & Progressive 50fps
    PINMUX_HDMIMODE_1440X576P50_16X9    = 64+30,    ///< HDMI Video format is 1440x576 & Progressive 50fps & 16:9
    PINMUX_HDMIMODE_1920X1080P50        = 64+31,    ///< HDMI Video format is 1920x1080 & Progressive 50fps
    PINMUX_HDMIMODE_1920X1080P24        = 64+32,    ///< HDMI Video format is 1920x1080 & Progressive 24fps
    PINMUX_HDMIMODE_1920X1080P25        = 64+33,    ///< HDMI Video format is 1920x1080 & Progressive 25fps
    PINMUX_HDMIMODE_1920X1080P30        = 64+34,    ///< HDMI Video format is 1920x1080 & Progressive 30fps
    PINMUX_HDMIMODE_1920X1080I50_VT1250 = 64+39,    ///< HDMI Video format is 1920x1080 & Interlaced  50fps & V-total is 1250 lines
    PINMUX_HDMIMODE_1920X1080I100       = 64+40,    ///< HDMI Video format is 1920x1080 & Interlaced  100fps
    PINMUX_HDMIMODE_1280X720P100        = 64+41,    ///< HDMI Video format is 1280X720  & Progressive 100fps
    PINMUX_HDMIMODE_720X576P100         = 64+42,    ///< HDMI Video format is 720X576   & Progressive 100fps
    PINMUX_HDMIMODE_720X576P100_16X9    = 64+43,    ///< HDMI Video format is 720X576   & Progressive 100fps & 16:9
    PINMUX_HDMIMODE_720X576I100         = 64+44,    ///< HDMI Video format is 720X576  & Interlaced 100fps
    PINMUX_HDMIMODE_720X576I100_16X9    = 64+45,    ///< HDMI Video format is 720X576  & Interlaced 100fps & 16:9
    PINMUX_HDMIMODE_1920X1080I120       = 64+46,    ///< HDMI Video format is 1920X1080 & Interlaced 120fps
    PINMUX_HDMIMODE_1280X720P120        = 64+47,    ///< HDMI Video format is 1280X720  & Progressive 120fps
    PINMUX_HDMIMODE_720X480P120         = 64+48,    ///< HDMI Video format is 720X480   & Progressive 120fps
    PINMUX_HDMIMODE_720X480P120_16X9    = 64+49,    ///< HDMI Video format is 720X480   & Progressive 120fps & 16:9
    PINMUX_HDMIMODE_720X480I120         = 64+50,    ///< HDMI Video format is 720X480  & Interlaced 120fps
    PINMUX_HDMIMODE_720X480I120_16X9    = 64+51,    ///< HDMI Video format is 720X480  & Interlaced 120fps & 16:9
    PINMUX_HDMIMODE_720X576P200         = 64+52,    ///< HDMI Video format is 720X576  & Progressive 200fps
    PINMUX_HDMIMODE_720X576P200_16X9    = 64+53,    ///< HDMI Video format is 720X576  & Progressive 200fps & 16:9
    PINMUX_HDMIMODE_720X576I200         = 64+54,    ///< HDMI Video format is 720X576  & Interlaced 200fps
    PINMUX_HDMIMODE_720X576I200_16X9    = 64+55,    ///< HDMI Video format is 720X576  & Interlaced 200fps & 16:9
    PINMUX_HDMIMODE_720X480P240         = 64+56,    ///< HDMI Video format is 720X480  & Progressive 240fps
    PINMUX_HDMIMODE_720X480P240_16X9    = 64+57,    ///< HDMI Video format is 720X480  & Progressive 240fps & 16:9
    PINMUX_HDMIMODE_720X480I240         = 64+58,    ///< HDMI Video format is 720X480  & Interlaced 240fps
    PINMUX_HDMIMODE_720X480I240_16X9    = 64+59,    ///< HDMI Video format is 720X480  & Interlaced 240fps & 16:9

    PINMUX_DSI_1_LANE_CMD_MODE_RGB565           = 96+0, ///< DSI command mode with RGB565 format
    PINMUX_DSI_1_LANE_CMD_MODE_RGB666P          = 96+1, ///< DSI command mode with RGB666 packed
    PINMUX_DSI_1_LANE_CMD_MODE_RGB666L          = 96+2, ///< DSI command mode with RGB666 loosely
    PINMUX_DSI_1_LANE_CMD_MODE_RGB888           = 96+3, ///< DSI command mode with RGB888

    PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB565     = 96+4, ///< DSI video sync pulse mode with RGB565 format
    PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666P    = 96+5, ///< DSI video sync pulse mode with RGB666 packed
    PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666L    = 96+6, ///< DSI video sync pulse mode with RGB666 loosely
    PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB888     = 96+7, ///< DSI video sync pulse mode with RGB888

    PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB565     = 96+8, ///< DSI video sync event burst mode with RGB565 format
    PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666P    = 96+9, ///< DSI video sync event burst mode with RGB666 packed
    PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666L    = 96+10,///< DSI video sync event burst mode with RGB666 loosely
    PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB888     = 96+11,///< DSI video sync event burst mode with RGB888

    PINMUX_DSI_2_LANE_CMD_MODE_RGB565           = 96+12,///< DSI command mode with RGB565 format
    PINMUX_DSI_2_LANE_CMD_MODE_RGB666P          = 96+13,///< DSI command mode with RGB666 packed
    PINMUX_DSI_2_LANE_CMD_MODE_RGB666L          = 96+14,///< DSI command mode with RGB666 loosely
    PINMUX_DSI_2_LANE_CMD_MODE_RGB888           = 96+15,///< DSI command mode with RGB888

    PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB565     = 96+16,///< DSI video sync pulse mode with RGB565 format
    PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666P    = 96+17,///< DSI video sync pulse mode with RGB666 packed
    PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666L    = 96+18,///< DSI video sync pulse mode with RGB666 loosely
    PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB888     = 96+19,///< DSI video sync pulse mode with RGB888

    PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB565     = 96+20,///< DSI video sync event burst mode with RGB565 format
    PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB666P    = 96+21,///< DSI video sync event burst mode with RGB666 packed
    PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB666L    = 96+22,///< DSI video sync event burst mode with RGB666 loosely
    PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB888     = 96+23,///< DSI video sync event burst mode with RGB888




    ENUM_DUMMY4WORD(PINMUX_LCDINIT)
} PINMUX_LCDINIT;

/**
    PIN config for Parallel MI

    @note For pinmux_init() with PIN_FUNC_LCD or PIN_FUNC_LCD2.\n
            For example, you can use {PIN_FUNC_LCD, PINMUX_DISPMUX_SEL_LCD|PINMUX_PMI_CFG_NORMAL|PINMUX_LCDMODE_MI_FMT0}\n
            to tell display object that PIN_FUNC_LCD is bound to MI and format is FMT0,\n
            and MI is located at primary location.
*/
typedef enum
{
    PINMUX_PMI_CFG_NORMAL = 0x00,                   ///< Normal Parallel MI location (at LCD)
    PINMUX_PMI_CFG_2ND_PINMUX = 0x01<<26,           ///< Secondary Parallel MI location (at LGPIO[12..25])

    PINMUX_PMI_CFG_MASK = 0x03<<26,
    ENUM_DUMMY4WORD(PINMUX_PMI_CFG)
} PINMUX_PMI_CFG;

/**
    PIN config for TV/HDMI

    @note For pinmux_init() for PIN_FUNC_TV or PIN_FUNC_HDMI.\n
            For example, you can use {PIN_FUNC_HDMI, PINMUX_TV_HDMI_CFG_NORMAL|PINMUX_HDMIMODE_1280X720P60}\n
            to tell display object that HDMI activation will disable PANEL,\n
            and HDMI mode is 1280x720 P60.
*/
typedef enum
{
    PINMUX_TV_HDMI_CFG_GPIO = 0x00,                 ///< TV activation will disable PINMUX to GPIO
    PINMUX_TV_HDMI_CFG_NORMAL = 0x00,               ///< TV activation will disable PANEL which shared the same IDE
    PINMUX_TV_HDMI_CFG_PINMUX_ON = 0x01<<28,        ///< TV activation will keep PINMUX setting

    PINMUX_TV_HDMI_CFG_MASK = 0x03<<28,
    ENUM_DUMMY4WORD(PINMUX_TV_HDMI_CFG)
} PINMUX_TV_HDMI_CFG;

/**
    PIN config for HDMI

    @note For pinmux_init() for PIN_FUNC_HDMI.\n
            For example, you can use {PIN_FUNC_HDMI, PINMUX_HDMI_CFG_CEC|PINMUX_TV_HDMI_CFG_NORMAL}\n
            to declare HDMI CEC pinmux is enabled.
*/
typedef enum
{
    PINMUX_HDMI_CFG_GPIO = 0x00,                    ///< HDMI specific PIN to GPIO
    PINMUX_HDMI_CFG_HOTPLUG = 0x01<<26,             ///< HDMI HOTPLUG. Enable HDMI_PLUG (on P_GPIO[30])
    PINMUX_HDMI_CFG_CEC = 0x02<<26,                 ///< HDMI CEC. Enable HDMI_CEC (on P_GPIO[27])

    PINMUX_HDMI_CFG_MASK = 0x03<<26,
    ENUM_DUMMY4WORD(PINMUX_HDMI_CFG)
} PINMUX_HDMI_CFG;

/**
    PIN location of LCD

    @note For pinmux_init() with PIN_FUNC_LCD or PIN_FUNC_LCD2.\n
            For example, you can use {PIN_FUNC_LCD, PINMUX_DISPMUX_SEL_LCD2|PINMUX_LCDMODE_XXX}\n
            to tell display object that PIN_FUNC_LCD is located on secondary LCD pinmux.
*/
typedef enum
{
    PINMUX_DISPMUX_SEL_NONE = 0x00<<28,             ///< PINMUX none
    PINMUX_DISPMUX_SEL_LCD = 0x01<<28,              ///< PINMUX at LCD interface
    PINMUX_DISPMUX_SEL_LCD2 = 0x02<<28,             ///< PINMUX at LCD2 interface

    PINMUX_DISPMUX_SEL_MASK = 0x03<<28,
    ENUM_DUMMY4WORD(PINMUX_DISPMUX_SEL)
} PINMUX_DISPMUX_SEL;

/**
    PIN config for USB

    @note For pinmux_init() for PIN_FUNC_USB.\n
            For example, you can use {PIN_FUNC_USB, PINMUX_USB_CFG_DEVICE}\n
            to select USB as device.
*/
typedef enum
{
    PINMUX_USB_CFG_NONE = 0x00,                     ///< USB as device
    PINMUX_USB_CFG_DEVICE = 0x00,                   ///< USB as device
    PINMUX_USB_CFG_HOST = 0x01,                     ///< USB as host

    ENUM_DUMMY4WORD(PINMUX_USB_CFG)
} PINMUX_USB_CFG;

/**
    Pinmux Function identifier

    @note For pinmux_getDispMode(), pinmux_setPinmux().
*/
typedef enum
{
    PINMUX_FUNC_ID_LCD,                             ///< 1st Panel (LCD), pinmux can be:
                                                    ///< - @b PINMUX_LCD_SEL_GPIO
                                                    ///< - @b PINMUX_LCD_SEL_CCIR656
                                                    ///< - @b PINMUX_LCD_SEL_CCIR601
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_RGB_6BITS
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_RGB_8BITS
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS
                                                    ///< - @b PINMUX_LCD_SEL_PARALLE_MI_8BITS
                                                    ///< - @b PINMUX_LCD_SEL_PARALLE_MI_9BITS
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_MI_SDIO
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_MI_SDI_SDO
                                                    ///< ORed with
                                                    ///< - @b PINMUX_LCD_SEL_DE_ENABLE
                                                    ///< - @b PINMUX_LCD_SEL_TE_ENABLE
    PINMUX_FUNC_ID_LCD2,                            ///< 2nd Panel (LCD), pinmux can be:
                                                    ///< - @b PINMUX_LCD_SEL_GPIO
                                                    ///< - @b PINMUX_LCD_SEL_CCIR656
                                                    ///< - @b PINMUX_LCD_SEL_CCIR601
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_RGB_6BITS
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_RGB_8BITS
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS
                                                    ///< - @b PINMUX_LCD_SEL_PARALLE_MI_8BITS
                                                    ///< - @b PINMUX_LCD_SEL_PARALLE_MI_9BITS
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_MI_SDIO
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_MI_SDI_SDO
                                                    ///< ORed with
                                                    ///< - @b PINMUX_LCD_SEL_DE_ENABLE
    PINMUX_FUNC_ID_TV,                              ///< TV, pinmux can be:
                                                    ///< - @b PINMUX_LCD_SEL_GPIO
    PINMUX_FUNC_ID_HDMI,                            ///< HDMI, pinmux can be:
                                                    ///< - @b PINMUX_LCD_SEL_GPIO
    PINMUX_FUNC_ID_SN_MES0,                         ///< Sensor MES0, pinmux can be:
                                                    ///< - @b PINMUX_SENSOR_SEL_INACTIVE: PWM0 pad is GPIO (P_GPIO[36]) when PIN_SENSOR_CFG_MES0. PWM16 pad is GPIO (P_GPIO[52]) when PIN_SENSOR_CFG_MES0_2ND.
                                                    ///< - @b PINMUX_SENSOR_SEL_ACTIVE: enable ME_SHUT0 on PWM0 (P_GPIO[36]) when PIN_SENSOR_CFG_MES0. enable ME_SHUT0 on PWM16 (P_GPIO[52]) when PIN_SENSOR_CFG_MES0_2ND.
    PINMUX_FUNC_ID_SN_MES1,                         ///< Sensor MES1, pinmux can be:
                                                    ///< - @b PINMUX_SENSOR_SEL_INACTIVE: PWM1 pad is GPIO (P_GPIO[37]) when PIN_SENSOR_CFG_MES1. PWM17 pad is GPIO (P_GPIO[53]) when PIN_SENSOR_CFG_MES1_2ND.
                                                    ///< - @b PINMUX_SENSOR_SEL_ACTIVE: enable ME_SHUT1 on PWM1 (P_GPIO[37]) when PIN_SENSOR_CFG_MES1. enable ME_SHUT1 on PWM17 (P_GPIO[53]) when PIN_SENSOR_CFG_MES1_2ND.
    PINMUX_FUNC_ID_SN_FLCTR,                        ///< Sensor Flash Control, pinmux can be:
                                                    ///< - @b PINMUX_SENSOR_SEL_INACTIVE: FL_TRIG pad is GPIO (S_GPIO[28])
                                                    ///< - @b PINMUX_SENSOR_SEL_ACTIVE: enable FL_TRIG (on S_GPIO[28])
    PINMUX_FUNC_ID_SN_MCLK,                         ///< Sensor MCLK, pinmux can be:
                                                    ///< - @b PINMUX_SENSOR_SEL_INACTIVE: SN_MCLK pad is GPIO (S_GPIO[24])
                                                    ///< - @b PINMUX_SENSOR_SEL_ACTIVE: enable SN_MCLK (on S_GPIO[24])
    PINMUX_FUNC_ID_SPI2,                            ///< SPI channel2 pinmux switch, pinmux can be:
                                                    ///< - @b PINMUX_SPI2_SEL_INACTIVE
                                                    ///< - @b PINMUX_SPI2_SEL_ACTIVE
                                                    ///< - @b PINMUX_SPI2_SEL_ACTIVE_1ST
                                                    ///< - @b PINMUX_SPI2_SEL_ACTIVE_2ND
    PINMUX_FUNC_ID_SPI3,                            ///< SPI channel3 pinmux switch, pinmux can be:
                                                    ///< - @b PINMUX_SPI_SEL_INACTIVE
                                                    ///< - @b PINMUX_SPI_SEL_ACTIVE

    PINMUX_FUNC_ID_COUNT,                           //< Total function count
    ENUM_DUMMY4WORD(PINMUX_FUNC_ID)
} PINMUX_FUNC_ID;

/**
    Pinmux selection for LCD

    @note For PINMUX_FUNC_ID_LCD, PINMUX_FUNC_ID_LCD2
*/
typedef enum
{
    PINMUX_LCD_SEL_GPIO,                            ///< GPIO
    PINMUX_LCD_SEL_CCIR656,                         ///< CCIR-656 8 bits. Enable CCIR_YC[0..7]/CCIR_CLK on LCD[0..8] (L_GPIO[0..8])
    PINMUX_LCD_SEL_CCIR656_16BITS,                  ///< CCIR-656 16 bits. Enable CCIR_Y[0..7]/CCIR_CLK/CCIR_C[0..7] on LCD[0..8]/LCD[12..19] (L_GPIO[0..8]/L_GPIO[12..19])
    PINMUX_LCD_SEL_CCIR601,                         ///< CCIR-601 8 bits. Enable CCIR_YC[0..7]/CCIR_CLK/CCIR_VD/CCIR_HD on LCD[0..10] (L_GPIO[0..10])
    PINMUX_LCD_SEL_CCIR601_16BITS,                  ///< CCIR-601 16 bits. Enable CCIR_Y[0..7]/CCIR_CLK/CCIR_VD/CCIR_HD/CCIR_C[0..7] on LCD[0..19] (L_GPIO[0..19])
    PINMUX_LCD_SEL_SERIAL_RGB_6BITS,                ///< Serial RGB 6 bits. Enable RGB_D[2..7]/RGB_CLK/RGB_VD/RGB_HD on LCD[2..10] (L_GPIO[2..10])
    PINMUX_LCD_SEL_SERIAL_RGB_8BITS,                ///< Serial RGB 8 bits. Enable RGB_D[0..7]/RGB_CLK/RGB_VD/RGB_HD on LCD[0..10] (L_GPIO[0..10])
    PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS,              ///< Serial YCbCr 8 bits. Enable CCIR_YC[0..7]/CCIR_CLK/CCIR_VD/CCIR_HD on LCD[0..10] (L_GPIO[0..10])
    PINMUX_LCD_SEL_PARALLE_RGB565,                  ///< Parallel RGB565. Enable RGB_C0_[0..4]/RGB_DCLK/RGB_VS/RGB_HS/RGB_C1_[0..5]/RGB_C2_[0..4] on LCD[3..10]/LCD[14..19]/LCD[23..27] (L_GPIO[3..10]/L_GPIO[14..19]/L_GPIO[23..27])
    PINMUX_LCD_SEL_PARALLE_RGB666,                  ///< Parallel RGB666. Enable RGB_C0_[0..5]/RGB_DCLK/RGB_VS/RGB_HS/RGB_C1_[0..5]/RGB_C2_[0..5] on LCD[2..10]/LCD[14..19]/LCD[22..27] (L_GPIO[2..10]/L_GPIO[14..19]/L_GPIO[22..27])
    PINMUX_LCD_SEL_PARALLE_RGB888,                  ///< Parallel RGB888. Enable RGB_C0_[0..7]/RGB_DCLK/RGB_VS/RGB_HS/RGB_C1_[0..7]/RGB_C2_[0..7] on LCD[0..10]/LCD[12..27] (L_GPIO[2..10]/L_GPIO[12..27])
    PINMUX_LCD_SEL_RGB_16BITS,                      ///< RGB 16 bits. Enable CCIR_Y[0..7]/CCIR_CLK/CCIR_VD/CCIR_HD/CCIR_C[0..7] on LCD[0..19] (L_GPIO[0..19])
    PINMUX_LCD_SEL_PARALLE_MI_8BITS,                ///< Parallel MI 8 bits.
                                                    ///< When PINMUX_PMI_CFG_NORMAL is set, Enable MPU_D[0..7]/MPU_CS/MPU_RS/MPU_WR/MPU_RD on LCD[0..7]/LCD[9..12] (L_GPIO[0..7]/L_GPIO[9..12])
                                                    ///< When PINMUX_PMI_CFG_2ND_PINMUX is set, Enable MPU2_RS/MPU2_CS/MPU2_D[0..7]/MPU2_WR/MPU2_RD on LCD[12..22]/LCD[24] (L_GPIO[12..22]/L_GPIO[24])
    PINMUX_LCD_SEL_PARALLE_MI_9BITS,                ///< Parallel MI 9 bits.
                                                    ///< When PINMUX_PMI_CFG_NORMAL is set, Enable MPU_D[0..8]/MPU_CS/MPU_RS/MPU_WR/MPU_RD on LCD[0..7]/LCD[9..13] (L_GPIO[0..7]/L_GPIO[9..13])
                                                    ///< When PINMUX_PMI_CFG_2ND_PINMUX is set, Enable MPU2_RS/MPU2_CS/MPU2_D[0..8]/MPU2_WR/MPU2_RD on LCD[12..24] (L_GPIO[12..24])
    PINMUX_LCD_SEL_PARALLE_MI_16BITS,               ///< Parallel MI 16 bits. Enable MPU_D[0..15]/MPU_CS/MPU_RS/MPU_WR/MPU_RD on LCD[0..7]/LCD[9..20] (L_GPIO[0..7]/L_GPIO[9..20])
    PINMUX_LCD_SEL_PARALLE_MI_18BITS,               ///< Parallel MI 18 bits. Enable MPU_D[0..17]/MPU_CS/MPU_RS/MPU_WR/MPU_RD on LCD[0..7]/LCD[9..22] (L_GPIO[0..7]/L_GPIO[9..22])
    PINMUX_LCD_SEL_SERIAL_MI_SDIO,                  ///< Serial MI SDIO bi-direction. Enable MPU_CS/MPU_RS/MPU_CLK/MPU_SDIO on LCD[15..18] (L_GPIO[15..18])
    PINMUX_LCD_SEL_SERIAL_MI_SDI_SDO,               ///< Serial MI SDI/SDO seperate. Enable MPU_SDO/MPU_SDI/MPU_CS/MPU_RS/MPU_CLK on LCD[13..17] (L_GPIO[13..17])
    PINMUX_LCD_SEL_MIPI,                            ///< MIPI DSI

    PINMUX_LCD_SEL_TE_ENABLE = 0x01<<23,            ///< TE Enable (For Parallel/Serial MI)
                                                    ///< When normal (PINMUX_PMI_CFG_NORMAL) parallel MI, enable MPU_TE on LCD8 (L_GPIO[8]).
                                                    ///< When 2nd (PINMUX_PMI_CFG_2ND_PINMUX) parallel MI, enable MPU_TE on LCD25 (L_GPIO[25]).
                                                    ///< When serial MI, enable MI_TE on LCD19 (L_GPIO[19]).
    PINMUX_LCD_SEL_DE_ENABLE = 0x01<<24,            ///< DE Enable (For CCIR656 8/16bits, CCIR601 16bits, Serial RGB 6/8bits, Serial YCbCr 8 bits, RGB 16 bits)
                                                    ///< When corresponding item in pinmux_init() is set with PINMUX_DISPMUX_SEL_LCD, enable CCIR_DE on LCD11 (L_GPIO[11]).
                                                    ///< When corresponding item in pinmux_init() is set with PINMUX_DISPMUX_SEL_LCD2, enable CCIR2_DE on LCD23 (L_GPIO[23]).
    PINMUX_LCD_SEL_HVLD_VVLD = 0x01<<25,            ///< HVLD/VVLD Enable (For CCIR-601 8 bits). Enable CCIR_HVLD/CCIR_VVLD on LCD[12..13] (L_GPIO[12..13])
    PINMUX_LCD_SEL_FIELD = 0x01<<26,                ///< FIELD Enable (For CCIR-601 8 bits). Enable CCIR_FIELD on LCD14 (L_GPIO[14])

    PINMUX_LCD_SEL_FEATURE_MSK = 0x0F<<23,

    ENUM_DUMMY4WORD(PINMUX_LCD_SEL)
} PINMUX_LCD_SEL;

/**
    Pinmux selection for Storage

    @note For PINMUX_FUNC_ID_SDIO, PINMUX_FUNC_ID_SPI, PINMUX_FUNC_ID_NAND
*/
typedef enum
{
    PINMUX_STORAGE_SEL_INACTIVE,                    ///< Inactive storage
    PINMUX_STORAGE_SEL_ACTIVE,                      ///< Active storage
    PINMUX_STORAGE_SEL_INEXIST,                     ///< Inexist storage
    PINMUX_STORAGE_SEL_EXIST,                       ///< Exist storage
    ENUM_DUMMY4WORD(PINMUX_STORAGE_SEL)
} PINMUX_STORAGE_SEL;

/**
    Pinmux selection for sensor

    @note For PINMUX_FUNC_ID_SN_MES0, PINMUX_FUNC_ID_SN_MES1, PINMUX_FUNC_ID_SN_FLCTR
*/
typedef enum
{
    PINMUX_SENSOR_SEL_INACTIVE,                     ///< Inactive
    PINMUX_SENSOR_SEL_ACTIVE,                       ///< Active
    ENUM_DUMMY4WORD(PINMUX_SENSOR_SEL)
} PINMUX_SENSOR_SEL;

/**
    Pinmux selection for sensor

    @note For PINMUX_FUNC_ID_USB_VBUSI
*/
typedef enum
{
    PINMUX_USB_SEL_INACTIVE,                        ///< Inactive
    PINMUX_USB_SEL_ACTIVE,                          ///< Active
    ENUM_DUMMY4WORD(PINMUX_USB_SEL)
} PINMUX_USB_SEL;

/**
    Pinmux selection for SPI

    @note For PINMUX_FUNC_ID_SPI2, PINMUX_FUNC_ID_SPI3
*/
typedef enum
{
    PINMUX_SPI_SEL_INACTIVE,                        ///< Inactive
    PINMUX_SPI_SEL_ACTIVE,                          ///< Active
    ENUM_DUMMY4WORD(PINMUX_SPI_SEL)
} PINMUX_SPI_SEL;

/**
    Pinmux selection for SPI2

    @note For PINMUX_FUNC_ID_SPI2
*/
typedef enum
{
    PINMUX_SPI2_SEL_INACTIVE,                       ///< Inactive
    PINMUX_SPI2_SEL_ACTIVE,                         ///< Active (according to top_init() setting)
    PINMUX_SPI2_SEL_ACTIVE_1ST,                     ///< Active to SPI2_1 on SPI_CS/SPI_CLK/SPI_DO/SPI_DI(P_GPIO[21..24])
    PINMUX_SPI2_SEL_ACTIVE_2ND,                     ///< Active to SPI2_2 on UART2_TX/UART2_RX/UART2_RTS/UART2_CTS (P_GPIO[17..20])

    ENUM_DUMMY4WORD(PINMUX_SPI2_SEL)
} PINMUX_SPI2_SEL;

/**
    Pinmux group

    @note For pinmux_init()
*/
typedef struct
{
    PIN_FUNC    pinFunction;            ///< PIN Function group
    UINT32      config;                 ///< Configuration for pinFunction
} PIN_GROUP_CONFIG;

//
// API for Top controller
//
extern void     pinmux_select_debugport(PINMUX_DEBUGPORT uiDebug);
extern void     pinmux_select_ocp_debugport(PINMUX_DEBUGPORT_OCP uiDebug);

extern ER       pinmux_init(PIN_GROUP_CONFIG *pConfig);
extern ER       pinmux_setDispMode(PINMUX_FUNC_ID id, PINMUX_LCDINIT dispMode);
extern PINMUX_LCDINIT pinmux_getDispMode(PINMUX_FUNC_ID id);
extern ER       pinmux_setPinmux(PINMUX_FUNC_ID id, UINT32 pinmux);
extern BOOL     pinmux_chkPinmux(PINMUX_FUNC_ID id, UINT32 pinmux);

//@}    //addtogroup mIHALSysCfg

#endif

