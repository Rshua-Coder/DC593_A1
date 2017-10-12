#ifndef _IPL_CALAPI_H_
#define _IPL_CALAPI_H_
#include "Type.h"
#include "errorno.h"
#include "ipl_ctrl_par.h"
#include "IPL_Calibration_3A.h"
#include "IPL_Calibration_ALG.h"
#include "IPL_Calibration_BUF.h"

typedef struct
{
    void*   pDrv_Sensor;
    void*   pCal_Sensor;
    fp_ImgRdy   fpRdyImg;//CB at ime FMD
}CTRL_PTR,*pCTRL_PTR;

//define for SetMode callback
typedef void (*fptr)(pIPLC_INFO pOutInfo,pCTRL_PTR pInfo);

/**
    calibration pipeline mode
*/
typedef enum
{
    CAL_IPL_NONE    = 0,
    CAL_IPL_STOP    = 1,    ///< pipeline YCC part stop and close
    CAL_IPL_SIESTOP = 2,    ///< SIE stop
    CAL_IPL_SIECLOSE= 3,    ///< SIE close
    CAL_IPL_PRV     = 4,    ///< streaming
    CAL_IPL_CAP     = 5,    ///< single image
    ENUM_DUMMY4WORD(CAL_IPL_MODE)
}CAL_IPL_MODE;

/**
    calibration bayer info
*/
typedef struct
{
    IPL_RAW_IMG_INFO BaseInfo;
    Coordinate       ActStart;
    Coordinate       CropStart;
}IPL_RAW_IMG;

/**
    Calibration object
*/
typedef struct
{
    ER                  (*fpSetMode)(CAL_IPL_MODE,pCTRL_PTR,fptr);
    IPL_RAW_IMG         (*fpGetRawInfo)(void);
    ER                  (*fpSaveRAW)(UINT32, UINT32, UINT32);
    pCAL_3A_OBJ         (*fp3AEvent)(void);
    pCAL_BUF_OBJ        (*fpBufEvent)(void);
    pCAL_ALG_OBJ        (*fpAlgEvent)(void);
} CalApi_Obj, *pCalApi_Obj;

/**
    calibration application
*/
extern pCalApi_Obj CalApi(void);

#endif