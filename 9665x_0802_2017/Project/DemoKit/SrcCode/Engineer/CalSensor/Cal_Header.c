#include "Cal_Header_Int.h"
#include "IPL_CAL_Debug.h"

#if   (_IPPLIB_ == _IPL_AR0330M_C12_)

    ER Set2Preview(UINT32 BufAddr,UINT32 BufSize)
    {
        extern ER Set2Preview_AR0330M_C12(UINT32 BufAddr,UINT32 BufSize) ;
        return Set2Preview_AR0330M_C12(BufAddr,BufSize);
    }

    pCAL_ITEM_OBJ CAL_ITEM(void)
    {
        extern pCAL_ITEM_OBJ CAL_ITEM_AR0330M_C12(void) ;
        return CAL_ITEM_AR0330M_C12();
    }

    BOOL CAL_ReadDebugConfig(void)
    {
        extern BOOL CAL_ReadDebugConfig_AR0330M_C12(void);
        return CAL_ReadDebugConfig_AR0330M_C12();
    }
#elif (_IPPLIB_ == _IPL_F22P_C12_FF_)
    ER Set2Preview(UINT32 BufAddr,UINT32 BufSize)
    {
        extern ER Set2Preview_F22P_C12_FF(UINT32 BufAddr,UINT32 BufSize) ;
        return Set2Preview_F22P_C12_FF(BufAddr,BufSize);
    }

    pCAL_ITEM_OBJ CAL_ITEM(void)
    {
        extern pCAL_ITEM_OBJ CAL_ITEM_F22P_C12_FF(void) ;
        return CAL_ITEM_F22P_C12_FF();
    }

    BOOL CAL_ReadDebugConfig(void)
    {
        extern BOOL CAL_ReadDebugConfig_F22P_C12_FF(void);
        return CAL_ReadDebugConfig_F22P_C12_FF();
    }
#else
#endif
