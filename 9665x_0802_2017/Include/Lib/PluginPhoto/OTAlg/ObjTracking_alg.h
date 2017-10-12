/**
    Motion Detection Algorithm.

    

    @file       ObjTracking_Alg.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _OT_ALG_INT_H_
#define _OT_ALG_INT_H_

//
extern void setObjectWindowSize(UINT32 Obj_SizeX, UINT32 Obj_SizeY, UINT32 SubWindow_SizeX, UINT32 SubWindow_SizeY, UINT32 Obj_CentX, UINT32 Obj_CentY);
extern void setAftrackingThershold(UINT32 StableDistanceX, UINT32 StableDistanceY,UINT32 Th_OutOfScreen,UINT32 Th_BackToScreen); //Tele «G¶ÀsetY_C_Weighting(2,4)

//Path = 0, use video image; Path = 1, use display image; path = 2, use wifi image
extern BOOL Set_OT_SourcePath(UINT32 Path);
extern void setY_C_Weighting(UINT32 W_Y,UINT32 W_C);
extern BOOL GetTrackingWindow_Center(UINT32 *X, UINT32 *Y);
extern BOOL Start_Subject_Tracking(void);
extern void Stop_Subject_Tracking(void);
extern void Subject_Tracking(void);
extern void OT_SetBreak(BOOL bBreak);

#endif //_AE_ALG_INT_H_
