/********************************************************
 * util.h
 *
 * this file provides function declarations for various
 * utility functions for SER486.
 *
 * Author:   Doug Sandy
 * Date:     3/24/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#ifdef __cplusplus
   extern "C" {
#endif
/* update the checksum (last byte) of the specifed data so
* that the sum of all the bytes will be zero
*/
void update_checksum(unsigned char *data, unsigned int dsize);

/* returns 1 if the sum of all the bytes in the data structure is 0.
* Otherwise, returns 0.
*/
int is_checksum_valid(unsigned char *data, unsigned int dsize);

/* dump the contents of the eeprom (instructor provided code) */
void dump_eeprom(unsigned int start_address, unsigned int numbytes);


#ifdef __cplusplus
   }
#endif

#endif // UTIL_H_INCLUDED
