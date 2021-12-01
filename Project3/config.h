/********************************************************
 * config.h
 *
 * this file provides declarations for SER486
 * configuration class library functions.
 *
 * Author:   Doug Sandy
 * Date:     4/5/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */
#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED
    typedef struct {
        char          token[4];
        int           hi_alarm;
        int           hi_warn;
        int           lo_alarm;
        int           lo_warn;
        char          use_static_ip;
        unsigned char static_ip[4];
        unsigned char checksum;
    } config_struct;

    /* "public member functions and data" */
    extern config_struct config;

    /* initialize the config object, reading the contents from the eeprom */
    void config_init();

    /* write back the config to the eeprom write buffer. */
    void config_update();

    /* write back the config to the eeprom if the eeprom is not busy.  For flushing the
    * cache during a watchdog event, only the first call to this function will do anything
    * since the cache will no longer be modified after the first call.
    */
    void config_update_noisr();

    /* set the modified flag for the configuration data */
    void config_set_modified();

    /* debug code */
    void config_dump();

    /* private member functions and data
    static unsigned char modified
    int config_is_data_valid()
    void config_update_checksum(char*data)
    */
#endif // CONFIG_H_INCLUDED
