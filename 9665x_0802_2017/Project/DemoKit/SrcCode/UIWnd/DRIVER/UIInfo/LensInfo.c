////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "PStore.h"
//#include "NVTUserCommand.h"
////////////////////////////////////////////////////////////////////////////////
#if _DEMO_TODO
#include "GxFlash.h"
#include "Lens.h"
#include "sie_lib.h"
#endif

#define LENSINFO_TRACEMSG     0
#define LENSINFO_WARNINGMSG   0
#define LENSINFO_ERRORMSG     0

//Trace Message
#if LENSINFO_TRACEMSG
#define LENSINFO_TRACE(...) debug_msg ("^RLensInfo:>"__VA_ARGS__)
#else
#define LENSINFO_TRACE(...)
#endif

#if LENSINFO_WARNINGMSG
#define LENSINFO_WARNING(...) debug_msg("^GLensInfo:>"__VA_ARGS__)
#else
#define LENSINFO_WARNING(...)
#endif

//Error Message
#if LENSINFO_ERRORMSG
#define LENSINFO_ERROR(...) debug_msg("^BLensInfo:>"__VA_ARGS__)
#else
#define LENSINFO_ERROR(...)
#endif

#define PS_LENS_FLAG DISABLE

#define PS_LENS_DATA            "LEND"
#define PS_STAT_DATA            "STAT"

#define LENSFLAG_DIRTY 0x5555AAAA
#define LENSFLAG_OK    0xAAAA5555

#define LENS_NOTOPEN   0xFF00
#define LENS_SAVEOK    0xFF01
#define LENS_ERROR     0xFF02


typedef struct _PS_SYS_RECORD
{
    UINT32 Flag;
    struct tm Curr_DateTime;
}
PS_SYS_RECORD;

#if _DEMO_TODO
static UINT32  u32lensFlag = ZOOM_FLAG_INSIDE;
UINT32  g_u32lensWriteCount = 0;

BOOL Lens_IsWrite(void)
{
    return (g_u32lensWriteCount!=0);
}
//#NT#2010/10/27#Photon Lin -begin
//#fix typing error
void Set_LensFlag(UINT32 LensFlag)
{
    u32lensFlag = LensFlag;
}

UINT32 Get_LensFlag(void)
{
    return u32lensFlag;
}
//#NT#2010/10/27#Photon Lin -end
#endif

void Save_LensInfo(UINT32 lensInfo)
{
#if _DEMO_TODO
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32 RWOperation = 0;

    PS_SYS_RECORD SysRecord;

//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
    debug_err(("^YWrite LENSINFO\r\n"));
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
    SysRecord.Flag = lensInfo;
    SysRecord.Curr_DateTime = HwClock_GetCurrentTime();


    Save_SystemFlag(LENSFLAG_DIRTY); // set lens flag dirty

    if ((pSection = PStore_OpenSection(PS_LENS_DATA, PS_RDONLY)) == E_PS_SECHDLER)
    {
        RWOperation = PS_RDWR | PS_CREATE;
    }
    else
    {
        PStore_CloseSection(pSection);
        RWOperation = PS_WRONLY ;
    }

    if ((pSection = PStore_OpenSection(PS_LENS_DATA, RWOperation)) != E_PS_SECHDLER)
    {
        // save real lens info
        if(PStore_WriteSection((UINT8 *)&SysRecord, 0, sizeof(SysRecord), pSection)==E_OK)
        {
            if (PStore_CloseSection(pSection)== E_OK)
            {
                Save_SystemFlag(LENSFLAG_OK); // set lens flag ok
                LENSINFO_TRACE("Write lens flag success\r\n");
            }
        }
    }
    else
    {
        LENSINFO_ERROR("Write lens flag error\r\n");
    }
#endif
}

void LensTask2CB(UINT32 MessageID, UINT32 *Parameter)
{
    switch(MessageID)
    {
#if _DEMO_TODO
        case LENS_CB_READ_STATUS:
            *Parameter = u32lensFlag;
        break;
        case LENS_CB_WRITE_STATUS:
            if (g_bIsInitSystemFinish)
            {
                u32lensFlag = *Parameter;
                Save_LensInfo(*Parameter);
            }
            else
            {
                u32lensFlag = *Parameter;
            }
            g_u32lensWriteCount ++;
        break;
//#NT#2010/12/04#Jeffery Chuang -begin
//Add focus move during zoomin/out
        case LENS_CB_FOCUSMOVE:
            //debug_msg("===LENS_CB_FOCUSMOVE=== %d\r\n",*Parameter);
            Lens_Focus_DoAction((FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC),(INT32)*Parameter);
        break;
//#NT#2010/12/04#Jeffery Chuang -end
#endif
        default:
            LENSINFO_ERROR("Parameter error in LensTask2CB() (%d)\r\n",MessageID);
        break;

    }

}
//#NT#2010/09/09#Photon Lin -end


