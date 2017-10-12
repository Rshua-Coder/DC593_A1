#ifndef _ECOS_FILESYS_H
#define _ECOS_FILESYS_H

#include <sys\types.h>

void cyg_FileSys_RegisterCB(void);
void date_unix2dos(cyg_uint32 unix_timestamp, cyg_uint16 *dos_time, cyg_uint16 *dos_date);
void listdir( char *name, int statp);
#endif