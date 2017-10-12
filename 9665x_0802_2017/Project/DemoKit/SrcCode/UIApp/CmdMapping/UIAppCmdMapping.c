#ifdef __ECOS
#include <stdio.h>
#include <string.h>
#include "UIAppCmdMapping.h"
#include "UIAppNetwork.h"
#include "WifiAppCmd.h"
#include "ErrorNo.h"
#include <cyg/usocket/usocket.h>
#include "FileSysTsk.h"


#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIApp_CmdMapping
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


#include "JSONparser.h"

static UINT32            guiSerialNum    = 0xFFFFFFFF;
UIAPPCMDMAP_CMD_REQ_INFO gUiAppReq_Info  = {0};

#if 0
static const char *TEST_STRING ="{\"token\":0,"
								 "\"msg_id\":0x2,"
								 "\"msg_index\":1000,\n"
								 "\"type\":event_record_sensitivity,\n"
								 "\"param\":\"00:11:22:33:44:55\"}";
#endif

#define MAX_TOKEN 128 /* We expect no more than 128 tokens */

static int jsoneq(const char *json, jsontok_t *tok, const char *s){
	if (tok->type == JSON_STRING && (int) strlen(s) == tok->end - tok->start &&
		strncmp(json + tok->start, s, tok->end - tok->start) == 0){
		return 0;
	}
	return -1;
}

static UINT32 UIAPPCmd_CheckCmd_GetInfo(UIAPPCMDMAP_CMD_REQ_INFO *preq_info)
{
    if (preq_info->msg_id == 11)
        return TRUE;
    else
        return FALSE;
}

#define CMD_TYPE_SET_SETTING_TOKEN  (UINT32)1

static UINT32 UIAPPCmd_CheckCmd_StartSession(UIAPPCMDMAP_CMD_REQ_INFO *preq_info)
{
    if ((preq_info->token == START_SESSION_TOKEN) &&
        (preq_info->msg_id == START_SESSION_MSGID))
    {
        DBG_DUMP("return SETTING_TOKEN!!\r\n");
        sprintf(preq_info->param, "%d", CMD_TYPE_SET_SETTING_TOKEN);
        return TRUE;
    }
    else
        return FALSE;

}

static UINT32 UIAPPCmd_CheckCmd_SetSetting(UIAPPCMDMAP_CMD_REQ_INFO *preq_info)
{
    if (preq_info->token != CMD_TYPE_SET_SETTING_TOKEN)
    {
        DBG_ERR("token id is not match!!!\r\n");
        return TRUE;//FALSE;
    }

    if (preq_info->msg_index != guiSerialNum)
    {
        DBG_ERR("msg_id is not match!!!\r\n");
        return TRUE;//FALSE;
    }

    return TRUE;

}

INT32 UIAppCmdMap_parsecmd(char *JSON_content, UIAPPCMDMAP_CMD_REQ_INFO *output){


	int i, token_number;
	json_parser parser;
	jsontok_t t[MAX_TOKEN];
	json_init(&parser);
	int str_len=0;
	memset(output,0,sizeof(UIAPPCMDMAP_CMD_REQ_INFO));

	token_number = json_parse(&parser, JSON_content, strlen(JSON_content), t, MAX_TOKEN);


	/* Assume the top-level element is an object */
	if (token_number < 1 || t[0].type != JSON_OBJECT){
		DBG_ERR("Object expected token_number=%d t[0].type=%d\r\n",token_number,t[0].type);
		return -1;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < token_number; i++){
		if (jsoneq(JSON_content, &t[i], "token") == 0){

			str_len = t[i+1].end-t[i+1].start;
            char tmp_buf[128]={0};
			snprintf(tmp_buf,sizeof(tmp_buf),"%.*s",str_len, JSON_content + t[i+1].start);
            output->token = atoi(tmp_buf);
			//DBG_IND("- token: %s\r\n", output->token);

			i++;

		}
		else if(jsoneq(JSON_content, &t[i], "msg_id") == 0){

			str_len = t[i+1].end-t[i+1].start;
			char tmp_buf[128]={0};
			snprintf(tmp_buf,sizeof(tmp_buf),"%.*s",str_len, JSON_content + t[i+1].start);
			//DBG_IND("- msg_id: %s\r\n", tmp_buf);
			output->msg_id = atoi(tmp_buf);
			i++;
		}
		else if (jsoneq(JSON_content, &t[i], "msg_index") == 0){

			str_len = t[i+1].end-t[i+1].start;
			char tmp_buf[128]={0};
			snprintf(tmp_buf,sizeof(tmp_buf),"%.*s",str_len, JSON_content + t[i+1].start);
			DBG_IND("- msg_index: %s\r\n", tmp_buf);
			output->msg_index= atoi(tmp_buf);
			i++;
		}
		else if (jsoneq(JSON_content, &t[i], "type") == 0){

			str_len = t[i+1].end-t[i+1].start;


			snprintf(output->type,sizeof(output->type),"%.*s",str_len, JSON_content + t[i+1].start);
			DBG_IND("- type: %s\r\n", output->type);

			i++;
		}
		else if (jsoneq(JSON_content, &t[i], "param") == 0){

			str_len = t[i+1].end-t[i+1].start;

			snprintf(output->param,sizeof(output->param),"%.*s",str_len, JSON_content + t[i+1].start);
			DBG_IND("- param: %s\r\n", output->param);
			i++;
		}
        else if ((jsoneq(JSON_content, &t[i], "key") == 0) ||
                 (jsoneq(JSON_content, &t[i], "len") == 0) ){
			output->key = 1;
			i++;
		}

		else{
			DBG_ERR("Unexpected key: %.*s\r\n", t[i].end-t[i].start,
				JSON_content + t[i].start);
		}
	}

    return 0;

}

void UIAppCmdMap_AuthCmd(UIAPPCMDMAP_CMD_REQ_INFO *req_info, UINT32 uReqType)
{
    char head[64]={0};
	char cmd[512]={0};
	int size=0, headsize=0;

    if (uReqType == REQTYPE_START_SESSION)
    {
        sprintf(cmd,UIAPPCMDMAP_START_SESSION_RESPONSE,
		        0, req_info->msg_id, req_info->msg_index, atoi(req_info->param) );
    }
    else if (uReqType == REQTYPE_SET_SETTING)
    {
        sprintf(cmd,UIAPPCMDMAP_AUTH_RESPONSE,
		        0, req_info->msg_id, req_info->msg_index, req_info->param );
    }
    else if (uReqType == REQTYPE_ERROR)
    {
        sprintf(cmd,UIAPPCMDMAP_AUTH_RESPONSE,
		        -1, req_info->msg_id, req_info->msg_index, req_info->param );
    }
    else if (uReqType == REQTYPE_INFO)
    {
        char *pMacAddr = UINet_GetMAC();
        sprintf(cmd, UIAPPCMDMAP_START_SESSION_INFO, req_info->msg_id, req_info->msg_index,
                                                     pMacAddr[0], pMacAddr[1], pMacAddr[2],
                                                     pMacAddr[3], pMacAddr[4], pMacAddr[5]);
    }
    else
        return;

    size = strlen(cmd);

    // send header
    sprintf(head, UIAPPCMDMAP_REQ_HEAD, size);
    headsize = strlen(head);
    headsize++; // must 32 Character size!
    DBG_DUMP("Head:%s\r\n",head);
    UserSocket_Send(head, &headsize);

    // Send REQ
	DBG_DUMP("REQ:%s\r\n",cmd);
	UserSocket_Send(cmd, &size);

}

void UIAppCmdMap_DoCmd(char *cmd){

	int ret=0;

    memset((void *)&gUiAppReq_Info, 0, sizeof(gUiAppReq_Info));

	ret = UIAppCmdMap_parsecmd(cmd, &gUiAppReq_Info);
	if(ret < 0){
		DBG_ERR("parser cmd error  msg:\r\n%s\r\n",cmd);
		return;
	}

    DBG_IND("=======\r\n");
	DBG_IND("token=%d\r\n",gUiAppReq_Info.token);
	DBG_IND("msg_id=%u\r\n",gUiAppReq_Info.msg_id);
	DBG_IND("msg_index=%u\r\n",gUiAppReq_Info.msg_index);
	DBG_IND("type=%s\r\n",gUiAppReq_Info.type);
	DBG_IND("param=%s\r\n",gUiAppReq_Info.param);

    if (gUiAppReq_Info.key == 1)
    {
        //UIAppCmdMap_AuthCmd(&gUiAppReq_Info, REQTYPE_KEY);
        return;
    }

    // check whether is FW_INTERFACE_CMD_TYPE_START_SESSION
    if (UIAPPCmd_CheckCmd_GetInfo(&gUiAppReq_Info))
    {
        UIAppCmdMap_AuthCmd(&gUiAppReq_Info, REQTYPE_INFO);
    }
    else if (UIAPPCmd_CheckCmd_StartSession(&gUiAppReq_Info))
    {
        DBG_DUMP("param=%s\r\n",gUiAppReq_Info.param);
        guiSerialNum = gUiAppReq_Info.msg_index;
        DBG_DUMP("guiSerialNum=%d\r\n",guiSerialNum);
        UIAppCmdMap_AuthCmd(&gUiAppReq_Info, REQTYPE_START_SESSION);
    }
    // check whether is FW_INTERFACE_CMD_TYPE_SET_SETTING
    else if (UIAPPCmd_CheckCmd_SetSetting(&gUiAppReq_Info))
    {
        // DO COMMAND HERE!
        UIAppCmdMap_DispatchCmd(&gUiAppReq_Info);
    }
    else
    {
        DBG_ERR("command format is error!\r\n");
        UIAppCmdMap_AuthCmd(&gUiAppReq_Info, REQTYPE_ERROR);
    }

    return;

}
#endif
