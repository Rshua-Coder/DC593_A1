
#ifndef UI_PRINTEROBJ_H
#define UI_PRINTEROBJ_H

#include "UIFramework.h"
#include "AppLib.h"
#include "PlaybackTsk.h"


#define FLGDPS_INITOK           0x00200000      // init complete
#define FLGDPS_INITFAIL         0x00400000      // init fail ! (not PTP)


//call back status
#define USIDC_PRINTIMG_OK           0x01
#define USIDC_PRINTIMG_FAIL         0x02
#define USIDC_PRINTIMG_WARNING      0x03
//Add init status into callback function
#define USIDC_INIT_OK               0x05
#define USIDC_INIT_FAIL             0x06
#define USIDC_PRINT_NOTIFYJOB       0x76
#define USIDC_CANCEL_OK             0x90
#define USIDC_CONT_JOB_OK           0x92
#define USIDC_CONT_JOB_FAIL         0x93
#define USIDC_WARNING_PAPER_ERROR   0xB0 //00010000
#define USIDC_ERROR_PAPER_ERROR     0xB1
#define USIDC_WARNING_PAPER_OUT     0xB2 //00010100
#define USIDC_ERROR_PAPER_OUT       0xB3
#define USIDC_WARNING_PAPER_LOAD    0xB4 //00010200
#define USIDC_ERROR_PAPER_LOAD      0xB5
#define USIDC_WARNING_PAPER_JAM     0xB6 //00010300, 00010400, 00010500
#define USIDC_ERROR_PAPER_JAM       0xB7
#define USIDC_WARNING_PAPER_NOMSG   0xB8 //00010600
#define USIDC_ERROR_PAPER_NOMSG     0xB9
#define USIDC_WARNING_PAPER_NOSUP   0xBA //00010700
#define USIDC_ERROR_PAPER_NOSUP     0xBB
#define USIDC_WARNING_INK_ERROR     0xBC //00020000
#define USIDC_ERROR_INK_ERROR       0xBD
#define USIDC_WARNING_INK_OUT       0xBE //00020100
#define USIDC_ERROR_INK_OUT         0xBF
#define USIDC_WARNING_INK_LOW       0xC0 //00020200
#define USIDC_ERROR_INK_LOW         0xC1
#define USIDC_WARNING_PTR_ERROR     0xC2 //00030000, 00030100, 00030200, 00030500
#define USIDC_ERROR_PTR_ERROR       0xC3
#define USIDC_WARNING_PTR_UC        0xC4 //00030300, 00030400
#define USIDC_ERROR_PTR_UC          0xC5
#define USIDC_WARNING_PTR_OPEN      0xC6 //00030600
#define USIDC_ERROR_PTR_OPEN        0xC7
#define USIDC_WARNING_PTR_HEAD      0xC8 //00030700
#define USIDC_ERROR_PTR_HEAD        0xC9
#define USIDC_WRANING_PTR_INK       0xCA //00030800
#define USIDC_ERROR_PTR_INK         0xCB
#define USIDC_WARNING_PTR_CART      0xCC //00030900
#define USIDC_ERROR_PTR_CART        0xCD
#define USIDC_WARNING_FILE_ERROR    0xCE //00040000
#define USIDC_ERROR_FILE_ERROR      0xCF
#define USIDC_CLOSE_SESSION         0xFE

#define USIDC_CANCEL_FAIL           0x96
#define USIDC_PRINTIMG_FATALFAIL    0xFD  //Not execute printing
#define USIDC_PRINTING_NOW          0xFC
#define USIDC_DISCONN_ENABLE        0x94
#define USIDC_CAP_CHANGED           0x95

#define SIDC_UNKNOWN            0
#define SIDC_PTR                1
#define SIDC_PC                 2

#define DPS_CAP_DATEPRINT_DEF       0x01 // default
#define DPS_CAP_DATEPRINT_OFF       0x02 // off
#define DPS_CAP_DATEPRINT_ON        0x04 // on

#define DPS_CAP_PAPERSIZE_DEF       0x00000001 //default
#define DPS_CAP_PAPERSIZE_01L       0x00000002 // L 89mmx127mm
#define DPS_CAP_PAPERSIZE_02L       0x00000004 // 2L 127mmx178mm
#define DPS_CAP_PAPERSIZE_POS       0x00000008 // postcard 100x148mm
#define DPS_CAP_PAPERSIZE_CARD      0x00000010 // card 54x85.6mm
#define DPS_CAP_PAPERSIZE_150       0x00000020 // 100x150mm
#define DPS_CAP_PAPERSIZE_4x6       0x00000040 // 4x6 101x152mm
#define DPS_CAP_PAPERSIZE_8x10      0x00000080 // 8x10 203x254mm
#define DPS_CAP_PAPERSIZE_LET       0x00000100 // letter 216x279mm
#define DPS_CAP_PAPERSIZE_11x17     0x00000200 // 11x17 279x431mm
#define DPS_CAP_PAPERSIZE_A0        0x00000400
#define DPS_CAP_PAPERSIZE_A1        0x00000800
#define DPS_CAP_PAPERSIZE_A2        0x00001000
#define DPS_CAP_PAPERSIZE_A3        0x00002000
#define DPS_CAP_PAPERSIZE_A4        0x00004000
#define DPS_CAP_PAPERSIZE_A5        0x00008000
#define DPS_CAP_PAPERSIZE_A6        0x00010000
#define DPS_CAP_PAPERSIZE_A7        0x00020000
#define DPS_CAP_PAPERSIZE_A8        0x00040000
#define DPS_CAP_PAPERSIZE_A9        0x00080000
#define DPS_CAP_PAPERSIZE_B0        0x00100000
#define DPS_CAP_PAPERSIZE_B1        0x00200000
#define DPS_CAP_PAPERSIZE_B2        0x00400000
#define DPS_CAP_PAPERSIZE_B3        0x00800000
#define DPS_CAP_PAPERSIZE_B4        0x01000000
#define DPS_CAP_PAPERSIZE_B5        0x02000000
#define DPS_CAP_PAPERSIZE_B6        0x04000000
#define DPS_CAP_PAPERSIZE_B7        0x08000000
#define DPS_CAP_PAPERSIZE_B8        0x10000000
#define DPS_CAP_PAPERSIZE_B9        0x20000000
#define DPS_CAP_PAPERSIZE_89RO      0x40000000 //89mm roll
#define DPS_CAP_PAPERSIZE_127RO     0x80000000 //127mm roll
//#define DPS_CAP_PAPERSIZE_100RO     0x00000001 //100mm roll
//#define DPS_CAP_PAPERSIZE_210RO     0x00000002 //210mm roll

#define DPS_CAP_LAYOUT_DEF          0x0001 //default
#define DPS_CAP_LAYOUT_01           0x0002 ///1-up
#define DPS_CAP_LAYOUT_02           0x0004 //2-up
#define DPS_CAP_LAYOUT_03           0x0008 //3-up
#define DPS_CAP_LAYOUT_04           0x0010 //4-up
#define DPS_CAP_LAYOUT_05           0x0020 //5-up
#define DPS_CAP_LAYOUT_06           0x0040 //6-up
#define DPS_CAP_LAYOUT_07           0x0080 //7-up
#define DPS_CAP_LAYOUT_08           0x0100 //8-up
#define DPS_CAP_LAYOUT_09           0x0200 //9-up
#define DPS_CAP_LAYOUT_10           0x0400 //16-up
#define DPS_CAP_LAYOUT_250UP        0x0800 //250-up
#define DPS_CAP_LAYOUT_IDX          0x1000 //index print
#define DPS_CAP_LAYOUT_NOB          0x2000 //borderless

#define DPS_CAP_QUALITY_DEF         0x01 //default
#define DPS_CAP_QUALITY_NOR         0x02 //normal
#define DPS_CAP_QUALITY_DRA         0x04 //draft
#define DPS_CAP_QUALITY_FIN         0x08 //fine

#define DPS_CAP_NAMEPRINT_DEF       0x01
#define DPS_CAP_NAMEPRINT_OFF       0x02
#define DPS_CAP_NAMEPRINT_ON        0x04

#define DPS_CAP_FILETYPES_DEF       0x01
#define DPS_CAP_FILETYPES_EXIFJ     0x02 //exif/jpeg
#define DPS_CAP_FILETYPES_EXIF      0x04 //exif
#define DPS_CAP_FILETYPES_JPEG      0x08 //jpeg
#define DPS_CAP_FILETYPES_JFIF      0x10 // jfif
#define DPS_CAP_FILETYPES_ASSO      0x20 // association
#define DPS_CAP_FILETYPES_DPOF      0x40 // DPOF

typedef enum
{
    NVTEVT_PRINT_MIN    = APP_EVENT_MASK + 0x00000400,
    NVTEVT_PRTINIT,
    NVTEVT_PRTENTERPB,
    NVTEVT_PRTCLOSESESSION,
    NVTEVT_PRTCAPSINIT,
    NVTEVT_PRTRESETDATA,
    NVTEVT_PRTSTARTPRINT,
    NVTEVT_PRTCHEKC_PSIZE,
    NVTEVT_PRTCHEKC_LAYOUT,
    NVTEVT_PRT_ABORT,
    NVTEVT_PRT_CONTINUE,
    NVTEVT_PRINT_MAX 
}
NVT_PRINT_EVENT;

typedef enum
{
    PRT_PBSTATUS,
    PRT_CUR_MODE,
    PRT_ALL_NUM,
    PRT_SINGLE_NUM,
    PRT_SIDC_TYPE,
    PRT_CALLBACK_FUNC,
    PRT_TOTAL_PB_IMG,
    PRT_TOTAL_PRINT_DPOF,
    PRT_TOTAL_PRINT_SINGLE,
    PRT_TOTAL_PRINT_ALL,
    PRT_SINGLE_DATE,
    PRT_ALL_DATE,
    PRT_PAPERSIZE,
    PRT_LAYOUT,
    PRT_QUALITY,
    PRT_NAMEPRINT,
    PRT_CAPINFO_DATE,
    PRT_CAPINFO_PAPERSIZE,
    PRT_CAPINFO_LAYOUT,
    PRT_CAPINFO_QUALITY,
    PRT_CAPINFO_NAMEPRINT,
    PRT_CAPINFO_FILETYPE,
    PRT_CURR_PROGRESS,
    PRT_TOL_PROGRESS,
    PRT_DATA_MAX
}PRT_DATA_SET;

typedef enum
{
    PMODE_SINGLE = 0x00,
    PMODE_ALL,
    PMODE_DPOF,
    PMODE_NUM
}PRT_MODE;

typedef struct _PRT_INITIAL
{
    UINT32 uiBufAddr;
    UINT32 uiBufSize;
}
PRT_INITIAL;

//#NT#2010/09/29#Lily Kao -begin
//#Move the configuration of Picture-Display(PLAY_OBJ) to Project level
typedef void (*PRINTOBJ_SET_PLAYOBJINFO_FUNC)(PLAY_OBJ *);
extern void AppPrint_RegSetPlayObjCB(PRINTOBJ_SET_PLAYOBJINFO_FUNC pFunc);
//#NT#2010/09/29#Lily Kao -end


extern VControl UIPrinterObjCtrl;
extern EVENT_ENTRY UIPrinterObjCtrlCmdMap[];
extern void AppPrint_Init(PRT_INITIAL* pInit);
extern void AppPrint_SetData(PRT_DATA_SET attribute, UINT32 value);
extern UINT32 AppPrint_GetData(PRT_DATA_SET attribute);
#endif
