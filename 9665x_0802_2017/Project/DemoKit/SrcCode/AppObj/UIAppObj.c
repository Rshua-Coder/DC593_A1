#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "debug.h"
#include "AppLib.h"

//#NT#2009/12/24#Jeah Yen - begin
////////////////////////////////////////////////////////////

extern VControl UIPlayObjCtrl;
extern VControl UIPhotoObjCtrl;
extern VControl UIMoviePlayObjCtrl;
extern VControl UIMovieRecObjCtrl;
#if (UI_STYLE!=UI_STYLE_DRIVER)
extern VControl UIPrinterObjCtrl;
#endif

//the order of this array should be the same as APP_TYPE
VControl* gAppCtrlTypeList[APP_TYPE_MAX-APP_TYPE_MIN]=
{
    0,                   //APP_BASE
    0,                   //APP_SETUP
    &UIPlayObjCtrl,      //APP_PLAY
    &UIPhotoObjCtrl,     //APP_PHOTO
    &UIMoviePlayObjCtrl, //APP_MOVIEPLAY
    &UIMovieRecObjCtrl,  //APP_MOVIEREC
#if (UI_STYLE!=UI_STYLE_DRIVER)
    &UIPrinterObjCtrl,   //APP_PRINT
#else
    0,
#endif
    0,                   //APP_PCC,
    0,                   //APP_MSDC
};
//#NT#2009/12/24#Jeah Yen - end

