#include <stdio.h>
#include <string.h>
#include "kernel.h"
#include "SysCfg.h"
#include "Utility.h"
#include "Jpg2Movie.h"
#include "JpgDec.h"
#include "JpgHeader.h"
#include "GxImage.h"
#include "FilesysTsk.h"
#include "MediaRecAPI.h"
#include "MediaWriteLib.h"
#include "FileDB.h"
#include "dma.h"

#define __MODULE__          JPG2MOV
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

typedef struct
{
    UINT32  uiJpgAddr;      ///< [in]  JPEG data address
    UINT32  uiJpgSize;      ///< [in]  JPEG data size
    UINT32  uiJpgWidth;     ///< [out] JPEG width
    UINT32  uiJpgHeight;    ///< [out] JPEG height
    UINT32  uiJpgFmt;       ///< [out] JPEG format
    UINT32  uiDecodeBuf;    ///< [in]  JPEG decoding buffer
    UINT32  uiScaleBuf;     ///< [in]  image scaling buffer
    UINT32  uiImgWidth;     ///< [in]  scaled image width
    UINT32  uiImgHeight;    ///< [in]  scaled image height
    UINT32  uiYAddr;        ///< [out] output Y address
    UINT32  uiUAddr;        ///< [out] output U address
    UINT32  uiVAddr;        ///< [out] output V address
    UINT32  uiYLineOffset;  ///< [out] Y line-offset
    UINT32  uiUVLineOffset; ///< [out] UV line-offset
} JPG2YUV_INFO;


extern UINT32 FLG_ID_RECMEDIA;

//static char sDirPath[40];
static char sFileName[40];

//-------------------------------------------------------------------------------------------------
//
// Decode JPEG to YUV, then transfer decoded image to 420 UV-packed and do scaling.
//
//-------------------------------------------------------------------------------------------------
ER JPG2YUV_DecodeAndScale(JPG2YUV_INFO *pJpg2Yuv)
{
    JPG_DEC_INFO        JpgDecInfo = {0};
    JPGHEAD_DEC_CFG     JpgDecCfg = {0};
    GX_IMAGE_PIXEL_FMT  PxlFmt, PxlFmtOut;
    ER                  err;

    JpgDecInfo.pDstAddr         = (UINT8 *)pJpg2Yuv->uiYAddr;
    JpgDecInfo.jdcfg_p          = &JpgDecCfg;
    JpgDecInfo.DecodeType       = DEC_PRIMARY;
    JpgDecInfo.bEnableTimeOut   = FALSE;
    JpgDecInfo.bEnableCrop      = FALSE;
    JpgDecInfo.pSrcAddr         = (UINT8 *)pJpg2Yuv->uiJpgAddr;
    JpgDecInfo.JPGFileSize      = pJpg2Yuv->uiJpgSize;
    err = Jpg_DecOneJPG(&JpgDecInfo);

    if (err == E_OK)
    {
        pJpg2Yuv->uiYLineOffset     = JpgDecInfo.jdcfg_p->lineoffsetY;
        pJpg2Yuv->uiUVLineOffset    = JpgDecInfo.jdcfg_p->lineoffsetUV;
        pJpg2Yuv->uiYAddr           = JpgDecInfo.uiOutAddrY;
        pJpg2Yuv->uiUAddr           = JpgDecInfo.uiOutAddrCb;
        pJpg2Yuv->uiVAddr           = pJpg2Yuv->uiUAddr + 1;
        pJpg2Yuv->uiJpgWidth        = JpgDecInfo.jdcfg_p->imagewidth;
        pJpg2Yuv->uiJpgHeight       = JpgDecInfo.jdcfg_p->imageheight;
        pJpg2Yuv->uiJpgFmt          = JpgDecInfo.jdcfg_p->fileformat;

        DBG_MSG("JPEG: w %d, h %d, fmt 0x%x\r\n", pJpg2Yuv->uiJpgWidth, pJpg2Yuv->uiJpgHeight, pJpg2Yuv->uiJpgFmt);
        DBG_MSG("YUV: Y 0x%08x, Cb 0x%08x, Cr 0x%08x, Y loff %d, UV loff %d\r\n",
            pJpg2Yuv->uiYAddr, pJpg2Yuv->uiUAddr, pJpg2Yuv->uiVAddr, pJpg2Yuv->uiYLineOffset, pJpg2Yuv->uiUVLineOffset);

        if (JpgDecInfo.jdcfg_p->fileformat == JPG_FMT_YUV211)
            PxlFmt = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
        else
            PxlFmt = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;

        PxlFmtOut = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
#if 0
        // ToDo: transfer YUV format to 420 UV-packed and do scaling if necessary
        {
            IMG_BUF DecodedBuf = {0}, DstImgBuf = {0};
            UINT32  LineOff[3], PxlAddr[3];
            UINT32  ImgSize, Width, Height;

            LineOff[0] = JpgDecInfo.jdcfg_p->lineoffsetY;
            LineOff[1] = JpgDecInfo.jdcfg_p->lineoffsetUV;

            PxlAddr[0] = JpgDecInfo.uiOutAddrY;
            PxlAddr[1] = JpgDecInfo.uiOutAddrCb;
            Width = JpgDecInfo.jdcfg_p->imagewidth;
            Height = JpgDecInfo.jdcfg_p->imageheight;
            ImgSize = Width*Height*2;

            GxImg_InitBufEx(&DecodedBuf,JpgDecInfo.jdcfg_p->imagewidth,JpgDecInfo.jdcfg_p->imageheight,PxlFmt,LineOff,PxlAddr);
            GxImg_InitBuf(&DstImgBuf,Width,Height,PxlFmtOut,GXIMAGE_LINEOFFSET_ALIGN(16),pJpg2Yuv->uiYAddr,ImgSize);
            GxImg_ScaleDataFine(&DecodedBuf, REGION_MATCH_IMG, &DstImgBuf, REGION_MATCH_IMG);
            pJpg2Yuv->uiYLineOffset     = DstImgBuf.LineOffset[0];
            pJpg2Yuv->uiUVLineOffset    = DstImgBuf.LineOffset[1];
            pJpg2Yuv->uiYAddr           = DstImgBuf.PxlAddr[0];
            pJpg2Yuv->uiUAddr           = DstImgBuf.PxlAddr[1];
            pJpg2Yuv->uiVAddr           = pJpg2Yuv->uiUAddr + 1;
            pJpg2Yuv->uiJpgWidth        = DstImgBuf.Width;
            pJpg2Yuv->uiJpgHeight       = DstImgBuf.Height;
            pJpg2Yuv->uiJpgFmt          = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
        }
#endif
    }
    else
    {
        DBG_ERR("JPEG decode error %d!\r\n", err);
    }

    return err;
}

//-------------------------------------------------------------------------------------------------
//
// Sample code for merging several YUV data to a movie.
//
//-------------------------------------------------------------------------------------------------
typedef struct
{
    UINT32  uiWidth;
    UINT32  uiHeight;
    UINT32  uiVidFrameRate;
    UINT32  uiTargetBitrate;
    UINT32  uiDispAspectRatio;
} MOVIE_SIZE_PARAM;

static MOVIE_SIZE_PARAM g_MovieSizeTable =
{1920,  1080,   30,     1450*1024,  VIDENC_DAR_DEFAULT  };  // 1080P, 30fps,  1450KB/sec, DAR default

static BOOL     g_bVidEncReady = FALSE;

extern void MediaRec_TriggerWriteFile(void);

static void JPG2MOV_RecCB(UINT32 uiEventID, UINT32 param)
{
    switch (uiEventID)
    {
    case MEDIAREC_EVENT_ENCONE_OK:
        g_bVidEncReady = TRUE;
        break;

    default:
        break;
    }
}

static void JPG2MOV_SetRecParam(void)
{
    UINT32  uiFileType;
    UINT32  uiWidth, uiHeight;
    UINT32  uiVidCodec, uiVidFrameRate, uiTargetBitrate, uiH264GopType, ui3DNRLevel, uiVidRotate, uiDispAspectRatio;
    UINT32  uiAudCodec, uiAudSampleRate, uiAudChannelNum, uiAudChannelType;
    BOOL    bAudFilterEn;
    //UINT32  uiWidth2, uiHeight2, uiTargetBitrate2, uiDispAspectRatio2, uiVidFrameRate2;

    uiFileType          = MEDIAREC_MOV;                         // file type
    //uiFileType          = MEDIAREC_MP4;                         // file type
    //uiFileType          = MEDIAREC_AVI;                         // file type

    uiWidth             = g_MovieSizeTable.uiWidth;             // image width
    uiHeight            = g_MovieSizeTable.uiHeight;            // image height
    uiVidFrameRate      = g_MovieSizeTable.uiVidFrameRate;      // video frame rate
    uiTargetBitrate     = g_MovieSizeTable.uiTargetBitrate;     // enable rate control and set target rate
    uiDispAspectRatio   = g_MovieSizeTable.uiDispAspectRatio;   // display aspect ratio

    uiH264GopType       = MEDIAREC_H264GOP_IPONLY;              // H.264 GOP type (IP only)
    ui3DNRLevel         = H264_3DNR_DISABLE;                    // H.264 3DNR disable

    uiAudSampleRate     = 32000;                                // audio sampling rate
    uiAudChannelType    = MEDIAREC_AUDTYPE_RIGHT;               // audio channel type
    uiAudChannelNum     = 1;                                    // audio channel number
    bAudFilterEn        = FALSE;                                // enable/disable audio filter

    switch (uiFileType)
    {
    case MEDIAREC_AVI: // AVI format, audio should be PCM, video can be H.264 or MJPEG
        uiAudCodec  = MEDIAREC_ENC_PCM;
        uiVidCodec  = MEDIAREC_ENC_H264;
        //uiVidCodec  = MEDIAREC_ENC_JPG;
        break;

    case MEDIAREC_MP4: // MP4 format, audio should be AAC, video should be H.264
        uiAudCodec  = MEDIAREC_ENC_AAC;
        uiVidCodec  = MEDIAREC_ENC_H264;
        break;

    case MEDIAREC_MOV: // MOV format, audio can be PCM or AAC, video can be H.264 or MJPEG
    default:
        //uiAudCodec  = MEDIAREC_ENC_AAC;
        uiAudCodec  = MEDIAREC_ENC_PCM;
        uiVidCodec  = MEDIAREC_ENC_H264;
        //uiVidCodec  = MEDIAREC_ENC_JPG;
        break;
    }

    uiVidRotate = MEDIAREC_MOV_ROTATE_0;

    //---------------------------------------------------------------------------------------------
    // set media recording control parameters
    //---------------------------------------------------------------------------------------------
    // disable golf shot recording
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_GOLFSHOT_ON, FALSE, 0, 0);
    // disable flash recording
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_FLASH_ON, FALSE, 0, 0);
    // disable power off protection
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_PWROFFPT, FALSE, 0, 0);
    // set recording format to MEDIAREC_MERGEYUV
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_RECFORMAT, MEDIAREC_MERGEYUV, 0, 0);
    // set customized user data on/off
    //MediaRec_ChangeParameter(MEDIAREC_RECPARAM_EN_CUSTOMUDATA, FALSE, 0, 0);
    // set file type (MEDIAREC_AVI, MEDIAREC_MOV, MEDIAREC_MP4)
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_FILETYPE, uiFileType, 0, 0);

    // set recording end type to MEDIAREC_ENDTYPE_NORMAL
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_ENDTYPE, MEDIAREC_ENDTYPE_NORMAL, 0, 0);

    // disable time lapse recording
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_TIMELAPSE, FALSE, 0, 0);

    //---------------------------------------------------------------------------------------------
    // set video parameters
    //---------------------------------------------------------------------------------------------
    // set image width
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_WIDTH, uiWidth, 0, 0);
    // set image height
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_HEIGHT, uiHeight, 0, 0);
    // set video frame rate
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_FRAMERATE, uiVidFrameRate, 0, 0);
    // set target data rate (bytes per second)
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_TARGETRATE, uiTargetBitrate, 0, 0);
    // set video codec (MEDIAREC_ENC_H264, MEDIAREC_ENC_JPG)
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_VIDEOCODEC, uiVidCodec, 0, 0);
    // set H.264 GOP type (MEDIAREC_H264GOP_IPONLY, MEDIAREC_H264GOP_IPB)
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_H264GOPTYPE, uiH264GopType, 0, 0);
    // set H.264 3DNR level (H264_3DNR_DISABLE, H264_3DNR_WEAKEST, H264_3DNR_WEAK, ...)
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_H2643DNRLEVEL, ui3DNRLevel, 0, 0);
    // set rotation information
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_MOV_ROTATE, uiVidRotate, 0, 0);
    // set YUV 420 format
    if (uiVidCodec == MEDIAREC_ENC_JPG)
        MediaRec_ChangeParameter(MEDIAREC_RECPARAM_JPG_YUVFORMAT, MEDIAREC_JPEG_FORMAT_420, 0, 0);
    // set display aspect ratio
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_DAR, uiDispAspectRatio, 0, 0);

    //DscMovie_Config(MOVIE_CFG_MULTIREC, 0);
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_MULTIREC_ON, FALSE, 0, 0);

    //---------------------------------------------------------------------------------------------
    // set audio parameters
    //---------------------------------------------------------------------------------------------
    // set audio sampling rate (AUDIO_SR_8000, AUDIO_SR_11025, AUDIO_SR_12000, ...)
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_AUD_SAMPLERATE, uiAudSampleRate, 0, 0);
    // set audio source (AUDIO_CH_RIGHT, AUDIO_CH_LEFT, AUDIO_CH_STEREO)
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_AUD_SRC, uiAudChannelType, 0, 0);
    // set audio channel number (1 or 2; if audio channel is mono but channel number is 2, the audio data will be duplicated)
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_AUD_CHS, uiAudChannelNum, 0, 0);
    // set audio codec (MEDIAREC_ENC_PCM, MEDIAREC_ENC_AAC)
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_AUD_CODEC, uiAudCodec, 0, 0);
    // set audio filter on/off
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_EN_AUDFILTER, bAudFilterEn, 0, 0);
}

INT32 JPG2MOV_MakeMovie(JPG2MOV_OBJ *pObj)
{
    #define BS_SIZE_MAX         (10*1024*1024)   // 10MB
    #define YUV_SIZE_MAX        (12*1024*1024)   // 12MB
    #define MEDIAREC_BUF_SIZE   (60*1024*1024)   // 60MB

    UINT32          uiPoolAddr, uiPoolSize, uiBSAddr, uiFileSize;
    UINT32          i, j, uiTimeCount, uiMediaRecAddr;
    JPG2YUV_INFO    Jpg2Yuv;
    FST_FILE        pFile;
    ER              err = E_OK;
    MEDIAREC_OBJ    MediaRecObj = {0};

    UINT32 FileNumber;
    FILEDB_HANDLE       fileDbHandle, fileDbbufSize=0x200000, fileDbbufAddr;
    PFILEDB_FILE_ATTR   pFileAttr;

    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_APP);
    uiPoolSize = OS_GetMempoolSize(POOL_ID_APP);

    uiBSAddr = uiPoolAddr + uiPoolSize - BS_SIZE_MAX;
    uiMediaRecAddr = uiPoolAddr + YUV_SIZE_MAX;
    fileDbbufAddr = uiBSAddr - fileDbbufSize;

    // ----------------------------------- <--- APP buffer starting address
    // | JPEG decode YUV buffer          | (original image format)
    // -----------------------------------
    // | YUV buffer 1 for media recorder | (image format transform: to YUV420 UV-pack, ...)
    // -----------------------------------
    // | YUV buffer 2 for media recorder |
    // -----------------------------------
    // | YUV buffer 3 for media recorder |
    // -----------------------------------
    // | ...                             |
    // -----------------------------------
    // | JPEG bitstream buffer           |
    // ----------------------------------- <--- IQ buffer end address
    //
    // Note: Be sure the buffer is enough for storing the necessary YUV patterns!

    Perf_Open();
    uiTimeCount = Perf_GetCurrent();
    MediaRecObj.CallBackFunc = JPG2MOV_RecCB;
    if (MediaRec_Open(&MediaRecObj) != E_OK)
    {
        DBG_ERR("MediaRec_Open() failed!\r\n");
        return E_SYS;
    }

    // reset video encode ready flag to TRUE
    g_bVidEncReady = TRUE;

    //MediaRec_OpenCutMsg(1);
    MediaRec_SetGiveYUVBuf(uiMediaRecAddr, MEDIAREC_BUF_SIZE);
    JPG2MOV_SetRecParam();
    //MediaRec_ChangeParameter(MEDIAREC_RECPARAM_PRE_ENCODE, FALSE, 0, 0);
    //MediaRec_ChangeParameter(MEDIAREC_RECPARAM_MERGEYUV, 1, 0, 0);
    MediaRec_Record();

    //  create fileDB for copy
    {
        FILEDB_INIT_OBJ   FilDBInitObj={0};
        //CHAR              rootPath[20]=FILEDB_TMLPS_ROOT;//"A:\\TMLPS\\";
        CHAR              rootPath[20]="A:\\TMLPS\\";

        FilDBInitObj.rootPath = rootPath;
        FilDBInitObj.bIsRecursive = TRUE;
        FilDBInitObj.bIsCyclic = TRUE;
        FilDBInitObj.bIsMoveToLastFile = TRUE;
        FilDBInitObj.bIsSupportLongName = TRUE;
        FilDBInitObj.bIsDCFFileOnly = FALSE;
        FilDBInitObj.bIsChkHasFile = FALSE;
        FilDBInitObj.u32MaxFilePathLen = 60;
        FilDBInitObj.u32MaxFileNum = 20000;
        FilDBInitObj.fileFilter = FILEDB_FMT_JPG;
        FilDBInitObj.u32MemAddr = dma_getCacheAddr(fileDbbufAddr);
        FilDBInitObj.u32MemSize = fileDbbufSize;
        fileDbHandle = FileDB_Create(&FilDBInitObj);
    }

    // merge JPEG file one by one
    FileNumber = FileDB_GetTotalFileNum(fileDbHandle);
    for (i = 0; i < FileNumber; i++)
    {
        DBG_MSG("Handle photo no. %d\r\n", i);

        // 1. Read JPG data from file
        pFileAttr = FileDB_SearhFile(fileDbHandle, i);
        //sprintf(sFileName, "A:\\TMLPS\\PHOTO\\2000_0509_043848_001.JPG", (i+1));
        uiFileSize = BS_SIZE_MAX;
        pFile = FileSys_OpenFile(pFileAttr->filePath, FST_OPEN_READ);
        if (pFile)
        {
            FileSys_ReadFile(pFile, (UINT8 *)uiBSAddr, &uiFileSize, 0, NULL);
            FileSys_CloseFile(pFile);
            DBG_MSG("File %s, size %d\r\n", sFileName, uiFileSize);

            if (uiFileSize == BS_SIZE_MAX)
            {
                DBG_ERR("Read buffer is not enough!\r\n");
                MediaRec_Stop(MEDIAREC_WAIT_END);
                MediaRec_Close();
                return E_SYS;
            }
        }
        else
        {
            DBG_ERR("Open %s for reading failed!\r\n", sFileName);
            MediaRec_Stop(MEDIAREC_WAIT_END);
            MediaRec_Close();
            return E_SYS;
        }

        // 2. Decode JPEG, do format transform and image scaling
        Jpg2Yuv.uiJpgAddr = uiBSAddr;
        Jpg2Yuv.uiJpgSize = uiFileSize;
        Jpg2Yuv.uiYAddr = uiPoolAddr;
        err = JPG2YUV_DecodeAndScale(&Jpg2Yuv);

        // 3. Wait for video encode ready last time
        j = 0;
        while (j < 40)
        {
            if (g_bVidEncReady) // check video encode ready
            {
                break;
            }

            Delay_DelayMs(1); // delay 1 ms
            j++;
        }

        // 4. Trigger writing file every second
        if (i && ((i % 30) == 0))
        {
            ////MediaRec_TriggerWriteFile();
            set_flg(FLG_ID_RECMEDIA, FLGPTN_BIT(23));
        }

        // 5. Do H.264 encode
        if (err == E_OK)
        {
            MEDIAREC_READYBUF_INFO readyInfo;

            readyInfo.y = Jpg2Yuv.uiYAddr;
            readyInfo.cb = Jpg2Yuv.uiUAddr;
            readyInfo.cr = Jpg2Yuv.uiVAddr;
            readyInfo.y_lot = Jpg2Yuv.uiYLineOffset;
            readyInfo.uv_lot = Jpg2Yuv.uiUVLineOffset;
            g_bVidEncReady = FALSE;
            MediaRec_GiveYUV(&readyInfo);
        }

        // 6. Update progress test
        if (pObj->fpProgressCB)
            pObj->fpProgressCB(((i+1)*100)/FileNumber);
    }
    FileDB_Release(fileDbHandle);

    MediaRec_Stop(MEDIAREC_WAIT_END);
    MediaRec_Close();
    uiTimeCount = Perf_GetCurrent() - uiTimeCount;
    DBG_DUMP("^YTotal merge time %d ms\r\n", uiTimeCount/1000);

    return E_OK;
}
