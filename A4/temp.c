/********************************************************
 * temp.c
 *
 * SER486 Assignment 4
 * Fall 2021
 * Written By:  Jared Ham
 *
 * this file implements functions associated with SER486
 * homework assignment 4.  The procedures implemented
 * provide support for using the internal temperature sensor
 * of the Atmega328p
 *
 * functions are:
 *    void temp_init();
 *
 *    int temp_is_data_ready();
 *
 *    void temp_start();
 *
 *    long temp_get();
 */


 // define registers of the Atmega 328p used in this file
#define ADMUX (*((volatile char *)0x7C))
#define ADCSRA (*((volatile char *)0x7A))
#define ADCL (*((volatile char *)0x78))
#define ADCH (*((volatile char *)0x79))


/****
temp_init()

This code initializes the ADC on the Atmega 328p to read the
internal temperature sensor with a reference voltage of 1.1v

returns:
    nothing

changes:
    Changes the ADMUX register(0x7C), setting the ADC to read the internal
    temperature sensor

*/
void temp_init(){
    ADMUX = 0xC8; // Internal Voltage Reference (1.1v) and Internal Temp sensor
}

/****
temp_start()

This code starts the conversion process of the ADC

returns:
    nothing

changes:
    Changes the ADCSRA register

NOTE: This function assumes that the ADMUX register is already setup to a reference voltage of 1.1v and to read
    from the internal temp sensor. temp_init() can be called to set this configuration
*/
void temp_start(){
    // set the prescalar to 64
    ADCSRA |= 0x06;

    // enable the ADC by setting bit 7 of ADCSRA
    ADCSRA |= 0x80;

    // start the conversion by setting bit 6 of ADCSRA to 1
    ADCSRA |= 0x40;

}

/****
temp_is_data_ready()

This code checks the status of the ADC conversion.

returns:
    int:
        0 if the ADC is still converting
        1 if the ADC is done converting

changes:

NOTE: This function assumes that temp_start was called before calling this method
*/
int temp_is_data_ready(){
    int status = (ADCSRA & 0x40) >> 6;
    return status == 0;
}


/****
temp_get()

This returns the current internal temperature in degrees Celsius from the ADC.

returns:
    long - The internal temperature reading from the ADC

changes:

NOTE: This function assumes that the caller has verified that the ADC has finished a conversion
*/
long temp_get(){
    long cur_adcl = ADCL & 0x00FF;
    long cur_adch = ADCH << 8;
    long reading = cur_adcl | cur_adch;
    long  degrees = (long)((reading * 101.0/100.0)) - 273.0;
    return degrees;
}
