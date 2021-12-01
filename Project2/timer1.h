/********************************************************
 * timer1.h
 *
 * this file provides function declarations for SER486
 * timer1 library functions.  Timer 1 is a 16-bit hardware
 * timer implemented by the ATMEGA 328P micro-controller.
 * These functions initialize the timer for a 1second
 * tick rate and provide a 32-bit counter that is incremented
 * with each tick.
 *
 * Author:   Doug Sandy
 * Date:     3/1/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */
#ifndef TIMER1_H_INCLUDED
#define TIMER1_H_INCLUDED

/* initialize timer1 for a 1 second periodic tick */
void timer1_init();

/* return the tick count value */
unsigned long timer1_get();

/* clear the value of the tick counter */
void timer1_clear();

#endif // TIMER1_H_INCLUDED
