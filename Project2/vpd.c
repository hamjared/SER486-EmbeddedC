/********************************************************
 * vpd.c
 *
 * SER486 Project 2
 * Fall 2021
 * Written By:  Jared Ham
 *
 This file implements the header file for the vpd class
 */

#include "vpd.h"
#include "eeprom.h"
#include "util.h"




static vpd_struct defaults = {"SER", "Jared", "Ham", "123456789", 0, {0x4A, 'a','r', 'H','a','m'}, "USA", 0 };

vpd_struct vpd = {0};


 /**
 vpd_is_data_valid()

 this function checks if the vpd data in the vpa internal value is valid by checking the token and the checksum

arguments:
    None

returns:
    1 if data is valid, 0 otherwise

changes:
    NONE


 */
 static int vpd_is_data_valid(){
     int result = 1;

     if(vpd.token[0] != 'S' || vpd.token[1] != 'E' || vpd.token[2] != 'R' || vpd.token[3] != '\0'){
        result = 0;
     }

     if(! is_checksum_valid((unsigned char*)&vpd,sizeof(vpd))){
        result = 0;
     }

     return result;
 }

  /**
 vpd_write_defaults()

 this function writes the default vpd data to the eeprom buffer

arguments:
    None

returns:
    NONE

changes:
    The first 55 bytes of eeprom are updated


 */
 static void vpd_write_defaults(){
     update_checksum((unsigned char*)&defaults, sizeof(defaults));

     eeprom_writebuf(0, (unsigned char*)&defaults, sizeof(defaults));
 }

  /**
 vpd_init()

 this function initiates the vpd. It first checks the current eeprom data to see if vpd is valid. If not it
 sets the vpd in eeprom to default values

arguments:
    None

returns:
    NONE

changes:
    The first 55 bytes of eeprom is potentially updated


 */
 void vpd_init(){
     // block wait until eeprom is not busy
     while(eeprom_isbusy());

     //initialze vpd data with data from eeprom
     eeprom_readbuf(0, (unsigned char*)&vpd, sizeof(vpd_struct));

     if(vpd_is_data_valid()){
        return;
     }

     vpd_write_defaults();

     while(eeprom_isbusy());
     eeprom_readbuf(0, (unsigned char*)&vpd, sizeof(vpd_struct));



 }
