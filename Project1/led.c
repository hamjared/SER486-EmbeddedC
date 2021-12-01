/********************************************************
 * led.c
 *
 * SER486 Project 1
 * Fall 2021
 * Written By:  Jared Ham
 *
 This file implements the led class
 *
 * public functions are:
        void led_set_blink(char*);
        void led_update();
 *
 *
 */

 #define PHASE1 1
 #define PHASE2 2

 #include "led.h"
 #include "delay.h"

 static char* blink_msg;
 static unsigned int blink_pos; // which character of the message is currently being blinked
 static unsigned char blink_state;


/****
set_delay_and_led(char c)

This function sets the led state and delay time for a given character ('.', '-', ' ')

arguments:
    char - the morse code character ('.', '-', ' ')

returns:
    nothing
changes:
    The state of the led
    The limit of the delay 0 timer is changed
*/
 static void set_delay_and_led(char c){
    switch(c){
        case '-':
            led_on();
            delay_set(0,750);
            break;
        case '.':
            led_on();
            delay_set(0,250);
            break;
        case ' ':
            led_off();
            delay_set(0,1000);
            break;
        default:
            led_off();
            delay_set(0,0);
            break;
        }
 }

 /****
led_set_blink(char* msg)

This function sets the message that the led will blink

arguments:
    char* - the message to blink morse code for.

returns:
    nothing

changes:
    the state of the led to off
    The delay 0 timer is set to a limit of 0
*/
 void led_set_blink(char* msg){
     // initialize message
    blink_msg = msg;
    blink_pos = 0;

    // reset FSM
    blink_state = PHASE1;
    delay_set(0,0);
    led_off();
 }

  /****
led_update()

This function updates the status of the led FSM

arguments:
    none

returns:
    nothing

changes:
    the state of the FSM
    The limit of the delay 0 timer
*/
 void led_update(){
     // if the message to blink is empty return
     if(blink_msg == '\0'){
        return;
     }

     // if the delay is not done return
     if(!delay_isdone(0)){
        return;
     }

     // update the finite state machine
     switch(blink_state){
         case PHASE1:
             if(delay_isdone(0) && blink_msg[blink_pos] != ' '){
                // set state to Phase 2 and turn off led for 100 ms.
                blink_state = PHASE2;
                led_off();
                delay_set(0, 100);
             }
             else if(delay_isdone(0) && blink_msg[blink_pos] == ' '){
                if(blink_msg[blink_pos + 1] != 0){
                    blink_pos ++;
                }
                else{
                    blink_pos = 0;
                }
             }
            break;
         case PHASE2:
             if(delay_isdone(0)){
                if(blink_msg[blink_pos+1] != '\0'){
                    blink_pos++;
                }
                else{
                    blink_pos = 0;
                }

                // set the state to PHASE 1 and set the corresponding led state and delay
                blink_state = PHASE1;
                set_delay_and_led(blink_msg[blink_pos]);
             }
            break;
     }
 }
