////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "MsdcNvtApi.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_Photo.h"
#include "FilesysTsk.h"
#include "ImgCapInfor.h"
#include "DispSrvApi.h"
#include <string.h>
#include "UIAppPhoto.h"
#include "UIPhotoMapping.h"
#include "DCF.h"

//for MSDCVendorNVT_AddCallback_Bi
static void MsdcNvtCb_PhotoBegin(void* pData);
static void MsdcNvtCb_GetDisplaySettings2(void* pData);
static void MsdcNvtCb_GetDisplayCurrentBufSel(void* pData);
static void MsdcNvtCb_CaptureJpgWidthGetInfo(void* pData);
static void MsdcNvtCb_CaptureRawWidthGetInfo(void* pData);

MSDCNVT_REG_BI_BEGIN(m_MsdcNvtPhoto)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_PhotoBegin)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_GetDisplaySettings2)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_GetDisplayCurrentBufSel)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_CaptureJpgWidthGetInfo)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_CaptureRawWidthGetInfo)
MSDCNVT_REG_BI_END()

BOOL MsdcNvtRegBi_Photo(void)
{
    return MsdcNvt_AddCallback_Bi(m_MsdcNvtPhoto);
}


static void MsdcNvtCb_PhotoBegin(void* pData)
{
    tMSDCEXT_PARENT* pDesc = MSDCNVT_CAST(tMSDCEXT_PARENT,pData);
    if(pDesc==NULL)
        return;

    pDesc->bOK = FALSE;

    if(System_GetState(SYS_STATE_CURRMODE)!=PRIMARY_MODE_PHOTO)
    {
        INT32 nRetry = 20;
        Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PHOTO);
        //polling to wait finish
        while(System_GetState(SYS_STATE_CURRMODE)!=PRIMARY_MODE_PHOTO && nRetry-->0)
        {
            Delay_DelayMs(500);
        }
    }

    if(System_GetState(SYS_STATE_CURRMODE)==PRIMARY_MODE_PHOTO)
    {
        pDesc->bOK = TRUE;
    }
}

static void MsdcNvtCb_GetDisplaySettings2(void* pData)
{
    tMSDCEXT_DISPLAY_SETTING2* pDesc = MSDCNVT_CAST(tMSDCEXT_DISPLAY_SETTING2,pData);

    UINT32 i;
    IMG_BUF* pImg;
    DISPSRV_CMD Cmd={0};
    DISPSRV_MODE CfgMode = {0};

    if(pDesc==NULL)
        return;

    pDesc->Parent.bOK = FALSE;

    memset(&Cmd,0,sizeof(DISPSRV_CMD));
    Cmd.Idx = DISPSRV_CMD_IDX_GET_MODE;
    Cmd.Prop.bExitCmdFinish = TRUE;
    Cmd.Out.uiNumByte = sizeof(CfgMode);
    Cmd.Out.pData = &CfgMode;
    DispSrv_Cmd(&Cmd);

    if(CfgMode.IdxMode==DISPSRV_MODE_IDX_SINGLE_DIRECT)
    {
        DISPSRV_SRC_CFG CfgSrc = {0};

        memset(&Cmd,0,sizeof(DISPSRV_CMD));
        Cmd.Idx = DISPSRV_CMD_IDX_GET_SRC;
        Cmd.Prop.bExitCmdFinish = TRUE;
        Cmd.Out.uiNumByte = sizeof(CfgSrc);
        Cmd.Out.pData = &CfgSrc;
        DispSrv_Cmd(&Cmd);

        pDesc->nValidFrm = CfgSrc.Desc[DISPSRV_SRC_IDX_PRIMARY].uiNumSrc;
        pImg = CfgSrc.Desc[DISPSRV_SRC_IDX_PRIMARY].pSrc;
    }
    else
    {
        DISPSRV_DISP_CFG CfgDisp = {0};

        memset(&Cmd,0,sizeof(DISPSRV_CMD));
        Cmd.Idx = DISPSRV_CMD_IDX_GET_DISP;
        Cmd.Prop.bExitCmdFinish = TRUE;
        Cmd.Out.uiNumByte = sizeof(CfgDisp);
        Cmd.Out.pData = &CfgDisp;
        DispSrv_Cmd(&Cmd);

        pDesc->nValidFrm = CfgDisp.Desc[DISPSRV_SRC_IDX_PRIMARY].uiNumDisp;
        pImg = CfgDisp.Desc[DISPSRV_SRC_IDX_PRIMARY].pDisp;
    }

    for(i=0;i<pDesc->nValidFrm;i++)
    {
        pDesc->AddrY[i] = pImg[i].PxlAddr[0];
        pDesc->AddrU[i] = pImg[i].PxlAddr[1];
        pDesc->AddrV[i] = pImg[i].PxlAddr[2];
    }

    pDesc->PitchY = pImg[0].LineOffset[0];
    pDesc->PitchUV = pImg[0].LineOffset[1];
    pDesc->Width = pImg[0].Width;
    pDesc->HeightY = pImg[0].Height;
    if(pImg->PxlFmt==GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
    {
        pDesc->HeightUV = pImg[0].Height>>1;
    }
    else
    {
        pDesc->HeightUV = pImg[0].Height;
    }

    pDesc->Parent.bOK = TRUE;
}

static void MsdcNvtCb_GetDisplayCurrentBufSel(void* pData)
{
    tMSDCEXT_DISPLYA_CURRENT* pDesc = MSDCNVT_CAST(tMSDCEXT_DISPLYA_CURRENT,pData);

    DISPSRV_CMD Cmd={0};

    if(pDesc==NULL)
        return;

    pDesc->Parent.bOK = FALSE;

    memset(&Cmd,0,sizeof(DISPSRV_CMD));
    Cmd.Idx = DISPSRV_CMD_IDX_GET_CURR_DISPIDX;
    Cmd.Prop.bExitCmdFinish = TRUE;
    Cmd.Out.uiNumByte = sizeof(pDesc->OPTBUF);
    Cmd.Out.pData = &pDesc->OPTBUF;
    DispSrv_Cmd(&Cmd);

    pDesc->BUFNUM = pDesc->OPTBUF;

    pDesc->Parent.bOK = TRUE;
}

static void MsdcNvtCb_CaptureJpgWidthGetInfo(void* pData)
{
    tMSDCEXT_INFO_CAPTURE_JPG* pDesc = MSDCNVT_CAST(tMSDCEXT_INFO_CAPTURE_JPG,pData);
    if(pDesc==NULL)
        return;

    pDesc->Parent.bOK = FALSE;

    ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_SINGLE);
    ImgCap_SetUIInfo(CAP_SEL_PICNUM, 1);
    ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 1);
    ImgCap_SetUIInfo(CAP_SEL_JPG_BUF_NUM, 1);
    ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
    ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
    ImgCap_SetUIInfo(CAP_SEL_ENCRATIO, 100);
    ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_H_SIZE, GetPhotoSizeWidth(UI_GetData(FL_PHOTO_SIZE)));
    ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_V_SIZE, GetPhotoSizeHeight(UI_GetData(FL_PHOTO_SIZE)));
    ImgCap_StartCapture();
    ImgCap_WaitFinish();

    ImgCap_GetCurJpgInfo(&pDesc->JpgAddr, &pDesc->JpgFileSize);

    pDesc->Parent.bOK = TRUE;
}

static void MsdcNvtCb_CaptureRawWidthGetInfo(void* pData)
{
    UINT32 BufSize;
    UINT32 uiPoolAddrEnd;
    UINT32 uiFolderId;
    UINT32 uiFileId;
    FST_FILE hFile=NULL;
    SEL_RAW_BITDEPTH BitDepth = SEL_RAW_BITDEPTH_8;

    IPL_SIE_RAW_INFO IplInfo={0};
    char path[DCF_FULL_FILE_PATH_LEN];

    tMSDCEXT_INFO_CAPTURE_RAW* pDesc = MSDCNVT_CAST(tMSDCEXT_INFO_CAPTURE_RAW,pData);
    if(pDesc==NULL)
        return;


    pDesc->Parent.bOK = FALSE;

    DCF_GetNextID(&uiFolderId,&uiFileId);
    DCF_MakeObjPath(uiFolderId,uiFileId,DCF_FILE_TYPE_RAW,path);

    IPL_AlgSetUIInfo(IPL_SEL_RAW_BITDEPTH_CAP, BitDepth);
    ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_SINGLE);
    ImgCap_SetUIInfo(CAP_SEL_PICNUM, 1);
    ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 1);
    ImgCap_SetUIInfo(CAP_SEL_JPG_BUF_NUM, 1);
    ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_RAW);
    ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
    ImgCap_SetUIInfo(CAP_SEL_ENCRATIO, 100);
    ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_H_SIZE, GetPhotoSizeWidth(UI_GetData(FL_PHOTO_SIZE)));
    ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_V_SIZE, GetPhotoSizeHeight(UI_GetData(FL_PHOTO_SIZE)));
    ImgCap_StartCapture();
    ImgCap_WaitFinish();

    uiPoolAddrEnd = OS_GetMempoolAddr(POOL_ID_APP);
    if (uiPoolAddrEnd == 0)
    {
        pDesc->Parent.bOK = FALSE;
        return;
    }
    uiPoolAddrEnd = uiPoolAddrEnd + POOL_SIZE_APP;

    IplInfo.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_SIE_CUR_CAP_CH0_RAW_INFOR, &IplInfo);
    pDesc->RawWidth=IplInfo.ImgLineOfs;
    pDesc->RawHeight=IplInfo.ImgHeight;

    switch(BitDepth)
    {
        case SEL_RAW_BITDEPTH_8:
            pDesc->RawNbit = 8;
            break;
        case SEL_RAW_BITDEPTH_10:
            pDesc->RawNbit = 10;
            break;
        case SEL_RAW_BITDEPTH_12:
            pDesc->RawNbit = 12;
            break;
        case SEL_RAW_BITDEPTH_16:
            pDesc->RawNbit = 16;
            break;
        default:
            break;
    }

    pDesc->RawFileSize = pDesc->RawWidth*pDesc->RawHeight*((pDesc->RawNbit+7)>>3);
    ImgCap_GetCurJpgInfo(&pDesc->RawAddr, &BufSize);
    pDesc->RawAddr=ALIGN_FLOOR_32(pDesc->RawAddr);
    BufSize = pDesc->RawFileSize;

    hFile = FileSys_OpenFile(path,FST_OPEN_READ);
    if(FileSys_ReadFile(hFile,(UINT8*)pDesc->RawAddr, &BufSize, 0, NULL)!=0)
    {
        pDesc->Parent.bOK = FALSE;
        FileSys_CloseFile(hFile);
        return;
    }
    FileSys_CloseFile(hFile);

    //Delete Current File After Read To Memory
    FileSys_DeleteFile(path);

    pDesc->Parent.bOK = TRUE;
}
