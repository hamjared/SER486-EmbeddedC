/********************************************************
 * output.c
 *
 * SER486 Assignment 4
 * Spring 2018
 * Written By:  Doug Sandy (instructor)
 * Modified By: Jared Ham
 *
 * this file implements functions associated with SER486
 * homework assignment 3.  The procedures implemented
 * provide led and console output support for debugging
 * of future SER 486 assignments.
 *
 * functions are:
 *    writestr(char *)  - a function to write an ascii
 *                        null-terminated string to the
 *                        avr atmega 328P uart port.
 *                        (instructor provided)
 *
 *    writehex8(unsigned char) - a function to write the
 *                        hexadecimal representation of
 *                        an 8-bit unsigned integer to
 *                        avr atmega 328P uart port.
 *
 *    writehex16(unsigned int) - a function to write the
 *                        hexadecimal representation of
 *                        a 16-bit unsigned integer to
 *                        avr atmega 328P uart port.
 *
 *    blink_led(char *) - a function to send a specific
 *                        blink pattern to the development
 *                        board's user-programmable LED.
 *
 *    delay(unsigned int) - delay code execution for
 *                        the specified amount of milliseconds.
 *                        (instructor provided)
 */

 #include "hw4lib.h"

 /* macro definitions used by delay() */
 #define MSEC_PER_SEC     1000
 #define CLOCKS_PER_LOOP  16
 #define LOOPS_PER_MSEC   ((F_CPU/MSEC_PER_SEC)/CLOCKS_PER_LOOP)

 // macro defintion for PORTB register
 #define PORTB (*((volatile char *)0x25))

/**********************************
 * delay(unsigned int msec)
 *
 * This code delays execution for for
 * the specified amount of milliseconds.
 *
 * arguments:
 *   msec - the amount of milliseconds to delay
 *
 * returns:
 *   nothing
 *
 * changes:
 *   nothing
 *
 * NOTE: this is not ideal coding practice since the
 * amount of time spent in this delay may change with
 * future hardware changes.  Ideally a timer should be
 * used instead of a set of for loops.  Timers will be
 * taught later in the semester.
 */
void delay(unsigned int msec)
{
    volatile unsigned i,j ;  /* volatile prevents loops from being optimized away */

    /* outer loop, loops for specified number of milliseconds */
    for (i=0; i<msec; i++) {
        /* inner loop, loops for 1 millisecond delay */
        for (j=0;j<LOOPS_PER_MSEC;j++) {}
    }
}


/**********************************
 * writestr(char * str)
 *
 * This code writes a null-terminated string
 * to the UART.
 *
 * arguments:
 *   str - a pointer to the beginning of the
 *         string to be printed.
 *
 * returns:
 *   nothing
 *
 * changes:
 *   the state of the uart transmit buffer will
 *   be changed by this function.
 *
 * NOTE: uart_init() should be called this function
 *   is invoked for the first time or unpredictable
 *   UART behavior may result.
 */
void writestr(char * str)
{
    unsigned int i;

    /* loop for each character in the string */
    for (i=0; str[i]!=0;i++) {
        /* output the character to the UART */
        uart_writechar(str[i]);
    }
}

/************************************************************
* STUDENT CODE BELOW THIS POINT
*************************************************************/

/**********************************
* writehex8(unsigned char)

* This code writes out the hex numeric value of the num passed in.
* For example is 0x0A is passed in, 0A would be written to the console
*
* arguments:
*   char - number that you wish to output hex value to the screen for
*
*
* returns:
*    nothing
*
* changes:
*   The state of the uart buffer will be changed.
*
* NOTE: uart_init() should be called this function
*   is invoked for the first time or unpredictable
*   UART behavior may result.
*
 */
void writehex8(unsigned char num)
{
    // extract the most significant 4 bits of the num
    char leftMost4Bits = (num & 0xF0) >> 4;
    // extract the least significant 4 bits of the num
    char rightMost4Bits = num & 0x0F;

    // put the 4 msbs and 4 lsbs into an array so they can be looped over
    int nums[2];
    nums[0] = leftMost4Bits;
    nums[1] = rightMost4Bits;

    // loop over the 4 msbs and 4 lsbs, using a switch statment to output
    // the correct characters to the console
    for(int i = 0; i < 2; i++){
        switch(nums[i]){
        case 0:
            writestr("0");
            break;
        case 1:
            writestr("1");
            break;
        case 2:
            writestr("2");
            break;
        case 3:
            writestr("3");
            break;
        case 4:
            writestr("4");
            break;
        case 5:
            writestr("5");
            break;
        case 6:
            writestr("6");
            break;
        case 7:
            writestr("7");
            break;
        case 8:
            writestr("8");
            break;
        case 9:
            writestr("9");
            break;
        case 10:
            writestr("A");
            break;
        case 11:
            writestr("B");
            break;
        case 12:
            writestr("C");
            break;
        case 13:
            writestr("D");
            break;
        case 14:
            writestr("E");
            break;
        case 15:
            writestr("F");
            break;
        }
    }

}

/**********************************
* writehex16(unsigned int)

* This code writes out the hex numeric value of the num passed in.
* For example is 0xC0DE is passed in, C0DE would be written to the console
*
* arguments:
*   int - number that you wish to output hex value to the screen for
*
*
* returns:
*    nothing
*
* changes:
*   The state of the uart buffer will be changed.
*
* NOTE: uart_init() should be called this function
*   is invoked for the first time or unpredictable
*   UART behavior may result.
*
 */
void writehex16(unsigned int num)
{
    writehex8(num >> 8);
    writehex8(num & 0x00FF);

}

/**********************************
* blink_led(char *)
*
* This code blinks the LED on PB1 with morse code passed in according to the following rules:
*   . turn on the LED for 250ms, followed by the LED off for 100ms.
*   - turn on the LED for 750ms, followed by the LED off for 100ms.
*   <space> LED off for 1000ms.
*
* arguments:
*   char * - NULL terminated string containing only . - <space>
*
* returns:
*    nothing
*
* changes:
*   The state of PB1
*
* NOTE: PB1 must be initialized as an output before calling this function
*       The led must also be configured to turn on when PB1 is high and turn off when
*       PB1 is low for this to function properly
*
 */
void blink_led(char *msg)
{
    while(*msg != '\0'){
        switch(*msg){
        case '-':
            PORTB |= 0x02; // output high on PB1 (LED on)
            delay(750);
            PORTB &= 0xFD; // output low on PB1 (LED off)
            delay(100);
            break;
        case '.':
            PORTB |= 0x02; // output high on PB1 (LED on)
            delay(250);
            PORTB &= 0xFD; // output low on PB1 (LED off)
            delay(100);
            break;
        case ' ':
            PORTB &= 0xFD; // output low on PB1 (LED off)
            delay(1000);
            break;
        }

        msg++;
    }
}
