/*
    Display object object Golden Sample for driving Memory interface panel

    @file       ILI9341.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "debug.h"
#include "display.h"
#include "dispdev_ifmi.h"
#include "dispdev_panlcomn.h"


/*
    Panel Parameters for TCON ILI9341
*/
//@{
/*Used in MI format*/
const tPANEL_CMD tCmdModeMiFmt3[] =
{

//************* Start Initial Sequence **********//

  {MICMD_CMD,0xCF},
  {MICMD_DATA,0x00},
  {MICMD_DATA,0x81},
  {MICMD_DATA,0X30},


  {MICMD_CMD,0xED},
  {MICMD_DATA,0x64},
  {MICMD_DATA,0x03},
  {MICMD_DATA,0x12},
  {MICMD_DATA,0x81},

  {MICMD_CMD,0xE8},
  {MICMD_DATA,0x85},
  {MICMD_DATA,0x00},
  {MICMD_DATA,0x79},


  {MICMD_CMD,0xCB},
  {MICMD_DATA,0x39},
  {MICMD_DATA,0x2C},
  {MICMD_DATA,0X00},
  {MICMD_DATA,0X34},
  {MICMD_DATA,0X02},



 {MICMD_CMD,0xF6},
 {MICMD_DATA,0x01},
 {MICMD_DATA,0x30},
 {MICMD_DATA,0x00},

 {MICMD_CMD,0xF7},
 {MICMD_DATA,0x20},

  {MICMD_CMD,0xEA},
  {MICMD_DATA,0x00},
  {MICMD_DATA,0x00},

 {MICMD_CMD,0xC0}, //Power control
 {MICMD_DATA,0x21}, //VRH[5:0]      //21

 {MICMD_CMD,0xC1}, //Power control
 {MICMD_DATA,0x02}, //SAP[2:0];BT[3:0]   //12

 {MICMD_CMD,0xC5}, //VCM control
 {MICMD_DATA,0x50},  //30
 {MICMD_DATA,0x30},  //3//3F

{MICMD_CMD,0xC7}, //VCM control2
{MICMD_DATA,0x95},//0xA6    //93

#if 0
    {MICMD_CMD,     0x2A},  //Column address set
    {MICMD_DATA,    0x00},
    {MICMD_DATA,    0x00},
    {MICMD_DATA,    0x01},
    {MICMD_DATA,    0x3F},
    {MICMD_CMD,     0x2B},  // row address set
    {MICMD_DATA,    0x00},
    {MICMD_DATA,    0x00},
    {MICMD_DATA,    0x00},
    {MICMD_DATA,    0xEF},
#endif

  {MICMD_CMD,0x3A}, // Memory Access Control
  {MICMD_DATA,0x66}, //66

  {MICMD_CMD,0x36}, // Memory Access Control
//  {MICMD_DATA,0x88},
  //{MICMD_DATA,0x98}, //08   48
  {MICMD_DATA,0x48}, //08   48

  {MICMD_CMD,0xB1},
  {MICMD_DATA,0x00},
  {MICMD_DATA,0x15},///1f



  {MICMD_CMD,0xF2}, // #Gamma function Disable
  {MICMD_DATA,0x00},

  {MICMD_CMD,0x26}, //Gamma curve selected
  {MICMD_DATA,0x01},

  {MICMD_CMD,0x35}, //Set TE ON  //ADD
  {MICMD_DATA,0x00},

  {MICMD_CMD,0x44}, //   ADD
  {MICMD_DATA,0x00},
  {MICMD_DATA,0x3f},

 {MICMD_CMD,0xb5}, // ADD
 {MICMD_DATA,0x02},
 {MICMD_DATA,0x02},
 {MICMD_DATA,0xff},//0a
 {MICMD_DATA,0xff},///14

  // {MICMD_CMD,0xB6},
  //{MICMD_DATA,0x0a},
  //{MICMD_DATA,0xA2},//A2



  {MICMD_CMD,0xE0}, //Set Gamma
  {MICMD_DATA,0x0F},
  {MICMD_DATA,0x31},
  {MICMD_DATA,0x30},
  {MICMD_DATA,0x0B},
  {MICMD_DATA,0x0E},
  {MICMD_DATA,0x08},
  {MICMD_DATA,0x5B},
  {MICMD_DATA,0xB8},
  {MICMD_DATA,0x49},
  {MICMD_DATA,0x0B},
  {MICMD_DATA,0x00},
  {MICMD_DATA,0x00},
  {MICMD_DATA,0x00},
  {MICMD_DATA,0x00},
  {MICMD_DATA,0x00},

  {MICMD_CMD,0xE1}, //Set Gamma
  {MICMD_DATA,0x00},
  {MICMD_DATA,0x0E},
  {MICMD_DATA,0x0F},
  {MICMD_DATA,0x05},
  {MICMD_DATA,0x11},
  {MICMD_DATA,0x07},
  {MICMD_DATA,0x23},
  {MICMD_DATA,0x47},
  {MICMD_DATA,0x36},
  {MICMD_DATA,0x04},
  {MICMD_DATA,0x1F},
  {MICMD_DATA,0x0F},
  {MICMD_DATA,0x3F},
  {MICMD_DATA,0x3F},
  {MICMD_DATA,0x0F},

  {MICMD_CMD,0x11}, //Exit Sleep
  {CMDDELAY_MS,   120},
  {MICMD_CMD,0x29}, //Display on
  {MICMD_CMD,0x2c}, //memory write
};


const tPANEL_CMD tCmdStandbyMI[] =
{

//  {MICMD_CMD,0x28}, //Display off
  {CMDDELAY_US, 1},
//  {MICMD_CMD,0x10}, //enter sleep mode
//  {MICMD_CMD,0x2c}, //memory write
};

const tLCD_PARAM tModeMI[] =
{
    /***********       MI Serial Format 1      *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_MI_FMT3,         //!< LCDMode
            6.80f,                          //!< fDCLK
            (0x20+240),                     //!< uiHSyncTotalPeriod
            240,                            //!< uiHSyncActivePeriod
            0x10,                           //!< uiHSyncBackPorch
            0x8+320,                        //!< uiVSyncTotalPeriod
            320,                            //!< uiVSyncActivePeriod
            0x02,                           //!< uiVSyncBackPorchOdd
            0x02,                           //!< uiVSyncBackPorchEven
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
            PINMUX_LCD_SEL_PARALLE_MI_8BITS|PINMUX_LCD_SEL_TE_ENABLE,  //!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {FALSE, FALSE},                   //!< dithering[2];
            DISPLAY_DEVICE_MI,              //!< **DONT-CARE**
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

        (tPANEL_CMD*)tCmdModeMiFmt3,                  //!< pCmdQueue
        sizeof(tCmdModeMiFmt3)/sizeof(tPANEL_CMD),    //!< nCmd
    }
};

const tPANEL_CMD tCmdRotate_None[] =
{
  {MICMD_CMD,0x36}, // Memory Access Control
  {MICMD_DATA,0x48},//48

     //{MICMD_CMD,0xB6},
     //{MICMD_DATA,0x0a},
     //{MICMD_DATA,0x82},//A2

 // {MICMD_CMD,0xB6},
 // {MICMD_DATA,0x0a},
 // {MICMD_DATA,0x82},//A2

//  {MICMD_CMD,0x0B}, // Read Display MADCTL
//  {MICMD_DATA,0x00},
  {MICMD_CMD,0x2c}, //memory write

};
const tPANEL_CMD tCmdRotate_180[] =
{


  {MICMD_CMD,0x36}, // Memory Access Control
  {MICMD_DATA,0x98},  //88

//  {MICMD_CMD,0x0B}, // Read Display MADCTL
//  {MICMD_DATA,0x14},
   //{MICMD_CMD,0xB6},
 // {MICMD_DATA,0x0a},
 //  {MICMD_DATA,0xA2},//A2  gs=1

  {MICMD_CMD,0x2c}, //memory write
};
const tLCD_ROT tRotMI[] =
{
    {DISPDEV_LCD_ROTATE_NONE,(tPANEL_CMD*)tCmdRotate_None,3},
    {DISPDEV_LCD_ROTATE_180,(tPANEL_CMD*)tCmdRotate_180,3}
};

//@}

tLCD_ROT* dispdev_getLcdRotateMiCmd(UINT32 *ModeNumber)
{
    if(tRotMI != NULL)
    {
        *ModeNumber = sizeof(tRotMI)/sizeof(tLCD_ROT);
    }
    else
    {
        *ModeNumber = 0;
    }
    return (tLCD_ROT*)tRotMI;
}

tLCD_PARAM* dispdev_getConfigModeMI(UINT32 *ModeNumber)
{
    *ModeNumber = sizeof(tModeMI)/sizeof(tLCD_PARAM);
    return (tLCD_PARAM*)tModeMI;
}

tPANEL_CMD* dispdev_getStandbyCmdMI(UINT32 *CmdNumber)
{
    *CmdNumber = sizeof(tCmdStandbyMI)/sizeof(tPANEL_CMD);
    return (tPANEL_CMD*)tCmdStandbyMI;
}

void dispdev_writeToLcdMi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    if(addr & MICMD_DATA)
        mi_setCmd1(WrDATA, (value&0xff));
    else
        mi_setCmd1(WrCMD, (value&0xff));

    mi_setCmdCnt(1);
    mi_waitCmdDone();
}


void dispdev_readFromLcdMi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *pValue)
{
    *pValue = 0;
    debug_err(("No MI Read reg implement!\r\n"));
}

void      dispdev_setMiConfig(tMI_CONFIG *pMiConfig)
{
    FLOAT   MiTargetClk = 50.0; // Max is 60MHz

    mi_setCtrl(PARALLEL_I80, MI_AUTOFMT3, pMiConfig->DataFmt, FALSE, FALSE, FALSE, FALSE);
    //mi_setSerialCsCtrl(TRUE);
    //mi_setPolCtrl(FALSE, FALSE, FALSE, FALSE, SCLMOD0);
    //mi_setSerialAutoRsBitValue(TRUE);
    #if 0
    Div = (UINT32) (pMiConfig->fMiSrcClk/MiTargetClk);
    pll_setClockRate(PLL_CLKSEL_MI_CLKDIV, PLL_MI_CLKDIV(Div-1));
    #else
    mi_setConfig(MI_CONFIG_ID_FREQ, MiTargetClk*1000000);
    #endif
}

void dispdev_setMiAutoCmd(void)
{
    BOOL        bTeSyncEn   = TRUE;
    MI_TYPE     Type;
    MI_AUTO_FMT AutoType;
    MI_DATAFMT  DataFMT;
    BOOL        bRGBSWAP;
    BOOL        bBITSWAP;
    BOOL        bSync;
    BOOL        bPINMUX;

    mi_getCtrl(&Type, &AutoType, &DataFMT, &bRGBSWAP, &bBITSWAP, &bSync, &bPINMUX);
    mi_setCtrl(Type, AutoType, DataFMT, bRGBSWAP, bBITSWAP, bTeSyncEn, bPINMUX);
    return;
}
