#ifndef _AFDRV_DCJ36_G2007_650_H_
#define _AFDRV_DCJ36_G2007_650_H_
#include "IPL_AlgInfor.h"
#include "af_alg.h"

typedef struct _AFDRV_MANUAL
{
    INT16 iManualStartIdx;
    INT16 iManualEndIdx;
    INT8  iManualStep;
}AFDRV_MANUAL;

typedef enum
{
    Model_Normal = 0,
    Model_FallOff_Strong,
    ENUM_DUMMY4WORD(AF_AETRANSFERMODE)
}AF_AETRANSFERMODE;//Decide AFAE transfer mode

extern UINT16 AFdrv_GetFocusDist(void);
extern void AFdrv_SetManualRange(AFDRV_MANUAL ManualSet);
extern void AFdrv_GetCurrentRange(PAFALG_INFO_IN af_Obj, SEL_AFMODE AFMode);
extern void AFdrv_AFPrepare(BOOL bOP, UINT32 step);
extern void AFdrv_AEPrepare(BOOL bOP, UINT32 step);
#if 0 // VA TBD
extern void AFdrv_VAPrepare(BOOL bface, BOOL bOP, UINT32 step);
#endif
extern void AFdrv_SetAFModeChange(BOOL EN);
extern BOOL AFdrv_CheckAFModeChange(void);
//extern void AFdrv_AASwitch(BOOL bOP);
extern AF_AETRANSFERMODE AFdrv_GetAFModel(void);

//Temp
extern UINT16 AF_GetFocusDist(void);

#endif
