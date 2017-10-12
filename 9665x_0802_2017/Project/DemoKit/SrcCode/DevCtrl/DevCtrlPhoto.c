/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DevCtrlPhoto.c
    @ingroup    mIPRJAPCfg

    @brief      Control device for photo mode
                Control LCD/LCD backlight on/off, CCD power on/off.
                Lens...

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{


#include "SysCommon.h"
#include "SysMain.h"
#include "DxSensor.h"
//#include "GxSystem.h"
//#include "GxOutput.h"
//#include "GxPower.h"
//#include "GxFlash.h"

#include "DeviceCtrl.h"
#include "ide.h"
#include "Lens.h"
#include "Utility.h"

#include "UIPhotoInfo.h"
#include "UIAppPhoto.h"
#include "UIPhotoMapping.h"
#include "UIGraphics.h"

#include "UIMode.h"
#include "UIInfo.h"


//#NT#2010/10/15#Jeah Yen -begin
#include "UIAppMovie.h"
//#include "UIMovieInfo.h"
//#NT#2010/10/15#Jeah Yen -end

//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
extern BOOL g_bPreviewFirstTime;
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
//int bFirstStable =0;


//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
void DevCtrl_ModePhoto_FastSensor(void)
{
    //#Avoid to see ghost of "splash logo" overlay with new VDO screen of first mode
    idec_setV1En(IDE_ID_1, FALSE);
    //idec_setV2En(IDE_ID_1, FALSE);

    DevCtrl_ModePhoto();
}
void DevCtrl_ModePhoto_FastLens(void)
{
    DevCtrl_Lens(TRUE);
}
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end

void DevCtrl_ModePhoto(void)
{

    #if _DEMO_TODO
    ImageAlgInfo *AlgInfo;

    //進Playback,Sensor會進Sensor的Sleep Mode, 所以這邊要 Wake Up
    Sensor_OnSystem(SYSTEM_CMD_SLEEPLEAVE);

    //#NT#2010/10/15#Jeah Yen -begin
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_MOVIEGDC,1, UI_GetData(UI_MovieGdcIndex));
    //#NT#2010/10/15#Jeah Yen -end

    #if (GDC_FUNC == ENABLE)
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_GDC,1, TRUE);
    #endif

        if (UI_GetData(UI_NextMode) == DSC_MODE_PHOTO_PANORAMA)
        {
            //Photo_SetImageRatio(_Ratio_4_3);
            Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_IMAGE_RATIO,1,_Ratio_4_3);
        }
        else
        {
            //debug_msg("Photo_GetUserIndex(PHOTO_USR_SIZE) =%d\r\n",Photo_GetUserIndex(PHOTO_USR_SIZE));
            if (UI_GetData(UI_IsPlaybackInSCN) != TRUE)
            {
                if (Get_SizeValue(Photo_GetUserIndex(PHOTO_USR_SIZE)) == _IMGSize_12M_32 || Get_SizeValue(Photo_GetUserIndex(PHOTO_USR_SIZE)) == _IMGSize_10M_32)
                {
                    //Photo_SetImageRatio(_Ratio_3_2);
                    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_IMAGE_RATIO,1,_Ratio_3_2);
                }
                else if (Get_SizeValue(Photo_GetUserIndex(PHOTO_USR_SIZE)) == _IMGSize_10M_169 || Get_SizeValue(Photo_GetUserIndex(PHOTO_USR_SIZE)) == _IMGSize_9M_169)
                {
                    //Photo_SetImageRatio(_Ratio_16_9);
                    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_IMAGE_RATIO,1,_Ratio_16_9);
                }
                else
                {
                    //Photo_SetImageRatio(_Ratio_4_3);
                    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_IMAGE_RATIO,1,_Ratio_4_3);
                }
            }
        }

    if (DevCtrl_GetIPLMode() != DEVCTRL_IPLMODE_PREVIEW)
    {
        UINT32 uiPrevMode = System_GetState(SYS_STATE_PREVMODE);
        if(bFirstStable)
        {
            if((uiPrevMode != PRIMARY_MODE_PHOTO)
            && (uiPrevMode != PRIMARY_MODE_MOVIE))
            {
                //Switch PHOTO/MOVIE mode between other modes

                //#NT#2010/03/09#Jeah Yen -begin
                UI_ResetDisplay(); //To avoid garbage on screen, hide all display
                //#NT#2010/03/09#Jeah Yen -end
            }
        }
    //#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
        if(!g_bPreviewFirstTime)
        {
            //TM_Begin(DEVCTRL_LENS);
            //#NT#2010/12/14#Lincy Lin -begin
            //#NT#Add lens error check
            if (DevCtrl_Lens(TRUE) == FALSE)
            {
                //debug_msg("lens error close\r\n");
                return;
            }
            //#NT#2010/12/14#Lincy Lin -end
            //TM_End(DEVCTRL_LENS);
        }
    //#NT#2010/04/15#Jeah Yen -FASTBOOT- -end

        // Set to preview mode
        DevCtrl_SetIPLMode(DEVCTRL_IPLMODE_PREVIEW);
        if(!bFirstStable)
        {
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
            //TM_Begin(IPL_PHOTO);
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
        }

        AlgInfo = UI_GetParameter();
        // Start zoom initialization
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
        PhotoDisplay_SetMode(DISPLAY_MODE_PREVIEW, FALSE);
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end

    }
    else
    {
        //#NT#2010/12/13#Lincy Lin -begin
        if (UI_GetData(FL_NextMode) == DSC_MODE_PHOTO_PANORAMA || UI_GetData(FL_PreMode) == DSC_MODE_PHOTO_PANORAMA)
        {
            DevCtrl_SetIPLMode(DEVCTRL_IPLMODE_OFF);
            DevCtrl_SetIPLMode(DEVCTRL_IPLMODE_PREVIEW);
        }
        //#NT#2010/12/13#Lincy Lin -end
        if (UI_GetData(FL_Dzoom) == DZOOM_OFF || UI_GetData(FL_NextMode) == DSC_MODE_PHOTO_PANORAMA || UI_GetData(FL_NextMode) == DSC_MODE_PHOTO_ASCN)
        {
            //If DZoom config is set to off at startup, DZoom reset can NOT be proceeded until IPL is ready
            Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_ZOOM,2, UI_ZOOM_CTRL_RESET_DZOOM, FALSE);
        }

    }
    #else
    UINT32 uiPrevMode = System_GetState(SYS_STATE_PREVMODE);
    debug_msg("^MMODE %d->%d\r\n", uiPrevMode, PRIMARY_MODE_PHOTO);
    if(((INT32)uiPrevMode != PRIMARY_MODE_PHOTO)
    && ((INT32)uiPrevMode != PRIMARY_MODE_MOVIE))
    {
        DrvSensor_TurnOnPower();
    }
    DevCtrl_Lens(TRUE);
    #endif
}

//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
void DevCtrl_ModePhoto_WaitFinish(void)
{
#if _DEMO_TODO
        TM_Begin(CCD_WAITPHOTO);
        PhotoDisplay_WaitIdle(TRUE);
        TM_End(CCD_WAITPHOTO);
#if (WAITPHOTO_FUNCTION != ENABLE)
        if(!bFirstStable)
        {
            idec_setV1En(IDE_ID_1, TRUE);
            idec_setV2En(IDE_ID_1, FALSE);
            TM_End(IPL_PHOTO);
            TM_End(OFF_TO_PREVIEW);
            bFirstStable = 1;
        }
#else
        TM_End(IPL_PHOTO);
        TM_End(OFF_TO_PREVIEW);
#endif
#endif
}
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
//@}
