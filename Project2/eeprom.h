/********************************************************
 * eeprom.h
 *
 * SER486 Project 2
 * Fall 2021
 * Written By:  Jared Ham
 *
 This file implements the header file for the eeprom class which follows the hardware device design pattern.
 Reads are read directly from the device, writes are buffered and written using an interrupt service routine
 */

#ifndef EEPROM_H_INCLUDED
#define EEPROM_H_INCLUDED

/* this function places data into the write buffer for later writing to EEPROM.
Do not call if a write is already in progress */
void eeprom_writebuf(unsigned int addr, unsigned char * buf, unsigned char size);

/* this function reads the specified number of bytes from eeprom and places data in the buf */
void eeprom_readbuf(unsigned int addr, unsigned char * buf, unsigned char size);

/* returns 0 if write_busy is 0 otherwise 1 */
int eeprom_isbusy();



#endif // EEPROM_H_INCLUDED
