#ifndef _QR_ENCODE_H_
#define _QR_ENCODE_H_

#include "Type.h"

#define MAX_BITDATA      (3917)	/* Max QR Code Data */
#define QR_REQ_BUF_SIZE  (57000)

extern UINT32 QRCode_Encode(unsigned char *inStr, unsigned char *QRBuf, UINT32 QRBufSize);

#endif
