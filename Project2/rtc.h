/********************************************************
 * rtc.h
 *
 * this file provides function declarations for SER486
 * real-time-clock (rtc) library functions. The real
 * time clock is implemented in software using a 32-bit
 * count variable which is updated by a 1 second timer
 * tick from hardware timer 1.
 *
 * Author:   Doug Sandy
 * Date:     3/1/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */
#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

/* initialize the rtc and timer1 */
void rtc_init();

/* get the human-readable date string for the current date/time*/
char * rtc_get_date_string();

/* set the date from a string of the format "MM/DD/YYYY HH:MM:SS" */
void rtc_set_by_datestr(char *datestr);

/* get the current value of the 32-bit date/time number */
unsigned long rtc_get_date();

/* return the human-readable date string for the
* specified 32-bit data/time number */
char * rtc_num2datestr(unsigned long num);

#endif // RTC_H_INCLUDED
