/********************************************************
wdt.c

Jared Ham
SER 486 Project 3
Fall 2021
 *
 */

 #include "config.h"
 #include "led.h"
 #include "log.h"

 // define registers used in this class
#define WDTCSR (*((volatile char *)0x60)) // wdt conrtol register

#define SREG (*((volatile char *)0x5F))


// define WDTCSR Bits
#define WDE 3
#define WDCE 4
#define WDIE 6
#define WPD0 0
#define WPD1 1
#define WPD2 2

/**
Watchdog time out interrupt.
*/
void __vector_6(void) __attribute__ ((signal, used, externally_visible));
void __vector_6(void){
    // turn led on
    led_on();
    led_update();

    //add log entry and write
    log_add_record(EVENT_WDT);
    for(int i = 0; i < log_get_num_entries(); i++){
        log_update_noisr();
    }


    // update config without interrupts
    config_update_noisr();

    // loop waiting for the wdt to do reset
    while(1);
}

/* initialize the watchdog timer for a 2 second timeout and interrupt+reset mode */
#pragma GCC push_options
#pragma GCC optimize ("Os")
void wdt_init(){

    // Disable interrupts
    __builtin_avr_cli();
    // Set WDT CSR for interrupt followed bu system reset
    WDTCSR |= (1 << WDCE) | (1 << WDE);


    // Setup watchdog timer for 2s timeout
    WDTCSR = (1 << WDIE) | (1<<WDE) | (1 << WPD0) | (1 << WPD1) | (1 << WPD2) ;

    //enable global interrupts
    __builtin_avr_sei();



}
#pragma GCC pop_options

/* reset the watchdog timer so that it does not time out. */

void wdt_reset(){
    __builtin_avr_wdr();
}


/* force a system restart (reset only, no interrupt) by disabling the watchdog
* interupt and waiting for a watchdog timeout.
*/
void wdt_force_restart(){

    // disable WDT interrupts
    WDTCSR &= ~(1<<WDIE);

    //loop until wdt timer runs out
    while(1);
}

