/**
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    @file       UIAppCmdMapping.h
    @brief      Header file of UIAppCmdMapping.c

    @note       Nothing.

*/

#ifndef _UIAPPCMDMAPPING_H
#define _UIAPPCMDMAPPING_H
#include "Type.h"

#define UIAPPCMDMAP_AUTH_RESPONSE          "{\"rval\":%d,\"msg_id\":%u,\"msg_index\":%u,\"param\":%s}"
#define UIAPPCMDMAP_START_SESSION_RESPONSE "{\"rval\":%d,\"msg_id\":%u,\"msg_index\":%u,\"param\":%d}"
#define UIAPPCMDMAP_START_SESSION_INFO     "{\"rval\":0,\"msg_id\":%d,\"msg_index\":%d,\"brand\":\"novatek\",\"chip\":\"NT96658\",\"app_type\":\"car\",\"product_ver\":\"J521\",\"fw_ver\":\"J521-1.1.9-S\",\"SN\":\"360LA63610200535\",\"MAC\":\"%02x:%02x:%02x:%02x:%02x:%02x\",\"bt_status\":\"off\",\"menu_vid\":\"TBD\",\"menu_pic\":\"TBD\",\"base64_menu_vid\":\"5omL5Yq/5ouN5pGE\",\"base64_menu_pic\":\"5omL5Yq/5ouN5pGE\",\"crypt\":\"base64\",\"encoding\":\"utf-8\"}"
//#define UIAPPCMDMAP_START_SESSION_INFO     "{\"rval\":0,\"msg_id\":%d,\"msg_index\":%d,\"brand\":\"ambarella\",\"chip\":\"A12\",\"app_type\":\"car\",\"product_ver\":\"J521\",\"fw_ver\":\"J521-1.1.9-S\",\"SN\":\"360LA63610200535\",\"MAC\":\"%02x:%02x:%02x:%02x:%02x:%02x\",\"bt_status\":\"on\",\"menu_vid\":\"test\",\"menu_pic\":\"test\",\"base64_menu_vid\":\"5omL5Yq/5ouN5pGE\",\"base64_menu_pic\":\"5omL5Yq/5ouN5pGE\",\"crypt\":\"base64\",\"encoding\":\"utf-8\"}"
#define UIAPPCMDMAP_REQ_HEAD               "{\"key\":\"j511\",\"len\":\"%08d\"}"
#define START_SESSION_TOKEN                0
#define START_SESSION_MSGID                257

#define UIAPPCMD_GETTYPE                   0x01
#define UIAPPCMD_SETTYPE                   0x02
#define END_OF_CMDAMP                      0xFFFFFFFF

typedef enum
{
    REQTYPE_START_SESSION = 0,
    REQTYPE_SET_SETTING,
    REQTYPE_ERROR,
    REQTYPE_INFO,
    ENUM_DUMMY4WORD(UIAPPCMD_REQTYPE)
}UIAPPCMD_REQTYPE;

typedef struct{

	UINT32 	token;
	UINT32  msg_id;
	UINT32  msg_index;
	char 	type[128];
	char 	param[128];
    UINT32  key;

}UIAPPCMDMAP_CMD_REQ_INFO;

typedef struct _UIAPP_CMD_ENTRY
{
    UINT32                cmdidx;
    char                  cmd[128];
    UINT32                event;
    UINT32                GetCB;
    UINT32                ParamTranCB;
}UIAPP_CMD_ENTRY;


typedef UINT32 UiAppCmd_GetCB(char *pparam);
typedef UINT32 UiAppCmd_ParamTranCB(char *pparam);

#define UIAPP_CMD_BEGIN(tbl) UIAPP_CMD_ENTRY (Cmd_##tbl)[]={ ///< begin a command table
#define UIAPP_CMD_ITEM(cmdidx,cmd,event,func,tran)  {(cmdidx),(cmd),(event),(func),(tran)}, ///< insert a command item in command table
#define UIAPP_CMD_END()    {END_OF_CMDAMP,0,0,0,0}}; ///< end a command table


extern void  UIAppCmdMap_DoCmd(char *cmd);
extern void  UIAppCmdMap_AuthCmd(UIAPPCMDMAP_CMD_REQ_INFO *req_info, UINT32 uReqType);
extern INT32 UIAppCmdMap_DispatchCmd(UIAPPCMDMAP_CMD_REQ_INFO *preq_info);
extern void  UIAppCmdMap_SetExecCmdTable(UIAPP_CMD_ENTRY *pAppCmdTbl);
extern void  UIAppCmdMap_Init(void);


#endif //_UIAPPCMDMAPPING_H
