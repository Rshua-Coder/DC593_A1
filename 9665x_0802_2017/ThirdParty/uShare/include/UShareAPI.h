/**
    UShare module

    DLNA server for sharing media files.\n
    It is ported from the open source uShare (http://ushare.geexbox.org/).

    @file       UShareAPI.h

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/
#ifndef _USHAREAPI_H
#define _USHAREAPI_H

/**
    Open DLNA server.

    This API will create a DLNA server for sharing media files.\n
    The input parameters are based on uShare usage (http://ushare.geexbox.org/#Usage)

    @param[in] argc     The argument count
    @param[in] argv     The argument vector

    @note The network and eCos file system should be initialized first.

    Example:
    @code
    {
        //command line: ushare -c /shares -d -i eth0
        char *argv[6];
        argv[0] = "ushare";
        argv[1] = "-c";
        argv[2] = "/shares";
        argv[3] = "-d";
        argv[4] = "-i";
        argv[5] = "eth0";
        UShare_Open(6, argv);
    }
    @endcode
*/
__externC void UShare_Open( int argc, char **argv );

/**
    Close DLNA server.

    This API will close the DLNA server created by UShare_Open()
*/
__externC void UShare_Close(void);


#endif //USHAREAPI
