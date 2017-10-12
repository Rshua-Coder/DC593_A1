#ifndef UI_PHOTOOBJ_H
#define UI_PHOTOOBJ_H

#include "UIFramework.h"
#include "AppLib.h"
#include "PrjCfg.h"


/*
#define ZOOM_IF_DIGITAL 1
#define ZOOM_IF_OPTICAL 0
// Zoom Control
#define UI_ZOOM_CTRL_STOP               0
#define UI_ZOOM_CTRL_IN                 1
#define UI_ZOOM_CTRL_OUT                2
#define UI_ZOOM_CTRL_RESET_DZOOM        3

#define UI_DZOOM_IDX_MIN                10
#define DZOOM_STATUS_OFF                0
#define DZOOM_STATUS_ON                 1


extern void   UI_ZoomControl(UINT32 uiZoomCtrl, BOOL bDZoomEnable);
extern void   Set_ZoomIFIndex(UINT16 ubIndex);
extern UINT32 Get_ZoomIFIndex(void);
extern void   SetDzoomResetFlag(BOOL bReset);
extern BOOL   GetDzoomResetFlag(void);
extern void   Set_DzoomStatusIndex(UINT16 ubIndex);
extern UINT32 Get_DzoomStatusIndex(void);
extern BOOL   Get_DzoomEnable(void);
*/

#if 0
extern UINT32 Get_SizeValue(UINT32 uhIndex);
extern UINT32 Get_QualityValue(UINT32 uhIndex);
extern UINT32 Get_ColorValue(UINT32 uhIndex);
extern BOOL Get_ZoomStatus(void);
#endif

#if (!_DEMO_TODO)
typedef struct _AlgMsgInfo
{
    int err;
}
AlgMsgInfo;
typedef UINT32 _UI_Item_sel;
#endif
extern AlgMsgInfo * GetAlgMsgInfo(void);
extern void SetAlgMsgInfo(AlgMsgInfo * algMsgInfo);
//#NT#2009/12/14#Photon Lin -begin
extern void AppPhoto_SetData(VControl *pCtrl, _UI_Item_sel item, UINT32 value);
//#NT#2009/12/14#Photon Lin -end
//#NT#2009/10/29#Jeah Yen - begin
typedef enum
{
    NVTEVT_PHOTO_MIN    = APP_EVENT_MASK + 0x00000200,
    //#NT#2009/11/26#Photon Lin -begin
    NVTEVT_SHUTTER1_PRESS,
    NVTEVT_SHUTTER1_RELEASE,
    NVTEVT_CAPTURE,
    //#NT#2009/11/26#Photon Lin -end
    NVTEVT_PHOTO_MAX
}
NVT_PHOTO_EVENT;
//#NT#2009/10/29#Jeah Yen - end


extern VControl UIPhotoObjCtrl;


#endif
