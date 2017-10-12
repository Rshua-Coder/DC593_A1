/**
    UI Interface of sample Calibration.

    Processing MSG & Display on LCD.

    @file            IPL_Cal_KeyStrSample.c
    @brief          Processing MSG & Display
    @ingroup    Calibration
    @note          UI Layer
    @author      Randy Ho
    @Date

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/
#include    "stdio.h"

#include    "Type.h"
#include    "GxDC.h"
#include    "GxGfx.h"
#include    "GxVideo.h"
#include    "GxInput.h"
#include    "KeyDef.h"
#include    "Delay.h"

#include    "IPL_Calibration.h"

#include    "UIDisplay.h"
#include    "UIGraphics.h"
#include    "UIResource.h"
#include    "DxInput.h"
#include    "IPL_CalApi.h"
#include    "UICommon.h"

#define __MODULE__ CALKEYSTRSAMPLE
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

/**
    show OSD Information (Color/Display Size/...)
*/
// Buffer
#define MaxLine 10
#define MaxStrWidth 100
char CalShowStringBuf[MaxLine][MaxStrWidth];//max (10 line x width 100)

// String Color
static BVALUE gStrColor = CLRID_IDX_TRANSPART;

// Display Background Color
static BVALUE gDispBgColor = CLRID_IDX_TRANSPART;

// Display Size
static UINT32 gDispSize_w = 320;
static UINT32 gDispSize_h = 240;

// String Size
static UINT32 gStrSize_w = 240;
static UINT32 gStrSize_h = 30;

// Record all color information each line for re-write OSD after set background but not clear
static UINT32 StrColor_Last[MaxLine] = {0};
static UINT32 DispBgColor_Last = 0;

/**
    Set Color Information
*/
// Set String Color
static void Cal_SetStrColor(UINT32 StrColor)
{
    // String Color
    switch(StrColor)
    {
        case UICOL_NONE:
            gStrColor = CLRID_IDX_TRANSPART;
        break;

        case UICOL_RED:
            gStrColor = CLRID_IDX_RED;
        break;

        case UICOL_GREEN:
            gStrColor = CLRID_IDX_GREEN;
        break;

        case UICOL_BLUE:
            gStrColor = CLRID_IDX_BLUE;
        break;

        case UICOL_WHITE:
            gStrColor = CLRID_IDX_WHITE;
        break;

        case UICOL_BLACK:
            gStrColor = CLRID_IDX_BLACK;
        break;

        case UICOL_GRAY:
            gStrColor = CLRID_IDX_BLACK50;
        break;

        case UICOL_YELLOW:
            gStrColor = CLRID_IDX_YELLOW;
        break;

        default:
            gStrColor = CLRID_IDX_TRANSPART;
        break;
    }
}

// Set Display Background Color
static void Cal_SetDispBgColor(UINT32 DispBgColor)
{
    switch(DispBgColor)
    {
        case UICOL_NONE:
            gDispBgColor = DispBgColor_Last;
        break;

        case UICOL_RED:
            gDispBgColor = CLRID_IDX_RED;
        break;

        case UICOL_GREEN:
            gDispBgColor = CLRID_IDX_GREEN;
        break;

        case UICOL_BLUE:
            gDispBgColor = CLRID_IDX_BLUE;
        break;

        case UICOL_WHITE:
            gDispBgColor = CLRID_IDX_WHITE;
        break;

        case UICOL_BLACK:
            gDispBgColor = CLRID_IDX_BLACK;
        break;

        case UICOL_GRAY:
            gDispBgColor = CLRID_IDX_BLACK50;
        break;

        case UICOL_YELLOW:
            gDispBgColor = CLRID_IDX_YELLOW;
        break;

        default:
            gDispBgColor = CLRID_IDX_TRANSPART;
        break;
    }
}

// Get Display Size
static void Cal_GetDispSize(void)
{
    ISIZE disp_size;
    disp_size = GxVideo_GetDeviceSize(DOUT1);

    gDispSize_w = disp_size.w;
    gDispSize_h = disp_size.h;
}

// Get String Size
static void Cal_GetStrSize(void)
{
    gStrSize_w = gDispSize_w;
    gStrSize_h = 30;
}

static void Cal_ShowStr(CHAR *pStr, PURECT pRect)
{
    DC**     pCalDCList;
    UI_SetDisplayDirty(TRUE); //set TRUE to force dirty current begin/end
    pCalDCList = (DC**)UI_BeginScreen();

    //clr OSD in the specified position
    GxGfx_SetTextColor(DispBgColor_Last, DispBgColor_Last, 0);
    GxGfx_FillRect(pCalDCList[GxGfx_OSD],pRect->x,pRect->y,pRect->x+pRect->w,pRect->y+pRect->h);

    // set OSD background in the specified position (same as display background)
    GxGfx_SetShapeColor(DispBgColor_Last, DispBgColor_Last, 0);
    GxGfx_FillRect(pCalDCList[GxGfx_OSD],pRect->x,pRect->y,pRect->x+pRect->w,pRect->y+pRect->h);

    // show OSD
    GxGfx_SetTextStroke((const FONT*)gDemoKit_Font, FONTSTYLE_NORMAL, SCALE_1X);
    GxGfx_SetTextColor(gStrColor, gStrColor, 0); // set string color
    GxGfx_Text(pCalDCList[GxGfx_OSD], pRect->x, pRect->y, pStr);

    UI_EndScreen((UINT32)pCalDCList);
}

static void Cal_ClrOSD(void)
{
    //UI_ClearOSD(gDispBgColor);
    DC** pDCList;
    pDCList = (DC**)UI_BeginScreen();
    GxGfx_SetShapeColor(0, gDispBgColor, 0);
    GxGfx_FillRect(pDCList[GxGfx_OSD], 0, 0, pDCList[GxGfx_OSD]->size.w, pDCList[GxGfx_OSD]->size.h);
    UI_EndScreen((UINT32)pDCList);

    DispBgColor_Last = gDispBgColor;
}

/*static void Cal_DisplayString(CHAR *pStr, UINT16 X, UINT16 Y)
{
    if (CalApi()->fpGetCalUsbMode())
    {
        URECT tmpRect = {0, 0, 320, 240};
        tmpRect.y = Y;
        tmpRect.x = X;
        CalUSB_DrawString(pStr, &tmpRect, RGB_COLOR(0, 255, 0), TRUE);
    }
    else
    {
        Cal_ShowStringWithColor(pStr, X, Y, 3);
    }

    return;
}*/

static void Cal_WaitKey(UINT32 Key)
{
#if 1
    Input_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_NULL);
    Delay_DelayMs(100);
    while(1)
    {
        if(GxKey_GetData(GXKEY_NORMAL_KEY) & Key)
            break;
        else
            Delay_DelayMs(33);
    }
    //incase of ui to detect key pressed when key pressed too long.
    //but key still can't be pressed over 2 sec.
    Delay_DelayMs(2000);
    Input_SetKeyMask(KEY_PRESS, FLGKEY_UP|FLGKEY_DOWN|FLGKEY_SHUTTER2);
#else
    if (CalApi()->fpGetCalUsbMode())
    {
        CalUSB_WaitKey();
    }
    else
    {
        do
        {
            Delay_DelayMs(10);
        }while(DrvKey_DetNormalKey()!=Key);
    }
#endif
    return;
}

static void UICB_Common(CalUICB UICB,CHAR* pstr, UINT32 Color)
{
    static UINT32 StartAddr = (UINT32)(&CalShowStringBuf[0][0]);
    static UINT32 Addr = (UINT32)(&CalShowStringBuf[0][0]);
    //static UINT32 buf_cnt = 0;
    static UINT32 lineCnt = 0;
    static URECT StrRect = {0, 0, 320, 30};

    // Color =  DispBgColor*10 + StrColor
    UINT32 StrColor = Color%10;
    UINT32 DispBgColor = Color/10;

    Cal_GetDispSize();
    Cal_GetStrSize();//need to after Cal_GetDispSize (need to reference display width)
    Cal_SetStrColor(StrColor);
    Cal_SetDispBgColor(DispBgColor);
    StrRect.w = gStrSize_w;
    StrRect.h = gStrSize_h;

    static UINT32 shift_lineCnt = 0; // if UICB_START , first line for start
    switch(UICB)
    {
        case UICB_START:
            DBG_IND("==UICB_START==\r\n");
            if(pstr)// Printed on the first line
            {
                Addr = StartAddr;
                sprintf((char *)Addr, pstr);
                StrRect.x = 0;
                StrRect.y = 0;
                Cal_ShowStr(&CalShowStringBuf[0][0], &StrRect);

                // shift first line for start
                static BOOL bFirstStart = TRUE;
                if (bFirstStart)
                {
                    shift_lineCnt = 1;
                    bFirstStart = FALSE;
                }

                // record string and string background color
                StrColor_Last[0] = StrColor;
            }
            break;

        case UICB_WAITKEY:
            DBG_IND("==UICB_WAITKEY==\r\n");
            // show "PRESS OK KEY to continue"
            Addr = StartAddr + sizeof(CalShowStringBuf[0])*(lineCnt+shift_lineCnt);
            sprintf((char *)Addr, "PRESS OK KEY to continue");
            StrRect.x = 0;
            StrRect.y = gStrSize_h*(lineCnt+shift_lineCnt);
            Cal_ShowStr(&CalShowStringBuf[lineCnt+shift_lineCnt][0], &StrRect);

            // wait key
            Cal_WaitKey(FLGKEY_KEY_MASK);

            // clear "PRESS OK KEY to continue"
            sprintf((char *)Addr, " ");
            StrRect.x = 0;
            StrRect.y = gStrSize_h*(lineCnt+shift_lineCnt);
            Cal_ShowStr(&CalShowStringBuf[lineCnt+shift_lineCnt][0], &StrRect);
            break;

        case UICB_PROCESSING:
            DBG_IND("==UICB_PROCESSING==\r\n");
            if(pstr)
            {
                Addr = StartAddr + sizeof(CalShowStringBuf[0])*(lineCnt+shift_lineCnt);
                sprintf((char *)Addr, pstr);
                StrRect.x = 0;
                StrRect.y = gStrSize_h*(lineCnt+shift_lineCnt);
                Cal_ShowStr(&CalShowStringBuf[lineCnt+shift_lineCnt][0], &StrRect);
                lineCnt++;
            }
            else // wait any key to continue
            {
                Cal_WaitKey(FLGKEY_KEY_MASK);
            }
            break;

        case UICB_CLEAN:
            DBG_IND("==UICB_CLEAN==\r\n");
            Cal_ClrOSD();
            lineCnt = 0;
            //shift_lineCnt = 0;
            break;

        case UICB_DELETE:
            DBG_IND("==UICB_DELETE==\r\n");
            Addr = StartAddr + sizeof(CalShowStringBuf[0])*(lineCnt+shift_lineCnt);
            sprintf((char *)Addr, " ");
            StrRect.x = 0;
            StrRect.y = gStrSize_h*(lineCnt+shift_lineCnt);
            Cal_ShowStr(&CalShowStringBuf[lineCnt+shift_lineCnt][0], &StrRect);
            lineCnt--;
            break;

        case UICB_END:
            DBG_IND("==UICB_END==\r\n");
            Cal_ClrOSD();
            if(pstr)
            {
                sprintf((char *)Addr, pstr);
                StrRect.x = 110;
                StrRect.y = 110;
                Cal_ShowStr(pstr,&StrRect);
            }
            lineCnt = 0;
            //shift_lineCnt = 0;
            break;

        case UICB_NONE:
        default:
            break;
    }

    if (((lineCnt+shift_lineCnt)<0)||(lineCnt+shift_lineCnt>(gDispSize_h/gStrSize_h)))
    {
        DBG_WRN("lineCnt out of range: lineCnt %d, shift_lineCnt %d", lineCnt, shift_lineCnt);
        lineCnt = 0;
    }
    return;
}

static void UICB_DP_W(CalUICB UICB,CHAR* pstr, UINT32 Color)
{
    UICB_Common(UICB, pstr, Color);

    return;
}

static void UICB_DP_D(CalUICB UICB,CHAR* pstr, UINT32 Color)
{
    UICB_Common(UICB, pstr, Color);

    return;
}

static void UICB_ECS(CalUICB UICB,CHAR* pstr, UINT32 Color)
{
    UICB_Common(UICB, pstr, Color);

    return;
}

static void UICB_ISO(CalUICB UICB,CHAR* pstr, UINT32 Color)
{
    UICB_Common(UICB, pstr, Color);

    return;
}

static void UICB_AWB(CalUICB UICB,CHAR* pstr, UINT32 Color)
{
    UICB_Common(UICB, pstr, Color);

    return;
}

CAL_UICB_OBJ Cal_UI =
{
    UICB_AWB,
    UICB_ISO,
    UICB_ECS,
    NULL,
    UICB_DP_W,
    UICB_DP_D,
};

pCAL_UICB_OBJ CAL_UICB(void)
{
    return &Cal_UI;

}
