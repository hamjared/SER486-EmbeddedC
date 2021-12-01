/********************************************************
 * process_packet.h
 *
 * this file provides function declarations for various
 * packet processing functions for SER486.
 *
 * Author:   Doug Sandy
 * Date:     7/29/2019
 * Revision: 1.0
 *
 * Copyright(C) 2019, Arizona State University
 * All rights reserved
 *
 */

#ifndef PROCESS_PACKET_H_INCLUDED
#define PROCESS_PACKET_H_INCLUDED

/* update the configuration tcrit_hi limit with the specified value
* This function is called by the packet command parser.
*/
int update_tcrit_hi(int value);

/* Update the configuration twarn_hi limit with the specified value
* This function is called by the packet command parser.
*/
int update_twarn_hi(int value);

/* Update the configuration tcrit_lo limit with the specified value
* This function is called by the packet command parser.
*/
int update_tcrit_lo(int value);

/* Update the configuration twarn_lo limit with the specified value
* This function is called by the packet command parser.
*/
int update_twarn_lo(int value);

/* parse the current packet and take actions based on the type of request */
void parse_and_send_response();

#endif // PROCESS_PACKET_H_INCLUDED
