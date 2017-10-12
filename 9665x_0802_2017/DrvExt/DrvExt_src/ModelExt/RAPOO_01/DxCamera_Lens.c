#include "DxCfg.h"
#include "IOCfg.h"
#include "Debug.h"
#include "DxLens.h"
#include "utility.h"
#include "DeviceCtrl.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxLens
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

static LENSCTRL_APPOBJ LensCtrlObj;
static LENS_DEVICE_OBJ LensDevice;
static UINT32 uiBufGPIO[LENS_IO_MAX];
static UINT32 uiBufADC[LENS_ADC_CH_MAX];
extern BOOL ZoomBarPanelCommon_ZoomCB(void);

void DrvLens_PowerOn(void)
{
    DrvLens_PowerOn_Init();
}

void DrvLens_PowerOff(void)
{

}

PLENS_DEVICE_OBJ DrvLens_GetDevice(void)
{

    LensDevice.pGPIOArray                    = &uiBufGPIO[0];
    LensDevice.pADCArray                     = &uiBufADC[0];

    LensDevice.pGPIOArray[LENS_IO_IN_0]        =  GPIO_LENS_IN1A;
    LensDevice.pGPIOArray[LENS_IO_IN_1]        =  GPIO_LENS_IN1B;
    LensDevice.pGPIOArray[LENS_IO_IN_2]        =  GPIO_LENS_IN2A;
    LensDevice.pGPIOArray[LENS_IO_IN_3]        =  GPIO_LENS_IN2B;
    LensDevice.pGPIOArray[LENS_IO_IN_4]        =  GPIO_LENS_IN3A;
    LensDevice.pGPIOArray[LENS_IO_IN_5]        =  GPIO_LENS_IN3B;
    LensDevice.pGPIOArray[LENS_IO_IN_6]        =  GPIO_LENS_IN4A;
    LensDevice.pGPIOArray[LENS_IO_IN_7]        =  GPIO_LENS_IN4B;
    LensDevice.pGPIOArray[LENS_IO_IN_8]        =  GPIO_LENS_IN5A;
    LensDevice.pGPIOArray[LENS_IO_IN_9]        =  GPIO_LENS_IN5B;
    LensDevice.pGPIOArray[LENS_IO_IN_10]       =  GPIO_LENS_IN6A;
    LensDevice.pGPIOArray[LENS_IO_IN_11]       =  GPIO_LENS_IN6B;
    LensDevice.pGPIOArray[LENS_IO_ZOOM_PI]     =  GPIO_LENS_ZOOM_PI;
    LensDevice.pGPIOArray[LENS_IO_ZOOM_PR]     =  GPIO_LENS_ZOOM_PR;
    LensDevice.pGPIOArray[LENS_IO_FOCUS_PI]    =  GPIO_LENS_FOCUS_PI;
    LensDevice.pGPIOArray[LENS_IO_ZOOM_PIINT]  =  GPIO_LENS_ZOOM_PI_ITR;

    return &LensDevice;

}

void DrvLens_RegCtrlCB(void* pLensTask1CB, void* pLensTask2CB, void* pLensAPICB)
{
    LensCtrlObj.Task1CB = (FPLENSCALLBACK)pLensTask1CB;
    LensCtrlObj.Task2CB = (FPLENSCALLBACK)pLensTask2CB;
    LensCtrlObj.APICB = (FPLENSCALLBACK)pLensAPICB;
}

void DrvLens_PowerOn_Init(void)
{

    LensCtrlObj.pLensDev = DrvLens_GetDevice();
    LensCtrlObj.pLens = lens_getObject();
    LensCtrlObj.pMotor = motor_getObject();
}

PLENSCTRL_APPOBJ DrvLens_GetLensCtrlObj(void)
{
    if(LensCtrlObj.pLens && LensCtrlObj.pMotor)
        return &LensCtrlObj;
    else
    {
        DBG_ERR("LensCtrl obj not initialized.\r\n");
        return NULL;
    }
}

