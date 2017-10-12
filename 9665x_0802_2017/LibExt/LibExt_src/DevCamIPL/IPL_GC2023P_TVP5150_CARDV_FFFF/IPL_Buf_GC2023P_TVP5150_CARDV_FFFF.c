/**
    Buf IMX322LQJ_TVP5150_CARDV_FFFF.

    @file       IPL_BufIMX322LQJ_TVP5150_CARDV_FFFF.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "IPL_GC2023P_TVP5150_CARDV_FFFF_Int.h"

CHAR* IPL_PrvBufNameTab[IPLBUF_PRV_MAX_CNT] =
{
    "SIE_RAW_CH0_1",
    "SIE_RAW_CH0_2",
    "SIE_RAW_CH0_3",
    "",
    "SIE_RAW_CH2_1",
    "SIE_RAW_CH2_2",
    "",
    "SIE_VA_1",
    "SIE_VA_2",
    "",
    "SIE_CA_1",
    "SIE_CA_2",
    "",
    "IPE_OUT_1",
    "IPE_OUT_2",
    "",
    "IPE_VA_1",
    "IPE_VA_2",
    "",
    "IPE_ETH_1",
    "IPE_ETH_2",
    "IPE_ETH_3",
    "",
    "IPE_LCA_1",
    "IPE_LCA_2",
    "IPE_LCA_3",
    "IPE_LCA_4",
    "",
    "IPE_V_1",
    "IPE_V_2",
    "IPE_V_3",
    "IPE_V_4",
    "",
    "IPEIFE2_LCA_1",
    "IPEIFE2_LCA_2",
    "",
    "IMEP1_1",
    "IMEP1_2",
    "IMEP1_3",
    "IMEP1_4",
    "IMEP1_5",
    "IMEP1_6",
    "IMEP1_7",
    "IMEP1_8",
    "IMEP1_9",
    "IMEP1_10",
    "",
    "IMEP2_1",
    "IMEP2_2",
    "IMEP2_3",
    "IMEP2_4",
    "IMEP2_5",
    "IMEP2_6",
    "IMEP2_7",
    "IMEP2_8",
    "IMEP2_9",
    "IMEP2_10",
    "",
    "IMEP3_1",
    "IMEP3_2",
    "IMEP3_3",
    "IMEP3_4",
    "IMEP3_5",
    "IMEP3_6",
    "IMEP3_7",
    "IMEP3_8",
    "IMEP3_9",
    "IMEP3_10",
    "",
    "IE_WDR",
    "STAMP",
    "ALBUM",
    "PHOTOFRAME"
};

#define IPPBUF_WORDALIGN(x) IPL_ALIGN_ROUNDUP(x, 4)

extern IPLBUF_RST IPL_BufAssignFlowA(IPLBUF_ENG2DRAMINFO *Data);
extern IPLBUF_RST IPL_BufAssignFlowB(IPLBUF_ENG2DRAMINFO *Data);
extern IPLBUF_RST IPL_BufAssignFlowC(IPLBUF_ENG2DRAMINFO *Data);
extern IPLBUF_RST IPL_BufAssignFlowD(IPLBUF_ENG2DRAMINFO *Data);
extern IPLBUF_RST IPL_BufAssignFlowCap(IPLBUF_ENG2DRAMINFO *Data);


FPIPPBUFASSIGNER IPL_BufAssignObj[IPLBUF_MODE_MAX_CNT] =
{
    IPL_BufAssignFlowA,
    IPL_BufAssignFlowB,
    IPL_BufAssignFlowC,
    IPL_BufAssignFlowD,
    IPL_BufAssignFlowCap,
};

IPLBUF_RST IPL_BufAssign(IPLBUF_MODE ModeIdx, IPLBUF_ENG2DRAMINFO *Data)
{
    IPLBUF_RST rtRst;

    rtRst.er_code = E_SYS;
    rtRst.BufSize = 0;
    if (ModeIdx >= IPLBUF_MODE_MAX_CNT)
    {
        DBG_ERR("ModeIdx = %d overflow\r\n", ModeIdx);
        return rtRst;

    }

    if (IPL_BufAssignObj[ModeIdx] != NULL)
    {
        rtRst = IPL_BufAssignObj[ModeIdx](Data);
    }
    return rtRst;
}

IPLBUF_RST IPL_BufAssignFlowA(IPLBUF_ENG2DRAMINFO *Data)
{
    IPLBUF_RST rtRst;
    UINT32 BufStAddr;
    UINT32 BufSize;
    UINT32 Size, Addr, i;

    IPL_BufGetResInfo(Data->Id, &BufStAddr, &BufSize);
    Addr = BufStAddr;

    if (Data->SIECh0En == ENABLE)
    {
        if ((Data->SIEOutCh0.BufNum > 0) && (Data->SIEOutCh0.BufNum <= (IPLBUF_SIERAW_CH0_MAX - IPLBUF_SIERAW_CH0_1)))
        {
            Size = IPL_UTI_RAW_BUF_SIZE(Data->SIEOutCh0.Width, Data->SIEOutCh0.LineOfs, Data->SIEOutCh0.Height);
            Size = IPPBUF_WORDALIGN(Size);
            for (i = 0; i < Data->SIEOutCh0.BufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_SIERAW_CH0_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIERAW_CH0_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("SIE Ch0 out Buffer number error (%d)\r\n", Data->SIEOutCh0.BufNum);
        }
    }

    if (Data->SIECh2En == ENABLE)
    {
        if ((Data->SIEOutCh2.BufNum > 0) && (Data->SIEOutCh2.BufNum <= (IPLBUF_SIERAW_CH2_MAX - IPLBUF_SIERAW_CH2_1)))
        {
            Size = IPL_UTI_RAW_BUF_SIZE(Data->SIEOutCh2.Width, Data->SIEOutCh2.LineOfs, Data->SIEOutCh2.Height);
            Size = IPPBUF_WORDALIGN(Size);
            for (i = 0; i < Data->SIEOutCh2.BufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_SIERAW_CH2_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIERAW_CH2_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("SIE Ch2 out Buffer number error (%d)\r\n", Data->SIEOutCh2.BufNum);
        }
    }

    if (Data->SIEVAEn == ENABLE)
    {
        if ((Data->SIEVABufNum > 0) && (Data->SIEVABufNum <= (IPLBUF_SIEVA_MAX - IPLBUF_SIEVA_1)))
        {
            Size = IPPBUF_WORDALIGN(Data->SIEVASize);
            for (i = 0; i < Data->SIEVABufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_SIEVA_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIEVA_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("SIE VA out Buffer number error (%d)\r\n", Data->SIEVABufNum);
        }
    }

    if (Data->SIECAEn == ENABLE)
    {
        if ((Data->SIECABufNum > 0) && (Data->SIECABufNum <= (IPLBUF_SIECA_MAX - IPLBUF_SIECA_1)))
        {
            Size = IPPBUF_WORDALIGN(Data->SIECABufSize);
            for (i = 0; i < Data->SIECABufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_SIECA_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIECA_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("SIE CA out Buffer number error (%d)\r\n", Data->SIECABufNum);
        }
    }

    if (Data->IPEVAEn == ENABLE)
    {
        if (Data->IPEVABufNum == (IPLBUF_IPEVA_MAX - IPLBUF_IPEVA_1))
        {
            Size = IPPBUF_WORDALIGN(Data->IPEVASize);
            for (i = 0; i < Data->IPEVABufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_IPEVA_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IPEVA_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("IPE VA out Buffer number error (%d)\r\n", Data->IPEVABufNum);
        }
    }

    if (Data->IPEEthEn == ENABLE)
    {
        Size = IPPBUF_WORDALIGN(Data->IPEEthSize);
        IPL_BufSet(Data->Id, IPLBUF_IPEETH_1, Addr, Size, IPL_PrvBufNameTab[IPLBUF_IPEETH_1]);
        Addr += Size;
        IPL_BufSet(Data->Id, IPLBUF_IPEETH_2, Addr, Size, IPL_PrvBufNameTab[IPLBUF_IPEETH_2]);
        Addr += Size;
        IPL_BufSet(Data->Id, IPLBUF_IPEETH_3, Addr, Size, IPL_PrvBufNameTab[IPLBUF_IPEETH_3]);
        Addr += Size;
    }

    if (Data->IPELCAEn == ENABLE)
    {
        if ((Data->IPELCABufNum > 0) && (Data->IPELCABufNum <= (IPLBUF_IPELCA_MAX - IPLBUF_IPELCA_1)))
        {
            Size = IPPBUF_WORDALIGN(Data->IPELCABufSize);
            for (i = 0; i < Data->IPELCABufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_IPELCA_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IPELCA_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("IPE LCA out Buffer number error (%d)\r\n", Data->IPELCABufNum);
        }
    }

    if (Data->IPEVEn == ENABLE)
    {
        if ((Data->IPEVBufNum > 0) && (Data->IPEVBufNum <= (IPLBUF_IPEV_MAX - IPLBUF_IPEV_1)))
        {
            Size = IPPBUF_WORDALIGN(Data->IPEVBufSize);
            for (i = 0; i < Data->IPEVBufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_IPEV_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IPEV_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("IPE V out Buffer number error (%d)\r\n", Data->IPEVBufNum);
        }
    }

    if (Data->IPEIFE2LCAEn == ENABLE)
    {
        if ((Data->IPEIFE2LCABufNum > 0) && (Data->IPEIFE2LCABufNum <= (IPLBUF_IPEIFE2LCA_MAX - IPLBUF_IPEIFE2LCA_1)))
        {
            Size = IPPBUF_WORDALIGN(Data->IPEIFE2LCABufSize);
            for (i = 0; i < Data->IPEIFE2LCABufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_IPEIFE2LCA_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IPEIFE2LCA_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("IFE2 LCA out Buffer number error (%d)\r\n", Data->IPEIFE2LCABufNum);
        }
    }

    if (Data->IMEPath1En == ENABLE)
    {
        if ((Data->IMEPath1.BufNum > 0) && (Data->IMEPath1.BufNum <= (IPLBUF_IMEP1_MAX - IPLBUF_IMEP1_1)))
        {
            Size = IPL_YUV_SIZE(Data->IMEPath1.Fmt, Data->IMEPath1.YLineOfs, Data->IMEPath1.YHeight);
            Size = IPPBUF_WORDALIGN(Size);
            for (i = 0; i < Data->IMEPath1.BufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_IMEP1_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP1_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("IME P1 out Buffer number error (%d)\r\n", Data->IMEPath1.BufNum);
        }
    }

    if (Data->IMEPath2En == ENABLE)
    {
        if ((Data->IMEPath2.BufNum > 0) && (Data->IMEPath2.BufNum <= (IPLBUF_IMEP2_MAX - IPLBUF_IMEP2_1)))
        {
            Size = IPL_YUV_SIZE(Data->IMEPath2.Fmt, Data->IMEPath2.YLineOfs, Data->IMEPath2.YHeight);
            Size = IPPBUF_WORDALIGN(Size);
            for (i = 0; i < Data->IMEPath2.BufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_IMEP2_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP2_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("IME P2 out Buffer number error (%d)\r\n", Data->IMEPath2.BufNum);
        }
    }

    if (Data->IMEPath3En == ENABLE)
    {
        if ((Data->IMEPath3.BufNum > 0) && (Data->IMEPath3.BufNum <= (IPLBUF_IMEP3_MAX - IPLBUF_IMEP3_1)))
        {
            Size = IPL_YUV_SIZE(Data->IMEPath3.Fmt, Data->IMEPath3.YLineOfs, Data->IMEPath3.YHeight);
            Size = IPPBUF_WORDALIGN(Size);
            for (i = 0; i < Data->IMEPath3.BufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_IMEP3_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP3_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("IME P3 out Buffer number error (%d)\r\n", Data->IMEPath3.BufNum);
        }
    }

    if (Data->IEffectWDREn == ENABLE)
    {
        Size = IPPBUF_WORDALIGN(Data->IEffectWDRBufSize);
        IPL_BufSet(Data->Id, IPLBUF_IMGEFFECT_WDR, Addr, Size, IPL_PrvBufNameTab[IPLBUF_IMGEFFECT_WDR]);
        Addr += Size;
    }

    if (Data->StampEn == ENABLE)
    {
        Size = IPPBUF_WORDALIGN(Data->StampBufSize);
        IPL_BufSet(Data->Id, IPLBUF_STAMP, Addr, Size, IPL_PrvBufNameTab[IPLBUF_STAMP]);
        Addr += Size;
    }

    if (Data->AlbumEn == ENABLE)
    {
        Size = IPPBUF_WORDALIGN(Data->AlbumBufSize);
        IPL_BufSet(Data->Id, IPLBUF_ALBUM, Addr, Size, IPL_PrvBufNameTab[IPLBUF_ALBUM]);
        Addr += Size;
    }

    if (Data->PhotoFrameEn == ENABLE)
    {
        Size = IPPBUF_WORDALIGN(Data->PhotoFrameBufSize);
        IPL_BufSet(Data->Id, IPLBUF_PHOTOFRAME, Addr, Size, IPL_PrvBufNameTab[IPLBUF_PHOTOFRAME]);
        Addr += Size;
    }

    rtRst.er_code = E_OK;
    rtRst.BufSize = (Addr - BufStAddr);
    if (rtRst.BufSize > BufSize)
    {
        rtRst.er_code = E_NOMEM;
        DBG_ERR("Buf. not enough 0x%x<0x%x\r\n", BufSize, rtRst.BufSize);
    }

    return rtRst;
}

IPLBUF_RST IPL_BufAssignFlowB(IPLBUF_ENG2DRAMINFO *Data)
{
    return IPL_BufAssignFlowA(Data);
}

IPLBUF_RST IPL_BufAssignFlowC(IPLBUF_ENG2DRAMINFO *Data)
{
    return IPL_BufAssignFlowA(Data);
}

IPLBUF_RST IPL_BufAssignFlowD(IPLBUF_ENG2DRAMINFO *Data)
{
    return IPL_BufAssignFlowA(Data);
}

IPLBUF_RST IPL_BufAssignFlowCap(IPLBUF_ENG2DRAMINFO *Data)
{
#if 0 //abby add @20160317
    IPLBUF_RST rtRst;

    rtRst.BufSize = 0;
    rtRst.er_code = E_OK;
    return rtRst;
#else
  IPLBUF_RST rtRst;
    UINT32 BufStAddr;
    UINT32 BufSize;
    UINT32 Size, Addr, i;

    IPL_BufGetResInfo(Data->Id, &BufStAddr, &BufSize);
    Addr = BufStAddr;

    if (Data->SIECAEn == ENABLE)
    {
        if ((Data->SIECABufNum > 0) && (Data->SIECABufNum <= (IPLBUF_SIECA_MAX - IPLBUF_SIECA_1)))
        {
            Size = IPPBUF_WORDALIGN(Data->SIECABufSize);
            for (i = 0; i < Data->SIECABufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_SIECA_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIECA_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("SIE CA out Buffer number error (%d)\r\n", Data->SIECABufNum);
        }
    }

    rtRst.er_code = E_OK;
    rtRst.BufSize = (Addr - BufStAddr);
    if (rtRst.BufSize > BufSize)
    {
        rtRst.er_code = E_NOMEM;
        DBG_ERR("Buf. not enough 0x%x<0x%x\r\n", BufSize, rtRst.BufSize);
    }
    return rtRst;
#endif
}
