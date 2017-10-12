/**
    MD5 utility

    @file       md5_util.c
    @ingroup

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.

	This is a wrap of axtls md5 function
*/

#ifndef _MD5_UTIL_H_
#define _MD5_UTIL_H_

//#define u_int32_t UINT32

#include "Type.h"
//#include "md5.h"			// refer to axTLS lib in eCos thirdParty library ("\..\..\ThirdParty\eCos\include\sys\md5.h)
//#include "$(ROOT_DIR)/ThirdParty/eCos/include/sys/md5.h"			// refer to axTLS lib in eCos thirdParty library ("\..\..\ThirdParty\eCos\include\sys\md5.h)


/**
     Calculate MD5 value of a buffer

     Alloc space from stack.
     @note The memory space is using working buffer.

     @param[in] digest          An unsigned char array with size 16 to store MD5 result
     @param[in] in_buf          Buffer address
     @param[in] in_len          Buffer length
     @return
         - @b TRUE:             Successed
         - @b FALSE:            Input parameter error
*/
extern BOOL md5_calc(unsigned char *digest, unsigned char *in_buf, unsigned int in_len);

#endif	// #ifndef _MD5_UTIL_H_