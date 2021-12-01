/********************************************************
 * alarm.h
 *
 * this file provides function declarations for SER486
 * alarm class library functions.
 *
 * Author:   Doug Sandy
 * Date:     4/5/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */
#ifndef ALARM_H_INCLUDED
#define ALARM_H_INCLUDED

/* initialize the alarm class */
void alarm_init();

/* send an alarm with the specified number.  The number should match
* the event type found in log.h.  Alarms will be sent to both the
* local debug port as well as a UDP multicast message on the local
* area network.
*/
void alarm_send(unsigned eventnum);

#endif // ALARM_H_INCLUDED
