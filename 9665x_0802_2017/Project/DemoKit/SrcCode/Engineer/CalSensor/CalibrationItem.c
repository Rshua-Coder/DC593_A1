/**
    Calibration item

    @file       CalibrationItem.c
    @ingroup    calibration
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "SxCmd.h"
#include "ae_task.h"
#include "awb_task.h"
#include "IPL_CAL_Debug.h"
#include "ipl_calrw.h"
#include "DxSensor.h"
#include "IPL_Cmd.h"
#include "Ae_api.h"
#include "Awb_api.h"
#include "SysCfg.h"
#include "Utility.h"
#include "SysKer.h"
#include "GxSystem.h"
#include "IPL_Cmd.h"
#include "SysCfg.h"
#include "ImgCaptureAPI.h"
#include "GxDisplay.h"
#include "IPL_Utility.h"
#include "stdio.h"
#include "IPL_Display.h"
#include "SysCommon.h"
#include "adc.h"
#include "UIResource.h"
#include "IPL_Calibration.h"
#include "IPL_calapi.h"
#include "Cal_Header_Int.h"
#include "CalibrationItem.h"
#include "IPL_Cal_KeyStrSample_Int.h"
#include "CalibrationWnd.h"
#include "NvtUser.h"
#include "Dx.h"
#include "DCF.h"

#include "SysKer.h"
#include "GxSystem.h"
#include "GxVideo.h"
#include "DxSensor.h"
#include "IPL_Cmd.h"
#include "Delay.h"
#include "SysCfg.h"
#include "ImgCaptureAPI.h"
#include "GxDisplay.h"
#include "IPL_Utility.h"
#include "IPL_AlgInfor.h"
#include "PhotoTask.h"
#include "stdio.h"
#include "ExamSrvApi.h"
#include "FileSysTsk.h"
#include "IPL_Display.h"
#include "dis_alg.h"
#include "IPL_Ctrl.h"
#include "IPL_CalRW.h"
#include "EngineerMode.h"
#include "ImgCapInfor.h"
#include "Delay.h"
#include "UIMode.h"
#include "MsdcNvtCb.h"
#include "UIGraphics.h"
#include "Ipe_lib.h"
#include "Cal_Header_Int.h"
#include "CalibrationItem.h"
#include "IPL_Cal_KeyStrSample_Int.h"
#include "IPL_CalApi.h"
#include "IPL_AlgInfor.h"
#include "Sensor.h"
#include "PStore.h"
#include "VControl.h"
#include "FileSysTsk.h"
#include "GxVideo.h"
#include "Sie_lib.h"
#if isDemokit
#include "UIAppPhoto.h"
#include "CalibrationWnd.h"
#include "SysCommon.h"
#include "UIResource.h"
#include "IPL_Cmd.h"

#endif

#define MaxSenMode 1

#if 0
- par
#endif
static BOOL m_CalOpen = FALSE;
static UINT32 m_Buf_StartAddr = 0;
static UINT32 m_Buf_Size = 0;
static UINT32 m_Header_Addr = 0;
static UINT32 m_Header_Size = 0;
UINT32 shift_prv_size = 0x2000000;

/**
    for debug information
*/
char m_CalDbgMsg[100];
char m_CalSaveMsg[100];
char m_CalSaveMsgName[100];

/**
    lens information
*/
static char g_lensTypeName[20] = {"N"};
static UINT32 g_lensTypeNum = 0;

#if 0
- static api
#endif

static CAL_DMA_RST _PS_R(UINT32 dmaAddr,UINT32 dmaSize);
extern void IQ_GetAdjustRst(CalIQInfo *CalData);

/*
    This function load pstore data into CAL POOL(DRAM)
*/
ER Cal_PS2MemPool(void)
{
    CAL_DMA_RST ps_rst = {0};

    ps_rst=_PS_R(OS_GetMempoolAddr(POOL_ID_CAL), POOL_SIZE_CAL);

    if(ps_rst.erCode==E_OK)
    {
        m_Header_Addr = OS_GetMempoolAddr(POOL_ID_CAL);
        m_Header_Size = POOL_SIZE_CAL - ps_rst.RemainSize;
    }
    else
    {
        return E_SYS;
    }

    return E_OK;
}

/**
    read header and all sections from pstore to dram,
    if header not exit, init dram for header
*/
static CAL_DMA_RST _PS_R(UINT32 dmaAddr,UINT32 dmaSize)
{
    CAL_DMA_RST ps_rst = {0};

    ps_rst = CAL_RW()->fpSectionAllRead(dmaAddr,dmaSize);
    if(ps_rst.erCode != E_OK)
    {
        if(ps_rst.erCode == E_NOMEM)
            DBG_IND("buf not enougth\r\n");
        //pstore header
        ps_rst = CAL_RW()->fpSectionRead_Header(dmaAddr,dmaSize);
        if(ps_rst.erCode != E_OK)
        {
            DBG_IND("no header\r\n");
            //init should only be executed once at the very first execution of calibration
            ps_rst = CAL_RW()->fpInit(dmaAddr,dmaSize,NULL);
            if(ps_rst.erCode != E_OK)
            {
                DBG_ERR("pstore/dram init fail!\r\n");
            }
            else
            {
                //inorder to let all following calibration functions can be executed correctly.
                //because all cailbration functions' 1st operation is to read header
                if(CAL_RW()->fpSectionWrite_Header() != E_OK)
                    DBG_ERR("header write 2 PS fail\r\n");
            }
        }
    }
    return ps_rst;
}

#if 0
- extern api
#endif
/**
    open all ipl calibration resource
    requirement:
    1. sensor need to be closed.
    2. display ctrl task need to be closed.
    3. ipl_ctrl task need to be closed.
*/
ER Cal_IplOpen(UINT32 BufAddr, UINT32 BufSize)
{
    AE_OBJ_INFO AeObj = {0};
    AWB_OBJ_INFO AwbObj = {0};
    CAL_DMA_RST ps_rst;
    IPL_CHGMODE_DATA CmdData = {0};

    if(m_CalOpen)
    {
        DBG_ERR("\r\n");
        return E_SYS;
    }

    IPL_MODE_INFOR Info;
    Info.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);

    if(Info.Mode != IPL_MODE_OFF)
    {
        CmdData.Mode = IPL_MODE_OFF;
        IPL_SetCmd(IPL_SET_CHGMODE, (void *)&CmdData);
        IPL_WaitCmdFinish();
    }
    if(IPL_IsOpen())
    {
        DBG_ERR("\r\n");
    }
    else
    {
        IPL_Open();
    }

    IPL_Display_CloseTsk();
    //DBG_ERR("Cal_IplOpen IPL_Display_OpenTsk\r\n");

    // open sensor
    SENSOR_INIT_OBJ SenObj;
    SENSOR_DRVTAB *SenTab;

    //DrvSensor_PowerSaveOff();
    SenObj = DrvSensor_GetObj1st();
    SenTab = DrvSensor_GetTab1st();
    Sensor_Open(SENSOR_ID_1, &SenObj, SenTab);



    //open ae
    AeObj.EventTab[IPL_UTI_CONV2_AE_ID(IPL_ID_1)].Start = AE_Init;
    AeObj.EventTab[IPL_UTI_CONV2_AE_ID(IPL_ID_1)].Process = AE_Process;
    AeObj.EventTab[IPL_UTI_CONV2_AE_ID(IPL_ID_1)].Pause = NULL;
    AeObj.ProcNum = 1;
    if(AE_Open(&AeObj))
    {
       DBG_ERR("\r\n");
       return E_SYS;
    }

    //open awb
    AwbObj.EventTab[IPL_UTI_CONV2_AWB_ID(IPL_ID_1)].Start = AWB_Init;
    AwbObj.EventTab[IPL_UTI_CONV2_AWB_ID(IPL_ID_1)].Process = AWB_Process;
    AwbObj.EventTab[IPL_UTI_CONV2_AWB_ID(IPL_ID_1)].Pause = NULL;
    AwbObj.ProcNum = 1;
    if(AWB_Open(&AwbObj))
    {
       DBG_ERR("\r\n");
       return E_SYS;
    }
    ps_rst = _PS_R(BufAddr,BufSize);
    if(ps_rst.erCode != E_OK)
        return E_SYS;
    m_Header_Addr = BufAddr;
    m_Header_Size = BufSize - ps_rst.RemainSize;

    m_Buf_StartAddr = ps_rst.RemainAddr;
    m_Buf_Size = ps_rst.RemainSize;
   if(Set2Preview(m_Buf_StartAddr,m_Buf_Size))
   {
       DBG_ERR("Set2Preview fail\r\n");
   }
    m_CalOpen = TRUE;

    return E_OK;
}

ER Cal_IplClose(void)
{
    // UINT32 j = SENSOR_ID_MAX_NUM;
    if(!m_CalOpen)
    {
        DBG_ERR("\r\n");
        return E_SYS;
    }

    //stop pipeline ycc part
    CalApi()->fpSetMode(CAL_IPL_STOP,NULL,NULL);

    //pause sie
    CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);

    //close sie
    CalApi()->fpSetMode(CAL_IPL_SIECLOSE,NULL,NULL);

    CalApi()->fpBufEvent()->fpUnInit(IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));

    //close AWB
    AWB_Close();

    //close AE
    AE_Close();

    IPL_Close();

    /*
        we set ipl off mode before entry calibration,
        but when shutdown, ipl check status func will hang on,
        so we must set it  to the entry status.
    */
    //DscPhoto_Open();

    Sensor_Close(IPL_ID_1);

    /*
    IPL_Display_CloseTsk();

    //close sensor
    while(j--)
        Sensor_Close(j);
    */

    m_CalOpen = FALSE;
    return E_OK;
}

/**
    show calibration result (OK/NG) on OSD
*/
static BOOL bCalResetData = FALSE;
#define bCalSenMode 0x456
void Cal_GetCalRst(void)
{
    static CHAR m_CalMsg[100];
    CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
    CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
    UINT32 SenModeCondiCnt = 0;
    BOOL TempCalStatus = TRUE;

    // clear OSD
    CAL_UICB()->AWB_UI(UICB_CLEAN,(CHAR*)m_CalMsg,UICOL_BLACK*10);

    // get calibration result
    CAL_ITEM()->fpGetAdjustRst(NULL, CalDebugInfor_DNP);

    /*
        ECS
    */
    TempCalStatus = TRUE;
    // get calibration data and check status
    for (SenModeCondiCnt=0; SenModeCondiCnt < Max_SenMode_Condition; SenModeCondiCnt++)
    {
        // check which sensor mode have been made to the calibration
        //if (CalDebugInfor_DNP->CalDebugECS->CalAeRst[SenModeCondiCnt].ISO != 0)
        if (CalDebugInfor_DNP->CalDebugECS->CalDebugInfor[SenModeCondiCnt] == bCalSenMode)
        {
            // check calibration status
            if (CalDebugInfor_DNP->CalDebugECS->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
            {
                TempCalStatus = FALSE;
            }
        }
    }
    // show cal status on OSD
    if ((TempCalStatus == TRUE)&&(bCalResetData == FALSE))
    {
        sprintf((char*)m_CalMsg, "ECS - Pass");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
    }
    else
    {
        sprintf((char*)m_CalMsg, "ECS - Fail");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
    }

    /*
        ISO
    */
    TempCalStatus = TRUE;
    // get calibration data and check status
    for (SenModeCondiCnt=0; SenModeCondiCnt < MaxSenMode; SenModeCondiCnt++)
    {
        // check which sensor mode have been made to the calibration
        //if (CalDebugInfor_DNP->CalDebugISO->CalAeRst[SenModeCondiCnt].ISO != 0)
        if (CalDebugInfor_DNP->CalDebugISO->CalDebugInfor[SenModeCondiCnt] == bCalSenMode)
        {
            // check calibration status
            if (CalDebugInfor_DNP->CalDebugISO->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
            {
                TempCalStatus = FALSE;
            }
        }
    }
    // show cal status on OSD
    if ((TempCalStatus == TRUE)&&(bCalResetData == FALSE))
    {
        sprintf((char*)m_CalMsg, "ISO - Pass");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
    }
    else
    {
        sprintf((char*)m_CalMsg, "ISO - Fail");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
    }

    /*
        AWB
    */
    TempCalStatus = TRUE;
    // get calibration data and check status
    for (SenModeCondiCnt=0; SenModeCondiCnt < MaxSenMode; SenModeCondiCnt++)
    {
        // check which sensor mode have been made to the calibration
        //if (CalDebugInfor_DNP->CalDebugAWB->CalAeRst[SenModeCondiCnt].ISO != 0)
        if (CalDebugInfor_DNP->CalDebugAWB->CalDebugInfor[SenModeCondiCnt] == bCalSenMode)
        {
            // check calibration status
            if (CalDebugInfor_DNP->CalDebugAWB->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
            {
                TempCalStatus = FALSE;
            }
        }
    }
    // show cal status on OSD
    if ((TempCalStatus == TRUE)&&(bCalResetData == FALSE))
    {
        sprintf((char*)m_CalMsg, "AWB - Pass");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
    }
    else
    {
        sprintf((char*)m_CalMsg, "AWB - Fail");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
    }

    /*
        DP dark
    */
    TempCalStatus = TRUE;
    // get calibration data and check status
    for (SenModeCondiCnt=0; SenModeCondiCnt < MaxSenMode; SenModeCondiCnt++)
    {
        // check which sensor mode have been made to the calibration
        //if (CalDebugInfor_DNP->CalDebugDP->CalAeRstDark[SenModeCondiCnt].ISO != 0)
        if (CalDebugInfor_DNP->CalDebugDP->CalDebugInfor[SenModeCondiCnt] == bCalSenMode)
        {
            // check calibration status
            if ((CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
                &&(CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_DP_D_OK))
            {
                TempCalStatus = FALSE;
            }
        }
    }
    // show cal status on OSD
    if ((TempCalStatus == TRUE)&&(bCalResetData == FALSE))
    {
        sprintf((char*)m_CalMsg, "DP dark - Pass");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
    }
    else
    {
        sprintf((char*)m_CalMsg, "DP dark - Fail");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
    }

    /*
        DP bright
    */
    TempCalStatus = TRUE;
    // get calibration data and check status
    for (SenModeCondiCnt=0; SenModeCondiCnt < MaxSenMode; SenModeCondiCnt++)
    {
        // check which sensor mode have been made to the calibration
        //if (CalDebugInfor_DNP->CalDebugDP->CalAeRstBright[SenModeCondiCnt].ISO != 0)
        if (CalDebugInfor_DNP->CalDebugDP->CalDebugInfor[SenModeCondiCnt] == bCalSenMode)
        {
            // check calibration status
            if ((CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
                &&(CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_DP_B_OK))
            {
                TempCalStatus = FALSE;
            }
        }
    }
    // show cal status on OSD
    if ((TempCalStatus == TRUE)&&(bCalResetData == FALSE))
    {
        sprintf((char*)m_CalMsg, "DP bright - Pass");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
    }
    else
    {
        sprintf((char*)m_CalMsg, "DP bright - Fail");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
    }

    // wait any key (keep result on OSD)
    // CAL_UICB()->AWB_UI(UICB_WAITKEY,(CHAR*)m_CalMsg,UICOL_GREEN);

    // wait any key to power off
    CAL_UICB()->AWB_UI(UICB_PROCESSING, NULL, UICOL_NONE); // wait any key to continue
    System_PowerOff(0);
}

void Cal_Reset_Data(void)
{
    ER ER_Code = E_OK;
    static CHAR m_CalMsg[100];

    ER_Code = CAL_RW()->fpDeleteSection(CAL_SEC_A);
    if ((ER_Code != E_OK)&&(ER_Code != E_NOEXS))
        goto finish;

    ER_Code = CAL_RW()->fpDeleteSection(CAL_SEC_B);
    if ((ER_Code != E_OK)&&(ER_Code != E_NOEXS))
        goto finish;

    ER_Code = CAL_RW()->fpDeleteSection(CAL_SEC_C);
    if ((ER_Code != E_OK)&&(ER_Code != E_NOEXS))
        goto finish;

    ER_Code = CAL_RW()->fpDeleteSection(CAL_SEC_E);
    if ((ER_Code != E_OK)&&(ER_Code != E_NOEXS))
        goto finish;


    finish:
    if ((ER_Code != E_OK)&&(ER_Code != E_NOEXS))
    {
        DBG_ERR("delete pstore data error %d!!!!!\r\n", ER_Code);
        sprintf((char*)m_CalMsg, "delete pstore data error!!!!!");
        CAL_UICB()->AWB_UI(UICB_END,(CHAR*)m_CalMsg, UICOL_RED*10 + UICOL_WHITE);
    }
    else
    {
        DBG_IND("delete all CAL data!\r\n");
        sprintf((char*)m_CalMsg, "delete all CAL data!");
        CAL_UICB()->AWB_UI(UICB_END,(CHAR*)m_CalMsg, UICOL_GREEN*10 + UICOL_BLACK);
        bCalResetData = TRUE;
    }

    // wait any key to power off
    CAL_UICB()->AWB_UI(UICB_PROCESSING, NULL, UICOL_NONE); // wait any key to continue
    System_PowerOff(0);
}

static CAL_STATUS DNP_STATUS[5] =
{/*  ECS         ISO       AWB      DP Dark   DP Bright*/
  _CAL_NONE, _CAL_NONE, _CAL_NONE, _CAL_NONE, _CAL_NONE,
};
static BOOL showDNP(void)
{
    UINT32 StationCnt = 0;
    static CHAR m_CalMsg[100];
    BOOL CALRst = TRUE;

    CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_BLACK*10);
    //sprintf((char*)m_CalMsg, "<RESULT>");
    //CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_WHITE);
    for (StationCnt = 0; StationCnt < 5; StationCnt++)
    {

        if (DNP_STATUS[StationCnt] == _CAL_OK)
        {
            switch(StationCnt)
            {
                case 0://ECS
                    sprintf((char*)m_CalMsg, "ECS OK");
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
                    break;
                case 1://ISO
                    sprintf((char*)m_CalMsg, "ISO OK");
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
                     break;
                case 2://AWB
                    sprintf((char*)m_CalMsg, "AWB OK");
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
                     break;
                case 3://DP dark
                    sprintf((char*)m_CalMsg, "DP dark OK");
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
                     break;
                case 4://DP bright
                    sprintf((char*)m_CalMsg, "DP bright OK");
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
                     break;
                default:
                    break;
            }
        }
        else if (DNP_STATUS[StationCnt] == _CAL_OverFlow)
        {
            CALRst = FALSE;
            switch(StationCnt)
            {
                case 0://ECS
                    sprintf((char*)m_CalMsg, "ECS FAIL");
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                    break;
                case 1://ISO
                    sprintf((char*)m_CalMsg, "ISO FAIL");
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                     break;
                case 2://AWB
                    sprintf((char*)m_CalMsg, "AWB FAIL");
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                     break;
                case 3://DP dark
                    sprintf((char*)m_CalMsg, "DP dark FAIL");
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                     break;
                case 4://DP bright
                    sprintf((char*)m_CalMsg, "DP bright FAIL");
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                     break;
                default:
                    break;
            }
        }
        else // no result
        {
            CALRst = FALSE;
            switch(StationCnt)
            {
                case 0://ECS
                    sprintf((char*)m_CalMsg, "ECS NO RESULT");
                    //CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                    break;
                case 1://ISO
                    sprintf((char*)m_CalMsg, "ISO NO RESULT");
                    //CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                     break;
                case 2://AWB
                    sprintf((char*)m_CalMsg, "AWB NO RESULT");
                    //CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                     break;
                case 3://DP dark
                    sprintf((char*)m_CalMsg, "DP dark NO RESULT");
                    //CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                     break;
                case 4://DP bright
                    sprintf((char*)m_CalMsg, "DP bright NO RESULT");
                    //CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                     break;
                default:
                    break;
            }
        }
    }

    return CALRst;
}

static BOOL getDNPRst(void)
{
    IPL_PROC_ID Id = IPL_ID_1;
    UINT32 StationCnt = 0;
    BOOL CALRst = TRUE;

    for (Id = 0; Id <= IPL_ID_MAX_NUM; Id++)
    {
        for (StationCnt = 0; StationCnt < 5; StationCnt++)
        {
            if (DNP_STATUS[StationCnt] == _CAL_OverFlow)
            {
                CALRst = FALSE;
            }
        }
    }
    return CALRst;
}

void Cal_SenECS(void)
{
#if 0 // ui sample
    ER ER_Code=E_OK;
    static CHAR m_CalMsg[100];

    CAL_UICB()->AWB_UI(UICB_CLEAN, NULL,UICOL_BLACK*10);

    sprintf((char*)m_CalMsg, "ECS ING");
    CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_WHITE);
    sprintf((char*)m_CalMsg, "ECS OK");
    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);

    sprintf((char*)m_CalMsg, "ISO ING");
    CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_WHITE);
    sprintf((char*)m_CalMsg, "ISO NG");
    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);

    sprintf((char*)m_CalMsg, "Debug OK");
    CAL_UICB()->AWB_UI(UICB_END, (CHAR*)m_CalMsg,UICOL_GREEN*10 + UICOL_WHITE);

    CAL_UICB()->AWB_UI(UICB_CLEAN, NULL,UICOL_BLACK*10);

    sprintf((char*)m_CalMsg, "ECS OK");
    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg, UICOL_GREEN);

    sprintf((char*)m_CalMsg, "ISO NG");
    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg, UICOL_RED);
#endif

    CAL_DMA_RST ps_rst;
    BOOL CalRstOK = TRUE;
    static CHAR m_CalMsg[100];

    sprintf((char*)m_CalMsg, "ECS ing - %s", g_lensTypeName);
    CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_WHITE);

    if(m_Header_Addr==0)
    {
        Cal_PS2MemPool();
    }

    Cal_IplOpen(OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));

    if(!m_CalOpen)
    {
        DBG_ERR("Calibration not open yet\r\n");
        return;
    }

    if(CAL_ITEM()->fpECSAdj(CAL_UICB()->ECS_UI,m_Buf_StartAddr+shift_prv_size,m_Buf_Size-shift_prv_size) == E_OK)
    {
        if(CAL_RW()->fpSectionWrite_Header() != E_OK)
            DBG_ERR("\r\n");

        //re-read all section because of maybe pstore have new section information after calibration function.
        ps_rst = _PS_R(m_Header_Addr,(m_Header_Size + m_Buf_Size));
        //must be return E_OK at each calibration function excluding Cal_IplOpen()
        if(ps_rst.erCode != E_OK)
            return;
        m_Header_Size = (m_Header_Size + m_Buf_Size) - ps_rst.RemainSize;
        m_Buf_StartAddr = ps_rst.RemainAddr;
        m_Buf_Size = ps_rst.RemainSize;
    }

    /*
        1. get calibration result
        2. (save result in txt) depend on eng.cfg
        3. get calibration final result to show on OSD
    */
    CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
    CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
    UINT32 SenModeCondiCnt = 0;
    // get calibration result
    CAL_ITEM()->fpGetAdjustRst(NULL, CalDebugInfor_DNP);

    for (SenModeCondiCnt=0; SenModeCondiCnt < MaxSenMode; SenModeCondiCnt++)
    {
        if (CalDebugInfor_DNP->CalDebugECS->CalAeRst[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
        {
            if (CalDebugInfor_DNP->CalDebugECS->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
            {
                CalRstOK = FALSE;
            }
        }
    }

    /*
            show calibration final result on OSD
    */
    //CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_RED);
    if (CalRstOK == FALSE)
    {
        DNP_STATUS[0] = _CAL_OverFlow;
    }
    else
    {
        DNP_STATUS[0] = _CAL_OK;
    }

    Cal_IplClose();

    DBG_IND("===== ECS finish =====\r\n");

}
void Cal_SenISO(void)
{
    CAL_DMA_RST ps_rst;
    BOOL CalRstOK = TRUE;
    static CHAR m_CalMsg[100];

    sprintf((char*)m_CalMsg, "ISO ing - %s", g_lensTypeName);
    CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_WHITE);

    Cal_IplOpen(OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));

    if(!m_CalOpen)
    {
        DBG_ERR("Calibration not open yet\r\n");
        return;
    }

    if(CAL_ITEM()->fpISOAdj(CAL_UICB()->ISO_UI,m_Buf_StartAddr+shift_prv_size,m_Buf_Size-shift_prv_size) == E_OK)
    {
        if(CAL_RW()->fpSectionWrite_Header() != E_OK)
            DBG_ERR("\r\n");

        //re-read all section because of maybe pstore have new section information after calibration function.
        ps_rst = _PS_R(m_Header_Addr,(m_Header_Size + m_Buf_Size));
        //must be return E_OK at each calibration function excluding Cal_IplOpen()
        if(ps_rst.erCode != E_OK)
            return;
        m_Header_Size = (m_Header_Size + m_Buf_Size) - ps_rst.RemainSize;
        m_Buf_StartAddr = ps_rst.RemainAddr;
        m_Buf_Size = ps_rst.RemainSize;
    }

    /*
        1. get calibration result
        2. get calibration final result to show on OSD
    */
    CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
    CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
    UINT32 SenModeCondiCnt = 0;
    // get calibration result
    CAL_ITEM()->fpGetAdjustRst(NULL, CalDebugInfor_DNP);
    for (SenModeCondiCnt=0; SenModeCondiCnt < MaxSenMode; SenModeCondiCnt++)
    {
        if (CalDebugInfor_DNP->CalDebugISO->CalAeRst[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
        {
            if (CalDebugInfor_DNP->CalDebugISO->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
            {
                CalRstOK = FALSE;
            }
        }
    }

    /*
            show calibration final result on OSD
    */
    //CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_RED);
    if (CalRstOK == FALSE)
    {
        DNP_STATUS[1] = _CAL_OverFlow;
    }
    else
    {
        DNP_STATUS[1] = _CAL_OK;
    }

    Cal_IplClose();

    DBG_IND("===== ISO finish =====\r\n");

}

void Cal_SenAWB(void)
{
    CAL_DMA_RST ps_rst;
    BOOL CalRstOK = TRUE;
    static CHAR m_CalMsg[100];

    sprintf((char*)m_CalMsg, "AWB ing - %s", g_lensTypeName);
    CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_WHITE);

    Cal_IplOpen(OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));

    if(!m_CalOpen)
    {
        DBG_ERR("Calibration not open yet\r\n");
        return;
    }

    if(CAL_ITEM()->fpAWBAdj(CAL_UICB()->AWB_UI,m_Buf_StartAddr+shift_prv_size,m_Buf_Size-shift_prv_size) == E_OK)
    {
        if(CAL_RW()->fpSectionWrite_Header() != E_OK)
            DBG_ERR("\r\n");

        //re-read all section because of maybe pstore have new section information after calibration function.
        ps_rst = _PS_R(m_Header_Addr,(m_Header_Size + m_Buf_Size));
        //must be return E_OK at each calibration function excluding Cal_IplOpen()
        if(ps_rst.erCode != E_OK)
            return;
        m_Header_Size = (m_Header_Size + m_Buf_Size) - ps_rst.RemainSize;
        m_Buf_StartAddr = ps_rst.RemainAddr;
        m_Buf_Size = ps_rst.RemainSize;
    }

    /*
        1. get calibration result
        2. get calibration final result to show on OSD
    */
    CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
    CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
    UINT32 SenModeCondiCnt = 0;
    // get calibration result
    CAL_ITEM()->fpGetAdjustRst(NULL, CalDebugInfor_DNP);
    for (SenModeCondiCnt=0; SenModeCondiCnt < MaxSenMode; SenModeCondiCnt++)
    {
        if (CalDebugInfor_DNP->CalDebugAWB->CalAeRst[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
        {
            if (CalDebugInfor_DNP->CalDebugAWB->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
            {
                CalRstOK = FALSE;
            }
        }
    }

    /*
            show calibration final result on OSD
    */
    //CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_RED);
    if (CalRstOK == FALSE)
    {
        DNP_STATUS[2] = _CAL_OverFlow;
    }
    else
    {
        DNP_STATUS[2] = _CAL_OK;
    }

    Cal_IplClose();

    DBG_IND("===== AWB finish =====\r\n");

}

void Cal_SenDP_Dark(void)
{
    CAL_DMA_RST ps_rst;
    BOOL CalRstOK = TRUE;
    static CHAR m_CalMsg[100];

    sprintf((char*)m_CalMsg, "DP(Dark) ing - %s", g_lensTypeName);
    CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_WHITE);

    Cal_IplOpen(OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));

    if(!m_CalOpen)
    {
        DBG_ERR("Calibration not open yet\r\n");
        return;
    }

    if(CAL_ITEM()->fpDP_Adj_Dark(CAL_UICB()->DP_W_UI,m_Buf_StartAddr+shift_prv_size,m_Buf_Size-shift_prv_size) == E_OK)
    {
        if(CAL_RW()->fpSectionWrite_Header() != E_OK)
            DBG_ERR("\r\n");

        //re-read all section because of maybe pstore have new section information after calibration function.
        ps_rst = _PS_R(m_Header_Addr,(m_Header_Size + m_Buf_Size));
        //must be return E_OK at each calibration function excluding Cal_IplOpen()
        if(ps_rst.erCode != E_OK)
            return;
        m_Header_Size = (m_Header_Size + m_Buf_Size) - ps_rst.RemainSize;
        m_Buf_StartAddr = ps_rst.RemainAddr;
        m_Buf_Size = ps_rst.RemainSize;
    }

    /*
        1. get calibration result
        2. get calibration final result to show on OSD
    */
    CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
    CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
    UINT32 SenModeCondiCnt = 0;
    // get calibration result
    CAL_ITEM()->fpGetAdjustRst(NULL, CalDebugInfor_DNP);
    for (SenModeCondiCnt=0; SenModeCondiCnt < MaxSenMode; SenModeCondiCnt++)
    {
        if (CalDebugInfor_DNP->CalDebugDP->CalAeRstDark[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
        {
            if (CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_DP_D_OK)
            {
                CalRstOK = FALSE;
            }
        }
    }

    /*
            show calibration final result on OSD
    */
    //CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_RED);
    if (CalRstOK == FALSE)
    {
        DNP_STATUS[3] = _CAL_OverFlow;
    }
    else
    {
        DNP_STATUS[3] = _CAL_OK;
    }

    Cal_IplClose();

    DBG_IND("===== DP(Dark) finish =====\r\n");

}

void Cal_SenDP_Bright(void)
{
    CAL_DMA_RST ps_rst;
    BOOL CalRstOK = TRUE;
    static CHAR m_CalMsg[100];

    sprintf((char*)m_CalMsg, "DP(Bright) ing - %s", g_lensTypeName);
    CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_WHITE);

    Cal_IplOpen(OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
    if(!m_CalOpen)
    {
        DBG_ERR("Calibration not open yet\r\n");
        return;
    }

    if(CAL_ITEM()->fpDP_Adj_Bright(CAL_UICB()->DP_W_UI,m_Buf_StartAddr+shift_prv_size,m_Buf_Size-shift_prv_size) == E_OK)
    {
        if(CAL_RW()->fpSectionWrite_Header() != E_OK)
            DBG_ERR("\r\n");

        //re-read all section because of maybe pstore have new section information after calibration function.
        ps_rst = _PS_R(m_Header_Addr,(m_Header_Size + m_Buf_Size));
        //must be return E_OK at each calibration function excluding Cal_IplOpen()
        if(ps_rst.erCode != E_OK)
            return;
        m_Header_Size = (m_Header_Size + m_Buf_Size) - ps_rst.RemainSize;
        m_Buf_StartAddr = ps_rst.RemainAddr;
        m_Buf_Size = ps_rst.RemainSize;
    }

    /*
        1. get calibration result
        2. get calibration final result to show on OSD
    */
    CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
    CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
    UINT32 SenModeCondiCnt = 0;
    // get calibration result
    CAL_ITEM()->fpGetAdjustRst(NULL, CalDebugInfor_DNP);
    for (SenModeCondiCnt=0; SenModeCondiCnt < MaxSenMode; SenModeCondiCnt++)
    {
        if (CalDebugInfor_DNP->CalDebugDP->CalAeRstBright[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
        {
            if (CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
            {
                CalRstOK = FALSE;
            }
        }
    }

    /*
            show calibration final result on OSD
    */
    //CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_RED);
    if (CalRstOK == FALSE)
    {
        DNP_STATUS[4] = _CAL_OverFlow;
    }
    else
    {
        DNP_STATUS[4] = _CAL_OK;
    }

    Cal_IplClose();

    DBG_IND("===== DP(Bright) finish =====\r\n");

}

void Cal_SenDNP(void)
{
    static CHAR m_CalMsg[100];
    BOOL CALRst = TRUE;
    ER ER_Code = E_OK;
    char* Read_WifiMAC;

    CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_BLACK*10);
    CAL_UICB()->AWB_UI(UICB_WAITKEY, NULL, UICOL_GREEN);

    bCalResetData = FALSE;

    // check debug cfg in SD card
    CAL_ReadDebugConfig();

    // set wifi mac addr to libext
    Read_WifiMAC = EngineerMode_ReadWiFiMAC();
    sprintf(m_CalSaveMsgName, "%s", Read_WifiMAC);
    sprintf(m_CalSaveMsg, "non-assigned");
    ER_Code = CAL_ITEM()->fpCAL_SaveMsg(&m_CalSaveMsgName[0],&m_CalSaveMsg[0], FALSE);

    // get lens information
    CAL_ITEM()->fpCAL_GetLensInfor(&g_lensTypeName[0], &g_lensTypeNum);

    DBG_IND("get lens type number %d\r\n", g_lensTypeNum);

    if (CAL_ITEM()->fpGetDebugLevel(CALDEBUG_INFO_USER_A)==1)
    {
        sprintf(m_CalSaveMsgName, "A:\\%s_C_NVT.txt", Read_WifiMAC);
        sprintf(m_CalSaveMsg, "lens%s_", g_lensTypeName);
        ER_Code = CAL_ITEM()->fpCAL_SaveMsg(&m_CalSaveMsgName[0],&m_CalSaveMsg[0], TRUE);
    }
    /*
        ECS calibration
    */
    // calibration
    Cal_SenECS();

    // show DNP & save debug message
    if (CAL_ITEM()->fpGetDebugLevel(CALDEBUG_OSD_WAITKEY)!=1) // fail to end
    {
        CALRst = getDNPRst();
        if(!CALRst) // ECS fail
        {
            // save debug data
            if (CAL_ITEM()->fpGetDebugLevel(CALDEBUG_INFO_USER_A)==1)
            {
                sprintf(m_CalSaveMsgName, "A:\\%s_C_NVT.txt", Read_WifiMAC);
                sprintf(m_CalSaveMsg, "ISO_NG_FF_FF_FF_F,AWB_NG_FF_FF_FF_FF_F,DPD_NG_FF_FF_FF_F,DPB_NG_FF_FF_FF_F");
                ER_Code = CAL_ITEM()->fpCAL_SaveMsg(&m_CalSaveMsgName[0],&m_CalSaveMsg[0], FALSE);
            }

            sprintf((char*)m_CalMsg, "ECS FAIL");
            CAL_UICB()->AWB_UI(UICB_END,(CHAR*)m_CalMsg, UICOL_RED*10 + UICOL_WHITE);
            CAL_UICB()->AWB_UI(UICB_PROCESSING, NULL, UICOL_NONE); // wait any key to continue
            goto finish;
        }
        else
        {
            showDNP();
        }
    }
    else
    {
        showDNP();
    }


    /*
        ISO calibration
    */
    // calibration
    Cal_SenISO();

    // show DNP & save debug message
    if (CAL_ITEM()->fpGetDebugLevel(CALDEBUG_OSD_WAITKEY)!=1) // fail to end
    {
        CALRst = getDNPRst();
        if(!CALRst) // ISO fail
        {
            // save debug data
            if (CAL_ITEM()->fpGetDebugLevel(CALDEBUG_INFO_USER_A)==1)
            {
                sprintf(m_CalSaveMsgName, "A:\\%s_C_NVT.txt", Read_WifiMAC);
                sprintf(m_CalSaveMsg, "AWB_NG_FF_FF_FF_FF_F,DPD_NG_FF_FF_FF_F,DPB_NG_FF_FF_FF_F");
                ER_Code = CAL_ITEM()->fpCAL_SaveMsg(&m_CalSaveMsgName[0],&m_CalSaveMsg[0], FALSE);
            }

            sprintf((char*)m_CalMsg, "ISO FAIL");
            CAL_UICB()->AWB_UI(UICB_END,(CHAR*)m_CalMsg, UICOL_RED*10 + UICOL_WHITE);
            CAL_UICB()->AWB_UI(UICB_PROCESSING, NULL, UICOL_NONE); // wait any key to continue
            goto finish;
        }
        else
        {
            showDNP();
        }
    }
    else
    {
        showDNP();
    }

    /*
        AWB calibration
    */
    // calibration
    Cal_SenAWB();

    // show DNP & save debug message
    if (CAL_ITEM()->fpGetDebugLevel(CALDEBUG_OSD_WAITKEY)!=1) // fail to end
    {
        CALRst = getDNPRst();
        if(!CALRst) // AWB fail
        {
            // save debug data
            if (CAL_ITEM()->fpGetDebugLevel(CALDEBUG_INFO_USER_A)==1)
            {
                sprintf(m_CalSaveMsgName, "A:\\%s_C_NVT.txt", Read_WifiMAC);
                sprintf(m_CalSaveMsg, "DPD_NG_FF_FF_FF_F,DPB_NG_FF_FF_FF_F");
                ER_Code = CAL_ITEM()->fpCAL_SaveMsg(&m_CalSaveMsgName[0],&m_CalSaveMsg[0], FALSE);
            }

            sprintf((char*)m_CalMsg, "AWB FAIL");
            CAL_UICB()->AWB_UI(UICB_END,(CHAR*)m_CalMsg, UICOL_RED*10 + UICOL_WHITE);
            CAL_UICB()->AWB_UI(UICB_PROCESSING, NULL, UICOL_NONE); // wait any key to continue
            goto finish;
        }
        else
        {
            showDNP();
        }
    }
    else
    {
        showDNP();
    }

    /*
        DP dark calibration
    */
    // calibration
    Cal_SenDP_Dark();

    // show DNP & save debug message
    if (CAL_ITEM()->fpGetDebugLevel(CALDEBUG_OSD_WAITKEY)!=1) // fail to end
    {
        CALRst = getDNPRst();
        if(!CALRst) // DPD fail
        {
            // save debug data
            if (CAL_ITEM()->fpGetDebugLevel(CALDEBUG_INFO_USER_A)==1)
            {
                sprintf(m_CalSaveMsgName, "A:\\%s_C_NVT.txt", Read_WifiMAC);
                sprintf(m_CalSaveMsg, "DPB_NG_FF_FF_FF_F");
                ER_Code = CAL_ITEM()->fpCAL_SaveMsg(&m_CalSaveMsgName[0],&m_CalSaveMsg[0], FALSE);
            }

            sprintf((char*)m_CalMsg, "DP(DARK) FAIL");
            CAL_UICB()->AWB_UI(UICB_END,(CHAR*)m_CalMsg, UICOL_RED*10 + UICOL_WHITE);
            CAL_UICB()->AWB_UI(UICB_PROCESSING, NULL, UICOL_NONE); // wait any key to continue
            goto finish;
        }
        else
        {
            showDNP();
        }
    }
    else
    {
        showDNP();
    }

    /*
        set to dark
    */
    /* LCD show OK and green*/
    if (CAL_ITEM()->fpGetDebugLevel(CALDEBUG_OSD_WAITKEY)!=1) // fail to end
    {
        sprintf((char*)m_CalMsg, "OK");
        CAL_UICB()->AWB_UI(UICB_END,(CHAR*)m_CalMsg, UICOL_GREEN*10 + UICOL_WHITE);
    }
    else
    {
        /* LCD show OK and green*/
        sprintf((char*)m_CalMsg, "debug");
        CAL_UICB()->AWB_UI(UICB_END,(CHAR*)m_CalMsg, UICOL_GRAY*10 + UICOL_WHITE);
    }
    CAL_UICB()->AWB_UI(UICB_PROCESSING, NULL, UICOL_NONE);// wait anykey
    CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_BLACK*10);// clear background
    showDNP();

    /*
        DP bright calibration
    */
    // calibration
    Cal_SenDP_Bright();

    // show DNP & save debug message
    if (CAL_ITEM()->fpGetDebugLevel(CALDEBUG_OSD_WAITKEY)!=1) // fail to end
    {
        CALRst = getDNPRst();
        if(!CALRst) // DPB fail
        {
            sprintf((char*)m_CalMsg, "DP(BRIGHT) FAIL");
            CAL_UICB()->AWB_UI(UICB_END,(CHAR*)m_CalMsg, UICOL_RED*10 + UICOL_WHITE);
            CAL_UICB()->AWB_UI(UICB_PROCESSING, NULL, UICOL_NONE); // wait any key to continue
            goto finish;
        }
        else
        {
            //showDNP();
        }
    }
    else
    {
        showDNP();
    }

    if (CAL_ITEM()->fpGetDebugLevel(CALDEBUG_OSD_WAITKEY)!=1) // fail to end
    {
        /* LCD show OK and green*/
        sprintf((char*)m_CalMsg, "OK(ALL)");
        CAL_UICB()->AWB_UI(UICB_END,(CHAR*)m_CalMsg, UICOL_GREEN*10 + UICOL_WHITE);
        goto Exit;
    }
    else // debug mode
    {
        sprintf((char*)m_CalMsg, "debug (ALL)");
        CAL_UICB()->AWB_UI(UICB_END,(CHAR*)m_CalMsg, UICOL_GRAY*10 + UICOL_WHITE);
        CAL_UICB()->AWB_UI(UICB_PROCESSING, NULL, UICOL_NONE); // wait any key to continue
    }


    finish:
        showDNP();
        sprintf((char*)m_CalMsg, "RESULT");
        CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg, UICOL_WHITE);
    Exit:

    if (CAL_ITEM()->fpGetDebugLevel(CALDEBUG_INFO_USER_A)==1)
    {
        sprintf(m_CalSaveMsgName, "A:\\%s_C_NVT.txt", Read_WifiMAC);
        sprintf(m_CalSaveMsg, ";\r\n");
        ER_Code = CAL_ITEM()->fpCAL_SaveMsg(&m_CalSaveMsgName[0],&m_CalSaveMsg[0], FALSE);
    }

    // poweroff:
    // wait any key to power off
    CAL_UICB()->AWB_UI(UICB_PROCESSING, NULL, UICOL_NONE); // wait any key to continue
    System_PowerOff(0);
}

//#NT#2016/05/17#Silvia Wu -begin
//#NT# for burn-in test

void Cal_SenDNPBurnIn(void)
{
    // need to cancel power off/wait any key & ae always stable
    UINT32 burnInTimes = 999;
    UINT32 burnInCnt = 1;
    for (burnInCnt = 1; burnInCnt < burnInTimes; burnInCnt++)
    {
        DBG_IND("****** burnInCnt %d ******\r\n", burnInCnt);
        Cal_SenDNP();
    }
}

/**
    for show current EV/LV/Rgain/Bgain
*/
void Cal_ShowCurEnv(void)
{
#if 1
    static UINT32 ShowCnt = 0;
    static UINT32 EV,Rgain,Ggain,Bgain = 0;
    char* Read_WifiMAC;
    extern void AWB_GetPrvGain(UINT32 *Rgain, UINT32 *Ggain, UINT32 *Bgain);
    extern void SenFP_ClearOSD(void);
    extern void SenFP_ShowOSDString(CHAR *pString, UINT16 uiX, UINT16 uiY, UINT16 uiColor);
    extern UINT32 AE_GetPrvEVValue(void);

    int iCurrMode = System_GetState(SYS_STATE_CURRMODE);

    // Get current mode
    if (iCurrMode != PRIMARY_MODE_PHOTO)
    {
        SetEngineerModeChange(TRUE);
        Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PHOTO);
        Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SIZE, 1, PHOTO_SIZE_2MHD);
        return;
    }

    // check cal data
    CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
    CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
    UINT32 SenModeCondiCnt = 0;
    char pStringdata[512];
    CAL_ITEM()->fpGetAdjustRst(NULL, CalDebugInfor_DNP);
    if (CalDebugInfor_DNP->CalDebugECS->CalStatusRst[SenModeCondiCnt] == _CAL_DEBUG_OK)
    {
        SenFP_ClearOSD();
        sprintf(pStringdata, "ECS cal data in pstore!!!!!");
        SenFP_ShowOSDString(pStringdata, 20, 60, 4);
        CAL_UICB()->AWB_UI(UICB_PROCESSING, NULL, UICOL_NONE); // wait any key to continue
        System_PowerOff(0);
    }
    if (CalDebugInfor_DNP->CalDebugISO->CalStatusRst[SenModeCondiCnt] == _CAL_DEBUG_OK)
    {
        SenFP_ClearOSD();
        sprintf(pStringdata, "ISO cal data in pstore!!!!!");
        SenFP_ShowOSDString(pStringdata, 20, 60, 4);
        CAL_UICB()->AWB_UI(UICB_PROCESSING, NULL, UICOL_NONE); // wait any key to continue
        System_PowerOff(0);
    }
    if (CalDebugInfor_DNP->CalDebugAWB->CalStatusRst[SenModeCondiCnt] == _CAL_DEBUG_OK)
    {
        SenFP_ClearOSD();
        sprintf(pStringdata, "AWB cal data in pstore!!!!!");
        SenFP_ShowOSDString(pStringdata, 20, 60, 4);
        CAL_UICB()->AWB_UI(UICB_PROCESSING, NULL, UICOL_NONE); // wait any key to continue
        System_PowerOff(0);
    }
    if (CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] == _CAL_DEBUG_OK)
    {
        SenFP_ClearOSD();
        sprintf(pStringdata, "DP cal data in pstore!!!!!");
        SenFP_ShowOSDString(pStringdata, 20, 60, 4);
        CAL_UICB()->AWB_UI(UICB_PROCESSING, NULL, UICOL_NONE); // wait any key to continue
        System_PowerOff(0);
    }


    while(1)
    {
        if(GxKey_GetData(GXKEY_NORMAL_KEY) & 0x80000000)
        {
            Read_WifiMAC = EngineerMode_ReadWiFiMAC();

            sprintf(m_CalSaveMsgName, "A:\\CurEV_NVT_%s.txt", Read_WifiMAC);
            sprintf(m_CalSaveMsg, "AWB %d %d Lum %d\r\n", Rgain, Bgain, EV);
            CAL_ITEM()->fpCAL_SaveMsg(&m_CalSaveMsgName[0],&m_CalSaveMsg[0], FALSE);
            System_PowerOff(0);
            break;
        }
        else
        {
            sie_waitVD(10);
            //if ( (ShowCnt%1000) == 0 )
            {
                char pString[512];
                static UINT32 lastEV, lastRgain, lastBgain = 0;

                AWB_GetPrvGain(&Rgain, &Ggain, &Bgain);
                EV = AE_GetRealEVValue();

                sprintf(pString, "AWB %d %d Lum %d", Rgain, Bgain, EV);

                if ((lastEV == EV)&&(lastRgain == Rgain)&&(lastBgain == Bgain))
                {
                }
                else
                {
                    SenFP_ClearOSD();
                    SenFP_ShowOSDString(pString, 20, 60, 4);
                    Delay_DelayMs(300);
                }

                lastEV = EV;
                lastRgain = Rgain;
                lastBgain = Bgain;
            }
            ShowCnt++;
        }
    }

#endif
}


//#NT#2016/05/17#Silvia Wu -end

/**
    Focus Adjust,Cal_SenFocus
    Image Test,Cal_ImageTest
*/
UINT32 VAResult[5][256] = {0};
UINT32 VA5Point[5] = {0};
UINT32 VA5MaxPoint[5] = {0};
UINT16 CaRGB[3][1024];
UINT32 CurISO;
extern UINT32 uiRGain[], uiGGain[], uiBGain[];
extern void Cal_ShowMultiString(UINT32 Idx, CHAR *pStr, PURECT pRect, BOOL bClear, BOOL bShowRect);
extern void Input_SetKeyMask(KEY_STATUS uiMode, UINT32 uiMask);
extern UINT32 Input_GetKeyMask(KEY_STATUS uiMode);
extern NVTRET Ux_Redraw(void);
extern void IPL_DzoomGetPREInSize(UINT32 Id, UINT32 Idx, UINT32 *SizeH, UINT32 *SizeV);
extern UINT32 IPL_GetCap2PrvFlag (void);
extern void IPL_SetCap2PrvFlag (UINT32 Flag);
extern void Cal_SetClrOSD(BOOL Flag);
extern void Cal_SetDispColor(UINT32 Idx, UINT32 uiTextColor);


#define VA_WIN(x, y) (x + (y * 16))   //(0~15, 0~15)
static void Cal_getVARst(void)
{

    IPL_VA_INFOR vainfo;

    vainfo.Grp0 = &VAResult[0][0];
    vainfo.Grp1H = &VAResult[1][0];
    vainfo.Grp1V = &VAResult[2][0];
    vainfo.Grp2H = &VAResult[3][0];
    vainfo.Grp2V = &VAResult[4][0];
    vainfo.Length = 256;
    vainfo.VAoutputFmt = GRP0_1_2;
    vainfo.VAWin = VA_WIN_16X16;

    ipe_waitFrameEnd(1);

    IPL_GetCmd(IPL_GET_CUR_VA_INFOR, (void *)&vainfo);

    //LT
    VA5Point[0] = 0;
#if 1
    VA5Point[0] += (VAResult[1][VA_WIN(2, 2)] + VAResult[2][VA_WIN(2, 2)]);
    VA5Point[0] += (VAResult[1][VA_WIN(3, 2)] + VAResult[2][VA_WIN(3, 2)]);
    VA5Point[0] += (VAResult[1][VA_WIN(2, 3)] + VAResult[2][VA_WIN(2, 3)]);
    VA5Point[0] += (VAResult[1][VA_WIN(3, 3)] + VAResult[2][VA_WIN(3, 3)]);
#else
    VA5Point[0] += (VAResult[1][VA_WIN(1, 2)] + VAResult[2][VA_WIN(1, 2)]);
    VA5Point[0] += (VAResult[1][VA_WIN(2, 2)] + VAResult[2][VA_WIN(2, 2)]);
    VA5Point[0] += (VAResult[1][VA_WIN(1, 3)] + VAResult[2][VA_WIN(1, 3)]);
    VA5Point[0] += (VAResult[1][VA_WIN(2, 3)] + VAResult[2][VA_WIN(2, 3)]);
#endif
    //#NT#2016/05/31#Jarkko Chang -begin
    //#NT#
    //VA5Point[0] = VA5Point[0] / 4;//// / 2500;
    VA5Point[0] = VA5Point[0] / 40;//// / 1000;
    //#NT#2016/05/31#Jarkko Chang -end
    if (VA5Point[0] > VA5MaxPoint[0])
    {
        VA5MaxPoint[0] = VA5Point[0];
    }

    //RT
    VA5Point[1] = 0;
#if 1
    VA5Point[1] += (VAResult[1][VA_WIN(12, 2)] + VAResult[2][VA_WIN(12, 2)]);
    VA5Point[1] += (VAResult[1][VA_WIN(13, 2)] + VAResult[2][VA_WIN(13, 2)]);
    VA5Point[1] += (VAResult[1][VA_WIN(12, 3)] + VAResult[2][VA_WIN(12, 3)]);
    VA5Point[1] += (VAResult[1][VA_WIN(13, 3)] + VAResult[2][VA_WIN(13, 3)]);
#else
    VA5Point[1] += (VAResult[1][VA_WIN(13, 2)] + VAResult[2][VA_WIN(13, 2)]);
    VA5Point[1] += (VAResult[1][VA_WIN(14, 2)] + VAResult[2][VA_WIN(14, 2)]);
    VA5Point[1] += (VAResult[1][VA_WIN(13, 3)] + VAResult[2][VA_WIN(13, 3)]);
    VA5Point[1] += (VAResult[1][VA_WIN(14, 3)] + VAResult[2][VA_WIN(14, 3)]);
#endif
    //#NT#2016/05/31#Jarkko Chang -begin
    //#NT#
    //VA5Point[1] = VA5Point[1] / 4;//// / 1000;
    VA5Point[1] = VA5Point[1] / 40;//// / 1000;
    //#NT#2016/05/31#Jarkko Chang -end
    if (VA5Point[1] > VA5MaxPoint[1])
    {
        VA5MaxPoint[1] = VA5Point[1];
    }

    //LB
    VA5Point[2] = 0;
#if 1
    VA5Point[2] += (VAResult[1][VA_WIN(2, 12)] + VAResult[2][VA_WIN(2, 12)]);
    VA5Point[2] += (VAResult[1][VA_WIN(3, 12)] + VAResult[2][VA_WIN(3, 12)]);
    VA5Point[2] += (VAResult[1][VA_WIN(2, 13)] + VAResult[2][VA_WIN(2, 13)]);
    VA5Point[2] += (VAResult[1][VA_WIN(3, 13)] + VAResult[2][VA_WIN(3, 13)]);
#else
    VA5Point[2] += (VAResult[1][VA_WIN(1, 12)] + VAResult[2][VA_WIN(1, 12)]);
    VA5Point[2] += (VAResult[1][VA_WIN(2, 12)] + VAResult[2][VA_WIN(2, 12)]);
    VA5Point[2] += (VAResult[1][VA_WIN(1, 13)] + VAResult[2][VA_WIN(1, 13)]);
    VA5Point[2] += (VAResult[1][VA_WIN(2, 13)] + VAResult[2][VA_WIN(2, 13)]);
#endif
    //#NT#2016/05/31#Jarkko Chang -begin
    //#NT#
    //VA5Point[2] = VA5Point[2] / 4;//// / 1000;
    VA5Point[2] = VA5Point[2] / 40;//// / 1000;
    //#NT#2016/05/31#Jarkko Chang -end
    if (VA5Point[2] > VA5MaxPoint[2])
    {
        VA5MaxPoint[2] = VA5Point[2];
    }

    //RB
    VA5Point[3] = 0;
#if 1
    VA5Point[3] += (VAResult[1][VA_WIN(12, 12)] + VAResult[2][VA_WIN(12, 12)]);
    VA5Point[3] += (VAResult[1][VA_WIN(13, 12)] + VAResult[2][VA_WIN(13, 12)]);
    VA5Point[3] += (VAResult[1][VA_WIN(12, 13)] + VAResult[2][VA_WIN(12, 13)]);
    VA5Point[3] += (VAResult[1][VA_WIN(13, 13)] + VAResult[2][VA_WIN(13, 13)]);
#else
    VA5Point[3] += (VAResult[1][VA_WIN(13, 12)] + VAResult[2][VA_WIN(13, 12)]);
    VA5Point[3] += (VAResult[1][VA_WIN(14, 12)] + VAResult[2][VA_WIN(14, 12)]);
    VA5Point[3] += (VAResult[1][VA_WIN(13, 13)] + VAResult[2][VA_WIN(13, 13)]);
    VA5Point[3] += (VAResult[1][VA_WIN(14, 13)] + VAResult[2][VA_WIN(14, 13)]);
#endif
    //#NT#2016/05/31#Jarkko Chang -begin
    //#NT#
    //VA5Point[3] = VA5Point[3] / 4;//// / 1000;
    VA5Point[3] = VA5Point[3] / 40;//// / 1000;
    //#NT#2016/05/31#Jarkko Chang -end
    if (VA5Point[3] > VA5MaxPoint[3])
    {
        VA5MaxPoint[3] = VA5Point[3];
    }


    //CEN
    VA5Point[4] = 0;
    VA5Point[4] += (VAResult[1][VA_WIN(7, 7)] + VAResult[2][VA_WIN(7, 7)]);
    VA5Point[4] += (VAResult[1][VA_WIN(8, 7)] + VAResult[2][VA_WIN(8, 7)]);
    VA5Point[4] += (VAResult[1][VA_WIN(7, 8)] + VAResult[2][VA_WIN(7, 8)]);
    VA5Point[4] += (VAResult[1][VA_WIN(8, 8)] + VAResult[2][VA_WIN(8, 8)]);
    //#NT#2016/05/31#Jarkko Chang -begin
    //#NT#
    //VA5Point[4] = VA5Point[4] / 4;//// / 1000;
    VA5Point[4] = VA5Point[4] / 40;///// / 1000;
    //#NT#2016/05/31#Jarkko Chang -end
    if (VA5Point[4] > VA5MaxPoint[4])
    {
        VA5MaxPoint[4] = VA5Point[4];
    }
}


#define VA_X_STEP   (320 / 16)
#define VA_Y_STEP   (240 / 16)
#define VA_X_POS(x) (VA_X_STEP * x)
#define VA_Y_POS(y) (VA_Y_STEP * y)
#define VA_WIDTH    (VA_X_STEP * 2)
#define VA_HEIGHT   (VA_Y_STEP * 2)
UINT32 FocusAdjust_Info[7];

static INT32 fs_getline(CHAR **lineptr, UINT32 offset ,UINT32 filesize)
{
    char   *ptr = *lineptr;
    INT32  templen=0;

    if (offset >= filesize)
        return -1;
    while (offset+templen < filesize)
    {
        //DBG_MSG("0x%x ",*ptr);
        if (*ptr == '\n' || *ptr == 0)
        {
            //DBG_MSG("0x%x ",*ptr);
            return templen+1;
        }
        ptr++;
        templen++;
    }
    //DBG_MSG("\r\n");
    return templen;
}

UINT32 Temp_VAMaxValue[5] =
{
    400,
    400,
    400,
    400,
    400,
};
UINT32 CornerDiff = 0;

static BOOL CAL_Focus_ReadCALConfig(UINT32* ConfigLU, UINT32* ConfigRU, UINT32* ConfigLD, UINT32* ConfigRD, UINT32* ConfigCenter, UINT32* Diff)
{
    char        path[20]= "A:\\Focus Adjust.CAL";
    FST_FILE    filehdl;
    CHAR        tempbuf[200];
    UINT32      Size = sizeof(tempbuf);
    UINT32      fileSize;
    char        *line = NULL;
    UINT32      len = 0;
    INT32       read, offset;
    char        keyTempstr[30];



    filehdl = FileSys_OpenFile(path,FST_OPEN_READ);
    if(NULL == filehdl)
    {
        DBG_ERR("open %s fail\r\n",path);
        return FALSE;
    }
    fileSize = FileSys_GetFileLen(path);
    if (fileSize == 0)
    {
        FileSys_CloseFile(filehdl);
        return FALSE;
    }
    FileSys_SeekFile(filehdl, 0, FST_SEEK_SET);
    FileSys_ReadFile(filehdl, (UINT8*)tempbuf, &Size, 0, NULL);

    line = tempbuf;
    offset = 0;
    while ((read = fs_getline(&line, offset ,fileSize)) != -1)
    {
        if (line[read - 1] == '\n')
        {
            line[read - 1] = 0;
            read-=1;
        }

        len = snprintf(keyTempstr, sizeof(keyTempstr),"LU=");
        if (strncmp(keyTempstr,line,len) ==0)
        {
            UINT32 LU_thr = 0;
            sscanf(line+len,"%d",&LU_thr);
            DBG_IND("LU=%d\r\n", LU_thr);
            *ConfigLU = LU_thr;
            line+=read;
            offset+=read;
            continue;
        }

        len = snprintf(keyTempstr, sizeof(keyTempstr),"RU=");
        if (strncmp(keyTempstr,line,len) ==0)
        {
            UINT32 RU_thr = 0;
            sscanf(line+len,"%d",&RU_thr);
            DBG_IND("RU=%d\r\n", RU_thr);
            *ConfigRU = RU_thr;
            line+=read;
            offset+=read;
            continue;
        }

        len = snprintf(keyTempstr, sizeof(keyTempstr),"LD=");
        if (strncmp(keyTempstr,line,len) ==0)
        {
            UINT32 LD_thr = 0;
            sscanf(line+len,"%d",&LD_thr);
            DBG_IND("LD=%d\r\n", LD_thr);
            *ConfigLD = LD_thr;
            line+=read;
            offset+=read;
            continue;
        }

        len = snprintf(keyTempstr, sizeof(keyTempstr),"RD=");
        if (strncmp(keyTempstr,line,len) ==0)
        {
            UINT32 RD_thr = 0;
            sscanf(line+len,"%d",&RD_thr);
            DBG_IND("RD=%d\r\n", RD_thr);
            *ConfigRD = RD_thr;
            line+=read;
            offset+=read;
            continue;
        }

        len = snprintf(keyTempstr, sizeof(keyTempstr),"Center=");
        if (strncmp(keyTempstr,line,len) ==0)
        {
            UINT32 Center_thr = 0;
            sscanf(line+len,"%d",&Center_thr);
            DBG_IND("Center=%d\r\n", Center_thr);
            *ConfigCenter = Center_thr;
            line+=read;
            offset+=read;
            continue;
        }

        len = snprintf(keyTempstr, sizeof(keyTempstr),"Diff=");
        if (strncmp(keyTempstr,line,len) ==0)
        {
            UINT32 Diff_thr = 0;
            sscanf(line+len,"%d",&Diff_thr);
            DBG_IND("Diff=%d\r\n", Diff_thr);
            *Diff = Diff_thr;
            line+=read;
            offset+=read;
            continue;
        }

        line+=read;
        offset+=read;
    }
    FileSys_CloseFile(filehdl);
    return TRUE;
}

void Cal_SenShowVA(void)
{
    UINT32 i;
    CHAR     StringBuf[30];
    BOOL CAL_Config = TRUE;
    static BOOL Init_CAL_Config = FALSE;
    UINT32 Corner_thr, CornerMax = 0, CornerMin = 0xffffffff;

    static UINT32 VAMaxValue[5] =
    {
        400,//200,
        400,//200,
        400,//200,
        400,//200,
        400,//200,
    };

    if (Init_CAL_Config == FALSE)
    {
        CAL_Config = CAL_Focus_ReadCALConfig(&Temp_VAMaxValue[0], &Temp_VAMaxValue[1], &Temp_VAMaxValue[2], &Temp_VAMaxValue[3], &Temp_VAMaxValue[4], &CornerDiff);
        if (CAL_Config)
        {
            VAMaxValue[0] = Temp_VAMaxValue[0];
            VAMaxValue[1] = Temp_VAMaxValue[1];
            VAMaxValue[2] = Temp_VAMaxValue[2];
            VAMaxValue[3] = Temp_VAMaxValue[3];
            VAMaxValue[4] = Temp_VAMaxValue[4];
            //DBG_ERR("%d %d %d %d %d \r\n", VAMaxValue[0], VAMaxValue[1], VAMaxValue[2], VAMaxValue[3], VAMaxValue[4]);
            Init_CAL_Config = TRUE;
        }
    }

    URECT    tempRECT[12] =
    {
        //{20,60,200,25},
        //{20,90,200,25},
        //{20,120,200,25},
        //{20,150,200,25},
        //{20,180,200,25}
#if (_SENSORLIB_ == _SENSORLIB_CMOS_F22P_)
        {VA_X_POS(2),  VA_Y_POS(2),  VA_WIDTH, VA_HEIGHT},  //LT
        {VA_X_POS(12), VA_Y_POS(2),  VA_WIDTH, VA_HEIGHT},  //RT
        {VA_X_POS(2),  VA_Y_POS(12), VA_WIDTH, VA_HEIGHT},  //LB
        {VA_X_POS(12), VA_Y_POS(12), VA_WIDTH, VA_HEIGHT},  //RB
        {VA_X_POS(7),  VA_Y_POS(7),  VA_WIDTH, VA_HEIGHT},  //C

        {VA_X_POS(2),  VA_Y_POS(4),  VA_WIDTH, VA_HEIGHT},  //LT Max
        {VA_X_POS(12), VA_Y_POS(4),  VA_WIDTH, VA_HEIGHT},  //RT Max
        {VA_X_POS(2),  VA_Y_POS(14), VA_WIDTH, VA_HEIGHT},  //LB Max
        {VA_X_POS(12), VA_Y_POS(14), VA_WIDTH, VA_HEIGHT},  //RB Max
        {VA_X_POS(7),  VA_Y_POS(9),  VA_WIDTH, VA_HEIGHT},  //C  Max
#else   //AR0330M
        {VA_X_POS(1),  VA_Y_POS(2),  VA_WIDTH, VA_HEIGHT},  //LT
        {VA_X_POS(13), VA_Y_POS(2),  VA_WIDTH, VA_HEIGHT},  //RT
        {VA_X_POS(1),  VA_Y_POS(12), VA_WIDTH, VA_HEIGHT},  //LB
        {VA_X_POS(13), VA_Y_POS(12), VA_WIDTH, VA_HEIGHT},  //RB
        {VA_X_POS(7),  VA_Y_POS(7),  VA_WIDTH, VA_HEIGHT},  //C

        {VA_X_POS(1),  VA_Y_POS(4),  VA_WIDTH, VA_HEIGHT},  //LT Max
        {VA_X_POS(13), VA_Y_POS(4),  VA_WIDTH, VA_HEIGHT},  //RT Max
        {VA_X_POS(1),  VA_Y_POS(14), VA_WIDTH, VA_HEIGHT},  //LB Max
        {VA_X_POS(13), VA_Y_POS(14), VA_WIDTH, VA_HEIGHT},  //RB Max
        {VA_X_POS(7),  VA_Y_POS(9),  VA_WIDTH, VA_HEIGHT},  //C  Max
#endif
        {VA_X_POS(6),  VA_Y_POS(0),  VA_WIDTH, VA_HEIGHT},  //ISO

        {VA_X_POS(7),  VA_Y_POS(14),  VA_WIDTH, VA_HEIGHT},  //difference
    };

    //Cal_ShowString(StringBuf, &tempRECT, TRUE);
    IPC_WaitVD(FALSE, IPL_ID_1);
    Cal_getVARst();

    //Current value
    for (i = 0; i < 5; i ++)
    {
        if (i != 4) // 4 is center va value
        {
            if (VA5Point[i] >= CornerMax)
                CornerMax = VA5Point[i];
            if ((VA5Point[i] <= VAMaxValue[i])&&(VA5Point[i] <= CornerMin))
                CornerMin = VA5Point[i];
        }

        sprintf(StringBuf,"%d", VA5Point[i]);
        if (VAMaxValue[i] > VA5Point[i])
        {
            Cal_SetDispColor(i, CLRID_IDX_RED);
        }
        else
        {
            Cal_SetDispColor(i, CLRID_IDX_GREEN);
        }
        //FOCUS_UI(StringBuf, &tempRECT[i], TRUE);
        Cal_ShowMultiString(i, StringBuf, &tempRECT[i], TRUE, TRUE);
        FocusAdjust_Info[i] = VA5Point[i];
        //sprintf(StringBuf,"");
        //Cal_SetDispColor(i, CLRID_IDX_WHITE);
        //Cal_ShowMultiString(i, StringBuf, &tempRECT[i], FALSE, TRUE);
    }

    //MAX value
    for (i = 0; i < 5; i ++)
    {
        sprintf(StringBuf,"%d", VA5MaxPoint[i]);
/*
        if (VAMaxValue[i] > VA5MaxPoint[i])
        {
            Cal_SetDispColor((i + 5), CLRID_IDX_RED);
        }
        else
        {
            Cal_SetDispColor((i + 5), CLRID_IDX_GREEN);
        }
*/
        Cal_SetDispColor((i + 5), CLRID_IDX_WHITE);
        Cal_ShowMultiString((i + 5), StringBuf, &tempRECT[i + 5], TRUE, FALSE);
    }

    sprintf(StringBuf,"ISO:%d", CurISO);
    Cal_SetDispColor(10, CLRID_IDX_WHITE);
    Cal_ShowMultiString(10, StringBuf, &tempRECT[10], TRUE, FALSE);
    FocusAdjust_Info[5] = CurISO;

    // corner VA difference check
    Corner_thr = CornerMax - CornerMin;
    if ((CornerMin != 0xffffffff)&&(Corner_thr > CornerDiff))
        Cal_SetDispColor(11, CLRID_IDX_RED);
    else
       Cal_SetDispColor(11, CLRID_IDX_GREEN);

    sprintf(StringBuf,"%d", Corner_thr);
    Cal_ShowMultiString(11, StringBuf, &tempRECT[11], TRUE, FALSE);

    FocusAdjust_Info[6] = CornerDiff;
}


void Cal_SenFocus(void)
{

    int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
    BOOL b_CardOK = TRUE;

    // Get current mode
    if (iCurrMode != PRIMARY_MODE_PHOTO)
    {
        SetEngineerModeChange(TRUE);
        Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PHOTO);
        Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SIZE, 1, PHOTO_SIZE_2MHD);
        return;
    }

    UINT32 Freq;
    UINT32 keyMask;
    IPL_SET_VA_DATA vainfo;
    IPL_DZOOM_INFOR dzoominfo;
    URECT rect;
    UINT32 tmp;

    EngineerMode_TurnOnWiFi(); // test, turn on WiFi

    REDO_AFADJUST:
    keyMask = Input_GetKeyMask(KEY_PRESS);
    Input_SetKeyMask(KEY_PRESS, 0);
    Input_SetKeyMask(KEY_RELEASE, 0);

    Cal_SetClrOSD(TRUE);
    rect.x = 0;
    rect.y = 0;
    rect.w = 240;
    rect.h = 25;
    for (tmp = 0; tmp < 12; tmp ++)
    {
        Cal_SetDispColor(tmp, CLRID_IDX_WHITE);
        Cal_ShowMultiString(tmp, "", &rect, TRUE, FALSE);
    }

    Cal_ShowMultiString(0, "alignment AF chart....press any key", &rect, TRUE, FALSE);
    Ux_Redraw();
    Cal_SetClrOSD(FALSE);
    while(1)
    {
        if(GxKey_GetData(GXKEY_NORMAL_KEY) & 0xffffffff)
        {
            break;
        }
    }

    dzoominfo.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_CUR_DZ_INFOR, &dzoominfo);

    vainfo.Id = IPL_ID_1;

    IPL_DzoomGetPREInSize(IPL_ID_1, dzoominfo.CurIdx, &(vainfo.VAWin16X16.szh), &(vainfo.VAWin16X16.szv));

    vainfo.VAWin16X16.szv = vainfo.VAWin16X16.szv - 4;
    //DBGD(vainfo.VAWin16X16.szh);
    //DBGD(vainfo.VAWin16X16.szv);
    vainfo.VAWin16X16.stx = 0;
    vainfo.VAWin16X16.sty = 0;
    vainfo.VAWin16X16.szh = (vainfo.VAWin16X16.szh >> 4);
    vainfo.VAWin16X16.szv = (((vainfo.VAWin16X16.szv >> 4) / 4) * 4);
    vainfo.VAWin16X16.vanf = 7; //1;
    //DBGD(vainfo.VAWin16X16.szh);
    //DBGD(vainfo.VAWin16X16.szv);

    vainfo.VAOutEn = ENABLE;
    vainfo.VAWinInd.Win0En = DISABLE;
    vainfo.VAWinInd.Win1En = DISABLE;
    vainfo.VAWinInd.Win2En = DISABLE;

    vainfo.VASettings.Vacc.ManualUpdateEn = 0;  //auto update
    vainfo.VASettings.Vacc.VAOutSel = GRP0_1_2;
    vainfo.VASettings.Vacc.G0SRC = EdgeKer_5x5_OUT;
    vainfo.VASettings.Vacc.GRP0.High = 32;
    vainfo.VASettings.Vacc.GRP0.Low = 16;
    vainfo.VASettings.Vacc.GRP1.ThresH.High = 32;
    vainfo.VASettings.Vacc.GRP1.ThresH.Low = 16;
    vainfo.VASettings.Vacc.GRP1.ThresV.High = 32;
    vainfo.VASettings.Vacc.GRP1.ThresV.Low = 16;
    vainfo.VASettings.Vacc.GRP2.ThresH.High = 32;
    vainfo.VASettings.Vacc.GRP2.ThresH.Low = 16;
    vainfo.VASettings.Vacc.GRP2.ThresV.High = 32;
    vainfo.VASettings.Vacc.GRP2.ThresV.Low = 16;

    vainfo.VASettings.Vdet.VdetSel = E_POS_GAMMA;

    vainfo.VASettings.Vdet.GRP1_H.VdFilt = Filtering_5;
    vainfo.VASettings.Vdet.GRP1_H.Coef.A = 2;
    vainfo.VASettings.Vdet.GRP1_H.Coef.B = -1;
    vainfo.VASettings.Vdet.GRP1_H.Coef.C = 0;
    vainfo.VASettings.Vdet.GRP1_H.Coef.D = 0;
    vainfo.VASettings.Vdet.GRP1_H.Div = 1;

    vainfo.VASettings.Vdet.GRP1_V.VdFilt = Filtering_5;
    vainfo.VASettings.Vdet.GRP1_V.Coef.A = 2;
    vainfo.VASettings.Vdet.GRP1_V.Coef.B = -1;
    vainfo.VASettings.Vdet.GRP1_V.Coef.C = 0;
    vainfo.VASettings.Vdet.GRP1_V.Coef.D = 0;
    vainfo.VASettings.Vdet.GRP1_V.Div = 1;

    vainfo.VASettings.Vdet.GRP2_H.VdFilt = Filtering_5;
    vainfo.VASettings.Vdet.GRP2_H.Coef.A = 2;
    vainfo.VASettings.Vdet.GRP2_H.Coef.B = 0;
    vainfo.VASettings.Vdet.GRP2_H.Coef.C = -1;
    vainfo.VASettings.Vdet.GRP2_H.Coef.D = 0;
    vainfo.VASettings.Vdet.GRP2_H.Div = 1;

    vainfo.VASettings.Vdet.GRP2_V.VdFilt = Filtering_5;
    vainfo.VASettings.Vdet.GRP2_V.Coef.A = 2;
    vainfo.VASettings.Vdet.GRP2_V.Coef.B = 0;
    vainfo.VASettings.Vdet.GRP2_V.Coef.C = -1;
    vainfo.VASettings.Vdet.GRP2_V.Coef.D = 0;
    vainfo.VASettings.Vdet.GRP2_V.Div = 1;

    //wait AE stable
    rect.x = 0;
    rect.y = 30;
    rect.w = 240;
    rect.h = 25;
    Cal_SetDispColor(0, CLRID_IDX_GREEN);
    Cal_ShowMultiString(0, "wait ae stable....", &rect, TRUE, FALSE);
    Ux_Redraw();
    Freq = IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY);
    IPL_AlgSetUIInfo(IPL_SEL_FREQUENCY, SEL_FREQUENCY_50HZ);

    if (AE_Wait_Stable(10, (20 * 6)) == TRUE)
    {
        rect.x = 0;
        rect.y = 30;
        rect.w = 240;
        rect.h = 25;
        Cal_SetDispColor(0, CLRID_IDX_GREEN);
        Cal_ShowMultiString(0, "wait ae stable....PASS", &rect, TRUE, FALSE);
        Ux_Redraw();
    }
    else
    {
        rect.x = 0;
        rect.y = 30;
        rect.w = 240;
        rect.h = 25;
        Cal_SetDispColor(0, CLRID_IDX_RED);
        Cal_ShowMultiString(0, "wait ae stable....FAIL", &rect, TRUE, FALSE);
        Ux_Redraw();
        while(1)
        {
            if(GxKey_GetData(GXKEY_NORMAL_KEY) & 0xffffffff)
            {
                break;
            }
        }
        goto REDO_AFADJUST;
    }
    AE_Lock(AE_ID_1, TRUE, TRUE);
    AE_GetPrvAEArg(&CurISO, &tmp, &tmp);


    //wait AWB stable
    rect.x = 0;
    rect.y = 60;
    rect.w = 240;
    rect.h = 25;
    Cal_SetDispColor(0, CLRID_IDX_GREEN);
    Cal_ShowMultiString(0, "wait awb stable....", &rect, TRUE, FALSE);
    Ux_Redraw();

    UINT32 j;
    for (j = 0; j < 15; j ++)
    {
        IPL_CA_INFOR CAInfo;
        UINT32 i, SumRGB[3];

        CAInfo.Id = IPL_ID_1;
        CAInfo.R = &CaRGB[0][0];
        CAInfo.G = &CaRGB[1][0];
        CAInfo.B = &CaRGB[2][0];
        CAInfo.Length = 1024;
        IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CAInfo);

        SumRGB[0] = 0;
        SumRGB[1] = 0;
        SumRGB[2] = 0;

        for (i = 0; i < 1024; i ++)
        {
            SumRGB[0] += CaRGB[0][i];
            SumRGB[1] += CaRGB[1][i];
            SumRGB[2] += CaRGB[2][i];
        }

        SumRGB[0] = SumRGB[0] / 1024;
        SumRGB[1] = SumRGB[1] / 1024;
        SumRGB[2] = SumRGB[2] / 1024;

        if (SumRGB[0] == 0) SumRGB[0] = 1;
        if (SumRGB[1] == 0) SumRGB[1] = 1;
        if (SumRGB[2] == 0) SumRGB[2] = 1;

        loc_cpu();
        uiRGain[AWB_ID_1] = (SumRGB[1] * 256) / SumRGB[0];
        uiGGain[AWB_ID_1] = (SumRGB[1] * 256) / SumRGB[1];
        uiBGain[AWB_ID_1] = (SumRGB[1] * 256) / SumRGB[2];
        unl_cpu();
        IPC_WaitVD(FALSE, IPL_ID_1);
    }
    AWB_Lock(AWB_ID_1, TRUE, TRUE);

    rect.x = 0;
    rect.y = 60;
    rect.w = 240;
    rect.h = 25;
    Cal_SetDispColor(0, CLRID_IDX_GREEN);
    Cal_ShowMultiString(0, "wait awb stable....PASS", &rect, TRUE, FALSE);
    Ux_Redraw();
    Delay_DelayMs(500);

    Cal_SetClrOSD(TRUE);

    VA5MaxPoint[0] = 0;
    VA5MaxPoint[1] = 0;
    VA5MaxPoint[2] = 0;
    VA5MaxPoint[3] = 0;
    VA5MaxPoint[4] = 0;

    char* Read_WifiMAC;

    EngineerMode_SaveWiFiMAC(); // test, get and save MAC address
    Read_WifiMAC = EngineerMode_ReadWiFiMAC(); // test, read MAC address
    //DBG_WRN("%s\r\n", Read_WifiMAC);

    IPL_SetCmd(IPL_SET_VA, &vainfo);
    IPL_WaitCmdFinish();
    //IPL_AlgSetUIInfo(IPL_SEL_FLASHMODE, SEL_FLASHMODE_FORCEOFF);
    //ImgCap_SetUIInfo(CAP_SEL_IMG_RATIO, IPL_SENCROPRATIO_16_9);
    //ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_NONE);
    //ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_H_SIZE, 1920);
    //ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_V_SIZE, 1080);
    //IPL_SetCap2PrvFlag(FALSE);
    while(1)
    {
        //if (IPL_GetCap2PrvFlag() == TRUE)
        //{
        //    IPL_SetCap2PrvFlag(FALSE);
        //    Delay_DelayMs(50);
        //    IPL_SetCmd(IPL_SET_VA, &vainfo);
        //    IPL_WaitCmdFinish();
        //}
        //else
        {
            Delay_DelayMs(100);

            Cal_SenShowVA();
            Ux_Redraw();
        }

        if(GxKey_GetData(GXKEY_NORMAL_KEY) & 0xffffffff)
        {
            break;
        }
    }
    //Delay_DelayMs(1000);
    Input_SetKeyMask(KEY_PRESS, 0xffffffff);
    Delay_DelayMs(100);

    rect.x = 0;
    rect.y = 0;
    rect.w = 320;
    rect.h = 240;
    for (tmp = 0; tmp < 12; tmp ++)
    {
        Cal_SetDispColor(tmp, CLRID_IDX_GREEN);
        Cal_ShowMultiString(tmp, "", &rect, TRUE, FALSE);
    }
    UI_ClearOSD(CLRID_IDX_GREEN);
    Cal_SetDispColor(0, CLRID_IDX_WHITE);
    rect.x = 20;
    rect.y = 120;
    rect.w = 240;
    rect.h = 25;
    CHAR MAC_ADD[32];
    sprintf(MAC_ADD, "%s", Read_WifiMAC);
    Cal_SetClrOSD(FALSE);
    Cal_ShowMultiString(0, MAC_ADD, &rect, FALSE, FALSE);
    Ux_Redraw();

    while(1)
    {
        if(GxKey_GetData(GXKEY_NORMAL_KEY) & 0xffffffff)
        {
            break;
        }
    }
    //Delay_DelayMs(1000);
    Input_SetKeyMask(KEY_PRESS, 0xffffffff);
    Delay_DelayMs(100);


    if(1)// write log file to SD card
    {
        CHAR pcFilename[32],DataInfo[100];
        UINT32 RW_OpenResult1 = {0};
        FST_FILE RW_OpenResult = &RW_OpenResult1;
        UINT32 size;
        INT32 ER_Code;
        //UINT32 lens_type = 0;

        //sprintf(pcFilename, "A:\\StationA.txt");
        sprintf(pcFilename, "A:\\%s_A.txt", Read_WifiMAC);
        RW_OpenResult = FileSys_OpenFile(pcFilename, FST_CREATE_ALWAYS|FST_OPEN_WRITE);
        if(RW_OpenResult == NULL)
        {
            DBG_ERR("Open file error!!\r\n");
            Cal_SetClrOSD(TRUE);
            rect.x = 0;
            rect.y = 0;
            rect.w = 240;
            rect.h = 25;
            for (tmp = 0; tmp < 12; tmp ++)
            {
                Cal_SetDispColor(tmp, CLRID_IDX_WHITE);
                Cal_ShowMultiString(tmp, "", &rect, TRUE, FALSE);
            }

            Cal_SetDispColor(0, CLRID_IDX_RED);
            Cal_ShowMultiString(0, "SD Card Full : STOP", &rect, TRUE, FALSE);
            Ux_Redraw();
            b_CardOK = FALSE;
        }
        else
        {
        	#if 0
            lens_type = IPL_AlgGetUIInfo(IPL_CUMSTOMER_DEF0);
            if (lens_type == SEL_LENS_L)
                sprintf(DataInfo, "Lens_L, Center_%d,RU_%d,RD_%d,LU_%d,LD_%d,ISO_%d, Diff_%d;\r\n", FocusAdjust_Info[4],FocusAdjust_Info[1],FocusAdjust_Info[3],FocusAdjust_Info[0],FocusAdjust_Info[2],FocusAdjust_Info[5],FocusAdjust_Info[6]);
            else if (lens_type == SEL_LENS_W)
                sprintf(DataInfo, "Lens_W, Center_%d,RU_%d,RD_%d,LU_%d,LD_%d,ISO_%d, Diff_%d;\r\n", FocusAdjust_Info[4],FocusAdjust_Info[1],FocusAdjust_Info[3],FocusAdjust_Info[0],FocusAdjust_Info[2],FocusAdjust_Info[5],FocusAdjust_Info[6]);
            else if (lens_type == SEL_LENS_A)
                sprintf(DataInfo, "Lens_A, Center_%d,RU_%d,RD_%d,LU_%d,LD_%d,ISO_%d, Diff_%d;\r\n", FocusAdjust_Info[4],FocusAdjust_Info[1],FocusAdjust_Info[3],FocusAdjust_Info[0],FocusAdjust_Info[2],FocusAdjust_Info[5],FocusAdjust_Info[6]);
            else
                sprintf(DataInfo, "Lens_B Center_%d,RU_%d,RD_%d,LU_%d,LD_%d,ISO_%d, Diff_%d;\r\n", FocusAdjust_Info[4],FocusAdjust_Info[1],FocusAdjust_Info[3],FocusAdjust_Info[0],FocusAdjust_Info[2],FocusAdjust_Info[5],FocusAdjust_Info[6]);
			#endif

            size = strlen(DataInfo);

            ER_Code = FileSys_WriteFile(RW_OpenResult, (UINT8*)&DataInfo, &size, 0, NULL);
            if(ER_Code != E_OK)
            {
                rect.x = 0;
                rect.y = 0;
                rect.w = 320;
                rect.h = 25;
                Cal_SetClrOSD(TRUE);
                for (tmp = 0; tmp < 12; tmp ++)
                {
                    Cal_SetDispColor(tmp, CLRID_IDX_WHITE);
                    Cal_ShowMultiString(tmp, "", &rect, TRUE, FALSE);
                }
                Cal_SetDispColor(0, CLRID_IDX_RED);
                Cal_ShowMultiString(0, "SD Card Full : STOP", &rect, TRUE, FALSE);
                Ux_Redraw();
                FileSys_CloseFile(RW_OpenResult);
                b_CardOK = FALSE;
            }
            else
                FileSys_CloseFile(RW_OpenResult);
        }
    }

    IPL_AlgSetUIInfo(IPL_SEL_FREQUENCY, Freq);
    uiRGain[AWB_ID_1] = 0;
    uiGGain[AWB_ID_1] = 0;
    uiBGain[AWB_ID_1] = 0;
    if(!b_CardOK)
    {
        while(1)
        {
            if(GxKey_GetData(GXKEY_NORMAL_KEY) & 0xffffffff)
            {
                break;
            }
        }
        Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MAIN);
    }
    else
    {
        USB_SetForceMsdcNvt(FALSE);
        Cal_ForceExitWnd();
        FlowMode_OnUsbUnplug();
        MsdcNvtCb_Close();
        Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_USBMSDC);
    }
}

void Cal_ImageTest(void)
{

    int iCurrMode = System_GetState(SYS_STATE_CURRMODE);

    // Get current mode
    if (iCurrMode != PRIMARY_MODE_PHOTO)
    {
        SetEngineerModeChange(TRUE);
        Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PHOTO);
        Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SIZE, 1, PHOTO_SIZE_2MHD);
        return;
    }

    UINT32 Freq;
    UINT32 keyMask;
    URECT rect;
    UINT32 tmp;
    UINT64 FreeSpace;

REDO_IMAGETEST:
    Freq = IPL_AlgGetUIInfo(IPL_SEL_FREQUENCY);
    IPL_AlgSetUIInfo(IPL_SEL_FREQUENCY, SEL_FREQUENCY_50HZ);

    keyMask = Input_GetKeyMask(KEY_PRESS);
    Input_SetKeyMask(KEY_PRESS, 0);
    Input_SetKeyMask(KEY_RELEASE, 0);

    Cal_SetClrOSD(TRUE);
    rect.x = 0;
    rect.y = 0;
    rect.w = 240;
    rect.h = 25;
    for (tmp = 0; tmp < 11; tmp ++)
    {
        Cal_SetDispColor(tmp, CLRID_IDX_GREEN);
        Cal_ShowMultiString(tmp, "", &rect, TRUE, FALSE);
    }

    FreeSpace = FileSys_GetDiskInfo(FST_INFO_FREE_SPACE);
    if(FreeSpace < 0x200000)
    {
        DBG_ERR("Free space=%d\r\n",FreeSpace);
        rect.x = 0;
        rect.y = 0;
        rect.w = 240;
        rect.h = 25;
        Cal_SetDispColor(0, CLRID_IDX_RED);
        Cal_ShowMultiString(0, "SD Card Full : STOP", &rect, TRUE, FALSE);
        Ux_Redraw();
        goto TEST_END;

    }

    Cal_ShowMultiString(0, "alignment test chart..press any key", &rect, TRUE, FALSE);
    Ux_Redraw();
    Cal_SetClrOSD(FALSE);
    while(1)
    {
        if(GxKey_GetData(GXKEY_NORMAL_KEY) & 0xffffffff)
        {
            break;
        }
    }

    //wait AE stable
    rect.x = 0;
    rect.y = 30;
    rect.w = 240;
    rect.h = 25;
    Cal_SetDispColor(0, CLRID_IDX_GREEN);
    Cal_ShowMultiString(0, "wait ae stable....", &rect, TRUE, FALSE);
    Ux_Redraw();

    if (AE_Wait_Stable(10, (20 * 6)) == TRUE)
    {
        rect.x = 0;
        rect.y = 30;
        rect.w = 240;
        rect.h = 25;
        Cal_SetDispColor(0, CLRID_IDX_GREEN);
        Cal_ShowMultiString(0, "wait ae stable....PASS", &rect, TRUE, FALSE);
        Ux_Redraw();
    }
    else
    {
        rect.x = 0;
        rect.y = 30;
        rect.w = 240;
        rect.h = 25;
        Cal_SetDispColor(0, CLRID_IDX_RED);
        Cal_ShowMultiString(0, "wait ae stable....FAIL", &rect, TRUE, FALSE);
        Ux_Redraw();

        while(1)
        {
            if(GxKey_GetData(GXKEY_NORMAL_KEY) & 0xffffffff)
            {
                break;
            }
        }
        goto REDO_IMAGETEST;
    }
    AE_Lock(AE_ID_1, TRUE, TRUE);
    AE_GetPrvAEArg(&CurISO, &tmp, &tmp);


    //wait AWB stable
    rect.x = 0;
    rect.y = 60;
    rect.w = 240;
    rect.h = 25;
    Cal_SetDispColor(0, CLRID_IDX_GREEN);
    Cal_ShowMultiString(0, "wait awb stable....", &rect, TRUE, FALSE);
    Ux_Redraw();

    UINT32 j;
    for (j = 0; j < 15; j ++)
    {
        IPL_CA_INFOR CAInfo;
        UINT32 i, SumRGB[3];

        CAInfo.Id = IPL_ID_1;
        CAInfo.R = &CaRGB[0][0];
        CAInfo.G = &CaRGB[1][0];
        CAInfo.B = &CaRGB[2][0];
        CAInfo.Length = 1024;
        IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CAInfo);

        SumRGB[0] = 0;
        SumRGB[1] = 0;
        SumRGB[2] = 0;

        for (i = 0; i < 1024; i ++)
        {
            SumRGB[0] += CaRGB[0][i];
            SumRGB[1] += CaRGB[1][i];
            SumRGB[2] += CaRGB[2][i];
        }

        SumRGB[0] = SumRGB[0] / 1024;
        SumRGB[1] = SumRGB[1] / 1024;
        SumRGB[2] = SumRGB[2] / 1024;

        if (SumRGB[0] == 0) SumRGB[0] = 1;
        if (SumRGB[1] == 0) SumRGB[1] = 1;
        if (SumRGB[2] == 0) SumRGB[2] = 1;

        loc_cpu();
        uiRGain[AWB_ID_1] = (SumRGB[1] * 256) / SumRGB[0];
        uiGGain[AWB_ID_1] = (SumRGB[1] * 256) / SumRGB[1];
        uiBGain[AWB_ID_1] = (SumRGB[1] * 256) / SumRGB[2];
        unl_cpu();
        IPC_WaitVD(IPL_ID_1, FALSE);
    }
    AWB_Lock(AWB_ID_1, TRUE, TRUE);

    rect.x = 0;
    rect.y = 60;
    rect.w = 240;
    rect.h = 25;
    Cal_SetDispColor(0, CLRID_IDX_GREEN);
    Cal_ShowMultiString(0, "wait awb stable....PASS", &rect, TRUE, FALSE);
    Ux_Redraw();

    IPL_AlgSetUIInfo(IPL_SEL_FLASHMODE, SEL_FLASHMODE_FORCEOFF);

    //#NT#2016/07/04#Jarkko Chang -begin
    //#NT# force disable wdr when capture
    IPL_AlgSetUIInfo(IPL_SEL_WDR, SEL_WDR_LV0);
    IPL_AlgSetUIInfo(IPL_SEL_IMAGEEFFECT, SEL_IMAGEEFFECT_OFF);
    //#NT#2016/07/04#Jarkko Chang -end

    ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_BURST);
    ImgCap_SetUIInfo(CAP_SEL_IMG_RATIO, IPL_SENCROPRATIO_16_9);
    ImgCap_SetUIInfo(CAP_SEL_PICNUM, 1);
    ImgCap_SetUIInfo(CAP_SEL_JPG_BUF_NUM, 1);
    ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 1);
    ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
    ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
    ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_H_SIZE, 1920);
    ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_V_SIZE, 1080);
    ImgCap_StartCapture();

    ImgCap_WaitFinish();

    rect.x = 0;
    rect.y = 90;
    rect.w = 240;
    rect.h = 25;
    Cal_SetDispColor(0, CLRID_IDX_GREEN);
    Cal_ShowMultiString(0, "capture JPG....PASS", &rect, TRUE, FALSE);
    Ux_Redraw();
    IPL_AlgSetUIInfo(IPL_SEL_FREQUENCY, Freq);
    uiRGain[AWB_ID_1] = 0;
    uiGGain[AWB_ID_1] = 0;
    uiBGain[AWB_ID_1] = 0;
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MAIN);
    //Delay_DelayMs(1000);

    USB_SetForceMsdcNvt(FALSE);
    Cal_ForceExitWnd();
    FlowMode_OnUsbUnplug();
    MsdcNvtCb_Close();
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_USBMSDC);
    goto PROC_END;
TEST_END:
    //rect.x = 0;
    //rect.y = 120;
    //rect.w = 240;
    //rect.h = 25;
    //Cal_SetDispColor(0, CLRID_IDX_GREEN);
    //Cal_ShowMultiString(0, "wait power key exit", &rect, TRUE, FALSE);
    //Ux_Redraw();

    while(1)
    {
        if(GxKey_GetData(GXKEY_NORMAL_KEY) & 0x80000000)
        {
            System_PowerOff(0);
            break;
        }
    }
PROC_END:
    Delay_DelayMs(100);
}

INT32 Cal_GetSensorVersion(void)
{
    INT32 rtVal = -1;
    SENSOR_CMD Cmd3072 = {0};
    SENSOR_CMD Cmd30F0 = {0};

    Cmd3072.uiAddr = 0x3072;
    if (Sensor_ReadReg(SENSOR_ID_1, &Cmd3072) != E_OK)
    {
        DBG_ERR("sensor read register 3072 fail\r\n");
        return rtVal;
    }

    if (Cmd3072.uiData[0] == 0x0006)
    {
        return 3;
    }
    else if (Cmd3072.uiData[0] == 0x0007)
    {
        return 4;
    }
    else if (Cmd3072.uiData[0] == 0x0008)
    {
        return 5;
    }
    else if (Cmd3072.uiData[0] == 0x0000)
    {
        Cmd30F0.uiAddr = 0x30F0;
        if (Sensor_ReadReg(SENSOR_ID_1, &Cmd30F0) != E_OK)
        {
            DBG_ERR("sensor read register 30F0 fail\r\n");
            return rtVal;
        }

        if (Cmd30F0.uiData[0] == 0x1200)
        {
            return 1;
        }
        else if (Cmd30F0.uiData[0] == 0x1208)
        {
            return 2;
        }
    }
    DBG_ERR("R0x3072: 0x%.8x, R0x30F0: 0x%.8x, not support\r\n", Cmd3072.uiData[0], Cmd30F0.uiData[0]);
    return rtVal;
}

//@}
