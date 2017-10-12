#ifndef UI_EXECUTE_FUNC_H
#define UI_EXECUTE_FUNC_H

typedef enum
{
    NVTEVT_BKW_START = 1,
    NVTEVT_BKW_DELALL = NVTEVT_BKW_START,
    NVTEVT_BKW_COPY2CARD,
    NVTEVT_BKW_COPYCARD1ToCARD2,
    NVTEVT_BKW_COPYCARD2ToCARD1,
    NVTEVT_BKW_FORMAT,
    NVTEVT_BKW_FORMAT_CARD,
    NVTEVT_BKW_FORMAT_NAND,
    NVTEVT_BKW_NUM_RESET,
    NVTEVT_BKW_SETDPOF,
    NVTEVT_BKW_SETPROTECT,
    NVTEVT_BKW_WAIT_PTR_INIT,
    NVTEVT_BKW_RESIZE,
    NVTEVT_BKW_FIXREDEYE,
    NVTEVT_BKW_RESAVEFILE,
    NVTEVT_BKW_NOVALIGHT_FIX,
    NVTEVT_BKW_ROTATE,
    NVTEVT_BKW_DUMMY_WAIT,//do nothing, just wait for xxx ms then exit
    NVTEVT_BKW_DUMMY_WAIT_USB,
    NVTEVT_BKW_FW_UPDATE,
//#NT#2010/10/01#Ben Wang -begin
//#NT#Add deleting by date
    NVTEVT_BKW_DELFOLDER,
//#NT#2010/10/01#Ben Wang -end
//#NT#2011/02/01#Ben Wang -begin
    NVTEVT_BKW_USER_FUNCTION,
//#NT#2011/02/01#Ben Wang -end
    //#NT#2011/02/10#Photon Lin -begin
    //#Add file access
    NVTEVT_BKW_FILE_ACCESS,
    //#NT#2011/02/10#Photon Lin -end
    NVTEVT_BKW_DZOOM_IN,
    NVTEVT_BKW_DZOOM_OUT,
    //#NT#2012/10/23#Hideo Lin -begin
    //#NT#For picture in movie processing
    NVTEVT_BKW_PIM_PROCESS,
    //#NT#2012/10/23#Hideo Lin -end
    //#NT#2012/10/30#Calvin Chang#Generate Audio NR pattern by noises of zoom, focus and iris -begin
    NVTEVT_BKW_ANR_LENS_ACTION,
    //#NT#2012/10/30#Calvin Chang -end
    NVTEVT_BKW_WIFI_ON,
    NVTEVT_BKW_WIFI_CLEARACL,
    NVTEVT_BKW_MAKE_MOVIE, // make a movie by some JPEG images
    NVTEVT_BKW_END

}NVT_BKW_CMD;

#define PLAYDPOF_SETONE                 0x00000002
#define PLAYDPOF_SETALL                 0x00000004
//#NT#2010/05/31#[0010985]Lily Kao -begin
//#NT#DPOF-All from the first picture
#define PLAYDPOF_SETALL_FROM_FIRST      0x00004000
//#NT#2010/05/31#[0010985]Lily Kao -end

// Protect file
#define PLAY_PROTECT_ONE                0x01
#define PLAY_PROTECT_ALL                0x02
#define PLAY_UNPROTECT_ONE              0x04
#define PLAY_UNPROTECT_ALL              0x08


#define UI_BACKGND_DUMMY_WAIT_INFINITE  0xFFFFFFFF
//#NT#2010/04/20#[0010806]Lily Kao -begin
//MSDC connects to PC but shows connection-fail
#define UI_BACKGND_DUMMY_WAIT_MSDC      20000
//#NT#2010/04/20#[0010806]Lily Kao -end

typedef enum
{
    BKW_NEW_WIDTH,    //write only, for NVTEVT_BKW_RESIZE
    BKW_NEW_HEIGHT,   //write only, for NVTEVT_BKW_RESIZE
    BKW_OVERWRITE,    //write only, for NVTEVT_BKW_RESIZE and NVTEVT_BKW_RESAVEFILE
    BKW_DPOF_MODE,    //write only, for NVTEVT_BKW_SETDPOF
    BKW_DPOF_NUM,     //write only, for NVTEVT_BKW_SETDPOF
    BKW_DPOF_DATE,    //write only, for NVTEVT_BKW_SETDPOF
    BKW_PROTECT_TYPE, //write only, for NVTEVT_BKW_SETPROTECT
    BKW_WAIT_TIME,    //write only, for NVTEVT_BKW_DUMMY_WAIT, Unit: ms
    BKW_CHK_CONNECT_PTR,
    BKW_USER_FUNC,
    BKW_DZOOM_STOP,   //write only, for stop Dzoom moving, the value is the pointer of UIN32 stopFlag.
    BKW_DATA_MAX
}BKW_DATA_SET;

extern void AppBKW_SetData(BKW_DATA_SET attribute, UINT32 value);
extern BKG_JOB_ENTRY gBackgroundExtFuncTable[];
extern UINT32 BackgroundDummyWait_Stop(void);
//#NT#2010/11/09#Lily Kao -begin
extern void BackgroundWait_PrintModeIsClosed(UINT32 isClosed);
//#NT#2010/11/09#Lily Kao -end


#endif

