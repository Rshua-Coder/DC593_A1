
#include <stdio.h>
#include <string.h>
#include "AppControl.h"
//Common
#include "SysKer.h"
#include "SysCfg.h"
#include "PrjCfg.h"
#include "AppLib.h"
#include "Usb.h"

//Lib
#include "UVAC.h"
#include "GxSystem.h"
#include "FileSysTsk.h"
#include "DCF.h"
#include "JpgHeader.h"
#include "BinaryFormat.h"
#include "JpgDec.h"
#include "GxImage.h"
#include "SysCfg.h"
#include "GxUSB.h"
#include "UIAppUsbCam.h"
#include "Utility.h"
#include "Sensor.h"
#include "DxSensor.h"
#include "IPL_Cmd.h"
#include "IPL_Display.h"

#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIApp_UVAC
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#define  SKIP_RATE  10
#define  UVAC_H264_MAX_RESO_WIDTH        1920
#define  UVAC_H264_MAX_RESO_HEIGHT       1080
#define  UVAC_MJPG_MAX_RESO_WIDTH        1920 //1280
#define  UVAC_MJPG_MAX_RESO_HEIGHT       1080 //720
#define  UVAC_HD_HEIGHT                   720
#define  UVAC_FULL_HD_HEIGHT             1080
#define  UVAC_MJPG_TARGET_SIZE          (100*1024*30)  //96650 ISO bandwidth 4M

//#NT#unplug usb ==> power off
#define UNPLUG_USB_PWR_OFF    1


#if (THIS_DBGLVL >= PRJ_DBG_LVL_TRC)
UINT32 gPccGteBufCB = 0;
#endif
static MEM_RANGE m_VideoBuf;

_ALIGNED(4) static UINT16 m_UVACSerialStrDesc3[] =
{
    0x0320,                             // 20: size of String Descriptor = 32 bytes
                                        // 03: String Descriptor type
    '9', '6', '6', '5', '0',            // 96611-00000-001 (default)
    '0', '0', '0', '0', '0',
    '0', '0', '1', '0', '0'
};
_ALIGNED(4) const static UINT8 m_UVACManuStrDesc[] =
{
    USB_VENDER_DESC_STRING_LEN*2+2, // size of String Descriptor = 6 bytes
    0x03,                       // 03: String Descriptor type
    USB_VENDER_DESC_STRING
};

_ALIGNED(4) const static UINT8 m_UVACProdStrDesc[] =
{
    USB_PRODUCT_DESC_STRING_LEN*2+2, // size of String Descriptor = 6 bytes
    0x03,                       // 03: String Descriptor type
    USB_PRODUCT_DESC_STRING
};
static void xUSBMakerInit_UVAC(UVAC_INFO *pUVACInfo)
{
    pUVACInfo->pManuStringDesc = (UVAC_STRING_DESC *)m_UVACManuStrDesc;
    pUVACInfo->pProdStringDesc = (UVAC_STRING_DESC *)m_UVACProdStrDesc;

    pUVACInfo->pSerialStringDesc = (UVAC_STRING_DESC *)m_UVACSerialStrDesc3;

    pUVACInfo->VID = USB_VID;
    pUVACInfo->PID = USB_PID_PCCAM;

}
static MEM_RANGE m_IPLworkBuf;
static void  xUvacOpenIPL(PMEM_RANGE pWorkBuf)
{
    m_IPLworkBuf = *pWorkBuf;
    SENSOR_INIT_OBJ SenObj;
    SENSOR_DRVTAB *SenTab;

    SenObj = DrvSensor_GetObj1st();
    SenTab = DrvSensor_GetTab1st();
    Sensor_Open(SENSOR_ID_1, &SenObj, SenTab);
    IPL_Open();
    IPL_Display_OpenTsk();

}
static void xUvacCloseIPL(void)
{
    //#NT#unplug usb ==> power off
    #if UNPLUG_USB_PWR_OFF
    IPL_CHGMODE_DATA CmdData;
    DBG_IND("+CloUV:%d,%d\r\n",IPL_IsOpen(),GxUSB_GetIsUSBPlug());
    if (FALSE == GxUSB_GetIsUSBPlug())
    {
        CmdData.Mode = IPL_MODE_OFF;
        CmdData.BufAddr = 0;
        CmdData.BufSize = 0;
        CmdData.CB_FP = NULL;
        CmdData.ImgSizeH_Disp = 0;
        CmdData.ImgSizeV_Disp = 0;
        CmdData.ImgFmt_Disp = 0;
        CmdData.ImgP1En = DISABLE;
        CmdData.ImgP3En = DISABLE;
        IPL_SetCmd(IPL_SET_CHGMODE, (void *)&CmdData);
        IPL_WaitCmdFinish();
    }
    #endif
    IPL_Close();
    IPL_Display_CloseTsk();
    Sensor_Close(SENSOR_ID_1);
}
static UINT32 xUvacStartVideoByIPL(UINT32 videoWidth, UINT32 videoHeight, UVAC_VIDEO_FORMAT videoFmt)
{

    IPL_CHGMODE_DATA CmdData = {0};
    IPL_BUF_INFOR iplBufInfo = {0};

    CmdData.Mode = IPL_MODE_PREVIEW;
    CmdData.SenNum = 1;
    if((videoHeight == UVAC_FULL_HD_HEIGHT) || (videoHeight == UVAC_HD_HEIGHT))
    {
        CmdData.CropRatio = IPL_SENCROPRATIO_16_9;
    }
    else
    {
        CmdData.CropRatio = IPL_SENCROPRATIO_4_3;
    }
    CmdData.BufAddr = m_IPLworkBuf.Addr;
    CmdData.BufSize = m_IPLworkBuf.Size;
    CmdData.CB_FP = NULL;

    CmdData.BufSizeH_Disp = UVAC_H264_MAX_RESO_WIDTH;//videoWidth;
    CmdData.BufSizeV_Disp = UVAC_H264_MAX_RESO_HEIGHT;//videoHeight;
    CmdData.ImgSizeH_Disp = videoWidth;
    CmdData.ImgSizeV_Disp = videoHeight;

    #if 1
    debug_msg("^RPath2-420 For H264:Fmt=%d,W=%d,H=%d\r\n",videoFmt,videoWidth,videoHeight);
    CmdData.ImgFmt_Disp = IPL_IMG_Y_PACK_UV420;
    #else
    if (videoFmt == UVAC_VIDEO_FORMAT_422)
    {
        CmdData.ImgFmt_Disp = IPL_IMG_Y_PACK_UV422;
    }
    else
    {
        CmdData.ImgFmt_Disp = IPL_IMG_Y_PACK_UV420;
    }
    #endif

    CmdData.ImgP1En = DISABLE;
    #if 1
    //Set path3 for MJPG
    debug_msg("^RPath3-422 For MJPG:Fmt=%d,W=%d,H=%d\r\n",videoFmt,videoWidth,videoHeight);
    CmdData.ImgP3En = ENABLE;
    CmdData.ImgFmt_P3 = IPL_IMG_Y_PACK_UV422;
    CmdData.BufSizeH_P3 = UVAC_MJPG_MAX_RESO_WIDTH;//videoWidth;
    CmdData.BufSizeV_P3 = UVAC_MJPG_MAX_RESO_HEIGHT;//videoHeight;
    CmdData.ImgSizeH_P3 = videoWidth;
    CmdData.ImgSizeV_P3 = videoHeight;
    #else
    CmdData.ImgP3En = DISABLE;
    #endif

    CmdData.ISR_CB_FP[IPL_ISR_SIE] = NULL;
    CmdData.ISR_CB_FP[IPL_ISR_PRE] = NULL;
    CmdData.ISR_CB_FP[IPL_ISR_IFE] = NULL;
    CmdData.ISR_CB_FP[IPL_ISR_DCE] = NULL;
    CmdData.ISR_CB_FP[IPL_ISR_IPE] = NULL;
    CmdData.ISR_CB_FP[IPL_ISR_IME] = NULL;
    CmdData.ISR_CB_FP[IPL_ISR_IFE2] = NULL;
    CmdData.FuncEn = IPL_FUNC_NONE;
    debug_msg("^B%s:W=%d,H=%d,Fmt=%d\r\n",__func__,videoWidth,videoHeight,videoFmt);
    IPL_SetCmd(IPL_SET_CHGMODE, (void *)&CmdData);
    IPL_WaitCmdFinish();

    iplBufInfo.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_CUR_BUF_INFOR, &iplBufInfo);
    debug_msg("^B%s:IPL MemNeedSize=0x%x,Addr=0x%x,id=%d\r\n",__func__,iplBufInfo.BufSize,iplBufInfo.BufAddr,iplBufInfo.Id);

    return E_OK;
}
__inline UINT32 xUvacChgVideoResoH264(UINT32 width, UINT32 height)
{
    IPL_IME_DATA imeData;
    debug_msg("+%s:w=%d,%d\r\n",__func__,width,height);
    imeData.Id = IPL_ID_1;//single sensor
    imeData.PathId = IPL_IME_PATH2;
    imeData.ImeP2.ImgFmt = IPL_IMG_Y_PACK_UV420;
    imeData.ImeP2.OutputEn = ENABLE;
    imeData.ImeP2.ImgSizeV = height;
    imeData.ImeP2.ImgSizeH = width;
    imeData.ImeP2.ImgSizeLOfs = width;
    IPL_SetCmd(IPL_SET_IME, (void *)&imeData);
    IPL_WaitCmdFinish();
    return E_OK;
}

__inline UINT32 xUvacChgVideoResoMjpg(UINT32 width, UINT32 height)
{
    IPL_IME_DATA imeData;
    debug_msg("+%s:w=%d,%d\r\n",__func__,width,height);
    imeData.Id = IPL_ID_1;//single sensor
    imeData.PathId = IPL_IME_PATH3;
    imeData.ImeP3.ImgFmt = IPL_IMG_Y_PACK_UV422;
    imeData.ImeP3.OutputEn = ENABLE;
    imeData.ImeP3.ImgSizeV = height;
    imeData.ImeP3.ImgSizeH = width;
    imeData.ImeP3.ImgSizeLOfs = width;
    IPL_SetCmd(IPL_SET_IME, (void *)&imeData);
    IPL_WaitCmdFinish();
    return E_OK;
}
UINT32 xUvacChgVideoResoCB(UINT32 width, UINT32 height, UVAC_CODEC_TYPE encType)
{
    if (UVAC_CODEC_TYPE_H264 == encType)
    {
        xUvacChgVideoResoH264(width, height);
    }
    else
    {
        xUvacChgVideoResoMjpg(width, height);
    }
    return E_OK;
}
static UINT32 xUvacGetActiveFrameByIPL(UINT32 *yAddr, UINT32 *cbAddr, UINT32 *crAddr, UINT32 *lineOfsY, UVAC_CODEC_TYPE encType)
{
    IPL_IME_BUF_ADDR CurInfo;

    CurInfo.Id = IPL_ID_1;
    IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&CurInfo);

    if ((CurInfo.P2Enable == ENABLE) && (UVAC_CODEC_TYPE_H264 == encType))
    {
        *yAddr = CurInfo.ImeP2.PixelAddr[0];
        *cbAddr = CurInfo.ImeP2.PixelAddr[1];
        *crAddr = CurInfo.ImeP2.PixelAddr[2];
        *lineOfsY = CurInfo.ImeP2.Ch[0].LineOfs;
    }
    else if ((CurInfo.P3Enable == ENABLE) && (UVAC_CODEC_TYPE_MJPG == encType))
    {
        *yAddr = CurInfo.ImeP3.PixelAddr[0];
        *cbAddr = CurInfo.ImeP3.PixelAddr[1];
        *crAddr = CurInfo.ImeP3.PixelAddr[2];
        *lineOfsY = CurInfo.ImeP3.Ch[0].LineOfs;
    }
    else
    {
        *yAddr = 0;
        *cbAddr = 0;
        *crAddr = 0;
        *lineOfsY = 0;
        debug_err(("^RIPL YUV=0,enc=%d,P2=%d,P3=%d\r\n",encType,CurInfo.P2Enable,CurInfo.P3Enable));
    }
    #if (THIS_DBGLVL >= PRJ_DBG_LVL_TRC)
    if (1 > (gPccGteBufCB++ % 1000))
    {
        debug_msg("-%s:0x%x,0x%x,0x%x,0x%x,enc=%d,P2E=%d,P3E%d\r\n",__func__,*yAddr,*cbAddr,*crAddr,*lineOfsY,encType,CurInfo.P2Enable,CurInfo.P3Enable);
    }
    #endif
    return E_OK;
}
static UINT32 xUvacStopVideoCBByIPL(UINT32 isClosed)
{
    IPL_CHGMODE_DATA CmdData;
    if (TRUE == isClosed)
    {
        CmdData.Mode = IPL_MODE_OFF;
        CmdData.BufAddr = 0;
        CmdData.BufSize = 0;
        CmdData.CB_FP = NULL;
        CmdData.ImgSizeH_Disp = 0;
        CmdData.ImgSizeV_Disp = 0;
        CmdData.ImgFmt_Disp = 0;
        CmdData.ImgP1En = DISABLE;
        CmdData.ImgP3En = DISABLE;
        IPL_SetCmd(IPL_SET_CHGMODE, (void *)&CmdData);
        IPL_WaitCmdFinish();
    }
    return E_OK;
}
#if 0
#define     TEMP_FILE_BUF_SIZE  0xA00000
#define     TEMP_DECODED_BUF_SIZE   (0x2000000)//16MB x 2
static void xUvacSetTestPattern(PMEM_RANGE pWorkBuf, UINT32 ImgStartIndex, UINT32 ImgNum, UINT32 Width, UINT32 Height,UINT32 videoFmt)
{
    UINT32 uiFileNum, uiDirNum, uiFileType;
    CHAR    chaFullName[30] = { 0 };
    UINT32 RawBuf, RawBufSize, FileBuf, FileBufSize;
    UINT32 i;
    ER Ret;
    JPG_DEC_INFO  DecOneJPGInfo = {0};
    JPGHEAD_DEC_CFG     JPGCfg ={0};
    UINT32  LineOff[3], PxlAddr[3];
    GX_IMAGE_PIXEL_FMT PxlFmt, PxlFmtOut;
    IMG_BUF DecodedBuf = {0},DstImgBuf = {0};
    UINT32  ImgBuf, ImgSize;


    FileBufSize = TEMP_FILE_BUF_SIZE;
    FileBuf =  ALIGN_FLOOR_4(pWorkBuf->Addr + pWorkBuf->Size- FileBufSize);

    RawBufSize = TEMP_DECODED_BUF_SIZE;
    RawBuf = ALIGN_FLOOR_4(FileBuf - RawBufSize);

    ImgBuf = pWorkBuf->Addr;
    ImgSize = Width*Height*2;
    for(i=ImgStartIndex; i<ImgStartIndex+ImgNum; i++)
    {
        DCF_GetObjInfo(i,&uiDirNum,&uiFileNum,&uiFileType);
        DCF_GetObjPath(i,uiFileType,chaFullName);

        {
        FST_FILE  filehdl;
        filehdl = FileSys_OpenFile(chaFullName,FST_OPEN_READ);
        FileSys_ReadFile(filehdl, (UINT8 *)FileBuf, &FileBufSize, 0, NULL);
        FileSys_CloseFile(filehdl);
        }

        DecOneJPGInfo.pDstAddr          = (UINT8 *)RawBuf;
        DecOneJPGInfo.jdcfg_p           = &JPGCfg;
        DecOneJPGInfo.DecodeType     = DEC_PRIMARY;
        DecOneJPGInfo.bEnableTimeOut = FALSE;
        DecOneJPGInfo.bEnableCrop = FALSE;
        DecOneJPGInfo.pSrcAddr      = (UINT8 *)FileBuf;
        DecOneJPGInfo.JPGFileSize   = FileBufSize;
        Ret = Jpg_DecOneJPG(&DecOneJPGInfo);
        if(E_OK == Ret)
        {
            LineOff[0]= DecOneJPGInfo.jdcfg_p->lineoffsetY;
            LineOff[1]= DecOneJPGInfo.jdcfg_p->lineoffsetUV;
            PxlAddr[0]= DecOneJPGInfo.uiOutAddrY;
            PxlAddr[1]= DecOneJPGInfo.uiOutAddrCb;
            if(DecOneJPGInfo.jdcfg_p->fileformat == JPG_FMT_YUV211)
                PxlFmt = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
            else
                PxlFmt = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;

            if (videoFmt == UVAC_VIDEO_FORMAT_422)
                PxlFmtOut = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
            else
                PxlFmtOut = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
            GxImg_InitBufEx(&DecodedBuf,DecOneJPGInfo.jdcfg_p->imagewidth,DecOneJPGInfo.jdcfg_p->imageheight,PxlFmt,LineOff,PxlAddr);
            GxImg_InitBuf(&DstImgBuf,Width,Height,PxlFmtOut,GXIMAGE_LINEOFFSET_ALIGN(16),ImgBuf+(i-ImgStartIndex)*ImgSize,ImgSize);
            GxImg_ScaleDataFine(&DecodedBuf, REGION_MATCH_IMG, &DstImgBuf, REGION_MATCH_IMG);
        }

    }
}
#endif
static UINT32 xUvacStartVideoCB(UINT32 videoWidth, UINT32 videoHeight, UVAC_VIDEO_FORMAT videoFmt)
{
    DBG_IND("Vid W=%d,H=%d,Fmt=%d\r\n",videoWidth,videoHeight,videoFmt);
    xUvacStartVideoByIPL(videoWidth, videoHeight, videoFmt);
    return E_OK;
}
static UINT32 xUvacGetVideoBufCB(UINT32 *yAddr, UINT32 *cbAddr, UINT32 *crAddr, UINT32 *lineOfsY, UVAC_CODEC_TYPE encType)
{
    xUvacGetActiveFrameByIPL(yAddr, cbAddr, crAddr, lineOfsY, encType);
    //DBG_IND("Addr Y=0x%X,Cb=0x%X,Cr=0x%X\r\n",*yAddr,*cbAddr,*crAddr);
    return E_OK;
}
static UINT32 xUvacStopVideoCB(UINT32 isClosed)
{
    DBG_IND(":isClosed=%d\r\n",isClosed);
    xUvacStopVideoCBByIPL(isClosed);
    return E_OK;
}
INT32 UVACExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    DBG_IND(" ++\r\n");
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_OPEN,paramNum,paramArray);
    if(GxUSB_GetIsUSBPlug())
    {
        UVAC_INFO       UvacInfo = {0};
        UINT32          uiPoolAddr;
        UINT32          retV = 0;

        #if 0
        uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO1);
        UvacInfo.UvacMemAdr = uiPoolAddr;
        UvacInfo.UvacMemSize = UVAC_GetNeedMemSize();
        m_VideoBuf.Addr = uiPoolAddr + UvacInfo.UvacMemSize;
        m_VideoBuf.Size = POOL_SIZE_APP - UVAC_GetNeedMemSize();
        #else
        uiPoolAddr = OS_GetMempoolAddr(POOL_ID_APP);
        m_VideoBuf.Addr = uiPoolAddr;
        m_VideoBuf.Size = POOL_SIZE_APP - UVAC_GetNeedMemSize();
        UvacInfo.UvacMemAdr    = uiPoolAddr + POOL_SIZE_APP - UVAC_GetNeedMemSize();
        UvacInfo.UvacMemSize   = UVAC_GetNeedMemSize();
        #endif
        xUvacOpenIPL(&m_VideoBuf);

        UvacInfo.fpStartVideoCB  = xUvacStartVideoCB;
        UvacInfo.fpGetVideoBufCB = xUvacGetVideoBufCB;
        UvacInfo.fpStopVideoCB  = xUvacStopVideoCB;
        UvacInfo.fpChgVideoReso = xUvacChgVideoResoCB;
        xUSBMakerInit_UVAC(&UvacInfo);

        debug_msg("^R%s:uvacAddr=0x%x,s=0x%x;IPLAddr=0x%x,s=0x%x\r\n",__func__,UvacInfo.UvacMemAdr,UvacInfo.UvacMemSize,m_VideoBuf.Addr,m_VideoBuf.Size);

        UVAC_SetConfig(UVAC_CONFIG_MJPG_TARGET_SIZE, UVAC_MJPG_TARGET_SIZE);
        UVAC_SetConfig(UVAC_CONFIG_VID_TRIG_TYPE, UVAC_TRIG_GETVID_TYPE_AUD);
        retV = UVAC_Open(&UvacInfo);
        if (retV != E_OK)
        {
            debug_err(("Error open UVAC task\r\n", retV));
        }

    }
    else
    {
        DBG_ERR("USB NOT connected!\r\n");
    }
    return NVTEVT_CONSUME;
}
INT32 UVACExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DBG_IND(" +\r\n");
    UVAC_Close();
    xUvacCloseIPL();
    DBG_IND(" -\r\n");
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_CLOSE,paramNum,paramArray);
    DBG_IND(" ---\r\n");
    return NVTEVT_CONSUME;
}

////////////////////////////////////////////////////////////
EVENT_ENTRY CustomUSBPCCObjCmdMap[] =
{
    {NVTEVT_EXE_OPEN,               UVACExe_OnOpen},
    {NVTEVT_EXE_CLOSE,              UVACExe_OnClose},
    {NVTEVT_NULL,                   0},  //End of Command Map
};

CREATE_APP(CustomUSBPCCObj, APP_PCC)

