#include <avr/io.h>
#include <util/delay.h>

// mask used to isolate only bit 2
#define BIT2_MASK 0x04

int main(void) 
{
    // bit 2 of port D (PD2) is set to 0 (input) by default

    // Set bit 0 of port B to output (LED D1)
    DDRB = 0x01;

    // Set bit 2 of port D to ON (SW1)
    PORTD = 0x04;

    while (1)
    {
        // when the switch is pressed (closing the circuit) the value of the specific bit will be set to 0
        if (!(PIND & BIT2_MASK))
            PORTB = PORTB | 0x01;
        else
            PORTB = PORTB & 0xFE; // 0xFE = 255 so only the last bit is set to off
        _delay_ms(100);
    }
}
