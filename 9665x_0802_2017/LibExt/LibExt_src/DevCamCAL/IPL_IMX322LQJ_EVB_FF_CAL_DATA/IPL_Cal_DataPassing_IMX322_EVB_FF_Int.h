#ifndef _IPL_CAL_DATAPASSING_IMX322_EVB_FF_INT_H_
#define _IPL_CAL_DATAPASSING_IMX322_EVB_FF_INT_H_
#include "IPL_HAL.h"
#include "IPL_CALRW.h"
typedef struct
{
    UINT32      Rg;
    UINT32      Bg;
    bitratio    BitSel;
}CAL_AWB_FMT;

typedef struct
{
    ECS_BitRatio    Ecs_bit;
    ECSMAP_ENUM     Ecs_map;
}CAL_ECS_HEADER;

typedef struct
{
    CAL_ECS_HEADER  header;
    UINT32          ECSBlockWin;
}CAL_ECS_FMT;

typedef struct
{
    UINT32 PixelCnt;    ///< total defect pixel number
    UINT32 LineCnt;     ///< total line number
}CAL_DPC_HEADER;

typedef struct
{
    CAL_DPC_HEADER     DPCnt;
    UINT32          DP_Pool;    ///< defect pixel coordinate pool(pool size can refer to DPCnt)
}CAL_DP_FMT;

typedef struct
{
    UINT32  GainBase;
}CAL_ISO_FMT;

typedef struct
{
    UINT32  DelayUS;
}CAL_MSHUT_FMT;

typedef struct
{
    UINT32  ChkStatus;
}CAL_AECHK_FMT;

typedef enum
{
    SEC_AWB     = CAL_SEC_A,
    SEC_ECS     = CAL_SEC_B,
    SEC_DP      = CAL_SEC_C,
    SEC_MSHUT   = CAL_SEC_D,
    SEC_ISO     = CAL_SEC_E,
    SEC_AECHK   = CAL_SEC_F,
    ENUM_DUMMY4WORD(CAL_SECMAP)
}CAL_SECMAP;


//base on sensor mode, focuse, ...etc.
typedef enum
{
    TAG_NONE    = 0,

    TAG_AWB_1   = 0x01000000,//
    TAG_AWB_2   = 0x01000001,

    TAG_ECS_1   = 0x02000000,
    TAG_ECS_2   = 0x02000001,

    TAG_DP_1    = 0x03000000,
    TAG_DP_2    = 0x03000001,

    TAG_MSHUT_1 = 0x04000000,
    TAG_MSHUT_2 = 0x04000001,

    TAG_ISO_1   = 0x05000000,
    TAG_ISO_2   = 0x05000001,

    TAG_AECHK_1 = 0x06000000,
    TAG_AECHK_2 = 0x06000001,
    ENUM_DUMMY4WORD(CAL_TAGID)
}CAL_TAGID;

/**
    for IPL sample to get calibration information
    param:
        input:
            Idx:(ex)
            when SecID == SEC_ECS, Idx is zoom section
                 SecID == SEC_AWB, Idx is sensor mode
    return:
        NULL if no matched.
        Otherwise,return pointer of Current Dram data
*/
extern CAL_TAGID GetTagID(CAL_SECMAP SecID,UINT32 Idx);
CAL_TAG* GetCalData(CAL_SECMAP SecID,UINT32 Idx);
#endif
