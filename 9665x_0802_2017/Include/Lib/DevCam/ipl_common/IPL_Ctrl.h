/**
    IPL_Ctrl module(D2D,dram to dram)

    This D2D module use 3 opeartion to model IPL D2D operation:
\n  IPC_D2D_Start   : start engine d2d behavior
\n  IPC_WaitFinish  : wait engine d2d behavior done
\n  IPC_D2D_End     : close engine.

    @file       IPL_Ctrl.h
    @ingroup    mILibIPH
    @note
\n  example of standard operation:
\n
\n          IPC_D2D_Start(...);
\n          IPC_WaitFinish(...);
\n          IPC_D2D_End(...);

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _IPL_CTRL_H_
#define _IPL_CTRL_H_
#include "IPL_Ctrl_Par.h"
/** \addtogroup mILibIPH */
//@{

/**
     D2D result location.

     decribe current ready data is in InputAddress or OutputAddress.
*/
typedef enum _DataDest
{
    _InputADDR  = 0,    ///< d2d output data in inputaddr
    _OutputADDR = 1,    ///< d2d output data in outputaddr
    ENUM_DUMMY4WORD(DataDest)
}DataDest;

/**
     Supported D2D operation
*/
typedef enum _IPLC_D2D
{
    IPLC_PRE    = 0, ///< PRE D2D
    IPLC_IPE,        ///< IPE D2D
    IPLC_DCE,        ///< DCE D2D
    IPLC_IFE,        ///< IFE D2D
    IPLC_IFE2,       ///< IFE2 D2D
    IPLC_IME,        ///< IME D2D
    IPLC_RDE,        ///< RDE D2D
    IPLC_PREIME,     ///< PRE IN, IME OUT(all engine direct path)
    IPLC_MAX,
    ENUM_DUMMY4WORD(IPLC_D2D)
}IPLC_D2D;

/**
     IPLC_PRE data structure

     PRE D2D configuration.
*/
typedef struct _IPLC_PRE
{
    IPL_HAL_PARASET ParIdx; ///< set configuration into indicated IPH inner table
    UINT32 eventID;         ///< map to PRE_D2D_CB eventID if PRE_D2D_CB exist
    IPH_PRECLK  clk;        ///< engine clk freq.
    PRE_IN_DMA  In;         ///< dram input info.
    PRE_OUT     out;        ///< ouput info.
    fpEngInit   fp_Init;    ///< this CB will replace eventID's CB if existed.
}IPLC_PRE_INFO;

/**
     IPLC_IPE data structure

     IPE D2D configuration.
*/
typedef struct _IPLC_IPE
{
    IPL_HAL_PARASET ParIdx;     ///< -
    UINT32 eventID;             ///< map to IPE_D2D_CB eventID if IPE_D2D_CB exist
    IPH_IPECLK  clk;            ///< engine clk freq.

    struct
    {
        IPESIZE Size;           ///< -
        enum
        {
            IPE_In_Bayer    = 0,    ///< input format is Bayer
            IPE_In_NonBayer = 1,    ///< input format is Y or Cb or Cr or UV packed ...etc.
            ENUM_DUMMY4WORD(IPEInFmt)
        }IPEInFmt;              ///< -
        IPE_PackBus packBus;    ///< only be needed in bayer format
        IPE_CFA Cfa;            ///< only be needed in bayer format
        UINT32  Addr;           ///< -
    }IPE_In;                    ///< input info.

    struct
    {
        IPEOutFmt   IPEFmt;     ///< -
        //addr
        UINT32  Addr_Y;         ///< -
        UINT32  Addr_UV;        ///< -
        //lineoffset
        UINT32  Lineofs_Y;      ///< -
        UINT32  Lineofs_UV;     ///< -
    }IPE_Out;                   ///< output info.
    fpEngInit   fp_Init;        ///< this CB will replace eventID's CB if existed.
}IPLC_IPE_INFO;

/**
     IPLC_DCE data structure

     DCE D2D configuration.
*/
typedef struct _IPLC_DCE
{
    IPL_HAL_PARASET ParIdx; ///< -
    UINT32 eventID;         ///< map to DCE_D2D_CB eventID
    IPH_DCECLK  clk;        ///< engine clk freq.

    USIZE       Size;       ///< input/out size
    struct
    {
        UINT32  Addr_Y;     ///< -
        UINT32  Addr_UV;    ///< -
        UINT32  Lineoffset_Y;///< -
        UINT32  Lineoffset_UV;///< -
        DCEFMT  Fmt;            ///< -
    } Dram_In;              ///< dram input info., only needed when dce dram in

    struct
    {
        UINT32  Addr_Y;     ///< -
        UINT32  Addr_UV;    ///< -
        UINT32  Lineoffset_Y;   ///< -
        UINT32  Lineoffset_UV;  ///< -
    } Dram_Out;             ///< dram output info., only needed when dce dram out

    MEM_RANGE*  pDceStpe;   ///< dce stripe table, NULL if no pre-calculation.
    fpEngInit   fp_Init;    ///< this CB will replace eventID's CB if existed.
}IPLC_DCE_INFO;

/**
     IPLC_IFE data structure

     IFE D2D configuration.
*/
typedef struct _IPLC_IFE
{
    IPL_HAL_PARASET ParIdx; ///< -
    UINT32 eventID;         ///< map to IFE_D2D_CB eventID if IFE_D2D_CB exist
    IPH_IFECLK  clk;        ///< engine clk freq.
    struct
    {
        UINT32  Addr;   ///< -
        UINT32  Width;  ///< -
        UINT32  Height; ///< -
        UINT32  Lineoffset; ///< -
        IFE_PackBus packBus;///< -
    }IFE_In;                ///< input info.

    struct
    {
        UINT32  Addr;   ///< -
        UINT32  Lineoffset;///< -
        IFE_PackBus packBus;///< -
    }IFE_Out;               ///< output info.

    IFEFMT IFEFmt;          ///< input data format
    fpEngInit   fp_Init;    ///< this CB will replace eventID's CB if existed.
}IPLC_IFE_INFO;

/**
     IPLC_IFE2 data structure

     IFE2 D2D configuration.
*/
typedef struct _IPLC_IFE2_INFO
{
    IPL_HAL_PARASET ParIdx; ///< -
    UINT32 eventID;         ///< map to IFE2_D2D_CB eventID if IFE2_D2D_CB exist
    IPH_IFE2CLK clk;        ///< engine clk freq.
    UINT32 InAddr;          ///< input data address
    IPL_YCC_CH_INFO InSize; ///< dram input size
    UINT32 OutAddr;         ///< Output data addrss
    UINT32 OutLineoffset;   ///< dram output lineofs
    IFE2_FMT Fmt;           ///< input data format
    IFE2_DBMODE IFE2D2DDebugMode;
    fpEngInit   fp_Init;    ///< this CB will replace eventID's CB if existed.
}IPLC_IFE2_INFO;

/**
     IPLC_IME data structure

     IME D2D configuration.
*/
typedef struct _IPLC_IME
{
    IPL_HAL_PARASET ParIdx; ///< -
    UINT32 eventID;         ///< map to IME_D2D_CB eventID if IME_D2D_CB exist
    IPH_IMECLK  clk;        ///< engine clk freq.

    struct
    {
        IMEADDR PPB0;       ///< -
        IMESIZE Size;       ///< -
        IMEFMT Fmt;         ///< -

        struct
        {
            IPL_YCC_CH_INFO Size;///< -
            UINT32 Addr;    ///< -
        }Retinex;           ///< retinex dram input info

        struct
        {
            IPL_YCC_CH_INFO Size;///< -
            UINT32 Addr;    ///< -
        }Adaption;          ///< chroma adaption dram input info
    }IME_In;                ///< input info.

    struct
    {
        BOOL    PathEn;         ///< path enable
        IMEADDR Addr;           ///< -
        IMESIZE Size;           ///< -
        IMEFMT Fmt;             ///< support Y/Cb/Cr 3 planer and Y/UV pack format
        IME_SCALER ScalMethod;  ///< -
    }Ime_Out_P1;                ///< path1 ycc data dram output info.

    struct
    {
        BOOL    PathEn;         ///< path enable
        IMEADDR Addr;           ///< -
        IMESIZE Size;           ///< -
        IMEFMT Fmt;             ///< support only Y/UV pack format
        IME_SCALER ScalMethod;  ///< -
    }Ime_Out_P2;                ///< path2 ycc data dram output info.

    struct
    {
        BOOL    PathEn;         ///< path enable
        IMEADDR Addr;           ///< -
        IMESIZE Size;           ///< -
        IMEFMT Fmt;             ///< support only Y/UV pack format
        IME_SCALER ScalMethod;  ///< -
    }Ime_Out_P3;                ///< path3 ycc data dram output info.
    fpEngInit   fp_Init;        ///< this CB will replace eventID's CB if existed.
}IPLC_IME_INFO;

/**
     IPLC_RDE data structure

     RDE D2D configuration.
*/
typedef struct _IPLC_RDE_INFO
{
    IPL_HAL_PARASET ParIdx;     ///< -
    UINT32 eventID;             ///< map to RDE_D2D_CB eventID if RDE_D2D_CB exist
    IPH_RDECLK  clk;            ///< engine clk freq.

    RDE_IN  In;                 ///< Dram input info
    RDE_OUT Out;                ///< Dram output info
}IPLC_RDE_INFO;


/**
     IPLC_PREIME data structure(pre_ife_ipe(dce)_ime)

     pre to ime in direct_path. (d2d frame mode)
*/
typedef struct _IPLC_PREIME
{
    IPL_HAL_PARASET ParIdx; ///< -
    UINT32 eventID;         ///< map to PREIME_D2D_xxxCB eventID if PREIME_D2D_xxxCB exist
    IPH_PRECLK  PREclk;     ///< engine clk freq.
    IPH_IPECLK  IPEclk;     ///< engine clk freq.
    IPH_DCECLK  DCEclk;     ///< engine clk freq.
    IPH_IFECLK  IFEclk;     ///< engine clk freq.
    IPH_IMECLK  IMEclk;     ///< engine clk freq.

    PRE_IN_DMA  In;         ///< raw iamge input infor.

    struct
    {
        UINT32 Lineofs;     ///< -
        UINT32 Addr;        ///< -
    }Dfs;                   ///< dark frame dram input info.

    struct
    {
        IPL_YCC_CH_INFO Size;   ///< -
        UINT32 Addr;        ///< -
    }Retinex;               ///< retinex dram input info

    struct
    {
        IPL_YCC_CH_INFO Size;   ///< -
        UINT32 Addr;        ///< -
    }Adaption;              ///< chroma adaption input info

    struct
    {
        BOOL CCFuncEn;      ///< CC sub output enable bit
        IPE_CC_SUBOUT Csub; ///< -
        BOOL VFuncEn;       ///< V sub output enable bit
        IPE_V_SUBOUT Vsub;  ///< -
    }IPE_sub;               ///< ipe sub-image output

    struct
    {
        BOOL    PathEn;         ///< path enable
        IMEADDR Addr;       ///< -
        IMESIZE Size;       ///< -
        IMEFMT Fmt;         ///< support Y/Cb/Cr 3 planer and Y/UV pack format
        IME_SCALER ScalMethod;
    }Ime_Out_P1;            ///< path1 ycc data dram output info.

    struct
    {
        BOOL    PathEn;         ///< path enable
        IMEADDR Addr;       ///< -
        IMESIZE Size;       ///< -
        IMEFMT Fmt;         ///< support only Y/UV pack format
        IME_SCALER ScalMethod;
    }Ime_Out_P2;            ///< path2 ycc data dram output info.

    struct
    {
        BOOL    PathEn;         ///< path enable
        IMEADDR Addr;       ///< -
        IMESIZE Size;       ///< -
        IMEFMT Fmt;         ///< support only Y/UV pack format
        IME_SCALER ScalMethod;///< -
    }Ime_Out_P3;            ///< path3 ycc data dram output info.

    MEM_RANGE*  pDceStpe;   ///< dce stripe table, NULL if no pre-calculation.

    fpEngInit   fp_Init[IPL_HAL_MAX];    ///< this CB will replace eventID's CB if existed. effeicient index:2,3,4,6,7
}IPLC_PREIME_INFO;

/**
     IPLC_PREIME data structure

*/
typedef union _IPCL_D2D_STRUCT
{
    IPLC_PRE_INFO PRE;          ///< for pre d2d
    IPLC_IPE_INFO IPE;
    IPLC_DCE_INFO DCE;          ///< for dce d2d
    IPLC_IFE_INFO IFE;          ///< for ife d2d
    IPLC_IFE2_INFO IFE2;        ///< for ife2 d2d
    IPLC_IME_INFO IME;          ///< for ime d2d
    IPLC_RDE_INFO RDE;          ///< for rde d2d
    IPLC_PREIME_INFO PREIME;    ///< for d2d frame mode
}IPLC_D2D_STRUCT;

typedef struct _IPCL_IMEP3_CROP_INFO
{
    BOOL CropEn;
    UINT32 Crop_Start_X;
    UINT32 Crop_Start_Y;
    UINT32 Crop_Size_H;
    UINT32 Crop_Size_V;
    IME_SCALER ScalMethod;
}IPCL_IMEP3_CROP_INFO;

//@{
/**
     DCE d2d configuration

     a weak symbol for d2d opeartion, it will be called right before DCE d2d start, in order to get
     DCE configuration.
     @note
        if this symbol is undefined, DCE d2d will return error and drop start operation!(beccause of stripe hw constraint)

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    Ref_1x:     sensor output size
     @param[out]    Ref_1xCen   distortion center of sensor output
     @param[out]    subF:       dce configuration
     @param[out]    group:      IPH DCE update item

     @return Description of data returned.
         - @b return value is the repeat time of dce d2d operation.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 DCE_D2D_CB(USIZE *Ref_1x,Coordinate *Ref_1xCen,DCE_SUBFUNC* subF,DCE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     PRE d2d configuration

     a weak symbol for d2d operation. it will be called right before PRE d2d start, in order to get
     PRE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       pre configuration
     @param[out]    group:      IPH PRE update item

     @return Description of data returned.
         - @b return value is the repeat time of pre d2d operation.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 PRE_D2D_CB(PRE_SUBFUNC* subF,PRE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     IPE d2d configuration

     a weak symbol for d2d operation. it will be called right before IPE d2d start, in order to get
     IPE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IPE configuration
     @param[out]    group:      IPH IPE update item

     @return Description of data returned.
         - @b return value is the repeat time of IPE d2d operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IPE_D2D_CB(IPE_SUBFUNC* subF,IPE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     IFE d2d configuration

     a weak symbol for d2d operation. it will be called right before IFE d2d start, in order to get
     IFE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IFE configuration
     @param[out]    group:      IPH IFE update item

     @return Description of data returned.
         - @b return value is the repeat time of IFE d2d operation.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IFE_D2D_CB(IFE_SUBFUNC* subF,IFE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     IFE2 d2d configuration

     a weak symbol for d2d operation. it will be called right before IFE2 d2d start, in order to get
     IFE2 configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IFE2 configuration
     @param[out]    group:      IPH IFE2 update item

     @return Description of data returned.
         - @b return value is the repeat time of IFE2 d2d operation.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IFE2_D2D_CB(IFE2_SUBFUNC* subF,IFE2_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     IME d2d configuration

     a weak symbol for d2d operation. it will be called right before IME d2d start, in order to get
     IME configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IME configuration
     @param[out]    group:      IPH IME update item

     @return Description of data returned.
         - @b return value is the repeat time of IME d2d operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IME_D2D_CB(IME_SUBFUNC* subF,IME_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));
/**
     RDE d2d configuration

     a weak symbol for d2d operation. it will be called right before RDE d2d start, in order to get
     RDE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       RDE configuration
     @param[out]    group:      IPH RDE update item

     @return Description of data returned.
         - @b return value is the repeat time of RDE d2d operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 RDE_D2D_CB(RDE_SUBFUNC* subF,RDE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     PRE configuration of pre2ime frame mode

     a weak symbol for pre2ime frame mode operation. it will be called right before frame mode start, in order to get
     PRE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       PRE configuration
     @param[out]    group:      IPH PRE update item

     @return Description of data returned.
         - @b return value is the repeat time of frame mode operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 PREIME_D2D_PRECB(PRE_SUBFUNC* subF,PRE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     IFE configuration of pre2ime frame mode

     a weak symbol for pre2ime frame mode operation. it will be called right before frame mode start, in order to get
     IFE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IFE configuration
     @param[out]    group:      IPH IFE update item

     @return Description of data returned.
         - @b return value is the repeat time of frame mode operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 PREIME_D2D_IFECB(IFE_SUBFUNC* subF,IFE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     DCE configuration of pre2ime frame mode

     pre2ime frame mode operation. it will be called right before frame mode start, in order to get
     DCE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    Ref_1x:     sensor output size
     @param[out]    Ref_1xCen   distortion center of sensor output
     @param[out]    subF:       dce configuration
     @param[out]    group:      IPH DCE update item

     @return Description of data returned.
         - @b return value is the repeat time of frame mode operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 PREIME_D2D_DCECB(USIZE *Ref_1x,Coordinate *Ref_1xCen,DCE_SUBFUNC* subF,DCE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     IPE configuration of pre2ime frame mode

     a weak symbol for pre2ime frame mode operation. it will be called right before frame mode start, in order to get
     IPE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IPE configuration
     @param[out]    group:      IPH IPE update item

     @return Description of data returned.
         - @b return value is the repeat time of frame mode operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 PREIME_D2D_IPECB(IPE_SUBFUNC* subF,IPE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     IME configuration of pre2ime frame mode

     a weak symbol for pre2ime frame mode operation. it will be called right before frame mode start, in order to get
     IME configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IME configuration
     @param[out]    group:      IPH IME update item

     @return Description of data returned.
         - @b return value is the repeat time of frame mode operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 PREIME_D2D_IMECB(IME_SUBFUNC* subF,IME_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));
//@}


/**
     start d2d operation.

     start d2d operation without wait operation finish.
     if previous d2d operation set not ready yet(under the same engine condition), current operation
     will wait until preview operation set done.
     @note
\n   this operation will trig multi-task protection("IPC_D2D_Start" and "IPC_D2D_End" are symmetrical.)

     @param[in] info         egnine configuration
     @param[in] d2dMode      supported d2d mode
     @return Description of data returned.
         - @b E_OK:    engine start.
         - @b E_SYS:   d2dMode not support.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPC_D2D_Start(IPLC_D2D_STRUCT* info,IPLC_D2D d2dMode);

/**
     wait d2d operation finish

     if d2d repeat more than one time, it will wait until last operation finish, and clear OS flag.

     @note:
\n     |--------|                         |--------|                            |--------|
\n     |  data  |   input addr            |        |   input addr               |  data  |   input addr
\n     |--------|                 1x      |--------|                    2x      |--------|
\n                                =>                                    =>
\n
\n     |--------|                         |--------|                            |--------|
\n     |        |   output addr           |  data  |   output addr              |        |   output addr
\n     |--------|                         |--------|                            |--------|
\n

     @param[in]     d2dMode         supported d2d mode
     @param[out]    DataLocation    return ready Data in InputAddr or OutputAddr
     @return Description of data returned.
         - @b E_OK:     engine data output ready
         - @b E_NOSPT:  opeartion error.

     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPC_WaitFinish(IPLC_D2D d2dMode,DataDest* DataLocation);

/**
     close d2d operation.

     close engine directly.

     @note
\n   this operation will trig multi-task protection("IPC_D2D_Start" and "IPC_D2D_End" are symmetrical.)
\n   if end operation being calling during engine still running, it will cause engine crash.

     @param[in] d2dMode      supported d2d mode
     @return Description of data returned.
         - @b E_OK:     engine closed
         - @b E_NOSPT:  opeartion error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPC_D2D_End(IPLC_D2D d2dMode);

/**
     wait ime frame end

     wait until ime frame end, and clear ime frame end.
     @note


     @param[in] bClrFlg:
\n   if bClrFlg == TRUE, clear frame end first , and then wait and clear ime frame end.
\n   if bClrFlg == FALSE, wait and clear ime frame end directly.

     @return Description of data returned.
         - @b E_OK:    frame end ready.
         - @b E_SYS:   engine is not running.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPC_WaitImeFMD(BOOL bClrFlg);
/**
     wait sie act start

     wait until sie act start, and clear sie act start.
     @note

     @param[in] id:       pipeline index
     @param[in] bClrFlg:
\n   if bClrFlg == TRUE, clear act start flag first , and then wait and clear act start flag.
\n   if bClrFlg == FALSE, wait and clear act start flag directly.

     @return Description of data returned.
         - @b E_OK:    act start flag ready.
         - @b E_SYS:   engine is not running.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPC_WaitSieActS(BOOL bClrFlg,IPL_PROC_ID id);
/**
     wait sie field end

     wait until sie field end, and clear sie field end.
     @note

     @param[in] id:       pipeline index
     @param[in] bClrFlg:
\n   if bClrFlg == TRUE, clear field end first , and then wait and clear sie field end.
\n   if bClrFlg == FALSE, wait and clear ime frame end directly.

     @return Description of data returned.
         - @b E_OK:    field end ready.
         - @b E_SYS:   engine is not running.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPC_WaitVD(BOOL bClrFlg,IPL_PROC_ID id);

/**
     open ipl ctrl task

     open ipl ctrl inner task, which support all api provided by ipl_ctrl module.
     @note :none

     @return :none
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void IPL_CTRL_OpenTsk(void);

/**
     close ipl ctrl task

     close ipl ctrl inner task.
     @note :none

     @return :none
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void IPL_CTRL_CloseTsk(void);

/**
    enable IPL_Ctrl module message

    print warning msg under pipeline in running status when enabled.
    @note :none

    @return :none
    Example: none
    @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void IPL_CTRL_MSG(BOOL bEnable);
extern void IPC_SetIMECropInfo(IPCL_IMEP3_CROP_INFO CropInfo);

extern IPCL_IMEP3_CROP_INFO IPC_GetIMECropInfo(void);

extern ER IPC_ChkVD(IPL_PROC_ID id, UINT32 delay_ms, UINT32 chk_cnt);
extern UINT32 IPC_GetSIECnt(IPL_PROC_ID id);
//@}
#endif
