#include_next <pkgconf/system.h>
#define RTLPKG_DEVS_ETH_RLTK_819X_WRAPPER v3_0
#define RTLPKG_DEVS_ETH_RLTK_819X_WRAPPER_v3_0
#define RTLNUM_DEVS_ETH_RLTK_819X_WRAPPER_VERSION_MAJOR 3
#define RTLNUM_DEVS_ETH_RLTK_819X_WRAPPER_VERSION_MINOR 0
#define RTLNUM_DEVS_ETH_RLTK_819X_WRAPPER_VERSION_RELEASE -1
#define CONFIG_RTL_KERNEL_TIMER 1
#define CONFIG_PRINTK 1
//#include_next <pkgconf/system.h>
#define RTLPKG_DEVS_ETH_RLTK_819X_WLAN v3_3
#define RTLPKG_DEVS_ETH_RLTK_819X_WLAN_v3_3
#define RTLNUM_DEVS_ETH_RLTK_819X_WLAN_VERSION_MAJOR 3
#define RTLNUM_DEVS_ETH_RLTK_819X_WLAN_VERSION_MINOR 3
#define RTLNUM_DEVS_ETH_RLTK_819X_WLAN_VERSION_RELEASE -1
/***** wlan driver proc output start *****/
#define RTLDAT_DEVS_ETH_RLTK_819X_WLAN0_INL "devs_eth_rltk_819x_wlan0.inl"
#define RTLDAT_DEVS_ETH_RLTK_819X_WLAN1_INL "devs_eth_rltk_819x_wlan1.inl"
#define RTLDAT_DEVS_ETH_RLTK_819X_WLAN_CFG <pkgconf/devs_eth_rltk_819x_wlan.h>
/*****  wlan driver proc output end  *****/
#define CYGBLD_DEVS_WLAN_DEVICE_H <pkgconf/devs_eth_rltk_819x_wlan.h>
#define CONFIG_SDIO_HCI 1
#define CONFIG_SDIO_NOVA 1
#define NOT_RTK_BSP 1
#define _LITTLE_ENDIAN_ 1
#define SMP_SYNC 1
#define CONFIG_INIT_NETDEV_BY_API 1
//#define CONFIG_USE_PCIE_SLOT_0 1	//by history following it, by checked no need!20150119
// 2015-01-16 need change to 8821 , need ref RTK sdk .config file
//#define CONFIG_SLOT_0_8192EE 1 // no used
#define CONFIG_WLAN_HAL 1
#define CONFIG_WLAN_HAL_88XX 1
#define CONFIG_WLAN_HAL_8192EE 1

/*under CONFIG_WLAN_HAL_8821AS define , mac use 92es , BB RF use 8881a ; 20150206 add for  8821AS porting to ecos */
#define CONFIG_WLAN_HAL_8821AS 1

#define CONFIG_RTL_ODM_WLAN_DRIVER 1
#define CONFIG_RTL_CLIENT_MODE_SUPPORT 1
//#define CONFIG_RTL_REPEATER_MODE_SUPPORT 1
//#define CONFIG_RTL_WDS_SUPPORT 1
//#define CONFIG_RTL_VAP_SUPPORT 1
#define CONFIG_ENABLE_EFUSE 1
#define CONFIG_RTL_P2P_SUPPORT 1
#define CONFIG_RTL_WPS2_SUPPORT 1
#define CONFIG_TXPWR_LMT 1
#define CONFIG_BAND_5G_ON_WLAN0 1
//#define SDIO_2_PORT 1
#define CONFIG_PHY_WLAN_EAT_40MHZ 1
