#ifndef _UI_RESORUCE_H_
#define _UI_RESORUCE_H_
#include "prjcfg.h"
#include "GxGfx.h"

#include "D320x240_DRIVER/UIResource/DemoKit_String.h"
#include "D320x240_DRIVER/UIResource/DemoKit_image.h"
#include "D320x240_DRIVER/UIResource/DemoKit_font.h"
#include "D320x240_DRIVER/UIResource/DemoKit_palette.h"
#include "D320x240_DRIVER/UIResource/Demo_BigFont.h"
#include "D320x240_DRIVER/UIResource/Demo_SmallFont.h"

#if 0
#include "demo_string.h"
#include "demo_image.h"
#include "demo_font.h"
#include "demo_smallfont.h"
#include "demo_bigfont.h"
#include "demo_borderfont.h"
#include "demo_palette.h"
#include "demo_sound.h"
//#NT#2009/10/29#Lincy Lin -begin
#include "demo_imagejpg.h"
//#NT#2009/10/29#Lincy Lin -end
#include "BG_images.h"
#endif
// Update FW check byte
#define FW_UPDATE_OFFSET        0x57    // value should match with loader
// Update FW Tag
#define FW_UPDATE_TAG           0x55    // value should match with loader

extern const unsigned char gDemo_ARString[];
extern const unsigned char gDemo_PERString[];
extern FONT* UIFontTable[];


///////////////////////////////////////////////////////////////////////////////
//
//  Palette

#define _OSD_INDEX_TRANSPART             0
#define _OSD_INDEX_BLUE                  1
#define _OSD_INDEX_WHITE                 2
#define _OSD_INDEX_BLACK                 3
#define _OSD_INDEX_GRAY75                8
#define _OSD_INDEX_DARKGRAY              9
#define _OSD_INDEX_ORANGE                15
#define _OSD_INDEX_GREEN                 13
#define _OSD_INDEX_GRAY50                4
#define _OSD_INDEX_RED                   14
#define _OSD_INDEX_LIGHTGREEN            12
#define _OSD_INDEX_YELLOW                16
#define _OSD_INDEX_GRAY                  6
//#define _OSD_INDEX_PINK                  64
//#define _OSD_INDEX_PURPLE                62
#define _OSD_INDEX_BLACK50               0x3E
#define _OSD_INDEX_BLACK75               0x3F
#define _OSD_INDEX_WHITE50               0x40


///////////////////////////////////////////////////////////////////////////////
//
//  Font


///////////////////////////////////////////////////////////////////////////////
//
//  Wallpaper (JPG)

#define WP_LOGO     0
#define WP_MENU     1

//#NT#2010/03/08#JeahYen -begin
typedef UINT32 (*PF_PREPARE_USER_WP)(UINT32 addr, UINT32 size);
//#NT#2010/03/08#JeahYen -end

extern void Reg_UserWPFunc(PF_PREPARE_USER_WP fp); //please register user wallpaper prepare function

extern BOOL UIRes_ChkWriteWP(UINT32 id);
extern void UIRes_InitReadWP(UINT32 id);
//#NT#2010/03/01#Jeah Yen -begin
extern void UIRes_InitReadWP_fast(UINT32 id); //load under partial-one
//#NT#2010/03/01#Jeah Yen -end
//extern void UIRes_DumpMemWP(UINT32 id);
extern void UIRes_DeleteWP(UINT32 id);

//#NT#2010/02/26#Jeah Yen -begin
extern const IMAGE* Get_WPImage(UINT32 uhIndex);
extern const IMAGE* Get_LOGOImage(void);
//#NT#2010/02/26#Jeah Yen -end

///////////////////////////////////////////////////////////////////////////////
//
//  Image (JPG)



extern BOOL UIRes_ChkWriteIMG(void);
extern void UIRes_InitReadIMG(void);
extern void UIRes_DumpMemIMG(void);
extern void UIRes_DeleteIMG(void);

extern UINT32 Get_IMGTable(UINT32 uhIndex);

//#NT#2011/02/14Steven feng-begin

//Define the number of Slideshow WAV files
#define SSWAV_NUM 4

extern BOOL UIRes_ChkWriteSSWAV(void);
extern void UIRes_InitReadSSWAV(int SSWAV_id);
//#NT#2011/02/15Steven feng-end

//#NT#2011/02/16#Lily Kao -begin
extern UINT32 UIRes_ReadSSWAV(int SSWAV_id, UINT32 uiPoolAddr, UINT32 uiSize);
//#NT#2011/02/16#Lily Kao -end


///////////////////////////////////////////////////////////////////////////////
//
//  String (LANG)


extern void UIRes_InitReadLang(int lang_id);
extern BOOL UIRes_ChkWriteLang(void);
extern void UIRes_SetLang(int lang_id);
extern void UIRes_DumpMemLang(void);
extern void UIRes_DeleteLang(void);

extern UINT32 Get_LanguageTable(void);

///////////////////////////////////////////////////////////////////////////////
//
//  Sound


///////////////////////////////////////////////////////////////////////////////
//
//  Photo Frame (JPG)


extern BOOL UIRes_ChkWritePHFE(void);
extern void UIRes_InitReadPHFE(void);
extern void UIRes_DeletePHFE(void);


#endif //_UI_RESORUCE_H_
