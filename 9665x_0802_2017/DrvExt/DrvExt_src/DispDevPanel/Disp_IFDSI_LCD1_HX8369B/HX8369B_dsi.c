/*
    Display object for driving DSI device

    @file       HX8369B_dsi.c
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

#define HX8369B_CFG_1_LANE  0
#define HX8369B_CFG_2_LANE  1

#define HX8369B_CFG_LANE    HX8369B_CFG_2_LANE


#define HX8369B_IND_MSG(...)       debug_msg(__VA_ARGS__)

#define HX8369B_ERR_MSG(...)       debug_msg(__VA_ARGS__)

#define HX8369B_WRN_MSG(...)       debug_msg(__VA_ARGS__)

#define HX8369B_TE_OFF              0
#define HX8369B_TE_ON               1

#define HX8369B_TE_PACKET           0
#define HX8369B_TE_PIN              1

/*
    RGB888 = 1 pixel = 3bytes packet
    If DSI src = 240MHz, internal clock = 30MHz, data rate = 30MHz x 1bytes = 30MB / sec per lane
    2 lane = 60MB = 20Mpixel ' IDE need > 20MHz
*/


#define DSI_FORMAT_RGB565          0    //N/A in HX8369B
#define DSI_FORMAT_RGB666P         1    //N/A in HX8369B
#define DSI_FORMAT_RGB666L         2    //N/A in HX8369B
#define DSI_FORMAT_RGB888          3    //IDE use 480 & DSI use 480

#define DSI_OP_MODE_CMD_MODE       1
#define DSI_OP_MODE_VDO_MODE       0

#define DSI_PACKET_FORMAT          DSI_FORMAT_RGB888



//#define DSI_TARGET_CLK             240
#define DSI_TARGET_CLK             480
//#define DSI_TARGET_CLK             160
//#define DSI_TARGET_CLK             120
#define DSI_OP_MODE                DSI_OP_MODE_VDO_MODE

#if (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P) && (DSI_OP_MODE == DSI_OP_MODE_CMD_MODE)
#error "Command mode not support RGB666P"
#endif
#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888) || (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
#define _IDE_fDCLK      ((((DSI_TARGET_CLK / 8) * 2)/3)) * 2
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P)
#if (DSI_TARGET_CLK == 240)
#define _IDE_fDCLK      53.32f//26.66f * 2
#else
//#define _IDE_fDCLK      ((((((DSI_TARGET_CLK / 8) * 2))*9)/4))
#if(DSI_TARGET_CLK == 120)
#define _IDE_fDCLK      26.66f
#else

#endif
#endif
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
#define _IDE_fDCLK      ((((DSI_TARGET_CLK / 8) * 2) / 2))
#endif


/*
    Panel Parameters for TCON HX8369B
*/
//@{
/*Used in DSI*/
const tPANEL_CMD tCmdModeDSI[] =
{
    {DSICMD_CMD,    0x01},   // SW reset
    {CMDDELAY_MS,   50},

    {DSICMD_CMD,    0xB9},     //Set EXTC
    {DSICMD_DATA,   0xFF},
    {DSICMD_DATA,   0x83},
    {DSICMD_DATA,   0x69},


    {DSICMD_CMD,    0x3A},     // Interface pixel format
#if (DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
    {DSICMD_DATA,   0x55},     // x101, [0x05]: 16 bits / pixel
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P)
    {DSICMD_DATA,   0x66},     // x101, [0x06]: 18 bits / pixel
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
    {DSICMD_DATA,   0x77},     // x101, [0x07]: 24 bits / pixel
#endif



    {DSICMD_CMD,    0xD5},     // Set GIP timing control
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x13},
    {DSICMD_DATA,   0x03},
    {DSICMD_DATA,   0x35},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x01},
    {DSICMD_DATA,   0x10},
    {DSICMD_DATA,   0x01},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x01},
    {DSICMD_DATA,   0x7A},
    {DSICMD_DATA,   0x16},
    {DSICMD_DATA,   0x04},
    {DSICMD_DATA,   0x04},
    {DSICMD_DATA,   0x13},
    {DSICMD_DATA,   0x07},
    {DSICMD_DATA,   0x40},
    {DSICMD_DATA,   0x13},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x20},
    {DSICMD_DATA,   0x10},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x36},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x48},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x85},
    {DSICMD_DATA,   0x42},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x18},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x86},
    {DSICMD_DATA,   0x71},
    {DSICMD_DATA,   0x35},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x58},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x87},
    {DSICMD_DATA,   0x63},
    {DSICMD_DATA,   0x11},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x08},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x84},
    {DSICMD_DATA,   0x50},
    {DSICMD_DATA,   0x24},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x51},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x0F},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x0F},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x0F},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x0F},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x01},
    {DSICMD_DATA,   0x5A},





    {DSICMD_CMD,    0xBA},     // Set MIPI Characteristic

#if (HX8369B_CFG_LANE == HX8369B_CFG_1_LANE)
    {DSICMD_DATA,   0x30},     // 0x30:1 lane, 0x31: 2 lane
#else
    {DSICMD_DATA,   0x31},     // 0x30:1 lane, 0x31: 2 lane
#endif

    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x16},
    {DSICMD_DATA,   0xCA},
//  {DSICMD_DATA,   0x91},     //bit[5]=0, disable EoPT detect
    {DSICMD_DATA,   0xB1},     //bit[5]=1,  enable EoPT detect
    {DSICMD_DATA,   0x0A},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x10},
    {DSICMD_DATA,   0x28},
    {DSICMD_DATA,   0x02},
    {DSICMD_DATA,   0x21},
    {DSICMD_DATA,   0x21},
    {DSICMD_DATA,   0x9A},
    {DSICMD_DATA,   0x1A},
    {DSICMD_DATA,   0x8F},

    {DSICMD_CMD,    0xB1},     // set Power
    {DSICMD_DATA,   0x09},
    {DSICMD_DATA,   0x83},
    {DSICMD_DATA,   0x67},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x92},
    {DSICMD_DATA,   0x12},
    {DSICMD_DATA,   0x16},
    {DSICMD_DATA,   0x16},
    {DSICMD_DATA,   0x0C},
    {DSICMD_DATA,   0x02},


    {DSICMD_CMD,    0xB2},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x70},

    {DSICMD_CMD,    0xB3},     // set RGB display
    {DSICMD_DATA,   0x03},     // RES_SEL[4..0]
                                //      [0x00] = 480 x 1024
                                //      [0x01] = 480 x 864
                                //      [0x02] = 480 x 854
                                //      [0x03] = 480 x 800
                                //      [0x04] = 480 x 720
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x31},
    {DSICMD_DATA,   0x03},
    {DSICMD_DATA,   0x01},
    {DSICMD_DATA,   0x15},
    {DSICMD_DATA,   0x14},     //  RCM = 0, VS+HS+DE ,Mode



    {DSICMD_CMD,    0xB4},     //  Set Display Inversion
    {DSICMD_DATA,   0x02},     //  N_NW[2:0]=・010・: Normal mode 2-dot inversion
                               //  I_NW[2:0]=・000・: Idle mode column inversion

    {DSICMD_CMD,    0xB5},     // set Vref
    {DSICMD_DATA,   0x0B},
    {DSICMD_DATA,   0x0B},
    {DSICMD_DATA,   0x24},

    {DSICMD_CMD,    0xB6},     // set VCOM
    {DSICMD_DATA,   0x7C},
    {DSICMD_DATA,   0x7C},
    {DSICMD_DATA,   0x00},

    {DSICMD_CMD,    0xC0},     // Set Source Driving Ability
    {DSICMD_DATA,   0x73},
    {DSICMD_DATA,   0x50},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x34},
    {DSICMD_DATA,   0xC4},
    {DSICMD_DATA,   0x02},

    {DSICMD_CMD,    0xC1},     // Set DGC
    {DSICMD_DATA,   0x03},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x09},
    {DSICMD_DATA,   0x11},
    {DSICMD_DATA,   0x1A},
    {DSICMD_DATA,   0x21},
    {DSICMD_DATA,   0x29},
    {DSICMD_DATA,   0x31},
    {DSICMD_DATA,   0x38},
    {DSICMD_DATA,   0x40},
    {DSICMD_DATA,   0x48},
    {DSICMD_DATA,   0x50},
    {DSICMD_DATA,   0x58},
    {DSICMD_DATA,   0x61},
    {DSICMD_DATA,   0x69},
    {DSICMD_DATA,   0x70},
    {DSICMD_DATA,   0x78},
    {DSICMD_DATA,   0x80},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x90},
    {DSICMD_DATA,   0x9A},
    {DSICMD_DATA,   0xA1},
    {DSICMD_DATA,   0xAA},
    {DSICMD_DATA,   0xB3},
    {DSICMD_DATA,   0xBA},
    {DSICMD_DATA,   0xC2},
    {DSICMD_DATA,   0xCB},
    {DSICMD_DATA,   0xD2},
    {DSICMD_DATA,   0xDA},
    {DSICMD_DATA,   0xE3},
    {DSICMD_DATA,   0xE9},
    {DSICMD_DATA,   0xF1},
    {DSICMD_DATA,   0xF8},
    {DSICMD_DATA,   0xFF},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x09},
    {DSICMD_DATA,   0x11},
    {DSICMD_DATA,   0x1A},
    {DSICMD_DATA,   0x21},
    {DSICMD_DATA,   0x29},
    {DSICMD_DATA,   0x31},
    {DSICMD_DATA,   0x38},
    {DSICMD_DATA,   0x40},
    {DSICMD_DATA,   0x48},
    {DSICMD_DATA,   0x50},
    {DSICMD_DATA,   0x58},
    {DSICMD_DATA,   0x61},
    {DSICMD_DATA,   0x69},
    {DSICMD_DATA,   0x70},
    {DSICMD_DATA,   0x78},
    {DSICMD_DATA,   0x80},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x90},
    {DSICMD_DATA,   0x9A},
    {DSICMD_DATA,   0xA1},
    {DSICMD_DATA,   0xAA},
    {DSICMD_DATA,   0xB3},
    {DSICMD_DATA,   0xBA},
    {DSICMD_DATA,   0xC2},
    {DSICMD_DATA,   0xCB},
    {DSICMD_DATA,   0xD2},
    {DSICMD_DATA,   0xDA},
    {DSICMD_DATA,   0xE3},
    {DSICMD_DATA,   0xE9},
    {DSICMD_DATA,   0xF1},
    {DSICMD_DATA,   0xF8},
    {DSICMD_DATA,   0xFF},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x09},
    {DSICMD_DATA,   0x11},
    {DSICMD_DATA,   0x1A},
    {DSICMD_DATA,   0x21},
    {DSICMD_DATA,   0x29},
    {DSICMD_DATA,   0x31},
    {DSICMD_DATA,   0x38},
    {DSICMD_DATA,   0x40},
    {DSICMD_DATA,   0x48},
    {DSICMD_DATA,   0x50},
    {DSICMD_DATA,   0x58},
    {DSICMD_DATA,   0x61},
    {DSICMD_DATA,   0x69},
    {DSICMD_DATA,   0x70},
    {DSICMD_DATA,   0x78},
    {DSICMD_DATA,   0x80},
    {DSICMD_DATA,   0x88},
    {DSICMD_DATA,   0x90},
    {DSICMD_DATA,   0x9A},
    {DSICMD_DATA,   0xA1},
    {DSICMD_DATA,   0xAA},
    {DSICMD_DATA,   0xB3},
    {DSICMD_DATA,   0xBA},
    {DSICMD_DATA,   0xC2},
    {DSICMD_DATA,   0xCB},
    {DSICMD_DATA,   0xD2},
    {DSICMD_DATA,   0xDA},
    {DSICMD_DATA,   0xE3},
    {DSICMD_DATA,   0xE9},
    {DSICMD_DATA,   0xF1},
    {DSICMD_DATA,   0xF8},
    {DSICMD_DATA,   0xFF},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},

    {DSICMD_CMD,    0xC6},     // Set ECO
    {DSICMD_DATA,   0x41},
    {DSICMD_DATA,   0xFF},
    {DSICMD_DATA,   0x7A},
    //bit[0]BGR_PANEL : The color filter order direction selected.
    //  0 : S1:S2:S3=R:G:B
    //  1 : S1:S2:S3=B:G:R
    //bit[1]REV_PANEL :  The source output data polarity selected.
    //  0: normally white panel.
    //  1: normally black panel.
    //bit[2]GS_PANEL : The gate driver output shift direction selected.
    //  0: G1G1024
    //  1: G1024G1
    //bit[3]SS_PANEL :The source driver output shift direction selected.
    //  0: S1S1440
    //  1: S1440S1
    {DSICMD_CMD,    0xCC},     //  Set Panel
    {DSICMD_DATA,   0x02},     //  REV_PANEL = 1                   => GIP Forward Scan
//  {DSICMD_DATA,   0x00},     //  GS_PANEL = 1, REV_PANEL = 1    => GIP backward Scan


    {DSICMD_CMD,    0xE3},     // set  EQ
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x13},
    {DSICMD_DATA,   0x1B},

    {DSICMD_CMD,    0xEA},     //  Set MeSSI
    {DSICMD_DATA,   0x62},     //  CABC_PWM1_OE = 0,   CABC_PWM1_OUT output enable
                               //  CABC_PWM2_OE = 0,   CABC_PWM2_OUT output enable
                               //  VCOM_PWR =  1, VCOM power source comes from VSN

    {DSICMD_CMD,    0xE0},    //  Set Gamma
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x05},
    {DSICMD_DATA,   0x0B},
    {DSICMD_DATA,   0x2F},
    {DSICMD_DATA,   0x2F},
    {DSICMD_DATA,   0x30},
    {DSICMD_DATA,   0x1B},
    {DSICMD_DATA,   0x3D},
    {DSICMD_DATA,   0x07},
    {DSICMD_DATA,   0x0D},
    {DSICMD_DATA,   0x0E},
    {DSICMD_DATA,   0x12},
    {DSICMD_DATA,   0x13},
    {DSICMD_DATA,   0x12},
    {DSICMD_DATA,   0x13},
    {DSICMD_DATA,   0x11},
    {DSICMD_DATA,   0x1A},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x05},
    {DSICMD_DATA,   0x0B},
    {DSICMD_DATA,   0x2F},
    {DSICMD_DATA,   0x2F},
    {DSICMD_DATA,   0x30},
    {DSICMD_DATA,   0x1B},
    {DSICMD_DATA,   0x3D},
    {DSICMD_DATA,   0x07},
    {DSICMD_DATA,   0x0D},
    {DSICMD_DATA,   0x0E},
    {DSICMD_DATA,   0x12},
    {DSICMD_DATA,   0x13},
    {DSICMD_DATA,   0x12},
    {DSICMD_DATA,   0x13},
    {DSICMD_DATA,   0x11},
    {DSICMD_DATA,   0x1A},
    {DSICMD_DATA,   0x01},

    {DSICMD_CMD,    0x11},     // sleep out
    {CMDDELAY_MS,   120},


    {DSICMD_CMD,    0x29},     // display on
    {CMDDELAY_MS,   10},
    {CMDDELAY_MS,   10},
    {CMDDELAY_MS,   10},
    {CMDDELAY_MS,   10},
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
#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
#if (HX8369B_CFG_LANE == HX8369B_CFG_1_LANE)
            PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB888,
#else
            PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB888,
            //PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB888,

#endif
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
            PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666L,
            //PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666L,
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P)
            //PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666P,
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
#if (HX8369B_CFG_LANE == HX8369B_CFG_1_LANE)
            PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB565,
#else
            //PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB565,
            PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB565,
            //PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB565,
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
            //0x2F,                                   //!< uiHSyncBackPorch
            0x40,                                   //!< uiHSyncBackPorch
            0x20+PANEL_HEIGHT,                      //!< uiVSyncTotalPeriod
            PANEL_HEIGHT,                           //!< uiVSyncActivePeriod
            0x2,                                    //!< uiVSyncBackPorchOdd
            0x2,                                    //!< uiVSyncBackPorchEven
            PANEL_HEIGHT,                           //!< uiBufferWidth
            PANEL_WIDTH,                            //!< uiBufferHeight
            PANEL_WIDTH,                            //!< uiWindowWidth
            PANEL_HEIGHT,                           //!< uiWindowHeight
            FALSE,                                  //!< bYCbCrFormat

            /* New added parameters */
            //0x01,                                   //!< uiHSyncSYNCwidth
            0x20,                                   //!< uiHSyncSYNCwidth
            //0x01                                    //!< uiVSyncSYNCwidth
            0x02                                    //!< uiVSyncSYNCwidth
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
    dsi_setConfig(DSI_CONFIG_ID_FREQ, DSI_TARGET_CLK * 1000000);
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
#if (HX8369B_CFG_LANE == HX8369B_CFG_1_LANE)
    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_0);
#else
    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_1);
#endif
    dsi_setConfig(DSI_CONFIG_ID_TE_BTA_INTERVAL, 0x1F);
    //dsi_setConfig(DSI_CONFIG_ID_CLK_PHASE_OFS, 0x3);
    //dsi_setConfig(DSI_CONFIG_ID_PHASE_DELAY_ENABLE_OFS, 0x1);

    dsi_setConfig(DSI_CONFIG_ID_CLK_LP_CTRL, 0x1);
    dsi_setConfig(DSI_CONFIG_ID_SYNC_DLY_CNT, 0xF);

    dsi_setConfig(DSI_CONFIG_ID_EOT_PKT_EN, 1);
}
