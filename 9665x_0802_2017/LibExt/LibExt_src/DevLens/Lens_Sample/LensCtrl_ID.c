#include "LensAPI.h"
#include "SysKer.h"
#include "LensCtrlTsk.h"

#define PRI_LENSCTRL            7
#define STKSIZE_LENSCTRL        4096//2048
extern void LensCtrlTsk(void);

UINT32 FLG_ID_LENS = 0;
UINT32 LENSCTRLTSK_ID = 0;
UINT32 SEMID_LENS_CTLID = 0;

void LensCtrl_InstallID(void)
{
    OS_CONFIG_FLAG(FLG_ID_LENS);
    OS_CONFIG_TASK(LENSCTRLTSK_ID,   PRI_LENSCTRL,  STKSIZE_LENSCTRL,   LensCtrlTsk);
    OS_CONFIG_SEMPHORE(SEMID_LENS_CTLID, 0, 1, 1);
}
