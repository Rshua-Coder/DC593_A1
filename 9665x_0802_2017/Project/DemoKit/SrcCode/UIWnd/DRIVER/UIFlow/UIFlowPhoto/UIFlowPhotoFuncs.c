#include <stdio.h>
#include "UIFramework.h"
#include "FileSysTsk.h"
#include "UIFlow.h"


PHOTO_TASK_DATA gPhotoData = { 0 };

static UINT32 g_uiFreePicNum        = 0;
static CHAR    g_cFreePicNumStr[8]     = {0};
static BOOL    g_SDStatus = FALSE;

#if 0
UINT32 Get_FreePicNum(UINT32 uiSizeIndex, UINT32 uiQualityIndex)
{
    UINT32              fileNum;
    UINT64              uiFreeSpace,uiGetFreeSpace;
    UINT32              uiCompressRate;
    UINT32              freeImgNum;
    UINT32              reserveSize = 0x100000;
    UINT32              MinCaptureSize;
    UINT8               CompressRateTable[QUALITY_ID_MAX]
                        ={
                           23,  // quality fine,        25%
                           14,  // quality standard,    15%
                           9    // quality economy,     10%
                         };

    uiGetFreeSpace = FileSys_GetDiskInfo(FST_INFO_FREE_SPACE);
    if (UI_GetData(FL_CardStatus) == CARD_REMOVED)
    {
        if(uiGetFreeSpace < (reserveSize/2))
        {
            uiFreeSpace = 0;
        }
        else
        {
            uiFreeSpace = uiGetFreeSpace;
        }
    }
    else
    {
        if(uiGetFreeSpace < reserveSize)
        {
            uiFreeSpace = 0;
        }
        else
        {
            uiFreeSpace = uiGetFreeSpace - reserveSize;
        }
    }

    debug_ind(("Free Space = %ld\r\n", uiFreeSpace));

    uiCompressRate = CompressRateTable[uiQualityIndex];

    //size = Get_SizeValue();
    //MinCaptureSize = (JPGIMG_SIZE[size][0] * JPGIMG_SIZE[size][1] * 3) * uiCompressRate / 200;
    debug_ind(("MinCaptureSize = %ld K \r\n", MinCaptureSize/1024));

    // Reserve double size for last file.
    if(uiFreeSpace < MinCaptureSize * 2)
    {
    if (UI_GetData(FL_CardStatus) == CARD_REMOVED)
    {
            //FilesysGetDCFFileTypeNum(FST_FMT_JPG,&fileNum);

            if(fileNum == 0)
            {
                freeImgNum = 1;
    }
    else
    {
                freeImgNum = 0;
            }

            if(uiFreeSpace < (reserveSize/2))
            {
                freeImgNum = 0;
            }
        }
        else
        {
            freeImgNum = 0;
        }
    }
    else    // uiFreeSpace = (N-1)*MinCaptureSize + MinCaptureSize*2
    {
        if (UI_GetData(FL_CardStatus) == CARD_REMOVED)
        {
            //FilesysGetDCFFileTypeNum(FST_FMT_JPG,&fileNum);

            if(fileNum == 0)
        {
                freeImgNum = 1;
        }
            else
        {
                freeImgNum = 0;
        }
        }
        else
        {
            freeImgNum = (uiFreeSpace - MinCaptureSize * 2) / MinCaptureSize + 1;
        }
    }

    debug_err((">> Free ImgNum = %ld\r\n", freeImgNum));

    return freeImgNum;
 }
#endif

CHAR *Get_FreePicNumString(UINT32 uiValue)
{
    sprintf(g_cFreePicNumStr, "%05ld", uiValue);
    return g_cFreePicNumStr;
}

void FlowPhoto_DoCapture(void)
{
    switch(SysGetFlag(FL_CONTINUE_SHOT))
    {
      case CONTINUE_SHOT_OFF:
           UI_SetData(FL_ContShotIndex, CONT_SHOT_OFF);
           // update DCF Folder/File name
           FlowDCF_UpdateName();
           Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_START,1, ON_CAPTURE_SINGLE);
        break;
      case CONTINUE_SHOT_ON:
           UI_SetData(FL_ContShotIndex, CONT_SHOT_10);
           // update DCF Folder/File name
           FlowDCF_UpdateName();
           Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_START,1, ON_CAPTURE_CONT);
        break;
    }
}

BOOL FlowPhoto_GetSDStatus(void)
{
    return g_SDStatus;
}

void FlowPhoto_SetFdSdProc(BOOL sd_start)
{
    g_SDStatus = sd_start;
    switch (SysGetFlag(FL_FD))
    {
    case FD_OFF:
        Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_FD, 1, FD_OFF);
        Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SMILE, 1, SD_STOP);
        break;
    case FD_ON:
        Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_FD, 1, FD_ON);
        Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SMILE, 1, SD_STOP);
        break;
    case FD_SMILE:
        Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_FD, 1, FD_ON);
        if (sd_start==TRUE)
        {
            /* Set smile detection start flag */
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SMILE, 1, SD_START);
        } else {
            /* Set smile detection start flag */
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SMILE, 1, SD_CONTINUE);
        }
        // restore FL_FD flag to FD_SMILE
        SysSetFlag(FL_FD,FD_SMILE);

        break;
    }
}

void FlowPhoto_ForceLockFdSd(void)
{
//    Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_FD, 1, FD_OFF);
    Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SMILE, 1, SD_STOP);
}

UINT32 FlowPhoto_GetFreePicNum(void)
{
    return g_uiFreePicNum;
}

BOOL FlowPhoto_IsStorageErr(void)
{
    BOOL    retV = FALSE;

    if(UIStorageCheck(STORAGE_CHECK_ERROR, NULL) == TRUE)
    {
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,FLOWWRNMSG_ISSUE_MEM_ERR,FLOWWRNMSG_TIMER_KEEP);
        return TRUE;
    }
    if(UIStorageCheck(STORAGE_CHECK_FULL, &(g_uiFreePicNum)) == TRUE)
    {
        if (UI_GetData(FL_CardStatus) == CARD_REMOVED)
        {
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_PLEASE_INSERT_SD,FLOWWRNMSG_TIMER_2SEC);
        }
        else
        {
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_FULL,FLOWWRNMSG_TIMER_2SEC);
        }
        return TRUE;
    }
    if(UIStorageCheck(STORAGE_CHECK_LOCKED, NULL) == TRUE)
    {
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_LOCKED,FLOWWRNMSG_TIMER_2SEC);
        return TRUE;
    }

    return retV;
}

void FlowPhoto_OnTimer01SecIndex(void)
{

    if ((gPhotoData.SelfTimerCount % 10 == 0) && (gPhotoData.SelfTimerCount != 0))
        {
            FlowPhoto_IconDrawSelftimerTime(&UIFlowWndPhoto_StaticTXT_SelftimerCntCtrl, gPhotoData.SelfTimerCount/10);
            UISound_Play(DEMOSOUND_SOUND_KEY_TONE);
        }

        if (gPhotoData.SelfTimerCount == 0)
        {
            UISound_Play(DEMOSOUND_SOUND_KEY_TONE);
            FlowPhoto_IconHideSelftimer(&UIFlowWndPhoto_StatusICN_SelftimerCtrl);
            FlowPhoto_IconHideSelftimerTime(&UIFlowWndPhoto_StaticTXT_SelftimerCntCtrl);

            /* Clear the whole OSD screen */
            UxCtrl_SetAllChildShow(&UIFlowWndPhotoCtrl,FALSE);

            gPhotoData.State = PHOTO_ST_CAPTURE;
            FlowPhoto_DoCapture();
        }
}

void FlowPhoto_OnTimer1SecIndex(void)
{
    switch (gPhotoData.State)
    {
    case PHOTO_ST_VIEW:
    case PHOTO_ST_VIEW | PHOTO_ST_ZOOM:
    case PHOTO_ST_SELFTIMER:
        gPhotoData.SysTimeCount++;
        break;
    }
}
