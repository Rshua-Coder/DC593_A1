#ifndef _SENSOR_DRV_H
#define _SENSOR_DRV_H

#include "type.h"
#include "Sensor.h"

/************* Sensor 1 *************/

#if  (_SENSORLIB_ == _SENSORLIB_CMOS_S5K2P1YX_)
SENSOR_DRVTAB* Sensor_GetDrvTab_S5K2P1YX(void);
#endif

#if  (_SENSORLIB_ == _SENSORLIB_CMOS_IMX083CQK_)
SENSOR_DRVTAB* Sensor_GetDrvTab_IMX083CQK(void);
#endif


#if  (_SENSORLIB_ == _SENSORLIB_CMOS_AR0330P_)
SENSOR_DRVTAB* Sensor_GetDrvTab_AR0330(void);
#endif


#if  (_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_)
SENSOR_DRVTAB* Sensor_GetDrvTab_AR0330M(void);
#endif

#if  (_SENSORLIB_ == _SENSORLIB_CMOS_NT99231M_)
SENSOR_DRVTAB* Sensor_GetDrvTab_NT99231M(void);
#endif

#if  (_SENSORLIB_ == _SENSORLIB_CMOS_MI5130_)
SENSOR_DRVTAB* Sensor_GetDrvTab_MI5130(void);
#endif

#if  (_SENSORLIB_ == _SENSORLIB_CMOS_MI5100_)
SENSOR_DRVTAB* Sensor_GetDrvTab_MI5100(void);
#endif

#if  (_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_)
SENSOR_DRVTAB* Sensor_GetDrvTab_MN34110(void);
#endif

#if (_SENSORLIB_ == _SENSORLIB_CMOS_IMX208PQH_)
SENSOR_DRVTAB* Sensor_GetDrvTab_IMX208PQH(void);
#endif

#if (_SENSORLIB_ == _SENSORLIB_CMOS_IMX179_)
SENSOR_DRVTAB* Sensor_GetDrvTab_IMX179(void);
#endif

#if (_SENSORLIB_ == _SENSORLIB_CMOS_IMX078CQK_)
SENSOR_DRVTAB* Sensor_GetDrvTab_IMX078CQK(void);
#endif

#if (_SENSORLIB_ == _SENSORLIB_CMOS_IMX206CQC_)
SENSOR_DRVTAB* Sensor_GetDrvTab_IMX206CQC(void);
#endif

#if (_SENSORLIB_ == _SENSORLIB_CMOS_OV2710M_)
SENSOR_DRVTAB* Sensor_GetDrvTab_OV2710M(void);
#endif

#if (_SENSORLIB_ == _SENSORLIB_CMOS_OV2710_)
SENSOR_DRVTAB* Sensor_GetDrvTab_OV2710(void);
#endif

#if (_SENSORLIB_ == _SENSORLIB_CMOS_OV8858_)
SENSOR_DRVTAB* Sensor_GetDrvTab_OV8858(void);
#endif

#if (_SENSORLIB_ == _SENSORLIB_CMOS_F22P_)
SENSOR_DRVTAB* Sensor_GetDrvTab_F22P(void);
#endif

#if (_SENSORLIB_ == _SENSORLIB_CMOS_IMX322LQJ_)
SENSOR_DRVTAB*  Sensor_GetDrvTab_IMX322LQJ(void);
#endif

#if (_SENSORLIB_ == _SENSORLIB_CMOS_IMX323LQN_)
SENSOR_DRVTAB*  Sensor_GetDrvTab_IMX323LQN(void);
#endif

#if  (_SENSORLIB_ == _SENSORLIB_CMOS_F02C1M_)
SENSOR_DRVTAB* Sensor_GetDrvTab_F02C1M(void);
#endif

#if  (_SENSORLIB_ == _SENSORLIB_DUMMY_)
SENSOR_DRVTAB* Sensor_GetDrvTab_Dummy(void);
#endif

#if  (_SENSORLIB_ == _SENSORLIB_VIRTUAL_)
SENSOR_DRVTAB* Sensor_GetDrvTab_Virtual(void);
#endif

/************* Sensor 2 *************/

#if  (_SENSORLIB2_ == _SENSORLIB2_CMOS_NT99141_)
SENSOR_DRVTAB* Sensor_GetDrvTab_NT99141(void);
#endif

#if  (_SENSORLIB2_ == _SENSORLIB2_CMOS_NT99141_TI_)
SENSOR_DRVTAB* Sensor_GetDrvTab_NT99141(void);
#endif

#if  (_SENSORLIB2_ == _SENSORLIB2_CMOS_TVP5150_)
SENSOR_DRVTAB* Sensor_GetDrvTab_TVP5150(void);
#endif

#if  (_SENSORLIB2_ == _SENSORLIB2_DUMMY_)
SENSOR_DRVTAB* Sensor_GetDrvTab_Dummy2(void);
#endif

#endif //_SENSOR_DRV_H
