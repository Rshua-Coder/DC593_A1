
#include "UIInfo.h"


#if defined(__ECOS) && (_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_NVT_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_BRCM_)
#include <LibWifi/HAL/nvtwifi.h>
#endif
#ifdef __ECOS
#include "net_api.h"
#include <cyg/fileio/fileio.h>
#include "ecos_FileSys.h"
#include <cyg/fs/nvtfs.h>
#include <cyg/hfs/hfs.h>

#include "DhcpNvt.h"
#include <net/dhcpelios/dhcpelios.h>

#if (USOCKET_CLIENT==ENABLE)
#include <cyg/usocket_cli/usocket_cli.h>
#endif

#include "wps_api.h"
#include "WifiAppCmd.h"
#include "WifiAppXML.h"
//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#include <axTLS/ssl.h>
#include <axTLS/crypto.h>
//#NT#2016/04/21#Lincy Lin -end
#if (YOUKU_SDK == ENABLE)
#include "YoukuAppCmd.h"
#endif

#endif
#include "Timer.h"
#include "NVTToolCommand.h"
#include "UIBackgroundObj.h"
#include "UIAppNetwork.h"
#include "AppCommon.h"
#include "SysCommon.h"
#include "DxWiFi.h"
#include "EngineerMode.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UINet
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#define PASSPHRASE_DEFAULT  "12345678"
#ifdef __ECOS
#define MOUNT_FS_ROOT        "/sdcard"
char gNvtDhcpHostNameCli[DHCP_HOST_NAME_MAX_LEN] = "nvtsys01cli";
char gNvtDhcpHostNameSrv[DHCP_HOST_NAME_MAX_LEN] = "nvtsys01srv";
char gMacAddr[6] = {0,0,0,0x81,0x89,0xe5};
UINT32 gAuthType = NET_AUTH_WPA2;
UINT32 gMode = NET_AP_MODE;
char gSSID[WSC_MAX_SSID_LEN] = "NVT_CARDV";
char gSSID_AP_default[WSC_MAX_SSID_LEN] = "NVT_CARDV";
char gPASSPHRASE[MAX_WEP_KEY_LEN] = "12345678";
char gPASSPHRASE_AP_default[MAX_WEP_KEY_LEN] = "12345678";
char gCurrIP[NET_IP_MAX_LEN] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
UINT32 gChannel = 0;//default 0:auto
char gLastMacAddr[NET_IP_MAX_LEN] = {0};
char gRemoveMacAddr[NET_IP_MAX_LEN] = {0};
char currentMacAddr[NET_IP_MAX_LEN] = {0};
#define EXAM_NET_IP_ETH0            "192.168.0.3"
#define EXAM_NET_NETMASK_ETH0       "255.255.255.0"
#define EXAM_NET_BRAODCAST_ETH0     "192.168.0.255"
#define EXAM_NET_GATEWAY_ETH0       "192.168.0.1"
#define EXAM_NET_SRVIP_ETH0         "192.168.0.1"
#define EXAM_NET_LEASE_START_ETH0   "192.168.0.12"
#define EXAM_NET_LEASE_END_ETH0     "192.168.0.32"
#define EXAM_NET_LEASE_DNS_ETH0     "0.0.0.0"
#define EXAM_NET_LEASE_WINS_ETH0    "0.0.0.0"

#define EXAM_NET_IP_ETH1            "192.168.1.3"
#define EXAM_NET_NETMASK_ETH1       "255.255.255.0"
#define EXAM_NET_BRAODCAST_ETH1     "192.168.1.255"
#define EXAM_NET_GATEWAY_ETH1       "192.168.1.1"
#define EXAM_NET_SRVIP_ETH1         "192.168.1.1"

#define EXAM_NET_IP_WLAN0            "192.168.1.254"
#define EXAM_NET_NETMASK_WLAN0       "255.255.255.0"
#define EXAM_NET_BRAODCAST_WLAN0     "192.168.1.255"
#define EXAM_NET_GATEWAY_WLAN0       "192.168.1.254"
#define EXAM_NET_SRVIP_WLAN0         "192.168.1.254"
#define EXAM_NET_LEASE_START_WLAN0   "192.168.1.33"
#define EXAM_NET_LEASE_END_WLAN0     "192.168.1.52"
#define EXAM_NET_LEASE_DNS_WLAN0     "0.0.0.0"
#define EXAM_NET_LEASE_WINS_WLAN0    "0.0.0.0"

#define WPS_SUPPORT                 0
#define HIDDEN_AP_SUPPORT           0

extern UIMenuStoreInfo  currentInfo;
nvt_wifi_settings wifiConfig = {0};
#if(_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_NVT_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_BRCM_)
static BOOL g_CliConnected = FALSE;
int SX_TIMER_NET_CONNECT_ID = -1;
#ifndef WIFI_STA_RECONNECT_MAX_COUNT
#define WIFI_STA_RECONNECT_MAX_COUNT 10
#endif
#ifndef WIFI_STA_RECONNECT_INTERVAL
#define WIFI_STA_RECONNECT_INTERVAL 150
#endif
static int reconnect_count = 0;
#endif

#if (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189_) || (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189xtal26_)|| (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189ftv_)
extern void wifi_set_passwd_hash_delay(int tick); // >0: delay in ticks, 0: turn off the mechanism, <0: use cyg_thread_yield() instead
extern int wifi_get_passwd_hash_delay(void);
#endif
extern void print_wscd_status(int status);

#if (WIFI_FTP_FUNC==ENABLE)
void start_ftpserver(void);
void stop_ftpserver(void);
#endif

#define DHCP_LEASE_CNT    1  // Only 1 client is allowed.

char gNetIntf[3][6] =
{
    'e','t','h','0',0,0,
    'e','t','h','1',0,0,
    'w','l','a','n','0',0,
};
cyg_uint32 xExamHfs_getCurrTime(void)
{
    return timer_getCurrentCount(timer_getSysTimerID());
}

//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#if (_SSLTYPE_==_SSL_OFF_)
static const char * const axtls_unsupported_str = "Error: axtls Feature not supported\n";
EXP_FUNC void STDCALL __attribute__((weak)) MD5_Init(MD5_CTX *ctx)
{
    DBG_ERR("%s",axtls_unsupported_str);
}

EXP_FUNC void STDCALL __attribute__((weak)) MD5_Update(MD5_CTX *ctx, const uint8_t * msg, int len)
{
    DBG_ERR("%s",axtls_unsupported_str);
}

EXP_FUNC void STDCALL __attribute__((weak)) MD5_Final(uint8_t *digest, MD5_CTX *ctx)
{
    DBG_ERR("%s",axtls_unsupported_str);
}
#endif
//#NT#2016/04/21#Lincy Lin -end

static void UINet_HFSNotifyStatus(int status)
{
    switch (status)
    {
        // HTTP client has request coming in
        case CYG_HFS_STATUS_CLIENT_REQUEST:
             //DBG_IND("client request %05d ms\r\n",xExamHfs_getCurrTime()/1000);
             break;
        // HTTP server send data start
        case CYG_HFS_STATUS_SERVER_RESPONSE_START:
             //DBG_IND("send data start, time= %05d ms\r\n",xExamHfs_getCurrTime()/1000);
             break;
        // HTTP server send data start
        case CYG_HFS_STATUS_SERVER_RESPONSE_END:
             //DBG_IND("send data end, time= %05d ms\r\n",xExamHfs_getCurrTime()/1000);
             break;
        // HTTP client disconnect
        case CYG_HFS_STATUS_CLIENT_DISCONNECT:
             //DBG_IND("client disconnect, time= %05d ms\r\n",xExamHfs_getCurrTime()/1000);
             break;

    }
}
int UINet_HFSUploadResultCb(int result, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType)
{
    XML_DefaultFormat(WIFIAPP_CMD_UPLOAD_FILE,result,bufAddr,(cyg_uint32 *)bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int UINet_HFSDeleteResultCb(int result, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType)
{
    XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE,result,bufAddr,(cyg_uint32 *)bufSize,mimeType);
    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

#if(JSON_PARSER == ENABLE)
static INT32 UINet_HFSGetCustomData(CHAR* path, CHAR* argument, UINT32 bufAddr, UINT32* bufSize, CHAR* mimeType, UINT32 segmentCount)
{
    UINT32            bufAddress = bufAddr;
    char*             buf = (char*)bufAddress;

    DBG_DUMP("path = %s, argument -> %s, mimeType= %s, buf = 0x%x, bufsize= %d, segmentCount= %d\r\n",path,argument, mimeType, buf, *bufSize, segmentCount);

    if (!strncmp(argument, "fmt=thumb", strlen("fmt=thumb")))
    {
        DBG_DUMP("get thumbnail!!\r\n");
        return XML_GetThumbnail(path, argument, (cyg_uint32)bufAddr, (cyg_uint32 *)bufSize, mimeType, segmentCount);
    }

    return CYG_HFS_CB_GETDATA_RETURN_OK;
}

static INT32 UINet_HFSHeadCB(UINT32 headerAddr, UINT32 headerSize, CHAR* filepath, CHAR* mimeType, void *reserved)
{
    char *headerstr;

    headerstr = (char *)headerAddr;
    DBG_DUMP("http headerstr=%s",headerstr);

    if (!strncmp(headerstr, "GET", strlen("GET")))
    {
        if (strstr(headerstr, "?fmt=thumb") != NULL)
        {
            return CYG_HFS_CB_HEADER_RETURN_CUSTOM;
        }

        return CYG_HFS_CB_HEADER_RETURN_DEFAULT;
    }

    DBG_DUMP("Unknown http method\r\n");
    return CYG_HFS_CB_HEADER_RETURN_ERROR;
}
#endif


void Net_DhcpCliCb(NET_DHCP_CB_TYPE cbType, unsigned int p1, unsigned int p2, unsigned int p3)
{
    DBG_IND("::type=%d,P=%d, %d, %d\r\n",cbType, p1, p2, p3);
    if (NET_DHCP_CB_TYPE_CLI_REQ_CNT == cbType)
    {
        //post an event to UI-task show (cycle,count) that client request an IP
    }
    else if (NET_DHCP_CB_TYPE_CLI_REQ_RESULT == cbType)
    {
        if (p1)
        {
            DBG_DUMP("DHCP Client IP = 0x%x\r\n",p1);
            //post an event to UI-task to call,Dhcp_Client_Start_Ok(p1) for the first time.
            //post an event to UI-task to show Got-IP
            Ux_PostEvent(NVTEVT_WIFI_AUTHORIZED_OK, 0);
        }
        else
        {
            DBG_DUMP("DHCP Client IP Fail\r\n");
            g_CliConnected = FALSE;
            //post an event to UI-task to Close DHCP Client,Dhcp_Client_Stop(), and show Get-IP fail
        }
    }
    else if (NET_DHCP_CB_TYPE_MAX == cbType)
    {
        OS_DumpKernelResStatus();
    }
}

//////////////////////////////////////////////////////////////////////////
BOOL UINet_GetStationConnected(void)
{
    return g_CliConnected;
}
char* UINet_GetConnectedMAC(void)
{
    return currentMacAddr;
}
void UINet_SetMode(NET_MODE_SETTING mode)
{
    gMode = mode;
}
UINT32 UINet_GetMode(void)
{
    return gMode;
}
void UINet_SetAuthType(UINT32 authtype)
{
    gAuthType = authtype;
}
UINT32 UINet_GetAuthType(void)
{
    return gAuthType;
}
char* UINet_GetSSID(void)
{
    return gSSID;
}
char* UINet_GetDefaultAPSSID(void)
{
    return gSSID_AP_default;
}
char* UINet_GetPASSPHRASE(void)
{
    return gPASSPHRASE;
}
char* UINet_GetDefaultAPPASSPHRASE(void)
{
    return gPASSPHRASE_AP_default;
}
char* UINet_GetIP(void)
{
    return gCurrIP;
}
char* UINet_GetMAC(void)
{
    return gMacAddr;
}
void UINet_SetSSID(char *ssid,UINT32 len)
{
    if(len>WSC_MAX_SSID_LEN)
    {
        DBG_ERR("max len %d\r\n",WSC_MAX_SSID_LEN);
        len = WSC_MAX_SSID_LEN;
    }
    memset(gSSID,'\0',WSC_MAX_SSID_LEN);
    sprintf(gSSID,ssid,len);
    DBG_IND("%s\r\n",gSSID);
}
void UINet_SetPASSPHRASE(char *pwd,UINT32 len)
{

    if(len>MAX_WEP_KEY_LEN)
    {
        DBG_ERR("max len %d\r\n",MAX_WEP_KEY_LEN);
        len = MAX_WEP_KEY_LEN;
    }
    memset(gPASSPHRASE,'\0',MAX_WEP_KEY_LEN);
    strncpy(gPASSPHRASE,pwd,len);
    DBG_IND("%s\r\n",gPASSPHRASE);

}
//0 for auto channel, 1 -14 for 11b/11g,36 -165 for 11a
void UINet_SetChannel(UINT32 channel)
{
    gChannel=channel;
}
UINT32 UINet_GetChannel(void)
{
    return gChannel;
}

void UINet_ResetPASSPHRASE(void)
{
    UINet_SetPASSPHRASE(PASSPHRASE_DEFAULT, strlen(PASSPHRASE_DEFAULT));
}

void UINet_ResetSSID(void)
{
    memset(currentInfo.strSSID,'\0',NVT_WSC_MAX_SSID_LEN);
    sprintf(currentInfo.strSSID,"%s%s", AP_NAME_STRING,EngineerMode_ReadAPMAC());
}

#if(_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_NVT_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_BRCM_)

static int IS_MCAST(unsigned char *da)
{
    if ((*da) & 0x01)
        return 1;
    else
        return 0;
}

void UINet_RemoveUser(void)
{
    UINT32 bNewUser = FALSE;

    CHKPNT;
    //if 2nd user connect,diconnect 1st user
    if(strncmp(currentMacAddr,gLastMacAddr,strlen(currentMacAddr))!=0 )
    {
        //notify current connected socket
        //new connect not get IP and socket not create
        //cannot disconnet immediate,suggest after 500 ms
        WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_REMOVE_BY_USER);
        bNewUser = TRUE;
    }

    Ux_PostEvent(NVTEVT_WIFI_AUTHORIZED_OK, 1,bNewUser);
}
void UINet_AddACLTable(void)
{
    if(strncmp(currentMacAddr,gLastMacAddr,strlen(currentMacAddr))!=0 )
    {
        if(strlen(gRemoveMacAddr)!=0)
        {
            DBG_ERR("%s not remove from ACL !!\r\n",gRemoveMacAddr);
        }

        strncpy(gRemoveMacAddr,gLastMacAddr,strlen(gLastMacAddr));
        Wifi_HAL_Run_Cmd("iwpriv", "wlan0", "add_acl_table", gRemoveMacAddr);
        strncpy(gLastMacAddr,currentMacAddr,strlen(currentMacAddr));
        DBG_IND("********** gLastMacAddr    %s\r\n",gLastMacAddr);
        DBG_IND("********** gRemoveMacAddr  %s\r\n",gRemoveMacAddr);

        BKG_PostEvent(NVTEVT_BKW_WIFI_CLEARACL);

    }
}
void UINet_ClearACLTable(void)
{
    if(strlen(gRemoveMacAddr))
    {
        DBG_IND("**********clr gRemoveMacAddr  %s\r\n",gRemoveMacAddr);
        Wifi_HAL_Run_Cmd("iwpriv", "wlan0", "rm_acl_table", gRemoveMacAddr);
        memset(gRemoveMacAddr,0,strlen(gRemoveMacAddr));
    }
}


void UINet_StationStatus_CB(char *pIntfName, char *pMacAddr, int status)
{
    //#NT#2013/12/06#KS Hung -begin
    //#NT#Post AUTHORIZED_OK event
    if (status == NVT_WIFI_STA_STATUS_ASSOCIATED) {

    DBG_IND("ASSOCIATED %d ms\r\n",Perf_GetCurrent()/1000);

        DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is associated\r\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == NVT_WIFI_STA_STATUS_REASSOCIATED) {
        DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is reassociated\r\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == NVT_WIFI_STA_STATUS_DISASSOCIATED) {
        DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is disassociated\r\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));

        Ux_PostEvent(NVTEVT_WIFI_DEAUTHENTICATED, 0);
    }
    else if (status == NVT_WIFI_STA_STATUS_DEAUTHENTICATED) {
        DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is deauthenticated\r\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));

        Ux_PostEvent(NVTEVT_WIFI_DEAUTHENTICATED, 0);
    }
    else if (status == NVT_WIFI_STA_STATUS_PORT_AUTHORIZED) {

        DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is port authorized\r\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));

        {
            sprintf(currentMacAddr,"%02x%02x%02x%02x%02x%02x",
            *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
            DBG_IND("********** currentMacAddr  %s\r\n",currentMacAddr);

            if(strlen(gLastMacAddr)==0)
            {
                strncpy(gLastMacAddr,currentMacAddr,strlen(currentMacAddr));
                DBG_IND("********** gLastMacAddr    %s\r\n",gLastMacAddr);
            }

            //#NT#2016/03/23#Isiah Chang -begin
            //#NT#add new Wi-Fi UI flow.
            #if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
            WifiCmd_ReceiveCmd(TRUE); // Ready to receive Wi-Fi APP command.
            #endif
            //#NT#2016/03/23#Isiah Chang -end
            Ux_PostEvent(NVTEVT_WIFI_AUTHORIZED_OK, 1,0);
        }

    }
    else if (status == NVT_WIFI_AP_READY) {
        DBG_IND("AP ready\r\n");
        DBG_ERR("AP ready %d ms\r\n",Perf_GetCurrent()/1000);

    }
    //#NT#2016/03/23#Isiah Chang -begin
    //#NT#add new Wi-Fi UI flow.
    else if (status == NVT_WIFI_STA_STATUS_AUTH_FAIL) {
        DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is disassociated\r\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));

        Ux_PostEvent(NVTEVT_WIFI_DEAUTHENTICATED, 0);
    }
    //#NT#2016/03/23#Isiah Chang -end
    else
        DBG_IND("unknown status %d\r\n",status);
    //#NT#2013/12/06#KS Hung -end

    Ux_PostEvent(NVTEVT_WIFI_STATE, 2, NET_STATE_WIFI_CB,status);

}

void UINet_Link2APStatus_CB(char *pIntfName, int status)
{
    if (status == NVT_WIFI_LINK_STATUS_CONNECTED) {
        DBG_IND("%s: Connected with AP\r\n", pIntfName);
        #if 0 // Unused code. Because new Wi-Fi driver has fixed this issue.
        Wifi_HAL_Run_Cmd("ifconfig", "wlan0", gCurrIP);  // Issue ARP Request to router.
        #endif
        if (strcmp(wifiConfig.mode, "sta") == 0){
            Ux_PostEvent(NVTEVT_EXE_WIFI_DHCP_START,1,1);
        }
        //#NT#2016/03/23#Isiah Chang -begin
        //#NT#add new Wi-Fi UI flow.
        #if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
        WifiCmd_ReceiveCmd(TRUE); // Ready to receive Wi-Fi APP command.
        #endif
        Ux_PostEvent(NVTEVT_WIFI_AUTHORIZED_OK, 0);
        //#NT#2016/03/23#Isiah Chang -end
    }
    else if (status == NVT_WIFI_LINK_STATUS_DISCONNECTED) {
        DBG_IND("%s: Disconnected with AP\r\n", pIntfName);
        g_CliConnected = FALSE;
        Ux_PostEvent(NVTEVT_WIFI_DEAUTHENTICATED, 0);
    }
    else if (status == NVT_WIFI_LINK_STATUS_DEAUTHENTICATED) {
        DBG_IND("%s: Deauthenticated\r\n", pIntfName);
        g_CliConnected = FALSE;
        Ux_PostEvent(NVTEVT_WIFI_DEAUTHENTICATED, 0);
    }
    else if (status == NVT_WIFI_LINK_STATUS_SACN_ZREO_RESULT) {
        DBG_IND("%s: ZREO\r\n", pIntfName);
        g_CliConnected = FALSE;
        Ux_PostEvent(NVTEVT_WIFI_DEAUTHENTICATED, 0);
    }
    else
        DBG_IND("unknown status %d \r\n",status);

    Ux_PostEvent(NVTEVT_WIFI_STATE, 2, NET_STATE_WIFI_CB,status);

}

void UINet_P2PEvent_CB(char *pIntfName,  int status)
{
    if (status==NVT_WIFI_P2P_EVENT_BACK2DEV) {
        Ux_PostEvent(NVTEVT_EXE_WIFI_BACK2DEV,0);
    }
    else if (status==NVT_WIFI_P2P_EVENT_START_DHCPD) {
        Ux_PostEvent(NVTEVT_EXE_WIFI_DHCP_START,1,0);
    }
    else if(status==NVT_WIFI_P2P_EVENT_START_DHCPC) {
        Ux_PostEvent(NVTEVT_EXE_WIFI_DHCP_START,1,1);
    }
    else if(status==NVT_WIFI_P2P_EVENT_INDICATE_PBC_REQ) {

        DBG_ERR("PCB request,do you want to accept?\r\n");
    }
}

int UINet_WSCFlashParam_CB(nvt_wsc_flash_param *param)
{
    nvt_wifi_settings *pwifi=&wifiConfig;

    pwifi->is_configured = 1;
    strcpy(pwifi->ssid, param->SSID);
    DBG_IND("new SSID %s \r\n", pwifi->ssid);

    pwifi->config_by_ext_reg = param->WSC_CONFIGBYEXTREG;
    if (param->WEP) {
        if (param->WEP == 1) //WEP64
            strcpy(pwifi->security, "wep64-auto");
        else
            strcpy(pwifi->security, "wep128-auto");

        pwifi->wep_default_key = param->WEP_DEFAULT_KEY;
        strcpy(pwifi->wep_key1, param->WEP_KEY1);
        strcpy(pwifi->wep_key2, param->WEP_KEY1);
        strcpy(pwifi->wep_key3, param->WEP_KEY1);
        strcpy(pwifi->wep_key4, param->WEP_KEY1);
    }
    else {
        if (param->WSC_AUTH==WSC_AUTH_OPEN) {
            if (param->WSC_ENC==WSC_ENCRYPT_NONE) {
                strcpy(pwifi->security, "none");
            }
        }
        else if (param->WSC_AUTH==WSC_AUTH_WPAPSK) {
            if (param->WSC_ENC==WSC_ENCRYPT_TKIP)
                strcpy(pwifi->security, "wpa-psk-tkip");
            else if (param->WSC_ENC==WSC_ENCRYPT_AES)
                strcpy(pwifi->security, "wpa-psk-aes");
            else if (param->WSC_ENC==WSC_ENCRYPT_TKIPAES)
                strcpy(pwifi->security, "wpa-psk-tkipaes");
            strcpy(pwifi->passphrase, param->WSC_PSK);
        }
        else if (param->WSC_AUTH==WSC_AUTH_WPA2PSK) {
            if (param->WSC_ENC==WSC_ENCRYPT_TKIP)
                strcpy(pwifi->security, "wpa2-psk-tkip");
            else if (param->WSC_ENC==WSC_ENCRYPT_AES)
                strcpy(pwifi->security, "wpa2-psk-aes");
            else if (param->WSC_ENC==WSC_ENCRYPT_TKIPAES)
                strcpy(pwifi->security, "wpa2-psk-tkipaes");
            strcpy(pwifi->passphrase, param->WSC_PSK);
        }
        else if (param->WSC_AUTH==WSC_AUTH_WPA2PSKMIXED) {
            if (param->WSC_ENC==WSC_ENCRYPT_TKIP)
                strcpy(pwifi->security, "wpa-auto-psk-tkip");
            else if (param->WSC_ENC==WSC_ENCRYPT_AES)
                strcpy(pwifi->security, "wpa-auto-psk-aes");
            else if (param->WSC_ENC==WSC_ENCRYPT_TKIPAES)
                strcpy(pwifi->security, "wpa-auto-psk-tkipaes");
            strcpy(pwifi->passphrase, param->WSC_PSK);
        }
    }

    if (strcmp(pwifi->mode, "p2pdev") == 0) {
    DBG_IND("change p2pmode  %d\r\n",param->WLAN0_P2P_TYPE);
    pwifi->p2pmode = param->WLAN0_P2P_TYPE;
    }

    return 0;
}

void UINet_WSCEvent_CB(int evt, int status)
{
    DBG_IND("evt %d\r\n",evt);
    if (evt == WPS_REINIT_EVENT) {

        Ux_PostEvent(NVTEVT_EXE_WIFI_REINIT,0);
    }
    else if (evt == WPS_STATUS_CHANGE_EVENT) {
        print_wscd_status(status);
    }
    else
        DBG_IND("unknown event\n");
}
void UINet_DhcpCb(DHCPD_CLI_STS cliSts, dhcp_assign_ip_info *ipInfo, dhcpd_lease_tbl_loc *pTbl)
{
    switch(cliSts)
    {
    case DHCPD_CLI_STS_RELEASE_IP:
		debug_msg("^G UINet_DhcpCb RELEASE_IP\r\n");	
        break;
    case DHCPD_CLI_STS_REQUEST_IP:
		debug_msg("^G UINet_DhcpCb REQUEST_IP\r\n");	
        DBG_IND("REQUEST_IP %d ms\r\n",Perf_GetCurrent()/1000);
        break;
    case DHCPD_CLI_STS_DECLINE_IP:
        break;
    case DHCPD_CLI_STS_SRVSTART:
        break;
    case DHCPD_CLI_STS_SRVREADY:
        break;
    default:
        break;
    }
}

BOOL UINet_SetFixLeaseInfo(UINT32 sec,UINT32 cnt)
{
    NET_DHCP_LEASE_CONF leaseInfo;

    memset((void *)&leaseInfo, 0, sizeof(NET_DHCP_LEASE_CONF));//
    memcpy((void *)leaseInfo.ip_pool_start, EXAM_NET_LEASE_START_WLAN0, strlen(EXAM_NET_LEASE_START_WLAN0));
    memcpy((void *)leaseInfo.ip_pool_end, EXAM_NET_LEASE_END_WLAN0, strlen(EXAM_NET_LEASE_END_WLAN0));
    memcpy((void *)leaseInfo.ip_dns, EXAM_NET_LEASE_DNS_WLAN0, strlen(EXAM_NET_LEASE_DNS_WLAN0));
    memcpy((void *)leaseInfo.ip_wins, EXAM_NET_LEASE_WINS_WLAN0, strlen(EXAM_NET_LEASE_WINS_WLAN0));
    memcpy((void *)leaseInfo.ip_server, EXAM_NET_SRVIP_WLAN0, strlen(EXAM_NET_SRVIP_WLAN0));
    memcpy((void *)leaseInfo.ip_gateway, EXAM_NET_GATEWAY_WLAN0, strlen(EXAM_NET_GATEWAY_WLAN0));
    memcpy((void *)leaseInfo.ip_subnetmask, EXAM_NET_NETMASK_WLAN0, strlen(EXAM_NET_NETMASK_WLAN0));
    memcpy((void *)leaseInfo.ip_broadcast, EXAM_NET_BRAODCAST_WLAN0, strlen(EXAM_NET_BRAODCAST_WLAN0));
    leaseInfo.lease_time = (unsigned int)sec;
    leaseInfo.max_lease_cnt = (unsigned int)cnt;
    Dhcp_Conf_Lease(&leaseInfo);

    return TRUE;
}

//#NT#2016/10/27#YongChang Qui-begin
//#NT#After switching to station mode, tell novacam about station ip
#if (AUTO_AP_TO_STA == ENABLE)
static INT32 g_change_mode_response = 0;
static int change_mode_recv(char* addr, int size)
{
    if(addr && (size>0))
    {
        DBG_IND("peer respond with %d bytes\r\n",size);
        g_change_mode_response = 1;
    }
    return TRUE;
}

static void pass_ip_to_peer(char *myIP)
{
#define MAX_RETRY 30
    int                     retry = 0, size, result;
    usocket_cli_obj         usocketCliObj={0};
    usocket_cli_obj         *pNewObj = 0;
    char                    msg[64];
    unsigned int            serverIP = 0;
    char                    server_ip[NET_IP_MAX_LEN] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    Dhcp_Client_Get_ServerIP(&serverIP);
    snprintf(server_ip, NET_IP_MAX_LEN, "%d.%d.%d.%d",(serverIP&0xFF), (serverIP>>8)&0xFF,(serverIP>>16)&0xFF,(serverIP>>24)&0xFF );

    strncpy(msg, myIP, sizeof(msg));
    strncat(msg, "\r\n", sizeof(msg));
    g_change_mode_response = 0;

    usocketCliObj.recv = change_mode_recv;
    strcpy(usocketCliObj.svrIP, server_ip);
    usocketCliObj.portNum = 7777;
    usocketCliObj.sockbufSize = 64;
    usocketCliObj.timeout = 10;
    pNewObj = usocket_cli_get_newobj();
    if(pNewObj)
    {
        usocket_cli_install(pNewObj,&usocketCliObj);
        result = usocket_cli_connect(pNewObj);
        if(result==0)
            usocket_cli_start();
        else{
            DBG_ERR("can't start usocket client to %s\r\n", server_ip);
            goto out1;
        }
    }
    else{
        DBG_ERR("usocket_cli_get_newobj() fail\r\n");
        goto out2;
    }

    DBG_IND("Passing ip[%s] to peer[%s]\r\n", myIP, server_ip);
    while(!g_change_mode_response && retry < MAX_RETRY){
        size = strlen(msg);
        usocket_cli_send(pNewObj,msg,&size);
        Delay_DelayMs(1000);
        retry++;
    }

    //usocket_cli_disconnect(pNewObj);

out1:
    usocket_cli_stop();

out2:
    if(!g_change_mode_response){
        DBG_ERR("peer doesn't respond. return to ap mode\r\n");
        Ux_SendEvent(0,NVTEVT_EXE_WIFI_STOP,0);
        Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_WIFI_MODE,1,NET_AP_MODE);
        BKG_PostEvent(NVTEVT_BKW_WIFI_ON); // Enable WIFI interface
    }
}
#endif
//#NT#2016/10/27#YongChang Qui-end

UINT32 UINet_DHCP_Start(UINT32 isClient)
{
    UINT32 result = E_OK;

    DBG_IND("isCli %d\r\n",isClient);

    if(isClient)
    {
        unsigned int gotIP = 0;

        Wifi_HAL_ignore_down_up("wlan0", 1);

        //make sure dhcpc has closed
        Dhcp_Client_Stop();
        //make sure dhcpd has closed
        Dhcp_Server_Stop(0);
        //start dhcpc

        //set DHCP client
        result = Dhcp_Client_Start(gNvtDhcpHostNameCli, Net_DhcpCliCb, TRUE);
        if (NET_DHCP_RET_OK != result )
        {
            DBG_ERR("Dhcp cli fail %d\r\n",result);
        }
        else
        {
            Dhcp_Client_Get_IP(&gotIP);
            sprintf(gCurrIP,"%d.%d.%d.%d",(gotIP&0xFF), (gotIP>>8)&0xFF,(gotIP>>16)&0xFF,(gotIP>>24)&0xFF );
            DBG_IND("ip=0x%x %s\r\n",gotIP,gCurrIP);
            g_CliConnected = TRUE;
            reconnect_count = 0;
            SxTimer_SetFuncActive(SX_TIMER_NET_CONNECT_ID,FALSE);

#if (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189_) || (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189xtal26_)|| (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189ftv_)
            wifi_set_passwd_hash_delay(0); //YongChang// Reset hash delay time once got IP.
            DBG_IND("^Bhash delay time:%d ms\r\n", wifi_get_passwd_hash_delay());//YongChang
#endif
        }
        Wifi_HAL_ignore_down_up("wlan0", 0);

        //#NT#2016/10/27#YongChang -begin
        //#NT#After switching to station mode, tell novacam about station ip
        #if (AUTO_AP_TO_STA == ENABLE)
        pass_ip_to_peer(gCurrIP);
        #endif
        //#NT#2016/10/27#YongChang -end
    }
    else
    {
        //set DHCP server
        Dhcp_Set_Interface("wlan0");
        dhcpd_reg_cli_attach_cb_func(UINet_DhcpCb);
        UINet_SetFixLeaseInfo(30, DHCP_LEASE_CNT);  // Limit user number to be allowed to get IP.
        result = Dhcp_Server_Start(gNvtDhcpHostNameSrv);
        if (NET_DHCP_RET_OK != result)
        {
            DBG_ERR("Dhcp svr fail\r\n");
        }
    }
    Ux_PostEvent(NVTEVT_WIFI_STATE, 2, NET_STATE_DHCP_START,result);

    return result;
}
#if 0
void UINet_dump_wifi_settings(void)
{
    printf("wlan_disable=%d\n", wifiConfig.wlan_disable);
    printf("wps_disable=%d\n", wifiConfig.wps_disable);
    printf("mode=%s\n", wifiConfig.mode);
    if (strcmp(wifiConfig.mode, "sta") == 0)
        printf("is_dhcp=%d\n", wifiConfig.is_dhcp);
    printf("security=%s\n", wifiConfig.security);
    printf("channel=%d\n", wifiConfig.channel);
    printf("ssid=%s\n", wifiConfig.ssid);
    printf("passphrase=%s\n", wifiConfig.passphrase);
    printf("wep_default_key=%d\n", wifiConfig.wep_default_key);
    printf("wep_key1=%s\n", wifiConfig.wep_key1);
    printf("wep_key2=%s\n", wifiConfig.wep_key2);
    printf("wep_key3=%s\n", wifiConfig.wep_key3);
    printf("wep_key4=%s\n", wifiConfig.wep_key4);
    printf("wsc_pin=%s\n", wifiConfig.wsc_pin);
    printf("is_configured=%d\n", wifiConfig.is_configured);
    printf("config_by_ext_reg=%d\n", wifiConfig.config_by_ext_reg);
    printf("p2pmode=%d\n", wifiConfig.p2pmode);
}
#endif

cyg_thread_info tinfo;
static int UINet_get_thread_info_by_name(char *name, cyg_thread_info *pinfo)
{
    cyg_handle_t thandle = 0;
    cyg_uint16 id = 0;

    while (cyg_thread_get_next(&thandle, &id)) {
        cyg_thread_get_info(thandle, id, pinfo);
        //printf("ID: %04x name: %10s pri: %d\n",
                //  pinfo->id, pinfo->name ? pinfo->name:"----", pinfo->set_pri);
        if (strcmp(name, pinfo->name)==0)
            return 1;
    }
    return 0;
}

void UINet_WifiSettings(nvt_wifi_settings *pwifi,UINT32 mode,UINT32 security)
{
    if (mode == NET_AP_MODE) {
        strcpy(pwifi->mode,"ap");
        if(currentInfo.strSSID[0] == 0)
        {
#if (MAC_APPEN_SSID==ENABLE)
        sprintf(pwifi->ssid,"%s%02x%02x%02x%02x%02x%02x",gSSID, gMacAddr[0], gMacAddr[1], gMacAddr[2],
            gMacAddr[3], gMacAddr[4], gMacAddr[5]);
#else
        sprintf(pwifi->ssid,"%s",gSSID);
#endif
        }
        else
        {
            sprintf(pwifi->ssid,"%s",gSSID);
        }

        strcpy(pwifi->device_name, "Wireless AP");
    }

    else if(mode == NET_STATION_MODE){
        strcpy(pwifi->mode,"sta");
        sprintf(pwifi->ssid,"%s",gSSID);
        strcpy(pwifi->device_name, "Wireless STA");
    }
    else if (mode == NET_P2P_DEV_MODE){
        pwifi->p2pmode = NVT_P2P_DEVICE;
        strcpy(pwifi->mode,"p2pdev");
        /*p2p note ; p2p GO's SSID must in the form "DIRECT-xy-anystring"  xy is random*/
        strcpy(pwifi->ssid, "DIRECT-de-12345678");
        strcpy(pwifi->device_name, "p2p-dev");
    }
    else if (mode == NET_P2P_GO_MODE){
        pwifi->p2pmode = NVT_P2P_TMP_GO;
        strcpy(pwifi->mode,"p2pgo");
        /*p2p note ; p2p GO's SSID must in the form "DIRECT-xy-anystring"  xy is random*/
        strcpy(pwifi->ssid, "DIRECT-go-12345678");
        pwifi->is_configured = 1;            // let wscd under configured mode
        strcpy(pwifi->device_name, "p2p-go");
    }
    else{
        DBG_ERR("unknown mode %d\r\n",mode);
    }

    if(security == NET_AUTH_NONE)
    {
        strcpy(pwifi->security,"none");
    }
    else if(security == NET_AUTH_WPA2)
    {
        strcpy(pwifi->security,"wpa2-psk-aes");
    }
    else if(security == NET_AUTH_WPA)
    {
        strcpy(pwifi->security,"wpa-psk-tkip");
    }
    else
    {
        DBG_ERR("unknown security %d\r\n",security);
    }

    pwifi->channel = gChannel;

    strcpy(pwifi->passphrase, gPASSPHRASE);

    pwifi->wep_default_key = 0;
    if (strcmp(pwifi->security, "wep64-auto") == 0) {
        strcpy(pwifi->wep_key1, "1234567890");
        strcpy(pwifi->wep_key2, "1234567890");
        strcpy(pwifi->wep_key3, "1234567890");
        strcpy(pwifi->wep_key4, "1234567890");
    }
    else {
        strcpy(pwifi->wep_key1, "12345678901234567890123456");
        strcpy(pwifi->wep_key2, "12345678901234567890123456");
        strcpy(pwifi->wep_key3, "12345678901234567890123456");
        strcpy(pwifi->wep_key4, "12345678901234567890123456");
    }
}

void UINet_WifiP2PReInit(void)
{
    nvt_wifi_settings *pwifi = &wifiConfig;

    CHKPNT;
    Wifi_HAL_Config(pwifi);

    if (strcmp(pwifi->mode, "ap") == 0) {
            Wifi_HAL_Interface_Config("wlan0", gCurrIP, "255.255.255.0");
    }
    else if (strcmp(pwifi->mode, "sta") == 0) {
            Wifi_HAL_Interface_Delete("wlan0");
            Wifi_HAL_Interface_Up("wlan0");
    }
    else if (strcmp(pwifi->mode, "p2pgo") == 0) {
        if (pwifi->p2pmode==NVT_P2P_TMP_GO) {
            Wifi_HAL_Interface_Delete("wlan0");
            memcpy((void *)gCurrIP, EXAM_NET_IP_WLAN0, NET_IP_MAX_LEN);
            Wifi_HAL_Interface_Config("wlan0", gCurrIP, "255.255.255.0");
            DBG_IND(" setip=%s -\r\n", gCurrIP);
            UINet_DHCP_Start(FALSE);
        }
    }
    else if (strcmp(pwifi->mode, "p2pdev") == 0) {
        if (pwifi->p2pmode==NVT_P2P_DEVICE) {
            Wifi_HAL_Interface_Delete("wlan0");
            Wifi_HAL_Interface_Up("wlan0");
        }
        else if (pwifi->p2pmode==NVT_P2P_CLIENT) {
            Wifi_HAL_Interface_Delete("wlan0");
            Wifi_HAL_Interface_Up("wlan0");
        }
    }

    if (UINet_get_thread_info_by_name("wscd", &tinfo)) {
        if (!pwifi->wlan_disable && !pwifi->wps_disable)
        {
            Wifi_HAL_wsc_reinit();
            DBG_IND("wsc_reinit\r\n");
        }
        else
        {
            Wifi_HAL_wsc_stop_service();
            DBG_IND("wsc_stop_service\r\n");
        }
    }
    else {
            DBG_IND("wsc_stop_service\r\n");
        if (!pwifi->wlan_disable && !pwifi->wps_disable)
            Wifi_HAL_create_wscd();
    }

}

void UINet_WifiBack2Dev(void)
{
    nvt_wifi_settings *pwifi=&wifiConfig;
    CHKPNT;
    UINet_WifiSettings(pwifi,NET_P2P_DEV_MODE,NET_AUTH_NONE);
    Ux_PostEvent(NVTEVT_EXE_WIFI_REINIT,0);

}
INT32 UINet_WifiInit(UINT32 mode,UINT32 security)
{
    UINT32 result=0;
    static BOOL bInit = FALSE;
    nvt_wifi_settings *pwifi=&wifiConfig;

    DBG_IND("set wifi %d  %d \r\n",mode,security);

    DrvWiFi_PowerOn();

    memset(pwifi,0,sizeof(nvt_wifi_settings));

    if(!bInit)
    {
        result = Wifi_HAL_Init(0);
        if (result!=0 )
        {
            DBG_ERR("wifi init fail %d\r\n",result);
            return E_SYS;
        }
        else
        {
            DBG_IND("wifi init succesful \r\n");
        }
        bInit = TRUE;
    }

    //register sta status cb
    Wifi_HAL_RegisterStaCB(WIFI_IFNAME, UINet_StationStatus_CB);

    //register link status cb
    Wifi_HAL_RegisterLinkCB(WIFI_IFNAME, UINet_Link2APStatus_CB);
    Wifi_HAL_RegisterWSCCB(UINet_WSCEvent_CB);
    Wifi_HAL_RegisterWSCFlashCB(UINet_WSCFlashParam_CB);
#if defined(__ECOS) && ((_WIFI_MODULE_ == _WIFI_MODULE_RTL8189_) || (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189xtal26_)|| (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189ftv_))
    Wifi_HAL_RegisterP2PCB("wlan0", UINet_P2PEvent_CB);
#endif

    if (Wifi_HAL_Get_Mac(gMacAddr) < 0)
    {
        DBG_IND("Wifi_HAL_Get_Mac() fail. Use hardcoded mac.\r\n");
        Wifi_HAL_Set_Mac("wlan0", "\x00\x00\x00\x81\x89\xe5");
    }
    else
    {
        unsigned char zero[] = {0, 0, 0, 0, 0, 0};
        DBG_IND("wlan0 efuse mac [%02x:%02x:%02x:%02x:%02x:%02x]\r\n",
        gMacAddr[0], gMacAddr[1], gMacAddr[2],
        gMacAddr[3], gMacAddr[4], gMacAddr[5]);
        //sanity check
        if (!memcmp(gMacAddr, zero, NVT_WIFIIPC_MAC_ADDR_LEN) || IS_MCAST((unsigned char *)gMacAddr))
        {
            DBG_ERR("efuse mac format is invalid. Use hardcoded mac.\r\n");
            Wifi_HAL_Set_Mac("wlan0", "\x00\x00\x00\x81\x89\xe5");
            sprintf(gMacAddr,"%c%c%c%c%c%c",0,0, 0,0x81,0x89,0xe5);
            DBG_ERR("default wlan0 efuse mac [%02x:%02x:%02x:%02x:%02x:%02x]\r\n",
            gMacAddr[0], gMacAddr[1], gMacAddr[2],
            gMacAddr[3], gMacAddr[4], gMacAddr[5]);
        }
        else
        {
            Wifi_HAL_Set_Mac("wlan0", gMacAddr);
        }
    }

    UINet_WifiSettings(pwifi,mode,security);

#if (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189_|| _WIFI_MODULE_ == _WIFI_MODULE_RTL8189ftv_)
    if(!strcmp(pwifi->mode, "sta"))
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "wsc_enable=0", NULL_STR);
#endif
    if (UINet_get_thread_info_by_name("wscd", &tinfo))
    {
        Wifi_HAL_wsc_reinit(); //trigger wsc to reinit
        DBG_IND("wsc_reinit\r\n");
    }
    else
    {
        Wifi_HAL_create_wscd();
        DBG_IND("create_wscd\r\n");
    }

    Wifi_HAL_generate_pin_code(pwifi->wsc_pin);

    DBG_IND("wsc_pin=%s\n", pwifi->wsc_pin);

    Wifi_HAL_Config(pwifi);

#if(WPS_SUPPORT ==1)
    if(security == NET_AUTH_NONE)
    {
        Wifi_HAL_wsc_reinit(); //trigger wsc to reinit
        DBG_IND("wsc_reinit\r\n");
    }
#endif


    if ((mode == NET_AP_MODE)|| (mode == NET_P2P_GO_MODE))
    {
        memcpy((void *)gCurrIP, EXAM_NET_IP_WLAN0, NET_IP_MAX_LEN);
        Wifi_HAL_Interface_Config("wlan0", gCurrIP, "255.255.255.0");
        DBG_IND(" setip=%s -\r\n", gCurrIP);
        result = UINet_DHCP_Start(FALSE);
    }
    else if (mode == NET_STATION_MODE)
    {
#if (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189_) || (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189xtal26_)|| (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189ftv_)
        // Set loop delay time of hash algorithm.
        wifi_set_passwd_hash_delay(10);//YongChang
        DBG_IND("^Bhash delay time:%d ms\r\n", wifi_get_passwd_hash_delay());//YongChang
#endif

        Wifi_HAL_Interface_Delete("wlan0");
        Wifi_HAL_Interface_Up("wlan0");
    }
    else if (mode == NET_P2P_DEV_MODE)
    {
        Wifi_HAL_Interface_Delete("wlan0");
        Wifi_HAL_Interface_Up("wlan0");
    }
    else
    {
       DBG_ERR("mode %d unknown\r\n",mode);
    }

    DBG_IND("Default IP=%s\r\n",gCurrIP);


    if(result==NET_DHCP_RET_OK)
    {
        result=UINet_HFSInit();
    }

    if (mode == NET_STATION_MODE){
        g_CliConnected = FALSE;
        reconnect_count = 0;
    }

    Ux_PostEvent(NVTEVT_WIFI_STATE, 2, NET_STATE_WIFI_START,result);

    return result;
}

INT32 UINet_WifiUnInit(UINT32 mode)
{
    UINT32 ret = 0;

    Wifi_HAL_Interface_Delete("wlan0");

    ret = Dhcp_Server_Stop(FALSE);

    ret = Dhcp_Client_Stop();

    Wifi_HAL_wsc_stop_service();//trigger wsc to stop service

    ret = Wifi_HAL_Interface_Down("wlan0");

    DrvWiFi_PowerOff();

    if(mode == NET_STATION_MODE){
        g_CliConnected = FALSE;
        reconnect_count = 0;
    }

    return ret;
}

void UINet_print_bss_desc(struct nvt_bss_desc *pBss)
{
    char ssidbuf[40], tmp2Buf[20], tmp3Buf[20];

    //BSSID
    DBG_DUMP("%02x:%02x:%02x:%02x:%02x:%02x",
        pBss->bssid[0], pBss->bssid[1], pBss->bssid[2],
        pBss->bssid[3], pBss->bssid[4], pBss->bssid[5]);

    //channel
    DBG_DUMP("  %3d", pBss->channel);

    //band
    tmp2Buf[0] = 0;
    if (pBss->network & NVT_WIRELESS_11N) {
        strcat(tmp2Buf, "N");
    }
    if (pBss->network & NVT_WIRELESS_11G) {
        strcat(tmp2Buf, "G");
    }
    if (pBss->network & NVT_WIRELESS_11B) {
        strcat(tmp2Buf, "B");
    }
    DBG_DUMP("%5s", tmp2Buf);

    //rssi
    DBG_DUMP(" %3d", pBss->rssi);

    //signal quality
    DBG_DUMP("   %3d ", pBss->sq);

    //Security
    memset(tmp3Buf,0x00,sizeof(tmp3Buf));

    // refer to get_security_info() for details
    if ((pBss->capability & nvt_cPrivacy) == 0) {
        strcpy(tmp2Buf, "");
        strcpy(tmp3Buf, "None");
    }
    else {
        if (pBss->t_stamp[0] == 0) {
            strcpy(tmp2Buf, "");
            strcpy(tmp3Buf, "WEP");
        }
        else {
            int wpa_exist = 0, idx = 0;
            if (pBss->t_stamp[0] & 0x0000ffff) {
                idx = sprintf(tmp2Buf,"%s","WPA");
                if (((pBss->t_stamp[0] & 0x0000f000) >> 12) == 0x4)
                    idx += sprintf(tmp2Buf+idx,"%s","-PSK");
                else if(((pBss->t_stamp[0] & 0x0000f000) >> 12) == 0x2)
                    idx += sprintf(tmp2Buf+idx,"%s","-1X");
                wpa_exist = 1;

                if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x5)
                    sprintf(tmp3Buf,"%s","AES/TKIP");
                else if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x4)
                    sprintf(tmp3Buf,"%s","AES");
                else if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x1)
                    sprintf(tmp3Buf,"%s","TKIP");
            }
            if (pBss->t_stamp[0] & 0xffff0000) {
                if (wpa_exist)
                    idx += sprintf(tmp2Buf+idx,"%s","/");
                idx += sprintf(tmp2Buf+idx,"%s","WPA2");
                if (((pBss->t_stamp[0] & 0xf0000000) >> 28) == 0x4)
                    idx += sprintf(tmp2Buf+idx,"%s","-PSK");
                else if (((pBss->t_stamp[0] & 0xf0000000) >> 28) == 0x2)
                    idx += sprintf(tmp2Buf+idx,"%s","-1X");

                if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x5)
                    sprintf(tmp3Buf,"%s","AES/TKIP");
                else if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x4)
                    sprintf(tmp3Buf,"%s","AES");
                else if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x1)
                    sprintf(tmp3Buf,"%s","TKIP");
            }
        }
    }
    DBG_DUMP("%-10s ", tmp3Buf);
    DBG_DUMP("%-16s ", tmp2Buf);

    //SSID
    memcpy(ssidbuf, pBss->ssid, pBss->ssidlen);
    ssidbuf[pBss->ssidlen] = '\0';
    DBG_DUMP("%s\r\n", ssidbuf);
}

void UINet_DumpAllSSID(NVT_SS_STATUS_Tp pStatus)
{
    int i;
    struct nvt_bss_desc *pBss;

    if(!pStatus)
    {
        DBG_ERR("no pStatus \r\n");
        return ;
    }

    DBG_DUMP("pStatus->number %d\r\n",pStatus->number);

    //success
    DBG_DUMP(" #      BSSID          ch band rssi  sq  enc        auth             SSID\r\n");
    for (i=0; i<pStatus->number && pStatus->number!=0xff; i++) {
        pBss = &pStatus->bssdb[i];
        DBG_DUMP("%2d ", i);
        UINet_print_bss_desc(pBss);
    }
    //free(pStatus);
    pStatus = NULL;

}
void UINet_SiteSurvey(char *ifname,NVT_SS_STATUS_Tp pStatus)
{
    int status;
    unsigned char res;
    int wait_time;

    //below is copied from formWlSiteSurvey()
    {
        // issue scan request
        wait_time = 0;
        while (1) {
            // ==== modified by GANTOE for site survey 2008/12/26 ====
            switch(Wifi_HAL_getWlSiteSurveyRequest(ifname, &status))
            {
                case -2:
                    DBG_ERR("Auto scan running!!please wait...\n");
                    goto ss_err;
                    break;
                case -1:
                    DBG_ERR("Site-survey request failed!\n");
                    goto ss_err;
                    break;
                default:
                    break;
            }

            if (status != 0) {  // not ready
                if (wait_time++ > 5) {
                    DBG_ERR("scan request timeout!\n");
                    goto ss_err;
                }
                //sleep(1);
                Delay_DelayMs(1000);
            }
            else
                break;
        }

        // wait until scan completely
        wait_time = 0;
        while (1) {
            res = 1;    // only request request status
            if (Wifi_HAL_getWlSiteSurveyResult(ifname, (NVT_SS_STATUS_Tp)&res,sizeof(res)) < 0 ) {
                DBG_ERR("Read site-survey status failed!");
                goto ss_err;
            }
            if (res == 0xff) {   // in progress
                if (wait_time++ > 10)
                {
                    DBG_ERR("scan timeout!");
                    goto ss_err;
                }
                //sleep(1);
                Delay_DelayMs(1000);
            }
            else
                break;
        }
    }

    pStatus->number = 0; // request BSS DB
    if (Wifi_HAL_getWlSiteSurveyResult(ifname, pStatus,sizeof(NVT_SS_STATUS_T)) < 0 ) {
        DBG_ERR("Read site-survey status failed!\n");
        //free(pStatus);
        pStatus = NULL;
        goto ss_err;
    }

    return;

ss_err:
    //failed
    //free(pStatus);
    pStatus = NULL;
}

void UINet_P2PDump(NVT_SS_STATUS_Tp pStatus)
{
    struct nvt_bss_desc *pBss;
    int i;

    DBG_DUMP("pStatus->number=%d\r\n",pStatus->number);
    DBG_DUMP(" # Channel Device address    Role WSCMethod Device Name\r\n");
    for (i=0; i<pStatus->number && pStatus->number!=0xff; i++) {
        pBss = &pStatus->bssdb[i];
        DBG_DUMP("%2d %3d     ", i, pBss->channel);
        DBG_DUMP("%02x:%02x:%02x:%02x:%02x:%02x ",
            pBss->p2paddress[0], pBss->p2paddress[1], pBss->p2paddress[2],
            pBss->p2paddress[3], pBss->p2paddress[4], pBss->p2paddress[5]);
        if(pBss->p2prole == 2) //2: device
            DBG_DUMP("DEV");
        else if(pBss->p2prole == 1) // 1:GO
            DBG_DUMP("GO ");
        else
            DBG_DUMP("???");
        DBG_DUMP("  0x%04x", pBss->p2pwscconfig);
        DBG_DUMP("     %s\r\n", pBss->p2pdevname);
    }
}
void UINet_P2PScan(char *ifname,NVT_SS_STATUS_Tp pStatus)
{
#if defined(__ECOS) && ((_WIFI_MODULE_ == _WIFI_MODULE_RTL8189_) || (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189xtal26_)|| (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189ftv_))
    int status;
    unsigned char res;
    int wait_time;

    // issue scan request
    wait_time = 0;
    while (1) {
        switch (Wifi_HAL_getWlP2PScanRequest(ifname, &status)) {
        case -2:
            DBG_ERR("Auto scan running!!please wait...\r\n");
            goto ss_err;
            break;
        case -1:
            DBG_ERR("Site-survey request failed!\r\n");
            goto ss_err;
            break;
        default:
            break;
        }

        if (status != 0) { // not ready
            if (wait_time++ > 5) {
                DBG_ERR("scan request timeout!\r\n");
                goto ss_err;
            }
            //sleep(1);
            Delay_DelayMs(1000);
        }
        else
            break;
    }

    // wait until scan completely
    wait_time = 0;
    while (1) {
        res = 1;
        if (Wifi_HAL_getWlP2PScanResult(ifname, (NVT_SS_STATUS_Tp)&res,sizeof(res)) < 0) {
            DBG_ERR("Read site-survey status failed!\r\n");
            goto ss_err;
        }

        if (res == 0xff) { // in progress
            if (wait_time++ > 20) {
                DBG_ERR("scan timeout!\r\n");
                goto ss_err;
            }
            //sleep(1);
            Delay_DelayMs(1000);
        }
        else
            break;
    }

    // get scan result
    pStatus->number = 0; // request BSS DB
    if ( Wifi_HAL_getWlP2PScanResult(ifname, pStatus,sizeof(NVT_SS_STATUS_T)) < 0 ) {
        DBG_ERR("Read site-survey status failed!\r\n");
        //free(pStatus);
        pStatus = NULL;
        return;
    }

ss_err:
    pStatus = NULL;
#endif
}

UINT32 UINet_P2PConnect(NVT_SS_STATUS_Tp pStatus,UINT32 idx)
{
    UINT32 result=0;
#if defined(__ECOS) && ((_WIFI_MODULE_ == _WIFI_MODULE_RTL8189_) || (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189xtal26_)|| (_WIFI_MODULE_ == _WIFI_MODULE_RTL8189ftv_))
    if ((idx < 0) || (idx >= pStatus->number)) {
        DBG_ERR("wifi p2pconnect <0~%d>\r\n", (pStatus->number-1));
    }
    else {
        NVT_P2P_PROVISION_COMM_T p2pProvisionComm;

        //p2pProvisionComm.wsc_config_method = CONFIG_METHOD_DISPLAY;
        //p2pProvisionComm.wsc_config_method = CONFIG_METHOD_KEYPAD;
        p2pProvisionComm.wsc_config_method = CONFIG_METHOD_PBC;
        p2pProvisionComm.channel = pStatus->bssdb[idx].channel;
        memcpy(p2pProvisionComm.dev_address, pStatus->bssdb[idx].p2paddress, 6);
        result = Wifi_HAL_sendP2PProvisionCommInfo("wlan0", &p2pProvisionComm);
        if ( result< 0) {
            DBG_ERR("SendP2PProvisionCommInfo fail\r\n");
        }
    }
#endif
    return result;
}
void UINet_P2PConfirm(void)
{
    Wifi_HAL_Run_Cmd("iwpriv", "wlan0", "p2pcmd", "pbcconf");
}

void WIFI_CliReconnect(void)
{
    nvt_wifi_settings *pwifi=&wifiConfig;

    if(strcmp(pwifi->mode, "sta"))
        return;

    if( WIFI_STA_RECONNECT_MAX_COUNT && reconnect_count >= WIFI_STA_RECONNECT_MAX_COUNT){
        SxTimer_SetFuncActive(SX_TIMER_NET_CONNECT_ID,FALSE);
        Ux_PostEvent(NVTEVT_WIFI_STATE, 2, NET_STATE_WIFI_CONNECTING,NET_CONNECTING_TIMEOUT);
        Wifi_HAL_Interface_Down("wlan0");
        return;
    }
    else
        reconnect_count++;

    if(SX_TIMER_NET_CONNECT_ID == -1)
        UINet_Reg_Timer();

    if(g_CliConnected == TRUE){
        SxTimer_SetFuncActive(SX_TIMER_NET_CONNECT_ID,FALSE);
        return;
    }

    Wifi_HAL_Interface_Down("wlan0");
    Wifi_HAL_Config(pwifi);
    Wifi_HAL_Interface_Up("wlan0");
    SxTimer_SetFuncActive(SX_TIMER_NET_CONNECT_ID,TRUE);
    return;
}

BOOL UINet_CliReConnect(void)
{
    nvt_wifi_settings *pwifi=&wifiConfig;
    if(strcmp(pwifi->mode, "sta"))
        return FALSE;

    if( WIFI_STA_RECONNECT_MAX_COUNT && reconnect_count >= WIFI_STA_RECONNECT_MAX_COUNT)
        return FALSE;
    else
        reconnect_count++;

    if(SX_TIMER_NET_CONNECT_ID == -1)
        UINet_Reg_Timer();

    if(g_CliConnected == TRUE){
        SxTimer_SetFuncActive(SX_TIMER_NET_CONNECT_ID,FALSE);
        return FALSE;
    }

    SxTimer_SetFuncActive(SX_TIMER_NET_CONNECT_ID,TRUE);
    return TRUE;
}

SX_TIMER_ITEM(WIFI_CliReconnect, WIFI_CliReconnect, WIFI_STA_RECONNECT_INTERVAL, FALSE)
void UINet_Reg_Timer()
{
    SX_TIMER_NET_CONNECT_ID = SxTimer_AddItem(&Timer_WIFI_CliReconnect);
}

#elif defined(__ECOS) && (_NETWORK_ == _NETWORK_SPI_EVB_ETHERNET_)
static NET_IP_PAIR gExamNetIP[NET_IP_PAIR_MAX_CNT] =
{
    //eth0
    {EXAM_NET_IP_ETH0,EXAM_NET_NETMASK_ETH0,EXAM_NET_BRAODCAST_ETH0,EXAM_NET_GATEWAY_ETH0,EXAM_NET_SRVIP_ETH0, 0},
    //eth1
    {EXAM_NET_IP_ETH1,EXAM_NET_NETMASK_ETH1,EXAM_NET_BRAODCAST_ETH1,EXAM_NET_GATEWAY_ETH1,EXAM_NET_SRVIP_ETH1, 1},
    //wlan0
    {EXAM_NET_IP_WLAN0,EXAM_NET_NETMASK_WLAN0,EXAM_NET_BRAODCAST_WLAN0,EXAM_NET_GATEWAY_WLAN0,EXAM_NET_SRVIP_WLAN0, 2}
};

INT32 UINet_LanInit(UINT32 mode)
{
    NET_DHCP_RET ret =0;

    DBG_IND("[SetupObj] set Lan %d\r\n",mode);

    if(mode == NET_AP_MODE) //ap mode
    {
        ret=Dhcp_Init_Network_With_IP(gExamNetIP, NET_IP_PAIR_MAX_CNT);
        if (NET_DHCP_RET_OK == ret)
        {
            ret=Dhcp_Server_Start(gNvtDhcpHostNameSrv);
            if(NET_DHCP_RET_OK !=ret )
            {
                DBG_ERR("AP fail %d\r\n",ret);
            }
        }
        else
        {
            DBG_ERR("eth0 fail %d\r\n",ret);
        }
    }
    else if(mode == NET_STATION_MODE) //station mode
    {
        //start DHCP client
        unsigned int gotIP = 0;
        ret=Dhcp_Client_Start(gNvtDhcpHostNameCli, Net_DhcpCliCb, TRUE);
        if (NET_DHCP_RET_OK != ret)
        {
            DBG_ERR("dhcp cli fail\r\n");
            return E_SYS;
        }
        else
        {
            Dhcp_Client_Get_IP(&gotIP);
            sprintf(gCurrIP,"%d.%d.%d.%d",(gotIP&0xFF), (gotIP>>8)&0xFF,(gotIP>>16)&0xFF,(gotIP>>24)&0xFF );
            DBG_IND("^R:::: ip=0x%x %s\r\n",gotIP,gCurrIP);

            #if 0
            debug_err(("%x \r\n",gotIP));
            debug_err(("%x \r\n",(gotIP&0xFF) ));
            debug_err(("%x \r\n",(gotIP>>8)&0xFF ));
            debug_err(("%x \r\n",(gotIP>>16)&0xFF ));
            debug_err(("%x \r\n",(gotIP>>24)&0xFF ));
            #endif

        }
    }
    else
    {
        ret = E_SYS;
        DBG_ERR("not support mode %d\r\n",mode);
    }

    if(ret==NET_DHCP_RET_OK)
    {
        ret=UINet_HFSInit();
    }

    return ret;
}



INT32 UINet_LanUnInit(UINT32 mode)
{
    UINT32 ret = 0;
    if(mode == NET_AP_MODE) //ap mode
        ret =Dhcp_Server_Stop(FALSE);
    else
        ret =Dhcp_Client_Stop();
    return ret;
}
void UINet_RemoveUser(void)
{
}
void UINet_AddACLTable(void)
{
}
void UINet_ClearACLTable(void)
{
}
void UINet_SiteSurvey(char *ifname,NVT_SS_STATUS_Tp pStatus)
{
}
void UINet_DumpAllSSID(NVT_SS_STATUS_Tp pStatus)
{
}
UINT32 UINet_DHCP_Start(UINT32 isClient)
{
    return 0;
}
void UINet_P2PDump(NVT_SS_STATUS_Tp pStatus)
{
}
void UINet_P2PScan(char *ifname,NVT_SS_STATUS_Tp pStatus)
{
}
UINT32 UINet_P2PConnect(NVT_SS_STATUS_Tp pStatus,UINT32 idx)
{
    return 0;
}
void UINet_P2PConfirm(void)
{
}
void UINet_WifiBack2Dev(void)
{
}
void UINet_WifiP2PReInit(void)
{
}
#endif

INT32 UINet_HFSInit(void)
{
    int err;

    cyg_FileSys_RegisterCB();
    err = mount( "", MOUNT_FS_ROOT, "nvtfs" );
    if( err < 0 ) DBG_ERR("mount fs fail %d\r\n",err);
    //hfs start
    {
        cyg_hfs_open_obj  hfsObj={0};

        // noify some status of HTTP server
        hfsObj.notify = UINet_HFSNotifyStatus;
        // if need to check user & password
        hfsObj.check_pwd = NULL;//xExamHfs_CheckPasswd;
        // set thread priority
        hfsObj.threadPriority = 6;//20;
        // set socket buffer
        hfsObj.sockbufSize = 51200;//50K speed up get file speed
        // set query callback
        hfsObj.clientQueryCb = WifiCmd_getQueryData;
        hfsObj.uploadResultCb = UINet_HFSUploadResultCb;
        hfsObj.deleteResultCb = UINet_HFSDeleteResultCb;
        // set query string
        strcpy(hfsObj.clientQueryStr, "query_devinfo");
        // set HFS root dir path
        strcpy(hfsObj.rootdir, MOUNT_FS_ROOT);
        #if(JSON_PARSER == DISABLE)
        // set port number
        hfsObj.portNum = 80;
        // register callback function of get XML data.
        hfsObj.getCustomData = WifiCmd_GetData;
        // register put data callback function.
        hfsObj.putCustomData = WifiCmd_PutData;
        hfsObj.headerCb      = NULL;
        #else
        // set port number
        hfsObj.portNum = 9090;
        // register callback function of get custom data.
        hfsObj.getCustomData = UINet_HFSGetCustomData;
        // set put data callback function as NULL.
        hfsObj.putCustomData = NULL;
        // register header data parse callback function.
        hfsObj.headerCb      = UINet_HFSHeadCB;
        #endif

        // start HFS
        cyg_hfs_open(&hfsObj);

    }

    WifiAppCmd_init();
    UserSocket_Open();
	UserSocket_Open_multi();
    #if (WIFI_FTP_FUNC==ENABLE)
    start_ftpserver();
    #endif
    #if (YOUKU_SDK == ENABLE)
    err = YoukuSdkOpen();
    #endif

    return err;
}

INT32 UINet_HFSUnInit(void)
{
    #if (YOUKU_SDK == ENABLE)
    YoukuSdkClose();
    #endif
    cyg_hfs_close();
    #if (WIFI_FTP_FUNC==ENABLE)
    stop_ftpserver();
    #endif

    umount(MOUNT_FS_ROOT);

    return E_OK;
}

#if (WIFI_FTP_FUNC==ENABLE)
////////////////////////////////////FTP server ///////////////////////////////////

#include <network.h>
#include <net/ftpd.h>

#define STACK_SIZE (32*1024 + 0x1000)
static char stackFtpd[STACK_SIZE];
static cyg_thread thread_dataFtpd;
static cyg_handle_t thread_handleFtpd;

static ftp_server_t gFtpServer;

static int ftpCheckPasswd(const char * username, const char * passwd)
{
    if (strncmp(username, "ftpuser", strlen("ftpuser")) == 0)
    {
        if (strncmp(passwd, "12345", strlen("12345")) == 0)
        {
            // return 0 indicates user accepted
            return 0;
        }
    }
    else if (strncmp(username, "nvtuser", strlen("nvtuser")) == 0)
    {
        if (strncmp(passwd, "nvt123", strlen("nvt123")) == 0)
        {
            // return 0 indicates user accepted
            return 0;
        }
    }

    // return 1 indicates user rejected
    return 1;
}


static void ftpd_thread(cyg_addrword_t p)
{
    int sts;

    memset(&gFtpServer, 0, sizeof(gFtpServer));
    gFtpServer.allow_guest = 0;     // allow anonymous login
    gFtpServer.check_pwd = ftpCheckPasswd;
    gFtpServer.max_nr_of_clients = 4;
    strcpy(gFtpServer.chrootdir, MOUNT_FS_ROOT);
    sts = ftpd_server(&gFtpServer);
    if (sts != 0)
    {
        DBG_IND("%s: result is %d\r\n", __func__, sts);
    }
    DBG_IND("ftpd ended\r\n");

    cyg_thread_exit();
}

void start_ftpserver(void)
{
    // assume a file system is already mounted on /sdcard/ftp_home

/* Create a main thread, so we can run the scheduler and have time 'pass' */
    cyg_thread_create(10,                // Priority - just a number
                    ftpd_thread,          // entry
                    0,                 // entry parameter
                    "FTP test",        // Name
                    &stackFtpd[0],         // Stack
                    STACK_SIZE,        // Size
                    &thread_handleFtpd,    // Handle
                    &thread_dataFtpd       // Thread data structure
                    );
    cyg_thread_resume(thread_handleFtpd);  /* Start it */
}


static ftp_server_t gFtpServer;
static cyg_handle_t thread_handleFtpd;

void stop_ftpserver(void)
{
    // assume gFtpServer and thread_handleFtpd are those used to start ftp server

ftpd_server_stop(&gFtpServer);
cyg_thread_delete(thread_handleFtpd);
}
#endif

#else // USE_ECOS_KERNEL = OFF

BOOL UINet_GetStationConnected(void)
{
    return FALSE;
}

char* UINet_GetConnectedMAC(void)
{
    return NULL;
}

void UINet_SetMode(NET_MODE_SETTING mode)
{
}

UINT32 UINet_GetMode(void)
{
    return 0;
}

void UINet_SetAuthType(UINT32 authtype)
{
}

UINT32 UINet_GetAuthType(void)
{
    return 0;
}

char* UINet_GetSSID(void)
{
    return NULL;
}

char* UINet_GetDefaultAPSSID(void)
{
    return NULL;
}

char* UINet_GetPASSPHRASE(void)
{
    return NULL;
}

char* UINet_GetDefaultAPPASSPHRASE(void)
{
    return NULL;
}

char* UINet_GetIP(void)
{
    return NULL;
}

char* UINet_GetMAC(void)
{
    return NULL;
}

void UINet_SetSSID(char *ssid, UINT32 len)
{
}

void UINet_SetPASSPHRASE(char *pwd, UINT32 len)
{
}

BOOL UINet_CliReConnect(void)
{
	return FALSE;
}

void UINet_RemoveUser(void)
{
}

void UINet_AddACLTable(void)
{
}

void UINet_ClearACLTable(void)
{
}

void UINet_SiteSurvey(char *ifname, NVT_SS_STATUS_Tp pStatus)
{
}

void UINet_DumpAllSSID(NVT_SS_STATUS_Tp pStatus)
{
}

UINT32 UINet_DHCP_Start(UINT32 isClient)
{
    return 0;
}

void UINet_P2PDump(NVT_SS_STATUS_Tp pStatus)
{
}

void UINet_P2PScan(char *ifname, NVT_SS_STATUS_Tp pStatus)
{
}

UINT32 UINet_P2PConnect(NVT_SS_STATUS_Tp pStatus, UINT32 idx)
{
    return 0;
}

void UINet_P2PConfirm(void)
{
}

void UINet_WifiBack2Dev(void)
{
}

void UINet_WifiP2PReInit(void)
{
}

void UINet_ResetPASSPHRASE(void)
{
}

void UINet_ResetSSID(void)
{
}

#endif
