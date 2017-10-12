/**
    IPL dzoom table F22P_EVB_FF.

    @file       IPL_dzoomTableF22P_EVB_FF.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "IPL_F22P_EVB_FF_Int.h"

const UINT32 VDOZOOM_INFOR_MODE_1_TABLE[1][DZOOM_ITEM_MAX] =
{
    //sie         sie
    //crop out    ch0 out      pre in
    { 1920, 1080, 1920, 1080, 1920, 1080},
};

UINT32* SenMode2Tbl(UINT32 SenMode, UINT32 *DzMaxidx)
{
    UINT32 *Ptr;
    switch(SenMode)
    {
        case SENSOR_MODE_1:
        case SENSOR_MODE_2:
        case SENSOR_MODE_3:
        case SENSOR_MODE_4:
        case SENSOR_MODE_5:
            Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_1_TABLE[0][0];
            *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_1_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            break;

        default:
            DBG_ERR("Wrong Dzoom Mode\r\n");
            Ptr = NULL;
            *DzMaxidx = 0;
            break;
    }
    return Ptr;
}
