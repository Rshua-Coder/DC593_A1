#include <network.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/infra/diag.h>
#include <cyg/io/devtab.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/plf_io.h>
#include <cyg/kernel/kapi.h>
#include <cyg/io/io.h>
#include <cyg/hal/hal_arch.h>

//#include <cyg/kernel/kapi.h>

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cyg/infra/mainfunc.h>
#include <net/dhcpelios/dhcpelios.h>
#include <dhcp.h>
//#include <cyg/io/sdio_linux.h>
#include <cyg/io/brcm_wifi.h>
#include <brcm_fop.h>
#include <arpa/inet.h>

#include "wps_api.h"

#define ECOS_DHCP      1
#define wps            1

// for client mode
#define WIFI_LINK_STATUS_DISCONNECTED   0
#define WIFI_LINK_STATUS_CONNECTED      1
#define WIFI_LINK_STATUS_AUTH_FAIL      2
#define WIFI_LINK_STATUS_DISASSOCIATED  3
#define WIFI_LINK_STATUS_DEAUTHENTICATED    4
#define WIFI_LINK_STATUS_HANDSHAKE_TIMEOUT  5
/*when indicate this event means once scan done and no any result*/
#define WIFI_LINK_STATUS_SACN_ZREO_RESULT   6


// for AP mode
#define WIFI_STA_STATUS_ASSOCIATED  1
#define WIFI_STA_STATUS_REASSOCIATED    2
#define WIFI_STA_STATUS_DISASSOCIATED   3
#define WIFI_STA_STATUS_DEAUTHENTICATED 4
#define WIFI_STA_STATUS_PORT_AUTHORIZED 5
#define WIFI_STA_STATUS_AUTH_FAIL       6
#define WIFI_STA_STATUS_EXPIRED     7
#define WIFI_AP_READY               8


char gapssid[33] = "brcm_ap";
char gappwd[65] = "12345678";

char gstassid[33] = "dlink";
char gstapwd[65] = "12345678";

unsigned char gch = 1;
unsigned char ght = 20;
unsigned char gplusminus = 0;
unsigned char gdtim = 1;



#if ECOS_DHCP
bool geCosDhcpCliOpened = false;
bool geCosDhcpSrvOpened = false;
char geCosNvtNetHostNameCli[CYGNUM_NET_DHCP_OPTION_HOST_NAME_LEN] = "nvtdsc01cli";
char geCosNvtNetHostNameSrv[CYGNUM_NET_DHCP_OPTION_HOST_NAME_LEN] = "nvtdsc01srv";

static int copy_str(char *pdest, char *psrc)
{
    int i = 0;
    int max = CYGNUM_NET_DHCP_OPTION_HOST_NAME_LEN - 1;
    if (pdest && psrc)
    {
        i = strlen(psrc);
        i = (i < max) ? i : max;
        memcpy((void *)pdest, (void *)psrc, i);
        *(pdest+i) = 0;
    }
    return i;
}

bool eCos_Dhcp_Set_Interface(char *pIntf)
{
    return (dhcpd_set_interface(pIntf));
}

void eCos_Dhcp_Client_Stop(void)
{
    if (geCosDhcpCliOpened)
    {
        dhcp_set_hostname(" ");
        dhcp_release_interfaces(3);
        dhcp_reg_cb(0);
        uninit_all_network_interfaces();
        geCosDhcpCliOpened = false;
    }
    else
    {
        printf("Dhcp Client Not Started;Not End it.\r\n");
    }
}

void eCos_Dhcp_Client_Start(char *pName, dhcp_cb_func pFunc, bool forceStopIfPrvExist)
{
    printf(" dhcpc=%d,dhcps=%d,force=%d, func=0x%x +\r\n",geCosDhcpCliOpened,geCosDhcpSrvOpened,forceStopIfPrvExist,pFunc);

    if (geCosDhcpSrvOpened)
    {
        printf("DhcpSrv Started;Can Not Be Dhcp Client at the same time\r\n");
    }
    else if (forceStopIfPrvExist)
    {
        printf("Dhcp Client Already Started !! Stop First, then Restart is again:%d\r\n",forceStopIfPrvExist);
        eCos_Dhcp_Client_Stop();
    }
    else if (geCosDhcpCliOpened)
    {
        printf("Dhcp Client Already Started !! Not Start it again:%d\r\n",forceStopIfPrvExist);
        return;
    }
    copy_str(geCosNvtNetHostNameCli, pName);
    dhcp_set_hostname(geCosNvtNetHostNameCli);
    geCosDhcpCliOpened = true;
    dhcp_reg_cb((dhcp_cb_func)pFunc);
    init_all_network_interfaces();
}

void eCos_Dhcp_Server_Start(char *pName)
{
    if (geCosDhcpCliOpened)
    {
        printf("Dhcp Client Started;Can Not Be Dhcp Server at the same time\r\n");
    }
    else if (false == geCosDhcpSrvOpened)
    {
        geCosDhcpSrvOpened = true;
        copy_str(geCosNvtNetHostNameSrv, pName);
        dhcp_set_hostname(geCosNvtNetHostNameSrv);
        dhcpd_startup();
    }
    else
    {
        printf("Dhcp Server Already Started !!\r\n");
    }
}

void eCos_Dhcp_Server_Stop(bool uninitIP)
{
    if (true == geCosDhcpSrvOpened)
    {
        dhcp_set_hostname(" ");
        dhcpd_exit();
        geCosDhcpSrvOpened = false;
        if ((true == uninitIP) && (true == geCosDhcpSrvOpened))
        {
            printf("Dhcp Server Running.Please Stop it first.\r\n");
        }
    }
    else
    {
        printf("Dhcp Server Not Started;Not End it.\r\n");
    }
}
#endif

int interface_config(const char *intf, char *addr, char *netmask)
{
    struct sockaddr_in *addrp;
    struct ifreq ifr;

    int s=-1;
    int one = 1;
    //struct in_addr bcaddr, gateway;
    int retcode = -1;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("socket");
        goto out;
    }

    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &one, sizeof(one))) {
        perror("setsockopt");
        goto out;
    }

    addrp = (struct sockaddr_in *) &ifr.ifr_addr;

    memset(addrp, 0, sizeof(*addrp));

    addrp->sin_family = AF_INET;

    addrp->sin_len = sizeof(*addrp);

    addrp->sin_port = 0;

    //addrp->sin_addr.s_addr = INADDR_ANY;

    if (inet_aton(addr, &(addrp->sin_addr)) == 0) {
        printf("bad value: %s", addr);
        goto out;
    }

    // Must do this temporarily with default route and netmask so that

    // [sub]netmask can be set.

    strcpy(ifr.ifr_name, intf);

    if (ioctl(s, SIOCSIFADDR, &ifr)) { // set ifnet address
        perror("SIOCSIFADDR");
        goto out;
    }

    if (netmask) {
        if (inet_aton(netmask, &(addrp->sin_addr)) == 0) {
            printf("bad value2: %s", netmask);
            goto out;
        }

        if (ioctl(s, SIOCSIFNETMASK, &ifr)) { // set net addr mask
            perror("SIOCSIFNETMASK");
            goto out;
        }

        // Must do this again so that [sub]netmask (and so default route)

        // is taken notice of.

        if (inet_aton(addr, &(addrp->sin_addr)) == 0) {
            printf("bad value3: %s", addr);
            goto out;
        }

        if (ioctl(s, SIOCSIFADDR, &ifr)) {
            perror("SIOCSIFADDR 2");
            goto out;
        }
    }

    ifr.ifr_flags = IFF_UP | IFF_BROADCAST | IFF_RUNNING;
    if (ioctl(s, SIOCSIFFLAGS, &ifr)) { // set ifnet flags
        perror("SIOCSIFFLAGS");
        goto out;
    }

    retcode = 0;

 out:
    if (s != -1)
      close(s);
    return retcode;
}

void interface_up(const char *intf)
{
  struct ifreq ifr;
  int s = -1;

  s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s < 0) {
    perror("socket");
    return;
  }

  strcpy(ifr.ifr_name, intf);
  ifr.ifr_flags = IFF_UP | IFF_BROADCAST | IFF_RUNNING;
  if (ioctl(s, SIOCSIFFLAGS, &ifr)) {
    perror("SIOCSIFFLAGS");
  }
  close(s);
}

void interface_down(const char *intf)
{
  struct ifreq ifr;
  int s = -1;

  s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s < 0) {
    perror("socket");
    return;
  }

  strcpy(ifr.ifr_name, intf);
  ifr.ifr_flags &= ~(IFF_UP | IFF_RUNNING);
  if (ioctl(s, SIOCSIFFLAGS, &ifr)) {
    perror("SIOCSIFFLAGS");
  }
  close(s);
}

void wifi_callback_cb_fn(unsigned int status)
{
    if (status == WIFI_LINK_STATUS_CONNECTED) {
        printf("Connected with AP\n");
       // cyg_flag_setbits(&sys_flag, SYS_WIFI_LINK_UP_EVENT);
    }
    else if (status == WIFI_LINK_STATUS_DISCONNECTED) {
        printf("Disconnected with AP\n");
       // cyg_flag_setbits(&sys_flag, SYS_WIFI_LINK_DOWN_EVENT);
    }
    else if (status == WIFI_LINK_STATUS_AUTH_FAIL) {
       printf("Fail to authenticate with AP\n");
    }
    else if (status == WIFI_LINK_STATUS_DISASSOCIATED) {
      printf("Disassociated by AP\n");
    }
    else if (status == WIFI_LINK_STATUS_DEAUTHENTICATED) {
       printf("Deauthenticated by AP\n");
    }
    else if (status == WIFI_LINK_STATUS_HANDSHAKE_TIMEOUT) {
        printf("%s: handshake timeout\n");
    }
    else if (status == WIFI_LINK_STATUS_SACN_ZREO_RESULT) {
        //printf("%s: scan no result\n", pIntfName);
    }
    else
        printf("unknown status\n");
}

void wifi_ap_callback_cb_fn(unsigned int status, unsigned char *pMacAddr)
{
    if (status == WIFI_STA_STATUS_ASSOCIATED)
    {
        printf("A client(%02X:%02X:%02X:%02X:%02X:%02X) is associated\n",
                    *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_DISASSOCIATED)
    {
        printf("A client(%02X:%02X:%02X:%02X:%02X:%02X) is disassociated\n",
                    *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_DEAUTHENTICATED)
    {
        printf("A client(%02X:%02X:%02X:%02X:%02X:%02X) is deauth\n",
                    *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_AP_READY)
    {
        printf("AP is ready (%02X:%02X:%02X:%02X:%02X:%02X)\n",
                    *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }


}


void wifi_wps_sta_callback_cb_fn(char *ssid, char *key, char *encr, char *auth)
{
    char *v[32];
    int c;
    int argc = 3;

    printf("wifi_wps_sta_callback_cb_fn\n");

    if (strcmp(encr, "AES") == 0)
    {
        printf("AES\n");
        encr = "4";
    }
    else if (strcmp(encr, "TKIP") == 0)
    {
        printf("TKIP\n");
        encr = "2";
    }

    if (strcmp(auth, "WPA2-PSK") == 0)
    {
        printf("WPA2-PSK\n");
        auth = "0x80";
    }
    else if (strcmp(auth, "WPA-PSK") == 0)
    {
        printf("WPA-PSK\n");
        auth = "0x04";
    }

    c = 0;
    v[c++] = "wl";
    v[c++] = "wsec";
    v[c++] = encr;   //4: aes, 2: tkip
    v[c] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "wpa_auth";
    v[c++] = auth;   //0x80: wpa2, 0x04: wpa
    v[c] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "sup_wpa";
    v[c++] = "1";
    v[c++] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "set_pmk"; //password
    v[c++] = key;
    v[c++] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "join";
    v[c++] = gstassid;  //SSID
    v[c++] = "imode";
    v[c++] = "bss";
    v[c++] = "amode";
    v[c++] = "wpa2psk";
    v[c++] = 0;
    wlu_main(argc, v);

    init_all_network_interfaces();

}



void wifi_init(void)
{
    brcm_set_dbg_level(0);
    //brcm_test();
    printf("wifi init\r\n");
}

void wifi_down(void)
{
    char *v[32];
    int c;
    int argc = 3;

    c = 0;
    v[c++] = "wl";
    v[c++] = "down";
    v[c] = 0;
    wlu_main(argc, v);

    eCos_Dhcp_Client_Stop();
    eCos_Dhcp_Server_Stop(0);

    interface_down("wlan0");

    printf("wifi down\r\n");
}
#if (wps)
void wifi_ap_wpa2(void)
{
    char *v[32];
    int c;
    int argc = 3;

    dhd_register_wifi_status_callback(wifi_callback_cb_fn);
    dhd_register_wifi_ap_status_callback(wifi_ap_callback_cb_fn);

    set_to_wps(gapssid, gappwd);

    c = 0;
    v[c++] = "wl";
    v[c++] = "down";
    v[c] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "mpc";
    v[c++] = "0";
    v[c] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "ap";
    v[c++] = "1";
    v[c] = 0;
    wlu_main(argc, v);


    c = 0;
    v[c++] = "wl";
    v[c++] = "wsec";
    //v[c++] = "4";   //4: aes, 2: tkip, wps: 0x40
    v[c++] = "0x44";
    v[c] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "wpa_auth";
    v[c++] = "0x80";   //0x80: wpa2, 0x04: wpa
    v[c] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "sup_wpa";
    v[c++] = "1";
    v[c++] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "set_pmk"; //password
    v[c++] = gappwd;
    v[c++] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "up";
    v[c] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "ssid";
    v[c++] = gapssid;  //SSID
    v[c++] = 0;
    wlu_main(argc, v);

    interface_config("wlan0", "192.168.1.254", "255.255.255.0");
    eCos_Dhcp_Set_Interface("wlan0");
    eCos_Dhcp_Server_Start("DhcpSrv01");

    printf("wifi ap wpa2, %s %s\r\n", gapssid, gappwd);
}
#endif
void set_auto_channel(void)
{
    char *v[32];
    int c;
    int argc = 7;

    c = 0;
    v[c++] = "wl";
    v[c++] = "down";
    v[c] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "mpc";
    v[c++] = "0";
    v[c] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "infra";
    v[c++] = "1";
    v[c] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "up";
    v[c] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "ssid";
    v[c++] = "";
    v[c] = 0;
    wlu_main(argc, v);

    cyg_thread_delay(2000);
    c = 0;
    v[c++] = "wl";
    v[c++] = "autochannel";
    v[c++] = "1";
    v[c] = 0;
    wlu_main(argc, v);

    cyg_thread_delay(3000);//Need time to scan all channels,it might be longer

    c = 0;
    v[c++] = "wl";
    v[c++] = "autochannel";
    v[c] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "autochannel";
    v[c++] = "2";
    v[c] = 0;
    wlu_main(argc, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "channel";
    v[c] = 0;
    wlu_main(argc, v);

}
#if (wps)
void del_my_ie(void)
{
#if 0
    char *v[32];
    int c, argc =0;

    //wps probrsp IE
    c = 0;
    v[c++] = "wl";
    v[c++] = "del_ie";
    v[c++] = "2";
    v[c++] = "135";
    v[c++] = "00:50:f2";
    v[c++] = "04104a00011010440001021041000101101200020004105300020284103b000103104700102221020304050607080900904cc512381021000842726f6164636f6d10230006536f66744150102400013010420001301054000800060050f20400011011000a736f667461705f777073100800020284103c0001011049000600372a000120";
    v[c++] = 0;
    wlu_main(argc, v);

    //wps beacon IE
    c = 0;
    v[c++] = "wl";
    v[c++] = "del_ie";
    v[c++] = "1";
    v[c++] = "67";
    v[c++] = "00:50:f2";
    v[c++] = "04104a000110104400010210410001011012000200041053000202841049000600372a0001201054000800060050f20400011011000a736f667461705f777073";
    v[c++] = 0;
    wlu_main(argc, v);

    //wps assocRsp IE
    c = 0;
    v[c++] = "wl";
    v[c++] = "del_ie";
    v[c++] = "4";
    v[c++] = "24";
    v[c++] = "00:50:f2";
    v[c++] = "04104a000110103b0001031049000600372a000120";
    v[c++] = 0;
    wlu_main(argc, v);
#endif


}

void pre_add_wps_ie(void)
{

    char *v[32];
    int c;
    int argc = 7;

    //wps beacon IE
    c = 0;
    v[c++] = "wl";
    v[c++] = "add_ie";
    v[c++] = "1";
    v[c++] = "67";
    v[c++] = "00:50:f2";
    v[c++] = "04104a000110104400010210410001011012000200041053000202841049000600372a0001201054000800060050f20400011011000a736f667461705f777073";
    v[c++] = 0;
    wlu_main(argc, v);

    //wps probrsp IE
    c = 0;
    v[c++] = "wl";
    v[c++] = "add_ie";
    v[c++] = "2";
    v[c++] = "135";
    v[c++] = "00:50:f2";
    v[c++] = "04104a00011010440001021041000101101200020004105300020284103b000103104700102221020304050607080900904cc512381021000842726f6164636f6d10230006536f66744150102400013010420001301054000800060050f20400011011000a736f667461705f777073100800020284103c0001011049000600372a000120";
    v[c++] = 0;
    wlu_main(argc, v);

}

void set_infra(void)
{
    char *v[32];
    int c = 0;
    int argc = 3;

    v[c++] = "wl";
    v[c++] = "infra";
    v[c++] = "1";
    v[c] = 0;
    wlu_main(argc, v);
}
#endif

MAINFUNC_ENTRY(wifi,argc,argv)
{
    char *v[32];
    int c;
    int ra;
    char t[1024];
    int num,i;
    simple_scan_result *list;

    dhd_register_wifi_status_callback(wifi_callback_cb_fn);
    dhd_register_wifi_ap_status_callback(wifi_ap_callback_cb_fn);

    if (argc < 2)
        goto show_help;
#if (wps)
    if (strcmp(argv[1], "wps-sta-pin") == 0)
    {
        dhd_register_wps_sta_callback(wifi_wps_sta_callback_cb_fn);
        set_role_flag(1);
        set_pwd_flag(1);
        set_infra();
        init_wps(argc, argv);
        wps_start();
        return 0;
    }
    if (strcmp(argv[1], "wps-sta-pbc") == 0)
    {
        dhd_register_wps_sta_callback(wifi_wps_sta_callback_cb_fn);
        set_role_flag(1);
        set_pwd_flag(0);
        set_infra();
        init_wps(argc, argv);
        wps_start();
        return 0;
    }
    if (strcmp(argv[1], "wps-ap-pin") == 0)
    {
        if (argv[2] == NULL)
        {
            printf("You need to input STA PIN number\n");
            goto show_help;
        }
        wifi_ap_wpa2();
        pre_add_wps_ie();
        cyg_thread_delay(2000);
        //del_my_ie();
        set_wps_pin(argv[2]);
        set_role_flag(0);
        set_pwd_flag(1);
        init_wps(argc, argv);
        wps_start();

        interface_config("wlan0", "192.168.1.254", "255.255.255.0");

        eCos_Dhcp_Set_Interface("wlan0");
        eCos_Dhcp_Server_Start("DhcpSrv01");
        return 0;
    }
    if (strcmp(argv[1], "wps-ap-pbc") == 0)
    {
        wifi_ap_wpa2();
        pre_add_wps_ie();
        cyg_thread_delay(2000);
        //del_my_ie();
        set_role_flag(0);
        set_pwd_flag(0);
        init_wps(argc, argv);
        wps_start();

        interface_config("wlan0", "192.168.1.254", "255.255.255.0");

        eCos_Dhcp_Set_Interface("wlan0");
        eCos_Dhcp_Server_Start("DhcpSrv01");
        return 0;
    }
#endif
    if (argc == 2)
    {
        if (strcmp(argv[1], "init") == 0)
        {
            brcm_set_dbg_level(0);
            //brcm_test();
            wifi_start_wlan0_netdev();
            printf("wifi init\r\n");
            return 0;
        }
        else if (strcmp(argv[1], "rand") == 0)
        {
            ra = cyg_arc4random();
            printf("ra = %d\r\n", ra);
            return 0;
        }
        else if (strcmp(argv[1], "get_ethaddr") == 0)
        {
           char ea[6];
            c = 0;
            v[c++] = "wl";
            v[c++] = "cur_etheraddr";
            v[c] = 0;
            wlu_main(argc, v);

            get_eth_addr(ea);

            printf("eth addr = %x:%x:%x:%x:%x:%x\n",ea[0],ea[1],ea[2],ea[3],ea[4],ea[5]);
            return 0;
        }
        else if (strcmp(argv[1], "site_survey") == 0)
        {


            c = 0;
            v[c++] = "wl";
            v[c++] = "ap";
            v[c] = 0;
            wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "scan";
            v[c] = 0;
            wlu_main(argc, v);

            cyg_thread_delay(1000);

            c = 0;
            v[c++] = "wl";
            v[c++] = "scanresults";
            v[c] = 0;
            wlu_main(argc, v);

            get_simple_scan_result(t, &num);
            list = (simple_scan_result *)t;

            for (i = 0;i<num;i++)
                printf("t[%d].ssid = %s, auth = %d, sec = %d\n",i,list[i].ssid,list[i].auth,list[i].sec);


        }
        else if (strcmp(argv[1], "dhcpd") == 0)
        {
            printf("dhcpd\r\n");
            interface_config("wlan0", "192.168.1.254", "255.255.255.0");
            eCos_Dhcp_Set_Interface("wlan0");
            eCos_Dhcp_Server_Start("DhcpSrv01");
        }
        else if (strcmp(argv[1], "dhcpc") == 0)
        {
            printf("dhcpcli\r\n");
            init_all_network_interfaces();
        }
        else if (strcmp(argv[1], "down") == 0)
        {
            wifi_down();
        }
        else if (strcmp(argv[1], "fw") == 0) {
            // dump current firmware type
            dhd_ecos_fop_get_firmware_type();
            return 0;
        }
        else
        {
            goto show_help;
        }

    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], "fw") == 0) {
            if (strcmp(argv[2], "mfg") == 0) {
                // choose mfg firmware
                dhd_ecos_fop_set_firmware_type(0);
            }
            else if (strcmp(argv[2], "normal") == 0) {
                // choose normal firmware
                dhd_ecos_fop_set_firmware_type(1);
            } else {
                printf("wifi fw mfg|normal\n");
            }
            return 0;
        }
        else if (strcmp(argv[1], "ap") == 0)
        {

            //If last time you are sta and connected with one AP, you should kick off this AP first
            //c = 0;
            //v[c++] = "wl";
            //v[c++] = "disassoc";
            //v[c] = 0;
            //wlu_main(argc, v);
             c = 0;
             v[c++] = "wl";
             v[c++] = "deauthenticate";
             v[c] = 0;
             wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "down";
            v[c] = 0;
            wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "ap";
            v[c++] = "1";
            v[c] = 0;
            wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "auth";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "wsec";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "wpa_auth";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "sup_wpa";
            v[c++] = "0";
            v[c++] = 0;
            wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "mpc";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(argc, v);

            /*c = 0;
            v[c++] = "wl";
            v[c++] = "dtim";
            v[c++] = "1";
            v[c] = 0;
            wlu_main(argc, v);*/

            c = 0;
            v[c++] = "wl";
            v[c++] = "ampdu_rx_factor";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "ampdu_ba_wsize";
            v[c++] = "8";
            v[c] = 0;
            wlu_main(argc, v);

            if (strcmp(argv[2], "none") == 0)
            {

            }
            else if (strcmp(argv[2], "autoch") == 0)
            {
                set_auto_channel();
            }
            else if (strcmp(argv[2], "wep") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "wsec";
                v[c++] = "1";
                v[c] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "addwep";
                v[c++] = "0";//can be set 0,1,2,3 for each index
                v[c++] = "1234567890";
                v[c++] = 0;
                //strcpy(v[2], argv[2]);
                wlu_main(argc, v);

            }
            else if (strcmp(argv[2], "wpa-psk-tkip") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "wsec";
                v[c++] = "2";   //4: aes, 2: tkip
                v[c] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "wpa_auth";
                v[c++] = "0x04";   //0x80: wpa2, 0x04: wpa
                v[c] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "sup_wpa";
                v[c++] = "1";
                v[c++] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "set_pmk"; //password
                v[c++] = gappwd;
                v[c++] = 0;
                //strcpy(v[2], argv[2]);
                wlu_main(argc, v);

            }
            else if (strcmp(argv[2], "wpa2-psk-aes") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "wsec";
                v[c++] = "0x44";   //4: aes, 2: tkip, 0x40:wps
                v[c] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "wpa_auth";
                v[c++] = "0x80";   //0x80: wpa2, 0x04: wpa
                v[c] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "sup_wpa";
                v[c++] = "1";
                v[c++] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "set_pmk"; //password
                v[c++] = gappwd;
                v[c++] = 0;
                //strcpy(v[2], argv[2]);
                wlu_main(argc, v);

            }

            c = 0;
            v[c++] = "wl";
            v[c++] = "up";
            v[c] = 0;
            wlu_main(argc, v);


            c = 0;
            v[c++] = "wl";
            v[c++] = "ssid";
            v[c++] = gapssid;  //SSID
            v[c++] = 0;
            wlu_main(argc, v);

            interface_config("wlan0", "192.168.1.254", "255.255.255.0");

            eCos_Dhcp_Set_Interface("wlan0");
            eCos_Dhcp_Server_Start("DhcpSrv01");
            return 0;
        }
        else if (strcmp(argv[1], "sta") == 0)
        {

            //eCos_Dhcp_Client_Stop();
            //eCos_Dhcp_Server_Stop(0);
            c = 0;
            v[c++] = "wl";
            v[c++] = "down";
            v[c] = 0;
            wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "ap";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "auth";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "wsec";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "wpa_auth";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "mpc";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(argc, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "up";
            v[c] = 0;
            wlu_main(argc, v);

            if (strcmp(argv[2], "none") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "join";
                v[c++] = gstassid;  //SSID
                v[c++] = 0;
                wlu_main(argc, v);

                //init_all_network_interfaces();
            }
            else if (strcmp(argv[2], "wep") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "addwep";
                v[c++] = "0";
                v[c++] = "1234567890";
                v[c] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "primary_key";
                v[c++] = "0";
                v[c++] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "auth"; //password
                v[c++] = "1";
                v[c++] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "join";
                v[c++] = gstassid;  //SSID
                v[c++] = "imode";
                v[c++] = "bss";
                v[c++] = 0;
                wlu_main(argc, v);

            }
            else if (strcmp(argv[2], "wpa-psk-tkip") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "wsec";
                v[c++] = "2";   //4: aes, 2: tkip
                v[c] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "wpa_auth";
                v[c++] = "0x04";   //0x80: wpa2, 0x04: wpa
                v[c] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "sup_wpa";
                v[c++] = "1";
                v[c++] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "set_pmk"; //password
                v[c++] = gstapwd;
                v[c++] = 0;
                //strcpy(v[2], argv[2]);
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "join";
                v[c++] = gstassid;  //SSID
                v[c++] = "imode";
                v[c++] = "bss";
                v[c++] = "amode";
                v[c++] = "wpapsk";
                v[c++] = 0;
                wlu_main(argc, v);

            }
            else if (strcmp(argv[2], "wpa2-psk-aes") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "wsec";
                v[c++] = "4";   //4: aes, 2: tkip
                v[c] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "wpa_auth";
                v[c++] = "0x80";   //0x80: wpa2, 0x04: wpa
                v[c] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "sup_wpa";
                v[c++] = "1";
                v[c++] = 0;
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "set_pmk"; //password
                v[c++] = gstapwd;
                v[c++] = 0;
                //strcpy(v[2], argv[2]);
                wlu_main(argc, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "join";
                v[c++] = gstassid;  //SSID
                v[c++] = "imode";
                v[c++] = "bss";
                v[c++] = "amode";
                v[c++] = "wpa2psk";
                v[c++] = 0;
                wlu_main(argc, v);


            }

            init_all_network_interfaces();
            return 0;
        }
    }
    else if (argc >= 4)
    {
        //unsigned char ch;

        if (strcmp(argv[1], "ap") == 0)
        {
            if (strcmp(argv[2], "setssid") == 0)
            {
                strcpy((char *)gapssid, argv[3]);
                printf("ap ssid %s\n", gapssid);
                return 0;
            }
            else if (strcmp(argv[2], "setpwd") == 0)
            {
                strcpy((char *)gappwd, argv[3]);
                printf("ap pwd %s\n", gappwd);
                return 0;
            }
/*            else if (strcmp(argv[2], "setch") == 0)
            {
                ch = atoi(argv[3]);
                if ((ch >=1) && (ch <= 14))
                {
                  gch = ch;
                  printf("ap channel %s\n", gch);
                }
            }
*/
        }
        else if (strcmp(argv[1], "sta") == 0)
        {
            if (strcmp(argv[2], "setssid") == 0)
            {
                strcpy((char *)gstassid, argv[3]);
                printf("sta ssid %s\n", gstassid);
                return 0;
            }
            else if (strcmp(argv[2], "setpwd") == 0)
            {
                strcpy((char *)gstapwd, argv[3]);
                printf("sta pwd %s\n", gstapwd);
                return 0;
            }
        }
    }
show_help:
    printf("wifi fw mfg\n");
    printf("wifi fw normal\n");

    printf("wifi init\n");

    printf("wifi ap autoch\n");
    printf("wifi ap none\n");
    printf("wifi ap wep\n");
    printf("wifi ap wpa-psk-tkip\n");
    printf("wifi ap wpa2-psk-aes\n");
    printf("wifi ap setssid x\n");
    printf("wifi ap setpwd x\n");
    //printf("wifi ap setmod x\n");
    //printf("wifi ap setch x\n");
    //printf("wifi ap setht x\n");

    printf("wifi sta none\n");
    printf("wifi sta wep\n");
    printf("wifi sta wpa-psk-tkip\n");
    printf("wifi sta wpa2-psk-aes\n");
    printf("wifi sta setssid x\n");
    printf("wifi sta setpwd x\n");

    printf("wifi site_survey\n");
    printf("wifi get_ethaddr\n");
    printf("wifi rand\n");
    printf("wifi dhcpd\n");
    printf("wifi dhcpc\n");
    printf("wifi down\n");

    printf("wifi wps-sta-pin\n");
    printf("wifi wps-sta-pbc\n");
    printf("wifi wps-ap-pin\n");
    printf("wifi wps-ap-pbc\n");


    return 0;
}

MAINFUNC_ENTRY(wl, argc, argv)
{
    //int ret;
    //int s;

    if (argc < 2)
    return 0;

    wlu_main(argc, argv);
    return 0;
}

MAINFUNC_ENTRY(aaa,argc,argv)
{
    brcm_set_dbg_level(0);
    init_all_network_interfaces();
    return 0;
}
#if 0

MAINFUNC_ENTRY(brcmdbg,argc,argv)
{
    dhd_ecos_dbg(argc, argv);
}

MAINFUNC_ENTRY(brcmcons,argc,argv)
{
    char *endptr = NULL;
    int interval = 0;

    if (argc>1) {
        argv++;
        interval = strtol(*argv, &endptr, 0);
    }
    diag_printf("interval=%x\n", interval);
    brcm_set_console_interval((unsigned int)interval);
    return 0;
}

MAINFUNC_ENTRY(ap, argc, argv)
{
    char *v[32];
    int c;

    if (argc < 2)
    return 0;

    printf("argv[0] = %s\n",argv[0]);
    printf("argv[1] = %s\n",argv[1]);
    printf("argv[2] = %s\n",argv[2]);

        c = 0;
        v[c++] = "wl";
        v[c++] = "down";
        v[c] = 0;
        wlu_main(argc, v);

        c = 0;
        v[c++] = "wl";
        v[c++] = "mpc";
        v[c++] = "0";
        v[c] = 0;
        wlu_main(argc, v);

        c = 0;
        v[c++] = "wl";
        v[c++] = "ap";
        v[c++] = "1";
        v[c] = 0;
        wlu_main(argc, v);


#if 0
        c = 0;
        v[c++] = "wl";
        v[c++] = "wsec";
        v[c++] = "4";
        v[c] = 0;
        wlu_main(argc, v);

        c = 0;
        v[c++] = "wl";
        v[c++] = "wpa_auth";
        v[c++] = "0x80";
        v[c] = 0;
        wlu_main(argc, v);
#endif

        c = 0;
        v[c++] = "wl";
        v[c++] = "up";
        v[c++] = 0;
        wlu_main(argc, v);

        c = 0;
        v[c++] = "wl";
        v[c++] = "ssid";
        v[c++] = "brcm_ap";
        v[c++] = 0;
        //strcpy(v[2], argv[1]);
        wlu_main(argc, v);
#if 0
        c = 0;
        v[c++] = "wl";
        v[c++] = "sup_wpa";
        v[c++] = "1";
        v[c++] = 0;
        wlu_main(argc, v);

        c = 0;
        v[c++] = "wl";
        v[c++] = "set_pmk";
        v[c++] = "12345678";
        v[c++] = 0;
        //strcpy(v[2], argv[2]);
        wlu_main(argc, v);
#endif

    return 0;
}

MAINFUNC_ENTRY(dhcpd,argc,argv)
{
    interface_config("wlan0", "192.168.1.1", "255.255.255.0");
    eCos_Dhcp_Set_Interface("wlan0");
    eCos_Dhcp_Server_Start("DhcpSrv01");

    return 0;
}
#endif
void print_wscd_status(int status)
{
    switch(status) {
    case NOT_USED:
        printf("NOT_USED\n");
        break;
    case PROTOCOL_START:
        printf("PROTOCOL_START\n");
        break;
    case PROTOCOL_PBC_OVERLAPPING:
        printf("PROTOCOL_PBC_OVERLAPPING\n");
        break;
    case PROTOCOL_TIMEOUT:
        printf("PROTOCOL_TIMEOUT\n");
        break;
    case PROTOCOL_SUCCESS:
        printf("PROTOCOL_SUCCESS\n");
        break;
    case SEND_EAPOL_START:
        printf("SEND_EAPOL_START\n");
        break;
    case RECV_EAPOL_START:
        printf("RECV_EAPOL_START\n");
        break;
    case SEND_EAP_IDREQ:
        printf("SEND_EAP_IDREQ\n");
        break;
    case RECV_EAP_IDRSP:
        printf("RECV_EAP_IDRSP\n");
        break;
    case SEND_EAP_START:
        printf("SEND_EAP_START\n");
        break;
    case SEND_M1:
        printf("SEND_M1\n");
        break;
    case RECV_M1:
        printf("RECV_M1\n");
        break;
    case SEND_M2:
        printf("SEND_M2\n");
        break;
    case RECV_M2:
        printf("RECV_M2\n");
        break;
    case RECV_M2D:
        printf("RECV_M2D\n");
        break;
    case SEND_M3:
        printf("SEND_M3\n");
        break;
    case RECV_M3:
        printf("RECV_M3\n");
        break;
    case SEND_M4:
        printf("SEND_M4\n");
        break;
    case RECV_M4:
        printf("RECV_M4\n");
        break;
    case SEND_M5:
        printf("SEND_M5\n");
        break;
    case RECV_M5:
        printf("RECV_M5\n");
        break;
    case SEND_M6:
        printf("SEND_M6\n");
        break;
    case RECV_M6:
        printf("RECV_M6\n");
        break;
    case SEND_M7:
        printf("SEND_M7\n");
        break;
    case RECV_M7:
        printf("RECV_M7\n");
        break;
    case SEND_M8:
        printf("SEND_M8\n");
        break;
    case RECV_M8:
        printf("RECV_M8\n");
        break;
    case PROC_EAP_ACK:
        printf("PROC_EAP_ACK\n");
        break;
    case WSC_EAP_FAIL:
        printf("WSC_EAP_FAIL\n");
        break;
    case HASH_FAIL:
        printf("HASH_FAIL\n");
        break;
    case HMAC_FAIL:
        printf("HMAC_FAIL\n");
        break;
    case PWD_AUTH_FAIL:
        printf("PWD_AUTH_FAIL\n");
        break;
    case PROTOCOL_PIN_NUM_ERR:
        printf("PROTOCOL_PIN_NUM_ERR\n");
        break;
    case PROC_EAP_DONE:
        printf("PROC_EAP_DONE\n");
        break;
    case PROTOCOL_ERR:
        printf("PROTOCOL_ERR\n");
        break;
    default:
        printf("Unknown wscd status\n");
        break;
    }
}


