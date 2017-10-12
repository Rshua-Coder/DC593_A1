#include <string.h>
#include <stdio.h>
#include "GxDisplay.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIPrinterObj.h"
//#include "UIMenuPrinterSettings.h"
#include "debug.h"
#include "PlaybackTsk.h"
//#include "GlobalVar.h"
#include "DPOF.h"
#include "DPSFuncs.h"
#include "DCF.h"
#include "PrjCfg.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIPrinterObj
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


#define NVT_PRT_DATA_MAX     100
#define NVT_PRT_MAX_CNT         10000    //#NT#2010/02/10#Lily Kao


_ALIGNED(4) static DPS_INIT_PARAMS gDPSGetCapInfo;

//static DPS_PRINT_PARAMS g_MultiPrintInfo[PB_PRINT_DATA_MAX];
static PB_PRINT_INFO        gPBPrintInfo = {0, 0, 0, 0, 0};
static PB_PRINT_DATA        gPBPrintData[NVT_PRT_DATA_MAX];


#define _UIPRT_CHG_IDXPRT_2_MULTIPLEPRT_  0
#if (_UIPRT_CHG_IDXPRT_2_MULTIPLEPRT_)
//#NT#2008/12/23#Lily Kao -begin
//Change multiple-copies Idx-Prt to multiple single-copy Idx-Prt
PB_PRINT_DATA   gPBPrintDataBackUp[NVT_PRT_DATA_MAX] = {0};
#endif

static UINT32 g_uiStatus;
static UINT32 g_uiPrtAllNum = 0;
static UINT32 g_uiPrtCurMode = PMODE_SINGLE;
static UINT32 g_uiPrtPaperSize = DPS_CAP_PAPERSIZE_DEF;
static UINT32 g_uiPrtLayout = DPS_CAP_LAYOUT_DEF;
//#NT#2009/12/04#Lily Kao - begin
static UINT32 g_uiPrtAllDate = DPS_CAP_DATEPRINT_DEF;
static UINT32 g_uiPrtSingleDate = DPS_CAP_DATEPRINT_DEF;
//#NT#2009/12/04#Lily Kao - end

//XXXXXXXXXXXXXXX 2009/05/26 Jeah Yen - begin
PRT_INITIAL g_PrtInit={0,0};


//#NT#2010/09/29#Lily Kao -begin
//#Move the configuration of Picture-Display(PLAY_OBJ) to Project level
PLAY_OBJ    gPrtObjPlayObjInfo;
PRINTOBJ_SET_PLAYOBJINFO_FUNC gpPrtObjSetPlayObjFunc = 0;

void AppPrint_RegSetPlayObjCB(PRINTOBJ_SET_PLAYOBJINFO_FUNC pFunc)
{
    gpPrtObjSetPlayObjFunc = pFunc;
}
//#NT#2010/09/29#Lily Kao -end


void AppPrint_Init(PRT_INITIAL* pInit)
{
    g_PrtInit = *pInit;
}
//XXXXXXXXXXXXXXX 2009/05/26 Jeah Yen - end
#if 0
BOOL GetPaperSizeCapByIndex(UINT32 index)
{
    DBG_DUMP("[GetPaperSizeCapByIndex] gDPSGetCapInfo.paperSizes = 0X%X\r\n",gDPSGetCapInfo.paperSizes);
    if(index >= PSIZE_NUM)
    {
        DBG_ERR("[GetPaperSizeCapByIndex] Parameter Error!!! index should smaller than %d\r\n",PSIZE_NUM);
        return FALSE;
    }
    else if((gDPSGetCapInfo.paperSizes& g_uiPrtPaperSize))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

BOOL GetLayoutTypeCapByIndex(UINT32 index)
{
    DBG_DUMP("[GetLayoutTypeCapByIndex] gDPSGetCapInfo.layouts = 0X%X\r\n",gDPSGetCapInfo.layouts);
    if(index >= PLAYOUT_NUM)
    {
        DBG_ERR("[GetLayoutTypeCapByIndex] Parameter Error!!! index should smaller than %d\r\n",PLAYOUT_NUM));
        return FALSE;
    }
    else if((gDPSGetCapInfo.layouts& g_uiPrtLayout))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#endif
//#NT#2008/12/23#Lily Kao -begin
//Change multiple-copies Idx-Prt to multiple single-copy Idx-Prt
/*
    If it is index print, turn n-copies to n-elements
    針對一張圖片多張copies索引列印，將索引列印轉換為多個單張列印

    @Refer Novatek Solution.

    @Param
        UINT32 *pImgCnt: total image count to be printed.
        PB_PRINT_DATA *pPrtData: used to store image data to be printed.
    @Return
        void.

*/
#if (_UIPRT_CHG_IDXPRT_2_MULTIPLEPRT_)
static void nvtPrt_chgMultiCopyIdxPrt2SingleCopy(PB_PRINT_DATA *pPrtData, UINT32 *pImgCnt)
{
    UINT32 entryCnt, imgCnt;
    UINT16 tmpCopies = 0;
    UINT32 totalImages = *pImgCnt;


    if (!pPrtData || (totalImages > NVT_PRT_DATA_MAX))
    {
        DBG_ERR("#nvtPrt_chgMultiCopyIdxPrt2SingleCopy:PrtImg over NVT_PRT_DATA_MAX=%d\r\n", NVT_PRT_DATA_MAX);
        return;
    }

    entryCnt = imgCnt = 0;
    while ((imgCnt < totalImages) && (entryCnt < NVT_PRT_DATA_MAX))
    {
        tmpCopies = pPrtData->quantity;
        pPrtData->quantity = 1;
        while (tmpCopies && (entryCnt < NVT_PRT_DATA_MAX))
        {
            memcpy((void *)&gPBPrintDataBackUp[entryCnt], pPrtData, sizeof(PB_PRINT_DATA));
            entryCnt ++;
            tmpCopies --;
        }
        while (entryCnt < NVT_PRT_DATA_MAX)
        {
            memset((void *)&gPBPrintDataBackUp[entryCnt], 0, sizeof(PB_PRINT_DATA));
            entryCnt ++;
        }
        pPrtData ++;
        imgCnt ++;
    }

    if (entryCnt)
    {
        memcpy((void *)&gPBPrintData[0], (void *)&gPBPrintDataBackUp[0],entryCnt*sizeof(PB_PRINT_DATA));
        *pImgCnt = entryCnt;
    }
    else
    {
        DBG_ERR("#nvtPrt_chgMultiCopyIdxPrt2SingleCopy:No Change\r\n");
    }

}
#endif
//#NT#2008/12/23#Lily Kao -end

static UINT16 nvtPrt_CheckFilePrint(UINT16 dirID, UINT16 fileID, UINT8 *qua, UINT8 *datePrt, UINT8 *filePrt)
{
    UINT16 i;

    for (i = 0; i<gPBPrintInfo.total_images; i++)
    {
        if ((gPBPrintData[i].dirID == dirID)&&(gPBPrintData[i].fileID == fileID))
        {
            *qua = gPBPrintData[i].quantity;
            *datePrt = gPBPrintData[i].datePrints;
            *filePrt = gPBPrintData[i].filenamePrints;
            return i;
        }
    }
    *qua =0;
    *datePrt = 0;
    *filePrt = 0;
    return i;
}

//#NT#2009/12/04#Lily Kao - begin
static UINT8 PrtCheckThisFileDate(void)
{
    UINT16  uiFileID, uiDirID;
    UINT8    qua = 0, datePrt = 0, namePrt = 0;

    uiFileID = DCF_GetDBInfo(DCF_INFO_CUR_FILE_ID);
    uiDirID = DCF_GetDBInfo(DCF_INFO_CUR_DIR_ID);

    if (uiDirID && uiFileID)
        nvtPrt_CheckFilePrint(uiDirID, uiFileID, &qua, &datePrt, &namePrt);

    return datePrt;
}
//#NT#2009/12/04#Lily Kao - end

static void nvtPrt_SetFile(UINT16 dirID, UINT16 fileID, UINT8 qut, UINT8 namePrt, UINT8 datePrt)
{
    UINT16  totalImages, totalPrints, i, j;
    UINT8   findOldOrNot =0, newPrint=0, deletePrint =0;
    //#NT#2008/04/03#Lily Kao -begin
    //cropping test only
    #if _DPS_CROPPING_
    UINT16 cropX=0, cropY=0, cropW=0, cropH=0;
    #endif
    //#NT#2008/04/03#Lily Kao -end

    totalImages = gPBPrintInfo.total_images;
    totalPrints = gPBPrintInfo.total_prints;

    for (i=0;i<=totalImages; i++)
    {
        if ((gPBPrintData[i].dirID == dirID)&&(gPBPrintData[i].fileID == fileID))
        {
            findOldOrNot =1;
            if (qut ==0)
            {
                for (j=i+1;j<totalImages; j++)
                {
                    memcpy(&gPBPrintData[j-1], &gPBPrintData[j], sizeof(PB_PRINT_DATA));
                }
                memset(&gPBPrintData[totalImages-1], 0, sizeof(PB_PRINT_DATA));
                deletePrint =1;
            }
            else
            {
                gPBPrintData[i].quantity = qut;
                gPBPrintData[i].filenamePrints= namePrt;
                gPBPrintData[i].datePrints = datePrt;
            }
        }
    }

    if ((findOldOrNot ==0)&&(qut !=0))
    {
        gPBPrintData[totalImages].dirID = dirID;
        gPBPrintData[totalImages].fileID = fileID;
        gPBPrintData[totalImages].quantity = qut;
        gPBPrintData[totalImages].filenamePrints= namePrt;
        gPBPrintData[totalImages].datePrints = datePrt;
        //#NT#2008/04/03#Lily Kao -begin
        //cropping test only
        #if _DPS_CROPPING_
        gPBPrintData[totalImages].cropX = cropX;
        gPBPrintData[totalImages].cropY = cropY;
        gPBPrintData[totalImages].cropW = cropW;
        gPBPrintData[totalImages].cropH = cropH;
        #endif
        //#NT#2008/04/03#Lily Kao -end

        newPrint =1;
    }
    totalPrints =0;
    for (i =0; i<=totalImages; i++)
    {
        totalPrints += gPBPrintData[i].quantity;
    }

    if ((totalImages < NVT_PRT_DATA_MAX-1)&&(newPrint==1))
    {
        totalImages +=1;
    }
    else if (deletePrint ==1)
    {
        if (totalImages>0)
            totalImages -=1;
    }

    gPBPrintInfo.total_images = totalImages;
    gPBPrintInfo.total_prints = totalPrints;

}

static void PrtSavePrintSet(UINT8 copy)
{
    UINT16 fId, dId;

    fId = DCF_GetDBInfo(DCF_INFO_CUR_FILE_ID);
    dId = DCF_GetDBInfo(DCF_INFO_CUR_DIR_ID);

    nvtPrt_SetFile(dId, fId, copy, \
        DPS_CAP_NAMEPRINT_OFF, \
           g_uiPrtSingleDate);
}

static UINT8 PrtCheckThisFilePrint(void)
{
    UINT32  uiFileID, uiDirID;
    UINT8    qua = 0, datePrt = 0, namePrt = 0;

    uiFileID = DCF_GetDBInfo(DCF_INFO_CUR_FILE_ID);
    uiDirID = DCF_GetDBInfo(DCF_INFO_CUR_DIR_ID);

    if (uiDirID && uiFileID)
        nvtPrt_CheckFilePrint(uiDirID, uiFileID, &qua, &datePrt, &namePrt);

    return qua;
}

static UINT16 PrtGetCurTotalPrints(UINT8 PrintMode)
{
    UINT32 totalPrints = 0;
    UINT32 startAddr, totalImages = 0;
    INT32 tmpNum;

    if(PrintMode==PMODE_SINGLE)
    {
        totalPrints = gPBPrintInfo.total_prints;
        totalImages = gPBPrintInfo.total_images;

        if (totalPrints > NVT_PRT_MAX_CNT)
        {
            tmpNum = (INT32)(totalImages - 1);
            totalPrints = totalImages = 0;

            for(;tmpNum >= 0; tmpNum--)
            {
                totalPrints += gPBPrintData[tmpNum].quantity;

                if (totalPrints > NVT_PRT_MAX_CNT)
                {
                    totalPrints -= gPBPrintData[tmpNum].quantity;
                    break;
                }
                totalImages ++;
            }
            tmpNum ++;
            DBG_ERR("   nvtPrt_GetCurTotalPrints,Over 500: totalImages=%x, totalPrints=%x, tmpNum=%x\r\n", totalImages, totalPrints, tmpNum);
        }
    }
    else if(PrintMode==PMODE_ALL)
    {
        Sidc_GetPrintAllData(&startAddr, &totalImages);
        totalPrints = totalImages * g_uiPrtAllNum;

        if (totalPrints > NVT_PRT_MAX_CNT)
        {
            tmpNum = totalImages;
            totalImages = (NVT_PRT_MAX_CNT/g_uiPrtAllNum);
            totalPrints = totalImages * g_uiPrtAllNum;
            tmpNum -= totalImages;
            startAddr += (tmpNum * sizeof(PB_PRINT_DATA));
            DBG_ERR("   nvtPrt_GetCurTotalPrints,Over 500: Copies=%x,totalPrints=%x, totalImages=%x, startAddr=%x\r\n", g_uiPrtAllNum, totalPrints, totalImages, startAddr);
        }
    }
    else if(PrintMode==PMODE_DPOF)
    {
        totalPrints = DPOF_GetTotalPrints();
    }
    DBG_DUMP("### PrintMode=%d, totalImages=%d, totalPrints=%d ###\r\n",PrintMode, totalImages, totalPrints);

    return totalPrints;
}

#if 0
INT32 OnExe_DateOn(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_MSG("OnExe_DateOn uhSelect=%d\r\n",uhSelect);
    Set_PrinterDate(uhSelect);
    return NVTEVT_CONSUME;
}

INT32 OnExe_PaperSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_MSG("OnExe_PaperSize uhSelect=%d\r\n",uhSelect);
    Set_PrinterSize(uhSelect);
    Set_PrinterType(PLAYOUT_DEFAULT);
    gDPSGetCapInfo.layouts = PictB_GetLayoutsSpecific(g_uiPrtPaperSize);
    return NVTEVT_CONSUME;
}

INT32 OnExe_Layout(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_MSG("OnExe_Layout uhSelect=%d\r\n",uhSelect);
    Set_PrinterType(uhSelect);
    return NVTEVT_CONSUME;
}

INT32 OnExe_PrtQuality(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_MSG("OnExe_Quality uhSelect=%d\r\n",uhSelect);
    Set_PrinterQuality(uhSelect);
    return NVTEVT_CONSUME;
}

INT32 OnExe_PrtNamePrint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_MSG("OnExe_NamePrint uhSelect=%d\r\n",uhSelect);
    Set_PrinterNamePrint(uhSelect);
    return NVTEVT_CONSUME;
}
#endif

INT32 UIPrint_Init(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //BKG_PostEvent(NVTEVT_BKW_WAIT_PTR_INIT);
    return NVTEVT_CONSUME;
}
//#NT#2010/09/29#Lily Kao -begin
//#Move the configuration of Picture-Display(PLAY_OBJ) to Project level
INT32 UIPrint_EnterPB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
//XXXXXXXXXXXXXXX 2009/05/26 Jeah Yen - begin
    if((g_PrtInit.uiBufAddr == 0)
    || (g_PrtInit.uiBufSize == 0))
        DBG_ERR("UIPrint_EnterPB Error! uiBufAddr=%08x uiBufSize=%08x\r\n", g_PrtInit.uiBufAddr, g_PrtInit.uiBufSize);

    memset(&gPrtObjPlayObjInfo, 0, sizeof(PLAY_OBJ));
    if (gpPrtObjSetPlayObjFunc)
    {
        gpPrtObjSetPlayObjFunc(&gPrtObjPlayObjInfo);
    }
    else
    {
        gPrtObjPlayObjInfo.uiPlayFileFmt = PBFMT_JPG;
    }
    gPrtObjPlayObjInfo.uiMemoryAddr        = g_PrtInit.uiBufAddr;
    gPrtObjPlayObjInfo.uiMemorySize        = g_PrtInit.uiBufSize;
    g_uiStatus = PB_Open(&gPrtObjPlayObjInfo);
    DBG_MSG("UIPrint_EnterPB g_uiStatus=%d\r\n", g_uiStatus);
    PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
}
//#NT#2010/09/29#Lily Kao -end
INT32 UIPrint_CloseSession(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    FLGPTN uiUSBFlag;

    DBG_MSG("+UIPrint_CloseSession\r\n");
    PB_Close(PB_WAIT_INFINITE);

    //clr_flg(FLG_DPS, FLGDPS_INITOK|FLGDPS_INITFAIL);
    PictB_ClrFlag(FLGDPS_INITOK|FLGDPS_INITFAIL);
    DBG_MSG("+wait init-ok\r\n");
    //wai_flg(&uiUSBFlag, FLG_DPS, FLGDPS_INITOK|FLGDPS_INITFAIL, TWF_ORW|TWF_CLR);
    PictB_WaiFlg(&uiUSBFlag, FLGDPS_INITOK|FLGDPS_INITFAIL, TWF_ORW|TWF_CLR);
    DBG_MSG("-wait init-ok:%d\r\n",uiUSBFlag);

    if (uiUSBFlag & FLGDPS_INITFAIL)
    {
        DBG_ERR("[UIPrint_CloseSession]FLGDPS_INITFAIL\r\n");
    }
    //#NT#2010/02/22#Lily Kao -begin
    else
    {
        PictB_GetCapabilityItem(&gDPSGetCapInfo);
        Ux_SendEvent(&UIPrinterObjCtrl,NVTEVT_PRTENTERPB,0);
        if(AppPrint_GetData(PRT_PBSTATUS) == E_OK)
        {
            DBG_DUMP("CloseSession: Playback Open OK\r\n");
        }
        else
        {
            DBG_ERR("CloseSession: Playback Open Fail\r\n");
        }
    }
    //#NT#2010/02/22#Lily Kao -end
    return NVTEVT_CONSUME;
}

INT32 UIPrint_CapsInit(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PictB_GetCapabilityItem(&gDPSGetCapInfo);
    return NVTEVT_CONSUME;
}


INT32 UIPrint_ResetData(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    memset((UINT8 *)&gPBPrintInfo, 0, sizeof(PB_PRINT_INFO));
    memset((UINT8 *)&gPBPrintData[0], 0, NVT_PRT_DATA_MAX * sizeof(PB_PRINT_DATA));
    return NVTEVT_CONSUME;
}

INT32 UIPrint_StartPrint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 totalPrints = 0;
    UINT32 startAddr, totalImages;

    INT32 tmpNum;
    UINT16 i=0;//Terry

    //case 0:Print Set-Copies Images
    if(g_uiPrtCurMode==PMODE_SINGLE)
    {
        totalPrints = gPBPrintInfo.total_prints;
        totalImages = gPBPrintInfo.total_images;
        DBG_DUMP("   Print: totalImages=%x, totalPrints=%x\r\n", totalImages, totalPrints);

        if (totalPrints > NVT_PRT_MAX_CNT)
        {
            tmpNum = (INT32)(totalImages - 1);
            totalPrints = totalImages = 0;

            for(;tmpNum >= 0; tmpNum--)
            {
                totalPrints += gPBPrintData[tmpNum].quantity;

                if (totalPrints > NVT_PRT_MAX_CNT)
                {
                    totalPrints -= gPBPrintData[tmpNum].quantity;
                    break;
                }

                totalImages ++;
            }

            tmpNum ++;
            DBG_ERR("   Over 500: totalImages=%x, totalPrints=%x, tmpNum=%x\r\n", totalImages, totalPrints, tmpNum);
        }
        else
        {
            tmpNum = 0;
        }
        //#NT#2009/12/04#Lily Kao - begin
        if(g_uiPrtAllDate != DPS_CAP_DATEPRINT_DEF)
        {
            DBG_DUMP("UIPrtObj,startprint,allDate=%d\r\n", g_uiPrtAllDate);
            for(i = 0; i < totalImages; i++)
            {
                gPBPrintData[i].datePrints = g_uiPrtAllDate;
            }
        }
        //#NT#2009/12/04#Lily Kao - end
        if (totalPrints)
        {
            //gPictBridgePrtStatus = NVT_PRT_STAT_PRINTING;
            //#NT#2008/12/23#Lily Kao -begin
            //Change multiple-copies Idx-Prt to multiple single-copy Idx-Prt
            #if 0
            if (g_uiPrtLayout == DPS_CAP_LAYOUT_IDX)
            {
                DBG_ERR("Change multiple-copies Idx-Prt to multiple single-copy Idx-Prt\r\n");
                nvtPrt_chgMultiCopyIdxPrt2SingleCopy(&gPBPrintData[tmpNum], &totalImages);
                PictB_PrintMoreImagesEachSetting(&gPBPrintData[0], totalImages);
            }
            else
            {
                PictB_PrintMoreImagesEachSetting(&gPBPrintData[tmpNum], totalImages);
            }
            #else
            PictB_PrintMoreImagesEachSetting(&gPBPrintData[tmpNum], totalImages);
            #endif
            //#NT#2008/12/23#Lily Kao -end
        }
    }
    //case1:Print All Images with same Copies
    else if(g_uiPrtCurMode==PMODE_ALL)
    {
        Sidc_GetPrintAllData(&startAddr, &totalImages);
        totalPrints = totalImages * g_uiPrtAllNum;
        DBG_DUMP("   PrintAll: Copies=%x,totalPrints=%x, totalImages=%x, startAddr=%x\r\n", g_uiPrtAllNum, totalPrints, totalImages, startAddr);
        //#NT#2010/02/10#Lily Kao -begin
        if (totalPrints)
        {
            if (totalPrints > NVT_PRT_MAX_CNT)
            {
                tmpNum = totalImages;
                totalImages = (NVT_PRT_MAX_CNT/g_uiPrtAllNum);
                totalPrints = totalImages * g_uiPrtAllNum;
                tmpNum -= totalImages;
                startAddr += (tmpNum * sizeof(PB_PRINT_DATA));
                DBG_ERR("   StartPrintAll,Over 10000: Copies=%x,totalPrints=%x, totalImages=%x, startAddr=%x\r\n", g_uiPrtAllNum, totalPrints, totalImages, startAddr);
            }
            PictB_PrintAllImages((PB_PRINT_DATA *)startAddr, totalImages, g_uiPrtAllNum);
        }
        //#NT#2010/02/10#Lily Kao -end
    }
    //case2:Print DPOF, one or more images with individual Copies
    else if(g_uiPrtCurMode==PMODE_DPOF)
    {

        totalPrints = DPOF_GetTotalPrints();
        DBG_DUMP("   PrintDPOF: totalPrints=0x%x\r\n", totalPrints);
        //#NT#2010/02/10#Lily Kao -begin
        //if (totalPrints && (totalPrints < NVT_PRT_MAX_CNT))
        if (totalPrints)
        {
            if (DPOF_GetTotalDatePrt() && (gDPSGetCapInfo.datePrints & DPS_CAP_DATEPRINT_ON))
            {
                DBG_DUMP("   PrintDPOF: DateOn\r\n");
                PictB_SetPTRCapability(DPS_CAPITEM_DATEPRINTS, DPS_CAP_DATEPRINT_ON);
            }
            else
            {
                DBG_DUMP("   PrintDPOF: DateDefault\r\n");
                PictB_SetPTRCapability(DPS_CAPITEM_DATEPRINTS, DPS_CAP_DATEPRINT_DEF);
            }
            PictB_PrintDPOF();
        }
        //#NT#2010/02/10#Lily Kao -end
    }
    return 0;
}

INT32 UIPrint_CheckPaperSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiPaperSize = 0;
    if(paramNum>0)
    {
        uiPaperSize= paramArray[0];
    }
    else
    {
        DBG_ERR("UIPrint_CheckPaperSize  parameter error!!!\r\n");
        return (INT32)FALSE;
    }
    DBG_MSG("UIPrint_CheckPaperSize uiPaperSize=0X%X\r\n",uiPaperSize);
    if((gDPSGetCapInfo.paperSizes& uiPaperSize))
    {
        return (INT32)TRUE;
    }
    else
    {
        return (INT32)FALSE;
    }
}

INT32 UIPrint_CheckLayout(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiLayout = 0;
    if(paramNum>0)
    {
        uiLayout= paramArray[0];
    }
    else
    {
        DBG_ERR("UIPrint_CheckLayout  parameter error!!!\r\n");
        return (INT32)FALSE;
    }
    DBG_MSG("UIPrint_CheckLayout uiLayout=0X%X\r\n",uiLayout);
    if((gDPSGetCapInfo.layouts& uiLayout))
    {
        return (INT32)TRUE;
    }
    else
    {
        return (INT32)FALSE;
    }
}

INT32 UIPrint_Abort(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PictB_PrintImageAbort();
    return NVTEVT_CONSUME;
}

INT32 UIPrint_Continue(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PictB_PrintImageContinue();
    return NVTEVT_CONSUME;
}
////////////////////////////////////////////////////////////

EVENT_ENTRY UIPrinterObjCmdMap[] =
{
    #if 0
    {NVTEVT_EXE_DATEON,        OnExe_DateOn},
    {NVTEVT_EXE_PAPERSIZE,     OnExe_PaperSize},
    {NVTEVT_EXE_LAYOUT,        OnExe_Layout},
    {NVTEVT_EXE_PRT_QUALITY,   OnExe_PrtQuality},
    {NVTEVT_EXE_PRT_NAMEPRINT, OnExe_PrtNamePrint},
    #endif
    {NVTEVT_PRTINIT,           UIPrint_Init},
    {NVTEVT_PRTENTERPB,        UIPrint_EnterPB},
    {NVTEVT_PRTCLOSESESSION,   UIPrint_CloseSession},
    {NVTEVT_PRTCAPSINIT,       UIPrint_CapsInit},
    {NVTEVT_PRTRESETDATA,      UIPrint_ResetData},
    {NVTEVT_PRTSTARTPRINT,     UIPrint_StartPrint},
    {NVTEVT_PRTCHEKC_PSIZE,    UIPrint_CheckPaperSize},
    {NVTEVT_PRTCHEKC_LAYOUT,   UIPrint_CheckLayout},
    {NVTEVT_PRT_ABORT,         UIPrint_Abort},
    {NVTEVT_PRT_CONTINUE,      UIPrint_Continue},
    {NVTEVT_NULL,                           0},  //End of Command Map
};

CREATE_APP(UIPrinterObj,APP_SETUP)

void AppPrint_SetData(PRT_DATA_SET attribute, UINT32 value)
{
    switch(attribute)
    {
        case PRT_CUR_MODE:
            g_uiPrtCurMode= value;
            break;
        case PRT_ALL_NUM:
            g_uiPrtAllNum = value;
            break;
        case PRT_SINGLE_NUM:
            PrtSavePrintSet((UINT8)value);
            break;
        //#NT#2009/12/04#Lily Kao - begin
        case PRT_SINGLE_DATE:
            g_uiPrtSingleDate = value;
            break;
        case PRT_ALL_DATE:
            g_uiPrtAllDate = value;
            PictB_SetPTRCapability(DPS_CAPITEM_DATEPRINTS, value);
            break;
        //#NT#2009/12/04#Lily Kao - end
        case PRT_PAPERSIZE:
            g_uiPrtPaperSize= value;
            PictB_SetPTRCapability(DPS_CAPITEM_PAPERSIZE, value);
            gDPSGetCapInfo.layouts = PictB_GetLayoutsSpecific(g_uiPrtPaperSize);
            break;
        case PRT_LAYOUT:
            g_uiPrtLayout= value;
            PictB_SetPTRCapability(DPS_CAPITEM_LAYOUTS, value);
            break;
        case PRT_CALLBACK_FUNC:
            PictB_RegCBFunction((USIDCPTR_CB)value);
            break;
        //#NT#2010/02/23#Lily Kao -begin
        case PRT_NAMEPRINT:
            PictB_SetPTRCapability(DPS_CAPITEM_NAMEPRINTS, value);
            break;
        default:
            DBG_ERR("[AppPrint_SetData]no this attribute=%d\r\n",attribute);
        //#NT#2010/02/23#Lily Kao -end
            break;
    }
}

UINT32 AppPrint_GetData(PRT_DATA_SET attribute)
{
    UINT8 uiSIDCType;
    UINT32  uiStartAddr, uiTotalImages;
    UINT16 CurrProgress, TolProgress;
    switch(attribute)
    {
        case PRT_PBSTATUS:
            return g_uiStatus;
        case PRT_CUR_MODE:
            return g_uiPrtCurMode;
        case PRT_ALL_NUM:
            return g_uiPrtAllNum;
        case PRT_SINGLE_NUM:
            return (UINT32)PrtCheckThisFilePrint();
        case PRT_SIDC_TYPE:
            Sidc_GetPTPType(&uiSIDCType);
            return uiSIDCType;
        case PRT_TOTAL_PB_IMG:
            Sidc_GetPrintAllData(&uiStartAddr, &uiTotalImages);
            return uiTotalImages;
        case PRT_TOTAL_PRINT_DPOF:
            return (UINT32)PrtGetCurTotalPrints(PMODE_DPOF);
        case PRT_TOTAL_PRINT_ALL:
            return (UINT32)PrtGetCurTotalPrints(PMODE_ALL);
        case PRT_TOTAL_PRINT_SINGLE:
            return (UINT32)PrtGetCurTotalPrints(PMODE_SINGLE);
        //#NT#2009/12/04#Lily Kao - begin
        case PRT_SINGLE_DATE:
            return (UINT32)PrtCheckThisFileDate();
        case PRT_ALL_DATE:
            return g_uiPrtAllDate;
        //#NT#2009/12/04#Lily Kao - end
        case PRT_PAPERSIZE:
            return g_uiPrtPaperSize;
        case PRT_LAYOUT:
            return g_uiPrtLayout;
        case PRT_CAPINFO_PAPERSIZE:
            return gDPSGetCapInfo.paperSizes;
        case PRT_CAPINFO_LAYOUT:
            return gDPSGetCapInfo.layouts;
        case PRT_CAPINFO_DATE:
            return gDPSGetCapInfo.datePrints;
        case PRT_CAPINFO_QUALITY:
            return gDPSGetCapInfo.qualities;
        case PRT_CAPINFO_NAMEPRINT:
            return gDPSGetCapInfo.fileNamePrints;
        //#NT#2009/12/04#Lily Kao - begin
        case PRT_CAPINFO_FILETYPE:
            return gDPSGetCapInfo.fileTypes;
        //#NT#2009/12/04#Lily Kao - end
        case PRT_CURR_PROGRESS:
            PictB_GetPTRnotifyJob(&CurrProgress, &TolProgress);
            return CurrProgress;
        case PRT_TOL_PROGRESS:
            PictB_GetPTRnotifyJob(&CurrProgress, &TolProgress);
            return TolProgress;
        default:
            DBG_ERR("[AppPlay_GetData]no this attribute");
            return 0;
    }
}

