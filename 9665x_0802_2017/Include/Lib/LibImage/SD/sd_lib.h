/**
    Smile detection library.

    @file       Sd_lib.h
    @ingroup    mILibSD

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _SD_LIB_H
#define _SD_LIB_H

/**
    @addtogroup mILibFD
*/

/**
     @name SD return status
*/
//@{
#define SD_STA_OK                  (0)                   ///<  the status is ok
#define SD_STA_ERROR               (-1)                  ///<  some error occurred
//@}

/**
     Smile detection callback message.

*/
typedef enum _SD_CBMSG
{
    SD_CBMSG_DETEND = 0,       ///< user will receive this notification when have dectectd smiles in the image
    SD_CBMSG_MAX_CNT,          ///< total callback message numbers
    ENUM_DUMMY4WORD(SD_CBMSG)
}
SD_CBMSG;


/**
    SD callback funcion proto type
*/
typedef void (*SD_CBMSG_FP)(SD_CBMSG uiMsgID, void *Data);

/**
     Init SmileDetect engine.

     This function is used to initialize smile detection buffer and set the process rate.

     @param[in] buf: the work buffer for smile detection process. the required buffer size can be get by SD_CalcBuffSize().
     @param[in] ProcessRate: the process rate of SD_Process, if set value as 3, means calling the function SD_Process() 3 times will run the smile dection once.
     @return
         - @b SD_STA_OK:       Success.
         - @b SD_STA_ERROR:    Some error occurred.

     Example:
     @code
     {
         MEM_RANGE buf;

         buf.Addr = pbuf;
         buf.Size = SD_CalcBuffSize();

         SD_UnInit();
         // Init SD buffer , and set process rate as 3
         SD_Init(buf, 3);
     }
     @endcode
*/
extern INT32 SD_Init(MEM_RANGE *buf, UINT32 ProcessRate);

/**
     Uninit SmileDetect engine.

     This function is used to release SmileDetect engine.
     @return
         - @b SD_STA_OK:       Success.
         - @b SD_STA_ERROR:    Some error occurred.
*/
extern INT32 SD_UnInit(void);

/**
     Process the smile detection.

     This function will find smiles in the result of face detection. It will firstly use FD_GetRsltFaceNum() to check if any Face detected
     and then detect if any smiles of the founded faces.

     @note The process function will really run the smile detection when SD is unlocked. It will just returned when SD is locked.\n
           User can lock/unlock the SD function by calling SD_Lock().\n
           The process rate can be set by SD_Init().

     @return
         - @b >=0:    the number of detected smiles.
         - @b <0:     Some error occurred.

     Example:
     @code
     {
         UINT32  SmileNumber;

         SmileNumber = SD_Process();
     }
     @endcode
*/
extern INT32 SD_Process(void);

/**
     Calculate Smile detection required buffer size.

     @return the required buffer size.
*/
extern UINT32 SD_CalcBuffSize(void);

/**
     Register callback function.

     @note User must register callback funcion then can receive the callback message notification when detected smiles.

     @param[in] CB: the callback function pointer.

     Example:
     @code
     {
        void Photo_SdCB(SD_CBMSG uiMsgID, void *Data)
        {
            switch (uiMsgID)
            {
                case SD_CBMSG_DETEND:
                   DBG_IND("===Smile Detected===\r\n");
                   Ux_PostEvent(NVTEVT_ALGMSG_SDEND,1, NVTEVT_ALGMSG_SDEND);
                   break;
                default:
                   break;
            }
        }
        SD_RegisterCB(Photo_SdCB);
     }
     @endcode
*/
extern void   SD_RegisterCB(SD_CBMSG_FP CB);

/**
     Lock/Unlock smile detction funcion.

     @param[in] bLock: lock or unlock.

     Example:
     @code
     {
         // lock the SD
         SD_Lock(TRUE);

         // doing something such as capture
         ...

         // end of capture

         // unlock the SD
         SD_Lock(FALSE);
     }
     @endcode
*/
extern void   SD_Lock(BOOL bLock);
#endif

