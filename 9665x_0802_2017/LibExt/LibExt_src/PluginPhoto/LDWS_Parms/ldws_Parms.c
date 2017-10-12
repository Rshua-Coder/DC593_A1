#include    "Type.h"
#include    "GxImage.h"
#include    "ldws_Parms_int.h"
#include    "ldws_Parms.h"
#include    "ldws_alg.h"
#include    "IPL_Utility.h"
#include    "IPL_Cmd.h"
#include    "HwMem.h"
#include    "FileSysTsk.h"



#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          LDWS_PARMS
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define THIRDPARTY_A_LDWS_BUF_SIZE (720*480*2)// working buffer size
#define LDWS_ROI_SIZE_X  720
#define LDWS_ROI_SIZE_Y  480
#define UPDATE_VP        TRUE

// System
static UINT32            g_uiLdwsBufAddr = 0;
UINT32                   g_uiLdwsRate = 1;
UINT32                   g_uiLdwsCount = 0;
BOOL                     g_LdwsLock = FALSE;
LDWS_CBMSG_FP            g_LdwsCbFP = NULL;
IPL_IME_BUF_ADDR IPL_IME_CurInfo1;
// LDWS init parameters
LDWS_INIT_PARMS      g_LdwsSysParamter;
// UI parameters
static UINT32 g_uiVanishPtx = 0, g_uiVanishPty = 0; // correspong to FHD
// Video parameters
UINT32 LdwsProcessTime = 0, LdwsProcessMaxTime = 0;
//LDWS result
LDWS_RESULT_INFO g_LdwsInfor;
// other
UINT32                   g_uiLdwsSrcFrameAddr;
UINT32                   g_uiLdwsFrameWidth = 0, g_uiLdwsFrameHeight = 0, g_uiLdwsFrameLineOfs=0;




UINT32 LdwsParms_CalcBuffSize(void)
{
    UINT32 BuffSize;

    DBG_IND("\r\n LDWS:: LDWS_CalcBuffSize \r\n" );
    BuffSize = THIRDPARTY_A_LDWS_BUF_SIZE;

    return BuffSize;
}

INT32 LdwsParms_Init(MEM_RANGE *buf, UINT32 ProcessRate)
{
    //IMG_BUF   in_img;

    if (!buf)
    {
        DBG_ERR("buf is NULL\r\n");
        return LDWSPARMS_STA_ERROR;
    }
    if (!buf->Addr)
    {
        DBG_ERR("bufAddr is NULL\r\n");
        return LDWSPARMS_STA_ERROR;
    }
    if (buf->Size < LdwsParms_CalcBuffSize())
    {
        DBG_ERR("bufSize %d < need %d\r\n",buf->Size,LdwsParms_CalcBuffSize());
        return LDWSPARMS_STA_ERROR;
    }
    //LDWS initial
    g_uiLdwsBufAddr = buf->Addr;
    g_uiLdwsRate = ProcessRate;


    // Set LDWS parameters

    // user enter
    g_LdwsSysParamter.uiFocalLen = 6; // 6mm
    g_LdwsSysParamter.ik = 1;
    g_LdwsSysParamter.uiHs = 111;

    #ifdef UPDATE_VP
        g_LdwsSysParamter.ieuslope = 0;
    #else
        g_LdwsSysParamter.ieuslope = -10;
    #endif


    // fixed parameters
    g_LdwsSysParamter.iInitRow = 57;
    g_LdwsSysParamter.iLsensitivity = 50;
    g_LdwsSysParamter.iRsensitivity = 50;
    g_LdwsSysParamter.iThreshold1 = 15;
    g_LdwsSysParamter.iThreshold2 = 10;
    g_LdwsSysParamter.iFsizeDq2 = 145*g_LdwsSysParamter.uiFocalLen/6;
    g_LdwsSysParamter.uiCamera2CarDistanceCm = 160;
    g_LdwsSysParamter.ieu = g_LdwsSysParamter.ik * g_LdwsSysParamter.uiFocalLen;
    g_LdwsSysParamter.iev = g_LdwsSysParamter.ik * g_LdwsSysParamter.uiFocalLen;

    LDWS_setInitParms(g_LdwsSysParamter);
    LDWS_setInitFlag(TRUE);

    // set initial value for ldws result structure
    g_LdwsInfor.DepartureDir = LDWS_DEPARTURE_NONE; // no departure
    g_LdwsInfor.Failure = LDWS_FAILURE_FALSE;
    g_LdwsInfor.iLx1 = 0;
    g_LdwsInfor.iLx2 = 0;
    g_LdwsInfor.iLy1 = 0;
    g_LdwsInfor.iLy2 = 0;
    g_LdwsInfor.iRx1 = 0;
    g_LdwsInfor.iRx2 = 0;
    g_LdwsInfor.iRy1 = 0;
    g_LdwsInfor.iRy2 = 0;
    g_LdwsInfor.uiVPYNew = 480 - (240 + g_LdwsSysParamter.ieuslope); // new VP
    g_LdwsInfor.uiVPYDef = 480 - (240 + g_LdwsSysParamter.ieuslope); // default VP


    return LDWSPARMS_STA_OK;

}

INT32 LdwsParms_UnInit(void)
{
    LDWS_setInitFlag(OFF);
    return LDWSPARMS_STA_OK;
}


INT32 LdwsParms_Process(void)
{
    INT32 Status;

    g_uiLdwsCount++;

    if (!g_LdwsLock)
    {
        if((g_uiLdwsCount%g_uiLdwsRate)==0)
        {
            if (LDWS_checkInitFlag() == OFF)
            {
               return LDWSPARMS_STA_ERROR;
            }
            else
            {
               Status = LdwsParms_Detection();//The main detection function
               return Status;
            }
        }
    }
    return LDWSPARMS_STA_OK;
}


INT32 LdwsParms_Detection(void)
{
    UINT32 IMEImgInBufAddr, RoiBufAddr;
    UINT32 g_uiUsedLdwsBufAddr;
    UINT32 T, totalT = 0, i;
    LDWS_SRCIMG_INFO LDWS_Src_info;

    IPL_IME_CurInfo1.Id = IPL_ID_1;

    //Get source frame data from ime output
    IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&IPL_IME_CurInfo1);
    g_uiLdwsSrcFrameAddr = IPL_IME_CurInfo1.ImeP1.PixelAddr[0];//Image source frame buffer Y address
    g_uiLdwsFrameWidth = IPL_IME_CurInfo1.ImeP1.Ch[0].Width;//Image source width
    g_uiLdwsFrameHeight = IPL_IME_CurInfo1.ImeP1.Ch[0].Height;//Image source height
    g_uiLdwsFrameLineOfs = IPL_IME_CurInfo1.ImeP1.Ch[0].LineOfs;//Image source line offset

    //DBG_DUMP("IPL_IME_CurInfo1.P1Enable = %d, gldwsFrameWidth = %d, gldwsFrameHeight = %d\r\n",IPL_IME_CurInfo1.P1Enable, gldwsFrameWidth,gldwsFrameHeight);

    if (g_uiLdwsFrameWidth > 0)
    {
        // copy IME data to ldws buffer
        IMEImgInBufAddr = g_uiLdwsBufAddr;

        #if 1
        LDWS_Src_info.uiLdRoiSx = g_uiLdwsFrameWidth / 2 - LDWS_ROI_SIZE_X/2;
        LDWS_Src_info.uiLdRoiSy = g_uiLdwsFrameHeight / 2 - LDWS_ROI_SIZE_Y/2;
        #else
        LDWS_Src_info.uiLdRoiSx = g_uiVanishPtx - LDWS_ROI_SIZE_X/2;
        LDWS_Src_info.uiLdRoiSy = g_uiVanishPty - LDWS_ROI_SIZE_Y/2;
        #endif

        //DBG_DUMP("(wid,hei) = (%d %d), Crop (x,y) = (%d %d)\r\n", gldwsFrameWidth, gldwsFrameHeight, LDWS_Src_info.uiLdRoiSx, LDWS_Src_info.uiLdRoiSy);

        RoiBufAddr = g_uiLdwsSrcFrameAddr + LDWS_Src_info.uiLdRoiSy*g_uiLdwsFrameWidth + LDWS_Src_info.uiLdRoiSx;
        hwmem_open();
        for(i = 0;i<LDWS_ROI_SIZE_Y;i++)
        {
            hwmem_memcpy(IMEImgInBufAddr + i*LDWS_ROI_SIZE_X, RoiBufAddr + i*g_uiLdwsFrameWidth, LDWS_ROI_SIZE_X);
        }
        hwmem_close();
        g_uiUsedLdwsBufAddr = IMEImgInBufAddr + LDWS_ROI_SIZE_X*LDWS_ROI_SIZE_Y;


        LDWS_Src_info.uiLdSrcBufAddr= IMEImgInBufAddr;
        LDWS_Src_info.uiLdSrcHeight= g_uiLdwsFrameHeight;
        LDWS_Src_info.uiLdSrcWidth= g_uiLdwsFrameWidth;
        LDWS_Src_info.uiLdSrcLineofs = g_uiLdwsFrameWidth;
        LDWS_Src_info.uiLdRoiWidth = LDWS_ROI_SIZE_X;
        LDWS_Src_info.uiLdRoiHeight = LDWS_ROI_SIZE_Y;

        T = LDWS_Detection(LDWS_Src_info, g_uiUsedLdwsBufAddr, g_LdwsSysParamter, &g_LdwsInfor);


        if (g_LdwsInfor.Failure)
        {
            g_LdwsInfor.iLx1 = 0;
            g_LdwsInfor.iLx2 = 0;
            g_LdwsInfor.iLy1 = 0;
            g_LdwsInfor.iLy2 = 0;
            g_LdwsInfor.iRx1 = 0;
            g_LdwsInfor.iRx2 = 0;
            g_LdwsInfor.iRy1 = 0;
            g_LdwsInfor.iRy2 = 0;
        }
        totalT+= T;

        LdwsProcessTime = T;
        //DBG_DUMP("time = %d\r\n",T);

        if ( (T > LdwsProcessMaxTime) && (g_uiLdwsCount >= 100) )
        {
            LdwsProcessMaxTime = T;
        }


        //LDWS_GetLdwsStatus();
        //DBG_DUMP("Ry1 = %d, Rx1 = %d\r\n",gLdws_infor.Ry1,gLdws_infor.Rx1);
        //DBG_DUMP("Ry2 = %d, Rx2 = %d\r\n",gLdws_infor.Ry2,gLdws_infor.Rx2);
        //DBG_DUMP("Ly1 = %d, Lx1 = %d\r\n",gLdws_infor.Ly1,gLdws_infor.Lx1);
        //DBG_DUMP("Ly2 = %d, Lx2 = %d\r\n",gLdws_infor.Ly2,gLdws_infor.Lx2);
        //DBG_DUMP("departure dir = %d\r\n",gLdws_infor.Departure_dir);
        //DBG_DUMP("Filure mode = %d\r\n",gLdws_infor.Failure);

        if(g_LdwsCbFP)
        {
            memcpy(&g_LdwsInfor.LdSrcImgInfo, &LDWS_Src_info, sizeof(LDWS_SRCIMG_INFO));
            g_LdwsCbFP(LDWS_CBMSG_WARNNING, (void *)&g_LdwsInfor);
        }
        else
        {
          //DBG_IND("\r\n LDWS:: gLdwsCbFP is not registered...   \r\n" );

        }

        g_LdwsInfor.DepartureDir = LDWS_DEPARTURE_NONE;
        g_LdwsInfor.Failure = LDWS_FAILURE_FALSE;
        g_LdwsInfor.iLx1 = 0;
        g_LdwsInfor.iLx2 = 0;
        g_LdwsInfor.iLy1 = 0;
        g_LdwsInfor.iLy2 = 0;
        g_LdwsInfor.iRx1 = 0;
        g_LdwsInfor.iRx2 = 0;
        g_LdwsInfor.iRy1 = 0;
        g_LdwsInfor.iRy2 = 0;


        g_uiLdwsFrameWidth = 0;

    }



    return LDWSPARMS_STA_OK;
}

UINT32 LdwsParms_setVanishingPtPos(UINT32 VanishPty)
{
    g_uiVanishPtx = 960;
    g_uiVanishPty = VanishPty;

    //DBG_DUMP("LDWS_SetVanishingPtPos, %d %d\r\n", g_uiVanishPtx, g_uiVanishPty);

    return LDWSPARMS_STA_OK;
}
UINT32 LdwsParms_getVanishingPtPos(void)
{
    return g_uiVanishPty;
}

LDWS_RESULT_INFO LdwsParms_getLdwsStatus(void)
{
    return g_LdwsInfor;
}

void LdwsParms_RegisterCB(LDWS_CBMSG_FP CB)
{
    g_LdwsCbFP = CB;
}

