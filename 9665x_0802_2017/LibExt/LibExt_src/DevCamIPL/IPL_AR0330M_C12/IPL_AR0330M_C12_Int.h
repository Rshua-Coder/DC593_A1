#ifndef _IPL_AR0330M_C12_INT_H_
#define _IPL_AR0330M_C12_INT_H_

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

//AR0330M_C12
#include "IPL_Cmd_AR0330M_C12_Int.h"
#include "IPL_Mode_AR0330M_C12_Int.h"
#include "IPL_Buf_AR0330M_C12_Int.h"
#include "IPL_Ctrl_AR0330M_C12_Int.h"
#include "IPL_CtrlNormal_AR0330M_C12_Int.h"
#include "IPL_CtrlFWPProc_AR0330M_C12_Int.h"
#include "IPL_CtrlFW_AR0330M_C12_Int.h"
#include "IPL_DZoomTab_AR0330M_C12_Int.h"
#include "IPL_FlowCB_AR0330M_C12_Int.h"
#include "IPL_Isr_AR0330M_C12_Int.h"
#if (IQS_VERSION == IQS_VERSION_1)
#include "IQS_SettingFlow_AR0330M_C12_Int.h"
#include "IQS_SettingTable_AR0330M_C12_Int.h"
#include "IQS_ImageEffectTable_AR0330M_C12_Int.h"
#else
#include "IQS2_SettingFlow_AR0330M_C12_Int.h"
#include "IQS2_SettingTable_AR0330M_C12_Int.h"
#include "IQS2_ImageEffectTable_AR0330M_C12_Int.h"
#endif


#define IQS_DUMP_ENGREG 0
#define AWB_CAPRAW_EMB_DEBUG_INFO 0

//calibration
#define CAL_ECS DISABLE//ENABLE
#define CAL_DP  DISABLE//ENABLE

#endif //_IPL_AR0330M_C12_INT_H_
