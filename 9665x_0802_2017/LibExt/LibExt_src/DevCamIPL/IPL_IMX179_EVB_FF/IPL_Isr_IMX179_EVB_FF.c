/**
    ISR IMX179_EVB_FF.

    @file       IPL_IsrIMX179_EVB_FF.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "IPL_IMX179_EVB_FF_Int.h"

UINT32 currTime = 0;
UINT32 prevTime = 0;
extern UINT32 ShowFPS;

void IPL_IsrSIECB(IPL_PROC_ID Id, UINT32 IntStatus,UINT32 idx)
{
    //customized op
    if (IntStatus & SIE_FLDEND)
    {
        AE_TrigEvent(IPL_UTI_CONV2_AE_ID(Id));
        AWB_TrigEvent(IPL_UTI_CONV2_AWB_ID(Id));
        if ( ShowFPS == TRUE )
        {
            currTime = Perf_GetCurrent();
            debug_err(("sie = %d ms\r\n", (currTime-prevTime)/1000));
            prevTime = currTime;
        }
    }
    if (IntStatus & SIE_BP1)
    {
    }
    if (IntStatus & SIE_BP2)
    {
    }
}

void IPL_IsrIMECB(IPL_PROC_ID Id, UINT32 IntStatus,UINT32 idx)
{
    if (IntStatus & IME_FMD)
    {
        IPL_CBMsgProc(IPL_CBMSG_IMEFMD, NULL);
    }
}

void IPL_IsrIPECB(IPL_PROC_ID Id, UINT32 IntStatus,UINT32 idx)
{
    static UINT32 PrvAddr = 0;
    DISALG_PARAM disInfo = {0};
    IPC_QINFO QInfo = {0};

    if (IntStatus & IPE_ETH_DMAE)
    {
        if (IPL_CtrlGetInfor(Id, IPLCTRL_DIS_EN) == ENABLE)
        {
            QInfo = IPL_CTRLQuery(ICF_ETHOUT_RDY);

            disInfo.frame_cnt = idx;
            disInfo.InSizeH = QInfo.Rdy_Eth.Width;
            disInfo.InLineOfs = QInfo.Rdy_Eth.Lineofs;
            disInfo.InSizeV = QInfo.Rdy_Eth.Height;

            if (PrvAddr == 0)
            {
                disInfo.InAdd0 = QInfo.Rdy_Eth.Addr;
            }
            else
            {
                disInfo.InAdd0 = PrvAddr;
            }
            disInfo.InAdd1 = QInfo.Rdy_Eth.Addr;


            DBG_IND("%d %d %d %d 0x%.8x 0x%.8x\r\n", disInfo.frame_cnt, disInfo.InSizeH,
                                disInfo.InLineOfs, disInfo.InSizeV, disInfo.InAdd0, disInfo.InAdd1);

            if (dis_getTriggerStatus() == FALSE)
            {
                dis_setDisInfor(&disInfo);
                PrvAddr = QInfo.Rdy_Eth.Addr;
            }
        }
    }

    if (IntStatus & IPE_FMD)
    {
    }

}

void IPL_IsrIFECB(IPL_PROC_ID Id, UINT32 IntStatus,UINT32 idx)
{
    if (IntStatus & IFE_FMD)
    {
    }
}
