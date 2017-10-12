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
OUTPUT_ARCH(mips)
ENTRY(__start)
SEARCH_DIR(.)

SECTIONS
{
    .BOOT_EXEC 0xFF00000|0xA0000000: /*BOOT_EXEC_TAG*/
    AT (0)
    {
    	_load_BOOT_EXEC_start_base = .;
    	KEEP(Loader650_Data/Release/ObjectCode/reset.o(.data));
    	Loader650_Data/Release/ObjectCode/reset.o(.text);
    	_load_BOOT_EXEC_end_base = .;
    }
    .LOADER_CONFIGRAM (0xFF00000|0xA0000000) + 0x30: /*LOADER_CONFIGRAM_TAG*/
    AT (0x30)
    {
        _load_LOADER_CONFIGRAM_start_base = .;
        KEEP(Loader650_Data/Release/ObjectCode/configramLZ.o(.data));
        _load_LOADER_CONFIGRAM_end_base = .;
        _load_LOADER_CONFIGRAM_FREQ_PARAM_start_base = .;
        KEEP(Loader650_Data/Release/ObjectCode/configramFreqParam.o(.data));
        _load_LOADER_CONFIGRAM_FREQ_PARAM_end_base = .;
    }
    .internal_stg_type (0xFF00000|0xA0000000) + 0x220: { LONG (0x444E414E) } /*INTERNAL_STORAGE_TYPE*/
    .internal_stg_version :
	AT (LOADADDR(.internal_stg_type) + SIZEOF(.internal_stg_type))
    {
		_load_nand_table_start_base = .;
        KEEP( Loader650_Data/Release/ObjectCode/nand_tbl_version.o(.data*)) /*INTERNAL_STORAGE_VERSION*/
		_load_nand_table_end_base = .;
    }
    .LOADER_REMAP (0xFF00000|0xA0000000) + 0x230: /*LOADER_REMAP_TAG*/
    AT (0x230)
    {
        _load_vector_int_table_start_base = .;
        KEEP(Loader650_Data/Release/ObjectCode/isr.o(.text));
        _load_vector_int_table_end_base = .;
        Loader650_Data/Release/ObjectCode/doremapLZ_A.o(.text);
        _load_LOADER_REMAP_end_base = .;
        _load_vector_exp_table_start_base = .;
        KEEP(Loader650_Data/Release/ObjectCode/exp.o(.text));
        _load_vector_exp_table_end_base = .;
    }
        
    .LOADER_EXEC (0xFF00000|0x80000000) + 0x4000:
    AT (LOADADDR(.LOADER_REMAP) + SIZEOF(.LOADER_REMAP))
    {
        _loader_exec_start_base = .;
        * (.part1)
        _loader_exec_end_base = .;
    }
    _loader_exec_cpu_addr = LOADADDR(.LOADER_EXEC) + ADDR(.BOOT_EXEC);
    _loader_exec_size = SIZEOF(.LOADER_EXEC);

	. = ALIGN(4);
    .text (0xFF00000|0x80000000) + 0x5000: /*COMPRESS_PART_TAG*/
    AT (LOADADDR(.LOADER_EXEC) + SIZEOF(.LOADER_EXEC))
    {
        _loader_exec_compres_start = .;
        KEEP( Loader650_Data/Release/ObjectCode/nand_tbl_samsung_128M_2K.o(.data*)) /*LOADER_SUPPORT_NAND_1*/
        KEEP( Loader650_Data/Release/ObjectCode/nand_tbl_ato_128M_2K.o(.data*)) /*LOADER_SUPPORT_NAND_2*/
        KEEP( Loader650_Data/Release/ObjectCode/nand_tbl_toshiba_128M_2K.o(.data*)) /*LOADER_SUPPORT_NAND_3*/
        *(.text)
    }
    _loader_exec_compress_load_base = LOADADDR(.text);
    _loader_exec_compress_load_cpu_addr = LOADADDR(.text) + ADDR(.BOOT_EXEC);
    _load_general_var_base = .;
    _image_general_var_base = .;
    .data : { *(.data) }
    _image_general_var_limit = .;
    . = ALIGN(4);
    .got : { *(.got) }
    . = ALIGN(4);
    .rodata : { *(.rodata) }
    . = ALIGN(4);
    .bss : {
        _image_general_zi_zi_base = .;
        *(.bss)
        _image_general_zi_zi_limit = .;
    	_load_load_reset_base = .;
    	_image_load_reset_base = .;
    	_image_load_reset_length = .;
    	_image_load_reset_zi_base = .;
    	_image_load_reset_zi_limit = .;
    }
    _stack_start = .;

}
