/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       EngineerModeTbl.c
    @ingroup    mIPRJAPTest

    @brief      Engineer Mode item table
                Engineer Mode item table

    @note       Nothing.

    @date       2007/12/18
*/

/** \addtogroup mIPRJAPTest */
//@{
#include <string.h>
#include "SysCommon.h"
#include "AppCommon.h"
#include "UICommon.h"
#include "CalibrationWnd.h"
#include "DynamicMenu.h"
#include "FileSysTsk.h"
#include "SysCfg.h"
//#include "CalCIPA.h"
//#include "CalMITF.h"
//#include "CalBurnIn.h"
//#include "CalSerNum.h"
//#include "CalEmptyVolt.h"
//#include "CalRtc.h"
//#include "CalLangSet.h"
//#include "CalMisc.h"
//#include "CalGyro.h"
#include "PStore.h"
//#include "ae_task.h"
//#include "awb_task.h"
//#include "Lens.h"
//#include "LensCtrlTsk.h"
//#include "af_api.h"
#include "EngineerMode.h"
//#include "CalRunScript.h"
#include "CalibrationItem.h"
// Calibration requirement: Show different text color in menu. OK: Green text; NG: Red text.
//#include "CalibrationAPI.h"
#include "IPL_CalRW.h"
#include "UserCmd.h"
#include "nvtwifi.h"
#include "SysMain.h"
#include "UIAppNetwork.h"
#include "DxWiFi.h"
#include "PStore.h"
#ifdef __ECOS
#include "net_api.h"
#include <cyg/io/eth/rltk/819x/wlan/wifi_api.h>
#endif

//extern void IQ_GetAdjustRst(CalIQInfo *CalData);

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          EngTbl
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

UIDMenuInfo UIFlowInfoEngMode =
{
    0,
    0,
    0,
    ITEMS_PER_PAGE
};
UIDMenuInfo     *pEngModeDMenuInfo = &UIFlowInfoEngMode;
volatile BOOL   g_bCalbirationOpened = FALSE;
CHAR*           g_pExt;
UINT32          g_uiResult;
volatile BOOL   g_bCalibrationModeChange = FALSE;
BOOL            g_bIsEngineerMode = FALSE;
BOOL            g_bForcedSetPCC = FALSE;
BOOL            g_bAELogEnable = FALSE;
BOOL            g_bADASLogEnable = FALSE;
BOOL            g_bWiFiTest = FALSE;
BOOL            g_bResetToDefault=FALSE;
BOOL            g_bIsGPSTest = FALSE;

static BOOL EngineerMode_CheckAutoWiFi(void);

// Calibration requirement: Show different text color in menu. OK: Green text; NG: Red text.
/*
static UINT32 ChkSelfTestStatus(void)
{
    return  (gCalSysData.RTC_Test_Status == _RTC_TEST_STATUS);
}

static UINT32 ChkBatEmptyVolt(void)
{
    return (gCalSysData.EmptyVolt_Test_Status == _EMPTYVOLT_STATUS);
}

static UINT32 ChkBurnInSts(void)
{
    return (gCalSysData.Burnin_Status== _BURNIN_STATUS);
}

static UINT32 ChkLangSts(void)
{
    return (gCalSysData.Language_Status == _LANGUAGE_STATUS);
}

static UINT32 ChkSNTestSts(void)
{
    return (gCalSysData.SerialNumber_Test_Status == _SERNUM_STATUS);
}

static UINT32 ChkGryoZeroAdjust(void)
{
    return (gCalSysData.Gyro_Zero_Adjust_Status == _GYRO_ZERO_ADJUST_STATUS);
}

static UINT32 ChkOrientAdjust(void)
{
    return (gCalSysData.Orient_Adjust_Status == _ORIENT_ADJUST_STATUS);
}


static UINT32 ChkDNPCalSts(void)
{
    return (DNP_Status_chk(NULL));
}

static UINT32 ChkISOCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.ISO_STATUS == _CAL_OK);
}

static UINT32 ChkAWBCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.AWB_STATUS == _CAL_OK);
}

static UINT32 ChkDPCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.DP_STATUS == _CAL_OK);
}

static UINT32 ChkDP_Dark_CalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return ((CalIQData.Cal_Status.DP_STATUS == _CAL_UnderFlow) || (CalIQData.Cal_Status.DP_STATUS == _CAL_OK));
}

static UINT32 ChkDP_Bright_CalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.DP_STATUS == _CAL_OK);
}

static UINT32 ChkECSCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.ECS_STATUS == _CAL_OK);
}

static UINT32 ChkAECHKCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.AECHK_STATUS == _CAL_OK);
}
*/

void Cal_BurnIn(void)
{
    DBG_DUMP("^C Cal_BurnIn!\r\n");
    Cal_ForceExitWnd();
    UserCmd_RunCmd(USERCMD_BURNIN);
}

void Cal_RunScript(void)
{
    DBG_DUMP("^C Cal_RunScript!\r\n");
    Cal_ForceExitWnd();
    UserCmd_RunCmd(USERCMD_RUNSCRIPT);
}

void Cal_AudioTest(void)
{
    DBG_DUMP("^C Cal_AudioTest!\r\n");
    Cal_ForceExitWnd();
    UserCmd_RunCmd(USERCMD_AUDIOTEST);
}

void Cal_GPSTest(void)
{
    DBG_DUMP("^C Cal_GPSTest!\r\n");
    Cal_ForceExitWnd();
    g_bIsGPSTest = TRUE;
    UserCmd_RunCmd(USERCMD_GPSTEST);
}

void Cal_WavTest(void)
{
    DBG_DUMP("^C Cal_WavTest!\r\n");
    Cal_ForceExitWnd();
    UserCmd_RunCmd(USERCMD_WAVTEST);
}

void Cal_PCC(void)
{
    DBG_DUMP("^C Cal_PCC!\r\n");
    g_bForcedSetPCC = TRUE;
    Cal_ForceExitWnd();
    UserCmd_RunCmd(USERCMD_PCC);
}

void Cal_AELog(void)
{
    DBG_DUMP("^C Cal_AELog!\r\n");
    g_bAELogEnable = TRUE;
    UI_SetData(FL_MOVIE_DATEIMPRINT, MOVIE_DATEIMPRINT_ON);
    Cal_ForceExitWnd();
}

void Cal_ADASLog(void)
{
    DBG_DUMP("^C Cal_ADASLog!\r\n");
    g_bADASLogEnable = TRUE;
    Cal_ForceExitWnd();
}

void Cal_NvtDebug(void)
{
    DBG_DUMP("^C Cal_NvtDebug!\r\n");
    Cal_ForceExitWnd();
    Delay_DelayMs(100); // make sure Cal window is closed
    USB_SetForceMsdcNvt(TRUE);
}

void Cal_WiFiTest(void)
{
    BOOL bAutoWiFi;

    DBG_DUMP("^C Cal_WiFiTest!\r\n");
    g_bWiFiTest = TRUE;
    bAutoWiFi = EngineerMode_CheckAutoWiFi();
    Cal_ForceExitWnd();
    Delay_DelayMs(100); // make sure Cal window is closed
    USB_SetForceMsdcNvt(TRUE);
    if (bAutoWiFi)
    {
        BKG_PostEvent(NVTEVT_BKW_WIFI_ON);
    }
}

void Cal_FwVersion(void)
{
    CHAR    cDataStr[30], cSNData[30];
    char    *pSNStr;

    DBG_DUMP("^C Cal_FwVersion!\r\n");

    // read serial number
    pSNStr = EngineerMode_ReadSerialNum();

    UI_ClearOSD(CLRID_IDX_BLUE);
    memset((void *)cDataStr, 0, sizeof(cDataStr));
    memset((void *)cSNData, 0, sizeof(cSNData));

    // show FW version
    snprintf(cDataStr, sizeof(cDataStr), "FW ver: %s", FW_VERSION_NUM);
    UI_DrawOsdString(cDataStr, 12, 80, CLRID_IDX_WHITE, FALSE);

    // show serial number
    snprintf(cDataStr, sizeof(cDataStr), "SN: %s", pSNStr);
    UI_DrawOsdString(cDataStr, 12, 120, CLRID_IDX_WHITE, FALSE);

    // wait for any key
    while ((GxKey_GetData(GXKEY_NORMAL_KEY) & 0xffffffff) == 0)
    {
        Ux_FlushEvent();
        Delay_DelayMs(10);
    }

    Cal_ForceExitWnd();
}

void Cal_ResetToDefault(void)
{
    CHAR    cDataStr[30];

    g_bResetToDefault = TRUE;

    DBG_DUMP("^C Cal_ResetToDefault!\r\n");

    UI_ClearOSD(CLRID_IDX_GREEN);

    // show FW version
    snprintf(cDataStr, sizeof(cDataStr), "Reset OK!");
    UI_DrawOsdString(cDataStr, 100, 80, CLRID_IDX_WHITE, FALSE);

    Save_MenuInfo();

    Delay_DelayMs(2000);

    System_PowerOff(SYS_POWERON_NORMAL);
}

void Cal_CheckMacAddr(void)
{
#if defined(__ECOS) && (_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_NVT_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_BRCM_)
    CHAR    cDataStr[30];
    CHAR    cMacAddr[6];

    UI_ClearOSD(CLRID_IDX_BLUE);
    memset((void *)cDataStr, 0, sizeof(cDataStr));

    EngineerMode_TurnOnWiFi();

    if (wifi_get_wlan0_efuse_mac(cMacAddr) < 0)
    {
        // no MAC address
        UI_DrawOsdString("No MAC Address!", 12, 100, CLRID_IDX_RED, FALSE);
    }
    else
    {
        // show MAC address
        snprintf(cDataStr, sizeof(cDataStr), "MAC: %02x:%02x:%02x:%02x:%02x:%02x",
            cMacAddr[0], cMacAddr[1], cMacAddr[2], cMacAddr[3], cMacAddr[4], cMacAddr[5]);
        UI_DrawOsdString(cDataStr, 12, 100, CLRID_IDX_WHITE, FALSE);
    }

    DrvWiFi_PowerOff();

    // wait for any key
    while ((GxKey_GetData(GXKEY_NORMAL_KEY) & 0xffffffff) == 0)
    {
        Ux_FlushEvent();
        Delay_DelayMs(10);
    }

    Cal_ForceExitWnd();
#endif
}

// Calibration requirement: Show different text color in menu. OK: Green text; NG: Red text.
DMENU_ITEM g_EngineerItem[]    =
{
    //Main Items, Intergrated Items
    {" 1. Burn-In",                     Cal_BurnIn               ,NULL,     NULL},
#if (_CALIBRATION_MODE_ == ENABLE)
    {" 2. Focus Adjust",                Cal_SenFocus             ,NULL,     NULL},
    {" 3. DNP Adjust",                  Cal_SenDNP               ,NULL,     NULL},
    {" 4. Image Test",                  Cal_ImageTest            ,NULL,     NULL},
    {" 5. Read CAL status",             Cal_GetCalRst            ,NULL,     NULL},
#endif
    {" 6. Audio Test",                  Cal_AudioTest            ,NULL,     NULL},
    {" 7. GPS Test",                    Cal_GPSTest              ,NULL,     NULL},
    {" 8. Exit",                        NULL                     ,NULL,     NULL},

    // the following (after item "Exit") will not be displayed on calibration menu,
    // but user can use related .CAL to execute it
    {" Run Script",                     Cal_RunScript            ,NULL,     NULL},
    {" Wav Test",                       Cal_WavTest              ,NULL,     NULL},
    {" PCC Test",                       Cal_PCC                  ,NULL,     NULL},
    {" AE Log",                         Cal_AELog                ,NULL,     NULL},
    {" ADAS Log",                       Cal_ADASLog              ,NULL,     NULL},
    {" NvtDebug",                       Cal_NvtDebug             ,NULL,     NULL},
    {" WiFi Test",                      Cal_WiFiTest             ,NULL,     NULL},
    {" FW Version",                     Cal_FwVersion            ,NULL,     NULL},
    {" Reset To Default",               Cal_ResetToDefault       ,NULL,     NULL},
    {" Reset CAL Data",                 Cal_Reset_Data           ,NULL,     NULL},
    {" Show Current Env",               Cal_ShowCurEnv           ,NULL,     NULL},
    {" DNP Adjust BurnIn",              Cal_SenDNPBurnIn         ,NULL,     NULL},
    {" MAC Addr",                       Cal_CheckMacAddr         ,NULL,     NULL},
};

UINT32 g_EngineerMaxItemNum     = sizeof(g_EngineerItem) / sizeof(DMENU_ITEM) - 1;
UINT32 g_EngineerMaxItemNumOri  = sizeof(g_EngineerItem) / sizeof(DMENU_ITEM) - 1; // original max item


static void CalRoot_ScanCB(FIND_DATA *findData, BOOL *bContinue, UINT16 *cLongname, UINT32 Param)
{
#if 0
    CHAR    orgFilename[9];
    UINT32  i;
    if (!M_IsDirectory(findData->attrib))
    {
        if (!strncmp(findData->FATExtName, g_pExt, 3))
        {
            memcpy(orgFilename, findData->FATMainName, 8);
            orgFilename[8] = '\0';
            sscanf(orgFilename, "%08X", &g_uiResult);
            for (i=0; i<8; i++)
            {
                if (isxdigit(*(orgFilename + i)) == 0)
                {
                    g_uiResult = NO_DEFAULT_RUN;
                    break;
                }
            }
        }
    }

#else // for long file name

    #define LONG_NAME_MAX   30
    #define EXT_NAME_SIZE   4

    CHAR    cFileName[LONG_NAME_MAX]; // long file name, include extension
    CHAR    ch;
    UINT32  i, j;
    UINT32  uiShift = 0, uiCompareSize = 0;

    if (!M_IsDirectory(findData->attrib))
    {
        if (!strncmp(findData->FATExtName, g_pExt, 3))
        {
            if (cLongname[0])
            {
                memset(cFileName, 0, LONG_NAME_MAX);
                for (i = 0; i < LONG_NAME_MAX; i++)
                {
                    if (cLongname[i] == 0)
                    {
                        uiCompareSize = i - EXT_NAME_SIZE;
                        break;
                    }

                    // only support ASCII
                    if (cLongname[i] < 128)
                    {
                        cFileName[i] = (CHAR)cLongname[i];
                    }
                    else
                    {
                        break;
                    }
                }
            }

            if (uiCompareSize)
            {
                for (i = 0; i <= g_EngineerMaxItemNumOri; i++)
                {
                    for (j = 0; j < LONG_NAME_MAX; j++)
                    {
                        ch = g_EngineerItem[i].pItemName[j];
                        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
                        {
                            uiShift = j;
                            break;
                        }
                    }

                    if (memcmp(&g_EngineerItem[i].pItemName[uiShift], cFileName, uiCompareSize) == 0)
                    {
                        g_uiResult = i + 1;
                        break;
                    }
                }
            }
        }
    }
#endif
}

/**
  Find file that filename with extension in root folder

  Find file that filename with extension in root folder and return the main
  filename (convert to integer of UINT32)
  [Calibration internal API]

  @param INT8 *pExt: extension
  @return UINT32: Filename of integer
*/

UINT32 Cal_GetDefaultInRoot(CHAR * pExt)
{
    g_uiResult = NO_DEFAULT_RUN;
    g_pExt = pExt;
#if 0
    FileSys_ScanDir("A:\\", CalRoot_ScanCB, FALSE, 0);
#else // for long file name test
    FileSys_ScanDir("A:\\", CalRoot_ScanCB, TRUE, 0);
#endif
    return g_uiResult;
}

BOOL EngineerMode_CheckEng(void)
{
    BOOL ret = FALSE;
    FST_FILE filehdl = NULL;

    if (UI_GetData(FL_CardStatus) == CARD_INSERTED)
    {
        filehdl = FileSys_OpenFile(ENG_MODE_FILE,FST_OPEN_READ);
        if (filehdl!=NULL)
        {
           FileSys_CloseFile(filehdl);
           ret = TRUE;
        }
    }
    return ret;
}


void EngineerMode_Open(void)
{
    UINT32  i;

    if (g_bCalbirationOpened == TRUE)
    {
        return;
    }
    g_bCalbirationOpened = TRUE;
    g_bIsEngineerMode = TRUE;

    Input_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    Input_SetKeyMask(KEY_RELEASE, FLGKEY_KEY_MASK_DEFAULT);
    Input_SetKeyMask(KEY_CONTINUE, FLGKEY_KEY_MASK_DEFAULT);

    for (i = 0; i <= g_EngineerMaxItemNum; i++)
    {
        if (g_EngineerItem[i].ItemFunction == NULL)
        {
            DBG_DUMP("^Y max active cal item = %d\r\n", i);
            break;
        }
    }
    g_EngineerMaxItemNum = i;

    //Ux_OpenWindow((VControl *)(&EngModeWndCtrl), 3, g_EngineerItem,g_EngineerMaxItemNum,NO_DEFAULT_RUN);
    pEngModeDMenuInfo->menuItem = g_EngineerItem;
    pEngModeDMenuInfo->uiMaxItem = g_EngineerMaxItemNum;
    //pEngModeDMenuInfo->uiSelectNum = NO_DEFAULT_RUN;
    pEngModeDMenuInfo->uiSelectNum = Cal_GetDefaultInRoot("CAL");
    // don't need to open calibration window here since it will be opened in EngineerMode_WndOpen()
    //Ux_OpenWindow((VControl *)(&CalibrationWndCtrl), 1, pEngModeDMenuInfo);

    {
        static BOOL bSNFileCheck = FALSE;

        if (bSNFileCheck == FALSE)
        {
            bSNFileCheck = TRUE;
            if (EngineerMode_CheckSNFile() == E_OK)
            {
                EngineerMode_SaveSerialNum();
            }
        }
    }
}


BOOL IsEngineerModeOpened(void)
{
    return g_bCalbirationOpened;
}

void EngineerMode_WndOpen(void)
{
    Input_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    Input_SetKeyMask(KEY_RELEASE, FLGKEY_KEY_MASK_DEFAULT);
    Input_SetKeyMask(KEY_CONTINUE, FLGKEY_KEY_MASK_DEFAULT);

    //Ux_OpenWindow((VControl *)(&EngModeWndCtrl), 3, g_EngineerItem,g_EngineerMaxItemNum,NO_DEFAULT_RUN);
    pEngModeDMenuInfo->menuItem = g_EngineerItem;
    pEngModeDMenuInfo->uiMaxItem = g_EngineerMaxItemNum;
    //pEngModeDMenuInfo->uiSelectNum = NO_DEFAULT_RUN;
    pEngModeDMenuInfo->uiSelectNum = Cal_GetDefaultInRoot("CAL");
    //temp use CalibrationWndCtrl UI
    Ux_OpenWindow((VControl *)(&CalibrationWndCtrl), 1, pEngModeDMenuInfo);
}

void EngineerMode_WndClose(void)
{
    Ux_CloseWindow((VControl *)(&CalibrationWndCtrl), 0);
}

BOOL IsEngineerModeChange(void)
{
    return g_bCalibrationModeChange;
}

void SetEngineerModeChange(BOOL b)
{
    g_bCalibrationModeChange = b;
}

BOOL IsEngineerMode(void)
{
    return g_bIsEngineerMode;
}

BOOL IsForcedSetPCC(void)
{
    return g_bForcedSetPCC;
}

BOOL IsAELog(void)
{
    return g_bAELogEnable;
}

BOOL IsADASLog(void)
{
    return g_bADASLogEnable;
}

BOOL IsWiFiTest(void)
{
    return g_bWiFiTest;
}

BOOL IsResetToDefault(void)
{
    return g_bResetToDefault;
}

//-------------------------------------------------------------------------------------------------
// note: max length of PStore section name is 12!
#define PS_WIFI_MAC         "WIFI_MAC"
#define PS_WIFI_CRYSTAL     "WIFI_CRYS"
#define PS_SERIAL_NUM       "SERIAL_NUM"
#define CRYSTAL_INFO_DATA   "26MHz"
#define CRYSTAL_INFO_SIZE   sizeof(CRYSTAL_INFO_DATA)
#define SN_FILE_NAME        "A:\\SN.txt"
#define SN_STR_SIZE         30

static char gMacAddr[NVT_WIFIIPC_MAC_ADDR_LEN] = {0,0,0,0x81,0x89,0xe5};
static char gWiFiMacStr[NVT_WIFIIPC_MAC_ADDR_LEN*2+1] = {0};
static char gCrystal[CRYSTAL_INFO_SIZE] = {0};
static char gApMacStr[NVT_WIFIIPC_MAC_ADDR_LEN+1] = {0};
static char gSNStr[SN_STR_SIZE] = {0};

ER EngineerMode_TurnOnWiFi(void)
{
#if defined(__ECOS) && (_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_NVT_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_BRCM_)
    UINT32  result = 0;

    DrvWiFi_PowerOn();
    result = wifi_init_wlan0_netdev(0);  // Init Wi-Fi module
    if (result != 0)
    {
        DBG_ERR("wifi init fail %d\r\n",result);
        return E_SYS;
    }
    else
    {
        DBG_DUMP("wifi init succesful \r\n");
    }
#endif
    return E_OK;
}

void EngineerMode_SaveWiFiMAC(void)
{
#if defined(__ECOS) && (_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_NVT_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_BRCM_)
    PPSTORE_SECTION_HANDLE  pSection;

    if (wifi_get_wlan0_efuse_mac(gMacAddr) < 0)
    {
        DBG_ERR("wifi_get_wlan0_efuse_mac() fail. Use hardcoded mac.\r\n");
        set_mac_address("wlan0", "\x00\x00\x00\x81\x89\xe5");
    }
    else
    {
        DBG_DUMP("wlan0 efuse mac [%02x:%02x:%02x:%02x:%02x:%02x]\r\n",
            gMacAddr[0], gMacAddr[1], gMacAddr[2],
            gMacAddr[3], gMacAddr[4], gMacAddr[5]);
    }

    #if PST_FUNC
    // save MAC address to PStore
    if ((pSection = PStore_OpenSection(PS_WIFI_MAC, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        PStore_WriteSection((UINT8 *)gMacAddr, 0, sizeof(gMacAddr), pSection);
        PStore_CloseSection(pSection);
    }
    else
    {
        DBG_ERR("Write WiFi MAC to PS failed!\r\n");
    }
    #endif
#endif
}

char* EngineerMode_ReadAPMAC(void)
{
#if defined(__ECOS) && (_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_NVT_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_BRCM_)
	#if PST_FUNC
    PPSTORE_SECTION_HANDLE  pSection;

    // read MAC address from PStore
    if ((pSection = PStore_OpenSection(PS_WIFI_MAC, PS_RDWR)) != E_PS_SECHDLER)
    {
        if (PStore_ReadSection((UINT8 *)gMacAddr, 0, sizeof(gMacAddr), pSection) != E_PS_OK)
        {
            DBG_ERR("Read WiFi MAC from PS failed!\r\n");
        }
        PStore_CloseSection(pSection);
    }

    snprintf(gApMacStr, sizeof(gApMacStr), "%02X%02X", gMacAddr[4], gMacAddr[5]);
    DBG_DUMP("^Y Read AP MAC: %s\r\n", gApMacStr);
    return gApMacStr;
	#endif
#else
	return gApMacStr;
#endif
}

char* EngineerMode_ReadWiFiMAC(void)
{
#if defined(__ECOS) && (_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_NVT_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_BRCM_)
	#if PST_FUNC
    PPSTORE_SECTION_HANDLE  pSection;

    // read MAC address from PStore
    if ((pSection = PStore_OpenSection(PS_WIFI_MAC, PS_RDWR)) != E_PS_SECHDLER)
    {
        if (PStore_ReadSection((UINT8 *)gMacAddr, 0, sizeof(gMacAddr), pSection) != E_PS_OK)
        {
            DBG_ERR("Read WiFi MAC from PS failed!\r\n");
        }
        PStore_CloseSection(pSection);
    }

    snprintf(gWiFiMacStr, sizeof(gWiFiMacStr), "%02X%02X%02X%02X%02X%02X",
        gMacAddr[0], gMacAddr[1], gMacAddr[2], gMacAddr[3], gMacAddr[4], gMacAddr[5]);
    DBG_DUMP("^Y Read WiFi MAC: %s\r\n", gWiFiMacStr);
    return gWiFiMacStr;
	#endif
#else
	return gWiFiMacStr;
#endif
}

char* EngineerMode_GetWiFiMAC(void)
{
    return gWiFiMacStr;
}

ER EngineerMode_CheckWiFiCrystal(void)
{
#if defined(__ECOS) && (_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_NVT_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_BRCM_)
	#if PST_FUNC
    PPSTORE_SECTION_HANDLE  pSection;

    // read WiFi crystal setting from PStore
    if ((pSection = PStore_OpenSection(PS_WIFI_CRYSTAL, PS_RDWR)) != E_PS_SECHDLER)
    {
        if (PStore_ReadSection((UINT8 *)gCrystal, 0, sizeof(gCrystal), pSection) != E_PS_OK)
        {
            DBG_ERR("Read WiFi crystal from PS failed!\r\n");
        }
        PStore_CloseSection(pSection);
    }

    // check crystal value
    DBG_DUMP("^C Read crystal data %s\r\n", gCrystal);
    if (memcmp(gCrystal, CRYSTAL_INFO_DATA, CRYSTAL_INFO_SIZE) == 0)
    {
        DBG_DUMP("^G Check WiFi crystal OK!\r\n");
        return E_OK;
    }

    EngineerMode_TurnOnWiFi();

    // check if WiFi eFuse setting is OK ... waiting for RTK's solution

    // write crystal setting to WiFi eFuse
    RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "efuse_get", "HW_CRYSTAL_CLK", NULL_STR);
    RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "efuse_set", "HW_CRYSTAL_CLK=50", NULL_STR);
    RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "efuse_sync", NULL_STR);
    RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "efuse_get", "HW_CRYSTAL_CLK", NULL_STR);

    //??//UI_SetData(FL_COMMON_LOCAL,WIFI_OFF);
    DrvWiFi_PowerOff();

    // write WiFi crystal setting to PStore
    DBG_DUMP("^C Write crystal data %s\r\n", CRYSTAL_INFO_DATA);
    if ((pSection = PStore_OpenSection(PS_WIFI_CRYSTAL, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        PStore_WriteSection((UINT8 *)CRYSTAL_INFO_DATA, 0, CRYSTAL_INFO_SIZE, pSection);
        PStore_CloseSection(pSection);
    }
    else
    {
        DBG_ERR("Write WiFi crystal to PS failed!\r\n");
        return E_SYS;
    }

	#endif
#endif
    return E_OK;
}

BOOL EngineerMode_CheckSNFile(void)
{
    UINT32      uiReadSize = SN_STR_SIZE;
    CHAR        cDataBuf[SN_STR_SIZE];
    FST_FILE    pFile;

    pFile = FileSys_OpenFile(SN_FILE_NAME, FST_OPEN_READ);
    if (pFile)
    {
        memset(cDataBuf, 0, sizeof(cDataBuf));
        FileSys_ReadFile(pFile, (UINT8 *)cDataBuf, &uiReadSize, 0, NULL);
        FileSys_CloseFile(pFile);

        DBG_DUMP("Read size %d\r\n", uiReadSize);
        if (uiReadSize)
        {
            EngineerMode_ReadAPMAC();
            snprintf(gSNStr, sizeof(gSNStr), "%s%s", cDataBuf, gApMacStr);
            DBG_DUMP("^Y SN = %s, MAC = %s\r\n", gSNStr, gApMacStr);
        }
        else
        {
            DBG_ERR("SN file size error!\r\n");
            return E_SYS;
        }
    }
    else
    {
        DBG_ERR("Open SN file error!\r\n");
        return E_SYS;
    }

    return E_OK;
}

void EngineerMode_SaveSerialNum(void)
{
#if PST_FUNC
    PPSTORE_SECTION_HANDLE  pSection;

    // save MAC address to PStore
    if ((pSection = PStore_OpenSection(PS_SERIAL_NUM, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        PStore_WriteSection((UINT8 *)gSNStr, 0, sizeof(gSNStr), pSection);
        PStore_CloseSection(pSection);
    }
    else
    {
        DBG_ERR("Write SN to PS failed!\r\n");
    }
#endif
}

char* EngineerMode_ReadSerialNum(void)
{
#if PST_FUNC
    PPSTORE_SECTION_HANDLE  pSection;

    // read MAC address from PStore
    if ((pSection = PStore_OpenSection(PS_SERIAL_NUM, PS_RDWR)) != E_PS_SECHDLER)
    {
        if (PStore_ReadSection((UINT8 *)gSNStr, 0, sizeof(gSNStr), pSection) != E_PS_OK)
        {
            DBG_ERR("Read SN from PS failed!\r\n");
        }
        PStore_CloseSection(pSection);
    }

    DBG_DUMP("^Y Read SN: %s\r\n", gSNStr);

    return gSNStr;
#endif
}

static BOOL EngineerMode_CheckAutoWiFi(void)
{
#if defined(__ECOS) && (_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_NVT_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_BRCM_)
    #define WIFI_SETTING_SIZE   60
    #define WIFI_ONOFF_STRING   "WIFI="
    #define WIFI_SSID_STRING    "SSID="
    #define WIFI_PASS_STRING    "PASSWORD="

    UINT32      uiReadSize = WIFI_SETTING_SIZE, uiStrLen;
    BOOL        bAutoWiFi = FALSE;
    CHAR        *pCh;
    static CHAR cDataBuf[WIFI_SETTING_SIZE];
    static CHAR cWiFiOnOff[4];
    static CHAR cWiFiSSID[25];
    static CHAR cWiFiPass[16];
    FST_FILE    pFile;

    pFile = FileSys_OpenFile("A:\\WiFi Test.CAL", FST_OPEN_READ);
    if (pFile)
    {
        FileSys_ReadFile(pFile, (UINT8 *)cDataBuf, &uiReadSize, 0, NULL);
        FileSys_CloseFile(pFile);

        //DBG_DUMP("Read size %d\r\n", uiReadSize);
        if (uiReadSize)
        {
            memset((void *)cWiFiOnOff, 0, sizeof(cWiFiOnOff));
            memset((void *)cWiFiSSID,  0, sizeof(cWiFiSSID));
            memset((void *)cWiFiPass,  0, sizeof(cWiFiPass));

            // check WiFi on/off setting
            pCh = strstr(cDataBuf, WIFI_ONOFF_STRING);
            if (pCh == NULL)
            {
                DBG_DUMP("^R No WiFi ON/OFF setting!\r\n");
            }
            else
            {
                sscanf(pCh + sizeof(WIFI_ONOFF_STRING) - 1, "%s", &cWiFiOnOff);
                DBG_DUMP("^C WiFi ON/OFF: %s\r\n", cWiFiOnOff);
                if (!strncmp(cWiFiOnOff, "ON", 2) || !strncmp(cWiFiOnOff, "on", 2))
                {
                    bAutoWiFi = TRUE;
                }
            }

            // check WiFi SSID setting
            pCh = strstr(cDataBuf, WIFI_SSID_STRING);
            if (pCh == NULL)
            {
                DBG_DUMP("^R No SSID setting!\r\n");
            }
            else
            {
                pCh += sizeof(WIFI_SSID_STRING) - 1;

                if ((*pCh == '\r') && (*(pCh + 1) == '\n'))
                {
                    DBG_DUMP("^R SSID is NULL!\r\n");
                }
                else
                {
                    sscanf(pCh, "%s", &cWiFiSSID);
                    uiStrLen = strlen(cWiFiSSID);
                    if (uiStrLen == 0)
                    {
                        DBG_DUMP("^R SSID is NULL!\r\n");
                    }
                    else
                    {
                        DBG_DUMP("^C WiFi SSID %s, len %d\r\n", cWiFiSSID, uiStrLen);
                        UINet_SetSSID(cWiFiSSID, uiStrLen);
                    }
                }
            }

            // check WiFi password setting
            pCh = strstr(cDataBuf, WIFI_PASS_STRING);
            if (pCh == NULL)
            {
                DBG_DUMP("^R No password setting!\r\n");
            }
            else
            {
                pCh += sizeof(WIFI_PASS_STRING) - 1;

                if ((*pCh == '\r') && (*(pCh + 1) == '\n'))
                {
                    DBG_DUMP("^R Password is NULL!\r\n");
                }
                else
                {
                    sscanf(pCh, "%s", &cWiFiPass);
                    uiStrLen = strlen(cWiFiPass);
                    if (uiStrLen == 0)
                    {
                        DBG_DUMP("^R Password is NULL!\r\n");
                    }
                    else
                    {
                        DBG_DUMP("^C WiFi password %s, len %d\r\n", cWiFiPass, uiStrLen);
                        UINet_SetPASSPHRASE(cWiFiPass, uiStrLen);
                    }
                }
            }
        }
    }
    else
    {
        DBG_ERR("Open WiFi Test file error!\r\n");
    }

    return bAutoWiFi;
#else
	return FALSE;
#endif
}

void EngineerMode_CheckGPSTest(void)
{
    if (g_bIsGPSTest)
    {
        // if currently in GPS test mode, enter it again
        UserCmd_RunCmd(USERCMD_GPSTEST);
    }
}

//@}
