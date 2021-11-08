/********************************************************
 * led.h
 *
 * this file provides function declarations for SER486
 * led library functions.  Blink is implemented using
 * a hardware-based timer and finite state machine (FSM)
 * in order to improve system latency characteristics.
 *
 * the blink and update functions utilizes delay(0) for
 * timing functions.  delay(1) is still available for
 * other delay operations.
 *
 * Author:   Doug Sandy
 * Date:     3/1/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */
#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

void led_init();           /* initialize the LED device */
void led_on();             /* turn on the LED */
void led_off();            /* turn off the LED */
int  led_is_on();          /* returns 1 if the LED is on, otherwise, 0 */
void led_set_blink(char*); /* set the Morse Code blink message */
void led_update();         /* update the blink FSM */

#endif // LED_H_INCLUDED
