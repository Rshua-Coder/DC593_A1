/*
    Display object object panel configuration parameters for RZWT27P02

    @file       RZWT27P02.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "display.h"
#include "dispdev_IF8bits.h"
#include "dispdev_panlcomn.h"


/*
    Panel Parameters for RZWT27P02
*/
//@{


/*Used in RGB-Dummy 320*/
const tPANEL_CMD tCmdModeRgbd320[] =
{
    #if 1
    {0x00, 0x0f},        
    {0x00, 0x05},
    {CMDDELAY_MS, 50},
    {0x00, 0x0f},
    {0x00, 0x05},
    {CMDDELAY_MS, 50},
    {0x00, 0x0f},
    {0xf5, 0x46},
    {0x90, 0x60},
    {0x50, 0x00},
    {CMDDELAY_MS, 50},
    {0x30, 0x08},
    {0x70, 0x40},
    {0xc0, 0x05},
    {0xE0, 0x13},
    {0x60, 0x01}
    #else
{ 0x02, 0x00},
{ 0x03, 0x01},
{ 0x04, 0x0B},
{ 0x05, 0x3B},
{ 0x06, 0x18},
{ 0x07, 0x08},
{ 0x08, 0x00},
{ 0x09, 0x20},
{ 0x0A, 0x20},
{ 0x0B, 0x20},
{ 0x0C, 0x10},
{ 0x10, 0x2E},
{ 0x11, 0x3F},
{ 0x14, 0x33},
{ 0x15, 0x11},
{ 0x16, 0x01},
{ 0x17, 0x00},
{ 0x18, 0x01}
    #endif
};

/*Used in YUV640 & CCIR601*/
const tPANEL_CMD tCmdModeYUV640[] =
{
    {0x04, 0x08},

    {0x00, 0x09},
    {0x01, 0x90},
    {0x03, 0x40},
    {0x05, 0x5F},
    {0x06, 0x15},
    {0x07, 0x46},
    {0x08, 0x00},
    {0x0B, 0x83},
    {0x0C, 0x06},
    {0x0D, 0x40},
    {0x0E, 0x40},
    {0x0F, 0x40},
    {0x10, 0x40},
    {0x11, 0x40},
    {0x12, 0x00},
    {0x13, 0x01},
    {0x16, 0x04},
    {0x17, 0x54},
    {0x18, 0x54},
    {0x19, 0x43},
    {0x1A, 0x54},
    {0x95, 0x00},
    {0x96, 0x03},
    {0x97, 0x02}
};

/*Used in CCIR656*/
const tPANEL_CMD tCmdModeCCIR656[] =
{
    {0x00, 0x09},
    {0x01, 0x90},
    {0x03, 0x40},
    {0x05, 0x5F},
    {0x06, 0x15},
    {0x07, 0x46},
    {0x08, 0x00},
    {0x0B, 0x83},
    {0x0C, 0x06},
    {0x0D, 0x40},
    {0x0E, 0x40},
    {0x0F, 0x40},
    {0x10, 0x40},
    {0x11, 0x40},
    {0x12, 0x00},
    {0x13, 0x01},
    {0x16, 0x04},
    {0x17, 0x54},
    {0x18, 0x54},
    {0x19, 0x43},
    {0x1A, 0x54},
    {0x95, 0x00},
    {0x96, 0x03},
    {0x97, 0x02}
};

const tPANEL_CMD tCmdStandby[] =
{
    #if 1
    {0x00, 0x00}
    #else
    
    {0x05, 0x5E}
    #endif
};

const tLCD_PARAM tMode[] =
{
    /***********       RGB Dummy-320 MODE         *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_RGBD320,         //!< LCDMode
            24.54f,                          //!< fDCLK
            1560,                           //!< uiHSyncTotalPeriod
            1280,                           //!< uiHSyncActivePeriod
            253,                            //!< uiHSyncBackPorch
            262,                            //!< uiVSyncTotalPeriod
            240,                            //!< uiVSyncActivePeriod
            13,                             //!< uiVSyncBackPorchOdd
            13,                             //!< uiVSyncBackPorchEven
            320,                            //!< uiBufferWidth
            240,                            //!< uiBufferHeight
            320,                            //!< uiWindowWidth
            240,                            //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat

            /* New added parameters */
            0x01,                           //!< uiHSyncSYNCwidth
            0x01                            //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_SERIAL_RGB_8BITS,//!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {FALSE,FALSE},                  //!< dithering[2];
            DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_R,                      //!< odd;
            IDE_LCD_G,                      //!< even;
            TRUE,                           //!< hsinv;
            TRUE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            TRUE,                           //!< clkinv;
            FALSE,                          //!< fieldinv;
            FALSE,                          //!< **DONT-CARE**
            FALSE,                          //!< interlace;
            FALSE,                          //!< **DONT-CARE**
            0x40,                           //!< ctrst;
            0x00,                           //!< brt;
            0x40,                           //!< cmults;
            FALSE,                          //!< cex;
            FALSE,                          //!< **DONT-CARE**
            TRUE,                           //!< **DONT-CARE**
            TRUE,                           //!< tv_powerdown;
            {0x00,0x00},                    //!< **DONT-CARE**

            /* New added parameters */
            FALSE,                          //!< YCex
            FALSE,                          //!< HLPF
            {FALSE,FALSE,FALSE},               //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS}//!< DitherBits[3]
        },

        (tPANEL_CMD*)tCmdModeRgbd320,                  //!< pCmdQueue
        sizeof(tCmdModeRgbd320)/sizeof(tPANEL_CMD),    //!< nCmd
    },

    /***********       CCIR656 MODE         *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_CCIR656,         //!< LCDMode
            27.0f,                         //!< fDCLK
            1716,                           //!< uiHSyncTotalPeriod
            1440,                           //!< uiHSyncActivePeriod
            272,                            //!< uiHSyncBackPorch
            525,                            //!< uiVSyncTotalPeriod
            480,                            //!< uiVSyncActivePeriod
            36,                             //!< uiVSyncBackPorchOdd
            37,                             //!< uiVSyncBackPorchEven
            720,                            //!< uiBufferWidth
            480,                            //!< uiBufferHeight
            720,                            //!< uiWindowWidth
            480,                            //!< uiWindowHeight
            TRUE,                          //!< bYCbCrFormat

            /* New added parameters */
            0x00,                           //!< uiHSyncSYNCwidth
            0x05                            //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_CCIR656|PINMUX_LCD_SEL_DE_ENABLE,//!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {FALSE,FALSE},                  //!< dithering[2];
            DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_R,                      //!< odd;
            IDE_LCD_G,                      //!< even;
            FALSE,                           //!< hsinv;
            FALSE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            TRUE,                           //!< clkinv;
            FALSE,                          //!< fieldinv;
            FALSE,                          //!< **DONT-CARE**
            TRUE,                           //!< interlace;
            FALSE,                          //!< **DONT-CARE**
            0x40,                           //!< ctrst;
            0x00,                           //!< brt;
            0x40,                           //!< cmults;
            FALSE,                          //!< cex;
            FALSE,                          //!< **DONT-CARE**
            TRUE,                           //!< **DONT-CARE**
            TRUE,                           //!< tv_powerdown;
            {0x00,0x00},                    //!< **DONT-CARE**

            /* New added parameters */
            FALSE,                          //!< YCex
            FALSE,                          //!< HLPF
            {FALSE,FALSE,FALSE},            //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS}//!< DitherBits[3]
        },

        (tPANEL_CMD*)tCmdModeCCIR656,                  //!< pCmdQueue
        sizeof(tCmdModeCCIR656)/sizeof(tPANEL_CMD),    //!< nCmd
    },

    /***********       YUV640 MODE         *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_YUV640,         //!< LCDMode
            24.55f,                          //!< fDCLK
            1560,                           //!< uiHSyncTotalPeriod
            1280,                           //!< uiHSyncActivePeriod
            252,                            //!< uiHSyncBackPorch
            262,                            //!< uiVSyncTotalPeriod
            240,                            //!< uiVSyncActivePeriod
            13,                             //!< uiVSyncBackPorchOdd
            13,                             //!< uiVSyncBackPorchEven
            640,                            //!< uiBufferWidth
            240,                            //!< uiBufferHeight
            640,                            //!< uiWindowWidth
            240,                            //!< uiWindowHeight
            TRUE,                          //!< bYCbCrFormat

            /* New added parameters */
            0x00,                           //!< uiHSyncSYNCwidth
            0x00                            //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS,//!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {FALSE,FALSE},                  //!< dithering[2];
            DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_R,                      //!< odd;
            IDE_LCD_G,                      //!< even;
            TRUE,                           //!< hsinv;
            TRUE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            TRUE,                           //!< clkinv;
            FALSE,                          //!< fieldinv;
            FALSE,                          //!< **DONT-CARE**
            FALSE,                          //!< interlace;
            FALSE,                          //!< **DONT-CARE**
            0x40,                           //!< ctrst;
            0x00,                           //!< brt;
            0x40,                           //!< cmults;
            FALSE,                          //!< cex;
            FALSE,                          //!< **DONT-CARE**
            TRUE,                           //!< **DONT-CARE**
            TRUE,                           //!< tv_powerdown;
            {0x00,0x00},                    //!< **DONT-CARE**

            /* New added parameters */
            FALSE,                          //!< YCex
            FALSE,                          //!< HLPF
            {FALSE,FALSE,FALSE},            //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS}//!< DitherBits[3]
        },

        (tPANEL_CMD*)tCmdModeYUV640,                  //!< pCmdQueue
        sizeof(tCmdModeYUV640)/sizeof(tPANEL_CMD),    //!< nCmd
    },

    /***********       CCIR601 MODE         *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_CCIR601,         //!< LCDMode
            24.55f,                          //!< fDCLK
            1560,                           //!< uiHSyncTotalPeriod
            1280,                           //!< uiHSyncActivePeriod
            252,                            //!< uiHSyncBackPorch
            262,                            //!< uiVSyncTotalPeriod
            240,                            //!< uiVSyncActivePeriod
            13,                             //!< uiVSyncBackPorchOdd
            13,                             //!< uiVSyncBackPorchEven
            640,                            //!< uiBufferWidth
            240,                            //!< uiBufferHeight
            640,                            //!< uiWindowWidth
            240,                            //!< uiWindowHeight
            TRUE,                           //!< bYCbCrFormat

            /* New added parameters */
            0x00,                           //!< uiHSyncSYNCwidth
            0x00                            //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_CCIR601,         //!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {FALSE,FALSE},                  //!< dithering[2];
            DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_R,                      //!< odd;
            IDE_LCD_G,                      //!< even;
            TRUE,                           //!< hsinv;
            TRUE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            TRUE,                           //!< clkinv;
            FALSE,                          //!< fieldinv;
            FALSE,                          //!< **DONT-CARE**
            FALSE,                          //!< interlace;
            FALSE,                          //!< **DONT-CARE**
            0x40,                           //!< ctrst;
            0x00,                           //!< brt;
            0x40,                           //!< cmults;
            FALSE,                          //!< cex;
            FALSE,                          //!< **DONT-CARE**
            TRUE,                           //!< **DONT-CARE**
            TRUE,                           //!< tv_powerdown;
            {0x00,0x00},                    //!< **DONT-CARE**

            /* New added parameters */
            FALSE,                          //!< YCex
            FALSE,                          //!< HLPF
            {FALSE,FALSE,FALSE},            //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS}//!< DitherBits[3]
        },

        (tPANEL_CMD*)tCmdModeYUV640,                  //!< pCmdQueue
        sizeof(tCmdModeYUV640)/sizeof(tPANEL_CMD),    //!< nCmd
    },



    /***********       RGB-SERIAL MODE         *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_RGB_SERIAL,      //!< LCDMode
            9.7f,                           //!< fDCLK
            616,                            //!< uiHSyncTotalPeriod
            480,                            //!< uiHSyncActivePeriod
            100,                            //!< uiHSyncBackPorch
            262,                            //!< uiVSyncTotalPeriod
            240,                            //!< uiVSyncActivePeriod
            18,                             //!< uiVSyncBackPorchOdd
            18,                             //!< uiVSyncBackPorchEven
            960,                            //!< uiBufferWidth
            240,                            //!< uiBufferHeight
            960,                            //!< uiWindowWidth
            240,                            //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat

            /* New added parameters */
            0x00,                           //!< uiHSyncSYNCwidth
            0x05                            //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_SERIAL_RGB_8BITS,//!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {FALSE,FALSE},                  //!< dithering[2];
            DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_R,                      //!< odd;
            IDE_LCD_G,                      //!< even;
            TRUE,                           //!< hsinv;
            TRUE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            TRUE,                           //!< clkinv;
            FALSE,                          //!< fieldinv;
            FALSE,                          //!< **DONT-CARE**
            FALSE,                          //!< interlace;
            FALSE,                          //!< **DONT-CARE**
            0x40,                           //!< ctrst;
            0x00,                           //!< brt;
            0x40,                           //!< cmults;
            FALSE,                          //!< cex;
            FALSE,                          //!< **DONT-CARE**
            TRUE,                           //!< **DONT-CARE**
            TRUE,                           //!< tv_powerdown;
            {0x00,0x00},                    //!< **DONT-CARE**

            /* New added parameters */
            FALSE,                          //!< YCex
            FALSE,                          //!< HLPF
            {TRUE,TRUE,TRUE},               //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS}//!< DitherBits[3]
        },

        (tPANEL_CMD*)tCmdModeYUV640,                  //!< pCmdQueue
        sizeof(tCmdModeYUV640)/sizeof(tPANEL_CMD),    //!< nCmd
    },

    /***********       THROUGH MODE         *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_RGB_THROUGH,     //!< LCDMode
            9.7f,                           //!< fDCLK
            616,                            //!< uiHSyncTotalPeriod
            480,                            //!< uiHSyncActivePeriod
            100,                            //!< uiHSyncBackPorch
            262,                            //!< uiVSyncTotalPeriod
            240,                            //!< uiVSyncActivePeriod
            18,                             //!< uiVSyncBackPorchOdd
            18,                             //!< uiVSyncBackPorchEven
            320,                            //!< uiBufferWidth
            240,                            //!< uiBufferHeight
            320,                            //!< uiWindowWidth
            240,                            //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat

            /* New added parameters */
            0x00,                           //!< uiHSyncSYNCwidth
            0x05                            //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_SERIAL_RGB_8BITS,//!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {FALSE,FALSE},                  //!< dithering[2];
            DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_R,                      //!< odd;
            IDE_LCD_G,                      //!< even;
            TRUE,                           //!< hsinv;
            TRUE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            TRUE,                           //!< clkinv;
            FALSE,                          //!< fieldinv;
            FALSE,                          //!< **DONT-CARE**
            FALSE,                          //!< interlace;
            FALSE,                          //!< **DONT-CARE**
            0x40,                           //!< ctrst;
            0x00,                           //!< brt;
            0x40,                           //!< cmults;
            FALSE,                          //!< cex;
            FALSE,                          //!< **DONT-CARE**
            TRUE,                           //!< **DONT-CARE**
            TRUE,                           //!< tv_powerdown;
            {0x00,0x00},                    //!< **DONT-CARE**

            /* New added parameters */
            FALSE,                          //!< YCex
            FALSE,                          //!< HLPF
            {FALSE,FALSE,FALSE},            //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS}//!< DitherBits[3]
        },

        (tPANEL_CMD*)tCmdModeCCIR656,                  //!< pCmdQueue
        sizeof(tCmdModeCCIR656)/sizeof(tPANEL_CMD),    //!< nCmd
    }



};

const tLCD_ROT *tRotate = NULL;

//@}

tLCD_ROT* dispdev_getLcdRotateCmd(UINT32 *ModeNumber)
{
    if(tRotate != NULL)
    {
        *ModeNumber = sizeof(tRotate)/sizeof(tLCD_ROT);
    }
    else
    {
        *ModeNumber = 0;
    }
    return (tLCD_ROT*)tRotate;
}

tLCD_PARAM* dispdev_getConfigMode(UINT32 *ModeNumber)
{
    *ModeNumber = sizeof(tMode)/sizeof(tLCD_PARAM);
    return (tLCD_PARAM*)tMode;
}

tPANEL_CMD* dispdev_getStandbyCmd(UINT32 *CmdNumber)
{
    *CmdNumber = sizeof(tCmdStandby)/sizeof(tPANEL_CMD);
    return (tPANEL_CMD*)tCmdStandby;
}

#if 1
void dispdev_writeToLcdSif(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    UINT32                  uiSIFData;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;

    uiSIFData = ((((UINT32)addr << 8)|(UINT32)value) << 16);

    pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    sif_send(DevIOCtrl.SEL.GET_REG_IF.uiSifCh, uiSIFData, 0, 0);

}


void dispdev_writeToLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    UINT32                  uiSIFData,j;
    UINT32                  SifClk,SifSen,SifData;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;

    pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    SifClk  = DevIOCtrl.SEL.GET_REG_IF.uiGpioClk;
    SifSen  = DevIOCtrl.SEL.GET_REG_IF.uiGpioSen;
    SifData = DevIOCtrl.SEL.GET_REG_IF.uiGpioData;

    gpio_setDir(SifSen, GPIO_DIR_OUTPUT);
    gpio_setDir(SifClk, GPIO_DIR_OUTPUT);
    gpio_setDir(SifData, GPIO_DIR_OUTPUT);

    gpio_setPin(SifSen);
    gpio_setPin(SifData);
    gpio_clearPin(SifClk);

    uiSIFData = (UINT32)addr << 8 | (UINT32)value;

    Delay_DelayUs(500);

    gpio_clearPin(SifSen);

    for(j=16; j>0; j--)
    {
        if (uiSIFData & 0x8000)
        {
            gpio_setPin(SifData);
        }
        else
        {
            gpio_clearPin(SifData);
        }

        Delay_DelayUs(100);
        gpio_setPin(SifClk);
        Delay_DelayUs(200);
        gpio_clearPin(SifClk);
        Delay_DelayUs(100);
        uiSIFData <<= 1;
    }
    Delay_DelayUs(500);
    gpio_setPin(SifSen);

}



void dispdev_readFromLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32* p_value)
{
  //  debug_err(("Has not implement dispdev_readFromLcdGpio \r\n"));

    #if   (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_AUCN01)
    #elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TOPPOLY99000543_636)
    #elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_AUA027DN01)
    #elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_AUA027DN03)
    #elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_AUCN02)
    #elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_PS20P00)
    #elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_RZWT27P02)
    #elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TOPPOLY99000734)
    #elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TOPPOLYA3AA)
    #elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TOPPOLYTD025)
    #elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TOPPOLYTD030WHEA1)
    #elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TXDT240C)
    #elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TXDT270M)
    #elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TXDT270CDR_2V7)
    #elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_WMF4823V47FLW)
    #else
    #endif
}
#endif
