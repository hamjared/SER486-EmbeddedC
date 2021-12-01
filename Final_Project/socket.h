/********************************************************
 * socket.h
 *
 * this file provides function declarations for SER486
 * socket (TCP) and udpsocket (udp) communications socket
 * classes.
 *
 * Author:   Doug Sandy
 * Date:     4/18/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */
#ifndef	_SOCKET_H_
#define	_SOCKET_H_

#define SOCKET unsigned char

/*****************************************************************
* TCP Socket Functions
******************************************************************/
/* initialize the socket for use */
unsigned char socket_open(SOCKET s, unsigned int port);

/* Attempt to connect with a remote host.  Once connected the device transitions
* the the TCP ESTAB state.  This function is for client-side code.
*/
unsigned char socket_connect(SOCKET s, unsigned char * addr, unsigned int port);

/* Attempt to gracefully close the TCP connection with acknowledgement from
* the remote host. When the connection has closed, the socket will be in the
* TCP CLOSED state.
*/
void          socket_disconnect(SOCKET s);

/* force an immediate disconnection from any remote host and force the socket
* into the TCP CLOSED state
*/
void          socket_close(SOCKET s);

/* Place the socket in passive connect (listen) mode. */
unsigned char socket_listen(SOCKET s);

/* return 1 if the socket is not closed or in the process of closing,
* otherwise, return 0
*/
unsigned char socket_is_active(SOCKET s);

/* return 1 if the socket is in LISTEN mode, otherwise, return 0 */
unsigned char socket_is_listening(SOCKET s);

/* return 1 if the socket has an established connection with a remote host,
* otherwise, return 0.
*/
unsigned char socket_is_established(SOCKET s);

/* return 1 if the socket is in the CLOSED state, otherwise, return 0. */
unsigned char socket_is_closed(SOCKET s);

/* send the contents of the buffer to the remote host */
unsigned int  socket_send(SOCKET s, const unsigned char * buf, unsigned int len);

/* send a single character to the remote host */
void          socket_writechar(SOCKET s, const char ch);

/* send an ascii string to the remote host (not including the terminating null) */
void          socket_writestr(SOCKET s, const char*str);

/* send the specified ascii string to the remote host, enclosed in double quote characters */
void          socket_writequotedstring(SOCKET s, const char*str);

/* send the specified 8-bit integer to the remote host as a hexadecimal, text representation */
void          socket_writehex8(SOCKET s, const unsigned char x);

/* send the specified 16-bit integer to the remote host as hexadecimal, text representation */
void          socket_writehex16(SOCKET s, const unsigned int x);

/* send the specified integer to the remote host as a decimal text representation */
void          socket_writedec32(SOCKET s, int n);

/* given a RTC date/time number, send a text representation of the date to the remote host */
void          socket_writedate(SOCKET s, unsigned long datenum);

/* given a pointer to an array of 6 unsigned characters representing a mac address, send
* the text representation consisting of 6 8-bit hexadecimal numbers, separated by colons */
void          socket_write_macaddress(SOCKET s, unsigned char *mac_address);

/* return how many bytes (characters) are available in the socket’s receive buffer */
int           socket_recv_available(SOCKET s);

/* return 1 if a CRLF has been received in the socket’s receive buffer.  The contents
* of the receive buffer are not altered.
*/
unsigned char socket_received_line(SOCKET s);

/* returns 1 if a blank line is the next data within the socket’s receive buffer.
* The contents of the receive buffer are not altered.
*/
unsigned char socket_is_blank_line(SOCKET s);

/* Fills the specified character buffer with the first character present
* in the socket’s receive buffer.  The contents of the buffer are not altered.
*/
unsigned int  socket_peek(SOCKET s, unsigned char *buf);

/* Receive a certain number of characters from the receive buffer and place
* them in the specified character buffer.
*/
int           socket_recv(SOCKET s, unsigned char * buf, int len);

/* Receive a string representation of an integer number from the receive buffer
* and convert it to an integer.  The  integer value is placed at *num.
* Returns 1 on success, 0 on error. On success, the text is removed from the
* receive buffer, otherwise, the buffer is not altered.
*/
unsigned char socket_recv_int(SOCKET s, int *num);

/* Compare the first byte of the receive buffer with the specified string.
* If they match, the bytes are removed from the buffer and the function returns
* a value of 1.  Otherwise, the contents of the receive buffer are not altered
* and the function returns 0.
*/
unsigned char socket_recv_compare(SOCKET s, const char*str);

/* flush characters from the receive buffer until no more characters are
* available, or a CRLF is found.  In the case of the latter, the CRLF is
* also flushed from the buffer.
*/
void          socket_flush_line(SOCKET s);

/*****************************************************************
* UDP Socket Functions
******************************************************************/
/* initialize the udp socket for use */
unsigned char udpsocket_open(SOCKET s, unsigned int port);

/* initialize the udp socket for multicast use on the specified multicast address and port */
unsigned char udpsocket_open_multicast(SOCKET s, unsigned char * address, unsigned int port);

/* close the socket */
void          udpsocket_close(SOCKET s);

/* return 1 if the socket is open, 0 otherwise */
unsigned char udpsocket_is_open(SOCKET s);

/* return 1 if the socket is closed, 0 otherwise */
unsigned char udpsocket_is_closed(SOCKET s);

/* send len characters from the specified buffer to the remote host address/port */
unsigned int  udpsocket_sendto(SOCKET s, const unsigned char * buf, unsigned int len, unsigned char * addr, unsigned int port);

/* start a datagram to be sent to the remote host specified by the address and port parameters */
int           udpsocket_start_datagram(SOCKET s, unsigned char* addr, unsigned int port);

/* add additional datagram.  The offset parameter specifies the byte offset within the datagram
* where the new data should be placed.
*/
unsigned int  udpsocket_add_to_datagram(SOCKET s, unsigned int offset, const unsigned char* buf, unsigned int len);

/* send the datagram created using the udpsocket_start_datagram() and udpsocket_add_to_datagram() functions */
int           udpsocket_send_datagram(SOCKET s);

/* returns the number of bytes available within the socket's receive buffer */
int           udpsocket_recv_available(SOCKET s);

/* receive data from a specified remote host.  The len parameter specifies the maximum data that may
* be transferred.  The function returns the number of bytes actually received.
*/
unsigned int  udpsocket_recvfrom(SOCKET s, unsigned char * buf, unsigned int len, unsigned char * addr, unsigned int *port);

/* receive data from the receive buffer.  The len parameter specifies the maximum data that may
* be transferred.  The function returns the number of bytes actually received.
*/
unsigned int  udpsocket_recv_data(SOCKET s, unsigned char *buf, unsigned int len);


#endif
/* _SOCKET_H_ */
