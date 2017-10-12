#ifndef IPL_BUF_AR0330P_EVB_FF_H_
#define IPL_BUF_AR0330P_EVB_FF_H_

/**
    IPLBUF_MODE
    Mode selection for IPP-Buffer-Allocation
    Add ones if necessary
*/
typedef enum {
    IPLBUF_MODE_FLOW_A = 0,     ///< for Preview/Live-view mode
    IPLBUF_MODE_FLOW_B,         ///< for WHDR mode
    IPLBUF_MODE_FLOW_C,         ///< for FW Trig mode
    IPLBUF_MODE_CAP,            ///< for capture mode(only working buffer)
    IPLBUF_MODE_MAX_CNT,
    ENUM_DUMMY4WORD(IPLBUF_MODE)
} IPLBUF_MODE;


/**
    IPLBUF_PRV_IDX for IPLBUF_MODE_PRV
    Manage all the buffers for Image-Pipeline
    Add ones if necessary
*/
typedef enum
{
    IPLBUF_SIERAW_CH0_1 = 0,    ///< for SIE output raw image
    IPLBUF_SIERAW_CH0_2,        ///< for SIE output raw image
    IPLBUF_SIERAW_CH0_3,        ///< for SIE output raw image
    IPLBUF_SIERAW_CH0_MAX,

    IPLBUF_SIERAW_CH2_1,        ///< for SIE output raw image
    IPLBUF_SIERAW_CH2_2,        ///< for SIE output raw image
    IPLBUF_SIERAW_CH2_MAX,

    IPLBUF_SIEVA_1,         ///< for SIE output VA
    IPLBUF_SIEVA_2,         ///< for SIE output VA
    IPLBUF_SIEVA_MAX,

    IPLBUF_SIECA_1,         ///< for SIE output CA
    IPLBUF_SIECA_2,         ///< for SIE output CA
    IPLBUF_SIECA_MAX,

    IPLBUF_IPEOUT_1,         ///< for IPE output
    IPLBUF_IPEOUT_2,         ///< for IPE output
    IPLBUF_IPEOUT_MAX,

    IPLBUF_IPEVA_1,         ///< for IPE output VA
    IPLBUF_IPEVA_2,         ///< for IPE output VA
    IPLBUF_IPEVA_MAX,

    IPLBUF_IPEETH_1,        ///< for IPE output ETH
    IPLBUF_IPEETH_2,        ///< for IPE output ETH
    IPLBUF_IPEETH_3,        ///< for IPE output ETH
    IPLBUF_IPEETH_MAX,

    IPLBUF_IPELCA_1,        ///< for IPE output LCA
    IPLBUF_IPELCA_2,        ///< for IPE output LCA
    IPLBUF_IPELCA_3,        ///< for IPE output LCA
    IPLBUF_IPELCA_4,        ///< for IPE output LCA
    IPLBUF_IPELCA_MAX,

    IPLBUF_IPEV_1,        ///< for IPE output V
    IPLBUF_IPEV_2,        ///< for IPE output V
    IPLBUF_IPEV_3,        ///< for IPE output V
    IPLBUF_IPEV_4,        ///< for IPE output V
    IPLBUF_IPEV_MAX,

    IPLBUF_IPEIFE2LCA_1,   ///< for IFE2 D2D
    IPLBUF_IPEIFE2LCA_2,   ///< for IFE2 D2D
    IPLBUF_IPEIFE2LCA_MAX,

    IPLBUF_IMEP1_1,         ///< for IME YCC image output pingpong 1
    IPLBUF_IMEP1_2,         ///< for IME YCC image output pingpong 2
    IPLBUF_IMEP1_3,         ///< for IME YCC image output pingpong 3
    IPLBUF_IMEP1_4,         ///< for IME YCC image output pingpong 4
    IPLBUF_IMEP1_5,         ///< for IME YCC image output pingpong 5
    IPLBUF_IMEP1_6,         ///< for IME YCC image output pingpong 6
    IPLBUF_IMEP1_7,         ///< for IME YCC image output pingpong 7
    IPLBUF_IMEP1_8,         ///< for IME YCC image output pingpong 8
    IPLBUF_IMEP1_9,         ///< for IME YCC image output pingpong 9
    IPLBUF_IMEP1_10,        ///< for IME YCC image output pingpong 10
    IPLBUF_IMEP1_MAX,

    IPLBUF_IMEP2_1,         ///< for IME YCC image output pingpong 1
    IPLBUF_IMEP2_2,         ///< for IME YCC image output pingpong 2
    IPLBUF_IMEP2_3,         ///< for IME YCC image output pingpong 3
    IPLBUF_IMEP2_4,         ///< for IME YCC image output pingpong 4
    IPLBUF_IMEP2_5,         ///< for IME YCC image output pingpong 5
    IPLBUF_IMEP2_6,         ///< for IME YCC image output pingpong 6
    IPLBUF_IMEP2_7,         ///< for IME YCC image output pingpong 7
    IPLBUF_IMEP2_8,         ///< for IME YCC image output pingpong 8
    IPLBUF_IMEP2_9,         ///< for IME YCC image output pingpong 9
    IPLBUF_IMEP2_10,        ///< for IME YCC image output pingpong 10
    IPLBUF_IMEP2_MAX,

    IPLBUF_IMEP3_1,         ///< for IME YCC image output pingpong 1
    IPLBUF_IMEP3_2,         ///< for IME YCC image output pingpong 2
    IPLBUF_IMEP3_3,         ///< for IME YCC image output pingpong 3
    IPLBUF_IMEP3_4,         ///< for IME YCC image output pingpong 4
    IPLBUF_IMEP3_5,         ///< for IME YCC image output pingpong 5
    IPLBUF_IMEP3_6,         ///< for IME YCC image output pingpong 6
    IPLBUF_IMEP3_7,         ///< for IME YCC image output pingpong 7
    IPLBUF_IMEP3_8,         ///< for IME YCC image output pingpong 8
    IPLBUF_IMEP3_9,         ///< for IME YCC image output pingpong 9
    IPLBUF_IMEP3_10,        ///< for IME YCC image output pingpong 10
    IPLBUF_IMEP3_MAX,

    IPLBUF_IMGEFFECT_WDR,   ///< for image effect
    IPLBUF_STAMP,           ///< for stamp
    IPLBUF_ALBUM,           ///< for Album mode
    IPLBUF_PHOTOFRAME,      ///< for Photo frame

    IPLBUF_PRV_MAX_CNT,
    ENUM_DUMMY4WORD(IPLBUF_PRV_IDX)
} IPLBUF_PRV_IDX;

//check IPLBUF_PRV_IDX range
STATIC_ASSERT((IPLBUF_PRV_MAX_CNT <= IPL_BUF_ELEMENT_MAX));

/**
     IPLBUF Assigner result

     Detailed description.
     @note (Optional)
*/
typedef struct _IPPBUF_RST
{
    ER     er_code;         ///< Description
    UINT32 BufSize;         ///< required buffer size
}IPLBUF_RST,*PIPLBUF_RST;

typedef struct
{
    UINT32 BufNum;
    UINT32 Width;
    UINT32 Height;
    UINT32 LineOfs;
    UINT32 Bit;
} IPLBUF_RAWINFO;

typedef struct
{
    UINT32 BufNum;
    UINT32 YWidth;
    UINT32 YHeight;
    UINT32 YLineOfs;
    IPL_YUV_IMG_TYPE Fmt;
} IPLBUF_YINFO;

typedef struct
{
    IPL_PROC_ID Id;

    BOOL SIECh0En;
    IPLBUF_RAWINFO SIEOutCh0;

    BOOL SIECAEn;
    UINT32 SIECABufNum;
    UINT32 SIECABufSize;

    BOOL SIECh2En;
    IPLBUF_RAWINFO SIEOutCh2;

    BOOL SIEVAEn;
    UINT32 SIEVABufNum;
    UINT32 SIEVASize;

    BOOL IPELCAEn;
    UINT32 IPELCABufNum;
    UINT32 IPELCABufSize;

    BOOL IPEOutEn;
    IPLBUF_YINFO IPEOut;

    BOOL IPEVEn;
    UINT32 IPEVBufNum;
    UINT32 IPEVBufSize;

    BOOL IPEVAEn;
    UINT32 IPEVABufNum;
    UINT32 IPEVASize;

    BOOL IPEEthEn;
    UINT32 IPEEthSize;

    BOOL IPEIFE2LCAEn;
    UINT32 IPEIFE2LCABufNum;
    UINT32 IPEIFE2LCABufSize;

    BOOL IMEPath1En;
    IPLBUF_YINFO IMEPath1;

    BOOL IMEPath2En;
    IPLBUF_YINFO IMEPath2;

    BOOL IMEPath3En;
    IPLBUF_YINFO IMEPath3;

    BOOL IEffectWDREn;
    UINT32 IEffectWDRBufSize;

    BOOL StampEn;
    UINT32 StampBufSize;

    BOOL AlbumEn;
    UINT32 AlbumBufSize;

    BOOL PhotoFrameEn;
    UINT32 PhotoFrameBufSize;

}IPLBUF_ENG2DRAMINFO;

typedef IPLBUF_RST (*FPIPPBUFASSIGNER)(IPLBUF_ENG2DRAMINFO* Data);


extern IPLBUF_RST IPL_BufAssign(IPLBUF_MODE ModeIdx, IPLBUF_ENG2DRAMINFO *Data);

#endif //IPL_BUF_AR0330P_EVB_FF_H_
