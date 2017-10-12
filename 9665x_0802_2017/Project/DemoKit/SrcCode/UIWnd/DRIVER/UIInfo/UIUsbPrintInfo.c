#include <string.h>
#include <stdio.h>
#include "UIUsbPrintInfo.h"
#include "type.h"
#include "DPOF.h"
#include "UIPrinterObj.h"

#if _DEMO_TODO
#include "DPSFuncs.h"
#endif

//static UINT32               uhPrinterNum = 0;
static UINT32               uhPrinterDate = PDATE_OFF;
static UINT32               uhPrinterSize = PSIZE_DEFAULT;
static UINT32               uhPrinterType = PLAYOUT_DEFAULT;
static UINT32               uhPrinterQuality;
static UINT32               uhPrinterNamePrint;


UINT32               gPTRDatePrintTbl[PDATE_NUM] = {DPS_CAP_DATEPRINT_OFF,DPS_CAP_DATEPRINT_ON};
UINT32               gPTRPapaerSizeTbl[PSIZE_NUM] = {DPS_CAP_PAPERSIZE_DEF,
                                                            DPS_CAP_PAPERSIZE_01L,
                                                            DPS_CAP_PAPERSIZE_02L,
                                                            //DPS_CAP_PAPERSIZE_POS,
                                                            //DPS_CAP_PAPERSIZE_150,
                                                            //DPS_CAP_PAPERSIZE_4x6,
                                                            //DPS_CAP_PAPERSIZE_A4,
                                                            //DPS_CAP_PAPERSIZE_8x10,
                                                            //DPS_CAP_PAPERSIZE_LET,
                                                            //DPS_CAP_PAPERSIZE_11x1
                                                            };

UINT32  gPTRLayoutTbl[PLAYOUT_NUM] = {DPS_CAP_LAYOUT_DEF,
                                                DPS_CAP_LAYOUT_02,
                                                DPS_CAP_LAYOUT_04,
                                                DPS_CAP_LAYOUT_IDX,
                                                DPS_CAP_LAYOUT_NOB};

UINT32   gPTRQualityTbl[PQUALITY_NUM] = {DPS_CAP_QUALITY_DEF,
                                                DPS_CAP_QUALITY_NOR,
                                                DPS_CAP_QUALITY_DRA,
                                                DPS_CAP_QUALITY_FIN};

UINT32   gPTRNamePrintTbl[PNAMEPRINT_NUM] = {DPS_CAP_NAMEPRINT_DEF,
                                                DPS_CAP_NAMEPRINT_OFF,
                                                DPS_CAP_NAMEPRINT_ON};
#if 0
void UxPrint_SetIndex(PRT_INDEX attribute, UINT32 value)
{
    switch(attribute)
    {
        case DATEON:
            uhPrinterDate = value;
            break;
        case PAPERSIZE:
            uhPrinterSize = value;
            break;
        case PAPERLAYOUT:
            uhPrinterType = value;
            break;
        default:
            debug_err(("[UxPrt_SetIndex] attribute error!!!"));
            break;
    }
}

UINT32 UxPrint_GetIndex(PRT_INDEX attribute)
{
    switch(attribute)
    {
        case DATEON:
            return uhPrinterDate;
        case PAPERSIZE:
            return uhPrinterSize;
        case PAPERLAYOUT:
            return uhPrinterType;
        default:
            debug_err(("[UxPrt_GetIndex] attribute error!!!"));
            return 0;
    }
}

#endif
void Set_PrinterDate(UINT32 uhIndex)
{
    uhPrinterDate = uhIndex;
    AppPrint_SetData(PRT_ALL_DATE, gPTRDatePrintTbl[uhIndex]);//#NT#2009/12/04#Lily Kao
}

UINT32 Get_PrinterDate(void)
{
    return uhPrinterDate;
}

void Set_PrinterSize(UINT32 uhIndex)
{
    uhPrinterSize = uhIndex;
    AppPrint_SetData(PRT_PAPERSIZE, gPTRPapaerSizeTbl[uhIndex]);
}

UINT32 Get_PrinterSize(void)
{
    return uhPrinterSize;
}

void Set_PrinterType(UINT32 uhIndex)
{
    uhPrinterType = uhIndex;
    AppPrint_SetData(PRT_LAYOUT, gPTRLayoutTbl[uhIndex]);
}

UINT32 Get_PrinterType(void)
{
    return uhPrinterType;
}

void Set_PrinterQuality(UINT32 uhIndex)
{
    uhPrinterQuality = uhIndex;
    AppPrint_SetData(PRT_QUALITY, gPTRQualityTbl[uhIndex]);
}

UINT32 Get_PrinterQuality(void)
{
    return uhPrinterQuality;
}

void Set_PrinterNamePrint(UINT32 uhIndex)
{
    uhPrinterNamePrint = uhIndex;
    AppPrint_SetData(PRT_NAMEPRINT, gPTRNamePrintTbl[uhIndex]);
}

UINT32 Get_PrinterNamePrint(void)
{
    return uhPrinterNamePrint;
}
