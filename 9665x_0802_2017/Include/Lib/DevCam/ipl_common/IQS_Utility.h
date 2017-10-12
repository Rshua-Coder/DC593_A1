#ifndef _IQS_UTILITY_H_
#define _IQS_UTILITY_H_
/**
    IQS_Utility.h


    @file       IQS_Utility.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "type.h"
#include "ErrorNo.h"
#include "IPL_Utility.h"
#include "IPL_Hal_DCE_Info.h"

#define IQS_UD_INDEX_NOSET  999999


/** \addtogroup IQS_Utility */
//@{


#define IQS_SIE_REG_SIZE  0x100
#define IQS_PRE_REG_SIZE  0xd0
#define IQS_IFE_REG_SIZE  0xd8
#define IQS_DCE_REG_SIZE  0x108
#define IQS_IPE_REG_SIZE  0x1e10
#define IQS_IME_REG_SIZE  0x2b8
#define IQS_IFE2_REG_SIZE 0x48

#define IQS_PRI_SIEREG_FILE  "A:\\IQS\\PRI\\SIE.BIN"
#define IQS_PRI_PREREG_FILE  "A:\\IQS\\PRI\\PRE.BIN"
#define IQS_PRI_IPEREG_FILE  "A:\\IQS\\PRI\\IPE.BIN"
#define IQS_PRI_IFEREG_FILE  "A:\\IQS\\PRI\\IFE.BIN"
#define IQS_PRI_DCEREG_FILE  "A:\\IQS\\PRI\\DCE.BIN"
#define IQS_PRI_IMEREG_FILE  "A:\\IQS\\PRI\\IME.BIN"
#define IQS_PRI_IFE2_Y_FILE  "A:\\IQS\\PRI\\IFE2Y.BIN"
#define IQS_PRI_IFE2_UV_FILE "A:\\IQS\\PRI\\IFE2CC.BIN"

#define IQS_QV_SIEREG_FILE  "A:\\IQS\\QV\\SIE.BIN"
#define IQS_QV_PREREG_FILE  "A:\\IQS\\QV\\PRE.BIN"
#define IQS_QV_IPEREG_FILE  "A:\\IQS\\QV\\IPE.BIN"
#define IQS_QV_IFEREG_FILE  "A:\\IQS\\QV\\IFE.BIN"
#define IQS_QV_DCEREG_FILE  "A:\\IQS\\QV\\DCE.BIN"
#define IQS_QV_IMEREG_FILE  "A:\\IQS\\QV\\IME.BIN"
#define IQS_QV_IFE2_Y_FILE  "A:\\IQS\\QV\\IFE2Y.BIN"
#define IQS_QV_IFE2_UV_FILE "A:\\IQS\\QV\\IFE2CC.BIN"

#define IQS_QVREF_IMED2D_FILE "A:\\IQS\\QV\\IMEQVREF.BIN"
#define IQS_QV_IMED2D_FILE  "A:\\IQS\\QV\\IMED2D.BIN"

typedef enum
{
    IQS_SIE_ADDR  = 0xB0C00000,
    IQS_PRE_ADDR  = 0xB0C11000,
    IQS_DCE_ADDR  = 0xB0C20000,
    IQS_IFE_ADDR  = 0xB0C70000,
    IQS_IPE_ADDR  = 0xB0C30000,
    IQS_IME_ADDR  = 0xB0C40000,
    IQS_IFE2_ADDR = 0xB0D00000,
    ENUM_DUMMY4WORD(IQS_END_ADDR)
}IQS_END_ADDR;

/**
     re-organize gamma lut

     @note convert Gamma Lut
     @param[in]
     @return
         - @b UINT32.
*/
UINT32* IPE_GammaReorgon(UINT32* RLut, UINT32* GLut, UINT32* BLut);

/**
     The functions to fix the IQS table

     @param[in]
     @return
         - @b UINT32.
*/
void UserDefineIQIndexGet(UINT32 *ISOIndex, UINT32 *SenModeIndex, UINT32 *ZoomIndex);
void UserDefineIQIndexSet(UINT32 ISOIndex, UINT32 SenModeIndex, UINT32 ZoomIndex);
void UserDefineIQIndexReset(void);

/**
     The function to check the type of DCE Lut
     Return DCE Lut type

     @param[in] Lut    DCE Lut

     @return
         - @b NON_STRICTLY_INC_DEC:       Not strictly increase or decrease
         - @b STRICTLY_INC_DEC:           Strictly increase or decrease
*/
DCE_LutType DCE_GetTableType(UINT32 *Lut);

/**
     The function to dump the IPP register

     @param[in] Msg    Message from capture flow

     @return
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern BOOL IQS_DumpEngRegister(IPL_FLOW_PROC_MSG Msg);
extern BOOL IQS_DumpPrvRegister(void);

typedef void (*IQS_MOVIELOG_FP)(char* Buf, UINT32 BufLen);
void IQS_SetMovieLogFP(IQS_MOVIELOG_FP fp);
void IQS_GetLogString(char* Buf, UINT32 BufLen);

char *IQS_GetVerInfo(void);
char *IQS_GetBuildDate(void);
void IQS_SetVerInfo(char* Info);
void IQS_SetBuildDate(char* Info);
//@}
#endif //_IQS_UTILITY_H_
