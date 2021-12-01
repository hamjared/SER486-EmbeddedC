/********************************************************
 * tempfsm.h
 *
 * this file provides declarations for SER486
 * temperature finite state machine  functions.
 *
 * Author:   Doug Sandy
 * Date:     4/5/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */
#ifndef TEMPFSM_H_INCLUDED
#define TEMPFSM_H_INCLUDED

/* update the state of the temperature sensor finite state machine (provides
* hysteresis).  Sends alarms and updates the led blink based on state transitions */
void tempfsm_update(int current, int hicrit, int hiwarn, int locrit, int lowarn);

/* reset the state machine to the initial state (normal) */
void tempfsm_reset();

/* initialize the finite state machine */
void tempfsm_init();

#endif // TEMPFSM_H_INCLUDED
