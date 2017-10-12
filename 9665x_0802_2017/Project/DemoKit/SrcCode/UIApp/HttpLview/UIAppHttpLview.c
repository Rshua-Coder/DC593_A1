#include "PrjCfg.h"
#include "HwMem.h"
#include "Timer.h"
#include "Dma.h"
#if (HTTP_LIVEVIEW_FUNC == ENABLE)
#ifdef __ECOS
#include "LViewNvtAPI.h"
#include "DhcpNvt.h"
#include <cyg\lviewd\lviewd.h>
#include <sys/time.h>

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppHttp
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


static BOOL bStartHttp = FALSE;

static int UIAppHttp_getJpg(int* jpgAddr, int* jpgSize)
{
    MEM_RANGE  JpgBuf;
    int        ret;

    if(bStartHttp)
    {
        ret = LviewNvt_getCurDecJpgBuf(&JpgBuf);

        if (ret == E_OK)
        {
            //*jpgAddr = dma_getCacheAddr(pJpgBuf->Addr);
            //dma_flushReadCache(*jpgAddr,pJpgBuf->Size);
            *jpgAddr = JpgBuf.Addr;
            *jpgSize = JpgBuf.Size;
            ret = TRUE;
        }
        else
        {
            *jpgAddr = 0;
            *jpgSize = 0;
            ret = FALSE;
        }
        DBG_IND("jpgAddr=0x%x,jpgSize=0x%x\r\n ",JpgBuf.Addr,JpgBuf.Size);
    }
    else
    {
        *jpgAddr = 0;
        *jpgSize = 0;
        ret = FALSE;
    }
    return ret;
}

static cyg_uint32 UIAppHttp_getCurrTime(void)
{
    return timer_getCurrentCount(timer_getSysTimerID());
}


static void UIAppHttp_notifyStatus(int status)
{
    static  UINT32 preClientTime = 0;
    switch (status)
    {
        case CYG_LVIEW_STATUS_CLIENT_REQUEST:
             DBG_DUMP("client request %d ms\r\n",UIAppHttp_getCurrTime()/1000);
             preClientTime = UIAppHttp_getCurrTime();
             break;
        case CYG_LVIEW_STATUS_SERVER_RESPONSE_START:
             DBG_IND("send data start, time= %05d ms\r\n",UIAppHttp_getCurrTime()/1000);
             break;
        case CYG_LVIEW_STATUS_SERVER_RESPONSE_END:
             DBG_IND("send data end, time= %05d ms\r\n",UIAppHttp_getCurrTime()/1000);
             break;
        case CYG_LVIEW_STATUS_CLIENT_DISCONNECT:
             DBG_DUMP("client disconnect, time= %05d ms\r\n",UIAppHttp_getCurrTime()/1000);
             break;
    }
}


static void UIAppHttp_hwmem_memcpy(cyg_uint32 uiDst, cyg_uint32 uiSrc, cyg_uint32 uiSize)
{
    hwmem_open();
    hwmem_memcpy(uiDst, uiSrc, uiSize);
    hwmem_close();
}


void UIAppHttp_Open(void)
{
    extern UINT32 g_lviewPort, g_lviewTaskPri, g_hwmemcpy, g_maxJpgSize, g_lviewframeRate;

    cyg_lviewd_install_obj  lviewObj={0};

    DBG_IND("\r\n");
    lviewObj.getJpg = UIAppHttp_getJpg;
    lviewObj.notify = UIAppHttp_notifyStatus;
    if (g_hwmemcpy)
    {
        lviewObj.hwmem_memcpy = UIAppHttp_hwmem_memcpy;
    }
    lviewObj.portNum = g_lviewPort;
    lviewObj.threadPriority = g_lviewTaskPri;
    lviewObj.maxJpgSize = g_maxJpgSize;
    lviewObj.frameRate = g_lviewframeRate;
    lviewObj.sockbufSize = 40960;    
    lviewObj.arg = NULL;
    cyg_lviewd_install(&lviewObj);

    // init network interface
    //Dhcp_Client_Start("Nvt", NULL, TRUE);

    // start live view daemon
    cyg_lviewd_startup();

}

void UIAppHttp_Close(void)
{
    cyg_lviewd_stop();
}

void UIAppHttp_Start(void)
{
    bStartHttp = TRUE;
}
void UIAppHttp_Stop(void)
{
    bStartHttp = FALSE;
}
#endif
#endif
