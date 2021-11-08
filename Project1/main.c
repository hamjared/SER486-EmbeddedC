/*
Jared Ham
SER 486 Fall 2021
Project 1
*/

#include "uart.h"
#include "delay.h"
#include "led.h"
#include "rtc.h"




int main(void)
{
    //TODO
    //initialize uart, led and rtc
    uart_init();
    led_init();
    rtc_init();

    // TODO
    // Set default led morse code to OK
    led_set_blink("--- -.-");

    // initialize rtc date/time
    rtc_set_by_datestr("01/01/2019 00:00:00");

    // Print assignment details to console
    uart_writestr("SER 486 Project 1 - Jared Ham\n\r");

       /* get baseline performance */
    signed long c1=0;
    delay_set(1,10000); while (!delay_isdone(1)) { c1++; }

    /* measure performance with led_update */
    signed long c2=0;
    delay_set(1,10000); while (!delay_isdone(1)) { led_update(); c2++; }

    /* display the results */
    uart_writedec32(c1); uart_writestr(" ");
    uart_writedec32(c2); uart_writestr("\r\n");

    //set the delay 1 counter
    delay_set(1,0);

    while(1){
        if(delay_isdone(1)){
            delay_set(1,500);
            uart_writestr(rtc_get_date_string());
            uart_writestr("\r");
        }
        led_update();
    }


    return 0;
}
