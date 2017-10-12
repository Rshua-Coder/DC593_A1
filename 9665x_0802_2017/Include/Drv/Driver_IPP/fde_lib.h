/*
    Public header file for FDE module.

    This file is the header file that define the API and data type for FDE
    module.

    @file       fde_lib.h
    @ingroup    mIIPPFDE
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _FDE_LIB_H
#define _FDE_LIB_H

/**
    @addtogroup mIIPPFDE
*/
//@{

/**
    Define FDE interrput enable.
*/
//@{
#define FDE_INTE_ALL                    0x00000001  ///< all interrupt
#define FDE_INTE_SIZE                   0x00000002  ///< one size done
#define FDE_INTE_16                     0x00000004  ///< 16 faces detected
#define FDE_INTE_OFAIL                  0x00000008  ///< overflow flag
//@}

/**
    Define FDE interrput status.
*/
//@{
#define FDE_INT_ALL                     0x00000001  ///< all interrupt
#define FDE_INT_SIZE                    0x00000002  ///< one size done
#define FDE_INT_16                      0x00000004  ///< 16 faces detected
#define FDE_INT_OFAIL                   0x00000008  ///< overflow flag
//@}

/**
    Define FDE parameters.
*/
//@{
#define FACE_NUM_MAX                    36          ///< max detection number
//@}

/**
    FDE FD Function Selection.

    Select FD functions.
\n  Used for fde_setFDMode()
*/
//@{
typedef enum
{
    FDE_FD_FACEDETECTION    = 0,    ///< face detection
    FDE_FD_EDGE_EXTRACTION  = 1,    ///< edge extraction
    FDE_FD_SCALING          = 2,    ///< scaling down
    FDE_USERDEFINE          = 3,    ///< all manual mode
    ENUM_DUMMY4WORD(FDE_FDFUNC)
} FDE_FDFUNC;
//@}

/**
    FDE FD OPeration Mode Selection.

    Select FDE operation mode.
*/
//@{
typedef enum
{
    FDE_OPMODE_FD       = 0,        ///< normal face detection mode
    FDE_OPMODE_LUMIN    = 1,        ///< output last scaled luminance plane
    FDE_OPMODE_EDGH     = 2,        ///< output last scaled horizontal edge plane
    FDE_OPMODE_EDGV     = 3,        ///< output last scaled vertical edge plane
    FDE_OPMODE_EDGHV0   = 4,        ///< output last scaled abs(H) + abs(V) edge plane
    FDE_OPMODE_EDGHV1   = 5,        ///< output last scaled abs(H+V) edge plane
    ENUM_DUMMY4WORD(FDE_FDOPMODE)
} FDE_FDOPMODE;
//@}

/**
    FDE FD Scaling Direction.

    Select which direction of scaling factor
*/
//@{
typedef enum
{
    FDE_HORIZONTAL  = 0,            ///< horizontal scaling factor
    FDE_VERTICAL    = 1,            ///< vertical scaling factor
    ENUM_DUMMY4WORD(FDE_FDDIRECTION)
} FDE_FDDIRECTION;
//@}

/**
    FDE OPP En/Disable Selection.

    Select output buffer
*/
//@{
typedef enum
{
    FDE_OPPDISABLE   = 0,           ///< one ping-pong buffer
    FDE_OPPENABLE    = 1,           ///< two ping-pong buffer
    FDE_OPPSEQUEN    = 2,           ///< sequential output scaled data
    ENUM_DUMMY4WORD(FDE_FDOPP)
} FDE_FDOPP;
//@}

/**
    FDE Result Format Selection.

    Select result format
\n  FD_RSLT_ORI:  { Score[31..24] Size[22..18] Sty[17..9] Stx[8..0]}
\n  FD_RSLT_MOD:  { Score[31..25] Target[24..23] Size[22..18] Sty[17..9] Stx[8..0]}
\n  Stx = actual_stx >> 2;
\n  Sty = actual_sty >> 2;

*/
//@{
typedef enum
{
    FD_RSLT_ORI     = 0,        ///< original result format
    FD_RSLT_MOD     = 1,        ///< modified result format
    FD_RSLT_DBG     = 2,        ///< debug result format
    ENUM_DUMMY4WORD(FD_RSLT_SEL)
} FD_RSLT_SEL;
//@}

/**
    FDE Result Selection.

    Select origilal FD coordinate results or remove overlap ones
*/
//@{
typedef enum
{
    FDE_RSLT_NONOVERLAP   = 0,      ///< numbers of overlap FDs are removed
    FDE_RSLT_ORIGINAL     = 1,      ///< numbers of original FD engine output
    ENUM_DUMMY4WORD(FD_RSLT_OVLP)
} FD_RSLT_OVLP;
//@}

/**
    FDE Face Orientation.

    Select which orientations to detect
*/
//@{
typedef enum
{
    FDE_R000_P000       = 0,    ///< frontal face
    FDE_R030_P000       = 1,    ///< rotate positive 30 degree
    FDE_R330_P000       = 2,    ///< rotate negative 30 degree
    FDE_R000_PR90       = 3,    ///< right side face
    FDE_R030_PR90       = 4,    ///< right side face rotate positive 30 degree
    FDE_R330_PR90       = 5,    ///< right side face rotate negative 30 degree
    FDE_R000_PL90       = 6,    ///< left side face
    FDE_R030_PL90       = 7,    ///< left side face rotate positive 30 degree
    FDE_R330_PL90       = 8,    ///< left side face rotate negative 30 degree
    FDE_ORIENT_DISABLE  = 9,    ///< disable this target
    FDE_ORIENT_NUMMAX   = 10,   ///< max number of orientation
    ENUM_DUMMY4WORD(FDE_ORIENT)
} FDE_ORIENT;
//@}

/**
    Struct FDE open object.

    Structure for fde_open()
*/
//@{
typedef struct _FDE_OPENOBJ
{
    VOID (*FP_FDEISR_CB)(UINT32 uiIntStatus); ///< isr callback function
    UINT32 uiFdeClockSel; ///< support 220/192/160/120/80/60/48 Mhz
} FDE_OPENOBJ;
//@}

/**
    Struct FDE Result Lowpass.
    @brief none.
*/
//@{
typedef struct _FDE_LPF_PRAM
{
    BOOL    bEnable;          ///< enable result lowpass
    UINT32  uiShift;          ///< result window moving speed
    UINT32  uiOverdue;        ///< result window overdue
} FDE_LPF_PRAM;
//@}


/**
    Struct FDE target threshold.

    Set FDE targets threshold.
*/
//@{
typedef struct _FDE_FDE_THPRAM
{
    UINT32  uiWeight0;      ///< channel0 weighting
    UINT32  uiWeight1;      ///< channel1 weighting
    UINT32  uiWeight2;      ///< channel2 weighting
    UINT32  uiScoreShf;     ///< score shift
    UINT32  uiScoreTh;      ///< score threshold
    UINT32  uiStdTh;        ///< std threshold
    UINT32  uiChanThEn;     ///< channel threshold enable
    UINT32  uiChanTh0;      ///< channel threshold 0
    UINT32  uiChanTh1;      ///< channel threshold 1
    UINT32  uiChanTh2;      ///< channel threshold 0
} FDE_FDE_THPRAM;
//@}

/**
    Struct FDE target threshold.

    Set FDE targets threshold.

\n  edge kernal = [eextdb   eextv   eextda
\n                 eexth    eextc   eexth
\n                 eextda   eextv   eextdb] >> eextdiv
*/
//@{
typedef struct _FDE_FDE_EDGPARM
{
    UINT8  uiEdgExtDiv;     ///< edge kernal division
    UINT8  uiEdgExtC;       ///< edge kernal center coefficient
    UINT8  uiEdgExtH;       ///< edge kernal horizontal coefficient
    UINT8  uiEdgExtV;       ///< edge kernal vertical coefficient
    UINT8  uiEdgExtDa;      ///< edge kernal diagonalA coefficient
    UINT8  uiEdgExtDb;      ///< edge kernal diagonalB coefficient
} FDE_FDE_EDGPARM;
//@}

/**
    Struct FDE Information.

    Set FDE information to execute.
*/
//@{
typedef struct _FDE_FDE_PRAM
{
    UINT32  uiInAddr;           ///< input starting address
    UINT32  uiOutAddr0;         ///< output starting address 0
    UINT32  uiOutAddr1;         ///< output starting address 1
    UINT32  uiOutAddr2;         ///< output starting address 2
    UINT32  uiWidth;            ///< image width
    UINT32  uiHeight;           ///< image height
    UINT32  uiInOfs;            ///< image input lineoffset
    UINT32  uiOutOfs;           ///< image output lineoffset
    FDE_FDOPMODE  OpMode;       ///< operation mode
    UINT32  uiSclNum;           ///< scaling number
    UINT32  uiShf0;             ///< channel 0 shift
    UINT32  uiShf1;             ///< channel 1 shift
    UINT32  uiShf2;             ///< channel 2 shift
    UINT32  uiHFact;            ///< horizontal scaling factor
    UINT32  uiVFact;            ///< vertical scaling factor
    FDE_FDE_THPRAM *pTrgTh;     ///< target 0 threshold
    FDE_FDE_THPRAM *pTrgTh2;    ///< target 1 threshold
    FDE_FDE_THPRAM *pTrgTh3;    ///< REMOVED in NT96460
    UINT32  *puiTarget;         ///< face target 0
    UINT32  *puiTarget2;        ///< face target 1
    UINT32  *puiTarget3;        ///< REMOVED in NT96460
    FDE_ORIENT Orient;          ///< face orient 0
    FDE_ORIENT Orient2;         ///< face orient 1
    FDE_ORIENT Orient3;         ///< REMOVED in NT96460
    FDE_FDE_EDGPARM *pEdgKer;   ///< edge kernal 0
    FDE_FDE_EDGPARM *pEdgKer2;  ///< edge kernal 1
    FD_RSLT_SEL RsltSel;        ///< result format
    FDE_FDOPP   Opp;            ///< output ping-pong buffer enable
    UINT8   uiOrientPri;        ///< orientation piority
    FDE_LPF_PRAM RsltLpf;       ///< result low pass parameter
} FDE_FDE_PRAM;
//@}

/**
    Struct FDE Location and Score

    Information of face result
*/
//@{
typedef struct _FACE
{
    UINT16  uiStx;            ///< result start x
    UINT16  uiSty;            ///< result start y
    UINT16  uiWidth;          ///< result width
    UINT16  uiHeight;         ///< result height
    UINT8   uiScore;          ///< result score
    UINT8   uiTrgIdx;         ///< target index
    UINT32  uiOverlapCount;
    UINT32  uiTotalScore;     
} FACE, *PFACE;
//@}

/**
    Struct FDE input size infor

    FDE input size information
*/
//@{
typedef struct _FACESIZEINFOR
{
    UINT16  uiWidth;          ///< input width
    UINT16  uiHeight;         ///< input height
    UINT16  uiInOfs;          ///< input lineoffset
    UINT16  uiOutOfs;         ///< output lineoffset
} FACESIZEINFOR, *P_FACESIZEINFOR;
//@}

extern ER       fde_setFDMode(FDE_FDFUNC Mode, FDE_FDE_PRAM *pFdeInfo);
extern ID       fde_getLockStatus(VOID);
extern ER       fde_open(FDE_OPENOBJ *pObjCB);
extern BOOL     fde_isOpened(VOID);
extern ER       fde_close(VOID);
extern ER       fde_EnableInt(UINT32 uiIntr);
extern UINT32   fde_getIntEnable(VOID);
extern UINT32   fde_IntStatus(VOID);
extern ER       fde_ClearInt(UINT32 uiIntr);
extern ER       fde_SetRsltNum(UINT32  uiNum);
extern UINT32   fde_GetDmaInAddr(VOID);
extern UINT32   fde_GetDmaOutAddr0(VOID);
extern UINT32   fde_GetDmaOutAddr1(VOID);
extern UINT32   fde_GetDmaOutAddrRslt(VOID);
extern UINT32   fde_GetRsltNum(FD_RSLT_OVLP Overlap);
extern ER       fde_GetInSizeInfor(FACESIZEINFOR *pSizeInfor);
extern UINT32   fde_GetScaleFactor(FDE_FDDIRECTION Direction);
extern UINT32   fde_GetScaleNum(VOID);
extern FDE_FDOPP fde_GetOPPMode(VOID);
extern ER fde_modParam(FDE_ORIENT Orient0, UINT32 uiIndex, UINT32 uiValue);
extern FDE_FDOPMODE fde_GetOPmode(VOID);
extern UINT32 fde_getClockRate(VOID);
#endif

//@}
