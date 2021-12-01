/********************************************************
util.c

Jared Ham
SER 486 Project 3
Fall 2021
 *
 */

#include "config.h"

extern config_struct config;


/**
update_tcrit_hi(int value)

This function update the configuration tcrit_hi limit with the specified value.

arguments:
    int value : value to set config to. Bound checking is performed: twarn_hi < value <= 0x3FF

returns:
    0 if bound checks are met
    1 if not
changes:
    config data in eeprom

*/
int update_tcrit_hi(int value){
    if(value <= config.hi_warn || value > 0x3FF){
        return 1;
    }
    config.hi_alarm = value;
    config_set_modified();
    config_update();

    return 0;
}


/**
update_twarn_hi(int value)

This function update the configuration twarn_hi limit with the specified value.

arguments:
    int value : value to set config to. Bound checking is performed: twarn_lo < value < tcrit_hi

returns:
    0 if bound checks are met
    1 if not
changes:
    config data in eeprom

*/
int update_twarn_hi(int value){
    if(value < config.lo_warn || value > config.hi_alarm){
        return 1;
    }
    config.hi_warn = value;
    config_set_modified();
    config_update();
    return 0;
}


/**
update_tcrit_lo(int value)

This function update the configuration tcrit_lo limit with the specified value.

arguments:
    int value : value to set config to. Bound checking is performed: tcrit_lo < value < twarn_hi

returns:
    0 if bound checks are met
    1 if not
changes:
    config data in eeprom

*/
int update_tcrit_lo(int value){
    if(value < config.lo_warn){
        config.lo_alarm = value;
        config_set_modified();
        config_update();
        return 0;
    }

    return 1;

}


/**
update_twarn_lo(int value)

This function update the configuration twarn_lo limit with the specified value.

arguments:
    int value : value to set config to. Bound checking is performed: value < twarn_lo

returns:
    0 if bound checks are met
    1 if not
changes:
    config data in eeprom

*/
int update_twarn_lo(int value){
    if(value > config.lo_alarm && value < config.hi_warn){
        config.lo_warn = value;
        config_set_modified();
        config_update();
        return 0;
    }
    return 1;
}
