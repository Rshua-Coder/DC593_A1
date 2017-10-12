#ifndef _UIAPPNET_H_
#define _UIAPPNET_H_

#include <LibWifi/HAL/nvtwifi.h>

#define TXT_WIFI_AP_MODE  "AP mode"
#define TXT_WIFI_ST_MODE  "station mode"
#define TXT_WIFI_CONNECT  "Conntecting"
#if (_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_ || _NETWORK_ == _NETWORK_SDIO_EVB_WIFI_NVT_)
#define TXT_WIFI          "wifi"
#define TXT_WIFI_SET      "wifi Setting"
#define TXT_WIFI_START    "wifi start"
#else
#define TXT_WIFI          "LAN"
#define TXT_WIFI_SET      "LAN Setting"
#define TXT_WIFI_START    "LAN start"

#endif


typedef enum
{
    NET_AP_MODE = 0,
    NET_STATION_MODE,
    NET_P2P_DEV_MODE,
    NET_P2P_GO_MODE,
    NET_REBOOT,
    NET_NONE = 999,
    NET_MODE_SETTING_MAX
}NET_MODE_SETTING;



#define NET_AUTH_NONE  NVT_WIFI_SEC_NONE
#define NET_AUTH_WPA2  NVT_WIFI_SEC_WPA2_PSK_AES
#define NET_AUTH_WPA   NVT_WIFI_SEC_WPA_PSK_TKIP

typedef enum
{
    NET_STATE_WIFI_START,
    NET_STATE_WIFI_CONNECTING,
    NET_STATE_WIFI_CB,
    NET_STATE_DHCP_START,
    NET_STATE_MAX,
}NET_STATE;

#define WIFI_IFNAME             "wlan0"   //interface name

#define NET_CONNECTING_TIMEOUT    (-1)

extern void UINet_WifiBack2Dev(void);
extern void UINet_WifiP2PReInit(void);
extern INT32 UINet_WifiInit(UINT32 mode,UINT32 security);
extern INT32 UINet_LanInit(UINT32 mode);
extern INT32 UINet_HFSInit(void);
extern INT32 UINet_WifiUnInit(UINT32 mode);
extern INT32 UINet_LanUnInit(UINT32 mode);
extern INT32 UINet_HFSUnInit(void);
extern char* UINet_GetSSID(void);
extern char* UINet_GetDefaultAPSSID(void);
extern char* UINet_GetPASSPHRASE(void);
extern char* UINet_GetDefaultAPPASSPHRASE(void);
extern BOOL UINet_GetStationConnected(void);
extern char* UINet_GetIP(void);
extern char* UINet_GetMAC(void);
extern char* UINet_GetConnectedMAC(void);
extern void UINet_SetAuthType(UINT32 authtype);
extern UINT32 UINet_GetMode(void);
extern UINT32 UINet_GetAuthType(void);
extern void UINet_SetSSID(char *ssid,UINT32 len);
extern void UINet_SetPASSPHRASE(char *pwd,UINT32 len);
extern void UINet_SetChannel(UINT32 channel);
extern UINT32 UINet_GetChannel(void);
extern void UINet_RemoveUser(void);
extern void UINet_AddACLTable(void);
extern void UINet_ClearACLTable(void);
extern void UINet_SiteSurvey(char *ifname,NVT_SS_STATUS_Tp pStatus);
extern void UINet_DumpAllSSID(NVT_SS_STATUS_Tp pStatus);
extern UINT32 UINet_DHCP_Start(UINT32 isClient);
extern void UINet_P2PDump(NVT_SS_STATUS_Tp pStatus);
extern void UINet_P2PScan(char *ifname,NVT_SS_STATUS_Tp pStatus);
extern UINT32 UINet_P2PConnect(NVT_SS_STATUS_Tp pStatus,UINT32 idx);
extern void UINet_P2PConfirm(void);
extern BOOL UINet_CliReConnect(void);
extern void UINet_Reg_Timer(void);
extern void UINet_ResetPASSPHRASE(void);
extern void UINet_ResetSSID(void);

#endif //_UIAPPNET_H_
