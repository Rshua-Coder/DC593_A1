/**
    IPL_Ctrl module (ISR Event)

    @file       IPL_ISREvent.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_ISREVENT_H_
#define _IPL_ISREVENT_H_
#include "type.h"
#include "IPL_Utility.h"
/** \addtogroup mILibIPH */
//@{
typedef void (*DRV_ISR)(UINT32);//input is ISR status
//1st is process id, 2nd UINT32 is isr status,3rd UINT32 is img idx which start at 1 during streaming
typedef void (*IPL_ISR_EVENT_FP)(IPL_PROC_ID,UINT32,UINT32);

/**
    Engine ISR enum
*/
typedef enum _IPL_ISR
{
    IPL_ISR_UNKNOWN = 0,
    IPL_ISR_SIE = 1,    ///< SIE
    IPL_ISR_IFE = 2,    ///< IFE
    IPL_ISR_IPE = 3,    ///< IPE
    IPL_ISR_IME = 4,    ///< IME
    IPL_ISR_PRE = 5,    ///< PRE
    IPL_ISR_IFE2= 6,    ///< IFE2
    IPL_ISR_DCE = 7,    ///< DCE
    IPL_ISR_RDE = 8,    ///< RDE
    IPL_ISR_SIE2 = 9,   ///< SIE2
    IPL_ISR_MAX_CNT = 10,
    ENUM_DUMMY4WORD(IPL_ISR)
} IPL_ISR;

/**
     isr status
*/
typedef enum _IPL_ISR_STATUS
{
    //ISR_REV         = 0,

    //SIE_EFLD        = 0x00000001,       ///< Even field end
    //SIE_OFLD        = 0x00000002,       ///< Odd field end
    SIE_FLDEND      = 0x00000004,       ///< Field end
    SIE_BP1         = 0x00000010,       ///< Break point 1
    SIE_BP2         = 0x00000020,       ///< Break point 2
    SIE_BP3         = 0X00000040,       ///< Break point 3
    SIE_ACTST       = 0x00000080,       ///< Active window start
    SIE_CRPST       = 0X00000100,       ///< Crop window start
    SIE_FLASH_T     = 0x00000400,       ///< Flash trigger from external pin
    SIE_MSH_T       = 0x00000800,       ///< Mshutter trigger from external pin
    SIE_FLSH_END    = 0x00001000,       ///< Flash end
    SIE_MSH_CLOSE_END   = 0x00002000,   ///< Mshutter close end
    SIE_MSH_OPEN_END    = 0X00004000,   ///< Mshutter open end
    SIE_DRAM0_END   = 0x00100000,       ///< Dram output channel 0 end
    SIE_DRAM1_END   = 0X00200000,       ///< Dram output channel 1 end
    SIE_DRAM2_END   = 0X00400000,       ///< Dram output channel 2 end
    SIE_DPCWARN     = 0X01000000,       ///< Defect Pixel/Column concealment warning,happened at continus bp.(more than 2 bp)
    SIE_VAEND       = 0X02000000,       ///< VA output end
    SIE_RAWENC_BUFF = 0X08000000,       ///< Raw encoder output buffer full

    //IPE_FMS         = 0x00000001,   ///< frame start
    IPE_FMD         = 0x00000002,   ///< frame end
    IPE_STPE        = 0X00000004,   ///< stripe end
    IPE_YCC_DMAE    = 0x00000010,   ///< ycc dma output end
    IPE_VA_DMAE     = 0x00000020,   ///< va dma output end
    IPE_ETH_DMAE    = 0X00000040,   ///< edge threshold dma output end
    IPE_CSUB_DMAE   = 0X00000080,   ///< CC subsample dma output end
    IPE_VSUB_DMAE   = 0X00000100,   ///< v subsample dma output end
    IPE_GAMMA_END   = 0X00000200,   ///< gamma dma in end
    IPE_3DLUT_END   = 0x00000400,   ///< 3d lut dma in end

    PRE_FMS         = 0x00000001,   ///<Frame start
    PRE_FMD         = 0x00000002,   ///<Frame end
    PRE_HSTPEND     = 0x00000004,   ///<Horizontal stripe end
    PRE_EMDPCW      = 0x00000008,   ///<Embedded defect pixel warning

    IFE_FMD         = 0x00000001,   ///< frame end interrupt
    //IFE_BP1         = 0x00000002,   ///< bp1 interrupt
    IFE_ROWDEFW     = 0x00000004,   ///< row defect warning interrupt
    IFE_HSTPE       = 0x00000008,   ///< H stripe end

    IFE2_FMD        = 0x00000001,   ///<Frame end

    IME_STPE        = 0X20000000,   ///< stripe end
    //IME_FMS         = 0X40000000,   ///< frame start
    IME_FMD         = 0x80000000,   ///< frame end

    DCE_FMS         = 0x00000001,   ///< frame start
    DCE_FMD         = 0x00000002,   ///< frame end
    DCE_STRPE       = 0x00000004,   ///< current stripe end

    RDE_FMD         = 0x00000001,   ///< frame end
    RDE_ERR         = 0x0000000E,   ///< decode err(under/over flow,bitstream err)
}IPL_ISR_STATUS;

//@}
#endif //_IPL_ISREVENT_H_