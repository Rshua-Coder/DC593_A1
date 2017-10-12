/*
    Panel device open/close Common control header

    @file       dispdev_panel.h
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _DISPDEV_PANLCOMN_H
#define _DISPDEV_PANLCOMN_H

#include "display.h"
#include "top.h"
#include "ide.h"
#include "dispdev_ioctrl.h"



#define DISPANL_DEBUG  DISABLE
#if DISPANL_DEBUG
#define dispanl_debug(msg)       debug_msg msg
#else
#define dispanl_debug(msg)
#endif



/*
    Structure for standard format of Panel Register Access
*/
typedef struct
{
    UINT32  uiAddress;                      // Panel Register Address
    UINT32  uiValue;                        // Panel Register config value
}tPANEL_CMD;


/*
    Structure for standard format of panel open parameters
*/
typedef struct
{
    /* Old prototype */

    PINMUX_LCDINIT  LCDMode;                // LCD Mode Select
    float           fDCLK;                  // LCD Target Clock Rate

    UINT32          uiHSyncTotalPeriod;     // Horizontal Total Period
    UINT32          uiHSyncActivePeriod;    // Horizontal Active Width
    UINT32          uiHSyncBackPorch;       // Horizontal Backporth

    UINT32          uiVSyncTotalPeriod;     // Vertical Total Period
    UINT32          uiVSyncActivePeriod;    // Vertical Active Width
    UINT32          uiVSyncBackPorchOdd;    // Vertical Odd Lines Backporch
    UINT32          uiVSyncBackPorchEven;   // Vertical Even Lines Backporch

    UINT32          uiBufferWidth;          // Buffer Width returned for upper layer
    UINT32          uiBufferHeight;         // Buffer Height returned for upper layer
    UINT32          uiWindowWidth;          // Window Width returned for upper layer
    UINT32          uiWindowHeight;         // Window Height returned for upper layer
    BOOL            bYCbCrFormat;           // TRUE(YUV): YUV/UPS051/UPS052,  FALSE(RGB): RGB/RGBD

    /* New added parameters */
    UINT32          uiHSyncSYNCwidth;       // Horizontal Sync pulse Width
    UINT32          uiVSyncSYNCwidth;       // Vertical Sync pulse Width

}tPANEL_PARAM;



/*
    Structure for standard format of IDE paramrters
*/
typedef struct
{
    /* Old prototype */

    PINMUX_LCD_SEL  pinmux_select_lcd;  // LCD Pinmux select
    ICST_TYPE       icst;               // ICST Output Type
    BOOL            dithering[2];       // [0]: Dither Enable.   [1]: Dither FreeRun
    IDE_DEVICE_TYPE device;             // **DONT-CARE** Useless parameter in current version
    IDE_PDIR        pdir;               // RGB Pixel Output Direction
    IDE_PORDER      odd;                // LCD Odd Lines pixel Start from R or G or B.
    IDE_PORDER      even;               // LCD Odd Lines pixel Start from R or G or B.
    BOOL            hsinv;              // H-SYNC Invert
    BOOL            vsinv;              // V-SYNC Invert
    BOOL            hvldinv;            // H-Valid Invert
    BOOL            vvldinv;            // V-Valid Invert
    BOOL            clkinv;             // PIXEL CLOCK Invert
    BOOL            fieldinv;           // FIELD Invert
    BOOL            rgbdummy;           // **DONT-CARE** Useless parameter in current version
    BOOL            interlace;          // Output interlaced format
    BOOL            startfield;         // **DONT-CARE** Useless parameter in current version
    UINT32          ctrst;              // Contrast Value
    UINT32          brt;                // Brightness Value
    UINT32          cmults;             // Saturation Value
    BOOL            cex;                // Cb/Cr Exchane
    BOOL            standby_signal;     // **DONT-CARE** Useless parameter in current version
    BOOL            clamp;              // clamp
    BOOL            tv_powerdown;       // Assert TV power down in panel open
    UINT32          sif_startend[2];    // **DONT-CARE** Useless parameter in current version


    /* New added parameters */
    BOOL             YCex;               // Y/C exchange
    BOOL             HLPF;               // Horizontal lowpass filter
    BOOL             subpixOdd[3];       // Sub-Pixel ON/OFF for Odd lines. [0]:R   [1]:G   [2]:B
    BOOL             subpixEven[3];      // Sub-Pixel ON/OFF for Even lines.[0]:R   [1]:G   [2]:B
    IDE_DITHER_VBITS DitherBits[3];      // [0]:R dither bits. [1]:G dither bits. [2]:B dither bits. This is valid only if dithering[0]=TRUE.
    BOOL             clk1_2;             // clock 1/2
}tIDE_PARAM;



typedef struct
{
    tPANEL_PARAM Panel;         // Panel Related Paramters
    tIDE_PARAM   IDE;           // IDE Related Paramters
    tPANEL_CMD*  pCmdQueue;     // Panel Register Command Queue
    UINT32       nCmd;          // Panel Register Command Queue Number
}tLCD_PARAM;



typedef enum
{
    tLCD_INF_SERIAL_8BITS,      // Such as RGB-Serial/RGB-Dummy/RGB-Through/YUV
    tLCD_INF_PARALLEL_16BITS,   // Such as Parallel RGB565 / YUV
    tLCD_INF_MI,                // Interface is Memory interface
    tLCD_INF_MIPI_DSI,

    ENUM_DUMMY4WORD(tLCD_INF)
}tLCD_INF;



typedef struct
{
    DISPDEV_LCD_ROTATE  Rotation;            // LCD Roate ID
    tPANEL_CMD*         pCmdQueue;      // LCD Roate ID Command Queue
    UINT32              nCmd;           // LCD Roate ID Command Queue Number
}tLCD_ROT;





extern ER dispdev_setDisplay(DISPDEV_IOCTRL pDispDevControl, tLCD_INF LcdInf, tLCD_PARAM *pMode, PINMUX_FUNC_ID PinFuncID);

#endif
