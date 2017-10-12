#include "PinmuxCfg.h"
#include "top.h"

const PIN_GROUP_CONFIG vTopConfig[] =
{
    // Module name              pinmux config

    //
    // Storage configuration
    //
    {PIN_FUNC_SDIO,             PIN_SDIO_CFG_4BITS},
    {PIN_FUNC_SDIO2,
#if (_INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
                                PIN_SDIO_CFG_8BITS|PIN_SDIO_CFG_2ND_PINMUX|
#endif
#if (_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_)
                                PIN_SDIO_CFG_8BITS|PIN_SDIO_CFG_1ST_PINMUX|
#endif
                                PIN_SDIO_CFG_NONE},

#if (_INTERSTORAGE_ == _INTERSTORAGE_NAND_)
    {PIN_FUNC_NAND,             PIN_NAND_CFG_1CS},
#else
    {PIN_FUNC_NAND,             PIN_NAND_CFG_NONE},
#endif

    //
    // Sensor configuration
    //
#if  (_SENSORLIB_ == _SENSORLIB_CMOS_IMX083CQK_)
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_LVDS|PIN_SENSOR_CFG_LVDS_VDHD|PIN_SENSOR_CFG_MCLK},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_CLK0 | PIN_MIPI_LVDS_CFG_DAT0 | PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 |
                                PIN_MIPI_LVDS_CFG_DAT3 | PIN_MIPI_LVDS_CFG_DAT4 | PIN_MIPI_LVDS_CFG_DAT5 | PIN_MIPI_LVDS_CFG_DAT6 |
                                PIN_MIPI_LVDS_CFG_DAT7 | PIN_MIPI_LVDS_CFG_DAT8 | PIN_MIPI_LVDS_CFG_DAT9},
#elif  (_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_)
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_LVDS|PIN_SENSOR_CFG_LVDS_VDHD|PIN_SENSOR_CFG_MCLK | PIN_SENSOR_CFG_MES0 | PIN_SENSOR_CFG_MES1},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_CLK0 | PIN_MIPI_LVDS_CFG_DAT0 | PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 |
                                PIN_MIPI_LVDS_CFG_DAT3 | PIN_MIPI_LVDS_CFG_DAT4 | PIN_MIPI_LVDS_CFG_DAT5 | PIN_MIPI_LVDS_CFG_DAT6 |
                                PIN_MIPI_LVDS_CFG_DAT7 | PIN_MIPI_LVDS_CFG_DAT8 | PIN_MIPI_LVDS_CFG_DAT9},
#elif  (_SENSORLIB_ == _SENSORLIB_CMOS_MI5100_ || _SENSORLIB_ == _SENSORLIB_CMOS_AR0330P_)
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_12BITS|PIN_SENSOR_CFG_MCLK},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_SENSOR2_CFG_NONE},
#elif  (_SENSORLIB_ == _SENSORLIB_CMOS_IMX208PQH_)
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_MIPI|PIN_SENSOR_CFG_MCLK},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_CLK0 | PIN_MIPI_LVDS_CFG_DAT0 | PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 |
                                PIN_MIPI_LVDS_CFG_DAT3 | PIN_MIPI_LVDS_CFG_DAT4 | PIN_MIPI_LVDS_CFG_DAT5 | PIN_MIPI_LVDS_CFG_DAT6 |
                                PIN_MIPI_LVDS_CFG_DAT7 | PIN_MIPI_LVDS_CFG_DAT8 | PIN_MIPI_LVDS_CFG_DAT9},
#elif  ((_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_) && (_SENSORLIB2_ == _SENSORLIB2_CMOS_NT99141_))
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_MIPI|PIN_SENSOR_CFG_MCLK|PIN_SENSOR_CFG_MCLK2},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_CCIR8BITS},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_CLK0 | PIN_MIPI_LVDS_CFG_DAT0 | PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 | PIN_MIPI_LVDS_CFG_DAT3},
#elif  (_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_)
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_MIPI|PIN_SENSOR_CFG_MCLK},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_CLK0 | PIN_MIPI_LVDS_CFG_DAT0 | PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 | PIN_MIPI_LVDS_CFG_DAT3},
#else
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_MIPI},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_CLK0 | PIN_MIPI_LVDS_CFG_DAT0 | PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 | PIN_MIPI_LVDS_CFG_DAT3},
#endif

    //
    // Serial interface configuration
    //
    /*
    NOTE:
    PIN_I2C_CFG_CH1             (independent for MOTOR)
    PIN_I2C_CFG_CH1_2ND_PINMUX  (share to /SIF0 /SPI3)
    PIN_I2C_CFG_CH2             (independent for HDMI)
    PIN_I2C_CFG_CH2_2ND_PINMUX  (share to /Parallel_Sensor_VDHD)
    */
    {PIN_FUNC_I2C,              PIN_I2C_CFG_NONE |
#if (1) //for PowerIC
                                PIN_I2C_CFG_CH1 |
#endif
#if (_SENSORLIB_ == _SENSORLIB_CMOS_MI5100_|| _SENSORLIB_ == _SENSORLIB_CMOS_AR0330P_|| _SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_||_SENSORLIB_ == _SENSORLIB_CMOS_IMX208PQH_)
                                PIN_I2C_CFG_CH1_2ND_PINMUX |
#endif
#if (_SENSORLIB2_ == _SENSORLIB2_CMOS_NT99141_)
                                PIN_I2C_CFG_CH2_2ND_PINMUX |
#endif

#if (_HDMITYPE_ == _HDMI_ON_)
                                PIN_I2C_CFG_CH2 |
#endif
                                0},

    {PIN_FUNC_SIF,              PIN_SIF_CFG_NONE |
#if ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX083CQK_) || (_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_))
                                PIN_SIF_CFG_CH0 |
#endif
#if (_LENSMOTORLIB_ == _LENSMOTORLIB_G2007_)
                                PIN_SIF_CFG_CH2 |
#endif
                                0},

    {PIN_FUNC_UART,             PIN_UART_CFG_CH1},
    {PIN_FUNC_SPI,              PIN_SPI_CFG_NONE |
#if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
                                PIN_SPI_CFG_CH1 |
#endif
#if (_NETWORK_ == _NETWORK_SPI_EVB_ETHERNET_)
                                PIN_SPI_CFG_CH1_2ND_PINMUX |
#endif
                                0},
    {PIN_FUNC_REMOTE,           PIN_REMOTE_CFG_NONE},

    {PIN_FUNC_PWM,              PIN_PWM_CFG_NONE |
#if (_LENSDRVLIB_ == _LENSDRVLIB_DCJ36_)
                                PIN_PWM_CFG_PWM4 | PIN_PWM_CFG_PWM5 | PIN_PWM_CFG_PWM6 | PIN_PWM_CFG_PWM7 |
#endif
                                0},

    //
    // AUDIO configuration
    //
#if( _AUDIOCODEC_EXT_ == _AUDIOCODEC_EXT_WM8978_)//for WM8978
    {PIN_FUNC_AUDIO,            PIN_AUDIO_CFG_I2S|PIN_AUDIO_CFG_MCLK},
#else
    {PIN_FUNC_AUDIO,            PIN_AUDIO_CFG_NONE},
#endif
    //
    // Display configuration
    //
    {PIN_FUNC_LCD,              PINMUX_DISPMUX_SEL_NONE |
#if (_LCDTYPE_ == _LCDTYPE_AUCN01_)
                                PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_YUV640 |
#elif (_LCDTYPE_ == _LCDTYPE_A030VN01_)
                                PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_CCIR601 |
#elif (_LCDTYPE_ == _LCDTYPE_ILI9163C_DSI_)
                                PINMUX_DISPMUX_SEL_LCD | PINMUX_DSI_2_LANE_CMD_MODE_RGB565 |
#elif (_LCDTYPE_ == _LCDTYPE_TXDT270C_8961_6A6_)
                                PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_RGBD320 |
#elif (_LCDTYPE_ == _LCDTYPE_TXDT270C_8961_6A6_Flip_)
                                PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_RGBD320 |
#elif (_LCDTYPE_ == _LCDTYPE_RZWT15P00_)
                                PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_RGBD320 |
#elif (_LCDTYPE_ == _LCDTYPE_ILI9341_)
                                PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_MI_FMT3 |
#elif (_LCDTYPE_ == _LCDTYPE_PW35P00_HX8238D_)
                                PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_RGB_SERIAL |
#elif (_LCDTYPE_ == _LCDTYPE_PW27P05_ILI8961_)
                                PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_RGB_SERIAL |
#endif
                                0},
    {PIN_FUNC_LCD2,             PINMUX_DISPMUX_SEL_NONE |
#if (_LCD2TYPE_ == _LCDTYPE_AUCN01_)
                                PINMUX_DISPMUX_SEL_LCD2 | PINMUX_LCDMODE_YUV640 |
#endif
                                0},
    {PIN_FUNC_TV,               PINMUX_TV_HDMI_CFG_NORMAL},
    {PIN_FUNC_HDMI,             PINMUX_TV_HDMI_CFG_NORMAL |
#if (_HDMITYPE_ == _HDMI_ON_)
                                PINMUX_HDMIMODE_720X480P60 |
#endif
                                0},
    //
    // USB configuration
    //
    {PIN_FUNC_USB,              PINMUX_USB_CFG_DEVICE}
};

void IO_InitPinmux(void)
{
    pinmux_init((PIN_GROUP_CONFIG *)vTopConfig);
}

