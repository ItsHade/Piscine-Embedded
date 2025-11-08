#include <avr/io.h>
#include <util/delay.h>

// mask used to isolate only specific bit
#define BIT0_MASK 0x01
#define BIT2_MASK 0x04

int main(void) 
{
    unsigned int switchPressed = 0;
    // bit 2 of port D (PD2) is set to 0 (input) by default

    // Set bit 0 of port B to output (LED D1)
    DDRB = 0x01;

    // Set bit 2 of port D to ON (SW1)
    PORTD = 0x04;

    while (1)
    {
        if (!(PIND & BIT2_MASK) && switchPressed == 0)
        {
                // inverts every bit of port B (we only use the last one)
                // PORTB = !PORTB; 
                // Using XOR inverts only wanted bit here its bit 0
                PORTB = PORTB ^ BIT0_MASK;
                switchPressed = 1;
        }
        else if (PIND & BIT2_MASK)
            switchPressed = 0;
        _delay_ms(100);
    }
}
