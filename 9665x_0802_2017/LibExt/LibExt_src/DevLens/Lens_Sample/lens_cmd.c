#include "Lens.h"
#include "LensAPI.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "Type.h"
#include "LensCtrlTsk.h"
#include "ADC.h"
#include "Gpio.h"
#include "pwm.h"
#include "top.h"
#include "ErrorNo.h"
#include "uart.h"
#include "IOInit.h"

#define USTEP_THRESHOLD_EN    0  // To generate square wave form.

#define _UART_MONITOR_CMD_SIZE          64

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          LENCMD
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//extern UINT32 pinmux_getPinmux(PIN_FUNC id);
extern void motorG2007_focus_GPIO_switch(BOOL bOnOff);
extern UINT32 motor_G2007_focus_get_cur_phase(void);
extern void motor_G2007_focus_set_cur_phase(UINT32 uiCurPhase);

PIN_GROUP_CONFIG vEmuPwmConfig[] =
{
    // Module name              pinmux config

    //
    // Storage configuration
    //
    {PIN_FUNC_SDIO,             PIN_SDIO_CFG_NONE},
    {PIN_FUNC_SDIO2,            PIN_SDIO_CFG_NONE},
    {PIN_FUNC_NAND,             PIN_NAND_CFG_NONE},

    //
    // Sensor configuration
    //
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_NONE},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_NONE},

    //
    // Serial interface configuration
    //
    {PIN_FUNC_I2C,              PIN_I2C_CFG_NONE},
    {PIN_FUNC_SIF,              PIN_SIF_CFG_NONE},
    {PIN_FUNC_UART,             PIN_UART_CFG_CH1},
    {PIN_FUNC_SPI,              PIN_SPI_CFG_NONE},
    {PIN_FUNC_REMOTE,           PIN_REMOTE_CFG_NONE},

    //
    // PWM configuration
    //
    {PIN_FUNC_PWM,              PIN_PWM_CFG_NONE},

    //
    // AUDIO configuration
    //
    {PIN_FUNC_AUDIO,            PIN_AUDIO_CFG_NONE},

    //
    // Display configuration
    //
    {PIN_FUNC_LCD,              PINMUX_DISPMUX_SEL_LCD},
    {PIN_FUNC_LCD2,             PINMUX_DISPMUX_SEL_LCD2},
    {PIN_FUNC_TV,               PINMUX_TV_HDMI_CFG_GPIO},
    {PIN_FUNC_HDMI,             PINMUX_TV_HDMI_CFG_GPIO},

    //
    // USB configuration
    //
    {PIN_FUNC_USB,              PINMUX_USB_CFG_DEVICE}
};

typedef struct _PWM_4CH_
{
    UINT32  uiCh_A1;
    UINT32  uiCh_A2;
    UINT32  uiCh_B1;
    UINT32  uiCh_B2;
}PWM_4CH;

// 4 represent 4 step
PWM_4CH pwmChannel[4] =
{
#if 0
    //AF_A1 AF_B1   AF_A2  AF_B2
    {   0,    4,      2,     6  }, //Step0
    {   2,    6,      4,     0  }, //Step1
    {   4,    0,      6,     2  }, //Step2
    {   6,    2,      0,     4  }, //Step3
#else
    //AF_A1 AF_B1   AF_A2  AF_B2
    {   1,    5,      3,     7  }, //Step0
    {   3,    7,      5,     1  }, //Step1
    {   5,    1,      7,     3  }, //Step2
    {   7,    3,      1,     5  }, //Step3
#endif
};

BOOL Lens_OnCommand(CHAR *pcCmdStr)
{

    switch (*pcCmdStr)
    {
    // Command start with 'D'
    case 'd':
        break;

    case 'f':
            /*
                No.3 AF_A1 ->PWM0
                No.4 AF_A2 ->PWM1
                No.5 AF_B1 ->PWM2
                No.6 AF_B2 ->PWM3
            */
            #if 0
            /*
                No.3 AF_A1 ->PWM0
                No.4 AF_A2 ->PWM1
                No.5 AF_B1 ->PWM2
                No.6 AF_B2 ->PWM3
            */
            {
                BOOL            bThresholdEn = 0;
                UINT32          i;
                UINT32          uiStepPerPhase;
                MS_CH_PHASE_CFG msSetChPhCfg;
                MSCOMMON_CFG    msCommonCfg;
                UINT32          uiStartStep;
                UINT32          uiStepCnt;
                UINT32          uiDir;
                UINT32          uiGlobalStep = motor_G2007_focus_get_cur_phase() - 1;

                sscanf(pcCmdStr + 2, "%d %d %d", &uiDir,  &uiStepCnt, &bThresholdEn);

                DBG_MSG("^Y\033[1m Micro step select start step 0~3\r\n");
                //uiStartStep = emu_GetNumber(0, 3);

                uiStartStep = uiGlobalStep;
                //uiGlobalStep = 0;

                DBG_MSG("^Y\033[1m Micro step select step count 0-32767 (0 = free run)\r\n");
                //uiStepCnt = emu_GetNumber(0, 32767);

                //uiStepCnt = 4;

                DBG_MSG("^Y\033[1m Micro step select Direct 1 : near(decrease) 0 : inf(increase)\r\n");
                //uiDir = emu_GetNumber(0, 1);

                //uiDir = 0;

                DBG_MSG("^Y Micro step select step per phase 0:8, 1:16, 2:32, 3:64\r\n");

                //uiStepPerPhase = emu_GetNumber(0, 3);

                DBG_MSG("^Y Micro step select step per phase 0:8, 1:16, 2:32, 3:64\r\n");
                uiStepPerPhase = 0;

                motorG2007_focus_GPIO_switch(0);
                LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
                Delay_DelayMs(5); // Delay to ensure all signals are stable.

                LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_ON,0);
                Delay_DelayMs(1); // Delay to ensure all signals are stable.
                DBG_MSG("MOTOR_FOCUS_EXCITE_ON\r\n");
                #if 1
                //Switch Pinmux of pwm0 - 3
                vEmuPwmConfig[PIN_FUNC_PWM].config = (PIN_PWM_CFG_PWM4|PIN_PWM_CFG_PWM5|PIN_PWM_CFG_PWM6|PIN_PWM_CFG_PWM7);

                pinmux_init((PIN_GROUP_CONFIG *)vEmuPwmConfig);
                #endif

                msCommonCfg.uiDir = uiDir;
                //msCommonCfg.uiDiv = 3;

                DBG_MSG("#First StepCnt:%d, StartStep:%d\r\n", uiStepCnt, uiStartStep);
                if (uiStepCnt <= 4)
                    msCommonCfg.uiOnCycle = uiStepCnt;
                else
                    msCommonCfg.uiOnCycle = 4; //uiStepCnt;

                msCommonCfg.uiStepPerPhase = ((TOTAL_08_STEP_PER_PHASE) << uiStepPerPhase);
                msCommonCfg.uiPhaseType = PWM_MS_2_2_PHASE_TYPE;
                //#if USTEP_THRESHOLD_EN
                if(bThresholdEn)
                    msCommonCfg.uiThresholdEn = 1;
                else
                    msCommonCfg.uiThresholdEn = 0;
                //#endif
                msCommonCfg.uiThreshold = 0;

                msSetChPhCfg.uiCH0_Phase = pwmChannel[uiStartStep].uiCh_A1;
                msSetChPhCfg.uiCH1_Phase = pwmChannel[uiStartStep].uiCh_A2;
                msSetChPhCfg.uiCH2_Phase = pwmChannel[uiStartStep].uiCh_B1;
                msSetChPhCfg.uiCH3_Phase = pwmChannel[uiStartStep].uiCh_B2;

                #if 1
                pwm_msSetClockDiv(PWM_MS_SET_1, 23);
                #else // Slow down if lost step.
                pwm_msSetClockDiv(PWM_MS_SET_1, 30);
                #endif

                if(pwm_msSetOpen(PWM_MS_SET_1) != E_OK)
                {
                    DBG_MSG("MS set open error\r\n");
                    break;
                }

                if(pwm_msSetConfig(PWM_MS_SET_1, &msSetChPhCfg, &msCommonCfg) != E_OK)
                {
                    DBG_MSG("MS set config error\r\n");
                    break;
                }
                //gpio_setPin(D_GPIO_0);
                pwm_setEn(PWM_MS_SET_1);
                Delay_DelayMs(5);
                //gpio_clearPin(D_GPIO_0);
                //pwm_msChannelEn(pinmux_getPinmux(PIN_FUNC_PWM));
                pwm_msSetEn(PWM_MS_SET_1);
                if(uiStepCnt != 0)
                {
                    pwm_msSetWait(PWM_MS_SET_1);
                    //pwm_msSetClose(PWM_MS_SET_1, FALSE);

                    if(uiStepCnt > 4)
                    {
                        uiStepCnt -= 4;

                        if(uiDir)
                        {
                            for(i = 0; i < 4; i++)
                            {
                                if(uiGlobalStep == 0)
                                    uiGlobalStep = 3;
                                else
                                    uiGlobalStep--;
                            }
                        }
                        else
                            uiGlobalStep += 4;
                    }
                    else
                    {
                        if(uiDir)
                        {
                            for(i = 0; i < uiStepCnt; i++)
                            {
                                if(uiGlobalStep == 0)
                                    uiGlobalStep = 3;
                                else
                                    uiGlobalStep--;
                            }
                        }
                        else
                            uiGlobalStep += uiStepCnt;

                        uiStepCnt = 0;
                    }

                    uiGlobalStep %= 4;
                }
                //gpio_setPin(D_GPIO_0);

                if(uiStepCnt) // Fast step.
                {
                    //uiStepCnt = 8;
                    msCommonCfg.uiOnCycle = uiStepCnt;
                    msCommonCfg.uiStepPerPhase = ((TOTAL_08_STEP_PER_PHASE) << uiStepPerPhase);
                    msCommonCfg.uiPhaseType = PWM_MS_2_2_PHASE_TYPE;
                    //#if USTEP_THRESHOLD_EN
                    if(bThresholdEn)
                        msCommonCfg.uiThresholdEn = 1;
                    else
                        msCommonCfg.uiThresholdEn = 0;
                    //#endif
                    msCommonCfg.uiThreshold = 0;


                    uiStartStep = uiGlobalStep;

                    msSetChPhCfg.uiCH0_Phase = pwmChannel[uiStartStep].uiCh_A1;
                    msSetChPhCfg.uiCH1_Phase = pwmChannel[uiStartStep].uiCh_A2;
                    msSetChPhCfg.uiCH2_Phase = pwmChannel[uiStartStep].uiCh_B1;
                    msSetChPhCfg.uiCH3_Phase = pwmChannel[uiStartStep].uiCh_B2;
                    #if 1
                    pwm_setStop(PWM_MS_SET_1);
                    pwm_msSetClockDiv(PWM_MS_SET_1, 9);
                    #else // Slow down if lost step.
                    //pwm_msSetClockDiv(PWM_MS_SET_1, 10);
                    #endif
                    if(pwm_msSetConfig(PWM_MS_SET_1, &msSetChPhCfg, &msCommonCfg) != E_OK)
                    {
                        DBG_MSG("MS set config error\r\n");
                        break;
                    }
                    //pwm_msChannelEn(pinmux_getPinmux(PIN_FUNC_PWM));
                    pwm_setEn(PWM_MS_SET_1);
                    pwm_msSetEn(PWM_MS_SET_1);
                    if(uiStepCnt != 0)
                    {
                        pwm_msSetWait(PWM_MS_SET_1);
                        //gpio_clearPin(D_GPIO_0);
                        if(uiDir)
                        {
                            for(i = 0; i < uiStepCnt; i++)
                            {
                                if(uiGlobalStep == 0)
                                    uiGlobalStep = 3;
                                else
                                    uiGlobalStep--;
                            }
                        }
                        else
                            uiGlobalStep += uiStepCnt;

                        uiGlobalStep %= 4;
                    }

                }

                Delay_DelayMs(5);
                pwm_msSetClose(PWM_MS_SET_1, TRUE);
                //gpio_setPin(D_GPIO_0);

                DBG_MSG("MOTOR_FOCUS_EXCITE_OFF -begin \r\n");
                LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_OFF,0);
                #if 1
                IO_InitPinmux();
                #endif
                LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
                motorG2007_focus_GPIO_switch(1);
                motor_G2007_focus_set_cur_phase(uiGlobalStep+1);
                DBG_MSG("MOTOR_FOCUS_EXCITE_OFF -end \r\n");
            }
            #else
            #endif
        break;
    //Lens operation
    case 'g':
        if (!strncmp(pcCmdStr,"gzs", 3))
            {
                DBG_ERR("Get Zoom Step= #%d, *%d, %d\r\n", Lens_Zoom_GetSection(ZOOM_CUR_SECTION),
                                                              Lens_Zoom_GetStep(ZOOM_CUR_SECTION_STEP),
                                                              Lens_Zoom_GetStep(ZOOM_CUR_STEP));
                return TRUE;
            }

        break;

    case 'i':
        if (!strncmp(pcCmdStr,"i0",2))
            {
                DBG_ERR("IRIS IRIS_POS_BIG ......\r\n");
                Lens_Aperture_Move(IRIS_POS_BIG);
                return TRUE;
            }
        else if (!strncmp(pcCmdStr,"i1",2))
            {
                DBG_ERR("IRIS IRIS_POS_SMALL ......\r\n");
                Lens_Aperture_Move(IRIS_POS_SMALL);
                return TRUE;
            }
        else if (!strncmp(pcCmdStr,"i2",2))
            {
                DBG_ERR("IRIS IRIS_POS_CLOSE ......\r\n");
                Lens_Aperture_Move(IRIS_POS_CLOSE);
                return TRUE;
            }
        break;
    case 'm':    //Motor driver verification
    {

        if (!strncmp(pcCmdStr,"md z f",6))    //Zoom forward test
        {
            DBG_MSG("^R Zoom forward test\r\n");
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
            LensCtrl_SetSignal(MD_SIGNAL_ZPI,ON);
            LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);

            LensCtrl_ZoomChangeSpeed(90);
            LensCtrl_ZoomForward();

            Delay_DelayMs(300);
            LensCtrl_ZoomBrake();

            //DBG_MSG("^B ZPR = %d\r\n",Lens_CheckLDSignal(LD_SIGNAL_ZPR));

            LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
            LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md z r",6))//Zoom reverse test
        {
            DBG_MSG("^R Zoom reverse test\r\n");

            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
            LensCtrl_SetSignal(MD_SIGNAL_ZPI,ON);
            LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);

            LensCtrl_ZoomChangeSpeed(90);
            LensCtrl_ZoomReverse();

            Delay_DelayMs(300);
            LensCtrl_ZoomBrake();
            Delay_DelayMs(30);

            //DBG_MSG("^B ZPR = %d\r\n",Lens_CheckLDSignal(LD_SIGNAL_ZPR));

            LensCtrl_SetSignal(MD_SIGNAL_ZPI,OFF);
            LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md z b",6))//Zoom break test
        {
            DBG_MSG("^R Zoom break test\r\n");

            LensCtrl_ZoomBrake();
            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md z o",6))//Zoom break test
        {
            DBG_MSG("^R Zoom off  test\r\n");

            LensCtrl_ZoomOff();
            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md z",4))//Focus forward
        {
            UINT32 value = 50;
            sscanf(pcCmdStr+4, "%d", &value);

            DBG_MSG("^R Zoom speed change to %d\r\n",value);

            LensCtrl_ZoomChangeSpeed(value);

            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md i n",6))//iris F no. 1.8 ~ 8.0.
        {
            UINT32 value = IRIS_POS_BIG ;
            sscanf(pcCmdStr+6, "%d", &value);

            DBG_MSG("^R Iris seg. %d test\r\n", value);
            if (value > IRIS_POS_SMALL)
            {
                DBG_ERR("Iris test para error!\r\n");
                return TRUE;
            }

            Lens_Aperture_Move(value);
            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md i b",6))//Big iris
        {
            DBG_MSG("^R Iris big test\r\n");
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
            //#NT#2010/12/01#Lincy Lin -begin
            //#NT#
            //LensCtrl_Aperture_SetState(IRIS_POS_BIG);
            LensCtrl_Aperture_SetState(MOTOR_APERTURE_NORMAL,IRIS_POS_BIG);
            //#NT#2010/12/01#Lincy Lin -end
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md i s",6))//Small iris
        {
            DBG_MSG("^R Iris small test\r\n");
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
            //#NT#2010/12/01#Lincy Lin -begin
            //#NT#
            //LensCtrl_Aperture_SetState(IRIS_POS_SMALL);
            LensCtrl_Aperture_SetState(MOTOR_APERTURE_NORMAL,IRIS_POS_SMALL);
            //#NT#2010/12/01#Lincy Lin -end
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md i c",6))//close iris
        {
            DBG_MSG("^R Iris close test\r\n");
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
            //#NT#2010/12/01#Lincy Lin -begin
            //#NT#
            //LensCtrl_Aperture_SetState(IRIS_POS_CLOSE);
            LensCtrl_Aperture_SetState(MOTOR_APERTURE_NORMAL,IRIS_POS_CLOSE);
            //#NT#2010/12/01#Lincy Lin -end
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md s on",7))//Shutter on
        {
            DBG_MSG("^R Shutter on test\r\n");
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
            LensCtrl_Shutter_SetState(MOTOR_SHUTTER_NORMAL,SHUTTER_POS_OPEN);
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md s off",7))//Shutter off
        {
            DBG_MSG("^R Shutter off test\r\n");
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
            LensCtrl_Shutter_SetState(MOTOR_SHUTTER_NORMAL,SHUTTER_POS_CLOSE);
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md f f",6))//Focus forward
        {
            UINT32 value = 8;
            UINT32 i;
            sscanf(pcCmdStr+6, "%d", &value);

            DBG_MSG("^R Focus forward test, %d\r\n",value);
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
            LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);
            LensCtrl_SetSignal(MD_SIGNAL_FPI,ON);

            Delay_DelayMs(2); // Delay to ensure all signals are stable.

            LensCtrl_Focus_SetState(MOTOR_FOCUS_MICRO_STEP_EN, 1);
            LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_ON, 1);

            for (i=0;i<value;i++)
            {
                LensCtrl_Focus_SetState(MOTOR_FOCUS_FWD,0);
            }

            LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_OFF, 1);
            LensCtrl_Focus_SetState(MOTOR_FOCUS_MICRO_STEP_EN, 0);

            //DBG_MSG("^B FPI = %d\r\n",Lens_CheckLDSignal(LD_SIGNAL_FPI));

            LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
            LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md f b",6))//Focus backward
        {
            UINT32 i;
            UINT32 value;
            sscanf(pcCmdStr+6, "%d", &value);

            DBG_MSG("^R Focus backward test, %d\r\n",value);
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
            LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);
            LensCtrl_SetSignal(MD_SIGNAL_FPI,ON);

            LensCtrl_Focus_SetState(MOTOR_FOCUS_MICRO_STEP_EN, 1);
            LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_ON,0);
            for (i=0;i<value;i++)
            {
                LensCtrl_Focus_SetState(MOTOR_FOCUS_BWD,0);
            }

            LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_OFF,0);
            LensCtrl_Focus_SetState(MOTOR_FOCUS_MICRO_STEP_EN, 0);

            //DBG_MSG("^B FPI = %d\r\n",Lens_CheckLDSignal(LD_SIGNAL_FPI));

            LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
            LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md f x",6))//Focus forward
        {
            UINT32 value = 8;
            UINT32 i, j, uitime = 0, uibacklash = 2;
            sscanf(pcCmdStr+7, "%d %d %d", &uitime, &value, &uibacklash);

            DBG_MSG("^R Focus burn in test - %d times, %d step, %d backlash\r\n", uitime, value, uibacklash);

            for(j = 0; j < uitime; j++)
            {
                DBG_MSG("^R Focus forward test - round %d\r\n",j);
                Lens_Init(LENS_INIT_ZOOM_PART1);
                Lens_Init(LENS_INIT_ZOOM_PART2);
                DBG_MSG("Lens zoom Init...done\r\n");

                Delay_DelayMs(2000);

                Lens_Init(LENS_INIT_APERTURE);
                Lens_Init(LENS_INIT_FOCUS);
                DBG_MSG("Lens focus Init...done\r\n");

                Delay_DelayMs(1000);

                LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
                LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);
                LensCtrl_SetSignal(MD_SIGNAL_FPI,ON);

                Delay_DelayMs(2); // Delay to ensure all signals are stable.

                LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_ON,0);
                for (i=0;i<369 + uibacklash;i++) // Get back home.
                {
                    LensCtrl_Focus_SetState(MOTOR_FOCUS_BWD,0);
                }

                //Delay_DelayMs(10); // Delay to ensure all signals are stable.
                //DBG_MSG("^G #FPI = %d\r\n",Lens_CheckLDSignal(LD_SIGNAL_FPI));
                LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_OFF,0);

                #if 1
                LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_ON,0);

                for (i=0;i<value;i++)
                {
                    LensCtrl_Focus_SetState(MOTOR_FOCUS_FWD,0);
                }

                LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_OFF,0);

                //Delay_DelayMs(10); // Delay to ensure FPI is stable.
                //DBG_MSG("^B FPI = %d\r\n",Lens_CheckLDSignal(LD_SIGNAL_FPI));

                LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
                LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
                LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

                Delay_DelayMs(500); // Delay to let motor take a good rest before backward.

                DBG_MSG("^R Focus backward test - round %d\r\n",j);
                LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
                LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);
                LensCtrl_SetSignal(MD_SIGNAL_FPI,ON);

                Delay_DelayMs(2); // Delay to ensure all signals are stable.

                LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_ON,0);
                for (i=0;i<value;i++)
                {
                    LensCtrl_Focus_SetState(MOTOR_FOCUS_BWD,0);
                }

                LensCtrl_Focus_SetState(MOTOR_FOCUS_EXCITE_OFF,0);

                //Delay_DelayMs(10); // Delay to ensure FPI is stable.
                //DBG_MSG("^B FPI = %d\r\n",Lens_CheckLDSignal(LD_SIGNAL_FPI));

                #endif
                LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
                LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
                LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

                Delay_DelayMs(500); // Delay to let motor take a good rest before backward.

                DBG_MSG("^R check signal ZPR&FPI \r\n");
                LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
                LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);
                LensCtrl_SetSignal(MD_SIGNAL_FPI,ON);

                Delay_DelayMs(10); // Delay to get stable signals.
                if(Lens_CheckLDSignal(LD_SIGNAL_FPI))
                    DBG_MSG("^Yfpi= 1 => OK\r\n");
                else
                {
                    DBG_MSG("^Rfpi= 0 => NG\r\n");
                    j = uitime; // force to quit loop.
                }


                LensCtrl_Focus_SetState(MOTOR_FOCUS_FWD,0);

                Delay_DelayMs(10); // Delay to get stable signals.
                if(!Lens_CheckLDSignal(LD_SIGNAL_FPI))
                    DBG_MSG("^Yfpi= 0 => OK\r\n");
                else
                {
                    DBG_MSG("^Rfpi= 1 => NG\r\n");
                    j = uitime; // force to quit loop.
                }

                LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
                LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
                LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);


            }
            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md i mf", 7))//iris forward
        {
            UINT32 i;
            UINT32 value;
            sscanf(pcCmdStr+7, "%d", &value);

            DBG_MSG("^R Iris forward test, %d\r\n",value);
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);

            LensCtrl_Aperture_SetState(MOTOR_APERTURE_EXCITE_ON,0);
            for (i=0;i<value;i++)
            {
                LensCtrl_Aperture_SetState(MOTOR_APERTURE_FWD,0);
            }

            LensCtrl_Aperture_SetState(MOTOR_APERTURE_EXCITE_OFF,0);

            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

            return TRUE;
        }
        else if (!strncmp(pcCmdStr,"md i mb", 7))//iris backward
        {
            UINT32 i;
            UINT32 value;
            sscanf(pcCmdStr+7, "%d", &value);

            DBG_MSG("^R Iris backward test, %d\r\n",value);
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);

            LensCtrl_Aperture_SetState(MOTOR_APERTURE_EXCITE_ON,0);
            for (i=0;i<value;i++)
            {
                LensCtrl_Aperture_SetState(MOTOR_APERTURE_BWD,0);
            }

            LensCtrl_Aperture_SetState(MOTOR_APERTURE_EXCITE_OFF,0);

            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);

            return TRUE;
        }
        break;


    }
    case 'r':
        if (!strncmp(pcCmdStr,"rpi",3))
        {
            UINT32 ui16value;

            DBG_MSG("^R check signal ZPR&FPI \r\n");
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,ON);
            LensCtrl_SetSignal(MD_SIGNAL_ZPR,ON);
            LensCtrl_SetSignal(MD_SIGNAL_FPI,ON);

            sscanf(pcCmdStr+3, "%d", &ui16value);

            ui16value &= 0xFFF;
            Delay_DelayMs(ui16value); // Delay to get stable signals.

//            DBG_ERR("zpi=%d\r\n",Lens_CheckLDSignal(LD_SIGNAL_ZPI)));
            #if 1
            DBG_ERR("zpr=%d\r\n",Lens_CheckLDSignal(LD_SIGNAL_ZPR));
            DBG_ERR("fpi=%d\r\n",Lens_CheckLDSignal(LD_SIGNAL_FPI));
            #else
            gpio_setDir(P_GPIO_49, GPIO_DIR_INPUT);
            gpio_setDir(P_GPIO_50, GPIO_DIR_INPUT);
            DBG_ERR("zpr=%d\r\n",gpio_getPin(P_GPIO_49)));
            DBG_ERR("fpi=%d\r\n",gpio_getPin(P_GPIO_50)));
            #endif

            LensCtrl_SetSignal(MD_SIGNAL_ZPR,OFF);
            LensCtrl_SetSignal(MD_SIGNAL_FPI,OFF);
            LensCtrl_SetSignal(MD_SIGNAL_STANDBY,OFF);
            return TRUE;
        }
        break;
    case 'l':
            if (!strncmp(pcCmdStr,"l reset",7))
            {
                DBG_MSG("Lens reset...\r\n");

                Lens_Reset();
                return TRUE;
            }
            else if (!strncmp(pcCmdStr,"l retract", 9))
            {
                Lens_Retract();
                DBG_MSG("Lens Retract...\r\n");
                return TRUE;
            }
            else if (!strncmp(pcCmdStr,"lis",3))
            {
                Lens_Init(LENS_INIT_ZOOM_PART1);
                Lens_Init(LENS_INIT_ZOOM_PART2);
                DBG_MSG("Lens Init...\r\n");
                return TRUE;
            }
            else if (!strncmp(pcCmdStr,"lie",3))
            {

                Lens_Init(LENS_INIT_APERTURE);
                Lens_Init(LENS_INIT_FOCUS);

                DBG_MSG("Lens END...\r\n");
                return TRUE;
            }
            else if (!strncmp(pcCmdStr,"lr",2))
            {
                Lens_Zoom_Stop();
                DBG_MSG("Lens_Zoom_Stop...\r\n");
                return TRUE;
            }
            else if (!strncmp(pcCmdStr,"lz",2))
            {
                UINT32 value = 500;
                sscanf(pcCmdStr+2, "%d", &value);
                DBG_ERR("Lens_Zoom_in...%d ms\r\n",value);
                Lens_Zoom_In();
                Delay_DelayMs(value);
                DBG_ERR("Lens_Zoom_stop...%d ms\r\n",value);
                Lens_Zoom_Stop();
                return TRUE;
            }
            else if (!strncmp(pcCmdStr,"lo",2))
            {
                UINT32 value = 500;
                sscanf(pcCmdStr+2, "%d", &value);
                DBG_ERR("Lens_Zoom_out...%d ms\r\n",value);
                Lens_Zoom_Out();
                Delay_DelayMs(value);
                DBG_ERR("Lens_Zoom_stop...%d ms\r\n",value);
                Lens_Zoom_Stop();
                return TRUE;
            }
            else if (!strncmp(pcCmdStr,"lao",3))
            {
                Lens_Aperture_Move(IRIS_POS_BIG);
                return TRUE;
            }
            else if (!strncmp(pcCmdStr,"lac",3))
            {
                Lens_Aperture_Move(IRIS_POS_SMALL);
                return TRUE;
            }
            else if (!strncmp(pcCmdStr,"lsc",3))
            {
                Lens_Shutter_Move(MOTOR_SHUTTER_NORMAL,SHUTTER_POS_CLOSE);
                return TRUE;
            }
            else if (!strncmp(pcCmdStr,"lso",3))
            {
                Lens_Shutter_Move(MOTOR_SHUTTER_NORMAL,SHUTTER_POS_OPEN);
                return TRUE;
            }
            else if (!strncmp(pcCmdStr,"lfi",3))
            {
                UINT32 value;
                sscanf(pcCmdStr+3, "%d", &value);
                DBG_ERR("Focus run Before %d\r\n",Lens_Focus_GetPosition());
                Lens_Focus_DoAction((FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC),Lens_Focus_GetPosition()+value);
                DBG_ERR("Focus run After %d\r\n",Lens_Focus_GetPosition());
                return TRUE;
            }
            else if (!strncmp(pcCmdStr,"lfo",3))
            {
                UINT32 value;
                sscanf(pcCmdStr+3, "%d", &value);
                DBG_ERR("Focus run Before %d\r\n",Lens_Focus_GetPosition());
                Lens_Focus_DoAction((FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC),Lens_Focus_GetPosition()-value);
                DBG_ERR("Focus run After %d\r\n",Lens_Focus_GetPosition());
                return TRUE;
            }
            else if (!strncmp(pcCmdStr,"lbi",3))  // zoom burn in.
            {
                UINT32 value, i, j;
                sscanf(pcCmdStr+4, "%d", &value);

                DBG_MSG("zoom burn in test - %d times\r\n", value);
                Lens_Init(LENS_INIT_ZOOM_PART1);
                Lens_Init(LENS_INIT_ZOOM_PART2);
                DBG_MSG("Lens Init...\r\n");

                Delay_DelayMs(2000);

                Lens_Init(LENS_INIT_APERTURE);
                Lens_Init(LENS_INIT_FOCUS);

                Delay_DelayMs(1000);
                for(i = 0; i < value; i++)
                {
                    DBG_MSG("^R Test Round %d\r\n", i);
                    DBG_MSG("^Y zoom in to tele\r\n");
                    for(j = 0; j < 10; j++)
                    {
                        Lens_Zoom_In();
                        Delay_DelayMs(30);
                        Lens_Zoom_Stop();
                        Delay_DelayMs(100);
                    }

                    DBG_MSG("^Y zoom out to wide\r\n");
                    for(j = 0; j < 10; j++)
                    {
                        Lens_Zoom_Out();
                        Delay_DelayMs(30);
                        Lens_Zoom_Stop();
                        Delay_DelayMs(100);
                    }
                }

                DBG_ERR("Get Zoom Step= #%d, *%d, %d\r\n", Lens_Zoom_GetSection(ZOOM_CUR_SECTION),
                                                              Lens_Zoom_GetStep(ZOOM_CUR_SECTION_STEP),
                                                              Lens_Zoom_GetStep(ZOOM_CUR_STEP));

                return TRUE;
            }

        break;
    }
    return FALSE;
}

