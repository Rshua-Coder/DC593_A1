#ifndef _IPL_CALIBRATION_BUF_H_
#define _IPL_CALIBRATION_BUF_H_
/**
    IPL Calibration working buffer

    this buffer control mechanism is for storing in/out image during IPL calibration.

    @file       ipl_calibration_buf.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#include "Type.h"
#include "errorno.h"

/** \addtogroup mILibCal */
//@{

/**
    IPL calibration working buffer group ID
*/
//#NT#2013/12/23#Jarkko Chang -begin
//#NT# buffer size modify
typedef enum _Cal_BufID
{
    BUF_ID_1 = 0,   ///< -
    BUF_ID_2,       ///< -
    BUF_ID_3,       ///< -
    BUF_ID_4,       ///< -
    BUF_ID_5,       ///< -
    BUF_ID_6,       ///< -
    BUF_ID_7,       ///< -
    BUF_ID_8,       ///< -
    BUF_ID_9,       ///< -
    BUF_ID_10,       ///< -
    BUF_ID_11,       ///< -
    BUF_ID_12,       ///< -
    BUF_ID_MAX_NUM,
    ENUM_DUMMY4WORD(Cal_BufID)
}Cal_BufID;
//#NT#2013/12/23#Jarkko Chang -end
/**
    working buffer information
*/
typedef struct _CalBuf
{
    Cal_BufID       Id;         ///< group id
    UINT32          StartAddr;  ///< buffer starting address
    UINT32          Size;       ///< buffer size
}CalBuf;

/**
    calibration working buffer operation structure
    @note plz refer to Cal_BufEvent()
*/
typedef struct _CAL_BUF_OBJ
{
    ER (*fpInit)(CalBuf);                                               ///< plz refer to Cal_BufEvent()
    ER (*fpUnInit)(Cal_BufID);                                          ///< plz refer to Cal_BufEvent()
    ER (*fpSetAddr)(Cal_BufID, UINT32 idx, UINT32 size);                ///< plz refer to Cal_BufEvent()
    ER (*fpGetAddr)(Cal_BufID, UINT32 idx, UINT32* pAddr,UINT32* pSize);///< plz refer to Cal_BufEvent()
    ER (*fpGetEnd)(Cal_BufID , UINT32* );                               ///< plz refer to Cal_BufEvent()
}CAL_BUF_OBJ,*pCAL_BUF_OBJ;

/**
     calibration working buffer operation


     @return Description of data returned.
         - @b fpInit:       register indicated continous memory space as working buffer with ID "CalBuf.Cal_BufID"
         - @b fpUnInit:     unmark working buffer.
         - @b fpSetAddr:    partition working buffer with a indicated size and index.
         - @b fpGetAddr:    get the address and size of a partitioned block.
         - @b fpGetEnd:     get the rest of wroking buffer which is not partitioned yet.
     Example: (Optional)
     @code
     {
        Cal_BufEvent()->fpInit(WB);
        Cal_BufEvent()->fpGetEnd(WB.Id,&rest);
        if(rest >= 0x100)
            Cal_BufEvent()->fpSetAddr(WB.Id,id,0x100);
        ...

        //if WB is no longer needed.
        Cal_BufEvent()->fpUnInit(WB.Id);
     }
     @endcode
*/
extern pCAL_BUF_OBJ Cal_BufEvent(void);
//@}
#endif
