/*
Jared Ham
SER 486 Fall 2021
Homework 2
*/

#define PINB (*((volatile char *)0x23))
#define DDRB (*((volatile char *)0x24))

void delay(){
    for(unsigned int i = 0 ; i < 0xFF; i++){
        for(unsigned int j = 0; j < 0xFF; j++){
                for(unsigned int k = 0; k < 0x04; k++){

                }
        }
    }
}

int main(void)
{

    DDRB |= 0x2; // Set PB1 as an output pin

    while(1){
       delay();
       PINB |= 0x2;
    }


    return 0;
}
