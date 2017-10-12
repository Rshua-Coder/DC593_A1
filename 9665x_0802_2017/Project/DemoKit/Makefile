#----------------------------------------------------------------------
# set the ROOT directory here
#----------------------------------------------------------------------
ROOT_DIR = ../..

MAKE_COMMON_DIR = $(ROOT_DIR)/MakeCommon
include $(MAKE_COMMON_DIR)/MakeOption.txt
include ModelConfig.txt
include ModelConfig_$(MODEL).txt
include MakeConfig.txt

#----------------------------------------------------------------------
# set the project name here
#----------------------------------------------------------------------
PRJ_NAME = DemoKit

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = . \
    $(ROOT_DIR)/Include

#----------------------------------------------------------------------
# Add/delete the project C source and assembly files here
#----------------------------------------------------------------------
ASM = \
    SrcCode/Kernel/CodeInfo.s

SRC = \
    SrcCode/Kernel/_main.c \
    SrcCode/Kernel/BinInfo.c \
    SrcCode/Kernel/SysCfg.c

ifeq "$(USE_ECOS_KERNEL)" "ON"

SRC += \
    SrcCode/Ecos/ecos_main.c \
    SrcCode/Ecos/test_ecos.c \
    SrcCode/Ecos/twothreads.c \
    SrcCode/Ecos/ecos_FileSys.c \
    SrcCode/Ecos/test_iperf.c \

ifeq "$(NETWORK)" "NETWORK_SDIO_EVB_WIFI"

SRC += \
    SrcCode/Ecos/test_wifi.c \

endif

ifeq "$(NETWORK)" "NETWORK_SDIO_EVB_WIFI_BRCM"
SRC += \
    SrcCode/Ecos/test_brcwifi.c \
	
endif

endif

################### CARDV ######################

SRC += \
    SrcCode/System/main.c \
    SrcCode/System/SysMain.c \
    SrcCode/System/SysMain_Exe.c \
    SrcCode/System/SysMain_Flow_Init.c \
    SrcCode/System/SysMain_Flow_Exit.c \
    SrcCode/System/SysMain_Flow_Mode.c \
    SrcCode/System/SysMain_Flow_Sleep.c \
    SrcCode/System/SysMain_State.c \
    SrcCode/System/SysLock.c \
    SrcCode/System/SysCmdCB.c \
    SrcCode/System/SysDumpVerInfo.c \
    SrcCode/System/SysSoundCB.c \
    SrcCode/System/SysOutput_Exe.c \
    SrcCode/System/SysVideo_Exe.c \
    SrcCode/System/SysAudio_Exe.c \
    SrcCode/System/SysUsb_Exe.c \
    SrcCode/System/SysStrg_Exe.c \
    SrcCode/System/SysInput_Exe.c \
    SrcCode/System/SysPower_Exe.c \
    SrcCode/System/SysSensor_Exe.c \
    SrcCode/System/SysLens_Exe.c \
    SrcCode/ProjectInfo.c \
    SrcCode/UI/SysUICB.c \
    SrcCode/UI/UIDisplay.c \
    SrcCode/UI/UIGraphics.c \
    SrcCode/UI/UIGraphicsID.c \
    SrcCode/UI/UIView.c \
    SrcCode/UI/PlaybackView.c \
    SrcCode/UI/PhotoView.c \
    SrcCode/UI/MovieView.c \
    SrcCode/AppObj/UIAppObj.c \
    SrcCode/AppObj/UIMoviePlayObj.c \
    SrcCode/AppObj/UIMovieRecObj.c \
    SrcCode/AppObj/UIPhotoObj.c \
    SrcCode/AppObj/UIPlayObj.c \
    SrcCode/AppObj/UIPrinterObj.c \
    SrcCode/UIApp/DpofVendor.c  \
    SrcCode/UIApp/ExifVendor.c  \
    SrcCode/UIApp/MovieUdtaVendor.c \
    SrcCode/UIApp/MovieStamp/MovieStamp.c \
    SrcCode/UIApp/MovieStamp/DateStampFont10x16.c \
    SrcCode/UIApp/MovieStamp/DateStampFont12x20.c \
    SrcCode/UIApp/MovieStamp/DateStampFont18x30.c \
    SrcCode/UIApp/MovieStamp/DateStampFont20x44.c \
    SrcCode/UIApp/MovieStamp/DateStampFont26x44.c \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb.c  \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_Photo.c  \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_Adj.c  \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_UpdFw.c  \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_CustomSi.c \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_IQSim.c \
    SrcCode/UIApp/HttpLview/UIAppHttpLview.c \
    SrcCode/UIApp/PipView.c \
    SrcCode/UIApp/Jpg2Movie/Jpg2Movie.c \
    SrcCode/UIApp/UserCmd/UserCmd.c \
    SrcCode/UIApp/UserCmd/UserCmdID.c \
    SrcCode/UIApp/CmdMapping/UIAppCmdMapping.c \
    SrcCode/UIApp/CmdMapping/UIAppCmdDispatch.c \
    SrcCode/UIApp/CmdMapping/UIAppCmdMapTlb.c  \
    SrcCode/DevCtrl/DevCtrl_Lens.c \
    SrcCode/DevCtrl/DevCtrlPhoto.c \
    SrcCode/DevCtrl/DevCtrlMovie.c \
    SrcCode/DevCtrl/DevCtrlPlayback.c \

################### UIMode ######################
SRC += \
    SrcCode/Mode/UIMode.c \
    SrcCode/Mode/UIModeMain.c \
    SrcCode/Mode/UIModeWifi.c \

################### UIWnd ######################
ifeq "$(UI_STYLE)" "UI_STYLE_DRIVER"
SRC += \
	SrcCode/UIWnd/DRIVER/UIInfo/UIMovieMapping.c \
	SrcCode/UIWnd/DRIVER/UIInfo/UIPhotoMapping.c \
	SrcCode/UIConfig.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuCommon/MenuCommonItem.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuCommon/MenuCommonOption.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuCommon/TabMenu.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuMovie/MenuMovie.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuPhoto/MenuPhoto.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuPhoto/UIMenuWndPhotoExposure.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuPhoto/UIMenuWndPhotoWB.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuPhoto/UIMenuWndPhotoColor.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuPlay/MenuPlayback.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuPlay/UIMenuWndPlayDel.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuPlay/UIMenuWndPlayConfirmDel.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShow.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShowCB.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuSetup/MenuSetup.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuSetup/UIMenuWndSetupDateTime.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuSetup/UIMenuWndSetupDefaultSetting.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuSetup/UIMenuWndSetupFormat.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuSetup/UIMenuWndSetupFormatConfirm.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuSetup/UIMenuWndSetupVersion.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuWiFi/UIMenuWndWiFiWait.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuWiFi/UIMenuWndWiFiModuleLink.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuWiFi/UIMenuWndWiFiMobileLinkOK.c \
	SrcCode/UIWnd/DRIVER/UIMenu/UIMenuUSB/UIMenuWndUSB.c \
	SrcCode/UIWnd/DRIVER/UIFlow/UIFlowCommon/UIFlowWndWrnMsg.c  \
	SrcCode/UIWnd/DRIVER/UIFlow/UIFlowCommon/UIFlowWndWaitMoment.c  \
	SrcCode/UIWnd/DRIVER/UIFlow/UIFlowCommon/UIFlowWndUSB.c \
	SrcCode/UIWnd/DRIVER/UIFlow/UIFlowCommon/UIFlowWndBatteryLow.c \
	SrcCode/UIWnd/DRIVER/UIFlow/UIFlowPhoto/UIFlowWndPhoto.c  \
	SrcCode/UIWnd/DRIVER/UIFlow/UIFlowPhoto/UIFlowPhotoFuncs.c  \
	SrcCode/UIWnd/DRIVER/UIFlow/UIFlowPhoto/UIFlowPhotoIcons.c  \
	SrcCode/UIWnd/DRIVER/UIFlow/UIFlowMovie/UIFlowWndMovie.c  \
	SrcCode/UIWnd/DRIVER/UIFlow/UIFlowMovie/UIFlowMovieFuncs.c  \
	SrcCode/UIWnd/DRIVER/UIFlow/UIFlowMovie/UIFlowMovieIcons.c  \
    SrcCode/UIWnd/DRIVER/UIFlow/UIFlowPlay/UIFlowWndPlay.c  \
    SrcCode/UIWnd/DRIVER/UIFlow/UIFlowPlay/UIFlowWndPlayThumb.c  \
	SrcCode/UIWnd/DRIVER/UIFlow/UIFlowPlay/UIFlowWndPlayMagnify.c  \
    SrcCode/UIWnd/DRIVER/UIFlow/UIFlowPlay/UIFlowCBSlideShow.c  \
    SrcCode/UIWnd/DRIVER/UIFlow/UIFlowPlay/UIFlowPlayIcons.c  \
    SrcCode/UIWnd/DRIVER/UIFlow/UIFlowPlay/UIFlowPlayFuncs.c \
	SrcCode/UIWnd/DRIVER/UIFlow/UIFlowWifi/UIFlowWndWiFiPhoto.c \
	SrcCode/UIWnd/DRIVER/UIFlow/UIFlowWifi/UIFlowWndWiFiMovie.c \
   
endif

################### Resource ######################
ifeq "$(UI_STYLE)" "UI_STYLE_DRIVER"
SRC += \
	  SrcCode/UIWnd/DRIVER/Resource/UIResource.c \
	  SrcCode/UIWnd/DRIVER/Resource/UIResource2.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/BGDB/BG_Watermark.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/BGDB/BG_Images.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/BGDB/BG_Opening.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/BGDB/screen_160x120.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/Demo_BigFont.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/Demo_SmallFont.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/DemoKit_Font.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/DemoKit_Image.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/DemoKit_Palette.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/DemoKit_String_EN.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/DemoKit_String_DE.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/DemoKit_String_ES.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/DemoKit_String_PO.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/DemoKit_String_IT.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/DemoKit_String_FR.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/DemoKit_String_SC.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/DemoKit_String_TC.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/DemoKit_String_JP.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/DemoKit_String_RU.c \
	  SrcCode/UIWnd/DRIVER/Resource/D320x240_DRIVER/UIResource/SoundData.c
endif

################### UIInfo ######################
ifeq "$(UI_STYLE)" "UI_STYLE_DRIVER"
SRC += \
    SrcCode/UIWnd/DRIVER/UIInfo/UIInfo.c \
    SrcCode/UIWnd/DRIVER/UIInfo/DateTimeInfo.c \
    SrcCode/UIWnd/DRIVER/UIInfo/TPInfo.c \
    SrcCode/UIWnd/DRIVER/UIInfo/GSInfo.c \
    SrcCode/UIWnd/DRIVER/UIInfo/CalibrationInfo.c \
    SrcCode/UIWnd/DRIVER/UIInfo/LensInfo.c
endif

################### UIApp ######################
SRC += \
    SrcCode/UIApp/SensorFP.c \

################### Engineer ######################
SRC += \
    SrcCode/Engineer/CalibrationWnd.c \
    SrcCode/Engineer/Engineer_ID.c \
    SrcCode/Engineer/EngineerModeTbl.c \
    SrcCode/Engineer/DynamicMenu.c \
    SrcCode/Engineer/CalSensor/Cal_Header.c \
    SrcCode/Engineer/CalSensor/CalibrationItem.c \
    SrcCode/Engineer/CalSensor/UI_Sample/IPL_Cal_KeyStrSample.c \
    SrcCode/Engineer/BurnIn/BurnInWnd.c \
    SrcCode/Engineer/BurnIn/KeyTestWnd.c \
    SrcCode/Engineer/BurnIn/MovieRecTestWnd.c \
    SrcCode/Engineer/BurnIn/MoviePlayTestWnd.c \
    SrcCode/Engineer/BurnIn/LCDTestWnd.c \
    SrcCode/Engineer/BurnIn/LEDTestWnd.c \
    SrcCode/Engineer/BurnIn/AudioTestWnd.c \
    SrcCode/Engineer/BurnIn/WavTestWnd.c \
    SrcCode/Engineer/BurnIn/GPSTestWnd.c

################### Network ######################
SRC += \
    SrcCode/UIApp/Network/UIAppNetwork.c \
	SrcCode/UIApp/Network/WifiAppXML.c \
	SrcCode/UIApp/Network/WifiAppCmd.c \
	SrcCode/UIApp/Network/WifiAppCmdMapping.c \

################### Youku ######################
SRC += \
#    SrcCode/UIApp/Youku/YoukuAppCmd.c \
#    SrcCode/UIApp/Youku/StatusData.c \
    	
################### Movie ######################
SRC += \
    SrcCode/Mode/UIModeMovie.c \
    SrcCode/UIApp/Movie/UIAppMovie_Exe.c \
    SrcCode/UIApp/Movie/UIAppMovie_CB.c \
    SrcCode/UIApp/Movie/UIAppMovie_3DNR.c \
    SrcCode/UIApp/Movie/UIAppMovie_AudNR_Spec_Zoom.c \
    SrcCode/UIApp/Movie/UIAppMovie_AudNR_Spec_Focus.c \
    SrcCode/UIApp/Movie/UIAppMovie_AudNR_Spec_Iris.c \


################### Photo ######################
SRC += \
    SrcCode/Mode/UIModePhoto.c \
    SrcCode/UIApp/Photo/UIAppPhoto_Exe.c \
    SrcCode/UIApp/Photo/UIAppPhoto_CB.c \
    SrcCode/UIApp/Photo/UIDateImprint.c \
    SrcCode/UIApp/Photo/UIStorageCheck.c \

################### Play ######################
SRC += \
    SrcCode/Mode/UIModePlayback.c \
    SrcCode/UIApp/Play/UIAppPlay_Exe.c \
    SrcCode/UIApp/Play/UIAppPlay_CB.c \
    SrcCode/UIApp/Play/UIPlaySlide.c \
    SrcCode/UIApp/Play/UIPlayWallpaper.c \
    SrcCode/UIApp/Play/UIPlayComm.c \

################### Setup ######################
SRC += \
    SrcCode/UIApp/Setup/UISetup_Exe.c \

################### Background ######################
SRC += \
    SrcCode/UIApp/Background/UIBackgroundObj.c \

################### UsbCam ######################
SRC += \
    SrcCode/Mode/UIModeUsbCam.c \
    SrcCode/UIApp/UsbCam/UIAppUsbCam_Exe.c \

################### UsbDisk ######################
SRC += \
    SrcCode/Mode/UIModeUsbDisk.c \
    SrcCode/UIApp/UsbDisk/UIAppUsbDisk_Exe.c \

################### UsbPrint ######################
SRC += \
    SrcCode/Mode/UIModeUsbPrint.c \
    SrcCode/UIApp/UsbPrint/UIAppUsbPrint_Exe.c \
    SrcCode/UIApp/UsbPrint/UIAppUsbPrint_CB.c \


################### UsbPTP ######################
SRC += \
    SrcCode/Mode/UIModeUsbPTP.c \

################### UsbCharge ######################
SRC += \
    SrcCode/Mode/UIModeUsbCharge.c \
    SrcCode/UIApp/UsbCharge/UIAppUsbCharge_Exe.c \

################### UsbMenu ######################
SRC += \
    SrcCode/Mode/UIModeUsbMenu.c \


#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = $(PRJ_NAME)_Data

#----------------------------------------------------------------------
# set the library directory here
#----------------------------------------------------------------------
ARC_BASE_DIR = $(ROOT_DIR)/ARC

#----------------------------------------------------------------------
# add additional C and assembly flags here, for example, -Otime
#----------------------------------------------------------------------
CFLAGS_D = $(C_DEFINE_EXT)

ASMFLAGS_D = \
    $(CHIP_PARAM_ASM)   \
    $(FWLOAD_PARAM_ASM)

CFLAGS_R = $(C_DEFINE_EXT)

ASMFLAGS_R = \
    $(CHIP_PARAM_ASM)   \
    $(FWLOAD_PARAM_ASM)

GCCFLAGS = $(C_DEFINE_EXT)

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
include $(MAKE_COMMON_DIR)/OutputImg.txt
