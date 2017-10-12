/**
    IPL Display control

    @file       IPL_Display_int.h
    @ingroup    mISYSAlg
    @note       This moudle is used to control the relationship of IME/display survice/video encoder/DIS

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_DISPLAY_H_
#define _IPL_DISPLAY_H_

#include "IPL_Utility.h"
#define DisplaySourcePath       3
#define DisplayTotalRingBuffer  10//8//5// 4

#if 0
 - Display Ctrl task
#endif
#define FLGPTN_DISPLAYCTRL_IDLE         FLGPTN_BIT(0)
#define FLGPTN_DISPLAYCTRL_RESET        FLGPTN_BIT(1)
#define FLGPTN_DISPLAYCTRL_CHANGE       FLGPTN_BIT(2)
#define FLGPTN_DISPLAYCTRL_LOCK         FLGPTN_BIT(3)
#define FLGPTN_DISPLAYCTRL_OUTPUT_READY         FLGPTN_BIT(4)

typedef struct
{
    UINT32 FrameIdx;
    UINT32 OffsetX;
    UINT32 OffsetY;
} DIS_Infor;

typedef struct
{
    INT32 MotionX;
    INT32 MotionY;
} Disp_Motion_Infor;

/**
    Open display control task
    @return None
*/
extern void IPL_Display_OpenTsk(void);


/**
    Close display control task
    @return None
*/
extern void IPL_Display_CloseTsk(void);


/**
    Change buffer index, and it is called by IME_ISR
    @return None
*/
extern void IPL_AskDisplay2ChangeBuffer(void);


/**
    Get specified IME output buffer
    
    @param[in] UINT32 IME output path
    @param[in] UINT32 buffer index ID
    @return IPL_YCC_IMG_INFO
        - @b image information(type, address, size)
*/
extern IPL_YCC_IMG_INFO IPL_GetSpecifiedIMEoutputAddr(UINT32 path, UINT32 BufferID);


/**
    Reset buffer control flow, and DisplayCtrl_Tsk() will control buffer automaticly.
    @return None
*/
extern void IPL_ResetDisplayCtrlFlow(void);


/**
    Enable/disable to freeeze image buffer. If enable that, the display image will be frozen.

    @param[in] BOOL enable/disable
    @return None
*/
extern void IPL_FreezeDisplayImage(BOOL OnOFF);


/**
    Get ring buffer number

    @return UINT32
        - @b ring buffer number
*/
extern UINT32 IPL_GetDisplayCtrlBufferNum(void);


extern UINT32 IPL_GetDisplayCtrlBufferNum3(void);


/**
    Assign buffer address to specified buffer

    @param[in] UINT32 specified path
    @param[in] UINT32 specified buffer id
    @param[in] IPL_YCC_IMG_INFO Y/UV buffer address
    @return None
*/
extern void IPL_InitDisplayBuffer(UINT32 Path, UINT32 BufferID,IPL_YCC_IMG_INFO *BufferInfo);


/**
    Get ready buffer address after post processing
    
    @param[in] UINT32 IME output path
    @return IPL_YCC_IMG_INFO
        - @b image information(type, address, size)
*/
extern IPL_YCC_IMG_INFO IPL_GetIMEReadyBufferAddr(UINT32 path);


/**
    Get ready buffer address of display after post processing
    
    @return IPL_YCC_IMG_INFO
        - @b image information(type, address, size)
*/
extern IPL_YCC_IMG_INFO IPL_GetDisplayBufferAddr(void);


/**
    Enable/disable post processing to specified IME output path

    @param[in] UINT32 IME output path
    @param[in] BOOL enable/disable post processing
    @return None
*/
extern void IPL_SetPostImageProcessOnOff(UINT32 Path, BOOL Enable);


/**
    Get post processing status(enable/disable) to specified IME output path

    @param[in] UINT32 IME output path
    @return BOOL
        - @b enable/disable
*/
extern BOOL IPL_GetPostImageProcessOnOff(UINT32 Path);


/**
    callback function in IME_ISR to exchange address & size information

    @param[in] UINT32 IME output image index(count)
    @param[in/out] IPL_YCC_IMG_INFO exchange IME output path1 information. in: type, size. out: next buffer address
    @param[in/out] IPL_YCC_IMG_INFO exchange IME output path2 information. in: type, size. out: next buffer address
    @param[in/out] IPL_YCC_IMG_INFO exchange IME output path3 information. in: type, size. out: next buffer address
    @param[in] fpCallBack callback for post processing done
    @return None
*/
extern void IPL_CtrlPostImage(UINT32 ImgIdx, IPL_YCC_IMG_INFO* path1,IPL_YCC_IMG_INFO* path2,IPL_YCC_IMG_INFO* path3,fpCallBack fpDone);


/**
    Set each device(path) size, such as LCD, HDMI, Video, etc.

    @param[in] UINT32 IME output path
    @param[in] IPL_YCC_CH_INFO device size info 
    @return None
*/
extern void IPL_SetDeviceInputInfo(UINT32 path, IPL_YCC_CH_INFO SizeInfo);


/**
    Pause display control
    @return None
*/
extern void IPL_Display_Pause(void);


/**
    Resume display control
    @return None
*/
extern void IPL_Display_Resume(void);


/**
    Check buffer ready or not
    
    @return BOOL
        - @b TRUE/FALSE
*/
extern BOOL IPL_Display_ChkBufferReady(void);


/**
    Set skip frames to ignore invaild image befor starting display
    If user don,t set this parameter, the default value is 2

    @param[in] UINT32 skip frames
    @return None
*/
extern void IPL_Display_InitSkipFrame(UINT32 SkipFrame);

/**
    Set IME output path to display
 
    @param[in] UINT32 IME output path (0~2)
 
    @return none
*/
extern void IPL_SetDisplayInputPath(UINT32 path);

/**
    Get next IME output path information
 
    @param[in] UINT32 IME output path (0~2)
 
    @return IPL_YCC_IMG_INFO
        - @b image information(type, address, size)
*/
extern IPL_YCC_IMG_INFO IPL_GetNextIMEoutputAddr(UINT32 path);

/**
    The display control task, should be installed by OS
    @return None
*/
extern void IPL_DisplayCtrl_Tsk(void);


/**
    Set IME path3 to dual display source (HDMI)
    This API should be called before IPL_ResetDisplayCtrlFlow( )

    @param[in] UINT32 IME output path (0~2)

    @return None
*/
extern void IPL_DisplayDualOutput(BOOL DualOutput);

extern void IPL_Display_SetBufferNum(UINT32 BufNum);
extern void IPL_Display_SetBufferNum3(UINT32 BufNum);

extern Disp_Motion_Infor IPL_GetIMEReadyBufferMotion(UINT32 path);

#endif
