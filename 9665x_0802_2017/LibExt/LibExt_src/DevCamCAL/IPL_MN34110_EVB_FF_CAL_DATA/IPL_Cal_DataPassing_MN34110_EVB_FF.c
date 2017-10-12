#include "IPL_CAL_DataPassing_MN34110_EVB_FF_Int.h"
#include "sensor.h"
#include "IPL_CAL_Debug.h"

#define TblSize 2
UINT32 CalSenMode;

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

UINT32 CalSenModeMap(CAL_SECMAP SecID)
{
    if ((SecID == SEC_AWB) || (SecID == SEC_ISO))
    {
        return SENSOR_MODE_2;
    }
    else
    {
        return CalSenMode;
    }
}

#if 0
    - extern
#endif

void SetCalSensorMode(UINT32 Idx)
{
    CalSenMode = Idx;
}


CAL_TAG* GetCalData(CAL_SECMAP SecID)
{
    UINT32 Cal_SenMode;

    Cal_SenMode = CalSenModeMap(SecID);
    return CAL_RW()->fpReadTag(SecID,GetTagID(SecID, Cal_SenMode));
}