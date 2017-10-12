#include "PlaybackTsk.h"
#include "UIAppPlay.h"
#include "UIBackgroundObj.h"
#include "NVTUserCommand.h"
#include "SysCfg.h" //for memory POOL_ID
#include "UIInfo.h"

#include "SlideEffect.h"
#include "FileDB.h"
#include "GxDisplay.h"
#include "UIAppMovie.h"
#include "PrjCfg.h"
#include "BG_Images.h"
#include "NvtSystem.h"
#include "DPOF.h"
#include "DpofVendor.h"
#include "PBXFileList_DCF.h"
#include "PBXFileList_FileDB.h"
//#NT#2013/05/08#Calvin Chang#Register decode video file callback function -begin
#include "MovieUdtaVendor.h"      // for movie udta (user data) parsing
//#NT#2013/05/08#Calvin Chang -end
#include "SysMain.h"

#if(WIFI_AP_FUNC==ENABLE)
#include "RtspNvtApi.h"
#include "WifiAppCmd.h"
#endif


#define __MODULE__          UIPlayExe
//#define __DBGLVL__ 0        //OFF mode, show nothing
//#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGLVL__ 1        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

///////////////////////////////////////////////////////////////////////
//#NT#2010/09/10#Ben Wang -begin
//#NT#Refine code for sort by date only
//#NT#2010/08/09#Lincy Lin -begin
//#NT#Add index view for playback
#define FILEDB_SUPPORT_FILENUM   60000
//#NT#2010/08/09#Lincy Lin -end
#define PB_MAX_BUFFER_WIDTH     1920
#define PB_MAX_BUFFER_HEIGHT    1080


#define MAX_REC_TIME  60//sec

//#NT#2010/09/28#Jeah Yen -begin
ISIZE Play_GetBufferSize(void) //return USER CUSTOM buffer size
{
    ISIZE sz;
    DISPSRV_CMD Cmd={0};
    DISPSRV_DISP_CFG CfgDisp={0};
    Cmd.Idx = DISPSRV_CMD_IDX_GET_DISP;
    Cmd.Prop.bExitCmdFinish = TRUE;
    Cmd.Out.uiNumByte = sizeof(CfgDisp);
    Cmd.Out.pData = &CfgDisp;
    DispSrv_Cmd(&Cmd);
    sz.w = CfgDisp.Desc[DISPSRV_DISP_IDX_PRIMARY].pDisp[0].Width;
    sz.h = CfgDisp.Desc[DISPSRV_DISP_IDX_PRIMARY].pDisp[0].Height;
    return sz;
}
//#NT#2010/09/28#Jeah Yen -end

#if (_MOVIE_MODE_ == _MOVIE_ASFMPEG4_)
#define _PB_VALID_FORMAT        (PBFMT_JPG | PBFMT_WAV | PBFMT_ASF)
#elif ((_MOVIE_MODE_ == _MOVIE_MOVMJPG_)||(_MOVIE_MODE_ == _MOVIE_AVIMJPEG_))
#define _PB_VALID_FORMAT        (PBFMT_JPG | PBFMT_WAV | PBFMT_AVI | PBFMT_MOVMJPG | PBFMT_MP4)
#elif (_MOVIE_MODE_ == _MOVIE_MPEG1_)
#define _PB_VALID_FORMAT        (PBFMT_JPG | PBFMT_WAV | PBFMT_MPG)
#endif

static UINT32 Play_GetTempBuffer(UINT32 uiSize)
{
    void *pBuf = 0;
    PLAY_OBJ            PBObj;
    PB_GetParam(PBPRMID_PLAYBACK_OBJ, (UINT32 *)&PBObj);
    if(uiSize <= PBObj.uiMemorySize)
    {
        pBuf = (void*)PBObj.uiMemoryAddr;
        DBGH(pBuf);
    }
    if(pBuf == NULL)
    {
        DBG_ERR("get buffer fail\r\n");
    }
    return (UINT32)pBuf;
}



//#NT#2013/05/08#Calvin Chang#Register decode video file callback function -begin
BOOL PBDecVideoCB(UINT32 uiUserDataAddr, UINT32 uiUserDataSize)
{
    return MovieUdta_ParseVendorUserData(uiUserDataAddr, uiUserDataSize);
}
//#NT#2013/05/08#Calvin Chang -end


ISIZE _QueryDeviceModeAspect(UINT32 NewDevObj, UINT32 mode);
/**
  Initialize application for Playback mode

  Initialize application for Playback mode.

  @param void
  @return void
*/
INT32 PlayExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if 1

    MEM_RANGE Pool;
    ISIZE BufferSize, Buffer2Size;
    ISIZE DeviceSize, Device2Size;
    ISIZE AspectSize, Aspect2Size;
    UINT32 CurrDevObj, CurrDev2Obj;
    UINT32 useFileDB = 0;

    //call default
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_OPEN,paramNum,paramArray);

    Pool.Addr = OS_GetMempoolAddr(POOL_ID_APP);
    //#NT#2012/07/12#Ben Wang -begin
    //#NT#Add DPOF Init
    DpofVendor_Open(Pool.Addr);
    Pool.Addr += DPOF_BUFFER_SIZE;
    Pool.Size = POOL_SIZE_APP - DPOF_BUFFER_SIZE;
    //#NT#2013/02/20#Lincy Lin -begin
    //#NT#Support Filelist plugin
    #if USE_FILEDB
    UI_SetData(FL_IsUseFileDB, 1);
    #else
    UI_SetData(FL_IsUseFileDB, 0);
    #endif
    useFileDB = UI_GetData(FL_IsUseFileDB);
    if (useFileDB)
    {
        MEM_RANGE FDBMem;
        #if (YOUKU_SDK == ENABLE)
        char rootPath[NMC_OTHERS_MAX_LEN];
        CHAR*      FilterFolder="MOVIE|PHOTO";
        snprintf(rootPath,NMC_OTHERS_MAX_LEN,"A:\\%s\\",FILEDB_CARDV_ROOT);
        #else
        //CHAR*      rootPath="A:\\";
        CHAR*      rootPath="A:\\CarDV\\";
        #endif

        PPBX_FLIST_OBJ  pFlist = PBXFList_FDB_getObject();
        FDBMem.Addr = OS_GetMempoolAddr(POOL_ID_FILEDB);
        FDBMem.Size = POOL_SIZE_FILEDB;
        pFlist->Config(PBX_FLIST_CONFIG_MEM,FDBMem.Addr,FDBMem.Size);
        pFlist->Config(PBX_FLIST_CONFIG_MAX_FILENUM,10000,0);
        pFlist->Config(PBX_FLIST_CONFIG_MAX_FILEPATH_LEN,60,0);
        pFlist->Config(PBX_FLIST_CONFIG_VALID_FILETYPE,PBX_FLIST_FILE_TYPE_JPG|PBX_FLIST_FILE_TYPE_AVI|PBX_FLIST_FILE_TYPE_MOV|PBX_FLIST_FILE_TYPE_MP4,0);
        pFlist->Config(PBX_FLIST_CONFIG_DCF_ONLY,FALSE,0);
        //pFlist->Config(PBX_FLIST_CONFIG_SORT_TYPE,PBX_FLIST_SORT_BY_FILEPATH,0);
        pFlist->Config(PBX_FLIST_CONFIG_SORT_TYPE,PBX_FLIST_SORT_BY_MODDATE,0);
        pFlist->Config(PBX_FLIST_CONFIG_ROOT_PATH,(UINT32)rootPath,0);
        pFlist->Config(PBX_FLIST_CONFIG_SUPPORT_LONGNAME,1,0);
        //pFlist->Config(PBX_FLIST_CONFIG_DEFAULT_FOLDER,(UINT32)defaultfolder,0);
        #if (YOUKU_SDK == ENABLE)
        pFlist->Config(PBX_FLIST_CONFIG_FILTER_FOLDER,(UINT32)FilterFolder,0);
        #elif (defined(_SMALL_CLONE_MOVIE_) && (_SMALL_CLONE_MOVIE_ == ENABLE))
        // Note: the folder name should match which defined in MovieExe_OnOpen()
        pFlist->Config(PBX_FLIST_CONFIG_ROOT_PATH,(UINT32)DV_ROOT_PATH,0);
        pFlist->Config(PBX_FLIST_CONFIG_FILTER_FOLDER, (UINT32)PLAY_FOLDER_FILTER, 0);
        pFlist->Config(PBX_FLIST_CONFIG_RECURSIVE, TRUE, 0);
        #endif
        //pFlist->Config(PBX_FLIST_CONFIG_SORT_LARGEFIRST,1,0); // sorting sequence is large first

        PB_SetParam(PBPRMID_FILELIST_OBJ, (UINT32)pFlist);
    }
    //#else
    else
    {
        PB_SetParam(PBPRMID_FILELIST_OBJ, (UINT32)PBXFList_DCF_getObject());
    }
    //#endif
    //#NT#2013/02/20#Lincy Lin -end
    DscPlay_Config(PLAYBACK_CFG_POOL, (UINT32)&Pool);
    //#NT#2012/07/12#Ben Wang -end
    SxCmd_RegTempMemFunc(Play_GetTempBuffer);
    //#NT#2013/05/08#Calvin Chang#Register decode video file callback function -begin
    PB_SetParam(PBPRMID_DEC_VIDEO_CALLBACK, (UINT32)PBDecVideoCB);
    //#NT#2013/05/08#Calvin Chang -end

    //1.get current device size (current mode)
    DeviceSize = GxVideo_GetDeviceSize(DOUT1);
    BufferSize = DeviceSize;
    #if (_FPGA_EMULATION_ == ENABLE)
    //overwrite buffer size to reduce BW
    BufferSize.w = 320;
    BufferSize.h = 240;
    #endif
    //3.get its Aspect Ratio
    CurrDevObj = GxVideo_GetDevice(DOUT1);
    AspectSize = _QueryDeviceModeAspect(CurrDevObj, GxVideo_QueryDeviceLastMode(CurrDevObj));
    //config disp1 view
    AppView_ConfigEnable(0, TRUE);
    AppView_ConfigDevice(0, DeviceSize.w, DeviceSize.h, AspectSize.w, AspectSize.h);
//#NT#2012/11/13#Scottie -begin
//#NT#Temporarily use secondary display buffer as source buffer for dual display
//    AppView_ConfigBuffer(0, BufferSize.w, BufferSize.h, GX_IMAGE_PIXEL_FMT_YUV422_PACKED);
    AppView_ConfigBuffer(0, BufferSize.w, BufferSize.h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED);
//#NT#2012/11/13#Scottie -end
    AppView_ConfigWindow(0, 0, 0, DeviceSize.w, DeviceSize.h);

    //config disp2 view
    CurrDev2Obj = GxVideo_GetDevice(DOUT2);
    if (CurrDev2Obj)
    {
        Device2Size = GxVideo_GetDeviceSize(DOUT2);
        Buffer2Size.w = Device2Size.w;
        Buffer2Size.h = Device2Size.h;
        Aspect2Size = _QueryDeviceModeAspect(CurrDev2Obj, GxVideo_QueryDeviceLastMode(CurrDev2Obj));
        AppView_ConfigEnable(1, TRUE);
        AppView_ConfigDevice(1, Device2Size.w, Device2Size.h, Aspect2Size.w, Aspect2Size.h);
        //#NT#2012/11/13#Scottie -begin
        //#NT#Temporarily use secondary display buffer as source buffer for dual display
        //AppView_ConfigBuffer(1, Buffer2Size.w, Buffer2Size.h, GX_IMAGE_PIXEL_FMT_YUV422_PACKED);
        AppView_ConfigBuffer(1, Buffer2Size.w, Buffer2Size.h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED);
        //#NT#2012/11/13#Scottie -end
        AppView_ConfigWindow(1, 0, 0, Device2Size.w, Device2Size.h);
    }
    else
    {
        AppView_ConfigEnable(1, FALSE);
    }

    PB_SetParam(PBPRMID_MAX_PANELSZ, PB_MAX_BUFFER_WIDTH*PB_MAX_BUFFER_HEIGHT);
    //config src
    AppView_ConfigSource(&gPlay_src);
    //open
    AppView_Open();
    DscPlay_Open();
#else
    UINT32 uiPoolAddr, uiPBCacheAddr;
    PLAY_OBJ            PlayObj;
#if _WAV_FUNCTION_
    WAVSTUDIO_APPOBJ    WavObj;
    WAVSTUDIO_FMT       WavFormat;
#endif
//#NT#2010/08/09#Lincy Lin -begin
//#NT#Add index view for playback
#if( _DATEFOLDER_MODE_ || _IKEY_DATEVIEW_MODE_)
    UINT32  NeedBufSize;
#endif
//#NT#2010/08/09#Lincy Lin -end
    //#NT#2010/09/28#JeahYen -begin
    ISIZE BufferSize;
    ISIZE DeviceSize;
    //#NT#2010/09/28#JeahYen -end

    //#NT#2010/09/28#JeahYen -begin
    BufferSize = Play_GetBufferSize();
    DeviceSize = GxVideo_GetDeviceSize(DOUT1);
    //#NT#2010/09/28#JeahYen -end


    //#NT#2010/10/05#Lily Kao -begin
    //#Changeable Manufacture,Product names by project
    Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_SET_MOV_MANUNAME, 0);
    //#NT#2010/10/05#Lily Kao -end

    //call default
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_OPEN,paramNum,paramArray);

    // init the Playback object
    memset(&PlayObj, 0, sizeof(PLAY_OBJ));

    //
    // setting the Playback object..
    //
    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO1);
    //#NT#2010/11/04#Ben Wang -begin
    //#NT#use cache for fileDB
    uiPBCacheAddr = OS_GetMempoolAddr(POOL_ID_CACHE);
#if( _DATEFOLDER_MODE_ || _IKEY_DATEVIEW_MODE_)
    NeedBufSize = PB_SetBuffForFileDB(uiPBCacheAddr, FILEDB_SUPPORT_FILENUM);
    if(NeedBufSize > POOL_SIZE_CACHE)
    {
        debug_err(("Not enough cache memory for FileDB, use playback memory instead\r\n"));
        PB_SetBuffForFileDB(uiPoolAddr, FILEDB_SUPPORT_FILENUM);
        PlayObj.uiMemoryAddr        = uiPoolAddr + NeedBufSize;
        PlayObj.uiMemorySize        = POOL_SIZE_DISP_VDO1 + POOL_SIZE_DISP_VDO1TEMP + POOL_SIZE_APP - POOL_SIZE_WAV - POOL_SIZE_PBRSV - NeedBufSize;
    }
    else
#endif
    {
        PlayObj.uiMemoryAddr        = uiPoolAddr;
        PlayObj.uiMemorySize        = POOL_SIZE_DISP_VDO1 + POOL_SIZE_DISP_VDO1TEMP + POOL_SIZE_APP - POOL_SIZE_WAV - POOL_SIZE_PBRSV;
    }
    //#NT#2010/11/04#Ben Wang -end


    PlayObj.CallBackFunc        = NULL;

#if (_MOVIE_MODE_ == _MOVIE_ASFMPEG4_)
    PlayObj.usPlayFileFormat    = PBFMT_JPG | PBFMT_WAV | PBFMT_ASF;
#elif ((_MOVIE_MODE_ == _MOVIE_MOVMJPG_)||(_MOVIE_MODE_ == _MOVIE_AVIMJPEG_))
    PlayObj.usPlayFileFormat    = PBFMT_JPG | PBFMT_WAV | PBFMT_AVI | PBFMT_MOVMJPG;
#elif (_MOVIE_MODE_ == _MOVIE_MPEG1_)
    PlayObj.usPlayFileFormat    = PBFMT_JPG | PBFMT_WAV | PBFMT_MPG;
#endif

    PlayObj.Orientation         = PLAY_HORIZONTAL;

    PlayObj.Flags               = PB_ENABLE_KEEP_ASPECT_RATIO; //PB_DISABLE_TRANSFROM;
#if (PLAY_BG_MOVIE)
    PlayObj.Flags              |= PB_ENABLE_SHOW_BG_IN_MOVIE;
#endif
#if (PLAY_BG_THUMBNAIL)
    PlayObj.Flags              |= PB_ENABLE_SHOW_BACKGROUND_IN_THUMB;
#endif
#if (PLAY_BG_BADFILE)
    PlayObj.Flags              |= PB_ENABLE_SHOW_BG_IN_BADFILE;
#endif
    //#NT#2010/08/27#Ben Wang -begin
    //#Place Hidden image after primary image
    PlayObj.Flags              |= PB_ENABLE_SPEEDUP_SCREENNAIL;
    //#NT#2010/08/27#Ben Wang -end

    PlayObj.uiThumbShowImageMode= PB_SHOW_THUMB_IMAGE_IN_THE_SAME_TIME;
    //#NT#2010/08/04#Ben Wang -begin
    //#NT#Refine code for EXIF rotation
    PlayObj.bEnableAutoRotate   = TRUE;
    //#NT#2010/08/04#Ben Wang -end
    //#NT#2010/12/06#Ben Wang -begin
    //#NT#i key playback don't need to show last image
    if(UI_GetData(FL_PlayIKeyModeIndex) == PLAY_BY_DATE)
        PlayObj.bDisplayLastImg     = FALSE;
    else
        PlayObj.bDisplayLastImg     = TRUE;
    //#NT#2010/12/06#Ben Wang -end
    //#NT#2010/11/04#Ben Wang -begin
    //#NT#use cache for fileDB
    #if ( _DATEFOLDER_MODE_ || _IKEY_DATEVIEW_MODE_)
    PlayObj.uiCacheMemSize = 0;
    #else
//#NT#2010/10/25#Scottie -begin
//#NT#Support cache memory for Playback application
    PlayObj.uiCacheMemAddr = uiPBCacheAddr;
    PlayObj.uiCacheMemSize = POOL_SIZE_CACHE;
//#NT#2010/10/25#Scottie -end
    #endif
    //#NT#2010/11/04#Ben Wang -end

    // Open playback task
    if (PB_Open(&PlayObj) != E_OK)
    {
        debug_err(("Error open playback task\r\n"));
    }

#if (PLAY_BG_MOVIE)
    // Set the position and size of first video frame
    if (GxVideo_GetDevice(DOUT1) == gDevTVObj)
    {
        if (g_uiTVMode == TV_MODE_NTSC)
        {
            PB_Set1stVideoFrame(VIDFRAME_POS_X_NTSC, VIDFRAME_POS_Y_NTSC,
                                VIDFRAME_WIDTH_NTSC, VIDFRAME_HEIGHT_NTSC);
        }
        else//if (g_uiTVMode == TV_MODE_PAL)
        {
            PB_Set1stVideoFrame(VIDFRAME_POS_X_PAL, VIDFRAME_POS_Y_PAL,
                                VIDFRAME_WIDTH_PAL, VIDFRAME_HEIGHT_PAL);
        }
    }
    else
    {
        PB_Set1stVideoFrame(VIDFRAME_POS_X, VIDFRAME_POS_Y,
                            VIDFRAME_WIDTH, VIDFRAME_HEIGHT);
    }
#endif

#if _WAV_FUNCTION_
    // Set WAV application object
    //WavObj.uiMemoryAddr         = uiPoolAddr + POOL_SIZE_APP - POOL_SIZE_WAV;
    WavObj.uiMemoryAddr         = PlayObj.uiMemoryAddr + PlayObj.uiMemorySize;
    WavObj.uiMemorySize         = POOL_SIZE_WAV;
    WavObj.WavStudioCB          = (WAVSTUDIO_CB)Play_WavCB;


    WavFormat.AudioChannel      = AUDIO_CH_RIGHT;
    WavFormat.AudioSamplingRate = AUDIO_SR_11025;
    WavFormat.AudioCodec        = AUDIOCODEC_PCM;//AUDIOCODEC_IMA_ADPCM;
    //#NT#2010/10/29#Ben Wang -begin
    //#NT#refine code for wav init
    WavFormat.uiBitsPerSample   = WAV_BITS_PER_SAM_PCM_16;
    //#NT#2010/10/29#Ben Wang -end
#if WAVFILE_ASF_ENABLE
    WavFormat.AudioCodec        = AUDIOCODEC_PCM;
    WavFormat.FileFormat        = WAVFILE_ASF;//WAVFILE_RIFF
#endif
    WavStudio_SetFormat(&WavFormat);

    WavStudio_SetMaxRecTime(MAX_REC_TIME);

    // Open Wav studio task
    if (WavStudio_Open(&WavObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for wav studio task\r\n"));
    }
#endif

    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_APP);
    uiPoolAddr = uiPoolAddr + POOL_SIZE_APP - POOL_SIZE_PBRSV;
    {
        PBRSV_DATA RsvData;
        RsvData.uiPBRsvBuffAddr = uiPoolAddr;
        RsvData.uiPBRsvBuffSize = POOL_SIZE_PBRSV;
        //#NT#2010/09/28#JeahYen -begin
        RsvData.uiDisplayWidth  = BufferSize.w;
        RsvData.uiDisplayHeight = BufferSize.h;
        //#NT#2010/09/28#JeahYen -end
        #if PLAY_BG_MOVIE
        RsvData.uiMovBgFileAddr = (UINT32)g_ucBGMovie_320x240;
        RsvData.uiMovBgFileSize = BG_MOVIE_BS_SIZE;
        #else
        RsvData.uiMovBgFileAddr = NULL;
        RsvData.uiMovBgFileSize = 0;
        #endif
        #if PLAY_BG_BADFILE
        RsvData.uiBadBgFileAddr = (UINT32)g_ucBGBadFile_320x240;
        RsvData.uiBadBgFileSize = BG_BADFILE_BS_SIZE;
        #else
        RsvData.uiBadBgFileAddr = NULL;
        RsvData.uiBadBgFileSize = 0;
        #endif
        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_UPDATE_RSVDATA, 1, (UINT32)(&RsvData));
    }
#endif
#if(WIFI_AP_FUNC==ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        RTSPNVT_OPEN Open={0};
        Open.uiApiVer = RTSPNVT_API_VERSION;
        Open.Type = RTSPNVT_TYPE_PLAYBACK;
        Open.uiPortNum = 554;
        Open.uiWorkAddr = OS_GetMempoolAddr(POOL_ID_APP);
        Open.uiWorkSize = POOL_SIZE_APP;
        RtspNvt_Open(&Open);
        DscMovie_RegRTSPSetBSCB(RtspNvt_RTSPSetBS);
        WifiCmd_Done(WIFIFLAG_MODE_DONE,E_OK);
    }
#endif
    return NVTEVT_CONSUME;
}

INT32 PlayExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

#if 1
#if(WIFI_AP_FUNC==ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        DscMovie_RegRTSPSetBSCB(NULL);
        RtspNvt_Close();
    }
#endif
    DpofVendor_Close();

    //PB_Close(PB_WAIT_INFINITE);
    DscPlay_Close();

    //WavStudio_Close();

    //call default
    //Ux_DefaultEvent(pCtrl,NVTEVT_EXE_CLOSE,paramNum,paramArray);
    AppView_Close();

#else
    PB_Close(PB_WAIT_INFINITE);
    WavStudio_Close();

    //call default
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_CLOSE,paramNum,paramArray);
#endif
    return NVTEVT_CONSUME;
}
//#NT#2010/09/10#Ben Wang -end
INT32 PlayExe_OnCopy2Card(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _DEMO_TODO
    debug_msg("[PlayObj] do copy to card\r\n");
    BKG_PostEvent(NVTEVT_BKW_COPY2CARD);
#endif
    return NVTEVT_CONSUME;
}
INT32 PlayExe_OnSlide(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];
    UI_SetData(FL_SLIDE_SHOW,uhSelect);

    return NVTEVT_CONSUME;
}
INT32 PlayExe_OnVoiceMemo(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];
    UI_SetData(FL_VMIndex,uhSelect);

    return NVTEVT_CONSUME;
}
INT32 PlayExe_OnPlayEffect(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];
    UI_SetData(FL_PlayEffectIndex,uhSelect);

    return NVTEVT_CONSUME;
}

////////////////////////////////////////////////////////////

EVENT_ENTRY CustomPlayObjCmdMap[] =
{
    {NVTEVT_EXE_OPEN,               PlayExe_OnOpen            },
    {NVTEVT_EXE_CLOSE,              PlayExe_OnClose            },
    {NVTEVT_EXE_COPY2CARD,          PlayExe_OnCopy2Card        },
    {NVTEVT_EXE_SLIDE,              PlayExe_OnSlide            },
    {NVTEVT_EXE_VOICEMEMO,          PlayExe_OnVoiceMemo       },
    {NVTEVT_EXE_PLAYEFFECT,         PlayExe_OnPlayEffect       },
    {NVTEVT_NULL,                   0},  //End of Command Map
};

CREATE_APP(CustomPlayObj,APP_PLAY)

