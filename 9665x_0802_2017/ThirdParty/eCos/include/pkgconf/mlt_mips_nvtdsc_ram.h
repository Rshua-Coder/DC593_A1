// eCos memory layout - Mon Apr 09 14:17:14 2001

// This is a generated file - do not edit

#ifndef __ASSEMBLER__
#include <cyg/infra/cyg_type.h>
#include <pkgconf/hal_mips_nvtdsc.h>
#include <stddef.h>

#endif
#ifndef __ASSEMBLER__
extern char CYG_LABEL_NAME (__ebase);
#endif

static inline unsigned int CYGMEM_REGION_get_ram_SIZE(void)
{
	extern unsigned int cyg_mem_size;	// unit Mbit

	return (cyg_mem_size / 8);
}

#define CYGMEM_REGION_ram (0x80000000)
#define CYGMEM_REGION_ram_SIZE (CYGMEM_REGION_get_ram_SIZE() * 1024 * 1024)
#define CYGMEM_REGION_ram_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)
#ifndef __ASSEMBLER__
extern char CYG_LABEL_NAME (__heap1) [];
#endif
#define CYGMEM_SECTION_heap1 (CYG_LABEL_NAME (__heap1))
#define CYGMEM_SECTION_heap1_SIZE ((CYGMEM_REGION_ram+CYGMEM_REGION_ram_SIZE) - (size_t) CYG_LABEL_NAME (__heap1))
