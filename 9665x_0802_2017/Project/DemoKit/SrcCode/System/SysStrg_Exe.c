/*
    System Storage Callback

    System Callback for Storage Module.

    @file       SysStrg_Exe.c
    @ingroup    mIPRJSYS

    @note       這個檔案負責一件事

                1.接收Storage Event的介面
                  可能的event有:

                     STRG_INSERT --- 代表有CARD插入
                       執行unmount之前需要做的事情
                       呼叫File unmound (NAND),
                       執行mount之前需要做的事情
                       呼叫File mount (CARD)

                     STRG_REMOVE --- 代表有CARD拔出
                       執行unmount之前需要做的事情
                       呼叫File unmount (CARD)
                       執行mount之前需要做的事情
                       呼叫File_mount (NAND)

                     STRG_ATTACH --- 代表File mount完畢
                       執行mount之後需要做的事情
                       這邊會傳入mount的結果status

                     STRG_DETACH --- 代表File unmount完畢
                       執行unmount之後需要做的事情
                       這邊會傳入unmount的結果status


    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"

#include "DxType.h"
#include "Dx.h"
#include "DxApi.h"

#include "DxStorage.h"
#include "PStore.h"
#include "DCF.h"
#include "GxFlash.h"
#include "ExifVendor.h"

#include "BinInfo.h"
#include "FileSysTsk.h"
#if (UI_STYLE!=UI_STYLE_DRIVER)
#include "CalibrationAPI.h"
#endif
//#NT#2016/05/03#Lincy Lin -begin
//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
#include "LogFile.h"
#endif
//#NT#2016/05/03#Lincy Lin -end

#include "gpio.h"
#include "UIFlow.h"

//#NT#2016/12/26#KCHong -begin
//#NT#Sample code to calculate MD5
#include "md5_util.h"
//#NT#2016/12/26#KCHong -end

#define MAX_BLK_PER_SEC         512
#define MAX_SEC_NUM             64
#define FW_UPD_FW_TMP_MEM_SIZE  0x1000000 //16MB

//#NT#2016/02/17#Niven Cho -begin
//#NT#Mechanism for Fw Updating Fw failed.
#define LOADER_UPD_FW_PATH "A:\\"_BIN_NAME_".BIN"
#define FW_DEL_INDIACTION_PATH "A:\\NVTDELFW"
//#NT#2016/02/17#Niven Cho -end

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysStrgExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define MD5_CHECK_ENABLE		0

extern BININFO gBinInfo;

//#NT#2017/02/17#KCHong -begin
//#NT#Skip NVTEVT_STRG_REMOVE event when firmware updating
static BOOL IsFWUpdating = FALSE;
//#NT#2017/02/17#KCHong -end

#if (MD5_CHECK_ENABLE)
UINT32 FWBufAddr = 0;
UINT32 FWBufSize = 0;
char CurVerInfo[] = FW_VERSION_NUM;
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  INMEM
//
///////////////////////////////////////////////////////////////////////////////
//Drv or DrvExt
DX_HANDLE DxNandDev0 = 0;
DX_HANDLE DxNandDev1 = 0;
DX_HANDLE DxNandDev3 = 0;
DXSTRG_INIT UserNandInit ={0};
#define NAND_PARTITION_NUM   3
#if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
DXSTRG_PARTITION NandPartition[NAND_PARTITION_NUM] =
{
	#if (_MODEL_DSC_ == _MODEL_C12_)
    {DX_CLASS_STORAGE_EXT|DX_TYPE_NAND0,0x00000000,0x00A00000,0}, //FWRSV_SIZE = LOADER+FW
    {DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1,0x00A00000,0,0}, //RAMDSIK
    {DX_CLASS_STORAGE_EXT|DX_TYPE_NAND3,0x00A00000,0x00600000,0}, //PSTORE_SIZE
	#else
    {DX_CLASS_STORAGE_EXT|DX_TYPE_NAND0,0x00000000,0x003c0000,0}, //FWRSV_SIZE = LOADER+FW
    {DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1,0x003c0000,0,0}, //RAMDSIK
    {DX_CLASS_STORAGE_EXT|DX_TYPE_NAND3,0x003c0000,0x00040000,0}, //PSTORE_SIZE
    #endif
};
#else
DXSTRG_PARTITION NandPartition[NAND_PARTITION_NUM]=
{
    {DX_CLASS_STORAGE_EXT|DX_TYPE_NAND0,0x00000000,NAND_FWRSV_SIZE,0},           //FWS
    {DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1,NAND_FWRSV_SIZE,NAND_FAT_SIZE,0x200000}, //FS
    {DX_CLASS_STORAGE_EXT|DX_TYPE_NAND3,NAND_FWRSV_SIZE+NAND_FAT_SIZE,PSTORE_SIZE,0x80000}, //PST
};
#endif


void System_OnStrgInit_INMEM(void)
{
    static BOOL bStrg_init_INMEM = FALSE;
    UINT32 i=0;
    if(bStrg_init_INMEM)
        return;

    //check partition overlap
    //part 0(FW area) must match loader reserved size,and next area can not overlap previos area
    for(i=0;i<NAND_PARTITION_NUM;i++)
    {
        if(i==0)
        {
            if(NandPartition[i].uiPhySize<gBinInfo.ld.FWResvSize)
            {
               DBG_ERR("part0 %x < FWResvSize %x,cannot init\r\n",NandPartition[i].uiPhySize,gBinInfo.ld.FWResvSize);
               return;
            }
        }
        else
        {
            if(NandPartition[i].uiPhyAddr<NandPartition[i-1].uiPhyAddr+NandPartition[i-1].uiPhySize)
            {
                DBG_ERR("part%d start %x overlap part%d end %x,cannot init\r\n",i,NandPartition[i].uiPhyAddr,i-1,NandPartition[i-1].uiPhyAddr+NandPartition[i-1].uiPhySize);
                return;
            }
        }
    }

    for(i=0;i<NAND_PARTITION_NUM;i++)
    {
        DX_HANDLE DxNandDev = Dx_GetObject(NandPartition[i].uiDxClassType);
        //#NT#2014/07/16#Calvin Chang#Support DRAM Tiny Mode -begin
        #if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
        UserNandInit.buf.Addr = OS_GetMempoolAddr(POOL_ID_STORAGE_RAMDISK);
        UserNandInit.buf.Size = OS_GetMempoolSize(POOL_ID_STORAGE_RAMDISK);
        #else
        UserNandInit.buf.Addr = OS_GetMempoolAddr(POOL_ID_STORAGE_NAND);
        UserNandInit.buf.Size = OS_GetMempoolSize(POOL_ID_STORAGE_NAND);
        #endif
        UserNandInit.prt.uiDxClassType = NandPartition[i].uiDxClassType;
        UserNandInit.prt.uiPhyAddr = NandPartition[i].uiPhyAddr;
        UserNandInit.prt.uiPhySize = NandPartition[i].uiPhySize;
        if(i==0)
            UserNandInit.prt.uiResvSize = gBinInfo.ld.LdBlockCount; //FWS use loader block count as reserved blcok
        else
            UserNandInit.prt.uiResvSize = NandPartition[i].uiResvSize;
        Dx_Init(DxNandDev,&UserNandInit,0,STORAGE_VER);
    }

    bStrg_init_INMEM = TRUE;
}
void System_OnStrgExit_INMEM(void)
{
    //PHASE-1 : Close Drv or DrvExt
}

///////////////////////////////////////////////////////////////////////////////
//
//  EXMEM
//
///////////////////////////////////////////////////////////////////////////////
//Drv or DrvExt
DXSTRG_INIT UserSdioInit;
#if(COPYCARD2CARD_FUNCTION == ENABLE)
DXSTRG_INIT UserSdio2Init;
#endif
//extern SX_CMD_ENTRY Cmd_card[]; //device
DX_HANDLE DxCardDev1 = 0;

void System_OnStrgInit_EXMEM(void)
{
    static BOOL bStrg_init_EXMEM = FALSE;
    if(bStrg_init_EXMEM)
        return;
    {
    DX_HANDLE DxCardDev1 = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1);
    UserSdioInit.buf.Addr = OS_GetMempoolAddr(POOL_ID_STORAGE_SDIO);
    UserSdioInit.buf.Size = OS_GetMempoolSize(POOL_ID_STORAGE_SDIO);
    Dx_Init(DxCardDev1,&UserSdioInit,0,STORAGE_VER);
    }
    #if(COPYCARD2CARD_FUNCTION == ENABLE)
    // for card 2
    {
    DX_HANDLE DxCardDev2 = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD2);
    UserSdio2Init.buf.Addr = OS_GetMempoolAddr(POOL_ID_STORAGE_SDIO2);
    UserSdio2Init.buf.Size = OS_GetMempoolSize(POOL_ID_STORAGE_SDIO2);
    Dx_Init(DxCardDev2,&UserSdio2Init,0,STORAGE_VER);
    }
    #endif

    bStrg_init_EXMEM = TRUE;
}

void System_OnStrgExit_EXMEM(void)
{
    //PHASE-1 : Close Drv or DrvExt

}

///////////////////////////////////////////////////////////////////////////////
//
//  FILESYS
//
///////////////////////////////////////////////////////////////////////////////
//Lib or LibExt
extern void Strg_CB(UINT32 event, UINT32 param1, UINT32 param2);
void Card_DetInsert(void);
void Card_DetBusy(void);
#if (SDINSERT_FUNCTION == ENABLE)
SX_TIMER_ITEM(Card_DetInsert, Card_DetInsert, 2, FALSE)
#endif
SX_TIMER_ITEM(System_DetBusy, Card_DetBusy, 25, FALSE)
int SX_TIMER_DET_STRG_ID = -1;
int SX_TIMER_DET_SYSTEM_BUSY_ID = -1;
//#NT#2012/11/14#Philex Lin - begin
// init DCF FolderID/FileID with RTC data
#if (UI_STYLE==UI_STYLE_DRIVER)
void System_OnStrgInitDCFFolder(void);
#endif

void System_OnStrgInit_FS(void)
{
    {
        MEM_RANGE Pool;
        Pool.Addr = OS_GetMempoolAddr(POOL_ID_FS_BUFFER);
        Pool.Size = OS_GetMempoolSize(POOL_ID_FS_BUFFER);
        #if USE_FILEDB
        GxStrg_SetConfig(FILE_CFG_SUPPORT_EXFAT, FALSE); // cannot support exFAT if using FileDB
        #endif

        GxStrg_SetConfig(FILE_CFG_BUF, (UINT32)&Pool);
        //FileSys_InstallCmd();
    }
    {
        MEM_RANGE WorkBuf;
        WorkBuf.Addr = OS_GetMempoolAddr(POOL_ID_EXIF);
        WorkBuf.Size = POOL_SIZE_EXIF;
        Exif_Init(&WorkBuf, ExifCB);
        #if (_SENSORLIB2_ != _SENSORLIB2_DUMMY_)
        WorkBuf.Addr += POOL_SIZE_EXIF;
        EXIF_Init(EXIF_HDL_ID_2, &WorkBuf, ExifCB);
        #endif
    }
    GxStrg_RegCB(Strg_CB);         //Register CB function of GxStorage (NANR or CARD)
    {
        #if 0 // don't need to set DCF parameters since we use FileDB
        //FileSys:
        //1.設定init值
        //#NT#2012/11/14#Philex Lin - begin
        // init DCF FolderID/FileID with RTC data
        #if (UI_STYLE==UI_STYLE_DRIVER)
        System_OnStrgInitDCFFolder();
        #else
        // Initialize DCF name.ext
        //DCF dir-name
        DCF_SetDirFreeChars( DCF_SUBPATH );
        //DCF file-name
        DCF_SetFileFreeChars( DCF_FILE_TYPE_ANYFORMAT, DCF_IMAGE_NAME );
        #endif
        //#NT#2012/11/14#Philex Lin - end
        //DCF file-ext
        #if (MOVIE_FORMAT == MOVIE_FORMAT_AVI)
        DCF_SetParm(DCF_PRMID_SET_VALID_FILE_FMT, DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_AVI);
        #else
        DCF_SetParm(DCF_PRMID_SET_VALID_FILE_FMT, DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_AVI|DCF_FILE_TYPE_MOV|DCF_FILE_TYPE_MP4);
        #endif
        DCF_SetParm(DCF_PRMID_SET_DEP_FILE_FMT, DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_WAV|DCF_FILE_TYPE_MPO);
        //TODO: [DCF] How to add an new format & its ext?
        #endif

        //2.設定CB值,
        //3.註冊SxJob服務 ---------> System Job
        //4.註冊SxTimer服務 ---------> Detect Job
        #if (SDINSERT_FUNCTION == ENABLE)
        SX_TIMER_DET_STRG_ID = SxTimer_AddItem(&Timer_Card_DetInsert);
        #endif
        SX_TIMER_DET_SYSTEM_BUSY_ID = SxTimer_AddItem(&Timer_System_DetBusy);
        //5.註冊SxCmd服務 ---------> Cmd Function
        //System_AddSxCmd(Storage_OnCommand); //GxStorage

        //start scan
        SxTimer_SetFuncActive(SX_TIMER_DET_STRG_ID, TRUE);
        SxTimer_SetFuncActive(SX_TIMER_DET_SYSTEM_BUSY_ID, TRUE);
        /*
        #if (SDINSERT_FUNCTION == ENABLE)
        {
            //提早detect storage可以得到兩個好處:
            //1.知道storage的type是哪一種
            //2.立刻"啟動File System"
            //  如果沒這樣, 就要等到SXOPEN之後, SX_TIMER_DET_STRG_ID開始跑才會"啟動File System",
            //  與其他task的競爭結果, 容易造成"啟動File System"的timing不固定
            GxDet_Storage();
            GxDet_Storage();
            Delay_DelayMs(30);
        }
        #endif
        */
    }

}

void System_OnStrgExit_FS(void)
{
    //PHASE-2 : Close Lib or LibExt
    //#NT#2016/05/03#Lincy Lin -begin
    //#NT#Support logfile function
    #if (LOGFILE_FUNC==ENABLE)
    LogFile_Close();
    #endif
    //#NT#2016/05/03#Lincy Lin -end
    {
        GxStrg_CloseDevice(0); // unmount file system
    }
}

//#NT#2012/11/14#Philex Lin - begin
// init DCF FolderID/FileID with RTC data
#if (UI_STYLE==UI_STYLE_DRIVER)
#include "HwClock.h"
void System_OnStrgInitDCFFolder(void)
{
    CHAR pFolderName[6]="MEDIA";
    CHAR pFileName[5]="PTDC";
    struct tm tm_cur=HwClock_GetCurrentTime();;

    sprintf(pFolderName,"%1d%02d%02d",tm_cur.tm_year%0x0A,tm_cur.tm_mon,tm_cur.tm_mday);
    sprintf(pFileName,"%02d%02d",tm_cur.tm_hour,tm_cur.tm_min);

    // Initialize DCF name.ext
    //DCF dir-name
    DCF_SetDirFreeChars( pFolderName );
    //DCF file-name
    DCF_SetFileFreeChars( DCF_FILE_TYPE_ANYFORMAT, pFileName);
}
#endif
//#NT#2012/11/14#Philex Lin - end

//# add init DCF Dir/File ID
//# add detect file system status
#if 0
static void FileSys_SetInitNumbering(void)
{
    UINT32     DCF_DirId=0, DCF_FileId=0;

    DCF_GetNextID(&DCF_DirId,&DCF_FileId);

    if (DCF_DirId < UI_GetData(FL_DCF_DIR_ID))
    {
        DCF_DirId = UI_GetData(FL_DCF_DIR_ID);
        DCF_FileId = UI_GetData(FL_DCF_FILE_ID);
    }
    else if(DCF_DirId == UI_GetData(FL_DCF_DIR_ID) && DCF_FileId < UI_GetData(FL_DCF_FILE_ID))
    {
        DCF_FileId = UI_GetData(FL_DCF_FILE_ID);
    }
    DCF_SetNextID(DCF_DirId,DCF_FileId);
}
#endif

static BOOL FileSys_DetBusy(void)
{
    return (BOOL)((INT32)FileSys_GetParam(FST_PARM_TASK_STS, 0) == FST_STA_BUSY);
}

void Card_DetInsert(void)
{
    DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1);
    GxStrg_Det((UINT32)pStrgDev);
}

void Card_DetBusy(void)
{
    static BOOL bBusyLED = FALSE;
    //static BOOL bIsSendingStorageInit = FALSE;

    if(GxPower_GetControl(GXPWR_CTRL_SLEEP_LEVEL) <=1 && (!GxPower_GetControl(GXPWR_CTRL_BATTERY_CHARGE_EN)))
    {
        if (FileSys_DetBusy() || GxFlash_IsCharging() == TRUE)
        {
            if (bBusyLED == FALSE)
            {
                DBG_IND("System - busy\r\n");
                bBusyLED = TRUE;
                //GxLED_SetCtrl(KEYSCAN_LED_RED,SETLED_SPEED,GXLED_1SEC_LED_TOGGLE_CNT/5);
                //GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,TRUE);
                //GxLED_SetCtrl(KEYSCAN_LED_GREEN,SETLED_SPEED,GXLED_1SEC_LED_TOGGLE_CNT/5);
                //GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,FALSE);
                //GxLED_SetCtrl(KEYSCAN_LED_GREEN,SET_TOGGLE_LED,TRUE);
            }
        }
        else
        {
            if (bBusyLED == TRUE)
            {
                DBG_IND("System - not busy\r\n");
                bBusyLED = FALSE;
                //GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,FALSE);
                //GxLED_SetCtrl(KEYSCAN_LED_RED,TURNON_LED,FALSE);
                //GxLED_SetCtrl(KEYSCAN_LED_GREEN,SET_TOGGLE_LED,FALSE);
                //GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,FALSE);
            }
        }
    }
}

//Device flow event

INT32 System_OnStrgInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //if (bStorageInitsts == FALSE)
    char* pDxName=NULL;
    DX_HANDLE pStrgDevNew = (DX_HANDLE)(paramArray[0]);
    DX_HANDLE pStrgDevOld = GxStrg_GetDevice(0);
    /*
         STORAGE_CB_CARD_UNKNOWN
         呼叫File_Abort(), PowerOff()
    */

    if(Dx_GetInfo(pStrgDevOld, DX_INFO_NAME,&pDxName)!=DX_OK)
    {
        pDxName = NULL;
    }

    if((pStrgDevOld!=0) && pDxName!=NULL && strcmp(pDxName,"Storage_Card1")!=0) //old is not card
    {
        //#NT#2012/10/30#Lincy Lin -begin
        //#NT#Card hot plug in, out, should power off
        #if 1
			#if 0
	        char StringTmpBuf[] = "Card Inserted, System Reboot";
	        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,1,FLOWWRNMSG_ISSUE_CARD_ERR,0);
	        //State_SetData(&UIFlowWndWrnMsg_StatusTXT_MsgCtrl,STATE_CURITEM,Txt_Pointer(StringTmpBuf));
	        UxState_SetItemData(&UIFlowWndWrnMsg_StatusTXT_MsgCtrl,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_NO_CARD,STATE_ITEM_STRID,Txt_Pointer(StringTmpBuf));
	        Ux_Redraw();
	        Delay_DelayMs(1000);
	        GxSystem_EnableHWResetByAlarmTime(0,0,0,5);
			#endif
	        System_PowerOff(SYS_POWEROFF_NORMAL);
        #else
	        DBG_IND("File change to CARD storage\r\n");

	        // unmount file system
	        if (GxStrg_CloseDevice(0) != TRUE)
	        {
	            DBG_ERR("Storage unmount %s fail\r\n", Dx_GetName(pStrgDevOld));
	            return NVTEVT_CONSUME;
	        }
        #endif
    }
    pStrgDevOld = (DX_HANDLE)GxStrg_GetDevice(0);
    if(pStrgDevOld==0)
    {
        // mount file system
        if (GxStrg_OpenDevice(0, pStrgDevNew) != TRUE)
        {
            char* pDxName="unknown";
            Dx_GetInfo(pStrgDevNew, DX_INFO_NAME,&pDxName);
            DBG_ERR("Storage mount %s fail\r\n",pDxName);
            return NVTEVT_CONSUME;
        }
    }
    if (GxStrg_GetDeviceCtrl(0, CARD_READONLY))
    {
        UI_SetData(FL_CardStatus, CARD_LOCKED);
        DBG_IND("Card Locked\r\n");
    }
    else
    {
        UI_SetData(FL_CardStatus, CARD_INSERTED);
        DBG_IND("Card inserted\r\n");
    }
    return NVTEVT_CONSUME;
}

INT32 System_OnStrgRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //if (bStorageInitsts == FALSE)
    char* pDxName=NULL;
    DX_HANDLE pStrgDevOld = GxStrg_GetDevice(0);
    DX_HANDLE pStrgDevNew = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1);

    UI_SetData(FL_CardStatus, CARD_REMOVED);

    //#NT#2016/05/03#Lincy Lin -begin
    //#NT#Support logfile function
    #if (LOGFILE_FUNC==ENABLE)
    LogFile_Suspend();
    LogFile_Close();
    #endif
    //#NT#2016/05/03#Lincy Lin -end

    if(Dx_GetInfo(pStrgDevOld, DX_INFO_NAME,&pDxName)!=DX_OK)
    {
        pDxName = NULL;
    }

    if((pStrgDevOld!=0) && pDxName!=NULL && strcmp(pDxName,"Storage_Card1")==0) //old is card
    {
        //#NT#2012/10/30#Lincy Lin -begin
        //#NT#Card hot plug in, out, should power off
        #if(RTSP_LIVEVIEW_FUNC==ENABLE)
        //RTSP do not power off for easy to work on debug
        #elif (1)
        #if 0
        char StringTmpBuf[] = "Card Removed, System Reboot";
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,1,FLOWWRNMSG_ISSUE_NO_CARD,0);
        //State_SetData(&UIFlowWndWrnMsg_StatusTXT_MsgCtrl,STATE_CURITEM,Txt_Pointer(StringTmpBuf));
        UxState_SetItemData(&UIFlowWndWrnMsg_StatusTXT_MsgCtrl,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_NO_CARD,STATE_ITEM_STRID,Txt_Pointer(StringTmpBuf));
        //Static_SetData(&UIFlowWndWrnMsg_StatusTXT_MsgCtrl,STATIC_VALUE,Txt_Pointer(StringTmpBuf));
        Ux_Redraw();
        Delay_DelayMs(1000);
        GxSystem_EnableHWResetByAlarmTime(0,0,0,5);
        #endif

//#NT#2017/02/17#KCHong -begin
//#NT#Skip NVTEVT_STRG_REMOVE event when firmware updating
		if (IsFWUpdating == FALSE)
//#NT#2017/02/17#KCHong -end
        	System_PowerOff(SYS_POWEROFF_NORMAL);
        #else

        DBG_IND("File change to NAND storage\r\n");
        // unmount file system
        if (GxStrg_CloseDevice(0) != TRUE)
        {
            DBG_ERR("Storage unmount %s fail\r\n", Dx_GetName(pStrgDevOld));
            return NVTEVT_CONSUME;
        }
        #endif
        //#NT#2012/10/30#Lincy Lin -end
    }
    pStrgDevOld = GxStrg_GetDevice(0);
    if(pStrgDevOld==0)
    {
        // mount file system
        if (GxStrg_OpenDevice(0, pStrgDevNew) != TRUE)
        {
            char* pDxName="unknown";
            Dx_GetInfo(pStrgDevNew, DX_INFO_NAME,&pDxName);
            DBG_ERR("Storage mount %s fail\r\n", pDxName);
            return NVTEVT_CONSUME;
        }
        else
        {
			//#NT#2017/02/20#KCHong -begin
            //#NT#Enable SD_CARD_POWER
            // Turn off SD card power
			extern void DrvCARD_EnableCardPower(BOOL bEn);
            DrvCARD_EnableCardPower(FALSE);
			//#NT#2017/02/20#KCHong -end
        }
        //bStorageInitsts = TRUE;
    }
    return NVTEVT_CONSUME;
}

INT32 System_OnStrgAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 result = paramArray[1];
    //if spi use RamDisk as inner Memory,need to format RamDisk
#if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
    DX_HANDLE pStrgDevCur = (DX_HANDLE)GxStrg_GetDevice(0);
    DX_HANDLE pStrgDevRAM = (DX_HANDLE)Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1);

    if((pStrgDevCur == pStrgDevRAM)&&(result!=FST_STA_OK))
    {
        //UINT32 hStrgObj = Dx_Getcaps((UINT32)pStrgDevCur, STORAGE_CAPS_HANDLE, 0);
        //result = FileSys_FormatDisk((HNVT_STRG)hStrgObj, TRUE);
        result = FileSys_FormatDisk(pStrgDevCur, TRUE);
    }
#endif
    switch(result)
    {
    case FST_STA_OK:
        //#NT#2013/03/14#Lincy Lin -begin
        //#NT#Move out from GxStrg
        #if !USE_FILEDB
        if (!UI_GetData(FL_IsCopyToCarding))
        {
            DCF_ScanObj();
        }
        #endif

        if(gBinInfo.ld.LdCtrl2 & LDCF_UPDATE_FW)
        {
            FST_FILE hFile = FileSys_OpenFile(FW_DEL_INDIACTION_PATH,FST_OPEN_READ|FST_OPEN_EXISTING);
            if(hFile != NULL)
            {
                DBG_DUMP("Detected %s, delete %s \r\n",FW_DEL_INDIACTION_PATH,LOADER_UPD_FW_PATH);
                FileSys_CloseFile(hFile);
                // Delete FW bin from A:
                if(FileSys_DeleteFile(LOADER_UPD_FW_PATH)!=FST_STA_OK)
                {
                    DBG_ERR("delete "_BIN_NAME_".BIN failed .\r\n");
                }
                if(FileSys_DeleteFile(FW_DEL_INDIACTION_PATH)!=FST_STA_OK)
                {
                    DBG_ERR("delete %s failed .\r\n",FW_DEL_INDIACTION_PATH);
                }
            }
        }
        //#NT#2013/03/14#Lincy Lin -end
        //#NT#2016/02/18#Lincy Lin -begin
        //#NT#Support logfile function
        #if (LOGFILE_FUNC==ENABLE)
        {
            LOGFILE_OPEN    logOpenParm={0};
            UINT32          maxFileNum = 10;
            UINT32          maxFileSize = 0x40000; // 256KB
            CHAR            rootDir[LOGFILE_ROOT_DIR_MAX_LEN+1]="A:\\Novatek\\LOG\\";

            logOpenParm.maxFileNum = maxFileNum;
            logOpenParm.maxFileSize = maxFileSize;
            strncpy(logOpenParm.rootDir,rootDir,LOGFILE_ROOT_DIR_MAX_LEN);
            LogFile_Open(&logOpenParm);
        }
        #endif
        //#NT#2016/02/18#Lincy Lin -end
        UI_SetData(FL_FSStatus, FS_INIT_OK);
        break;
    case FST_STA_DISK_UNFORMAT:
        UI_SetData(FL_FSStatus, FS_UNFORMATTED);
        break;
    case FST_STA_DISK_UNKNOWN_FORMAT:
        UI_SetData(FL_FSStatus, FS_UNKNOWN_FORMAT);
        break;
    case FST_STA_CARD_ERR:
        UI_SetData(FL_FSStatus, FS_DISK_ERROR);
        break;
    #if 0
    case FST_STA_OUT_OF_ID:
        UI_SetData(FL_FSStatus, FS_NUM_FULL);
        break;
    #endif
    default:
        UI_SetData(FL_FSStatus, FS_DISK_ERROR);
        break;
    }
    Ux_PostEvent(NVTEVT_EXE_FILESYS_INIT, 0, 0);
    Ux_PostEvent(NVTEVT_STORAGE_INIT, 0, 0);
    GxPower_SetControl(GXPWR_CTRL_SLEEP_RESET,0);
    return NVTEVT_CONSUME;
}

INT32 System_OnStrgDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    switch(paramArray[1])
    {
    case FST_STA_OK:
        DBG_IND("FS: unmount OK\r\n");
        break;

    default:
        DBG_ERR("^RFS: unmount FAIL\r\n");
        break;
    }
    return NVTEVT_CONSUME;
}

//static BOOL bStorageInitsts = FALSE;
BOOL gChkCardPwrOn = FALSE;
BOOL gChkCardChange = FALSE;

void Storage_PowerOn_Start(void)
{
    gChkCardPwrOn = GxStrg_GetDeviceCtrl(0, CARD_INSERT);
    DBG_IND("^BStg Power On = %d\r\n", gChkCardPwrOn);
}
void Storage_UpdateSource(void)
{
    DBG_IND("^Y-------------CARD det\r\n");
    if(gChkCardPwrOn)
    {
        if (FALSE == GxStrg_GetDeviceCtrl(0, CARD_INSERT)) //CARD已拔除
        {
            gChkCardChange = TRUE;
        }
    }
    else
    {
        if (TRUE == GxStrg_GetDeviceCtrl(0, CARD_INSERT)) //CARD已插入
        {
            gChkCardChange = TRUE;
        }
    }
}
void Storage_PowerOn_End(void)
{
    Storage_UpdateSource();
    gChkCardPwrOn = GxStrg_GetDeviceCtrl(0, CARD_INSERT);

    if (TRUE == gChkCardChange) //CARD曾經插拔
    {
        System_PowerOff(SYS_POWEROFF_NORMAL); //關機
        return;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
//  PSTORE
//
///////////////////////////////////////////////////////////////////////////////

void System_OnStrgInit_PS(void)
{
    //PHASE-2 : Init & Open Lib or LibExt
#if PST_FUNC
    // Open PStore
    PSFMT gFmtStruct = {MAX_BLK_PER_SEC,MAX_SEC_NUM};
    PSTORE_INIT_PARAM gPStoreParam;
    UINT32 result=0;
    UINT8 *pBuf;
    DX_HANDLE pStrgDev = 0;
    //UINT32 hStrg = 0;
    pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND3);

    //PStore_InstallCmd();

    pBuf = (UINT8 *)OS_GetMempoolAddr(POOL_ID_PS_BUFFER);

    gPStoreParam.pBuf = pBuf;
    gPStoreParam.uiBufSize = POOL_SIZE_PS_BUFFER;

    result = PStore_Open(pStrgDev,&gPStoreParam);

    if(result!=E_PS_OK)
    {
         DBG_ERR("PStore Open fail %d format \r\n",result);
         result = PStore_Format(&gFmtStruct);
    }
    //#NT#2013/05/28#Lincy Lin -begin
    //#NT#
    // read calibration data
    #if (UI_STYLE!=UI_STYLE_DRIVER)
    Cal_ReadSysData();
    Cal_ApplyData(CAL_SYS_DATA);
    #endif
    //#NT#2013/05/28#Lincy Lin -end
#endif
}
void System_OnStrgExit_PS(void)
{
#if PST_FUNC
    PStore_Close();
#endif
}

///////////////////////////////////////////////////////////////////////////////
//
//  FWSTORE
//
///////////////////////////////////////////////////////////////////////////////
#include "DxStorage.h"
#include "FileSysTsk.h"
#include "FwSrvAPI.h"
#include "MemCheck.h"
#include "GxStrg.h"

static UINT32 gPL_BufAddr = 0;
static UINT32 gPL_BufSize = 0;
static FWSRV_INIT gPL_Init={0};
static FWSRV_CMD gPL_Cmd={0};
static FWSRV_PL_LOAD_BURST_IN gPL_In = {0};

void System_OnStrgInit_FWS(void)
{
    BOOL bLZC = FALSE;
    ER er;
    UINT32 m_LdLoadedOffest;
    DX_HANDLE pStrgDev = 0;

    DBG_MSG("Init!\r\n");
    DBG_IND("^M LD_BLOCK=%d\r\n", gBinInfo.ld.LdBlockCount);
    DBG_IND("^M FW_MAX_SIZE=%08X\r\n", gBinInfo.ld.FWResvSize);
    DBG_IND("^MFW_validate-update:");
    //ValidateUpdateFW
    {
        UINT32 bUpdate = ((gBinInfo.ld.LdCtrl2 & LDCF_UPDATE_FW)!=0);
        UINT32 uiUpdateResult = ((gBinInfo.ld.LdCtrl2 & LDCF_UPDATE_FW_DONE)!=0);
        if(bUpdate)
        {
            DBG_IND("\r\n^MFW is just updated.\r\n");
            if(!uiUpdateResult)
            {
                DBG_FATAL("\r\nFW updated: fail!\r\n");
            }
        }
    }
    DBG_IND("^M ok\r\n");


    pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND0);

    #if (_INTERSTORAGE_==_INTERSTORAGE_SPI_)
    bLZC = TRUE;
    #else
    bLZC = FALSE;
    #endif

    if(bLZC)
    {
        gPL_Init.PlInit.DataType = PARTLOAD_DATA_TYPE_COMPRESS_LZ;
        gPL_BufAddr = OS_GetMempoolAddr(POOL_ID_APP);
        gPL_BufSize = POOL_SIZE_APP;
    }
    else
    {
        gPL_Init.PlInit.DataType = PARTLOAD_DATA_TYPE_UNCOMPRESS;
        gPL_BufAddr = OS_GetMempoolAddr(POOL_ID_FWS_BUFFER);
        gPL_BufSize = POOL_SIZE_FWS_BUFFER;
    }

    m_LdLoadedOffest = gBinInfo.ld.LdLoadSize; //Get Ld loaded size

    gPL_Init.uiApiVer = FWSRV_API_VERSION;
    gPL_Init.TaskID = 0;
    gPL_Init.SemID = 0;
    gPL_Init.FlagID = 0;
    gPL_Init.PlInit.uiApiVer = PARTLOAD_API_VERSION;
    gPL_Init.PlInit.hStrg = pStrgDev;
    gPL_Init.PlInit.uiAddrBegin = OS_GetMemAddr(MEM_CODE)+m_LdLoadedOffest; //Must be block boundary align
    gPL_Init.PlInit.uiWorkingAddr = gPL_BufAddr;
    gPL_Init.PlInit.uiWorkingSize = gPL_BufSize;
    er = FwSrv_Init(&gPL_Init);
    if(er != FWSRV_ER_OK)
    {
        DBG_ERR("Init failed!\r\n");
    }
    er = FwSrv_Open();
    if(er != FWSRV_ER_OK)
    {
        DBG_ERR("Open failed!\r\n");
    }
}

void System_OnStrgExit_FWS(void)
{
    ER er;
    er = FwSrv_Close();
    if(er != FWSRV_ER_OK)
    {
        DBG_ERR("Close failed!\r\n");
    }
}

void System_OnStrg_DownloadFW(UINT32* SecOrderTable, void (*LoadCallback)(const UINT32 Idx))
{
    BOOL bEnablePartload = FALSE;
    int nSect = 0;
    DBG_MSG("Init!\r\n");

    DBG_DUMP("^G[LOAD-FW]\r\n");
    nSect = OS_GetMemSectionCount(MEM_CODE);  //How to get last section ZI
    DBG_DUMP("^GTotal Sections = %d\r\n", nSect);
    if(LoadCallback)
        LoadCallback(CODE_SECTION_01);
    DBG_IND("^M P1_LOAD_SIZE=%08X, TIME=%d\r\n", gBinInfo.ld.LdLoadSize, gBinInfo.ld.LdLoadTime);

    DBG_IND("^MPL_check_Ld:\r\n");
    DBG_IND("^M PL_EN=%08X\r\n", gBinInfo.ld.LdCtrl & LDCF_PARTLOAD_EN);
    DBG_IND("^M LZ_EN=%08X\r\n", gBinInfo.head.BinCtrl & HDCF_LZCOMPRESS_EN);
    bEnablePartload = ((gBinInfo.ld.LdCtrl & LDCF_PARTLOAD_EN)!=0);

    if(!bEnablePartload)
        return;

    DBG_IND("^MPL_validate-p1-range:");
    //ValidatePartOne
    {
        // Make Sure: Partloader start (PART-2 start) <= Loader loaded end
        int i;
        UINT32 FwSize = 0;
        UINT32 uiDestAddress = OS_GetMemAddr(MEM_CODE)+gBinInfo.ld.LdLoadSize;
        if((uiDestAddress != 0) && (uiDestAddress < OS_GetMemAddr(1)))
        {
            DBG_FATAL("PART_TWO start (%08x) > PART_ONE loaded end (%08x)!\r\n",
                OS_GetMemAddr(1), uiDestAddress );
        }

        // Make Sure: Partloader end (PART-last end) <= MEM_CODE end
        for(i=0;i<nSect;i++)
        {
            DBG_IND("Sect%d: Addr:%08X, Size:%08X\r\n",i,OS_GetMemAddr(i),OS_GetMemSize(i));
            FwSize += OS_GetMemSize(i);
        }
        if (OS_GetMemAddr(0)+FwSize > OS_GetMemAddr(MEM_CODE)+OS_GetMemSize(MEM_CODE))
        {
            DBG_FATAL("PART_LAST end (%08x) > MEM_CODE end (%08x)!\r\n",
                OS_GetMemAddr(0)+FwSize, OS_GetMemAddr(MEM_CODE)+OS_GetMemSize(MEM_CODE));
        }
    }
    DBG_IND("^M ok\r\n");

    DBG_DUMP("^MPL_begin\r\n");
    {
        ER er;
        gPL_In.puiIdxSequence = SecOrderTable;
        gPL_In.fpLoadedCb = LoadCallback;
        gPL_Cmd.Idx = FWSRV_CMD_IDX_PL_LOAD_BURST; //continue load
        gPL_Cmd.In.pData = &gPL_In;
        gPL_Cmd.In.uiNumByte = sizeof(gPL_In);
        gPL_Cmd.Prop.bExitCmdFinish = TRUE;
        er = FwSrv_Cmd(&gPL_Cmd);
        if(er != FWSRV_ER_OK)
        {
            DBG_ERR("Process failed!\r\n");
        }
    }
    DBG_DUMP("^MPL_end\r\n");
}

#define FW_CHECK_NOCHECK            0
#define FW_CHECK_CHECKSUM           1
#define FW_CHECK_CRC                2
#define FW_CHECK_METHOD             FW_CHECK_NOCHECK

#define FW_CHECK_PSEUDO_STR         "DSC_STR"
#define FW_UPDATE_FILE_LEN          16
static char uiUpdateFWName[FW_UPDATE_FILE_LEN] = FW_UPDATE_NAME;
static FWSRV_CMD Cmd={0};
static BOOL m_bUpdRename = FALSE;

static void CheckSumOKCb(void)
{
    FST_FILE hFile = FileSys_OpenFile(FW_DEL_INDIACTION_PATH,FST_OPEN_WRITE|FST_OPEN_ALWAYS);
    if(hFile != NULL)
    {
        FileSys_CloseFile(hFile);
    }
    else
    {
        DBG_ERR("create indication %s failed.\r\n",FW_DEL_INDIACTION_PATH);
        return;
    }
#if (MD5_CHECK_ENABLE)
    hFile = FileSys_OpenFile(uiUpdateFWName, FST_OPEN_WRITE|FST_OPEN_ALWAYS);
    if(hFile != NULL)
    {
	    FileSys_TruncFile(hFile, FWBufSize);
        FileSys_CloseFile(hFile);
    }
    else
    {
        DBG_ERR("create backup file %s failed.\r\n", "A:\\"_BIN_NAME_".BIN");
        return;
    }
#endif
    //Rename to be safe, that loader can update by name for next updating if FW updating FW failed.
    if(FileSys_RenameFile(_BIN_NAME_".BIN",uiUpdateFWName, TRUE)==FST_STA_OK)
    {
        m_bUpdRename = TRUE;
    }
    else
    {
        DBG_ERR("cannot rename bin file.\r\n");
    }
}


UINT32   System_OnStrg_UploadFW(UINT32 DevID)
{
    UINT32 m_MemSrc=0;
    UINT32 uiMemSize=0;
    //DX_HANDLE pStrgDev = (DX_HANDLE)GxStrg_GetDevice(DevID);
    DX_HANDLE pGxStrgDXH = (DX_HANDLE)GxStrg_GetDevice(DevID);
    //HNVT_STRG pStrg,pWhichObj;
    DX_HANDLE pFSStrgDXH;
    FST_FILE hFile=NULL;
    UINT32 uiSize=0; //FileSize
    INT32 fst_er;
#if (FW_CHECK_METHOD != FW_CHECK_NOCHECK)
    UINT32 uiSizeExt;
    MEMCHECK_PSEUDOSTR Pesudo = {0};
#endif
    FWSRV_ER fws_er;
	//#NT#2016/12/26#KCHong -begin
	//#NT#Sample code to calculate MD5
#if (MD5_CHECK_ENABLE)
	unsigned char MD5Digest[16] = {0};
	UINT32 i;
    INT32 MD5Status;
	char *cPtr = NULL;
	UINT16 AppendLen = 0;
	char *VerInfo = NULL;
#endif
	//#NT#2016/12/26#KCHong -end

    //if (pStrgDev == 0)
    if (pGxStrgDXH == 0)
    {
        debug_msg("Incorrect storage\r\n");
        return UPDNAND_STS_FW_INVALID_STG;
    }

    FileSys_GetStrgObj(&pFSStrgDXH);
    if(pFSStrgDXH != pGxStrgDXH)
    {
        debug_msg("Incorrect storage\r\n");
        return UPDNAND_STS_FW_INVALID_STG;
    }

    uiSize = (UINT32)FileSys_GetFileLen(uiUpdateFWName);

    //alloc mem
    uiMemSize = ALIGN_CEIL_16(uiSize) + FW_UPD_FW_TMP_MEM_SIZE + 16; //8:margin for m_MemSrc is not align 16
    m_MemSrc = ALIGN_CEIL_16(SxCmd_GetTempMem(uiMemSize));

    if(m_MemSrc==0)
    {
        DBG_ERR("not enough memory to read FW\r\n");

        return UPDNAND_STS_FW_READ_ERR;
    }

    if((uiSize==0) || (uiSize>uiMemSize))
    {
        debug_msg("FW bin read fail.\r\n");

        return UPDNAND_STS_FW_READ_ERR;
    }

    hFile = FileSys_OpenFile(uiUpdateFWName, FST_OPEN_READ);

    if(hFile != 0)
    {
        fst_er = FileSys_ReadFile(hFile, (UINT8*)m_MemSrc, &uiSize, 0, NULL);
        FileSys_CloseFile(hFile);
        if(fst_er != FST_STA_OK)
        {
            debug_msg("FW bin read fail. 1\r\n");
            return UPDNAND_STS_FW_READ2_ERR;
        }
		//#NT#2016/12/26#KCHong -begin
		//#NT#Sample code to calculate MD5
#if (MD5_CHECK_ENABLE)
		MD5Status = TRUE;

		// Get append length information
		cPtr = (UINT8*)(m_MemSrc+uiSize);
		AppendLen = (*(cPtr-2))+(*(cPtr-1))*256;
		if (AppendLen >= uiSize)
		{
			MD5Status = FALSE;
			goto MD5_Err;
		}

		// Check whether "ecbf" is presented at file tail.
		uiSize -= AppendLen;
		cPtr = (UINT8*)(m_MemSrc+uiSize);
		if((*cPtr != 'e') || (*(cPtr+1) != 'c') || (*(cPtr+2) != 'b') || (*(cPtr+3) != 'f'))
		{
			debug_msg("Cannot find MD5 info\r\n");
			MD5Status = FALSE;
			goto MD5_Err;
		}

		// If you want compare version, do something here....
		VerInfo = (UINT8*)(m_MemSrc+uiSize+4+16+2);
		DBG_DUMP("========>New VerInfo = %s\r\n", VerInfo);
		DBG_DUMP("========>Cur VerInfo = %s\r\n", CurVerInfo);

		// Calculate MD5 value
		md5_calc(MD5Digest, (UINT8*)m_MemSrc, uiSize);

		// Compare MD5 value
		for (i = 0; i < 16; i++)
		{
			if (MD5Digest[i] != *(cPtr+4+i))
			{
				MD5Status = FALSE;
			}
		}

MD5_Err:
		if(MD5Status == TRUE)
		{
			FWBufAddr = m_MemSrc;
			FWBufSize = uiSize;
			DBG_DUMP(" MD5 value TRUE \r\n");
		}
		else
		{
			DBG_DUMP(" MD5 value fail \r\n");
	        return UPDNAND_STS_FW_READ2_ERR;
		}
#endif
		//#NT#2016/12/26#KCHong -end
    }
    else
    {
        DBG_ERR("cannot find %s\r\n",uiUpdateFWName);
        return UPDNAND_STS_FW_READ2_ERR;
    }

/*
    memset(&Cmd,0,sizeof(Cmd));
    Buf.uiAddr = m_MemSrc;
    Buf.uiSize = (uiSize+(m_SectSize-1))&(~(m_SectSize-1));
    Cmd.Idx = FWSRV_CMD_IDX_FW_WRITE;
    Cmd.In.pData = &Buf;
    Cmd.In.uiNumByte = sizeof(Buf);
    Cmd.Prop.bExitCmdFinish = TRUE;
    fws_er = FwSrv_Cmd(&Cmd);
    if(fws_er != FWSRV_ER_OK)
    {
        debug_msg("FW bin write failed\r\n");
        return UPDNAND_STS_FW_WRITE_CHK_ERR;
    }
*/

	//#NT#2017/03/06#KCHong -begin
	//#NT#Bugzilla 3028#Calculate checksum here to filter invalid firmware file.
	if(MemCheck_CalcCheckSum16Bit(m_MemSrc, uiSize) != 0)
    {
        DBG_ERR("uITRON FW bin check sum error\r\n");
		return UPDNAND_STS_FW_READ_CHK_ERR;
    }
	//#NT#2017/03/06#KCHong -end

    FWSRV_BIN_UPDATE_ALL_IN_ONE FwInfo = {0};
    FwInfo.uiSrcBufAddr = m_MemSrc;
    FwInfo.uiSrcBufSize = uiSize;
    FwInfo.uiWorkBufAddr = m_MemSrc+ALIGN_CEIL_16(uiSize);
    FwInfo.uiWorkBufSize = FW_UPD_FW_TMP_MEM_SIZE;
    FwInfo.pPartition = NandPartition;
    FwInfo.fpItemCb = NULL;
    FwInfo.fpCheckSumCb = CheckSumOKCb;
    memset(&Cmd,0,sizeof(Cmd));
    Cmd.Idx = FWSRV_CMD_IDX_BIN_UPDATE_ALL_IN_ONE;
    Cmd.In.pData = &FwInfo;
    Cmd.In.uiNumByte = sizeof(FwInfo);
    Cmd.Prop.bExitCmdFinish = TRUE;

//#NT#2017/02/17#KCHong -begin
//#NT#Skip NVTEVT_STRG_REMOVE event when firmware updating
	IsFWUpdating =  TRUE;
//#NT#2017/02/17#KCHong -end

    fws_er = FwSrv_Cmd(&Cmd);
    //#NT#2016/02/17#Niven Cho -begin
    //#NT#Mechanism for Fw Updating Fw failed.
    if(fws_er == FWSRV_ER_INVALID_UPDATED_DATA || fws_er == FWSRV_ER_WRITE_BLOCK)
    {
        DBG_ERR("update failed, start to use loader update mechanism.\r\n");
        GxSystem_EnableHWResetByAlarmTime(0,0,0,5);
        GxSystem_PowerOff();
        return UPDNAND_STS_FW_WRITE_CHK_ERR;
    }
    else if(fws_er != FWSRV_ER_OK)
    {
        DBG_ERR("FW bin check failed %d\r\n",fws_er);
        //#NT#2016/02/18#Niven Cho -begin
        //#NT#improve fw update fw flow
        if(m_bUpdRename)
        {
            if(FileSys_RenameFile(&uiUpdateFWName[3],LOADER_UPD_FW_PATH, TRUE)==FST_STA_OK) //[3]: remove "A:\\"
            {
                FileSys_DeleteFile(FW_DEL_INDIACTION_PATH);
            }
        }
        //#NT#2016/02/18#Niven Cho -end
        return UPDNAND_STS_FW_READ_CHK_ERR;
}

    //#NT#2016/02/18#Niven Cho -begin
    //#NT#improve fw update fw flow
    //if success, we rename back to original name and delete indication file
    if(m_bUpdRename)
    {
        if(FileSys_RenameFile(&uiUpdateFWName[3],LOADER_UPD_FW_PATH, TRUE)==FST_STA_OK) //[3]: remove "A:\\"
        {
            FileSys_DeleteFile(FW_DEL_INDIACTION_PATH);
        }
    }
    //#NT#2016/02/18#Niven Cho -end
    //#NT#2016/02/17#Niven Cho -end
    DBG_DUMP("Update Success.\r\n");

    return UPDNAND_STS_FW_OK;
}

