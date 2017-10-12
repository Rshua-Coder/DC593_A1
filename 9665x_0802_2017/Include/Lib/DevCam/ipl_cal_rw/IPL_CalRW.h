#ifndef _IPL_CALRW_H_
#define _IPL_CALRW_H_
/**
    Calibration R/W module

    calibration data management on Dram/PStore

    @file       IPL_CalRW.h
    @ingroup    mILibCalRW

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#include "type.h"
#include "pstore.h"
#include "debug.h"
/** \addtogroup mILibCalRW */
//@{

/**
     @name section name ascii and 32bit-integer transfer
*/
//@{
#define SECNAME_ATOI(char1,char2,char3,char4)   (((char4)<<24)|((char3)<<16)|((char2)<<8)|((char1)))
#define SECNAME_ITOA(UINT_32,pChar)\
{\
    UINT32 UINTE = UINT_32;\
    while( (*pChar++ = (UINTE&0xff)))\
        UINTE = UINTE>>8;\
    *pChar = '\0';\
    pChar -= 5;\
}
//@}

/**
     @name Cal_Header
*/
//@{
#define CAL_HEADER  SECNAME_ATOI('C', 'H', 'E', 'A')    ///< header section
#define CAL_HEADER_SIZE     0x100       ///< Header max size: 256 Bytes
//@}

/**
     @name Basic Section ID
*/
//@{
#define CAL_SEC_A SECNAME_ATOI('C', 'S', 'E', 'A')
#define CAL_SEC_B SECNAME_ATOI('C', 'S', 'E', 'B')
#define CAL_SEC_C SECNAME_ATOI('C', 'S', 'E', 'C')
#define CAL_SEC_D SECNAME_ATOI('C', 'S', 'E', 'D')
#define CAL_SEC_E SECNAME_ATOI('C', 'S', 'E', 'E')
#define CAL_SEC_F SECNAME_ATOI('C', 'S', 'E', 'F')
#define CAL_SEC_G SECNAME_ATOI('C', 'S', 'E', 'G')
#define CAL_SEC_H SECNAME_ATOI('C', 'S', 'E', 'H')
#define CAL_SEC_I SECNAME_ATOI('C', 'S', 'E', 'I')
#define CAL_SEC_J SECNAME_ATOI('C', 'S', 'E', 'J')

#define CAL_SEC_K SECNAME_ATOI('C', 'S', 'E', 'K')
#define CAL_SEC_L SECNAME_ATOI('C', 'S', 'E', 'L')
#define CAL_SEC_M SECNAME_ATOI('C', 'S', 'E', 'M')
#define CAL_SEC_N SECNAME_ATOI('C', 'S', 'E', 'N')
#define CAL_SEC_O SECNAME_ATOI('C', 'S', 'E', 'O')
#define CAL_SEC_P SECNAME_ATOI('C', 'S', 'E', 'P')
#define CAL_SEC_Q SECNAME_ATOI('C', 'S', 'E', 'Q')
#define CAL_SEC_R SECNAME_ATOI('C', 'S', 'E', 'R')
#define CAL_SEC_S SECNAME_ATOI('C', 'S', 'E', 'S')
#define CAL_SEC_T SECNAME_ATOI('C', 'S', 'E', 'T')

#define MAX_SEC 20  ///< maximum sections supported by this module
//@}

/**
     Calibration result status.
*/
typedef enum _CAL_STATUS
{
    _CAL_NONE       = 0x0,          ///< no result
    _CAL_OK         = 0xfedba950,   ///< result is pass
    _CAL_OverFlow   = 0x51,         ///< result is over spec
    _CAL_UnderFlow  = 0x52,         ///< result is under spec
    _CAL_DP_D_OK    = 0x61,         ///< DP Dark is finish and PASS
    _CAL_DP_B_OK    = 0x62,         ///< DP Bright is finish and PASS
    _CAL_AE_TIMEOUT = 0x70,         ///< AE timeout
    ENUM_DUMMY4WORD(CAL_STATUS)
}CAL_STATUS;

/**
    Tag information
*/
typedef struct _CAL_TAG
{
    UINT32      TagID;          ///< Tag identifier
    CAL_STATUS  CAL_STATUS;     ///< Tag status
    UINT32      TagDataLen;     ///< Tag data length, unit: Byte
    UINT32      TagData;        ///< Tag data pool
}CAL_TAG,*pCAL_TAG;

#define TAG_TypeSize (sizeof(CAL_TAG) - sizeof(UINT32))

/**
    Result of Dram memory managment
*/
typedef struct _CAL_DMA_RST
{
    ER erCode;          ///< -
    UINT32 RemainAddr;  ///< remain space starting address
    UINT32 RemainSize;  ///< remain space size
}CAL_DMA_RST;

/**
    return FALSE if all sections have been allcated memory space.
    otherwise, return TRUE and it will be re-called until the return value is FALSE.
*/
typedef BOOL (*fpSectionAllocate)(UINT32* pSecID,UINT32* pDmaSize);

/**
    R/W module operation structure.

    Detail description plz reference api:CAL_RW().
*/
typedef struct _CAL_RW_OP
{
    CAL_DMA_RST (*fpInit)(UINT32 DmaAddr,UINT32 DmaSize,fpSectionAllocate fptr); ///< plz reference CAL_RW()

    ER (*fpAddTag)(UINT32 SecID,CAL_TAG* pTagInfo);///< plz reference CAL_RW()
    ER (*fpDeleteTag)(UINT32 SecID,UINT32 TagID);///< plz reference CAL_RW()
    pCAL_TAG (*fpReadTag)(UINT32 SecID,UINT32 TagID);///< plz reference CAL_RW()

    ER (*fpAddSection)(UINT32 SecID,UINT32 DmaAddr,UINT32 DmaSize);///< plz reference CAL_RW()
    ER (*fpDeleteSection)(UINT32 SecID);///< plz reference CAL_RW()

    CAL_DMA_RST (*fpSectionRead_Header)(UINT32 BufAddr,UINT32 BufSize);///< plz reference CAL_RW()
    ER (*fpSectionWrite_Header)(void);///< plz reference CAL_RW()

    CAL_DMA_RST (*fpSectionRead)(UINT32 SecID,UINT32 DmaAddr,UINT32 DmaSize);///< plz reference CAL_RW()
    ER (*fpSectionWrite)(UINT32 SecID);///< plz reference CAL_RW()

    CAL_DMA_RST (*fpSectionAllRead)(UINT32 DmaAddr,UINT32 DmaSize);///< plz reference CAL_RW()
    ER (*fpSectionAllWrite)(void);///< plz reference CAL_RW()
}CAL_RW_OP,*pCAL_RW_OP;



/**
     Calibration data Read/Write control

     control calibration data to read/wirte from/to Pstore or Dram.
     must be to "Init" first before using this R/W control module.

     @return Description of data returned.
         - @b fpInit:   Initial R/W module and allcate all sections' memory space
\n                      input para:
\n                          DmaAddr, DmaSize: dram starting address and available dram size for this R/W module.
\n                          fpSectionAllocate: function pointer of section allocation api if needed.
         - @b fpAddTag: add TAG into specified Section.
\n                      input para:
\n                          SecId:  add Tag data to this Section Id.
\n                          pTagInfo: pointer of this tag.
         - @b fpDeleteTag:  Delete Tag data from specified Section.
\n                      input para:
\n                          SecID:  delete Tag from this Section Id.
\n                          TagID:  delete Tag data of this Tad id.
        - @b fpReadTag:  return Tag data.
\n                      input para:
\n                          SecID:  read Tag from this Section Id.
\n                          TagID:  read Tag data of this Tad id.
        - @b fpAddSection:  add section's memory location into R/W module
\n                      input para:
\n                          SecID:  add this Section ID's memory location
\n                          DmaAddr,DmaSize:  Section's memory location
        - @b fpDeleteSection:  delete Section record from R/W module
\n                      input para:
\n                          SecID:  delete this Section ID information.
        - @b fpSectionRead_Header:  read Header from Pstore to target dram location
\n                      input para:
\n                          BufAddr,BufSize: target dram location infomation.
        - @b fpSectionWrite_Header:  write Header from dram to Pstore.
\n                      input para: none
        - @b fpSectionRead:  read Section from Pstore to target dram location
\n                      input para:
\n                          SecID:  read Pstore according this Section ID.
\n                          BufAddr,BufSize: target dram location infomation.
        - @b fpSectionWrite:  write Section from dram to Pstore.
\n                      input para:
\n                          SecID:  Write Pstore according this Section ID.
        - @b fpSectionAllRead:  read Header and All Sections from Pstore to Dram.
\n                      input para:
\n                          BufAddr,BufSize: target dram location infomation.
        - @b fpSectionAllWrite:  write Header and All Sections from Pstore to Dram.
\n                      input para:none

     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern pCAL_RW_OP CAL_RW(void);

/**
     write version as calibration pstore file name


     @param[in] pStr       version string
*/
extern void CAL_PatchVer(CHAR* pStr);
//@}
#endif
