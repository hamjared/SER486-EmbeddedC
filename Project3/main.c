/********************************************************
 * main.c
 *
 * this file provides function implementation for SER486
 * project 3 main code.
 *
 * Author:   Doug Sandy
 * Date:     4/7/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 * This code may not be distributed or used outside the
 * intended purposes for SER486 project 3.
 */
#include "alarm.h"
#include "config.h"
#include "delay.h"
#include "eeprom.h"
#include "led.h"
#include "log.h"
#include "rtc.h"
#include "temp.h"
#include "tempfsm.h"
#include "uart.h"
#include "uartsocket.h"
#include "util.h"
#include "process_packet.h"
#include "vpd.h"
#include "wdt.h"


/* global variables */
int current_temperature = 88;  /* the most recent temperature reading */

/* macros to convert command line macro definition of name to a string */
#define STRINGIZE(x) #x
#define STRINGIZE_VALUE(x) STRINGIZE(x)

/********************************************************************
* main()
*
* The main function for SER486 project 3. This function implements a
* control loop design pattern with "RMS" scheduling of a fixed set
* of tasks in order to provide a temperature sensor endpoint to an
* industrial IOT system. Ethernet sockets are not yet supported -
* all transactions are carried out over the uart debug port.
*/
int main(void)
{
    /* initialize the hardware and device state */
    config_init();
    vpd_init();
    rtc_init();
    log_init();
    alarm_init();
    uart_init();
    led_init();
    temp_init();
    wdt_init();
    tempfsm_init();

    unsigned char connected = 0;  /* 1 if socket is connected, otherwise, 0 */

    /* print the banner to the debug port */
    uart_writestr("SER486 Project 3 - Integration\r\n");
    uart_writestr( STRINGIZE_VALUE(YOURNAME) );
    uart_writestr("\r\n");

    /* log the startup event */
    log_add_record(EVENT_STARTUP);

    /* start the first temperature reading and wait 5 seconds before reading again
    * this long delay ensures that the temperature spike during startup does not
    * trigger any false alarms.
    */
    temp_start();
    delay_set(1,5000);

    /* control loop */
    while(1) {
       /* reset  the watchdog timer every loop */
       wdt_reset();

       /* update the led state based on the blink code */
       led_update();

       /* if the temperature sensor delay is done, update the current temperature
       * from the temperature sensor, update the temperature sensor finite state
       * machine (which provides hysteresis) and send any temperature sensor
       * alarms (from FSM update).
       */
       if (delay_isdone(1)) {
          /* read the temperature sensor */
          current_temperature = temp_get();

          tempfsm_update(current_temperature,config.hi_alarm,config.hi_warn,config.lo_alarm,config.lo_warn);

          /* restart the temperature sensor delay to trigger in 1 second */
          temp_start();
          delay_set(1,1000);
       } else if (connected == 0) {
          /* if the connection state is 0 (unconnected), wait for the socket to connect */
          if (uartsocket_is_connected()) {
             /* socket is now connected - print a message to the uart debug port and update our connection status */
             uart_writestr("Connection Established\r\n");
             connected = 1;
          }
       } else if (uartsocket_is_packet_available()) {
           /* here if we are connected and a packet is available for processing.
           * Process the packet, update the device state and send the appropriate response to the client
           */
           parse_and_send_response();
           if (!uartsocket_is_connected()) {
              /* if connection is lost, print a debug message and update the connection state */
              uartsocket_writestr("Connection Lost\r\n");
              connected = 0;
           }
       } else {
          /* update any pending write backs */
          if (!eeprom_isbusy()) log_update();
          if (!eeprom_isbusy()) config_update();
       }
    }

    return 0;
}
