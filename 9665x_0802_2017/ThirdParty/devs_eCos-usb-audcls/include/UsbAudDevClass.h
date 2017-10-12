/**
    Header file for USB-Host Audio Device Class

    This file is the header file for USB USB-Host Audio Device Class

    @file       UsbAudDevClass.h
    @ingroup    mIUSBHost
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _USBAUDDEVCLASS_H_
#define _USBAUDDEVCLASS_H_

#include <cyg/infra/cyg_type.h>


/**
    Audio Device Class callback function prototype
*/
typedef void (*AUDDC_CB)(cyg_uint32 uiEvent);



/**
    Audio Device Class general configuration Selection

    This is used in auddc_setConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum
{
    AUDDC_CONFIG_ID_ATTACH_CALLBACK,    ///< Hook the Callback for class driver attached/detached (audio class driver init done or removed).
                                        ///< Please use AUDDC_CB as prototype. uiEvent=1 is attached. uiEvent=0 is detached.
    AUDDC_CONFIG_ID_BUFDONE_CALLBACK,   ///< Hook the Callback for Buffer Sent Done Callback.
                                        ///< Please use AUDDC_CB as prototype.
    AUDDC_CONFIG_ID_SAMPLE_RATE,        ///< Configure Audio Codec Sample rate. Unit in Hertz.
    AUDDC_CONFIG_ID_CHANNEL_NO,         ///< Configure the source data channel number.
    AUDDC_CONFIG_ID_VOLUME_LVL,         ///< Configure the Volume Total Levels.
    AUDDC_CONFIG_ID_VOLUME,             ///< Configure the Volume
    AUDDC_CONFIG_ID_BTLMIXING,          ///< Enable/Disable the Mixing Stereo to Mono with BTL output. Default Enabled.

}AUDDC_CONFIG_ID;




/**
    Register Audio Device Class Driver to usb host driver.

    This would register three interface driver to usb host driver,
    which are Audio-Control-SubClass / Audio-Stream-SubClass / HID SubClass.
    The registered HID interface driver has no real usages.
*/
extern void         auddc_registerDriver(void);

/**
    Set Audio Device Class Driver general configuration

    Set Audio Device Class Driver general configuration, such as
    Callback-Handler / SamplingRate / Channel-Number / Volume /... etc.

    @param[in] ConfigID     Config Selection. Please refer to AUDDC_CONFIG_ID.
    @param[in] uiCfgValue   Config Value. Depends on ConfigID.

    @return void
*/
extern void         auddc_setConfig(AUDDC_CONFIG_ID ConfigID, cyg_uint32 uiCfgValue);

/**
    Get Audio Device Class Driver general configuration

    Get Audio Device Class Driver general configuration, such as
    Callback-Handler / SamplingRate / Channel-Number / Volume /... etc.

    @param[in] ConfigID     Config Selection. Please refer to AUDDC_CONFIG_ID.

    @return Config Value. Depends on ConfigID.
*/
extern cyg_uint32   auddc_getConfig(AUDDC_CONFIG_ID ConfigID);

/**
    Audio Buffer Playback(/Transmit)

    Audio Buffer Playback(/Transmit). This buffer would be enqueued to driver internal buffer queue.
    After this buffer transfer done, the BUFDONE_CALLBACK would be triggered.

    @param[in] bSt      Trigger start playback.(Kick driver state machine start playbacking.)
    @param[in] uiStadr  Buffer Start Address. Should be word aligned.
    @param[in] uiBufSz  Buffer Size. Should be multiple of 4.

    @return
     - @b FALSE: Enqueue success.
     - @b TRUE:  Error occured.
*/
extern cyg_int32    auddc_transmit(cyg_bool bSt, cyg_uint32 uiStadr, cyg_uint32 uiBufSz);

/**
    Abort/Stop Audio Playback

    Abort/Stop Audio Playback. This would also dequeued all of the buffers.
    This API can be used during audio stop.

    @return void
*/
extern void         auddc_abort(void);

/**
    Pause Audio Playback

    After calling to this api to pause, it would be returned until
    the audio buffer internal playback done to enable pause.
    The latency is about 50ms ~ 100 ms.

    @return void
*/
extern void         auddc_pause(void);

/**
    Resume Audio Playback

    Resume Audio Playback after pause.

    @return void
*/
extern void         auddc_resume(void);

/**
    Wait the audio driver inetrnal queue all finished playback

    Wait the audio driver inetrnal queue all finished playback

    @return void
*/
extern void         auddc_waitIdle(void);

/**
    Check if audio driver is still playbacking?

    Check if audio driver is still playbacking?

    @return
     - @b TRUE:  Playback ongoing.
     - @b FALSE: All queue is done. Means in idle state.
*/
extern cyg_uint32   auddc_isBusy(void);

/**
	Suspend USB

	Suspend USB

    @return void
*/
extern void auddc_suspendUSB(void);

/**
	Resume USB

	Resume USB

    @return void
*/
extern void auddc_resumeUSB(void);
#endif
