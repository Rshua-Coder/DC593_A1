#ifndef _IQS_SETTINGFLOW_ID_INT_H_
#define _IQS_SETTINGFLOW_ID_INT_H_

#define IQS_SEMI_EN ENABLE

extern void Install_IQSID(void) _SECTION(".kercfg_text");

extern UINT32 _SECTION(".kercfg_data") IQS_SIESemiId;
extern UINT32 _SECTION(".kercfg_data") IQS_PRESemiId;
extern UINT32 _SECTION(".kercfg_data") IQS_IPESemiId;
extern UINT32 _SECTION(".kercfg_data") IQS_IMESemiId;
extern UINT32 _SECTION(".kercfg_data") IQS_IFESemiId;
extern UINT32 _SECTION(".kercfg_data") IQS_IFE2SemiId;
extern UINT32 _SECTION(".kercfg_data") IQS_DCESemiId;

#if IQS_SEMI_EN
#define WAI_SIESEM (wai_sem(IQS_SIESemiId))
#define SIG_SIESEM (sig_sem(IQS_SIESemiId))
#define WAI_PRESEM (wai_sem(IQS_PRESemiId))
#define SIG_PRESEM (sig_sem(IQS_PRESemiId))
#define WAI_IPESEM (wai_sem(IQS_IPESemiId))
#define SIG_IPESEM (sig_sem(IQS_IPESemiId))
#define WAI_IMESEM (wai_sem(IQS_IMESemiId))
#define SIG_IMESEM (sig_sem(IQS_IMESemiId))
#define WAI_IFESEM (wai_sem(IQS_IFESemiId))
#define SIG_IFESEM (sig_sem(IQS_IFESemiId))
#define WAI_IFE2SEM (wai_sem(IQS_IFE2SemiId))
#define SIG_IFE2SEM (sig_sem(IQS_IFE2SemiId))
#define WAI_DCESEM (wai_sem(IQS_DCESemiId))
#define SIG_DCESEM (sig_sem(IQS_DCESemiId))
#else
#define WAI_SIESEM ()
#define SIG_SIESEM ()
#define WAI_PRESEM ()
#define SIG_PRESEM ()
#define WAI_IPESEM ()
#define SIG_IPESEM ()
#define WAI_IMESEM ()
#define SIG_IMESEM ()
#define WAI_IFESEM ()
#define SIG_IFESEM ()
#define WAI_IFE2SEM ()
#define SIG_IFE2SEM ()
#define WAI_DCESEM ()
#define SIG_DCESEM ()

#endif

#endif
