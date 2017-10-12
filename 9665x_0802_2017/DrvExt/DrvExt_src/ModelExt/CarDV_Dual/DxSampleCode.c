
#include "Dx.h"
#include "DxCommon.h"
#include "ErrorNo.h"
#include "Utility.h"
#include "debug.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//public func
UINT32 DrvXXXXGetcaps(UINT32 CapID); // Get Capability Flag (Base on interface version)
UINT32 DrvXXXXInit(void* pInitParam); // Set Init Parameters
UINT32 DrvXXXXOpen(void); // Common Constructor
UINT32 DrvXXXXClose(void); // Common Destructor 
UINT32 DrvXXXXState(UINT32 StateID, UINT32 Value); // General Properties
UINT32 DrvXXXXControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
BOOL DrvXXXXCommand(CHAR *pcCmdStr); //General Command Console

//dx object
DX_OBJECT gDevXXXX =
{
    DXFLAG_SIGN,
    DX_CLASS_NULL,
    0, //VERSION NUMBER
    "Display_XXXX",
    0,0,0,0,
    DrvXXXXGetcaps,
    DrvXXXXInit,
    DrvXXXXOpen,
    DrvXXXXClose,
    DrvXXXXState,
    DrvXXXXControl,
    DrvXXXXCommand,
    0,
};

UINT32 hDevXXXX = (UINT32)&gDevXXXX;

UINT32 DrvXXXXGetcaps(UINT32 CapID) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    default:
        break;
    }
    return v;
}

UINT32 DrvXXXXInit(void* pInitParam) // Set Init Parameters
{
    DBG_FUNC_BEGIN("\r\n");
    return DX_OK;
}

UINT32 DrvXXXXOpen(void) // Common Constructor
{
    DBG_FUNC_BEGIN("\r\n");
    return DX_OK;
}

UINT32 DrvXXXXClose(void) // Common Destructor 
{
    DBG_FUNC_BEGIN("\r\n");
    return DX_OK;
}

UINT32 DrvXXXXState(UINT32 StateID, UINT32 Value) // General Properties
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

UINT32 DrvXXXXControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
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

BOOL DrvXXXXCommand(CHAR *pcCmdStr) //General Command Console
{
    switch (*pcCmdStr)
    {
    case 'd':
        /*if (!strncmp(pcCmdStr, "XXXX dump", 9))
        {
            return TRUE;
        }*/
        break;
    }        
    return FALSE;
}


