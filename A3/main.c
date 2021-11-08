/*
Jared Ham
SER 486 Fall 2021
Homework 3
This file implements HW3 for SER 486.
It Does the following:
    Initialize uart for serial output
    Sets PB1 as an output
    Prints HW assignment and my name to console
    Calls writehex8 and writehex16 to display numeric value of number in hex
    Loops forever blinking the led in morse code for O K
*/

#include "hw4lib.h"
#include "output.h"

// Set up variable names to reference registers on the 328p microcontroller
#define PINB (*((volatile char *)0x23))
#define DDRB (*((volatile char *)0x24))

int main(void)
{
    // Do the required initialization
    uart_init(); // Call uart_init to intialize ability to write messages to console
    DDRB |= 0x2; // Set PB1 as an output pin

    //Output assignment details
    writestr("SER486 HW3 - Jared Ham\n\r"); // \r is required by console to display the carriage return

    // Now call the writeHex functions to print out the required sections for part 2 of the HW
    writehex8(0x0A);
    writestr("\n\r");

    writehex16(0xC0DE);
    writestr("\n\r");

    // loop forever blinking the led with morse code
    while(1){
        blink_led("--- -.-"); // Morse Code = O K
    }

    return 0;
}
