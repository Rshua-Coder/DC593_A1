////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
//#include "NVTUserCommand.h"
#include "PStore.h"
#include "UIInfo.h"
////////////////////////////////////////////////////////////////////////////////
#if _DEMO_TODO
#include "CalibrationAPI.h"
#include "Cache.h"
#endif

#define CAL_CONVERT     DISABLE

#define PS_CAL_DATA             "CALD"
#define PS_LENS_DATA            "LEND"

#if (CAL_CONVERT == ENABLE)
typedef struct _CAL_DATA_OLD{
    UINT Iris_Test_Status;
    UINT MShtr_Test_Status;
    UINT AFE_Test_Status;
    UINT Zoom_SpeedTest_Status;
    UINT Zoom_StepMissTest_Status;
    UINT Focus_StepMissTest_Status;
    UINT NFocus_Status;
    UINT FFocus_Status;
    UINT Far_Focus[15];
    UINT Near_Focus[15];
    UINT Focus_Test_Status;
    UINT ISO_Status;
    UINT Agc_ISO_100;
    UINT Mshutter_Status[8];
    UINT FTP_Status;
    UINT AWB_Status;
    AWB_Struct AWB;
    AWB_Struct AWB_Prv;
    DP_Struct DP_Status;
    UINT Shading_Status;
    UINT ShadingXcenter;
    UINT ShadingYcenter;
    UINT ShadingDecayRatio;
    UINT FlashAE_Status;
    UINT Flash_Y_Avg;
    UINT FlashAWB_Status;
    AWB_Struct AWB_Flash;
    UINT OB_Status;
    UINT Agc_ISOMin;
    UINT Mshutter[8];
    UINT DP_Cap_Num[8];
    UW Defect_Pixel[DP_Prv_MAX+DP_Cap_MAX];
    UW DP_Dzoom[DP_Dzoom_MAX];
    UINT FlashTriggerPoint;
    UINT OB_Min;
    UINT Shading_Gain[5];
    UINT Cap_Field_Num;
    UINT Cap_Field_DP_Cnt[8];
    USHORT DP_Cap_Dark_Histo[255];
    USHORT Flash_ADC_Val[2][8];
    UINT Flash_Max_Width_Y;
    UINT8 SerialNumber[CAL_SERIAL_LEN];
    UINT RTC_Test_Status;
    UINT EmptyVolt_Test_Status;
    UINT EmptyVolt;
    UINT EmptyVolt_Delta; //ricky@1211/2007
    UINT uiLanguageIndex;
    UINT uiTVModeIndex;
    UINT Burnin_Status;
    UINT Zoom_Stop2PR_Status;//JJ0228
    UINT uiZoomStop2PR;//JJ0228
    INT16 iRedundancy[2];//maintain the order while extend the size
    UINT uiFocusOffsetStatus;//JJ0324
    UINT Language_Status; //ricky@0401/2008
    UINT Far_Focus2[15];// AO asked to save af infor. for two iris as well, this array is for small-iris while the original array is for big-iris
    UINT Near_Focus2[15];// AO asked to save af infor. for two iris as well, this array is for small-iris while the original array is for big-iris
    INT16 iFocusOffset[16];//JJ0324

    UINT EmptyVolt_Test_Ver;
    UINT uiZoomPR2Tele;
    UINT DL_Status;
    UINT Defect_line[5];
}CAL_DATA_OLD;

volatile CAL_DATA_OLD gCalData_old;
#endif

#define VOID_PTR       (void*)(UINT32)

void Save_CalInfo(void);

void Load_CalInfo(void)
{
#if _DEMO_TODO
    PPSTORE_SECTION_HANDLE  pSection;
    //need to write buffer
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
    debug_ind(("^YRead SENSORINFO\r\n"));
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
    CPUCleanInvalidateDCacheBlock((UINT32)&gCalData, (UINT32)&gCalData + sizeof(gCalData));
    CPUDrainWriteBuffer();
    if ((pSection = PStore_OpenSection(PS_CAL_DATA, PS_RDWR)) != E_PS_SECHDLER)
    {
        PStore_ReadSection((UINT8 *)&gCalData, 0, sizeof(gCalData), pSection);
        PStore_CloseSection(pSection);
        debug_ind(("Read Adjust Data from Nand success\r\n"));
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
        if(gCalData.Version[0] != 'V')
        {
            debug_msg("^YCAL version = OLD!\r\n");
#if (CAL_CONVERT == ENABLE)
            debug_msg("^Ystart convert\r\n");
            if ((pSection = PStore_OpenSection(PS_CAL_DATA, PS_RDWR)) != E_PS_SECHDLER)
            {
                PStore_ReadSection((UINT8 *)&gCalData_old, 0, sizeof(gCalData_old), pSection);
                PStore_CloseSection(pSection);
            }
            strcpy(VOID_PTR(gCalData.Version), CAL_VER);
            gCalData.Sys_Adjust.uiLanguageIndex = gCalData_old.uiLanguageIndex;
            gCalData.Sys_Adjust.Language_Status = gCalData_old.Language_Status;
            gCalData.Sys_Adjust.uiTVModeIndex = gCalData_old.uiTVModeIndex;
            gCalData.Sys_Adjust.Burnin_Status = gCalData_old.Burnin_Status;
            gCalData.Sys_Adjust.RTC_Test_Status = gCalData_old.RTC_Test_Status;
            gCalData.Sys_Adjust.EmptyVolt_Test_Status = gCalData_old.EmptyVolt_Test_Status;
            gCalData.Sys_Adjust.EmptyVolt = gCalData_old.EmptyVolt;
            gCalData.Sys_Adjust.EmptyVolt_Delta = gCalData_old.EmptyVolt_Delta;
            gCalData.Sys_Adjust.EmptyVolt_Test_Ver = gCalData_old.EmptyVolt_Test_Ver;
            memcpy(VOID_PTR(&(gCalData.Sys_Adjust.SerialNumber)), VOID_PTR(&(gCalData_old.SerialNumber)), sizeof(UINT8)*CAL_SERIAL_LEN);
            gCalData.Len_Adjust.Iris_Test_Status = gCalData_old.Iris_Test_Status;
            gCalData.Len_Adjust.MShtr_Test_Status = gCalData_old.MShtr_Test_Status;
            gCalData.Len_Adjust.FFocus_Status = gCalData_old.FFocus_Status;
            gCalData.Len_Adjust.NFocus_Status = gCalData_old.NFocus_Status;
            gCalData.Len_Adjust.Zoom_Stop2PR_Status = gCalData_old.Zoom_Stop2PR_Status;
            gCalData.Len_Adjust.Zoom_SpeedTest_Status = gCalData_old.Zoom_SpeedTest_Status;
            gCalData.Len_Adjust.Zoom_StepMissTest_Status = gCalData_old.Zoom_StepMissTest_Status;
            gCalData.Len_Adjust.Focus_StepMissTest_Status = gCalData_old.Focus_StepMissTest_Status;
            gCalData.Len_Adjust.uiFocusOffsetStatus = gCalData_old.uiFocusOffsetStatus;
            memcpy(VOID_PTR(&(gCalData.Len_Adjust.Far_Focus)), VOID_PTR(&(gCalData_old.Far_Focus)), sizeof(UINT32)*15);
            memcpy(VOID_PTR(&(gCalData.Len_Adjust.Far_Focus2)), VOID_PTR(&(gCalData_old.Far_Focus2)), sizeof(UINT32)*15);
            memcpy(VOID_PTR(&(gCalData.Len_Adjust.Near_Focus)), VOID_PTR(&(gCalData_old.Near_Focus)), sizeof(UINT32)*15);
            memcpy(VOID_PTR(&(gCalData.Len_Adjust.Near_Focus2)), VOID_PTR(&(gCalData_old.Near_Focus2)), sizeof(UINT32)*15);
            gCalData.Len_Adjust.uiZoomStop2PR = gCalData_old.uiZoomStop2PR;
            gCalData.Len_Adjust.uiZoomPR2Tele = gCalData_old.uiZoomPR2Tele;
            gCalData.Len_Adjust.Focus_Test_Status = gCalData_old.Focus_Test_Status;
            memcpy(VOID_PTR(&(gCalData.Len_Adjust.iFocusOffset)), VOID_PTR(&(gCalData_old.iFocusOffset)), sizeof(UINT32)*16);
            gCalData.AAA_Adjust.AFE_Test_Status = gCalData_old.AFE_Test_Status;
            gCalData.AAA_Adjust.ISO_Status = gCalData_old.ISO_Status;
            memcpy(VOID_PTR(&(gCalData.AAA_Adjust.Mshutter_Status)), VOID_PTR(&(gCalData_old.Mshutter_Status)), sizeof(UINT32)*8);
            gCalData.AAA_Adjust.FTP_Status = gCalData_old.FTP_Status;
            gCalData.AAA_Adjust.AWB_Status = gCalData_old.AWB_Status;
            gCalData.AAA_Adjust.Shading_Status = gCalData_old.Shading_Status;
            gCalData.AAA_Adjust.FlashAE_Status = gCalData_old.FlashAE_Status;
            gCalData.AAA_Adjust.FlashAWB_Status = gCalData_old.FlashAWB_Status;
            gCalData.AAA_Adjust.OB_Status = gCalData_old.OB_Status;
            gCalData.AAA_Adjust.DL_Status = gCalData_old.DL_Status;
            memcpy(VOID_PTR(&(gCalData.AAA_Adjust.DP_Status)), VOID_PTR(&(gCalData_old.DP_Status)), sizeof(DP_Struct));
            memcpy(VOID_PTR(&(gCalData.AAA_Adjust.AWB)), VOID_PTR(&(gCalData_old.AWB)), sizeof(AWB_Struct));
            memcpy(VOID_PTR(&(gCalData.AAA_Adjust.AWB_Prv)), VOID_PTR(&(gCalData_old.AWB_Prv)), sizeof(AWB_Struct));
            memcpy(VOID_PTR(&(gCalData.AAA_Adjust.AWB_Flash)), VOID_PTR(&(gCalData_old.AWB_Flash)), sizeof(AWB_Struct));
            gCalData.AAA_Adjust.Agc_ISOMin = gCalData_old.Agc_ISOMin;
            gCalData.AAA_Adjust.Agc_ISO_100 = gCalData_old.Agc_ISO_100;
            memcpy(VOID_PTR(&(gCalData.AAA_Adjust.Mshutter)), VOID_PTR(&(gCalData_old.Mshutter)), sizeof(UINT32)*8);
            gCalData.AAA_Adjust.OB_Min = gCalData_old.OB_Min;
            gCalData.AAA_Adjust.FlashTriggerPoint = gCalData_old.FlashTriggerPoint;
            gCalData.AAA_Adjust.Flash_Y_Avg = gCalData_old.Flash_Y_Avg;
            //memcpy(VOID_PTR(&(gCalData.AAA_Adjust.Flash_ADC_Val)), VOID_PTR(&(gCalData_old.Flash_ADC_Val)), sizeof(USHORT)*16);
            gCalData.AAA_Adjust.Flash_Max_Width_Y = gCalData_old.Flash_Max_Width_Y;
            gCalData.AAA_Adjust.ShadingXcenter = gCalData_old.ShadingXcenter;
            gCalData.AAA_Adjust.ShadingYcenter = gCalData_old.ShadingYcenter;
            gCalData.AAA_Adjust.ShadingDecayRatio = gCalData_old.ShadingDecayRatio;
            memcpy(VOID_PTR(&(gCalData.AAA_Adjust.Shading_Gain)), VOID_PTR(&(gCalData_old.Shading_Gain)), sizeof(UINT32)*5);
            memcpy(VOID_PTR(&(gCalData.AAA_Adjust.Defect_line)), VOID_PTR(&(gCalData_old.Defect_line)), sizeof(UINT32)*5);
            gCalData.AAA_Adjust.Cap_Field_Num = gCalData_old.Cap_Field_Num;
            memcpy(VOID_PTR(&(gCalData.AAA_Adjust.Cap_Field_DP_Cnt)), VOID_PTR(&(gCalData_old.Cap_Field_DP_Cnt)), sizeof(UINT32)*8);
            memcpy(VOID_PTR(&(gCalData.AAA_Adjust.DP_Cap_Num)), VOID_PTR(&(gCalData_old.DP_Cap_Num)), sizeof(UINT32)*8);
            //memcpy(&(DP_Cap_Dark_Histo), (VOID_PTR)&(gCalData_old.DP_Cap_Dark_Histo), sizeof(UINT16)*255);
            memcpy(VOID_PTR(&(gCalData.AAA_Adjust.DP_Dzoom)), VOID_PTR(&(gCalData_old.DP_Dzoom)), sizeof(UINT32)*DP_Dzoom_MAX);
            memcpy(VOID_PTR(&(gCalData.AAA_Adjust.Defect_Pixel)), VOID_PTR(&(gCalData_old.Defect_Pixel)), sizeof(UINT32)*(DP_Prv_MAX+DP_Cap_MAX));
            debug_msg("^Yend convert\r\n");
            Save_CalInfo();
#endif
        }
        else
        {
            debug_ind(("^YCAL version = %s\r\n", gCalData.Version));
        }
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
    }
    else
    {
        debug_err(("Read Adjust Data from Nand error\r\n"));
    }
#endif
}

void Save_CalInfo(void)
{
#if _DEMO_TODO
    PPSTORE_SECTION_HANDLE  pSection;
    //need to write buffer
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
    debug_ind(("^YWrite SENSORINFO\r\n"));
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
    CPUCleanInvalidateDCacheBlock((UINT32)&gCalData, (UINT32)&gCalData + sizeof(gCalData));
    CPUDrainWriteBuffer();
    if ((pSection = PStore_OpenSection(PS_CAL_DATA, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        PStore_WriteSection((UINT8 *)&gCalData, 0, sizeof(gCalData), pSection);
        PStore_CloseSection(pSection);
        debug_ind(("Write Adjust Data to Nand success\r\n"));
    }
    else
    {
        debug_err(("Write Adjust Data to Nand error\r\n"));
    }
#endif
}

