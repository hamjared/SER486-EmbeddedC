/*************************************************************************
* temputil.h
*
Jared Ham
SER 486 Final Project
Fall 2021
*/

#ifndef TEMPUTIL_H_INCLUDED
#define TEMPUTIL_H_INCLUDED

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
int update_tcrit_hi(int value);


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
int update_twarn_hi(int value);


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
int update_tcrit_lo(int value);


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
int update_twarn_lo(int value);

#endif // TEMPUTIL_H_INCLUDED
