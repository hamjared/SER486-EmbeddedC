/********************************************************
 * util.c
 *
 * SER486 Project 2
 * Fall 2021
 * Written By:  Jared Ham
 *
 This file implements the config class which reads and stores config data in eeprom
 */

 #include "eeprom.h"
 #include "util.h"
 #include "config.h"


 #define CONFIG_EEPROM_ADDRESS 0x040



static config_struct defaults = {"ASU", 0x3FF, 0x3FE, 0x0000, 0x0001, 0, {192,168,1,100}, 0 };

config_struct config = {0};

 static unsigned char modified = 0;



  /**
config_is_data_valid()

Checks if config data is valid


arguments:
    None

returns:
    1 if data is valid, 0 if not.

changes:

NOTE:

*/
 static unsigned int config_is_data_valid(){
     // check if token = ASU
     int result = 1;

     if(config.token[0] != 'A' || config.token[1] != 'S' || config.token[2] != 'U' || config.token[3] != '\0'){
        result = 0;
     }

     if(is_checksum_valid((unsigned char*)&config,sizeof(config)) == 0){
        result = 0;
     }

     return result;
 }

 /**
config_write_defaults()

writes default config values to eeprom


arguments:
    None

returns:
    nothing

changes:
    Config data in eeprom is updated

NOTE:

*/
 static void config_write_defaults(){
     update_checksum((unsigned char*)&defaults, sizeof(defaults));
     eeprom_writebuf(CONFIG_EEPROM_ADDRESS, (unsigned char*)&defaults, sizeof(defaults));
 }


/**
config_init()

 initializes config member data from the EEPROM. If config data is invalid
after initialization, the EEPROM is written to “factory defaults”, and the config data is
reinitialized from the new EEPROM values.


arguments:
    None

returns:
    nothing

changes:
    If config data in eeprom is not valid, the config eeprom data is updated

NOTE:

*/
void config_init(){
    // block wait if eeprom is busy
    while(eeprom_isbusy());

    // initialize config with data from eeprom
    eeprom_readbuf(CONFIG_EEPROM_ADDRESS, (unsigned char*)&config, sizeof(config_struct));

    //check if data is valid, if it is valid we are done here
    if(config_is_data_valid() > 0){
        return;
    }

    // if config data not valid, set to defaults
    update_checksum((unsigned char*)&defaults, sizeof(defaults));
    config_write_defaults();

    // pull data from eeprom again into config
    eeprom_readbuf(CONFIG_EEPROM_ADDRESS, (unsigned char*)&config, sizeof(config_struct));


    // clear the modified flag
    modified = 0;

}

/*if config has been modified and eeprom is not busy, write all of the
configuration data to the eeprom write buffer */
/**
config_update()

if config has been modified and eeprom is not busy, write all of the
configuration data to the eeprom write buffer

arguments:
    None

returns:
    nothing

changes:
    If config struct has been updated, the config eeprom data is updated

NOTE:

*/
void config_update(){
    // if eeprom is busy, return
    if(eeprom_isbusy() > 0){
        return;
    }

    // if config data is not modified return
    if(! modified){
        return;
    }

    // Update the config data with what is in the config struct

    update_checksum((unsigned char*)&config, sizeof(config));
    eeprom_writebuf(CONFIG_EEPROM_ADDRESS, (unsigned char*)&config, sizeof(config));

    // clear the modified flag
    modified = 0;

}


/**
config_set_modified()

set the modified flag. This function should be called any time the
config data is modified

arguments:
    None

returns:
    nothing

changes:
    None
NOTE:

*/
void config_set_modified(){
    modified = 1;
}
