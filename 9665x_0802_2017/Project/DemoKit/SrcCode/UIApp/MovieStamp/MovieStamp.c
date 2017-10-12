#include <stdio.h>
#include <string.h>
#include "kernel.h"
#include "SysCfg.h"
#include "Utility.h"
#include "HwClock.h"
#include "GxImage.h"
#include "MovieInterface.h"
#include "MovieStamp.h"
#include "DateStampFont10x16.h"
#include "DateStampFont12x20.h"
#include "DateStampFont18x30.h"
#include "DateStampFont20x44.h"
#include "DateStampFont26x44.h"
#include "IQS_Utility.h"
#include "EngineerMode.h"
#include "ldws_alg.h"

#define __MODULE__          MovieStamp
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

#define CHECK_STAMP_TIME    DISABLE
#define CHECK_ENC_INFO      DISABLE

#define COLOR_BG_Y          0x00
#define COLOR_BG_U          0x80
#define COLOR_BG_V          0x80
#define COLOR_FR_Y          0x00
#define COLOR_FR_U          0x80
#define COLOR_FR_V          0x80
#define COLOR_FG_Y          RGB_GET_Y(250, 160, 10)
#define COLOR_FG_U          RGB_GET_U(250, 160, 10)
#define COLOR_FG_V          RGB_GET_V(250, 160, 10)
#define COLOR_ID_BG         0
#define COLOR_ID_FR         1
#define COLOR_ID_FG         2
#define COLORKEY_BG_YUV     0x00000000  // stamp BG data for color key operation
#define COLORKEY_YUV        0x00010101  // color key for transparent background

#define STAMP_WIDTH_TOLERANCE   8   // total font width error tolerance
#define STAMP_LOGO_GAP          8   // date stamp and water logo position gap

//#NT#2016/08/08#Lincy Lin -begin
//#NT#Support HTTP liveview datastamp function
#define VIDEO_IN_MAX        3   // max 3 video paths
#define MOVIE_STAMP_MAX_LEN     32

//variable declare
static STAMP_POS    g_MovieStampPos[VIDEO_IN_MAX] =
{
    {0, 0}, // stamp position for primary image 1
    {0, 0}, // stamp position for primary image 2
    {0, 0}  // stamp position for HTTP+MJPG streaming
};
static char         g_cMovieStampStr[VIDEO_IN_MAX][MOVIE_STAMP_MAX_LEN];
static UINT32       g_uiMovieStampSetup[VIDEO_IN_MAX] = {STAMP_OFF, STAMP_OFF, STAMP_OFF};
static STAMP_INFO   g_MovieStampInfo[VIDEO_IN_MAX];
static struct tm    g_CurDateTime[VIDEO_IN_MAX];
static UINT32       g_uiMovieStampYAddr[VIDEO_IN_MAX] = {0};
//static UINT32       g_uiMovieStampHeightMax = 44; // maximum height of date stamp font (gDateStampFont26x44)
static BOOL         g_bStampColorSetup[VIDEO_IN_MAX] = {FALSE, FALSE, FALSE};
static UINT32       g_uiMovieSrcYAddr[VIDEO_IN_MAX] = {0};
static WATERLOGO_BUFFER g_MovieWaterInfo[VIDEO_IN_MAX] = {0};
static UINT32       g_uiWaterLogoYAddr[VIDEO_IN_MAX] = {0}, g_uiWaterLogoUVAddr[VIDEO_IN_MAX] = {0};
//#NT#2016/08/08#Lincy Lin -end

void MovieStamp_Setup(UINT32 uiVidEncId, UINT32 uiFlag, UINT32 uiImageWidth, UINT32 uiImageHeight, WATERLOGO_BUFFER *pWaterLogoBuf)
{
    PSTAMP_INFO     pStampInfo;
    ICON_DB const   *pDB;
    UINT32          uiIconID;
    UINT32          uiStrOffset;
	UINT32 			i;

    g_uiMovieStampSetup[uiVidEncId] = uiFlag;

    if ((uiFlag & STAMP_SWITCH_MASK) == STAMP_OFF)
    {
        return;
    }

	for (i = 0; i < VIDEO_IN_MAX; i++)
		g_uiMovieSrcYAddr[i] = 0;

    #if (WATERLOGO_FUNCTION == ENABLE)
    {
        if ((uiFlag & STAMP_LOGO_MASK) == STAMP_LOGO_ON)
        {
            // setup water logo
            memcpy((void *)&g_MovieWaterInfo[uiVidEncId], (const void *)pWaterLogoBuf, sizeof(WATERLOGO_BUFFER));
            g_MovieWaterInfo[uiVidEncId].uiWaterLogoYAddr = g_uiWaterLogoYAddr[uiVidEncId];
            g_MovieWaterInfo[uiVidEncId].uiWaterLogoUVAddr = g_uiWaterLogoUVAddr[uiVidEncId];
        }
    }
    #endif

    pStampInfo = &g_MovieStampInfo[uiVidEncId];
    pStampInfo->pi8Str = &g_cMovieStampStr[uiVidEncId][0];

    // set date stamp font data base
    switch (uiImageWidth)
    {
    case 1920:  // 1920x1080
    case 1536:  // 1536x1536
        if (IsAELog() || IsADASLog())
            pStampInfo->pDataBase = &gDateStampFont12x20;
        else
        pStampInfo->pDataBase = &gDateStampFont26x44;
        break;

    case 1728:  // 1728x1296 (DAR 16:9)
    case 1440:  // 1440x1080 (DAR 16:9)
        pStampInfo->pDataBase = &gDateStampFont20x44;
        g_MovieWaterInfo[uiVidEncId].uiWidth = (g_MovieWaterInfo[uiVidEncId].uiWidth * 3) / 4;
        break;

    case 1280:  // 1280x720
        pStampInfo->pDataBase = &gDateStampFont18x30;
        g_MovieWaterInfo[uiVidEncId].uiWidth = (g_MovieWaterInfo[uiVidEncId].uiWidth * 30) / 44;
        g_MovieWaterInfo[uiVidEncId].uiHeight = (g_MovieWaterInfo[uiVidEncId].uiHeight * 30) / 44;
        break;

    case 320:   // QVGA
        pStampInfo->pDataBase = &gDateStampFont10x16;
        g_MovieWaterInfo[uiVidEncId].uiWidth = (g_MovieWaterInfo[uiVidEncId].uiWidth * 16) / 44;
        g_MovieWaterInfo[uiVidEncId].uiHeight = (g_MovieWaterInfo[uiVidEncId].uiHeight * 16) / 44;
        break;

    default:    // VGA & others
        pStampInfo->pDataBase = &gDateStampFont12x20;
        g_MovieWaterInfo[uiVidEncId].uiWidth = (g_MovieWaterInfo[uiVidEncId].uiWidth * 20) / 44;
        g_MovieWaterInfo[uiVidEncId].uiHeight = (g_MovieWaterInfo[uiVidEncId].uiHeight * 20) / 44;
        break;
    }

    #if (WATERLOGO_FUNCTION == ENABLE)
    {
        if ((uiFlag & STAMP_LOGO_MASK) == STAMP_LOGO_ON)
        {
            // do water logo scaling
            UINT32      uiLineOffset[2] = {0};
            UINT32      uiPxlAddr[2] = {0};
            UINT32      uiScaleYAddr, uiScaleUVAddr;
            IMG_BUF     SrcImg, DstImg;
            IRECT       SrcRegion = {0};
            IRECT       DstRegion = {0};
            BOOL        bScale = TRUE;

            g_MovieWaterInfo[uiVidEncId].uiWidth = ALIGN_CEIL_4(g_MovieWaterInfo[uiVidEncId].uiWidth);
            g_MovieWaterInfo[uiVidEncId].uiHeight = ALIGN_CEIL_4(g_MovieWaterInfo[uiVidEncId].uiHeight);

            uiScaleYAddr = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYAddr;
            uiScaleUVAddr = g_MovieWaterInfo[uiVidEncId].uiWaterLogoUVAddr;

            if (bScale)
            {
                // Init source buffer
                uiLineOffset[0] = pWaterLogoBuf->uiWaterLogoYLineOffset;
                uiLineOffset[1] = pWaterLogoBuf->uiWaterLogoYLineOffset;
                uiPxlAddr[0]    = pWaterLogoBuf->uiWaterLogoYAddr;
                uiPxlAddr[1]    = pWaterLogoBuf->uiWaterLogoUVAddr;

                GxImg_InitBufEx(&SrcImg,
                                pWaterLogoBuf->uiWidth,
                                pWaterLogoBuf->uiHeight,
                                GX_IMAGE_PIXEL_FMT_YUV422_PACKED,
                                uiLineOffset,
                                uiPxlAddr);

                // Init destination buffer
                uiLineOffset[0] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
                uiLineOffset[1] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
                uiPxlAddr[0]    = uiScaleYAddr;
                uiPxlAddr[1]    = uiScaleUVAddr;

                GxImg_InitBufEx(&DstImg,
                                g_MovieWaterInfo[uiVidEncId].uiWidth,
                                g_MovieWaterInfo[uiVidEncId].uiHeight,
                                GX_IMAGE_PIXEL_FMT_YUV422_PACKED,
                                uiLineOffset,
                                uiPxlAddr);

                // Set scale region
                SrcRegion.x = 0;
                SrcRegion.y = 0;
                SrcRegion.w = pWaterLogoBuf->uiWidth;
                SrcRegion.h = pWaterLogoBuf->uiHeight;
                //DBG_DUMP("^YLogo Src: x %d, y %d, w %d, h %d\r\n", SrcRegion.x, SrcRegion.y, SrcRegion.w, SrcRegion.h);

                DstRegion.x = 0;
                DstRegion.y = 0;
                DstRegion.w = g_MovieWaterInfo[uiVidEncId].uiWidth;
                DstRegion.h = g_MovieWaterInfo[uiVidEncId].uiHeight;
                //DBG_DUMP("^YLogo Dst: x %d, y %d, w %d, h %d\r\n", DstRegion.x, DstRegion.y, DstRegion.w, DstRegion.h);

                // Scale image by ISE
                GxImg_ScaleData(&SrcImg, &SrcRegion, &DstImg, &DstRegion);
            }
        }
    }
    #endif

    // set stamp string (for calculating stamp position)
    switch (uiFlag & STAMP_DATE_TIME_MASK)
    {
    case STAMP_DATE_TIME_AMPM:
        snprintf(pStampInfo->pi8Str, MOVIE_STAMP_MAX_LEN, "0000/00/00 00:00:00 AM");
        break;

    case STAMP_DATE: // date only is not suitable for movie stamp (it's suitable for still image stamp)
    case STAMP_TIME:
        snprintf(pStampInfo->pi8Str, MOVIE_STAMP_MAX_LEN, "00:00:00");
        break;

    case STAMP_DATE_TIME:
    default:
        snprintf(pStampInfo->pi8Str, MOVIE_STAMP_MAX_LEN, "0000/00/00 00:00:00");
        break;
    }

    // set font width and height (use the width of 1st font, so the total width may have some error)
    pDB = pStampInfo->pDataBase;
    uiStrOffset = pDB->uiDrawStrOffset;
    uiIconID = pStampInfo->pi8Str[0] - uiStrOffset; // 1st font
    pStampInfo->ui32FontWidth  = pDB->pIconHeader[uiIconID].uiWidth;
    pStampInfo->ui32FontHeight = pDB->pIconHeader[uiIconID].uiHeight;
    pStampInfo->ui32DstHeight  = pStampInfo->ui32FontHeight; // no scaling

    // Set date stamp position
    if ((uiFlag & STAMP_OPERATION_MASK) == STAMP_AUTO)
    {
        //UINT32  uiImageWidth = GetMovieSizeWidth(uiMovieIndex);
        //UINT32  uiImageHeight = GetMovieSizeHeight(uiMovieIndex);
        UINT32  uiStampWidth = (pStampInfo->ui32DstHeight * pStampInfo->ui32FontWidth) / pStampInfo->ui32FontHeight;

        switch (uiFlag & STAMP_POSITION_MASK)
        {
        case STAMP_TOP_LEFT:
            if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos[uiVidEncId].uiX = 0;
                g_MovieStampPos[uiVidEncId].uiY = 0;
            }
            else
            {
                #if 0
                g_MovieStampPos[uiVidEncId].uiX = uiStampWidth * 2; // 2 fonts width gap
                g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight; // 1 font height gap
                #else
                g_MovieStampPos[uiVidEncId].uiX = uiStampWidth; // 1 font width gap
                g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight / 2; // 1/2 font height gap
                #endif
            }
            #if (WATERLOGO_FUNCTION == ENABLE)
            if ((uiFlag & STAMP_LOGO_MASK) == STAMP_LOGO_ON)
            {
                if (g_MovieWaterInfo[uiVidEncId].uiXPos == WATERLOGO_AUTO_POS && g_MovieWaterInfo[uiVidEncId].uiYPos == WATERLOGO_AUTO_POS)
                {
                    g_MovieWaterInfo[uiVidEncId].uiXPos = g_MovieStampPos[uiVidEncId].uiX;
                    g_MovieWaterInfo[uiVidEncId].uiYPos = g_MovieStampPos[uiVidEncId].uiY;
                    g_MovieStampPos[uiVidEncId].uiX += (g_MovieWaterInfo[uiVidEncId].uiWidth + STAMP_LOGO_GAP);
                }
            }
            #endif
            break;

        case STAMP_TOP_RIGHT:
            if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * strlen(pStampInfo->pi8Str) - STAMP_WIDTH_TOLERANCE;
                g_MovieStampPos[uiVidEncId].uiY = 0;
            }
            else
            {
                #if 0
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 2); // 2 fonts width gap
                g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight; // 1 font height gap
                #else
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 1); // 1 font width gap
                g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight / 2; // 1/2 font height gap
                #endif
            }
            #if (WATERLOGO_FUNCTION == ENABLE)
            if ((uiFlag & STAMP_LOGO_MASK) == STAMP_LOGO_ON)
            {
                if (g_MovieWaterInfo[uiVidEncId].uiXPos == WATERLOGO_AUTO_POS && g_MovieWaterInfo[uiVidEncId].uiYPos == WATERLOGO_AUTO_POS)
                {
                    g_MovieWaterInfo[uiVidEncId].uiXPos = g_MovieStampPos[uiVidEncId].uiX - g_MovieWaterInfo[uiVidEncId].uiWidth - STAMP_LOGO_GAP;
                    g_MovieWaterInfo[uiVidEncId].uiYPos = g_MovieStampPos[uiVidEncId].uiY;
                }
            }
            #endif
            break;

        case STAMP_BOTTOM_LEFT:
            if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos[uiVidEncId].uiX = 0;
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight;
            }
            else
            {
                #if 0
                g_MovieStampPos[uiVidEncId].uiX = uiStampWidth * 2; // 2 fonts width gap
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight * 2; // 1 font height gap
                #else
                g_MovieStampPos[uiVidEncId].uiX = uiStampWidth; // 1 font width gap
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - (pStampInfo->ui32DstHeight * 3) / 2; // 1/2 font height gap
                #endif
            }
            #if (WATERLOGO_FUNCTION == ENABLE)
            if ((uiFlag & STAMP_LOGO_MASK) == STAMP_LOGO_ON)
            {
                if (g_MovieWaterInfo[uiVidEncId].uiXPos == WATERLOGO_AUTO_POS && g_MovieWaterInfo[uiVidEncId].uiYPos == WATERLOGO_AUTO_POS)
                {
                    g_MovieWaterInfo[uiVidEncId].uiXPos = g_MovieStampPos[uiVidEncId].uiX;
                    g_MovieWaterInfo[uiVidEncId].uiYPos = g_MovieStampPos[uiVidEncId].uiY + pStampInfo->ui32DstHeight - g_MovieWaterInfo[uiVidEncId].uiHeight;
                    g_MovieStampPos[uiVidEncId].uiX += (g_MovieWaterInfo[uiVidEncId].uiWidth + STAMP_LOGO_GAP);
                }
            }
            #endif
            break;

        case STAMP_BOTTOM_RIGHT:
        default:
            if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * strlen(pStampInfo->pi8Str) - STAMP_WIDTH_TOLERANCE;
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight;
            }
            else
            {
                #if 0
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 2); // 2 fonts width gap
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight * 2; // 1 font height gap
                #else
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 1); // 1 font width gap
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - (pStampInfo->ui32DstHeight * 3) / 2; // 1/2 font height gap
                #endif
            }
            #if (WATERLOGO_FUNCTION == ENABLE)
            if ((uiFlag & STAMP_LOGO_MASK) == STAMP_LOGO_ON)
            {
                if (g_MovieWaterInfo[uiVidEncId].uiXPos == WATERLOGO_AUTO_POS && g_MovieWaterInfo[uiVidEncId].uiYPos == WATERLOGO_AUTO_POS)
                {
                    g_MovieWaterInfo[uiVidEncId].uiXPos = g_MovieStampPos[uiVidEncId].uiX - g_MovieWaterInfo[uiVidEncId].uiWidth - STAMP_LOGO_GAP;
                    g_MovieWaterInfo[uiVidEncId].uiYPos = g_MovieStampPos[uiVidEncId].uiY + pStampInfo->ui32DstHeight - g_MovieWaterInfo[uiVidEncId].uiHeight;
                }
            }
            #endif
            break;
        }
    }

    g_MovieStampPos[uiVidEncId].uiX = ALIGN_FLOOR_4(g_MovieStampPos[uiVidEncId].uiX);
    g_MovieStampPos[uiVidEncId].uiY = ALIGN_FLOOR_4(g_MovieStampPos[uiVidEncId].uiY);

    #if (WATERLOGO_FUNCTION == ENABLE)
    if ((uiFlag & STAMP_LOGO_MASK) == STAMP_LOGO_ON)
    {
        g_MovieWaterInfo[uiVidEncId].uiXPos = ALIGN_FLOOR_4(g_MovieWaterInfo[uiVidEncId].uiXPos);
        g_MovieWaterInfo[uiVidEncId].uiYPos = ALIGN_FLOOR_4(g_MovieWaterInfo[uiVidEncId].uiYPos);
    }
    #endif

    // set default stamp color if necessary
    if (g_bStampColorSetup[uiVidEncId] == FALSE)
    {
        // Stamp background color
        pStampInfo->Color[COLOR_ID_BG].ucY = COLOR_BG_Y;
        pStampInfo->Color[COLOR_ID_BG].ucU = COLOR_BG_U;
        pStampInfo->Color[COLOR_ID_BG].ucV = COLOR_BG_V;

        // Stamp frame color
        pStampInfo->Color[COLOR_ID_FR].ucY = COLOR_FR_Y;
        pStampInfo->Color[COLOR_ID_FR].ucU = COLOR_FR_U;
        pStampInfo->Color[COLOR_ID_FR].ucV = COLOR_FR_V;

        // Stamp foreground color (text body)
        pStampInfo->Color[COLOR_ID_FG].ucY = COLOR_FG_Y;
        pStampInfo->Color[COLOR_ID_FG].ucU = COLOR_FG_U;
        pStampInfo->Color[COLOR_ID_FG].ucV = COLOR_FG_V;
    }

    // Reset reference time
    g_CurDateTime[uiVidEncId].tm_sec = 61;
}

void MovieStamp_SetPos(UINT32 uiVidEncId, UINT32 x, UINT32 y)
{
    g_MovieStampPos[uiVidEncId].uiX = x;
    g_MovieStampPos[uiVidEncId].uiY = y;
}

void MovieStamp_SetColor(UINT32 uiVidEncId, PSTAMP_COLOR pStampColorBg, PSTAMP_COLOR pStampColorFr, PSTAMP_COLOR pStampColorFg)
{
    g_bStampColorSetup[uiVidEncId] = TRUE;

    // Stamp background color
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucY = pStampColorBg->ucY;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucU = pStampColorBg->ucU;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucV = pStampColorBg->ucV;

    // Stamp frame color
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucY = pStampColorFr->ucY;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucU = pStampColorFr->ucU;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucV = pStampColorFr->ucV;

    // Stamp foreground color (text body)
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucY = pStampColorFg->ucY;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucU = pStampColorFg->ucU;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucV = pStampColorFg->ucV;
}

#if 0
UINT32 MovieStamp_GetMaxFontHeight(void)
{
    return g_uiMovieStampHeightMax;
}
#endif

static BOOL MovieStamp_DrawIcon420UV(PSTAMP_INFO pStampInfo, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
{
    ICON_HEADER pIconHeader;
    ICON_DB const *pDB;
    UINT8   const *uiIconData;
    UINT8   *pChar, *pCharU, *pCharV;
    UINT32  uiIconWidth;
    UINT32  uiIconHeight;
    UINT32  uiPixelCount;
    UINT32  i, j;
    UINT32  uiIndex;

    pDB = pStampInfo->pDataBase;
    pIconHeader = pDB->pIconHeader[uiIconID];
    uiIconWidth = pIconHeader.uiWidth;
    uiIconHeight = pIconHeader.uiHeight;
    uiIconData = (pDB->pIconData) + (pIconHeader.uiOffset);

    uiPixelCount = 0;

    for(i = 0; i < uiIconHeight; i++)
    {
        pChar = (UINT8 *)uiFBY;
        pCharU = (UINT8 *)uiFBCb;
        pCharV = (UINT8 *)uiFBCb+1;

        for(j = 0; j < uiIconWidth; j ++)
        {
            uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

            if (uiIndex)
            {
                if (uiIndex == 1) // frame color
                {
                    *pChar = pStampInfo->Color[COLOR_ID_FR].ucY;
                }
                else // foreground color
                {
                    *pChar = pStampInfo->Color[COLOR_ID_FG].ucY;
                }

                if (((j&1)==0)&&((i&1)==0))
                {
                    if (uiIndex == 1) // frame color
                    {
                        *pCharU = pStampInfo->Color[COLOR_ID_FR].ucU;
                        *pCharV = pStampInfo->Color[COLOR_ID_FR].ucV;
                    }
                    else // foreground color
                    {
                        *pCharU = pStampInfo->Color[COLOR_ID_FG].ucU;
                        *pCharV = pStampInfo->Color[COLOR_ID_FG].ucV;
                    }
                }
            }
            pChar++;
            if ((j&1)==0)
            {
                pCharU+=2;
                pCharV+=2;
            }
            uiPixelCount ++;
            if (uiPixelCount % 4 == 0)
                uiIconData ++;

        }
        uiFBY += uiLineOffset;
        if  ((i&1) == 0)
        {
            uiFBCb += uiLineOffset;
            uiFBCr += uiLineOffset;
        }
    }
    return TRUE;
}

static BOOL MovieStamp_DrawIcon422UV(PSTAMP_INFO pStampInfo, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
{
    ICON_HEADER pIconHeader;
    ICON_DB const *pDB;
    UINT8   const *uiIconData;
    UINT8   *pChar, *pCharU, *pCharV;
    UINT32  uiIconWidth;
    UINT32  uiIconHeight;
    UINT32  uiPixelCount;
    UINT32  i, j;
    UINT32  uiIndex;

    pDB = pStampInfo->pDataBase;
    pIconHeader = pDB->pIconHeader[uiIconID];
    uiIconWidth = pIconHeader.uiWidth;
    uiIconHeight = pIconHeader.uiHeight;
    uiIconData = (pDB->pIconData) + (pIconHeader.uiOffset);

    uiPixelCount = 0;

    for(i = 0; i < uiIconHeight; i++)
    {
        pChar = (UINT8 *)uiFBY;
        pCharU = (UINT8 *)uiFBCb;
        pCharV = (UINT8 *)uiFBCb+1;

        for(j = 0; j < uiIconWidth; j ++)
        {
            uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

            if (uiIndex)
            {
                if (uiIndex == 1) // frame color
                {
                    *pChar = pStampInfo->Color[COLOR_ID_FR].ucY;
                }
                else // foreground color
                {
                    *pChar = pStampInfo->Color[COLOR_ID_FG].ucY;
                }

                //if (((j&1)==0)&&((i&1)==0))
                if ((j&1)==0)
                {
                    if (uiIndex == 1) // frame color
                    {
                        *pCharU = pStampInfo->Color[COLOR_ID_FR].ucU;
                        *pCharV = pStampInfo->Color[COLOR_ID_FR].ucV;
                    }
                    else // foreground color
                    {
                        *pCharU = pStampInfo->Color[COLOR_ID_FG].ucU;
                        *pCharV = pStampInfo->Color[COLOR_ID_FG].ucV;
                    }
                }
            }
            pChar++;
            if ((j&1)==0)
            {
                pCharU+=2;
                pCharV+=2;
            }
            uiPixelCount ++;
            if (uiPixelCount % 4 == 0)
                uiIconData ++;

        }
        uiFBY += uiLineOffset;
        //if  ((i&1) == 0)
        {
            uiFBCb += uiLineOffset;
            uiFBCr += uiLineOffset;
        }
    }
    return TRUE;
}

static UINT32 MovieStamp_GetStampDataWidth(PSTAMP_INFO pStampInfo)
{
    ICON_DB const *pDB;
    UINT32  i, j;
    UINT32  uiStrLen;
    UINT32  uiDataWidth;
    UINT32  uiIconID;
    UINT32  uiStrOffset;

    uiStrLen = strlen(pStampInfo->pi8Str);

    pDB = pStampInfo->pDataBase;
    uiStrOffset = pDB->uiDrawStrOffset;

    uiDataWidth = 0;
    for (i = 0; i < uiStrLen; i++)
    {
        //get icon database
        uiIconID = pStampInfo->pi8Str[i] - uiStrOffset;
        j = pDB->pIconHeader[uiIconID].uiWidth;
        uiDataWidth += j;

        if (j % 2 != 0)
        {
            j++;
        }
    }

    return uiDataWidth;
}

static UINT32 MovieStamp_DrawString(UINT32 uiVidEncId, PSTAMP_INFO pStampInfo, PSTAMP_BUFFER pStampBuffer)
{
    ICON_DB const *pDB;
    UINT32  i, j;
    UINT32  uiStrLen;
    UINT32  uiDataWidth;
    UINT32  uiIconID;
    UINT32  uiStrOffset;
    UINT32  uiFBAddrY, uiFBAddrCb, uiFBAddrCr;

    uiStrLen = strlen(pStampInfo->pi8Str);

    pDB = pStampInfo->pDataBase;
    uiStrOffset = pDB->uiDrawStrOffset;

    uiFBAddrY  = pStampBuffer->uiYAddr;
    uiFBAddrCb = pStampBuffer->uiUAddr;
    uiFBAddrCr = pStampBuffer->uiVAddr;

    uiDataWidth = 0;
    for (i = 0; i < uiStrLen; i++)
    {
        //get icon database
        uiIconID = pStampInfo->pi8Str[i] - uiStrOffset;
        j = pDB->pIconHeader[uiIconID].uiWidth;
        uiDataWidth += j;

        if (j % 2 != 0)
        {
            j++;
        }

        if (pStampBuffer->uiFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
            MovieStamp_DrawIcon420UV(pStampInfo, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pStampBuffer->uiYLineOffset);
        else
            MovieStamp_DrawIcon422UV(pStampInfo, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pStampBuffer->uiYLineOffset);
        uiFBAddrY  += (j);
        uiFBAddrCb += (j);
        uiFBAddrCr += (j);
    }

    return uiDataWidth;
}

void MovieStamp_SetDataAddr(UINT32 uiVidEncId, UINT32 uiAddr)
{
    g_uiWaterLogoYAddr[uiVidEncId]  = uiAddr;
    g_uiWaterLogoUVAddr[uiVidEncId] = uiAddr + WATERLOGO_BUF_SIZE/2; // YUV422, UV-packed
    g_uiMovieStampYAddr[uiVidEncId] = uiAddr + WATERLOGO_BUF_SIZE;
}

#if (CHECK_ENC_INFO == ENABLE)
extern void MediaRec_GetEncInfo(UINT32 puiParam[7]);
#endif
void MovieStamp_CopyData(UINT32 uiVidEncId, UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth)
{
    GX_IMAGE_PIXEL_FMT GxImgFormat;
    STAMP_BUFFER    StampBuffer;
    struct tm       CurDateTime;
    IMG_BUF         SrcImg, DstImg;
    UINT32          x, y;
    UINT32          uiYAddrOffset, uiUAddrOffset;
    static UINT32   uiStampDataWidth[VIDEO_IN_MAX] = {0};
    UINT32          uiPreWidth, uiWidth;
    UINT32          uiStampDataHeight;
    UINT32          uiBgData;
    UINT32          uiLineOffset[2] = {0};
    UINT32          uiPxlAddr[2] = {0};
    #if (CHECK_STAMP_TIME == ENABLE)
    UINT32          uiTime; // for performance measurement
    #endif

    if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_SWITCH_MASK) == STAMP_OFF)
    {
        return;
    }

    if (yAddr == 0 || cbAddr == 0)
    {
        return;
    }

    // To avoid different stamp overlap for same source image
    {
        UINT32 frame_rate = 0, rec_format = 0, video_id = uiVidEncId;
        MovRec_GetInfo(MOVREC_GETINFO_VIDEO_FPS, &frame_rate, &video_id, 0);
        MovRec_GetInfo(MOVREC_GETINFO_RECFORMAT, &rec_format, 0, 0);

        if ((g_uiMovieSrcYAddr[uiVidEncId] == yAddr) &&
            (rec_format != MEDIAREC_TIMELAPSE) &&
            (rec_format != MEDIAREC_MERGEYUV) &&
            (frame_rate >= 15))
        {
            // if Y address of current encode path is the same as previous one (i.e. the same source image),
            // do not apply data stamp otherwise the stamp data will overlap
            return;
        }
        g_uiMovieSrcYAddr[uiVidEncId] = yAddr;
    }

    switch (g_uiMovieStampSetup[uiVidEncId] & STAMP_IMG_FORMAT_MASK)
    {
    case STAMP_IMG_420UV:
        GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
        break;

    case STAMP_IMG_422UV:
        GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
        break;

    default:
        DBG_ERR("Only support 420/422 UV pack!\r\n");
        return;
    }

    x = g_MovieStampPos[uiVidEncId].uiX;
    y = g_MovieStampPos[uiVidEncId].uiY;

    CurDateTime = HwClock_GetCurrentTime();

    // Y address offset of destination image to put Y stamp data
    uiYAddrOffset = lineY * y + x;
    // UV address offset of destination image to put UV stamp data
    if (GxImgFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
    {
        uiUAddrOffset = lineY * y / 2 + x;
    }
    else
    {
        uiUAddrOffset = lineY * y + x;
    }

    uiStampDataHeight = g_MovieStampInfo[uiVidEncId].ui32DstHeight;

    StampBuffer.uiYLineOffset   = imgWidth;
    StampBuffer.uiUVLineOffset  = imgWidth;
    StampBuffer.uiYAddr         = g_uiMovieStampYAddr[uiVidEncId];
    StampBuffer.uiUAddr         = StampBuffer.uiYAddr + StampBuffer.uiYLineOffset * uiStampDataHeight;
    StampBuffer.uiVAddr         = StampBuffer.uiUAddr; // for 420 UV pack
    StampBuffer.uiFormat        = GxImgFormat;

    #if (CHECK_STAMP_TIME == ENABLE)
    Perf_Open(); // for performance measurement
    #endif

    #if (CHECK_ENC_INFO == DISABLE)
    if ((g_CurDateTime[uiVidEncId].tm_sec != CurDateTime.tm_sec) || IsAELog())
    #endif
    {
        g_CurDateTime[uiVidEncId].tm_sec = CurDateTime.tm_sec;

        if (!IsAELog() && !IsADASLog())
        {
            // Prepare date-time string
            if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_DATE_TIME_MASK) == STAMP_DATE_TIME)
            {
                switch (g_uiMovieStampSetup[uiVidEncId] & STAMP_DATE_FORMAT_MASK)
                {
                case STAMP_DD_MM_YY:
		    		snprintf(&g_cMovieStampStr[uiVidEncId][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d",CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
                case STAMP_MM_DD_YY:
                    snprintf(&g_cMovieStampStr[uiVidEncId][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d",CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
                default:
                    snprintf(&g_cMovieStampStr[uiVidEncId][0], MOVIE_STAMP_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d",CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
                }
            }
            else if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_DATE_TIME_MASK) == STAMP_DATE_TIME_AMPM)
            {
                if (CurDateTime.tm_hour >= 12)
                {
                    if (CurDateTime.tm_hour > 12)
                        CurDateTime.tm_hour -= 12;

                    switch (g_uiMovieStampSetup[uiVidEncId] & STAMP_DATE_FORMAT_MASK)
                    {
                    case STAMP_DD_MM_YY:
                        snprintf(&g_cMovieStampStr[uiVidEncId][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    case STAMP_MM_DD_YY:
                        snprintf(&g_cMovieStampStr[uiVidEncId][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    default:
                        snprintf(&g_cMovieStampStr[uiVidEncId][0], MOVIE_STAMP_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d PM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    }
                }
                else
                {
                    switch (g_uiMovieStampSetup[uiVidEncId] & STAMP_DATE_FORMAT_MASK)
                    {
                    case STAMP_DD_MM_YY:
                        snprintf(&g_cMovieStampStr[uiVidEncId][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    case STAMP_MM_DD_YY:
                        snprintf(&g_cMovieStampStr[uiVidEncId][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    default:
                        snprintf(&g_cMovieStampStr[uiVidEncId][0], MOVIE_STAMP_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d AM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    }
                }
            }
            else
            {
                snprintf(&g_cMovieStampStr[uiVidEncId][0], MOVIE_STAMP_MAX_LEN, "%02d:%02d:%02d", CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
            }
        }

        if (IsAELog())
        {
            ////<< spark add AE function
            IQS_GetLogString(&g_cMovieStampStr[uiVidEncId][0], MOVIE_STAMP_MAX_LEN);
        }

        #if (CHECK_ENC_INFO == ENABLE)
        {
            UINT32 uiParam[7];

            MediaRec_GetEncInfo(uiParam);
            snprintf(&g_cMovieStampStr[uiVidEncId][0], MOVIE_STAMP_MAX_LEN, "%d %d %d %d / %d %d %d",
                uiParam[0], uiParam[1], uiParam[2], uiParam[3], uiParam[4], uiParam[5], uiParam[6]);
        }
        #endif

        uiPreWidth = uiStampDataWidth[uiVidEncId]; // previous stamp data width
        uiStampDataWidth[uiVidEncId] = MovieStamp_GetStampDataWidth(&g_MovieStampInfo[uiVidEncId]);
        uiWidth = (uiPreWidth > uiStampDataWidth[uiVidEncId]) ? uiPreWidth : uiStampDataWidth[uiVidEncId];

        // Set background data
        #if (CHECK_STAMP_TIME == ENABLE)
        uiTime = Perf_GetCurrent();
        #endif
        // Clear source (stamp) buffer
        uiLineOffset[0] = StampBuffer.uiYLineOffset;
        uiLineOffset[1] = StampBuffer.uiUVLineOffset;
        uiPxlAddr[0]    = StampBuffer.uiYAddr;
        uiPxlAddr[1]    = StampBuffer.uiUAddr;

        GxImg_InitBufEx(&SrcImg,
                        uiWidth,
                        uiStampDataHeight,
                        GxImgFormat,
                        uiLineOffset,
                        uiPxlAddr);

        if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
        {
            //uiBgData = COLORKEY_BG_YUV; // for LE op
            uiBgData = COLORKEY_YUV; // for EQ op
        }
        else
        {
            uiBgData =  ((UINT32)g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucV << 16) |
                        ((UINT32)g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucU << 8)  |
                        g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucY;
        }

        GxImg_FillData(&SrcImg, REGION_MATCH_IMG, uiBgData);
        #if (CHECK_STAMP_TIME == ENABLE)
        DBG_DUMP("time1: %d us\r\n", Perf_GetCurrent() - uiTime);
        #endif

        // Draw string by getting data from date stamp font
        #if (CHECK_STAMP_TIME == ENABLE)
        uiTime = Perf_GetCurrent();
        #endif
        MovieStamp_DrawString(uiVidEncId, &g_MovieStampInfo[uiVidEncId], &StampBuffer);
        #if (CHECK_STAMP_TIME == ENABLE)
        DBG_DUMP("time2: %d us\r\n", Perf_GetCurrent() - uiTime);
        #endif
    }

    // copy data from date stamp draw buffer to image buffer
    #if (CHECK_STAMP_TIME == ENABLE)
    uiTime = Perf_GetCurrent();
    #endif
    // Init source (stamp) buffer
    uiLineOffset[0] = StampBuffer.uiYLineOffset;
    uiLineOffset[1] = StampBuffer.uiUVLineOffset;
    uiPxlAddr[0]    = StampBuffer.uiYAddr;
    uiPxlAddr[1]    = StampBuffer.uiUAddr;

    GxImg_InitBufEx(&SrcImg,
                    uiStampDataWidth[uiVidEncId],
                    uiStampDataHeight,
                    GxImgFormat,
                    uiLineOffset,
                    uiPxlAddr);

    // Init destination (image) buffer
    uiLineOffset[0] = lineY;
    uiLineOffset[1] = lineY;
    uiPxlAddr[0]    = yAddr  + uiYAddrOffset;
    uiPxlAddr[1]    = cbAddr + uiUAddrOffset;

    GxImg_InitBufEx(&DstImg,
                    imgWidth,
                    240, // don't care, but should be > 2
                    GxImgFormat,
                    uiLineOffset,
                    uiPxlAddr);

    if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
    {
        GxImg_CopyColorkeyData(&DstImg, REGION_MATCH_IMG, &SrcImg, REGION_MATCH_IMG, COLORKEY_YUV, FALSE);
    }
    else
    {
        GxImg_CopyData(&SrcImg, REGION_MATCH_IMG, &DstImg, REGION_MATCH_IMG);
    }

    #if (WATERLOGO_FUNCTION == ENABLE)
    {
        if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_LOGO_MASK) == STAMP_LOGO_ON)
        {
            UINT32      x2, y2, uiYAddrOffset2, uiUAddrOffset2;
            IMG_BUF     SrcImg2, DstImg2;

            x2 = g_MovieWaterInfo[uiVidEncId].uiXPos;
            y2 = g_MovieWaterInfo[uiVidEncId].uiYPos;

            uiYAddrOffset2 = lineY * y2 + x2;
            if (GxImgFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
            {
                uiUAddrOffset2 = lineY * y2 / 2 + x2;
            }
            else
            {
                uiUAddrOffset2 = lineY * y2 + x2;
            }

            // Init source (watermark) buffer
            uiLineOffset[0] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
            if (GxImgFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
            {
                uiLineOffset[1] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset * 2; // *2 for YUV422 => YUV420
            }
            else // GX_IMAGE_PIXEL_FMT_YUV422_PACKED
            {
                uiLineOffset[1] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
            }
            uiPxlAddr[0]    = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYAddr;
            uiPxlAddr[1]    = g_MovieWaterInfo[uiVidEncId].uiWaterLogoUVAddr;

            GxImg_InitBufEx(&SrcImg2,
                            g_MovieWaterInfo[uiVidEncId].uiWidth,
                            g_MovieWaterInfo[uiVidEncId].uiHeight,
                            GxImgFormat,
                            uiLineOffset,
                            uiPxlAddr);

            // Init destination (image) buffer
            uiLineOffset[0] = lineY;
            uiLineOffset[1] = lineY;
            uiPxlAddr[0]    = yAddr  + uiYAddrOffset2;
            uiPxlAddr[1]    = cbAddr + uiUAddrOffset2;

            GxImg_InitBufEx(&DstImg2,
                            imgWidth,
                            240, // don't care, but should be > 2
                            GxImgFormat,
                            uiLineOffset,
                            uiPxlAddr);

            if (g_MovieWaterInfo[uiVidEncId].uiWaterLogoColorKey == WATERLOGO_FILL_COLOR)
                GxImg_CopyData(&SrcImg2, REGION_MATCH_IMG, &DstImg2, REGION_MATCH_IMG);
            else
                GxImg_CopyColorkeyData(&DstImg2, REGION_MATCH_IMG, &SrcImg2, REGION_MATCH_IMG, g_MovieWaterInfo[uiVidEncId].uiWaterLogoColorKey, FALSE);
        }
    }
    #endif

//#NT#2015/06/05#KS Hung -begin
//#NT#Add the LDWS drawing
#if _LDWS_FUNC_
    if(uiVidEncId == 1) // Only stamp on the live view stream while recording.
    {
        extern UINT32 MovieLine[6][20][5];
        //extern UINT32 MovieLineIdx;
        UINT32 iML;
        UINT8 ColorKey[5][3] = { {RGB_GET_Y(  0, 128,   0), RGB_GET_U(  0, 128,   0), RGB_GET_V(  0, 128,   0)},
                                 {RGB_GET_Y(128,   0,   0), RGB_GET_U(128,   0,   0), RGB_GET_V(128,   0,   0)},   //lane
                                 {RGB_GET_Y(  0, 250,   0), RGB_GET_U(  0, 250,   0), RGB_GET_V(  0, 250,   0)},   //sky & car
                                 {RGB_GET_Y(250, 250,   0), RGB_GET_U(250, 250,   0), RGB_GET_V(250, 250,   0)},   //roi
                                 {RGB_GET_Y(  0, 128,   0), RGB_GET_U(  0, 128,   0), RGB_GET_V(  0, 128,   0)},}; //
        #if 0
        iML = (MovieLineIdx + 1) % 6;
        #else
        iML = (uiVidEncId % 2); //MovieLineIdx;
        #endif
        int i=0;
        for (i=0; i<20; i++ )
        {
            if ( MovieLine[iML][i][4] == 0 )
                continue;
            int x, y;
            int x1 = MovieLine[iML][i][0];
            int y1 = MovieLine[iML][i][1];
            int x2 = MovieLine[iML][i][2];
            int y2 = MovieLine[iML][i][3];
            UINT32 KeyY = ColorKey[MovieLine[iML][i][4]][0];
            UINT32 KeyCb = ColorKey[MovieLine[iML][i][4]][1];
            UINT32 KeyCr = ColorKey[MovieLine[iML][i][4]][2];
            if ( y2 == y1 )
            {
                for ( x=x1; x<x2; x+=2 )
                {
                    y = y2;
                    ((UINT8*)yAddr)[y*imgWidth+x] = KeyY;
                    ((UINT8*)cbAddr)[(y/2)*(imgWidth)+x] = KeyCb;
                    ((UINT8*)crAddr)[(y/2)*(imgWidth)+x+1] = KeyCr;
                }
            }
            else
            {
                for ( y=y1; y<y2; y++ )
                {
                    if ( (y % 10) < 5 ) continue;
                    x = x1 + (x2-x1)*(y-y1)/(y2-y1);
                    x = ((int)(x/2)) * 2;
                    ((UINT8*)yAddr)[y*imgWidth+x] = KeyY;
                    ((UINT8*)yAddr)[y*imgWidth+x+1] = KeyY;
                    ((UINT8*)cbAddr)[(y/2)*(imgWidth)+x] = KeyCb;
                    ((UINT8*)crAddr)[(y/2)*(imgWidth)+x+1] = KeyCr;

                    x = x+2;
                    ((UINT8*)yAddr)[y*imgWidth+x] = KeyY;
                    ((UINT8*)yAddr)[y*imgWidth+x+1] = KeyY;
                    ((UINT8*)cbAddr)[(y/2)*(imgWidth)+x] = KeyCb;
                    ((UINT8*)crAddr)[(y/2)*(imgWidth)+x+1] = KeyCr;

                }
            }
        }// for end
    }
#endif
//#NT#2015/06/05#KS Hung -end

    #if (CHECK_STAMP_TIME == ENABLE)
    DBG_DUMP("time3: %d us\r\n", Perf_GetCurrent() - uiTime);
    //Perf_Close();
    #endif
}
