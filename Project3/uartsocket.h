/********************************************************
 * uartsocket.h
 *
 * this file provides function declarations for SER486
 * uartsocket communications socket class.  This really is
 * just the same functionality as the uart library with
 * support for full line processing, and "connection"
 *
 * Author:   Doug Sandy
 * Date:     7/29/2019
 * Revision: 1.0
 *
 * Copyright(C) 2019, Arizona State University
 * All rights reserved
 *
 */
#ifndef	_UARTSOCKET_H_
#define	_UARTSOCKET_H_

/* initialize the uart 'socket' for use */
void uartsocket_init(void);

/* write a single character to the uart */
void uartsocket_writechar(char ch);

/* write an ascii string to the uart */
void uartsocket_writestr(char * str);

/* output a 8-bit hexadecimal value as ascii text */
void uartsocket_writehex8(unsigned char num);

/* output a 16-bit hexadecimal value as ascii text */
void uartsocket_writehex16(unsigned int num);

void uartsocket_writedec32(signed long num);

/* output a 32-bit signed decimal value as ascii text */
unsigned char uartsocket_is_connected();

/* returns 1 if a complete line has been received on the socket */
unsigned char uartsocket_is_packet_available();

/* read characters from the socket and place them in the buffer until a delimeter is found */
/* this function blocks until a "packet" is received */
unsigned char uartsocket_readpacket(char* buf,char *delimiters);

/* return 1 (true) if the socket is connected */
unsigned char uartsocket_is_connected();

#endif
/* _UARTSOCKET_H_ */
