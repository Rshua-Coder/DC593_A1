#ifndef CYGONCE_CYG_WIFI_NVT_H
#define CYGONCE_CYG_WIFI_NVT_H

//=============================================================================
//define NVTWIFI_SIM for simulation
//#define NVTWIFI_SIM

//-------------------------------------------------------------------------
//NvtWifiECOS API return value
#define NVTWIFI_RET_OK           0
#define NVTWIFI_RET_OPENED       1
#define NVTWIFI_RET_ERR          (-1)
#define NVTWIFI_RET_NO_FUNC      (-2)

#define NVTWIFI_TOKEN_PATH       "wifiipc"
#define NVTWIFI_MAX_PARAM                6
#define NVTWIFI_PARAM_LEN        64

//-------------------------------------------------------------------------
//IPC command id and parameters
typedef enum{
    NVTWIFI_CMDID_GET_VER_INFO  =   0x00000000,
    NVTWIFI_CMDID_GET_BUILD_DATE,
    NVTWIFI_CMDID_TEST,
    NVTWIFI_CMDID_INIT,
    NVTWIFI_CMDID_INTF_IS_UP,
    NVTWIFI_CMDID_INTF_UP,
    NVTWIFI_CMDID_INTF_DOWN,
    NVTWIFI_CMDID_INTF_CONFIG,
    NVTWIFI_CMDID_INTF_DELETE_ADDR,
    NVTWIFI_CMDID_CONFIG,
    NVTWIFI_CMDID_GET_MAC,
    NVTWIFI_CMDID_SET_MAC,
    NVTWIFI_CMDID_RUN_CMD,
    NVTWIFI_CMDID_REG_STACB,
    NVTWIFI_CMDID_REG_LINKCB,
    NVTWIFI_CMDID_REG_WSCCB,
    NVTWIFI_CMDID_REG_WSC_FLASHCB,
    NVTWIFI_CMDID_REG_P2PCB,
    NVTWIFI_CMDID_STA_CB,
    NVTWIFI_CMDID_LINK_CB,
    NVTWIFI_CMDID_WSCCB,
    NVTWIFI_CMDID_WSC_FLASHCB,
    NVTWIFI_CMDID_P2PCB,
    NVTWIFI_CMDID_SITE_REQ,
    NVTWIFI_CMDID_SITE_RET,
    NVTWIFI_CMDID_P2P_REQ,
    NVTWIFI_CMDID_P2P_RET,
    NVTWIFI_CMDID_P2P_PROVISION,
    NVTWIFI_CMDID_WSCD_CREATE,
    NVTWIFI_CMDID_WSCD_REINIT,
    NVTWIFI_CMDID_WSCD_STOP,
    NVTWIFI_CMDID_WSCD_STATUS,
    NVTWIFI_CMDID_GEN_PIN,
    NVTWIFI_CMDID_THREAD_INFO_BY_NAME,
    NVTWIFI_CMDID_IGNORE_DOWN_UP,
    NVTWIFI_CMDID_SYSREQ_ACK,
}NVTWIFI_CMDID;

typedef struct{
    NVTWIFI_CMDID CmdId;
    int Arg;
}NVTWIFI_MSG;

//-------------------------------------------------------------------------
//NVTWIFI definition

typedef struct _NVTWIFI_OPEN_OBJ{
    unsigned int        RcvParamAddr;
    unsigned int        SndParamAddr;
}NVTWIFI_OPEN_OBJ;


#define WIFIIPC_BUF_CHK_SIZE    (4)
#define WIFIIPC_BUF_TAG         MAKEFOURCC('W','I','F','I')

#define NVT_MAX_BSS_DESC            (64)
#define NVT_WIFIIPC_MAC_ADDR_LEN    (6)
#define NVT_MESH_ID_LEN             32

#define NVT_WSC_MAX_SSID_LEN            (32)
#define NVT_MAX_WEP_KEY_LEN             (26)
#define NVT_MAX_NETWORK_KEY_LEN         (64)
#define NVT_PIN_LEN                      (8)
#define NVT_MAX_DEVICE_NAME_LEN         (32)

#define NVT_WPS_REINIT_EVENT            1
#define NVT_WPS_STATUS_CHANGE_EVENT     2

// for client mode

#define NVT_WIFI_LINK_STATUS_TIMEOUT            -1
#define NVT_WIFI_LINK_STATUS_DISCONNECTED       0
#define NVT_WIFI_LINK_STATUS_CONNECTED          1
#define NVT_WIFI_LINK_STATUS_AUTH_FAIL          2
#define NVT_WIFI_LINK_STATUS_DISASSOCIATED      3
#define NVT_WIFI_LINK_STATUS_DEAUTHENTICATED    4
#define NVT_WIFI_LINK_STATUS_HANDSHAKE_TIMEOUT  5
#define NVT_WIFI_LINK_STATUS_SACN_ZREO_RESULT   6

// for AP mode
#define NVT_WIFI_STA_STATUS_ASSOCIATED      1
#define NVT_WIFI_STA_STATUS_REASSOCIATED    2
#define NVT_WIFI_STA_STATUS_DISASSOCIATED   3
#define NVT_WIFI_STA_STATUS_DEAUTHENTICATED 4
#define NVT_WIFI_STA_STATUS_PORT_AUTHORIZED 5
#define NVT_WIFI_STA_STATUS_AUTH_FAIL       6
#define NVT_WIFI_STA_STATUS_EXPIRED         7
#define NVT_WIFI_AP_READY                   8
#define NVT_WIFI_EASY_SETUP                 9

// for p2p
#define NVT_WIFI_P2P_EVENT_BACK2DEV            1
#define NVT_WIFI_P2P_EVENT_START_DHCPD      2
#define NVT_WIFI_P2P_EVENT_START_DHCPC      3
#define NVT_WIFI_P2P_EVENT_INDICATE_PBC_REQ 4   /*wlan drv indicate has peer request to do P2P nego by PBC method*/ 

// for wifi scan
#define NVT_WIFI_SCAN_REPORT    1
#define NVT_WIFI_SCAN_DONE      2
#define NVT_WIFI_SCAN_FAILED    3

typedef struct _nvt_wifi_settings
{
    int wlan_disable;
    int wps_disable;
    char mode[16];
    char security[32];
    int channel;
    char ssid[NVT_WSC_MAX_SSID_LEN+1];
    char passphrase[NVT_MAX_NETWORK_KEY_LEN+1];
    char wep_key1[NVT_MAX_WEP_KEY_LEN+1];
    char wep_key2[NVT_MAX_WEP_KEY_LEN+1];
    char wep_key3[NVT_MAX_WEP_KEY_LEN+1];
    char wep_key4[NVT_MAX_WEP_KEY_LEN+1];
    int wep_default_key;
    char wsc_pin[NVT_PIN_LEN+1];
    int is_configured;
    int config_by_ext_reg;
    int p2pmode;
    char device_name[NVT_MAX_DEVICE_NAME_LEN+1];
    int is_dhcp; // static ip or dhcp for client mode
} nvt_wifi_settings;

typedef struct _nvt_wsc_flash_param {
    char SSID[NVT_WSC_MAX_SSID_LEN+1];
    int WSC_AUTH;     /* WSC_AUTH_OPEN=1, WSC_AUTH_WPAPSK=2, WSC_AUTH_SHARED=4, WSC_AUTH_WPA=8, WSC_AUTH_WPA2=0x10, WSC_AUTH_WPA2PSK=0x20, WSC_AUTH_WPA2PSKMIXED=0x22 */
    char WSC_PSK[NVT_MAX_NETWORK_KEY_LEN+1];
    int WEP; /* WEP_DISABLED=0, WEP64=1, WEP128=2*/
    char WEP_KEY1[NVT_MAX_WEP_KEY_LEN+1];
    char WEP_KEY2[NVT_MAX_WEP_KEY_LEN+1];
    char WEP_KEY3[NVT_MAX_WEP_KEY_LEN+1];
    char WEP_KEY4[NVT_MAX_WEP_KEY_LEN+1];
    int WEP_DEFAULT_KEY;
    int WSC_ENC; /* WSC_ENCRYPT_NONE=1, WSC_ENCRYPT_WEP=2, WSC_ENCRYPT_TKIP=4, WSC_ENCRYPT_AES=8, WSC_ENCRYPT_TKIPAES=12 */
    int WSC_CONFIGBYEXTREG; /* CONFIG_BY_INTERNAL_REGISTRAR=1, CONFIG_BY_EXTERNAL_REGISTRAR=2, MANUAL_SETTING_TO_ENROLLEE=3 */
    int WLAN0_P2P_TYPE;
}nvt_wsc_flash_param;

struct nvt_bss_desc {
    unsigned char   bssid[NVT_WIFIIPC_MAC_ADDR_LEN];
    unsigned char   ssid[32];
    unsigned char   *ssidptr;           // unused, for backward compatible
    unsigned short  ssidlen;
    unsigned char   meshid[NVT_MESH_ID_LEN];
    unsigned char   *meshidptr;         // unused, for backward compatible
    unsigned short  meshidlen;
    unsigned int    bsstype;
    unsigned short  beacon_prd;
    unsigned char   dtim_prd;
    unsigned int    t_stamp[2];
    unsigned int    ibss_par;
    unsigned short  capability;
    unsigned char   channel;
    unsigned int    basicrate;
    unsigned int    supportrate;
    unsigned char   bdsa[NVT_WIFIIPC_MAC_ADDR_LEN];
    unsigned char   rssi;
    unsigned char   sq;
    unsigned int    network;
    /*add for P2P_SUPPORT ; for sync; it exist no matter p2p enabled or not*/
    unsigned char   p2pdevname[33];
    unsigned char   p2prole;
    unsigned short  p2pwscconfig;
    unsigned char   p2paddress[NVT_WIFIIPC_MAC_ADDR_LEN];
};

typedef struct _nvt_sitesurvey_status {
    unsigned char number;
    unsigned char pad[3];
    //BssDscr bssdb[MAX_BSS_DESC];
    struct nvt_bss_desc bssdb[NVT_MAX_BSS_DESC];
} NVT_SS_STATUS_T, *NVT_SS_STATUS_Tp;

/* Any changed here MUST sync with 8192cd_p2p.h */
typedef struct _nvt_p2p_provision_comm
{
    unsigned char dev_address[6];
    unsigned short wsc_config_method;
    unsigned char channel;
} NVT_P2P_PROVISION_COMM_T, *NVT_P2P_PROVISION_COMM_Tp;

enum NVT_NETWORK_TYPE {
    NVT_WIRELESS_11B = 1,
    NVT_WIRELESS_11G = 2,
    NVT_WIRELESS_11A = 4,
    NVT_WIRELESS_11N = 8,
    NVT_WIRELESS_11AC = 64,
    NVT_WIRELESS_24G = 128,
    NVT_WIRELESS_5G = 256,
    NVT_WIRELESS_32BIT = 512
};

typedef enum _NVT_Capability {
    nvt_cESS        = 0x01,
    nvt_cIBSS       = 0x02,
    nvt_cPollable       = 0x04,
    nvt_cPollReq        = 0x01,
    nvt_cPrivacy        = 0x10,
    nvt_cShortPreamble  = 0x20,
} NVT_Capability;

enum nvt_wifi_state {
    NVT_WIFI_STATION_STATE  =   0x00000008,
    NVT_WIFI_AP_STATE       =   0x00000010,
    NVT_WIFI_P2P_SUPPORT    =   0x08000000
};

enum nvt_p2p_role_more{
    NVT_P2P_DEVICE=1,
    NVT_P2P_PRE_CLIENT=2,
    NVT_P2P_CLIENT=3,
    NVT_P2P_PRE_GO=4,    // after GO nego , we are GO and proceed WSC exchange
    NVT_P2P_TMP_GO=5     // after GO nego , we are GO and proceed WSC exchange is done
};

typedef enum
{
    NVT_WIFI_SEC_INVALID              =0,
    NVT_WIFI_SEC_NONE                 =0x01,
    NVT_WIFI_SEC_WEP                  =0x02,
    NVT_WIFI_SEC_WEP_128              =0x20,

    NVT_WIFI_SEC_WPA_PSK              =0x04,
    NVT_WIFI_SEC_WPA_1X               =0x05,

    NVT_WIFI_SEC_WPA_PSK_AES_TKIP     =0x09,
    NVT_WIFI_SEC_WPA_PSK_AES          =0x0A,
    NVT_WIFI_SEC_WPA_PSK_TKIP         =0x0B,

    NVT_WIFI_SEC_WPA_1X_AES_TKIP      =0x0C,
    NVT_WIFI_SEC_WPA_1X_AES           =0x0D,
    NVT_WIFI_SEC_WPA_1X_TKIP          =0x0E,

    NVT_WIFI_SEC_WPA2_PSK             =0x10,
    NVT_WIFI_SEC_WPA2_1X              =0x11,

    NVT_WIFI_SEC_WPA2_PSK_AES_TKIP    =0x15,
    NVT_WIFI_SEC_WPA2_PSK_AES         =0x16,
    NVT_WIFI_SEC_WPA2_PSK_TKIP        =0x17,

    NVT_WIFI_SEC_WPA2_1X_AES_TKIP     =0x18,
    NVT_WIFI_SEC_WPA2_1X_AES          =0x19,
    NVT_WIFI_SEC_WPA2_1X_TKIP         =0x1A,

    NVT_WIFI_SEC_WPA_AUTO_PSK_AES_TKIP    =0x1B,
    NVT_WIFI_SEC_WPA_AUTO_PSK_AES         =0x1C,
    NVT_WIFI_SEC_WPA_AUTO_PSK_TKIP        =0x1D,
    NVT_WIFI_SEC_MAX,
}NVT_WIFI_SEC_SETTING;

enum nvt_wifi_wakeup_reason {
    NVT_WIFI_WAKEUP_REASON_UNKNOWN               =   0,
    NVT_WIFI_WAKEUP_REASON_DATA                  =   1,
    NVT_WIFI_WAKEUP_REASON_DISCONNECT            =   2,
	NVT_WIFI_WAKEUP_REASON_NOT_WAKEUP            =   3,
	NVT_WIFI_WAKEUP_REASON_AUTO_RECOVERY_SUCCESS =   4,
	NVT_WIFI_WAKEUP_REASON_AUTO_RECOVERY_FAIL    =   5,
	NVT_WIFI_WAKEUP_REASON_DHCP_OK               =   6,
	NVT_WIFI_WAKEUP_REASON_DHCP_NAK              =   7,
	NVT_WIFI_WAKEUP_REASON_DHCP_No_Resp          =   8,
};

enum nvt_wifi_sleep_recovery_action {
    NVT_WIFI_SLEEP_RECOVERY_ACTION_ALL        =   0,
    NVT_WIFI_SLEEP_RECOVERY_ACTION_RECOVER    =   1,
    NVT_WIFI_SLEEP_RECOVERY_ACTION_RECONNECT  =   2,
	NVT_WIFI_SLEEP_RECOVERY_ACTION_NONE       =   3,
	NVT_WIFI_SLEEP_RECOVERY_ACTION_FINAL_FAIL =   4,
};

///////////////////////parameter struct ///////////////
typedef struct{
    int param1;
    int param2;
    int param3;
}NVTWIFI_PARAM_VALUE;

typedef struct{
    char pIntfName[NVTWIFI_PARAM_LEN];
}NVTWIFI_PARAM_INTF;

typedef struct{
    char pIntfName[NVTWIFI_PARAM_LEN];
    int  status;
}NVTWIFI_PARAM_STATUS;

typedef struct{
    char pIntfName[NVTWIFI_PARAM_LEN];
    char string[NVTWIFI_PARAM_LEN];
    int  status;
}NVTWIFI_PARAM_STATUS2;

typedef struct{
    char string1[NVTWIFI_PARAM_LEN];
    char string2[NVTWIFI_PARAM_LEN];
    char string3[NVTWIFI_PARAM_LEN];
}NVTWIFI_PARAM_STRING;

typedef struct{
    char param[NVTWIFI_MAX_PARAM][NVTWIFI_PARAM_LEN];
}NVTWIFI_PARAM_RUN_CMD;

typedef struct{
    char pIntfName[NVTWIFI_PARAM_LEN];
    NVT_SS_STATUS_T  status;
}NVTWIFI_PARAM_SITE_STATUS;

typedef struct{
    char pIntfName[NVTWIFI_PARAM_LEN];
    NVT_P2P_PROVISION_COMM_T  status;
}NVTWIFI_PARAM_PROVISION_STATUS;

typedef struct{
	unsigned char smac[6];
	unsigned char dmac[6];
    unsigned char sip[4];
	unsigned char dip[4];
	unsigned int  sport;
	unsigned int  dport;
	unsigned char *payload;
	unsigned int  plen;
	unsigned int  interval;
}NVTWIFI_PARAM_KEEP_ALIVE;

typedef struct{
	unsigned char smac[6];
	unsigned char dmac[6];
    unsigned char sip[4];
	unsigned char dip[4];
	int           tcp;
	unsigned int  sport;
	unsigned int  dport;
	unsigned char *payload;
	unsigned int  plen;
	unsigned int  interval;
}NVTWIFI_PARAM_WAKEUP_PATTERN;

typedef struct{
	char                 ssid[32];
	char                 password[32];
	NVT_WIFI_SEC_SETTING security;
	unsigned char        ip[4];
	int                  reserved;
}NVTWIFI_PARAM_EASY_SETUP;

typedef struct{
	unsigned int scan_frequency;
	unsigned int repeat;
    unsigned int exponent;
	unsigned int timeout;
	unsigned int action;
	char         ssid[32];
    char         security[32];
}NVTWIFI_PARAM_SLEEP_RECOVERY;

typedef struct{
	unsigned char current_ip[4];
	unsigned int  lease_time;
}NVTWIFI_PARAM_DHCP_OFFLOAD;

int Wifi_HAL_Test                      (void);
int Wifi_HAL_Init                      (int param);
int Wifi_HAL_Is_Interface_Up           (const char *pIntfName);
int Wifi_HAL_Interface_Up              (const char *pIntfName);
int Wifi_HAL_Interface_Down            (const char *pIntfName);
int Wifi_HAL_Interface_Config          (const char *pIntfName, char *addr, char *netmask);
int Wifi_HAL_Interface_Delete          (char *pIntfName);
int Wifi_HAL_Config                    (nvt_wifi_settings *pwifi);
int Wifi_HAL_Get_Mac                   (char *mac);
int Wifi_HAL_Set_Mac                   (const char *pIntfName, char *mac);
int Wifi_HAL_Run_Cmd                   (char *param1, char *param2, char *param3, char *param4);
int Wifi_HAL_RegisterStaCB             (char *pIntfName, void (*CB)(char *pIntfName, char *pMacAddr, int status));
int Wifi_HAL_RegisterLinkCB            (char *pIntfName, void (*CB)(char *pIntfName, int status));
int Wifi_HAL_RegisterWSCCB             (void (*CB)(int evt, int status));
int Wifi_HAL_RegisterWSCFlashCB        (int (*CB)(nvt_wsc_flash_param *param));
int Wifi_HAL_RegisterP2PCB             (char *pIntfName, void (*CB)(char *pIntfName,  int status));
int Wifi_HAL_RegisterScanCB            (char *pIntfName, void (*CB)(char *pIntfName, struct nvt_bss_desc *result, int status));
int Wifi_HAL_RegisterQueryCB           (char *pIntfName, void (*CB)(char *pIntfName, char *ssid, char *mode));
int Wifi_HAL_getWlSiteSurveyRequest    (char *pIntfName, int *pStatus);
int Wifi_HAL_getWlSiteSurveyResult     (char *pIntfName, NVT_SS_STATUS_T *pSSStatus, unsigned int size);
int Wifi_HAL_getWlP2PScanRequest       (char *pIntfName, int *pStatus);
int Wifi_HAL_getWlP2PScanResult        (char *pIntfName, NVT_SS_STATUS_T *pSSStatus, unsigned int size);
int Wifi_HAL_sendP2PProvisionCommInfo  (char *pIntfName, NVT_P2P_PROVISION_COMM_Tp pP2pProvision);
int Wifi_HAL_create_wscd               (void);
int Wifi_HAL_wsc_reinit                (void);
int Wifi_HAL_wsc_stop_service          (void);
int Wifi_HAL_wscd_status               (void);
int Wifi_HAL_generate_pin_code         (char *pinbuf);
int Wifi_HAL_ignore_down_up            (char *pIntfName, int val);
int Wifi_HAL_Wow_Clear                 (void);
int Wifi_HAL_Wow_Show_Status           (void);
int Wifi_HAL_Wow_SetIP                 (char *IP,char *Mac,char *AP);
int Wifi_HAL_Wow_Offload               (void);
int Wifi_HAL_Wow_Set_Pattern           (void);
int Wifi_HAL_Wow_Apply                 (void);
int Wifi_HAL_Wow_Keep_Alive_Pattern    (NVTWIFI_PARAM_KEEP_ALIVE *param);
int Wifi_HAL_Wow_Wakeup_Pattern        (NVTWIFI_PARAM_WAKEUP_PATTERN *param);
int Wifi_HAL_Wow_Apply2                (void);
int Wifi_HAL_Wow_Set_Auto_Recovery     (NVTWIFI_PARAM_SLEEP_RECOVERY *param);
int Wifi_HAL_Dhcp_Offload_Config       (NVTWIFI_PARAM_DHCP_OFFLOAD *param);
int Wifi_HAL_Dhcp_Offload_Enable       (int enable);
int Wifi_HAL_Set_Dtim                  (char *pIntfName, int unit, int duration);
int Wifi_HAL_Set_Beacon_Timeout        (char *pIntfName, int time);
int Wifi_HAL_Set_Gratuitous_Arp        (unsigned char smac[6], unsigned char sip[4], unsigned int  interval);
int Wifi_HAL_Set_Power_Manage_Mode     (char *pIntfName, int mode);
int Wifi_HAL_Set_Country               (const char *c);
int Wifi_HAL_Set_Keep_Resp_Detect      (unsigned char sip[4], int idx, int last, int interval);

typedef struct _NVTWIFI_CMDID_IMPL
{
    int               (*Wifi_CmdId_Test)(void);
    int               (*Wifi_CmdId_Init)(int param);
    int               (*Wifi_CmdId_Is_Interface_Up)(const char *pIntfName);
    int               (*Wifi_CmdId_Interface_Up)(const char *pIntfName);
    int               (*Wifi_CmdId_Interface_Down)(const char *pIntfName);
    int               (*Wifi_CmdId_Interface_Config)(const char *pIntfName, char *addr, char *netmask);
    int               (*Wifi_CmdId_Interface_Delete)(char *pIntfName);
    int               (*Wifi_CmdId_Config)(nvt_wifi_settings *pwifi);
    int               (*Wifi_CmdId_Get_Mac)(char *mac);
    int               (*Wifi_CmdId_Set_Mac)(const char *pIntfName, char *mac);
    int               (*Wifi_CmdId_Run_Cmd)(char *param1, char *param2, char *param3, char *param4);
    int               (*Wifi_CmdId_RegisterStaCB)(char *pIntfName);
    int               (*Wifi_CmdId_RegisterLinkCB)(char *pIntfName);
    int               (*Wifi_CmdId_RegisterWSCCB)(void);
    int               (*Wifi_CmdId_RegisterWSCFlashCB)(void);
    int               (*Wifi_CmdId_RegisterP2PCB)(char *pIntfName);
    int               (*Wifi_CmdId_RegisterQueryCB)(char *pIntfName);
    int               (*Wifi_CmdId_getWlSiteSurveyRequest)(char *pIntfName, int *pStatus);
    int               (*Wifi_CmdId_getWlSiteSurveyResult)(char *pIntfName, NVT_SS_STATUS_T *pSSStatus, unsigned int size);
    int               (*Wifi_CmdId_getWlP2PScanRequest)(char *pIntfName, int *pStatus);
    int               (*Wifi_CmdId_getWlP2PScanResult)(char *pIntfName, NVT_SS_STATUS_T *pSSStatus);
    int               (*Wifi_CmdId_sendP2PProvisionCommInfo)(char *pIntfName, NVT_P2P_PROVISION_COMM_Tp pP2pProvision);
    int               (*Wifi_CmdId_create_wscd)(void);
    int               (*Wifi_CmdId_wsc_reinit)(void);
    int               (*Wifi_CmdId_wsc_stop_service)(void);
    int               (*Wifi_CmdId_wscd_status)(void);
    int               (*Wifi_CmdId_generate_pin_code)(char *pinbuf);
    int               (*Wifi_CmdId_ignore_down_up)(char *pIntfName, int val);
    int               (*Wifi_CmdId_wow_clear)(void);
    int               (*Wifi_CmdId_wow_show_status)(void);
    int               (*Wifi_CmdId_wow_set_ip)(char *IP,char *Mac,char *AP);
    int               (*Wifi_CmdId_wow_offload)(void);
    int               (*Wifi_CmdId_wow_set_pattern)(void);
    int               (*Wifi_CmdId_wow_apply)(void);
	int               (*Wifi_CmdId_wow_keep_alive_pattern)(NVTWIFI_PARAM_KEEP_ALIVE *param);
    int               (*Wifi_CmdId_wow_wakeup_pattern)(NVTWIFI_PARAM_WAKEUP_PATTERN *param);
    int               (*Wifi_CmdId_wow_apply2)(void);
    int               (*Wifi_CmdId_wow_set_auto_recovery)(NVTWIFI_PARAM_SLEEP_RECOVERY *param);
    int               (*Wifi_CmdId_dhcp_offload_config)(NVTWIFI_PARAM_DHCP_OFFLOAD *param);
    int               (*Wifi_CmdId_dhcp_offload_enable)(int enable);
    int               (*Wifi_CmdId_set_dtim)(char *pIntfName, int unit, int duration);
    int               (*Wifi_CmdId_set_beacon_timeout)(char *pIntfName, int time);
    int               (*Wifi_CmdId_set_gratuitous_arp)(unsigned char smac[6], unsigned char sip[4], unsigned int  interval);
    int               (*Wifi_CmdId_set_power_manage_mode)(char *pIntfName, int mode);
    int               (*Wifi_CmdId_set_country)(const char *c);
    int               (*Wifi_CmdId_set_keep_resp_detect)(unsigned char sip[4], int idx, int last, int interval);
}NVTWIFI_CMDID_IMPL;

extern const NVTWIFI_CMDID_IMPL gCmdImplFp;

void Wifi_P2P_CB(char *pIntfName,  int status);
int  Wifi_WSC_Flash_CB(nvt_wsc_flash_param *flash_param);
void Wifi_WSC_Event_CB(int evt, int status);
void Wifi_Link_Status_CB(char *pIntfName,int status);
void Wifi_Sta_Status_CB(char *pIntfName, char *pMacAddr, int status);
void Wifi_Query_CB(char *pIntfName, char *ssid, char *mode);

#ifndef __externC
#ifdef __cplusplus
# define __externC extern "C"
#else
# define __externC extern
#endif
#endif

#endif // CYGONCE_CYG_WIFI_NVT_H
// End of nvtwifi.h
