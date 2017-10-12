#include "UIAppMovie.h"
#include "ae_api.h"
#include "MovieInterface.h"

#define __MODULE__          UIMovie3DNR
//#define __DBGLVL__ 0        //OFF mode, show nothing
//#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"
#include "ae_api.h"

static INT32    iAE_AVG_old=30;
static UINT32   ui_KeepCnt=0;

//-------------------------------------------------------------------------------------------------
//
// H.264 3DNR parameters adjustment for MN34110
//
//-------------------------------------------------------------------------------------------------
#if (_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_)//2013.11.18 LS500W
UINT32  ui3DNRLevel;
UINT32 UIMovie_3DNRAdjust(void)
{
    UINT32  uiISO1, uiExpT1, uiIris1;
//    UINT32  ui3DNRLevel;
    INT32   iAE_AVG;

    iAE_AVG=AE_getCurLum(12);

    AE_GetPrvAEArg(&uiISO1, &uiExpT1, &uiIris1);

    if((abs(iAE_AVG_old-iAE_AVG)>(10*16))|(ui_KeepCnt))// scene change detection
    {
        ui3DNRLevel = H264_3DNR_LV2;
        ui_KeepCnt++;
        if(ui_KeepCnt>3)
            {ui_KeepCnt=0;}
    }
    else
    {
        if(uiISO1<101)
        {
            ui3DNRLevel = H264_3DNR_LV0;
        }
        else  if(uiISO1<201)
        {
            ui3DNRLevel = H264_3DNR_LV1;
        }
        else  if(uiISO1<301)
        {
            ui3DNRLevel = H264_3DNR_LV2;
        }
        else  if(uiISO1<1201)
        {
            ui3DNRLevel = H264_3DNR_LV3;
        }
        else  if(uiISO1<2001)
        {
            ui3DNRLevel = H264_3DNR_LV4;
        }
        else
        {
            if(iAE_AVG<120)
            {
                ui3DNRLevel = H264_3DNR_LV6;
            }
            else
            {
                ui3DNRLevel = H264_3DNR_LV5;
            }
        }
    }
    iAE_AVG_old=iAE_AVG;

    return ui3DNRLevel;
}

#elif 0//(_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_)//G1
UINT32 UIMovie_3DNRAdjust(void)
{
    UINT32  uiISO1, uiExpT1, uiIris1;
    UINT32  ui3DNRLevel;
    INT32   iAE_AVG;

    iAE_AVG=AE_getCurLum(12);

    AE_GetPrvAEArg(&uiISO1, &uiExpT1, &uiIris1);

    if((abs(iAE_AVG_old-iAE_AVG)>(10*16))|(ui_KeepCnt))// scene change detection
    {
        ui3DNRLevel = H264_3DNR_WEAKEST;
        ui_KeepCnt++;
        if(ui_KeepCnt>3)
            {ui_KeepCnt=0;}
    }
    else
    {

        if(uiISO1<301)
        {
            ui3DNRLevel = H264_3DNR_WEAK;
        }
        else if(uiISO1<401)
        {
            ui3DNRLevel = H264_3DNR_NORMAL;
        }
        else if(uiISO1<501)
        {
            ui3DNRLevel = H264_3DNR_STRONG;
        }
        else if(uiISO1<601)
        {
            ui3DNRLevel = H264_3DNR_STRONGEST;
        }
        else if(uiISO1<701)
        {
            ui3DNRLevel = H264_3DNR_STRONGEST2;
        }
        else if(uiISO1<801)
        {
            ui3DNRLevel = H264_3DNR_STRONGEST3;
        }
        else  if(uiISO1<901)
        {
            ui3DNRLevel = H264_3DNR_STRONGEST3;
        }
        else  if(uiISO1<1601)
        {
            if(iAE_AVG<150)
            {
                ui3DNRLevel = H264_3DNR_STRONGEST5;
            }
            else if(iAE_AVG<240)
            {
                ui3DNRLevel = H264_3DNR_STRONGEST4;
            }
            else
            {
                ui3DNRLevel = H264_3DNR_STRONGEST3;
            }
        }
        else
        {
            ui3DNRLevel = H264_3DNR_STRONGEST3;
        }

        //ui3DNRLevel =H264_3DNR_STRONGEST3;//adrian0611 test

#if 0
        if(uiISO1<301)
        {
            ui3DNRLevel = H264_3DNR_WEAK;
        }
        else if(uiISO1<601)
        {
            ui3DNRLevel = H264_3DNR_NORMAL;
        }
        else if(uiISO1<1001)
        {
            ui3DNRLevel = H264_3DNR_STRONG;
        }
        else
        {
            if(iAE_AVG<150)
            {
                ui3DNRLevel = H264_3DNR_STRONGEST3;
            }
            else if(iAE_AVG<240)
            {
                ui3DNRLevel = H264_3DNR_STRONGEST2;
            }
            else
            {
                ui3DNRLevel = H264_3DNR_STRONGEST;
            }
        }
#endif
    }
    iAE_AVG_old=iAE_AVG;

    return ui3DNRLevel;
}

//-------------------------------------------------------------------------------------------------
//
// H.264 3DNR parameters adjustment for AR0330
//
//-------------------------------------------------------------------------------------------------
#elif (_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_ || _SENSORLIB_ == _SENSORLIB_CMOS_AR0330P_ || _SENSORLIB_ == _SENSORLIB_CMOS_OV2710_  || _SENSORLIB_ == _SENSORLIB_CMOS_IMX179_ || _SENSORLIB_ == _SENSORLIB_CMOS_F22P_)
UINT32 UIMovie_3DNRAdjust(void)
{
    UINT32  uiISO1, uiExpT1, uiIris1;
    UINT32  ui3DNRLevel;
    INT32   iAE_AVG;

    iAE_AVG=AE_getCurLum(12);

    AE_GetPrvAEArg(&uiISO1, &uiExpT1, &uiIris1);

    if((abs(iAE_AVG_old-iAE_AVG)>(10*16))|(ui_KeepCnt))// scene change detection
    {
        ui3DNRLevel = H264_3DNR_WEAKEST;
        ui_KeepCnt++;
        if(ui_KeepCnt>3)
            {ui_KeepCnt=0;}
    }
    else
    {
        if(uiISO1<301)
        {
            ui3DNRLevel = H264_3DNR_WEAK;
        }
        else if(uiISO1<501)
        {
            ui3DNRLevel = H264_3DNR_NORMAL;
        }
        else if(uiISO1<601)
        {
            ui3DNRLevel = H264_3DNR_STRONG;
        }
        else if(uiISO1<701)
        {
            ui3DNRLevel = H264_3DNR_STRONGEST;
        }
        else if(uiISO1<801)
        {
            ui3DNRLevel = H264_3DNR_STRONGEST2;
        }
        else  if(uiISO1<901)
        {
            ui3DNRLevel = H264_3DNR_STRONGEST3;
        }
        else
        {
            ui3DNRLevel = H264_3DNR_STRONGEST3;
        }
    }
    iAE_AVG_old=iAE_AVG;

    return ui3DNRLevel;
}

//-------------------------------------------------------------------------------------------------
//
// H.264 3DNR parameters adjustment for other sensors
//
//-------------------------------------------------------------------------------------------------
#else
UINT32 UIMovie_3DNRAdjust(void)
{
    UINT32  uiISO1, uiExpT1, uiIris1;
    UINT32  ui3DNRLevel;

    AE_GetPrvAEArg(&uiISO1, &uiExpT1, &uiIris1);

    if(uiISO1<301)
    {
        ui3DNRLevel = H264_3DNR_WEAK;
    }
    else if(uiISO1<601)
    {
        ui3DNRLevel = H264_3DNR_NORMAL;
    }
    else if(uiISO1<1001)
    {
        ui3DNRLevel = H264_3DNR_STRONG;
    }
    else
    {
        ui3DNRLevel = H264_3DNR_STRONGEST;
    }

    return ui3DNRLevel;
}

#endif
