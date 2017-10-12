/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       DetTV.c
    @ingroup    mIPRJAPKeyIO

    @brief      Detect USB is plugging in or unplugged
                Detect USB is plugging in or unplugged

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "DxCfg.h"
#include "IOCfg.h"

#include "DxDisplay.h"
#include "DxCommon.h"
#include "DxApi.h"
#include "DispDef.h"

//driver
#include "display.h"
#include "pll.h"
#include "stdio.h" //for sscanf

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define HDMI_FORMAT_DEFAULT     HDMI_640X480P60

static DISPDEV_PARAM    DispDev;
static DISPCTRL_PARAM   DispCtrl;
static PDISP_OBJ        pObj = 0;
static PDISPDEV_PARAM   pDev = &DispDev;
static PDISPCTRL_PARAM  pCtrl = &DispCtrl;

static UINT32           g_HDMIEnable = FALSE; //not yet enable
// Exported Variables
static UINT32           g_HDMIDout = 0; //current dout
static UINT32           g_HDMIMode = 0; //current mode
static UINT32           g_HDMILastMode = HDMI_FORMAT_DEFAULT; //current mode

// Local Variables
static UINT32           g_HDMISleep = FALSE; //not yet sleep
static UINT32           g_HDMIBacklightEn = FALSE;

static BOOL g_bHDMIWaitTurnOnFinish = FALSE;

static UINT32 g_localMode = 0; //new mode
static USIZE  g_localSize = {0,0};//new size
static USIZE* pCurrentSize = 0; //return size

//public func
UINT32 DrvHDMIGetVideoMode(void);
UINT32 DrvHDMIGetcaps(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvHDMISetcfgs(UINT32 CfgID, UINT32 Param1); // Set Config Setting
UINT32 DrvHDMIInit(void* pInitParam); // Set Init Parameters
UINT32 DrvHDMIOpen(void); // Common Constructor
UINT32 DrvHDMIClose(void); // Common Destructor
UINT32 DrvHDMIState(UINT32 StateID, UINT32 Value); // General Properties
UINT32 DrvHDMIControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
UINT32 DrvHDMICommand(CHAR *pcCmdStr); //General Command Console

//internal func
void DrvHDMI_TurnOn(void);
void DrvHDMI_TurnOn_WaitFinish(void);
void DrvHDMI_TurnOff(void);
void DrvHDMI_SleepEnter(void);
void DrvHDMI_SleepLeave(void);
void GPIOMap_TurnOnHDMIBacklight(void);
void GPIOMap_TurnOffHDMIBacklight(void);

typedef struct _HDMI_FORMAT
{
    UINT32          uiIndex;
    char            str[16];
    HDMI_VIDEOID    VideoID;
    UINT32          uiWidth;
    UINT32          uiHeight;

} HDMI_FORMAT;

HDMI_FORMAT g_HDMIFormat[] =
{
    { 1,    {"640x480p60"},     HDMI_640X480P60,    640,    480},
    { 2,    {"720x480p60"},     HDMI_720X480P60,    720,    480},
    { 3,    {"720x576p50"},     HDMI_720X576P50,    720,    576},
    { 4,    {"1280x720p60"},    HDMI_1280X720P60,   1280,   720},
    { 5,    {"1920x1080p30"},   HDMI_1920X1080P30,  1920,   1080},
    { 6,    {"1920x1080i50"},   HDMI_1920X1080I50,  1920,   1080},
    { 7,    {"1920x1080i60"},   HDMI_1920X1080I60,  1920,   1080},
//    { 8,    {"1920x1080p50"},   HDMI_1920X1080P50,  1920,   1080}, // bandwidth may be not enough
//    { 9,    {"1920x1080p60"},   HDMI_1920X1080P60,  1920,   1080}, // bandwidth may be not enough
};

#define HDMI_FORMAT_MAX     (sizeof(g_HDMIFormat)/ sizeof(HDMI_FORMAT))

//dx object
DX_OBJECT gDevHDMI =
{
    DXFLAG_SIGN,
    DX_CLASS_DISPLAY_EXT|DX_TYPE_HDMIOUT,
    DISPLAY_VER,
    "Display_HDMI",
    0,0,0,0,
    DrvHDMIGetcaps,
    DrvHDMISetcfgs,
    DrvHDMIInit,
    DrvHDMIOpen,
    DrvHDMIClose,
    DrvHDMIState,
    DrvHDMIControl,
    DrvHDMICommand,
    0,
};

UINT32 hDevHDMI = (UINT32)&gDevHDMI;

static void _DrvHDMIHookDout(UINT32 id)
{
    pObj = 0;
    if(id == 0)
        pObj = disp_getDisplayObject(DISP_1);
    if(id == 1)
        pObj = disp_getDisplayObject(DISP_2);
    if(!pObj)
    {
        DBG_ERR("invalid display id = %d!\r\n", id);
        return;
    }
#if (_HDMITYPE_ == _HDMI_ON_)
    pDev->SEL.HOOK_DEVICE_OBJECT.DevID         = DISPDEV_ID_TVHDMI;
    pDev->SEL.HOOK_DEVICE_OBJECT.pDispDevObj   = dispdev_getHdmiDevObj();
    pObj->devCtrl(DISPDEV_HOOK_DEVICE_OBJECT,pDev);
#endif
}

UINT32 DrvHDMIGetVideoMode(void)
{
    UINT32  i, j;
    UINT32  uiEDID;
    UINT32  uiLen;
    UINT32  uiVideoMode = HDMI_FORMAT_DEFAULT;
    HDMI_VDOABI VideoAbility[32]; // EDID max is 32
    HDMI_FORMAT *pHDMIFormat = NULL;

    hdmitx_open();

    uiEDID = hdmitx_getEdidInfo();
    if (uiEDID)
    {
        hdmitx_getVideoAbility(&uiLen, VideoAbility);

        for (i = 0; i < uiLen; i++)
        {
            DBG_IND("HDMI Video Ability %d: %d\r\n", i, VideoAbility[i].VideoID);
            for (j = 0; j < HDMI_FORMAT_MAX; j++)
            {
                if (g_HDMIFormat[j].VideoID == VideoAbility[i].VideoID)
                {
                    if (pHDMIFormat == NULL)
                    {
                        pHDMIFormat = &g_HDMIFormat[j];
                        DBG_IND("match: %d %s\r\n", j, pHDMIFormat->str);
                    }
                    else if(g_HDMIFormat[j].uiIndex > pHDMIFormat->uiIndex)
                    {
                        pHDMIFormat = &g_HDMIFormat[j];
                        DBG_IND("match: %d %s\r\n", j, pHDMIFormat->str);
                    }
                }
            }
        }

        if (pHDMIFormat == NULL)
        {
            DBG_ERR("HDMI format not match!\r\n");
        }
        else
        {
            DBG_IND("HDMI resolution: %s\r\n", pHDMIFormat->str);
            uiVideoMode = pHDMIFormat->VideoID;
        }
    }
    else
    {
        DBG_ERR("No EDID, is DVI?\r\n");
    }

    hdmitx_close();

    return uiVideoMode;
}

UINT32 DrvHDMIGetcaps(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID & 0x0000ffff)
    {
    case DISPLAY_CAPS_BASE:
        //DBG_IND("get HDMI plug caps\r\n");
        v = DISPLAY_BF_BACKLIGHT|DISPLAY_BF_DETPLUG;
        break;
    case DISPLAY_CAPS_PLUG:
        //DBG_IND("det HDMI plug status\r\n");
        v = GPIOMap_DetHDMI();
        break;
    case DISPLAY_CAPS_DEFMODE:
        //v = DISP_HDMIMODE_1920X1080I60;
        v = HDMI_FORMAT_DEFAULT;
        break;
    case DISPLAY_CAPS_MODEINFO:
        {
            int mode = (CapID & 0xffff0000) >>16;
            ISIZE* pModeInfo = (ISIZE*)Param1;
            _DrvHDMIHookDout(g_HDMIDout);
            pDev->SEL.SET_HDMIMODE.VideoID = mode;
            pDev->SEL.SET_HDMIMODE.AudioID = HDMI_AUDIO_NO_CHANGE;
            pObj->devCtrl(DISPDEV_SET_HDMIMODE, pDev);
            pDev->SEL.GET_PREDISPSIZE.DevID = DISPDEV_ID_TVHDMI;
            pObj->devCtrl(DISPDEV_GET_PREDISPSIZE,pDev);
            if(pModeInfo)
            {
            pModeInfo->w = pDev->SEL.GET_PREDISPSIZE.uiBufWidth;
            pModeInfo->h = pDev->SEL.GET_PREDISPSIZE.uiBufHeight;
            }
        }
        break;
    case DISPLAY_CAPS_VIDMODE:
        v = DrvHDMIGetVideoMode();
        break;
    default:
        break;
    }
    return v;
}

UINT32 DrvHDMISetcfgs(UINT32 CfgID, UINT32 Param1) // Set Config Setting
{
    switch(CfgID)
    {
    case DISPLAY_CFG_MODE:
        g_HDMILastMode = Param1;
        break;
    case DISPLAY_CFG_DOUT:
        g_HDMIDout = Param1;
        break;
    case DISPLAY_CFG_INFOBUF:
        pCurrentSize = (USIZE*)Param1;
        break;
    default:
        break;
    }
    return DX_OK;
}

UINT32 DrvHDMIInit(void* pInitParam) // Set Init Parameters
{
    DBG_FUNC_BEGIN("\r\n");

    //g_HDMILastMode = DrvHDMIGetcaps(DISPLAY_CAPS_DEFMODE, 0); // marked; change to get video ability of HDMI device

    return DX_OK;
}

UINT32 DrvHDMIOpen(void) // Common Constructor
{
    DBG_FUNC_BEGIN("\r\n");

    _DrvHDMIHookDout(g_HDMIDout);
    DBG_IND("HDMI dout by IDE%d\r\n", g_HDMIDout+1);
    ////////////////////////////////////////////////////
    #if (_FPGA_EMULATION_ == ENABLE)
    pll_setPLLEn(PLL_ID_2, ENABLE);

    //#Temp Solution for FPGA - begin, should be remove in real chip (Klins)
    pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_480);
    //pll_setConfig(PLL_CONFIG_ID_IDE_CLK_SRC,  480);
    //#Temp Solution for FPGA - end
    #else
    // HDMI need PLL2 for precisely CLK
    if(g_HDMIDout == 0)
    {
    pCtrl->SEL.SET_SRCCLK.SrcClk = DISPCTRL_SRCCLK_PLL2;
    pObj->dispCtrl(DISPCTRL_SET_SRCCLK ,pCtrl);
    }
    if(g_HDMIDout == 1)
    {
    pCtrl->SEL.SET_SRCCLK.SrcClk = DISPCTRL_SRCCLK_IDE2_PLL2;
    pObj->dispCtrl(DISPCTRL_SET_SRCCLK ,pCtrl);
    }
    #endif
    ////////////////////////////////////////////////////

    g_HDMILastMode = DrvHDMIGetcaps(DISPLAY_CAPS_VIDMODE, 0); // get HDMI video mode ability

    g_localMode = g_HDMILastMode;
    DBG_IND("mode %08x\r\n", g_localMode);
    DrvHDMI_TurnOn();
    GPIOMap_TurnOffHDMIBacklight();
    g_HDMIEnable = TRUE;

    return DX_OK;
}

UINT32 DrvHDMIClose(void) // Common Destructor
{
    DBG_FUNC_BEGIN("\r\n");

    DrvHDMI_TurnOff();
    GPIOMap_TurnOnHDMIBacklight();
    g_HDMILastMode = g_HDMIMode;
    g_HDMIMode = 0;
    g_HDMIEnable = FALSE;

    return DX_OK;
}

void DrvHDMI_TurnOn(void)
{
    DBG_FUNC_BEGIN("\r\n");
    // Init HDMI object
    if(g_localMode < DISP_HDMIMODE_MAX) //must be HDMI
    {
        pDev->SEL.SET_HDMIMODE.VideoID    = g_localMode;
        pDev->SEL.SET_HDMIMODE.AudioID    = HDMI_AUDIO32KHZ;
        pObj->devCtrl(DISPDEV_SET_HDMIMODE, pDev);
        /*
        pDev->SEL.GET_PREDISPSIZE.DevID = DISPDEV_ID_TVHDMI;
        pObj->devCtrl(DISPDEV_GET_PREDISPSIZE,pDev);
        g_localSize.w    = pDev->SEL.GET_PREDISPSIZE.uiBufWidth;
        g_localSize.h    = pDev->SEL.GET_PREDISPSIZE.uiBufHeight;
        */
    }
    else
    {
        DBG_ERR("HDMI fmt = %d is incorrect!\r\n", g_localMode);
        return;
    }

    //IMPORTANT! for FAST BOOT, Photo App will ask disp size very early~
    //Update size of first mode (before open)
    //DxDisplay_UpdateModeInfo(g_HDMIDout<<4, g_localMode, &g_localSize);

    pDev->SEL.OPEN_DEVICE.DevID = DISPDEV_ID_TVHDMI;
    pObj->devCtrl(DISPDEV_OPEN_DEVICE,pDev);

    pObj->devCtrl(DISPDEV_GET_DISPSIZE,pDev);
    g_localSize.w = pDev->SEL.GET_DISPSIZE.uiBufWidth;
    g_localSize.h = pDev->SEL.GET_DISPSIZE.uiBufHeight;
    if(pCurrentSize)
        pCurrentSize[0] = g_localSize;

    g_HDMIBacklightEn = TRUE;

#if 0
    // Enable IDE
    //pObj->waitFrmEnd();
    pCtrl->SEL.SET_ENABLE.bEn = TRUE;
    pObj->dispCtrl(DISPCTRL_SET_ENABLE,pCtrl);
    {
    int i;
    // Delay to wait for IDE stable
    for (i=6; i>0; i--)
    {
        //ide_wait_frame_end();
        pObj->waitFrmEnd();
    }
    }
#endif

    g_HDMIMode = g_localMode;
}

void DrvHDMI_TurnOn_WaitFinish(void)
{
    DBG_FUNC_BEGIN("\r\n");

#if _MIPS_TODO
    {
    int i;
    //Delay 16 IDE frames to avoid seeing garbage display
    //Delay to wait for TV DAC stable
    for(i=0;i<16;i++)
    {
        //ide_wait_frame_end();
        pObj->waitFrmEnd();
    }
    }
#endif
    g_HDMIEnable = TRUE;
}


#if _MIPS_TODO
static void delay(UINT32 times)
{
    for(;times > 0; times--)
        ;
}
#endif

void DrvHDMI_TurnOff(void)
{
    DBG_FUNC_BEGIN("\r\n");

#if 1
    pObj->devCtrl(DISPDEV_CLOSE_DEVICE, NULL);
    g_HDMIBacklightEn = FALSE;
#endif

#if 0
    // Disable IDE
    //ide_set_en(FALSE);
    //ide_wait_frame_end();
    //ide_set_en(FALSE);
    pCtrl->SEL.SET_ENABLE.bEn = FALSE;
    pObj->dispCtrl(DISPCTRL_SET_ENABLE,pCtrl);
    pObj->waitFrmEnd();

    // wait for Power Stable
    //delay(10000);
#endif
    g_HDMIEnable = FALSE;
}

void DrvHDMI_SleepEnter(void)
{
    GPIOMap_TurnOffHDMIBacklight();

    // Disable IDE
    //ide_set_en(FALSE);
    pObj->waitFrmEnd();
    pCtrl->SEL.SET_ENABLE.bEn = FALSE;
    pObj->dispCtrl(DISPCTRL_SET_ENABLE,pCtrl);

    g_HDMISleep = TRUE;
}

void DrvHDMI_SleepLeave(void)
{
    // Enable IDE
    //ide_set_en(TRUE);
    pObj->waitFrmEnd();
    pCtrl->SEL.SET_ENABLE.bEn = TRUE;
    pObj->dispCtrl(DISPCTRL_SET_ENABLE,pCtrl);

#if _MIPS_TODO
    {
    int i;
    // Delay 6 IDE frames to avoid seeing garbage display
    for(i=6; i>0; i--)
    {
        //ide_wait_frame_end();
        pObj->waitFrmEnd();
    }
    }
#endif

    GPIOMap_TurnOnHDMIBacklight();
    g_HDMISleep = FALSE;
}

UINT32 DrvHDMIState(UINT32 StateID, UINT32 Value) // General Properties
{
    DBG_FUNC_BEGIN("\r\n");

    if(StateID & DXGET)
    {
    UINT32 rvalue =  0;
    StateID &= ~DXGET;
    switch(StateID)
    {
    case DRVDISP_STATE_ENBALE:
        //lost device? ---> cannot determine enable or not
        //under flow like: enable/disable
        rvalue = g_HDMIEnable;
        break;
    case DRVDISP_STATE_MODE:
        //lost device? ---> cannot determine mode
        //under flow like: change mode
        rvalue = g_HDMIMode;
        break;
    case DRVDISP_STATE_LASTMODE:
        rvalue =  g_HDMILastMode;
        break;
    case DRVDISP_STATE_BACKLIGHT:
        rvalue = g_HDMIBacklightEn;
        break;
    case DRVDISP_STATE_SLEEP:
        rvalue = g_HDMISleep;
        break;
    default:
        DBG_ERR("state=0x%02X not support!\r\n", StateID);
        break;
    }
    return rvalue;
    }
    else if(StateID & DXSET)
    {
    StateID &= ~DXSET;
    switch(StateID)
    {
    case DRVDISP_STATE_BACKLIGHT:
        if(Value == DRVDISP_BACKLIGHT_OFF)
        {
            DBG_IND("DRVDISP_BACKLIGHT_OFF\r\n");
            GPIOMap_TurnOffHDMIBacklight();
        }
        else //if(value == DRVDISP_BACKLIGHT_ON)
        {
            DBG_IND("DRVDISP_BACKLIGHT_ON\r\n");
            GPIOMap_TurnOnHDMIBacklight();
        }
        break;
    case DRVDISP_STATE_SLEEP:
        if(Value == TRUE)
        {
            DBG_IND("DRVDISP_CTRL_SLEEP 1\r\n");
            DrvHDMI_SleepEnter();
        }
        else if(Value == FALSE)
        {
            DBG_IND("DRVDISP_CTRL_SLEEP 0\r\n");
            DrvHDMI_SleepLeave();
        }
        break;
    default:
        DBG_ERR("state=0x%02X not support!\r\n", StateID);
        break;
    }
    }
    return DX_OK;
}

UINT32 DrvHDMIControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
{
    DBG_FUNC_BEGIN("\r\n");

    switch(CtrlID)
    {
    case DRVDISP_CTRL_MODE:
        {
            //DrvHDMI_TurnOff();
            g_localMode = Param1;
            //DrvHDMI_TurnOn();
            //g_bHDMIWaitTurnOnFinish = TRUE;
        }
        break;
    case DRVDISP_CTRL_WAITFINISH:
        if(g_bHDMIWaitTurnOnFinish == TRUE)
        {
            DrvHDMI_TurnOn_WaitFinish();
            g_bHDMIWaitTurnOnFinish = FALSE;
        }
        break;

    default:
        DBG_ERR("ctrlid=0x%02X not support!\r\n", CtrlID);
        break;
    }
    return DX_OK;
}

UINT32 DrvHDMICommand(CHAR *pcCmdStr) //General Command Console
{
    switch (*pcCmdStr)
    {
    case 'd':
        /*if (!strncmp(pcCmdStr, "disp tv dump", 12))
        {
            ...
            return TRUE;
        }*/
        break;
#if (USE_VIO == ENABLE)
    case 'v':
        if (!strncmp(pcCmdStr, "vdet", 4))
        {
            char ts[10];
            UINT32 v1;
            sscanf(pcCmdStr,"%s %d", ts, &v1);
            DBG_IND("v1 = %d\r\n", v1);
            vio_setpin(VIO_HDMI_DET, v1);
            return TRUE;
        }
        break;
#endif
    }
    return FALSE;
}


/**
  Turn on HDMI backlight

  Turn on HDMI backlight.

  @param void
  @return void
*/
void GPIOMap_TurnOnHDMIBacklight(void)
{
    DBG_IND("GPIOMap_TurnOnHDMIBacklight\r\n");

    if (g_HDMIBacklightEn == FALSE)
    {
        // HDMI AV ON
        pDev->SEL.SET_POWERDOWN.bPowerDown = 0x00; //FALSE = power on
        pObj->devCtrl(DISPDEV_SET_POWERDOWN,pDev);
        g_HDMIBacklightEn = TRUE;
    }
}

/**
  Turn off HDMI backlight

  Turn off HDMI backlight.

  @param void
  @return void
*/
void GPIOMap_TurnOffHDMIBacklight(void)
{
    DBG_IND("GPIOMap_TurnOffHDMIBacklight\r\n");

    if (g_HDMIBacklightEn == TRUE)
    {
        // HDMI AV MUTE
        pDev->SEL.SET_POWERDOWN.bPowerDown = 0x01; //TRUE = power off.
        pObj->devCtrl(DISPDEV_SET_POWERDOWN,pDev);
        g_HDMIBacklightEn = FALSE;
    }
}

/**
  Detect HDMI is plugging in or unplugged

  Detect HDMI is plugging in or unplugged.
  Return TRUE if HDMI is plugging in, FALSE if HDMI is unplugged.

  @param void
  @return BOOL: TRUE -> HDMI is plugging in, FALSE -> HDMI is unplugged
*/
BOOL GPIOMap_DetHDMI(void)
{
#if (USE_VIO == ENABLE)
    return (vio_getpin(VIO_HDMI_DET) != 0);
#else
    return hdmitx_checkHotplug();
#endif
}

//@}
