/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       ModuleInstall.c
    @ingroup

    @brief      Install Task,Sem,Flag for module.
                Install Task,Sem,Flag for module.

    @note       Nothing.
*/

#include <stdio.h>
#include <stdlib.h>
#include "Type.h"
#include "SysCfg.h"
#include "EXTERNS.h"
#include "SysKer.h"

//===========================================================================
#include "DxSys.h"

//System Service
#include "SxCmd.h"
#include "GxSystem.h"
#include "DbgUtApi.h"
#include "Utility.h"

//Device Service
#include "FwSrvApi.h"
#include "PStore.h"
#include "FileSysTsk.h"
#include "GxTimer.h"
#include "DispSrvApi.h"

//Lib
#include "DCF.h"
#include "DPOF.h"
#include "Exif.h"
#include "FileDB.h"
#include "GxSound.h"
#include "UMSDC.h"
#include "UVAC.h"
#include "USBSIDC2.h"
#include "Audio.h"
#include "WavStudioTsk.h"
#include "AudFilterAPI.h"
#include "Lens.h"
#include "LensCtrlTsk.h"
#include "GxFlash.h"
#include "imageeffect.h"
#include "VideoCodec_h264.h"
#if HTTP_LIVEVIEW_FUNC
#include "LViewNvtAPI.h"
#endif
//#NT#2016/05/03#Lincy Lin -begin
//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
#include "LogFile.h"
#endif
//#NT#2016/05/03#Lincy Lin -end

//App Playback
#include "PlaybackTsk.h"

//App Photo
#include "ae_task.h"
#include "awb_task.h"
#include "af_task.h"
#include "ipl_Cmd.h"
#include "ImgCaptureAPI.h"
#include "Sensor.h"
#include "phototask.h"

//App Usb
#include "MsdcNvtApi.h"

//App Media
//#include "MediaRecAPI.h"
#include "MovieInterface.h"
#include "MediaPlayAPI.h"
#include "SMediaPlayAPI.h"
#if((RTSP_LIVEVIEW_FUNC==ENABLE)||(WIFI_AP_FUNC==ENABLE))
#include "RtspNvtApi.h"
#endif

//User
#include "NvtSystem.h"
#include "UIControl.h"
#include "NvtUser.h"
#include "NvtBack.h"
#include "UserCmd.h"

// Engineer mode
#include "EngineerMode.h"

#include "UIGraphics.h"
#include "spiflash.h"
#include "IPL_Display_Id.h"
#include "IPL_Display_Id2.h"
#if (GPS_FUNCTION == ENABLE)
#include "GPS.h"
#endif
#if(WIFI_AP_FUNC==ENABLE)
#include "WifiAppCmd.h"
#endif
//===========================================================================
//static OS resource (Config Task,Flag,Semaphore & MemPool)

DEFINE_OS_RESOURCE
(
    MAX_TASK_COUNT,
    MAX_SEMAPHORE_COUNT,
    MAX_FLAG_COUNT,
    BUF_NUM,
    MAX_MAILBOX_COUNT,
    MAX_MEMPOOL_COUNT
);

//===========================================================================
//Install user OS resource (Config Task,Flag,Semaphore & MemPool)
//  Note: stack is config by Task, heap is config by MemPool

//for sdio driver
UINT32 POOL_ID_STORAGE_SDIO = 0;
#if(COPYCARD2CARD_FUNCTION == ENABLE)
UINT32 POOL_ID_STORAGE_SDIO2 = 0;
#endif
//for nand driver
UINT32 POOL_ID_STORAGE_NAND = 0;
//#NT#2014/07/16#Calvin Chang#Support DRAM Tiny Mode -begin
UINT32 POOL_ID_STORAGE_RAMDISK = 0;
//#NT#2014/07/16#Calvin Chang -end
//for FWStore
UINT32 POOL_ID_FWS_BUFFER = 0;
//for FileSys
UINT32 POOL_ID_FS_BUFFER = 0;
//for PStore
UINT32 POOL_ID_PS_BUFFER = 0;
//for Gfx
UINT32 POOL_ID_GFX_IMAGE = 0;
UINT32 POOL_ID_GFX_STRING = 0;
UINT32 POOL_ID_GFX_TEMP = 0;
//for Exif
UINT32 POOL_ID_EXIF = 0;
//for Date Imprint
UINT32 POOL_ID_DATEIMPRINT = 0;
//for fileDB
UINT32 POOL_ID_FILEDB = 0;
//for Display
UINT32 POOL_ID_DISP_OSD2 = 0;
UINT32 POOL_ID_DISP_OSD2TEMP = 0;
UINT32 POOL_ID_DISP_OSD1 = 0;
UINT32 POOL_ID_DISP_OSD1TEMP = 0;
UINT32 POOL_ID_DISP_VDO2 = 0;
UINT32 POOL_ID_DISP_VDO2TEMP = 0;
UINT32 POOL_ID_DISP_VDO1 = 0;
UINT32 POOL_ID_DISP_VDO1TEMP = 0;
UINT32 POOL_ID_DISP_VDO1TEMP2 = 0;
// for HTTP liveview
UINT32 POOL_ID_HTTPLVIEW = 0;
//for calibration
UINT32 POOL_ID_CAL = 0;
//free
UINT32 POOL_ID_APP = 0;

UINT32 POOL_ID_PIP_FLIP = 0;

extern void Install_DevStrgMEM(void);
extern void Install_DevDispMEM(void);

void Install_User(void)
{
//對應partial-load的配套措施:
//NOTE 1: Install ID之前, 只確定ZI已經clear ok, part-1已經load ok, 其餘code section可能都尚未ready
//NOTE 2: Install ID過程, 所有的function code都必須位於part-1, 所以prototype請加上_SECTION(".kercfg_text")
//NOTE 3: Install ID過程, 所有的global variable都必須位於part-1或ZI, 所以prototype請加上_SECTION(".kercfg_data")
//NOTE 4: Install ID過程, 使用的stack是特別保留給kernel的stack, 其size只有1K, 請勿呼叫到太多層API
//NOTE 5: Install ID過程, kernel尚未啟動, 所以除了kini_xxx()的initial API之外, 請勿呼叫到任何kernel API
//NOTE 6: Install ID過程, drv與i/o尚未完全初始化完畢, 所以除了debug msg之外, 請勿用到任何driver API或控制任何i/o


//System Service
    SwTimer_InstallID();
    SxCmd_InstallID();
    GxSystem_InstallID();

//Device Ext
    spiflash_InstallID();//SPI FLASH

//Device Service
    DbgUt_InstallID(); //CPU,DMA
    //Timer
    GxTimer_InstallID(); //TIMER
    //Storage
    FwSrv_InstallID(); //NAND
    FileSys_InstallID(); //CARD,NAND
    PStore_InstallID(); //NAND
    //Display
    DispSrv_InstallID(); //DISPLAY

//Lib
    DCF_InstallID();
    FileDB_InstallID();    
    DPOF_InstallID();
    Exif_InstallID();
    Msdc_InstallID();
    UVAC_InstallID();
    #if HTTP_LIVEVIEW_FUNC
    LviewNvt_InstallID();
    #endif
    //Sidc_InstallID();
    WavStudio_InstallID();
    GxSound_InstallID();
    AudFilter_InstallID();
    #if (LENS_FUNCTION == ENABLE)
    LensCtrl_InstallID();
    LensCtrl2_InstallID();
    #endif
    GxFlash_InstallID();
    IE_InstallID();
    MP_H264Enc_InstallID();

//#NT#2016/05/03#Lincy Lin -begin
//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
    LogFile_InstallID();
#endif
//#NT#2016/05/03#Lincy Lin -end

//App Playback
    PB_InstallID();

//App Photo
    Sensor_InstallID();
    IPL_InstallID();
    IPL_Display_InstallID();
    IPL_Display_InstallID2();
    Cap_InstallID();
    Photo_InstallID();
    AE_InstallID();
    AF_InstallID();
    AWB_InstallID();

//App Usb
    MsdcNvt_InstallID();

//App Media
    MovRec_InstallID();
    MediaPlay_InstallID();
    SMediaPlay_InstallID();
#if((RTSP_LIVEVIEW_FUNC==ENABLE)||(WIFI_AP_FUNC==ENABLE))
    RtspNvt_InstallID();
#endif

//User
    #if (GPS_FUNCTION == ENABLE)
    GPS_InstallID();
    #endif
    NvtSystem_InstallID();
    NvtUser_InstallID();
    NvtBack_InstallID();
    UIControl_InstallID();
#if (_CALIBRATION_MODE_ == ENABLE)
    Engineer_InstallID();
#endif
    UI_InstallID();
    UserCmd_InstallID();
#if(WIFI_AP_FUNC==ENABLE)
    WifiCmd_InstallID();
#endif
}

void Install_DevStrgMEM(void)
{
//for nand driver
    OS_CONFIG_MEMPOOL(POOL_ID_STORAGE_NAND,  POOL_SIZE_STORAGE_NAND,    POOL_CNT_STORAGE_NAND);      // nand driver working buffer
//for sdio driver
    OS_CONFIG_MEMPOOL(POOL_ID_STORAGE_SDIO,  POOL_SIZE_STORAGE_SDIO,    POOL_CNT_STORAGE_SDIO);      // sdio driver working buffer
#if(COPYCARD2CARD_FUNCTION == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_STORAGE_SDIO2, POOL_SIZE_STORAGE_SDIO,    POOL_CNT_STORAGE_SDIO);      // sdio2 driver working buffer
#endif
//for FWStore
    OS_CONFIG_MEMPOOL(POOL_ID_FWS_BUFFER,    POOL_SIZE_FWS_BUFFER,      POOL_CNT_FWS_BUFFER);        // FWStore working buffer
//for FileSys
    OS_CONFIG_MEMPOOL(POOL_ID_FS_BUFFER,     POOL_SIZE_FS_BUFFER,       POOL_CNT_FS_BUFFER);         // FileSys working buffer
//for PStore
    OS_CONFIG_MEMPOOL(POOL_ID_PS_BUFFER,     POOL_SIZE_PS_BUFFER,       POOL_CNT_PS_BUFFER);         // PStore working buffer
//for RAM DISK
    //#NT#2014/07/16#Calvin Chang#Support DRAM Tiny Mode -begin
    OS_CONFIG_MEMPOOL(POOL_ID_STORAGE_RAMDISK,  POOL_SIZE_STORAGE_RAMDISK,    POOL_CNT_STORAGE_RAMDISK);      // ram disk working buffer
    //#NT#2014/07/16#Calvin Chang -end
}

void Install_DevDispMEM(void)
{
//for Display
    UINT32 osd2_size = 0;
    UINT32 osd1_size = 0;
    UINT32 vdo2_size = 0;
    UINT32 vdo1_size_1 = 0x10;
    UINT32 vdo1_size_2 = 0x10;
    #if(VDO_USE_ROTATE_BUFFER==ENABLE)
    UINT32 vdo1_size_r = 0x10;
    #endif

    //UI - manage by UIDisplay, buffer is fixed memory after OS start
    osd2_size =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(OSD_W,OSD_H,TYPE_FB,PXLFMT_INDEX8));

    //UI - manage by UIDisplay, buffer is fixed memory after OS start
    osd1_size =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(OSD_W,OSD_H,TYPE_FB,PXLFMT_INDEX8));

    //UI - manage by UIDisplay, buffer is fixed memory after OS start
    vdo2_size =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(VDO_W,VDO_H,TYPE_FB,PXLFMT_YUV422_PK));

    //App - manage by DispSrv, buffer is fixed memory after OS start
#if (VDO_BUFFER_SIZE_OPTIMIZATION == ENABLE)

    /*
    NOTE:

    Total required VDO1 buffer size is optimized by config following settings in PrjCfg_XXXX.h,
    User need to review these settings to get optimized buffer size.

    1. device
      - LCD2_FUNC
      - TV_FUNC
      - HDMI_FUNC
      - DUALDISP_FUNC (for dual device LCD+LCD2, or LCD+TV, or LCD+HDMI)

    2. device max buffer size
      - LCD_MAX_W and LCD_MAX_H
      - LCD2_MAX_W and LCD2_MAX_H
      - TV_MAX_W and TV_MAX_H
      - HDMI_MAX_W and HDMI_MAX_H

    3. device need to rotate
      - VDO_USE_ROTATE_BUFFER (for LCD device)

    Total required VDO1 buffer size =
      for single display : max_size(LCD,TV,HDMI) = maximum buffer area of LCD/TV/HDMI
        buffer size = VDO1_MAX_W*VDO1_MAX_H buffer x2
      for dual display : LCD + max_size(LCD,TV,HDMI)
        buffer size = LCD_MAX_W*LCD_MAX_H buffer x2 + VDO1_MAX_W*VDO1_MAX_H buffer x2
      for LCD device rotate:
        buffer size += LCD_MAX_W*LCD_MAX_H buffer x1

    */
    {
    UINT32 vdo1_lcd_w = 0;
    UINT32 vdo1_lcd_h = 0;
    UINT32 vdo1_max_w = 0;
    UINT32 vdo1_max_h = 0;
    vdo1_lcd_w = LCD_MAX_W;
    vdo1_lcd_h = LCD_MAX_H;
    vdo1_max_w = vdo1_lcd_w;
    vdo1_max_h = vdo1_lcd_h;
    #if (_LCD2TYPE_ != _LCDTYPE_OFF_)
    if((vdo1_max_w*vdo1_max_h) < (LCD2_MAX_W*LCD2_MAX_H))
    {
        vdo1_max_w = LCD2_MAX_W;
        vdo1_max_h = LCD2_MAX_H;
    }
    #endif
    #if(TV_FUNC==ENABLE)
    if((vdo1_max_w*vdo1_max_h) < (TV_MAX_W*TV_MAX_H))
    {
        vdo1_max_w = TV_MAX_W;
        vdo1_max_h = TV_MAX_H;
    }
    #endif
    #if(HDMI_FUNC==ENABLE)
    if((vdo1_max_w*vdo1_max_h) < (HDMI_MAX_W*HDMI_MAX_H))
    {
        vdo1_max_w = HDMI_MAX_W;
        vdo1_max_h = HDMI_MAX_H;
    }
    #endif
    #if(DUALDISP_FUNC==DUALDISP_ONBOTH)
    vdo1_size_1 =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(vdo1_lcd_w,vdo1_lcd_h,TYPE_FB,PXLFMT_YUV422_PK))*2;
    vdo1_size_2 =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(vdo1_max_w,vdo1_max_h,TYPE_FB,PXLFMT_YUV422_PK))*2;
    #else
    vdo1_size_1 =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(vdo1_max_w,vdo1_max_h,TYPE_FB,PXLFMT_YUV422_PK))*2;
    #endif
    #if(VDO_USE_ROTATE_BUFFER==ENABLE)
    vdo1_size_r =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(vdo1_lcd_w,vdo1_lcd_h,TYPE_FB,PXLFMT_YUV422_PK));
    #endif
    }
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD2,     osd2_size,       POOL_CNT_DISP_OSD2);     // Display OSD2 buffer
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD2TEMP, osd2_size,       POOL_CNT_DISP_OSD2TEMP);     // Display OSD2 buffer 2
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD1,     osd1_size,       POOL_CNT_DISP_OSD1);     // Display OSD1 buffer
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD1TEMP, osd1_size,       POOL_CNT_DISP_OSD1TEMP);     // Display OSD1 buffer 2
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO2,     vdo2_size,       POOL_CNT_DISP_VDO2);     // Display VDO2 jpg buffer
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO2TEMP, vdo2_size,       POOL_CNT_DISP_VDO2TEMP);     // Display VDO2 jpg buffer 2
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO1,     vdo1_size_1,     POOL_CNT_DISP_VDO1);      // App: VDO1 frame buffer
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO1TEMP, vdo1_size_2,     POOL_CNT_DISP_VDO1TEMP);  // App: VDO1 frame buffer 2
    #if (VDO_USE_ROTATE_BUFFER == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO1TEMP2, vdo1_size_r,    POOL_CNT_DISP_VDO1TEMP);  // App: VDO1 frame buffer 3
    #endif

#else

    //Support dual display LCD+HDMI case
    vdo1_size_1 =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(VDO_W,VDO_H,TYPE_FB,PXLFMT_YUV422_PK))*2;  //1st device (LCD)
    vdo1_size_2 =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(FHD_W,FHD_H,TYPE_FB,PXLFMT_YUV422_PK))*2;  //2nd device (HDMI)
    #if (VDO_USE_ROTATE_BUFFER == ENABLE)
    vdo1_size_r =
        vdo1_size_1;
    #endif
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD2,     osd2_size,       POOL_CNT_DISP_OSD2);     // Display OSD2 buffer
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD2TEMP, osd2_size,       POOL_CNT_DISP_OSD2TEMP);     // Display OSD2 buffer 2
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD1,     osd1_size,       POOL_CNT_DISP_OSD1);     // Display OSD1 buffer
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD1TEMP, osd1_size,       POOL_CNT_DISP_OSD1TEMP);     // Display OSD1 buffer 2
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO2,     vdo2_size,       POOL_CNT_DISP_VDO2);     // Display VDO2 jpg buffer
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO2TEMP, vdo2_size,       POOL_CNT_DISP_VDO2TEMP);     // Display VDO2 jpg buffer 2
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO1,     vdo1_size_1,     POOL_CNT_DISP_VDO1);      // App: VDO1 frame buffer
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO1TEMP, vdo1_size_2,     POOL_CNT_DISP_VDO1TEMP);  // App: VDO1 frame buffer 2
    #if (VDO_USE_ROTATE_BUFFER == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO1TEMP2, vdo1_size_r,    POOL_CNT_DISP_VDO1TEMP);  // App: VDO1 frame buffer 3
    #endif

    #endif
}

void Install_UserMEM(void)
{
//Device Driver
    //Storage
    Install_DevStrgMEM();
    //Display
    Install_DevDispMEM();

//for Gfx
    OS_CONFIG_MEMPOOL(POOL_ID_GFX_IMAGE,     POOL_SIZE_GFX_IMAGE,       POOL_CNT_GFX_IMAGE);         // Gfx jpeg image resource buffer
    OS_CONFIG_MEMPOOL(POOL_ID_GFX_STRING,    POOL_SIZE_GFX_STRING,      POOL_CNT_GFX_STRING);        // Gfx language resource buffer
    OS_CONFIG_MEMPOOL(POOL_ID_GFX_TEMP,      POOL_SIZE_GFX_TEMP,        POOL_CNT_GFX_TEMP);          // Gfx working + JPG decode buffer
//for Exif
    OS_CONFIG_MEMPOOL(POOL_ID_EXIF,          POOL_SIZE_EXIF,            POOL_CNT_EXIF);              // Exif lib working buffer
//for DateImprint
    OS_CONFIG_MEMPOOL(POOL_ID_DATEIMPRINT,   POOL_SIZE_DATEIMPRINT,     POOL_CNT_DATEIMPRINT);       // DATE IMPRINT working buffer
//for fileDB
    OS_CONFIG_MEMPOOL(POOL_ID_FILEDB,        POOL_SIZE_FILEDB,          POOL_CNT_FILEDB);            // FileDB working buffer
//for PIP flip
    OS_CONFIG_MEMPOOL(POOL_ID_PIP_FLIP,      POOL_SIZE_PIP_FLIP,        POOL_CNT_PIP_FLIP)           // PIP 2nd path flip
//for HTTP liveview
    OS_CONFIG_MEMPOOL(POOL_ID_HTTPLVIEW,     POOL_SIZE_HTTPLVIEW,       POOL_CNT_HTTPLVIEW);         // HTTP liveview working buffer
#if(POOL_SIZE_CAL)
    OS_CONFIG_MEMPOOL(POOL_ID_CAL,           POOL_SIZE_CAL,             POOL_CNT_CAL);               // Calibration data buffer
#endif
//free
    OS_CONFIG_MEMPOOL(POOL_ID_APP,           POOL_SIZE_FREE,            POOL_CNT_APP);               // App: config all free memory for it
}
