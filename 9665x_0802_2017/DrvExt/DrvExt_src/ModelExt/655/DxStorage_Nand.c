#include "type.h"
#include "DxCfg.h"
#include "IOCfg.h"

#include "DxStorage.h"
#include "DxCommon.h"
#include "DxApi.h"

#include "debug.h"
#include "spiflash.h"
#include "Sdio.h"
#include "pad.h"
#include "strgdef.h"
#include <string.h>


#include "RamDisk.h"
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//public func
UINT32 DrvNANDGetcaps0(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvNANDGetcaps1(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvNANDGetcaps3(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvNANDInit(void* pInitParam); // Set Init Parameters
UINT32 DrvNANDOpen(void); // Common Constructor
UINT32 DrvNANDClose(void); // Common Destructor
UINT32 DrvNANDState(UINT32 StateID, UINT32 Value); // General Properties
UINT32 DrvNANDControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
UINT32 DrvNANDCommand(CHAR *pcCmdStr); //General Command Console

//dx object
DX_OBJECT gDevNAND0 =
{
    DXFLAG_SIGN,
    DX_CLASS_STORAGE_EXT|DX_TYPE_NAND0,
    STORAGE_VER,
    "Storage_Nand0",
    0,0,0,0,
    DrvNANDGetcaps0,
    0,
    DrvNANDInit,
    DrvNANDOpen,
    DrvNANDClose,
    DrvNANDState,
    DrvNANDControl,
    DrvNANDCommand,
    0,
};

DX_OBJECT gDevNAND1 =
{
    DXFLAG_SIGN,
    DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1,
    STORAGE_VER,
    "Storage_Nand1",
    0,0,0,0,
    DrvNANDGetcaps1,
    0,
    DrvNANDInit,
    DrvNANDOpen,
    DrvNANDClose,
    DrvNANDState,
    DrvNANDControl,
    DrvNANDCommand,
    0,
};

DX_OBJECT gDevNAND3 =
{
    DXFLAG_SIGN,
    DX_CLASS_STORAGE_EXT|DX_TYPE_NAND3,
    STORAGE_VER,
    "Storage_Nand3",
    0,0,0,0,
    DrvNANDGetcaps3,
    0,
    DrvNANDInit,
    DrvNANDOpen,
    DrvNANDClose,
    DrvNANDState,
    DrvNANDControl,
    DrvNANDCommand,
    0,
};


UINT32 DrvNANDGetcaps0(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    case STORAGE_CAPS_HANDLE:
        v = (UINT32)spiflash_getStorageObject(STRG_OBJ_FW_RSV1);
        break;
    default:
        break;
    }
    return v;
}

UINT32 DrvNANDGetcaps1(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    case STORAGE_CAPS_HANDLE:
        //#NT#2014/07/16#Calvin Chang#Support DRAM Tiny Mode -begin
        #if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
        v = (UINT32)ramdsk_getStorageObject();
        #else
        v = (UINT32)nand_getStorageObject();
        #endif
        //v = (UINT32)ramdsk_getStorageObject();
        //#NT#2014/07/16#Calvin Chang -end
        break;
    default:
        break;
    }
    return v;
}

UINT32 DrvNANDGetcaps3(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    case STORAGE_CAPS_HANDLE:
        v = (UINT32)spiflash_getStorageObject(STRG_OBJ_PSTORE1);
        break;
    default:
        break;
    }
    return v;
}

UINT32 DrvNANDInit(void* pInitParam) // Set Init Parameters
{
    DXSTRG_INIT *g_pInit = (DXSTRG_INIT*)pInitParam; //fat and pstore not complete

    if(!g_pInit)
        return DX_PARAM_ERROR;

    DBG_FUNC_BEGIN("\r\n");

    {
        DXSTRG_PARTITION* p = &(g_pInit->prt);
        DX_HANDLE DxNandDev = Dx_GetObject(p->uiDxClassType);
        STORAGE_OBJ* pStrg = (STORAGE_OBJ *)Dx_Getcaps(DxNandDev, STORAGE_CAPS_HANDLE, 0);
        UINT32 blksize = 0;

        pStrg->Lock();


        if(p->uiDxClassType ==(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND0))
        {
            pStrg->SetParam(STRG_SET_MEMORY_REGION, g_pInit->buf.Addr, g_pInit->buf.Size);
            pStrg->SetParam(STRG_SET_PARTITION_SIZE,p->uiPhyAddr,p->uiPhySize);
            pStrg->GetParam(STRG_GET_SECTOR_SIZE, (UINT32)&blksize, 0); //get block size
            pStrg->SetParam(STRG_SET_LOADER_SECTOR_COUNT,p->uiResvSize/blksize, 0);
            if(p->uiPhyAddr!=0)
            {
                DBG_ERR("Fw area have to be at phyical address 0.\r\n");
                return E_PAR;
            }
        }
        else
        {
            pStrg->SetParam(STRG_SET_MEMORY_REGION, g_pInit->buf.Addr, g_pInit->buf.Size);
            pStrg->SetParam(STRG_SET_PARTITION_SIZE,p->uiPhyAddr,p->uiPhySize);
        }

        pStrg->Unlock();

    }
    return DX_OK;
}

UINT32 DrvNANDOpen(void) // Common Constructor
{
    DBG_FUNC_BEGIN("\r\n");
    return DX_OK;
}

UINT32 DrvNANDClose(void) // Common Destructor
{
    DBG_FUNC_BEGIN("\r\n");
    return DX_OK;
}

UINT32 DrvNANDState(UINT32 StateID, UINT32 Value) // General Properties
{
    DBG_FUNC_BEGIN("\r\n");

    if(StateID & DXGET)
    {
    UINT32 rvalue =  0;
    StateID &= ~DXGET;
    DBG_IND("get %08x\r\n", StateID);
    switch(StateID)
    {
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

UINT32 DrvNANDControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
{
    DBG_FUNC_BEGIN("\r\n");
    DBG_IND("ctrl %08x\r\n", CtrlID);

    switch(CtrlID)
    {
    default:
        DBG_ERR("ctrlid=0x%02X not support!\r\n", CtrlID);
        break;
    }
    return DX_OK;
}

UINT32 DrvNANDCommand(CHAR *pcCmdStr) //General Command Console
{
    switch (*pcCmdStr)
    {
    case 'd':
        /*if (!strncmp(pcCmdStr, "Nand dump", 9))
        {
            return TRUE;
        }*/
        break;
    }
#if 0
#if _MIPS_TODO
    if((nand_info.ucDeviceCode == NAND_128M_2K)
    || (nand_info.ucDeviceCode == NAND_256M_2K))
    {
        DBG_DUMP("NAND type = 2K page\r\n");
    }
    else
    {
        DBG_DUMP("NAND type = 512B page\r\n");
    }
#endif
    //#NT#2010/02/22#Jeah Yen -end
    DBG_DUMP("Page Count/Block = %d\r\n", nand_info.uiPagePerBlock);
    DBG_DUMP("Data Size/Page = %d bytes\r\n", nand_info.uiBytesPerPageData);
    DBG_DUMP("Spare Size/Page = %d bytes\r\n", nand_info.uiSparePerPage);
    DBG_DUMP("Data Size/Block = %d bytes\r\n", nand_info.uiBlockSize);
    DBG_DUMP("Spare Size/Block = %d bytes\r\n", nand_info.uiPagePerBlock*nand_info.uiSparePerPage);

    DBG_DUMP("Physical Block Count = %d\r\n", nand_info.uiPhysicalBlockCount);
    DBG_DUMP("Total NAND data size = 0x%08x bytes\r\n",
        nand_info.uiPhysicalBlockCount*nand_info.uiBlockSize);
    DBG_DUMP("Total NAND spare size = 0x%08x bytes\r\n",
        nand_info.uiPhysicalBlockCount*nand_info.uiPagePerBlock*nand_info.uiSparePerPage);

/*
Ex:
ERR: Page Count/Block = 32
ERR: Data Size/Page = 512 bytes
ERR: Spare Size/Page = 16 bytes
ERR: Data Size/Block = 16384 bytes
ERR: Spare Size/Block = 512 bytes

ERR: Physical Block Count = 1728  //<------------ 注意! 不包含PST區的block
ERR: Total NAND data size = 0x01b00000 bytes  //<------------ 注意! 不包含PST區的block
ERR: Total NAND spare size = 0x000d8000 bytes  //<------------ 注意! 不包含PST區的block
*/
#endif
    return FALSE;
}


