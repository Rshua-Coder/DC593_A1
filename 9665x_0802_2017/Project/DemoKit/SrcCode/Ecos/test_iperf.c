#include <stdio.h>
#include <stdlib.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/maincmd.h>
#include <cyg/infra/mainfunc.h>
#include <fcntl.h>
#include <unistd.h>
#include <cyg/fileio/fileio.h>
#include <cyg/infra/diag.h>
#include "Iperf.h"
#include "Type.h"
#include "Stdio.h"

#if 1 //debug
#define DBG_IND(fmtstr, args...) diag_printf("iperf: %s() " fmtstr, __func__, ##args)
#else //release
#define DBG_IND(...)
#endif
#define DBG_ERR(fmtstr, args...) diag_printf("iperf: ERR %s() " fmtstr, __func__, ##args)
#define DBG_DUMP(fmtstr, args...) diag_printf("%s" fmtstr, __func__, ##args)

char gsziPerfPath[PATH_MAX]     = "/sdcard/iperf3.log";
char gsziPerfWriteBuf[PATH_MAX] = {0};
int  gfiled                     = -1;

int  gWriteSize                 = 0;

MAINFUNC_ENTRY(iperf,argc,argv)
{
    Iperf_Close();
    Iperf_Open(argc, argv);
    return 0;
}

MAINFUNC_ENTRY(iperfc,argc,argv)
{
    Iperf_RunClient(argc, argv);
    return 0;
}

#if 1
MAINFUNC_ENTRY(iperfresult,argc,argv)
{
    UINT32 ResultAddr = 0;
    int    err;

    if(argc == 2)
    {
        sscanf(argv[1],"%u",&ResultAddr);
        *(float *)ResultAddr = Iperf_GetResult();
    }

    printf("\nIperf Result = %f Mbits/sec @ 0x%X\n",Iperf_GetResult(),ResultAddr);

    // Write result to file
    if (gfiled > 0)
    {
         // write file
         snprintf(gsziPerfWriteBuf, PATH_MAX, "\nIperf Result = %f Mbits/sec\n", Iperf_GetResult());
         gWriteSize = strlen(gsziPerfWriteBuf);

         err = write(gfiled, gsziPerfWriteBuf, gWriteSize);
         if( err < 0 )
         {
            DBG_ERR("write failed\r\n");
            return -1;
         }

         // close file
         err = close(gfiled);

         if( err < 0 )
         {

            DBG_ERR("close %s failed\r\n", gsziPerfPath);
            return -1;
         }

         gfiled = -1;
    }

    return 0;
}

void ReportCB(double StartTime, double EndTime, double Bandwidth)
{
    int err;

    // Write interval result to file
    if (gfiled > 0)
    {
         // write file
         snprintf(gsziPerfWriteBuf, PATH_MAX, "StartTime=%6.2f sec  StartTime=%6.2f sec  Bandwidth=%6.2f Mbits/sec\r\n", StartTime, EndTime, Bandwidth);
         gWriteSize = strlen(gsziPerfWriteBuf);

         err = write(gfiled, gsziPerfWriteBuf, gWriteSize);
         if( err < 0 )
         {
            DBG_ERR("write failed\r\n");
            return;
         }
    }
    else
        printf("StartTime=%6.2f  StartTime=%6.2f  Bandwidth=%6.2f \r\n",StartTime,EndTime,Bandwidth);
}

MAINFUNC_ENTRY(iperflog,argc,argv)
{
    // Register callback of iperf3
    Iperf_RegReportCB(ReportCB);

    // Create log file
    gfiled = open(gsziPerfPath, O_WRONLY|O_CREAT);
    if( gfiled < 0 )
    {
        DBG_ERR("open %s failed\r\n", gsziPerfPath);
        return -1;
    }

    return 0;
}

#endif
