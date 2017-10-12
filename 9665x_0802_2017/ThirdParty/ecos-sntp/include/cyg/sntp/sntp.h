#ifndef CYGONCE_SNTP_SNTP_H
#define CYGONCE_SNTP_SNTP_H
//=============================================================================
//
//      sntp.h
//
//      SNTP - Simple Network Time Protocol
//
//=============================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2003 Andrew Lunn
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   andrew.lunn
// Contributors:
// Date:        2003-02-11
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <cyg/infra/cyg_type.h>

typedef unsigned long       SNTP_U32;                    ///<  Unsigned 32 bits data type
typedef signed long         SNTP_BOOL;


typedef SNTP_BOOL  cyg_sntp_timeCb(char *cmd);           ///< Callback when ntp time got from ntp server.


typedef struct
{
    cyg_sntp_timeCb     *sntp_timeCb;                    ///< Callback when ntp time got from ntp server
} sntp_config_obj;

/**
    config some parameters of sntp.

    @param[in] config:  config info for sntp.

    Example:
    @code
    {
        sntp_config_obj   config_obj = {0};

        config_obj.sntp_timeCb = (cyg_sntp_timeCb *)SxCmd_DoCommand;
        sntp_config(&config_obj);
    }
    @endcode

*/
__externC void sntp_config(sntp_config_obj *config);

/**
    Start/Stop the SNTP Client.

    Call this function to start/stop the SNTP Client.

    @param[in] cmd:  sntp command.

    Example:
    @code
    {
        // start sntp client , ex: sntp -open "ntp server address"
        sntp_cmd("sntp -open 192.168.0.4");


        // stop sntp client
        sntp_cmd("sntp -close");
    }
    @endcode

*/
__externC void sntp_cmd(char *cmd);

#endif
