#ifndef _IPL_CAL_CTRL_MN34110_EVB_FF_INT_H_
#define _IPL_CAL_CTRL_MN34110_EVB_FF_INT_H_
#include "ipl_calapi.h"
#include "ipl_calibration.h"
typedef struct
{
    //prv
    fptr  fpPrv_CtrlInit;
    //cap
    fptr  fpISO_CtrlInit;
    fptr  fpAWB_CtrlInit;
    fptr  fpECS_CtrlInit;
    fptr  fpMSHUT_CtrlInit;
    fptr  fpDP_CtrlInit;
}CAL_CTRL_OBJ,*pCAL_CTRL_OBJ;


extern pCAL_CTRL_OBJ Cal_CtrlInit(void);

extern ER Set2Preview_MN34110_EVB_FF(UINT32 BufAddr,UINT32 BufSize);
extern pCAL_ITEM_OBJ CAL_ITEM_MN34110_EVB_FF(void);
#endif