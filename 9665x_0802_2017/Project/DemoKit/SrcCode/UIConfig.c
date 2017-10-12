/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalibrationTsk.c
    @ingroup    mIPRJAPTest

    @brief      Calibration task API
                Calibration task API

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#include "FileSysTsk.h"
#include "GxInput.h"
#include "Delay.h"
#include "SysMain.h"
#include "UIConfig.h"
#include "PStore.h"
#include "Type.h"
#include "UIFlow.h"
//#include "CalibrationTsk.h"
#include "UICfgDefault.h"
#include "UIInfo.h"


#define KEY_LEN         (0X20)
#define VALUE_LEN       (0X30)
#define KEY_VALUE_LEN   (0X50)

#define SYSPARAM_LOGO_LEN    (0XF000)
#define SYSPARAM_SYSFLAG_LEN (0X1000)
#define LOGO_MAX_FILESIZE   (SYSPARAM_LOGO_LEN-4)
#define CFG_MAX_FILESIZE    (SYSPARAM_SYSFLAG_LEN-4)

#define SPI_SYSFLAG_PARAM_MAGIC     0x9582AB15

#if 1
#define USE_MSG(msg)    debug_msg msg
#else
#define USE_MSG(msg)    debug_ind (msg)
#endif

static BOOL get_key_value (char *cfgbuf,char *flgbuf);

/*--- Setup Default value---*/
static UIConfigInfo gUIConfigInfo;


UINT32 FLG_ID_CALIBRATION = 0;


char *pFwVersion = "CarDV.20130321.01";
char *pMakerString = "Novatek";
char *pModelString = "DSC Turnkey";


void Calibration_InstallID(void)
{
    OS_CONFIG_FLAG(FLG_ID_CALIBRATION);
}

CHAR *GetMakerString(void)
{
    return pMakerString;
}

CHAR *GetModelString(void)
{
    return pModelString;
}

CHAR *GetVersionString(void)
{
    return pFwVersion;
}

CHAR *GetUIConfigSoftwareVer(void)
{
    return gUIConfigInfo.strSoftwareVer;
}
CHAR *GetUIConfigMakerString(void)
{
    return gUIConfigInfo.strMakerString;
}
CHAR *GetUIConfigModelString(void)
{
    return gUIConfigInfo.strModelString;
}
CHAR *GetUIConfigMakeNote(void)
{
    return gUIConfigInfo.strMakeNote;
}
CHAR *GetUIConfigImageDescription(void)
{
    return gUIConfigInfo.strImageDescription;
}
void UI_SetSizeDefaultIndex(UINT32 index)
{
    gUIConfigInfo.uiSizeDefaultIndex = index;
}
UINT32 UI_GetSizeDefaultIndex(void)
{
    if (gUIConfigInfo.uiSizeDefaultIndex >= PHOTO_SIZE_ID_MAX)
    {
        gUIConfigInfo.uiSizeDefaultIndex = 0;
    }
    return gUIConfigInfo.uiSizeDefaultIndex;
}
void UI_SetMovieSizeDefaultIndex(UINT32 index)
{
    gUIConfigInfo.uiMovieSizeDefaultIndex = index;
}
UINT32 UI_GetMovieSizeDefaultIndex(void)
{
    if (gUIConfigInfo.uiMovieSizeDefaultIndex >= MOVIE_SIZE_ID_MAX)
    {
        gUIConfigInfo.uiMovieSizeDefaultIndex = 0;
    }
    return gUIConfigInfo.uiMovieSizeDefaultIndex;
}

void UI_SetLanguageDefaultIndex(UINT32 index)
{
    gUIConfigInfo.uiLanguageDefaultIndex = index;
}
UINT32 UI_GetLanguageDefaultIndex(void)
{
    if (gUIConfigInfo.uiLanguageDefaultIndex >= LANG_ID_MAX)
    {
        gUIConfigInfo.uiLanguageDefaultIndex = 0;
    }
    return gUIConfigInfo.uiLanguageDefaultIndex;
}
void UI_SetFreqDefaultIndex(UINT32 index)
{
    gUIConfigInfo.uiFreqDefaultIndex = index;
}
UINT32 UI_GetFreqDefaultIndex(void)
{
    if (gUIConfigInfo.uiFreqDefaultIndex >= FREQUENCY_ID_MAX)
    {
        gUIConfigInfo.uiFreqDefaultIndex = 0;
    }
    return gUIConfigInfo.uiFreqDefaultIndex;
}
void UI_SetTVModeDefaultIndex(UINT32 index)
{
    gUIConfigInfo.uiTVModeDefaultIndex = index;
}
UINT32 UI_GetTVModeDefaultIndex(void)
{
    if (gUIConfigInfo.uiTVModeDefaultIndex >= TV_MODE_ID_MAX)
    {
        gUIConfigInfo.uiTVModeDefaultIndex = 0;
    }
    return gUIConfigInfo.uiTVModeDefaultIndex;
}
void UI_SetDateImprintDefaultIndex(UINT32 index)
{
    gUIConfigInfo.uiDateImprintDefaultIndex = index;
}
UINT32 UI_GetDateImprintDefaultIndex(void)
{
    if (gUIConfigInfo.uiDateImprintDefaultIndex >= (MOVIE_DATEIMPRINT_ID_MAX))
    {
        gUIConfigInfo.uiDateImprintDefaultIndex = 0;
    }
    return gUIConfigInfo.uiDateImprintDefaultIndex;
}
void UI_SetBeepDefaultIndex(UINT32 index)
{
    gUIConfigInfo.uiBeepDefaultIndex = index;
}
UINT32 UI_GetBeepDefaultIndex(void)
{
    if (gUIConfigInfo.uiBeepDefaultIndex >= (BEEP_ID_MAX))
    {
        gUIConfigInfo.uiBeepDefaultIndex = 0;
    }
    return gUIConfigInfo.uiBeepDefaultIndex;
}
void UI_SetVideoAudioDefaultIndex(UINT32 index)
{
    gUIConfigInfo.uiVideoAudioDefaultIndex = index;
}
UINT32 UI_GetVideoAudioDefaultIndex(void)
{
    if (gUIConfigInfo.uiVideoAudioDefaultIndex >= (MOVIE_AUDIO_ID_MAX))
    {
        gUIConfigInfo.uiVideoAudioDefaultIndex = 0;
    }
    return gUIConfigInfo.uiVideoAudioDefaultIndex;
}


BOOL UI_LoadCfgFile(void)
{
    ER erReturn;
    UINT32 i;

    char *SysParam_Sector;
    UINT32      uiFileSize;
    UINT8       *BgBuf;
    PSTORE_SECTION_HANDLE* pSecHdl;


    strcpy(gUIConfigInfo.strMakerString, (CHAR *)GetMakerString());
    strcpy(gUIConfigInfo.strModelString,(CHAR *)GetModelString());
    strcpy(gUIConfigInfo.strSoftwareVer,(CHAR *)GetVersionString());

    gUIConfigInfo.uiSizeDefaultIndex        = DEFAULT_PHOTO_SIZE;
    gUIConfigInfo.uiMovieSizeDefaultIndex   = DEFAULT_MOVIE_SIZE;
    gUIConfigInfo.uiLanguageDefaultIndex    = DEFAULT_LANGUAGE;
    gUIConfigInfo.uiFreqDefaultIndex        = DEFAULT_FREQUENCY;
    gUIConfigInfo.uiTVModeDefaultIndex      = DEFAULT_TV_MODE;
    gUIConfigInfo.uiDateImprintDefaultIndex = DEFAULT_MOVIE_DATEIMPRINT;
    gUIConfigInfo.uiBeepDefaultIndex        = DEFAULT_BEEP;
    gUIConfigInfo.uiVideoAudioDefaultIndex  = DEFAULT_MOVIE_AUDIO;


    BgBuf = (UINT8 *)OS_GetMempoolAddr(POOL_ID_APP);
    //PStore_EnablePS();
    pSecHdl = PStore_OpenSection(PS_BG_CFG, PS_RDONLY);

    if (pSecHdl == E_PS_SECHDLER)
    {
        debug_err(("UserPS_ReadCfgFile: Section open fail\r\n"));
        goto err_ret;
    }

    erReturn = PStore_ReadSection(BgBuf, 0, 4, pSecHdl);

    if (erReturn != E_PS_OK)
    {
        debug_err(("UserPS_ReadCfgFile: PStore read error\r\n"));
        goto err_ret;
    }

    uiFileSize = MAKE_LONG(MAKE_WORD(BgBuf[0],BgBuf[1]),MAKE_WORD(BgBuf[2],BgBuf[3]));

    USE_MSG(("UserPS_ReadCfgFile: cfg filesize=%d\r\n", uiFileSize));

    if (uiFileSize > CFG_MAX_FILESIZE)
    {
        debug_err(("UserPS_ReadCfgFile: file size error\n\r"));
        goto err_ret;
    }

    erReturn = PStore_ReadSection(BgBuf, 0, (uiFileSize+4), pSecHdl);
    if (erReturn != E_PS_OK)
    {
        debug_err(("UserPS_ReadCfgFile: PStore read error\r\n"));
        goto err_ret;
    }
    PStore_CloseSection(pSecHdl);
    //PStore_DisablePS();

    SysParam_Sector = (char *)(BgBuf + ((uiFileSize+4+4-1)/4)*4);

    memset((UINT8 *)SysParam_Sector,0,SYSPARAM_SYSFLAG_LEN);
    if (!get_key_value((char *)&BgBuf[4],(char*)SysParam_Sector))
    {
        debug_err(("UserPS_ReadCfgFile: Read cfg file error\r\n"));
        return FALSE;//goto err_ret;
    }

    i = 0;
    while(TRUE)
    {
        if (SysParam_Sector[0] == '\0')
        {
             debug_err(("sysflag is null\r\n"));
             break;
        }
        if (i*KEY_VALUE_LEN >= SYSPARAM_SYSFLAG_LEN)
        {
            break;
        }
        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "beep"))
        {
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "yes"))
            {
                USE_MSG(("beep yes\r\n"));
                UI_SetBeepDefaultIndex(BEEP_ON);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "no"))
            {
                USE_MSG(("beep no\r\n"));
                UI_SetBeepDefaultIndex(BEEP_OFF);
            }
        }

        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "videoaudio"))
        {
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "yes"))
            {
                USE_MSG(("videoaudio yes\r\n"));
                UI_SetVideoAudioDefaultIndex(MOVIE_AUDIO_ON);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "no"))
            {
                USE_MSG(("videoaudio no\r\n"));
                UI_SetVideoAudioDefaultIndex(MOVIE_AUDIO_OFF);
            }
        }

        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "hz"))
        {
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "50hz"))
            {
                USE_MSG(("50hz\r\n"));
                UI_SetFreqDefaultIndex(FREQUENCY_50HZ);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "60hz"))
            {
                USE_MSG(("60hz\r\n"));
                UI_SetFreqDefaultIndex(FREQUENCY_60HZ);
            }
        }
        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "tvmode"))
        {
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "pal"))
            {
                USE_MSG(("tvmode pal\r\n"));
                UI_SetTVModeDefaultIndex(TV_MODE_PAL);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "ntsc"))
            {
                USE_MSG(("tvmode  ntsc\r\n"));
                UI_SetTVModeDefaultIndex(TV_MODE_NTSC);
            }
        }
        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "date"))
        {
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "yes"))
            {
                USE_MSG(("date_time on\r\n"));
                UI_SetDateImprintDefaultIndex(DATE_TIME_ON);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "no"))
            {
                USE_MSG(("date_time off\r\n"));
                UI_SetDateImprintDefaultIndex(DATE_TIME_OFF);
            }
        }
        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "moviesize"))
        {
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "1080p"))
            {
                USE_MSG(("moviesize-1080p\r\n"));
                gTM_OPTIONS_MOVIE_SIZE[MOVIE_SIZE_1080P].TextId = IDS_MOVIE_SIZE_1080FHD;
                gTM_OPTIONS_MOVIE_SIZE[MOVIE_SIZE_1080P].IconId = IDI_MOVIE_SIZE_1080FHD;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "720p"))
            {
                USE_MSG(("moviesize-720p\r\n"));
                gTM_OPTIONS_MOVIE_SIZE[MOVIE_SIZE_720P].TextId = IDS_MOVIE_SIZE_720P;
                gTM_OPTIONS_MOVIE_SIZE[MOVIE_SIZE_720P].IconId = IDI_MOVIE_SIZE_720P;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "wvga"))
            {
                USE_MSG(("moviesize-wvga\r\n"));
                gTM_OPTIONS_MOVIE_SIZE[MOVIE_SIZE_WVGA].TextId = IDS_MOVIE_SIZE_WVGA;
                gTM_OPTIONS_MOVIE_SIZE[MOVIE_SIZE_WVGA].IconId = IDI_MOVIE_SIZE_WVGA;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "vga"))
            {
                USE_MSG(("moviesize-vga\r\n"));
                gTM_OPTIONS_MOVIE_SIZE[MOVIE_SIZE_VGA].TextId = IDS_MOVIE_SIZE_VGA;
                gTM_OPTIONS_MOVIE_SIZE[MOVIE_SIZE_VGA].IconId = IDI_MOVIE_SIZE_VGA;
            }
        }
        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "photosize"))
        {
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "12m"))
            {
                USE_MSG(("photosize-12m\r\n"));
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_12M].TextId = IDS_PHOTO_SIZE_12M;
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_12M].IconId = IDI_PHOTO_SIZE_12M;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "10m"))
            {
                USE_MSG(("photosize-10m\r\n"));
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_10M].TextId = IDS_PHOTO_SIZE_10M;
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_10M].IconId = IDI_PHOTO_SIZE_10M;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "8m"))
            {
                USE_MSG(("photosize-8m\r\n"));
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_8M].TextId = IDS_PHOTO_SIZE_8M;
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_8M].IconId = IDI_PHOTO_SIZE_8M;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "5m"))
            {
                USE_MSG(("photosize-5m\r\n"));
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_5M].TextId = IDS_PHOTO_SIZE_5M;
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_5M].IconId = IDI_PHOTO_SIZE_5M;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "3m"))
            {
                USE_MSG(("photosize-3m\r\n"));
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_3M].TextId = IDS_PHOTO_SIZE_3M;
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_3M].IconId = IDI_PHOTO_SIZE_3M;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "2m169"))
            {
                USE_MSG(("photosize-2m9\r\n"));
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_2MHD].TextId = IDS_PHOTO_SIZE_2MHD;
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_2MHD].IconId = IDI_PHOTO_SIZE_2MHD;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "vga"))
            {
                USE_MSG(("photosize-vgam\r\n"));
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_VGA].TextId = IDS_PHOTO_SIZE_VGA;
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_VGA].IconId = IDI_PHOTO_SIZE_VGA;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "1m"))
            {
                USE_MSG(("photosize-1m\r\n"));
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_1M].TextId = IDS_PHOTO_SIZE_1M;
                gTM_OPTIONS_PHOTO_SIZE[PHOTO_SIZE_1M].IconId = IDI_PHOTO_SIZE_1M;
            }
        }
        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "defaultmoviesize"))
        {
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "1080p"))
            {
                USE_MSG(("defaultmoviesize-1080p\r\n"));
                UI_SetMovieSizeDefaultIndex(MOVIE_SIZE_1080P);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "720p"))
            {
                USE_MSG(("defaultmoviesize-720p\r\n"));
                UI_SetMovieSizeDefaultIndex(MOVIE_SIZE_720P);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "wvga"))
            {
                USE_MSG(("defaultmoviesize-wvga\r\n"));
                UI_SetMovieSizeDefaultIndex(MOVIE_SIZE_WVGA);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "vga"))
            {
                USE_MSG(("defaultmoviesize-vga\r\n"));
                UI_SetMovieSizeDefaultIndex(MOVIE_SIZE_VGA);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "qvga"))
            {
                USE_MSG(("defaultmoviesize-qvga\r\n"));
                UI_SetMovieSizeDefaultIndex(MOVIE_SIZE_QVGA);
            }
        }
        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "defaultphotosize"))
        {
            #if (UPDATE_CFG == UPDATE_CFG_YES)
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "vga"))
            {
                USE_MSG(("defaultphotosize-vga\r\n"));
                UI_SetSizeDefaultIndex(PHOTO_SIZE_VGA);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "2m169"))
            {
                USE_MSG(("defaultphotosize-2m169\r\n"));
                UI_SetSizeDefaultIndex(PHOTO_SIZE_2MHD);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "2m"))
            {
                USE_MSG(("defaultphotosize-2m\r\n"));
                UI_SetSizeDefaultIndex(PHOTO_SIZE_2M);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "3m"))
            {
                USE_MSG(("defaultphotosize-3m\r\n"));
                UI_SetSizeDefaultIndex(PHOTO_SIZE_3M);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "5m"))
            {
                USE_MSG(("defaultphotosize-5m\r\n"));
                UI_SetSizeDefaultIndex(PHOTO_SIZE_5M);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "8m"))
            {
                USE_MSG(("defaultphotosize-8m\r\n"));
                UI_SetSizeDefaultIndex(PHOTO_SIZE_8M);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "9m"))
            {
                USE_MSG(("defaultphotosize-9m\r\n"));
                //UI_SetSizeDefaultIndex(PHOTO_SIZE_10M);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "10m"))
            {
                USE_MSG(("defaultphotosize-10m\r\n"));
                UI_SetSizeDefaultIndex(PHOTO_SIZE_10M);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "12m"))
            {
                USE_MSG(("defaultphotosize-12m\r\n"));
                UI_SetSizeDefaultIndex(PHOTO_SIZE_12M);
            }
            #endif
        }

        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "language"))
        {
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "en"))
            {
                USE_MSG(("language-en\r\n"));
                gTM_OPTIONS_LANGUAGE[LANG_EN].TextId = IDS_LANG_EN;
                gTM_OPTIONS_LANGUAGE[LANG_EN].IconId = IDI_LANG_EN;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "de"))
            {
                USE_MSG(("language-de\r\n"));
                gTM_OPTIONS_LANGUAGE[LANG_DE].TextId = IDS_LANG_DE;
                gTM_OPTIONS_LANGUAGE[LANG_DE].IconId = IDI_LANG_DE;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "es"))
            {
                USE_MSG(("language-es\r\n"));
                gTM_OPTIONS_LANGUAGE[LANG_ES].TextId = IDS_LANG_ES;
                gTM_OPTIONS_LANGUAGE[LANG_ES].IconId = IDI_LANG_ES;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "it"))
            {
                USE_MSG(("language-it\r\n"));
                gTM_OPTIONS_LANGUAGE[LANG_IT].TextId = IDS_LANG_IT;
                gTM_OPTIONS_LANGUAGE[LANG_IT].IconId = IDI_LANG_IT;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "ru"))
            {
                USE_MSG(("language-ru\r\n"));
                gTM_OPTIONS_LANGUAGE[LANG_RU].TextId = IDS_LANG_RU;
                gTM_OPTIONS_LANGUAGE[LANG_RU].IconId = IDI_LANG_RU;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "fr"))
            {
                USE_MSG(("language-fr\r\n"));
                gTM_OPTIONS_LANGUAGE[LANG_FR].TextId = IDS_LANG_FR;
                gTM_OPTIONS_LANGUAGE[LANG_FR].IconId = IDI_LANG_FR;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "sc"))
            {
                USE_MSG(("language-sc\r\n"));
                gTM_OPTIONS_LANGUAGE[LANG_SC].TextId = IDS_LANG_SC;
                gTM_OPTIONS_LANGUAGE[LANG_SC].IconId = IDI_LANG_SC;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "tc"))
            {
                USE_MSG(("language-tc\r\n"));
                gTM_OPTIONS_LANGUAGE[LANG_TC].TextId = IDS_LANG_TC;
                gTM_OPTIONS_LANGUAGE[LANG_TC].IconId = IDI_LANG_TC;
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "jp"))
            {
                USE_MSG(("language-jp\r\n"));
                gTM_OPTIONS_LANGUAGE[LANG_JP].TextId = IDS_LANG_JP;
                gTM_OPTIONS_LANGUAGE[LANG_JP].IconId = IDI_LANG_JP;
            }
        }
        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "defaultlanguage"))
        {
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "en"))
            {
                USE_MSG(("defaultlanguage-en\r\n"));
                UI_SetLanguageDefaultIndex(LANG_EN);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "fr"))
            {
                USE_MSG(("defaultlanguage-fr\r\n"));
                UI_SetLanguageDefaultIndex(LANG_FR);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "de"))
            {
                USE_MSG(("defaultlanguage-de\r\n"));
                UI_SetLanguageDefaultIndex(LANG_DE);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "es"))
            {
                USE_MSG(("defaultlanguage-es\r\n"));\
                UI_SetLanguageDefaultIndex(LANG_ES);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "ru"))
            {
                USE_MSG(("defaultlanguage-ru\r\n"));
                UI_SetLanguageDefaultIndex(LANG_RU);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "it"))
            {
                USE_MSG(("defaultlanguage-it\r\n"));
                UI_SetLanguageDefaultIndex(LANG_IT);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "sc"))
            {
                USE_MSG(("defaultlanguage-sc\r\n"));
                UI_SetLanguageDefaultIndex(LANG_SC);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "tc"))
            {
                USE_MSG(("defaultlanguage-tc\r\n"));
                UI_SetLanguageDefaultIndex(LANG_TC);
            }
            if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN, "jp"))
            {
                USE_MSG(("defaultlanguage-jp\r\n"));
                UI_SetLanguageDefaultIndex(LANG_JP);
            }
        }

        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "maker"))
        {

            USE_MSG(("maker:%s\r\n",SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN));
            strcpy(gUIConfigInfo.strMakerString,SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN);
        }

        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "model"))
        {

            USE_MSG(("model:%s\r\n",SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN));
            strcpy(gUIConfigInfo.strModelString,SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN);
        }

        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "version"))
        {

            USE_MSG(("verion:%s\r\n",SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN));
            strcpy(gUIConfigInfo.strSoftwareVer,SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN);
        }
        if (!strcmp(SysParam_Sector+i*KEY_VALUE_LEN, "ImageDescription"))
        {

            USE_MSG(("sImageDescription:%s\r\n",SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN));
            strcpy(gUIConfigInfo.strImageDescription,SysParam_Sector+i*KEY_VALUE_LEN+KEY_LEN);
        }

        i++;
    }

    return TRUE;
err_ret:

    PStore_CloseSection(pSecHdl);
    //PStore_DisablePS();
    return FALSE;
}

#if 0
void UI_UpdateCfgFile(void)
{
#if 0
    UINT32      uiKeyAct, uiKeyCode;
    FLGPTN     uiFlag;
#endif
    char*       str;
    FST_FILE    pFile;
    UINT32      uiFileSize;
    UINT8       *BgBuf;
    char        *fileName;
    URECT       Rect = {0, 0, 320, 240};
    BOOL        Ret  =TRUE ;
    ER          erReturn;

    PSTORE_SECTION_HANDLE* pSecHdl; // to save the handle of PStore section

    fileName = "A:\\NTCONFIG.bin";

    FileSys_WaitFinish();
    pFile = FileSys_OpenFile(fileName, FST_OPEN_READ);

    if (pFile == NULL)
    {
        debug_err(("open file error\r\n"));
        Ret  =FALSE;
    }

    uiFileSize = FileSys_GetFileLen(fileName);  // for small page nand
    USE_MSG(("file size = %d\r\n",uiFileSize));

    //if file is null
    if (uiFileSize == 0)
    {
        USE_MSG(("file size is 0\r\n"));
        Ret  =FALSE;
    }

    if (uiFileSize > CFG_MAX_FILESIZE)
    {
        USE_MSG(("file size is bigger = %d\r\n",uiFileSize));
        Ret  =FALSE;
    }

    //get_blk((void *)&BgBuf,  POOL_ID_SICD);
    //rel_blk(POOL_ID_SICD, BgBuf);
    //SysGetSICDAddr((UINT32*)&BgBuf);
    BgBuf = (UINT8 *)OS_GetMempoolAddr(POOL_ID_APP);
    if (FileSys_ReadFile(pFile,BgBuf,&uiFileSize,FST_FLAG_NONE,NULL)!=FST_STA_OK)
    {
        USE_MSG(("file read failed\r\n"));
        Ret  =FALSE;
    }

    FileSys_CloseFile(pFile);

    *BgBuf = LO_BYTE(LO_WORD(uiFileSize));
    *(BgBuf+1) = HI_BYTE(LO_WORD(uiFileSize));
    *(BgBuf+2) = LO_BYTE(HI_WORD(uiFileSize));
    *(BgBuf+3) = HI_BYTE(HI_WORD(uiFileSize));

    //PStore_EnablePS();
    pSecHdl = PStore_OpenSection(PS_BG_CFG, PS_RDWR | PS_CREATE);

    if (pSecHdl == E_PS_SECHDLER)
    {
        debug_err(("Section open fail\r\n"));
        PStore_CloseSection(pSecHdl);
        //PStore_DisablePS();
        Ret  =FALSE;
    }

    erReturn = PStore_WriteSection(BgBuf, 0, SYSPARAM_SYSFLAG_LEN, pSecHdl);

    PStore_CloseSection(pSecHdl);

    if (erReturn != E_PS_OK)
    {
        debug_err(("PStore program error\r\n"));
        Ret  =FALSE;
    }


    UI_FillRect(&Rect, _OSD_INDEX_BLACK);

    Rect.x   = 56;
    Rect.y   = 108;
    Rect.w   = 212;
    Rect.h   = 24;

    if (Ret  == FALSE)
    {
        str =  "Update Cfg file error";
        UI_ShowStringByColor(str, &Rect, _OSD_INDEX_YELLOW, _OSD_INDEX_BLACK);
    }else
    {
        str = "Update Cfg file ok";
        UI_ShowStringByColor(str, &Rect, _OSD_INDEX_YELLOW, _OSD_INDEX_BLACK);
    }

    Delay_DelayMs(1000);

    if (Ret)
    {
       System_PowerOff(SYS_POWEROFF_NORMAL);
    }

#if 0

    clr_flg(FLG_ID_CALIBRATION, 0xffffffff);
    while (1)
    {
        wai_flg(&uiFlag, FLG_ID_CALIBRATION, 0xffffffff, TWF_ORW | TWF_CLR);

        debug_msg("^GuiFlag:%x\r\n",uiFlag);
        if (uiFlag)
            break;
    }
#else
    Delay_DelayMs(1500);
#endif
}
#endif

BOOL UI_ShowLogoFile(UINT32 logoId,UINT32 *uiJpgAddr,UINT32 *uiJpgSize)
{
    UINT32      uiFileSize;
    UINT8       *BgBuf;
    ER          erReturn;
    PSTORE_SECTION_HANDLE* pSecHdl; // to save the handle of PStore section
    char *psecnameId;


    if (logoId == UI_LOGO_POWERON)
    {
        psecnameId = PS_POWERON_LOGO;
    }
    else //if (logoId == UI_LOGO_POWEROFF)
    {
        psecnameId = PS_POWEROFF_LOGO;
    }

    //get_blk((void *)&BgBuf,  POOL_ID_SICD);
    //rel_blk(POOL_ID_SICD, BgBuf);
    BgBuf = (UINT8 *)OS_GetMempoolAddr(POOL_ID_APP);

    //PStore_EnablePS();
    //pSecHdl = PStore_OpenSection(PS_BG_LOGO, PS_RDONLY);
    pSecHdl = PStore_OpenSection(psecnameId, PS_RDONLY);

    if (pSecHdl == E_PS_SECHDLER)
    {
        debug_err(("Section PS_BG_LOGO open fail\r\n"));
        PStore_CloseSection(pSecHdl);
        //PStore_DisablePS();

        return FALSE;
    }

    erReturn = PStore_ReadSection(BgBuf, 0, 4, pSecHdl);

    if (erReturn != E_PS_OK)
    {
        debug_err(("PStore read error\r\n"));
    }
    uiFileSize = MAKE_LONG(MAKE_WORD(BgBuf[0],BgBuf[1]),MAKE_WORD(BgBuf[2],BgBuf[3]));

    USE_MSG(("UserPS_ShowLogoFile: logo uiFileSize %d\r\n",uiFileSize));
    //check file size logo size:  1k---20k
    if ((uiFileSize < 1024) || (uiFileSize > LOGO_MAX_FILESIZE))
    {
        USE_MSG(("Show logo error\n\r"));
        PStore_CloseSection(pSecHdl);
        //PStore_DisablePS();

        return FALSE;
    }
    erReturn = PStore_ReadSection(BgBuf, 0, (uiFileSize+4), pSecHdl);

    if (erReturn != E_PS_OK)
    {
        debug_err(("PStore read error\r\n"));
        PStore_CloseSection(pSecHdl);
        return FALSE;
    }

    PStore_CloseSection(pSecHdl);

    *uiJpgAddr = (UINT32)(BgBuf+4);
    *uiJpgSize = (UINT32)uiFileSize;

    return TRUE;
}

static BOOL UI_WriteLogoFileName(UINT32 logoId)
{
    FST_FILE    pFile;
    UINT32      uiFileSize;
    UINT8       *BgBuf;
    char        *fileName;
    IRECT       Rect = {0, 0, 320, 240};
    ER          erReturn = E_OK;
    PSTORE_SECTION_HANDLE* pSecHdl; // to save the handle of PStore section
    char *psecnameId;

    if (logoId == UI_LOGO_POWERON)
    {
        fileName = "A:\\logo.jpg";
        psecnameId = PS_POWERON_LOGO;
    }
    else //if (logoId == UI_LOGO_POWEROFF)
    {
        fileName = "A:\\logo2.jpg";
        psecnameId = PS_POWEROFF_LOGO;
    }

    FileSys_WaitFinish();
    pFile = FileSys_OpenFile(fileName, FST_OPEN_READ);

    if (pFile == NULL)
    {
        debug_err(("open file error\r\n"));
        return FALSE;
    }

    uiFileSize = FileSys_GetFileLen(fileName);  // for small page nand

    USE_MSG(("file size = %d\r\n",uiFileSize));

    if ((uiFileSize < 1024) || (uiFileSize > LOGO_MAX_FILESIZE))
    {
        UI_FillRect(&Rect, _OSD_INDEX_BLACK);
        UI_ShowStringByColor("jpg size is too big or too small\n", &Rect, _OSD_INDEX_YELLOW, _OSD_INDEX_BLACK);
        Delay_DelayMs(2000);
        UI_FillRect(&Rect, _OSD_INDEX_BLACK);
        debug_err(("Write logo error\n\r"));
        return FALSE;
    }

    BgBuf = (UINT8 *)OS_GetMempoolAddr(POOL_ID_APP);
    if (FileSys_ReadFile(pFile,(UINT8 *)(BgBuf+4),&uiFileSize,FST_FLAG_NONE,NULL)!=FST_STA_OK)
    {
        USE_MSG(("file read failed\r\n"));
        return FALSE;
    }

    FileSys_CloseFile(pFile);

    *BgBuf = LO_BYTE(LO_WORD(uiFileSize));
    *(BgBuf+1) = HI_BYTE(LO_WORD(uiFileSize));
    *(BgBuf+2) = LO_BYTE(HI_WORD(uiFileSize));
    *(BgBuf+3) = HI_BYTE(HI_WORD(uiFileSize));

    pSecHdl = PStore_OpenSection(psecnameId, PS_RDWR | PS_CREATE);

    if (pSecHdl == E_PS_SECHDLER)
    {
        debug_err(("Section open fail\r\n"));
        goto err_ret;
    }

    erReturn = PStore_WriteSection(BgBuf, 0, SYSPARAM_LOGO_LEN, pSecHdl);

    if (erReturn != E_PS_OK)
    {
        debug_err(("PStore program error\r\n"));
        goto err_ret;
    }
    //#PIC#2010/07/23#Creator -begin
    PStore_CloseSection(pSecHdl);
    //#PIC#2010/07/23#Creator -end
    return TRUE;

err_ret:
    PStore_CloseSection(pSecHdl);
    //PStore_DisablePS();
    return FALSE;
}

void UI_UpdateLogoFile(void)
{
    //UINT32    uiKeyAct, uiKeyCode;
    //FLGPTN    uiFlag;
    IRECT     Rect = {0, 0, 320, 240};
    BOOL      ret, ret2;

    ret  = UI_WriteLogoFileName(UI_LOGO_POWERON);
    ret2 = UI_WriteLogoFileName(UI_LOGO_POWEROFF);

    if (ret == TRUE)
    {
        UI_FillRect(&Rect, _OSD_INDEX_BLACK);
        UI_ShowStringByColor("Update logo  OK!\n", &Rect, _OSD_INDEX_YELLOW, _OSD_INDEX_BLACK);
        Delay_DelayMs(2000);
    }
    else
    {
        UI_FillRect(&Rect, _OSD_INDEX_BLACK);
        UI_ShowStringByColor("Update logo  failed!\n", &Rect, _OSD_INDEX_YELLOW, _OSD_INDEX_BLACK);
        Delay_DelayMs(2000);
    }

    if (ret2 == TRUE)
    {
        UI_FillRect(&Rect, _OSD_INDEX_BLACK);
        UI_ShowStringByColor("Update logo2  OK!\n", &Rect, _OSD_INDEX_YELLOW, _OSD_INDEX_BLACK);
        Delay_DelayMs(2000);
    }
    else
    {
        UI_FillRect(&Rect, _OSD_INDEX_BLACK);
        UI_ShowStringByColor("Update logo2  failed!\n", &Rect, _OSD_INDEX_YELLOW, _OSD_INDEX_BLACK);
        Delay_DelayMs(2000);
    }


    Delay_DelayMs(1500);
}

static BOOL get_key_value (char *cfgbuf,char *flgbuf)
{
    char* current;
    char* tail;
    char* value;
    char* key;
    UINT8 index=0;
    UINT8 count=0;
    if (!cfgbuf)
        return FALSE;

    current = cfgbuf;
    //remove first " "
    while (*current == ' ' && count < 0x80)
    {
          current++;
          count++;
    }

    while(TRUE)
    {
        count=0;
        tail = current;
        key = current;
        //key=value
        //first,find key(=)
        while (*tail != '=' && count < 0x80)
        {
              if (*tail == '\0' || *tail == '#') return TRUE;
              tail++;
              count++;
        }
        if (count >= 0x79) break;

        *tail = '\0';

        value = tail+1;

        count=0;

        tail++;
        while ( *tail != '\r' && *tail != '\n' && *tail != '\0' && count < 0x80)
        {
            tail++;
            count++;
        }
        if (count >= 0x79) break;
        *tail = '\0';
        tail++;
        while(TRUE)
        {
            if (*tail == '\r' || *tail == '\n')
            {
                tail++;
            }else
            {
                break;
            }
        }
        current = tail;
        if ((strlen(key) > KEY_LEN) || (strlen(value) > VALUE_LEN))
        {
            return FALSE;
        }
        strcpy(flgbuf+index*KEY_VALUE_LEN,key);
        strcpy(flgbuf+index*(KEY_VALUE_LEN)+KEY_LEN,value);
        index++;
    }
    return TRUE;
}


