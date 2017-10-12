#include <stdio.h>
#include <string.h>
#include "SysMain.h"
#include "UIAppCmdMapTlb.h"
#include "UIFramework.h"
#include "UIAppCmdMapping.h"
#include "UIInfo.h"
#include "NVTUserCommand.h"
#include "AppControl.h"
#include "SysCommon.h"


#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIApp_CmdMapping
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

extern UIAPP_CMD_ENTRY Cmd_AppCmdMap[];

void UIAppCmdMap_Init(void)
{
    UIAppCmdMap_SetExecCmdTable(Cmd_AppCmdMap);

    // For test
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 2, PRIMARY_MODE_MOVIE,SYS_SUBMODE_WIFI);
}

static UINT32 UIAppCmd_GetAudio(char *pparam)
{
    switch (UI_GetData(FL_MOVIE_AUDIO))
    {
        default:
        case MOVIE_AUDIO_OFF:
            sprintf(pparam, "\"%s\"", PARAM_OFF);
            break;
        case MOVIE_AUDIO_ON:
            sprintf(pparam, "\"%s\"", PARAM_ON);
            break;
    }

    return TRUE;
}

static UINT32 UIAppCmd_SetAudioParam(char *pparam)
{
    if (!strcmp(pparam, PARAM_OFF))
        return MOVIE_AUDIO_OFF;
    else if (!strcmp(pparam, PARAM_ON))
        return MOVIE_AUDIO_ON;
    else
        DBG_ERR("paramter %s is not defined!!\r\n", pparam);

    return 0;
}

UIAPP_CMD_BEGIN(AppCmdMap)
UIAPP_CMD_ITEM(UIAPPCMD_SETTYPE, COMMAND_REC_AUDIO, NVTEVT_WIFI_EXE_MOVIE_AUDIO, 0, (UINT32) UIAppCmd_SetAudioParam)
UIAPP_CMD_ITEM(UIAPPCMD_GETTYPE, COMMAND_REC_AUDIO, 0, (UINT32)UIAppCmd_GetAudio, 0)
UIAPP_CMD_END()


