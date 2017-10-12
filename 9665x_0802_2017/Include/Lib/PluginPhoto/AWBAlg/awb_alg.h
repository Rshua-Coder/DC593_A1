/**
    Auto White Balance Algorithm.

    depend on current image information to calculate white balance gain

    @file       Awb_Alg.h
    @ingroup    mILibAWBAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AWB_ALG_H_
#define _AWB_ALG_H_
#ifdef WIN32
#define ENUM_DUMMY4WORD(a)
#endif

#define AWB_FUNCCTRL_FACEMATCH_EN     0x00000001    //Enable to match the face color,

#define AWB_FUNCCTRL_R1_PPROC_EN      0x10000001    //Enable high\low color temperature postprocess
#define AWB_FUNCCTRL_R1_CTTAB_EN      0x10000002    //Enable color temperature table
#define AWB_FUNCCTRL_R1_STABLE_EN     0x10000004    //Enable gain stable

#define AWB_FUNCCTRL_R2_PPROC_EN      0x20000001    //Enable high\low color temperature postprocess
#define AWB_FUNCCTRL_R2_CTTAB_EN      0x20000002    //Enable color temperature table
#define AWB_FUNCCTRL_R2_STABLE_EN     0x20000004    //Enable gain stable
#define AWB_FUNCCTRL_R2_MULTICT_EN    0x20000008    //Enable to check multi color temperature
#define AWB_FUNCCTRL_R2_REMAP_EN      0x20000010    //Enable to remap to check white block

#define AWB_ACC_BIT         12   //Define CA ACC Bit Cnt

/**
     @name AWB Alg. default value
     @note
*/


typedef struct _AWB_CT_MAPPING
{
    UINT32 CT_MAX_MAPPING;
    UINT32 CT_7500K_MAPPING;
    UINT32 CT_6000K_MAPPING;
    UINT32 CT_5000K_MAPPING;
    UINT32 CT_4000K_MAPPING;
    UINT32 CT_3000K_MAPPING;
    UINT32 CT_MIN_MAPPING;
}AWB_CT_MAPPING;

typedef struct _AWB_CT_TABLE
{
    AWB_CT_MAPPING CT;
    UINT32 CT_7500K;
    UINT32 CT_6000K;
    UINT32 CT_5000K;
    UINT32 CT_4000K;
    UINT32 CT_3000K;

}AWB_CT_TABLE;

typedef enum _AWBALG_MODE
{
    AWBALG_MODE_NVTALG_R1=0,
    AWBALG_MODE_NVTALG_R2,
    AWBALG_MODE_GRAYWORLD,
    ENUM_DUMMY4WORD(AWBALG_MODE)
}AWBALG_MODE;
#define AWBALG_MODE_NVTALG AWBALG_MODE_NVTALG_R1 //define for old version

/**
     @name AWB Alg. default value
     @note
*/
typedef enum _AWBALG_CH
{
    AWBALG_CH_R = 0,
    AWBALG_CH_G,
    AWBALG_CH_B,
    AWBALG_CH_MAX,
    ENUM_DUMMY4WORD(AWBALG_CH)
}AWBALG_CH;

#define AWBALG_CH_RGAIN AWBALG_CH_R
#define AWBALG_CH_BGAIN AWBALG_CH_B
#define AWBALG_CH_CT    AWBALG_CH_G


/**
     check type.

     @note
*/
typedef enum _AWBALG_TYPE
{
    AWBALG_TYPE_Y = 0,
    AWBALG_TYPE_RG,
    AWBALG_TYPE_BG,
    AWBALG_TYPE_RBG,
    AWBALG_TYPE_SAT,
    AWBALG_TYPE_RBGSUM,
    AWBALG_TYPE_RXBG,
    AWBALG_TYPE_RBGDIF,
    AWBALG_TYPE_MAX,
    AWBALG_TAB_END = 0xffffffff,
    ENUM_DUMMY4WORD(AWBALG_TYPE)
}AWBALG_TYPE;


/**
     check type error.

     @note
*/
/*
typedef enum _AWBALG_ER
{
    AWBALG_FAIL = 0,
    AWBALG_OK = 1,
    AWBALG_PAR = 2,
    AWBALG_GAIN_ADJ = 3,
    AWBALG_PDLIGHT = 4,
    AWBALG_Y = 100,
    AWBALG_RG = 110,
    AWBALG_BG = 120,
    AWBALG_RBG = 130,
    AWBALG_SAT = 140,
    AWBALG_RBGSUM = 150,
    AWBALG_RXBG = 160,
    AWBALG_RBGDIF = 170,
    AWBALG_USER = 180,
    AWBALG_OUTL = 190,
    AWBALG_GREEN = 200,
    AWBALG_MAX,
    ENUM_DUMMY4WORD(AWBALG_ER)
}AWBALG_ER;
*/
typedef enum _AWBALG_ER
{
    AWBALG_FAIL = 0,
    AWBALG_OK = 1,
    AWBALG_PAR = 2,
    AWBALG_GAIN_ADJ = 3,
    AWBALG_PDLIGHT = 4,
    AWBALG_Y_L = 100,
    AWBALG_Y_H = 105,
    AWBALG_RG_L = 110,
    AWBALG_RG_H = 115,
    AWBALG_BG_L = 120,
    AWBALG_BG_H = 125,
    AWBALG_RBG_L = 130,
    AWBALG_RBG_H = 135,
    AWBALG_SAT = 140,
    AWBALG_RBGSUM_L = 150,
    AWBALG_RBGSUM_H = 155,
    AWBALG_RXBG_L = 160,
    AWBALG_RXBG_H = 165,
    AWBALG_RBGDIF = 170,
    AWBALG_USER = 180,
    AWBALG_OUTL = 190,
    AWBALG_GREEN = 200,
    AWBALG_MAX,
    ENUM_DUMMY4WORD(AWBALG_ER)
}AWBALG_ER;


/**
     AWB element.

     @note
*/
typedef struct _AWBALG_ELEMET
{
    AWBALG_TYPE Type;
    UINT32 ValH;
    UINT32 ValL;
    UINT32 ValHR[2];
    UINT32 ValLR[2];
}AWBALG_ELEMET;

/**
     AWB Path.

     @note
*/
typedef enum _AWBALG_ENV
{
    AWBALG_ENV_Night = 1,
    AWBALG_ENV_InDoor = 2,
    AWBALG_ENV_OutDoor = 3,
    AWBALG_ENV_Other_A = 4,
    AWBALG_ENV_Other_B = 5,
    AWBALG_ENV_NULL = 6,
    AWBALG_ENV_MAX,
    ENUM_DUMMY4WORD(AWBALG_ENV)
}AWBALG_ENV;


/**
     AWB Table.

     @note
*/
typedef struct _AWBALG_TAB
{
    UINT32 EvH;
    UINT32 EvL;
    AWBALG_ELEMET *Element;
    AWBALG_ENV Env;
}AWBALG_TAB;


/**
     AWB PreDefined Light Table.
     @note
*/
typedef struct _AWBALG_PDLTAB
{
    UINT32 RGain;
    UINT32 BGain;
    UINT32 Range;
}AWBALG_PDLTAB;

typedef UINT32 (*USER_CHK_WHITE)(UINT32 R, UINT32 G, UINT32 B);
typedef UINT32 (*USER_ADJ_GAIN)(UINT32 *nRgain, UINT32 *nBgain);

/**
     AWB algorithm event.
     @note
*/
typedef struct _AWBALG_EVENT_TAB
{
    //AWBALG_ChkWhite event
    USER_CHK_WHITE UserChkWhite;    ///< SUCCESS/FAIL return AWBALG_OK/AWBALG_USER
    USER_ADJ_GAIN  UserAdjGain;

}AWBALG_EVENT_TAB;

/**
     AWB CT Table
     @note
*/
typedef struct _AWBALG_GAIN
{
    UINT32 RGain;
    UINT32 BGain;
}AWBALG_GAIN;


typedef struct
{
    UINT16 R;
    UINT16 G;
    UINT16 B;
} AWBALG_RGB_INFO;

typedef struct _AWBALG_REMAP_INFO
{
    UINT32 CT_VectorAngle;
    UINT32 HCT_RotateAngle;
    UINT32 LCT_RotateAngle;

    AWBALG_RGB_INFO HCTInfo;
    AWBALG_RGB_INFO MCTInfo;
    AWBALG_RGB_INFO LCTInfo;
} AWBALG_REMAP_INFO;

typedef struct _AWBALG_COLORBLOCK
{
    UINT16  x;               ///< result start x
    UINT16  y;               ///< result start y
    UINT16  w;               ///< result width
    UINT16  h;               ///< result height
} AWBALG_COLORBLOCK, *P_AWBALG_COLORBLOCK;

typedef struct _AWBALG_STABLE_GAIN
{
    UINT32 RGain;
    UINT32 BGain;
    UINT32 Score;
    UINT32 Tolerance;   ///< 256 is 100%
}AWBALG_STABLE_GAIN;

/**
     AWB CT Parameter
     @note
*/
typedef struct _AWBALG_CT_PARAM
{
    UINT32 HCTMaxEV;
    UINT32 HCTMinEV;
    AWBALG_GAIN HCTMaxGain;
    AWBALG_GAIN HCTMinGain;
    UINT32 LCTMaxEV;
    UINT32 LCTMinEV;
    AWBALG_GAIN LCTMaxGain;
    AWBALG_GAIN LCTMinGain;
}AWBALG_CT_PARAM;

/**
     AWB Postprocess
     @note
*/
typedef struct _AWBALG_POSTPROC_PARAM
{
    UINT32 HCTRRatio; //100~200
    UINT32 HCTBRatio; //100~200
    AWBALG_GAIN HCTMaxGain;
    AWBALG_GAIN HCTMinGain;
    UINT32 LCTRRatio;  //100~200
    UINT32 LCTBRatio;  //100~200
    AWBALG_GAIN LCTMaxGain;
    AWBALG_GAIN LCTMinGain;

}AWBALG_POSTPROC_PARAM;

/**
     AWB input information.

     @note
*/
typedef struct _AWBALG_INFO
{
    UINT32 CurEV;
    UINT32 KRgain;
    UINT32 KBgain;
    UINT32 OutlR;
    UINT32 OutlB;
    UINT32 WinNumX;
    UINT32 WinNumY;
    UINT32 *AccTab[AWBALG_CH_MAX];
    UINT32 *CalTab[AWBALG_CH_MAX];
    UINT32 MinWTotal;
    UINT32 *WTab;
    UINT32 *FlagTab;
    AWBALG_TAB *Tab;
    UINT32 TabCnt;
    AWBALG_PDLTAB *PDLightTab;
    UINT32 PDLightTabCnt;
    AWBALG_CT_PARAM *CTParam;
    AWBALG_POSTPROC_PARAM *PostParam;
    UINT32 ColorBlockCnt;
    AWBALG_COLORBLOCK ColorBlockTab[32];
    AWBALG_EVENT_TAB EventFp;
    UINT32 FuncCtrl;
    AWBALG_REMAP_INFO AwbRemapInfo;
    AWBALG_STABLE_GAIN StableGain;
    //AWBALG_GAIN DefGain;
    AWBALG_GAIN PreGain;
    //AWBALG_GAIN AWBGain;
    UINT32 AwbConvSpeed;
    UINT32 AwbConvStep;
    UINT32 AWBCnt;
    BOOL AwbWeightEn;
} AWBALG_INFO;

/**
     AWB debug information.

     @note
*/
typedef struct _AWBALG_DEBUG_INFO
{
    UINT32 StartTag;
    UINT32 ImgWidth;
    UINT32 ImgHeight;
    UINT32 ImgBitDepth;
    UINT32 CurRgain;
    UINT32 CurGgain;
    UINT32 CurBgain;
    UINT32 PrevRgain;
    UINT32 PrevGgain;
    UINT32 PrevBgain;
    UINT32 CapRgain;
    UINT32 CapGgain;
    UINT32 CapBgain;
    UINT32 EV;
    UINT32 LV;
    UINT32 CABits;
    UINT32 CA_R[1024];
    UINT32 CA_G[1024];
    UINT32 CA_B[1024];
    UINT32 FaceTabCnt;
    AWBALG_COLORBLOCK FaceTab[32];
} AWBALG_DEBUG_INFO;

/**

     calculate Rgain, Bgain

     @note

     @param[in] AWBALG_INFO
     @param[out] Rgain
     @param[out] Bgain
     @return
         - @b AWBALG_OK: SUCCESS
*/
extern AWBALG_ER AWBSim(AWBALG_INFO *pAWBAlgInfo, UINT32 *RGain, UINT32 *GGain, UINT32 *BGain);

extern void AWB_SetAlgMode(AWBALG_MODE mode);
extern AWBALG_MODE AWB_GetAlgMode(void);

extern AWBALG_ER AWBColorMatchSim(AWBALG_INFO *pAWBInfo, UINT32 *RGain, UINT32 *GGain, UINT32 *BGain,
                             UINT32 RValue, UINT32 GValue, UINT32 BValue,
                             AWBALG_COLORBLOCK *pColorBlock,
                             UINT32 wImgWidth, UINT32 wImgHeight);

extern void AWBALG_Remap_Init(AWBALG_INFO *pAWBInfo);
#endif //_AWB_ALG_H_
