#ifndef _WIFIAPPXML_H_
#define _WIFIAPPXML_H_

#include <cyg/hfs/hfs.h>

int XML_GetModeData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_PBGetData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_QueryCmd(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetThumbnail(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetScreen(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetMovieFileInfo(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetRawEncJpg(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetVersion(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetLiveViewFmt(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_FileList(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
void XML_DefaultFormat(UINT32 cmd,UINT32 ret,cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType);
int XML_QueryCmd_CurSts(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetHeartBeat(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetFreePictureNum(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetMovieRecStatus(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetMaxRecordTime(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetDiskFreeSpace(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_DeleteOnePicture(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_DeleteAllPicture(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetPictureEnd(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetBattery(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_HWCapability(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int WifiCmd_getQueryData(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetCardStatus(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetDownloadURL(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetUpdateFWPath(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetSSID_passphrase(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetMovieSizeCapability(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
int XML_GetMenuItem(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);

//upload command xml
int XML_UploadFile(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32 bufSize, cyg_uint32 segmentCount, HFS_PUT_STATUS putStatus);
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
int XML_GetCntModeStatus(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
//#NT#2016/03/23#Isiah Chang -end
//#NT#2016/06/01#Isiah Chang -begin
//#NT#add a Wi-Fi command to get current mode status.
int XML_APP_STARTUP(char* path, char* argument, cyg_uint32 bufAddr, cyg_uint32* bufSize, char* mimeType, cyg_uint32 segmentCount);
//#NT#2016/06/01#Isiah Chang -end
#endif
