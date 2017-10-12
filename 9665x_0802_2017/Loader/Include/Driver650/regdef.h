/*
 * MIPS register definitions, originally from:
 *
 * include/asm-mips/regdefs.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License, Version 2.  See the file "COPYING" in the main directory of this
 * archive for more details.
 *
 * Copyright (C) 1994, 1995 by Ralf Baechle
 */

/*
 * Symbolic register names for 32 bit ABI
 */
#define zero    $0      /* wired zero */
#define AT      $1      /* assembler temp  - uppercase because of ".set at" */
#define v0      $2      /* return value */
#define v1      $3
#define a0      $4      /* argument registers */
#define a1      $5
#define a2      $6
#define a3      $7
#define t0      $8      /* caller saved */
#define t1      $9
#define t2      $10
#define t3      $11
#define t4      $12
#define t5      $13
#define t6      $14
#define t7      $15
#define s0      $16     /* callee saved */
#define s1      $17
#define s2      $18
#define s3      $19
#define s4      $20
#define s5      $21
#define s6      $22
#define s7      $23
#define t8      $24     /* caller saved */
#define t9      $25
#define jp      $25     /* PIC jump register */
#define k0      $26     /* kernel scratch */
#define k1      $27
#define gp      $28     /* global pointer */
#define sp      $29     /* stack pointer */
#define fp      $30     /* frame pointer */
#define s8      $30     /* same like fp! */
#define ra      $31     /* return address */

#define C0_Count        $9
#define C0_Compare      $11
#define C0_Status       $12
#define C0_Cause        $13
#define C0_EPC          $14
#define C0_Config       $16
#define C0_WatchLo      $18
#define C0_WatchHi      $19

#define S_ConfigK0    0                       /* Kseg0 coherency algorithm (R/W) */
#define M_ConfigK0    (0x7 << S_ConfigK0)

#define S_StatusERL   2                       /* Denotes error level (R/W) */
#define M_StatusERL   (0x1 << S_StatusERL)


#define S_StatusKSU   3                       /* Two-bit current mode (R/W) */
#define M_StatusKSU   (0x3 << S_StatusKSU)

#define S_StatusIE    0                       /* Enables interrupts (R/W) */
#define M_StatusIE    (0x1 << S_StatusIE)

#define S_StatusBEV   22                      /* Enable Boot Exception Vectors (R/W) */
#define M_StatusBEV   (0x1 << S_StatusBEV)


/*
 * Cache attribute values in the C field of EntryLo and the
 * K0 field of Config
 */
#define K_CacheAttrCWTnWA       0                       /* Cacheable, write-thru, no write allocate */
#define K_CacheAttrCWTWA        1                       /* Cacheable, write-thru, write allocate */
#define K_CacheAttrU            2                       /* Uncached */
#define K_CacheAttrC            3                       /* Cacheable */
#define K_CacheAttrCN           3                       /* Cacheable, non-coherent */
#define K_CacheAttrCCE          4                       /* Cacheable, coherent, exclusive */
#define K_CacheAttrCCS          5                       /* Cacheable, coherent, shared */
#define K_CacheAttrCCU          6                       /* Cacheable, coherent, update */
#define K_CacheAttrUA           7                       /* Uncached accelerated */

#define S_EntryLo0PFN           S_EntryLoPFN
#define M_EntryLo0PFN           M_EntryLoPFN


#define R_C0_SRSCtl             12
#define R_C0_SelSRSCtl          2

#define S_SRSCtlHSS             26                      /* Highest shadow set (R) */
#define W_SRSCtlHSS             4
#define M_SRSCtlHSS             (0xf << S_SRSCtlHSS)
#define S_SRSCtlEICSS           18                      /* Exception shadow set (R/W) */
#define W_SRSCtlEICSS           4
#define M_SRSCtlEICSS           (0xf << S_SRSCtlEICSS)
#define S_SRSCtlESS             12                      /* Exception shadow set (R/W) */
#define W_SRSCtlESS             4
#define M_SRSCtlESS             (0xf << S_SRSCtlESS)
#define S_SRSCtlPSS             6                       /* Previous shadow set (R/W) */
#define W_SRSCtlPSS             4
#define M_SRSCtlPSS             (0xf << S_SRSCtlPSS)
#define S_SRSCtlCSS             0
#define W_SRSCtlCSS             4
#define M_SRSCtlCSS             (0xf << S_SRSCtlCSS)    /* Current Shadow set (R/W) */

#define M_SRSCtl0Fields         0xc3c30c30
#define M_SRSCtlRFields         0x3c3c000f


#define KUSEG_MSK                 0x80000000
#define KSEG_MSK                  0xE0000000
#define KUSEGBASE                 0x00000000
#define KSEG0BASE                 0x80000000
#define KSEG1BASE                 0xA0000000
#define KSSEGBASE                 0xC0000000
#define KSEG3BASE                 0xE0000000

