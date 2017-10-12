#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
#if _MIPS_TODO
#include "DxCamera.h"
#include "GxFlash.h"
#endif

#include "DrvExt.h"

#include "ADC.h" //for adc_readData()
#include "Top.h" //for pinmux_select_ick(), pinmux_select_goice(), PINMUX_ICK, PINMUX_GOICE
#if _MIPS_TODO
#include "DxDisplay.h"
#endif

#include "Driver.h" //for drv_getVerInfo(), drv_getBuildDate()
#include "IoCfg.h" //for USE_VIO

#include "DxApi.h"
#include "DxFlash.h"
#include "DxOutput.h"
#include "hdmitx.h"




///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#include "SxCmd.h"



static BOOL Cmd_dx_lcd(CHAR* strCmd)
{
    DX_HANDLE devLCDObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_LCD);
    return Dx_Command(devLCDObj, strCmd);
}
static BOOL Cmd_dx_tv(CHAR* strCmd)
{
    DX_HANDLE devTVObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_TVOUT);
    return Dx_Command(devTVObj, strCmd);
}
static BOOL Cmd_dx_hdmi(CHAR* strCmd)
{
    DX_HANDLE devHDMIObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_HDMIOUT);
    return Dx_Command(devHDMIObj, strCmd);
}

SXCMD_BEGIN(dx,"driver extern debug using")
SXCMD_ITEM("lcd %", Cmd_dx_lcd, "lcd relay command")
SXCMD_ITEM("tv %", Cmd_dx_tv, "tv relay command")
SXCMD_ITEM("hdmi %", Cmd_dx_hdmi, "hdmi relay command")
SXCMD_END()



BOOL Dx_OnCommand(CHAR *pcCmdStr)
{
    switch (*pcCmdStr)
    {
    case 'v':
        if (!strncmp(pcCmdStr, "drv ver", 7))
        {
            /* version messages */
            //DBG_DUMP("- Driver       ver: %s, build: %s\r\n", drv_getVerInfo(), drv_getBuildDate());
            //DBG_DUMP("- Driver IPP   ver: %s, build: %s\r\n", drv_getVerInfo_IPP(), drv_getBuildDate_IPP()); //only avaliable from 460
            //DBG_DUMP("- Driver Codec ver: %s, build: %s\r\n", drv_getVerInfo_Codec(), drv_getBuildDate_Codec()); //only avaliable from 460
            return TRUE;
        }
        break;
#if _MIPS_TODO
///////////////////////////////////////////////////////////////////////////////
// battery
///////////////////////////////////////////////////////////////////////////////
    case 'b':
        if (!strncmp(pcCmdStr,"b1", 2))
        {
            UINT32 batt, ms1, ms2, key1;
            batt = adc_readData(0);
            ms1 = adc_readData(1);
            ms2 = adc_readData(2);
            key1 = adc_readData(3);

            Dx_OutputMsg(("batt = %d, ms1 = %d, ms2 = %d, key1 = %d", batt, ms1, ms2, key1));
            return TRUE;
        }
        break;
///////////////////////////////////////////////////////////////////////////////
// flash
///////////////////////////////////////////////////////////////////////////////
    case 'f':
        if (!strncmp(pcCmdStr,"flash ch 1",10))//Single capture
        {
            DrvFlash_ChargeFlash(TRUE, FALSE);
            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"flash ch 0",10))//Single capture
        {
            DrvFlash_ChargeFlash(FALSE, FALSE);
            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"flash check",11))//Single capture
        {
            UINT32 ready = 0;
            ready = DrvFlash_IsCharge();
            DBG_DUMP("Flash is charge %d \r\n", ready);
            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"flash ready",11))//Single capture
        {
            UINT32 ready = 0;
            ready = DrvFlash_IsFlashRdy();
            DBG_DUMP("Flash is ready %d \r\n", ready);
            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"flash tri",9))//Single capture
        {
            GxFlash_TriggerFlash(30000);
            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"flash test",10))//Single capture
        {
            BOOL bQuit = FALSE;
            INT32 iCount = 0;
            while (!bQuit)
            {
                DBG_DUMP("start charge ");
                DrvFlash_ChargeFlash(TRUE, FALSE);

                do {
                ;
                } while (!DrvFlash_IsFlashRdy());
                DBG_DUMP("charge done ");
                GxFlash_TriggerFlash(30000);
                DBG_DUMP("tri done \r\n");
                Delay_DelayMs(5000);
                iCount++;
                if(iCount == 1000)
                    bQuit = TRUE;
            }
            return TRUE;
        }
        break;
#endif
    }
    return FALSE;
}


