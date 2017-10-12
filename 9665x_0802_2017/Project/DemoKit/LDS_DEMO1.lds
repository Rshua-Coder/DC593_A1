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
ENTRY(__start)
SEARCH_DIR(.)

SECTIONS
{
    .SPECIAL_RO 0x80000000:
    {
    	_section_01_addr = ABSOLUTE(.);
        KEEP(BinInfo.o(.data*));
    }
    .EXCEPTION_TABLE 0x80000180:
    {
        KEEP(Startup/exception_MIPS.o(.text*));
    }
    .VECTOR_TABLE 0x80000200:
    {
        __start_vector_table = ABSOLUTE(.);
        KEEP(Startup/isr_MIPS.o(.text*));
        __end_vector_table = ABSOLUTE(.);
    }
    .CODE_INFO 0x800002F0:
    {
        KEEP(CodeInfo.o(.data*));
    }
    .START_UP 0x80000400:
    {
        _load_entry_start_base = ABSOLUTE(.);
        Startup/Loader_MIPS.o(.text*)
        Startup/Startup_MIPS.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        _load_entry_end_base = ABSOLUTE(.);
    }

    /* kernel common */
    . = ALIGN(4);
    .SECTION_01 :
    {
        /*============================ before os start ===============================  */
        *(.kercfg_data*) /*collect install ID process of all modules*/
        *(.kercfg_text*) /*collect install ID process of all modules*/
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
        LogFile.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ after os start ===============================  */
        GxSystem.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        main.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysMain.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysMain_Flow_Init.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysMain_Exe.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysStrg_Exe.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysCmdCB.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        PipView.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ boot flow ===============================  */
        FwSrv.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        PartLoad.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DevMan.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        NvtSystem.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        NVTUser.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ drv ext ===============================  */
        $GSENSOREXT.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Dx.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SpiFlash.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Ramdisk.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        $MODELEXT.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
    }

    /* device common */
    . = ALIGN(4);
    .SECTION_02 :
    {
    	_section_01_size = ABSOLUTE(.) - _section_01_addr;
    	KEEP (*(.nvtversioninfo.begin*))
        _section_02_addr = ABSOLUTE(.);
	_section_verinfostart = _section_02_addr;
        KEEP(*(.version.info.*))
	_section_verinfoend = ABSOLUTE(.);
        KEEP (*(.nvtversioninfo.end*))
        *(.text*)
        *(.got*)
        *(.rodata*)
        *(.data*)
    }
    _section_02_size = ABSOLUTE(.) - _section_02_addr;

    /* NOTE:
    if using -fcommon compiler option, un-initialized global data will be isolated to .common section.
    if using -fno-common compiler option, un-initialized global data will be merged to .bss section.
    memory between _image_general_zi_zi_base and _image_general_zi_zi_limit will be fill to zero before _main().
    */
    . = ALIGN(4);
    .bss :
    {
        _image_general_zi_zi_base = ABSOLUTE(.);
        /* collect zero initialized global data (mark "B" in sym file) */
        /* collect static local data (mark "b" in sym file) */
        *(.bss*)
    }
    . = ALIGN(4);
    .common :
    {
        __common_base = ABSOLUTE(.);
        /* collect "small" un-initialized global data (mark "C" in sym file) */
        *(.scommon*)
        /* collect "large" un-initialized global data (mark "B" in sym file) */
        *(COMMON)
        __common_limit = ABSOLUTE(.);
    }

    . = ALIGN(4);
        _image_general_zi_zi_limit = ABSOLUTE(.);
    _stack_start = .;

}
