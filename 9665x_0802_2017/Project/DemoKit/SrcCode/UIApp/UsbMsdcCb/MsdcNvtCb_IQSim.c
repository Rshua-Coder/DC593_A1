#include "SysKer.h"
#include "MsdcNvtApi.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_IQSim.h"
#include <string.h>
#include "IPL_cmd.h"
#include "IPL_AlgInfor.h"
#include "SysCfg.h"
#include "FileSysTsk.h"
#include "ImgCaptureAPI.h"
#include "ImgCapInfor.h"
#include "Utility.h"
#include "DCF.h"
#include "IQSim_Common.h"
//#include "Dep_Exif.h"
//#include "Dep_Fs.h"
//#include "Dep_DispSrv.h"
//#include "IPL_Display.h"

#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          MsdcIQSim
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define IQSIM_MAX_BIN_SIZE 0x100000
#define IQSIM_MAX_CH0_SIZE 0x1800000
#define IQSIM_MAX_CH2_SIZE 0x1800000

void IQSim_GetBinAddr(void);
void IQSim_GetRawCh0Addr(void);
void IQSim_GetRawCh2Addr(void);
void IQSim_GetJpgAddr(void);
void IQSim_GetJpgOk(void);
void IQSim_GetJpgSize(void);
void IQSim_SetConvertInit(void);
void IQSim_SetConvertProc(void);
void IQSim_SetConvertEnd(void);
void IQSim_GetQVAddr(void);
void IQSim_GetQVSize(void);
void IQSim_GetQVFmt(void);
void IQSim_GetPRIAddr(void);
void IQSim_GetPRISize(void);
void IQSim_GetPRIFmt(void);

void SetTestAppInfo(void);

//Step 2: Create your function mapping table for 'Get' Command
static void (*MSDCIQSimGet[])(void)=
{
        IQSim_GetBinAddr,     // 0 - get input bin address
        IQSim_GetRawCh0Addr,  // 1 - get input raw address
        IQSim_GetRawCh2Addr,  // 2 - get input raw address
        IQSim_GetJpgAddr,     // 3 - get output JPG address
        IQSim_GetJpgOk,       // 4 - get output JPG status
        IQSim_GetJpgSize,     // 5 - get output JPG size
        IQSim_GetQVAddr,      // 6 - get output Quickview address
        IQSim_GetQVSize,      // 7 - get output Quickview size
        IQSim_GetQVFmt,       // 8 - get output Quickview size
        IQSim_GetPRIAddr,     // 9 - get output Primary Image address
        IQSim_GetPRISize,     // 10- get output Primary Image size
        IQSim_GetPRIFmt,      // 11- get output Primary Image size

};

//Step 3: Create your function mapping table for 'Set' Command
static void (*MSDCIQSimSet[])(void)=
{
        IQSim_SetConvertInit,
        IQSim_SetConvertProc,
        IQSim_SetConvertEnd,
};


BOOL MsdcNvtRegSi_IQSim(void)
{
    return MsdcNvt_AddCallback_Si(MSDCIQSimGet,sizeof(MSDCIQSimGet)/sizeof(MSDCIQSimGet[0]),MSDCIQSimSet,sizeof(MSDCIQSimSet)/sizeof(MSDCIQSimSet[0]));

}

void IQSim_GetBinAddr(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;
    pData[0] = (UINT32)IQSIM_GetAppInfo();
}

void IQSim_GetRawCh0Addr(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;
    UINT32 ch0, ch2;
    IQSIM_GetRawAddr(&ch0, &ch2);
    pData[0] = ch0;
}

void IQSim_GetRawCh2Addr(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;
    UINT32 ch0, ch2;
    IQSIM_GetRawAddr(&ch0, &ch2);
    pData[0] = ch2;
}


void IQSim_GetJpgAddr(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size;
    ImgCap_GetCurJpgInfo(&Addr, &Size);
    pData[0] = Addr;
    DBG_ERR("IQSim_GetJpgAddr return %0x08x\r\n", pData[0]);
}

void IQSim_GetJpgSize(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size;
    ImgCap_GetCurJpgInfo(&Addr, &Size);
    pData[0] = Size;
    DBG_ERR("IQSim_GetJpgSize return %d\r\n", pData[0]);
}


void IQSim_GetQVAddr(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size, Fmt;
    ImgCap_GetCurQVInfo(&Addr, &Size, &Fmt);
    pData[0] = Addr;
    DBG_ERR("IQSim_GetQVAddr return %0x08x\r\n", pData[0]);
}

void IQSim_GetQVSize(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size, Fmt;
    ImgCap_GetCurQVInfo(&Addr, &Size, &Fmt);
    pData[0] = Size;
    DBG_ERR("IQSim_GetQVSize return %d\r\n", pData[0]);
}

void IQSim_GetQVFmt(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size, Fmt;
    ImgCap_GetCurQVInfo(&Addr, &Size, &Fmt);
    pData[0] = Fmt;
    DBG_ERR("IQSim_GetQVFmt return %d\r\n", pData[0]);
}


void IQSim_GetPRIAddr(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size, Fmt;
    ImgCap_GetCurPRIInfo(&Addr, &Size, &Fmt);
    pData[0] = Addr;
    DBG_ERR("IQSim_GetPRIAddr return %0x08x\r\n", pData[0]);
}

void IQSim_GetPRISize(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size, Fmt;
    ImgCap_GetCurPRIInfo(&Addr, &Size, &Fmt);
    pData[0] = Size;
    DBG_ERR("IQSim_GetPRISize return %d\r\n", pData[0]);
}

void IQSim_GetPRIFmt(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size, Fmt;
    ImgCap_GetCurPRIInfo(&Addr, &Size, &Fmt);
    pData[0] = Fmt;
    DBG_ERR("IQSim_GetPRIFmt return %d\r\n", pData[0]);
}

extern ER ImgCap_RawIsIdle(void);
extern ER ImgProc_IsIdle(void);
extern ER ImgEnc_IsIdle(void);

void IQSim_GetJpgOk(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;
    if ( ImgCap_RawIsIdle() == E_OK && ImgProc_IsIdle() == E_OK && ImgEnc_IsIdle() == E_OK )
        pData[0] = TRUE;
    else
        pData[0] = FALSE;
    DBG_ERR("IQSim_GetJpgOk return %d\r\n", pData[0]);
}

void IQSim_SetConvertInit(void)
{
    UINT32 uiPoolAddr;
    CAP_OBJ_INFO CapObj = {0};
    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_APP);

#if 0
    UINT32 BinAddr = uiPoolAddr;
    UINT32 Ch0Addr = BinAddr+IQSIM_MAX_BIN_SIZE;
    UINT32 Ch2Addr = Ch0Addr+IQSIM_MAX_CH0_SIZE;
    UINT32 uiBufferSize = IQSIM_MAX_BIN_SIZE+IQSIM_MAX_CH0_SIZE+IQSIM_MAX_CH2_SIZE;
#else
    UINT32 BinAddr = uiPoolAddr;
    UINT32 Ch2Addr = uiPoolAddr+POOL_SIZE_APP-IQSIM_MAX_CH2_SIZE;
    UINT32 Ch0Addr = Ch2Addr-IQSIM_MAX_CH0_SIZE;
    UINT32 uiBufferSize = IQSIM_MAX_BIN_SIZE;
#endif

    DBG_ERR("BinAddr = %X\r\n", BinAddr);
    DBG_ERR("Ch0Addr = %X\r\n", Ch0Addr);
    DBG_ERR("Ch2Addr = %X\r\n", Ch2Addr);
    DBG_ERR("uiBufferSize = %X\r\n", uiBufferSize);

    //enter
    IPL_Open();
   // IPL_Display_OpenTsk();
    CapObj.uiBufAddr = uiPoolAddr + uiBufferSize;
    CapObj.uiBufSize = POOL_SIZE_APP - uiBufferSize;
    CapObj.S2DetFP = NULL;
    CapObj.CBMsgFP = NULL;
    CapObj.WriteFileFP = NULL;
    ImgCap_Open(&CapObj);

    IQSIM_SetAppInfo(BinAddr);
    IQSIM_SetRawAddr(Ch0Addr, Ch2Addr);

#if 0
    Dep_Fs_Init(TRUE,TRUE);
    Dep_Exif_Init();
#endif
}

void IQSim_SetConvertProc(void)
{
    IQSIM_APPINFO *AppInfo;

    IQSIM_ReorgAppInfo();

    AppInfo = (IQSIM_APPINFO *)IQSIM_GetAppInfo();
    ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_SIM);
    ImgCap_SetUIInfo(CAP_SEL_PICNUM, 1);
    ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 1);
    ImgCap_SetUIInfo(CAP_SEL_JPG_BUF_NUM,1);
    ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
    ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
    ImgCap_SetUIInfo(CAP_SEL_ENCRATIO, 100);
    ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_H_SIZE, AppInfo->IQSimJpgInfo.Size.w);
    ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_V_SIZE, AppInfo->IQSimJpgInfo.Size.h);

#if 0
    ImgCap_SetUIInfo(CAP_SEL_QV_BUF_H_SIZE, AppInfo->IQSimQVInfo.RawInfo.Size.w);
    ImgCap_SetUIInfo(CAP_SEL_QV_BUF_V_SIZE, AppInfo->IQSimQVInfo.RawInfo.Size.h);
    ImgCap_SetUIInfo(CAP_SEL_QV_IMG_H_SIZE, AppInfo->IQSimQVInfo.RawInfo.Size.w);
    ImgCap_SetUIInfo(CAP_SEL_QV_IMG_V_SIZE, AppInfo->IQSimQVInfo.RawInfo.Size.h);
#endif

//    ImgCap_SetUIInfo(CAP_SEL_QV_BUF_H_SIZE, 640);
//    ImgCap_SetUIInfo(CAP_SEL_QV_BUF_V_SIZE, 480);
    ImgCap_SetUIInfo(CAP_SEL_QV_IMG_H_SIZE, 640);
    ImgCap_SetUIInfo(CAP_SEL_QV_IMG_V_SIZE, 480);
    ImgCap_SetUIInfo(CAP_SEL_REENCTYPE, SEL_REENCTYPE_FIXQ);
    ImgCap_SetUIInfo(CAP_SEL_QUALITY, AppInfo->IQSimJpgInfo.BitRate);

    DBG_ERR("Pri SubRatio = %d\r\n", AppInfo->IQSimPriInfo.IME_RetInex.SubRatio);
    DBG_ERR("QV  SubRatio = %d\r\n", AppInfo->IQSimQVInfo.IME_RetInex.SubRatio);

    DBG_ERR("JPG BitRate = %d\r\n", AppInfo->IQSimJpgInfo.BitRate);

    ImgCap_StartCapture();
}

void IQSim_SetConvertEnd(void)
{
    DBG_ERR("IQSim_SetConvertEnd\r\n");

    //exit
#if 0
    Dep_Fs_Init(TRUE,TRUE);
    Dep_Exif_Init();

    char    filename1[64];
    FST_FILE    pFileHdl;
    UINT32 JpgAddr, JpgSize;
    ImgCap_WaitFinish(); //APP Func

    ImgCap_GetCurJpgInfo(&JpgAddr, &JpgSize);
    DBG_ERR("JPGADD = 0x%08x, JPGSIZE = %d\r\n", JpgAddr, JpgSize);
    sprintf(filename1, "A:\\IQSIM\\IQSIM.JPG");
    pFileHdl = FileSys_OpenFile(filename1,FST_CREATE_ALWAYS|FST_OPEN_WRITE);
    FileSys_WriteFile(pFileHdl, (UINT8*)JpgAddr, &JpgSize, 0, NULL);
    FileSys_CloseFile(pFileHdl);
    Dep_Fs_Exit();
#endif

    ImgCap_Close();
    IPL_Close();
    //IPL_Display_CloseTsk();
}

