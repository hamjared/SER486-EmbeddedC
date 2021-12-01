/********************************************************
 * vpd.h
 *
 * SER486 Project 2
 * Fall 2021
 * Written By:  Jared Ham
 *
 This file implements the header file for the vpd class
 */

#ifndef VPD_H_INCLUDED
#define VPD_H_INCLUDED

 typedef struct {
     char token[4];
     char model[12];
     char manufacturer[12];
     char serial_number[12];
     unsigned long manufacture_date;
     unsigned char mac_address[6];
     char country_of_origin[4];
     unsigned char checksum;
 } vpd_struct;
/* initialize vpd data in eeprom */
void vpd_init();




#endif // VPD_H_INCLUDED
