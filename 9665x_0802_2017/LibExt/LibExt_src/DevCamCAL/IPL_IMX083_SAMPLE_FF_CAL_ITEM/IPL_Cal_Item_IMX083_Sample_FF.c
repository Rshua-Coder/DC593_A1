#include "string.h"
#include "stdio.h"
#include "IPL_CAL_Debug.h"
#include "IPL_Cal_Ctrl_IMX083_Sample_FF_Int.h"
#include "Ipl_Cal_Setting_IMX083_Sample_FF_Int.h"
#include "ae_task.h"
#include "awb_task.h"
#include "ipl_alginfor.h"
#include "ipl_calibration.h"
#include "ipl_CalRW.h"
#include "ipl_cal_datapassing_IMX083_Sample_FF_int.h"
#include "ipl_display.h"
#include "IPL_Hal_Op_Int.h"
#include "IPL_Hal.h"

static CHAR m_CalMsg[100];


static ER CopyTag(UINT32 TargetAdd, CAL_TAG *pTag)
{
    UINT32 Cpysize = TAG_TypeSize + pTag->TagDataLen;
    CAL_TAG *_ptmpTag;

    memcpy((void*)TargetAdd,(const void*)pTag,Cpysize);
    _ptmpTag = (CAL_TAG *)TargetAdd;

    return E_OK;
}


/**
    add section
    return:
        E_EXS:  ID duplicated
        E_NOEXS: header or init or current ID in section table not exist
        E_BOVR::"header buffer" or "section buffer" overflow
*/
_INLINE CAL_DMA_RST CfgSectionBuf(CAL_SECMAP SecID,UINT32 DmaAddr,UINT32 DmaSize)
{
    UINT32 i= SectionNum;
    CAL_DMA_RST rst;
    while(i--)
    {
        if(Section_Size[i][0] == SecID)
        {
            rst.erCode = CAL_RW()->fpAddSection(SecID,DmaAddr,Section_Size[i][1]);
            if(rst.erCode == E_OK)
            {
                rst.RemainAddr = DmaAddr + Section_Size[i][1];
                rst.RemainSize = DmaSize - Section_Size[i][1];
            }
            return rst;
        }
    }
    rst.erCode = E_NOEXS;
    return rst;
}
/**
    return Total sensor mode which is needed to calibrate
*/
_INLINE UINT32 ChkSenModeNum(void)
{
    UINT32 i,j=SENSOR_ID_MAX_NUM;
    UINT32 SenNum = 0;

    while(j--)
    {
        i = CalSensorModeNum;
        while(i--)
        {
            if(SensorMode[i][j].SenMode != BYPASS)
            {
                SenNum++;
                break;
            }
        }
    }
    return SenNum;
}

_INLINE void AAA_SWITCH(BOOL lock,AE_PROC_ID Id)
{
    if(lock)
    {
        AE_Lock(Id,TRUE,TRUE);
        AWB_Lock(Id,TRUE,TRUE);
    }
    else
    {
        AE_Lock(Id,FALSE,TRUE);
        AWB_Lock(Id,FALSE,TRUE);
    }

}

static ER ISO_Adjust(fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    UINT32 i;
    UINT32 SensorID = SENSOR_ID_MAX_NUM;
    CAL_AE_RST aeRst;
    CTRL_PTR CtrlPtr;
    SENSOR_INFO Drv_Sensor;
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    CAL_ISO_FMT* pISOFmt;

    sprintf((char*)m_CalMsg, "Set the environment to LV8");
    if(fpUIcb)
        fpUIcb(UICB_START,(CHAR*)m_CalMsg,UICOL_GREEN);

    //stop pipeline ycc part
    CalApi()->fpSetMode(CAL_IPL_STOP,NULL,NULL);
    //pause sie
    CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);
    //allocate section buffer
    dmaRst = CfgSectionBuf(SEC_ISO,DmaAddr,DmaSize);
    if(dmaRst.erCode != E_OK)
        return E_SYS;
    else
    {
        DmaAddr = dmaRst.RemainAddr;
        DmaSize = dmaRst.RemainSize;
    }

    //shift current dram address (shift size: 1 tag size), temporal buf
    TagTempAddr = DmaAddr;
    pTag = (pCAL_TAG)TagTempAddr;
    pTag->TagDataLen = sizeof(CAL_ISO_FMT);
    TagSize = (TAG_TypeSize + pTag->TagDataLen);


    if(DmaSize >= TagSize)
    {
        DmaAddr += TagSize;
        DmaSize -= TagSize;
    }
    else
    {
        DBG_ERR("section buffer overflow\r\n");
        return E_SYS;
    }

    while(SensorID--)
    {
        i = sizeof(SensorMode_ISO)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
        while(i--)
        {
            if(SensorMode_ISO[i][SensorID].SenMode != BYPASS)
            {
                CalBuf buf;
                //lock 3a,fd...etc.
                AAA_SWITCH(TRUE,SensorID);
                //ipp buf init
                buf.Id = SensorMode_ISO[i][SensorID].SenMode;
                buf.StartAddr = DmaAddr;
                buf.Size = DmaSize;
                CalApi()->fpBufEvent()->fpInit(buf);
                //init sensor info
                CtrlPtr.pCal_Sensor = (void*)&SensorMode_ISO[i][SensorID];
                Sensor_GetStatus(SensorID, SensorMode_ISO[i][SensorID].SenMode, &Drv_Sensor);
                CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;
                //config target sensor mode
                IPC_WaitVD(TRUE,SensorID);
                Sensor_ChgMode(SensorID, SensorMode_ISO[i][SensorID].SenMode);
                IPC_WaitVD(TRUE,SensorID);
                //get sensor info
                Sensor_GetStatus(SensorID, SensorMode_ISO[i][SensorID].SenMode, &Drv_Sensor);

                //chg sie to capture mode
                CalApi()->fpSetMode(CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpISO_CtrlInit);
                Bayer = CalApi()->fpGetRawInfo();

                //cal 3a converage
                //ae
                aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,AESetting_ISO_ADJ[SensorID],SensorID);

                //pause sie
                CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);

                //run alg
                pISOFmt = (CAL_ISO_FMT*)&pTag->TagData;
                pISOFmt->GainBase = aeRst.uiISO;
                DBG_IND("ISO Rst(SensorMode = %d): Gain Base = %d\r\n",SensorMode_ISO[i][SensorID].SenMode, pISOFmt->GainBase);
                //CalApi()->fpSaveRAW(Bayer.BaseInfo.PixelAddr,Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height);
                //chk calibration rst OK or NG
                pTag->CAL_STATUS = _CAL_OK;

                pTag->TagDataLen = sizeof(CAL_ISO_FMT);
                pTag->TagID = (UINT32)GetTagID(SEC_ISO, SensorMode_ISO[i][SensorID].SenMode);
                //write tag
                CAL_RW()->fpAddTag(SEC_ISO,pTag);
                //release buf
                CalApi()->fpBufEvent()->fpUnInit(buf.Id);

                sprintf((char*)m_CalMsg, "ISO Rst(SensorMode:%d), Gain Base:%d",SensorMode_ISO[i][SensorID].SenMode,pISOFmt->GainBase);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
            }
        }
    }
    //close sie
    CalApi()->fpSetMode(CAL_IPL_SIECLOSE,NULL,NULL);

    //write pstore
    CAL_RW()->fpSectionWrite(SEC_ISO);
#if 0
{
    pCAL_TAG pTag;
    CAL_ISO_FMT* pFmt;
    CAL_RW()->fpSectionRead(SEC_ISO,DmaAddr,DmaSize);
    pTag = CAL_RW()->fpReadTag(SEC_ISO, GetTagID(SEC_ISO, SensorMode_ISO[++i][++SensorID].SenMode));
    pFmt = (CAL_ISO_FMT*)&pTag->TagData;
    DBG_IND("CHK: %d,\r\n",pFmt->GainBase);
}
#endif
    if(fpUIcb)
        fpUIcb(UICB_END,NULL,UICOL_GREEN);
    return E_OK;
}

static ER ECS_Adjust(fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    UINT32 i;
    UINT32 SensorID = SENSOR_ID_MAX_NUM;
    CAL_AE_RST aeRst;
    CTRL_PTR CtrlPtr;
    SENSOR_INFO Drv_Sensor;
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    UINT32 CompenRatio=100;// Compensation ratio, range = 0~100
    CAL_ECS_FMT* pECSFmt;
    USIZE tblSize = {0};

    sprintf((char*)m_CalMsg, "Set the environment to Bright");
    if(fpUIcb)
        fpUIcb(UICB_START,(CHAR*)m_CalMsg,UICOL_GREEN);

    //stop pipeline ycc part
    CalApi()->fpSetMode(CAL_IPL_STOP,NULL,NULL);
    //pause sie
    CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);
    //allocate section buffer
    dmaRst = CfgSectionBuf(SEC_ECS,DmaAddr,DmaSize);
    if(dmaRst.erCode != E_OK)
        return E_SYS;
    else
    {
        DmaAddr = dmaRst.RemainAddr;
        DmaSize = dmaRst.RemainSize;
    }
    ECS_MAP_SIZE(Cal_ecs_info.Ecs_map, &tblSize.w);
    tblSize.h = tblSize.w;
    //shift current dram address (shift size: 1 tag size), temporal buf
    TagTempAddr = DmaAddr;
    pTag = (pCAL_TAG)TagTempAddr;
    pTag->TagDataLen = sizeof(CAL_ECS_HEADER)+sizeof(UINT32)*tblSize.w*tblSize.h;
    TagSize = (TAG_TypeSize + pTag->TagDataLen);


    if(DmaSize >= TagSize)
    {
        DmaSize -= TagSize;
        DmaAddr += TagSize;
    }
    else
    {
        DBG_ERR("section buffer overflow\r\n");
        return E_SYS;
    }

    while(SensorID--)
    {
        i = sizeof(SensorMode_ECS)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
        while(i--)
        {
            if(SensorMode_ECS[i][SensorID].SenMode != BYPASS)
            {
                CalBuf buf;
                //lock 3a,fd...etc.
                AAA_SWITCH(TRUE,SensorID);
                //ipp buf init
                buf.Id = SensorMode_ECS[i][SensorID].SenMode;
                buf.StartAddr = DmaAddr;
                buf.Size = DmaSize;
                CalApi()->fpBufEvent()->fpInit(buf);
                //init sensor info
                CtrlPtr.pCal_Sensor = (void*)&SensorMode_ECS[i][SensorID];
                Sensor_GetStatus(SensorID, SensorMode_ECS[i][SensorID].SenMode, &Drv_Sensor);
                CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;
                //config target sensor mode
                IPC_WaitVD(TRUE,SensorID);
                Sensor_ChgMode(SensorID, SensorMode_ECS[i][SensorID].SenMode);
                IPC_WaitVD(TRUE,SensorID);
                //get sensor info
                Sensor_GetStatus(SensorID, SensorMode_ECS[i][SensorID].SenMode, &Drv_Sensor);

                //chg sie to capture mode
                CalApi()->fpSetMode(CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpECS_CtrlInit);
                Bayer = CalApi()->fpGetRawInfo();

                //cal 3a converage
                //ae
                aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,AESetting_ECS_ADJ[SensorID],SensorID);

                //pause sie
                CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);

                //run alg
                pECSFmt = (CAL_ECS_FMT*)&pTag->TagData;
                pECSFmt->header.Ecs_bit = Cal_ecs_info.Ecs_bit;
                pECSFmt->header.Ecs_map = Cal_ecs_info.Ecs_map;

                CalApi()->fpAlgEvent()->fpECS_Tbl(&Bayer.BaseInfo, CompenRatio,(UINT32)&pECSFmt->ECSBlockWin,tblSize.w,tblSize.h,pECSFmt->header.Ecs_bit);
                DBG_IND("ECS (SensorMode = %d) finish!!\r\n",SensorMode_ECS[i][SensorID].SenMode);

                //chk calibration rst OK or NG
                pTag->CAL_STATUS = _CAL_OK;
                pTag->TagDataLen = sizeof(CAL_ECS_HEADER) + sizeof(UINT32)*tblSize.w*tblSize.h;

                pTag->TagID = (UINT32)GetTagID(SEC_ECS, SensorMode_ECS[i][SensorID].SenMode);
                //write tag
                CAL_RW()->fpAddTag(SEC_ECS,pTag);
                //release buf
                CalApi()->fpBufEvent()->fpUnInit(buf.Id);

                sprintf((char*)m_CalMsg, "ECS (SensorMode:%d) finish!!",SensorMode_ECS[i][SensorID].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);

            }
        }
    }
    //close sie
    CalApi()->fpSetMode(CAL_IPL_SIECLOSE,NULL,NULL);

    //write pstore
    CAL_RW()->fpSectionWrite(SEC_ECS);


#if 0
{
    pCAL_TAG pTag;
    CAL_ECS_FMT* pFmt;
    CAL_RW()->fpSectionRead(SEC_ECS,DmaAddr,DmaSize);
    pTag = CAL_RW()->fpReadTag(SEC_ECS, GetTagID(SEC_ECS, SensorMode_ECS[++i][++SensorID].SenMode));
    pFmt = (CAL_ECS_FMT*)&pTag->TagData;
    DBG_IND("CHK: %d,%d,%d\r\n",pFmt->header.Ecs_bit,pFmt->header.Ecs_map,pFmt->ECSBlockWin);
}
#endif
    if(fpUIcb)
        fpUIcb(UICB_END,NULL,UICOL_GREEN);
    return E_OK;
}

static ER AWB_Adjust(fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    UINT32 i;
    UINT32 SensorID = SENSOR_ID_MAX_NUM;
    CAL_AE_RST aeRst;
    CTRL_PTR CtrlPtr;
    SENSOR_INFO Drv_Sensor;
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    CAL_AWB_FMT* pAwbFmt;

    sprintf((char*)m_CalMsg, "Set the environment to Bright");
    if(fpUIcb)
        fpUIcb(UICB_START,(CHAR*)m_CalMsg,UICOL_GREEN);

    //stop pipeline ycc part
    CalApi()->fpSetMode(CAL_IPL_STOP,NULL,NULL);
    //pause sie
    CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);
    //allocate section buffer
    dmaRst = CfgSectionBuf(SEC_AWB,DmaAddr,DmaSize);
    if(dmaRst.erCode != E_OK)
        return E_SYS;
    else
    {
        DmaAddr = dmaRst.RemainAddr;
        DmaSize = dmaRst.RemainSize;
    }

    //shift current dram address (shift size: 1 tag size), temporal buf
    TagTempAddr = DmaAddr;
    pTag = (pCAL_TAG)TagTempAddr;
    pTag->TagDataLen = sizeof(CAL_AWB_FMT);
    TagSize = (TAG_TypeSize + pTag->TagDataLen);

    if(DmaSize >= TagSize)
    {
        DmaSize -= TagSize;
        DmaAddr += TagSize;
    }
    else
    {
        DBG_ERR("section buffer overflow\r\n");
        return E_SYS;
    }

    while(SensorID--)
    {
        i = sizeof(SensorMode_AWB)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
        while(i--)
        {
            //create tag
            if(SensorMode_AWB[i][SensorID].SenMode != BYPASS)
            {
                CalBuf buf;

                //lock 3a,fd...etc.
                AAA_SWITCH(TRUE,SensorID);
                //ipp buf init
                buf.Id = SensorMode_AWB[i][SensorID].SenMode;
                buf.StartAddr = DmaAddr;
                buf.Size = DmaSize;
                CalApi()->fpBufEvent()->fpInit(buf);
                //init sensor info
                CtrlPtr.pCal_Sensor = (void*)&SensorMode_AWB[i][SensorID];
                Sensor_GetStatus(SensorID, SensorMode_AWB[i][SensorID].SenMode, &Drv_Sensor);
                CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;
                //config target sensor mode
                IPC_WaitVD(TRUE,SensorID);
                Sensor_ChgMode(SensorID, SensorMode_AWB[i][SensorID].SenMode);
                IPC_WaitVD(FALSE,SensorID);
                //get sensor info
                Sensor_GetStatus(SensorID, SensorMode_AWB[i][SensorID].SenMode, &Drv_Sensor);

                //chg sie to capture mode
                CalApi()->fpSetMode(CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpAWB_CtrlInit);
                Bayer = CalApi()->fpGetRawInfo();

                //cal 3a converage
                //ae
                aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,AESetting_AWB_ADJ[SensorID],SensorID);

                //pause sie
                CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);

                //run alg
                pAwbFmt = (CAL_AWB_FMT*)&pTag->TagData;
                pAwbFmt->BitSel = CAL_AWB_BIT;
                CalApi()->fpAlgEvent()->fpAWB_AVG(&Bayer.BaseInfo,&(pAwbFmt->Rg),&(pAwbFmt->Bg),pAwbFmt->BitSel);
                DBG_IND("AWB Rst(SensorMode = %d): Rgain = %d,Bgain = %d\r\n",SensorMode_AWB[i][SensorID].SenMode,(pAwbFmt->Rg), (pAwbFmt->Bg));
                //CalApi()->fpSaveRAW(Bayer.BaseInfo.PixelAddr,Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height);
                //chk calibration rst OK or NG
                pTag->CAL_STATUS = _CAL_OK;

                pTag->TagID = (UINT32)GetTagID(SEC_AWB, SensorMode_AWB[i][SensorID].SenMode);
                pTag->TagDataLen = sizeof(CAL_AWB_FMT);
                //write tag
                CAL_RW()->fpAddTag(SEC_AWB,pTag);
                //release buf
                CalApi()->fpBufEvent()->fpUnInit(buf.Id);

                sprintf((char*)m_CalMsg, "AWB Rst SensorMode:%d, Rgain:%d,Bgain:%d",SensorMode_AWB[i][SensorID].SenMode,(pAwbFmt->Rg), (pAwbFmt->Bg));
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
            }
        }
    }
    //close sie
    CalApi()->fpSetMode(CAL_IPL_SIECLOSE,NULL,NULL);

    //write pstore
    CAL_RW()->fpSectionWrite(SEC_AWB);
#if 0
{
    pCAL_TAG pTag;
    CAL_AWB_FMT* pFmt;
    CAL_RW()->fpSectionRead(SEC_AWB,DmaAddr,DmaSize);
    pTag = CAL_RW()->fpReadTag(SEC_AWB, GetTagID(SEC_AWB, SensorMode_AWB[++i][++SensorID].SenMode));
    pFmt = (CAL_AWB_FMT*)&pTag->TagData;
    DBG_IND("CHK: %d,%d,%d\r\n",pFmt->Rg,pFmt->Bg,pFmt->BitSel);
}
#endif
    if(fpUIcb)
        fpUIcb(UICB_END,NULL,UICOL_GREEN);
    return E_OK;
}

static ER DP_Adjust(fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    UINT32 i, SensorID = SENSOR_ID_MAX_NUM;
    CAL_AE_RST aeRst;
    CTRL_PTR CtrlPtr;
    SENSOR_INFO Drv_Sensor;
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    DPC_ALG_INFO DP_Info;

    sprintf((char*)m_CalMsg, "Set the environment to Bright");
    if(fpUIcb)
        fpUIcb(UICB_START,(CHAR*)m_CalMsg,UICOL_GREEN);

    //stop pipeline ycc part
    CalApi()->fpSetMode(CAL_IPL_STOP,NULL,NULL);
    //pause sie
    CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);
    //allocate section buffer
    dmaRst = CfgSectionBuf(SEC_DP,DmaAddr,DmaSize);
    if(dmaRst.erCode != E_OK)
        return E_SYS;
    else
    {
        DmaAddr = dmaRst.RemainAddr;
        DmaSize = dmaRst.RemainSize;
    }

    TagTempAddr = DmaAddr;

    //shift current dram address (shift size: 1 tag size), temporal buf
    pTag = (pCAL_TAG)TagTempAddr;
    pTag->TagDataLen = sizeof(CAL_DP_FMT) + DP_MAX_CNT*sizeof(UINT32);
    DBG_ERR("DP table size need to review\r\n");
    TagSize = (TAG_TypeSize + pTag->TagDataLen);

    if(DmaSize >= TagSize)
    {
        DmaSize -= TagSize;
        DmaAddr += TagSize;
    }
    else
    {
        DBG_ERR("buffer overflow\r\n");
        return E_SYS;
    }
    //Bright environment
    while(SensorID--)
    {
        CAL_DP_FMT* pDPFmt;
        i = sizeof(SensorMode_DP)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
        while(i--)
        {
            if(SensorMode_DP[i][SensorID].SenMode != BYPASS)
            {
                BOOL bDpOverFlow = FALSE;
                CalBuf buf;
                //lock 3a,fd...etc.
                AAA_SWITCH(TRUE,SensorID);
                //ipp buf init
                buf.Id = SensorMode_DP[i][SensorID].SenMode;
                buf.StartAddr = DmaAddr;
                buf.Size = DmaSize;
                CalApi()->fpBufEvent()->fpInit(buf);
                //init sensor info
                CtrlPtr.pCal_Sensor = (void*)&SensorMode_DP[i][SensorID];
                Sensor_GetStatus(SensorID, SensorMode_DP[i][SensorID].SenMode, &Drv_Sensor);
                CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;
                //config target sensor mode
                IPC_WaitVD(TRUE,SensorID);
                Sensor_ChgMode(SensorID, SensorMode_DP[i][SensorID].SenMode);
                IPC_WaitVD(TRUE,SensorID);
                //get sensor info
                Sensor_GetStatus(SensorID, SensorMode_DP[i][SensorID].SenMode, &Drv_Sensor);

                //chg sie to capture mode
                CalApi()->fpSetMode(CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpDP_CtrlInit);
                Bayer = CalApi()->fpGetRawInfo();

                //cal 3a converage
                //ae
                aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,AESetting_DP_W_ADJ[SensorID],SensorID);

                //pause sie
                CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);

                //run alg
                pDPFmt = (CAL_DP_FMT*)&pTag->TagData;

                DP_Info.DPC.Bayer = Bayer.BaseInfo;
                DP_Info.DPC.DPAddr = (UINT32)&pDPFmt->DP_Pool;
                DP_Info.DPC.BayField = 1;//cmos
                DP_Info.DPC.BayAct_Str = Bayer.ActStart;
                DP_Info.DPC.BayCrop_Str = Bayer.CropStart;
                DP_Info.DPC.Count = 0;
                DP_Info.DPC.MaxCount = DP_MAX_CNT;
                DP_Info.DPC.Threshold = DPC_THRES_BRIGHT[SensorID].Threshold;
                DP_Info.DPC.Block_X = DPC_THRES_BRIGHT[SensorID].Block_X;
                DP_Info.DPC.Block_Y = DPC_THRES_BRIGHT[SensorID].Block_Y;
                DP_Info.SkipXL = 0;
                DP_Info.SkipXR = 0;
                DP_Info.SkipYD = 0;
                DP_Info.SkipYU = 0;

                //CalApi()->fpSaveRAW(Bayer.BaseInfo.PixelAddr,Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height);

                pDPFmt->DPCnt.PixelCnt = CalApi()->fpAlgEvent()->fpDPC_Bayer(&DP_Info,&bDpOverFlow);

                DBG_IND("DP Rst(SensorMode = %d): DP cnt= %d\r\n",SensorMode_DP[i][SensorID].SenMode,pDPFmt->DPCnt.PixelCnt);

                if(bDpOverFlow)
                {
                    pTag->CAL_STATUS = _CAL_OverFlow;
                }
                else
                {
                    pTag->CAL_STATUS = _CAL_OK;
                }
                pTag->TagDataLen = sizeof(CAL_DPC_HEADER) + sizeof(UINT32)*pDPFmt->DPCnt.PixelCnt;
                pTag->TagID = (UINT32)GetTagID(SEC_DP, SensorMode_DP[i][SensorID].SenMode);
                //write tag
                CAL_RW()->fpAddTag(SEC_DP,pTag);

                //release buf
                CalApi()->fpBufEvent()->fpUnInit(buf.Id);

                sprintf((char*)m_CalMsg, "DP Rst(SensorMode:%d), DP cnt:%d",SensorMode_DP[i][SensorID].SenMode,pDPFmt->DPCnt.PixelCnt);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
            }
        }
    }

    sprintf((char*)m_CalMsg, "Set the environment to Dark");
    if(fpUIcb)
        fpUIcb(UICB_START,(CHAR*)m_CalMsg,UICOL_GREEN);

    /* Dark environment */
    SensorID=SENSOR_ID_MAX_NUM;
    while(SensorID--)
    {
        CAL_DP_FMT* pDPFmt;
        UINT32 DP_addr_BrightEnd;
        CAL_DPC_HEADER Dark_Cnt;

        i = sizeof(SensorMode_DP)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
        while(i--)
        {
            if(SensorMode_DP[i][SensorID].SenMode != BYPASS)
            {
                BOOL bDpOverFlow = FALSE;
                CalBuf buf;
                //lock 3a,fd...etc.
                AAA_SWITCH(TRUE,SensorID);
                //read bright dp result
                pTag = CAL_RW()->fpReadTag(SEC_DP, GetTagID(SEC_DP, SensorMode_DP[i][SensorID].SenMode));
                if(pTag == NULL)
                {
                    DBG_ERR("DP tag read fail!!\r\n");
                    return E_SYS;
                }
                CopyTag(TagTempAddr,pTag);
                pTag = (pCAL_TAG)TagTempAddr;
                //ipp buf init
                buf.Id = SensorMode_DP[i][SensorID].SenMode;
                buf.StartAddr = DmaAddr;
                buf.Size = DmaSize;
                CalApi()->fpBufEvent()->fpInit(buf);
                //init sensor info
                CtrlPtr.pCal_Sensor = (void*)&SensorMode_DP[i][SensorID];
                Sensor_GetStatus(SensorID, SensorMode_DP[i][SensorID].SenMode, &Drv_Sensor);
                CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;
                //config target sensor mode
                IPC_WaitVD(TRUE,SensorID);
                Sensor_ChgMode(SensorID, SensorMode_DP[i][SensorID].SenMode);
                IPC_WaitVD(TRUE,SensorID);
                //get sensor info
                Sensor_GetStatus(SensorID, SensorMode_DP[i][SensorID].SenMode, &Drv_Sensor);

                //chg sie to capture mode
                CalApi()->fpSetMode(CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpDP_CtrlInit);
                Bayer = CalApi()->fpGetRawInfo();

                //cal 3a converage
                //ae
                aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,AESetting_DP_B_ADJ[SensorID],SensorID);

                //pause sie
                CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);

                pDPFmt = (CAL_DP_FMT*)&pTag->TagData;

                DP_addr_BrightEnd = (UINT32)&pTag->TagData + pTag->TagDataLen;

                DP_Info.DPC.Bayer = Bayer.BaseInfo;
                DP_Info.DPC.DPAddr = DP_addr_BrightEnd;
                DP_Info.DPC.BayField = 1;//cmos
                DP_Info.DPC.BayAct_Str = Bayer.ActStart;
                DP_Info.DPC.BayCrop_Str = Bayer.CropStart;
                DP_Info.DPC.Count = pDPFmt->DPCnt.PixelCnt;
                DP_Info.DPC.MaxCount = DP_MAX_CNT;
                DP_Info.DPC.Threshold = DPC_THRES_DARK[SensorID].Threshold;
                DP_Info.DPC.Block_X = DPC_THRES_DARK[SensorID].Block_X;
                DP_Info.DPC.Block_Y = DPC_THRES_DARK[SensorID].Block_Y;
                DP_Info.SkipXL = 0;
                DP_Info.SkipXR = 0;
                DP_Info.SkipYD = 0;
                DP_Info.SkipYU = 0;

                Dark_Cnt.PixelCnt = CalApi()->fpAlgEvent()->fpDPC_Bayer(&DP_Info,&bDpOverFlow);

                pDPFmt->DPCnt.PixelCnt += Dark_Cnt.PixelCnt;
                DBG_IND("DP Rst(SensorMode = %d): DP cnt= %d\r\n",SensorMode_DP[i][SensorID].SenMode,pDPFmt->DPCnt.PixelCnt);

                if(bDpOverFlow)
                    pTag->CAL_STATUS = _CAL_OverFlow;
                else
                {
                    pTag->CAL_STATUS = _CAL_OK;
                }
                //sort Bright & Dark DP.
                CalApi()->fpAlgEvent()->fpDPC_Sort(&pDPFmt->DP_Pool,pDPFmt->DPCnt.PixelCnt);

                pTag->TagDataLen = sizeof(CAL_DPC_HEADER) + CalApi()->fpAlgEvent()->fpDPC_ArgFmt(&pDPFmt->DP_Pool,pDPFmt->DPCnt.PixelCnt,&pDPFmt->DPCnt.LineCnt);
                //write tag
                CAL_RW()->fpAddTag(SEC_DP,pTag);
                //release buf
                CalApi()->fpBufEvent()->fpUnInit(buf.Id);

                sprintf((char*)m_CalMsg, "DP Rst(SensorMode:%d), DP cnt:%d",SensorMode_DP[i][SensorID].SenMode,pDPFmt->DPCnt.PixelCnt);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
            }
        }
    }

    //close sie
    CalApi()->fpSetMode(CAL_IPL_SIECLOSE,NULL,NULL);

    //write pstore
    CAL_RW()->fpSectionWrite(SEC_DP);
#if 0
{
    pCAL_TAG pTag;
    CAL_DP_FMT* pFmt;
    CAL_RW()->fpSectionRead(SEC_DP,DmaAddr,DmaSize);
    pTag = CAL_RW()->fpReadTag(SEC_DP, GetTagID(SEC_DP, SensorMode_DP[++i][++SensorID].SenMode));
    pFmt = (CAL_DP_FMT*)&pTag->TagData;
    DBG_IND("CHK: %d,%d,%x\r\n",pFmt->DPCnt.PixelCnt,pFmt->DPCnt.LineCnt,pFmt->DP_Pool);
}
#endif
    if(fpUIcb)
        fpUIcb(UICB_END,NULL,UICOL_GREEN);
    return E_OK;
}

static ER Mshutter_Adjust(fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    if(fpUIcb)
        fpUIcb(UICB_START,NULL,UICOL_GREEN);

    DBG_ERR("not ready yet\r\n");

    if(fpUIcb)
        fpUIcb(UICB_END,(CHAR*)m_CalMsg,UICOL_GREEN);
    return E_OK;
}

#if 0
#endif
CAL_ITEM_OBJ m_CalItem = {
    ISO_Adjust,
    AWB_Adjust,
    Mshutter_Adjust,
    ECS_Adjust,
    DP_Adjust,
};


pCAL_ITEM_OBJ CAL_ITEM_IMX083_SAMPLE_FF(void)
{
    //release buf used by set2preview first
    CalApi()->fpBufEvent()->fpUnInit(IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));
    return &m_CalItem;
}

/**
    this preview have no calibration result.
*/
ER Set2Preview_IMX083_SAMPLE_FF(UINT32 BufAddr,UINT32 BufSize)
{
    CalBuf buf = {0};
    SENSOR_INFO Drv_Sensor;
    CTRL_PTR CtrlInfo = {0};
    ER ErrorCode = E_OK;
    UINT32 i;
    IPL_RAW_IMG BayerInfo;

    IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SensorMode[0][SENSOR_ID_1].SenMode);
    //init buf for preview only
    buf.Id = IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE);
    buf.StartAddr = BufAddr;
    buf.Size = BufSize;
    ErrorCode = CalApi()->fpBufEvent()->fpInit(buf);
    DBG_WRN("need to review : preview support only 1 sensor ...\r\n");


    //set sensor mode
    Sensor_ChgMode(SENSOR_ID_1,IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));

    Sensor_GetStatus(SENSOR_ID_1, IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &Drv_Sensor);

    /*
        TODO: need to add displayer control
    */
    //CtrlInfo.fpRdyImg = ;
    DBG_WRN("need to add displayer control\r\n");
    CtrlInfo.pDrv_Sensor = (void*)&Drv_Sensor;
    IPL_ResetDisplayCtrlFlow();
        //get calibration info mapping to current sensor mode
        i = CalSensorModeNum;
        while(i--)
        {
            if(SensorMode[i][SENSOR_ID_1].SenMode == IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE))
            {
                CtrlInfo.pCal_Sensor = (void*)&SensorMode[i][SENSOR_ID_1];
                break;
            }
        }

    //start ipp
    ErrorCode = CalApi()->fpSetMode(CAL_IPL_PRV,&CtrlInfo,Cal_CtrlInit()->fpPrv_CtrlInit);

    //trigger AE & AWB
    //for (i = IPL_ID_1; i < ChgMode->SenNum; i ++)
    if(1)
    {
        AE_Start(IPL_UTI_CONV2_AE_ID(IPL_ID_1), TRUE);
        AWB_Start(IPL_UTI_CONV2_AWB_ID(IPL_ID_1), TRUE);
    }

    BayerInfo = CalApi()->fpGetRawInfo();
    CalApi()->fp3AEvent()->fpAEStable(&BayerInfo.BaseInfo,AESetting_Prv[0],0);

    //AE_WaitStable(3);
    return ErrorCode;
}
