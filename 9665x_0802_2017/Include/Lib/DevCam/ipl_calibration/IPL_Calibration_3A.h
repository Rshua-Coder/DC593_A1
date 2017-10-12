#ifndef _IPL_CALIBRATION_3A_H_
#define _IPL_CALIBRATION_3A_H_
/**
    ipl calibration 3A

    this calibration 3A is for getting proper cal. condition.

    @file       ipl_calibration_3a.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#include "Type.h"
#include "errorno.h"
#include "Sensor.h"
#include "Ipl_Utility.h"

/** \addtogroup mILibCal */
//@{

/**
    calibration auto exposure fix option
*/
typedef enum _CAL_AE_TYPE
{
    AE_ADJ_GAIN = 0x00, ///< adjust iso
    AE_ADJ_TIME,        ///< adjust exposure time
    AE_ADJ_ISOBASE,     ///< adjust ISO Base
    AE_FIX_ALL,         ///< Fix iso/base and exposure time
    ENUM_DUMMY4WORD(CAL_AE_TYPE)
} CAL_AE_TYPE;

/**
    calibration current auto exposure status
*/
typedef enum _CAL_AE_STATUS
{
    CAL_AE_NONE    = 0, ///< none
    CAL_AE_RUNNING = 1, ///< converging lumi.
    CAL_AE_TIMEOUT,     ///< lumi. can't be converged to target lumi.
    CAL_AE_STABLE,      ///< lumi. is converged to target lumi.
    ENUM_DUMMY4WORD(CAL_AE_RST)
} CAL_AE_STATUS;

/**
    Calibration AE setting
*/
typedef struct _CAL_AE_INFO
{
    BOOL            bPreGamma;  ///< TRUE if source is before gamma proc.
    CAL_AE_TYPE     uiAEType;   ///< converge type
    UINT32          uiTarget;   ///< taget luminance
    UINT32 uiExpT;              ///< exposure time
    UINT32 uiISO;               ///< ISO
    UINT32 uiISOBase;           ///< ISO Base
    UINT32 uiAdjStep;           ///< AE adjust step
    UINT32 uiTimeout;           ///< timeout count
} CAL_AE_INFO;

/**
    calibration ae result
*/
typedef struct _CAL_AE_RST
{
    CAL_AE_STATUS status;       ///< rst status
    UINT32 uiExpT;              ///< current exposure time
    UINT32 uiISO;               ///< current ISO
    UINT32 uiISOBase;           ///< current ISO Base
    UINT32 convergeLum;           ///< final converge lum in AE
}CAL_AE_RST;


/**
    calibration 3A operation structure
    @note plz refer to Cal_3AEvent
*/
typedef struct _CAL_3A_OBJ
{
    CAL_AE_RST  (*fpAEStable)(IPL_RAW_IMG_INFO* pRaw,CAL_AE_INFO aeInfo,SENSOR_ID SenId);   ///< plz refer to Cal_3AEvent
}CAL_3A_OBJ,*pCAL_3A_OBJ;

/**
     calibration 3A operation


     @return Description of data returned.
         - @b fpAEStable:   search defect pixel on bayer image.
\n                      input para:
\n                          pRaw:   image information
\n                          aeInfo: converging condition
\n                          SenId:  sensor Idx
\n                      return:
\n                          the result of convergence
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern pCAL_3A_OBJ Cal_3AEvent(void);
//@}
#endif
