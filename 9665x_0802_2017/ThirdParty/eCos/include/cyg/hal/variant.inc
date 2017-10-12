#ifndef CYGONCE_HAL_VARIANT_INC
#define CYGONCE_HAL_VARIANT_INC
##=============================================================================
##
##	variant.inc
##
##	MIPS 32/64 family assembler header file
##
##=============================================================================
#####ECOSGPLCOPYRIGHTBEGIN####
## -------------------------------------------
## This file is part of eCos, the Embedded Configurable Operating System.
## Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
## Copyright (C) 2005 eCosCentric Limited
##
## eCos is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation; either version 2 or (at your option) any later version.
##
## eCos is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
## for more details.
##
## You should have received a copy of the GNU General Public License along
## with eCos; if not, write to the Free Software Foundation, Inc.,
## 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
##
## As a special exception, if other files instantiate templates or use macros
## or inline functions from this file, or you compile this file and link it
## with other works to produce a work based on this file, this file does not
## by itself cause the resulting work to be covered by the GNU General Public
## License. However the source code for this file must still be made available
## in accordance with section (3) of the GNU General Public License.
##
## This exception does not invalidate any other reasons why a work based on
## this file might be covered by the GNU General Public License.
##
## Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
## at http://sources.redhat.com/ecos/ecos-license/
## -------------------------------------------
#####ECOSGPLCOPYRIGHTEND####
##=============================================================================
#######DESCRIPTIONBEGIN####
##
## Author(s): 	dmoseley
## Contributors:	dmoseley
## Date:	2000-06-07
## Purpose:	MIPS32 family definitions.
## Description:	This file contains various definitions and macros that are
##              useful for writing assembly code for the MIPS32 CPU family.
## Usage:
##		#include <cyg/hal/variant.inc>
##		...
##		
##
######DESCRIPTIONEND####
##
##=============================================================================

#include <pkgconf/hal.h>
	
#include <cyg/hal/mips.inc>

#include <cyg/hal/platform.inc>

#define CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#include <cyg/hal/mips-regs.h>
#include <cyg/hal/hal_arch.h>

##-----------------------------------------------------------------------------
## Define CPU variant for architecture HAL.

#define	CYG_HAL_MIPS_NT96650

# -----------------------------------------------------------------------------
# Cache macros.
# -----------------------------------------------------------------------------
#ifndef CYGPKG_HAL_MIPS_CACHE_DEFINED

    .macro  hal_cache_init

/*
 ************************************************************************
 *               C O N F I G   R E G I S T E R   ( 1 6 )                *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |M|                             |B| A |  A  |  M  |RSVD |V|  K  | Config
 * | | Reserved for Implementations|E| T |  R  |  T  |     |I|  0  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |K23|                 |M|   |B|
 *   |   |                 |M|   |M|
 *   +---+                 +-+   +-+
 */
    # Set KSeg0 to cacheable, write-back
    # Config.K0 set K0 = 0 programing guide p.28
    mfc0    $10, $16        # Read Config Register
    li      $11, 0x7        # $11 = 0x00000007
    not     $11             # $11 = 0xFFFFFFF8
    and     $10, $11        # Clear Config: KO[2:0]
    or      $10, 0x3        # K0[2:0]=3 => cache, 3 => Cacheable, write-back
    mtc0    $10, $16        # Write back to Config Register

    # Issue sync command
    mfc0      $12, $16, 7   # read config7 register into $12
    or        $12, 0x100    # set bit8 = 1 (ES)
    mtc0      $12, $16, 7   # write $12 back to config7 => issue sync command


    .endm

#define CYGPKG_HAL_MIPS_CACHE_DEFINED

#endif  

#------------------------------------------------------------------------------
# Monitor initialization.
	
#ifndef CYGPKG_HAL_MIPS_MON_DEFINED

	.macro	hal_mon_init
	.endm

#define CYGPKG_HAL_MIPS_MON_DEFINED

#endif	

#------------------------------------------------------------------------------
# Decide whether the VSR table is defined externally, or is to be defined
# here.

#if defined(CYGPKG_HAL_MIPS_SIM) ||        \
    ( defined(CYGPKG_HAL_MIPS_ATLAS) &&    \
      defined(CYG_HAL_STARTUP_RAM) &&      \
      !defined(CYGSEM_HAL_USE_ROM_MONITOR) \
    )

## VSR table defined in linker script

#else

//#define CYG_HAL_MIPS_VSR_TABLE_DEFINED

#endif

#------------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_VARIANT_INC
# end of variant.inc
