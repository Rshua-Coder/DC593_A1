#ifndef UI_PLAYOBJ_H
#define UI_PLAYOBJ_H

#include "UIFramework.h"
//#NT#2009/10/23#Jeah Yen - begin
#include "AppLib.h"
//#NT#2009/10/23#Jeah Yen - end
//#NT#2009/10/23#Jeah Yen - begin
#include "PlaybackTsk.h"
//#NT#2009/10/23#Jeah Yen - end
#include "PrjCfg.h"

//-----------------------------------------------------
// Function Definition
//-----------------------------------------------------
//#NT#2010/07/08#Ben Wang -begin
//#NT#Add fast browse mode
#define BROWSE_MODE_NORMAL 0
#define BROWSE_MODE_FAST   1
#define BROWSE_MODE    BROWSE_MODE_NORMAL
//#NT#2010/07/08#Ben Wang -end

#define PLAYMODE_USERJPGOUT             77
#define PLAYMODE_QVIEW                  66      // show first image from SDRAM Q-view buffer

#define PLAYMODE_PICBRIDGE              55
#define PLAYMODE_ONLYTHUMB              33      // first decode always decode only thumbnail
#define PLAYMODE_UNKNOWN                99      // first decode the max-file-id file
#define PLAYMODE_PRIMARY                10
#define PLAYMODE_AVI                    11
#define PLAYMODE_ROTATE                 12
#define PLAYMODE_WAV                    13
#define PLAYMODE_ASF                    14
#define PLAYMODE_MPG                    16

#define PLAYMODE_THUMB                  9
#define PLAYMODE_SLIDESHOW              15
#define PLAYMODE_MOVMJPG                17

#define PLAYZOOM_IN                     0x01
#define PLAYZOOM_OUT                    0x02

#define PLAYZOOM_UP                     0x04
#define PLAYZOOM_DOWN                   0x08
#define PLAYZOOM_LEFT                   0x10
#define PLAYZOOM_RIGHT                  0x20

#define PB_BROWSER_CURR                 0x01
#define PB_BROWSER_NEXT                 0x02
#define PB_BROWSER_PREV                 0x04

#define PB_DELETE_ONE                   0x01
#define PB_DELETE_ALL                   0x02

//Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PROTECT,1,PLAY_PROTECT_ONE);
#define PLAY_PROTECT_ONE                0x01
#define PLAY_PROTECT_ALL                0x02
#define PLAY_UNPROTECT_ONE              0x04
#define PLAY_UNPROTECT_ALL              0x08

// Rotate parameter
#define PLAY_ROTATE_DIR_UNKNOWN         0xFF
#define PLAY_ROTATE_DIR_0               PLAY_ROTATE_DIR_UNKNOWN
#define PLAY_ROTATE_DIR_90              0x00
#define PLAY_ROTATE_DIR_180             0x01
#define PLAY_ROTATE_DIR_270             0x02
#define PLAY_ROTATE_DIR_HOR             0x03
#define PLAY_ROTATE_DIR_VER             0x04

//#NT#2011/01/31#Ben Wang -begin
//#NT#Add screen control to sync display with OSD
//Screen control parameter
#define PLAY_LOCK_SCREEN   0  //<0, the result of image operation will not show on display
#define PLAY_FLUSH_SCREEN  1  //<1, show the result of image operation on display
//#NT#2011/01/31#Ben Wang -end


typedef struct
{
    UINT32   DisplayStart_X;        // crop display start x
    UINT32   DisplayStart_Y;        // crop display start y
    UINT32   DisplayWidth;          // crop display width
    UINT32   DisplayHeight;         // crop display height
    BOOL     IfOverWrite;           // TRUE / FALSE, use to overwrite or not
} APPPLAY_CROPSAVE_OBJ, *PAPPPLAY_CROPSAVE_OBJ;
//#NT#2010/11/05#Ben Wang -begin
//#NT#Add the feature of "show selected thumb on background"
typedef struct
{
    UINT8   HorNums; //the number of images in horizontal direction
    UINT8   VerNums; //the number of images in vertical direction
    PURECT  pThumbRect;//Pointer of the rect-array of browser files
//#NT#2012/11/27#Scottie -begin
//#NT#Support Dual Display for PB
    PURECT  pThumbRect2;//Pointer of the rect-array of browser files
//#NT#2012/11/27#Scottie -end
    BOOL    bDrawCurrImgOnBG; // draw selected thumb image on background
} BROWSE_INIT_OBJ, *PBROWSE_INIT_OBJ;
//#NT#2010/11/05#Ben Wang -end
// Copy to card mode
#define PB_COPY2CARD_NEW_DIR            0x00
#define PB_COPY2CARD_APPEND             0x01
#define PB_COPY2CARD_NO_RENAME          0x02
#define PB_COPY2CARD_NEW_DIR_ORG_FILE   0x03
/**
  Event: NVTEVT_PLAYINIT
  Parameter number: 0
  Description: Init playback and wait the first image done
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYINIT,0);


  Event: NVTEVT_PLAYSINGLE
  Parameter number: 2
  Parameter1 : play command
                  PB_SINGLE_CURR
                  PB_SINGLE_NEXT
                  PB_SINGLE_PREV
                  PB_SINGLE_THUMB
  Parameter2: jump offset
  Description: Display single image, default for primary JPG or PB_SINGLE_THUMB for thumbnail
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYSINGLE, 2, PB_SINGLE_PREV, 1);
        or
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYSINGLE, 2, PB_SINGLE_NEXT | PB_SINGLE_THUMB, 1);

  Event: NVTEVT_DELETE
  Parameter number: 1
  Parameter1 : delete command
                  PB_DELETE_ONE
                  PB_DELETE_ALL
  Description: delete single or all files
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_DELETE, 1, PB_DELETE_ONE);

  Event: NVTEVT_DELETE_BY_DATE
  Parameter number: 3
  Parameter1 : Year
  Parameter2 : Month
  Parameter3 : Day

  Description: Delete files by date, only valid in play by date mode
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_DELETE_BY_DATE, 3, 2010, 1, 31);

  Event: NVTEVT_PROTECT
  Parameter number: 1
  Parameter1 : protect command
                PLAY_PROTECT_ONE
                PLAY_PROTECT_ALL
                PLAY_UNPROTECT_ONE
                PLAY_UNPROTECT_ALL
  Description: protect/unprotect single or all files
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PROTECT,1,PLAY_PROTECT_ONE);

  Event: NVTEVT_SET_DPOFONE
  Parameter number: 2
  Parameter1 : print number, max is 99
  Parameter2:  date print, TRUE or FALSE
  Description: Set DPOF for current image
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_SET_DPOFONE,2, 10, TRUE);

  Event: NVTEVT_SET_DPOFALL
  Parameter number: 2
  Parameter1 : print number, max is 99
  Parameter2:  date print, TRUE or FALSE
  Description: Set DPOF for all image
  Example:
    Ux_SendEvent(pSysCtrl,NVTEVT_SET_DPOFALL,2, 10, TRUE);

  Event: NVTEVT_ZOOMINIT
  Parameter number: 1
  Parameter1 : zoom table,  should be cast to UINT32
  Description: Init zoom scale table
  Example:
    UINT32   g_UIPlayZoomLvlTbl[4] =
    //1X      2X      3X      4X
    { 100, 200, 300, 400};
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_ZOOMINIT,1, (UINT32)g_UIPlayZoomLvlTbl);

  Event: NVTEVT_ZOOMINOUT
  Parameter number: 1
  Parameter1 : zoom command, PLAYZOOM_IN or PLAYZOOM_OUT
  Description: zoom in or out
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_ZOOMINOUT, 1, PLAYZOOM_IN);

  Event: NVTEVT_ZOOMNAVI
  Parameter number: 3
  Parameter1 : zoom command
                PLAYZOOM_UP
                PLAYZOOM_DOWN
                PLAYZOOM_LEFT
                PLAYZOOM_RIGHT
  Parameter2 : horizontal offset
  Parameter3 : vertical offset
  Description: zoom moving
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_ZOOMNAVI,3, PLAYZOOM_UP, 0,100);
              or
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_ZOOMNAVI,3, PLAYZOOM_LEFT|PLAYZOOM_UP, 50,100);

  Event: NVTEVT_BROWSEINIT
  Parameter number: 1
  Parameter1 : the structure point of BROWSE_INIT_OBJ, should be cast to UINT32
  Example:
    //2 x 2 thumbnail mode
    URECT    g_ThumbRect[2*2] =
    {
        { 44,  30, 272, 226}, {328,  30, 272, 226},
        { 44, 270, 272, 226}, {328, 270, 272, 226},
    };
    BROWSE_INIT_OBJ BrowseInitObj;
    BrowseInitObj.HorNums = 2;
    BrowseInitObj.VerNums = 2;
    BrowseInitObj.pThumbRect = &g_ThumbRect[0];
    BrowseInitObj.bDrawCurrImgOnBG = FALSE;
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_BROWSEINIT,1, (UINT32)&BrowseInitObj);

  Event: NVTEVT_BROWSESINGLE
  Parameter number: 2
  Parameter1 : browse command
            PB_BROWSER_CURR
            PB_BROWSER_NEXT
            PB_BROWSER_PREV
  Parameter2: move offset
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_BROWSESINGLE, 2, PB_BROWSER_NEXT, 1);

  Event: NVTEVT_BROWSEFIN
  Parameter number: 0
  Description:wait NVTEVT_BROWSESINGLE command done while browse a new page
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_BROWSESINGLE, 2, PB_BROWSER_NEXT, 1);
    if(AppPlay_GetData(PLAY_PAGE_CHANGED))
        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_BROWSEFIN, 0);

  Event: NVTEVT_UPDATE_RSVDATA
  Parameter number: 1
  Parameter1: the structure point of PBRSV_DATA, should be cast to UINT32
  Description: Update reserved data such as movie background image, bad file image...
              , usually for TV plug in or unplug
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_UPDATE_RSVDATA, 1, (UINT32)(&RsvData));

  Event: NVTEVT_CHANGE_PBSIZE
  Parameter number: 1
  Parameter1: the structure point of ISIZE, should be cast to UINT32
  Description: Update playback buffer size, usually for TV plug in or unplug
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_CHANGE_PBSIZE, 1, (UINT32)pSize);

  Event: NVTEVT_REQUALITY
  Parameter number: 2
  Parameter1 : QUALITY 1~99
                 Quality > 94   =>  Quality_SuperHigh
                 Quality 90~94  =>  Quality_High
                 Quality < 90   =>  Quality_Economy
  Parameter2 : TRUE: Overwrite original file
               FALSE: Save a new file
  Description: change quality
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_REQUALITY, 2, 90, TRUE);

  Event: NVTEVT_RESIZE
  Parameter number: 3
  Parameter1 : New Width
  Parameter2 : New Height
  Parameter3 : TRUE: Overwrite original file
               FALSE: Save a new file
  Description: change size
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_RESIZE, 3, 640, 480, TRUE);

  Event: NVTEVT_FIXREDEYE
  Parameter number: 1
  Parameter1 :  whether show the result to display
                PLAY_RED_EYE_REMOVAL_DISPLAY_ONLY
                PLAY_RED_EYE_REMOVAL_SAVE_FILE
                PLAY_RED_EYE_REMOVAL_SAVE_AS_NEW_FILE
  Description: Fix red eye without saving file
  Example:
    //fix red eye and show result to display
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_FIXREDEYE, 1, PLAY_RED_EYE_REMOVAL_SAVE_AS_NEW_FILE);

  Event: NVTEVT_RESAVEFILE
  Parameter number: 1
  Parameter1 : TRUE: Overwrite original file
               FALSE: Save a new file
  Description: save the current raw data to a jpeg file with previous file header
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_RESAVEFILE, 1, TRUE);

  Event: NVTEVT_ROTATE_DISPLAY
  Parameter number: 1
  Parameter1 : rotation direction
                PLAY_ROTATE_DIR_0
                PLAY_ROTATE_DIR_90
                PLAY_ROTATE_DIR_180
                PLAY_ROTATE_DIR_270
                PLAY_ROTATE_DIR_HOR
                PLAY_ROTATE_DIR_VER
  Description: rotate display only
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_ROTATE_DISPLAY, 1, PLAY_ROTATE_DIR_90);

  Event: NVTEVT_ROTATE_EXIF
  Parameter number: 2
  Parameter1 : Orientation
                PLAY_ROTATE_DIR_0
                PLAY_ROTATE_DIR_90
                PLAY_ROTATE_DIR_180
                PLAY_ROTATE_DIR_270

  Parameter2 : TRUE: Overwrite original file
               FALSE: Save a new file

  Description: rotate image by editing exif and display the result
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_ROTATE_IMAGE, 2, PLAY_ROTATE_DIR_90, TRUE);

  Event: NVTEVT_ROTATE_IMAGE
  Parameter number: 2
  Parameter1 : rotation direction
                PLAY_ROTATE_DIR_0
                PLAY_ROTATE_DIR_90
                PLAY_ROTATE_DIR_180
                PLAY_ROTATE_DIR_270
                PLAY_ROTATE_DIR_HOR
                PLAY_ROTATE_DIR_VER

  Parameter2 : TRUE: Overwrite original file
               FALSE: Save a new file

  Description: rotate image raw data and display the result
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_ROTATE_IMAGE, 2, PLAY_ROTATE_DIR_90, TRUE);

  Event: NVTEVT_NOVALIGHT_DISPLAY
  Parameter number: 1
  Parameter1: the structure point of PB_NOVALIGHT_DISP, should be cast to UINT32
  Description: Show the result of NovaLight on specified position, for display only, not changing raw data
  Note:The position should base on PBDisplay size, NOT OSD size.
  Example:
    PB_NOVALIGHT_DISP NovaLight;
    NovaLight.DstStart_X = 0;
    NovaLight.DstStart_Y = 0;
    NovaLight.DstWidth = 640;
    NovaLight.DstHeight = 240;
    NovaLight.Enhance = NOVALIGHT_LEVEL_3;
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_NOVALIGHT_DISPLAY, 1, (UINT32)(&NovaLight));

  Event: NVTEVT_NOVALIGHT_FIX
  Parameter number: 1
  Parameter1 : TRUE: Overwrite original file
               FALSE: Save a new file
  Description: Process raw data with NovaLight function and save to a new file or overwrite the original one
  Note:This event should be used after NVTEVT_NOVALIGHT_DISPLAY.
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_NOVALIGHT_FIX, 1, TRUE);

  Event: NVTEVT_SINGLE_SPE
  Parameter number: 1
  Parameter1: the structure point of PB_SINGLE_DISP, should be cast to UINT32
  Description: Show the current raw data to specified position.
  Note:The position should base on PBDisplay size, NOT OSD size.
  Example:
    PB_SINGLE_DISP Single;
    Single.DstStart_X = 0;
    Single.DstStart_Y = 0;
    Single.DstWidth = 640;
    Single.DstHeight = 240;
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_SINGLE_SPE, 1, (UINT32)(&Single));

  Event: NVTEVT_FILL_BG
  Parameter number: 1
  Parameter1 : color data in YCbCr mode, EX:black=0x00008080
  Description: Fill in the specified color data in frame buffer, not OSD buffer
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_FILL_BG, 1, 0x00008080);

  Event: NVTEVT_SETVIDEO_DISP
  Parameter number: 1
  Parameter1: the structure point of VDO_WIN_ATTR, should be cast to UINT32
  Description: Set VIDEO layer to specified position and size.
  Note:The position should base on OSD coordinate.
  Example:
    VDO_WIN_ATTR VideoAttr;
    VideoAttr.uiVideoID = VIDEOID_1;
    VideoAttr.uiOSDWidth = 180;
    VideoAttr.uiOSDHeight= 120;
    VideoAttr.uiOutStartX = 50;
    VideoAttr.uiOutStartY= 50;
    VideoAttr.uiOutWidth = OSD_W;
    VideoAttr.uiOutHeight= OSD_H;
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_SET_VDO_WIN, 1, (UINT32)(&VideoAttr));

  Event: NVTEVT_CROP_SAVE
  Parameter number: 1
  Parameter1: the structure point of VDO_WIN_ATTR, should be cast to UINT32
  Description: Set VIDEO layer to specified position and size.
  Note:The position should base on OSD coordinate.
  Example:
    APPPLAY_CROPSAVE_OBJ PlayCropSaveObj;
    PlayCropSaveObj.DisplayStart_X = 0;
    PlayCropSaveObj.DisplayStart_Y = 0;
    PlayCropSaveObj.DisplayWidth = [LCD Size Width];
    PlayCropSaveObj.DisplayHeight = [LCD Size Height]
    PlayCropSaveObj.IfOverWrite = FALSE;
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_CROP_SAVE, 1, (UINT32)(&PlayCropSaveObj));

  Event: NVTEVT_COPY2CARD
  Parameter number: 1
  Parameter1 : copy style
                PB_COPY2CARD_NEW_DIR
                PB_COPY2CARD_APPEND
                PB_COPY2CARD_NO_RENAME
                PB_COPY2CARD_NEW_DIR_ORG_FILE
  Description:
  Example:
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_COPY2CARD, 1, PB_COPY2CARD_NEW_DIR);

  Event: NVTEVT_DETECT_FACE
  Parameter number: 0
  Description: detect face
  Example:
    URECT*  pRectFace;
    UINT32 nFaces;

    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_DETECT_FACE,0);
    nFaces = AppPlay_GetData(PLAY_FACENUM);
    pRectFace = (URECT*)AppPlay_GetData(PLAY_FACERECT);

  Event: NVTEVT_SCREEN_CTRL
  Parameter number: 1
  Parameter1 : screen control
                PLAY_LOCK_SCREEN
                PLAY_FLUSH_SCREEN
  Description: Control the operation result to show on display immediately or not.
    This API is only valid for one PB operation.
  Example:
    Ux_SendEvent(&UIPlayObjCtrl, NVTEVT_SCREEN_CTRL, 1, PLAY_FLUSH_SCREEN);

**/


//#NT#2009/10/29#Jeah Yen - begin
typedef enum
{
    NVTEVT_PLAY_MIN    = APP_EVENT_MASK + 0x00000100,
    NVTEVT_PLAYINIT    = NVTEVT_PLAY_MIN,
    NVTEVT_PLAYSINGLE,
    NVTEVT_DELETE,
    NVTEVT_PROTECT,
    NVTEVT_ZOOMINIT,
    NVTEVT_ZOOMINOUT,
    NVTEVT_ZOOMNAVI,
    NVTEVT_BROWSEINIT,
    NVTEVT_BROWSESINGLE,
    NVTEVT_BROWSEFIN,
    NVTEVT_UPDATE_RSVDATA,
    NVTEVT_CHANGE_PBSIZE,
    NVTEVT_REQUALITY,
    NVTEVT_RESIZE,
    NVTEVT_FIXREDEYE,
    NVTEVT_RESAVEFILE,
    NVTEVT_ROTATE_DISPLAY,
    NVTEVT_ROTATE_EXIF,
    NVTEVT_NOVALIGHT_DISPLAY,
    NVTEVT_NOVALIGHT_FIX,
    NVTEVT_COPY2CARD,
//#NT#2010/09/28#Ben Wang -begin
//#NT#Add face detect function
    NVTEVT_DETECT_FACE,
//#NT#2010/09/28#Ben Wang -end

//#NT#2011/01/31#Ben Wang -begin
//#NT#Add screen control to sync display with OSD
    NVTEVT_SCREEN_CTRL,
//#NT#2011/01/31#Ben Wang -end
    NVTEVT_PLAY_MAX
}
NVT_PLAY_EVENT;
//#NT#2009/10/29#Jeah Yen - end

typedef enum
{
    PLAY_MOVE_EFFECT,   //write only
    PLAY_PBSTATUS,      //read only
    PLAY_CURRMODE,      //read only
    PLAY_LOCKSTATUS,    //read only
    PLAY_DIRID,         //read only
    PLAY_FILEID,        //read only
    PLAY_FILESEQ,       //read only
    PLAY_FILENUM,       //read only
    PLAY_FILESIZE,      //read only
    PLAY_MAXDIRID,      //read only
    PLAY_DIRNUM,        //read only
    PLAY_ISO,           //read only
    PLAY_ZOOMINDEX,     //read only
    PLAY_BROWSEINDEX,   //read only
    PLAY_PAGE_CHANGED,  //read only, return TRUE if browse a new page
    PLAY_ROTATION,      //read only
    PLAY_IMGWIDTH,      //read only
    PLAY_IMGHEIGHT,     //read only
    PLAY_IMGWIDTH_ORI,  //read only
    PLAY_IMGHEIGHT_ORI, //read only
    PLAY_PANSRCX,       //read only
    PLAY_PANSRCY,       //read only
    PLAY_PANMAXX,       //read only
    PLAY_PANMAXY,       //read only
    PLAY_ORIENTATION,   //read only
    PLAY_LIGHTSOURCE,   //read only
    PLAY_EXIF_WB,       //read only
    PLAY_FILEFMT,       //read only
    PLAY_ISJPGWITHMEMO, //read only, return TRUE or FALSE
    PLAY_ALLTHUMBFMT,   //read only, the array pointer(UINT16 *) of all thumb-file-format
    PLAY_ALLTHUMBSEQ,   //read only, the array pointer(UINT16 *) of all thumb-file-sequence
//#NT#2010/09/24#Ben Wang -begin
//#NT#Add thumb movie length info
    PLAY_ALLTHUMBMOVIELENGTH,//read only, the array pointer(UINT36 *) of all thumb-file-movie_length
//#NT#2010/09/24#Ben Wang -end
//#NT#2010/12/15#Ben Wang -begin
//#NT#Add thumb special function info
    PLAY_ALLTHUMBSPECFUNC,
//#NT#2010/12/15#Ben Wang -end
//#NT#2010/12/06#Ben Wang -begin
//#NT#Add thumb decode result info
    PLAY_ALLTHUMBDEC,   //read only, the array pointer(UINT16 *) of all thumb file decoded result
//#NT#2010/12/06#Ben Wang -end
    PLAY_CURRTHUMBNUM,  //read only,
    PLAY_MAX,
    ENUM_DUMMY4WORD(PLAY_DATA_SET)
}PLAY_DATA_SET;

typedef struct
{
    UINT32   uiPBRsvBuffAddr;
    UINT32   uiPBRsvBuffSize;
    UINT32   uiDisplayWidth;
    UINT32   uiDisplayHeight;
    UINT32   uiMovBgFileAddr;
    UINT32   uiMovBgFileSize;
    UINT32   uiBadBgFileAddr;
    UINT32   uiBadBgFileSize;
} PBRSV_DATA, *PPBRSV_DATA;

typedef struct
{
    UINT16 DstStart_X;
    UINT16 DstStart_Y;
    UINT16 DstWidth;
    UINT16 DstHeight;
} PB_SINGLE_DISP, *PPB_SINGLE_DISP;

//#NT#2010/04/12#Ben Wang -begin
#define  EXIF_NOT_EXIST     0xFFFFFFFF
//#NT#2010/04/12#Ben Wang -end

//return value of AppPlay_GetData(PLAY_QUALITY)
#define  Quality_Undefine           0       ///< use default quality value
#define  Quality_SuperHigh          1       ///< uiQuality as 0, 97~95
#define  Quality_High               2       ///< uiQuality as 94~90
#define  Quality_Economy            3       ///< uiQuality as 89~80

//parameter of AppPlay_SetData(PLAY_MOVE_EFFECT, xxx)
//#NT#2011/11/30#Lincy Lin -begin
//#NT#slide effect plug in
/*
#define SLIDE_EFFECT_NONE                       0x00    // No effect
#define SLIDE_EFFECT_UP2DOWN                    0x01    // Move image from up to down
#define SLIDE_EFFECT_DOWN2UP                    0x02    // Move image from down to up
#define SLIDE_EFFECT_LEFT2RIGHT                 0x03    // Move image from left to right
#define SLIDE_EFFECT_RIGHT2LEFT                 0x04    // Move image from right to left
#define SLIDE_EFFECT_R5HDRAPEUP2DOWN            0x11    // Move image from up to down with drape effect in 5 horizontal area
#define SLIDE_EFFECT_R2VIN2OUT                  0x51    // Move image from in to out
*/
#define PBX_SLIDE_EFFECT_NONE                       0

//#NT#2011/11/30#Lincy Lin -end

//return value of AppPlay_GetData(PLAY_LIGHTSOURCE)
#define LightUnknown              0
#define LightDaylight             1
#define LightFluorescent          2
#define LightTungsten             3
#define LightFlash                4
#define LightFineWeather          9
#define LightCloudyWeather        10
#define LightDaylightFluorescent  12
#define LightDayWhiteFluorescent  13
#define LightCoolWhiteFluorescent 14
#define LightWhiteFluorescent     15
#define LightStandardA            17
#define LightStandardB            18
#define LightStandardC            19

//return value of AppPlay_GetData(PLAY_EXIF_WB)
#define AutoWhite              0
#define ManualWhite            1

//File format
#define PLAYFMT_UNKNOWN                 0x0000
#define PLAYFMT_JPG                     0x0001
#define PLAYFMT_WAV                     0x0002
#define PLAYFMT_AVI                     0x0004
#define PLAYFMT_ASF                     0x0008
#define PLAYFMT_JPGMEMO                 0x0010
#define PLAYFMT_DPOF                    0x0020
#define PLAYFMT_READONLY                0x0040
#define PLAYFMT_MPG                     0x0080
#define PLAYFMT_MOVMJPG                 0x0100

extern VControl UIPlayObjCtrl;
extern UINT32 AppPlay_GetData(PLAY_DATA_SET attribute);
extern void AppPlay_SetData(PLAY_DATA_SET attribute, UINT32 value);
extern void UILockAll(BOOL bLock);
extern void UIDelAll(void);

#endif
