#include "string.h"
#include "stdio.h"
#include "IPL_CAL_Debug.h"
#include "IPL_Cal_Ctrl_IMX322_EVB_FF_Int.h"
#include "Ipl_Cal_Setting_IMX322_EVB_FF_Int.h"
#include "ae_task.h"
#include "awb_task.h"
#include "ipl_alginfor.h"
#include "ipl_calibration.h"
#include "ipl_CalRW.h"
#include "ipl_cal_datapassing_IMX322_EVB_FF_int.h"
#include "ipl_display.h"
#include "IPL_Hal_Op_Int.h"
#include "IPL_Hal.h"
#include "display.h"
#include "filesystsk.h"
#include "Ide.h"

CalIQInfo gCalIQData = {0};
static CHAR m_CalMsg[100];
INT32 CalStringBuffer[32];
char    sFileName[64];
FST_FILE    pFileHdl;

CAL_AWB_FMT gAwbFmt = {0};
CAL_AWB_FMT gAwbGSFmt = {323,532};
UINT32 CalStrColor = UICOL_GREEN;

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
    CAL_DMA_RST rst = {0};
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
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor;
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    CAL_ISO_FMT* pISOFmt;
    UINT32 Defalut_Gain_Base = 250;     //default gain base.
    UINT32 Default_Gain_Thr = 20; //default gain base threshold (%).

    CAL_RW()->fpDeleteSection(SEC_ISO);

    sprintf((char*)m_CalMsg, "Set to Bright");
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

                //cal 3a converage //ae
                //run alg
                aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,AESetting_ISO_ADJ[SensorID],SensorID);

                //pause sie
                CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);

                pISOFmt = (CAL_ISO_FMT*)&pTag->TagData;
                pISOFmt->GainBase = aeRst.uiISOBase;

                DBG_IND("ISO Rst(SensorMode = %d): Gain Base = %d\r\n",SensorMode_ISO[i][SensorID].SenMode, pISOFmt->GainBase);

                //save calibration raw
                //CalApi()->fpSaveRAW(Bayer.BaseInfo.PixelAddr,Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height);

#if 0   //support update ISO threshold from text file
                UINT32 ISO_THR_Size = 2;
                UB ISO_THR_Addr[2];
                UINT32 Gain_Threshold_In; //Input hreshold from text file if necessary.

                sprintf(sFileName, "A:\\ISO_THR.txt", 0);
                pFileHdl = FileSys_OpenFile(sFileName, FST_OPEN_READ);

                if (pFileHdl)
                {
                    FileSys_ReadFile(pFileHdl, (UINT8 *)ISO_THR_Addr, &ISO_THR_Size, 0, NULL);
                    Gain_Threshold_In =(ISO_THR_Addr[0]-0x30)*10+(ISO_THR_Addr[1]-0x30);

                    DBG_ERR("GAIN THR CHANGED: %d\r\n",Gain_Threshold_In);

                    if ((Gain_Threshold_In >= 0) && (Gain_Threshold_In <= 100))
                    {
                        Default_Gain_Thr = Gain_Threshold_In;
                    }
                    FileSys_CloseFile(pFileHdl);
                }
#endif
#if 0   //support update ISO BASE from text file
                UINT32 ISO_BASE_Size = 3;
                UB ISO_BASE_Addr[3];
                UINT32 Gain_Base_In; //Input gain base from text file if necessary.

                sprintf(sFileName, "A:\\ISO_BASE.txt", 0);
                pFileHdl = FileSys_OpenFile(sFileName, FST_OPEN_READ);

                if (pFileHdl)
                {
                    FileSys_ReadFile(pFileHdl, (UINT8 *)ISO_BASE_Addr, &ISO_BASE_Size, 0, NULL);
                    Gain_Base_In =(ISO_BASE_Addr[0]-0x30)*100+(ISO_BASE_Addr[1]-0x30)*10+(ISO_BASE_Addr[2]-0x30);

                    DBG_ERR("GAIN BASE CHANGED: %d\r\n",Gain_Base_In);

                    if (Gain_Base_In != 0)
                    {
                        Defalut_Gain_Base = Gain_Base_In;
                    }
                    FileSys_CloseFile(pFileHdl);
                }
#endif

                DBG_ERR("DEFAULT ISO BASE: %d,DEFAULT ISO_GAIN_THR: %d\r\n", Defalut_Gain_Base, Default_Gain_Thr);

                //chk calibration rst OK or NG
                if (((100*abs(Defalut_Gain_Base - pISOFmt->GainBase))/Defalut_Gain_Base) <= Default_Gain_Thr)
                {
                    pTag->CAL_STATUS = _CAL_OK;
                    gCalIQData.Cal_Status.ISO_STATUS = _CAL_OK;
                    sprintf((char*)m_CalMsg, "ISO  PASS");

                    if(fpUIcb)
                        fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);

                    CalStrColor = UICOL_GREEN;
                }
                else
                {
                    pTag->CAL_STATUS = _CAL_NONE;
                    gCalIQData.Cal_Status.ISO_STATUS = _CAL_NONE;
                    sprintf((char*)m_CalMsg, "ISO  NG");

                    if(fpUIcb)
                        fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);

                    CalStrColor = UICOL_RED;
                }

                pTag->TagDataLen = sizeof(CAL_ISO_FMT);
                pTag->TagID = (UINT32)GetTagID(SEC_ISO, SensorMode_ISO[i][SensorID].SenMode);
                //write tag
                CAL_RW()->fpAddTag(SEC_ISO,pTag);
                //release buf
                CalApi()->fpBufEvent()->fpUnInit(buf.Id);

                sprintf((char*)m_CalMsg, "DEFAULT BASE: %d",Defalut_Gain_Base);    //Default Gain Base
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);

                sprintf((char*)m_CalMsg, "CAL GAIN BASE: %d",pISOFmt->GainBase);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);
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
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor;
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    UINT32 CompenRatio = 90;// Compensation ratio, range = 0~100
    CAL_ECS_FMT* pECSFmt;
    USIZE tblSize = {0};
    BOOL ECS_Over_Spec;

    CAL_RW()->fpDeleteSection(SEC_ECS);

    sprintf((char*)m_CalMsg, "Set to Bright");
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

                //CalApi()->fpSaveRAW(Bayer.BaseInfo.PixelAddr,Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height);

                ECS_Over_Spec = CalApi()->fpAlgEvent()->fpECS_Tbl(&Bayer.BaseInfo, CompenRatio,(UINT32)&pECSFmt->ECSBlockWin,tblSize.w,tblSize.h,pECSFmt->header.Ecs_bit);
                DBG_IND("ECS (SensorMode = %d) finish!!\r\n",SensorMode_ECS[i][SensorID].SenMode);

                //chk calibration rst OK or NG
                if (ECS_Over_Spec == TRUE)
                {
                    pTag->CAL_STATUS = _CAL_OverFlow;
                    gCalIQData.Cal_Status.ECS_STATUS = _CAL_OverFlow;
                    sprintf((char*)m_CalMsg, "COLOR SHADING OVERFLOW");
                    CalStrColor = UICOL_RED;
                }
                else
                {
                    pTag->CAL_STATUS = _CAL_OK;
                    gCalIQData.Cal_Status.ECS_STATUS = _CAL_OK;
                    sprintf((char*)m_CalMsg, "COLOR SHADING CAL OK");
                    CalStrColor = UICOL_GREEN;
                }

                pTag->TagDataLen = sizeof(CAL_ECS_HEADER) + sizeof(UINT32)*tblSize.w*tblSize.h;

                pTag->TagID = (UINT32)GetTagID(SEC_ECS, SensorMode_ECS[i][SensorID].SenMode);
                //write tag
                CAL_RW()->fpAddTag(SEC_ECS,pTag);
                //release buf
                CalApi()->fpBufEvent()->fpUnInit(buf.Id);
            }
        }
    }

    if(fpUIcb)
        fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);

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
        fpUIcb(UICB_END,NULL,CalStrColor);
    return E_OK;
}

static ER AWB_Adjust(fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    UINT32 i;
    UINT32 SensorID = SENSOR_ID_MAX_NUM;
    CAL_AE_RST aeRst;
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor;
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    CAL_AWB_FMT* pAwbFmt;
    UINT32 GS_Rg = 0x100, GS_Bg = 0x100, Gain_Threshold = 15;

    CAL_RW()->fpDeleteSection(SEC_AWB);

    sprintf((char*)m_CalMsg, "Set to Bright");
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

                gAwbFmt.Rg = pAwbFmt->Rg;
                gAwbFmt.Bg = pAwbFmt->Bg;


#if 0   //support update awb threshold by text file
                UINT32 AWB_THR_Size = 2;
                UB AWB_THR_Addr[2];
                UINT32 TempGain_Threshold;

                sprintf(sFileName, "A:\\AWB_THR.txt", 0);
                pFileHdl = FileSys_OpenFile(sFileName, FST_OPEN_READ);

                if (pFileHdl)
                {
                    FileSys_ReadFile(pFileHdl, (UINT8 *)AWB_THR_Addr, &AWB_THR_Size, 0, NULL);
                    TempGain_Threshold =(AWB_THR_Addr[0]-0x30)*10+(AWB_THR_Addr[1]-0x30);

                    DBG_ERR("Modify AWB GS param Threshold: %d\r\n",TempGain_Threshold);

                    if (TempGain_Threshold > 0 && TempGain_Threshold < 100)
                    {
                        Gain_Threshold = TempGain_Threshold;
                    }

                    FileSys_CloseFile(pFileHdl);
                }
#endif
#if 1   //Input GS value from text file
                UINT32 AWB_GS_Size = 7;
                UB AWB_GS_Addr[7];

                sprintf(sFileName, "A:\\AWB_GSgain.txt", 0);
                pFileHdl = FileSys_OpenFile(sFileName, FST_OPEN_READ);

                if (pFileHdl)
                {
                    FileSys_ReadFile(pFileHdl, (UINT8 *)AWB_GS_Addr, &AWB_GS_Size, 0, NULL);
                    GS_Rg =(AWB_GS_Addr[0]-0x30)*100+(AWB_GS_Addr[1]-0x30)*10+(AWB_GS_Addr[2]-0x30);
                    GS_Bg =(AWB_GS_Addr[4]-0x30)*100+(AWB_GS_Addr[5]-0x30)*10+(AWB_GS_Addr[6]-0x30);

                    DBG_ERR("Modify AWB GS param GS R: %d,GS B: %d\r\n",GS_Rg,GS_Bg);

                    if (GS_Rg > 0 && GS_Rg < 1023)
                    {
                        gAwbGSFmt.Rg= GS_Rg;
                    }

                    if (GS_Bg > 0 && GS_Bg < 1023)
                    {
                        gAwbGSFmt.Bg= GS_Bg;
                    }

                    FileSys_CloseFile(pFileHdl);
                }
#endif

                if (gAwbGSFmt.Rg != 0 && gAwbGSFmt.Bg != 0)
                {
                    GS_Rg = gAwbGSFmt.Rg;
                    GS_Bg = gAwbGSFmt.Bg;

                    DBG_ERR("GS %d,%d,%d\r\n",GS_Rg,GS_Bg,Gain_Threshold);

                    if ((((100*abs(GS_Rg - pAwbFmt->Rg))/GS_Rg) < Gain_Threshold) && (((100*abs(GS_Bg - pAwbFmt->Bg))/GS_Bg) < Gain_Threshold))
                    {
                        sprintf((char*)m_CalMsg, "AWB: PASS");
                        gCalIQData.Cal_Status.AWB_STATUS = _CAL_OK;
                        CalStrColor = UICOL_GREEN;
                    }
                    else
                    {
                        sprintf((char*)m_CalMsg, "AWB: NG");
                        gCalIQData.Cal_Status.AWB_STATUS = _CAL_NONE;
                        pTag->CAL_STATUS = _CAL_NONE;
                        CalStrColor = UICOL_RED;
                    }

                    pAwbFmt->Rg = (pAwbFmt->Rg*100)/GS_Rg;
                    pAwbFmt->Bg = (pAwbFmt->Bg*100)/GS_Bg;
                }
                else
                {
                    pAwbFmt->Rg = 100;
                    pAwbFmt->Bg = 100;

                    sprintf((char*)m_CalMsg, "AWB NG-NO GS");
                    gCalIQData.Cal_Status.AWB_STATUS = _CAL_NONE;
                    pTag->CAL_STATUS = _CAL_NONE;
                    CalStrColor = UICOL_RED;
                }

                //write tag
                CAL_RW()->fpAddTag(SEC_AWB,pTag);
                //release buf
                CalApi()->fpBufEvent()->fpUnInit(buf.Id);

                DBG_ERR("AWB_GS_Rg = %d, AWB_GS_Bg = %d, Rg = %d, BG = %d\r\n",GS_Rg,GS_Bg,pAwbFmt->Rg,pAwbFmt->Bg);

                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);

                sprintf((char*)m_CalMsg, "RED  %d",pAwbFmt->Rg);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);

                sprintf((char*)m_CalMsg, "BLUE %d",pAwbFmt->Bg);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);
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

static ER AWBGS_Adjust(fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    UINT32 i;
    UINT32 SensorID = SENSOR_ID_MAX_NUM;
    CAL_AE_RST aeRst;
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor;
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    CAL_AWB_FMT* pAwbFmt;
    UINT32 size = 7;

    CAL_RW()->fpDeleteSection(SEC_AWB);

    sprintf((char*)m_CalMsg, "Set to Bright");
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
//                CAL_RW()->fpAddTag(SEC_AWB,pTag);
                gAwbGSFmt.Rg = pAwbFmt->Rg;
                gAwbGSFmt.Bg = pAwbFmt->Bg;

                gAwbFmt.Rg = pAwbFmt->Rg;
                gAwbFmt.Bg = pAwbFmt->Bg;

                //Write AWB GS gain to SD card
                sprintf(sFileName, "A:\\AWBGS.txt");
                pFileHdl = FileSys_OpenFile(sFileName,FST_CREATE_ALWAYS|FST_OPEN_WRITE);

                if (pFileHdl)
                {
                    sprintf((char*)CalStringBuffer, "%d %d",gAwbGSFmt.Rg,gAwbGSFmt.Bg);
                    FileSys_WriteFile(pFileHdl, (UINT8*)CalStringBuffer, &size, 0, NULL);
                    FileSys_CloseFile(pFileHdl);

                    sprintf((char*)m_CalMsg, "AWB GS Rg %d",pAwbFmt->Rg);
                    if(fpUIcb)
                        fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);

                    sprintf((char*)m_CalMsg, "AWB GS Bg %d",pAwbFmt->Bg);
                    if(fpUIcb)
                        fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);

                    FileSys_CloseFile(pFileHdl);
                }
                else
                {
                    sprintf((char*)m_CalMsg, "AWBGS Save Fail");
                    if(fpUIcb)
                        fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                }

                pAwbFmt->Rg = 100;
                pAwbFmt->Bg = 100;

                //write tag
                CAL_RW()->fpAddTag(SEC_AWB,pTag);
                //release buf
                CalApi()->fpBufEvent()->fpUnInit(buf.Id);

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

static ER DP_Adjust_Dark(fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    UINT32 i, SensorID = SENSOR_ID_MAX_NUM;
    CAL_AE_RST aeRst;
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor;
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    DPC_ALG_INFO DP_Info;
    UINT32 Cal_Status = _CAL_OK;
    UINT32 DP_Status_Chk = _CAL_DP_B_OK;

    pTag = (pCAL_TAG)DmaAddr;
    pTag->TagDataLen = sizeof(CAL_DP_FMT) + DP_MAX_CNT*sizeof(UINT32);
    TagSize = (TAG_TypeSize + pTag->TagDataLen);

    if(DmaSize < TagSize)
    {
        DBG_ERR("buffer overflow\r\n");
        return E_SYS;
    }
debug_err((">>>>>DP Dard IMX322\r\n"));
    i = sizeof(SensorMode_DP)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
    
    while(i--)
    {
        if(SensorMode_DP[i][SensorID].SenMode != BYPASS)
        {
            pTag = CAL_RW()->fpReadTag(SEC_DP, GetTagID(SEC_DP, SensorMode_DP[i][SensorID].SenMode));
            if(pTag != NULL)
            {
                if (pTag->CAL_STATUS != _CAL_DP_B_OK)
                {
                    DP_Status_Chk = _CAL_NONE;
                }
            }
            else
            {
                DP_Status_Chk = _CAL_NONE;
            }
        }
    }

    sprintf((char*)m_CalMsg, "Set to Bright");
    if(fpUIcb)
        fpUIcb(UICB_WAITKEY,(CHAR*)m_CalMsg,UICOL_GREEN);

    //stop pipeline ycc part
    CalApi()->fpSetMode(CAL_IPL_STOP,NULL,NULL);
    //pause sie
    CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);

    if (DP_Status_Chk != _CAL_DP_B_OK)
    {
        CAL_RW()->fpDeleteSection(SEC_DP);

        //allocate section buffer
        dmaRst = CfgSectionBuf(SEC_DP,DmaAddr,DmaSize);
        if(dmaRst.erCode != E_OK)
            return E_SYS;
        else
        {
            DmaAddr = dmaRst.RemainAddr;
            DmaSize = dmaRst.RemainSize;
        }
    }
    TagTempAddr = DmaAddr;

    //shift current dram address (shift size: 1 tag size), temporal buf
    pTag = (pCAL_TAG)TagTempAddr;
    pTag->TagDataLen = sizeof(CAL_DP_FMT) + DP_MAX_CNT*sizeof(UINT32);
//    DBG_ERR("DP table size need to review\r\n");
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
        CAL_DPC_HEADER Dark_Cnt;

        i = sizeof(SensorMode_DP)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
        while(i--)
        {
            if(SensorMode_DP[i][SensorID].SenMode != BYPASS)
            {
                BOOL bDpOverFlow = FALSE;
                CalBuf buf;
                //lock 3a,fd...etc.
                AAA_SWITCH(FALSE,SensorID);

                if (DP_Status_Chk == _CAL_DP_B_OK)
                {
                    //read bright dp result
                    pTag = CAL_RW()->fpReadTag(SEC_DP, GetTagID(SEC_DP, SensorMode_DP[i][SensorID].SenMode));

                    if(pTag == NULL || pTag->CAL_STATUS != _CAL_DP_B_OK)
                    {
                        DBG_ERR("DP tag read fail %d!!\r\n",pTag->CAL_STATUS);
                        return E_SYS;
                    }

                    CopyTag(TagTempAddr,pTag);
                    pTag = (pCAL_TAG)TagTempAddr;
                }

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

                if (DP_Status_Chk != _CAL_DP_B_OK)
                {
                    pDPFmt->DPCnt.PixelCnt = 0;
                }

                DP_Info.DPC.Bayer = Bayer.BaseInfo;
                DP_Info.DPC.DPAddr = (UINT32)&pDPFmt->DP_Pool;
                DP_Info.DPC.BayField = 1;//cmos
                DP_Info.DPC.BayAct_Str = Bayer.ActStart;
                DP_Info.DPC.BayCrop_Str = Bayer.CropStart;
                DP_Info.DPC.Count = pDPFmt->DPCnt.PixelCnt;
                DP_Info.DPC.MaxCount = DP_MAX_CNT;
                DP_Info.DPC.Threshold = DPC_THRES_BRIGHT[SensorID].Threshold;
                DP_Info.DPC.Block_X = DPC_THRES_BRIGHT[SensorID].Block_X;
                DP_Info.DPC.Block_Y = DPC_THRES_BRIGHT[SensorID].Block_Y;
                DP_Info.SkipXL = 0;
                DP_Info.SkipXR = 0;
                DP_Info.SkipYD = 0;
                DP_Info.SkipYU = 0;

                //CalApi()->fpSaveRAW(Bayer.BaseInfo.PixelAddr,Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height);
#if 1 //save RAW
{
    char    filename1[64];
    UINT32 size, fn = 0;
    FST_FILE    pFileHdl;
 DBG_ERR("save dp \r\n");

debug_err((">>>Loffset = %d H = %d\r\n",Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height));
 
    size = Bayer.BaseInfo.LineOffset*Bayer.BaseInfo.Height;

    //for ( fn = 0; fn < 1; fn ++ )
    {
        if (i == 0)
        sprintf(filename1, "A:\\DP_Dard0.RAW", fn);
        else
            sprintf(filename1, "A:\\DP_Dard1.RAW", fn);
        pFileHdl = FileSys_OpenFile(filename1,FST_CREATE_ALWAYS|FST_OPEN_WRITE);
        FileSys_WriteFile(pFileHdl, (UINT8 *)Bayer.BaseInfo.PixelAddr, &size, 0, NULL);
        FileSys_CloseFile(pFileHdl);
    }
}
#endif
                Dark_Cnt.PixelCnt = CalApi()->fpAlgEvent()->fpDPC_Bayer(&DP_Info,&bDpOverFlow);
                pDPFmt->DPCnt.PixelCnt += Dark_Cnt.PixelCnt;

                if(bDpOverFlow)
                {
                    Cal_Status = _CAL_OverFlow;
                    pTag->CAL_STATUS = _CAL_OverFlow;
                }
                else
                {
                    if (DP_Status_Chk == _CAL_DP_B_OK)
                    {
//                        Cal_Status = _CAL_OK;
                        pTag->CAL_STATUS = _CAL_OK;
                    }
                    else
                    {
                        if (Cal_Status != _CAL_OverFlow)
                            Cal_Status = _CAL_DP_D_OK;

                        pTag->CAL_STATUS = _CAL_DP_D_OK;
                    }
                }

                if (pTag->CAL_STATUS == _CAL_OK)
                {
                    if ( pDPFmt->DPCnt.PixelCnt != 0)
                    {
                        //sort Bright & Dark DP.
                        CalApi()->fpAlgEvent()->fpDPC_Sort(&pDPFmt->DP_Pool,pDPFmt->DPCnt.PixelCnt);
                    }
                    pTag->TagDataLen = sizeof(CAL_DPC_HEADER) + CalApi()->fpAlgEvent()->fpDPC_ArgFmt(&pDPFmt->DP_Pool,pDPFmt->DPCnt.PixelCnt,&pDPFmt->DPCnt.LineCnt);
                }
                else
                {
                    pTag->TagDataLen = sizeof(CAL_DPC_HEADER) + sizeof(UINT32)*pDPFmt->DPCnt.PixelCnt;
                }

                pTag->TagID = (UINT32)GetTagID(SEC_DP, SensorMode_DP[i][SensorID].SenMode);

                DBG_IND("DP Rst(SensorMode = %d): DP cnt= %d, DATALen = %d\r\n",SensorMode_DP[i][SensorID].SenMode,pDPFmt->DPCnt.PixelCnt,pTag->TagDataLen);

                //write tag
                CAL_RW()->fpAddTag(SEC_DP,pTag);

                //release buf
                CalApi()->fpBufEvent()->fpUnInit(buf.Id);


                if (pDPFmt->DPCnt.PixelCnt >= DP_MAX_CNT)
                {
                    CalStrColor = UICOL_RED;
                }
                else
                {
                    CalStrColor = UICOL_GREEN;
                }

                sprintf((char*)m_CalMsg, "MODE %d DP cnt:%d",SensorMode_DP[i][SensorID].SenMode,pDPFmt->DPCnt.PixelCnt);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);
            }
        }
    }

    if (Cal_Status == _CAL_OverFlow)
    {
        sprintf((char*)m_CalMsg, "DP Dark:  NG");
        CalStrColor = UICOL_RED;
    }
    else if(Cal_Status == _CAL_DP_D_OK)
    {
        sprintf((char*)m_CalMsg, "DP Dark:  PASS");
        CalStrColor = UICOL_GREEN;
    }
    else //(Cal_Status == _CAL_OK)
    {
        sprintf((char*)m_CalMsg, "DP:  PASS");
        CalStrColor = UICOL_GREEN;
    }

    if(fpUIcb)
        fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);

    //close sie
    CalApi()->fpSetMode(CAL_IPL_SIECLOSE,NULL,NULL);

    DBG_ERR("\r\n-------------> cal sts %x, %x, %x",pTag->CAL_STATUS, DP_Status_Chk,Cal_Status);

    if (Cal_Status == _CAL_DP_D_OK || Cal_Status == _CAL_OK)
    {
        //write pstore
        CAL_RW()->fpSectionWrite(SEC_DP);
    }

#if 0
{
    pCAL_TAG pTag;
    CAL_DP_FMT* pFmt;
    CAL_RW()->fpSectionRead(SEC_DP,DmaAddr,DmaSize);
    pTag = CAL_RW()->fpReadTag(SEC_DP, GetTagID(SEC_DP, SensorMode_DP[++i][++SensorID].SenMode));
    pFmt = (CAL_DP_FMT*)&pTag->TagData;
    DBG_ERR("CHK: %d,%d,%x\r\n",pFmt->DPCnt.PixelCnt,pFmt->DPCnt.LineCnt,pFmt->DP_Pool);
}
#endif
    if(fpUIcb)
        fpUIcb(UICB_END,NULL,CalStrColor);
    return E_OK;
}

static ER DP_Adjust_Bright(fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    UINT32 i, SensorID = SENSOR_ID_MAX_NUM;
    CAL_AE_RST aeRst;
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor;
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    DPC_ALG_INFO DP_Info;
    UINT32 Cal_Status = _CAL_OK;
    UINT32 DP_Status_Chk = _CAL_DP_D_OK;

    pTag = (pCAL_TAG)DmaAddr;
    pTag->TagDataLen = sizeof(CAL_DP_FMT) + DP_MAX_CNT*sizeof(UINT32);
    TagSize = (TAG_TypeSize + pTag->TagDataLen);

    if(DmaSize < TagSize)
    {
        DBG_ERR("buffer overflow\r\n");
        return E_SYS;
    }

    i = sizeof(SensorMode_DP)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
    while(i--)
    {
        if(SensorMode_DP[i][SensorID].SenMode != BYPASS)
        {
            pTag = CAL_RW()->fpReadTag(SEC_DP, GetTagID(SEC_DP, SensorMode_DP[i][SensorID].SenMode));
            if(pTag != NULL)
            {
                if (pTag->CAL_STATUS != _CAL_DP_D_OK)
                {
                    DP_Status_Chk = _CAL_NONE;
                }
            }
            else
            {
                DP_Status_Chk = _CAL_NONE;
            }
        }
    }

    sprintf((char*)m_CalMsg, "Set to Dark");
    if(fpUIcb)
        fpUIcb(UICB_WAITKEY,(CHAR*)m_CalMsg,UICOL_GREEN);
debug_err((">>>>>DP Br IMX322\r\n"));
    //stop pipeline ycc part
    CalApi()->fpSetMode(CAL_IPL_STOP,NULL,NULL);
    //pause sie
    CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);

    if (DP_Status_Chk != _CAL_DP_D_OK)
    {
        CAL_RW()->fpDeleteSection(SEC_DP);
        //allocate section buffer
        dmaRst = CfgSectionBuf(SEC_DP,DmaAddr,DmaSize);
        if(dmaRst.erCode != E_OK)
            return E_SYS;
        else
        {
            DmaAddr = dmaRst.RemainAddr;
            DmaSize = dmaRst.RemainSize;
        }
    }
    TagTempAddr = DmaAddr;

    //shift current dram address (shift size: 1 tag size), temporal buf
    pTag = (pCAL_TAG)TagTempAddr;
    pTag->TagDataLen = sizeof(CAL_DP_FMT) + DP_MAX_CNT*sizeof(UINT32);
//    DBG_ERR("DP table size need to review\r\n");
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
debug_err((">>>>>DP Br IMX322 sensor ID = %d\r\n",SensorID));
    /* Dark environment */
    while(SensorID--)
    {
        CAL_DP_FMT* pDPFmt;
        CAL_DPC_HEADER Bright_Cnt;

        i = sizeof(SensorMode_DP)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
        while(i--)
        {
            debug_err(("// i = %d sensor id = %d\r\n",i,SensorID));
            if(SensorMode_DP[i][SensorID].SenMode != BYPASS)
            {
                BOOL bDpOverFlow = FALSE;
                CalBuf buf;
                //lock 3a,fd...etc.
                debug_err((">>Br lock AE\r\n"));
                AAA_SWITCH(FALSE,SensorID);
     debug_err((">>>lock AE\r\n"));
                if (DP_Status_Chk == _CAL_DP_D_OK)
                {
                    //read bright dp result
                    pTag = CAL_RW()->fpReadTag(SEC_DP, GetTagID(SEC_DP, SensorMode_DP[i][SensorID].SenMode));

                    debug_err(("--->mode %d, cal sts %x\r\n",SensorMode_DP[i][SensorID].SenMode,pTag->CAL_STATUS));
                    if(pTag == NULL || pTag->CAL_STATUS != _CAL_DP_D_OK)
                    {
                        DBG_ERR("DP tag read fail %d!!\r\n",pTag->CAL_STATUS);
                        return E_SYS;
                    }

                    CopyTag(TagTempAddr,pTag);
                    pTag = (pCAL_TAG)TagTempAddr;
                }
            debug_err(("// i = %d sensor id = %d\r\n",i,SensorID));
                //ipp buf init
                buf.Id = SensorMode_DP[i][SensorID].SenMode;
                buf.StartAddr = DmaAddr;
                buf.Size = DmaSize;
                CalApi()->fpBufEvent()->fpInit(buf);
                //init sensor info
                            debug_err(("cal sensor i = %d sensor id = %d\r\n",i,SensorID));
                CtrlPtr.pCal_Sensor = (void*)&SensorMode_DP[i][SensorID];
                Sensor_GetStatus(SensorID, SensorMode_DP[i][SensorID].SenMode, &Drv_Sensor);
                CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;
                            debug_err(("chg mode i = %d sensor id = %d\r\n",i,SensorID));
                //config target sensor mode
                IPC_WaitVD(TRUE,SensorID);
                Sensor_ChgMode(SensorID, SensorMode_DP[i][SensorID].SenMode);
                IPC_WaitVD(TRUE,SensorID);
                //get sensor info
                            debug_err(("status i = %d sensor id = %d\r\n",i,SensorID));
                Sensor_GetStatus(SensorID, SensorMode_DP[i][SensorID].SenMode, &Drv_Sensor);
debug_err((">>>>>DP Br IMX322 to capture\r\n"));
                //chg sie to capture mode
                CalApi()->fpSetMode(CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpDP_CtrlInit);
                Bayer = CalApi()->fpGetRawInfo();

                //cal 3a converage
                //ae
                aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,AESetting_DP_B_ADJ[SensorID],SensorID);
debug_err((">>>>>DP Br IMX322 AE end\r\n"));
                //pause sie
                CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);

                //run alg
                pDPFmt = (CAL_DP_FMT*)&pTag->TagData;

                if (DP_Status_Chk != _CAL_DP_D_OK)
                {
                    pDPFmt->DPCnt.PixelCnt = 0;
                }

                DP_Info.DPC.Bayer = Bayer.BaseInfo;
                DP_Info.DPC.DPAddr = (UINT32)&pDPFmt->DP_Pool;
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


//                CalApi()->fpSaveRAW(Bayer.BaseInfo.PixelAddr,Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height);
#if 1 //save RAW
{
    char    filename1[64];
    UINT32 size, fn = 0;
    FST_FILE    pFileHdl;
 DBG_ERR("save dp \r\n");

debug_err((">>>Loffset = %d H = %d\r\n",Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height));
 
    size = Bayer.BaseInfo.LineOffset*Bayer.BaseInfo.Height;

    //for ( fn = 0; fn < 1; fn ++ )
    {
        if (i == 0)
        sprintf(filename1, "A:\\DP_Bri0.RAW", fn);
        else
            sprintf(filename1, "A:\\DP_Bri1.RAW", fn);
        pFileHdl = FileSys_OpenFile(filename1,FST_CREATE_ALWAYS|FST_OPEN_WRITE);
        FileSys_WriteFile(pFileHdl, (UINT8 *)Bayer.BaseInfo.PixelAddr, &size, 0, NULL);
        FileSys_CloseFile(pFileHdl);
    }
}
#endif
debug_err((">>>>>DP Br IMX322 to alg\r\n"));
                Bright_Cnt.PixelCnt = CalApi()->fpAlgEvent()->fpDPC_Bayer(&DP_Info,&bDpOverFlow);
debug_err((">>>>>DP Br IMX322 end \r\n"));
                pDPFmt->DPCnt.PixelCnt += Bright_Cnt.PixelCnt;
                DBG_IND("DP Rst(SensorMode = %d): DP cnt= %d\r\n",SensorMode_DP[i][SensorID].SenMode,pDPFmt->DPCnt.PixelCnt);

                if(bDpOverFlow)
                {
                    Cal_Status = _CAL_OverFlow;
                    pTag->CAL_STATUS = _CAL_OverFlow;
                }
                else
                {
                    if (DP_Status_Chk == _CAL_DP_D_OK)
                    {
//                        Cal_Status = _CAL_OK;
                        pTag->CAL_STATUS = _CAL_OK;
                    }
                    else
                    {
                        if (Cal_Status != _CAL_OverFlow)
                            Cal_Status = _CAL_DP_B_OK;
                        pTag->CAL_STATUS = _CAL_DP_B_OK;
                    }
                }
debug_err((">>>>>DP Br IMX322 status\r\n"));
                if (pTag->CAL_STATUS == _CAL_OK)
                {
                    if ( pDPFmt->DPCnt.PixelCnt != 0)
                    {
                        //sort Bright & Dark DP.
                        CalApi()->fpAlgEvent()->fpDPC_Sort(&pDPFmt->DP_Pool,pDPFmt->DPCnt.PixelCnt);
                    }
                    pTag->TagDataLen = sizeof(CAL_DPC_HEADER) + CalApi()->fpAlgEvent()->fpDPC_ArgFmt(&pDPFmt->DP_Pool,pDPFmt->DPCnt.PixelCnt,&pDPFmt->DPCnt.LineCnt);
                }
                else
                {
                    pTag->TagDataLen = sizeof(CAL_DPC_HEADER) + sizeof(UINT32)*pDPFmt->DPCnt.PixelCnt;
                }

                pTag->TagID = (UINT32)GetTagID(SEC_DP, SensorMode_DP[i][SensorID].SenMode);

                DBG_IND("DP Rst(SensorMode = %d): DP cnt= %d, DATALen = %d\r\n",SensorMode_DP[i][SensorID].SenMode,pDPFmt->DPCnt.PixelCnt,pTag->TagDataLen);

                //write tag
                CAL_RW()->fpAddTag(SEC_DP,pTag);

                //release buf
                CalApi()->fpBufEvent()->fpUnInit(buf.Id);

//                sprintf((char*)m_CalMsg, "DP Rst(SensorMode:%d), DP cnt:%d",SensorMode_DP[i][SensorID].SenMode,pDPFmt->DPCnt.PixelCnt);
                if (pDPFmt->DPCnt.PixelCnt >= DP_MAX_CNT)
                {
                    CalStrColor = UICOL_RED;
                }
                else
                {
                    CalStrColor = UICOL_GREEN;
                }
                sprintf((char*)m_CalMsg, "MODE %d DP CNT:%d",SensorMode_DP[i][SensorID].SenMode,pDPFmt->DPCnt.PixelCnt);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);
            }
        }
    }

    if (Cal_Status == _CAL_OverFlow)
    {
        sprintf((char*)m_CalMsg, "DP Bright:  NG");
        CalStrColor = UICOL_RED;
    }
    else if(Cal_Status == _CAL_DP_B_OK)
    {
        sprintf((char*)m_CalMsg, "DP Bright:  PASS");
        CalStrColor = UICOL_GREEN;
    }
    else
    {
        sprintf((char*)m_CalMsg, "DP:  PASS");
        CalStrColor = UICOL_GREEN;
    }


    if(fpUIcb)
        fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);

    //close sie
    CalApi()->fpSetMode(CAL_IPL_SIECLOSE,NULL,NULL);

    DBG_ERR("\r\n-------------> cal sts %x, %x, %x",pTag->CAL_STATUS, DP_Status_Chk,Cal_Status);

    if (Cal_Status == _CAL_DP_B_OK || Cal_Status == _CAL_OK)
    {
        //write pstore
        CAL_RW()->fpSectionWrite(SEC_DP);
    }

#if 0
{
    pCAL_TAG pTag;
    CAL_DP_FMT* pFmt;
    CAL_RW()->fpSectionRead(SEC_DP,DmaAddr,DmaSize);
    pTag = CAL_RW()->fpReadTag(SEC_DP, GetTagID(SEC_DP, SensorMode_DP[++i][++SensorID].SenMode));
    pFmt = (CAL_DP_FMT*)&pTag->TagData;
    DBG_ERR("CHK: %d,%d,%x\r\n",pFmt->DPCnt.PixelCnt,pFmt->DPCnt.LineCnt,pFmt->DP_Pool);
}
#endif
    if(fpUIcb)
        fpUIcb(UICB_END,NULL,CalStrColor);
debug_err((">>>>>DP Br IMX322 end\r\n"));
    return E_OK;
}


static ER DP_Adjust(fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    DP_Adjust_Dark(fpUIcb,DmaAddr,DmaSize);

    DP_Adjust_Bright(fpUIcb,DmaAddr,DmaSize);

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

static ER AE_Check(fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    UINT32 i;
    UINT32 SensorID = SENSOR_ID_MAX_NUM;
    CAL_AE_RST aeRst;
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor;
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    CAL_AECHK_FMT* pAECHKFmt;

    CAL_RW()->fpDeleteSection(SEC_AECHK);

    sprintf((char*)m_CalMsg, "Set to LV15");
    if(fpUIcb)
        fpUIcb(UICB_START,(CHAR*)m_CalMsg,UICOL_GREEN);

    //stop pipeline ycc part
    CalApi()->fpSetMode(CAL_IPL_STOP,NULL,NULL);
    //pause sie
    CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);

    //allocate section buffer
    dmaRst = CfgSectionBuf(SEC_AECHK,DmaAddr,DmaSize);
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
    pTag->TagDataLen = sizeof(CAL_AECHK_FMT);
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
        i = sizeof(SensorMode_AE_Check)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
        while(i--)
        {
            if(SensorMode_AE_Check[i][SensorID].SenMode != BYPASS)
            {
                CalBuf buf;
                //lock 3a,fd...etc.
                AAA_SWITCH(TRUE,SensorID);
                //ipp buf init
                buf.Id = SensorMode_AE_Check[i][SensorID].SenMode;
                buf.StartAddr = DmaAddr;
                buf.Size = DmaSize;
                CalApi()->fpBufEvent()->fpInit(buf);
                //init sensor info
                CtrlPtr.pCal_Sensor = (void*)&SensorMode_AE_Check[i][SensorID];
                Sensor_GetStatus(SensorID, SensorMode_AE_Check[i][SensorID].SenMode, &Drv_Sensor);
                CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;
                //config target sensor mode
                IPC_WaitVD(TRUE,SensorID);
                Sensor_ChgMode(SensorID, SensorMode_AE_Check[i][SensorID].SenMode);
                IPC_WaitVD(TRUE,SensorID);
                //get sensor info
                Sensor_GetStatus(SensorID, SensorMode_AE_Check[i][SensorID].SenMode, &Drv_Sensor);

                //chg sie to capture mode
                CalApi()->fpSetMode(CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpISO_CtrlInit);
                Bayer = CalApi()->fpGetRawInfo();

                //cal 3a converage
                //ae
                aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,AESetting_AE_Check[SensorID],SensorID);

                //pause sie
                CalApi()->fpSetMode(CAL_IPL_SIESTOP,NULL,NULL);

                //run alg
                pAECHKFmt = (CAL_AECHK_FMT*)&pTag->TagData;
                pAECHKFmt->ChkStatus = aeRst.status;

                pTag->CAL_STATUS = _CAL_OK;

                if(aeRst.status == CAL_AE_STABLE)
                {
                    gCalIQData.Cal_Status.AECHK_STATUS = _CAL_OK;
                    sprintf((char*)m_CalMsg, "AE CHECK OK");
                    CalStrColor = UICOL_GREEN;
                }
                else
                {
                    pTag->CAL_STATUS = _CAL_NONE;
                    gCalIQData.Cal_Status.AECHK_STATUS = _CAL_NONE;
                    sprintf((char*)m_CalMsg, "AE CHECK NG");
                    CalStrColor = UICOL_RED;
                }

                pTag->TagDataLen = sizeof(CAL_AECHK_FMT);
                pTag->TagID = (UINT32)GetTagID(SEC_AECHK, SensorMode_AE_Check[i][SensorID].SenMode);
                //write tag
                CAL_RW()->fpAddTag(SEC_AECHK,pTag);
                //release buf
                CalApi()->fpBufEvent()->fpUnInit(buf.Id);

                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);
            }
        }
    }
    //close sie
    CalApi()->fpSetMode(CAL_IPL_SIECLOSE,NULL,NULL);

    //write pstore
    CAL_RW()->fpSectionWrite(SEC_AECHK);

    if(fpUIcb)
        fpUIcb(UICB_END,NULL,CalStrColor);
    return E_OK;
}

static BOOL GetAdjustRst(fpUI_cb fpUIcb, CalIQInfo *CalData)
{
    CAL_TAG* pTag = {0};
    UINT32 i;
    UINT32 SensorID = SENSOR_ID_MAX_NUM;
    BOOL Status_chk = TRUE;
    CAL_ISO_FMT* pISOFmt;
    CAL_AWB_FMT* pAWBFmt;
    CAL_AECHK_FMT* pAECHKFmt;

//check DP Adjust
    gCalIQData.Cal_Status.DP_STATUS = _CAL_DP_D_OK;
    while(SensorID--)
    {
        i = sizeof(SensorMode_DP)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
        while(i--)
        {
            if(SensorMode_DP[i][SensorID].SenMode != BYPASS)
            {
                pTag = GetCalData(SEC_DP,SensorMode_DP[i][SensorID].SenMode);
                if(pTag)
                {
                    if((pTag->CAL_STATUS != _CAL_DP_D_OK) || (pTag->CAL_STATUS != _CAL_OK) || (pTag->CAL_STATUS != _CAL_DP_B_OK))
                    {
                        gCalIQData.Cal_Status.DP_STATUS = pTag->CAL_STATUS;
                        Status_chk = FALSE;
                    }
                }
                else
                {
                    gCalIQData.Cal_Status.DP_STATUS = _CAL_NONE;
                    Status_chk = FALSE;
                }
            }
        }
    }

    if (Status_chk == TRUE)
    {
        gCalIQData.Cal_Status.DP_STATUS = pTag->CAL_STATUS;
    }

//check ECS Adjust
    SensorID = SENSOR_ID_MAX_NUM;
    gCalIQData.Cal_Status.ECS_STATUS = _CAL_OK;
    while(SensorID--)
    {
        i = sizeof(SensorMode_ECS)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
        while(i--)
        {
            if(SensorMode_ECS[i][SensorID].SenMode != BYPASS)
            {
                pTag = GetCalData(SEC_ECS,SensorMode_ECS[i][SensorID].SenMode);
                if(pTag)
                {
                    if(pTag->CAL_STATUS != _CAL_OK)
                    {
                        gCalIQData.Cal_Status.ECS_STATUS = pTag->CAL_STATUS;
                        Status_chk = FALSE;
                    }
                }
                else
                {
                    gCalIQData.Cal_Status.ECS_STATUS = _CAL_NONE;
                    Status_chk = FALSE;
                }
            }
        }
    }

//check ISO Adjust
    SensorID = SENSOR_ID_MAX_NUM;
    gCalIQData.Cal_Status.ISO_STATUS = _CAL_OK;
    gCalIQData.Cal_ISO_Base = 0;

    while(SensorID--)
    {
        i = sizeof(SensorMode_ISO)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
        while(i--)
        {
            if(SensorMode_ISO[i][SensorID].SenMode != BYPASS)
            {
                pTag = GetCalData(SEC_ISO,SensorMode_ISO[i][SensorID].SenMode);

                if (pTag != NULL)
                {
                    pISOFmt = (CAL_ISO_FMT*)&pTag->TagData;
                    if(pTag->CAL_STATUS != _CAL_OK)
                    {
                        gCalIQData.Cal_Status.ISO_STATUS = _CAL_NONE;
                        Status_chk = FALSE;
                    }
                    gCalIQData.Cal_ISO_Base = pISOFmt->GainBase;
                }
                else
                {
                    gCalIQData.Cal_Status.ISO_STATUS = _CAL_NONE;
                    Status_chk = FALSE;
                }
            }
        }
    }

//check AE Adjust
    SensorID = SENSOR_ID_MAX_NUM;
    gCalIQData.Cal_Status.AECHK_STATUS = _CAL_NONE;

    while(SensorID--)
    {
        i = sizeof(SensorMode_AE_Check)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
        while(i--)
        {
            if(SensorMode_AE_Check[i][SensorID].SenMode != BYPASS)
            {
                pTag = GetCalData(SEC_AECHK,SensorMode_AE_Check[i][SensorID].SenMode);
                if (pTag != NULL)
                {
                    if(pTag->CAL_STATUS == _CAL_OK)
                    {
                        pAECHKFmt = (CAL_AECHK_FMT*)&pTag->TagData;
                        if (pAECHKFmt->ChkStatus == CAL_AE_STABLE)
                        {
                            gCalIQData.Cal_Status.AECHK_STATUS = _CAL_OK;
                        }
                    }
                }
            }
        }
    }

//check AWB Adjust
    SensorID = SENSOR_ID_MAX_NUM;
    gCalIQData.Cal_Status.AWB_STATUS = _CAL_OK;

    gCalIQData.Cal_AWB_Rg = 0;
    gCalIQData.Cal_AWB_Bg = 0;

    while(SensorID--)
    {
        i = sizeof(SensorMode_AWB)/(sizeof(Cal_SENSOR_INFO)*SENSOR_ID_MAX_NUM);
        while(i--)
        {
            if(SensorMode_AWB[i][SensorID].SenMode != BYPASS)
            {
                pTag = GetCalData(SEC_AWB,SensorMode_AWB[i][SensorID].SenMode);

                if (pTag != NULL)
                {
                    pAWBFmt = (CAL_AWB_FMT*)&pTag->TagData;
                    if(pTag->CAL_STATUS != _CAL_OK)
                    {
                        gCalIQData.Cal_Status.AWB_STATUS = _CAL_NONE;
                        Status_chk = FALSE;
                    }
                    gCalIQData.Cal_AWB_Rg = pAWBFmt->Rg;
                    gCalIQData.Cal_AWB_Bg = pAWBFmt->Bg;
                }
                else
                {
                    gCalIQData.Cal_Status.AWB_STATUS = _CAL_NONE;
                    Status_chk = FALSE;
                }
            }
        }
    }

    sprintf((char*)m_CalMsg, "DNP Adjust Status");
    if(fpUIcb)
        fpUIcb(UICB_START,(CHAR*)m_CalMsg,UICOL_GREEN);

    if ((gCalIQData.Cal_Status.DP_STATUS == _CAL_DP_D_OK) || (gCalIQData.Cal_Status.DP_STATUS == _CAL_OK))
    {
        sprintf((char*)m_CalMsg, "OK   DP Dark Adjust");
        CalStrColor = UICOL_GREEN;
    }
    else
    {
        sprintf((char*)m_CalMsg, "NG   DP Dark Adjust");
        CalStrColor = UICOL_RED;
    }

    if(fpUIcb)
        fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);


    if (gCalIQData.Cal_Status.ECS_STATUS == _CAL_OK)
    {
        sprintf((char*)m_CalMsg, "OK   ECS Adjust");
        CalStrColor = UICOL_GREEN;
    }
    else
    {
        sprintf((char*)m_CalMsg, "NG   ECS Adjust");
        CalStrColor = UICOL_RED;
    }

    if(fpUIcb)
        fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);


    if (gCalIQData.Cal_Status.ISO_STATUS == _CAL_OK)
    {
        sprintf((char*)m_CalMsg, "OK   ISO Adjust");
        CalStrColor = UICOL_GREEN;
    }
    else
    {
        sprintf((char*)m_CalMsg, "NG   ISO Adjust");
        CalStrColor = UICOL_RED;
    }

    if(fpUIcb)
        fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);

    if (gCalIQData.Cal_Status.AWB_STATUS == _CAL_OK)
    {
        sprintf((char*)m_CalMsg, "OK   AWB Adjust");
        CalStrColor = UICOL_GREEN;
    }
    else
    {
        sprintf((char*)m_CalMsg, "NG   AWB Adjust");
        CalStrColor = UICOL_RED;
    }

    if(fpUIcb)
        fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);

    if(fpUIcb)
        fpUIcb(UICB_END,NULL,CalStrColor);

    *CalData = gCalIQData;

    return Status_chk;
}

#if 0
#endif
CAL_ITEM_OBJ m_CalItem = {
    ISO_Adjust,
    AWB_Adjust,
    AWBGS_Adjust,
    Mshutter_Adjust,
    ECS_Adjust,
    DP_Adjust,
    DP_Adjust_Bright,
    DP_Adjust_Dark,
    AE_Check,
    GetAdjustRst,
};


pCAL_ITEM_OBJ CAL_ITEM_IMX322_EVB_FF(void)
{
    //release buf used by set2preview first
    CalApi()->fpBufEvent()->fpUnInit(IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));
    return &m_CalItem;
}

static void Cal_Display_cfg(UINT32 ImgIdx,IPL_YCC_IMG_INFO* path1,IPL_YCC_IMG_INFO* path2,IPL_YCC_IMG_INFO* path3,fpCallBack fp_CB)
{
    DISPLAYER_PARAM LyrParam;
    DISP_OBJ* pDispObj=disp_getDisplayObject(DISP_1);

    static UINT32 cnt = 0;

    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));

    LyrParam.SEL.SET_MODE.BufFormat = DISPBUFFORMAT_YUV420PACK;

    LyrParam.SEL.SET_MODE.BufMode = 0; //don't care
    LyrParam.SEL.SET_MODE.BufNumber = 0; //don't care
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1,DISPLAYER_OP_SET_MODE,&LyrParam);


    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));
    LyrParam.SEL.SET_BUFSIZE.uiBufWidth = path2->Ch[0].Width;
    LyrParam.SEL.SET_BUFSIZE.uiBufHeight = path2->Ch[0].Height;
    LyrParam.SEL.SET_BUFSIZE.uiBufLineOfs = path2->Ch[0].LineOfs;
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1,DISPLAYER_OP_SET_BUFSIZE,&LyrParam);

    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));
    LyrParam.SEL.SET_VDOBUFADDR.BufSel = DISPBUFADR_0;
    LyrParam.SEL.SET_VDOBUFADDR.uiAddrY0 = path2->PixelAddr[0];
    LyrParam.SEL.SET_VDOBUFADDR.uiAddrCb0 = path2->PixelAddr[1];
    LyrParam.SEL.SET_VDOBUFADDR.uiAddrCr0 = path2->PixelAddr[2];
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1,DISPLAYER_OP_SET_VDOBUFADDR,&LyrParam);

    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));
    LyrParam.SEL.SET_BUFXY.uiBufOfsX = 0;
    LyrParam.SEL.SET_BUFXY.uiBufOfsY = 0;
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1,DISPLAYER_OP_SET_BUFXY,&LyrParam);

    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));
    LyrParam.SEL.SET_WINSIZE.iWinOfsX = 0;
    LyrParam.SEL.SET_WINSIZE.iWinOfsY = 0;
    LyrParam.SEL.SET_WINSIZE.uiWinWidth = path2->Ch[0].Width;
    LyrParam.SEL.SET_WINSIZE.uiWinHeight = path2->Ch[0].Height;
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1,DISPLAYER_OP_SET_WINSIZE,&LyrParam);


    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));
    LyrParam.SEL.SET_OUTDIR.BufOutDir = DISPOUTDIR_NORMAL;
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1,DISPLAYER_OP_SET_OUTDIR,&LyrParam);
    pDispObj->load(TRUE);

    cnt++;
/*
    if (cnt%3 == 0)
    {
        CalApi()->fpBufEvent()->fpGetAddr(IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE),ime_p2_y_0,
        &a,NULL);
        CalApi()->fpBufEvent()->fpGetAddr(IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE),ime_p2_uv_0,
        &b,NULL);
    }
    else if (cnt%3 == 1)
    {
        CalApi()->fpBufEvent()->fpGetAddr(IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE),ime_p2_y_1,
        &a,NULL);
        CalApi()->fpBufEvent()->fpGetAddr(IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE),ime_p2_uv_1,
        &b,NULL);
    }
    else
    {
        CalApi()->fpBufEvent()->fpGetAddr(IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE),ime_p2_y_2,
        &a,NULL);
        CalApi()->fpBufEvent()->fpGetAddr(IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE),ime_p2_uv_2,
        &b,NULL);
    }
    path2->PixelAddr[0] = a;
    path2->PixelAddr[1] = b;
    path2->PixelAddr[2] = b;
    */
}

/**
    this preview have no calibration result.
*/
ER Set2Preview_IMX322_EVB_FF(UINT32 BufAddr,UINT32 BufSize)
{
    CalBuf buf = {0};
    SENSOR_INFO Drv_Sensor;
    CTRL_PTR CtrlInfo = {0};
    ER ErrorCode = E_OK;
    UINT32 i;
    IPL_RAW_IMG BayerInfo;
    IPL_AlgSetUIInfo(IPL_SEL_VIDEO_SIZE, SensorMode[0][SENSOR_ID_1].SenMode);
    
    IPL_AlgSetUIInfo(IPL_SEL_PRVSENMODE, SensorMode[0][SENSOR_ID_1].SenMode);
    //init buf for preview only
    buf.Id = IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE);
    buf.StartAddr = BufAddr;
    buf.Size = BufSize;
    ErrorCode = CalApi()->fpBufEvent()->fpInit(buf);
    DBG_WRN("need to review : preview support only 1 sensor ...\r\n");

    debug_err(("???? set sensor mode = %d\r\n",IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE)));
    //set sensor mode
    Sensor_ChgMode(SENSOR_ID_1,IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));

    Sensor_GetStatus(SENSOR_ID_1, IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &Drv_Sensor);
    /*
        TODO: need to add displayer control
    */
    CtrlInfo.fpRdyImg = Cal_Display_cfg;
    DBG_WRN("need to add displayer control\r\n");
    CtrlInfo.pDrv_Sensor = (void*)&Drv_Sensor;
    IPL_ResetDisplayCtrlFlow();
    ide_enableVideo(IDE_VIDEOID_1);


        //get calibration info mapping to current sensor mode
        i = CalSensorModeNum;
        while(i--)
        {
            if(SensorMode[i][SENSOR_ID_1].SenMode == IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE))
            {
                debug_err((">>>sensor mode idx = %d\r\n",i));
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
debug_err((">>>>set2preview imx322 evb ff end\r\n"));
    //AE_WaitStable(3);
    return ErrorCode;
}
