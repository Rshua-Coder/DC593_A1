#ifndef _UIMODE_H_
#define _UIMODE_H_

#include "type.h"
#include "UIInfo.h"

//FL_ModeIndex
typedef enum
{
    //#NT#2011/01/19#Lincy Lin -begin
    //#NT#
    #if (MODEKEY_FUNCTION == DISABLE)
    DSC_MODE_PHOTO_AUTO = 0,
    DSC_MODE_PHOTO_MANUAL,
    DSC_MODE_PHOTO_ASCN,
    DSC_MODE_PHOTO_PORTRAIT,
    DSC_MODE_PHOTO_SCENE,
    DSC_MODE_PHOTO_PANORAMA,
    DSC_MODE_PHOTO_STAB,
    DSC_MODE_MOVIE,
    #else
    DSC_MODE_PHOTO_AUTO = 0,
    DSC_MODE_PHOTO_MANUAL,
    DSC_MODE_PHOTO_ASCN,
    DSC_MODE_PHOTO_PANORAMA,
    DSC_MODE_PHOTO_SCENE,
    DSC_MODE_PHOTO_STAB,
    DSC_MODE_PHOTO_PORTRAIT,
    DSC_MODE_MOVIE,
    #endif
    //#NT#2011/01/19#Lincy Lin -end
    DSC_MODE_PLAYBACK,
    //#NT#2010/08/31#Steven feng -end
    //#NT#2010/03/02#JeahYen -begin
    DSC_MODE_USB_APP,
    //#NT#2010/07/21#Lily Kao -begin
    DSC_MODE_USB_APP_MSDC,
    DSC_MODE_USB_APP_SIDC,
    DSC_MODE_USB_APP_PRINTER,
    DSC_MODE_USB_APP_PCC,
    DSC_MODE_USB_APP_YOUTUBE,//#NT#2010/07/29#Lily Kao
    //#NT#2010/07/21#Lily Kao -end
    //#NT#2010/11/26#Jeah Yen -begin
    DSC_MODE_USB_FAST_CHARGE,
    //#NT#2010/11/26#Jeah Yen -end
    DSC_MODE_USB_VENDOR,
    //#NT#2010/03/02#JeahYen -end
    DSC_MODE_TOTAL_NUM
} DSC_MODE;


#define DSCMODE_SWITCH_FORCE            0       // DSC mode switch w/o animation intro
#define DSCMODE_SWITCH_ANI              1       // DSC mode switch with animation intro

#define DSCMODE_SWITCHDIR_DONT_CARE     0       // DSC mode switch direction don't care
#define DSCMODE_SWITCHDIR_L_R           1       // DSC mode switch in L -> R direction
#define DSCMODE_SWITCHDIR_R_L           2       // DSC mode switch in R -> L direction

extern void     UI_SetPrevMode(UINT32 value);
extern UINT32   UI_GetPrevMode(void);
extern void     UI_Switch_DscMode(UINT32 uiModeTransTo, UINT32 uiTransParam, UINT32 uiTransDir);

extern void     FlowMode_OnPowerOn(void);
extern void     FlowMode_OnPowerOff(void);
extern void     FlowMode_OnKeyMode(UINT32 uiDscMode);
extern void     FlowMode_OnUsbPlug(void);
extern void     FlowMode_OnUsbUnplug(void);


//#NT#2010/07/20#JeahYen -begin
extern BOOL     FlowMode_IsPowerOnPlayback(void);
//#NT#2010/09/28#Lincy Lin -begin
//#NT#Add check if write Photo frame Pstore when update FW
extern BOOL     FlowMode_IsPowerOnUpdateFW(void);
//#NT#2010/09/28#Lincy Lin -end
extern INT32    FlowMode_GetPowerOnMode(void);
extern INT32    FlowMode_IsPowerOnPreview(void);
//#NT#2010/07/20#JeahYen -end
//#NT#2010/12/22#Jeah Yen -begin
extern INT32    FlowMode_GetPowerOnMode_FromChargeMode(void);
//#NT#2010/12/22#Jeah Yen -end

#endif //_UIMODE_H_

