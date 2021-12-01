/********************************************************
 * eeprom.c
 *
 * SER486 Project 2
 * Fall 2021
 * Written By:  Jared Ham
 *
 This file implements the eeprom class which follows the hardware device design pattern.
 Reads are read directly from the device, writes are buffered and written using an interrupt service routine
 */

 #include "eeprom.h"
 // define registers used in this class
#define EEARH (*((volatile char *)0x42))
#define EEARL (*((volatile char *)0x41))
#define EEDR (*((volatile char *)0x40))  // EEPROM data register
#define EECR (*((volatile char *)0x3F)) // EEPROM control register
#define SREG (*((volatile char *)0x5F))

// define bits withing the EECR
#define EERE 0 // EEPROM Read enable
#define EEPE 1  // EEPROM  write enable
#define EEMPE 2 // EEPROM master write enable
#define EERIE 3 // EEPROM Ready Interrupt Enable. Set this bit to enable EEPROM interrupts on vector 22

 // Internal State variables
 static unsigned char writebuf[64];
 static unsigned char bufidx = 0; // index of next write buffer
 static unsigned char writesize = 0; // size of data in write buffer that needs written
 static unsigned int writeaddr = 0;
 static volatile unsigned char write_busy = 0; // 1 when there is data that needs written from the write buffer

#pragma GCC push_options
#pragma GCC optimize ("Os")
/************************************************
* eeprom_unlock
* Unlock the eeprom for writing
* Arguments ...
* Returns ...
* Changes ...
*/
static void eeprom_unlock()
{
    /* Write logical one to EEMPE */
    EECR |= (1<<EEMPE);
    /* Start eeprom write by setting EEPE */
    EECR |= (1<<EEPE);
}
#pragma GCC pop_options
/**
set_eear(unsigned int address)
private internal function used to set the HIGH and LOW registers for the EEAR
*/
static void set_eear(unsigned int address){
    EEARH = address >> 8;
    EEARL = address & 0xFF;
    return;
}


static void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
    // get current global interrupt state
    unsigned char curInterruptState = SREG & 0x80;
    // disable interrupts
    __builtin_avr_cli();

    /* Wait for completion of previous write */
    while(EECR & (1<<EEPE));
    /* Set up address and Data Registers */
    set_eear(uiAddress);
    EEDR = ucData;

    // unlock and start eeprom write
    eeprom_unlock();

    // restore global interrupt state
    SREG |= curInterruptState;

}

static void block_wait_on_write(){
    while(eeprom_isbusy() > 0);
}

void __vector_22(void) __attribute__ ((signal, used, externally_visible));
void __vector_22(void){
    if(bufidx < writesize){
        // write next byte in write buffer to eeprom
        EEPROM_write(writeaddr, writebuf[bufidx]);

        //increment writeaddr
        writeaddr++;
        // increment buffer index
        bufidx++;
    }
    else{
        // Done writing buffer
        // Disable EEPROM interrupts
        EECR &= ~(1<<EERIE);
        // set write_busy = 0
        write_busy = 0;
    }

}


/**
eeprom_writebuf(unsigned int addr, unsigned char * buf, unsigned char size)

 this function places data into the write buffer for later writing to EEPROM.


arguments:
    unsigned int - address of eeprom to write to
    unsigned char* - buffer containing data to be written to eeprom
    unsigned char - size of the buffer to write to eeprom

returns:
    nothing

changes:


NOTE:
    Do not call if a write is already in progress
*/
void eeprom_writebuf(unsigned int addr, unsigned char * buf, unsigned char size){

    // block wait if a write is already in progress
    block_wait_on_write();

    // set the write address
    writeaddr = addr;

    // set write busy = 1
    write_busy = 1;

    // set buffer index = 0
    bufidx = 0;

    //copy buffer passed in to the internal write buffer
    for (int i = 0; i < size; i++){
        writebuf[i] = buf[i];
    }

    // set the internal write size
    writesize = size;

    // enable EEPROM ready interrupts which will call vector22
    EECR |= (1<<EERIE);

}

/**
eeprom_readbuf(unsigned int addr, unsigned char * buf, unsigned char size)

 this function reads the specified number of bytes from eeprom and places data in the buf


arguments:
    unsigned int - address of eeprom to read from
    unsigned char* - buffer to write data to
    unsigned char - number of bytes to read from eeprom

returns:
    nothing

changes:


NOTE:

*/
void eeprom_readbuf(unsigned int addr, unsigned char * buf, unsigned char size){
     // block wait if a write is already in progress
     block_wait_on_write();

     // read contiguous characters from eeprom and place into buf
     for(int i = 0 ; i < size; i++){
        set_eear(addr + i);
        EECR |= (1<<EERE);
        buf[i] = EEDR;
     }




}


/**
eeprom_isbusy()

checks if an eeprom write is currently in progress



arguments:
    None

returns:
    returns 0 if write_busy is 0 otherwise 1

changes:


NOTE:

*/
int eeprom_isbusy(){
    return write_busy == 1;
}
