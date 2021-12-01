/********************************************************
 * signature.h
 *
 * this file provides function declarations for the SER486
 * final project signature class.
 *
 * Author:   Doug Sandy
 * Date:     10/14/2019
 * Revision: 1.0
 *
 * Copyright(C) 2019, Arizona State University
 * All rights reserved
 *
 */
#ifndef SIGNATURE_H_INCLUDED
#define SIGNATURE_H_INCLUDED

/* a function to sign the final project. */
void signature_set(char*firstname, char*lastname, char*asurite);

/* a function to check if the student has requested the final test */
void check_for_test_start();

#endif // SIGNATURE_H_INCLUDED
