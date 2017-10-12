#ifndef UI_USBPRINTINFO_H
#define UI_USBPRINTINFO_H

#include "UIInfo.h"

typedef enum
{
    PDATE_OFF = 0x00,
    PDATE_ON,
    PDATE_NUM
}PRT_DATE_SETTING;

typedef enum
{
    PSIZE_DEFAULT = 0x00,
    PSIZE_L,
    PSIZE_2L,
    //PSIZE_POSTCARD,
    //PSIZE_10X15,
    //PSIZE_4X6,
    //PSIZE_A4,
    //PSIZE_8X10,
    //PSIZE_LETTER,
    //PSIZE_11X17,

    PSIZE_NUM
}PRT_SIZE_SETTING;


typedef enum
{
    PLAYOUT_DEFAULT = 0x00,
    PLAYOUT_2UP,
    PLAYOUT_4UP,
    PLAYOUT_INDEX,
    PLAYOUT_BORDERLESS,
    PLAYOUT_NUM
}PRT_TYPE_SETTING;

typedef enum
{
    PQUALITY_DEFAULT = 0x00,
    PQUALITY_NOR,
    PQUALITY_DRA,
    PQUALITY_FIN,
    PQUALITY_NUM
}PRT_QUALITY_SETTING;

typedef enum
{
    PNAMEPRINT_DEFAULT = 0x00,
    PNAMEPRINT_OFF,
    PNAMEPRINT_ON,
    PNAMEPRINT_NUM
}PRT_NAMEPRINT_SETTING;

#if 0
typedef enum
{
    DATEON,
    PAPERSIZE,
    PAPERLAYOUT,
    PRT_INDEX_MAX
}PRT_INDEX;
extern void     UxPrint_SetIndex(PRT_INDEX attribute, UINT32 value);
extern UINT32   UxPrint_GetIndex(PRT_INDEX attribute);
#endif

extern UINT32   gPTRDatePrintTbl[PDATE_NUM];
extern UINT32   gPTRPapaerSizeTbl[PSIZE_NUM];
extern UINT32   gPTRLayoutTbl[PLAYOUT_NUM];
extern UINT32   gPTRQualityTbl[PQUALITY_NUM];
extern UINT32   gPTRNamePrintTbl[PNAMEPRINT_NUM];


extern void     Set_PrinterDate(UINT32 uhIndex);
extern UINT32   Get_PrinterDate(void);

extern void     Set_PrinterSize(UINT32 uhIndex);
extern UINT32   Get_PrinterSize(void);
extern void     Set_PrinterType(UINT32 uhIndex);
extern UINT32   Get_PrinterType(void);
//#NT#2009/12/24#Lily Kao - begin
//remove warnning
extern void Set_PrinterNamePrint(UINT32 uhIndex);
//#NT#2009/12/24#Lily Kao - end

#endif
