#ifndef _IPL_HAL_STRIPE_INT_H_
#define _IPL_HAL_STRIPE_INT_H_
#include "IPL_HAL_DCE_Info.h"
#include "type.h"
#include "IPL_Utility.h"
#define StripeTblSize   16
#define DCE_STPE_Cal(overlap)     (overlap==H_OVERLAP_16)?(16):((overlap==H_OVERLAP_24)?(24):((overlap==H_OVERLAP_32)?(32):(0)))

typedef struct
{
    UINT32 width;
    UINT32 height;
    UINT32 centX;
    UINT32 centY;
    UINT32 Xdisf;
    UINT32 Ydisf;
}IPH_NormInfo;

typedef struct
{
    UINT32 GDC_NormFact;    ///< Radius normalization factor
    UINT32 GDC_NormBit;     ///< Radius normalization shift bit
}IPH_NormResult;

typedef struct
{
    IPL_PROC_ID id;
    UINT32 width;
    UINT32 height;

    UINT32 centX;
    UINT32 centY;
    UINT32 Xdisf;
    UINT32 Ydisf;

    UINT32 GDC_NormFact;    ///< Radius normalization factor
    UINT32 GDC_NormBit;     ///< Radius normalization shift bit
    BOOL   distsel;
    UINT32 rFactor;
    UINT32 gFactor;
    UINT32 bFactor;
    UINT32 geo_fov_gain;
    UINT32 Holap;           ///< unit:pixel
    DCE_LutType lut_type;
    INT32  *geo_lut;
}IPH_StripeInfo;

//table address must be at the bottom of this structure, because of stripe pre-cal. mech.
typedef struct
{
  UINT32 GDC_MaxInc;
  UINT32 GDC_MaxDec;
  UINT32 STPETBL_Addr;
}IPH_StripeResult;

typedef struct
{
  UINT32 DCE_width;
  UINT32 DCE_height;
  BOOL IMEP1_En;
  UINT32 IMEP1_ScaleOutWidth;
  UINT32 IMEP1_ScaleOutHeight;
  BOOL IMEP2_En;
  UINT32 IMEP2_ScaleOutWidth;
  UINT32 IMEP2_ScaleOutHeight;
  BOOL IMEP3_En;
  UINT32 IMEP3_ScaleOutWidth;
  UINT32 IMEP3_ScaleOutHeight;
  INT32 *GDC_HSTP;
}IPH_StripeChangeInfo;


extern IPH_NormResult IPH_Norm(IPH_NormInfo *NFInfo);
extern ER IPH_Stripe(IPH_StripeInfo *NSInfo, IPH_StripeResult *NSResult);
extern ER IPH_changeStripe(IPH_StripeChangeInfo *StripeInfo);
#endif
