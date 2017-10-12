/**
    PAD controller header

    PAD controller header

    @file       pad.h
    @ingroup    mIHALSysPAD
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _PAD_H
#define _PAD_H

//#include "Type.h"
#include "constant.h"

/** \addtogroup mIHALSysPAD */
//@{

// Macro to generate dummy element for enum type to expand enum size to word (4 bytes)
#define ENUM_DUMMY4WORD(name)   E_##name = 0x10000000

/**
    Pad type select

    Pad type select

    Pad type value for pad_setPullUpDown().
*/
typedef enum
{
    PAD_NONE         =          0x00,      ///< none of pull up/down
    PAD_PULLDOWN     =          0x01,      ///< pull down
    PAD_PULLUP       =          0x02,      ///< pull up
    PAD_KEEPER       =          0x03,      ///< keeper

    ENUM_DUMMY4WORD(PAD_PULL)
} PAD_PULL;

/**
    Pad driving select

    Pad driving select

    Pad driving value for pad_setDrivingSink().
*/
typedef enum
{
    PAD_DRIVINGSINK_2P5MA  =    0x01,       ///< Pad driver/sink 2.5mA
    PAD_DRIVINGSINK_5MA  =      0x02,       ///< Pad driver/sink 5mA
    PAD_DRIVINGSINK_7P5MA  =    0x04,       ///< Pad driver/sink 7.5mA
    PAD_DRIVINGSINK_10MA  =     0x08,       ///< Pad driver/sink 10mA

    PAD_DRIVINGSINK_12P5MA =    0x10,       ///< Pad driver/sink 12.5mA
    PAD_DRIVINGSINK_15MA =      0x20,       ///< Pad driver/sink 15mA
    PAD_DRIVINGSINK_17P5MA =    0x40,       ///< Pad driver/sink 17.5mA
    PAD_DRIVINGSINK_20MA =      0x80,       ///< Pad driver/sink 20mA
    PAD_DRIVINGSINK_25MA =     0x100,       ///< Pad driver/sink 25mA
    PAD_DRIVINGSINK_30MA =     0x200,       ///< Pad driver/sink 30mA
    PAD_DRIVINGSINK_35MA =     0x400,       ///< Pad driver/sink 35mA
    PAD_DRIVINGSINK_40MA =     0x800,       ///< Pad driver/sink 40mA

    ENUM_DUMMY4WORD(PAD_DRIVINGSINK)
} PAD_DRIVINGSINK;


/**
    @name Pad type pin ID.

    Pad type pin ID.

    Pad ID of pad_setPullUpDown().
*/
#define PAD_PIN_NOT_EXIST       (15)                        // For backward compatible
//@{
//CGPIOx group begin
#define PAD_PIN_CGPIO_BASE      0
#define PAD_PIN_CGPIO0          (PAD_PIN_CGPIO_BASE + 0)      ///< CGPIO0
#define PAD_PIN_CGPIO1          (PAD_PIN_CGPIO_BASE + 1)      ///< CGPIO1
#define PAD_PIN_CGPIO2          (PAD_PIN_CGPIO_BASE + 2)      ///< CGPIO2
#define PAD_PIN_CGPIO3          (PAD_PIN_CGPIO_BASE + 3)      ///< CGPIO3
#define PAD_PIN_CGPIO4          (PAD_PIN_CGPIO_BASE + 4)      ///< CGPIO4
#define PAD_PIN_CGPIO5          (PAD_PIN_CGPIO_BASE + 5)      ///< CGPIO5
#define PAD_PIN_CGPIO6          (PAD_PIN_CGPIO_BASE + 6)      ///< CGPIO6
#define PAD_PIN_CGPIO7          (PAD_PIN_CGPIO_BASE + 7)      ///< CGPIO7
#define PAD_PIN_CGPIO8          (PAD_PIN_CGPIO_BASE + 8)      ///< CGPIO8
#define PAD_PIN_CGPIO9          (PAD_PIN_CGPIO_BASE + 9)      ///< CGPIO9
#define PAD_PIN_CGPIO10         (PAD_PIN_CGPIO_BASE + 10)     ///< CGPIO10
#define PAD_PIN_CGPIO11         (PAD_PIN_CGPIO_BASE + 11)     ///< CGPIO11
#define PAD_PIN_CGPIO12         (PAD_PIN_CGPIO_BASE + 12)     ///< CGPIO12
#define PAD_PIN_CGPIO13         (PAD_PIN_CGPIO_BASE + 13)     ///< CGPIO13
#define PAD_PIN_CGPIO14         (PAD_PIN_CGPIO_BASE + 14)     ///< CGPIO14
#define PAD_PIN_CGPIO15         (PAD_PIN_CGPIO_BASE + 15)     ///< CGPIO15

#define PAD_PIN_CGPIO16         (PAD_PIN_CGPIO_BASE + 16)     ///< CGPIO16
#define PAD_PIN_CGPIO17         (PAD_PIN_CGPIO_BASE + 17)     ///< CGPIO17
#define PAD_PIN_CGPIO18         (PAD_PIN_CGPIO_BASE + 18)     ///< CGPIO18
#define PAD_PIN_CGPIO19         (PAD_PIN_CGPIO_BASE + 19)     ///< CGPIO19
#define PAD_PIN_CGPIO20         (PAD_PIN_CGPIO_BASE + 20)     ///< CGPIO20
#define PAD_PIN_CGPIO21         (PAD_PIN_CGPIO_BASE + 21)     ///< CGPIO21
#define PAD_PIN_CGPIO22         (PAD_PIN_CGPIO_BASE + 22)     ///< CGPIO22
#define PAD_PIN_CGPIO23         (PAD_PIN_CGPIO_BASE + 23)     ///< CGPIO23
#define PAD_PIN_CGPIO24         (PAD_PIN_CGPIO_BASE + 24)     ///< CGPIO24
#define PAD_PIN_CGPIO25         (PAD_PIN_CGPIO_BASE + 25)     ///< CGPIO25
#define PAD_PIN_CGPIO26         (PAD_PIN_CGPIO_BASE + 26)     ///< CGPIO26
#define PAD_PIN_CGPIO27         (PAD_PIN_CGPIO_BASE + 27)     ///< CGPIO27
#define PAD_PIN_CGPIO28         (PAD_PIN_CGPIO_BASE + 28)     ///< CGPIO28
#define PAD_PIN_CGPIO29         (PAD_PIN_CGPIO_BASE + 29)     ///< CGPIO29
#define PAD_PIN_CGPIO30         (PAD_PIN_CGPIO_BASE + 30)     ///< CGPIO30
#define PAD_PIN_CGPIO31         (PAD_PIN_CGPIO_BASE + 31)     ///< CGPIO31
//CGPIOx group end

//SGPIOx group begin
#define PAD_PIN_SGPIO_BASE       100
#define PAD_PIN_SGPIO24         (PAD_PIN_SGPIO_BASE + 0)      ///< SGPIO24
#define PAD_PIN_SGPIO25         (PAD_PIN_SGPIO_BASE + 1)      ///< SGPIO25
#define PAD_PIN_SGPIO26         (PAD_PIN_SGPIO_BASE + 2)      ///< SGPIO26
#define PAD_PIN_SGPIO27         (PAD_PIN_SGPIO_BASE + 3)      ///< SGPIO27
#define PAD_PIN_SGPIO28         (PAD_PIN_SGPIO_BASE + 4)      ///< SGPIO28
#define PAD_PIN_SGPIO29         (PAD_PIN_SGPIO_BASE + 5)      ///< SGPIO29
//SGPIOx group end

//PGPIOx group begin
#define PAD_PIN_PGPIO_BASE       150
#define PAD_PIN_PGPIO0          (PAD_PIN_PGPIO_BASE + 0)      ///< PGPIO0
#define PAD_PIN_PGPIO1          (PAD_PIN_PGPIO_BASE + 1)      ///< PGPIO1
#define PAD_PIN_PGPIO2          (PAD_PIN_PGPIO_BASE + 2)      ///< PGPIO2
#define PAD_PIN_PGPIO3          (PAD_PIN_PGPIO_BASE + 3)      ///< PGPIO3
#define PAD_PIN_PGPIO4          (PAD_PIN_PGPIO_BASE + 4)      ///< PGPIO4
#define PAD_PIN_PGPIO5          (PAD_PIN_PGPIO_BASE + 5)      ///< PGPIO5
#define PAD_PIN_PGPIO6          (PAD_PIN_PGPIO_BASE + 6)      ///< PGPIO6
#define PAD_PIN_PGPIO7          (PAD_PIN_PGPIO_BASE + 7)      ///< PGPIO7
#define PAD_PIN_PGPIO8          (PAD_PIN_PGPIO_BASE + 8)      ///< PGPIO8
#define PAD_PIN_PGPIO9          (PAD_PIN_PGPIO_BASE + 9)      ///< PGPIO9
#define PAD_PIN_PGPIO10         (PAD_PIN_PGPIO_BASE + 10)     ///< PGPIO10
#define PAD_PIN_PGPIO11         (PAD_PIN_PGPIO_BASE + 11)     ///< PGPIO11
#define PAD_PIN_PGPIO12         (PAD_PIN_PGPIO_BASE + 12)     ///< PGPIO12
#define PAD_PIN_PGPIO13         (PAD_PIN_PGPIO_BASE + 13)     ///< PGPIO13
#define PAD_PIN_PGPIO14         (PAD_PIN_PGPIO_BASE + 14)     ///< PGPIO14
#define PAD_PIN_PGPIO15         (PAD_PIN_PGPIO_BASE + 15)     ///< PGPIO15

#define PAD_PIN_PGPIO16         (PAD_PIN_PGPIO_BASE + 16)     ///< PGPIO16
#define PAD_PIN_PGPIO17         (PAD_PIN_PGPIO_BASE + 17)     ///< PGPIO17
#define PAD_PIN_PGPIO18         (PAD_PIN_PGPIO_BASE + 18)     ///< PGPIO18
#define PAD_PIN_PGPIO19         (PAD_PIN_PGPIO_BASE + 19)     ///< PGPIO19
#define PAD_PIN_PGPIO20         (PAD_PIN_PGPIO_BASE + 20)     ///< PGPIO20
#define PAD_PIN_PGPIO21         (PAD_PIN_PGPIO_BASE + 21)     ///< PGPIO21
#define PAD_PIN_PGPIO22         (PAD_PIN_PGPIO_BASE + 22)     ///< PGPIO22
#define PAD_PIN_PGPIO23         (PAD_PIN_PGPIO_BASE + 23)     ///< PGPIO23
#define PAD_PIN_PGPIO24         (PAD_PIN_PGPIO_BASE + 24)     ///< PGPIO24
#define PAD_PIN_PGPIO25         (PAD_PIN_PGPIO_BASE + 25)     ///< PGPIO25
//#define PAD_PIN_PGPIO26         (PAD_PIN_PGPIO_BASE + 26)     ///< PGPIO26
#define PAD_PIN_PGPIO27         (PAD_PIN_PGPIO_BASE + 27)     ///< PGPIO27
#define PAD_PIN_PGPIO28         (PAD_PIN_PGPIO_BASE + 28)     ///< PGPIO28
#define PAD_PIN_PGPIO29         (PAD_PIN_PGPIO_BASE + 29)     ///< PGPIO29
#define PAD_PIN_PGPIO30         (PAD_PIN_PGPIO_BASE + 30)     ///< PGPIO30
#define PAD_PIN_PGPIO31         (PAD_PIN_PGPIO_BASE + 31)     ///< PGPIO31

#define PAD_PIN_PGPIO32         (PAD_PIN_PGPIO_BASE + 32)     ///< PGPIO32
#define PAD_PIN_PGPIO33         (PAD_PIN_PGPIO_BASE + 33)     ///< PGPIO33
#define PAD_PIN_PGPIO34         (PAD_PIN_PGPIO_BASE + 34)     ///< PGPIO34
#define PAD_PIN_PGPIO35         (PAD_PIN_PGPIO_BASE + 35)     ///< PGPIO35
#define PAD_PIN_PGPIO36         (PAD_PIN_PGPIO_BASE + 36)     ///< PGPIO36
#define PAD_PIN_PGPIO37         (PAD_PIN_PGPIO_BASE + 37)     ///< PGPIO37
#define PAD_PIN_PGPIO38         (PAD_PIN_PGPIO_BASE + 38)     ///< PGPIO38
#define PAD_PIN_PGPIO39         (PAD_PIN_PGPIO_BASE + 39)     ///< PGPIO39
#define PAD_PIN_PGPIO40         (PAD_PIN_PGPIO_BASE + 40)     ///< PGPIO40
#define PAD_PIN_PGPIO41         (PAD_PIN_PGPIO_BASE + 41)     ///< PGPIO41
#define PAD_PIN_PGPIO42         (PAD_PIN_PGPIO_BASE + 42)     ///< PGPIO42
#define PAD_PIN_PGPIO43         (PAD_PIN_PGPIO_BASE + 43)     ///< PGPIO43
#define PAD_PIN_PGPIO44         (PAD_PIN_PGPIO_BASE + 44)     ///< PGPIO44
#define PAD_PIN_PGPIO45         (PAD_PIN_PGPIO_BASE + 45)     ///< PGPIO45
#define PAD_PIN_PGPIO46         (PAD_PIN_PGPIO_BASE + 46)     ///< PGPIO46
#define PAD_PIN_PGPIO47         (PAD_PIN_PGPIO_BASE + 47)     ///< PGPIO47

#define PAD_PIN_PGPIO48         (PAD_PIN_PGPIO_BASE + 48)     ///< PGPIO48
#define PAD_PIN_PGPIO49         (PAD_PIN_PGPIO_BASE + 49)     ///< PGPIO49
#define PAD_PIN_PGPIO50         (PAD_PIN_PGPIO_BASE + 50)     ///< PGPIO50
#define PAD_PIN_PGPIO51         (PAD_PIN_PGPIO_BASE + 51)     ///< PGPIO51
#define PAD_PIN_PGPIO52         (PAD_PIN_PGPIO_BASE + 52)     ///< PGPIO52
#define PAD_PIN_PGPIO53         (PAD_PIN_PGPIO_BASE + 53)     ///< PGPIO53
#define PAD_PIN_PGPIO54         (PAD_PIN_PGPIO_BASE + 54)     ///< PGPIO54
#define PAD_PIN_PGPIO55         (PAD_PIN_PGPIO_BASE + 55)     ///< PGPIO55
#define PAD_PIN_PGPIO56         (PAD_PIN_PGPIO_BASE + 56)     ///< PGPIO56
#define PAD_PIN_PGPIO57         (PAD_PIN_PGPIO_BASE + 57)     ///< PGPIO57
#define PAD_PIN_PGPIO58         (PAD_PIN_PGPIO_BASE + 58)     ///< PGPIO58
#define PAD_PIN_PGPIO59         (PAD_PIN_PGPIO_BASE + 59)     ///< PGPIO59
#define PAD_PIN_PGPIO60         (PAD_PIN_PGPIO_BASE + 60)     ///< PGPIO60
#define PAD_PIN_PGPIO61         (PAD_PIN_PGPIO_BASE + 61)     ///< PGPIO61
//PGPIOx group end


//LGPIOx group begin
#define PAD_PIN_LGPIO_BASE      250
#define PAD_PIN_LGPIO0          (PAD_PIN_LGPIO_BASE + 0)       ///< LGPIO0
#define PAD_PIN_LGPIO1          (PAD_PIN_LGPIO_BASE + 1)       ///< LGPIO1
#define PAD_PIN_LGPIO2          (PAD_PIN_LGPIO_BASE + 2)       ///< LGPIO2
#define PAD_PIN_LGPIO3          (PAD_PIN_LGPIO_BASE + 3)       ///< LGPIO3
#define PAD_PIN_LGPIO4          (PAD_PIN_LGPIO_BASE + 4)       ///< LGPIO4
#define PAD_PIN_LGPIO5          (PAD_PIN_LGPIO_BASE + 5)       ///< LGPIO5
#define PAD_PIN_LGPIO6          (PAD_PIN_LGPIO_BASE + 6)       ///< LGPIO6
#define PAD_PIN_LGPIO7          (PAD_PIN_LGPIO_BASE + 7)       ///< LGPIO7
#define PAD_PIN_LGPIO8          (PAD_PIN_LGPIO_BASE + 8)       ///< LGPIO8
#define PAD_PIN_LGPIO9          (PAD_PIN_LGPIO_BASE + 9)       ///< LGPIO9
#define PAD_PIN_LGPIO10         (PAD_PIN_LGPIO_BASE + 10)      ///< LGPIO10
#define PAD_PIN_LGPIO11         (PAD_PIN_LGPIO_BASE + 11)      ///< LGPIO11
#define PAD_PIN_LGPIO12         (PAD_PIN_LGPIO_BASE + 12)      ///< LGPIO12
#define PAD_PIN_LGPIO13         (PAD_PIN_LGPIO_BASE + 13)      ///< LGPIO13
#define PAD_PIN_LGPIO14         (PAD_PIN_LGPIO_BASE + 14)      ///< LGPIO14
#define PAD_PIN_LGPIO15         (PAD_PIN_LGPIO_BASE + 15)      ///< LGPIO15

#define PAD_PIN_LGPIO16         (PAD_PIN_LGPIO_BASE + 16)      ///< LGPIO16
#define PAD_PIN_LGPIO17         (PAD_PIN_LGPIO_BASE + 17)      ///< LGPIO17
#define PAD_PIN_LGPIO18         (PAD_PIN_LGPIO_BASE + 18)      ///< LGPIO18
#define PAD_PIN_LGPIO19         (PAD_PIN_LGPIO_BASE + 19)      ///< LGPIO19
#define PAD_PIN_LGPIO20         (PAD_PIN_LGPIO_BASE + 20)      ///< LGPIO20
#define PAD_PIN_LGPIO21         (PAD_PIN_LGPIO_BASE + 21)      ///< LGPIO21
#define PAD_PIN_LGPIO22         (PAD_PIN_LGPIO_BASE + 22)      ///< LGPIO22
#define PAD_PIN_LGPIO23         (PAD_PIN_LGPIO_BASE + 23)      ///< LGPIO23
#define PAD_PIN_LGPIO24         (PAD_PIN_LGPIO_BASE + 24)      ///< LGPIO24
#define PAD_PIN_LGPIO25         (PAD_PIN_LGPIO_BASE + 25)      ///< LGPIO25
#define PAD_PIN_LGPIO26         (PAD_PIN_LGPIO_BASE + 26)      ///< LGPIO26
#define PAD_PIN_LGPIO27         (PAD_PIN_LGPIO_BASE + 27)      ///< LGPIO27
//LGPIOx group end

//DGPIO group begin
#define PAD_PIN_DGPIO_BASE      300
#define PAD_PIN_DGPIO0          (PAD_PIN_DGPIO_BASE + 0)       ///< DGPIO0
#define PAD_PIN_DGPIO1          (PAD_PIN_DGPIO_BASE + 1)       ///< DGPIO1
#define PAD_PIN_DGPIO2          (PAD_PIN_DGPIO_BASE + 2)       ///< DGPIO2
#define PAD_PIN_DGPIO3          (PAD_PIN_DGPIO_BASE + 3)       ///< DGPIO3
#define PAD_PIN_DGPIO4          (PAD_PIN_DGPIO_BASE + 4)       ///< DGPIO4
#define PAD_PIN_DGPIO5          (PAD_PIN_DGPIO_BASE + 5)       ///< DGPIO5
#define PAD_PIN_DGPIO6          (PAD_PIN_DGPIO_BASE + 6)       ///< DGPIO6
#define PAD_PIN_DGPIO7          (PAD_PIN_DGPIO_BASE + 7)       ///< DGPIO7
#define PAD_PIN_DGPIO8          (PAD_PIN_DGPIO_BASE + 8)       ///< DGPIO8
//DGPIO group end

//@}



#define PAD_DS_GROUP2_10        0
#define PAD_DS_GROUP5_20        0x1000
#define PAD_DS_GROUP12_40       0x2000
#define PAD_DS_GROUP12_20       0x4000

/**
    @name Pad driving pin ID.

    Pad driving pin ID.

    Pad ID of pad_setDrivingSink()
*/
//@{
//CGPIO group begin
#define PAD_DS_CGPIO_BASE      0
#define PAD_DS_CGPIO0          (PAD_DS_CGPIO_BASE + 0)      ///< CGPIO0
#define PAD_DS_CGPIO1          (PAD_DS_CGPIO_BASE + 1)      ///< CGPIO1
#define PAD_DS_CGPIO2          (PAD_DS_CGPIO_BASE + 2)      ///< CGPIO2
#define PAD_DS_CGPIO3          (PAD_DS_CGPIO_BASE + 3)      ///< CGPIO3
#define PAD_DS_CGPIO4          (PAD_DS_CGPIO_BASE + 4)      ///< CGPIO4
#define PAD_DS_CGPIO5          (PAD_DS_CGPIO_BASE + 5)      ///< CGPIO5
#define PAD_DS_CGPIO6          (PAD_DS_CGPIO_BASE + 6)      ///< CGPIO6
#define PAD_DS_CGPIO7          (PAD_DS_CGPIO_BASE + 7)      ///< CGPIO7
#define PAD_DS_CGPIO8          (PAD_DS_CGPIO_BASE + 8)      ///< CGPIO8
#define PAD_DS_CGPIO9          (PAD_DS_CGPIO_BASE + 9)      ///< CGPIO9
#define PAD_DS_CGPIO10         (PAD_DS_CGPIO_BASE + 10)     ///< CGPIO10
#define PAD_DS_CGPIO11         (PAD_DS_CGPIO_BASE + 11)     ///< CGPIO11
#define PAD_DS_CGPIO12         (PAD_DS_CGPIO_BASE + 12)     ///< CGPIO12
#define PAD_DS_CGPIO13         (PAD_DS_CGPIO_BASE + 13)     ///< CGPIO13
#define PAD_DS_CGPIO14         (PAD_DS_CGPIO_BASE + 14)     ///< CGPIO14
#define PAD_DS_CGPIO15         (PAD_DS_CGPIO_BASE + 15)     ///< CGPIO15
#define PAD_DS_CGPIO16         ((PAD_DS_CGPIO_BASE + 16) | PAD_DS_GROUP5_20)    ///< CGPIO16
#define PAD_DS_CGPIO17         (PAD_DS_CGPIO_BASE + 17)     ///< CGPIO17
#define PAD_DS_CGPIO18         (PAD_DS_CGPIO_BASE + 18)     ///< CGPIO18
#define PAD_DS_CGPIO19         (PAD_DS_CGPIO_BASE + 19)     ///< CGPIO19
#define PAD_DS_CGPIO20         (PAD_DS_CGPIO_BASE + 20)     ///< CGPIO20
#define PAD_DS_CGPIO21         (PAD_DS_CGPIO_BASE + 21)     ///< CGPIO21
#define PAD_DS_CGPIO22         ((PAD_DS_CGPIO_BASE + 22) | PAD_DS_GROUP5_20)    ///< CGPIO22
#define PAD_DS_CGPIO23         (PAD_DS_CGPIO_BASE + 23)     ///< CGPIO23
#define PAD_DS_CGPIO24         (PAD_DS_CGPIO_BASE + 24)     ///< CGPIO24
#define PAD_DS_CGPIO25         (PAD_DS_CGPIO_BASE + 25)     ///< CGPIO25
#define PAD_DS_CGPIO26         (PAD_DS_CGPIO_BASE + 26)     ///< CGPIO26
#define PAD_DS_CGPIO27         (PAD_DS_CGPIO_BASE + 27)     ///< CGPIO27
#define PAD_DS_CGPIO28         (PAD_DS_CGPIO_BASE + 28)     ///< CGPIO28
#define PAD_DS_CGPIO29         (PAD_DS_CGPIO_BASE + 29)     ///< CGPIO29
#define PAD_DS_CGPIO30         (PAD_DS_CGPIO_BASE + 30)     ///< CGPIO30
#define PAD_DS_CGPIO31         (PAD_DS_CGPIO_BASE + 31)     ///< CGPIO31
//CGPIO group end

//SGPIO group Driving/Sink begin
#define PAD_DS_SGPIO_BASE      32
#define PAD_DS_SGPIO24         ((PAD_DS_SGPIO_BASE + 0) | PAD_DS_GROUP5_20)     ///< SGPIO24
#define PAD_DS_SGPIO25         ((PAD_DS_SGPIO_BASE + 1) | PAD_DS_GROUP5_20) ///< SGPIO25
#define PAD_DS_SGPIO26         (PAD_DS_SGPIO_BASE + 2)      ///< SGPIO26
#define PAD_DS_SGPIO27         (PAD_DS_SGPIO_BASE + 3)      ///< SGPIO27
#define PAD_DS_SGPIO28         ((PAD_DS_SGPIO_BASE + 4) | PAD_DS_GROUP5_20)     ///< SGPIO28
#define PAD_DS_SGPIO29         ((PAD_DS_SGPIO_BASE + 5) | PAD_DS_GROUP12_40)    ///< SGPIO29
//SGPIO group Driving/Sink end

//PGPIO group Driving/Sink begin
#define PAD_DS_PGPIO_BASE      48
#define PAD_DS_PGPIO0          (PAD_DS_PGPIO_BASE + 0)      ///< PGPIO0
#define PAD_DS_PGPIO1          (PAD_DS_PGPIO_BASE + 1)      ///< PGPIO1
#define PAD_DS_PGPIO2          (PAD_DS_PGPIO_BASE + 2)      ///< PGPIO2
#define PAD_DS_PGPIO3          (PAD_DS_PGPIO_BASE + 3)      ///< PGPIO3
#define PAD_DS_PGPIO4          (PAD_DS_PGPIO_BASE + 4)      ///< PGPIO4
#define PAD_DS_PGPIO5          (PAD_DS_PGPIO_BASE + 5)      ///< PGPIO5
#define PAD_DS_PGPIO6          (PAD_DS_PGPIO_BASE + 6)      ///< PGPIO6
#define PAD_DS_PGPIO7          (PAD_DS_PGPIO_BASE + 7)      ///< PGPIO7
#define PAD_DS_PGPIO8          (PAD_DS_PGPIO_BASE + 8)      ///< PGPIO8
#define PAD_DS_PGPIO9          (PAD_DS_PGPIO_BASE + 9)      ///< PGPIO9
#define PAD_DS_PGPIO10         (PAD_DS_PGPIO_BASE + 10)     ///< PGPIO10
#define PAD_DS_PGPIO11         (PAD_DS_PGPIO_BASE + 11)     ///< PGPIO11
#define PAD_DS_PGPIO12         (PAD_DS_PGPIO_BASE + 12)     ///< PGPIO12
#define PAD_DS_PGPIO13         (PAD_DS_PGPIO_BASE + 13)     ///< PGPIO13
#define PAD_DS_PGPIO14         (PAD_DS_PGPIO_BASE + 14)     ///< PGPIO14
#define PAD_DS_PGPIO15         (PAD_DS_PGPIO_BASE + 15)     ///< PGPIO15

#define PAD_DS_PGPIO16         (PAD_DS_PGPIO_BASE + 16)     ///< PGPIO16
#define PAD_DS_PGPIO17         (PAD_DS_PGPIO_BASE + 17)     ///< PGPIO17
#define PAD_DS_PGPIO18         (PAD_DS_PGPIO_BASE + 18)     ///< PGPIO18
#define PAD_DS_PGPIO19         (PAD_DS_PGPIO_BASE + 19)     ///< PGPIO19
#define PAD_DS_PGPIO20         (PAD_DS_PGPIO_BASE + 20)     ///< PGPIO20
#define PAD_DS_PGPIO21         (PAD_DS_PGPIO_BASE + 21)     ///< PGPIO21
#define PAD_DS_PGPIO22         (PAD_DS_PGPIO_BASE + 22)     ///< PGPIO22
#define PAD_DS_PGPIO23         (PAD_DS_PGPIO_BASE + 23)     ///< PGPIO23
#define PAD_DS_PGPIO24         (PAD_DS_PGPIO_BASE + 24)     ///< PGPIO24
#define PAD_DS_PGPIO25         ((PAD_DS_PGPIO_BASE + 25) | PAD_DS_GROUP12_20)   ///< PGPIO25
#define PAD_DS_DGPIO8          (PAD_DS_PGPIO_BASE + 26)     ///< DGPIO8
//#define PAD_DS_PGPIO26         (PAD_DS_PGPIO_BASE + 26)     ///< PGPIO26
#define PAD_DS_PGPIO27         (PAD_DS_PGPIO_BASE + 27)     ///< PGPIO27
#define PAD_DS_PGPIO28         (PAD_DS_PGPIO_BASE + 28)     ///< PGPIO28
#define PAD_DS_PGPIO29         (PAD_DS_PGPIO_BASE + 29)     ///< PGPIO29
#define PAD_DS_PGPIO30         (PAD_DS_PGPIO_BASE + 30)     ///< PGPIO30
#define PAD_DS_PGPIO31         (PAD_DS_PGPIO_BASE + 31)     ///< PGPIO31

#define PAD_DS_PGPIO32         (PAD_DS_PGPIO_BASE + 32)     ///< PGPIO32
#define PAD_DS_PGPIO33         (PAD_DS_PGPIO_BASE + 33)     ///< PGPIO33
#define PAD_DS_PGPIO34         (PAD_DS_PGPIO_BASE + 34)     ///< PGPIO34
#define PAD_DS_PGPIO35         (PAD_DS_PGPIO_BASE + 35)     ///< PGPIO35
#define PAD_DS_PGPIO36         (PAD_DS_PGPIO_BASE + 36)     ///< PGPIO36
#define PAD_DS_PGPIO37         (PAD_DS_PGPIO_BASE + 37)     ///< PGPIO37
#define PAD_DS_PGPIO38         (PAD_DS_PGPIO_BASE + 38)     ///< PGPIO38
#define PAD_DS_PGPIO39         (PAD_DS_PGPIO_BASE + 39)     ///< PGPIO39
#define PAD_DS_PGPIO40         (PAD_DS_PGPIO_BASE + 40)     ///< PGPIO40
#define PAD_DS_PGPIO41         (PAD_DS_PGPIO_BASE + 41)     ///< PGPIO41
#define PAD_DS_PGPIO42         (PAD_DS_PGPIO_BASE + 42)     ///< PGPIO42
#define PAD_DS_PGPIO43         (PAD_DS_PGPIO_BASE + 43)     ///< PGPIO43
#define PAD_DS_PGPIO44         (PAD_DS_PGPIO_BASE + 44)     ///< PGPIO44
#define PAD_DS_PGPIO45         (PAD_DS_PGPIO_BASE + 45)     ///< PGPIO45
#define PAD_DS_PGPIO46         (PAD_DS_PGPIO_BASE + 46)     ///< PGPIO46
#define PAD_DS_PGPIO47         (PAD_DS_PGPIO_BASE + 47)     ///< PGPIO47

#define PAD_DS_PGPIO48         (PAD_DS_PGPIO_BASE + 48)     ///< PGPIO48
#define PAD_DS_PGPIO49         (PAD_DS_PGPIO_BASE + 49)     ///< PGPIO49
#define PAD_DS_PGPIO50         (PAD_DS_PGPIO_BASE + 50)     ///< PGPIO50
#define PAD_DS_PGPIO51         (PAD_DS_PGPIO_BASE + 51)     ///< PGPIO51
#define PAD_DS_PGPIO52         (PAD_DS_PGPIO_BASE + 52)     ///< PGPIO52
#define PAD_DS_PGPIO53         (PAD_DS_PGPIO_BASE + 53)     ///< PGPIO53
#define PAD_DS_PGPIO54         (PAD_DS_PGPIO_BASE + 54)     ///< PGPIO54
#define PAD_DS_PGPIO55         (PAD_DS_PGPIO_BASE + 55)     ///< PGPIO55
#define PAD_DS_PGPIO56         ((PAD_DS_PGPIO_BASE + 56) | PAD_DS_GROUP5_20)    ///< PGPIO56
#define PAD_DS_PGPIO57         (PAD_DS_PGPIO_BASE + 57)     ///< PGPIO57
#define PAD_DS_PGPIO58         (PAD_DS_PGPIO_BASE + 58)     ///< PGPIO58
#define PAD_DS_PGPIO59         (PAD_DS_PGPIO_BASE + 59)     ///< PGPIO59
#define PAD_DS_PGPIO60         (PAD_DS_PGPIO_BASE + 60)     ///< PGPIO60
#define PAD_DS_PGPIO61         (PAD_DS_PGPIO_BASE + 61)     ///< PGPIO61
//PGPIO group Driving/Sink end

//LCD group Driving/Sink begin
#define PAD_DS_LGPIO_BASE      112
#define PAD_DS_LGPIO0          (PAD_DS_LGPIO_BASE + 0)      ///< LGPIO0
#define PAD_DS_LGPIO1          (PAD_DS_LGPIO_BASE + 1)      ///< LGPIO1
#define PAD_DS_LGPIO2          (PAD_DS_LGPIO_BASE + 2)      ///< LGPIO2
#define PAD_DS_LGPIO3          (PAD_DS_LGPIO_BASE + 3)      ///< LGPIO3
#define PAD_DS_LGPIO4          (PAD_DS_LGPIO_BASE + 4)      ///< LGPIO4
#define PAD_DS_LGPIO5          (PAD_DS_LGPIO_BASE + 5)      ///< LGPIO5
#define PAD_DS_LGPIO6          (PAD_DS_LGPIO_BASE + 6)      ///< LGPIO6
#define PAD_DS_LGPIO7          (PAD_DS_LGPIO_BASE + 7)      ///< LGPIO7
#define PAD_DS_LGPIO8          (PAD_DS_LGPIO_BASE + 8)      ///< LGPIO8
#define PAD_DS_LGPIO9          (PAD_DS_LGPIO_BASE + 9)      ///< LGPIO9
#define PAD_DS_LGPIO10         (PAD_DS_LGPIO_BASE + 10)     ///< LGPIO10
#define PAD_DS_LGPIO11         (PAD_DS_LGPIO_BASE + 11)     ///< LGPIO11
#define PAD_DS_LGPIO12         (PAD_DS_LGPIO_BASE + 12)     ///< LGPIO12
#define PAD_DS_LGPIO13         (PAD_DS_LGPIO_BASE + 13)     ///< LGPIO13
#define PAD_DS_LGPIO14         (PAD_DS_LGPIO_BASE + 14)     ///< LGPIO14
#define PAD_DS_LGPIO15         (PAD_DS_LGPIO_BASE + 15)     ///< LGPIO15

#define PAD_DS_LGPIO16         (PAD_DS_LGPIO_BASE + 16)     ///< LGPIO16
#define PAD_DS_LGPIO17         (PAD_DS_LGPIO_BASE + 17)     ///< LGPIO17
#define PAD_DS_LGPIO18         (PAD_DS_LGPIO_BASE + 18)     ///< LGPIO18
#define PAD_DS_LGPIO19         (PAD_DS_LGPIO_BASE + 19)     ///< LGPIO19
#define PAD_DS_LGPIO20         (PAD_DS_LGPIO_BASE + 20)     ///< LGPIO20
#define PAD_DS_LGPIO21         (PAD_DS_LGPIO_BASE + 21)     ///< LGPIO21
#define PAD_DS_LGPIO22         (PAD_DS_LGPIO_BASE + 22)     ///< LGPIO22
#define PAD_DS_LGPIO23         (PAD_DS_LGPIO_BASE + 23)     ///< LGPIO23
#define PAD_DS_LGPIO24         (PAD_DS_LGPIO_BASE + 24)     ///< LGPIO24
#define PAD_DS_LGPIO25         (PAD_DS_LGPIO_BASE + 25)     ///< LGPIO25
#define PAD_DS_LGPIO26         (PAD_DS_LGPIO_BASE + 26)     ///< LGPIO26
#define PAD_DS_LGPIO27         (PAD_DS_LGPIO_BASE + 27)     ///< LGPIO27
//LCD group Driving/Sink end

//DGPIO group Driving/Sink begin
#define PAD_DS_DGPIO_BASE      144
#define PAD_DS_DGPIO0          ((PAD_DS_DGPIO_BASE + 0) | PAD_DS_GROUP5_20)     ///< DGPIO0
#define PAD_DS_DGPIO1          ((PAD_DS_DGPIO_BASE + 1) | PAD_DS_GROUP5_20)     ///< DGPIO1
#define PAD_DS_DGPIO2          ((PAD_DS_DGPIO_BASE + 2) | PAD_DS_GROUP5_20)     ///< DGPIO2
#define PAD_DS_DGPIO3          ((PAD_DS_DGPIO_BASE + 3) | PAD_DS_GROUP5_20)     ///< DGPIO3
#define PAD_DS_DGPIO4          (PAD_DS_DGPIO_BASE + 4)      ///< DGPIO4
#define PAD_DS_DGPIO5          (PAD_DS_DGPIO_BASE + 5)      ///< DGPIO5
#define PAD_DS_DGPIO6          (PAD_DS_DGPIO_BASE + 6)      ///< DGPIO6
#define PAD_DS_DGPIO7          (PAD_DS_DGPIO_BASE + 7)      ///< DGPIO7
//DGPIO group Driving/Sink end
//@}


extern ER pad_setPullUpDown(UINT32 pin, PAD_PULL pulltype);
extern ER pad_setDrivingSink(UINT32 name, PAD_DRIVINGSINK driving);


//@}

#endif
