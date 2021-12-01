/********************************************************
 * temp.h
 *
 * this file provides function declarations for SER486
 * temp (temperature sensor) class member functions.
 *
 * Author:   Doug Sandy
 * Date:     4/18/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */#ifndef TEMP_H_INCLUDED
#define TEMP_H_INCLUDED

void temp_init();              /* initialize the ADC for temperature reading */
int  temp_is_data_ready();     /* check to see if the most recent conversion has completed */
void temp_start();             /* start a new conversion */
int  temp_get();               /* return the value of the temperature sensor reading */

#endif // TEMP_H_INCLUDED
