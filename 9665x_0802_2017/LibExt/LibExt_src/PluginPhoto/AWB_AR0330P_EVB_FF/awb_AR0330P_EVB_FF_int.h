/**
    Auto white balance.


    @file       awb_sample_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AWB_AR0330P_EVB_FF_INT_H_
#define _AWB_AR0330P_EVB_FF_INT_H_

/**
     @name AE default value
     @note
*/
//@{

#define AWB_RATE            4          ///< awb rate
#define AWB_DEF_KRGAIN      100
#define AWB_DEF_KBGAIN      100

#define AWB_DEF_RGAIN       420
#define AWB_DEF_GGAIN       256
#define AWB_DEF_BGAIN       450

#define AWB_OUTL_R          17
#define AWB_OUTL_B          17
#define AWB_W_THRES         3

#define AWB_WIN_X           32
#define AWB_WIN_Y           32
#define AWB_WIN_TOTAL       (AWB_WIN_X * AWB_WIN_Y)

#define AWB_WHITE_TAB_CNT 2
#define AWB_GREEN_TAB_CNT 0
#define AWB_PDL_TAB_CNT 0

#define AWB_MAX_WHITE_TAB 5
#define AWB_MAX_GREEN_TAB 5
#define AWB_MAX_PDL_TAB 5

//@}


/**
     Awb mode

     @note
*/
typedef enum _AWB_MODE
{
    AWB_MDOE_AUTO = 0,
    AWB_MODE_DAYLIGHT,
    AWB_MODE_CLOUDY,
    AWB_MODE_TUNGSTEN,
    AWB_MODE_FLUORESCENT1,
    AWB_MODE_FLUORESCENT2,
    AWB_MODE_SUNSET,
    AWB_MODE_BIRTHDAYPARTY,
    AWB_MODE_CUSTOMER1,
    AWB_MODE_CUSTOMER2,
    AWB_MODE_CUSTOMER3,
    AWB_MODE_MAX,
    ENUM_DUMMY4WORD(AWB_MODE)
}AWB_MODE;



//extern const UINT32 MwbTAB[AWB_MODE_MAX][2];
extern UINT32 MwbTAB[AWB_MODE_MAX][2];
extern AWBALG_TAB AwbWhiteTAB[AWB_MAX_WHITE_TAB];
extern AWBALG_PDLTAB AwbPDLTAB[AWB_MAX_PDL_TAB];
extern AWBALG_CT_PARAM AwbCTParam;
extern AWBALG_POSTPROC_PARAM AwbPostParam;


/**

     set color gain

     @note

     @param[in] R gain
     @param[in] G gain
     @param[in] B gain
*/
extern void AWB_setGain(UINT32 RGain, UINT32 GGain, UINT32 BGain);

/**
     get ca value

     @note

     @param[out] R acc value
     @param[out] G acc value
     @param[out] B acc value
     @param[in] window size

*/
extern void AWB_getCA(UINT32 *R, UINT32 *G, UINT32 *B, UINT32 Size);


/**
     awb mode map function
     @note

     @param[in] awb mode index
     @return
         - @b awb mode.
*/
extern AWB_MODE AWB_getMode(UINT32 Idx);


/**
     awb auto process
     @note
*/
extern void AWB_autoProc(AWBALG_INFO *Info);

/**
     awb manual process
     @note

     @param[in] awb mode index
*/
extern void AWB_manualProc(AWB_MODE Idx);


/**
     awb get preview color gain
     @note

     @param[out] R gain
     @param[out] G gain
     @param[out] B gain
*/
extern void AWB_GetPrvGain(UINT32 *Rgain, UINT32 *Ggain, UINT32 *Bgain);

//extern AWBALG_ER fpUserChkWhite(UINT32 R, UINT32 G, UINT32 B);
#endif //_AWB_SAMPLE_INT_H_
