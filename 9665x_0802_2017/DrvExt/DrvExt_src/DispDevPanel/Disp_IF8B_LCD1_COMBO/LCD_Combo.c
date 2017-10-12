/*
    Display object object panel configuration parameters for ILI8961

    @file       ILI8961.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "Kernel.h"
#include "Debug.h"
#include "display.h"
#include "dispdev_if8bits.h"
#include "dispdev_panlcomn.h"
#include "LCD_Combo.h"

static PLCD_OBJ GLCD_getLCDObj(void);

static PLCD_OBJ GLCD_getLCDObj(void)
{
    // Check LCD model here!!!
    if (1) {
	    // ILI9342C
        return LCD_ILI9342C_getLCDObj();
    }
}

tLCD_ROT* dispdev_getLcdRotateCmd(UINT32 *ModeNumber)
{
    PLCD_OBJ LCDObj = GLCD_getLCDObj();
    return LCDObj->dispdev_getLcdRotateCmd(ModeNumber);
}

tLCD_PARAM* dispdev_getConfigMode(UINT32 *ModeNumber)
{
    PLCD_OBJ LCDObj = GLCD_getLCDObj();
    return LCDObj->dispdev_getConfigMode(ModeNumber);
}

tPANEL_CMD* dispdev_getStandbyCmd(UINT32 *CmdNumber)
{
    PLCD_OBJ LCDObj = GLCD_getLCDObj();
    return LCDObj->dispdev_getStandbyCmd(CmdNumber);
}

void dispdev_writeToLcdSif(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    PLCD_OBJ LCDObj = GLCD_getLCDObj();
    LCDObj->dispdev_writeToLcdSif(pDispDevControl, addr, value);
}

void dispdev_writeToLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    PLCD_OBJ LCDObj = GLCD_getLCDObj();
    LCDObj->dispdev_writeToLcdGpio(pDispDevControl, addr, value);
}

void dispdev_readFromLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32* p_value)
{
    PLCD_OBJ LCDObj = GLCD_getLCDObj();
    LCDObj->dispdev_readFromLcdGpio(pDispDevControl, addr, p_value);
}
