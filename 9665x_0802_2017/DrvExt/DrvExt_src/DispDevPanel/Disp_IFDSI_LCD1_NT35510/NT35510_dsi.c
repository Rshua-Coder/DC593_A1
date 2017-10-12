/*
    Display object for driving DSI device

    @file       NT355510.c
    @ingroup
    @note       This panel MUST select IDE clock to PLL1 ( 480 ). Once change to \n
                another frequence, the _IDE_fDCLK should be re calculated

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "debug.h"
#include "display.h"
#include "dispdev_ifDsi.h"
#include "dispdev_panlcomn.h"

#define PANEL_WIDTH     480
#define PANEL_HEIGHT    800

#define NT35510_IND_MSG(...)       debug_msg(__VA_ARGS__)

#define NT35510_ERR_MSG(...)       debug_msg(__VA_ARGS__)

#define NT35510_WRN_MSG(...)       debug_msg(__VA_ARGS__)
#define NT35510_TE_OFF              0
#define NT35510_TE_ON               1

#define NT35510_TE_PACKET           0
#define NT35510_TE_PIN              1

/*
    RGB888 = 1 pixel = 3bytes packet
    If DSI src = 240MHz, internal clock = 30MHz, data rate = 30MHz x 1bytes = 30MB / sec per lane
    2 lane = 60MB = 20Mpixel ' IDE need > 20MHz
*/


#define DSI_FORMAT_RGB565          0    //IDE use 480 & DSI use 480
#define DSI_FORMAT_RGB666P         1    //IDE use 480 & DSI use 480
#define DSI_FORMAT_RGB666L         2    //IDE use 480 & DSI use 480
#define DSI_FORMAT_RGB888          3    //IDE use 480 & DSI use 480

#define DSI_OP_MODE_CMD_MODE       1
#define DSI_OP_MODE_VDO_MODE       0
#define DSI_PACKET_FORMAT          DSI_FORMAT_RGB888

#define DSI_TARGET_CLK             480
//#define DSI_TARGET_CLK             240
//#define DSI_TARGET_CLK             160
//#define DSI_TARGET_CLK             120
#define DSI_OP_MODE                DSI_OP_MODE_VDO_MODE

#if (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P) && (DSI_OP_MODE == DSI_OP_MODE_CMD_MODE)
#error "Command mode not support RGB666P"
#endif
#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888) || (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
#define _IDE_fDCLK      ((((DSI_TARGET_CLK / 8) * 2)/3))
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P)
#if (DSI_TARGET_CLK == 240)
#define _IDE_fDCLK      26.66f
#else
//#define _IDE_fDCLK      ((((((DSI_TARGET_CLK / 8) * 2))*9)/4))
#if(DSI_TARGET_CLK == 120)
#define _IDE_fDCLK      13.33f
#else

#endif
#endif
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
#define _IDE_fDCLK      ((((DSI_TARGET_CLK / 8) * 2) / 2))
#endif


/*
    Panel Parameters for TCON NT35510
*/
//@{
/*Used in DSI*/
const tPANEL_CMD tCmdModeDSI[] =
{
#if 1
    // SW reset
    {DSICMD_CMD,     0x01},
    {CMDDELAY_MS,    50},

    // Sleep out
    {DSICMD_CMD,     0x11},
    {CMDDELAY_MS,    10},

#if (DSI_OP_MODE == DSI_OP_MODE_CMD_MODE)
    // 480 x 800
/*
    {DSICMD_CMD,     0x2A},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x01},
    {DSICMD_DATA,    0xDF},
    {CMDDELAY_MS,    10},
    {DSICMD_CMD,     0x2B},     // set page address
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x03},
    {DSICMD_DATA,    0x1F},
    {CMDDELAY_MS,    10},
*/
    //{CMDTEON,        NT35510_TE_ON},
    //{CMDTEEXTPIN,    NT35510_TE_PACKET},
    //{CMDTEEXTPIN,    NT35510_TE_PIN},
    //{CMDTEONEACHFRAME,0x1},
    //{CMDBTA,         0x0},
    //{DSICMD_CMD,     0x35},
    //{CMDBTA,         0x0},
    //{DSICMD_CMD,     0x3A},     // Interface pixel format
#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
    {DSICMD_DATA,    0x77},     // x101, x101 [0x55]: 16 bits / pixel
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
    {DSICMD_DATA,    0x66},     // x101, x101 [0x55]: 16 bits / pixel
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
    {DSICMD_DATA,    0x55},     // x101, x101 [0x55]: 16 bits / pixel
#endif
    {CMDDELAY_MS,    10},
                                // x110, x110 [0x66]: 18 bits / pixel
                                // x111, x111 [0x11]: 24 bits / pixel

//    {DSICMD_CMD,     0x36},     // set memory access control
//    {DSICMD_DATA,    0x00},     // bit[3] = 0, RGB filter / bit[3] = 1, BGR filter
//    {CMDDELAY_MS,    10},
    //{DSICMD_CMD,     0xB6},     // set page address
    //{DSICMD_DATA,    0x01},
    //{DSICMD_DATA,    0xC0},
    //{DSICMD_DATA,    0xFF},
    //{DSICMD_DATA,    0x04},

    //{DSICMD_CMD,     0xB5},     // VBP
    //{DSICMD_DATA,    0x04},
    //{DSICMD_DATA,    0x04},

    //{DSICMD_CMD,     0xF2},
    //{DSICMD_DATA,    0x01},

    //{DSICMD_CMD,     0xE0},     // Gamma
    //{DSICMD_DATA,    0x3F},
    //{DSICMD_DATA,    0x1B},
    //{DSICMD_DATA,    0x19},
    //{DSICMD_DATA,    0x27},
    //{DSICMD_DATA,    0x1F},
    //{DSICMD_DATA,    0x0D},
    //{DSICMD_DATA,    0x45},
    //{DSICMD_DATA,    0xB8},
    //{DSICMD_DATA,    0x30},
    //{DSICMD_DATA,    0x17},
    //{DSICMD_DATA,    0x0F},
    //{DSICMD_DATA,    0x05},
    //{DSICMD_DATA,    0x13},
    //{DSICMD_DATA,    0x02},
    //{DSICMD_DATA,    0x00},
    /*
    {DSICMD_CMD,     0xE1},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x24},
    {DSICMD_DATA,    0x26},
    {DSICMD_DATA,    0x08},
    {DSICMD_DATA,    0x10},
    {DSICMD_DATA,    0x12},
    {DSICMD_DATA,    0x3A},
    {DSICMD_DATA,    0x74},
    {DSICMD_DATA,    0x4F},
    {DSICMD_DATA,    0x08},
    {DSICMD_DATA,    0x20},
    {DSICMD_DATA,    0x2A},
    {DSICMD_DATA,    0x3C},
    {DSICMD_DATA,    0x3D},
    {DSICMD_DATA,    0x3F},
    */
    //TEE
    {DSICMD_CMD,     0xFF},
    {DSICMD_DATA,    0xAA},
    {DSICMD_DATA,    0x55},
    {DSICMD_DATA,    0x25},
    {DSICMD_DATA,    0x01},

    {DSICMD_CMD,     0xF8},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x02},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x20},
    {DSICMD_DATA,    0x33},
    {DSICMD_DATA,    0x13},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x48},


    {CMDTEON,        NT35510_TE_ON},
    {CMDTEEXTPIN,    NT35510_TE_PACKET},
    //{CMDTEEXTPIN,    NT35410_TE_PIN},


    //{CMDTEONEACHFRAME,0x1},
    {CMDBTA,         0x0},
    {DSICMD_CMD,     0x35},     //TE-ON
    {CMDBTA,         0x0},
    {CMDDELAY_MS,    10},

#else
    {DSICMD_CMD,     0xF0},
    {DSICMD_DATA,    0x55},
    {DSICMD_DATA,    0xAA},
    {DSICMD_DATA,    0x52},
    {DSICMD_DATA,    0x08},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xB1},
    {DSICMD_DATA,    0xDC},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0x35},
#endif
    // Display on
    {DSICMD_CMD,     0x29},
    {CMDDELAY_MS,    10},

#endif
};


const tPANEL_CMD tCmdStandbyDSI[] =
{
    {DSICMD_CMD,     0x28},         // Display OFF
    {CMDDELAY_MS,    10},
    //{DSICMD_CMD,     0x10},      // Sleep in
    //{CMDDELAY_MS,    10},
};

const tLCD_PARAM tModeDSI[] =
{
    /***********       MI Serial Format 1      *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            //PINMUX_DSI_2_LANE_CMD_MODE_RGB565,    //!< LCDMode
            //PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB565,//!< LCDMode
            //PINMUX_DSI_2_LANE_CMD_MODE_RGB565,
            //PINMUX_DSI_2_LANE_CMD_MODE_RGB666L,
#if (DSI_OP_MODE == DSI_OP_MODE_CMD_MODE)
#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
            PINMUX_DSI_2_LANE_CMD_MODE_RGB888,
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
            PINMUX_DSI_2_LANE_CMD_MODE_RGB666L,
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
            PINMUX_DSI_2_LANE_CMD_MODE_RGB565,
#endif
#else
 #if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
            PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB888,
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
            PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666L,
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P)
            PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666P,
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
            ///PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB565,
            PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB565,
#endif
#endif
            //PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB888,
            //PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB888, //OK
            //PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB565,
            //PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB565, //OK
            //PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB666L,
            //PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666L,
            //PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB666P,
            //PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666P,
            _IDE_fDCLK,                             //!< fDCLK
            (0x68+PANEL_WIDTH),                     //!< uiHSyncTotalPeriod
            PANEL_WIDTH,                            //!< uiHSyncActivePeriod
            0x34,                                   //!< uiHSyncBackPorch
            0x20+PANEL_HEIGHT,                      //!< uiVSyncTotalPeriod
            PANEL_HEIGHT,                           //!< uiVSyncActivePeriod
            0x10,                                   //!< uiVSyncBackPorchOdd
            0x10,                                   //!< uiVSyncBackPorchEven
            PANEL_WIDTH,                            //!< uiBufferWidth
            PANEL_HEIGHT,                           //!< uiBufferHeight
            PANEL_WIDTH,                            //!< uiWindowWidth
            PANEL_HEIGHT,                           //!< uiWindowHeight
            FALSE,                                  //!< bYCbCrFormat

            /* New added parameters */
            0x01,                                   //!< uiHSyncSYNCwidth
            0x01                                    //!< uiVSyncSYNCwidth
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
#if 0
    // DSI input source clock = 480
    // Target can be 480 / 240 / 160 / 120
    FLOAT   DsiTargetClk = DSI_TARGET_CLK;
    UINT32  Div;


    Div = (UINT32) (pDsiConfig->fDsiSrcClk/DsiTargetClk);

    if(Div == 0)
    {
        NT35510_WRN_MSG("Div = 0 force ++\r\n");
        Div++;
    }
    pll_setClockRate(PLL_CLKSEL_DSI_CLKDIV, PLL_DSI_CLKDIV(Div-1));
#else
    dsi_setConfig(DSI_CONFIG_ID_FREQ, DSI_TARGET_CLK * 1000000);
#endif
#if(DSI_TARGET_CLK == 160)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 1);
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
#elif(DSI_TARGET_CLK == 480)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 3);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 15);

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 5);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 7);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 3);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 14);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 10);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 3);

#elif(DSI_TARGET_CLK == 240)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 1);
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
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 0);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 3);

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 2);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 1);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 0);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 5);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 8);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 1);
#endif
    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_1);
    dsi_setConfig(DSI_CONFIG_ID_TE_BTA_INTERVAL, 0x1F);
    //dsi_setConfig(DSI_CONFIG_ID_CLK_PHASE_OFS, 0x3);
    //dsi_setConfig(DSI_CONFIG_ID_PHASE_DELAY_ENABLE_OFS, 0x1);

    dsi_setConfig(DSI_CONFIG_ID_CLK_LP_CTRL, 0x1);
    dsi_setConfig(DSI_CONFIG_ID_SYNC_DLY_CNT, 0xF);
}
