/*
    System UI Callback

    System Callback for UI Module.

    @file       SysUICB.c
    @ingroup    mIPRJSYS
    @note       None

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
//#include "UICommon.h"
////////////////////////////////////////////////////////////////////////////////

#if _MIPS_TODO
#include "utility.h" //for Perf API
#include "PhotoTsk.h"
#endif
#include "CoordTs.h"
#include "DispSrvApi.h"
#include "UIResource.h"

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "UIDisplay.h"

//View Plugin
#include "UIView.h"
#include "PhotoView.h"
#include "MovieView.h"
#include "PlaybackView.h"

//---------------------SysUICB Debug Definition -----------------------------
//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysUICB
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//---------------------SysUICB Global Variables -----------------------------
//#NT#2009/10/29#Jeah Yen - begin
extern VControl* gUxCtrlTypeList[];
extern VControl* gAppCtrlTypeList[];
//#NT#2009/10/29#Jeah Yen - end

//---------------------SysUICB Prototype Declaration  -----------------------
//---------------------SysUICB Public API  ----------------------------------
//---------------------SysUICB Private API ----------------------------------

/////////////////////////////////////////////////////////////////////////////
void UI_OnSystem(int cmd)
{
#if 0
    switch(cmd)
    {
    case SYSTEM_CMD_POWERON:
    {

        //Init UI module
        //1.設定init值
        //2.設定CB值,
        //3.註冊SxJob服務 ---------> System Job
        //4.註冊SxTimer服務 ---------> Detect Job
        //5.註冊SxCmd服務 ---------> Cmd Function
#if _MIPS_TODO
        System_AddSxCmd(UI_OnCommand); //GxSystem
#endif
        //TM_Begin(UI_INIT);
        UI_GfxInit();
        UI_DispInit();
        UI_ControlShowInit();
        UI_ControlEventInit();
        //TM_End(UI_INIT);
    }
    break;
    case SYSTEM_CMD_POWEROFF:
    {
#if _MIPS_TODO
        //#NT#2010/10/21#Jeah Yen -begin
        if(BKG_GetExecFuncTable() == gBackgroundExtFuncTable)
        {
            //Normal Path
            Ux_Close();
            BKG_Close();
        }
        else
        {
            //Calibration Burn In + Low Battery Auto Power Off = System hang
            //Burn In Path: cannot wait for BACKGROUND_IDLE, or system will hang
            DBG_ERR("^Y>>>Under BurnIn, skip Ux_Close\r\n");
        }
#else
            Ux_Close();
            BKG_Close();
#endif
        //#NT#2010/10/21#Jeah Yen -end
    }
    break;
    case SYSTEM_CMD_SLEEPENTER:
    {
    }
    break;
    case SYSTEM_CMD_SLEEPLEAVE:
    {
    }
    break;
    default:
    break;
    }
#endif
}

///////////////////////////////////////////////////////////////////////////////

#define MYVIEW_COUNT    13
VIEW_TABLE MyViewList[MYVIEW_COUNT]=
{
//VDO1
{UI_SHOW_PREVIEW, LAYER_VDO1, View_Preview}, //VDO preview
{UI_SHOW_PLAYBACK, LAYER_VDO1, View_Playback}, //VDO playback
//VDO2
{UI_SHOW_QUICKVIEW, LAYER_VDO2, View_Quickview}, //VDO quickview
{UI_SHOW_PANL2RVIEW, LAYER_VDO2, View_Guideview_L},//VDO guideview of pancapture (L2R)
{UI_SHOW_PANR2LVIEW, LAYER_VDO2, View_Guideview_R}, //VDO guideview of pancapture (R2L)
{UI_SHOW_PHOTOFRAME, LAYER_VDO2, View_Photoframe}, //VDO Photo frame preview
{UI_SHOW_PS_VIEW, LAYER_VDO2, View_CS_Quickview}, //VDO PS previw
{UI_SHOW_CS_VIEW, LAYER_VDO2, View_CS_Quickview}, //VDO CS previw
{UI_SHOW_BACKGND, LAYER_VDO2, View_Background}, //VDO backgound
{UI_SHOW_BACKGND_CK, LAYER_VDO2, View_Background_CK}, //VDO backgound with ColorKey (by VDO_TRANSPARENT_COLOR)
//OSD1
{UI_SHOW_WINDOW, LAYER_OSD1, View_Window}, //OSD UI window
//OSD2
{UI_SHOW_INFO, LAYER_OSD2, View_Info}, //OSD face detection frame
{0, 0} //END of table
};


/////////////////////////////////////////////////////////////////////////
//
//  Customize Display Flow
//
/////////////////////////////////////////////////////////////////////////

//font table
FONT* UIFontTable[2]=
{
#if (UI_STYLE==UI_STYLE_DRIVER)
    (FONT*)gDemoKit_Font,
    #if(_USR_WND_==ENABLE)
    (FONT*)gDemoKit_Font
    #endif

#else
    (FONT*)gDemo_Font,
    #if(_USR_WND_==ENABLE)
    (FONT*)gDemo_BorderFont
    #endif

#endif
};

BOOL bInitUI = FALSE;
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
extern int bFirstStable;
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
//#NT#2010/05/24#Jeah Yen -FASTBOOT- -begin
extern BOOL g_bPreviewFirstTime;
//#NT#2010/05/24#Jeah Yen -FASTBOOT- -end
_ALIGNED(4) static UINT8 m_TsOsdToVdo[COORDTS_REQ_MEM_SIZE]={0};
COORDTS_HANDLE g_hTsOsdToVdo = NULL;
/*
typedef struct _GFX_INIT
{
    UINT32 uiBufAddr;
    UINT32 uiBufSize;
    //////////////////////////
    UINT32 uiImageCount;
    UINT32 uiPaletteCount;
    UINT32 uiFontCount;
    UINT32 uiStringCount;
    const IMAGE_TABLE*(*pImageTable)(UINT32 id); //id = image style
    const PALETTE_ITEM*(*pPaletteTable)(UINT32 id); //id = color style
    const FONT*(*pFontTable)(UINT32 id); //id = font style
    const STRING_TABLE*(*pStringTable)(UINT32 id); //id = string language
}
GFX_INIT;
*/

void UI_GfxInit(void)
{
    UINT32              uiPoolAddr;

    //Initial all states, all buffers
    DBG_FUNC_BEGIN("\r\n");

    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_GFX_TEMP);

    ////////////////////////////////////////////////////////////////

    GxGfx_Config(CFG_STRING_BUF_SIZE,256);
    //Init Gfx
    GxGfx_Init((UINT32*)uiPoolAddr,POOL_SIZE_GFX_TEMP);       //initial Graphics

    //set default shape, text, image state for GxGfx
    GxGfx_SetAllDefault();
    //set custom image state for GxGfx
    GxGfx_SetImageStroke(ROP_KEY,IMAGEPARAM_DEFAULT);
    GxGfx_SetImageColor(IMAGEPALETTE_DEFAULT,IMAGEPARAM_DEFAULT);
#if (UI_STYLE==UI_STYLE_DRIVER)
    GxGfx_SetImageTable((const IMAGE_TABLE*)gDemoKit_Image);
#else
    GxGfx_SetImageTable((const IMAGE_TABLE*)gDemo_Image);
#endif
    //set custom text state for GxGfx
    GxGfx_SetTextColor(TEXTFORECOLOR1_DEFAULT, TEXTFORECOLOR2_DEFAULT, TEXTFORECOLOR3_DEFAULT);
#if (UI_STYLE==UI_STYLE_DRIVER)
    GxGfx_SetTextStroke((const FONT*)gDemoKit_Font, FONTSTYLE_NORMAL, SCALE_1X);
#else
    GxGfx_SetTextStroke((const FONT*)gDemo_Font, FONTSTYLE_NORMAL, SCALE_1X);
#endif
    GxGfx_SetStringTable((const STRING_TABLE*)Get_LanguageTable());
}

extern UINT32 UICTRL_DRW_SEM_ID;

void UI_DispInit(void)
{
    COORDTS_CREATE Create={0};
    COORDTS_ORIGIN Origin={0};

    //DeviceSize = GxVideo_GetDeviceSize(DOUT1); //Get LCD size

    //Initial all states, all buffers
    DBG_FUNC_BEGIN("\r\n");

    ////////////////////////////////////////////////////////////////
    //Init Coordinate Translate

    Create.uiApiVer = COORDTS_API_VERSION;
    Create.pRegBuf = m_TsOsdToVdo;
    Create.uiRegBufSize = sizeof(m_TsOsdToVdo);
    g_hTsOsdToVdo = CoordTs_Create(&Create);

    Origin.hHandle=g_hTsOsdToVdo;
    Origin.SizeOrigin.w = TOOL_LAYOUT_W;
    Origin.SizeOrigin.h = TOOL_LAYOUT_H;
    CoordTs_SetOrigin(&Origin);
    ////////////////////////////////////////////////////////////////
    //Init UI show

    UI_SetLayoutSize(TOOL_LAYOUT_W, TOOL_LAYOUT_H);

    //Init UI show resource
#if (UI_STYLE==UI_STYLE_DRIVER)
    #if (DISPLAY_OSD_FMT == PXLFMT_INDEX8)
    UI_SetDisplayPalette(LAYER_OSD1,0,256,gDemoKit_Palette_Palette);
    #else
    UI_SetDisplayPalette(LAYER_OSD1,0,16,gDemoKit_Palette_Palette);
    #endif
    #if (DISPLAY_OSD_FMT == PXLFMT_INDEX8)
    UI_SetDisplayPalette(LAYER_OSD2,0,256,gDemoKit_Palette_Palette);
    #else
    UI_SetDisplayPalette(LAYER_OSD2,0,16,gDemoKit_Palette_Palette);
    #endif
#else
    #if (DISPLAY_OSD_FMT == PXLFMT_INDEX8)
    UI_SetDisplayPalette(LAYER_OSD1,0,256,gDemo_Palette_Palette);
    #else
    UI_SetDisplayPalette(LAYER_OSD1,0,16,gDemo_Palette_Palette);
    #endif
    #if (DISPLAY_OSD_FMT == PXLFMT_INDEX8)
    UI_SetDisplayPalette(LAYER_OSD2,0,256,gDemo_Palette_Palette);
    #else
    UI_SetDisplayPalette(LAYER_OSD2,0,16,gDemo_Palette_Palette);
    #endif
#endif
    //Init UI show view
    UI_RegisterView(MYVIEW_COUNT, (VIEW_TABLE*)MyViewList);

    UI_Show(UI_SHOW_WINDOW, FALSE);
    UI_Show(UI_SHOW_INFO, FALSE);

    //clear screen
#if _MIPS_TODO
    UI_CleanDisplay(!gIsUSBChargePreCheck);
#else
    UI_CleanDisplay(FALSE);
#endif
//#NT#2010/05/24#Jeah Yen -FASTBOOT- -begin
    bInitUI = TRUE;
//#NT#2010/05/24#Jeah Yen -FASTBOOT- -end
}

//Init renderer for UIControl show process
void UI_ControlShowInit(void)
{
    DBG_FUNC_BEGIN("\r\n");

    //set palette rendering-time switch table for Ux_Redraw()
    Ux_SetPaletteTable(0);
    //set colormap rendering-time switch table for Ux_Redraw()
    Ux_SetColorMapTable(0);
    //set font switch rendering-time table for Ux_Redraw()
    Ux_SetFontTable(UIFontTable);

    //Init UI framework render object
    Ux_SetRender(&demoRender);
#if 0
    UI_SetDrawSemID(SEMID_UIGRAPHIC);
#endif
}

/////////////////////////////////////////////////////////////////////////////


void Load_ResInfo(void)
{
//jeahyen - EVB - begin
/*
//#Multi lang using Pstore
// check FW update Tag
if (*(volatile UINT8 *)(FW_UPDATE_OFFSET)  == FW_UPDATE_TAG)
{
    //#JPG using Pstore
    #if(_LANG_STORE_ == _PSTORE_)
    if(UIRes_ChkWriteLang())
    {
        Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_LANGUAGE,1,Get_LanguageIndex());
    }
    #endif
    #if (_JPG_STORE_ == _PSTORE_)
    if(UIRes_ChkWriteJPGIMG())
    {
    }
    UIRes_InitReadJPGIMG();
    #endif
}
*/
//jeahyen - EVB - end
}

/////////////////////////////////////////////////////////////////////////////

