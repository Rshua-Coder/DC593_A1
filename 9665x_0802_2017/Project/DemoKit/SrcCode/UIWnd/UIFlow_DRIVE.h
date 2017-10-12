#ifndef _UIFLOW_TURNKEY_H
#define _UIFLOW_TURNKEY_H

#include "PrjCfg.h"
#include "SysCfg.h"
//UIApp
#include "SysCommon.h"
#include "AppCommon.h"

//System
#include "UICommon.h"

//UI Display
#include "DRIVER/Resource/D320x240_DRIVER/BGDB/BG_Images.h"
#include "DRIVER/Resource/D320x240_DRIVER/BGDB/BG_Opening.h"
#include "DRIVER/Resource/D320x240_DRIVER/BGDB/screen_160x120.h"

//UI Info
#include "UIPlayObj.h"
#include "UISetup.h"
#include "UIStorageCheck.h"
#include "UICommon.h"
#include "SoundData.h"

#include "DRIVER/UIInfo/UIInfo.h"
//#include "DRIVER/UIInfo/UICfgDefault.h"
//#include "DRIVER/UIInfo/UISystemStatusSettings.h"
//#include "DRIVER/UIInfo/UIMenuPrintSettings.h"


//UIFlow common
#include "DRIVER/UIFlow/UIFlowCommon/UIFlowWndWrnMsg.h"
#include "DRIVER/UIFlow/UIFlowCommon/UIFlowWndWrnMsgAPI.h"
#include "DRIVER/UIFlow/UIFlowCommon/UIFlowWndWaitMoment.h"
#include "DRIVER/UIFlow/UIFlowCommon/UIFlowWndUSB.h"
#include "DRIVER/UIFlow/UIFlowCommon/UIFlowWndUSBAPI.h"
#include "DRIVER/UIFlow/UIFlowCommon/UIFlowWndBatteryLow.h"


#if 0
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowCommonIcons.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndFwVersion.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndUSB.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndUSBAPI.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndWaitMoment.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndWaitMomentAPI.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndWrnMsg.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndWrnMsgAPI.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndWrnStrgHotPlug.h"
#endif

//UIFlow movie
#include "DRIVER/UIFlow/UIFlowMovie/UIFlowMovieFuncs.h"
#include "DRIVER/UIFlow/UIFlowMovie/UIFlowMovieIcons.h"
#include "DRIVER/UIFlow/UIFlowMovie/UIFlowWndMovie.h"


//UIFlow photo
#include "DRIVER/UIFlow/UIFlowPhoto/UIFlowPhotoFuncs.h"
#include "DRIVER/UIFlow/UIFlowPhoto/UIFlowPhotoIcons.h"
#include "DRIVER/UIFlow/UIFlowPhoto/UIFlowWndPhoto.h"

//UIFlow play
#include "DRIVER/UIFlow/UIFlowPlay/UIFlowWndPlay.h"
//#include "DRIVER/UIFlow/UIFlowPlay/UIFlowWndPlayNoFile.h"
#include "DRIVER/UIFlow/UIFlowPlay/UIFlowPlayFuncs.h"
#include "DRIVER/UIFlow/UIFlowPlay/UIFlowPlayIcons.h"
//#include "DRIVER/UIFlow/UIFlowPlay/UIFlowWndPlayMagnify.h"
#include "DRIVER/UIFlow/UIFlowPlay/UIFlowWndPlayThumb.h"

//UIFlow WiFi
#include "DRIVER/UIFlow/UIFlowWiFi/UIFlowWndWiFiMovie.h"
#include "DRIVER/UIFlow/UIFlowWiFi/UIFlowWndWiFiPhoto.h"
#include "DRIVER/UIFlow/UIFlowWiFi/UIFlowWndWiFiPlayback.h"
#include "DRIVER/UIMenu/UIMenuWiFi/UIMenuWndWiFiModuleLink.h"


//UIFlow printer
//#include "DRIVE/UIFlow/UIFlowPrint/UIFlowWndPrint.h"

//UIMenu Common
#include "DRIVER/UIMenu/UIMenuCommon/TabMenu.h"
#include "DRIVER/UIMenu/UIMenuCommon/MenuId.h"
#include "DRIVER/UIMenu/UIMenuCommon/MenuCommonItem.h"   //Item
#include "DRIVER/UIMenu/UIMenuCommon/MenuCommonOption.h" //Option

//UIMenu calibration
#include "DRIVER/UIMenu/UIMenuCalibration/UIMenuWndCalibration.h"

//UIMenu movie
#include "DRIVER/UIMenu/UIMenuMovie/MenuMovie.h"

//UIMenu photo
#include "DRIVER/UIMenu/UIMenuPhoto/MenuPhoto.h"
#include "DRIVER/UIMenu/UIMenuPhoto/UIMenuWndPhotoColor.h"
#include "DRIVER/UIMenu/UIMenuPhoto/UIMenuWndPhotoExposure.h"
#include "DRIVER/UIMenu/UIMenuPhoto/UIMenuWndPhotoQuickSetting.h"
#include "DRIVER/UIMenu/UIMenuPhoto/UIMenuWndPhotoWB.h"


//UIMenu playback
#include "DRIVER/UIMenu/UIMenuPlay/MenuPlayback.h"
#include "DRIVER/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShow.h"
#include "DRIVER/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShowCB.h"
#include "DRIVER/UIMenu/UIMenuPlay/UIMenuWndPlayDel.h"
#include "DRIVER/UIMenu/UIMenuPlay/UIMenuWndPlayConfirmDel.h"

#if 0
#include "DRIVE/UIMenu/UIMenuPlay/MenuPlayback.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayConfirmDel.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayCopyToCard.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayDel.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayDPOF.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayProtect.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayQuickConfirmDel.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayQuickDel.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlaySetDPOF.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShow.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShowCB.h"
#endif

//UIMenu printer
#if 0
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint.h"
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_Confirm.h"
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_Crop.h"
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintAll.h"
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintDPOF.h"
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintProgress.h"
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintSetting.h"
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_SelectImages.h"
#endif

//UIMenu Setup
#include "DRIVER/UIMenu/UIMenuSetup/MenuSetup.h"
#include "DRIVER/UIMenu/UIMenuSetup/MenuSetup.h"
#include "DRIVER/UIMenu/UIMenuSetup/UIMenuWndSetupDateTime.h"
#include "DRIVER/UIMenu/UIMenuSetup/UIMenuWndSetupDefaultSetting.h"
#include "DRIVER/UIMenu/UIMenuSetup/UIMenuWndSetupFormat.h"
#include "DRIVER/UIMenu/UIMenuSetup/UIMenuWndSetupFormatConfirm.h"
//#include "DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupLoaderVersion.h"
#include "DRIVER/UIMenu/UIMenuSetup/UIMenuWndSetupVersion.h"

//UIMenu WiFi
#include "DRIVER/UIMenu/UIMenuWiFi/UIMenuWndWiFiWait.h"
#include "DRIVER/UIMenu/UIMenuWiFi/UIMenuWndWiFiModuleLink.h"
#include "DRIVER/UIMenu/UIMenuWiFi/UIMenuWndWiFiMobileLinkOK.h"

//UIMenu USB
//#include "DRIVE/UIMenu/UIMenuUSB/UIMenuWndUSB.h"



#endif
