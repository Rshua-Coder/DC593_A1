#ifndef _IPL_GC2023P_EVB_FF_INT_H_
#define _IPL_GC2023P_EVB_FF_INT_H_

#define IQS_VERSION_1 1
#define IQS_VERSION_2 2
#define IQS_VERSION IQS_VERSION_2
//other
#include "type.h"
#include "Sensor.h"
#include "Ae_api.h"
#include "Ae_Task.h"
#include "Awb_api.h"
#include "Awb_Task.h"
#include "Af_api.h"
#include "Af_Task.h"
#include "PhotoFrame.h"
#include "DataStamp.h"
#include "IPL_Cmd.h"
#include "IPL_Utility.h"
#include "flashlight_api.h"
#include "dis_alg.h"
#include "awb_alg.h"

//common
#include "IPL_HAL_OP_Int.h"
#include "IPL_Ctrl_Int.h"
#include "IPL_Ctrl_RSC_Int.h"
#include "IPL_Ctrl_Flow_Int.h"
#include "IPL_Buffer_Int.h"
#include "IPL_CBMsg_Int.h"
#include "IPL_Display.h"
#include "IPL_dzoom_Int.h"
#include "IPL_CtrlInfor_Int.h"
#include "IPL_AlgInfor_Int.h"
#include "IPL_PostProc_Int.h"
#include "IPL_PostProc_HDR_Int.h"
#include "IPL_PostProc_DS_Int.h"
#include "IPL_PostProc_PF_Int.h"
#include "IPL_Task_Int.h"
#include "IPL_Cmd_Int.h"
#include "IPL_Debug_Int.h"
#include "IQS_SettingFlow.h"
#include "IQS_SettingFlow_Id_Int.h"
#include "IQS_Utility.h"

//GC2023P_EVB_FF
#include "IPL_Cmd_GC2023P_EVB_FF_Int.h"
#include "IPL_Mode_GC2023P_EVB_FF_Int.h"
#include "IPL_Buf_GC2023P_EVB_FF_Int.h"
#include "IPL_Ctrl_GC2023P_EVB_FF_Int.h"
#include "IPL_CtrlNormal_GC2023P_EVB_FF_Int.h"
#include "IPL_CtrlFWPProc_GC2023P_EVB_FF_Int.h"
#include "IPL_CtrlFW_GC2023P_EVB_FF_Int.h"
#include "IPL_DZoomTab_GC2023P_EVB_FF_Int.h"
#include "IPL_FlowCB_GC2023P_EVB_FF_Int.h"
#include "IPL_Isr_GC2023P_EVB_FF_Int.h"
#if (IQS_VERSION == IQS_VERSION_1)
#include "IQS_SettingFlow_GC2023P_EVB_FF_Int.h"
#include "IQS_SettingTable_GC2023P_EVB_FF_Int.h"
#include "IQS_ImageEffectTable_GC2023P_EVB_FF_Int.h"
#else
#include "IQS2_SettingFlow_GC2023P_EVB_FF_Int.h"
#include "IQS2_SettingTable_GC2023P_EVB_FF_Int.h"
#include "IQS2_ImageEffectTable_GC2023P_EVB_FF_Int.h"
#endif

#include "GyroDrv.h"

#define IQS_DUMP_ENGREG 0
#define AWB_CAPRAW_EMB_DEBUG_INFO 0

extern void IPL_FCB_FlashTrig(FLASH_INFOR *Data);
extern void IPL_FCB_MshutterOpen(void);
extern void IPL_FCB_MshutterClose(UINT32 uiEvent);
#endif //_IPL_GC2023P_EVB_FF_INT_H_