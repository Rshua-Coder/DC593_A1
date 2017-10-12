////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UiInfo.h"
#include "UIPhotoInfo.h"
#include "DateTimeInfo.h"
#include "DataStamp.h"
#include "UiDateImprint.h"
#include "UIResource.h"
#include "Utility.h"
#include "FontToYuv.h"
#include "UIMode.h"
#include "GxImage.h"
#include "UIPhotoMapping.h"

#define  USE_DBG_STAMP DISABLE

#define DATE_COLOR_TRANSPARENT  0x00808000
#define DATE_COLOR_WHITE        0x008080FF
#define DATE_COLOR_BLACK        0x00818101
#define DATE_COLOR_ORANGED      0x00D4328A
#define DATE_COLOR_RED          0x00FF554E

typedef struct _DATE_IMPRINT_CTRL{
    IMG_CAP_DATASTAMP_INFO* pInfo;
    UINT32          MemAddr;
    UINT32          MemSize;
    PALETTE_ITEM    FontPalette[3];
    UINT32          PosX;
    UINT32          PosY;
    FONT_TO_YUV_IN  tYuvDesc;
    FONT_TO_YUV_OUT tYuvInfo;
    DS_STAMP_INFOR  StampInfor;
}tDATE_IMPRINT_CTRL;

static tDATE_IMPRINT_CTRL gDateImprintCtrl = {0};
static char gUiDateImprint_StrBuf[64] = {0};
static BOOL   UiDateImprint_GenYuvData(void);
static void   UiDateImprint_CalcPosition(UPOINT* pos);

void UiDateImprint_SetEnable(BOOL bEnable)
{
    if(bEnable)
    {
        DataStamp_Open();
    }
    else
    {
        DataStamp_Close();
    }
}

BOOL UiDateImprint_GenData(IMG_CAP_DATASTAMP_INFO* pInfo)
{
    tDATE_IMPRINT_CTRL* pCtrl = &gDateImprintCtrl;
    BOOL bGenYuv = FALSE;

    pCtrl->pInfo = pInfo;

    switch(pInfo->EventId)
    {
        case CAP_DS_EVENT_THUMB:
        case CAP_DS_EVENT_QV:
        case CAP_DS_EVENT_SCR:
        case CAP_DS_EVENT_PRI:
            bGenYuv = TRUE;
            break;
    }

    if(bGenYuv)
    {
        DS_STAMP_INFOR*  pStampInfor = &pCtrl->StampInfor;
        FONT_TO_YUV_OUT* pYuvInfo = &pCtrl->tYuvInfo;
        UPOINT           pos = {0};

        if(strlen(gUiDateImprint_StrBuf)==0)
        {
            debug_msg("UiDateImprint: strlen=0\r\n");
            return FALSE;
        }

        #if USE_DBG_STAMP
        Perf_Open();
        Perf_Mark();
        #endif

        pCtrl->MemAddr = OS_GetMempoolAddr(POOL_ID_DATEIMPRINT);
        pCtrl->MemSize = POOL_SIZE_DATEIMPRINT;

        //GenYuvData
        System_ChangeCPUSpeed(CPUSPEED_HIGH);
        #if _DEMO_TODO
        wai_sem(SEMID_UIGRAPHIC);
        #endif
        UiDateImprint_GenYuvData();
        #if _DEMO_TODO
        sig_sem(SEMID_UIGRAPHIC);
        #endif
        System_ChangeCPUSpeed(CPUSPEED_NORMAL);

        //Calc Position
        UiDateImprint_CalcPosition(&pos);
        //dump pic just for debug
        #if 0
        GxImg_DumpBuf(&pYuvInfo->GenImg,TRUE);
        #endif
        pStampInfor->PosX = pos.x;
        pStampInfor->PosY = pos.y;
        #if 0
        pStampInfor->Img.Addr[0] = pYuvInfo->GenImg.PxlAddr[0];
        pStampInfor->Img.Addr[1] = pYuvInfo->GenImg.PxlAddr[1];
        pStampInfor->Img.LineOffset[0] = pYuvInfo->GenImg.LineOffset[0];
        pStampInfor->Img.LineOffset[1] = pYuvInfo->GenImg.LineOffset[1];
        pStampInfor->Img.Width[0] = pYuvInfo->GenImg.Width;
        pStampInfor->Img.Width[1] = pYuvInfo->GenImg.Width;
        pStampInfor->Img.Height[0] = pYuvInfo->GenImg.Height;
        if (pYuvInfo->GenImg.PxlFmt == GX_IMAGE_PIXEL_FMT_YUV420)
        {
            pStampInfor->Img.Format = DS_YCCFORMAT_420_PACK;
            pStampInfor->Img.Height[1] = pYuvInfo->GenImg.Height>>1;
        }
        else
        {
            pStampInfor->Img.Format = DS_YCCFORMAT_422_PACK;
            pStampInfor->Img.Height[1] = pYuvInfo->GenImg.Height;
        }
        #else
        pStampInfor->Img = pYuvInfo->GenImg;
        #endif
        pStampInfor->ColorKey = COLOR_MAKE_YUVD(pYuvInfo->ColorKeyY,pYuvInfo->ColorKeyCb,pYuvInfo->ColorKeyCr);
        pStampInfor->StampWeight = 255;
        DataStamp_SetInfor(pStampInfor);

        #if USE_DBG_STAMP
        debug_msg("Time Use = %d ms, MemMax Use = %d bytes, MemMin Use=%d bytes\r\n",Perf_GetDuration()/1000,pYuvInfo->UsedMaxMemSize,pYuvInfo->UsedMemSize);
        Perf_Close();
        #endif
    }

    return TRUE;
}

void UiDateImprint_UpdateDate(void)
{
    DateTime_Load();

    switch(UI_GetData(FL_DATE_STAMP))
    {
        case DATEIMPRINT_DATE:
            strncpy(gUiDateImprint_StrBuf,DateTime_MakeYMD(),sizeof(gUiDateImprint_StrBuf)-1);
        break;

        case DATEIMPRINT_DATE_TIME:
            strncpy(gUiDateImprint_StrBuf,DateTime_MakeYMDHMS(),sizeof(gUiDateImprint_StrBuf)-1);
        break;

        default:
            gUiDateImprint_StrBuf[0]=0; //Empty String
    }
}
#if(LOW_POWER_TIMELAPSE_REC == ENABLE)
#include "rtc.h"
#endif
static BOOL UiDateImprint_GenYuvData(void)
{
    tDATE_IMPRINT_CTRL* pCtrl = &gDateImprintCtrl;
    FONT_TO_YUV_IN*  pIn = &pCtrl->tYuvDesc;
    FONT_TO_YUV_OUT* pOut = &pCtrl->tYuvInfo;
    UINT32 photo_w = pCtrl->pInfo->ImgInfo.Ch[0].Width;
    UINT32 photo_h = pCtrl->pInfo->ImgInfo.Ch[0].Height;
    //UINT32 font_h;
    //UINT32 stamp_scale;
    //UINT32 stamp_ratio_num; //numerator
    //UINT32 stamp_ratio_denom; //denominator
    //UINT32 ImgSize;

    pIn->MemAddr = pCtrl->MemAddr;
    pIn->MemSize = pCtrl->MemSize;
    pIn->pStr = gUiDateImprint_StrBuf;

    if((UI_GetData(FL_ModeIndex) == DSC_MODE_PHOTO_SCENE) && UI_GetData(FL_SCENE) == SCENE_TEXT)
        pIn->ciSolid = DATE_COLOR_BLACK;
    else if((UI_GetData(FL_ModeIndex) == DSC_MODE_PHOTO_SCENE) && UI_GetData(FL_SCENE) == SCENE_SKETCH)
        pIn->ciSolid = DATE_COLOR_BLACK;
    else if (((UI_GetData(FL_COLOR_EFFECT) == COLOR_BW) || (UI_GetData(FL_COLOR_EFFECT) == COLOR_SEPIA)))
        pIn->ciSolid = DATE_COLOR_WHITE;
    else
        pIn->ciSolid = DATE_COLOR_RED;
    pIn->ciFrame = DATE_COLOR_TRANSPARENT;
    pIn->ciTransparet = DATE_COLOR_TRANSPARENT;

    pIn->Format = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
#if(LOW_POWER_TIMELAPSE_REC == ENABLE)
    if(rtc_isPWRAlarm())
    {
        pIn->Format = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
    }
#endif
    pIn->bEnableSmooth= FALSE;
    //ImgSize = photo_w*photo_h;

    if(photo_w*photo_h <= 160*120)
    {
        //thumbnail Size
        pIn->pFont = (FONT*)gDemo_SmallFont;
        pIn->ScaleFactor = 65536/3; //0.33x
    }
    else if(photo_w*photo_h <= 640*480)
    {
        //VGA Size
        pIn->pFont = (FONT*)gDemo_SmallFont;
        pIn->ScaleFactor = 65536; //1.0x
    }
    else
    {
        UINT32 font_h,stamp_ratio;
        pIn->pFont = (FONT*)gDemo_BigFont;

        font_h = 190;
        stamp_ratio = 0x00000A00; //0.04x
        pIn->ScaleFactor = photo_h * stamp_ratio / font_h;
    }
    FontToYuv(pIn,pOut);
    return TRUE;
}

static void UiDateImprint_CalcPosition(UPOINT* pos)
{
    tDATE_IMPRINT_CTRL* pCtrl = &gDateImprintCtrl;
    IMG_CAP_DATASTAMP_INFO* pInfo = pCtrl->pInfo;
    FONT_TO_YUV_OUT* pYuvInfo = &pCtrl->tYuvInfo;

    UINT32 photo_w = pInfo->ImgInfo.Ch[0].Width;
    UINT32 photo_h = pInfo->ImgInfo.Ch[0].Height;
    #if 0

    BOOL IsThumb = (pInfo->EventId == CAP_DS_EVENT_THUMB)?TRUE:FALSE;

    UINT32 ImgXRatio, ImgYRatio;
    UINT32 Numerator, Denominator;
    UINT32 xPos, yPos;
    UINT32 XStartRatio, YStartRatio;
    UINT32 ImageSize;

    ImageSize = UI_GetData(FL_PHOTO_SIZE);

    if (IsThumb == TRUE)
    {
        xPos = 100;
        yPos = 100;
    }
    else
    {
        xPos = 200;
        yPos = 190;
    }
    ImgXRatio = 4;
    ImgYRatio = 3;
    if (ImageSize == SIZE_12M32)
    {
        if (IsThumb == TRUE)
        {
            yPos += 40;
        }
        ImgXRatio = 3;
        ImgYRatio = 2;
        xPos = (xPos * ImgXRatio) / 4;
        yPos = (yPos * ImgYRatio) / 3;
    }
    else if (ImageSize == SIZE_10M169)
    {
        if (IsThumb == TRUE)
        {
            yPos += 100;
        }
        else
        {
            yPos -= 40;
        }
        ImgXRatio = 16;
        ImgYRatio = 9;
        xPos = (xPos * ImgXRatio) / 4;
        yPos = (yPos * ImgYRatio) / 3;
    }

    Denominator = (ImgXRatio << 10);
    Numerator = Denominator;
    Numerator -= (ImgXRatio * pYuvInfo->GenImg.Width << 10) / photo_w + xPos;
    XStartRatio = (Numerator << 10) / Denominator;
    Denominator = (ImgYRatio << 10);
    Numerator = Denominator;
    Numerator -= (ImgYRatio * pYuvInfo->GenImg.Height << 10) / photo_h + yPos;
    YStartRatio = (Numerator << 10) / Denominator;

    if (IsThumb == TRUE)
    {
        //correct vertical ratio
        if ((photo_w == 640) && (photo_h == 480))
        {
            if (ImageSize == SIZE_12M32)
            {
                YStartRatio -= (10 << 10) / 240;
            }
            else if (ImageSize == SIZE_10M169)
            {
                YStartRatio -= (22 << 10) / 240;
            }
        }
    }

    pos->x = ALIGN_ROUND(XStartRatio, 4);
    pos->y = ALIGN_ROUND(YStartRatio, 2);
    #else
    pos->x = ALIGN_ROUND_4(photo_w - pYuvInfo->GenImg.Width - pYuvInfo->GenImg.Height);
    pos->y = ALIGN_ROUND_4(photo_h - pYuvInfo->GenImg.Height - pYuvInfo->GenImg.Height);
    #endif
}

void UiDateImprint_InitBuff(void)
{
    DS_STAMP_INFOR  stampInfo ={0};
    UINT32          genMaxw,genMaxh;

    genMaxw = GetPhotoSizeWidth(SIZE_14M)/2;
    genMaxh = GetPhotoSizeHeight(SIZE_14M) * 0.04;
    stampInfo.Img.LineOffset[0] = genMaxw;
    stampInfo.Img.LineOffset[1] = genMaxw;
    stampInfo.Img.Width = genMaxw;
    stampInfo.Img.Height = genMaxh;
    stampInfo.Img.PxlFmt = GX_IMAGE_PIXEL_FMT_YUV422;
    DataStamp_SetInfor(&stampInfo);
}

