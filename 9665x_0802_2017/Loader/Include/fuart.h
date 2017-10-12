/**
    Header file for UART

    This file is the header file for UART driver

    @file       fuart.h
    @ingroup    mIIOUART
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/


#ifndef _FUART_H
#define _FUART_H

/**
    @addtogroup mIIOUART
*/
//@{

/**
    @name Put system UART string

    Print string to UART

    @param[in] m        printed string
*/
//@{
#define uart_putSystemUARTStr(m)    fLib_PutSerialStr(m)    ///< Print string MACRO
//@}

/**
    @name Put system UART character

    Print character to UART

    @param[in] m        printed character
*/
//@{
#define uart_putSystemUARTChar(m)   fLib_PutSerialChar(m)   ///< Print character MACRO
//@}

extern void fLib_PutSerialChar(char Ch);
extern void fLib_PutSerialStr(char *Str);
extern void uart_openSystemUART(void);

//@}

#endif
