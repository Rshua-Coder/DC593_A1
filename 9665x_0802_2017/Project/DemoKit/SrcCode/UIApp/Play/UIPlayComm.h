//This source code is generated by UI Designer Studio.

#ifndef UIPLAYCOMM_H
#define UIPLAYCOMM_H

#include "NVTUserCommand.h"
typedef struct
{
    NVT_KEY_EVENT   NaviKey;        // Navigation key only. NVTEVT_KEY_UP,NVTEVT_KEY_DOWN,NVTEVT_KEY_LEFT,NVTEVT_KEY_RIGHT,
    UINT8   HorNums;                // How many images in horizontal direction, HorNums * VerNums must <=25
    UINT8   VerNums;                // How many images in vertical direction, HorNums * VerNums must <=25
} BROWSE_NAVI_INFO, *PBROWSE_NAVI_INFO;

#define MAX_THUMB_RECT_NUM 25
#define MAX_PLAY_BY_DATE_FILE_NUM 5000

extern URECT g_ThumbRectVideoComm[MAX_THUMB_RECT_NUM];
//#NT#2012/11/27#Scottie -begin
//#NT#Support Dual Display for PB
extern URECT g_ThumbRectVideoComm2[MAX_THUMB_RECT_NUM];
//#NT#2012/11/27#Scottie -end
extern void BrowseThumbUpdateLayout(URECT* pOSDRect, UINT32 uiThumbNum);
extern void BrowseThumbNaviKey(PBROWSE_NAVI_INFO pBrowseNavi);
extern void CloseAutoHideTimer(void);
extern void StopAutoHideTimer(void);
extern void ResetAutoHideTimer(void);
extern void ResetPlayKeySound(void);
//#NT#2011/03/30#Ben Wang -begin
//#NT#Add common key sound setting
extern void UIWndBackupKeySound(void);
extern void UIWndRestoreKeySound(void);
extern void UIWndSetActiveKeySound(UINT32 uiPressKeySound, UINT32 uiContKeySound);
//#NT#2011/03/30#Ben Wang -end
extern void ChkKeySoundComm(void);
extern void PlayNextPhotoComm(void);
extern void PlayPrevPhotoComm(void);
extern BOOL IsFreeSpaceEnough(void);
extern BOOL IsFileError(void);
extern BOOL IsMovieFile(void);
extern BOOL IsNoJpgFile(void);
extern BOOL IsPanoramaImg(void);
extern BOOL IsHDRImg(void);
extern BOOL IsOurImg(void);
extern BOOL IsHDMIPlugIn(void);
extern BOOL ExceedMaxPlayByDateFileNum(void);
//#NT#2011/01/31#Ben Wang -begin
//#NT#Add screen control to sync display with OSD
extern void SetVdoWinSizeForPB(UINT32 uiStartX, UINT32 uiStartY, UINT32 uiWidth, UINT32 uiHeight);
extern void UpdateVdoWinForPB(void);
//#NT#2011/01/31#Ben Wang -end
extern INT32 UIPlay_PlaySingle(UINT32 Command);
#endif
