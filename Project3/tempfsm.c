/********************************************************
tempfsm.c

Jared Ham
SER 486 Project 3
Fall 2021
 *
 */

 // define states
 #define NORM_1 1
 #define NORM_2 2
 #define NORM_3 3
 #define WARN_HI_1 4
 #define WARN_HI_2 5
 #define CRITICAL_HI 6
 #define WARN_LO_1 7
 #define WARN_LO_2 8
 #define CRITICAL_LO 9

 #include "alarm.h"
 #include "log.h"
 #include "led.h"

 static unsigned char state;


/**
tempfsm_update(int current, int hicrit, int hiwarn, int locrit, int lowarn)

This function updates the Temperature finite state machine, sends alarms if necessary and sets
the led blink pattern: Fast = Critical temp achieved, Slow = Warn temp achieved, None = Normal state

arguments:
    int current : current temperature reading from device
    int hicrit  : critical high temperature
    int hiwarn  : warning high temperature
    int locrit  : critical low temperature
    int lowarn  : warning low temperature

returns:
    nothing
changes:
    Led blink pattern
    Sends device alarms
    Entries are added to the log for alarm events
NOTE:
    make sure to call tempfsm_init before calling this function
*/
void tempfsm_update(int current, int hicrit, int hiwarn, int locrit, int lowarn){

    switch(state){
    case NORM_1:
        if(current >= hiwarn){
            alarm_send(EVENT_HI_WARN);
            log_add_record(EVENT_HI_WARN);
            state = WARN_HI_1;
        }
        else if(current <= lowarn){
            alarm_send(EVENT_LO_WARN);
            log_add_record(EVENT_LO_WARN);
            state = WARN_LO_1;
        }
        break;
    case NORM_2:
        if(current >= hiwarn){
            alarm_send(EVENT_HI_WARN);
            log_add_record(EVENT_HI_WARN);
            state = WARN_HI_1;
        }
        else if(current <= lowarn){
            state = WARN_LO_1;
        }
        break;
    case NORM_3:
        if(current <= lowarn){
            alarm_send(EVENT_LO_WARN);
            log_add_record(EVENT_LO_WARN);
            state = WARN_LO_1;
        }
        else if(current >= hiwarn){
            state = WARN_HI_1;
        }
        break;
    case WARN_HI_1:
        if(current >= hicrit){
            alarm_send(EVENT_HI_ALARM);
            log_add_record(EVENT_HI_ALARM);
            state = CRITICAL_HI;
        }
        else if(current < hiwarn){
            state = NORM_3;
        }
        break;
    case WARN_HI_2:
        if(current >= hicrit){
            state = CRITICAL_HI;
        }
        else if(current < hiwarn){
            state = NORM_3;
        }
        break;
    case CRITICAL_HI:
        if(current < hicrit){
            state = WARN_HI_2;
        }
        break;
    case WARN_LO_1:
        if(current <= locrit){
            alarm_send(EVENT_LO_ALARM);
            log_add_record(EVENT_LO_ALARM);
            state = CRITICAL_LO;
        }
        else if(current > lowarn){
            state = NORM_2;
        }
        break;
    case WARN_LO_2:
        if(current <= locrit){
            state = CRITICAL_LO;
        }
        else if(current > lowarn){
            state = NORM_2;
        }
        break;
    case CRITICAL_LO:
        if(current > locrit){
            state = WARN_LO_2;
        }
        break;
    default:
        state = NORM_1;
        break;

    }

    //update logs
    log_update();

    // Set LED to proper blink pattern
    if(state == CRITICAL_HI || state == CRITICAL_LO){
        led_set_blink(".");
    }
    else if (state == WARN_HI_1 || state == WARN_HI_2 || state == WARN_LO_1 || state == WARN_LO_2){
        led_set_blink("-");
    }
    else{
        led_set_blink(" ");
    }

}


/**
tempfsm_reset()

This function resets the state machine to the initial state (normal)

returns:
    nothing
*/
void tempfsm_reset(){

    state = NORM_1;
}

/**
tempfsm_reset()

This function initialize the finite state machine

returns:
    nothing
*/
void tempfsm_init(){

    // set intial state
    state = NORM_1;

}

