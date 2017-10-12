#ifndef _FILEDEF_H
#define _FILEDEF_H

#if 0
#include "DCF.h"


#ifndef FST_STA_OK

#define FSS_OFFSET          (-256)

#ifndef FST_STA_OK
#define FST_STA_OK                  (0)
#define FST_STA_CHG_DIR             (1)     // chage directory condition had happened
#define FST_STA_OVERSIZE            (2)     // file size is small then caller want.
#define FST_STA_TOTAL_FILES         (3)
#define FST_STA_COPIED_ONE          (4)
#define FST_STA_ACCESS_ONE          (4)
#define FST_STA_DELETE_OK_WITH_UNDO (5)
#define FST_STA_ERROR               (-1)
#define FST_STA_NO_FILE             (-2)
#define FST_STA_READING             (-3)
#define FST_STA_WRITING             (-4)
#define FST_STA_BUSY                (-5)    // file system task is busing
#define FST_STA_EXIST_FILE          (-6)
#define FST_STA_UNKNOW_CMD          (-7)    // unknow command
#define FST_STA_PARAM_ERR           (-9)    // the input parameters are wrong
#define FST_STA_NOFREE_SPACE        (-10)   // there are no more free space
#define FST_STA_SIZE_ZERO           (-11)   // this file size is zero
#define FST_STA_TIME_OUT            (-12)   // used of wait command
#define FST_STA_OUT_OF_ID           (-13)   // the DCF ID reach the maximum
#define FST_STA_FAT32               (FSS_FORMAT_FAT32)  // -14
#define FST_STA_DISKINIT_FAIL       (-14)   // this error code split two case
                                            // FST_STA_DISK_UNFORMAT and
                                            // FST_STA_DISK_UNKNOWN_FORMAT
#define FST_STA_DISK_UNFORMAT       (-15)
#define FST_STA_DISK_UNKNOWN_FORMAT (-18)
#define FST_STA_FUNC_NOT_READY      (-16)   // this function not ready
#define FST_STA_CARD_ERR            ((-17)+ FSS_OFFSET)
#define FST_STA_FAT_ERR             (-19)
#define FST_STA_UNUSED_CARD         (-20)
#define FST_STA_NODISK_SPACE        (-21)
#define FST_STA_STORAGE_PROCESSING  (-22)
#define FST_STA_READ_ABORT          (-23)
#endif

#define CHK_VALID_FMT               1  //#NT#2006/05/15  Janice Huang
#endif
// --------------------------------------------------------------------
// DCF standard
// --------------------------------------------------------------------
//#NT#2012/03/08#Lincy Lin -begin
//#NT#
#if 0
#define MIN_DCF_FILE_NUM            (1)
#define MAX_DCF_FILE_NUM            (9999)      // 0x270F although in DCF spec maximum is 9999, here for perfornmance issue we reduce this number
#define MIN_DCF_DIR_NUM             (100)       // 0x0064
#define MAX_DCF_DIR_NUM             (999)       // 0x03E7

#define FST_DCF_FILE_ALL            (0xFFFF)    // used in delete file, this should only be filled in FilenaemID member
#define FST_DCF_DIR_ALL             (0xFFFF)    // used in delete file, this should only be filled in DirectoryID member
#endif
//#NT#2012/03/08#Lincy Lin -end


#endif

#endif
