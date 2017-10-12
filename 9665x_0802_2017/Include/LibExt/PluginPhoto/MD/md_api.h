/**
    Motion Detection api.


    @file       md_api.h
    @ingroup    mILibMDAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _MD_API_H_
#define _MD_API_H_

typedef enum _MD_LEVEL
{
    MD_LEVEL_N5     = 125,
    MD_LEVEL_N4     = 120,
    MD_LEVEL_N3     = 115,
    MD_LEVEL_N2     = 110,
    MD_LEVEL_N1     = 105,
    MD_LEVEL_NORMAL = 100,
    MD_LEVEL_P1     = 95,
    MD_LEVEL_P2     = 90,
    MD_LEVEL_P3     = 85,
    MD_LEVEL_P4     = 80,
    MD_LEVEL_P5     = 75,
    MD_LEVEL_MAX,
    ENUM_DUMMY4WORD(MD_LEVEL)
}MD_LEVEL;

extern void MD_SetLevel(MD_LEVEL level);
extern BOOL MD_Process(void);

#endif //_MD_API_H_
