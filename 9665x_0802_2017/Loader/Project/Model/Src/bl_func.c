/*
    Main control function

    This file is implement by user mode

    @file       bl_func.c

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include "fuart.h"
#include "fat.h"
#include "rtc.h"
#include "timer.h"
#include "StorageDef.h"
#include "Global.h"
#include "clock.h"
#include "bl_func.h"
#include "Cache.h"
#include "string.h"
#include "BinInfo.h"
#include "pad.h"
#include "gpio.h"

#define bl_memcpy   memcpy
#define bl_memset   memset

extern char _loader_exec_compres_start[];
extern char _load_nand_table_start_base[];
extern char _load_LOADER_CONFIGRAM_FREQ_PARAM_end_base[];

// Function from Timer.c
//extern UINT32 timer_getLdrElapse(void);

// Define in MakeConfig.txt
UINT8 *UPDATE_FW_NAME     = {(UINT8 *)_UPDATE_FW_NAME_};
UINT8 *UPDATE_LOADER_NAME = {(UINT8 *)_UPDATE_LOADER_NAME_};
UINT8 *RUN_FW_NAME        = {(UINT8 *)_RUN_FW_NAME_};
UINT8 *LOADER_START_STR   = {(UINT8 *)_LOADER_START_STR_};
UINT8 *UPDATE_CALFW_NAME  = {(UINT8 *)_UPDATE_CALFW_NAME_};

// Error message
static char FWErrorMsg[]     = "\r\nFW check fail\r\n";
static char RWErrorMsg[]     = "\r\nR/W error\r\n";
static char LoaderErrorMsg[] = "\r\nLoader check fail\r\n";

static UINT32   g_uiBootRevservedNandBufAddr;
static UINT32   g_uiUpdateMainBinBufAddr;       // pointer to store BIN read from card/flash
static UINT32   g_uiUpdateBootloaderBufAddr;    // pointer to store loader BIN read from card
static UINT32   g_uiFWTestBinBufAddr;           // pointer to store compared BIN after programming flash

//#NT#2013/04/25#Steven Wang -begin
//#NT#Reduce Warning
//static UINT32 g_uiUpdateCalBufAddr;
//static UINT32 g_uiRunTestBufAddr;
//#NT#2013/04/25#Steven Wang -end
static UINT32 g_uiTmpBufferAddr;
static UINT32 g_heapBufferAddr;

UINT32 TopOfStack;
UINT32 BaseOfStack;

//---------------------------------------------------------------------------
// Static function
//---------------------------------------------------------------------------
/**
    Display error message.

    Display error message.
    CPU loop forever and LED is red.

    @param Msg: Message to display
    @return void
*/
static void bl_displayErrMsg(char *Msg)
{
    // Display error message to UART
    uart_putSystemUARTStr(Msg);

    // Loop forever
    while (1)
    {
        ;
    }
}

#if 0
/**
    Do power off.

    Do power off.

    @return void
*/
static void bl_turnOffPower(void)
{
    volatile UINT32 uiReg;

    uiReg = INREG32((CPE_RTC_BASE + 0x18));

    // Reset shutdown timer
    OUTREG32(CPE_RTC_BASE + 0x18, uiReg | 0x00000001);
    // Do power off
    OUTREG32(CPE_RTC_BASE + 0x18, uiReg | 0x00000002);

    // Because there is some latency between FW do power off and HW
    // really power off, we have to loop forever to prevent CPU run
    // other instructions.
    while (1)
    {
        ;
    }
}
#endif

#if (SPECIAL_KEY_UPDATE == ENABLE)
/**
    Check special key1(s) is(are) pressed.

    Check special key1(s) is(are) pressed or not.

    @return Key pressed status
        - @b TRUE: Special key(s) is(are) pressed
        - @b FALSE: Otherwise
*/
static BOOL bl_isSpecialKeyPressed(void)
{
    // Detect key example, must press shutter2 and playback mode keys
    // OUT_0: DGPIO 0
    // OUT_1: DGPIO 14
    // OUT_2: DGPIO 2
    // OUT_3: DGPIO 3
    // OUT_4: DGPIO 4
    // IN_0 : DGPIO 5
    // IN_1 : DGPIO 6
    // IN_2 : DGPIO 7
    // IN_3 : DGPIO 8
    //              KEY_IN_0    KEY_IN_1    KEY_IN_2    KEY_IN_3
    // KEY_OUT_0    MENU        SET         PLAYBACK    DISPLAY
    // KEY_OUT_1    WIDE        LEFT        MOVIE
    // KEY_OUT_2    TELE        DOWN                    TRASH
    // KEY_OUT_3    SHUTTER2    RIGHT       CAPTURE
    // KEY_OUT_4    SHUTTER1    UP
    // Add the necessary code..
    return TRUE;
}
#endif


#if (FW_CHECK_METHOD == FW_CHECK_CRC)
/**
    Create CRC table.

    Create CRC table for CRC32 checking.

    @return void
*/
static void bl_makeCRCTable(void)
{
    UINT32 uiValue, i, j;

    for (i=0; i<256; i++)
    {
        uiValue = i;
        for (j=0; j<8; j++)
        {
            uiValue = uiValue & 1 ? 0xEDB88320 ^ (uiValue >> 1) : (uiValue >> 1);
        }
        g_uiCRCTable[i] = uiValue;
    }
}
#endif

/**
    Check FW code.

    Check FW code and file length.
    The FW binary file must be post-proecessd by encrypt_bin.exe.
    This function must sync to encrypt_bin.exe.
    If FW checking is fail, the CPU will loop forever and LED is red.

    @param[in] uiAddress    FW code in DRAM starting address
    @param[in] uiFileLen    FW code length
    @return void
*/
static void bl_checkFW(UINT32 uiAddress, UINT32 uiFileLen)
{

#if (FW_CHECK_METHOD == FW_CHECK_CHECKSUM)
    UINT32 i, uiSum;
  #if (FW_CHECK_PSEUDO_STR_EN == ENABLE)
    UINT32 uiStrLen, uiTempLen;
  #endif
    UINT16 *puiValue;
    UINT32 uiUpdateLength;

    uiSum       = 0;
    puiValue    = (UINT16 *)uiAddress;
    if (INREG32(uiAddress+BININFO_FULLCOMPRESS_OFFSET) == COMPRESS_TAG_VALUE)
    {
        UINT32 uiTemp;

        uart_putSystemUARTStr("FullComp\r\n");
        uiTemp = INREG32(uiAddress+BININFO_FULLCOMPRESS_LENGTH_OFFSET);
        uiUpdateLength = ((uiTemp&0xFF) << 24) |
                            ((uiTemp & 0xFF00) << 8) |
                            ((uiTemp & 0xFF0000) >> 8) |
                            ((uiTemp & 0xFF000000) >> 24);
        uiUpdateLength += LDC_HEADER_SIZE;
    }
    else
    {
        uart_putSystemUARTStr("NonComp\r\n");
        uiUpdateLength = INREG32(uiAddress+BININFO_FW_LENGTH_OFFSET);
    }

  #if (FW_CHECK_PSEUDO_STR_EN == ENABLE)
    uiStrLen = strlen(FW_CHECK_PSEUDO_STR);
    if (uiStrLen > 0)
    {
        uiTempLen = (uiStrLen + 3) & ~3;
        memset((void *)(uiAddress + uiFileLen), 0, uiTempLen);
        memcpy((void *)(uiAddress + uiFileLen), (void *)FW_CHECK_PSEUDO_STR, uiStrLen);
        uiFileLen += uiTempLen;
    }
    else
    {
        uiTempLen   = 0;
    }
  #endif

    //#NT#2012/12/17#HH Chuang -begin
    //#NT#Accelerate checksum calculation
    for (i=0; i<(uiFileLen >> 1); i++)
    {
        uiSum += *(puiValue + i);
    }
    uiSum += ((uiFileLen >> 1) * ((uiFileLen >> 1) - 1)) >> 1;
    //#NT#2012/12/17#HH Chuang -end

    // Checksum and file size
    if (((uiSum & 0x0000FFFF) != 0) ||
        (uiUpdateLength != 0 &&
  #if (FW_CHECK_PSEUDO_STR_EN == ENABLE)
         uiUpdateLength != (uiFileLen - uiTempLen)))
  #else
         uiUpdateLength != uiFileLen))
  #endif
    {
        bl_displayErrMsg(FWErrorMsg);
    }
#elif (FW_CHECK_METHOD == FW_CHECK_CRC)
    UINT32 uiCRC, uiValue;
    UINT8 *puiTemp;
  #if (FW_CHECK_PSEUDO_STR_EN == ENABLE)
    UINT32 uiStrLen, uiTempLen;

    uiStrLen = strlen(FW_CHECK_PSEUDO_STR);
    if (uiStrLen > 0)
    {
        uiTempLen = (uiStrLen + 3) & ~3;
        memset((void *)(uiAddress + uiFileLen), 0, uiTempLen);
        memcpy((void *)(uiAddress + uiFileLen), (void *)FW_CHECK_PSEUDO_STR, uiStrLen);
        uiFileLen += uiTempLen;
    }
  #endif

    // Check binary TAG and length
    if ((*(UINT32 *)(uiAddress + BININFO_BINARY_TAG_OFFSET) != FW_BINARY_TAG) ||
        ((*(UINT32 *)(uiAddress + BININFO_FW_LENGTH_OFFSET) ^ *(UINT32 *)(uiAddress + BININFO_LENGTH_CHECK_OFFSET)) != FW_BINARY_TAG))
    {
        bl_displayErrMsg(FWErrorMsg);
    }

    // Read CRC and reset the CRC value
    uiCRC = *(UINT32 *)(uiAddress + BININFO_CHKSUM_OFFSET);
    *(UINT32 *)(uiAddress + BININFO_CHKSUM_OFFSET) = 0;

    // Check CRC
    uiValue = 0xFFFFFFFF;
    puiTemp = (UINT8 *)uiAddress;
    while (uiFileLen--)
    {
        uiValue = g_uiCRCTable[(uiValue ^ (*puiTemp++)) & 0xFF] ^ (uiValue >> 8);
    }

    if (uiCRC != (uiValue ^ 0xFFFFFFFF))
    {
        bl_displayErrMsg(FWErrorMsg);
    }

    // Restore CRC value
    *(UINT32 *)(uiAddress + BININFO_CHKSUM_OFFSET) = uiCRC;
    return;
#else
    bl_displayErrMsg(FWErrorMsg);
#endif
    return;
}

/**
    Check boot loader code.

    Check boot loader code.

    @return void
*/
static void bl_checkLoader(void)
{
    UINT16  *puiValue, uiSum;
    UINT32  i;

    puiValue    = (UINT16 *)g_uiUpdateBootloaderBufAddr;
    uiSum       = 0;

    for (i=0; i<(LOADER_CODE_SIZE>>1); i++)
    {
        uiSum += (*puiValue + i);
        puiValue++;
    }

    if ((*(UINT16 *)(g_uiUpdateBootloaderBufAddr + LOADER_TAG_OFFSET) != LOADER_TAG_VALUE) ||
        (uiSum != 0))
    {
        bl_displayErrMsg(LoaderErrorMsg);
    }
}

/**
    Toggle_LED_R

    file system will called Toggle_LED_R() after read every sector

    if project want to flash red led during read sd card

    Example

    DATA_READ_OPERATION     : called from each FAT read operation
    DATA_WRITE_OPERATION    : called from each internal storage device(maybe NAND or SPI) write operation

    void Toggle_LED_R(void)
    {
        TurnOn_RedLED();
    }

    @return void
*/
//#NT#2012/12/17#HH Chuang -begin
//#NT#Modify to install LED callback to storage driver
void Toggle_LED_R(void)
//#NT#2012/12/17#HH Chuang -end
{
    static UINT32 uiCnt = 0;
	static BOOL bFristTime = FALSE; //TRUE OPEN WATCH DOG FUNCTION
    // set LED GPIO output direction
    gpio_setDir(P_GPIO_43, GPIO_DIR_OUTPUT);  //This is a sample. refer to real schematic circuit
#if 0			
	{ //drh 20170331
		static UINT32 WatchDog = 0;
		if(bFristTime){
			while(WatchDog < 0x023000FF){
				WatchDog ++ ;
				if((WatchDog % 100000) == 0){
					if(gpio_getPin(P_GPIO_44)){
						gpio_clearPin(P_GPIO_44);
					}
					else{
						gpio_setPin(P_GPIO_44);
					}
				}
			}
			bFristTime = FALSE;
			gpio_clearPin(P_GPIO_44);
		}
	}
#endif

//drh 20170901 disable watchdog
	if(bFristTime)
	{
		{	
			static UINT32 WatchDog_Count = 0;
			gpio_setDir(L_GPIO_12,GPIO_DIR_OUTPUT);
			gpio_setPin(L_GPIO_12);
			WatchDog_Count ++;
			while(WatchDog_Count < 5000){
				WatchDog_Count ++;
			}
			gpio_clearPin(L_GPIO_12);
			bFristTime = FALSE;
		}
	}
			
    switch(uiCnt % 9)
    {
      case 0:
          gpio_setPin(P_GPIO_43);  //This is a sample. refer to real schematic circuit
          break;
      case 5:
          gpio_clearPin(P_GPIO_43); //This is a sample. refer to real schematic circuit
          break;
    }
    uiCnt++;
}

//#NT#2013/04/25#Steven Wang -begin
//#NT#Show Duty calibration log
#if (_LOADER_DUTY_CALIBRATION_ == ENABLE && _LOADER_DUTY_CALIBRATION_LOG_ == ENABLE)
void BL_showROMLog(UINT32 buffer)
{
    UINT32  value;
    UINT32  index;
    UINT32  P_duty;
    UINT32  N_duty;
    UINT32  diff;

    index = 0;
    diff = 0;
    P_duty = 0;
    N_duty = 0;
    uart_putSystemUARTStr("\r\n");
    while(1)
    {
        value = INREG32(buffer + index);

        if((value & 0xF0000000) == 0xF0000000)
        {
            value &= ~(0x40000000);
            uart_putSystemUARTStr(Dec2HexStr(value));
            uart_putSystemUARTStr(" = ");
        }

        else if((value & 0xF0000000) == 0x50000000)
        {
            uart_putSystemUARTStr(Dec2HexStr(value & 0xFFFFFFF));
            uart_putSystemUARTStr("\r\n");
        }

        else if((value & 0xF0000000) == 0x20000000)
        {
            uart_putSystemUARTStr("P_duty = ");
            P_duty = value & 0xFFFFFFF;
            //uart_putSystemUARTStr(Dec2HexStr(value & 0xFFFFFFF));
            uart_putSystemUARTStr(Dec2HexStr(P_duty));
            uart_putSystemUARTStr("\r\n");
        }

        else if((value & 0xF0000000) == 0x30000000)
        {
            uart_putSystemUARTStr("N_duty = ");
            N_duty = value & 0xFFFFFFF;
            //uart_putSystemUARTStr(Dec2HexStr(value & 0xFFFFFFF));
            uart_putSystemUARTStr(Dec2HexStr(N_duty));
            uart_putSystemUARTStr("\r\n");
            if(P_duty > N_duty)
            {
                if(diff == 2)
                {
                    uart_putSystemUARTStr("============\r\n");
                    diff = 3;
                }
                diff = 1;
            }
            else
            {
                if(diff == 1)
                {
                    uart_putSystemUARTStr("============\r\n");
                    diff = 3;
                }
                diff = 2;
            }
        }
        else if((value & 0xF0000000) == 0x60000000)
        {
            uart_putSystemUARTStr("N+P = ");
            uart_putSystemUARTStr(Dec2HexStr(value & 0xFFFFFFF));
            uart_putSystemUARTStr("\r\n");
        }
        else if((value & 0xF0000000) == 0x70000000)
        {
            if((value & 0xFFFFFFF) == 0x0654321)
                break;
        }
        else
        {
            uart_putSystemUARTStr("Unknow \r\n");
        }
        index += 4;
    }
    uart_putSystemUARTStr("\r\n[0xB0001000] = 0x");
    uart_putSystemUARTStr(Dec2HexStr(INREG32(0xB0001000)));
    uart_putSystemUARTStr("  [0xB000101C] = 0x");
    uart_putSystemUARTStr(Dec2HexStr(INREG32(0xB000101C)));
    uart_putSystemUARTStr("\r\n[0xB00011D0] = 0x");
    uart_putSystemUARTStr(Dec2HexStr(INREG32(0xB00011D0)));
    uart_putSystemUARTStr("  [0xB00011D4] = 0x");
    uart_putSystemUARTStr(Dec2HexStr(INREG32(0xB00011D4)));
    uart_putSystemUARTStr("\r\n");
}
#endif
//#NT#2013/04/25#Steven Wang -end


#if (DRAM_RANGE_SCAN_EN == ENABLE)
/**
    Check Sram code.

    Check is dram scan code or not

    @param[in] uiAddress    Scan FW code in DRAM starting address
    @return void
*/
static BOOL bl_checkDramScanFW(UINT32 uiAddress)
{
    if((*(UINT32 *)uiAddress & SRAM_TAG) == SRAM_TAG)
        return TRUE;
    else
        return FALSE;
}
#endif

#if (STORAGEINT_SPI == 1)
static BOOL bl_spiIdentify(UINT32 uiMfgID, UINT32 uiTypeID, UINT32 uiCapacityID, PSPI_IDENTIFY pIdentify)
{
    // Sample to support SST25VF032
    if ((uiMfgID == 0xBF) && (uiCapacityID == 0x4A))
    {
        pIdentify->bDualRead = FALSE;
        pIdentify->bSupportAAI = TRUE;
        pIdentify->bSupportEWSR = TRUE;
        pIdentify->uiFlashSize = 4*1024*1024;
        return TRUE;
    }
    else if ((uiMfgID == 0x8C) && (uiTypeID == 0x41) && (uiCapacityID == 0x17)) // ESMT
    {
        pIdentify->bDualRead = FALSE;
        pIdentify->uiQuadReadType = SPI_QUAD_TYPE1;
        pIdentify->bSupportAAI = FALSE;
        pIdentify->bSupportEWSR = FALSE;
        pIdentify->uiFlashSize = 8*1024*1024;
        return TRUE;
    }

    return FALSE;
}
#endif

/**
    main flow code

    If there is Calibration Firmware code store in NAND, running flow as follow
    O's work flow

    @return int
*/
int bl_mainFlow(void)
{
    //#NT#2013/04/25#Steven Wang -begin
    //#NT#Reduce warning
    //UINT32  uiPowerOnSrc = 0;
    //#NT#2013/04/25#Steven Wang -end
    UINT32  uiNandBlkSize = 0;
    UINT32  uiUpdateFileLen = 0;
    UINT32  uiFWLen = 0;
    UINT32  uiNANDReservedBadBlockCnt = 0;
    UINT32  uiLoaderFunc = 0;
    UINT32  uiTmpCompressFWLen = 0;
#if (STORAGEINT_SPI == 1)
    UINT32  uiTotalFlashSize;
#endif
	UINT32   iiii;

    //#NT#2013/04/25#Steven Wang -begin
    //#NT#Show Duty calibration log
    INT32   iResult;
    UINT32  uiStorageVersion = (UINT32)&_load_nand_table_start_base;
    //UINT32  uiStorageFlag = (UINT32)&_load_LOADER_CONFIGRAM_FREQ_PARAM_end_base;
#if (_LOADER_DUTY_CALIBRATION_ == ENABLE && _LOADER_DUTY_CALIBRATION_LOG_ == ENABLE)
    UINT32  uiLoaderAddress;
    UINT32  uiLogSramAddress;
#endif
    //#NT#2013/04/25#Steven Wang -end

    // BaseOfStack is initialized at doRemapLZ.s
    g_heapBufferAddr             = BaseOfStack + 0x4000;        // reserve 16KB for tmp buffer usage
//    g_heapBufferAddr             = BaseOfStack + 0x4000 + LOADER_TMP_BUFFER_SIZE;   // reserve 16KB for tmp buffer usage
    g_uiTmpBufferAddr            = g_heapBufferAddr + FAT_HEAP_BUFFER_SIZE;

    g_uiUpdateBootloaderBufAddr  = g_uiTmpBufferAddr + 0x4000;
    g_uiBootRevservedNandBufAddr = g_uiUpdateBootloaderBufAddr  + LOADER_CODE_SIZE;
    g_uiUpdateMainBinBufAddr     = SDRAM_Start_FW;
//    g_uiUpdateMainBinBufAddr     = g_uiBootRevservedNandBufAddr + RESERVED_DATA_SIZE;
    g_uiFWTestBinBufAddr         = g_uiUpdateMainBinBufAddr + FW_MAX_CODE_SIZE;
//Philex modify for carDV --begin
     gpio_setDir(P_GPIO_50, GPIO_DIR_OUTPUT);  //This is a sample. refer to real schematic circuit

	gpio_clearPin(P_GPIO_50); //This is a sample. refer to real schematic circuit

   gpio_setDir(P_GPIO_47, GPIO_DIR_OUTPUT);  //This is a sample. refer to real schematic circuit
	gpio_setPin(P_GPIO_47); //This is a sample. refer to real schematic circuit

	//for(iiii=0;iiii<2000;iiii++); //drh 20170718 removie

	for(iiii=0;iiii<200;iiii++);
	gpio_setPin(P_GPIO_50); //This is a sample. refer to real schematic circuit

#if 0	
	gpio_setDir(L_GPIO_12,GPIO_DIR_OUTPUT);
	gpio_setPin(L_GPIO_12);
	for(iiii=0;iiii<200;iiii++);
	gpio_clearPin(L_GPIO_12);
#endif

    flash_installAccessCB(Toggle_LED_R);
//Philex modify for carDV --end

    // UART initial sequence
    uart_openSystemUART();

    // rtc reset shutdown timer
    rtc_resetShutDownTimer();

#if 0
    // adjust PAD driving (if required)
    bl_adjustDriving();
    /*
        - @b RTC_PWR_SW_STS: Power on source is PWR_SW
        - @b RTC_PWR_VBAT_STS: Power on source is PWR_VBAT
        - @b RTC_PWR_VBUS_STS: Power on source is PWR_VBUS
    */
    uiPowerOnSrc = rtc_getPWRONSource();

    if(uiPowerOnSrc & RTC_PWR_SW_STS)
        uart_putSystemUARTStr("\r\nSW PON\r\n");
    else if(uiPowerOnSrc & RTC_PWR_VBAT_STS)
        uart_putSystemUARTStr("\r\nVBAT PON\r\n");
    else if(uiPowerOnSrc & RTC_PWR_VBUS_STS)
        uart_putSystemUARTStr("\r\nVBUS PON\r\n");
    else if (rtc_getIsAlarmPowerOn())
        uart_putSystemUARTStr("\r\nPwrAlarm PON\r\n");
    else
        uart_putSystemUARTStr("\r\nPOR PON\r\n");
#endif
    // Display Loader Version
    uart_putSystemUARTStr((char *)LOADER_START_STR);
    uart_putSystemUARTStr(_LOADER_VERSION_STR_);

    UTL_setDrvTmpBufferAddress(g_uiTmpBufferAddr);

#if (STORAGEINT_SPI == 1)
    // Sample to hook spi flash extending function
    //pad_setDrivingSink(PAD_DS_CGPIO2, PAD_DRIVINGSINK_10MA); // enable it if using SPI 4 bit read
    flash_installIdentifyCB(bl_spiIdentify);
    //flash_setFrequency(SPI_CLOCK_RATE_80_MHZ); // enable it if using SPI 4 bit read
    //flash_setConfig(FLASH_CFG_ID_SPI_SUPPORT_4BITS, TRUE); // enable it if using SPI 4 bit read
#endif

#if (SPECIAL_KEY_UPDATE == ENABLE)
    // Check if special key(s) is(are) pressed
    if (bl_isSpecialKeyPressed() == TRUE)
    {
#endif
        if(bl_checkCardExist())
        {
            if(card_open() == TRUE && fat_initFAT(g_heapBufferAddr, FAT_HEAP_BUFFER_SIZE) == TRUE)
            {
                // Update loader or not, loader is fixed to 16 KB
                if(fat_open_rootfile(UPDATE_LOADER_NAME) == TRUE)
                {
                    // Read byte count specified in file directory entry
                    fat_read_rootfile((UINT8 *)g_uiUpdateBootloaderBufAddr, FAT_READ_TOTAL_FILE_LENGTH);
                    fat_close_rootfile();
                    uiLoaderFunc |= FUNC_UPDATE_LOADER;
                }
                // "Update FW" or "Run FW" function
                //  Update FW has higher priority
                if(fat_open_rootfile(UPDATE_FW_NAME) == TRUE)
                {
#if 1
                    // Read 1st 512 bytes from file UPDATE_FW_NAME
                    // offset BININFO_FW_LENGTH_OFFSET will hold valid file size filled by encrypt bin
                    fat_read_rootfile((UINT8 *)g_uiUpdateMainBinBufAddr, 0x200);
                    // Read byte count specified in offset BININFO_FW_LENGTH_OFFSET
                    uiUpdateFileLen = fat_read_rootfile((UINT8 *)g_uiUpdateMainBinBufAddr, INREG32(g_uiUpdateMainBinBufAddr + BININFO_FW_LENGTH_OFFSET));
#else
                    // Read byte count specified in file directory entry
                    uiUpdateFileLen = fat_read_rootfile((UINT8 *)g_uiUpdateMainBinBufAddr, FAT_READ_TOTAL_FILE_LENGTH);
#endif
                    fat_close_rootfile();
                    uiLoaderFunc |= FUNC_UPDATE_FW;
                }
                // Run FW has lower priority
                else if(fat_open_rootfile(RUN_FW_NAME) == TRUE)
                {
#if 1
                    // Read 1st 512 bytes from file RUN_FW_NAME
                    // offset BININFO_FW_LENGTH_OFFSET will hold valid file size filled by encrypt bin
                    fat_read_rootfile((UINT8 *)g_uiUpdateMainBinBufAddr, 0x200);
                    if(INREG32(g_uiUpdateMainBinBufAddr + BININFO_FULLCOMPRESS_OFFSET) == COMPRESS_TAG_VALUE)   // Full compressed FW
                    {
                        // Adjust memory location
                        // g_uiUpdateMainBinBufAddr = SDRAM_Start_FW (0x8000_0000) + Comressed FW Len (length before compressed)
                        uiTmpCompressFWLen = INREG32(g_uiUpdateMainBinBufAddr + BININFO_FULLCOMPRESS_LENGTH_OFFSET);
                        uiUpdateFileLen = ((uiTmpCompressFWLen & 0x000000FF) << 24) |  ((uiTmpCompressFWLen & 0x0000FF00) << 8) | ((uiTmpCompressFWLen & 0x00FF0000) >> 8) | ((uiTmpCompressFWLen & 0xFF000000) >> 24);
                        uiUpdateFileLen = ((uiUpdateFileLen+LDC_HEADER_SIZE+3)>>2) << 2;
                        g_uiUpdateMainBinBufAddr = SDRAM_Start_FW + uiUpdateFileLen;
                    }
                    // Read byte count specified in offset BININFO_FW_LENGTH_OFFSET
                    uiUpdateFileLen = fat_read_rootfile((UINT8 *)g_uiUpdateMainBinBufAddr, INREG32(g_uiUpdateMainBinBufAddr + BININFO_FW_LENGTH_OFFSET));
#else
                    // Read byte count specified in file directory entry
                    uiUpdateFileLen = fat_read_rootfile((UINT8 *)g_uiUpdateMainBinBufAddr, FAT_READ_TOTAL_FILE_LENGTH);
#endif
                    fat_close_rootfile();
                    uiLoaderFunc |= FUNC_RUN_CARD;
                }
            }
            else
            {
                uart_putSystemUARTStr("\r\nfail\n\r");
            }
            card_close();
        }
#if (SPECIAL_KEY_UPDATE == ENABLE)
    }
#endif

//#NT#2013/04/25#Steven Wang -begin
//#NT#Show Duty calibration log
#if (_LOADER_DUTY_CALIBRATION_ == ENABLE && _LOADER_DUTY_CALIBRATION_LOG_ == ENABLE)
    //Calibration enable + log enable
    uiLoaderAddress = (UINT32)&_load_BOOT_EXEC_start_base;

    if((INREG32(uiLoaderAddress + 0x38) & 0x80000) == 0x80000)
    {
        uiLogSramAddress = INREG32(uiLoaderAddress + 0x0C);

        if((uiLogSramAddress & 0x40000000) == 0x40000000)
        {
            uiLogSramAddress = uiLogSramAddress & ~(0x40000000);
            uart_putSystemUARTStr(Dec2HexStr(uiLogSramAddress));
            BL_showROMLog(uiLogSramAddress);
        }
    }
#endif
//#NT#2013/04/25#Steven Wang -end

    // Check FW read from SD card
    // The FW binary file must be post-processed by encrypt_bin.exe
    if (uiLoaderFunc & (FUNC_RUN_CARD | FUNC_UPDATE_FW))
    {
        #if (DRAM_RANGE_SCAN_EN == ENABLE)
        // First word is code entry point address, once if entry address is 0xB000XXXX
        // represent code is running on sram.
        if(bl_checkDramScanFW(g_uiUpdateMainBinBufAddr) == TRUE)
        {
            UINT32 i;

            // Enable sram usage
            SETREG32(0xb0900128, 0x00000002);
            SETREG32(0xb0800128, 0x00000006);
            SETREG32(0xB0020060, 0x00030002);

            uiUpdateFileLen = ((uiUpdateFileLen + 3) & 0xFFFFFFFC);

            for(i = 0; i < uiUpdateFileLen; i += 4)
            {
                *(UINT32 *)(SRAM_Start_FW + i) = *(UINT32 *)(g_uiUpdateMainBinBufAddr + i);
            }
            *(UINT32 *)(SDRAM_Start_FW) = *(UINT32 *)(g_uiUpdateMainBinBufAddr);
            return TRUE;
        }
        else
        #endif
        {
#if (FW_CHECK_METHOD == FW_CHECK_CRC)
            bl_makeCRCTable();
#endif
            bl_checkFW(g_uiUpdateMainBinBufAddr, uiUpdateFileLen);
        }
    }

    // Init internal flash : NAND or Serial flash
    // Choose by MakeConfig.txt
    iResult = flash_open();
    if(iResult >= 0)
    {
        OUTREG32(NAND_TABLE_VERSION_ADDR, INREG32(uiStorageVersion));

        if(iResult == E_OK)
        {
            OUTREG32(NAND_TABLE_FLAG_ADDR, 0x46495053); //'S''P''I''F'
        }
        else // E_OK_TABLE_FOUND(1) or E_OK_TABLE_NOT_FOUND(2)
        {
            OUTREG32(NAND_TABLE_FLAG_ADDR, iResult);
        }

        uiNandBlkSize = flash_getBlockSize();

#if (STORAGEINT_SPI == 1)
        uiTotalFlashSize = flash_getTotalSize();
#endif

        // Update loader, fixed to 16 KB
        if (uiLoaderFunc & FUNC_UPDATE_LOADER)
        {
            // Check boot loader read from SD card
            bl_checkLoader();

            uart_putSystemUARTStr("\r\nUa Ldr\r\n");

            // Program loader
            if(flash_writeSectors(StartNandBlkUpdateLoader, LOADER_CODE_SIZE, (UINT8 *)g_uiUpdateBootloaderBufAddr, NAND_RW_LOADER) < 0)
                bl_displayErrMsg(RWErrorMsg);

            // Read back
            g_uiFWTestBinBufAddr = g_uiUpdateBootloaderBufAddr + LOADER_CODE_SIZE;
            if (flash_readSectors(StartNandBlkUpdateLoader, LOADER_CODE_SIZE, (UINT8*)g_uiFWTestBinBufAddr, NAND_RW_LOADER) < 0)
            {
                bl_displayErrMsg("rd fail\r\n");
            }
            // Verify
            if (memcmp((void*)g_uiUpdateBootloaderBufAddr, (void*)g_uiFWTestBinBufAddr, LOADER_CODE_SIZE) != 0)
            {
                bl_displayErrMsg("verify fail\r\n");
            }

        }
        // Update FW
        if (uiLoaderFunc & FUNC_UPDATE_FW)
        {
            uart_putSystemUARTStr("\r\nUd FW\r\n");

#if (_MAINCODE_COMPRESS_ == 1)
            if(INREG32(g_uiUpdateMainBinBufAddr+BININFO_FULLCOMPRESS_OFFSET) != COMPRESS_TAG_VALUE)
            {
                // FW not full compressed (partial compressed OR not comporessed)
                uiFWLen = INREG32(g_uiUpdateMainBinBufAddr + BININFO_FW_LENGTH_OFFSET);

#if (STORAGEINT_SPI == 1)
                if ((uiFWLen+LOADER_CODE_SIZE) > uiTotalFlashSize)
                {
                    uart_putSystemUARTStr("!!FW CODE SIZE > FLASH SIZE!!\r\n");
                    while (1);
                }
#endif
            }
            else
            {
                // FW is full compressed
                // Total fill len = OUTPUT length + 16(header length)
                uiTmpCompressFWLen = INREG32(g_uiUpdateMainBinBufAddr + BININFO_FULLCOMPRESS_LENGTH_OFFSET);

                uiFWLen = ((uiTmpCompressFWLen & 0x000000FF) << 24) |  ((uiTmpCompressFWLen & 0x0000FF00) << 8) | ((uiTmpCompressFWLen & 0x00FF0000) >> 8) | ((uiTmpCompressFWLen & 0xFF000000) >> 24);

                uiFWLen += LDC_HEADER_SIZE;
            }
#else   // (_MAINCODE_COMPRESS_ == 0)
            uiFWLen = INREG32(g_uiUpdateMainBinBufAddr + BININFO_FW_LENGTH_OFFSET);
#if (STORAGEINT_SPI == 1)
            if ((uiFWLen + LOADER_CODE_SIZE) > uiTotalFlashSize)
            {
                uart_putSystemUARTStr("!!FW CODE SIZE > FLASH SIZE!!\r\n");
                while (1);
            }
#endif
#endif  // end of (_MAINCODE_COMPRESS_ == 1)

            // NAND Maximum Bad block replacement count = (FW_CODE_SIZE - FWLen) / blkSize

            //#NT#2010/11/17#Steven Wang -begin
            //#NT#Reserved block for bad block replacement shell - StartNandBlkUpdateFW
            uiNANDReservedBadBlockCnt = ((FW_MAX_CODE_SIZE - uiFWLen) / uiNandBlkSize) - StartNandBlkUpdateFW;
            //#NT#2010/11/17#Steven Wang -end

            flash_setReservedBadBlockNumber(uiNANDReservedBadBlockCnt);

            if(flash_writeSectors(StartNandBlkUpdateFW, uiFWLen, (UINT8 *)g_uiUpdateMainBinBufAddr, NAND_RW_FIRMWARE) < 0)
                bl_displayErrMsg(RWErrorMsg);

            g_uiFWTestBinBufAddr = ((g_uiUpdateMainBinBufAddr + uiFWLen + 3)>>2) << 2;
            if (flash_readSectors(StartNandBlkUpdateFW, uiFWLen, (UINT8*)g_uiFWTestBinBufAddr, NAND_RW_FIRMWARE) < 0)
            {
                bl_displayErrMsg("rd fail\r\n");
            }
            if (memcmp((void*)g_uiUpdateMainBinBufAddr, (void*)g_uiFWTestBinBufAddr, uiFWLen) != 0)
            {
                bl_displayErrMsg("verify fail\r\n");
            }

            uiLoaderFunc |= FUNC_UPDATE_FW_DONE;
        }

        // Run Internal Storage
        if (! (uiLoaderFunc & FUNC_RUN_CARD))  // Not run card => i.e. run flash
        {
            uart_putSystemUARTStr("\r\nRFlsh\n\r");
            uiLoaderFunc |= FUNC_RUN_FLASH;
            // Read first block in NAND
            if(flash_readSectors(StartNandBlkUpdateFW, uiNandBlkSize, (UINT8 *)SDRAM_Start_FW, NAND_RW_FIRMWARE) < 0)
                bl_displayErrMsg(RWErrorMsg);
#if (_MAINCODE_COMPRESS_ == 1)
            if(INREG32(SDRAM_Start_FW+BININFO_FULLCOMPRESS_OFFSET) != COMPRESS_TAG_VALUE)   // FW NOT full compressed
            {
                // If != 0 --> partial loading flag exist
                if (INREG32(SDRAM_Start_FW+BININFO_LDCTRL_OFFSET) & LDCTRL_PARTIAL_LOAD_MSK)
                {
                    uiUpdateFileLen = *(volatile UINT32 *)(SDRAM_Start_FW + BININFO_FW_PART1_SIZE_OFFSET);
                    //#NT#2009/10/12#Steven Wang -begin
                    //#NT#Align partial load length to block boundary.
                    //#NT#And re fill into partial load offset
                    if((uiUpdateFileLen % uiNandBlkSize) != 0)
                    {
                        uiUpdateFileLen = ((uiUpdateFileLen / uiNandBlkSize) + 1) * uiNandBlkSize;
                    }
                    //#NT#2009/10/12#Steven Wang -end
                    uart_putSystemUARTStr("\r\nPL\n\r");
                    if(flash_readSectors(StartNandBlkUpdateFW + 1, (uiUpdateFileLen - uiNandBlkSize), (UINT8 *)(SDRAM_Start_FW + uiNandBlkSize), NAND_RW_FIRMWARE) < 0)
                        bl_displayErrMsg(RWErrorMsg);
                }
                // If = 0 --> using full loading
                else
                {
                    uart_putSystemUARTStr("\r\nFL\n\r");
                    uiUpdateFileLen = *(volatile UINT32 *)(SDRAM_Start_FW + BININFO_FW_LENGTH_OFFSET);
                    if(uiUpdateFileLen == 0)
                        uiUpdateFileLen = FW_MAX_CODE_SIZE - (4 * uiNandBlkSize);

                    if(flash_readSectors(StartNandBlkUpdateFW + 1, (uiUpdateFileLen - uiNandBlkSize), (UINT8 *)(SDRAM_Start_FW + uiNandBlkSize), NAND_RW_FIRMWARE) < 0)
                        bl_displayErrMsg(RWErrorMsg);
                }
            }
            else    // FW full compressed
            {
                uart_putSystemUARTStr("\r\nFCompress\n\r");

                uiTmpCompressFWLen = INREG32(SDRAM_Start_FW + BININFO_FULLCOMPRESS_LENGTH_OFFSET);

                uiUpdateFileLen = ((uiTmpCompressFWLen & 0x000000FF) << 24) |  ((uiTmpCompressFWLen & 0x0000FF00) << 8) | ((uiTmpCompressFWLen & 0x00FF0000) >> 8) | ((uiTmpCompressFWLen & 0xFF000000) >> 24);

                if(uiUpdateFileLen == 0)
                {
                    bl_displayErrMsg(RWErrorMsg);
                }
                else
                {
                    g_uiFWTestBinBufAddr = ((SDRAM_Start_FW + (uiUpdateFileLen + LDC_HEADER_SIZE) + 3)>>2) << 2;
                    if(flash_readSectors(StartNandBlkUpdateFW, (uiUpdateFileLen + LDC_HEADER_SIZE), (UINT8 *)(g_uiFWTestBinBufAddr), NAND_RW_FIRMWARE) < 0)
                        bl_displayErrMsg(RWErrorMsg);

                    // Decompress loader image
                    LZ_Uncompress((unsigned char *)(g_uiFWTestBinBufAddr + LDC_HEADER_SIZE), (unsigned char *)(SDRAM_Start_FW), uiUpdateFileLen);

                    uiUpdateFileLen = INREG32(SDRAM_Start_FW + BININFO_FW_LENGTH_OFFSET);

#if (FW_CHECK_METHOD != FW_CHECK_NOCHECK)
                    // Not necessary to checksum each time f/w loaded
                    // Only checksum when f/w is programed first time
                    if (uiLoaderFunc & FUNC_UPDATE_FW)
                    {
                        bl_checkFW(SDRAM_Start_FW, uiUpdateFileLen);
                    }
#endif
                }
            }
#else   // (_MAINCODE_COMPRESS_ == 0)
            // Read partial loading flag in FW binary file
            uiUpdateFileLen = *(volatile UINT32 *)(SDRAM_Start_FW + BININFO_FW_PART1_SIZE_OFFSET);

            // If != 0 --> partial loading flag exist
            if(INREG32(SDRAM_Start_FW + BININFO_LDCTRL_OFFSET) & LDCTRL_PARTIAL_LOAD_MSK)
            {
                //#NT#2009/10/12#Steven Wang -begin
                //#NT#Align partial load length to block boundary.
                //#NT#And re fill into partial load offset
                if((uiUpdateFileLen % uiNandBlkSize) != 0)
                {
                    uiUpdateFileLen = ((uiUpdateFileLen / uiNandBlkSize) + 1) * uiNandBlkSize;
///                    OUTREG32(SDRAM_Start_FW + FW_PARTIAL_LOADING_OFFSET, uiUpdateFileLen);
                }
                //#NT#2009/10/12#Steven Wang -end

                uart_putSystemUARTStr("\r\nPL\n\r");
                if(flash_readSectors(StartNandBlkUpdateFW + 1, (uiUpdateFileLen - uiNandBlkSize), (UINT8 *)(SDRAM_Start_FW + uiNandBlkSize), NAND_RW_FIRMWARE) < 0)
                    bl_displayErrMsg(RWErrorMsg);
            }
            // If = 0 --> using full loading
            else
            {
                uart_putSystemUARTStr("\r\nFL\n\r");
                uiUpdateFileLen = *(volatile UINT32 *)(SDRAM_Start_FW + BININFO_FW_LENGTH_OFFSET);
                if(uiUpdateFileLen == 0)
                    uiUpdateFileLen = FW_MAX_CODE_SIZE;

                if(flash_readSectors(StartNandBlkUpdateFW + 1, (uiUpdateFileLen - uiNandBlkSize), (UINT8 *)(SDRAM_Start_FW + uiNandBlkSize), NAND_RW_FIRMWARE) < 0)
                    bl_displayErrMsg(RWErrorMsg);
            }
#endif  // end of (_MAINCODE_COMPRESS_ == 1)

        }

        //uart_putSystemUARTStr(_LOADER_VERSION_STR_);

        flash_close();
    }
    else
    {
        // debug
        uart_putSystemUARTStr("flash open fail\r\n");
    }

    // Run FW
    if (uiLoaderFunc & FUNC_RUN_CARD)
    {
        uart_putSystemUARTStr("\r\nRC\n\r");

#if (_MAINCODE_COMPRESS_ == 1)
        if(INREG32(g_uiUpdateMainBinBufAddr + BININFO_FULLCOMPRESS_OFFSET) != COMPRESS_TAG_VALUE) // FW is NOT fully compressed
        {
            if ((INREG32(g_uiUpdateMainBinBufAddr + BININFO_LDCTRL_OFFSET) & LDCTRL_PARTIAL_LOAD_MSK) &&
                    (INREG32(g_uiUpdateMainBinBufAddr + BININFO_FW_COMPRESS_OFFSET) & FW_COMPRESS_MSK))
            {
                uart_putSystemUARTStr("\r\nPCmp\n\r");

                uiUpdateFileLen = INREG32(g_uiUpdateMainBinBufAddr + BININFO_FW_PART1_SIZE_OFFSET);
                // Copy to DRAM FW starting address (already at SDRAM_Start_FW, skip copy)
///                bl_memcpy((void *)SDRAM_Start_FW, (void *)g_uiUpdateMainBinBufAddr, uiUpdateFileLen);
            }
            else
            {
                uart_putSystemUARTStr("\r\nNrml\n\r");

                // Copy to DRAM FW starting address (already at SDRAM_Start_FW, skip copy)
///                bl_memcpy((void *)SDRAM_Start_FW, (void *)g_uiUpdateMainBinBufAddr, uiUpdateFileLen);
            }
            //OUTREG32((SDRAM_Start_FW + FW_RESERVED_SIZE_OFFSET), FW_CODE_SIZE);
        }
        else // Run Card and fully compress
        {
            uart_putSystemUARTStr("\r\nF compress\n\r");

            uiTmpCompressFWLen = INREG32(g_uiUpdateMainBinBufAddr + BININFO_FULLCOMPRESS_LENGTH_OFFSET);

            uiFWLen = ((uiTmpCompressFWLen & 0x000000FF) << 24) |  ((uiTmpCompressFWLen & 0x0000FF00) << 8) | ((uiTmpCompressFWLen & 0x00FF0000) >> 8) | ((uiTmpCompressFWLen & 0xFF000000) >> 24);

            LZ_Uncompress((UINT8 *)(g_uiUpdateMainBinBufAddr + LDC_HEADER_SIZE), (UINT8 *)SDRAM_Start_FW, (uiFWLen));

            uiUpdateFileLen = INREG32(SDRAM_Start_FW + BININFO_FW_LENGTH_OFFSET);

            #if (FW_CHECK_METHOD == FW_CHECK_CRC)
            bl_makeCRCTable();
            #endif
            bl_checkFW(SDRAM_Start_FW, uiUpdateFileLen);

        }

#else   // (_MAINCODE_COMPRESS_ == 0)
        // Copy to DRAM FW starting address
        bl_memcpy((void *)SDRAM_Start_FW, (void *)g_uiUpdateMainBinBufAddr, uiUpdateFileLen);
#endif  // end of (_MAINCODE_COMPRESS_ == 1)
    }

    // Update loader info
    memset((void*)(SDRAM_Start_FW + BININFO_LOADER_INFO_OFFSET), 0, 16);
#if (_CHIP_ == _CHIP_655_)
    memcpy((void*)(SDRAM_Start_FW + BININFO_LOADER_INFO_OFFSET), "LD655", 5);
#elif (_CHIP_ == _CHIP_658_)
    memcpy((void*)(SDRAM_Start_FW + BININFO_LOADER_INFO_OFFSET), "LD658", 5);
#else
    memcpy((void*)(SDRAM_Start_FW + BININFO_LOADER_INFO_OFFSET), "LD650", 5);
#endif
    OUTREG32(SDRAM_Start_FW + BININFO_LOADER_INFO_OFFSET + 8, _LDR_VER_);
    OUTREG32(SDRAM_Start_FW + BININFO_LD_RUN_FLAG_OFFSET, uiLoaderFunc);
    OUTREG32(SDRAM_Start_FW + BININFO_REAL_LOADLEN_OFFSET, uiUpdateFileLen);
    OUTREG32(SDRAM_Start_FW + BININFO_ELAPSETIME_OFFSET, timer_getLdrElapse());
    //#NT#2012/12/17#HH Chuang -begin
    //#NT#Change reserved block from block unit to byte unit
    OUTREG32(SDRAM_Start_FW + BININFO_LD_RESERVED_BLOCK_OFFSET, StartNandBlkUpdateFW*uiNandBlkSize);
    //#NT#2012/12/17#HH Chuang -end
    OUTREG32(SDRAM_Start_FW + BININFO_RESERVED_SIZE_OFFSET, FW_MAX_CODE_SIZE);

    //invalid Instruciton and data cache
    CPUCleanInvalidateDCacheAll();
    CPUInvalidateICacheAll();
    return TRUE;

}
