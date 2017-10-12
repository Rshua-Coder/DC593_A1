#ifndef _IPL_SAMPLE_INT_H_
#define _IPL_SAMPLE_INT_H_

//other
#include "type.h"
#include "DxSensor.h"
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

//sample
#include "IPL_Cmd_IMX083_Sample_FF_Int.h"
#include "IPL_Mode_IMX083_Sample_FF_Int.h"
#include "IPL_Buf_IMX083_Sample_FF_Int.h"
#include "IPL_Ctrl_IMX083_Sample_FF_Int.h"
#include "IPL_CtrlNormal_IMX083_Sample_FF_Int.h"
#include "IPL_CtrlFWPProc_IMX083_Sample_FF_Int.h"
#include "IPL_CtrlFW_IMX083_Sample_FF_Int.h"
#include "IPL_DZoomTab_IMX083_Sample_FF_Int.h"
#include "IPL_FlowCB_IMX083_Sample_FF_Int.h"
#include "IPL_Isr_IMX083_Sample_FF_Int.h"
#include "IQS_SettingFlow_IMX083_Sample_FF_Int.h"
#include "IQS_SettingTable_IMX083_Sample_FF_Int.h"
#include "IQS_ImageEffectTable_IMX083_Sample_FF_Int.h"

#include "GyroDrv.h"

#define IQS_DUMP_ENGREG 0
#define AWB_CAPRAW_EMB_DEBUG_INFO 0
#define DMA333 ENABLE//DISABLE

#endif //_IPL_SAMPLE_INT_H_
