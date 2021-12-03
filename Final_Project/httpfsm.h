/*************************************************************************
* httpfsm.h
*
* This code for implementing an http fsm which is used to receive and respond to
http requests received on socket 0.

Jared Ham
SER 486 final project
*/

#ifndef HTTPFSM_H_INCLUDED
#define HTTPFSM_H_INCLUDED


void httpfsm_update();

void httpfsm_init();

void httpfsm_reset();


#endif // HTTPFSM_H_INCLUDED
