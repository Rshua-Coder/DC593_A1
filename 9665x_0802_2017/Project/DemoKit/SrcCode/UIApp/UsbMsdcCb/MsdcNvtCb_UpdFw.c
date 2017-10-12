#include "SysKer.h"
#include "MsdcNvtApi.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_UpdFw.h"
#include "FwSrvApi.h"
#include "FilesysTsk.h"
#include "GxSystem.h"
#include <string.h>

//for MSDCVendorNVT_AddCallback_Bi
static void MsdcNvtCb_UpdFwGetInfo(void* pData);
static void MsdcNvtCb_UpdFwGetWorkMem(void* pData);
static void MsdcNvtCb_UpdFwSetBlock(void* pData);

MSDCNVT_REG_BI_BEGIN(m_MsdcNvtUpdfw)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_UpdFwGetInfo)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_UpdFwGetWorkMem)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_UpdFwSetBlock)
MSDCNVT_REG_BI_END()

BOOL MsdcNvtRegBi_UpdFw(void)
{
    return MsdcNvt_AddCallback_Bi(m_MsdcNvtUpdfw);
}

static void MsdcNvtCb_UpdFwGetInfo(void* pData)
{
    UINT32 uiBlkSize;
    FWSRV_CMD Cmd = {0};
    tMSDCEXT_UPDFW_GET_BLOCK_INFO* pDesc = MSDCNVT_CAST(tMSDCEXT_UPDFW_GET_BLOCK_INFO,pData);
    if(pDesc==NULL)
        return;

    Cmd.Idx = FWSRV_CMD_IDX_GET_BLK_SIZE;
    Cmd.Out.pData = &uiBlkSize;
    Cmd.Out.uiNumByte = sizeof(uiBlkSize);
    Cmd.Prop.bExitCmdFinish=TRUE;
    if(FwSrv_Cmd(&Cmd)==FWSRV_ER_OK)
    {
        pDesc->uiBytesPerBlock = uiBlkSize;
        pDesc->tParent.bOK = TRUE;
    }
    else
    {
        debug_msg("MsdcNvtCb_UpdFwGetInfo: FwSrv_Cmd Failed\r\n!");
    }
}

static void MsdcNvtCb_UpdFwGetWorkMem(void* pData)
{
    PARTLOAD_INIT Init={0};
    tMSDCEXT_UPDFW_GET_WORK_MEM* pDesc = MSDCNVT_CAST(tMSDCEXT_UPDFW_GET_WORK_MEM,pData);
    if(pDesc==NULL)
        return;

    PartLoad_GetInit(&Init);
    pDesc->uiAddr = Init.uiWorkingAddr;
    pDesc->uiSize = Init.uiWorkingSize;

    pDesc->tParent.bOK = TRUE;
}

static void MsdcNvtCb_UpdFwSetBlock(void* pData)
{
    UINT32 uiBlkSize;
    FWSRV_BLK_DESC BlkInfo={0};
    FWSRV_CMD Cmd = {0};
    PARTLOAD_INIT Init={0};
    tMSDCEXT_UPDFW_SET_BLOCK_INFO* pDesc = MSDCNVT_CAST(tMSDCEXT_UPDFW_SET_BLOCK_INFO,pData);

    if(pDesc==NULL)
        return;

    if(pDesc->uiKey!=MAKEFOURCC('N','O','V','A'))
    {
        debug_msg("MsdcNvtCb_UpdFwSetBlock: Invalid Key\r\n!");
        return;
    }

    PartLoad_GetInit(&Init);

    Cmd.Idx = FWSRV_CMD_IDX_GET_BLK_SIZE;
    Cmd.Out.pData = &uiBlkSize;
    Cmd.Out.uiNumByte = sizeof(uiBlkSize);
    Cmd.Prop.bExitCmdFinish=TRUE;
    FwSrv_Cmd(&Cmd);

    memset(&Cmd,0,sizeof(Cmd));
    BlkInfo.Area = FWSRV_AREA_FIRMWARE;
    BlkInfo.uiIdx = pDesc->iBlock;
    BlkInfo.Buf.uiAddr = Init.uiWorkingAddr;
    BlkInfo.Buf.uiSize = uiBlkSize;
    Cmd.Idx = FWSRV_CMD_IDX_BLK_WRITE;
    Cmd.In.pData = &BlkInfo;
    Cmd.In.uiNumByte = sizeof(BlkInfo);
    Cmd.Prop.bExitCmdFinish=TRUE;
    if(FwSrv_Cmd(&Cmd)!=FWSRV_ER_OK)
    {
        return;
    }

    if(pDesc->IsLastBlock)
    {
        GxSystem_Reboot();
    }

    pDesc->tParent.bOK = TRUE;

}
