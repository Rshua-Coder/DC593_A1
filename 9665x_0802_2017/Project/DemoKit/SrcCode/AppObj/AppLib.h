#ifndef _APPLIB_H_
#define _APPLIB_H_

//#include "Syscfg.h"
#include "ErrorNo.h"
#include "Debug.h"
#include "Utility.h"

/////////////////////////////////////////
// AppLib

//#include "AppCfg.h"
#include "UIFramework.h"
#include "UIControlEvent.h"

#include "AppControl.h"

#include "NvtAppPhotoEvent.h"
#include "NvtAppMovieEvent.h"
#include "NvtAppPlayEvent.h"
#include "NvtAppUsbEvent.h"
//#include "NVTUserCommand.h"


//#NT#2009/10/29#Jeah Yen - begin
typedef enum
{
    APP_BASE = APP_TYPE_MIN,
    APP_SETUP,
    APP_PLAY,
    APP_PHOTO,
    APP_MOVIEPLAY,
    APP_MOVIEREC,
    APP_PRINT,
    APP_PCC,
    APP_MSDC,
    //#NT#2010/12/15#Lily Kao -begin
    APP_USBCHARGE,
    //#NT#2010/12/15#Lily Kao -end
    APP_TYPE_MAX
}
APP_TYPE;
//#NT#2009/10/29#Jeah Yen - end


#endif //_APPLIB_H_
