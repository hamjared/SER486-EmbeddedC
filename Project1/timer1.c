
#define SREG (*((volatile char *)0x5F))
#define TCCR1A (*((volatile char *)0x80))
#define TCCR1B (*((volatile char *)0x81))
#define TIMSK1 (*((volatile char *)0x6F))
#define OCR1AH (*((volatile char *)0x89))
#define OCR1AL (*((volatile char *)0x88))

static volatile unsigned long count = 0;

static unsigned char get_global_interrupt_state(){
    return (SREG & 0x80);
}

static void set_global_interrupt_state(unsigned char state){
    SREG |= state;
}

/**
This function is what gets called when the timer1 interrupt is triggered.
It increments the count each time it is called
*/
void __vector_11(void) __attribute__ ((signal, used, externally_visible));
void __vector_11(void){
    // increment the count
    count++;
}

/* initialize timer1 for a 1 second periodic tick */
void timer1_init(){

    // prescalar will be 64 and atmega328p has clock of 16mhz
    // 16M/256 = 62.5khz.
    // compare = 62.5k/1 - 1 = 62,499 = 0xF423
    OCR1AH = 0xF4;
    OCR1AL = 0x23;

    // Set CTC mode
    TCCR1B |= 0x08;

    // Set clock divisor
    // need a prescalar of 256 which means setting prescalar
    // bit to 100b.
    TCCR1B |= 0x04;

    // Enable interrupts on output compare A
    TIMSK1 |= 0x02;

}

/* return the tick count value */
unsigned long timer1_get(){

    // Get current global interrupt enable bit state
    unsigned char saved_interrupt_state = get_global_interrupt_state();

    // Disable interrupts
    __builtin_avr_cli();

    // Get the count[n] value
    unsigned long cur_count = count;

    // Restore saved global interrupt state
    set_global_interrupt_state(saved_interrupt_state);

    // return the count
    return cur_count;

}

/* clear the value of the tick counter */
void timer1_clear(){
     // Get current global interrupt enable bit state
    unsigned char saved_interrupt_state = get_global_interrupt_state();

    // Disable interrupts
    __builtin_avr_cli();

    // set count to 0
    count = 0;

    // Restore saved global interrupt state
    set_global_interrupt_state(saved_interrupt_state);

}
