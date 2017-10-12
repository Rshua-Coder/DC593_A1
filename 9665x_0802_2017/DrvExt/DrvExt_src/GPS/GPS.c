/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GPS.c
    @ingroup

    @brief      Excute command via UART interface.
                Excute command and output message via UART interface.

    @note       Nothing.

    @version    V1.00.000
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Debug.h"
#include "SysKer.h"
#include "Kernel.h"
#include "UART.h"
#include "GPS.h"



#define _GPS_DEBUG_SYMBOL         0
#if _GPS_DEBUG_SYMBOL
#define GPS_debug(arg)            debug_msg arg
#else
#define GPS_debug(arg)
#endif

#define _GPS_NMEA_DEBUG_SYMBOL    0
#if _GPS_NMEA_DEBUG_SYMBOL
#define NMEA_debug(arg)            debug_msg arg
#else
#define NMEA_debug(arg)
#endif


//Declare functions
void GPSRec_NMEAParser(CHAR *NMEA_str, UINT32 Len, NMEATYPE NMEAtype);


//Declare variables
GSAINFO GSAInfo;
RMCINFO RMCInfo;
GGAINFO GGAInfo;
GSVINFO GSVInfo;
_ALIGNED(4) NMEASENTENCE NMEASentence={0};


#define PRI_GPSRECEIVE            10
#define STKSIZE_GPSRECEIVE        6144//64KB stack size
UINT32 GPSRECEIVE_ID = 0;
UINT32 FLG_ID_GPS = 0;

// global variables for parsing NMEA code
CHAR   type[6],RemainStr[NMEA_SENTENCE_SIZE], PostPtr[NMEA_SENTENCE_SIZE + 20], *pPostPrt;
UINT32 PostPtrLen;
UINT32 CheckSum;
//GSA used
CHAR   SelMode;
UINT32 FixMode, SatUsed01, SatUsed02, SatUsed03, SatUsed04, SatUsed05, SatUsed06, SatUsed07, SatUsed08, SatUsed09, SatUsed10, SatUsed11, SatUsed12;
double  PDOP, HDOP, VDOP;
//RMC used
UINT32 Date;
CHAR   Status,MagVarInd;
double  Speed, Angle, MagVar;
//GGA used
double  UTCTime;
UINT32 PosFixInd, SatNumber, Empty1, Empty2;
double  Lat, Lon, Alt, MSL;
CHAR   NSInd, EWInd, M1, M2;
//GSV used
UINT32 NumOfSen, SenNum, SatInView, SatNum1, Elev1, Azi1, SNR1, SatNum2, Elev2, Azi2, SNR2, SatNum3, Elev3, Azi3, SNR3, SatNum4, Elev4, Azi4, SNR4;


extern void GPSRecTsk(void);

void GPSRec_TransformData(CHAR *NMEA_str, UINT32 Len, CHAR *pPostPrt, UINT32 *PostPtrLen);
BOOL GPSRec_CheckSum(CHAR *NMEA_str, UINT32 Len, UINT32 CheckSum);
BOOL GPSRec_GetUTCDate(UINT32 *Year, UINT32 *Month, UINT32 *Day);
BOOL GPSRec_GetUTCTime(UINT32 *Hour, UINT32 *Minute, UINT32 *Second);

void GPS_InstallID(void)
{
    OS_CONFIG_TASK(GPSRECEIVE_ID, PRI_GPSRECEIVE, STKSIZE_GPSRECEIVE, GPSRecTsk);
    OS_CONFIG_FLAG(FLG_ID_GPS);
}

static BOOL bGPSRecOpened = FALSE;
ER GPSRec_Open(UART_BAUDRATE BaudRate)
{

    if (bGPSRecOpened)
        return E_SYS;

    #if (RECEIVE_FROM_UART2)
    if (uart2_open() == E_OK)
    #else
    if (uart_open() == E_OK)
    #endif
    {
        debug_msg("\r\nUART2: Open success\r\n");
        #if (RECEIVE_FROM_UART2)
        uart2_init(BaudRate, UART_LEN_L8_S1, UART_PARITY_NONE);
        #else
        uart_init(BaudRate, UART_LEN_L8_S1, UART_PARITY_NONE);
        #endif

    }
    else
    {
        debug_err(("UART2: open fail!\r\n"));
        return E_SYS;
    }

    debug_msg("Start GPS receive task\r\n");
    sta_tsk(GPSRECEIVE_ID,0);

    loc_cpu();
    bGPSRecOpened = TRUE;
    unl_cpu();

    return E_OK;
}

ER GPSRec_Close(void)
{

    if (!bGPSRecOpened)
        return E_SYS;

    #if (RECEIVE_FROM_UART2)
    if (uart2_close() == E_OK)
    #else
    if (uart_close() == E_OK)
    #endif
        debug_msg(("UART2: close success\r\n"));
    else
        debug_err(("UART2: close fail!\r\n"));

    debug_msg("Terminate GPS receive task\r\n");

    ter_tsk(GPSRECEIVE_ID);

    loc_cpu();
    bGPSRecOpened = FALSE;
    unl_cpu();

    return E_OK;
}

void GPSRecTsk(void)
{
    CHAR      cLen;
    UINT16    CheckSentenceType1, CheckSentenceType2;
    UINT32    CheckSentenceType3;
    CHAR      RecSentence[NMEA_SENTENCE_SIZE];
    CHAR      *pSentenceData;

    kent_tsk();

    clr_flg(FLG_ID_GPS, GPS_FLAG_DATAVALID);

    while(1)
    {
        cLen = NMEA_SENTENCE_SIZE ;
        #if (RECEIVE_FROM_UART2)
        if (uart2_getString(RecSentence, &cLen) == E_PAR)
        #else
        if (uart_getString(RecSentence, &cLen) == E_PAR)
        #endif
            debug_err(("UART2: parameter error!\r\n"));


        //debug_err(("%s\r\n",RecSentence));
        //Each NMEA sentence begins with '$' (0x24)
        if (RecSentence[0] == 0x24)
        {
            pSentenceData = RecSentence;
            CheckSentenceType1 = *(UINT16 *)(pSentenceData + 2);
            CheckSentenceType2 = *(UINT16 *)(pSentenceData + 4);
            CheckSentenceType3 = ((UINT)CheckSentenceType2 << 16) | (UINT)CheckSentenceType1;

            switch(CheckSentenceType3)
            {
            case GSA_SENTENCE:
                NMEASentence.GSA = RecSentence;
                GPSRec_NMEAParser(RecSentence, (UINT32)cLen, GSA);
                break;

            case RMC_SENTENCE:
                NMEASentence.RMC = RecSentence;
                GPSRec_NMEAParser(RecSentence, (UINT32)cLen, RMC);
                break;

            case GGA_SENTENCE:
                NMEASentence.GGA = RecSentence;
                GPSRec_NMEAParser(RecSentence, (UINT32)cLen, GGA);
                break;

            case GSV_SENTENCE:
                NMEASentence.GSV = RecSentence;
                GPSRec_NMEAParser(RecSentence, (UINT32)cLen, GSV);
                break;

            case GLL_SENTENCE:
                NMEASentence.GLL = RecSentence;
                GPSRec_NMEAParser(RecSentence, (UINT32)cLen, GLL);
                break;

            case VTG_SENTENCE:
                NMEASentence.VTG = RecSentence;
                GPSRec_NMEAParser(RecSentence, (UINT32)cLen, VTG);
                break;

            default:
                debug_err(("GPS: Can't identify this sentence 0x%X \r\n",CheckSentenceType3));
                break;
            }


        }




    }

}


ER GPSRec_Reset(void)
{

    return E_OK;
}

BOOL GPSRec_CheckData(void)
{
    debug_msg("GPS: DATA VALID: 0x%X\r\n",kchk_flg(FLG_ID_GPS, GPS_FLAG_DATAVALID));
    if (kchk_flg(FLG_ID_GPS, GPS_FLAG_DATAVALID))
        return TRUE;
    else
        return FALSE;

}


void GPSRec_TransformData(CHAR *NMEA_str, UINT32 Len, CHAR *pPostPrt, UINT32 *PostPtrLen)
{
    UINT32 i;
    //Process sentence, change ',' as ' '.
    //If message is null, change to corresponding value.
    for (i = 0; i <= Len; i++)
    {
        *pPostPrt = *NMEA_str;

        if (*NMEA_str == ',')
        {
            *pPostPrt = 0x20;   //Instead of ' '
            if ((*(NMEA_str+1) == ',')|(*(NMEA_str+1) == '*'))
            {
                *++pPostPrt = 0x30;  //Insert '0'
                *PostPtrLen = *PostPtrLen + 1;
            }
        }
        else if ((*NMEA_str == '*')|(i == Len))
        {
            *pPostPrt = 0x20;   //Instead of ' '
        }

        NMEA_str++;
        pPostPrt++;

    }

    NMEA_str -= Len;
    pPostPrt -= *PostPtrLen;

}


BOOL GPSRec_CheckSum(CHAR *NMEA_str, UINT32 Len, UINT32 CheckSum)
{
    UINT32 i, LenTemp = 1;
    CHAR XORData;
    //#NT#2009/04/07#Photon Lin -begin
    //#Refine GPS firmware
    XORData = *(NMEA_str++);

    //Doing 8 bit exclusive OR of all characters between, but not including, the '$' and '*'.
    for (i = 1; i < Len; i++)
    {
        NMEA_str++;
        if (*NMEA_str == '*')
            break;

        LenTemp++;
        XORData ^= *NMEA_str;
    }
    //#NT#2009/04/07#Photon Lin -end
    NMEA_str -= LenTemp;
    if (XORData == (CHAR)CheckSum)
        return TRUE;
    else
    {
        debug_err(("GPS: check sum error! 0x%X != 0x%X\r\n",CheckSum, XORData));
        return FALSE;
    }


}

void GPSRec_NMEAParser(CHAR *NMEA_str, UINT32 Len, NMEATYPE NMEAtype)
{
  UINT32 i;

    pPostPrt = PostPtr;
    PostPtrLen = Len;
		
    GPSRec_TransformData(NMEA_str, Len, pPostPrt, &PostPtrLen);
    switch(NMEAtype)
    {
    case GSA:
        sscanf(pPostPrt, "%s %c %d %d %d %d %d %d %d %d %d %d %d %d %d %lf %lf %lf %X\n", type, &SelMode, &FixMode, &SatUsed01, &SatUsed02, &SatUsed03, &SatUsed04, &SatUsed05, &SatUsed06, &SatUsed07, &SatUsed08, &SatUsed09, &SatUsed10, &SatUsed11, &SatUsed12, &PDOP, &HDOP, &VDOP, &CheckSum);
        #if (_GPS_CheckSum_Enable)
            GPSRec_CheckSum(NMEA_str, Len, CheckSum);
        #endif
        if (FixMode != No_Fix)
        {
            GSAInfo.SelMode = SelMode;
            GSAInfo.FixMode = FixMode;
            GSAInfo.SatUsed01 = SatUsed01;
            GSAInfo.SatUsed02 = SatUsed02;
            GSAInfo.SatUsed03 = SatUsed03;
            GSAInfo.SatUsed04 = SatUsed04;
            GSAInfo.SatUsed05 = SatUsed05;
            GSAInfo.SatUsed06 = SatUsed06;
            GSAInfo.SatUsed07 = SatUsed07;
            GSAInfo.SatUsed08 = SatUsed08;
            GSAInfo.SatUsed09 = SatUsed09;
            GSAInfo.SatUsed10 = SatUsed10;
            GSAInfo.SatUsed11 = SatUsed11;
            GSAInfo.SatUsed12 = SatUsed12;
            GSAInfo.PDOP = PDOP;
            GSAInfo.HDOP = HDOP;
            GSAInfo.VDOP = VDOP;
            GPS_debug(("#GSA SelMode = %c, FixMode = %d \r\n",GSAInfo.SelMode,GSAInfo.FixMode));
            GPS_debug(("#GSA SelMode = %c, FixMode = %d \r\n",GSAInfo.SelMode,GSAInfo.FixMode));
            GPS_debug(("#GSA SatUsed1 = %d, %d, %d, %d \r\n",GSAInfo.SatUsed01,GSAInfo.SatUsed02, GSAInfo.SatUsed03, GSAInfo.SatUsed04));
            GPS_debug(("#GSA SatUsed2 = %d, %d, %d, %d \r\n",GSAInfo.SatUsed05,GSAInfo.SatUsed06, GSAInfo.SatUsed07, GSAInfo.SatUsed08));
            GPS_debug(("#GSA SatUsed3 = %d, %d, %d, %d \r\n",GSAInfo.SatUsed09,GSAInfo.SatUsed10, GSAInfo.SatUsed11, GSAInfo.SatUsed12));
            GPS_debug(("#PDOP = %.1f, HDOP = %.1f, VDOP = %.1f CheckSum = %X\r\n",GSAInfo.PDOP,GSAInfo.HDOP, GSAInfo.VDOP,CheckSum));
        }
        if (Len)
        {
            for (i = 0; i < Len; i++)
                NMEA_debug(("%c",*NMEA_str++));
        }
        NMEA_debug(("\r\n"));

        //Check processed sentence
        /*
        if (PostPtrLen)
        {
            for (i = 0; i < PostPtrLen; i++)
                NMEA_debug(("%c",*pPostPrt++));
        }
        NMEA_debug(("\r\n"));
        */
        break;

    case RMC:
		GPS_debug(("^Y GPS RMC Raw Info %s",NMEA_str));   //drh 20170918
        sscanf(pPostPrt, "%s %lf %c %lf %c %lf %c %lf %lf %d %lf %c %X\n", type, &UTCTime, &Status, &Lat, &NSInd, &Lon, &EWInd, &Speed, &Angle, &Date, &MagVar, &MagVarInd, &CheckSum);
        #if (_GPS_CheckSum_Enable)
            GPSRec_CheckSum(NMEA_str, Len, CheckSum);
        #endif

            RMCInfo.Hour =      (UINT32)UTCTime / 10000;
            RMCInfo.Minute =    ((UINT32)UTCTime % 10000) / 100;
            RMCInfo.Second =    ((UINT32)UTCTime % 100);
            RMCInfo.Status =    Status;
            RMCInfo.Latitude =  Lat;
            RMCInfo.NSInd =     NSInd;
            RMCInfo.Longitude = Lon;
            RMCInfo.EWInd =     EWInd;
            RMCInfo.Speed =     Speed;
            RMCInfo.Angle =     Angle;
            RMCInfo.Day =       Date / 10000;
            RMCInfo.Month =     (Date % 10000) / 100;
            RMCInfo.Year =      (Date % 100);
        if (Status == 'A')   //Data valid
        {
            set_flg(FLG_ID_GPS, GPS_FLAG_DATAVALID);

            GPS_debug(("\x1b[34m"));   //Blue
            GPS_debug(("#RMC: Status = %c, Speed = %.1f, Angle = %.1f \r\n",RMCInfo.Status,RMCInfo.Speed,RMCInfo.Angle));
            GPS_debug(("#RMC: Date = %d\\%d\\%d CheckSum = %X\r\n",RMCInfo.Year,RMCInfo.Month,RMCInfo.Day,CheckSum));
            GPS_debug(("\x1b[30m"));   //Black
        }
        else if (Status == 'V')
        {
            clr_flg(FLG_ID_GPS, GPS_FLAG_DATAVALID);
        }
        NMEA_debug(("\x1b[34m"));   //Blue
        if (Len)
        {
            for (i = 0; i < Len; i++)
                NMEA_debug(("%c",*NMEA_str++));
        }
        NMEA_debug(("\r\n"));
        //Check processed sentence
        /*
        if (PostPtrLen)
        {
            for (i = 0; i < PostPtrLen; i++)
                NMEA_debug(("%c",*pPostPrt++));
        }
        NMEA_debug(("\r\n"));
        */
        NMEA_debug(("\x1b[30m"));   //Black

        break;

    case GGA:

        sscanf(pPostPrt, "%s %lf %lf %c %lf %c %d %d %lf %lf %c %lf %c %d %d %X\n", type, &UTCTime, &Lat, &NSInd, &Lon, &EWInd, &PosFixInd, &SatNumber, &HDOP, &Alt, &M1, &MSL, &M2, &Empty1, &Empty2, &CheckSum);
        #if (_GPS_CheckSum_Enable)
            GPSRec_CheckSum(NMEA_str, Len, CheckSum);
        #endif

        if (PosFixInd == 1)  //Data valid
        {
            GGAInfo.Hour = (UINT32)UTCTime / 10000;
            GGAInfo.Minute = ((UINT32)UTCTime % 10000) / 100;
            GGAInfo.Second = ((UINT32)UTCTime % 100);
            GGAInfo.Latitude = Lat;
            GGAInfo.NSInd = NSInd;
            GGAInfo.Longitude = Lon;
            GGAInfo.EWInd = EWInd;
            GGAInfo.PosFixInd = PosFixInd;
            GGAInfo.SatNumber = SatNumber;
            GGAInfo.HDOP = HDOP;
            GGAInfo.Altitude = Alt;
            GGAInfo.MSL = MSL;
            GPS_debug(("\x1b[31m"));   //Red
            GPS_debug(("#GGA: Time = %d:%d:%d CheckSum = %X\r\n", GGAInfo.Hour, GGAInfo.Minute, GGAInfo.Second,CheckSum));
            GPS_debug(("#GGA: Lat = %1f %c, Lon = %1f %c \r\n", GGAInfo.Latitude, NSInd, GGAInfo.Longitude, EWInd));
            GPS_debug(("#GGA: PosFixInd = %d, SatNum = %d, HDOP = %2.1f, Alt = %.1f, MSL = %.1f \r\n", GGAInfo.PosFixInd, GGAInfo.SatNumber, GGAInfo.HDOP, GGAInfo.Altitude, GGAInfo.MSL));
            GPS_debug(("\x1b[30m"));   //Black

        }

        NMEA_debug(("\x1b[31m"));   //Red
        if (Len)
        {
            for (i = 0; i < Len; i++)
                NMEA_debug(("%c",*NMEA_str++));
        }
        NMEA_debug(("\r\n"));

        //Check processed sentence
        /*
        if (PostPtrLen)
        {
            for (i = 0; i < PostPtrLen; i++)
                NMEA_debug(("%c",*pPostPrt++));
        }
        NMEA_debug(("\r\n"));
        */
        NMEA_debug(("\x1b[30m"));   //Black

        break;

    case GSV:
        sscanf(pPostPrt, "%s %d %d %d %s\n", type, &NumOfSen, &SenNum, &SatInView, RemainStr);
        if (NumOfSen == SenNum)
        {
            if ((SatInView % 4) == 1)
                sscanf(pPostPrt, "%s %d %d %d %d %d %d %d %X\n", type, &NumOfSen, &SenNum, &SatInView, &SatNum1, &Elev1, &Azi1, &SNR1, &CheckSum);
            else if ((SatInView % 4) == 2)
                sscanf(pPostPrt, "%s %d %d %d %d %d %d %d %d %d %d %d %X\n", type, &NumOfSen, &SenNum, &SatInView, &SatNum1, &Elev1, &Azi1, &SNR1, &SatNum2, &Elev2, &Azi2, &SNR2, &CheckSum);
            else if ((SatInView % 4) == 3)
                sscanf(pPostPrt, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %X\n", type, &NumOfSen, &SenNum, &SatInView, &SatNum1, &Elev1, &Azi1, &SNR1, &SatNum2, &Elev2, &Azi2, &SNR2, &SatNum3, &Elev3, &Azi3, &SNR3, &CheckSum);
            else
                sscanf(pPostPrt, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %X\n", type, &NumOfSen, &SenNum, &SatInView, &SatNum1, &Elev1, &Azi1, &SNR1, &SatNum2, &Elev2, &Azi2, &SNR2, &SatNum3, &Elev3, &Azi3, &SNR3, &SatNum4, &Elev4, &Azi4, &SNR4, &CheckSum);
        }
        else
            sscanf(pPostPrt, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %X\n", type, &NumOfSen, &SenNum, &SatInView, &SatNum1, &Elev1, &Azi1, &SNR1, &SatNum2, &Elev2, &Azi2, &SNR2, &SatNum3, &Elev3, &Azi3, &SNR3, &SatNum4, &Elev4, &Azi4, &SNR4, &CheckSum);

        #if (_GPS_CheckSum_Enable)
            GPSRec_CheckSum(NMEA_str, Len, CheckSum);
        #endif

        if (SenNum == 1)   //GSV1
        {
            GSVInfo.NumOfSen =     NumOfSen;
            GSVInfo.SatInView =    SatInView;
            GSVInfo.SAT01.SatNum = SatNum1;
            GSVInfo.SAT01.Elev =   Elev1;
            GSVInfo.SAT01.Azi =    Azi1;
            GSVInfo.SAT01.SNR =    SNR1;
            GSVInfo.SAT02.SatNum = SatNum2;
            GSVInfo.SAT02.Elev =   Elev2;
            GSVInfo.SAT02.Azi =    Azi2;
            GSVInfo.SAT02.SNR =    SNR2;
            GSVInfo.SAT03.SatNum = SatNum3;
            GSVInfo.SAT03.Elev =   Elev3;
            GSVInfo.SAT03.Azi =    Azi3;
            GSVInfo.SAT03.SNR =    SNR3;
            GSVInfo.SAT04.SatNum = SatNum4;
            GSVInfo.SAT04.Elev =   Elev4;
            GSVInfo.SAT04.Azi =    Azi4;
            GSVInfo.SAT04.SNR =    SNR4;
            GPS_debug(("\x1b[35m"));   //Violet
            GPS_debug(("#GSV%d SatInView = %d, CheckSum = %X\r\n",SenNum, SatInView, CheckSum));
            GPS_debug(("#GSV%d Sat1 = %d, %d, %d, %d, Sat2 = %d, %d, %d, %d\r\n",SenNum, SatNum1, Elev1, Azi1, SNR1, SatNum2, Elev2, Azi2, SNR2));
            GPS_debug(("#GSV%d Sat3 = %d, %d, %d, %d, Sat4 = %d, %d, %d, %d\r\n",SenNum, SatNum3, Elev3, Azi3, SNR3, SatNum4, Elev4, Azi4, SNR4));
            GPS_debug(("\x1b[30m"));      //Black

        }
        else if (SenNum == 2)   //GSV2
        {
            GSVInfo.NumOfSen =     NumOfSen;
            GSVInfo.SatInView =    SatInView;
            GSVInfo.SAT05.SatNum = SatNum1;
            GSVInfo.SAT05.Elev =   Elev1;
            GSVInfo.SAT05.Azi =    Azi1;
            GSVInfo.SAT05.SNR =    SNR1;
            GSVInfo.SAT06.SatNum = SatNum2;
            GSVInfo.SAT06.Elev =   Elev2;
            GSVInfo.SAT06.Azi =    Azi2;
            GSVInfo.SAT06.SNR =    SNR2;
            GSVInfo.SAT07.SatNum = SatNum3;
            GSVInfo.SAT07.Elev =   Elev3;
            GSVInfo.SAT07.Azi =    Azi3;
            GSVInfo.SAT07.SNR =    SNR3;
            GSVInfo.SAT08.SatNum = SatNum4;
            GSVInfo.SAT08.Elev =   Elev4;
            GSVInfo.SAT08.Azi =    Azi4;
            GSVInfo.SAT08.SNR =    SNR4;
            GPS_debug(("\x1b[35m"));   //Violet
            GPS_debug(("#GSV%d SatInView = %d, CheckSum = %X\r\n",SenNum, SatInView, CheckSum));
            GPS_debug(("#GSV%d Sat5 = %d, %d, %d, %d, Sat6 = %d, %d, %d, %d\r\n",SenNum, SatNum1, Elev1, Azi1, SNR1, SatNum2, Elev2, Azi2, SNR2));
            GPS_debug(("#GSV%d Sat7 = %d, %d, %d, %d, Sat8 = %d, %d, %d, %d\r\n",SenNum, SatNum3, Elev3, Azi3, SNR3, SatNum4, Elev4, Azi4, SNR4));
            GPS_debug(("\x1b[30m"));      //Black

        }
        else if (SenNum == 3)   //GSV3
        {
            GSVInfo.NumOfSen =     NumOfSen;
            GSVInfo.SatInView =    SatInView;
            GSVInfo.SAT09.SatNum = SatNum1;
            GSVInfo.SAT09.Elev =   Elev1;
            GSVInfo.SAT09.Azi =    Azi1;
            GSVInfo.SAT09.SNR =    SNR1;
            GSVInfo.SAT10.SatNum = SatNum2;
            GSVInfo.SAT10.Elev =   Elev2;
            GSVInfo.SAT10.Azi =    Azi2;
            GSVInfo.SAT10.SNR =    SNR2;
            GSVInfo.SAT11.SatNum = SatNum3;
            GSVInfo.SAT11.Elev =   Elev3;
            GSVInfo.SAT11.Azi =    Azi3;
            GSVInfo.SAT11.SNR =    SNR3;
            GSVInfo.SAT12.SatNum = SatNum4;
            GSVInfo.SAT12.Elev =   Elev4;
            GSVInfo.SAT12.Azi =    Azi4;
            GSVInfo.SAT12.SNR =    SNR4;
            GPS_debug(("\x1b[35m"));   //Violet
            GPS_debug(("#GSV%d SatInView = %d, CheckSum = %X\r\n",SenNum, SatInView, CheckSum));
            GPS_debug(("#GSV%d Sat9 = %d, %d, %d, %d,  Sat10 = %d, %d, %d, %d\r\n",SenNum, SatNum1, Elev1, Azi1, SNR1, SatNum2, Elev2, Azi2, SNR2));
            GPS_debug(("#GSV%d Sat11 = %d, %d, %d, %d, Sat12 = %d, %d, %d, %d\r\n",SenNum, SatNum3, Elev3, Azi3, SNR3, SatNum4, Elev4, Azi4, SNR4));
            GPS_debug(("\x1b[30m"));      //Black

        }
        else
			;
            //debug_err(("GPS: Invalid number of GSV"));


        NMEA_debug(("\x1b[35m"));   //Violet
        if (Len)
        {
            for (i = 0; i < Len; i++)
                NMEA_debug(("%c",*NMEA_str++));
        }
        NMEA_debug(("\r\n"));
        //Check processed sentence
        /*
        if (PostPtrLen)
        {
            for (i = 0; i < PostPtrLen; i++)
                NMEA_debug(("%c",*pPostPrt++));
        }
        NMEA_debug(("\r\n"));
        */
        NMEA_debug(("\x1b[30m"));      //Black
        break;
    //Do not support GLL and VTG yet
    case GLL:

        NMEA_debug(("\x1b[36m"));   //Blue
        if (Len)
        {
            for (i = 0; i < Len; i++)
                NMEA_debug(("%c",*NMEA_str++));
        }
        NMEA_debug(("\r\n"));
        NMEA_debug(("\x1b[30m"));   //Black


        break;

    case VTG:

        NMEA_debug(("\x1b[32m"));   //Green
        if (Len)
        {
            for (i = 0; i < Len; i++)
                NMEA_debug(("%c",*NMEA_str++));
        }
        NMEA_debug(("\r\n"));
        NMEA_debug(("\x1b[30m"));   //Black

        break;

    default:

        break;
    }





}

BOOL GPSRec_GetRMCDate(RMCINFO *pRMCInfo)
{
    memcpy(pRMCInfo,&RMCInfo,sizeof(RMCINFO));
    if (RMCInfo.Status == 'A')  //Data valid
    {
        return TRUE;
    }
    else
        return FALSE;
}

BOOL GPSRec_GetUTCDate(UINT32 *Year, UINT32 *Month, UINT32 *Day)
{
    if (RMCInfo.Status == 'A')  //Data valid
    {
        *Year   = RMCInfo.Year;
        *Month  = RMCInfo.Month;
        *Day    = RMCInfo.Day;
        return TRUE;
    }
    else
        return FALSE;

}

BOOL GPSRec_GetUTCTime(UINT32 *Hour, UINT32 *Minute, UINT32 *Second)
{
    if (RMCInfo.Status == 'A')   //Data valid
    {
        *Hour   = RMCInfo.Hour;
        *Minute = RMCInfo.Minute;
        *Second = RMCInfo.Second;
        return TRUE;
    }
    else
        return FALSE;

}


