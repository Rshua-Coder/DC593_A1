/* Linker script for malta
 *
 * Version: Sourcery G++ Lite 4.3-221
 * Support: https://support.codesourcery.com/GNUToolchain/
 *
 * Copyright (c) 2007, 2008, 2009 CodeSourcery, Inc.
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions.  No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 * */

/* Rule:
 * 1. to use .o in project layer just type the .o name with file extension. (UiFlow.o)
 * 2. to use .o in library layer have to add prefix module name. (DemoKit_Data/Release/ObjectLds/DscSystem/SysTask.o)
 * */

OUTPUT_ARCH(mips)
ENTRY(_start)

/*
MEMORY
{
    ram : ORIGIN = 0x80000000, LENGTH = 0x4000000
}
*/

SECTIONS
{
    .SPECIAL_RO 0x80000000:
    {
        _section_01_addr = ABSOLUTE(.);
        KEEP(DemoKit_Data/Release/ObjectCode/BinInfo.o(.data*));
    }
    .EXCEPTION_TABLE 0x80000180:
    {
        /* KEEP(../../ARC_eCos/Common/Release/OS.a(.text.exp)); */
        /* KEEP(../../ARC_eCos/Common/Release/Startup.a(.other_vector)); */
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.other_vector));
    }
    .VECTOR_TABLE 0x80000200:
    {
        /* hal_vsr_table = ABSOLUTE(.);*/	/* exception code vector table */
        /* hal_virtual_vector_table = ABSOLUTE(.); */	/* ??? TODO ??? */
        __ebase = ABSOLUTE(.);	/* ??? TODO ??? */
        __start_vector_table = ABSOLUTE(.);
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.interrupt_vector*));
        /*KEEP(../../ARC_eCos/Common/Release/Startup.a(.text.isr));*/
        __end_vector_table = ABSOLUTE(.);
    }
    .CODE_INFO 0x800002F0:
    {
        KEEP(DemoKit_Data/Release/ObjectCode/CodeInfo.o(.data*));
    }

    /* _stext = .; */	/* ??? TODO ??? */

    .START_UP 0x80000400:
    {
        _load_entry_start_base = ABSOLUTE(.);
        /* KEEP(../../ARC_eCos/Common/Release/Startup.a(.text.start)); */
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.text.start));
        _load_entry_end_base = ABSOLUTE(.);
    }

    /* all global C++ constructors */
    . = ALIGN(4);

       .ctors ALIGN (0x4) : { . = .; __CTOR_LIST__ = .; PROVIDE (__CTOR_LIST__ = .); KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors*)) __CTOR_END__ = .; PROVIDE (__CTOR_END__ = .); }
       .dtors ALIGN (0x4) : { . = .; __DTOR_LIST__ = .; PROVIDE (__DTOR_LIST__ = .); KEEP (*(SORT(.dtors.*))) KEEP (*(.dtors*)) __DTOR_END__ = .; PROVIDE (__DTOR_END__ = .); }
       .devtab ALIGN (0x4) : { . = .; KEEP(*( SORT (.ecos.table.*))) ; }

    /* kernel common */
    . = ALIGN(4);
    .SECTION_01 :
    {
        /*============================ before os start ===============================  */
        *(.kercfg_data*) /* collect install ID process of all modules */
        *(.kercfg_text*) /* collect install ID process of all modules */
        DemoKit_Data/Release/ObjectCode/_main.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DemoKit_Data/Release/ObjectCode/SysCfg.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ARC_eCos/Common/Release/Kernel.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ARC_eCos/Common/Release/OS.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ARC_eCos/Drv/Release/DxSys.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ARC_eCos/Drv/Release/DSPCFG.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ARC_eCos/Drv/Release/Driver_Sys.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DemoKit_Data/Release/ObjectLds/Driver_IPP/grph.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DemoKit_Data/Release/ObjectLds/Driver_Codec/jpeg.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DemoKit_Data/Release/ObjectLds/Driver_Codec/jpeg_int.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ARC_eCos/Common/Release/Debug.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        LibC.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        libgcc.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ARC_eCos/Common/Release/Utility.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DemoKit_Data/Release/ObjectLds/GxGfx/DC.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ARC_eCos/Drv/Release/DbgUt.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DemoKit_Data/Release/ObjectLds/GxStrg/GxStrg_p1.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)

        /* below is ecos related  begin */
        libstdc++.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ThirdParty/eCos/lib/libtarget.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ThirdParty/eCos/lib/libextras.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ThirdParty/eCos/lib/extras.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ThirdParty/eCos/lib/vectors.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ThirdParty/devs_eCos-sdio/lib/libsdiont96650.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
         ../../ThirdParty/ecos-nvtwifi/lib/libnvtwifi.a( .text* .data*  .rodata* .rodata1* .reginfo*) /*NVTWIFI_TAG*/
         ../../ThirdParty/ecos-rtkwifi/lib/librtkwifi.a( .text* .data*  .rodata* .rodata1* .reginfo*) /*WIFI_ADAPTOR_TAG*/
        ../../ThirdParty/devs_eCos-rtl8189ftves/lib/lib8189ftvwlan.a( .text* .data*  .rodata* .rodata1* .reginfo*) /*WIFI_LINK_TAG1*/
        ../../ThirdParty/devs_eCos-rtl8189ftves/lib/lib8189ftvwrapper.a( .text* .data* .data1* .rodata* .rodata1* .reginfo*) /*WIFI_LINK_TAG2*/
        ../../ThirdParty/rtkcmd/lib/librtkcmd.a( .text* .data* .data1* .rodata* .rodata1* .reginfo*) /*WIFI_LINK_TAG3*/
        /*WIFI_LINK_TAG4*/
        ../../ThirdParty/devs_eCos-spi-eth/lib/libethnt96650.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ThirdParty/devs_eCos-spi-eth/lib/libiospi.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ThirdParty/devs_eCos-spi-eth/lib/libspint96650.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DemoKit_Data/Release/ObjectCode/ecos_main.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DemoKit_Data/Release/ObjectCode/test_ecos.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ThirdParty/live555/lib/Live555.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /* ecos related  end */

        /*============================ after os start ===============================  */
        ../../ARC_eCos/Lib/Release/GxSystem.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DemoKit_Data/Release/ObjectCode/main.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DemoKit_Data/Release/ObjectCode/SysMain.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DemoKit_Data/Release/ObjectCode/SysMain_Flow_Init.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DemoKit_Data/Release/ObjectCode/SysMain_Exe.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DemoKit_Data/Release/ObjectCode/SysStrg_Exe.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DemoKit_Data/Release/ObjectCode/SysCmdCB.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)

        /*============================ boot flow ===============================  */
        ../../ARC_eCos/Lib/Release/FwSrv.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ARC_eCos/Lib/Release/PartLoad.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ARC_eCos/App/Release/DevMan.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ARC_eCos/App/Release/NvtSystem.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ARC_eCos/App/Release/NVTUser.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)

        /*============================ drv ext ===============================  */
        ../../ARC_eCos/Drv/Release/GSensor_DA380.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ARC_eCos/Drv/Release/Dx.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ../../ARC_eCos/Drv/Release/MODELEXT_WIFICARDV.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
    }

    /* device common */
    . = ALIGN(4);
    .SECTION_02 :
    {
        _section_01_size = ABSOLUTE(.) - _section_01_addr;
        _section_02_addr = ABSOLUTE(.);
    }

    /* device photo */
    . = ALIGN(4);
    .SECTION_03 :
    {
    	_section_02_size = ABSOLUTE(.) - _section_02_addr;
        _section_03_addr = ABSOLUTE(.);
    }

    /* device movie */
    . = ALIGN(4);
    .SECTION_04 :
    {
    	_section_03_size = ABSOLUTE(.) - _section_03_addr;
        _section_04_addr = ABSOLUTE(.);
    }

    /* device playback */
    . = ALIGN(4);
    .SECTION_05 :
    {
    	_section_04_size = ABSOLUTE(.) - _section_04_addr;
        _section_05_addr = ABSOLUTE(.);
    }

    /* application common */
    . = ALIGN(4);
    .SECTION_06 :
    {
    	_section_05_size = ABSOLUTE(.) - _section_05_addr;
        _section_06_addr = ABSOLUTE(.);
    }

    /* application photo */
    . = ALIGN(4);
    .SECTION_07 :
    {
    	_section_06_size = ABSOLUTE(.) - _section_06_addr;
        _section_07_addr = ABSOLUTE(.);
    }

    /* application movie */
    . = ALIGN(4);
    .SECTION_08 :
    {
    	_section_07_size = ABSOLUTE(.) - _section_07_addr;
        _section_08_addr = ABSOLUTE(.);
    }

    /* application playback */
    . = ALIGN(4);
    .SECTION_09 :
    {
    	_section_08_size = ABSOLUTE(.) - _section_08_addr;
        _section_09_addr = ABSOLUTE(.);
    }

    /* others */
    . = ALIGN(4);
    .SECTION_10 :
    {
    	_section_09_size = ABSOLUTE(.) - _section_09_addr;
        _section_10_addr = ABSOLUTE(.);
	KEEP (*(.nvtversioninfo.begin*))
	_section_verinfostart = _section_10_addr;
        KEEP(*(.version.info.*))
	_section_verinfoend = ABSOLUTE(.);
        KEEP (*(.nvtversioninfo.end*))
        *(.text*)
        *(.got*)
        *(.rodata*)
        *(.data*)
    }

    _section_10_size = ABSOLUTE(.) - _section_10_addr;

    . = ALIGN(4);
    .init ALIGN (0x4) : { . = .; KEEP (*(.init*)) }
    .text ALIGN (0x4) : { _stext = .; _ftext = . ; *(.text*) *(.text.*) *(.stub) *(.gnu.warning) *(.gnu.linkonce.t*) *(.mips16.fn.*) *(.mips16.call.*) } _etext = .; PROVIDE (etext = .);

    .fini ALIGN (0x4) : { . = .; KEEP (*(.fini*)) }
    .rodata ALIGN (0x8) : { . = .; *(.rodata*) *(.rodata.*) *(.gnu.linkonce.r*) }
    .rodata1 ALIGN (0x8) : { . = .; *(.rodata1*) *(.rodata1.*) }
    .data ALIGN (0x8) : { __ram_data_start = ABSOLUTE (.); _fdata = . ; *(.data*) *(.data.*) *(.gnu.linkonce.d*) *( .2ram.*) . = ALIGN (8); SORT(CONSTRUCTORS) } __rom_data_start = LOADADDR(.data);
    .data1 ALIGN (0x8) : { . = .; *(.data1*) *(.data1.*) }
    .eh_frame_hdr ALIGN (0x4) : { . = .; __EH_FRAME_BEGIN__ = .; KEEP(*(.eh_frame_hdr*)) __FRAME_END__ = .; . = . + 8; }
    .eh_frame ALIGN (0x4) : { . = .; __EH_FRAME_BEGIN__ = .; KEEP(*(.eh_frame*)) __FRAME_END__ = .; . = . + 8; }
    .gcc_except_table ALIGN (0x4) : { . = .; KEEP(*(.gcc_except_table*)) *(.gcc_except_table.*) }

    _gp = ALIGN(16) + 0x7ff0; .got ALIGN (0x4) : { . = .; *(.got.plt) *(.got*) }
    .dynamic ALIGN (0x4) : { . = .; *(.dynamic*) }
    .sdata ALIGN (0x4) : { . = .; *(.sdata*) *(.sdata.*) *(.gnu.linkonce.s*) }
    .lit8 ALIGN (0x8) : { . = .; *(.lit8*) }
    .lit4 : AT ((LOADADDR (.lit8) + SIZEOF (.lit8) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; *(.lit4) } __ram_data_end = .; _edata = . ; PROVIDE (edata = .);


    /* collect zero initialized global data (mark "B" in sym file) */
    /* collect static local data (mark "b" in sym file) */
    _section_zi_addr = ABSOLUTE(.);
    _image_general_zi_zi_base = ABSOLUTE(.);

    __bss_start = .; _fbss = .; .sbss ALIGN (0x8) : { . = .; *(.dynsbss*) *(.sbss) *(.sbss.*) *(.scommon*) *(.gnu.linkonce.sb.*) }
    .bss ALIGN (0x8) : { *(.dynbss*) *(.bss*) *(.bss.*) *(COMMON) *(.gnu.linkonce.b.*) } __bss_end = .;

    /* NOTE:
    if using -fcommon compiler option, un-initialized global data will be isolated to .common section.
    if using -fno-common compiler option, un-initialized global data will be merged to .bss section.
    memory between _image_general_zi_zi_base and _image_general_zi_zi_limit will be fill to zero before _main().
    */
    . = ALIGN(4);
    .common : {
       __common_base = ABSOLUTE(.);
       /* collect "small" un-initialized global data (mark "C" in sym file) */
       *(.scommon*)
       /* collect "large" un-initialized global data (mark "B" in sym file) */
       *(COMMON)
       __common_limit = ABSOLUTE(.);
    }
    /* _image_general_zi_zi_limit = ABSOLUTE(.); */


    /* heap area start for eCos */
    . = ALIGN(16);
    __heap1 = .;
    . = . + 0x00300000; /*reserved 3MB for ecos heap memory, 3MB is enough for EVB ecos network using ,
                          this value is project dependent should be adjusted by project*/
    . = ALIGN(0x100000); /* because _ecos_mem_sizembit must align to 1MB  */
    __heap1_end = .;

    _ecos_mem_end = .;
    _ecos_mem_size = . - _section_01_addr;
    _ecos_mem_sizembit = _ecos_mem_size / (1024*1024) * 8;

    /* end of code and eCos stuff */
    _end = .; PROVIDE (end = .);

    /* stack area start for RTOS tasks */
    _image_general_zi_zi_limit = ABSOLUTE(.);
    _stack_start = ALIGN(16);

}

EXTERN(cyg_sdio_devtab_entry sdio_initial rltk819x_wlan0_netdev0 rltk819x_wlan_init wlan0_main)

EXTERN(AppControl_LIBRARY_VERSION_INFO Capture_LIBRARY_VERSION_INFO CurlNvt_LIBRARY_VERSION_INFO DevMan_LIBRARY_VERSION_INFO LviewNvt_LIBRARY_VERSION_INFO MediaPlay_LIBRARY_VERSION_INFO MediaRec_LIBRARY_VERSION_INFO MovieInterface_LIBRARY_VERSION_INFO NVTUser_LIBRARY_VERSION_INFO NvtAec_LIBRARY_VERSION_INFO NvtSystem_LIBRARY_VERSION_INFO Photo_LIBRARY_VERSION_INFO Playback_LIBRARY_VERSION_INFO RtspNvt_LIBRARY_VERSION_INFO SMediaPlay_LIBRARY_VERSION_INFO SMediaRec_LIBRARY_VERSION_INFO UIControl_LIBRARY_VERSION_INFO VControl_LIBRARY_VERSION_INFO ExamSrv_LIBRARY_VERSION_INFO Kernel_LIBRARY_VERSION_INFO NvtPrjVer_LIBRARY_VERSION_INFO Nvtver_LIBRARY_VERSION_INFO OS_LIBRARY_VERSION_INFO Startup_LIBRARY_VERSION_INFO Utility_LIBRARY_VERSION_INFO Audio_Common_LIBRARY_VERSION_INFO Audio_ExtWM8978_LIBRARY_VERSION_INFO DbgUt_LIBRARY_VERSION_INFO Dispobj_LIBRARY_VERSION_INFO Disprot_LIBRARY_VERSION_INFO Driver_Codec_LIBRARY_VERSION_INFO Driver_IPP_LIBRARY_VERSION_INFO Driver_Sys_LIBRARY_VERSION_INFO Dx_LIBRARY_VERSION_INFO DxSys_LIBRARY_VERSION_INFO hotShoeIO_LIBRARY_VERSION_INFO lensIO_LIBRARY_VERSION_INFO Ramdisk_LIBRARY_VERSION_INFO Sensor_LIBRARY_VERSION_INFO SpiFlash_LIBRARY_VERSION_INFO AAC_LIBRARY_VERSION_INFO ADPCM_LIBRARY_VERSION_INFO AEAlg_LIBRARY_VERSION_INFO AEC_LIBRARY_VERSION_INFO AETask_LIBRARY_VERSION_INFO AVFile_MakerAvi_LIBRARY_VERSION_INFO AVFile_MakerMov_LIBRARY_VERSION_INFO AVFile_ParserAvi_LIBRARY_VERSION_INFO AVFile_ParserMov_LIBRARY_VERSION_INFO AWBAlg_LIBRARY_VERSION_INFO AWBTask_LIBRARY_VERSION_INFO AudFilt_LIBRARY_VERSION_INFO AudNR_LIBRARY_VERSION_INFO AudProc_LIBRARY_VERSION_INFO AudioCodec_AAC_LIBRARY_VERSION_INFO AudioCodec_PCM_LIBRARY_VERSION_INFO AudioCodec_PPCM_LIBRARY_VERSION_INFO AudioCodec_Zero_LIBRARY_VERSION_INFO CEC_LIBRARY_VERSION_INFO CompareView_LIBRARY_VERSION_INFO CoordTs_LIBRARY_VERSION_INFO DCF_LIBRARY_VERSION_INFO DIS_LIBRARY_VERSION_INFO DIS_FW_LIBRARY_VERSION_INFO DIS_HW_LIBRARY_VERSION_INFO DPOF_LIBRARY_VERSION_INFO DataStamp_LIBRARY_VERSION_INFO DhcpNvt_LIBRARY_VERSION_INFO DispSrv_LIBRARY_VERSION_INFO EXIF_LIBRARY_VERSION_INFO FD_LIBRARY_VERSION_INFO FileAccess_LIBRARY_VERSION_INFO FileDB_LIBRARY_VERSION_INFO FlashLightAlg_LIBRARY_VERSION_INFO FontToYuv_LIBRARY_VERSION_INFO FwSrv_LIBRARY_VERSION_INFO GxDisplay_LIBRARY_VERSION_INFO GxFlash_LIBRARY_VERSION_INFO GxGfx_LIBRARY_VERSION_INFO GxImage_LIBRARY_VERSION_INFO GxImageFile_LIBRARY_VERSION_INFO GxInput_LIBRARY_VERSION_INFO GxOutput_LIBRARY_VERSION_INFO GxPower_LIBRARY_VERSION_INFO GxSound_LIBRARY_VERSION_INFO GxStrg_LIBRARY_VERSION_INFO GxSystem_LIBRARY_VERSION_INFO GxTimer_LIBRARY_VERSION_INFO GxUSB_LIBRARY_VERSION_INFO GxVideo_LIBRARY_VERSION_INFO GxVideoFile_LIBRARY_VERSION_INFO H264_LIBRARY_VERSION_INFO HWAcc_LIBRARY_VERSION_INFO ImageEffect_LIBRARY_VERSION_INFO ImgStatsLib_LIBRARY_VERSION_INFO JSONparser_LIBRARY_VERSION_INFO JpegBRC_Default_LIBRARY_VERSION_INFO LDWS_ALG_LIBRARY_VERSION_INFO LogFile_LIBRARY_VERSION_INFO MsdcNvt_LIBRARY_VERSION_INFO MsdcNvtCb_Disp_LIBRARY_VERSION_INFO MsdcNvtCb_File_LIBRARY_VERSION_INFO NVTFD_LIBRARY_VERSION_INFO NVTFT_LIBRARY_VERSION_INFO NameRule_Custom_LIBRARY_VERSION_INFO NameRule_DCFFull_LIBRARY_VERSION_INFO NameRule_Empty_LIBRARY_VERSION_INFO NameRule_FileDB_LIBRARY_VERSION_INFO NvtHDR_LIBRARY_VERSION_INFO NvtNS_LIBRARY_VERSION_INFO ObjTrackingAlg_LIBRARY_VERSION_INFO PBXEdit_LIBRARY_VERSION_INFO PBXFile_LIBRARY_VERSION_INFO PBXFileList_DCF_LIBRARY_VERSION_INFO PBXFileList_FileDB_LIBRARY_VERSION_INFO PBXRedEye_LIBRARY_VERSION_INFO PHOTOFRAME_LIBRARY_VERSION_INFO PStore_LIBRARY_VERSION_INFO PartLoad_LIBRARY_VERSION_INFO RSC_LIBRARY_VERSION_INFO RectZoom_LIBRARY_VERSION_INFO SD_LIBRARY_VERSION_INFO SceneRender_LIBRARY_VERSION_INFO SizeConvert_LIBRARY_VERSION_INFO UCDC_LIBRARY_VERSION_INFO UMSD_LIBRARY_VERSION_INFO UPCC_LIBRARY_VERSION_INFO USIDC_LIBRARY_VERSION_INFO UVAC_LIBRARY_VERSION_INFO UserJPEG_LIBRARY_VERSION_INFO VideoCodec_BStr_LIBRARY_VERSION_INFO VideoCodec_H264_LIBRARY_VERSION_INFO VideoCodec_MJPG_LIBRARY_VERSION_INFO Wav_LIBRARY_VERSION_INFO WavStudio_LIBRARY_VERSION_INFO WifiCmdXML_LIBRARY_VERSION_INFO ipl_calibration_LIBRARY_VERSION_INFO ipl_common_LIBRARY_VERSION_INFO SlideEffect_ClientLeft2Right_LIBRARY_VERSION_INFO SlideEffect_ClientRight2Left_LIBRARY_VERSION_INFO SlideEffect_Fade_LIBRARY_VERSION_INFO SlideEffect_PushFadeRight2Left_LIBRARY_VERSION_INFO SlideEffect_R2VIn2Out_LIBRARY_VERSION_INFO SlideEffect_R5HDrapeUp2Down_LIBRARY_VERSION_INFO SlideEffect_SlideinRight2Left_LIBRARY_VERSION_INFO SlideEffect_Default_LIBRARY_VERSION_INFO AFALG_LIBRARY_VERSION_INFO AFTASK_LIBRARY_VERSION_INFO IPL_DISPLAY_LIBRARY_VERSION_INFO IPL_DISPLAY2_LIBRARY_VERSION_INFO IPL_CAL_RW_LIBRARY_VERSION_INFO MDAlg_LIBRARY_VERSION_INFO ObjTrackingAlg_LIBRARY_VERSION_INFO Iperf_LIBRARY_VERSION_INFO curl_LIBRARY_VERSION_INFO devssdio_LIBRARY_VERSION_INFO devsspieth_LIBRARY_VERSION_INFO libaudclass_LIBRARY_VERSION_INFO libextras_LIBRARY_VERSION_INFO libtarget_LIBRARY_VERSION_INFO Dhcpelios_LIBRARY_VERSION_INFO hfs_LIBRARY_VERSION_INFO Lviewd_LIBRARY_VERSION_INFO libProfile_LIBRARY_VERSION_INFO sntp_LIBRARY_VERSION_INFO sntp_LIBRARY_VERSION_INFO usocket_LIBRARY_VERSION_INFO usocketcli_LIBRARY_VERSION_INFO FileSysECOS_LIBRARY_VERSION_INFO PStoreECOS_LIBRARY_VERSION_INFO Live555_LIBRARY_VERSION_INFO libFtpd_LIBRARY_VERSION_INFO UShare_LIBRARY_VERSION_INFO nvtwifi_LIBRARY_VERSION_INFO devsrtl8189ftv_LIBRARY_VERSION_INFO nvtwifi_LIBRARY_VERSION_INFO nvtrtk_LIBRARY_VERSION_INFO rtkcmd_LIBRARY_VERSION_INFO rtkwps_LIBRARY_VERSION_INFO)
