#include "IPL_CAL_DataPassing_IMX322_EVB_FF_Int.h"
#include "sensor.h"
#include "IPL_CAL_Debug.h"

#define TblSize 2

static UINT32 SecctionTbl_Tag[][TblSize] = {
    {SEC_AWB,   TAG_AWB_1},

    {SEC_ECS,   TAG_ECS_1},

    {SEC_DP,    TAG_DP_1},

    {SEC_ISO,   TAG_ISO_1},

    {SEC_MSHUT, TAG_MSHUT_1},

    {SEC_AECHK,   TAG_AECHK_1},
};

CAL_TAGID GetTagID(CAL_SECMAP SecID,UINT32 Idx)
{
    UINT32 i;
    UINT32 loop = sizeof(SecctionTbl_Tag)/(sizeof(UINT32)*TblSize);
    //DBG_ERR("not ready\r\n");

    for(i=0;i<loop;i++)
    {
        if(SecctionTbl_Tag[i][0] == SecID)
            return (SecctionTbl_Tag[i][1]+Idx);
    }
    DBG_ERR("no match\r\n");
    return TAG_NONE;
}



#if 0
    - extern
#endif

CAL_TAG* GetCalData(CAL_SECMAP SecID,UINT32 Idx)
{
    return CAL_RW()->fpReadTag(SecID,GetTagID(SecID,Idx));
}