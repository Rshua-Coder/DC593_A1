
#----------------------------------------------------------------------
# include make config file
#----------------------------------------------------------------------
include MakeConfig.txt

#----------------------------------------------------------------------
# set the project name here
#----------------------------------------------------------------------
PRJ_NAME = Loader$(CHIP)

#----------------------------------------------------------------------
# set the scatter file name here
#----------------------------------------------------------------------
ifeq "$(BL_COMPRESS)" "ENABLE"
SCATTER = Loader$(CHIP)LZ.scf
else
ifeq "$(STORAGEINT)" "SPIFlash"
SCATTER = Loader$(CHIP)_SPI.scf
else
SCATTER = Loader$(CHIP).scf
endif
endif
SCATTER_D = $(PRJ_NAME)_D.lds
ifeq "$(BL_COMPRESS)" "ENABLE"
ifeq "$(CHIP_VER)" "A"
SCATTER_R = $(PRJ_NAME)LZ_A_R.lds
else
SCATTER_R = $(PRJ_NAME)LZ_R.lds
endif
else
SCATTER_R = $(PRJ_NAME)_R.lds
endif

#----------------------------------------------------------------------
# set loader execution DRAM address
#----------------------------------------------------------------------
# 255MB
ifeq "$(SDRAM_SIZE)" "1024"
LD_BASE_ADDR = 0x7F00000
else ifeq "$(SDRAM_SIZE)" "2048"
LD_BASE_ADDR = 0xFF00000
else
LD_BASE_ADDR = 0x0000000 
endif
BL_LDADDR_PARAM = \
-DLD_BASE_ADDR=$(LD_BASE_ADDR)

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = . \
	../../Include

#----------------------------------------------------------------------
# add/delete the project assembly files here
#----------------------------------------------------------------------
#ASM = Src/reset.S \
#Src/remap.S
        
        
        

#----------------------------------------------------------------------
# add/delete the project C source files here
#----------------------------------------------------------------------
SRC = \
	Src/main.c \
	Src/bl_func.c
	
	
	

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = $(PRJ_NAME)_Data

#----------------------------------------------------------------------
# set the library directory here
#----------------------------------------------------------------------
LIB_BASE_DIR = ../../LIB

#----------------------------------------------------------------------
# add additional C and assembly flags here, for example, -Otime
#----------------------------------------------------------------------
CFLAGS_R = \
	$(MODEL_PARAM) \
	$(CHIP_PARAM) \
	$(PCB_STAGE_PARAM) \
	$(DRAMTYPE_PARAM) \
	$(DRAMSIZE_PARAM) \
	$(BL_UPDATE_NAME_PARAM) \
	$(BL_STRINGS_PARAM) \
	$(BL_COMPRESS_PARAM) \
	$(PCB_STAGE_PARAM) \
	$(CHKBOOTIMAGE_PARAM) \
	$(CHKFWIMAGE_PARAM) \
	$(LEDFUNCTION_PARAM) \
	$(FWDECOMPRESS_PARAM) \
	$(FWCOMPRESS_PARAM) \
	$(CALFW_UPDATE_PARAM) \
	$(BL_LOADER_VER_PARAM) \
	$(BL_FWSIZE_PARAM) \
  $(BL_LDADDR_PARAM) \
	$(FWKEY_PARAM) \
	$(STORAGE_INT_TYPE)\
	$(DRAMSCAN_PARAM)\
	$(BL_DUTY_CALIBRATION_PARAM)\
	$(BL_DUTY_CALIBRATION_TYPE_PARAM)\
	$(BL_LOADER_FLOW_PARAM)

ASMFLAGS_R = \
	$(CHIP_PARAM_ASM) \
	$(DRAMTYPE_PARAM_ASM) \
	$(DRAMSIZE_PARAM_ASM) \
	$(BL_COMPRESS_PARAM_ASM)

GCCFLAGS = \
	$(MODEL_PARAM) \
	$(CHIP_PARAM) \
	$(PCB_STAGE_PARAM) \
	$(DRAMTYPE_PARAM) \
	$(DRAMSIZE_PARAM) \
	$(BL_UPDATE_NAME_PARAM) \
	$(BL_STRINGS_PARAM) \
	$(BL_COMPRESS_PARAM) \
	$(PCB_STAGE_PARAM) \
	$(CHKBOOTIMAGE_PARAM) \
	$(CHKFWIMAGE_PARAM) \
	$(LEDFUNCTION_PARAM) \
	$(FWDECOMPRESS_PARAM) \
	$(FWCOMPRESS_PARAM) \
	$(CALFW_UPDATE_PARAM) \
	$(BL_LOADER_VER_PARAM) \
	$(FWKEY_PARAM) \
	$(STORAGE_INT_TYPE)\
	$(DRAMSCAN_PARAM)\
	$(BL_DUTY_CALIBRATION_PARAM)\
	$(BL_DUTY_CALIBRATION_TYPE_PARAM)\
	$(BL_LOADER_FLOW_PARAM)

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR = ../../MakeCommon
include $(MAKE_COMMON_DIR)/OutputImg.txt
