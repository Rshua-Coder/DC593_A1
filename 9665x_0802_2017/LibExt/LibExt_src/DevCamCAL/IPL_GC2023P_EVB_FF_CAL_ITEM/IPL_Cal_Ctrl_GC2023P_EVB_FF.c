#include "sensor.h"
#include "IPL_Cal_Ctrl_GC2023P_EVB_FF_Int.h"
#include "IPL_Cal_Setting_GC2023P_EVB_FF_Int.h"
#include "IPL_CAL_Debug.h"
#include "GXdisplay.h"
#include "ipl_display.h"
#include "ipl_alginfor.h"
#include "IPL_Cal_DataPassing_GC2023P_EVB_FF_Int.h"
#define DMA333 ENABLE

#define RAW_BIT         IPL_RAW_BIT_8//SUPPORT ONLY 8bit or 16bit
static IPLC_BASEINFO    m_BaseInfo = {0};
static IPLC_EXT         m_ExtInfo = {0};
static SIE_UPDATE       m_sieUpdate = {0};
static SIE_SUBFUNC      m_sieSubF = {0};
static SIE              m_sie = {0};
typedef enum
{
    sie_raw_0,
    sie_raw_1,
    sie_ca_0,
    sie_ca_1,

    ime_p2_y_0,
    ime_p2_y_1,
    ime_p2_y_2,
    ime_p2_y_3,
    ime_p2_y_4,
    ime_p2_y_5,
    ime_p2_y_6,
    ime_p2_y_7,
    ime_p2_y_8,
    ime_p2_y_9,
    ime_p2_uv_0,
    ime_p2_uv_1,
    ime_p2_uv_2,
    ime_p2_uv_3,
    ime_p2_uv_4,
    ime_p2_uv_5,
    ime_p2_uv_6,
    ime_p2_uv_7,
    ime_p2_uv_8,
    ime_p2_uv_9,

    ipe_va,
    ENUM_DUMMY4WORD(CAL_PRV_ENUM)
}CAL_PRV_ENUM;

_INLINE UINT32 _DCE_IntCB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    group->dceUpdate |= (
                        DCE_DisT       | ///< update H/V distance factor/ratio selection
                        DCE_CAB        | ///< update color aberration factor
                        DCE_LUT        | ///< update LUT table address
                        DCE_FOV        | ///< update FOV
                        DCE_ENH        | ///< update enhancement
                        DCE_DISTOR      ///< update distortion parameter set
                      );
    Cal_DCE_Distortion.Center.x = m_BaseInfo.SIE_IN.Act.SizeH>>1;
    Cal_DCE_Distortion.Center.y = m_BaseInfo.SIE_IN.Act.SizeV>>1;
    DCE_LutTableType(((UINT32*)Cal_DCE_Distortion.LUT_TblAddr),(&Cal_DCE_Distortion.LutType));
    Info->pDce_sub->pDistortion = &Cal_DCE_Distortion;
    Info->pDce_sub->Dce_Dist = CAL_DCE_Dist;
    return 1;
}

SIE_CACC Cal_cacc;
SIE_EmbeddedCS m_Ecs = {ECS_D2F8,ENABLE,ENABLE,_BIT_1_2};
_INLINE UINT32 _SIE_IntCB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    CAL_TAG* pTag;
    DBG_ERR("not ready\r\n");
    /**
        TODO:
            1. ob value should link with ipl sample
            2. should read other calibration resullt,like ECS...etc.
    */
//sun 0610
    Info->pSie_sub->OBSub.Offset=0xe8;
    Info->pSie_sub->OBSub.DtGain=0x20;
    Info->pSie_sub->sieFuncEn |= OBSubSelEn;
    group->sieUpdate |= (SIE_OBOFS_ | SIE_SubFEn);

    Cal_cacc.CAPack = _32BIT;
    Cal_cacc.CA_WIN.Start.x = 0;
    Cal_cacc.CA_WIN.Start.y = 0;
    Cal_cacc.CA_WIN.WinNum_X = 32;
    Cal_cacc.CA_WIN.WinNum_Y = 32;
    Cal_cacc.CA_WIN.SizeH = m_BaseInfo.SIE_IN.Act.SizeH;
    Cal_cacc.CA_WIN.SizeV = m_BaseInfo.SIE_IN.Act.SizeV;

    Info->pSie_sub->Ca = Cal_cacc;
    Info->pSie_sub->sieFuncEn |= (DMAOutCh0En|CACCEn);
    group->sieUpdate |= (SIE_SubFEn|SIE_CH0Out_SRC|SIE_CA_);

    //ecs
    pTag = GetCalData(SEC_ECS,IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));
    if(pTag)
    {
        if(pTag->CAL_STATUS == _CAL_OK)
        {
            CAL_ECS_FMT *pFmt;
            pFmt = (CAL_ECS_FMT *)&pTag->TagData;
            Info->pSie_sub->sieFuncEn |= (ECSEn);
            Info->pSie_sub->Ecs.EcsMap = pFmt->header.Ecs_map;
            m_Ecs.BitSel = pFmt->header.Ecs_bit;
            Info->pSie_sub->Ecs.pecs = &m_Ecs;
            Info->pSie_sub->Ecs.Inaddr = (UINT32)&pFmt->ECSBlockWin;
            group->sieUpdate |= (SIE_SubFEn|SIE_ECS_);
        }
    }

    //dp
    pTag = GetCalData(SEC_DP,IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));
    if(pTag)
    {
        if(pTag->CAL_STATUS == _CAL_OK)
        {
            CAL_DP_FMT *pFmt;
            pFmt = (CAL_DP_FMT *)&pTag->TagData;
            Info->pSie_sub->sieFuncEn |= (DpcEn);
            Info->pSie_sub->Dpc.DP_TblAddr = (UINT32)&pFmt->DP_Pool;
            Info->pSie_sub->Dpc.TblSize = pTag->TagDataLen - sizeof(CAL_DPC_HEADER);
            Info->pSie_sub->Dpc.Weight = _50_Percent;
            group->sieUpdate |= (SIE_SubFEn|SIE_DPC_);
        }
    }
    return 1;
}


_INLINE UINT32 _IPE_IntCB(ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    Info->pIpe_sub->IpeFuncEn |= (IPE_CSTEn|IPE_CFAINTEEn|IPE_CCREn|IPE_CgainEn|IPE_RGBGAMMAEn);
    group->ipeUpdate |= (IPE_SubFEn);
    return 1;
}

_INLINE void _SIETrigSel(SIEPreTrigSel *TrigSel)
{
    if(TrigSel)
        *TrigSel = Cur_FLDD;
}

_INLINE void _SIEIn(SENSOR_INFO *pSenInfo, Cal_SENSOR_INFO *pCalSenInfo,SIE_WINDOW *Win)
{
    UINT32 i;
    for (i = 0; i < SIE_MAX_DIV; i ++)
    {
        Win->Act.DIV_X[i] = 0;
    }

    for (i = 0; i < pSenInfo->Mode->DIVNum; i ++)
    {
        Win->Act.DIV_X[i] = pSenInfo->Mode->TransHD[i].DataStart;
    }

    for (i = 0; i < MAX_FLD; i ++)
    {
        Win->Act.FLD_Y[i] = 0;
    }

    for (i = 0; i < pSenInfo->Mode->FieldNum; i ++)
    {
        Win->Act.FLD_Y[i] = pSenInfo->Mode->TransVD[i].DataStart;
    }

    Win->Act.SizeH = pSenInfo->Mode->TransHD[0].DataSize;
    Win->Act.SizeV = pSenInfo->Mode->TransVD[0].DataSize;
    Win->Act.CFA = pSenInfo->Mode->StPix;
debug_err((">>>>cal act h = %d v = %d\r\n",Win->Act.SizeH,Win->Act.SizeV));
    if( (pCalSenInfo->CropShrinkX % 4) )
    {
        pCalSenInfo->CropShrinkX += (4 - (pCalSenInfo->CropShrinkX % 4));//must be mutiple of 4
        DBG_WRN("\r\n");
    }
    if( (pCalSenInfo->CropShrinkY % 4) )
    {
        pCalSenInfo->CropShrinkY += (4 - (pCalSenInfo->CropShrinkY % 4));//must be mutiple of 4
        DBG_WRN("\r\n");
    }

    Win->Crop.SizeH = Win->Act.SizeH - pCalSenInfo->CropShrinkX;
    Win->Crop.SizeV = Win->Act.SizeV - pCalSenInfo->CropShrinkY;
    Win->Crop.CFA = Win->Act.CFA;
debug_err((">>>Crop h = %d v = %d\r\n",Win->Crop.SizeH,Win->Crop.SizeV));
    Win->Crop.StartPix.x = (Win->Act.SizeH - Win->Crop.SizeH) >> 1;
    Win->Crop.StartPix.y = (Win->Act.SizeV - Win->Crop.SizeV) >> 1;
}


_INLINE void _SIESignal(SENSOR_INFO *pSenInfo, Cal_SENSOR_INFO *pCalSenInfo, IPP_SIE_SIGNAL *Signal)
{
    UINT32 i, j;
    debug_err(("cal>>>>>Inv h=%d v=%d\r\n",pCalSenInfo->HDinv,pCalSenInfo->VDinv));
    Signal->Sie_HDVD.HD_ini = pCalSenInfo->HDinv;
    Signal->Sie_HDVD.HD_offset = 0;
    Signal->Sie_HDVD.HD_sync = pSenInfo->Mode->TransHD[0].Sync;
    Signal->Sie_HDVD.HD_total = pSenInfo->Mode->TransHD[0].Period;

    Signal->Sie_HDVD.VD_ini = pCalSenInfo->VDinv;
    Signal->Sie_HDVD.VD_offset = 0;
    Signal->Sie_HDVD.VD_sync = pSenInfo->Mode->TransVD[0].Sync;
    Signal->Sie_HDVD.VD_total = pSenInfo->Mode->TransVD[0].Period;
debug_err((">>>phase v=%d h=%d d=%d\r\n",pCalSenInfo->VDPhase,pCalSenInfo->HDPhase,pCalSenInfo->DataPhase));

    Signal->SensorPhase.VD = pCalSenInfo->VDPhase;
    Signal->SensorPhase.HD = pCalSenInfo->HDPhase;
    Signal->SensorPhase.Data = pCalSenInfo->DataPhase;

    Signal->Sie_ReOrder.Reorder_VEn = DISABLE;
    Signal->Sie_FLD.SieFldNum = pSenInfo->Mode->FieldNum - 1;
    for (i = 0; i < FieldCycle; i ++)
    {
        for (j = 0; j < MAX_FLD; j ++)
        {
            if (pSenInfo->Mode->FieldTab == NULL)
            {
                Signal->Sie_FLD.FldLine[i][j] = 0;
            }
            else
            {
                Signal->Sie_FLD.FldLine[i][j] = pSenInfo->Mode->FieldTab->Map[(i * MAX_FLD) + j] - 1;
            }
        }
    }

    Signal->SieDivNum = pSenInfo->Mode->DIVNum - 1;
}

_INLINE void _SIEAct(SENSOR_INFO *pSenInfo, SieACTMode *ActMode)
{
    if ((pSenInfo->DataType == SENSOR_DATA_LVDS) || (pSenInfo->DataType == SENSOR_DATA_MIPI))
    {
        *ActMode = Serial;
    }
    else if (pSenInfo->DataType == SENSOR_DATA_PARALLEL)
    {
        if (pSenInfo->SigType == SENSOR_SIGNAL_MASTER)
        {
            *ActMode = Para_M;
        }
        else if (pSenInfo->SigType == SENSOR_SIGNAL_SLAVE)
        {
            *ActMode = Para_S;
        }
        else
        {
            DBG_ERR("unsupport signal type %d\r\n", pSenInfo->SigType);
        }
    }
    else
    {
        DBG_ERR("unsupport Data Type %d\r\n", pSenInfo->DataType);
    }
}

_INLINE UINT32 _SIE_PclkSelCBPad(VOID)
{
    return IPH_SIEPXCLK_PAD;
}

_INLINE void _SIE_BURST_LENG(SIE_BURST_LENG* pLeng)
{
#if (DMA333 == DISABLE)
        if(pLeng)
            *pLeng = _BURST_64WORD;
#else
        if(pLeng)
            *pLeng = _BURST_128WORD;
#endif
}

static IPH_CLK CAL_SIEClkCBFlow(void)
{
    IPH_CLK clk;
    SENSOR_INFO SenInfo;

    Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(IPL_ID_1), IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE), &SenInfo);

    if (SenInfo.DataType == SENSOR_DATA_LVDS)
    {
        clk.SIEclk = 0;
    }
    else if (SenInfo.DataType == SENSOR_DATA_DUMMY)
    {
        clk.SIEclk = 27000000;
    }
    else
    {
        //set sie clk for parallel sensor
        //clk.SIEclk = 0;
        clk.SIEclk = 96000000;
    }
    return clk;
}

#if 0
#endif


static void Prv_CtrlInit(pIPLC_INFO pIplObj,pCTRL_PTR pCtrlptr)
{
    SENSOR_INFO *pSenInfo = (SENSOR_INFO *)pCtrlptr->pDrv_Sensor;
    Cal_SENSOR_INFO *pCalInfo = (Cal_SENSOR_INFO*)pCtrlptr->pCal_Sensor;
    ISIZE disp_size;
    UINT32 i;
    IPL_YCC_IMG_INFO imeInfo;

    pIplObj->pBaseInfo = &m_BaseInfo;
    pIplObj->pExtInfo = &m_ExtInfo;

    //sie
    _SIESignal(pSenInfo, pCalInfo ,&(pIplObj->pBaseInfo->SenSorIn));
    _SIEIn(pSenInfo, pCalInfo,&(pIplObj->pBaseInfo->SIE_IN));
    _SIEAct(pSenInfo,&(pIplObj->pBaseInfo->SIE_Act));
    _SIE_BURST_LENG(&pIplObj->pBaseInfo->Sie_BL_Ch0);

    pIplObj->pBaseInfo->SIE_PPB0.Bit = RAW_BIT;
    pIplObj->pBaseInfo->SIE_PPB0.Width = pIplObj->pBaseInfo->SIE_IN.Crop.SizeH;
    pIplObj->pBaseInfo->SIE_PPB0.Height = pIplObj->pBaseInfo->SIE_IN.Crop.SizeV;
    pIplObj->pBaseInfo->SIE_PPB0.LineOffset = pIplObj->pBaseInfo->SIE_PPB0.Width * pIplObj->pBaseInfo->SIE_PPB0.Bit / 8;
    pIplObj->pBaseInfo->SIE_PPB0.STPix = pSenInfo->Mode->StPix;
    pIplObj->pBaseInfo->SIE_Src = NonScaled_Img;
    pIplObj->pExtInfo->fp_SieInit = _SIE_IntCB;
    pIplObj->pExtInfo->fp_SIEClk= CAL_SIEClkCBFlow;
    pIplObj->pExtInfo->fp_SIEPclkSel=_SIE_PclkSelCBPad;

    //dce
    pIplObj->pBaseInfo->DCE_Ref_1X.w = pSenInfo->Mode->TransHD[0].DataSize;
    pIplObj->pBaseInfo->DCE_Ref_1X.h = pSenInfo->Mode->TransVD[0].DataSize;
    pIplObj->pBaseInfo->DCE_Ref_1XCenter.x = pIplObj->pBaseInfo->DCE_Ref_1X.w >>1;
    pIplObj->pBaseInfo->DCE_Ref_1XCenter.y = pIplObj->pBaseInfo->DCE_Ref_1X.h >>1;

    pIplObj->pExtInfo->fp_DceInit = _DCE_IntCB;


    //ipe
    pIplObj->pExtInfo->fp_IpeInit = _IPE_IntCB;

    //ime
    disp_size = GxVideo_GetDeviceSize(DOUT1);
    pIplObj->pBaseInfo->IME_P2_PPB0.type = IPL_IMG_Y_PACK_UV420;
    pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].Width = disp_size.w;
    pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].Height = disp_size.h;
    pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].LineOfs = pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].Width;
    pIplObj->pBaseInfo->IME_P2_PPB0.Ch[1] = IPL_UTI_Y_INFO_CONV2(pIplObj->pBaseInfo->IME_P2_PPB0.type,pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0]);


    //buf allocate
    CalApi()->fpBufEvent()->fpSetAddr(pCalInfo->SenMode,sie_raw_0,
    pIplObj->pBaseInfo->SIE_PPB0.LineOffset * pIplObj->pBaseInfo->SIE_PPB0.Height);
    CalApi()->fpBufEvent()->fpSetAddr(pCalInfo->SenMode,sie_raw_1,
    pIplObj->pBaseInfo->SIE_PPB0.LineOffset * pIplObj->pBaseInfo->SIE_PPB0.Height);

    for(i = 0;i<IPL_GetDisplayCtrlBufferNum();i++)
    {
        CalApi()->fpBufEvent()->fpSetAddr(pCalInfo->SenMode,ime_p2_y_0+i,
            pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].LineOfs*pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].Height);
        CalApi()->fpBufEvent()->fpSetAddr(pCalInfo->SenMode,ime_p2_uv_0+i,
            pIplObj->pBaseInfo->IME_P2_PPB0.Ch[1].LineOfs*pIplObj->pBaseInfo->IME_P2_PPB0.Ch[1].Height);
    }
    CalApi()->fpBufEvent()->fpSetAddr(pCalInfo->SenMode,ipe_va,16*16*4);
    CalApi()->fpBufEvent()->fpSetAddr(pCalInfo->SenMode,sie_ca_0, 32*32*8); // 1024 words
    CalApi()->fpBufEvent()->fpSetAddr(pCalInfo->SenMode,sie_ca_1, 32*32*8);

    //sie addr
    CalApi()->fpBufEvent()->fpGetAddr(pCalInfo->SenMode,sie_raw_0,
    &pIplObj->pBaseInfo->SIE_PPB0.PixelAddr,NULL);
    CalApi()->fpBufEvent()->fpGetAddr(pCalInfo->SenMode,sie_raw_1,
    &pIplObj->pBaseInfo->SIE_PPB1_ADDR,NULL);

    CalApi()->fpBufEvent()->fpGetAddr(pCalInfo->SenMode,sie_ca_0,
    &pIplObj->CA_PPB.PPB0_Addr,NULL);
    CalApi()->fpBufEvent()->fpGetAddr(pCalInfo->SenMode,sie_ca_1,
    &pIplObj->CA_PPB.PPB1_Addr,NULL);

    //pre addr
    pIplObj->pBaseInfo->PRE_PPB0 = pIplObj->pBaseInfo->SIE_PPB0;
    pIplObj->pBaseInfo->PRE_PPB1_ADDR = pIplObj->pBaseInfo->SIE_PPB1_ADDR;

    //ime addr
    CalApi()->fpBufEvent()->fpGetAddr(pCalInfo->SenMode,ime_p2_y_0,
    &pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[0],NULL);
    CalApi()->fpBufEvent()->fpGetAddr(pCalInfo->SenMode,ime_p2_uv_0,
    &pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[1],NULL);

    //set dispaly buffer
    imeInfo = pIplObj->pBaseInfo->IME_P2_PPB0;
    for (i = 0; i < IPL_GetDisplayCtrlBufferNum(); i ++)
    {
        CalApi()->fpBufEvent()->fpGetAddr(pCalInfo->SenMode,ime_p2_y_0+i,
            &pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[0],NULL);
        CalApi()->fpBufEvent()->fpGetAddr(pCalInfo->SenMode,ime_p2_uv_0+i,
            &pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[1],NULL);
        IPL_InitDisplayBuffer(1, i, &pIplObj->pBaseInfo->IME_P2_PPB0);
    }
    //only for p1/p3 para init,path will not be enabled
    pIplObj->pExtInfo->IME_P1_PPB0 = pIplObj->pBaseInfo->IME_P2_PPB0;
    pIplObj->pExtInfo->IME_P1_PPB0.type = IPL_IMG_Y_PACK_UV422;
    pIplObj->pExtInfo->IME_P3_PPB0 = pIplObj->pBaseInfo->IME_P2_PPB0;
    pIplObj->pExtInfo->IME_P3_PPB0.type = IPL_IMG_Y_PACK_UV422;

    CalApi()->fpBufEvent()->fpGetAddr(pCalInfo->SenMode,ipe_va,
    &pIplObj->pBaseInfo->IPE_VA_ADDR,NULL);

    IPL_SetDeviceInputInfo(1, imeInfo.Ch[0]);
}

#if 0
#endif

static void ISO_CtrlInit(pIPLC_INFO pIplObj,pCTRL_PTR pCtrlptr)
{
    SENSOR_INFO *pSenInfo = (SENSOR_INFO *)pCtrlptr->pDrv_Sensor;
    Cal_SENSOR_INFO *pCalInfo = (Cal_SENSOR_INFO*)pCtrlptr->pCal_Sensor;

    //clr global var.
    m_sieUpdate = 0;
    memset((void *)&m_sie,0,sizeof(SIE));
    memset((void *)&m_sieSubF,0,sizeof(SIE_SUBFUNC));

    m_sie.pSubFunc = &m_sieSubF;
    pIplObj->psie = &m_sie;
    pIplObj->psieUpdate = &m_sieUpdate;


    //sie in
    _SIESignal(pSenInfo , pCalInfo, &(pIplObj->psie->Sie_Signal));
    _SIEIn(pSenInfo , pCalInfo, &(pIplObj->psie->IMG_Window));
    _SIEAct(pSenInfo , &(pIplObj->psie->ActMode));
    _SIETrigSel(&(pIplObj->psie->TrigSel));
    _SIE_BURST_LENG(&pIplObj->psie->BurstLeng_Ch0);
    //inte
    pIplObj->psie->pSubFunc->InteEn |= (SIE__INTE_FLDEND);
        //sie output size
    pIplObj->psie->Ch0.Out.packBus = RAW_BIT;
    pIplObj->psie->Ch0.SiePPBNum = SIE_BUF_2;
    pIplObj->psie->Ch0.Src = NonScaled_Img;
    pIplObj->psie->DmaOutSizeH = pIplObj->psie->IMG_Window.Crop.SizeH;
    pIplObj->psie->DmaOutSizeV = pIplObj->psie->IMG_Window.Crop.SizeV;
    pIplObj->psie->Ch0.Out.LineOffset = pIplObj->psie->DmaOutSizeH * pIplObj->psie->Ch0.Out.packBus / 8;
    //sie subF
    pIplObj->psie->pSubFunc->sieFuncEn |= (DMAOutCh0En);
    /**
        TODO:
            1. ob value should link with ipl sample
    */
    //buf allocate
    CalApi()->fpBufEvent()->fpSetAddr(pCalInfo->SenMode,pCalInfo->SenMode,
    pIplObj->psie->Ch0.Out.LineOffset * pIplObj->psie->DmaOutSizeV);

    //sie addr
    CalApi()->fpBufEvent()->fpGetAddr(pCalInfo->SenMode,pCalInfo->SenMode,
    &pIplObj->psie->Ch0.Out.PPB0_Addr,NULL);
    pIplObj->psie->Ch0.Out.PPB1_Addr = pIplObj->psie->Ch0.Out.PPB0_Addr;
    m_sieUpdate |= (SIE_OutAddr_Ch0|SIE_IOSize|SIE_SubFEn|SIE_INIT|SIE_BURSTLENG);
}


#if 0
#endif
static void AWB_CtrlInit(pIPLC_INFO pIplObj,pCTRL_PTR pCtrlptr)
{
    SENSOR_INFO *pSenInfo = (SENSOR_INFO *)pCtrlptr->pDrv_Sensor;
    Cal_SENSOR_INFO *pCalInfo = (Cal_SENSOR_INFO*)pCtrlptr->pCal_Sensor;
    CAL_TAG* pTag;

    //clr global var.
    m_sieUpdate = 0;
    memset((void *)&m_sie,0,sizeof(SIE));
    memset((void *)&m_sieSubF,0,sizeof(SIE_SUBFUNC));

#if 1
    //ecs
    pTag = GetCalData(SEC_ECS,SENSOR_MODE_1);
    if(pTag)
    {
        if(pTag->CAL_STATUS == _CAL_OK)
        {
            CAL_ECS_FMT *pFmt;
            pFmt = (CAL_ECS_FMT *)&pTag->TagData;
            m_sieSubF.sieFuncEn |= (ECSEn);
            m_sieSubF.Ecs.EcsMap = pFmt->header.Ecs_map;
            m_Ecs.BitSel = pFmt->header.Ecs_bit;
            m_sieSubF.Ecs.pecs = &m_Ecs;
            m_sieSubF.Ecs.Inaddr = (UINT32)&pFmt->ECSBlockWin;
            m_sieUpdate |= (SIE_SubFEn|SIE_ECS_);

            DBG_ERR("cal sensor mode %d\r\n",IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE));
        }
    }
#endif



    m_sie.pSubFunc = &m_sieSubF;
    pIplObj->psie = &m_sie;
    pIplObj->psieUpdate = &m_sieUpdate;


    //sie in
    _SIESignal(pSenInfo , pCalInfo, &(pIplObj->psie->Sie_Signal));
    _SIEIn(pSenInfo , pCalInfo, &(pIplObj->psie->IMG_Window));
    _SIEAct(pSenInfo , &(pIplObj->psie->ActMode));
    _SIETrigSel(&(pIplObj->psie->TrigSel));
    _SIE_BURST_LENG(&pIplObj->psie->BurstLeng_Ch0);
    //inte
    pIplObj->psie->pSubFunc->InteEn |= (SIE__INTE_FLDEND);
        //sie output size
        pIplObj->psie->Ch0.Out.packBus = RAW_BIT;
        pIplObj->psie->Ch0.SiePPBNum = SIE_BUF_2;
        pIplObj->psie->Ch0.Src = NonScaled_Img;
        pIplObj->psie->DmaOutSizeH = pIplObj->psie->IMG_Window.Crop.SizeH;
        pIplObj->psie->DmaOutSizeV = pIplObj->psie->IMG_Window.Crop.SizeV;
        pIplObj->psie->Ch0.Out.LineOffset = pIplObj->psie->DmaOutSizeH * pIplObj->psie->Ch0.Out.packBus / 8;
        //sie subF
        pIplObj->psie->pSubFunc->sieFuncEn |= (DMAOutCh0En);
        /**
            TODO:
                1. ob value should link with ipl sample
                2. should read other calibration resullt,like ECS...etc.
        */
        //buf allocate
        CalApi()->fpBufEvent()->fpSetAddr(pCalInfo->SenMode,pCalInfo->SenMode,
        pIplObj->psie->Ch0.Out.LineOffset * pIplObj->psie->DmaOutSizeV);

        //sie addr
        CalApi()->fpBufEvent()->fpGetAddr(pCalInfo->SenMode,pCalInfo->SenMode,
        &pIplObj->psie->Ch0.Out.PPB0_Addr,NULL);
        pIplObj->psie->Ch0.Out.PPB1_Addr = pIplObj->psie->Ch0.Out.PPB0_Addr;
        m_sieUpdate |= (SIE_OutAddr_Ch0|SIE_IOSize|SIE_SubFEn|SIE_INIT|SIE_BURSTLENG);

}

#if 0
#endif
static void ECS_CtrlInit(pIPLC_INFO pIplObj,pCTRL_PTR pCtrlptr)
{
    SENSOR_INFO *pSenInfo = (SENSOR_INFO *)pCtrlptr->pDrv_Sensor;
    Cal_SENSOR_INFO *pCalInfo = (Cal_SENSOR_INFO*)pCtrlptr->pCal_Sensor;

    //clr global var.
    m_sieUpdate = 0;
    memset((void *)&m_sie,0,sizeof(SIE));
    memset((void *)&m_sieSubF,0,sizeof(SIE_SUBFUNC));

    m_sie.pSubFunc = &m_sieSubF;
    pIplObj->psie = &m_sie;
    pIplObj->psieUpdate = &m_sieUpdate;


    //sie in
    _SIESignal(pSenInfo , pCalInfo, &(pIplObj->psie->Sie_Signal));
    _SIEIn(pSenInfo , pCalInfo, &(pIplObj->psie->IMG_Window));
    _SIEAct(pSenInfo , &(pIplObj->psie->ActMode));
    _SIETrigSel(&(pIplObj->psie->TrigSel));
    _SIE_BURST_LENG(&pIplObj->psie->BurstLeng_Ch0);
    //inte
    pIplObj->psie->pSubFunc->InteEn |= (SIE__INTE_FLDEND);
        //sie output size
    pIplObj->psie->Ch0.Out.packBus = RAW_BIT;
    pIplObj->psie->Ch0.SiePPBNum = SIE_BUF_2;
    pIplObj->psie->Ch0.Src = NonScaled_Img;
    pIplObj->psie->DmaOutSizeH = pIplObj->psie->IMG_Window.Crop.SizeH;
    pIplObj->psie->DmaOutSizeV = pIplObj->psie->IMG_Window.Crop.SizeV;
    pIplObj->psie->Ch0.Out.LineOffset = pIplObj->psie->DmaOutSizeH * pIplObj->psie->Ch0.Out.packBus / 8;
    //sie subF
    pIplObj->psie->pSubFunc->sieFuncEn |= (DMAOutCh0En);
    /**
        TODO:
            1. ob value should link with ipl sample
    */
    //buf allocate
    CalApi()->fpBufEvent()->fpSetAddr(pCalInfo->SenMode,pCalInfo->SenMode,
    pIplObj->psie->Ch0.Out.LineOffset * pIplObj->psie->DmaOutSizeV);

    //sie addr
    CalApi()->fpBufEvent()->fpGetAddr(pCalInfo->SenMode,pCalInfo->SenMode,
    &pIplObj->psie->Ch0.Out.PPB0_Addr,NULL);
    pIplObj->psie->Ch0.Out.PPB1_Addr = pIplObj->psie->Ch0.Out.PPB0_Addr;
    m_sieUpdate |= (SIE_OutAddr_Ch0|SIE_IOSize|SIE_SubFEn|SIE_INIT|SIE_BURSTLENG);
}


#if 0
#endif
static void DP_CtrlInit(pIPLC_INFO pIplObj,pCTRL_PTR pCtrlptr)
{
    SENSOR_INFO *pSenInfo = (SENSOR_INFO *)pCtrlptr->pDrv_Sensor;
    Cal_SENSOR_INFO *pCalInfo = (Cal_SENSOR_INFO*)pCtrlptr->pCal_Sensor;

    //clr global var.
    m_sieUpdate = 0;
    memset((void *)&m_sie,0,sizeof(SIE));
    memset((void *)&m_sieSubF,0,sizeof(SIE_SUBFUNC));

#if 0  //sun0610
    //ecs
    CAL_TAG* pTag;

    pTag = GetCalData(SEC_ECS,IPL_AlgGetUIInfo(IPL_SEL_CALSENMODE));
    if(pTag)
    {
        if(pTag->CAL_STATUS == _CAL_OK)
        {
            CAL_ECS_FMT *pFmt;
            pFmt = (CAL_ECS_FMT *)&pTag->TagData;
            m_sieSubF.sieFuncEn |=(ECSEn);
            m_sieSubF.Ecs.EcsMap = pFmt->header.Ecs_map;
            m_Ecs.BitSel = pFmt->header.Ecs_bit;
            m_sieSubF.Ecs.pecs = &m_Ecs;
            m_sieSubF.Ecs.Inaddr = (UINT32)&pFmt->ECSBlockWin;
            m_sieUpdate |= (SIE_SubFEn|SIE_ECS_);
        }
    }

#endif

    m_sie.pSubFunc = &m_sieSubF;
    pIplObj->psie = &m_sie;
    pIplObj->psieUpdate = &m_sieUpdate;


    //sie in
    _SIESignal(pSenInfo , pCalInfo, &(pIplObj->psie->Sie_Signal));
    _SIEIn(pSenInfo , pCalInfo, &(pIplObj->psie->IMG_Window));
    _SIEAct(pSenInfo , &(pIplObj->psie->ActMode));
    _SIETrigSel(&(pIplObj->psie->TrigSel));
    _SIE_BURST_LENG(&pIplObj->psie->BurstLeng_Ch0);
    //inte
    pIplObj->psie->pSubFunc->InteEn |= (SIE__INTE_FLDEND);
        //sie output size
    pIplObj->psie->Ch0.Out.packBus = RAW_BIT;
    pIplObj->psie->Ch0.SiePPBNum = SIE_BUF_2;
    pIplObj->psie->Ch0.Src = NonScaled_Img;
    pIplObj->psie->DmaOutSizeH = pIplObj->psie->IMG_Window.Crop.SizeH;
    pIplObj->psie->DmaOutSizeV = pIplObj->psie->IMG_Window.Crop.SizeV;
    pIplObj->psie->Ch0.Out.LineOffset = pIplObj->psie->DmaOutSizeH * pIplObj->psie->Ch0.Out.packBus / 8;
    //sie subF
    pIplObj->psie->pSubFunc->sieFuncEn |= (DMAOutCh0En);

    // OB sun0610
    pIplObj->psie->pSubFunc->OBSub.Offset=0xe8;
    pIplObj->psie->pSubFunc->OBSub.DtGain=0x20;

    pIplObj->psie->pSubFunc->sieFuncEn |= OBSubSelEn;

    //buf allocate
    CalApi()->fpBufEvent()->fpSetAddr(pCalInfo->SenMode,pCalInfo->SenMode,
    pIplObj->psie->Ch0.Out.LineOffset * pIplObj->psie->DmaOutSizeV);

    //sie addr
    CalApi()->fpBufEvent()->fpGetAddr(pCalInfo->SenMode,pCalInfo->SenMode,
    &pIplObj->psie->Ch0.Out.PPB0_Addr,NULL);
    pIplObj->psie->Ch0.Out.PPB1_Addr = pIplObj->psie->Ch0.Out.PPB0_Addr;
    m_sieUpdate |= (SIE_OutAddr_Ch0|SIE_IOSize|SIE_SubFEn|SIE_INIT|SIE_BURSTLENG|SIE_OBOFS_ | SIE_SubFEn);
}


#if 0
#endif
static void MSHUT_CtrlInit(pIPLC_INFO pIplObj,pCTRL_PTR pCtrlptr)
{
    SENSOR_INFO *pSenInfo = (SENSOR_INFO *)pCtrlptr->pDrv_Sensor;
    Cal_SENSOR_INFO *pCalInfo = (Cal_SENSOR_INFO*)pCtrlptr->pCal_Sensor;

    //clr global var.
    m_sieUpdate = 0;
    memset((void *)&m_sie,0,sizeof(SIE));
    memset((void *)&m_sieSubF,0,sizeof(SIE_SUBFUNC));

    m_sie.pSubFunc = &m_sieSubF;
    pIplObj->psie = &m_sie;
    pIplObj->psieUpdate = &m_sieUpdate;


    //sie in
    _SIESignal(pSenInfo , pCalInfo, &(pIplObj->psie->Sie_Signal));
    _SIEIn(pSenInfo , pCalInfo, &(pIplObj->psie->IMG_Window));
    _SIEAct(pSenInfo , &(pIplObj->psie->ActMode));
    _SIETrigSel(&(pIplObj->psie->TrigSel));
    _SIE_BURST_LENG(&pIplObj->psie->BurstLeng_Ch0);
    //inte
    pIplObj->psie->pSubFunc->InteEn |= (SIE__INTE_FLDEND);
        //sie output size
    pIplObj->psie->Ch0.Out.packBus = RAW_BIT;
    pIplObj->psie->Ch0.SiePPBNum = SIE_BUF_2;
    pIplObj->psie->Ch0.Src = NonScaled_Img;
    pIplObj->psie->DmaOutSizeH = pIplObj->psie->IMG_Window.Crop.SizeH;
    pIplObj->psie->DmaOutSizeV = pIplObj->psie->IMG_Window.Crop.SizeV;
    pIplObj->psie->Ch0.Out.LineOffset = pIplObj->psie->DmaOutSizeH * pIplObj->psie->Ch0.Out.packBus / 8;
    //sie subF
    pIplObj->psie->pSubFunc->sieFuncEn |= (DMAOutCh0En);
    /**
        TODO:
            1. ob value should link with ipl sample
    */
    //buf allocate
    CalApi()->fpBufEvent()->fpSetAddr(pCalInfo->SenMode,pCalInfo->SenMode,
    pIplObj->psie->Ch0.Out.LineOffset * pIplObj->psie->DmaOutSizeV);

    //sie addr
    CalApi()->fpBufEvent()->fpGetAddr(pCalInfo->SenMode,pCalInfo->SenMode,
    &pIplObj->psie->Ch0.Out.PPB0_Addr,NULL);
    pIplObj->psie->Ch0.Out.PPB1_Addr = pIplObj->psie->Ch0.Out.PPB0_Addr;
    m_sieUpdate |= (SIE_OutAddr_Ch0|SIE_IOSize|SIE_SubFEn|SIE_INIT|SIE_BURSTLENG);
}


#if 0
#endif
static CAL_CTRL_OBJ m_CTRL_OBJ = {
    Prv_CtrlInit,
    ISO_CtrlInit,
    AWB_CtrlInit,
    ECS_CtrlInit,
    MSHUT_CtrlInit,
    DP_CtrlInit
};

pCAL_CTRL_OBJ Cal_CtrlInit(void)
{
    return &m_CTRL_OBJ;
}
