/*
    Crypto driver header file

    Crypto driver header file.

    @file       crypto.h
    @ingroup    mIMISCCrypto
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _CRYPTO_H
#define _CRYPTO_H

#include "Type.h"

/**
    @addtogroup mIMISCCrypto
*/
//@{

/**
    Crypto engine crypto mode
*/
typedef enum
{
    CRYPTO_DES = 0x00,                  ///< Select Crypto mode DES
    CRYPTO_3DES,                        ///< Select Crypto mode 3DES
    CRYPTO_AES,                         ///< Select Crypto mode AES
    CRYPTO_MODE_NUM,
    ENUM_DUMMY4WORD(CRYPTO_MODE)
} CRYPTO_MODE;


/**
    Crypto engine encrypt or decrypt
*/
typedef enum
{
    CRYPTO_ENCRYPT = 0x00,              ///< Select Crypto engine encrypt
    CRYPTO_DECRYPT,                     ///< Select Crypto engine decrypt
    CRYPTO_TYPE_NUM,

    ENUM_DUMMY4WORD(CRYPTO_TYPE)
} CRYPTO_TYPE;


/**
    Crypto engine endian selection
*/
typedef enum
{
    CRYPTO_BYTE_LITTLE_ENDIAN = 0x00,   ///< Select Crypto engine little endian
    CRYPTO_BYTE_BIG_ENDIAN,             ///< Select Crypto engine big endian

    CRYPTO_BYTE_ENDIAN_NUM,
    ENUM_DUMMY4WORD(CRYPTO_BYTE_ENDIAN)
} CRYPTO_BYTE_ENDIAN;


/**
    Crypto engine bit order selection
*/
typedef enum
{
    CRYPTO_B_0_7 = 0x00,                ///< Select Crypto engine bit order 0 to 7
    CRYPTO_B_7_0,                       ///< Select Crypto engine bit order 7 to 0

    CRYPTO_BIT_ORDER_NUM,
    ENUM_DUMMY4WORD(CRYPTO_BIT_ORDER)
} CRYPTO_BIT_ORDER;


/**
    Crypto engine clock rate
*/
typedef enum
{
    CRYPTO_CLOCK_80MHz = 0x00,          ///< Select Crypto clock rate 80 MHz
    CRYPTO_CLOCK_160MHz,                ///< Select Crypto clock rate 160 MHz

    CRYPTO_CLOCK_RATE_NUM,
    ENUM_DUMMY4WORD(CRYPTO_CLOCK_RATE)
} CRYPTO_CLOCK_RATE;

/**
    Crypto configuration identifier

    @note For crypto_setConfig()
*/
typedef enum
{
    CRYPTO_CONFIG_ID_CLOCK_RATE,        ///< Select Crypto engine internal clock rate usage
                                        ///< Context can be any of:
                                        ///< - @b CRYPTO_CLOCK_80MHz:   Internal clock rate 80MHz
                                        ///< - @b CRYPTO_CLOCK_160MHz:  Internal clock rate 160MHz

    ENUM_DUMMY4WORD(CRYPTO_CONFIG_ID)
} CRYPTO_CONFIG_ID;



typedef struct _CRYPT_SET
{
    UINT32  uiMode;                     ///<Crypto mode
                                        ///<Context can by any of
                                        ///<  -@b CRYPTO_DES
                                        ///<  -@b CRYPTO_AES
                                        ///<  -@b CRYPTO_3DES
    UINT32  uiType;
    UINT32  uiByteEndian;
    UINT32  uiBitOrder;
    UINT8   ucInput[16];
    UINT8   ucKey[24];
    UINT8   ucOutput[16];
}CRYPT_SET, *pCRYPT_SET;

extern void crypto_setMode(UINT32 uiMode, UINT32 uiType, UINT32 uiByteEndian, UINT32 uiBitOrder);
extern void crypto_setKey(UINT8 * ucKey, UINT32 uiLen);
extern void crypto_setInput(UINT8 * ucInput, UINT32 uiLen);
extern void crypto_getOutput(UINT8 * Output, UINT32 Len);
extern ER   crypto_setConfig(CRYPTO_CONFIG_ID configID, UINT32 configContext);
extern ER   crypto_open(void);
extern ER   crypto_close(void);
extern BOOL crypto_isBusy(void);
//@}

#endif // _CRYPTO_H



