#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "debug.h"
#include "UIPlayObj.h"
#include "PlaybackTsk.h"
#include "UIControlEvent.h"
#include "NVTUserCommand.h"
#include "FileSysTsk.h"
#include "DCF.h"
#include "Dx.h"
#include "DxType.h"
#include "DxStorage.h"
#include "Exif.h"
#include "ExifDef.h"
#include "UIPlayComm.h"
#include "DispSrvApi.h"
#include "PrjCfg.h"
#include "FileDB.h"
#include "PBXFileList_DCF.h"
#include "PBXFileList_FileDB.h"
#include "UIInfo.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIPlayObj
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


//#Fix copy file bug when buf size is large than 16 MB
#define PB_MAX_COPY_FILE_BUF_SIZE       0x1000000


#if _DEMO_TODO
typedef struct {
    UINT8               *pSrcAddr;      ///< Source bitstream address
    UINT8               *pTempAddr;      ///< Source bitstream address
    UINT8               *pDstAddr;      ///< Destination YCbCr raw image buffer address
    UINT32              JPGFileSize;    ///< Source bitstream size
    UINT32              uiNewWidth;     ///
    UINT32              uiNewHeight;    ///
    //UINT32              uiMaxBuffLimit;
    //UINT32              uiOutAddrY;     ///< Output raw image Y buffer address in pDstAddr
    //UINT32              uiOutAddrCb;    ///< Output raw image Cb buffer address in pDstAddr
    //UINT32              uiOutAddrCr;    ///< Output raw image Cr buffer address in pDstAddr
} DEC_USERJPG, *PDEC_USERJPG;
#endif

static PLAY_SINGLE_OBJ FlowPlaySingleObj;
static UINT32 g_uiStatus = PB_STA_UNKNOWN;        // return value of PB task
static UINT8  g_HorNums = 1;
static UINT8  g_VerNums = 1;
//#NT#2010/11/05#Ben Wang -begin
//#NT#Add the feature of "show selected thumb on background"
static BOOL   g_bDrawCurrImgOnBG = FALSE;
//#NT#2010/11/05#Ben Wang -end
//static UINT32 g_uiSliderTime;
//static UINT32 *pZoomLevelTbl;
static BOOL   g_bPagechanged = FALSE;
//#NT#2010/09/28#Ben Wang -begin
//#NT#Add face detect function
#if _DEMO_TODO
static URECT*  g_pRectFace = NULL;
static UINT32 g_nFaces = 0;
//#NT#2010/09/28#Ben Wang -end
#endif

#if _DEMO_TODO
static ER DecodeUserJpeg2NewSize(PDEC_USERJPG pUserJpg)
{
    JPG_DEC_INFO DecOneJPGInfo;
    JPGHEAD_DEC_CFG     JPGCfg;
    UINT32 bufSize;
    UINT32 uiBGAddrY, uiBGAddrCb, uiBGAddrCr;
    ER  retV;
    tUT_SCALE_SET Scale = {0};

    // JPG information
    DecOneJPGInfo.pJPGScalarHandler = NULL;
    DecOneJPGInfo.pSrcAddr          = pUserJpg->pSrcAddr;
    DecOneJPGInfo.pDstAddr          = pUserJpg->pTempAddr;
    DecOneJPGInfo.JPGFileSize       = pUserJpg->JPGFileSize;
    DecOneJPGInfo.jdcfg_p           = &JPGCfg;
    DecOneJPGInfo.DecodeType     = DEC_PRIMARY;
    DecOneJPGInfo.bEnableTimeOut = FALSE;
    DecOneJPGInfo.bEnableCrop = FALSE;

    // Decode startup JPG, Jpg_DecOneJPG() will open JPEG driver
    retV = Jpg_DecOneJPG(&DecOneJPGInfo);
    if (retV != E_OK)
    {
        DBG_ERR("DecodeUserJpeg> decode JPG FAIL!\r\n"));
    }

    bufSize = pUserJpg->uiNewWidth * pUserJpg->uiNewHeight;
    uiBGAddrY = (UINT32)pUserJpg->pDstAddr;
    uiBGAddrCb = uiBGAddrY + bufSize;
    uiBGAddrCr = uiBGAddrCb + (bufSize>>1);

    Scale.InAddrY  = DecOneJPGInfo.uiOutAddrY;
    Scale.InAddrCb = DecOneJPGInfo.uiOutAddrCb;
    Scale.InAddrCr = DecOneJPGInfo.uiOutAddrCr;
    Scale.InWidth  = JPGCfg.imagewidth;
    Scale.InHeight = JPGCfg.imageheight;
    Scale.InLineOffsetY = JPGCfg.lineoffsetY;
    Scale.InLineOffsetUV = JPGCfg.lineoffsetUV;
    if(JPGCfg.fileformat == JPG_FMT_YUV211)
        Scale.InFormat = UT_IMAGE_FMT_YUV422;
    else if(JPGCfg.fileformat  == JPG_FMT_YUV420)
        Scale.InFormat = UT_IMAGE_FMT_YUV420;
    else
        Scale.InFormat = UT_IMAGE_FMT_YUV422;
    //OutPut Settings
    Scale.OutAddrY  = uiBGAddrY;
    Scale.OutAddrCb = uiBGAddrCb;
    Scale.OutAddrCr = uiBGAddrCr;
    Scale.OutWidth  = pUserJpg->uiNewWidth;
    Scale.OutHeight = pUserJpg->uiNewHeight;
    Scale.OutLineOffsetY = pUserJpg->uiNewWidth;
    Scale.OutLineOffsetUV = pUserJpg->uiNewWidth >> 1;
    Scale.OutFormat = UT_IMAGE_FMT_YUV422;
    UtImageScaleByIme(&Scale);

    return retV;

}
#endif

static INT32 UIPlay_OnInit(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DBG_MSG("UIPlay_OnInit ++\r\n ");
    //FlowPlaySingleObj.MoveImgMode     = SLIDE_EFFECT_NONE;
    FlowPlaySingleObj.slideEffectFunc = 0;
    g_uiStatus = (UINT32)PB_WaitCommandFinish(PB_WAIT_INFINITE);
    DBG_MSG("UIPlay_OnInit --\r\n ");
    return NVTEVT_CONSUME;
}

static INT32 UIPlay_OnSingle(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(paramNum ==0 || paramNum > 2 || !(paramArray[0]&(PB_SINGLE_NEXT|PB_SINGLE_PREV|PB_SINGLE_CURR)))
    {
        DBG_ERR("NVTEVT_PLAYSINGLE PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }


    if(paramArray[0] & PB_SINGLE_CURR || paramArray[1] == 0)
        paramArray[1] = 1;
    DBG_MSG("UIPlay_OnSingle paramArray[0]=0x%x,paramArray[1]=%d\r\n",paramArray[0],paramArray[1]);
    if(paramArray[0] & PB_SINGLE_THUMB)
        FlowPlaySingleObj.PlayCommand = (UINT8)paramArray[0];
    else//default let playback show primary or hidden JPEG
    {
        FlowPlaySingleObj.PlayCommand = (UINT8)paramArray[0] | PB_SINGLE_PRIMARY;
        #if (_SCREENNAIL_SIZE_ == _SCREENNAIL_SIZE_VGA_)
        if(IsHDMIPlugIn())
        {
            FlowPlaySingleObj.PlayCommand |= PB_SINGLE_NO_HIDDEN;
        }
        #endif
    }
    FlowPlaySingleObj.JumpOffset  = (UINT8)paramArray[1];
    PB_PlaySingleMode(&FlowPlaySingleObj);
    g_uiStatus = (UINT32)PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
}

//#if USE_FILEDB

#if (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
static ER xUIPlay_FList_FDB_DelSmllCloneFile(PFILEDB_FILE_ATTR FileAttr)
{
    INT32  ret = FST_STA_OK;
    char   SmallCloneFilePath[PBX_FLIST_NAME_MAX_LENG] = {0};
    char   MainFileName[32] = {0};
    UINT32 pathlen;

    pathlen = strlen(FileAttr->filename);

    // no ext name
    strncpy(MainFileName, FileAttr->filename, pathlen-4);

    snprintf(SmallCloneFilePath,PBX_FLIST_NAME_MAX_LENG,"A:\\%s\\Movie_s\\%s_s.MP4",FILEDB_CARDV_ROOT, MainFileName);

    DBG_DUMP("delete small clone file %s\r\n", SmallCloneFilePath);

    ret = FileSys_DeleteFile(SmallCloneFilePath);

    if (ret != FST_STA_OK)
        DBG_ERR("delete small clone file is failed!!\r\n");

    return ret;

}
#endif

ER xUIPlay_FList_FDB_DelFile(UINT32  cmdID)
{
    FILEDB_HANDLE     FileDBHdl = 0;
    PFILEDB_FILE_ATTR FileAttr = NULL;
    INT32             ret;
    INT32             FileNum,i;

    Perf_Mark();
    DBG_IND("cmdID=%d\r\n",cmdID);
    if (cmdID == PB_DELETE_ONE)
    {
        FileAttr = FileDB_CurrFile(FileDBHdl);
        i =  FileDB_GetCurrFileIndex(FileDBHdl);
        if (FileAttr)
        {
            ret = FileSys_DeleteFile(FileAttr->filePath);

			#if (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
            xUIPlay_FList_FDB_DelSmllCloneFile(FileAttr);
            #endif

            if (ret == FST_STA_OK)
                FileDB_DeleteFile(FileDBHdl,i);
            else
                goto FDB_Delete_Err;
        }
        else
            goto FDB_Delete_Err;
    }
    else if (cmdID == PB_DELETE_ALL)
    {
        BOOL  isCurrFileReadOnly = FALSE;
        CHAR  path[PBX_FLIST_NAME_MAX_LENG+1];
        UINT32 Index;

        FileAttr = FileDB_CurrFile(FileDBHdl);
        if( FileAttr && M_IsReadOnly(FileAttr->attrib))
        {
            isCurrFileReadOnly = TRUE;
            strncpy(path,FileAttr->filePath,PBX_FLIST_NAME_MAX_LENG);
        }
        FileNum = FileDB_GetTotalFileNum(FileDBHdl);
        for (i=FileNum-1;i>=0;i--)
        {
            FileAttr = FileDB_SearhFile(FileDBHdl,i);
            if (FileAttr)
            {
                if (M_IsReadOnly(FileAttr->attrib))
                    continue;
                ret = FileSys_DeleteFile(FileAttr->filePath);
                DBG_IND("i = %04d path=%s\r\n",i,FileAttr->filePath);

				#if (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
                xUIPlay_FList_FDB_DelSmllCloneFile(FileAttr);
                #endif

                if (ret != FST_STA_OK)
                    goto FDB_Delete_Err;
                else
                    FileDB_DeleteFile(FileDBHdl,i);
            }
            else
                goto FDB_Delete_Err;
        }
        if (isCurrFileReadOnly)
        {
           Index = FileDB_GetIndexByPath(FileDBHdl,path);
           FileDB_SearhFile(FileDBHdl,Index);
        }
    }
    DBG_IND("DelTime = %04d ms \r\n",Perf_GetDuration()/1000);
    return E_OK;
FDB_Delete_Err:
    DBG_ERR("cmdID=%d\r\n",cmdID);
    return E_SYS;

}

ER xUIPlay_FList_FDB_ProtectFile(UINT32  cmdID)
{
    FILEDB_HANDLE     FileDBHdl = 0;
    PFILEDB_FILE_ATTR FileAttr = NULL;
    INT32             ret;
    UINT32            FileNum,i;
    BOOL              bLock;

    Perf_Mark();
    if (cmdID == PLAY_PROTECT_ONE || cmdID == PLAY_UNPROTECT_ONE )
    {
        if (cmdID == PLAY_PROTECT_ONE)
            bLock = TRUE;
        else
            bLock = FALSE;
        FileAttr = FileDB_CurrFile(FileDBHdl);
        i =  FileDB_GetCurrFileIndex(FileDBHdl);
        if (FileAttr)
        {
            ret = FileSys_SetAttrib(FileAttr->filePath,FST_ATTRIB_READONLY,bLock);
            if (ret == FST_STA_OK)
            {
                if (bLock)
                    FileAttr->attrib |=FS_ATTRIB_READ;
                else
                    FileAttr->attrib &=(~FS_ATTRIB_READ);
            }
            else
                goto FDB_Protect_Err;
        }
        else
            goto FDB_Protect_Err;
    }
    else if (cmdID == PLAY_PROTECT_ALL || cmdID == PLAY_UNPROTECT_ALL)
    {
        if (cmdID == PLAY_PROTECT_ALL)
            bLock = TRUE;
        else
            bLock = FALSE;
        FileNum = FileDB_GetTotalFileNum(FileDBHdl);
        for (i=0;i<FileNum;i++)
        {
            FileAttr = FileDB_SearhFile(FileDBHdl,i);
            if (FileAttr)
            {
                ret = FileSys_SetAttrib(FileAttr->filePath,FST_ATTRIB_READONLY,bLock);
                if (ret == FST_STA_OK)
                {
                    if (bLock)
                        FileAttr->attrib |=FS_ATTRIB_READ;
                    else
                        FileAttr->attrib &=(~FS_ATTRIB_READ);
                }
                else
                    goto FDB_Protect_Err;
            }
            else
                goto FDB_Protect_Err;
        }
    }
    DBG_IND("ProtectTime = %04d ms \r\n",Perf_GetDuration()/1000);
    return E_OK;
FDB_Protect_Err:
    DBG_ERR("cmdID=%d\r\n",cmdID);
    return E_OK;
}

//#else
static BOOL    gbNoFile = TRUE;
static void xUIPlay_Filesys_DelCB(FIND_DATA *pFindData, BOOL *bDelete, UINT32 Param1, UINT32 Param2)
{
    char *   pFileName;
    INT32    fileNum;
    UINT32   fileType;
    UINT32   filterType = (DCF_GetDBInfo(DCF_INFO_VALID_FILE_FMT)|DCF_GetDBInfo(DCF_INFO_DEP_FILE_FMT));
    UINT8     attrib;

    pFileName = pFindData->FATMainName;
    attrib    = pFindData->attrib;

    DBG_IND("pFileName = %s\r\n",pFileName);

    fileNum = DCF_IsValidFile(pFileName, &fileType);

    if (!M_IsReadOnly(attrib) && fileNum && (filterType&fileType) )
        *bDelete = TRUE;
    else
    {
        *bDelete = FALSE;
        gbNoFile = FALSE;
    }
}

static void xUIPlay_FList_DCF_DelAll(void)
{
    SDCFDIRINFO dirinfo;
    char path[DCF_FULL_FILE_PATH_LEN];
    BOOL ret;
    UINT32 i,uiMaxDirNum;

    uiMaxDirNum = DCF_GetDBInfo(DCF_INFO_MAX_DIR_ID);
    for(i=100;i<=uiMaxDirNum;i++)
    {
       // check if folder has file
       ret = DCF_GetDirInfo(i,&dirinfo);
       if (ret /*&& dirinfo.uiNumOfDcfObj*/)
       {
            // delete all in one folder
            DCF_GetDirPath(i,path);
            gbNoFile = TRUE;
            FileSys_DelDirFiles(path,xUIPlay_Filesys_DelCB);
            // delete empty folder
            if (gbNoFile)
            {
                FileSys_DeleteDir(path);
            }
       }
    }
    DCF_Refresh();
}

#if 0
static UINT32 gReadOnlyCount;
static void xUIPlay_ReadOnly_SacnCB(FIND_DATA *findFile, BOOL *bContinue, UINT16 *cLongname, UINT32 Param)
{
    char *   pFileName;
    INT32    fileNum;
    UINT32   fileType;
    UINT32   filterType = (DCF_GetDBInfo(DCF_INFO_VALID_FILE_FMT)|DCF_GetDBInfo(DCF_INFO_DEP_FILE_FMT));

    pFileName = findFile->FATMainName;
    DBG_IND("pFileName = %s\r\n",pFileName);
    fileNum = DCF_IsValidFile(pFileName, &fileType);
    if (fileNum && (filterType&fileType) && M_IsReadOnly(findFile->attrib))
    {
        gReadOnlyCount++;
    }
}

static void xUIPlay_FList_DCF_CountReadOnlyAll(void)
{
    SDCFDIRINFO dirinfo;
    char path[DCF_FULL_FILE_PATH_LEN];
    UINT32 i,uiMaxDirNum;

    gReadOnlyCount = 0;
    uiMaxDirNum = DCF_GetDBInfo(DCF_INFO_MAX_DIR_ID);
    for(i=100;i<=uiMaxDirNum;i++)
    {
        // check if folder has file
        DCF_GetDirInfo(i,&dirinfo);
        if (dirinfo.uiNumOfDcfObj)
        {
            DCF_GetDirPath(i,path);
            FileSys_ScanDir(path, xUIPlay_ReadOnly_SacnCB, FALSE);
        }
    }
}
#endif

static ER xUIPlay_FList_DCF_DelFile(UINT32  cmdID)
{
    DBG_IND("cmdID=%d\r\n",cmdID);
    if (cmdID == PB_DELETE_ONE)
    {
        UINT32 index, uiThisFileFormat,i,tmpFileType = 1;
        CHAR   filePath[DCF_FULL_FILE_PATH_LEN];

        index = DCF_GetCurIndex();
        uiThisFileFormat = DCF_GetDBInfo(DCF_INFO_CUR_FILE_TYPE);
        for (i=0;i<=DCF_FILE_TYPE_NUM;i++)
        {
            tmpFileType = 0x0001 << i;
            if (uiThisFileFormat & tmpFileType)
            {
               DCF_GetObjPath(index, uiThisFileFormat, filePath);
               FileSys_DeleteFile(filePath);
               DCF_DelDBfile(filePath);
            }
        }
    }
    else if (cmdID == PB_DELETE_ALL)
    {
        char        filePath[DCF_FULL_FILE_PATH_LEN];
        BOOL        ret;
        UINT8       attrib;
        UINT32      DirID,FileID,Index,uiThisFileFormat;
        BOOL        isCurrFileReadOnly = FALSE;

        Index = DCF_GetDBInfo(DCF_INFO_CUR_INDEX);
        DCF_GetObjInfo(Index,&DirID, &FileID, &uiThisFileFormat);
        DCF_GetObjPath(Index,uiThisFileFormat, filePath);

        ret = FileSys_GetAttrib(filePath,&attrib);
        if( (ret == E_OK) && M_IsReadOnly(attrib) == TRUE )
            isCurrFileReadOnly = TRUE;

        // ---------  delete all files  -----------------------
        xUIPlay_FList_DCF_DelAll();
        if (isCurrFileReadOnly)
        {
           Index = DCF_GetIndexByID(DirID,FileID);
           DCF_SetCurIndex(Index);
        }
        DCF_Refresh();
    }
    return E_OK;
}

static void xUIPlay_Filesys_LockCB(FIND_DATA *pFindData, BOOL *bApply, UINT32 Param1, UINT32 Param2)
{
    char *   pFileName;
    INT32    fileNum;
    UINT32   fileType;
    UINT32   filterType = (DCF_GetDBInfo(DCF_INFO_VALID_FILE_FMT)|DCF_GetDBInfo(DCF_INFO_DEP_FILE_FMT));

    pFileName = pFindData->FATMainName;
    DBG_IND("pFileName = %s\r\n",pFileName);
    fileNum = DCF_IsValidFile(pFileName, &fileType);
    if (fileNum && (filterType&fileType))
        *bApply = TRUE;
    else
    {
        *bApply = FALSE;
    }
}

static void xUIPlay_FList_DCF_LockAll(BOOL bLock)
{
    SDCFDIRINFO dirinfo;
    char path[DCF_FULL_FILE_PATH_LEN];
    UINT32 i,uiMaxDirNum;

    uiMaxDirNum = DCF_GetDBInfo(DCF_INFO_MAX_DIR_ID);
    for(i=100;i<=uiMaxDirNum;i++)
    {
        // check if folder has file
        DCF_GetDirInfo(i,&dirinfo);
        if (dirinfo.uiNumOfDcfObj)
        {
            DCF_GetDirPath(i,path);
            FileSys_LockDirFiles(path,bLock,xUIPlay_Filesys_LockCB);
        }
    }
}

ER xUIPlay_FList_DCF_ProtectFile(UINT32  cmdID)
{
    char     filePath[DCF_FULL_FILE_PATH_LEN];
    UINT32   index,i,uiThisFileFormat,tmpFileType;
    BOOL     bLock;

    Perf_Mark();
    DBG_IND("cmdID=%d\r\n",cmdID);
    if( (cmdID == PLAY_PROTECT_ONE)||(cmdID == PLAY_UNPROTECT_ONE) )
    {
        if(cmdID == PLAY_PROTECT_ONE)
            bLock = TRUE;  // lock
        else
            bLock = FALSE; // unlock
        index = DCF_GetCurIndex();
        uiThisFileFormat = DCF_GetDBInfo(DCF_INFO_CUR_FILE_TYPE);
        for (i=0;i<=DCF_FILE_TYPE_NUM;i++)
        {
            tmpFileType = 1;
            tmpFileType <<= i;
            if (uiThisFileFormat & tmpFileType)
            {
               DCF_GetObjPath(index, tmpFileType, filePath);
               FileSys_SetAttrib(filePath,FST_ATTRIB_READONLY,bLock);
            }
        }
    }
    else if( (cmdID == PLAY_PROTECT_ALL)||(cmdID == PLAY_UNPROTECT_ALL) )
    {
        if(cmdID == PLAY_PROTECT_ALL)
            bLock  = TRUE;  // lock
        else
            bLock  = FALSE; // unlock
        xUIPlay_FList_DCF_LockAll(bLock);
    }
    DBG_IND("ProtectTime = %04d ms \r\n",Perf_GetDuration()/1000);
    return E_OK;
}
//#endif

static INT32 UIPlay_Delete(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 index;
    UINT32 useFileDB=0;
    if(paramNum !=1 || (paramArray[0]!=PB_DELETE_ONE && paramArray[0]!=PB_DELETE_ALL))
    {
        DBG_ERR("NVTEVT_DELETE PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }

    DBG_MSG("UIPlay_Delete paramArray[0]=0x%x,paramArray[1]=%d\r\n",paramArray[0]);

    useFileDB = UI_GetData(FL_IsUseFileDB);
    if (useFileDB)
    {
        xUIPlay_FList_FDB_DelFile(paramArray[0]);
    }
    else
    {
        xUIPlay_FList_DCF_DelFile(paramArray[0]);
    }

    PB_GetParam(PBPRMID_CURR_FILESEQ, &index);

    PB_OpenSpecFileBySeq(index,TRUE);

    PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
}



static INT32 UIPlay_Protect(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32      LockCmmd;
    UINT32      useFileDB    =0;

    if(paramNum !=1 ||
        (paramArray[0]!=PLAY_PROTECT_ONE &&
         paramArray[0]!=PLAY_PROTECT_ALL &&
         paramArray[0]!=PLAY_UNPROTECT_ONE&&
         paramArray[0]!=PLAY_UNPROTECT_ALL))
    {
        DBG_ERR("NVTEVT_PROTECT PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }

    DBG_MSG("UIPlay_Protect paramArray[0]=0x%x,paramArray[1]=%d\r\n",paramArray[0]);

    LockCmmd = paramArray[0];
    useFileDB = UI_GetData(FL_IsUseFileDB);

    //#if USE_FILEDB
    if (useFileDB)
    {
        xUIPlay_FList_FDB_ProtectFile(LockCmmd);
    }
    else
    {
        xUIPlay_FList_DCF_ProtectFile(LockCmmd);
    }
    return NVTEVT_CONSUME;
}
//#NT#2011/01/31#Ben Wang -begin
//#NT#Add screen control to sync display with OSD
static INT32 UIPlay_SetScreenCtrl(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _DEMO_TODO
    if(paramNum !=1 ||
        (paramArray[0]!=PLAY_LOCK_SCREEN &&
         paramArray[0]!=PLAY_FLUSH_SCREEN ))
    {
        DBG_ERR("NVTEVT_SCREEN_CTRL PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }

    PB_ScreenControl((PB_SCREEN_CTRL_TYPE)paramArray[0]);
#endif
    return NVTEVT_CONSUME;
}
//#NT#2011/01/31#Ben Wang -end
static INT32 UIZoom_OnInit(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(paramNum != 1 || !(paramArray[0]))
    {
        DBG_ERR("NVTEVT_ZOOMINIT PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }
    PB_SetParam(PBPRMID_ZOOM_LVL_TBL, (UINT32)(paramArray[0]));
    //PB_PlayZoomMode(PLAYZOOM_IN);
    //g_uiStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
}

static INT32 UIZoom_OnZoomInOut(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(paramNum != 1 || !(paramArray[0]&(PLAYZOOM_IN|PLAYZOOM_OUT)))
    {
        DBG_ERR("NVTEVT_ZOOMINOUT PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }

    PB_PlayZoomMode(paramArray[0]);
    g_uiStatus = (UINT32)PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
}

static INT32 UIZoom_OnZoomNavi(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(paramNum != 3 ||
        !(paramArray[0]&(PLAYZOOM_UP|PLAYZOOM_DOWN|PLAYZOOM_LEFT|PLAYZOOM_RIGHT)))
    {
        DBG_ERR("NVTEVT_ZOOMNAVI PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }
    PB_PlayZoomPanMode(paramArray[0], paramArray[1], paramArray[2]);
    g_uiStatus = (UINT32)PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
}

static INT32 UIBrowse_OnInit(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2010/11/05#Ben Wang -begin
    //#NT#Add the feature of "show selected thumb on background"
    PBROWSE_INIT_OBJ pBrowseInitObj;
    if(paramNum != 1)
    {
        DBG_ERR("NVTEVT_BROWSEINIT PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }
    pBrowseInitObj = (PBROWSE_INIT_OBJ)paramArray[0];
    g_HorNums = pBrowseInitObj->HorNums;
    g_VerNums = pBrowseInitObj->VerNums;
    PB_SetParam(PBPRMID_THUMB_LAYOUT_ARRAY, (UINT32)pBrowseInitObj->pThumbRect);
    g_bDrawCurrImgOnBG = pBrowseInitObj->bDrawCurrImgOnBG;
    //#NT#2010/11/05#Ben Wang -end

//#NT#2012/11/27#Scottie -begin
//#NT#Support Dual Display for PB
    {
        DISPSRV_CMD DispCmd={0};
        DISPSRV_DISP_CFG CfgDisp={0};

        //wait DispSrv finish setting
        memset(&DispCmd,0,sizeof(DISPSRV_CMD));
        DispCmd.Idx = DISPSRV_CMD_IDX_GET_DISP;
        DispCmd.Prop.bExitCmdFinish = TRUE;
        DispCmd.Out.uiNumByte = sizeof(CfgDisp);
        DispCmd.Out.pData= &CfgDisp;
        DispSrv_Cmd(&DispCmd);

        if(TRUE == CfgDisp.bDual)
        {
            PB_SetParam(PBPRMID_THUMB_LAYOUT_ARRAY2, (UINT32)pBrowseInitObj->pThumbRect2);
        }
    }
//#NT#2012/11/27#Scottie -end

    return NVTEVT_CONSUME;
}
static INT32 UIBrowse_OnSingle(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PLAY_BROWSER_OBJ     FlowPBBrowserObj;
    UINT32 FileNumsInDir, NumsPerPage;
    if(paramNum != 2)
    {
        DBG_ERR("NVTEVT_BROWSESINGLE PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }
    NumsPerPage = g_HorNums * g_VerNums;
    FileNumsInDir = AppPlay_GetData(PLAY_FILENUM);
    FlowPBBrowserObj.BrowserCommand = (UINT8)paramArray[0];
    FlowPBBrowserObj.slideEffectFunc    = FlowPlaySingleObj.slideEffectFunc;
    FlowPBBrowserObj.JumpOffset     = (UINT8)paramArray[1];
    FlowPBBrowserObj.HorNums        = g_HorNums;
    FlowPBBrowserObj.VerNums        = g_VerNums;
    if(FileNumsInDir <= NumsPerPage)// means only one page
        FlowPBBrowserObj.bReDecodeImages = FALSE;
    else
        FlowPBBrowserObj.bReDecodeImages = TRUE;
    //#NT#2010/11/05#Ben Wang -begin
    //#NT#Add the feature of "show selected thumb on background"
    //FlowPBBrowserObj.bDrawCurrImgOnBG = g_bDrawCurrImgOnBG;
    //#NT#2010/11/05#Ben Wang -end
    if (PB_PlayBrowserMode(&FlowPBBrowserObj) != PB_STA_DONE)
    {
        g_bPagechanged = TRUE;
    }
    else
    {
        g_bPagechanged = FALSE;
    }
    return NVTEVT_CONSUME;
}
static INT32 UIBrowse_OnFinish(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    g_uiStatus = (UINT32)PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
}
static INT32 UIPlay_UpDateRsvData(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _DEMO_TODO
    PPLAY_OBJ   pPlayObj;
    DEC_USERJPG UserJpg;
    UINT32 uiTempRawAddr, uiBgAddr, uiDispSize;
    PPBRSV_DATA pRsvData;

    if(paramNum != 1 || !(paramArray[0]))
    {
        DBG_ERR("NVTEVT_UPDATE_BGDATA PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }

    pRsvData = (PPBRSV_DATA)paramArray[0];

    DBG_MSG("[UIPlay_UpDateRsvData] Display WxH=(%d , %d)\r\n",pRsvData->uiDisplayWidth,pRsvData->uiDisplayHeight);

    pPlayObj = PB_GetPBObject();
    uiDispSize = pRsvData->uiDisplayWidth*pRsvData->uiDisplayHeight;

    //Reserve the max possible size for LCD backup (display(422) buffer)
    if(uiDispSize > PB_GetMaxPanelSize())
        uiBgAddr = pRsvData->uiPBRsvBuffAddr + uiDispSize*2;
    else
        uiBgAddr = pRsvData->uiPBRsvBuffAddr + PB_GetMaxPanelSize()*2;

    //uiTempRawAddr = uiBgAddr + uiDispSize*2;
    uiTempRawAddr = pRsvData->uiPBRsvBuffAddr;

#if _MIPS_TODO
//TBB: will modify the drawing backgroud image mechanism
    //one BG data shoule reserve a display(422) size
    if(pRsvData->uiMovBgFileAddr)
    {
        UserJpg.pSrcAddr = (UINT8 *)pRsvData->uiMovBgFileAddr;
        UserJpg.pTempAddr = (UINT8 *)uiTempRawAddr;
        UserJpg.pDstAddr = (UINT8 *)uiBgAddr;
        UserJpg.JPGFileSize = pRsvData->uiMovBgFileSize;
        UserJpg.uiNewWidth = pRsvData->uiDisplayWidth;
        UserJpg.uiNewHeight = pRsvData->uiDisplayHeight;
        DecodeUserJpeg2NewSize(&UserJpg);
        pPlayObj->uiMovieBGAddrY  = uiBgAddr;
        pPlayObj->uiMovieBGAddrCb = uiBgAddr + uiDispSize;
        pPlayObj->uiMovieBGAddrCr = pPlayObj->uiMovieBGAddrCb + (uiDispSize>>1) ;
        uiBgAddr += (uiDispSize*2);
    }

    //one BG data shoule reserve a display(422) size
    if(pRsvData->uiBadBgFileAddr)
    {
        UserJpg.pSrcAddr = (UINT8 *)pRsvData->uiBadBgFileAddr;
        UserJpg.pTempAddr = (UINT8 *)uiTempRawAddr;
        UserJpg.pDstAddr = (UINT8 *)uiBgAddr;
        UserJpg.JPGFileSize = pRsvData->uiBadBgFileSize;
        UserJpg.uiNewWidth = pRsvData->uiDisplayWidth;
        UserJpg.uiNewHeight = pRsvData->uiDisplayHeight;
        DecodeUserJpeg2NewSize(&UserJpg);
        pPlayObj->uiBadFileBGAddrY  = uiBgAddr;
        pPlayObj->uiBadFileBGAddrCb = uiBgAddr + uiDispSize;
        pPlayObj->uiBadFileBGAddrCr = pPlayObj->uiBadFileBGAddrCb + (uiDispSize>>1);
        uiBgAddr += (uiDispSize*2);
    }
#endif

    if( uiBgAddr > pRsvData->uiPBRsvBuffAddr + pRsvData->uiPBRsvBuffSize)
    {
        DBG_ERR("####BG data might overlap!!! Please increase buffer size.###\r\n");
    }

    //#NT#2010/01/20#Jeah Yen -begin
    //fix TV plug/unplug flow bug under playback mode: avoid turn on VDO1 by playback
    //while UIPlay_ChangePBSize()->PB_SetIMEIDESize()->PB_FrameBufferSetting(), VDO1 will be turn on
    if(GxVideo_GetDevice() == 0) //display is under locked state (TV plug/unplug flow)
    {
        UINT32 uiEnFlag;

        PB_GetParam(PBPRMID_EN_FLAGS, &uiEnFlag);
        PB_SetParam(PBPRMID_EN_FLAGS, uiEnFlag|PB_ENABLE_TV_PLUGIN);
    }
    else
    {
        UINT32 uiEnFlag;

        PB_GetParam(PBPRMID_EN_FLAGS, &uiEnFlag);
        PB_SetParam(PBPRMID_EN_FLAGS, uiEnFlag & ~(PB_ENABLE_TV_PLUGIN));
    }
    PB_SetParam(PBPRMID_PLAYBACK_OBJ, (UINT32)pPlayObj);
#endif
    return NVTEVT_CONSUME;
}

static INT32 UIPlay_ChangePBSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _DEMO_TODO
    ISIZE* pSize;
    RESIZE Resize;
    PPLAY_OBJ   pPlayObj;

    if(paramNum != 1 || !(paramArray[0]))
    {
        DBG_ERR("NVTEVT_CHANGE_PBSIZE PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }
    pSize = (ISIZE*)paramArray[0];

    Resize.uiSrcWidth = (UINT32)pSize->w;
    Resize.uiSrcHeight = (UINT32)pSize->h;
    Resize.uiDestWidth = (UINT32)pSize->w;
    Resize.uiDestHeigth = (UINT32)pSize->h;

    PB_SetIMEIDESize(&Resize);
    PB_Set1stVideoFrame(0, 0, (UINT16)pSize->w, (UINT16)pSize->h);
#endif
    return NVTEVT_CONSUME;
}
static INT32 UIPlay_Resize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PB_EDIT_CONFIG Config = {0};

    if(paramNum != 3)
    {
        DBG_ERR("NVTEVT_RESIZE PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }

    Config.Operation = PBEDIT_RESIZE;
    Config.puiParam[0] = paramArray[0];
    Config.puiParam[1] = paramArray[1];
    Config.puiParam[2] = paramArray[2];
    PB_EditImage(&Config);
    PB_WaitCommandFinish(PB_WAIT_INFINITE);

    return NVTEVT_CONSUME;
}
static INT32 UIPlay_FixRedEye(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PB_EDIT_CONFIG Config = {0};

    if(paramNum < 3)
    {
        DBG_ERR("NVTEVT_FIXREDEYE PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }

    Config.Operation = paramArray[0];
    Config.puiParam[0] = paramArray[1];
    Config.puiParam[1] = paramArray[2];
    if(paramNum == 4)
    {
        Config.puiParam[2] = paramArray[3];
    }
    PB_EditImage(&Config);
    g_uiStatus = (UINT32)PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
}
static INT32 UIPlay_ReSaveFile(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
//    if(paramNum != 1)
    {
        DBG_ERR("NVTEVT_RESAVEFILE PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }
    //backward compatible
/*
    if(paramArray[0])
        PB_SetRedEyeRemoval(PLAY_RED_EYE_REMOVAL_SAVE_FILE);
    else
        PB_SetRedEyeRemoval(PLAY_RED_EYE_REMOVAL_SAVE_AS_NEW_FILE);
    g_uiStatus = (UINT32)PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
*/
}
static INT32 UIPlay_RotateDisplay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PB_EDIT_CONFIG Config = {0};

    if(paramNum != 1)
    {
        DBG_ERR("NVTEVT_ROTATE_DISPLAY PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }

    Config.Operation = PBEDIT_ROTATE_DISP;
    Config.puiParam[0] = paramArray[0];
    PB_EditImage(&Config);
    g_uiStatus = (UINT32)PB_WaitCommandFinish(PB_WAIT_INFINITE);

    return NVTEVT_CONSUME;
}
static INT32 UIPlay_RotateExif(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 OldOrient, NewOrient, Rotation;
    EXIF_GETTAG exifTag;
    BOOL bIsLittleEndian;
    PB_EDIT_CONFIG Config = {0};
    //FST_FILE   filehdl;
    //UINT32 Curindex,uiDirNum,uiFileNum,uiFileType;
    //CHAR    chaFullName[30] = { 0 };
    //INT32 ret;
    if(paramNum != 2)
    {
        DBG_ERR("NVTEVT_ROTATE_EXIF PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }
    Exif_GetInfo(EXIFINFO_BYTEORDER, &bIsLittleEndian, sizeof(bIsLittleEndian));
    exifTag.uiTagIfd = EXIF_IFD_0TH;
    exifTag.uiTagId = TAG_ID_ORIENTATION;
    if(E_OK == Exif_GetTag(&exifTag))
    {
        OldOrient = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
    }
    else
    {
        DBG_ERR("NVTEVT_ROTATE_EXIF THIS IMAGE HAS NO EXIF INFO!!!\r\n");
        return NVTEVT_CONSUME;
    }
    Rotation = paramArray[0];
    switch (Rotation)
    {
        case PLAY_ROTATE_DIR_UNKNOWN:
            NewOrient = JPEG_EXIF_ORI_DEFAULT;
        break;
        case PLAY_ROTATE_DIR_90:
            if(OldOrient == JPEG_EXIF_ORI_DEFAULT)
                NewOrient = JPEG_EXIF_ORI_ROTATE_90;
            else if(OldOrient == JPEG_EXIF_ORI_ROTATE_90)
                NewOrient = JPEG_EXIF_ORI_ROTATE_180;
            else if(OldOrient == JPEG_EXIF_ORI_ROTATE_180)
                NewOrient = JPEG_EXIF_ORI_ROTATE_270;
            else//if(OldOrient == JPEG_EXIF_ORI_ROTATE_270)
                NewOrient = JPEG_EXIF_ORI_DEFAULT;
        break;
        case PLAY_ROTATE_DIR_180:
            if(OldOrient == JPEG_EXIF_ORI_DEFAULT)
                NewOrient = JPEG_EXIF_ORI_ROTATE_180;
            else if(OldOrient == JPEG_EXIF_ORI_ROTATE_90)
                NewOrient = JPEG_EXIF_ORI_ROTATE_270;
            else if(OldOrient == JPEG_EXIF_ORI_ROTATE_180)
                NewOrient = JPEG_EXIF_ORI_DEFAULT;
            else//if(OldOrient == JPEG_EXIF_ORI_ROTATE_270)
                NewOrient = JPEG_EXIF_ORI_ROTATE_90;
        break;
        case PLAY_ROTATE_DIR_270:
            if(OldOrient == JPEG_EXIF_ORI_DEFAULT)
                NewOrient = JPEG_EXIF_ORI_ROTATE_270;
            else if(OldOrient == JPEG_EXIF_ORI_ROTATE_90)
                NewOrient = JPEG_EXIF_ORI_DEFAULT;
            else if(OldOrient == JPEG_EXIF_ORI_ROTATE_180)
                NewOrient = JPEG_EXIF_ORI_ROTATE_90;
            else//if(OldOrient == JPEG_EXIF_ORI_ROTATE_270)
                NewOrient = JPEG_EXIF_ORI_ROTATE_180;
        break;

        default:
            DBG_ERR("NVTEVT_ROTATE_EXIF not support this rotation yet!\r\n");
            return NVTEVT_CONSUME;
    }
    DBG_MSG("UIPlay_RotateExif Rotation=0x%x, OldOrient=0x%x, NewOrient=0x%x\r\n",Rotation, OldOrient, NewOrient);
    #if 0
    Curindex = DCF_GetDBInfo(DCF_INFO_CUR_INDEX);
    DCF_GetObjInfo(Curindex,&uiDirNum,&uiFileNum,&uiFileType);
    uiFileType &= ~DCF_FILE_TYPE_WAV;
    DCF_GetObjPath(Curindex,uiFileType,chaFullName);
    filehdl = FileSys_OpenFile((char *)chaFullName,FST_OPEN_ALWAYS|FST_OPEN_WRITE);
    FileSys_SeekFile(filehdl, exifTag.uiTagOffset, FST_SEEK_SET);
    ret = FileSys_WriteFile(filehdl, (UINT8*)&NewOrient, &exifTag.uiTagLen, 0, NULL);
    FileSys_CloseFile(filehdl);
    if(ret != E_OK)
    {
        DBG_ERR("FST open file failed!\r\n");
    }
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYSINGLE, 1, PB_SINGLE_CURR);
    #else
    Config.Operation = PBEDIT_ROTATE;
    Config.puiParam[0] = NewOrient;
    Config.puiParam[1] = TRUE;
    Config.puiParam[2] = paramArray[1];
    PB_EditImage(&Config);
    g_uiStatus = (UINT32)PB_WaitCommandFinish(PB_WAIT_INFINITE);
    #endif

    return NVTEVT_CONSUME;
}
static INT32 UIPlay_NovaLightDisp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PB_EDIT_CONFIG Config = {0};

    if(paramNum != 1)
    {
        DBG_ERR("NVTEVT_NOVALIGHT_DISPLAY PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }

    Config.Operation = PBEDIT_NLIGHT_DISP;
    Config.puiParam[0] = paramArray[0];
    PB_EditImage(&Config);
    PB_WaitCommandFinish(PB_WAIT_INFINITE);

    return NVTEVT_CONSUME;
}
static INT32 UIPlay_NovaLightFix(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PB_EDIT_CONFIG Config = {0};

    //#NT#2009/10/07#Lincy Lin -begin
    //#mark unused function
    #if 1
    if(paramNum != 1)
    {
        DBG_ERR("NVTEVT_NOVALIGHT_FIX PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }
    Config.Operation = PBEDIT_NLIGHT;
    Config.puiParam[0] = paramArray[0];
    PB_EditImage(&Config);
    PB_WaitCommandFinish(PB_WAIT_INFINITE);
    #endif
    //#NT#2009/10/07#Lincy Lin -end

    return NVTEVT_CONSUME;
}
#if 0
static INT32 UIPlay_Copy2Card(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if 1//_DEMO_TODO
    UINT32 uiTempBufAddr, uiTempBufSize, FileNumber;
    INT32  uiError_Code;
    //HNVT_STRG pSrcObj, pDstObj, pOriObj;
    DX_HANDLE pSrcDXH, pDstDXH, pOriDXH;
    COPYTO_BYNAME_INFO CopyInfo;
    UINT32 uiThisFileFormat;
    UINT32 DirID,FileID;
    UINT32 NewDirID,NewFileID,i;
    CHAR   srcPath[DCF_FULL_FILE_PATH_LEN],dstPath[DCF_FULL_FILE_PATH_LEN];
    UINT32 copMode;
    //DX_HANDLE pStrgDev = 0;

    if(paramNum != 1)
    {
        DBG_ERR("NVTEVT_COPY2CARD PARAMETER ERROR!!!\r\n");
        return NVTEVT_CONSUME;
    }
    copMode = paramArray[0];

    PB_GetParam(PBPRMID_DATABUF_ADDR, &uiTempBufAddr);
    PB_GetParam(PBPRMID_DATABUF_SIZE, &uiTempBufSize);
    if (uiTempBufSize > PB_MAX_COPY_FILE_BUF_SIZE)
    {
        uiTempBufSize = PB_MAX_COPY_FILE_BUF_SIZE;
    }

    //FileSys_GetStrgObj(&pDstObj);
    FileSys_GetStrgObj(&pDstDXH);
    //pOriObj = pDstObj;
    pOriDXH = pDstDXH;
    //pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1);
    //pSrcObj = (HNVT_STRG)Dx_Getcaps(pStrgDev, STORAGE_CAPS_HANDLE, 0);
    pSrcDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1);
    memset(&CopyInfo, 0, sizeof(CopyInfo));
    //CopyInfo.pSrcStrg = pSrcObj;
    //CopyInfo.pDstStrg = pDstObj;
    CopyInfo.pSrcStrg = pSrcDXH;
    CopyInfo.pDstStrg = pDstDXH;
    CopyInfo.pBuf = (char *)uiTempBufAddr;
    CopyInfo.uiBufSize = uiTempBufSize;
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
            DCF_GetNextID(&NewDirID,&NewFileID);

            // get copy file number
            //FileSys_ChangeDisk(pSrcObj);
            FileSys_ChangeDisk(pSrcDXH);
            DCF_ScanObj();
            FileNumber = DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT);
        }
        if (NewDirID == ERR_DCF_DIR_NUM && NewFileID == ERR_DCF_FILE_NUM)
            return E_SYS;
        switch(copMode)
        {
        case PB_COPY2CARD_NEW_DIR:
            if (NewFileID != MIN_DCF_FILE_NUM)
            {
                NewDirID++;
                NewFileID = MIN_DCF_FILE_NUM;
                if (NewDirID > MAX_DCF_DIR_NUM)
                    return E_SYS;
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
                    break;
                }
                NewFileID++;
                if (NewFileID > MAX_DCF_FILE_NUM)
                {
                    NewFileID = 1;
                    NewDirID++;
                    if (NewDirID > MAX_DCF_DIR_NUM)
                        return E_SYS;
                }
            }
            break;

        case PB_COPY2CARD_NO_RENAME:
        case PB_COPY2CARD_NEW_DIR_ORG_FILE:
        default:
            DBG_ERR("Not Support Copy command %d\r\n",copMode);
            break;
        }
    }
    //if (pOriObj!=pSrcObj)
    if (pOriDXH!=pSrcDXH)
    {
        //FileSys_ChangeDisk(pOriObj);
        FileSys_ChangeDisk(pOriDXH);
    }
    DCF_Refresh();
    DCF_SetCurIndex(DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT));
    PB_OpenSpecFileBySeq(DCF_GetDBInfo(DCF_INFO_CUR_INDEX),TRUE);
    PB_WaitCommandFinish(PB_WAIT_INFINITE);
    #else
    #if 0
    PB_SetCopy2Card((UINT8)paramArray[0]);
    g_uiStatus = (UINT32)PB_WaitCommandFinish(PB_WAIT_INFINITE);
    #endif
#endif
    return NVTEVT_CONSUME;
}
#endif
/////////////////////////////////////////////////////////////////////////

EVENT_ENTRY UIPlayObjCmdMap[] =
{
    {NVTEVT_PLAYINIT,          UIPlay_OnInit},
    {NVTEVT_PLAYSINGLE,        UIPlay_OnSingle},
    {NVTEVT_DELETE,            UIPlay_Delete},
    {NVTEVT_PROTECT,           UIPlay_Protect},
    {NVTEVT_ZOOMINIT,          UIZoom_OnInit},
    {NVTEVT_ZOOMINOUT,         UIZoom_OnZoomInOut},
    {NVTEVT_ZOOMNAVI,          UIZoom_OnZoomNavi},
    {NVTEVT_BROWSEINIT,        UIBrowse_OnInit},
    {NVTEVT_BROWSESINGLE,      UIBrowse_OnSingle},
    {NVTEVT_BROWSEFIN,         UIBrowse_OnFinish},
    {NVTEVT_UPDATE_RSVDATA,    UIPlay_UpDateRsvData},
    {NVTEVT_CHANGE_PBSIZE,     UIPlay_ChangePBSize},
    {NVTEVT_RESIZE,            UIPlay_Resize},
    {NVTEVT_FIXREDEYE,         UIPlay_FixRedEye},
    {NVTEVT_RESAVEFILE,        UIPlay_ReSaveFile},
    {NVTEVT_ROTATE_DISPLAY,    UIPlay_RotateDisplay},
    {NVTEVT_ROTATE_EXIF,       UIPlay_RotateExif},
    {NVTEVT_NOVALIGHT_DISPLAY, UIPlay_NovaLightDisp},
    {NVTEVT_NOVALIGHT_FIX,     UIPlay_NovaLightFix},
    //{NVTEVT_COPY2CARD,         UIPlay_Copy2Card},
    //#NT#2011/01/31#Ben Wang -begin
    //#NT#Add screen control to sync display with OSD
    {NVTEVT_SCREEN_CTRL,       UIPlay_SetScreenCtrl},
    //#NT#2011/01/31#Ben Wang -end
    {NVTEVT_NULL,                   0},  //End of Command Map
};

CREATE_APP(UIPlayObj,APP_SETUP)

/////////////////////////////////////////////////////////////////////////

void AppPlay_SetData(PLAY_DATA_SET attribute, UINT32 value)
{
    switch(attribute)
    {
        case PLAY_MOVE_EFFECT:
            FlowPlaySingleObj.slideEffectFunc = (UINT32)value;
            break;
        default:
            DBG_ERR("[AppPlay_SetData]no this attribute");
            break;
    }
}

UINT32 AppPlay_GetData(PLAY_DATA_SET attribute)
{
#if 1//_DEMO_TODO
    JPGHEAD_DEC_CFG JPGInfo = {0};
    PLAY_OBJ PlayObj = {0};
    UINT32 uiParamVal;
    EXIF_GETTAG exifTag;
    BOOL bIsLittleEndian;
    Exif_GetInfo(EXIFINFO_BYTEORDER, &bIsLittleEndian, sizeof(bIsLittleEndian));

    PB_GetParam(PBPRMID_INFO_IMG, (UINT32 *)&JPGInfo);
    PB_GetParam(PBPRMID_PLAYBACK_OBJ, (UINT32 *)&PlayObj);
    //#NT#2010/04/12#Ben Wang -begin
    //#NT#check exif value
    switch(attribute)
    {
        case PLAY_PBSTATUS:
            return g_uiStatus;
        case PLAY_CURRMODE:
            PB_GetParam(PBPRMID_PLAYBACK_MODE, &uiParamVal);
            return uiParamVal;
        case PLAY_LOCKSTATUS:
            PB_GetParam(PBPRMID_FILE_ATTR_LOCK, &uiParamVal);
            return uiParamVal;
        case PLAY_DIRID:
            PB_GetParam(PBPRMID_NAMEID_DIR, &uiParamVal);
            return uiParamVal;
        case PLAY_FILEID:
            PB_GetParam(PBPRMID_NAMEID_FILE, &uiParamVal);
            return uiParamVal;
        case PLAY_FILESEQ:
            PB_GetParam(PBPRMID_CURR_FILESEQ, &uiParamVal);
            return uiParamVal;
        case PLAY_FILENUM:
            PB_GetParam(PBPRMID_TOTAL_FILE_COUNT, &uiParamVal);
            return uiParamVal;
        case PLAY_FILESIZE:
            PB_GetParam(PBPRMID_CURR_FILESIZE, &uiParamVal);
            return uiParamVal;
        case PLAY_ISO:
            exifTag.uiTagIfd = EXIF_IFD_EXIF;
            exifTag.uiTagId = TAG_ID_ISO_SPEED_RATING;
            if(E_OK == Exif_GetTag(&exifTag))
            {
                uiParamVal = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
            }
            else
            {
                uiParamVal = EXIF_NOT_EXIST;
            }
            return uiParamVal;
        case PLAY_ZOOMINDEX:
            PB_GetParam(PBPRMID_ZOOM_INDEX, &uiParamVal);
            return uiParamVal;
        case PLAY_BROWSEINDEX:
            PB_GetParam(PBPRMID_THUMB_CURR_IDX, &uiParamVal);
            return uiParamVal;
        case PLAY_PAGE_CHANGED:
            return (UINT32)g_bPagechanged;
        case PLAY_ROTATION:
            PB_GetParam(PBPRMID_ROTATE_DIR, &uiParamVal);
            return uiParamVal;
        case PLAY_IMGWIDTH:
            return (UINT32)JPGInfo.imagewidth;
        case PLAY_IMGHEIGHT:
            return (UINT32)JPGInfo.imageheight;
        case PLAY_IMGWIDTH_ORI:
            exifTag.uiTagIfd = EXIF_IFD_EXIF;
            exifTag.uiTagId = TAG_ID_PIXEL_X_DIMENSION;
            if(E_OK == Exif_GetTag(&exifTag))
            {
                uiParamVal = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
            }
            else
            {
                uiParamVal = (UINT32)JPGInfo.ori_imagewidth;
            }
            return uiParamVal;
        case PLAY_IMGHEIGHT_ORI:
            exifTag.uiTagIfd = EXIF_IFD_EXIF;
            exifTag.uiTagId = TAG_ID_PIXEL_Y_DIMENSION;
            if(E_OK == Exif_GetTag(&exifTag))
            {
                uiParamVal = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
            }
            else
            {
                uiParamVal = (UINT32)JPGInfo.ori_imageheight;
            }
            return uiParamVal;
        case PLAY_PANSRCX:
            PB_GetParam(PBPRMID_PAN_CURX, &uiParamVal);
            return uiParamVal;
        case PLAY_PANSRCY:
            PB_GetParam(PBPRMID_PAN_CURY, &uiParamVal);
            return uiParamVal;
        case PLAY_PANMAXX:
            PB_GetParam(PBPRMID_PAN_MAXX, &uiParamVal);
            return uiParamVal;
        case PLAY_PANMAXY:
            PB_GetParam(PBPRMID_PAN_MAXY, &uiParamVal);
            return uiParamVal;
        case PLAY_ISJPGWITHMEMO:
            PB_GetParam(PBPRMID_JPEG_WITH_VOICE, &uiParamVal);
            return uiParamVal;
        case PLAY_ALLTHUMBFMT:
            PB_GetParam(PBPRMID_THUMB_FMT_ARRAY, &uiParamVal);
            return uiParamVal;
        case PLAY_ALLTHUMBSEQ:
            PB_GetParam(PBPRMID_THUMB_SEQ_ARRAY, &uiParamVal);
            return uiParamVal;
        case PLAY_ORIENTATION:
            exifTag.uiTagIfd = EXIF_IFD_0TH;
            exifTag.uiTagId = TAG_ID_ORIENTATION;
            if(E_OK == Exif_GetTag(&exifTag))
            {
                return Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
            }
            else
                return EXIF_NOT_EXIST;
        case PLAY_EXIF_WB:
            exifTag.uiTagIfd = EXIF_IFD_EXIF;
            exifTag.uiTagId = TAG_ID_WHITE_BALANCE;
            if(E_OK == Exif_GetTag(&exifTag))
            {
                return Get16BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
            }
            else
                return EXIF_NOT_EXIST;
        case PLAY_FILEFMT:
            PB_GetParam(PBPRMID_CURR_FILEFMT, &uiParamVal);
            return uiParamVal;
        case PLAY_ALLTHUMBMOVIELENGTH:
            PB_GetParam(PBPRMID_THUMB_VDO_LTH_ARRAY, &uiParamVal);
            return uiParamVal;
        case PLAY_ALLTHUMBDEC:
            PB_GetParam(PBPRMID_THUMB_DEC_ARRAY, &uiParamVal);
            return uiParamVal;
        case PLAY_CURRTHUMBNUM:
            PB_GetParam(PBPRMID_THUMB_CURR_NUM, &uiParamVal);
            return uiParamVal;
        default:
            DBG_ERR("[AppPlay_GetData]no this attribute %d!\r\n", attribute);
            return 0;
        //#NT#2010/04/12#Ben Wang -end
    }
#endif
}

