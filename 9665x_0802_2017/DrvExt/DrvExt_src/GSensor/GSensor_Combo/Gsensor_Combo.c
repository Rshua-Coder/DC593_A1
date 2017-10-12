#include "Type.h"
#include "i2c.h"
#include "GSensor.h"
#include "Gsensor_Combo.h"

#define __MODULE__          GSensor_Combo
//#define __DBGLVL__ 0        //OFF mode, show nothing
//#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGLVL__ 1       //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

static BOOL  g_bGsensorOpened = FALSE;
static PGSENSOR_OBJ  g_pGSensorObj = NULL;

static PGSENSOR_OBJ (*g_GetGSensorFunc[])(void) = {
	GSensor_NONE_getGSensorObj,
	GSensor_BMA253_getGSensorObj,
};


BOOL GSensor_IsOpened(void)
{
	return g_bGsensorOpened;
}

BOOL GSensor_open(void)
{
	UINT32 i;

	if (g_bGsensorOpened == TRUE) {
		DBG_IND("Gsensor open already\r\n");
		return TRUE;
	}

	for (i = (GSENSOR_MAX-1); i >= GSENSOR_NONE; i--) {
		g_pGSensorObj = g_GetGSensorFunc[i]();
		if ((g_pGSensorObj->open()) == TRUE) {
			g_bGsensorOpened = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL GSensor_close(void)
{
	if (g_bGsensorOpened == FALSE) {
		debug_msg("I2C close already\r\n");
		return TRUE;
	}

	g_pGSensorObj->close();
	GSensor_I2C_Exit();
	g_bGsensorOpened = FALSE;

	return TRUE;
}

BOOL GSensor_GetStatus(Gsensor_Data *GS_Data)
{
	if (g_bGsensorOpened == FALSE)
		return FALSE;
	return g_pGSensorObj->GetStatus(GS_Data);
}

void GSensor_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity)
{
	if (g_bGsensorOpened == FALSE)
		return;
	g_pGSensorObj->SetSensitivity(GSensorSensitivity);
}

void GSensor_GetAxisValue(INT32 *pX, INT32 *pY, INT32 *pZ)
{
	if (g_bGsensorOpened == FALSE)
		return;
	g_pGSensorObj->GetAxisValue(pX, pY, pZ);
}

BOOL GSensor_ParkingMode(void)
{
	if (g_bGsensorOpened == FALSE)
		return FALSE;
	return g_pGSensorObj->ParkingMode();
}

void ClearIntSts(void)
{
	if (g_bGsensorOpened == FALSE)
		return;
	g_pGSensorObj->ClearIntSts();
}

BOOL GSensor_CrashMode(void)
{
	if (g_bGsensorOpened == FALSE)
		return FALSE;
	return g_pGSensorObj->CrashMode();
}

