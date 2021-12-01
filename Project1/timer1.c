/********************************************************
 * timer1.c
 *
 * SER486 Project 1
 * Fall 2021
 * Written By:  Jared Ham
 *
 This file implements a 1 second tick timer. This allows for accurate time keeping,
which the rtc class accomplishes through using this timer1 class


 */
#define SREG (*((volatile char *)0x5F))
#define TCCR1A (*((volatile char *)0x80))
#define TCCR1B (*((volatile char *)0x81))
#define TIMSK1 (*((volatile char *)0x6F))
#define OCR1AH (*((volatile char *)0x89))
#define OCR1AL (*((volatile char *)0x88))

static volatile unsigned long count = 0;

/**
 get_global_interrupt_state()

 arguments
    none

 returns
    The status of global interruprs. 0 = off > 0 = on
 changes
    nothing
*/
static unsigned char get_global_interrupt_state(){
    return (SREG & 0x80);
}

/**
 set_global_interrupt_state(unsigned char state)

 arguments
    unsigned char - the already shifted value to or equal into SREG. Ideally this value would be the value obtained from get_global_interrupt_state()

 returns
    none
 changes
    Changes the global interrupt bit of the SREG register
*/
static void set_global_interrupt_state(unsigned char state){
    SREG |= state;
}

/**
This function is what gets called when the timer1 interrupt is triggered.
It increments the count each time it is called
*/
void __vector_11(void) __attribute__ ((signal, used, externally_visible));
void __vector_11(void){
    // increment the count
    count++;
}

  /****
timer1_init()

This function sets the timer1 registers to a 1s tick rate

arguments:
    none

returns:
    nothing

changes:
    OCR1AH and OCR1AL set for a compare value for 1s tick
    TCCR1B set for CTC mode and for prescalar of 256
    TIMSK1 set to enable interrupts on output compare A
*/
void timer1_init(){

    // prescalar will be 64 and atmega328p has clock of 16mhz
    // 16M/256 = 62.5khz.
    // compare = 62.5k/1 - 1 = 62,499 = 0xF423
    OCR1AH = 0xF4;
    OCR1AL = 0x23;

    // Set CTC mode
    TCCR1B |= 0x08;

    // Set clock divisor
    // need a prescalar of 256 which means setting prescalar
    // bit to 100b.
    TCCR1B |= 0x04;

    // Enable interrupts on output compare A
    TIMSK1 |= 0x02;

}

  /****
timer1_get()

This function returns the current count of the timer

arguments:
    none

returns:
    the current count of the timer. Which should be seconds since clear was called

changes:
    global interrupts are disabled but restored before returning from the function.
*/
unsigned long timer1_get(){

    // Get current global interrupt enable bit state
    unsigned char saved_interrupt_state = get_global_interrupt_state();

    // Disable interrupts
    __builtin_avr_cli();

    // Get the count[n] value
    unsigned long cur_count = count;

    // Restore saved global interrupt state
    set_global_interrupt_state(saved_interrupt_state);

    // return the count
    return cur_count;

}

  /****
timer1_clear()

Resets the count of this timer to 0

arguments:
    none

returns:
    none

changes:
    global interrupts are disabled but restored before returning from the function.
*/
void timer1_clear(){
     // Get current global interrupt enable bit state
    unsigned char saved_interrupt_state = get_global_interrupt_state();

    // Disable interrupts
    __builtin_avr_cli();

    // set count to 0
    count = 0;

    // Restore saved global interrupt state
    set_global_interrupt_state(saved_interrupt_state);

}
