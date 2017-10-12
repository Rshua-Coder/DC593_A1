#ifndef _EXIF_VENDOR_H
#define _EXIF_VENDOR_H
#include "Exif.h"
#include "Type.h"

typedef enum
{
    EXIFVENDOR_CERTIFICATION,
    EXIFVENDOR_QUALITY,
    ENUM_DUMMY4WORD(EXIFVENDOR_DATA_SET)
}EXIFVENDOR_DATA_SET;


extern ER ExifCB(EXIF_EVENT event, PMEM_RANGE pBuffer, UINT32 uiRetParamNum, UINT32 *pParamArray);
extern void ExifVendor_Write0thIFD(EXIF_HANDLE_ID HandleID);
extern void ExifVendor_WriteExifIFD(EXIF_HANDLE_ID HandleID);
extern void ExifVendor_Write0thIntIFD(EXIF_HANDLE_ID HandleID);

extern void ExifVendor_SetData(EXIFVENDOR_DATA_SET attribute, UINT32 value);
extern UINT32 ExifVendor_GetData(EXIFVENDOR_DATA_SET attribute);

extern void ExposureBiasToStr(INT32 Rat_num, INT32 Rat_denum, UINT8 *pstr);
extern void RationalToStr(INT32 Rat_num, INT32 Rat_denum, UINT8 *pstr);
extern void ExposureTimeToStr(INT32 Rat_num, INT32 Rat_denum, UINT8 *pstr);
#endif//_EXIF_VENDOR_H
