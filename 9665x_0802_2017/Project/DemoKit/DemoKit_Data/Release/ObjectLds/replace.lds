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
 * 2. to use .o in library layer have to add prefix module name. (DscSystem/SysTask.o)
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
        KEEP(BinInfo.o(.data*));
    }
    .EXCEPTION_TABLE 0x80000180:
    {
        /* KEEP(OS.a(.text.exp)); */
        /* KEEP(Startup.a(.other_vector)); */
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.other_vector));
    }
    .VECTOR_TABLE 0x80000200:
    {
        /* hal_vsr_table = ABSOLUTE(.);*/	/* exception code vector table */
        /* hal_virtual_vector_table = ABSOLUTE(.); */	/* ??? TODO ??? */
        __ebase = ABSOLUTE(.);	/* ??? TODO ??? */
        __start_vector_table = ABSOLUTE(.);
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.interrupt_vector*));
        /*KEEP(Startup.a(.text.isr));*/
        __end_vector_table = ABSOLUTE(.);
    }
    .CODE_INFO 0x800002F0:
    {
        KEEP(CodeInfo.o(.data*));
    }

    /* _stext = .; */	/* ??? TODO ??? */

    .START_UP 0x80000400:
    {
        _load_entry_start_base = ABSOLUTE(.);
        /* KEEP(Startup.a(.text.start)); */
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
        _main.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysCfg.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Kernel.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        OS.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DxSys.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DSPCFG.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Driver_Sys.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Driver_IPP/grph.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Driver_Codec/jpeg.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Driver_Codec/jpeg_int.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Debug.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        LibC.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        libgcc.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Utility.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        GxGfx/DC.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DbgUt.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        GxStrg/GxStrg_p1.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)

        /* below is ecos related  begin */
        libstdc++.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        libtarget.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        libextras.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        extras.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        vectors.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        libsdiont96650.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*NVTWIFI_TAG*/
        /*WIFI_ADAPTOR_TAG*/
        /*WIFI_LINK_TAG1*/
        /*WIFI_LINK_TAG2*/
        /*WIFI_LINK_TAG3*/
        /*WIFI_LINK_TAG4*/
        libethnt96650.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        libiospi.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        libspint96650.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ecos_main.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        test_ecos.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Live555.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /* ecos related  end */

        /*============================ after os start ===============================  */
        GxSystem.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        main.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysMain.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysMain_Flow_Init.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysMain_Exe.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysStrg_Exe.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysCmdCB.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)

        /*============================ boot flow ===============================  */
        FwSrv.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        PartLoad.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DevMan.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        NvtSystem.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        NVTUser.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)

        /*============================ drv ext ===============================  */
        GSensor_DA380.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Dx.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        MODELEXT_WIFICARDV.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
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

