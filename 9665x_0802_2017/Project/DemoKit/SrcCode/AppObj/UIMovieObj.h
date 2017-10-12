#ifndef UI_MOVIEOBJ_H
#define UI_MOVIEOBJ_H

#include "UIFramework.h"
#include "AppCfg.h"
#include "PrjCfg.h"
//#NT#2010/09/16#Lily Kao -begin
//#Remove code within (_MOVIE_MODE_ == _MOVIE_MOVMJPG_) block
//#Remove useless comments
#define UIPLAYAVI_SPEED_X1_IDX          0
#define UIPLAYAVI_SPEED_X2_IDX          1
#define UIPLAYAVI_SPEED_X4_IDX          2
//#NT#2010/10/06#Lily Kao -begin
//Speed 8X
#define UIPLAYAVI_SPEED_X8_IDX          3
//#NT#2010/10/06#Lily Kao -end

#define MOVOBJ_REC_JPEG_FORMAT444      0
#define MOVOBJ_REC_JPEG_FORMAT422      1
#define MOVOBJ_REC_JPEG_FORMAT420      2
#define MOVOBJ_REC_AUD_VID_BOTH        0
#define MOVOBJ_REC_VID_ONLY            1
#define MOVOBJ_REC_QUALITY_BEST        0
#define MOVOBJ_REC_QUALITY_NORMAL      1
#define MOVOBJ_REC_QUALITY_DRAFT       2

//----- record -----
#define MOVREC_REC_STATUS_NOT_OPENED                1
#define MOVREC_REC_STATUS_NOT_RECORD                2
#define MOVREC_REC_STATUS_NOT_RECORDING             3

#define MOVREC_EVENT_RESULT_NORMAL                  1        //1    ///< Finish normally
#define MOVREC_EVENT_RESULT_HW_ERR                  2        //2    ///< Hardware error
#define MOVREC_EVENT_RESULT_FULL                    3          //3    ///< Card full
#define MOVREC_EVENT_RESULT_SLOW                    4          //4    ///< Write card too slowly
#define MOVREC_EVENT_RESULT_NOTREADY                5      //5    ///< Not ready
#define MOVREC_EVENT_ONE_SECOND                     6           //6    ///< One second arrives
#define MOVREC_EVENT_RESULT_OVERTIME                7      //7    ///< Overtime
#define MOVREC_EVENT_RESULT_OVERWRITE               8     //8    ///< Buffer overwrite
#define MOVREC_EVENT_RESULT_INVALIDFILE             9   //9    ///< SAVE INVALIDFILE
#define MOVREC_EVENT_RESULT_WRITE_ERR               10     //10

//----- Play -----
#define MOVPLAY_EVENT_FINISH                        1            //1   ///< Finish
#define MOVPLAY_EVENT_ONE_SECOND                    2        //2   ///< One second arrives
#define MOVPLAY_EVENT_ONE_VIDEOFRAME                3    //3   ///< Decode one video-frame
#define MOVPLAY_EVENT_START                         4           //6   //AVIPLAY_EVENT_START
#define MOVPLAY_EVENT_INIT_PROCESSING               5   //5
#define MOVPLAY_EVENT_INIT_OK                       6           //6
#define MOVPLAY_EVENT_ERROR                         7
#define MOVPLAY_EVENT_TEST                          8
//#NT#2011/09/15#Lincy Lin -begin
//#NT#Add event for audio opened
#define MOVPLAY_EVENT_AUD_OPENED                    9
//#NT#2011/09/15#Lincy Lin -end

//---- Debug ------------------------------------------------------
#define _UIMOVOBJ_DBG_NONE_           0
#define _UIMOVOBJ_DBG_CHK_            1
#define _UIMOVOBJ_DBG_IO_             2
#define _UIMOVOBJ_DBG_ALL_            3
#define _UIMOVOBJ_DBG_LVL_            _UIMOVOBJ_DBG_NONE_
#if (_UIMOVOBJ_DBG_LVL_ == _UIMOVOBJ_DBG_NONE_)
    #define DbgMsg_UIMovObjIO(msg)
    #define DbgMsg_UIMovObj(msg)
    #define DbgCode_UIMovObj(x)
#elif (_UIMOVOBJ_DBG_LVL_ == _UIMOVOBJ_DBG_ALL_)
    #include "Debug.h"
    #define DbgMsg_UIMovObjIO(msg)      debug_msg msg
    #define DbgMsg_UIMovObj(msg)        debug_msg msg
    #define DbgCode_UIMovObj(x)         x
#elif (_UIMOVOBJ_DBG_LVL_ == _UIMOVOBJ_DBG_CHK_)
    #include "Debug.h"
    #define DbgMsg_UIMovObjIO(msg)
    #define DbgMsg_UIMovObj(msg)        debug_msg msg
    #define DbgCode_UIMovObj(x)         x
#elif (_UIMOVOBJ_DBG_LVL_ == _UIMOVOBJ_DBG_IO_)
    #include "Debug.h"
    #define DbgMsg_UIMovObjIO(msg)      debug_msg msg
    #define DbgMsg_UIMovObj(msg)
    #define DbgCode_UIMovObj(x)
#endif
//------------------------------------------------------------

typedef enum
{
    RECMOVIE_MAXSECOND,
    RECMOVIE_REC_STATUS,
    RECMOVIE_SAVING_STATUS,
    //#NT#2011/02/10#Photon Lin -begin
    //#Add file access
    RECMOVIE_QUEUE_STATUS,
    //#NT#2011/02/10#Photon Lin -end
    RECMOVIE_GET_DATATYPE_MAX
}RECMOVIE_GET_DATATYPE;

typedef enum
{
    PLAYMOVIE_DIRID,
    PLAYMOVIE_FILEID,
    PLAYMOVIE_TOTAL_FRAME,
    PLAYMOVIE_TOTAL_SECOND,
    PLAYMOVIE_EV,
    PLAYMOVIE_WIDTH,
    PLAYMOVIE_CURR_FRAME,
    PLAYMOVIE_CURR_SECOND,
    PLAYMOVIE_LOCKSTATUS,
    PLAYMOVIE_FILENUM,
    PLAYMOVIE_OPENED,
    PLAYMOVIE_GET_DATATYPE_MAX
}PLAYMOVIE_GET_DATATYPE;


typedef enum
{
    PLAYMOVIE_SPEEDL1,
    PLAYMOVIE_SPEEDL2,
    PLAYMOVIE_SPEEDL4,
    //#NT#2010/10/06#Lily Kao -begin
    //Speed 8X
    PLAYMOVIE_SPEEDL8,
    //#NT#2010/10/06#Lily Kao -end
    PLAYMOVIE_AUD_VOLUME,
    PLAYMOVIE_SET_DATATYPE_MAX
}PLAYMOVIE_SET_DATATYPE;


typedef enum
{
    RECMOVIE_WIDTH,
    RECMOVIE_HEIGHT,
    RECMOVIE_FRAMERATE,
    RECMOVIE_TARGETBITRATE,
    RECMOVIE_QUALITY,
    //#NT#2010/09/29#Photon Lin -begin
    //#Add record second for custom usage
    RECMOVIE_RECORDTIME,
    //#NT#2010/09/29#Photon Lin -end
    //#NT#2011/04/15#Photon Lin -begin
    //#NT#Add AV sync mechanism
    RECMOVIE_AYSYNCCNT,
    //#NT#2011/04/15#Photon Lin -end
    RECMOVIE_AUD_VOLUME,
    RECMOVIE_SET_DATATYPE_MAX
}RECMOVIE_SET_DATATYPE;

typedef enum
{
    NVTEVT_MOVIE_MIN    = APP_EVENT_MASK + 0x00000300,
    NVTEVT_CLOSE_REC_MOVIE = NVTEVT_MOVIE_MIN,
    NVTEVT_START_REC_MOVIE,
    NVTEVT_STOP_REC_MOVIE,
    NVTEVT_START_RTSP,
    NVTEVT_STOP_RTSP,
    NVTEVT_INIT_PLAY_MOVIE,
    NVTEVT_CLOSE_PLAY_MOVIE,
    NVTEVT_CLOSEREOPEN_PLAY_MOVIE,
    NVTEVT_PAUSE_PLAY_MOVIE,
    //#NT#2012/08/30#Calvin Chang#Porting Media Framework on UIAPP-UIMoviePlayObj -begin
    NVTEVT_PLAY_PLAY_MOVIE,
    //#NT#2012/08/30#Calvin Chang -end
    NVTEVT_FWDPLAY_MOVIE,
    NVTEVT_BWDPLAY_MOVIE,
    NVTEVT_FWDSTEPPLAY_MOVIE,
    NVTEVT_BWDSTEPPLAY_MOVIE,
    NVTEVT_SWITCH_FIRST_FRAME_MOVIE,
    NVTEVT_SWITCH_LAST_FRAME_MOVIE,
    NVTEVT_CHANGE_MOVSIZE,
    NVTEVT_MOVIE_MAX
}NVT_MOVIE_EVENT;

typedef enum
{
    RECMOVIE_AUD_OFF  =   0,  //   0%
    RECMOVIE_AUD_VOL1 =  13,  //  13%
    RECMOVIE_AUD_VOL2 =  25,  //  25%
    RECMOVIE_AUD_VOL3 =  37,  //  37%
    RECMOVIE_AUD_VOL4 =  50,  //  50%
    RECMOVIE_AUD_VOL5 =  63,  //  63%
    RECMOVIE_AUD_VOL6 =  75,  //  75%
    RECMOVIE_AUD_VOL7 =  87,  //  87%
    RECMOVIE_AUD_ON   = 100,  // 100%
    RECMOVIE_AUD_MAX
}RECMOVIE_AUD_TYPE;



extern VControl UIMoviePlayObjCtrl;
extern VControl UIMovieRecObjCtrl;
extern UINT32   UIMovObj_GetMovieSizeWidth(UINT32 ubIndex);
extern UINT32   UIMovObj_GetMovieSizeHeight(UINT32 ubIndex);
extern UINT32   UIMovObj_GetMovieFrameRate(UINT32 ubIndex);
extern UINT32   UIMovObj_GetMovieQualityValue(UINT16 ubIndex);
extern UINT32   UIMovObj_GetMovieTargetBitrate(UINT16 ubIndex);
extern UINT32   UIMovObj_GetDataPlayMovie(PLAYMOVIE_GET_DATATYPE dataType);
extern void     UIMovObj_SetDataPlayMovie(PLAYMOVIE_SET_DATATYPE dataType, UINT32 data);
//#NT#2012/09/19#Calvin Chang#Porting Media Play Demo1 Flow -begin
extern INT32    UIMovObj_SetAudPlayVolume(UINT32 vol);
//#NT#2012/09/19#Calvin Chang -end
extern INT32    UIMovRecObj_Close(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern void     UIMovRecObj_SetData(RECMOVIE_SET_DATATYPE dataType, UINT32 data);
extern UINT32   UIMovRecObj_GetData(RECMOVIE_GET_DATATYPE dataType);
extern void     UIMovRecObj_WaitReady(void);

//#NT#2010/09/16#Lily Kao -end

#endif
