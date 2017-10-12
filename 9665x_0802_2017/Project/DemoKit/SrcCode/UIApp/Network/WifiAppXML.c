#ifdef __ECOS
#include "FileSysTsk.h"
#include <cyg\hfs\hfs.h>
#include "FileDB.h"
#include "PBXFileList_FileDB.h"
#include "DCF.h"
#include "SysCommon.h"
#include "WifiAppCmd.h"
#include "UIPlayObj.h"
#include "UIMovieObj.h"
#include "PrjCfg.h"
#include "Exif.h"
#include "HwMem.h"
#include "UIInfo.h"
#include "GxVideoFile.h"
#include "WifiAppXML.h"
#include "ProjectInfo.h"
#include "UIFlow.h"
#include "UIAppNetwork.h"
#include "MovieInterface.h"
#include "FwSrvApi.h"
#include "WifiAppCmdMapping.h"
#include "nvt_auth.h"

#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          WifiAppXML
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#define QUERY_XML_PATH  "A:\\query.xml"
#define QUERY_CMD_CUR_STS_XML_PATH  "A:\\query_cmd_cur_sts.xml"
#define CMD_STR "custom=1&cmd="
#define PAR_STR "&par="
#define QUERY_FMT  "{\"mfr\":\"%s\",\"type\":\"%d\",\"model\":\"%s\",\"p2p_uuid\":\"\"}"
/*mfr:廠商名稱
type:1-行車紀錄器 2- IP camera
model:產品型號
p2p_uuid:用戶p2p標誌,沒有可以不填 ""
*/
typedef enum _MOVIE_REC_TYPE{
    MOVIE_REC_TYPE_UNKNOWN,
    MOVIE_REC_TYPE_FRONT,
    MOVIE_REC_TYPE_BEHIND,
    MOVIE_REC_TYPE_DUAL,
    MOVIE_REC_TYPE_CLONE,
}MOVIE_REC_TYPE;

#define FAT_GET_DAY_FROM_DATE(x)     (x & 0x1F)              ///<  get day from date
#define FAT_GET_MONTH_FROM_DATE(x)   ((x >> 5) & 0x0F)       ///<  get month from date
#define FAT_GET_YEAR_FROM_DATE(x)    ((x >> 9) & 0x7F)+1980  ///<  get year from date
#define FAT_GET_SEC_FROM_TIME(x)     (x & 0x001F)<<1         ///<  seconds(2 seconds / unit)
#define FAT_GET_MIN_FROM_TIME(x)     (x & 0x07E0)>>5         ///<  Minutes
#define FAT_GET_HOUR_FROM_TIME(x)    (x & 0xF800)>>11        ///<  hours
#define XML_STRCPY(dst, src, dst_size)  do { strncpy(dst, src, dst_size-1); dst[dst_size-1] = '\0'; } while(0)


void XML_StringResult(UINT32 cmd,char *str,cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType);
void XML_ValueResult(UINT32 cmd,UINT64 value,cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType);


#define HW_TV_ENABLE           0x00000001
#define HW_GSENDOR_ENABLE      0x00000002
//#NT#2016/06/01#Isiah Chang -begin
//#NT#add a Wi-Fi command to get current mode status.
#define HW_WIFI_SOCKET_HANDSHAKE_ENABLE      0x00000004
//#NT#2016/06/01#Isiah Chang -end
#define HW_WIFI_CLIENT_SOCKET_NOTIFY_ENABLE  0x00000008

static FILEDB_INIT_OBJ gWifiFDBInitObj={
                         "A:\\",  //rootPath
                         NULL,  //defaultfolder
                         NULL,  //filterfolder
                         TRUE,  //bIsRecursive
                         TRUE,  //bIsCyclic
                         TRUE,  //bIsMoveToLastFile
                         TRUE, //bIsSupportLongName
                         FALSE, //bIsDCFFileOnly
                         TRUE,  //bIsFilterOutSameDCFNumFolder
                         {'N','V','T','I','M'}, //OurDCFDirName[5]
                         {'I','M','A','G'}, //OurDCFFileName[4]
                         FALSE,  //bIsFilterOutSameDCFNumFile
                         FALSE, //bIsChkHasFile
                         60,    //u32MaxFilePathLen
                         10000,  //u32MaxFileNum
                         (FILEDB_FMT_JPG|FILEDB_FMT_AVI|FILEDB_FMT_MOV|FILEDB_FMT_MP4),
                         0,     //u32MemAddr
                         0,     //u32MemSize
                         NULL   //fpChkAbort

};

UINT32 XML_snprintf(char** buf, UINT32 *size, const char* fmt, ... )
{
    UINT32 len =0;
    va_list marker;

    va_start( marker, fmt );     // Initialize variable arguments.

    len = vsnprintf(*buf, *size, fmt, marker);

    va_end( marker );              // Reset variable arguments.

    *buf+=len;
    *size = *size-len;
    return len;
}


static void XML_ecos2NvtPath(const char* inPath, char* outPath)
{
    outPath+=sprintf(outPath,"A:");
    //inPath+=strlen(MOUNT_FS_ROOT);
    while (*inPath !=0)
    {
        if (*inPath == '/')
            *outPath = '\\';
        else
            *outPath = *inPath;
        inPath++;
        outPath++;
    }
    //*outPath++ = '\\'; //If adding this, it will be regarded as folder.
    *outPath = 0;
}
static void XML_Nvt2ecosPath(const char* inPath, char* outPath)
{
    //outPath+=sprintf(outPath,MOUNT_FS_ROOT);
    inPath+=strlen("A:");
    while (*inPath !=0)
    {
        if (*inPath == '\\')
            *outPath = '/';
        else
            *outPath = *inPath;
        inPath++;
        outPath++;
    }
    *outPath = 0;
}
#if USE_FILEDB
int XML_PBGetData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    static UINT32     gIndex;
    UINT32            len, fileCount, i, bufflen = *bufSize;
    char*             buf = (char*)bufAddr;
    PPBX_FLIST_OBJ    pFlist = PBXFList_FDB_getObject();
    UINT32            FileDBHandle = 0;

    Ux_SendEvent(0,NVTEVT_PLAYINIT,0);

    if (segmentCount == 0)
    {
        len = sprintf(buf,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");
        buf+=len;
    }

    pFlist->GetInfo(PBX_FLIST_GETINFO_DB_HANDLE,&FileDBHandle,0);
    fileCount = FileDB_GetTotalFileNum(FileDBHandle);

    if (segmentCount == 0)
    {
        // reset some global variables
        gIndex = 0;
    }
    DBG_IND("gIndex = %d %d \r\n",gIndex,fileCount);
    for (i=gIndex;i<fileCount;i++)
    {
        PFILEDB_FILE_ATTR        pFileAttr;
        // check buffer length , reserved 512 bytes
        // should not write data over buffer length
        if ((cyg_uint32)buf-bufAddr > bufflen - 512)
        {
            DBG_IND("totallen=%d >  bufflen(%d)-512\r\n",(cyg_uint32)buf-bufAddr,bufflen);
            *bufSize = (cyg_uint32)(buf)-bufAddr;
            gIndex = i;
            return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
        }
        // get dcf file
        pFileAttr = FileDB_SearhFile(FileDBHandle, i);

        //debug_msg("file %s %d\r\n",pFileAttr->filePath,pFileAttr->fileSize);

        {
            len = sprintf(buf,"<ALLFile><File>\n<NAME>%s</NAME>\n<FPATH>%s</FPATH>\n",pFileAttr->filename,pFileAttr->filePath);
            buf+=len;
            len = sprintf(buf,"<SIZE>%d</SIZE>\n<TIMECODE>%ld</TIMECODE>\n<TIME>%04d/%02d/%02d %02d:%02d:%02d</TIME>\n<ATTR>%d</ATTR></File>\n</ALLFile>\n",pFileAttr->fileSize,(pFileAttr->lastWriteDate <<16) + pFileAttr->lastWriteTime,
                  FAT_GET_YEAR_FROM_DATE(pFileAttr->lastWriteDate),FAT_GET_MONTH_FROM_DATE(pFileAttr->lastWriteDate),FAT_GET_DAY_FROM_DATE(pFileAttr->lastWriteDate),
                  FAT_GET_HOUR_FROM_TIME(pFileAttr->lastWriteTime),FAT_GET_MIN_FROM_TIME(pFileAttr->lastWriteTime),FAT_GET_SEC_FROM_TIME(pFileAttr->lastWriteTime),
                  pFileAttr->attrib );
            buf+=len;
        }
    }
    len = sprintf(buf,"</LIST>\n");
    buf+=len;
    *bufSize = (cyg_uint32)(buf)-bufAddr;
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}


#else

int XML_PBGetData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    static UINT32     gIndex;
    UINT32            len, fileCount, i, bufflen = *bufSize, FileType;
    char*             buf = (char*)bufAddr;
    char              tempPath[128];
    FST_FILE_STATUS   FileStat;
    FST_FILE          filehdl;
    char              dcfFilePath[128];

    // set the data mimetype
    strcpy(mimeType,"text/xml");
    XML_ecos2NvtPath(path, tempPath);

    if (segmentCount == 0)
    {
        len = sprintf(buf,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");
        buf+=len;
    }
    fileCount = DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT);
    if (segmentCount == 0)
    {
        // reset some global variables
        gIndex = 1;
    }
    DBG_IND("gIndex = %d %d \r\n",gIndex,fileCount);
    for (i=gIndex;i<=fileCount;i++)
    {
        // check buffer length , reserved 512 bytes
        // should not write data over buffer length
        if ((cyg_uint32)buf-bufAddr > bufflen - 512)
        {
            DBG_IND("totallen=%d >  bufflen(%d)-512\r\n",(cyg_uint32)buf-bufAddr,bufflen);
            *bufSize = (cyg_uint32)(buf)-bufAddr;
            gIndex = i;
            return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
        }
        // get dcf file
        DCF_SetCurIndex(i);
        FileType = DCF_GetDBInfo(DCF_INFO_CUR_FILE_TYPE);

        if (FileType & DCF_FILE_TYPE_JPG)
        {
            DCF_GetObjPath(i, DCF_FILE_TYPE_JPG, dcfFilePath);
        }
        else if (FileType & DCF_FILE_TYPE_MOV)
        {
            DCF_GetObjPath(i, DCF_FILE_TYPE_MOV, dcfFilePath);
        }
        else
        {
            continue;
        }
        // get file state
        filehdl = FileSys_OpenFile(dcfFilePath,FST_OPEN_READ);
        FileSys_StatFile(filehdl, &FileStat);
        FileSys_CloseFile(filehdl);
        XML_Nvt2ecosPath(dcfFilePath,tempPath);

        // this is a dir
        if (M_IsDirectory(FileStat.uiAttrib))
        {
            len = sprintf(buf,"<Dir>\n<NAME>\n<![CDATA[%s]]></NAME><FPATH>\n<![CDATA[%s]]></FPATH>",&tempPath[15],tempPath);
            buf+=len;
            len = sprintf(buf,"<TIMECODE>%ld</TIMECODE><TIME>%04d/%02d/%02d %02d:%02d:%02d</TIME>\n</Dir>\n",(FileStat.uiModifiedDate <<16) + FileStat.uiModifiedTime,
                  FAT_GET_YEAR_FROM_DATE(FileStat.uiModifiedDate),FAT_GET_MONTH_FROM_DATE(FileStat.uiModifiedDate),FAT_GET_DAY_FROM_DATE(FileStat.uiModifiedDate),
                  FAT_GET_HOUR_FROM_TIME(FileStat.uiModifiedTime),FAT_GET_MIN_FROM_TIME(FileStat.uiModifiedTime),FAT_GET_SEC_FROM_TIME(FileStat.uiModifiedTime));
            buf+=len;
        }
        // this is a file
        else
        {
            len = sprintf(buf,"<ALLFile><File>\n<NAME>\n<![CDATA[%s]]></NAME><FPATH>\n<![CDATA[%s]]></FPATH>",&tempPath[15],tempPath);
            buf+=len;
            len = sprintf(buf,"<SIZE>%lld</SIZE>\n<TIMECODE>%ld</TIMECODE>\n<TIME>%04d/%02d/%02d %02d:%02d:%02d</TIME>\n</File>\n</ALLFile>\n",FileStat.uiFileSize,(FileStat.uiModifiedDate <<16) + FileStat.uiModifiedTime,
                  FAT_GET_YEAR_FROM_DATE(FileStat.uiModifiedDate),FAT_GET_MONTH_FROM_DATE(FileStat.uiModifiedDate),FAT_GET_DAY_FROM_DATE(FileStat.uiModifiedDate),
                  FAT_GET_HOUR_FROM_TIME(FileStat.uiModifiedTime),FAT_GET_MIN_FROM_TIME(FileStat.uiModifiedTime),FAT_GET_SEC_FROM_TIME(FileStat.uiModifiedTime));
            buf+=len;
        }
    }
    len = sprintf(buf,"</LIST>\n");
    buf+=len;
    *bufSize = (cyg_uint32)(buf)-bufAddr;
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}
#endif


int XML_GetModeData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32            len;
    char*             buf = (char*)bufAddr;
    UINT32            par =0;
    char              tmpString[32];

    // set the data mimetype
    strcpy(mimeType,"text/xml");
    DBG_IND("path = %s, argument -> %s, mimeType= %s, bufsize= %d, segmentCount= %d\r\n",path,argument, mimeType, *bufSize, segmentCount);

    sprintf(tmpString,"custom=1&cmd=%d&par=",WIFIAPP_CMD_MODECHANGE);
    if(strncmp(argument,tmpString,strlen(tmpString))==0)
    {
        sscanf(argument+strlen(tmpString),"%d",&par);
    }
    else
    {
        *bufSize =0;
        DBG_ERR("error par\r\n");
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }

    if (par == WIFI_APP_MODE_PLAYBACK)
    {
        return XML_PBGetData(path,argument,bufAddr,bufSize,mimeType,segmentCount);
    }
    else if (par == WIFI_APP_MODE_PHOTO)
    {
        len = sprintf(buf,DEF_XML_HEAD);
        buf+=len;
        len = sprintf(buf,"<FREEPICNUM>%d</FREEPICNUM>\n",UI_GetData(FL_WIFI_PHOTO_FREEPICNUM));
        buf+=len;
        *bufSize = (cyg_uint32)(buf)-bufAddr;

        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }
    else if (par == WIFI_APP_MODE_MOVIE)
    {
        len = sprintf(buf,DEF_XML_HEAD);
        buf+=len;
        len = sprintf(buf,"<MAXRECTIME>%d</MAXRECTIME>\n",UI_GetData(FL_WIFI_MOVIE_MAXRECTIME));
        buf+=len;
        *bufSize = (cyg_uint32)(buf)-bufAddr;
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }

    *bufSize =0;
     DBG_ERR("error mode\r\n");
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}




int XML_QueryCmd(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32            len, i=0;
    char*             buf = (char*)bufAddr;
    FST_FILE          filehdl;
    char              pFilePath[64];
    UINT32            fileLen =*bufSize-512;
    UINT32            bufflen =*bufSize-512;
    WIFI_CMD_ENTRY*   appCmd=0;
    // set the data mimetype
    strcpy(mimeType,"text/xml");
    sprintf(pFilePath,"%s",QUERY_XML_PATH);  //html of all command list
    filehdl = FileSys_OpenFile(pFilePath,FST_OPEN_READ);
    if(filehdl)
    {
        FileSys_ReadFile(filehdl, (UINT8 *)buf, &fileLen, 0,0);
        FileSys_CloseFile(filehdl);
        *bufSize = fileLen;
        *(buf+fileLen) ='\0';
    }
    else
    {
        if (segmentCount == 0)
        {
            len = sprintf(buf,DEF_XML_HEAD);
            buf+=len;
        }

        len = sprintf(buf,"<Function>\n");
        buf+=len;
        appCmd= WifiCmd_GetExecTable();
        while(appCmd[i].cmd != 0)
        {
            len = sprintf(buf,"<Cmd>%d</Cmd>\n",appCmd[i].cmd);
            buf+=len;
            i++;
        }
        len = sprintf(buf,"</Function>\n");
        buf+=len;

        *bufSize = (cyg_uint32)(buf)-bufAddr;
    }

    if(*bufSize > bufflen)
    {
        DBG_ERR("no buffer\r\n");
        *bufSize= bufflen;
    }


    return CYG_HFS_CB_GETDATA_RETURN_OK;
}


FST_FILE          gMovFilehdl;
ER XML_VdoReadCB(UINT32 pos, UINT32 size, UINT32 addr)
{
    ER result =E_SYS;

    DBG_IND("XML_VdoReadCB  pos=0x%x, size=%d, addr=0x%x\r\n",pos,size,addr);
    if(gMovFilehdl)
    {
        FileSys_SeekFile(gMovFilehdl, pos, FST_SEEK_SET);
        //not close file,close file in XML_GetThumbnail()
        result=FileSys_ReadFile(gMovFilehdl, (UINT8 *)addr, &size, 0,0);
    }
    return result;
}
#if 1
int XML_GetThumbnail(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    char*             buf = (char*)bufAddr;
    FST_FILE          filehdl;
    char              tempPath[128];
    UINT32            TempLen,TempBuf;
    UINT32            bufflen = *bufSize-512;
    UINT32            ThumbOffset =0;
    UINT32            ThumbSize =0;
    char*             pch;
    UINT32            result =0;

    if(!SxCmd_GetTempMem(POOL_SIZE_FILEDB))
    {
        XML_DefaultFormat(WIFIAPP_CMD_THUMB,WIFIAPP_RET_NOBUF,bufAddr,bufSize,mimeType);
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }

    // set the data mimetype
    strcpy(mimeType,"image/jpeg");
    XML_ecos2NvtPath(path, tempPath);

    pch=strchr(tempPath,'.');

    if((!path)||(!pch))
    {
        XML_DefaultFormat(WIFIAPP_CMD_THUMB,WIFIAPP_RET_PAR_ERR,bufAddr,bufSize,mimeType);
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }

    if((strncmp(pch+1,"mov",3)==0)||(strncmp(pch+1,"MOV",3)==0)||(strncmp(pch+1,"mp4",3)==0)||(strncmp(pch+1,"MP4",3)==0))
    {
        FST_FILE_STATUS FileStat;
        GXVIDEO_INFO VideoInfo;
        MEM_RANGE WorkBuf;
        UINT32 uiBufferNeeded=0;


        gMovFilehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);
        if(gMovFilehdl)
        {
            FileSys_StatFile(gMovFilehdl,&FileStat);
            GxVidFile_Query1stFrameWkBufSize(&uiBufferNeeded, FileStat.uiFileSize);
            TempBuf = SxCmd_GetTempMem(uiBufferNeeded);
            //parse video info for single mode only
            WorkBuf.Addr = TempBuf;
            WorkBuf.Size = uiBufferNeeded;
            result = GxVidFile_ParseVideoInfo(XML_VdoReadCB, &WorkBuf, (UINT32)FileStat.uiFileSize, &VideoInfo);
            if((result==GXVIDEO_PRSERR_OK)&&(VideoInfo.uiThumbSize))
            {
                FileSys_SeekFile(gMovFilehdl, VideoInfo.uiThumbOffset, FST_SEEK_SET);
                FileSys_ReadFile(gMovFilehdl, (UINT8 *)buf, &VideoInfo.uiThumbSize, 0,0);
                FileSys_CloseFile(gMovFilehdl);
                *bufSize = VideoInfo.uiThumbSize;
            }
            else
            {
                FileSys_CloseFile(gMovFilehdl);
                result = WIFIAPP_RET_EXIF_ERR;
                DBG_ERR("exif error\r\n");
            }
        }
        else
        {
            result = WIFIAPP_RET_NOFILE;
            DBG_ERR("no %s\r\n",tempPath);
        }

    }
    else
    {

        filehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);

        if(filehdl)
        {
            MEM_RANGE ExifData;
            EXIF_GETTAG exifTag;
            BOOL bIsLittleEndian=0;
            UINT32 uiTiffOffsetBase=0;

            TempLen= MAX_APP1_SIZE;
            TempBuf = SxCmd_GetTempMem(TempLen);
            FileSys_ReadFile(filehdl, (UINT8 *)TempBuf, &TempLen, 0,0);
            FileSys_CloseFile(filehdl);

            ExifData.Size = MAX_APP1_SIZE;
            ExifData.Addr = TempBuf;
            if(EXIF_ER_OK == Exif_ParseExif(&ExifData))
            {
                Exif_GetInfo(EXIFINFO_BYTEORDER, &bIsLittleEndian, sizeof(bIsLittleEndian));
                Exif_GetInfo(EXIFINFO_TIFF_OFFSET_BASE, &uiTiffOffsetBase, sizeof(uiTiffOffsetBase));
                //find thumbnail
                exifTag.uiTagIfd = EXIF_IFD_1ST;
                exifTag.uiTagId = TAG_ID_INTERCHANGE_FORMAT;
                if(E_OK == Exif_GetTag(&exifTag))
                {
                    ThumbOffset = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian) + uiTiffOffsetBase;
                    exifTag.uiTagId = TAG_ID_INTERCHANGE_FORMAT_LENGTH;
                    if(E_OK == Exif_GetTag(&exifTag))
                        ThumbSize = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
                }
            }
            else
            {
                result = WIFIAPP_RET_EXIF_ERR;
                DBG_ERR("exif error\r\n");
            }

            if(ThumbSize)
            {
                if(ThumbSize<bufflen)
                {
                    hwmem_open();
                    hwmem_memcpy((UINT32)buf, TempBuf+ThumbOffset, ThumbSize);
                    hwmem_close();
                    *bufSize = ThumbSize;
                    //debug_msg("photo thumb trans %d\r\n",*bufSize);

                }
                else
                {
                    result = WIFIAPP_RET_NOBUF;
                    DBG_ERR("size too large\r\n");
                }
            }
        }
        else
        {
            result = WIFIAPP_RET_NOFILE;
            DBG_ERR("no %s\r\n",tempPath);
        }
    }

    if(result!=0)
        XML_DefaultFormat(WIFIAPP_CMD_THUMB,result,bufAddr,bufSize,mimeType);

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

#else

#define TRANS_JPG_FROM_H264IDR  1

#if TRANS_JPG_FROM_H264IDR
#include "JpgEnc.h"
#include "SizeConvert.h"
#include "GxImageFile.h"

#define SCALE_BUFF_SIZE     ALIGN_CEIL_32(3840*2160*3/2)
#define JPEG_BS_SIZE        ALIGN_CEIL_32(SCALE_BUFF_SIZE/4)
#define WIFI_RESERVE_SIZE   ALIGN_CEIL_32(2*1024*1024)
#define XML_THUMB_W         320
#define XML_THUMB_H         240

typedef struct
{
    UINT32  uiJpgWidth;     ///< JPEG width
    UINT32  uiJpgHeight;    ///< JPEG height
    UINT32  uiJpgBsAddr;    ///< JPEG bs address
    UINT32  uiJpgBsSize;    ///< JPEG bs size
    UINT32  uiDAR;          ///< original image DAR
    UINT32  uiFileSize;     ///< original file size
} IDR_TO_JPG, *PIDR_TO_JPG;

static INT32 XML_MakeJpgFromIFrame(IDR_TO_JPG *pIdrToJpg, PIMG_BUF pDecodedImgBuf, UINT32 JpgBsBuf)
{
    MEM_RANGE           WorkBuf;
    IMG_BUF             DstImgBuf;
    UINT32              uiDecodeBufSize = 0;
    UINT32              uiTempBuf, uiDecodeBuf, uiScaleBuf;
    UINT32              uiJpgBsBuf, uiJpgBsSize;
    UINT32              uiEncQ;
    INT32               iResult;
    JPG_ENC_CFG         JPGParam = {0};
    UINT32              uiLineOffset[2] = {0};
    UINT32              uiPxlAddr[2] = {0};
    IMG_BUF             SrcImg, DstImg;
    IRECT               SrcRegion = {0};
    IRECT               DstRegion = {0};
    SIZECONVERT_INFO    ScaleInfo = {0};

    if(NULL == pDecodedImgBuf) //make JPG from I frame
    {
        GxVidFile_Query1stFrameWkBufSize(&uiDecodeBufSize, pIdrToJpg->uiFileSize);
        uiTempBuf = SxCmd_GetTempMem(WIFI_RESERVE_SIZE + uiDecodeBufSize + JPEG_BS_SIZE + SCALE_BUFF_SIZE);
        DBG_IND("Temp buf 0x%x, need size 0x%x\r\n", uiTempBuf, uiDecodeBufSize);
        if (!uiTempBuf)
        {
            DBG_ERR("buf not enough!\r\n");
            return E_SYS;
        }

        uiDecodeBuf = uiTempBuf + WIFI_RESERVE_SIZE;
        uiJpgBsBuf  = uiDecodeBuf + uiDecodeBufSize;
        uiScaleBuf  = uiJpgBsBuf + JPEG_BS_SIZE;

        // decode 1st I-frame
        WorkBuf.Addr = uiDecodeBuf;
        WorkBuf.Size = uiDecodeBufSize;
        iResult = GxVidFile_Decode1stFrame(XML_VdoReadCB, &WorkBuf, &DstImgBuf);
        DBG_IND("w %d, h %d, fmt %d, Y 0x%x, U 0x%x, V 0x%x, YL %d, UL %d, VL %d\r\n",
            DstImgBuf.Width, DstImgBuf.Height, DstImgBuf.PxlFmt,
            DstImgBuf.PxlAddr[0], DstImgBuf.PxlAddr[1], DstImgBuf.PxlAddr[2],
            DstImgBuf.LineOffset[0], DstImgBuf.LineOffset[1], DstImgBuf.LineOffset[2]);

        // only support YUV420 UV-packed format
        if (DstImgBuf.PxlFmt != GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
        {
            DBG_ERR("format not 420-packed!\r\n");
            return E_SYS;
        }
    }
    else //make JPG from decoded YUV data
    {
        memcpy((void *) &DstImgBuf, (void *) pDecodedImgBuf, sizeof(IMG_BUF));
        uiJpgBsBuf  = JpgBsBuf;
        uiScaleBuf  = uiJpgBsBuf + JPEG_BS_SIZE;
    }

    ///////////////////////////////
    // scaled to JPEG resolution //
    ///////////////////////////////

    // Init source buffer
    uiLineOffset[0] = DstImgBuf.LineOffset[0];
    uiLineOffset[1] = DstImgBuf.LineOffset[1];
    uiPxlAddr[0]    = DstImgBuf.PxlAddr[0];
    uiPxlAddr[1]    = DstImgBuf.PxlAddr[1];

    GxImg_InitBufEx(&SrcImg,
                    DstImgBuf.Width,
                    DstImgBuf.Height,
                    DstImgBuf.PxlFmt,
                    uiLineOffset,
                    uiPxlAddr);

    // Init destination buffer
    uiLineOffset[0] = ALIGN_CEIL_32(pIdrToJpg->uiJpgWidth);
    uiLineOffset[1] = ALIGN_CEIL_32(pIdrToJpg->uiJpgWidth);
    uiPxlAddr[0]    = uiScaleBuf;
    uiPxlAddr[1]    = uiScaleBuf + ALIGN_CEIL_32(pIdrToJpg->uiJpgWidth) * ALIGN_CEIL_16(pIdrToJpg->uiJpgHeight);

    GxImg_InitBufEx(&DstImg,
                    pIdrToJpg->uiJpgWidth,
                    pIdrToJpg->uiJpgHeight,
                    DstImgBuf.PxlFmt,
                    uiLineOffset,
                    uiPxlAddr);

    // Set scale region
    SrcRegion.x = 0;
    SrcRegion.y = 0;
    SrcRegion.w = DstImgBuf.Width;
    SrcRegion.h = DstImgBuf.Height;

    if (pIdrToJpg->uiDAR == VIDENC_DAR_16_9)
    {
        ScaleInfo.uiSrcWidth  = 16;
        ScaleInfo.uiSrcHeight = 9;
    }
    else
    {
        ScaleInfo.uiSrcWidth  = DstImgBuf.Width;
        ScaleInfo.uiSrcHeight = DstImgBuf.Height;
    }
    ScaleInfo.uiDstWidth  = pIdrToJpg->uiJpgWidth;
    ScaleInfo.uiDstHeight = pIdrToJpg->uiJpgHeight;
    ScaleInfo.uiDstWRatio = pIdrToJpg->uiJpgWidth;
    ScaleInfo.uiDstHRatio = pIdrToJpg->uiJpgHeight;
    ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_32;
    DisplaySizeConvert(&ScaleInfo);

    DstRegion.x = ScaleInfo.uiOutX;
    DstRegion.y = ScaleInfo.uiOutY;
    DstRegion.w = ScaleInfo.uiOutWidth;
    DstRegion.h = ScaleInfo.uiOutHeight;
    DBG_IND("Dst: x %d, y %d, w %d, h %d\r\n", DstRegion.x, DstRegion.y, DstRegion.w, DstRegion.h);

    // Clear buffer by black
    GxImg_FillData(&DstImg, REGION_MATCH_IMG, COLOR_YUV_BLACK);

    // Scale image by ISE
    GxImg_ScaleData(&SrcImg, &SrcRegion, &DstImg, &DstRegion);

    //////////////////////
    // do JPEG encoding //
    //////////////////////
    uiJpgBsSize                 = JPEG_BS_SIZE;
    JPGParam.Width              = pIdrToJpg->uiJpgWidth;
    JPGParam.Height             = pIdrToJpg->uiJpgHeight;
    JPGParam.JPGBSAddr          = uiJpgBsBuf;
    JPGParam.JPGBSBufSize       = &uiJpgBsSize;
    JPGParam.RawImageAddr[0]    = uiPxlAddr[0];
    JPGParam.RawImageAddr[1]    = uiPxlAddr[1];
    JPGParam.RawImageAddr[2]    = uiPxlAddr[1];
    JPGParam.LineOffset[0]      = uiLineOffset[0];
    JPGParam.LineOffset[1]      = uiLineOffset[1];
    JPGParam.LineOffset[2]      = uiLineOffset[1];
    JPGParam.YUVFormat          = JPG_FMT_YUV420;
    JPGParam.PxlLayout          = JPG_PXL_UVPACKED;
    JPGParam.DCOutCfg.DCEnable  = FALSE;
    JPGParam.BrcEn              = FALSE;

    uiEncQ = 85; // default Q
    while (uiEncQ > 40) // minimum Q
    {
        uiJpgBsSize = JPEG_BS_SIZE; // reset target JPEG bs size to bs buffer size
        DBG_IND("Q %d, bs size %d, buf size %d\r\n", uiEncQ, uiJpgBsSize, JPEG_BS_SIZE);
        iResult = Jpg_EncOneImg(&JPGParam, uiEncQ);
        if (iResult == E_OK)
        {
            break;
        }
        uiEncQ -= 5;
    }
    DBG_IND("^M Q %d, bs size %d, buf size %d\r\n", uiEncQ, uiJpgBsSize, JPEG_BS_SIZE);

    pIdrToJpg->uiJpgBsAddr = uiJpgBsBuf;
    pIdrToJpg->uiJpgBsSize = uiJpgBsSize;

    return E_OK;
}
#endif

int XML_GetThumbnail(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    char*             buf = (char*)bufAddr;
    FST_FILE          filehdl;
    char              tempPath[128];
    UINT32            TempLen,TempBuf;
    UINT32            bufflen = *bufSize-512;
    UINT32            ThumbOffset =0;
    UINT32            ThumbSize =0;
    static UINT32     remain =0;
    static UINT32     ThumbBuf = 0;
    char*             pch;
    UINT32            result =0;
    #if !TRANS_JPG_FROM_H264IDR
    UINT8*            ptr=0;
    #endif

    // set the data mimetype
    strcpy(mimeType,"image/jpeg");
    XML_ecos2NvtPath(path, tempPath);

    pch=strchr(tempPath,'.');

    if((!path)||(!pch))
    {
        XML_DefaultFormat(WIFIAPP_CMD_THUMB,WIFIAPP_RET_PAR_ERR,bufAddr,bufSize,mimeType);
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }

    if((strncmp(pch+1,"mov",3)==0)||(strncmp(pch+1,"MOV",3)==0)||(strncmp(pch+1,"mp4",3)==0)||(strncmp(pch+1,"MP4",3)==0))
    {
        if(segmentCount ==0)
        {
            FST_FILE_STATUS FileStat;
            GXVIDEO_INFO VideoInfo;
            MEM_RANGE WorkBuf;
            UINT32 uiBufferNeeded=0;


            gMovFilehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);
            if(gMovFilehdl)
            {
                FileSys_StatFile(gMovFilehdl,&FileStat);
                GxVidFile_Query1stFrameWkBufSize(&uiBufferNeeded, FileStat.uiFileSize);
                TempBuf = SxCmd_GetTempMem(uiBufferNeeded);

                if(!TempBuf)
                {
                    FileSys_CloseFile(gMovFilehdl);
                    result = WIFIAPP_RET_NOBUF;
                    DBG_ERR("no buf\r\n");
                }
                else
                {
                    //parse video info for single mode only
                    WorkBuf.Addr = TempBuf;
                    WorkBuf.Size = uiBufferNeeded;
                    result = GxVidFile_ParseVideoInfo(XML_VdoReadCB, &WorkBuf, (UINT32)FileStat.uiFileSize, &VideoInfo);
                    if((result==GXVIDEO_PRSERR_OK)&&(VideoInfo.uiThumbSize))
                    {
                        FileSys_SeekFile(gMovFilehdl, VideoInfo.uiThumbOffset, FST_SEEK_SET);
                        FileSys_ReadFile(gMovFilehdl, (UINT8 *)TempBuf, &VideoInfo.uiThumbSize, 0,0);
                        FileSys_CloseFile(gMovFilehdl);
                        //*bufSize = VideoInfo.uiThumbSize;
                        remain = VideoInfo.uiThumbSize;
                        ThumbBuf = TempBuf;
                    }
                    else//VideoInfo.ui1stFrameSize) // Return 1st I frame back to APP.
                    {
                        #if TRANS_JPG_FROM_H264IDR
                        IDR_TO_JPG          IdrToJpg = {0};
                        INT32               iResult;

                        // no DAR param now, suppose 2880x2160 is DAR 16:9
                        if ((VideoInfo.uiVidWidth == 2880) && (VideoInfo.uiVidHeight== 2160))
                        {
                            IdrToJpg.uiDAR = VIDENC_DAR_16_9;
                        }
                        else
                        {
                            IdrToJpg.uiDAR = VIDENC_DAR_DEFAULT;
                        }
                        IdrToJpg.uiJpgWidth  = XML_THUMB_W;
                        IdrToJpg.uiJpgHeight = XML_THUMB_H;
                        IdrToJpg.uiFileSize  = FileStat.uiFileSize;
                        iResult = XML_MakeJpgFromIFrame(&IdrToJpg, 0, 0);

                        FileSys_CloseFile(gMovFilehdl);

                        if (iResult == E_OK)
                        {
                            ThumbBuf = IdrToJpg.uiJpgBsAddr;
                            remain = IdrToJpg.uiJpgBsSize;
                        }
                        else
                        {
                            result = WIFIAPP_RET_EXIF_ERR;
                            DBG_ERR("No Thumbnail and I frame.\r\n");
                            ThumbBuf = 0;
                            remain = 0;
                        }
                        #else
                        FileSys_CloseFile(gMovFilehdl);
                        result = WIFIAPP_RET_EXIF_ERR;
                        DBG_ERR("No Thumbnail and I frame.\r\n");
                        #endif
                    }
                }
            }
            else
            {
                result = WIFIAPP_RET_NOFILE;
                DBG_ERR("no %s\r\n",tempPath);
            }
        }
    }
    else
    {
        if(segmentCount ==0)
        {

            filehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);

            if(filehdl)
            {
                MEM_RANGE ExifData;
                EXIF_GETTAG exifTag;
                BOOL bIsLittleEndian=0;
                UINT32 uiTiffOffsetBase=0;

                TempLen= MAX_APP1_SIZE;
                TempBuf = SxCmd_GetTempMem(TempLen);
                FileSys_ReadFile(filehdl, (UINT8 *)TempBuf, &TempLen, 0,0);
                FileSys_CloseFile(filehdl);

                ExifData.Size = MAX_APP1_SIZE;
                ExifData.Addr = TempBuf;
                if(EXIF_ER_OK == EXIF_ParseExif(EXIF_HDL_ID_1, &ExifData))
                {
                    EXIF_GetInfo(EXIF_HDL_ID_1, EXIFINFO_BYTEORDER, &bIsLittleEndian, sizeof(bIsLittleEndian));
                    EXIF_GetInfo(EXIF_HDL_ID_1, EXIFINFO_TIFF_OFFSET_BASE, &uiTiffOffsetBase, sizeof(uiTiffOffsetBase));
                    //find thumbnail
                    exifTag.uiTagIfd = EXIF_IFD_1ST;
                    exifTag.uiTagId = TAG_ID_INTERCHANGE_FORMAT;
                    if(E_OK == EXIF_GetTag(EXIF_HDL_ID_1, &exifTag))
                    {
                        ThumbOffset = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian) + uiTiffOffsetBase;
                        exifTag.uiTagId = TAG_ID_INTERCHANGE_FORMAT_LENGTH;
                        if(E_OK == EXIF_GetTag(EXIF_HDL_ID_1, &exifTag))
                            ThumbSize = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
                    }
                }
                else
                {

                    GXIMGDEC_SCALE_DOWN_LEVEL ScaleDownlevel;
                    MEM_RANGE AvailableMem;
                    MEM_RANGE SrcFile;
                    UINT32              uiTempBuf, uiDecodeBuf,uiDecodeBufSize, uiJpgBsBuf;
                    IMG_BUF DecodedImgBuf = {0};
                    GXIMG_HEADER_INFO HeaderInfo = {0};
                    uiDecodeBufSize = SCALE_BUFF_SIZE;
                    uiTempBuf = SxCmd_GetTempMem(WIFI_RESERVE_SIZE + uiDecodeBufSize + JPEG_BS_SIZE + SCALE_BUFF_SIZE);

                    TempLen= FileSys_GetFileLen(tempPath);
                    if(TempLen > WIFI_RESERVE_SIZE)
                    {
                        result = WIFIAPP_RET_NOBUF;
                        DBG_ERR("size too large\r\n");
                        goto GET_THUMBNAIL_RET;
                    }
                    filehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);
                    FileSys_ReadFile(filehdl, (UINT8 *)uiTempBuf, &TempLen, 0,0);
                    FileSys_CloseFile(filehdl);

                    SrcFile.Addr = uiTempBuf;
                    SrcFile.Size = TempLen;
                    GxImgFile_ParseHeader(&SrcFile, &HeaderInfo);

                    uiDecodeBuf = uiTempBuf + WIFI_RESERVE_SIZE;
                    uiJpgBsBuf  = uiDecodeBuf + uiDecodeBufSize;

                    AvailableMem.Addr = uiDecodeBuf;
                    AvailableMem.Size = uiDecodeBufSize;
                    if(E_OK == GxImgFile_AllocDecBuf(&DecodedImgBuf, &AvailableMem, &HeaderInfo, &ScaleDownlevel))
                    {
                        ER Ret;
                        if(ScaleDownlevel)
                            Ret = GxImgFile_DecodeByDownsample(&DecodedImgBuf, &SrcFile, ScaleDownlevel);
                        else
                            Ret = GxImgFile_Decode(&DecodedImgBuf, &SrcFile);
                        if(E_OK == Ret)
                        {
                            IDR_TO_JPG          IdrToJpg = {0};
                            INT32               iResult;

                            // no DAR param now, suppose 2880x2160 is DAR 16:9
                            if ((HeaderInfo.uiImageWidth== 2880) && (HeaderInfo.uiImageHeight== 2160))
                            {
                                IdrToJpg.uiDAR = VIDENC_DAR_16_9;
                            }
                            else
                            {
                                IdrToJpg.uiDAR = VIDENC_DAR_DEFAULT;
                            }
                            IdrToJpg.uiJpgWidth  = XML_THUMB_W;
                            IdrToJpg.uiJpgHeight = XML_THUMB_H;
                            iResult = XML_MakeJpgFromIFrame(&IdrToJpg, &DecodedImgBuf, uiJpgBsBuf);
                            if (iResult == E_OK)
                            {
                                ThumbOffset = 0;//means NOT from EXIF
                                ThumbBuf = IdrToJpg.uiJpgBsAddr;
                                ThumbSize = IdrToJpg.uiJpgBsSize;
                            }
                            else
                            {
                                DBG_ERR("Decode Error!(0x%X)\r\n",Ret);
                                result = WIFIAPP_RET_FILE_ERROR;
                                goto GET_THUMBNAIL_RET;
                            }
                        }
                        else
                        {
                            DBG_ERR("Decode Error!(0x%X)\r\n",Ret);
                            result = WIFIAPP_RET_FILE_ERROR;
                            goto GET_THUMBNAIL_RET;

                        }
                    }
                    else
                    {
                        result = WIFIAPP_RET_NOBUF;
                        DBG_ERR("size too large\r\n");
                        goto GET_THUMBNAIL_RET;
                    }
                }

                if(ThumbSize)
                {
                    if(ThumbSize<bufflen)
                    {
                        hwmem_open();
                        if(ThumbOffset)
                            hwmem_memcpy((UINT32)buf, TempBuf+ThumbOffset, ThumbSize);
                        else
                            hwmem_memcpy((UINT32)buf, ThumbBuf, ThumbSize);
                        hwmem_close();
                        *bufSize = ThumbSize;
                        //debug_msg("photo thumb trans %d\r\n",*bufSize);

                    }
                    else
                    {
                        result = WIFIAPP_RET_NOBUF;
                        DBG_ERR("size too large\r\n");
                    }
                }
            }
            else
            {
                result = WIFIAPP_RET_NOFILE;
                DBG_ERR("no %s\r\n",tempPath);
            }

            ThumbBuf = 0; // JPEG thumbnail is very samll. It just takes one time to transfer.
        }
    }

    if(ThumbBuf)
    {
        if(remain>bufflen)
        {
            hwmem_open();
            hwmem_memcpy((UINT32)buf, ThumbBuf+bufflen*segmentCount, bufflen);
            hwmem_close();
            *bufSize = bufflen;
            remain-=bufflen;
            return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
        }
        else
        {
            hwmem_open();
            hwmem_memcpy((UINT32)buf, ThumbBuf+bufflen*segmentCount, remain);
            hwmem_close();
            *bufSize = remain;
            remain=0;
            ThumbBuf =0;
            //debug_msg("last trans ok\r\n");
        }
    }
GET_THUMBNAIL_RET:
    if(result!=0)
        XML_DefaultFormat(WIFIAPP_CMD_THUMB,result,bufAddr,bufSize,mimeType);

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}
#endif

int XML_GetScreen(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    char*             buf = (char*)bufAddr;
    FST_FILE          filehdl;
    char              tempPath[128];
    UINT32            TempLen,TempBuf=0;
    UINT32            bufflen = *bufSize-512;
    UINT32            ScreenOffset =0;
    UINT32            ScreenSize =0;
    static UINT32     remain =0;
    static UINT32     ScreenBuf = 0;
    char*             pch;
    UINT32            result=0;

    if(!SxCmd_GetTempMem(POOL_SIZE_FILEDB))
    {
        XML_DefaultFormat(WIFIAPP_CMD_SCREEN,WIFIAPP_RET_NOBUF,bufAddr,bufSize,mimeType);
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }


    // set the data mimetype
    strcpy(mimeType,"image/jpeg");
    XML_ecos2NvtPath(path, tempPath);

    pch=strchr(tempPath,'.');
    if((strncmp(pch+1,"mov",3)==0)||(strncmp(pch+1,"MOV",3)==0))
    {
        if(segmentCount ==0)
        {
            FST_FILE_STATUS FileStat;
            GXVIDEO_INFO VideoInfo;
            MEM_RANGE WorkBuf;
            UINT32 uiBufferNeeded=0;

            gMovFilehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);
            if(gMovFilehdl)
            {
                FileSys_StatFile(gMovFilehdl,&FileStat);
                GxVidFile_Query1stFrameWkBufSize(&uiBufferNeeded, FileStat.uiFileSize);
                TempBuf = SxCmd_GetTempMem(uiBufferNeeded);
                //parse video info for single mode only
                WorkBuf.Addr = TempBuf;
                WorkBuf.Size = uiBufferNeeded;
                result = GxVidFile_ParseVideoInfo(XML_VdoReadCB, &WorkBuf, (UINT32)FileStat.uiFileSize, &VideoInfo);
                if((result==GXVIDEO_PRSERR_OK)&&(VideoInfo.uiScraSize))
                {
                    FileSys_SeekFile(gMovFilehdl, VideoInfo.uiScraOffset, FST_SEEK_SET);
                    FileSys_ReadFile(gMovFilehdl, (UINT8 *)TempBuf, &VideoInfo.uiScraSize, 0,0);
                    FileSys_CloseFile(gMovFilehdl);
                    remain = VideoInfo.uiScraSize;
                    ScreenBuf = TempBuf;
                }
                else
                {
                    FileSys_CloseFile(gMovFilehdl);
                    result = WIFIAPP_RET_EXIF_ERR;
                    DBG_ERR("exif error\r\n");
                }
            }
            else
            {
                result = WIFIAPP_RET_NOFILE;
                DBG_ERR("no %s\r\n",tempPath);
            }

        }

    }
    else
    {

        if(segmentCount ==0)
        {
            filehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);

            if(filehdl)
            {
                MEM_RANGE ExifData;
                MAKERNOTE_INFO MakernoteInfo;

                TempLen= MAX_APP1_SIZE;
                TempBuf = SxCmd_GetTempMem(TempLen);
                FileSys_ReadFile(filehdl, (UINT8 *)TempBuf, &TempLen, 0,0);

                ExifData.Size = MAX_APP1_SIZE;
                ExifData.Addr = TempBuf;
                if(EXIF_ER_OK == Exif_ParseExif(&ExifData))
                {
                   //find screennail
                    Exif_GetInfo(EXIFINFO_MAKERNOTE, &MakernoteInfo, sizeof(MakernoteInfo));
                    ScreenOffset = MakernoteInfo.uiScreennailOffset;
                    ScreenSize = MakernoteInfo.uiScreennailSize;
                    //read screennail to buffer
                    ScreenBuf = SxCmd_GetTempMem(ScreenSize);
                    if(ScreenBuf)
                    {
                        FileSys_SeekFile(filehdl, ScreenOffset, FST_SEEK_SET);
                        FileSys_ReadFile(filehdl, (UINT8 *)ScreenBuf, &ScreenSize, 0,0);
                        FileSys_CloseFile(filehdl);
                        remain = ScreenSize;
                    }
                    else
                    {
                        FileSys_CloseFile(filehdl);
                        result = WIFIAPP_RET_NOBUF;
                        DBG_ERR("no TempBuf\r\n");
                    }
                }
                else
                {
                    result = WIFIAPP_RET_EXIF_ERR;
                    FileSys_CloseFile(filehdl);
                    DBG_ERR("exif error\r\n");
                }
            }
            else
            {
                result = WIFIAPP_RET_NOFILE;
                DBG_ERR("no %s\r\n",tempPath);
            }
        }

    }
    if(ScreenBuf)
    {
        if(remain>bufflen)
        {
            hwmem_open();
            hwmem_memcpy((UINT32)buf, ScreenBuf+bufflen*segmentCount, bufflen);
            hwmem_close();
            *bufSize = bufflen;
            remain-=bufflen;
            return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
        }
        else
        {
            hwmem_open();
            hwmem_memcpy((UINT32)buf, ScreenBuf+bufflen*segmentCount, remain);
            hwmem_close();
            *bufSize = remain;
            remain=0;
            ScreenBuf =0;
            //debug_msg("last trans ok\r\n");
        }
    }

    if(result!=0)
        XML_DefaultFormat(WIFIAPP_CMD_SCREEN,result,bufAddr,bufSize,mimeType);

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetMovieFileInfo(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32            TempBuf;
    UINT32            result =0;
    GXVIDEO_INFO      VideoInfo;
    char              szFileInfo[128];
    char              tempPath[128];
    char*             pch;

    if(!SxCmd_GetTempMem(POOL_SIZE_FILEDB))
    {
        XML_DefaultFormat(WIFIAPP_CMD_MOVIE_FILE_INFO,WIFIAPP_RET_NOBUF,bufAddr,bufSize,mimeType);
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }



    // set the data mimetype
    strcpy(mimeType,"image/jpeg");
    XML_ecos2NvtPath(path, tempPath);

    pch=strchr(tempPath,'.');

    if((!path)||(!pch))
    {
        XML_DefaultFormat(WIFIAPP_CMD_MOVIE_FILE_INFO,WIFIAPP_RET_PAR_ERR,bufAddr,bufSize,mimeType);
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }

    if((strncmp(pch+1,"mov",3)==0)||(strncmp(pch+1,"MOV",3)==0)||(strncmp(pch+1,"mp4",3)==0)||(strncmp(pch+1,"MP4",3)==0))
    {
        FST_FILE_STATUS FileStat;
        MEM_RANGE WorkBuf;
        UINT32 uiBufferNeeded=0;


        gMovFilehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);
        if(gMovFilehdl)
        {
            FileSys_StatFile(gMovFilehdl,&FileStat);
            GxVidFile_Query1stFrameWkBufSize(&uiBufferNeeded, FileStat.uiFileSize);
            TempBuf = SxCmd_GetTempMem(uiBufferNeeded);
            //parse video info for single mode only
            WorkBuf.Addr = TempBuf;
            WorkBuf.Size = uiBufferNeeded;
            result = GxVidFile_ParseVideoInfo(XML_VdoReadCB, &WorkBuf, (UINT32)FileStat.uiFileSize, &VideoInfo);
            if((result==GXVIDEO_PRSERR_OK)&&(VideoInfo.uiThumbSize))
            {
                FileSys_CloseFile(gMovFilehdl);
                *bufSize = VideoInfo.uiThumbSize;
            }
            else
            {
                FileSys_CloseFile(gMovFilehdl);
                result = WIFIAPP_RET_EXIF_ERR;
                DBG_ERR("exif error\r\n");
            }
        }
        else
        {
            result = WIFIAPP_RET_NOFILE;
            DBG_ERR("no %s\r\n",tempPath);
        }

    }
    else
    {
        result = WIFIAPP_RET_FILE_ERROR;
        DBG_ERR("no %s\r\n",tempPath);
    }


    if(result!=0)
        XML_DefaultFormat(WIFIAPP_CMD_MOVIE_FILE_INFO,result,bufAddr,bufSize,mimeType);
    else
    {
        sprintf(szFileInfo, "Width:%d, Height:%d, Length:%d sec", VideoInfo.uiVidWidth, VideoInfo.uiVidHeight, VideoInfo.uiToltalSecs);
        XML_StringResult(WIFIAPP_CMD_MOVIE_FILE_INFO, szFileInfo, bufAddr,bufSize,mimeType);
    }

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetRawEncJpg(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    char*             buf = (char*)bufAddr;
    UINT32            bufflen = *bufSize-512;
    static UINT32     remain =0;
    static UINT32     uiJpgAddr = 0;
    UINT32            uiJpgSize = 0;
    UINT32            result=0;


    // set the data mimetype
    strcpy(mimeType,"image/jpeg");

    if(segmentCount ==0)
    {
        MovRec_RawEncGetJpgData(&uiJpgAddr, &uiJpgSize, 0);
        if(uiJpgAddr)
        {
            remain = uiJpgSize;
        }
        else
        {
            result = WIFIAPP_RET_NOFILE;
            DBG_ERR("no JPG file\r\n");
        }
    }

    if(uiJpgAddr)
    {
        if(remain>bufflen)
        {
            hwmem_open();
            hwmem_memcpy((UINT32)buf, uiJpgAddr+bufflen*segmentCount, bufflen);
            hwmem_close();
            *bufSize = bufflen;
            remain-=bufflen;
            return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
        }
        else // last data.
        {
            hwmem_open();
            hwmem_memcpy((UINT32)buf, uiJpgAddr+bufflen*segmentCount, remain);
            hwmem_close();
            *bufSize = remain;
            remain=0;
            uiJpgAddr =0;
            //debug_msg("last trans ok\r\n");
        }
    }

    if(result!=0)
        XML_DefaultFormat(WIFIAPP_CMD_MOVIE_GET_RAWENC_JPG, result, bufAddr, bufSize, mimeType);

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetVersion(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    XML_StringResult(WIFIAPP_CMD_VERSION,Prj_GetVersionString(),bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetLiveViewFmt(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32 len;
    char* buf = (char*)bufAddr;
    UINT32 xmlBufSize = *bufSize;


    len= XML_snprintf(&buf,&xmlBufSize,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");
    // Check movie live view streaming format and assign corresponding streaming link.
    #if (MOVIE_LIVEVIEW == RTSP_LIVEVIEW)
    len= XML_snprintf(&buf,&xmlBufSize, "<MovieLiveViewLink>rtsp://%s/xxx.mov</MovieLiveViewLink>\n", UINet_GetIP());
    #else
    len= XML_snprintf(&buf,&xmlBufSize, "<MovieLiveViewLink>http://%s:8192</MovieLiveViewLink>\n", UINet_GetIP());
    #endif
    len= XML_snprintf(&buf,&xmlBufSize, "<PhotoLiveViewLink>http://%s:8192</PhotoLiveViewLink>\n", UINet_GetIP());

    len= XML_snprintf(&buf,&xmlBufSize,"</LIST>\n");
    *bufSize = (cyg_uint32)(buf)-bufAddr;

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_FileList(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{

    PFILEDB_INIT_OBJ        pFDBInitObj = &gWifiFDBInitObj;
    static FILEDB_HANDLE    FileDBHandle =0;
    static UINT32           gIndex;
    UINT32                  len, fileCount, i, bufflen = *bufSize;
    char*                   buf = (char*)bufAddr;

    Perf_Mark();
    pFDBInitObj->u32MemAddr = SxCmd_GetTempMem(POOL_SIZE_FILEDB);

    if(!pFDBInitObj->u32MemAddr)
    {
        XML_DefaultFormat(WIFIAPP_CMD_FILELIST,WIFIAPP_RET_NOBUF,bufAddr,bufSize,mimeType);
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }

    //playback mode use default FileDB 0
    if ((segmentCount == 0)&&(System_GetState(SYS_STATE_CURRMODE)!=PRIMARY_MODE_PLAYBACK))
    {
        pFDBInitObj->u32MemSize = POOL_SIZE_FILEDB;
        FileDBHandle = FileDB_Create(pFDBInitObj);
        DBG_IND("createTime = %04d ms \r\n",Perf_GetDuration()/1000);
        Perf_Mark();
        FileDB_SortBy(FileDBHandle,FILEDB_SORT_BY_MODDATE,FALSE);
        DBG_IND("sortTime = %04d ms \r\n",Perf_GetDuration()/1000);
    }
    else if(System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK)
    {
        FileDBHandle = 0;
        FileDB_Refresh(0);
    }
    //FileDB_DumpInfo(FileDBHandle);

    if (segmentCount == 0)
    {
        len = sprintf(buf,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");
        buf+=len;
    }

    fileCount = FileDB_GetTotalFileNum(FileDBHandle);
    debug_msg("fileCount  %d\r\n",fileCount);

    if (segmentCount == 0)
    {
        // reset some global variables
        gIndex = 0;
    }
    DBG_IND("gIndex = %d %d \r\n",gIndex,fileCount);
    for (i=gIndex;i<fileCount;i++)
    {
        PFILEDB_FILE_ATTR        pFileAttr;
        // check buffer length , reserved 512 bytes
        // should not write data over buffer length
        if ((cyg_uint32)buf-bufAddr > bufflen - 512)
        {
            DBG_IND("totallen=%d >  bufflen(%d)-512\r\n",(cyg_uint32)buf-bufAddr,bufflen);
            *bufSize = (cyg_uint32)(buf)-bufAddr;
            gIndex = i;
            return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
        }
        // get dcf file
        pFileAttr = FileDB_SearhFile(FileDBHandle, i);

        debug_msg("file %d %s %d\r\n",i,pFileAttr->filePath,pFileAttr->fileSize);

        {
            len = sprintf(buf,"<ALLFile><File>\n<NAME>%s</NAME>\n<FPATH>%s</FPATH>\n",pFileAttr->filename,pFileAttr->filePath);
            buf+=len;
            len = sprintf(buf,"<SIZE>%d</SIZE>\n<TIMECODE>%ld</TIMECODE>\n<TIME>%04d/%02d/%02d %02d:%02d:%02d</TIME>\n<ATTR>%d</ATTR></File>\n</ALLFile>\n",pFileAttr->fileSize,(pFileAttr->lastWriteDate <<16) + pFileAttr->lastWriteTime,
                  FAT_GET_YEAR_FROM_DATE(pFileAttr->lastWriteDate),FAT_GET_MONTH_FROM_DATE(pFileAttr->lastWriteDate),FAT_GET_DAY_FROM_DATE(pFileAttr->lastWriteDate),
                  FAT_GET_HOUR_FROM_TIME(pFileAttr->lastWriteTime),FAT_GET_MIN_FROM_TIME(pFileAttr->lastWriteTime),FAT_GET_SEC_FROM_TIME(pFileAttr->lastWriteTime),
                  pFileAttr->attrib );
            buf+=len;
        }
    }
    len = sprintf(buf,"</LIST>\n");
    buf+=len;
    *bufSize = (cyg_uint32)(buf)-bufAddr;

    if(System_GetState(SYS_STATE_CURRMODE)!=PRIMARY_MODE_PLAYBACK)
    {
        FileDB_Release(FileDBHandle);
    }

    return CYG_HFS_CB_GETDATA_RETURN_OK;

}


void XML_DefaultFormat(UINT32 cmd,UINT32 ret,cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType)
{
    UINT32 len=0;
    UINT32 bufflen = *bufSize-512;
    char*  buf = (char*)bufAddr;

    strcpy(mimeType,"text/xml");
    len = sprintf(buf,DEF_XML_HEAD);
    buf+=len;
    len = sprintf(buf,DEF_XML_RET,cmd,ret);
    buf+=len;
    *bufSize = (cyg_uint32)(buf)-bufAddr;

    if(*bufSize > bufflen)
    {
        DBG_ERR("no buffer\r\n");
        *bufSize= bufflen;
    }
}

void XML_StringResult(UINT32 cmd,char *str,cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType)
{
    UINT32 len=0;
    UINT32 bufflen = *bufSize-512;
    char*  buf = (char*)bufAddr;

    strcpy(mimeType,"text/xml");
    len = sprintf(buf,DEF_XML_HEAD);
    buf+=len;
    len = sprintf(buf,DEF_XML_STR,cmd,0,str);//status OK
    buf+=len;
    *bufSize = (cyg_uint32)(buf)-bufAddr;

    if(*bufSize > bufflen)
    {
        DBG_ERR("no buffer\r\n");
        *bufSize= bufflen;
    }
}

void XML_ValueResult(UINT32 cmd,UINT64 value,cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType)
{
    UINT32 len=0;
    UINT32 bufflen = *bufSize-512;
    char*  buf = (char*)bufAddr;

    strcpy(mimeType,"text/xml");
    len = sprintf(buf,DEF_XML_HEAD);
    buf+=len;
    len = sprintf(buf,DEF_XML_VALUE,cmd,0,value);//status OK
    buf+=len;
    *bufSize = (cyg_uint32)(buf)-bufAddr;

    if(*bufSize > bufflen)
    {
        DBG_ERR("no buffer\r\n");
        *bufSize= bufflen;
    }
}

int XML_QueryCmd_CurSts(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    static UINT32     uiCmdIndex;
    UINT32            len/*, i=0*/;
    char*             buf = (char*)bufAddr;
    FST_FILE          filehdl;
    char              pFilePath[64];
    UINT32            fileLen =*bufSize-512;
    UINT32            bufflen =*bufSize-512;
    WIFI_CMD_ENTRY *  appCmd=0;


    // set the data mimetype
    strcpy(mimeType,"text/xml");
    sprintf(pFilePath,"%s",QUERY_CMD_CUR_STS_XML_PATH);  //html of all command status list
    filehdl = FileSys_OpenFile(pFilePath,FST_OPEN_READ);
    if(filehdl)
    {
        FileSys_ReadFile(filehdl, (UINT8 *)buf, &fileLen, 0,0);
        FileSys_CloseFile(filehdl);
        *bufSize = fileLen;
        *(buf+fileLen) ='\0';

        uiCmdIndex = 0;
    }
    else
    {
        if (segmentCount == 0)
        {
            len = sprintf(buf,DEF_XML_HEAD);
            buf+=len;
            uiCmdIndex = 0;
        }

        len = sprintf(buf,"<Function>\n");
        buf+=len;
        appCmd= WifiCmd_GetExecTable();
        while(appCmd[uiCmdIndex].cmd != 0)
        {
            if(appCmd[uiCmdIndex].UIflag != FL_NULL)
            {
                len = sprintf(buf, DEF_XML_CMD_CUR_STS, appCmd[uiCmdIndex].cmd, UI_GetData(appCmd[uiCmdIndex].UIflag));
                buf+=len;
                //debug_msg(DEF_XML_CMD_CUR_STS, appCmd[uiCmdIndex].cmd, UI_GetData(appCmd[uiCmdIndex].UIflag));
            }
            uiCmdIndex++;

            // check buffer length , reserved 512 bytes
            // should not write data over buffer length
            if ((cyg_uint32)buf-bufAddr > bufflen - 512)
            {
                DBG_IND("totallen=%d >  bufflen(%d)-512\r\n",(cyg_uint32)buf-bufAddr,bufflen);
                *bufSize = (cyg_uint32)(buf)-bufAddr;
                return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
            }
        }

        len = sprintf(buf,"</Function>\n");
        buf+=len;
        *bufSize = (cyg_uint32)(buf)-bufAddr;
    }

    if(*bufSize > bufflen)
    {
        DBG_ERR("no buffer\r\n");
        *bufSize= bufflen;
    }


    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetHeartBeat(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    XML_DefaultFormat(WIFIAPP_CMD_HEARTBEAT,0,bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetFreePictureNum(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    XML_ValueResult(WIFIAPP_CMD_FREE_PIC_NUM,PhotoExe_GetFreePicNum(),bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}
int XML_GetMaxRecordTime(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32 sec = UIFlowWiFiMovie_GetMaxRecTime();
    XML_ValueResult(WIFIAPP_CMD_MAX_RECORD_TIME,sec,bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetDiskFreeSpace(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    DBG_IND("FileSys_GetDiskInfo(FST_INFO_DISK_SIZE) %lld\r\n",FileSys_GetDiskInfo(FST_INFO_DISK_SIZE));
    XML_ValueResult(WIFIAPP_CMD_DISK_FREE_SPACE, FileSys_GetDiskInfo(FST_INFO_FREE_SPACE), bufAddr, bufSize, mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_DeleteOnePicture(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    char fullPath[128] ={0};
    int ret;
    char*             pch=0;
    UINT8      attrib;

    pch=strrchr(argument,'&');
    if(strncmp(pch,PARS_STR,strlen(PARS_STR))==0)
    {
        sscanf(pch+strlen(PARS_STR),"%s",&fullPath);
    }
    DBG_IND("fullPath path=%s\r\n",fullPath);

    ret = FileSys_GetAttrib(fullPath,&attrib);
    if ( (ret == E_OK) && (M_IsReadOnly(attrib) == TRUE) )
        {
            DBG_IND("File Locked\r\n");
            XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE,WIFIAPP_RET_FILE_LOCKED,bufAddr,bufSize,mimeType);
    }
    else if (ret == FST_STA_FILE_NOT_EXIST)
    {
        DBG_IND("File not existed\r\n");
        XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE,WIFIAPP_RET_NOFILE,bufAddr,bufSize,mimeType);
        }
        else
        {
        ret = FileSys_DeleteFile(fullPath);
            DBG_IND("ret = %d\r\n", ret);
            if (ret != FST_STA_OK)
            {
            XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE,WIFIAPP_RET_FILE_ERROR,bufAddr,bufSize,mimeType);
            }
            else
            {
            #if 0
            Index = FileDB_GetIndexByPath(FileDBHdl,fullPath);
            DBG_IND("Index = %04d\r\n",Index);
            if (Index != FILEDB_SEARCH_ERR)
            {
                FileDB_DeleteFile(FileDBHdl,Index);
            }
            #endif

            XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE,WIFIAPP_RET_OK,bufAddr,bufSize,mimeType);
        }
    }

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_DeleteAllPicture(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    char  tempPath[128];
    BOOL  isCurrFileReadOnly = FALSE;
    UINT32 Index;
    PFILEDB_FILE_ATTR FileAttr = NULL;
    INT32             ret;
    INT32             FileNum,i;
    PFILEDB_INIT_OBJ   pFDBInitObj = &gWifiFDBInitObj;
    FILEDB_HANDLE      FileDBHandle =0;

    Perf_Mark();

    pFDBInitObj->u32MemAddr = SxCmd_GetTempMem(POOL_SIZE_FILEDB);
    if(!pFDBInitObj->u32MemAddr)
    {
        XML_DefaultFormat(WIFIAPP_CMD_DELETE_ALL,WIFIAPP_RET_NOBUF,bufAddr,bufSize,mimeType);
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }
    pFDBInitObj->u32MemSize = POOL_SIZE_FILEDB;
    FileDBHandle = FileDB_Create(pFDBInitObj);
    DBG_IND("createTime = %04d ms \r\n",Perf_GetDuration()/1000);
    Perf_Mark();
    FileDB_SortBy(FileDBHandle,FILEDB_SORT_BY_MODDATE,FALSE);
    DBG_IND("sortTime = %04d ms \r\n",Perf_GetDuration()/1000);

    FileAttr = FileDB_CurrFile(FileDBHandle);
    if( FileAttr && M_IsReadOnly(FileAttr->attrib))
    {
        isCurrFileReadOnly = TRUE;
        strncpy(tempPath,FileAttr->filePath,sizeof(tempPath));
    }

    FileNum = FileDB_GetTotalFileNum(FileDBHandle);
    for (i=FileNum-1;i>=0;i--)
    {
        FileAttr = FileDB_SearhFile(FileDBHandle,i);
        if (FileAttr)
        {
            if (M_IsReadOnly(FileAttr->attrib))
            {
                //DBG_IND("File Locked\r\n");
                DBG_IND("File %s is locked\r\n", FileAttr->filePath);
                continue;
            }
            ret = FileSys_DeleteFile(FileAttr->filePath);
            //DBG_IND("i = %04d path=%s\r\n",i,FileAttr->filePath);
            if (ret != FST_STA_OK)
            {
                DBG_IND("Delete %s failed\r\n", FileAttr->filePath);
            }
            else
            {
                FileDB_DeleteFile(FileDBHandle,i);
                DBG_IND("Delete %s OK\r\n", FileAttr->filePath);
            }
        }
        else
        {
            DBG_IND("%s not existed\r\n", FileAttr->filePath);
        }
    }

    if (isCurrFileReadOnly)
    {
       Index = FileDB_GetIndexByPath(FileDBHandle,tempPath);
       FileDB_SearhFile(FileDBHandle,Index);
    }

    FileDB_Release(FileDBHandle);

    XML_DefaultFormat(WIFIAPP_CMD_DELETE_ALL,WIFIAPP_RET_OK,bufAddr,bufSize,mimeType);

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
extern UINT32 WifiCmd_WaitFinish(FLGPTN waiptn);

int XML_APP_STARTUP(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    #if 0 // Return fail if it's not allowed to run Wi-Fi APP command.
    XML_DefaultFormat(WIFIAPP_CMD_APP_STARTUP, WIFIAPP_RET_FAIL,bufAddr,bufSize,mimeType);
    #else
    XML_DefaultFormat(WIFIAPP_CMD_APP_STARTUP, WIFIAPP_RET_OK,bufAddr,bufSize,mimeType);
    //Clear wifi flag before waiting for it.
    WifiCmd_ClrFlg(WIFIFLAG_MODE_DONE);

    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 2, PRIMARY_MODE_MOVIE,SYS_SUBMODE_WIFI);

    WifiCmd_WaitFinish(WIFIFLAG_MODE_DONE);

    #endif

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}
//#NT#2016/03/23#Isiah Chang -end

//#NT#2016/06/01#Isiah Chang -begin
//#NT#add a Wi-Fi command to get current mode status.
int XML_GetCntModeStatus(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    if (System_GetState(SYS_STATE_CURRMODE)== PRIMARY_MODE_MOVIE)
        XML_ValueResult(WIFIAPP_CMD_GET_MODE_STAUTS,UI_GetData(FL_WIFI_MOVIE_FMT),bufAddr,bufSize,mimeType);
    else if(System_GetState(SYS_STATE_CURRMODE)== PRIMARY_MODE_PLAYBACK)
        XML_ValueResult(WIFIAPP_CMD_GET_MODE_STAUTS,3,bufAddr,bufSize,mimeType);
    else if(System_GetState(SYS_STATE_CURRMODE)== PRIMARY_MODE_PHOTO)
        XML_ValueResult(WIFIAPP_CMD_GET_MODE_STAUTS,4,bufAddr,bufSize,mimeType);
    else
        XML_ValueResult(WIFIAPP_CMD_GET_MODE_STAUTS,UIFlowWndWiFiMovie_GetStatus(),bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}
//#NT#2016/06/01#Isiah Chang -end

int XML_GetPictureEnd(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32            len;
    char*             buf = (char*)bufAddr;
    UINT32            result =0;
    UINT32            FreePicNum=0;
    char*             pFilePath=0;
    char*             pFileName=0;
    // set the data mimetype
    strcpy(mimeType,"text/xml");
    DBG_IND("path = %s, argument -> %s, mimeType= %s, bufsize= %d, segmentCount= %d\r\n",path,argument, mimeType, *bufSize, segmentCount);


    if(UIStorageCheck(STORAGE_CHECK_FOLDER_FULL, NULL))
    {
        result = WIFIAPP_RET_FOLDER_FULL;
        DBG_ERR("folder full\r\n");
    }
    else if(UIStorageCheck(STORAGE_CHECK_FULL, &FreePicNum))
    {
        result = WIFIAPP_RET_STORAGE_FULL;
        DBG_ERR("storage full\r\n");
    }
    else if(UI_GetData(FL_FSStatus)== FS_DISK_ERROR)
    {
        result = WIFIAPP_RET_FILE_ERROR;
        DBG_ERR("write file fail\r\n");
    }
    else
    {
        pFilePath = DscPhoto_GetLastWriteFilePath();
        //DBG_ERR("path %s %d",pFilePath,strlen(pFilePath));
        if(strlen(pFilePath))
        {
            pFileName = strrchr(pFilePath,'\\')+1;
        }
        else
        {
            result = WIFIAPP_RET_FILE_ERROR;
            DBG_ERR("write file fail\r\n");
        }
    }
    if(result == 0)
    {
        len = sprintf(buf,DEF_XML_HEAD);
        buf+=len;
        len = sprintf(buf,"<Function>\n");
        buf+=len;
        len = sprintf(buf,DEF_XML_CMD_CUR_STS,WIFIAPP_CMD_CAPTURE,result);
        buf+=len;
        len = sprintf(buf,"<File>\n<NAME>%s</NAME>\n<FPATH>%s</FPATH></File>\n",pFileName,pFilePath);
        buf+=len;
        len = sprintf(buf,"<FREEPICNUM>%d</FREEPICNUM>\n",FreePicNum);
        buf+=len;
        len = sprintf(buf,"</Function>\n");
        buf+=len;
        *bufSize = (cyg_uint32)(buf)-bufAddr;
    }
    else
    {
        XML_DefaultFormat(WIFIAPP_CMD_CAPTURE,result,bufAddr,bufSize,mimeType);
    }
    DBG_ERR("%s",bufAddr);
    return CYG_HFS_CB_GETDATA_RETURN_OK;

}

int XML_GetBattery(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    XML_ValueResult(WIFIAPP_CMD_GET_BATTERY,GetBatteryLevel(),bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}
int XML_HWCapability(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32 value =0;
    #if(TV_FUNC==ENABLE)
    value |= HW_TV_ENABLE;
    #endif
    #if (GSENSOR_FUNCTION == ENABLE)
    value |= HW_GSENDOR_ENABLE;
    #endif
//#NT#2016/06/01#Isiah Chang -begin
//#NT#add a Wi-Fi command to get current mode status.
    #if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
    value |= HW_WIFI_SOCKET_HANDSHAKE_ENABLE;
    #endif
//#NT#2016/06/01#Isiah Chang -end
    #if (USOCKET_CLIENT == ENABLE) // Enable client socket notification for Android APP to get DHCP client's IP.
    value |= HW_WIFI_CLIENT_SOCKET_NOTIFY_ENABLE;
    #endif

    //DBG_ERR("XML_HWCapability value %x\r\n",value);
    XML_ValueResult(WIFIAPP_CMD_GET_HW_CAP,value,bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}


int XML_GetMovieRecStatus(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    if(UIFlowWndWiFiMovie_GetStatus() == WIFI_MOV_ST_RECORD )
        XML_ValueResult(WIFIAPP_CMD_MOVIE_RECORDING_TIME,FlowMovie_GetRecCurrTime(),bufAddr,bufSize,mimeType);
    else
        XML_ValueResult(WIFIAPP_CMD_MOVIE_RECORDING_TIME,0,bufAddr,bufSize,mimeType);

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetCardStatus(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    // check card inserted
    #if 0
    XML_ValueResult(WIFIAPP_CMD_GET_CARD_STATUS, UI_GetData(FL_CardStatus), bufAddr,bufSize,mimeType);
    #else
    if(UI_GetData(FL_CardStatus) != CARD_REMOVED)
    {
        if(UIStorageCheck(STORAGE_CHECK_ERROR, NULL) == FALSE)
        {
            XML_ValueResult(WIFIAPP_CMD_GET_CARD_STATUS, UI_GetData(FL_CardStatus), bufAddr,bufSize,mimeType);
        }
        else // Filesystem error happened.
        {
            // Return WIFIAPP_CMD_GET_CARD_STATUS + UI_FSStatus.
            /*
            typedef enum
            {
                FS_DISK_ERROR = 0,
                FS_UNKNOWN_FORMAT,
                FS_UNFORMATTED,
                FS_NOT_INIT,
                FS_INIT_OK,
                FS_NUM_FULL
            } FS_STATUS; */
            XML_ValueResult(WIFIAPP_CMD_GET_CARD_STATUS, WIFIAPP_CMD_GET_CARD_STATUS + UI_GetData(FL_FSStatus), bufAddr,bufSize,mimeType);
        }
    }
    else
    {
        XML_ValueResult(WIFIAPP_CMD_GET_CARD_STATUS, UI_GetData(FL_CardStatus), bufAddr,bufSize,mimeType);
    }
    #endif
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetDownloadURL(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    XML_StringResult(WIFIAPP_CMD_GET_DOWNLOAD_URL, WIFI_APP_DOWNLOAD_URL, bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetUpdateFWPath(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    char url[128];
    char FWPath[64];

    XML_Nvt2ecosPath(FW_UPDATE_NAME,FWPath);
    sprintf(url,"http://%s%s",UINet_GetIP(),(char *)FWPath);

    XML_StringResult(WIFIAPP_CMD_GET_UPDATEFW_PATH, url, bufAddr,bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetSSID_passphrase(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32 len;
    char* buf = (char*)bufAddr;

    len = sprintf(buf,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");
    buf+=len;

    len = sprintf(buf, "<SSID>%s</SSID>\n", UINet_GetSSID());
    buf+=len;

    len = sprintf(buf, "<PASSPHRASE>%s</PASSPHRASE>\n", UINet_GetPASSPHRASE());
    buf+=len;

    len = sprintf(buf,"</LIST>\n");
    buf+=len;
    *bufSize = (cyg_uint32)(buf)-bufAddr;

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}
extern WIFIAPPINDEXMAP movieSizeMap[];
int XML_GetMovieSizeCapability(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32  len;
    char*   buf = (char*)bufAddr;
    TM_ITEM*    pItem=0;
    TM_OPTION*  pOption;
    UINT16      i=0;
    UINT32      xmlBufSize = *bufSize;

    TM_SetMenu(&gMovieMenu); //for count movie page item
    pItem = TM_GetItem(&gMovieMenu,IDM_MOVIE_SIZE);
    if(pItem)
    {
        XML_STRCPY(mimeType,"text/xml",CYG_HFS_MIMETYPE_MAXLEN);

        len= XML_snprintf(&buf,&xmlBufSize,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");
        for(i=0;i<pItem->Count;i++)
        {
            pOption = &pItem->pOptions[i];
            //if((pOption->Status & TM_OPTION_STATUS_MASK) == TM_OPTION_ENABLE)//650 always enable
            {
                len= XML_snprintf(&buf,&xmlBufSize, "<Item>\n");


                //len= XML_snprintf(&buf,&xmlBufSize, "<Name>%s</Name>\n", UIRes_GetUserString(pOption->TextId));
                len= XML_snprintf(&buf,&xmlBufSize, "<Name>%s</Name>\n",movieSizeMap[i].string );

                len= XML_snprintf(&buf,&xmlBufSize, "<Index>%d</Index>\n", i);

                len= XML_snprintf(&buf,&xmlBufSize, "<Size>%d*%d</Size>\n",GetMovieSizeWidth_2p(0,i),GetMovieSizeHeight_2p(0,i));

                len= XML_snprintf(&buf,&xmlBufSize, "<FrameRate>%d</FrameRate>\n",GetMovieFrameRate_2p(0,i));

                len= XML_snprintf(&buf,&xmlBufSize, "<Type>%d</Type>\n",MOVIE_REC_TYPE_CLONE);

                len= XML_snprintf(&buf,&xmlBufSize, "</Item>\n");
            }
        }

        len= XML_snprintf(&buf,&xmlBufSize,"</LIST>\n");
        *bufSize = (cyg_uint32)(buf)-bufAddr;
    }
    else
    {
        XML_ValueResult(WIFIAPP_CMD_QUERY_MOVIE_SIZE,WIFIAPP_RET_FAIL,bufAddr,bufSize,mimeType);
    }
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

#define QUERYMENU_CMD_STR "custom=1&cmd=3031&str="

int XML_GetMenuItem(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{
    UINT32 len=0;
    char*  buf = (char*)bufAddr;
    WIFI_CMD_ENTRY  *appCmd=0;
    UINT32          uiCmdIndex=0;
    WIFIAPPINDEXMAP  *mapTbl =0;
    WIFIAPPMAP       *wifiAppFlagMap;
    UINT32 i=0,j=0;
    UINT8 auth[2] = {0};
    UINT8 key[4]={0};
    UINT32 xmlBufSize = *bufSize;

    appCmd= WifiCmd_GetExecTable();
    wifiAppFlagMap = WifiCmd_GetMapTbl();

    if((!appCmd)||(!wifiAppFlagMap))
    {
        XML_DefaultFormat(WIFIAPP_CMD_QUERY_MENUITEM,WIFIAPP_RET_FAIL,bufAddr,bufSize,mimeType);
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }

    XML_STRCPY(mimeType,"text/xml",CYG_HFS_MIMETYPE_MAXLEN);
    len= XML_snprintf(&buf,&xmlBufSize,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");

    while(appCmd[uiCmdIndex].cmd != 0)
    {
        if(appCmd[uiCmdIndex].UIflag != FL_NULL)
        {
            i=0;
            while(wifiAppFlagMap[i].uiFlag != FL_NULL)
            {
                if(wifiAppFlagMap[i].uiFlag == appCmd[uiCmdIndex].UIflag)
                {
                    len= XML_snprintf(&buf,&xmlBufSize, "<Item>\n");
                    len= XML_snprintf(&buf,&xmlBufSize, "<Cmd>%d</Cmd>\n",appCmd[uiCmdIndex].cmd);
                    len= XML_snprintf(&buf,&xmlBufSize, "<Name>%s</Name>\n",wifiAppFlagMap[i].name);
                    len= XML_snprintf(&buf,&xmlBufSize, "<MenuList>\n");

                    mapTbl= wifiAppFlagMap[i].maptbl;
                    j=0;
                    while(mapTbl[j].uiIndex!=INDEX_END)
                    {
                        len= XML_snprintf(&buf,&xmlBufSize, "<Option>\n");
                        len= XML_snprintf(&buf,&xmlBufSize, "<Index>%d</Index>\n",mapTbl[j].uiIndex);
                        len= XML_snprintf(&buf,&xmlBufSize, "<Id>%s</Id>\n",mapTbl[j].string);
                        len= XML_snprintf(&buf,&xmlBufSize, "</Option>\n");
                        j++;
                    }
                    len= XML_snprintf(&buf,&xmlBufSize, "</MenuList>\n");
                    len= XML_snprintf(&buf,&xmlBufSize, "</Item>\n");
                }
                i++;
            }
        }
        uiCmdIndex++;
    }

    //DBG_DUMP("Data2 path = %s, argument -> %s, mimeType= %s, bufsize= %d, segmentCount= %d\r\n",path,argument, mimeType, *bufSize, segmentCount);

    if(strncmp(argument,QUERYMENU_CMD_STR,strlen(QUERYMENU_CMD_STR))==0)
    {
        UINT32 keyValue =0;
        #if 0
        sscanf_s(argument+strlen(QUERYMENU_CMD_STR),"%x",&keyValue,sizeof(keyValue));
        #else
        sscanf(argument+strlen(QUERYMENU_CMD_STR),"%x",&keyValue);
        #endif
        key[0]=keyValue>>24&0xFF;
        key[1]=keyValue>>16&0xFF;
        key[2]=keyValue>>8&0xFF;
        key[3]=keyValue&0xFF;
    }
    else
    {
        DBG_ERR("no key\r\n");
        XML_DefaultFormat(WIFIAPP_CMD_QUERY_CUR_STATUS,WIFIAPP_RET_PAR_ERR,bufAddr,bufSize,mimeType);
        return CYG_HFS_CB_GETDATA_RETURN_OK;
    }

    Nvt_AuthGen(key,(UINT8 *)bufAddr,(UINT32)((buf)-bufAddr),auth);

    len= XML_snprintf(&buf,&xmlBufSize,"<CHK>%02X%02X</CHK>\n",auth[0],auth[1]);

    len= XML_snprintf(&buf,&xmlBufSize,"</LIST>\n");
    *bufSize = (cyg_uint32)(buf)-bufAddr;

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int WifiCmd_getQueryData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount)
{

    UINT32 len=0;
    UINT32 bufflen = *bufSize-512;
    char*  buf = (char*)bufAddr;

    DBG_DUMP("Data2 path = %s, argument -> %s, mimeType= %s, bufsize= %d, segmentCount= %d\r\n",path,argument, mimeType, *bufSize, segmentCount);

    strcpy(mimeType,"text/xml");

    len = sprintf(buf,QUERY_FMT,WIFI_APP_MANUFACTURER,1,WIFI_APP_MODLE);
    DBG_DUMP(buf);
    buf+=len;

    *bufSize = (cyg_uint32)(buf)-bufAddr;

    if(*bufSize > bufflen)
    {
        DBG_ERR("no buffer\r\n");
        *bufSize= bufflen;
    }
    return CYG_HFS_CB_GETDATA_RETURN_OK;

}

#define MAX_UPLOAD_FILE_SIZE                 0x00380000
#define FW_UPLOAD_USAGE_SAVE_AS_FILE         0x00
#define FW_UPLOAD_USAGE_UPDATE_FW_IN_DRAM    0x01
#define FW_UPLOAD_USAGE_SAVE_IN_DRAM         0x02

static FWSRV_BUF_DESC Buf={0};
static FWSRV_CMD Cmd={0};
int XML_UploadFile(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32 bufSize, cyg_uint32 segmentCount, HFS_PUT_STATUS putStatus)
{
    static      FST_FILE fhdl = 0;
    static      UINT32   uiTempBuf = 0, uiCopiedBytes = 0;
    static      UINT32   uiUsage = FW_UPLOAD_USAGE_SAVE_AS_FILE;
    UINT32      fileSize = 0;
    UINT32      m_SectSize = 0;
    FWSRV_ER    fws_er;
    char        tempPath[128];
    char        tmpString[32];

    DBG_IND("path =%s, argument = %s, bufAddr = 0x%x, bufSize =0x%x , segmentCount  =%d , putStatus = %d\r\n",path,argument ,bufAddr, bufSize, segmentCount, putStatus);

    if (putStatus == HFS_PUT_STATUS_ERR)
    {
        DBG_ERR("receive data error\r\n");
        return CYG_HFS_UPLOAD_FAIL_RECEIVE_ERROR;
    }
    else
    {
        if(segmentCount==0)
        {
            // Check if upload to a file or DRAM buffer.
            sprintf(tmpString,"custom=1&cmd=%d&par=", WIFIAPP_CMD_UPLOAD);
            if(strncmp(argument,tmpString,strlen(tmpString))==0)
            {
                sscanf(argument+strlen(tmpString), "%d", &uiUsage);
            }
            else
                uiUsage = FW_UPLOAD_USAGE_SAVE_AS_FILE;

            if(uiUsage == FW_UPLOAD_USAGE_SAVE_AS_FILE)
            {
                XML_ecos2NvtPath(path, tempPath);
                fhdl = FileSys_OpenFile(tempPath,FST_CREATE_ALWAYS|FST_OPEN_WRITE);
                FileSys_SeekFile(fhdl, 0 , FST_SEEK_SET);
            }
            else
            {
                uiCopiedBytes = 0;
                uiTempBuf = SxCmd_GetTempMem(MAX_UPLOAD_FILE_SIZE);
            }
        }

        if(uiUsage == FW_UPLOAD_USAGE_SAVE_AS_FILE)
        {
            fileSize = bufSize;
            FileSys_SeekFile(fhdl, 0 , FST_SEEK_END);
            FileSys_WriteFile(fhdl,(UINT8*)bufAddr,&fileSize, 0, NULL);
        }
        else // Copy file to a buffer.
        {
            if((uiCopiedBytes + bufSize) <= MAX_UPLOAD_FILE_SIZE)
            {
                hwmem_open();
                hwmem_memcpy((UINT32)(uiTempBuf + uiCopiedBytes), bufAddr, bufSize);
                hwmem_close();

                uiCopiedBytes += bufSize;
                //debug_msg("^BCopying...%d\r\n", uiCopiedBytes);
            }
            else
            {
                DBG_ERR("Out of bound of buffer!\r\n");
                return CYG_HFS_UPLOAD_FAIL_RECEIVE_ERROR;
            }
        }

        if(putStatus == HFS_PUT_STATUS_FINISH)
        {
            if(uiUsage == FW_UPLOAD_USAGE_SAVE_AS_FILE)
            {
                FileSys_CloseFile(fhdl);
                fhdl = 0;
            }
            else if(uiUsage == FW_UPLOAD_USAGE_UPDATE_FW_IN_DRAM)  // Update FW into Flash. Might send POWER_OFF command then.
            {
                debug_msg("^GCopied bytes:%d\r\n", uiCopiedBytes);

                #if 1
                debug_msg("^BUpdate FW - begin\r\n");

                memset(&Cmd,0,sizeof(Cmd));
                Cmd.Idx = FWSRV_CMD_IDX_GET_BLK_SIZE;
                Cmd.Out.pData = &m_SectSize;
                Cmd.Out.uiNumByte = sizeof(m_SectSize);
                Cmd.Prop.bExitCmdFinish = TRUE;
                fws_er = FwSrv_Cmd(&Cmd);
                if(fws_er != FWSRV_ER_OK)
                {
                    debug_msg("FW bin write failed\r\n");
                    return UPDNAND_STS_FW_WRITE_CHK_ERR;
                }

                memset(&Cmd,0,sizeof(Cmd));
                Buf.uiAddr = uiTempBuf;
                Buf.uiSize = (uiCopiedBytes+(m_SectSize-1))&(~(m_SectSize-1));
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

                debug_msg("^BUpdate FW - end\r\n");
                #else // Save as file to do verification.
                {
                    fhdl = FileSys_OpenFile("A:\\test.bin",FST_CREATE_ALWAYS|FST_OPEN_WRITE);
                    FileSys_WriteFile(fhdl,(UINT8*)uiTempBuf, &uiCopiedBytes, 0, NULL);
                    FileSys_CloseFile(fhdl);
                    fhdl = 0;
                }
                #endif
            }
            else
            {
                //debug_msg("File saved in DRAM.\r\n");
            }
        }
    }

    return CYG_HFS_UPLOAD_OK;
}

#endif
