#ifndef _DXDISPLAY_H_
#define _DXDISPLAY_H_

#include "Dx.h"


//built-in driver
//#include "LCDTV.h"

//extern LCD_DEVICE_OBJ   g_LCDCtrlObj;

typedef struct _DISPLAYMODE
{
    UINT32  uiFormat;
    UINT32  uiWidth;
    UINT32  uiHeight;
}
DISPLAYMODE;

typedef struct _DISPLAYMODELIST
{
    int     iMaxMode;
}
DISPLAYMODELIST;

//ver
#define DISPLAY_VER                 0x00010000

//caps id
#define DISPLAY_CAPS_BASE           0x00000001
#define DISPLAY_CAPS_PLUG           0x00000002
#define DISPLAY_CAPS_DEFMODE        0x00000003
#define DISPLAY_CAPS_MODEINFO       0x00000004
#define DISPLAY_CAPS_DIRECT         0x00000005
#define DISPLAY_CAPS_VIDMODE        0x00000006 // supported video modes (for HDMI)

//caps base flag
#define DISPLAY_BF_DETPLUG          0x00000001
#define DISPLAY_BF_BACKLIGHT        0x00000002
#define DISPLAY_BF_DETDIR           0x00000004

#define DISPLAY_CFG_MODE            0x00000001
#define DISPLAY_CFG_DOUT            0x00000002
#define DISPLAY_CFG_DUAL            0x00000003
#define DISPLAY_CFG_INFOBUF         0x00000004

//state id
#define DRVDISP_STATE_ENBALE        0   //enable (R/O)
#define DRVDISP_STATE_MODE          1   //mode (R/O)
#define DRVDISP_STATE_SLEEP         2   //sleep (R/O)
#define DRVDISP_STATE_BACKLIGHT     3   //backlight
#define DRVDISP_STATE_BRIGHTLVL     4   //backlight brightness
#define DRVDISP_STATE_INSERT        5   //TV insert
#define DRVDISP_STATE_LOCK          6   //lock mode (hiding screen to avoid see flikcer)
#define DRVDISP_STATE_LASTMODE      7   //mode (R/O)
#define DRVDISP_STATE_DIRECT        8   //control screen dir by LCD (W/O)

//control id
#define DRVDISP_CTRL_MODE           1   //mode control (for change mode)
#define DRVDISP_CTRL_SLEEP          2   //sleep control (for enter/leave sleep mode)
#define DRVDISP_CTRL_WAITFINISH     5   //wait enable finish (for change device)

//value for DRVDISP_CTRL_ENABLE
//TRUE or FALSE

//value for DRVDISP_CTRL_SLEEP
//TRUE or FALSE

//value for DRVDISP_CTRL_BACKLIGHT
#define DRVDISP_BACKLIGHT_OFF       0
#define DRVDISP_BACKLIGHT_ON        0xff

void DxDisplay_UpdateModeInfo(UINT32 DevID, UINT32 uiLCDMode, USIZE *pWinSize);

extern UINT32 hDevLCD;
extern UINT32 hDevTV;

#endif //_DXDISPLAY_H_

