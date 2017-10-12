
//NOTE!!! these API is used under partial-one!

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "UICommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIResource.h"
#include "UIInfo.h"
//#include "Demo_Logo.h"

#include "Pstore.h"
//#include "PSDrv.h"

#define UIRes_Debug     debug_err

///////////////////////////////////////////////////////////////////////////////
//
//  Wallpaper (JPG)

#define PS_WALLPAPER_IMAGE       "WALL"
#define PS_LOGO_IMAGE            "LOGO"


//#NT#2010/03/01#JeahYen -begin
extern UINT32   p_gLogoJPG;
extern UINT32   p_gLogoSize;
//#NT#2010/03/01#JeahYen -end


void UIRes_InitReadWP_fast(UINT32 id)
{
    UINT32 tempBuf, tmpSize;
    PPSTORE_SECTION_HANDLE  pSection;

    tempBuf = OS_GetMempoolAddr(POOL_ID_GFX_IMAGE);

    if(id == WP_LOGO) //wellcome wallpaper
    {
        p_gLogoJPG = 0;
        p_gLogoSize = 0;

        pSection = PStore_OpenSection(PS_LOGO_IMAGE, PS_RDWR);
        if(pSection == E_PS_SECHDLER)
        {
            debug_err(("[WP] PStore Read WP[%d] fail\r\n", id));
            return;
        }
        //Read File Size(4Bytes)
        PStore_ReadSection((UINT8*)&tmpSize,0,4,pSection);
        PStore_ReadSection((UINT8*)tempBuf,4,tmpSize,pSection);
        PStore_CloseSection(pSection);
        p_gLogoJPG = tempBuf;
        p_gLogoSize = tmpSize;
        UIRes_Debug(("\n[WP] PStore Read WP[%d] size = %d\r\n", id, tmpSize));
    }
}


