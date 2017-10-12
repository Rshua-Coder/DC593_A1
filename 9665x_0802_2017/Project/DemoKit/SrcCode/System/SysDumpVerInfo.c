#include "Delay.h"
#include "NvtVerInfo.h"
#include "SwTimer.h"


//---------------------SysCmdCB Debug Definition -----------------------------
//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysCmdCB
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//---------------------SysCmdCB Global Variables -----------------------------
typedef CHAR* (*VERINFO_FP)(void);
typedef CHAR* (*BUILDDATE_FP)(void);

typedef struct _VER_INFO
{
    CHAR*          description;          ///<
    CHAR*          name;                 ///<
    VERINFO_FP     verInfoFunc;
    BUILDDATE_FP   BuildDateFunc;
}VER_INFO, *PVER_INFO;

VER_INFO common_ver[]={
#if 0
{"Debug",        "Debug.a",   Debug_GetVerInfo,   Debug_GetBuildDate  },
{"Exam Service", "ExamSrv.a", ExamSrv_GetVerInfo, ExamSrv_GetBuildDate},
{"Kernel",       "Kernel.a",  Ker_GetVerInfo,     Ker_GetBuildDate    },
{"OS",           "OS.a",      OS_GetVerInfo,      OS_GetBuildDate     },
#ifndef __ECOS
{"Startup",      "Startup.a", Strup_GetVerInfo,   Strup_GetBuildDate  },
#endif
{"Utility",      "Utility.a", Util_GetVerInfo,    Util_GetBuildDate   },
#endif
{"","",NULL,NULL}  // this item should be the last item
};

VER_INFO driver_ver[]={
#if 0
{"Audio",            "Audio_Common.a", aud_getVerInfo,       aud_getBuildDate     },
{"Codec Driver",     "Driver_Codec.a", drvcodec_getVerInfo,  drvcodec_getBuildDate},
{"CPU DMA Utility",  "DbgUt.a",        DbgUt_GetVerInfo,     DbgUt_GetBuildDate   },
{"Dx System",        "DxSys.a",        DxSys_GetVerInfo,     DxSys_GetBuildDate   },
{"Dx",               "Dx.a",           Dx_GetVerInfo,        Dx_GetBuildDate      },
{"Display Obj",      "Dispobj.a",      disp_getVerInfo,      disp_getBuildDate    },
{"IPP Driver",       "Driver_IPP.a",   drvipp_getVerInfo,    drvipp_getBuildDate  },
{"RamDisk",          "RamDisk.a",      ramdsk_getVerInfo,    ramdsk_getBuildDate  },
{"System Driver",    "Driver_Sys.a",   drvsys_getVerInfo,    drvsys_getBuildDate  },
{"SPI Flash",        "SpiFlash.a",     spiflash_getVerInfo,  spiflash_getBuildDate},
{"Sensor interface", "Sensor.a",       Sensor_GetVerInfo,    Sensor_GetBuildDate  },
#endif
{"","",NULL,NULL}  // this item should be the last item
};

VER_INFO lib_ver[]={
#if 0
{"ADPCM",               "ADPCM.a",            Adpcm_GetVerInfo,          Adpcm_GetBuildDate         },
{"Audio Filter",        "AudFilt.a",          AudFilt_GetVerInfo,        AudFilt_GetBuildDate       },
{"Audio NR",            "AudNR.a",            AudNR_GetVerInfo,          AudNR_GetBuildDate         },
{"AE Alg",              "AEAlg.a",            AeAlg_GetVerInfo,          AeAlg_GetBuildDate         },
{"AE Task",             "AETask.a",           AeTsk_GetVerInfo,          AeAlg_GetBuildDate         },
{"AWB Alg",             "AWBAlg.a",           AWBAlg_GetVerInfo,         AWBAlg_GetBuildDate        },
{"AWB Task",            "AWBTask.a",          AWBTsk_GetVerInfo,         AWBAlg_GetBuildDate        },
{"AF Alg",              "AF_Alg.a",           AfAlg_GetVerInfo,          AfAlg_GetBuildDate         },
{"AF Task",             "AF_Task.a",          AfTask_GetVerInfo,         AfTask_GetBuildDate        },
{"AAC API",             "AAC.a",              AAC_GetVerInfo,            AAC_GetBuildDate           },
{"Audio Codec AAC",     "AudioCodec_AAC.a",   AudioCodecAAC_GetVerInfo,  AudioCodecAAC_GetBuildDate },
{"Audio Codec PCM",     "AudioCodec_PCM.a",   AudioCodecPCM_GetVerInfo,  AudioCodecPCM_GetBuildDate },
{"Audio Codec Zero",    "AudioCodec_Zero.a",  AudioCodecZero_GetVerInfo, AudioCodecZero_GetBuildDate},
{"Audio Process (Filter/NR)", "AudProc.a",    AudFilter_GetVerInfo,      AudFilter_GetBuildDate     },
{"AVI File Maker",      "AVFile_MakerAvi.a",  MP_AviWriteLib_GetVerInfo, MP_AviWriteLib_GetBuildDate},
{"AVI File Parser",     "AVFile_ParserAvi.a", MP_AviReadLib_GetVerInfo,  MP_AviReadLib_GetBuildDate },
{"CEC",                 "CEC.a",              Cec_GetVerInfo,            Cec_GetBuildDate           },
{"CoordTs",             "CoordTs.a",          CoordTs_GetVerInfo,        CoordTs_GetBuildDate       },
{"CompareView",         "CompareView.a",      CompView_GetVerInfo,       CompView_GetBuildDate      },
{"DataStamp",           "DataStamp.a",        DataStamp_GetVerInfo,      DataStamp_GetBuildDate     },
{"DCF",                 "DCF.a",              DCF_GetVerInfo,            DCF_GetBuildDate           },
{"DHCPNvt",             "DhcpNvt.a",          DhcpNvt_GetVerInfo,        DhcpNvt_GetBuildDate       },
{"DIS",                 "DIS.a",              Dis_GetVerInfo,            Dis_GetBuildDate           },

{"DPOF",                "DPOF.a",             DPOF_GetVerInfo,           DPOF_GetBuildDate          },
{"Display Service",     "DispSrv.a",          DispSrv_GetVerInfo,        DispSrv_GetBuildDate       },
{"EXIF",                "EXIF.a",             Exif_GetVerInfo,           Exif_GetBuildDate          },
{"Flash charge control","GxFlash.a",          GxFlash_GetVerInfo,        GxFlash_GetBuildDate       },
{"Font To Yuv",         "FontToYuv.a",        FontToYuv_GetVerInfo,      FontToYuv_GetBuildDate     },
{"Firmware Service",    "FwSrv.a",            FwSrv_GetVerInfo,          FwSrv_GetBuildDate         },
{"Flashlight Alg",      "FlashlightAlg.a",    FlashAlg_GetVerInfo,       FlashAlg_GetBuildDate      },
{"File DataBase",       "FileDB.a",           FileDB_GetVerInfo,         FileDB_GetBuildDate        },
{"File System",         "FileAccess.a",       FileSys_GetVerInfo,        FileSys_GetBuildDate       },
#if _FD_FUNC_
{"Face Detection",      "FD.a",               FD_GetVerInfo,             FD_GetBuildDate            },
#endif
{"GxGfx",               "GxGfx.a",            GxGfx_GetVerInfo,          GxGfx_GetBuildDate         },
{"GxOutput",            "GxOutput.a",         GxOutput_GetVerInfo,       GxOutput_GetBuildDate      },
{"GxDisplay",           "GxDisplay.a",        GxDisplay_GetVerInfo,      GxDisplay_GetBuildDate     },
{"GxVideo",             "GxVideo.a",          GxVideo_GetVerInfo,        GxVideo_GetBuildDate       },
{"GxTimer",             "GxTimer.a",          GxTimer_GetVerInfo,        GxTimer_GetBuildDate       },
{"GxSound",             "GxSound.a",          GxSound_GetVerInfo,        GxSound_GetBuildDate       },
{"Gx System",           "GxSystem.a",         GxSystem_GetVerInfo,       GxSystem_GetBuildDate      },
{"H264 Library",        "H264.a",             H264Codec_GetVerInfo,      H264Codec_GetBuildDate     },
{"HW Accelerate",       "HWAcc.a",            HWAcc_GetVerInfo,          HWAcc_GetBuildDate         },
{"IPL Common",          "Ipl_Common.a",       IPL_GetVerInfo,            IPL_GetBuildDate           },
{"IPL Display",         "IPL_Display.a",      IplDisplay_GetVerInfo,     IplDisplay_GetBuildDate    },
{"IPL Cal RW",          "IPL_CAL_RW.a",       IPL_CalRW_GetVerInfo,      IPL_CalRW_GetBuildDate     },
{"IPL CAL",             "IPL_CAL.a",          IPL_Cal_GetVerInfo,        IPL_Cal_GetBuildDate       },
{"ImgStat",             "ImgStat.a",          ImgStat_GetVerInfo,        ImgStat_GetBuildDate       },
{"Image encod/decode",  "GxImageFile.a",      GxImgFile_GetVerInfo,      GxImgFile_GetBuildDate     },
{"Image D2D Process",   "GxImage.a",          GxImg_GetVerInfo,          GxImg_GetBuildDate         },
{"JPEG BRC",            "JPEGBRC_Default.a",  JpgBrc_GetVerInfo,         JpgBrc_GetBuildDate        },
{"Key/touch detection", "GxInput.a",          GxInput_GetVerInfo,        GxInput_GetBuildDate       },
{"Msdc Nvt",            "MsdcNvt.a",          MsdcNvt_GetVerInfo,           MsdcNvt_GetBuildDate          },
{"MsdcNvt Plug-File",   "MsdcNvtCb_File.a",   MsdcNvtRegBi_File_GetVerInfo, MsdcNvtRegBi_File_GetBuildDate},
{"MsdcNvt Plug-Disp",   "MsdcNvtCb_Disp.a",   MsdcNvtRegBi_Disp_GetVerInfo, MsdcNvtRegBi_Disp_GetBuildDate},
{"MotionDetectionAlg",  "MDAlg.a",            MDAlg_GetVerInfo,          MDAlg_GetBuildDate         },
{"Movie Play (single frame)", "GxVideoFile.a", GxVidFile_GetVerInfo,     GxVidFile_GetBuildDate     },
{"MOV File Maker",      "AVFile_MakerMov.a",  MP_MovWriteLib_GetVerInfo, MP_MovWriteLib_GetBuildDate},
{"MOV File Parser",     "AVFile_ParserMov.a", MP_MovReadLib_GetVerInfo,  MP_MovReadLib_GetBuildDate },
{"NameRule DCFFull",    "NameRule_DCFFull.a", NameRuleDCFFull_GetVerInfo, NameRuleDCFFull_GetBuildDate},
{"NameRule FileDB",     "NameRule_FileDB.a",  NameRuleFileDB_GetVerInfo,  NameRuleFileDB_GetBuildDate },
{"PB Image Edit Plug",  "PBXEdit.a",          PBXEdit_GetVerInfo,        PBXEdit_GetBuildDate       },
{"PB FileAccess Plug",  "PBXFile.a",          PBXFile_GetVerInfo,        PBXFile_GetBuildDate       },
{"PB FileList DCF Plug", "PBXFileList_DCF.a", PBXFList_DCF_GetVerInfo,   PBXFList_DCF_GetBuildDate  },
{"PB FileList FDB Plug", "PBXFileList_FILEDB.a", PBXFList_FDB_GetVerInfo, PBXFList_FDB_GetBuildDate },
{"PB RedEye Remove Plug","PBXRedEye.a",       PBXEdit_RedEye_GetVerInfo, PBXEdit_RedEye_GetBuildDate},
{"Partial Load",        "PartLoad.a",         PartLoad_GetVerInfo,       PartLoad_GetBuildDate      },
{"PStore",              "PStore.a",           PStore_GetVerInfo,         PStore_GetBuildDate        },
{"Power Control",       "GxPower.a",          GxPower_GetVerInfo,        GxPower_GetBuildDate       },
{"PhotoFrame",          "PhotoFrame.a",       PhotoFrame_GetVerInfo,     PhotoFrame_GetBuildDate    },
{"ImageEffect",         "ImageEffect.a",      ImageEffect_GetVerInfo,    ImageEffect_GetBuildDate   },
{"RSC",                 "RSC.a",              Rsc_GetVerInfo,            Rsc_GetBuildDate           },
{"Rectangle Zoom",      "RectZoom.a",         RectZoom_GetVerInfo,       RectZoom_GetBuildDate      },
{"SE Common lib",       "SlideEffect_Default.a",             SlideEffect_GetVerInfo,           SlideEffect_GetBuildDate          },
{"SE ClientLeft2Right", "SlideEffect_ClientLeft2Right.a",    SE_ClientLeft2Right_GetVerInfo,   SE_ClientLeft2Right_GetBuildDate  },
{"SE ClientRight2Left", "SlideEffect_ClientRight2Left.a",    SE_ClientRight2Left_GetVerInfo,   SE_ClientRight2Left_GetBuildDate  },
{"SE Fade",             "SlideEffect_Fade.a",                SE_Fade_GetVerInfo,               SE_Fade_GetBuildDate              },
{"SE PushFadeRight2Left","SlideEffect_PushFadeRight2Left.a", SE_PushFadeRight2Left_GetVerInfo, SE_PushFadeRight2Left_GetBuildDate},
{"SE R5HDrapeUp2Down",  "SlideEffect_R5HDrapeUp2Down.a",     SE_R5HDrapeUp2Down_GetVerInfo,    SE_R5HDrapeUp2Down_GetBuildDate   },
{"SE SlideinRight2Left","SlideEffect_SlideinRight2Left.a",   SE_SlideinRight2Left_GetVerInfo,  SE_SlideinRight2Left_GetBuildDate },
{"SizeConvert",         "SizeConvert.a",      SizeConvert_GetVerInfo,    SizeConvert_GetBuildDate   },
{"Storage Mount",       "GxStrg.a",           GxStrg_GetVerInfo,         GxStrg_GetBuildDate        },
{"Scene Render",        "SceneRenderLib.a",   SR_GetVerInfo,             SR_GetBuildDate            },
#if _SD_FUNC_
{"Smile Detection",     "SD.a",               SD_GetVerInfo,             SD_GetBuildDate            },
#endif
{"UserJPEG",            "UserJPEG.a",         Jpg_GetVerInfo,            Jpg_GetBuildDate           },
{"USB detection",       "GxUSB.a",            GxUSB_GetVerInfo,          GxUSB_GetBuildDate         },
{"USB MSDC",            "UMSD.a",             Msdc_GetVerInfo,           Msdc_GetBuildDate          },
{"USB PCC (UVC)",       "UPCC.a",             Uvc_GetVerInfo,            Uvc_GetBuildDate           },
{"USB SIDC",            "USIDC.a",            Sidc_GetVerInfo,           Sidc_GetBuildDate          },
{"Video Codec H264",    "VideoCodec_H264.a",  VideoCodecH264_GetVerInfo, VideoCodecH264_GetBuildDate},
{"Video Codec MJPG",    "VideoCodec_MJPG.a",  VideoCodecMJPG_GetVerInfo, VideoCodecMJPG_GetBuildDate},
{"Video Codec BStr",    "VideoCodec_BStr.a",  VideoCodecBStr_GetVerInfo, VideoCodecBStr_GetBuildDate},
{"WavStudio",           "WavStudio.a",        WavStudio_GetVerInfo,      WavStudio_GetBuildDate     },
{"WAVFile",             "WAVFile.a",          WavLib_GetVerInfo,         WavLib_GetBuildDate        },
#endif
{"","",NULL,NULL}  // this item should be the last item
};

VER_INFO app_ver[]={
#if 0
{"AppControl",     "AppControl.a", AppCtrl_GetVerInfo,   AppCtrl_GetBuildDate  },
{"Capture App",    "Capture.a",    ImgCap_GetVerInfo,    ImgCap_GetBuildDate   },
{"DevMan",         "DevMan.a",     DevMan_GetVerInfo,    DevMan_GetBuildDate   },
#if HTTP_LIVEVIEW_FUNC
{"Liveview App",   "LviewNvt.a",   LviewNvt_GetVerInfo,  LviewNvt_GetBuildDate },
#endif
{"Media Play App", "MediaPlay.a",  MediaPlay_GetVerInfo, MediaPlay_GetBuildDate},
{"Media Rec App",  "MediaRec.a",   MediaRec_GetVerInfo,  MediaRec_GetBuildDate },
{"NvtUser",        "NvtUser.a",    NvtUser_GetVerInfo,   NvtUser_GetBuildDate  },
{"NvtSystem",      "NvtSystem.a",  NvtSystem_GetVerInfo, NvtSystem_GetBuildDate},
{"Playback App",   "Playback.a",   PB_GetVerInfo,        PB_GetBuildDate       },
{"Photo App",      "Photo.a",      Photo_GetVerInfo,     Photo_GetBuildDate    },
{"RtspNvt App",    "RtspNvt.a",    RtspNvt_GetVerInfo,   RtspNvt_GetBuildDate },
{"UIControl",      "UIControl.a",  UICtrl_GetVerInfo,    UICtrl_GetBuildDate   },
{"VControl",       "VControl.a",   VControl_GetVerInfo,  VControl_GetBuildDate },
#endif
{"","",NULL,NULL}  // this item should be the last item
};

#ifdef __ECOS
#if 0
static VER_INFO eCos_Ver[] =
{
    { "eCos kernel - extras",      "libextras.a",      ecoskerlibextras_GetVerInfo, ecoskerlibextras_GetBuildDate },
    { "eCos kernel - target",      "libtarget.a",      ecoskerlibtarget_GetVerInfo, ecoskerlibtarget_GetBuildDate },
    { "eCos library - iPerf",      "Iperf.a",          Iperf_GetVerInfo,            Iperf_GetBuildDate            },
    { "eCos library - Live555",    "Live555.a",        Live555_GetVerInfo,          Live555_GetBuildDate          },
    { "eCos supplement - FileSys", "FileSysECOS.a",    FileSysECOS_GetVerInfo,      FileSysECOS_GetBuildDate      },
    { "eCos supplement - spieth",  "libethnt96650.a",  devsspieth_GetVerInfo,       devsspieth_GetBuildDate       },
    { "eCos supplement - lviewd",  "liblviewd.a",      Lviewd_GetVerInfo,           Lviewd_GetBuildDate           },
    { "eCos supplement - sdio",    "libsdiont96650.a", devssdio_GetVerInfo,         devssdio_GetBuildDate         },
    { "eCos supplement - PStore",  "PStoreECOS.a",     PStoreECOS_GetVerInfo,       PStoreECOS_GetBuildDate       },
    {"","",NULL,NULL}  // this item should be the last item
};

static void dumpver(VER_INFO verInfo[])
{
    UINT32      i=0;
    PVER_INFO   pVerInfo;
    pVerInfo = &verInfo[i];
    while (pVerInfo->verInfoFunc)
    {
        DBG_DUMP("%-25s (%-20s):%s, %s\r\n",pVerInfo->description,pVerInfo->name,pVerInfo->verInfoFunc(),pVerInfo->BuildDateFunc());
        SwTimer_DelayMs(5);
        i++;
        pVerInfo = &verInfo[i];
    }
}
#endif
#endif

#if 0
void SysDump_ver_common(void)
{
    DBG_DUMP("\r\n=========================================================\r\n");
    SwTimer_DelayMs(5);
    DBG_DUMP("    NT96650 Common version info.\r\n");
    SwTimer_DelayMs(5);
    DBG_DUMP("=========================================================\r\n");
    SwTimer_DelayMs(5);
    //dumpver(common_ver);
}

void SysDump_ver_drv(void)
{
    DBG_DUMP("\r\n=========================================================\r\n");
    SwTimer_DelayMs(5);
    DBG_DUMP("    NT96650 Driver version info.\r\n");
    SwTimer_DelayMs(5);
    DBG_DUMP("=========================================================\r\n");
    SwTimer_DelayMs(5);
    //dumpver(driver_ver);
}

void SysDump_ver_lib(void)
{
    DBG_DUMP("\r\n=========================================================\r\n");
    SwTimer_DelayMs(5);
    DBG_DUMP("    NT96650 Library version info.\r\n");
    SwTimer_DelayMs(5);
    DBG_DUMP("=========================================================\r\n");
    SwTimer_DelayMs(5);
    //dumpver(lib_ver);
}

void SysDump_ver_app(void)
{
    DBG_DUMP("\r\n=========================================================\r\n");
    SwTimer_DelayMs(5);
    DBG_DUMP("    NT96650 Application version info.\r\n");
    SwTimer_DelayMs(5);
    DBG_DUMP("=========================================================\r\n");
    SwTimer_DelayMs(5);
    //dumpver(app_ver);
}

#ifdef __ECOS
void SysDump_ver_eCos(void)
{
    DBG_DUMP("\r\n=========================================================\r\n");
    SwTimer_DelayMs(5);
    DBG_DUMP("    NT96650 eCos version info.\r\n");
    SwTimer_DelayMs(5);
    DBG_DUMP("=========================================================\r\n");
    SwTimer_DelayMs(5);
    dumpver(eCos_Ver);
}
#endif
#endif

void SysDump_ver_all(void)
{
#ifdef __ECOS
    //SysDump_ver_eCos();
#endif

    //SysDump_ver_common();
    //SysDump_ver_drv();
    //SysDump_ver_lib();
    //SysDump_ver_app();
}

