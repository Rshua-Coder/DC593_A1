

#include <stdio.h>

#include "type.h"
#include "debug.h"
#include "flash_alg.h"
#include "Flashlight_sample_int.h"
//#include "ImageAlgInfor.h"
#include "Sie_lib.h"
//#include "Sensor.h"
#include "Ae_Alg.h"
#include "Ae_Api.h"
#include "Flashlight_api.h"
#include "IPL_AlgInfor.h"
#include "Sensor.h"
#include "IPL_Cmd.h"
#include "Awb_api.h"
//#include "IPL_Hal_SIE.h"
//#include "IPL_Hal_Op_Int.h"
//#include "IPL_CtrlSample_Int.h"
#include "Utility.h"
#include "ImgStatLib.h"

#define __MODULE__ FLASHLIGHT
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

extern void AWB_setGain(UINT32 RG, UINT32 GG, UINT32 BG);

static FLASH_PARAM FlashParam={0};
FLASH_INPUTPARAM FlashInputParam;
UINT32 FlashWB_PrvRgain,FlashWB_PrvBgain,FlashWB_CapRgain,FlashWB_CapGgain=256,FlashWB_CapBgain;
UINT32 NoFlashRgain,NoFlashGgain,NoFlashBgain, NoFlashR, NoFlashG, NoFlashB, CapFlashR, CapFlashG, CapFlashB;

FLASH_PARAM* GetFlashParam(void)
{
    return &FlashParam;
}


void flash_RedEyeReduct(FLASH_PARA_SET FlashSetting_api)
{
    UINT32 DelayLine, RedEyeFlashWidth,IntrLvBtwnRedEyePrvFlsh;
    UINT32 FlashMode;
    FLASH_INFOR Flashlight_Setting;
    SENSOR_INFO SensorInfo;
    SENSOR_MODE CurSensorMode;

    FlashMode = IPL_AlgGetUIInfo(IPL_SEL_FLASHMODE);

    if(FlashParam.Enable && ((FlashMode == SEL_FLASHMODE_AUTOREDEYE)||(FlashMode == SEL_FLASHMODE_FORCEONREDEYE)))
    {
        //Sensor_GetCurMode(SENSOR_ID_1, &CurSensorMode);
        CurSensorMode = (SENSOR_MODE)IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE);
        Sensor_GetStatus(SENSOR_ID_1, CurSensorMode, &SensorInfo);

        RedEyeFlashWidth = REDEYE_WIDTH*(SensorInfo.Mode->SIEFreq/2/1000000);// preflash width(us) * pixel clock(MHz)

        DelayLine = 1;
        IntrLvBtwnRedEyePrvFlsh = 300;//ms

        Flashlight_Setting.DelayLine = DelayLine;
        Flashlight_Setting.DelayPixel = 0;
        Flashlight_Setting.TriggerPolarity = FLASH_HIGH_ACT;
        Flashlight_Setting.TriggerMode = FLASH_TRI_MODE_IMD;
        Flashlight_Setting.FlashWidth = RedEyeFlashWidth;

        //sie_waitVD(1);
        FlashSetting_api(&Flashlight_Setting);
        Delay_DelayMs(IntrLvBtwnRedEyePrvFlsh);
    }
}

/************************ Modified by Project ***********************************/

#define PreFlashY_n 14
#define MainTargetY_n 7
UINT16 NoFlash_R[Flash_CAWinY][Flash_CAWinX],NoFlash_G[Flash_CAWinY][Flash_CAWinX],NoFlash_B[Flash_CAWinY][Flash_CAWinX],PreFlash_R[Flash_CAWinY][Flash_CAWinX],PreFlash_G[Flash_CAWinY][Flash_CAWinX],PreFlash_B[Flash_CAWinY][Flash_CAWinX];

UINT32 PreFlashYTbl[PreFlashY_n] = {255,50,28,15,12,8,6,5,4,3,2,1,0,0}; // 0m,0.3m,0.45m,0.6m,0.75m,0.9m, 1.05m, 1.2m, 1.5m, 1.8m, 2.1m, 2.4m,2.7m infinity
UINT32 MainFlashYTbl[MainTargetY_n] = {60,50,40,30,20,10,0};
UINT32 MainFlashWidthTbl[MainTargetY_n][PreFlashY_n] ={
{9,17,22,27,32,40,48,57,78,90,115,140,150,170},
{9,16,20,25,29,35,42,48,60,75,90,110,120,130},
{9,14,18,22,26,32,35,43,53,64,75,84,90,100},
{9,12,16,20,22,27,30,33,41,52,57,62,70,75},
{9,11,13,17,18,22,25,27,33,40,44,48,50,55},
{9,9,11,13,15,17,18,21,23,27,30,33,35,38},
{9,9,9,9,9,9,9,9,9,9,9,9,9,9},
};
UINT32 FlashWeightTable[8][8] =
{
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1}
};

void flash_Evaluate(void)
{
    switch(IPL_AlgGetUIInfo(IPL_SEL_FLASHMODE))
    {
        case SEL_FLASHMODE_AUTO:
        case SEL_FLASHMODE_AUTOREDEYE:
            if(AE_GetPrvEVValue()<FLASH_TRIGGERPOINT) // LV5.0
                FlashParam.Enable = TRUE;
            else
                FlashParam.Enable = FALSE;
            break;
        case SEL_FLASHMODE_FORCEON:
        case SEL_FLASHMODE_FORCEONREDEYE:
        case SEL_FLASHMODE_NIGHTSTROBE:
            FlashParam.Enable = TRUE;
            if(AE_GetPrvEVValue()>FLASH_TRIGGERPOINT) // LV5.0
                FlashParam.FakeFlash = 1;
            else
                FlashParam.FakeFlash = 0;
            break;

        case SEL_FLASHMODE_FORCEOFF:
        default:
            FlashParam.Enable = FALSE;
            break;
    }
}

UINT16 PreFlashBlockY[FlashWinY][FlashWinX], BackGndBlockY[FlashWinY][FlashWinX], WindowCnt[FlashWinY][FlashWinX];

static FLASH_INFOR flash_PreFlashFlow_IGBT(FLASH_PARA_SET FlashSetting_api)
{
    // To be modified

    UINT32 i,j,DelayLine, PreFlashWidth, MainFlashWidth;
    //SENSOR_STATUS SensorStatus = {0},CapSensorStatus = {0};
    UINT32 FlashMode;
    FLASH_INFOR Flashlight_Setting, Pre_FlashSetting;
    SENSOR_INFO CurSensorInfo,CapSensorInfo;
    SENSOR_MODE CurSensorMode,CapSensorMode;
    EXPOSURE_SETTING ExpTSetting;
    GAIN_SETTING Gain;
    UINT32 NoFlashY=0, PreflashY=0, BlockIdxY, BlockIdxX;
    UINT32 PrvISO, PrvExpT, PrvIris, CapISO, CapExpT, CapIris;
    UINT32 SumR=0,SumG=0,SumB=0;

    FlashMode = IPL_AlgGetUIInfo(IPL_SEL_FLASHMODE);
    //Sensor_GetCurMode(SENSOR_ID_1, &CurSensorMode);
    CurSensorMode = (SENSOR_MODE)IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE);
    Sensor_GetStatus(SENSOR_ID_1, CurSensorMode, &CurSensorInfo);

    CapSensorMode = (SENSOR_MODE)IPL_AlgGetUIInfo(IPL_SEL_CAPSENMODE);
    Sensor_GetStatus(SENSOR_ID_1, CapSensorMode, &CapSensorInfo);

    flash_Evaluate();   //Evaluate flash status according to menu setting

#if 0//PREFLASH_CHARGE
        SensorInfor.FP_SetFlashCharge(1,0);
#endif
    if(FlashParam.Enable)
    {
        flash_RedEyeReduct(FlashSetting_api);
        //FlashInputParam.WidthPreFlash = 20;
        FlashInputParam.PreflashTbale = &PreFlashYTbl[0];
        FlashInputParam.ExpectYTbale = &MainFlashYTbl[0];
        FlashInputParam.FlashCurveTbale = &MainFlashWidthTbl[0][0];
        //FlashInputParam.PreviewISO = 200;
        FlashInputParam.PreviewExpoTime = CurSensorInfo.Mode->SenHD->Period*(CurSensorInfo.Mode->SenVD->Period)/(CurSensorInfo.Mode->Pclk/1000000);//UNIT is ms
#if 0// for rolling shutter
        FlashInputParam.CaptureExpoTime = CapSensorInfo.Mode->SenHD->Period*(CapSensorInfo.Mode->SenVD->Period)/(CapSensorInfo.Mode->Pclk/1000000);//UNIT is ms
#else// for Globe reset
        AE_GetCapAEArg(&CapISO,&CapExpT,&CapIris);
        FlashInputParam.CaptureExpoTime = CapExpT/2;//UNIT is ms
#endif
        //FlashInputParam.DelayLinePreFlash = 960;
        FlashInputParam.PreviewBinning = CurSensorInfo.Mode->biningRatio;
        FlashInputParam.CaptureBinning = CapSensorInfo.Mode->biningRatio;
        //FlashInputParam.PixelClock = 60;
        FlashInputParam.WeightingTable = &FlashWeightTable[0][0];
        FlashInputParam.CurveTbl_RowNum = PreFlashY_n;
        FlashInputParam.CurveTbl_ColNum = MainTargetY_n;
        FlashInputParam.BlockWinX = FlashWinX;
        FlashInputParam.BlockWinY = FlashWinY;
        FlashInputParam.TargetY = Flash_TARGET_Y;

        DelayLine = 0;//CurSensorInfo.Mode->SenVD->Period-5;
        PreFlashWidth = PRE_FLASH_WIDTH*(CurSensorInfo.Mode->SIEFreq/2/1000000);// preflash width(us) * pixel clock(MHz)

        Sensor_GetExpoSetting(SENSOR_ID_1, FlashInputParam.PreviewExpoTime, CurSensorMode, &ExpTSetting);

        AE_GetCapAEArg(&PrvISO,&PrvExpT,&PrvIris);

        if(PrvISO > 400)
            PrvISO = 400;
         else if(PrvISO < 100)
            PrvISO = 50;
         else
            PrvISO/=2;

        Sensor_GetGainSetting(SENSOR_ID_1, PrvISO, &Gain);

        Sensor_SetExpTime(SENSOR_ID_1, &ExpTSetting);
        sie_waitVD(1);
        Sensor_SetGain(SENSOR_ID_1, &Gain);

        sie_waitVD(2);
        Img_GetCA_Result(IPL_ID_1,&NoFlash_R[0][0],&NoFlash_G[0][0],&NoFlash_B[0][0],Flash_CAWinX*Flash_CAWinY);
        Pre_FlashSetting.DelayLine = DelayLine;
        Pre_FlashSetting.DelayPixel = 0;
        Pre_FlashSetting.FlashWidth = PreFlashWidth;
        Pre_FlashSetting.TriggerEnable = TRUE;
        Pre_FlashSetting.TriggerMode = FLASH_TRI_MODE_NORMAL;
        Pre_FlashSetting.TriggerPolarity = FLASH_HIGH_ACT;
        FlashSetting_api(&Pre_FlashSetting);
        sie_waitVD(2);
        Img_GetCA_Result(IPL_ID_1,&PreFlash_R[0][0],&PreFlash_G[0][0],&PreFlash_B[0][0],Flash_CAWinX*Flash_CAWinY);

        for(j=0;j<FlashWinY;j++)
        {
            for(i=0;i<FlashWinX;i++)
            {
                BackGndBlockY[j][i] = 0;
                PreFlashBlockY[j][i] = 0;
                WindowCnt[j][i] = 0;
            }
        }

        for(j=0;j<Flash_CAWinY;j++)
        {
            BlockIdxY = j*FlashWinY/Flash_CAWinY;
            for(i=0;i<Flash_CAWinX;i++)
            {
                BlockIdxX = i*FlashWinX/Flash_CAWinX;
                BackGndBlockY[BlockIdxY][BlockIdxX] += NoFlash_G[j][i]>>4;
                PreFlashBlockY[BlockIdxY][BlockIdxX] += PreFlash_G[j][i]>>4;
                WindowCnt[BlockIdxY][BlockIdxX] ++;
            }
        }

        for(j=0;j<FlashWinY;j++)
        {
            for(i=0;i<FlashWinX;i++)
            {
                BackGndBlockY[j][i] /= WindowCnt[j][i];
                PreFlashBlockY[j][i] /= WindowCnt[j][i];
                NoFlashY += BackGndBlockY[j][i];
                PreflashY += PreFlashBlockY[j][i];
            }
        }
        NoFlashY/=(FlashWinX*FlashWinY);
        PreflashY/=(FlashWinX*FlashWinY);
        DBG_IND("Noflash=%d Preflash=%d\r\n",NoFlashY,PreflashY);
        FlashParam.YNoFlash = NoFlashY;
        FlashParam.YPreFlash = PreflashY;
        FlashParam.WidthPreFlash = PreFlashWidth;

        FlashInputParam.BackGroundBlockY = &BackGndBlockY[0][0];
        FlashInputParam.PeflashBlockY = &PreFlashBlockY[0][0];

        MainFlashWidth = flash_AEConverge(&FlashInputParam);

        FlashParam.WidthMainFlash = MainFlashWidth*(CapSensorInfo.Mode->SIEFreq/2/1000000); // *Pixelclock(MHz)
        FlashParam.ISO_MainFlash = PrvISO; // Modify ISO by LV
        FlashParam.ExpoTime_MainFlash = FlashInputParam.CaptureExpoTime;
        FlashParam.DelayLineMainFlash = CapSensorInfo.Mode->SenVD->Period-10;

        AWB_GetColorGian(&NoFlashRgain,&NoFlashGgain,&NoFlashBgain);
        NoFlashG = FlashInputParam.FlashReturnInfo.CaptureBackLum;
        NoFlashR = NoFlashG*256/NoFlashRgain;
        NoFlashB = NoFlashG*256/NoFlashBgain;
        CapFlashG = FlashInputParam.FlashReturnInfo.CaptureFlashCompensateLum;
        CapFlashR = CapFlashG*256/FlashRgain;
        CapFlashB = CapFlashG*256/FlashBgain;
        FlashWB_CapRgain = (NoFlashG+CapFlashG)*256/(NoFlashR+CapFlashR);
        FlashWB_CapBgain = (NoFlashG+CapFlashG)*256/(NoFlashB+CapFlashB);
        FlashWB_CapGgain = 256;

        AWB_setGain(FlashWB_CapRgain,FlashWB_CapGgain,FlashWB_CapBgain);

        for(j=0;j<Flash_CAWinY;j++)
        {
            for(i=0;i<Flash_CAWinX;i++)
            {
                SumR += PreFlash_R[j][i]>>4;
                SumG += PreFlash_G[j][i]>>4;
                SumB += PreFlash_B[j][i]>>4;
            }
        }
        FlashWB_PrvRgain = SumG*256/SumR;
        FlashWB_PrvBgain = SumG*256/SumB;
    }
    //SetFlashFireStatus(FlashParam.Enable);
    Flashlight_Setting.DelayLine = FlashParam.DelayLineMainFlash;
    Flashlight_Setting.DelayPixel = 0;
    Flashlight_Setting.FlashWidth = FlashParam.WidthMainFlash;
    Flashlight_Setting.Flash_Cap_ExpoTime = FlashInputParam.CaptureExpoTime;
    Flashlight_Setting.Flash_Cap_ISO = FlashParam.ISO_MainFlash;
    Flashlight_Setting.TriggerEnable = FlashParam.Enable;
    Flashlight_Setting.TriggerMode = FLASH_TRI_MODE_NORMAL;
    Flashlight_Setting.TriggerPolarity = FLASH_HIGH_ACT;

    return Flashlight_Setting;
}

static FLASH_INFOR flash_PreFlashFlow_SCR(void)
{
    FLASH_INFOR Flashlight_Setting;
    SENSOR_INFO SensorInfo;
    SENSOR_MODE CapSensorMode;

    /***************** need to mofity to capture sensor mode ***************/
    //Sensor_GetCurMode(SENSOR_ID_1, &CapSensorMode);
    CapSensorMode = (SENSOR_MODE)IPL_AlgGetUIInfo(IPL_SEL_CAPSENMODE);
    Sensor_GetStatus(SENSOR_ID_1, CapSensorMode, &SensorInfo);

    FlashParam.WidthMainFlash = (SensorInfo.Mode->SIEFreq/2/1000000)* 1000; // 1ms

    if(AE_GetPrvEVValue()< 60)
        FlashParam.ISO_MainFlash = 200; // Modify ISO by LV
    else if(AE_GetPrvEVValue()<70)
        FlashParam.ISO_MainFlash = 100; // Modify ISO by LV
    else if(AE_GetPrvEVValue()<80)
        FlashParam.ISO_MainFlash = 50; // Modify ISO by LV

    //FlashParam.ExpoTime_MainFlash = CapSensorStatus.pSyncInfo->HD_total*(CapSensorStatus.pSyncInfo->VD_total-1)/CapSensorStatus.uiPclk;
    //FlashParam.DelayLineMainFlash = CapSensorStatus.pSyncInfo->VD_total-10;
    FlashParam.ExpoTime_MainFlash = SensorInfo.Mode->SenHD->Period*(SensorInfo.Mode->SenVD->Period-1)/SensorInfo.Mode->Pclk;
    FlashParam.DelayLineMainFlash = SensorInfo.Mode->SenVD->Period-10;

    Flashlight_Setting.DelayLine = FlashParam.DelayLineMainFlash;
    Flashlight_Setting.DelayPixel = 0;
    Flashlight_Setting.FlashWidth = FlashParam.WidthMainFlash;
    Flashlight_Setting.Flash_Cap_ExpoTime = FlashParam.ExpoTime_MainFlash;
    Flashlight_Setting.Flash_Cap_ISO = FlashParam.ISO_MainFlash;
    Flashlight_Setting.TriggerEnable = FlashParam.Enable;
    Flashlight_Setting.TriggerMode = FLASH_TRI_MODE_NORMAL;
    Flashlight_Setting.TriggerPolarity = FLASH_HIGH_ACT;

    return Flashlight_Setting;
}


FLASH_INFOR flash_PreFlashFlow(FLASH_PARA_SET FlashSetting_api)
{
    FLASH_INFOR MainFlash_Infor;

    DBG_IND("flash_PreFlashFlow Sample\r\n");
    flash_Evaluate();   //Evaluate flash status according to menu setting
    if(0)//(FLASH_LIGHT ==   _FLASH_SCR_)
        MainFlash_Infor = flash_PreFlashFlow_SCR();
    else
        MainFlash_Infor = flash_PreFlashFlow_IGBT(FlashSetting_api);
    // To be modified
    //SetFlashFireStatus(FlashParam.Enable);
    return MainFlash_Infor;
}

void Flash_GetColorGian(UINT32 *Rg,UINT32 *Gg,UINT32 *Bg)
{
    *Rg = FlashWB_CapRgain;
    *Gg = FlashWB_CapGgain;
    *Bg = FlashWB_CapBgain;
}


#include "AE_task.h"
#include "DxFlash.h"
extern void IPL_FCB_FlashTrig(FLASH_INFOR *Data);

UINT32 FlashTestTbl[40]={1, 2, 3, 4, 5, 7, 9, 11, 13, 15, 18, 22, 27, 33, 40, 48, 57, 67, 78, 90, 103, 117, 132, 148, 165, 183, 202, 222, 243, 265, 288, 304, 329, 355, 382, 410, 439, 479, 520, 600};
void flash_PreFlashFlow_Test(void)
{
    UINT32 i,j,DelayLine, PreFlashWidth, MainFlashWidth;
    //SENSOR_STATUS SensorStatus = {0},CapSensorStatus = {0};
    FLASH_INFOR Pre_FlashSetting;
    SENSOR_INFO CurSensorInfo;
    SENSOR_MODE CurSensorMode;
    EXPOSURE_SETTING ExpTSetting;
    GAIN_SETTING Gain;
    UINT32 Cnt, PreflashY=0, BlockIdxY, BlockIdxX;

    CurSensorMode = (SENSOR_MODE)IPL_AlgGetUIInfo(IPL_SEL_PRVSENMODE);
    Sensor_GetStatus(SENSOR_ID_1, CurSensorMode, &CurSensorInfo);

    //AE_Lock(0, TRUE, TRUE)
    AE_Pause(0, ENABLE);

    for(Cnt=0; Cnt<(sizeof(FlashTestTbl)/4);Cnt++)
    {
        MainFlashWidth = FlashTestTbl[Cnt];


        DrvFlash_ChargeFlash(TRUE, TRUE);
        for(i=0;i<5;i++)
        {
            Delay_DelayMs(1000);
            if(DrvFlash_IsFlashRdy())
                break;
        }
        DrvFlash_ChargeFlash(FALSE, FALSE);

        DelayLine = 0;//CurSensorInfo.Mode->SenVD->Period-5;
        PreFlashWidth = PRE_FLASH_WIDTH*(CurSensorInfo.Mode->SIEFreq/2/1000000);// preflash width(us) * pixel clock(MHz)

        FlashInputParam.PreviewExpoTime = CurSensorInfo.Mode->SenHD->Period*(CurSensorInfo.Mode->SenVD->Period)/(CurSensorInfo.Mode->Pclk/1000000);//UNIT is ms
        Sensor_GetExpoSetting(SENSOR_ID_1, FlashInputParam.PreviewExpoTime, CurSensorMode, &ExpTSetting);
        Sensor_GetGainSetting(SENSOR_ID_1, 200, &Gain);

        Sensor_SetExpTime(SENSOR_ID_1, &ExpTSetting);
        sie_waitVD(1);
        Sensor_SetGain(SENSOR_ID_1, &Gain);

        sie_waitVD(2);
        Pre_FlashSetting.DelayLine = DelayLine;
        Pre_FlashSetting.DelayPixel = 0;
        Pre_FlashSetting.FlashWidth = PreFlashWidth;
        Pre_FlashSetting.TriggerEnable = TRUE;
        Pre_FlashSetting.TriggerMode = FLASH_TRI_MODE_NORMAL;
        Pre_FlashSetting.TriggerPolarity = FLASH_HIGH_ACT;
        //FlashSetting_api(&Pre_FlashSetting);
        IPL_FCB_FlashTrig(&Pre_FlashSetting);
        sie_waitVD(2);
        Img_GetCA_Result(IPL_ID_1,&PreFlash_R[0][0],&PreFlash_G[0][0],&PreFlash_B[0][0],Flash_CAWinX*Flash_CAWinY);

        for(j=0;j<FlashWinY;j++)
        {
            for(i=0;i<FlashWinX;i++)
            {
                PreFlashBlockY[j][i] = 0;
                WindowCnt[j][i] = 0;
            }
        }

        for(j=0;j<Flash_CAWinY;j++)
        {
            BlockIdxY = j*FlashWinY/Flash_CAWinY;
            for(i=0;i<Flash_CAWinX;i++)
            {
                BlockIdxX = i*FlashWinX/Flash_CAWinX;
                PreFlashBlockY[BlockIdxY][BlockIdxX] += PreFlash_G[j][i]>>4;
                WindowCnt[BlockIdxY][BlockIdxX] ++;
            }
        }

        for(j=0;j<FlashWinY;j++)
        {
            for(i=0;i<FlashWinX;i++)
            {
                PreFlashBlockY[j][i] /= WindowCnt[j][i];
                PreflashY += PreFlashBlockY[j][i];
            }
        }
        PreflashY/=(FlashWinX*FlashWinY);
        DBG_ERR("Preflash=%d\r\n",PreflashY);


        Pre_FlashSetting.DelayLine = DelayLine;
        Pre_FlashSetting.DelayPixel = 0;
        Pre_FlashSetting.FlashWidth = MainFlashWidth*(CurSensorInfo.Mode->SIEFreq/2/1000000);
        Pre_FlashSetting.TriggerEnable = TRUE;
        Pre_FlashSetting.TriggerMode = FLASH_TRI_MODE_NORMAL;
        Pre_FlashSetting.TriggerPolarity = FLASH_HIGH_ACT;
        //FlashSetting_api(&Pre_FlashSetting);
        IPL_FCB_FlashTrig(&Pre_FlashSetting);
        sie_waitVD(2);
        Img_GetCA_Result(IPL_ID_1,&PreFlash_R[0][0],&PreFlash_G[0][0],&PreFlash_B[0][0],Flash_CAWinX*Flash_CAWinY);

        for(j=0;j<FlashWinY;j++)
        {
            for(i=0;i<FlashWinX;i++)
            {
                PreFlashBlockY[j][i] = 0;
                WindowCnt[j][i] = 0;
            }
        }

        for(j=0;j<Flash_CAWinY;j++)
        {
            BlockIdxY = j*FlashWinY/Flash_CAWinY;
            for(i=0;i<Flash_CAWinX;i++)
            {
                BlockIdxX = i*FlashWinX/Flash_CAWinX;
                PreFlashBlockY[BlockIdxY][BlockIdxX] += PreFlash_G[j][i]>>4;
                WindowCnt[BlockIdxY][BlockIdxX] ++;
            }
        }

        for(j=0;j<FlashWinY;j++)
        {
            for(i=0;i<FlashWinX;i++)
            {
                PreFlashBlockY[j][i] /= WindowCnt[j][i];
                PreflashY += PreFlashBlockY[j][i];
            }
        }
        PreflashY/=(FlashWinX*FlashWinY);
        DBG_ERR("Width=%d, Lum=%d\r\n",MainFlashWidth,PreflashY);
    }
    //SetFlashFireStatus(FlashParam.Enable);
    DBG_ERR("Flash test finish!!\r\n");
}


UINT32 Flash_WriteDebugInfo(UINT32 Addr)
{
    UINT32 i,j;
    Addr += sprintf((char *)Addr, "\r\nFlash Info------------------------------------------------------------------------------------------------------------------------\r\n");
    Addr += sprintf((char *)Addr, "Flash Enable = %d, Redeye = %d, ET = %d, ISO = %d\r\n",FlashParam.Enable,FlashParam.RedeyeEnable,FlashParam.ExpoTime_MainFlash,FlashParam.ISO_MainFlash);
    Addr += sprintf((char *)Addr, "NoFlashY = %d, PreflashY = %d, PreflashWidth = %d, Mainflash = %d, CapBackLum = %d, CapFlashCompensation = %d\r\n",FlashParam.YNoFlash, FlashParam.YPreFlash, FlashParam.WidthPreFlash, FlashParam.WidthMainFlash,FlashInputParam.FlashReturnInfo.CaptureBackLum,FlashInputParam.FlashReturnInfo.CaptureFlashCompensateLum);
    Addr += sprintf((char *)Addr, "NoflashRgain = %d, NoflashBgain = %d, PreFlashRgain = %d, PreflashBgain = %d, CapflashRgain = %d, CapflashBgain = %d\r\n",NoFlashRgain,NoFlashBgain,FlashWB_PrvRgain,FlashWB_PrvBgain,FlashWB_CapRgain,FlashWB_CapBgain);
    Addr += sprintf((char *)Addr, "Noflash R = %d, G = %d, B = %d, CapFlashR = %d, CapFlashG = %d, CapFlashB = %d\r\n",NoFlashR,NoFlashG,NoFlashB,CapFlashR,CapFlashG,CapFlashB);

    Addr += sprintf((char *)Addr, "Noflash CA:\r\n");
    for(j=0;j<Flash_CAWinY;j++)
    {
        for(i=0;i<Flash_CAWinX;i++)
        {
            Addr += sprintf((char *)Addr, "%4d %4d %4d\t", NoFlash_R[j][i]>>4,NoFlash_G[j][i]>>4,NoFlash_B[j][i]>>4);
        }
        Addr += sprintf((char *)Addr, "\r\n");
    }

    Addr += sprintf((char *)Addr, "Preflash CA:\r\n");
    for(j=0;j<Flash_CAWinY;j++)
    {
        for(i=0;i<Flash_CAWinX;i++)
        {
            Addr += sprintf((char *)Addr, "%4d %4d %4d\t", PreFlash_R[j][i]>>4,PreFlash_G[j][i]>>4,PreFlash_B[j][i]>>4);
        }
        Addr += sprintf((char *)Addr, "\r\n");
    }
    return Addr;
}


/********************************************************************************/


