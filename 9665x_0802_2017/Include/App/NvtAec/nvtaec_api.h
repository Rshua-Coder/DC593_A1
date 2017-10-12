#ifndef _NVTAECAPI_H
#define _NVTAECAPI_H

#include "Type.h"
#include "Audio.h"

extern void NvtAec_InstallID(void) _SECTION(".kercfg_text");


/*
    Get play start time.

    Get play start time.

    @param[in] p1    no use.
    @param[in] p2    no use.
    @param[in] p3    no use.

    @return
     - @b E_OK:     Get successfully.
*/
extern ER NvtAec_GetPlayStart(UINT32 p1, UINT32 p2, UINT32 p3);

/*
    Get record start time.

    Get record start time.

    @param[in] p1    no use.
    @param[in] p2    no use.
    @param[in] p3    no use.

    @return
     - @b E_OK:     Get successfully.
*/
extern ER NvtAec_GetRecStart(UINT32 p1, UINT32 p2, UINT32 p3);

/*
    Calculate recorded sample count.

    Calculate recorded sample count.

    @param[in] p1    record buffer address.
    @param[in] p2    record buffer size.
    @param[in] p3    no use.

    @return
     - @b E_OK:     success.
*/
extern ER NvtAec_AddRecCount(UINT32 p1, UINT32 p2, UINT32 p3);

/*
    Add buffer to be played to queue.

    Add buffer to be played to queue.

    @param[in] p1    play buffer address.
    @param[in] p2    play buffer size.

    @return
     - @b E_OK:     success.
     - @b E_SYS:    buffer is too large.
*/
extern ER NvtAec_AddBufToQue(UINT32 p1, UINT32 p2);

/*
    Add recored sample count to queue.

    Add recored sample count to queue.

    @param[in] p1    no use.
    @param[in] p2    no use.

    @return
     - @b E_OK:     success.
*/
extern ER NvtAec_AddSampleCountQue(UINT32 p1, UINT32 p2);

/*
    Open AEC library.

    Open AEC library.

    @param[in] uiCh    Audio channel number.
    @param[in] uiSR    Audio sample rate.

    @return
     - @b E_OK:     Initial successfully.
     - @b E_OACV:   Open AEC failed.
     - @b E_SYS:    Initial AEC failed.
*/
extern ER NvtAec_Open(UINT32 uiCh, UINT32 uiSR);

/*
    Close AEC library.

    Close AEC library.

    @return
     - @b E_OK:     Close successfully.
     - @b E_SYS:    AEC is not opened.
*/
extern ER NvtAec_Close(void);

/*
    Run AEC.

    Run AEC.

    @param[in] Addr    recorded audio buffer address.
    @param[in] Size    recorded audio buffer size.

    @return
     - @b E_OK:     AEC done successfully.
     - @b E_SYS:    AEC done failed.
*/

extern ER NvtAec_Run(UINT32 Addr, UINT32 Size);

/*
    Stop AEC.

    Stop AEC when play task is stopped.

    @return
     - @b E_OK:     Stop successfully.
*/
extern ER NvtAec_Stop(void);

/*
    Reset record sample count queue.

    Reset record sample count queue. Should be called before new streaming starts.

    @return
     - @b E_OK:     Reset successfully.
*/
extern ER NvtAec_ResetSCQue(void);

#endif
