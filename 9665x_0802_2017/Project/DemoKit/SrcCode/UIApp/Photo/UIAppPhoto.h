#ifndef _UIVIEWPHOTO_H_
#define _UIVIEWPHOTO_H_

#include "AppLib.h"

#include "UIPhotoObj.h"

#include "UIPhotoInfo.h"

#include "fd_lib.h"

#include "sd_lib.h"

#include "ae_api.h"

#include "awb_api.h"

extern UINT32 gPhotoFrameBuf;

extern UINT32 PhotoExe_GetMaxDzoomIdx(void);
extern UINT32 PhotoExe_GetDZoomIdx(void);
extern UINT32 PhotoExe_GetMinOzoomIdx(void);
extern UINT32 PhotoExe_GetMaxOzoomIdx(void);
extern UINT32 PhotoExe_GetCurOzoomIdx(void);
extern UINT32 PhotoExe_GetDZoomRatio(void);

#define DZOOM_IDX_MIN()         (0)
//#define UI_DZOOM_IDX_MIN        0

#define DZOOM_IDX_MAX()         PhotoExe_GetMaxDzoomIdx()
#define DZOOM_IDX_GET()         PhotoExe_GetDZoomIdx()

#define OZOOM_IDX_MIN()         PhotoExe_GetMinOzoomIdx()
#define OZOOM_IDX_MAX()         PhotoExe_GetMaxOzoomIdx()
#define OZOOM_IDX_GET()         PhotoExe_GetCurOzoomIdx()

#define CURR_EV()               AE_GetPrvEVValue()

// Zoom Control
#define UI_ZOOM_CTRL_STOP               0
#define UI_ZOOM_CTRL_IN                 1
#define UI_ZOOM_CTRL_OUT                2
#define UI_ZOOM_CTRL_RESET_DZOOM        3
#define UI_ZOOM_CTRL_RESET_OZOOM        4



#define PHOTO_FRAME_COUNT               3

// APP event class
typedef enum
{
    NVTEVT_EXE_MACRO = NVTEVT_PHOTO_MAX+1,
    NVTEVT_EXE_SELFTIMER,
    NVTEVT_EXE_FLASH,
    NVTEVT_EXE_EV,
    NVTEVT_EXE_SIZE,
    NVTEVT_EXE_QUALITY,
    NVTEVT_EXE_WB,
    NVTEVT_EXE_MWB_ADJUST,
    NVTEVT_EXE_MWB_RGB,
    NVTEVT_EXE_COLOR,
    NVTEVT_EXE_ISO,
    NVTEVT_EXE_AFWINDOW,
    NVTEVT_EXE_AFBEAM,
    NVTEVT_EXE_CONTAF,
    NVTEVT_EXE_METERING,
    NVTEVT_EXE_CAPTURE_MODE,
    NVTEVT_EXE_DATE_PRINT,
    NVTEVT_EXE_PREVIEW,
    NVTEVT_EXE_DIGITAL_ZOOM,
    NVTEVT_EXE_BSHOOT,
    NVTEVT_EXE_CONTSHOT,
    NVTEVT_EXE_TIMELAPSE,
    NVTEVT_EXE_BLINKDET,
    NVTEVT_EXE_SMILEDET, //for menu setting
    NVTEVT_EXE_FD,
    NVTEVT_EXE_ASCN,
    NVTEVT_EXE_PAN,
    NVTEVT_EXE_HDR,
    NVTEVT_EXE_SCENEMODE,
    NVTEVT_EXE_CAPTURE_START,
    NVTEVT_EXE_CAPTURE_STOP,
    NVTEVT_EXE_CAPTURE_END,
    NVTEVT_EXE_CAPTURE_SMILE,
    NVTEVT_EXE_ASPSCS,
    NVTEVT_EXE_PANCAPTURE,
    NVTEVT_EXE_ZOOM,
    NVTEVT_EXE_START_FUNC,
    NVTEVT_EXE_STOP_FUNC,
    NVTEVT_EXE_AF_PROCESS,
    NVTEVT_EXE_AF_RELEASE,
    NVTEVT_EXE_AF_WAITEND,
    NVTEVT_EXE_IMAGE_RATIO,
    NVTEVT_EXE_GEN_DATE_STR,
    NVTEVT_EXE_OBJTRACK,
    NVTEVT_EXE_PHOTO_FRAME,
    NVTEVT_EXE_ID_POSTPRS,
    NVTEVT_EXE_SINGLE_VIEW_OPEN,
    NVTEVT_EXE_SINGLE_VIEW_CLOSE,
    NVTEVT_EXE_PRV_POSTPRS_OPEN,
    NVTEVT_EXE_PRV_POSTPRS_CLOSE,
    NVTEVT_EXE_GDC,
    NVTEVT_EXE_DSCMODE,
    NVTEVT_EXE_SHARPNESS,
    NVTEVT_EXE_SATURATION,
    NVTEVT_NOTIFY_PREPARE_CAP,
    //#NT#2012/8/1#Philex - begin
    NVTEVT_EXE_CONTINUESHOT,
    NVTEVT_EXE_ANTISHAKING,
    //#NT#2012/8/1#Philex - end
    NVTEVT_EXE_GEN_DATE_PIC,
    NVTEVT_EXE_INIT_DATE_BUF,
    NVTEVT_EXE_IMAGE_EFFECT,
    NVTEVT_EXE_3DNR,
    NVTEVT_EXE_SUPER_RESOLUTION,
    NVTEVT_EXE_PLAY_SHUTTER_SOUND,
    NVTEVT_EXE_FOV,
#if 0  //move to UIAppPhotoEvent.h
    /////////////////////////// to be remove!
    NVTEVT_CB_ZOOM,
    NVTEVT_CALLBACK,
    NVTEVT_ALGMSG_FDEND,
    NVTEVT_ALGMSG_SDEND,
    NVTEVT_ALGMSG_FOCUSEND,
    NVTEVT_ALGMSG_ASCEND,
    NVTEVT_ALGMSG_FLASH,
    NVTEVT_ALGMSG_QVSTART,
    NVTEVT_ALGMSG_JPGOK,
    NVTEVT_ALGMSG_RESUMEKEY,
    NVTEVT_ALGMSG_CAPFSTOK,
    NVTEVT_ALGMSG_CAPTUREEND,
    NVTEVT_ALGMSG_SLOWSHUTTER,
    NVTEVT_ALGMSG_MWBCALEND,
    NVTEVT_ALGMSG_PDEND,
    NVTEVT_ALGMSG_PROGRESSEND,
    NVTEVT_ALGMSG_DZOOMSTEP,
#endif
}
CUSTOM_PHOTO_EVENT;

typedef enum
{
    ON_CAPTURE_SINGLE = 0,
    ON_CAPTURE_CONT,
    ON_CAPTURE_SD_CAPTURE,
    ON_CAPTURE_MAX
}PHOTO_ON_CAPTURE_PARM;


typedef enum
{
    SD_STOP = 0,
    SD_START,
    SD_CONTINUE,
    SD_MAX
}SD_PARM;


typedef enum
{
    OBJTRACK_STOP = 0,
    OBJTRACK_START,
    OBJTRACK_MAX
}OBJTRACK_PARM;


//  param NVTEVT_EXE_LOCK
#define UIAPP_PHOTO_AE    0x00000001
#define UIAPP_PHOTO_AWB   0x00000002
#define UIAPP_PHOTO_FD    0x00000004
#define UIAPP_PHOTO_CAF   0x00000008
#define UIAPP_PHOTO_ASC   0x00000010


#define UIAPP_PHOTO_WAITIDLE     TRUE
#define UIAPP_PHOTO_NOWAITIDLE   FALSE



typedef enum
{
    UIAPPPHOTO_Attr_FD =0,
    UIAPPPHOTO_Attr_SD,
    UIAPPPHOTO_Attr_CAF,
    UIAPPPHOTO_Attr_ASCN,
    UIAPPPHOTO_Attr_MAX
}UIAPPPHOTO_Attr;


typedef enum
{
    UIAPPPHOTO_CB_CAPSTART =0,
    UIAPPPHOTO_CB_STOP_CONTSHOT,
    UIAPPPHOTO_CB_FLASH,
    UIAPPPHOTO_CB_QVSTART,
    UIAPPPHOTO_CB_JPGOK,
    UIAPPPHOTO_CB_FSTOK,
    UIAPPPHOTO_CB_CAPTUREEND,
    UIAPPPHOTO_CB_SLOWSHUTTER,
    UIAPPPHOTO_CB_FDEND,
    UIAPPPHOTO_CB_SDEND,
    UIAPPPHOTO_CB_ASCEND,
    UIAPPPHOTO_CB_FOCUSEND,
    UIAPPPHOTO_CB_PREVIEW_STABLE,
    UIAPPPHOTO_CB_MAX
}UIAPPPHOTO_CB;


typedef enum
{
    UIAPPPHOTO_FRAME_CMD_LOADPRV_ADDR = 0,
    UIAPPPHOTO_FRAME_CMD_LOADPRV,
    UIAPPPHOTO_FRAME_CMD_LOADCAP,
    UIAPPPHOTO_FRAME_CMD_EN,
    UIAPPPHOTO_FRAME_CMD_DIS,
    UIAPPPHOTO_FRAME_CMD_MAX
}UIAPPPHOTO_FRAME_CMD;

typedef struct _PhotoFrameDataType
{
    UINT32        offset;
    UINT32        size;
}PhotoFrameDataType;

typedef struct _PhotoFrameInfoType
{
    UINT32             TotalLen;
    UINT32             Ver;
    UINT32             Header[2];
    PhotoFrameDataType Data[PHOTO_FRAME_COUNT*2];
}PhotoFrameInfoType;


typedef struct _UIAppPhotoExeInfoType
{
    UINT32                     FreePicNum;
    BOOL                       isFolderFull;
    BOOL                       isCardFull;
    BOOL                       isFocusEnd;
    BOOL                       isStartCapture;
    BOOL                       isContShotStateSetOn;
    BOOL                       isDoingContShot;
    BOOL                       isStopingContShot;
    BOOL                       isStartSmileDetect;       //
    UINT32                     uiTakePicNum;
    BOOL                       isAscnOn;                 //
    BOOL                       isCafOn;                  //
    BOOL                       isSdOn;                   //
    BOOL                       isFdOn;                   //
    BOOL                       IsJPGok;                  //
    BOOL                       IsAFProcess;              //
    BOOL                       BDstatus;
    BOOL                       IsCallBack;
    UINT32                     CallBackEvent;
    BOOL                       isPhotoFrameOn;           //
    BOOL                       isFlashTrigger;           //
    BOOL                       isSingleViewOpen;         //
    BOOL                       isPrvPostProcess;
    BOOL                       isAFBeam;
    UINT32                     uiMaxImageSize;
    BOOL                       isDzoomStart;
    UINT32                     ZoomCtrl;
    BOOL                       isStartHighSpeedTone;
    URECT                      FdDispCoord;
} UIAppPhotoExeInfoType;

extern UIAppPhotoExeInfoType *pPhotoExeInfo;
extern PhotoFrameInfoType     gPFInfo;
extern UINT32 PhotoExe_GetFreePicNum(void);
extern void   PhotoExe_SetData(UINT32 attribute,UINT32 value);
extern UINT32 PhotoExe_GetFdFaceNum(void);

//Photo Config
//#NT#2010/09/28#Jeah Yen -begin
extern ISIZE Photo_GetBufferSize(void);
//#NT#2010/09/28#Jeah Yen -end

//Photo Init
extern INT32 PhotoExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

//Photo Exe
extern INT32 PhotoExe_OnSelftimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnAFWindow(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnFlash(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnQuality(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnSceneMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnEV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnMetering(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnWB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnISO(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnColor(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnSaturation(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnSharpness(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnCaptureMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnQuickShot(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnDigitalZoom(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnFindBlur(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnPreview(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnDatePrint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnAFBeam (VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnContAF(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnBlinkDet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnPan(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
//Photo CB
extern void PhotoExe_DateImprint(void);
extern void Photo_RegCB(void);
extern VControl CustomPhotoObjCtrl;

void PhotoExe_OpenHttpLiveView(void);
void PhotoExe_CloseHttpLiveView(void);
#endif //_UIVIEWPHOTO_H_
