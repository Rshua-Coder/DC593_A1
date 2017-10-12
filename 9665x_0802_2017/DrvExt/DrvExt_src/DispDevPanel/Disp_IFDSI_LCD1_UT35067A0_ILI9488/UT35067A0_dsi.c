/*
    Display object for driving DSI device

    @file       UT35067A0_dsi.c
    @ingroup
    @note       This panel MUST select IDE clock to PLL1 ( 480 ). Once change to \n
                another frequence, the _IDE_fDCLK should be re calculated

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "debug.h"
#include "display.h"
#include "dispdev_ifDsi.h"
#include "dispdev_panlcomn.h"

#define FW_ROTATE       ENABLE

#if (FW_ROTATE == ENABLE)
#define PANEL_WIDTH     480
#define PANEL_HEIGHT    320
#else
#define PANEL_WIDTH     320
#define PANEL_HEIGHT    480
#endif

#define UT35067A0_IND_MSG(...)       debug_msg(__VA_ARGS__)

#define UT35067A0_ERR_MSG(...)       debug_msg(__VA_ARGS__)

#define UT35067A0_WRN_MSG(...)       debug_msg(__VA_ARGS__)

#define UT35067A0_TE_OFF              0
#define UT35067A0_TE_ON               1

#define UT35067A0_TE_PACKET           0
#define UT35067A0_TE_PIN              1

/*
    RGB888 = 1 pixel = 3bytes packet
    If DSI src = 240MHz, internal clock = 30MHz, data rate = 30MHz x 1bytes = 30MB / sec per lane
    2 lane = 60MB = 20Mpixel ' IDE need > 20MHz
*/


#define DSI_FORMAT_RGB565          0    //N/A in UT35067A0
#define DSI_FORMAT_RGB666P         1    //N/A in UT35067A0
#define DSI_FORMAT_RGB666L         2    //N/A in UT35067A0
#define DSI_FORMAT_RGB888          3    //IDE use 480 & DSI use 480

#define DSI_OP_MODE_CMD_MODE       1
#define DSI_OP_MODE_VDO_MODE       0

#define DSI_PACKET_FORMAT          DSI_FORMAT_RGB888

//#define DSI_TARGET_CLK             240
#define DSI_TARGET_CLK             480
//#define DSI_TARGET_CLK             160
//#define DSI_TARGET_CLK             120
#define DSI_OP_MODE                DSI_OP_MODE_CMD_MODE

#if (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P) && (DSI_OP_MODE == DSI_OP_MODE_CMD_MODE)
#error "Command mode not support RGB666P"
#endif


#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888) || (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
#if(DSI_TARGET_CLK == 160)
#define _IDE_fDCLK      6.66f
#else
#define _IDE_fDCLK      (((DSI_TARGET_CLK / 8)/3)*2)
#endif
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P)
#if(DSI_TARGET_CLK == 160)
#define _IDE_fDCLK      8.88f
#elif(DSI_TARGET_CLK == 120)
#define _IDE_fDCLK      6.66f
#else
#define _IDE_fDCLK      ((((DSI_TARGET_CLK / 8) * 4)/9))
#endif
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
#if(DSI_TARGET_CLK == 120)
#define _IDE_fDCLK      7.5f
#else
#define _IDE_fDCLK      (((DSI_TARGET_CLK / 8)/2))
#endif
#endif




/*
    Panel Parameters for TCON UT35067A0
*/
//@{
/*Used in DSI*/
const tPANEL_CMD tCmdModeDSI[] =
{
    {DSICMD_CMD,     0xF7},     //
    {DSICMD_DATA,    0xA9},     //
    {DSICMD_DATA,    0x51},     //
    {DSICMD_DATA,    0x2C},     //
    {DSICMD_DATA,    0x82},     //

    {DSICMD_CMD,     0xC0},     //  Power Control 1
    {DSICMD_DATA,    0x11},     //  Verg1out =4.52
    {DSICMD_DATA,    0x11},     //  Vreg2out = -4.12

    {DSICMD_CMD,     0xC1},     //  VGH=14.17,VGL =-9.42
    {DSICMD_DATA,    0x41},     //

    {DSICMD_CMD,     0xC5},     //
    {DSICMD_DATA,    0x00},     //
    {DSICMD_DATA,    0x17},     //
    {DSICMD_DATA,    0x80},     //

    {DSICMD_CMD,     0xB1},     //  Frame rate
    {DSICMD_DATA,    0xB0},     //  70Hz
    {DSICMD_DATA,    0x11},     //

    {DSICMD_CMD,     0xB4},     //  Display Inversion Control
    {DSICMD_DATA,    0x02},     //  2-dot

    {DSICMD_CMD,     0xB6},     //  Interface Mode Control
    {DSICMD_DATA,    0x02},     //  RGB/MCU Interface Control
    {DSICMD_DATA,    0x02},     //

    {DSICMD_CMD,     0x55},     //
    {DSICMD_DATA,    0x00},     //

    {DSICMD_CMD,     0xE9},     //
    {DSICMD_DATA,    0x00},     //

#if (DSI_OP_MODE == DSI_OP_MODE_CMD_MODE)
    {DSICMD_CMD,     0x36},     // set memory access control
    {DSICMD_DATA,    0x48},     // bit[3] = 0, RGB filter / bit[3] = 1, BGR filter
#if (DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
    #error "Not support RGB565"
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
    #error "Not support RGB666L"
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
    {DSICMD_CMD,     0x3A},     // Interface pixel format
    {DSICMD_DATA,    0x77},     // x101, x101 [0x77]: 24 bits / pixel
#endif

    {DSICMD_CMD,     0xE0},     //
    {DSICMD_DATA,    0x00},     //
    {DSICMD_DATA,    0x06},     //
    {DSICMD_DATA,    0x06},     //
    {DSICMD_DATA,    0x08},     //
    {DSICMD_DATA,    0x18},     //
    {DSICMD_DATA,    0x0C},     //
    {DSICMD_DATA,    0x41},     //
    {DSICMD_DATA,    0x9B},     //
    {DSICMD_DATA,    0x4F},     //
    {DSICMD_DATA,    0x07},     //
    {DSICMD_DATA,    0x0E},     //
    {DSICMD_DATA,    0x0C},     //
    {DSICMD_DATA,    0x1C},     //
    {DSICMD_DATA,    0x1C},     //
    {DSICMD_DATA,    0x0F},     //

    {DSICMD_CMD,     0xE1},     //
    {DSICMD_DATA,    0x00},     //
    {DSICMD_DATA,    0x1B},     //
    {DSICMD_DATA,    0x1E},     //
    {DSICMD_DATA,    0x03},     //
    {DSICMD_DATA,    0x0E},     //
    {DSICMD_DATA,    0x04},     //
    {DSICMD_DATA,    0x35},     //
    {DSICMD_DATA,    0x24},     //
    {DSICMD_DATA,    0x49},     //
    {DSICMD_DATA,    0x04},     //
    {DSICMD_DATA,    0x0F},     //
    {DSICMD_DATA,    0x0E},     //
    {DSICMD_DATA,    0x37},     //
    {DSICMD_DATA,    0x3A},     //
    {DSICMD_DATA,    0x0F},     //

    //{CMDTEON, 1},             // Enable DSP TE (LCD_8), if the TE pin of DSI LCD doesn't connect to
    //{CMDTEEXTPIN, 1},         // DSP, don't enable it

    {DSICMD_CMD, 0x35 },         // Tearing Effect ON
    {DSICMD_DATA,  0x00},     //

    {DSICMD_CMD,     0x11},     // Sleep OUT
    {CMDDELAY_MS,    120},
    {DSICMD_CMD,     0x29},     // display on
#endif
};


const tPANEL_CMD tCmdStandbyDSI[] =
{
    {DSICMD_CMD,     0x28},     // Display OFF
    {DSICMD_CMD,     0x10}      // Sleep in
};

const tLCD_PARAM tModeDSI[] =
{
    /***********       MI Serial Format 1      *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            //PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666P,   //!< LCDMode
            //PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666P,   //!< LCDMode
            //PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666L,
            //PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666L,   //!< LCDMode
            //PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB565,
#if (DSI_OP_MODE == DSI_OP_MODE_VDO_MODE)
#if (DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
            //PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB565,
            PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB565,
#elif (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P)
            //PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666P,
            PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666P,
#elif (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
            PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666L,
            //PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666L,
#elif (DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
            PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB888,
            //PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB888,
#endif

#else
#if (DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
            PINMUX_DSI_1_LANE_CMD_MODE_RGB565,
#elif (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
            PINMUX_DSI_1_LANE_CMD_MODE_RGB666L,
#elif (DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
            PINMUX_DSI_1_LANE_CMD_MODE_RGB888,
#endif
#endif
            _IDE_fDCLK,                                 //!< fDCLK
#if (FW_ROTATE == ENABLE)
            (0x68+PANEL_HEIGHT),                         //!< uiHSyncTotalPeriod
            PANEL_HEIGHT,                                //!< uiHSyncActivePeriod
            0x34,                                       //!< uiHSyncBackPorch
            0x04+PANEL_WIDTH,                           //!< uiVSyncTotalPeriod
            PANEL_WIDTH,                               //!< uiVSyncActivePeriod
            0x02,                                       //!< uiVSyncBackPorchOdd
            0x02,                                       //!< uiVSyncBackPorchEven
#else
            (0x68+PANEL_WIDTH),                         //!< uiHSyncTotalPeriod
            PANEL_WIDTH,                                //!< uiHSyncActivePeriod
            0x34,                                       //!< uiHSyncBackPorch
            0x04+PANEL_HEIGHT,                           //!< uiVSyncTotalPeriod
            PANEL_HEIGHT,                               //!< uiVSyncActivePeriod
            0x02,                                       //!< uiVSyncBackPorchOdd
            0x02,                                       //!< uiVSyncBackPorchEven
#endif
            PANEL_WIDTH,                                //!< uiBufferWidth
            PANEL_HEIGHT,                               //!< uiBufferHeight
            PANEL_WIDTH,                                //!< uiWindowWidth
            PANEL_HEIGHT,                               //!< uiWindowHeight

            FALSE,                                      //!< bYCbCrFormat

            /* New added parameters */
            0x01,                                       //!< uiHSyncSYNCwidth
            0x01                                        //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_GPIO,            //!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {TRUE, FALSE},                  //!< dithering[2];
            DISPLAY_DEVICE_MIPIDSI,         //!< **DONT-CARE**
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
            {IDE_DITHER_5BITS,IDE_DITHER_6BITS,IDE_DITHER_5BITS},//!< DitherBits[3]
            FALSE                           //!< clk1/2
        },

        (tPANEL_CMD*)tCmdModeDSI,                  //!< pCmdQueue
        sizeof(tCmdModeDSI)/sizeof(tPANEL_CMD),    //!< nCmd
    }
};

const tLCD_ROT *tRotDSI = NULL;

//@}

tLCD_ROT* dispdev_getLcdRotateDSICmd(UINT32 *ModeNumber)
{
    if(tRotDSI != NULL)
    {
        *ModeNumber = sizeof(tRotDSI)/sizeof(tLCD_ROT);
    }
    else
    {
        *ModeNumber = 0;
    }
    return (tLCD_ROT*)tRotDSI;
}

tLCD_PARAM* dispdev_getConfigModeDSI(UINT32 *ModeNumber)
{
    *ModeNumber = sizeof(tModeDSI)/sizeof(tLCD_PARAM);
    return (tLCD_PARAM*)tModeDSI;
}

tPANEL_CMD* dispdev_getStandbyCmdDSI(UINT32 *CmdNumber)
{
    *CmdNumber = sizeof(tCmdStandbyDSI)/sizeof(tPANEL_CMD);
    return (tPANEL_CMD*)tCmdStandbyDSI;
}


void dispdev_setDSIConfig(tDSI_CONFIG *pDsiConfig)
{
    // DSI input source clock = 480
    // Target can be 480 / 240 / 160 / 120
#if 0
    FLOAT   DsiTargetClk = DSI_TARGET_CLK;

    UINT32  Div;



    Div = (UINT32) (pDsiConfig->fDsiSrcClk/DsiTargetClk);

    if(Div == 0)
    {
        UT35067A0_WRN_MSG("Div = 0 force ++\r\n");
        Div++;
    }
    pll_setClockRate(PLL_CLKSEL_DSI_CLKDIV, PLL_DSI_CLKDIV(Div-1));
#else
    dsi_setConfig(DSI_CONFIG_ID_FREQ, DSI_TARGET_CLK * 1000000);
#endif

#if(DSI_TARGET_CLK == 160)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 3);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 7);
    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 2);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 3);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 7);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 8);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 1);
#elif(DSI_TARGET_CLK == 240)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 2);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 7);
    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 3);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 4);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 2);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 9);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 9);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 2);
#elif(DSI_TARGET_CLK == 120)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 1);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 1);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 0);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 4);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 7);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 1);

#endif
    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_0);
    dsi_setConfig(DSI_CONFIG_ID_TE_BTA_INTERVAL, 0x1F);
    dsi_setConfig(DSI_CONFIG_ID_CLK_LP_CTRL, 0x1);
    dsi_setConfig(DSI_CONFIG_ID_SYNC_DLY_CNT, 0xF);
}
