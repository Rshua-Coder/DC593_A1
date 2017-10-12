/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       PrjCfg.h
    @ingroup

    @brief

    @note       Nothing.
*/

#ifndef _PRJCFG_COMM_H_
#define _PRJCFG_COMM_H_

#define _DEMO_TODO                      0
//#define _DEMO_TODO                        1

////////////////////////////////////////////////////////////////////////////////
// Project

#define PRJ_DBG_LVL_NONE                0
#define PRJ_DBG_LVL_ERR                 1
#define PRJ_DBG_LVL_TRC                 2

//reference bigger brother "DEBUG_LEVEL"
#if DEBUG_LEVEL
#define PRJ_DBG_LVL                     PRJ_DBG_LVL_ERR //switch project debug msg
#else
#define PRJ_DBG_LVL                     PRJ_DBG_LVL_NONE
#endif


////////////////////////////////////////////////////////////////////////////////
#define _CUSTOMER_A_                    0
#define _CUSTOMER_N_                    1

//..............................................................................
// Audio format
#define _VM_FORMAT_PCM8_                0
#define _VM_FORMAT_PCM16_               1
#define _VM_FORMAT_ADPCM_               2
#define _VM_FORMAT_ASFPCM_              3

//..............................................................................
// MSDC modes : For the MSDC card HotPlug & Multi-LUNs function
#define _MSDC_SINGLE_                   0           // Only the current active storage device can be accessed through USB MSDC, i.e. card "OR" nand.
#define _MSDC_ALL_                      1           // All the storage devices can be accessed concurrently through USB MSDC, i.e. card "AND" nand.
#define _MSDC_SINGLE_NAND2_             2           // Only the active storage device plus the NAND2 can be accessed concurrently through USB MSDC.
#define _MSDC_ALL_NAND2_                3           // All the storage devices plus the NAND2 can be accessed concurrently through USB MSDC.

//..............................................................................
#define MOVIE_FORMAT_AVI                0
#define MOVIE_FORMAT_MOV                1

//..............................................................................
#define PANCAPTURE_MODE_AUTO            0
#define PANCAPTURE_MODE_MANIAL          1
#define PANCAPTURE_SIZE_2M              0
#define PANCAPTURE_SIZE_3M              1

//..............................................................................
#define _MAX_IMAGE_SIZE_14M_            0           // 14M
#define _MAX_IMAGE_SIZE_12M_            1           // 12M
#define _MAX_IMAGE_SIZE_10M_            2           // 10M

//..............................................................................
#define _INSIDECODE_                    0
#define _PSTORE_                        1

//..............................................................................
#define _LOGO_NOVATEK_                  0
#define _LOGO_GE_                       1
#define _LOGO_POLAROID_                 2
#define _LOGO_AGFA_                     3

//..............................................................................
#define _SCREENNAIL_SIZE_VGA_           0
#define _SCREENNAIL_SIZE_3M_            1

#define _QUICKVIEW_SIZE_VGA_            0
#define _QUICKVIEW_SIZE_SUIT_FOR_DISP   1


#define _FD_FUNC_                       DISABLE
#define _SD_FUNC_                       DISABLE
#define _LDWS_FUNC_                     DISABLE
//#define _MULTI_RECORD_FUNC_             DISABLE

#define CARDV_ON_MEDIA_BOX_DEMO         DISABLE   //for realtek mdeia box demo

#if (CARDV_ON_MEDIA_BOX_DEMO ==ENABLE)
#define AUTO_WIFI                       DISABLE//ENABLE
#else
#define AUTO_WIFI                       DISABLE
#endif

#define YOUKU_SDK                       DISABLE
#if (YOUKU_SDK == ENABLE)
#define WIFI_MOVIE_FOLDER             "MOVIE_WIFI"
#endif

//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
#ifndef WIFI_UI_FLOW_VER
#define WIFI_UI_VER_1_0                 0
#define WIFI_UI_VER_2_0                 1
#define WIFI_UI_FLOW_VER                WIFI_UI_VER_1_0
#endif
//#NT#2016/03/23#Isiah Chang -end

#define IP_LIMITE                       DISABLE

#define MAC_APPEN_SSID                  ENABLE

#define ACL_TIME                        15000 //minmum 15 sec

#define SENSOR2_DISP_ONLY				DISABLE

#define _SMALL_CLONE_MOVIE_             DISABLE
#define DV_ROOT_PATH                    "A:\\CarDV\\"
#define MOVIE_ROOT_PATH                 "A:\\CarDV\\Movie\\"
#define MOVIE_FOLDER                    "Movie"
#define PHOTO_FOLDER                    "Photo"
#define MOVIE_RO_FOLDER                 "Movie\\RO"
#define MOVIE_SHARE_FOLDER              "MovieShare"
#define MOVIE_SHARE_RO_FOLDER           "MovieShare\\RO"
#define PLAY_FOLDER_FILTER              "Movie|Photo"

#if (_SMALL_CLONE_MOVIE_ == ENABLE)
#undef _MULTI_RECORD_FUNC_
#define _MULTI_RECORD_FUNC_             ENABLE
#else
#undef _MULTI_RECORD_FUNC_
#define _MULTI_RECORD_FUNC_             DISABLE
#endif

#ifndef _QR_FUNC_
#define _QR_FUNC_                       DISABLE
#endif

#endif //_PRJCFG_COMM_H_
