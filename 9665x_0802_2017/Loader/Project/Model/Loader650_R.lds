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
    .BOOT_EXEC 0x80500000: /*BOOT_EXEC_TAG*/
    {
        _load_BOOT_EXEC_start_base = .;
    	KEEP(Loader650_Data/Release/ObjectCode/reset.o(.data));
    	Loader650_Data/Release/ObjectCode/reset.o(.text);    	    	
    	_load_BOOT_EXEC_end_base = .;    	
    }    
    .LOADER_CONFIGRAM 0x80500000 + 0x30: /*LOADER_CONFIGRAM_TAG*/
    {             
        _load_LOADER_CONFIGRAM_start_base = .;
        KEEP(Loader650_Data/Release/ObjectCode/configramNM.o(.data));
        _load_LOADER_CONFIGRAM_end_base = .;
    }
    
    .LOADER_REMAP 0x80500000 + 0x200: /*LOADER_REMAP_TAG*/
    {   
        _load_vector_int_table_start_base = .;        
        KEEP(Loader650_Data/Release/ObjectCode/isr.o(.text));
        _load_vector_int_table_end_base = .;  
        Loader650_Data/Release/ObjectCode/doremapNM.o(.text);
        _load_LOADER_REMAP_end_base = .;
        _load_vector_exp_table_start_base = .;
        KEEP(Loader650_Data/Release/ObjectCode/exp.o(.text));
        _load_vector_exp_table_end_base = .;
    }    

	. = ALIGN(4);
	.text : 
	{ 
		*(.text) 
	}
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
