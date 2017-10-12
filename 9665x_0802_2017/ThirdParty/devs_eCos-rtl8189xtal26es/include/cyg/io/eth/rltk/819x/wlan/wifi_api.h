#ifndef _WIFI_API_H
#define _WIFI_API_H

/*
 * pri: use to assign wifi thread priority, 0 - default priority
 * retrun: 0 success, -1 fail
 */
extern int wifi_init_wlan0_netdev(int pri);
extern int wifi_get_wlan0_efuse_mac(char *pMacAddr); //0 success, -1 fail


extern int wifi_set_cystalclk_to_efuse(char *StrInput);
extern unsigned char wifi_get_cystalclk_to_efuse(void);



// for client mode
#define WIFI_LINK_STATUS_DISCONNECTED	0
#define WIFI_LINK_STATUS_CONNECTED		1
#define WIFI_LINK_STATUS_AUTH_FAIL		2
#define WIFI_LINK_STATUS_DISASSOCIATED	3
#define WIFI_LINK_STATUS_DEAUTHENTICATED	4
#define WIFI_LINK_STATUS_HANDSHAKE_TIMEOUT	5
#define WIFI_LINK_STATUS_SACN_ZREO_RESULT 6
// callback function prototype for link status report
typedef void (wifi_link_status_cb_func_t)(char *pIntfName, int status);

//return 1: register success, 0: register fail.
extern int register_wifi_link_status_cb(char *pIntfName, wifi_link_status_cb_func_t *pFunc);

// for AP mode
#define WIFI_STA_STATUS_ASSOCIATED	1
#define WIFI_STA_STATUS_REASSOCIATED	2
#define WIFI_STA_STATUS_DISASSOCIATED	3
#define WIFI_STA_STATUS_DEAUTHENTICATED	4
#define WIFI_STA_STATUS_PORT_AUTHORIZED	5
#define WIFI_STA_STATUS_AUTH_FAIL		6
#define WIFI_STA_STATUS_EXPIRED		7
#define WIFI_AP_READY				8
// callback function prototype for station status report
typedef void (wifi_sta_status_cb_func_t)(char *pIntfName, char *pMacAddr, int status);

//return 1: register success, 0: register fail.
extern int register_wifi_sta_status_cb(char *pIntfName, wifi_sta_status_cb_func_t *pFunc);

// for p2p
#define WIFI_P2P_EVENT_BACK2DEV            1
#define WIFI_P2P_EVENT_START_DHCPD      2
#define WIFI_P2P_EVENT_START_DHCPC      3
#define WIFI_P2P_EVENT_INDICATE_PBC_REQ 4   /*wlan drv indicate has peer request to do P2P nego by PBC method*/ 

// callback function prototype for p2p
typedef void (p2p_event_indicate_cb_func_t)(char *pIntfName, int status);

//return 1: register success, 0: register fail.
extern int register_p2p_event_indicate_cb(char *pIntfName, p2p_event_indicate_cb_func_t *pFunc);

extern int wifi_ignore_down_up(char *pIntfName, int val); // 1:success, 0:fail

extern void wifi_set_passwd_hash_delay(int tick); // >0: delay in ticks, 0: turn off the mechanism, <0: use cyg_thread_yield() instead
extern int wifi_get_passwd_hash_delay(void);

/*add for simple config sync between wifi driver and simple config daemon*/


typedef struct _sc_info{
    unsigned char sc_pin_enabled;                   // 1, MUST have PIN for SIMPLE CONFIG
    char sc_pin[65];
    char sc_default_pin[65];
    char sc_device_name[64];
    unsigned char sc_device_type;
}_SC_INFO_T;


typedef struct _sc_info2{
    unsigned int sc_uint_tmp;    
    int sc_int_tmp;        
}_SC_INFO_T2;

typedef struct _sc_info3{
    char sc_chararray[65];
}_SC_INFO_T3;


enum SimpleConfig_query {
    xSC_STATUS=0,
    xSC_CONTROL_IP,    
    xSC_PASSWORD,
    xSC_SSID,
    xSC_WLAN_CONNECT_STATUS,    
};


extern void wifi_get_simple_config_info(struct _sc_info *sc_info_t);

extern void wifi_get_simple_config_info2(struct _sc_info2 *sc_info_t2,int queryidx);
extern void wifi_get_simple_config_info3(struct _sc_info3 *sc_info_t3,int queryidx);



/*add for simple config sync between wifi driver and simple config daemon*/
#endif
