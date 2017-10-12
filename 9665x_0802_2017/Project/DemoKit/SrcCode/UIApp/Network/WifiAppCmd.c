#ifdef __ECOS
#include "PrjCfg.h"
#include <cyg/usocket/usocket.h>
#include <cyg/usocket_cli/usocket_cli.h>
#include "WifiAppCmd.h"
#include "NVTToolCommand.h"
#include "UIInfo.h"
#include "WifiAppXML.h"
#if (JSON_PARSER == ENABLE)
#include "UIAppCmdMapping.h"
#endif
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
#include "SysMain.h"
#endif
//#NT#2016/03/23#Isiah Chang -end
#define UDP_SOCKET          DISABLE

#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          WifiAppCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


extern WIFI_CMD_ENTRY Cmd_app[];

#if (USOCKET_CLIENT==ENABLE)
INT32 gUsockCliHandle = 0;

void UserCliSocket_Notify(int status,int parm)
{
    DBG_IND("status %d %d\r\n",status,parm);
    switch (status)
    {
        case CYG_USOCKETCLI_STATUS_CLIENT_REQUEST:
        {
        }
        break;
        case CYG_USOCKETCLI_STATUS_CLIENT_DISCONNECT:
        {
            DBG_ERR("UserCliSocket disconnect!!!\r\n");
        }
        break;
    }
}
int UserCliSocket_Recv(char* addr, int size)
{
    if(addr && (size>0))
    {
        addr[size]='\0';
        DBG_IND("%s %d\r\n",addr,size);
    }
    return TRUE;
}

void UserSocketClient_Open(void)
{
    usocket_cli_obj  usocketCliObj={0};
    usocket_cli_obj *pNewObj = 0;
    int result=0;

    DBG_IND("open usocket client \r\n");
    usocketCliObj.notify = UserCliSocket_Notify;
    usocketCliObj.recv = UserCliSocket_Recv;
    sprintf(usocketCliObj.svrIP,"%s","192.168.11.101");
    usocketCliObj.portNum = 8888;
    usocketCliObj.sockbufSize = 40960;
    usocketCliObj.timeout = 10;

    pNewObj = usocket_cli_get_newobj();
    if(pNewObj)
    {
        usocket_cli_install(pNewObj,&usocketCliObj);
        result = usocket_cli_connect(pNewObj);
        if(result==0)
        {
            usocket_cli_start();
            gUsockCliHandle = (INT32)pNewObj;
            DBGH(gUsockCliHandle);
        }
    }
}

void UserSocketClient_Close(void)
{
    DBGH(gUsockCliHandle);
    usocket_cli_stop();
}

int UserSocketClient_Send(char* addr, int* size)
{
    return usocket_cli_send((usocket_cli_obj*)gUsockCliHandle,addr,size);
}
#endif
void UserSocket_Notify(int status,int parm)
{
    DBG_DUMP(" %d %d\r\n",status,parm);
}

#if UDP_SOCKET
void UserUdpSocket_Notify(int status,int parm)
{
    switch (status)
    {
        case CYG_USOCKET_UDP_STATUS_CLIENT_REQUEST:
        {
        }
        break;
    }
}

int UserUdpSocket_Recv(char* addr, int size)
{
    if(addr && (size>0))
    {
        addr[size]='\0';
        DBG_IND("%s %d\r\n",addr,size);
    }
    return TRUE;
}
#endif

UINT32 WifiApp_SendCmd(UINT32 cmd,UINT32 status)
{
#if(JSON_PARSER == DISABLE)
    UINT32 result=0;
    static char bufArry[128];
    static UINT32 len =0;
    char *buf =bufArry;
    UINT32 sendlen=0;
    len = sprintf(buf,DEF_XML_HEAD);
    buf+=len;
    len = sprintf(buf,DEF_XML_RET,cmd,status);
    buf+=len;

    len=buf-bufArry;
    sendlen= len;
    DBG_DUMP(" %s %d\r\n",bufArry,sendlen);
    result = UserSocket_Send(bufArry,(int *)&len);
    if(sendlen!=len)
    {
        result = -1;
        DBG_ERR("sent %d error,should %d\r\n",len,sendlen);
    }

    return result;
#else
    return 0;
#endif


}

INT32 WifiApp_Recv(char* addr, INT32 size)
{
    *(addr+(size))='\0';
    DBG_DUMP("WifiApp_Recv %s %d\r\n",addr,size);
    return TRUE;
}

//#NT#2016/11/15#Isiah Chang -begin
//#NT#Ignore command except app_startup command is received..
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
static UINT32 g_uiSessionClose = FALSE;
void WifiApp_SetSessionCloseSts(UINT32 session_close)
{
    g_uiSessionClose = session_close;
}

UINT32 WifiApp_IsSessionClose(void)
{
    return g_uiSessionClose;
}
#endif

static INT32 WifiApp_CheckIfWifiMode(void)
{
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
    if(WifiApp_IsSessionClose()) // Ignore command except app_startup command is received.
    {
        DBG_DUMP("APP Session is closed\r\n");
        return FALSE;
    }

    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
        return TRUE;

    return FALSE;
#elif(WIFI_UI_FLOW_VER == WIFI_UI_VER_1_0)
    return TRUE;
#endif
}
//#NT#2016/11/15#Isiah Chang -end

void UserSocket_Open(void)
{

    usocket_install_obj  usocketObj={0};

    DBG_IND("open usocket\r\n");
    usocketObj.notify = UserSocket_Notify;
    usocketObj.recv = UserSocket_Recv;
#if(JSON_PARSER == ENABLE)
	usocketObj.portNum = 7878;
#else
    usocketObj.portNum = 3333;
#endif
    usocketObj.threadPriority = 6;
    usocketObj.sockbufSize = 40960;
    usocket_install(&usocketObj);
    usocket_open();

#if UDP_SOCKET
    usocket_install_obj  usocketUdpObj={0};
    DBG_IND("open udp usocket\r\n");
    usocketUdpObj.notify = UserUdpSocket_Notify;
    usocketUdpObj.recv = UserUdpSocket_Recv;
    usocketUdpObj.portNum = 2222;
    usocketUdpObj.threadPriority = 8;
    usocketUdpObj.sockbufSize = 0;
    usocket_udp_install(&usocketUdpObj);
    usocket_udp_open();
#endif

#if(JSON_PARSER == ENABLE)
	UIAppCmdMap_Init();
#endif

}

void UserSocket_Close(void)
{
    usocket_close();
}

int UserSocket_Send(char* addr, int* size)
{
    return usocket_send(addr,size);
}

int UserSocket_Recv(char* addr, int size)
{
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
    if(addr && (size>0))
    {
        addr[size]='\0';
        DBG_IND("%s %d\r\n",addr,size);
#if(JSON_PARSER == ENABLE)
		UIAppCmdMap_DoCmd(addr);
#else

#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
        Ux_PostEvent(NVTEVT_WIFI_EXE_HEARTBEAT, 0);
#endif

#endif
    }
//#NT#2016/03/23#Isiah Chang -end
    return TRUE;
}

//#NT#2017/01/13#Charlie Chang -begin
//#NT#support  usokcet multi
int UserSocket_Send_multi(char* addr, int* size, int obj_index)
{
    return usocket_send_multi(addr,size,obj_index);
}

int UserSocket_Recv_multi(char* addr, int size, int socket_index)
{
	////sample code for opening file and sending data by usocket with port 878
#if 1
    if(addr && (size>0))
    {
        addr[size]='\0';
        DBG_DUMP("get[%d]:%s %d\r\n",socket_index,addr,size);
		FST_FILE filehdl;
		char Buf[256];
		UINT32 size=16;
		INT32 ret=0;
		filehdl = FileSys_OpenFile("A:\\test_file",FST_OPEN_READ);
		if(filehdl == NULL){
			DBG_ERR("open test_file error!!\r\n");
			return -1;
		}

		while(1){
			memset(Buf,0,sizeof(Buf));
			ret = FileSys_ReadFile(filehdl, (UINT8 *)Buf, &size, 0, NULL);
			if(ret != FST_STA_OK){
				DBG_ERR("read test_file error!!\r\n");
				FileSys_CloseFile(filehdl);

				return 0;
			}
			Buf[size]='\0';
			////send to uscoket with port 8787
			DBG_DUMP("send size:%d buf:%s\r\n",size,Buf);
			usocket_send_multi(Buf,(int* )&size,socket_index);
			if(size < 16){
				DBG_DUMP("final read!!\r\n");
				break;
			}
		}
		FileSys_CloseFile(filehdl);

    }
#endif

    return TRUE;
}

void UserSocket_Close_multi(void)
{
    int obj_index = usocket_get_obj_index(8787);

	if(obj_index < 0){
		DBG_ERR("get usocket multi obj_index error obj_index=%d\r\n",obj_index);
		return;
	}
    usocket_close_multi(obj_index);
}
void UserSocket_Notify_multi(int status,int parm, int obj_index)
{
    DBG_DUMP(" %d %d %d\r\n",status,parm,obj_index);
}

void UserSocket_Open_multi(void)
{
    usocket_install_obj_multi  usocketObj={0};
	int obj_index=-1;

    DBG_IND("open usocket\r\n");
    usocketObj.notify = UserSocket_Notify_multi;
    usocketObj.recv = UserSocket_Recv_multi;

	usocketObj.portNum = 8787;

    usocketObj.threadPriority = 6;
    usocketObj.sockbufSize = 0;
    obj_index= usocket_install_multi(&usocketObj);
	if(obj_index == -1){
		DBG_ERR("usocket_install error obj_index=%d\r\n",obj_index);
		return;
	}

    usocket_open_multi(obj_index);

}

//#NT#2017/01/13#Charlie Chang -end
void WifiAppCmd_init(void)
{
#if(JSON_PARSER == DISABLE)
    WifiCmd_SetExecTable(Cmd_app);
    WifiCmd_SetDefautCB((UINT32)XML_DefaultFormat);
    WifiCmd_SetEventHandle((UINT32)Ux_PostEvent);
    //#NT#2016/03/23#Isiah Chang -begin
    //#NT#add new Wi-Fi UI flow.
    WifiCmd_SetAppStartupChecker((UINT32)WifiApp_CheckIfWifiMode);
    WifiCmd_SetAppStartupCmdCode(WIFIAPP_CMD_APP_STARTUP);
    //#NT#2016/03/23#Isiah Chang -end
#endif
}


WIFI_CMD_BEGIN(app)
WIFI_CMD_ITEM(WIFIAPP_CMD_CAPTURE,NVTEVT_WIFI_EXE_PHOTO_CAPTURE,(UINT32)XML_GetPictureEnd,WIFIFLAG_CAPTURE_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_CAPTURESIZE,NVTEVT_WIFI_EXE_PHOTO_SIZE,0,0, FL_PHOTO_SIZE)
WIFI_CMD_ITEM(WIFIAPP_CMD_FREE_PIC_NUM, 0,(UINT32)XML_GetFreePictureNum, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_RECORDING_TIME, 0, (UINT32)XML_GetMovieRecStatus, 0, FL_WIFI_MOVIE_FMT)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_REC_TRIGGER_RAWENC, NVTEVT_WIFI_EXE_MOVIE_TRIGGER_RAWENC, 0, WIFIFLAG_MOVIE_REC_RAWENC_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_GET_RAWENC_JPG, 0, (UINT32)XML_GetRawEncJpg, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_GET_LIVEVIEW_FMT, 0, (UINT32)XML_GetLiveViewFmt, 0, FL_NULL)

WIFI_CMD_ITEM(WIFIAPP_CMD_RECORD,NVTEVT_WIFI_EXE_MOVIE_REC,0,WIFIFLAG_RECORD_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_REC_SIZE, NVTEVT_WIFI_EXE_MOVIE_REC_SIZE,0,0, FL_MOVIE_SIZE)
WIFI_CMD_ITEM(WIFIAPP_CMD_CYCLIC_REC      ,NVTEVT_WIFI_EXE_CYCLIC_REC   ,0,0, FL_MOVIE_CYCLIC_REC)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_HDR     ,NVTEVT_WIFI_EXE_MOVIE_HDR      ,0,0, FL_MOVIE_HDR)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_EV    ,NVTEVT_WIFI_EXE_MOVIE_EV         ,0,0, FL_EV)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOTION_DET    ,NVTEVT_WIFI_EXE_MOTION_DET     ,0,0, FL_MOVIE_MOTION_DET)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_AUDIO   ,NVTEVT_WIFI_EXE_MOVIE_AUDIO    ,0,0, FL_MOVIE_AUDIO)
WIFI_CMD_ITEM(WIFIAPP_CMD_DATEIMPRINT     ,NVTEVT_WIFI_EXE_DATEIMPRINT  ,0,0, FL_MOVIE_DATEIMPRINT)
WIFI_CMD_ITEM(WIFIAPP_CMD_MAX_RECORD_TIME   ,0 ,(UINT32)XML_GetMaxRecordTime, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_LIVEVIEW_SIZE, NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_SIZE, 0, 0, FL_WIFI_MOVIE_APP_PREVIEW_SIZE)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_GSENSOR_SENS, NVTEVT_WIFI_EXE_MOVIE_GSENSOR_SENS, 0, 0, FL_GSENSOR)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_AUTO_RECORDING, NVTEVT_WIFI_EXE_SET_AUTO_RECORDING, 0, 0, FL_WIFI_AUTO_RECORDING)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_REC_BITRATE    ,NVTEVT_WIFI_EXE_MOVIE_REC_BITRATE, 0,0, FL_NULL)
#if(MOVIE_LIVEVIEW==RTSP_LIVEVIEW)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_LIVEVIEW_BITRATE, NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_BITRATE, 0,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_LIVEVIEW_START, NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_START,0,WIFIFLAG_PREVIEW_DONE, FL_NULL)
#endif
WIFI_CMD_ITEM(WIFIAPP_CMD_MODECHANGE,NVTEVT_WIFI_EXE_MODE,0,WIFIFLAG_MODE_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_QUERY         ,0,(UINT32)XML_QueryCmd,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_SSID    ,NVTEVT_WIFI_EXE_SET_SSID         ,0,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_PASSPHRASE,NVTEVT_WIFI_EXE_SET_PASSPHRASE ,0,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_DATE      ,NVTEVT_WIFI_EXE_SET_DATE       ,0,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_TIME      ,NVTEVT_WIFI_EXE_SET_TIME       ,0,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_POWEROFF      ,NVTEVT_WIFI_EXE_POWEROFF       ,0,0, FL_AUTO_POWER_OFF)
WIFI_CMD_ITEM(WIFIAPP_CMD_LANGUAGE      ,NVTEVT_WIFI_EXE_LANGUAGE       ,0,0, FL_LANGUAGE)
WIFI_CMD_ITEM(WIFIAPP_CMD_TVFORMAT      ,NVTEVT_WIFI_EXE_TVFORMAT       ,0,0, FL_TV_MODE)
WIFI_CMD_ITEM(WIFIAPP_CMD_FORMAT        ,NVTEVT_WIFI_EXE_FORMAT         ,0,WIFIFLAG_FORMAT_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SYSRESET      ,NVTEVT_WIFI_EXE_SYSRESET       ,0,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_VERSION       ,0,(UINT32)XML_GetVersion,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_FWUPDATE      ,NVTEVT_WIFI_EXE_FWUPDATE       ,0,WIFIFLAG_UPDATE_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_QUERY_CUR_STATUS, 0, (UINT32)XML_QueryCmd_CurSts, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_FILELIST       ,0,(UINT32)XML_FileList,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_HEARTBEAT, 0, (UINT32)XML_GetHeartBeat, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_DISK_FREE_SPACE, 0, (UINT32)XML_GetDiskFreeSpace, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_RECONNECT_WIFI,NVTEVT_WIFI_EXE_RECONNECT       ,0,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_BATTERY,0       ,(UINT32)XML_GetBattery,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SAVE_MENUINFO,NVTEVT_WIFI_EXE_SAVEMENU        ,0,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_HW_CAP,0       ,(UINT32)XML_HWCapability,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_REMOVE_USER,NVTEVT_WIFI_EXE_REMOVE_USER       ,0,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_CARD_STATUS, 0, (UINT32)XML_GetCardStatus,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_DOWNLOAD_URL, 0, (UINT32)XML_GetDownloadURL,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_UPDATEFW_PATH, 0, (UINT32)XML_GetUpdateFWPath,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_PIP_STYLE, NVTEVT_WIFI_EXE_PIP_STYLE, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_SSID_PASSPHRASE, 0, (UINT32)XML_GetSSID_passphrase, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_QUERY_MOVIE_SIZE, 0, (UINT32)XML_GetMovieSizeCapability, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_QUERY_MENUITEM, 0, (UINT32)XML_GetMenuItem, 0, FL_NULL)

WIFI_CMD_ITEM(WIFIAPP_CMD_SEND_SSID_PASSPHRASE, NVTEVT_WIFI_EXE_SEND_SSID_PASSPHRASE, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_WIFI_CONNECT_MODE, NVTEVT_WIFI_EXE_SET_WIFI_CONNECT_MODE, 0, 0, FL_NetWorkMode)

//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
WIFI_CMD_ITEM(WIFIAPP_CMD_APP_STARTUP, 0, (UINT32)XML_APP_STARTUP, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_APP_SESSION_CLOSE, NVTEVT_WIFI_EXE_APP_SESSION_CLOSE, 0, 0, FL_NULL)
#endif
//#NT#2016/03/23#Isiah Chang -end
//#NT#2016/06/01#Isiah Chang -begin
//#NT#add a Wi-Fi command to get current mode status.
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_MODE_STAUTS,0,(UINT32)XML_GetCntModeStatus,0, FL_NULL)
//#NT#2016/06/01#Isiah Chang -end
WIFI_CMD_ITEM(WIFIAPP_CMD_THUMB,0,(UINT32)XML_GetThumbnail,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SCREEN,0,(UINT32)XML_GetScreen,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_FILE_INFO,0,(UINT32)XML_GetMovieFileInfo,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_DELETE_ONE,0,(UINT32)XML_DeleteOnePicture,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_DELETE_ALL,0,(UINT32)XML_DeleteAllPicture,0, FL_NULL)

WIFI_CMD_ITEM(WIFIAPP_CMD_UPLOAD,0,(UINT32)XML_UploadFile,0, FL_NULL)

WIFI_CMD_END()
#endif
