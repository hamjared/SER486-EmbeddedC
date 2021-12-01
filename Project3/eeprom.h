/********************************************************
 * eeprom.h
 *
 * this file provides function declarations for SER486
 * eeprom class member functions.
 *
 * Author:   Doug Sandy
 * Date:     4/18/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */
#ifndef EEPROM_H_INCLUDED
#define EEPROM_H_INCLUDED

/* place the data (specified by buf and size) into the write buffer for later writing to
* the EEPROM.  The addr parameter specifies the location to write the data to.
* This function should not be called when another write is in progress.
*/
void eeprom_writebuf(unsigned int addr, unsigned char *buf, unsigned char size);

/* program the data (specified by buf and size) into the eeprom without use of
* the interrupt service routine.  The addr parameter specifies the location to
* write the data to.
*/
void eeprom_writebuf_noisr(unsigned int addr, unsigned char *buf, unsigned char size);

/* read a specified amount of data (size) from the EEPROM starting at a
* specified address (addr) and places it in the specified buffer (buf).
*/
void eeprom_readbuf(unsigned int addr, unsigned char *buf, unsigned char size);

/* return 0 if the eeprom is writing from its write buffer, otherwise, return 1.*/
int eeprom_isbusy();

#endif // EEPROM_H_INCLUDED
