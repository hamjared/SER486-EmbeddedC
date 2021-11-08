/**
Jared Ham
SER 486 Fall 2021
Homework 4
This file implements HW4 for SER 486.
It Does the following:
    Initialize uart for serial output
    Initialize led for use
    Initialize internal temp sensor for use

    Print Assignment details
    Loop forever printing the reading from internal temp sensor every second.
*/

#include "uart.h"
#include "simpledelay.h"
#include "simpleled.h"
#include "temp.h"



int main(void)
{
    // initialize hardware
    uart_init();
    led_init();
    temp_init();

    //Output assignment details
    uart_writestr("SER486 HW4 - Jared Ham\n\r"); // \r is required by console to display the carriage return

    while(1){
        // start the ADC conversion to get the temp
        temp_start();

        // loop until the ADC data is ready
        while(temp_is_data_ready() != 1);

        // get and display the temperature
        uart_writedec32(temp_get());
        uart_writestr("\n\r");

        // delay for 1 second
        delay(1000);
    }


    return 0;
}
