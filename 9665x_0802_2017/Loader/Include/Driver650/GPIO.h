/**
    General Purpose I/O controller header file

    General Purpose I/O controller header file

    @file       gpio.h
    @ingroup    mIIOGPIO
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _GPIO_H
#define _GPIO_H

#include "IOReg.h"

/**
    @addtogroup mIIOGPIO
*/
//@{

/**
    GPIO direction

    GPIO direction definition for gpio_setDir() and gpio_getDir()

    @name   GPIO direction
*/
//@{
#define GPIO_DIR_INPUT              0       ///< GPIO is input direction
#define GPIO_DIR_OUTPUT             1       ///< GPIO is output direction
//@}

/**
    GPIO interrupt type

    GPIO interrupt type definition for type argument of gpio_setIntTypePol()

    @name   GPIO interrupt type
*/
//@{
#define GPIO_INTTYPE_EDGE           0       ///< GPIO interrupt is edge trigger
#define GPIO_INTTYPE_LEVEL          1       ///< GPIO interrupt is level trigger
//@}

/**
    GPIO interrupt polarity

    GPIO interrupt polarity definition for pol argument of gpio_setIntTypePol()

    @name   GPIO interrupt polarity
*/
//@{
#define GPIO_INTPOL_POSHIGH         0       ///< GPIO interrupt polarity is \n
                                            ///< - @b positvie edge for edge trigger
                                            ///< - @b high level for level trigger
#define GPIO_INTPOL_NEGLOW          1       ///< GPIO interrupt polarity is \n
                                            ///< - @b negative edge for edge trigger
                                            ///< - @b low level for level trigger
//@}



/**
    @name   GPIO pins ID

    GPIO pins ID definition

    For detail GPIO pin out, please refer to NT96650 data sheet.
*/
//@{
/*Storage GPIO - CGPIO*/
#define C_GPIO_0    0                       ///< CGPIO[0]
#define C_GPIO_1    1                       ///< CGPIO[1]
#define C_GPIO_2    2                       ///< CGPIO[2]
#define C_GPIO_3    3                       ///< CGPIO[3]
#define C_GPIO_4    4                       ///< CGPIO[4]
#define C_GPIO_5    5                       ///< CGPIO[5]
#define C_GPIO_6    6                       ///< CGPIO[6]
#define C_GPIO_7    7                       ///< CGPIO[7]
#define C_GPIO_8    8                       ///< CGPIO[8]
#define C_GPIO_9    9                       ///< CGPIO[9]
#define C_GPIO_10   10                      ///< CGPIO[10]
#define C_GPIO_11   11                      ///< CGPIO[11]
#define C_GPIO_12   12                      ///< CGPIO[12]
#define C_GPIO_13   13                      ///< CGPIO[13]
#define C_GPIO_14   14                      ///< CGPIO[14]
#define C_GPIO_15   15                      ///< CGPIO[15]
#define C_GPIO_16   16                      ///< CGPIO[16]
#define C_GPIO_17   17                      ///< CGPIO[17]
#define C_GPIO_18   18                      ///< CGPIO[18]
#define C_GPIO_19   19                      ///< CGPIO[19]
#define C_GPIO_20   20                      ///< CGPIO[20]
#define C_GPIO_21   21                      ///< CGPIO[21]
#define C_GPIO_22   22                      ///< CGPIO[22]
#define C_GPIO_23   23                      ///< CGPIO[23]
#define C_GPIO_24   24                      ///< CGPIO[24]
#define C_GPIO_25   25                      ///< CGPIO[25]
#define C_GPIO_26   26                      ///< CGPIO[26]
#define C_GPIO_27   27                      ///< CGPIO[27]
#define C_GPIO_28   28                      ///< CGPIO[28]
#define C_GPIO_29   29                      ///< CGPIO[29]
#define C_GPIO_30   30                      ///< CGPIO[30]
#define C_GPIO_31   31                      ///< CGPIO[31]

/*Peripheral GPIO - PGPIO*/
#define P_GPIO_SHIFT_BASE 64
#define P_GPIO_0    (0 +P_GPIO_SHIFT_BASE)  ///< PGPIO[0]
#define P_GPIO_1    (1 +P_GPIO_SHIFT_BASE)  ///< PGPIO[1]
#define P_GPIO_2    (2 +P_GPIO_SHIFT_BASE)  ///< PGPIO[2]
#define P_GPIO_3    (3 +P_GPIO_SHIFT_BASE)  ///< PGPIO[3]
#define P_GPIO_4    (4 +P_GPIO_SHIFT_BASE)  ///< PGPIO[4]
#define P_GPIO_5    (5 +P_GPIO_SHIFT_BASE)  ///< PGPIO[5]
#define P_GPIO_6    (6 +P_GPIO_SHIFT_BASE)  ///< PGPIO[6]
#define P_GPIO_7    (7 +P_GPIO_SHIFT_BASE)  ///< PGPIO[7]
#define P_GPIO_8    (8 +P_GPIO_SHIFT_BASE)  ///< PGPIO[8]
#define P_GPIO_9    (9 +P_GPIO_SHIFT_BASE)  ///< PGPIO[9]
#define P_GPIO_10   (10+P_GPIO_SHIFT_BASE)  ///< PGPIO[10]
#define P_GPIO_11   (11+P_GPIO_SHIFT_BASE)  ///< PGPIO[11]
#define P_GPIO_12   (12+P_GPIO_SHIFT_BASE)  ///< PGPIO[12]
#define P_GPIO_13   (13+P_GPIO_SHIFT_BASE)  ///< PGPIO[13]
#define P_GPIO_14   (14+P_GPIO_SHIFT_BASE)  ///< PGPIO[14]
#define P_GPIO_15   (15+P_GPIO_SHIFT_BASE)  ///< PGPIO[15]
#define P_GPIO_16   (16+P_GPIO_SHIFT_BASE)  ///< PGPIO[16]
#define P_GPIO_17   (17+P_GPIO_SHIFT_BASE)  ///< PGPIO[17]
#define P_GPIO_18   (18+P_GPIO_SHIFT_BASE)  ///< PGPIO[18]
#define P_GPIO_19   (19+P_GPIO_SHIFT_BASE)  ///< PGPIO[19]
#define P_GPIO_20   (20+P_GPIO_SHIFT_BASE)  ///< PGPIO[20]
#define P_GPIO_21   (21+P_GPIO_SHIFT_BASE)  ///< PGPIO[21]
#define P_GPIO_22   (22+P_GPIO_SHIFT_BASE)  ///< PGPIO[22]
#define P_GPIO_23   (23+P_GPIO_SHIFT_BASE)  ///< PGPIO[23]
#define P_GPIO_24   (24+P_GPIO_SHIFT_BASE)  ///< PGPIO[24]
#define P_GPIO_25   (25+P_GPIO_SHIFT_BASE)  ///< PGPIO[25]
#define P_GPIO_26   (26+P_GPIO_SHIFT_BASE)  ///< PGPIO[26]
#define P_GPIO_27   (27+P_GPIO_SHIFT_BASE)  ///< PGPIO[27]
#define P_GPIO_28   (28+P_GPIO_SHIFT_BASE)  ///< PGPIO[28]
#define P_GPIO_29   (29+P_GPIO_SHIFT_BASE)  ///< PGPIO[29]
#define P_GPIO_30   (30+P_GPIO_SHIFT_BASE)  ///< PGPIO[30]
#define P_GPIO_31   (31+P_GPIO_SHIFT_BASE)  ///< PGPIO[31]
#define P_GPIO_32   (32+P_GPIO_SHIFT_BASE)  ///< PGPIO[32]
#define P_GPIO_33   (33+P_GPIO_SHIFT_BASE)  ///< PGPIO[33]
#define P_GPIO_34   (34+P_GPIO_SHIFT_BASE)  ///< PGPIO[34]
#define P_GPIO_35   (35+P_GPIO_SHIFT_BASE)  ///< PGPIO[35]
#define P_GPIO_36   (36+P_GPIO_SHIFT_BASE)  ///< PGPIO[36]
#define P_GPIO_37   (37+P_GPIO_SHIFT_BASE)  ///< PGPIO[37]
#define P_GPIO_38   (38+P_GPIO_SHIFT_BASE)  ///< PGPIO[38]
#define P_GPIO_39   (39+P_GPIO_SHIFT_BASE)  ///< PGPIO[39]
#define P_GPIO_40   (40+P_GPIO_SHIFT_BASE)  ///< PGPIO[40]
#define P_GPIO_41   (41+P_GPIO_SHIFT_BASE)  ///< PGPIO[41]
#define P_GPIO_42   (42+P_GPIO_SHIFT_BASE)  ///< PGPIO[42]
#define P_GPIO_43   (43+P_GPIO_SHIFT_BASE)  ///< PGPIO[43]
#define P_GPIO_44   (44+P_GPIO_SHIFT_BASE)  ///< PGPIO[44]
#define P_GPIO_45   (45+P_GPIO_SHIFT_BASE)  ///< PGPIO[45]
#define P_GPIO_46   (46+P_GPIO_SHIFT_BASE)  ///< PGPIO[46]
#define P_GPIO_47   (47+P_GPIO_SHIFT_BASE)  ///< PGPIO[47]
#define P_GPIO_48   (48+P_GPIO_SHIFT_BASE)  ///< PGPIO[48]
#define P_GPIO_49   (49+P_GPIO_SHIFT_BASE)  ///< PGPIO[49]
#define P_GPIO_50   (50+P_GPIO_SHIFT_BASE)  ///< PGPIO[50]
#define P_GPIO_51   (51+P_GPIO_SHIFT_BASE)  ///< PGPIO[51]
#define P_GPIO_52   (52+P_GPIO_SHIFT_BASE)  ///< PGPIO[52]
#define P_GPIO_53   (53+P_GPIO_SHIFT_BASE)  ///< PGPIO[53]
#define P_GPIO_54   (54+P_GPIO_SHIFT_BASE)  ///< PGPIO[54]
#define P_GPIO_55   (55+P_GPIO_SHIFT_BASE)  ///< PGPIO[55]
#define P_GPIO_56   (56+P_GPIO_SHIFT_BASE)  ///< PGPIO[56]
#define P_GPIO_57   (57+P_GPIO_SHIFT_BASE)  ///< PGPIO[57]
#define P_GPIO_58   (58+P_GPIO_SHIFT_BASE)  ///< PGPIO[58]
#define P_GPIO_59   (59+P_GPIO_SHIFT_BASE)  ///< PGPIO[59]
#define P_GPIO_60   (60+P_GPIO_SHIFT_BASE)  ///< PGPIO[60]
#define P_GPIO_61   (61+P_GPIO_SHIFT_BASE)  ///< PGPIO[61]

/*Sensor GPIO - SGPIO*/
#define S_GPIO_SHIFT_BASE 128
#define S_GPIO_0    (0 +S_GPIO_SHIFT_BASE)  ///< SGPIO[0]
#define S_GPIO_1    (1 +S_GPIO_SHIFT_BASE)  ///< SGPIO[1]
#define S_GPIO_2    (2 +S_GPIO_SHIFT_BASE)  ///< SGPIO[2]
#define S_GPIO_3    (3 +S_GPIO_SHIFT_BASE)  ///< SGPIO[3]
#define S_GPIO_4    (4 +S_GPIO_SHIFT_BASE)  ///< SGPIO[4]
#define S_GPIO_5    (5 +S_GPIO_SHIFT_BASE)  ///< SGPIO[5]
#define S_GPIO_6    (6 +S_GPIO_SHIFT_BASE)  ///< SGPIO[6]
#define S_GPIO_7    (7 +S_GPIO_SHIFT_BASE)  ///< SGPIO[7]
#define S_GPIO_8    (8 +S_GPIO_SHIFT_BASE)  ///< SGPIO[8]
#define S_GPIO_9    (9 +S_GPIO_SHIFT_BASE)  ///< SGPIO[9]
#define S_GPIO_10   (10+S_GPIO_SHIFT_BASE)  ///< SGPIO[10]
#define S_GPIO_11   (11+S_GPIO_SHIFT_BASE)  ///< SGPIO[11]
#define S_GPIO_12   (12+S_GPIO_SHIFT_BASE)  ///< SGPIO[12]
#define S_GPIO_13   (13+S_GPIO_SHIFT_BASE)  ///< SGPIO[13]
#define S_GPIO_14   (14+S_GPIO_SHIFT_BASE)  ///< SGPIO[14]
#define S_GPIO_15   (15+S_GPIO_SHIFT_BASE)  ///< SGPIO[15]
#define S_GPIO_16   (16+S_GPIO_SHIFT_BASE)  ///< SGPIO[16]
#define S_GPIO_17   (17+S_GPIO_SHIFT_BASE)  ///< SGPIO[17]
#define S_GPIO_18   (18+S_GPIO_SHIFT_BASE)  ///< SGPIO[18]
#define S_GPIO_19   (19+S_GPIO_SHIFT_BASE)  ///< SGPIO[19]
#define S_GPIO_20   (20+S_GPIO_SHIFT_BASE)  ///< SGPIO[20]
#define S_GPIO_21   (21+S_GPIO_SHIFT_BASE)  ///< SGPIO[21]
#define S_GPIO_22   (22+S_GPIO_SHIFT_BASE)  ///< SGPIO[22]
#define S_GPIO_23   (23+S_GPIO_SHIFT_BASE)  ///< SGPIO[23]
#define S_GPIO_24   (24+S_GPIO_SHIFT_BASE)  ///< SGPIO[24]
#define S_GPIO_25   (25+S_GPIO_SHIFT_BASE)  ///< SGPIO[25]
#define S_GPIO_26   (26+S_GPIO_SHIFT_BASE)  ///< SGPIO[26]
#define S_GPIO_27   (27+S_GPIO_SHIFT_BASE)  ///< SGPIO[27]
#define S_GPIO_28   (28+S_GPIO_SHIFT_BASE)  ///< SGPIO[28]
#define S_GPIO_29   (29+S_GPIO_SHIFT_BASE)  ///< SGPIO[29]

/*LCD GPIO - LGPIO*/
#define L_GPIO_SHIFT_BASE 160
#define L_GPIO_0    (0 +L_GPIO_SHIFT_BASE)  ///< LGPIO[0]
#define L_GPIO_1    (1 +L_GPIO_SHIFT_BASE)  ///< LGPIO[1]
#define L_GPIO_2    (2 +L_GPIO_SHIFT_BASE)  ///< LGPIO[2]
#define L_GPIO_3    (3 +L_GPIO_SHIFT_BASE)  ///< LGPIO[3]
#define L_GPIO_4    (4 +L_GPIO_SHIFT_BASE)  ///< LGPIO[4]
#define L_GPIO_5    (5 +L_GPIO_SHIFT_BASE)  ///< LGPIO[5]
#define L_GPIO_6    (6 +L_GPIO_SHIFT_BASE)  ///< LGPIO[6]
#define L_GPIO_7    (7 +L_GPIO_SHIFT_BASE)  ///< LGPIO[7]
#define L_GPIO_8    (8 +L_GPIO_SHIFT_BASE)  ///< LGPIO[8]
#define L_GPIO_9    (9 +L_GPIO_SHIFT_BASE)  ///< LGPIO[9]
#define L_GPIO_10   (10+L_GPIO_SHIFT_BASE)  ///< LGPIO[10]
#define L_GPIO_11   (11+L_GPIO_SHIFT_BASE)  ///< LGPIO[11]
#define L_GPIO_12   (12+L_GPIO_SHIFT_BASE)  ///< LGPIO[12]
#define L_GPIO_13   (13+L_GPIO_SHIFT_BASE)  ///< LGPIO[13]
#define L_GPIO_14   (14+L_GPIO_SHIFT_BASE)  ///< LGPIO[14]
#define L_GPIO_15   (15+L_GPIO_SHIFT_BASE)  ///< LGPIO[15]
#define L_GPIO_16   (16+L_GPIO_SHIFT_BASE)  ///< LGPIO[16]
#define L_GPIO_17   (17+L_GPIO_SHIFT_BASE)  ///< LGPIO[17]
#define L_GPIO_18   (18+L_GPIO_SHIFT_BASE)  ///< LGPIO[18]
#define L_GPIO_19   (19+L_GPIO_SHIFT_BASE)  ///< LGPIO[19]
#define L_GPIO_20   (20+L_GPIO_SHIFT_BASE)  ///< LGPIO[20]
#define L_GPIO_21   (21+L_GPIO_SHIFT_BASE)  ///< LGPIO[21]
#define L_GPIO_22   (22+L_GPIO_SHIFT_BASE)  ///< LGPIO[22]
#define L_GPIO_23   (23+L_GPIO_SHIFT_BASE)  ///< LGPIO[23]
#define L_GPIO_24   (24+L_GPIO_SHIFT_BASE)  ///< LGPIO[24]
#define L_GPIO_25   (25+L_GPIO_SHIFT_BASE)  ///< LGPIO[25]
#define L_GPIO_26   (26+L_GPIO_SHIFT_BASE)  ///< LGPIO[26]
#define L_GPIO_27   (27+L_GPIO_SHIFT_BASE)  ///< LGPIO[27]

/*General GPIO - GGPIO*/
#define G_GPIO_SHIFT_BASE 192

/*Dedicated GPIO - DGPIO*/
// In order to backward comaptible, DGPIO is used as " GPIO_IS_DGPIO | D_GPIO_* "
#define D_GPIO_SHIFT_BASE 224
#define D_GPIO_0    (0 +D_GPIO_SHIFT_BASE)  ///< DGPIO[0]
#define D_GPIO_1    (1 +D_GPIO_SHIFT_BASE)  ///< DGPIO[1]
#define D_GPIO_2    (2 +D_GPIO_SHIFT_BASE)  ///< DGPIO[2]
#define D_GPIO_3    (3 +D_GPIO_SHIFT_BASE)  ///< DGPIO[3]
#define D_GPIO_4    (4 +D_GPIO_SHIFT_BASE)  ///< DGPIO[4]
#define D_GPIO_5    (5 +D_GPIO_SHIFT_BASE)  ///< DGPIO[5]
#define D_GPIO_6    (6 +D_GPIO_SHIFT_BASE)  ///< DGPIO[6]
#define D_GPIO_7    (7 +D_GPIO_SHIFT_BASE)  ///< DGPIO[7]
#define D_GPIO_8    (8 +D_GPIO_SHIFT_BASE)  ///< DGPIO[8]
#define D_GPIO_16   (16 +D_GPIO_SHIFT_BASE) ///< DGPIO[16] (USB Wakeup)
//@}

//@}




/**
    @name   GPIO Interrupt ID

    GPIO interrupt ID definition

    GPIO interrupt ID for interrupt related APIs.
*/
//@{
#define GPIO_INT_00                 0       ///< GPIO INT[0]: CGPIO[15]
#define GPIO_INT_01                 1       ///< GPIO INT[1]: CGPIO[16]
#define GPIO_INT_02                 2       ///< GPIO INT[2]: CGPIO[26]
#define GPIO_INT_03                 3       ///< GPIO INT[3]: CGPIO[27]
#define GPIO_INT_04                 4       ///< GPIO INT[4]: PGPIO[0]
#define GPIO_INT_05                 5       ///< GPIO INT[5]: PGPIO[1]
#define GPIO_INT_06                 6       ///< GPIO INT[6]: PGPIO[7]
#define GPIO_INT_07                 7       ///< GPIO INT[7]: PGPIO[8]
#define GPIO_INT_08                 8       ///< GPIO INT[8]: PGPIO[9]
#define GPIO_INT_09                 9       ///< GPIO INT[9]: PGPIO[11]
#define GPIO_INT_10                 10      ///< GPIO INT[10]: PGPIO[16]
#define GPIO_INT_11                 11      ///< GPIO INT[11]: PGPIO[17]
#define GPIO_INT_12                 12      ///< GPIO INT[12]: PGPIO[18]
#define GPIO_INT_13                 13      ///< GPIO INT[13]: PGPIO[19]
#define GPIO_INT_14                 14      ///< GPIO INT[14]: PGPIO[20]
#define GPIO_INT_15                 15      ///< GPIO INT[15]: PGPIO[25]
#define GPIO_INT_16                 16      ///< GPIO INT[16]: PGPIO[27]
#define GPIO_INT_17                 17      ///< GPIO INT[17]: CGPIO[24]
#define GPIO_INT_18                 18      ///< GPIO INT[18]: CGPIO[25]
#define GPIO_INT_19                 19      ///< GPIO INT[19]: CGPIO[22]
#define GPIO_INT_20                 20      ///< GPIO INT[20]: CGPIO[23]
#define GPIO_INT_21                 21      ///< GPIO INT[21]: PGPIO[30]
#define GPIO_INT_22                 22      ///< GPIO INT[22]: PGPIO[31]
#define GPIO_INT_23                 23      ///< GPIO INT[23]: PGPIO[32]
#define GPIO_INT_24                 24      ///< GPIO INT[24]: PGPIO[33]
#define GPIO_INT_25                 25      ///< GPIO INT[25]: PGPIO[34]
#define GPIO_INT_26                 26      ///< GPIO INT[26]: PGPIO[35]
#define GPIO_INT_27                 27      ///< GPIO INT[27]: PGPIO[54]
#define GPIO_INT_28                 28      ///< GPIO INT[28]: PGPIO[66]
#define GPIO_INT_29                 29      ///< GPIO INT[29]: SGPIO[29]
#define GPIO_INT_30                 30      ///< GPIO INT[30]: LGPIO[18]
#define GPIO_INT_31                 31      ///< GPIO INT[31]: LGPIO[19]

#define GPIO_INT_32                 32      ///< DGPIO INT[0]: DGPIO[0]
#define GPIO_INT_33                 33      ///< DGPIO INT[1]: DGPIO[1]
#define GPIO_INT_34                 34      ///< DGPIO INT[2]: DGPIO[2]
#define GPIO_INT_35                 35      ///< DGPIO INT[3]: DGPIO[3]
#define GPIO_INT_36                 36      ///< DGPIO INT[4]: DGPIO[4]
#define GPIO_INT_37                 37      ///< DGPIO INT[5]: DGPIO[5]
#define GPIO_INT_38                 38      ///< DGPIO INT[6]: DGPIO[6]
#define GPIO_INT_39                 39      ///< DGPIO INT[7]: DGPIO[7]
#define GPIO_INT_40                 40      ///< DGPIO INT[8]: DGPIO[8]

#define GPIO_INT_48                 48      ///< DGPIO INT[16]: USB wakeup

#define GPIO_INT_USBWAKEUP          (GPIO_INT_48)   ///< GPIO INT[19]: USB wakeup
#define GPIO_INT_USBPLUGIN          (GPIO_INT_40)   ///< GPIO INT[20]: USB VBUS
//@}

// In order to backward comaptible, DGPIO is used as " GPIO_IS_DGPIO | D_GPIO_* " or "D_GPIO_*"
#define GPIO_IS_DGPIO               (224)



//
// The general api for the GPIO device driver
//
extern void     gpio_setDir(UINT32 pin, UINT32 dir);
extern UINT32   gpio_getDir(UINT32 pin);
extern void     gpio_setPin(UINT32 pin);
extern void     gpio_clearPin(UINT32 pin);
extern UINT32   gpio_getPin(UINT32 pin);
extern void     gpio_pullSet(UINT32 pin);
extern void     gpio_pullClear(UINT32 pin);
extern UINT32   gpio_readData(UINT32 dataidx);
extern void     gpio_writeData(UINT32 dataidx, UINT32 value);

extern UINT32   dgpio_readData(void);
extern void     dgpio_writeData(UINT32 value);
//@}
#endif

