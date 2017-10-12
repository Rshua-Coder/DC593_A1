/**
    IPL Display control

    @file       IPL_Display_int.h
    @ingroup    mISYSAlg
    @note       This moudle is used to control the relationship of IME/display survice/video encoder/DIS

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_DISPLAY2_H_
#define _IPL_DISPLAY2_H_

#include "IPL_Utility.h"
#define DisplaySourcePath2       2
#define DisplayTotalRingBuffer2  10//5// 4

#if 0
 - Display Ctrl task
#endif
#define FLGPTN_DISPLAYCTRL_IDLE_2         FLGPTN_BIT(0)
#define FLGPTN_DISPLAYCTRL_RESET_2        FLGPTN_BIT(1)
#define FLGPTN_DISPLAYCTRL_CHANGE_2       FLGPTN_BIT(2)
#define FLGPTN_DISPLAYCTRL_LOCK_2         FLGPTN_BIT(3)
#define FLGPTN_DISPLAYCTRL_OUTPUT_READY_2         FLGPTN_BIT(4)



/**
    Open display control task
    @return None
*/
extern void IPL_Display_OpenTsk2(void);


/**
    Close display control task
    @return None
*/
extern void IPL_Display_CloseTsk2(void);


/**
    Change buffer index, and it is called by IME_ISR
    @return None
*/
extern void IPL_AskDisplay2ChangeBuffer2(void);


/**
    Reset buffer control flow, and DisplayCtrl_Tsk() will control buffer automaticly.
    @return None
*/
extern void IPL_ResetDisplayCtrlFlow2(void);


/**
    Enable/disable to freeeze image buffer. If enable that, the display image will be frozen.

    @param[in] BOOL enable/disable
    @return None
*/
extern void IPL_FreezeDisplayImage2(BOOL OnOFF);


/**
    Get ring buffer number

    @return UINT32
        - @b ring buffer number
*/
extern UINT32 IPL_GetDisplayCtrlBufferNum2(void);


/**
    Assign buffer address to specified buffer

    @param[in] UINT32 specified path
    @param[in] UINT32 specified buffer id
    @param[in] IPL_YCC_IMG_INFO Y/UV buffer address
    @return None
*/
extern void IPL_InitDisplayBuffer2(UINT32 Path, UINT32 BufferID,IPL_YCC_IMG_INFO *BufferInfo);


/**
    Get ready buffer address after post processing

    @param[in] UINT32 IME output path
    @return IPL_YCC_IMG_INFO
        - @b image information(type, address, size)
*/
extern IPL_YCC_IMG_INFO IPL_GetReadyBufferAddr2(UINT32 path);


/**
    Get ready buffer address of display after post processing

    @return IPL_YCC_IMG_INFO
        - @b image information(type, address, size)
*/
extern IPL_YCC_IMG_INFO IPL_GetDisplayBufferAddr2(void);


/**
    Enable/disable post processing to specified IME output path

    @param[in] UINT32 IME output path
    @param[in] BOOL enable/disable post processing
    @return None
*/
extern void IPL_SetPostImageProcessOnOff2(UINT32 Path, BOOL Enable);


/**
    Get post processing status(enable/disable) to specified IME output path

    @param[in] UINT32 IME output path
    @return BOOL
        - @b enable/disable
*/
extern BOOL IPL_GetPostImageProcessOnOff2(UINT32 Path);


/**
    callback function in IME_ISR to exchange address & size information

    @param[in] UINT32 IME output image index(count)
    @param[in/out] IPL_YCC_IMG_INFO exchange IME output path1 information. in: type, size. out: next buffer address
    @param[in/out] IPL_YCC_IMG_INFO exchange IME output path2 information. in: type, size. out: next buffer address
    @param[in/out] IPL_YCC_IMG_INFO exchange IME output path3 information. in: type, size. out: next buffer address
    @param[in] fpCallBack callback for post processing done
    @return None
*/
extern void IPL_CtrlPostImage2(UINT32 ImgIdx, IPL_YCC_IMG_INFO* path1,IPL_YCC_IMG_INFO* path2);


/**
    Set each device(path) size, such as LCD, HDMI, Video, etc.

    @param[in] UINT32 IME output path
    @param[in] IPL_YCC_CH_INFO device size info
    @return None
*/
extern void IPL_SetDeviceInputInfo2(UINT32 path, IPL_YCC_CH_INFO SizeInfo);


/**
    Pause display control
    @return None
*/
extern void IPL_Display_Pause2(void);


/**
    Resume display control
    @return None
*/
extern void IPL_Display_Resume2(void);


/**
    Check buffer ready or not

    @return BOOL
        - @b TRUE/FALSE
*/
extern BOOL IPL_Display_ChkBufferReady2(void);


/**
    Set skip frames to ignore invaild image befor starting display
    If user don,t set this parameter, the default value is 2

    @param[in] UINT32 skip frames
    @return None
*/
extern void IPL_Display_InitSkipFrame2(UINT32 SkipFrame);

/**
    Set IME output path to display

    @param[in] UINT32 IME output path (0~2)

    @return none
*/
extern void IPL_SetDisplayInputPath2(UINT32 path);

/**
    Get next IME output path information

    @param[in] UINT32 IME output path (0~2)

    @return IPL_YCC_IMG_INFO
        - @b image information(type, address, size)
*/
extern IPL_YCC_IMG_INFO IPL_GetNextIMEoutputAddr2(UINT32 path);

/**
    The display control task, should be installed by OS
    @return None
*/
extern void IPL_DisplayCtrl_Tsk2(void);

/**
    Get specified IME output buffer

    @param[in] UINT32 IME output path
    @param[in] UINT32 buffer index ID
    @return IPL_YCC_IMG_INFO
        - @b image information(type, address, size)
*/
extern IPL_YCC_IMG_INFO IPL_GetSpecifiedIMEoutputAddr2(UINT32 path, UINT32 BufferID);

/**
    Set the ring buffer number of DisplayTask2

    @param[in] UINT32 BufNum (2~10)

    @return none
*/
extern void IPL_Display_SetBufferNum2(UINT32 BufNum);

/**
    Set the H264 lag frame to avoid printing stamp on LCD (2nd pipeline flow use the same image buffer for H264 & display)

    @param[in] UINT32 Lagframe (0~n-2, n = max buffer number)

    @return none
*/
extern void IPL_Display_SetEncodeLag(UINT32 Lagframe);


#endif
