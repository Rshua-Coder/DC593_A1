/**
    IPL Ctrl module(streaming) data type

    Define all data type needed by IPL Ctrl module(streaming).

    @file       IPL_Ctrl_flow_Int.h
    @ingroup    mIIPL_Ctrl
    @note       None

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/


#ifndef _IPL_CTRL_FLOW_INT_H
#define _IPL_CTRL_FLOW_INT_H
#include "Sysker.h"
#include "ipl_utility.h"
#include "IPL_Ctrl.h"
/** \addtogroup mIIPL_Ctrl */
//@{
/**
     IPL_CTRL Stop mode

     @note:inner status mechine(valid):
\n        run -> stop_all.

\n        run -> stop ycc ( -> ipc_pause_sie -> ipe_resume sie) -> ipc_stop_raw.
                                   |---<-----<----<----|

\n        run -> ipc_pause_sie -> ipc_resume_sie
                    |---<-----<----<----|

\n        run -> ipc_pause_pre -> ipc_resume_pre
                    |---<-----<----<----|
*/
typedef enum
{
    IPC_STOP_UNKNOWN = 0,    ///< Description
    IPC_STOP_ALL,       ///< stop and close all engine
    IPC_STOP_YCC,       ///< stop and close all engine but sie, sie engine still in running
    IPC_PAUSE_SIE,      ///< stop sie (sie still open, not close)
    IPC_RESUME_SIE,     ///< start sie from status:IPC_PAUSE_SIE
    IPC_PAUSE_SIE2,     ///< stop sie2(ccir) (sie2 still open, not close, and sie already pause)
    IPC_PAUSE_SIE2_ONLY,//< stop sie2(ccir) only, sie still work(sie2 still open, not close)
    IPC_RESUME_SIE2,    ///< start sie2(ccir)  from status:IPC_PAUSE_SIE2
    IPC_STOP_RAW,       ///< stop and close sie engine
    IPC_PAUSE_PRE,      ///< pause pre,
    IPC_RESUME_PRE,     ///< resume pre.
    ENUM_DUMMY4WORD(IPC_STOP)
}IPC_STOP;

/**
     IPL_Ctrl RunTime chang IPL configuration

     only valid when IPL_Ctrl in run_status ,
     excluding :"IPC_SIE",   "IPC_Chg_ImeP1Out", "IPC_Chg_ImeP2Out",
                "IPC_Chg_ImeP3Out", "IPC_Chg_Crop", "IPC_Chg_Load" !

     @note :
\n           "IPC_SIE" only valid when IPL stop at "IPC_PAUSE_SIE"
\n           "IPC_Chg_ImeP1Out","IPC_Chg_ImeP2Out","IPC_Chg_ImeP3Out","IPC_Chg_Crop"
             these four enum will be latched until chg == "IPC_Chg_Load".
\n              ex:
\n                  IPL_CTRLRunTimeChg(IPC_Chg_ImeP2Out,ptr);
\n                  IPL_CTRLRunTimeChg(IPC_Chg_DCE,ptr);               this cmd will fail.
\n                  IPL_CTRLRunTimeChg(IPC_Chg_Load,NULL);

*/
typedef enum _IPC_RUNTIMECHG
{
    IPC_UNKNOW          = 0x00000000,
    IPC_Dzoom           = 0x00000001,   ///<                                                ,data type: IPC_Dzoom_Info
    IPC_SIE_            = 0x00000002,   ///< control sie switch.ex:prv2cap or cap2prv       ,data type :IPC_SIE_Info
    IPC_SenChgMode      = 0x00000003,   ///< chg sensor mode when pipe line running         ,data type :IPC_SenChgMode_Info
    IPC_SIE2_           = 0x00000004,   ///< control sie2 switch.ex:prv2cap or cap2prv      ,data type :IPC_SIE_Info

    //IPC_Chg_ChromaA_Retinex = 0x00000004,   ///< chg chroma adaption/Retinex parameter,     ,data type :IPC_ChromaA_Retinex_Info

    IPC_Chg_PRE_INAddr  = 0x00000005,   ///< chg pre input address                          ,data type :IPC_PREInAddr
    IPC_Chg_DCE         = 0x00000006,   ///< chg Distortion configuration                   ,data type :IPC_IQparam
    //IPC_Chg_IpeSubOut   = 0x00000007,   ///< chg CC/V SubImg size/VsubOutSel/FuncEn         ,data type :IPC_IQparam

    IPC_Chg_ImeP1Out    = 0x00001000,   ///< chg path1 output size/lineofs/fmt/PathEn       ,data type :IPC_IMEOut_Info
    IPC_Chg_ImeP2Out    = 0x00002000,   ///< chg path2 output size/lineofs/fmt/PathEn       ,data type :IPC_IMEOut_Info
    IPC_Chg_ImeP3Out    = 0x00004000,   ///< chg path3 output size/lineofs/fmt/PathEn       ,data type :IPC_IMEOut_Info
    IPC_Chg_Crop        = 0x00008000,   ///< chg sie out and pre in size/lineofs            ,data type :IPC_FRONTEND
    IPC_Chg_Load        = 0x00010000,   ///< load command 0x00001000~ 0x00008000 into reg.  ,data type :none
    ENUM_DUMMY4WORD(IPC_RUNTIMECHG)
}IPC_RUNTIMECHG;

/**
     IPL_Ctrl RunTime chang IPL IQ configuration

*/
typedef enum _IPC_RUNTIMECHG_IQ
{

    IPC_SIE_IQparam     = 0x10000000,   ///< chg SIE iq parameter set                       ,data type :IPC_IQparam
    IPC_PRE_IQparam     = 0x20000000,   ///< chg PRE iq parameter set                       ,data type :IPC_IQparam
    IPC_IFE_IQparam     = 0x30000000,   ///< chg IFE iq parameter set                       ,data type :IPC_IQparam
    IPC_IPE_IQparam     = 0x40000000,   ///< chg IPE iq parameter set                       ,data type :IPC_IQparam
    IPC_IFE2_IQparam    = 0x50000000,   ///< chg IFE2 iq parameter set                      ,data type :IPC_IQparam
    IPC_IME_IQparam     = 0x60000000,   ///< chg IME iq parameter set                       ,data type :IPC_IQparam
    //IPC_SIE2_IQparam    = 0x70000000,   ///< chg sie2 iq parameter set                      ,data type :IPC_IQparam
    ENUM_DUMMY4WORD(IPC_RUNTIMECHG_IQ)
}IPC_RUNTIMECHG_IQ;

/**
     IQ runTimeChg.

     definition for Engine Quality chg when IPL in "IPL_RUN" status.
*/
typedef struct
{
    IPL_HAL_PARASET Idx;
    IPL_HAL_GROUP   *pGrp;
    ENG_SUBFUNC     EngSubF;
}IPC_IQparam;

/**
     PRE dma input address runTimeChg.

     definition for change pre dma input address when IPL in "IPL_RUN" status.
*/
typedef struct
{
    IPL_HAL_PARASET Idx;
    UINT32 PPB0Addr;
    UINT32 PPB1Addr;
}IPC_PREInAddr;

/**
     ime dma output configuration runTimeChg.

     definition for change ime dma output configuration when IPL in "IPL_RUN" status.
     @note:
\n   can't change output address and scaling mehtod here
\n   scale IPL output image.
\n   control IPL dma output pathEn
*/
typedef struct
{
    IPL_HAL_PARASET Idx;
    OUTCH           Path;//output address and ScalMethod are useless here
}IPC_IMEOut_Info;

/**
     IPL raw_image in/out size runTimeChg.

     definition for change sie output size/pre inout size when IPL in "IPL_RUN" status.
     @note:
\n   support digital zoom application.
\n   support cropping application.
*/
typedef struct
{
    IPL_HAL_PARASET Idx;

    UINT32 sieCropHini;
    UINT32 sieCropVini;
    UINT32 sieCropH;
    UINT32 sieCropV;
    UINT32 sieOutH;
    UINT32 sieOutV;
    UINT32 sieOutLineOfs;
    UINT32 preInH;
    UINT32 preInV;
    UINT32 preInLoneOfs;

    BOOL   bChgCAParam;
    UINT32 Rev;
}IPC_Dzoom_Info;
typedef IPC_Dzoom_Info IPC_FRONTEND;

/**
     SIE control.

     reset SIE engine.
*/
typedef struct
{
    IPL_HAL_PARASET Idx;
    SIE_UPDATE sieUpdate;
    SIE *psie;

    SIE2_UPDATE sie2Update;
    SIE2 *psie2;
}IPC_SIE_Info;

/*
     not rdy yet!

     Detailed description.
     @note (Optional)
*/
typedef struct
{
    IPL_HAL_PARASET Idx;
    void (*fpSenCmd)(void);
    SIE_VACC    *pSieVa;
    IPE__VA      *pIpeVa;
}IPC_SenChgMode_Info;

/*
typedef struct
{
    BOOL            ChromaAdapt_En;
    IME_CHRA_ADAP*  pChromaAdaption;
    BOOL            Retinex_En;
    IME_RETINEX*    pRetinex;
    IFE2_Chroma*    pChromaAdj;            ///< UVpack only
    IFE2_AvgFilter* pAvgFilter;
}IPC_ChromaA_Retinex_Info;
*/



#if 0
- structure of supported flow
#endif

/**
     IPL_Ctrl flow.

     IPL_Ctrl module support following control flows.
*/
typedef enum _IPLCTRL_FLOW
{
    ICF_REV1        = 0,
    ICF_FLOW_A      = 1,    ///< all engines control by HW auto handshake
    ICF_FLOW_B      = 2,    ///< all engines control by FW handshake and synchronous with POST-PROCESS controller.
                            ///< fps depends not only by sensor, also by POST-PROCESS controller.
    ICF_FLOW_C      = 3,    ///< all engines control by FW handshake
    ICF_FLOW_D      = 4,    ///< dual sensor(BAYER + BAYER)
    //ICF_FLOW_E      = 5,    ///< dual sensor(CCIR + BAYER)
    ICF_MAX         = 6,
    ENUM_DUMMY4WORD(IPLCTRL_FLOW)
}IPLCTRL_FLOW;


/**
    Enum for query IPL information
*/
typedef enum _IPLCTRL_QUERY
{
    ICF_FLOWA           = ICF_FLOW_A,   ///< minimum buffer number needed by flow_A.
    ICF_FLOWB           = ICF_FLOW_B,   ///< minimum buffer number needed by flow_B.
    ICF_FLOWC           = ICF_FLOW_C,   ///< minimum buffer number needed by flow_C.
    ICF_FLOWD           = ICF_FLOW_D,   ///< minimum buffer number needed by flow_D with SIE1.
    //ICF_FLOWE           = ICF_FLOW_E, ///< minimum buffer number needed by flow_E.
    ICF_FLOWD_CCIR      = 7 ,           ///< minimum buffer number needed by flow_D with CCIR.
    ICF_CSUB_RDY        = 10,           ///< current ready CC sub-image
    ICF_VSUB_RDY        = 11,           ///< current ready V sub-iamge
    ICF_ETHOUT_RDY      = 12,           ///< current ready edge threshold output
    ICF_PREIO_RDY       = 13,           ///< current ready Pre input ch0 info.
    ICF_CCIR_RDY        = 14,           ///< current ready CCIR output info.
    ICF_SIE_CH0_RDY     = 15,           ///< current ready SIE CH0 output info.
    ICF_INDVA_RDY       = 16,           ///< current ready ind va result,
    ICF_CAPARAM_RDY     = 17,
    ICF_FRAME_CNT       = 18,
    ENUM_DUMMY4WORD(IPLCTRL_QUERY)
}IPLCTRL_QUERY;

/**
     data structure of ICF_FLOW_A

*/
typedef struct _ICF_FLOW_A_STRUCT
{
    IPL_HAL_PARASET  ParaIdx;           ///<indicate IPL_HAL parameter set index

    IPLC_BASEINFO    BaseInfo;          ///< IPL_Ctrl main-image configuration

    SIE_CH1_Dma_Out  CA_PPB;            ///<ca out

    UINT32           IPE_PPB0_LCA_ADDR;     ///<CbCr out(CbCr addr are continuous)
    UINT32           IPE_PPB1_LCA_ADDR;     ///<CbCr out(CbCr addr are continuous)

    UINT32           IFE2_PPB0_ADDR;        ///<ife2 output addr
    UINT32           IFE2_PPB1_ADDR;        ///<ife2 output addr

    ///<ife2 d2d operation start and end callback
    fpEngInit        fp_Ife2Init;       ///<CB when ife2 operation init at each frame.(need return ife2 repeat time)
    fpEngInit        fp_Ife2Setting;    ///<CB at each ife2 operation end
    ///<ipe C/Vsub_out CB.
    fpEngSubFunc     fp_IpeCsub;
    fpEngSubFunc     fp_IpeVsub;
    ///<optional information
    IPLC_EXT         Ext;
}ICF_FLOW_A_STRUCT,*PICF_FLOW_A_STRUCT;

/**
     data structure of ICF_FLOW_B

*/
typedef struct _ICF_FLOW_B_STRUCT
{
    IPL_HAL_PARASET  ParaIdx;           ///<indicate IPL_HAL parameter set index

    IPLC_BASEINFO    BaseInfo;          ///< IPL_Ctrl main-image configuration

    SIE_CH1_Dma_Out  CA_PPB;            ///<ca out

    UINT32           IPE_PPB0_LCA_ADDR;     ///<CbCr out(CbCr addr are continuous)
    UINT32           IPE_PPB1_LCA_ADDR;     ///<CbCr out(CbCr addr are continuous)

    UINT32           IPE_PPB0_HDR_ADDR;     ///<v out
    UINT32           IPE_PPB1_HDR_ADDR;     ///<v out

    UINT32           IFE2_PPB0_ADDR;        ///<ife2 output addr
    UINT32           IFE2_PPB1_ADDR;        ///<ife2 output addr

    ///<ife2 d2d operation start and end callback
    fpEngInit        fp_Ife2Init;       ///<CB when ife2 operation init at each frame.(need return ife2 repeat time)
    fpEngInit        fp_Ife2Setting;    ///<CB at each ife2 operation end
    ///<ime chroma adapation CB
    fpEngSubFunc     fp_AdaptSetting;   ///<CB at each pre2ime fw_operation start,
                                        ///<return NULL if no need to update, otherwise,return ptr of update data
    ///<ime retienx CB
    fpEngSubFunc     fp_RetinexSetting; ///<CB at each pre2ime fw_operation start,
                                        ///<return NULL if no need to update, otherwise,return ptr of update data

    ///<ipe C/Vsub_out CB, if Adaption being enabled, Csub CB will be ingored.
    fpEngSubFunc     fp_IpeCsub;        ///<CB at each pre2ime fw operation start
                                        ///<return NULL if no need to update, otherwise,return ptr of update data
    fpEngSubFunc     fp_IpeVsub;        ///<CB at each pre2ime fw operation start
                                        ///<return NULL if no need to update, otherwise,return ptr of update data
    ///<optional information
    IPLC_EXT         Ext;
}ICF_FLOW_B_STRUCT,*PICF_FLOW_B_STRUCT;

/**
     data structure of ICF_FLOW_C

*/
typedef struct _ICF_FLOW_C_STRUCT
{
    IPL_HAL_PARASET  ParaIdx;           ///<indicate IPL_HAL parameter set index

    IPLC_BASEINFO    BaseInfo;          ///< IPL_Ctrl main-image configuration

    SIE_CH1_Dma_Out  CA_PPB;            ///<ca out

    UINT32           IPE_PPB0_LCA_ADDR;     ///<CbCr out.(CbCr addr are continuous)
    UINT32           IPE_PPB1_LCA_ADDR;     ///<CbCr out.(CbCr addr are continuous)

    UINT32           IFE2_PPB0_ADDR;        ///<ife2 output addr
    UINT32           IFE2_PPB1_ADDR;        ///<ife2 output addr

    ///<ife2 d2d operation start and end callback
    fpEngInit        fp_Ife2Init;       ///<CB when ife2 operation init at each frame.(need return ife2 repeat time)
    fpEngInit        fp_Ife2Setting;    ///<CB at each ife2 operation end
    ///<ime chroma adapation CB
    fpEngSubFunc     fp_AdaptSetting;   ///<CB at each pre2ime fw_operation start,
                                        ///<return NULL if no need to update, otherwise,return ptr of update data
                                        ///<if funcEn is FALSE, must return NULL!
    ///<ipe C/Vsub_out CB, if Adaption being enabled, Csub CB will be ingored.
    fpEngSubFunc     fp_IpeCsub;        ///<CB at each pre2ime fw operation start
                                        ///<return NULL if no need to update, otherwise,return ptr of update data
    fpEngSubFunc     fp_IpeVsub;        ///<CB at each pre2ime fw operation start
                                        ///<return NULL if no need to update, otherwise,return ptr of update data
    ///<optional information
    IPLC_EXT         Ext;
    struct
    {
        BOOL                CCIR_En;            ///< CCIR function enable
        SIE2_WINDOW         SIE2_IN;            ///< sie input window
        IPP_SIE2_SIGNAL     SenSorIn;           ///< sensor input format
        SIE2_BURST_LENG     Sie2_BL_Ch0;        ///< sie ch0 dram burst length
        UINT32              SIE2_PPB0_ADDR;     ///< out
        UINT32              SIE2_PPB1_ADDR;     ///< out
        fpccirFmt           fp_CCIRFmt;
        fpEngCLK            fp_Sie2Clk;         ///< engine clk CB, default clk will be applied to when CB is NULL!
        fpSIEPclkPad        fp_SIE2PclkSel;     ///< sie engine pclk selection CB
        IPL_YCC_IMG_INFO    YUV_PPB;
    }CCIR;                                      ///< for sie2 CCIR function only
}ICF_FLOW_C_STRUCT,*PICF_FLOW_C_STRUCT;

/**
     data structure of ICF_FLOW_D

     @note (Optional)
*/
typedef struct _ICF_FLOW_D_STRUCT
{
    IPL_HAL_PARASET  ParaIdx;           ///<indicate IPL_HAL parameter set index

    IPLC_BASEINFO    BaseInfo;          ///< IPL_Ctrl main-image configuration

    SIE_CH1_Dma_Out  CA_PPB;            ///<ca out

    UINT32           IPE_PPB0_LCA_ADDR;     ///<CbCr out.(CbCr addr are continuous)
    UINT32           IPE_PPB1_LCA_ADDR;     ///<CbCr out.(CbCr addr are continuous)

    UINT32           IFE2_PPB0_ADDR;        ///<ife2 output addr
    UINT32           IFE2_PPB1_ADDR;        ///<ife2 output addr

    ///<ife2 d2d operation start and end callback
    fpEngInit        fp_Ife2Init;       ///<CB when ife2 operation init at each frame.(need return ife2 repeat time)
    fpEngInit        fp_Ife2Setting;    ///<CB at each ife2 operation end
    ///<ime chroma adapation CB
    fpEngSubFunc     fp_AdaptSetting;   ///<CB at each pre2ime fw_operation start,
                                        ///<return NULL if no need to update, otherwise,return ptr of update data
                                        ///<if funcEn is FALSE, must return NULL!
    ///<ipe C/Vsub_out CB, if Adaption being enabled, Csub CB will be ingored.
    fpEngSubFunc     fp_IpeCsub;        ///<CB at each pre2ime fw operation start
                                        ///<return NULL if no need to update, otherwise,return ptr of update data
    fpEngSubFunc     fp_IpeVsub;        ///<CB at each pre2ime fw operation start
                                        ///<return NULL if no need to update, otherwise,return ptr of update data
    ///<optional information
    IPLC_EXT         Ext;
    BOOL                SIE1_En;            ///< SIE1 function enable
    struct
    {
        BOOL                CCIR_En;            ///< CCIR function enable
        SIE2_WINDOW         SIE2_IN;            ///< sie input window
        IPP_SIE2_SIGNAL     SenSorIn;           ///< sensor input format
        SIE2_BURST_LENG     Sie2_BL_Ch0;        ///< sie ch0 dram burst length
        UINT32              SIE2_PPB0_ADDR;     ///< out
        UINT32              SIE2_PPB1_ADDR;     ///< out
        fpccirFmt           fp_CCIRFmt;
        fpEngCLK            fp_Sie2Clk;         ///< engine clk CB, default clk will be applied to when CB is NULL!
        fpSIEPclkPad        fp_SIE2PclkSel;             ///< sie engine pclk selection CB
        IPL_YCC_IMG_INFO    YUV_PPB;
    }CCIR;                                      ///< for sie2 CCIR function only
}ICF_FLOW_D_STRUCT,*PICF_FLOW_D_STRUCT;

/**
    feedback information when querry "ICF_FLOWA".
*/
typedef struct _QUERY_FLOW_A
{
    UINT32 nBUF_SIEPPB;     ///< sie buffer num
    UINT32 nBUF_CAPPB;      ///< sie ca buffer num
    UINT32 nBUF_SIE_VAPPB;  ///< sie va buffer num
    UINT32 nBUF_IPE_LCAPPB; ///< ipe LCA CbCr buffer num
    UINT32 nBUF_IPE_VAPPB;  ///< ipe VA buffer num
    UINT32 nBUF_IFE2PPB;    ///< ife2 working buffer num
    UINT32 nBUF_IMEPPB;     ///< only return ime 1 path requirement (all 3 pathes need the same pingpong buf num)
}QUE_FLOW_A;

/**
    feedback information when querry "ICF_FLOW_B".
*/
typedef struct _QUERY_FLOW_B
{
    UINT32 nBUF_SIEPPB;         ///< sie buffer num
    UINT32 nBUF_CAPPB;          ///< sie ca buffer num
    UINT32 nBUF_SIE_VAPPB;      ///< sie va buffer num
    UINT32 nBUF_IPE_LCAPPB;     ///< ipe LCA CbCr buffer num
    UINT32 nBUF_IPE_HDRPPB;     ///< ipe HDR V buf num
    UINT32 nBUF_IPE_VAPPB;      ///< ipe VA buffer num
    UINT32 nBUF_IFE2PPB;        ///< ife2 working buffer num
    UINT32 nBUF_IMEPPB;         ///< only return ime 1 path requirement (all 3 pathes need the same pingpong buf num)
}QUE_FLOW_B;

/**
    feedback information when querry "ICF_FLOW_C".
*/
typedef struct _QUE_FLOW_C
{
    UINT32 nBUF_SIEPPB;     ///< sie buffer num
    UINT32 nBUF_CAPPB;      ///< sie ca buffer num
    UINT32 nBUF_SIE_VAPPB;  ///< sie va buffer num
    UINT32 nBUF_IPE_LCAPPB; ///< ipe LCA CbCr buffer num
    UINT32 nBUF_IPE_VAPPB;  ///< ipe VA buffer num
    UINT32 nBUF_IFE2PPB;    ///< ife2 working buffer num
    UINT32 nBUF_IMEPPB;     ///< only return ime 1 path requirement (all 3 pathes need the same pingpong buf num)

    //only needed when CCIR function enable
    UINT32 nBUF_BAYER2PPB;
    UINT32 nBUF_YCC2PPB;
}QUE_FLOW_C;

/**
    feedback information when querry "ICF_FLOW_D".
*/
typedef struct _QUE_FLOW_D
{
    UINT32 nBUF_SIEPPB;     ///< sie buffer num
    UINT32 nBUF_CCIRPPB;    ///< ccir buffer num
    UINT32 nBUF_CAPPB;      ///< sie ca buffer num
    UINT32 nBUF_SIE_VAPPB;  ///< sie va buffer num
    UINT32 nBUF_IPE_LCAPPB; ///< ipe LCA CbCr buffer num
    UINT32 nBUF_IPE_VAPPB;  ///< ipe VA buffer num
    UINT32 nBUF_IFE2PPB;    ///< ife2 working buffer num
    UINT32 nBUF_IMEPPB;     ///< only return ime 1 path requirement (all 3 pathes need the same pingpong buf num)

    //only needed when CCIR function enable
    UINT32 nBUF_BAYER2PPB;
    UINT32 nBUF_YCC2PPB;
}QUE_FLOW_D;

//@}
#endif
