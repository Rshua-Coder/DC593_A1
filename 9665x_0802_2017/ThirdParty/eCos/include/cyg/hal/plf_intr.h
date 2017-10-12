#ifndef CYGONCE_HAL_PLF_INTR_H
#define CYGONCE_HAL_PLF_INTR_H

//==========================================================================
//
//      plf_intr.h
//
//      HAL interrupt and clock support
//
//==========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
// Copyright (C) 2006 eCosCentric Limited
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    nickg
// Contributors: nickg, jskov,
//               gthomas, jlarmour, dmoseley
// Date:         2001-03-20
// Purpose:      Define Interrupt support
// Description:  The macros defined here provide the HAL APIs for handling
//               interrupts and the real-time clock.
//
// Usage:
//              #include <cyg/hal/plf_intr.h>
//              ...
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>

// First an assembly safe part
#ifndef __ASSEMBLER__

//--------------------------------------------------------------------------
// Interrupt vectors.

#ifndef CYGHWR_HAL_INTERRUPT_VECTORS_DEFINED

// These are decoded via the interrupt controller ID
// register when an external interrupt is delivered.

#define CYGNUM_HAL_INTR_NULL                0
#define CYGNUM_HAL_INTR_TIMER               1
#define CYGNUM_HAL_INTR_SIE12               2
#define CYGNUM_HAL_INTR_PRE                 3
#define CYGNUM_HAL_INTR_IPE                 4
#define CYGNUM_HAL_INTR_IME                 5
#define CYGNUM_HAL_INTR_DCE_IFE12           6
#define CYGNUM_HAL_INTR_DIS_FDE_RDE         7
#define CYGNUM_HAL_INTR_DAI                 8
#define CYGNUM_HAL_INTR_H264                9
#define CYGNUM_HAL_INTR_JPEG                10
#define CYGNUM_HAL_INTR_GRAPH12_AFFINE      11
#define CYGNUM_HAL_INTR_ISE                 12
#define CYGNUM_HAL_INTR_GPIO_REMOTE         13
#define CYGNUM_HAL_INTR_PWM                 14
#define CYGNUM_HAL_INTR_USB                 15
#define CYGNUM_HAL_INTR_NAND                16
#define CYGNUM_HAL_INTR_SDIO                17
#define CYGNUM_HAL_INTR_SDIO2               18
#define CYGNUM_HAL_INTR_DRAM                19
#define CYGNUM_HAL_INTR_SPI                 20
#define CYGNUM_HAL_INTR_SPI23               21
#define CYGNUM_HAL_INTR_SIF                 22
#define CYGNUM_HAL_INTR_I2C12               23
#define CYGNUM_HAL_INTR_UART                24
#define CYGNUM_HAL_INTR_UART12              24
#define CYGNUM_HAL_INTR_ADC                 25
#define CYGNUM_HAL_INTR_IDE12               26
#define CYGNUM_HAL_INTR_MIPIDSI_MI          27
#define CYGNUM_HAL_INTR_HDMI                28
#define CYGNUM_HAL_INTR_LVDS_HISPI_MIPICSI12    29
#define CYGNUM_HAL_INTR_RTC                 30
#define CYGNUM_HAL_INTR_WDT                 31
#define CYGNUM_HAL_INTR_CG                  32

#define CYGNUM_HAL_INTR_SW0         33  //translate and append SW0 intr to end of HW0
#define CYGNUM_HAL_INTR_SW1         34  //translate and append SW1 intr to end of HW0
#define CYGNUM_HAL_INTR_HW1         35  //translate and append HW1 intr to end of HW0
#define CYGNUM_HAL_INTR_HW2         36  //translate and append HW2 intr to end of HW0
#define CYGNUM_HAL_INTR_HW3         37  //translate and append HW3 intr to end of HW0
#define CYGNUM_HAL_INTR_HW4         38  //translate and append HW4 intr to end of HW0
#define CYGNUM_HAL_INTR_HW5         39  //translate and append HW5 intr to end of HW0
#define CYGNUM_HAL_INTR_CPUCNT          CYGNUM_HAL_INTR_HW5


// Min/Max ISR numbers (Interrupt Controller)
#define HAL_INTR_MIN                    1
#define HAL_INTR_MAX                    32

// Virtual table
#define CYGNUM_HAL_ISR_MIN              0
#define CYGNUM_HAL_ISR_MAX              39  //32
#define CYGNUM_HAL_ISR_COUNT            (CYGNUM_HAL_ISR_MAX - CYGNUM_HAL_ISR_MIN + 1)

#ifndef CYGNUM_HAL_INTERRUPT_RTC
#define CYGNUM_HAL_INTERRUPT_RTC        CYGNUM_HAL_INTR_CPUCNT
#endif

//#define CYGNUM_HAL_CPU2_INTERRUPT_TIMER     64

#define CYGHWR_HAL_INTERRUPT_VECTORS_DEFINED

#endif /* CYGHWR_HAL_INTERRUPT_VECTORS_DEFINED */

#include <cyg/infra/cyg_type.h>
#include <cyg/hal/plf_io.h>

//--------------------------------------------------------------------------
// Interrupt controller access.

#ifndef CYGHWR_HAL_INTERRUPT_CONTROLLER_ACCESS_DEFINED

// Array which stores the configured priority levels for the configured
// interrupts.
externC volatile CYG_BYTE hal_interrupt_level[CYGNUM_HAL_ISR_COUNT];

externC void hal_plf_interrupt_mask( cyg_uint32 vector );
#define HAL_INTERRUPT_MASK( _vector_ ) hal_plf_interrupt_mask( _vector_ )

externC void hal_plf_interrupt_unmask( cyg_uint32 vector );
#define HAL_INTERRUPT_UNMASK( _vector_ ) hal_plf_interrupt_unmask( _vector_ )

externC void hal_plf_interrupt_set_level( cyg_uint32 vector, cyg_uint32 prilevel );
#define HAL_INTERRUPT_SET_LEVEL( _vector_, _level_ ) hal_plf_interrupt_set_level( _vector_, _level_ )

externC void hal_plf_interrupt_acknowledge( cyg_uint32 vector );
#define HAL_INTERRUPT_ACKNOWLEDGE( _vector_ ) hal_plf_interrupt_acknowledge( _vector_ )

externC void hal_plf_interrupt_configure( cyg_uint32 vector, cyg_uint32 leveltriggered, cyg_uint32 up );
#define HAL_INTERRUPT_CONFIGURE( _vector_, _level_, _up_ ) hal_plf_interrupt_configure( _vector_, _level_, _up_ )

#define HAL_INTERRUPT_TRIGGER

#define CYGHWR_HAL_INTERRUPT_CONTROLLER_ACCESS_DEFINED

#endif /* CYGHWR_HAL_INTERRUPT_CONTROLLER_ACCESS_DEFINED */

//--------------------------------------------------------------------------
// Control-C support.

#if defined(CYGDBG_HAL_MIPS_DEBUG_GDB_CTRLC_SUPPORT)

# define CYGHWR_HAL_GDB_PORT_VECTOR CYGNUM_HAL_INTERRUPT_SER

externC cyg_uint32 hal_ctrlc_isr(CYG_ADDRWORD vector, CYG_ADDRWORD data);

# define HAL_CTRLC_ISR hal_ctrlc_isr

#endif

//--------------------------------------------------------------------------
// Reset.
#ifndef CYGHWR_HAL_RESET_DEFINED
externC void hal_plf_reset( void );
#define CYGHWR_HAL_RESET_DEFINED
#define HAL_PLATFORM_RESET()             hal_plf_reset()

//#define HAL_PLATFORM_RESET_ENTRY 0xbfc00000
#define HAL_PLATFORM_RESET_ENTRY 0x80000400 //???

#endif // CYGHWR_HAL_RESET_DEFINED

#endif // __ASSEMBLER__

//--------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_PLF_INTR_H
// End of plf_intr.h
