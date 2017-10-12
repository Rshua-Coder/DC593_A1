/**
    IPL_Ctrl module(Rolling shutter correction control, FW trig flow only)

    enable/disable/configure/trig control rolling shutter correction correction

    @file       IPL_Ctrl_RSC_Int.h
    @ingroup    mIIPL_Ctrl
    @note       none

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_CTRL_RSC_INT_H_
#define _IPL_CTRL_RSC_INT_H_

#include "IPL_Hal.h"
/** \addtogroup mIIPL_Ctrl */
//@{

/**
    Gyro data format.
    temporal def, this should be from Gyro driver
*/
typedef enum
{
    _INT16,     ///< signed 16 bits
    _IS2000A,   ///< 2000A
    _ISIDG2020, ///< IDG2020
    ENUM_DUMMY4WORD(RSCCTRL_GD_FMT)
}RSCCTRL_GD_FMT;

//C H Lin - RSC_Modification: Add Gyro Direction
typedef enum
{
    _RSC_GYRO_DIR_UP_RIGHT,     ///< X+ = up    , Y+ = right
    _RSC_GYRO_DIR_UP_LEFT,      ///< X+ = up    , Y+ = left
    _RSC_GYRO_DIR_DOWN_RIGHT,   ///< X+ = down  , Y+ = right
    _RSC_GYRO_DIR_DOWN_LEFT,    ///< X+ = down  , Y+ = left
    _RSC_GYRO_DIR_RIGHT_UP,     ///< X+ = right , Y+ = up
    _RSC_GYRO_DIR_RIGHT_DOWN,   ///< X+ = right , Y+ = down
    _RSC_GYRO_DIR_LEFT_UP,      ///< X+ = left  , Y+ = up
    _RSC_GYRO_DIR_LEFT_DOWN,    ///< X+ = left  , Y+ = down
    ENUM_DUMMY4WORD(RSCCTRL_GYRO_DIRECTION)
}RSCCTRL_GYRO_DIRECTION;

/**
    RSC quality
*/
typedef struct
{
    UINT32 CompRatio;   ///< compensation ratio in %.(ratio=margin/body, while total-size=margin+body)
    UINT32 Stability;   ///< the magnitude of stability, 100% max.
}RSCCTRL_Q;

/**
     Distortion information for RSC
*/
typedef struct
{
    DIST        Dce_Dist;
    Fact_Norm   Radious;
    Coordinate  Center;
    UINT32      FovGain;
    UINT32*     pLUT_Tbl;    ///< diistortion table (tab num:65, Data type:UINT32)
    UINT32      minStrpeSize;
}RSCCTRL_DCEinfo;


/**
     Bayer format image size info.
*/
typedef struct
{
    USIZE SieCropSize;
    USIZE SieOutSize;
    USIZE PreInSize;
}RSCCTRL_IPLSIZE_IN;

/**
     YCC format image size info
*/
typedef struct
{
    USIZE   IME_P1_ScaleSize;
    USIZE   IME_P1_CropSize;
    USIZE   IME_P2_ScaleSize;
    USIZE   IME_P2_CropSize;
}RSCCTRL_IPLSIZE_OUT;


/**
    Sensor array info
*/
typedef USIZE SenArraySize; ///<sensor array size, uint:um

/**
    Sensor output info
*/
typedef struct
{
    UINT32 VdT;         ///< length of VD signal, uint:us
    UINT32 StrLnT;      ///< length from VD start to 1st active line(including OB), uint:us
    UINT32 EndLnT;      ///< length from VD start to last active line(including OB), uint:us
    UINT32 TransDelyT;  ///< length from exposure end to start of data transmission, uint:us
    USIZE  OutSize;     ///< sensor array size(including OB),unit: pixel
}RSCCTRL_SEN_OUT;


/**
    Gyro reference
*/
typedef struct
{
    RSCCTRL_GD_FMT  GDFmt;      ///< Gyro data format
    //C H Lin - RSC_Modification: Add Gyro Direction
    RSCCTRL_GYRO_DIRECTION GDDir; ///< Gyro data Direction
    UINT32          GDunit;     ///< Gyro data unit, unit: degree
    UINT32          GDCalGain;  ///< calibration gain.
    UINT32          GDCalOfs;   ///< calibration offset.
}RSCCTRL_Gyro_REF;

/**
     Gyro data table,Max length:8 ctrl points.
*/
typedef struct
{
    //UINT32 (*pGyroDataTbl)[2];
    UINT32 *pGyroX;
    UINT32 *pGyroY;
    UINT32 DataCnt;             ///< valid data number
}RSCCTRL_GYRO_TBL;

/**
    information for RSC inner operand calculation
*/
typedef struct
{
    RSCCTRL_SEN_OUT*        pSenInfo;
    RSCCTRL_DCEinfo*        pDceInfo;
    RSCCTRL_IPLSIZE_IN*     pIPLIn;
    RSCCTRL_IPLSIZE_OUT*    pIPLOut;
    UINT32*                 pFL;        ///< Focal length
    UINT32*                 pExpT;      ///< exposure time
    RSCCTRL_GYRO_TBL*       pGDTbl;
}RSCCTRL_INFO;

typedef void (*FP_RSCCTRL_RunTimeChg)(RSCCTRL_INFO*);

/**
    RSC initial configuration
*/
typedef struct
{
    IPL_HAL_PARASET     Idx;
    SenArraySize        SenSize;
    BOOL                bEIS_En;  ///< true if EIS switch on.
    UINT32              GD_Num;   ///< the number of gyro-data each frame; 8 for RSC-ON, 1~8 for EIS-ON
    RSCCTRL_Gyro_REF    GyroRef;
    //RSCCTRL_INFO        *pInfo;
    RSCCTRL_Q           *pRSC_Q;
    FP_RSCCTRL_RunTimeChg   fpRunTimeChg;
}RSCCTRL_INIT;

#if 0
/**
     RSC calculation mode
*/
typedef enum
{
    RSCCTRL_Gyro_OK     = 0,    ///< all gyro data are ready
    RSCCTRL_Gyro_MISS   = 1,    ///< some of these gyro data miss.
    ENUM_DUMMY4WORD(RSCCTRL_Gyro_FB)
}RSCCTRL_Gyro_FB;
#endif

/**
     Check RSC calculation ready or not

     if RSC calculation is done, result will be configured into ime automatically.
     otherwise, ime will start with previous RSC result.
     @note (Optional)
     @param[out] pFuncEn:   function ctrl var. address, return 0 if no need to update
     @param[out] pRsc:      Rsc result var. address, return 0 if no need to update
     @return Description of data returned.
         - @b E_OK:     RSC calculation is done.
         - @b E_SYS:    RSC calculation still running.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER ICF_RSCCTRL_ChkResult(UINT32 *pFuncEnAdr,UINT32 *pRscAdr);



/**
     Trig RSC start

     trig RSC task start to calcaulte, this command won't wait unitl calculation complete.
     @note:if Gyro data missd, Lib will start the mech. of Gyro data prediction.


     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void ICF_RSCCTRL_TRIG(void);

/**
     Initialize RSC

     @param[in] pInit:    Initial configuration
     @return Description of data returned.
         - @b E_OK:     RSC open is done.
         - @b E_SYS:    RSC have been opened, or input is NULL.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER ICF_RSCCTRL_OPEN(RSCCTRL_INIT *pInit);

/**
    CLOSE RSC
*/
extern ER ICF_RSCCTRL_CLOSE(void);
//@}
#endif
