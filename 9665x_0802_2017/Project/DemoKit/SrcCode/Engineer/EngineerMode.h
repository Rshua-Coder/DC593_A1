#ifndef _ENGINEERMODE_H
#define _ENGINEERMODE_H

#define ENG_MODE_FILE  "A:\\engmode"

extern volatile BOOL  g_bCalbirationOpened;

extern void Engineer_InstallID(void) _SECTION(".kercfg_text");
extern BOOL EngineerMode_CheckEng(void);
extern void EngineerMode_Open(void);
extern BOOL IsEngineerModeOpened(void);
extern void EngineerMode_WndOpen(void);
extern void EngineerMode_WndClose(void);
extern BOOL IsEngineerModeChange(void);
extern void SetEngineerModeChange(BOOL b);
extern BOOL IsEngineerMode(void);
extern BOOL IsForcedSetPCC(void);
extern BOOL IsAELog(void);
extern BOOL IsADASLog(void);
extern BOOL IsWiFiTest(void);
extern BOOL IsResetToDefault(void);
extern ER EngineerMode_TurnOnWiFi(void);
extern void EngineerMode_SaveWiFiMAC(void);
extern char* EngineerMode_ReadAPMAC(void);
extern char* EngineerMode_ReadWiFiMAC(void);
extern char* EngineerMode_GetWiFiMAC(void);
extern ER EngineerMode_CheckWiFiCrystal(void);
extern BOOL EngineerMode_CheckSNFile(void);
extern void EngineerMode_SaveSerialNum(void);
extern char* EngineerMode_ReadSerialNum(void);
extern void EngineerMode_CheckGPSTest(void);
#endif
