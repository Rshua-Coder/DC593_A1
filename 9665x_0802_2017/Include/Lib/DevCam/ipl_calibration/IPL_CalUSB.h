#ifndef _IPL_CALUSB_H_
#define _IPL_CALUSB_H_
#include "Type.h"
#include "IPL_utility.h"

#define RGB_COLOR(r, g, b) ((UINT32)(UINT8)(b) | ((UINT32)(UINT8)(g) << 8) | ((UINT32)(UINT8)(r) << 16))

/**

     register usb command

     @note

     @return
         - @b BOOL TRUE:OK, FALSE:NG
*/
extern BOOL CalUSB_MsdcCB(void);

/**
     open calibration USB mode

     @note

     @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalUSB_Open(void);

/**
     close calibration USB mode

     @note

     @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalUSB_Close(void);

/**
     clear osd

     @note

     @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalUSB_ClrOSD(URECT *rect);

/**

     show string

     @note
     @param[in] CHAR string
     @param[in] URECT x, y, width, height
     @param[in] UINT32 RGBColor(RGB_COLOR(R, G, B))
     @param[in] BOOL clear flag
     @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalUSB_DrawString(CHAR *str, URECT *rect, UINT32 RGBColor, BOOL bClear);

/**

     wait key

     @note
     @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalUSB_WaitKey(void);

/**

     set calibration function

     @note
     @param[in] CHAR name
     @param[in] FP function pointer
     @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalUSB_SetFunc(CHAR *name, FP fp);

/**

     get pc display size

     @note
     @param[out] w
     @param[out] h
     @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalUSB_GetDispSize(UINT32 *w, UINT32 *h);

/**

     set display image information

     @note
     @param[in] IPL_YCC_IMG_INFO
     @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalUSB_SetYUVInfo(IPL_YCC_IMG_INFO *Info);


#endif //IPL_CAL_USB
