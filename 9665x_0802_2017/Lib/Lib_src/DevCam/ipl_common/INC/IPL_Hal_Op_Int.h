/**
    IPH module

    this module provide consistent interface connecting to SIE/PRE/IFE/DCE/IPE/IFE2/IME/RDE engine driver

    @file       IPL_Hal.h
    @ingroup    mIIPH
    @note       none

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_HAL_OP_INT_H_
#define _IPL_HAL_OP_INT_H_
#include "IPL_Hal.h"
#include "IPL_Utility.h"
/** \addtogroup mIIPH */
//@{

/**
    query operation supported by each engine
*/
typedef union
{
    IPH_SIE_QUE SieQ;
    IPH_PRE_QUE PreQ;
    IPH_IFE_QUE IfeQ;
    IPH_DCE_QUE DceQ;
    IPH_IPE_QUE IpeQ;
    IPH_IFE2_QUE Ife2Q;
    IPH_IME_QUE ImeQ;
    IPH_SIE2_QUE Sie2Q;
    UINT32 data;
}IPH_QUE;

/**
    engine inner table info
*/
typedef union
{
    SIE_READ sieRead;
    PRE_READ preRead;
    IFE_READ ifeRead;
    DCE_READ dceRead;
    IPE_READ ipeRead;
    IFE2_READ ife2Read;
    IME_READ imeRead;
    SIE2_READ sie2Read;
    UINT32 data;
}IPL_HAL_READ;

/**
    -operation group
*/
//{

/**
     Open engine

     Open engine and hook interrupt handler call-back function
     @note : operation rule as follow.

\n         -------        -------        -----------
\n        | close |  ->  | open  |  ->  | chg/load  | <--
\n         -------        -------        -----------     |
\n            ^                             |            |
\n            |                             v            |
\n         -------                       -------         |
\n        | stop  |     <----------     | start |  ------
\n         -------                       -------
\n            ^                             |
\n            |           -------           |
\n             --------  | wait  |   <------
\n                        -------
\n
     @param[in]  eng       open indicated engine
     @param[in]  fp_ptr    ISR callback func.
     @param[in]  clk       engine working clk
     @return Description of data returned.
         - @b E_OK:     engine open
         - @b E_SYS:    engine is being opened.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_Open(IPL_HAL_ENG eng,DRV_ISR fp_ptr,IPH_CLK clk);


_INLINE ER SIE_ENG_Open(DRV_ISR fp_ptr,IPH_CLK clk,UINT32 PclkSel)
{
    PclkSel = PclkSel<<16;
    return IPH_ENG_Open(IPL_HAL_SIE|PclkSel,fp_ptr,clk);
}

_INLINE ER SIE2_ENG_Open(DRV_ISR fp_ptr,IPH_CLK clk,UINT32 PclkSel)
{
    PclkSel = PclkSel<<16;
    return IPH_ENG_Open(IPL_HAL_SIE2|PclkSel,fp_ptr,clk);
}

/**
     close engine

     @note : plz reference the note of api: IPH_ENG_Open().

     @param[in]  eng       close indicated engine
     @return Description of data returned.
         - @b E_OK:   engine close
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_Close(IPL_HAL_ENG eng);

/**
     engine start to run

     @note : plz reference the note of api: IPH_ENG_Open().

     @param[in]  eng       start indicated engine
     @return Description of data returned.
         - @b E_OK:   engine start
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_Start(IPL_HAL_ENG eng);
/**
    functional design same as IPH_ENG_Start(), but for ISR only
*/
extern ER IPH_ENG_Start_ISR(IPL_HAL_ENG eng);

/**
     engine start to run and pause engine immediately

     @note : plz reference the note of api: IPH_ENG_Open().

     @param[in]  eng       start indicated engine
     @return Description of data returned.
         - @b E_OK:   engine start
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_StartPause(IPL_HAL_ENG eng);
extern ER IPH_ENG_StartPause_ISR(IPL_HAL_ENG eng);
/**
     stop/pause engine

     @note : plz reference the note of api: IPH_ENG_Open().

     @param[in]  eng       stop indicated engine
     @return Description of data returned.
         - @b E_OK:   engine stop
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_Stop(IPL_HAL_ENG eng);

/**
    functional design same as IPH_ENG_Stop(), but for ISR only
*/
extern ER IPH_ENG_Stop_ISR(IPL_HAL_ENG eng);

/**
     wait indicated signal

     wait indicated signal and then clear flag.

     @note : plz reference the note of api: IPH_ENG_Open().

     @param[in]  eng       indicated engine
     @param[in]  Signal    indicated signal
     @param[in]  bflgclr   clear/non-clear signal before wait
     @return Description of data returned.
         - @b E_OK:   done.
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_WaitSignal(IPL_HAL_ENG eng,IPL_ISR_STATUS Signal,BOOL bflgclr);

/**
     clear indicated signal flag

     @note none

     @param[in]  eng       indicated engine
     @param[in]  Signal    indicated signal
*/
extern ER IPH_ENG_ClrSignal(IPL_HAL_ENG eng,IPL_ISR_STATUS Signal);

/**
     chg IPP_HAL inner parameter set.

     @note:this chg do not configure engine reg. directly.

     @param[in]  id        pipeline index
     @param[in]  idx       indicated param. set
     @param[in]  eng       indicated engine
     @param[in]  Idx       chg indicated param. set
     @param[in]  group     update bit
     @param[in]  pEng      update data
     @return Description of data returned.
         - @b E_OK:   done.
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        SIE sie;
        ...
        IPH_ENG_ChgParam(IPL_ID_1,IPL_HAL_SIE,grp,(void*)&sie);
     }
     @endcode
*/
extern ER IPH_ENG_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_ENG eng,IPL_HAL_GROUP group,void* pEng);
/**
    functional design same as IPH_ENG_ChgParam(), but for ISR only
*/
extern ER IPH_ENG_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_ENG eng,IPL_HAL_GROUP group,void* pEng);
/**
    SIE chg param
*/
_INLINE ER IPH_SIE_ChgParam(IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,SIE*    pSie_mgr)
{
    return IPH_ENG_ChgParam(IPL_ID_1,Idx,IPL_HAL_SIE,group,(void*)pSie_mgr);
}
/**
    SIE chg param,isr only
*/
_INLINE ER IPH_SIE_ChgParam_ISR(IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,SIE*    pSie_mgr)
{
    return IPH_ENG_ChgParam_ISR(IPL_ID_1,Idx,IPL_HAL_SIE,group,(void*)pSie_mgr);
}
/**
    SIE2 chg param
*/
_INLINE ER IPH_SIE2_ChgParam(IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,SIE2*    pSie2_mgr)
{
    return IPH_ENG_ChgParam(IPL_ID_1,Idx,IPL_HAL_SIE2,group,(void*)pSie2_mgr);
}
/**
    SIE2 chg param,isr only
*/
_INLINE ER IPH_SIE2_ChgParam_ISR(IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,SIE2*    pSie2_mgr)
{
    return IPH_ENG_ChgParam_ISR(IPL_ID_1,Idx,IPL_HAL_SIE2,group,(void*)pSie2_mgr);
}
/**
    PRE chg param
*/
_INLINE ER IPH_PRE_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,PRE*    pPre_mgr)
{
    return IPH_ENG_ChgParam(id,Idx,IPL_HAL_PRE,group,(void*)pPre_mgr);
}
/**
    PRE chg param,isr only
*/
_INLINE ER IPH_PRE_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,PRE*    pPre_mgr)
{
    return IPH_ENG_ChgParam_ISR(id,Idx,IPL_HAL_PRE,group,(void*)pPre_mgr);
}
/**
    IPE chg param
*/
_INLINE ER IPH_IPE_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IPE*    pIpe_mgr)
{
    return IPH_ENG_ChgParam(id,Idx,IPL_HAL_IPE,group,(void*)pIpe_mgr);
}
/**
    IPE chg param,isr only
*/
_INLINE ER IPH_IPE_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IPE*    pIpe_mgr)
{
    return IPH_ENG_ChgParam_ISR(id,Idx,IPL_HAL_IPE,group,(void*)pIpe_mgr);
}
/**
    DCE chg param
*/
_INLINE ER IPH_DCE_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,DCE*    pDce_mgr)
{
    return IPH_ENG_ChgParam(id,Idx,IPL_HAL_DCE,group,(void*)pDce_mgr);
}
/**
    DCE chg param,isr only
*/
_INLINE ER IPH_DCE_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,DCE*    pDce_mgr)
{
    return IPH_ENG_ChgParam_ISR(id,Idx,IPL_HAL_DCE,group,(void*)pDce_mgr);
}
/**
    IFE chg param
*/
_INLINE ER IPH_IFE_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IFE*    pIfe_mgr)
{
    return IPH_ENG_ChgParam(id,Idx,IPL_HAL_IFE,group,(void*)pIfe_mgr);
}
/**
    IFE chg param,isr only
*/
_INLINE ER IPH_IFE_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IFE*    pIfe_mgr)
{
    return IPH_ENG_ChgParam_ISR(id,Idx,IPL_HAL_IFE,group,(void*)pIfe_mgr);
}
/**
    IFE2 chg param
*/
_INLINE ER IPH_IFE2_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IFE2*    pIfe2_mgr)
{
    return IPH_ENG_ChgParam(id,Idx,IPL_HAL_IFE2,group,(void*)pIfe2_mgr);
}
/**
    IFE2 chg param,isr only
*/
_INLINE ER IPH_IFE2_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IFE2*    pIfe2_mgr)
{
    return IPH_ENG_ChgParam_ISR(id,Idx,IPL_HAL_IFE2,group,(void*)pIfe2_mgr);
}
/**
    IME chg param
*/
_INLINE ER IPH_IME_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IME*    pIme_mgr)
{
    return IPH_ENG_ChgParam(id,Idx,IPL_HAL_IME,group,(void*)pIme_mgr);
}
/**
    IME chg param,isr only
*/
_INLINE ER IPH_IME_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IME*    pIme_mgr)
{
    return IPH_ENG_ChgParam_ISR(id,Idx,IPL_HAL_IME,group,(void*)pIme_mgr);
}
/**
    RED chg param
*/
_INLINE ER IPH_RDE_ChgParam(IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,RDE*    pRde_mgr)
{
    return IPH_ENG_ChgParam(IPL_ID_1,Idx,IPL_HAL_RDE,group,(void*)pRde_mgr);
}

/**  ENG PPB Set.

     configure ENG PPB address

     @param[in]  id        ipieline index
     @param[in]  eng       indicated engine
     @param[in]  sel       indicated operation
     @param[in]  pEng      updated data
     @return Description of data returned.
         - @b E_OK:   done.
         - @b E_SYS:  operation error.
     @endcode
*/

extern ER IPH_ENG_SetPPB(IPL_PROC_ID id,IPL_HAL_ENG eng,IPL_HAL_PARASET Idx,IPE_PPBSet_SEL sel,void* pEng)
;

/**
    functional design same as IPH_ENG_SetPPB(), but for ISR only
*/

extern ER IPH_ENG_SetPPB_ISR(IPL_PROC_ID id,IPL_HAL_ENG eng,IPL_HAL_PARASET Idx,IPE_PPBSet_SEL sel,void* pEng);


/**
     Load engine reg.

     configure IPL_Hal inner parameter set into engine reg with specified group

     @note : plz reference the note of api: IPH_ENG_Open().

     @param[in]  id        pipeline index
     @param[in]  eng       indicated engine
     @param[in]  Idx       indicated param. set
     @param[in]  group     update bit
     @return Description of data returned.
         - @b E_OK:   done.
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_Load(IPL_PROC_ID id,IPL_HAL_ENG eng,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group);

/**
    functional design same as IPH_ENG_Load(), but for ISR only
*/
extern ER IPH_ENG_Load_ISR(IPL_PROC_ID id,IPL_HAL_ENG eng,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group);
//}

/**
     read IPL_Hal inner table

     @note :none

     @param[in]  id        pipeline index
     @param[in]  eng       indicated engine
     @param[in]  Idx       indicated param. set
     @param[in]  readItem  item to be read
     @param[out] Output    output inner table content,data type plz reference "IPL_HAL_READ" description
     @return Description of data returned.
         - @b E_OK:   done.
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_Read(IPL_PROC_ID id,IPL_HAL_ENG eng,IPL_HAL_PARASET Idx,IPL_HAL_READ readItem,void* Output);
/**
    functional design same as IPH_ENG_Read(), but for ISR only
*/
extern ER IPH_ENG_Read_ISR(IPL_PROC_ID id,IPL_HAL_ENG eng,IPL_HAL_PARASET Idx,IPL_HAL_READ readItem,void* Output);
/**
    dump eng all current parameter set
*/
extern ER IPH_ENG_Dump(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_ENG eng,BOOL all);

/**
     read hw register

     @note none

     @param[in]  eng       indicated engine
     @param[in]  Que       which infor need to be queried
     @param[out] Rst       output register rst,data type plz reference "IPH_QUE" description
     @return Description of data returned.
         - @b E_OK:   done.
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_ReadReg(IPL_HAL_ENG eng,IPH_QUE Que,void* Output);

/**
    functional design same as IPH_ENG_ReadReg(), but for ISR only
*/
extern ER IPH_ENG_ReadReg_ISR(IPL_HAL_ENG eng,IPH_QUE Que,void* Output);

/**
     un-lock update bit

     un-lock Specific update bit to allow relative info being Chg or Load

     @note none

     @param[in]  eng       indicated engine
     @param[out] group     update bit need to be un-locked.
     @return Description of data returned.
         - @b E_OK:   done.
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_BitUnLock(IPL_HAL_ENG eng,IPL_HAL_GROUP group);

/**
     lock update bit

     lock Specific update bit to allow relative info being Chg or Load

     @note none

     @param[in]  eng       indicated engine
     @param[out] group     update bit need to be un-locked.
     @return Description of data returned.
         - @b E_OK:   done.
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_BitLock(IPL_HAL_ENG eng,IPL_HAL_GROUP group);

//@}
#endif
