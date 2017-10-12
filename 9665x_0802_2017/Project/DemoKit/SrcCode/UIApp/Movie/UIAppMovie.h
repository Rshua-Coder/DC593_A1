#ifndef _UIVIEWMOVIE_H_
#define _UIVIEWMOVIE_H_

#include "AppLib.h"
#include "af_api.h"
#include "UIMovieObj.h"
#include "UIMovieInfo.h"
#include "UIBackgroundObj.h"

#include "ldws_alg.h"
#include "ldws_parms.h"
extern void UIMovie_LDWSCB(LDWS_CBMSG uiMsgID, void *Data);
//#NT#2013/2/6#Philex -begin
// include "GPS.h"
#if (UI_STYLE==UI_STYLE_DRIVER)
#include "GPS.h"
#endif
//#NT#2013/2/6#Philex -end

//-------Debug Define---------------------------------------------
#define _UIMOVEXE_DBG_MSG_NONE_           0
#define _UIMOVEXE_DBG_MSG_CHK_            1
#define _UIMOVEXE_DBG_MSG_IO_             2
#define _UIMOVEXE_DBG_MSG_ALL_            3
#define _UIMOVEXE_DBG_MSG_LVL_            _UIMOVEXE_DBG_MSG_NONE_
#if (_UIMOVEXE_DBG_MSG_LVL_ == _UIMOVEXE_DBG_MSG_NONE_)
    #define DbgMsg_UIMovExeIO(msg)
    #define DbgMsg_UIMovExe(msg)
    #define DbgCode_UIMovExe(x)
#elif (_UIMOVEXE_DBG_MSG_LVL_ == _UIMOVEXE_DBG_MSG_ALL_)
    #include "Debug.h"
    #define DbgMsg_UIMovExeIO(msg)      debug_msg msg
    #define DbgMsg_UIMovExe(msg)        debug_msg msg
    #define DbgCode_UIMovExe(x)         x
#elif (_UIMOVEXE_DBG_MSG_LVL_ == _UIMOVEXE_DBG_MSG_CHK_)
    #include "Debug.h"
    #define DbgMsg_UIMovExeIO(msg)
    #define DbgMsg_UIMovExe(msg)        debug_msg msg
    #define DbgCode_UIMovExe(x)         x
#elif (_UIMOVEXE_DBG_MSG_LVL_ == _UIMOVEXE_DBG_MSG_IO_)
    #include "Debug.h"
    #define DbgMsg_UIMovExeIO(msg)      debug_msg msg
    #define DbgMsg_UIMovExe(msg)
    #define DbgCode_UIMovExe(x)
#endif
//------------------------------------------------------

// APP event class

typedef enum
{
    NVTEVT_EXE_MOVIESIZE     = NVTEVT_MOVIE_MAX+1,
    NVTEVT_EXE_MOVIEQUALITY,
    //#NT#2011/04/15#Photon Lin -begin
    //#NT#Add AV sync mechanism
    NVTEVT_EXE_MOVIEAVSYNC,
    //#NT#2011/04/15#Photon Lin -end
    NVTEVT_EXE_MOVIECOLOR,
    NVTEVT_EXE_MOVIE_EV,
    NVTEVT_EXE_MOVIEAUDPLAYVOLUME,
    NVTEVT_EXE_MOVIEAUDRECVOLUME,
    NVTEVT_EXE_MOVIEDZOOM,
    NVTEVT_EXE_MOVIECONTAF,
    NVTEVT_EXE_MOVIEMETERING,
    NVTEVT_EXE_MOVIEDIS,
    NVTEVT_EXE_MOVIE_MCTF,
    NVTEVT_EXE_MOVIE_RSC,
    NVTEVT_EXE_MOVIE_HDR,
    NVTEVT_EXE_MOVIEGDC,
    NVTEVT_EXE_MOVIESMEAR,
    NVTEVT_EXE_MOVIEDIS_ENABLE,
    NVTEVT_EXE_MOVIE_DATE_IMPRINT,
    NVTEVT_EXE_MOVIE_AUTO_REC,
    //#NT#2012/07/31#Hideo Lin -begin
    NVTEVT_EXE_MOVIE_REC_START,
    NVTEVT_EXE_MOVIE_REC_STOP,
    NVTEVT_EXE_MOVIE_REC_PIM,    // picture in movie (still image is full resolution)
    NVTEVT_EXE_MOVIE_REC_RAWENC, // raw encode (still image size is equal to or smaller than video size)
    //#NT#2012/07/31#Hideo Lin -end
    NVTEVT_EXE_INITPLAYAVI,
#if 0
    NVTEVT_CB_MOVIE_START,
    NVTEVT_CB_MOVIE_ONE_SEC,
    NVTEVT_CB_MOVIE_FINISH,
    NVTEVT_CB_MOVIE_CONTINUOUS_RECORD,
    NVTEVT_CB_MOVIE_FULL,
    NVTEVT_CB_MOVIE_SLOW,
    //#NT#2011/01/03#Photon Lin -begin
    //#NT#Add slow media
    NVTEVT_CB_MOVIE_SLOWMEDIA,
    //#NT#2011/01/03#Photon Lin -end
    //#NT#2011/02/10#Photon Lin -begin
    //#Add CB for file access with case of card full
    NVTEVT_CB_MOVIE_FILEACCESS,
    //#NT#2011/02/10#Photon Lin -end
    NVTEVT_CB_MOVIE_ONE_VIDEOFRAME,
    NVTEVT_CB_MOVIE_OVERTIME,
#endif
    //#NT#2012/07/30#Philex Lin -begin
#if 0
    NVTEVT_CB_MOVIE_WR_ERROR,
    NVTEVT_CB_PREVIEWSTABLE,
    NVTEVT_CB_OZOOMSTEPCHG,
    NVTEVT_CB_DZOOMSTEPCHG,
#endif
    NVTEVT_EXE_DUAL_REC,
    NVTEVT_EXE_CYCLIC_REC,
    NVTEVT_EXE_MOTION_DET,
    NVTEVT_EXE_DATEIMPRINT,
    NVTEVT_EXE_MOVIE_AUDIO,
    //#NT#2012/07/30#Philex Lin -end
    //#NT#2010/10/05#Lily Kao -begin
    //#Changeable Manufacture,Product names by project
    NVTEVT_EXE_SET_MOV_MANUNAME,
    //#NT#2010/10/05#Lily Kao -end
    NVTEVT_EXE_GSENSOR,
    NVTEVT_EXE_MOVIE_FDEND,
    NVTEVT_EXE_MOVIE_FD,
    NVTEVT_EXE_MOVIE_FOV,
    NVTEVT_EXE_MOVIE_WATERLOGO,
    NVTEVT_EXE_MOVIE_PIPVIEW,
	NVTEVT_EXE_PARK_WAKEUP,      // parking wake up function
}CUSTOM_MOVIE_EVENT;

//#NT#2013/2/6#Philex -begin
#if (UI_STYLE==UI_STYLE_DRIVER)
typedef struct {
    char            IQVer[16];
    char            IQBuildDate[16];
    RMCINFO         rmcinfo;

//#NT#2011/10/14#Spark Chou -begin
//#NT# Video IQ debug info
#if _MOVIE_IQLOG_
    char    IQDebugInfo[5120];
#endif
//#NT#2011/10/14#Spark Chou -end
} GPSDATA,*pGPSDATA;
#endif

//Movie Config
//#NT#2010/09/28#Jeah Yen -begin
extern ISIZE Movie_GetBufferSize(void);
//#NT#2010/09/28#Jeah Yen -end

//Movie Exe
extern INT32 MovieExe_OnContAF(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 MovieExe_OnMetering(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 MovieExe_OnDis(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 MovieExe_OnGdc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 MovieExe_OnSmear(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern void Movie_RegCB(void);

//Movie CB
//extern void Movie_RecCB(UINT32 uiEventID, UINT32 uiSeconds);
//extern void Movie_DataStampCB(UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth);
//extern void Movie_RawEncCB(UINT32 uiEventID);
//extern void Movie_OnChangeSize(ISIZE *pSize, ISIZE *pOldSize);
extern void UIMovie_AudFilterProc(void);
extern void UIMovie_ImmProc(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3);
extern void UIMovie_MovieFileNamingCB(UINT32 uiPathId, char *pFileName);
extern void UIMovie_EMRMovFileNamingCB(UINT32 uiPathId, char *pFileName);
extern void UIMovie_PhotoFileNamingCB(UINT32 uiPathId, char *pFileName);
extern void UIMovie_PhotoFileNamingCB_EngMode(UINT32 uiPathId, char *pFileName);
extern void UIMovie_AFCB(AF_CBMSG uiMsgID, void *Data);
//#NT#2012/11/07#Calvin Chang#API function for Lens state updating -begin
extern void UIMovie_UpdateLensState(UINT32 LensState);
//#NT#2012/11/07#Calvin Chang -end
extern UINT32 UIMovie_3DNRAdjust(void);

extern VControl CustomMovieObjCtrl;

#if (UI_STYLE==UI_STYLE_DRIVER)
extern void Movie_SetSDSlow(BOOL IsSlow);
#endif

//#NT#2013/10/29#Isiah Chang -begin
//#Implement YUV merge mode of recording func.
extern void   FlowMovie_RecSetYUVMergeMode(BOOL bEnable);
extern BOOL   FlowMovie_RecGetYUVMergeMode(void);
extern void   FlowMovie_RecSetYUVMergeRecInterval(UINT32 uiSec);
extern UINT32 FlowMovie_RecGetYUVMergeRecInterval(void);
extern void   FlowMovie_RecSetYUVMergeRecCounter(UINT32 uiCount);
extern UINT32 FlowMovie_RecGetYUVMergeRecCounter(void);
extern void   FlowMovie_RecYUVMergeCounterInc(void);
//#NT#2013/10/29#Isiah Chang -end

#endif //_UIVIEWMOVIE_H_
