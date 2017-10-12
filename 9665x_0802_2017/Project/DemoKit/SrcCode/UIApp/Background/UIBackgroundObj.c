////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFramework.h"
#include "UIBackgroundObj.h"
#include "UIPlayObj.h"
#include "Utility.h"
#include "UIPrinterObj.h" //#NT#2009/12/04#Lily Kao
#include "UIMovieObj.h"
#include "DpofVendor.h"

#include "Filesystsk.h"
#include "DCF.h"
#include "UIInfo.h"
#include "UIMode.h"
#if _DEMO_TODO
#include "DPSFuncs.h"
//#NT#2011/02/15#Brad Chen -begin
//#NT# add detect storage
#include "DxStorage.h"
//#NT#2011/02/15#Brad Chen -end
//#NT#2009/07/01#Brad Chen -begin
//#NT#Add to support MSDC CDROM function for nand2
//#NT#2009/07/01#Brad Chen -end
#endif
#include "UIAppPhoto.h"
#include "UIAppMovie.h"
//for IPL DZoom - begin
#include "IPL_Cmd.h"
#include "IPL_Utility.h"
//for IPL DZoom - end
//#NT#2012/10/30#Calvin Chang#Generate Audio NR pattern by noises of zoom, focus and iris -begin
#include "UIAppMovie.h"
#include "Lens.h"
//#NT#2012/10/30#Calvin Chang -end
#include "UIAppNetwork.h"
//#NT#2009/09/14#Brad Chen -begin
//#NT#Add copy file from SD to Nand
#define FILE_SIZE_LIMITATION    0x1000000 //16M
//#NT#2009/09/14#Brad Chen -end

#include "DXType.h"
#include "DX.h"
#include "Jpg2Movie.h"

#include "PlaybackTsk.h" //#NT#2010/10/01#Ben Wang -begin

#include "PBXEdit.h"
#include "PBXRedEye.h"
#include "DialogWarn.h"
#include "FileDB.h"
#include "Dma.h"
#include "UISetup.h"
#define NVT_PTR_RESPOSE_TIME            15  //15 seconds
#define NVT_PC_RESPOSE_TIME             10  //10 seconds

static UINT32 BackgroundDelAll(void);
static UINT32 BackgroundCopy2Card(void);
#if(COPYCARD2CARD_FUNCTION == ENABLE)
static UINT32 BackgroundCopyCard1ToCard2(void);
static UINT32 BackgroundCopyCard2ToCard1(void);
#endif
static UINT32 BackgroundFormat(void);
static UINT32 BackgroundFormatCard(void);
static UINT32 BackgroundFormatNand(void);
static UINT32 BackgroundNumReset(void);
static UINT32 BackgroundSetDPOF(void);
static UINT32 BackgroundSetProtect(void);
static UINT32 BackgroundWaitPrintInit(void);
static UINT32 BackgroundResize(void);
static UINT32 BackgroundFixRedEye(void);
static UINT32 BackgroundReSave(void);
static UINT32 BackgroundNovaLightFix(void);
static UINT32 BackgroundDummyWait(void);
static UINT32 BackgroundDummyWaitUSB(void);//#NT#2010/06/23#Lily Kao
static UINT32 BackgroundFWUpdate(void);
static UINT32 BackgroundDelFolder(void);//#NT#2010/10/01#Ben Wang -begin
static UINT32 BackgroundUserFunc(void);
//#NT#2011/02/10#Photon Lin -begin
//#Add file access
static UINT32 BackgroundFileAccess(void);
//#NT#2011/02/10#Photon Lin -end
static UINT32 BackgroundDZoomIn(void);
static UINT32 BackgroundDZoomOut(void);
static UINT32 BackgroundPIMProcess(void);
//#NT#2012/10/30#Calvin Chang#Generate Audio NR pattern by noises of zoom, focus and iris -begin
static UINT32 BackgroundAudiNRLensAction(void);
//#NT#2012/10/30#Calvin Chang -end
static UINT32 BackgroundWiFiOn(void);
static UINT32 BackgroundWiFiClearACL(void);
static UINT32 BackgroundMakeMovie(void);

static UINT32 g_uiNewWidth = 0, g_uiNewHeight = 0;
static BOOL  g_bOverWrite = TRUE;
static UINT32 g_uiDpofOPMode = PLAYDPOF_SETONE;
static UINT32 g_uiDpofPrtNum = 0;
static UINT32 g_uiDpofDateOn = FALSE;
static UINT32 g_uiProtectType = PLAY_PROTECT_ALL;
static UINT32 g_uiWaitTime = 1;
static FP    g_fpUserFunc = NULL;
static UINT32* g_pDzoomStop = NULL;

UINT32 gUIStopBckGndDummyWait = FALSE;
UINT32 g_uiWaitTimeChkPtr = FALSE;


BKG_JOB_ENTRY gBackgroundExtFuncTable[] =
{
    {NVTEVT_BKW_DELALL,             BackgroundDelAll},
    {NVTEVT_BKW_COPY2CARD,          BackgroundCopy2Card},
    #if(COPYCARD2CARD_FUNCTION == ENABLE)
    {NVTEVT_BKW_COPYCARD1ToCARD2,   BackgroundCopyCard1ToCard2},
    {NVTEVT_BKW_COPYCARD2ToCARD1,   BackgroundCopyCard2ToCard1},
    #endif
    {NVTEVT_BKW_FORMAT,             BackgroundFormat},
    {NVTEVT_BKW_FORMAT_CARD,        BackgroundFormatCard},
    {NVTEVT_BKW_FORMAT_NAND,        BackgroundFormatNand},
    {NVTEVT_BKW_NUM_RESET,          BackgroundNumReset},
    {NVTEVT_BKW_SETDPOF,            BackgroundSetDPOF},
    {NVTEVT_BKW_SETPROTECT,         BackgroundSetProtect},
    {NVTEVT_BKW_WAIT_PTR_INIT,      BackgroundWaitPrintInit},
    {NVTEVT_BKW_RESIZE,             BackgroundResize},
    {NVTEVT_BKW_FIXREDEYE,          BackgroundFixRedEye},
    {NVTEVT_BKW_RESAVEFILE,         BackgroundReSave},
    {NVTEVT_BKW_NOVALIGHT_FIX,      BackgroundNovaLightFix},
    {NVTEVT_BKW_DUMMY_WAIT,         BackgroundDummyWait},
    {NVTEVT_BKW_DUMMY_WAIT_USB,     BackgroundDummyWaitUSB},//#NT#2010/06/23#Lily Kao
    {NVTEVT_BKW_FW_UPDATE,          BackgroundFWUpdate},
//#NT#2010/10/01#Ben Wang -begin
//#NT#Add deleting by date
    {NVTEVT_BKW_DELFOLDER,          BackgroundDelFolder},
//#NT#2010/10/01#Ben Wang -end
//#NT#2011/02/01#Ben Wang -begin
    {NVTEVT_BKW_USER_FUNCTION,      BackgroundUserFunc},
//#NT#2011/02/01#Ben Wang -end
    //#NT#2011/02/10#Photon Lin -begin
    //#Add file access
    {NVTEVT_BKW_FILE_ACCESS,        BackgroundFileAccess},
    //#NT#2011/02/10#Photon Lin -end
    {NVTEVT_BKW_DZOOM_IN,           BackgroundDZoomIn},
    {NVTEVT_BKW_DZOOM_OUT,          BackgroundDZoomOut},
    {NVTEVT_BKW_PIM_PROCESS,        BackgroundPIMProcess},
    //#NT#2012/10/30#Calvin Chang#Generate Audio NR pattern by noises of zoom, focus and iris -begin
    {NVTEVT_BKW_ANR_LENS_ACTION,    BackgroundAudiNRLensAction},
    //#NT#2012/10/30#Calvin Chang -end
    {NVTEVT_BKW_WIFI_ON,            BackgroundWiFiOn},
    {NVTEVT_BKW_WIFI_CLEARACL,      BackgroundWiFiClearACL},
    {NVTEVT_BKW_MAKE_MOVIE,         BackgroundMakeMovie},
    {0,                             0},
};

//#NT#2010/11/09#Lily Kao -begin
UINT32 gUIPrtModeIsClosed = FALSE;


UINT32 Background_CopyTo(BOOL isCopyToCard,UINT32 copMode)
{
    UINT32 uiTempBufAddr, uiTempBufSize, uiTempBufEnd, FileNumber;
    INT32  uiError_Code = FST_STA_OK;
    //HNVT_STRG pSrcObj, pDstObj, pOriObj;
    DX_HANDLE pSrcDXH = 0, pDstDXH, pOriDXH;
    COPYTO_BYNAME_INFO CopyInfo;
    #if USE_FILEDB
    FILEDB_HANDLE fileDbHandle, fileDbbufSize=0x200000, fileDbbufAddr;
    #else
    UINT32 uiThisFileFormat;
    UINT32 DirID,FileID;
    UINT32 NewDirID,NewFileID,i;
    CHAR   srcPath[DCF_FULL_FILE_PATH_LEN],dstPath[DCF_FULL_FILE_PATH_LEN];
    #endif

    uiTempBufSize = 0x100000;
    uiTempBufAddr = SxCmd_GetTempMem(uiTempBufSize);
    uiTempBufEnd = OS_GetMempoolAddr(POOL_ID_APP)+ POOL_SIZE_APP;
    uiTempBufSize = uiTempBufEnd - uiTempBufAddr;

    // reserved 2MB for FileDB using
    #if USE_FILEDB
    uiTempBufSize -=fileDbbufSize;
    fileDbbufAddr = uiTempBufAddr+uiTempBufSize;
    #endif

    //debug_msg("uiTempBufAddr = 0x%x, uiTempBufSize = 0x%x\r\n",uiTempBufAddr,uiTempBufSize);
    //debug_msg("fileDbbufAddr = 0x%x, fileDbbufSize = 0x%x\r\n",fileDbbufAddr,fileDbbufSize);

    UI_SetData(FL_IsCopyToCarding, TRUE);

    if (isCopyToCard)
    {
        FileSys_GetStrgObj(&pDstDXH);
        pOriDXH = pDstDXH;
        // --> TODO:remove begin
        #if (_INTERSTORAGE_ == _INTERSTORAGE_NAND_)
        {
            pSrcDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1);
        }
        #else
        //pSrcObj = (HNVT_STRG)sdio2_getStorageObject(STRG_OBJ_FAT1);
        //sdio2_setDetectCardExistHdl(LibFs_CheckCardInserted);
        //sdio2_setDetectCardProtectHdl(SLibFs_CheckCardWP);
        #endif
        // --> TODO:remove end
    }
    else
    {
        //FileSys_GetStrgObj(&pOriObj);
        FileSys_GetStrgObj(&pOriDXH);
        //pSrcObj = pOriObj;
        pSrcDXH = pOriDXH;
        // --> TODO:remove begin
        #if (_INTERSTORAGE_ == _INTERSTORAGE_NAND_)
        {
            pDstDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1);
        }
        #else
        //pDstObj = (HNVT_STRG)sdio2_getStorageObject(STRG_OBJ_FAT1);
        //sdio2_setDetectCardExistHdl(LibFs_CheckCardInserted);
        //sdio2_setDetectCardProtectHdl(SLibFs_CheckCardWP);
        #endif
    }
    memset(&CopyInfo, 0, sizeof(CopyInfo));
    CopyInfo.pSrcStrg = pSrcDXH;
    CopyInfo.pDstStrg = pDstDXH;
    CopyInfo.pBuf = (char *)uiTempBufAddr;
    CopyInfo.uiBufSize = uiTempBufSize;

    #if USE_FILEDB
    {
        if (pOriDXH!=pSrcDXH)
        {
            FileSys_ChangeDisk(pSrcDXH);
        }
        //  create fileDB for copy
        {
            FILEDB_INIT_OBJ   FilDBInitObj={0};
            CHAR              rootPath[20]="A:\\";

            FilDBInitObj.rootPath = rootPath;
            FilDBInitObj.bIsRecursive = TRUE;
            FilDBInitObj.bIsCyclic = TRUE;
            FilDBInitObj.bIsMoveToLastFile = TRUE;
            FilDBInitObj.bIsSupportLongName = TRUE;
            FilDBInitObj.bIsDCFFileOnly = FALSE;
            FilDBInitObj.bIsChkHasFile = FALSE;
            FilDBInitObj.u32MaxFilePathLen = 60;
            FilDBInitObj.u32MaxFileNum = 20000;
            FilDBInitObj.fileFilter = FILEDB_FMT_JPG|FILEDB_FMT_AVI|FILEDB_FMT_MOV|FILEDB_FMT_MP4;
            FilDBInitObj.u32MemAddr = dma_getCacheAddr(fileDbbufAddr);
            FilDBInitObj.u32MemSize = fileDbbufSize;
            fileDbHandle = FileDB_Create(&FilDBInitObj);
        }
        // copy file one by one
        {
            UINT32                   i=0,CurrIndex;
            PFILEDB_FILE_ATTR        pFileAttr;

            CurrIndex = FileDB_GetCurrFileIndex(fileDbHandle);
            FileNumber = FileDB_GetTotalFileNum(fileDbHandle);
            for(i=0; i<FileNumber; i++)
            {
                pFileAttr = FileDB_SearhFile(fileDbHandle, i);
                CopyInfo.pSrcPath = pFileAttr->filePath;
                CopyInfo.pDstPath = pFileAttr->filePath;
                debug_msg("Copy file %s\r\n",pFileAttr->filePath);
                uiError_Code = FileSys_CopyToByName(&CopyInfo);
                if(uiError_Code != FST_STA_OK)
                {
                    if(uiError_Code == FST_STA_NOFREE_SPACE)
                    {
                        uiError_Code = WARN_CARD_FULL;
                    }
                    break;
                }
            }
            if (FileNumber)
            {
                FileDB_SearhFile(fileDbHandle, CurrIndex);
            }
        }
        FileDB_Release(fileDbHandle);
    }
    #else
    {
        //if (pOriObj==pSrcObj)
        if (pOriDXH==pSrcDXH)
        {
            // get copy file number
            FileNumber = DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT);

            // get dst max ID
            //FileSys_ChangeDisk(pDstObj);
            FileSys_ChangeDisk(pDstDXH);
            DCF_ScanObj();
            DCF_GetNextID(&NewDirID,&NewFileID);
            //FileSys_ChangeDisk(pOriObj);
            FileSys_ChangeDisk(pOriDXH);
            DCF_ScanObj();
        }
        // pOriObj == pDstObj
        else
        {
            // get dst max ID
            #if 0
            DCF_GetNextID(&NewDirID,&NewFileID);
            #else // Rescan DCF if not scanned yet.
            if(!DCF_GetNextID(&NewDirID,&NewFileID))
            {
                DCF_ScanObj();
                DCF_GetNextID(&NewDirID,&NewFileID);
            }
            #endif
            // get copy file number
            //FileSys_ChangeDisk(pSrcObj);
            FileSys_ChangeDisk(pSrcDXH);
            DCF_ScanObj();
            FileNumber = DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT);
        }
        if (NewDirID == ERR_DCF_DIR_NUM && NewFileID == ERR_DCF_FILE_NUM)
        {
            uiError_Code = WRN_EXCEED_MAX_FOLDER;
            goto L_CopyToCard_Exit;
        }
        switch(copMode)
        {
        case PB_COPY2CARD_NEW_DIR:
            if (NewFileID != MIN_DCF_FILE_NUM)
            {
                NewDirID++;
                NewFileID = MIN_DCF_FILE_NUM;
                if (NewDirID > MAX_DCF_DIR_NUM)
                {
                    uiError_Code = WRN_EXCEED_MAX_FOLDER;
                    goto L_CopyToCard_Exit;
                }
            }
            // continue run append code should not add break here
        case PB_COPY2CARD_APPEND:
            for (i =1;i<=FileNumber;i++)
            {
                // get src path
                DCF_SetCurIndex(i);
                DCF_GetObjInfo(i,&DirID, &FileID, &uiThisFileFormat);
                DCF_GetObjPath(i,uiThisFileFormat, srcPath);
                CopyInfo.pSrcPath = srcPath;

                // get dst path
                DCF_MakeObjPath(NewDirID, NewFileID, uiThisFileFormat, dstPath);
                CopyInfo.pDstPath = dstPath;
                uiError_Code = FileSys_CopyToByName(&CopyInfo);
                if(uiError_Code != FST_STA_OK)
                {
                    if(uiError_Code == FST_STA_NOFREE_SPACE)
                    {
                        uiError_Code = WARN_CARD_FULL;
                    }
                    break;
                }
                NewFileID++;
                if (NewFileID > MAX_DCF_FILE_NUM)
                {
                    NewFileID = 1;
                    NewDirID++;
                    if (NewDirID > MAX_DCF_DIR_NUM)
                    {
                        uiError_Code = WRN_EXCEED_MAX_FOLDER;
                        goto L_CopyToCard_Exit;
                    }
                }
            }
            break;

        case PB_COPY2CARD_NEW_DIR_ORG_FILE:
            {
                UINT32 tmpDirID  = 0;

                // Fix bug 0050970: Show no picture warning message if no file in Nand when doing copy to card function.
                if(!FileNumber)
                {
                    uiError_Code = WARN_COPY_NO_PICTURE ;
                    goto L_CopyToCard_Exit;
                }

                for (i =1;i<=FileNumber;i++)
                {
                    // get src path
                    DCF_SetCurIndex(i);
                    DCF_GetObjInfo(i,&DirID, &FileID, &uiThisFileFormat);
                    DCF_GetObjPath(i,uiThisFileFormat, srcPath);
                    CopyInfo.pSrcPath = srcPath;

                    // check if need to new dir
                    if (tmpDirID!=DirID)
                    {
                        tmpDirID = DirID;
                        NewDirID++;
                        if (NewDirID > MAX_DCF_DIR_NUM)
                        {
                            uiError_Code = WRN_EXCEED_MAX_FOLDER;
                            goto L_CopyToCard_Exit;
                        }
                    }

                    // get dst path
                    DCF_MakeObjPath(NewDirID, FileID, uiThisFileFormat, dstPath);
                    CopyInfo.pDstPath = dstPath;
                    uiError_Code = FileSys_CopyToByName(&CopyInfo);
                    if(uiError_Code != FST_STA_OK)
                    {
                        if(uiError_Code == FST_STA_NOFREE_SPACE)
                        {
                            uiError_Code = WARN_CARD_FULL;
                        }

                        break;
                    }
                }
            }
            break;
        case PB_COPY2CARD_NO_RENAME:

        default:
            debug_msg("Not Support Copy command %d\r\n",copMode);
            break;
        }
    }
L_CopyToCard_Exit:
    #endif

    if (pOriDXH!=pSrcDXH)
    {
        FileSys_ChangeDisk(pOriDXH);
    }

    #if USE_FILEDB
    {
        UINT32  curIndex;

        fileDbHandle = 0;
        FileDB_Refresh(fileDbHandle);
        FileNumber = FileDB_GetTotalFileNum(fileDbHandle);
        curIndex = FileNumber -1;
        if (FileNumber)
        {
            FileDB_SearhFile(fileDbHandle, curIndex);
        }
        if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK)
        {
            PB_OpenSpecFileBySeq(curIndex+1,TRUE);
            PB_WaitCommandFinish(PB_WAIT_INFINITE);
        }
    }
    #else
    {
        DCF_Refresh();
        DCF_SetCurIndex(DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT));

        if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK)
        {
            PB_OpenSpecFileBySeq(DCF_GetDBInfo(DCF_INFO_CUR_INDEX),TRUE);
            PB_WaitCommandFinish(PB_WAIT_INFINITE);
        }
    }
    #endif

    UI_SetData(FL_IsCopyToCarding, FALSE);
    return uiError_Code;
}




void BackgroundWait_PrintModeIsClosed(UINT32 isClosed)
{
    gUIPrtModeIsClosed = isClosed;
}
//#NT#2010/11/09#Lily Kao -end

UINT32 BackgroundDelAll(void)
{
    INT32 ret;

    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_DELETE, 1, PB_DELETE_ALL);
    ret = TRUE;//dummy return value
    //debug_err(("DelAll ret =%d \r\n",ret));

     return (UINT32)ret;
}

UINT32 BackgroundCopy2Card(void)
{
    return Background_CopyTo(TRUE,PB_COPY2CARD_NEW_DIR);
}

UINT32 BackgroundCopy2Nand(void)
{
    return Background_CopyTo(FALSE,PB_COPY2CARD_NEW_DIR);
}

#if(COPYCARD2CARD_FUNCTION == ENABLE)
UINT32 Background_CopyCardToCard(BOOL IsPrimaryCard,UINT32 copMode)
{
    UINT32 uiTempBufAddr, uiTempBufSize, uiTempBufEnd, FileNumber;
    INT32  uiError_Code = FST_STA_OK;
    //HNVT_STRG pSrcObj, pDstObj, pOriObj;
    DX_HANDLE pSrcDXH, pDstDXH, pOriDXH;
    COPYTO_BYNAME_INFO CopyInfo;
    #if USE_FILEDB
    FILEDB_HANDLE fileDbHandle, fileDbbufSize=0x200000, fileDbbufAddr;
    #else
    UINT32 uiThisFileFormat;
    UINT32 DirID,FileID;
    UINT32 NewDirID,NewFileID,i;
    CHAR   srcPath[DCF_FULL_FILE_PATH_LEN],dstPath[DCF_FULL_FILE_PATH_LEN];
    #endif


    uiTempBufSize = 0x100000;
    uiTempBufAddr = SxCmd_GetTempMem(uiTempBufSize);
    uiTempBufEnd = OS_GetMempoolAddr(POOL_ID_APP)+ POOL_SIZE_APP;
    uiTempBufSize = uiTempBufEnd - uiTempBufAddr;

    // reserved 2MB for FileDB using
    #if USE_FILEDB
    uiTempBufSize -=fileDbbufSize;
    fileDbbufAddr = uiTempBufAddr+uiTempBufSize;
    #endif

    UI_SetData(FL_IsCopyToCarding, TRUE);

    if (IsPrimaryCard)
    {
        FileSys_GetStrgObj(&pDstDXH);
        pOriDXH = pDstDXH;
        {
            pSrcDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD2);
        }
    }
    else
    {
        FileSys_GetStrgObj(&pOriDXH);
        pSrcDXH = pOriDXH;
        {
            pDstDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD2);
        }
    }

    memset(&CopyInfo, 0, sizeof(CopyInfo));
    CopyInfo.pSrcStrg = pSrcDXH;
    CopyInfo.pDstStrg = pDstDXH;
    CopyInfo.pBuf = (char *)uiTempBufAddr;
    CopyInfo.uiBufSize = uiTempBufSize;

    #if USE_FILEDB
    {
        if (pOriDXH!=pSrcDXH)
        {
            FileSys_ChangeDisk(pSrcDXH);
        }
        //  create fileDB for copy
        {
            FILEDB_INIT_OBJ   FilDBInitObj={0};
            CHAR              rootPath[20]="A:\\";

            FilDBInitObj.rootPath = rootPath;
            FilDBInitObj.bIsRecursive = TRUE;
            FilDBInitObj.bIsCyclic = TRUE;
            FilDBInitObj.bIsMoveToLastFile = TRUE;
            FilDBInitObj.bIsSupportLongName = TRUE;
            FilDBInitObj.bIsDCFFileOnly = FALSE;
            FilDBInitObj.bIsChkHasFile = FALSE;
            FilDBInitObj.u32MaxFilePathLen = 60;
            FilDBInitObj.u32MaxFileNum = 20000;
            FilDBInitObj.fileFilter = FILEDB_FMT_JPG|FILEDB_FMT_AVI|FILEDB_FMT_MOV|FILEDB_FMT_MP4;
            FilDBInitObj.u32MemAddr = dma_getCacheAddr(fileDbbufAddr);
            FilDBInitObj.u32MemSize = fileDbbufSize;
            fileDbHandle = FileDB_Create(&FilDBInitObj);
        }
        // copy file one by one
        {
            UINT32                   i=0,CurrIndex;
            PFILEDB_FILE_ATTR        pFileAttr;

            CurrIndex = FileDB_GetCurrFileIndex(fileDbHandle);
            FileNumber = FileDB_GetTotalFileNum(fileDbHandle);
            for(i=0; i<FileNumber; i++)
            {
                pFileAttr = FileDB_SearhFile(fileDbHandle, i);
                CopyInfo.pSrcPath = pFileAttr->filePath;
                CopyInfo.pDstPath = pFileAttr->filePath;
                debug_msg("Copy file %s\r\n",pFileAttr->filePath);
                uiError_Code = FileSys_CopyToByName(&CopyInfo);

                if(uiError_Code != FST_STA_OK)
                {
                    if(uiError_Code == FST_STA_NOFREE_SPACE)
                    {
                        uiError_Code = WARN_CARD_FULL;
                    }
                    break;
                }
            }
            if (FileNumber)
            {
                FileDB_SearhFile(fileDbHandle, CurrIndex);
            }
        }
        FileDB_Release(fileDbHandle);
    }
    #else
    {
        //if (pOriObj==pSrcObj)
        if (pOriDXH==pSrcDXH)
        {
            // get copy file number
            FileNumber = DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT);

            // get dst max ID
            //FileSys_ChangeDisk(pDstObj);
            FileSys_ChangeDisk(pDstDXH);
            DCF_ScanObj();
            DCF_GetNextID(&NewDirID,&NewFileID);
            //FileSys_ChangeDisk(pOriObj);
            FileSys_ChangeDisk(pOriDXH);
            DCF_ScanObj();
        }
        // pOriObj == pDstObj
        else
        {
            // get dst max ID
            #if 0
            DCF_GetNextID(&NewDirID,&NewFileID);
            #else // Rescan DCF if not scanned yet.
            if(!DCF_GetNextID(&NewDirID,&NewFileID))
            {
                DCF_ScanObj();
                DCF_GetNextID(&NewDirID,&NewFileID);
            }
            #endif
            // get copy file number
            //FileSys_ChangeDisk(pSrcObj);
            FileSys_ChangeDisk(pSrcDXH);
            DCF_ScanObj();

            FileNumber = DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT);
        }
        if (NewDirID == ERR_DCF_DIR_NUM && NewFileID == ERR_DCF_FILE_NUM)
        {
            uiError_Code = WRN_EXCEED_MAX_FOLDER;
            goto L_CopyToCard_Exit;
        }
        switch(copMode)
        {
        case PB_COPY2CARD_NEW_DIR:
            if (NewFileID != MIN_DCF_FILE_NUM)
            {
                NewDirID++;
                NewFileID = MIN_DCF_FILE_NUM;
                if (NewDirID > MAX_DCF_DIR_NUM)
                {
                    uiError_Code = WRN_EXCEED_MAX_FOLDER;
                    goto L_CopyToCard_Exit;
                }
            }
            // continue run append code should not add break here
        case PB_COPY2CARD_APPEND:
            for (i =1;i<=FileNumber;i++)
            {
                // get src path
                DCF_SetCurIndex(i);
                DCF_GetObjInfo(i,&DirID, &FileID, &uiThisFileFormat);
                DCF_GetObjPath(i,uiThisFileFormat, srcPath);
                CopyInfo.pSrcPath = srcPath;

                // get dst path
                DCF_MakeObjPath(NewDirID, NewFileID, uiThisFileFormat, dstPath);
                CopyInfo.pDstPath = dstPath;
                uiError_Code = FileSys_CopyToByName(&CopyInfo);
                if(uiError_Code != FST_STA_OK)
                {
                    if(uiError_Code == FST_STA_NOFREE_SPACE)
                    {
                        uiError_Code = WARN_CARD_FULL;
                    }
                    break;
                }
                NewFileID++;
                if (NewFileID > MAX_DCF_FILE_NUM)
                {
                    NewFileID = 1;
                    NewDirID++;
                    if (NewDirID > MAX_DCF_DIR_NUM)
                    {
                        uiError_Code = WRN_EXCEED_MAX_FOLDER;
                        goto L_CopyToCard_Exit;
                    }
                }
            }
            break;

        case PB_COPY2CARD_NEW_DIR_ORG_FILE:
            {
                UINT32 tmpDirID  = 0;

                // Fix bug 0050970: Show no picture warning message if no file in Nand when doing copy to card function.
                if(!FileNumber)
                {
                    uiError_Code = WARN_COPY_NO_PICTURE ;
                    goto L_CopyToCard_Exit;
                }

                for (i =1;i<=FileNumber;i++)
                {
                    // get src path
                    DCF_SetCurIndex(i);
                    DCF_GetObjInfo(i,&DirID, &FileID, &uiThisFileFormat);
                    DCF_GetObjPath(i,uiThisFileFormat, srcPath);
                    CopyInfo.pSrcPath = srcPath;

                    // check if need to new dir
                    if (tmpDirID!=DirID)
                    {
                        tmpDirID = DirID;
                        NewDirID++;
                        if (NewDirID > MAX_DCF_DIR_NUM)
                        {
                            uiError_Code = WRN_EXCEED_MAX_FOLDER;
                            goto L_CopyToCard_Exit;
                        }
                    }

                    // get dst path
                    DCF_MakeObjPath(NewDirID, FileID, uiThisFileFormat, dstPath);
                    CopyInfo.pDstPath = dstPath;
                    uiError_Code = FileSys_CopyToByName(&CopyInfo);
                    if(uiError_Code != FST_STA_OK)
                    {
                        if(uiError_Code == FST_STA_NOFREE_SPACE)
                        {
                            uiError_Code = WARN_CARD_FULL;
                        }

                        break;
                    }
                }
            }
            break;
        case PB_COPY2CARD_NO_RENAME:

        default:
            debug_msg("Not Support Copy command %d\r\n",copMode);
            break;
        }
    }
L_CopyToCard_Exit:
    #endif

    if (pOriDXH!=pSrcDXH)
    {
        FileSys_ChangeDisk(pOriDXH);
    }

    #if USE_FILEDB
    {
        UINT32  curIndex;

        fileDbHandle = 0;
        FileDB_Refresh(fileDbHandle);
        FileNumber = FileDB_GetTotalFileNum(fileDbHandle);

        curIndex = FileNumber -1;
        if (FileNumber)
        {
            FileDB_SearhFile(fileDbHandle, curIndex);
        }
        if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK)
        {
            PB_OpenSpecFileBySeq(curIndex+1,TRUE);
            PB_WaitCommandFinish(PB_WAIT_INFINITE);
        }
    }
    #else
    {
        DCF_Refresh();
        DCF_SetCurIndex(DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT));

        if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK)
        {
            PB_OpenSpecFileBySeq(DCF_GetDBInfo(DCF_INFO_CUR_INDEX),TRUE);
            PB_WaitCommandFinish(PB_WAIT_INFINITE);
        }
    }
    #endif

    UI_SetData(FL_IsCopyToCarding, FALSE);
    return uiError_Code;
}

UINT32 BackgroundCopyCard1ToCard2(void)
{
    return Background_CopyCardToCard(FALSE,PB_COPY2CARD_NEW_DIR);
}

UINT32 BackgroundCopyCard2ToCard1(void)
{
    return Background_CopyCardToCard(TRUE,PB_COPY2CARD_NEW_DIR);
}
#endif


#if _DEMO_TODO
//#NT#2009/07/01#Brad Chen -begin
//#NT#Add to support MSDC CDROM function
ER copy_iso(void)
{
    FST_FILE    pFile;
    //UINT32      uiFileSize;
    UINT32      sectNum;
    UINT32      copySectCnt=64*32; //512(bytes)*64(sectors)*32 = 1MB
    UINT32      copyCnt, i ,startSect, readSect;
    CHAR        *pBuf;
    char        *fileName = "A:\\GEISO.ISO";
    PSTRG_TAB   nandObj;
    PSTRG_TAB   sdmmcObj;
    //int time ;
    //Perf_Open();
    //Perf_Mark();
    DX_HANDLE pStrgDev = 0;
    DX_HANDLE pStrgDev2 = 0;
    pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1);
    pStrgDev2 = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND2);

    pBuf = (CHAR *)OS_GetMempoolAddr(POOL_ID_APP);

    pFile = FileSys_OpenFile((char *)fileName,FST_OPEN_READ);
    if (pFile == NULL)
    {
        debug_err(("open file error\r\n"));
        return E_NOMEM;
    }

    // the number of sectors to copy a file
    sectNum = (pFile->fileSize/512);
    if (pFile->fileSize%512)
        sectNum++;

    // Copy the "copySectCnt" numbers of sectors once
    copyCnt = sectNum/copySectCnt;
    if (sectNum%copySectCnt)
        copyCnt++;

    debug_err(("size: file(0x%x) pool(0x%x) sectors(%d)\r\n", pFile->fileSize, POOL_SIZE_APP, sectNum));
    FileSys_CloseFile(pFile);

    // Start to copy
    for (i=0;i<copyCnt;i++)
    {
        pFile = FileSys_OpenFile((char *)fileName,FST_OPEN_READ);
        if (pFile == NULL)
        {
            debug_err(("open file error\r\n"));
            return E_SYS;
        }

        startSect = copySectCnt*i;
        if (i < copyCnt-1)
            readSect = copySectCnt;
        else
            readSect = sectNum-(copySectCnt*i);

        debug_err(("%d/%d/%d\r\n",i,copyCnt,readSect));

        // Read to memory
        FileSys_SeekFile(pFile, startSect*512, FST_SEEK_SET);
        FileSys_ReadFile(pFile, (UINT8 *)pBuf, readSect*512, 0, NULL);
        FileSys_CloseFile(pFile);

        sdmmcObj = (PSTRG_TAB)Dx_Getcaps((UINT32)pStrgDev, STORAGE_CAPS_HANDLE, 0);
        sdmmcObj->Close();

        // Copy to flash
        nandObj = (PSTRG_TAB)Dx_Getcaps((UINT32)pStrgDev2, STORAGE_CAPS_HANDLE, 0);
        nandObj->Open();
        nandObj->WrSectors(pBuf, startSect, readSect);
        nandObj->RdSectors(pBuf+(copySectCnt*512), startSect, readSect);
        if(memcmp(pBuf,(pBuf+(copySectCnt*512)), readSect*512 ) == 0)
        {
            debug_err(("check ok\r\n"));
        }
        else
        {
            debug_err(("^R burn %s NG\r\n",fileName));
            nandObj->Close();

            sdmmcObj->Open();

            return E_SYS;
        }
        nandObj->Close();

        sdmmcObj->Open();

    }
    //time = Perf_GetDuration();
    //debug_err(("time = %d ms \r\n",time/1000));
    //Perf_Close();
    return E_OK;
}
//#NT#2009/07/01#Brad Chen -end

//#NT#2009/07/01#Brad Chen -begin
//#NT#Add to support MSDC CDROM function
ER check_iso(UINT32 addr,UINT32 value,UINT32 Length)
{
    CHAR        *pBuf;
    PSTRG_TAB   nandObj;
    PSTRG_TAB   sdmmcObj;
    ER result = E_OK;
    DX_HANDLE pStrgDev = 0;
    DX_HANDLE pStrgDev2 = 0;
    pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1);
    pStrgDev2 = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND2);

    pBuf = (CHAR *)OS_GetMempoolAddr(POOL_ID_APP);

    sdmmcObj = (PSTRG_TAB)Dx_Getcaps((UINT32)pStrgDev, STORAGE_CAPS_HANDLE, 0);
    sdmmcObj->Close();

    // Copy to flash
    nandObj = (PSTRG_TAB)Dx_Getcaps((UINT32)pStrgDev2, STORAGE_CAPS_HANDLE, 0);
    nandObj->Open();
    nandObj->RdSectors(pBuf, 0, (addr/512)+1);
    if(*(UINT32 *)(pBuf+addr)!=value)
    {
        result = E_SYS;
        debug_msg("ISO 0X%X 0X%X\r\n",*(UINT32 *)(pBuf+addr),value);
    }

    nandObj->Close();
    sdmmcObj->Open();

    if(result)
        return E_SYS;

    return E_OK;
}
//#NT#2009/07/01#Brad Chen -end

//#NT#2011/02/14#Brad Chen -begin
//#NT# Format nand
void FormatNand(void)
{
    //UINT8  StringBuf[64];
    //DX_OBJECT* pStrgDev1 = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1);
    //PSTRG_TAB pStrgObj = (PSTRG_TAB)Dx_Getcaps(pStrgDev1, STORAGE_CAPS_HANDLE, 0);
    DX_HANDLE pStrgDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1);
    //if (FileSys_FormatDisk(pStrgObj, TRUE) == E_OK)
    if (FileSys_FormatDisk(pStrgDXH, TRUE) == E_OK)
    {
    }
    else
    {
        debug_msg("format nand error\r\n");
        //while(1)
            ;
    }
    //#NT#2009/04/19#JJ Huang -begin
    //if (SDIOCardUser_CheckCardInserted())
    if (DrvCARD_DetStrgCard())
    {
        debug_msg("change to card\r\n");
        GxStrg_OpenDevice(0, (UINT32)pStrgDev2);
        SwTimer_DelayMs(1000);
    }
    //#NT#2009/04/19#JJ Huang -end
}
//#NT#2011/02/14#Brad Chen -end

//#NT#2009/09/14#Brad Chen -begin
//#NT#Add copy file from SD to Nand
ER copy_filetoNand(void)
{
    #if !USE_DCF_LIB
    char * pBuf ;
    UINT32 uiTempBufAddr, uiTempBufSize, FileNumber;
    INT32  uiError_Code;
    PFILE_TSK_PARAM     pParam;
    DX_HANDLE pStrgDev1 = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1);
    PSTRG_TAB pWhichObj;
    PSTRG_TAB pStrgObj;

    pBuf = (char *)OS_GetMempoolAddr(POOL_ID_APP);

    uiTempBufAddr = (UINT32) pBuf;
    uiTempBufSize = FILE_SIZE_LIMITATION;

    FileSys_GetStrgObj(&pWhichObj);
    pStrgObj = (PSTRG_TAB)Dx_Getcaps(pStrgDev1, STORAGE_CAPS_HANDLE, 0);
    uiError_Code = FilesysCopyTo(pWhichObj, pStrgObj, (char *)uiTempBufAddr, uiTempBufSize, FST_COPYTO_APPEND,  &FileNumber,NULL, FALSE);

    // get new info
    pParam = (PFILE_TSK_PARAM)FilesysGetFSTParam();
    //gMenuPlayInfo.DispFileName[gMenuPlayInfo.CurFileIndex - 1] = pParam->FilenameID;
    //gMenuPlayInfo.DispDirName[gMenuPlayInfo.CurFileIndex - 1]  = pParam->DirectoryID;

    if(uiError_Code != FST_STA_OK)
    {
        return E_SYS;
    }
    #endif
    return E_OK;
}
//#NT#2009/09/14#Brad Chen -end
#endif

UINT32 BackgroundFormat(void)
{
    int ret;

    //HNVT_STRG   pWhichObj;
    DX_HANDLE   pStrgDXH;

    //FileSys_GetStrgObj(&pWhichObj);
    FileSys_GetStrgObj(&pStrgDXH);
    //ret = FileSys_FormatDisk(pWhichObj, FALSE);
    ret = FileSys_FormatDisk(pStrgDXH, FALSE);

    if(ret==FST_STA_OK)
    {
        // reset file ID (for FileDB)
        #if USE_FILEDB
        if (UI_GetData(FL_IsUseFileDB))
        {
            Ux_SendEvent(&UISetupObjCtrl,NVTEVT_FILEID_RESET,0);
        }
        #endif

        //reset next id 100 , 1 (for DCF)
        DCF_SetNextID(MIN_DCF_DIR_NUM,MIN_DCF_FILE_NUM);
        UI_SetData(FL_DCF_DIR_ID,MIN_DCF_DIR_NUM);
        UI_SetData(FL_DCF_FILE_ID,MIN_DCF_FILE_NUM);

        UI_SetData(FL_IsDCIMReadOnly,FALSE);
        if(System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK)
        {
            Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYSINGLE, 1, PB_SINGLE_CURR);
        }
    }
    return (UINT32)ret;
}

UINT32 BackgroundFormatCard(void)
{
    int ret;

    //DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1);
    //UINT32 hStrgObj = Dx_Getcaps(pStrgDev, STORAGE_CAPS_HANDLE, 0);
    //ret = FileSys_FormatDisk((HNVT_STRG)hStrgObj, FALSE);
    DX_HANDLE pStrgDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1);
    ret = FileSys_FormatDisk(pStrgDXH, FALSE);

    if(ret==FST_STA_OK)
    {
        // reset file ID (for FileDB)
        #if USE_FILEDB
        if (UI_GetData(FL_IsUseFileDB))
        {
            Ux_SendEvent(&UISetupObjCtrl,NVTEVT_FILEID_RESET,0);
        }
        #endif

        //reset next id 100 , 1 (for DCF)
        DCF_SetNextID(MIN_DCF_DIR_NUM,MIN_DCF_FILE_NUM);
        UI_SetData(FL_DCF_DIR_ID,MIN_DCF_DIR_NUM);
        UI_SetData(FL_DCF_FILE_ID,MIN_DCF_FILE_NUM);

        UI_SetData(FL_IsDCIMReadOnly,FALSE);
        if(System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK)
        {
            Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYSINGLE, 1, PB_SINGLE_CURR);
        }
    }
    return (UINT32)ret;
}
UINT32 BackgroundFormatNand(void)
{
    int ret;

    //DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1);
    //UINT32 hStrgObj = Dx_Getcaps(pStrgDev, STORAGE_CAPS_HANDLE, 0);
    //ret = FileSys_FormatDisk((HNVT_STRG)hStrgObj, FALSE);
    DX_HANDLE pStrgDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1);
    ret = FileSys_FormatDisk(pStrgDXH, FALSE);

    if(ret==FST_STA_OK)
    {
        // reset file ID (for FileDB)
        #if USE_FILEDB
        if (UI_GetData(FL_IsUseFileDB))
        {
            Ux_SendEvent(&UISetupObjCtrl,NVTEVT_FILEID_RESET,0);
        }
        #endif

        //reset next id 100 , 1 (for DCF)
        DCF_SetNextID(MIN_DCF_DIR_NUM,MIN_DCF_FILE_NUM);
        UI_SetData(FL_DCF_DIR_ID,MIN_DCF_DIR_NUM);
        UI_SetData(FL_DCF_FILE_ID,MIN_DCF_FILE_NUM);

        UI_SetData(FL_IsDCIMReadOnly,FALSE);
        if(System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK)
        {
            Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYSINGLE, 1, PB_SINGLE_CURR);
        }
    }
    return (UINT32)ret;
}

UINT32 BackgroundNumReset(void)
{
    UINT32 fid=0,did=0;

    //#Get DCF Folder/FIle ID
    DCF_GetNextID((UINT32 *)&did,(UINT32 *)&fid);
    debug_err(("SetupExe_OnNumReset(), DirNum = %d, FileNum = %d\r\n",did,fid));

    if( did < MAX_DCF_DIR_NUM )
    {
        //reset
        DCF_SetNextID(did+1,1);

        //#Get DCF Folder/FIle ID
        DCF_GetNextID((UINT32 *)&did,(UINT32 *)&fid);

        SwTimer_DelayMs(0x2000); //for ui show wait window for 2 second
        debug_err(("SetupExe_OnNumReset(), DirNum = %d, FileNum = %d\r\n",did,fid));

        return TRUE;
    }
    else
    {
        debug_err(("did over %d\r\n",MAX_DCF_DIR_NUM));
        return FALSE;
    }

}
UINT32 BackgroundSetDPOF(void)
{
    INT32 ret;

    if(g_uiDpofOPMode == PLAYDPOF_SETONE)
        //Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_SET_DPOFONE,2, g_uiDpofPrtNum, g_uiDpofDateOn);
        DpofVendor_SetDPOFQty(DPOF_OP_SET_CURR, g_uiDpofPrtNum, g_uiDpofDateOn);
    else
    {
        //#NT#2010/05/31#[0010985]Lily Kao -begin
        //#NT#DPOF-All from the current or the first picture
        //Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_SET_DPOFALL_FROM_FIRST,2, g_uiDpofPrtNum, g_uiDpofDateOn);
        //#NT#2010/05/31#[0010985]Lily Kao -end
        DpofVendor_SetDPOFQty(DPOF_OP_SET_ALL, g_uiDpofPrtNum, g_uiDpofDateOn);
    }
    ret = TRUE;//dummy return value
    return (UINT32)ret;
}

UINT32 BackgroundSetProtect(void)
{
    INT32 ret;
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PROTECT,1,g_uiProtectType);
    ret = TRUE;//dummy return value
    return (UINT32)ret;
}

UINT32 BackgroundWaitPrintInit(void)
{
#if _DEMO_TODO
    UINT32    uiUSBFlag;
    UINT8 notFirst = 1;
    UINT32 notResponseTime =0;

    debug_msg("BackgroundWaitPrintInit\r\n ");
    //#NT#2010/11/09#Lily Kao -begin
    BackgroundWait_PrintModeIsClosed(FALSE);
    //#NT#2010/11/09#Lily Kao -end
    //clr_flg(FLG_DPS, FLGDPS_INITOK|FLGDPS_INITFAIL|FLGDPS_FIRSTPTPCMD);
    PictB_ClrFlag(FLGDPS_INITOK|FLGDPS_INITFAIL|FLGDPS_FIRSTPTPCMD);
    while(notFirst)
    {
        uiUSBFlag = PictB_ChkFlg(FLGDPS_FIRSTPTPCMD);
        if ((uiUSBFlag & FLGDPS_FIRSTPTPCMD))
        {
            //clr_flg(FLG_DPS, FLGDPS_FIRSTPTPCMD);
            PictB_ClrFlag(FLGDPS_FIRSTPTPCMD);
            notFirst = 0;
        }
        else
        {
            SwTimer_DelayMs(1000);
            notResponseTime++;
            //#NT#2010/11/09#Lily Kao -begin
            if (gUIPrtModeIsClosed || (notResponseTime > NVT_PTR_RESPOSE_TIME) )
            //#NT#2010/11/09#Lily Kao -end
            {
                return FLGDPS_INITFAIL;
            }
        }
    }

    uiUSBFlag = PictB_ChkFlg(FLGDPS_INITOK|FLGDPS_INITFAIL);
    if (uiUSBFlag == 0)
    {
        notResponseTime = 0;
        while((notResponseTime < NVT_PC_RESPOSE_TIME))
        {
            uiUSBFlag = PictB_ChkFlg(FLGDPS_INITOK|FLGDPS_INITFAIL);
            if(uiUSBFlag)
            {
                break;
            }
            notResponseTime ++;

            //#NT#2010/11/09#Lily Kao -begin
            if (gUIPrtModeIsClosed)
            {
                debug_err(("PrtMode Close2:%d\r\n",gUIPrtModeIsClosed));
                notResponseTime = NVT_PC_RESPOSE_TIME;
                break;
            }
            //#NT#2010/11/09#Lily Kao -end
            SwTimer_DelayMs(1000);
        }
        if (notResponseTime == NVT_PC_RESPOSE_TIME)
        {
            return FLGDPS_INITFAIL;
        }
    }

    if (uiUSBFlag & FLGDPS_INITFAIL)
    {
        notResponseTime= 0;
        //clr_flg(FLG_DPS, FLGDPS_INITOK|FLGDPS_INITFAIL);
        PictB_ClrFlag(FLGDPS_INITOK|FLGDPS_INITFAIL);

        while (notResponseTime < NVT_PTR_RESPOSE_TIME)
        {
            SwTimer_DelayMs(1000);
            uiUSBFlag = PictB_ChkFlg(FLGDPS_INITOK);
            debug_err(("Init Fail, wait flag again:uiUSBFlag=%x, notResponseTime=%x\r\n", uiUSBFlag, notResponseTime));
            if (uiUSBFlag & FLGDPS_INITOK)
            {
                break;
            }
            else
            {
                notResponseTime++;
            }
            //#NT#2010/11/09#Lily Kao -begin
            if (gUIPrtModeIsClosed)
            {
                debug_err(("PrtMode Close3:%d\r\n",gUIPrtModeIsClosed));
                notResponseTime = NVT_PTR_RESPOSE_TIME;
                break;
            }
            //#NT#2010/11/09#Lily Kao -end
        }
        if (notResponseTime == NVT_PTR_RESPOSE_TIME)
        {
            return FLGDPS_INITFAIL;
        }
    }
    //clr_flg(FLG_DPS, FLGDPS_INITOK|FLGDPS_INITFAIL);
    PictB_ClrFlag(FLGDPS_INITOK|FLGDPS_INITFAIL);
    if (uiUSBFlag & FLGDPS_INITOK)
    {
        debug_msg("Wait PrintInit ok !!!\r\n ");
        if (GxUSB_GetIsUSBPlug() == TRUE)
        {
            if(SIDC_PTR == AppPrint_GetData(PRT_SIDC_TYPE))
            {
                debug_msg("Run CAPS INIT !!!\r\n ");
                Ux_SendEvent(&UIPrinterObjCtrl,NVTEVT_PRTCAPSINIT,0);
            }
            return FLGDPS_INITOK;
        }
        else
        {
            debug_err(("USB Unplug\r\n"));
            return FLGDPS_INITFAIL;
        }
    }
    else
    {
        debug_msg("Wait PrintInit Fail !!!\r\n ");
        return FLGDPS_INITFAIL;
    }
#else
    return TRUE;
#endif
}

UINT32 BackgroundResize(void)
{
    INT32 ret = TRUE;
#if _DEMO_TODO
    if(g_uiNewWidth == 0 || g_uiNewHeight == 0)
    {
        debug_err(("NVTEVT_BKW_RESIZE PARAMETER ERROR!!!\r\n"));
        return NVTEVT_CONSUME;
    }
    debug_msg("[BackgroundResize] NewWidth =%d, NewHeight=%d, OverWrite=%d\r\n",g_uiNewWidth,g_uiNewHeight,g_bOverWrite);

    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_RESIZE, 3, g_uiNewWidth, g_uiNewHeight, g_bOverWrite);
    ret = TRUE;//dummy return value
#endif
    return ret;
}

UINT32 BackgroundFixRedEye(void)
{
    INT32 ret;
    #if 0
    Ux_SendEvent(&UIPlayObjCtrl, NVTEVT_FIXREDEYE, 3, PBEDIT_CUSTOMIZE_DISP, PBXEdit_RedEye_Remove, 0x100000);
    #endif
    ret = TRUE;//dummy return value
    return ret;
}

UINT32 BackgroundReSave(void)
{
    #if 0
    INT32 ret;

    Ux_SendEvent(&UIPlayObjCtrl, NVTEVT_FIXREDEYE, 3, PBEDIT_CUSTOMIZE_DISP, PBXEdit_RedEye_Remove, 0x100000);
    if(PB_STA_ERR_DECODE == AppPlay_GetData(PLAY_PBSTATUS))
    {
        //not detect red eye
        ret = FALSE;
    }
    else
    {
        if(g_bOverWrite)
        {
            Ux_SendEvent(&UIPlayObjCtrl, NVTEVT_FIXREDEYE, 4, PBEDIT_CUSTOMIZE, PBXEdit_RedEye_Remove, 0x100000, TRUE);
        }
        else
        {
            Ux_SendEvent(&UIPlayObjCtrl, NVTEVT_FIXREDEYE, 4, PBEDIT_CUSTOMIZE, PBXEdit_RedEye_Remove, 0x100000, FALSE);
        }

        ret = TRUE;
    }
    return ret;
    #else
    return FALSE;
    #endif
}

//#NT#2009/08/25#Ben Wang -begin
UINT32 BackgroundNovaLightFix(void)
{
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_NOVALIGHT_FIX, 1, g_bOverWrite);
    return 0;
}


UINT32 BackgroundDummyWait_PrinterCheck(void)
{
#if (UI_STYLE!=UI_STYLE_DRIVER)
    if (AppPrint_GetData(PRT_SIDC_TYPE) == SIDC_PTR)
    {
        //debug_msg("#PTP Connect to Printer\r\n");
        return TRUE;
    }
    else
    {
        //debug_msg("#PTP Connect ...\r\n");
        return FALSE;
    }
#else
    return FALSE;
#endif
}

UINT32 BackgroundDummyWait_Stop(void)
{
    //debug_msg("BackgroundDummyWait_Stop:%d,%d\r\n", gUIStopBckGndDummyWait, g_uiWaitTime);
    if (g_uiWaitTime)
    {
        gUIStopBckGndDummyWait = TRUE;
    }
    return g_uiWaitTime;
}
//#NT#2010/06/23#Lily Kao -begin
UINT32 BackgroundDummyWait(void)
{
    UINT32 i = g_uiWaitTime;

    gUIStopBckGndDummyWait = FALSE;
    while (i)
    {
        //debug_msg("BackgroundDummyWait:%d,%d,%d\r\n", gUIStopBckGndDummyWait, g_uiWaitTime,i);
        Delay_DelayMs(1);
        i--;
        if (gUIStopBckGndDummyWait == TRUE)
        {
            //debug_msg("BackgroundDummyWait break:%d,%d\r\n", gUIStopBckGndDummyWait, g_uiWaitTime);
            gUIStopBckGndDummyWait = FALSE;
            g_uiWaitTime = i = 0;
            break;
        }
    }
    return 1;
}

UINT32 BackgroundDummyWaitUSB(void)
{
    UINT32 i = g_uiWaitTime;

    gUIStopBckGndDummyWait = FALSE;
    while (i || (g_uiWaitTime == UI_BACKGND_DUMMY_WAIT_INFINITE))
    {
        //debug_msg("BackgroundDummyWait:%d,%d,%d\r\n", gUIStopBckGndDummyWait, g_uiWaitTime,i);
        Delay_DelayMs(1);
        i--;
        if (gUIStopBckGndDummyWait == TRUE)
        {
            //debug_msg("BackgroundDummyWaitUSB break:%d,%d\r\n", gUIStopBckGndDummyWait, g_uiWaitTime);
            gUIStopBckGndDummyWait = FALSE;
            g_uiWaitTime = i = 0;
            break;
        }
        if ((g_uiWaitTimeChkPtr == TRUE) && (g_uiWaitTime == UI_BACKGND_DUMMY_WAIT_INFINITE))
        {
            if (BackgroundDummyWait_PrinterCheck() == TRUE)
            {
                gUIStopBckGndDummyWait = FALSE;
                g_uiWaitTime = i = 0;
                g_uiWaitTimeChkPtr = FALSE;
                break;
            }
            //#NT#2010/12/20#Lily Kao -begin
            #if (USB_CHARGE_FUNCTION == ENABLE)
            if (((g_uiWaitTime - i) > UI_BACKGND_DUMMY_WAIT_MSDC) && (AppPrint_GetData(PRT_SIDC_TYPE) == SIDC_UNKNOWN))
            {
                gUIStopBckGndDummyWait = FALSE;
                g_uiWaitTime = i = 0;
                g_uiWaitTimeChkPtr = FALSE;
                break;
            }
            #endif
            //#NT#2010/12/20#Lily Kao -end
        }
    }
    return 1;
}
//#NT#2010/06/23#Lily Kao -end
//#NT#2010/10/01#Ben Wang -begin
//#NT#Add deleting by date
UINT32 BackgroundDelFolder(void)
{
    INT32 ret = TRUE;
#if _DEMO_TODO
    UINT16   Year, Month, Day;

    Year = (INT32)PB_GET_YEAR_FROM_DATE(PB_GetCurrPlayDay());
    Month = (INT32)PB_GET_MONTH_FROM_DATE(PB_GetCurrPlayDay());
    Day = (INT32)PB_GET_DAY_FROM_DATE(PB_GetCurrPlayDay());

    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_DELETE_BY_DATE, 3, Year, Month, Day);
    ret = TRUE;//dummy return value
#endif

    return (UINT32)ret;
}
//#NT#2010/10/01#Ben Wang -end
//#NT#2011/02/01#Ben Wang -begin
UINT32 BackgroundUserFunc(void)
{
    if(g_fpUserFunc)
        g_fpUserFunc();
    return FALSE;//dummy return value
}
//#NT#2011/02/01#Ben Wang -end
extern UINT32   System_OnStrg_UploadFW(UINT32 uiStorage);
static UINT32 BackgroundFWUpdate(void)
{
    return System_OnStrg_UploadFW(PRIMARY_STGTYPE_CARD);
}

//#NT#2011/02/10#Photon Lin -begin
//#Add file access
static UINT32 BackgroundFileAccess(void)
{
#if _DEMO_TODO
    Ux_SendEvent(&UIMovieRecObjCtrl,NVTEVT_STOP_REC_MOVIE,NULL);
    UIMovRecObj_WaitReady();
    Delay_DelayMs(1000);
#endif
    return TRUE;

}
//#NT#2011/02/10#Photon Lin -end
static UINT32 BackgroundDZoomIn(void)
{
    IPL_DZOOM Info;
    UINT32    index;

    //DBG_IND("[zoom]DzoomIdx=%d, Max=%d\r\n",DZOOM_IDX_GET(),DZOOM_IDX_MAX());
    index = DZOOM_IDX_GET();
    while (!(*g_pDzoomStop) && index < DZOOM_IDX_MAX())
    {
        Info.Id = IPL_ID_1;
        Info.Idx = ++index;
        IPL_SetCmd(IPL_SET_DZOOM, (void *)&Info);
        IPL_WaitCmdFinish();
        //localInfo->ZoomCtrl = UI_ZOOM_CTRL_IN;
        //localInfo->isDzoomStart = TRUE;
        //DBG_IND("Zoom start\r\n");
    }
    return TRUE;
}

static UINT32 BackgroundDZoomOut(void)
{
    IPL_DZOOM Info;
    UINT32    index;

    //DBG_IND("[zoom]DzoomIdx=%d\r\n",DZOOM_IDX_GET());
    index = DZOOM_IDX_GET();
    while(!(*g_pDzoomStop) && index > DZOOM_IDX_MIN())
    {
        Info.Id = IPL_ID_1;
        Info.Idx = --index;
        IPL_SetCmd(IPL_SET_DZOOM, (void *)&Info);
        IPL_WaitCmdFinish();
        //localInfo->ZoomCtrl = UI_ZOOM_CTRL_OUT;
        //localInfo->isDzoomStart = TRUE;
        //DBG_IND("Zoom start\r\n");
    }
    return TRUE;
}

//#NT#2012/10/23#Hideo Lin -begin
//#NT#For picture in movie processing
static UINT32 BackgroundPIMProcess(void)
{
    Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_MOVIE_REC_STOP,0);
    return NVTRET_OK;
}
//#NT#2012/10/23#Hideo Lin -end

//#NT#2012/10/30#Calvin Chang#Generate Audio NR pattern by noises of zoom, focus and iris -begin
static UINT32 BackgroundAudiNRLensAction(void)
{
    UINT32 i, uiLoop;

    Lens_Zoom_SetSpeed(ZOOM_SPEED_LOW);

    Delay_DelayMs(3000); // Wait 2 seconds

    // 1. Zoom In/out + Focus in 3 seconds
    debug_err(("Zoom In/out + Focus in 3 seconds!\r\n"));
    uiLoop = 1;
    for (i = 0; i < uiLoop; i++)
    {
        Lens_Zoom_In();
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, 50); // position = 45 ~ 600;
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, 600); // position = 45 ~ 600;
        Delay_DelayMs(150);
        Lens_Zoom_Stop();
        Delay_DelayMs(25);
        Lens_Zoom_Out();
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, 50); // position = 45 ~ 600;
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, 600); // position = 45 ~ 600;
        Delay_DelayMs(150);
        Lens_Zoom_Stop();
        Delay_DelayMs(25);
    }

    Delay_DelayMs(6000); // Wait 6 seconds

    //2. Focus Forward N steps in 3 seconds
    debug_err(("Focus Forward N steps in 3 seconds!\r\n"));
    uiLoop = 6;
    for (i = 0; i < uiLoop; i++)
    {
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, 50); // position = 45 ~ 600;
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, 450); // position = 45 ~ 600;

    }

    Delay_DelayMs(7000); // Wait 7 seconds

    //3. Iris switch in 3 seconds
    debug_err(("Iris switch in 3 seconds!\r\n"));
    uiLoop = 15;
    for (i = 0; i < uiLoop; i++)
    {
        Lens_Aperture_Move(IRIS_POS_SMALL);
        Delay_DelayMs(50);
        Lens_Aperture_Move(IRIS_POS_BIG);
        Delay_DelayMs(50);
    }

    Delay_DelayMs(4000);

    Lens_Zoom_SetSpeed(ZOOM_SPEED_HIGH);

    // Record stop!
    Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_MOVIE_REC_STOP,0);

    debug_err(("Record Stop!\r\n"));

    return NVTRET_OK;
}
//#NT#2012/10/30#Calvin Chang -end

static UINT32 BackgroundWiFiOn(void)
{
    Ux_SendEvent(0,NVTEVT_EXE_WIFI_START,0);
    return NVTRET_OK;
}

static void BackgroundMakeMovieCB(UINT32 uiProgress)
{
    debug_msg("^Y Progress %d\r\n", uiProgress);
}

static UINT32 BackgroundMakeMovie(void)
{
    INT32   iRst;
    JPG2MOV_OBJ Jpg2MovObj = {0};

    Jpg2MovObj.fpProgressCB = BackgroundMakeMovieCB;
    iRst = JPG2MOV_MakeMovie(&Jpg2MovObj);

    if (iRst == E_OK)
        return NVTRET_OK;
    else
        return NVTRET_ERROR;
}

static UINT32 BackgroundWiFiClearACL(void)
{
    Delay_DelayMs(ACL_TIME);
    UINet_ClearACLTable();
    return NVTRET_OK;
}
void AppBKW_SetData(BKW_DATA_SET attribute, UINT32 value)
{
    switch(attribute)
    {
        case BKW_NEW_WIDTH:
            g_uiNewWidth = value;
            break;
        case BKW_NEW_HEIGHT:
            g_uiNewHeight= value;
            break;
        case BKW_OVERWRITE:
            g_bOverWrite = value;
            break;
        case BKW_DPOF_MODE:
            g_uiDpofOPMode = value;
            break;
        case BKW_DPOF_NUM:
            g_uiDpofPrtNum = value;
            break;
        case BKW_DPOF_DATE:
            g_uiDpofDateOn = value;
            break;
        case BKW_PROTECT_TYPE:
            g_uiProtectType = value;
            break;
         case BKW_WAIT_TIME:
            g_uiWaitTime = value;
            break;
         case BKW_CHK_CONNECT_PTR:
            g_uiWaitTimeChkPtr = value; //#NT#2010/06/23#Lily Kao
            break;
         //#NT#2011/02/01#Ben Wang -begin
         case BKW_USER_FUNC:
            g_fpUserFunc = (FP)value;
            break;
         //#NT#2011/02/01#Ben Wang -end

         case BKW_DZOOM_STOP:
             g_pDzoomStop = (UINT32*)value;
             break;
         default:
            debug_err(("[AppBKW_SetData]no this attribute"));
            break;
    }

}


