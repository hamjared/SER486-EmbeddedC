/********************************************************
 * log.h
 *
 * this file provides function declarations for SER486
 * log library functions.  The system event log is
 * is implemented as a circular queue with overwrite
 * of the oldest entry.  Events are timestamped using
 * the same time format as that used by the rtc.  The
 * only event information that is stored is a single
 * character that identifies the type of event that
 * that occurred.
 *
 * Author:   Doug Sandy
 * Date:     3/24/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */
#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED
    /* macro definitions for event types */
    #define EVENT_STARTUP   0x00
    #define EVENT_RESET     0x01
    #define EVENT_WDT       0x02
    #define EVENT_TIMESET   0x03
    #define EVENT_NEWTIME   0x04
    #define EVENT_HI_ALARM  0x05
    #define EVENT_HI_WARN   0x06
    #define EVENT_LO_ALARM  0x07
    #define EVENT_LO_WARN   0x08
    #define EVENT_SHUTDOWN  0x09
    #define EVENT_COMERROR  0x0A
    #define EVENT_UNK   0xFF

    /* "public member functions and data" */

    /* read the local copy of the log from the eeprom */
    void log_init();

    /* write back one log entry to the eeprom if the eeprom is not busy */
    void log_update();
    void log_update_noisr();

    /* clear all entries from the event log and mark the log as modified */
    void log_clear();

    /* add a log record to the log, automatically calling the rtc to get
    * the timestamp.  The new record is marked as modified.
    */
    void log_add_record(unsigned char eventnum);

    /* Provides the values for the time and event for the specified log record
    * (if it exist).  Returns 0 if the log entry does not exist.  Otherwise, returns 1
    */
    int  log_get_record(unsigned long index, unsigned long *time, unsigned char *eventnum);

    /* returns the number of valid records within the log */
    unsigned char log_get_num_entries();

#endif // LOG_H_INCLUDED
