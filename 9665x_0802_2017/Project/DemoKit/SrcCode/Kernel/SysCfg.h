/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       SysCfg.h
    @ingroup

    @brief      Header file of SysCfg.c
                Config the task, event, semaphore, memory pool, memory layout, mailbox

    @note       Nothing.
*/

#ifndef _SYSCFG_H
#define _SYSCFG_H

#include "Type.h"
#include "SysKer.h"
#include "PrjCfg.h"

//===========================================================================

typedef enum
{
    CODE_SECTION_01 = 0,
    CODE_SECTION_02,
    CODE_SECTION_03,
    CODE_SECTION_04,
    CODE_SECTION_05,
    CODE_SECTION_06,
    CODE_SECTION_07,
    CODE_SECTION_08,
    CODE_SECTION_09,
    CODE_SECTION_10,
    CODE_SECTION_11,
    CODE_SECTION_12,
    CODE_SECTION_13,
    CODE_SECTION_14,
    CODE_SECTION_15,
    CODE_SECTION_16,
    ENUM_DUMMY4WORD(_CODE_SECTION_)
}
CODE_SECTION;

//===========================================================================

// Total number of kernel ID sources
#define MAX_TASK_COUNT              128 // 64
#define MAX_FLAG_COUNT              128
#define MAX_SEMAPHORE_COUNT         (128 + 8)
#define MAX_MAILBOX_COUNT           12
#define MAX_MEMPOOL_COUNT           32
#define BUF_NUM                     255


//===========================================================================
extern void Install_Drv(void);

//===========================================================================
#define VDO_W   DISPLAY_VDO_W
#define VDO_H   DISPLAY_VDO_H
#define OSD_W   DISPLAY_OSD_W
#define OSD_H   DISPLAY_OSD_H

#define FB_W    VDO_W
#define FB_H    VDO_H
#define WIN_W   OSD_W
#define WIN_H   OSD_H

#define FHD_W   1920  //Full-HD buffer width: for H264 decoder, it must align to 16
#define FHD_H   1088  //Full-HD buffer height: for H264 decoder, it must align to 16

//===========================================================================
// User defined Task IDs
//===========================================================================

//===========================================================================
// User defined Semaphore IDs
//===========================================================================

//===========================================================================
//  User defined Mailbox IDs
//===========================================================================
extern UINT32 MBXID_SYS_EVENT;

//===========================================================================
//  User defined Mempool IDs
//===========================================================================
extern UINT32 POOL_ID_STORAGE_SDIO;
#if(COPYCARD2CARD_FUNCTION == ENABLE)
extern UINT32 POOL_ID_STORAGE_SDIO2;
#endif
extern UINT32 POOL_ID_STORAGE_NAND;
extern UINT32 POOL_ID_STORAGE_RAMDISK;
extern UINT32 POOL_ID_FWS_BUFFER;
extern UINT32 POOL_ID_FS_BUFFER;
extern UINT32 POOL_ID_PS_BUFFER;
extern UINT32 POOL_ID_GFX_IMAGE;
extern UINT32 POOL_ID_GFX_STRING;
extern UINT32 POOL_ID_GFX_TEMP;
extern UINT32 POOL_ID_EXIF;
extern UINT32 POOL_ID_DATEIMPRINT;
extern UINT32 POOL_ID_FILEDB;
extern UINT32 POOL_ID_HTTPLVIEW;
extern UINT32 POOL_ID_DISP_OSD2;
extern UINT32 POOL_ID_DISP_OSD2TEMP;
extern UINT32 POOL_ID_DISP_OSD1;
extern UINT32 POOL_ID_DISP_OSD1TEMP;
extern UINT32 POOL_ID_DISP_VDO2;
extern UINT32 POOL_ID_DISP_VDO2TEMP;
extern UINT32 POOL_ID_DISP_VDO1;
extern UINT32 POOL_ID_DISP_VDO1TEMP;
extern UINT32 POOL_ID_DISP_VDO1TEMP2;
extern UINT32 POOL_ID_PIP_FLIP;
extern UINT32 POOL_ID_APP;
extern UINT32 POOL_ID_CAL;

//_DEMO_TODO need porting - begin
#define POOL_ID_WAV              0
#define POOL_ID_PBRSV            0
#define POOL_ID_SIDC             0
#define POOL_ID_CACHE            0
#define POOL_ID_CACHE_LANGUAGE   0
//_DEMO_TODO need porting - end

#define POOL_CNT_STORAGE_NAND    1
#define POOL_CNT_STORAGE_RAMDISK 1
#define POOL_CNT_STORAGE_SDIO    1
#define POOL_CNT_FWS_BUFFER      1
#define POOL_CNT_FS_BUFFER       1
#define POOL_CNT_PS_BUFFER       1
#define POOL_CNT_GFX_IMAGE       1
#define POOL_CNT_GFX_STRING      1
#define POOL_CNT_GFX_TEMP        1
#if (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
#define POOL_CNT_EXIF            2
#else
#define POOL_CNT_EXIF            1
#endif
#define POOL_CNT_DATEIMPRINT     1
#define POOL_CNT_FILEDB          1
#define POOL_CNT_HTTPLVIEW       1
#define POOL_CNT_DISP_OSD2       1
#define POOL_CNT_DISP_OSD2TEMP   1
#define POOL_CNT_DISP_OSD1       1
#define POOL_CNT_DISP_OSD1TEMP   1
#define POOL_CNT_DISP_VDO2       1
#define POOL_CNT_DISP_VDO2TEMP   1
#define POOL_CNT_DISP_VDO1       1
#define POOL_CNT_DISP_VDO1TEMP   1
#define POOL_CNT_PIP_FLIP        1
#define POOL_CNT_APP             1
#define POOL_CNT_CAL             1

//#NT#2014/07/16#Calvin Chang#Support DRAM Tiny Mode -begin
#if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
#define POOL_SIZE_STORAGE_NAND     ALIGN_CEIL_32(0x20)       // Min Memory Pool size is 32 Bytes
#define POOL_SIZE_STORAGE_RAMDISK  ALIGN_CEIL_32(0x10000)     // 64K bytes for RAM Disk label setting.
#else
#define POOL_SIZE_STORAGE_NAND     ALIGN_CEIL_32(0x20000)    // 128K bytes
#define POOL_SIZE_STORAGE_RAMDISK  ALIGN_CEIL_32(0x20)       // Min Memory Pool size is 32 Bytes
#endif
#define POOL_SIZE_STORAGE_SDIO  ALIGN_CEIL_32(512)
//#NT#2014/07/16#Calvin Chang -end
/**
     POOL_SIZE_FWS_BUFFER

     For Partial Loading, In two case:
     1. un-compressed bin: set POOL_SIZE_FWS_BUFFER to 1 sector size (use STRG_GET_SECTOR_SIZE to get size)
     2. compressed bin: better to set it to (binsize + 2*sector_size)

*/
#define POOL_SIZE_FWS_BUFFER    ALIGN_CEIL_32(128*1024)         // 128K == 1 sector size (for 2K Page)
#define POOL_SIZE_FS_BUFFER     (ALIGN_CEIL_32(0xA2000)+ALIGN_CEIL_32(0x80020))
                                                                // R/W buf = 0xA2000 (Sectbuf1=128K,SectBuf2=128k, ScanBuf = 128k, BitMap=256k,UpCase=8k, Total 648k = 0xA2000)
                                                                // FAT buf = 0x80020 (FAT buff = 512k + 32 bytes reserved = 0x80020)#define POOL_SIZE_PS_BUFFER     (ALIGN_CEIL_32(0x20000)+ALIGN_CEIL_32(0x20000))
#if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
#define POOL_SIZE_PS_BUFFER     (ALIGN_CEIL_32(0x1000)+ALIGN_CEIL_32(0x3604)) // 10K bytes = 1 block size + about 6K work
#else
#define POOL_SIZE_PS_BUFFER     (ALIGN_CEIL_32(0x20000)+ALIGN_CEIL_32(0x1800)) // 134K bytes = 1 block size + about 6K work
#endif
#if (DISPLAY_OSD_BPP == 24)
#define POOL_SIZE_DISP_OSD1     ALIGN_CEIL_32(OSD_W*OSD_H*3)     // OSD -> 320*240 pixel, 24 bits for one pixel
#elif (DISPLAY_OSD_BPP == 8)
#define POOL_SIZE_DISP_OSD1     ALIGN_CEIL_32(OSD_W*OSD_H)       // OSD -> 320*240 pixel, 8 bits for one pixel
#else
#define POOL_SIZE_DISP_OSD1     ALIGN_CEIL_32(OSD_W*OSD_H/2)     // OSD -> 320*240 pixel, 4 bits for one pixel
#endif
#define POOL_SIZE_DISP_OSD2     POOL_SIZE_DISP_OSD1

#define POOL_SIZE_DISP_VDO1     ALIGN_CEIL_32((VDO_W*VDO_H*2)*2) //VDO_W*VDO_H*2 for YCbCr, *2 for double buffer (1st-view on LCD)

#define POOL_SIZE_DISP_VDO1TEMP ALIGN_CEIL_32((FHD_W*FHD_H*2)*2) //FHD_W*FHD_H*2 for YCbCr, *2 for double buffer (2nd-view on HDMI)

#define POOL_SIZE_DISP_VDO2     ALIGN_CEIL_32((VDO_W*VDO_H*2))

#define POOL_SIZE_GFX_TEMP      ALIGN_CEIL_32(0x1000)//ALIGN_CEIL_32(POOL_SIZE_DISP_VDO2+(0x1000)) //at least 4k for Gxlib

#if (_LANG_STORE_ == _PSTORE_)
#define POOL_SIZE_GFX_STRING    ALIGN_CEIL_32(0x8000)
#else
#define POOL_SIZE_GFX_STRING    ALIGN_CEIL_32(0x20)                // Min Memory Pool size is 32 Bytes
#endif

#if (_JPG_STORE_ == _PSTORE_)
#define POOL_SIZE_GFX_IMAGE     ALIGN_CEIL_32(0x20000)
#else
#define POOL_SIZE_GFX_IMAGE     ALIGN_CEIL_32(0x00000020)
#endif

#define POOL_SIZE_EXIF          ALIGN_CEIL_32(0x10000)

//#define POOL_SIZE_DATEIMPRINT   ALIGN_CEIL_32(0x200000)
#define POOL_SIZE_DATEIMPRINT   ALIGN_CEIL_32(0x80000)              // Reduce data stamp buffer size to 512K

#if USE_FILEDB
#define POOL_SIZE_FILEDB        ALIGN_CEIL_32(0x130000)
#else
#define POOL_SIZE_FILEDB        ALIGN_CEIL_32(0x20)
#endif

#if HTTP_LIVEVIEW_FUNC
#define POOL_SIZE_HTTPLVIEW     ALIGN_CEIL_32(0x200000)
#else
#define POOL_SIZE_HTTPLVIEW     ALIGN_CEIL_32(0x20)
#endif

#if (PIP_1ST_PATH_FLIP || PIP_2ND_PATH_FLIP)
#define POOL_SIZE_PIP_FLIP      ALIGN_CEIL_32(0x100000)
#else
#define POOL_SIZE_PIP_FLIP      ALIGN_CEIL_32(0x20)
#endif

//_DEMO_TODO need porting - begin
#define POOL_SIZE_WAV           0
#define POOL_SIZE_PBRSV         0
#define POOL_SIZE_SIDC          0
#define POOL_SIZE_CACHE_LANGUAGE    0
#define POOL_CACHE_OFFSET_LANGUAGE  0
//_DEMO_TODO need porting - end

#if (_CALIBRATION_MODE_ == ENABLE)
#define POOL_SIZE_CAL           ALIGN_CEIL_32(0x80000) ///< IQ says it needs 512KB CAL buffer for C12 project
#else
#define POOL_SIZE_CAL           ALIGN_CEIL_32(0) ///< here size needs to ask IQ how many should be used
#endif

#define POOL_SIZE_APP           ((UINT32)OS_GetMempoolSize(POOL_ID_APP)) //for query pool size of POOL_ID_APP after config

#define POOL_SIZE_FREE          ((UINT32)OS_GetMemFreeSize(MEM_HEAP)) //query current free heap memory size to config pool size of POOL_ID_APP


#endif //_SYSCFG_H

