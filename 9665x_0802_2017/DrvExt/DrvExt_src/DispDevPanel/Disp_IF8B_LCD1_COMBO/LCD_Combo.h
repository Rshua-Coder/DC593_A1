/*
    Display object object panel configuration parameters for ILI8961

    @file       ILI8961.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _LCD_COMBO_H
#define _LCD_COMBO_H

typedef enum
{
    LCD_NONE    = 0,
    LCD_ILI9342C,
}LCD_MAKER;

typedef struct
{
     tLCD_ROT*    (*dispdev_getLcdRotateCmd)(UINT32 *ModeNumber);
     tLCD_PARAM*  (*dispdev_getConfigMode)(UINT32 *ModeNumber);
     tPANEL_CMD*  (*dispdev_getStandbyCmd)(UINT32 *CmdNumber);
     void         (*dispdev_writeToLcdSif)(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value);
     void         (*dispdev_writeToLcdGpio)(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value);
     void         (*dispdev_readFromLcdGpio)(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32* p_value);
}LCD_OBJ, *PLCD_OBJ;

extern PLCD_OBJ LCD_ILI9342C_getLCDObj(void);

///////////////////////////////////////////////////////////////////////////////////////////////

#define OPTION_SIF_0000  0x0   ///< SIF bus mode 0000
#define OPTION_SIF_0001  0x1   ///< SIF bus mode 0001
#define OPTION_SIF_0010  0x2   ///< SIF bus mode 0010
#define OPTION_SIF_0011  0x3   ///< SIF bus mode 0011
#define OPTION_SIF_0100  0x4   ///< SIF bus mode 0100
#define OPTION_SIF_0101  0x5   ///< SIF bus mode 0101
#define OPTION_SIF_0110  0x6   ///< SIF bus mode 0110
#define OPTION_SIF_0111  0x7   ///< SIF bus mode 0111
#define OPTION_SIF_1000  0x8   ///< SIF bus mode 1000
#define OPTION_SIF_1001  0x9   ///< SIF bus mode 1001
#define OPTION_SIF_1010  0xA   ///< SIF bus mode 1010
#define OPTION_SIF_1011  0xB   ///< SIF bus mode 1011
#define OPTION_SIF_1100  0xC   ///< SIF bus mode 1100
#define OPTION_SIF_1101  0xD   ///< SIF bus mode 1101
#define OPTION_SIF_1110  0xE   ///< SIF bus mode 1110
#define OPTION_SIF_1111  0xF   ///< SIF bus mode 1111

#define SETTING_SIF_MODE                    OPTION_SIF_0000 //SIF MODE: SIF_MODE_0000 ~ SIF_MODE_1111
#define SETTING_PANEL_REG_BIT_SHIFT_ADDRESS 8               //D[?] is Address LSB
#define SETTING_PANEL_REG_BIT_SHIFT_VALUE   0               //D[?] is Content LSB
#define SETTING_PANEL_REG_BIT_WRITE         14              //D[?] is Bit of Write Command
#define SETTING_PANEL_REG_MODE_WRITE        0               //BIT High(1) or LOW(0) is Write Mode
#define SETTING_PANEL_REG_MODE_READ         1               //BIT High(1) or LOW(0) is Read  Mode

#if (SETTING_SIF_MODE==OPTION_SIF_0000)     //Confirmed
    #define USED_SIF_MODE     SIF_MODE_00
    #define SIF_CLK_INIT()    gpio_setPin(SifClk)
    #define SIF_CLK_BEGIN()   gpio_clearPin(SifClk)
    #define SIF_CLK_END()     gpio_setPin(SifClk)
    #define SIF_SEN_INIT()    gpio_setPin(SifSen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
    #define SIF_SEN_END()     gpio_setPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0001)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_01
    #define SIF_CLK_INIT()    gpio_clearPin(SifClk)
    #define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
    #define SIF_CLK_END()     gpio_clearPin(SifClk)
    #define SIF_SEN_INIT()    gpio_setPin(SifSen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
    #define SIF_SEN_END()     gpio_setPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0010)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_02
    #define SIF_CLK_INIT()    gpio_setPin(SifClk)
    #define SIF_CLK_BEGIN()   gpio_clearPin(SifClk)
    #define SIF_CLK_END()     gpio_setPin(SifClk)
    #define SIF_SEN_INIT()    gpio_clearPin(SifSen)
    #define SIF_SEN_BEGIN()   gpio_setPin(SifSen)
    #define SIF_SEN_END()     gpio_clearPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0011)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_03
    #define SIF_CLK_INIT()    gpio_clearPin(SifClk)
    #define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
    #define SIF_CLK_END()     gpio_clearPin(SifClk)
    #define SIF_SEN_INIT()    gpio_clearPin(SifSen)
    #define SIF_SEN_BEGIN()   gpio_setPin(SifSen)
    #define SIF_SEN_END()     gpio_clearPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0100)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_04
    #define SIF_CLK_INIT()    gpio_clearPin(SifClk)
    #define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
    #define SIF_CLK_END()     gpio_clearPin(SifClk)
    #define SIF_SEN_INIT()    gpio_setPin(SifSen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
    #define SIF_SEN_END()     gpio_setPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0101)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_05
    #define SIF_CLK_INIT()    gpio_setPin(SifClk)
    #define SIF_CLK_BEGIN()   gpio_clearPin(SifClk)
    #define SIF_CLK_END()     gpio_setPin(SifClk)
    #define SIF_SEN_INIT()    gpio_setPin(SifSen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
    #define SIF_SEN_END()     gpio_setPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0110)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_06
    #define SIF_CLK_INIT()    gpio_clearPin(SifClk)
    #define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
    #define SIF_CLK_END()     gpio_clearPin(SifClk)
    #define SIF_SEN_INIT()    gpio_clearPin(SifSen)
    #define SIF_SEN_BEGIN()   gpio_setPin(SifSen)
    #define SIF_SEN_END()     gpio_clearPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0111)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_07
    #define SIF_CLK_INIT()    gpio_setPin(SifClk)
    #define SIF_CLK_BEGIN()   gpio_clearPin(SifClk)
    #define SIF_CLK_END()     gpio_setPin(SifClk)
    #define SIF_SEN_INIT()    gpio_clearPin(SifSen)
    #define SIF_SEN_BEGIN()   gpio_setPin(SifSen)
    #define SIF_SEN_END()     gpio_clearPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_1000)   //Confirmed
    #define USED_SIF_MODE     SIF_MODE_08
    #define SIF_CLK_INIT()    gpio_clearPin(SifClk)
    #define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
    #define SIF_CLK_END()     gpio_clearPin(SifClk)
    #define SIF_SEN_INIT()    gpio_setPin(SifSen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
    #define SIF_SEN_END()     gpio_setPin(SifSen)
#else
    #error("This SIF Mode has not implement!");
#endif

#endif
