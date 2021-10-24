/*
Jared Ham
SER 486 Fall 2021
Homework 1
*/
#include "libs/uart.h"

#define PINB (*((volatile char *)0x23))
#define DDRB (*((volatile char *)0x24))
#define PORTB (*((volatile char *)0x25))


void writestr(char* message){
    unsigned int i = 0; // I sure hope our message is less than 65,535 characters, otherwise this might cause problems.
    while(message[i] != '\0'){
        uart_writechar(message[i]);
        i++;
    }
}

void getDigits(int size, char* msg);

void delay(unsigned int milliseconds){

}

int main(void)
{

    // Initialize serial port
    uart_init();

    // Hello world
    writestr("Hello World from Atmega328\n");
    writestr("SER486 -- Homework Assignment 1\n");
    writestr("Jared Ham\n");

    // char array to hold the string representation of the number. 3 chars is enough since we are not expecting triple digit values.
    char msg[3] = "";

    // size of char
    getDigits(8*sizeof(char), msg);
    writestr("char size (bits) = "); writestr(msg); writestr(" bits\n");

    // size of int
    getDigits(8*sizeof(int), msg);
    writestr("int size (bits) = "); writestr(msg); writestr(" bits\n");

    // size of long
    getDigits(8*sizeof(long), msg);
    writestr("long size (bits) = "); writestr(msg); writestr(" bits\n");

    // Now loop forever.
    DDRB |= 0x2; // Set PB1 as an output pin

    while(1){
       delay(2000);
       PINB |= 0x2;
    }



    return 0;
}

void getDigits(int size, char* msg){
    // don't worry - I did not manually type all this out. Sometimes python can be really handy.
    switch(size){
        case 0:
            msg[0] = '0';
            msg[1] = '\0';
            break;
        case 1:
            msg[0] = '1';
            msg[1] = '\0';
            break;
        case 2:
            msg[0] = '2';
            msg[1] = '\0';
            break;
        case 3:
            msg[0] = '3';
            msg[1] = '\0';
            break;
        case 4:
            msg[0] = '4';
            msg[1] = '\0';
            break;
        case 5:
            msg[0] = '5';
            msg[1] = '\0';
            break;
        case 6:
            msg[0] = '6';
            msg[1] = '\0';
            break;
        case 7:
            msg[0] = '7';
            msg[1] = '\0';
            break;
        case 8:
            msg[0] = '8';
            msg[1] = '\0';
            break;
        case 9:
            msg[0] = '9';
            msg[1] = '\0';
            break;
        case 10:
            msg[0] = '1';
            msg[1] = '0';
            msg[2] = '\0';
            break;
        case 11:
            msg[0] = '1';
            msg[1] = '1';
            msg[2] = '\0';
            break;
        case 12:
            msg[0] = '1';
            msg[1] = '2';
            msg[2] = '\0';
            break;
        case 13:
            msg[0] = '1';
            msg[1] = '3';
            msg[2] = '\0';
            break;
        case 14:
            msg[0] = '1';
            msg[1] = '4';
            msg[2] = '\0';
            break;
        case 15:
            msg[0] = '1';
            msg[1] = '5';
            msg[2] = '\0';
            break;
        case 16:
            msg[0] = '1';
            msg[1] = '6';
            msg[2] = '\0';
            break;
        case 17:
            msg[0] = '1';
            msg[1] = '7';
            msg[2] = '\0';
            break;
        case 18:
            msg[0] = '1';
            msg[1] = '8';
            msg[2] = '\0';
            break;
        case 19:
            msg[0] = '1';
            msg[1] = '9';
            msg[2] = '\0';
            break;
        case 20:
            msg[0] = '2';
            msg[1] = '0';
            msg[2] = '\0';
            break;
        case 21:
            msg[0] = '2';
            msg[1] = '1';
            msg[2] = '\0';
            break;
        case 22:
            msg[0] = '2';
            msg[1] = '2';
            msg[2] = '\0';
            break;
        case 23:
            msg[0] = '2';
            msg[1] = '3';
            msg[2] = '\0';
            break;
        case 24:
            msg[0] = '2';
            msg[1] = '4';
            msg[2] = '\0';
            break;
        case 25:
            msg[0] = '2';
            msg[1] = '5';
            msg[2] = '\0';
            break;
        case 26:
            msg[0] = '2';
            msg[1] = '6';
            msg[2] = '\0';
            break;
        case 27:
            msg[0] = '2';
            msg[1] = '7';
            msg[2] = '\0';
            break;
        case 28:
            msg[0] = '2';
            msg[1] = '8';
            msg[2] = '\0';
            break;
        case 29:
            msg[0] = '2';
            msg[1] = '9';
            msg[2] = '\0';
            break;
        case 30:
            msg[0] = '3';
            msg[1] = '0';
            msg[2] = '\0';
            break;
        case 31:
            msg[0] = '3';
            msg[1] = '1';
            msg[2] = '\0';
            break;
        case 32:
            msg[0] = '3';
            msg[1] = '2';
            msg[2] = '\0';
            break;
        case 33:
            msg[0] = '3';
            msg[1] = '3';
            msg[2] = '\0';
            break;
        case 34:
            msg[0] = '3';
            msg[1] = '4';
            msg[2] = '\0';
            break;
        case 35:
            msg[0] = '3';
            msg[1] = '5';
            msg[2] = '\0';
            break;
        case 36:
            msg[0] = '3';
            msg[1] = '6';
            msg[2] = '\0';
            break;
        case 37:
            msg[0] = '3';
            msg[1] = '7';
            msg[2] = '\0';
            break;
        case 38:
            msg[0] = '3';
            msg[1] = '8';
            msg[2] = '\0';
            break;
        case 39:
            msg[0] = '3';
            msg[1] = '9';
            msg[2] = '\0';
            break;
        case 40:
            msg[0] = '4';
            msg[1] = '0';
            msg[2] = '\0';
            break;
        case 41:
            msg[0] = '4';
            msg[1] = '1';
            msg[2] = '\0';
            break;
        case 42:
            msg[0] = '4';
            msg[1] = '2';
            msg[2] = '\0';
            break;
        case 43:
            msg[0] = '4';
            msg[1] = '3';
            msg[2] = '\0';
            break;
        case 44:
            msg[0] = '4';
            msg[1] = '4';
            msg[2] = '\0';
            break;
        case 45:
            msg[0] = '4';
            msg[1] = '5';
            msg[2] = '\0';
            break;
        case 46:
            msg[0] = '4';
            msg[1] = '6';
            msg[2] = '\0';
            break;
        case 47:
            msg[0] = '4';
            msg[1] = '7';
            msg[2] = '\0';
            break;
        case 48:
            msg[0] = '4';
            msg[1] = '8';
            msg[2] = '\0';
            break;
        case 49:
            msg[0] = '4';
            msg[1] = '9';
            msg[2] = '\0';
            break;
        case 50:
            msg[0] = '5';
            msg[1] = '0';
            msg[2] = '\0';
            break;
        case 51:
            msg[0] = '5';
            msg[1] = '1';
            msg[2] = '\0';
            break;
        case 52:
            msg[0] = '5';
            msg[1] = '2';
            msg[2] = '\0';
            break;
        case 53:
            msg[0] = '5';
            msg[1] = '3';
            msg[2] = '\0';
            break;
        case 54:
            msg[0] = '5';
            msg[1] = '4';
            msg[2] = '\0';
            break;
        case 55:
            msg[0] = '5';
            msg[1] = '5';
            msg[2] = '\0';
            break;
        case 56:
            msg[0] = '5';
            msg[1] = '6';
            msg[2] = '\0';
            break;
        case 57:
            msg[0] = '5';
            msg[1] = '7';
            msg[2] = '\0';
            break;
        case 58:
            msg[0] = '5';
            msg[1] = '8';
            msg[2] = '\0';
            break;
        case 59:
            msg[0] = '5';
            msg[1] = '9';
            msg[2] = '\0';
            break;
        case 60:
            msg[0] = '6';
            msg[1] = '0';
            msg[2] = '\0';
            break;
        case 61:
            msg[0] = '6';
            msg[1] = '1';
            msg[2] = '\0';
            break;
        case 62:
            msg[0] = '6';
            msg[1] = '2';
            msg[2] = '\0';
            break;
        case 63:
            msg[0] = '6';
            msg[1] = '3';
            msg[2] = '\0';
            break;
        case 64:
            msg[0] = '6';
            msg[1] = '4';
            msg[2] = '\0';
            break;

    }

}
