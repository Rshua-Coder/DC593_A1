/**
    IPH module (RDE type define)

    @file       IPL_Hal_RDE.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_RDE_H_
#define _IPL_Hal_RDE_H_
#include "IPL_Hal_RDE_Info.h"
#include "IPL_ISREvent.h"
/** \addtogroup mILibIPH */
//@{

typedef enum _IPH_RDE_QUE
{
    ENUM_DUMMY4WORD(IPH_RDE_QUE)
}IPH_RDE_QUE;

/**
     enum of RDE update selection
*/
typedef enum _RDE_UPDATE
{
    //RDE_SubFEn      = 0x1,
    //RDE_DigiGain    = 0x2,
    //RDE_Gamma       = 0x4,
    RDE_INIT        = 0x10000000,
    ENUM_DUMMY4WORD(RDE_UPDATE)
}RDE_UPDATE;


typedef enum _RDE_READ
{
    ENUM_DUMMY4WORD(RDE_READ)
}RDE_READ;

/**
     RDE configuration

     all RDE configuration including all RDE sub function
*/
typedef struct _RDE_Manager
{
    RDE_IN  Input;      ///< input bitstream
    RDE_OUT Output;     ///< output bayer data info

    RDE_SUBFUNC     *pSubFunc;///< -
    //reverse
    //UINT32 rev;                     //reserve for IPH paraset idx
}RDE_Manager,*pRDE_Manager;

/**
     RDE configuration
*/
typedef RDE_Manager RDE;

//@}
#endif
