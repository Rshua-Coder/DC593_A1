/**
    IPH module (SIE sub function type define)

    @file       IPL_Hal_SIE_Info.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_SIE_Info_H
#define _IPL_Hal_SIE_Info_H
#include "Type.h"
#include "IPL_Utility.h"
/** \addtogroup mILibIPH */
//@{
#define SIE_MaxParaSet  5

/**
    sie engine supported clk
*/
/*
typedef enum
{
    IPH_SIECLK0 = 50,
    ENUM_DUMMY4WORD(IPH_SIECLK)
}IPH_SIECLK;
*/

typedef enum _IPH_SIE_PXCLKSRC
{
    IPH_SIEPXCLK_OFF = 0,
    IPH_SIEPXCLK_PAD = 1,
    ENUM_DUMMY4WORD(IPH_SIE_PXCLKSRC),
}IPH_SIE_PXCLKSRC;

typedef enum _ECS_BitRatio
{
    ECS_D2F8    = 0,    ///<2 bits decimal, 8bits fraction
    ECS_D3F7    = 1,    ///<3 bits decimal, 7bits fraction
    ENUM_DUMMY4WORD(ECS_BitRatio)
}ECS_BitRatio;          ///< bit filed of ecs compesation gain

/**
    signal phase
*/
typedef enum _SIG_PHASE
{
    _RISING     = 0,  ///< rising edge trig
    _FALLING    = 1,  ///< falling edge trig
}SIG_PHASE;

/**
    sie out buffer number
*/
typedef enum _SIE_PPBNum
{
    SIE_BUF_1 = 0,  ///< -
    SIE_BUF_2 = 1,  ///< -
    ENUM_DUMMY4WORD(SIE_PPBNum)
}SIE_PPBNum;

/**
    normalization factor for Optical black
*/
typedef enum _OB_NormaFact
{
    MSB_8bit    = 0,    ///< output MSB 8-bit
    DIVIDE_2    = 1,    ///< -
    DIVIDE_4    = 2,    ///< -
    DIVIDE_8    = 3,    ///< -
    ENUM_DUMMY4WORD(OB_NormaFact)
}OB_NormaFact;

/**
    scaler lp weighting on bayer domain
*/
typedef struct _BS_QUALITY
{
    UINT32  LpfWeight_H;    ///< range: 0~100
    UINT32  LpfWeight_V;    ///< range: 0~100
    UINT32  Binning_H;      ///< -
    UINT32  Binning_V;      ///< -
}BS_QUALITY;

/**
    SIE channel 0 output selection
*/
typedef enum _CH0_SRC
{
    NonScaled_Img   = 0,    ///< output non-scaled image
    CH0_Scaled_Img  = 1,    ///< output scaled image(scaling factor: crop size/dma out size
    ENUM_DUMMY4WORD(CH0_SRC)
}CH0_SRC;

/**
    SIE channel 2 output selection
*/
typedef enum _CH2_SRC
{
    VACC            = 0,    ///< va accumulation
    CH2_Scaled_Img  = 1,    ///< scaled down image
    Smear_Line      = 2,    ///< smear debug info.
    ENUM_DUMMY4WORD(CH2_SRC)
}CH2_SRC;

/**
    SIE data bitdepth
*/
typedef IPL_RAW_BIT SIE_PackBus;

/**
    Bayer CFA
*/
typedef IPL_RAW_PIX SIE_CFA;

/**
    Defect concealment weighting enum
*/
typedef enum _DEF_WEIGHT
{
    _50_Percent  = 0,    ///< cross-channel weighting = 50% for defect concealment
    _25_Percent  = 1,    ///< cross-channel weighting = 25% for defect concealment
    _0_Percent   = 2,    ///< cross-channel weighting =  0% for defect concealment
    ENUM_DUMMY4WORD(DEF_WEIGHT)
}DEF_WEIGHT;

/**
    sie dma output
*/
typedef struct _SIE_Dma_Out
{
    UINT32  PPB0_Addr;      ///< -
    UINT32  PPB1_Addr;      ///< only work when PPBNum = SIE_BUF_2
    SIE_PackBus packBus;    ///< data bit depth
    UINT32  LineOffset;     ///< -
}SIE_Dma_Out;

/**
    SIE channel 1 output address
*/
typedef struct _SIE_CH1_Dma_Out
{
    UINT32 PPB0_Addr;       ///< -
    UINT32 PPB1_Addr;       ///< only work when PPBNum = SIE_BUF_2
}SIE_CH1_Dma_Out;

/**
    sie interrupt enable
*/
typedef enum _SIE_INTE_EN
{
    SIE__CLR                 = 0,
    SIE__INTE_EFLDEND        = 0x00000001,   ///< enable interrupt: Even field end
    SIE__INTE_OFLDEND        = 0x00000002,   ///< enable interrupt: Odd field end
    SIE__INTE_FLDEND         = 0x00000004,   ///< enable interrupt: Field end
    SIE__INTE_BP1            = 0x00000010,   ///< enable interrupt: Break point 1
    SIE__INTE_BP2            = 0x00000020,   ///< enable interrupt: Break point 2
    SIE__INTE_BP3            = 0x00000040,   ///< enable interrupt: Break point 3
    SIE__INTE_ACTST          = 0x00000080,   ///< enable interrupt: Active window start
    SIE__INTE_CRPST          = 0x00000100,   ///< enable interrupt: Crop window start
    SIE__INTE_FLSH_TRG       = 0x00000400,   ///< enable interrupt: Flash trigger from external pin
    SIE__INTE_MSH_TRG        = 0x00000800,   ///< enable interrupt: Mshutter trigger from external pin
    SIE__INTE_FLSH_END       = 0x00001000,   ///< enable interrupt: Flash end
    SIE__INTE_MSH_CLOSE_END  = 0x00002000,   ///< enable interrupt: Mshutter close end
    SIE__INTE_MSH_OPEN_END   = 0x00004000,   ///< enable interrupt: Mshutter open end
    SIE__INTE_DRAM_OUT0_END  = 0x00100000,   ///< enable interrupt: Dram output channel 0 end
    SIE__INTE_DRAM_OUT1_END  = 0x00200000,   ///< enable interrupt: Dram output channel 1 end
    SIE__INTE_DRAM_OUT2_END  = 0x00400000,   ///< enable interrupt: Dram output channel 2 end
    SIE__INTE_DPCWARN        = 0x01000000,   ///< enable interrupt: Defect Pixel/Column concealment warning,happened at continus bp.(more than 2 bp)
    SIE__INTE_VAEND          = 0x02000000,   ///< enable interrupt: VA output end
    SIE__INTE_RAWENC_OUTOVFL = 0x08000000,   ///< enable interrupt: Raw encoder output buffer full
    //SIE__INTE_DRAM_IN0_UDFL  = 0x00008000,   ///<                 : input ch0 underflow
    //SIE__INTE_DRAM_IN1_UDFL  = 0x00010000,   ///<                 : input ch1 underflow
    //SIE__INTE_DRAM_OUT0_OVFL = 0x00020000,   ///<                 : output ch0 overflow
    //SIE__INTE_DRAM_OUT1_OVFL = 0x00040000,   ///<                 : output ch1 overflow
    //SIE__INTE_DRAM_OUT2_OVFL = 0x00080000,   ///<                 : output ch2 overflow
    SIE__INTE_ALL            = 0xffffffff,   ///< enable interrupt: all
}SIE_INTE_EN;


/**
    SIE Dram output burst length
*/
typedef enum _SIE_BURST_LENG
{
    _BURST_32WORD   = 32,   ///< SIE Dram burst length  32 words
    _BURST_64WORD   = 64,   ///< SIE Dram burst length  64 words
    _BURST_96WORD   = 96,   ///< SIE Dram burst length  96 words
    _BURST_128WORD  = 128,  ///< SIE Dram burst length  128 words
    _BURST_16WORD   = 16,   ///< SIE Dram burst length  16 words
    _BURST_24WORD   = 24,   ///< SIE Dram burst length  24 words
    _BURST_48WORD   = 48,   ///< SIE Dram burst length  48 words
    ENUM_DUMMY4WORD(SIE_BURST_LENG)
}SIE_BURST_LENG;

/**
    sie signal receive mode
*/
typedef enum _SieACTMode
{
    Para_S  = 0,        ///< SIE acts as slave for parallel-IF sensor
    Para_M  = 1,        ///< SIE acts as master for parallel-IF sensor
    Serial  = 2,        ///< SIE/HighSpeedIf acts together for Serial-IF sensor
    PATGEN  = 3,        ///< SIE 2D INCREMENTAL fix pattern
    ENUM_DUMMY4WORD(SieACTMode)
}SieACTMode;

/**
    sie trig pre timming
*/
typedef enum _SIEPreTrigSel
{
    Cur_DRAM_E  = 0,    ///< Trigger PRE at dram end
    Cur_FLDD    = 1,    ///< Trigger PRE at field end
    Next_ACTST  = 2,    ///< Trigger PRE at next active-window start
    ENUM_DUMMY4WORD(SIEPreTrigSel)
}SIEPreTrigSel;


#define FieldCycle  2
#define NodePerLine 2
/**
    SIE input data format and signal
*/
typedef struct _IPP_SIE_SIGNAL
{
    enum
    {
        SIE_PROGRESSIVE_DIV = 0,    ///< -
        SIE_2_DIV       = 1,        ///< -
        SIE_3_DIV       = 2,        ///< -
        SIE_4_DIV       = 3,        ///< -
        SIE_5_DIV       = 4,        ///< -
        SIE_MAX_DIV     = 5,
        ENUM_DUMMY4WORD(SieDivNum)
    }SieDivNum;                     ///< division number

    struct
    {
        enum
        {
            PROGRESSIVE_FLD = 0,    ///< -
            _2_FLD          = 1,    ///< -
            _3_FLD          = 2,    ///< -
            _4_FLD          = 3,    ///< -
            _5_FLD          = 4,    ///< -
            _6_FLD          = 5,    ///< -
            _7_FLD          = 6,    ///< -
            _8_FLD          = 7,    ///< -
            _9_FLD          = 8,    ///< -
            _10_FLD         = 9,    ///< -
            _11_FLD         = 10,   ///< -
            _12_FLD         = 11,   ///< -
            _13_FLD         = 12,   ///< -
            _14_FLD         = 13,   ///< -
            _15_FLD         = 14,   ///< -
            _16_FLD         = 15,   ///< -
            MAX_FLD         = 16,
            ENUM_DUMMY4WORD(SieFldNum)
        }SieFldNum;                 ///< field number
        UINT8   FldLine[FieldCycle][MAX_FLD];   ///< field num of the 1st to 16th input field which is corresponding to each cycle
    }Sie_FLD;                       ///< field information

    struct
    {
        enum
        {
            _1_Line = 0,    ///< -
            _2_Line = 1,    ///< -
            _3_Line = 2,    ///< -
            _4_Line = 3,    ///< -
            Max_Line= 4,
            ENUM_DUMMY4WORD(SieLineNum)
        }SieLineNum;        ///< reorder fmt
        UINT32  StartPixelCnt;          ///< start pixel, start pixel idx at 0
        UINT8   PixelIdx[Max_Line][NodePerLine]; ///<each line map to two pixel idx
        BOOL    Reorder_VEn;            ///< vertical direction reorder enable/disable
    }Sie_ReOrder;                   ///< reorder information

    struct
    {
        SIG_PHASE   VD;     ///< -
        SIG_PHASE   HD;     ///< -
        SIG_PHASE   Data;   ///< -
    }SensorPhase;           ///< signal phase

    struct
    {
        UINT32  HD_sync;    ///< HD sync period
        UINT32  HD_total;   ///< HD total period
        UINT32  HD_offset;  ///< HD offset
        BOOL    HD_ini;     ///< HD inverse
        UINT32  VD_total;   ///< VD total period
        UINT32  VD_sync;    ///< VD sync period
        UINT32  VD_offset;  ///< VD offset
        BOOL    VD_ini;     ///< VD inverse
    }Sie_HDVD;              ///< HD/VD signal
}IPP_SIE_SIGNAL;

/**
    SIE active and crop window
*/
typedef struct _SIE_WINDOW
{
    struct
    {
        UINT16  DIV_X[SIE_MAX_DIV]; ///< x-direction active start coordinate of 1st,2nd ...,5th division
                                ///< if no div, only assign DIV_X[0],others assign 0
        UINT16  FLD_Y[MAX_FLD]; ///< y-direction active start coordinate of 1st,2nd ...,16th field
                                ///< if no fld, only assign FLD_Y[0],others assign 0
        UINT32  SizeH;      ///< active window size
        UINT32  SizeV;      ///< active window size
        SIE_CFA CFA;        ///< CFA pattern of active window 1st pixel
    }Act;                           ///< active window
    struct
    {
        Coordinate  StartPix;///<crop start coordinate
        UINT32  SizeH;      ///< crop window size
        UINT32  SizeV;      ///< crop window size
        SIE_CFA CFA;    ///< CFA pattern of crop window 1st pixel
    }Crop;                          ///< crop window
}SIE_WINDOW;

/**
    flash / m-shutter configuration
*/
typedef struct _Component_Para
{
    enum
    {
        high_act    =0,     ///< activated at high level
        low_act     =1,     ///< activated at low level
        ENUM_DUMMY4WORD(OutPolarity)
    }OutPolarity;           ///< component output
    SIG_PHASE   InPhase;    ///< component input
    enum
    {
        TRI_COUNT   = 0,    ///< count delay setting to trig when "SIE_FLASH_START | SIE_MSHUT_CLOSE"
        TRI_GPIO    = 1,    ///< trig by GPIO when "SIE_FLASH_START | SIE_MSHUT_CLOSE"
        TRI_IMD     = 2,    ///< trig immediately when "SIE_FLASH_START | SIE_MSHUT_CLOSE"
        ENUM_DUMMY4WORD(TrigMode)
    }TrigMode;
    UINT32  DelayLine;      ///< unit:line
    UINT32  Delay2Pixel;    ///< unit: 2 pixels
    UINT32  Width;          ///< unit:pix
}Component_Para;

/**
    flash configuration
*/
typedef Component_Para SIE_FLASH;

/**
     m-shutter configuration
*/
typedef struct _SIE_Mshtr
{
    enum
    {
        MSH_LEVEL   = 0,    ///< level mode
        MSH_PULSE   = 1,    ///< pulse mode
        ENUM_DUMMY4WORD(MshtrMode)
    }MshtrMode;             ///< msht trig mode
    BOOL    bPinSwap;   ///< swap pimux:mes0 and mes1
    Component_Para info;///< -
    UINT32 DelayFrame;  ///<1:next frame,0:cur frame
}SIE_Mshtr;


/**
     SIE sub fucntion enable
*/
typedef enum _SieFuncEn
{
    //SieEn       = 0x00000004,   ///< enable ACT_en
    RawEncEn    = 0x00000008,   ///< enable Raw encoder
    SpiTrigEn   = 0x00000020,   ///< enable SPI trigger
    //DviEn       = 0x00000040,   ///< enable DVI(CCIR)
    //ReorderHEn  = 0x00000100,   ///< enable Horizontal re-order
    //ReorderVEn  = 0x00000200,   ///< enable Vertical re-order
    DpcEn       = 0x00000400,   ///< enable Defect pixel concealment
    EMBdpcEn    = 0x00000800,   ///< enable Embeded defect pixel mark
    ColumnDefEn = 0x00001000,   ///< enable Column defect concealment
    SMREn       = 0x00002000,   ///< enable Smear detection and reduction
    OBAvgEn     = 0x00004000,   ///< enable OB average ( ob detection)
    OBSubSelEn  = 0x00008000,   ///< enable OB subtraction selection
    OBFrameAvgEn= 0x00010000,   ///< enable OB-Frame average
    OBPlaneSubEn= 0x00020000,   ///< enable OB-Plane subtraction
    PFPCEn      = 0x00040000,   ///< enable PFPC
    GammaEn     = 0x00080000,   ///< enable Gamma
    ECSEn       = 0x00100000,   ///< enable Color shading compensation
    DGainEn     = 0x00200000,   ///< enable Digital Gain
    ///SIE_BS_H_EN               0x00400000   ///< enable Horizontal bayer scaling
    ///SIE_BS_V_EN               0x00800000   ///< enable Vertical bayer scaling
    VIGEn       = 0x01000000,   ///< enable vignette shading compensation(for CA)
    CACCEn      = 0x02000000,   ///< enable Color accumulation
    VACCEn      = 0x04000000,   ///< enable Variation accumulation
    HFlipEn     = 0x10000000,   ///< enable Horizontal flip
    VFlipEn     = 0x20000000,   ///< enable Vertical flip
    DMAOutCh0En = 0x40000000,   ///< enable Dram output channel 0
    DMAOutCh2En = 0x80000000,   ///< enable Dram output channel 2
}SieFuncEn;

/**
     data structure for OB
*/
typedef struct _OB_Para
{
    UINT32 offset;  ///< -
    UINT32 Gain;    ///< -
}OB_Para;


/**
    Defect Pixel cocealment
*/
typedef struct _SIE_DPC
{
    UINT32      DP_TblAddr;         ///< Ch0 input
    UINT32      TblSize;            ///< -
    DEF_WEIGHT  Weight;             ///< -
}SIE_DPC;


//sie dpc table size macro,excluding end string,0xFFFF. (unit:byte)
#define SIE_DPC_TBLSIZE(pixcnt,linecnt) ( ((pixcnt * 2) + (linecnt * 2) ) )

/**
   Enbedded Decfect Pixel
*/
typedef struct _SIE_EMDPC
{
    UINT32 DP_TblAddr;              ///< Ch0 input
    UINT32 TblSize;                 ///< -
    UINT32 MarkValue;               ///< marked spot is the defected pixel
}SIE_EMDPC;

/**
    Defect Column concealment
*/
typedef struct _SIE_ColumnDef
{
    UINT32  ColumnNum;      ///< range: 0~32
    DEF_WEIGHT Weight;      ///< -
    UINT32  ColumnCoor_Y_TblAddr;   ///< Column Y coordinate tbl addr, tab num:32, Data type: UINT16
}SIE_ColumnDef;

/**
    OB window average detection
*/
typedef struct _SIE_OBDET
{
    Coordinate  StartPix; ///< window start pixel coordinate
    UINT32  SizeH;      ///< window horizontal size
    UINT32  SizeV;      ///< window vertical size
    UINT32  SubRatio_X; ///< window horizontal subsample
    UINT32  AvgActThres;    ///< threshold for ob avg activated counter
    UINT32  SmrActThres;    ///< threshold for smr activated counter
}SIE_OBDET;

/**
    OB window average detection
*/
typedef SIE_OBDET   SIE_OBAVG;

/**
    OB substration
*/
typedef struct _SIE_OBSUB
{
    UINT16  Offset; ///< -
    UINT16  DtGain; ///< -
}SIE_OBSUB;

/**
    OB Frame calculation
    each window divide into 8 tabs(each tab length is "Interval")
*/
typedef struct _SIE_OBFAVG
{
    struct
    {
        Coordinate  StartPix;   ///< -
        UINT32      Interval;   ///< -
    }TopWindow;                 ///< -
    struct
    {
        Coordinate  StartPix;   ///< -
        UINT32      Interval;   ///< -
    }LeftWindow;                ///< -
    struct
    {
        Coordinate  StartPix;   ///< -
    }RightWindow;               ///< -
    struct
    {
        Coordinate  StartPix;   ///< -
    }BottomWindow;              ///< -
    OB_NormaFact OBF_NormFact;  ///< -

    UINT32 Thres;   ///< threshold for 4 windows
}SIE_OBFAVG;

/**
    OB Plane
    ob plane and scaling factor will be calcaulated automatically
*/
typedef struct _SIE_OBPSUB
{
    //UINT32 TableAddr;  ///< tab num: 289 (17x17), Data type: UINT8
    //UINT32  Scale_H;///< Horizontal scaling factor
    //UINT32  Scale_V;///< Vertical scaling factor
    OB_NormaFact OBP_NormaFact; ///< -
}SIE_OBPSUB;

/**
    Smear Detection and Reduction
*/
typedef struct _SIE_SMR
{
    struct
    {
        UINT32  MoveStep1;  ///< -
        UINT32  MoveStep2;  ///< -
        UINT32  Thres1;     ///< -
        UINT32  Thres2;     ///< -
    }TemproalFilt;          ///< temporal filter
    struct
    {
        UINT32  MaxOut; ///< curve filter max output
        enum
        {
            _CF_15  = 0,    ///< -
            _CF_13  = 1,    ///< -
            _CF_11  = 2,    ///< -
            _CF_9   = 3,    ///< -
            _CF_7   = 4,    ///< -
            _CF_5   = 5,    ///< -
            _CF_3   = 6,    ///< -
            _CF_1   = 7,    ///< (no filtering)
            ENUM_DUMMY4WORD(SMRCfiltSize)
        }SMRCfiltSize;      //< filter size
        UINT32  Thres1;     ///< -
        UINT32  Thres2;     ///< -
    }CurveFilt;
    struct
    {
        UINT32  OBvalue;    ///< -
        UINT32  Thres1;     ///< -
        UINT32  Thres2;     ///< -
    }StaticLv;              ///< static level
    struct
    {
        UINT32  InThres;        ///< safe zone in threshold
        UINT32  OutThres;       ///< safe zone out threshold
        enum
        {
            W_0 = 0,            ///< no transition
            W_2 = 1,            ///< 2 pixel
            W_4 = 2,            ///< 4 pixel
            W_8 = 3,            ///< 8 pixel
            ENUM_DUMMY4WORD(TransW)
        }TransW;                ///< transition width
        enum
        {
            NodeS_1 = 0,    ///< no down-sample
            NodeS_2 = 1,    ///< 1/2 down-sample
            NodeS_4 = 2,    ///< 1/4 down-sample
        }Node_Sub;          ///< (node interval)
    }SafeZone;
    UINT32  SmrY;               ///< smear line vetical position
    UINT32  SmrSel_MultiLine;   ///< smear line selection for multi-line mode
    UINT32  OutlTh;             ///< outlier removal threshold
    UINT32  SmrRduWeight;       ///< smear reduction weight (0x80 as 100%)
    UINT32  SmrRduSrc_Max;      ///< source value which are over this threshold are remained
    /*
    struct
    {
        BOOL    DmaInEn;            ///< Input smear line from DRAM, mutually exclusive with ECS
        UINT32  InAddr;             ///< ch1
        SIE_PackBus InPack;
    }DmaIn;                         ///< for smear line(debug only)
    */
}SIE_SMR;

/**
    SIE Periodic fix pattern
*/
typedef struct _SIE_PFP
{
    UINT32   Gain_TblAddr;  ///<tab num:32, Data type:UINT8
    UINT32   Ofs_TblAddr;   ///<tab num:32, Data type:UINT8
    enum
    {
        Mode_4x8    = 0,    ///< 4x8 period
        Mode_8x4    = 1,    ///< 8x4 period
        ENUM_DUMMY4WORD(PFPC_MODE)
    }PFPC_MODE;             ///< -
}SIE_PFP;

/**
    SIE color shading supported table size
*/
typedef enum _ECSMAP_ENUM
{
    ecs129x129 = 0, ///< 129x129
    ecs65x65 = 1,   ///< 65x65
    ecs49x49 = 2,   ///< 49x49
    ENUM_DUMMY4WORD(ECSMAP_ENUM)
}ECSMAP_ENUM;

#define ECS_MAP_SIZE(_enum,pSize) \
{\
    if(_enum == ecs129x129)\
        *pSize = 129;\
    else if(_enum == ecs65x65)\
        *pSize = 65;\
    else if(_enum == ecs49x49)\
        *pSize = 49;\
}

/**
     Sie Embedded Color shading
     @note (Optional)
*/
typedef struct _SIE_EmbeddedCS
{
    ECS_BitRatio    BitSel;     ///< -
    BOOL    DthrEn;             ///< dithering enable
    BOOL    DthrRst;            ///< dithering reset
    enum
    {
        _BIT_0_1    = 0,    ///< add dithering at bit[0:1]
        _BIT_1_2    = 1,    ///< add dithering at bit[1:2]
        _BIT_2_3    = 2,    ///< -
        _BIT_3_4    = 3,    ///< -
        _BIT_4_5    = 4,    ///< -
        _BIT_5_6    = 5,    ///< -
        _BIT_6_7    = 6,    ///< -
        _BIT_7_8    = 7,    ///< -
        ENUM_DUMMY4WORD(ECSDthrLv)
    }ECSDthrLv;                 ///< dithering level
}SIE_EmbeddedCS;

/**
    SIE VIG (for CA)
*/
typedef struct _SIE_CA_VIG
{
    Coordinate  Center;         ///< center
    UINT32      Mul;            ///< distance gain. unsigned 2.6
    UINT32      T;              ///< disabled area
    UINT32      TabGain;        ///< gain factor
    BOOL        bDither;        ///< dithering enable/disable
    UINT32      XDIV;      ///< scaling factor in x direction. unsigned 0.6
    UINT32      YDIV;      ///< scaling factor in y direction. unsigned 0.6
    UINT32      Dist_Y;         ///< vertical consecutive line distance (for multi-field)
    UINT32      LUT_TblAddr;    ///< vignette LUT .(tab num:17,  Data type: UINT8)
}SIE_CA_VIG;

/**
    accumulation window
*/
typedef struct _ACC_WIN
{
    Coordinate  Start;  ///< Hor & Ver window start position
    UINT32  SizeH;   ///< window horizontal size
    UINT32  SizeV;   ///< window vertical size
    UINT32  WinNum_X;   ///< horizontal window number
    UINT32  WinNum_Y;   ///< vertical window number
}ACC_WIN;

/**
    SIE color accumulation
*/
typedef struct _SIE_CACC
{
    enum
    {
        _32BIT   = 0,    ///< 32bit per win, [11,10,11]
        _64BIT   = 1,    ///< 64bit per win, [16,16,16]
        ENUM_DUMMY4WORD(CAPack)
    }CAPack;            ///< CA bitdepth
    ACC_WIN CA_WIN;     ///< accumulation window
}SIE_CACC;

/**
    SIE Variation accumulation
*/
typedef struct _SIE_VACC
{
    ACC_WIN VA_WIN;     ///< accumulation window
    UINT32  Thres;      ///< -
    enum
    {
        VA_1    = 0,        ///< [-1 0 2 0 -1]
        VA_2    = 1,        ///< [-1 0 0 2 0 0 -1]
        ENUM_DUMMY4WORD(VAFilt)
    }VAFilt;                ///< filter selection
}SIE_VACC;

/**
    SIE RAW Encoder
*/
typedef struct _SIE_RAWENC
{
    UINT32  CompRate;       ///< range: 1~100
    UINT32  CompMaxSize;    ///< max output size,unit: byte
}SIE_RAWENC;


/**
    Sie bayer scaling Quality,
    Horizontal is workable when crop_H != DmaOutH
    Vertical is workable when crop_V != DmaOutV
*/
typedef struct _SIE_SCAL_PAR
{
    BOOL    bRst;           ///< TRUE if need to reset lp weighting
    BS_QUALITY  ScalingQuality; ///< -
}SIE_SCAL_PAR;

/**
    SIE 4-ch gamma curve
*/
typedef struct _SIE_GAMMA
{
    UINT32  R_TblAddr;          ///< tab number:129, Data type: 16bit
    UINT32  Gr_TblAddr;         ///< -
    UINT32  Gb_TblAddr;         ///< -
    UINT32  B_TblAddr;          ///< -
}SIE_GAMMA;

/**
    digital gain
*/
typedef struct _SIE__DGAIN
{
    UINT32  DigitalGain;        ///< -
}SIE__DGAIN;

/**
    SIE breakpoint/inverse
*/
typedef struct _SIE_BP
{
    UINT32  BP1;        ///< break point1 y-coordinate
    UINT32  BP2;        ///< -
    UINT32  BP3;        ///< -
    BOOL    BP1_ref;    ///< true for reference offset signal,flase for reference non-offset signal
    BOOL    BP2_ref;    ///< -
    BOOL    BP3_ref;    ///< -
}SIE_BP;

/**
    channel information
*/
typedef struct _SIE_CH0PARAM
{
    UINT32 RdyAddr;         ///< ch0 ready output address
    UINT32 RdyIdx;          ///< ch0 ready output address index
}SIE_CH0PARAM;
/**
    color accumulation information
*/
typedef struct _SIE_CAPARAM
{
    UINT32 Addr;         ///< output address
    UINT32 WinNumH;      ///< ca window number(hor.)
    UINT32 WinNumV;      ///< ca window number(ver.)
    UINT32 WinSizeH;     ///< ca window size(hor.)
    UINT32 WinSizeV;     ///< ca window size(ver.)
    UINT32 NormFactor;   ///< ca normal factor
    UINT32 CAOutMode;    ///< ca outmode
}SIE_CAPARAM;

/**
    color accumulation result
*/
typedef struct _SIE_CARST
{
    UINT16* pR;         ///< output: assign R acc result to this addr
    UINT16* pG;         ///< output: assign G acc result to this addr
    UINT16* pB;         ///< output: assign B acc result to this addr
    UINT32  Length;     ///< input:  window num, ex: 8x8->64.   16x16->256
    SIE_CAPARAM Param;  ///< input:  window information
}SIE_CARST;

/**
    variation accumulation result
*/
typedef struct _SIE_VARST
{
    UINT16* pVA;        ///< table pointer
    UINT16  Length;     ///< table size
}SIE_VARST;

/**
     SIE sub function configuration.
*/
typedef struct _SIE_SUBFUNC
{
    SIE_INTE_EN InteEn;     ///< interrupt enable
    SieFuncEn   sieFuncEn;  ///< function enable

    SIE_BP      BP;         ///< break point configuration

    SIE_DPC     Dpc;        ///< defect pixel configuration
    SIE_EMDPC   EMDpc;      ///< embeded defect pixel configuration
    SIE_ColumnDef   ColumnDef;  ///< column defect concealment configuration
    SIE_OBDET   OBAvg;          ///< OB average dectection configuration
    SIE_OBSUB   OBSub;          ///< OB substration configuration
    SIE_OBFAVG  OBFAvg;         ///< OB frame configuration
    SIE_OBPSUB  OBPSub;         ///< OB plane configuration


    SIE__DGAIN  DGain;      ///< digital gain in 3.7 bits format
    SIE_CACC    Ca;         ///< color acc. configuration
    SIE_VACC    Va;         ///< variation acc. configuration
    SIE_RAWENC  RawEnc;     ///< raw encode configuration
    SIE_SCAL_PAR *pScale;  ///< bayer scaler configuration

    SIE_GAMMA   *pGamma;    ///< gamma table
    struct
    {
        UINT32  Inaddr;         ///< ecs table input addr(ch1)
        ECSMAP_ENUM     EcsMap; ///< ecs table size
        SIE_EmbeddedCS *pecs;   ///<
    }Ecs;                   ///< embed color shading configuration
    SIE_CA_VIG  *pVig;      ///< CACC vignette configuration
    SIE_SMR     *pSmr;      ///< smear configuration
    SIE_PFP     *pPfpc;     ///< periodic fix pattern configuration

    SIE_FLASH   *pFlash;    ///< flash light configuration
    SIE_Mshtr   *pMshtr;    ///< mechanical shutter configuration
}SIE_SUBFUNC;
//@}
#endif