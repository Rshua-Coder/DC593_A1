#include "GSensor.h"
#include "Gsensor_Combo.h"
#include "Gsensor_None.h"

#define __MODULE__          GSensor_NONE
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2       //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

static BOOL GSensor_NONE_open(void);
static BOOL GSensor_NONE_close(void);
static BOOL GSensor_NONE_GetStatus(Gsensor_Data *GS_Data);
static BOOL GSensor_NONE_ParkingMode(void);
static BOOL GSensor_NONE_CrashMode(void);
static void GSensor_NONE_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity);
static void GSensor_NONE_GetAxisValue(INT32 *pX, INT32 *pY, INT32 *pZ);
static void NONE_ClearIntSts(void);

static GSENSOR_OBJ g_GsensorNoneObj = {
	GSensor_NONE_open,
	GSensor_NONE_close,
	GSensor_NONE_GetStatus,
	GSensor_NONE_ParkingMode,
	GSensor_NONE_CrashMode,
	GSensor_NONE_SetSensitivity,
	GSensor_NONE_GetAxisValue,
	NONE_ClearIntSts
};

PGSENSOR_OBJ GSensor_NONE_getGSensorObj(void)
{
    return &g_GsensorNoneObj;
}

static BOOL GSensor_NONE_open(void)
{
	DBG_DUMP("GSensor_None is opened OK!!\r\n");
    return TRUE;
}

static BOOL GSensor_NONE_close(void)
{
    return TRUE;
}

static BOOL GSensor_NONE_GetStatus(Gsensor_Data *GS_Data)
{
    return FALSE;
}

static BOOL GSensor_NONE_ParkingMode(void)
{
    return FALSE;
}

static BOOL GSensor_NONE_CrashMode(void)
{
    return FALSE;
}

static void GSensor_NONE_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity)
{
	return;
}

static void GSensor_NONE_GetAxisValue(INT32 *pX, INT32 *pY, INT32 *pZ)
{
	*pX = 0;
	*pY = 0;
	*pZ = 0;
	return;
}

static void NONE_ClearIntSts(void)
{
	// Since we didnot use interrupt mode here.
	return;
}

