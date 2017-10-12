/** \addtogroup mIPRJAPTest */
//@{

#ifndef _CALIBRATIONITEM_H
#define _CALIBRATIONITEM_H

#include "Type.h"


extern void     Cal_SenDP_Bright(void);
extern void     Cal_SenDP_Dark(void);
extern void     Cal_SenISO(void);
extern void     Cal_SenAWB(void);
extern void     Cal_SenECS(void);
extern void     Cal_SenDNP(void);
//#NT#2016/05/18#Silvia Wu -begin
//#NT# for burn in test
extern void     Cal_SenDNPBurnIn(void);
//#NT#2016/05/18#Silvia Wu -end
extern void     Cal_ShowCurEnv(void);
extern void     Cal_GetCalRst(void);
extern void     Cal_Reset_Data(void);
extern void     Cal_SenFocus(void);
extern void     Cal_ImageTest(void);

extern ER Cal_IplOpen(UINT32 BufAddr, UINT32 BufSize);
extern ER Cal_IplInit (void);
extern ER Cal_IplUnInit (void);
extern SX_CMD_ENTRY* UiCalTest_GetCmdEntry(void);
extern void CalTest_InstallCmd(void);
#endif

//@}
