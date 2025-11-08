#include <avr/io.h>

int main(void) 
{
    // DDR is Direction register (sets as input 0 or output 1)
    // DDRB is the port B data direction register
    // each port is a byte (8 bits)
    // set the last bit of port B to 1
    DDRB = 0x01;

    // PORT is Data register (sets the state of that port OFF 0 or ON 1)
    // PORTB is the port B data register
    // set the last bit of port B to 1
    PORTB = 0x01;
}
