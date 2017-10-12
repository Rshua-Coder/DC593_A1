/**
    Public header file for Sensor driver

    This file is the header file that define the API and data type for Sensor driver.

    @file       Sensor.h
    @ingroup    mIDrvSensor
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _SENSOR_H
#define _SENSOR_H

//----------Header file include-------------------------------------------------
#include "Type.h"
#include "ErrorNo.h"
#include "sensori2c.h"

/** \addtogroup mISensor */
//@{

#define SEN_IGNORE  0xffffffff  ///< sensor ignore information

/**
    Sensor ID
*/
typedef enum
{
    SENSOR_ID_1 = 0,                ///< sensor id 0
    SENSOR_ID_2,                    ///< sensor id 1
    SENSOR_ID_MAX_NUM,
    SENSOR_ID_NONE = 0xffffffff,    ///< sensor id null
} SENSOR_ID;

/**
    sensor signal type
*/
typedef enum
{
    SENSOR_SIGNAL_MASTER = 0,       ///< sensor output HD and VD signal
    SENSOR_SIGNAL_SLAVE,            ///< dsp output HD and VD signal
    SENSOR_SIGNAL_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_SIGNAL_TYPE)
} SENSOR_SIGNAL_TYPE;

/**
    sensor type
*/
typedef enum
{
    SENSOR_TYPE_CCD = 0,            ///< CCD
    SENSOR_TYPE_CMOS,               ///< CMOS
    SENSOR_TYPE_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_TYPE)
} SENSOR_TYPE;

/**
    sensor output data type
*/
typedef enum
{
    SENSOR_DATA_PARALLEL = 0,       ///< parallel
    SENSOR_DATA_LVDS,               ///< LVDS
    SENSOR_DATA_MIPI,               ///< MIPI
    SENSOR_DATA_CCIR,               ///< DVI
    SENSOR_DATA_DUMMY,
    SENSOR_DATA_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_DATA_TYPE)
} SENSOR_DATA_TYPE;

/**
    sensor command interface
*/
typedef enum
{
    SENSOR_CMD_UNKNOWN = 0,
    SENSOR_CMD_SIF,                 ///< serial
    SENSOR_CMD_I2C,                 ///< I2C
    SENSOR_CMD_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_CMD_TYPE)
} SENSOR_CMD_TYPE;

/**
    Sensor mode
*/
typedef enum _SENSOR_MODE
{
    SENSOR_MODE_UNKNOWN = 0,    ///< Unknow Mode
    SENSOR_MODE_1       = 1,    ///< Sensor mode 1
    SENSOR_MODE_2       = 2,    ///< Sensor mode 2
    SENSOR_MODE_3       = 3,    ///< Sensor mode 3
    SENSOR_MODE_4       = 4,    ///< Sensor mode 4
    SENSOR_MODE_5       = 5,    ///< Sensor mode 5
    SENSOR_MODE_6       = 6,    ///< Sensor mode 6
    SENSOR_MODE_7       = 7,    ///< Sensor mode 7
    SENSOR_MODE_8       = 8,    ///< Sensor mode 8
    SENSOR_MODE_9       = 9,    ///< Sensor mode 9
    SENSOR_MODE_10      = 10,   ///< Sensor mode 10
    SENSOR_MODE_11      = 11,   ///< Sensor mode 11
    SENSOR_MODE_12      = 12,   ///< Sensor mode 12
    SENSOR_MODE_13      = 13,   ///< Sensor mode 13
    SENSOR_MODE_14      = 14,   ///< Sensor mode 14
    SENSOR_MODE_15      = 15,   ///< Sensor mode 15
    SENSOR_MODE_Num,
    ENUM_DUMMY4WORD(SENSOR_MODE)
} SENSOR_MODE, *PSENSOR_MODE;

/**
    Sensor mode ratio
*/
typedef enum _SENSOR_RATIO
{
    SENSOR_RATIO_4_3 = 0,       ///< Sensor ratio 4:3
    SENSOR_RATIO_3_2,           ///< Sensor ratio 3:2
    SENSOR_RATIO_16_9,          ///< Sensor ratio 16:9
    SENSOR_RATIO_1_1,           ///< Sensor ratio 1:1
    SENSOR_RATIO_9_16,          ///< Sensor ratio 9:16
    SENSOR_RATIO_2_3,           ///< Sensor ratio 2:3
    SENSOR_RATIO_3_4,           ///< Sensor ratio 3:4
    SENSOR_RATIO_2_1,			///< Sensor ratio 2:1
    SENSOR_RATIO_Num,
    ENUM_DUMMY4WORD(SENSOR_RATIO)
} SENSOR_RATIO, *PSENSOR_RATIO;

/**
    Sensor start pixel
*/
typedef enum
{
    SENSOR_STPIX_R = 0,         ///< start pixel R
    SENSOR_STPIX_GR,            ///< start pixel GR
    SENSOR_STPIX_GB,            ///< start pixel GB
    SENSOR_STPIX_B,             ///< start pixel B
    SENSOR_STPIX_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_STPIX)
} SENSOR_STPIX;

/**
    Sensor data format
*/
typedef enum
{
    SENSOR_FMT_POGRESSIVE = 0,      ///< pogressive mode
    SENSOR_FMT_2_LINE,              ///< 2-line mix
    SENSOR_FMT_3_LINE,              ///< 3-line mix
    SENOSR_FMT_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_FMT)
} SENSOR_FMT;


/**
    Sensor Act Select(for CCIR656 only)
*/
typedef enum
{
    SENSOR_DVI_SEL_EAV = 0,         ///< EAV code recognition
    SENSOR_DVI_SEL_ACTWIN,          ///< Active window size setting
    SENOSR_DVI_SEL_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_DVI_SEL)
} SENSOR_DVI_SEL;

/**
    Sensor DVI MODE
*/
typedef enum
{
    SENSOR_DVI_MODE_SD = 0,         ///< DVI sd mode
    SENSOR_DVI_MODE_HD,             ///< DVI hd mode
    SENOSR_DVI_MODE_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_DVI_MODE)
} SENSOR_DVI_MODE;


/**
    Sensor DVI format
*/
typedef enum
{
    SENSOR_DVI_CCIR601 = 0,         ///< DVI CCIR 601
    SENSOR_DVI_CCIR656,             ///< DVI CCIR 656
    SENOSR_DVI_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_DVI_FMT)
} SENSOR_DVI_FMT;


/**
    Sensor Option
*/
typedef enum _SENSOR_OPTION
{
    SENSOR_OPTION_NONE = 0,       ///< none
    SENSOR_OPTION_MIRROR_FLIP,    ///< Sensor mirror & flip
    SENSOR_OPTION_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_OPTION)
} SENSOR_OPTION, *PSENSOR_OPTION;

/**
    Sensor signal

    @note relative falling edge
*/
typedef struct
{
    UINT32 Sync;            ///< sync
    UINT32 Period;          ///< period
    UINT32 DataStart;       ///< valid data start pos
    UINT32 DataSize;        ///< valid data size
} SENSOR_SIGNAL;

/**
    Sensor OB position
    @note relative falling edge
*/
typedef struct
{
    UINT32 TopWin[4];          ///< OB window left-top point(X1(Pt[0]), Y1(Pt[1])), right-bottom point(X2(Pt[2]), Y2(Pt[3]))
    UINT32 BottomWin[4];       ///< OB window left-top point(X1(Pt[0]), Y1(Pt[1])), right-bottom point(X2(Pt[2]), Y2(Pt[3]))
    UINT32 LeftWin[4];         ///< OB window left-top point(X1(Pt[0]), Y1(Pt[1])), right-bottom point(X2(Pt[2]), Y2(Pt[3]))
    UINT32 RightWin[4];        ///< OB window left-top point(X1(Pt[0]), Y1(Pt[1])), right-bottom point(X2(Pt[2]), Y2(Pt[3]))
  } SENSOR_OB;

/**
    Sensor field map
*/
typedef struct
{
    UINT32 Map[32];      ///< field map(3 -> 2 -> 1 ......), min value 1
} SENSOR_FIELD_TAB;

/**
    Sensor div map
*/
typedef struct
{
    UINT32 RouteMap[10];    ///< div route map(3 -> 2 -> 1 -> 2 -> 3 ......), min value 1
} SENSOR_DIV_TAB;

/**
    Sensor lvds ctrl pattern
*/
typedef struct
{
    UINT32 CtrlHD;          ///< lvds ctrl words
    UINT32 MaskHD;          ///< lvds mask
    UINT32 CtrlVD;          ///< lvds ctrl words
    UINT32 MaskVD;          ///< lvds mask
} SENSOR_LVDS_CTRLPTN;


/**
    Sensor lvds sync code pattern
*/
typedef struct
{
    UINT32 Num;             ///< sync code total number
    UINT32 Code[7];         ///< sync code
} SENSOR_LVDS_SYNC_CODE_PTN;

/**
    Sensor lvds information
*/
typedef struct
{
    UINT32 XHS;                         ///< HD length for sony sensor only.
    UINT32 XVS;                         ///< VD length for sony sensor only.
    UINT32 Width;                       ///< data output width
    UINT32 Height;                      ///< data output height
    UINT32 LaneNum;                     ///< data lanes number
    UINT32 BitDepth;                    ///< data bits
    UINT32 DataAlign;                   ///< data MSB/LSB
    UINT32 OutputPixelOrder[10];        ///< output data pixel order
} SENSOR_LVDS;

/**
    Sensor command structure
*/
typedef struct _SENSOR_CMD
{
    UINT32 uiAddr;                      ///< address
    UINT32 uiDataLen;                   ///< data length(bytes)
    UINT32 uiData[2];                   ///< data idx1(LSB) -> idx2(MSB)
} SENSOR_CMD;

/**
    Sensor DVI structure
*/
typedef struct _SENSOR_DVI
{
    SENSOR_DVI_FMT Fmt;                 ///< dvi format
    SENSOR_DVI_MODE Mode;               ///< dvi mode
    SENSOR_DVI_SEL ActSel;              ///< dvi EVA code recognition or Act window(CCIR656 only)
    BOOL FldEn;                         ///< start input from field with internal field signal=0
    BOOL FldInvEn;                      ///< inverse the polarity of internal field signal
    BOOL InSwapEn;                      ///< input Y/C channel pin swap for CCIR656
    BOOL OutSwapEn;                     ///< output Y/C channel pin swap for CCIR656
} SENSOR_DVI;

/**
    Sensor mode information
*/
typedef struct
{
    UINT32 SIEFreq;                 ///< SIE clock frequence Hz
    UINT32 MCLKFreq;                ///< MCLK frequence Hz
    SENSOR_STPIX StPix;             ///< Sensor start pixel
    SENSOR_FMT Fmt;                 ///< Sensor data type
    SENSOR_RATIO Ratio;             ///< Sensor ratio
    SENSOR_OB OB;                   ///< Sensor OB
    UINT32 FrameRate;               ///< frame rate X 10
    UINT32 Pclk;                    ///< pixel clock Hz
    UINT32 biningRatio;             ///< binning ratio X 100
    UINT32 StrLnT;                  ///< length from VD start to 1st active line(including OB), uint:us
    UINT32 EndLnT;                  ///< length from VD start to last active line(including OB), uint:us
    UINT32 TransDelyT;              ///< length from exposure end to start of data transmission, uint:us

    UINT32 FieldNum;                ///< field number 1 ~ x
    SENSOR_SIGNAL *TransHD;         ///< transfer HD signal
    SENSOR_SIGNAL *TransVD;         ///< transfer VD signal
    SENSOR_SIGNAL *SenHD;           ///< Sensor HD signal
    SENSOR_SIGNAL *SenVD;           ///< Sensor VD signal
    SENSOR_FIELD_TAB *FieldTab;     ///< Sensor output field

    UINT32 DIVNum;                  ///< division number 1 ~ x
    SENSOR_DIV_TAB *DIVTab;         ///< division map table
    SENSOR_LVDS *LVDS;              ///< lvds information

    SENSOR_DVI *DVI;                ///< dvi information
} SENSOR_MODE_INFO;

/**
    Sensor information
*/
typedef struct
{
    SENSOR_TYPE SenType;            ///< sensor type
    SENSOR_SIGNAL_TYPE SigType;     ///< sensor mask or slave
    SENSOR_DATA_TYPE DataType;      ///< transfer type
    UINT32 CellWidth;               ///< cell width mm * 1000
    UINT32 CellHeight;              ///< cell height mm * 1000
    SENSOR_MODE_INFO *Mode;         ///< sensor current mode
} SENSOR_INFO;

/**
    command information
*/
typedef struct _SEN_SIF
{
    UINT32      chanel;     ///< map to enum:SIF channel
    UINT32      busclk;     ///< unit:HZ
    UINT32      sen_d_s;    ///< map to sif duration, ref:SIF_CONFIG_ID_SENS,SIF_CONFIG_ID_SEND
    UINT32      sen_h;      ///< map to sif hold time,ref:SIF_CONFIG_ID_SENH
    BOOL        DMA_en;     ///< dma mode
}SEN_SIF;

/**
    I2C information
*/
typedef struct _SEN_I2C
{
    SENSOR_I2C I2c;                 ///< I2C channel
    UINT32  sclk;                   ///< I2C clock
    BOOL    extSlaveAddrEnable;     ///< using slave address or not
    UINT32  extSlaveRAddr;          ///< read command address
    UINT32  extSlaveWAddr;          ///< write command address
}SEN_I2C;

/**
    Sensor command information
*/
typedef struct
{
    SENSOR_CMD_TYPE CmdType;        ///< command type
    union
    {
        SEN_SIF SIF;                ///< SIF information
        SEN_I2C I2C;                ///< I2C information
    } INFO;
} SENSOR_CMD_INFO;

/**
    MCLK information
*/
typedef struct
{
    BOOL   bSieMClkEn;            ///< Enable/Disable MCLK to sensor
    UINT32 uiMClkSrc;             ///< config MCLK source Id
    UINT32 uiMClkFreq;            ///< MclkFreq Hz
} SENSOR_PCLK_INFO;

/**
    sensor initial object
*/
typedef struct
{
    SENSOR_PCLK_INFO PclkInfo;      ///< clock information
    SENSOR_PCLK_INFO Pclk2Info;      ///< clock information
    SENSOR_CMD_INFO CmdInfo;        ///< command information
    UINT32 *Sen2LVDSPinMap;          ///< hw 2 lvds pin map
    SENSOR_OPTION Option;           ///< Special Command
} SENSOR_INIT_OBJ;


/**
    sensor eshutter info
*/
typedef struct
{
    UINT32 uiFrame;         ///< exposure frame
    UINT32 uiLine;          ///< exposure line
    UINT32 uiPixel;         ///< exposure pixel
} EXPOSURE_SETTING;

/**
    sensor gain info
*/
typedef struct
{
    UINT32 data1;         ///< depend on sensor register
    UINT32 data2;         ///< depend on sensor register
    UINT32 data3;         ///< depend on sensor register
} GAIN_SETTING;

/**
    sensor driver common interface
*/
typedef struct
{
    //Common
    ER    (*fpInit)(SENSOR_INIT_OBJ *InitObj);              ///< initial sensor flow
    ER    (*fpUnInit)(void);                                ///< un-initial sensor flow
    ER    (*fpPwron)(void);                                 ///< power on flow
    ER    (*fpPwroff)(void);                                ///< power off flow
    ER    (*fpSleep)(void);                                 ///< enter sensor sleep mode
    ER    (*fpWakeup)(void);                                ///< exit sensor sleep mode
    ER    (*fpWrite)(SENSOR_CMD *Cmd);                      ///< write command
    ER    (*fpRead)(SENSOR_CMD *Cmd);                       ///< read command

    ER    (*fpChgMode)(SENSOR_MODE Mode);                   ///< change mode flow
    ER    (*fpGetCurMode)(SENSOR_MODE *Mode);               ///< get current mode

    ER    (*fpGetStatus)(SENSOR_MODE Mode, SENSOR_INFO *Info);  ///< get sensor mode information
    UINT32 (*fpGetModeNum)(void);                               ///< get total sensor mode

    //AE
    UINT32  (*fpGetExpoSetting)(UINT32 ExpTime, SENSOR_MODE Mode, EXPOSURE_SETTING *ExpTSetting);   ///< depend on exposure time(us), get frame, line, pixel settings
    //#NT#2013/11/04#Jarkko Chang -begin
    //#NT# add iso base setting
    ER    (*fpSetAGC_ISOBase)(BOOL Cal_Status, UINT32 ISOBase);   //set calibration agc gain
    //#NT#2013/11/04#Jarkko Chang -end
    ER    (*fpGetGainSetting)(UINT32 ISO, GAIN_SETTING *Gain);  ///< depend on ISO(50, 100...), get gain settings
    ER    (*fpSetFrame)(UINT32 Frame, UINT32 *VTotal, UINT32 *HTotal);  ///< set frame rate
    ER    (*fpSetLine)(UINT32 Line);    ///< set exposure line
    ER    (*fpSetPixel)(UINT32 Pixel);  ///< set exposure pixel
    ER    (*fpSetGain)(GAIN_SETTING *Gain); ///< set iso gain
    ER    (*fpGRS_Mode)(void);            ///< set sensor exposure mode to global reset
} SENSOR_DRVTAB;

/**
    General Sensor APIs
*/
/**
    open sensor

    @param[in] Id       sensor id
    @param[in] InitObj  sensor initial object
    @param[in] DrvTab   sensor driver table
    @return
        - @b 0 suceess, < 0 failed
 */
extern ER Sensor_Open(SENSOR_ID Id, SENSOR_INIT_OBJ *InitObj, SENSOR_DRVTAB *DrvTab);

/**
    close sensor

    @param[in] Id sensor id
    @return
        - @b 0 suceess, < 0 failed
 */
extern ER Sensor_Close(SENSOR_ID Id);


/**
    Turn on sensor step by step

    @param[in] Id sensor id
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_PwrOn(SENSOR_ID Id);

/**
    Turn off sensor step by step

    @param[in] Id sensor id
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_PwrOff(SENSOR_ID Id);

/**
    Set sensor to standby state

    @param[in] Id sensor id
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_Sleep(SENSOR_ID Id);

/**
    Resume the sensor from the standby

    @param[in] Id sensor id
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_WakeUp(SENSOR_ID Id);


/**
    configure sensor register

    @param[in] Id    sensor id
    @param[in] Cmd   sensor cmd
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_WriteReg(SENSOR_ID Id, SENSOR_CMD *Cmd);

/**
    read sensor register

    @param[in] Id    sensor id
    @param[in] Cmd   sensor cmd
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_ReadReg(SENSOR_ID Id, SENSOR_CMD *Cmd);

/**
    Set the current operation mode

    @param[in] Id    sensor id
    @param[in] Mode  sensor mode
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_ChgMode(SENSOR_ID Id, SENSOR_MODE Mode);

/**
    Get the current mode

    @param[in] Id       sensor id
    @param[out] Mode    sensor mode
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_GetCurMode(SENSOR_ID Id, SENSOR_MODE *Mode);

/**
    Get idicate status of sensor

    @param[in] Id       sensor id
    @param[in] Mode     sensor mode
    @param[out] Info    sensor information
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_GetStatus(SENSOR_ID Id, SENSOR_MODE Mode, SENSOR_INFO *Info);

/**
    Get idicate total sensor mode of sensor

    @param[in] Id    sensor id

    @return
        - @b UINT32: total sensor number
 */
extern UINT32 Sensor_GetModeNum(SENSOR_ID Id);

/**
    Get exposure time settings

    @param[in] Id            sensor id
    @param[in] ExpTime       exposure time(us)
    @param[in] Mode          sensor mode
    @param[out] ExpTSetting  exposure settings
    @return
        - @b UINT32: exposure ratio (HW settings exposure time)/(ExpTime) * 100
 */
extern UINT32 Sensor_GetExpoSetting(SENSOR_ID Id, UINT32 ExpTime, SENSOR_MODE Mode, EXPOSURE_SETTING *ExpTSetting);


/**
    Get iso gain settings

    @param[in] Id       sensor id
    @param[in] Gain    calibration gain
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_SetAGC_ISOBase(SENSOR_ID Id, BOOL Cal_Status, UINT32 ISOBase);

/**
    Get iso gain settings

    @param[in] Id       sensor id
    @param[in] ISO      iso gain
    @param[out] Gain    gain setting
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_GetGainSetting(SENSOR_ID Id, UINT32 ISO, GAIN_SETTING *Gain);

/**
    set exposure time

    @param[in] Id   sensor id
    @param[in] ExpT exposure settings
    @return
        - @b ER: 0 suceess, < 0 failed
*/
extern ER Sensor_SetExpTime(SENSOR_ID Id, EXPOSURE_SETTING *ExpT);

/**
    set sensor gain

    @param[in] Id    sensor id
    @param[in] Gain  gain settings
    @return
        - @b ER: 0 suceess, < 0 failed
*/
extern ER Sensor_SetGain(SENSOR_ID Id, GAIN_SETTING *Gain);

/**
    set sensor fps

    @param[in] SENSOR_ID    sensor id
    @param[in] FPS_SETTING pFPS settings
    @param[in] VTotal
    @param[in] HTotal

    @return
        - @b ER: 0 suceess, < 0 failed
*/
extern ER Sensor_SetFrame(SENSOR_ID Id, UINT32 Frame, UINT32 *VTotal, UINT32 *HTotal);


/**
    set sensor to GRS mode

    @param[in] SENSOR_ID    sensor id

    @return
        - @b ER: 0 suceess, < 0 failed
*/
extern ER Sensor_SetGRS(SENSOR_ID Id);

extern void Sensor_InstallID(void) _SECTION(".kercfg_text");
//@}
#endif
