/**
    Public header file for IPL command

    This file is the header file that define the API and data type for IPL command.

    @file       IPL_Cmd.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IPL_CMD_H_
#define _IPL_CMD_H_
#include "Type.h"
#include "ErrorNo.h"
#include "IPL_Utility.h"
#include "IPL_CBMSG.h"
#include "IPL_ISREvent.h"
#include "IPL_Hal_ipe_Info.h"

/***********************************************************/
/*                  IPL set command                        */
/***********************************************************/

/** \addtogroup mIIPL */
//@{

/**
    IPL SET Command(IPL_SetCmd())
*/
typedef enum _IPL_SET_CMD
{
    IPL_SET_CHGMODE = 0,       ///< change mode command, data type IPL_CHGMODE_DATA
    IPL_SET_IME,               ///< change IME settings command, data type IPL_IME_DATA
    IPL_SET_DZOOM,             ///< change digital idx command, data type IPL_DZOOM
    IPL_SET_SLEEP,             ///< enter sleep mode command, data type IPL_SLEEP_INFO
    IPL_SET_WAKEUP,            ///< exit sleep mode command, data type IPL_WAKEUP_INFO
    IPL_SET_GET_CAP_RAW,       ///< get cap raw command, data type IPL_GET_CAP_RAW_DATA
    IPL_SET_PAUSEDMA,          ///< pause all IPL dma, for system before change IC frequency
    IPL_SET_RESUMEDMA,         ///< resume all IPL dma, for system after change IC frequency
    IPL_SET_IMGRATIO,          ///< set SIE crop ratio & ime out size, data type IPL_SET_IMGRATIO_DATA
    IPL_SET_VA,                ///< set VA settings command, data type IPL_SET_VA_DATA
    IPL_SET_MAX_CNT_DEF,
    ENUM_DUMMY4WORD(IPL_SET_CMD)
} IPL_SET_CMD;

#define IPL_SET_SWITCH_ALL  0xffffffff
#define IPL_SET_CMD_SWITCH_TAB  32      ///< maxinum value of IPL SET command

//check IPL_SET_CMD range
STATIC_ASSERT((IPL_SET_MAX_CNT_DEF <= IPL_SET_CMD_SWITCH_TAB));


/********* IPL_CMD_CHGMODE *********/
/**
    IPL MODE
*/
typedef enum _IPL_MODE
{
    IPL_MODE_OFF = 0,       ///< IPL off mode.
    IPL_MODE_PREVIEW,       ///< IPL photo mode.
    IPL_MODE_VIDEO,         ///< IPL video mode.
    IPL_MODE_VIDEOREC,      ///< IPL video recording mode.
    IPL_MODE_CAP,           ///< IPL capture mode.
    IPL_MODE_NUM,
    ENUM_DUMMY4WORD(IPL_MODE)
} IPL_MODE;

/**
    IPL post process function, IPL_SET_CHGMODE command
*/
typedef enum _IPL_POST_PROC_FUNC_EN
{
    IPL_FUNC_NONE           = 0x00000000,       ///< disable post process function.
    IPL_FUNC_DISP           = 0x00000001,       ///< enable Display control function.
    IPL_FUNC_VA             = 0x00000002,       ///< allocate va output buffer.
    IPL_FUNC_STAMP          = 0x00000004,       ///< prepare data stamp function & working buffer.
    IPL_FUNC_ALBUM          = 0x00000008,
    IPL_FUNC_DIS            = 0x00000010,       ///< prepare DIS function & working buffer.
    IPL_FUNC_DUALDISP       = 0x00000020,
    IPL_FUNC_PHOTOFRAME     = 0x00000040,       ///< prepare photo frame function & working buffer.

    IPL_FUNC_RSC            = 0x00000080,       ///< prepare rsc function.

    //image effect type(bit20 ~ bit31)
    IPL_FUNC_IMGEFFECT_WDR  = 0x00100000,       ///< Image effect WDR
    ENUM_DUMMY4WORD(IPL_POST_PROC_FUNC_EN)
} IPL_POST_PROC_FUNC_EN;

/**
    IPL SIE crop ratio

    IPL_SET_CHGMODE command(only support stream mode, ex: preview, video, video record mode)
*/
typedef enum _IPL_SENCROPRATIO
{
    IPL_SENCROPRATIO_9_16 = 0,                  ///< 9:16
    IPL_SENCROPRATIO_2_3,                       ///< 2:3
    IPL_SENCROPRATIO_3_4,                       ///< 3:4
    IPL_SENCROPRATIO_1_1,                       ///< 1:1
    IPL_SENCROPRATIO_4_3,                       ///< 4:3
    IPL_SENCROPRATIO_3_2,                       ///< 3:2
    IPL_SENCROPRATIO_16_9,                      ///< 16:9
    IPL_SENCROPRATIO_2_1,                       ///< 2:1
    IPL_SENCROPRATIO_MAX_CNT,
    IPL_SENCROPRATIO_NONE,
    ENUM_DUMMY4WORD(IPL_SENCROPRATIO)
} IPL_SENCROPRATIO;

/**
    IPL change mode parameter

    Data structure for IPL_SET_CHGMODE command operation.
*/
typedef struct _IPL_CHGMODE_DATA
{
    IPL_MODE Mode;                                  ///< next mode

    UINT32 BufAddr;                                 ///< input buffer address
    UINT32 BufSize;                                 ///< input & ouput buffer size(return used buffer size)
    UINT32 SenNum;                                  ///< total sensor number(only valid for off -> preview), must be (< IPL_MAX_SEN_NUM, > 0)

    IPL_SENCROPRATIO CropRatio;                     ///< sie crop ratio

    UINT32 BufSizeH_Disp;                           ///< reserve buffer H size, format reference ImgFmt_Disp
    UINT32 BufSizeV_Disp;                           ///< reserve buffer V size, format reference ImgFmt_Disp
    UINT32 ImgSizeH_Disp;                           ///< display H size, ImgSizeH_Disp x ImgSizeV_Disp must be <= BufSizeH_Disp x BufSizeV_Disp
    UINT32 ImgSizeV_Disp;                           ///< display V size, ImgSizeH_Disp x ImgSizeV_Disp must be <= BufSizeH_Disp x BufSizeV_Disp
    IPL_YUV_IMG_TYPE ImgFmt_Disp;                   ///< display format

    UINT32 ImgP1En;                                 ///< ime p1 enable
    UINT32 BufSizeH_P1;                             ///< reserve buffer H size, format reference ImgFmt_P1
    UINT32 BufSizeV_P1;                             ///< reserve buffer V size, format reference ImgFmt_P1
    UINT32 ImgSizeH_P1;                             ///< ime p1 H size, ImgSizeH_P1 x ImgSizeV_P1 must be <= BufSizeH_P1 x BufSizeV_P1
    UINT32 ImgSizeV_P1;                             ///< ime p1 V size, ImgSizeH_P1 x ImgSizeV_P1 must be <= BufSizeH_P1 x BufSizeV_P1
    IPL_YUV_IMG_TYPE ImgFmt_P1;                     ///< ime p1 format

    UINT32 ImgP3En;                                 ///< ime p3 enable
    UINT32 BufSizeH_P3;                             ///< reserve buffer H size, format reference ImgFmt_P3
    UINT32 BufSizeV_P3;                             ///< reserve buffer V size, format reference ImgFmt_P3
    UINT32 ImgSizeH_P3;                             ///< ime p3 H size, ImgSizeH_P3 x ImgSizeV_P3 must be <= BufSizeH_P3 x BufSizeV_P3
    UINT32 ImgSizeV_P3;                             ///< ime p3 V size, ImgSizeH_P3 x ImgSizeV_P3 must be <= BufSizeH_P3 x BufSizeV_P3
    IPL_YUV_IMG_TYPE ImgFmt_P3;                     ///< ime p3 format

    IPL_CBMSG_FP CB_FP;                             ///< flow callback function
    IPL_ISR_EVENT_FP ISR_CB_FP[IPL_ISR_MAX_CNT];    ///< ISR CB for photo mode

    IPL_POST_PROC_FUNC_EN FuncEn;                   ///< notify IPL post process enable & prepare buffer

    IPL_PROC_ID Id;                                 ///< user can choice IPL_PROC_ID1 or IPL_PROC_ID2, when in capture mode
    UINT32 EventId;                                 ///< only valid when in capture mode, notify IPL current still event.
    UINT32 ProcSenId[IPL_ID_MAX_NUM];             ///< only valid when SenNum = 2, user can assign sensor Id (SIE + CCIR only)
} IPL_CHGMODE_DATA;

/********* IPL_SET_SETDZOOM *********/

/**
    IPL digital zoom parameter

    Data structure for IPL_SET_SETDZOOM command operation.
*/
typedef struct _IPL_DZOOM
{
    IPL_PROC_ID Id;             ///< ipl process id
    UINT32 Idx;                 ///< zoom index
} IPL_DZOOM;

/********* IPL_SET_SLEEP *********/
/**
    IPL sleep command parameter

    Data structure for IPL_SET_SLEEP command operation.
*/
typedef struct _IPL_SLEEP_INFO
{
    IPL_PROC_ID Id;             ///< ipl process id
} IPL_SLEEP_INFO;

/********* IPL_SET_WAKEUP *********/
/**
    IPL WAKEUP command parameter

    Data structure for IPL_SET_WAKEUP command operation.
*/
typedef struct _IPL_WAKEUP_INFO
{
    IPL_PROC_ID Id;             ///< ipl process id
} IPL_WAKEUP_INFO;

/********* IPL_SET_IME *********/

/**
    IPL ime path id
*/
typedef enum _IPL_IME_PATH
{
    IPL_IME_PATH_NONE = 0x00000000,     ///< path null
    IPL_IME_PATH1 = 0x00000001,         ///< path id 1
    IPL_IME_PATH2 = 0x00000002,         ///< path id 2
    IPL_IME_PATH3 = 0x00000004,         ///< path id 3
    ENUM_DUMMY4WORD(IPL_IME_PATH)
} IPL_IME_PATH;

/**
    IPL ime path information
*/
typedef struct _IPL_IME_PATH_INFO
{
    UINT32 OutputEn;            ///<output enable/disable
    UINT32 ImgSizeH;            ///<output image size H
    UINT32 ImgSizeV;            ///<output image size V
    UINT32 ImgSizeLOfs;         ///<output image line offset
    IPL_YUV_IMG_TYPE ImgFmt;    ///<output image format
} IPL_IME_PATH_INFO;

/**
    IPL ime path information
    Data structure for IPL_SET_IME command operation.
*/
typedef struct _IPL_IME_DATA
{
    IPL_PROC_ID Id;             ///< ipl process id
    IPL_IME_PATH PathId;        ///< config path id
    IPL_IME_PATH_INFO ImeP1;    ///< ime path1 config information
    IPL_IME_PATH_INFO ImeP2;    ///< ime path2 config information
    IPL_IME_PATH_INFO ImeP3;    ///< ime path3 config information
} IPL_IME_DATA;

/********* IPL_SET_GET_CAP_RAW *********/
/**
    IPL get raw information
    Data structure for IPL_SET_GET_CAP_RAW command
*/
typedef struct _IPL_GET_CAP_RAW_DATA
{
    IPL_PROC_ID Id;             ///< ipl process id
    UINT32 CompressRatio;       ///< raw compress ratio 0 ~ 100%, 0, 100:disable, 1~99%:Enable
    UINT32 CompressMaxSize;     ///< raw compress maxinum bit stream size
    UINT32 OutCh0En;            ///< sie channel 0 output enable/disable
    UINT32 BufAddrCh0;          ///< sie channel 0 output buffer address
    UINT32 OutCh2En;            ///< sie channel 2 outout enable/disable
    UINT32 BufAddrCh2;          ///< sie channel 2 output buffer address
    UINT32 res[10];
} IPL_GET_CAP_RAW_DATA;

/********* IPL_SET_IMGRATIO *********/

/**
    IPL set image ratio information
    Data structure for IPL_SET_IMGRATIO command
*/
typedef struct _IPL_SET_IMGRATIO_DATA
{
    IPL_PROC_ID Id;                     ///< ipl process id
    IPL_SENCROPRATIO CropRatio;         ///< sie crop ratio
    IPL_IME_PATH PathId;                ///< select ime path
    IPL_IME_PATH_INFO ImeP1;            ///< ime path1 information, only valid when PathId & path1 == 1
    IPL_IME_PATH_INFO ImeP2;            ///< ime path2 information, only valid when PathId & path2 == 1
    IPL_IME_PATH_INFO ImeP3;            ///< ime path3 information, only valid when PathId & path3 == 1
} IPL_SET_IMGRATIO_DATA;

/********* IPL_SET_VA *********/

/**
    IPL set va information
    For IPL_SET_VA command
*/
typedef struct
{
    IPL_PROC_ID Id;                 ///< ipl process id
    UINT32 VAOutEn;                 ///< va output enable/disable
    VA_WIN VAWin16X16;              ///< va 16 x 16 window information
    IPE__VA_IND VAWinInd;           ///< va independent window
    IPE__VA VASettings;             ///< va acc, det settings
} IPL_SET_VA_DATA;

/********* IPL_SET_CMD *********/
typedef union
{
    IPL_CHGMODE_DATA ChgMode;
    IPL_GET_CAP_RAW_DATA GetRaw;
    IPL_IME_DATA ChgImeSet;
    IPL_DZOOM DZoom;
    IPL_SLEEP_INFO SleepInfo;
    IPL_WAKEUP_INFO WakeInfo;
    IPL_SET_IMGRATIO_DATA CropInfo;
    IPL_SET_VA_DATA VaInfo;
} IPL_SET_CMD_DATA;

/**
     IPL SET command

     @note only one command can be executed each time

     @param[in] Cmd       ipl command (IPL_SET_CMD and user define command)
     @param[in] Data      ipl command data

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_SetCmd(UINT32 Cmd, void *Data);

/**
     wait ipl SET command finish

     @note only valid for SET command
*/
extern void IPL_WaitCmdFinish(void);
/***********************************************************/
/*                  IPL get command                        */
/***********************************************************/

/**
    IPL GET Command
*/
typedef enum _IPL_GET_CMD
{
    IPL_GET_CUR_MODE = 0,               ///< get current mode command, data type IPL_MODE_INFOR
    IPL_GET_CUR_BUF_INFOR,              ///< get current buffer information, data type IPL_BUF_INFOR
    IPL_GET_IME_INFOR,                  ///< get IME information, data IPL_IME_INFOR
    IPL_GET_IME_CUR_BUF_ADDR,           ///< get IME current output Address, data type IPL_IME_BUF_ADDR
    IPL_GET_SIE_CUR_CAP_CH0_RAW_INFOR,  ///< get SIE channel 0 current output size, data type IPL_SIE_RAW_INFO
    IPL_GET_SIE_CUR_CAP_CH2_RAW_INFOR,  ///< get SIE channel 2 current output size, data type IPL_SIE_RAW_INFO
    IPL_GET_SIE_CAP_MAX_RAW_INFOR,      ///< get SIE channel 0 max output size, data type IPL_SIE_RAW_INFO
    IPL_GET_CAP_INFOR,                  ///< get capture information data type IPL_CAP_INFOR
    IPL_GET_CAP_ENC_RST_INFOR,          ///< get image raw encode information, data type IPL_ENC_RST_INFOR
    IPL_GET_CUR_DZ_INFOR,               ///< get current digital zoom information, data type IPL_DZOOM_INFOR
    IPL_GET_CUR_CA_INFOR,               ///< get current image CA information, data type IPL_CA_INFOR
    IPL_GET_CA_INFOR,                   ///< get CA setting information, data type IPL_CA_SET_INFOR
    IPL_GET_CUR_VA_INFOR,               ///< get current image VA information data type IPL_VA_INFOR
    IPL_GET_PRE_CUR_RAW_INFOR,          ///< get current pre raw information data type IPL_PRE_RAW_INFO
    IPL_GET_SIE_CUR_CAP_CCIR_RAW_INFOR, ///< get SIE channel 0 current output size, data type IPL_SIE_RAW_INFO
    IPL_GET_MAX_CNT_DEF,
    ENUM_DUMMY4WORD(IPL_GET_CMD)
} IPL_GET_CMD;

#define IPL_GET_SWITCH_ALL  0xffffffff
#define IPL_GET_CMD_SWITCH_TAB  32      ///< maxinum value of IPL GET command

//check IPL_GET_CMD range
STATIC_ASSERT((IPL_GET_MAX_CNT_DEF <= IPL_GET_CMD_SWITCH_TAB));

/********* IPL_VA_INFOR *********/
/**
    IPL va window id
*/
typedef enum _VA_WINIDX
{
    VA_WIN_16X16 = 0,               ///< va 16 * 16 window
    VA_WIN_INDP0,                   ///< va independent window 0
    VA_WIN_INDP1,                   ///< va independent window 1
    VA_WIN_INDP2,                   ///< va independent window 2
    ENUM_DUMMY4WORD(VA_WINIDX)
} VA_WINIDX;

/**
    IPL current va information
*/
typedef struct _IPL_VA_INFOR
{
    UINT32 *Grp0;                   ///< va group 0 output pointer
    UINT32 *Grp1H;                  ///< va group 1H output pointer
    UINT32 *Grp1V;                  ///< va group 1V output pointer
    UINT32 *Grp2H;                  ///< va group 2H output pointer
    UINT32 *Grp2V;                  ///< va group 2V output pointer
    UINT32 Length;                  ///< va output length
    VA_WINIDX VAWin;                ///< va output window
    VA_OUTSEL VAoutputFmt;          ///< va output format
    UINT32 frameCnt;
} IPL_VA_INFOR;

/********* IPL_CA_INFOR *********/
/**
    IPL current ca information
*/
typedef struct _IPL_CA_INFOR
{
    IPL_PROC_ID Id;             ///< input parameter, ipl process id
    UINT16 *R;                  ///< output parameter, need assign memory
    UINT16 *G;                  ///< output parameter, need assign memory
    UINT16 *B;                  ///< output parameter, need assign memory
    UINT32 Length;              ///< input array length for *R, *G, *B
} IPL_CA_INFOR;

/********* IPL_CA_SET_INFOR *********/

/**
    IPL ca setting information
*/
typedef struct _IPL_CA_SET_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    UINT32 WinXNum;             ///<output parameter, the number of the horizontal window
    UINT32 WinYNum;             ///<output parameter, the number of the vertical window
    UINT32 SieCropSizeH;        ///<output parameter, sie crop H size
    UINT32 SieCropSizeV;        ///<output parameter, sie corp V size
} IPL_CA_SET_INFOR;

/********* IPL_GET_CUR_DZ_INFOR *********/

/**
    IPL current digital zoom information
*/
typedef struct _IPL_DZOOM_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    UINT32 CurIdx;              ///<output parameter, digital zoom current index
    UINT32 MaxIdx;              ///<output parameter, digital zoom maxinum index
    UINT32 Ratio;               ///<output parameter, digital zoom ratio
} IPL_DZOOM_INFOR;

/********* IPL_GET_CAP_ENC_RST_INFOR *********/

/**
    IPL rde information(only valid when in capture mode)
*/
typedef struct _IPL_ENC_RST_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    UINT32 BSOutputSize;        ///<output parameter, bit stream size
} IPL_ENC_RST_INFOR;

/********* IPL_CAP_INFOR *********/

/**
    IPL capture information(only valid when in capture mode)
*/
typedef struct _IPL_CAP_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    UINT32 SIECh2SubRatio;      ///<output parameter, sie channel2 output ratio(%)
    UINT32 DFEnable;            ///<output parameter, dark frame function enable/disable
    UINT32 PRIVEnable;          ///<output parameter, pri v function enable/disable
    UINT32 PRIVSubRatio;        ///<output parameter, pri v output ratio(%)
    UINT32 PRICCEnable;         ///<output parameter, pri cc function enable/disable
    UINT32 PRICCSubRatio;       ///<output parameter, pri cc output ratio(%)
    UINT32 QVVEnable;           ///<output parameter, qv v function enable/disable
    UINT32 QVVSubRatio;         ///<output parameter, qv v output ratio(%)
    UINT32 QVCCEnable;          ///<output parameter, qv cc function enable/disable
    UINT32 QVCCSubRatio;        ///<output parameter, qv cc output ratio(%)
    UINT32 CCIRFlowEnable;      ///<only for 96650 CCIR flow
} IPL_CAP_INFOR;


/********* IPL_GET_CUR_MODE *********/

/**
    IPL current mode information
*/
typedef struct _IPL_MODE_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    IPL_MODE Mode;              ///<output parameter, current mode
} IPL_MODE_INFOR;

/********* IPL_GET_CUR_BUF_INFO *********/

/**
    IPL current buffer information
*/
typedef struct _IPL_BUF_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    UINT32 BufAddr;             ///<output parameter, current ipl buffer address
    UINT32 BufSize;             ///<output parameter, current ipl buffer size
} IPL_BUF_INFOR;

/********* IPL_GET_IME_INFOR *********/

/**
    IPL ime information
*/
typedef struct _IPL_IME_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    UINT32 P1Num;               ///<output parameter, path1 buffer number
    UINT32 P1Enable;            ///<output parameter, path1 enable/disable
    IPL_YCC_IMG_INFO ImeP1[10];  ///<output parameter, path1 image setting(format, buffer address, size)
    UINT32 P2Num;               ///<output parameter, path2 buffer number
    UINT32 P2Enable;            ///<output parameter, path2 enable/disable
    IPL_YCC_IMG_INFO ImeP2[10];  ///<output parameter, path2 image setting(format, buffer address, size)
    UINT32 P3Num;               ///<output parameter, path3 buffer number
    UINT32 P3Enable;            ///<output parameter, path3 enable/disable
    IPL_YCC_IMG_INFO ImeP3[10];  ///<output parameter, path3 image setting(format, buffer address, size)
} IPL_IME_INFOR;

/********* IPL_GET_IME_CUR_BUF_ADDR *********/

/**
    IPL current ime output information
*/
typedef struct _IPL_IME_BUF_ADDR
{
    IPL_PROC_ID Id;                 ///<input parameter, ipl process id
    UINT32 P1Enable;                ///<output parameter, path1 enable/disable
    IPL_YCC_IMG_INFO ImeP1;         ///<output parameter, current path1 image setting(format, buffer address, size)
    UINT32 P2Enable;                ///<output parameter, path2 enable/disable
    IPL_YCC_IMG_INFO ImeP2;         ///<output parameter, current path2 image setting(format, buffer address, size)
    UINT32 P3Enable;                ///<output parameter, path3 enable/disable
    IPL_YCC_IMG_INFO ImeP3;         ///<output parameter, current path3 image setting(format, buffer address, size)
} IPL_IME_BUF_ADDR;

/********* IPL_SIE_RAW_SIZE *********/

/**
    IPL raw information
*/
typedef struct _IPL_SIE_RAW_INFO
{
    IPL_PROC_ID Id;                 ///<input parameter, ipl process id
    UINT32 ImgWidth;                ///<output parameter, raw width
    UINT32 ImgHeight;               ///<output parameter, raw height
    UINT32 ImgLineOfs;              ///<output parameter, raw line offset
    IPL_RAW_BIT ImgBit;             ///<output parameter, raw bit
    IPL_RAW_PIX ImgPix;             ///<output parameter, raw start pixel
} IPL_SIE_RAW_INFO;

/********* IPL_SIE_RAW_SIZE *********/

/**
    IPL raw information
*/
typedef IPL_SIE_RAW_INFO IPL_PRE_RAW_INFO;

/********* IPL_GET_CMD *********/
typedef union
{
    IPL_MODE_INFOR ModeInfo;
    IPL_BUF_INFOR BufInfo;
    IPL_IME_INFOR ImeInfo;
    IPL_IME_BUF_ADDR ImeBufInfo;
    IPL_SIE_RAW_INFO RawInfo;
    IPL_CAP_INFOR CapInfo;
    IPL_ENC_RST_INFOR EncRstInfo;
    IPL_DZOOM_INFOR DZInfo;
    IPL_CA_INFOR CaInfo;
    IPL_CA_SET_INFOR CaSetInfo;
    IPL_VA_INFOR VaInfo;
    IPL_PRE_RAW_INFO PreRawInfo;
} IPL_GET_CMD_DATA;

/**
     Get IPL command

     @param[in] Cmd       ipl command (IPL_GET_CMD)
     @param[in,out] Data  ipl command data

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_GetCmd(IPL_GET_CMD Cmd, void *Data);

/**
     IPL open
     initial ipl module

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_Open(void);

/**
     IPL close
     un-initial ipl module

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_Close(void);

/**
     check IPL module is open or not

     @return Description of data returned.
         - @b TRUE: ipl opened
         - @b FALSE: ipl closed
*/
extern BOOL IPL_IsOpen(void);

/**
     install IPL control id
*/
extern void Install_IPLCtrlID(void) _SECTION(".kercfg_text");

/**
     install IPL extend id
*/
extern void IPL_InstallID(void) _SECTION(".kercfg_text");

/**
     IPL_DumpDataStatus
     dump ipl status

     @return Description of data returned.
         - @UINT32 data number
*/
extern UINT32 IPL_DumpDataStatus(UINT32* pData);

//@}
#endif //_IPL_CMD_H_