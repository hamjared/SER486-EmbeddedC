/*
main.c

This file implements Project 2 which focused on writing data to eeprom

Jared Ham
SER 486 Fall 2021
Project 2
*/

#include "uart.h"
#include "led.h"
#include "log.h"
#include "rtc.h"
#include "eeprom.h"
#include "util.h"
#include "vpd.h"
#include "config.h"


extern config_struct config;
extern vpd_struct vpd;


int main(void)
{
    //initialize classes
    uart_init();
    config_init();
    led_init();
    log_init();
    rtc_init(); // also initializes timer1
    vpd_init();

    // Print assignment Details
    uart_writestr("SER 486 Project 2 - Jared Ham\n\r");
    uart_writestr(vpd.model);
    uart_writestr("\n\r");
    uart_writestr(vpd.manufacturer);
    uart_writestr("\n\r");
    uart_writestr(config.token);
    uart_writestr("\n\r");
    uart_writehex8(vpd.checksum);

    // set led to blink morse code for OK
    led_set_blink("--- -.- ");

    //set rtc clock start time
    rtc_set_by_datestr("01/01/2019 00:00:00");

    // set config to use static ip
    config.use_static_ip = 1;
    config_set_modified();

    int dumped = 0;

    log_add_record(0xaa);
    log_add_record(0xbb);
    log_add_record(0xcc);

    while(1){
        config_update();
        led_update();
        log_update();


        //instructor provided code below to dump eeprom data;
        if ((!eeprom_isbusy()) && (!dumped)) {
            dump_eeprom(0,0x100);
            dumped=1;
        }
    }


    return 0;
}
