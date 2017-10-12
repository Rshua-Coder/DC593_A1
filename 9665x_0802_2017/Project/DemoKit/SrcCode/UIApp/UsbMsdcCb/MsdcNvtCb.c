#include "SysCfg.h"
#include "UMSDC.h"
#include "MsdcNvtApi.h"
#include "MsdcNvtCb.h"
#include "SxCmd.h"
#include "DxStorage.h"

#if(MSDCVENDOR_NVT==ENABLE)

//------------------------------------------------------------------------------
// Multi Module Selection
//------------------------------------------------------------------------------
#define CFG_MSDC_NVT_CB_PHOTO   ENABLE  //!< Preview / Capture
#define CFG_MSDC_NVT_CB_DISP    ENABLE  //!< Display Engine Adjustment (IDE Gamma)
#define CFG_MSDC_NVT_CB_FILE    ENABLE  //!< Remote File System
#define CFG_MSDC_NVT_CB_ADJ     ENABLE  //!< Universal Adjustment
#define CFG_MSDC_NVT_CB_UPDFW   ENABLE  //!< Update Fw

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Single Module Selection
//------------------------------------------------------------------------------
#define SI_MODULE_NONE          0  //!< No Single Module
#define SI_MODULE_CUSTOM_SI     1  //!< Customer Develop Module
#define SI_MODULE_IQSIM         2
#define SI_MODULE_AE            3
//Configure your Single Module. Module can be SI_MODULE_NONE / SI_MODULE_CUSTOM_SI / SI_MODULE_IQSIM..etc
#define CFG_SI_MODULE SI_MODULE_CUSTOM_SI
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Cache Memory Selection
//------------------------------------------------------------------------------
#define CACHE_USE_LOCAL             0
#define CACHE_USE_MEM_POOL          1

//Configure your Cache use Local Array or Memory Pool
#define CFG_CACHE_USE               CACHE_USE_LOCAL //!< CACHE_USE_MEM_POOL / CACHE_USE_LOCAL
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Config Multi Module Include Headers
//------------------------------------------------------------------------------
#if(CFG_MSDC_NVT_CB_PHOTO==ENABLE)
#include "MsdcNvtCb_Photo.h"
#endif
#if(CFG_MSDC_NVT_CB_DISP==ENABLE)
#include "MsdcNvtCb_Disp.h"
#endif
#if(CFG_MSDC_NVT_CB_FILE==ENABLE)
#include "MsdcNvtCb_File.h"
#endif
#if(CFG_MSDC_NVT_CB_ADJ==ENABLE)
#include "MsdcNvtCb_Adj.h"
#endif
#if(CFG_MSDC_NVT_CB_UPDFW==ENABLE)
#include "MsdcNvtCb_UpdFw.h"
#endif

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Config Single Module Include Headers
//------------------------------------------------------------------------------
#if(CFG_SI_MODULE==SI_MODULE_CUSTOM_SI)
#include "MsdcNvtCb_CustomSi.h"
#elif (CFG_SI_MODULE==SI_MODULE_IQSIM)
#include "MsdcNvtCb_IQSim.h"
#elif (CFG_SI_MODULE==SI_MODULE_AE)
#include "Ae_Alg.h"
#endif

#if(CFG_CACHE_USE==CACHE_USE_LOCAL)
_ALIGNED(4) static UINT8 m_MsdcNvtCache[MSDCNVT_REQUIRE_MIN_SIZE];
#endif

void MsdcNvtCb_Open(MSDCNVTCB_OPEN* pOpen)
{
    UINT8*              pMemPool = NULL;
    UINT32              MemSize = 0;
    MSDCNVT_INIT        MsdcNvtInit = {0};
    USB_MSDC_INFO       MSDCInfo = {0};
    MSDCNVT_LUN*        pLun = MsdcNvt_GetNullLun();

    //Due to PC Limit Max Packet Size is 64k Bytes, so MsdcVendor work need to cache buffer with 0x24000 bytes
    #if(CFG_CACHE_USE==CACHE_USE_LOCAL)
    pMemPool    = m_MsdcNvtCache;
    MemSize     = sizeof(m_MsdcNvtCache);
    #elif(CFG_CACHE_USE==CACHE_USE_MEM_POOL)
    pMemPool = (UINT8*)OS_GetMempoolAddr(POOL_ID_CACHE);
    pMemPool    += POOL_CACHE_OFFSET_MSDC_DBGSYS;
    MemSize     = POOL_SIZE_CACHE_MSDC_DBGSYS;
    #else
        #error "You have to select a cache source!"
    #endif

    MsdcNvtInit.uiApiVer         = MSDCNVT_API_VERSION;
    MsdcNvtInit.pMemCache        = (UINT8*)pMemPool;
    MsdcNvtInit.uiSizeCache      = MemSize;
    MsdcNvtInit.bHookDbgMsg      = TRUE;
    MsdcNvtInit.uiUartIdx        = 0;
    if(!MsdcNvt_Init(&MsdcNvtInit))
    {
        debug_err(("MsdcNvt_Init() Failed!\r\n"));
        return;
    }

    //Register Bi - Direction Functions
    #if(CFG_MSDC_NVT_CB_PHOTO==ENABLE)
    MsdcNvtRegBi_Photo();
    #endif
    #if(CFG_MSDC_NVT_CB_DISP==ENABLE)
    MsdcNvtRegBi_Disp();
    #endif
    #if(CFG_MSDC_NVT_CB_FILE==ENABLE)
    {
        MSDCNVTCBFILE_INIT Init={0};
        Init.uiWorkingAddr = OS_GetMempoolAddr(POOL_ID_APP);
        Init.uiWorkingSize = POOL_SIZE_APP;
        MsdcNvtCbFile_Init(&Init);
        MsdcNvtRegBi_File();
    }
    #endif
    #if(CFG_MSDC_NVT_CB_ADJ==ENABLE)
    MsdcNvtRegBi_Adj();
    #endif
    #if(CFG_MSDC_NVT_CB_UPDFW==ENABLE)
    MsdcNvtRegBi_UpdFw();
    #endif

    //Register Single Direction Functions
    #if(CFG_SI_MODULE==SI_MODULE_CUSTOM_SI)
    MsdcNvtRegSi_CustomSi();
    #elif (CFG_SI_MODULE==SI_MODULE_IQSIM)
    MsdcNvtRegSi_IQSim();
    #elif (CFG_SI_MODULE==SI_MODULE_AE)
    AEAlg2_MsdcCB();
    #endif

    MSDCInfo.uiMsdcBufAddr = (UINT32)pMemPool;
    MSDCInfo.uiMsdcBufSize = (UINT32)MemSize;
    pOpen->fpUSBMakerInit(&MSDCInfo);

    //The callback functions for the MSDC Vendor command.
    //If project doesn't need the MSDC Vendor command, set this callback function as NULL.
    MSDCInfo.msdc_check_cb = MsdcNvt_Verify_Cb;
    MSDCInfo.msdc_vendor_cb = MsdcNvt_Vendor_Cb;

    //Assign a Null Lun
    MSDCInfo.pStrgHandle[0] = pLun->hStrg;
    MSDCInfo.msdc_type[0] = pLun->Type;
    MSDCInfo.msdc_storage_detCB[0] = pLun->fpStrgDetCb;
    MSDCInfo.msdc_strgLock_detCB[0] = pLun->fpStrgLockCb;
    MSDCInfo.LUNs = pLun->uiLUNs;

    // Open MSDC task
    Msdc_SetConfig(USBMSDC_CONFIG_ID_RC_DISABLESTALL, TRUE);
    if (Msdc_Open(&MSDCInfo) != E_OK)
    {
        debug_err(("Error open USB MSDC task\r\n"));
    }
}

void MsdcNvtCb_Close(void)
{
    MsdcNvt_Exit();
    Msdc_Close();
}

void MsdcNvtCb_Attach(USB_MSDC_INFO* pInfo)
{
    MSDCNVT_INIT MsdcNvtInit = {0};

    MsdcNvtInit.uiApiVer         = MSDCNVT_API_VERSION;
    MsdcNvtInit.pMemCache        = (UINT8*)(pInfo->uiMsdcBufAddr);
    MsdcNvtInit.uiSizeCache      = pInfo->uiMsdcBufSize;
    if(!MsdcNvt_Init(&MsdcNvtInit))
    {
        debug_err(("MsdcNvt_Init() Failed!\r\n"));
        return;
    }

    pInfo->msdc_check_cb = MsdcNvt_Verify_Cb;
    pInfo->msdc_vendor_cb = MsdcNvt_Vendor_Cb;
}

#endif //(MSDCVENDOR_NVT==ENABLE)
