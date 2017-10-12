////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "TPInfo.h"
#include "PStore.h"

//#include "GSensorAPI.h"
//#include "NVTUserCommand.h"

//------------------------------------------------------------

typedef struct
{
    GX_TOUCH_CALI   TouchCali;
    UINT32 uiVerifyCode;
}TPCALI_INFO;

#define PS_TP_CALI              "TPCA"
#define  TPCALI_VERIFICATION    0x54504532 //"TPE2"

TPCALI_INFO     TPCaliInfo;
BOOL bTPReady = 0;

void Save_TPInfo(void)
{
    PPSTORE_SECTION_HANDLE  pSection;

    debug_msg("[Init_MenuInfo][PStore write TPCali]\r\n");
    //save data to PStore
    if ((pSection = PStore_OpenSection(PS_TP_CALI, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        TPCaliInfo.uiVerifyCode= TPCALI_VERIFICATION;
        PStore_WriteSection((UINT8 *)&TPCaliInfo, 0, sizeof(TPCaliInfo), pSection);
        PStore_CloseSection(pSection);
    }
    else
    {
        debug_err(("[TPCalibrate][PStore Write TP calibration param fail]\r\n"));
    }
}

void Load_TPInfo(void)
{
    PPSTORE_SECTION_HANDLE  pSection;

    debug_msg("[Init_MenuInfo][PStore read TPCali]\r\n");
    // Read TP calibration info from PStore
    if ((pSection = PStore_OpenSection(PS_TP_CALI, PS_RDWR)) != E_PS_SECHDLER)
    {
        PStore_ReadSection((UINT8 *)&TPCaliInfo, 0, sizeof(TPCALI_INFO), pSection);
        PStore_CloseSection(pSection);
        bTPReady = 1;
    }
    else
        debug_err(("^R Pstore of TouchPanel open to read FAIL\r\n"));

}

void Init_TPInfo(void)
{
    debug_msg("[Init_MenuInfo][PStore update TPCali]\r\n");
    // Read TP calibration info from PStore

    if (bTPReady)
    {
        #if 0//debug only
        INT32   Xs0, Ys0, Xs1,Ys1, Xs2, Ys2;
        INT32   Xd0, Yd0, Xd1,Yd1, Xd2, Yd2;
        Xs0 = (INT32)TPCaliInfo.TouchCali.TouchPoint[0].x;
        Ys0 = (INT32)TPCaliInfo.TouchCali.TouchPoint[0].y;
        Xs1 = (INT32)TPCaliInfo.TouchCali.TouchPoint[1].x;
        Ys1 = (INT32)TPCaliInfo.TouchCali.TouchPoint[1].y;
        Xs2 = (INT32)TPCaliInfo.TouchCali.TouchPoint[2].x;
        Ys2 = (INT32)TPCaliInfo.TouchCali.TouchPoint[2].y;
        Xd0 = (INT32)TPCaliInfo.TouchCali.UIPoint[0].x;
        Yd0 = (INT32)TPCaliInfo.TouchCali.UIPoint[0].y;
        Xd1 = (INT32)TPCaliInfo.TouchCali.UIPoint[1].x;
        Yd1 = (INT32)TPCaliInfo.TouchCali.UIPoint[1].y;
        Xd2 = (INT32)TPCaliInfo.TouchCali.UIPoint[2].x;
        Yd2 = (INT32)TPCaliInfo.TouchCali.UIPoint[2].y;
        debug_msg("UIPoint 1-(%d,%d), 2-(%d,%d), 3-(%d,%d),\r\n",Xd0,Yd0,Xd1,Yd1,Xd2,Yd2);
        debug_msg("TouchPoint 1-(%d,%d), 2-(%d,%d), 3-(%d,%d),\r\n",Xs0,Ys0,Xs1,Ys1,Xs2,Ys2);
        debug_msg("TPCaliInfo.uiVerifyCode = 0x%x\r\n",TPCaliInfo.uiVerifyCode);
        #endif
        if( TPCaliInfo.uiVerifyCode == TPCALI_VERIFICATION)
        {
         //open calibration for ES panel
            GxTouch_InitCalibration(&TPCaliInfo.TouchCali);
        }
        else
        {
            debug_err(("^R Calbration data of TP is invalid, Using default value\r\n"));
        }
    }
}

void Update_TPInfo(GX_TOUCH_CALI* pTouchCali)
{
    {
        TPCaliInfo.TouchCali = *pTouchCali;
    }
}


