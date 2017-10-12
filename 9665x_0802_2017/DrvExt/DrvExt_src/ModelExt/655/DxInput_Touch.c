/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       Input_Touch.c
    @ingroup    mIPRJAPKeyIO

    @brief      Scan Touch Panel HW level

    @note       Nothing.

    @date       2009/04/22
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "Kernel.h"
#include "Debug.h"
#include "DxInput.h"
//#include "gpio.h"
//#include "pad.h"
#include "ADC.h"
#include "Utility.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//#NT#2009/06/12#Photon Lin -end
#define TP_PRESSURE_TH      240     // 255 - 10

#define TP_HW_MAX_X        3900
#define TP_HW_MAX_Y        3872
#define TP_HW_MIN_X        150
#define TP_HW_MIN_Y        153

#define TP_PANEL_X1          0
#define TP_PANEL_Y1          0
#define TP_PANEL_X2          319
#define TP_PANEL_Y2          239

#define TP_PANEL_W          (TP_PANEL_X2-TP_PANEL_X1 + 1)
#define TP_PANEL_H          (TP_PANEL_Y2-TP_PANEL_Y1 + 1)
//////////////////////////////////////////////////////////////////////
#define TP_SAMPLE_NUM              4//1


/*
    0:  Use AUTO Mode with Polling
    1:  Use AUTO Mode with Interrupt
    2:  Use AUTO Mode with FIFO Threshold
*/
#define GSTP_USED_MODE      0


static void DetTP_Convert2RealXY(INT32 *pX, INT32 *pY)
{
    INT32 tempX=0,tempY=0;
    tempX = *pX;
    tempY = *pY;
    if (tempX > TP_HW_MAX_X)
        tempX = TP_HW_MAX_X;
    else if (tempX < TP_HW_MIN_X)
        tempX = TP_HW_MIN_X;

    if (tempY > TP_HW_MAX_Y)
        tempY = TP_HW_MAX_Y;
    else if (tempY < TP_HW_MIN_Y)
        tempY = TP_HW_MIN_Y;

    //*pX =  TP_PANEL_W*(tempX - TP_HW_MIN_X)/(TP_HW_MAX_X - TP_HW_MIN_X);
    *pX =  TP_PANEL_X1 + TP_PANEL_W*(tempX - TP_HW_MIN_X)/(TP_HW_MAX_X - TP_HW_MIN_X);
    //*pY =  TP_PANEL_H*(tempY - TP_HW_MIN_Y)/(TP_HW_MAX_Y - TP_HW_MIN_Y);
    *pY =  TP_PANEL_Y1 + TP_PANEL_H*(TP_HW_MAX_Y - tempY)/(TP_HW_MAX_Y - TP_HW_MIN_Y);

    DBG_IND("Convert:(%d,%d) to (%d,%d)\r\n",tempX,tempY,*pX,*pY);

}

void DetTP_Init(void)
{
    DBG_IND("DetTP_InitHW\r\n");

    if (adc_openTp() != E_OK)
    {
        DBG_ERR("DetTP_InitHW ERR: Can't open ADC Touch Panel detection\r\n");
        return;
    }

    adc_setTpConfig(ADC_TP_CONFIG_ID_GSR,           0xA0);
    adc_setTpConfig(ADC_TP_CONFIG_ID_RESISTOR,      0xA);
    adc_setTpConfig(ADC_TP_CONFIG_ID_SAMPLE_FREQ,   512);// Samples per second

    #if (GSTP_USED_MODE==1)
    adc_setTpConfig(ADC_TP_CONFIG_ID_OPERATE_MODE,  ADC_TP_OPMODE_AUTO_NORMAL);
    adc_setTpConfig(ADC_TP_CONFIG_ID_ISR_CALLBACK,  (UINT32)DetTP_CallBack);
    adc_setTpConfig(ADC_TP_CONFIG_ID_INTEN,         ENABLE);
    #elif (GSTP_USED_MODE==2)
    adc_setTpConfig(ADC_TP_CONFIG_ID_OPERATE_MODE,  ADC_TP_OPMODE_AUTO_THRESHOLD);
    adc_setTpConfig(ADC_TP_CONFIG_ID_FIFO_TH,       2);
    adc_setTpConfig(ADC_TP_CONFIG_ID_ISR_CALLBACK,  (UINT32)DetTP_CallBack);
    adc_setTpConfig(ADC_TP_CONFIG_ID_INTEN,         ENABLE);
    #else
    adc_setTpConfig(ADC_TP_CONFIG_ID_OPERATE_MODE,  ADC_TP_OPMODE_AUTO_NORMAL);
    adc_setTpConfig(ADC_TP_CONFIG_ID_ISR_CALLBACK,  (UINT32)NULL);
    adc_setTpConfig(ADC_TP_CONFIG_ID_INTEN,         DISABLE);
    #endif

    adc_setTpEnable(ENABLE);

}

void DetTP_GetXY(INT32 *pX, INT32 *pY)
{
    INT32 i,j,k;
    UINT32 xsum, ysum, temp;
    UINT16 x[TP_SAMPLE_NUM] = {0}, y[TP_SAMPLE_NUM]= {0};

    temp = 0 ;
    xsum = ysum = 0;

    for (i = 0; i < TP_SAMPLE_NUM; i++)
    {
        adc_clearTPDataReady();
        while(!adc_isTPDataReady())
        {
            Delay_DelayUs(300);
        }
        adc_readTPXY(&x[i],&y[i]);
    }


    //ignore the max and min to calculate average
    if( TP_SAMPLE_NUM > 3)
    {
        //sorting x
        for (j = 0; j < TP_SAMPLE_NUM -1; ++j)
        {
            for (k = j+1; k < TP_SAMPLE_NUM; ++k)
            {
                if(x[j]>x[k])
                {
                    temp = x[j];
                    x[j]=x[k];
                    x[k]=temp;
                }
            }
        }
        //sorting y
        for (j = 0; j < TP_SAMPLE_NUM -1; ++j)
        {
            for (k = j+1; k < TP_SAMPLE_NUM; ++k)
            {
                if(y[j]>y[k])
                {
                    temp = y[j];
                    y[j]=y[k];
                    y[k]=temp;
                }
            }
        }
        for (i = 1; i < TP_SAMPLE_NUM-1; i++)
        {
            xsum += x[i];
            ysum += y[i];
        }
        *pX = xsum/(TP_SAMPLE_NUM-2);
        *pY=  ysum/(TP_SAMPLE_NUM-2);
    }
    else if(TP_SAMPLE_NUM > 0)//just take average
    {
        for (i = 0; i < TP_SAMPLE_NUM; i++)
        {
            xsum += x[i];
            ysum += y[i];
        }
        *pX = xsum/(TP_SAMPLE_NUM);
        *pY=  ysum/(TP_SAMPLE_NUM);
    }

    for (i = 0; i < TP_SAMPLE_NUM; i++)
    {
        DBG_IND("GetHWXY:%d-(%d,%d)\r\n",i,x[i],y[i]);
    }

    DetTP_Convert2RealXY(pX, pY);

}

BOOL DetTP_IsPenDown(void)
{
    if(adc_isTPPD())
    {
        DBG_IND("Pen Down\r\n");
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

