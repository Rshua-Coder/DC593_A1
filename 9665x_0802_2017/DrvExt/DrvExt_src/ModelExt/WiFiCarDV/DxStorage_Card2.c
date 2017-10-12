/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanSD.c
    @ingroup    mIPRJAPKeyIO

    @brief      Detect SD card2 status
                Detect SD card2 insert/remove, lock/unlock

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "Type.h"
#include "DxCfg.h"
#include "IOCfg.h"

#include "DxStorage.h"
#include "DxCommon.h"
#include "DxApi.h"

#include "Sdio.h"
#include "SdmmcDesc.h"
#include "Strgdef.h"
#include "Debug.h"
#include "Utility.h"
#include "pll.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

void DrvCARD2_EnableCardPower(BOOL bEn);
BOOL DrvCARD2_DetStrgCard(void);
BOOL DrvCARD2_DetStrgCardWP(void);

//public func
UINT32 DrvCARD2Getcaps(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvCARD2Init(void* pInitParam); // Set Init Parameters
UINT32 DrvCARD2Open(void); // Common Constructor
UINT32 DrvCARD2Close(void); // Common Destructor
UINT32 DrvCARD2State(UINT32 StateID, UINT32 Value); // General Properties
UINT32 DrvCARD2Control(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
UINT32 DrvCARD2Command(CHAR *pcCmdStr); //General Command Console

//dx object
DX_OBJECT gDevCARD2 =
{
    DXFLAG_SIGN,
    DX_CLASS_NULL,
    STORAGE_VER,
    "Storage_Card2",
    0,0,0,0,
    DrvCARD2Getcaps,
    0,
    DrvCARD2Init,
    DrvCARD2Open,
    DrvCARD2Close,
    DrvCARD2State,
    DrvCARD2Control,
    DrvCARD2Command,
    0,
};

UINT32 DrvCARD2Getcaps(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    case STORAGE_CAPS_BASE:
        DBG_IND("get card2 plug caps\r\n");
        v = STORAGE_BF_DETPLUG|STORAGE_BF_DETWP;
        break;
    case STORAGE_CAPS_HANDLE:
        v = (UINT32)sdio2_getStorageObject(STRG_OBJ_FAT1);
        break;
    default:
        break;
    }
    return v;
}

UINT32 DrvCARD2Init(void* pInitParam) // Set Init Parameters
{
    PSTORAGE_OBJ   pStrg;
    DXSTRG_INIT *g_pInit = (DXSTRG_INIT*)pInitParam; //fat and pstore not complete

    DBG_FUNC_BEGIN("\r\n");

    pStrg = sdio2_getStorageObject(STRG_OBJ_FAT1);
    sdio2_setCallBack(SDIO_CALLBACK_CARD_DETECT, (SDIO_CALLBACK_HDL)DrvCARD2_DetStrgCard);
    sdio2_setCallBack(SDIO_CALLBACK_WRITE_PROTECT, (SDIO_CALLBACK_HDL)DrvCARD2_DetStrgCardWP);

    /*
    // Enable High Speed mode
    //sdio_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE, (UINT32)(SDIO_BUS_SPEED_MODE_DS | SDIO_BUS_SPEED_MODE_HS|SDIO_BUS_SPEED_MODE_SDR12|SDIO_BUS_SPEED_MODE_SDR25|SDIO_BUS_SPEED_MODE_SDR50));
    sdio_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE, (UINT32)(SDIO_BUS_SPEED_MODE_DS | SDIO_BUS_SPEED_MODE_HS|SDIO_BUS_SPEED_MODE_SDR12|SDIO_BUS_SPEED_MODE_SDR25|SDIO_BUS_SPEED_MODE_DDR50));
    // Enable setting driving sink of clock pin
    sdio_setConfig(SDIO_CONFIG_ID_DRIVING_SINK_EN, (UINT32)TRUE);

    // Default speed driving -> 5.0 mA
    sdio_setConfig(SDIO_CONFIG_ID_DS_DRIVING, 50);
    // High speed driving    -> 15.0 mA
    //sdio_setConfig(SDIO_CONFIG_ID_HS_DRIVING, 150);
    // Set clock source
    sdio_setConfig(SDIO_CONFIG_ID_SRCLK, PLL_CLKSEL_SDIO_48);
    // Set maximum bus width to 8 bits
    sdio_setConfig(SDIO_CONFIG_ID_BUS_WIDTH, SD_HOST_BUS_8_BITS);
    */
#if 1
    #if 1 //best mode: after confirm SDIO_CONFIG_ID_HS_DRIVING setting
    // High speed driving    -> 15 mA
    //sdio2_setConfig(SDIO_CONFIG_ID_HS_DRIVING, 150); //for DEMO (MS)
    //sdio2_setConfig(SDIO_CONFIG_ID_HS_MAX_CLK, 24000000); //default for high speed
    #else //safe mode: before confirm SDIO_CONFIG_ID_HS_DRIVING setting
    // Default driving    -> 10 mA
    sdio_setConfig(SDIO_CONFIG_ID_HS_DRIVING, 100); //default for unknown
    sdio_setConfig(SDIO_CONFIG_ID_HS_MAX_CLK, 24000000); //safe speed
    #endif
    //sdio_setCallBack(SDIO_CALLBACK_POWER_CONTROL, (SDIO_CALLBACK_HDL)DrvCARD2_EnableCardPower); //(to improve card capability)
#endif
    //if(STRG_OBJ_FAT1)
    {
        pStrg->SetParam(STRG_SET_MEMORY_REGION, g_pInit->buf.Addr, g_pInit->buf.Size);
    }

    return DX_OK;
}

UINT32 DrvCARD2Open(void) // Common Constructor
{
    DBG_FUNC_BEGIN("\r\n");
#if 0
    PSTRG_TAB       pStorageObj;
#endif
#if 0
    {
        STORAGE_OBJ* pStrg = sdio_getStorageObject(STRG_OBJ_FAT1);
        pStrg->Lock();
        pStrg->Open();
        pStrg->Unlock();
    }
#endif
    DrvCARD2_EnableCardPower(TRUE);

    return DX_OK;
}

UINT32 DrvCARD2Close(void) // Common Destructor
{
    DBG_FUNC_BEGIN("\r\n");

#if 0
    {
        STORAGE_OBJ* pStrg = sdio_getStorageObject(STRG_OBJ_FAT1);
        pStrg->Lock();
        pStrg->Close();
        pStrg->Unlock();
    }
#endif
    DrvCARD2_EnableCardPower(FALSE);

    return DX_OK;
}

UINT32 DrvCARD2State(UINT32 StateID, UINT32 Value) // General Properties
{
    DBG_FUNC_BEGIN("\r\n");

    if(StateID & DXGET)
    {
    UINT32 rvalue =  0;
    StateID &= ~DXGET;
    DBG_IND("get %08x\r\n", StateID);
    switch(StateID)
    {
    case STORAGE_STATE_INSERT:
        rvalue = DrvCARD2_DetStrgCard();
        break;
    case STORAGE_STATE_LOCK:
        {
        BOOL bLock = FALSE;
        if(DrvCARD2_DetStrgCard())
        {
            bLock = DrvCARD2_DetStrgCardWP();
        }
        rvalue = bLock;
        }
        break;
        break;
    default:
        DBG_ERR("state=0x%02X not support!\r\n", StateID);
        break;
    }
    return rvalue;
    }
    else if(StateID & DXSET)
    {
    StateID &= ~DXSET;
    DBG_IND("set %08x\r\n", StateID);
    switch(StateID)
    {
    default:
        DBG_ERR("state=0x%02X not support!\r\n", StateID);
        break;
    }
    }
    return DX_OK;
}

UINT32 DrvCARD2Control(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
{
    DBG_FUNC_BEGIN("\r\n");
    DBG_IND("ctrl %08x\r\n", CtrlID);

    switch(CtrlID)
    {
    case STORAGE_CTRL_POWER:
        DrvCARD2_EnableCardPower(Param1);
        break;
    default:
        DBG_ERR("ctrlid=0x%02X not support!\r\n", CtrlID);
        break;
    }
    return DX_OK;
}

UINT32 DrvCARD2Command(CHAR *pcCmdStr) //General Command Console
{
    switch (*pcCmdStr)
    {
    case 'd':
        /*if (!strncmp(pcCmdStr, "Card2 dump", 9))
        {
            return TRUE;
        }*/
        break;
    }
#if 0
    sdio_printCardInformation();
#endif
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// GPIO related

#define KEYSCAN_CD_GPIO_INT     GPIO_INT_00
#define KEYSCAN_CARDDET_INT     DISABLE
#define KEYSCAN_SDWRPTDET_GPIO  DISABLE

void DrvCARD2_EnableCardPower(BOOL bEn)
{
#if 0
    // add Card Power Control
    if(bEn)
    {
        gpio_clearPin(GPIO_CARD_POWER);
        Delay_DelayMs(20);
    }
    else
    {

        gpio_setPin(GPIO_CARD_POWER);
        Delay_DelayMs(350);
    }
#endif
}

#if (KEYSCAN_CARDDET_INT == ENABLE)
static volatile UINT32  uiStrgCardIntCnt  = 0;

/**
  ISR of SD card detection

  ISR of SD card detection

  @param void
  @return void
*/
static void KeyScan_DetStrgCardIsr(UINT32 uiEvent)
{
    uiStrgCardIntCnt++;

    // Debounce
    if (uiStrgCardIntCnt > 1)
    {
        uiStrgCardIntCnt = 0;
    }
}
#endif

void DrvCARD2_SetInsert(UINT32 status)
{
    #if (KEYSCAN_CARDDET_INT == ENABLE)
    if(status)
        gpio_setIntTypePol(KEYSCAN_CD_GPIO_INT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    else
        gpio_setIntTypePol(KEYSCAN_CD_GPIO_INT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_NEGLOW);
    #endif
}

void DrvCARD2_DetRestart(void)
{
    #if (KEYSCAN_CARDDET_INT == ENABLE)
    gpio_setIntIsr(KEYSCAN_CD_GPIO_INT, KeyScan_DetStrgCardIsr);
    gpio_enableInt(KEYSCAN_CD_GPIO_INT);
    #endif
}

/**
  Detect Storage card2 is inserted or not

  Detect Storage card2 is inserted or not.
  Return TRUE if storage card2 is inserted, FALSE if storage card2 is removed.

  @param void
  @return BOOL: TRUE -> Storage card2 inserted, FALSE -> Storage card2 removed
*/
BOOL DrvCARD2_DetStrgCard(void)
{
#if 1
    return (gpio_getPin(GPIO_CARD_DETECT) == 0 ? TRUE : FALSE);
#else
    return TRUE;
#endif
}

/**
  Detect Storage card is write protected or not

  Detect Storage card is write protected or not.
  Return TRUE if storage card is write protected, FALSE if storage card is not write protected.

  @param void
  @return BOOL: TRUE -> Storage card is write protected, FALSE -> Storage card is not write protected
*/
BOOL DrvCARD2_DetStrgCardWP(void)
{
#if 0
    return (gpio_getPin(GPIO_CARD_WP) == 1 ? TRUE : FALSE );
#else
    return FALSE;
#endif
}


//@}
