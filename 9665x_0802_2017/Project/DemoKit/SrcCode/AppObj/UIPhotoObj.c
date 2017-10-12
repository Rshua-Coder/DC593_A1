
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIPhotoObj.h"
#include "debug.h"
#include "ImgCaptureAPI.h"


//----------------------Prototype function declaration----------------------
//////////////////////////////////////////////////////////////////

INT32 OnShutter1Press(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    return NVTEVT_CONSUME;
}

INT32 OnShutter1Release(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    return NVTEVT_CONSUME;
}

INT32 OnCapture(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    return NVTEVT_CONSUME;
}

void AppPhoto_SetData(VControl *pCtrl, _UI_Item_sel item, UINT32 value)
{

}


EVENT_ENTRY UIPhotoObjCmdMap[] =
{
    {NVTEVT_CAPTURE,                OnCapture},
    {NVTEVT_SHUTTER1_PRESS,         OnShutter1Press},
    {NVTEVT_SHUTTER1_RELEASE,       OnShutter1Release},
    {NVTEVT_NULL,                   0},  //End of Command Map

};

CREATE_APP(UIPhotoObj,APP_SETUP)

