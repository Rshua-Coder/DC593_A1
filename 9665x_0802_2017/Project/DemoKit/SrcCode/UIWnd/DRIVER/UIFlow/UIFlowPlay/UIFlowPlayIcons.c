#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "UIFramework.h"
#include "UIFrameworkExt.h"
//#include "UIFlow.h"
#include "JpgEnc.h"
#include "Exif.h"
#include "PlaybackTsk.h"
#include "Utility.h"
#include "audio.h"
#include "SMediaPlayAPI.h"
#include "UIFlowWndPlay.h"
#include "UIPlayObj.h"
#include "UIInfo.h"
#include "UIResource.h"
#include "DxPower.h"
#include "PowerDef.h"
#include "UIFlowPlayFuncs.h"
#include "UIFlow.h"
#include "DpofVendor.h"
#include "ExifVendor.h"
#include "DCF.h"
#include "GxVideoFile.h"

/*
    DSCMode/Flash/EV Icon/Image size/WB/image quality/sharpness
    DZoom/Battery/Histogram/Protect/Date/Time
*/

static void FlowPB_IconDrawDSCMode(BOOL bShow)
{
    UINT32 uiFlag;
    UINT32 uiFileFmt = AppPlay_GetData(PLAY_FILEFMT);

    //hide icon
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_DSCModeCtrl, FALSE);
        return;
    }

    //show icon
    if(uiFileFmt & PBFMT_AVI || uiFileFmt & PBFMT_MOVMJPG)
    {
        uiFlag = ICON_FILE_VIDEO;
    }
    else
    {
        uiFlag = ICON_MODE_PLAYBACK;
    }
    UxStatic_SetData(&UIFlowWndPlay_StaticICN_DSCModeCtrl,STATIC_VALUE,uiFlag);
    UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_DSCModeCtrl, TRUE);
}

static void FlowPB_IconDrawFlash(BOOL bShow)
{
    // Hide icon
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_FlashCtrl, FALSE);
    } else {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_FlashCtrl, TRUE);
    }
}


void FlowPB_IconDrawEVIcon(BOOL bShow)
{
    //PJPGHEAD_DEC_CFG pJpgInfo;
    UINT32           uiFlag;
    //char             *str;
    CHAR str[5];
    EXIF_GETTAG exifTag;
    BOOL bIsLittleEndian;
    UINT32 Data1, Data2;

    // Hide icon
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_EVCtrl,FALSE);
        return;
    }
    //show icon
    //PB_GetParam(PBPRMID_INFO_IMG, (UINT32 *)&pJpgInfo);
    //str = (char *)pJpgInfo->pExifInfo->ExpBiasValue;

    Exif_GetInfo(EXIFINFO_BYTEORDER, &bIsLittleEndian, sizeof(bIsLittleEndian));

    exifTag.uiTagIfd = EXIF_IFD_EXIF;
    exifTag.uiTagId = TAG_ID_EXPOSURE_BIAS_VALUE;
    if(E_OK == Exif_GetTag(&exifTag))
    {
        Data1=Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
        Data2=Get32BitsData(exifTag.uiTagDataAddr+4, bIsLittleEndian);
        ExposureBiasToStr((INT32)Data1, (INT32)Data2,(UINT8 *)str);
    }

    if      (!strncmp(str,"-2.0",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M2P0;
    else if (!strncmp(str,"-5/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M1P6;
    else if (!strncmp(str,"-4/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M1P3;
    else if (!strncmp(str,"-1.0",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M1P0;
    else if (!strncmp(str,"-2/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M0P6;
    else if (!strncmp(str,"-1/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M0P3;
    else if (!strncmp(str,"+1/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P0P3;
    else if (!strncmp(str,"+2/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P0P6;
    else if (!strncmp(str,"+1.0",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P1P0;
    else if (!strncmp(str,"+4/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P1P3;
    else if (!strncmp(str,"+5/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P1P6;
    else if (!strncmp(str,"+2.0",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P2P0;
    else                             uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P0P0;

    UxState_SetData(&UIFlowWndPlay_StatusICN_EVCtrl,STATE_CURITEM,uiFlag);
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_EVCtrl,TRUE);
}

CHAR    gchaFullName[60] = { 0 };
static void FlowPB_IconDrawDCFFileID(BOOL bShow)
{
    static char item1_Buf[32];
#if USE_FILEDB
    UINT32 uiPBFileFmt, rootFolderLength;
#endif


    //hide icon
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_FilenameCtrl, FALSE);
        return;
    }

    //show icon
#if USE_FILEDB
    rootFolderLength = strlen(FILEDB_CARDV_ROOT);
    uiPBFileFmt = AppPlay_GetData(PLAY_FILEFMT);
    PB_GetParam(PBPRMID_CURR_FILEPATH, (UINT32 *)&gchaFullName);

    //show icon

    if (uiPBFileFmt & PBFMT_AVI ||
        uiPBFileFmt & PBFMT_MOVMJPG ||
        uiPBFileFmt & PBFMT_MP4)
    {

        if (uiPBFileFmt & PBFMT_READONLY)
         {
            strncpy(item1_Buf, (gchaFullName+rootFolderLength+10+3),20);
            item1_Buf[20] = '\0';
         } else {
            strncpy(item1_Buf, (gchaFullName+rootFolderLength+10),20);
            item1_Buf[20] = '\0';
         }
    } else {
        strncpy(item1_Buf, (gchaFullName+rootFolderLength+10),20);
        item1_Buf[20] = '\0';
    }
#else
    snprintf(item1_Buf,32,"%03ld-%04ld",AppPlay_GetData(PLAY_DIRID),AppPlay_GetData(PLAY_FILEID));
#endif

    //debug_msg("^GgchaFullName:%s,item1_Buf:%s\r\n",gchaFullName,item1_Buf);
    UxStatic_SetData(&UIFlowWndPlay_StaticTXT_FilenameCtrl,STATIC_VALUE,Txt_Pointer(item1_Buf));
    UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_FilenameCtrl, TRUE);
}

static void FlowPB_IconDrawImageSize(BOOL bShow)
{
    GXVIDEO_INFO MovieInfo;
    static char item1_Buf[32];
    UINT32 uiFileFmt = AppPlay_GetData(PLAY_FILEFMT);

    //hide icon
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_SizeCtrl, FALSE);
        return;
    }
    //show icon
    if(uiFileFmt & PBFMT_MP4 || uiFileFmt & PBFMT_AVI || uiFileFmt & PBFMT_MOVMJPG)
    {
        PB_GetParam(PBPRMID_INFO_VDO, (UINT32 *)&MovieInfo);
        switch (MovieInfo.uiVidWidth)
        {
        case 1920:
            snprintf(item1_Buf,32,"1080FHD");
            break;
        case 1440:
            snprintf(item1_Buf,32,"1080P");
            break;
        case 1280:
            snprintf(item1_Buf,32,"720P");
            break;
        case 848:
            snprintf(item1_Buf,32,"WVGA");
            break;
        case 640:
            snprintf(item1_Buf,32,"VGA");
            break;
        case 320:
            snprintf(item1_Buf,32,"QVGA");
            break;
        default:
            snprintf(item1_Buf,32,"%lux%lu",AppPlay_GetData(PLAY_IMGWIDTH_ORI),AppPlay_GetData(PLAY_IMGHEIGHT_ORI));
        }
    }
    else
    {
        snprintf(item1_Buf,32,"%lux%lu",AppPlay_GetData(PLAY_IMGWIDTH_ORI),AppPlay_GetData(PLAY_IMGHEIGHT_ORI));
    }


    UxStatic_SetData(&UIFlowWndPlay_StaticTXT_SizeCtrl,STATIC_VALUE,Txt_Pointer(item1_Buf));
    UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_SizeCtrl, TRUE);
}


static void FlowPB_IconDrawWB(BOOL bShow)
{
    //PJPGHEAD_DEC_CFG pJpgInfo;
    UINT32        uiFlag;

    //hide icon
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_WBCtrl, FALSE);
        return;
    }
    //show icon
    //PB_GetParam(PBPRMID_INFO_IMG, (UINT32 *)&pJpgInfo);
    switch (AppPlay_GetData(PLAY_EXIF_WB))
    {
    case AutoWhite:
    case ManualWhite:
        //Todo: implement all kind of manual white
    default:
        uiFlag = UIFlowWndPlay_StatusICN_WB_ICON_WB_AUTO;
        break;
    }

    UxState_SetData(&UIFlowWndPlay_StatusICN_WBCtrl,STATE_CURITEM,uiFlag);
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_WBCtrl, TRUE);
}


static void FlowPB_IconDrawImageQuality(BOOL bShow)
{
    UINT32 uiFlag;

    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_QualityCtrl, FALSE);
        return;
    }
    switch(ExifVendor_GetData(EXIFVENDOR_QUALITY))
    {
    case QUALITY_SUPER:
        uiFlag = UIFlowWndPlay_StatusICN_Quality_ICON_QUALITY_FINE;
        break;
    case QUALITY_FINE:
        uiFlag = UIFlowWndPlay_StatusICN_Quality_ICON_QUALITY_NORMAL;
        break;
    case QUALITY_NORMAL:
    default:
        uiFlag = UIFlowWndPlay_StatusICN_Quality_ICON_QUALITY_BASIC;
        break;
    }

    UxState_SetData(&UIFlowWndPlay_StatusICN_QualityCtrl,STATE_CURITEM,uiFlag);
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_QualityCtrl, TRUE);
}

static void FlowPB_IconDrawSharpness(BOOL bShow)
{
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_SharpnessCtrl, FALSE);
        return;
    }
}

static void FlowPB_IconDrawStorage(BOOL bShow)
{
    if (bShow==FALSE)
    {
       UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_StorageCtrl, FALSE);
       return;
    }
    switch(UI_GetData(FL_CardStatus))
    {
    case CARD_REMOVED:
        UxState_SetData(&UIFlowWndPlay_StatusICN_StorageCtrl,STATE_CURITEM,UIFlowWndPlay_StatusICN_Storage_ICON_INTERNAL_FLASH);
        break;
    default:
        UxState_SetData(&UIFlowWndPlay_StatusICN_StorageCtrl,STATE_CURITEM,UIFlowWndPlay_StatusICN_Storage_ICON_SD_CARD);
        break;
    }

    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_StorageCtrl, TRUE);
}

static void FlowPB_IconDrawBattery(BOOL bShow)
{

    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_BatteryCtrl, FALSE);
        return;
    }

    UxState_SetData(&UIFlowWndPlay_StatusICN_BatteryCtrl,STATE_CURITEM,GetBatteryLevel());
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_BatteryCtrl, TRUE);
}


static void FlowPB_IconDrawHistogram(BOOL bShow)
{
    if (bShow==TRUE)
        UxCtrl_SetShow(&UIFlowWndPlay_HistoCtrl, TRUE);
    else
        UxCtrl_SetShow(&UIFlowWndPlay_HistoCtrl, FALSE);
}


static void FlowPB_IconDrawFileLock(BOOL bShow)
{
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_ProtectCtrl, FALSE);
        return;
    }

    if (AppPlay_GetData(PLAY_LOCKSTATUS))
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_ProtectCtrl, TRUE);
    }
    else
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_ProtectCtrl, FALSE);
    }
}

void FlowPB_IconDrawMovPlayVolumn(UINT32 uiVolumn)
{
    UINT32 uiFlag;

    switch(uiVolumn)
    {
    case AUDIO_VOL_MUTE:
        uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_0;
        break;
    case AUDIO_VOL_0:
        uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_1;
        break;
    case AUDIO_VOL_1:
        uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_2;
        break;
    case AUDIO_VOL_2:
		uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_2;	
        //uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_3;
        break;
    case AUDIO_VOL_3:
		uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_2;	
        //uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_4;
        break;
    case AUDIO_VOL_4:
		uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_2;	
        //uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_5;
        break;
    case AUDIO_VOL_5:
		uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_2;	
        //uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_6;
        break;
    case AUDIO_VOL_6:
		uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_2;	
        //uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_7;
        break;
    case AUDIO_VOL_7:
		uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_2;	
        //uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_8;
        break;
    default:
        uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_0;
        break;
    }

    UxState_SetData(&UIFlowWndPlay_StatusICN_VolumnCtrl,STATE_CURITEM,uiFlag);
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_VolumnCtrl, TRUE);
}

void FlowPB_IconHideMovPlayVolumn(void)
{
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_VolumnCtrl, FALSE);
}

void FlowPB_IconDrawMovBwd(BOOL bShow)
{
    if (bShow==TRUE)
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovBwdCtrl, TRUE);
    else
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovBwdCtrl, FALSE);
}

void FlowPB_IconDrawMovFwd(BOOL bShow)
{
    if (bShow==TRUE)
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovFwdCtrl, TRUE);
    else
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovFwdCtrl, FALSE);
}

void FlowPB_IconDrawPrint(BOOL bShow)
{
    if (bShow == FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_PrintCtrl, FALSE);
    } else {
        if (DpofVendor_GetDPOFInfo(DPOF_INFO_CURR_QTY))
        {
            UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_PrintCtrl, TRUE);
        }
        else
        {
            UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_PrintCtrl, FALSE);
        }
    }
}

void FlowPB_IconDrawDate(BOOL bShow)
{
    UINT32  creDateTime[6],modDateTime[6];
    //UINT32  uiDirNum,uiFileNum,uiFileType,index;
//    CHAR    chaFullName[34] = { 0 };
    static char item1_Buf[34];

    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_DateCtrl, FALSE);
        return;
    }

    FileSys_WaitFinish();
#if 0
    uiDirNum = AppPlay_GetData(PLAY_DIRID);
    uiFileNum = AppPlay_GetData(PLAY_FILEID);
    index = DCF_GetIndexByID(uiDirNum,uiFileNum);
    DCF_GetObjInfo(index, &uiDirNum, &uiFileNum, &uiFileType);
    DCF_GetObjPath(index,uiFileType,chaFullName);
#endif
    PB_GetParam(PBPRMID_CURR_FILEPATH, (UINT32 *)&gchaFullName);
    FileSys_GetDateTime(gchaFullName,creDateTime,modDateTime);
    modDateTime[0] %= 100;

    if (modDateTime[0] > 50)  modDateTime[0] = 50;
    if (modDateTime[1] > 12)  modDateTime[0] = 12;
    if (modDateTime[2] > 31)  modDateTime[0] = 31;
    snprintf(item1_Buf,20,"%02lu/%02lu/%02lu",modDateTime[0],modDateTime[1],modDateTime[2]);

    UxStatic_SetData(&UIFlowWndPlay_StaticTXT_DateCtrl,STATIC_VALUE,Txt_Pointer(item1_Buf));
    UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_DateCtrl, TRUE);
}


static void FlowPB_IconDrawTime(BOOL bShow)
{
    UINT32  creDateTime[6],modDateTime[6];
    //UINT32  uiDirNum,uiFileNum,uiFileType,index;
//    CHAR    chaFullName[34] = { 0 };
    static char item1_Buf[34];

    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_TimeCtrl, FALSE);
        return;
    }

    FileSys_WaitFinish();
#if 0
    uiDirNum = AppPlay_GetData(PLAY_DIRID);
    uiFileNum = AppPlay_GetData(PLAY_FILEID);
    index = DCF_GetIndexByID(uiDirNum,uiFileNum);
    DCF_GetObjInfo(index, &uiDirNum, &uiFileNum, &uiFileType);
    DCF_GetObjPath(index,uiFileType,chaFullName);
#endif
    PB_GetParam(PBPRMID_CURR_FILEPATH, (UINT32 *)&gchaFullName);
    FileSys_GetDateTime(gchaFullName,creDateTime,modDateTime);

    if (modDateTime[3]>23)   modDateTime[3]=0;
    if (modDateTime[4]>59)   modDateTime[4]=0;

    snprintf(item1_Buf,20,"%02lu:%02lu",modDateTime[3],modDateTime[4]);

    UxStatic_SetData(&UIFlowWndPlay_StaticTXT_TimeCtrl,STATIC_VALUE,Txt_Pointer(item1_Buf));
    UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_TimeCtrl, TRUE);
}


static void FlowPB_IconDrawThumbSeq(BOOL bShow)
{
    static char item1_Buf[32];
    UINT32  *pThumbSeqID;

    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlayThumb_ThumbIDCtrl, FALSE);
        return;
    }
    pThumbSeqID = (UINT32  *)AppPlay_GetData(PLAY_ALLTHUMBSEQ);

    snprintf(item1_Buf,32,"%lu/%lu",*(pThumbSeqID + AppPlay_GetData(PLAY_BROWSEINDEX) - 1),AppPlay_GetData(PLAY_FILENUM));

    UxStatic_SetData(&UIFlowWndPlayThumb_ThumbIDCtrl,STATIC_VALUE,Txt_Pointer(item1_Buf));
    UxCtrl_SetShow(&UIFlowWndPlayThumb_ThumbIDCtrl, TRUE);
}


void FlowPB_IconDrawMovPlay(BOOL bShow)
{
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovPlayCtrl, FALSE);
        return;
    }
    if(g_PlbData.State == PLB_ST_PLAY_MOV)
    {
        UxState_SetData(&UIFlowWndPlay_StatusICN_MovPlayCtrl,STATE_CURITEM,UIFlowWndPlay_StatusICN_MovPlay_ICON_PLAY_PAUSE);
    }
    else
    {
        UxState_SetData(&UIFlowWndPlay_StatusICN_MovPlayCtrl,STATE_CURITEM,UIFlowWndPlay_StatusICN_MovPlay_ICON_PLAY_PLAY);
    }
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovPlayCtrl, TRUE);
}

static void FlowPB_IconDrawMovStop(BOOL bShow)
{
    if (bShow==TRUE)
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovStopCtrl, TRUE);
    else
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovStopCtrl, FALSE);
}

//void FlowPB_IconDrawMovSpeed(UINT16 iVideoSpeedStep)
void FlowPB_IconDrawMovSpeed(void)
{
  static CHAR VideoSpeedStr[10];
  BOOL bShow = TRUE;

     switch(g_PlbData.VideoPBSpeed) {
        case PLB_FWD_MOV_2x:
              sprintf(VideoSpeedStr,"2X");
            break;
        case PLB_FWD_MOV_4x:
              sprintf(VideoSpeedStr,"4X");
            break;
        case PLB_FWD_MOV_8x:
              sprintf(VideoSpeedStr,"8X");
            break;
        case PLB_BWD_MOV_2x:
              sprintf(VideoSpeedStr,"-2X");
            break;
        case PLB_BWD_MOV_4x:
              sprintf(VideoSpeedStr,"-4X");
            break;
        case PLB_BWD_MOV_8x:
              sprintf(VideoSpeedStr,"-8X");
            break;
        default:
            break;
     }

     if (g_PlbData.VideoPBSpeed==PLB_FWD_MOV_1x)
     {
        bShow = FALSE;
     }

     UxStatic_SetData(&UIFlowWndPlay_StaticTXT_SpeedCtrl,STATIC_VALUE,Txt_Pointer(VideoSpeedStr));
     UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_SpeedCtrl, bShow);
}

void FlowPB_IconHideMovSpeed(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPB_IconDrawMovPlayTime(BOOL bShow)
{
    static char item1_Buf[32];
    UINT32 uiPlayTimeSecs;

    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_MovPlayTimeCtrl, FALSE);
        return;
    }
    uiPlayTimeSecs = SMediaPlay_GetCurrPlaySecs();

    snprintf(item1_Buf, 32, "%02ld:%02ld:%02ld", uiPlayTimeSecs/3600, (uiPlayTimeSecs%3600)/60, (uiPlayTimeSecs%3600)%60);

    UxStatic_SetData(&UIFlowWndPlay_StaticTXT_MovPlayTimeCtrl,STATIC_VALUE,Txt_Pointer(item1_Buf));
	//UxCtrl_SetDirty(&UIFlowWndPlay_StaticTXT_MovPlayTimeCtrl,TRUE);
	UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_MovPlayTimeCtrl, FALSE);
    UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_MovPlayTimeCtrl, TRUE);
}

void FlowPB_UpdateIcons(BOOL bShow)
{
    if (bShow == FALSE)
    {
        FlowPB_IconDrawDSCMode(FALSE);
        FlowPB_IconDrawFlash(FALSE);
        FlowPB_IconDrawEVIcon(FALSE);
        FlowPB_IconDrawDCFFileID(FALSE);
        FlowPB_IconDrawImageSize(FALSE);
        FlowPB_IconDrawWB(FALSE);
        FlowPB_IconDrawImageQuality(FALSE);
        FlowPB_IconDrawSharpness(FALSE);
        FlowPB_IconDrawStorage(FALSE);
        FlowPB_IconDrawBattery(FALSE);
        FlowPB_IconDrawHistogram(FALSE);
        FlowPB_IconDrawFileLock(FALSE);
        FlowPB_IconDrawDate(FALSE);
        FlowPB_IconDrawPrint(FALSE);
        FlowPB_IconHideMovPlayVolumn();
        FlowPB_IconDrawTime(FALSE);
        FlowPB_IconDrawMovPlay(FALSE);
        FlowPB_IconDrawMovStop(FALSE);
        FlowPB_IconDrawMovPlayTime(FALSE);
        FlowPB_IconHideMovSpeed(&UIFlowWndPlay_StaticTXT_SpeedCtrl);
    }
    else
    {
        FlowPB_IconDrawDSCMode(TRUE);
        FlowPB_IconDrawFlash(FALSE);
        FlowPB_IconDrawEVIcon(FALSE);
        FlowPB_IconDrawDCFFileID(TRUE);
        FlowPB_IconDrawImageSize(TRUE);
        FlowPB_IconDrawWB(FALSE);
        FlowPB_IconDrawImageQuality(FALSE);
        FlowPB_IconDrawSharpness(FALSE);
        FlowPB_IconDrawStorage(FALSE);
        FlowPB_IconDrawBattery(TRUE);
        FlowPB_IconDrawHistogram(FALSE);
        FlowPB_IconDrawFileLock(TRUE);
        FlowPB_IconDrawPrint(TRUE);
        FlowPB_IconHideMovPlayVolumn();
        FlowPB_IconHideMovSpeed(&UIFlowWndPlay_StaticTXT_SpeedCtrl);
        FlowPB_IconDrawDate(TRUE);
        FlowPB_IconDrawTime(TRUE);
        FlowPB_IconDrawMovPlayTime(FALSE);
        if (AppPlay_GetData(PLAY_CURRMODE) == PLAYMODE_PRIMARY)
        {
            FlowPB_IconDrawMovPlay(FALSE);
            FlowPB_IconDrawMovStop(FALSE);
            FlowPB_IconDrawMovBwd(FALSE);
            FlowPB_IconDrawMovFwd(FALSE);
        }
        else
        {
            FlowPB_IconDrawMovPlay(TRUE);
            FlowPB_IconDrawMovStop(TRUE);
            FlowPB_IconDrawMovBwd(TRUE);
            FlowPB_IconDrawMovFwd(TRUE);
        }
    }
}

//#NT#2012/7/4#Philex - begin
void FlowPB_DrawThumbRect(BOOL bShow)
{
    switch(AppPlay_GetData(PLAY_BROWSEINDEX))
    {
    //Playback fileIndex starts from 1.
    case 1:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel0Ctrl,bShow);
        break;
    case 2:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel1Ctrl,bShow);
        break;
    case 3:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel2Ctrl,bShow);
        break;
    case 4:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel3Ctrl,bShow);
        break;
    case 5:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel4Ctrl,bShow);
        break;
    case 6:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel5Ctrl,bShow);
        break;
    case 7:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel6Ctrl,bShow);
        break;
    case 8:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel7Ctrl,bShow);
        break;
    case 9:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel8Ctrl,bShow);
        break;
    }
}


void FlowPB_ClearAllThumbIcon(void)
{
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel0Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel1Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel2Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel3Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel4Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel5Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel6Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel7Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel8Ctrl,FALSE);

    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect0Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect1Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect2Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect3Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect4Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect5Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect6Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect7Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect8Ctrl,FALSE);

    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm0Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm1Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm2Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm3Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm4Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm5Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm6Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm7Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm8Ctrl,FALSE);

    UxCtrl_SetShow(&UIFlowWndPlayThumb_ThumbIDCtrl,FALSE);
}


void FlowPB_ShowAllThumbIcon(void)
{
    UINT32  i, loopcnts,CurrFileFormat ;
    UINT16  *pCurrFileFormat;   // Hideo@0503: variable type should be UINT16

    FlowPB_DrawThumbRect(TRUE);

    FlowPB_IconDrawThumbSeq(TRUE);

    pCurrFileFormat = (UINT16 *)AppPlay_GetData(PLAY_ALLTHUMBFMT);
    loopcnts = AppPlay_GetData(PLAY_CURRTHUMBNUM);

    for (i=0; i<loopcnts; i++)
    {
        CurrFileFormat  = *pCurrFileFormat++;

        if (CurrFileFormat & PBFMT_READONLY)
        {
            switch(i)
            {
            case 0:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect0Ctrl,TRUE);
                break;
            case 1:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect1Ctrl,TRUE);
                break;
            case 2:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect2Ctrl,TRUE);
                break;
            case 3:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect3Ctrl,TRUE);
                break;
            case 4:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect4Ctrl,TRUE);
                break;
            case 5:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect5Ctrl,TRUE);
                break;
            case 6:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect6Ctrl,TRUE);
                break;
            case 7:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect7Ctrl,TRUE);
                break;
            case 8:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect8Ctrl,TRUE);
                break;
            }
        }

        if (CurrFileFormat & (PBFMT_AVI|PBFMT_ASF|PBFMT_MOVMJPG|PBFMT_MP4))
        {
            switch(i)
            {
            case 0:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm0Ctrl,TRUE);
                break;
            case 1:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm1Ctrl,TRUE);
                break;
            case 2:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm2Ctrl,TRUE);
                break;
            case 3:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm3Ctrl,TRUE);
                break;
            case 4:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm4Ctrl,TRUE);
                break;
            case 5:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm5Ctrl,TRUE);
                break;
            case 6:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm6Ctrl,TRUE);
                break;
            case 7:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm7Ctrl,TRUE);
                break;
            case 8:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm8Ctrl,TRUE);
                break;
            }
        }
    }
}



