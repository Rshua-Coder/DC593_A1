#include <stdio.h>
#include <string.h>
#include "UIFramework.h"
#include "UIAppCmdMapping.h"
#include "ErrorNo.h"


#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIApp_CmdMapping
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

static UIAPP_CMD_ENTRY *g_pUiAppCmdTab = NULL;

void UIAppCmdMap_SetExecCmdTable(UIAPP_CMD_ENTRY *pAppCmdTbl)
{
    g_pUiAppCmdTab = pAppCmdTbl;
}

INT32 UIAppCmdMap_DispatchCmd(UIAPPCMDMAP_CMD_REQ_INFO *preq_info)
{
    int i = 0;

    if (!g_pUiAppCmdTab)
    {
        DBG_ERR("command table is not set!\r\n");
        UIAppCmdMap_AuthCmd(preq_info, REQTYPE_ERROR);
        return E_SYS;
    }

    while(g_pUiAppCmdTab[i].cmdidx != END_OF_CMDAMP)
    {
        if ((preq_info->msg_id== g_pUiAppCmdTab[i].cmdidx) &&
            (!strcmp(preq_info->type, g_pUiAppCmdTab[i].cmd)))
        {
            if (preq_info->msg_id == UIAPPCMD_GETTYPE)
            {
                if (g_pUiAppCmdTab[i].GetCB)
                {
                    UiAppCmd_GetCB *getcb = (UiAppCmd_GetCB *)g_pUiAppCmdTab[i].GetCB;
                    if(getcb(preq_info->param))
                    {
                        UIAppCmdMap_AuthCmd(preq_info, REQTYPE_SET_SETTING);
                        return E_OK;
                    }
                    else
                    {
                        DBG_ERR("callback function is failed!\r\n");
                        UIAppCmdMap_AuthCmd(preq_info, REQTYPE_ERROR);
                        return E_SYS;
                    }
                }
                else
                {
                    DBG_ERR("get callback function is not set!\r\n");
                    UIAppCmdMap_AuthCmd(preq_info, REQTYPE_ERROR);
                    return E_SYS;
                }
            }
            else if (preq_info->msg_id == UIAPPCMD_SETTYPE)
            {
                if (g_pUiAppCmdTab[i].event)
                {
                    UiAppCmd_ParamTranCB *trancb = (UiAppCmd_ParamTranCB *)g_pUiAppCmdTab[i].ParamTranCB;

                    if (trancb)
                        Ux_PostEvent(g_pUiAppCmdTab[i].event, 1, (UINT32)trancb(preq_info->param));
                    else
                    {
                        DBG_ERR("paramter to UI_data CB is not set!\r\n");
                        UIAppCmdMap_AuthCmd(preq_info, REQTYPE_ERROR);
                        return E_SYS;
                    }
                }
                else
                {
                    DBG_ERR("event handler is not set!\r\n");
                    UIAppCmdMap_AuthCmd(preq_info, REQTYPE_ERROR);
                    return E_SYS;
                }
            }

            return E_OK;
        }

        i++;
    }

    DBG_ERR("command is not found!\r\n");
    UIAppCmdMap_AuthCmd(preq_info, REQTYPE_ERROR);
    return E_SYS;;
}

