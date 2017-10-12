include ../../../../Project/ProjectConfig.txt
include ../../../../Project/$(ACTIVE_PROJECT)/ModelConfig.txt
include ../../../../Project/$(ACTIVE_PROJECT)/ModelConfig_$(MODEL).txt
include ../../../../Project/$(ACTIVE_PROJECT)/MakeConfig.txt

#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = MODELEXT_CARDV_DUAL

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------

SRC = \
    DxCfg.c \
    PinmuxCfg.c \
    IOCfg.c \
    IrRxParser.c \
    DxStorage.c \
    DxStorage_Card.c \
    DxStorage_Card2.c \
    DxStorage_Nand.c \
    DxDisplay.c \
    DxDisplay_LCD.c \
    DxDisplay_TV.c \
    DxDisplay_HDMI.c \
    DxAudio.c \
    DxUsb.c \
    DxWiFi.c \
    DxOutput_LED.c \
    DxInput_Key.c \
    DxInput_Touch.c \
    DxSound_Audio.c \
    DxCmd.c \
    DxPower_Battery.c \
    DxCamera_Sensor.c \
    DxCamera_Flash.c \
    DxCamera_Lens.c


ifeq "$(MODELEXT)" "x"
    DxSound_Audio.c \
    DxCamera_IPL.c \
    DxCamera_Lens.c \
    DxCamera_Sensor.c \
    DxCamera_Flash.c \
    DxPower_Battery_Alkaline.c
endif

ASM =

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = . \
	../../../../Include \

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = ../../../../ARC/Drv

#----------------------------------------------------------------------
# add additional C and assembly flags here
#----------------------------------------------------------------------
CFLAGS_D = $(C_DEFINE_EXT)

ASMFLAGS_D = \
	$(CHIP_PARAM_ASM) \
	$(DRAMSIZE_PARAM_ASM) \
	$(MODEL_PARAM_ASM)

CFLAGS_R = $(C_DEFINE_EXT)

ASMFLAGS_R = \
	$(CHIP_PARAM_ASM) \
	$(DRAMSIZE_PARAM_ASM) \
	$(MODEL_PARAM_ASM)

GCCFLAGS = $(C_DEFINE_EXT)

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR = ../../../../MakeCommon
include $(MAKE_COMMON_DIR)/OutputLib.txt
