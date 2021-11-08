/********************************************************
 * output.h
 *
 * this file provides function declarations for SER486
 * homework assignment 4.  The procedures implemented
 * provide led and console output support for debugging
 * of future SER 486 assignments.
 *
 */
#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

void delay(unsigned int msec);
void writestr(char * str);
void writehex8(unsigned char num);
void writehex16(unsigned int num);
void blink_led(char *msg);

#endif // OUTPUT_H_INCLUDED
