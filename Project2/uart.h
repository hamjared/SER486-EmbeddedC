/********************************************************
 * uart.h
 *
 * this file provides function declarations for SER486
 * uart library functions.
 *
 * Author:   Doug Sandy
 * Date:     2/16/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */
#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

void uart_init(void);                   /* initialize the uart hardware */
void uart_writechar(char ch);           /* write a single character to the uart */
void uart_writestr(char * str);         /* write an ascii string to the uart */
void uart_writehex8(unsigned char num); /* output a 8-bit hexadecimal value as ascii text */
void uart_writehex16(unsigned int num); /* output a 16-bit hexadecimal value as ascii text */
void uart_writedec32(signed long num); /* output a 32-bit signed decimal value as ascii text */

#endif // UART_H_INCLUDED
