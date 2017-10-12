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

#define USE_VIO DISABLE

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
#define LCD_COMM_CTRL               LCD_COMM_DUMMY //EVB
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
#define GPIO_LCD_RESET              L_GPIO_21

// LCD backlight
#define LCD_BACKLIGHT_DUMMY         0
#define LCD_BACKLIGHT_BY_GPIO       1
#define LCD_BACKLIGHT_BY_PWM        2
#define LCD_BACKLIGHT_BY_PWRIC      3
#define LCD_BACKLIGHT_CTRL          LCD_BACKLIGHT_BY_GPIO
#if (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_PWM)
#define GPIO_PWM_BLG_PCTL           x
#define PWMID_BLG_PCTL              PWMID_x
#elif (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_GPIO)
#define GPIO_LCD_BLG_PCTL           P_GPIO_51    //PWM15
#define PAD_LCD_BLG_PCTL            PAD_PIN_PGPIO51
#endif

// TV
#define TV_DET_DUMMY                0
#define TV_DET_BY_VIO               1
#define TV_DET_BY_GPIO              2
#define TV_DET_BY_ADC               3
#define TV_DET_CTRL                 TV_DET_BY_GPIO//TV_DET_DUMMY//TV_DET_BY_GPIO
#if (TV_DET_CTRL == TV_DET_BY_GPIO)
#define GPIO_TV_PLUG                P_GPIO_43
#define PAD_TV_PLUG                 PAD_PIN_PGPIO43
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
#define GPIO_CARD_POWER             C_GPIO_12
#define PAD_CARD_POWER              PAD_PIN_CGPIO12
// SD card detect
#define GPIO_CARD_DETECT            (2 | GPIO_IS_DGPIO)
#define PAD_CARD_DETECT             PAD_PIN_DGPIO2
// SD card write protect
#define GPIO_CARD_WP                (3 | GPIO_IS_DGPIO)
#define PAD_CARD_WP                 PAD_PIN_DGPIO3

// Sensor
#define GPIO_SENSOR_RESET           D_GPIO_7
#if 0
#define GPIO_SENSOR_PWM0            P_GPIO_36
#define GPIO_SENSOR_PWM1            P_GPIO_37
#elif (_SENSORLIB_ == _SENSORLIB_CMOS_IMX322LQJ_)
#define GPIO_SENSOR_PWM0            P_GPIO_39  //1.2V
#define GPIO_SENSOR_PWM1            P_GPIO_37 //1.8V
#define GPIO_SENSOR_PWM2            P_GPIO_38 //2.7V
#else
#define GPIO_SENSOR_PWM0            P_GPIO_37
#define GPIO_SENSOR_PWM1            P_GPIO_38
#endif

//WIFI
#define GPIO_WIFI_POWER                   P_GPIO_50//PWM14
#define GPIO_WIFI_SHDN                      P_GPIO_43//PWM7

//LCD
#define GPIO_DSI18_EN                      P_GPIO_54//PWM18
#define GPIO_DSI30_EN                      P_GPIO_55//PWM19

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
// Key defined (650 FPGA KEYPAD)
//
// <Layout>
//--------------------------//   //--------------------------//
//  SW3   SW1   SW4   SW2   //   //  kzi   kzo   ks1   ks2   //
//                          //   //                          //
//           SW7            //   //           ku             //
//                          //   //                          //
//      SW5  SW6  SW8       //   //      kl   ke   kr        //
//                          //   //                          //
//           SW10           //   //           kd             //
//                          //   //                          //
//    SW12   SW11   SW9     //   //    km     kmd    kmp     //
//                          //   //                          //
//--------------------------//   //--------------------------//
//
// <Mapping>  SW1~SW12 = MC22~MC31,DCPIO4,5 = C_GPIO_22~31,DCPIO4,5 (Roy)
// <Active>   Low (Roy)
//
#define GPIO_KEY_SHUTTER2           P_GPIO_42           // Key Shutter2 (ks2) //PWM6
#define PAD_KEY_SHUTTER2            PAD_PIN_PGPIO42

#define GPIO_KEY_ENTER           P_GPIO_49           // Key Shutter2 (ks2) //PWM13
#define PAD_KEY_ENTER            PAD_PIN_PGPIO49

#define GPIO_KEY_MODE_SW            P_GPIO_47           // PWM12
#define PAD_KEY_MODE_SW             PAD_PIN_PGPIO47

#define GPIO_KEY_WIFI_SW           P_GPIO_48           // PWM11
#define PAD_KEY_WIFI_SW            PAD_PIN_PGPIO48


#define GPIO_WIFI_EN                P_GPIO_50
#define PAD_WIFI_EN                 PAD_PIN_PGPIO50

#define GPIO_AUDIO_EN               C_GPIO_13
#define PAD_AUDIO_EN                PAD_PIN_CGPIO13


/*
#define ADC_CH_VOLDET_MS1           ADC_CHANNEL_1
#define ADC_CH_VOLDET_MS2           ADC_CHANNEL_0
*/

#define ADC_CH_VOLDET_KEY1          ADC_CHANNEL_1
#define ADC_CH_VOLDET_KEY2          ADC_CHANNEL_2

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
extern BOOL     GPIOMap_DetMovieSize(void);

//--------------------------------------------------------------------
// Output device
//--------------------------------------------------------------------
// LED
#define LED_RED_DUMMY               0
#define LED_RED_BY_GPIO             1
#define LED_RED_CTRL                LED_RED_BY_GPIO
#define GPIO_RED_LED                P_GPIO_44//P_GPIO_40//46
#define PAD_RED_LED                 PAD_PIN_PGPIO44//PAD_PIN_PGPIO40//46  //PWR LED

#define LED_GREEN_DUMMY             0
#define LED_GREEN_BY_GPIO           1
#define LED_GREEN_CTRL              LED_GREEN_BY_GPIO
#define GPIO_GREEN_LED              P_GPIO_45//P_GPIO_44
#define PAD_GREEN_LED               PAD_PIN_PGPIO45//PAD_PIN_PGPIO44 // LED STATUS

#define LED_WIFI_DUMMY              0
#define LED_WIFI_BY_GPIO            1
#define LED_WIFI_CTRL               LED_WIFI_BY_GPIO
#define GPIO_WIFI_LED               P_GPIO_46//P_GPIO_40//P_GPIO_45
#define PAD_WIFI_LED                PAD_PIN_PGPIO46//PAD_PIN_PGPIO40//PAD_PIN_PGPIO45 //WIFI LED


#define LED_FOCUS_DUMMY             0
#define LED_FOCUS_BY_GPIO           1
#define LED_FOCUS_BY_PWM            2 //use PWM to control LED brightness
#define LED_FOCUS_CTRL              LED_FOCUS_DUMMY
#define GPIO_FOCUS_LED              P_GPIO_46
#define PAD_FOCUS_LED               PAD_PIN_PGPIO46  //LED STATUS
#define PWMID_FOCUS_LED             PWMID_10



//--------------------------------------------------------------------
// Power device
//--------------------------------------------------------------------
//Battery
#define ADC_CH_VOLDET_BATTERY       ADC_CHANNEL_0
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
extern void     GPIOMap_CtlMotorStatus(BOOL En);
extern void     GPIOMap_CtlWIFISHDN(BOOL En);

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

#define GPIO_LENS_POWER_EN          55
#define GPIO_LENS_ZOOM_PI           P_GPIO_55
#define GPIO_LENS_ZOOM_PI_ITR       GPIO_INT_15

#define GPIO_LENS_ZOOM_PR           P_GPIO_55
#define GPIO_LENS_FOCUS_PI          P_GPIO_55

#define PAD_LENS_ZOOM_PI            PAD_PIN_DGPIO1
#define PAD_LENS_ZOOM_PR            PAD_PIN_PGPIO49
#define PAD_LENS_FOCUS_PI           PAD_PIN_PGPIO50

/* Output pins of NT96650.  */

#define GPIO_LENS_EN                P_GPIO_55
#define GPIO_LENS_MD1               P_GPIO_55
#define GPIO_LENS_MD2               P_GPIO_55
#define GPIO_LENS_MD3               P_GPIO_55
#define GPIO_LENS_IN1A              P_GPIO_55 //AF_A+
#define GPIO_LENS_IN1B              P_GPIO_55 //AF_A-
#define GPIO_LENS_IN2A              P_GPIO_55 //AF_B+
#define GPIO_LENS_IN2B              P_GPIO_55 //AF_B-
#define GPIO_LENS_IN3A              P_GPIO_55 //IRIS_A+
#define GPIO_LENS_IN3B              P_GPIO_55 //IRIS_A-
#define GPIO_LENS_IN4A              P_GPIO_55 //IRIS_B+
#define GPIO_LENS_IN4B              P_GPIO_55 //IRIS_B-
#define GPIO_LENS_IN5A              P_GPIO_55 //ZOOM_+
#define GPIO_LENS_IN5B              P_GPIO_55 //ZOOM_-
#define GPIO_LENS_IN6A              P_GPIO_55 //Shutter_+
#define GPIO_LENS_IN6B              P_GPIO_55 //Shutter_-

//#NT#2012/07/20#Isiah Chang -end

#endif

//@}

