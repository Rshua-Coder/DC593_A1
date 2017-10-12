/*
    Display object object panel configuration parameters for ALT020DSLN

    @file       ALT020DSLN_J2.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "display.h"
#include "dispdev_IF8bits.h"
#include "dispdev_panlcomn.h"


/*
    Panel Parameters for ALT020DSLN
*/
//@{


/*Used in RGB 960*/
const tPANEL_CMD tCmdModeRgbd320[] =
{
{0, 80},
};

const tPANEL_CMD tCmdStandby[] =
{
    {0x05, 0x5E}
};

const tLCD_PARAM tMode[] =
{
    /***********       RGB-SERIAL MODE         *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_RGB_SERIAL,      //!< LCDMode
            40.0f,                           //!< fDCLK
            1716,                            //!< uiHSyncTotalPeriod
            1440+168,                            //!< uiHSyncActivePeriod
            10+39,                             //!< uiHSyncBackPorch
            288,                            //!< uiVSyncTotalPeriod
            272+4,                            //!< uiVSyncActivePeriod
            10,                             //!< uiVSyncBackPorchOdd
            10,                             //!< uiVSyncBackPorchEven
            1440,                            //!< uiBufferWidth
            272,                            //!< uiBufferHeight
            1440,                            //!< uiWindowWidth
            272,                            //!< uiWindowHeight
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
            DISPLAY_DEVICE_TOPPOLY,         //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_R,                      //!< odd;
            IDE_LCD_R,                      //!< even;
            TRUE,                           //!< hsinv;
            TRUE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            0,                           //!< clkinv;
            FALSE,                          //!< fieldinv;
            FALSE,                           //!< rgbdummy
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

        (tPANEL_CMD*)tCmdModeRgbd320,                  //!< pCmdQueue
        sizeof(tCmdModeRgbd320)/sizeof(tPANEL_CMD),    //!< nCmd
    },
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

#define      TimerDelayUs                Delay_DelayUs
void dispdev_writeToLcdSif(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
   // UINT32                  uiSIFData;
   // DISPDEV_IOCTRL_PARAM    DevIOCtrl;

   // uiSIFData = ((((UINT32)addr << 8)|(UINT32)value) << 16);

   // pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    //sif_sendMSB(DevIOCtrl.SEL.GET_REG_IF.uiSifCh, uiSIFData);

}

void dispdev_writeToLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{

}

void dispdev_readFromLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32* p_value)
{
  //  debug_err(("Has not implement dispdev_readFromLcdGpio \r\n"));

}

