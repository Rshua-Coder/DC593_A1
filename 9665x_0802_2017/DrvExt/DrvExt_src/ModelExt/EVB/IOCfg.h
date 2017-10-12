/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       IOCfg.h
    @ingroup    mIPRJAPCommonIO

    @brief      Header file of IO config
                This file is the header file of IO config

    @note       Nothing.
    @date       2012/09/04
*/

/** \addtogroup mIPRJAPCommonIO */
//@{

#ifndef _IOCFG_H
#define _IOCFG_H

#include "Type.h"
#include "IOInit.h"
#include "pad.h"
#include "top.h"
#include "gpio.h"
#include "adc.h"
#include "pwm.h"

#define USE_VIO ENABLE

#if (USE_VIO == ENABLE)
#define VIO_TV_DET                  1
#define VIO_HDMI_DET                2
#define VIO_MAX_ID                  16
extern UINT32 vio_getpin(UINT32 id);
extern void vio_setpin(UINT32 id, UINT32 v);
#endif

//--------------------------------------------------------------------
// GPIO common
//--------------------------------------------------------------------
typedef struct _GPIO_INIT_OBJ{
    UINT32  GpioPin;
    UINT32  GpioDir;
    UINT32  PadDir;
    UINT32  PadPin;
} GPIO_INIT_OBJ,*PGPIO_INIT_OBJ;


//--------------------------------------------------------------------
// ADC common
//--------------------------------------------------------------------

#define VOLDET_ADC_CONT_MODE        ENABLE //!< ENABLE for continuous, DISABLE for one-shot

#if (VOLDET_ADC_CONT_MODE == ENABLE)
#define VOLDET_ADC_MODE             TRUE
#else
#define VOLDET_ADC_MODE             FALSE
#endif

//--------------------------------------------------------------------
// PWM common
//--------------------------------------------------------------------



//--------------------------------------------------------------------
// Display device
//--------------------------------------------------------------------
// LCD communicate
#define LCD_COMM_DUMMY              0
#define LCD_COMM_BY_GPIO            1
#define LCD_COMM_BY_SIF             2
#define LCD_COMM_CTRL               LCD_COMM_BY_GPIO //EVB
#if (LCD_COMM_CTRL == LCD_COMM_BY_GPIO)
#define GPIO_LCD_SIF_SEN            L_GPIO_20 //EVB
#define GPIO_LCD_SIF_SCK            L_GPIO_21 //EVB
#define GPIO_LCD_SIF_SDA            L_GPIO_22 //EVB
#endif
#if (LCD_COMM_CTRL == LCD_COMM_BY_SIF)
#define SIF_LCD_CH                  SIF_CHx //DEMO
#define PAD_LCD_SIF_SEN             PAD_PIN_xGPIOx
#define PAD_LCD_SIF_SCK             PAD_PIN_xGPIOx
#define PAD_LCD_SIF_SDA             PAD_PIN_xGPIOx
#endif
//#define GPIO_LCD_SLEEP              87 //EVB not support
//#define GPIO_LCD_RESET              88 //EVB not support

// LCD backlight
#define LCD_BACKLIGHT_DUMMY         0
#define LCD_BACKLIGHT_BY_GPIO       1
#define LCD_BACKLIGHT_BY_PWM        2
#define LCD_BACKLIGHT_BY_PWRIC      3
#define LCD_BACKLIGHT_CTRL          LCD_BACKLIGHT_DUMMY //EVB
#if (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_PWM)
#define GPIO_PWM_BLG_PCTL           x
#define PWMID_BLG_PCTL              PWMID_x
#elif (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_GPIO)
#define GPIO_LCD_BLG_PCTL           x_GPIO_x
#define PAD_LCD_BLG_PCTL            PAD_PIN_xGPIOx
#endif

// TV
#define TV_DET_DUMMY                0
#define TV_DET_BY_VIO               1
#define TV_DET_BY_GPIO              2
#define TV_DET_BY_ADC               3
#define TV_DET_CTRL                 TV_DET_BY_VIO //EVB
#if (TV_DET_CTRL == TV_DET_BY_GPIO)
#define GPIO_TV_PLUG                C_GPIO_x
#define PAD_TV_PLUG                 PAD_PIN_CGPIOx
#endif
#if (TV_DET_CTRL == TV_DET_BY_ADC)
#define ADC_CH_TV_PLUG              ADC_CHANNEL_x
#endif

extern BOOL     GPIOMap_DetHDMI(void);
extern BOOL     GPIOMap_DetTV(void);
extern void     GPIOMap_SwitchTV(BOOL bIsSwitch);

//--------------------------------------------------------------------
// Storage device
//--------------------------------------------------------------------
// CARD
// card power
#define GPIO_CARD_POWER             x_GPIO_x //EVB
#define PAD_CARD_POWER              PAD_PIN_xGPIOx
// SD card detect
#define GPIO_CARD_DETECT            D_GPIO_2 //DEMO
#define PAD_CARD_DETECT             PAD_PIN_DGPIO2
// SD card write protect
#define GPIO_CARD_WP                x_GPIO_x //DEMO
#define PAD_CARD_WP                 PAD_PIN_xGPIOx


extern BOOL     SDIOCardUser_CheckCardInserted(void);
extern BOOL     SDIOCardUser_CheckCardWP(void);
#if (_INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
extern BOOL     SDIO2CardUser_CheckCardInserted(void);
extern BOOL     SDIO2CardUser_CheckCardWP(void);
#endif

//--------------------------------------------------------------------
// Audio device
//--------------------------------------------------------------------
//Audio
extern BOOL     GPIOMap_DetAudio(void);


//--------------------------------------------------------------------
// Input device
//--------------------------------------------------------------------
// Key defined (650 EVB)
//
// <Layout>
//--------------------------------//   //------------------------------//
//  PSW1  PSW2  PSW3  PSW4   FUN1 //   //  koff  kmp   ----  ----  ks1 //
//                                //   //                              //
//                           FUN2 //   //                          ks2 //
//           UP                   //   //           ku                 //
//                           FUN3 //   //                          kzi //
//                                //   //                              //
//     LEFT  OK   RIGHT      FUN4 //   //    kl     ke     kr      kzo //
//                                //   //                              //
//                           FUN5 //   //                          km  //
//          DOWN                  //   //           kd                 //
//                           FUN6 //   //                          kmd //
//--------------------------------//   //------------------------------//
//
// <Mapping> <Active> (Roy)
/*
PWR KEY1 = RTC_PWRON_SRC_PWR_SW
PWR KEY2 = RTC_PWRON_SRC_PWR_SW2
PWR KEY3 = RTC_PWRON_SRC_PWR_SW3
PWR KEY4 = RTC_PWRON_SRC_PWR_SW4
KEY ARROW = AD_IN2
KEY FUN1 = P_GPIO[17], def=pull-up, active high
KEY FUN2 = P_GPIO[18], def=pull-up, active high
KEY FUN3 = P_GPIO[19], def=pull-up, active high
KEY FUN4 = P_GPIO[20], def=pull-up, active high
KEY FUN5 = S_GPIO[28], def=pull-down, active high
KEY FUN6 = S_GPIO[29], def=pull-down, active high
*/

#define GPIO_KEY_SHUTTER1           P_GPIO_17           // Key Shutter1 (ks1)
#define PAD_KEY_SHUTTER1            PAD_PIN_PGPIO17
#define GPIO_KEY_SHUTTER2           P_GPIO_18           // Key Shutter2 (ks2)
#define PAD_KEY_SHUTTER2            PAD_PIN_PGPIO18
#define GPIO_KEY_ZOOMIN             P_GPIO_19           // Key Zoom In (kzi)
#define PAD_KEY_ZOOMIN              PAD_PIN_PGPIO19
#define GPIO_KEY_ZOOMOUT            P_GPIO_20           // Key Zoom Out (kzo)
#define PAD_KEY_ZOOMOUT             PAD_PIN_PGPIO20
#define GPIO_KEY_MENU               S_GPIO_28            // Key Menu (km)
#define PAD_KEY_MENU                PAD_PIN_SGPIO28
#define GPIO_KEY_MODE               S_GPIO_29            // Key Mode (kmd)
#define PAD_KEY_MODE                PAD_PIN_SGPIO29
/*
#define GPIO_KEY_LEFT               C_GPIO_x           // Key Left (kl)
#define PAD_KEY_LEFT                PAD_PIN_CGPIOx
#define GPIO_KEY_ENTER              C_GPIO_x           // Key OK (ke)
#define PAD_KEY_ENTER               PAD_PIN_CGPIOx
#define GPIO_KEY_UP                 C_GPIO_x           // Key Up (ku)
#define PAD_KEY_UP                  PAD_PIN_CGPIOx
#define GPIO_KEY_RIGHT              C_GPIO_x           // Key Right (kr)
#define PAD_KEY_RIGHT               PAD_PIN_CGPIOx
#define GPIO_KEY_DOWN               C_GPIO_x           // Key Down (kd)
#define PAD_KEY_DOWN                PAD_PIN_CGPIOx
*/
/*
#define GPIO_KEY_PLAYBACK           C_GPIO_x           // Key Playback (kmp)
#define PAD_KEY_PLAYBACK            PAD_PIN_CGPIOx
*/
#define ADC_CH_KEYDET_KEY1          ADC_CHANNEL_2 //EVB


//Touch panel controller

//#define GPIO_TP_BUSY                (2 | GPIO_IS_DGPIO)  //HW TS_INT =DGPIO-02
#define GPIO_TP_PENIRQ              (7 | GPIO_IS_DGPIO) //HW TS_PENIRQ =DGPIO-12
#define GPIO_TP_DOUT                P_GPIO_58 //HW TP_DI  =SBDAT0/AGPIO-26   (FW output = HW input)
#define GPIO_TP_DIN                 (1 | GPIO_IS_DGPIO) //HW TS_DO  =DGPIO-01  (FW input = HW output)
#define GPIO_TP_DCLK                P_GPIO_57 //HW TS_CLK =SBCK0/AGPIO-25
//#define GPIO_TP_CS                  28 //HW TS_CS  =SBCS1/AGPIO-28  //SIF channel 1
#define GPIO_TP_CS                  P_GPIO_56 //HW TS_CS  =SBCS0/AGPIO-27  //SIF channel 0
//#define PAD_TP_BUSY                 PAD_PIN_DGPIO2
#define PAD_TP_PENIRQ               PAD_PIN_DGPIO7
#define PAD_TP_DOUT                 PAD_PIN_PGPIO58
#define PAD_TP_DIN                  PAD_PIN_DGPIO1
#define PAD_TP_DCLK                 PAD_PIN_PGPIO57
//#define PAD_TP_CS                   PAD_PIN_SBCS1  //SIF channel 1
#define PAD_TP_CS                   PAD_PIN_PGPIO56  //SIF channel 0


//G-Sensor
/*
#define GPIO_GS_INT1                (3 | GPIO_IS_DGPIO)
#define GPIO_GS_INT2                (4 | GPIO_IS_DGPIO)
#define PAD_GS_INT1                 PAD_PIN_DGPIO3
#define PAD_GS_INT2                 PAD_PIN_DGPIO4
*/
extern UINT32   GPIOMap_DetKey(void);
extern UINT32   GPIOMap_DetModeDial(void);
extern BOOL     GPIOMap_DetShutter1(void);
extern BOOL     GPIOMap_DetMacro(void);
extern BOOL     GPIOMap_DetPoweroff(void);

//--------------------------------------------------------------------
// Output device
//--------------------------------------------------------------------
// LED
#define LED_RED_DUMMY               0
#define LED_RED_BY_GPIO             1
#define LED_RED_CTRL                LED_RED_DUMMY
#define GPIO_RED_LED                x_GPIO_x  //EVB
#define PAD_RED_LED                 PAD_PIN_xGPIOx

#define LED_GREEN_DUMMY             0
#define LED_GREEN_BY_GPIO           1
#define LED_GREEN_CTRL              LED_RED_DUMMY
#define GPIO_GREEN_LED              x_GPIO_x  //EVB
#define PAD_GREEN_LED               PAD_PIN_xGPIOx

#define LED_FOCUS_DUMMY             0
#define LED_FOCUS_BY_GPIO           1
#define LED_FOCUS_BY_PWM            2 //use PWM to control LED brightness
#define LED_FOCUS_CTRL              LED_RED_DUMMY
#define GPIO_FOCUS_LED              x_GPIO_x  //EVB
#define PAD_FOCUS_LED               PAD_PIN_xGPIOx
#define PWMID_FOCUS_LED             PWMID_x //EVB


//--------------------------------------------------------------------
// Power device
//--------------------------------------------------------------------
//Battery
#define ADC_CH_VOLDET_BATTERY       ADC_CHANNEL_0 //EVB
extern char*    VolDet_GetStatusString(void);



//--------------------------------------------------------------------
// Sensor device
//--------------------------------------------------------------------
// CCD
/*
typedef enum
{
    CCD_PWR_STATUS_OFF = 0,
    CCD_PWR_STATUS_ON,
    CCD_PWR_STATUS_SLEEP,
    CCD_PWR_STATUS_MAXIDX
}CCD_PWR_STATUS;
*/
// Sensor
#if (_SENSORLIB_ == _SENSORLIB_CMOS_IMX208PQH_)
#define GPIO_SENSOR_RESET           P_GPIO_56
#define GPIO_SENSOR_PW1             D_GPIO_4
#define GPIO_SENSOR_PW2             D_GPIO_5
#define GPIO_SENSOR_PW3             D_GPIO_6
#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_) && (_SENSORLIB2_ == _SENSORLIB2_CMOS_NT99141_))
#define GPIO_SENSOR_STANDBY         D_GPIO_6
#define GPIO_SENSOR_RESET           D_GPIO_7
#elif (_SENSORLIB_ == _SENSORLIB_CMOS_IMX322LQJ_)
#define GPIO_SENSOR_STANDBY         D_GPIO_6
#define GPIO_SENSOR_RESET           D_GPIO_4
#else
#define GPIO_SENSOR_STANDBY         D_GPIO_5
#define GPIO_SENSOR_RESET           D_GPIO_4
#endif



/*
#define GPIO_CCDTG_VH               L_GPIO_20
#define GPIO_CCDTG_VL               L_GPIO_19
//#define GPIO_CCDTG_VH_2             50
#define GPIO_CCDTG_RST              C_GPIO_22
//#define GPIO_CCDTG_HDRV_EN          88
#define SIF_SENSOR_TG               SIF_CH2
#define SIF_SENSOR_AFE              SIF_CH3
*/
extern void     GPIOMap_TurnOnCCDPower(void);
extern void     GPIOMap_TurnOffCCDPower(void);
extern UINT8    GPIOMap_IsCCDPowerOn(void);
extern void     GPIOMap_SleepCCDPower(void);
extern void     GPIOMap_WakeUpCCDPower(void);

//--------------------------------------------------------------------
// Storbe device
//--------------------------------------------------------------------
// STROBE
/*
#define GPIO_FLASH_CHARGE           C_GPIO_21
#define PAD_FL_CHARGE               PAD_PIN_CGPIO21
#define GPIO_FLASH_RDY              P_GPIO_12
#define PAD_FL_RDY                  PAD_PIN_PGPIO12
#define GPIO_FLASH_TRIGGER          D_GPIO_7
#define PAD_FL_TRIGGER              PAD_PIN_DGPIO7
*/

//--------------------------------------------------------------------
// Lens device
//--------------------------------------------------------------------
//#NT#2012/07/20#Isiah Chang -begin
//#NT#Rearrange GPIO map of lens.
//FPGA Lens temporarily
/* Input pins of NT96650. */

#define GPIO_MD_POWER_EN            P_GPIO_51
#define GPIO_LENS_ZOOM_PI           D_GPIO_1
#define GPIO_LENS_ZOOM_PI_ITR       GPIO_INT_33

#define GPIO_LENS_ZOOM_PR           P_GPIO_49
#define GPIO_LENS_FOCUS_PI          P_GPIO_50

#define PAD_LENS_ZOOM_PI            PAD_PIN_DGPIO1
#define PAD_LENS_ZOOM_PR            PAD_PIN_PGPIO49
#define PAD_LENS_FOCUS_PI           PAD_PIN_PGPIO50

/* Output pins of NT96650.  */

#define GPIO_LENS_EN                S_GPIO_18
#define GPIO_LENS_MD1               S_GPIO_19
#define GPIO_LENS_MD2               L_GPIO_16
#define GPIO_LENS_MD3               L_GPIO_17
#define GPIO_LENS_IN1A              P_GPIO_40 //AF_A+
#define GPIO_LENS_IN1B              P_GPIO_41 //AF_A-
#define GPIO_LENS_IN2A              P_GPIO_42 //AF_B+
#define GPIO_LENS_IN2B              P_GPIO_43 //AF_B-
#define GPIO_LENS_IN3A              P_GPIO_44 //IRIS_A+
#define GPIO_LENS_IN3B              P_GPIO_45 //IRIS_A-
#define GPIO_LENS_IN4A              P_GPIO_46 //IRIS_B+
#define GPIO_LENS_IN4B              P_GPIO_47 //IRIS_B-
#define GPIO_LENS_IN5A              P_GPIO_38 //ZOOM_+
#define GPIO_LENS_IN5B              P_GPIO_39 //ZOOM_-
#define GPIO_LENS_IN6A              P_GPIO_36 //Shutter_+
#define GPIO_LENS_IN6B              P_GPIO_37 //Shutter_-

//#NT#2012/07/20#Isiah Chang -end

#endif

//@}

